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
/* Description: Adds a footer to a standard HTML page via the Mod_ooRexx      */
/*              Apache                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/* version of this script */
script_version = 'v2.2.0'

/* these are some typical Apache return codes */
OK        = 0      /* Module has handled this stage. */
NOT_FOUND = 404    /* Main document not found. */

/* get the Apache request record ptr */
r = arg(1)

body = '</BODY>'

/* try to open the main document HTML file */
retc = stream(wwwfilename, 'c', 'open read')
if retc <> 'READY:' then do
   return NOT_FOUND
   end

/* set content-type and send the HTTP header */
call WWWSendHTTPHeader r, "text/html"
call WWWGetArgs r

/* read in the document and look for the </BODY> tag */
call on notready name seteof
eof = 0
line = linein(wwwfilename)
x = pos(body, translate(line))
do while x = 0 & eof = 0
   say line
   line = linein(wwwfilename)
   x = pos(body, translate(line))
   end
if eof = 1 then return OK

/* we found a </BODY> tag so send out the footer */
say substr(line, 1, x - 1)
call send_footer
say substr(line, x)

/* read in the rest of the document */
line = linein(wwwfilename)
do while eof = 0
   say line
   line = linein(wwwfilename)
   end

/* we are done */
return OK


/* function which indicates end-of-file */
seteof:
eof = 1
return


/* send the footer */
send_footer:
say '<br />'
say '<hr />'
say '<br />'
say '&copy; 2001 <a href="http://www.ibm.com/">IBM Corporation</a>'
say '<br />'
say '<em>Last Modified:' date()'</em>'
say '<br />'
say '<em>Footer added by Mod_Rexx.</em>'
say '<br />'
return

