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


::class usrbadlogins public subclass linuxsa

::method init
use strict arg self~user, self~system, self~debug = .false
return


::method get_current
-- display the question
question = 'The following non-existent users had unsuccessful logins.'
say question
-- get the answer
retc = self~execute('cat /etc/passwd')
arr = self~arr_from_file(self~tfile)
users = .set~new
do line over arr
   parse var line user ':' . ':' uid ':' gid ':' uname ':' homedir ':' ushell
   if uid < 500 then iterate
   users~put(user, user)
   end
retc = self~execute('lastb')
arr = self~arr_from_file(self~tfile)
badusers = .set~new
do i = 1 to arr~items()
   if users~hasindex(arr[i]~word(1)) = 0 then do
      badusers~put(arr[i]~word(1), arr[i]~word(1))
      end
   end
baduserarr = badusers~makearray()
if baduserarr~items() = 0 then do
   retarr = baduserarr
   end
else do
   maxl = 0
   do user over baduserarr
      maxl = max(maxl, user~length())
      end
   items = format(60 / (maxl + 1),, 0)
   retarr = .array~new()
   do i = 1 to baduserarr~items() by items
      arrline = ''
      do j = 0 to items - 1
         if baduserarr[i + j] = '' then iterate
         if baduserarr[i + j] <> .nil then arrline = arrline left(baduserarr[i + j], maxl)
         end
      retarr~append(arrline)
      end
   end
-- set up the report section
rptarr = .array~new()
rptarr~append('<listitem>')
rptarr~append('<indexterm><primary>/etc/passwd</primary></indexterm>')
rptarr~append('<indexterm><primary>bad logins</primary></indexterm>')
rptarr~append('<para>'question'</para>')
rptarr~append('<programlisting>')
if retarr~items() > 0 then do line over retarr
   rptarr~append(line)
   end
else rptarr~append('None found')
rptarr~append('</programlisting>')
if retarr~items() > 0 then rptarr~append('<para>The user(s) listed above should be investigated.</para>')
rptarr~append(self~msg14)
rptarr~append('</listitem>')
return rptarr

                               
::requires './classes/linuxsa.cls'
             
