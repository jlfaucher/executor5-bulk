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

::options novalue error

-- main control class the tests.  Holds a lot of global test values rather than
-- storing them in .local
::class "ooRexxUnit"
-- main setup method for the unit test framework
::method activate class
  expose interpreterName interpreterDate interpreterLanguageLevel osname shellname frameworkDirectory originalPath architecture isUnix

  interpreterName = .rexxinfo~name
  interpreterDate = .rexxinfo~date
  interpreterLanguageLevel = .rexxinfo~languageLevel

  -- Note:  initialize the OSName first, other methods depend on this
  OSName = .rexxinfo~platform
  isUnix = .true
  -- make the Windows one a little more generic
  if OSName == "WindowsNT" then do
     OSName = "Windows"
     isUnix = .false
  end

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
-- generally one container object that is used for each of the
-- test objects.  This one is used by default and implements just
-- default behaviours.
::class "TestContainer" mixinclass object
::method init
  expose testTypes resultStream verbose ticker result

  -- by default, we allow everything
  testTypes = .nil
  resultStream = .error
  verbose = .false
  ticker = .nil
  result = .resultset~new

-- start the ticker running
::method startTicker
  expose ticker

  -- don't start this more than once
  if ticker \= .nil then return
  ticker = .ProgressTicker~new

-- stop any running ticker
::method stopTicker
  expose ticker

  -- don't start this more than once
  if ticker == .nil then return

  -- turn off the ticker and release it
  ticker~stop = .true
  ticker = .nil


-- set the container verbosity
::attribute verbose

-- return the default result stream for the container
::attribute resultStream get

-- the result object for the test run
::attribute result get


-- filter a particular testcase method
::method filterTestMethod
  expose testTypes
  use strict arg testCase, methodName, testMethod

  -- a .nil value indicates we accept everything
  if testTypes = .nil then return .true

  -- for now, just filter based on the types
  return testMethod~hasTestType(testTypes)


-- filter an entire testCase or testsuite class
::method filterTests
  expose testTypes
  use strict arg test

  -- a .nil value indicates we accept everything
  if testTypes = .nil then return .true

  return test~hasTestType(testTypes)

-- logging methods.  These are NOPS in the default implementation
::method logFailure
  use strict arg testResult

::method logError
  use strict arg testResult

::method logTestMethodStart
  expose verbose
  use strict arg testMethod

::method logTestMethodEnd
  use strict arg testMethod

::method logTestCaseStart
  expose verbose ticker
  use strict arg testCase

  -- if asking for verbosity, give the indicator
  if verbose then say "Executing TestCase" testCase~string "with" testCase~testCount "tests"
  -- if we have a ticker running, nudge it
  if ticker \= .nil then ticker~tick


::method logTestCaseEnd
  use strict arg testCase

::method logTestGroupStart
  expose verbose
  use strict arg testGroup

  if verbose then say "Executing TestGroup" testGroup~string "with" testGroup~testCount "test sets"

::method logTestGroupEnd
  use strict arg testGroup

-- handle display feed back.  This is a nop by default
::method feedBack
  expose verbose
  use strict arg message

  if verbose then say message


-- create the default result set for a test group instance
::method createResultSet
  use strict arg group
  return .ResultSet~new(group)

-- notification messages added to a result set
::class 'NotificationTypes' public mixinclass Object

  ::constant MIN_TYPE    1

  ::constant SKIP_TYPE   1
  ::constant WARN_TYPE   2
  ::constant TEXT_TYPE   3
  ::constant STEP_TYPE   4
  ::constant STATS_TYPE  5
  ::constant LOG_TYPE    6

  ::constant MAX_TYPE    6

/* Notes on LOG_TYPE notification

     notification~reason == return code
     notification~additional == command line
     notification~message == some message
     notification~additionObject == .array of lines of captured output
*/

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

    self~originatorsID = .nil

-- End of class: Notification


::class 'ExceptionTypes' public mixinclass Object

  ::constant TRAP        1
  ::constant ANOMLY      2
  ::constant UNEXPECTED  3
  ::constant EXTERNAL    4

