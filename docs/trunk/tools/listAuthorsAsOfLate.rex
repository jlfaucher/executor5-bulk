#!/usr/bin/env rexx
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2022-2024, Rexx Language Association. All rights reserved.         */
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

/* purpose: goes through all books and retrieves the authors of the latest commits and
            lists them alphabetically
*/
parse arg dir
dir=dir~strip

pkgLocal=.context~package~local
pkgLocal~dirSep=.rexxInfo~directorySeparator
tgtDir=dir || .dirSep

if \sysFileExists(dir) then
do
   .error~say("directory" pp(dir) "does not exist, aborting ...")
   exit -1
end

call sysFileTree tgtDir"*", "dirs.", "DO"
do i=1 to dirs.0
  bookPath=dirs.i
  book=FileSpec('name',dirs.i)   -- get directory assume it is a book
  authors=getAuthors(bookPath,book)
  say (book~left(17,'.')) authors
end

::requires "svnListRevisions.rex"   -- get revision utility

/* ========================================================================== */
::routine pp                     -- enclose argument in square brackets
  return "["arg(1)"]"

/* ========================================================================== */
/* get authors from log, return unique, ordered list */
::routine getAuthors
  use arg bookPath, book
  -- look into the files in "en-US", ignore ${book}.ent file
  tgtDir=bookPath || .dirSep || "en-US"
  if \sysFileExists(tgtDir) then tgtDir=bookPath   -- e.g. the tools subdirectory has no "en-US" directory

  ignoreFiles=book".ent" "Author_Group.xml"  -- add additional file names, if sensible

  setAuthors=.set~new
  arr=getRevisionInfos(tgtDir)
  do revItem over arr
      if wordPos(revItem~name,ignoreFiles)>0 then iterate
      setAuthors~put(revItem~author)
  end
  return setAuthors~allIndexes~sort~toString(,", ")
