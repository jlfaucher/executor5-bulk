#! /usr/bin/rexx
-- #! "c:\program files\oorexx\rexx.exe"
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Description: Simple wiki written in ooRexx.                                */
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


-- this is our version
version = '0.2'

-- determine if we were invoked as a CGI or from Mod_Rexx
if value('REQUEST_METHOD', , 'ENVIRONMENT') = '' then do
   r = arg(1) -- get the Apache request record ptr
   call mod_rexx_setup r
   end
else do
   call cgi_setup
   end
-- when we get here everything should be like the Mod_Rexx startup env

-- get the global config settings
conf = get_wikiconf()

-- add our version info to the global config settings
conf['wiki_version'] = version

-- figure out what action we are performing
if wwwquery_string <> '' then action = wwwquery_string
else action = 'display'
if action = 'preview' then do
   action = 'display'
   preview = .true
end
else preview = .false

-- we may need to adjust the wwwpath_info if it is incomplete
if wwwpath_info = '' then wwwpath_info = '/' || conf['wiki_main_page']
if wwwpath_info~substr(wwwpath_info~length, 1) = '/' then ,
 wwwpath_info = wwwpath_info || conf['wiki_main_page']

-- is this a save operation?
if action = 'save' then do
   do i = 1 to wwwargs.0
      if wwwargs.i.!NAME = 'submit' then do
         if wwwargs.i.!VALUE = 'Save Changes' then do
            call save_wiki
            action = 'display'
            end
         else if wwwargs.i.!VALUE = 'Preview' then do
            call preview_wiki
            action = 'preview'
            end
         else if wwwargs.i.!VALUE = 'Cancel' then action = 'display'
         else nop -- we should never get here
         leave
         end
      end
   call send_redirect
   return 0
   end

-- send the HTML page to the browser
call create_html_header conf
call create_html_banner conf
say '<table width="100%" cellspacing="8" cellpadding="0">'
say '<tr>'
call create_html_left_menu wwwpath_info, conf, action, wwwremote_user
if preview = .true then do
   say '<center><h1>Preview</h1></center>'
   say '<center><h3>Use the Back button to save the file</h3></center>'
   end
select
   when action = 'display' then do
      wiki = .wiki~new(conf, wwwpath_info)
      say wiki
      end
   when action = 'edit' then call edit_wiki
   otherwise do
      say '<td class="wiki_content_error">'
      say '<center>Error: Invalid action specified.</center>'
      say '</td>'
      end
   end
call create_html_right_menu wwwpath_info, conf, action, wwwremote_user
say '</tr>'
say '</table>'
call create_html_footer conf

return 0


/*----------------------------------------------------------------------------*/
/* Function: mod_rexx_setup                                                   */
/* Description: perform Mod_Rexx initialization.                              */
/*----------------------------------------------------------------------------*/

mod_rexx_setup:

-- get other stuff
call WWWGetArgs r
call WWWGetCookies r
-- set content-type and send the HTTP header
call WWWSendHTTPHeader r, "text/html"
return


/*----------------------------------------------------------------------------*/
/* Function: cgi_setup                                                        */
/* Description: perform CGI initialization.                                   */
/*----------------------------------------------------------------------------*/

cgi_setup:

-- get the CGI stuff into the Mod_Rexx variables
WWWAUTH_TYPE = value('AUTH_TYPE', , 'ENVIRONMENT')
WWWCONTENT_LENGTH = value('CONTENT_LENGTH', , 'ENVIRONMENT')
WWWCONTENT_TYPE = value('CONTENT_TYPE', , 'ENVIRONMENT')
WWWDEFAULT_TYPE = value('DEFAULT_TYPE', , 'ENVIRONMENT')
WWWFILENAME = value('FILENAME', , 'ENVIRONMENT')
WWWFNAMETEMPLATE = ''
WWWGATEWAY_INTERFACE = value('GATEWAY_INTERFACE', , 'ENVIRONMENT')
WWWHOSTNAME = value('HOSTNAME', , 'ENVIRONMENT')
WWWHTTP_USER_ACCEPT = value('HTTP_USER_ACCEPT', , 'ENVIRONMENT')
WWWHTTP_USER_AGENT = value('HTTP_USER_AGENT', , 'ENVIRONMENT')
WWWIS_MAIN_REQUEST = 1
WWWPATH_INFO = value('PATH_INFO', , 'ENVIRONMENT')
WWWPATH_TRANSLATED = value('PATH_TRANSLATED', , 'ENVIRONMENT')
WWWPOST_STRING = ''
WWWQUERY_STRING = value('QUERY_STRING', , 'ENVIRONMENT')
WWWREMOTE_ADDR = value('REMOTE_ADDR', , 'ENVIRONMENT')
WWWREMOTE_HOST = value('REMOTE_HOST', , 'ENVIRONMENT')
WWWREMOTE_IDENT = value('REMOTE_IDENT', , 'ENVIRONMENT')
WWWREMOTE_USER = value('REMOTE_USER', , 'ENVIRONMENT')
WWWREQUEST_METHOD = value('REQUEST_METHOD', , 'ENVIRONMENT')
WWWRSPCOMPILER = ''
WWWSCRIPT_NAME = value('SCRIPT_NAME', , 'ENVIRONMENT')
WWWSERVER_NAME = value('SERVER_NAME', , 'ENVIRONMENT')
WWWSERVER_ROOT = value('SERVER_ROOT', , 'ENVIRONMENT')
WWWSERVER_PORT = value('SERVER_PORT', , 'ENVIRONMENT')
WWWSERVER_PROTOCOL = value('SERVER_PRETOCOL', , 'ENVIRONMENT')
WWWSERVER_SOFTWARE = value('SERVER_SOFTWARE', , 'ENVIRONMENT')
WWWUNPARSEDURI = value('UNPARSEDURI', , 'ENVIRONMENT')
WWWURI = value('URI', , 'ENVIRONMENT')
WWWARGS.0 = 0
if wwwrequest_method = 'GET' then call parse_query_string wwwquery_string
else if wwwrequest_method = 'POST' then do
   if wwwcontent_length <> '' then do
      WWWPOST_STRING = charin(, , wwwcontent_length)
      end
   call parse_query_string wwwpost_string
   end
