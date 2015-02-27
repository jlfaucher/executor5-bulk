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


::class svcdefault public subclass linuxsa

::attribute svclist

::method init
expose svclist
use strict arg self~user, self~system, self~debug = .false
svclist = .array~new()
if .local~sysdist = 'FC' | .local~sysdist = 'RH' then do
   svclist~append(.service~new('apmd', 'disable'))
   svclist~append(.service~new('auditd', 'configure'))
   svclist~append(.service~new('autofs', 'disable'))
   svclist~append(.service~new('cpuspeed', 'enable'))
   svclist~append(.service~new('crond', 'configure'))
   svclist~append(.service~new('firstboot', 'disable'))
   svclist~append(.service~new('gpm', 'disable'))
   svclist~append(.service~new('hidd', 'disable'))
   svclist~append(.service~new('ip6tables', 'configure'))
   svclist~append(.service~new('iptables', 'configure'))
   svclist~append(.service~new('irqbalance', 'enable'))
   svclist~append(.service~new('netfs', 'disable'))
   svclist~append(.service~new('network', 'enable'))
   svclist~append(.service~new('nfslock', 'disable'))
   svclist~append(.service~new('portmap', 'disable'))
   svclist~append(.service~new('readahead_early', 'disable'))
   svclist~append(.service~new('readahead_later', 'disable'))
   svclist~append(.service~new('restorecond', 'enable'))
   svclist~append(.service~new('rhnsd', 'disable'))
   svclist~append(.service~new('rpcgssd', 'disable'))
   svclist~append(.service~new('rpcidmapd', 'disable'))
   svclist~append(.service~new('smartd', 'enable'))
   svclist~append(.service~new('rsyslog', 'configure'))
   svclist~append(.service~new('xfs', 'disable'))
   svclist~append(.service~new('yum-updatesd', 'disable'))
   end
else if .local~sysdist = 'SU' then do
   svclist~append(.service~new('SuSEfirewall2_init', 'enable'))
   svclist~append(.service~new('SuSEfirewall2_setup', 'enable'))
   svclist~append(.service~new('a11y', 'disable'))
   svclist~append(.service~new('arpwatch', 'disable'))
   svclist~append(.service~new('auditd', 'configure'))
   svclist~append(.service~new('autofs', 'disable'))
   svclist~append(.service~new('avahi-dnsconfd', 'disable'))
   svclist~append(.service~new('bgpd', 'disable'))
   svclist~append(.service~new('bluetooth-coldplug', 'disable'))
   svclist~append(.service~new('brld', 'disable'))
   svclist~append(.service~new('cron', 'configure'))
   svclist~append(.service~new('dbus', 'disable'))
   svclist~append(.service~new('dnsmasq', 'disable'))
   svclist~append(.service~new('earlysyslog', 'enable'))
   svclist~append(.service~new('earlyxdm', 'enable'))
   svclist~append(.service~new('fbset', 'enable'))
   svclist~append(.service~new('gpm', 'disable'))
   svclist~append(.service~new('irq_balancer', 'enable'))
   svclist~append(.service~new('java.binfmt_misc', 'enable'))
   svclist~append(.service~new('kbd', 'enable'))
   svclist~append(.service~new('kexec', 'disable'))
   svclist~append(.service~new('lirc', 'disable'))
   svclist~append(.service~new('mdadmd', 'disable'))
   svclist~append(.service~new('multipathd', 'disable'))
   svclist~append(.service~new('nagios', 'disable'))
   svclist~append(.service~new('network-remotefs', 'enable'))
   svclist~append(.service~new('nfs', 'enable'))
   svclist~append(.service~new('nmb', 'disable'))
   svclist~append(.service~new('nscd', 'enable'))
   svclist~append(.service~new('ntp', 'disable'))
   svclist~append(.service~new('ospf6d', 'disable'))
   svclist~append(.service~new('ospfd', 'disable'))
   svclist~append(.service~new('powerd', 'disable'))
   svclist~append(.service~new('random', 'enable'))
   svclist~append(.service~new('raw', 'disable'))
   svclist~append(.service~new('ripd', 'disable'))
   svclist~append(.service~new('ripngd', 'disable'))
   svclist~append(.service~new('rsyncd', 'disable'))
   svclist~append(.service~new('sbl', 'enable'))
   svclist~append(.service~new('smartd', 'enable'))
   svclist~append(.service~new('smbfs', 'disable'))
   svclist~append(.service~new('smolt', 'disable'))
   svclist~append(.service~new('splash', 'enable'))
   svclist~append(.service~new('splash_early', 'enable'))
   svclist~append(.service~new('stopblktrace', 'enable'))
   svclist~append(.service~new('svnserve', 'disable'))
   svclist~append(.service~new('uuidd', 'disable'))
   svclist~append(.service~new('xfs', 'disable'))
   svclist~append(.service~new('zebra', 'disable'))
   end
else nop
return


::method chkconfig
use strict arg svc, action
if .local~sysdist = 'RH' then do
   retc = self~execute('/usr/sbin/chkconfig --list' svc)
   end
else do
   retc = self~execute('chkconfig --list' svc)
