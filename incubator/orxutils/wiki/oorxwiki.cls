/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Description: Simple mediawiki processor written in ooRexx.                 */
/*                                                                            */
/* Copyright (c) 2006 Rexx Language Association. All rights reserved.         */
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
/* Author: W. David Ashley                                                    */
/*                                                                            */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: WIKI                                                                */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class wiki public


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: WIKI                                                                */
/*        Private methods                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


::method conf attribute private
::method filename attribute private
::method filestrm attribute private
::method filelines attribute private
::method linknums attribute private


/*----------------------------------------------------------------------------*/
/* Method: isspace                                                            */
/* Description: returns .true if the argument character is a space char       */
/*----------------------------------------------------------------------------*/

::method isspace private
use arg char
if char == ' ' then return .true
if char == d2c(9) then return .true -- HT
if char == d2c(11) then return .true -- VT
if char == d2c(12) then return .true -- FF
-- there is no need to test for CR and LF, they have already been eliminated
return .false


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: WIKI                                                                */
/*        Public methods                                                      */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Method: init                                                               */
/* Description: instance initialization                                       */
/*----------------------------------------------------------------------------*/

::method init
expose filename filestrm filelines conf
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 2 then raise syntax 93.902 array (2)
-- initialize our page linking numbers
self~linknums = 0
-- get args
if arg() = 1 then do
   use arg conf
   filename = .nil
   end
else use arg conf, filename
-- get the file lines
if filename <> .nil then do
   -- morph the filname into a full OS path
   filename = conf['wiki_apdoc_dir'] || conf['wiki_ns_main'] || filename
   -- open and fetch the contents of the input wiki file
   filestrm = .stream~new(filename)
   status = filestrm~open('read')
   if status <> 'READY:' then do
      filelines = .nil
      filestrm~close()
      filestrm = .nil
      return
      end
   filelines = filestrm~arrayin()
   filestrm~close()
   if filelines~items = 0 then do
      filelines = .nil
      filestrm = .nil
      return
      end
   end
else filelines = .nil
return


/*----------------------------------------------------------------------------*/
/* Method: set_content                                                        */
/* Description: set the array of lines to be transformed                      */
/*----------------------------------------------------------------------------*/

::method set_content
expose filelines
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
use arg filelines
return


/*----------------------------------------------------------------------------*/
/* Method: string                                                             */
/* Description: return the converted string                                   */
/*----------------------------------------------------------------------------*/

::method string
expose filelines filestrm
if arg() > 0 then raise syntax 93.902 array (0)
if filelines = .nil then do
   say '<td class="wiki_content_error">'
   say 'Invalid Page Request: page not found.'
   say '</td>'
   return
   end
