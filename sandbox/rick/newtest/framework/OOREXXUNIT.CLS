#!/usr/bin/rexx
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2005-2014 Rexx Language Association. All rights reserved.    */
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
  interpreterLanguageLevel = .rexxinfo~languageLevel

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
   if \OSName~caselessEquals("Windows") then do
      -- get the fully qualified shell name from the environment
      unixShell=value("SHELL", , "environment")
      -- and get the shell name portion
      shell = .File~new(unixShell)~name
   end
   return shell

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
     start=str~verify(nonPrintableChars, "Match")
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


-- Base class for controlling how tests run.  This implements all
-- methods for filtering tests, logging progress, controlling where
-- output goes, etc.  When running collections of tests, there is
-- generally one controller object that is used for each of the
-- test objects.  This one is used by default and implements just
-- default behaviours.
::class "TestController" mixinclass object
-- the default output streams
::method setup

-- where results get written
::method resultStream
  -- default is to .error so this can be separated from
  -- say output on redirect.
  return .error

-- filter a particular testcase method
::method filterTestMethod
  use strict arg testCase, methodName, testMethod
  -- this version always selects
  return .true

-- filter an entire testCase or testsuite class
::method filterTests
  use strict arg test
  return .true

-- logging methods.  These are NOPS in the default implementation
::method logFailure
  use strict arg testResult

::method logError
  use strict arg testResult

::method logTestStart
  use strict arg testMethod

::method logTestEnd
  use strict arg testMethod

::method logTestCaseStart
  use strict arg testCase

::method logTestCaseEnd
  use strict arg testCase

::method logTestSuiteStart
  use strict arg testSuite

::method logTestSuiteEnd
  use strict arg testSuite

-- handle display feed back.  This is a nop by default
::method feedBack
  use strict arg message


-- Record the result from an individual test method.  This will hold all of the
-- error/failure information, plus a count of the successful assertions.
::class "TestResultBase" public
::method init
  expose errorInformation assertionCount isError isFailure skippedTests isSkipped

  assertionCount = 0
  errorInformation = .nil
  isError = .false
  isFailure = .false
  isSkipped = .false

-- add an error to the result
::method setError
  expose errorInformation isError
  use strict arg errorInformation

  isError = .true

-- add a failure to the result
::method setFailure
  expose errorInformation isFailure
  use strict arg errorInformation

  isFailure = .true

::attribute assertionCount GET

-- returns whether the entire test was successful or not
::method wasSuccessful
  expose errorInformation
  return errorInformation == .nil

::attribute isFailure get
::attribute isError get
::attribute isSkipped

-- get the error information
::method failureResult
  expose errorInformation
  return errorInformation

-- display any failure information
::method displayFailure
  expose errorInformation
  use strict arg output

  if errorInformation \= .nil then
     errorInformation~display(output)


-- Record the result from an individual test method.  This will hold all of the
-- error/failure information, plus a count of the successful assertions.
::class "TestMethodResult" public subclass TestResultBase
::method init
  expose testMethod
  use strict arg testMethod

  self~init:super

-- add an error to the result
::method setError
  forward class(super) continue
  testMethod~logError(self)

-- add a failure to the result
::method setFailure
  forward class(super) continue
  testMethod~logFailure(self)

-- Record test completion
::method endTest
  expose testMethod

  -- and add an end event to our test log
  testMethod~logEndTest(self)

::method startTest
  expose testMethod

  testMethod~logTestStart(self)

-- special error for failures instantiating a test class
::class TestInstantiationError subclass TestResultBase
::method init
  use strict arg testClass, conditionObj

  self~init:super

  self~setError(.GeneralError~new("Error instantiating test class" testClass~id, conditionObj))

-- special error for failures instantiating a test class
::class FileNotFoundFailure subclass TestResultBase
::method init
  use strict arg testSuite, fileName

  self~init:super

  self~setFailure(.GeneralError~new("Error loading file" fileName "for test suite" testSuite~string))


-- special result for skipped test cases
::class "SkippedTestMethod" subclass TestResultBase
::method init
  use strict arg testMethod

  self~init:super

  self~skippedTests = 1
  self~isSkipped = .true

-- special result for skipped test cases
::class "SkippedTestSets" subclass TestResultBase
::method init
  use strict arg testClass

  self~skippedTestSets = 1
  self~isSkipped = .true

