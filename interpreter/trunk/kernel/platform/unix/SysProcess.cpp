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
/*****************************************************************************/
/* REXX Windows Support                                                      */
/*                                                                           */
/* Process support for Windows                                               */
/*                                                                           */
/*****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#ifdef HAVE_PWD_H
# include <pwd.h>
#endif

#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "SysProcess.hpp"

/*********************************************************************/
/*                                                                   */
/*   Subroutine Name:   relinquish                                   */
/*                                                                   */
/*   Function:          Performs peekmsg to yield to other processes */
/*                                                                   */
/*********************************************************************/

void SysProcess::relinquish(void)
{
    ;
}

SysProcess::PlatformVersion SysProcess::platformVersion = SysProcess::Unix;
bool SysProcess::useMessageLoop = false;

void SysProcess::setSystemVersion()
{
    ;
}

/**
 * Get the current user name information.
 *
 * @param buffer The buffer (of at least MAX_USERID_LENGTH characters) into which the userid is copied.
 */
void SysProcess::getUserID(char *buffer)
{
#if defined( HAVE_GETPWUID )
    struct passwd * pstUsrDat;
#endif

#if defined( HAVE_GETPWUID )
    pstUsrDat = getpwuid(geteuid());
    strncpy( buffer,  pstUsrDat->pw_name, MAX_USERID_LENGTH-1);
#elif defined( HAVE_IDTOUSER )
    strncpy( buffer, IDtouser(geteuid()), MAX_USERID_LENGTH-1);
#else
    strcpy( buffer, "unknown" );
#endif
}


/**
 * Do process specific sleep.
 *
 * @param milliseconds
 *               The number of milliseconds to sleep.
 */
void SysProcess::sleep(int milliseconds)
{
    usleep(milliseconds * 1000);
}


/**
 * Realloc and concanenate a tring to the original.
 *
 * @param target The target string.
 * @param src    The source string to be concatenated.
 * @param len    The length of the source string.
 */
static char * localstrncat(char *target, const char *src, int len)
{
    if (len == 0) {
        return target;
    }
    target = (char *)realloc(target, strlen(target) + len + 1);
    strncat(target, src, len);
    return target;
}


/**
 * Reconstruct the command line from the argv array.
 *
 * @param argc   The number of arguments.
 * @param argv   The arguments.
 */
char * reconstructCommandLine(const int argc, const char *argv[])
{
    char *cmdline;
    const char *needle, *start;
    int i;
    bool dblquote;

    // start the cmdline off as a zero-length C-string
    cmdline = (char *)malloc(1);
    *cmdline = '\0';
    // skip argv[0], we do not want the name of the program
    for (i = 1; i < argc; i++) {
        // if this is not the first argument then concatenate a space character
        if (strlen(cmdline) != 0) {
            cmdline = localstrncat(cmdline, " ", 1);
        }
        // does the argument contain a space character?
        if (strstr(argv[i], " ") != NULL) {
            dblquote = true;
        }
        else {
            dblquote = false;
        }
        // add a starting double quote if necessary
        if (dblquote) {
            cmdline = localstrncat(cmdline, "\"", 1);
        }
        // does the argument contain backslash characters?
        start = argv[i];
        needle = strstr(start, "\\");
        while (needle != NULL) {
            cmdline = localstrncat(cmdline, start, needle - start);
            cmdline = localstrncat(cmdline, "\\", 1);
            start = needle + 1;
            needle = strstr(start, "\\");
        }
        cmdline = localstrncat(cmdline, start, strlen(start));
        // does the argument contain double quote characters?
        start = argv[i];
        needle = strstr(start, "\"");
        while (needle != NULL) {
            cmdline = localstrncat(cmdline, start, needle - start);
            cmdline = localstrncat(cmdline, "\"", 1);
            start = needle + 1;
            needle = strstr(start, "\"");
        }
        cmdline = localstrncat(cmdline, start, strlen(start));
        // add an ending double quote if necessary
        if (dblquote) {
            cmdline = localstrncat(cmdline, "\"", 1);
        }
    }
    return cmdline;
}

