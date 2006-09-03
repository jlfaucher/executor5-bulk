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
/* REXX Kernel                                                                */
/*                                                                            */
/* Primitive Method Class                                                     */
/*                                                                            */
/******************************************************************************/
#include <stdlib.h>
#include <string.h>
#include "RexxCore.h"
#include "StringClass.hpp"
#include "DirectoryClass.hpp"
#include "RexxNativeCode.hpp"
#include "Package.hpp"

/**
 * Initialize a Rexx native method used as an external
 * function.  This is initialized with sufficient information
 * for the package to re-resolve its entrypoint location
 * after unflatten cycle.
 *
 * @param name       The name of the function.
 * @param proc       Name of the package entry point.
 * @param pack       Name of the library package this was loaded from.
 * @param entryPoint A resolved entry point for this function (can be NULL).
 */
RexxNativeCode::RexxNativeCode(RexxString  *name, RexxString  *proc, RexxString  *pack, PFN entryPoint)
{
  this->entry = entryPoint;            // no resolved entry point yet
  this->index = 0;                     // and the index does not apply either
  // save the library object and the procedure
  OrefSet(this, package, pack);
  OrefSet(this, procedure, proc);
  OrefSet(this, functionName, name);
}


/**
 * Initialize a Rexx native method used as a function.
 * This is initialized with sufficient information
 * for the package to re-resolve its entrypoint location
 * after unflatten cycle.
 *
 * @param proc   Name of the package entry point.
 * @param pack   Name of the library package this was loaded from.
 * @param a      A function activator used to call this function.
 */
RexxNativeCode::RexxNativeCode(RexxString  *proc, RexxString  *pack, FunctionActivator *a)
{
    this->entry = NULL;                  // no resolved entry point yet
    this->index = 0;                     // and the index does not apply either
    // save the library object and the procedure
    OrefSet(this, package, pack);
    OrefSet(this, procedure, proc);
    OrefSet(this, functionName, OREF_NULL);
    OrefSet(this, activator, a);
}

/**
 * Initialize a NativeCode object directly to a function
 * entry point.  These methods cannot be resolved again after
 * a flatten operation.
 *
 * @param entryPoint The function entry point address.
 */
RexxNativeCode::RexxNativeCode(PFN entryPoint)
{
    this->entry = entryPoint;            // no resolved entry point yet
    this->index = 0;                     // and the index does not apply either
                                         // this has neither a library nor a procedure
    OrefSet(this, this->procedure, OREF_NULL);
    OrefSet(this, this->functionName, OREF_NULL);
    OrefSet(this, this->activator, OREF_NULL);
}

void RexxNativeCode::reinit(           /* reinitialize the nmethod entry    */
     PFN entryAddr)                    /* entry point value                 */
/****************************************************************************/
/* Function:  Reinitialize a REXX native method                             */
/****************************************************************************/
{
    // just reinit this
    this->entry = entryAddr;
}

void RexxNativeCode::live()
/******************************************************************************/
/* Function:  Normal garbage collection live marking                          */
/******************************************************************************/
{
  setUpMemoryMark
  memory_mark(this->package);
  memory_mark(this->procedure);
  memory_mark(this->functionName);
  memory_mark(this->activator);
  cleanUpMemoryMark
}

void RexxNativeCode::liveGeneral()
/******************************************************************************/
/* Function:  Generalized object marking                                      */
/******************************************************************************/
{
  setUpMemoryMarkGeneral
  memory_mark_general(this->package);
  memory_mark_general(this->procedure);
  memory_mark_general(this->functionName);
  memory_mark_general(this->activator);
  cleanUpMemoryMarkGeneral
}

void RexxNativeCode::flatten(RexxEnvelope *envelope)
/******************************************************************************/
/* Function:  Flatten an object                                               */
/******************************************************************************/
{
  setUpFlatten(RexxNativeCode)

   flatten_reference(newThis->package, envelope);
   flatten_reference(newThis->procedure, envelope);
   flatten_reference(newThis->functionName, envelope);
                                       /* Set entry to NUll for 2 reasons   */
                                       /* 1st force branch to 0 is not      */
                                       /*restored, 2 used to indicated if   */
                                       /* the method has bee unflattened    */
   newThis->entry = NULL;
   newThis->activator = OREF_NULL;     // activators are never stored either.
  cleanUpFlatten
}

RexxObject * RexxNativeCode::unflatten(RexxEnvelope *envelope)
/******************************************************************************/
/* Function:  unflatten an object                                             */
/******************************************************************************/
{
   // we're being unflattened, so we need to resolve the entry point address
   // a new.  We can be either a function or a method, so we need to do the
   // appropriate resolution.
    if (functionName != OREF_NULL && activator == OREF_NULL)
    {
        // this is a function, so resolve this back to an activator.
        OrefSet(this, activator, ThePackageManager->resolveFunction(functionName, package, procedure));
    }
    // unresolved native library methods?
    else if (procedure != OREF_NULL && entry == NULL)
    {
        entry = ThePackageManager->resolveMethodEntry(package, procedure);
    }
    return (RexxObject *)this;          /* return ourself.                   */
}


void * RexxNativeCode::operator new(
     size_t      size)                 /* object size                       */
/****************************************************************************/
/* Function:  Create a new Native method                                    */
/****************************************************************************/
{
    return new_object(size, TheNativeCodeBehaviour);
}


/**
 * Get a function activator for this function that matches the
 * call style of the function.  If this native method is not
 * a function-sytle method, OREF_NULL is returned.
 *
 * @return A function activator appropriate for this method.
 */
FunctionActivator *RexxNativeCode::getFunctionActivator()
{
    // don't have an activator yet?
    if (activator != OREF_NULL)
    {
        // is this actually a function routine?  Can't call this as a function.
        if (functionName == OREF_NULL)
        {
            return OREF_NULL;
        }
        // this is a function, so resolve this back to an activator.
        OrefSet(this, activator, ThePackageManager->resolveFunction(functionName, package, procedure));
    }
    // return the activator.
    return activator;
}
