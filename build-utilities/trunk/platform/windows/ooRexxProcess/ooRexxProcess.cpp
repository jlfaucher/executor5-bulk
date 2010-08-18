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

/**
 * NSIS plugin used by the ooRexx Windows installer.
 *
 * Contains functions: findProcess and killProcess
 *
 * Usage (in nsis script):
 *
 *   ooRexxProcess::findProcess <processName>
 *   Pop <varName>
 *
 *   ooRexxProcess::killProcess <processName>
 *   Pop <varName>
 *
 * Returns a numeric value, 0 for success.
 *
 * Example (in nsis script):
 *
 *   ooRexxProcess::killProcess "rxapi"
 *   Pop $0
 *
 *   ; $0 now contains 0 for success, 1 if no rxapi process was found.
 *
 * Return codes:
 *
 *  0  Success.
 *  1  The specified process was not found.
 *  2  No process name was passed to the function.
 *  3  The process was found, everything worked, but termination failed.
 *  4  Creating the process list failed.
 *  5  The Process32First() API failed.
 *  6  The LookupPrivilegeValue() API failed.
 *  7  The OpenProcess() API, for our process, failed.
 *  8  The OpenProcessToken() API failed.
 *  9  The OpenProcess() API, for the specified process, failed.
 * 10  The AdjuctTokenPrivileges() API failed.
 *
 * TODO - It would be nice to figure out a way of passing back the GetLastError
 *        error code in addition to the above codes.
 */

/*
 * Notes from the NSIS example plugin code:
 *
 * note if you want parameters from the stack, pop them off in order.
 * i.e. if you are called via ooRexxProcess::myFunction file.dat poop.dat
 * calling popstring() the first time would give you file.dat,
 * and the second time would give you poop.dat.
 * you should empty the stack of your parameters, and ONLY your
 * parameters.
 *
 */

#include <windows.h>
#include <shlwapi.h>
#include <tlhelp32.h>
#include <pluginapi.h>
#include "ooRexxProcess.hpp"

BEGIN_EXTERN_C()

void __declspec(dllexport) findProcess(HWND hwndParent, int string_size, char *variables, stack_t **stacktop,
                                       extra_parameters *extra)
{
    char buf[1024];
    char processName[BUFFER_SIZE];

    EXDLL_INIT();
    g_hwndParent = hwndParent;

    if ( getName(processName) )
    {
        findMaybeKill(processName, false);
    }

    setReturn();
}

void __declspec(dllexport) killProcess(HWND hwndParent, int string_size, char *variables, stack_t **stacktop,
                                       extra_parameters *extra)
{
    char processName[BUFFER_SIZE];

    EXDLL_INIT();
    g_hwndParent = hwndParent;

    if ( getName(processName) )
    {
        findMaybeKill(processName, true);
    }

    setReturn();
}

END_EXTERN_C()

static bool getName(char *processName)
{
    char *pExt;

    *processName = '\0';
    popstring(processName);

    if ( *processName == '\0' )
    {
        returnCode = R_NO_ARG;
        return false;
    }

    pExt = StrRChr(processName, NULL, '.');
    if ( pExt == NULL || (lstrcmpi(pExt, ".exe") != 0) )
    {
        lstrcat(processName, ".exe");
    }

    return true;
}

static void findMaybeKill(char *process, bool killIt)
{
    bool           found = false;
    HANDLE         hAllProcesses;
    PROCESSENTRY32 pe32;

    // Get a snapshot of all processes running on the system.
    pe32.dwSize = sizeof(PROCESSENTRY32);
    hAllProcesses = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if( hAllProcesses == INVALID_HANDLE_VALUE )
    {
        printError("Creating process list", NULL);
        returnCode = R_NO_PROCESS_LIST;
        return;
    }

    // Get the first process entry.
    if( ! Process32First(hAllProcesses, &pe32) )
    {
        printError("Process32First()", NULL);
        CloseHandle(hAllProcesses);
        returnCode = R_PROCESS_32_FIRST;
        return;
    }

    // Look at each process until we find the one we want.  Then try to kill it.
    do
    {
        if ( lstrcmpi(pe32.szExeFile, process) == 0 )
        {
            found = true;
            if ( killIt )
            {
                tryToKill(&pe32);
            }
            break;
        }
    } while( Process32Next(hAllProcesses, &pe32) );

    CloseHandle(hAllProcesses);

    if ( ! found )
    {
        returnCode = R_NOT_FOUND;
    }
}

static void tryToKill(PROCESSENTRY32 *pPe32)
{
    HANDLE hProcess = openForTerminate(pPe32->th32ProcessID);

    if ( hProcess != NULL )
    {
        if ( ! TerminateProcess(hProcess, 0) )
        {
            returnCode = R_FAILED_TO_KILL_PROCESS;
        }
        CloseHandle(hProcess);
    }
}

