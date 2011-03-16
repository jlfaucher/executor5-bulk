/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2011 Rexx Language Association. All rights reserved.         */
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
 * Concurrent tools for ooRexx.
 *
 * Moritz Hoffmann - 110316
 *
 **/


::class THREAD PUBLIC

::method init
  expose interrupt running alive name
  use strict arg name="Unnamed Thread"
  interrupt = .false
  started = .false
  alive = .false

::method process ABSTRACT
::attribute name get

::method start
  expose started
  reply
  started = .true
  self~process
  alive = .false

::method interrupt
  expose interrupt
  interrupt = .true

::method isInterrupted
  expose interrupt
  return interrupt

::method join UNGUARDED
  expose alive
  guard on when alive = .false

::method isAlive
  expose alive
  return alive

::method isStarted
  expose started
  return started

::class MessageThread subclass Thread PUBLIC
::method init
  expose message
  use strict arg message

::method process
  expose message
  message~send

/**
 * A thread pool. Allows to run executable objects using a number of
 * pooled threads.
 */
::class THREADPOOL PUBLIC
::method init
  expose min max threads messages running idle size
  use strict arg min=2, max=5
  size = 1 
  threads = .array~new(max)
  messages = .queue~new
  running = .set~new
  idle = .set~new
  do min
    idle~put(self~createThread)
  end

::method createThread PRIVATE
  expose idle size max
  thread = .ThreadPoolThread~new(self, size)
  thread~start
  size = size + 1
  return thread

::method shutdown
  expose shutdown idle running
  shutdown = .true
  do i over idle
    i~interrupted=.true
  end
  threads = running~makearray
  guard off
  do r over threads
    r~join
  end

::method post
  expose messages
  use strict arg message
  messages~append(message)
  self~wakeThread

::method wakeThread PRIVATE
  expose messages size max idle running
  message = messages~peek
  if message == .nil then
    return

  idlesupplier = idle~supplier
  if idlesupplier~available then do
    thread = idlesupplier~item
    idle~remove(thread)
  end
  else if size < max then
    thread = self~createThread
  else do
    return
  end

  -- thread must be set at this point
  messages~pull
  running~put(thread)
  thread~message = message
  

::method return
  expose idle running shutdown messages
  use strict arg thread
  running~remove(thread)
  if shutdown & messages~items = 0 then do
    thread~interrupted=.true
    return
  end
  idle~put(thread)
  self~wakeThread

::class ThreadPoolThread subclass Thread
::method init
  expose threadpool name message
  use strict arg threadpool, name
  message = .nil
  self~interrupted = .false
  self~init:super(name)

::attribute message

::attribute interrupted 

::method process
  expose threadpool message name interrupted
--  trace r
  do while \ self~isInterrupted
    guard on when message \= .nil | interrupted
    if interrupted then return
    message~send
    message = .nil
    threadpool~return(self)
    guard off
  end

