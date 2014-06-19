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


-- Derived from Listing 5-3
-- Foundations of GTK+ Development
-- by Andrew Krause

call gtk_init
dialog = .GtkDialog~new('Edit User Information', .nil,,
                        .gtk~GTK_DIALOG_MODAL,,
                        .gtk~GTK_STOCK_OK, .gtk~GTK_RESPONSE_OK)
dialog~add_button(.gtk~GTK_STOCK_CANCEL, .gtk~GTK_RESPONSE_CANCEL)
dialog~set_default_response(.gtk~GTK_RESPONSE_OK)

lbl1 = .GtkLabel~new('User Name')
lbl2 = .GtkLabel~new('Real Name')
lbl3 = .GtkLabel~new('Home Dir')
lbl4 = .GtkLabel~new('Host Name')

user = .GtkEntry~new()
real = .GtkEntry~new()
home = .GtkEntry~new()
host = .GtkEntry~new()

user~set_text(g_get_user_name())
real~set_text(g_get_real_name())
home~set_text(g_get_home_dir())
host~set_text(g_get_host_name())

table = .GtkTable~new(4, 2, .false)
table~attach_defaults(lbl1, 0, 1, 0, 1)
table~attach_defaults(lbl2, 0, 1, 1, 2)
table~attach_defaults(lbl3, 0, 1, 2, 3)
table~attach_defaults(lbl4, 0, 1, 3, 4)

table~attach_defaults(user, 1, 2, 0, 1)
table~attach_defaults(real, 1, 2, 1, 2)
table~attach_defaults(home, 1, 2, 2, 3)
table~attach_defaults(host, 1, 2, 3, 4)

table~set_row_spacings(5)
table~set_col_spacings(5)
table~set_border_width(5)

dialog~vbox~pack_start_defaults(table)
dialog~show_all()

retc = dialog~dialog_run()
if retc = .gtk~GTK_RESPONSE_OK then do
   say 'User Name:' user~get_text()
   say 'Real Name:' real~get_text()
   say 'Home Folder:' home~get_text()
   say 'Host Name:' host~get_text()
   end

dialog~destroy()
return


::requires 'oorexxgtk3.cls'

