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

::method set_font
-- set the display font for the widget
use strict arg self~font
return

::method grab_focus
-- set the keyboard focus to this widget
use strict arg
call GrxWidgetGrabFocus self~pointer
return

::method hide
-- hide the widget (default when created is hidden)
use strict arg
call GrxWidgetHide self~pointer
return

::method hide_all
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

::method set_size_request
-- all widgets are created with a default size of (0,0) so they are self-sizing
use strict arg width, height
call GrxWidgetSetSize self~pointer, width, height
return

::method show
-- show the widget (default when created is hidden)
use strict arg
call GrxWidgetShow self~pointer
return

::method show_all
-- show the widget and all children (default when created is hidden)
use strict arg
call GrxWidgetShowAll self~pointer
return

::method accessible
-- get the accessibility object
expose access_obj
use strict arg
if access_obj = .NIL then do
   access_ptr = GrxWidgetGetAccessible(self~pointer)
   if access_ptr = 0 then return .NIL
   access_obj = .AtkObject(access_ptr)
   end
return access_obj

::method get_accessible
-- get the accessibility object
return self~accessible

::method accessible_obj private
-- get the accessibility object
expose access_obj
use strict arg
return access_obj

::method 'accessible_obj=' private
-- set the accessibility object
expose access_obj
use strict arg access_obj
return

::method name
-- get the widget name
use strict arg
return GrxWidgetGetName(self~pointer)

::method get_name
-- get the widget name
use strict arg
return self~name

::method 'name=' private
-- set widget name
use strict arg newname
call GrxWidgetSetName self~pointer, newname
return

::method set_name private
-- set widget name
use strict arg self~name
return

::method 'sensitive='
-- set widget sensitivity (enable/disable the widget for input)
use strict arg flag
call GrxWidgetSetSensitive self~pointer, flag
return

::method set_sensitive
-- set widget sensitivity (enable/disable the widget for input)
use strict arg self~sensitive
return

::method run_method
use strict arg meth, methargs = ''
meth_inst = self~instanceMethod(meth)
if meth_inst <> .nil then do
   self~run(meth_inst, 'I', methargs)
   end
return

::attribute user_data
-- This attribute is not a part of the GTK system. It has been added to
-- support callbacks or any other use the user may have.

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
raise syntax 93.964 array ('A GtkContainer class cannot be instantiated.')
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

::method 'border_width='
-- set the amount of blank space around the objects in the container
use strict arg width
call GrxContainerSetBorderWidth self~pointer, width
return

::method set_border_width
use strict arg self~border_width
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


/*============================================================================*/
/* Class: Box                                                                 */
/*============================================================================*/

::class GtkBox public subclass GtkContainer

::method init
raise syntax 93.964 array ('A GtkBox class cannot be instantiated.')
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

::method connect_signal
use strict arg event
self~connect_signal:super(event)
return


/*============================================================================*/
/* Class: VBox                                                                */
/*============================================================================*/

::class GtkVBox public subclass GtkBox

::method init
use arg homogeneous, spacing
self~pointer = GrxVBoxNew(homogeneous, spacing)
return

::method connect_signal
use strict arg event
self~connect_signal:super(event)
return


/*============================================================================*/
/* Class: HBox                                                                */
/*============================================================================*/

::class GtkHBox public subclass GtkBox

::method init
use arg homogeneous, spacing
self~pointer = GrxHBoxNew(homogeneous, spacing)
return

::method connect_signal
use strict arg event
self~connect_signal:super(event)
return


/*============================================================================*/
/* Class: Paned                                                               */
/*============================================================================*/

::class GtkPaned public subclass GtkContainer

::method init
raise syntax 93.964 array ('A GtkPaned class cannot be instantiated.')
return

::method add1
-- add an object to the paned
use strict arg mywidget
call GrxPanedAdd1 self~pointer, mywidget~pointer
return

::method add2
-- add an object to the paned
use strict arg mywidget
call GrxPanedAdd2 self~pointer, mywidget~pointer
return

::method pack1
-- pack an object in the paned
use strict arg mywidget, resize, shrink
call GrxPanedPack1 self~pointer, mywidget~pointer, resize, shrink
return

