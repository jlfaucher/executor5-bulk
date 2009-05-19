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

   testResult = .ooRexxUnit.default.TestResult.Class~new
   testResult~noAutoTiming

   -- Set verbosity.
   testResult~setVerbosity(cl~getVerbosity)

   if cl~buildFirst then do
     -- Create a phase report here and send it along.
     ret = buildExternalBins(testResult, file, cl~forceBuild)
   end

   if cl~noTests then do
     overallPhase~done
     testResult~addEvent(overallPhase)

     testResult~print("ooTest Framework - Automated Test of the ooRexx Interpreter")
     return 0
   end

   searchPhase  = .PhaseReport~new(file, .PhaseReport~FILE_SEARCH_PHASE)
   msg = "Searching for test containers"
   if cl~suppressAllTicks then
     say msg
   else
     searchPhase~tickTock(msg)

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
   suite~beVerbose = cl~showTestCases

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

   executionPhase~done
   overallPhase~done

   testResult~addEvent(executionPhase)
   testResult~addEvent(overallPhase)

   testResult~print("ooTest Framework - Automated Test of the ooRexx Interpreter")

   if cl~waitAtCompletion then do
     say
     say "The automated test run is finished, hit enter to continue"
     pull
   end

return 0

::requires "ooTest.frm"

::routine buildExternalBins
  use arg testResult, file, force

  signal on syntax name loadErr
  .context~package~loadPackage('building.frm')

  ret = buildBinaries(testResult, force)

return .ooTestConstants~SUCCESS_RC

loadErr:
  err = .ExceptionData~new(timeStamp(), file, .ExceptionData~TRAP)
  err~setLine(sigl)
  err~conditionObject = condition('O')
  err~msg = 'Error loading the required framework for compiling ("building.frm")'
  testResult~addException(err)
return .ooTestConstants~FAILED_PACKAGE_LOAD_RC

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

::attribute showTestCases get                  -- S
::attribute showTestCases set private
::attribute showProgress get                   -- s
::attribute showProgress set private
::attribute suppressAllTicks get               -- U
::attribute suppressAllTicks set private
::attribute suppressTestcaseTicks get          -- u
::attribute suppressTestcaseTicks set private
::attribute buildFirst get                     -- b
::attribute buildFirst set private
::attribute forceBuild get                     -- B
::attribute forceBuild set private
::attribute noTests get                        -- n
::attribute noTests set private
::attribute waitAtCompletion get               -- w
::attribute waitAtCompletion set private

-- Don't need to look at patterns, multi-dirs, single file, etc..
::attribute simpleTestSelection get
::attribute simpleTestSelection set private

::attribute testFile get
::attribute testFile set private

::attribute testOpts private

::method init
  expose cmdLine testOpts
  use arg cmdLine

  testOpts = .directory~new
  self~setAllDefaults

  self~readOptionsFile

  .environment~testOpts = testOpts

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

  say "testOORexx version" self~version "ooTest Framework version" .ooTest_Framework_version
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
  if \ self~suppressAllTicks,  \ self~suppressTestcaseTicks, \ self~showProgress, \ self~showTestCases then
    return .true
  return .false

::method resolveTestTypes private
  expose testOpts

  includes = testOpts~testTypeIncludes
  excludes = testOpts~testTypesExcludes
  if includes \== .nil | excludes \== .nil then do
    select
      when includes \== .nil, excludes \== .nil then self~testTypes = includes~difference(excludes)
      when includes \== .nil then self~testTypes = includes
      otherwise self~testTypes = .ooTestTypes~all~difference(excludes)
    end
    -- End select
  end
  testOpts~testTypes = self~testTypes

::method resolveFiles private
  expose testOpts

  -- Here we would resolve files and directories.  Only have the single file
  -- option implemented so far.
  if testOpts~file \== .nil then do
    self~simpleTestSelection = .false
    self~testFile = testOpts~file
  end

