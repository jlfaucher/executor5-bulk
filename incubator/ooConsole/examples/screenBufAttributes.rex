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

  con = .StdOutput~new

  say 'StdOutput:' con
  say 'Screen Buffer Size: ' con~getScreenBufferSize~print
  say 'Window Size:        ' con~getWindowSize~print
  say 'Window Rect:        ' con~getWindowRect~print

  say
  say 'Screen Buffer Info'
  d = con~getScreenBufferInfo
  say 'Attributes:         ' d~Attributes
  say 'Window Rect:        ' d~WindowRect~print
  say 'Screen Buffer Size: ' d~BufferSize~print
  say 'Maximum Window Size:' d~MaxWindowSize~print
  say 'Cursor Position:    ' d~CursorPos~print

  say
  say 'Screen Buffer Info Extended'
  d = con~getScreenBufferInfoEx
  say 'Attributes:         ' d~Attributes
  say 'Window Rect:        ' d~WindowRect~print
  say 'Screen Buffer Size: ' d~BufferSize~print
  say 'Maximum Window Size:' d~MaxWindowSize~print
  say 'Cursor Position:    ' d~CursorPos~print
  say 'Full Screen Support:' d~FullScreenSupport
  say 'Color Table:        '
  line = ''
  do i = 1 to d~colorTable~items
    line ||= d~colorTable[i]~right(9)
    if i // 4 == 0 then do
      say line
      line = ''
    end
  end

  d2 = .Directory~new
  d2~attributes = "BgBlue FgBlue FgRed FgIntensity"
  if \ con~setScreenBufferInfoEx(d2) then do
    say 'Failed to set NEW screen buffer info.  errRc:' con~errRC
  end
  else do
    say; say 'Set NEW screen buffer attributes.'; say
  end

  j = SysSleep(10)

  -- Restore the original screen buffer info
  if \ con~setScreenBufferInfoEx(d) then do
    say 'Failed to RESTORE screen buffer.  errRc:' con~errRC
  end
  else do
    say 'RESTORED screen buffer.'
  end

::requires 'ooConsole.cls'