::method pack2
-- pack an object in the paned
use strict arg mywidget, resize, shrink
call GrxPanedPack2 self~pointer, mywidget~pointer, resize, shrink
return

::method connect_signal
use strict arg event
self~connect_signal:super(event)
return


/*============================================================================*/
/* Class: HPaned                                                              */
/*============================================================================*/

::class GtkHPaned public subclass GtkPaned

::method init
if arg() = 0 then self~pointer = GrxHPanedNew()
else use strict arg self~pointer
return

::method connect_signal
use strict arg event
self~connect_signal:super(event)
return


/*============================================================================*/
/* Class: VPaned                                                              */
/*============================================================================*/

::class GtkVPaned public subclass GtkPaned

::method init
if arg() = 0 then self~pointer = GrxVPanedNew()
else use strict arg self~pointer
return

::method connect_signal
use strict arg event
self~connect_signal:super(event)
return


/*============================================================================*/
/* Class: Window                                                              */
/*============================================================================*/

::class GtkWindow public subclass GtkContainer

::method init
use strict arg type
self~pointer = GrxWindowNew(type)
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

::method set_title
use strict arg self~title
return

::method start
-- run the window in a background thread
use strict arg
retc = GrxWindowStart(self~pointer)
return retc

::method connect_signal
use strict arg event
select
   when event = 'activate_default' then call GrxWindowConnectSignal self~pointer, event
   when event = 'activate_focus' then call GrxWindowConnectSignal self~pointer, event
   when event = 'keys_changed' then call GrxWindowConnectSignal self~pointer, event
   when event = 'set_focus' then call GrxWindowConnectSignal self~pointer, event
   otherwise self~connect_signal:super(event)
   end
return

::method signal_activate_default
return

::method signal_activate_focus
return

::method signal_keys_changed
return

::method signal_set_focus
return


/*============================================================================*/
/* Class: Label                                                               */
/*============================================================================*/

::class GtkLabel public subclass GtkWidget

::method init
expose justify
self~pointer = GrxLabelNew()
use strict arg text = ''
self~text = text
justify = 'GTK_JUSTIFY_CENTER'
return

::method justify
-- get the label text justification
expose justify
return justify

::method get_justify
-- get the label text justification
expose justify
return justify

::method 'justify='
-- set the label text justification
expose justify
use strict arg justify
call GrxLabelSetJustify self~pointer, translate(justify)
return

::method set_justify
-- set the label text justification
use strict arg self~justify
return

::method 'text='
-- set the label text
use strict arg text
call GrxLabelSetText self~pointer, text
return

::method set_text
-- set the label text
use strict arg self~text
return

::method 'selectable='
-- set the label text
use strict arg flag
call GrxLabelSetSelectable self~pointer, flag
return

::method set_selectable
-- set the label text
use strict arg self~selectable
return

::method connect_signal
use strict arg event
self~connect_signal:super(event)
return

/*============================================================================*/
/* Class: Button                                                              */
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

::method get_label
-- return the button's label
use strict arg
return self~label

::method 'label='
-- set the button's label
use strict arg label
call GrxButtonSetLabel self~pointer, label
return

::method set_label
-- set the button's label
use strict arg self~label
return

::method 'use_underline='
-- set the button to usemnemonics
use strict arg flag
call GrxButtonUseUnderline self~pointer, flag
return

::method set_use_underline
-- set the button to usemnemonics
use strict arg self~use_underline
return

::method 'relief='
-- set the button relief style
use strict arg relief
call GrxButtonSetRelief self~pointer, relief
return

::method set_relief
-- set the button relief style
use strict arg self~relief
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
/* Class: Button_With_Mnemonic                                                */
/*============================================================================*/

::class GtkButton_With_Mnemonic public subclass GtkButton

::method init
self~init:super
use arg label = ''
self~use_underline = .true
self~label = label
return

/*============================================================================*/
/* Class: Button_With_Label                                                   */
/*============================================================================*/

::class GtkButton_With_Label public subclass GtkButton

::method init
self~init:super
use arg label = ''
self~label = label
return

/*============================================================================*/
/* Class: Button_FromStock                                                    */
/*============================================================================*/

