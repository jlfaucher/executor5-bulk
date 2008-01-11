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


-- FIXME! remove the comment from this section of code before releasing
--if rxfuncquery('GrxLoadFuncs') then do
   call rxfuncadd 'GrxLoadFuncs', 'rexxgtk', 'GrxLoadFuncs'
   retc = GrxLoadFuncs()
--   say 'Return code from loading RexxGTK functions was' retc
--   end

-- Create the Rexx queue we will use for callbacks
.local['GTK_Queue'] = RxQueue('Create', 'GTKQUEUE23')
call GrxSetRexxQueueName .local['GTK_Queue']

-- Create the widget database
.local['GTK_Database'] = .directory~new()

-- Create the GTK quit flag
.local['GTK_Quit'] = .false


/*============================================================================*/
/* Routine: gtk_main                                                          */
/*============================================================================*/

::routine gtk_main public
do while .local['GTK_Quit'] = .false
   call GrxMainIteration
   old_queue = RxQueue('Set', .local['GTK_Queue'])
   do while queued() > 0
      pull pointer msg args
      say pointer msg args
      widget = .local['GTK_Database']~at(pointer)
      if widget <> .nil then do
         widget~run_method(msg, args)
         end
      end
   call RxQueue 'Set', old_queue
   end
.local['GTK_Quit'] = .false  -- we need this for nested calls to this method
return


/*============================================================================*/
/* Class:Widget                                                               */
/*============================================================================*/

::class GtkWidget public

::method init
use strict arg
self~pointer = 0
self~access_obj = .NIL
return

::method uninit
-- don't destroy widgets by default!
return

::method destroy
-- destroy the widget
use strict arg
if self~pointer <> 0 then call GrxWidgetDestroy self~pointer
-- this ensures that if we try to reuse the widget we will get an error
self~pointer = 0
return

::method font
-- get the display font for the widget
use strict arg
return GrxWidgetGetFont(self~pointer)

::method 'font='
-- set the display font for the widget
use strict arg fontname
call GrxWidgetSetFont self~pointer, fontname
return

::method grabfocus
-- set the keyboard focus to this widget
use strict arg
call GrxWidgetGrabFocus self~pointer
return

::method hide
-- hide the widget (default when created is hidden)
use strict arg
call GrxWidgetHide self~pointer
return

::method hideall
-- hide the widget and all children (default when created is hidden)
use strict arg
call GrxWidgetHideAll self~pointer
return

::method pointer
-- return the pointer to the widget
expose pointer
use strict arg
return pointer

::method 'pointer=' private
-- set the pointer to the widget
expose pointer
use strict arg ptr
pointer = ptr
.local['GTK_Database']~put(self, upper(pointer))
return

::method setsize
-- all widgets are created with a default size of (0,0) so they are self-sizing
use strict arg width, height
call GrxWidgetSetSize self~pointer, width, height
return

::method show
-- show the widget (default when created is hidden)
use strict arg
call GrxWidgetShow self~pointer
return

::method showall
-- show the widget and all children (default when created is hidden)
use strict arg
call GrxWidgetShowAll self~pointer
return

::method access
-- get the accessibility object
expose access_obj
use strict arg
if access_obj = .NIL then do
   access_ptr = GrxWidgetGetAccessible(self~pointer)
   if access_ptr = 0 then return .NIL
   access_obj = .AtkObject(access_ptr)
   end
return access_obj

::method access_obj private
-- get the accessibility object
expose access_obj
use strict arg
return access_obj

::method 'access_obj=' private
-- set the accessibility object
expose access_obj
use strict arg access_obj
return

::method name
-- get the widget name
use strict arg
return GrxWidgetGetName(self~pointer)

::method 'name=' private
-- set widget name
use strict arg newname
call GrxWidgetSetName self~pointer, newname
return

::method 'sensitive=' private
-- set widget sensitivity (enable/disable the widget for input)
use strict arg flag
if flag <> .false then flag = .true
call GrxWidgetSetSensitive self~pointer, flag
return

