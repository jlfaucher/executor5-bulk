#!/usr/bin/rexx
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Description: This is the build script for ooRexx automated *nix builds.    */
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


-- these are some valid os names
-- fedora13-i386
-- fedora13-x86_64
-- redhat54-i386
-- redhat54-x86_64
-- opensuse11-i386
-- opensuse11-x86_64
-- SLES10-s390x

-- initialization
-- Note: THE ORDER HERE IS VERY IMPORTANT!!!
build = .build~new()
build~acct = userid()
build~homedir = '/home/' || build~acct()
build~osnamefile = build~homedir()'/osname.txt'
build~osname = build~get_osname(arg(1))
if build~osname()~pos('fedora') > 0 then build~type = 'rpm'
else if build~osname()~pos('opensuse') > 0 then build~type = 'rpm'
else if build~osname()~pos('SLES') > 0 then build~type = 'rpm'
else if build~osname()~pos('ubuntu') > 0 then build~type = 'deb'
else if build~osname()~pos('debian') > 0 then build~type = 'deb'
build~date = date('S')
build~localdir = build~homedir()'/buildorx'
build~targetacct = 'dashley'
build~targethost = 'build.oorexx.org'
build~targetdir = '/home/' || build~targetacct()'/website/docroot/builds/interpreter-main'
build~logdir = '/home/' || build~targetacct()'/website/docroot/builds/status'
build~logfile = build~homedir() || '/' || build~date() || '-' || build~osname()

-- do the build
call directory build~homedir()
build~log('Starting build.')
build~do_build()
build~log('Finished build.')

-- Cleanup   
'scp' build~logfile() build~targetacct() || '@' || build~targethost() || ':' || build~targetdir()
call SysFileDelete build~logfile()
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

::attribute osname        -- the name of this OS and machine type
::attribute osnamefile    -- the file containing the osname value
::attribute acct          -- the account to used to perform the build
::attribute homedir       -- the build acct home dir
::attribute date          -- the date the build was started
::attribute type          -- the build type (exe, rpm, deb, docs, etc)
::attribute localdir      -- the local temp build dir
::attribute svnver        -- the current subversion revision
::attribute targetacct    -- the account on the build server
::attribute targethost    -- the target system ip addr or dns name
::attribute targetdir     -- the target dir for build outputs
::attribute logdir        -- the target dir for the build status file
::attribute logfile       -- the local build status file

/*----------------------------------------------------------------------------*/
/* Method: log                                                                */
/*----------------------------------------------------------------------------*/

::method log
-- log messages
use strict arg msg
strm = .stream~new(self~logfile())
strm~open('write append')
msg = date('S') time('N') msg
say msg
strm~lineout(msg)
strm~close()
return

/*----------------------------------------------------------------------------*/
/* Method: targetexist                                                        */
/*----------------------------------------------------------------------------*/

::method targetexists
use strict arg target
tempf = '/tmp/orxbuild.tmp'
'ssh' self~targetacct()'@'self~targethost() '"ls -l' target'" >' tempf
strm = .stream~new(tempf)
strm~open('read')
arr = strm~arrayin()
strm~close()
if arr~items() = 0 then return .false
if arr[1]~pos('cannot access') > 0 then return .false
return .true

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
/* Method: do_build                                                           */
/*----------------------------------------------------------------------------*/

::method do_build
use strict arg
savedir = directory()
-- create temp dir and checkout the source
'rm -rf' self~localdir()  -- make sure the temp subdir is removed
'mkdir' self~localdir()
'svn co http://oorexx.svn.sourceforge.net/svnroot/oorexx/main/trunk/' self~localdir
call directory self~localdir()
-- see if we have already built this revision
self~svnver = self~getsvnrevision()
if \datatype(self~svnver(), 'W') then do
   self~log('Subversion checkout failed.')
   return
   end
newdir = self~targetdir'/'self~svnver()'/'self~osname
if self~targetexists(newdir) = .false then do
   select
      when self~type() = 'rpm' then self~build_rpm(newdir)
      when self~type() = 'deb' then self~build_deb(newdir)
      when self~type() = 'exe' then self~build_exe(newdir)
      otherwise  do
         self~log('Unknown build type. No output files produced.')
         end
      end
   end   
else self~log('This was a duplicate build request for SVN revision' svnver'.')
-- remove everything
call directory savedir
'rm -rf' self~localdir()
'rm oorexx*.deb'
return


/*----------------------------------------------------------------------------*/
/* Method: build_deb                                                          */
/*----------------------------------------------------------------------------*/

::method build_deb
use strict arg newdir
buildrpt = self~osname()'.buildrpt.txt'
self~log('Building SVN revision' self~svnver()'.')
'./bootstrap 2>&1 | tee -a' buildrpt
'./configure --disable-static 2>&1 | tee -a' buildrpt
'make deb 2>&1 | tee -a' buildrpt
-- copy the results to the host
'ssh' self~targetacct()'@'self~tergethost() '"mkdir -p' newdir'"'
'scp ../oorexx*.deb' self~targetacct()'@'self~targethost()':'newdir
'scp' buildrpt self~targetact()'@'self~targethost()':'newdir
return