-- initialize some variables
stack = .queue~new -- used to store open HTML tags
table_open = .false
listlevel = 0
say '<td class="wiki_content">'
-- process each line
do line over filelines
   idx = 1

   -- Note: the order of the following is significant!

   -- process blank lines (new paragraph)
   if line~strip() = '' then do
      call unwind_stack
      end

   -- process pre text
   else if self~isspace(line~substr(1, 1)) then do
      if stack~peek = 'pre' then do
         say line
         end
      else do
         call unwind_stack
         say '<pre>'
         stack~push('pre')
         say line
         end
      end

   -- process hr text
   else if line~substr(1, 4) == '----' then do
      call unwind_stack
      say '<hr />'
      end

   -- process blockquote text
   else if line~substr(1, 1) == ':' & listlevel = 0 then do
      say '<blockquote>'inline_formatting(line~substr(2))'</blokequote'
      end

   -- process h text
   else if line~substr(1, 1) == '=' then do
      call unwind_stack
      -- process h6 text
      if line~substr(1, 6) == '======' then do
         if line~substr(line~length - 5) <> '======' then do
            say inline_formatting(line)
            end
         else say '<h6>'line~substr(7, line~length - 12)'</h6>'
         end
      -- process h5 text
      else if line~substr(1, 5) == '=====' then do
         if line~substr(line~length - 4) <> '=====' then do
            say inline_formatting(line)
            end
         else say '<h5>'line~substr(6, line~length - 10)'</h5>'
         end
      -- process h4 text
      else if line~substr(1, 4) == '====' then do
         if line~substr(line~length - 3) <> '====' then do
            say inline_formatting(line)
            end
         else say '<h4>'line~substr(5, line~length - 8)'</h4>'
         end
      -- process h3 text
      else if line~substr(1, 3) == '===' then do
         if line~substr(line~length - 2) <> '===' then do
            say inline_formatting(line)
            end
         else say '<h3>'line~substr(4, line~length - 6)'</h3>'
         end
      -- process h2 text
      else if line~substr(1, 2) == '==' then do
         if line~substr(line~length - 1) <> '==' then do
            say inline_formatting(line)
            end
         else say '<h2>'line~substr(3, line~length - 4)'</h2>'
         end
      -- process h1 text
      else do
         if line~substr(line~length) <> '=' then do
            say inline_formatting(line)
            end
         else say '<h1>'line~substr(2, line~length - 2)'</h1>'
         end
      end

   --process ul, ol, dl lists
   else if '*#;:'~pos(line~substr(1, 1)) > 0 then do
      -- see if we need to end a listitem
      if stack~peek = 'li' | stack~peek = 'dd' then do
         say '</'stack~pull'>'
         end
      -- get the length of the list tag
      p = line~pos(' ') - 1
      -- handle the start of a new level
      do while p > listlevel
         listlevel = listlevel + 1
         tag = line~substr(listlevel, 1)
         select
            when tag = '*' then do
               say '<ul>'
               stack~push('ul')
               end
            when tag = '#' then do
               say '<ol>'
               stack~push('ol')
               end
            when tag = ';' then do
               say '<dl>'
               stack~push('dl')
               end
            otherwise nop
            end
         end
      -- handle the closing of old levels
      do while p < listlevel
         say '</'stack~pull'>'
         listlevel = listlevel - 1
         end
      -- now handle the list item contents
      tag = line~substr(p, 1)
      select
         when tag = ';' then do
            -- do a dt tag
            tline = line~substr(p + 1)
            parse var tline term ':' def
            if def~strip() <> '' then do
               say '<dt>'inline_formatting(term)'</dt>'
               say '<dd>'inline_formatting(def)
               stack~push('dd')
               end
            else do
               say '<dt>'inline_formatting(term)'</dt>'
               end
            end
         when tag = ':' then do
            -- do a dd tag
            say '<dd>'inline_formatting(line~substr(p + 1))
            stack~push('dd')
            end
         otherwise  do
            -- do an li tag
            say '<li>'inline_formatting(line~substr(p + 1))
            stack~push('li')
            end
         end
      end

   -- process table tags
   else if '{|!'~pos(line~substr(1, 1)) > 0 then do
      if line~substr(1, 2) = '{|' then do
         say '<table' line~substr(3)'>'
         stack~push('table')
         iterate
         end
      else if line~substr(1, 2) = '|}' then do
         say '</'stack~pull'>' -- end the last row
         say '</'stack~pull'>' -- end the table
         iterate
         end
      else if line~substr(1, 2) = '|+' then do
         say '<caption>'line~substr(3)'</caption>'
         iterate
         end
      -- make sure a row has been started
      if stack~peek <> 'tr' then do
         say '<tr>'
         stack~push('tr')
         end
      -- see if we need to start a new row
      if line~substr(1, 2) = '|-' then do
         say '</tr><tr' line~substr(3)'>'
         iterate
         end
      -- now process the cell tags
      else if line~substr(1, 1) = '|' then do
         tmpline = line~substr(2)
         parse var tmpline cell '||' tmpline
         do while cell~strip() <> ''
            -- see if we have any cell attributes
            parse var cell attr '|' cell
            if cell <> '' then do
               say '<td' attr'>'inline_formatting(cell)'</td>'
               end
            else do
               say '<td>'inline_formatting(attr)'</td>'
               end
            parse var tmpline cell '||' tmpline
            end
         end
      else if line~substr(1, 1) = '!' then do
         tmpline = line~substr(2)
         parse var tmpline cell '!!' tmpline
         do while cell~strip() <> ''
            -- see if we have any cell attributes
            parse var cell attr '!' cell
            if cell <> '' then do
               say '<th' attr'>'inline_formatting(cell)'</th>'
               end
            else do
               say '<th>'inline_formatting(attr)'</th>'
               end
            parse var tmpline cell '!!' tmpline
            end
         end
      else do
         -- we don't know what to do
         nop
         end
      end

   -- process everything else
   else do
      if stack~items = 0 then do
         say '<p>'
         stack~push('p')
         end
      say inline_formatting(line)
      end
   end

call unwind_stack
-- add on the last edited info
say '<p class="wiki_content_lastedited">Last edited'
say filestrm~query('timestamp')
say '</p>'
say '</td>'
return


