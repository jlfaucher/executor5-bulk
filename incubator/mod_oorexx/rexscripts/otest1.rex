/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (C) W. David Ashley 2004-2008. All Rights Reserved.              */
/* Copyright (c) 2009-2009 Rexx Language Association. All rights reserved.    */
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
/* Description: Test page for the Mod_ooRexx Apache module using the ooRexx   */
/*              classes.                                                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/* version of this script */
script_version = 'v2.2.0'

/* these are some typical Apache return codes */
DECLINED  = -1    /* Module declines to handle */
DONE      = -2    /* Module has served the response completely */
OK        = 0     /* Module has handled this stage. */
NOT_FOUND = 404   /* Main document not found. */

/* get environment stuff */
envname = 'ENVIRONMENT'
parse upper version v

/* get the Apache request record ptr */
r = arg(1)

/* set content-type and send the HTTP header */
call WWWSendHTTPHeader r, 'text/html' /* note: this call is NOT optional */

/* start sending the html page */
say '<HTML>'
say '<HEAD>'
say '<TITLE>Sample HTML Page From Rexx</TITLE>'
say '</HEAD>'
say '<BODY>'
say '<H1>Sample HTML Page From Rexx</H1>'

say '<p>The Version of this script is' script_version'.'

say '<p>The Mod_Rexx version string is "'WWWGetVersion()'"'

say '<p>The number of Rexx procedure arguments is' arg()

say '<p>The value of arg(1) is' arg(1)

say '<p>The current RFC 822 formatted time at the server is "'WWWHTTP_time(r)'"'

say '<p>The following is some standard Rexx environment information:'
say '<p>The Rexx Version is <b>'v'</b>.'
say '<br>The Rexx procedure path/filename is <b>'path'</b>.'
say '<br>The Rexx runtime environment name is <b>'env'</b>.'
say '<br>The Rexx standard environment name that should be used is <b>'envname'</b>.'
say '<br>The starting Address environment is <b>'address()'</b>.'
say '<br>The current directory is <b>'directory()'</b>.'

/* now instantiate the orexx request_rec class */
reqx = .request_rec~new(r)

/* now instantiate the orexx server_rec and connection_rec classes */
conx = .connection_rec~new(reqx~connection)
srvx = .server_rec~new(reqx~server)

say '<h2>The Request_rec Methods</h2>'

say '<p>The following is the list of request_rec methods'
say 'which return their values:'
say '<table border="1"><tr><th>Method Name</th><th>Value</th></tr>'
say '<tr><td>ismain</td><td>'
if reqx~ismain = .true then say 'true</td></tr>'
else say 'false</td></tr>'
say '<tr><td>the_request</td><td>'vorb(reqx~the_request)'</td></tr>'
say '<tr><td>proxyreq</td><td>'
if reqx~proxyreq = .true then say 'true</td></tr>'
else say 'false</td></tr>'
say '<tr><td>header_only</td><td>'
if reqx~header_only = .true then say 'true</td></tr>'
else say 'false</td></tr>'
say '<tr><td>protocol</td><td>'vorb(reqx~protocol)'</td></tr>'
say '<tr><td>status_line</td><td>'vorb(reqx~status_line)'</td></tr>'
say '<tr><td>status</td><td>'vorb(reqx~status)'</td></tr>'
say '<tr><td>method</td><td>'vorb(reqx~method)'</td></tr>'
say '<tr><td>method_number</td><td>'vorb(reqx~method_number)'</td></tr>'
say '<tr><td>allowed</td><td>'vorb(reqx~allowed)'</td></tr>'
say '<tr><td>bytes_sent</td><td>'vorb(reqx~bytes_sent)'</td></tr>'
say '<tr><td>content_type</td><td>'vorb(reqx~content_type)'</td></tr>'
say '<tr><td>content_encoding</td><td>'vorb(reqx~content_encoding)'</td></tr>'
say '<tr><td>handler</td><td>'vorb(reqx~handler)'</td></tr>'
say '<tr><td>content_languages</td><td>'vorb(reqx~content_languages)'</td></tr>'
say '<tr><td>no_cache</td><td>'
if reqx~no_cache = .true then say 'true</td></tr>'
else say 'false</td></tr>'
say '<tr><td>uri</td><td>'vorb(reqx~uri)'</td></tr>'
say '<tr><td>filename</td><td>'vorb(reqx~filename)'</td></tr>'
say '<tr><td>path_info</td><td>'vorb(reqx~path_info)'</td></tr>'
say '<tr><td>args</td><td>'vorb(reqx~args)'</td></tr>'
say '<tr><td>finfo.st_mode</td><td>'vorb(reqx~finfo_stmode~x2b)'</td></tr>'
say '<tr><td>user</td><td>'vorb(reqx~user)'</td></tr>'
say '<tr><td>auth_type</td><td>'vorb(reqx~auth_type)'</td></tr>'
say '</table>'

