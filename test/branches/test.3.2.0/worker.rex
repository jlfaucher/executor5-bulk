#!/usr/bin/rexx
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2007-2008 Rexx Language Association. All rights reserved.    */
/*                                                                            */
/* This program and the accompanying materials are made available under       */
/* the terms of the Common Public License v1.0 which accompanies this         */
/* distribution. A copy is also available at the following address:           */
/* http://www.ibm.com/developerworks/oss/CPLv1.0.htm                          */
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

/** worker.rex
 * This is the program that actually executes the test suite.  It is designed
 * to be invoked from testOORexx.rex, or a similar program.  testOORexx sets
 * the path to the framework files prior to invoking this program.  This allows
 * the test suite to be executed from anywhere on the file system without
 * needing the framework directories in the existing path.
 */
arguments = arg(1)

   parse source . . file
   overallPhase = .PhaseReport~new(file, .PhaseReport~AUTOMATED_TEST_PHASE)

   cl = .CommandLine~new(arguments)
   if cl~needsHelp then return cl~showHelp

   .local~bRunTestsLocally = .false

   searchPhase  = .PhaseReport~new(file, .PhaseReport~FILE_SEARCH_PHASE)
   msg = "Searching for test containers"
   if cl~noTicks then
     say msg
   else
     searchPhase~tickTock(msg)

   testResult = .ooRexxUnit.default.TestResult.Class~new
   testResult~noAutoTiming

   -- Set verbosity.
   testResult~setVerbosity(cl~getVerbosity)

   finder = .ooTestFinder~new(cl~root, cl~ext, cl~testTypes)
   if \ cl~simpleTestSelection then do
     finder~includeFiles(cl~testFile)
   end
   containers = finder~seek(testResult)

   -- Building the test suite takes very little time at this point.  No need to
   -- show ticks.
   testResult~addEvent(searchPhase~~done)
   suiteBuildPhase  = .PhaseReport~new(file, .PhaseReport~SUITE_BUILD_PHASE)

   suite = .ooTestSuite~new
   suite~showProgress = cl~showProgress
   suite~beVerbose = cl~verboseTestCase

   do container over containers
     if cl~testTypes == .nil then container~suite(suite)
     else container~suiteForTestTypes(cl~testTypes, suite)
   end

   testResult~addEvent(suiteBuildPhase~~done)

   executionPhase  = .PhaseReport~new(file, .PhaseReport~TEST_EXECUTION_PHASE)
   msg = 'Executing automated test suite'
   if cl~doTestCaseTicks then
     executionPhase~tickTock(msg)
   else
     say msg

   -- This next line is what it is all about.
   suite~execute(testResult)

   testResult~addEvent(executionPhase~~done)
   testResult~addEvent(overallPhase~~done)

   testResult~print("ooTest Framework - Automated Test of the ooRexx Interpreter")

return 0

::requires "ooTest.frm"

::class 'CommandLine' public inherit ooTestConstants NoiseAdjustable

::attribute version get
::attribute version set private
::attribute needsHelp get
::attribute needsHelp set private
::attribute root get
::attribute root set private
::attribute ext get
::attribute ext set private
::attribute testTypes get
::attribute testTypes set private

::attribute doLongHelp private
::attribute errMsg private
::attribute doVersionOnly private

::attribute verboseTestCase get             -- S
::attribute verboseTestCase set private
::attribute showProgress get                -- s
::attribute showProgress set private
::attribute noTicks get                     -- U
::attribute noTicks set private
::attribute noTestCaseTicks get             -- u
::attribute noTestCaseTicks set private

-- Don't need to look at patterns, multi-dirs, single file, etc..
::attribute simpleTestSelection get
::attribute simpleTestSelection set private

::attribute testFile get
::attribute testFile set private


::attribute optTable private

::method init
  expose cmdLine
  use arg cmdLine

  self~setAllDefaults

  if cmdLine == "" then return
  if self~hasHelpArg then return

  self~parse

  if self~errMsg \== .nil then return
  if self~doVersionOnly then do
    self~needsHelp = .true
    return
  end

  self~resolveTestTypes
  self~resolveFiles
  self~resolveOptions

