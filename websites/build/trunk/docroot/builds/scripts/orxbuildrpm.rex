#!/usr/bin/rexx
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Description: This is the build script for any rpm-based build machine.     */
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


-- Initialization
build = .build~new('./orxbuild.local.properties', arg(1)~strip())

-- Move to our home directory
call directory build~homedir

-- Do the build
build~build_rpm()
   
-- Cleanup   
'scp' build~statusfile() ,
 'dashley@build.oorexx.org:/pub/www/build/docroot/builds/status/' ||,
 build~builddate() || '-' || build~osname
build~email_result()
call SysFileDelete build~statusfile
call SysFileDelete build~lockfile
return


/*----------------------------------------------------------------------------*/
/* Class: build                                                               */
/*----------------------------------------------------------------------------*/

::class build public

/*----------------------------------------------------------------------------*/
/* Attributes:                                                                */
/*----------------------------------------------------------------------------*/

::attribute svnrevision
::attribute homedir
::attribute targetdir
::attribute builddir      -- the temp build dir
::attribute osname
::attribute builddate
::attribute statusfile
::attribute lockfile
::attribute src
::attribute location      -- the build type ie. branch4.1.0
::attribute email

/*----------------------------------------------------------------------------*/
/* Method: build_rpm                                                          */
/*----------------------------------------------------------------------------*/

::method build_rpm
do while stream(self~lockfile, 'c', 'query exists') <> ''
   'sleep 50'
   end
'touch' self~lockfile
self~log('Starting build.')
buildrpt = self~osname()'.'self~location()'.buildrpt.txt'
savedir = directory()
-- create temp dir and checkout the source
'rm -rf' self~builddir()  -- make sure the subdir is erased
'mkdir' self~builddir()
'svn co' self~src() self~builddir()
call directory self~builddir()
svnver = self~getsvnrevision()
if svnver = 'unknown' then do
   self~log('Subversion checkout failed.')
   'rm -rf' self~builddir
   'rm' self~lockfile
   return
   end
newdir = self~targetdir'/'svnver'/'self~osname
-- build the rpm
self~log('Building SVN revision' svnver'.')
'./bootstrap 2>&1 | tee -a' buildrpt
'./configure 2>&1 | tee -a' buildrpt
'make rpm 2>&1 | tee -a' buildrpt
-- copy the results to the host
'ssh dashley@build.oorexx.org "mkdir -p' newdir'"'
if SysIsFileDirectory('/home/'userid()'/rpmbuild/RPMS/i386') then ,
 'scp ~/rpmbuild/RPMS/i386/ooRexx*.rpm dashley@build.oorexx.org:'newdir
else if SysIsFileDirectory('/home/'userid()'/rpmbuild/RPMS/i486') then ,
 'scp ~/rpmbuild/RPMS/i486/ooRexx*.rpm dashley@build.oorexx.org:'newdir
else if SysIsFileDirectory('/home/'userid()'/rpmbuild/RPMS/i586') then ,
 'scp ~/rpmbuild/RPMS/i586/ooRexx*.rpm dashley@build.oorexx.org:'newdir
else if SysIsFileDirectory('/home/'userid()'/rpmbuild/RPMS/i686') then ,
 'scp ~/rpmbuild/RPMS/i686/ooRexx*.rpm dashley@build.oorexx.org:'newdir
else if SysIsFileDirectory('/home/'userid()'/rpmbuild/RPMS/x86_64') then ,
 'scp ~/rpmbuild/RPMS/x86_64/ooRexx*.rpm dashley@build.oorexx.org:'newdir
else if SysIsFileDirectory('/home/'userid()'/rpmbuild/RPMS/s390x') then ,
 'scp ~/rpmbuild/RPMS/s390x/ooRexx*.rpm dashley@build.oorexx.org:'newdir
else if SysIsFileDirectory('/home/'userid()'/rpmbuild/RPMS/s390') then ,
 'scp ~/rpmbuild/RPMS/s390/ooRexx*.rpm dashley@build.oorexx.org:'newdir
else nop -- it must not be a supported rpm type
'scp' buildrpt 'dashley@build.oorexx.org:'newdir
self~log('The build is located at http://build.oorexx.org/builds/interpreter-main/'svnver'/'self~osname)
-- remove everything
call directory savedir
self~log('Finished build.')
'rm' self~lockfile
return

/*----------------------------------------------------------------------------*/
/* Method: log                                                                */
/*----------------------------------------------------------------------------*/

::method log
-- log messages
use strict arg msg
msg = date('S') time('N') msg
say msg
strm = .stream~new(self~statusfile)
strm~open('write append')
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
/* Method: email_result                                                       */
/*----------------------------------------------------------------------------*/

::method email_result
if self~email() = ''  then return
tmpemail = .stream~new('tmpemail.txt')
tmpemail~open('write replace')
tmpemail~lineout('This email is from a service machine. DO NOT REPLY!')
tmpemail~lineout('')
statstrm = .stream~new(self~statusfile())
statstrm~open(read)
arr = statstrm~arrayin()
statstrm~close()
do line over arr
   tmpemail~lineout(line)
   end
tmpemail~lineout('')
tmpemail~lineout('This email is from a service machine. DO NOT REPLY!')
tmpemail~close()
'mailx -s "Your ooRexx Build Is Ready" -r build@build.oorexx.org' self~email() '< tmpemail.txt'
call SysFileDelete 'tmpemail.txt'
return

/*----------------------------------------------------------------------------*/
/* Method: init                                                               */
/*----------------------------------------------------------------------------*/

::method init
use strict arg propfile, cmdline
-- get the saved properties/attributes
prop = .properties~load(propfile)
self~osname = prop['osname']
self~homedir = prop['homedir']
self~builddir = prop['builddir']
self~targetdir = prop['targetdir']
self~lockfile = prop['lockfile']
-- get the unsaves attributes
self~builddate = date('S') || '-' || changestr(':', time(), '')
self~statusfile = self~homedir() || '/' || self~builddate() || '-' || self~osname
-- get the comman line options
argc = cmdline~words()
if argc > 0 then self~location = cmdline~word(1)
else self~location = 'trunk'
select
   when self~location = 'branch' then do
      self~src = prop['branchsrc']
      self~location = 'branch4.1.0'
      end
   otherwise do
      self~src = prop['trunksrc']
      end
   end
if argc > 1 then self~email = cmdline~word(2)
else self~email = ''
return

