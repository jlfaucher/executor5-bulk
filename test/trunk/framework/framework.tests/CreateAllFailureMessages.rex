#!/usr/bin/rexx
/*
   name:             ooRexx.framework.CreateAllFailureMessages.rex
   author:           Rony G. Flatscher
   date:             2006-12-28
   version:          1.0.0

-- line commented lines are ignored, when building the directory of infos from this header
   changed:

   languageLevel:    6.0
   purpose:          Cause all failure messages to occur
   remark:           ---


   license:          CPL 1.0 (Common Public License v1.0, see below)
   link:

   category1:        ooRexx
   category2:        framework
*/
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2006 Rexx Language Association. All rights reserved.         */
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



-------------------------------------------------------------------------------------------
-- ===> adapt the "testUnitList" to your testCase classes; each element in the list is <===
-- ===> an array object, the first element containing the testCase class object, the   <===
-- ===> second element is a list of test method names which are regarded to be         <===
-- ===> mandatory (if the list remains empty all test methods are mandatory)           <===

   /* list of array objects, each containing the testUnit class object and an
      optional list of mandatory test case methods name                       */

mandatoryTestMethods=.list~new   -- no mandatory tests for this testCase class
testUnitList=.list~of( .array~of(.ooRexx.framework.CreateAllFailureMessages.rex,  mandatoryTestMethods) )


-------------------------------------------------------------------------------------------
-- ===> the following code needs not to be individualized                              <===

   -- read top comment, containing infos about this program
arrLines=.array~new
do i=1 to 150 until arrLines[i]="*/"
   arrLines[i]=sourceline(i)
end
      -- supply information for the testClass(es) in this file; the class attribute
      -- "TestCaseInfo" (a directory object, index points to a queue) will store
      -- the parsed infos
aTestUnitClass=testUnitList~at(testUnitList~first)[1] -- get first testClass

   -- will parse the array lines and store result in class object
call makeDirTestInfo aTestUnitClass, arrLines
tmpDir=aTestUnitClass~TestCaseInfo
parse source s   -- op_sys invocationType fullPathToThisFile
tmpDir~setentry("test_Case-source", s)

   -- now add this directory to other testCase classes, if any left
do arr over testUnitList
   if arr[1]=aTestUnitClass then iterate  -- already handled
   arr[1]~TestCaseInfo=tmpDir             -- save info in class object
end

-- if this file is CALLed or REQUIRED then define an entry "bRunTestLocally" in .local
-- and set it to .false; this way the independent local invocation of the tests is inhibited
if .local~hasentry("bRunTestsLocally")=.false then
   .local~bRunTestsLocally=.true -- if this file is executed directly, then run tests for debugging

if .bRunTestsLocally=.true then  -- run ALL tests in this test unit
do
   ts=.testSuite~new             -- create a testSuite
   do arr over testUnitList
      -- create a testSuite for the given test case class, use all its testmethods
      ts~addTest( .testSuite~new(arr[1]))
   end
   -- testResult=.testSuite~new(testUnitClass)~execute
   testResult=ts~execute       -- now run all the tests

   call simpleDumpTestResults testResult
end

   /* return list of array objects containing test case classes and
      optionally list of mandatory test methods                      */
return testUnitList



::requires ooRexxUnit.cls     -- load the ooRexxUnit classes

::class "ooRexx.framework.CreateAllFailureMessages.rex" subclass TestCase public


/* *** test methods which create failures and demonstrate the
       resulting failure messages */
::method "test_fail_assertEquals_01"
   self~assertEquals(12, 13)

::method "test_fail_assertEquals_02"
   self~assertEquals(12, 13, "A message in case of a failure.")


::method "test_fail_assertFalse_01"
   self~assertFalse(.true)

::method "test_fail_assertFalse_02"
   self~assertFalse(.true, "A message in case of a failure.")


::method "test_fail_assertNotEquals_01"
   self~assertNotEquals(12, 12)

::method "test_fail_assertNotEquals_02"
   self~assertNotEquals(12, 12, "A message in case of a failure.")


::method "test_fail_assertNotNull_01"
   self~assertNotNull(.nil)

::method "test_fail_assertNotNull_02"
   self~assertNotNull( .nil, "A message in case of a failure.")


::method "test_fail_assertNotSame_01"
   a=12
   self~assertNotSame(a, a)

::method "test_fail_assertNotSame_02"
   a=12
   self~assertNotSame(a, a, "A message in case of a failure.")


::method "test_fail_assertNull_01"
   self~assertNull(12)

::method "test_fail_assertNull_02"
   self~assertNull(12, "A message in case of a failure.")


::method "test_fail_assertSame_01"
   self~assertSame(12, " 12")

::method "test_fail_assertSame_02"
   self~assertSame(12, "12 ", "A message in case of a failure.")


::method "test_fail_assertTrue_01"
   self~assertTrue(.false)

::method "test_fail_assertTrue_02"
   self~assertTrue(.false, "A message in case of a failure.")


::method "test_fail_expectSyntax"
   self~expectSyntax(13.1)          -- but do not raise it!


::method "test_fail_expectCondition"
   self~expectCondition("NOVALUE")  -- but do not raise it!


::method "test_fail_unexpectedSyntax"
   raise syntax 13.1 array("xyz", "01")


::method "test_fail_expectedCondition"
   raise novalue


