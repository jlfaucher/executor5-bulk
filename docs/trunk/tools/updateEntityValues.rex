#!/usr/bin/env rexx
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2022-2023 Rexx Language Association. All rights reserved.    */
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

/* purpose: - allow updating the entity values for YEAR, VERSION and EDITION
              in the appropriate books (${book}/en-US/${book].ent)

   usage:   - no argument  ... show help (see "::resource usage" at the very end below)
            - dir argument ... show current values for YEAR, VERSION and EDITION entities
            - arguments    ... [[-y "year string"] [-v "versionFrom -> versionTo"] [-e "edition string"] [-r[r|i] revisionNumber]] [dir]

   date:    2022-12-21, 2022-12-22, 2023-01-21

   changes: - 2023-01-21 - ignore revisions defined in new file ignoreRevisions.txt
                         - allow for ignoring ${book}/revision_info.txt with new switch "-ri revision",
                           or refresh that file using svn revision with the new switch "-rr revision";
                           create condition if "-r[i|r]" switch without "-e" switch
   version: 1.1
   usage:   see "::resource usage" at the end
*/

if arg()=0 then            -- show usage
do
   say .resources~usage
   exit
end

call analyzeArgs
if .argYear | .argVersion | .argEdition then -- update any of the entity values?
do
   call updateEntityValues
end
exit

/* ========================================================================== */
analyzeArgs: procedure
 pkgLocal=.context~package~local
 pkgLocal~dirSep=.rexxInfo~directorySeparator
 pkgLocal~revInfoFileName="revision_info.txt"
 pkgLocal~dir=.nil

 argNum=.sysCargs~items
 if argNum<2 then             -- show values: either no argument (currDir) or directory argument
 do
    pkgLocal~dir=.sysCArgs[1]
    if \sysFileExists(.dir) then
    do
       .error~say("directory" pp(.dir) "does not exist, aborting ...")
       exit -1
    end
    call showEntityValues
    exit 0
 end

 if argNum//2=1 then    -- uneven, last argument is the directory argument
 do
    pkgLocal~dir=.sysCargs[argNum]  -- assign directory argument
    if \sysFileExists(.dir) then
    do
       .error~say("directory" pp(.dir) "does not exist, aborting ...")
       exit -2
    end
    argNum-=1           -- now only switch/value pairs if any
 end
 else if .dir~isNil then   -- directory to work on is missing
 do
    .error~say("missing directory, must be last argument, aborting ...")
    exit -3
 end

 pkgLocal~argYear    =.false  -- default values
 pkgLocal~argVersion =.false
 pkgLocal~argEdition =.false
 pkgLocal~argRevision=.false
 pkgLocal~argIgnoreRevisionFile=.false -- ignore "${book}/revision_info.txt"
 pkgLocal~argRefreshRevisionFile=.false   -- refresh (replace) "${book}/revision_info.txt"

 do i=1 to argNum by 2
    select case .sysCargs[i]

      when "-r", "-ri", "-rr" then  -- revision: honor only revisions smaller than supplied revision value, ignore revision file?
               do
                  pkgLocal~revisionValue   =.sysCargs[i+1]~strip
                  pkgLocal~argRevision     =.true
                  select case .sysCargs[i]
                    when "-ri" then pkgLocal~argIgnoreRevisionFile=.true
                    when "-rr" then pkgLocal~argRefreshRevisionFile=.true
                    otherwise nop
                  end
               end

      when "-y" then    -- latest year value: if alreay a year range, then change the upper year to the supplied one
               do
                  pkgLocal~yearValue      =.sysCargs[i+1]~strip
                  pkgLocal~argYear        =.true
               end
      when "-v" then    -- replace for all
               do
                  parse value .sysCargs[i+1] with currValue "->" newValue
                  if newValue<>"" then
                  do
                     pkgLocal~versionCurrValue=currValue~strip
                     pkgLocal~versionNewValue =newValue~strip
                     pkgLocal~argVersion       =.true
                  end
               end
      when "-e" then
               do
                  pkgLocal~editionValue=.sysCargs[i+1]   -- replace for all plus add book's revision information
                  pkgLocal~argEdition     =.true
               end
      otherwise
          say "unknown switch" pp(.sysCargs[i]) "with value" pp(.sysCargs[i+1])", aborting ..."
          exit -4
    end
 end