/* class: ExceptionData- - - - - - - - - - - - - - - - - - - - - - - - - - - -*\

    A data object containing information concerning an unrecoverable error that
    occurred during the execution of an automated group of tests.

    Errors that occur during the invocation a test case method are trapped by
    the exception handle.  However, it is also possible for errors to occur
    during other phases of an an automated test.  For example, errors that
    happen during some set up prior to actually invoking a test case method.

\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::class 'ExceptionData' public subclass TestProblem inherit ExceptionTypes

  ::attribute severity
  ::attribute msg

  ::attribute typeName get
  ::attribute typeName set private

  ::method init
    forward class (super) continue

    self~severity = "Fatal"
    self~msg = ""

    select
      when self~type == self~TRAP then self~typeName = "Trap"
      when self~type == self~ANOMLY then self~typeName = "Anomly"
      when self~type == self~UNEXPECTED then self~typeName = "Unexpected Error"
      when self~type == self~EXTERNAL then self~typeName = "External Command Failure"
      otherwise self~typeName = "Unexpected Error"
    end
    -- End select

  ::method getMessage

    if self~msg \== "" then return self~msg
    else return "(none)"

  /** print()
   * Prints to the console the information this object contains.
   *
   * @param  title    A name / title for the data print out
   * @parar  compact  If true compact the file path name(s).
   */
  ::method print
    use strict arg title = "Framework exception", compact = .true,  -
                   verbose = (.NoiseAdjustable~DEFAULT_VERBOSITY)

    say "["title"]" self~when
    say "  Type:" self~typeName "Severity:" self~severity

    if compact then say "  File:" pathCompact(self~where, 70)
    else say "  File:" self~where

    if self~line <> -1 then say "  Line:" self~line
    if self~msg \== "" then say " " self~getMessage

    if self~type == self~EXTERNAL then do
      self~printExternalException(compact, verbose)
      say
      return
    end

    if self~conditionObject <> .nil then do
      self~printConditionInfo(compact)
      say
      return
    end

    if self~additional~isA(.string) then say " " self~additional
    say

  ::method printConditionInfo private
    use strict arg compact

    obj = self~conditionObject

    sameFiles = (self~where~caselessCompare(obj~program) == 0 & self~line == obj~position)

    say "  Condition:" obj~condition

    if obj~condition == "SYNTAX" then do
      say "   " obj~message
      if \ sameFiles then do
        if compact then say "    File:" pathCompact(obj~program, 70)
        else say "    File:" obj~program
        say "    Line:" obj~position
      end
    end
    else do
      say "   " obj~description
    end

    if obj~traceBack~isA(.list) then do line over obj~traceBack
      say " " line
    end

  ::method printExternalException private
    use strict arg compact, verbose

    n = self~additionalObject

    say " " n~message
    say "  Command line:" n~additional
    say "  Return code: " n~reason

    if n~where \== "" then do
      l = "  Location:"
      if compact then say l pathCompact(n~where, 70)
      else say l n~where
    end

    if verbose >= 5, n~additionalObject \== .nil then do
      log = n~additionalObject
      say
      do line over log
        say line
      end
    end

-- End of class: ExceptionData

-- an accumulated set of test results
::class "ResultSet"
::method init
  expose tests skippedTests skippedTestSets assertions failures errors failureResults testEntity knownFailures
  use strict arg testEntity

  tests = 0
  skippedTests = 0
  skippedTestSets = 0
  assertions = 0
  failures = 0
  errors = 0
  knownFailures = 0
  failureResults = .array~new

  self~notifications = .array~new
  self~exceptions = .array~new
  self~events = .array~new
  self~knownFailures = .array~new

::attribute tests get
::attribute assersions get
::attribute failures get
::attribute errors get
::attribute failureResults get

::attribute notifications  private
::attribute exceptions     private
::attribute events         private
::attribute knownFailures  private
::attribute newFailures    private

::attribute newFailureCount   set private
::attribute newFailureCount   get
::attribute knownFailureCount set private
::attribute knownFailureCount get


-- start the execution of a test group
::method startTestGroup
  expose startTime
  startTime = .DateTime~new


-- start the execution of a test group
::method endTestGroup
  expose endTime
  endTime = .DateTime~new


