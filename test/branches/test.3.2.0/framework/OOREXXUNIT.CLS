#!/usr/bin/rexx
/*
-- this is a ooRexx line comment, will get ignored by pre-processor which analyzes this header
-- enclosed between the very first block comment, i.e. everything between the first "/*" .. "*/"

   name:             ooRexxUnit.cls
   author:           Rony G. Flatscher, Rick McGuire
   date:             2005-08-07
   version:          1.0.6
   changed1:         2005-08-07, ---rgf, moved assertion routines to a class "Assert", which
                                 serves as a superclass for TestCase: this is the junit-approach
                     2005-08-20, ---rgf, added license, assertCount, corrected some little bugs
                     2005-08-21, ---rgf, added public routine "makeTestSuiteFromFileList()"
                     2005-08-30, ---rgf, changed some comments from "ooRexx" to "ooRexxUnit"
                     2005-10-09, ---rgf, only count assertions, if successful
                     2005-10-14, ---rgf, changed failure-message string slightly to improve understandibility
                     2005-10-27, ---rgf, added ability to report which assertion failed for what reason
                                         (suggested by Walter Pachl)
                     2005-10-28, ---rgf, added @assertFailure attribute to lead-in the report on
                                         the assertion failure, removed angle-brackets from that added text
                     2006-03-25, ---rgf, removed 'private' attribute from TestResult's ooRexx only attributes
                                         "logQueue" and "TestCaseTable"
                     2006-04-11, Rick McGuire, added condition-handling to Assert and TestCase.run()
                     2006-05-17, ---rgf, altered Rick's code slightly,
                                            expectSyntax(errorCode) and
                                            expectCondition(conditionName)
                     2006-10-17, ---rgf, escape non-printable chars in error/failure string information to
                                         a Rexx style string literal (concatenating hexadecimal strings);
                                         use new syntax error # 93.964 (application error) instead of
                                         error # 40.1, which had to be used prior to ooRexx 3.1.1
                     2006-11-05, ---rgf, - moved initialisation of "defaultTestResultClass" to the class'
                                           constructor; *but* this also needs the definition of the "TestResult"
                                           class to be moved physically before the "TestCase" class (otherwise
                                           the class is not known yet and the string ".TESTREUSLT" is stored
                                           instead of the class object!!);
                                         - error message encodings now always add a colon to the word "ERROR"; the
                                           the error message after the eye catcher string "--->" is not enquoted
                                           in square brackets anymore
                                         - failure message encodings, if given, now use the string "--->"
                                           as an eye catcher for parsing and is not enquoted in square brackets anymore
                     2006-11-27, ---rgf, - changed Assert[Not]Same to show ObjectID in failure message to ease comparison
                     2006-11-28, ---rgf, - corrected logic to intercept any condition from running a test case
                     2006-12-13, ---rgf, - changed wording from "expected condition...was not received" to
                                           "expected condition...was not raised"
                                         - made sure that "makeTestSuiteFromFileList()" will create an own test suite per
                                           test class for which mandatory test methods got listed
                     2006-12-14, ---rgf, added hashbang line
                     2006-12-26/27/28, ---rgf, - removed "bWalterPachl" flag (to show reason of failure) as
                                           reason of failures are always shown; even if no failure message
                                           is supplied the assertion methods will still supply the "@assertionFailure"
                                           string to indicate expected and (not matching) received values
                                         - enhanced "assertEquals" to work on ordered and unordered collections:
                                           - unordered collections (of any type) are regarded to be equal, if both contain
                                             the same number of the same index/value pairs;
                                           - ordered collections (of any type) are regarded to be equal, if the items
                                             in the MAKEARRAY object are the same in the same order
                     2006-12-30, ---rgf, added method assertNotEquals to Assert class (ooRexxUnit only, makes it easier
                                         to test for unequal collection values)
                     2007-01-24, ---rgf, - changed EOL to Unix style (to make hash-bang work on Unix)
                                         - added a function ooRexxUnit.getShellName(): central routine to return
                                           the name of the shell to ADDRESS to
                     2007-01-??, Mark Miesfeld, - added ooRexxUnit.getShellName()
                                                - added ooRexxUnit.getOSName()
                     2007-04-28, ---rgf, - makeTestSuiteFromFileList(): if required files are missing
                                           in testUnit's, then program does not abend, rather it ignores
                                           that testUnit, displying the condition on STDERR
                                         - added ooRexxUnit.formatConditionObject()
                     2007-05-27, ---rgf, - added environment symbols ".oorexxunit.interpreterName",
                                           ".oorexxunit.languageLevel" and ".oorexxunit.interpreterDate",
                                           retrieved from "PARSE VERSION"
                     2007-05-30, ---rgf, - added environment symbols ".ooRexxUnit.shellName", "ooRexxUnit.OSName",
                                           ".ooRexxUnit.line.separator", ".ooRexxUnit.directory.separator",
                                           ".ooRexxUnit.path.separator"
                     2007-07-12, ---rgf, - added attribute 'definedInFile' to 'TestCase' class
                                         - 'TestResult': new directory entry in queue for 'startTest' and 'endTest'
                                           named 'OOREXXUNIT.CLASS_TESTCASEINFO', which refers to the class object's
                                           'testCaseInfo' directory
                                         - 'makeTestCaseString()': now returns fully qualified filename after
                                           '@' marker or the string "n/a_XXXXXXXX", where "XXXXXX" is o~"=="~c2x
                     2007-07-19, ---rgf, - added option to "TestCase" and "TestSuite"'s 'run' method to
                                           display information about the test case to be run about to stderr;
                                           this can be used to present the user a feedback about the proceedings
                                           of the tests
                     2007-08-17, ---rgf, - corrected a small typo; now that we have a release version removing pre 3.2
                                           code and tests, using new features
                     2007-08-18, ---mm,  - add public routine simpleFormatTestResults()
                     2007-10-10, ---rgf, - enhanced framework to allow accepting testSuite objects at testSuite
                                           creation time (and also collections of test cases)


   language-level:   6.0.2
   needs:            ooRexx 3.2 or later
