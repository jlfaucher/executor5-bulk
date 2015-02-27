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


::class cfgconsole public subclass linuxsa

::method init
use strict arg self~user, self~system, self~debug = .false
return


::method get_current
-- display the question
question = 'Are non-root users restricted from device ownership?'
say question
-- get the first answer
retc = self~execute('cat /etc/security/console.perms.d/50-default.perms')
arr = self~arr_from_file(self~tfile)
flag = .false
newarr = .array~new()
do line over arr
   if line~substr(1, 1) = '#' & line~pos('permission definitions') > 0 then flag = .true
   if line~substr(1, 1) = '#' then iterate
   if line~strip() = '' then iterate
   if flag & line~word(1) = '<console>' then newarr~append(line)
   end
-- set up the first report section
rptarr = .array~new()
rptarr~append('<listitem>')
rptarr~append('<indexterm><primary>/etc/security/console.perms.d/50-default.perms</primary></indexterm>')
rptarr~append('<indexterm><primary>device ownerships</primary></indexterm>')
rptarr~append('<para>'question'</para>')
rptarr~append('<para>Testing <computeroutput>/etc/security/console.perms.d/50-default.perms</computeroutput></para>')
if newarr~items > 0 then do
   rptarr~append('<programlisting>No</programlisting>')
   rptarr~append(self~msg6)
   rptarr~append('<programlisting>')
   do line over newarr
      line = line~changestr('&', '&amp;')
      line = line~changestr('<', '&lt;')
      line = line~changestr('>', '&gt;')
      arr~append(line)
      end
   rptarr~append('</programlisting>')
   rptarr~append('<note><title>Warning</title>')
   rptarr~append('<para>The lines listed above from the')
   rptarr~append('<computeroutput>/etc/security/console.perms.d/50-default.perms</computeroutput>')
   rptarr~append('file should all be prepended with a "#" symbol to restrict non-root users')
   rptarr~append('from owning devices.</para>')
   rptarr~append('</note>')
   end
else do
   rptarr~append('<programlisting>Yes</programlisting>')
   rptarr~append(self~msg7)
   end
-- get the second answer
retc = self~execute('cat /etc/security/console.perms')
arr = self~arr_from_file(self~tfile)
newarr = .array~new()
do line over arr
   if line~substr(1, 1) = '#' then iterate
   if line~strip() = '' then iterate
   if line = '<console>=tty[0-9][0-9]* vc/[0-9][0-9]* :[0-9]+\.[0-9]+ :[0-9]+' then newarr~append(line)
   if line = '<xconsole>=:[0-9]+\.[0-9]+ :[0-9]+' then newarr~append(line)
   end
-- set up the second report section
rptarr~append('<para>Testing <computeroutput>/etc/security/console.perms</computeroutput></para>')
if newarr~items > 0 then do
   rptarr~append('<programlisting>No</programlisting>')
   rptarr~append(self~msg6)
   rptarr~append('<programlisting>')
   do line over newarr
      line = line~changestr('&', '&amp;')
      line = line~changestr('<', '&lt;')
      line = line~changestr('>', '&gt;')
      arr~append(line)
      end
   rptarr~append('</programlisting>')
   rptarr~append('<note><title>Warning</title>')
   rptarr~append('<para>The lines listed above from the')
   rptarr~append('<computeroutput>/etc/security/console.perms</computeroutput>')
   rptarr~append('file should be changed to the following:</para>')
   rptarr~append('</note>')
   rptarr~append('<programlisting>')
   rptarr~append('&lt;console&gt;=tty[0-9][0-9]* vc/[0-9][0-9]* :0\.[0-9] :')
   rptarr~append('&lt;xconsole&gt;=:0\.[0-9] :0')
   rptarr~append('</programlisting>')
   end
else do
   rptarr~append('<programlisting>Yes</programlisting>')
   rptarr~append(self~msg7)
   end
rptarr~append('</listitem>')
return rptarr

                               
::requires './classes/linuxsa.cls'

