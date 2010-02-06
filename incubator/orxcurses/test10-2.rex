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

text1 = "Lorem ipsum dolor sit amet, consectetuer adipiscing elit, sed diam " ||,
        "nonummy nibh euismod tincidunt ut laoreet dolore magna aliquam erat " ||,
        "volutpat. Ut wisi enim ad minim veniam, quis nostrud exerci tation " ||,
        "ullamcorper suscipit lobortis nisl ut aliquip ex ea commodo consequat."
text2 = "Four score and seven years ago our fathers brought forth on this " ||,
        "continent, a new nation, conceived in Liberty, and dedicated to the " ||,
        "proposition that all men are created equal.";

scr = .window~new()

maxyx = scr~getmaxyx()
parse var maxyx maxy maxx .
halfy = (maxy / 2)~format(, 0)
halfx = (maxx / 2)~format(, 0)

top = .window~new(halfy, halfx, 1, 1)
bottom = .window~new(halfy, halfx, halfy + 1, halfx + 1)

top~addstr(text1)
top~refresh()
bottom~addstr(text2)
bottom~refresh()

bottom~getch()

retc = bottom~copywin(top, 1, 1, 1, 1, 5, 13, .false)
bottom~refresh()
bottom~getch()

scr~endwin()
return


::requires 'ncurses.cls'

