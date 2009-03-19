/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2008-2009 Rexx Language Association. All rights reserved.    */
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

/**
 *  This is the Rexx program code used by both the lessSimpleRW and the ???
 *  example programs.  These examples show how to use the new C++ native APIs
 *  to create and work with the ooRexx interpreter from C++.
 *
 *  This file contains two class definitions.  A worker class that has methods
 *  to read and write from a supplied buffer and a fifo buffer class that is
 *  used to supply a buffer for the worker class.  The RW in most of the file
 *  names for these examples comes from reader / writer.
 *
 *  lessSimple.rex is based on simpleRW.rex and is essentially the same, with
 *  the exception that the executable code to create two worker objects and
 *  start them working is moved into a ::routine.
 *
 *  Since these examples are intended for the C / C++ programmer interested in
 *  using the C++ native APIs, most of the comment is in the C++ files.
 */

   -- info of interpreter instance running, maybe changed by C++ nutshell
.local~interpreter.info="[# 0]"

::routine startWorkers
a=.worker~new     -- used to read & write messages to buffer
b=.worker~new     -- used to write messages to buffer

.local~buffer=.fifo~new    -- buffer that gets shared

a~write(.buffer, "from_a", 5)  -- write 5 messages
b~write(.buffer, "FROM_B", 6)  -- write 6 messages

   -- how much got written asynchroneously so far?
say .interpreter.info "... 'a' has written" a~i "messages so far"
say .interpreter.info "... 'b' has written" b~i "messages so far"

   -- let the main program sleep a little bit
sleepTime=.001
say .interpreter.info "sleeping" sleepTime "seconds"
call sysSleep sleepTime

   -- how much got written asynchroneously so far?
say .interpreter.info "... 'a' has written" a~i "messages so far"
say .interpreter.info "... 'b' has written" b~i "messages so far"

   -- now read asynchroneously from the buffer
say .interpreter.info "... now starting to read from buffer"
a~read(.buffer)                -- read all messages from buffer
say .interpreter.info "--> main program finished @" .DateTime~new~string


/* Read and write messages from/to given buffer asynchroneously */
::class Worker
::method init class
  expose activeWriter
  activeWriter=0  -- number of active writers

::attribute activeWriter class

::method init
  expose i
  i=0          -- initialize variable

::attribute i get unguarded

::method write
  expose i
  use arg buffer, msg, repetitions
  self~class~activeWriter+=1    -- increase counter
  REPLY        -- start asynchroneous execution
  do i=1 to repetitions
    buffer~write(i msg~left(8) "@" .DateTime~new~string)
    call sysSleep random(1, 9)/1000
  end

  buffer~write("--> writing message ["msg"] finished @" .DateTime~new~string)
  self~class~activeWriter-=1    -- decrease counter

::method read unguarded
  use arg buffer
  REPLY        -- start asynchroneous execution
  do while buffer~items>0 | self~class~activeWriter>0  for 50
    msg=buffer~read
    if msg == .nil then
      call sysSleep .001
    else
      say .interpreter.info "just read from buffer: ["|| buffer~identityHash"] message: ["msg"]"
  end
  say .interpreter.info  "--> read(): finished @" .DateTime~new~string



/* FIFO buffer, backed by a Rexx .Queue */
::class FIFO

::method init
  expose buffer
  buffer=.queue~new

::method write
  expose buffer
  use arg tmp
  buffer~queue(tmp)

::method read
  expose buffer
  return buffer~pull

::method items
  expose buffer
  return buffer~items
