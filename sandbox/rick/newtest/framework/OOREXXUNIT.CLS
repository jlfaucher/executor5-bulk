#!/usr/bin/rexx
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2005-2007 Rexx Language Association. All rights reserved.    */
/*                                                                            */
/* This program and the accompanying materials are made available under       */
/* the terms of the Common Public License v1.0 which accompanies this         */
/* distribution. A copy is also available at the following address:           */
/* http://www.opensource.org/licenses/cpl1.0.php                              */
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


-- End of entry point.

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\
  Directives, Classes, or Routines.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

-- main control class the tests.  Holds a lot of global test values rather than
-- storing them in .local
::class "ooRexxUnit"
-- main setup method for the unit test framework
::method activate class

  -- this is the framework version number
  self~version = 3.0.0
  -- this is the minimum language level required by the framework
  self~mininumInterpreterVersion = 5.0.0

  parse version self~interpreterName self~languageLevel self~interpreterDate

  -- Note:  initialize the OSName first, other methods depend on this
  self~OSName = self~getOSName
  self~shellName = self~getShellName

  -- this is our default class for reporting errors.  Can be replaced
  self~defaultTestResultClass = .TestResult

  -- Define a 'marker' that can be used to flag a known test case failure.
  self~knownBugFlag = "tracker bug #"

  -- Capture the ooRexxUnit framework directory and ensure it is in the path.
  -- Also capture the original path in case someone needs it.
  self~frameworkDirectory = .File~new(.context~package~name)~parent
  -- set up an original search path for files, includeing the framework directory
  self~originalPath = self~addToPath(self~frameWorkDirectory)

  self~architecture = self~getAddressingMode

-- class attributes for global data
::attribute version class
::attribute minimumInterpreterVersion class
::attribute minimumInterpreterVersion class
::attribute shellName class
::attribute OSName class

::attribute defaultTestResultClass class
::attribute knownBugFlag class
::attribute frameworkDirectory class
::attribute originalPath class
::attribute architecture class


-- Determine and return the shell name to be used in ADDRESS keyword statements. This
-- is a central location to make it easy to maintain in the future, in case a non-Windows
-- and non-Unix compatible operating systems comes up, or new shell variants develop.
::method getShellName private class
  expose OSName


   -- make sure we address the shell
   shell="CMD"

   -- for now, we have a Windows/not Windows situation.
   if \OSName~caselessEquals("Windows") do
   do
      -- get the fully qualified shell name from the environment
      unixShell=value("SHELL", , "environment")
      // and get the shell name portion
      shell = .File~new(unixShell)~name
   end
   return shell


-- Determine and return the operating system under which the currently executing
-- program is running.  Provides an uniform method for test cases to determine
-- which operating system they are executing on.
--
-- The possible list of return words as of ooRexx 3.2.0 is:
--
--   WINDOWS, LINUX, SUNOS, AIX, MACOSX, CYGNUS, FREEBSD, and NETBSD
--
-- Although it is doubtful that ooRexx is compilable on CYGNUS, FREEBSD, or
-- NETBSD.
--
::method getOSName private class
  parse upper source os .
  if os~abbrev("WIN") then os = "WINDOWS"
  return os


-- Adds the specified directory to the path only if it is not already in the
-- path.  By default the directory is added to the beginning of the path.
--
-- @param  dir  REQUIRED
--   The directory to add to the path
-- @param  at   OPTIONAL
--   Where to add the directory, beginning or end. Only the first letter is
--   needed.  Specifying anything other than "E"nd results in the beginning.
-- @return  Returns the old path, the current path at time of invocation.
--
::class addToPath class
  use strict arg dir, at = 'B'

  at = at~left(1)~upper
  if at \== 'E' then at = 'B'

  curPath = value("PATH", , 'ENVIRONMENT')
  if \self~isInPath(dir) then do
    if at == 'B' then
      newPath = dir || .File~pathSeparator || curPath
    else
      newPath curPath || .File~pathSeparator || dir
  end

  call value "PATH", newPath, "ENVIRONMENT"

  return curPath

-- Returns true if the specified directory is in the current PATH, otherwise
-- returns false.
::method isInPath
  use strict arg dir

  path = value("PATH", ,'ENVIRONMENT')
  sl   = .File~separator
  sep  = .File~pathSeparator

  if \.File~isCaseSensitive then do
    if path~caselessContains(dir || sep) then return .true
    if path~caseLessContains(dir || sl || pathSep) then return .true
    if path~right(dir~length)~caselessEquals(dir) then return .true
  end
  else do
    if path~contains(dir || sep) then return .true
    if path~contains(dir || sl || pathSep) then return .true
    if path~right(dir~length)~equals(dir) then return .true
  end

return .false

-- Determine if this is a 32-bit or 64-bit interpreter.
::method getAddressingMode private class
  parse version ")_" mode "-bit"
  return mode

/* class: TestUtil - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\

    A class with static (class) utility methods that can be used anywhere.

\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::class "TestUtil" public
::method activate class
  expose nonPrintableChars

  -- the nonprintable characters
  nonPrintableChars = xrange("00"x, "1F"x) || "FF"x

/** isWholeRange()
 * Tests if an object is a whole number within the range of min and max.
 *
 * @param number  REQUIRED  The object to test.
 * @param min     REQUIRED  The minimal value of the range.
 * @param max     REQURIED  The maximal value of the range.
 */
::method isWholeRange class
  use strict arg number, min, max

  if number~isA(.String), number~dataType('W'), number >= min, number <= max then return .true
  return .false