-- determines the minimum ooRexx language level (6.02 = ooRexx, IBM Object REXX)

   purpose:          Supply the base classes for a JUnit compliant testing framework for ooRexx

   remark:           Wherever possible the JUnit class and method names are used to help ease
                     the understanding.

   license:          CPL 1.0 (Common Public License v1.0, see below)

   link:             http://www.junit.org
                     http://junit.sourceforge.net/doc/cookbook/cookbook.htm
                     http://junit.sourceforge.net/doc/cookstour/cookstour.htm


-- there may be any number of subcategories, most important listed first, second important second, ...
-- no need to append numbers, but may be easier to realize the category level easily

-- this is the main categorization
   category0:        ooRexxUnit

-- this is the next concrete categorization
   category1:        framework
*/

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

-- Define the version number with: X.Y.Z  Where X is the ooRexxUnit.cls version,
-- Y is the minimum ooRexx interpreter level required, and Z is the date the
-- version was changed.
.local~ooRexxUnit.version=106.320.20071205

.local~chars.NonPrintable=xrange("00"x, "1F"x) || "FF"x  -- define non-printable chars

parse version interpreterName languageLevel interpreterDate
.local~ooRexxUnit.interpreterName=interpreterName
.local~ooRexxUnit.languageLevel=languageLevel
.local~ooRexxUnit.interpreterDate=interpreterDate

.local~ooRexxUnit.shellName=ooRexxUnit.getShellName()
.local~ooRexxUnit.OSName   =ooRexxUnit.getOSName()

      -- define end-of-line chars
.local~ooRexxUnit.line.separator=.endOfLine

      -- get and memorize the path.separator on this system
.local~ooRexxUnit.directory.separator=filespec("Path", directory())~left(1)
if .ooRexxUnit.directory.separator="\" then     -- DOS-like
   .local~ooRexxUnit.path.separator=";"
else                                            -- Unix-like
   .local~ooRexxUnit.path.separator=":"

-- Capture the ooRexxUnit framework directory and ensure it is in the path.
parse source . . fileSpec
.local~ooRexxUnit.dir = fileSpec~left(fileSpec~caseLessPos("OOREXXUNIT.CLS") - 2 )
currentPath = value("PATH", , 'ENVIRONMENT')
if currentPath~caseLessPos(.ooRexxUnit.dir) == 0 then
   oldPath = value("PATH", .ooRexxUnit.dir || .ooRexxUnit.path.separator || currentPath, 'ENVIRONMENT')


/* *********************************************************************************** */
/* *********************************************************************************** */
::class "TestResult" public
::method init
  expose fErrors fFailures fRunTests fStop fTestRuns TestCaseTable logQueue fAssertions
      -- initialize object variables
  fErrors=.queue~new
  fFailures=.queue~new
  logQueue=.queue~new  -- ooRexxUnit only, logs all

  fAssertions=0
  fRunTests=0
  fStop=.false

  TestCaseTable=.table~new

::method logQueue attribute               -- ooRexxUnit only
::method TestCaseTable attribute          -- ooRexxUnit only

::method addError       -- ooRexxUnit only, allows to be overriden/intercepted by subclasses
  expose fErrors TestCaseTable logQueue
  use arg aTestCase, co

  fErrors~queue(co)     -- enqueue the condition object; cf. entry "OOREXXUNIT.CONDITION"
  logQueue~queue(co)
  TestCaseTable[aTestCase]~queue("   " co~ooRexxUnit.Condition) -- "--->" pp(msg))


::method addFailure     -- ooRexxUnit only, allows to be overriden/intercepted by subclasses
  expose fFailures TestCaseTable logQueue
  use arg aTestCase, co

  fFailures~queue(co)   -- enqueue the condition object; cf. entry "OOREXXUNIT.CONDITION"
  logQueue~queue(co)
  TestCaseTable[aTestCase]~queue("   " co~ooRexxUnit.Condition)

::method assertCount    -- ooRexxUnit only
  expose fAssertions
  return fAssertions

::method endTest        -- informs that the supplied test was completed
  expose TestCaseTable fStop logQueue fAssertions
  use arg aTestCase

  dateTime=pp(date("s") time("L"))
  TestCaseTable[aTestCase]~queue(dateTime": endTest")
  dir=.directory~new ~~setentry("OOREXXUNIT.CONDITION", dateTime":" pp("endTest") makeTestCaseString(aTestCase) )

  -- save class' 'testCaseInfo' with test case for later analysis
  dir~setentry("OOREXXUNIT.class_testCaseInfo", aTestCase~class~testCaseInfo)

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


::method run            -- convenience method to run given TestCase
  use arg aTestCase
  return aTestCase~run(self)

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

  dateTime=pp(date("s") time("L"))
  TestCaseTable[aTestCase]~queue(dateTime": startTest")
  dir=.directory~new ~~setentry("OOREXXUNIT.CONDITION", dateTime":" pp("startTest") makeTestCaseString(aTestCase))

  -- save class' 'testCaseInfo' with test case for later analysis
  dir~setentry("OOREXXUNIT.class_testCaseInfo", aTestCase~class~testCaseInfo)

  logQueue~queue(dir)

  fStop=.false          -- reset indicator
  fRunTests=fRunTests+1 -- increase run counter

