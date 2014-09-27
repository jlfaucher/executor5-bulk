#!/usr/bin/rexx
/*
   name:             firstOoRexxUnitTest.rex
   author:           Rony G. Flatscher
   date:             2005-08-07
   version:          1.0.2

-- line commented lines are ignored, when building the directory of infos from this header
   changed1:         2005-08-07, ---rgf, adopted to having a superclass "Assert"
                     2005-08-20, ---rgf, added CPL 1.0 license

   languageLevel:    6.0
   purpose:          Initial test of the new ooRexxUnit-framework. Demos how to create
                     TestCase classes and a home-grown TestResult class ("myTestResult");
                     creates testSuites which automatically incorporate all test methods
                     (starting with the string "test"), creates testSuites containing
                     individual testCases, dumping the test results using an object from
                     "myTestResult"

   remark:           Needs further testing.
   license:          CPL 1.0 (Common Public License v1.0, see below)

   link:             http://www.junit.org
   link2:            http://junit.sourceforge.net/doc/cookbook/cookbook.htm
   link3:            http://junit.sourceforge.net/doc/cookstour/cookstour.htm

   category1:        ooRexxUnit
   category2:        test
   category3:        rubbish
*/
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2005-2009 Rexx Language Association. All rights reserved.    */
/*                                                                            */
/* This program and the accompanying materials are made available under       */
/* the terms of the Common Public License v1.0 which accompanies this         */
/* distribution. A copy is also available at the following address:           */
/* http://www.oorexx.org/license.html                                         */
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


   -- read top comment, containing infos about this program
arrLines=.array~new
do i=1 to 150 until arrLines[i]="*/"
   arrLines[i]=sourceline(i)
end

      -- supply information for the testClass "rgfAssertionTests",
      -- class attribute "TestCaseInfo" (a directory object, index points to a queue)
call makeDirTestInfo .rgfAssertionTests, arrLines

say copies("/// \\\", 10)
call dumpTestClassInfos    -- show parsing result, which got stored in the
say copies("\\\ ///", 10)

   /* use all methods starting with "test" for creating testCases */
ts1=.testSuite~new(.rgfAssertionTests)  -- create testCases from class object
say "TestSuite 'ts1' consists of" pp(ts1~countTestCases) "testCase(s)."
mtr1=ts1~execute(.myTestResult~new)
mtr1~dumpResults
mtr1~dumpLog

   /* use only two specific test methods  */
ts2=.testSuite~new
ts2~addTest( .rgfAssertionTests~new("testAssertTrue1") ) -- individual test case
ts2~addTest( .rgfAssertionTests~new("testAssertTrue3") ) -- individual test case
say "TestSuite 'ts2' consists of" pp(ts2~countTestCases) "testCase(s)."
ts2~execute(.myTestResult~new)~dumpResults

   /* use only two specific test methods  */
ts3=.testSuite~new
ts3~addTest( .rgfAssertionTests~new("testAssertTrue2") ) -- individual test case
ts3~addTest( .rgfAssertionTests~new("testAssertTrue4") ) -- individual test case
say "TestSuite 'ts3' consists of" pp(ts3~countTestCases) "testCase(s)."
ts3~execute(.myTestResult~new)~dumpResults

   /* use only two specific test methods  */
ts4=.testSuite~new
ts4~addTest( .rgfAssertionTests~new("testAssertTrue1") ) -- individual test case
ts4~addTest( .rgfAssertionTests~new("testAssertTrue2") ) -- individual test case
say "TestSuite 'ts4' consists of" pp(ts4~countTestCases) "testCase(s)."
ts4~execute(.myTestResult~new)~dumpResults

   /* just for the fun of it: use a single test from the test class,
      do not supply a TestResult object, hence a default one will be created and used */
.testSuite~new~~addTest(.rgfAssertionTests~new("testAssertTrue4"))~execute


   -- test executing a single testCase and via a test suite
ts=.testSuite~new                            -- create a test suite

tc=.rgfAssertionTests~new("testAssertTrue4") -- create a testcase
tc~execute                                       -- run this individual testcase


ts~addTest(.rgfAssertionTests~new("testAssertTrue2"))  -- create and add a testCase to suite
ts~addTest(tc)                               -- add the previously created testCase to suite
myTr=.myTestResult~new                       -- create explicitly a myTestResult object
ts~execute(myTr)                                 -- run the suite using "myTr" for the results
-- raise syntax 40.1 array ("--- stop! ---")

myTr~dumpLog            -- dump in chronological order the execution of testCases and their results
myTr~dumpResults        -- dump failures and errors, if any
myTr~dumpTestCases      -- dump each testcase results individually (in chronological order)



::requires ooRexxUnit.cls

/* ************************************************************************************** */
/* ************************************************************************************** */
::class "rgfAssertionTests" subclass TestCase public

::method testAssertTrue1
  -- could be any code to test something
  say "testAssertionTrue1: begin"
  self~assertTrue( .true )
  say "testAssertionTrue1: end"
  say

::method "testAssertTrue2"
  -- could be any code to test something
  say "testAssertionTrue2: begin"
  self~assertTrue( .true, "oopsla!" )
  say "testAssertionTrue2: end"
  say

::method testAssertTrue3
  -- could be any code to test something
  say "testAssertionTrue3: begin"
  self~assertTrue( .false )
  say "testAssertionTrue3: end"
  say

::method testAssertTrue4
  -- could be any code to test something
  say "testAssertionTrue4: begin"
  self~assertTrue( .false,  "should be .true, yet it is .false!" )
  say "testAssertionTrue4: end"
  say


/* ************************************************************************************** */
/* ************************************************************************************** */
::class myTestResult subclass TestResult

::method dumpResults
  say copies("-/**\-",10)
 say center(" myTestResult - results: ", 79, "-")
 say
 say "# successful assertions:" self~assertCount
 say
 say "# errors:  " self~errorCount
 do item over self~errors
    say "    error:" item~testName 'class:' pp(dir~type) "raised unexpectedly."
 end

 say "# failures:" self~failureCount
 do item over self~failures
    say "  failure:" item~testName 'expected:' item~expected 'actual:' item~actual
 end
 say
 say "tests were" iif(self~wasSuccessful, ":-) successful!", ":-((   NOT successful!")
 say copies("-", 79)

 say copies("=", 79)

::method dumpLog
  say copies("*--**",15)
  say center(" myTestResult - dumpLog ", 79, "-")
  do dir over self~logQueue
     select
       when dir~isA(.Directory) then say     "   " pp(dir~ooRexxUnit.condition)
       when dir~isA(.AssertFailure) then say "    Failed code:" dir~conditionObject~code 'at line' dir~line
       when dir~isA(.ErrorReport) then say   "    Error test: " dir~testName pp(dir~type) "raised unexpectedly."
       otherwise nop
     end
     -- End select
  end

::method dumpTestCases
  say copies("*/\**",15)
   tmpTable=self~TestCaseTable
   do testCase over tmpTable
      say "TestCase:" pp(testCase)
      do line over  tmpTable[testCase]
         say "   " line
      end
      say
   end



::routine dumpTestClassInfos
    say "Information about" pp(.rgfAssertionTests~string)":"
    say
    tmpDir=.rgfAssertionTests~TestCaseInfo
    orderedKeyword="name author date version changed languageLevel purpose remark link category"
    do i=1 to words(orderedKeyword)
       idx=word(orderedKeyword, i)
       say "keyWord="pp(idx)":"
       dir=tmpDir~entry(idx)
       if .nil<>dir then
       do line over dir
          say "    " pp(line)
       end
    end
    say


