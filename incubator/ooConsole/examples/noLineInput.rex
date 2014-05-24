/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2014-2014 Rexx Language Association. All rights reserved.    */
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

  stdin  = .StdInput~new

  mode  = stdin~getMode

  say 'Console mode STDIN: ' mode

  if \ stdin~setMode("EditMode") then do
    say 'Failed to set mode. errRC:' stdin~errRC
    return
  end
  say 'mode now:' stdin~getMode

  say 'Type.  Sleeping for 10 seconds'
  j = SysSleep(10)
  say 'Done sleeping'

  ans = ''
  charInput = stdin~getNumberOfInputEvents
  say 'charInput:' charInput
  say

  recs = stdin~peekInput(100)
  say 'Got records, count records:' recs~items
  msg = ''
  do r over recs
    say r~char pressed(r~keyDown) '('r~repeatCount')'
    if r~keyDown then msg ||=r~char~copies(r~repeatCount)
  end
  say msg
  say

  do while charInput > 1
    ans ||= stdin~read(1)
    charInput = stdin~getNumberOfInputEvents
    say 'charInput:' charInput
  end
  say 'Character input events:' stdin~getNumberOfInputEvents
  say 'ans:' ans
  say 'hex:' ans~c2x
  say

  what = stdin~peekInput(1)
  say '>>'what[1]~char pressed(what[1]~keyDown)'<<'
  stdin~flushInputBuffer
  say 'Character input events:' stdin~getNumberOfInputEvents

  stdin~setMode(mode)
  return

::requires 'ooConsole.cls'

::routine pressed
  use strict arg pressed

  if pressed then return 'pressed'
  return 'released'
