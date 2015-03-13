#!/usr/bin/rexx
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


-- Version information
ver = '0.2.0'


-- Get the cmdline args
cmdline = arg(1)
parse var cmdline . '--user=' user .
parse var cmdline . '--system=' system .
parse var cmdline . '--debug=' debug_setting .
if cmdline~pos('--help') > 0 then do
   call help
   return 0
   end
if cmdline~pos('--version') > 0 then do
   call version ver
   return 0
   end

-- Global variables
if debug_setting <> '' then debug = debug_setting
else debug = .false
if user = '' & system <> '' then user = userid()
questions = .array~new()
questions~append(.questions~new(.syslevel, user, system, debug))
questions~append(.questions~new(.sysrunlevel, user, system, debug))
questions~append(.questions~new(.sysgnome, user, system, debug))
questions~append(.questions~new(.syskde, user, system, debug))
questions~append(.questions~new(.syscinnamon, user, system, debug))
questions~append(.questions~new(.sysmate, user, system, debug))
questions~append(.questions~new(.sysxfce, user, system, debug))
questions~append(.questions~new(.sysunity, user, system, debug))
questions~append(.questions~new(.cfggrubpwd, user, system, debug))
questions~append(.questions~new(.cfguserattrib, user, system, debug))
questions~append(.questions~new(.cfgnodev, user, system, debug))
questions~append(.questions~new(.cfgrmmedia, user, system , debug))
questions~append(.questions~new(.cfgdhcp, user, system, debug))
questions~append(.questions~new(.cfgusb, user, system, debug))
questions~append(.questions~new(.cfgquotaexist, user, system, debug))
questions~append(.questions~new(.cfgdump, user, system, debug))
questions~append(.questions~new(.cfgexecshield, user, system, debug))
questions~append(.questions~new(.cfgxdnx, user, system, debug))
questions~append(.questions~new(.cfgconsole, user, system, debug))
questions~append(.questions~new(.cfginteractiveboot, user, system, debug))
questions~append(.questions~new(.cfgbashtimeout, user, system, debug))
questions~append(.questions~new(.cfgtcshtimeout, user, system, debug))
questions~append(.questions~new(.cfggnometimeout, user, system, debug))
questions~append(.questions~new(.cfgetcskeldot, user, system, debug))
questions~append(.questions~new(.cfgvlock, user, system, debug))
questions~append(.questions~new(.cfgaide, user, system, debug))
questions~append(.questions~new(.cfgselinux, user, system, debug))
questions~append(.questions~new(.cfgkernelnetconf, user, system, debug))
questions~append(.questions~new(.cfgwireless, user, system, debug))
questions~append(.questions~new(.cfgipv6, user, system, debug))
questions~append(.questions~new(.cfghostsallow, user, system, debug))
questions~append(.questions~new(.cfghostsdeny, user, system, debug))
questions~append(.questions~new(.cfglogmsgfile, user, system, debug))
questions~append(.questions~new(.cfglogsecfile, user, system, debug))
questions~append(.questions~new(.cfglogmailfile, user, system, debug))
questions~append(.questions~new(.cfglogmsgrotate, user, system, debug))
questions~append(.questions~new(.cfglogsecrotate, user, system, debug))
questions~append(.questions~new(.cfglogmailrotate, user, system, debug))
questions~append(.questions~new(.cfgworldwrite, user, system, debug))
questions~append(.questions~new(.cfgsuidsgid, user, system, debug))
questions~append(.questions~new(.cfgglobalumask, user, system, debug))
questions~append(.questions~new(.cfgrootumask, user, system, debug))
questions~append(.questions~new(.cfgusernetrc, user, system, debug))
questions~append(.questions~new(.cfghosts, user, system, debug))
questions~append(.questions~new(.cfgsingleuser, user, system, debug))
questions~append(.questions~new(.cfgmotd, user, system, debug))
questions~append(.questions~new(.cfgufw, user, system, debug))
questions~append(.questions~new(.svcdefault, user, system, debug))
questions~append(.questions~new(.svcrhnsd, user, system, debug))
questions~append(.questions~new(.svcsyslog, user, system, debug))
questions~append(.questions~new(.svcappamor, user, system, debug))
questions~append(.questions~new(.svciptablesaccept, user, system, debug))
questions~append(.questions~new(.svcxinetd, user, system, debug))
questions~append(.questions~new(.svctelnetd, user, system, debug))
questions~append(.questions~new(.svcrshserver, user, system, debug))
questions~append(.questions~new(.svcypbind, user, system, debug))
questions~append(.questions~new(.svctftpserver, user, system, debug))
questions~append(.questions~new(.svcdhcpd, user, system, debug))
questions~append(.questions~new(.svcntpd, user, system, debug))
questions~append(.questions~new(.svcvsftpd, user, system, debug))
questions~append(.questions~new(.svcftpother, user, system, debug))
questions~append(.questions~new(.svchttpd, user, system, debug))
questions~append(.questions~new(.svcldap, user, system, debug))
questions~append(.questions~new(.svcsetroubleshoot, user, system, debug))
questions~append(.questions~new(.svcmcstranslation, user, system, debug))
questions~append(.questions~new(.svcrouted, user, system, debug))
questions~append(.questions~new(.svctalkserver, user, system, debug))
questions~append(.questions~new(.svcisdn, user, system, debug))
questions~append(.questions~new(.svckdump, user, system, debug))
questions~append(.questions~new(.svckudzu, user, system, debug))
questions~append(.questions~new(.svcmdmonitor, user, system, debug))
questions~append(.questions~new(.svcmicrocode_ctl, user, system, debug))
questions~append(.questions~new(.svcpcscd, user, system, debug))
questions~append(.questions~new(.svcmessagebus, user, system, debug))
questions~append(.questions~new(.svchaldaemon, user, system, debug))
questions~append(.questions~new(.svcbluetooth, user, system, debug))
questions~append(.questions~new(.svcapmd, user, system, debug))
questions~append(.questions~new(.svcapcid, user, system, debug))
questions~append(.questions~new(.svcirda, user, system, debug))
questions~append(.questions~new(.svcanacron, user, system, debug))
questions~append(.questions~new(.svcatd, user, system, debug))
questions~append(.questions~new(.svcssh, user, system, debug))
questions~append(.questions~new(.svcsshprotocol, user, system, debug))
questions~append(.questions~new(.svcsshconfig, user, system, debug))
questions~append(.questions~new(.svcavahi, user, system, debug))
questions~append(.questions~new(.svccups, user, system, debug))
questions~append(.questions~new(.svchplip, user, system, debug))
questions~append(.questions~new(.svcsendmail, user, system, debug))
questions~append(.questions~new(.svcpostfix, user, system, debug))
questions~append(.questions~new(.svcnamed, user, system, debug))
questions~append(.questions~new(.svcbind, user, system, debug))
questions~append(.questions~new(.svcdovecot, user, system, debug))
questions~append(.questions~new(.svcsamba, user, system, debug))
questions~append(.questions~new(.svcsquid, user, system, debug))
questions~append(.questions~new(.svcsnmp, user, system, debug))
questions~append(.questions~new(.svcfirewalld, user, system, debug))
questions~append(.questions~new(.svcvnc, user, system, debug))
questions~append(.questions~new(.svclibvirt, user, system, debug))
questions~append(.questions~new(.svcmariadb, user, system, debug))
questions~append(.questions~new(.svcmysql, user, system, debug))
questions~append(.questions~new(.usrconsoleroot, user, system, debug))
questions~append(.questions~new(.usrwheelroot, user, system, debug))
questions~append(.questions~new(.usrwheelrootaudit, user, system, debug))
questions~append(.questions~new(.usrrootpath, user, system, debug))
questions~append(.questions~new(.usrrootpathperm, user, system, debug))
questions~append(.questions~new(.usrrootaliases, user, system, debug))
questions~append(.questions~new(.usrrootgrpmembers, user, system, debug))
questions~append(.questions~new(.usrsudoersoutput, user, system, debug))
questions~append(.questions~new(.usrsysacctlocked, user, system, debug))
questions~append(.questions~new(.usrnopasswd, user, system, debug))
questions~append(.questions~new(.usrnoshadowpw, user, system, debug))
questions~append(.questions~new(.usrpasswdexpire, user, system, debug))
questions~append(.questions~new(.usrpasswdquality, user, system, debug))
questions~append(.questions~new(.usrpasswdlockout, user, system, debug))
questions~append(.questions~new(.usrpasswdchk, user, system, debug))
questions~append(.questions~new(.usrpasswddictfile, user, system, debug))
questions~append(.questions~new(.usrpasswdlength, user, system, debug))
questions~append(.questions~new(.usrhomeperm, user, system, debug))
questions~append(.questions~new(.usrhomedotperm, user, system, debug))
questions~append(.questions~new(.usruseracctdisable, user, system, debug))
questions~append(.questions~new(.usrgrpdisable, user, system, debug))
questions~append(.questions~new(.usrguestexist, user, system, debug))
questions~append(.questions~new(.usrhomedirnfs, user, system, debug))
questions~append(.questions~new(.usrbadlogins, user, system, debug))
questions~append(.questions~new(.usrrootssh, user, system, debug))
questions~append(.questions~new(.usrremacct, user, system, debug))
questions~append(.questions~new(.usrremgrp, user, system, debug))
questions~append(.questions~new(.usrlastlogin, user, system, debug))
questions~append(.questions~new(.prmpasswd, user, system, debug))
questions~append(.questions~new(.prmetcprofile, user, system, debug))
questions~append(.questions~new(.prmetcfilesystems, user, system, debug))
questions~append(.questions~new(.prmetchosts, user, system, debug))
questions~append(.questions~new(.prmgrub, user, system, debug))
questions~append(.questions~new(.prmxhost, user, system, debug))
questions~append(.questions~new(.prmlogmsg, user, system, debug))
questions~append(.questions~new(.prmlogsec, user, system, debug))
questions~append(.questions~new(.prmlogmail, user, system, debug))
questions~append(.questions~new(.prmtcpipconf, user, system, debug))
questions~append(.questions~new(.prmtcpipbin, user, system, debug))
questions~append(.questions~new(.prmtcpipsbin, user, system, debug))
questions~append(.questions~new(.prmuserprofile, user, system, debug))
questions~append(.questions~new(.prmetccrontab, user, system, debug))
questions~append(.questions~new(.prmetcanacrontab, user, system, debug))
questions~append(.questions~new(.prmrpmyum, user, system, debug))

