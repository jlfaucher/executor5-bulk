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
filename = 'Readme.txt'
tall = 24
wide = 19
spacer = 5

scr = .window~new()

p = .pad~new(200, wide + 1)

s1 = p~subpad(tall, wide + 1, 1, 1)
s2 = p~subpad(tall, wide + 1, tall + 1, 1)
s3 = p~subpad(tall, wide + 1, (2 * tall) + 1, 1)

strm = .stream~new(filename)
strm~open('read')
eof = .false
call on notready name eof

ch = strm~charin()
do while \eof
   p~addch(ch)
   ch = strm~charin()
   end
strm~close()

-- s1~prefresh(1, 1, 1, 1, tall, wide + 1)
-- s2~prefresh(1, 1, 1, wide + spacer + 1, tall, (wide * 2) + spacer)
-- s3~prefresh(1, 1, 1, (wide * 2) + (spacer * 2), tall, (wide *3) + (spacer * 2))
/* this is the optimized version */
s1~pnoutrefresh(1, 1, 1, 1, tall, wide + 1)
s2~pnoutrefresh(1, 1, 1, wide + spacer + 1, tall, (wide * 2) + spacer)
s3~pnoutrefresh(1, 1, 1, (wide * 2) + (spacer * 2), tall, (wide *3) + (spacer * 2))
scr~doupdate()

scr~getch()

scr~endwin()
return


eof:
eof = .true
return


::requires 'ncurses.cls'

