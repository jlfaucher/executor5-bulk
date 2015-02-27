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


::class prmetcfilesystems public subclass linuxsa

::method init
use strict arg self~user, self~system, self~debug = .false
return


::method get_current
-- display the question
question = 'Are the permissions correct for the file <computeroutput>/etc/filesystems</computeroutput>?'
say question
-- get the answer
retc = self~execute('ls -l /etc/filesystems')
arr = self~arr_from_file(self~tfile)
if arr~items > 0 then item = arr[1]
else item = ''
-- set up the report section
rptarr = .array~new()
rptarr~append('<listitem>')
rptarr~append('<indexterm><primary>/etc/filesystems permissions</primary></indexterm>')
rptarr~append('<para>If the permissions for file <computeroutput>/etc/filesystems</computeroutput>')
rptarr~append('are not "-rw-r--r--" and the user and group are not')
rptarr~append('root then the file is vulnerable.</para>')
rptarr~append('<para>'question'</para>')
if item = '' then do
   rptarr~append('<programlisting>File /etc/filesystems not found.</programlisting>')
   rptarr~append('<note><title>Warning</title>'self~msg1)
   rptarr~append('</note>')
   end
else do
   rptarr~append('<programlisting>' || item || '</programlisting>')
   flag = .true
   if item~word(1)~substr(1, 10) <> self~bp_perm4 then do
      rptarr~append('<note><title>Warning</title>'self~msg3)
      rptarr~append('</note>')
      flag = .false
      end
   if item~word(3) <> self~bp_owner then do
      rptarr~append('<note><title>Warning</title>'self~msg4)
      rptarr~append('</note>')
      flag = .false
      end
   if item~word(4) <> self~bp_grp then do
      rptarr~append('<note><title>Warning</title>'self~msg5)
      rptarr~append('</note>')
      flag = .false
      end
   if flag then do
      rptarr~append(self~msg2)
      end
   end
rptarr~append('</listitem>')
return rptarr

                               
::requires './classes/linuxsa.cls'
             
