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

  -- loop searching for sections of non-printable charactes
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


-- Record the result from an individual test method.  This will hold all of the
-- error/failure information, plus a count of the successful assertions.
::class "TestResult" public
::method init
  expose testMethod log errorInformation assertionCount isError isFailure
  use strict arg testMethod

  assertionCount = 0
  errorInformation = .nil
  log = testMethod~log
  isError = .false
  isFailure = .false

-- add an error to the result
::method setError
  expose errorInformation log isError
  use strict arg errorInformation

  log~logError(self, errorInformation)
  isError = .true

-- add a failure to the result
::method setFailure
  expose errorInformation log expose isFailure
  use strict arg errorInformation

  log~logFailure(self, errorInformation)
  isFailure = .true

::attribute assertionCount GET

-- Record test completion
::method endTest
  expose stop log assertionCount

  -- and add an end event to our test log
  log~logEndTest(self)

::method startTest
  expose log

  log~logTestStart(self)

-- returns whether the entire test was successful or not
::method wasSuccessful
  expose errorInformation
  return errorInformation == .nil

::attribute isFailure get
::attribute isError get

-- display any failure information
::method displayFailure
  expose errorInformation
  use strict arg output

  if errorInformation \= .nil then
     errorInformation~display(output)


-- an accumulated set of test results
::class "ResultSet"
::method init
  expose tests assertions failures errors failureResult testId
  use strict arg testId

  tests = 0
  assertions = 0
  failures = 0
  errors = 0
  failureResults = .array~new

::attribute tests get
::attribute assersions get
::attribute failures get
::attribute errors get
::attribute failureResults get

-- mark the start of a set of tests
::method testStart
  expose startTime

  startTime = .DateTime~new

-- mark the end of a set of tests
::method testEnd
  expose endTime

  endTime = .DateTime~new

-- add a new result to the set for this test case
::method addResult
  expose tests assertions failures errors failureResults
  use strict arg result

  -- this is always an additional test
  tests += 1
  -- even failures can have successful assertions
  assertions += result~assertions

  -- assertion failure of some sort?  Save the failure
  -- information for the printout/log
  if result~isFailure then do
     failures += 1
     failureResults~append(result)
  end
  -- could be an unexpected exception, which is an error
  else if result~isError then do
     erross += 1
     failureResults~append(result)
  end

-- test if this test has any error information
::method wasSuccessful
  expose errors failures

  return errors + failures = 0

-- merge a result set into a another result set to accumulate.
-- used mostly for test suites that give a consoladated error
-- report.
::method merge
  expose tests assertions failures errors failureResults
  use strict arg otherSet

  tests += otherSet~tests
  assertions += otherSet~assertions
  errors += otherSet~errors
  failures += otherSet~failures

  failureResults~appendAll(otherSet~failureResults)

