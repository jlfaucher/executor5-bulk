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

/**
 * The include header for dbAccess, used to fake access to a real database.  The
 * users and passwords are just in hard-coded arrays.  The password for each
 * user ID is simply the user ID.  Except for Rick's because I figured he'd
 * cheat and look at this file first thing.  ;-)
 */

#ifndef Included_dbAccess
#define Included_dbAccess

#include "logon.h"      // Pick up the return code defines.

#define COUNT_USERS    20
#define NO_SUCH_USER   -1

#define DATABASE_INVALID_STATE_EXCEPTION  "Failure in system service: the user database is corrupt"

bool db_opened = false;
char *users[COUNT_USERS] = {
    "object.rexx",
    "administrator",
    "root",
    "david",
    "sahananda",
    "lee",
    "mark",
    "vistor",
    "guest",
    "admin",
    "rick",
    "larry",
    "karen",
    "sue",
    "suem",
    "mary",
    "ravi",
    "michelle",
    "linda",
    "cathy"
};

char *passwords[COUNT_USERS] = {
    "rick",
    "administrator",
    "root",
    "david",
    "sahananda",
    "lee",
    "mark",
    "vistor",
    "guest",
    "admin",
    "object.rexx",
    "larry",
    "karen",
    "sue",
    "suem",
    "mary",
    "ravi",
    "michelle",
    "linda",
    "cathy"
};

inline int findUser(const char *name)
{
    for ( int i = 0; i < COUNT_USERS; i++ ) { if ( strcmp(name, users[i]) == 0 ) return i; } return NO_SUCH_USER;
}

#endif