-- escape non printable characters in strings
::method escapeString class
  expose nonPrintableChars
  use arg str

  -- quick test to see if there is anything to do
  if str~verify(nonPrintableChars, "Match") = 0 then return str

  buff = .mutableBuffer~new
  sep = ""

  // loop searching for sections of non-printable charactes
  loop while str<>""
     start=str~ (str, nonPrintableChars, "Match")
     -- non-printing char found, look for printable char after it
     if start > 0 then do
        -- find non-matching position, deduct one to point to last non-printable chars in string
        end=str~verify(nonPrintableChars, "Nomatch", start) - 1
        -- no non-matching (=ending) position found: rest is non-printable
        if end = -1 then end = str~length

        -- printable chars before section with non-printable chars ?
        if start>1 then do
           buff~append(sep, '"', str~left(start - 1), '"')
           sep = " || "
        end

        buff~append(sep, '"', str~substr(start, end - start + 1)~c2x, '"x')
        sep = " || "

        -- extract non-processed part of string
        str = str~substr(end + 1)
     end
     ,
     else do
        buff~append(sep, '"', str, '"')
        leave
     end
  end
  return buff~string

-- Return a date / time string.  This allows a consistent format of time stamps
-- throughout the test framework.
::method timeStamp class
  return "[".datetime~new~isoDate"]"


/* class: NoiseAdjustable- - - - - - - - - - - - - - - - - - - - - - - - - - -*\

    A class that allows adjusting the level of ouput ("noise") produced.

\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::class "NoiseAdjustable" public mixinclass Object

::constant MAX_VERBOSITY      10
::constant MIN_VERBOSITY       0
::constant DEFAULT_VERBOSITY   2

::attribute verbose private

::method setVerbosity
  use strict arg level

  if \.TestUtil~isWholeRange(level, self~MIN_VERBOSITY, self~MAX_VERBOSITY) then
    raise syntax 88.907 array("1 'level'", self~MIN_VERBOSITY, self~MAX_VERBOSITY, level)
  self~verbose = level

::method getVerbosity
  return self~verbose

-- End of class: NoiseAdjustable


/** class:  TestCollectingParameter
  *   Defines an interface for a test data collecting parameter.  The TestResult
  *   class and subclasses apply the 'Collecting Parameter' design pattern.
  *   This object is used to collect data throughout the execution of a test.
  */
::class 'TestCollectingParameter' public inherit NoiseAdjustable

/** Ouputs in some manner the formatted data of this collecting parameter.    */
::method print          abstract

/** Sets the default formatter class to use when print() is invoked.          */
::method setFormatter   abstract

/** Returns the current default formatter class of this collecting parameter  */
::method getFormatter   abstract

-- End of class TestCollectingParameter


::class "TestResult" public subclass TestCollectingParameter

::attribute logQueue               -- ooRexxUnit only
::attribute TestCaseTable          -- ooRexxUnit only

-- The default formatter class for this test result.
::attribute formatter private

::method init
  expose fErrors fFailures fRunTests fStop fTestRuns TestCaseTable logQueue fAssertions

  fErrors=.queue~new
  fFailures=.queue~new
  logQueue=.queue~new

  fAssertions=0
  fRunTests=0
  fStop=.false

  TestCaseTable=.table~new

  self~setVerbosity(self~DEFAULT_VERBOSITY)
  self~formatter = .SimpleFormatter

::method addError
  expose fErrors TestCaseTable logQueue
  use arg aTestCase, errData

  fErrors~queue(errData)
  logQueue~queue(errData)

  -- TODO find out who uses this info and tell them it is broke.
  TestCaseTable[aTestCase]~queue(errData)


::method addFailure
  expose fFailures TestCaseTable logQueue
  use arg aTestCase, failData

  fFailures~queue(failData)
  logQueue~queue(failData)

  -- TODO find out who uses this info and tell them it is broke.
  TestCaseTable[aTestCase]~queue(failData)

::method assertCount    -- ooRexxUnit only
  expose fAssertions
  return fAssertions

::method endTest        -- informs that the supplied test was completed
  expose TestCaseTable fStop logQueue fAssertions
  use arg aTestCase

  dateTime=.TestUtils~timeStamp
  TestCaseTable[aTestCase]~queue(dateTime": endTest")
  dir=.directory~new~~setentry("OOREXXUNIT.CONDITION", dateTime": [endTest]" aTestCase~string)

  -- save class' 'caseInfo' with test case for later analysis
  dir~setentry("OOREXXUNIT.class_testCaseInfo", aTestCase~class~caseInfo)

  logQueue~queue(dir)
  fAssertions=fAssertions+aTestCase~assertCount
  fStop=.false          -- reset indicator


::method errorCount     -- return # of errors
  expose fErrors
  return fErrors~items

::method errors         -- return error queue
  expose fErrors
  return fErrors


::method failureCount   -- return # of failures
  expose fFailures
  return fFailures~items

::method failures       -- return failure queue
  expose fFailures
  return fFailures


::method execute            -- convenience method to run given TestCase
  use arg aTestCase
  return aTestCase~execute(self)

::method runCount       -- gets the number of run tests
  expose fRunTests
  return fRunTests

::method shouldStop     -- return value
  expose fStop
  return fStop

::method startTest
  expose TestCaseTable fStop fRunTests logQueue
  use arg aTestCase

  if TestCaseTable~hasindex(aTestCase)=.false then -- already a queue created for it?
     TestCaseTable[aTestCase]=.queue~new

  dateTime=.TestUtils~timeStamp
  TestCaseTable[aTestCase]~queue(dateTime": startTest")
  dir=.directory~new~~setentry("OOREXXUNIT.CONDITION", dateTime": [startTest]") aTestCase~string)

  -- save class' 'testCaseInfo' with test case for later analysis
  dir~setentry("OOREXXUNIT.class_testCaseInfo", aTestCase~class~caseInfo)

  logQueue~queue(dir)

  fStop=.false          -- reset indicator
  fRunTests=fRunTests+1 -- increase run counter

::method stop           --  mark that the test run should stop
  expose fStop
  fStop=.true

::method wasSuccessful  -- returns whether the entire test was successful or not
  expose fErrors fFailures

  return (fErrors~items+fFailures~items)=0