::method stop           --  mark that the test run should stop
  expose fStop
  fStop=.true

::method wasSuccessful  -- returns whether the entire test was successful or not
  expose fErrors fFailures

  return (fErrors~items+fFailures~items)=0



/* *********************************************************************************** */
/* *********************************************************************************** */
::class "Assert" public

::method init
  expose fAssertions          -- count assertions, ooRexxUnit only
  fAssertions=0

::method assertCount          -- ooRex only
  expose fAssertions          -- count assertions, ooRexxUnit only
  return fAssertions

::method fAssertions attribute private    -- ooRexxUnit only

   -- assertions will raise a user error, if they do not hold
::method assertEquals
  expose fAssertions          -- count assertions, ooRexxUnit only
  if arg()=2 then             -- no failure message supplied
  do
     bEquals=(arg(1)=arg(2))
         -- values are not equal, but both are collections, test for equality
     if \bEquals then
     do
         bEquals=isCollEqual(arg(1), arg(2))
     end

     if bEquals then
     do
        fAssertions=fAssertions+1
        return -- assertion holds
     end
     self~fail("@assertFailure assertEquals: expected="formatObjectInfo(arg(1))", actual="formatObjectInfo(arg(2))"."||"09"x)
  end

  bEquals=(arg(2)=arg(3))
         -- values are not equal, but both are collections, test for equality
  if \bEquals then
  do
      bEquals=isCollEqual(arg(2), arg(3))
  end

  if bEquals then
  do
     fAssertions=fAssertions+1
     return    -- assertion holds
  end

  sTmp="@assertFailure assertEquals: expected="formatObjectInfo(arg(2))", actual="formatObjectInfo(arg(3))"."||"09"x
  self~fail(sTmp || arg(1))      -- fail with msg




::method assertFalse
  expose fAssertions          -- count assertions, ooRexxUnit only
  if arg()=1 then             -- no failure message supplied
  do
     if arg(1)=.false then
     do
        fAssertions=fAssertions+1
        return   -- assertion holds
     end
     self~fail("@assertFailure assertFalse: expected=[0], actual="ppp(arg(1))"."||"09"x)
  end

  if arg(2)=.false then
  do
     fAssertions=fAssertions+1
     return      -- assertion holds
  end

  sTmp="@assertFailure assertFalse: expected=[0], actual="ppp(arg(2))"."||"09"x
  self~fail(sTmp || arg(1))      -- fail with msg


   -- assertions will raise a user error, if they do not hold
::method assertNotEquals      -- ooRexxUnit only, 2006-12-30
  expose fAssertions          -- count assertions, ooRexxUnit only
  if arg()=2 then             -- no failure message supplied
  do
     bEquals=(arg(1)=arg(2))
         -- values are not equal, but both are collections, test for equality
     if \bEquals then
     do
         bEquals=isCollEqual(arg(1), arg(2))
     end

     if \bEquals then
     do
        fAssertions=fAssertions+1
        return -- assertion holds
     end
     self~fail("@assertFailure assertNotEquals: expected="formatObjectInfo(arg(1), "\= ")", actual="formatObjectInfo(arg(2))"."||"09"x)
  end

  bEquals=(arg(2)=arg(3))
         -- values are not equal, but both are collections, test for equality
  if \bEquals then
  do
      bEquals=isCollEqual(arg(2), arg(3))
  end

  if \bEquals then
  do
     fAssertions=fAssertions+1
     return    -- assertion holds
  end

  sTmp="@assertFailure assertNotEquals: expected=["formatObjectInfo(arg(2), "\= ")", actual="formatObjectInfo(arg(3))"."||"09"x
  self~fail(sTmp || arg(1))      -- fail with msg



::method assertNotNull
  expose fAssertions          -- count assertions, ooRexxUnit only
  if arg()=1 then             -- no failure message supplied
  do
     if .nil<>arg(1) then
     do
        fAssertions=fAssertions+1
        return    -- assertion holds
     end
     self~fail("@assertFailure assertNotNull: expected=[\= [.nil]], actual=[.nil]."||"09"x)
  end

  if .nil<>arg(2) then
  do
     fAssertions=fAssertions+1
     return       -- assertion holds
  end

  sTmp="@assertFailure assertNotNull: expected=[\= [.nil]], actual=[.nil]."||"09"x
  self~fail(sTmp || arg(1))      -- fail with msg


::method assertNotSame
  expose fAssertions          -- count assertions, ooRexxUnit only
  if arg()=2 then             -- no failure message supplied
  do
     if (arg(1)==arg(2))=.false then
     do
        fAssertions=fAssertions+1
        return  -- assertion holds
     end

     self~fail("@assertFailure assertNotSame: expected="formatObjectInfo(arg(1), "\== ")", actual="formatObjectInfo(arg(2))"."||"09"x)

     -- self~fail("@assertFailure assertNotSame: not expected="ppp(arg(1)~string "<hashValue:" getEscapedhashValue(arg(1))">")", received="ppp(arg(2)~string "<hashValue:" getEscapedhashValue(arg(2))">")"."||"09"x)
  end

  if (arg(2)==arg(3))=.false then
  do
     fAssertions=fAssertions+1
     return     -- assertion holds
  end

  -- sTmp="@assertFailure assertNotSame: not expected="ppp(arg(2)~string "<hashValue:" getEscapedhashValue(arg(2))">")", received="ppp(arg(3)~string "<hashValue:" getEscapedhashValue(arg(3))">")"."||"09"x

  sTmp="@assertFailure assertNotSame: expected="formatObjectInfo(arg(2), "\== ")", actual="formatObjectInfo(arg(3))"."||"09"x
  self~fail(sTmp || arg(1))      -- fail with msg