::method showHelp
  expose errMsg

  say "testOORexx version" self~version "ooTest Framwork version" .ooTest_Framework_version
  if self~doVersionOnly then return self~TEST_SUCCESS_RC

  say
  if self~doLongHelp then return self~longHelp

  if errMsg == .nil then ret = self~TEST_HELP_RC
  else do
    ret = self~TEST_BADARGS_RC
    do line over errMsg
      say line
    end
    say
  end

  say "usage: testOORexx [OPTIONS]"
  say "Try 'testOORexx --help' for more information."

  return ret

::method getCommandLine
  expose originalCommandLine
  return originalCommandLine

/** doTestCaseTicks()  Convenience method. */
::method doTestCaseTicks
  if \ self~noTicks,  \ self~noTestCaseTicks, \ self~showProgress, \ self~verboseTestCase then
    return .true
  return .false

::method resolveTestTypes private
  expose optTable

  includes = optTable['testTypeIncludes']
  excludes = optTable['testTypesExcludes']
  if includes \== .nil | excludes \== .nil then do
    select
      when includes \== .nil, excludes \== .nil then self~testTypes = includes~difference(excludes)
      when includes \== .nil then self~testTypes = includes
      otherwise self~testTypes = .ooTestTypes~all~difference(excludes)
    end
    -- End select
  end

::method resolveFiles private
  expose optTable

  -- Here we would resolve files and directories.  Only have the single file
  -- option implemented so far.
  if optTable['file'] \== .nil then do
    self~simpleTestSelection = .false
    self~testFile = optTable['file']
  end

::method resolveOptions private
  expose optTable

  -- Here we would need to see if the command line options over-ride the
  -- settings in the options file.  But, the options file is not implemented,
  -- so we don't need to bother with that, yet.
  if optTable['showProgress'] \== .nil then self~showProgress = .true
  if optTable['showTestCases'] \== .nil then self~verboseTestCase = .true
  if optTable['suppressTicks'] \== .nil then self~noTestCaseTicks = .true
  if optTable['suppressAllTicks'] \== .nil then self~noTicks = .true


::method parse private
  expose cmdLine tokenCount errMsg originalCommandLine

  cmdLine = cmdLine~space(1)
  tokenCount = cmdLine~words

  done = self~checkFormat
  if done then return

  do i = 1 to tokenCount
    token = cmdLine~word(i)
    select
      when token~abbrev("--") then j = self~parseLongOpt(token, i)

      when token~abbrev("-") then j = self~parseShortOpt(token, i)

      otherwise do
        -- The error message list is not started at this point.
        self~errMsg = .list~of("Command line arguments must start with '-' or '--'")
        self~errMsg~insert("  Error at:" cmdLine~word(i))
        self~needsHelp = .true
        return
      end
    end
    -- End select

    if j < 0 then do
      -- The error message list *may* already have some messages.  We want these
      -- messaged to be first in the print out.
      if errMsg == .nil then errMsg = .list~new

      k = errMsg~insert("Bad command line", .nil)
      errMsg~insert("  CommandLine:" originalCommandLine, k)
      errMsg~insert("  Error at:   " cmdLine~word(i), k)
      self~needsHelp = .true
      return
    end
    i = j
  end
  -- End do

::method lastToken private
  expose tokenCount
  use strict arg index msg

  if index == tokenCount then do
    self~addErrorMsg(msg)
    return .true
  end
  return .false

::method isSingleValueToken private
  expose tokenCount
  use strict arg index msg

  if (index + 2) < tokenCount, \ self~isOptionToken(index + 2) then do
    self~addErrorMsg(msg)
    return .false
  end
  return .true

::method checkFileSegment private
  expose cmdLine
  use strict arg index

  if .ooTestConstants~SL == '\' then wrongSlash = '/'; else wrongSlash = '\'
  invalid = '*|[]:"<>?{}()+ ' || "'" || wrongSlash
  segment = cmdLine~word(index)

  pos = verify(segment, invalid, 'M')
  if pos <> 0 then do
    char = segment~substr(pos, 1)
    self~addErrorMsg("The file name segment:" segment "contains an invalid character ("char")")
    self~addErrorMsg("  File name segments can not contain invalid file name characters")
    self~addErrorMsg("  or regular expression characters.")
    return .false
  end
  return .true

