/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2010-2010 Rexx Language Association. All rights reserved.    */
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

#ifndef ooRexxProcess_Included
#define ooRexxProcess_Included

#ifdef __cplusplus
#define BEGIN_EXTERN_C() extern "C" {
#define END_EXTERN_C() }
#else
#define BEGIN_EXTERN_C()
#define END_EXTERN_C()
#endif

#define BUFFER_SIZE 260

// Return values - always numeric.
enum RESULT
{
    R_SUCCESS   = 0,              // Success.
    R_NOT_FOUND,                  // Process not found.
    R_NO_ARG,                     // Process name not passed to function.
    R_FAILED_TO_KILL_PROCESS,     // The process was found, everything worked, but TerminateProcess failed.
    R_NO_PROCESS_LIST,            // Creating the process list failed.
    R_PROCESS_32_FIRST,           // Process32First() API failed.
    R_LOOKUP_PRIVILEGE_VALUE,     // LookupPrivilegeValue() API failed.
    R_OPEN_PROCESS_THIS_PROCESS,  // OpenProcess() API, for our process, failed.
    R_OPEN_PROCESS_TOKEN,         // OpenProcessToken() API failed.
    R_OPEN_PROCESS,               // OpenProcess() API, for the specified process, failed.
    R_ADJUST_TOKEN_PRIVILEGES     // AdjuctTokenPrivileges() API failed.
};

char *returnCodes[] =
{
    "0",
    "1",
    "2",
    "3",
    "4",
    "5",
    "6",
    "7",
    "8",
    "9",
    "10",
    "11",
};

/* Prototypes */
static bool getName(char *processName);
static HANDLE openForTerminate(DWORD pid);
static void findMaybeKill(char *process, bool killIt);
static HANDLE openWithDebugPrivilege(char *errMsg, DWORD processID);
static void tryToKill(PROCESSENTRY32 *pPe32);


enum RESULT  returnCode = R_SUCCESS;
HANDLE       g_hInstance;
HWND         g_hwndParent;

static _inline void setReturn(void)
{
    pushstring(returnCodes[returnCode]);
}

#ifdef PLUGIN_DEBUG
static void printError(const char *msg, const char *errBuf);
static void formatMsg(char *buf, const char *additional);
#define printError(msg, errBuf)         printError(msg, errBuf)
#define formatMsg(buf, additional)      formatMsg(buf, additional)

#else

#define printError(msg, errBuf)
#define formatMsg(buf, additional)
#endif


#endif