/** print()
 * Output the result data using our assigned formatter.
 *
 * @param title    OPTIONAL    (String)
 *   Passes a title on to the formatter.
 *
 * @param level   OPTIONAL    (Whole Number)
 *   Sets the verbosity level of the print out by over-riding the verbosity
 *   level of this test result.  By default the verbosity of the print out will
 *   be that of this test result.
 */
::method print
  use arg title = "", level = (self~getVerbosity)

  if \ title~isA(.string) then
    raise syntax 88.914 array ("1 'title'", "String")

  formatter = self~formatter~new(self, title)

  if arg(2, 'E') then formatter~setVerbosity(level)
  else formatter~setVerbosity(self~getVerbosity)

  formatter~print

/** setFormatter()
 * Change the current default formatter class for this test result to that
 * specified.
 *
 * @param formatterClass  REQUIRED  (ResultFormatter)
 *   The new default formatter class for this test result.
 */
::method setFormatter
  use strict arg formatterClass

  if \formatterClass~isSubclassOf(.ResultFormatter) then
     raise syntax 88.914 array ("1 'formatterClass'", "ResultFormatter")
  self~formatter = formatterClass

/** getFormatter()
 * Returns the current default formatter class for this test result.
 */
::method getFormatter
  return self~formatter

-- End of class TestResult


/* class: ResultFormatter- - - - - - - - - - - - - - - - - - - - - - - - - - -*\

    An interface for an object that can format, and then output, the data
    contained in a test collecting parameter.

\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::class 'ResultFormatter' public inherit NoiseAdjustable

::attribute testResult private
::attribute title private

::method init
  use strict arg testResult, title = ""

  if \ testResult~class~isSubclassOf(.TestCollectingParameter) then
     raise syntax 88.914 array ("1 'testResult'", "TestCollectingParameter")

  self~testResult = testResult
  self~title = title
  self~setVerbosity(self~DEFAULT_VERBOSITY)

/** print()  Format and output the data of this formatter's test result. */
::method print abstract

/** setTitle()  Set a title for this formatter's output. */
::method setTitle abstract

-- End of class ResultFormatter


/* class: SimpleFormatter- - - - - - - - - - - - - - - - - - - - - - - - - - -*\

    Formats and prints to the console a simple rendition of a test result's
    data.

\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::class 'SimpleFormatter' public subclass ResultFormatter

::method setTitle
  use strict arg title

  if \ title~isA(.string) then
    raise syntax 88.914 array ("1 'title'", "String")

  self~title = title

::method print
  aTestResult = self~testResult

  if self~title <> "" then do
    say self~title
    say
  end

  say "nr of test runs:            " aTestResult~runCount
  say "nr of successful assertions:" aTestResult~assertCount

  say "nr of failures:             " aTestResult~failureCount
  if aTestResult~failureCount > 0 then do data over aTestResult~failures
    say "  " self~failureToString(data)
  end

  say "nr of errors:               " aTestResult~errorCount
  if aTestResult~errorCount > 0 then do err over aTestResult~errors
    say "  " self~errorToString(err)
  end

::method failureToString private
  use arg data

  str = data~when": [failure]" data~testString "---> @assertFailure" -
        data~type": expected="data~expected", actual="data~actual"."

  if data~msg <> "" then str = str ||"09"x || data~msg

return str

::method errorToString private
  use arg data

  str = data~when': [error]' data~testString '---> condition ['data~type'] raised unexpectedly.'

  if data~conditionObject~message <> .nil then str = str || '09'x || data~conditionObject~message

return str

-- End of class SimpleFormatter


/* class: SimpleConsoleFormatter - - - - - - - - - - - - - - - - - - - - - - -*\

    Formats a test result's data and prints it in a console friendly manner.

\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::class 'SimpleConsoleFormatter' public subclass ResultFormatter

::method setTitle
  use strict arg title
  if \ title~isA(.String) then
    raise syntax 88.914 array ("1 'title'", .string~id)

  self~title = title

::method print
  aTestResult = self~testResult

  if self~title <> "" then do
    say self~title
    say
  end

   versionStr = .ooRexxUnit.interpreterName .ooRexxUnit.languageLevel .ooRexxUnit.interpreterDate
   say "Interpreter:" versionStr
   say "ooRexxUnit: " .ooRexxUnit.version
   say
   say "Count of tests ran:            " aTestResult~runCount
   say "Count of successful assertions:" aTestResult~assertCount
   say "Count of failures:             " aTestResult~failureCount
   say "Count of errors:               " aTestResult~errorCount
   say

   if aTestResult~failureCount > 0 then do data over aTestResult~failures
      self~printFailureInfo(data)
   end

   if aTestResult~errorCount > 0 then do data over aTestResult~errors
      self~printErrorInfo(data)
   end

   -- If a number of failure or error information lines are printed, re-display
   -- the summary statistics again so that the number of failures is obvious to
   -- the user.
   if (aTestResult~failureCount + aTestResult~errorCount) > 3 then do
     say "Interpreter:" versionStr
     say "ooRexxUnit: " .ooRexxUnit.version
     say
     say "Count of tests ran:            " aTestResult~runCount
     say "Count of successful assertions:" aTestResult~assertCount
     say "Count of failures:             " aTestResult~failureCount
     say "Count of errors:               " aTestResult~errorCount
     say
   end


::method printFailureInfo private
  use arg data

  say "[failure]" data~when
  say "  Test:  " data~testname
  say "  Class: " data~className
  say "  File:  " data~where
  say "  Line:  " data~line
  say "  Failed:" data~type
  say "    Expected:" data~expected
  say "    Actual:  " data~actual

  if data~msg \== "" then
    say "    Message: " data~msg
  say

::method printErrorInfo private
  use arg data

  -- It is possible that the error happened in a file other than the test case
  -- file.  Most often the files are the same.
  different = (data~where~compareTo(data~conditionObject~program) <> 0)

  say "[error]" data~when
  say "  Test: " data~testName
  say "  Class:" data~className
  say "  File: " data~where
  say "  Event: ["data~type"] raised unexpectedly."
  if data~conditionObject~message \== .nil then
    say "    "data~conditionObject~message
  if different then
    say "    Program:" data~conditionObject~program
  say "    Line:   " data~line
  if data~conditionObject~traceBack~isA(.list) then do line over data~conditionObject~traceBack
    say line
  end
  say

-- End of class SimpleFormatter


/* class: ReportData - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\

    A base class for data objects reporting problems, events, or infomation
    during the execution of tests using the ooRexxUnit framework.

\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::class 'ReportData' public

::attribute when get
::attribute when set private

::attribute where get
::attribute where set private

::attribute type get
::attribute type set private

::attribute additionalObject
::attribute additional

::method init
  expose when where type
  use strict arg when, where, type, ...

  self~additionalObject = .nil
  self~additional = .nil


/* class: TestProblem- - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\

    The superclass for reporting problems during the execution of tests.

\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::class 'TestProblem' public subclass ReportData

::attribute className
::attribute testName
::attribute testString

::attribute conditionObject get
::attribute conditionObject set
  expose conditionObject
  use strict arg cObj
  if \ isConditionObj(cObj) then
    raise syntax 88.900 array ("conditionObject must be set to the object returned from CONDITION('O')")
  conditionObject = cObj

::method init
  expose conditionObject lineNumber
  forward class (super) continue

  conditionObject = .nil
  lineNumber = .nil

  self~className = .nil
  self~testName = .nil
  self~testString = .nil

::method setLine
  expose lineNumber
  use strict arg lineNumber

::method line
  expose lineNumber

  if lineNumber \== .nil then return lineNumber
  else return self~conditionObjLineNumber

/** conditionObjLineNumber()
 * Given an object, will return the line number of a syntax exception, if the
 * object is a condition object for a syntax exception.  Othewise returns -1.
 */
