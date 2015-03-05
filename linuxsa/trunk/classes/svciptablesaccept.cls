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


::class svciptablesaccept public subclass linuxsa

::method init
use strict arg self~user, self~system, self~debug = .false
return


::method get_current
-- display the question
question = 'Review the <computeroutput>iptables</computeroutput> ACCEPT rules.'
say question
-- get the answer
if .local~sysdist = 'UB' then do
   retc = self~execute('iptables -L')
   end
else do
   retc = self~execute('/usr/sbin/iptables -L')
   end
arr = self~arr_from_file(self~tfile)
-- set up the report section
rptarr = .array~new()
rptarr~append('<listitem>')
rptarr~append('<indexterm><primary>iptables daemon</primary></indexterm>')
rptarr~append('<para>'question'</para>')
rptarr~append('<programlisting>')
lines = .false
do line over arr
   parse var line target prot opt source destination comment
   comment = comment~strip()
   if target = 'ACCEPT' then do
      rptarr~append(target prot opt source destination '   'comment)
      lines = .true
      end
   end
rptarr~append('</programlisting>')
if lines = .true then do
   rptarr~append('<note><title>Warning</title>')
   rptarr~append('<para>All of the rules above should be reviewed to ensure they comply with the')
   rptarr~append('local security policy.</para>')
   rptarr~append('</note>')
   end
rptarr~append(self~msg14)
rptarr~append('</listitem>')
return rptarr

                               
::requires './classes/linuxsa.cls'
             
