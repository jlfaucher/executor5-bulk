
/*
   name:             runTestUnits.rex
   author:           Rony G. Flatscher
   date:             2005-08-20
   version:          1.0.3

   changed:          2005-10-19, ---rgf, fixed a little bug: if "-r" is given without a directory,
                                         start with current directory (instead of root directory!)
                     2006-11-29, ---rgf, changed name to indicate what this program is meant for
                     2006-12-12, ---rgf, changed title for brief test results slightly
                     2006-12-13, ---rgf, added elapsed time information (for running all test units)
                     2006-12-14, ---rgf, added hashbang line

   license:          CPL 1.0 (Common Public License v1.0, see below)

   languageLevel:    6.0
   purpose:          Load all ooRexx-base testUnits and run all the tests contained therein.

   remark:           Serves as an example of how to use and drive the ooRexxUnit framework

   category:
*/
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

parse arg directory
if directory="" then directory="-?"

switches="FO"  -- SysFileTree(): "F"iles only, "O"nly fully qualified name

word1=word(directory, 1)   -- check whether switch is given
if word1~left(1)="-" then  -- a switch in hand
do
   if substr(word1,2)="r" then   -- "R"ecursively
   do
      switches=switches||"S"                 -- add "S" switch for SysFileTree()
      directory=subword(directory, 2)        -- remove switch
   end
   else if word1<>"-?" then word1="-?"       -- wrong switch, show usage

   if word1="-?" then    -- show usage ?
   do
      parse source . . thisFile
      parse value filespec("N", thisFile) with thisName "." .
      say "rexx" thisName "[-r] path2directory"
      say
      say "    -r             ... optional switch: look in subdirectories recursively"
      say '    path2directory ... path to directory to search for "*.testUnit" files '
      exit
   end
end

if pos(right(directory,1), "/\")>0 then -- trailing [back]slash ? if so, remove it
   directory=directory~left(length(directory)-1)

   -- determine path separator, if on OS2 or Windows use backslash, else slash
if directory="" then directory="."  -- default to current directory
searchFile=directory || iif( pos(left(sysVersion(),1)~translate, "WO")>0, "\", "/") || "*.testUnit"

say "searchFile="pp(searchFile)", SysFileTree()-switches:" pp(switches) "..."

   -- get all test unit files in current directory
call sysFileTree searchFile, "tests.", switches

   -- create list of file names
list=.list~new
do i=1 to tests.0
   list~insert(tests.i)
end

ts=makeTestSuiteFromFileList(list)  -- create the testSuite object
call time "Reset"    -- start timer
tr=ts~run   -- run all the tests in the testSuite, retrieve the testResult object
e=time("Elapsed")

            -- show brief results
parse source . . s                  -- get full path to this program
this=filespec("Name", s)            -- extract file name
call simpleDumpTestResults tr, pp(this) "processed:" pp(searchFile) "with" pp(switches),
                               "found" pp(tests.0) "file(s), elapsed time:" pp(e) "sec(s)"

::requires ooRexxUnit.cls

