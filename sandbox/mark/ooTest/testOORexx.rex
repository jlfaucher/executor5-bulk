#!/usr/bin/rexx
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

/**
 * testOORexx is a planned future replacement for runTestGroups.  It is not
 * fully implemented.
 */
cmdLine = arg(1)

   parse source . . file
   overallPhase = .PhaseReport~new(file, .PhaseReport~AUTOMATED_TEST_PHASE)
   searchPhase  = .PhaseReport~new(file, .PhaseReport~FILE_SEARCH_PHASE)

   .local~bRunTestsLocally = .false

   baseDir = "ooRexx"
   --baseDir = "ooRexx\samples"

   -- Set types to .nil to execute all types of tests.
   types = .nil


   -- Here, execute all test types, except the UNIT_LONG and the GUI_SAMPLE types.
   excluded = .set~of(.ooTestTypes~UNIT_LONG_TEST, .ooTestTypes~GUI_SAMPLE_TEST)
   types = .ooTestTypes~all~difference(excluded)

   /*
   say 'Will execute tests of type:'
   do type over types
     say ' ' .ooTestTypes~nameForTest(type)
   end
   */

   testResult = .ooRexxUnit.default.TestResult.Class~new
   testResult~noAutoTiming

   -- Set verbosity from 0 (least output) to 10 (most output)
   testResult~setVerbosity(0)


   finder = .ooTestFinder~new(baseDir, ".testGroup", types)
   containers = finder~seek(testResult); say 'containers items:' containers~items

   testResult~addEvent(searchPhase~~done)
   suiteBuildPhase  = .PhaseReport~new(file, .PhaseReport~SUITE_BUILD_PHASE)

   suite = .ooTestSuite~new
   suite~showProgress = .false
   j = time('E')
   do container over containers
     if types == .nil then container~suite(suite)
     else container~suiteForTestTypes(types, suite)
   end

   testResult~addEvent(suiteBuildPhase~~done)

   executionPhase  = .PhaseReport~new(file, .PhaseReport~TEST_EXECUTION_PHASE)
   say 'Executing automated test suite.'
   suite~execute(testResult)

   testResult~addEvent(executionPhase~~done)
   testResult~addEvent(overallPhase~~done)

   testResult~print("My Title")

return 0

::requires "ooTest.frm"

