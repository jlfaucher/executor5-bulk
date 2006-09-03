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
/* Implementation of the InterpreterInstance class                            */
/*                                                                            */
/******************************************************************************/

#include "RexxCore.h"
#include "ExitHandler.hpp"
#include "rexx.h"
#include "SysInterpreter.hpp"
#include "InternalAPI.hpp"

bool ExitHandler::call(RexxActivity *activity, int function, int subfunction, void *parms)
{
    switch (callType)
    {
        // old style exit call
        case EXIT_CLASSIC_STYLE:
        {
            // marshall the structures and call the old way
            return legacyCall(activity, function, subfunction, parms);
        }

#ifndef __REXX64__
        // The new style call
        case EXIT_OBJECT_STYLE:
        {
            ExitContext context;
            // create an exit context for this
            activity->createExitContext(&context);

            ooRexxExitHandler *handler = (ooRexxExitHandler *)entryPoint;
                                               /* Exit loaded successfully;  */
                                               /* let it set the return code */
            int result =  (int)(*handler)(&context.threadContext, function, subfunction, parms);

            // now destroy this context..., which also reacquires access and checks for raised conditions
            activity->destroyExitContext(&context);
            return result != 0;
        }
#endif
    }
    return true;
}

/**
 * Resolve a classic-style exit handler to the actual target
 * entry point address and invocation style.
 *
 * @param name   The registered exit name.
 */
void ExitHandler::resolve(char *name)
{
    ooRexxResolveExit((RexxStringPointer)name, &entryPoint, &callType);
}