::class GtkButton_From_Stock public subclass GtkButton

::method init
use strict arg stockid
self~pointer = GrxButtonNewFromStock(stockid)
if self~pointer = 0 then do
   raise syntax 93.900 array ('A button with the spcified stockid was not found.')
   end
return


/*============================================================================*/
/* Class: ToggleButton                                                         */
/*============================================================================*/

::class GtkToggleButton public subclass GtkButton

::method init
self~pointer = GrxToggleButtonNew()
use strict arg self~label = ''
return

::method mode
-- is the button display option
use strict arg
return GrxToggleButtonGetMode(self~pointer)

::method get_mode
-- is the button display option
use strict arg
return self~mode

::method 'mode='
-- set the checked state
use strict arg mode
call GrxToggleButtonSetMode self~pointer, mode
return

::method set_mode
-- set the checked state
use strict arg self~mode
return

::method active
-- is the button display option
use strict arg
return GrxToggleButtonGetActive(self~pointer)

::method get_active
-- is the button display option
use strict arg
return self~active

::method 'active='
-- set the checked state
use strict arg active
call GrxToggleButtonSetActive self~pointer, mode
return

::method set_active
-- set the checked state
use strict arg self~active
return

::method inconsistent
-- is the button display option
use strict arg
return GrxToggleButtonGetInconsistent(self~pointer)

::method get_inconsistent
-- is the button display option
use strict arg
return self~inconsistent

::method 'inconsistent='
-- set the checked state
use strict arg inconsistent
call GrxToggleButtonSetInconsistent self~pointer, mode
return

::method set_inconsistent
-- set the checked state
use strict arg self~inconsistent
return

::method connect_signal
use strict arg event
select
   when event = 'toggled' then call GrxToggleButtonConnectSignal self~pointer, event
   otherwise self~connect_signal:super(event)
   end
return

::method signal_toggled
return

/*============================================================================*/
/* Class: ToggleButton_With_Mnemonic                                          */
/*============================================================================*/

::class GtkToggleButton_With_Mnemonic public subclass GtkToggleButton

::method init
self~init:super
use arg label = ''
self~use_underline = .true
self~label = label
return

/*============================================================================*/
/* Class: ToggleButton_With_Label                                             */
/*============================================================================*/

::class GtkToggleButton_With_Label public subclass GtkToggleButton

::method init
self~init:super
use arg label = ''
self~label = label
return


/*============================================================================*/
/* Class: Checkbutton                                                         */
/*============================================================================*/

::class GtkCheckbutton public subclass GtkToggleButton

::method init
self~pointer = GrxCheckButtonNew()
use strict arg self~label = ''
return

::method connect_signal
use strict arg event
self~connect_signal:super(event)
return

/*============================================================================*/
/* Class: CheckButton_With_Mnemonic                                           */
/*============================================================================*/

::class GtkCheckButton_With_Mnemonic public subclass GtkCheckButton

::method init
self~init:super
use arg label = ''
self~use_underline = .true
self~label = label
return

/*============================================================================*/
/* Class: CheckButton_With_Label                                              */
/*============================================================================*/

::class GtkCheckButton_With_Label public subclass GtkCheckButton

::method init
self~init:super
use arg label = ''
self~label = label
return


/*============================================================================*/
/* Class: Radiobutton                                                         */
/*============================================================================*/

::class GtkRadiobutton public subclass GtkCheckbutton

::method init
use strict arg group
self~pointer =GrxRadioButtonNew(group~head)
return

::method group
use strict arg
return GrxRadioButtonGetGroup(self~pointer);

::method get_group
use strict arg
return self~group

::method 'group='
use strict arg group
call GrxRadioButtonSetGroup self~pointer, group~head
return

::method set_group
use strict arg self~group
return

::method connect_signal
use strict arg event
select
   when event = 'group_changed' then call GrxRadioButtonConnectSignal self~pointer, event
   otherwise self~connect_signal:super(event)
   end
return

::method signal_group_changed
return

/*============================================================================*/
/* Class: RadioButton_With_Mnemonic                                           */
/*============================================================================*/

::class GtkRadioButton_With_Mnemonic public subclass GtkRadioButton

