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


-- Derived from Listing 5-9
-- Foundations of GTK+ Development
-- by Andrew Krause

-- Note: Unlike the example program in the book the initial color values are
--       never modified in this program. So when you switch between the modal
--       and the non-modal dialogs, the previuos color is lost.

if arg() > 0 then color = arg(1)
else color = '#333344445555FFFF'
alpha = 65535

window = .myMainWindow~new('GTK_WINDOW_TOPLEVEL')
window~title = 'Color Selection Dialogs'
window~signal_connect('destroy')
window~border_width = 10
window~set_size_request(200, 75)

modal = .mymodal~new('Modal', color, alpha)
nonmodal = .mynonmodal~new('Non-Modal', color, alpha)

modal~signal_connect('clicked')
nonmodal~signal_connect('clicked')

hbox = .GtkHBox~new(.true, 10)
hbox~pack_start_defaults(modal)
hbox~pack_start_defaults(nonmodal)

window~add(hbox)
window~show_all()

call gtk_main
return


::requires 'rexxgtk.cls'

::class myMainWindow subclass GtkWindow

::method signal_destroy
.local['GTK_Quit'] = .true
return

::class mymodal subclass GtkButton

::method init
expose color alpha
use arg title, color, alpha
self~init:super(title)


::method signal_clicked
expose color alpha
dialog = .myDialog~new('Choose Color -- Modal', color, alpha)
dialog~modal = .true
return

::class mynonmodal subclass GtkButton

::method init
expose color alpha
use arg title, color, alpha
self~init:super(title)

::method signal_clicked
expose color alpha
dialog = .myDialog~new('Choose Color -- Non-Modal', color, alpha)
dialog~modal = .false
return

::class myDialog subclass GtkColorSelectionDialog

::method init
use strict arg title, color, alpha
self~init:super(title, color, alpha)
self~signal_connect('response')
self~show_all()

::method signal_response
use strict arg response
say 'response =' response
response = Value2String('GTKRESPONSETYPE', response)
select
   when response = 'GTK_RESPONSE_HELP' then do
      say 'Read the GTK+ API documentation.'
      end
   when response = 'GTK_RESPONSE_OK' then do
      say self~color
      end
   otherwise do
      self~destroy()
      end
   end
return

