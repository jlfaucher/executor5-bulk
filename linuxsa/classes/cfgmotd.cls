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


::class cfgmotd public subclass linuxsa

::attribute vlist

::method init
expose vlist
use strict arg self~user, self~system, self~debug = .false
vlist = .array~new()
vlist~append('$CDPATH')
vlist~append('$EUID')
vlist~append('$GROUPS')
vlist~append('$HOME')
vlist~append('$HOST')
vlist~append('$MAILPATH')
vlist~append('$PATH')
vlist~append('$PPID')
vlist~append('$PWD')
vlist~append('$UID')
return


::method get_current
-- display the question
question = 'Does the <computeroutput>/etc/motd</computeroutput> or <computeroutput>/etc/issue</computeroutput> files contain sensitive information?'
say question
-- get the answer
retc = self~execute('cat /etc/motd')
arr = self~arr_from_file(self~tfile)
if arr <> .nil then do line over arr
   do varent over vlist
      if line~pos(varent) > 0 then return 'Yes'
      end
   end
retc = self~execute('cat /etc/issue')
arr = self~arr_from_file(self~tfile)
status = 'No'
if arr <> .nil then do line over arr
   do varent over vlist
      if line~pos(varent) > 0 then status = 'Yes'
      end
   end
-- set up the report section
rptarr = .array~new()
rptarr~append('<listitem>')
rptarr~append('<indexterm><primary>/etc/motd</primary></indexterm>')
rptarr~append('<indexterm><primary>/etc/issue</primary></indexterm>')
rptarr~append('<para>When a user logs on to a console the system can display a set of messages to')
rptarr~append('the user which are contained is system files. The messages displayed should not')
rptarr~append('contain sensitive information or hints that can provide information to a user')
rptarr~append('in creating attack mechanisims. Sensitive information might include the system') 
rptarr~append('TCP/IP address, host name, PATH information, etc.</para>')
rptarr~append('<para>'question'</para>')
rptarr~append('<programlisting>'status'</programlisting>')
if status = 'Yes' then do
   rptarr~append('<note><title>Warning</title>')
   rptarr~append(self~msg6)
   rptarr~append('<para>Check the contents of the files')
   rptarr~append('<computeroutput>/etc/motd</computeroutput> and')
   rptarr~append('<computeroutput>/etc/issue</computeroutput> for sensitive contect and')
   rptarr~append('remove any such content.</para>')
   rptarr~append('</note>')
   end
else do
   rptarr~append(self~msg7)
   end
rptarr~append('</listitem>')
return rptarr

                               
::requires './classes/linuxsa.cls'
             
