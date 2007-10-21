/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Description: Simple classes to encalsulate stream sockets.                 */
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
/*----------------------------------------------------------------------------*/


/* initialization */
if rxfuncquery('SockDropFuncs') then do
   call rxfuncadd 'SockLoadFuncs', 'rxsock', 'SockLoadFuncs'
   call SockLoadFuncs 'bypasscopyright'
   end


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Socket - a class for stream sockets                                 */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class 'Socket' public

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Socket                                                              */
/*        Class methods                                                       */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Method: gethostbyaddr                                                      */
/* Description: get a host information by address                             */
/* Arguments:                                                                 */
/*         hostaddr - host ip address                                         */
/*----------------------------------------------------------------------------*/

::method gethostbyaddr class
use strict arg hostaddr
return .HostInfo~new(hostaddr)

/*----------------------------------------------------------------------------*/
/* Method: gethostbyname                                                      */
/* Description: get a host information by name                                */
/* Arguments:                                                                 */
/*         hostname - host name (must be a known DNS entry)                   */
/*----------------------------------------------------------------------------*/

::method gethostbyname class
use strict arg hostname
return .HostInfo~new(hostname)

/*----------------------------------------------------------------------------*/
/* Method: gethostid                                                          */
/* Description: get the local host ip address                                 */
/* Arguments: none                                                            */
/*----------------------------------------------------------------------------*/

::method gethostid class
use strict arg
return SockGetHostID()

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Socket                                                              */
/*        Private methods                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::method s        attribute private -- the socket

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Socket                                                              */
/*        Public methods                                                      */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::method errno attribute

/*----------------------------------------------------------------------------*/
/* Method: accept                                                             */
/* Description: accept a connection                                           */
/* Arguments: none                                                            */
/*----------------------------------------------------------------------------*/

::method accept
expose s errno
use strict arg
-- are we being passes an existing socket?
newsocket = SockAccept(s)
if newsocket = -1 then do
   errno = SockSock_errno()
   return .nil
   end
else errno = ''
return .socket~new(newsocket)

/*----------------------------------------------------------------------------*/
/* Method: bind                                                               */
/* Description: bind a socket to a address/port                               */
/* Arguments:                                                                 */
/*         hostaddr - a host address of class InetAddress                     */
/*----------------------------------------------------------------------------*/

::method bind
expose s errno
use strict arg address
if \address~isA(.InetAddress) then ,
 raise syntax 93.914 array (1, 'an InetAddress', address)
stem. = address~makeStem()
retc = SockBind(s, 'stem.')
if retc = -1 then errno = SockSock_errno()
else errno = ''
return retc

/*----------------------------------------------------------------------------*/
/* Method: close                                                              */
/* Description: shutdown and close a socket                                   */
/* Arguments: none                                                            */
/*----------------------------------------------------------------------------*/

::method close
expose s errno
use strict arg
retc = SockClose(s)
if retc = -1 then errno = SockSock_errno()
else errno = ''
s = -1
return retc

/*----------------------------------------------------------------------------*/
/* Method: connect                                                            */
/* Description: connect a socket to a remote address                          */
/* Arguments:                                                                 */
/*         hostaddr - a host address of class InetAddress                     */
/*----------------------------------------------------------------------------*/

::method connect
expose s errno
use strict arg address
if \address~isA(.InetAddress) then ,
 raise syntax 93.914 array (1, 'an InetAddress', address)
stem. = address~makeStem()
retc = SockConnect(s, 'stem.!')
if retc = -1 then errno = SockSock_errno()
else errno = ''
return retc

/*----------------------------------------------------------------------------*/
/* Method: getOption                                                          */
/* Description: return a socket option                                        */
/* Arguments:                                                                 */
/*         name - the name of an option                                       */
/*----------------------------------------------------------------------------*/

::method getOption
expose s errno
use strict arg name
-- check args
optvals = .array~of('SO_BROADCAST', 'SO_DEBUG', 'SO_DONTROUTE', 'SO_ERROR',,
                    'SO_KEEPALIVE', 'SO_LINGER', 'SO_OOBINLINE', 'SO_RCVBUF',,
                    'SO_RCVLOWAT', 'SO_RCVTIMEO', 'SO_REUSEADDR', 'SO_SNDBUF',,
                    'SO_SNDLOWAT', 'SO_SNDTIMEO', 'SO_TYPE', 'SO_USELOOPBACK')
if optvals~hasItem(name) = .false then ,
 raise syntax 93.914 array (1, optvals~toString, name)
-- get the value
retc = SockGetSendOpt(s, 'SOL_SOCKET', name, 'xxx')
if retc = -1 then do
   errno = SockSock_errno()
   return .nil
   end
