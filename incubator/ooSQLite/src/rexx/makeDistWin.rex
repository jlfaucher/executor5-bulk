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

/* This file is used to create the Windows distribution file. */

use arg cmdLine

  svn_rev = cmdLine~strip('B', '"')
  bitness = 'x86_32'
  if .OS~is64Bit then bitnness = '_x86_64'

  parse var svn_rev major '.' minor '.' lvl '.' svn

  outDir = 'ooSQLite.' || svn_rev
  outFile = 'ooSQLite_'major'_'minor'_'lvl'_'svn'_' || bitness'_win.zip'

  say 'outDir:' outDir
  say 'outFile:' outFile

  'rd /q /s' outDir '> nul 2>&1'
  'del' outFile '> nul 2>&1'

  'md' outDir'\bin'
  'md' outDir'\doc'
  'md' outDir'\examples'
  'md' outDir'\misc'
  'md' outDir'\testing'

  'copy CPLv1.0.txt' outDir
  'copy NOTICE' outDir
  'copy ReadMe.txt' outDir
  'copy ReleaseNotes' outDir
  'copy setOOSQLiteEnv.bat' outDir

  'copy bin\windows\*' outDir'\bin'
  'copy doc\*' outDir'\doc'
  'copy examples\*' outDir'\examples'
  'copy misc\*' outDir'\misc'
  'copy testing\*' outDir'\testing'

  'zip -X9r' outFile outDir

::requires 'ooDialog.cls'
