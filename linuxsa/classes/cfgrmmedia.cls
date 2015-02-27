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


::class cfgrmmedia public subclass linuxsa

::method init
use strict arg self~user, self~system, self~debug = .false
return


::method get_current
-- display the question
question = 'Are the nodev, nosuid and noexec options being used on removable media?'
say question
-- get the answer
retc = self~execute('cat /etc/fstab')
arr = self~arr_from_file(self~tfile)
status = 'Yes'
do line over arr
   if line~substr(1, 1) = '#' then iterate
   if line~word(2)~pos('cdrom') > 0 then do
      if line~word(4)~pos('nodev') = 0 then status = 'No'
      if line~word(4)~pos('nosuid') = 0 then status = 'No'
      if line~word(4)~pos('noexec') = 0 then status = 'No'
      end
   if line~word(2)~pos('dvd') > 0 then do
      if line~word(4)~pos('nodev') = 0 then status = 'No'
      if line~word(4)~pos('nosuid') = 0 then status = 'No'
      if line~word(4)~pos('noexec') = 0 then status = 'No'
      end
   if line~word(2)~pos('floppy') > 0 then do
      if line~word(4)~pos('nodev') = 0 then status = 'No'
      if line~word(4)~pos('nosuid') = 0 then status = 'No'
      if line~word(4)~pos('noexec') = 0 then status = 'No'
      end
   if line~word(3)~pos('iso9660') > 0 then do
      if line~word(4)~pos('nodev') = 0 then status = 'No'
      if line~word(4)~pos('nosuid') = 0 then status = 'No'
      if line~word(4)~pos('noexec') = 0 then status = 'No'
      end
   if line~word(3)~pos('vfat') > 0 then do
      if line~word(4)~pos('nodev') = 0 then status = 'No'
      if line~word(4)~pos('nosuid') = 0 then status = 'No'
      if line~word(4)~pos('noexec') = 0 then status = 'No'
      end
   if line~word(3)~pos('msdos') > 0 then do
      if line~word(4)~pos('nodev') = 0 then status 'No'
      if line~word(4)~pos('nosuid') = 0 then status 'No'
      if line~word(4)~pos('noexec') = 0 then status 'No'
      end
   end
-- set up the report section
rptarr = .array~new()
rptarr~append('<listitem>')
rptarr~append('<indexterm><primary>nodev</primary></indexterm>')
rptarr~append('<indexterm><primary>nosuid</primary></indexterm>')
rptarr~append('<indexterm><primary>noexec</primary></indexterm>')
rptarr~append('<indexterm><primary>/etc/fstab</primary></indexterm>')
rptarr~append('<indexterm><primary>fstab</primary></indexterm>')
rptarr~append('<indexterm><primary>removable media</primary></indexterm>')
rptarr~append('<indexterm><primary>media, removable</primary></indexterm>')
rptarr~append('<para>In general, execution of files mounted via NFS')
rptarr~append('should be considered risky because of the possibility that an adversary could intercept the request and substitute')
rptarr~append('a malicious file. Allowing setuid files to be executed from remote servers is particularly risky, both for this reason')
rptarr~append('and because it requires the clients to extend root-level trust to the NFS server.</para>')
rptarr~append('<para>'question'</para>')
rptarr~append('<programlisting>'status'</programlisting>')
if status = 'No' then do
   rptarr~append('<note><title>Warning</title>')
   rptarr~append(self~msg6)
   rptarr~append('<para>One or more removable media mount points in do not have the')
   rptarr~append('<computeroutput>nodev</computeroutput>,')
   rptarr~append('<computeroutput>nosuid</computeroutput>, or the')
   rptarr~append('<computeroutput>noexec</computeroutput> option set.</para>')
   rptarr~append('</note>')
   end
else do
   rptarr~append(self~msg7)
   end
rptarr~append('</listitem>')
return rptarr

                               
::requires './classes/linuxsa.cls'

