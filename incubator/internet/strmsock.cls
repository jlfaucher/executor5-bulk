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

::class Socket public

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Socket                                                              */
/*        Class methods                                                       */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Method: gethostbyaddr                                                      */
/* Description: get a host name by address                                    */
/*----------------------------------------------------------------------------*/

::method gethostbyaddr class
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
retc = sockgethostbyaddr(arg(1), 'localhost.!')
return localhost.!alias.1

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Socket                                                              */
/*        Private methods                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::method s        attribute private -- the socket
::method rcvbuf   attribute private -- the recieve buffer

/*----------------------------------------------------------------------------*/
/* Method: open                                                               */
/* Description: open the socket                                               */
/*----------------------------------------------------------------------------*/

::method open private
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 2 then raise syntax 93.902 array (2)
use arg hostname, port
self~s = socksocket(self~domain, self~type, self~protocol);
if self~s = -1 then do
   self~errno = socksock_errno()
   return .true
   end
/* convert the remote host name to an ip addr */
retc = SockGetHostByName(hostname, 'hostinfo.!')
if retc = 1 then ipaddr = hostinfo.!addr
else ipaddr = hostname
/* create the address stem vars for the remote host command port */
addr.!family = self~domain
addr.!port = port
addr.!addr = ipaddr
/* connect to the smtp server */
retc = SockConnect(self~s, 'addr.!')
if retc = -1 then do
   self~errno = socksock_errno()
   call SockShutDown self~s, 2
   call SockClose self~s
   self~s = -1
   return .true
   end
self~errno = ''
return .false

/*----------------------------------------------------------------------------*/
/* Method: Write                                                              */
/* Description: write a message or block of bytes to the socket               */
/*----------------------------------------------------------------------------*/

::method write private
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
use arg msg
if self~s = -1 then return 0
retc = socksend(self~s, msg)
if retc = -1 then do
   self~errno = socksock_errno()
   raise notready array('Socket') return (-1)
   end
else self~errno = ''
return retc -- return the number of bytes written

/*----------------------------------------------------------------------------*/
/* Method: Read                                                               */
/* Description: read a message or block of bytes from the socket (may return  */
/*              a msg smaller than the requested bufsize)                     */
/*----------------------------------------------------------------------------*/

::method read private
if arg() > 0 then raise syntax 93.902 array (0)
if self~s = -1 then return
retc = sockrecv(self~s, 'x', self~bufsize)
if retc = -1 then do
   self~errno = socksock_errno()
   raise notready array('Socket') return ''
   end
else self~errno = ''
if retc = 0 then return ''
return x

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Socket                                                              */
/*        Public methods                                                      */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::method errno    attribute -- the last error string or number
::method type     attribute -- the socket type
::method domain   attribute -- the socket domain
::method protocol attribute -- the socket protocol
::method bufsize  attribute -- read buffer size

/*----------------------------------------------------------------------------*/
/* Method: init                                                               */
/* Description: instance initialization                                       */
/* Arguments:                                                                 */
/*    new(s) - the specified socket number will be used                       */
/*    new(host, port) - a new socket will be opened                           */
/*----------------------------------------------------------------------------*/

::method init
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 2 then raise syntax 93.902 array (2)
-- set defaults
self~domain = 'AF_INET'
self~type = 'SOCK_STREAM'
self~protocol = 0
self~bufsize = 4096
-- if the mutablebuffer class has the POS method then use it
if .mutablebuffer~hasmethod('POS') then self~rcvbuf = .mutablebuffer~new('', self~bufsize)
else self~rcvbuf = ''
select
   when arg() = 1 then self~s = arg(1)
   when arg() = 2 then if self~open(arg(1), arg(2)) = .true then self~s = -1
   otherwise nop
   end
return

/*----------------------------------------------------------------------------*/
/* Method: uninit                                                             */
/* Description: instance removal                                              */
/*----------------------------------------------------------------------------*/

::method uninit
if arg() > 0 then raise syntax 93.902 array (0)
self~close
return

/*----------------------------------------------------------------------------*/
/* Method: Close                                                              */
/* Description: close the socket                                              */
/*----------------------------------------------------------------------------*/

::method close
if arg() > 0 then raise syntax 93.902 array (0)
if self~s = -1 then return
retc = sockclose(self~s)
if retc = -1 then do
   self~errno = socksock_errno()
   self~s = -1
   end
else self~errno = ''
return

/*----------------------------------------------------------------------------*/
/* Method: SetReadTimeout                                                     */
/* Description: set the socket read timeout                                   */
/*----------------------------------------------------------------------------*/

::method setreadtimeout
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
use arg timeout
if self~s = -1 then return
retc = socksetsockopt(self~s, 'SOL_SOCKET', 'SO_RCVTIMEO', timeout)
if retc = -1 then self~errno = socksock_errno()
else self~errno = ''
return

