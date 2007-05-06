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
 * Program to kill a named process under Windows
 * Only argument: process name
 */
#include <windows.h>
#include <stdio.h>
#include "psapi.h"

int main( int argc, char *argv[] )
{
   // Find all processes

   DWORD aProcesses[4096], cbNeeded, cProcesses, processID, cLength;
   HANDLE hProcess;
   char szProcessName[MAX_PATH] = "";
   char szArgName[MAX_PATH] = "";
   unsigned int i;
   BOOL fFound = FALSE;
   char *pszExt;

   if ( argc != 2 )
   {
      fprintf( stderr, "No process name supplied\n" );
      return 1;
   }

   // Support either kill or taskkill syntax
   pszExt = strrchr( argv[1], '.' );
   if ( pszExt && (stricmp( pszExt, ".exe" ) == 0) )
      sprintf( szArgName, "%s", argv[1] );
   else
      sprintf( szArgName, "%s.exe", argv[1] );

   if ( !EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded ) )
   {
      CHAR LastError[256];
      ULONG last_error = GetLastError();
      FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT), LastError, 256, NULL ) ;
      fprintf( stderr, "No processes found. %s\n", LastError );
      return 1;
   }

   // How many processes?

   cProcesses = cbNeeded / sizeof(DWORD);

   // Find our process and kill it

   for ( i = 0; i < cProcesses; i++ )
   {
      // Get a handle to the process.
      hProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, aProcesses[i] );

      // Get the process name.
      if ( hProcess )
      {
         HMODULE hMod;

         if ( EnumProcessModules( hProcess, &hMod, sizeof(hMod), &cbNeeded) )
         {
            GetModuleBaseName( hProcess, hMod, szProcessName, sizeof(szProcessName) );
            if ( stricmp( szProcessName, szArgName ) == 0 )
            {
               fFound = TRUE;
               if ( TerminateProcess( hProcess, 0 ) )
               {
                  printf( "%s (Process ID: %u) killed successfully\n", szProcessName, aProcesses[i] );
               }
               else
               {
                  CHAR LastError[256];
                  ULONG last_error = GetLastError();
                  FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(), MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT), LastError, 256, NULL ) ;
                  printf( "%s (Process ID: %u) NOT killed. %s\n", szProcessName, aProcesses[i], LastError );
               }
            }
         }
         CloseHandle( hProcess );
      }
   }
   if ( ! fFound )
      printf( "The process \"%s\" was not found.\n", szArgName );

   return 0;
}
