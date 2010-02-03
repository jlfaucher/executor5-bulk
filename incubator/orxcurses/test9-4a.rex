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
lf = '0A'x

.window~setbase(0)
scr = .window~new()
scr~start_color()
scr~init_pair(1, scr~COLOR_WHITE, scr~COLOR_BLUE)
scr~init_pair(2, scr~COLOR_RED, scr~COLOR_YELLOW)
scr~init_pair(3, scr~COLOR_CYAN, scr~COLOR_GREEN)

maxyx = scr~getmaxyx()
parse var maxyx maxy maxx .

/* create subwindow */
grandpa = .window~new(maxy - 4, maxx - 10, 2, 5)
father = grandpa~subwin(maxy - 8, maxx - 20, 4, 10)
boy = father~subwin(maxy - 16, maxx - 40, 8, 20)

/* color window and splash some text */
grandpa~bkgd(grandpa~COLOR_PAIR(1))
grandpa~addstr("Grandpa")
father~bkgd(father~COLOR_PAIR(2))
father~addstr("Father")
boy~bkgd(boy~COLOR_PAIR(3))
boy~addstr("Boy")
grandpa~refresh()
ch = grandpa~getch()

/* remove the subwindow "boy" */
boy~delwin()
father~clear()
father~addstr("Bye, son!" || lf)
father~refresh()
ch = father~getch()

scr~endwin()
return


::requires 'ncurses.cls'

