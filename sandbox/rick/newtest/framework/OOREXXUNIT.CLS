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
  expose interpreterName interpreterDate interpreterLanguageLevel osname shellname frameworkDirectory originalPath architecture

  interpreterName = .rexxinfo~name
  interpreterDate = .rexxinfo~date
  interpreterLanguageLevel = rexxinfo~languageLevel

  -- Note:  initialize the OSName first, other methods depend on this
  OSName = .rexxinfo~platform
  -- make the Windows one a little more generic
  if OSName == "WindowsNT" then OSName = "Windows"

  shellName = self~getShellName

  -- this is our default class for reporting errors.  Can be replaced
  self~defaultTestResultClass = .TestResult

  -- Define a 'marker' that can be used to flag a known test case failure.
  self~knownBugFlag = "tracker bug #"

  -- Capture the ooRexxUnit framework directory and ensure it is in the path.
  -- Also capture the original path in case someone needs it.
  frameworkDirectory = .File~new(.context~package~name)~parent
  -- set up an original search path for files, includeing the framework directory
  originalPath = self~addToPath(self~frameWorkDirectory)

  architecture = .rexxinfo~architecture

-- class attributes for global data
::constant version '3.0.0'
::constant minimumInterpreterVersion '5.0.0'

::attribute shellName class get
::attribute OSName class get

::attribute defaultTestResultClass class
::attribute knownBugFlag class
::attribute frameworkDirectory class get
::attribute originalPath class get
::attribute architecture class get


-- Determine and return the shell name to be used in ADDRESS keyword statements. This
-- is a central location to make it easy to maintain in the future, in case a non-Windows
-- and non-Unix compatible operating systems comes up, or new shell variants develop.
::method getShellName private class
  expose OSName


   -- make sure we address the shell
   shell="CMD"

   -- for now, we have a Windows/not Windows situation.
   if \OSName~caselessEquals("Windows") do
      -- get the fully qualified shell name from the environment
      unixShell=value("SHELL", , "environment")
      // and get the shell name portion
      shell = .File~new(unixShell)~name
   end
   return shell

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

::attribute logQueue

-- The default formatter class for this test result.
::attribute formatter private

::method init
  expose errorList failureList testCount stop testRuns log assertionCount

  errors=.array~new
  failures=.array~new
  log=.TestLog~new

  assertionCount = 0
  testCount = 0
  step = .false

  self~setVerbosity(self~DEFAULT_VERBOSITY)
  self~formatter = .SimpleFormatter

-- add an error to the result
::method addError
  expose errors testCases log
  use strict arg testCase, errData

  errors~append(errData)
  log~logError(errData)

-- add a failure to the result
::method addFailure
  expose failureCount log
  use strict arg testCase, failureData

  failures~append(failureData)
  log~logFailure(failData)

::attribute assertionCount GET

-- Record test completion
::method endTest
  expose stop log assertionCount
  use strict arg testCase

  -- have the test case log the test end
  testCase~logEndTest
  -- and add an end event to our test log
  log~logEndTest(testCase)

  -- add this into the assertion totals
  assertionCount += testCase~assertionCount

  -- reset the termination indicator
  stop = .false

::attribute errorCount get
  expose errors
  return errors~items

::attribute errors get

::attribute failureCount
  expose failures
  return failures~items

::attribute failures get

::attribute testCount get

-- test if we should stop running tests
::method shouldStop
  expose stop
  return stop

::method startTest
  expose stop testCount log
  use arg testCase

  -- log this both with the test case and globally
  testCase~logTestStart
  log~logTestStart(testCase)

  -- reset the stop indicator for a new test
  stop = .false
  testCount += 1

--  mark that the test run should stop
::method stop
  expose stop
  stop=.true

-- returns whether the entire test was successful or not
::method wasSuccessful
  return self~errorCount + self~failureCount = 0


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
  use strict arg title = "", level = (self~getVerbosity), outStream = .output

  formatter = self~formatter~new(self, title, level)
  formatter~print(outStream)

::attribute formatter

-- End of class TestResult


