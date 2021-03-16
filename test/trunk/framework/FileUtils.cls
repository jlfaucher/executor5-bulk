/*
  SVN Revision: $Rev: 3047 $
  Change Date:  $Date: 2008-08-22 19:56:58 -0700 (Fri, 22 Aug 2008) $
*/
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2007-2021 Rexx Language Association. All rights reserved.    */
/*                                                                            */
/* This program and the accompanying materials are made available under       */
/* the terms of the Common Public License v1.0 which accompanies this         */
/* distribution. A copy is also available at the following address:           */
/* https://www.oorexx.org/license.html                                        */
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

/** FileUtils.cls
 *
 * Provides common public routines and utility classes to do file system related
 * tasks, to make writing test units easier.
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

  address '' with output append using (output) error append using (output)
  address '' cmd
  return rc


/** locateSamplePrg()
 * Locates a sample program normally shipped with the ooRexx distribution.
 *
 * An attempt is made to find it using the assumption that the test is being
 * executed from within a build directory.  This allows the test suite to be
 * run by a developer from his build directory without having a regular
 * install.  The sample is also searched for in the location(s) for sample
 * programs in a normal install.
 *
 * @param   name REQUIRED
 *            The name of the sample program.
 *
 * @return  The complete path name of the sample if it is located, otherwise
 *          .nil.
 */
::routine locateSamplePrg public
  use strict arg name

  /*
  CMake build tree definitions
  set (CMAKE_SAMPLES_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/samples)
  set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
  CMake install definitions (Windows, Unix)
  set (INSTALL_SAMPLES_DIR samples)
  set (INSTALL_SAMPLES_DIR ${CMAKE_INSTALL_PREFIX}/share/${CMAKE_PROJECT_NAME})

  if the executable is ...               the samples are located in ...
  C:\Program Files...\ooRexx\rexx.exe    C:\Program Files...\ooRexx\samples\
  C:\...\ooRexxBuild\rel\bin\rexx.exe    C:\...\ooRexxBuild\rel\samples\
  /usr/local/bin/rexx.exe                /usr/local/share/ooRexx/
  /.../oorexxbuild/rel/bin/rexx.exe      /.../oorexxbuild/rel/samples/
  /Applications/.../bin/rexx             /Applications/.../share/ooRexx/ (?)
  /Users/.../Applications/.../bin/rexx   /Users/.../Applications/.../share/ooRexx/ (?)
  /Library/Frameworks/.../Commands/rexx  /Library/Frameworks/.../Shared/ooRexx/
  */

  install = .RexxInfo~executable~parent
  do samples over "samples/", "../samples/", "../share/ooRexx/", "../Shared/ooRexx/"
    sample = .File~new(samples || name, install)
    if sample~isFile then
      return sample~absolutePath
  end
  return .nil


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
::routine createFile public
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
::routine createRexxPrgFile public
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
::routine addToFile public
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

  Provides a platform independent file delete.

  Input:
    fileName REQUIRED
      The file to delete.

  Returns:
    The operating system return code when the delete is done.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::routine deleteFile public
  use strict arg fileName

  file = .File~new(fileName)
  file~setWritable
  return file~delete


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
::routine execRexxPrgWithArgs public
  use strict arg prgName, params, output

  cmd = 'rexx' .TestUtil~enQuote(prgName) params
  prgRet = issueCmd(cmd, output)

return prgRet
-- End execRexxPrgWithArgs( prgName, params, output )

/**
 * This is a convenience method to execute a Rexx program with no arguments.  It
 * delegates to execRexxPrgWithArgs().
 */
::routine execRexxPrg public
  use strict arg prgName, output
return execRexxPrgWithArgs(prgName, "", output)


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
::routine createOleObject public
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


::options all syntax error condition
