/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Description: Routines for parts of HTML pages.                             */
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


/*----------------------------------------------------------------------------*/
/* Routine: htmlheader                                                        */
/* Description: output the HTML page header                                   */
/*----------------------------------------------------------------------------*/

::routine htmlheader public
use strict arg title
say 'Content-Type: text/html;charset=utf-8'
say
say '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "DTD/xhtml1-transitional.dtd">'
say '<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">'
say '<head>'
say '<title>'title'</title>'
say '<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1" />'
say '<link rel="stylesheet" href="/css/normal.css" type="text/css" title="normal"/>'
say '<link rel="alternate stylesheet" href="/css/large.css" type="text/css" title="large"/>'
say '<link rel="alternate stylesheet" href="/css/user.css" type="text/css" title="user"/>'
say '<script type="text/javascript" src="/js/styleswitcher.js"></script>'
say '</head>'
say '<body>'
say '   <div id="body"> '
say '       <div class="bg-left">'
say
say '<!-- ###### header start ###### -->'
say '           <div id="header">       '
say '           <a href="/"><img style="vertical-align: middle;" src="/images/oorexx_tm.gif" alt="RexxLA" border="0" /></a>'
say '           <font size="+3">&nbsp;'title'</font>'
say '               <div id="header-right">'
say '              <!-- Google CSE Search Box Begins -->'
say '              <form id="searchbox_006593742925915806160:fc02weygiw4" action="http://www.google.com/cse">'
say '              <input type="hidden" name="cx" value="006593742925915806160:fc02weygiw4" />'
say '              <input name="q" type="text" size="20" />'
say '              <input type="submit" name="sa" value="Search" />'
say '              <input type="hidden" name="cof" value="FORID:0" />'
say '              </form>'
say '              <script type="text/javascript" src="http://www.google.com/coop/cse/brand?form=searchbox_006593742925915806160%3Afc02weygiw4"></script>'
say '              <!-- End Google CSE Search Box Begins -->'
say '              <p><b>Screen Styles:</b><br />'
say '                 <a href="#" onclick="setActiveStyleSheet(''normal''); return false;">Normal</a>'
say '                 <a href="#" onclick="setActiveStyleSheet(''large''); return false;">Large</a>'
say '                 <a href="#" onclick="setActiveStyleSheet(''user''); return false;">UserPref</a>'
say '              </p>'
say '           </div> <!--header-right-->'
say '           <br /><font size="-2">&nbsp;&nbsp;&nbsp;&nbsp;ooRexx logo by Julian Choy</font>'
say '           </div> <!--header-->'
say '<!-- ###### header end ###### -->'
return


/*----------------------------------------------------------------------------*/
/* Routine: htmlnavbar                                                        */
/* Description: output the HTML left navagation bar                           */
/*----------------------------------------------------------------------------*/

::routine htmlnavbar public
say
say '<!-- ###### menu start ###### -->'
say '           <div id="menu-box"> '
say '               <div id="menu">'
say '               <!-- The &nbsp; embedded in the links keeps the entire menu item'
say '                    together when the window is resized and the menu items are '
say '                    split into multiple lines'
say '               -->'
say '                   <a href="/">&nbsp;Home&nbsp;</a>&nbsp;'
say '                   <a href="http://sourceforge.net/projects/oorexx/">&nbsp;SourceForge Site&nbsp;</a>&nbsp;'
say '                   <a href="http://www.oorexx.org/contact.html">&nbsp;Contact&nbsp;Us&nbsp;</a>&nbsp;'
say '                   <a href="http://oorexx.wiki.sourceforge.net/">&nbsp;ooRexx&nbsp;Wiki&nbsp;</a>&nbsp;'
say '                   <a href="http://www.rexxla.org/">&nbsp;Rexx&nbsp;Language&nbsp;Association&nbsp;</a>&nbsp;'
say '               </div> <!--menu-->'
say '           </div> <!--menubox-->'
say '<!-- ###### menu end ###### -->'
return


/*----------------------------------------------------------------------------*/
/* Routine: htmlfooter                                                        */
/* Description: output the HTML page footer                                   */
/*----------------------------------------------------------------------------*/

::routine htmlfooter public
say
say '      <div id="footer">'
say '         <hr />'
say '         <img alt="Powered by Apache/ooRexx" src="/images/pb_apache_oorexx.gif" />'
say '         <br />&#169; Copyright 2007-2009 Rexx Language Association. All rights reserved.'
say '      </div>'
say '   </div> <!--body-->'
say
say '</body>'
say '</html>'
return