::method assertNull
  expose fAssertions          -- count assertions, ooRexxUnit only
  if arg()=1 then             -- no failure message supplied
  do
     if .nil=arg(1) then
     do
        fAssertions=fAssertions+1
        return     -- assertion holds
     end
     self~fail("@assertFailure assertNull: expected=[.nil], actual="ppp(arg(1))"."||"09"x)
  end

  if .nil=arg(2)then
  do
     fAssertions=fAssertions+1
     return         -- assertion holds
  end

  sTmp="@assertFailure assertNull: expected=[.nil], actual="ppp(arg(2))"."||"09"x
  self~fail(sTmp || arg(1))      -- fail with msg


::method assertSame
  expose fAssertions          -- count assertions, ooRexxUnit only
  if arg()=2 then             -- no failure message supplied
  do
     if (arg(1)==arg(2))=.true then
     do
        fAssertions=fAssertions+1
        return-- assertion holds
     end
     -- self~fail("@assertFailure assertSame: expected="ppp(arg(1)~string "<hashValue:" getEscapedhashValue(arg(1))">")", received="ppp(arg(2)~string "<hashValue:" getEscapedhashValue(arg(2))">")"."||"09"x)
     self~fail("@assertFailure assertSame: expected="formatObjectInfo(arg(1))", actual="formatObjectInfo(arg(2))"."||"09"x)
  end

  if (arg(2)==arg(3))=.true then
  do
     fAssertions=fAssertions+1
     return   -- assertion holds
  end

  sTmp="@assertFailure assertSame: expected="formatObjectInfo(arg(2))", actual="formatObjectInfo(arg(3))"."||"09"x
  self~fail(sTmp || arg(1))      -- fail with msg


::method assertTrue
  expose fAssertions          -- count assertions, ooRexxUnit only
  if arg()=1 then             -- no failure message supplied
  do
     if arg(1)=.true then
     do
        fAssertions=fAssertions+1
        return    -- assertion holds
     end
     self~fail("@assertFailure assertTrue: expected=[1], actual="ppp(arg(1))"."||"09"x)
  end

  if arg(2)=.true then
  do
     fAssertions=fAssertions+1
     return       -- assertion holds
  end

  sTmp="@assertFailure assertTrue: expected=[1], actual="ppp(arg(2))"."||"09"x
  self~fail(sTmp || arg(1))      -- fail with msg


::method fail
       -- ooRexx: need to raise a syntax error, because USER exception needs to be propageted
  if arg()=0 then msg=""
             else msg=arg(1)

   -- use application definable syntax error (since ooRexx 3.1), supply description for
   -- test methods to be able to find out that the ooRexxUnit framework raised it
  RAISE syntax 93.964 array (msg) description ("ooRexxUnit.cls - source of syntax exception 'FAIL' method invocation in class 'ASSERT'.")



::method conditionExpected attribute

::method clearCondition
  expose conditionExpected
  conditionExpected = .false

::method expectSyntax
  expose conditionExpected conditionName errorCode
  parse arg errorCode       -- retrieve errorCode
  conditionName="SYNTAX"
  conditionExpected=.true

  -- self~expectCondition( "SYNTAX", arg(1) )

::method expectCondition
  expose conditionExpected conditionName
  use arg conditionName           -- only name of condition is expected, can be two words, e.g. "USER SOMETHING"
  conditionExpected = .true


::method checkCondition
  expose conditionExpected conditionName errorCode fAssertions
  use arg receivedCondition

  if receivedCondition~condition == conditionName then
  do
      if conditionName == "SYNTAX" then
      do
          if errorCode <> receivedCondition~code then
          do
              return .false
          end
      end
      fAssertions=fAssertions+1        -- asserted that expected conditionName has occurred!
      return .true
  end
  return .false



   /* Method that will fail, if a condition was expected, but had not been raised. */
::method check4ConditionFailure
  expose conditionExpected conditionName errorCode

  if conditionExpected=.true then
  do
     if conditionName="SYNTAX" then
        tmpStr=conditionName errorCode    -- supply the expected errorCode with the expected exception
     else
        tmpStr=conditionName

     sTmp="@assertFailure check4ConditionFailure: expected condition" pp(tmpStr) "was not raised."||"09"x

     self~fail(sTmp)                -- fail with msg
  end



/* *********************************************************************************** */
/* *********************************************************************************** */
::class "TestCase" subclass "Assert" public

::method init class
  self~defaultTestResultClass=.TestResult -- set default: use TestResult class
  self~TestCaseInfo=.directory~new
  forward class (super)

::method defaultTestResultClass class attribute -- ooRexxUnit only
::method TestCaseInfo class attribute


::method init        -- constructor
  expose fName fCountTestCases definedInFile    -- name of Testcase (method) to carry out
  parse arg fName

  fCountTestCases=1  -- default: individual test
  self~TestCaseInfo=.directory~new -- directory to contain information on test

  clzTCI=self~class~testCaseInfo    -- get access to clz' 'testCaseInfo' dir
  s=clzTCI~entry("test_Case-source")
  if .nil<>s then                   -- if source available, memorize fully qualified file name
     parse var s . . definedInFile
  else
    definedInFile="n/a_"self~identityHash -- indicate that no file name available


  self~init:super    -- let superclass initialize

