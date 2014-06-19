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


-- Derived from Listing 4-10
-- Foundations of GTK+ Development
-- by Andrew Krause

call gtk_init
window = .myMainWindow~new(.gtk~GTK_WINDOW_TOPLEVEL)
window~set_title('File Chooser Button')
window~signal_connect("destroy")
window~set_border_width(10)

label = .GtkLabel~new('')

chooser1 = .MyButton1~new('Chooser a Folder', .gtk~GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER)
chooser2 = .MyButton2~new('Chooser a Folder', .gtk~GTK_FILE_CHOOSER_ACTION_OPEN)

chooser1~user_data = chooser2
chooser2~user_data = label

chooser1~signal_connect('selection_changed')
chooser2~signal_connect('selection_changed')

chooser1~set_current_folder(g_get_home_dir())
chooser2~set_current_folder(g_get_home_dir())

filter1 = .GtkFileFilter~new()
filter2 = .GtkFileFilter~new()
filter1~set_name('Image Files')
filter2~set_name('All Files')
filter1~add_pattern('*.png')
filter1~add_pattern('*.jpg')
filter1~add_pattern('*.gif')
filter2~add_pattern('*')

chooser2~add_filter(filter1)
chooser2~add_filter(filter2)

vbox = .GtkVBox~new(.false, 5)
vbox~pack_start_defaults(chooser1)
vbox~pack_start_defaults(chooser2)
vbox~pack_start_defaults(label)

window~add(vbox)
window~show_all()

call gtk_main
return


::requires 'oorexxgtk3.cls'

::class myMainWindow subclass GtkWindow

::method signal_destroy
call gtk_main_quit
return

::class MyButton1 subclass GtkFileChooserButton

::method signal_selection_changed
-- the folder changed
folder = self~get_filename()
self~user_data~set_current_folder(folder)
return

::class MyButton2 subclass GtkFileChooserButton

::method signal_selection_changed
-- file selected
file = self~get_filename()
self~user_data~set_label(file)
return