::method init
use arg group, label = ''
self~init:super(group)
self~use_underline = .true
self~label = label
return

/*============================================================================*/
/* Class: RadioButton_With_Label                                              */
/*============================================================================*/

::class GtkRadioButton_With_Label public subclass GtkRadioButton

::method init
use arg group, label = ''
self~init:super(group)
self~label = label
return


/*============================================================================*/
/* Class: Table                                                               */
/*============================================================================*/

::class GtkTable public subclass GtkContainer

::method init
use strict arg rows, cols, homogeneous
self~pointer = GrxTableNew(rows, cols, homogeneous)
return

::method attach
use strict arg widget, left, right, top, bottom, xopt, yopt, xpad, ypad
call GrxTableAttach self~pointer, widget~pointer, left, right, top, bottom,,
                    xopt, yopt, xpad, ypad
return

::method set_row_spacings
use strict arg spacing
call GrxTableSetRowSpacings self~pointer, spacing
return

::method set_col_spacings
use strict arg spacing
call GrxTableSetColSpacings self~pointer, spacing
return

::method connect_signal
use strict arg event
self~connect_signal:super(event)
return


/*============================================================================*/
/* Class: Entry                                                               */
/*============================================================================*/

::class GtkEntry public subclass GtkWidget

::method init
use strict arg
self~pointer = GrxEntryNew()
return

::method max_length
-- get the max length of text in the object
use strict arg
return GrxEntryGetMaxLength(self~pointer)

::method get_max_length
-- get the max length of text in the object
use strict arg
return self~max_length

::method 'max_length='
-- set the max length of text in the object
use strict arg len
call GrxEntrySetMaxLength self~pointer, len
return

::method set_max_length
use strict arg self~max_length
return

::method 'width_chars='
-- set the visible width of the object in characters
use strict arg width
call GrxEntrySetWidthChars self~pointer, width
return

::method set_width_chars
-- set the visible width of the object in characters
use arg self~width_chars
return

::method text
-- get the text in the object
use strict arg
return GrxEntryGetText(self~pointer)

::method 'text='
-- set the text in the object
use strict arg text
call GrxEntrySetText self~pointer, text
return

::method set_text
-- set the text in the object
use strict arg self~text
return

::method connect_signal
use strict arg event
select
   when event = 'activate' then call GrxEntryConnectSignal self~pointer, event
   when event = 'backspace' then call GrxEntryConnectSignal self~pointer, event
   when event = 'copy_clipboard' then call GrxEntryConnectSignal self~pointer, event
   when event = 'cut_clipboard' then call GrxEntryConnectSignal self~pointer, event
   when event = 'delete_from_cursor' then call GrxEntryConnectSignal self~pointer, event
   when event = 'insert_at_cursor' then call GrxEntryConnectSignal self~pointer, event
   when event = 'move_cursor' then call GrxEntryConnectSignal self~pointer, event
   when event = 'paste_clipboard' then call GrxEntryConnectSignal self~pointer, event
   when event = 'populate_popup' then call GrxEntryConnectSignal self~pointer, event
   when event = 'toggle_overwrite' then call GrxEntryConnectSignal self~pointer, event
   otherwise self~connect_signal:super(event)
   end
return

::method signal_activate
return

::method signal_backspace
return

::method signal_copy_clipboard
return

::method signal_cut_clipboard
return

::method signal_delete_from_cursor
return

::method signal_insert_at_cursor
return

::method signal_move_cursor
return

::method signal_paste_clipboard
return

::method signal_populate_popup
return

::method signal_toggle_overwrite
return


/*============================================================================*/
/* Class: Fixed                                                               */
/*============================================================================*/

::class GtkFixed public subclass GtkContainer

::method init
use strict arg
self~pointer = GrxFixedNew()
return

::method put
use strict arg widget, xpos, ypos
call GrxFixedPut self~pointer, widget~pointer, xpos, ypos
return

::method move
use strict arg widget, xpos, ypos
call GrxFixedMove self~pointer, widget~pointer, xpos, ypos
return

::method connect_signal
use strict arg event
self~connect_signal:super(event)
return


/*============================================================================*/
/* Class: Expander                                                            */
/*============================================================================*/

