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


-- Derived from Listing 9-6
-- Foundations of GTK+ Development
-- by Andrew Krause

call gtk_init
window = .myMainWindow~new(.gtk~GTK_WINDOW_TOPLEVEL)
window~set_title('Toolbars')
window~signal_connect('destroy')
window~set_size_request(325, -1)

entry = .GtkEntry~new()
toolbar = .GtkToolbar~new()
call create_toolbar toolbar, entry

vbox = .GtkVBox~new(.false, 5)
vbox~pack_start_defaults(toolbar)
vbox~pack_start_defaults(entry)

window~add(vbox)
window~show_all()

call gtk_main
return

::requires 'rexxgtk.cls'

::class myMainWindow subclass GtkWindow

::method signal_destroy
call gtk_main_quit
return

::class myCut subclass GtkToolButton

::method signal_clicked
self~user_data~cut_clipboard()
return

::class myCopy subclass GtkToolButton

::method signal_clicked
self~user_data~copy_clipboard()
return

::class myPaste subclass GtkToolButton

::method signal_clicked
self~user_data~paste_clipboard()
return

::class mySelectall subclass GtkToolButton

::method signal_clicked
self~user_data~select_region(self~user_data, 0, -1)
return

::routine create_toolbar
use strict arg toolbar, entry

cut = .myCut~newFromStock(.gtk~GTK_STOCK_CUT);
copy = .myCopy~newFromStock(.gtk~GTK_STOCK_COPY);
paste = .myPaste~newFromStock(.gtk~GTK_STOCK_PASTE);
selectall = .mySelectall~newFromStock(.gtk~GTK_STOCK_SELECT_ALL);
separator = .GtkSeparatorToolItem~new();

toolbar~set_show_arrow(.true)
toolbar~set_style(.gtk~GTK_TOOLBAR_BOTH)

toolbar~insert(cut, 0)
toolbar~insert(copy, 1)
toolbar~insert(paste, 2)
toolbar~insert(separator, 3)
toolbar~insert(selectall, 4)

-- save user data for the callbacks
cut~user_data = entry
copy~user_data = entry
paste~user_data = entry
selectall~user_data = entry

cut~signal_connect('clicked')
copy~signal_connect('clicked')
paste~signal_connect('clicked')
selectall~signal_connect('clicked')

return

