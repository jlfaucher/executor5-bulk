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


::class prmgrub public subclass linuxsa

::method init
use strict arg self~user, self~system, self~debug = .false
return


::method get_current
-- display the question
question = 'Are the grub configuration file permissions correct?'
say question
-- get the answer
retc = self~execute('ls -l /boot/grub2/grub.cfg')
arr = self~arr_from_file(self~tfile)
if arr~items() = 0 then do
   retc = self~execute('ls -l /boot/grub/grub.cfg')
   arr = self~arr_from_file(self~tfile)
   end
if arr~items() = 0 then do
   retc = self~execute('ls -l /boot/grub/grub.conf')
   arr = self~arr_from_file(self~tfile)
   end
if arr~items() = 0 then do
   retc = self~execute('ls -l /etc/grub.conf')
   arr = self~arr_from_file(self~tfile)
   end
status = 'Yes'
if arr~items() = 0 then status = 'Cannot find grub configuration file.'
else if arr[1]~word(1)~substr(1, 10) <> self~bp_perm6 then status = 'No'
else if arr[1]~word(3) <> self~bp_owner then status = 'No'
else if arr[1]~word(4) <> self~bp_grp then status = 'No'
-- set up the report section
rptarr = .array~new()
rptarr~append('<listitem>')
rptarr~append('<indexterm><primary>/boot/grub2/grub.cfg permissions</primary></indexterm>')
rptarr~append('<indexterm><primary>/boot/grub/grub.conf permissions</primary></indexterm>')
rptarr~append('<indexterm><primary>/etc/grub.conf permissions</primary></indexterm>')
rptarr~append('<para>If the permissions for file <computeroutput>grub.cfg</computeroutput> are not "-rw-rw-r--" and the user and')
rptarr~append('group are not root then then the file is vulnerable.</para>')
rptarr~append('<para>'question'</para>')
rptarr~append('<programlisting>'status'</programlisting>')
if status = 'Cannot find grub configuration file.' > 0 then do
   rptarr~append('<note><title>Warning</title>')
   rptarr~append(self~msg1)
   rptarr~append('<para>The system could be a non-Intel architecture and thus grub would not')
   rptarr~append('be used as the boot loader.</para>')
   rptarr~append('</note>')
   end
else if status = 'No' > 0 then do
   rptarr~append('<note><title>Warning</title>')
   rptarr~append(self~msg3)
   rptarr~append('<para>The grub configuration file has bad permissions, or the group/world do not belong to root.</para>')
   rptarr~append('</note>')
   end
else do
   rptarr~append(self~msg2)
   end
rptarr~append('</listitem>')
return rptarr

                               
::requires './classes/linuxsa.cls'
             