-- TODO: get the cookies
WWWCOOKIES.0 = 0
-- set content-type and send the HTTP header
say 'Content-Type: text/html;charset=utf-8'
say
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
/* Function: edit_wiki                                                        */
/* Description: edit the contents of a wiki file via a browser HTML page.     */
/*----------------------------------------------------------------------------*/

edit_wiki: procedure expose wwwpath_info conf

say '<td>'
say '<h1>Edit:' wwwpath_info'</h1>'

-- read in the wiki file contents
filename = conf['wiki_apdoc_dir'] || conf['wiki_ns_main'] || wwwpath_info
filestrm = .stream~new(filename)
status = filestrm~open('read')
if status <> 'READY:' then do
   say '<p>Error: Unable to open file.</p>'
   say '</td>'
   return
   end
contentsize = filestrm~query('size')
content = filestrm~charin(1, contentsize)
filestrm~close()
say '[current page size is <b>'contentsize'</b> bytes]'
say '<form action="/wiki.rex'wwwpath_info'?save" method="post">'
say '<textarea name="content" cols="100" rows="30" wrap="off">'
say content
say '</textarea>'
say '<br /><br /><input type="submit" name="submit" value="Save Changes" />'
say '<input type="submit" name="submit" value="Preview" />'
say '<input type="submit" name="submit" value="Cancel" />'
say '</form>'
say '<hr />'
say '<h3>Editing - Quick Help</h3>'
say '<dl>'
say '<dt><b>Emphasis Text</b></dt>'
say "<dd>''<i>italic text</i>'', '''<b>bold text</b>''', '''''<b><i>bold italics</i></b>'''''</dd>"
say '<dt><b>Headings</b></dt>'
say '<dd>=Level 1=, ==Level 2==, ===Level 3===, etc.</dd>'
say '<dt><b>Lists</b></dt>'
say '<dd>* bullets, # numbered, ; term : definition</dd>'
say '<dt><b>Links</b></dt>'
say '<dd>[url], [url description]</dd>'
say '<dt><b>Wiki Links</b></dt>'
say '<dd>[[wikipath]], [[wikipath | description]], [[namespace:wikipath | description]]</dd>'
say '</dl>'
say '</td>'

return


/*----------------------------------------------------------------------------*/
/* Function: save_wiki                                                        */
/* Description: save the wiki file.                                           */
/*----------------------------------------------------------------------------*/

save_wiki:

-- Note: for any of this to work properly on Linux, Apache needs to have write
-- access to the target subdirectory. Proper permissions are needed in order
-- for the backup file to be created and written as well as saving the new
-- wiki contents to the old file.

-- morph the filname into a full OS path
filename = conf['wiki_apdoc_dir'] || conf['wiki_ns_main'] || wwwpath_info
-- open and get the old contents
filestrm = .stream~new(filename)
--status = filestrm~open('both replace')
status = filestrm~open('write replace')
-- backup the old file
/*
oldcontent = filestrm~arrayin()
bakfilename = filename || '.' || date('S') || '-' || time()
bakfilestrm = .stream~new(bakfilename)
status = bakfilestrm~open('write replace')
bakfilestrm~arrayout(oldcontent)
bakfilestrm~close()
filestrm~seek('=1')
*/
-- find the content and write it (replace the contents) to the file
do i = 1 to wwwargs.0
   if wwwargs.i.!NAME = 'content' then do
      retc = filestrm~charout(wwwargs.i.!VALUE)
      leave
      end
   end
filestrm~close()

return


/*----------------------------------------------------------------------------*/
/* Function: preview_wiki                                                     */
/* Description: create a wiki preview file for the user.                      */
/*----------------------------------------------------------------------------*/

preview_wiki: procedure expose wwwpath_info conf wwwargs.

prefix = '/preview/'
-- morph the filname into a full OS path
filename = conf['wiki_apdoc_dir'] || conf['wiki_ns_main'] || prefix
filename = SysTempFileName(filename'?????')
-- open the preview wiki file
filestrm = .stream~new(filename)
status = filestrm~open('write replace')
-- save the content
do i = 1 to wwwargs.0
   if wwwargs.i.!NAME = 'content' then do
      content = wwwargs.i.!VALUE
      leave
      end
   end
retc = filestrm~charout(content)
filestrm~close()
wwwpath_info = filename~substr(filename~pos(prefix))

return


/*----------------------------------------------------------------------------*/
/* Function: send_redirect                                                    */
/* Description: send a redirect to the browser.                               */
/*----------------------------------------------------------------------------*/

send_redirect: procedure expose wwwpath_info action

url = '/wiki.rex' || wwwpath_info
if action = '' | action = 'display' then nop
else url = url || '?' || action
say '<?xml version="1.0"?>'
say '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "DTD/xhtml-transitional.dtd">'
say '<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">'
say '<head>'
say '<meta http-equiv="Content-Type" content="text/html;charset=utf-8" />'
say '<meta http-equiv="Refresh" content="0; 'url'" />'
say '</head>'
say '<body>'
say '</body>'
say '</html>'

return

::requires 'oorxwikiconf.rex'
::requires 'oorxwiki.cls'
::requires 'templates.rex'

