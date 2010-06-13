#!/usr/bin/rexx
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Description: This is the build script for any rpm-based external OS.       */
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


-- platform specific variables!
-- osname = 'fedora13-i386'
-- osname = 'fedora13-i386'
-- osname = 'fedora13-x86_64'
-- osname = 'opensuse11-i386'
-- osname = 'opensuse11-x86_64'
osname = 'SLES11-s390x'
-- osname = 'winxpsp3-i386'

-- Initialization
build = .build~new()
build~homedir = '/home/'userid()  -- always do first!
-- build~builddir = build~homedir'/buildorx'
build~builddir = '/data1/buildorx'  -- value for oorexx.osdl.marist.edu
build~targetdir = '/home/dashley/website/trunk/docroot/builds/interpreter-main'
build~osname = osname

-- Set our home directory
call directory build~homedir

-- Do the build
build~build_rpm()
   
-- Cleanup   
'scp' self~homedir'/BuildRPM.log' 'dashley@build.oorexx.org:/home/dashley/website/trunk/builds/status/' ||,
 date('S') || self~osname
call SysFileDelete(self~homedir'/BuildRPM.log')
return


/*----------------------------------------------------------------------------*/
/* Class: build                                                               */
/*----------------------------------------------------------------------------*/

::class build public
::method init
return

/*----------------------------------------------------------------------------*/
/* Attributes:                                                                */
/*----------------------------------------------------------------------------*/

::attribute svnrevision
::attribute homedir
::attribute targetdir
::attribute builddir      -- the temp build dir
::attribute osname

/*----------------------------------------------------------------------------*/
/* Method: log                                                                */
/*----------------------------------------------------------------------------*/

::method log
-- log messages
use strict arg msg
strm = .stream~new(self~homedir'/BuildRPM.log')
strm~open('write append')
msg = date('S') time('N') msg
say msg
strm~lineout(msg)
strm~close()
return

/*----------------------------------------------------------------------------*/
/* Method: getsvnrevision                                                     */
/*----------------------------------------------------------------------------*/

::method getsvnrevision
verlocal = './temp.ver'
'svnversion >' verlocal
strm = .stream~new(verlocal)
retc = strm~open('read')
if retc <> 'READY:' then return ''
svnver = strm~lineIn()
retc = strm~close()
if svnver = '' then svnver = 'unknown'
return svnver

/*----------------------------------------------------------------------------*/
/* Method: build_rpm                                                          */
/*----------------------------------------------------------------------------*/

::method build_rpm
self~log('Starting build.')
buildrpt = self~osname'.buildrpt.txt'
savedir = directory()
-- create temp dir and checkout the source
'rm -rf' self~builddir  -- make sure the subdir is erased
'mkdir' self~builddir
'svn co http://oorexx.svn.sourceforge.net/svnroot/oorexx/main/trunk/' self~builddir
call directory self~builddir
-- see if we have already built this revision
svnver = self~getsvnrevision()
newdir = self~targetdir'/'svnver'/'self~osname
if self~targetexists('dashley', 'build.oorexx.org', newdir) = .false then do
   -- build the rpm
   './bootstrap 2>&1 | tee -a' buildrpt
   './configure 2>&1 | tee -a' buildrpt
   'make rpm 2>&1 | tee -a' buildrpt
   -- copy the results to the host
   'ssh dashley@build.oorexx.org "mkdir -p' newdir'"'
   if SysIsFileDirectory('./rpm/RPMS/i386') then ,
    'scp ./rpm/RPMS/i386/ooRexx*.rpm dashley@build.oorexx.org:'newdir
   else if SysIsFileDirectory('./rpm/RPMS/i486') then ,
    'scp ./rpm/RPMS/i486/ooRexx*.rpm dashley@build.oorexx.org:'newdir
   else if SysIsFileDirectory('./rpm/RPMS/i586') then ,
    'scp ./rpm/RPMS/i586/ooRexx*.rpm dashley@build.oorexx.org:'newdir
   else if SysIsFileDirectory('./rpm/RPMS/i686') then ,
    'scp ./rpm/RPMS/i686/ooRexx*.rpm dashley@build.oorexx.org:'newdir
   else if SysIsFileDirectory('./rpm/RPMS/x86_64') then ,
    'scp ./rpm/RPMS/x86_64/ooRexx*.rpm dashley@build.oorexx.org:'newdir
   else if SysIsFileDirectory('./rpm/RPMS/s390x') then ,
    'scp ./rpm/RPMS/s390x/ooRexx*.rpm dashley@build.oorexx.org:'newdir
   else if SysIsFileDirectory('./rpm/RPMS/s390') then ,
    'scp ./rpm/RPMS/s390/ooRexx*.rpm dashley@build.oorexx.org:'newdir
   else nop -- it must not be a supported rpm type
   'scp' buildrpt 'dashley@build.oorexx.org:'newdir
   end
-- remove everything
call directory savedir
'rm -rf' self~builddir
self~log('Finished build.')
return

/*----------------------------------------------------------------------------*/
/* Method: targetexist                                                        */
/*----------------------------------------------------------------------------*/

::method targetexists
use strict arg userid, host, target
tempf = '/var/orxbuild.tmp'
'ssh' userid'@'host '"ls -l' target'" >' tempf
strm = .stream~new(tempf)
strm~open('read')
arr = strm~arrayin()
strm~close()
if arr~items() = 0 then return .false
if arr[1]~pos('cannot access') > 0 then return .false
return .true

