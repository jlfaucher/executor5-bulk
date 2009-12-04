#!/usr/bin/rexx                      
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Description: This is the build daemon for the Windows KVM guest OS.        */
/*                                                                            */
/* Copyright (c) 2007-2010 Rexx Language Association. All rights reserved.    */
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
cmdfilename = 'winxp-build'
hostbuilds = 'r:'
cmdfile = 's:\cmds\'cmdfilename
cmdlocal = 'c:\cmd.txt'
host = '192.168.0.104'
hostacct = 'dashley'
builddir = 'c:\buildtemp'

-- make sure our temp dir is empty
'rmdir /S /Q' builddir

do forever
   -- see if we have anything to do
   'copy' cmdfile cmdlocal
   if stream(cmdlocal, 'C', 'QUERY EXISTS') = '' then do
      say date() time('N') 'Nothing to do, sleeping for 30 seconds.'
      'ping -n 31 127.0.0.1 > NUL'
      iterate
      end
   cmdline = getline(cmdlocal)
   parse var cmdline cmd target addressee .
   'del' cmdlocal
   -- perform the command
   select
      when cmd = 'interpreter-main' then do
         -- Note: Do not use RexxUtil functions because rxapi will be killed!
         -- create temp dir and checkout the source
         'md' builddir
         say 'Performing SVN checkout'
         'svn co http://oorexx.svn.sourceforge.net/svnroot/oorexx/main/trunk/' builddir
         call directory builddir'/trunk'
         -- build the exe
         call value 'SRC_DRV', 'c:', 'ENVIRONMENT'
         call value 'SRC_DIR', '\buildtemp\trunk', 'ENVIRONMENT'
         call setlatestdocs
         svnver = getsvnrevision()
         say 'Building ooRexx'
         'makeorx.bat BOTH PACKAGE'
         -- copy the results to the host
         say 'Copying build output files to the server'
         newdir = hostbuilds'\interpreter-main\'svnver
         'md' newdir
         newdir = newdir'\'cmdfilename
         'md' newdir
         'copy ooRexx*.exe' newdir
         'copy Win32Rel\Win32Rel.log' newdir'\Win32RelLog.txt'
         'copy Win32Dbg\Win32Dbg.log' newdir'\Win32DbgLog.txt'
         -- notify the user
         say 'Sending email'
         call interpretermain_notify addressee, svnver, cmdfilename
         -- remove everything
         say 'Cleanup'
         call directory 'c:\'
         'rmdir /S /Q' builddir
         -- be sure to remove the command file on the host
         'killer rxapi' -- this ensures we can remove everything in \buildtemp
         'del' cmdfile
         end
      otherwise nop
      end
   'ping -n 31 127.0.0.1 > NUL'
   end
return


getline: procedure
use arg filename
strm = .stream~new(filename)
retc = strm~open('read')
if retc <> 'READY:' then return ''
line = strm~lineIn()
retc = strm~close()
return line

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
say 'The SVN Revision is' svnver
return svnver

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
say 'The doc_location is' hostbuilds'\docs\'docdir
return

interpretermain_notify: procedure
use arg addressee, svnver, cmdfilename
if addressee = '' then return
if cmdfile = '' then return
filename = 'c:\notify.txt'
strm = .stream~new(filename)
retc = strm~open('write')
if retc <> 'READY:' then return ''
strm~lineOut('')
strm~lineOut('Do not reply to this email!')
strm~lineOut('')
strm~lineOut('The ooRexx interpreter build you requested is finished. It can')
strm~lineOut('be downloaded at http://build.oorexx.org/builds/interpreter-main/'svnver'/'cmdfilename'.')
strm~lineOut('')
strm~lineOut('The build may or may not have completed successfully. The file')
strm~lineOut('buildrpt.txt or Win32*.log contains the output of the build')
strm~lineOut('process so you can look for error messages in it.')
strm~lineOut('')
strm~lineOut('Thank you for using the ooRexx Build Machine!')
strm~lineOut('The ooRexx Project Team')
strm~lineOut('')
retc = strm~close()
'c:\bin\blat notify.txt -to' addressee '-f noreply@build.oorexx.org -server holmes4.com -u dashley -pw wda123aa'
'del c:\notify.txt'
return

