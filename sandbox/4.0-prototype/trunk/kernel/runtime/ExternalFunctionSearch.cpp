/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 1995, 2004 IBM Corporation. All rights reserved.             */
/* Copyright (c) 2005-2006 Rexx Language Association. All rights reserved.    */
/*                                                                            */
/* This program and the accompanying materials are made available under       */
/* the terms of the Common Public License v1.0 which accompanies this         */
/* distribution. A copy is also available at the following address:           */
/* http://www.ibm.com/developerworks/oss/CPLv1.0.htm                          */
/*                                                                            */
/* Redistribution and use in source and binary forms, with or                 */
/* without modification, are permitted provided that the following            */
/* conditions are met:                                                        */
/*                                                                            */
/* Redistributions of source code must retain the above copyright             */
/* notice, this list of conditions and the following disclaimer.              */
/* Redistributions in binary form must reproduce the above copyright          */
/* notice, this list of conditions and the following disclaimer in            */
/* the documentation and/or other materials provided with the distribution.   */
/*                                                                            */
/* Neither the name of Rexx Language Association nor the names                */
/* of its contributors may be used to endorse or promote products             */
/* derived from this software without specific prior written permission.      */
/*                                                                            */
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS        */
/* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT          */
/* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS          */
/* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT   */
/* OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,      */
/* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED   */
/* TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,        */
/* OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY     */
/* OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING    */
/* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS         */
/* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.               */
/*                                                                            */
/*----------------------------------------------------------------------------*/
/******************************************************************************/
/*  Methods to resolve external function calls...actually methods of          */
/*  RexxActivation in a separate file because of size.                        */
/******************************************************************************/

#include "RexxCore.h"                         /* global REXX definitions        */
#include "StringClass.hpp"
#include "ArrayClass.hpp"
#include "RexxActivity.hpp"
#include "RexxActivation.hpp"
#include "MethodClass.hpp"
#include "SourceFile.hpp"
#include "oorexx.h"                         /* Include REXX header            */
#include "InternalAPI.hpp"                  /* Get private REXXAPI API's      */
#include "FunctionActivator.hpp"
#include "Interpreter.hpp"
#include "Package.hpp"

#define MS_PREORDER   0x01                  /* Macro Space Pre-Search         */
#define MS_POSTORDER  0x02                  /* Macro Space Post-Search        */
/******************************************************************************/
/* Name:       callMacroSpaceFunction                                         */
/*                                                                            */
/* Function:   Searches for a function within the REXX macrospace.  If the    */
/*             target function is found, it executes the function and passes  */
/*             back the rc in result.                                         */
/******************************************************************************/
bool RexxActivation::callMacroSpaceFunction(
    RexxString     * target,             /* Name of external function         */
    RexxObject    ** arguments,          /* Argument array                    */
    size_t           argcount,           /* the count of arguments            */
    RexxString     * calltype,           /* Type of call                      */
    int              order,              /* Pre/Post order search flag        */
    ProtectedObject &result )            /* Result of function call           */
{
    RexxUnsignedNumber position;          /* located macro search position     */
    stringchar_t *macroName;              /* ASCII-Z name version              */
    RexxMethod * routine;                 /* method to execute                 */

    macroName = target->getStringData();  /* point to the string data          */
                                          /* did we find this one?             */
    if (ooRexxQueryMacro((RexxStringPointer)macroName, &position) == 0)
    {
        /* but not at the right time?        */
        if (order == MS_PREORDER && position == RXMACRO_SEARCH_AFTER)
        {
            return false;                    /* didn't really find this           */
        }
        /* get image of function             */
        routine = RexxMethod::getMacroCode(target);
        // not restoreable?
        if (routine == OREF_NULL)
        {
            // this is a call failure
            return false;
        }

        // create a function activator for this
        FunctionActivator *activator = new RexxFunctionActivator(target, routine);
        RexxInterpreter::addCachedExternalFunction(target, activator);

        // and call this now
        activator->call(this, arguments, argcount, calltype, result);
        return true;                       /* return success we found it flag   */
    }
    return false;                        /* nope, nothing to find here        */
}

