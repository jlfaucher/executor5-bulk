/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2012-2012 Rexx Language Association. All rights reserved.    */
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

/* Quick test that version works */

  say 'SQLite3 version:' .ooSQLite~sqlite3Version
  say '    lib version:' .ooSQLite~libVersion
  say '    lib number :' .ooSQLite~libVersionNumber
  say '    source ID  :' .ooSQLite~sourceId
  say; say '='~copies(40); say

  say 'ooSQLite version (full):'; say
  say .ooSQLite~version('F')
  say; say '='~copies(40); say

  say 'ooSQLite version (one line):   ' .ooSQLite~version('O')
  say; say '='~copies(40); say

  say 'ooSQLite version (compact):    ' .ooSQLite~version('C')
  say; say '='~copies(40); say

  say 'ooSQLite version (lib version):' .ooSQLite~version('l')
  say; say '='~copies(40); say

  say 'ooSQLite version (lib number): ' .ooSQLite~version('n')
  say; say '='~copies(40); say

  say 'ooSQLite version (source ID):  ' .ooSQLite~version('s')
  say; say '='~copies(40); say

  say 'ooSQLite version (default):    ' .ooSQLite~version()
  say; say '='~copies(80); say

  say 'Same type of test using the classic Rexx interface:'
  say

  say 'SQLite3 version:' oosqlVersion()
  say '    lib version:' oosqlLibVersion()
  say '    lib number :' oosqlLibVersionNumber()
  say '    source ID  :' oosqlSourceID()
  say; say '='~copies(40); say

  say 'ooSQLite version (full):'; say
  say ooSQLiteVersion('F')
  say; say '='~copies(40); say

  say 'ooSQLite version (one line):   ' ooSQLiteVersion('O')
  say; say '='~copies(40); say

  say 'ooSQLite version (compact):    ' ooSQLiteVersion('C')
  say; say '='~copies(40); say

  say 'ooSQLite version (lib version):' ooSQLiteVersion('l')
  say; say '='~copies(40); say

  say 'ooSQLite version (lib number): ' ooSQLiteVersion('n')
  say; say '='~copies(40); say

  say 'ooSQLite version (source ID):  ' ooSQLiteVersion('s')
  say; say '='~copies(40); say

  say 'ooSQLite version (default):    ' ooSQLiteVersion()


::requires 'ooSQLite.cls'
