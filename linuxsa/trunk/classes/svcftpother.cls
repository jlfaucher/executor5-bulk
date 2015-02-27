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


::class svcftpother public subclass linuxsa

::method init
use strict arg self~user, self~system, self~debug = .false
return


::method get_current
-- display the question
question = 'Are there any other FTP services installed?'
say question
-- get the answer
newarr = .array~new()
retc = self~execute('rpm -q ftp-server')
arr = self~arr_from_file(self~tfile)
if arr[1]~pos('not installed') = 0 then newarr~append(arr[1])
retc = self~execute('rpm -q perl-ftp')
arr = self~arr_from_file(self~tfile)
if arr[1]~pos('not installed') = 0 then newarr~append(arr[1])
retc = self~execute('rpm -q perl-Net-FTPServer')
arr = self~arr_from_file(self~tfile)
if arr[1]~pos('not installed') = 0 then newarr~append(arr[1])
retc = self~execute('rpm -q proftpd')
arr = self~arr_from_file(self~tfile)
if arr[1]~pos('not installed') = 0 then newarr~append(arr[1])
retc = self~execute('rpm -q pureftpd')
arr = self~arr_from_file(self~tfile)
if arr[1]~pos('not installed') = 0 then newarr~append(arr[1])
retc = self~execute('rpm -q wuftpd')
arr = self~arr_from_file(self~tfile)
if arr[1]~pos('not installed') = 0 then newarr~append(arr[1])
retc = self~execute('rpm -q wu-ftpd')
arr = self~arr_from_file(self~tfile)
if arr[1]~pos('not installed') = 0 then newarr~append(arr[1])
-- set up the report section
rptarr = .array~new()
rptarr~append('<listitem>')
rptarr~append('<indexterm><primary>ftp-server daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>perl-ftp daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>perl-Net-FTPServer daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>proftpd daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>pureftpd daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>wuftpd daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>wu-ftpd daemon</primary></indexterm>')
rptarr~append('<para>'question'</para>')
if newarr~items() = 0 then do
   rptarr~append('<programlisting>None installed</programlisting>')
   rptarr~append(self~msg7)
   end
else do
   rptarr~append('<programlisting>')
   do line over newarr
      rptarr~append(line)
      end
   rptarr~append('</programlisting>')
   rptarr~append('<note><title>Warning</title>')
   rptarr~append(self~msg6)
   rptarr~append('<para>If possible the daemon(s) listed above should be removed from the system.</para>')
   rptarr~append('</note>')
   end
rptarr~append('</listitem>')
return rptarr

                               
::requires './classes/linuxsa.cls'
             