if .argRevision, \.argEdition then
do
    say "error: revision switch [-r] without [-e] switch is illegal"
    exit -5
end


 return

/* ========================================================================== */
::routine showEntityValues       -- show the entity values YEAR, VERSION and EDITION of all books
  dirs.=determineDirectoriesToWorkOn() -- get directories to work on
  do i=1 to dirs.0
     bookPath=dirs.i
     book=FileSpec('name',dirs.i)   -- get directory assume it is a book
     entFile=bookPath || .dirSep || "en-US" || .dirSep || book".ent"
     if SysFileExists(entFile) then
     do
         res=analyzeEntFile(entFile)   -- returns the values
         say (book~left(17,'.')) res
     end
     else
        say entFile": does not exist, skipping ..."
  end

/* ========================================================================== */
::routine determineDirectoriesToWorkOn
  probeDir = .dir || .dirSep || "en-US"   -- is .dir a ${book} directory?
  if sysFileExists(probeDir) then
  do
     dirs.0=1
     dirs.1=qualify(.dir)
  end
  else   -- assuming the root dir in which the book directories are located
  do
     tgtDir=.dir || .dirSep
     call sysFileTree tgtDir"*", "dirs.", "DO"
  end
  return dirs.       -- return stem

/* ========================================================================== */
::routine analyzeEntFile
  parse arg entFile
  s=.stream~new(entFile)~~open("read")
  str=s~arrayin~makeString       -- get file's content
  s~close
  res=""

  do ent over "YEAR", "VERSION", "EDITION"
     needle1='<!ENTITY' ent
     needle2='>'
     res=res ent":"
     pos=1                                -- start out
     do forever
        pos=pos(needle1,str,pos)           -- look for the needle
        if pos=0 then
        do
           res=res pp("n/a")              -- not available (maybe commented out)
           leave
        end
        posCommEnd=lastPos('-->',str,pos) -- in an XML comment?
        if posCommEnd>0 then
        do
           posCommStart=lastPos('<!--',str,pos)
           if posCommEnd<posCommStart then      -- we are in a comment
           do
               pos=pos("-->",str,posCommStart)  -- position on end of comment
               if pos=0 then
               do
                  res=res pp("problem: XML end comment '-->' missing!")
                  leave
               end
               iterate
           end
        end
           -- o.k. outside of a comment
        parse var str 1 preStr +(pos-1) =(pos) (needle1) val (needle2)
        res = res val~strip
        leave
     end
  end
  return strip(res)


/* ========================================================================== */
::routine updateEntityValues       -- show the entity values YEAR, VERSION and EDITION of all books
  dirs.=determineDirectoriesToWorkOn()  -- get directories to work on

  do i=1 to dirs.0 -- for 1 -- NIXI
     bookPath=dirs.i
     book=FileSpec('name',dirs.i)   -- get directory assume it is a book
     entFile=bookPath || .dirSep || "en-US" || .dirSep || book".ent"

     if SysFileExists(entFile) then
     do
         res=updateEntFile(entFile, bookPath, book)   -- returns the values
         say (book~left(17,'.'))       res[1]
         if res[1]<>res[2] then say right("new -->", 17) res[2]
     end
  end


