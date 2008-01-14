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

/*
   name:             ooTest.frm
   authors:          Mark Miesfeld
   date:             2007-12-05
   version:          1.0.0

   purpose:          An extension to the ooRexxUnit framework providing function
                     and features specific to testing the ooRexx interpreter and
                     its distribution package.

                     Although others may find this framework useful, its primary
                     design goal is to fit the needs of the ooRexx development
                     team.  Classes in this framework are not guaranteed to be
                     backwards compatible with previous versions of this
                     framework as the ooRexx committers may decide to break
                     compatibility to further the goals of the project.

   category:         Framework
*/

if \ .local~hasEntry('OOTEST_FRAMEWORK_VERSION') then do
  .local~ooTest_Framework_version = 1.0.0_3.2.0

  -- Replace the default test result class in the environment with the ooRexx
  -- project's default class.
  .local~ooRexxUnit.default.TestResult.Class = .ooTestResult

  -- Capture the ooTest framework directory and ensure it is in the path.
  parse source . . fileSpec
  .local~ooTest.dir = fileSpec~left(fileSpec~caseLessPos("ooTest.frm") - 2 )
  j = addToPath(.ooTest.dir)

  -- If not already in the environment, save the current working directory.
  if \ .local~hasEntry(ooTest.originalWorkingDir) then
    .local~ooTest.originalWorkingDir = directory()

end
-- End of entry point.

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\
  Directives, Classes, or Routines.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::requires "OOREXXUNIT.CLS"
::requires "rxregexp.cls"

::routine makeSetOfWords public
  use strict arg wordCollection, upper = .true

  if \ isBoolean(upper) then upper = .true

  s = .set~new
  select
    when wordCollection~isA(.string) then do w over wordCollection~space(1)~makearray(" ")
      if upper then s~put(w~translate)
      else s~put(w)
    end

    when wordCollection~isA(.collection) then do w over wordCollection
      if \ w~isA(.string) then return .nil
      if w~words <> 1 then return .nil
      if upper then s~put(w~translate)
      else s~put(w)
    end

    otherwise return .nil
  end
  -- End select

return s
-- End makeSetOfWords()

::routine makeArrayOfWords public
  use strict arg wordCollection, upper = .true

  if \ upper~isA(.string) then upper = .true
  if \ upper~dataType('O') then upper = .true

  a = .array~new
  select
    when wordCollection~isA(.string) then do w over wordCollection~space(1)~makearray(" ")
      if upper then a~append(w~translate)
      else a~append(w)
    end

    when wordCollection~isA(.collection) then do w over wordCollection~allItems
      if \ w~isA(.string) then return .nil
      if upper then a~append(w~translate)
      else a~append(w)
    end

    otherwise return .nil
  end
  -- End select

return a
-- End makeArrayOfWords()


/** class:  TestContainer
 * Defines an interface for a test container.  Objects containing tests that
 * implement the TestContainer interface can be 'found' by the ooTestFinder
 * class.
 */
--::class 'TestContainer' public
::class 'TestContainer' public mixinclass Object

/** isEmpty() Returns true or false.  True if the container has no tests,
 * otherwise false.
 */
::method isEmpty abstract

/** hasTests() Returns true or false. True if the container contains some
 * executable tests, otherwise false.  Note that containing executable tests is
 * not the same as simply containing tests.
 */
::method hasTests abstract

/** hasTestTypes() Returns true or false.  When passed an object as arg 1, the
 * test container determines if it does or does not have tests under the
 * constraints of the object.
 */
::method hasTestTypes abstract

/** testCount() Returns the number of tests the container has.
 */
::method testCount abstract

/** getNoTestsReason() Returns a descriptive string, presumably explaining why
 * the container has no executable tests.
 */
::method getNoTestsReason abstract


/** class:  ooTestCollectingParameter
  *   Defines an interface extended from TestResult for a collecting parameter
  *   used specifically for testing the ooRexx intepreter and its distribution
  *   packages.
  * DFX TODO finish up doc here.
  */
::class 'ooTestCollectingParameter' public subclass TestResult
::method addNotification    abstract
::method getNotifications   abstract
::method notificationCount  abstract
::method addException       abstract
::method getExceptions      abstract
::method exceptionCount     abstract
::method addEvent           abstract
::method getEvents          abstract
::method eventCount         abstract


/* class: ooTestConstants- - - - - - - - - - - - - - - - - - - - - - - - - - -*\
    A class containing constants used in testing ooRexx.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::class 'ooTestConstants' public mixinclass Object

  ::method TEST_ROOT  class; return "ooRexx"
  ::method TEST_ROOT;        return "ooRexx"

  ::method TEST_CONTAINER_EXT  class; return ".testGroup"
  ::method TEST_CONTAINER_EXT;        return ".testGroup"

  -- .nil signals no restriction on test types.
  ::method TEST_TYPES_DEFAULT  class; return .nil
  ::method TEST_TYPES_DEFAULT;        return .nil

  ::method TEST_SUCCESS_RC   class; return 0
  ::method TEST_SUCCESS_RC;         return 0
  ::method TEST_HELP_RC      class; return 1
  ::method TEST_HELP_RC;            return 1
  ::method TEST_FAILURES_RC  class; return 2
  ::method TEST_FAILURES_RC;        return 2
  ::method TEST_ERRORS_RC    class; return 3
  ::method TEST_ERRORS_RC;          return 3
  ::method TEST_NO_TESTS_RC  class; return 4
  ::method TEST_NO_TESTS_RC;        return 4
  ::method TEST_BADARGS_RC   class; return 5
  ::method TEST_BADARGS_RC;         return 5

  -- SL (back SLash or forward SLash) abbreviation for the directory separator.
  ::method SL  class; return .ooRexxUnit.directory.separator
  ::method SL;        return .ooRexxUnit.directory.separator

/* class: ooTestTypes- - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\
    A class containing the constants for the test types supported by the ooTest
    framework.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::class 'ooTestTypes' public mixinclass Object

  ::method MIN_TEST_TYPE  class; return 1
  ::method MIN_TEST_TYPE;        return 1

  ::method UNIT_TEST    class; return 1
  ::method UNIT_TEST;          return 1
  ::method DEFAULT_TEST class; return 1
  ::method DEFAULT_TEST;       return 1

  ::method UNIT_LONG_TEST   class; return 2
  ::method UNIT_LONG_TEST;         return 2
  ::method SAMPLE_TEST      class; return 3
  ::method SAMPLE_TEST;            return 3
  ::method GUI_SAMPLE_TEST  class; return 4
  ::method GUI_SAMPLE_TEST;        return 4
  ::method DOC_EXAMPLE_TEST class; return 5
  ::method DOC_EXAMPLE_TEST;       return 5

  -- A test type that makes noise.  I frequently need to run the test suite in
  -- an environment where I need these types of test cases to be skipped.  This
  -- is a convenience for myself (Mark Miesfeld.)
  ::method DOC_EXAMPLE_NOISE_TEST   class; return 6
  ::method DOC_EXAMPLE_NOISE_TEST;         return 6

  ::method MAX_TEST_TYPE  class; return 6
  ::method MAX_TEST_TYPE;        return 6

  /** all()
   * Returns a set of all the test types possible.
   */
  ::method all class
    all = .set~new
    do i = self~MIN_TEST_TYPE to self~MAX_TEST_TYPE
      all~put(i)
    end
    return all

  ::method all
    return self~class~all

  /** allNames()
   * Returns a string of all the test type names separated by blanks.
   */
  ::method allNames class
    expose names

    if names~default == .nil then self~populate
    return self~namesString

  ::method allNames
    return self~class~allNames

  /** testForName()
   * Returns the numeric test type constant for the specified name, or .nil if
   * there is no such test type.
   */
  ::method testForName class
    expose names
    use strict arg name

    if names~DEFAULT == .nil then self~populate
    return names~entry(name~upper)

  ::method testForName
    use strict arg name
    return self~class(name)

  /** nameForTest()
   * Returns the string name corresponding to a numeric test type constant, or
   * .nil if there is no such test type.
   */
  ::method nameForTest class
    expose names
    use strict arg test

    if names~DEFAULT == .nil then self~populate
    return names~entry(test)

  /** namesForTests()
   * Returns a string containing the test names for all the corresponding
   * numeric test type constants in a collection.  Returns .nil if: one of the
   * items in the collection is not a test type, the argument is not a
   * collection.
   */
  ::method namesForTests class
    expose names
    use strict arg tests

    if \ tests~isA(.Collection) then return .nil

    if names~DEFAULT == .nil then self~populate

    names__ = ""
    do t over tests
      name = names~entry(t)
      if name == .nil then return .nil
      names__ = names__ name
    end

    return names__

  ::method namesForTests
    use strict arg tests
    return self~class~namesForTests(tests)

  ::attribute names get class
  ::attribute names set class private
  ::attribute namesString get class
  ::attribute namesString set class private

  ::method init class
    expose names namesString
    names = .directory~new
    namesString = ""

  ::method populate class private
    expose names namesString
    itr = self~methods(.nil)
    do while itr~available
      name = itr~index
      if name~right(5) == "_TEST" then do
        name = name~left(name~length - 5)
        number = itr~item~source[1]~word(2)

        n = name~lower(2)
        names~setEntry(name, number)
        names~setEntry(number, n)
        namesString = namesString n
      end
      itr~next
    end

