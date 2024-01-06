#!/usr/bin/env rexx
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2022 Rexx Language Association. All rights reserved.         */
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

/* purpose: update copyright information of all files in the given directory and
            all of its subdirectories

   usage: updateCopyright [dir] [year]
          dir  ... optional, defaults to ".."
          year ... optional, defaults to current year
*/
parse arg dir year
dir=dir~strip
if dir=""  then dir=".."
if year="" then parse value date() with . . year   -- use today's year
say "dir :" pp(dir)
say "year:" pp(year)

pkgLocal=.context~package~local
pkgLocal~dirSep=.rexxInfo~directorySeparator
tgtDir=dir || .dirSep

if \sysFileExists(dir) then
do
   .error~say("directory" pp(dir) "does not exist, aborting ...")
   exit -1
end
say "sysFileTree ..."
call sysFileTree tgtDir"*", "files.", "FOS"
say "found" pp(files.0) "files to process ..."

noneSum  =0
editedSum=0
do counter c1 i=1 to files.0
   say "file #" i~right(4)":" pp(files.i) "..."
   res=processFile(files.i,year)
   if res=.false then
   do
       say "      " i~right(4)":" pp(files.i) "- no copyright found ..."
       noneSum+=1
   end
   else
       editedSum+=1
end
say "-"~copies(79)
say "summary:"
say "       # edited (changed):" editedSum~right(4)
say "       # no copyright    :" noneSum  ~right(4)

/* ========================================================================== */
::routine processFile
  parse arg fn, year
  s=.stream~new(fn)~~open("read")
  data=s~charIn(1,s~chars)
  s~close

  needle1a="Copyright"
  needle1b="(c)"

  needle2="Rexx"
  parse var data before (needle1a) (needle1b) crData (needle2) rest
  if crData="" then return .false      -- no copyright not
  crData=crData~strip
   -- change copyright
  newCrStr=""
  parse var crData crStartYear "-" crEndYear
  if crStartYear=year then newCrStr=year     -- only a single year?
                      else newCrStr=crStartYear"-"year
  editedData = before || needle1a needle1b newCrStr"," needle2 || rest
  .stream~new(fn)~~open("write replace")~~charout(editedData)~~close
  return .true


/* ========================================================================== */
::routine pp
  return "["arg(1)"]"

