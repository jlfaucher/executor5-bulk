/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2015-2015 Rexx Language Association. All rights reserved.    */
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


::class svcsshprotocol public subclass linuxsa

::method init
use strict arg self~user, self~system, self~debug = .false
return


::method get_current
-- display the question
question = 'Review the <computeroutput>sshd_config</computeroutput> protocol entries.'
say question
-- get the answer
retc = self~execute('cat /etc/ssh/sshd_config')
arr = self~arr_from_file(self~tfile)
-- set up the report section
rptarr = .array~new()
rptarr~append('<listitem>')
rptarr~append('<indexterm><primary>/etc/ssh/sshd_config</primary></indexterm>')
rptarr~append('<indexterm><primary>SSH protocol</primary></indexterm>')
rptarr~append('<para>Only SSH protocol version 2 connections should be permitted. Version 1 of the protocol contains security')
rptarr~append('vulnerabilities. The default setting shipped in the configuration file is correct, but it is important enough to')
rptarr~append('check.</para>')
rptarr~append('<para>'question'</para>')
entry = .false
do line over arr
   parse var ptype proto .
   if ptype = 'PROTOCOL' & proto = 2 then entry = .true
   end
if entry = .false then do
   rptarr~append('<note><title>Warning</title>')
   rptarr~append('<para>The PROTOCOL entry in the file is not safe. Please review the entry.</para>')
   rptarr~append('</note>')
   end
else do
   rptarr~append('<para>The PROTOCOL entry in the file is correct.</para>')
   end
rptarr~append('</listitem>')
return rptarr

                               
::requires './classes/linuxsa.cls'
             
