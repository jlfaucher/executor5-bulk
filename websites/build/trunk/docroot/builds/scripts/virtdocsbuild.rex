#!/usr/bin/rexx
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Description: This is the build script for the docs KVM guest.              */
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
osname = 'fedora13-docs'
-- osname = 'fedora13-x86_64'
-- osname = 'opensuse11-i386'
-- osname = 'opensuse11-x86_64'
-- osname = 'SLES11-s390x'
-- osname = 'winxpsp3-i386'

-- Initialization
build = .build~new()
build~homedir = '/home/'userid()  -- always do first!
build~builddir = build~homedir'/buildorx'
build~targetdir = '/home/dashley/website/trunk/docroot/builds/docs'
build~osname = osname
build~builddate = date('S')
build~statusfile = build~homedir() || '/' || build~builddate() || '-' || build~osname

-- Set our home directory
call directory build~homedir

-- Do the build
build~build_docs()
   
-- Cleanup   
'scp' build~statusfile() ,
 'dashley@build.oorexx.org:/home/dashley/website/trunk/docroot/builds/status/' ||,
 build~builddate() || '-' || build~osname
call SysFileDelete build~statusfile
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

/*----------------------------------------------------------------------------*/
/* build_docs                                                                 */
/*----------------------------------------------------------------------------*/

::method build_docs
use strict arg
self~log('Starting build.')
buildrpt = './docs-buildrpt.txt'
savedir = directory()
-- create temp dir and checkout the source
tempdir = './builddocs'
'rm -rf' tempdir   -- make sure the subdir is erased
'mkdir' tempdir
'svn co http://oorexx.svn.sourceforge.net/svnroot/oorexx/docs/trunk' tempdir
call directory tempdir
-- see if we have already built this revision
svnver = self~getsvnrevision()
if \datatype(svnver, 'W') then do
   self~log('Subversion checkout failed.')
   return
   end
newdir = self~targetdir'/'svnver
if \sysisfiledirectory(newdir) then do
   -- build the docs
   self~log('Building SVN revision' svnver'.')
   'make all 2>&1 | tee -a' buildrpt
   -- copy the results to the host
   'ssh dashley@build.oorexx.org "mkdir' newdir'"'
   'scp ./oodialog/oodialog.pdf dashley@build.oorexx.org:'newdir
   'scp ./oodialog/oodialog-html.zip dashley@build.oorexx.org:'newdir
   'scp ./readme/readme.pdf dashley@build.oorexx.org:'newdir
   'scp ./readme/readme.html dashley@build.oorexx.org:'newdir
   'scp ./rexxpg/rexxpg.pdf dashley@build.oorexx.org:'newdir
   'scp ./rexxpg/rexxpg-html.zip dashley@build.oorexx.org:'newdir
   'scp ./rexxref/rexxref.pdf dashley@build.oorexx.org:'newdir
   'scp ./rexxref/rexxref-html.zip dashley@build.oorexx.org:'newdir
   'scp ./rxftp/rxftp.pdf dashley@build.oorexx.org:'newdir
   'scp ./rxftp/rxftp-html.zip dashley@build.oorexx.org:'newdir
   'scp ./rxmath/rxmath.pdf dashley@build.oorexx.org:'newdir
   'scp ./rxmath/rxmath-html.zip dashley@build.oorexx.org:'newdir
   'scp ./rxsock/rxsock.pdf dashley@build.oorexx.org:'newdir
   'scp ./rxsock/rxsock-html.zip dashley@build.oorexx.org:'newdir
   'scp ./rexxextensions/rexxextensions.pdf dashley@build.oorexx.org:'newdir
   'scp ./rexxextensions/rexxextensions-html.zip dashley@build.oorexx.org:'newdir
   'scp ./winextensions/winextensions.pdf dashley@build.oorexx.org:'newdir
   'scp ./winextensions/winextensions-html.zip dashley@build.oorexx.org:'newdir
   'scp' buildrpt 'dashley@build.oorexx.org:'newdir
   end
else self~log('This was a duplicate build request.')
-- remove everything
call directory savedir
'rm -rf' tempdir
self~log('Finished build.')
-- shutdown the system
-- 'sudo shutdown -h now'
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
/* log                                                                        */
/*----------------------------------------------------------------------------*/

::method log
-- log messages
use strict arg msg
use strict arg msg
msg = date('S') time('N') msg
say msg
strm = .stream~new(self~statusfile)
strm~open('write append')
strm~lineout(msg)
strm~close()
return