::method definedInFile attribute    -- allows to quickly get filename from which test case is taken

::method TestCaseInfo attribute                 -- ooRexxUnit only


::method createResult      -- creates a default TestResult object
  return self~class~defaultTestResultClass~new

::method "countTestCases=" private  -- set method
  expose  fCountTestCases
  use arg fCountTestCases


::method countTestCases -- return nr. of test cases (methods) in this class
  expose fCountTestCases
  return fCountTestCases


::method run            -- will get implemented in subclasses
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
     .error~say( "... running TestCase object" pp(self~string) "..." )

  call doTheTest self, fName, aTestResult  -- carry out the testmethod
  self~tearDown                     -- make sure tearDown is invoked after test
  aTestResult~endTest(self)         -- remember test ended

  return aTestResult

doTheTest: procedure    -- make sure exceptions are trapped locally
   use arg self, strM, aTestResult

   -- user exception not interceptable at this stage anymore, hence using SYNTAX exceptions
   -- signal on user AssertionFailedError name exceptionHandler

   signal on any name exceptionHandler
   .message~new(self, strM)~send -- create the message object and send it

   self~check4ConditionFailure   -- check, if a condition was expected and if so, fail
   return aTestResult

exceptionHandler:

   co=condition("O")    -- get the condition directory object

   if self~conditionExpected=.true then
   do
       if self~checkCondition(co) then
          return aTestResult
   end
   condition=condition("C")      -- get the condition
   additional=condition("A")     -- get additional msg, if any

   strAdditional=""     -- message(s)
   if additional~class=.array then
   do
      items=additional~items
      do i=1 to items
         strAdditional=strAdditional || additional[i]
         if i<items then strAdditional=strAdditional || "0d0a"x  -- add a CRLF
      end
      strAdditional=strAdditional
   end

   if co~code=93.964 then     -- o.k. an own raised exception (using new ooRexx 3.1 application defined syntax error)
   do
      tmpString=pp(date("S") time("L"))":" pp("failure") self~string || iif(strAdditional="", "", " --->" strAdditional)
      co~setentry("OOREXXUNIT.CONDITION", tmpString)  -- add ooRexxUnit-infos with condition object
      aTestResult~addFailure(self, co)    -- save the condition object
   end
   else     -- unexpected/untested failure, ie. an "error"
   do
      tmpInfo=co~condition

      if co~hasentry("CODE") then
         tmpInfo=tmpInfo co~code

      tmpInfo="condition" pp(tmpInfo) "raised unexpectedly." || "09"x

      if .nil<>co~message then
         tmpInfo=tmpInfo || co~message   -- add error message

      tmpString=pp(date("S") time("L"))":" pp("error") self~string "--->" tmpInfo
      co~setentry("OOREXXUNIT.CONDITION", tmpString)  -- add ooRexxUnit-infos with condition object
      aTestResult~addError(self, co)      -- save the condition object
   end

   return aTestResult   -- rgf, 2006-04-08


::method getName     -- returns the name for this TestCase
  expose fName
  return fName

::method setName     -- set the name for this TestCase
  expose fName
  parse arg fName

::method string      -- create a string representation, counterpart to Java's toString()
  return  makeTestCaseString(self)


::method setUp       -- will get implemented in subclasses (allows to create a test-environment)
  NOP                -- indicate that emptiness is intended

::method tearDown    -- will get implemented in subclasses (allows to remove a test-environment)
  NOP                -- indicate that emptiness is intended




/* *********************************************************************************** */
/* *********************************************************************************** */
::class "TestSuite" subclass TestCase public
::method init
  expose fTestList

  forward class (super) continue
  fTestList=.queue~new

      -- a class object, use reflection and create test cases
  if arg()>0 then       -- args there ?
  do
     use strict arg arg1    -- make sure, only one arg supplied

     if isSubClassOf(arg1~class, "TestSuite") then -- a TestSuite object ?
     do
        self~addTest(arg1)                -- just add it
        return
     end

      -- a TestCase class object in hand?
     else if arg1~isA(.class), isSubClassOf(arg1, "TestCase") then
     do
        testCaseClass=arg1
        fTestMethods.=self~class~getTestMethods(testCaseClass)
        do i=1 to fTestMethods.0          -- iterate over all test methods
           self~addTest(testCaseClass~new(fTestMethods.i))
        end
        return
     end

      -- a collection of individual TestCase objects to add?
     else if arg1~isA(.Collection) then
     do
        do tc over arg1~makeArray         -- iterate over test cases
           if isSubClassOf(tc~class, "TestCase") then
              self~addTest(tc)
           else
              raise syntax 88.914 array ("'collection item ["tc"]'", "'TestCase' or 'TestSuite'")
        end
        return
     end
     raise syntax 88.914 array ("1", "'TestCase', 'TestSuite' or 'Collection'")
  end
  return

  /* each TestCase/TestSuite class object could be created by a different requires, causing
     different class objects to be created than are available in the context of this run */
::routine isSubClassOf
  use arg clz, superClassName

  do while clz<>.nil
     if clz~id~caselessEquals(superClassName)=1 then   -- o.k. found!
        return .true
     clz=clz~superClass    -- look further, get immediate superclass
  end
  return .false




