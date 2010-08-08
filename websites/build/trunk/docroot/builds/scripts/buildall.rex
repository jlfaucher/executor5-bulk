#!/usr/bin/rexx
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Description: Do an ooRexx build on a virtual machine.                      */
/*                                                                            */
/* Copyright (c) 2010-2010 Rexx Language Association. All rights reserved.    */
/*                                                                            */
/* This program and the accompanying materials are made available under       */
/* the terms of the Common Public License v1.0 which accompanies this         */
/* distribution. A copy is also available at the following address:           */
/* http://www.ibm.com/developerworks/oss/CPLv1.0.htm                          */
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
/* Author: W. David Ashley                                                    */
/*                                                                            */
/*----------------------------------------------------------------------------*/


-- build the list of builddomains with their attributes
buildarr = .array~new()
buildarr~append(.builddomain~new('fedora13-i386',   '/home/'userid()'/fedora13-i386'))
buildarr~append(.builddomain~new('fedora13-x86_64', '/home/'userid()'/fedora13-x86_64'))
buildarr~append(.builddomain~new('ubuntu1004-i386', '/home/'userid()'/ubuntu1004-i386'))

buildarr~append(.builddomain~new('sles10-s390x',    '/home/'userid()'/sles10-s390x', .false))

-- do the docs build first
kvmmach = .kvmdomain~new(sys~osname())
if kvmmach~status() = 'shutoff' then kvnmach~startup()
do until kvmmach~status = 'running'
   call SysSleep 10
   end
-- get the ip address and perform the build
arr = file_arrayin(sys~addrfile)
ipaddr = arr[1]~strip()
'ssh dashley@'ipaddr '"/home/dashley/orxbuild.rex"'
kvmmach~shutdown()

-- do all the virtual builds
do sys over buildarr
   if sys~virt() = .false then iterate
   -- make sure the domain is running
   kvmmach = .kvmdomain~new(sys~osname())
   if kvmmach~status() = 'shutoff' then kvnmach~startup()
   do until kvmmach~status = 'running'
      call SysSleep 10
      end
   -- get the ip address and perform the build
   arr = file_arrayin(sys~addrfile)
   ipaddr = arr[1]~strip()
   'ssh dashley@'ipaddr '"/home/dashley/orxbuild.rex"'
   kvmmach~shutdown()
   end

-- do all the real builds
do sys over buildarr
   if sys~virt() = .true then iterate
   -- get the ip address and perform the build
   arr = file_arrayin(sys~addrfile)
   ipaddr = arr[1]~strip()
   'ssh dashley@'ipaddr '"/home/dashley/orxbuild.rex"'
   end

-- send an email that the builds are done
mime1 = .mimepart~new
mime1~addContent('The nightly builds of ooRexx are complete at' date('s') time()'.')
msg = .smtpmsg~new
msg~From = 'donotreply@build.oorexx.org'
msg~addRecipient('david.ashley.ibm@gmail.com')
msg~Subject = 'Nightly builds of ooRexx are complete'
msg~Content = mime1
smtpconx = .smtp~new
retc = smtpconx~connect('127.0.0.1', 'dashley', 'wda123aa')
if retc = -1 then do
   do rsp over smtpconx~cmdresponse
      say rsp
      end
   return
   end
retc = smtpconx~send(msg)
if retc = -1 then do
   do rsp over smtpconx~cmdresponse
      say rsp
      end
   return
   end
retc = smtpconx~logoff
return


::requires 'smtpmail.cls'
::requires 'kvmdomain.cls'


/*----------------------------------------------------------------------------*/
/* Class: builddomain                                                         */
/*----------------------------------------------------------------------------*/

::class builddomain public

::attribute osname
::attribute addr
::attribute addrfile
::attribute virt

::method init
expose osname addrfile
use strict arg osname, addrfile, virt = .true
return

::method get_ipaddr
arr = file_arrayin(self~addrfile())
ln = arr[2]
parse var ln . 'inet addr:' ipaddr .
return ipaddr


/*----------------------------------------------------------------------------*/
/* Routine: file_arrayin                                                      */
/*----------------------------------------------------------------------------*/

::routine file_arrayin
use strict arg file
strm = .stream~new(file)
strm~open('read')
arr = strm~arrayin()
strm~close()
return arr

