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


::class svcsshconfig public subclass linuxsa

::method init
use strict arg self~user, self~system, self~debug = .false
return


::method get_current
-- display the question
question = 'Is the <computeroutput>/etc/ssh/sshd_config</computeroutput> file configuration correct?'
say question
-- get the answer
retc = self~execute('cat /etc/ssh/sshd_config')
arr = self~arr_from_file(self~tfile)
-- set up the report section
rptarr = .array~new()
rptarr~append('<listitem>')
rptarr~append('<indexterm><primary>/etc/ssh/sshd_config</primary></indexterm>')
rptarr~append('<indexterm><primary>SSH configuration</primary></indexterm>')
rptarr~append('<para>'question'</para>')
fail = .false
do line over arr
   if line~pos('ClientAliveInterval 0') > 0 then do
      rptarr~append('<note><title>Warning</title>')
      rptarr~append(self~msg6)
      rptarr~append('<para>The ClientAliveInterval option should be set to something other than zero.</para>')
      rptarr~append('</note>')
      fail = .true
      end
   else if line~pos('IgnoreRhosts no') > 0 then do
      rptarr~append('<note><title>Warning</title>')
      rptarr~append(self~msg6)
      rptarr~append('<para>The IgnoreRhosts option should be set to "yes".</para>')
      rptarr~append('</note>')
      fail = .true
      end
   else if line~pos('HostbasedAuthentication yes') > 0 then do
      rptarr~append('<note><title>Warning</title>')
      rptarr~append(self~msg6)
      rptarr~append('<para>The HostbasedAuthentication option should be set to "no".</para>')
      rptarr~append('</note>')
      fail = .true
      end
   else if line~pos('PermitRootLogin yes') > 0 then do
      rptarr~append('<note><title>Warning</title>')
      rptarr~append(self~msg6)
      rptarr~append('<para>The PermitRootLogin option should be set to "no".</para>')
      rptarr~append('</note>')
      fail = .true
      end
   else if line~pos('PermitEmptyPasswords yes') > 0 then do
      rptarr~append('<note><title>Warning</title>')
      rptarr~append(self~msg6)
      rptarr~append('<para>The PermitEmptyPasswords option should be set to "no".</para>')
      rptarr~append('</note>')
      fail = .true
      end
   else if line~pos('Banner /some/path') > 0 then do
      rptarr~append('<note><title>Warning</title>')
      rptarr~append(self~msg6)
      rptarr~append('<para>The Banner option should be set to a path for the login banner.</para>')
      rptarr~append('</note>')
      fail = .true
      end
   else if line~pos('PermitUserEnvironment yes') > 0 then do
      rptarr~append('<note><title>Warning</title>')
      rptarr~append(self~msg6)
      rptarr~append('<para>The PermitUserEnvironment option should be set to "no".</para>')
      rptarr~append('</note>')
      fail = .true
      end
   else nop
   end
if fail = .false then do
   rptarr~append(self~msg2)
   end
rptarr~append('</listitem>')
return rptarr

                               
::requires './classes/linuxsa.cls'
             
