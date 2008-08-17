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


-- Derived from Listing 7-7
-- Foundations of GTK+ Development
-- by Andrew Krause

call gtk_init
text_scales = .array~new()
text_scales[1] = .text_to_double~new()
text_scales[1]~str = 'Quarter Sized'
text_scales[1]~scale = 0.25
text_scales[2] = .text_to_double~new()
text_scales[2]~str = 'Double Extra Small'
text_scales[2]~scale = .gtk~PANGO_SCALE_XX_SMALL
text_scales[3] = .text_to_double~new()
text_scales[3]~str = 'Extra Small'
text_scales[3]~scale = .gtk~PANGO_SCALE_X_SMALL
text_scales[4] = .text_to_double~new()
text_scales[4]~str = 'Small'
text_scales[4]~scale = .gtk~PANGO_SCALE_SMALL
text_scales[5] = .text_to_double~new()
text_scales[5]~str = 'Medium'
text_scales[5]~scale = .gtk~PANGO_SCALE_MEDIUM
text_scales[6] = .text_to_double~new()
text_scales[6]~str = 'Large'
text_scales[6]~scale = .gtk~PANGO_SCALE_LARGE
text_scales[7] = .text_to_double~new()
text_scales[7]~str = 'Extra Large'
text_scales[7]~scale = .gtk~PANGO_SCALE_X_LARGE
text_scales[8] = .text_to_double~new()
text_scales[8]~str = 'Double Extra Large'
text_scales[8]~scale = .gtk~PANGO_SCALE_XX_LARGE
text_scales[9] = .text_to_double~new()
text_scales[9]~str = 'Double Sized'
text_scales[9]~scale = 2.0

window = .myMainWindow~new(.gtk~GTK_WINDOW_TOPLEVEL)
window~set_title('Text Tags')
window~signal_connect('destroy')
window~set_border_width(10)
window~set_size_request(500, -1)

textview = .GtkTextview~new()
buffer = textview~get_buffer()

buffer~create_tag_int('bold', 'weight', .gtk~PANGO_WEIGHT_BOLD)
buffer~create_tag_int('italic', 'style', .gtk~PANGO_STYLE_ITALIC)
buffer~create_tag_int('strike', 'strikethrough', .true)
buffer~create_tag_int('underline', 'underline', .gtk~PANGO_UNDERLINE_SINGLE)

bold = .myFormatButton~newFromStock(.gtk~GTK_STOCK_BOLD)
italic = .myFormatButton~newFromStock(.gtk~GTK_STOCK_ITALIC)
underline = .myFormatButton~newFromStock(.gtk~GTK_STOCK_UNDERLINE)
strike = .myFormatButton~newFromStock(.gtk~GTK_STOCK_STRIKETHROUGH)
clear = .myClearButton~newFromStock(.gtk~GTK_STOCK_CLEAR)
scale = .myComboBox~new()

do ts over text_scales
   scale~append_text(ts~str)
   buffer~create_tag_float(ts~str, 'scale', ts~scale)
   end

bold~user_data = textview
italic~user_data = textview
underline~user_data = textview
strike~user_data = textview

bold~user_data = buffer
italic~user_data = buffer
underline~user_data = buffer
strike~user_data = buffer
clear~user_data = buffer
scale~user_data = buffer

bold~set_data('tag', 'bold')
italic~set_data('tag', 'italic')
underline~set_data('tag', 'underline')
strike~set_data('tag', 'strike')

bold~signal_connect('clicked')
italic~signal_connect('clicked')
underline~signal_connect('clicked')
strike~signal_connect('clicked')
scale~signal_connect('changed')
clear~signal_connect('clicked')

vbox = .GtkVBox~new(.true, 5)
vbox~pack_start(bold, .false, .false, 0)
vbox~pack_start(italic, .false, .false, 0)
vbox~pack_start(underline, .false, .false, 0)
vbox~pack_start(strike, .false, .false, 0)
vbox~pack_start(scale, .false, .false, 0)
vbox~pack_start(clear, .false, .false, 0)

scrolled_win = .GtkScrolledWindow~new(.nil, .nil)
scrolled_win~add(textview)
scrolled_win~set_policy(.gtk~GTK_POLICY_AUTOMATIC, .gtk~GTK_POLICY_ALWAYS)

hbox = .GtkHBox~new(.false, 5)
hbox~pack_start(scrolled_win, .true, .true, 0)
hbox~pack_start(vbox, .false, .true, 0)

window~add(hbox)
window~show_all()

call gtk_main
return


::requires 'rexxgtk.cls'

::class text_to_double

::attribute str
::attribute scale

::class myMainWindow subclass GtkWindow

::method signal_destroy
call gtk_main_quit
return


::class myFormatButton subclass GtkButton

::method signal_clicked
tagname = self~get_data('tag')
buffer = self~user_data
buffer~apply_tag_by_name(tagname)
return


::class myComboBox subclass GtkComboBoxText

::method signal_changed
if self~get_active() = -1 then return

text = self~get_active_text()
self~set_data('tag', text)
self~user_data~apply_tag_by_name(text)
self~set_active(-1)
return


::class myClearButton subclass GtkButton

::method signal_clicked
self~user_data~remove_all_tags()
return