/* ========================================================================== */
::routine updateEntFile
  parse arg entFile, bookPath, book
  s=.stream~new(entFile)~~open("read")
  str=s~charIn(1,s~chars)
  s~close
  res=""
  mb   =.MutableBuffer~new       -- new file's content
  mbUpdates=.MutableBuffer~new   --

  do ent over "YEAR", "VERSION", "EDITION"
     select case ent       -- skip entity ?
         when 'YEAR'    then if .argYear   =.false then iterate
         when 'VERSION' then if .argVersion=.false then iterate
         when 'EDITION' then if .argEdition=.false then iterate
         otherwise nop
     end

     needle1='<!ENTITY' ent
     needle2='>'
     res=res ent":"
     pos=1                                -- start out
     do forever
        pos=pos(needle1,str,pos)           -- look for the needle
        if pos=0 then
        do
           res=res pp("n/a")              -- not available (maybe commented out)
           leave
        end
        posCommEnd=lastPos('-->',str,pos) -- in an XML comment?
        if posCommEnd>0 then
        do
           posCommStart=lastPos('<!--',str,pos)
           if posCommEnd<posCommStart then      -- we are in a comment
           do
               pos=pos("-->",str,posCommStart)  -- position on end of comment
               if pos=0 then
               do
                  res=res pp("problem: XML end comment '-->' missing!")
                  leave
               end
               iterate
           end
        end
           -- o.k. outside of a comment
        parse var str 1 preStr +(pos-1) =(pos) (needle1) '"' val '"' (needle2) str

        mb~append(preStr,needle1,' "')

        if mbUpdates~length>0 THEN mbUpdates~append(" ")  -- already an entry, add blank
        mbUpdates~append(ent,': ')
        select case ent       -- process entity
            when 'YEAR'    then  -- replace or change upper year
                           do
                               if val~pos("-")>0 then -- already a range, replace upper year
                               do
                                  parse var val oldYear '-' currYear
                                  newVal=oldYear'-'.yearValue
                               end
                               else
                               do
                                  if val<.yearValue then
                                     newVal=val~strip"-".yearValue   -- turn into range
                                  else
                                  do
                                     newVal=.yearValue
                                  end
                               end
                               mb       ~append(newVal)
                               mbUpdates~append(newVal)
                           end
            when 'VERSION' then
                           do
                               newVal=val~changeStr(.versionCurrValue,.versionNewValue)
                               mb       ~append(newVal)
                               mbUpdates~append(newVal)
                           end
            when 'EDITION' then
                           do
                               mb~       append(.editionValue)
                               mbUpdates~append(.editionValue)
                               if .argRevision then    -- if release supplied: ignore release or newer
                               do
                                  if .argIgnoreRevisionFile then  -- get and use revision from svn
                                  do
                                     newVal=getRevInfo(bookPath, book) -- get revision information, can be .nil
                                  end
                                  else
                                  do
                                     revFileName=bookPath || .dirSep || .revInfoFileName
                                     -- use first line of an existing ${book}/revision_info.txt ?
                                     if sysFileExists(revFileName), .argRefreshRevisionFile=.false then
                                     do
                                         sr=.stream~new(revFileName)~~open("read")
                                         srArr=sr~arrayIn
                                         sr~close
                                         -- mb~append(" ",srArr[1])  -- write first line
                                         newVal=srArr[1]
                                     end
                                     else -- (re-)create ${book}/revision_info.txt from svn revision information?
                                     do
                                         newVal=getRevInfo(bookPath, book) -- get revision information, can be .nil
                                         -- mb~append(" ",revInfo)
                                          -- if revision exists, save in ${book}/revision_info.txt
                                         if \newVal~isNil then
                                            .stream~new(revFileName)~~open("replace")~~charout(revInfo)~~close
                                     end
                                  end
                                  if \newVal~isNil then  -- if non-ignorable revision available, use and append it
                                  do
                                     mb~       append(" ",newVal)
                                     mbUpdates~append(" ",newVal)
                                  end
                               end
                           end
            otherwise nop
        end
        mb~append('"',needle2)

        res = res val~strip
        leave
     end
  end
  mb~append(str)     -- append any left over
  -- replace file with new content
  .stream~new(entFile)~~open("replace")~~charout(mb~string)~~close
  return (strip(res),mbUpdates~string) -- return old and new content

/* ========================================================================== */
::requires "svnListRevisions.rex"   -- get revision utility

