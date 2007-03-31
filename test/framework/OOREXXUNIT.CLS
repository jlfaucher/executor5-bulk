#!/usr/bin/rexx
/*
-- this is a ooRexx line comment, will get ignored by pre-processor which analyzes this header
-- enclosed between the very first block comment, i.e. everything between the first "/*" .. "*/"

   name:             ooRexxUnit.cls
   author:           Rony G. Flatscher, Rick McGuire
   date:             2005-08-07
   version:          1.0.2
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


   language-level:   6.0
   needs:            ooRexx 3.1 or later (introduced syntax error # 93.964, which is exploited)
-- determines the minimum ooRexx language level (6.00 = ooRexx, IBM Object REXX)

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

.local~chars.NonPrintable=xrange("00"x, "1F"x) || "FF"x  -- define non-printable chars
.local~ooRexxUnit.version=102.20070124    -- define the version number


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

-- ::method TestCaseTable attribute private
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
  expose fName fCountTestCases   -- name of Testcase (method) to carry out
  parse arg fName

  fCountTestCases=1  -- default: individual test
  self~TestCaseInfo=.directory~new -- directory to contain information on test

  self~init:super    -- let superclass initialize


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
  use arg aTestResult

   -- make sure an instance of .TestResult is used
  if arg()=0 then aTestResult=self~createResult
             else aTestResult=arg(1)

  aTestResult~startTest(self)       -- remember test started
  self~setUp                        -- make sure setup is invoked before test
  call doTheTest self, fName, aTestResult  -- carry out the testmethod
  self~tearDown                     -- make sure tearDown is invoked after test
  aTestResult~endTest(self)         -- remember test ended

  return aTestResult

doTheTest: procedure    -- make sure exceptions are trapped locally
   use arg self, strM, aTestResult

   -- user exception not interceptable at this stage anymore, hence using SYNTAX exceptions
   -- signal on user AssertionFailedError name exceptionHandler

  signal on any name exceptionHandler
/*
   -- signal on any
   -- signal on novalue
   signal on error      name exceptionHandler
   signal on failure    name exceptionHandler
   signal on halt       name exceptionHandler
   signal on nomethod   name exceptionHandler
   -- signal on nostring   name exceptionHandler
   signal on notready   name exceptionHandler
   signal on syntax     name exceptionHandler
*/

-- say "self="pp(self) "strM="pp(strM)"..." "self~hasMethod(strM)="self~hasMethod(strM)

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
      -- tmpString=pp(date("S") time("L"))":" pp("failure")~left(11) self~string || iif(strAdditional="", "", " --->" strAdditional)
      tmpString=pp(date("S") time("L"))":" pp("failure") self~string || iif(strAdditional="", "", " --->" strAdditional)
      co~setentry("OOREXXUNIT.CONDITION", tmpString)  -- add ooRexxUnit-infos with condition object
      aTestResult~addFailure(self, co)    -- save the condition object
   end
   else     -- unexpected/untested failure, ie. an "error"
   do
      tmpInfo=co~condition

      -- if tmpInfo="SYNTAX" then tmpInfo=tmpInfo "ERROR"

      if co~hasentry("CODE") then
         tmpInfo=tmpInfo co~code

      tmpInfo="condition" pp(tmpInfo) "raised unexpectedly." || "09"x

      if .nil<>co~message then
         tmpInfo=tmpInfo || co~message   -- add error message

      -- tmpString=pp(date("S") time("L"))":" pp("error")~left(11) self~string "--->" tmpInfo
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
  if arg(1)~class=.class then
  do
     testCaseClass=arg(1)
     fTestMethods.=self~class~getTestMethods(testCaseClass)
     do i=1 to fTestMethods.0
        self~addTest(testCaseClass~new(fTestMethods.i))
     end
  end

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
  use arg aTestCase
  fTestList~queue(aTestCase)
  self~countTestCases = self~countTestCases+1


::method run
  expose fTestList
  use arg aTestResult

   -- make sure an instance of .TestResult is used
  if arg()=0 then aTestResult=self~createResult
             else aTestResult=arg(1)

  aTestResult~startTest(self)       -- remember test started
  self~setUp                        -- make sure setup is invoked before testSuite runs
  do aTestCase over fTestList while aTestResult~shouldStop=.false
     aTestCase~run(aTestResult)
  end
  self~tearDown                     -- make sure tearDown is invoked after testSuite ran
  aTestResult~endTest(self)         -- remember test ended

  return aTestResult

::method countTestCases -- return nr. of test cases (methods) in this class
  expose fTestList
  return fTestList~items




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

-- enQuote: procedure
::routine enQuote             public
  return '"' || arg(1) || '"'

/*
-- return hash value as a Rexx hex literal
::routine getEscapedhashValue  public
  use arg o
  return enQuote(o~"=="~c2x)"x"
*/

-- create
::routine formatObjectInfo public
  use arg o, hint
  if arg(2, 'omitted') then hint=""

  return pp(hint || ppp(o~string)", hashValue="enQuote(o~"=="~c2x)"x")

-- ppp(arg(1)~string "<hashValue:" getEscapedhashValue(arg(1))">")



   /* function returning .true, if both collections can be regarded to be the same,
      .false else
      logic: - if both values have the method SUBSET, assume unordered collection;
               proceed with test, if both values also possess the method SUPPLIER;
               each value can be of any collection type: returns .true only, if the
               same number of the same index/value pairs is present in both collections,
               .false else
               [there is a short-circuit logic if either value is of .set/.bag type
                in which case the test for equality is carried out directly with the
                help of the SUBSET method]

             - if above does not apply, but both values have the MAKEARRAY method assume
               an ordered collection: values of MAKEARRAY must occur in the same sequence
               (and both need to have the same number of items) to return .true, .false else
   */
::routine IsCollEqual public
  use arg expected, received

      -- both collections with "SUPPLIER", if so, sequence should not matter
  if expected~hasmethod("SUBSET") & received~hasmethod("SUBSET") then
  do
     if expected~hasmethod("SUPPLIER") & received~hasmethod("SUPPLIER") then
     do
         -- if both are either of type .set and/or .bag, then test directly with SUBSET for equality
        mS=.set~of(.set, .bag)
        if mS~hasindex(expected~class) & mS~hasindex(received~class) then
        do
           if expected~subset(received) then       -- equal, if each is a subset of the other
           do
              if received~subset(expected) then
                 return .true
           end

           return .false
        end

            /* create a bag rendering of both collections such, that the resulting
               bags can be used to test whether each is a subset of the other, hence
               can be regarded to be the same collection; the encoding takes the hex-value
               of the index object and concatenates it with a blank with the hex-value of
               the item object, creating a uniquely identifiable bag element
            */
        eBag=.bag~new               -- create a bag rendering for the expected value
        eSupp=expected~supplier
        do while eSupp~available
           eBag~put( eSupp~index~"==" eSupp~item~"==" )
           eSupp~next
        end

        rBag=.bag~new               -- create a bag rendering for the received value
        rSupp=received~supplier
        do while rSupp~available
           rBag~put( rSupp~index~"==" rSupp~item~"==" )
           rSupp~next
        end

            /* test whether both collections can be regarded to be the same: if both bags are
               subsets of each other, both values are regarded to be the same
            */
        if eBag~subset(rBag) then
        do
           if rBag~subset(eBag) then
              return .true
        end
        return .false
     end
  end

      -- maybe both collections with MAKEARRAY, if so, sequence matters
  if expected~hasmethod("MAKEARRAY") & received~hasmethod("MAKEARRAY") then
  do
     eArr=expected~makearray
     rArr=received~makearray
     if eArr~items<>rArr~items then
        return .false

     do i=1 to eArr~items
        if eArr[i]<>rArr[i] then
           return .false
     end
     return .true
  end

  return .false




/* *********************************************************************************** */
/* *********************************************************************************** */
::routine addN       -- if string starts with a vowel, then "n" is returned, "" else
  parse arg name

  if pos(name~left(1), "aeiouAEIOU")>0 then return "n"
  return ""

/* *********************************************************************************** */
/* *********************************************************************************** */
::routine makeTestCaseString -- string to represent an instance of a TestCase
  use arg aTestCase
  className=aTestCase~class~id   -- get class name
  return "testCase:" pp(aTestCase~getName) "(a" || addN(className) className || "@" || -
                 aTestCase~"=="~c2x")"


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
       call (fileName)        -- call file
       testUnitList=result    -- retrieve result (a list of array objects)
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

   Returns: WINDOWS, LINUX, SUNOS, or AIX.  (As of ooRexx 3.1.2)
*/
::routine ooRexxUnit.getOSName public

  parse upper source os .
  if os~abbrev("WIN") then
    return "WINDOWS"

  return os
