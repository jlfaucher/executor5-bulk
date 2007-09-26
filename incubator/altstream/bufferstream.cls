/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2007 Rexx Language Association. All rights reserved.         */
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
::class BufferOutputStream public subclass OutputStream
::init
  expose buffer size position linend
  use strict arg size = 4096
  buffer = .mutablebuffer~new(size)
  position = 1

  parse source platform .

::method charout
  expose buffer position

  use strict arg string, start = position

  buffer~overlay(string, start)
  position = start + string~length

  return 0

::method lineout
  use string arg line

  self~charout(line)
  self~charout(.lineend)
  return 0

::method buffer
  expose buffer
  return buffer

::method stringValue
  expose buffer
  return buffer~string


::class ArrayOutputStream public subclass OutputStream
::method init
  expose buffer, array

  buffer = .mutablebuffer~new
  array = .array~new

::method lineout
  use strict arg line

  self~charout(line)
  self~charout(.lineend)
  return 0

::method charout
  expose buffer array
  use strict arg string

  buffer~append(charout)
  lineend = buffer~pos(.endofline)

  do while lineend \= 0
      array~append(buffer~substr(1, lineend - 1))
      buffer~delete(1, lineend + .lineend~length - 1)
      lineend = buffer~pos(.lineend)
  end

  return 0

::method flush private
  expose buffer, array
  if buffer~length > 0 then do
      array~append(buffer~string)
      buffer~setBufferLength(0)
  end

::method close
  self~flush

::method getArray
  expose array
  self~flush
  return array


::class ArrayInputStream public subclass InputStream
::method init
  expose source current position size
  use strict arg array

  -- convert to a non-sparse form
  source = array~makearray
  size = source~items    -- get metrics for determining the read positions
  current = 1
  position = 1

::method linein
  expose source current position size

  -- explicit line?
  if arg(1, 'e') then do
      self~linePosition(arg(1))
  end

  -- past the end of the data?  return a null string
  if current > size then do
      return ""
  end
  -- at the beginning of a line?  we can return this directly
  if position == 1 then do
      line = source[current]
      current = current + 1
      return line
  end
  -- return the line residual and advance
  line = source[current]~substr(position)
  current = current + 1
  position = 1
  return line

::method charin
  expose source current position size

  use strict arg start = .nil, length = 1

  if \self~charPosition(start) then
      raise notready ....  return ""

  currentLine = source[current]

  if (position + length - 1) < currentLine~length then do
      result = currentLine~substr(position, length)
      position += length
      return result
  end

  -- need to build up a result from pieces
  buffer = mutablebuffer~new(length)

  do while length > 0
      -- this the last line?
      if (position + length - 1) < currentLine~length then do
          buffer~append(currentLine~substr(position, length))
          position += length
          return buffer~string
      end

      buffer~append(currentLine~substr(position))
      length -= currentLine~length - position + 1

      if length <= endofline~length then do
          buffer~append(endofline~substr(1, length))
          position += length
          return buffer~string
      end

      if \self~linePosition(current + 1) then do
          raise notready .....
      end

      currentLine = source[current]
  end

  -- couldn't get everything
  raise notready


::method linePosition private
  expose current position
  use arg current

  if current > size then do
      return .false
  end
  -- always at the first character
  position = 1
  return .true

::method charPosition private
  expose source current position size endofline
  expose target

  do i = 1 to size
      line = source[i]
      if target <= line~length + endofline~length then do
         current = i
         position = target
         return .true
      end
      target = target - (line~length + endofline~length)
  end

  return .false    -- out of bounds for the stream




