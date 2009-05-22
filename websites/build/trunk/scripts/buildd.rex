#!/usr/bin/rexx
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Description: This is the build daemon for the Red Hat/Fedora VMware huest. */
/*                                                                            */
/* Copyright (c) 2007 Rexx Language Association. All rights reserved.         */
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
/*    1. When this runs on Windows we need to avoid any RexxUtil functions    */
/*       because they reside in RXAPI and that gets stopped and restarted     */
/*       multiple times during the build process.                             */
/*                                                                            */
/*----------------------------------------------------------------------------*/


-- make sure our command file name was passed
cmdfilename = arg(1)
if cmdfilename = '' then do
   say 'Error: No command file name given.'
   say 'Syntax:'
   say '   buildd.rex cmdfilename'
   say 'where cmdfilename is one of the following:'
   say '   fedora7-build'
   say '   fedoracore6-build'
   say '   ubuntu704-build'
   say '   winxpprof-build'
   return
   end

-- set up the defaults
parse source osver
if osver~pos('Windows') > 0 then windows = .true
else windows = .false
host = 'build.oorexx.org'
hostacct = 'buildorx'
if windows then do
   hostbuilds = 'p:\builds'
   cmdfile = 'p:\cmds\'cmdfilename
   cmdlocal = '.\cmd.txt'
   end
else do
   hostbuilds = '/home/buildorx/builds'
   cmdfile = '/home/buildorx/website/cmds/'cmdfilename
   cmdlocation = hostacct'@'host':'cmdfile
   buildrpt = cmdfilename'-buildrpt.txt'
   cmdlocal = './cmd.txt'
   end
say 'Using command file' cmdfile

-- make sure our temp dir is empty
if \windows then 'rm -rf ./temp'
else 'rmdir /S /Q c:\buildtemp'