else errno = ''
return xxx

/*----------------------------------------------------------------------------*/
/* Method: getPeerName                                                        */
/* Description: get the peer name connected to a socket                       */
/* Arguments: none                                                            */
/*----------------------------------------------------------------------------*/

::method getPeerName
expose s errno
use strict arg
retc = SockGetPeerName(s, cmd, 'stem.')
if retc = -1 then do
   errno = SockSock_errno()
   return .nil
   end
else errno = ''
return .InetAddress~new(stem.!address, stem.!port, stem.!family)

/*----------------------------------------------------------------------------*/
/* Method: getSockName                                                        */
/* Description: get the name of the socket                                    */
/* Arguments: none                                                            */
/*----------------------------------------------------------------------------*/

::method getSockName
expose s errno
use strict arg
retc = SockGetSockName(s, cmd, 'stem.')
if retc = -1 then do
   errno = SockSock_errno()
   return .nil
   end
else errno = ''
return .InetAddress~new(stem.!address, stem.!port, stem.!family)

/*----------------------------------------------------------------------------*/
/* Method: init                                                               */
/* Description: instance initialization                                       */
/* Arguments:                                                                 */
/*         domain   - must be AF_INET                                         */
/*         type     - must be SOCK_STREAM, SOCK_DGRAM or SOCK_RAW             */
/*         protocol - (optional, if not supplied will be set to 0)            */
/*----------------------------------------------------------------------------*/

::method init
expose s errno
-- are we being passed an existing socket?
if arg() = 1 then do
   use strict arg s
   if s~datatype('W') = 0 then raise syntax 93.905 array (1, s)
   if s < 1 then raise syntax 93.907 array (1, s)
   return
   end
-- assume we want a new socket created
use strict arg domain, type, protocol = 0
-- check arguments
domains = .array~of('AF_INET')
if domains~hasItem(domain) = .false then ,
 raise syntax 93.914 array (1, domains~toString, domain)
types = .array~of('SOCK_STREAM', 'SOCK_DGRAM', 'SOCK_RAW')
if types~hasItem(type) = .false then ,
 raise syntax 93.914 array (2, types~toString, type)
protocols = .array~of(0, 'IPPROTO_UDP', 'IPPROTO_TCP')
if protocols~hasItem(protocol) = .false then ,
 raise syntax 93.914 array (3, protocols~toString, protocol)
-- create the socket
s = SockSocket(domain, type, protocol)
if s = -1 then errno = SockSock_errno()
else errno = ''
return

/*----------------------------------------------------------------------------*/
/* Method: ioctl                                                              */
/* Description: perform a special operation on a socket                       */
/* Arguments:                                                                 */
/*         cmd  - the command to send                                         */
/*         data - the command date                                            */
/*----------------------------------------------------------------------------*/

::method ioctl
expose s errno
use strict arg cmd, data
retc = SockIoctl(s, cmd, data)
if retc = -1 then do
   errno = SockSock_errno()
   return .nil
   end
else errno = ''
return retc

/*----------------------------------------------------------------------------*/
/* Method: listen                                                             */
/* Description: listen for connections on a socket                            */
/* Arguments:                                                                 */
/*         backlog - the backlog to use for pending connection requests       */
/*----------------------------------------------------------------------------*/

::method listen
expose s errno
use strict arg backlog
if backlog~datatype('W') = 0 then raise syntax 93.905 array (1, backlog)
if backlog < 1 then raise syntax 93.907 array (1, backlog)
retc = SockListen(s, backlog)
if retc = -1 then errno = SockSock_errno()
else errno = ''
return retc

/*----------------------------------------------------------------------------*/
/* Method: recv                                                               */
/* Description: recieve data on a socket                                      */
/* Arguments:                                                                 */
/*         len - the maximum amount of data to recieve in bytes               */
/*----------------------------------------------------------------------------*/

::method recv
expose s errno
use strict arg len
if len~datatype('W') = 0 then raise syntax 93.905 array (1, len)
if len < 1 then raise syntax 93.907 array (1, len)
retc = SockRecv(s, 'xxx', len)
if retc = -1 then do
   errno = socksock_errno()
   return .nil
   end
errno = ''
if retc = 0 then return ''
return xxx

/*----------------------------------------------------------------------------*/
/* Method: recvFrom                                                           */
/* Description: recieve data on a socket from a specified address             */
/* Arguments:                                                                 */
/*         len      - the maximum amount of data to recieve in bytes          */
/*         addressr - a host address of class InetAddress                     */
/*----------------------------------------------------------------------------*/