/* class: ResultFormatter- - - - - - - - - - - - - - - - - - - - - - - - - - -*\

    An interface for an object that can format, and then output, the data
    contained in a test collecting parameter.

\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::class 'ResultFormatter' public inherit NoiseAdjustable

::attribute testResult private
::attribute title private

::method init
  expose testResult title
  use strict arg testResult, title = ""

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

::method print
  use strict arg outStream = .output

  testResult = self~testResult

  if self~title <> "" then do
    outStream~lineout(self~title)
    outStream~lineout
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


-- messages templates for reporting various test problems.
::resource TestProblemMessages

::END

--  A base class for data objects reporting problems, events, or infomation
--  during the execution of tests using the ooRexxUnit framework.
::class 'ReportData' public

::attribute timeStamp get
::attribute where get
::attribute type get
::attribute additionalObject

::method init
  expose timeStamp where type additionalObject additional
  use strict arg where, type

  timeStamp = .datetime~new

  additionalObject = .nil
  additional = .nil


-- The base classs for reporting test problems
::class 'TestProblem' public subclass ReportData
::method activate
  expose messages
  messages = .stringtable~new

  loop line over .context~package~resource('errorMessages')
     parse var line id text
     messages[id] = text
  end

::method init
  expose conditionObject lineNumber testClass testName testString
  forward class (super) continue

  conditionObject = .nil
  lineNumber = .nil

  className = .nil
  testName = .nil
  testString = .nil

-- format a error message
::method formatMessage
  expose messages
  use strict arg id, substitutions, extra

  text = messages[id]
  buffer = .mutablebuffer~new(text)

  loop i = 1 to substitutions~last
     buffer~changestr('&'i, self~formatObject(substitutions[i]))
  end

  buffer~append(": ", extra)

::attribute testClass
::attribute testName
::attribute lineNumber
::attribute testString

::attribute conditionObject


--  AssertFailure is a report of a test case failure.  A test case fails when an
--  assertion does not hold.
::class 'AssertFailure' public subclass TestProblem
::method init
  expose expected actual msg
  forward class (super) continue

  expected = .nil
  actual = .nil
  msg = .nil

::attribute expected
::attribute actual
::attribute msg


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

::attribute assertions get
::attribute assertions set private
::attribute definedInFile    -- allows to quickly get filename from which test case is taken

::method init
  self~assertions = 0
  self~clearCondition

-- count a successful assertion
::method assertionPassed
  expose assertions

  assertions += 1


-- checks that two collections are equilvalent.  Does not take
-- order into account.  Use assertArraysEqual or assertArraysStrictlyEqual
-- if order is important
::method assertCollectionsEqual

  self~assertTrue(actual~isa(.Collection), msg': actual must be an array')
  self~assertTrue(expected~equivalent(actual), msg': actual must be equivalent to expected')

-- Checks if two objects are equal
::method assertEquals
  use strict arg expected, actual, msg = ""

  -- first compare the objects directly, and if they do not compare,
  -- try to compare them as collections.  Note that we use the "=" comparison
  -- variant here.
  if expected != actual then do
      self~fail(self~makeFailure(expected, actual, msg, "assertEquals"))
  end

  -- Assertion holds
  self~assertionPassed


--  assertFuzzyEquals()
-- Checks if two objects are equal using fuzzy math
::method assertFuzzyEquals
  use strict arg expected, actual, digits = (digits()), msg = ""

  numeric digits digits
  numeric fuzz 1

  if expected != actual then do
      self~fail(self~makeFailure(expected, actual, msg, "assertFuzzyEquals"))
  end

  -- Assertion holds
  self~assertionPassed


-- assert that a result does not equal a given value
::method assertNotEquals
  use strict arg expected, actual, msg = ""

  -- If the 2 objects are not equal AND they are not 2 equal collections ...
  if expected != actual then do
    -- ... the assertion holds
    self~assertionPassed
    return
  end

  expected = self~formatObjectInfo(expected)
  actual   = self~formatObjectInfo(actual)

  self~fail(self~makeFailure(expected '\=' actual, expected ' =' actual, msg, "assertNotEquals"))


-- assert that the result is equal to .nil
::method assertNull
  use strict arg actual, msg = ""

  if .nil \== actual then do
    self~fail(self~makeFailure("[.nil]", actual, msg, "assertNull"))
  end

  -- Assertion holds
  self~assertionPassed


-- assert that a result is not equal to .ni.
::method assertNotNull
  use strict arg actual, msg = ""

  if .nil == actual then do
    self~fail(self~makeFailure("[Not .nil]", "[.nil]", msg, "assertNotNull"))
  end

  -- Assertion holds
  self~assertionPassed

-- verify that a result is an instance of a particular class
::method assertIsA
  use strict arg target, class, msg = 'assertIsA'

  self~assertTrue(target~isa(class), msg)


-- compare an array of items.  each item must be the same, and in the same
-- order.  Item comparisons are the non-strict variety
::method assertArraysEqual
  use strict arg expected, actual, msg = "assertArraysEqual"

  self~assertTrue(actual~isa(.array), msg': actual must be an array')
  self~assertEquals(expected~items, actual~items, msg': expected and actual lists are different sizes')
  self~assertEquals(expected~last, actual~last, msg': have different index structures')

  loop i = 1 to expected~last
      self~assertEquals(expected[i], actual[i], msg': mismatch on element' i)
  end


-- compare an array of items.  each item must be the same, and in the same
-- order.  Item comparisons are the strict variety
::method assertArrayStrictlyEquals
  use strict arg expected, actual, msg = "assertArraysStrictlyEqual"

  self~assertTrue(actual~isa(.array), msg': actual must be an array')
  self~assertEquals(expected~items, actual~items, msg': expected and actual lists are different sizes')
  self~assertEquals(expected~last, actual~last, msg': have different index structures')

  loop i = 1 to expected~last
      self~assertStrictlyEquals(expected[i], actual[i], msg': mismatch on element' i)
  end


-- create a routine from an array of lines with the intent of checking
-- errors raised at translate time rather than runtime.
::method assertSyntaxError
  use strict arg error, code

  self~expectSyntax(error)
  -- this will translate this code and nothing else.
  r = .package~new("test", code)


-- perform a strict comparison on two objects.
::method assertStrictlyEqual
  use strict arg expected, actual, msg = ""

  if expected \== actual then do
    self~fail(self~makeFailure(expected, actual, msg, "assertSame"))
  end

  -- Assertion holds
  self~assertionPassed


::method assertNotStrictlyEqual
  use strict arg expected, actual, msg = ""

  if (expected == actual) then do
    expected = self~formatObjectInfo(expected)
    actual   = self~formatObjectInfo(actual)

    self~fail(self~makeFailure(expected '\==' actual, expected ' ==' actual, msg, "assertNotSame"))
  end

  -- Assertion holds
  self~assertionPassed

-- Check if an object is EXACTLY the same object instance
::method assertIdentical
  use strict arg expected, actual, msg = ""

  if (expected~identityHash != actual~identityHash) then do
    self~fail(self~makeFailure(expected, actual, msg, "assertIdentical"))
  end

  -- Assertion holds
  self~assertionPassed

-- Check if an object is NOT EXACTLY the same object instance
::method assertNotIdentical
  use strict arg expected, actual, msg = ""

  if (expected~identityHash == actual~identityHash) then do
    self~fail(self~makeFailure(expected, actual, msg, "assertNotIdentical"))
  end

  -- Assertion holds
  self~assertionPassed


/** assertEitherOr()
 * Exclusive or, either arg 1 is true or arg 2 is true, but not both.
 */
