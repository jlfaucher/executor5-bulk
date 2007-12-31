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
   version:          0.1.0

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
  .local~ooTest_Framework_version=010.320.20071208

  -- Replace the default test result class in the environment with the ooRexx
  -- project's default class.
  .local~ooRexxUnit.default.TestResult.Class = .ooTestResult

  -- Capture the ooTest framework directory and ensure it is in the path.
  parse source . . fileSpec
  .local~ooTest.dir = fileSpec~left(fileSpec~caseLessPos("ooTest.frm") - 2 )
  currentPath = value("PATH", , 'ENVIRONMENT')
  if currentPath~caseLessPos(.ooTest.dir) == 0 then
     oldPath = value("PATH", .ooTest.dir || .ooRexxUnit.path.separator || currentPath, 'ENVIRONMENT')

  do rtn over .methods
     .public_routines~put(.methods[rtn], rtn)
  end
end
-- End of entry point.

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\
  Directives, Classes, or Routines.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::requires "OOREXXUNIT.CLS"

::method makeSetOfWords
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

::method makeArrayOfWords
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
-- End makeSetOfWords()


/** class:  TestContainer
 * Defines an interface for a test container.  Objects containing tests that
 * implement the TestContainer interface can be 'found' by the ooTestFinder
 * class.
 */
::class 'TestContainer' public

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
 * the container has no tests.
 */
::method getNoTestsReason abstract


/** class:  ooTestCollectingParameter
  *   Defines an interface extended from TestResult for a collecting parameter
  *   used specifically for testing the ooRexx intepreter and its distribution
  *   packages.
  * DFX TODO finish up doc here.
  */
::class 'ooTestCollectingParameter' public subclass TestResult
::method addOmission    abstract
::method getOmissions   abstract
::method omissionCount  abstract
::method addException   abstract
::method getExceptions  abstract
::method exceptionCount abstract
::method addEvent       abstract
::method getEvents      abstract


/* class: ooTestCase - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\

    ooTestCases are used to test the ooRexx interpreter package.  An ooTestCase
    class is a test class where methods of the class define individual test
    cases.  In order to make it easy to construct automated tests with large
    numbers of test cases, a convention is followed:

    Each method of an ooTestCase class that starts with 'test' is considered
    an individual test case.

    Each ooTestCase has a class attribute defining the test type of test cases
    of the individual test cases the class contains.  (This list is still being
    defined)

    UNIT SAMPLE GUI_SAMPLE DOC_EXAMPLE STRESS

\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::class 'ooTestCase' public subclass TestCase

  -- ooTestType is the type of test cases contained in this test case class.
  -- The default type is "UNIT" and other typical types are "STRESS", "SAMPLE",
  -- "GUI_SAMPLE", "DOC_EXAMPLE", etc..
  ::attribute ooTestType get class
  ::attribute ooTestType set class private

  ::method init class
    forward class (super) continue

    -- Use the ooTestResult as the default test result.
    self~defaultTestResultClass = .ooTestResult
    self~ooTestType = "UNIT"

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

  /** run()
   * Executes the tests in this test suite.  Over-rides the superclass method.
   *
   * @param testResult    OPTIONAL    (ooTestCollectingParameter)
   *   The collecting parameter to use for the execution of the tests.  The
   *   default
   */
  ::method run
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
       test~run(testResult, verbose)
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

