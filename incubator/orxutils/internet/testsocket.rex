#!/usr/bin/rexx
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Description: Test the socket class.                                        */
/*                                                                            */
/* Copyright (c) 2007-2008 Rexx Language Association. All rights reserved.    */
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


host = '127.0.0.1'
port = 8080
srvr = .server~new(host, port)
call syssleep(1)

call client host, port, 'This is test 1'
call client host, port, 'This is test 2'
call client host, port, 'stop'
return


::requires 'socket.cls'


::routine client
use strict arg host, port, message
-- get a new socket
s = .socket~new()
-- set the server address/port to connection information
addr = .inetaddress~new(host, port)
-- connect to the server
retc = s~connect(addr)
if retc <> 0 then do
   say 'Error' s~errno() 'connecting to server socket.'
   return
   end
-- send the command
retc = s~send(message)
-- receive the command back
say s~recv(4096)
-- close the socket
s~close()
return


-- the server class

::class server
::method init
use strict arg host, port
-- get a new socket
s = .socket~new()
if s = -1 then do
   say 'Error' s~errno() 'creating server socket'
   return
   end
-- set the socket to reuse the addresses assigned to it
retc = s~setoption('SO_REUSEADDR', 1)
if retc = -1 then do
   say 'Error' s~errno() 'setting socket option'
   return
   end
-- bind the socket to an address/port
addr = .inetaddress~new(host, port)
retc = s~bind(addr)
if retc = -1 then do
   say 'Error' s~errno() 'binding socket'
   return
   end
-- mark it as a listening socket
retc = s~listen(3)
if retc = -1 then do
   say 'Error' s~errno() 'making the socket a listening socket'
   return
   end
say 'Server starting'
reply
stop = .false
do while \stop
   -- accept a client connection socket
   cs = s~accept()
   if cs = .nil then do
      say 'Error accepting new socket'
      iterate
      end
   -- receive the command from the client
   cmd = cs~recv(4096)
   -- echo the command back to the client
   cs~send(cmd)
   -- close the client connection socket
   cs~close()
   -- if the command was stop then stop the server
   if cmd~upper() = 'STOP' then do
      stop = .true
      end
   end
-- close the socket
s~close()
return

