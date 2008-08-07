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


-- Derived from Listing 3-9
-- Foundations of GTK+ Development
-- by Andrew Krause

call gtk_init
window = .myMainWindow~new(.gtk~GTK_WINDOW_TOPLEVEL)
window~set_title('Event Box')
window~signal_connect("destroy")
window~set_border_width(10)
window~set_size_request(200, 50)

eventbox = .MyEventBox~new()
label = .GtkLabel~new('Double-Click Me!')

eventbox~set_above_child(.false)

eventbox~user_data = label
eventbox~signal_connect('button_press_event')

eventbox~add(label)
window~add(eventbox)
eventbox~set_events(.gtk~GDK_BUTTON_PRESS_MASK)
eventbox~realize()
-- FIX THIS! Need to set the cursor to a hand.
window~show_all()

call gtk_main
return


::requires 'rexxgtk.cls'

::class myMainWindow subclass GtkWindow

::method signal_destroy
call gtk_main_quit
return

::class MyEventBox subclass GtkEventBox

::method signal_button_press_event
use strict arg event
if event~type() =.gtk~GDK_2BUTTON_PRESS then do
   text = self~user_data~get_label()
   if text~substr(1, 1) = 'D' then do
      self~user_data~set_label('I was Double-Clicked!')
      end
   else do
      self~user_data~set_label('Double-Click Me Again!')
      end
   end
return .false

