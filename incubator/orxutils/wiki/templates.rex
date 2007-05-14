/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Description: Routines for creating HTML for a wiki page.                   */
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
/*                                                                            */
/* Notes:                                                                     */
/*    These are template routines for creating HTML pages.                    */
/*                                                                            */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Routine: create_html_header                                                */
/* Description: create the HTML header for the wiki page.                     */
/*----------------------------------------------------------------------------*/

::routine create_html_header public
use arg conf

-- TODO: we will probably want to add some META tags

say '<?xml version="1.0"?>'
say '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "DTD/xhtml-transitional.dtd">'
say '<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">'
say '<head>'
say '<meta http-equiv="Content-Type" content="text/html;charset=utf-8" />'
say '<meta name="robots" content="index,nofollow" />'
say '<title>'conf['wiki_html_main_title']'</title>'
say '<link rel="stylesheet" type="text/css" charset="utf-8" media="all" href="'conf['wiki_html_stylesheet']'" />'
say '</head>'
say '<body>'
return


/*----------------------------------------------------------------------------*/
/* Routine: create_html_banner                                                */
/* Description: create the top HTML banner for the wiki page.                 */
/*----------------------------------------------------------------------------*/

::routine create_html_banner public
use arg conf

say '<div class="wiki_banner">'
say '<table width="100%" background="'conf['wiki_html_banner_background_image']'">'
say '<tr height="'conf['wiki_html_banner_height']'">'
say '<td width="'conf['wiki_html_banner_image_width']'" align="left" valign="middle">'
say '<img src="'conf['wiki_main_image']'" alt="'conf['wiki_main_image_alt_text']'" />'
say '</td>'
say '<td align="left" valign="middle">'
say '<h1>'conf['wiki_html_main_title']'</h1>'
say '</td>'
say '</tr>'
say '</table>'
say '</div>'
return


/*----------------------------------------------------------------------------*/
/* Routine: create_html_left_menu                                             */
/* Description: create the HTML left menu area.                               */
/*----------------------------------------------------------------------------*/

::routine create_html_left_menu public
use arg filename, conf, action, username

say '<td width="200" align="center" valign="top">'
say '<div class="wiki_left_menus">'

-- Main wiki menu
say '<table width="100%" cellspacing="0">'
say '<tr>'
say '<td class="wiki_menu_header">'
say 'Wiki'
say '</td>'
say '</tr>'
say '<tr>'
say '<td class="wiki_menu_content" onclick="location.href=''/wiki.rex/Main_Page''">'
say 'FrontPage'
say '</td>'
say '</tr>'
say '<tr>'
say '<td class="wiki_menu_content" onclick="location.href=''/wiki.rex/ooRexx/Main_Page''">'
say 'ooRexx'
say '</td>'
say '</tr>'
say '<tr>'
say '<td class="wiki_menu_content" onclick="location.href=''/wiki.rex/help/help''">'
say 'HelpContents'
say '</td>'
say '</tr>'
say '</table>'

-- Main page menu
say '<br />'
say '<table width="100%" cellspacing="0">'
say '<tr>'
say '<td class="wiki_menu_header">'
say 'Page'
say '</td>'
if action = 'edit' then do
   say '</tr>'
   say '<td class="wiki_menu_content" onclick="location.href=''/wiki.rex/help/edit''">'
   say 'Edit Help'
   say '</td>'
   say '</tr>'
   end
else do
   say '</tr>'
   say '<td class="wiki_menu_content" onclick="location.href=''/wiki.rex'filename'?edit''">'
   say 'Edit'
   say '</td>'
   say '</tr>'
   end
say '</tr>'
say '</table>'

-- Main user menu
say '<br />'
say '<table width="100%" cellspacing="0">'
say '<tr>'
say '<td class="wiki_menu_header">'
say 'User'
say '</td>'
say '</tr>'
if username = '' then do
   say '<tr>'
   say '<td class="wiki_menu_content">'
   say 'Logon'
   say '</td>'
   say '</tr>'
   end
else do
   say '<tr>'
   say '<td class="wiki_menu_content">'
   say username
   say '</td>'
   say '</tr>'
   end
if username <> '' then do
   say '<tr>'
   say '<td class="wiki_menu_content">'
   say 'Preferences'
   say '</td>'
   say '</tr>'
   end
say '</table>'

say '<br /><img src="'conf['wiki_html_main_copyright_image']'" />'
say '<p>&copy;' conf['wiki_html_main_copyright']
curdate = date('N')
parse var curdate . . year
say year'. All rights reserved.</p>'
say '</div>'
say '</td>'
return


/*----------------------------------------------------------------------------*/
/* Routine: create_html_right_menu                                            */
/* Description: create the HTML right menu area.                              */
/*----------------------------------------------------------------------------*/

::routine create_html_right_menu public
use arg filename, conf, action, username
return


/*----------------------------------------------------------------------------*/
/* Routine: create_html_footer                                                */
/* Description: create the HTML footer for the wiki page.                     */
/*----------------------------------------------------------------------------*/

::routine create_html_footer public
use arg conf

src = conf['wiki_html_main_footer_image']
say '<div class="wiki_footer">'
say '<br /><center>'
if src <> '' then say '<img src="'src'" /><br />'
say 'oorxwiki version' conf['wiki_version']
say '</center>'
say '</div>'
say '</body>'
say '</html>'
return


