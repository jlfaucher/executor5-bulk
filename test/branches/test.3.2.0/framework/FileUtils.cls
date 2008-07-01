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


/** issueCmd()
 * Issues a command and returns its output and return code.
 *
 * @param cmd     REQUIRED
 *   The command to issue.  This should include any arguments to the command.
 *
 * @param output  REQUIRED [In/Out]
 *   An array.  The output produced by issuing the command is returned in this
 *   array.  The output is appended to the array.
 *
 * @return  Returns the return code produced by issuing the command.  However,
 *          9999 is returned as an error indication.
 */
::routine issueCmd public
  use strict arg cmd, output

  if \ output~isInstanceOf(.array) then return 9999

  -- This is only known to be good for Linux and Windows.  When issued under
  -- another OS, it is likely to fail.  The select is used so that the proper
  -- code can be placed here when it does fail.
  select
    when .ooRexxUnit.OSName == "WINDOWS" then stdErrToStdOut = '2>&1'
    when .ooRexxUnit.OSName == "LINUX" then stdErrToStdOut = '2>&1'
    otherwise stdErrToStdOut = '2>&1'
  end

  tmpOutFile = 'tmpXXX_delete.me'

  cmd = cmd '>' tmpOutFile stdErrToStdOut
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


/** locateSamplePrg()
 * Locates a sample program normally shipped with the ooRexx distribution.
 *
 * The sample is searched for in the location(s) for samples programsin a normal
 * install.  If it is not found, an attempt to find it using the assumption that
 * the test is being executed from within a build directory.  This second search
 * allows the test suite to be run by a developer from his build directory
 * without having a regular install.
 *
 * @param   REQUIRED name  The name of the sample program
 * @return  The complete path name of the sample if it is located, otherwise
 *          .nil.
 */
::routine locateSamplePrg public
  use strict arg name

  sl = .ooRexxUnit.directory.separator
  retObj = .nil

  -- We have several possiblities here, but if REXX_HOME is set we just assume
  -- it is set correctly.  Currently, REXX_HOME is only set on Windows.

  currentOS = .ooRexxUnit.OSName
  home = value("REXX_HOME", , 'ENVIRONMENT')

  select label FINDSAMPLE
    when home \== "" then do
      j = SysFileTree(home || sl || 'samples' || sl || name, f., 'FOS')
      if j == 0, f.0 == 1 then retObj = f.1
    end

    when currentOS == "WINDOWS" then do
      currentWorkingDirectory = directory()
      buildRoot = findBuildRoot(currentOS)

      if buildRoot \== "" then do
        sampleDir = buildRoot || "\samples\"

        if SysIsFileDirectory(sampleDir) then do
          -- Okay, we are good (in all probability.)
          j = SysFileTree(sampleDir || name, f., 'FOS')
          if j == 0, f.0 == 1 then retObj = f.1
        end
      end
      j = directory(currentWorkingdirectory)
    end

    when currentOS == "LINUX" then do
      -- See if ooRexx is installed from a some type of package manager.
      retObj = sampleFromPackageInstall(name)

      if retObj \== .nil then leave FINDSAMPLE

      -- Not found, see if it is a user install in /opt
      if SysIsFileDirectory("/opt/ooRexx") then do
        j = SysFileTree("/opt/ooRexx/share/ooRexx/" || name, f., 'FOS')
        if j == 0, f.0 == 1 then retObj = f.1
        leave FINDSAMPLE
      end

      -- Not found, see if we can get a build directory
      currentWorkingDirectory = directory()
      buildRoot = findBuildRoot(currentOS)

      if buildRoot \== "" then do
        sampleDir = buildRoot || "/samples/"

        if SysIsFileDirectory(sampleDir) then do
          -- Okay, we are good (in all probability.)
          j = SysFileTree(sampleDir || name, f., 'FOS')
          if j == 0, f.0 == 1 then retObj = f.1
        end
      end
      j = directory(currentWorkingdirectory)
    end

    otherwise do
      -- When the test suite is starting to be run on other OSes this will
      -- have to be filled in.
      nop
    end
  end findSample -- End select

return retObj
-- End locateSample()

/** sampleFromPackageInstall()
 * A private helper function locate a sample program on a Linux system where
 * ooRexx has been installed by a package managing system.  Currently only works
 * for rpm and debian (dpkg.)
 *
 * returns .nil if not found.
 */
::routine sampleFromPackageInstall
  use strict arg name

  -- First decide if rpm or dpkg is on the system.
  cmdOut = .array~new
  ret = issueCmd('which rpm', cmdOut)
  haveRPM = (ret == 0)

  cmdOut~empty
  ret = issueCmd('which dpkg', cmdOut)
  haveDPKG = (ret == 0)

  if \ haveRPM, \ haveDPKG then return .nil

  if haveRPM then sample = checkRpmForSample(name, cmdOut~empty)
  if sample \== .nil then return sample

  if haveDPKG then return checkDpkgForSample(name, cmdOut~empty)