/*----------------------------------------------------------------------------*/
/* Method: CharIn                                                             */
/* Description: read a character                                              */
/*----------------------------------------------------------------------------*/

::method charin
if arg() > 2 then raise syntax 93.902 array (2)
use arg start, len -- we ignore start since this is not a persistent stream
if len = 'LEN' then len = 1
do while self~rcvbuf~length < len
   self~rcvbuf = self~rcvbuf || self~read()
   end
x = self~rcvbuf~substr(1, len)
self~rcvbuf = self~rcvbuf~substr(len + 1)
return x

/*----------------------------------------------------------------------------*/
/* Method: CharOut                                                            */
/* Description: write a character                                             */
/*----------------------------------------------------------------------------*/

::method charout
if arg() > 2 then raise syntax 93.902 array (2)
use arg str, start -- we ignore start since this is not a persistent stream
retc = self~write(str)
if retc = 0 then return 1
return 0

/*----------------------------------------------------------------------------*/
/* Method: Chars                                                              */
/* Description: return the remaining number of characters                     */
/*----------------------------------------------------------------------------*/

::method chars
if arg() > 0 then raise syntax 93.902 array (0)
-- since this is not a persistent stream we always return 1
-- (unless it is not open)
if self~s = -1 then return 0
return 1

/*----------------------------------------------------------------------------*/
/* Method: LineIn                                                             */
/* Description: read a line                                                   */
/*----------------------------------------------------------------------------*/

::method linein
if arg() > 2 then raise syntax 93.902 array (2)
use arg line, cnt -- we ignore line since this is not a persistent stream
if cnt = 'CNT' then cnt = 1
if cnt = 0 then return ''
if cnt > 1 then raise syntax 93.908 array(2, 1)
do while self~rcvbuf~pos('0D0A'x) = 0
   self~rcvbuf = self~rcvbuf || self~read()
   end
x = self~rcvbuf~substr(1, self~rcvbuf~pos('0D0A'x) - 1) -- strip the CRLF
self~rcvbuf = self~rcvbuf~substr(self~rcvbuf~pos('0D0A'x) + 2)
return x

/*----------------------------------------------------------------------------*/
/* Method: LineOut                                                            */
/* Description: write a line                                                  */
/*----------------------------------------------------------------------------*/

::method lineout
if arg() > 2 then raise syntax 93.902 array (2)
use arg str, line -- we ignore line since this is not a persistent stream
retc = self~write(str || '0D0A'x)
if retc = 0 then return 1
return 0

/*----------------------------------------------------------------------------*/
/* Method: Lines                                                              */
/* Description: return the remaining number of lines                          */
/*----------------------------------------------------------------------------*/

::method lines
if arg() > 1 then raise syntax 93.902 array (1)
use arg opt -- we ignore opt since this is not a persistent stream
-- since this is not a persistent stream we always return 1
-- (unless it is not open)
if self~s = -1 then return 0
return 1

/*----------------------------------------------------------------------------*/
/* Method: RemoteAddress                                                      */
/* Description: return the remote IP address of the socket                    */
/*----------------------------------------------------------------------------*/

::method remoteaddress
if SockGetPeerName(self~s, "socket.") = -1 then do
   self~errno = socksock_errno()
   return ''
   end
return socket.addr

/*----------------------------------------------------------------------------*/
/* Method: RemoteHostName                                                     */
/* Description: return the remote host name                                   */
/*----------------------------------------------------------------------------*/

::method remotehostname
if SockGetPeerName(self~s, "socket.") =-1 then do
   self~errno = socksock_errno()
   return ''
   end
if SockGetHostByAddr(socket.addr, "socket.") = 0 then do
   self~errno = socksock_errno()
   return ''
   end
return socket.name

/*----------------------------------------------------------------------------*/
/* Method: RemotePort                                                         */
/* Description: return the remote port                                        */
/*----------------------------------------------------------------------------*/

::method remoteport
if SockGetPeerName(self~s, "socket.") =-1 then do
   self~errno = socksock_errno()
   return ''
   end
return socket.port

/*----------------------------------------------------------------------------*/
/* Method: LocalAddress                                                       */
/* Description: return the local address                                      */
/*----------------------------------------------------------------------------*/

::method localaddress
if SockGetSockName(self~s, "socket.") =-1 then do
   self~errno = socksock_errno()
   return ''
   end
return socket.addr

/*----------------------------------------------------------------------------*/
/* Method: LocalHostName                                                      */
/* Description: return the local host name                                    */
/*----------------------------------------------------------------------------*/

::method localhostname
if SockGetSockName(self~s, "socket.") =-1 then do
   self~errno = socksock_errno()
   return ''
   end
if SockGetHostByAddr(socket.addr, "socket.")=0 then do
   self~errno = socksock_errno()
   return ''
   end
return socket.name