-- display this exception status
::method display
  expose tests assertions failures errors failureResults testId startTime endTime
  use strict arg output = .output

  output~lineout("Test Name       "  testId
  output~lineout("Interpreter:    " .rexxinfo~name)
  output~lineout("Addressing Mode:" .rexxInfo~architecture)
  output~lineout("ooRexxUnit:     " .ooRexxUnit~version)
  output~lineout
  output~lineout("Tests run:         " tests
  output~lineout("Assertions:        " assertions
  output~lineout("Failures:          " failures
  output~lineout("Errors:            " errors

  if \failureResults~isEmpty then do
     output~lineout
     loop result over failureResults
        output~lineout
        result~displayFailure(output)
     end
  end

  output~lineout
  output~lineout("Execution time:    " endTime - startTime


-- messages templates for reporting various test problems.
::resource TestProblemMessages
assertEqual Actual must be equal (=) to expected.
assertNotEqual Actual must not be equal (\=) to expected.
assertStrictlyEqual Acutual must be strictly equal (==) to expected.
assertStrictlyNotEqual Actual must not be strictly equal (\==) to expected.
assertFuzzyEqual Actual is not equal to expected using fuzzy comparison.
assertNull Actual must be The NIL object.
assertNotNull Actual cannot be The NIL object.
assertIsA Actual must be an instance of the expected class.
assertArrayEquals The actual array must match the expected array in all details.
assertIdentical Actual must be the same object instance as expected.
assertNotIdentical Actual must not be the same object instance as expected.
assertOneOf Actual must be strictly equal to one of the list objects.
assertTrue Actual must be .true.
assertFalse Actual must be .false.
assertFail Control unexpected reached a failure point.


::END

-- The base class for reporting test problems
::class 'TestFailure' public
::method activate
  expose messages
  messages = .stringtable~new

  loop line over .context~package~resource('errorMessages')
     parse var line id text
     messages[id] = text
  end

-- initialize a test failure.  At this level, we are given the
-- testMethod object, which contains all of the relevant test information,
-- and type type of failure
::method init
  expose testMethod failureType timestamp
  use strict arg testMethod, failureType

  timestamp = .datetime~new

::method formatObject
  use strict arg o

  buf = .MutableBuffer~new
  buf~append('[', .TestUtils~escapeString(o~string), ', identityHash="', o~identityHash, '"]')
  return buf~string

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

-- retrieve a specific message identifier
::method messageText
  expose messages
  use strict arg id

  return messages~entry(id)

-- display the test failure information
::method displayFailure
  expose testMethod failureType timestamp
  use strict arg output

  output~lineout('['failureType'] ['timestamp~isodate']')
  testMethod~display(output)


--  AssertionFailure is a report of a test case failure.  A test case fails when an
--  assertion does not hold.  This is the base assertion failure.
::class 'AssertionFailure' public subclass TestFailure
::method init
  expose message assertionLine assertionType secondaryAssertion
  use strict arg testMethod, assertionType, message

  self~init:super(testMethod, "Assertion Failure")
  secondaryAssertion = .nil
  assertionLine = testMethod~testLineNumber

-- display the test failure information
::method displayFailure
  expose message assertionLine assertionType secondaryException
  use strict arg output

  -- display the base information
  self~displayFailure:super(output)
  output~lineout("  Line:   " assertionLine)
  if message ~= .nil then do
      output~lineout("  Purpose:" message)
  end
  output~lineout("  Failed: " self~assertionMessage)
  if secondaryException \= .nil then do
     output~lineout("Failure Cause:")
     secondaryException~displaySecondaryFailure(output)
  end


-- display the test secondary failure information
::method displaySecondaryFailure
  expose message
  use strict arg output

  if message ~= .nil then do
      output~lineout("    Purpose:" message)
  end
  output~lineout("    Failed: " self~assertionMessage)


-- set a secondary assertion on the object
::attribute secondaryException set

-- get an explanation message for a given assertion failure type
::method assertionMessage
  expose assertionType

  return self~messageTest(assertionType)


--  OneValueAssertionFailure is a report of a test case failure.  A test case fails when an
--  assertion about an object does not hold.  This is the base assertion failure.
::class 'OneValueAssertionFailure' public subclass TestFailure
::method init
  expose actual
  use strict arg testMethod, assertionType, message, actual

  self~init:super(testMethod, assertionType, message)

-- display the test failure information
::method displayFailure
  expose actual
  use strict arg output

  -- display the base information
  self~displayFailure:super(output)
  output~lineout("  Actual: " self~formatObject(actual))


--  TwoValueAssertionFailure is a report of a test case failure.  A test case fails when an
--  assertion about an object does not hold.  This is the base assertion failure.
::class 'TwoValueAssertionFailure' public subclass TestFailure
::method init
  expose expected actual
  use strict arg testMethod, assertionType, message, expected, actual

  self~init:super(testMethod, assertionType, message)

-- display the test failure information
::method displayFailure
  expose expected actual
  use strict arg output

  -- display the base information
  self~displayFailure:super(output)
  output~lineout("    Actual:  " self~formatObject(expected))
  output~lineout("    Expected:" self~formatObject(actual))


-- class for an unexpected execution failure
::class 'UnexpectedError' public subclass TestFailure
::method init
  expose errorLine stackFrames conditionObject
  use strict arg testMethod, conditionObject

  self~init:super(testMethod, "Unexpected Error")

  -- fill in the failure information
  errorLine = testMethod~conditionLineNumber(conditionObject)
  stackFrames = testMethod~pruneStackFrames(conditionObject~stackFrames)


-- display the test failure information
::method displayFailure
  expose errorLine stackFrames conditionObject
  use strict arg output

  -- display the base information
  self~displayFailure:super(output)
  output~lineout("  Event:  [SYNTAX" conditionObject~code"] raised unexpectedly")
  output~lineout("    "conditionObject~message)
  output~lineout("    Line:" errorLine)

  -- add the trace back lines, excluding anything from the framework
  loop frame over stackFrames
     output~lineout(frame~string)
  end

-- class for a handling a test failure caused by not receiving an
-- expected error
::class 'ExpectedErrorFailure' public subclass TestFailure
::method init
  expose expectedCondition
  use strict arg testMethod, expectedCondition

  self~init:super(testMethod, "Condition expected")

-- display the test failure information
::method displayFailure
  expose expectedCondition
  use strict arg output

  -- display the base information
  self~displayFailure:super(output)
  expectedCondition~displayFailure(output)


-- a holder for expected syntax information
::class "ExpectedCondition"
::method init
  expose condition code assertionLine message
  use strict arg assertionLine, condition , code = .nil, message = .nil

-- do a check for condition equality
::method isMatch
  expose condition code
  use strict arg received

  -- note:  for non syntax conditions, the code in the condition object
  -- will be .nil, so this will compare equal.
  return condition == received~condition & code == receive~code

-- display information about an expected condition failure
::method displayFailure
  expose assertionLine, condition, code, message
  output~lineout("  Failed:  Condition expected")
  if message != .nil then do
     output~lineout("    Reason:  " message
  end
  output~lineout("    Line:   " assertionLine
  if condition == 'SYNTAX' then do
     output~lineout("    Expected: SYNTAX" code
  end
  else do
     output~lineout("    Expected:" condition
  end


-- this represents a single test method within a TestCase.  This
-- handles the details of running a single test method and recording the
-- results
::class "TestMethod"
-- create an individual test associated with a TestCase
::method init
  expose testCase testName testMethod expectedCondition
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

-- locate the current line number within a test method.  Generally for
-- determining assertion locations
::method testLineNumber
  return self~getLineNumber(.context~stackFrames)

-- Determine the line number within a test method that triggered a condition
::method conditionLineNumber
  use strict arg condition
  return self~getLineNumber(condition~stackFrames)

-- Given a collection of stack frames, return the line number of the line in the
-- test method as a failure point.
::method getLineNumber private
  expose testMethod
  use strict arg frames

  do frame over frames
      if frame~executable == testMethod then do
           return frame~line
      end
  end

  -- not found for some reason
  return -1

-- prune a set of stack frames to remove all stack frames
-- lower than the test method
::method pruneStackFrames
  expose testMethod
  use strict arg frames

  frames = frames~makeArray

  loop i = 1 to frames~last
     -- once we hit our target method, return all of the frames
     -- from the top to the method.
     if frames[i]~executable == testMethod then
        return frames~section(1, i)
     end
  end

  -- should never get here, but one never knows.
  return frames


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
   self~checkForConditionFailure(testResult)
   return testResult

exceptionHandler:

  -- Get the condition object.  (The condition object is a Directory object.)
  conditionObject = condition("O")

  -- If a condition was expected to be raised AND this condition matches the
  -- expected condition, then the test passed so just return.
  if self~checkCondition(conditionObject) then return testResult

  -- If the condition code is 93.964 then this condition was raised by the
  -- framework to signal a test case failure.  The assert failure object is at
  -- index 2 of the additional array.
  if conditionObject~code = .TestCase~assertionCode then do
     -- first get the failure object, then
     failure = conditionObject~additional[2]
     -- attach this to the result for this test
    testResult~addFailure(failure)
  end
  else do
    -- This is an unexpected, unanticipated error.
    -- add an error object to the test result
    err = .UnexpectedError~new(self, conditionObject)
    testResult~addError(err)
  end

  return testResult


-- check if to see if received condition matches a received condition
::method checkCondition
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
::method checkForConditionFailure
  expose expectedCondition
  use strict arg testResult

  -- if we have an expected condition, attach to the result.
  if expectedCondition \= .nil then do
       err = .ExpectedErrorFailure~new(self, expectedCondition)
       testResult~addFailure(err)
  end


-- Create a string representation of this test case
::method string
  expose testname testCase

  buf = .mutableBuffer~new
  buf~append("testCase: [", testName, "] ("testCase~string, ")")
  return buf~string

-- display base test information
::method display
  expose testCase testMethod
  use strict arg output

  -- display the method name, then add the testcase information
  output~lineout("  Test:  " testMethod)
  testCase~display(output)


-- base class for a TestCase class.  This is a class that implements
-- a set of test methods.  This handles invocation of the different
-- test methods and recording of result information from the tests.
::class "TestCase" public
::constant AssertionCode 93.964

::method init
  expose fileName svnInfo

  -- get the containing file name for reporting.
  fileName = self~class~package~name

  -- try to determine if this file is in svn and get the information
  -- if it is
  svnInfo = .svnInfo~new(fileName)

  -- initialize the superclass
  self~init:super

-- display base test information
::method display
  expose fileName svnInfo
  use strict arg output

  output~lineout("  Class: " self~class~id)
  output~lineout("  File:  " fileName)
  if svnInfo~revision \= .nil then do
     output~lineout("  SVN:     r"svnInfo~revision "  Change Date: " svnInfo~date)
  end

-- count a successful assertion
::method assertionPassed
  expose currentTestCase

  forward to(currentTestCase)


-- count a successful assertion where we have nesting.
::method nestedAssertionPassed
  expose mainAssertion

  mainAssertion = .nil
  self~assertionPassed


-- checks that two collections are equilvalent.  Does not take
-- order into account.  Use assertArrayEqual or assertArrayStrictlyEqual
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
      self~twoValueFail("assertEquals", expected, actual, msg)
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
      self~twoValueFail("assertFuzzyEquals", expected, actual, msg)
  end

  -- Assertion holds
  self~assertionPassed


-- assert that a result does not equal a given value
::method assertNotEquals
  use strict arg expected, actual, msg = .nil

  -- If the 2 objects are not equal AND they are not 2 equal collections ...
  if expected != actual then do
    -- ... the assertion holds
    self~assertionPassed
    return
  end

  self~twoValueFail("assertNotEquals", expected, actual, msg)


-- assert that the result is equal to .nil
::method assertNull
  use strict arg actual, msg = .nil

  if .nil \== actual then do
    self~oneValueFail("assertNull", actual, msg)
  end

  -- Assertion holds
  self~assertionPassed


-- assert that a result is not equal to .ni.
::method assertNotNull
  use strict arg actual, msg = ""

  if .nil == actual then do
    self~oneValueFail("assertNotNull", actual, msg)
  end

  -- Assertion holds
  self~assertionPassed


-- verify that a result is an instance of a particular class
::method assertIsA
  use strict arg target, class, msg = .nil

  if \target~isA(class) then do
    self~twoValueFail("assertIsA", target, class, msg)
  end

  -- Assertion holds
  self~assertionPassed


-- compare an array of items.  each item must be the same, and in the same
-- order.  Item comparisons are the non-strict variety
::method assertArrayEquals
  expose currentTestCase
  use strict arg expected, actual, msg = .nil

  -- set the main assertion failure type.  The nested assertion will have the details.
  self~setMainAssertion(.TwoValueAssertionFailure~new(currentTestCase, "assertArrayEquals", msg, expected, actual))

  self~assertIsA(actual, .array, "Actual is not an array item")
  self~assertEquals(expected~items, actual~items, "The item count is different between the two arrays")
  self~assertEquals(expected~last, actual~last, "The arrays have different index structures")

  loop i = 1 to expected~last
      self~assertEquals(expected[i], actual[i], "The arrays do not match at index" i)
  end

  -- Assertion holds
  self~nestedAssertionPassed


-- compare an array of items.  each item must be the same, and in the same
-- order.  Item comparisons are the strict variety
::method assertArrayStrictlyEquals
  expose currentTestCase
  use strict arg expected, actual, msg = .nil

  -- set the main assertion failure type.  The nested assertion will have the details.
  self~setMainAssertion(.TwoValueAssertionFailure~new(currentTestCase, "assertArrayStrictlyEquals", msg, expected, actual))

  self~assertIsA(actual, .array, "Actual is not an array item")
  self~assertEquals(expected~items, actual~items, "The item count is different between the two arrays")
  self~assertEquals(expected~last, actual~last, "The arrays have different index structures")

  loop i = 1 to expected~last
      self~assertStrictlyEqual(expected[i], actual[i], "The arrays do not match at index" i)
  end

  -- Assertion holds
  self~nestedAssertionPassed


-- create a routine from an array of lines with the intent of checking
-- errors raised at translate time rather than runtime.
::method assertSyntaxError
  use strict arg error, code, msg = .nil

  self~expectSyntax(error, msg)
  -- this will translate this code and nothing else.
  r = .package~new("test", code)


-- perform a strict comparison on two objects.
::method assertStrictlyEquals
  use strict arg expected, actual, msg = .nil

  if expected \== actual then do
      self~twoValueFail("assertStrictlyEquals", expected, actual, msg)
  end

  -- Assertion holds
  self~assertionPassed


-- do an assertion of non-equality
::method assertNotStrictlyEqual
  use strict arg expected, actual, msg = .nil

  if (expected == actual) then do
      self~twoValueFail("assertStrictlyNotEquals", expected, actual, msg)
  end

  -- Assertion holds
  self~assertionPassed


-- Check if an object is EXACTLY the same object instance
::method assertIdentical
  use strict arg expected, actual, msg = .nil

  if (expected~identityHash != actual~identityHash) then do
      self~twoValueFail("assertIdentical", expected, actual, msg)
  end

  -- Assertion holds
  self~assertionPassed


-- Check if an object is NOT EXACTLY the same object instance
::method assertNotIdentical
  use strict arg expected, actual, msg = .nil

  if (expected~identityHash == actual~identityHash) then do
      self~twoValueFail("assertNotIdentical", expected, actual, msg)
  end

  -- Assertion holds
  self~assertionPassed

-- assert that an object is one of a given list
::method assertOneOf
  use strict arg choices, actual, msg = .nil

  if \choices~hasItem(actual) then
     self~oneValueFail("assertOneOf", actual, msg)
  end

  -- Assertion holds
  self~assertionPassed


-- assert that a condition is true
::method assertTrue
  use strict arg actual, msg = ""

  if .true \== actual then do
     self~oneValueFail("assertTrue", actual, msg)
  end

  -- Assertion holds
  self~assertionPassed


-- assert that a condition value is false
::method assertFalse
  use strict arg actual, msg = ""

  if .false \== actual then do
     self~oneValueFail("assertFalse", actual, msg)
  end

  -- Assertion holds
  self~assertionPassed

-- create a package from an array of lines with the intent of checking
-- errors raised at translate time rather than runtime.
::method assertSyntaxError
  use strict arg error, code, msg = .nil

  self~expectSyntax(error, msg)
  -- this will translate this code and nothing else.
  r = .package~new("test", code)


-- create a routine and call from an array of lines with the intent of checking
-- errors raised at run time rather than during translation.
::method assertRuntimeError
  use strict arg error, code, msg = .nil

  self~expectSyntax(error, msg)
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
  expose currentTestCase
  use strict arg msg = .nil
  self~fail(.AssertionFailure~new(currentTestCase, "assertFail", msg)


-- create an error for a two-value assertion failure.
::method twoValueFail
  expose currentTestCase
  use strict arg type, expected, actual, msg

  failure = .TwoValueAssertionFailure~new(currentTestCase, type, msg, expected, actual)
  self~fail(failure)


-- create an error for a two-value assertion failure.
::method oneValueFail
  expose currentTestCase
  use strict arg type, actual, msg

  failure = .OneValueAssertionFailure~new(currentTestCase, type, msg, actual)
  self~fail(failure)


-- indicate we have an assertion that nests with other assertions
::method setMainAssertion
  expose mainAssertion
  use strict arg mainAssertion


-- raise a test failure using the the given failure reason.  This will raise a syntax
-- error that will be trapped by the test runner to record the data
::method fail
  expose mainAssertion
  use strict arg failure

  -- if this is a nested assertion situation, the main assertion is
  -- the error, while the assertion that raises the failure just provides
  -- additional information.
  if mainAssertion \= .nil then do
     mainAssertion~secondaryAssertion = failure
     failure = mainAssertion
     mainAssertion = .nil
  end

  -- the failure object will be the
  RAISE syntax (self~assertionCode) array ("Test assertion failure", failure)

-- access to the current running test method state
::attribute currentTestCase

-- clear any expected condition information
::method clearCondition
  expose currentTestCase
  forward to(currentTestCase)

-- indicate that the test is expecting a particular syntax error to be raised.
::method expectSyntax
  expose currentTestCase
  -- the details are handled by the current testcase under execution
  forward to(currentTestCase)


-- indicate that a particular condition type is expected.
::method expectCondition
  expose currentTestCase
  -- the details are handled by the current testcase under execution
  forward to(currentTestCase)


-- check if to see if received condition matches a received condition
::method checkCondition
  expose currentTestCase




-- execute all of the methods in a test case class, returning the accumulated
-- statistics
::method execute
  testMethods = self~getTestMethods

  -- create a result object for this test case class.
  testResult = .TestCaseResult~new(self)

  -- for each method create a runner for this and execute the test
  loop methodName overMethods
     self~currentTestCase = new .TestMethod(self, methodName)
     -- run the test method, then add the results to our accumulator.
     methodResult = self~currentTestCase~execute
     testResult~addResult(methodResult)
  end

  return testResult

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

-- test suite is a collection of test case files.  This will
--run all of the indicated tests and return a a consoladated
-- result object.
::class "TestSuite" subclass
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

-- read all of the lines of a file
::method arrayIn
  expose file

  s = .stream~new(file)
  a = s~arrayIn
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

-- class for holding svn info for a file
::class "SVNInfo"
::method init
  expose revision date
  use strict arg filename author

  revision = .nil
  date = .nil
  author = .nil

  file = .TemporaryTestFile~new(self, "svninfo.dat")

  -- turn off tracing so command errors don't trace
  trace 'Off'

  'svn --xml info "'filename'" 1>svninfo.dat 2>&1'
  if rc == 0 then do
     loop line over file~arrayIn
        if line~contains("revision=") then do
           parse var line 'revision="' revision '"'
        end
        else if line~contains("<date>") then do
           parse var line ">" date "<"
        end
        else if line~contains("<author>") then do
           parse var line ">" author "<"
        end
     end
  end

  file~delete

::attribute revision get
::attribute date get
::attribute author get

