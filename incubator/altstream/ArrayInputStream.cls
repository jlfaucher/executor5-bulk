/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2018 Rexx Language Association. All rights reserved.         */
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
::class ArrayInputStream public inherit InputStream
::method init
  expose currentLine lines
  use strict arg lines

  -- we want a copy of the array so that the content is
  -- isolated from changes. This also allows lists, queues, etc. to be
  -- converted to a stream
  lines = lines~makearray
  currentLine = 1

-- open will reset to the beginning
::method open
  expose currentLine
  use strict arg

  currentLine = 1

-- close will also reset to the beginning
::method close
  forward message('OPEN')

-- character methods not supported
::method charin
  raise syntax 93.963  -- not supported
::method chars
  raise syntax 93.963  -- not supported

::method linein
  expose lines currentLine
  -- we allow a line number to be specified.
  use strict arg lineNumber = (currentLine)

  -- are we trying to read beyond the end of the
  -- array? This is a notready condition with a null
  -- string return
  if lineNumber > lines~items then do
     raise notready additional(self) return("")
  end

  -- the next read is one line down
  currentLine = lineNumber + 1
  -- and complete the read
  return lines[lineNumber]

::method lines
  expose lines currentLine
  use strict arg  -- no arguments allowed

  -- calculate the default number of lines
  count = lines~items - currentLine + 1

  -- everything has been read, so lines is zero
  if count < 1 then do
      return 0
  end

  return count

