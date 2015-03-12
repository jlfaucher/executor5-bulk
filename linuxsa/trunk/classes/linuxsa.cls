/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2015-2015 Rexx Language Association. All rights reserved.    */
/*                                                                            */
/* This program and the accompanying materials are made available under       */
/* the terms of the Common Public License v1.0 which accompanies this         */
/* distribution. A copy is also available at the following address:           */
/* http://www.oorexx.org/license.html                                         */
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
/*----------------------------------------------------------------------------*/


::class linuxsa public

::attribute debug   -- debug flag
::attribute tests   -- array of test methods
::attribute user    -- ssh user name
::attribute system  -- ssh system name/ip address

::constant tfile './LinuxSecurity.tmp.txt'  -- temp file used for command output

-- Best practice values
::constant bp_perm1 '-rwxr-xr-x'
::constant bp_perm2 '-rwsr-xr-x'
::constant bp_perm3 '-rw-------'
::constant bp_perm4 '-rw-r--r--'
::constant bp_perm5 '-r--------'
::constant bp_perm6 '-rw-rw-r--'
::constant bp_owner 'root'
::constant bp_grp   'root'

-- Messages for the report
::constant msg1   '<para>The file could not be located or is not accessible.</para>'
::constant msg2   '<para>This file meets best practice requirements. This recommendation may or may not agree with your security policy.</para>'
::constant msg2p  '<para>These files/directories meet best practice requirements.</para>' 
::constant msg3   '<para>The permissions for this file/directory do not meet best practice requirements.</para>'
::constant msg3p  '<para>The permissions for these files/directories do not meet best practice requirements. This recommendation may or may not agree with your security policy.</para>'
::constant msg4   '<para>The owner for this file/directory does not meet best practice requirements. This recommendation may or may not agree with your security policy.</para>'
::constant msg5   '<para>The group for this file/directory does not meet best practice requirements. This recommendation may or may not agree with your security policy.</para>'
::constant msg6   '<para>This configuration option does not meet best practice requirements. This recommendation may or may not agree with your security policy.</para>'
::constant msg7   '<para>This configuration option meets best practice requirements.</para>'
::constant msg8   '<para>This meets best practice requirements.</para>'
::constant msg9   '<para>This file does not meet best practice requirements. This recommendation may or may not agree with your security policy.</para>'
::constant msg9p  '<para>These files do not meet best practice requirements. This recommendation may or may not agree with your security policy.</para>'
::constant msg10  '<para>This configuration does not meet best practice requirements. This recommendation may or may not agree with your security policy.</para>'
::constant msg11  '<para>The results of this test are not valid for SuSE distributions.</para>'
::constant msg12  '<para>The results of this test are not valid for Red Hat/Fedora distributions.</para>'
::constant msg13  '<para>The results of this test are not valid for Mandrake distributions.</para>'
::constant msg14  '<para>This test is not valid if not run as the root user.</para>'
::constant msg15  '<para>This service configuration meets best practice requirements.</para>'
::constant msg16  '<para>This service configuration does not meet best practice requirements. This recommendation may or may not agree with your security policy.</para>'
::constant msg17  '<para>The results of this test are not valid for Ubuntu-based distributions.</para>'


/*----------------------------------------------------------------------------*/
/* Execute a (remote) command.                                                */
/*----------------------------------------------------------------------------*/
::method execute
expose user system
use strict arg cmd
'rm -f' self~tfile  -- ensure the output file is new
if system = '' | system = '127.0.0.1' then do
   cmd '2> 1 >' self~tfile
   end
else do
   'ssh' user'@'system '"'cmd'" 2>1 >' self~tfile
   if rc = 127 | rc = 255 then do
      say
      say 'Error: Command failure:' cmd
      say '       SSH may not be installed or sshd may not be running on the'
      say '       remote system. Terminating LinuxSA.'
      exit 4
      end
   end
return rc


/*----------------------------------------------------------------------------*/
/* Write an array or a single string to a file.                               */
/*----------------------------------------------------------------------------*/
::method arr_to_file
-- note: arr can be an array or a simple string
use strict arg arr, filename
outfile = .stream~new(filename)
outfile~open('write replace')
if arr <> .nil then do
   if arr~isa(.array) then do line over arr
      outfile~lineout(line)
      end
   else outfile~lineout(arr)
   end
outfile~close
return