::method run_method
use strict arg meth, methargs = ''
meth_inst = self~instanceMethod(meth)
if meth_inst <> .nil then do
   self~run(meth_inst, 'I', methargs)
   end
return

::method connect_signal
use strict arg event
select
   when event = 'destroy' then call GrxWidgetConnectSignal self~pointer, event
   otherwise nop
   end
return

::method signal_destroy
return


/*============================================================================*/
/* Class: Container                                                           */
/*============================================================================*/

::class GtkContainer public subclass GtkWidget

::method init
use strict arg
return

::method add
-- add an object to the container
use strict arg mywidget
call GrxContainerAdd self~pointer, mywidget~pointer
return

::method remove
-- remove an object from the container
use strict arg mywidget
call GrxContainerRemove self~pointer, mywidget~pointer
return

::method 'borderwidth='
-- set the amount of blank space around the objects in the container
use strict arg width
call GrxContainerSetBorderWidth self~pointer, width
return

::method connect_signal
use strict arg event
select
   when event = 'add' then call GrxContainerConnectSignal self~pointer, event
   when event = 'check_resize' then call GrxContainerConnectSignal self~pointer, event
   when event = 'removs' then call GrxContainerConnectSignal self~pointer, event
   when event = 'set_focus_child' then call GrxContainerConnectSignal self~pointer, event
   otherwise self~connect_signal:super(event)
   end
return

::method signal_add
return

::method signal_check_resize
return

::method signal_remove
return

::method signal_set_focus_child
return


/*----------------------------------------------------------------------------*/
/* Box class                                                                  */
/*----------------------------------------------------------------------------*/

::class GtkBox public subclass GtkContainer

::method init
if arg() > 0 then raise syntax 40.4 array ('INIT', 0)
return

::method pack_end
-- insert an object into the box with attributes
use strict arg mywidget, expand, fill, padding
call GrxBoxPackEnd self~pointer, mywidget~pointer, expand, fill, padding
return

::method pack_end_defaults
-- insert an object into the box with default attributes
use strict arg mywidget
self~pack_end(mywidget, .true, .true, 0)
return

::method pack_start
-- insert an object into the box with attributes
use strict arg mywidget, expand, fill, padding
call GrxBoxPackStart self~pointer, mywidget~pointer, expand, fill, padding
return

::method pack_start_defaults
-- insert an object into the box with default attributes
use strict arg mywidget
self~pack_start(mywidget, .true, .true, 0)
return


/*----------------------------------------------------------------------------*/
/* VBox class                                                                 */
/*----------------------------------------------------------------------------*/

::class GtkVBox public subclass GtkBox

::method init
if arg() = 0 then self~pointer = GrxVBoxNew()
else use strict arg self~pointer
return


/*----------------------------------------------------------------------------*/
/* HBox class                                                                 */
/*----------------------------------------------------------------------------*/

::class GtkHBox public subclass GtkBox

::method init
if arg() = 0 then self~pointer = GrxHBoxNew()
else use strict arg self~pointer
return


/*============================================================================*/
/* Class: Window                                                              */
/*============================================================================*/

::class GtkWindow public subclass GtkContainer

::method init
self~pointer = GrxWindowNew()
use strict arg self~title = ''
return

::method uninit
-- Top windows are destroyed when closed
if self~pointer <> 0 then self~destroy()
return

::method 'title='
-- set the window's title
expose title
use strict arg newtitle
call GrxWindowSetTitle self~pointer, newtitle
title = newtitle
return

::method start
-- run the window in a background thread
use strict arg
retc = GrxWindowStart(self~pointer)
return retc

::method connect_signal
use strict arg event
select
   when event = 'set_focus' then call GrxWindowConnectSignal self~pointer, event
   otherwise self~connect_signal:super(event)
   end
return

::method signal_activat_default
return

::method signal_activat_focus
return

::method signal_keys_changed
return

::method signal_set_focus
return


/*============================================================================*/
/* Label class                                                                */
/*============================================================================*/