do forever
   -- see if we have anything to do
   if windows then 'copy' cmdfile cmdlocal
   else 'scp' cmdlocation cmdlocal
   if stream(cmdlocal, 'C', 'QUERY EXISTS') = '' then do
      say date() time('N') 'Nothing to do, sleeping for 30 seconds.'
      if \windows then call SysSleep 30
      else 'ping -n 31 127.0.0.1 > NUL'
      iterate
      end
   cmdline = getline(cmdlocal)
   parse var cmdline cmd target addressee .
   if windows then 'del' cmdlocal
   else call SysFileDelete cmdlocal
   -- perform the command
   select
      when cmd = 'docs' then do
         -- create temp dir and checkout the source
         'mkdir temp'
         call directory './temp'
         'svn co http://oorexx.svn.sourceforge.net/svnroot/oorexx/docs/trunk'
         call directory './trunk'
         -- see if we have already built this revision
         svnver = getsvnrevision()
         newdir = hostbuilds'/docs/'svnver
         sshcmd = 'if [ -e' newdir ']; then exit 1; else exit 0; fi;'
         sshcmd = 'ssh buildorx@build.oorexx.org "'sshcmd'"'
         address bash sshcmd
         if rc = 0 then do
            -- build the docs
            'make' target '2>&1 | tee -a' buildrpt
            -- copy the results to the host
            'ssh' hostacct'@'host 'mkdir' newdir
            'scp ./oodialog/oodialog.pdf' hostacct'@'host':'newdir
            'scp ./oodialog/oodialog-html.zip' hostacct'@'host':'newdir
            'scp ./readme/readme.pdf' hostacct'@'host':'newdir
            'scp ./readme/readme.html' hostacct'@'host':'newdir
            'scp ./rexxpg/rexxpg.pdf' hostacct'@'host':'newdir
            'scp ./rexxpg/rexxpg-html.zip' hostacct'@'host':'newdir
            'scp ./rexxref/rexxref.pdf' hostacct'@'host':'newdir
            'scp ./rexxref/rexxref-html.zip' hostacct'@'host':'newdir
            'scp ./rxftp/rxftp.pdf' hostacct'@'host':'newdir
            'scp ./rxftp/rxftp-html.zip' hostacct'@'host':'newdir
            'scp ./rxmath/rxmath.pdf' hostacct'@'host':'newdir
            'scp ./rxmath/rxmath-html.zip' hostacct'@'host':'newdir
            'scp ./rxsock/rxsock.pdf' hostacct'@'host':'newdir
            'scp ./rxsock/rxsock-html.zip' hostacct'@'host':'newdir
            'scp' buildrpt hostacct'@'host':'newdir
            end
         -- notify the user
         call docs_notify addressee, svnver
         -- remove everything
         call directory '../..'
         'rm -rf ./temp'
         -- be sure to remove the command file on the host
         'ssh' hostacct'@'host 'rm' cmdfile
         end
      when cmd = 'interpreter-main' then do
         if target = 'rpm' then do
            -- make sure we are not windows
            if windows then do
               call error_notify addressee
               'del' cmdfile
               iterate
               end
            -- create temp dir and checkout the source
            'mkdir temp'
            call directory './temp'
            'mkdir ./main'
            'svn co http://oorexx.svn.sourceforge.net/svnroot/oorexx/main/trunk/ ./main'
            call directory './main'
            svnver = getsvnrevision()
            newdir = hostbuilds'/interpreter-main/'svnver
            sshcmd = 'if [ -e' newdir'/'cmdfilename ']; then exit 1; else exit 0; fi;'
            sshcmd = 'ssh buildorx@build.oorexx.org "'sshcmd'"'
            address bash sshcmd
            if rc = 0 then do
               -- build the rpm
               './bootstrap 2>&1 | tee -a' buildrpt
               './configure 2>&1 | tee -a' buildrpt
               'make' target '2>&1 | tee -a' buildrpt
               -- copy the results to the host
               'ssh' hostacct'@'host 'mkdir' newdir
               newdir = newdir'/'cmdfilename
               'ssh' hostacct'@'host 'mkdir' newdir
               'scp ./rpm/RPMS/i386/ooRexx*.rpm' hostacct'@'host':'newdir
               'scp' buildrpt hostacct'@'host':'newdir
               end
            -- notify the user
            call interpretermain_notify addressee, svnver, cmdfilename
            -- remove everything
            call directory '../..'
            'rm -rf ./temp'
            -- be sure to remove the command file on the host
            'ssh' hostacct'@'host 'rm' cmdfile
            end
         else if target = 'deb' then do
            -- make sure we are not windows
            if windows then do
               call error_notify addressee
               'del' cmdfile
               iterate
               end
            -- create temp dir and checkout the source
            'mkdir temp'
            call directory './temp'
            'mkdir main'
            'svn co http://oorexx.svn.sourceforge.net/svnroot/oorexx/main/trunk/ ./main'
            call directory './main'
            svnver = getsvnrevision()
            newdir = hostbuilds'/interpreter-main/'svnver
            sshcmd = 'if [ -e' newdir'/'cmdfilename ']; then exit 1; else exit 0; fi;'
            sshcmd = 'ssh buildorx@build.oorexx.org "'sshcmd'"'
            address bash sshcmd
            if rc = 0 then do
               -- build the rpm
               './bootstrap 2>&1 | tee -a' buildrpt
               './configure --disable-static 2>&1 | tee -a' buildrpt
               'make' target '2>&1 | tee -a' buildrpt
               -- copy the results to the host
               'ssh' hostacct'@'host 'mkdir' newdir
               newdir = newdir'/'cmdfilename
               'ssh' hostacct'@'host 'mkdir' newdir
               'scp ../oorexx*.deb' hostacct'@'host':'newdir
               'scp' buildrpt hostacct'@'host':'newdir
               end
            -- notify the user
            call interpretermain_notify addressee, svnver, cmdfilename
            -- remove everything
            call directory '../..'
            'rm -rf ./temp'
            -- be sure to remove the command file on the host
            'ssh' hostacct'@'host 'rm' cmdfile
            end
         else if target = 'exe' then do
            -- Note: Do not use RexxUtil functions because rxapi will be killed!
            if \windows then do
               call error_notify addressee
               'ssh' hostacct'@'host 'rm' cmdfile
               iterate
               end
            -- create temp dir and checkout the source
            'md c:\buildtemp'
            call directory 'c:\buildtemp'
            'svn co http://oorexx.svn.sourceforge.net/svnroot/oorexx/main/trunk/'
            call directory 'trunk'
            -- build the exe
            call value 'SRC_DRV', 'c:', 'ENVIRONMENT'
            call value 'SRC_DIR', '\buildtemp\trunk', 'ENVIRONMENT'
            call win_setlatestdocs
            svnver = win_getsvnrevision()
            'makeorx.bat BOTH PACKAGE'
            -- copy the results to the host
            newdir = hostbuilds'\interpreter-main\'svnver
            'md' newdir
            newdir = newdir'\'cmdfilename
            'md' newdir
            'copy ooRexx*.exe' newdir
            'copy Win32Rel\Win32Rel.log' newdir'\Win32RelLog.txt'
            'copy Win32Dbg\Win32Dbg.log' newdir'\Win32DbgLog.txt'
            -- notify the user
            call interpretermain_notify addressee, svnver, cmdfilename
            -- remove everything
            call directory 'c:\'
            'rmdir /S /Q c:\buildtemp'
            -- be sure to remove the command file on the host
            'killer rxapi' -- this ensures we can remove everything in \buildtemp
            'del' cmdfile
            end
         end
      otherwise nop
      end
   if \windows then call SysSleep 30
   else 'ping -n 31 127.0.0.1 > NUL'
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
verlocal = './temp.ver'
'svnversion >' verlocal
svnver = getline(verlocal)
if svnver = '' then svnver = 'unknown'
return svnver

