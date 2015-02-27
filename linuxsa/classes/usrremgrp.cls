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


::class usrremgrp public subclass linuxsa

::attribute grplist


::method init
expose grplist
use strict arg self~user, self~system, self~debug = .false
grplist = .set~new()
grplist~put('ftp')
grplist~put('news')
grplist~put('uucp')
grplist~put('gopher')
grplist~put('dip')
grplist~put('nscd')
grplist~put('gopher')
grplist~put('dip')
grplist~put('games')
grplist~put('rpc')
grplist~put('rpcuser')
return


::method get_current
expose grplist
-- display the question
question = 'List candidate user groups that should be considered for removal.'
say question
-- get the answer
retc = self~execute('cat /etc/group')
arr = self~arr_from_file(self~tfile)
newarr = .array~new()
do line over arr
   parse var line gname ':' .
   if grplist~hasindex(gname) then newarr~append(gname)
   end
-- set up the report section
rptarr = .array~new()
rptarr~append('<listitem>')
rptarr~append('<indexterm><primary>remove candidate groups</primary></indexterm>')
rptarr~append('<para>'question'</para>')
if newarr~items = 0 then do
   rptarr~append('<programlisting>None found</programlisting>')
   rptarr~append(self~msg7)
   end
else do
   rptarr~append('<programlisting>')
   do gname over newarr
      rptarr~append(gname)
      end
   rptarr~append('</programlisting>')
   rptarr~append('<note><title>Warning</title>')
   rptarr~append(self~msg6)
   rptarr~append('<para>The list of groups listed above should be reviewed')
   rptarr~append('and considered for removal from the system.</para>')
   rptarr~append('</note>')
   end
rptarr~append('</listitem>')
return rptarr

                               
::requires './classes/linuxsa.cls'

