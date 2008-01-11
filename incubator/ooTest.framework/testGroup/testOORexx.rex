#!/usr/bin/rexx
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2005 Rexx Language Association. All rights reserved.         */
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

/**
 * testOORexx is a planned future replacement for runTestGroups.  It is not
 * fully implemented.
 */
arguments = arg(1)

   parse source . . file
   overallPhase = .PhaseReport~new(file, .PhaseReport~AUTOMATED_TEST_PHASE)

   cl = .CommandLine~new(arguments)
   if cl~needsHelp then return cl~showHelp

   .local~bRunTestsLocally = .false
   searchPhase  = .PhaseReport~new(file, .PhaseReport~FILE_SEARCH_PHASE)

   /*
   baseDir = "ooRexx"
   --baseDir = "ooRexx\samples"

   -- Set types to .nil to execute all types of tests.
   types = .nil


   -- Here, execute all test types, except the UNIT_LONG and the GUI_SAMPLE types.
   excluded = .set~of(.ooTestTypes~UNIT_LONG_TEST, .ooTestTypes~GUI_SAMPLE_TEST)
   types = .ooTestTypes~all~difference(excluded)

   say 'Will execute tests of type:'
   do type over types
     say ' ' .ooTestTypes~nameForTest(type)
   end
   */

   testResult = .ooRexxUnit.default.TestResult.Class~new
   testResult~noAutoTiming

   say 'test types:' cl~testTypes
   -- Set verbosity from 0 (least output) to 10 (most output)
   testResult~setVerbosity(cl~getVerbosity)

   finder = .ooTestFinder~new(cl~root, cl~ext, cl~testTypes)
   containers = finder~seek(testResult); say 'containers items:' containers~items

   testResult~addEvent(searchPhase~~done)
   suiteBuildPhase  = .PhaseReport~new(file, .PhaseReport~SUITE_BUILD_PHASE)

   suite = .ooTestSuite~new
   suite~showProgress = .false
   j = time('E')
   do container over containers
     if cl~testTypes == .nil then container~suite(suite)
     else container~suiteForTestTypes(cl~testTypes, suite)
   end

   testResult~addEvent(suiteBuildPhase~~done)

   executionPhase  = .PhaseReport~new(file, .PhaseReport~TEST_EXECUTION_PHASE)
   say 'Executing automated test suite.'
   suite~execute(testResult)

   testResult~addEvent(executionPhase~~done)
   testResult~addEvent(overallPhase~~done)

   testResult~print("My Title")

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
  end

::method getCommandLine
  expose originalCommandLine
  return originalCommandLine

::method parse private
  expose cmdLine tokenCount errMsg

  cmdLine = cmdLine~space(1)
  tokenCount = cmdLine~words

  done = self~checkFormat
  if done then return

  do i = 1 to tokenCount
    token = cmdLine~word(i); say 'i:' i "token:" token
    select
      when token~abbrev("--") then j = self~parseLongOpt(token, i)

      when token~abbrev("-") then j = self~parseShortOpt(token, i)

      otherwise do
        -- The error message list is not started at this point.
        self~errMsg = .list~of("Command line arguments must start with '-' or '--'")
        sefl~errMsg~insert("  Error at:" cmdLine~word(i))
        self~needsHelp = .true
        return
      end
    end
    -- End select
    say 'j is' j
    if j < 0 then do
      -- The error message list *may* already have some messages.  We want these
      -- messaged to be first in the print out.
      if errMsg == .nil then errMsg = .list~new

      k = errMsg~insert("Bad command line", .nil)
      errMsg~insert("  Error at:" cmdLine~word(i), k)
      self~needsHelp = .true
      return
    end
    i = j
  end
  -- End do

::method parseLongOpts private
  self~addErrorMsg("Long argument options are not implemented yet.")
  return -1