::class GtkExpander public subclass GtkContainer

::method init
use strict arg label
self~pointer = GrxExpanderNew(label)
return

::method 'expanded='
use strict arg flag
call GrxExpanderSetExpanded self~pointer, flag
return

::method set_expanded
use strict arg self~expanded
return

::method label
use strict arg
call GrxExpanderGetLabel self~pointer
return

::method get_label
use strict arg
return self~label

::method 'label='
use strict arg label
call GrxExpanderSetLabel self~pointer, label
return

::method set_label
use strict arg self~label
return

::method 'use_underline='
use strict arg flag
call GrxExpanderUseUnderline self~pointer, flag
return

::method set_use_underline
use strict arg self~use_underline
return

::method 'spacing='
use strict arg spacing
call GrxExpanderSetSpacing self~pointer, spacing
return

::method set_spacing
use strict arg self~spacing
return

::method connect_signal
use strict arg event
self~connect_signal:super(event)
return


/*============================================================================*/
/* Class: Expander_With_Mnemonic                                              */
/*============================================================================*/

::class GtkExpander_With_Mnemonic public subclass GtkExpander

::method init
use arg label = ''
self~init:super(label)
self~use_underline = .true
return


/*============================================================================*/
/* Class: HandleBox                                                           */
/*============================================================================*/

::class GtkHandleBox public subclass GtkContainer

::method init
use strict arg
self~pointer = GrxHandleBoxNew()
return

::method 'shadow_type='
use strict arg type
call GrxHandleBoxSetShadowType self~pointer, type
return

::method set_shadow_type
use strict arg self~shadow_type
return

::method 'handle_position='
use strict arg type
call GrxHandleBoxSetHandlePosition self~pointer, type
return

::method set_handle_position
use strict arg self~handle_position
return

::method 'snap_edge='
use strict arg type
call GrxHandleBoxSetSnapEdge self~pointer, type
return

::method set_snap_edge
use strict arg self~snap_edge
return

::method connect_signal
use strict arg event
self~connect_signal:super(event)
return


/*============================================================================*/
/* Class: Notebook                                                            */
/*============================================================================*/

::class GtkNotebook public subclass GtkContainer

::method init
use strict arg
self~pointer = GrxNotebookNew()
return

::method append_page
-- append a page
use strict arg page, label
if label = .nil then labelptr = 0
else labelptr = label~pointer
call GrxNotebookAppendPage self~pointer, page~pointer, labelptr
return

::method prepend_page
-- prepend a page
use strict arg page, label
if label = .nil then labelptr = 0
else labelptr = label~pointer
call GrxNotebookPrependPage self~pointer, page~pointer, labelptr
return

::method insert_page
-- insert a page
use strict arg page, label, position
if label = .nil then labelptr = 0
else labelptr = label~pointer
call GrxNotebookInsertPage self~pointer, page~pointer, labelptr, position
return

::method append_page_menu
-- append a page with a menu
use strict arg page, label, menu
if label = .nil then labelptr = 0
else labelptr = label~pointer
if menu = .nil then menuptr = 0
else menuptr = menu~pointer
call GrxNotebookAppendPageMenu self~pointer, page~pointer, labelptr, menuptr
return

::method prepend_page_menu
-- prepend a page with a menu
use strict arg page, label, menu
if label = .nil then labelptr = 0
else labelptr = label~pointer
if menu = .nil then menuptr = 0
else menuptr = menu~pointer
call GrxNotebookPrependPageMenu self~pointer, page~pointer, labelptr, menuptr
return

::method insert_page_menu
-- insert a page with a menu
use strict arg page, label, menu, position
if label = .nil then labelptr = 0
else labelptr = label~pointer
if menu = .nil then menuptr = 0
else menuptr = menu~pointer
call GrxNotebookInsertPageMenu self~pointer, page~pointer, labelptr, menuptr
return

::method remove_page
-- remove a page
use strict arg position
call GrxNotebookRemovePage self~pointer, position
return

::method 'show_border='
-- set the display of a border
use strict arg toggle
call GrxNotebookSetShowBorder self~pointer, toggle
return

::method set_show_border
-- set the display of a border
use strict arg self~show_border
return