/*----------------------------------------------------------------------------*/
/* unwind the stack and reset the list level                                  */
/*----------------------------------------------------------------------------*/

unwind_stack:
if arg() > 0 then do while stack~items > 0 & stack~peek <> arg(1)
   say '</'stack~pull'>'
   end
else do while stack~items > 0
   say '</'stack~pull'>'
   end
listlevel = 0
return


/*----------------------------------------------------------------------------*/
/* process a string for inline formatting characters                          */
/*----------------------------------------------------------------------------*/

inline_formatting: procedure expose self
fline = arg(1)
out = ''
stack = .queue~new
i = 1
-- note that inline formatting cannot cross line boundaries
do while i <= fline~length
   -- note that the order of the when clauses in the following
   -- select is significant
   select
      -- bold italic text
      when fline~substr(i, 5) = "'''''" then do
         if stack~items = 0 then do
            out = out || '<b><i>'
            stack~push('b')
            stack~push('i')
            end
         else do
            out = out || '</'stack~pull'></'stack~pull'>'
            end
         i = i + 4 -- skip past the format chars
         end
      -- bold text
      when fline~substr(i, 3) = "'''" then do
         if stack~items = 0 then do
            out = out || '<b>'
            stack~push('b')
            end
         else do
            out = out || '</'stack~pull'>'
            end
         i = i + 2 -- skip past the format chars
         end
      -- italic text
      when fline~substr(i, 2) = "''" then do
         if stack~items = 0 then do
            out = out || '<i>'
            stack~push('i')
            end
         else do
            out = out || '</'stack~pull'>'
            end
         i = i + 1 -- skip past the format chars
         end
      -- wiki links and images
      when fline~substr(i, 2) = "[[" then do
         j = fline~pos(']]', i)
         if j > 0 then do
            tag = fline~substr(i + 2, j - (i + 2))
            -- now we can do something with the tag
            out = out || process_wikilink(tag)
            i = j + 1 -- skip past the link
            end
         else do
            -- this is an error condition, could not find the closeing ']]'
            out = out || fline~substr(i)
            i = fline~length -- don't waste time with the rest of the line
            end
         end
      -- external links
      when fline~substr(i, 1) = "[" then do
         j = fline~pos(']', i)
         if j > 0 then do
            tag = fline~substr(i + 1, j - (i + 1))
            -- now we can do something with the tag
            out = out || process_extlink(tag)
            i = j -- skip past the link
            end
         else do
            -- this is an error condition, could not find the closeing ']'
            out = out || fline~substr(i)
            i = fline~length -- don't waste time with the rest of the line
            end
         end
      -- unmarked external links
      when fline~substr(i, 7) = "http://" then do
         tmpline = fline~substr(i)
         parse var tmpline url .
         out = out || '<a href="'url'">'url'</a>'
         i = i + url~length - 1 -- skip past the link
         end
      otherwise out = out || fline~substr(i, 1)
      end
   i = i + 1
   end
call unwind_stack -- each line should stand alone
return out


/*----------------------------------------------------------------------------*/
/* process a external link                                                    */
/*----------------------------------------------------------------------------*/

process_extlink: procedure expose self
tag = arg(1)
parse var tag url desc
if desc~strip() = '' then do
   self~linknums = self~linknums + 1
   return '<a href="'url'">['self~linknums']</a>'
   end
return '<a href="'url'">'desc'</a>'


/*----------------------------------------------------------------------------*/
/* process a wiki link/image                                                  */
/*----------------------------------------------------------------------------*/

process_wikilink: procedure expose self
tag = arg(1)
-- get the parts of the link
if tag~pos(':') > 0 then do
   parse var tag namespace ':' filename
   namespace = namespace~strip()
   end
else do
   namespace = ''
   filename = tag
   end
if filename~pos('|') > 0 then do
   parse var filename filename '|' desc
   end
else desc = ''
filename = filename~strip()
-- check to see if this is another namespace
if namespace = 'image' then do
   return '<img src="'self~conf['wiki_ns_image'] || filename'" alt="'desc'" />'
   end
else if namespace = 'html' then do
   return '<a href="'self~conf['wiki_ns_html'] || filename'">'desc'</a>'
   end
-- it must be a wiki file link
if desc = '' then do
   return '<a href="/wiki.rex'namespace || filename'">'filename'</a>'
   end
return '<a href="/wiki.rex'namespace || filename'">'desc'</a>'

