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


numeric digits 12
lf = .window~ASCII_LF~d2c()

mainmenu = "Main Menu"
menu = .array~of("Answer E-mail", "Off to the Web", "Word processing",,
                 "Financial management", "Maintenance", "Shutdown")
menuitem = 1

.window~setbase(0)
scr = .window~new()

call drawmenu menuitem

scr~keypad(.TRUE);
scr~noecho()			/* Disable echo */
key = scr~getch()
do while key <> lf
   select
      when key = scr~KEY_DOWN then do
         menuitem += 1
         if menuitem > menu~items() then menuitem = 1
         end
      when key = scr~KEY_UP then do
         menuitem -= 1
         if menuitem < 1 then menuitem = menu~items()
         end
      otherwise nop
      end
   call drawmenu menuitem
   key = scr~getch()
   end

scr~echo()				/* re-enable echo */

/* At this point, the value of the selected menu is kept in the
   menuitem variable. The program can branch off to whatever subroutine
   is required to carry out that function
*/

scr~endwin()
return


drawmenu:

use strict arg item
scr~clear()
scr~addstr(mainmenu)
do c = 1 to menu~items()
   if c = item then do
      scr~attron(scr~A_REVERSE)	   /* highlight selection */
      end
   scr~mvaddstr(1 + (c * 2), 20, menu[c])
   scr~attroff(scr~A_REVERSE)   /* remove highlight */
   end
scr~mvaddstr(17, 25, "Use arrow keys to move; Enter to select:")
scr~refresh()
return


::requires 'ncurses.cls'

