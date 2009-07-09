#!/usr/bin/rexx
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Description: This is the build daemon for the Red Hat/Fedora VMware huest. */
/*                                                                            */
/* Copyright (c) 2007-2009 Rexx Language Association. All rights reserved.    */
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


-- ensure we are called with arguments
cmdline = arg(1)
if cmdline = '' then do
   say 'Error: no command line arguments found.'
   call invocation_syntax
   return
   end


-- initialize our global variables
txnserver = '192.168.0.104'
txnport = 15776
parse var cmdline qname cmdline
select
   when qname = 'win.i386' then nop
   when qname = 'win.x86_64' then nop
   when qname = 'fedora11.i386' then nop
   when qname = 'fedora11.x86_64' then nop
   when qname = 'ubuntu8.04.i386' then nop
   when qname = 'docs' then nop
   otherwise  do
      say 'Error: incorrect build queue name argument found.'
      call invocation_syntax
      return
      end
   end

-- see if there is anything to do
do forever
   s = .streamsocket~new(txnserver, txnport)
   retc = s~open()
   if retc <> 'READY:'then do
      say 'Error' retc 'connecting to' txnserver 'server stream.'
      return
      end
   retc = s~lineout('items' qname)
   items = s~linein()
   s~close()
   if items > 0 then do
      -- we found a request
      s = .streamsocket~new(txnserver, txnport)
      retc = s~open()
      if retc <> 'READY:'then do
         say 'Error' retc 'connecting to' txnserver 'server stream.'
         return
         end
      retc = s~lineout('pull' qname)
      request = s~linein()
      call process_request qname, request
      s~close()
      end
   else do
      say date() time() 'Nothing to do, sleeping for 30 seconds.'
      call syssleep 30
      end
   end


::requires 'streamsocket.cls'


/*----------------------------------------------------------------------------*/
/* invocation_syntax                                                          */
/*----------------------------------------------------------------------------*/

::routine invocation_syntax
say 'Syntax:'
say '   buildd.rex qname'
say 'where qname is one of the following:'
say '   win.i386'
say '   win.x86_64'
say '   fedora11.i386'
say '   fedora11.x86_64'
say '   ubuntu8.04.i386'
say '   docs'
return


/*----------------------------------------------------------------------------*/
/* process_request                                                            */
/*----------------------------------------------------------------------------*/

::routine process_request
use strict arg qname email
select
   when qname = 'win.i386' then do
      call build_exe email
      end
--   when qname = 'win.x86_64' then do
--      call build_exe email
--      end
   when qname = 'fedora11.i386' then do
      call build_rpm qname, email
      end
   when qname = 'fedora11.x86_64' then do
      call build_rpm qname, email
      end
   when qname = 'ubunto8.04.i386' then do
      call build_deb email
      end
   when qname = 'docs' then do
      call build_docs email
      end
   otherwise do
      call error_notify email
      end
   end
return


/*----------------------------------------------------------------------------*/
/* build_rpm                                                                  */
/*----------------------------------------------------------------------------*/

::routine build_rpm
use strict arg qname email
if qname~pos('i386') > 0 then buildrpt = 'rpm.i386-buildrpt.txt'
else buildrpt = 'rpm.x86_64-buildrpt.txt'
targetdir = '/mnt/buildorx/builds/interpreter-main'
savedir = directory()
-- create temp dir and checkout the source
tempdir = '/tmp/buildorx'
'rm -rf' tempdir  -- make sure the subdir is erased
'mkdir' tempdir
'svn co http://oorexx.svn.sourceforge.net/svnroot/oorexx/main/trunk/' tempdir
call directory tempdir
-- see if we have already built this revision
svnver = getsvnrevision()
newdir = targetdir'/'svnver
if \sysisfile(newdir'/'buildrpt) then do
   -- build the rpm
   './bootstrap 2>&1 | tee -a' buildrpt
   './configure 2>&1 | tee -a' buildrpt
   'make rpm 2>&1 | tee -a' buildrpt
   -- copy the results to the host
   'mkdir' newdir
   if SysIsFileDirectory('./rpm/RPMS/i386') then ,
    'cp ./rpm/RPMS/i386/ooRexx*.rpm' newdir
   else if SysIsFileDirectory('./rpm/RPMS/i486') then ,
    'cp ./rpm/RPMS/i486/ooRexx*.rpm' newdir
   else if SysIsFileDirectory('./rpm/RPMS/i586') then ,
    'cp ./rpm/RPMS/i586/ooRexx*.rpm' newdir
   else if SysIsFileDirectory('./rpm/RPMS/i686') then ,
    'cp ./rpm/RPMS/i686/ooRexx*.rpm' newdir
   else if SysIsFileDirectory('./rpm/RPMS/x86_64') then ,
    'cp ./rpm/RPMS/x86_64/ooRexx*.rpm' newdir
   else nop -- must not be a supported rpm
   'cp' buildrpt newdir
   end
