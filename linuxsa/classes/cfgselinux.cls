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


::class cfgselinux public subclass linuxsa

::method init
use strict arg self~user, self~system, self~debug = .false
return


::method get_current
-- display the question
question = 'In what mode is SELinux running?'
say question
-- get the answer
status = 'unknown'
if .local~sysdist() = 'RH' then do
   retc = self~execute('cat /etc/sysconfig/selinux')
   arr = self~arr_from_file(self~tfile)
   if arr~items = 0 then status = 'not installed'
   else do line over arr
      if line~pos('SELINUX=') = 1 then do
         parse var line . '=' status .
         leave
         end
      end
   end
else do -- 'SU'
   retc = self~execute('cat /etc/sysconfig/selinux-policy')
   arr = self~arr_from_file(self~tfile)
   if arr~items = 0 then status = 'not installed'
   else do line over arr
      if line~pos('SELINUX=') = 1 then do
         parse var line . '=' status .
         leave
         end
      end
   end
-- set up the report section
rptarr = .array~new()
rptarr~append('<listitem>')
rptarr~append('<indexterm><primary>SELinux</primary></indexterm>')
rptarr~append('<indexterm><primary>/etc/sysconfig/selinux</primary></indexterm>')
rptarr~append('<para>SELinux provides a Mandatory Access Control (MAC) system that greatly augments')
rptarr~append('the DAC model. Under SELinux, every process and every object (e.g. file, socket,')
rptarr~append('pipe) on the system is given a security context, a label which include detailed')
rptarr~append('type information about the object. The kernel allows processes to access objects')
rptarr~append('only if that access is explicitly allowed by the policy in effect. The policy')
rptarr~append('defines transitions, so that a user can be allowed to run software, but the')
rptarr~append('software can run under a different context than the user''s default. This')
rptarr~append('automatically limits the damage that the software can do to files accessible by the')
rptarr~append('calling user - the user does not need to take any action to gain this benefit.')
rptarr~append('</para>')
rptarr~append('<para>'question'</para>')
rptarr~append('<programlisting>'status'</programlisting>')
if status = 'enforcing' then do
   rptarr~append(self~msg7)
   end
else if status = 'permissive' then do
   rptarr~append('<note><title>Warning</title>')
   rptarr~append(self~msg6)
   rptarr~append('<para>SELinux is not in enforcing mode. This does not meet best practice requirements.</para>')
   rptarr~append('</note>')
   end
else if status = 'disabled' then do
   rptarr~append('<note><title>Warning</title>')
   rptarr~append(self~msg6)
   rptarr~append('<para>SELinux is not in enforcing mode. This does not meet best practice requirements.</para>')
   rptarr~append('</note>')
   end
else do
   rptarr~append('<note><title>Warning</title>')
   rptarr~append(self~msg6)
   rptarr~append('<para>SELinux does not appear to be installed. This does not meet best practice requirements.</para>')
   rptarr~append('</note>')
   end
rptarr~append('</listitem>')
return rptarr

                               
::requires './classes/linuxsa.cls'
             