::class GtkLabel public subclass GtkWidget

::method init
expose justify
if arg() > 1 then raise syntax 40.4 array ('INIT', 1)
self~pointer = GrxLabelNew()
use strict arg text = ''
self~text = text
justify = 'GTK_JUSTIFY_CENTER'
return

::method justify
-- get the label text justification
expose justify
return justify

::method 'justify='
-- set the label text justification
expose justify
use strict arg justify
call GrxLabelSetJustify self~pointer, translate(justify)
return

::method 'text='
-- set the label text
use strict arg text
call GrxLabelSetText self~pointer, text
return

::method 'selectable='
-- set the label text
use strict arg flag
call GrxLabelSetSelectable self~pointer, flag
return

/*============================================================================*/
/* Button class                                                               */
/*============================================================================*/

::class GtkButton public subclass GtkContainer

::method init
self~pointer = GrxButtonNew()
use strict arg self~label = ''
return

::method label
-- return the button's label
use strict arg
return GrxButtonGetLabel(self~pointer)

::method 'label='
-- set the button's label
use strict arg label
call GrxButtonSetLabel self~pointer, label
return

::method 'use_underline='
-- set the button to usemnemonics
use strict arg flag
call GrxButtonUseUnderline self~pointer, flag
return

::method connect_signal
use strict arg event
select
   when event = 'pressed' then call GrxButtonConnectSignal self~pointer, event
   when event = 'released' then call GrxButtonConnectSignal self~pointer, event
   when event = 'clicked' then call GrxButtonConnectSignal self~pointer, event
   when event = 'enter' then call GrxButtonConnectSignal self~pointer, event
   when event = 'leave' then call GrxButtonConnectSignal self~pointer, event
   otherwise self~connect_signal:super(event)
   end
return

::method signal_pressed
return

::method signal_released
return

::method signal_clicked
return

::method signal_enter
return

::method signal_leave
return


/*============================================================================*/
/* Checkbutton class                                                          */
/*============================================================================*/

::class GtkCheckbutton public subclass GtkButton

::method init
self~pointer = GrxCheckButtonNew()
   use strict arg self~label = ''
return

::method state
-- is the button checked?
use strict arg
return GrxCheckButtonGetState(self~pointer)

::method 'state='
-- set the checked state
use strict arg state
return GrxCheckButtonSetState(self~pointer, state)

::method connect_signal
use strict arg event
select
   when event = 'pressed' then call GrxButtonConnectSignal self~pointer, event
   when event = 'released' then call GrxButtonConnectSignal self~pointer, event
   when event = 'clicked' then call GrxButtonConnectSignal self~pointer, event
   when event = 'enter' then call GrxButtonConnectSignal self~pointer, event
   when event = 'leave' then call GrxButtonConnectSignal self~pointer, event
   otherwise self~connect_signal:super(event)
   end
return

::method signal_pressed
return

::method signal_released
return

::method signal_clicked
return

::method signal_enter
return

::method signal_leave
return


/*============================================================================*/
/* Radiobutton class                                                          */
/*============================================================================*/

::class GtkRadiobutton public subclass GtkCheckbutton

::method init
use strict arg group, label = ''
if label <> '' then self~pointer =GrxRadioButtonNew(group~head, label)
else self~pointer =GrxRadioButtonNew(group~head)
group~head = GrxRadioButtonGroup(self~pointer);
return

::method connect_signal
use strict arg event
select
   when event = 'pressed' then call GrxButtonConnectSignal self~pointer, event
   when event = 'released' then call GrxButtonConnectSignal self~pointer, event
   when event = 'clicked' then call GrxButtonConnectSignal self~pointer, event
   when event = 'enter' then call GrxButtonConnectSignal self~pointer, event
   when event = 'leave' then call GrxButtonConnectSignal self~pointer, event
   otherwise self~connect_signal:super(event)
   end
return

::method signal_pressed
return

::method signal_released
return

::method signal_clicked
return

::method signal_enter
return

::method signal_leave
return