::method assertEitherOr
  use strict arg truthOne, truthTwo, msg = ""

  -- assume this works.  Allow an error to be raised if these are not
  -- logicals
  if \ truthOne && truthTwo then do
    expected = '[ one .true and one .false ]'
    actual   = '['self~formatObjectInfo(truthOne) self~formatObjectInfo(truthTwo)']'
    self~fail(self~makeFailure(expected, actual, msg, "assertEitherOr"))
  end

  -- Assertion holds
  self~assertionPassed


-- assert that an object is one of a given list
::method assertOneOf
  use strict arg choices, actual, msg = ""

  if \choices~hasItem(actual) then
    -- needs rework here

    expected = '['self~formatObjectInfo(one) 'or' self~formatObjectInfo(other)']'
    actual   = self~formatObjectInfo(actual)
    self~fail(self~makeFailure(expected, actual, msg, "assertOneOrAnother"))
  end

  -- Assertion holds
  self~assertionPassed


-- assert that a condition is true
::method assertTrue
  use strict arg actual, msg = ""

  if \actual then do
    self~fail(self~makeFailure("[1]", actual, msg, "assertTrue"))
  end

  -- Assertion holds
  self~assertionPassed


-- assert that a condition value is false
::method assertFalse
  use strict arg actual, msg = ""

  if actual then do
    self~fail(self~makeFailure("[0]", actual, msg, "assertFalse"))
  end

  -- Assertion holds
  self~assertionPassed