::method getTestMethods class -- use reflection to retrieve testmethods, sort alphabetically
  use arg class

  fTestMethods.0=0      -- set index to 0
   -- now get the test methods, i.e. methods starting with "TEST"
  methSupplier=class~methods(.nil)-- only get methods of the receiver class (= testClass)
  do while methSupplier~available -- iterate over supplied methods
     name=methSupplier~index
     if name~left(4)~translate="TEST" then   -- a test method in hand
     do
        i=fTestMethods.0+1
        fTestMethods.i= name      -- index should be uppercase for sorting
        fTestMethods.0=i
     end
     methSupplier~next
  end

  call sysStemSort fTestMethods. -- sort test methods into ascending order
  return fTestMethods.           -- return stem



::method addTest
  expose fTestList
  use strict arg aTestCase

  if \isSubClassOf(aTestCase~class, "TestCase") then
     raise syntax 88.914 array ("'aTestCase'", "'TestCase' or 'TestSuite'")

  fTestList~queue(aTestCase)
  self~countTestCases = self~countTestCases+1


::method run
  expose fTestList
  use arg aTestResult, bGiveFeedback=.false

  if \datatype(bGiveFeedback,"O") then
     raise syntax 93.903 array (bGiveFeedback) -- raise error

   -- make sure an instance of .TestResult is used
  if arg(1, 'O') then aTestResult=self~createResult
  else aTestResult=arg(1)

  if bGiveFeedback then
     .error~say( "running testSuite" pp(self~string"@"self~identityHash) "with" pp(self~countTestCases) "test cases ...")

  aTestResult~startTest(self)       -- remember test started
  self~setUp                        -- make sure setup is invoked before testSuite runs
  do aTestCase over fTestList while aTestResult~shouldStop=.false
     aTestCase~run(aTestResult, bGiveFeedback)
  end
  self~tearDown                     -- make sure tearDown is invoked after testSuite ran
  aTestResult~endTest(self)         -- remember test ended

  return aTestResult


/* *********************************************************************************** */
/* *********************************************************************************** */
-- routines

/* *********************************************************************************** */
/* *********************************************************************************** */
::routine iif public -- utility routine
  if arg(1)=.true then return arg(2)
                  else return arg(3)


/* *********************************************************************************** */
/* *********************************************************************************** */
::routine pp public  -- "pretty print" ;) encloses string value in square brackets
  return "[" || arg(1)~string || "]"

/* *********************************************************************************** */
/* *********************************************************************************** */
/* encloses string value in square brackets, escapes non-printable chars as Rexx
   concatenated Rexx hex strings
*/
::routine ppp public  -- "printable pretty print" ;)
  parse arg string    -- retrieve string value of argument

  if verify(string, .chars.NonPrintable, "Match")>0 then
     return "[" || escapeString(string) || "]" -- escape non-printable characters

  return "[" || string || "]"

::routine escapeString        public   -- escape non-printable characters in string
  parse arg str
  tmpStr=.mutableBuffer~new

  do forever while str<>""
     start=verify(str, .chars.nonPrintable, "Match")
     if start>0 then    -- non-printing char found, look for printable char after it
     do
            -- find non-matching position, deduct one to point to last non-printable chars in string
        end=verify(str, .chars.nonPrintable, "Nomatch", start)-1
        if end=-1 then   -- no non-matching (=ending) position found: rest is non-printable
           end=length(str)

        if start>1 then -- printable chars before section with non-printable chars ?
        do
           chunk=enQuote(substr(str, 1, start-1))
           if tmpStr~length<>0 then tmpStr~~append(" || ")~~append(chunk)
                               else tmpStr~append(chunk)
        end

            -- extract non-printable chars, encode them as a Rexx hex string
        chunk=enQuote(substr(str, start, end-start+1)~c2x) || "x"

        if tmpStr~length<>0 then tmpStr~~append(" || ")~~append(chunk)
                            else tmpStr~append(chunk)

            -- extract non-processed part of string
        str=substr(str, end+1)   -- get remaining string
     end
     else   -- only printable chars available respectively left
     do
        if tmpStr~length<>0 then tmpStr~~append(" || ")~~append(enquote(str))
                            else tmpStr~append(str)
        leave         -- str=""
     end
  end
  return tmpStr~string

::routine enQuote             public
  return '"' || arg(1)~string || '"'


-- create
::routine formatObjectInfo public
  use arg o, hint
  if arg(2, 'omitted') then hint=""

  return pp(hint || ppp(o~string)", identityHash="enQuote(o~identityHash))


   /* function returning .true, if both collections can be regarded to be equal (i.e.,
      each can be regarded to be a subset of the other), .false else
   */
::routine IsCollEqual public
  use arg expected, received

  /* Starting with 3.2 (language level 6.02) no more guessing necessary, check for collection
     type and act accordingly.
  */
  if expected~isA(.Collection), received~isA(.Collection), -
     expected~subset(received), received~subset(expected) then
  do
     return .true -- the one collection is a subset of the other
  end

  return .false   -- not dealing with collections or collections not a subset of each other


/* *********************************************************************************** */
/* *********************************************************************************** */
::routine addN       -- if string starts with a vowel, then "n" is returned, "" else
  parse arg name

  if "aeiou"~caselessPos(name~left(1))>0 then return "n"
  return ""