::method conditionObjLineNumber
  expose conditionObject testName

  -- belt and braces:  make sure we have a condition object
  if conditionObject == .nil then do
    return -1
  end

  do frame over conditionObject~stackFrames
      if frame~name~caselessEquals(testName) then do
           return frame~line
      end
  end

  return -1


/* class: AssertFailure- - - - - - - - - - - - - - - - - - - - - - - - - - - -*\

    AssertFailure is a report of a test case failure.  A test case fails when an
    assertion does not hold.

\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::class 'AssertFailure' public subclass TestProblem

::attribute expected
::attribute actual
::attribute msg

::method init
  forward class (super) continue

  self~expected = .nil
  self~actual = .nil
  self~msg = .nil


/* class: ErrorReport- - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\

    ErrorReports are used for unexpected errors during the execution of a test
    case.

    When an unexpected error happens during the execution of a test case, there
    is always a condition object at the time the error is trapped.  So, unlike
    AssertFailures, the condition object is required to instantiate an error
    report.

\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::class 'ErrorReport' public subclass TestProblem

::method init
  use strict arg dateTime, file, type, cObj
  forward class (super) continue

  self~conditionObject = cObj


/* class: Assert - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\

    Assertions will raise a user error, if they do not hold.

\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::class "Assert" public mixinclass Object

::attribute fAssertions private
::attribute definedInFile    -- allows to quickly get filename from which test case is taken

::method init
  self~fAssertions = 0
  self~clearCondition

::method assertCount
  return self~fAssertions     -- count assertions, ooRexxUnit only

-- test for collection equality.  This does not take order into account,
-- only that the collections contain the same members
::method isCollEqual
  use arg expected, received

  -- if both are collections, they need to be equivalent
  if expected~isA(.Collection), received~isA(.Collection) then
     return expected~equivalent(received)

  -- not collections, this is false
  return .false


/** assertEquals()
 * Checks if two objects are equal, or, if not, checks that the two objects are
 * equal collection objects.
 */
::method assertEquals
  use strict arg expected, actual, msg = ""

  if \ (expected = actual) then do
    if \self~isCollEqual(expected, actual) then do
      self~fail(self~makeFailure(expected, actual, msg, "assertEquals"))
    end
  end

  -- Assertion holds
  self~fAssertions += 1


/** assertFuzzyEquals()
 * Checks if two objects are equal using fuzzy math
 */
::method assertFuzzyEquals
  use strict arg expected, actual, digits = (digits()), msg = ""

  numeric digits digits
  numeric fuzz 1

  if \ (expected = actual) then do
      self~fail(self~makeFailure(expected, actual, msg, "assertFuzzyEquals"))
  end

  -- Assertion holds
  self~fAssertions += 1

::method assertNotEquals
  use strict arg expected, actual, msg = ""

  -- If the 2 objects are not equal AND they are not 2 equal collections ...
  if \ (expected = actual), \ self~isCollEqual(expected, actual) then do
    -- ... the assertion holds
    self~fAssertions += 1
    return
  end

  expected = self~formatObjectInfo(expected)
  actual   = self~formatObjectInfo(actual)

  self~fail(self~makeFailure(expected '\=' actual, expected ' =' actual, msg, "asserNotEquals"))


::method assertNull
  use strict arg actual, msg = ""

  if \ (.nil = actual) then do
    self~fail(self~makeFailure("[.nil]", actual, msg, "assertNull"))
  end

  -- Assertion holds.
  self~fAssertions += 1


::method assertNotNull
  use strict arg actual, msg = ""

  if (.nil = actual) then do
    self~fail(self~makeFailure("[Not .nil]", "[.nil]", msg, "assertNotNull"))
  end

  -- Assertion holds.
  self~fAssertions += 1

