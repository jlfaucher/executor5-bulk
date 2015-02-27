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


::class svcmessagebus public subclass linuxsa

::method init
use strict arg self~user, self~system, self~debug = .false
return


::method get_current
-- display the question
question = 'Is D-Bus installed/running?'
say question
-- get the answer
status = self~chkservice('messagebus')
-- set up the report section
rptarr = .array~new()
rptarr~append('<listitem>')
rptarr~append('<indexterm><primary>messagebus daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>D-Bus</primary></indexterm>')
rptarr~append('<para>D-Bus is an IPC mechanism that provides a common channel for inter-process communication.</para>')
rptarr~append('<para>A number of default services make use of D-Bus, including X Windows , Bluetooth')
rptarr~append('and Avahi. This guide recommends that D-Bus and all its dependencies be disabled unless there is')
rptarr~append('a mission-critical need for them.</para>')
rptarr~append('<para>Stricter configuration of D-Bus is possible and documented in the man page dbus daemon. D-Bus maintains')
rptarr~append('two separate configuration files, located in <computeroutput>/etc/dbus-1/</computeroutput>, one for system-specific configuration and the other for')
rptarr~append('session-specific configuration.</para>')
rptarr~append('<para>'question'</para>')
if status = 'none' then do
   rptarr~append('<programlisting>Not installed</programlisting>')
   rptarr~append(self~msg7)
   end
else if status = 'static' then do
   rptarr~append('<programlisting>Installed and running as a systemd static service</programlisting>')
   rptarr~append(self~msg7)
   end
else if status = 'enabled' then do
   rptarr~append('<programlisting>Installed and running</programlisting>')
   rptarr~append('<note><title>Warning</title>')
   rptarr~append(self~msg6)
   rptarr~append('<para>Ensure this daemon is needed and properly configured.</para>')
   rptarr~append('</note>')
   end
else if status = 'disabled' then do
   rptarr~append('<programlisting>Installed and disabled</programlisting>')
   rptarr~append('<note><title>Warning</title>')
   rptarr~append(self~msg6)
   rptarr~append('<para>Ensure this daemon is needed and properly configured.</para>')
   rptarr~append('</note>')
   end
else do
   rptarr~append('<programlisting>This daemon is in an unknown state</programlisting>')
   rptarr~append('<note><title>Warning</title>')
   rptarr~append(self~msg6)
   rptarr~append('<para>This daemon should be removed from the system.</para>')
   rptarr~append('</note>')
   end
rptarr~append('</listitem>')
return rptarr

                               
::requires './classes/linuxsa.cls'