::method recvFrom
expose s errno
use strict arg len, address
if len~datatype('W') = 0 then raise syntax 93.905 array (1, len)
if len < 1 then raise syntax 93.907 array (1, len)
if \address~isA(.InetAddress) then ,
 raise syntax 93.914 array (1, 'an InetAddress', address)
stem. = address~makeStem()
retc = SockRecvFrom(s, 'xxx', len, 'stem.')
if retc = -1 then do
   errno = SockSock_errno()
   return .nil
   end
if retc = 0 & xxx~length > 0 then do
   errno = SockSock_errno()
   if xxx~length > 0 then return xxx
   else return ''
   end
else errno = ''
return xxx

/*----------------------------------------------------------------------------*/
/* Method: select                                                             */
/* Description: monitor activity on a set of sockets                          */
/* Arguments:                                                                 */
/*         reads   - an array of sockets                                       */
/*         writes  - an array of sockets                                           */
/*         excepts - an array of sockets                                      */
/*         timeout - timeout in seconds                                       */
/*----------------------------------------------------------------------------*/

::method select
expose errno
use strict arg reads, writes, excepts, timeout
if \reads~isA(.Array) then ,
 raise syntax 93.914 array (1, 'an Array', reads)
if \writes~isA(.Array) then ,
 raise syntax 93.914 array (2, 'an Array', writes)
if \excepts~isA(.Array) then ,
 raise syntax 93.914 array (3, 'an Array', excepts)
reads.0 = reads~items
do i = 1 to reads~items
   reads.i = reads[i]~s
   end
writes.0 = writes~items
do i = 1 to writes~items
   writes.i = writes[i]~s
   end
excepts.0 = excepts~items
do i = 1 to excepts~items
   excepts.i = excepts[i]~s
   end
retc = SockSelect('reads.', 'writes.', 'excepts.', timeout)
if retc = -1 then do
   errno = SockSock_errno()
   return .nil
   end
else errno = ''
return xxx

/*----------------------------------------------------------------------------*/
/* Method: send                                                               */
/* Description: send data on a socket                                         */
/* Arguments:                                                                 */
/*         data - data to send over the socket                                */
/*----------------------------------------------------------------------------*/

::method send
expose s errno
use strict arg data
retc = SockSend(s, data)
if retc = -1 then errno = SockSock_errno()
else errno = ''
return retc

/*----------------------------------------------------------------------------*/
/* Method: setOption                                                          */
/* Description: set a socket option                                           */
/* Arguments:                                                                 */
/*         name - the name of the option to set                               */
/*         val  - the new value for the option                                */
/*----------------------------------------------------------------------------*/

::method setOption
expose s errno
use strict arg name, val
optvals = .array~of('SO_BROADCAST', 'SO_DEBUG', 'SO_DONTROUTE', 'SO_ERROR',,
                    'SO_KEEPALIVE', 'SO_LINGER', 'SO_OOBINLINE', 'SO_RCVBUF',,
                    'SO_RCVLOWAT', 'SO_RCVTIMEO', 'SO_REUSEADDR', 'SO_SNDBUF',,
                    'SO_SNDLOWAT', 'SO_SNDTIMEO', 'SO_TYPE', 'SO_USELOOPBACK')
if optvals~hasItem(name) = .false then ,
 raise syntax 93.914 array (1, optvals~toString, name)
if val~datatype('W') <> 1 & name <> 'SO_LINGER' then raise syntax 93.907 array(2, val)
retc = SockSetSendOpt(s, 'SOL_SOCKET', name, val)
if retc = -1 then do
   errno = SockSock_errno()
   return .nil
   end
else errno = ''
return xxx

/*----------------------------------------------------------------------------*/
/* Method: uninit                                                             */
/* Description: close the socket.                                             */
/*----------------------------------------------------------------------------*/

::method uninit
expose s
if s <> -1 then retc = self~close()
return


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: InetAddress - internet address encapsulation.                       */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class InetAddress public

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: InetAddress                                                         */
/*        Class methods                                                       */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: InetAddress                                                         */
/*        Private methods                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: InetAddress                                                         */
/*        Public methods                                                      */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Method: address                                                            */
/* Description: the InetAddress address                                       */
/* Arguments: none                                                            */
/*----------------------------------------------------------------------------*/

::method address
expose address
return address

/*----------------------------------------------------------------------------*/
/* Method: address=                                                           */
/* Description: the InetAddress address                                       */
/* Arguments:                                                                 */
/*         value - the ip address or host name                                */
/*----------------------------------------------------------------------------*/

::method 'address='
expose address
use strict arg address
if address <> 'INADDR_ANY' then do
   retc = SockGetHostByName(address, 'hostinfo.!')
   if retc = 1 then address = hostinfo.!addr
   end