-- create a routine from an array of lines with the intent of checking
-- errors raised at translate time rather than runtime.
::method assertSyntaxError
  use strict arg error, code

  self~expectSyntax(error)
  -- this will translate this code and nothing else.
  r = .routine~new("test", code)

-- compare an array of items.  each item must be the same, and in the same
-- order.
::method assertSameList
  use strict arg expected, actual, msg = "assertSameList"

  self~assertTrue(actual~isa(.array), msg': actual must be an array')
  self~assertEquals(expected~items, actual~items, msg': expected and actual lists are different sizes')

  loop i = 1 to expected~last
      self~assertEquals(expected[i], actual[i], msg': mismatch on element' i)
  end

::method assertSame
  use strict arg expected, actual, msg = ""

  if \ (expected == actual) then do
    self~fail(self~makeFailure(expected, actual, msg, "assertSame"))
  end

  -- Assertion holds.
  self~fAssertions += 1


::method assertNotSame
  use strict arg expected, actual, msg = ""

  if (expected == actual) then do
    expected = self~formatObjectInfo(expected)
    actual   = self~formatObjectInfo(actual)

    self~fail(self~makeFailure(expected '\==' actual, expected ' ==' actual, msg, "assertNotSame"))
  end

  -- Assertion holds.
  self~fAssertions += 1

-- Check if an object is EXACTLY the same object instance
::method assertIdentical
  use strict arg expected, actual, msg = ""

  if (expected~identityHash != actual~identityHash) then do
    self~fail(self~makeFailure(expected, actual, msg, "assertIdentical"))
  end

  -- Assertion holds.
  self~fAssertions += 1

-- Check if an object is NOT EXACTLY the same object instance
::method assertNotIdentical
  use strict arg expected, actual, msg = ""

  if (expected~identityHash == actual~identityHash) then do
    self~fail(self~makeFailure(expected, actual, msg, "assertNotIdentical"))
  end

  -- Assertion holds.
  self~fAssertions += 1


/** assertEitherOr()
 * Exclusive or, either arg 1 is true or arg 2 is true, but not both.
 */
::method assertEitherOr
  use strict arg truthOne, truthTwo, msg = ""

  if \ self~exclusiveOr(truthOne, truthTwo) then do
    expected = '[ one .true and one .false ]'
    actual   = '['self~formatObjectInfo(truthOne) self~formatObjectInfo(truthTwo)']'
    self~fail(self~makeFailure(expected, actual, msg, "assertEitherOr"))
  end

  -- Assertion holds.
  self~fAssertions += 1


/** assertOneOrAnother()
 * Exclusive or for objects.  The actual object is expected to equal either one
 * object, or another object, but not both.
 */
::method assertOneOrAnother
  use strict arg one, other, actual, msg = ""

  if \ self~exclusiveOrObject(one, other, actual) then do
    expected = '['self~formatObjectInfo(one) 'or' self~formatObjectInfo(other)']'
    actual   = self~formatObjectInfo(actual)
    self~fail(self~makeFailure(expected, actual, msg, "assertOneOrAnother"))
  end

  -- Assertion holds.
  self~fAssertions += 1


::method assertTrue
  use strict arg actual, msg = ""

  if \ (actual = .true) then do
    self~fail(self~makeFailure("[1]", actual, msg, "assertTrue"))
  end

  -- Assertion holds.
  self~fAssertions += 1


::method assertFalse
  use strict arg actual, msg = ""

  if \ (actual = .false) then do
    self~fail(self~makeFailure("[0]", actual, msg, "assertFalse"))
  end

  -- Assertion holds.
  self~fAssertions += 1

/** assertFail()
 * Assert a failure explicitly.  Useful for situations where control is not expected
 * to reach a particular point.
 */
::method assertFail
  use strict arg msg = ""
  self~assertTrue(.false, msg) -- just do this as a true assertion


/** fail()
 * Raise an application definable syntax condition with code 93.964.  The
 * exception handler for the test case method execution will check for this
 * specific code and know that the 'syntax' condition is actually a condition
 * raised by the framework for a test case failure.
 *
 * The data failure object is passed through to the exception handler in index
 * 2 of the additional array.  Index 1 of the array contains a distinctive
 * message to further ensure that the exception handler will know that the
 * condition is raised by the framework.
 *
 * We raise a syntax condition because syntax conditions are propagated from
 * an active message invocation (the test case method invocation) to the clause
 * containing the message invocation.  This allows the exception handler to trap
 * the failed test case.
 *
 * @param data  REQUIRED (AssertFailure or String)  Data concerning the failure.
 */
::method fail
  use strict arg data

  if data~isA(.String) then data = self~makeFailure("n/a", "n/a", data, "fail")

  msg = "ooRexxUnit.cls - source of syntax exception 'FAIL' method invocation in class 'ASSERT'."

  RAISE syntax 93.964 array (msg, data)


::attribute conditionExpected private
::attribute conditionName private
::attribute conditionCode private
::attribute conditionMsg private


::method clearCondition
  self~conditionExpected = .false
  self~conditionName = .nil
  self~conditionCode = .nil
  self~conditionMsg = .nil

/** expectSyntax()
 * The syntax error code to expect is required.
 */
::method expectSyntax
  use strict arg errorCode, msg = .nil

  self~conditionExpected = .true
  self~conditionName = "SYNTAX"
  self~conditionCode = errorCode
  self~conditionMsg = msg


/** expectCondition()
 * Only the name of the condition is required, and it can be two words.  E.g.
 * "USER SOMETHING"
 */
::method expectCondition
  use strict arg name, msg = .nil

  self~conditionExpected = .true
  self~conditionName = name
  self~conditionMsg = msg


/** checkCondition()
 * Return true if the condition raised matches the condition expected, otherwise
 * return false.
 */
::method checkCondition

  use arg receivedCondition

  if self~conditionExpected, self~conditionName == receivedCondition~condition then do
     -- For a syntax error, the actual error code has to match the expected error code.
     if self~conditionName == "SYNTAX", self~conditionCode <> receivedCondition~code then
        return .false

     self~fAssertions += 1
     return .true
  end
  return .false