-- mark the start of a set of tests
::method testStart
  expose starttests
  -- record that we're executing an addition al test case
  tests += 1

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
     errors += 1
     failureResults~append(result)
  end
  -- could be a known failure item.  We don't count this as a failure,
  -- but will display the failure
  else if result~isKnownFailure then do
     knownFailures += 1
     failureResults~append(result)
  end

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
  expose tests assertions failures errors failureResults testEntity startTime endTime skippedTests skippedTestSets
  use strict arg output = .output

  output~lineout("Test Class      "  testEntity~displayName)
  output~lineout("Test Package    "  testEntity~testPackage)
  output~lineout("Interpreter:    " .rexxinfo~name)
  output~lineout("Addressing Mode:" .rexxInfo~architecture)
  output~lineout("ooRexxUnit:     " .ooRexxUnit~version)
  output~lineout("")
  output~lineout("Tests run:         " tests)
  output~lineout("Tests skipped:     " skippedTests)
  output~lineout("Test sets skipped: " skippedTestSets)
  output~lineout("Assertions:        " assertions)
  output~lineout("Failures:          " failures)
  output~lineout("Errors:            " errors)

  -- display any failure information that is part of this set.
  self~displayFailure(output)

  output~lineout("")
  output~lineout("Execution time:    " endTime - startTime)

-- display just the test failure information.  If executing a
-- test suite, we'll display this early to free up the resources
-- and just merge the statistic information.
::method displayFailure
  expose failureResults
  use strict arg output = .output

  if \failureResults~isEmpty then do
     output~lineout("")
     loop testResult over failureResults
        output~lineout("")
        testResult~displayFailure(output)
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
assertArraysEqual The actual array must match the expected array in all details.
assertArraysStrictlyEqual The actual array must match the expected array in all details.
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
     messages~setEntry(id, text)
  end

-- initialize a test failure.  At this level, we are given the
-- testMethod object, which contains all of the relevant test information,
-- and type type of failure
::method init
  expose failureType timestamp
  use strict arg failureType

  timestamp = .datetime~new


-- format an object for a result
::method formatObject
  use strict arg o

  buf = .MutableBuffer~new
  buf~append('[', .TestUtil~escapeString(o~string), ', identityHash="', o~identityHash, '"]')
  return buf~string


-- format a error message
::method formatMessage class
  expose messages
  use strict arg id, substitutions, extra

  text = messages~entry(id)
  buffer = .mutablebuffer~new(text)

  loop i = 1 to substitutions~last
     buffer~changestr('&'i, self~formatObject(substitutions[i]))
  end

  buffer~append(": ", extra)


-- retrieve a specific message identifier
::method messageText class
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
  expose methodInfo
  use strict arg testMethod, failureType
  self~init:super(failureType)

  methodInfo = testMethod~methodinfo

  timestamp = .datetime~new

-- display the test failure information
::method displayFailure
  expose methodInfo
  use strict arg output

  self~displayFailure:super(output)
  methodInfo~display(output)


--  AssertionFailure is a report of a test case failure.  A test case fails when an
--  assertion does not hold.  This is the base assertion failure.
::class "AssertionFailure" public subclass TestFailure
::method init
  expose message assertionLine assertionType secondaryAssertion
  use strict arg testMethod, assertionType, message

  self~init:super(testMethod, "Assertion Failure")
  secondaryAssertion = .nil
  assertionLine = testMethod~testLineNumber


-- display the test failure information
::method displayFailure
  expose message assertionLine assertionType secondaryAssertion
  use strict arg output

  -- display the base information
  self~displayFailure:super(output)
  output~lineout("  Line:   " assertionLine)
  if message \= .nil then do
      output~lineout("  Purpose:" message)
  end
  output~lineout("  Failed: " self~assertionMessage)
  if secondaryAssertion \= .nil then do
     output~lineout("Failure Cause:")
     secondaryAssertion~displaySecondaryFailure(output)
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

  return .TestFailure~messageText(assertionType)


--  OneValueAssertionFailure is a report of a test case failure.  A test case fails when an
--  assertion about an object does not hold.  This is the base assertion failure.
::class 'OneValueAssertionFailure' public subclass AssertionFailure
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
::class 'TwoValueAssertionFailure' public subclass AssertionFailure
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
::method matches
  expose condition code
  use strict arg received

  -- note:  for non syntax conditions, the code in the condition object
  -- will be .nil, so this will compare equal.
  return condition == received~condition & code == received~code

