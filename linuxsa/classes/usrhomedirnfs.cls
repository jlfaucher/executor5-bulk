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


::class usrhomedirnfs public subclass linuxsa

::method init
use strict arg self~user, self~system, self~debug = .false
return


::method get_current
-- display the question
question = 'Is NFS used for user <computeroutput>/home</computeroutput> subdirectory storage?'
say question
-- get the answer
retc = self~execute('cd ~;df .')
arr = self~arr_from_file(self~tfile)
status = 'No'
temp = arr[2]
parse var temp home .
if home~pos(':') > 0 then status = 'Yes'
-- set up the report section
rptarr = .array~new()
rptarr~append('<listitem>')
rptarr~append('<indexterm><primary>/home</primary></indexterm>')
rptarr~append('<indexterm><primary>NFS home subdirectories</primary></indexterm>')
rptarr~append('<para>Once the user management is centralized to an LDAP-based directory, one user')
rptarr~append('related element will still remain decentralized. This is the users'' home')
rptarr~append('directories. Currently, every user has a completely separate home directory for')
rptarr~append('every system he accesses. Decentralized local directories can coexist perfectly')
rptarr~append('with LDAP-based user management, but this will lessen the elegance and simplicity')
rptarr~append('that the LDAP-based model provides.')
rptarr~append('</para>')
rptarr~append('<para>For example, in the decentralized home directories/LDAP-based user management')
rptarr~append('model, when a user is removed from the LDAP directory, AIX administration will')
rptarr~append('have to remove each home directory for each system that the user had access to.')
rptarr~append('Additionally, when a new system is installed, a home directory will need to be')
rptarr~append('created for each user that will access the system.')
rptarr~append('</para>')
rptarr~append('<para>A very powerful and elegant solution to this decentralized home directory')
rptarr~append('problem is to provide centralized storage via NFS. The NFS solution will allow the')
rptarr~append('same NFS server based home directory to be used for all the systems a user logs on')
rptarr~append('to. When a user is added or removed from the AIX environment, only one home')
rptarr~append('directory will need to be added or removed for the user. In contrast, decentralized')
rptarr~append('storage requires the creation and deletion of a home directory for every system')
rptarr~append('the user had access to. When LDAP-based user management and NFS based home')
rptarr~append('directories provide complete user related centralization, a new system can be added')
rptarr~append('to the network and be completely user provisioned with home directory access within')
rptarr~append('minutes after the operating system has been installed on the AIX partition.</para>')
rptarr~append('<para>'question'</para>')
rptarr~append('<programlisting>'status'</programlisting>')
if status = 'Yes' then do
   rptarr~append('<note><title>Warning</title>')
   rptarr~append(self~msg10)
   rptarr~append('<para>You should investigate using NFS for usr home directories.</para>')
   rptarr~append('</note>')
   end
else do
   rptarr~append(self~msg8)
   end
rptarr~append('</listitem>')
return rptarr

                               
::requires './classes/linuxsa.cls'
             
