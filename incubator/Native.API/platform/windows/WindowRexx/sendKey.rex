/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2011-2011 Rexx Language Association. All rights reserved.    */
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
 * This is small example of using the .WindowRexx package to send keys to some
 * arbitrary window.
 *
 * This ONLY WORKS FOR Google Earth AND SENDING Ctrl-Alt-S TO THAT WINDOW,
 * as written.  The technique itself is generic and will work with any open,
 * top-level, window to send a key press event.
 */

 -- The winsystm.cls package has the .VirtualKeys class and ooDialog 4.2.0 has
 -- the .VK class to convert between the virtual key codes and symbolic names.
 -- For this program, I've assigned the correct key code numbers to these
 -- variables to make the program simplier.  VK_MENU is the Alt key.
  VK_CONTROL = 17
  VK_MENU    = 18
  VK_L       = 76
  VK_S       = 83

  -- Try to find an open Google Earth window.
  do 10
    j = SysSleep(.1)
    googleWindow = .WindowRexx~findWindow("Google Earth")
    if googleWindow~class == .WindowRexx then leave
  end

  -- If we found the window, send Ctrl-Alt-S to it, otherwise abort.
  if googleWindow~class == .WindowRexx then do
    say 'Got dialog window'

    googleWindow~toForeground

    -- You can also try sending Ctrl-L by switching the two lines:

    ret = googleWindow~sendKeyPress(VK_S, .array~of(VK_CONTROL, VK_MENU))
    --ret = googleWindow~sendKeyPress(VK_L, .array~of(VK_CONTROL))

    say 'Got ret:' ret
    return ret
  end
  else do
    say "Failed to find Google Earth, aborting"
    return 99
  end

::requires 'windowRexx.frm'
