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


-- Derived from Listing 7-6
-- Foundations of GTK+ Development
-- by Andrew Krause

-- Text iters are hidden in this example.

call gtk_init
window = .myMainWindow~new(.gtk~GTK_WINDOW_TOPLEVEL)
window~set_title('Searching Buffers')
window~signal_connect('destroy')
window~set_border_width(10)

textview = .GtkTextView~new()
entry = .GtkEntry~new()
entry~set_text('Search for ...')
find = .MyButton~newFromStock(.gtk~GTK_STOCK_FIND)

find~user_data = .array~new()
find~user_data[1] = entry
find~user_data[2] = textview

find~signal_connect('clicked')

scrolled_win = .GtkScrolledWindow~new(.nil, .nil)
scrolled_win~set_size_request(250, 200)
scrolled_win~add(textview)

hbox = .GtkHBox~new(.false, 5)
hbox~pack_start(entry, .true, .true, 0)
hbox~pack_start(find, .false, .true, 0)

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


::class MyButton subclass GtkButton

::method signal_clicked
i = 0
find = self~user_data[1]~get_text()
buffer = self~user_data[2]~get_buffer()

iter = buffer~forward_search(find, .true)
do while \iter~isNull()
   i = i + 1
   iter = buffer~forward_search_next(find, iter)
end

output = "The string '"find"' was found" i "times!"
dialog = .GtkMessageDialog~new(.nil, .gtk~GTK_DIALOG_MODAL, .gtk~GTK_MESSAGE_INFO,,
                               .gtk~GTK_BUTTONS_OK, output)

dialog~dialog_run()
dialog~destroy()
return

