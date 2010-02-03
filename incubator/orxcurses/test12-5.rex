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


-- Note: When this is executed inside a GNOME window some function keys
--       are intercepted by GNOME and thus are never seen by NCurses.
--       F1, F10 & F11 are usually intercepted.

numeric digits 12
lf = .window~ASCII_LF~d2c()
center = 1
label_text = .array~of('I', 'AM', 'SAM', 'DO', 'NOT', 'LIKE', 'THAT', 'SAY',,
                       'WOULD', 'COULD', 'YOU', 'GREEN', 'EGGS', 'AND', 'HAM')

.window~slk_init(2)
scr = .window~new()
scr~noecho()
scr~keypad(.true)

do label = 1 to label_text~items()
   scr~slk_set(label, label_text[label], center)
   end
scr~slk_refresh()
scr~addstr('Use the Function Keys to type' || lf)
scr~addstr('Press "?" or "!" oe "." to end a line' || lf)
scr~addstr('Press Enter to quit' || lf || lf)
scr~refresh()

ch = scr~getch()
do while ch <> lf
   select
      when ch = '?' | ch = '!' | ch = '.' then do
         scr~addch(ch)
         scr~addch(lf)
         end
      when ch = scr~KEY_F1 then do
         scr~addstr(label_text[1] || ' ')
         end
      when ch = scr~KEY_F2 then do
         scr~addstr(label_text[2] || ' ')
         end
      when ch = scr~KEY_F3 then do
         scr~addstr(label_text[3] || ' ')
         end
      when ch = scr~KEY_F4 then do
         scr~addstr(label_text[4] || ' ')
         end
      when ch = scr~KEY_F5 then do
         scr~addstr(label_text[5] || ' ')
         end
      when ch = scr~KEY_F6 then do
         scr~addstr(label_text[6] || ' ')
         end
      when ch = scr~KEY_F7 then do
         scr~addstr(label_text[7] || ' ')
         end
      when ch = scr~KEY_F8 then do
         scr~addstr(label_text[8] || ' ')
         end
      when ch = scr~KEY_F9 then do
         scr~addstr(label_text[9] || ' ')
         end
      when ch = scr~KEY_F10 then do
         scr~addstr(label_text[10] || ' ')
         end
      when ch = scr~KEY_F11 then do
         scr~addstr(label_text[11] || ' ')
         end
      when ch = scr~KEY_F12 then do
         scr~addstr(label_text[12] || ' ')
         end
      otherwise nop
      end
   scr~refresh()
   ch = scr~getch()
   end

scr~endwin()
return


::requires 'ncurses.cls'

