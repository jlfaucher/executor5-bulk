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


-- Derived from Listing 5-11
-- Foundations of GTK+ Development
-- by Andrew Krause

call gtk_init
page = .Array~new()
page[1] = .PageInfo~new(.nil, -1, 'Introduction', .gtk~GTK_ASSISTANT_PAGE_INTRO, .true)
page[2] = .PageInfo~new(.nil, -1, '', .gtk~GTK_ASSISTANT_PAGE_CONTENT, .false)
page[3] = .PageInfo~new(.nil, -1, 'Click the Check Button', .gtk~GTK_ASSISTANT_PAGE_CONTENT, .false)
page[4] = .PageInfo~new(.nil, -1, 'Click the Button', .gtk~GTK_ASSISTANT_PAGE_PROGRESS, .false)
page[5] = .PageInfo~new(.nil, -1, 'Confirmation', .gtk~GTK_ASSISTANT_PAGE_CONFIRM, .true)

assistant = .myAssistant~new()
assistant~set_size_request(450,300)
assistant~set_title('GtkAssistant Example')

assistant~signal_connect('destroy')

page[1]~widget = .GtkLabel~new('This is an example of a GtkAssistant. By' || '0A'x ||,
                               'clicking the forward button, you can continue' || '0A'x ||,
                               'to the next section')
page[2]~widget = .GtkHBox~new(.false, 5)
page[3]~widget = .MyCheckButton~newWithLabel('Click Me To Continue')
page[4]~widget = .GtkAlignment~new(0.5, 0.5, 0.0, 0.0)
page[5]~widget = .GtkLabel~new('Text has been entered in the label and the' || '0A'x ||,
                               'combo box is clicked. If you are done, then' || '0A'x ||,
                               'it is time to leave!')

label = .GtkLabel~new('Your Name:')
entry = .myEntry~new()
page[2]~widget~pack_start(label, .false, .false, 5)
page[2]~widget~pack_start(entry, .false, .false, 5)

button = .myButton~newWithLabel('Click me!')
progress = .GtkProgressBar~new()
hbox = .GtkHBox~new(.false, 5)
hbox~pack_start(progress, .true, .false, 5)
hbox~pack_start(button, .false, .false, 5)
page[4]~widget~add(hbox)
button~user_data = .Array~new()
button~user_data[1] = assistant
button~user_data[2] = progress
button~user_data[3] = page[4]~widget

do i = 1 to 5
   page[i]~index = assistant~append_page(page[i]~widget)
   assistant~set_page_title(page[i]~widget, page[i]~title)
   assistant~set_page_type(page[i]~widget, page[i]~type)
   assistant~set_page_complete(page[i]~widget, page[i]~complete)
   end

entry~signal_connect('changed')
entry~user_data = assistant
page[3]~widget~signal_connect('toggled')
page[3]~widget~user_data = assistant
button~signal_connect('clicked')

assistant~signal_connect('cancel')
assistant~signal_connect('close')

assistant~show_all()

call gtk_main
return


::requires 'oorexxgtk3.cls'

::class myMainWindow subclass GtkWindow

::method signal_destroy
call gtk_main_quit
return

::class PageInfo

::attribute widget
::attribute index
::attribute title
::attribute type
::attribute complete

::method init
expose widget index title type complete
use strict arg widget, index, title, type, complete
return

::class myEntry subclass GtkEntry

::method signal_changed
text = self~get_text()
num = self~user_data~get_current_page()
page = self~user_data~get_nth_page(num)
self~user_data~set_page_complete(page, length(text) > 0)
return

::class myButton subclass GtkButton

::method signal_clicked
percent = 0
self~set_sensitive(.false)
assistant = self~user_data[1]
progress = self~user_data[2]
page = self~user_data[3]
do while percent <= 100
   message = percent'% Complete'
   progress~set_fraction(percent / 100)
   progress~set_text(message)
   do while gtk_events_pending()
      call gtk_main_iteration
      end
   call syssleep(0.5)
   percent = percent + 5
   end
assistant~set_page_complete(page, .true)
return

::class myCheckButton subclass GtkCheckButton

::method signal_toggled
active = self~get_active()
self~user_data~set_page_complete(self, active)
return

::class myAssistant subclass GtkAssistant

::method signal_cancel
self~destroy()
call gtk_main_quit
return

::method signal_close
say 'You would apply your changes now!'
self~destroy()
call gtk_main_quit
return

