#!/usr/bin/rexx
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2007-2008 Rexx Language Association. All rights reserved.    */
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


-- Derived from Listing 7-5
-- Foundations of GTK+ Development
-- by Andrew Krause

call gtk_init
window = .myMainWindow~new(.gtk~GTK_WINDOW_TOPLEVEL)
window~set_title('Cut, Copy & Paste')
window~signal_connect('destroy')
window~set_border_width(10)

textview = .GtkTextView~new()

cut = .MyCut~newFromStock(.gtk~GTK_STOCK_CUT)
copy = .MyCopy~newFromStock(.gtk~GTK_STOCK_COPY)
paste = .MyPaste~newFromStock(.gtk~GTK_STOCK_PASTE)

cut~user_data = textview
copy~user_data = textview
paste~user_data = textview

cut~signal_connect('clicked')
copy~signal_connect('clicked')
paste~signal_connect('clicked')

scrolled_win = .GtkScrolledWindow~new(.nil, .nil)
scrolled_win~set_size_request(300, 200)
scrolled_win~add(textview)

hbox = .GtkHBox~new(.false, 5)
hbox~pack_start_defaults(cut)
hbox~pack_start_defaults(copy)
hbox~pack_start_defaults(paste)

vbox = .GtkVBox~new(.false, 5)
vbox~pack_start(scrolled_win, .true, .true, 0)
vbox~pack_start(hbox, .false, .true, 0)

window~add(vbox)
window~show_all()

call gtk_main
return


::requires 'rexxgtk.cls'

::class myMainWindow subclass GtkWindow

::method signal_destroy
call gtk_main_quit
return


::class MyCut subclass GtkButton

::method signal_clicked
buffer = self~user_data~get_buffer()

buffer~cut_clipboard(.true)
return


::class MyCopy subclass GtkButton

::method signal_clicked
buffer = self~user_data~get_buffer()

buffer~copy_clipboard()
return


::class MyPaste subclass GtkButton

::method signal_clicked
buffer = self~user_data~get_buffer()

buffer~paste_clipboard(.true)
return

