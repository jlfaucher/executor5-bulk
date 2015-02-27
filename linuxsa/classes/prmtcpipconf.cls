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


::class prmtcpipconf public subclass linuxsa

::attribute vlist private

::method init
expose vlist
use strict arg self~user, self~system, self~debug = .false
vlist = .array~new()
vlist~append('/etc/aliases')
vlist~append('/etc/host.conf')
vlist~append('/etc/hosts')
vlist~append('/etc/netconfig')
vlist~append('/etc/networks')
vlist~append('/etc/ntp.conf')
vlist~append('/etc/protocols')
vlist~append('/etc/resolv.conf')
vlist~append('/etc/services')
vlist~append('/etc/xinetd.conf')
return


::method get_current_perm private
use strict arg bin
-- get the answer
retc = self~execute('ls -l' bin)
arr = self~arr_from_file(self~tfile)
if arr~items > 0 then return arr[1]
return ''


::method get_current
-- display the question
question = 'Verify TCP/IP config file permissions.'
say question
-- set up the report section
rptarr = .array~new()
rptarr~append('<listitem>')
rptarr~append('<indexterm><primary>/etc/aliases permissions</primary></indexterm>')
rptarr~append('<indexterm><primary>/etc/host.conf permissions</primary></indexterm>')
rptarr~append('<indexterm><primary>/etc/hosts permissions</primary></indexterm>')
rptarr~append('<indexterm><primary>/etc/netconfig permissions</primary></indexterm>')
rptarr~append('<indexterm><primary>/etc/networks permissions</primary></indexterm>')
rptarr~append('<indexterm><primary>/etc/ntp.conf permissions</primary></indexterm>')
rptarr~append('<indexterm><primary>/etc/protocols permissions</primary></indexterm>')
rptarr~append('<indexterm><primary>/etc/resolv.conf permissions</primary></indexterm>')
rptarr~append('<indexterm><primary>/etc/services permissions</primary></indexterm>')
rptarr~append('<indexterm><primary>/etc/xinetd.conf permissions</primary></indexterm>')
rptarr~append('<para>'question'</para>')
rptarr~append('<orderedlist>')
do bin over self~vlist
   rptarr~append('<listitem>')
   rptarr~append('<para>Verify permissions and ownership for the')
   rptarr~append('<computeroutput>' || bin || '</computeroutput> config file.')
   rptarr~append('The permissions for this file should be "'self~bp_perm4'" and the')
   rptarr~append('owner and group should be set to root.')
   rptarr~append('</para>')
   item = self~get_current_perm(bin) -- get the answer
   if item = '' then do
      rptarr~append('<programlisting>File' bin 'not found.</programlisting>')
      rptarr~append('<note><title>Warning</title>'self~msg1)
      rptarr~append('</note>')
      end
   else do
      rptarr~append('<programlisting>' || item || '</programlisting>')
      flag = .true
      if item~word(1)~substr(1, 10) <> self~bp_perm4 then do
         rptarr~append('<note><title>Warning</title>'self~msg3)
         rptarr~append('</note>')
         flag = .false
         end
      if item~word(3) <> self~bp_owner then do
         rptarr~append('<note><title>Warning</title>'self~msg4)
         rptarr~append('</note>')
         flag = .false
         end
      if item~word(4) <> self~bp_grp then do
         rptarr~append('<note><title>Warning</title>'self~msg5)
         rptarr~append('</note>')
         flag = .false
         end
      if flag then do
         rptarr~append(self~msg2)
         end
      end
   rptarr~append('</listitem>')
   end
rptarr~append('</orderedlist>')
rptarr~append('</listitem>')
return rptarr

                               
::requires './classes/linuxsa.cls'
             
