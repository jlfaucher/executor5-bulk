#!/usr/bin/rexx
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Description: This is the build daemon for any deb-based build machine.     */
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
-- osname = 'fedora13-x86_64'
osname = 'ubuntu1004-i386'
-- osname = 'ubuntu1004-x86_64'
-- osname = 'opensuse11-i386'
-- osname = 'opensuse11-x86_64'
-- osname = 'SLES10-s390x'
-- osname = 'winxpsp3-i386'

-- Initialization
build = .build~new()
build~homedir = '/home/'userid()  -- always do first!
build~builddir = build~homedir'/buildorx'
build~targetdir = '/pub/www/build/docroot/builds/interpreter-main'
build~osname = osname
build~builddate = date('S') || '-' || changestr(':', time(), '')
build~statusfile = build~homedir() || '/' || build~builddate() || '-' || build~osname
build~lockfile = '/tmp/ooRexxBuild.lock'

-- get the command line arguments
call parse_cmd_line arg(1)~strip(), build

-- Set our home directory
call directory build~homedir

-- Do the build
build~build_deb()

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
::attribute builddate
::attribute statusfile
::attribute lockfile
::attribute src
::attribute email

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
buildrpt = self~osname'.buildrpt.txt'
-- create temp dir and checkout the source
'mkdir' self~builddir()
call directory self~builddir()
'svn co' self~src() self~builddir
svnver = self~getsvnrevision()
if \datatype(svnver, 'W') then do
   self~log('Subversion checkout failed.')
   return
   end
newdir = self~targetdir'/'svnver'/'self~osname
if self~targetexists('dashley', 'build.oorexx.org', newdir) = .false then do
   -- build the deb
   self~log('Building SVN revision' svnver'.')
   './bootstrap 2>&1 | tee -a' buildrpt
   './configure --disable-static 2>&1 | tee -a' buildrpt
   'make deb 2>&1 | tee -a' buildrpt
   -- copy the results to the host
   'ssh dashley@build.oorexx.org "mkdir -p' newdir'"'
   'scp ../oorexx*.deb dashley@build.oorexx.org:'newdir
   'scp' buildrpt 'dashley@build.oorexx.org:'newdir
   self~log('The build is located at http://build.oorexx.org/builds/interpreter-main/'svnver'/'self~osname)
   end
else self~log('This was a duplicate build request for SVN revision' svnver'.')
-- remove everything
call directory savedir
'rm -rf' self~builddir()
'rm oorexx*.deb'
self~log('Finished build.')
'rm' self~lockfile
return

/*----------------------------------------------------------------------------*/
/* Method: targetexist                                                        */
/*----------------------------------------------------------------------------*/

::method targetexists
use strict arg userid, host, target
tempf = '/tmp/orxbuild.tmp'
'ssh' userid'@'host '"ls -l' target'" >' tempf
strm = .stream~new(tempf)
strm~open('read')
arr = strm~arrayin()
strm~close()
if arr~items() = 0 then return .false
if arr[1]~pos('cannot access') > 0 then return .false
return .true

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
return

/*----------------------------------------------------------------------------*/
/* Routine: parse_cmd_line                                                    */
/*----------------------------------------------------------------------------*/

::routine parse_cmd_line
use strict arg cmdline, build
argc = cmdline~words()
if argc > 0 then build~src = cmdline~word(1)
else build~src = 'trunk'
select
   when build~src = 'branch' then build~src = 'http://oorexx.svn.sourceforge.net/svnroot/oorexx/main/branches/4.1.0/trunk/'
   otherwise build~src = 'http://oorexx.svn.sourceforge.net/svnroot/oorexx/main/trunk/'
   end
if argc > 1 then build~email = cmdline~word(2)
else build~email = ''
-- just ignore everything else on the cmdline
return