-- End of class ooTestTypes


/* class: ooTestTypes- - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\
    A class containing the constants for the test types supported by the ooTest
    framework.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/* class: ooTestCase - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\

    ooTestCases are used to test the ooRexx interpreter package.  An ooTestCase
    class is a test class where methods of the class define individual test
    cases.  In order to make it easy to construct automated tests with large
    numbers of test cases, a convention is followed:

    Each method of an ooTestCase class that starts with 'test' is considered
    an individual test case.

    Each ooTestCase has a class attribute defining the test type of the
    individual test cases the class contains.  (This list is still being
    defined)

    UNIT SAMPLE GUI_SAMPLE DOC_EXAMPLE STRESS

\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::class 'ooTestCase' public subclass TestCase inherit ooTestTypes

  -- The ooTestType attribute is the type of test cases contained in this test
  -- case class.  The default type is set here.  Test case writers need to
  -- over-ride the class init() to provide the the test case type when the
  -- default is not appropriate.
  ::attribute ooTestType get class
  ::attribute ooTestType set class private

  ::method init class
    forward class (super) continue

    -- Use the ooTestResult as the default test result.
    self~defaultTestResultClass = .ooTestResult

    -- Set the type of test cases this class contains to the default.
    self~ooTestType = .ooTestTypes~DEFAULT_TEST

  -- End init( )

-- End of class: ooTestCase


/* class: ooTestSuite- - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\

\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::class 'ooTestSuite' public subclass TestSuite

  ::attribute showProgress get
  ::attribute showProgress set
    expose showProgress
    use strict arg show

    if \ isBoolean(show) then
      raise syntax 88.900 array ("showProgess must be set to true or false; found:" show)
    showProgress = show

  ::attribute beVerbose get
  ::attribute beVerbose set
    expose beVerbose
    use strict arg verbose

    if \ isBoolean(verbose) then
      raise syntax 88.900 array ("beVerbose must be set to true or false; found:" verbose)
    beVerbose = verbose

  ::method init
    forward class (super) continue

    self~showProgress = .false
    self~beVerbose = .false
  -- End init( )

  /** execute()
   * Executes the tests in this test suite.  Over-rides the superclass method.
   *
   * @param testResult    OPTIONAL    (ooTestCollectingParameter)
   *   The CollectingParameter object (a test result in plain words) to use for
   *   the execution of the tests.  The default ooTest framework test result is
   *   used if this argument is omitted.  (Which is most likely .ooTestResult)
   *
   * @return  Returns the test result object used for the execution of the
   *          tests.
   */
  ::method execute
    use arg testResult = (self~createResult), verbose = (self~beVerbose)

    if \ isBoolean(verbose) then
      raise syntax 88.916 array ("2 'verbose'", "true or false", verbose)

    if verbose then
      say "Executing testSuite" pp(self~string"@"self~identityHash) "with" pp(self~countTestCases) "test cases ..."

    tests = self~testQueue

    -- If we are already verbose, we don't need to show progress.
    show = (self~showProgress & \verbose)

    -- Mark the test starting and invoke the test suite setUp method.
    testResult~startTest(self)
    self~setUp(testResult)

    do test over tests while \ testResult~shouldStop
       if show then say "Executing tests from" pathCompact(test~definedInFile, 58)
       test~execute(testResult, verbose)
    end

    -- Invoke the the test suite tearDown method and mark the test has ended.
    self~tearDown
    testResult~endTest(self)

    return testResult
  -- End run()

-- End of class: ooTestSuite


