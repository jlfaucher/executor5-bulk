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


::class usrwheelroot public subclass linuxsa

::method init
use strict arg self~user, self~system, self~debug = .false
return


::method get_current
-- display the question
question = 'Is su access to root limited to the wheel group?'
say question
-- get the answer
retc = self~execute('cat /etc/group | grep wheel')
arr = self~arr_from_file(self~tfile)
status = 'No'
if arr~items > 0 then do
   retc = self~execute('cat /etc/pam.d/su')
   arr = self~arr_from_file(self~tfile)
   do line over arr
      if line~word(1) = 'auth' & line~word(2) = 'required' & line~word(3) = 'pam_wheel.so' &,
       line~word(4) = 'use_uid' then status = 'Yes'
      end
   end
-- set up the report section
rptarr = .array~new()
rptarr~append('<listitem>')
rptarr~append('<indexterm><primary>su access to root</primary></indexterm>')
rptarr~append('<indexterm><primary>root access via su</primary></indexterm>')
rptarr~append('<para>'question'</para>')
rptarr~append('<para>The root account can be restricted to a special group named')
rptarr~append('<computeroutput>wheel</computeroutput> such that only users that are members of the')
rptarr~append('<computeroutput>wheel</computeroutput> group can')
rptarr~append('<computeroutput>su</computeroutput> to the root account.</para>')
rptarr~append('<programlisting>'status'</programlisting>')
if status = 'Yes' then do
   rptarr~append(self~msg7)
   end
else do
   rptarr~append('<note><title>Warning</title>')
   rptarr~append(self~msg6)
   rptarr~append('<para>Either the wheel group does not exist or the file')
   rptarr~append('<computeroutput>/etc/sudoers</computeroutput> is not configured')
   rptarr~append('correctly.</para>')
   rptarr~append('</note>')
   end
rptarr~append('</listitem>')
return rptarr

                               
::requires './classes/linuxsa.cls'