-- display information about an expected condition failure
::method displayFailure
  expose assertionLine condition code message
  use strict arg output = .output

  output~lineout("  Failed:  Condition expected")
  if message \= .nil then do
     output~lineout("    Reason:  " message)
  end
  output~lineout("    Line:   " assertionLine)
  if condition == 'SYNTAX' then do
     output~lineout("    Expected: SYNTAX" code)
  end
  else do
     output~lineout("    Expected:" condition)
  end


-- information about a TestMethod that does not lock
-- the test classes in memory
::class "TestMethodInfo"
::method init
  expose testInfo testName
  use strict arg testCase, testName

  classInfo = testCase~testInfo


-- display base test information
::method display
  expose testInfo testName
  use strict arg output

  -- display the method name, then add the testcase information
  output~lineout("  Test:  " testName)
  testCase~display(output)


-- this represents a single test method within a TestCase.  This
-- handles the details of running a single test method and recording the
-- results
::class "TestMethod" inherit TestTypes
-- create an individual test associated with a TestCase
::method init
  expose testCase testName testMethod testResult expectedCondition knownFailure methodInfo
  use strict arg testCase, testResult, testName, testMethod

  methodInfo = .TestMethodInfo~new(testCase, testName)

  expectedCondition = .nil
  knownFailure = .nil

::attribute methodInfo GET

-- logging methods for test events.  These log to the parent entity
::method logFailure delegate testCase
::method logError delegate testCase

-- get any additional types marked for the method.
::method getTypes
  expose testMethod

  return .TestTypes~parseTypes(testMethod)


-- test if this method matches any test types.  Will match anything
-- if the method has no additional types defined
::method hasTestTypes
  use strict arg types

  methodTypes = self~getTypes
  if methodTypes == .nil then return .true

  return \methodTypes~intersection(types)~isEmpty


-- check if this method will work on the current platform
::method isForCurrentPlatform
  expose testMethod

  return .TestTypes~checkPlatform(testMethod)

-- test if this test is marked as a known failure
::method isKnownFailure
  expose testMethod knownFailure

  -- these are marked as known failures using annotations
  knownFailure = testMethod~annotation("KnownFailure")

  return knownFailure \= .nil


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
  expose testCase

  -- log the start event
  testCase~logTestMethodStart(self)

  -- setup for the test, creating a clean environment
  testCase~performTestSetup

  self~doTheTest                     -- carry out the testmethod

  -- cleanup after  the test, creating a clean environment
  testCase~performTestTeardown

  -- log the end event
  testCase~logTestMethodEnd(self)


-- wrapper around executing the actual test method
::method doTheTest
   expose testName testCase testResult knownFailure

   -- Trap all conditions propagated from the invocation of the test case
   -- method, or from the checkForConditionFailure method.
   signal on any name exceptionHandler

   -- now dynamically invoke the method
   testCase~send(testName)

   -- The test case method completed without raising a condition.  The last step
   -- is to check if the test case *exepected* a condition to be raised.
   self~checkForConditionFailure

exceptionHandler:

  -- Get the condition object.  (The condition object is a Directory object.)
  conditionObject = condition("O")

  -- If a condition was expected to be raised AND this condition matches the
  -- expected condition, then the test passed so just return.
  if self~checkCondition(conditionObject) then return

  -- If the condition code is 93.964 then this condition was raised by the
  -- framework to signal a test case failure.  The assert failure object is at
  -- index 2 of the additional array.
  if conditionObject~code = .TestCase~assertionCode then do
     -- first get the failure object, then
     failure = conditionObject~additional[2]

     -- if this method is annotated as a known failure, add the known
     -- failure information to the failure and add to the result known failures list
     if self~isKnownFailure then do
        failure~knownFailure = knownFailure
        testResult~addKnownFailure(failure)
     end
     -- add as a normal failure
     else do
       testResult~addFailure(failure)
     end
  end
  else do
    -- This is an unexpected, unanticipated error.
    -- add an error object to the test result
    err = .UnexpectedError~new(self, conditionObject)
    testResult~addError(err)
  end


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


