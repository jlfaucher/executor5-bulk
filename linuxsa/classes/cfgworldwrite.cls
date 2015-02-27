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


::class cfgworldwrite public subclass linuxsa

::method init
use strict arg self~user, self~system, self~debug = .false
return


::method get_current
-- display the question
question = 'Verify all world-writable <computeroutput>/home</computeroutput> subdirectories have sticky bits set.'
say question
-- get the answer
retc = self~execute('find /home -xdev -type d \( -perm -0002 -a ! -perm -1000 \) -print')
arr = self~arr_from_file(self~tfile)
-- set up the report section
rptarr = .array~new()
rptarr~append('<listitem>')
rptarr~append('<indexterm><primary>world-writable files</primary></indexterm>')
rptarr~append('<indexterm><primary>files, world-writable</primary></indexterm>')
rptarr~append('<indexterm><primary>/home</primary></indexterm>')
rptarr~append('<para>When the so-called "sticky bit" is set on a directory, only the owner of a given file may remove that file from the')
rptarr~append('directory. Without the sticky bit, any user with write access to a directory may remove any file in the directory.')
rptarr~append('Setting the sticky bit prevents users from removing each other\u2019s files. In cases where there is no reason for a')
rptarr~append('directory to be world-writable, a better solution is to remove that permission rather than to set the sticky bit.')
rptarr~append('However, if a directory is used by a particular application, consult that application\u2019s documentation instead of')
rptarr~append('blindly changing modes.</para>')
rptarr~append('<para>'question'</para>')
if arr~items() > 0 then do
   rptarr~append('<programlisting>')
   do line over arr
      rptarr~append(line)
      end
   rptarr~append('</programlisting>')
   rptarr~append('<note><title>Warning</title>')
   rptarr~append(self~msg9p)
   rptarr~append('<para>The list of subdirectories above do not have their sticky bit set.</para>')
   rptarr~append('</note>')
   end
else do
   rptarr~append('<programlisting></programlisting>')
   rptarr~append(self~msg8)
   end
rptarr~append(self~msg14)
rptarr~append('</listitem>')
return rptarr

                               
::requires './classes/linuxsa.cls'

