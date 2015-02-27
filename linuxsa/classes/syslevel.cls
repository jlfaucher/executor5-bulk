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


::class syslevel public subclass linuxsa

::method init
use strict arg self~user, self~system, self~debug = .false
return


::method sysid private
use strict arg
retc = self~execute('cat /etc/os-release')
arr = self~arr_from_file(self~tfile)
do line over arr
   if line~pos('PRETTY_NAME') > 0 then do
      parse var line 'PRETTY_NAME="' pname '"' .
      return pname
      end
   end
return 'Unknown'


::method get_current
use strict arg
question = 'Get the os and hardware info ('self~objectname~word(2)').'
say question
rptarr = .array~new()
rptarr~append('<listitem>')
rptarr~append('<indexterm><primary>System information</primary></indexterm>')
rptarr~append('<indexterm><primary>System host name</primary></indexterm>')
rptarr~append('<indexterm><primary>System memory</primary></indexterm>')
rptarr~append('<indexterm><primary>System partitions</primary></indexterm>')
rptarr~append('<indexterm><primary>System distribution</primary></indexterm>')
rptarr~append('<indexterm><primary>System ipaddress</primary></indexterm>')
rptarr~append('<para>'question'</para>')
rptarr~append('<programlisting>')
if self~system = '' then do
   rptarr~append('127.0.0.1') -- ipaddress/hostname
   end
else do
   rptarr~append(self~system) -- ipaddress/hostname
   end
retc = self~execute('echo $HOSTNAME')
arr = self~arr_from_file(self~tfile)
rptarr~append(arr[1]) -- internal hostname
sysdesc = self~sysid()
select
   when sysdesc~caselessPos('fedora') > 0 then .local~sysdist = 'RH'
   when sysdesc~caselessPos('red hat') > 0 then .local~sysdist = 'RH'
   when sysdesc~caselessPos('centos') > 0 then .local~sysdist = 'RH'
   when sysdesc~caselessPos('suse') > 0 then .local~sysdist = 'SU'
   when sysdesc~caselessPos('debian') > 0 | ,
    sysdesc~caselessPos('ubuntu') > 0 | ,
    sysdesc~caselessPos('mint') > 0 then do
      say
      say 'Error: Debian based systems are not supported by LinuxSA.'
      exit 8
      end
   otherwise .local~sysdist = 'Unknown'
   end
rptarr~append(sysdesc) -- release description
retc = self~execute('uname -s -n -r -m -p -i -o')
arr = self~arr_from_file(self~tfile)
rptarr~append(arr[1]) -- uname information
retc = self~execute('cat /proc/meminfo')
arr = self~arr_from_file(self~tfile)
rptarr~append(arr[1]) -- total memory information
retc = self~execute('cat /proc/partitions')
arr = self~arr_from_file(self~tfile)
do line over arr
   rptarr~append(line) -- total disk partitions information
   end
rptarr~append('</programlisting>')
rptarr~append('</listitem>')
return rptarr

                               
::requires './classes/linuxsa.cls'
             