-- a mixin class to add the test execution infrastructure to a test case type
::class "TestExecutor" mixinclass object
-- run the tests and display the results
::method run class
  use strict arg container = (.TestContainer~new)
  self~execute(container)
  container~display

-- execute an this test case by creating an instance and having it execute.
::method execute class
  use strict arg container = (.TestContainer~new)

  instance = self~createTestInstance(container)
  if instance == .nil then return

  -- execute the tests, returning a composite result
  instance~execute(container)

-- create an instance of a test case class, with syntax error protection
::method createTestInstance class
  use strict arg container, testResult

--  signal on syntax
  return self~new(container)

syntax:
  container~testResult~addError(.TestInstantiationError~new(testClass, condition('o')))
  return .nil


-- common display method for test entities
::method displayName class
  return self~id

-- the instance display method
::method displayName
  return self~class~displayName

-- common display method for test entities
::method testPackage class
  return self~package~name

-- the instance display method
::method testPackage
  return self~class~testPackage


-- information class for test case status reporting
::class "TestCaseInfo"
::method init
  expose className fileName mainAssertion
  use strict arg testCase

  className = testCase~class~id
  -- get the containing file name for reporting.
  fileName = testCase~class~package~name

  -- try to determine if this file is in svn and get the information
  -- if it is
  svnInfo = .svnInfo~new(fileName)


-- display base test information
::method display
  expose className fileName svnInfo
  use strict arg output = .output

  output~lineout("  Class: " className)
  output~lineout("  File:  " fileName)
  if svnInfo~revision \= .nil then do
     output~lineout("  SVN:     r"svnInfo~revision "  Change Date: " svnInfo~date)
  end


-- base class for a TestCase class.  This is a class that implements
-- a set of test methods.  This handles invocation of the different
-- test methods and recording of result information from the tests.
::class "TestCase" public inherit TestExecutor TestTypes
::constant AssertionCode 93.964

-- The test container is used for logging, filtering, output redirection, etc.
::method init
  expose container testInfo mainAssertion
  use strict arg container = (.TestContainer~new)

  -- creat an information item for this.
  testInfo = .TestCaseInfo~new(self)

  mainAssertion = .nil

-- access to the base information about the test case
::attribute testInfo


-- execute all of the methods in a test case class, returning the accumulated
-- statistics
::method execute
  expose container testResult
  use strict arg container

  testResult = container~testResult

  testMethods = self~getTestMethods

  container~logTestCaseStart(self)
  testResult~testStart(self)

  -- for each method create a runner for this and execute the test
  loop methodName over testMethods
     method = self~instanceMethod(methodName)
     -- create a wrapper to run this test method
     self~currentTestMethod = .TestMethod~new(self, testResult, methodName, method)
     if container~filterTestMethod(self, methodName, method) then do
         -- run the test method, which will add its results to the accumulator
         self~currentTestMethod~execute
     end
  end

  testResult~testEnd(self)
  container~logTestCaseEnd(self)


-- locate the test method names for this test case class.  Returns
-- an array of executable testMethod objects.
::method getTestMethods
  expose container

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

-- logging methods.  These just forward to the container object
::method logFailure delegate container
::method logError delegate container
::method logTestMethodStart delegate container
::method logTestMethodEnd delegate container


-- count a successful assertion
::method assertionPassed delegate testResult

-- count a successful assertion where we have nesting.
::method nestedAssertionPassed
  expose mainAssertion

  mainAssertion = .nil
  self~assertionPassed


-- checks that two collections are equilvalent.  Does not take
-- order into account.  Use assertArrayEqual or assertArrayStrictlyEqual
-- if order is important
::method assertCollectionsEqual
  use strict arg expected, actual, msg = ""

  self~assertTrue(actual~isa(.Collection), msg': actual must be a collection')
  self~assertTrue(expected~equivalent(actual), msg': actual must be equivalent to expected')


-- checks that two collections are equilvalent.  Does not take
-- order into account.  Use assertArrayEqual or assertArrayStrictlyEqual
-- if order is important
::method assertCollectionsNotEqual
  use strict arg expected, actual, msg = ""

  self~assertFalse(expected~equivalent(actual), msg': actual must not be equivalent to expected')

