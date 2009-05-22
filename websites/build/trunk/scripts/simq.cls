/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Description: Simple Internet Message Queue class.                          */
/*                                                                            */
/* Copyright (c) 2009 Rexx Language Association. All rights reserved.         */
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


::requires 'socket.cls'
::requires 'streamsocket.cls'

/*----------------------------------------------------------------------------*/
/* Class: simq                                                                */
/* Description: A simple internet message queue class.                        */
/*----------------------------------------------------------------------------*/

::class simq public

::attribute queues private -- the list of queues

::method init
expose queues
queues = .directory~new()
return

::method add_queue
expose queues
use strict arg quename
quename = quename~upper()
-- if the queue already exists do NOT replace it!
if queues~hasindex(quename) = 1 then return .true
queues[quename] = .directory~new()
return .false

::method queue_msg private
expose queues
use strict arg quename, msg
quename = quename~upper()
if queues~hasindex(quename) = 0 then return .true
-- create the iso timestamp in the form of yyyy-mm-ddThh:mm:ss.uuuuuu
ts = date('S', date(), 'N', '-') || 'T' || time('L')
q = queues[quename]
q[ts] = msg
return .false

::method pull_msg private
expose queues
use strict arg quename
quename = quename~upper()
if queues~hasindex(quename) = 0 then return ''
if queues[quename]~items() = 0 then return ''
arr = queues[quename]~allindexes()
arr~sort()
return queues[quename]~remove(arr[1]) -- get the message

::method peek_msg private
expose queues
use strict arg quename
quename = quename~upper()
if queues~hasindex(quename) = 0 then return ''
if queues[quename]~items() = 0 then return ''
arr = queues[quename]~allindexes()
arr~sort()
que = queues[quename]
return que[arr[1]] -- get the message

::method items private
expose queues
use strict arg quename
quename = quename~upper()
if queues~hasindex(quename) = 0 then return ''
return queues[quename]~items()

::method start_server
use strict arg host, port
-- get a new server socket
s = .socket~new()
if s = 0 then do
   say 'Error' s~errno() 'creating socket.'
   return
   end
-- set the server socket to reuse the addresses assigned to it
retc = s~setoption('SO_REUSEADDR', 1)
if retc = -1 then do
   say 'Error' s~errno() 'setting socket option'
   return
   end
-- bind the server socket to an address/port
addr = .inetaddress~new(host, port)
retc = s~bind(addr)
if retc <> 0 then do
   say 'Error' s~errno() 'binding socket.'
   return
   end
-- mark it as a listening socket
retc = s~listen(3)
say 'Server starting'
stop = .false
do while \stop
   -- accept a client connection socket
   cs = s~accept()
   if cs = .nil then do
      iterate
      end
   -- turn it into an open streamsocket
   css = .StreamSocket~new(cs)
   -- receive the command from the client
   msg = css~linein()
   say date() time() msg
   -- process the command
   parse var msg cmd qname msg
   cmd = cmd~upper()
   select
      when cmd = 'PULL' then cs~send(self~pull_msg(qname))
      when cmd = 'PEEK' then cs~send(self~peek_msg(qname))
      when cmd = 'QUEUE' then cs~send(self~queue_msg(qname, msg))
      when cmd = 'ITEMS' then cs~send(self~items(qname))
      when cmd = 'STOP' then do
         stop = .true
         css~lineout(.false)
         end
      otherwise css~lineout('-1')
      end
   -- close the client connection socket
   css~close()
   end
-- close the server socket
s~close()
return

