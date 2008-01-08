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

   absoluteBegin = .TimeSpan~new(time('F'))

   .local~bRunTestsLocally = .false

   baseDir = "ooRexx"
   --baseDir = "ooRexx\samples"

   -- Execute all test types, except the UNIT_LONG type.
   types = .ooTestTypes~all~difference(.set~of(.ooTestTypes~UNIT_LONG_TEST))
   do i over types
     say 'got test type:' i
   end

   testResult = .ooRexxUnit.default.TestResult.Class~new
   --testResult~setVerbosity(0)
   finder = .ooTestFinder~new(baseDir, ".testGroup", types)
   containers = finder~seek(testResult); say 'containers items:' containers~items
   searchEnd = .TimeSpan~new(time('F'))

   suite = .ooTestSuite~new
   suite~showProgress = .false
   j = time('E')
   do container over containers
     container~suite(suite)
   end
   suiteEnd = .TimeSpan~new(time('F'))

   say 'Executing automated test suite.'
   suite~execute(testResult)
   testEnd = .TimeSpan~new(time('F'))

   testResult~print("My Title", 4)

   absoluteEnd = .TimeSpan~new(time('F'))
   say 'File search:   ' (searchEnd - absoluteBegin)
   say 'Suite Build:   ' (suiteEnd - searchEnd)
   say 'Test execution:' (testEnd - suiteEnd)
   say 'Total time:    ' (absoluteEnd - absoluteBegin)
return 0

::requires "ooTest.frm"

