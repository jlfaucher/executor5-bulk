/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2011-2011 Rexx Language Association. All rights reserved.    */
/*                                                                            */
/* This program and the accompanying materials are made available under       */
/* the terms of the Common Public License v1.0 which accompanies this         */
/* distribution. A copy is also available at the following address:           */
/* http://www.oorexx.org/license.html                                         */
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

/**
 * myFile.cpp
 *
 * Contains the package entry point, routine and method tables for the native
 * API, some example routine and methods, and DLLMain().
 *
 * Copy this file and rename to your external function library.  Fill in your
 * code.
 */


#include <windows.h>
#include <CommCtrl.h>
#include <stdio.h>
#include "oorexxapi.h"


#ifdef __cplusplus
extern "C" {
#endif

BOOL REXXENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    if ( fdwReason == DLL_PROCESS_ATTACH )
    {
        // Your code if needed.
    }
    else if ( fdwReason == DLL_PROCESS_DETACH )
    {
        // Your code if needed.
    }
    return(TRUE);
}

#ifdef __cplusplus
}
#endif

/**
 * Example routine.  Delete / rename / copy as you prefer.
 *
 */
RexxRoutine0(int32_t, myRoutine_name)
{
    printf("Entered myRoutine routine.\n");
    return 0;
}


/**
 * Example method.  Delete / rename / copy as you prefer.
 *
 */
RexxMethod1(RexxObjectPtr, myMethod_name, OSELF, self)
{
    printf("Entered myMethod method.\n");
    return NULLOBJECT;
}


REXX_TYPED_ROUTINE_PROTOTYPE(myRoutine_name);

RexxRoutineEntry myFile_functions[] =
{
    REXX_TYPED_ROUTINE(myRoutine_name,      myRoutine_name),

    REXX_LAST_ROUTINE()
};


REXX_METHOD_PROTOTYPE(myMethod_name);

RexxMethodEntry myFile_methods[] =
{
    REXX_METHOD(myMethod_name,     myMethod_name),

    REXX_LAST_METHOD()
};

RexxPackageEntry myFile_package_entry =
{
    STANDARD_PACKAGE_HEADER
    REXX_INTERPRETER_4_0_0,              // needs at least the 4.0.0 interpreter
    "myFile",                            // name of the package
    "1.0.0",                             // package information
    NULL,                                // no load/unload functions
    NULL,
    myFile_functions,                    // the exported functions
    myFile_methods                       // the exported methods
};

// package loading stub.
OOREXX_GET_PACKAGE(myFile);