\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::class 'ConsoleFormatter' public subclass ResultFormatter

  /** init()
   *
   */
  ::method init
    use strict arg testResult
    forward class (super) continue

    -- We need an ooTestCollectingParameter.
    if \ isSubClassOf(testResult~class, "ooTestCollectingParameter") then
       raise syntax 88.914 array ("1 'testResult'", "ooTestCollectingParameter")
    self~testResult = testResult

  /** print()
   *
   * Prints the data collected by this test result in a "console-friendly"
   * manner.
   *
   * @param title    OPTIONAL    (String)
   *   Adds a title to the output.
   *
   * @param level   OPTIONAL    (Whole Number)
   *   Sets the verbosity level of the print out.  Values are 0 through 10, with
   *   lesser numbers meaning lesser verbosity.  The default level is 1.
   */
  ::method print
    use arg title = "", level = 0

    if \ title~isA(.string) then
      raise syntax 88.914 array ("1 'title'", "String")

    if arg(1, 'E') then self~setVerbosity(level)
    data = self~testResult

    if title<>"" then do
      say title
      say
    end

    versionStr = .ooRexxUnit.interpreterName .ooRexxUnit.languageLevel .ooRexxUnit.interpreterDate
    say "Interpreter:" versionStr
    say "ooRexxUnit: " .ooRexxUnit.version
    say
    say "Count of tests ran:            " data~runCount
    say "Count of successful assertions:" data~assertCount
    say "Count of failures:             " data~failureCount
    say "Count of errors:               " data~errorCount
    say "Count of exceptions:           " data~exceptionCount
    say "Count of skipped files:        " data~omissionCount
    say

    if data~failureCount > 0 then do co over data~failures                 -- DFX TODO fix this rough outline
      self~printFailureInfo(co~ooRexxUnit.condition)
    end

    if data~errorCount > 0 then do co over data~errors
      self~printErrorInfo(co~ooRexxUnit.condition)
    end
    say 'verbosity:' self~getVerbosity
    if self~getVerbosity > 2 then do
      if data~exceptionCount > 0 then self~printExceptions
      if data~omissionCount > 0 then self~printOmissions
    end

    -- If a number of failure or error information lines are printed, re-display
    -- the summary statistics again so that the number of failures is obvious to
    -- the user.
    if (data~failureCount + data~errorCount) > 3 | self~getVerbosity > 2 then do
      say "Interpreter:" versionStr
      say "ooRexxUnit: " .ooRexxUnit.version
      say
      say "Count of tests ran:            " data~runCount
      say "Count of successful assertions:" data~assertCount
      say "Count of failures:             " data~failureCount
      say "Count of errors:               " data~errorCount
      say "Count of exceptions:           " data~exceptionCount
      say "Count of skipped files:        " data~omissionCount
      say
    end

  -- End print()

  /** printFailureInfo()
   *
   * Helper method for print()
   */
  ::method printFailureInfo private
    use arg co

    parse var co dateTime "[failure]" idStuff "--->" additionalStuff
    dateTime = dateTime~strip~strip('T', ':')

    idTable = self~decomposeTestCaseID(idStuff)
    additional = self~decomposeAdditional(additionalStuff)

    say "[failure]" dateTime
    say "  Test:  "  idTable['name']
    say "  Class: "  idTable['class']
    say "  File:  "  idTable['file']
    if additional~items > 0 then self~printAssertInfo(additional)
    say

  return 0

  /** printErrorInfo()
   *
   * Helper method for print()
   */
  ::method printErrorInfo private
    use arg co

    parse var co dateTime "[error]" idStuff "--->" additionalStuff
    dateTime = dateTime~strip~strip('T', ':')

    idTable = self~decomposeTestCaseID(idStuff)
    additional = self~decomposeAdditional(additionalStuff)

    say "[error]" dateTime
    say "  Test: "  idTable['name']
    say "  Class:"  idTable['class']
    say "  File: "  idTable['file']

    if additional~items > 0 then do
      parse value additional[1] with c info
      say "  Event:" info
      if additional~items > 1 then do i = 2 to additional~items
        say "   " additional[i]
      end
    end
    say

  -- End printErrorInfo()

  /** decomposeTestCaseID()
   *
   * Helper method for print()
   */
  ::method decomposeTestCaseID private
    use arg id

    id = id~strip
    parse var id tcWord tcName lastPart
    tcName = tcName~substr(2, tcName~length - 2)
    parse var lastPart cls "@" file ")"
    clsName = cls~word(2)

    if file~length > 70 then file = pathCompact(file, 70)
    t = .table~new
    t['name']  = tcName
    t['class'] = clsName
    t['file']  = file

  return t
  -- End decomposeTestCaseInfo()

  /** decomposeAdditional()
   *
   * Helper method for print()
   */
  ::method decomposeAdditional private
    use arg extraStuff

    extraStuff = extraStuff~strip
    eol = .ooRexxUnit.line.separator
    tab = '09'x
    a = .array~new

    do while extraStuff~pos(eol) <> 0
      parse var extraStuff lead eol extraStuff
      do while lead~pos(tab) <> 0
        parse var lead line (tab) lead
        a[a~items + 1] = line
      end
      a[a~items + 1] = lead
    end

    do while extraStuff~pos(tab) <> 0
      parse var extraStuff line (tab) extraStuff
      a[a~items + 1] = line
    end
    a[a~items + 1] = extraStuff

  return a
  -- End decomposeAdditional()

  /** printAssertInfo()
   *
   * Helper method for print()
   */
  ::method printAssertInfo private
    use arg info

    line = info[1]
    if line~abbrev("@assert") then do
      if line~word(2)~abbrev('check4Condition') then do
        parse var line . '['expected']' .
        assert = "Error should be raised"
        actual = "Not raised"
      end
      else do
        parse var line atWord assert "expected=" expected ", actual=" actual
        assert = assert~strip~strip('T', ':')
        actual = actual~strip('T', '.')
      end
      say "  Failed:" assert
      say "    Expected:" expected
      say "    Actual:  " actual
    end
    else do
      say "  General failure:"
      say "   " line
    end

    if info~items > 1 then do i = 2 to info~items
      say "   " info[i]
    end

  -- End printAssertInfo()

  ::method printExceptions private      -- DFX TODO fix this rough outline

    q = self~testResult~getExceptions
    do e over q
      say "[Framework exception]" e~when
      say "  File:" pathCompact(e~where, 70)
      if e~line <> -1 then say "  Line:" e~line
      say "  Type:" e~type "Severity:" e~severity
      say " " e~getMessage
      say
    end

  ::method printOmissions private       -- DFX TODO fix this rough outline

    q = self~testResult~getOmissions
    do o over q
      say "[Skipped test group]"
      say "  File:" pathCompact(o~where, 70)
      say " " o~reason
      if o~additional \== .nil then
        say " " o~additional
      say
    end

