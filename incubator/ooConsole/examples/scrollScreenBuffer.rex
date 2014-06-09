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

/** scrollScreenBuffer.rex
 *
 * This example is a direct copy of a C++ example from the MSDN library.  It
 * scrolls the bottom 15 lines of the display window up 15 lines.
 */

    say .endOfLine"Printing 20 lines for reference. " || -
        "Notice that line 6 is discarded during scrolling.".endOfLine
    do i = 1 while i <= 20
        say i
    end
    j = SysSleep(4)

    stdOut = .StdOutput~new

    if stdOut == .nil then do
        say "Failed to get StdOuput object.  ErrRC:" stOut~ErrRC
        return
    end

    -- Get the screen buffer size.

    csbiInfo = stdOut~getScreenBufferInfo
    if csbiInfo == .nil then do
        say "getScreenBufferInfo() failed:" stdOut~ErrRC
        return
    end

    -- The scrolling rectangle is the bottom 15 rows of the
    -- display window.

    scrollRect = .Rect~new(0, csbiInfo~windowRect~Bottom - 15, csbiInfo~windowRect~Right, csbiInfo~windowRect~Bottom - 1)

    -- The destination for the scroll rectangle is one row up.

    coordDest = .Point~new(0, csbiInfo~windowRect~Bottom - 16)

    -- The clipping rectangle is the same as the scrolling rectangle.

    clipRect = scrollRect

    -- Fill the bottom row with green blanks.

    fill = .CharInfo~from(' ', "BgGreen FgRed")

    -- Scroll up one line.

    if \ stdout~scrollScreenBuffer(scrollRect, coordDest, fill, clipRect) then do
        say "scrollScreenBuffer() failed:" stdOut~ErrRC
        return;
    end

::requires "ooConsole.cls"