-- notify the user
call interpretermain_notify email, svnver, buildrpt
-- remove everything
call directory savedir
'rm -rf' tempdir
return


/*----------------------------------------------------------------------------*/
/* build_deb                                                                  */
/*----------------------------------------------------------------------------*/

::routine build_deb
use strict arg qname email
buildrpt = 'deb.i386-buildrpt.txt'
targetdir = '/mnt/buildorx/builds/interpreter-main'
savedir = directory()
-- create temp dir and checkout the source
tempdir = '/tmp/buildorx'
'rm -rf' tempdir  -- make sure the subdir is erased
'mkdir' tempdir
'svn co http://oorexx.svn.sourceforge.net/svnroot/oorexx/main/trunk/' tempdir
call directory tempdir
-- see if we have already built this revision
svnver = getsvnrevision()
newdir = targetdir'/'svnver
if \sysisfile(newdir'/'buildrpt) then do
   -- build the deb
   './bootstrap 2>&1 | tee -a' buildrpt
   './configure --disable-static 2>&1 | tee -a' buildrpt
   'make deb 2>&1 | tee -a' buildrpt
   -- copy the results to the host
   'mkdir' newdir
   'cp ../oorexx*.deb' newdir
   'cp' buildrpt newdir
   end
-- notify the user
call interpretermain_notify email, svnver, buildrpt
-- remove everything
call directory savedir
'rm -rf ./temp'
return


/*----------------------------------------------------------------------------*/
/* build_exe                                                                  */
/*----------------------------------------------------------------------------*/

::routine build_exe
use strict arg email
-- Note: Do not use RexxUtil functions because rxapi will be killed!
buildrpt = 'exe.i386-buildrpt.txt'
targetdir = 'p:\builds\interpreter-main'
savedir = directory()
-- create temp dir and checkout the source
tempdir = 'c:\buildorx'
'rmdir /S /Q c:\buildorx'   -- make sure the subdir is eraced
'md' tempdir
'svn co http://oorexx.svn.sourceforge.net/svnroot/oorexx/main/trunk/' tempdir
call directory tempdir
-- set up the build environment
call value 'SRC_DRV', 'c:', 'ENVIRONMENT'
call value 'SRC_DIR', '\buildorx', 'ENVIRONMENT'
call win_setlatestdocs
-- see if we have already built this revision
svnver = win_getsvnrevision()
newdir = targetdir'\'svnver
-- TODO: We really should check to see if this revision is already built like the rpm build.
-- build the exe
svnver = win_getsvnrevision()
'makeorx.bat BOTH PACKAGE'
-- copy the results to the host
'md' newdir
'copy ooRexx*.exe' newdir
'copy Win32Rel\Win32Rel.log' newdir
'copy Win32Dbg\Win32Dbg.log' newdir
-- notify the user
call interpretermain_notify addressee, svnver, buildrpt
-- remove everything
call directory savedir
'killer rxapi' -- this ensures we can remove everything in \buildorx
'rmdir /S /Q c:\buildorx'
return

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


/*----------------------------------------------------------------------------*/
/* build_docs                                                                 */
/*----------------------------------------------------------------------------*/

::routine build_docs
use strict arg email
buildrpt = 'docs-buildrpt.txt'
targetdir = '/mnt/buildorx/builds/docs'
savedir = directory()
-- create temp dir and checkout the source
tempdir = '/tmp/builddocs'
'rm -rf' tempdir   -- make sure the subdir is erased
'mkdir' tempdir
'svn co http://oorexx.svn.sourceforge.net/svnroot/oorexx/docs/trunk' tempdir
call directory tempdir
-- see if we have already built this revision
svnver = getsvnrevision()
newdir = targetdir'/'svnver
if \sysisfiledirectory(newdir) then do
   -- build the docs
   'make all 2>&1 | tee -a' buildrpt
   -- copy the results to the host
   'mkdir' newdir
   'cp ./oodialog/oodialog.pdf' newdir
   'cp ./oodialog/oodialog-html.zip' newdir
   'cp ./readme/readme.pdf' newdir
   'cp ./readme/readme.html' newdir
   'cp ./rexxpg/rexxpg.pdf' newdir
   'cp ./rexxpg/rexxpg-html.zip' newdir
   'cp ./rexxref/rexxref.pdf' newdir
   'cp ./rexxref/rexxref-html.zip' newdir
   'cp ./rxftp/rxftp.pdf' newdir
   'cp ./rxftp/rxftp-html.zip' newdir
   'cp ./rxmath/rxmath.pdf' newdir
   'cp ./rxmath/rxmath-html.zip' newdir
   'cp ./rxsock/rxsock.pdf' newdir
   'cp ./rxsock/rxsock-html.zip' newdir
   'cp ./winextensions/winextensions.pdf' newdir
   'cp ./winextensions/winextensions-html.zip' newdir
   'cp' buildrpt newdir
   end
