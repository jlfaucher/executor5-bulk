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


::class cfgusb public subclass linuxsa

::method init
use strict arg self~user, self~system, self~debug = .false
return


::method get_current
-- display the question
question = 'Are USB devices supported?'
say question
-- get the answer
retc = self~execute('rpm -q libusb')
arr = self~arr_from_file(self~tfile)
if arr[1]~pos('not installed') > 0 then status = 'No'
status = 'Yes'
-- set up the report section
rptarr = .array~new()
rptarr~append('<listitem>')
rptarr~append('<indexterm><primary>USB devices</primary></indexterm>')
rptarr~append('<indexterm><primary>libusb.rpm</primary></indexterm>')
rptarr~append('<para>Though unusual, some systems may be managed only remotely and yet also exposed to risk from attackers with')
rptarr~append('direct physical access to them. In these cases, reduce an attacker\u2019s access to the system by disabling unnecessary')
rptarr~append('external ports (e.g. USB, FireWire, NIC) in the system\u2019s BIOS.</para>')
rptarr~append('<para>Disabling USB ports is particularly unusual and will cause problems for important input devices')
rptarr~append('such as keyboards or mice attached to the system.</para>')
rptarr~append('<para>'question'</para>')
rptarr~append('<programlisting>'status'</programlisting>')
if status = 'Yes' then do
   rptarr~append('<note><title>Warning</title>')
   rptarr~append(self~msg6)
   rptarr~append('<para>It may not be possible to remove USB support because it may be needed')
   rptarr~append('to support keyboard and/or mouse devices.</para>')
   rptarr~append('</note>')
   end
else do
   rptarr~append(self~msg7)
   end
rptarr~append('</listitem>')
return rptarr

                               
::requires './classes/linuxsa.cls'

