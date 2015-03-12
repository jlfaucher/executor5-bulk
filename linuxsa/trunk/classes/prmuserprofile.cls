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


::class prmuserprofile public subclass linuxsa

::method init
use strict arg self~user, self~system, self~debug = .false
return


::method get_current
-- display the question
question = 'Do the user''s <computeroutput>.bash_profile</computeroutput>' || ,
 ' files have the correct permissions?'
say question
-- get the answer
retc = self~execute('cat /etc/passwd')
arr = self~arr_from_file(self~tfile)
newarr = .array~new
do line over arr
   parse var line user ':' . ':' uid ':' gid ':' uname ':' homedir ':' ushell
   if uid < 1000 then iterate
   if homedir = '' then iterate
   if homedir~substr(1, 4) = '/var' then iterate
   retc = self~execute('ls -l' homedir'/.bash_profile')
   parr = self~arr_from_file(self~tfile)
   if parr~items() = 0 then iterate
   if parr[1]~substr(1, 10) <> self~bp_perm4 then do
         newarr~append(parr[1])
      end
   end
-- set up the report section
rptarr = .array~new()
rptarr~append('<listitem>')
rptarr~append('<indexterm><primary>.bash_profile permissions</primary></indexterm>')
rptarr~append('<para>'question'</para>')
if newarr~items() > 0 then do
   rptarr~append('<programlisting>')
   do line over newarr
      rptarr~append(line)
      end
   rptarr~append('</programlisting>')
   rptarr~append('<note><title>Warning</title>')
   rptarr~append(self~msg6)
   rptarr~append('<para>These files should have their permission modified to')
   rptarr~append('"'self~bp_perm4'".</para>')
   rptarr~append('</note>')
   end
else do
   rptarr~append('<programlisting>None</programlisting>')
   rptarr~append(self~msg7)
   end
rptarr~append(self~msg14)
rptarr~append('</listitem>')
return rptarr

                               
::requires './classes/linuxsa.cls'
             