-- an accumulated set of test results
::class "ResultSet"
::method init
  expose tests skippedTests skippedTestSets assertions failures errors failureResult testEntity
  use strict arg testEntity

  tests = 0
  skippedTests = 0
  skippedTestSets = 0
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

  -- if the other is a result set, merge everything.
  if otherSet~isA(.ResultSet) then do
     tests += otherSet~tests
     skippedTests += otherSet~skippedTests
     skippedTestSets += otherSet~skippedTestSets
     assertions += otherSet~assertions
     errors += otherSet~errors
     failures += otherSet~failures
     failureResults~appendAll(otherSet~failureResults)
  end
  -- received a single result...likely a failure
  else do
      -- if this was a skipped test, just record that
      if otherSet~isSkipped then do
         skippTests += 1
      end
      else do
         -- always add in the assertions
         assertions += otherSet~assertions
         if otherSet~isFailure then do
            failures += 1
            failureResults~append(otherSet~failureResult)
         end
         else if otherSet~isFailure then do
            errors += 1
            failureResults~append(otherSet~failureResult)
         end
      end
  end

-- display this exception status
::method display
  expose tests assertions failures errors failureResults testEntity startTime endTime
  use strict arg output = .output

  output~lineout("Test Name       "  testEntity~displayName)
  output~lineout("Interpreter:    " .rexxinfo~name)
  output~lineout("Addressing Mode:" .rexxInfo~architecture)
  output~lineout("ooRexxUnit:     " .ooRexxUnit~version)
  output~lineout
  output~lineout("Tests run:         " tests)
  output~lineout("Tests skipped:     " skippedTests)
  output~lineout("Test sets skipped: " skippedTestSets)
  output~lineout("Assertions:        " assertions)
  output~lineout("Failures:          " failures)
  output~lineout("Errors:            " errors)

  -- display any failure information that is part of this set.
  self~displayFailure(output)

  output~lineout
  output~lineout("Execution time:    " endTime - startTime)

-- display just the test failure information.  If executing a
-- test suite, we'll display this early to free up the resources
-- and just merge the statistic information.
::method displayFailure
  expose failureResults
  use strict arg output = .output

  if \failureResults~isEmpty then do
     output~lineout
     loop result over failureResults
        output~lineout
        result~displayFailure(output)
     end
  end

  -- release the failure result information once displayed,
  -- which will free the test details for garbage collection
  failureResults~empty


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
::class 'TestFailureBase' public
::method activate class
  expose messages
  messages = .stringtable~new

  loop line over .context~package~resource('TestProblemMessages')
     parse var line id text
     messages[id] = text
  end

-- initialize a test failure.  At this level, we are given the
-- testMethod object, which contains all of the relevant test information,
-- and type type of failure
::method init
  expose failureType timestamp
  use strict arg failureType

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
  expose failureType timestamp
  use strict arg output

  output~lineout('['failureType'] ['timestamp~isodate']')

-- The base class for reporting test problems
::class 'TestFailure' public subclass TestFailureBase
-- initialize a test failure.  At this level, we are given the
-- testMethod object, which contains all of the relevant test information,
-- and type type of failure
::method init
  expose testMethod
  use strict arg testMethod, failureType
  self~init:super(failureType)

  timestamp = .datetime~new

-- display the test failure information
::method displayFailure
  expose testMethod
  use strict arg output

  self~displayFailure:super(output)
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
  if message \= .nil then do
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

  if message \= .nil then do
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


-- class for an unexpected execution failure
::class 'GeneralError' public subclass TestFailureBase
::method init
  expose message conditionObject
  use strict arg message, conditionObject

  self~init:super("Test Error")

-- display the test failure information
::method displayFailure
  expose message conditionObject
  use strict arg output

  -- display the base information
  self~displayFailure:super(output)
  output~lineout(message)

  -- add the trace back lines, excluding anything from the framework
  loop frame over conditionObject~stackFrames
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
  expose assertionLine condition code message
  use strict arg output = .output

  output~lineout("  Failed:  Condition expected")
  if message != .nil then do
     output~lineout("    Reason:  " message)
  end
  output~lineout("    Line:   " assertionLine)
  if condition == 'SYNTAX' then do
     output~lineout("    Expected: SYNTAX" code)
  end
  else do
     output~lineout("    Expected:" condition)
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

