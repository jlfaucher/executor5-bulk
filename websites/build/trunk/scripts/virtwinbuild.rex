/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Description: This is the build script for the Windows KVM guest OS.        */
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
/* Notes:                                                                     */
/*    1. On Windows we need to avoid any RexxUtil functions because they      */
/*       reside in RXAPI and that gets stopped and restarted multiple times   */
/*       during the build process.                                            */
/*                                                                            */
/*----------------------------------------------------------------------------*/


-- initialization
hostbuilds = 's:'
builddir = 'c:\buildtemp'
osname = 'winxpsp3-i386'
targetdir = hostbuilds'\interpreter-main'
startbuildtime = 165  -- 2:45 AM

-- make sure our temp dir is empty
'rmdir /S /Q' builddir

-- see if we just do one build only
cmdline = arg(1)~strip()
if cmdline = '--immediate' then do
   call dobuild
   return
   end

-- wait until the specified time
do forever
   if time('M') = startbuildtime then call dobuild
   'sleep 50'
   say 'Current time is' time('C')
   end
return


/*----------------------------------------------------------------------------*/
/* dobuild                                                                    */
/*----------------------------------------------------------------------------*/

dobuild:
builddate = date('S')  -- do this first!!!!
call log 'Starting build.'
-- create temp dir and checkout the source
'md' builddir
'svn co http://oorexx.svn.sourceforge.net/svnroot/oorexx/main/trunk/' builddir
call directory builddir
-- build the exe
call value 'SRC_DRV', 'c:', 'ENVIRONMENT'
call value 'SRC_DIR', '\buildtemp', 'ENVIRONMENT'
call setlatestdocs
svnver = getsvnrevision()
if datatype(svnver, 'W') then do
   call log 'Subversion checkout failed.'
   return
   end
if checkbuild(targetdir'\'svnver'\'osname) = .false then do
   call log 'Building ooRexx'
   'makeorx.bat BOTH PACKAGE'
   -- copy the results to the host
   call log 'Copying build output files to the server'
   newdir = targetdir'\'svnver
   'md' newdir
   newdir = newdir'\'osname
   'md' newdir
   'copy ooRexx*.exe' newdir
   'copy Win32Rel\Win32Rel.log' newdir'\Win32RelLog.txt'
   'copy Win32Dbg\Win32Dbg.log' newdir'\Win32DbgLog.txt'
   -- check build success
   if \checkbuild(newdir) then do
      call log 'Build was bad, no output files produced'
      end
   end
else do
   call log 'This was a duplicate build request for SVN revision' svnver'.'
   end
-- remove everything
call directory 'c:\'
'rmdir /S /Q' builddir
call log 'Finished build.'
return


/*----------------------------------------------------------------------------*/
/* getsvnrevision                                                             */
/*----------------------------------------------------------------------------*/

getsvnrevision: procedure
'svn info > svntemp'
strm = .stream~new('svntemp')
retc = strm~open('read')
if retc <> 'READY:' then return 'unknown'
lines = strm~arrayIn()
retc = strm~close()
revline = lines[5]
parse var revline . svnver .
if svnver = '' then svnver = 'unknown'
-- say 'The SVN Revision is' svnver
return svnver


/*----------------------------------------------------------------------------*/
/* setlatestdocs                                                              */
/*----------------------------------------------------------------------------*/

setlatestdocs:
-- Do not use SysFileTree here! RXAPI may not be running!
'dir /aD /oN' hostbuilds'\docs > dirlist.txt'
strm = .stream~new('dirlist.txt')
retc = strm~open('read')
if retc <> 'READY:' then return 'unknown'
lines = strm~arrayIn()
retc = strm~close()
dirline = lines[lines~items() - 2]
parse var dirline . . . . docdir .  -- this only works for WinXP
if docdir = '' then do
   parse var dirline . . . docdir . -- this works for Win2k
   end
if docdir <> '' then do
   call value 'DOC_LOCATION', hostbuilds'\docs\'docdir, 'ENVIRONMENT'
   end
'del dirlist.txt'
-- say 'The doc_location is' hostbuilds'\docs\'docdir
return


/*----------------------------------------------------------------------------*/
/* log                                                                        */
/*----------------------------------------------------------------------------*/

log:
-- log messages
use strict arg msg
strm = .stream~new(hostbuilds'\status\' || builddate || '-' || osname)
strm~open('write append')
msg = date('S') time('N') msg
say msg
strm~lineout(msg)
strm~close()
return


/*----------------------------------------------------------------------------*/
/* checkbuild                                                                 */
/*----------------------------------------------------------------------------*/

checkbuild: procedure
-- check fo good build
use strict arg newdir
'dir' newdir'\ooRexx*.exe > checkexists'
strm = .stream~new('checkexists')
retc = strm~open('read')
if retc <> 'READY:' then return 'unknown'
lines = strm~arrayIn()
retc = strm~close()
count = 0
do line over lines
   if line~pos('ooRexx') > 0 & line~pos('.exe') > 0 then count += 1
   end
if count < 2 then return .false -- bad build
return .true  -- good build