::method 'show_tabs='
-- set the display of the tabs
use strict arg toggle
call GrxNotebookSetShowTabs self~pointer, toggle
return

::method set_show_tabs
-- set the display of the tabs
use strict arg self~show_tabs
return

::method 'tab_pos='
-- set the display position of the tabs
use strict arg position
call GrxNotebookSetTabPos self~pointer, position~translate()
return

::method set_tab_pos
-- set the display position of the tabs
use strict arg self~tab_pos
return

::method 'homogeneous_tabs='
-- set the display of same-sized tabs
use strict arg toggle
call GrxNotebookSetHomogeneousTabs self~pointer, toggle
return

::method set_homogeneous_tabs
-- set the display of same-sized tabs
use strict arg self~homogeneous_tabs
return

::method 'tab_border='
-- set the border size of the tabs
use strict arg size
if size < 0 then size = 2 /* 2 is the GTK+ default */
call GrxNotebookSetTabBorder self~pointer, size
return

::method set_tab_border
-- set the border size of the tabs
use strict arg self~tab_border
return

::method 'scrollable='
-- set the ability to scroll the tabs
use strict arg toggle
call GrxNotebookSetScrollable self~pointer, toggle
return

::method set_scrollable
-- set the ability to scroll the tabs
use strict arg self~set_scrollable
return

::method set_popup_enable
-- enable the display of popup menus
use strict arg
call GrxNotebookPopupEnable self~pointer
return

::method set_popup_disable
-- disable the display of popup menus
use strict arg
call GrxNotebookPopupDisable self~pointer
return

::method 'current_page='
-- set the current notebook page
use strict arg page
call GrxNotebookSetCurrentPage self~pointer, page
return

::method set_current_page
-- set the current notebook page
use strict arg self~current_page
return

::method current_page
-- get the current notebook page
use strict arg
return GrxNotebookGetCurrentPage(self~pointer)

::method get_current_page
-- get the current notebook page
use strict arg
return self~current_page

::method connect_signal
use strict arg event
select
   when event = 'change_current_page' then call GrxLabelConnectSignal self~pointer, event
   when event = 'create_window' then call GrxLabelConnectSignal self~pointer, event
   when event = 'focus_tab' then call GrxLabelConnectSignal self~pointer, event
   when event = 'move_focus_out' then call GrxLabelConnectSignal self~pointer, event
   when event = 'page_added' then call GrxLabelConnectSignal self~pointer, event
   when event = 'page_removed' then call GrxLabelConnectSignal self~pointer, event
   when event = 'page_reordered' then call GrxLabelConnectSignal self~pointer, event
   when event = 'reorder_tab' then call GrxLabelConnectSignal self~pointer, event
   when event = 'select_page' then call GrxLabelConnectSignal self~pointer, event
   when event = 'switch_page' then call GrxLabelConnectSignal self~pointer, event
   otherwise self~connect_signal:super(event)
   end
return

::method signal_change_current_page
return

::method signal_create_window
return

::method signal_focus_tab
return

::method signal_move_focus_out
return

::method signal_page_added
return

::method signal_page_removed
return

::method signal_page_reordered
return

::method signal_reorder_tab
return

::method signal_select_tab
return

::method signal_switch_page
return


/*============================================================================*/
/* Class: EventBox                                                            */
/*============================================================================*/

::class GtkEventBox public subclass GtkContainer

::method init
use strict arg
self~pointer = GrxEventBoxNew()
return

::method 'above_child='
use strict arg flag
call GrxEventBoxSetAboveChild self~pointer, flag
return

::method set_above_child
use strict arg self~shadow_type
return

::method above_child
use strict arg
return GrxEventBoxGetAboveChild(self~pointer)

::method get_above_child
use strict arg
return self~above_child

::method 'visible_window='
use strict arg flag
call GrxEventBoxSetVisibleWindow self~pointer, flag
return

::method set_visible_window
use strict arg self~visible_window
return

::method visible_window
use strict arg
return GrxEventBoxGetVisibleWindow(self~pointer)

::method get_visible_window
use strict arg
return self~visible_window

::method connect_signal
use strict arg event
self~connect_signal:super(event)
return