/* class: ConsoleFormatter - - - - - - - - - - - - - - - - - - - - - - - - - -*\

    A console formatter formats the information from a test result and prints
    it out to the console.

    The format of the information is designed to be "console-friendly."  The
    information is broken up into lines, with an attempt made to keep all lines
    no longer than 80 characers wide.

    ConsoleFormatter works with an ooTestResult and therefore has more
    infomation available to it than SimpleConsoleFormatter.  This allows for
    more comprehensive reporting.

\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::class 'ConsoleFormatter' public subclass SimpleConsoleFormatter inherit NotificationTypes

  ::attribute rexxVersion get
  ::attribute rexxVersion set private
  ::attribute unitVersion get
  ::attribute unitVersion set private
  ::attribute ooTestVersion get
  ::attribute ooTestVersion set private

  ::attribute failTable private
  ::attribute notifications private

  /** init()
   *
   */
  ::method init
    use strict arg testResult, title = ""
    forward class (super) continue

    -- We need an ooTestResult.
    if \ isSubClassOf(testResult~class, "ooTestResult") then
       raise syntax 88.914 array ("1 'testResult'", "ooTestResult")
    self~testResult = testResult

    parse version self~rexxVersion
    self~unitVersion = .ooRexxUnit.version
    self~ooTestVersion = .ooTest_Framework_version

    self~failTable = .nil
    self~notifications = .nil

  /** printBrief()
   * The least possible print out.
   */
  ::method printBrief private
    use arg tResult, fails

    say
    say ' '~copies(20) 'ooTest'
    say '  Tests:   ' tResult~runCount
    say '  Failures:' fails['newCount']
    say '  Errors:  ' tResult~errorCount + tResult~exceptionCount
    say

  /** print()
   *
   * Prints the data collected by this test result in a "console-friendly"
   * manner.
   *
   * @param title    OPTIONAL    (String)
   *   Adds a title to the output.  Resets the default title for this
   *   formatter to that specified.
   *
   * @param level   OPTIONAL    (Whole Number)
   *   Sets the verbosity level of the print out.  Resets the default verbosity
   *   for this fromatter.
   */
  ::method print
    use arg title = (self~title), level = (self~getVerbosity)

    if arg(1, 'E') then self~setTitle(title)
    if arg(2, 'E') then self~setVerbosity(level)

    tResult = self~testResult
    verbose = self~getVerbosity

    if self~failTable == .nil then self~failTable = tResult~getExtendedFailureInfo
    if self~notifications == .nil then self~notifications = tResult~getNotifications

    if verbose == 0 then do
      self~printBrief(tResult, self~failTable)
      return
    end

    if self~title<>"" then do
      say
      say self~title
      say
    end

    stats = self~calcStats
    self~printSummary(stats)

    if tResult~failureCount > 0 then do
      if verbose < 7 then do data over self~failTable['newQ']
        self~printFailureInfo(data)
      end
      else do data over tResult~failures
        self~printFailureInfo(data)
      end
    end

    if tResult~errorCount > 0 then do data over tResult~errors
      self~printErrorInfo(data)
    end

    if tResult~exceptionCount > 0 then do data over tResult~getExceptions
      self~printExceptions(data)
    end

    if verbose > 3 then self~printSkippedFiles

    if verbose > 5 then self~printMessages

    -- If a number of failure or error information lines are printed, re-display
    -- the summary statistics again so that the number of failures is obvious to
    -- the user.
    if stats~totalProblems > 3 | verbose > 3 then
      self~printSummary(stats)

    if tResult~eventCount <> 0 then do
      events = tResult~getEvents
      holder = .array~new
      do e over events
        select
          when e~id == .PhaseReport~AUTOMATED_TEST_PHASE then holder[1] = e
          when e~id == .PhaseReport~FILE_SEARCH_PHASE    then holder[2] = e
          when e~id == .PhaseReport~SUITE_BUILD_PHASE    then holder[3] = e
          when e~id == .PhaseReport~TEST_EXECUTION_PHASE then holder[4] = e
          otherwise nop
        end
        -- End select
      end

      if holder[2] \== .nil then say 'File search:       ' holder[2]~duration
      if holder[3] \== .nil then say 'Suite construction:' holder[3]~duration
      if holder[4] \== .nil then say 'Test execution:    ' holder[4]~duration
      if holder[1] \== .nil then say 'Total time:        ' holder[1]~duration

      if holder~items > 0 then say
    end

  -- End print()

  ::method printSummary
    use arg stats

    verbose = self~getVerbosity

    say
    say "Interpreter:" self~rexxVersion
    say "ooRexxUnit: " self~unitVersion  '09'x || "ooTest:" self~ooTestVersion
    say
    say "Tests ran:"~left(20)  stats~tests
    say "Assertions:"~left(20) stats~asserts

    select
      when verbose < 3 then say "Failures:"~left(20) stats~newFails
      when verbose < 7 then do
        say "Failures:"~left(20) stats~newFails
        say "  (Known failures:)"~left(20) stats~knownFails
      end
      otherwise say "Failures:"~left(20) stats~totalFails
    end
    -- End select

    if verbose < 3 then say "Errors:"~left(20) stats~totalErrs
    else do
      say "Errors:"~left(20)     stats~errs
      say "Exceptions:"~left(20) stats~exceptions
    end

    if vebose < 3 then do
      say
      return
    end

    say "Skipped files:"~left(20) stats~skippedFiles

    if verbose < 4 then do
      say
      return
    end

    say "Messages:"~left(20) stats~messages
    say

  ::method calcStats private
    expose failTable notifications

    tResult = self~testResult
    stats = .directory~new

    stats~tests      = tResult~runCount
    stats~asserts    = tResult~assertCount
    stats~totalFails = failTable['totalCount']
    stats~newFails   = failTable['newCount']
    stats~knownFails = failTable['knownCount']
    stats~errs       = tResult~errorCount
    stats~exceptions = tResult~exceptionCount
    stats~totalErrs  = tResult~errorCount + tResult~exceptionCount

    stats~totalProblems = stats~totalFails + stats~totalErrs

    -- Brute force for now.
    skips = 0; msgs = 0
    do n over notifications
      select
        when n~type == self~SKIP_TYPE then skips += 1
        when n~type == self~TEXT_TYPE then msgs += 1
        otherwise nop -- For now, please fix.
      end
      -- End select
    end
    stats~skippedFiles = skips
    stats~messages = msgs

  return stats

  ::method printExceptions private      -- DFX TODO fix this rough outline
    use arg err

    say "[Framework exception]" err~when
    say "  File:" pathCompact(err~where, 70)
    if err~line <> -1 then say "  Line:" err~line
    say "  Type:" err~type "Severity:" err~severity
    say " " err~getMessage
    if err~conditionObject <> .nil then do
      if err~conditionObject~traceBack~isA(.list) then do line over err~conditionObject~traceBack
        say line
      end
    end
    say

  ::method printMessages private       -- DFX TODO fix this rough outline
    expose notifications

    do n over notifications
      if n~type == self~TEXT_TYPE then self~printMsg(n)
    end

  ::method printMsg private
    use arg n
    say "[Message]" n~when
    say "  File:" pathCompact(n~where, 70)
    say " " n~message
    if n~additional \== .nil then
      say " " n~additional
    if n~additionalObject \== .nil then
      say "  Object involved:" n~additionalObject
    say


  ::method printSkippedFiles private       -- DFX TODO fix this rough outline
    expose notifications

    do s over notifications
      if s~type == self~SKIP_TYPE then self~printSkip(s)
    end

  ::method printSkip private
    use arg s

    say "[Skipped test group]" s~when
    say "  File:" pathCompact(s~where, 70)
    say " " s~reason

    if s~additional \== .nil then do
      -- We use insider knowledge here.  The ooTestFinder starts the additional
      -- text with 'Specified Test Types' and adds the test types object.  We
      -- check for that to provide a better output.
      obj = s~additionalObject

      if obj \== .nil, obj~isA(.set), s~additional~abbrev("Specified Test Types") then
        say "  Specified Test Types:" .ooTestTypes~namesForTests(obj)
      else
        say " " s~additional
    end
    say

-- End of class ConsoleFormatter

/* class: ooTestResult - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\


\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::class 'ooTestResult' public subclass ooTestCollectingParameter

  ::attribute notifications  private
  ::attribute exceptions     private
  ::attribute events         private
  ::attribute knownFailures  private
  ::attribute newFailures    private

  ::attribute doAutoTiming   private
  ::attribute executionPhase private

  ::attribute newFailureCount   set private
  ::attribute newFailureCount   get
  ::attribute knownFailureCount set private
  ::attribute knownFailureCount get

  ::method init
    expose newFailureCount knownFailureCount
    use arg verbosity
    self~init:super

    self~notifications = .queue~new
    self~exceptions = .queue~new
    self~events = .queue~new
    self~knownFailures = .queue~new
    self~newFailures = .queue~new
    newFailureCount = 0
    knownFailureCount = 0

    -- If verbosity is specified, use it to over-ride the default.
    if arg(1, 'E') then self~setVerbosity(verbosity)

    -- Over-ride the default formatter
    self~formatter = .ConsoleFormatter

    parse source . . file
    phase = .PhaseReport~new(file, .PhaseReport~TEST_EXECUTION_PHASE)
    phase~description = "Stand alone execution of a TestGroup."
    self~doAutoTiming = .true
    self~executionPhase = phase

  -- End init( )

  /** noAutoTiming()
   * As a convenience when running a stand alone TestGroup, an ooTest result
   * attempts to time the test execution phase.  It does this by creating a
   * PhaseReport when it is initiated, setting the phase as finished when the
   * print method is invoked, and adding the phase report to the event queue.
   *
   * To disable this feature invoke this method.
   *
   */
  ::method noAutoTiming
    use strict arg
    self~doAutoTiming = .false
    self~executionPhase = .nil

  ::method print

    if self~doAutoTiming, self~executionPhase \== .nil then do
      self~executionPhase~done
      self~addEvent(self~executionPhase)
    end
    forward class (super)

  ::method addFailure
    expose newFailureCount knownFailureCount
    use strict arg testCase, failData
    forward class (super) continue

    if failData~msg~abbrev(.ooRexxUnit.knownBugFlag) then do
      self~knownFailures~queue(failData)
      knownFailureCount += 1
    end
    else do
      self~newFailures~queue(failData)
      newFailureCount += 1
    end

  /** getExtendedFailureInfo()
   * Returns a table with the failure objects sorted into known failures and
   * 'new' (i.e. unknown) failures.  The table has the indexes of: 'knowndQ',
   * 'newQ', 'knownCount', 'newCount', and 'totalCount'
   *
   */
  ::method getExtendedFailureInfo
    expose newFailureCount knownFailureCount
    t = .table~new
    t['newQ']       = self~newFailures~copy
    t['newCount']   = newFailureCount
    t['knownQ']     = self~knownFailures~copy
    t['knownCount'] = knownFailureCount
    t['totalCount'] = newFailureCount + knownFailureCount
    return t

  ::method addNotification
    use strict arg notification
    self~notifications~queue(notification)

  /** getNotifications()
   * Return a copy of the notifications queue so the caller can manipulate it
   * however she wants.  Note that all the queue return methods should do this,
   * just not implemented yet.
   */
  ::method getNotifications
    return self~notifications~copy

  ::method notificationCount
    return self~notifications~items

  ::method addException
    use strict arg exception
    self~exceptions~queue(exception)

  ::method getExceptions
    return self~exceptions

  ::method exceptionCount
    return self~exceptions~items

  ::method addEvent
    use strict arg event
    self~events~queue(event)

  ::method getEvents
    return self~events

  ::method eventCount
    return self~events~items

