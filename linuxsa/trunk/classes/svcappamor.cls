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


::class svcappamor public subclass linuxsa

::method init
use strict arg self~user, self~system, self~debug = .false
return


::method get_current
-- display the question
question = 'Is the AppAmor daemon installed/active?'
say question
-- get the answer
status = self~rpmdpkg('libapparmor1')
-- set up the report section
rptarr = .array~new()
rptarr~append('<listitem>')
rptarr~append('<indexterm><primary>AppArmor</primary></indexterm>')
rptarr~append('<indexterm><primary>libimmunix</primary></indexterm>')
rptarr~append('<para>AppArmor is an effective and easy-to-use Linux application security system')
rptarr~append('that protects your Linux operating system and applications from the effects of attacks, viruses,')
rptarr~append(']and malicious applications. AppArmor is not a firewall or a virus detection application--it is a')
rptarr~append('complete intrusion prevention system.</para>')
rptarr~append('<para>'question'</para>')
rptarr~append('<programlisting>'status'</programlisting>')
if status = 'Not installed' then do
   rptarr~append('<note><title>Warning</title>')
   rptarr~append('<para>AppAmoris not installed. This does not meet best practice requirements.')
   rptarr~append('This does not apply on Red Hat based systems where it is not installed by default.')
   rptarr~append('</para>')
   rptarr~append('</note>')
   end
else do
   rptarr~append(self~msg8)
   end
rptarr~append('</listitem>')

return rptarr

                               
::requires './classes/linuxsa.cls'
             