::method parseShortOpt private
  expose cmdLine tokenCount optTable
  use arg word, i

  j = i

  select
    when word == '-f' then do
      if self~lastToken(i, "The -f option must be followed by a file name segment") then return -1
      if \ self~isSingleValueToken(i, "The -F option must be followed by a single file name segment") then return -1

      j += 1
      if \ self~checkFileSegment(j) then return -1

      optTable['file'] = cmdLine~word(j)
    end

    when word == '-F' then do
      return self~notImplemented("-F")
    end

    when word == '-p' then do
      return self~notImplemented("-p")
    end

    when word == '-R' then do
      if self~lastToken(i, "The -R option must be followed by a directory name") then return -1
      if \ self~isSingleValueToken(i, "The -R option must be followed by a single directory name") then return -1

      j += 1
      self~root = cmdLine~word(j) || self~SL
    end

    when word == '-s' then do
      optTable['showProgress'] = .true
    end

    when word == '-S' then do
      optTable['showTestCases'] = .true
    end

    when word == '-I' then do
      j = self~addTestTypes(i, '-I')
    end

    when word == '-u' then do
      optTable['suppressTicks'] = .true
    end

    when word == '-U' then do
      optTable['suppressAllTicks'] = .true
    end

    when word == '-v' then do
      self~doVersionOnly = .true
    end

    when word == '-V' then do
      if self~lastToken(i, "The -V option must be followed by the verbosity level") then return -1
      if \ self~isSingleValueToken(i, "The -V option must be followed by only 1 verbosity level") then return -1

      j += 1
      level = cmdLine~word(j)

      if \ isWholeRange(level, self~MIN_VERBOSITY, self~MAX_VERBOSITY) then do
        self~addErrorMsg("The -V option must be followed by a valid verbosity level; found" level)
        self~addErrorMsg("  Valid levels are in the range of" self~MIN_VERBOSITY "to" self~MAX_VERBOSITY)
        return -1
      end

      self~setVerbosity(level)
    end

    when word == '-X' then do
      j = self~addTestTypes(i, '-X')
    end

    otherwise do
      self~addErrorMsg( '"'cmdLine~word(i)'"' "is not a valid option")
      return -1
    end
  end
  -- End select

  return j

::method nextOptionIndex private
  expose cmdLine tokenCount
  use strict arg start

  do i = start to tokenCount
    if cmdLine~word(i)~abbrev("-") then return i
  end
  return 0

::method notImplemented private
  use strict arg opt
  self~addErrorMsg("The" opt "argument option is not implemented yet.")
  return -1

::method parseLongOpt private
  self~addErrorMsg("Long argument options are not implemented yet.")
  return -1

::method addTestTypes private
  expose cmdLine tokenCount optTable
  use strict arg i, opt

  if i == tokenCount | self~isOptionToken(i + 1) then do
    self~addErrorMsg("The" opt "option must be followed by at least 1 test type")
    return -1
  end

  j = i + 1
  nextOpt = self~nextOptionIndex(j)

  if nextOpt == 0 then do
    types = makeSetOfWords(cmdLine~subWord(j))
    j = tokenCount
  end
  else do
    types = makeSetOfWords(cmdLine~subWord(j, (nextOpt - j)))
    j = nextOpt - 1
  end

  tmp = .set~new
  do t over types
    testType = .ooTestTypes~testForName(t)
    if testType == .nil then do
      self~addErrorMsg("The" opt "option must be followed by valid test types")
      self~addErrorMsg(" " t "is not a valid test type.")
      self~addErrorMsg("  Valid types are:" .ooTestTypes~allNames)
      return -1
    end
    tmp~put(testType)
  end

  if opt == '-I' then optTable['testTypeIncludes'] = tmp
  else optTable['testTypesExcludes'] = tmp

  return j