-- End of class: ooTestResult


/* class: TestGroup- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\

    Test Groups are containers of Tests from a single file.  Upon request they
    can provide a Test Suite consisting of all, or part, of the contained Tests.

    When a Test, in the form of the Test class object is added to a Test Group,
    the Test Group handles some of the rote chores used in configuring the Test.

\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::class "TestGroup" public subclass Object inherit TestContainer

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\
  Methods implementating the TestContainer interface.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  -- True if no tests have been added to this test container, otherwise false.
  ::method isEmpty
    return (self~tests~items == 0)
  -- End isEmpty()

  -- True if this group has any executable tests, otherwise false.
  ::attribute hasTests get
  ::attribute hasTests set private

  /** hasTestTypes()
   *
   * Returns true if this group has any tests matching the types of tests
   * specified, otherwise returns false.
   *
   * @param types REQUIRED
   *   The test type keywords to check for.  This can be a blank delimited
   *   string of words or any .collection object whose items are the words.
   *   Case is not significant.
   *
   */
  ::method hasTestTypes
    use strict arg types

    s = makeSetOfWords(types)
    if s == .nil then
       raise syntax 88.917 array ("1 'types'", "must be a string or a collection of words")

    if \ self~hasTests then return .false

  return s~intersection(self~currentTypes)~items <> 0
  -- End hasTestTypes()

  -- The number of test case classes this group contains.
  ::attribute testCount get
  ::attribute testCount set private

  /** getNoTestsReason
   *
   * A test group can contain tests, but not have any executable tests.  When
   * this is the case, the getNoTestsReason method returns the reason.
   *
   * Returns the reason why this test container has no executable tests, or the
   * empty string if this container does have executable tests.
   */
  ::method getNoTestsReason
    if self~hasTests then return ""
    else return self~noTests_Reason
  -- End getNoTestReason()

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\
  End of TestContainer interface implementation.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

  -- The fully qualified path name of the file this test group represents.
  ::attribute pathName get
  ::attribute pathName set private

  -- If an automated test is being run within the process this test group is
  -- instantiated in.  Note that is dependent on a convention establised when
  -- ooRexxUnit was first designed.  The convention is to set a variable in
  -- the .local directory: bRunTestsLocally.  If this variable is set to false,
  -- than an automated test is being run.  If the variable does not exist, or
  -- is set to true, then an automated test is not being run.
  ::attribute isAutomatedTest get
  ::attribute isAutomatedTest set private

  -- A directory object that is used to set the TestCaseInfo class attribute of
  -- each Test class this test group contains.
  ::attribute testInfo get
    expose testInfo
    return testInfo
  ::attribute testInfo set private

  -- A table of Test classes indexed by their category.
  ::attribute tests private

  -- A table of Test classes with their accompanying Suite indexed by their
  -- category.
  ::attribute testsWithSuite private

  -- A table of Test classes, with an accompanying set of individual methods for
  -- that Test class, indexed by their category.  There is a Suite associated
  -- with each Test class, by default ooTestSuite.
  ::attribute testCollections private

  -- A set of the categories of tests this group currently contains.
  ::attribute currentTypes get
  ::attribute currentTypes set private

  -- The operating system this group is executing on.
  ::attribute machineOS private

  -- A set containing all the possible OSes that ooRexx will run on.  This set
  -- is intended to be immutable.  If / when ooRexx is compilable on additional
  -- operating systems, the set will need to be updated.
  ::attribute knownOSes get
    expose knownOSes
    return knownOSes~copy
  ::attribute knownOSes set private

  -- A set containing the OSes the tests in this group can execute on.  By
  -- default the set contains all known OSes.
  ::attribute allowedOSes get
  ::attribute allowedOSes set private

  -- A string containing an explanation as to why this group has no executable
  -- tests.
  ::attribute noTests_Reason private

  -- Private state variable used to mark that this group has no executable tests
  -- and that should not change.
  ::attribute mustNotExecute private

  ::method readMetadata private
    use arg fsObj

    signal on notready

    lines = .array~new
    do i = 1 until lines[i] = "*/"
      lines[i] = fsObj~linein
    end
    fsObj~close
    return lines

    -- Hit the end of file without finding the marker line.  We will return .nil
    -- causing a syntax error to be raised.
    notready:
    fsObj~close
    return .nil

  /** init()
   *
   * Initializes a new test group instance.
   *
   * @param fileSpec  REQUIRED
   *   The path name of the file this test group represents.  The file must
   *   exist.  Relative path names are acceptable, if they will resolve from the
   *   current working directory.  This is UNLIKELY to be the case in an
   *   automated test run, so the fully qualified path name is usually needed.
   */
  ::method init
    use strict arg fileSpec

    fObj = .stream~new(fileSpec)
    self~pathName = fObj~query("EXISTS")
    if self~pathName == "" then
      raise syntax 88.917 array ("1 'fileSpec'", "must be an existing file path name")

    if fObj~open("SHAREREAD") \== "READY:" then
      raise syntax 88.917 array ("1 'fileSpec'", "must be a readable file")

    data = self~readMetadata(fObj)
    if data == .nil then
      raise syntax 88.917 array ("1 'fileSpec'", "TestGroup metadata format invalid. File:" fileSpec)

    self~tests = .table~new
    self~testsWithSuite = .table~new
    self~testCollections = .table~new
    self~hasTests = .false
    self~testCount = 0
    self~mustNotExecute = .false
    self~noTests_Reason = "No tests have been added to this container."
    self~currentTypes = .set~new

    -- All possible OS words are put into the allowed OSes set, although it is
    -- doubtful that ooRexx is compiled on the last 3.
    self~knownOSes = .set~of('WINDOWS', 'LINUX', 'SUNOS', 'AIX', 'MACOSX', 'CYGNUS', 'FREEBSD', 'NETBSD')
    self~allowedOSes = self~knownOSes~copy
    self~machineOS = .ooRexxUnit.OSName

    -- Determine if an automated test run is taking place.
    if isBoolean(.bRunTestsLocally) then self~isAutomatedTest = (\ .bRunTestsLocally)
    else self~isAutomatedTest = .false

    -- Create the metadata directory for this group.
    self~createMetaData(data)

  -- End init( )

  /** hasTestType()
   *
   * Returns true if this group has a test for the type of test specified,
   * otherwise returns false.
   *
   * @param type REQUIRED
   *   The test type keyword to check for.  This must be a string containing a
   *   single word.  Case is not significant.
   *
   */
  ::method hasTestType
    use strict arg type

    if \ type~isA(.string) then
      raise syntax 88.914 array ("1 'type'", "String")

    if type~words \== 1 then
       raise syntax 88.917 array ("1 'type'", "must be a single test type keyword")

  return self~currentTypes~hasIndex(type)
  -- End hasTestType()

  /** restrictOS()
   *
   * Alerts this test group that the tests it contains are operating system
   * specific.  By default tests in a test group are expected to execute on the
   * set of all OSes that ooRexx runs on.  However, test groups can be
   * restricted to only produce test suites for a subset of those OSes.
   *
   * @param acceptable
   *   A set of OS words that this test group should be restricted to.  This can
   *   be either a string of blank separated OS words, or a collection of the OS
   *   words. Case is not significant.  The collection must be a subset of the
   *   known OS words.
   *
   */
  ::method restrictOS
    expose knownOSes
    use strict arg acceptable

    s = makeSetOfWords(acceptable)
    if s == .nil then
       raise syntax 88.917 array ("1 'acceptable'", "must be a string or a collection of words")

    if \ s~subset(knownOSes) then
      raise syntax 88.917 array ("1 'acceptable'", "is not a subset of the known operating systems. Found:" acceptable)

    self~allowedOSes = s

    if \ s~hasIndex(self~machineOS) then do
      self~hasTests = .false
      self~mustNotExecute = .true
      self~noTests_Reason = "Test is a" self~wordSetToString(s) "specific test. Current OS is:" self~machineOS
    end

  -- End restrictOS()

  /** markNoTests()
   *
   * Informs this group that there is some reason why any tests the group
   * contains should not be executed.
   *
   * @param reason REQUIRED  (String)
   *   The reason why the tests should not be executed.
   */
  ::method markNoTests
    use strict arg reason

    if \ reason~isA(.string) then
      raise syntax 88.914 array ("1 'reason'", "String")

    if reason == "" then
      raise syntax 88.917 array  ("1 'reason'", "A reason must be supplied to mark a test group as having no tests")

    self~hasTests = .false
    self~noTests_Reason = reason
    self~mustNotExecute = .true
  -- End markNoTests()

  /** wordSetToString()
   *
   * Takes a set of words and turns it into a blank delimited string of words.
   * Note that this is a private method and no error checking is done.
   *
   * @param wordSet REQUIRED  (Set)
   *   The set to work with.
   */
  ::method wordSetToString private
    use arg wordSet
    s = .mutableBuffer~new
    do w over wordSet
      s~append(w" ")
    end
  return s~string~strip
  -- End wordSetToString()


  /** add()
   *
   * Adds a test class object to this group.
   *
   * @param test   REQUIRED  (subclass of ooTestCase)
   *
   */
  ::method add
    expose tests
    use strict arg test

    if \ isSubClassOf(test, "ooTestCase") then
      raise syntax 88.917 array ("1 'test'", "must be a subclass of the ooTestCase class. Found:" test)

    if tests~hasIndex(test~ooTestType) then
      raise syntax 88.917 array ("1 'test'", "is a test type ("test~ooTestType") already contained by this test group.")

    test~testCaseInfo = self~testInfo
    tests[test~ooTestType] = test
    self~currentTypes~put(test~ooTestType)
    self~testCount += 1

    if \ self~mustNotExecute then self~hasTests = .true
  -- End add()

  /** addWithSuite()
   *
   * Adds a test class object, and the test suite class to use with it, to this
   * group.
   *
   * When this group is asked to return a suite of its executable tests, for
   * the test class specified here, the accompanying test suite class will be
   * used in its construction.
   *
   * @param test   REQUIRED  (subclass of ooTestCase)
   *
   * @param suite  REQUIRED  (subclass of TestSuite)
   *
   */
  ::method addWithSuite
    expose testsWithSuite
    use strict arg test, suite

    if \ isSubClassOf(test, "ooTestCase") then
      raise syntax 88.917 array ("1 'test'", "must be a subclass of the ooTestCase class. Found:" test)

    if \ isSubClassOf(suite, "ooTestSuite") then
      raise syntax 88.917 array ("2 'suite'", "must be a subclass of the ooTestSuite class. Found:" suite)

    if testsWithSuite~hasIndex(test~ooTestType) then
      raise syntax 88.917 array ("1 'test'", "is a test type ("test~ooTestType") already contained by this test group.")

    test~testCaseInfo = self~testInfo
    testsWithSuite[test~ooTestType] = .TestWithSuite~new(test, suite)
    self~currentTypes~put(test~ooTestType)
    self~testCount += 1

    if \ self~mustNotExecute then self~hasTests = .true
  -- End addWithSuite()

  /** addWithCollection()
   *
   * Adds a test class object and a collection of the individual test method
   * names to this group.  Optionally a test suite class can be specified to use
   * in the construction of a suite of these tests.
   *
   * When this group is asked to return a suite of its executable tests, a suite
   * will be constructed using the test class and the method names.  If the
   * optional test suite is specified, that suite will be used for the
   * construction, otherwise the default ooTestSuite will be used.
   *
   * @param test   REQUIRED  (subclass of ooTestCase)
   *   The test case class object to be added to this group of test classes.
   *
   * @param names  REQUIRED  (String or Collection)
   *   The names of the individual test case methods to executed with the test
   *   class.  This can be a string of blank delimited method names or any
   *   Collection object whose items are the method names.  If this is an
   *   ordered collection, then the individual test case objects will be added
   *   to the resulting test suite in the same order as they are in the
   *   collection.
   *
   * @param suite  OPTIONAL  (subclass of ooTestSuite)
   *   Specifies the test suite class to use in the construction of a test suite
   *   containing these test cases.  This defaults to the ooTestSuite class.
   *
   */
  ::method addWithCollection
    expose testCollections
    use strict arg test, names, suite = .ooTestSuite

    if \ isSubClassOf(test, "ooTestCase") then
      raise syntax 88.917 array ("1 'test'", "must be a subclass of the ooTestCase class. Found:" test)

    -- Use makeArrayOfWords to preserve the order of the names, if an ordered
    -- collection is passed to us.
    methods = makeArrayOfWords(names)
    if methods == .nil then
       raise syntax 88.917 array ("2 'names'", "must be a string or collection of method names")

    if \ isSubClassOf(suite, "ooTestSuite") then
      raise syntax 88.917 array ("3 'suite'", "if used, must be a subclass of the ooTestSuite class. Found:" suite)

    if testCollections~hasIndex(test~ooTestType) then
      raise syntax 88.917 array ("1 'test'", "is a test type ("test~ooTestType") already contained by this test group.")

    test~testCaseInfo = self~testInfo
    testCollections[test~ooTestType] = .TestWithSuiteAndNames~new(test, methods, suite)
    self~currentTypes~put(test~ooTestType)
    self~testCount += 1

    if \ self~mustNotExecute then self~hasTests = .true
  -- End addWithCollection()


  /** suite()
   *
   * Returns a test suite containing all the executable tests, of any test type,
   * to the caller.
   *
   * @param testSuite  OPTIONAL  (subclass of ooTestSuite)
   *   If a test suite object is passed in, the tests will be added to that test
   *   suite.  Otherwise a new test suite is constructed and returned.
   */
  ::method suite
    expose tests testsWithSuite testCollections
    use arg testSuite = (.ooTestSuite~new)

    if \ isSubClassOf(testSuite~class, "ooTestSuite") then
      raise syntax 88.917 array ("1 'testSuite'", "if used, must be a subclass of the ooTestSuite class. Found:" testSuite)

    if \ self~hasTests then return testSuite

    do tClass over tests~allItems
      suite = .ooTestSuite~new(tClass)
      suite~definedInFile = self~pathName
      testSuite~addTest(suite)
    end

    do obj over testsWithSuite~allItems
      suite = obj~getSuite
      suite~definedInFile = self~pathName
      testSuite~addTest(suite)
    end

    do obj over testCollections~allItems
      suite = obj~getSuite
      suite~definedInFile = self~pathName
      testSuite~addTest(suite)
    end

  return testSuite

  /** suiteForTestTypes()
   *
   * Returns a test suite containing all the executable tests, of the specified
   * test type(s), to the caller.
   *
   * @param types  REQUIRED  (String or Collection)
   *   The test type keyword or keywords whose tests should be returned. This
   *   can be a single keyword, a string of blank delimited keywords, or any
   *   Collection object whose items are the keywords.
   *
   * @param testSuite  OPTIONAL  (subclass of ooTestSuite)
   *   If a test suite object is passed in, the tests will be added to that test
   *   suite.  Otherwise a new test suite is constructed and returned.
   */
  ::method suiteForTestTypes
    expose tests testsWithSuite testCollections
    use strict arg types, testSuite = (.ooTestSuite~new)

    testTypes = makeSetOfWords(types)
    if testTypes == .nil then
       raise syntax 88.917 array ("1 'types'", "must be a string or a collection of words")

    if \ isSubClassOf(testSuite~class, "ooTestSuite") then
      raise syntax 88.917 array ("2 'testSuite'", "if used, must be a subclass of the ooTestSuite class. Found:" testSuite)

    if \ self~hasTests then return testSuite

    testTypes = self~currentTypes~intersection(testTypes)
    if testTypes~items == 0 then return testSuite

    do t over testTypes
      testClass = tests[t]
      if testClass <> .nil then do
        suite = .ooTestSuite~new(testClass)
        suite~definedInFile = self~pathName
        testSuite~addTest(suite)
      end

      obj = testsWithSuite[t]
      if obj <> .nil then do
        suite = obj~getSuite
        suite~definedInFile = self~pathName
        testSuite~addTest(suite)
      end

      obj = testCollections[t]
      if obj <> .nil then do
        suite = obj~getSuite
        suite~definedInFile = self~pathName
        testSuite~addTest(suite)
      end
    end

  return testSuite

  /** createMetaData()
   *
   * Creates a directory object with the metadata from the header of the test
   * group file.  The directory object is used to set the TestInfo class
   * attribute of any Test classes this group contains.
   *
   */
  ::method createMetaData private
    use strict arg src

    data = .directory~new
    data~setentry("test_Case-source", self~pathName);

    keyWord=""
    tOut=xrange("A","Z")||xrange("a","z")
    tIn =xrange("A","Z")||xrange("a","z")||xrange()

    do line over src
      line = line~strip
      ch = line~left(2)
      if ch == "--" | ch == "/*" | ch == "*/" then iterate

      -- If not a keyword line, add the line if needed and iterate.
      if line~pos(":") == 0 then do
        if keyWord \== "" then data~entry(keyWord)~queue(line)
        iterate
      end

      -- It is a keyword line, process it.
      parse var line name ":" rest
      keyword = name~translate(tOut, tIn)~space(0)

      -- If the key word has changed then add a new entry.
      if \ data~hasEntry(keyword) then
        data~setentry(keyword, .queue~new)

      data~entry(keyword)~queue(rest~strip)
    end

    self~cleanUpMetaData(data)
    self~testInfo = data
  -- End createMetaData()

  /** cleanUpMetaData()
   *
   * Cleans up the the metadata directory object.  If an entry is a queue, blank
   * lines at the end of the queue are stripped out.  If the queue has less than
   * 2 items, the queue is eliminated and the directory item is replaced by a
   * string.
   *
   */
  ::method cleanUpMetaData private
    use arg data

    keywords = data~allIndexes
    do keyword over keywords
      if data~entry(keyword)~isA(.queue) then do
        -- First strip out any items at the end of the queue that are blank
        -- lines.
        q = data~entry(keyword)
        i = q~last
        do while i <> .nil
          if q~at(i) \== "" then leave
          q~remove(i)
          i = q~last
        end

        -- Now, if the queue has only 1 line left in it (or none,) set the data
        -- item to a string consisting of just this line.  (Discard the queue.)
        if q~items < 2 then do
          item = q~pull
          if item == .nil then item = ""
          data~setEntry(keyword, item)
        end
      end
    end

  -- End cleanUpMetaData()