/* *********************************************************************************** */
/* *********************************************************************************** */
::routine makeTestCaseString -- string to represent an instance of a TestCase
  use arg aTestCase
  className=aTestCase~class~id   -- get class name
  return "testCase:" pp(aTestCase~getName) "(a" || addN(className) className || "@" || -
                 aTestCase~definedInFile")"


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
::routine makeDirTestInfo public
   use arg aTestCaseClass, arrLines

   tmpDir=aTestCaseClass~TestCaseInfo   -- get directory object to add infos to

   keyWord=""
   tOut=xrange("A","Z")||xrange("a","z")
   tIn =xrange("A","Z")||xrange("a","z")||xrange()

   do i=1 to arrLines~items while arrLines[i]<>"*/"
      if arrLines[i]~strip~left(2)="--" then iterate    -- ignore comment

         -- a keyWord already set and this line has no new keyword, than append it
      if pos(":", arrLines[i])=0 then
      do
         if keyWord<>"" then  -- alreay a keyWord found, append line to it
         do
            tmpDir~entry(keyWord)~queue(arrLines[i])
         end
         iterate
      end


      parse value arrLines[i] with name ":" rest

      keyWord=name~translate(tOut, tIn)~space(0)   -- a keyWord change ?

      if tmpDir~hasEntry(keyWord)=.false then
         tmpDir~setentry(keyWord, .queue~new)      -- create a new queue for this keyword

      tmpDir~entry(keyWord)~queue(rest~strip)      -- add line
   end



   -- simple dumping of the testResult data
::routine simpleDumpTestResults public
   use arg aTestResult, title

   if arg()>1 & title<>"" then
   do
      say title
      say
   end

   say "nr of test runs:            " aTestResult~runCount
   say "nr of successful assertions:" aTestResult~assertCount

   say "nr of failures:             " aTestResult~failureCount
   if aTestResult~failureCount>0 then
   do
      do co over aTestResult~failures
         say "  " co~ooRexxUnit.condition
      end
   end

   say "nr of errors:               " aTestResult~errorCount
   if aTestResult~errorCount>0 then
   do
      do co over aTestResult~errors
         say "  " co~ooRexxUnit.condition
      end
   end


   -- create a testSuite object by calling the supplied testCaseFileList; needs testCase programs
   -- modelled after the example programs
::routine makeTestSuiteFromFileList public
   use arg testCaseFileList, ts

   if arg(2, "Omitted") then  -- no TestSuite object supplied?
      ts=.testSuite~new

      -- make sure, that the tests are not run when CALLing/REQUIRE'ing the testUnit programs
   .local~bRunTestsLocally=.false   -- do not run tests, if calling/requiring the testUnit files

   do fileName over testCaseFileList
/*
       call (fileName)        -- call file
       testUnitList=result    -- retrieve result (a list of array objects)
*/
       testUnitList=callTestUnit(fileName)

       do arr over testUnitList  -- loop over array objects
          classObject   =arr[1]  -- a class object
          mandatoryTests=arr[2]  -- a list

          -- check whether mandatory tests are defined
          bMandatoryTests=(.nil<>arr[2])
          if bMandatoryTests=.true then   -- o.k. not .nil in hand
          do
             bMandatoryTests=(.list=mandatoryTests~class)   -- is there a list in hand
             if bMandatoryTests then
             do
                bMandatoryTests=(mandatoryTests~items>0)    -- are there any entries?
             end
          end

          if bMandatoryTests then   -- mandatory tests available, just use them to create testCases
          do
            tsMand=.testSuite~new     -- create a test suite for this test class
            do testMethodName over mandatoryTests
               tsMand~addTest( classObject~new(testMethodName) )   -- create and add testCase
            end
            ts~addTest(tsMand)      -- now add the test suite of mandatory methods to the overall test suite
          end
          else    -- no mandatory tests defined, hence use all testmethods
          do
             ts~addTest(.testSuite~new(classObject))  -- creates testCases from all testmethods
          end
       end
   end
   return ts      -- return the testSuite object


callTestUnit: procedure -- rgf, 2007-04-28: possible, that a ::requires causes program to fail
   parse arg fileName
   signal on syntax
   call (fileName)      -- call file
   return result        -- return its return value

syntax:
   .error~say("ooRexxUnit's routine 'makeTestSuiteFromFileList', 'callTestUnit()':")
   .error~say("    testUnit-file: ["fileName"]")
   .error~say(ooRexxUnit.formatConditionObject(condition("O")))

   return .array~new    -- return empty array so no testsuite gets built for this testUnit



/* Determine and return the shell name to be used in ADDRESS keyword statements. This
   is a central location to make it easy to maintain in the future, in case a non-Windows
   and non-Unix compatible operating systems comes up, or new shell variants develop.
*/
::routine ooRexxUnit.getShellName public

   parse upper source opsys +1

      -- make sure we address the shell
   shell="CMD"          -- default to the OS2/Windows shell
   if pos(opsys, "WO")=0 then    -- if not running under OS/2 or Windows assume Unix
   do
      unixShell=value("SHELL", , "environment")    -- get the fully qualified shell
      shell=substr(unixShell, 1+lastpos("/", unixShell))    -- extract name of shell
   end

   return shell

/* Determine and return the operating system under which the currently executing
   program is running.  Provides an uniform method for test cases to determine
   which operating system they are executing on.

   The possible list of return words as of ooRexx 3.2.0 is:

     WINDOWS, LINUX, SUNOS, AIX, MACOSX, CYGNUS, FREEBSD, and NETBSD

   Although it is doubtful that ooRexx is compilable on CYGNUS, FREEBSD, or
   NETBSD.
*/
::routine ooRexxUnit.getOSName public

  parse upper source os .
  if os~abbrev("WIN") then os = "WINDOWS"
  return os