-- start the clock
start = time('r')

-- Gather all the information
do question over questions -- run all questions
   call get_report question~qclass, question~userid, question~system, question~debug
   end

-- Cleanup
call sysfiledelete .syslevel~tfile

-- report elapsed time
etime = time('e')
say
say 'Elapsed time for this run was' etime 'seconds.'
return 0

::routine get_report
use strict arg cls, user, system, debug
obj = cls~new(user, system, debug)
obj~report(debug)
return


::routine help
use strict arg
say
say 'The linixsa.rex command can be invoked as follows:'
say
say '   linuxsa.rex [--user=userid] [--system=sysnameorip] [--debug=0|1] [--help]'
say
say 'The following describes the valid arguments to the command options:'
say
say ' --user      The userid to use for each transaction. If not specified then the'
say '             current userid will be used.'
say ' --system    The target system for each transaction. If not specified then the'
say '             current system will be used (127.0.0.1). Either a name or an ip'
say '             address may be specified.'
say ' --debug     The only valid values are 0 or 1. The value of 1 turns on debug'
say '             output. 0 is the default.'
say ' --help      Displays this message.'
say ' --version   Displays LinuxSA version number.'
say
say 'Keep in mind that if the current system is not 127.0.0.1 then all transactions'
say 'will be carried out via ssh.'
say
return


