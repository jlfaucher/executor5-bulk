#!/usr/bin/rexx
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Description: This is the build daemon for the docs KVM huest.              */
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
/*----------------------------------------------------------------------------*/


-- initialize our global variables
txnserver = '192.168.0.104'
txnport = 15776
qname = 'docs'
lockfile = '/tmp/ooRexxDocsBuildLockFile'
home = '/home/dashley'

call directory home

if SysIsFile(lockfile) = 1 then return
else call touch lockfile

s = .streamsocket~new(txnserver, txnport)
retc = s~open()
if retc <> 'READY:'then do
   call log 'Error' retc 'connecting to' txnserver 'server stream.'
   call SysFileDelete lockfile
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
      call log 'Error' retc 'opening queue at' txnserver 'server.'
      call SysFileDelete lockfile
      return
      end
   retc = s~lineout('pull' qname)
   request = s~linein()
   s~close()
   parse var request timestamp email .
   call build_docs email
   end

call SysFileDelete lockfile
return


::requires 'streamsocket.cls'


/*----------------------------------------------------------------------------*/
/* build_docs                                                                 */
/*----------------------------------------------------------------------------*/

::routine build_docs
use strict arg email
call log 'Starting build.'
buildrpt = 'docs-buildrpt.txt'
targetdir = '/imports/builds/docs'
savedir = directory()
-- create temp dir and checkout the source
tempdir = './builddocs'
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
else call log 'This was a duplicate build request.'
-- notify the user
call docs_notify email, svnver, buildrpt
-- remove everything
call directory savedir
'rm -rf' tempdir
call log 'Finished build.'
return


/*----------------------------------------------------------------------------*/
/* docs_notify                                                                */
/*----------------------------------------------------------------------------*/

::routine docs_notify
use arg addressee, svnver, buildrpt
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
call log 'Mailing notification.'
'mail -s "Your ooRexx Build Is Complete" -r noreply@build.oorexx.org' addressee '<' filename
'rm' filename
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
/* touch                                                                      */
/*----------------------------------------------------------------------------*/

::routine touch
use strict arg fname
strm = .stream~new(fname)
strm~open('write replace')
strm~close()
return


/*----------------------------------------------------------------------------*/
/* log                                                                        */
/*----------------------------------------------------------------------------*/

::routine log
-- log messages
use strict arg msg
strm = .stream~new('/home/dashley/BuildDocs.log')
strm~open('write append')
msg = date('S') time('N') msg
say msg
strm~lineout(msg)
strm~close()
return