-- End of class: TestGroup


/* class: TestWithSuite- - - - - - - - - - - - - - - - - - - - - - - - - - - -*\

  Simple helper class to store a test case class and a test suite class.

\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::class 'TestWithSuite' public
::attribute testClass
::attribute suiteClass

::method init
  expose testClass suiteClass
  use arg testClass, suiteClass

::method getSuite
  expose testClass suiteClass
return suiteClass~new(testClass)
-- End of class: TestWithSuite


/* class: TestWithSuiteAndNames- - - - - - - - - - - - - - - - - - - - - - - -*\

  Simple helper class to store a test case class, a collection of test case
  names, and a test suite class.

\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::class 'TestWithSuiteAndNames' public
::attribute testClass private
::attribute names private
::attribute suiteClass private

::method init
  expose testClass names suiteClass
  use arg testClass, names, suiteClass

::method getSuite
  expose testClass names suiteClass

  if suiteClass == .nil then suiteClass = .TestSuite
  suite = suiteClass~new
  do methodName over names
    suite~addTest(testClass~new(methodName))
  end

return suite
-- End of class: TestWithSuiteAndNames


/* class: ooTestFinder - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\

    ooTest Finders search a directory tree for test containers with the desired
    type of tests.

\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::class 'ooTestFinder' public

  ::attribute testTypes private
  ::attribute root private
  ::attribute extension private
  ::attribute simpleFileSpec private
  ::attribute isSimpleSearch private

  ::attribute fileIncludes private
  ::attribute fileExcludes private

  ::attribute totalFound get
  ::attribute totalFound set private

  /** init()
   * Initializes this test finder.
   *
   * @param  root         REQUIRED
   *   The root of the directory tree to search for test containers.
   * @param  extension  REQUIRED
   *   The extension for test container files
   * @param  types      OPTIONAL
   *   The test types to search for.  A value of nil indicates all tests and is
   *   the default.
   */
  ::method init
    expose root extension simpleFileSpec sl
    use strict arg root, extension, types = .nil

    sl = .ooRexxUnit.directory.separator
    if root~right(1) \== sl then root = root || sl
    if extension~left(1) \== '.' then extension = '.' || extension

    simpleFileSpec = root || "*" || extension

    self~testTypes = types
    self~totalFound = 0
    self~fileIncludes = .nil
    self~fileExcludes = .nil
    self~isSimpleSearch = .true

  -- End init()

  /** includeFiles()
   * Add the named file or files to the include files array.  The files are
   * stored as regular expressions with the following conventions:
   *
   * If the name ends in the extension specified in init(), and no directory
   * slashes are in the name, then it will be considered the complete file name.
   * The regular expression will be: any series of characters, the directory
   * slash the specified name.
   *
   * If there are no slashes and the name does not end in the extension the the
   * name will be considered a segment of a file name.  The regular expression
   * will be any series of characters, the slash, any series of characters not a
   * slash, the name, any series of characters not a slash, the extension.
   *
   * The name ends in the slash, it will be considered a directory specification
   * and all files in the directory will be matched.  The regular expression
   * will be any series of characters, the name, any series of characters not
   * the slash, and the extension.
   *
   * Othewise, if it does contain a slash, the reqular expression will be any
   * series of characters, and the name.
   */
  ::method includeFiles
    expose fileIncludes
    use strict arg files

    if \ files~isA(.string), \ files~isA(.collection) then
      raise syntax 88.916 array ("1 'files'", "a string or a collection" files)

    if fileIncludes == .nil then fileIncludes = .array~new
    if files~isA(.string) then do
      regularExpression = self~buildRegEx(files)
      fileIncludes~append(regularExpression)
    end
    else do file over files
      if \ file~isA(.string) then
        raise syntax 88.900 array("Only file names (string objects) are accepted; found" file)

      regularExpression = self~buildRegEx(file)
      fileIncludes~append(regularExpression)
    end
    self~isSimpleSearch = .false

  ::method buildRegEx private
    expose extension sl
    use strict arg fileName

    endsInSlash = (fileName~right(1) == sl)
    hasExt = (fileName~right(extension~length)~upper == extension~upper)
    hasSlash = (fileName~pos(sl) <> 0 )

    notSlash = '[^' || sl || ']*'
    select
      when endsInSlash then do
        reg = '?*' || fileName~upper || notSlash || '(' || extension~upper || ')'
        reg = self~maybeEscapeSlashes(reg)
        return .RegularExpression~new(reg)
      end

      when hasExt, \ hasSlash then do
        reg = '?*' || sl || fileName~upper
        reg = self~maybeEscapeSlashes(reg)
        return .RegularExpression~new(reg)
      end

      when \ hasExt, \ hasSlash then do
        reg = '?+' || sl || notSlash || '(' || fileName~upper || ')' || notSlash || '(' || extension~upper || ')'
        reg = self~maybeEscapeSlashes(reg)
        return .RegularExpression~new(reg)
      end

      when hasExt, hasSlash then do
        reg = '?*' || fileName~upper
        reg = self~maybeEscapeSlashes(reg)
        return .RegularExpression~new(reg)
      end

      otherwise do
        -- \ hasExt, hasSlash
        p = fileName~lastPos(sl)
        parse var fileName lead =(p + 1) segment
        reg = '?*' || lead~upper || notSlash || '(' || segment~upper || ')' || notSlash || '(' || extension~upper || ')'
        reg = self~maybeEscapeSlashes(reg)
        return .RegularExpression~new(reg)
      end

    end
    -- End select

  ::method maybeEscapeSlashes private
    use strict arg exp

    if .ooRexxUnit.OSName \== "WINDOWS"then return exp

    escaped = ""
    do while exp~pos('\') <> 0
      parse var exp seg'\'exp
      escaped = escaped || seg || '\\'
    end
    escaped = escaped || exp

    return escaped


  ::method seek
    expose testTypes simpleFileSpec
    use strict arg testResult

    if \ isSubClassOf(testResult~class, "ooTestCollectingParameter") then
      raise syntax 88.917 array ("1 'testResult'", "must be a subclass of the ooTestCollectingParameter class. Found:" testResult)

    q = .queue~new
    files = self~findFiles

    if files~items == 0 then do
      err = .ExceptionData~new(timeStamp(), simpleFileSpec, "Anomly")
      err~severity = "Warning"
      err~msg = "No test containers found matching search paramters."
      testResult~addException(err)
      return q
    end


    do fileName over files
      container = self~getContainer(fileName)

      select
        when isSubClassOf(container~class, "ExceptionData") then do
          testResult~addException(container)
          iterate
        end

        when \ isSubClassOf(container~class, "TestContainer") then do
          obj = self~maybeCreateContainer(container, fileName)

          select
            when obj~isA(.Notification) then do
              testResult~addNotification(obj)
              iterate
            end

            when isSubclassOf(obj~class, "ErrorReport") then do
              testReport~addError(obj)
              iterate
            end

            otherwise do
              -- Add a notification that this was an old style test group, so we
              -- can update them.
              n = .Notification~new(timeStamp(), fileName, .Notification~TEXT_TYPE )
              n~message = "Converted old-style TestUnit list into a test group."
              n~additionalObject = obj
              testResult~addNotification(n)

              q~queue(obj)
            end
          end
          -- End select
        end

        when \ container~hasTests then do
          n = .Notification~new(timeStamp(), fileName, .Notification~SKIP_TYPE)
          n~reason = "Test container has no executable tests"
          n~additional = container~getNoTestsReason
          testResult~addNotification(n)
          iterate
        end

        -- If testTypes are .nil then the caller wants any executable tests from
        -- the container.  We know the container has tests.
        when testTypes == .nil then q~queue(container)

        -- Caller wants a certain type of tests.
        when \ container~hasTestTypes(testTypes) then do
          n = .Notification~new(timeStamp(), fileName, .Notification~SKIP_TYPE)
          n~reason = "Container has no executable tests of specified test types"
          n~additional = "Specified Test Types:" testTypes
          n~additionalObject = testTypes
          testResult~addNotification(n)
          iterate
        end

        otherwise q~queue(container)
      end
      -- End select
    end
    -- End do fileName over files

  return q
  -- End seek()

  ::method getContainer private
    use arg file

    signal on any name callError
    call (file)
    container = RESULT
    return container

    callError:
      err = .ExceptionData~new(timeStamp(), file, "Trap")
      err~conditionObject = condition('O')
      err~msg = "Initial call of Test Group failed"

  return err
  -- End getContainer()

  /** findFiles()
   *
   * An enhancement is to match includes and excludes.
   */
  ::method findFiles private
    expose simpleFileSpec

    f = .array~new
    j = SysFileTree(simpleFileSpec, files., "FOS")
    self~totalFound = files.0
    do i = 1 to files.0
      if self~matchFile(files.i) then f~append(files.i)
    end

  return f

  ::method matchFile
    expose isSimpleSearch fileIncludes
    use arg file

    if isSimpleSearch then return .true

    do re over fileIncludes
      if re~match(file~upper) then return .true
    end
    return .false


  /** maybeCreateContainer()
   * Attempts to create a TestGroup from object.  This is a temporary method,
   * used to ease the migration from ooRexxUnit 1 to ooRexxUnit 2.0.0.
   *
   */
  ::method maybeCreateContainer private
    use arg obj, fileName

    -- Try to determine if this is an old-style .testUnit list.  If it is, it
    -- can be converted to a test group.  If it isn't add the file to the
    -- omitted files list.

    data = .Notification~new(timeStamp(), fileName, .Notification~SKIP_TYPE)
    data~reason = "Attempt to convert returned object into a TestGroup failed"
    data~additional = ""
    if \ self~objectIsTestUnitList(obj, data) then return data

    -- Okay, an old-style TestUnit list.  Create a TestGroup and populate it
    -- with the TestCase class(es) from the file.  It is possible, but very
    -- unlikely that .TestGroup~new will throw an exception.  So, catch it.
    signal on any name tempHandler
    group = .TestGroup~new(fileName)

    -- objectIsTestUnitList() has already done our error checking for us.
    do a over obj
      group~add(a[1])
      if a[2]~isEmpty then iterate
      group~addWithCollection(a[1], a[2])
    end

    return group

  tempHandler:
    err = .ExceptionData~new(timeStamp(), fileName, "Trap")
    err~conditionObject = condition('O')
    err~msg = "Creating TestGroup from old-style TestUnit list file failed."
  return err

  ::method objectIsTestUnitList private
    use arg obj, data

    if obj~isA(.list) then do a over obj
      if \ a~isA(.array) then
        return self~updateData(data, "Item in TestUnit list is not an array", obj)

      if a~items \== 2 then
        return self~updateData(data, "Array item in TestUnit list does not have 2 indexes", obj)

      if \ isSubclassOf(a[1], "TestCase") then
        return self~updateData(data, "Index 1 of array item is not subclass of TestCase", obj)

      if \ a[2]~isA(.list) then
        return self~updateData(data, "Index 2 of array item is not a .List", obj)

      return .true
    end

  return self~updateData(data, "Object is not a list, object is:" obj)

  ::method updateData private
    use arg data, msg, obj
    data~additional = msg
    data~additionalObject = obj
  return .false

-- End of class: ooTestFinder


/* class: ExceptionData- - - - - - - - - - - - - - - - - - - - - - - - - - - -*\

    A data object containing information concerning an unrecoverable error that
    occurred during the execution of an automated group of tests.

    Errors that occur during the invocation a test case method are trapped by
    the exception handle.  However, it is also possible for errors to occur
    during other phases of an an automated test.  For example, errors that
    happen during some set up prior to actually invoking a test case method.

\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::class 'ExceptionData' public subclass TestProblem

  ::attribute severity
  ::attribute msg

  ::method init
    forward class (super) continue

    self~severity = "Fatal"
    self~msg = ""

  ::method getMessage

    if self~type == "Trap", self~conditionObject \== .nil then
      msg = self~conditionObject~message
    else if self~msg \== "" then
      msg = self~msg
    else
      msg = "n/a"

    return msg

-- End of class: ExceptionData


::class 'NotificationTypes' mixinclass Object
  /* Would prefer to use the CONSTANT directive */
  ::method MIN_TYPE  class; return 1
  ::method MIN_TYPE;        return 1

  ::method SKIP_TYPE  class; return 1
  ::method WARN_TYPE  class; return 2
  ::method TEXT_TYPE  class; return 3
  ::method STEP_TYPE  class; return 4
  ::method STATS_TYPE class; return 5
  ::method SKIP_TYPE;        return 1
  ::method WARN_TYPE;        return 2
  ::method TEXT_TYPE;        return 3
  ::method STEP_TYPE;        return 4
  ::method STATS_TYPE;       return 5

  ::method MAX_TYPE  class; return 5
  ::method MAX_TYPE;        return 5


/* class: Notification - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\

    A data object containing information concerning status, events, or other
    things that might need to be logged during the execution of a test, usually
    the execution of an automated suite of tests.

    At a minimum the object contains a time stamp, the name of the relevant
    file, and the notification type.

\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::class 'Notification' public subclass ReportData inherit NotificationTypes

  ::attribute reason
  ::attribute message
  ::attribute warning

  ::attribute additional
  ::attribute additionalObject
  ::attribute originatorsID

  ::method init
    use strict arg dateTime, file, type
    forward class (super) continue

    if \ isWholeRange(type, self~MIN_TYPE, self~MAX_TYPE) then
      raise syntax 88.907 array("3 'type'", self~MIN_TYPE, self~MAX_TYPE, type)

    select
      when type == self~SKIP_TYPE then do
        self~reason = "Reason is unknown"
        self~warning = .nil
        self~message = .nil
      end
      when type == self~WARN_TYPE then do
        self~reason = .nil
        self~warning = "Warning is unknown"
        self~message = .nil
      end
      otherwise do
        self~reason = .nil
        self~warning = .nil
        self~message = "Message is unknown"
      end
    end
    -- End select

    self~additional = .nil
    self~additionalObject = .nil
    self~originatorsID = .nil

-- End of class: Notification

/* class: PhaseReport- - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\

  A type of a notification containging data concerning a phase of the overall
  execution of a group of tests.  By definition, a phase covers some period of
  time and phase reports contain a begin and end time and can produce the
  duration of the phase.

\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::class 'PhaseReport' public subclass Notification

  ::method MIN_PHASE  class; return 1
  ::method MIN_PHASE;        return 1

  ::method AUTOMATED_TEST_PHASE class; return 1
  ::method FILE_SEARCH_PHASE    class; return 2
  ::method SUITE_BUILD_PHASE    class; return 3
  ::method TEST_EXECUTION_PHASE class; return 4
  ::method AUTOMATED_TEST_PHASE;       return 1
  ::method FILE_SEARCH_PHASE;          return 2
  ::method SUITE_BUILD_PHASE;          return 3
  ::method TEST_EXECUTION_PHASE;       return 4

  ::method MAX_PHASE  class; return 4
  ::method MAX_PHASE;        return 4

  ::attribute begin get
  ::attribute begin set private

  ::attribute finish get
  ::attribute finish set private

  ::attribute isFinished get
  ::attribute isFinished set private

  ::attribute id get
  ::attribute id set private

  ::attribute isTicking private
  ::attribute endTicking private

  ::method init
    use strict arg file, id

   	self~begin = .TimeSpan~new(time('F'))
    if \ isWholeRange(id, self~MIN_PHASE, self~MAX_PHASE) then
      raise syntax 88.907 array("2 'id'", self~MIN_PHASE, self~MAX_PHASE, type)

    self~init:super(timeStamp(), file, self~STEP_TYPE)

    self~id = id
    self~finish = .nil
    self~isFinished = .false
    self~isTicking = .false
    self~endTicking = .true

  /** tickTock()
   * Outputs dots to the screen in a separate thread.
   */
  ::method tickTock unguarded
    expose isTicking
    use arg msg
    reply

    .stdout~charout(msg)
    isTicking = .true
    self~endTicking = .false
    dots = msg~length

    do while \ self~endTicking
      do i = 1 to 2
        if self~endTicking then leave
        j = SysSleep(.5)
      end
      if dots == 75 then do
        .stdout~lineout(".")
        dots = 0
      end
      else do
        .stdout~charout(".")
      end
      dots += 1
    end
    .stdout~lineout(".")
    isTicking = .false

  /** stopTicking()
   * Provides a way to turn off the tick tock before the duration of this phase
   * is over.
   */
  ::method stopTicking unguarded
    expose isTicking
    self~endTicking = .true
    guard on when \ isTicking

  /** done()
   * Tells this phase that the phase is over.  Sets the finish time.  After this
   * message, the phase duration will always be the same.
   */
  ::method done
    use strict arg
    self~finish = .TimeSpan~new(time('F'))
    self~isFinished = .true

    if self~isTicking then self~stopTicking

  /** duration()
   * The time spanned by this phase.  If the phase is not done, is not finished,
   * then this will be the time elapsed up to this point.  When the phase is
   * done, it will be the total time elapsed for the phase.  Once the phase is
   * finished, duration will not change.
   */
  ::method duration
    if \ self~isFinished then return (self~finish - .TimeSpan~new(time('F')))
    else return (self~finish - self~begin)

  ::method 'description='
    use strict arg description
    self~message = description

  ::method 'description'
    return self~message

-- End of class: PhaseReport


/** printTestInfo()
 *
 * Debugging method to print out the information in the TestInfo class attribute
 * of a TestCase class.  Curious as to how useful this information is.
 *
 */
::routine printTestInfo public
  use arg metaData

  itr = metaData~supplier
  do while itr~available
    keyword = itr~index || ":"
    data = itr~item

    if \ data~isA(.queue) then do
      say keyword~left(18) data
    end
    else do
      say
      say keyword
      do line over data
        say line
      end
      say
    end
    itr~next
  end
return 0
-- End printTestInfo()