-- Checks if two objects are equal
::method assertEqual
  use strict arg expected, actual, msg = ""

  -- first compare the objects directly, and if they do not compare,
  -- try to compare them as collections.  Note that we use the "=" comparison
  -- variant here.
  if expected \= actual then do
      self~twoValueFail("assertEqual", expected, actual, msg)
  end

  -- Assertion holds
  self~assertionPassed


-- a compatibility synonym
::method assertEquals
  forward message('assertEqual')


--  assertFuzzyEquals()
-- Checks if two objects are equal using fuzzy math
::method assertFuzzyEqual
  use strict arg expected, actual, digits = (digits()), msg = ""

  numeric digits digits
  numeric fuzz 1

  if expected \= actual then do
      self~twoValueFail("assertFuzzyEqual", expected, actual, msg)
  end

  -- Assertion holds
  self~assertionPassed


-- assert that a result does not equal a given value
::method assertNotEqual
  use strict arg expected, actual, msg = .nil

  -- If the 2 objects are not equal AND they are not 2 equal collections ...
  if expected \= actual then do
    -- ... the assertion holds
    self~assertionPassed
    return
  end

  self~twoValueFail("assertNotEqual", expected, actual, msg)


-- a compatibility synonym
::method assertNotEquals
  forward message('assertNotEqual')


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
::method assertArraysEqual
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
::method assertArraysStrictlyEqual
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
::method assertStrictlyEqual
  use strict arg expected, actual, msg = .nil

  if expected \== actual then do
      self~twoValueFail("assertStrictlyEquals", expected, actual, msg)
  end

  -- Assertion holds
  self~assertionPassed


-- a compatibility synonym
::method assertSame
  forward message('assertStrictlyEqual')


-- do an assertion of non-equality
::method assertStrictlyNotEqual
  use strict arg expected, actual, msg = .nil

  if (expected == actual) then do
      self~twoValueFail("assertStrictlyNotEquals", expected, actual, msg)
  end

  -- Assertion holds
  self~assertionPassed


-- a compatibility synonym
::method assertNotSame
  forward message('assertStrictlyNotEqual')


-- Check if an object is EXACTLY the same object instance
::method assertIdentical
  use strict arg expected, actual, msg = .nil

  if (expected~identityHash \= actual~identityHash) then do
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

  if .true \== actual then
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
  return self~class~id'@'filename


-- test suite is a collection of test case files.  This will
--run all of the indicated tests and return a a consoladated
-- result object.
::class "TestGroup" inherit TestExecutor TestTypes
::method init
  expose testQueue

  -- this will be our set of tests to run.
  testQueue = .queue~new
  -- tests are frequently listed in a resource within the package,
  -- check to see if we have such a list
  self~checkResourceList


-- test if any tests have been added to this group
::method isEmpty
  expose testQueue

  return testQueue~isEmpty


-- return the count of testcase classes contained in the test group
::method testCount
  expose testQueue
  return testQueue~items

-- check this class for an annotation that identifies a resource
-- item in the same package that contains a list of files included
-- in this test suite.
::method checkResourceList
  expose testQueue
  fileResource = self~class~annotation('TestFiles')

  if fileResource == .nil then return

  fileList = self~class~package~resource(fileResouce)
  if fileList == .nil then return

  -- Try to load each file and add the test class to the queue.
  loop file over fileList
     self~loadFile(file)
  end


-- load a Test file to the set.
::method loadFile
  use strict arg fileName

  -- if we can't resolve this file, just add an error to the result
  fullName = self~resolveTestProgram(fileName)
  if fullName == .nil then do
     self~addLoadFailure(fileName)
     return .nil
  end

   -- now create a routine from this file
   signal on syntax
   r = .routine~newfile(fullName)

   -- and scan the package looking for eligible test files
   return self~loadPackage(r~package)

-- some sort of syntax error creating the file
syntax:
   -- add an error and return .nil to indicate a failure
   self~addLoadFailure(fileName)
   return .nil


-- add all test types from a package file to the test group
::method loadPackage
  expose testQueue container
  use strict arg package

  -- locate the classes that are executable tests.  Other classes
  -- are likely just part of the tests.
  loop testClass over package~classes
     -- we add an instance to the queue
     if testClass~isSubclassOf(.TestCase) | testClass~isSubClassOf(.TestGroup) then do
         testQueue~append(self~getTestInstance(testClass))
     end
  end


