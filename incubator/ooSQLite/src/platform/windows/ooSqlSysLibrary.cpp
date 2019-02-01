/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 1995, 2004 IBM Corporation. All rights reserved.             */
/* Copyright (c) 2005-2019 Rexx Language Association. All rights reserved.    */
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
 *  Windows implementation of ooSqlSysLibrary for ooSQLite.  This file is adapted
 *  from the interpreter's SysLibrary implmentation.
 *
 */

#include "winOS.hpp"
#include <oorexxapi.h>
#include "ooSqlSysLibrary.hpp"
#include "stdio.h"

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
    SetLastError(0);
    resetLastErr();

    void *func = (void *)GetProcAddress(libraryHandle, name);
    if ( func == NULL )
    {
        setLastErr("GetProcAddress", name);
    }
    return func;
}


/**
 * Load a named library in this process.
 *
 * @param name   Character string name of the library.
 *
 * @return True if able to load the library ok, false otherwise.
 *
 * @note  We always use LoadLibrary to load this, to bump the reference count on
 *        the module if it is already loaded in this process.
 *
 *        We make no attempt to construct a file name like we do in the Unix
 *        version.  Still, the user does not need to specify the .dll extension,
 *        Windows will add it automatically if there is no extension.
 */
bool ooSqlSysLibrary::load(const char *name)
{
    if ( libraryHandle != NULL )
    {
        return true;
    }

    if ( strlen(name) > MAX_LIBRARY_NAME_LENGTH )
    {
        char *fmt = "Library name: %s is too long";

        lastErrMsg = (char *)LocalAlloc(LPTR, strlen(name) + strlen(fmt) + 1);
        if ( lastErrMsg != NULL )
        {
            sprintf(lastErrMsg, fmt, name);
            lastErrCode = 1;
        }
        return false;
    }

    SetLastError(0);
    resetLastErr();

    libraryHandle = LoadLibrary(name);
    if ( libraryHandle == NULL )
    {
        setLastErr("LoadLibrary", name);
        return false;
    }
    return true;
}


/**
 * Sets the last error variables.
 */
void ooSqlSysLibrary::setLastErr(const char *api, const char *name)
{
    lastErrCode = GetLastError();

    void     *tmpBuf = NULL;
    uint16_t  lang   = MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT);
    uint32_t  flags  = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;

    uint32_t count = FormatMessage(flags, NULL, lastErrCode, lang, (char *)&tmpBuf, FORMAT_MESSAGE_MAX_WIDTH_MASK, NULL);

    // tmpBuf is null if count is 0.
    if ( count > 1 )
    {
        // Remove the trailing new line characters
        count--;
        *((char *)tmpBuf + count--) = '\0';
        *((char *)tmpBuf + count) = '\0';

        char buff[512];

        snprintf(buff, 512, "%s - %s (%s)", api, tmpBuf, name);
        LocalFree(tmpBuf);

        lastErrMsg = (char *)LocalAlloc(LPTR, strlen(buff) + 1);
        if ( lastErrMsg != NULL )
        {
            strcpy(lastErrMsg, buff);
        }
    }
}

/**
 * Frees any memory allocated for the laste error message and sets the error
 * message to null and the error code to 0.
 *
 * @note  Not very object orientated, we require the user of this class to
 *        invoke resetLastErr() after retrieving an error message and making a
 *        copy of it.
 */
void ooSqlSysLibrary::resetLastErr()
{
    if ( lastErrMsg != NULL )
    {
        LocalFree(lastErrMsg);
        lastErrMsg = NULL;
    }

    lastErrMsg = (char *)LocalAlloc(LPTR, strlen("no error") + 1);
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
        resetLastErr();
        return FreeLibrary(libraryHandle) != FALSE;
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