-- End of class ConsoleFormatter

/* class: ooTestResult - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\


\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::class 'ooTestResult' public subclass ooTestCollectingParameter

  ::attribute omissions  private
  ::attribute exceptions private
  ::attribute events     private

  ::method init
    use arg verbosity = 1
    forward class (super) continue

    self~omissions = .queue~new
    self~exceptions = .queue~new
    self~events = .queue~new

    -- If verbosity is specified, use it to over-ride the default.
    if arg(1, 'E') then self~setVerbosity(verbosity)

    -- Over-ride the default formatter
    self~formatter = .ConsoleFormatter

  -- End init( )

  ::method addOmission
    use strict arg omission
    self~omissions~queue(omission)

  ::method getOmissions
    return self~omissions

  ::method omissionCount
    return self~omissions~items

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

-- End of class: ooTestResult


/* class: TestGroup- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\

    Test Groups are containers of Tests from a single file.  Upon request they
    can provide a Test Suite consisting of all, or part, of the contained Tests.

    When a Test, in the form of the Test class object is added to a Test Group,
    the Test Group handles some of the rote chores used in configuring the Test.

\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::class "TestGroup" public subclass TestContainer

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


  /** init()
   *
   * Initializes a new test group instance.
   *
   * @param fileSpec  REQUIRED
   *   The path name of the file this test group represents.  The file must
   *   exist.  Relative path names are acceptable, if they will resolve from the
   *   current working directory.  This is UNLIKELY to be the case in an
   *   automated test run, so the fully qualified path name is usually needed.
   *
   * @param data      OPTIONAL
   *   An array of lines containing the metadata from the file header for this
   *   test group.  This is only useful if the file header is in the format
   *   specified in the ooTest Framework reference.
   */
  ::method init
    use strict arg fileSpec, data = (.array~new)

    fObj = .stream~new(fileSpec)
    self~pathName = fObj~query("EXISTS")
    if self~pathName == "" then
      raise syntax 88.917 array ("1 'fileSpec'", "must be an existing file path name")

    if \ data~isInstanceOf(.array) then
      raise syntax 88.914 array ("2 'data'", "Array")

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
    if \ .local~hasentry("bRunTestsLocally") then
      self~isAutomatedTest = .false
    else if .bRunTestsLocally~isA(.string), .bRunTestsLocally~dataType('O') then
      self~isAutomatedTest = (\ .bRunTestsLocally)
    else
      self~isAutomatedTest = .false

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
    if testType~items == 0 then return testSuite

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
  ::attribute fileSpec private

  ::method init
    use strict arg root, extension, types = .nil

    sl = .ooRexxUnit.directory.separator
    if root~right(1) == sl then root = root~strip('T', sl)
    if extension~left(1) == '.' then extension = extension~strip('L', '.')
    fileSpec = root || sl || "*." || extension

    self~testTypes = types
    self~fileSpec = fileSpec

  -- End init()

  ::method seek
    expose testTypes fileSpec
    use strict arg testResult

    if \ isSubClassOf(testResult~class, "ooTestCollectingParameter") then
      raise syntax 88.917 array ("1 'testResult'", "must be a subclass of the ooTestCollectingParameter class. Found:" testResult)

    q = .queue~new
    files = self~findFiles

    if files~items == 0 then do
      err = .ExceptionData~new(timeStamp(), fileSpec)
      err~type = "Anomaly"
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
          reason = "Object returned is not a test container.  Object is:" container
          o = .OmissionData~new(timeStamp(), fileName, reason)
          testResult~addOmission(o)
          iterate
        end

        when \ container~hasTests then do
          reason = "Test container has no executable tests"
          o = .OmissionData~new(timeStamp(), fileName, reason)
          o~additional = "Reason:" container~getNoTestsReason
          testResult~addOmission(o)
          iterate
        end

        -- If testTypes are .nil then the caller wants any executable tests from
        -- the container.  We know the container has tests.
        when testTypes == .nil then q~queue(container)

        -- Caller wants a certain type of tests.
        when \ container~hasTestTypes(testTypes) then do
          reason = "Container has no executable tests of specified test types"
          o = .OmissionData~new(timeStamp(), fileName, reason)
          o~additional = "Specified Test Types:" testTypes
          o~additionalObject = testTypes
          testResult~addOmission(o)
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
      conditionData = condition('O')
      err = .ExceptionData~new(timeStamp(), file, conditionData)
      err~msg = "Initial call of Test Group failed"

  return err
  -- End getContainer()

  /** findFiles()
   *
   * An enhancement is to match includes and excludes.
   */
  ::method findFiles private
    expose fileSpec
    say 'in find files filespec:' fileSpec

    f = .array~new
    j = SysFileTree(fileSpec, files., "FOS")
    do i = 1 to files.0
      f~append(files.i)
    end

  return f