-- logging methods for test events.  These log to the parent entity
::method logFailure delegate testCase
::method logError delegate testCase
::method logTestStart delegate testCase
::method logTestEnd delegate testCase

-- mark this test method as expecting a syntax error before completion
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

  -- should never get here, but one never knows.
  return frames


-- execute a specific test case method
::method execute
  expose testCase testResult

  -- create the test result
  testResult = .TestMethodResult~new(self)

  -- record the test
  testResult~startTest(self)
  -- log the start event
  testCase~logTestStart(self)

  testCase~feedBack( "... running TestCase object ["self~string"] ..." )

  -- setup for the test, creating a clean environment
  testCase~performTestSetup

  self~doTheTest(aTestResult)  -- carry out the testmethod

  -- cleanup after  the test, creating a clean environment
  testCase~peformTestSetup

  testResult~endTest(self)           -- remember test ended
  -- log the end event
  testCase~logTestEnd(self)
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
  expose expectedCondition
  use arg receivedCondition

  -- if we're not expecting a condition, this is a failure
  if expectedCondition == .nil then return .false

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


-- a mixin class to add the test execution infrastructure to a test case type
::class "TestExecutor" mixinclass object
-- run the tests and display the results
::method run class
  use strict arg controller = (.TestController~new)
  testResult = self~execute(controller)
  testResult~display(controller~resultStream)


-- execute an this test case by creating an instance and having it execute.
::method execute class
  use strict arg controller = (.TestController~new)

  testResult = .ResultSet~new(self)
  instance = self~createTestInstance(controller, testResult)
  if instance == .nil then return testResult

  -- execute the tests, returning a composite result
  return instance~execute


-- execute this test in the confines of a test suite.
::method executeInSuite class
  use strict arg testSuite

  -- if this passes filtration criteria, then execute it
  if testSuite~filterTests(self) then do
     return self~execute(testSuite)
  end
  -- return a test result that indicates we were skipped
  else return .SkippedTestSet~new(self)


-- create an instance of a test case class, with syntax error protection
::method createTestInstance class
  use strict arg controller, testResult

--  signal on syntax
  return self~new(controller)

syntax:
  testResult~addError(.TestInstantiationError~new(testClass, condition('o')))
  return .nil


-- base class for a TestCase class.  This is a class that implements
-- a set of test methods.  This handles invocation of the different
-- test methods and recording of result information from the tests.
::class "TestCase" public inherit TestExecutor
::constant AssertionCode 93.964

-- The test controller is used for logging, filtering, output redirection, etc.
::method init
  expose controller fileName svnInfo
  use strict arg controller = (.TestController~new)

  -- get the containing file name for reporting.
  fileName = self~class~package~name

  -- try to determine if this file is in svn and get the information
  -- if it is
  svnInfo = .svnInfo~new(fileName)

-- logging methods.  These just forward to the controller object
::method logFailure delegate controller
::method logError delegate controller
::method logTestStart delegate controller
::method logTestEnd delegate controller
::method logTestCaseStart delegate controller
::method logTestCaseEnd delegate controller
-- handle display feed back...also handled by the controller
::method feedBack delegate controller

-- display base test information
::method display
  expose fileName svnInfo
  use strict arg output = .output

  output~lineout("  Class: " self~class~id)
  output~lineout("  File:  " fileName)
  if svnInfo~revision \= .nil then do
     output~lineout("  SVN:     r"svnInfo~revision "  Change Date: " svnInfo~date)
  end

-- count a successful assertion
::method assertionPassed delegate currentTestMethod

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
  expose currentTestMethod
  use strict arg expected, actual, msg = .nil

  -- set the main assertion failure type.  The nested assertion will have the details.
  self~setMainAssertion(.TwoValueAssertionFailure~new(currentTestMethod, "assertArrayEquals", msg, expected, actual))

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
  expose currentTestMethod
  use strict arg expected, actual, msg = .nil

  -- set the main assertion failure type.  The nested assertion will have the details.
  self~setMainAssertion(.TwoValueAssertionFailure~new(currentTestMethod, "assertArrayStrictlyEquals", msg, expected, actual))

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

  -- Assertion holds
  self~assertionPassed


-- assert that a condition is true
::method assertTrue
  use strict arg actual, msg = ""

  if .true \== actual then d
     self~oneValueFail("assertTrue", actual, msg)

  -- Assertion holds
  self~assertionPassed