::routine checkRpmForSample
  use strict arg name, cmdOut

  sample = .nil
  ret = issueCmd('rpm -q ooRexx', cmdOut)

  if ret == 0 then do
    cmdOut~empty

    -- Escape the dot(s) in the sample program name
    escName = escapeDots(name)

    ret = issueCmd('rpm -q ooRexx --list | grep' escName, cmdOut)
    if cmdOut~items == 1 then sample = cmdOut[1]
  end
  return sample

::routine checkDpkgForSample
  use strict arg name, cmdOut

  sample = .nil
  ret = issueCmd('dpkg -s oorexx', cmdOut)

  if ret == 0 then do
    cmdOut~empty

    -- Escape the dot(s) in the sample program name
    escName = escapeDots(name)

    ret = issueCmd('dpkg -L oorexx | grep' escName, cmdOut)
    if cmdOut~items == 1 then sample = cmdOut[1]
  end
  return sample

::routine escapeDots
  use strict arg name

  escName = ""
  tmp = name
  do which tmp~pos('.') <> 0
    parse var tmp front '.' tmp
    escName = escName || front || '\.'
  end

  return escName || tmp


/** findBuildRoot()  A private helper function. */
::routine findBuildRoot
  use strict arg os

  buildRoot = ""

  tmp = directory(.ooTest.originalWorkingDir)
  if tmp \== "" then do
    select
      when os == "WINDOWS" then do
        tmp = directory("..\")
        if tmp \== "" then buildRoot = directory()
      end
      when os == "LINUX" then buildRoot = directory()
      otherwise nop
    end
    -- End select
  end
return buildRoot


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

/* addToFile( src, name )- - - - - - - - - - - - - - - - - - - - - - - - - - -*\

  Appends to an existing file using the supplied source.

  Input:
    src   REQUIRED
      An array containing the lines to be added to the file.

    name  REQUIRED
      The name of the file being appended.

  Returns:
    The fully qualified name of the file on succes.  Returns the empty string
    on error.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::routine  addToFile public
  use strict arg src, name

  fileName = ""
  fsObj = .stream~new(name)
  state = fsObj~open("WRITE APPEND")
  if state~abbrev("READY") then do
    do line over src
      fsObj~lineout(line)
    end
    fsObj~close
    fileName = fsObj~qualify
  end

return fileName
-- End addToFile( src, name )

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

  cmd = 'rexx' .TestUtil~enQuote(prgName) params
  prgRet = issueCmd(cmd, output)

return prgRet
-- End execRexxPrgWithArgs( prgName, params, output )

/**
 * This is a convenience method to execute a Rexx program with no arguments.  It
 * delegates to execRexxPrgWithArgs().
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

  srcLines = .array~of('::requires' .TestUtil~enQuote(fileName))
  fullName = createRexxPrgFile(srcLines, 'tmpIndirectRequire')

return fullName
-- End indirectRequire()


/* createOleObject( id ) - - - - - - - - - - - - - - - - - - - - - - - - - - -*\

  Creates an .OLEObject instance, a proxy for the specified COM object.  This
  routine is used to trap the REXX error that happens when the proxied COM
  object can not be created.

  Input:
    id          REQUIRED
      The string used to create the COM object.  I.e., the ProgID or CLSID.

    withEvents  OPTIONAL
      If true, create the OLE object with events, otherwise without events.  The
      default is false.

    beVerbose   OPTIONAL
      If true and the OleObject is not created, the error message is displayed.
      If false, the default, the message is not displayed.

      Use this option for test case development only.  Tests run for an
      automated test should not produce output.

  Returns:
    An instance of .OLEObject on success, .nil on failure.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::routine  createOleObject public
  use strict arg id, withEvents = .false, beVerbose = .false

  if \ isBoolean(withEvents) then
    raise syntax 88.900 array ("withEvents must be set to true or false; found:" withEvents)

  if \ isBoolean(beVerbose) then
    raise syntax 88.900 array ("beVerbose must be set to true or false; found:" beVerbose)

  signal on syntax name returnNil

  if withEvents then oleObject = .OLEObject~new(id, "WITHEVENTS")
  else oleObject = .OLEObject~new(id, "NOEVENTS")

  return oleObject

returnNil:
  if beVerbose then do
    cObj = condition("O")
    say "Error" rc":    " errortext(rc)
    say "Code " cObj~code":" cObj~message
  end

  return .nil
-- End createOleObject( id, verbose )


/** testForOleObject()
 * Provides a quick check to see if there would be a problem creating an
 * OLEObject on the current system.  (For example if the OLE Automation
 * application is not installed.)
 *
 * @param id  REQUIRED
 *   The string used to create the OLE Automation object.  I.e., the ProgID or
 *   CLSID.
 *
 * @return  Returns 0 if there are no problems, otherwise the syntax error
 *          code produced by the failure to create the OLE Automation object.
 */
::routine testForOleObject public
  use strict arg id

  signal on syntax name returnCode

  oleObject = .OLEObject~new(id, "NOEVENTS")
  drop oleObject
  return 0

returnCode:
  return condition('O')~code