-- End of class: ooTestFinder


/* class: OmissionData - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\

    A data object containing information on why a test file was ommitted from
    a group of tests.  At a minimum the object contains a time stamp, the name
    of the file, and a reason as to why it was omitted.

\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::class 'OmissionData' public subclass ProblemReport

  ::attribute reason get
  ::attribute reason set private

  ::attribute additional
  ::attribute additionalObject

  ::method init
    forward class (super) continue

    if arg(3, 'E') then self~reason = arg(3)
    else self~reason = "Unknown"

    self~additional = .nil
    self~additionalObject = .nil

-- End of class: OmissionData

/* class: ExceptionData- - - - - - - - - - - - - - - - - - - - - - - - - - - -*\

    A data object containing information concerning an unrecoverable error that
    occurred during the execution of tests.

    These are errors that are not directly related to the execution of a test.
    For example, errors that happen during some set up prior to actually
    invoking a test case method.

\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::class 'ExceptionData' public subclass ProblemReport
  ::attribute type
  ::attribute severity
  ::attribute line
  ::attribute classObject
  ::attribute msg
  ::attribute conditionObject

  ::method init
    use arg timeStamp, file, cObj = .nil
    forward class (super) continue

    self~type = "Trap"
    self~severity = "Fatal"

    if cObj <> .nil, cObj~position <> .nil then self~line = cObj~position
    else self~line = -1

    self~msg = ""
    self~classObject = .nil
    self~conditionObject = cObj

  ::method getMessage
    expose severity msg
  return severity || .endOfLine || "   " msg

-- End of class: OmissionData

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
