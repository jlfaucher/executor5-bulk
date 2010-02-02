#!/usr/bin/rexx
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2010-2010 Rexx Language Association. All rights reserved.    */
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
/* Authors;                                                                   */
/*       W. David Ashley <dashley@us.ibm.com>                                 */
/*                                                                            */
/*----------------------------------------------------------------------------*/


-- Note: If this is run inside a GNOME terminal then mouse button 2 will be
--       intercepted by GNOME before the program sees the event.

numeric digits 12
lf = '0A'x

scr = .window~new()
scr~noecho()
scr~keypad(.true)

mmask = scr~mousemask(scr~ALL_MOUSE_EVENTS)

do forever
   ch = scr~getch()
   if ch = scr~KEY_MOUSE then do
      scr~clear()
      mort = scr~getmouse()
      select
         when mort~bstate = mort~BUTTON1_PRESSED then do
            scr~mvaddstr(1, 1, 'Button 1 Pressed!')
            end
         when mort~bstate = mort~BUTTON1_RELEASED then do
            scr~mvaddstr(1, 1, 'Button 1 Released!')
            end
         when mort~bstate = mort~BUTTON1_CLICKED then do
            scr~mvaddstr(1, 1, 'Button 1 Clicked!')
            end
         when mort~bstate = mort~BUTTON1_DOUBLE_CLICKED then do
            scr~mvaddstr(1, 1, 'Button 1 Dbl-Clicked!')
            end
         when mort~bstate = mort~BUTTON2_PRESSED then do
            scr~mvaddstr(1, 1, 'Button 2 Pressed!')
            end
         when mort~bstate = mort~BUTTON2_RELEASED then do
            scr~mvaddstr(1, 1, 'Button 2 Released!')
            end
         when mort~bstate = mort~BUTTON2_CLICKED then do
            scr~mvaddstr(1, 1, 'Button 2 Clicked!')
            end
         when mort~bstate = mort~BUTTON2_DOUBLE_CLICKED then do
            scr~mvaddstr(1, 1, 'Button 2 Dbl-Clicked!')
            end
         when mort~bstate = mort~BUTTON3_PRESSED then do
            scr~mvaddstr(1, 1, 'Button 3 Pressed!')
            end
         when mort~bstate = mort~BUTTON3_RELEASED then do
            scr~mvaddstr(1, 1, 'Button 3 Released!')
            end
         when mort~bstate = mort~BUTTON3_CLICKED then do
            scr~mvaddstr(1, 1, 'Button 3 Clicked!')
            end
         when mort~bstate = mort~BUTTON3_DOUBLE_CLICKED then do
            scr~mvaddstr(1, 1, 'Button 3 Dbl-Clicked!')
            end
         otherwise nop
         end
      scr~refresh()
      end
   if ch = lf then leave
   end

scr~endwin()
return


::requires 'ncurses.cls'