-- create a package from an array of lines with the intent of checking
-- errors raised at translate time rather than runtime.
::method assertSyntaxError
  use strict arg error, code

  self~expectSyntax(error)
  -- this will translate this code and nothing else.
  r = .package~new("test", code)


-- create a routine and call from an array of lines with the intent of checking
-- errors raised at run time rather than during translation.
::method assertRuntimeError
  use strict arg error, code

  self~expectSyntax(error)
  -- this will create a routine and then we invoke it, returning the
  -- code result.
  r = .routine~new("test", code)
  return r[]

-- run some dynamically created source code that will return a result.
::method runDynamicSource
  use strict arg code

  -- this will create a routine object and return the result.
  r = .routine~new("test", code)
  return r[]


-- Assert a failure explicitly.  Useful for situations where control is not expected
-- to reach a particular point.
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


/** conditionObjLineNumber()
 * Given an object, will return the line number of a syntax exception, if the
 * object is a condition object for a syntax exception.  Othewise returns -1.
 */
::method conditionObjLineNumber
  expose conditionObject

  return self~testLineNumber(conditionObject~stackFrames, testMethod)



-- clear any expected condition information
::method clearCondition
  expose expectedCondition

  expectedCondition = .nil


-- indicate that the test is expecting a particular syntax error to be raised.
::method expectSyntax
  -- the details are handled by the current testcase under execution
  forward to(currentTestCase)


-- indicate that a particular condition type is expected.
::method expectCondition
  -- the details are handled by the current testcase under execution
  forward to(currentTestCase)


-- check if to see if received condition matches a received condition
::method checkCondition
  expose expectedCondition
  use arg receivedCondition

  -- if we're not expecting a condition, this is a failure
  if self~expectedCondition == .nil then return .false

  -- if we have a match, this is a good assertion
  if expectedCondition~matches(receivedCondition) then do
     self~assertionPassed
     return .true
  end
  return .false


-- Forces an assert failure if a condition is expected to be raised.  The
-- assumption is that this method is invoked when a condition has not been
-- raised.
::method checkConditionFailure
  expose expectedCondition

  if expectedCondition \= .nil then do

       -- this needs a complete redoing
       expected = self~conditionName
       type = 'expectCondtion'

       if expected == "SYNTAX", self~conditionCode <> .nil then do
          expected = expected self~conditionCode
          type = 'expectSyntax'
       end

      data = .AssertFailure~new(self~definedInFile, type)

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


-- a holder for expected syntax information
::class "ExpectedCondition"
::method init
  expose condition code assertionLine message
  use strict arg assertionLine, condition , code = .nil, message = .nil

::attribute condition get
::attribute code get
::attribute message get
::attribute assertionLine

-- do a check for condition equality
::method isMatch
  expose condition code
  use strict arg received

  -- note:  for non syntax conditions, the code in the condition object
  -- will be .nil, so this will compare equal.
  return condition == received~condition & code == receive~code


-- this represents a single test method within a TestCase.  This
-- handles the details of running a single test method and recording the
-- results
::class "TestMethod"
-- create an individual test associated with a TestCase
::method init
  expose testCase testName testMethod expectecCondition
  use strict arg testCase, testName, testMethod

  expectedCondition = .nil

::method expectSyntax
  expose expectedCondition
  use strict arg errorCode, msg = .nil

  expectedCondition = .ExpectedCondition~new(self~testLineNumber(.context~stackFrames), "SYNTAX", errorCode, msg)

-- indicate that a particular condition type is expected.
::method expectCondition
  expose expectedCondition
  use strict arg name, msg = .nil

  expectedCondition = .ExpectedCondition~new(self~testLineNumber(.context~stackFrames), name,, msg)


-- Given a collection of stack frames, return the line number of the line in the
-- test method as a failure point.
::method testLineNumber
  expose testMethod
  use strict arg frames

  do frame over frames
      if frame~executable == testMethod then do
           return frame~line
      end
  end

  -- not found for some reason
  return -1

-- execute a specific test case method
::method execute
  expose testCase

  -- create the test result
  testReult = self~createResult

  -- record the test
  testResult~startTest(self)

  testCase~feedBack( "... running TestCase object ["self~string"] ..." )

  -- setup for the test, creating a clean environment
  testCase~peformTestSetup

  self~doTheTest(aTestResult)  -- carry out the testmethod

  -- cleanup after  the test, creating a clean environment
  testCase~peformTestSetup

  testResult~endTest(self)           -- remember test ended
  return testResult