::routine version
use strict arg ver
say
say ver
return


::requires './classes/syslevel.cls'
::requires './classes/sysrunlevel.cls'
::requires './classes/sysgnome.cls'
::requires './classes/syskde.cls'
::requires './classes/syscinnamon.cls'
::requires './classes/sysmate.cls'
::requires './classes/sysxfce.cls'
::requires './classes/sysunity.cls'
::requires './classes/cfggrubpwd.cls'
::requires './classes/cfguserattrib.cls'
::requires './classes/cfgnodev.cls'
::requires './classes/cfgrmmedia.cls'
::requires './classes/cfgdhcp.cls'
::requires './classes/cfgusb.cls'
::requires './classes/cfgquotaexist.cls'
::requires './classes/cfgdump.cls'
::requires './classes/cfgexecshield.cls'
::requires './classes/cfgxdnx.cls'
::requires './classes/cfgconsole.cls'
::requires './classes/cfginteractiveboot.cls'
::requires './classes/cfgbashtimeout.cls'
::requires './classes/cfgtcshtimeout.cls'
::requires './classes/cfggnometimeout.cls'
::requires './classes/cfgetcskeldot.cls'
::requires './classes/cfgvlock.cls'
::requires './classes/cfgaide.cls'
::requires './classes/cfgselinux.cls'
::requires './classes/cfgkernelnetconf.cls'
::requires './classes/cfgwireless.cls'
::requires './classes/cfgipv6.cls'
::requires './classes/cfghostsallow.cls'
::requires './classes/cfghostsdeny.cls'
::requires './classes/cfglogmsgfile.cls'
::requires './classes/cfglogsecfile.cls'
::requires './classes/cfglogmailfile.cls'
::requires './classes/cfglogmsgrotate.cls'
::requires './classes/cfglogsecrotate.cls'
::requires './classes/cfglogmailrotate.cls'
::requires './classes/cfgworldwrite.cls'
::requires './classes/cfgsuidsgid.cls'
::requires './classes/cfgglobalumask.cls'
::requires './classes/cfgrootumask.cls'
::requires './classes/cfgusernetrc.cls'
::requires './classes/cfghosts.cls'
::requires './classes/cfgsingleuser.cls'
::requires './classes/cfgmotd.cls'
::requires './classes/cfgufw.cls'
::requires './classes/svcdefault.cls'
::requires './classes/svcrhnsd.cls'
::requires './classes/svcsyslog.cls'
::requires './classes/svcappamor.cls'
::requires './classes/svciptablesaccept.cls'
::requires './classes/svcxinetd.cls'
::requires './classes/svctelnetd.cls'
::requires './classes/svcrshserver.cls'
::requires './classes/svcypbind.cls'
::requires './classes/svctftpserver.cls'
::requires './classes/svcdhcpd.cls'
::requires './classes/svcntpd.cls'
::requires './classes/svcvsftpd.cls'
::requires './classes/svcftpother.cls'
::requires './classes/svchttpd.cls'
::requires './classes/svcldap.cls'
::requires './classes/svcsetroubleshoot.cls'
::requires './classes/svcmcstranslation.cls'
::requires './classes/svcrouted.cls'
::requires './classes/svctalkserver.cls'
::requires './classes/svcisdn.cls'
::requires './classes/svckdump.cls'
::requires './classes/svckudzu.cls'
::requires './classes/svcmdmonitor.cls'
::requires './classes/svcmicrocode_ctl.cls'
::requires './classes/svcpcscd.cls'
::requires './classes/svcmessagebus.cls'
::requires './classes/svchaldaemon.cls'
::requires './classes/svcbluetooth.cls'
::requires './classes/svcapmd.cls'
::requires './classes/svcapcid.cls'
::requires './classes/svcirda.cls'
::requires './classes/svcanacron.cls'
::requires './classes/svcatd.cls'
::requires './classes/svcssh.cls'
::requires './classes/svcsshprotocol.cls'
::requires './classes/svcsshconfig.cls'
::requires './classes/svcavahi.cls'
::requires './classes/svccups.cls'
::requires './classes/svchplip.cls'
::requires './classes/svcsendmail.cls'
::requires './classes/svcpostfix.cls'
::requires './classes/svcnamed.cls'
::requires './classes/svcbind.cls'
::requires './classes/svcdovecot.cls'
::requires './classes/svcsamba.cls'
::requires './classes/svcsquid.cls'
::requires './classes/svcsnmp.cls'
::requires './classes/svcfirewalld.cls'
::requires './classes/svcvnc.cls'
::requires './classes/svclibvirt.cls'
::requires './classes/svcmariadb.cls'
::requires './classes/svcmysql.cls'
::requires './classes/usrconsoleroot.cls'
::requires './classes/usrwheelroot.cls'
::requires './classes/usrwheelrootaudit.cls'
::requires './classes/usrrootpath.cls'
::requires './classes/usrrootpathperm.cls'
::requires './classes/usrrootaliases.cls'
::requires './classes/usrrootgrpmembers.cls'
::requires './classes/usrsudoersoutput.cls'
::requires './classes/usrsysacctlocked.cls'
::requires './classes/usrnopasswd.cls'
::requires './classes/usrnoshadowpw.cls'
::requires './classes/usrpasswdexpire.cls'
::requires './classes/usrpasswdquality.cls'
::requires './classes/usrpasswdlockout.cls'
::requires './classes/usrpasswdchk.cls'
::requires './classes/usrpasswddictfile.cls'
::requires './classes/usrpasswdlength.cls'
::requires './classes/usrhomeperm.cls'
::requires './classes/usrhomedotperm.cls'
::requires './classes/usruseracctdisable.cls'
::requires './classes/usrgrpdisable.cls'
::requires './classes/usrguestexist.cls'
::requires './classes/usrhomedirnfs.cls'
::requires './classes/usrbadlogins.cls'
::requires './classes/usrrootssh.cls'
::requires './classes/usrremacct.cls'
::requires './classes/usrremgrp.cls'
::requires './classes/usrlastlogin.cls'
::requires './classes/prmpasswd.cls'
::requires './classes/prmetcprofile.cls'
::requires './classes/prmetcfilesystems.cls'
::requires './classes/prmetchosts.cls'
::requires './classes/prmgrub.cls'
::requires './classes/prmxhost.cls'
::requires './classes/prmlogmsg.cls'
::requires './classes/prmlogsec.cls'
::requires './classes/prmlogmail.cls'
::requires './classes/prmtcpipconf.cls'
::requires './classes/prmtcpipbin.cls'
::requires './classes/prmtcpipsbin.cls'
::requires './classes/prmuserprofile.cls'
::requires './classes/prmetccrontab.cls'
::requires './classes/prmetcanacrontab.cls'
::requires './classes/prmrpmyum.cls'

::class questions
::attribute debug
::attribute qclass
::attribute userid
::attribute system

::method init
expose qclass userid system debug
use strict arg qclass, userid, system, debug = .false
return