/*----------------------------------------------------------------------------*/
/* Method: LocalPort                                                          */
/* Description: return the local port                                         */
/*----------------------------------------------------------------------------*/

::method localport
if SockGetSockName(self~s, "socket.")=-1 then do
   self~errno = socksock_errno()
   return ''
   end
return socket.port

/*----------------------------------------------------------------------------*/
/* Method: WaitForReceive                                                     */
/* Description: Waits for a receive to be successful,                         */
/*              Returns "1", if socket has become available, "0" if not, "-1" */
/*----------------------------------------------------------------------------*/

::method WaitForReceive unguarded
if arg() > 1 then raise syntax 93.902 array (1)
if args() > 0 then use arg timeout
else timeout = 0
r.0 = 1
r.1 = self~s
retc = SockSelect("R.", "", "", timeout)
if retc < 0 then self~errno = socksock_errno()
return retc

/*----------------------------------------------------------------------------*/
/* Method: WaitForSend                                                        */
/* Description: Waits for a send to be successful,                            */
/*              Returns "1", if socket has become available, "0" if not, "-1" */
/*----------------------------------------------------------------------------*/

::method WaitForSend unguarded
if arg() > 1 then raise syntax 93.902 array (1)
if args() > 0 then use arg timeout
else timeout = 0
s.0 = 1
s.1 = self~s
retc = SockSelect("", "S.", "", timeout)
if retc < 0 then self~errno = socksock_errno()
return retc

/* Use SockSelect() to wait for exception on socket, optionally supply a timeout
   value in whole seconds. If no timeout is given the invocation will block.
   Returns "1", if socket has become available, "0" if not, "-1" if an error has occurred.
*/
/*----------------------------------------------------------------------------*/
/* Method: WaitForException                                                   */
/* Description: Waits for a exception on a socket.                            */
/*              Returns "1", if socket has become available, "0" if not, "-1" */
/*----------------------------------------------------------------------------*/

::method WaitForException     unguarded
if arg() > 1 then raise syntax 93.902 array (1)
if args() > 0 then use arg timeout
else timeout = 0
e.0 = 1
e.1 = self~s
retc = SockSelect("", "", "E.", timeout)
if retc < 0 then self~errno = socksock_errno()
return retc


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: SRVRSTRMSOCK - a stream sockets server                              */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class ServerSocket public

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: ServerSocket                                                        */
/*        Private methods                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::method ss       attribute private -- the server socket
::method port     attribute private -- the server port
::method backlog  attribute private -- the listen queue size

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: ServerSocket                                                        */
/*        Public methods                                                      */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::method errno    attribute -- the last error string or number

/*----------------------------------------------------------------------------*/
/* Method: init                                                               */
/* Description: instance initialization                                       */
/* Arguments:                                                                 */
/*    new(s) - the local port to bind to                                      */
/*    new(port, backlog) - port to bind to and the backlog to use             */
/*----------------------------------------------------------------------------*/

::method init
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 2 then raise syntax 93.902 array (2)
self~backlog = 20
select
   when arg() = 1 then do
       port = arg(1)
      end
   when arg() = 2 then do
      port = arg(1)
      self~backlog = arg(2)
      end
   otherwise nop
   end
self~ss = socksocket(self~domain, self~type, self~protocol);
if self~ss = -1 then do
   self~errno = socksock_errno()
   return
   end
retc = socksetsockopt(self~ss, 'SOL_SOCKET', 'SO_REUSRADDR')
addr.!family = self~domain
addr.!port = self~port
addr.!addr = 'INADDR_ANY'
retc = sockbind(self~ss, 'addr')
if retc = -1 then do
   self~errno = socksock_errno()
   self~ss = -1
   return
   end
retc = socklisten(self~ss, self~backlog)
if retc = -1 then do
   self~errno = socksock_errno()
   self~ss = -1
   end
self~errno = ''
return

/*----------------------------------------------------------------------------*/
/* Method: uninit                                                             */
/* Description: instance removal                                              */
/*----------------------------------------------------------------------------*/

::method uninit
if arg() > 0 then raise syntax 93.902 array (0)
self~close
return

/*----------------------------------------------------------------------------*/
/* Method: Accept                                                             */
/* Description: accept a connection                                           */
/*----------------------------------------------------------------------------*/

::method accept unguarded
if arg() > 0 then raise syntax 93.902 array (0)
s = sockaccept(self~ss);
if retc = -1 then do
   self~errno = socksock_errno()
   s = -1
   end
else self~errno = ''
return .socket~new(s)

/*----------------------------------------------------------------------------*/
/* Method: Close                                                              */
/* Description: close the server socket                                       */
/*----------------------------------------------------------------------------*/

::method close
if arg() > 0 then raise syntax 93.902 array (0)
if self~ss = -1 then return
retc = sockclose(self~ss)
if retc = -1 then self~errno = socksock_errno()
else do
   self~errno = ''
   self~ss = -1
   end
return

