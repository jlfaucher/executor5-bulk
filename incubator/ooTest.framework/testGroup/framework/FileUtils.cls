#!/usr/bin/rexx
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2007-2008 Rexx Language Association. All rights reserved.    */
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
   name:             FileUtils.cls
   authors:          Mark Miesfeld
   last update:      2007-11-24

   purpose:          Provides common public routines and utility classes to do
                     file system related tasks, to make writing test units
                     easier.

   remarks:          This file has the requires directive for ooRexxUnit.cls.
                     When writing a test unit, use a requires directive for this
                     class and the requires directive for ooRexxUnit.cls is not
                     needed.

   category0:        ooRexxUnit
   category1:        framework
*/


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\
  Directives, Classes, or Routines.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::requires 'OOREXXUNIT.CLS'

/* createFile( src, name ) - - - - - - - - - - - - - - - - - - - - - - - - - -*\

  Writes out a file using the supplied source.

  Input:
    src   REQUIRED
      An array containing the lines to be written to the file.

    name  REQUIRED
      The name of the file to be written.

  Returns:
    The fully qualified name of the file on succes.  Returns the empty string
    on error.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::routine  createFile public
  use strict arg src, name

  fileName = ""
  fsObj = .stream~new(name)
  state = fsObj~open("WRITE REPLACE")
  if state~abbrev("READY") then do
    fsObj~arrayout(src)
    fsObj~close
    fileName = fsObj~qualify
  end

return fileName
-- End createFile( src, name )

/* Convenience method.  Calls createFile() with .rex tacked onto basename. */
::routine  createRexxPrgFile public
  use strict arg src, baseName
return createFile(src, baseName || '.rex')

/* deleteFile( fileName )- - - - - - - - - - - - - - - - - - - - - - - - - - -*\

  Provides a platform independent file delete.  On some platforms, SysFileDelete
  does not force a deletion.  This function will force the deletion where
  possible.

  Input:
    fileName REQUIRED
      The file to delete.

  Returns:
    The operating system return code when the delete is done.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::routine  deleteFile public
  use strict arg fileName

  -- On Linux (bash) and Windows the delete can be forced.  Not sure on other
  -- OSes
  select
    when .ooRexxUnit.OSName == "WINDOWS" then do
      'del /q /f' fileName '1>nul 2>&1'
      ret = RC
    end
    when .ooRexxUnit.OSName == "LINUX" then do
      'rm -f' fileName '>/dev/null 2>&1'
      ret = RC
    end
    otherwise ret = SysFileDelete(fileName)
  end

return ret
-- End deleteFile( fileName )

/* execRexxPrgWithArgs( prgName, params, output )- - - - - - - - - - - - - - -*\

  Executes a Rexx program using a separate instance of the interpreter.  This
  function captures the output and the return code from the executed program and
  returns it to the caller.

  Input:
    prgName REQUIRED
      The Rexx program to execute.

    params  REQUIRED
      The arguments to the Rexx program.  If the Rexx program has no arguments,
      either use the empty string, or use the convenience function execRexxPrg

    output  REQUIRED  [In / Out]
      An array object in which the the executed program's output is returned.
      The output lines are appended to the array, so the array does not need to
      be empty.

  Returns:
    The return code produced by executing the program on success.  Returns 9999
    for an internal error.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::routine  execRexxPrgWithArgs public
  use strict arg prgName, params, output

  if \ output~isInstanceOf(.array) then return 9999

  -- Only supported on Linux (bash) and Windows.  Other OSes or shells could
  -- be added here.  (Turns out Linux and Windows redirection is the same.)
  select
    when .ooRexxUnit.OSName == "WINDOWS" then stdErrToStdOut = '2>&1'
    when .ooRexxUnit.OSName == "LINUX" then stdErrToStdOut = '2>&1'
    otherwise return 9999
  end

  tmpOutFile = 'tmpXXX_delete.me'

  -- Could be spaces in the program path name, especially on Windows.
  prgName = prgName~strip
  if prgName~pos(' ') <> 0, prgName~left(1) \== '"' then prgName = enQuote(prgName)

  cmd = 'rexx' prgName params '>' tmpOutFile stdErrToStdOut
  cmd
  prgRet = RC

  fsObj = .stream~new(tmpOutFile)
  tmpArray = fsObj~arrayin
  do line over tmpArray
    output~append(line)
  end
  fsObj~close

  j = deleteFile(tmpOutFile)

return prgRet
-- End execRexxPrgWithArgs( prgName, params, output )

/**
 * This is a convenience method for execRexxPrgWithArgs().
 */
::routine  execRexxPrg public
  use strict arg prgName, output
return execRexxPrgWithArgs(prgName, "", output)


/* indirectRequire( fileName ) - - - - - - - - - - - - - - - - - - - - - - - -*\

  Provides a way for a test unit to indirectly 'require' (use the ::requires
  directive) a file.  This is best explained using an example.

  A test unit written to test a Windows specific feature of the interprerter,
  say the WindowsClipboard class, can test if the operating system is Windows
  before it executes any tests.  If the OS is not Windows, it simply returns an
  empty list and no tests are executed.

  However, if that test unit has a requires directive for a Windows only file,
  the test unit produces an error when run on a non-Windows OS because the
  required file is not found.  This error ocurrs before the OS test has a chance
  to execute.

  This function allows the test unit to first determine that the OS is Windows,
  then add the 'required' file.  This supports the ooRexx project goal of having
  the entire test suite run on any supported OS and not produce extraneous
  errors.  Although the example is Windows, the principle applies to any OS.

  Psuedo code to use this function:

  -- Test OS
  if not right OS then return empty list

  prgFile = indirectRequire(<fileToRequire>)
  if pgrFile == "" then return empty list  -- Some error happened.

  call (prgfile)       -- Now the requires directive for the file has executed.

  deleteFile(prgFile)  -- Delete the temporary file.

  Input:
    fileName   REQUIRED
      The file that is to be indirectly required.

  Returns:
    On success, the fully quailified file name of the Rexx program that will
    'require' the specified file.  The empty string is returned on error.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::routine indirectRequire public
  use strict arg fileName

  srcLines = .array~of('::requires' enQuote(fileName))
  fullName = createRexxPrgFile(srcLines, 'tmpIndirectRequire')

return fullName
-- End indirectRequire()