end
arr = self~arr_from_file(self~tfile)
if arr~items = 0 then return 'none'
do line over arr
   if line~pos('error') > 0 then return 'none'
   if line~pos('3:on') > 0 | line~pos('5:on') > 0 then do
      if action = 'configure' then return 'ok'
      else return 'disable'
      end
   else if line~pos('3:off') > 0 & line~pos('5:off') > 0 then do
      if action = 'configure' | action = 'enable' then return 'enable'
      else return 'ok'
      end
   end
return 'none'


::method systemctl
use strict arg svc, action
retc = self~execute('systemctl status' svc'.service')
arr = self~arr_from_file(self~tfile)
do line over arr
   select 
      when line~pos('static') > 0 then return 'static'
      when line~pos('not-found') > 0 & action = 'enable' then return 'install'
      when line~pos('not-found') > 0 & action = 'configure' then return 'install'
      when line~pos('not-found') > 0 & action = 'disable' then return 'none'
      when line~pos('enabled') > 0 & action = 'enable' then return 'ok'
      when line~pos('active') > 0 & action = 'enable' then return 'ok'
      when line~pos('enabled') > 0 & action = 'disable' then return 'disable'
      when line~pos('active') > 0 & action = 'disable' then return 'disable'
      when line~pos('disabled') > 0 & action = 'enable' then return 'enable'
      when line~pos('disabled') > 0 & action = 'disable' then return 'ok'
      when line~pos('enabled') > 0 & action = 'configure' then return 'ok'
      when line~pos('active') > 0 & action = 'configure' then return 'ok'
      when line~pos('disabled') > 0 & action = 'configure' then return 'ensable'
      otherwise nop
      end
   end
if action = 'enable' then return 'install'
return 'unknown'


::method get_current
expose svclist
use strict arg
rptarr = .array~new()
rptarr~append('<listitem>')
rptarr~append('<indexterm><primary>apmd daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>auditd daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>autofs daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>cpuspeed daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>crond daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>firstboot daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>gpm daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>hidd daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>ip6tables daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>iptables daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>irqbalance daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>netfs daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>network daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>nfslock daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>portmap daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>readahead_early daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>readahead_later daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>restorecond daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>rhnsd daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>rpcgssd daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>rpcidmapd daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>smartd daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>rsyslog daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>xfs daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>yum-updatesd daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>SuSEfirewall2_init daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>SuSEfirewall2_setup daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>ally daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>arpwatch daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>avhi-dnsconfd daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>bgpd daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>bluetooth-coldplug daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>brld daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>cron daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>dbus daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>dnsmasq daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>earlysyslog daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>earlyxdm daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>fbset daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>irq_balancer daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>java.binfmt_misc daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>kbd daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>kexec daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>lirc daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>mdadmd daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>multipathd daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>nagios daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>network-remotefs daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>nfs daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>nmb daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>nscd daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>ntp daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>osp6fd daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>ospfd daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>powerd daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>random daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>raw daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>ripd daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>ripngd daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>rsyncd daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>sbl daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>smbfs daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>smolt daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>splash daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>splash_early daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>stopblktrace daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>svnserve daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>uuidd daemon</primary></indexterm>')
rptarr~append('<indexterm><primary>zebra daemon</primary></indexterm>')
rptarr~append('<orderedlist>')
do svc over svclist
   status = self~systemctl(svc~name, svc~action)
   if status = 'none' then status = self~chkconfig(svc~name, svc~action)
   if status = 'none' then iterate
   rptarr~append('<listitem>')
   rptarr~append('<para>Is the' svc~name 'daemon installed/running?</para>')
   if status = 'ok' then do
      rptarr~append('<programlisting>Installed and running</programlisting>')
      rptarr~append(self~msg15)
      end
   else if status = 'static' then do
      rptarr~append('<programlisting>Installed and running as a systemd static service</programlisting>')
      rptarr~append(self~msg15)
      end
   else if status = 'enable' then do
      rptarr~append('<programlisting>Not installed</programlisting>')
      rptarr~append('<note><title>Warning</title>')
      rptarr~append(self~msg16)
      rptarr~append('<para>This service is currently disabled or misconfigured.')
      rptarr~append('It should enabled if at all possible.</para>')
      rptarr~append('</note>')
      end
   else if status = 'disable' then do
      rptarr~append('<programlisting>Installed and running</programlisting>')
      rptarr~append('<note><title>Warning</title>')
      rptarr~append(self~msg16)
      rptarr~append('<para>This service is currently enabled or misconfigured.')
      rptarr~append('It should disabled if at all possible.</para>')
      rptarr~append('</note>')
      end
   else if status = 'install' then do
      rptarr~append('<programlisting>This daemon is not installed</programlisting>')
      rptarr~append('<note><title>Warning</title>')
      rptarr~append(self~msg16)
      rptarr~append('<para>This service should be installed unless it violates local policy.</para>')
      rptarr~append('</note>')
      end
   else do
      rptarr~append('<programlisting>This daemon is in an unknown state</programlisting>')
      rptarr~append('<note><title>Warning</title>')
      rptarr~append(self~msg16)
      rptarr~append('<para>This service is in an unknown state or misconfigured.')
      rptarr~append('It should be checked and proper action taken.</para>')
      rptarr~append('</note>')
      end
   rptarr~append('</listitem>')
   end
rptarr~append('</orderedlist>')
rptarr~append('</listitem>')
return rptarr


::class service private
::attribute name
::attribute action

::method init
expose name action
use strict arg name, action
return

                               
::requires './classes/linuxsa.cls'
             
