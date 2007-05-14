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


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Notes:                                                                     */
/*    This file is actually designed to be included in an ooRexx program via  */
/*    a ::requires directive. It has one routine which sets the values in     */
/*    an ooRexx directory class. The entries in the directory class form the  */
/*    global configuration settings for the application.                      */
/*                                                                            */
/*    Do NOT remove any values from the set_wikiconf routine.                 */
/*                                                                            */
/*    The reason this file is broken out is so that all configuration info    */
/*    is in a single place. This makes it easy for a user to find and modify  */
/*    the configuration settings.                                             */
/*                                                                            */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Routine: get_wikiconf                                                      */
/* Description: returns an ooRexx directory class instance of the config.     */
/*----------------------------------------------------------------------------*/

::routine get_wikiconf public
conf = .directory~new

/* this needs to be modified to apache's document root directory */
conf['wiki_apdoc_dir'] = '/var/www/html' -- Linux version
-- conf['wiki_apdoc_dir'] = 'c:/Program Files/Apache Software Foundation/Apache2.2/htdocs' -- Windows version

/* the following are the wiki namespaces relative to Apache's doc root */
conf['wiki_ns_main'] = '/wikidb/wikins' -- wiki document subdirectory
conf['wiki_ns_image'] = '/wikidb/images' -- wiki images
conf['wiki_ns_html'] = '/wikidb/html' -- wiki static HTML

/* these are the html page settings for all wiki pages */
conf['wiki_main_page'] = 'Main_Page' -- default page file name
conf['wiki_html_main_title'] = 'Open Object Rexx Wiki'
conf['wiki_html_stylesheet'] = conf['wiki_ns_html'] || '/default.css'
-- this is the html page banner settings for all wiki pages
conf['wiki_html_banner_image_width'] = 165
conf['wiki_html_banner_height'] = 113
-- this is the copyright notice for all wiki pages
conf['wiki_html_main_copyright'] = '<a href="http://www.rexxla.org/">Rexx Language Association</a>'
conf['wiki_html_main_copyright_image'] = conf['wiki_ns_image'] || '/rexxla.png'

/* these are the html page images for all wiki pages */
conf['wiki_main_image'] = conf['wiki_ns_image'] || '/rexx3_orig_size_transparent.gif'
conf['wiki_main_image_alt_text'] = 'Open Object Rexx'
conf['wiki_html_banner_background_image'] = conf['wiki_ns_image'] || '/rexxback.png'
conf['wiki_html_main_footer_image'] = conf['wiki_ns_image'] || '/pb_apache_oorexx.png'

return conf