::method resolveOptions private
  expose testOpts

  if testOpts~showProgress \== .nil then self~showProgress = testOpts~showProgress
  if testOpts~showTestCases \== .nil then self~showTestCases = testOpts~showTestCases
  if testOpts~suppressTestcaseTicks \== .nil then self~suppressTestcaseTicks = testOpts~suppressTestCaseTicks
  if testOpts~suppressAllTicks \== .nil then self~suppressAllTicks = testOpts~suppressAllTicks
  if testOpts~noTests \== .nil then self~noTests = testOpts~noTests
  if testOpts~waitAtCompletion \== .nil then self~waitAtCompletion = testOpts~waitAtCompletion
  if testOpts~buildFirst \== .nil then self~buildFirst = testOpts~buildFirst
  if testOpts~forceBuild \== .nil then self~forceBuild = testOpts~forceBuild

  if self~forceBuild then do
    self~buildFirst = .true
    testOpts~buildFirst = .true
  end

::method readOptionsFile private
  -- Not implemented yet.

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
  expose cmdLine tokenCount testOpts
  use arg word, i

  j = i

  select
    when word == '-a' then do
      testOpts~testTypeIncludes = .ooTestTypes~all
    end

    when word == '-b' then do
      testOpts~buildFirst = .true
    end

    when word == '-B' then do
      testOpts~buildFirst = .true
      testOpts~forceBuild = .true
    end

    when word~abbrev("-D")  then do
      j = self~addDefine(i)
    end

    when word == '-f' then do
      if self~lastToken(i, "The -f option must be followed by a file name segment") then return -1
      if \ self~isSingleValueToken(i, "The -f option must be followed by a single file name segment") then return -1

      j += 1
      if \ self~checkFileSegment(j) then return -1

      testOpts~file = cmdLine~word(j)
    end

    when word == '-F' then do
      return self~notImplemented("-F")
    end

    when word == '-n' then do
      testOpts~noTests = .true
    end

    when word == '-p' then do
      return self~notImplemented("-p")
    end

    when word == '-R' then do
      if self~lastToken(i, "The -R option must be followed by a directory name") then return -1
      if \ self~isSingleValueToken(i, "The -R option must be followed by a single directory name") then return -1

      j += 1
      self~root = cmdLine~word(j) || self~SL
      testOpts~testCaseRoot = self~root
    end

    when word == '-s' then do
      testOpts~showProgress = .true
    end

    when word == '-S' then do
      testOpts~showTestCases = .true
    end

    when word == '-I' then do
      j = self~addTestTypes(i, '-I')
    end

    when word == '-u' then do
      testOpts~suppressTestCaseTicks = .true
    end

    when word == '-U' then do
      testOpts~suppressAllTicks = .true
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
      testOpts~verbosity = level
    end

    when word == '-w' then do
      testOpts~waitAtCompletion = .true
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
  expose cmdLine tokenCount testOpts
  use strict arg i, opt

  if i == tokenCount | self~isOptionToken(i + 1) then do
    self~addErrorMsg("The" opt "option must be followed by at least 1 test type, or the word all.")
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
    if t~caselessCompare('all') == 0 then do
      tmp = .ooTestTypes~all
      leave
    end

    testType = .ooTestTypes~testForName(t)
    if testType == .nil then do
      self~addErrorMsg("The" opt "option must be followed by valid test types")
      self~addErrorMsg(" " t "is not a valid test type.")
      self~addErrorMsg("  Valid types are:" .ooTestTypes~allNames)
      self~addErrorMsg("  In addition, the keyword 'all' can be used to indicate every test type.")
      return -1
    end

    tmp~put(testType)
  end

  if opt == '-I' then index = testTypeIncludes
  else index = testTypesExcludes

  if testOpts[index] == .nil then testOpts[index] = tmp
  else testOpts[index] = testOpts[index]~union(tmp)

  return j

