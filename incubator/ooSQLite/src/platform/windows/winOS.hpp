/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2012-2019 Rexx Language Association. All rights reserved.    */
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

#ifndef winOS_Included
#define winOS_Included


#define NTDDI_VERSION   NTDDI_LONGHORN
#define _WIN32_WINNT    0x0600
#define _WIN32_IE       0x0600
#define WINVER          0x0501

#define STRICT
#define OEMRESOURCE

#include <windows.h>
#include <shlwapi.h>

#define strcasecmp                 _stricmp
#define strncasecmp                _strnicmp
//efine snprintf                   _snprintf
#define oosqlGetCurrentThreadId()  GetCurrentThreadId()

#define MAX_LIBRARY_NAME_LENGTH     255
#define OOSQL_SLASH_CHR             '\\'
#define OOSQL_LIBRARY_PREFIX        NULL
#define OOSQL_LIBRARY_PREFIX_LEN    0


#ifdef NEED_DLL_MAIN

CRITICAL_SECTION     crit_sec = {0};

#define CRITICAL_SECTION_ENTER EnterCriticalSection(&crit_sec);
#define CRITICAL_SECTION_LEAVE LeaveCriticalSection(&crit_sec);

BEGIN_EXTERN_C()

BOOL APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    if ( fdwReason == DLL_PROCESS_ATTACH )
    {
        InitializeCriticalSection(&crit_sec);
    }
    else if ( fdwReason == DLL_PROCESS_DETACH )
    {
        DeleteCriticalSection(&crit_sec);
    }
    return(TRUE);
}

END_EXTERN_C()

#endif  /* If defined NEED_DLL_MAIN*/

#endif
