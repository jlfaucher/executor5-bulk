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


-- Derived from Listing 9-4
-- Foundations of GTK+ Development
-- by Andrew Krause

call gtk_init
window = .myMainWindow~new(.gtk~GTK_WINDOW_TOPLEVEL)
window~set_title('Popup Menus')
window~signal_connect('destroy')
window~set_border_width(10)
window~set_size_request(250, -1)

menu = .GtkMenu~new()
eventbox = .myEventBox~new()
progress = .GtkProgressBar~new()
statusbar = .GtkStatusbar~new()
progress~set_text('Nothing Yet Happened')
call create_popup_menu menu, progress, statusbar

progress~set_pulse_step(0.05)
eventbox~set_above_child(.false)

eventbox~user_data = menu
eventbox~signal_connect('button_press_event')

vbox = .GtkVBox~new(.false, 5)
eventbox~add(progress)
vbox~pack_start_defaults(eventbox)
vbox~pack_start_defaults(statusbar)
window~add(vbox)

eventbox~set_events(.gtk~GDK_BUTTON_PRESS_MASK)
eventbox~realize()

window~show_all()
call gtk_main
return


::requires 'oorexxgtk3.cls'

::routine create_popup_menu
use strict arg menu, progress, statusbar

pulse = .MyPulse~newWithLabel('Pulse Progress')
fill = .MyFill~newWithLabel('Set as Complete')
clear = .MyClear~newWithLabel('Clear Progress')
separator = .GtkSeparatorMenuItem~new()

pulse~signal_connect('activate')
fill~signal_connect('activate')
clear~signal_connect('activate')

-- set up the user data for the signals
pulse~user_data = .array~new()
pulse~user_data[1] = progress
pulse~user_data[2] = statusbar
pulse~user_data[3] = 'Pulse the progressbar one step.'
fill~user_data = .array~new()
fill~user_data[1] = progress
fill~user_data[2] = statusbar
fill~user_data[3] = 'Set the progress bar to 100%.'
clear~user_data = .array~new()
clear~user_data[1] = progress
clear~user_data[2] = statusbar
clear~user_data[3] = 'Clear the progress bar to 0%.'

pulse~signal_connect('enter_notify_event')
pulse~signal_connect('leave_notify_event')
fill~signal_connect('enter_notify_event')
fill~signal_connect('leave_notify_event')
clear~signal_connect('enter_notify_event')
clear~signal_connect('leave_notify_event')

menu~append(pulse)
menu~append(separator)
menu~append(fill)
menu~append(clear)

menu~attach_to_widget(progress)
menu~show_all()
return

::class myMainWindow subclass GtkWindow

::method signal_destroy
call gtk_main_quit
return
      
::class myEventBox subclass GtkEventBox

::method signal_button_press_event
use strict arg event
if event~button = 3 then do
   self~user_data~popup()
   end
return .true

::class MyPulse subclass GtkMenuItem

::method signal_activate
self~user_data[1]~pulse()
self~user_data[1]~set_text('Pulse!')
return

::method signal_enter_notify_event
use strict arg event
call statusbar_hint self, event, self~user_data[2]
return .false

::method signal_leave_notify_event
use strict arg event
call statusbar_hint self, event, self~user_data[2]
return .false

::class MyFill subclass GtkMenuItem

::method signal_activate
self~user_data[1]~set_fraction(1.0)
self~user_data[1]~set_text('One Hundred Percent')
return

::method signal_enter_notify_event
use strict arg event
call statusbar_hint self, event, self~user_data[2]
return .false

::method signal_leave_notify_event
use strict arg event
call statusbar_hint self, event, self~user_data[2]
return .false

::class MyClear subclass GtkMenuItem

::method signal_activate
self~user_data[1]~set_fraction(0.0)
self~user_data[1]~set_text('Reset to Zero')
return

::method signal_enter_notify_event
use strict arg event
call statusbar_hint self, event, self~user_data[2]
return .false

::method signal_leave_notify_event
use strict arg event
call statusbar_hint self, event, self~user_data[2]
return .false

::routine statusbar_hint
use strict arg self, event, statusbar
id = statusbar~get_context_id('MenuItemHints')
if event~type = .gtk~GDK_ENTER_NOTIFY then do
   statusbar~push(id, self~user_data[3])
   end
else do
   statusbar~pop(id)
   end
return

