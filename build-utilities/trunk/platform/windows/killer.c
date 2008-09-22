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
 * (compiled as 32-bit) should also work on 64-bit Windows.
 *
 * Only argument: The process name.  This can include, or not, the ".exe"
 * extension.
 */

#include <windows.h>
#include <stdio.h>
#include <tlhelp32.h>

/* Prototypes */
void tryToKill(PROCESSENTRY32 *pPe32);
void printError(const char *msg);

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
    HANDLE hProcess;

    hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pPe32->th32ProcessID);
    if( hProcess == NULL )
    {
        printError("OpenProcess()");
    }
    else
    {
        if ( TerminateProcess(hProcess, 0) )
        {
           printf("%s (process ID: %u) killed successfully\n", pPe32->szExeFile, pPe32->th32ProcessID);
        }
        else
        {
           TCHAR msg[64];
           _snprintf(msg, sizeof(msg), "Killing %s (process ID: %u)", pPe32->szExeFile, pPe32->th32ProcessID);
           printError(msg);
        }
        CloseHandle(hProcess);
    }
}

void printError(const char *msg)
{
    TCHAR buf[256];
    DWORD err = GetLastError();

    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                  buf, 256, NULL);
    fprintf(stderr, "%s failed with error %d (%s)\n", msg, err, buf);
}