/*----------------------------------------------------------------------------*/
/* Method: build_rpm                                                          */
/*----------------------------------------------------------------------------*/

::method build_rpm
use strict arg newdir
buildrpt = self~osname()'.buildrpt.txt'
self~log('Building SVN revision' self~svnver()'.')
'./bootstrap 2>&1 | tee -a' buildrpt
'./configure 2>&1 | tee -a' buildrpt
'make rpm 2>&1 | tee -a' buildrpt
-- copy the results to the host
'ssh' self~targetacct()'@'self~tergethost() '"mkdir -p' newdir'"'
if SysIsFileDirectory('./rpm/RPMS/i386') then ,
 'scp ./rpm/RPMS/i386/ooRexx*.rpm' self~targetacct()'@'self~targethost()':'newdir
else if SysIsFileDirectory('./rpm/RPMS/i486') then ,
 'scp ./rpm/RPMS/i486/ooRexx*.rpm' self~targetacct()'@'self~targethost()':'newdir
else if SysIsFileDirectory('./rpm/RPMS/i586') then ,
 'scp ./rpm/RPMS/i586/ooRexx*.rpm' self~targetacct()'@'self~targethost()':'newdir
else if SysIsFileDirectory('./rpm/RPMS/i686') then ,
 'scp ./rpm/RPMS/i686/ooRexx*.rpm' self~targetacct()'@'self~targethost()':'newdir
else if SysIsFileDirectory('./rpm/RPMS/x86_64') then ,
 'scp ./rpm/RPMS/x86_64/ooRexx*.rpm' self~targetacct()'@'self~targethost()':'newdir
else if SysIsFileDirectory('./rpm/RPMS/s390x') then ,
 'scp ./rpm/RPMS/s390x/ooRexx*.rpm' self~targetacct()'@'self~targethost()':'newdir
else if SysIsFileDirectory('./rpm/RPMS/s390') then ,
 'scp ./rpm/RPMS/s390/ooRexx*.rpm' self~targetacct()'@'self~targethost()':'newdir
else self~log('Unknown rpm type, no output rpms copied.')
'scp' buildrpt self~targetacct()'@'self~targethost()':'newdir
return


/*----------------------------------------------------------------------------*/
/* Method: get_osname                                                         */
/*----------------------------------------------------------------------------*/

::method get_osname
use strict arg osname
-- if there was a program argument use that as the osname
if osname <> '' then return osname
-- see if the osname is in a file
if SysIsFile(self~osnamefile()) then do
   arr = self~file_arrayin(self~osnamefile())
   if arr~items() > 0 & arr[1]~strip() <> '' then return arr[1]~strip()
   end
-- now try to dynamically figure out the osname
-- try fedora before redhat
if SysIsFile('/etc/fedora-release') then do
   arr = self~file_arrayin('/etc/fedora-release')
   ln = arr[1]
   parse var ln 'Fedora' 'release' release .
   osname = 'fedora'release
   end
-- try red hat
if SysIsFile('/etc/redhat-release') then do
   arr = self~file_arrayin('/etc/redhat-release')
   ln = arr[1]
   parse var ln 'Red Hat' 'release' release .
   osname = 'redhat'release
   end
-- try SuSE
if SysIsFile('/etc/SuSE-release') then do
   arr = self~file_arrayin('/etc/SuSE-release')
   if arr[1]~pos('SUSE Linux Enterprise Server') > 0 then relname = 'SLES'
   ln = arr[2]
   parse var ln 'VERSION =' ver
   ln = arr[3]
   parse var ln 'PATCHLEVEL =' patch
   osname = relname || ver || patch
   end
-- try debian/ubuntu
if SysIsFile('/etc/lsb-release') then do
   arr = self~file_arrayin('/etc/lsb-release')
   ln = arr[1]
   parse var ln 'DISTRIB_ID=' relname .
   relname = relname~lower()
   ln = arr[2]
   parse var ln 'DISTRIB_RELEASE=' release .
   osname = relname || release
   end
else osname = 'Unknown'
-- now get the architecture
'uname -m > /tmp/machtype.txt'
arr = self~file_arrayin('/tmp/machtype.txt')
ln = arr[1]
parse var ln mach .
if mach = 'i486' then mach = 'i386'
else if mach = 'i586' then mach = 'i386'
else if mach = 'i686' then mach = 'i386'
else nop
-- now return the full name
return osname || '-' || mach


/*----------------------------------------------------------------------------*/
/* Method: file_arrayin                                                       */
/*----------------------------------------------------------------------------*/

::method file_arrayin private
use strict arg file
strm = .stream~new(file)
strm~open('read')
arr = strm~arrayin()
strm~close()
return arr