-- notify the user
call docs_notify email, svnver, buildrpt
-- remove everything
call directory savedir
'rm -rf' tempdir
return


/*----------------------------------------------------------------------------*/
/* docs_notify                                                                */
/*----------------------------------------------------------------------------*/

::routine docs_notify
use arg addressee svnver buildrpt
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
strm~lineOut(buildrpt 'contains the output of the build process so you can')
strm~lineOut('look for error messages in it.')
strm~lineOut('')
strm~lineOut('Thank you for using the ooRexx Build Machine!')
strm~lineOut('The ooRexx Project Team')
strm~lineOut('')
retc = strm~close()
say 'Mailing notification.'
'mail -s "Your ooRexx Build Is Complete" -r buildorx@build.oorexx.org' addressee '<' filename
return


/*----------------------------------------------------------------------------*/
/* interpretermain_notify                                                     */
/*----------------------------------------------------------------------------*/

::routine interpretermain_notify
use arg addressee svnver buildrpt
if addressee = '' then return
filename = './notify.txt'
strm = .stream~new(filename)
retc = strm~open('write')
if retc <> 'READY:' then return ''
strm~lineOut('')
strm~lineOut('Do not reply to this email!')
strm~lineOut('')
strm~lineOut('The ooRexx interpreter build you requested is finished. It can')
strm~lineOut('be downloaded at http://build.oorexx.org/builds/interpreter-main/'svnver'.')
strm~lineOut('')
strm~lineOut('The build may or may not have completed successfully. The file')
strm~lineOut(buildrpt 'contains the output of the build')
strm~lineOut('process so you can look for error messages in it.')
strm~lineOut('')
strm~lineOut('Thank you for using the ooRexx Build Machine!')
strm~lineOut('The ooRexx Project Team')
strm~lineOut('')
retc = strm~close()
say 'Mailing notification.'
if windows() then 'p:\windows\common\blat262\full\blat notify.txt -to' ,
 addressee '-f buildorx@build.oorexx.org -server holmes4.com -u dashley -pw ashley'
else 'mail -s "Your ooRexx Build Is Complete" -r buildorx@build.oorexx.org' addressee '<' filename
return


/*----------------------------------------------------------------------------*/
/* error_notify                                                               */
/*----------------------------------------------------------------------------*/

::routine error_notify
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
say 'Mailing notification.'
if windows() then 'p:\windows\common\blat262\full\blat notify.txt -to' ,
 addressee '-f buildorx@build.oorexx.org -server holmes4.com -u dashley -pw ashley'
else 'mail -s "Your ooRexx Build Is Complete" -r buildorx@build.oorexx.org' addressee '<' filename
return


/*----------------------------------------------------------------------------*/
/* getsvnrevision                                                             */
/*----------------------------------------------------------------------------*/

::routine getsvnrevision
verlocal = './temp.ver'
'svnversion >' verlocal
svnver = getline(verlocal)
if svnver = '' then svnver = 'unknown'
return svnver


/*----------------------------------------------------------------------------*/
/* getline                                                                    */
/*----------------------------------------------------------------------------*/

::routine getline
use arg filename
strm = .stream~new(filename)
retc = strm~open('read')
if retc <> 'READY:' then return ''
line = strm~lineIn()
retc = strm~close()
return line


/*----------------------------------------------------------------------------*/
/* getline                                                                    */
/*----------------------------------------------------------------------------*/

::routine win_getsvnrevision
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


/*----------------------------------------------------------------------------*/
/* windows                                                                    */
/*----------------------------------------------------------------------------*/

::routine windows
parse source osver
if osver~pos('Windows') > 0 then return .true
return .false