if address~verify('0123456789.') <> 0 then ,
 raise syntax 93.953 array(1, 'dotted decimal address')
return

/*----------------------------------------------------------------------------*/
/* Method: family                                                             */
/* Description: the InetAddress family                                        */
/* Arguments: none                                                            */
/*----------------------------------------------------------------------------*/

::method family
expose family
return family

/*----------------------------------------------------------------------------*/
/* Method: family=                                                            */
/* Description: the InetAddress family                                        */
/* Arguments:                                                                 */
/*         value - the address family                                         */
/*----------------------------------------------------------------------------*/

::method 'family='
expose family
use strict arg family
families = .array~of('AF_INET')
if families~hasItem(family) = .false then ,
 raise syntax 93.914 array (1, families~toString, family)
return

/*----------------------------------------------------------------------------*/
/* Method: init                                                               */
/* Description: initialization of the InetAddress                             */
/* Arguments:                                                                 */
/*         address - the ip address or hostname                               */
/*         port    - the port to be used                                      */
/*         family  - (optional) the address family to use                     */
/*----------------------------------------------------------------------------*/

::method init
-- use the self methods to get error checking for address, port and family
use strict arg self~address, self~port, self~family = 'AF_INET'
return

/*----------------------------------------------------------------------------*/
/* Method: makeStem                                                           */
/* Description: make a stem out of the InetAddress                            */
/* Arguments: none                                                            */
/*----------------------------------------------------------------------------*/

::method makeStem
expose address family port
use strict arg
stem.!addr = address
stem.!family = family
stem.!port = port
return stem.

/*----------------------------------------------------------------------------*/
/* Method: port                                                               */
/* Description: the InetAddress port                                          */
/* Arguments: none                                                            */
/*----------------------------------------------------------------------------*/

::method port
expose port
return port

/*----------------------------------------------------------------------------*/
/* Method: port=                                                              */
/* Description: the InetAddress port                                          */
/* Arguments:                                                                 */
/*         value - the port number                                            */
/*----------------------------------------------------------------------------*/

::method 'port='
expose port
use strict arg port
if port~datatype('W') <> 1 then raise syntax 93.907 array(1, port)
if port < 1 then raise syntax 93.907 array(1, port)
return


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: HostInfo - encapsulate host information.                            */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class HostInfo public

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: HostInfo                                                            */
/*        Class methods                                                       */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: HostInfo                                                            */
/*        Private methods                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: HostInfo                                                            */
/*        Public methods                                                      */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::method addr      -- an array of internet addresses of the host
::method address   -- the main address of the host
::method alias     -- an array of alias names of the host
::method name      -- the standard host name

/*----------------------------------------------------------------------------*/
/* Method: getInfo                                                            */
/* Description: get the hostinfo                                              */
/* Arguments:                                                                 */
/*         hostname - (optional, required if self~name has no value)          */
/*----------------------------------------------------------------------------*/

::method getInfo
expose address alias addr name
-- initialize our attributes
alias~empty()
address = .''
addr~empty()
-- make sure we have a hostname to use
if self~name = '' or arg() > 0 then use strict arg self~name
-- get the information and set our attributes
if name~verify('0123456789.') = 0 then ,
 retc = SockGetHostByAddr(name, 'hostinfo.!')
else retc = SockGetHostByName(name, 'hostinfo.!')
if retc = -1 then return
address = hostinfo.!addr
do i = 1 to hostinfo.!alias.0
   alias[i] = hostinfo.!alias.i
   end
do i = 1 to hostinfo.!addr.0
   addr[i] = hostinfo.!addr.i
   end
return

/*----------------------------------------------------------------------------*/
/* Method: init                                                               */
/* Description: initialization of the InetAddress                             */
/* Arguments:                                                                 */
/*         address - (optional) the ip address or hostname                    */
/*----------------------------------------------------------------------------*/

::method init
expose address alias addr name
name = ''
alias = .array~new()
address = .''
addr = .array~new()
if arg() = 0 then nop
else do
   use arg strict name
   self~getInfo()
   end
return

/*----------------------------------------------------------------------------*/
/* Method: makeStem                                                           */
/* Description: make a stem out of the HostInfo                               */
/* Arguments: none                                                            */
/*----------------------------------------------------------------------------*/

::method makeStem
expose address alias addr name
use strict arg
stem.!name = name
stem.!addr = address
stem.!addrtype = 'AF_INET'
stem.!addr.0 = addr~items()
do i = 1 to addr~items()
   stem.!addr.i = addr[i]
   end
stem.!alias.0 = alias~items()
do i = 1 to alias~items()
   stem.!alias.i = alias[i]
   end
return stem.

