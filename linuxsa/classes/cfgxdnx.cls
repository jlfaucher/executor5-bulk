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


::class cfgxdnx public subclass linuxsa

::method init
use strict arg self~user, self~system, self~debug = .false
return


::method get_current
-- display the question
question = 'Does the system support XD or NX?'
say question
-- get the answer
retc = self~execute('cat /proc/cpuinfo')
arr = self~arr_from_file(self~tfile)
status = 'No'
do line over arr
   if line~word(1) = 'flags' then do
      if line~pos(' xd ') > 0 then status = 'Yes'
      else if line~pos(' nx ') > 0 then status = 'Yes'
      leave
      end
   end
-- set up the report section
rptarr = .array~new()
rptarr~append('<listitem>')
rptarr~append('<indexterm><primary>XD processor support</primary></indexterm>')
rptarr~append('<indexterm><primary>NX processor support</primary></indexterm>')
rptarr~append('<indexterm><primary>processor support flags</primary></indexterm>')
rptarr~append('<indexterm><primary>/proc/cpuinfo</primary></indexterm>')
rptarr~append('<para>Later 32-bit processors in the x86 family support the ability to prevent code execution on a per memory page')
rptarr~append('basis. Generically and on AMD processors, this ability is called No Execute (NX), while on Intel processors it')
rptarr~append('is called Execute Disable (XD). This ability can help prevent exploitation of buffer overflow vulnerabilities and')
rptarr~append('should be activated whenever possible. Extra steps must be taken to ensure that this protection is enabled on')
rptarr~append('32-bit x86 systems. Other processors, such as Itanium, POWER, and 64-bit x86 (both AMD64 or Intel 64), have')
rptarr~append('included such support since inception and the standard kernel for those platforms supports the feature.</para>')
rptarr~append('<para>'question'</para>')
rptarr~append('<programlisting>'status'</programlisting>')
if status = 'No' then do
   rptarr~append('<para>You should consider upgrading the CPU to one that does support XD or NX.</para>')
   end
else do
   rptarr~append('<para>Ensure you are using a kernel that supports these flags.</para>')
   end
rptarr~append('</listitem>')
return rptarr

                               
::requires './classes/linuxsa.cls'