-- assert that a condition value is false
::method assertFalse
  use strict arg actual, msg = ""

  if .false \== actual then
     self~oneValueFail("assertFalse", actual, msg)

  -- Assertion holds
  self~assertionPassed

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
  expose currentTestMethod
  use strict arg msg = .nil
  self~fail(.AssertionFailure~new(currentTestMethod, "assertFail", msg))


-- create an error for a two-value assertion failure.
::method twoValueFail
  expose currentTestMethod
  use strict arg type, expected, actual, msg

  failure = .TwoValueAssertionFailure~new(currentTestMethod, type, msg, expected, actual)
  self~fail(failure)


-- create an error for a two-value assertion failure.
::method oneValueFail
  expose currentTestMethod
  use strict arg type, actual, msg

  failure = .OneValueAssertionFailure~new(currentTestMethod, type, msg, actual)
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
::attribute currentTestMethod

-- clear any expected condition information
::method clearCondition delegate currentTestMethod

-- indicate that the test is expecting a particular syntax error to be raised.
::method expectSyntax delegate currentTestMethod

-- indicate that a particular condition type is expected.
::method expectCondition delegate currentTestMethod

-- execute this test in the confines of
::method executeInSuite
  expose controller
  -- the test suite functions as a controller (which might be running
  -- under a different controller.  This replaces any controller we
  -- might have been instantiated with.
  use strict arg controller

  -- if this passes filtration criteria, then execute it
  if controller~filterTests(self~class) then do
     return self~execute
  end
  -- return a test result that indicates we were skipped
  else .SkippedTests~new(self)


-- execute all of the methods in a test case class, returning the accumulated
-- statistics
::method execute
  expose controller
  testMethods = self~getTestMethods

  -- create a result object for this test case class.
  testResult = .TestMethodResult~new(self)

  -- for each method create a runner for this and execute the test
  loop methodName over methods
     method = self~instanceMethod(methodName)
     self~currentTestMethod = .TestMethod~new(self, methodName, method)
     if controller~filterTestMethod(self, methodName, method) then do
         -- run the test method, then add the results to our accumulator.
         methodResult = self~currentTestMethod~execute
         testResult~addResult(methodResult)
     end
     else do
         testResult~addResult(.SkippedTestMethod(self~currentTestMethod))
     end

  end

  return testResult

-- locate the test method names for this test case class.  Returns
-- an array of executable testMethod objects.
::method getTestMethods
  expose controller

  -- Get the test methods names.  By convention this is all methods starting
  -- with "TEST".  We use a set because there may be duplicates in the
  -- class hierarchy
  testMethodNames = .set~new

 -- Get all methods that start with "TEST", even ones from the superclasses
 -- Some tests might have method overrides that test different variants of
 -- the base test methods
  methSupplier = self~class~methods
  do while methSupplier~available -- iterate over supplied methods
     name = methSupplier~index
     if name~match(1, "TEST") then do
        testMethodNames~put(name)
     end
     methSupplier~next
  end

  -- return the method names in sorted order.
  return testMethodNames~makearray~sort

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
  self~setUp

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
  self~tearDown


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
::class "TestSuite" inherit TestExecutor
::method init
  expose testQueue

  -- this will be our set of tests to run.
  testQueue = .queue~new
  -- tests are frequently listed in a resource within the package,
  -- check to see if we have such a list
  self~checkResourceList

-- check this class for an annotation that identifies a resource
-- item in the same package that contains a list of files included
-- in this test suite.
::method checkResourceList
  expose testQueue
  fileResource = self~class~annotation('TESTFILES')

  if fileResource == .nil then return

  fileList = self~class~package~resource('TESTFILES')
  if fileList == .nil then return

  -- add each file to the test queue
  loop file over fileList
     testQueue~append(.TestFileRunner~new(self, file))
  end


-- add a test to the queue of individual test cases to run
-- These can be added in a number of forms:
-- 1)  An instance of a TestCase
-- 2)  An instance of a TestSuite
-- 3)  A TestCase class
-- 4)  A TestSuite class
-- 5)  A package object that might contain one or more definition of either
-- 6)  A string file name of a test group
::method addTest
  expose testQueue
  use strict arg newTestCase

  -- the classes and instances are polymorphic with the execute method, so
  -- we can just add these directly to the queue.
  if newTestCase~isA(.String) then do
     testQueue~append(.TestFileRunner~new(self, newTestCase))
  end
  else if newTestCase~isA(.TestCase) then do
     testQueue~append(newTestCase)
  end
  else if newTestCase~isA(.TestSuite) then do
     testQueue~append(newTestCase)
  end
  else if newTestCase~isA(.Class), newTestCase~isSubclassOf(.TestCase) then do
     testQueue~append(newTestCase)
  end
  else if newTestCase~isA(.Class), newTestCase~isSubclassOf(.TestSuite) then do
     testQueue~append(newTestCase)
  end
  else if newTestCase~isA(.Package) then do
     testQueue~append(.TestPackageRunner~new(self, newTestCase))
  end