/** check4ConditionFailure()
 * Forces an assert failure if a condition is expected to be raised.  The
 * assumption is that this method is invoked when a condition has not been
 * raised.
 */
::method check4ConditionFailure

  if self~conditionExpected then do
     expected = self~conditionName
     type = 'expectCondtion'

     if expected == "SYNTAX", self~conditionCode <> .nil then do
        expected = expected self~conditionCode
        type = 'expectSyntax'
     end

    data = .AssertFailure~new(.TestUtils~timeStamp, self~definedInFile, type)

    data~expected = expected
    data~actual = "Not raised"
    if self~conditionMsg <> .nil then data~msg = self~conditionMsg
    else data~msg = ""

    self~fail(data)
  end


::method makeFailure private
  use strict arg expected, actual, msg, type

  data = .AssertFailure~new(.TestUtils~timeStamp, self~definedInFile, type)

  if expected~isA(.string), expected~left(1) == "[", expected~right(1) == "]" then
    data~expected = expected
  else
    data~expected = self~formatObjectInfo(expected)

  if actual~isA(.string), actual~left(1) == "[", actual~right(1) == "]" then
    data~actual = actual
  else
    data~actual = self~formatObjectInfo(actual)

  data~msg = msg

  return data

::method formatObjectInfo private
  use arg o, hint = ""

  buf = .MutableBuffer~new
  buf~append('[', hint, '[', .TestUtils~escapeString(o~string), ', identityHash="', o~identityHash, '"]')
  return buf~string

::method exclusiveOr private
  use strict arg t1, t2

  if \ t1~isA(.string) | \ t2~isA(.string) then return .false
  if \ t1~datatype('O') | \ t2~datatype('O') then return .false

  -- t1 and t2 are both logicals, for exclusive or they must be different
  -- logicals.
  return (t1 \== t2)


::method exclusiveOrObject private
  use strict arg one, other, actual

  if one == actual, other == actual then return .false
  if one \== actual, other \== actual then return .false
  return .true



/* *********************************************************************************** */
/* *********************************************************************************** */
::class "TestCase" public inherit "Assert"

::attribute defaultTestResultClass class
::attribute caseInfo class

::method init class
  self~defaultTestResultClass=.TestResult -- set default: use TestResult class
  self~caseInfo=.directory~new
  forward class (super)


::method init        -- constructor
  expose fName fCountTestCases    -- name of Testcase (method) to carry out
  parse arg fName
  if fname == "" & self~class~id == "text.testgroup" then do
    say 1/0
  end

  fCountTestCases=1  -- default: individual test
  self~caseInfo=.directory~new -- directory to contain information on test

  clzTCI=self~class~caseInfo    -- get access to clz' 'caseInfo' dir
  s=clzTCI~entry("test_Case-source")
  if .nil<>s then                   -- if source available, memorize fully qualified file name
     self~definedInFile=s
  else
    self~definedInFile="n/a_"self~identityHash -- indicate that no file name available

  self~init:super   -- let superclass initialize


::method caseInfo attribute                 -- ooRexxUnit only


::method createResult      -- creates a default TestResult object
  -- Use the default class in the environment, if it exists, before using the
  -- self~class~defaultTestResultClass.
  if .local~hasEntry(ooRexxUnit.default.TestResult.Class) then do
    if .ooRexxUnit.default.TestResult.Class~isA(.class) then
      return .ooRexxUnit.default.TestResult.Class~new
  end
  return self~class~defaultTestResultClass~new

::method "countTestCases=" private  -- set method
  expose  fCountTestCases
  use arg fCountTestCases


::method countTestCases -- return nr. of test cases (methods) in this class
  expose fCountTestCases
  return fCountTestCases


::method execute            -- will get implemented in subclasses
  expose fName
  use arg aTestResult, bGiveFeedback=.false

  if \datatype(bGiveFeedback,"O") then
     raise syntax 93.903 array (bGiveFeedback) -- raise error

   -- make sure an instance of .TestResult is used
  if arg(1, 'O') then aTestResult=self~createResult
  else aTestResult=arg(1)

  aTestResult~startTest(self)       -- remember test started
  self~setUp                        -- make sure setup is invoked before test
  if bGiveFeedback then
     .error~say( "... running TestCase object ["self~string"] ..." )

  self~doTheTest(fName, aTestResult)  -- carry out the testmethod
  self~tearDown                       -- make sure tearDown is invoked after test
  aTestResult~endTest(self)           -- remember test ended

  return aTestResult

::method doTheTest private
   use arg methodName, aTestResult

   -- Trap all conditions propagated from the invocation of the test case
   -- method, or from the check4ConditionFailure method.
   signal on any name exceptionHandler

   -- The test case method itself is invoked by creating a message object and
   -- sending it to ourself.
   .message~new(self, methodName)~send

   -- The test case method completed without raising a condition.  The last step
   -- is to check if the test case *exepected* a condition to be raised.
   self~check4ConditionFailure
   return aTestResult

exceptionHandler:

  -- Get the condition object.  (The condition object is a Directory object.)
  cObj = condition("O")

  -- If a condition was expected to be raised AND this condition matches the
  -- expected condition, then the test passed so just return.
  if self~conditionExpected, self~checkCondition(cObj) then return aTestResult

  -- If the condition code is 93.964 then this condition was raised by the
  -- framework to signal a test case failure.  The assert failue object is at
  -- index 2 of the additional array.
  if cObj~code = 93.964 then do
    data = cObj~additional[2]

    -- It does not seem conceivable that data is not the object I think it is.
    -- The best thing to do would be to check if it is an AssertFailure object
    -- and, if not, create an ErrorReport object and do aTestResult~addError()
    -- Skipping that for now.

    data~className = self~class~id
    data~testName = self~getName
    data~testString = self~string
    data~conditionObject = cObj
    data~additionalObject = self
    aTestResult~addFailure(self, data)
  end
  else do
    -- This is an unexpected, unanticipated error.
    type = cObj~condition
    if cObj~hasentry("CODE") then type = type cObj~code

    err = .ErrorReport~new(.TestUtils~timeStamp, self~definedInFile, type, cObj)
    err~className = self~class~id
    err~testName = self~getName
    err~testString = self~string
    err~additionalObject = self
    aTestResult~addError(self, err)
  end

  return aTestResult