say '<h2>The Server_rec Methods</h2>'

say '<p>The following is the list of server_rec methods'
say 'and their values:'
say '<table border="1"><tr><th>Method Name</th><th>Value</th></tr>'
say '<tr><td>server_admin</td><td>'vorb(srvx~server_admin)'</td></tr>'
say '<tr><td>server_hostname</td><td>'vorb(srvx~server_hostname)'</td></tr>'
say '<tr><td>port</td><td>'vorb(srvx~port)'</td></tr>'
say '<tr><td>is_virtual</td><td>'
if srvx~is_virtual = .true then say 'true</td></tr>'
else say 'false</td></tr>'
say '</table>'

say '<h2>The Connection_rec Methods</h2>'

say '<p>The following is the list of connection_rec methods'
say 'and their values:'
say '<table border="1"><tr><th>Method Name</th><th>Value</th></tr>'
say '<tr><td>aborted</td><td>'
if conx~aborted = .true then say 'true</td></tr>'
else say 'false</td></tr>'
say '</table>'

call WWWGetArgs r /* note: this call is NOT optional */
say "<p>If the request method is GET then the following array will be filled"
say "with the Query String name/value pairs. If the request method is POST"
say "then the following array will be filled with the POST list of"
say "name/value pairs. The number of arguments is contained in the variable"
say "WWWARGS.0."
say "<p>The number of arguments is" wwwargs.0"."
if wwwargs.0 > 0 then do
   say '<table border="1">'
   do i = 1 to wwwargs.0
      say "<tr><td>WWWARGS."i".!NAME</td><td>"wwwargs.i.!name"</td></tr>"
      say "<tr><td>WWWARGS."i".!VALUE</td><td>"wwwargs.i.!value"</td></tr>"
   end
   say "</table>"
end

call WWWGetCookies r
say "<p>The following array contains all the cookies passed to this request."
say "The number of cookies is contained in the variable"
say "WWWCOOKIES.0."
say "<p>The number of cookies is" wwwcookies.0"."
if wwwcookies.0 > 0 then do
   say '<table border="1">'
   do i = 1 to wwwcookies.0
      say "<tr><td>WWWCOOKIES."i".!NAME</td><td>"wwwcookies.i.!name"</td></tr>"
      say "<tr><td>WWWCOOKIES."i".!VALUE</td><td>"wwwcookies.i.!value"</td></tr>"
   end
   say "</table>"
end

say '<p><hr><p>'

say 'The following will reinvoke this script using the POST request method.'


say '<FORM method="post" action="./otest1.rex/extrainfo">'
say '<INPUT type="hidden" name="hiddenfield" value="A hidden string">'
say '<P>Enter some text:'
say '<INPUT type="text" name="visibletext" size="50">'
say '<BR>'
say '<INPUT type="submit">'
say '</FORM>'

say '</BODY>'
say '</HTML>'
return OK


/* vorb: return the value or a required space */
vorb:

if length(arg(1)) > 0 then return arg(1)
return '&nbsp;'


::requires 'Apache.cls'

