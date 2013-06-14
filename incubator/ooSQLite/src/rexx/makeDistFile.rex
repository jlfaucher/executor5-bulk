#!/usr/bin/rexx
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2012-2013 Rexx Language Association. All rights reserved.    */
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
 * This file is used to create a binary distribution of ooSQLite.  The binary
 * distribution can just be unpacked where ever the user would like.
 *
 * Note that when packaging up the example programs, we just take everything
 * in the directories.  Trying to maintain file lists of the examples is to
 * hard to maintain.
 */

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
    osBinDir    = 'bin\windows\'
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
    osBinDir   = 'bin/linux/'
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
  mdCmd outDir'examples'sl'classic.rexx'
  mdCmd outDir'examples'sl'informal'
  mdCmd outDir'examples'sl'misc'
  mdCmd outDir'examples'sl'user.extensions'
  mdCmd outDir'examples'sl'user.extensions'sl'autoPackages'

  cpCmd 'CPLv1.0.txt' outDir
  cpCmd 'NOTICE' outDir
  cpCmd 'ReadMe.txt' outDir
  cpCmd 'ReleaseNotes' outDir
  cpCmd setEnvFile outDir

  if os == "WINDOWS" then do
    cpCmd osBinDir'ooSQLite.cls'       outDir'bin'
    cpCmd osBinDir'oosqlite.dll'       outDir'bin'
    cpCmd osBinDir'ooSQLite3.exe'      outDir'bin'
  end
  else do
    cpCmd osBinDir'liboosqlite.so'     outDir'bin'
    cpCmd osBinDir'oosqlite3'          outDir'bin'
    cpCmd osBinDir'ooSQLite.cls'       outDir'bin'
  end

  cpCmd 'doc'sl'oosqlite.pdf'          outDir'doc'
  cpCmd 'doc'sl'ooSQLite-ReadMe.txt'   outDir'doc'
  cpCmd 'doc'sl'ooSQLite-ReleaseNotes' outDir'doc'
  if os == "WINDOWS" then do
    cpCmd 'doc'sl'StatusAllSQLiteFunctions.xlsx' outDir'doc'
  end

  cpCmd 'examples'sl'collationCustom.rex'          outDir'examples'
  cpCmd 'examples'sl'createDatabase.rex'           outDir'examples'
  cpCmd 'examples'sl'enquote.rex'                  outDir'examples'
  cpCmd 'examples'sl'insertIntoDatabase.rex'       outDir'examples'
  cpCmd 'examples'sl'insertNullOne.rex'            outDir'examples'
  cpCmd 'examples'sl'insertNullThree.rex'          outDir'examples'
  cpCmd 'examples'sl'insertNullTwo.rex'            outDir'examples'
  cpCmd 'examples'sl'loadExtension.rex'            outDir'examples'
  cpCmd 'examples'sl'loadLibrary.rex'              outDir'examples'
  cpCmd 'examples'sl'loadPackage.rex'              outDir'examples'
  cpCmd 'examples'sl'ooFoods.rdbx'                 outDir'examples'
  cpCmd 'examples'sl'packageAutoRegistration.rex'  outDir'examples'
  cpCmd 'examples'sl'pullColumnData.rex'           outDir'examples'
  cpCmd 'examples'sl'udfExample.rex'               outDir'examples'
  cpCmd 'examples'sl'utilities.frm'                outDir'examples'

  cpCmd 'examples'sl'classic.rexx'sl'backupDB.rex'          outDir'examples'sl'classic.rexx'
  cpCmd 'examples'sl'classic.rexx'sl'ooFoods.rdbx'          outDir'examples'sl'classic.rexx'
  cpCmd 'examples'sl'classic.rexx'sl'preparedStmtRtn.rex'   outDir'examples'sl'classic.rexx'
  cpCmd 'examples'sl'classic.rexx'sl'pullColumnDataRtn.rex' outDir'examples'sl'classic.rexx'

  -- user.extensions subdirectory
  srcDirNow = 'examples'sl'user.extensions'sl
  dstDirNow = outDir'examples'sl'user.extensions'sl

  cpCmd dstDirNow'simpleExtension.c'  dstDirNow
  cpCmd srcDirNow'examplePackage.cpp' dstDirNow
  cpCmd srcDirNow'exampleLibrary.cpp' dstDirNow
  if os == "WINDOWS" then do
    cpCmd srcDirNow'Makefile.win'        dstDirNow
    cpCmd srcDirNow'simpleExtension.dll' dstDirNow
    cpCmd srcDirNow'examplePackage.dll'  dstDirNow
    cpCmd srcDirNow'exampleLibrary.dll'  dstDirNow
    cpCmd srcDirNow'simpleExtension.def' dstDirNow
    cpCmd srcDirNow'examplePackage.def'  dstDirNow
    cpCmd srcDirNow'exampleLibrary.def'  dstDirNow
  end
  else do
    cpCmd srcDirNow'Makefile.lin'          dstDirNow
    cpCmd srcDirNow'libsimpleextension.so' dstDirNow
    cpCmd srcDirNow'libexamplepackage.so'  dstDirNow
    cpCmd srcDirNow'libexamplelibrary.so'  dstDirNow
  end

  -- autoPackages subdirectory
  srcDirNow = 'examples'sl'user.extensions'sl'autoPackages'sl
  dstDirNow = outDir'examples'sl'user.extensions'sl'autoPackages'

  cpCmd srcDirNow'autoPackage1.cpp' dstDirNow
  cpCmd srcDirNow'autoPackage2.cpp' dstDirNow
  cpCmd srcDirNow'autoPackage3.cpp' dstDirNow
  cpCmd srcDirNow'autoPackage4.cpp' dstDirNow
  cpCmd srcDirNow'autoPackage5.cpp' dstDirNow
  cpCmd srcDirNow'ReadMe.txt'       dstDirNow
  if os == "WINDOWS" then do
    cpCmd srcDirNow'Makefile.win'      dstDirNow
    cpCmd srcDirNow'autoPackage1.dll'  dstDirNow
    cpCmd srcDirNow'autoPackage2.dll'  dstDirNow
    cpCmd srcDirNow'autoPackage3.dll'  dstDirNow
    cpCmd srcDirNow'autoPackage4.dll'  dstDirNow
    cpCmd srcDirNow'autoPackage5.dll'  dstDirNow
    cpCmd srcDirNow'autoPackage.def'   dstDirNow
  end
  else do
    cpCmd srcDirNow'Makefile.lin'       dstDirNow
    cpCmd srcDirNow'libautopackage1.so' dstDirNow
    cpCmd srcDirNow'libautopackage2.so' dstDirNow
    cpCmd srcDirNow'libautopackage3.so' dstDirNow
    cpCmd srcDirNow'libautopackage4.so' dstDirNow
    cpCmd srcDirNow'libautopackage5.so' dstDirNow
  end

  cpCmd 'misc'sl'foodsPristine.rdbx'    outDir'misc'
  cpCmd 'misc'sl'ReadMe.txt'            outDir'misc'

  -- testing subdirectory
  srcDirNow = 'testing'sl
  dstDirNow = outDir'testing'

  cpCmd srcDirNow'execTestA.rex'          dstDirNow
  cpCmd srcDirNow'execTestD.rex'          dstDirNow
  cpCmd srcDirNow'execTestNoExec.rex'     dstDirNow
  cpCmd srcDirNow'execTestS.rex'          dstDirNow
  cpCmd srcDirNow'loadDatabase.rex'       dstDirNow
  cpCmd srcDirNow'ooFoods.rdbx'           dstDirNow
  cpCmd srcDirNow'pragmaGetTest.rex'      dstDirNow
  cpCmd srcDirNow'pragmaSetTest.rex'      dstDirNow
  cpCmd srcDirNow'pragmaSpecialTest.rex'  dstDirNow
  cpCmd srcDirNow'pragmaTriggerTest.rex'  dstDirNow
  cpCmd srcDirNow'preparedStmtTest.rex'   dstDirNow
  cpCmd srcDirNow'statusTest.rex'         dstDirNow
  cpCmd srcDirNow'versionTest.rex'        dstDirNow

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
