#!/usr/bin/rexx
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Description: Text for orxsock6.cls.                                        */
/*                                                                            */
/* Copyright (c) 2014-2014 Rexx Language Association. All rights reserved.    */
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


-- test client and server
srvr = .server~new('::1', 8083)
-- give the server a chance to get initialized
call syssleep(1)
call client '::1', 8083, 'stop'
-- sleep here so that the server has a chance to clean up the server socket
call syssleep(1)
return

::requires 'orxsock6.cls'

-- the client routine 

::routine client
use strict arg host, port, message
-- get a new socket
s = .socket~new(.socket~PF_INET6, .socket~SOCK_STREAM, 0)
if !s~isa('socket') then do
   say 'Could not create the client socket.'
   return
   end
-- set the server address/port to connection information
addr = .inetaddress~new(host, port, .inetaddress~AF_INET6)
-- connect to the server
retc = s~connect(addr)
if retc = -1 then do
   say 'Could not connect the client to the address.'
   return
   end
-- send the command
retc = s~send(message)
if retc = -1 then do
   say 'Could not send a message on the client socket.'
   return
   end
-- receive the command back
buf = s~recv(4096)
if retc = -1 then do
   say 'Could not receive a message on the client socket.'
   return
   end
-- close the socket
s~close()
if retc = -1 then do
   say 'Could not close the client socket.'
   return
   end
say 'Client was successful'.
return


-- the socket server class

::class server
::method init
use strict arg host, port
-- get a new socket
s = .socket~new(.socket~PF_INET6, .socket~SOCK_STREAM, 0)
if !s~isa('socket') then do
   say 'Could not create the server socket.'
   return
   end
-- set the socket to reuse the addresses assigned to it
s~setsockopt(.socket~SO_REUSEADDR, 1)
-- bind the socket to an address/port
addr = .inetaddress~new(host, port, .inetaddress~AF_INET6)
retc = s~bind(addr)
if retc = -1 then do
   say 'Could not bind server socket to the address.'
   return
   end
-- mark it as a listening socket
retc = s~listen(3)
if retc = -1 then do
   say 'Could not listen on the server socket.'
   return
   end
reply
stop = .false
do while \stop
   -- accept a client connection socket
   cs = s~accept()
   if retc = -1 then do
      say 'Could not listen on the work server socket.'
      return
      end
   if !cs~isa('socket') then do
      say 'Accepted entity is not a socket.'
      return
      end
   -- receive the command from the client
   cmd = cs~recv(4096)
   if retc = -1 then do
      say 'Could not receive on the work server socket.'
      return
      end
   -- echo the command back to the client
   retc = cs~send(cmd)
   if retc = -1 then do
      say 'Could not send on the work server socket.'
      return
      end
   -- close the client connection socket
   retc = cs~close()
   if retc = -1 then do
      say 'Could not close the work server socket.'
      return
      end
   -- if the command was stop then stop the server
   if cmd~upper() = 'STOP' then do
      stop = .true
      end
   end
-- close the socket
retc = s~close()
if retc = -1 then do
   say 'Could not close the server socket.'
   return
   end
say 'Server was successful'.
return

