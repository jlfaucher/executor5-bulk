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

.window~setbase(0)
scr = .window~new()
scr~start_color()
scr~init_pair(1, scr~COLOR_BLACK, scr~COLOR_BLUE)
scr~init_pair(2, scr~COLOR_BLACK, scr~COLOR_RED)
scr~init_pair(3, scr~COLOR_BLACK, scr~COLOR_GREEN)
scr~init_pair(4, scr~COLOR_BLACK, scr~COLOR_CYAN)

/* calculate window sizes and locations */
maxyx = scr~getmaxyx()
parse var maxyx maxy maxx .
halfx = (maxx / 2)~format(, 0)
halfy = (maxy / 2)~format(, 0)

/* create four windows to fill the screen */
a = .window~new(halfy, halfx, 0, 0)
b = .window~new(halfy, halfx, 0, halfx)
c = .window~new(halfy, halfx, halfy, 0)
d = .window~new(halfy, halfx, halfy, halfx)

/* write to each window */
a~mvaddstr(0, 0, "This is window A" || lf)
a~bkgd(a~color_pair(1))
a~refresh()
b~mvaddstr(0, 0, "This is window B" || lf)
b~bkgd(b~color_pair(2))
b~refresh()
c~mvaddstr(0, 0, "This is window C" || lf)
c~bkgd(c~color_pair(3))
c~refresh()
d~mvaddstr(0, 0, "This is window D" || lf)
d~bkgd(c~color_pair(4))
d~refresh()
d~getch()

scr~endwin()
return


::requires 'ncurses.cls'