::method getName     -- returns the name for this TestCase
  expose fName
  return fName

::method setName     -- set the name for this TestCase
  expose fName
  parse arg fName

-- Create a string representation of this test case
::method string
  buf = .mutableBuffer~new
  buf~append("testCase: [", self~getName, "] (", self~defaultName, "@", self~definedInFile, ")")
  return buf~string


/** setUp()
 * setUp is invoked immediately prior the invocation of *each* individual test
 * case method.  If needed, subclasses of TestCase can implement this method to
 * provide some type of pre-test set up.  Normally this is not needed.
 *
 * NOP is used to indicate that this method body is empty on purpose.
 */
::method setUp
  NOP

/** tearDown()
 * tearDown is the counter-point to setUp.  It is invoked immediately after the
 * invocation of *each* individual test case method.  If needed, subclasses of
 * TestCase can implement this method to provide some type of post-test clean
 * up.  Normally this is not needed.
 *
 * NOP is used to indicate that this method body is empty on purpose.
 */
::method tearDown
  NOP

/** dataItem
 * dataItem extracts an embedded set of data from the test classes.  The data
 * is in the form of a method that embeds the data as comment in the method code.
 * The first line of the method must be the line "return /*".  The last line must
 * be the line "*/ return".  All lines between the two delimiters are returned as
 * an array of data.
 *
 * The target method must begin with the characters "data" and is requested using
 * the name without the modifier.  For example, a dataitem named "xml1" would be
 * contained in an instance method named "dataxml1" and would be requested using
 * lines = self~dataitem("xml1")
 */
::method dataItem
  use strict arg name

  method = self~instanceMethod("DATA"||name~upper)
  self~assertNotNull(method, "Method DATA"||name~upper "not found")
  source = method~source
  self~assertTrue(source~size > 2, "Data item method does not contain correct data")
  -- extract the middle section of the data and return as an array
  return source~section(2, source~size - 2)


/* *********************************************************************************** */
/* *********************************************************************************** */
/* *********************************************************************************** */
/* *********************************************************************************** */
   -- parse file-info into the supplied directory object
   /*
      uses the information about the program in the very first block-comment at the top:

      - keyword":" text
            if keyword starts with "changed", "purpose", "remark", "link", "category" then
            entry is a queue and text will get enqueued at the end it; the first four letters
            are used for matching these words

      - arrLines:

   */
::method makeDirTestInfo

   tmpDir = caseInfo   -- get directory object to add infos to
   -- The test information can be stored as a resource within the file.
   -- if this is there, parse it out
   sourceInfo = self~class~package~resources~testinfo
   -- just punt if this is not there as a resource

   if sourceInfo == .nil then return

   keyWord=""

   -- we have data, parse it into info entries
   loop line over sourceInfo
      -- ignore all comments
      if line~strip~left(2)="--" || line = "" then iterate    -- ignore comment

      -- we might already have a keyword we're working on...in that case, we
      -- just append the data to the entry

         -- a keyWord already set and this line has no new keyword, than append it
      if \line~contains(":") then
      do
         if keyWord<>"" then  -- already a keyWord found, append this line to with a line-end
            tmpdir[keyword] ||= .endofline || line
         iterate
      end

      parse value line with keyword . ":" rest
      keyword = keyword~upper

      if \tmpDir~hasIndex(keyWord) then
         tmpDir[keyWord] = rest~strip
      else
         tmpdir[keyword] ||= .endofline || rest~strip
   end


/* *********************************************************************************** */
/* *********************************************************************************** */
::class "TestSuite" subclass TestCase public

::attribute testQueue private

::method init

  forward class (super) continue
  self~testQueue = .queue~new

      -- a class object, use reflection and create test cases
  if arg()>0 then       -- args there ?
  do
     use strict arg arg1    -- make sure, only one arg supplied

     if arg1~class~isSubclassOf(.TestSuite) then -- a TestSuite object ?
     do
        self~addTest(arg1)                -- just add it
        return
     end

      -- a TestCase class object in hand?
     else if arg1~isA(.class), arg1~isSubClassOf(.TestCase) then
     do
        testCaseClass=arg1
        testMethods=self~class~getTestMethods(testCaseClass)
        do name over testMethods          -- iterate over all test methods
           self~addTest(testCaseClass~new(name))
        end
        return
     end

      -- a collection of individual TestCase objects to add?
     else if arg1~isA(.Collection) then
     do
        do tc over arg1~allItems          -- iterate over test cases
           if tc~class~isSubClassOf(.TestCase) then
              self~addTest(tc)
           else
              raise syntax 88.914 array ("'collection item ["tc"]'", "'TestCase' or 'TestSuite'")
        end
        return
     end
     raise syntax 88.914 array ("1", "'TestCase', 'TestSuite' or 'Collection'")
  end
  return