-- execute this test in the confines of
::method executeInSuite
  expose controller
  -- the test suite functions as a controller (which might be running
  -- under a different controller.  This replaces any controller we
  -- might have been instantiated with.
  use strict arg controller

  -- if this passes filtration criteria, then execute it
  if controller~filterTests(self~class) then do
     return self~execute
  end
  -- return a test result that indicates we were skipped
  else .SkippedTests~new(self)


-- execute the test suite instance.  This will execute all of the contained tests.
::method execute
  expose testQueue

  testResult = .ResultSet~new

  testResult~startTest(self)        -- remember test started
  self~setUp                        -- make sure setup is invoked before testSuite runs

  -- execute the tests and merge the result information
  loop while \testQueue~isEmpty
     test = testQueue~pull
     testResult~merge(test~executeinSuite(self))
  end

  self~tearDown                     -- make sure tearDown is invoked after testSuite ran
  testResult~endTest(self)          -- remember test ended

  return testResult


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


-- a class to manage delayed loading of a test file and
-- running of the test cases.  This will not attempt to load
-- and inspect the file until it attempts to run it to minimize
-- the storage impact of running a Suite
::class "TestFileRunner"
-- initialize the suite using a target file name.  Note that the
-- filename has already been fully resolved by the testSuite.
::method init
  expose testSuite fileName
  use strict arg testSuite, fileName


-- execute this test in the confines of
::method executeInSuite class
  expose testSuite
  -- the test suite functions as a controller (which might be running
  -- under a different controller.  This replaces any controller we
  -- might have been instantiated with.
  use strict arg testSuite

  testResult = .ResultSet~new

  -- try to load the file...the error information is already
  -- set in the result.
  package = self~loadFile(testResult)
  if package == .nil then return testResult

  -- create package runner and execute this
  runner = .TestPackageRunner(testSuite, package)
  return runner~executeInSuite(testSuite)


-- load a TestSuite file to the set.
::method loadFile
  expose fileName
  use strict arg testResult

  -- if we can't resolve this file, just add an error to the result
  fullName = testSuite~resolveTestProgram(fileName)
  if fullName == .nil then do
     testResult~addError(.FileNotFoundFailure(self~testSuite, fileName))
     return .nil
  end

   -- now create a routine from this file
   signal on syntax
   r = .routine~newfile(fullName)

   -- and return the package for this routine so we can locate the test files.
   return r~package

-- some sort of syntax error creating the file
syntax:
   -- add an error and return .nil to indicate a failure
   testResult~addError(.FileLoadFailure(self~testSuite, fullName, condition('o')))
   return .nil


-- a class to manage execution of test files from a package
::class "TestPackageRunner"
-- initialize the suite using a target file name.  Note that the
-- filename has already been fully resolved by the testSuite.
::method init
  expose testSuite package
  use strict arg testSuite, package


-- execute this test in the confines of
::method executeInSuite class
  expose testSuite
  -- the test suite functions as a controller (which might be running
  -- under a different controller.  This replaces any controller we
  -- might have been instantiated with.
  use strict arg testSuite

  testResult = .ResultSet~new

  tests = self~locateTestCases(package)

  -- now execute each of the tests
  loop test over tests
     testResults~merge(test~executeInSuite(testSuite))
  end


-- locate any test cases in a test suite file package
::method locateTestCases
  expose testSuite
  use strict arg package

  testClasses = .array~new

  -- locate the classes that are executable tests.  Other classes
  -- are likely just part of the tests.
  loop testClass over package~classes
     if testClass~isSubclassOf(.TestCase) | testClass~isSubClassOf(.TestSuite) then do
       testClasses~append(testClass)
     end
  end

  return testCases


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
  return a

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
  expose revision date authore
  use strict arg filename

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

