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


::class svcldap public subclass linuxsa

::method init
use strict arg self~user, self~system, self~debug = .false
return


::method get_current
-- display the question
question = 'Is the LDAP daemon installed/running?'
say question
-- get the answer
status = self~chkservice('ldap')
-- set up the report section
rptarr = .array~new()
rptarr~append('<listitem>')
rptarr~append('<indexterm><primary>ldap daemon</primary></indexterm>')
rptarr~append('<para>As the number of systems that any user has access to increases, the number of')
rptarr~append('user IDs and passwords that the user must keep track of also increases. As this')
rptarr~append('number increases, the ability to remember and maintain this user information')
rptarr~append('becomes more difficult. The user may try to maintain the same password on all')
rptarr~append('systems, but when forced to change these on a regular basis, the task of making the')
rptarr~append('changes can consume a considerable amount of time and organization. One potential')
rptarr~append('solution to this problem is to create a central repository for this information to')
rptarr~append('keep track of all the systems the user has access to. The LDAP protocol provides')
rptarr~append('the best way to centralize user accounts.</para>')
rptarr~append('<para>'question'</para>')
if status = 'none' then do
   rptarr~append('<programlisting>Not installed</programlisting>')
   rptarr~append('<para>It is highly recommended that LDAP be installed and configured')
   rptarr~append('to provide better user authentication</para>')
   end
else if status = 'enabled' then do
   rptarr~append('<programlisting>Installed and running</programlisting>')
   rptarr~append('<note><title>Warning</title>')
   rptarr~append(self~msg7)
   rptarr~append('<para>Ensure this daemon is needed and properly configured.</para>')
   rptarr~append('</note>')
   end
else do
   rptarr~append('<programlisting>Installed but not running</programlisting>')
   rptarr~append('<note><title>Warning</title>')
   rptarr~append(self~msg6)
   rptarr~append('<para>This daemon should either be removed from the system or')
   rptarr~append('enabled and configured to provide better user authentication</para>')
   rptarr~append('</note>')
   end
rptarr~append('</listitem>')
return rptarr

                               
::requires './classes/linuxsa.cls'
             