-- wrapper around executing the actual test method
::method doTheTest
   expose testName testMethod
   use arg testResult

   -- Trap all conditions propagated from the invocation of the test case
   -- method, or from the checkForConditionFailure method.
   signal on any name exceptionHandler

   -- save the target method for error tracking
   testMethod = self~instanceMethod(methodName)

   -- now dynamically invoke the method
   self~send(methodName)

   -- The test case method completed without raising a condition.  The last step
   -- is to check if the test case *exepected* a condition to be raised.
   self~checkForConditionFailure
   return testResult

exceptionHandler:

  -- Get the condition object.  (The condition object is a Directory object.)
  cObj = condition("O")

  -- If a condition was expected to be raised AND this condition matches the
  -- expected condition, then the test passed so just return.
  if self~conditionExpected, self~checkCondition(cObj) then return aTestResult

  -- If the condition code is 93.964 then this condition was raised by the
  -- framework to signal a test case failure.  The assert failure object is at
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
    testResult~addFailure(self, data)
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
    testResult~addError(self, err)
  end

  return testResult

-- Create a string representation of this test case
::method string
  expose testname testCase

  buf = .mutableBuffer~new
  buf~append("testCase: [", testName, "] ("testCase~string, ")")
  return buf~string


-- base class for a TestCase class.  This is a class that implements
-- a set of test methods.  This handles invocation of the different
-- test methods and recording of result information from the tests.
::class "TestCase" public inherit "Assert"
::method init
  expose fileName

  -- get the containing file name for reporting.
  fileName = self~class~package~name
  -- initialize the superclass
  self~init:super

-- locate the test method names for this test case class.  Returns
-- an array of executable testMethod objects.
::method getTestMethods

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

  -- return the method names in sorted order.
  return testMethodNames~makearray~sort

-- create a TestResult object.  Subclasses may override this to
-- supply different types
::method createResult
  return .TestResult~new

-- methods for temporarily disabling out
::method disableOutput
  .output~destination(.NullOutput~new)

::method disableError
  .error~destination(.NullOutput~new)

::method disableTraceOutput
  .traceOutput~destination(.NullOutput~new)

::method enableOutput
  .output~destination(.stdout)

::method enableError
  .error~destination(.stderr)

::method enableTraceOutput
  .traceOutput~destination(.error)

-- perform any environment setup required before running a test method
::method performTestSetup
  -- this is invoked before each test method is run
  testcase~setUp

  -- make sure the output streams are in default state
  .output~destination(.stdout)
  .error~destination(.stderr)
  .traceoutput~destination(.stderr)

-- perform any post test environment cleanup
::method performTestTeardown

  -- make sure the output streams are in default state again
  .output~destination(.stdout)
  .error~destination(.stderr)
  .traceoutput~destination(.stderr)

  -- tearDown is also invoked after each test
  testcase~tearDown


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

-- Create a string representation of this test case class
::method string
  expose filename

  buf = .mutableBuffer~new
  buf~append(testCase~defaultName, '@', filename, ")")
  return buf~string


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

-- a class for creating/deleting temporary test files
::class TemporaryTestFile public
::method init
  expose file package
  use strict arg host, name

  -- get the package the class is defined in
  package = host~class~package

  dir = .File~new(package~name)~parentFile
  file = .File~new(name, dir)

-- get the full resolved name of this file
::method fullName
  expose file
  return file~absolutePath

-- create a file from an array of lines
::method create
  expose file
  use strict arg lines

  -- make sure this is deleted
  file~delete

  s = .stream~new(file)

  if lines~isA(.String) then do
    s~lineout(lines)
  end
  else do
    s~arrayout(lines)
  end
  s~close

-- append an array of lines to a file
::method append
  expose file
  use strict arg lines

  s = .stream~new(file)
  s~open('WRITE APPEND')
  if lines~isA(.String) then do
    s~lineout(lines)
  end
  else do
    s~arrayout(lines)
  end
  s~close

-- create a file from a package resource
::method createFromResource
  expose package
  use strict arg resourceName

  self~create(package~resources~resourceName)

-- append a package resource to a file
::method appendFromResource
  expose package
  use strict arg resourceName

  self~append(package~resources~resourceName)

-- allow the file to be deleted
::method delete
  expose file
  file~delete

-- clean up the temporary file after termination.
::method uninit
  expose file
  file~delete

-- a dummy class for disabling output to .output or .debugoutput
::class NullOutput
::method lineout
  return 0


