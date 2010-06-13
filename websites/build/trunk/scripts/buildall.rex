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


call directory '/home/dashley'

call './virtbuild.rex' 'fedora13.i386.orxbuild.1'
call './virtbuild.rex' 'fedora13.x86_64.orxbuild.1'
call './virtbuild.rex' 'fedora13.i386.orxbuild.2'      -- docs build
call './virtbuild.rex' 'opensuse11.i386.orxbuild.1'
call './virtbuild.rex' 'opensuse11.x86_64.orxbuild.1'
call './virtbuild.rex' 'ubuntu56.i386.orxbuild.1'
call './virtbuild.rex' 'winxpsp3.i386.orxbuild.1'
call './realbuild.rex' '?.?.?.?'                       -- opensuse zlinux build

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

