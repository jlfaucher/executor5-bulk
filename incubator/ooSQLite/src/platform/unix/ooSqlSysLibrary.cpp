/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 1995, 2004 IBM Corporation. All rights reserved.             */
/* Copyright (c) 2005-2013 Rexx Language Association. All rights reserved.    */
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

/** ooSqlSysLibrary.cpp
 *
 *  Unix implementation of ooSqlSysLibrary for ooSQLite.  This file is adapted
 *  from the interpreter's ooSqlSysLibrary implmentation.
 *
 */

#include "sqlite3.h"
#include <oorexxapi.h>
#include "unixOS.hpp"
#include "ooSqlSysLibrary.hpp"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <dlfcn.h>


#define LIBARY_NAME_BUFFER_LENGTH (MAX_LIBRARY_NAME_LENGTH + sizeof("/usr/lib/lib") + sizeof(OOSQLITE_SHARED_LIBRARY_EXT))

ooSqlSysLibrary::ooSqlSysLibrary()
{
    libraryHandle = NULL;
    resetLastErr();
}


/**
 * Resolve a named function in a library.
 *
 * @param name   Function name.
 *
 * @return The resolved function address or null on error.
 */
void * ooSqlSysLibrary::getProcedure(const char *name)
{
    resetLastErr();
    dlerror();

    void *func = dlsym(libraryHandle, name);
    if ( func == NULL )
    {
        setLastErr(PROCEDURE_NOT_FOUND_RC);
    }
    return func;
}


/**
 * Load a named library, returning success/failure flag
 *
 * @param name  The name of the library.
 *
 * @return True on success or false on error.
 *
 * @note  If the user passes a pathname, we try opening the library with the
 *        name as is.  If this fails, we quit then.  This is different than the
 *        interpreter's implementation.
 */
bool ooSqlSysLibrary::load(const char *name)
{
    if ( libraryHandle != NULL )
    {
        return true;
    }

    char nameBuffer[LIBARY_NAME_BUFFER_LENGTH];

    resetLastErr();
    dlerror();

    if ( strlen(name) > MAX_LIBRARY_NAME_LENGTH )
    {
        CSTRING fmt = "Library name: %s is too long";

        lastErrMsg = (char *)malloc(strlen(name) + strlen(fmt) + 1);
        if ( lastErrMsg != NULL )
        {
            sprintf(lastErrMsg, fmt, name);
            lastErrCode = 1;
        }
        return false;
    }

    if ( strchr(name, '/') != NULL )
    {
        libraryHandle = dlopen(name, RTLD_LAZY);
        if ( libraryHandle == NULL )
        {
            setLastErr(MODULE_NOT_FOUND_RC);
            return false;
        }
        return true;
    }
    else

    sprintf(nameBuffer, "lib%s%s", name, OOSQLITE_SHARED_LIBRARY_EXT);

    libraryHandle = dlopen(nameBuffer, RTLD_LAZY);
    if ( libraryHandle == NULL )
    {
        sprintf(nameBuffer, "/usr/lib/lib%s%s", name, OOSQLITE_SHARED_LIBRARY_EXT);
        libraryHandle = dlopen(nameBuffer, RTLD_LAZY);
        if ( libraryHandle == NULL )
        {
            setLastErr(MODULE_NOT_FOUND_RC);
            return false;
        }
    }
    return true;
}


/**
 * Sets the last error variables when a dl library function indicates an error
 * has happened.
 */
void ooSqlSysLibrary::setLastErr(uint32_t rc)
{
    char *temp = dlerror();

    if ( temp == NULL )
    {
        temp = (char *)"no error";
    }

    lastErrMsg = (char *)malloc(strlen(temp) + 1);
    if ( lastErrMsg != NULL )
    {
        strcpy(lastErrMsg, temp);
        lastErrCode = rc;
    }
}

/**
 * Frees any memory allocated for the laste error message and sets the error
 * message to no error and the error code to 0.
 *
 * @note  Not very object orientated, we require the user of this class to
 *        invoke resetLastErr() after retrieving an error message and making a
 *        copy of it.
 */
void ooSqlSysLibrary::resetLastErr()
{
    if ( lastErrMsg != NULL )
    {
        free(lastErrMsg);
        lastErrMsg = NULL;
    }

    lastErrMsg = (char *)malloc(strlen("no error") + 1);
    if ( lastErrMsg != NULL )
    {
        strcpy(lastErrMsg, "no error");
    }
    lastErrCode = 0;
}


/**
 * Free a loaded library if the library is still loaded.
 *
 * @return True if we unloaded ok, false otherwise.
 */
bool ooSqlSysLibrary::unload()
{
    if ( libraryHandle != NULL )
    {
        dlclose(libraryHandle);
        resetLastErr();
        return true;
    }
    return false;
}


/**
 * Resets this object to unused.
 */
void ooSqlSysLibrary::reset()
{
    resetLastErr();
    libraryHandle = NULL;
}
