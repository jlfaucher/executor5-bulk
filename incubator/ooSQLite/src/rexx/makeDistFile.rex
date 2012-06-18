#!/usr/bin/rexx
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

/* This file is used to create the Linux distribution file. */

use arg cmdLine

  os = getOSName()

  if os == "WINDOWS" then do
    cpCmd      = 'copy'
    mdCmd      = 'md'
    rmCmd      = 'del'
    rmDir      = 'rd /q /s'
    toNull     = '> nul 2>&1'
    sl         = '\'
    zipCmd     = 'zip -X9r'
    ext        = 'win.zip'
    setEnvFile = 'setOOSQLiteEnv.bat'
    osBinFiles = 'bin\windows\*'
  end
  else do
    cpCmd      = 'cp'
    mdCmd      = 'mkdir -p'
    rmCmd      = 'rm -f'
    rmDir      = 'rm -rf'
    toNull     = '> /dev/null 2>&1'
    sl         = '/'
    zipCmd     = 'tar -czvf'
    ext        = '_lin.tgz'
    setEnvFile = 'setOOSQLiteEnv.sh'
    osBinFiles = 'bin/linux/*'
  end

  svn_rev = cmdLine~strip('B', '"')

  bitness = 'x86_32'
  if getBitness() == 64 then bitness = 'x86_64'

  parse var svn_rev major '.' minor '.' lvl '.' svn

  outDir = 'ooSQLite.'svn_rev || sl
  outFile = 'ooSQLite_'major'_'minor'_'lvl'_'svn'_' || bitness || ext

  say 'outDir:' outDir
  say 'outFile:' outFile

  rmDir outDir toNull
  rmCmd outFile toNull

  mdCmd outDir'bin'
  mdCmd outDir'doc'
  mdCmd outDir'examples\classic.rexx'
  mdCmd outDir'misc'
  mdCmd outDir'testing'

  cpCmd 'CPLv1.0.txt' outDir
  cpCmd 'NOTICE' outDir
  cpCmd 'ReadMe.txt' outDir
  cpCmd 'ReleaseNotes' outDir
  cpCmd setEnvFile outDir

  cpCmd osBinFiles                      outDir'bin'
  cpCmd 'doc'sl'*'                      outDir'doc'
  cpCmd 'examples'sl'*'                 outDir'examples'
  cpCmd 'examples'sl'classic.rexx'sl'*' outDir'examples'sl'classic.rexx'
  cpCmd 'misc'sl'*'                     outDir'misc'
  cpCmd 'testing'sl'*'                  outDir'testing'

  zipCmd outFile outDir


::routine getOSName

  parse upper source os .
  if os~abbrev("WIN") then os = "WINDOWS"
  return os


::routine getBitness

  tmpOutFile = 'tmpXXX_delete.me'

  'rexx -v >' tmpOutFile '2>&1'

  fsObj = .stream~new(tmpOutFile)
  tmpArray = fsObj~arrayin
  parse value tmpArray[3] with . . mode
  fsObj~close

  j = SysFileDelete(tmpOutFile)

  say 'mode is:' mode

return mode