/**
 * Attempts to open a process with process terminate access.  On Vista, opening
 * with all access can fail when opening with just terminate access will
 * succeed.
 *
 * If the process is not opened, then an attempt to open the process by granting
 * the current access token the SeDebugPivilege is made.  If the privilege is
 * granted, the open will succeed.  If it isn't granted then the invoker just
 * doesn't have the authority to kill the process, and the process won't be
 * opened.
 *
 * @param  pid  The id of the process to be opened.
 *
 * @return The opened handle, or null on failure.  Caller is responsible for
 *         closing the handle.
 */
static HANDLE openForTerminate(DWORD pid)
{
    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pid);

    if( hProcess == NULL )
    {
        TCHAR  errBuf[BUFFER_SIZE];
        hProcess = openWithDebugPrivilege(errBuf, pid);
        if ( hProcess == NULL )
        {
            printError(NULL, errBuf);
        }
    }
    return hProcess;
}

/**
 * Attempts to enable the SeDebugPrivilege in the current access token and then
 * open the process with terminate access.  After the open process the previous
 * privilege level of the access token is restored.
 *
 * @param   errMsg  Passed in buffer used to format a message on error and
 *                  return to the caller.
 *
 * @param   processID  ID of the process to be opened.
 *
 * @return  The opened handle to the process on success, otherwise the handle is
 *          NULL.
 */
static HANDLE openWithDebugPrivilege(char *errMsg, DWORD processID)
{
    TOKEN_PRIVILEGES newPrivileges;
    TOKEN_PRIVILEGES oldPrivileges;
    DWORD            oldLength = 0;
    LUID   luid;
    HANDLE hCurrent;
    HANDLE hProcess = NULL;
    HANDLE hToken;
    DWORD  pid;
    BOOL   success = FALSE;

    if ( ! LookupPrivilegeValue(NULL, "SeDebugPrivilege", &luid ) )
    {
        formatMsg(errMsg, "LookupPrivilegeValue()");
        returnCode = R_LOOKUP_PRIVILEGE_VALUE;
        return hProcess;
    }

    pid = GetCurrentProcessId();
    hCurrent = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
    if ( hCurrent == NULL )
    {
        formatMsg(errMsg, "OpenProcess() (this process)");
        returnCode = R_OPEN_PROCESS_THIS_PROCESS;
        return hProcess;
    }
    if ( ! OpenProcessToken(hCurrent, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken) )
    {
        CloseHandle(hCurrent);
        formatMsg(errMsg, "OpenProcessToken()");
        returnCode = R_OPEN_PROCESS_TOKEN;
        return NULL;
    }

    newPrivileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    newPrivileges.Privileges[0].Luid = luid;
    newPrivileges.PrivilegeCount = 1;

    // Try to enable the debug privilege
    success = AdjustTokenPrivileges(hToken, FALSE, &newPrivileges, sizeof(TOKEN_PRIVILEGES), &oldPrivileges, &oldLength);

    if ( success )
    {
        // Now try to open the process.  If the debug privilege was granted this
        // will work.  If the privilege was not granted, it will fail again.
        hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, processID);
        if ( hProcess == NULL )
        {
            formatMsg(errMsg, "OpenProcess()");
            returnCode = R_OPEN_PROCESS;
        }
        // Restore the token to the state it was in prior to the adjustment.
        success = AdjustTokenPrivileges(hToken, FALSE, &oldPrivileges, oldLength, NULL, NULL);
    }
    else
    {
        formatMsg(errMsg, "AdjustTokenPrivileges()");
        returnCode = R_ADJUST_TOKEN_PRIVILEGES;
    }

    CloseHandle(hCurrent);
    CloseHandle(hToken);
    return hProcess;
}

BOOL WINAPI DllMain(HANDLE hInst, ULONG reason, LPVOID lpReserved)
{
    if ( reason == DLL_PROCESS_ATTACH )
    {
        g_hInstance=hInst;
    }
	return TRUE;
}


#ifdef PLUGIN_DEBUG

static void printError(const char *msg, const char *errBuf)
{
    if ( errBuf == NULL )
    {
        TCHAR buf[BUFFER_SIZE];
        formatMsg(buf, msg);
        MessageBox(g_hwndParent, buf, 0, MB_OK);
    }
    else
    {
        MessageBox(g_hwndParent, errBuf, 0, MB_OK);
    }
}

static void formatMsg(char *buf, const char *additional)
{
    TCHAR sysMsg[BUFFER_SIZE];
    DWORD err = GetLastError();

    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                  sysMsg, BUFFER_SIZE, NULL);

    wsprintf(buf, "%s failed with error %d (%s)\n", additional, err, sysMsg);
}
#endif