::method parseShortOpt private
  expose cmdLine tokenCount
  use arg word, i

  j = i

  select
    when word == '-v' then do
      self~doVersionOnly = .true
    end

    when word == '-R' then do
      if i == tokenCount then do
        self~addErrorMsg("The -R option must be followed by a directory name")
        j = -1; leave
      end

      j += 1
      if (j + 1) < tokenCount~words, \ self~tokenIsOption(j + 1) then do
        self~addErrorMsg("The -R option must be followed by a single directory name")
        j = -1; leave
      end

      self~root = cmdLine~word(j) || self~SL
    end

    when word == '-T' then do
      j = self~addTestTypes(i, '-T')
    end

    when word == '-V' then do
      if i == tokenCount then do
        self~addErrorMsg("The -V option must be followed by the verbosity level")
        j = -1; leave
      end

      j += 1
      if (j + 1) < tokenCount~words, \ self~tokenIsOption(j + 1) then do
        self~addErrorMsg("The -V option must be followed by only 1 verbosity level")
        j = -1; leave
      end

      level = cmdLine~word(j)
      if \ isWholeRange(level, self~MIN_VERBOSITY, self~MAX_VERBOSITY) then do
        self~addErrorMsg("The -V option must be followed by a valid verbosity level; found" level)
        self~addErrorMsg("  Valid levels are in the range of" self~MIN_VERBOSITY "to" self~MAX_VERBOSITY)
        j = -1; leave
        self~setVerbosity(level)
      end
    end

    when word == '-X' then do
      j = self~addTestTypes(i, '-X')
    end

    otherwise return do
      self~addErrorMsg( '"'cmdLine~word(i)'"' "is not a valid option")
      j = -1
  end
  -- End select

  return j

::method addTestTypes
  expose cmdLine tokenCount optTable
  use strict arg i, opt

  if i == tokenCount then do
    self~addErrorMsg("The" opt "option must be followed by at least 1 test type")
    return -1
  end

  j = i + 1
  endPos = cmdLine~wordPos(" -", j)
  if endPos = 0 then endPos = tokenCount

  types = makeSetOfWords(cmdLine~subWord(j, (endPos - j + 1)))
  j = endPos

  tmp = .set~new
  do t over types
    say 't' t
    testType = .ooTestTypes~testForName(t)
    say 'testType' testType
    if testType == .nil then do
      self~addErrorMsg("The" opt "option must be followed by valid test types")
      self~addErrorMsg(" " t "is not a valid test type.")
      self~addErrorMsg("  Valid types are:" .ooTestTypes~allNames)
      return -1
    end
    tmp~put(testType)
  end

  if opt == '-T' then optTable['testTypeIncludes'] = tmp
  else optTable['testTypesExcludes'] = tmp

  return j

::method isOptionToken private
  expose cmdLine tokenCount
  use strict arg i

  if i > tokenCount then return .false
  else return cmdLind~word(i)~abbrev("-")


/** addErrorMsg()  Adds msg to the end of the error messages list. */
::method addErrorMsg private
  expose errMsg
  use strict arg msg
  if errMsg == .nil then errMsg = .list~new
  errMsg~insert(msg)

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

::method longHelp private
  say "usage: testOORexx [OPTIONS]"
  say 'Test the ooRexx interpreter using OPTIONS.  With no options runs'
  say 'the entire automated test suite.'
  say
  say 'Options must start with "-" or "--"'
  say
  say 'Test selection:'
  say '  -R, --root=DIR               DIR is root of search tree'
  say '  -T, --test-types=T1 T2 ..    Include test types T1 T2 ...'
  say '  -X  --exclude-types=X1 X2 .. Exclude test types X1 X2 ...'
  say
  say 'Output control:'
  say '  -h                           Show short help'
  say '      --help                   Show this help'
  say '  -v, --version                Show version and quit'
  say '  -V, --verbose=NUM            Set vebosity to NUM'


  return self~TEST_HELP_RC

