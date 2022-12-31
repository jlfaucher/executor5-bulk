#!/usr/bin/env rexx
/*
  SVN Revision: $Rev$
  Change Date:  $Date$
*/
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2007 - 2018 Rexx Language Association. All rights reserved.  */
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

filePattern="*.testUnit"   -- file pattern to look for
flags      ="FSO"          -- search files only, in all subdirs, return fully qualified path
call SysFileTree filePattern, "files.", flags

if files.0=0 then
do
   say filePattern": no files found, aborting!"
   exit -1
end

l=.list~new                -- create a list
do i=1 to files.0          -- add each found file into the list
   l~insert(files.i)
end

tSuite=.TestSuite~new      -- create a TestSuite object, which will contain all testUnits
call makeTestSuiteFromFileList l, tSuite
/* or alternatively:
   tSuite=makeTestSuiteFromFileList(l)  -- will create a testSuite object and return it
*/

tRes=.TestResult~new         -- create a TestResult object to be used to gather the test-log infos
tSuite~execute(tRes)             -- run all the testUnits collected in the Testsuite
/*
   or alternatively:
   tRes=tSuite~execute           -- will create a testResult object and return it
*/

say "--- all testUnits did run now, inspecting the logged results..."
say
say "    tSuite~countTestCases="pp(tSuite~countTestCases)
say "    tRes~errorCount      ="pp(tRes~errorCount    )
say "    tRes~failureCount    ="pp(tRes~failureCount  )
say

say "--- now going through the logged items in 'logQueue'..."
len=length(tRes~logQueue~items)+2
i=0
do item over tRes~logQueue
   i=i+1
   say "     queue item #" pp(i)~right(len)":" pp(item)
   call dumpDir item          -- show contents of directory object
end
say

say "--- now going through the logged items in 'TestCaseTable'..."
len=length(tRes~testCaseTable~items)+2
i=0
do item over tRes~testCaseTable
   i=i+1
   say "     queue item #" pp(i)~right(len)":" pp(item~class) pp(item)
   call dumpQueue tRes~testCaseTable~at(item)   -- show contents of directory object
end
say



::requires oorexxunit.cls  -- get the ooRexxUnit support

::routine dumpDir
  use arg dir
  do idx over dir
     say "           idx="pp(idx) "value:" pp(dir~at(idx))
  end

::routine dumpQueue
  use arg queue
  do item over queue
     say "        item="pp(item)
     if item~class=.directory then     -- if a directory in hand, dump its content
        call dumpDir(item)
  end

