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


::class cfgglobalumask public subclass linuxsa

::method init
use strict arg self~user, self~system, self~debug = .false
return


::method get_current
-- display the question
question = 'Is the global umask set to <computeroutput>077</computeroutput>?'
say question
-- get the answer
retc = self~execute('cat /etc/profile')
arr = self~arr_from_file(self~tfile)
status = 'Yes'
do line over arr
   if line~word(1) = 'umask' & line~word(2) \== '077' then do
      status = 'No'
      leave
      end
   end
retc = self~execute('cat /etc/bashrc')
arr = self~arr_from_file(self~tfile)
do line over arr
   if line~word(1) = 'umask' & line~word(2) \== '077' then do
      status = 'No'
      leave
      end
   end
retc = self~execute('cat /etc/csh.cshrc')
arr = self~arr_from_file(self~tfile)
do line over arr
   if line~word(1) = 'umask' & line~word(2) \== '077' then do
      status = 'No'
      leave
      end
   end
retc = self~execute('cat /etc/login.defs')
arr = self~arr_from_file(self~tfile)
do line over arr
   if line~word(1) = 'umask' & line~word(2) \== '077' then do
      status = 'No'
      leave
      end
   end
retc = self~execute('cat /etc/csh.login')
arr = self~arr_from_file(self~tfile)
do line over arr
   if line~word(1) = 'umask' & line~word(2) \== '077' then do
      status = 'No'
      leave
      end
   end
retc = self~execute('ls -l /etc/profile.d/*')
arr = self~arr_from_file(self~tfile)
do line over arr
   if line~substr(1, 1) = 'd' then iterate
   retc = self~execute('cat' line~word(line~words()))
   parr = self~arr_from_file(self~tfile)
   do pline over parr
      if pline~word(1) = 'umask' & pline~word(2) \== '077' then do
         status = 'No'
         leave
         end
      end
   end
-- set up the report section
rptarr = .array~new()
rptarr~append('<listitem>')
rptarr~append('<indexterm><primary>umask</primary></indexterm>')
rptarr~append('<indexterm><primary>/etc/profile.d/*</primary></indexterm>')
rptarr~append('<indexterm><primary>/etc/csh.login</primary></indexterm>')
rptarr~append('<indexterm><primary>/etc/login.defs</primary></indexterm>')
rptarr~append('<indexterm><primary>/etc/csh.cshrc</primary></indexterm>')
rptarr~append('<indexterm><primary>/etc/bashrc</primary></indexterm>')
rptarr~append('<indexterm><primary>/etc/profile</primary></indexterm>')
rptarr~append('<para>The system umask must be set to at least 077, or daemon processes may create world-writable files.</para>')
rptarr~append('<para>'question'</para>')
rptarr~append('<programlisting>'status'</programlisting>')
if status = 'No' then do
   rptarr~append('<note><title>Warning</title>')
   rptarr~append(self~msg6)
   rptarr~append('<para>The umask setting in one or more of the files')
   rptarr~append('<computeroutput>/etc/profile</computeroutput>,')
   rptarr~append('<computeroutput>/etc/bashrc</computeroutput>,')
   rptarr~append('<computeroutput>/etc/profile.d/*</computeroutput>,')
   rptarr~append('<computeroutput>/etc/login.defs</computeroutput>, or')
   rptarr~append('<computeroutput>/etc/csh.cshrc</computeroutput> is not 077.</para>')
   rptarr~append('<para>If you need a very secure system then you should consider changing the umask')
   rptarr~append('to 007 so that only owners have access to their own files (no user files are') 
   rptarr~append('world readable).</para>')
   rptarr~append('</note>')
   end
else do
   rptarr~append(self~msg7)
   end
rptarr~append('</listitem>')
return rptarr

                               
::requires './classes/linuxsa.cls'
             
