/*----------------------------------------------------------------------------*/
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

/*
 * Program to kill a named process under Windows.  The compiled program
 * (compiled as 32-bit) will also work on 64-bit Windows.
 *
 * Only argument: The process name.  This can include, or not, the ".exe"
 * extension.
 */

#include <windows.h>
#include <stdio.h>
#include <tlhelp32.h>

#define BUFFER_SIZE 256

/* Prototypes */
HANDLE openForTerminate(DWORD pid);
void tryToKill(PROCESSENTRY32 *pPe32);
void printError(const char *msg);
void formatMsg(char *buf, const char *additional);
HANDLE openWithDebugPrivilege(char *errMsg, DWORD processID);

int main( int argc, char *argv[] )
{
   char           szArgName[MAX_PATH] = "";
   BOOL           fFound = FALSE;
   char           *pszExt;
   HANDLE         hAllProcesses;
   PROCESSENTRY32 pe32;

   if ( argc != 2 )
   {
       fprintf(stderr, "No process name supplied\n");
       return 1;
   }

   // Support either kill or taskkill syntax
   pszExt = strrchr( argv[1], '.' );
   if ( pszExt && (stricmp(pszExt, ".exe") == 0) )
   {
       _snprintf(szArgName, sizeof(szArgName), "%s", argv[1]);
   }
   else
   {
       _snprintf(szArgName, sizeof(szArgName), "%s.exe", argv[1] );
   }

   // Get a snapshot of all processes running on the system.
   pe32.dwSize = sizeof(PROCESSENTRY32);
   hAllProcesses = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
   if( hAllProcesses == INVALID_HANDLE_VALUE )
   {
     printError("Creating process list");
     return 1;
   }

   // Get the first process entry.
   if( ! Process32First(hAllProcesses, &pe32) )
   {
     printError("Process32First()");
     CloseHandle(hAllProcesses);
     return 1;
   }

   // Look at each process until we find the one we want.  Then try to kill it.
   do
   {
       if ( stricmp(pe32.szExeFile, szArgName) == 0 )
       {
           fFound = TRUE;
           tryToKill(&pe32);
           break;
       }
   } while( Process32Next(hAllProcesses, &pe32) );

   CloseHandle(hAllProcesses);

   if ( ! fFound )
   {
       printf("The process \"%s\" was not found.\n", szArgName);
   }

   return 0;
}


void tryToKill(PROCESSENTRY32 *pPe32)
{
    HANDLE hProcess = openForTerminate(pPe32->th32ProcessID);

    if ( hProcess != NULL )
    {
        if ( TerminateProcess(hProcess, 0) )
        {
           printf("%s (process ID: %u) killed successfully\n", pPe32->szExeFile, pPe32->th32ProcessID);
        }
        else
        {
           TCHAR msg[BUFFER_SIZE];
           _snprintf(msg, BUFFER_SIZE, "Killing %s (process ID: %u)", pPe32->szExeFile, pPe32->th32ProcessID);
           printError(msg);
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
HANDLE openForTerminate(DWORD pid)
{
    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pid);

    if( hProcess == NULL )
    {
        TCHAR  errBuf[BUFFER_SIZE];
        hProcess = openWithDebugPrivilege(errBuf, pid);
        if ( hProcess == NULL )
        {
            fprintf(stderr, errBuf);
        }
    }
    return hProcess;
}

void printError(const char *msg)
{
    TCHAR buf[BUFFER_SIZE];
    formatMsg(buf, msg);
    fprintf(stderr, buf);
}

void formatMsg(char *buf, const char *additional)
{
    TCHAR sysMsg[BUFFER_SIZE];
    DWORD err = GetLastError();

    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                  sysMsg, BUFFER_SIZE, NULL);

    _snprintf(buf, BUFFER_SIZE, "%s failed with error %d (%s)\n", additional, err, sysMsg);
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
HANDLE openWithDebugPrivilege(char *errMsg, DWORD processID)
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
        return hProcess;
    }

    pid = GetCurrentProcessId();
    hCurrent = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
    if ( hCurrent == NULL )
    {
        formatMsg(errMsg, "OpenProcess() (this process)");
        return hProcess;
    }
    if ( ! OpenProcessToken(hCurrent, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken) )
    {
        CloseHandle(hCurrent);
        formatMsg(errMsg, "OpenProcessToken()");
        return FALSE;
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
        }
        // Restore the toke to the state it was in prior to the adjustment.
        success = AdjustTokenPrivileges(hToken, FALSE, &oldPrivileges, oldLength, NULL, NULL);
    }
    else
    {
        formatMsg(errMsg, "AdjustTokenPrivileges()");
    }

    CloseHandle(hCurrent);
    CloseHandle(hToken);
    return hProcess;
}