-- get and configure an instance of a test class for adding to this group
::method getTestInstance
  expose container
  use strict arg class

  return class~new(container)


-- configure an instance of a test class for adding to this group
::method configureTestInstance
  expose container
  use strict arg test

  test~setController(container)
  return test


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
     self~loadFile(newTestCase)
  end
  else if newTestCase~isA(.TestCase) then do
     testQueue~append(self~configureTestInstances(newTestCase))
  end
  else if newTestCase~isA(.TestGroup) then do
     testQueue~append(self~configureTestInstances(newTestCase))
  end
  else if newTestCase~isA(.Class), newTestCase~isSubclassOf(.TestCase) then do
     testQueue~append(self~getTestInstances(newTestCase))
  end
  else if newTestCase~isA(.Class), newTestCase~isSubclassOf(.TestGroup) then do
     testQueue~append)
  end
  else if newTestCase~isA(.Package) then do
     self~loadPackage(newTestCase)
  end


-- execute the test group instance.  This will execute all of the contained tests.
::method execute
  expose testQueue
  use strict arg container

  -- get a result set from the container
  testResult = container~testResult
  testResult~startTestGroup(self)   -- record that we're starting a test group

  self~setUp                        -- make sure setup is invoked before testSuite runs

  -- execute the tests and merge the result information
  loop while \testQueue~isEmpty
     test = testQueue~pull
     -- first check to see if this test passes the selection criteria,
     -- then run if requested.
     if container~filterTestCase(test) then do
        test~execute(container)
     end

  end

  self~tearDown                     -- make sure tearDown is invoked after testSuite ran
  testResult~endTestGroup(self)     -- remember test ended

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
  -- the test suite functions as a container (which might be running
  -- under a different container.  This replaces any container we
  -- might have been instantiated with.
  use strict arg testSuite

  testResult = .ResultSet~new(self)

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

-- a mixin class for managing test types
::class "TestTypes" public mixinclass Object
::method activate class
  expose defaultTypes typeToName nameToType allTypes nameString knownPlatforms unixPlatforms

  defaultTypes = .set~of(self~UNIT_TEST, self~UNIT_LONG_TEST, self~SAMPLE_TEST, self~GUI_TEST, self~GUI_SAMPLE_TEST, -
                  self~OLE_TEST, self~DOC_EXAMPLE_TEST, self~NATIVE_API_TEST, self~ENGLISH_ONLY_TEST)

  -- a set of all available types
  allTypes = .set~new
  do i = self~MIN_TEST_TYPE to self~MAX_TEST_TYPE
    all~put(i)
  end

  typeToName = .stringtable~new
  nameToType = .stringtable~new

  -- locate the test constants and fill the table
  itr = self~methods(.nil)

  do while itr~available
    name = itr~index
    if name~endsWith("_TEST") then do
      name = name~left(name~length - 5)
      number = self~send(itr~index)

      -- use a more readable name for this
      n = name~lower(2)
      names~setEntry(name, number)
      names~setEntry(number, n)
    end
    itr~next
  end

  -- All possible OS words are put into the allowed OSes set, although it is
  -- doubtful that ooRexx is compiled on the last 3.
  knownPlatforms = .set~of('WINDOWS', 'UNIX', 'LINUX', 'SUNOS', 'AIX', 'MACOSX', 'CYGNUS', 'FREEBSD', 'NETBSD')

::constant MIN_TEST_TYPE                1

::constant UNIT_TEST                    1

::constant UNIT_LONG_TEST               2
::constant SAMPLE_TEST                  3
::constant GUI_TEST                     4
::constant GUI_SAMPLE_TEST              5
::constant OLE_TEST                     6
::constant DOC_EXAMPLE_TEST             7

-- A test type that makes noise.  I frequently need to run the test suite in
-- an environment where I need these types of test cases to be skipped.  This
-- is a convenience for myself (Mark Miesfeld.)
::constant DOC_EXAMPLE_NOISE_TEST       8

-- A test type for the ooTest framework examples.
::constant FRAMEWORK_EXAMPLE_TEST       9

-- A test type for the ooRexx APIs.
::constant NATIVE_API_TEST             10

-- A test type involving TCPIP, smtp, ftp, for example, where the test might
-- need some special set up.  Like a ftp server, mail server, etc..
::constant TCPIP_TEST                  11

-- A test type that can only work with an English language version of Windows.
::constant ENGLISH_ONLY_TEST           12

::constant MAX_TEST_TYPE               12

-- The default test type is the unit test (see above for value.)
::constant DEFAULT_TEST_TYPE            1

-- The standard name of a type annotation
::constant TESTTYPES "TestTypes"

-- the standard name of an OS restriction annotation
::constant RESTRICTPLATFORM "RestrictPlatform"


-- Returns the set of tests that are always run.  Any test type in this set
-- will execute unless the tester specifically eXcludes it.
--
-- @param  format  Specifies the format of the returned set.  Can be either
--                 Constant or String, the default is Constant.  Constant
--                 returns a set containing the numeric constants of the
--                 default tests, String returns a set of the names of the
--                 default tests.  Only the first letter is needed and case
--                 is not significant.
--
-- @return A set of the tests that are always executed when the test suite is
--         run.
::method defaultTestSet class
  expose defaultTypes

  return defaultTypes~copy


-- an instance method forwarder.
::method defaultTestSet
  forward class (self~class)


-- Returns a set of all the test types possible.
::attribute allTypes get class


-- Returns the numeric test type constant for the specified name, or .nil if
-- there is no such test type.
::method getTestForName class
  expose nameToTest
  use strict arg name

  return nameToTest~entry(name)

-- instance method for mapping a test to a name
::method getTestForName
  forward to(self~class)


-- Returns the string name corresponding to a numeric test type constant, or
-- .nil if there is no such test type.
::method nameForTest class
  expose testToName
  use strict arg test

  return testToName[test]


-- Returns a string containing the test names for all the corresponding
-- numeric test type constants in a collection.  Returns .nil if: one of the
-- items in the collection is not a test type, the argument is not a
-- collection.
::method namesForTests class
  expose nameToTest
  use strict arg tests

  if \ tests~isA(.Collection) then return .nil

  names__ = ""
  do t over tests
    name = names~entry(t)
    if name == .nil then return .nil
    names__ = names__ name
  end

  return names__


-- instance method for getting test information
::method namesForTests
  forward to(self~class)

-- get the test types for a given class
::method getTypes class
  expose defaultTypes

  -- if we have an annotation, parse it out, otherwise return the full set
  types = self~parseTypes(self)
  if types == .nil then return defaultTypes


-- parse a type string into a set of recognized types
::method parseTypes class
  expose nameToType
  use strict arg entity

  types = entity~annotation(self~TESTTYPES)
  if types == .nil then return .nil

  testTypes = .set~new
  loop type over types~subwords
     typeValue = nameToType~entry(type)
     if typeValue \= .nil then typeTypes~put(typeValue)
  end

  return testTypes


-- instance method for getting test types
::method getTypes
  expose testTypes

  if \var('TESTTYPES') then testTypes = self~class~getTypes
  return testTypes


-- test if a class is a given type
::method isType
  use strict arg type

  return self~getTypes~hasIndex(type)


-- test if this test matches any of the test types
::method hasTestType
  use strict arg types

  return \self~getTypes~intersection(types)~isEmpty


-- check if this method will work on the current platform
::method isForCurrentPlatform class
  expose testMethod
  use strict arg entity

  -- if no annotation, then this is unrestricted, so
  -- return true
  types = entity~annotation(self~RESTRICTPLATFORM)
  if types == .nil then return .true

  -- if the current OS name is contained in the test, return immediately
  if types~caselessContainsWord(.ooRexxTest~OSName) then return .true

  -- we have a generic UNIX type for all unix versions
  if types~caselessContainsWord('UNIX'), .ooRexxTest~isUnix then return .true

  -- nope, won't work on this platform
  return .false


-- check if this method will work on the current platform
::method isForCurrentPlatform
  return self~classIsForCurrentPlatform(self~class)

-- give a progress heartbeat...one tick for each test group executed
::class ProgressTicker
::method init
  expose tick stop

  tick = .false
  stop = .false
  reply

  do while \stopp
    guard on when tick | stop
    .stdout~charout('.')
  end

::attribute tick unguarded set
::attribute stop unguarded set


