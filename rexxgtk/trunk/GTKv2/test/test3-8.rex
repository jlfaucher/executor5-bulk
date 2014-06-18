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


-- Derived from Listing 3-8
-- Foundations of GTK+ Development
-- by Andrew Krause

-- Note: The source code in the book is wrong. The downloaded source for the
-- book is correct. Label controls do not respond to the "clicked" signal so
-- we need buttons instead.

call gtk_init
window = .myMainWindow~new(.gtk~GTK_WINDOW_TOPLEVEL)
window~set_title('Notebook')
window~signal_connect("destroy")
window~set_border_width(10)
window~set_size_request(200, 100)

notebook = .GtkNotebook~new()
label1 = .GtkLabel~new('Page One')
label2 = .GtkLabel~new('Page Two')
child1 = .MyButton~newWithLabel('Go to page 2 to find the answer.')
child2 = .MyButton~newWithLabel('Go to page 1 to find the answer.')

child1~signal_connect('clicked')
child2~signal_connect('clicked')
-- Store the notebook widget so we can get access to it in the callbacks
child1~user_data = notebook
child2~user_data = notebook

notebook~append_page(child1, label1)
notebook~append_page(child2, label2)

notebook~set_tab_pos(.gtk~GTK_POS_BOTTOM)

window~add(notebook)
window~show_all()

call gtk_main
return


::requires 'oorexxgtk2.cls'

::class myMainWindow subclass GtkWindow

::method signal_destroy
call gtk_main_quit
return

::class MyButton subclass GtkButton

::method signal_clicked
notebook = self~user_data  -- get the notebook widget
page = notebook~get_current_page()
if page = 0 then notebook~set_current_page(1)
else if page = 1 then notebook~set_current_page(0)
else nop
return

