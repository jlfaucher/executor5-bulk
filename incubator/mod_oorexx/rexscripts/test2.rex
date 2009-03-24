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
/* Description: Test page for the Mod_ooRexx module (serialization test).     */
/*                                                                            */
/*----------------------------------------------------------------------------*/

call rxfuncadd 'SysLoadFuncs', 'rexxutil', 'SysLoadFuncs'
call SysLoadFuncs

/* version of this script */
script_version = 'v2.2.0'

call rxfuncadd 'SysLoadFuncs', 'rexxutil', 'SysLoadFuncs'
call SysLoadFuncs

/* these are some typical Apache return codes */
DECLINED  = -1    /* Module declines to handle */
DONE      = -2    /* Module has served the response completely */
OK        = 0     /* Module has handled this stage. */
NOT_FOUND = 404   /* Main document not found. */

/* get environment stuff */
parse source env . path
if env='OS/2' then envname='OS2ENVIRONMENT'
else envname='ENVIRONMENT'
parse upper version v

/* get the Apache request record ptr */
r = arg(1)

/* set content-type and send the HTTP header */
call WWWSendHTTPHeader r, "text/html" /* note: this call is NOT optional */

/* start sending the html page */
say "<HTML>"
say "<HEAD>"
say "<TITLE>Sample HTML Page From Rexx</TITLE>"
say "</HEAD>"
say "<BODY>"
say "<H1>Serialization Test for Mod_Rexx</H1>"

say '<p>The Mod_Rexx version string is "'WWWGetVersion()'"'

say '<p>The current RFC 822 formatted time at the server is "'WWWHTTP_time(r)'"'

say '<p>Sleeping 10 seconds'
call SysSleep 10

say '<p>The current RFC 822 formatted time at the server is "'WWWHTTP_time(r)'"'

say "</BODY>"
say "</HTML>"
return OK

