#!/usr/bin/rexx
/*
   name:    runAndDumpTestUnitLogData.rex (was: 'test4RainerKegel.rex')
   date:    2006-03-25
   author:  Rony G. Flatscher
   purpose: demonstrate how to initiate testUnit runs and to dump the log data from the TestResult class
            using the attributes "logQueue" and "TestCaseTable"; originally developed for Rainer Kegel to
            help proceed with his project

   license: CPL 1.0 (Common Public License v1.0, see below)
*/

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

