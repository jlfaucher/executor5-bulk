/*
   name:             aNutshellTestCase.rex
   author:           Rony G. Flatscher
   date:             2006-04-08, Vienna Airport
   version:          0.0.1
   languageLevel:    6.0
   purpose:          Demonstrates how a TestUnit class with TestCase methods looks like
                     and how one can instantiate and run individual TestCase methods,
                     dumping the logged results

   license:          CPL 1.0 (Common Public License v1.0, see below)
*/

   /* create a TestResult object which logs the test results               */
aTR=.TestResult~new

   /* now create TestCase instances, supplying the TestCase method name to be run,
      using the TestResult object 'aTR' to log the results of the tests    */
.NutshellTestUnit~new('test.ABBREV')~run(aTR)   /* this should execute o.k.*/

    /* the following TestCase method should fail on the identity test      */
.NutshellTestUnit~new('someTestCaseMethod')~run(aTR)

   /* the following attempt to run a TestCase method should cause an error,
      as there is no TestCase method by the name of 'this does not exist!!'*/
.NutshellTestUnit~new('this does not exist!!')~run(aTR)

.NutshellTestUnit~new('test.ABS')~run(aTR)      /* this should execute o.k.*/

   /* now dump the logged information about the TestCase results as
      logged in the TestResult object 'aTR'                                */
call simpleDumpTestResults aTR


::requires ooRexxUnit.cls     -- load the ooRexxUnit classes

::class "NutshellTestUnit" subclass TestCase public

   /* a TestCase method: test the ABBREV BIF, using examples from the documentation    */
::method "test.ABBREV"        -- a TestCase method

   word="Print"
   self~assertEquals("subTest1", ABBREV(word, "Pri"),    .true)

   word="PRINT"
   self~assertEquals("subTest2", ABBREV(word, "Pri"),    .false)
   self~assertEquals("subTest3", ABBREV(word, "PRI", 4), .false)
   self~assertEquals("subTest4", ABBREV(word, "PRY"),    .false)
   self~assertEquals("subTest5", ABBREV(word, ""),       .true)
   self~assertEquals("subTest6", ABBREV(word, "", 1),    .false)


   /* a TestCase method:  test the ABBREV BIF, using examples from the documentation   */
::method "test.ABS"           -- a TestCase method

   self~assertEquals("subTest1",  ABS('12.3'),    12.3)
   self~assertEquals("subTest2",  ABS(' -0.307'), 0.307)

      -- new tests
   self~assertEquals("subTest3",  ABS(' -0'),   0)
   self~assertEquals("subTest4",  ABS(' +0'),   0)
   self~assertEquals("subTest5",  ABS(' - 0 '), 0)
   self~assertEquals("subTest6",  ABS(' + 0 '), 0)
   self~assertEquals("subTest7",  ABS(' -1'),   1)
   self~assertEquals("subTest8",  ABS(' +1'),   1)
   self~assertEquals("subTest9",  ABS(' - 1 '), 1)
   self~assertEquals("subTest10", ABS(' + 1 '), 1)


   /* a TestCase method:  test for equality and identity (which should fail)           */
::method "someTestCaseMethod"
   a=" RexxLA   "
   b="RexxLA"
   self~assertEquals("testing for equality using 'assertEquals()'...", a, b)
   self~assertSame("testing for identity using 'assertSame()'...", a, b)



/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2005 Rexx Language Association. All rights reserved.         */
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