/******************************************************************************/
/* Name:       callRegisteredFunction                                         */
/*                                                                            */
/* Arguments:  target - Name of external function (string REXXOBJECT)         */
/*             argarray - Argument array (array REXXOBJECT)                   */
/*             calltype - Type of call (string REXXOBJECT)                    */
/*                                                                            */
/* Returned:   rc - Boolean return code:                                      */
/*                    0 means the function wasn't registered                  */
/*                    1 means we found and executed the registered function   */
/*             result - Result from running registered function               */
/*                                                                            */
/* Notes:      Queries the REXX External Function API to see if the target    */
/*             function is registered.  If it is, it asks the API to invoke   */
/*             the function, passing the rc back in result.                   */
/******************************************************************************/
bool RexxActivation::callRegisteredFunction(
    RexxString     * target,             /* Name of external function         */
    RexxObject    ** arguments,          /* Argument array                    */
    size_t           argcount,           /* the count of arguments            */
    RexxString     * calltype,           /* Type of call                      */
    ProtectedObject &result )            /* Result of function call           */
{
    RexxCallback entryPoint;             // resolved entry point for this function
    RexxNumber   style;                  // the style of interface call


    // see if this was in a loaded package
    FunctionActivator *activator = ThePackageManager->findFunction(target);
    if (activator != OREF_NULL)
    {
        // add this to the global list so we can go right there next time
        RexxInterpreter::addCachedExternalFunction(target, activator);
        // and call this now
        activator->call(this, arguments, argcount, calltype, result);
        return true;
    }

    stringchar_t *funcname = target->getStringData();       /* point to the function name        */
    // see if it is registered
    if (ooRexxResolveFunction((RexxStringPointer)funcname, NULL, &entryPoint, &style) != 0)
    {
        return false;
    }

    // now create the appropriate activator for this call type
    switch (style)
    {
        // classic string-oriented function call
        case FUNCTION_CLASSIC_STYLE:
        {
            activator = new ClassicNativeActivator(target, (PFN)entryPoint);
            break;
        }

        // arguments/results handled as objects, with a context
        case FUNCTION_OBJECT_STYLE:
        {
            activator = new ObjectNativeActivator(target, (PFN)entryPoint);
            break;
        }

        case FUNCTION_TYPED_STYLE:
        {
            activator = new TypedNativeActivator(target, (PFN)entryPoint);
            break;
        }
        // unknown type, consider this a failure
        default:
            return false;
    }
    // add this to the global list so we can go right there next time
    RexxInterpreter::addCachedExternalFunction(target, activator);
    // and call this now
    activator->call(this, arguments, argcount, calltype, result);

    return true;
}



/******************************************************************************/
/* Name:       externalFunctionCall                                           */
/*                                                                            */
/* Notes:      Handles searching for and executing an external function.  The */
/*             search order is:                                               */
/*               1) Macro-space pre-order functions                           */
/*               2) Registered external functions                             */
/*               3) REXX programs with same extension (if applicable)         */
/*               4) REXX programs with default extension                      */
/*               5) Macro-space post-order functions                          */
/******************************************************************************/
bool RexxActivation::externalFunctionCall(
    RexxString     * target,             /* Name of external function         */
    RexxString     * parent,             /* Parent program                    */
    RexxObject    ** arguments,          /* Argument array                    */
    size_t           argcount,           /* count of arguments                */
    RexxString     * calltype,           /* Type of call                      */
    ProtectedObject &result)
{
    if (!this->callMacroSpaceFunction(target, arguments, argcount, calltype, MS_PREORDER, result))
    {
        /* no luck try for a registered func */
        if (!this->callRegisteredFunction(target, arguments, argcount, calltype, result))
        {
            /* no go for an external file        */
            if (!this->callExternalRexx(target, parent, arguments, argcount, calltype, result))
            {
                /* last shot, post-order macro space */
                /* function.  If still not found,    */
                /* then raise an error               */
                if (!this->callMacroSpaceFunction(target, arguments, argcount, calltype, MS_POSTORDER, result))
                {
                    return false;
                }
            }
        }
    }
    return true;                         /* return result                     */
}