::method addDefine private
  expose cmdLine tokenCount testOpts
  use strict arg i

  token = cmdLine~subword(i, 1)
  if token~pos("=") == 0 then do
      self~addErrorMsg("The -D option must be in the following format:")
      self~addErrorMsg("  -Dname=value")
      self~addErrorMsg(" " token "is not valid.")
      return -1
  end

  nextOpt = self~nextOptionIndex(i + 1)

  if nextOpt == 0 then do
    define = cmdLine~subWord(i)
    i = tokenCount
  end
  else do
    define = cmdLine~subWord(i, (nextOpt - i))
    i = nextOpt - 1
  end

  define = define~substr(3)
  parse var define name "=" value
  testOpts[name~strip~upper] = value~strip

  return i


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
  expose cmdLine tokenCount testOpts

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
    testOpts~file = cmdLine
  end

  return .true

::method setAllDefaults private
  expose cmdLine originalCommandLine testOpts

  originalCommandLine = cmdLine~copy

  self~version = "1.1.0"
  self~needsHelp = .false
  self~doLongHelp = .false
  self~errMsg = .nil
  self~doVersionOnly = .false

  self~setVerbosity(self~DEFAULT_VERBOSITY)
  self~root      = self~TEST_ROOT || self~SL
  self~ext       = self~TEST_CONTAINER_EXT

  -- Get the default set of test types to always execute, defined in the .ooTestTypes class.
  -- The default format for the returned set is [C]onstant, i.e. the numeric test type values.
  self~testTypes = .ooTestTypes~defaultTestSet

  self~showTestCases = .false
  self~showProgress = .false
  self~suppressAllTicks = .false
  self~suppressTestcaseTicks = .false
  self~simpleTestSelection = .true
  self~testFile = .nil

  self~waitAtCompletion = .false

  self~buildFirst = .false
  self~forceBuild = .false
  self~noTests    = .false

  testOpts~version = self~version
  testOpts~verbosity = self~DEFAULT_VERBOSITY
  testOpts~testCaseRoot = self~root
  testOpts~testFileExt = self~ext
  testOpts~testTypes = self~testTypes
  testOpts~showTestCases = self~showTestCases
  testOpts~showProgress = self~showProgress
  testOpts~suppressAllTicks = self~suppressAllTicks
  testOpts~suppressTestcaseTicks = self~suppressTestcaseTicks
  testOpts~waitAtCompletion = self~waitAtCompletion
  testOpts~buildFirst = self~buildFirst
  testOpts~forceBuild = self~forceBuild
  testOpts~noTests = self~noTests

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
  say '  1. With no options the automated test suite is executed using the default'
  say '     set of test types, the default verbosity, and the default formatter.'
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
  say '  -a  --all-test-types         Include all test types'
  say '  -b  --build-first            Build external binaries before running tests'
  say '  -B  --force-build            Force building (implies -b)'
  say '  -f  --file=NAME              Excute the single NAME test group'
  say '  -F  --files=N1 N2 ...        Execute the N1 N2 ... test groups'
  say '  -I, --test-types=T1 T2 ..    Include test types T1 T2 ...'
  say '                               keyword "all" indicates all test types'
  say '  -n  --no-tests               No tests to execute (deliberately)'
  say '  -p  --files-with-pattern=PA  Execute test groups matching PA'
  say '  -R, --root=DIR               DIR is root of search tree'
  say '  -X  --exclude-types=X1 X2 .. Exclude test types X1 X2 ...'
  say
  say ' Output control:'
  say '  -h                             Show short help'
  say '      --help                     Show long help (this help)'
  say '  -s  --show-progress            Show test group progress'
  say '  -S  --show-testcases           Show test case progress'
  say '  -u  --suppress-testcase-ticks  Do not show ticks during test execution'
  say '  -U  --suppress-all-ticks       Do not show any ticks'
  say '  -v, --version                  Show version and quit'
  say '  -V, --verbose=NUM              Set vebosity to NUM'
  say '  -w, --wait-at-completion       At test end, wait for user to hit enter'
  say
  say ' Generic options:'
  say '  -D          Define option.  Format must be: -Dname=value'
  say

  return self~TEST_HELP_RC

