#!/usr/bin/env rexx
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2020-2023 Rexx Language Association. All rights reserved.    */
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
/*
   changed: 2023-01-20: ignore directories that are not under version control

   This tool uses "svn list --xml --incremental" to get the revision numbers of
   all files and directories under revision control from the supplied path.

   The public routine getRevisionInfos(path) returns an array of revisionedItem
   objects sorted ascendingly by revision.

   Each revisionedItem object has the attributes

      author:   author's name who committed this revision
      revision: svn revision number
      date:     date of commit
      path:     path to a file or directory
      name:     name of the revisioned file object
      kind:     "file" or "dir" indicating kind/type of file object

   The resulting array can be sorted

   Usage:
      - commandline:

         rexx svnListRevisions path

      - Rexx programs: require this file and call the function

         arr=getRevisionInfos(path) /* returns sorted array of revisionedItem objects */
*/

parse arg path
if path<>"" then call test path

/* ========================================================================== */
/* for testing purposes via the command line                                  */
::routine test public
   parse arg path

   if path="" then path="."
   else
   do
      path=path~strip
      if \sysFileExists(path) then
      do
         say arg(1)": does not exist, aborting ..."
         exit -1
      end
   end

   arr=getRevisionInfos(path)
   say "parse_xml_output's results (default: sorted by revision):"
   say arr
   say

   say "sorted by author name:"
   say arr~copy~sortWith(.revisionedItem~getAuthorComparator)
   say

   say "sorted by file/directory name:"
   say arr~copy~sortWith(.revisionedItem~getNameComparator)
   say

/* ========================================================================== */
/* control routine: get svn list output, parse and return array of "revisionedItem"    */
/*                  objects sorted ascendingly by revision number             */
::routine getRevisionInfos public
  parse arg path

  if \sysFileExists(path) then
     raise syntax 40.900 array ( "path" pp(path) "does not exist")

  res=parse_xml_data(get_list_infos(path))
  if res~items=0 then   -- return empty array, do not sort it
     return res
  return res~sortWith(.revisionedItem~getRevisionComparator)

/* ========================================================================== */
/* execute "svn --xml --incremental list" command, return stdout array        */
::routine get_list_infos
  parse arg path

  outData=.array~new
  command="svn -r HEAD --xml --incremental list" path
  address system command with output using (outData)
  if rc<>0 then      -- if svn error, then assume not under version control
     outData~empty   -- empty outData to ignore this path
  return outData

/* ========================================================================== */
/* parse the xml elements, create revisionedItem-objects, collect in and return array  */
::routine parse_xml_data
  parse arg data
  allEntries=.array~new
  parse var data "<list" . "path=" path ">" data "</list>"
  path=path~strip("both",'"')

  do while data<>""
     parse var data "<entry" . "kind=" kind ">" "<name>" name "</name>" -
                    "<commit" . 'revision=' revision ">" "<author>" author "</author>" . "<date>" date "<" "</entry>" -
                    data
     if kind="" then leave
     allEntries~append(.revisionedItem~new(path, kind~strip("both",'"'), name, revision~strip("both",'"'), author, date))
  end
  return allEntries

/* ========================================================================== */
::routine pp         -- "pretty-print": enclose argument in square brackets
  return "["arg(1)"]"


/* ========================================================================== */
/* class representing each entry from "--xml --incremental" output            */
::class  revisionedItem          public
/* -------------------------------------------------------------------------- */
::method getRevisionComparator   class
  return .revisionComparator~new

::method getNameComparator       class
  return .nameComparator~new

::method getAuthorComparator     class
  return .authorComparator~new

/* -------------------------------------------------------------------------- */
::attribute path
::attribute kind     -- "file", "dir"
::attribute name
::attribute revision -- attribute of commit element
::attribute author
::attribute date

::method init        -- constructor
  use local
  use arg path, kind, name, revision, author, date
  date=.dateTime~fromUtcIsoDate(date)
  self~objectname=self~string -- use required string value as object name

::method string      -- required string value
  use local
  return (author "")~left(12,".") revision date path "/" name "("kind")"

/* ========================================================================== */
::class revisionComparator    -- svn revision number
::method compare
  use strict arg left, right
  return left~revision~compareTo(right~revision)

/* ========================================================================== */
::class nameComparator        -- the sequence svn returns information
::method compare
  use strict arg left, right
  return left~name~caselessCompareTo(right~name)

/* ========================================================================== */
::class authorComparator      -- author's name
::method compare
  use strict arg left, right
  return left~author~caselessCompareTo(right~author)
