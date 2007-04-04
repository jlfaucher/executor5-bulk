/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2007 Rexx Language Association. All rights reserved.         */
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
#define _WIN32_WINDOWS   0x0400
#define WINVER           0x0400

#include <windows.h>
#include <stdio.h>
#include "stouch.h"

/**
 * Program entry point.  Simple touch program.  Touch all the file(s) specified
 * on the command line with the current time.  No arguments are supported other
 * than help.
 *
 * @param argc  The count of command line arguments.
 * @param argv  An array of the command line arguments.
 *
 * @return      0 if no error, otherwise non-zero on error.
 */
int main( int argc, char *argv[] )
{
    int      i;
    ARG_TYPE argt;
    DWORD    errRC = 0;

    if ( argc < 2 )
        return doHelp( NO_FILE_ARG );

    if ( wantsHelp( *argv[1] ) )
        return doHelp( WANTS_HELP );

    for ( i = 1; i < argc; i++ )
    {
        argt = checkArg( argv[i], &errRC );

        if ( argt == DIR_TYPE )
            continue;
        else if ( argt == FILE_TYPE )
            errRC = changeTime( argv[i], &argt );

        if ( errRC )
            return printErr( argv[i], argt, errRC );
    }
    return 0;
}

/**
 * Changes the last modified and accessed time of the specified file.
 *
 * @param pszFile  File to change.
 *
 * @param pFailed  [out]  On an error, returns the type of error.
 *
 * @return         0 on success, otherwise the Windows error code.
 */
DWORD changeTime( PSZ pszFile, PARG_TYPE pFailed )
{
    HANDLE     hFile;
    FILETIME   fileTime;
    SYSTEMTIME sysTime;
    DWORD      rc = 0;

    hFile = CreateFile( pszFile, GENERIC_WRITE, 0, NULL, OPEN_EXISTING,
                        FILE_ATTRIBUTE_NORMAL, NULL );

    if ( hFile == INVALID_HANDLE_VALUE )
    {
        rc  = GetLastError();
        *pFailed = OPEN_FAILED_TYPE;
    } else
    {
        GetSystemTime( &sysTime );
        SystemTimeToFileTime( &sysTime, &fileTime);

        if ( ! SetFileTime( hFile, (LPFILETIME)NULL, &fileTime, &fileTime ) )
        {
            rc = GetLastError();
            *pFailed = CHANGE_FAILED_TYPE;
        }
    }
    return rc;
}

/**
 * Prints out a message on error.
 *
 * @param arg      The command line argument where the error occurred.
 *
 * @param errType  An enum that denotes the type of error.
 *
 * @param errRC    The Windows error code produced by the error.
 *
 * @return         Non-zero.
 */
int printErr( PSZ arg, ARG_TYPE errType, DWORD errRC )
{

    switch ( errType )
    {
        case INVALID_TYPE :
            printf( INVALID_ERR, arg, errRC );
            break;

        case GET_FAILED_TYPE :
            printf( GET_ERR, arg, errRC );
            break;

        case OPEN_FAILED_TYPE :
            printf( OPEN_ERR, arg, errRC );
            break;

        case CHANGE_FAILED_TYPE :
            printf( CHANGE_ERR, arg, errRC );
            break;

        default :
            printf( UNKNOWN_ERR, arg, errRC );
            break;
    }
    return BAD_ARG;
}

/**
 * Check if an argument is a file, a directory or invalid.
 *
 * @param psz      The arguement to check.
 *
 * @param pErrrRC  If a system error occurs the error code is returned.
 *
 * @return         The type of the argument: file, directory, not valid, or an
 *                 error type.
 */
ARG_TYPE checkArg( PSZ psz, LPDWORD pErrrRC )
{
    WIN32_FILE_ATTRIBUTE_DATA fad;
    ARG_TYPE argType = INVALID_TYPE;

    if ( GetFileAttributesEx( psz, GetFileExInfoStandard, &fad ) )
    {
        if ( fad.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
            argType = DIR_TYPE;
        else if ( fad.dwFileAttributes & FILE_MASK )
            argType = FILE_TYPE;
    } else
    {
        *pErrrRC = GetLastError();
        argType = GET_FAILED_TYPE;
    }
    return argType;
}

/**
 * Naive check for a help string, like: ?, /H, -h, --help, /?, etc..
 * Only checks first character of the string for a suggestive leading
 * character.
 *
 * @param ch  The character to check.
 *
 * @return    True if it looks like it might be a help flag, otherwise false.
 */
BOOL __inline wantsHelp( CHAR ch )
{
    if ( ch == '?' || ch == '/' || ch == '-' )
        return TRUE;
    return FALSE;
}

/**
 * Prints out standard help information and returns the error code.
 *
 * @param which  Identifies which help to use and is also the error code
 *               returned.
 *
 * @return       The argument passed in (i.e. 'which')
 */
int doHelp( int which )
{

    switch ( which )
    {
        case WANTS_HELP :
            printf( HELP_HELP );
            break;

        case NO_FILE_ARG :
            printf( NO_ARG_HELP );
            break;

        default :
            printf( HELP_HELP );
            break;
    }
    return which;
}
