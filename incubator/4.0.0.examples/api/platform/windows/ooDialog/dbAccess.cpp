/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2008-2009 Rexx Language Association. All rights reserved.    */
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
#define _WIN32_WINNT    0x0501
#define WINVER          0x0501

#include <windows.h>
#include <stdio.h>
#include "oorexxapi.h"
#include "dbAccess.h"


/* This routine simulates the opening up of a data base. */
RexxRoutine0(int, db_open)
{
    int rc = USER_GRANTED_ACCESS_RC;

    for ( int i = 0; i < COUNT_USERS; i++ )
    {
        if ( users[i] == NULL )
        {
            context->RaiseException1(Rexx_Error_System_service_user_defined,
                                     context->CString(DATABASE_INVALID_STATE_EXCEPTION));
            rc = DATABASE_ERROR_RC;
        }
    }

    if ( rc == USER_GRANTED_ACCESS_RC )
    {
        for ( int i = 0; i < COUNT_USERS; i++ )
        {
            if ( passwords[i] == NULL )
            {
                context->RaiseException1(Rexx_Error_System_service_user_defined,
                                         context->CString(DATABASE_INVALID_STATE_EXCEPTION));
                rc = DATABASE_ERROR_RC;
            }
        }
    }

    if ( rc == USER_GRANTED_ACCESS_RC )
    {
        db_opened = true;
    }

    return rc;
}

RexxMethod2(logical_t, ld_verifyUser, CSTRING, name, OSELF, self)
{
    int internalID = findUser(name);
    return (internalID != NO_SUCH_USER);
}


RexxMethod3(logical_t, ld_verifyPassword, CSTRING, user, CSTRING, password, OSELF, self)
{
    //RexxMethodContext *context;
    int internalID = findUser(user);
    if ( internalID != NO_SUCH_USER )
    {
        return (strcmp(passwords[internalID], password) == 0);
    }
    return 0;
}


// Build the list of entry points for the routines:
RexxRoutineEntry dbAccess_functions[] =
{
    REXX_TYPED_ROUTINE(db_open, db_open),
    REXX_LAST_ROUTINE()
};

// Build the list of entry points for the methods:
RexxMethodEntry dbAccess_methods[] = {
    REXX_METHOD(ld_verifyUser,     ld_verifyUser),
    REXX_METHOD(ld_verifyPassword, ld_verifyPassword),
    REXX_LAST_METHOD()
};

RexxPackageEntry dbAccess_package_entry =
{
    STANDARD_PACKAGE_HEADER
    REXX_INTERPRETER_4_0_0,              // anything after 4.0.0 will work
    "dbAccess",                          // name of the package
    "4.0",                               // package information
    NULL,                                // no load/unload functions
    NULL,
    dbAccess_functions,                  // the exported functions
    dbAccess_methods                     // the exported methods
};

// The package loading stub.  OOREXX_GET_PACKAGE() is a macro that, basically,
// defines a function whose name is RexxGetPackage().  On Windows, this
// function, RexxGetPackage(), must be in the exports list of the .def file used
// when linking this library.
OOREXX_GET_PACKAGE(dbAccess);

