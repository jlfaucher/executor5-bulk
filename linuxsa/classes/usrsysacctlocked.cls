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


::class usrsysacctlocked public subclass linuxsa

::method init
use strict arg self~user, self~system, self~debug = .false
return


::method get_current
-- display the question
question = 'Are any system accounts not locked?'
say question
-- get the answer
newarr = .array~new()
retc = self~execute('cat /etc/passwd')
passwd = self~arr_from_file(self~tfile)
retc = self~execute('cat /etc/shadow')
shadow = self~arr_from_file(self~tfile)
do pline over passwd
   parse var pline user ':' . ':' uid ':' gid ':' uname ':' homedir ':' ushell
   if uid = 0 | uid >= 500 then iterate
   if ushell <> '/sbin/nologin' then do
      newarr~append(user)
      iterate
      end
   else do sline over shadow
      parse var sline suser ':' pw ':' .
      if user = suser & pw~substr(1, 1) <> '!' then do
         newarr~append(user)
         leave
         end
      end
   end
-- set up the report section
rptarr = .array~new()
rptarr~append('<listitem>')
rptarr~append('<indexterm><primary>accounts, locked</primary></indexterm>')
rptarr~append('<indexterm><primary>user accounts, locked</primary></indexterm>')
rptarr~append('<para>'question'</para>')
if newarr~items > 0 then do
   rptarr~append('<programlisting>')
   do line over newarr
      rptarr~append(line)
      end
   rptarr~append('</programlisting>')
   rptarr~append('<note><title>Warning</title>')
   rptarr~append(self~msg10)
   rptarr~append('<para>The above list of system accounts are not properly locked.</para>')
   rptarr~append('</note>')
   end
else do
   rptarr~append('<programlisting>None found</programlisting>')
   rptarr~append(self~msg8)
   end
rptarr~append('</listitem>')
return rptarr

                               
::requires './classes/linuxsa.cls'