/* ========================================================================== */
::routine pp
  return "["arg(1)"]"

/* ========================================================================== */
::routine getRevInfo
  use arg bookPath, book
  -- look into the files in "en-US", ignore ${book}.ent file
  tgtDir=bookPath || .dirSep || "en-US"
  arr=getRevisionInfos(tgtDir)         -- from svnListRevisions.rex
  if arr~items=0 then return "(n/a)"   -- ignore, not under version control

  ignoreFiles=book".ent" "Author_Group.xml"  -- add additional file names, if sensible
  ignorableRevisions=getIgnorableRevisions() -- get revisions to ignore (e.g. change copyright year commits and the like)

  maxRelease=.revisionValue-1        -- anything higher (= newer) gets ignored
  tmpRelease=.nil
  do revItem over arr
      if revItem~revision>maxRelease then iterate

      if ignorableRevisions~hasIndex(revItem~revision) then iterate  -- 2023-01-20
      if wordPos(revItem~name,ignoreFiles)>0 then iterate

      if tmpRelease=.nil then
         tmpRelease=revItem
      else if tmpRelease~date<revItem~date then
      do
         tmpRelease=revItem
      end
  end

  if tmpRelease~isNil then    -- all releases to be ignored according to the supplied arguments
     return .nil

  parse value tmpRelease~date with date "T" time
  return "(last revised on" date~changeStr("-","") "with r"tmpRelease~revision")"

/* ========================================================================== */
::routine getIgnorableRevisions

  parse source . . fullPath
  fn=filespec('location',fullPath)"ignoreRevisions.txt"
  ignoreRevisions=.set~new
  if sysFileExists(fn) then
  do
      s=.stream~new(fn)
      arr=s~arrayin
      s~close
      do line over arr
         if line="" then iterate
         if pos(line~strip~left(1),"-#") then iterate -- a comment?
         ignoreRevisions~put(line)  -- assume a revision string
      end
  end

  return ignoreRevisions


/* ========================================================================== */
::resource usage        -- usage information (if no argument supplied)
List or change the values for the entities YEAR, VERSION, EDITION from/in
all the files ${book}/en-US/${book].ent

updateEntityValues [[arguments] [dir]]
      no arguments   ... show this text
      dir            ... always the last argument, denotes the root of the docs
                         directory to work on (if dir contains spaces it needs
                         to be enquoted in double-quotes), or the path to a specific
                         ${book} directory.
                         If no switches are supplied than the current values
                         of the YEAR, VERSION and EDITION entity values for
                         each books get displayed.
                         If running off the
      -y latestYear  ... set the latest year in YEAR to latestYear (a number)
      -v "version"   ... set VERSION to the supplied string in the form (quoted!)
                         of "oldValue -> newValue"
      -e "edition"   ... set EDITION to the supplied string (can be anything);
                         if none of the -r switches are not set, then no revision
                         information string gets appended
      -r revision    ... append revision information to EDITION; if the file
                         ${book}/revision_info.txt exists, its first line will
                         be used as the revision information string; otherwise
                         the revision information is built from commits with
                         revisions that are smaller than the supplied revision,
                         the result gets also written to ${book}/revision_info.txt
      -ri revision   ... append revision information to EDITION ignore
                         ${book}/revision_info.txt use the latest svn revision that
                         is smaller than the supplied revision
      -rr revision   ... append revision information to EDITION, refresh (replace)
                         ${book}/revision_info.txt using svn revision smaller
                         than the supplied revision

examples:
      updateEntityValues.rex -v "5.0.0 -> 5.1.0" -e "2023.01.01" ".."
      updateEntityValues.rex -e "2023.01.01" -r 12537  ..
      updateEntityValues.rex -e "2023.01.01" -ri 12537  ..
      updateEntityValues.rex -e "2023.01.01" -rr 12537  ..
      updateEntityValues.rex -y 2023 -v "5.0 -> 5.1" -e "2023.01.01" -r 99999  ..
      updateEntityValues.rex ..
::END