/* Returns a string containing the directory entries in sorted order. In case an entry
   is a colleciton itself, its elements will be returned as strings.
*/
::routine ooRexxUnit.formatConditionObject public  /* dump condition object */
   use arg co

   arr=co~allIndexes~~stableSortWith(.CaselessComparator~new)
   len=length(arr~items)
   indent1=12
   sumIndent=len+indent1+2

   blanks=copies(" ", sumIndent) "--> "
   res=.mutableBuffer~new

   NL="0a"x
   TAB1="09"x
   TAB2=TAB1~copies(2)
   TAB3=TAB1~copies(3)
   TAB4=TAB1~copies(4)

   do index over arr
      o=co~entry(index)
      if o~isA(.Collection) then items=o~items
                            else items=""

      tmpString=.MutableBuffer~new

      tmpString~~append(TAB3) ~~append(index~left(indent1,".")) ~~append(pp(o))
      if items<>"" then tmpString~~append(" containing ") ~~append(items) ~~append(" item(s)")

      if res~length=0 then    -- first value to assign
      do
         res~~append(tmpString~string)
      end
      else                    -- value already available
      do
         res~~append(NL) ~~append(tmpString~string)
      end

      if items<>"" then       -- a collection object in hand?
      do
         nr=0
         do item over o       -- list items
            nr=nr+1
            res~~append(NL) ~~append(TAB3) ~~append(blanks) ~~append(pp(item))
         end

         res ~~append(TAB3)
      end
   end
   return res~string


pp: procedure
   use arg a
   if .nil=a then return "[.nil]"
   return "[" || a~string || "]"


/**
 * Public routine simpleFormatTestResults()
 *
 *  Variation on simpleDumpTestResults() that formats the result output in a
 *  more "console-friendly" manner.  The information is broken up into lines,
 *  with an attempt made to keep all lines no longer than 80 characers wide.
 */
::routine simpleFormatTestResults public
   use arg aTestResult, title

   if arg()>1 & title<>"" then do
      say title
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

   if aTestResult~failureCount > 0 then do co over aTestResult~failures
      z = printFailureInfo(co~ooRexxUnit.condition)
   end

   if aTestResult~errorCount > 0 then do co over aTestResult~errors
      z = printErrorInfo(co~ooRexxUnit.condition)
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

return 0

/**
 * Private routine printFailureInfo()
 *
 *  Helper routine for simpleFormatTestResults()
 */
::routine printFailureInfo
  use arg co

  parse var co dateTime "[failure]" idStuff "--->" additionalStuff
  dateTime = dateTime~strip~strip('T', ':')

  idTable = decomposeTestCaseID(idStuff)
  additional = decomposeAdditional(additionalStuff)

  say "[failure]" dateTime
  say "  Test:  "  idTable['name']
  say "  Class: "  idTable['class']
  say "  File:  "  idTable['file']
  if additional~items > 0 then ret = printAssertInfo(additional)
  say

return 0

/**
 * Private routine printErrorInfo()
 *
 *  Helper routine for simpleFormatTestResults()
 */
::routine printErrorInfo
  use arg co

  parse var co dateTime "[error]" idStuff "--->" additionalStuff
  dateTime = dateTime~strip~strip('T', ':')

  idTable = decomposeTestCaseID(idStuff)
  additional = decomposeAdditional(additionalStuff)

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

return 0

/**
 * Private routine decomposeTestCaseID()
 *
 *  Helper routine for simpleFormatTestResults()
 */
::routine decomposeTestCaseID
  use arg id

  id = id~strip
  parse var id tcWord tcName lastPart
  tcName = tcName~substr(2, tcName~length - 2)
  parse var lastPart cls "@" file ")"
  clsName = cls~word(2)

  if file~length > 70 then file = pathCompact(file, 70, .ooRexxUnit.directory.separator)
  t = .table~new
  t['name']  = tcName
  t['class'] = clsName
  t['file']  = file

return t

/**
 * Private routine decomposeAdditional()
 *
 *  Helper routine for simpleFormatTestResults()
 */
::routine decomposeAdditional
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

/**
 * Private routine printAssertInfo()
 *
 *  Helper routine for simpleFormatTestResults()
 */
::routine printAssertInfo
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

return 0

/* pathCompact( path, len, sl )- - - - - - - - - - - - - - - - - - - - - - - -*\

  Takes a long path name and compacts it to a shorter path name by removing some
  path components in the middle of the path name and replacing them with
  ellipses (...)

  Input:
    path REQUIRED
      The path name to compact.

    len  REQUIRED
      The maximum length of the returned compacted path.

    sl   REQUIRED
      The path component separator character, i.e., a backward slash on Windows.

  Returns:
    A compacted path no longer than specified.  If the input path name is
    already less than or equal to the specified length, it is returned
    unchanged.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::routine pathCompact public
  use arg path, len, sl

  if path~length <= len then return path

  -- Get position of directory separator after character 15
  fp = path~pos(sl, 15)

  -- Divide into leading and trailing pieces.  Leading piece, add "..."
  leading = path~left(fp) || "..."
  trailing = path~substr(fp)

  -- Find a position of the directory separator in the trailing part, such that
  -- when the trailing part starts there, the overall length of the resulting
  -- string is the specified length, or shorter.
  trailPos = trailing~pos(sl, trailing~length - (len - leading~length))
  if trailPos == 0 then do
    -- The last component of the path is too long, shorten the leading.
    leading = "..."
    trailPos = trailing~pos(sl, trailing~length - (len - leading~length))
  end

  if trailPos == 0 then do
    -- Still too long, just use the last len characters.
    leading = ""
    trailing = trailing~right(len)
  end
  else do
    trailing = trailing~substr(trailPos)
  end

return leading || trailing
-- End pathCompact( path, len, sl )