-- Add a set of files to a given TestSuite object
::method addFileList
   use arg testCaseFileList

   -- do not run tests, if calling/requiring the testUnit files
   .ooRexxUnit~runTestsLocally = .false

   do fileName over testCaseFileList

       testUnitList=self~loadTestFile(fileName)

       do arr over testUnitList  -- loop over array objects
          classObject   =arr[1]  -- a class object
          mandatoryTests=arr[2]  -- a list

          -- mandatory tests available, just use them to create testCases
          if mandatoryTests~isA(.list), \mandatoryTests~isEmpty then do
            tsMand=.testSuite~new     -- create a test suite for this test class
            do testMethodName over mandatoryTests
               tsMand~addTest(classObject~new(testMethodName))   -- create and add testCase
            end
            ts~addTest(tsMand)      -- now add the test suite of mandatory methods to the overall test suite
          end
          -- no mandatory tests defined, hence use all testmethods
          else do
             ts~addTest(.testSuite~new(classObject))  -- creates testCases from all testmethods
          end
       end
   end
   return ts      -- return the testSuite object

::method loadTestFile private
  use strict arg filename

   signal on syntax
   r = .routine~newfile(fileName)

   -- call and return the return result, which should be a test group of
   -- what is contained in there.
   return r[]

syntax:
   .error~say("Error loading a test unit file:")
   .error~say("    testUnit-file: ["fileName"]")
   self~dumpConditionObject(.error, condition('o'))
   return .array~new    -- return empty array so no testsuite gets built for this testUnit

-- write the contents of the condition object to the given string, in sorted order by name
::method dumpConditionObject
   use arg outStream, condition

   -- get the index names and sort them
   names = co~allIndexes
   names~stableSort

   indent1 = 12

   sumIndent = indent1 + 2

   blanks=copies(" ", sumIndent) "--> "

   NL = .string~nl
   TAB1 = .string~tab
   TAB2 = TAB1~copies(2)
   TAB3 = TAB1~copies(3)
   TAB4 = TAB1~copies(4)

   do index over name
      value = co[index]
      if value~isA(.Collection) then items = o~items
                                else items

      tmpString=.MutableBuffer~new

      tmpString~append(TAB3, index~left(indent1,"."), "[", value~string, "]")
      if value~isA(.Collection) then
         tmpString~append(" containing ", items, " item(s)")

      -- write out the main value string
      outStream~say(tmpString~string)

      -- a collection object in hand?
      if value~isA(.Collection) then do
         loop item over value
            tmpString~setText("")
            tmpString~append(TAB3, blanks, "[", item~string, "]")
            outStream~say(tmpString~string)
         end
      end
   end

::method getTestMethods class -- use reflection to retrieve testmethods, sort alphabetically
  use strict arg classObj

  if \classObj~isA(.class) then
    raise syntax 88.914 array ("1 'classObj'", 'Class')

  -- Get the test methods names.  By convention this is all methods starting
  -- with "TEST".  We use a set because there may be duplicates in the
  -- class hierarchy
  testMethodNames = .set~new

 -- Get all methods that start with "TEST", even ones from the superclasses
 -- Some tests might have method overrides that test different variants of
 -- the base test methods
  methSupplier = classObj~methods
  do while methSupplier~available -- iterate over supplied methods
     name = methSupplier~index
     if name~match(1, "TEST") then testMethodNames~put(name)
     methSupplier~next
  end

  return testMethodNames~makearray~sort


-- add a test to the queue of individual test cases to run
::method addTest
  use strict arg aTestCase

  if \aTestCase~class~isSubclassOf(.TestCase) then
     raise syntax 88.914 array ("'aTestCase'", "'TestCase'")

  self~testQueue~queue(aTestCase)
  self~countTestCases += 1

::method displayString
  return self~string'@'self~identityHash

::method execute
  use arg aTestResult = (self~createResult), bGiveFeedback = .false

  if bGiveFeedback then
     .error~say( "running testSuite [" self~displayString"] with ["self~countTestCases"] test cases ...")

  tests = self~testQueue

  aTestResult~startTest(self)       -- remember test started
  self~setUp                        -- make sure setup is invoked before testSuite runs
  do aTestCase over tests while aTestResult~shouldStop=.false
     aTestCase~execute(aTestResult, bGiveFeedback)
  end
  self~tearDown                     -- make sure tearDown is invoked after testSuite ran
  aTestResult~endTest(self)         -- remember test ended

  return aTestResult


/** setUp()
 * setUp is invoked immediately prior to the invocation of this test suite's
 * execute() method.  That means it will be invoked exactly once prior to the
 * execution of the tests contained in this test suite.
 *
 * Subclass the TestSuite class and implement a setUp method to provide a method
 * that will run prior to the execution of all tests the test suite contains.
 *
 * NOP is used to indicate that this method body is empty on purpose.
 *
 * Note:  When subclassing the TestSuite, do not inovke the superclass setUp()
 * method.  That will invoke the TestCase setUp() method, which is normally not
 * what is desired.
 */
::method setUp
  NOP

/** tearDown()
 * tearDown is the counter-point to setUp.  It is invoked immediately after the
 * execution of all test cases contained by this test suite has finished.
 *
 * Subclasses of TestSuite can implement this method to provide some a clean up
 * method that is invoked after all the test cases have been executed.
 *
 * NOP is used to indicate that this method body is empty on purpose.
 *
 * Note:  When subclassing the TestSuite, do not inovke the superclass
 * tearDown() method.  That will invoke the TestCase tearDown() method, which is
 * normally not what is desired.
 */
::method tearDown
  NOP


/* *********************************************************************************** */
/* *********************************************************************************** */
-- routines

/* *********************************************************************************** */
/* *********************************************************************************** */












   -- simple dumping of the testResult data
::routine simpleDumpTestResults public
   use arg aTestResult, title = ""

   f = .SimpleFormatter~new(aTestResult, title)
   f~print


/**
 * Public routine simpleFormatTestResults()
 *
 *  Variation on simpleDumpTestResults() that formats the result output in a
 *  more "console-friendly" manner.  The information is broken up into lines,
 *  with an attempt made to keep all lines no longer than 80 characers wide.
 */
::routine simpleFormatTestResults public
   use arg aTestResult, title = ""

   f = .SimpleConsoleFormatter~new(aTestResult, title)
   f~print

return 0