/*----------------------------------------------------------------------------*/
/* Read an array from a file.                                                 */
/*----------------------------------------------------------------------------*/
::method arr_from_file
use strict arg filename
infile = .stream~new(filename)
infile~open('read')
arr = infile~arrayin()
infile~close
if arr = .nil then arr = .array~new()
return arr


/*----------------------------------------------------------------------------*/
/* Write the report section to a file.                                        */
/*----------------------------------------------------------------------------*/
::method report
arr = self~get_current()
if self~debug = .true then do
   if arr~isa(.array) then do line over arr
      say line
      end
   else do
      say arr
      end
   say '========================================================================='
   end
else do
   self~arr_to_file(arr, './report/en-US/'self~objectname~word(2)'.xml')
   end
return


/*----------------------------------------------------------------------------*/
/* Use both chkconfig and systemctl to see if a service is active/installed.  */
/*----------------------------------------------------------------------------*/
::method chkservice
use strict arg svc
-- first check to see if the service is running (easiest check for this)
retc = self~execute('ps -e | grep' svc)
arr = self~arr_from_file(self~tfile)
status = 'none'
if arr~items > 0 then do
   daemon = arr[1]~word(4)
   if daemon = svc then status = 'enabled'
   end
-- now use alternative methods to find the service status
if status = 'none' then status = self~chkconfig(svc)
-- ###########################################
--
-- TODO:
--
-- When Ubuntu starts supporting systemd then we will need to remove the test
-- for 'UB' in the following statement.
--
-- ###########################################
if status = 'none' & .local~sysdist <> 'UB' then status = self~systemctl(svc)
return status


/*----------------------------------------------------------------------------*/
/* Use chkconfig or dpkg to see if a service is active/installed.             */
/*----------------------------------------------------------------------------*/
::method chkconfig
use strict arg svc
if .local~sysdist = 'RH' then do
   retc = self~execute('/usr/sbin/chkconfig --list' svc)
   arr = self~arr_from_file(self~tfile)
   if arr~items = 0 then return 'none'
   do line over arr
      if line~pos('3:on') > 0 | line~pos('5:on') > 0 then return 'enabled'
      if line~pos('3:off') > 0 & line~pos('5:off') > 0 then return 'disabled'
      end
   end
else if .local~sysdist = 'SU' then do
   retc = self~execute('chkconfig --list' svc)
   arr = self~arr_from_file(self~tfile)
   if arr~items = 0 then return 'none'
   do line over arr
      if line~pos('3:on') > 0 | line~pos('5:on') > 0 then return 'enabled'
      if line~pos('3:off') > 0 & line~pos('5:off') > 0 then return 'disabled'
      end
   end
else do  -- 'UB'
   retc = self~execute('dpkg --list' svc)
   arr = self~arr_from_file(self~tfile)
   do line over arr
      if line~words > 1 & line~word(2) = svc then do
         if line~word(1)~substr(2,1) = 'i' then return 'enabled'
         end
      end
   end
return 'none'


/*----------------------------------------------------------------------------*/
/* Use systemctl to see if a service is active/installed.                     */
/*----------------------------------------------------------------------------*/
::method systemctl
use strict arg svc
retc = self~execute('systemctl status' svc'.service')
arr = self~arr_from_file(self~tfile)
do line over arr
   if line~pos('enabled') > 0 then return 'enabled'
   if line~pos('disabled') > 0 then return 'disabled'
   if line~pos('static') > 0 then return 'static'
   if line~pos('not-found') > 0 then return 'none'
   end
return 'unknown'


/*----------------------------------------------------------------------------*/
/* Use rpm or dpkg to check if a package is installed.                        */
/*----------------------------------------------------------------------------*/
::method rpmdpkg
use strict arg svc
if .local~sysdist = 'UB' then do
   status = 'Not installed'
   retc = self~execute('dpkg --list' svc)
   arr = self~arr_from_file(self~tfile)
   do line over arr
      if line~words > 1 & line~word(2) = svc then do
         if line~word(1)~substr(2,1) = 'i' then status = 'Yes'
         end
      end
   end
else do
   retc = self~execute('rpm -q' svc)
   arr = self~arr_from_file(self~tfile)
   if arr~items = 0 then status = 'Not installed'
   else if arr[1]~pos('not installed') > 0 then status = 'Not installed'
   else status = 'Yes'
   end
return status