::method isOptionToken private
  expose cmdLine tokenCount
  use strict arg i

  if i > tokenCount then return .false
  else return cmdLine~word(i)~abbrev("-")


/** addErrorMsg()  Adds msg to the end of the error messages list. */
::method addErrorMsg private
  expose errMsg
  use strict arg msg
  if errMsg == .nil then errMsg = .list~new
  errMsg~insert(msg)

/** checkFormat()
 * Checks that the first command line token starts with "-", or if not, that it
 * is a simple command line with one file name on it.
 *
 * Returns true if command line parsing is done, otherwise false.  Parsing would
 * be done if there is an error, or if there is only a single file name on the
 * command line.
 */
::method checkFormat private
  expose cmdLine tokenCount optTable

  if cmdLine~left(1) == "-" then return .false

  -- Want to eventually support:
  --  testFileName
  -- or
  --  directoryName testFile
  --
  -- For now just support testFileName
  if tokenCount > 1 then do
    self~addErrorMsg("Command line arguments must start with '-' or '--'")
    self~needsHelp = .true
  end
  else do
    optTable['file'] = cmdLine
  end

  return .true

::method setAllDefaults private
  expose cmdLine originalCommandLine

  originalCommandLine = cmdLine~copy

  self~version = "0.9.0"
  self~needsHelp = .false
  self~doLongHelp = .false
  self~errMsg = .nil
  self~doVersionOnly = .false

  self~setVerbosity(self~DEFAULT_VERBOSITY)
  self~root      = self~TEST_ROOT || self~SL
  self~testTypes = self~TEST_TYPES_DEFAULT
  self~ext       = self~TEST_CONTAINER_EXT

  self~verboseTestCase = .false
  self~showProgress = .false
  self~noTicks = .false
  self~noTestCaseTicks = .false
  self~simpleTestSelection = .true
  self~testFile = .nil

  self~optTable = .table~new

::method hasHelpArg private
  expose cmdLine

  tokens = makeSetOfWords(cmdLine)
  helpTokens = .set~of("-H", "/H", "--H", "--HELP", "/?", "?", "-?", "--?")
  intersect = helpTokens~intersection(tokens)

  if intersect~isEmpty then return .false

  if intersect~hasIndex("--HELP") then self~doLongHelp = .true
  self~needsHelp = .true
  return .true

::method longHelp private
  say 'Test the ooRexx interpreter using the automated ooTest framework.'
  say "usage: 1.  testOORexx"
  say "       2.  testOORexx fileName"
  say "       3.  testOORexx [OPTIONS]"
  say
  say '  1. With no options the entire automated test suite is executed.  No'
  say '     tests are exclued and the default verbosity and formatter are'
  say '     used.'
  say
  say '  2. The single test group specified by "fileName" is executed.'
  say
  say '  3. The automated test suite is executed using the specified options.'
  say
  say '  Options must start with "-" or "--"  Spaces are not tolerated in'
  say '  either file names or directory names.'
  say
  say 'Valid options:'
  say ' Test selection:'
  say '  -f  --file=NAME              Excute the single NAME test group'
  say '  -F  --files=N1 N2 ...        Execute the N1 N2 ... test groups'
  say '  -I, --test-types=T1 T2 ..    Include test types T1 T2 ...'
  say '  -p  --files-with-pattern=PA  Execute test groups matching PA'
  say '  -R, --root=DIR               DIR is root of search tree'
  say '  -X  --exclude-types=X1 X2 .. Exclude test types X1 X2 ...'
  say
  say ' Output control:'
  say '  -h                           Show short help'
  say '      --help                   Show long help (this help)'
  say '  -s  --show-progress          Show test group progress'
  say '  -S  --show-testcases         Show test case progress'
  say '  -u  --suppress-ticks         Do not show ticks during test execution'
  say '  -U  --suppress-all-ticks     Do not show any ticks'
  say '  -v, --version                Show version and quit'
  say '  -V, --verbose=NUM            Set vebosity to NUM'
  say

  return self~TEST_HELP_RC