win_getsvnrevision: procedure
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

win_setlatestdocs:
-- Do not use SysFileTree here! RXAPI may not be running!
'dir /aD /oN p:\builds\docs > dirlist.txt'
strm = .stream~new('dirlist.txt')
retc = strm~open('read')
if retc <> 'READY:' then return 'unknown'
lines = strm~arrayIn()
retc = strm~close()
dirline = lines[lines~items() - 2]
parse var dirline . . . . docdir .  -- this only works for WinXP
if docdir = '' then do
   parse var dirline . . . docdir .  -- this works for Win2k
   end
if docdir <> '' then do
   call value 'DOC_LOCATION', 'p:\builds\docs\'docdir, 'ENVIRONMENT'
   end
'del dirlist.txt'
say 'The doc_location is p:\builds\docs\'docdir
return

docs_notify: procedure
use arg addressee, svnver
if addressee = '' then return
filename = './notify.txt'
strm = .stream~new(filename)
retc = strm~open('write')
if retc <> 'READY:' then return ''
strm~lineOut('')
strm~lineOut('Do not reply to this email!')
strm~lineOut('')
strm~lineOut('The ooRexx documentation build you requested is finished. It can')
strm~lineOut('be downloaded at http://build.oorexx.org/builds/docs/'svnver'.')
strm~lineOut('')
strm~lineOut('The build may or may not have completed successfully. The file')
strm~lineOut('buildrpt.txt contains the output of the build process so you can')
strm~lineOut('look for error messages in it.')
strm~lineOut('')
strm~lineOut('Thank you for using the ooRexx Build Machine!')
strm~lineOut('The ooRexx Project Team')
strm~lineOut('')
retc = strm~close()
'mail -s "Your ooRexx Build Is Complete"' addressee '<' filename
return

interpretermain_notify: procedure expose windows
use arg addressee, svnver, cmdfilename
if addressee = '' then return
if cmdfile = '' then return
filename = './notify.txt'
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
if windows then 'p:\windows\common\blat262\full\blat notify.txt -to' ,
 addressee '-f buildorx@build.oorexx.org -server holmes4.com -u dashley -pw ashley'
else 'mail -s "Your ooRexx Build Is Complete"' addressee '<' filename
return

interpreter4x_notify: procedure expose windows
use arg addressee, svnver, cmdfilename
if addressee = '' then return
filename = './notify.txt'
if cmdfile = '' then return
strm = .stream~new(filename)
retc = strm~open('write')
if retc <> 'READY:' then return ''
strm~lineOut('')
strm~lineOut('Do not reply to this email!')
strm~lineOut('')
strm~lineOut('The ooRexx interpreter build you requested is finished. It can')
strm~lineOut('be downloaded at http://build.oorexx.org/builds/interpreter-4.x/'svnver'/'cmdfilename'.')
strm~lineOut('')
strm~lineOut('The build may or may not have completed successfully. The file')
strm~lineOut('buildrpt.txt or Win32*.log contains the output of the build')
strm~lineOut('process so you can look for error messages in it.')
strm~lineOut('')
strm~lineOut('Thank you for using the ooRexx Build Machine!')
strm~lineOut('The ooRexx Project Team')
strm~lineOut('')
retc = strm~close()
if windows then 'p:\windows\common\blat262\full\blat notify.txt -to' ,
 addressee '-f buildorx@build.oorexx.org -server holmes4.com -u dashley -pw ashley'
else 'mail -s "Your ooRexx Build Is Complete"' addressee '<' filename
return

error_notify: procedure expose windows
use arg addressee
if addressee = '' then return
filename = './notify.txt'
strm = .stream~new(filename)
retc = strm~open('write')
if retc <> 'READY:' then return ''
strm~lineOut('')
strm~lineOut('Do not reply to this email!')
strm~lineOut('')
strm~lineOut('The ooRexx build type specified on your request was invalid. Please')
strm~lineOut('resubmit your request with the correct build type.')
strm~lineOut('')
strm~lineOut('Thank you for using the ooRexx Build Machine!')
strm~lineOut('The ooRexx Project Team')
strm~lineOut('')
retc = strm~close()
if windows then 'p:\windows\common\blat262\full\blat notify.txt -to' ,
 addressee '-f buildorx@build.oorexx.org -server holmes4.com -u dashley -pw ashley'
else 'mail -s "Your ooRexx Build Is Complete"' addressee '<' filename
return

