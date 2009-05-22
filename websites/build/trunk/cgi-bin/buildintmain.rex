#!/usr/bin/rexx
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Description: Submit an action to the build system.                         */
/*                                                                            */
/* Copyright (c) 2007 Rexx Language Association. All rights reserved.         */
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


-- get the CGI variables
request_method = value('REQUEST_METHOD', , 'ENVIRONMENT')
content_length = value('CONTENT_LENGTH', , 'ENVIRONMENT')
query_string = value('QUERY_STRING', , 'ENVIRONMENT')
if request_method = 'GET' then call parse_query_string query_string
else if request_method = 'POST' then do
   if content_length <> '' then do
      post_string = charin(, , content_length)
      call parse_query_string post_string
      end
   end
else nop
vm = get_postvar('vm')
target = get_postvar('target')
addressee = get_postvar('addressee')

-- set up the defaults
title = 'Build ooRexx Interpreter Main'
call htmlheader title
call htmlnavbar

say '<!-- ###### body content start ###### -->'
say '       <div id="column-middle">'
say
say '          <div id="title">'
say '             <b>'title'</b>'
say '          </div>   '
say
say '            <h2>Welcome</h2>'
retc = start_build(vm, target, addressee)
if retc = 0 then do
   say '   <p>The interpreter will be built by a background process.'
   say '      It should be ready within 15 minutes. The output of the build'
   say '      will be available in a subdirectory off of'
   say '      <a href="/builds/interpreter-main">this location</a>.</p>'
   if addressee <> '' then do
      say '   <p>A notification email will be sent to' addressee 'when the build'
      say '      completes. It will contain the specific URL for the output of'
      say '      the build.</p>'
      end
   end
else if retc = 1 then do
   say '   <p>The background process is currently busy performing a build.</p>'
   end
else if retc = -1 then do
   say '   <p>An error occured submitting the background build request. Please try again later.</p>'
   end
else do
   say '   <p>The build is either finished or an unknown error has occured.</p>'
   say '   <p>If the build is finished then the output of the build'
   say '      will be available in a subdirectory off of'
   say '      <a href="/builds/interpreter-main">this location</a>.</p>'
   end
say '   <p>The current time at the server is' date() time('N') '.</p>'
say '            <br />'
say '            <br />'
say '            <br />'
say '            <br />'
say '            <br />'
say '            <br />'
say '            <br />'
say '            <br />'
say '            <br />'
say '            <br />'
say '         </div> <!--column-middle-->'
say
say '<!-- ###### body content end ###### -->'
say
say '      </div> <!--left-->'

call htmlfooter
return


/*----------------------------------------------------------------------------*/
/* Function: parse_query_string                                               */
/* Description: parse a query string into an arg stem array.                  */
/*----------------------------------------------------------------------------*/

parse_query_string: procedure expose wwwargs.

qs = arg(1)
args = 0
do while qs~length > 0
   parse var qs attrname '=' attrvalue '&' qs
   args = args + 1
   attrname = attrname~changestr('+', ' ')
   nextpercent = attrname~pos('%')
   do while nextpercent > 0
      attrname = attrname~substr(1, nextpercent - 1) || ,
       x2c(attrname~substr(nextpercent + 1, 2)) || attrname~substr(nextpercent + 3)
      nextpercent = attrname~pos('%')
      end
   wwwargs.args.!NAME = attrname
   attrvalue = attrvalue~changestr('+', ' ')
   nextpercent = attrvalue~pos('%')
   do while nextpercent > 0
      attrvalue = attrvalue~substr(1, nextpercent - 1) || ,
       x2c(attrvalue~substr(nextpercent + 1, 2)) || attrvalue~substr(nextpercent + 3)
      nextpercent = attrvalue~pos('%')
      end
   wwwargs.args.!VALUE = attrvalue
   wwwargs.attrname = attrvalue
   end
wwwargs.0 = args
return

/*----------------------------------------------------------------------------*/
/* Function: start_build                                                      */
/* Description: create the command file that will start a build or queue      */
/*              the request.                                                  */
/*----------------------------------------------------------------------------*/


start_build: procedure
use arg vm, target, addressee
if vm = 'winxpprof-build' | vm = 'ubuntu704-build' then do
   cmdfile = '/home/buildorx/website/cmds/'vm
   if SysIsFile(cmdfile) then return 1
   strm = .stream~new(cmdfile)
   retc = strm~open('write')
   if retc <> 'READY:' then return 2
   cmd = ('interpreter-main' target addressee)~strip()
   strm~lineOut(cmd)
   retc = strm~close()
   end
else do
   msg = 'queue' vm addressee
   -- get a new stream
   s = .streamsocket~new('192.168.0.104', 15776)
   -- open the stream
   retc = s~open()
   if retc <> 'READY:'then return -1
   -- send the command
   retc = s~lineout(msg)
   -- receive the reply
   dummy = s~linein()
   -- close the stream
   s~close()
   end
return 0

/*----------------------------------------------------------------------------*/
/* Function: get_postvar                                                      */
/* Description: return the value of a CGI POST variable                       */
/*----------------------------------------------------------------------------*/


get_postvar: procedure expose wwwargs.
use arg varname
do i = 1 to wwwargs.0
   if wwwargs.i.!name = varname then return wwwargs.i.!value
   end
return ''


::requires '/home/buildorx/website/cgi-bin/buildhtmlparts.cls'
::requires 'streamsocket.cls'

