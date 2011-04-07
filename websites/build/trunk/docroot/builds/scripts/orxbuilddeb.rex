#!/usr/bin/rexx
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Description: This is the build daemon for any deb-based build machine.     */
/*                                                                            */
/* Copyright (c) 2010-2011 Rexx Language Association. All rights reserved.    */
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


-- This script should be invoked as follows:
--
-- orxbuilddeb.rex src target [email]
--
-- where
--
-- src    - the SVN source path
-- target - the target subdirectory path on the build server for build files 
-- email  - (optional) The email address for the build report


-- Initialization
build= .build~new('/home/'userid()'/orxbuild.local.properties', arg(1)~strip())

-- Set our home directory
call directory build~homedir

-- Do the build
build~build_deb()

-- Cleanup
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
::attribute location      -- the build type ie. branch | trunk
::attribute email
::attribute targetsys

/*----------------------------------------------------------------------------*/
/* build_deb                                                                  */
/*----------------------------------------------------------------------------*/

::method build_deb
use strict arg
do while stream(self~lockfile, 'c', 'query exists') <> ''
   'sleep 50'
   end
'touch' self~lockfile
self~log('Starting build.')
savedir = directory()
buildrpt = self~osname()'.'self~location()'.buildrpt.txt'
-- create temp dir and checkout the source
'rm -rf' self~builddir()  -- make sure the subdir is erased
'mkdir' self~builddir()
'svn co' self~src() self~builddir()
call directory self~builddir()
svnver = self~getsvnrevision()
if svnver = 'unknown' then do
   self~log('Subversion checkout failed.')
   'rm -rf' self~builddir()
   'rm' self~lockfile
   return
   end
newdir = self~targetdir'/'svnver'/'self~osname
-- build the deb
self~log('Building SVN revision' svnver'.')
'./bootstrap 2>&1 | tee -a' buildrpt
'./configure --disable-static 2>&1 | tee -a' buildrpt
'make deb 2>&1 | tee -a' buildrpt
-- copy the results to the host
'ssh' self~targetsys() '"mkdir -p' newdir'"'
'scp ../oorexx*.deb' self~targetsys() ':'newdir
'scp' buildrpt self~targetsys()':'newdir
self~log('The build is located at http://build.oorexx.org/builds/interpreter-main/'svnver'/'self~osname)
-- remove everything
call directory savedir
'rm -rf' self~builddir()
'rm oorexx*.deb'
self~log('Finished build.')
return

/*----------------------------------------------------------------------------*/
/* Method: log                                                                */
/*----------------------------------------------------------------------------*/

::method log
-- log messages
use strict arg msg
strm = .stream~new(self~statusfile())
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
'sleep 120'  -- give time for the email to be sent
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
self~lockfile = prop['lockfile']
self~targetsys = prop['targetsys']
-- get the unsaved attributes
self~builddate = date('S') || '-' || changestr(':', time(), '')
self~statusfile = self~homedir() || '/' || self~builddate() || '-' || self~osname
-- get the command line options
parse var cmdline src target email .
self~src = src
self~targetdir = target
self~email = email
-- determine if this is a trunk or branch build
if target~pos('branches') > 0 then self~location = 'branch'
else self~location = 'trunk'
return

