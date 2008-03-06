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
   call gtk_main_iteration
   end
.local['GTK_Quit'] = .false  -- we need this for recursive calls to this function
return


/*============================================================================*/
/* Routine: gtk_main_quit                                                     */
/*============================================================================*/

::routine gtk_main_quit public
.local['GTK_Quit'] = .true
return


/*============================================================================*/
/* Routine: gtk_events_pending                                                */
/*============================================================================*/

::routine gtk_events_pending public
return GrxEventsPending()


/*============================================================================*/
/* Routine: gtk_main_iteration                                                */
/*============================================================================*/

::routine gtk_main_iteration public
call GrxMainIteration
old_queue = RxQueue('Set', .local['GTK_Queue'])
do while queued() > 0
   pull pointer msg args
-- FIXME! remove the comment from this section of code before releasing
-- remove the following statement before the final release!
   say pointer msg args
   widget = .local['GTK_Database']~at(pointer)
   if widget <> .nil then do
      widget~run_method(msg, args)
      end
   end
call RxQueue 'Set', old_queue
return


/*============================================================================*/
/* Routine: String2Value                                                      */
/*============================================================================*/

::routine String2Value public
use strict arg group, str
return GrxConstantString2Value(upper(group), upper(str))


/*============================================================================*/
/* Routine: Value2String                                                      */
/*============================================================================*/

::routine Value2String public
use strict arg group, num
return GrxValue2ConstantString(upper(group), num)


/*============================================================================*/
/* Routine: StockID                                                           */
/*============================================================================*/

::routine StockID public
use strict arg id
sid = upper(id)
select
   when sid = 'GTK_STOCK_ABOUT' then return 'gtk-about'
   when sid = 'GTK_STOCK_ADD' then return 'gtk-add'
   when sid = 'GTK_STOCK_APPLY' then return 'gtk-apply'
   when sid = 'GTK_STOCK_BOLD' then return 'gtk-bold'
   when sid = 'GTK_STOCK_CANCEL' then return 'gtk-cancel'
   when sid = 'GTK_STOCK_CDROM' then return 'gtk-cdrom'
   when sid = 'GTK_STOCK_CLEAR' then return 'gtk-clear'
   when sid = 'GTK_STOCK_CLOSE' then return 'gtk-close'
   when sid = 'GTK_STOCK_COLOR_PICKER' then return 'gtk-color-picker'
   when sid = 'GTK_STOCK_CONVERT' then return 'gtk-comvert'
   when sid = 'GTK_STOCK_CONNECT' then return 'gtk-connect'
   when sid = 'GTK_STOCK_COPY' then return 'gtk-copy'
   when sid = 'GTK_STOCK_CUT' then return 'gtk-cut'
   when sid = 'GTK_STOCK_DELETE' then return 'gtk-delete'
   when sid = 'GTK_STOCK_DIALOG_ERROR' then return 'gtk-dialog-error'
   when sid = 'GTK_STOCK_DIALOG_INFO' then return 'gtk-dialog-info'
   when sid = 'GTK_STOCK_DIALOG_QUESTION' then return 'gtk-dialog-question'
   when sid = 'GTK_STOCK_DIALOG_WARNING' then return 'gtk-dialog-warning'
   when sid = 'GTK_STOCK_DIRECTORY' then return 'gtk-directry'
   when sid = 'GTK_STOCK_DISCARD' then return 'gtk-discard'
   when sid = 'GTK_STOCK_DISCONNECT' then return 'gtk-disconnect'
   when sid = 'GTK_STOCK_DND' then return 'gtk-dnd'
   when sid = 'GTK_STOCK_DND_MULTIPLE' then return 'gtk-dnd-multiple'
   when sid = 'GTK_STOCK_EDIT' then return 'gtk-edit'
   when sid = 'GTK_STOCK_EXECUTE' then return 'gtk-execute'
   when sid = 'GTK_STOCK_FILE' then return 'gtk-file'
   when sid = 'GTK_STOCK_FIND' then return 'gtk-find'
   when sid = 'GTK_STOCK_FIND_AND_REPLACE' then return 'gtk-find-and-replace'
   when sid = 'GTK_STOCK_FLOPPY' then return 'gtk-floppy'
   when sid = 'GTK_STOCK_FULLSCREEN' then return 'gtk-fullscreen'
   when sid = 'GTK_STOCK_GOTO_BOTTOM' then return 'gtk-goto-bottom'
   when sid = 'GTK_STOCK_GOTO_FIRST' then return 'gtk-goto-first'
   when sid = 'GTK_STOCK_GOTO_LAST' then return 'gtk-goto-last'
   when sid = 'GTK_STOCK_GOTO_TOP' then return 'gtk-goto-top'
   when sid = 'GTK_STOCK_GO_BACK' then return 'gtk-go-back'
   when sid = 'GTK_STOCK_GO_DOWN' then return 'gtk-go-down'
   when sid = 'GTK_STOCK_GO_FORWARD' then return 'gtk-go-forward'
   when sid = 'GTK_STOCK_GO_UP' then return 'gtk-go-up'
   when sid = 'GTK_STOCK_HARDDISK' then return 'gtk-harddisk'
   when sid = 'GTK_STOCK_HELP' then return 'gtk-help'
   when sid = 'GTK_STOCK_HOME' then return 'gtk-home'
   when sid = 'GTK_STOCK_INDENT' then return 'gtk-indent'
   when sid = 'GTK_STOCK_INDEX' then return 'gtk-index'
   when sid = 'GTK_STOCK_INFO' then return 'gtk-info'
   when sid = 'GTK_STOCK_ITALIC' then return 'gtk-italic'
   when sid = 'GTK_STOCK_JUMP_TO' then return 'gtk-jump-to'
   when sid = 'GTK_STOCK_JUSTIFY_CENTER' then return 'gtk-justify-center'
   when sid = 'GTK_STOCK_JUSTIFY_FILL' then return 'gtk-justify-fill'
   when sid = 'GTK_STOCK_JUSTIFY_LEFT' then return 'gtk-justify-left'
   when sid = 'GTK_STOCK_JUSTIFY_RIGHT' then return 'gtk-justify-right'
   when sid = 'GTK_STOCK_LEAVE_FULLSCREEN' then return 'gtk-leave-fullscreen'
   when sid = 'GTK_STOCK_MEDIA_FORWARD' then return 'gtk-media-forward'
   when sid = 'GTK_STOCK_MEDIA_NEXT' then return 'gtk-media-nect'
   when sid = 'GTK_STOCK_MEDIA_PAUSE' then return 'gtk-media-pause'
   when sid = 'GTK_STOCK_MEDIA_PLAY' then return 'gtk-media-play'
   when sid = 'GTK_STOCK_MEDIA_PREVIOUS' then return 'gtk-media-previous'
   when sid = 'GTK_STOCK_MEDIA_RECORD' then return 'gtk-media-record'
   when sid = 'GTK_STOCK_MEDIA_REWIND' then return 'gtk-media-rewind'
   when sid = 'GTK_STOCK_MEDIA_STOP' then return 'gtk-media-stop'
   when sid = 'GTK_STOCK_MISSING_IMAGE' then return 'gtk-missing-image'
   when sid = 'GTK_STOCK_NETWORK' then return 'gtk-network'
   when sid = 'GTK_STOCK_NEW' then return 'gtk-new'
   when sid = 'GTK_STOCK_NO' then return 'gtk-no'
   when sid = 'GTK_STOCK_OK' then return 'gtk-ok'
   when sid = 'GTK_STOCK_OPEN' then return 'gtk-open'
   when sid = 'GTK_STOCK_OREINTATION_LANDSCAPE' then return 'gtk-orientation-landscape'
   when sid = 'GTK_STOCK_OREINTATION_PORTRAIT' then return 'gtk-orientation-portrait'
   when sid = 'GTK_STOCK_OREINTATION_REVERSE_LANDSCAPE' then return 'gtk-orientation-reverse-landscape'
   when sid = 'GTK_STOCK_OREINTATION_REVERSE_LANDSCAPE' then return 'gtk-orientation-reverse-portrait'
   when sid = 'GTK_STOCK_PASTE' then return 'gtk-paste'
   when sid = 'GTK_STOCK_PREFERENCES' then return 'gtk-preferences'
   when sid = 'GTK_STOCK_PRINT' then return 'gtk-print'
   when sid = 'GTK_STOCK_PRINT_PREVIEW' then return 'gtk-print-preview'
   when sid = 'GTK_STOCK_PROPERTIES' then return 'gtk-properties'
   when sid = 'GTK_STOCK_QUIT' then return 'gtk-quit'
   when sid = 'GTK_STOCK_REDO' then return 'gtk-redo'
   when sid = 'GTK_STOCK_REFRESH' then return 'gtk-refresh'
   when sid = 'GTK_STOCK_REMOVE' then return 'gtk-remove'
   when sid = 'GTK_STOCK_REVERT_TO_SAVED' then return 'gtk-revert-to-saved'
   when sid = 'GTK_STOCK_SAVE' then return 'gtk-save'
   when sid = 'GTK_STOCK_SAVE_AS' then return 'gtk-save-as'
   when sid = 'GTK_STOCK_SELECT_ALL' then return 'gtk-select-all'
   when sid = 'GTK_STOCK_SELECT_COLOR' then return 'gtk-select-color'
   when sid = 'GTK_STOCK_SELECT_FONT' then return 'gtk-select-font'
   when sid = 'GTK_STOCK_SORT_ASCENDING' then return 'gtk-sort-ascending'
   when sid = 'GTK_STOCK_SORT_DESCENDING' then return 'gtk-sort-descending'
   when sid = 'GTK_STOCK_SPELL_CHECK' then return 'gtk-spell-check'
   when sid = 'GTK_STOCK_STOP' then return 'gtk-stop'
   when sid = 'GTK_STOCK_STRIKETHROUGH' then return 'gtk-strikethrough'
   when sid = 'GTK_STOCK_UNDELETE' then return 'gtk-undelete'
   when sid = 'GTK_STOCK_UNDERLINE' then return 'gtk-underline'
   when sid = 'GTK_STOCK_UNDO' then return 'gtk-undo'
   when sid = 'GTK_STOCK_UNINDENT' then return 'gtk-unindent'
   when sid = 'GTK_STOCK_YES' then return 'gtk-yes'
   when sid = 'GTK_STOCK_ZOOM_100' then return 'gtk-zoom-100'
   when sid = 'GTK_STOCK_ZOOM_FIT' then return 'gtk-zoom-fit'
   when sid = 'GTK_STOCK_ZOOM_IN' then return 'gtk-zoom-in'
   when sid = 'GTK_STOCK_ZOOM_OUT' then return 'gtk-zoom-out'
   otherwise nop
   end
return id


/*============================================================================*/
/* Class: GtkAdjustment                                                       */
/*============================================================================*/

::class GtkAdjustment public

::method init
expose pointer
if arg() = 1 then do
   use strict arg pointer
   end
else do
   use strict arg value, upper, lower, step_inc, page_inc, page_size
   pointer = GrxAdjustmentNew(value, upper, lower, step_inc, page_inc, page_size)
   end
return

::method pointer
expose pointer
return pointer

::method 'value='
expose pointer
use strict arg val
call GrxAdjustmentSetValue pointer, val
return

::method set_value
use strict arg val
self~value = val
return

::method value
expose pointer
use strict arg
return GrxAdjustmentGetValue(pointer)

::method get_value
return self~value

::method clamp_page
expose pointer
use strict arg lower, upper
call GrxAdjustmentClampPage pointer, lower, upper
return

::method signal_connect
use strict arg event
select
   when event = 'changed' then call GrxAdjustmentConnectSignal self~pointer, event
   when event = 'value-changed' then call GrxAdjustmentConnectSignal self~pointer, event
   otherwise return .false  -- this class has no parents
   end
return .true

::method signal_changed
return

::method signal_value_changed
return


/*============================================================================*/
/* Class: GtkWidget                                                           */
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
pointer = upper(ptr)
.local['GTK_Database']~put(self, pointer)
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

::method modify_fg
use strict arg state, color
call GrxWidgetModifyFG self~pointer, state, color
return

::method modify_bg
use strict arg state, color
call GrxWidgetModifyBG self~pointer, state, color
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

::method signal_connect
use strict arg event
select
   when event = 'destroy' then call GrxWidgetConnectSignal self~pointer, event
   otherwise return .false  -- this class has no parents
   end
return .true

::method signal_destroy
return


/*============================================================================*/
/* Class: GtkContainer                                                        */
/* Note:  This is strictly a base class. It should never be instantiated on   */
/*        its own. You should instantiate one of its subclasses instead.      */
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

::method signal_connect
use strict arg event
select
   when event = 'add' then call GrxContainerConnectSignal self~pointer, event
   when event = 'check-resize' then call GrxContainerConnectSignal self~pointer, event
   when event = 'removs' then call GrxContainerConnectSignal self~pointer, event
   when event = 'set-focus-child' then call GrxContainerConnectSignal self~pointer, event
   otherwise return self~signal_connect:super(event)
   end
return .true

::method signal_add
return

::method signal_check_resize
return

::method signal_remove
return

::method signal_set_focus_child
return


/*============================================================================*/
/* Class: GtkBox                                                              */
/* Note:  This is strictly a base class. It should never be instantiated on   */
/*        its own. You should instantiate one of its subclasses instead.      */
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

::method signal_connect
use strict arg event
return self~signal_connect:super(event)


/*============================================================================*/
/* Class: GtkHBox                                                             */
/*============================================================================*/

::class GtkHBox public subclass GtkBox

::method init
use arg homogeneous, spacing
self~pointer = GrxHBoxNew(homogeneous, spacing)
return

::method signal_connect
use strict arg event
return self~signal_connect:super(event)


/*============================================================================*/
/* Class: GtkHBox_From_Pointer                                                */
/*============================================================================*/

::class GtkHBox_From_Pointer private subclass GtkHBox

::method init
use strict arg self~pointer
return


/*============================================================================*/
/* Class: GtkVBox                                                             */
/*============================================================================*/

::class GtkVBox public subclass GtkBox

::method init
use arg homogeneous, spacing
self~pointer = GrxVBoxNew(homogeneous, spacing)
return

::method signal_connect
use strict arg event
return self~signal_connect:super(event)


/*============================================================================*/
/* Class: GtkVBox_From_Pointer                                                */
/*============================================================================*/

::class GtkVBox_From_Pointer private subclass GtkVBox

::method init
use strict arg self~pointer
return


/*============================================================================*/
/* Class: GtkAlignment                                                        */
/*============================================================================*/

::class GtkAlignment public subclass GtkContainer

::method init
use arg xalign, yalign, xscale, yscale
self~pointer = GrxAlignmentNew(xalign, yalign, xscale, yscale)
return

::method signal_connect
use strict arg event
return self~signal_connect:super(event)


/*============================================================================*/
/* Class: GtkPaned                                                            */
/* Note:  This is strictly a base class. It should never be instantiated on   */
/*        its own. You should instantiate one of its subclasses instead.      */
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

::method signal_connect
use strict arg event
return self~signal_connect:super(event)


/*============================================================================*/
/* Class: GtkHPaned                                                           */
/*============================================================================*/

::class GtkHPaned public subclass GtkPaned

::method init
if arg() = 0 then self~pointer = GrxHPanedNew()
else use strict arg self~pointer
return

::method signal_connect
use strict arg event
return self~signal_connect:super(event)


/*============================================================================*/
/* Class: GtkVPaned                                                           */
/*============================================================================*/

::class GtkVPaned public subclass GtkPaned

::method init
if arg() = 0 then self~pointer = GrxVPanedNew()
else use strict arg self~pointer
return

::method signal_connect
use strict arg event
return self~signal_connect:super(event)


/*============================================================================*/
/* Class: GtkScrolledWindow                                                   */
/*============================================================================*/

::class GtkScrolledWindow public subclass GtkContainer

::method init
use strict arg hadj, vadj
if hadj = .nil then hptr = 0
else hptr = hadj~pointer
if vadj = .nil then vptr = 0
else vptr = vadj~pointer
self~pointer = GrxScrolledWindowNew(hptr, vptr)
return

::method hadjustment
return .GtkAdjustment~new(GrxScrolledWindowGetHAdjustment(self~pointer))

::method get_hadjustment
return self~hadjustment

::method vadjustment
return .GtkAdjustment~new(GrxScrolledWindowGetVAdjustment(self~pointer))

::method get_vadjustment
return self~vadjustment

::method set_policy
use strict arg hpolicy, vpolicy
call GrxScrolledWindowSetPolicy self~pointer, hpolicy, vpolicy
return

::method add_with_viewport
use strict arg child
call GrxScrolledWindowAddWithViewport self~pointer, child~pointer
return

::method signal_connect
use strict arg event
select
   when event = 'move-focus-out' then call GrxContainerConnectSignal self~pointer, event
   when event = 'scroll-child' then call GrxContainerConnectSignal self~pointer, event
   otherwise return self~signal_connect:super(event)
   end
return .true

::method signal_move_focus_out
return

::method signal_scroll_child
return


/*============================================================================*/
/* Class: GtkViewport                                                         */
/*============================================================================*/

::class GtkViewport public subclass GtkContainer

::method init
use strict arg hadj, vadj
if hadj = .nil then hptr = 0
else hptr = hadj~pointer
if vadj = .nil then vptr = 0
else vptr = vadj~pointer
self~pointer = GrxViewportNew(hptr, vptr)
return

::method signal_connect
use strict arg event
select
   when event = 'set-scroll-adjustments' then call GrxContainerConnectSignal self~pointer, event
   otherwise return self~signal_connect:super(event)
   end
return .true

::method signal_set_scroll_adjustments
return


/*============================================================================*/
/* Class: GtkWindow                                                           */
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

::method 'modal='
-- set the window's modal flag
expose title
use strict arg flag
call GrxWindowSetModal self~pointer, flag
return

::method set_modal
use strict arg self~modal
return

::method signal_connect
use strict arg event
select
   when event = 'activate_default' then call GrxWindowConnectSignal self~pointer, event
   when event = 'activate_focus' then call GrxWindowConnectSignal self~pointer, event
   when event = 'keys_changed' then call GrxWindowConnectSignal self~pointer, event
   when event = 'set_focus' then call GrxWindowConnectSignal self~pointer, event
   otherwise return self~signal_connect:super(event)
   end
return .true

::method signal_activate_default
return

::method signal_activate_focus
return

::method signal_keys_changed
return

::method signal_set_focus
return


/*============================================================================*/
/* Class: GtkDialog                                                           */
/*============================================================================*/

::class GtkDialog public subclass GtkWindow

::method init
use strict arg
ptrs = GrxDialogNew()
parse var ptrs self~pointer self~vbox .
return

::method 'vbox=' private
expose vbox
use strict arg vbox_ptr
vbox = .GtkVBox_From_Pointer~new(vbox_ptr)
return vbox

::method vbox
expose vbox
return vbox

::method 'has_separator='
use strict arg flag
call GrxDialogSetHasSeparator self~pointer, flag
return

::method set_has_separator
use strict arg flag
self~has_separator = flag
return

::method 'default_response='
use strict arg resp
call GrxDialogSetDefaultResponse self~pointer, resp
return

::method set_default_response
use strict arg self~default_response
return

::method add_button
use strict arg text, resp
call GrxDialogAddButton self~pointer, text, resp
return

::method dialog_run
-- Note: the name of this method cannot be 'run' as that would override the
-- private 'run' instance method of the ooRexx Object class.
use strict arg
return GrxDialogRun(self~pointer)

::method signal_connect
use strict arg event
select
   when event = 'close' then call GrxDialogConnectSignal self~pointer, event
   when event = 'response' then call GrxDialogConnectSignal self~pointer, event
   otherwise return self~signal_connect:super(event)
   end
return .true

::method signal_close
return

::method signal_response
return


/*============================================================================*/
/* Class: GtkDialog_With_Buttons                                              */
/*============================================================================*/

::class GtkDialog_With_Buttons public subclass GtkDialog

::method init
use strict arg title, parent, type, button_text, button_resp
if parent = .nil then ptr = 0
else ptr = parent~pointer
ptrs = GrxDialogNewWithButtons(title, ptr, type, button_text, button_resp)
parse var ptrs self~pointer self~vbox .
return


/*============================================================================*/
/* Class: GtkMessageDialog                                                    */
/*============================================================================*/

::class GtkMessageDialog public subclass GtkDialog

::method init
use strict arg parent, flags, type, button, text
if parent = .nil then ptr = '0'
else ptr = parent~pointer
ptrs = GrxMessageDialogNew(ptr, flags, type, button, text)
parse var ptrs self~pointer self~vbox .
return


/*============================================================================*/
/* Class: GtkColorSelectionDialog                                             */
/*============================================================================*/

::class GtkColorSelectionDialog public subclass GtkDialog

::method init
use strict arg title, color, alpha
self~pointer = GrxColorSelectionDialogNew(title, color, alpha)
return

::method color
use strict arg
return GrxColorSelectionDialogGetColor(self~pointer)

::method get_color
use strict arg
return self~color


/*============================================================================*/
/* Class: GtkFontSelectionDialog                                              */
/*============================================================================*/

::class GtkFontSelectionDialog public subclass GtkDialog

::method init
use strict arg title
self~pointer = GrxFontSelectionDialogNew(title)
return

::method 'font_name='
use strict arg fname
call GrxFontSelectionDialogSetFontName self~pointer, fname
return

::method set_font_name
use strict arg self~font_name
return

::method font_name
use strict arg
return GrxFontSelectionDialogGetFontName(self~pointer)

::method get_font_name
use strict arg
return self~font_name

::method 'preview_text='
use strict arg text
call GrxFontSelectionDialogSetPreviewText self~pointer, text
return

::method set_preview_text
use strict arg self~preview_text
return


/*============================================================================*/
/* Class: GtkAboutDialog                                                      */
/*============================================================================*/

::class GtkAboutDialog public subclass GtkDialog

::method init
use strict arg
self~pointer = GrxAboutDialogNew()
self~vbox = .nil
return

::method 'program_name='
use strict arg pgmname
call GrxAboutDialogSetProgramName self~pointer, pgmname
return

::method set_program_name
use strict arg self~program_name
return

::method 'version='
use strict arg v
call GrxAboutDialogSetVersion self~pointer, v
return

::method set_version
use strict arg self~version
return

::method 'copyright='
use strict arg copyright
call GrxAboutDialogSetCopyright self~pointer, copyright
return

::method set_copyright
use strict arg self~copyright
return

::method 'comments='
use strict arg comments
call GrxAboutDialogSetComments self~pointer, comments
return

::method set_comments
use strict arg self~comments
return

::method 'license='
use strict arg license
call GrxAboutDialogSetLicense self~pointer, license
return

::method set_license
use strict arg self~license
return

::method 'wrap_license='
use strict arg wraplicense
call GrxAboutDialogSetWrapLicense self~pointer, wraplicense
return

::method set_wrap_license
use strict arg self~wrap_license
return

::method 'website='
use strict arg website
call GrxAboutDialogSetWebsite self~pointer, website
return

::method set_website
use strict arg self~website
return

::method 'website_label='
use strict arg label
call GrxAboutDialogSetWebsiteLabel self~pointer, label
return

::method set_website_label
use strict arg self~website_label
return

::method set_authors
stmt = 'call GrxAboutDialogSetAuthors self~pointer'
do i = 1 to arg()
   stmt = stmt',' arg(i)
   end
interpret stmt
return

::method set_artists
stmt = 'call GrxAboutDialogSetArtists self~pointer'
do i = 1 to arg()
   stmt = stmt',' arg(i)
   end
interpret stmt
return

::method set_documenters
stmt = 'call GrxAboutDialogSetDocumenters self~pointer'
do i = 1 to arg()
   stmt = stmt',' arg(i)
   end
interpret stmt
return

::method 'logo='
use strict arg logo
call GrxAboutDialogSetLogo self~pointer, logo
return

::method set_logo
use strict arg self~logo
return


/*============================================================================*/
/* Class: GtkFileChooserDialog                                                */
/*============================================================================*/

::class GtkFileChooserDialog public subclass GtkDialog inherit GtkFileChooser

::method init
use strict arg title, parent, action, btext, bresp
if parent = .nil then ptr = 0
else ptr = parent~pointer
self~pointer = GrxFileChooserDialogNew(title, ptr, action, btext, bresp)
self~vbox = .nil
return


/*============================================================================*/
/* Class: GtkAssistant                                                        */
/*============================================================================*/

::class GtkAssistant public subclass GtkWindow

::method init
use strict arg
self~pointer = GrxAssistantNew()
return

::method set_current_page
use strict arg widget, num
call GrxAssistantSetCurrentPage self~pointer, widget~pointer, num
return

::method current_page
use strict arg
return GrxAssistantGetCurrentPage(self~pointer)

::method get_current_page
use strict arg
return self~current_page()

::method get_nth_page
use strict arg num
ptr =  GrxAssistantGetNthPage(self~pointer, num)
return .local['GTK_Database']~at(ptr~upper())

::method prepend_page
use strict arg widget
return GrxAssistantPrependPage(self~pointer, widget~pointer)

::method append_page
use strict arg widget
return GrxAssistantAppendPage(self~pointer, widget~pointer)

::method insert_page
use strict arg widget, num
return GrxAssistantInsertPage(self~pointer, widget~pointer, num)

::method set_page_type
use strict arg widget, type
call GrxAssistantSetPageType self~pointer, widget~pointer, type
return

::method set_page_title
use strict arg widget, title
call GrxAssistantSetPageTitle self~pointer, widget~pointer, title
return

::method set_page_complete
use strict arg widget, flag
call GrxAssistantSetPageComplete self~pointer, widget~pointer, flag
return

::method set_page_header_image
use strict arg widget, image
call GrxAssistantSetPageHeaderImage self~pointer, widget~pointer, image
return

::method set_page_side_image
use strict arg widget, image
call GrxAssistantSetPageSideImage self~pointer, widget~pointer, image
return

::method signal_connect
use strict arg event
select
   when event = 'apply' then call GrxAssistantConnectSignal self~pointer, event
   when event = 'cancel' then call GrxAssistantConnectSignal self~pointer, event
   when event = 'close' then call GrxAssistantConnectSignal self~pointer, event
   when event = 'prepare' then call GrxAssistantConnectSignal self~pointer, event
   otherwise return self~signal_connect:super(event)
   end
return .true

::method signal_apply
return

::method signal_cancel
return

::method signal_close
return

::method signal_prepare
return


/*============================================================================*/
/* Class: GtkLabel                                                            */
/*============================================================================*/

::class GtkLabel public subclass GtkWidget

::method init
expose justify
self~pointer = GrxLabelNew()
if arg() = 1 then do
   use strict arg text = ''
   self~text = text
   end
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

::method signal_connect
use strict arg event
return self~signal_connect:super(event)

/*============================================================================*/
/* Class: GtkButton                                                           */
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

::method signal_connect
use strict arg event
select
   when event = 'pressed' then call GrxButtonConnectSignal self~pointer, event
   when event = 'released' then call GrxButtonConnectSignal self~pointer, event
   when event = 'clicked' then call GrxButtonConnectSignal self~pointer, event
   when event = 'enter' then call GrxButtonConnectSignal self~pointer, event
   when event = 'leave' then call GrxButtonConnectSignal self~pointer, event
   otherwise return self~signal_connect:super(event)
   end
return .true

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
/* Class: GtkButton_With_Mnemonic                                             */
/*============================================================================*/

::class GtkButton_With_Mnemonic public subclass GtkButton

::method init
self~init:super
use arg label = ''
self~use_underline = .true
self~label = label
return

/*============================================================================*/
/* Class: GtkButton_With_Label                                                */
/*============================================================================*/

::class GtkButton_With_Label public subclass GtkButton

::method init
self~init:super
use arg label = ''
self~label = label
return

/*============================================================================*/
/* Class: GtkButton_From_Stock                                                */
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
/* Class: GtkToggleButton                                                      */
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

::method signal_connect
use strict arg event
select
   when event = 'toggled' then call GrxToggleButtonConnectSignal self~pointer, event
   otherwise return self~signal_connect:super(event)
   end
return .true

::method signal_toggled
return

/*============================================================================*/
/* Class: GtkToggleButton_With_Mnemonic                                       */
/*============================================================================*/

::class GtkToggleButton_With_Mnemonic public subclass GtkToggleButton

::method init
self~init:super
use arg label = ''
self~use_underline = .true
self~label = label
return

/*============================================================================*/
/* Class: GtkToggleButton_With_Label                                          */
/*============================================================================*/

::class GtkToggleButton_With_Label public subclass GtkToggleButton

::method init
self~init:super
use arg label = ''
self~label = label
return


/*============================================================================*/
/* Class: GtkCheckbutton                                                      */
/*============================================================================*/

::class GtkCheckbutton public subclass GtkToggleButton

::method init
self~pointer = GrxCheckButtonNew()
use strict arg self~label = ''
return

::method signal_connect
use strict arg event
return self~signal_connect:super(event)

/*============================================================================*/
/* Class: GtkCheckButton_With_Mnemonic                                        */
/*============================================================================*/

::class GtkCheckButton_With_Mnemonic public subclass GtkCheckButton

::method init
self~init:super
use arg label = ''
self~use_underline = .true
self~label = label
return

/*============================================================================*/
/* Class: GtkCheckButton_With_Label                                           */
/*============================================================================*/

::class GtkCheckButton_With_Label public subclass GtkCheckButton

::method init
self~init:super
use arg label = ''
self~label = label
return


/*============================================================================*/
/* Class: GtkRadioButton                                                      */
/*============================================================================*/

::class GtkRadioButton public subclass GtkCheckbutton

::method init
use strict arg group
if group = .nil then self~pointer = GrxRadioButtonNew(0);
else self~pointer = GrxRadioButtonNew(group~head)
return

::method group
use strict arg
return GrxRadioButtonGetGroup(self~pointer);

::method get_group
use strict arg
return self~group

::method 'group='
use strict arg group
if group = .nil then call GrxRadioButtonSetGroup self~pointer, 0
else call GrxRadioButtonSetGroup self~pointer, group~head
return

::method set_group
use strict arg self~group
return

::method signal_connect
use strict arg event
select
   when event = 'group_changed' then call GrxRadioButtonConnectSignal self~pointer, event
   otherwise return self~signal_connect:super(event)
   end
return .true

::method signal_group_changed
return

/*============================================================================*/
/* Class: GtkRadioButton_With_Mnemonic                                        */
/*============================================================================*/

::class GtkRadioButton_With_Mnemonic public subclass GtkRadioButton

::method init
use arg group, label = ''
self~init:super(group)
self~use_underline = .true
self~label = label
return

/*============================================================================*/
/* Class: GtkRadioButton_With_Label                                           */
/*============================================================================*/

::class GtkRadioButton_With_Label public subclass GtkRadioButton

::method init
use arg group, label = ''
self~init:super(group)
self~label = label
return

/*============================================================================*/
/* Class: GtkRadioButton_From_Widget                                          */
/*============================================================================*/

::class GtkRadioButton_From_Widget public subclass GtkRadioButton

::method init
use arg widget
self~pointer = GrxRadioButtonNewFromWidget(widget~pointer)
return

/*============================================================================*/
/* Class: GtkRadioButton_From_Widget_With_Label                               */
/*============================================================================*/

::class GtkRadioButton_From_Widget_With_Label public subclass GtkRadioButton_From_Widget

::method init
use arg widget, label
self~init:super(widget)
self~label = label
return

/*============================================================================*/
/* Class: GtkRadioButton_From_Widget_With_Mnemonic                            */
/*============================================================================*/

::class GtkRadioButton_From_Widget_With_Mnemonic public subclass GtkRadioButton_From_Widget

::method init
use arg widget, label
self~init:super(widget)
self~use_underline = .true
self~label = label
return


/*============================================================================*/
/* Class: GtkColorButton                                                      */
/*============================================================================*/

::class GtkColorButton public subclass GtkButton

::method init
use strict arg
self~pointer = GrxColorButtonNew()
return

::method 'title='
use strict arg title
call GrxColorButtonSetTitle self~pointer, title
return

::method set_title
use strict arg self~title
return

::method color
use strict arg
return GrxColorButtonGetColor(self~pointer)

::method get_color
use strict arg
return self~color

::method 'color='
use strict arg color
call GrxColorButtonSetColor self~pointer, color
return

::method set_color
use strict arg self~color
return

::method signal_connect
use strict arg event
select
   when event = 'color_set' then call GrxColorButtonConnectSignal self~pointer, event
   otherwise return self~signal_connect:super(event)
   end
return .true

::method signal_color_set
return


/*============================================================================*/
/* Class: GtkColorButton_With_Color                                           */
/*============================================================================*/

::class GtkColorButton_With_Color public subclass GtkColorButton

::method init
use strict arg color
self~pointer = GrxColorButtonNewWithColor(color)
return


/*============================================================================*/
/* Class: GtkFileChooserButton                                                */
/*============================================================================*/

::class GtkFileChooserButton public subclass GtkHBox inherit GtkFileChooser

::method init
use strict arg title, action
self~pointer = GrxFileChooserButtonNew(title, action)
return

::method signal_connect
use strict arg event
-- first try the GtkFileChooser class hiearchy first
if self~signal_connect:.GtkFileChooser(event) = .true then return .true
-- now try the standard hiearchy
select
   when event = 'file_set' then call GrxFileChooserButtonConnectSignal self~pointer, event
   otherwise return self~signal_connect:super(event)
   end
return .true

::method signal_file_set
return


/*============================================================================*/
/* Class: GtkFontButton                                                       */
/*============================================================================*/

::class GtkFontButton public subclass GtkButton

::method init
use strict arg
self~pointer = GrxFontButtonNew()
return

::method 'title='
use strict arg title
call GrxFontButtonSetTitle self~pointer, title
return

::method set_title
use strict arg self~title
return

::method font_name
use strict arg
return GrxFontButtonGetFontName(self~pointer)

::method get_font_name
use strict arg
return self~font_name

::method signal_connect
use strict arg event
select
   when event = 'font_set' then call GrxFontButtonConnectSignal self~pointer, event
   otherwise return self~signal_connect:super(event)
   end
return .true

::method signal_font_set
return


/*============================================================================*/
/* Class: GtkTable                                                            */
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

::method attach_defaults
use strict arg widget, left, right, top, bottom
call GrxTableAttach self~pointer, widget~pointer, left, right, top, bottom,,
                    5, 5, 0, 0
return

::method 'row_spacings='
use strict arg spacing
call GrxTableSetRowSpacings self~pointer, spacing
return

::method set_row_spacings
use strict arg self~row_spacings
return

::method 'col_spacings='
use strict arg spacing
call GrxTableSetColSpacings self~pointer, spacing
return

::method set_col_spacings
use strict arg self~col_spacings
return

::method signal_connect
use strict arg event
return self~signal_connect:super(event)


/*============================================================================*/
/* Class: GtkEntry                                                            */
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

::method get_text
-- get the text in the object
use strict arg
return self~text

::method 'text='
-- set the text in the object
use strict arg text
call GrxEntrySetText self~pointer, text
return

::method set_text
-- set the text in the object
use strict arg self~text
return

::method visibility
use strict arg
return GrxEntrySetVisibility(self~pointer)

::method get_visibility
use strict arg
return self~visibility

::method 'visibility='
use strict arg flag
call GrxEntrySetVisibility self~pointer, flag
return

::method set_visibility
use strict arg self~visibility
return

::method invisible_char
use strict arg 
return GrxEntrySetInvisibileChar(self~pointer)

::method get_invisible_char
use strict arg
return self~invisible_char

::method 'invisible_char='
use strict arg char
call GrxEntrySetInvisibleChar self~pointer, char
return

::method set_invisible_char
use strict arg self~invisible_char
return

::method signal_connect
use strict arg event
select
   when event = 'activate' then call GrxEntryConnectSignal self~pointer, event
   when event = 'backspace' then call GrxEntryConnectSignal self~pointer, event
   when event = 'changed' then call GrxEntryConnectSignal self~pointer, event
   when event = 'copy_clipboard' then call GrxEntryConnectSignal self~pointer, event
   when event = 'cut_clipboard' then call GrxEntryConnectSignal self~pointer, event
   when event = 'delete_from_cursor' then call GrxEntryConnectSignal self~pointer, event
   when event = 'insert_at_cursor' then call GrxEntryConnectSignal self~pointer, event
   when event = 'move_cursor' then call GrxEntryConnectSignal self~pointer, event
   when event = 'paste_clipboard' then call GrxEntryConnectSignal self~pointer, event
   when event = 'populate_popup' then call GrxEntryConnectSignal self~pointer, event
   when event = 'toggle_overwrite' then call GrxEntryConnectSignal self~pointer, event
   otherwise return self~signal_connect:super(event)
   end
return .true

::method signal_activate
return

::method signal_backspace
return

::method signal_changed
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
/* Class: GtkFixed                                                            */
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

::method signal_connect
use strict arg event
return self~signal_connect:super(event)


/*============================================================================*/
/* Class: GtkExpander                                                         */
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

::method signal_connect
use strict arg event
return self~signal_connect:super(event)


/*============================================================================*/
/* Class: GtkExpander_With_Mnemonic                                           */
/*============================================================================*/

::class GtkExpander_With_Mnemonic public subclass GtkExpander

::method init
use arg label = ''
self~init:super(label)
self~use_underline = .true
return


/*============================================================================*/
/* Class: GtkHandleBox                                                        */
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

::method signal_connect
use strict arg event
return self~signal_connect:super(event)


/*============================================================================*/
/* Class: GtkNotebook                                                         */
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

::method signal_connect
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
   otherwise return self~signal_connect:super(event)
   end
return .true

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
/* Class: GtkEventBox                                                         */
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

::method signal_connect
use strict arg event
return self~signal_connect:super(event)


/*============================================================================*/
/* Class: GtkRange                                                            */
/* Note:  This is strictly a base class. It should never be instantiated on   */
/*        its own. You should instantiate one of its subclasses instead.      */
/*============================================================================*/

::class GtkRange public subclass GtkWidget

::method init
raise syntax 93.964 array ('A GtkRange class cannot be instantiated.')
return

::method signal_connect
use strict arg event
select
   when event = 'adjust_bounds' then call GrxLabelConnectSignal self~pointer, event
   when event = 'move_slider' then call GrxLabelConnectSignal self~pointer, event
   when event = 'value_changed' then call GrxLabelConnectSignal self~pointer, event
   otherwise return self~signal_connect:super(event)
   end
return .true

::method signal_adjust_bounds
return

::method signal_move_slider
return

::method signal_value_changed
return


/*============================================================================*/
/* Class: GtkScale                                                            */
/* Note:  This is strictly a base class. It should never be instantiated on   */
/*        its own. You should instantiate one of its subclasses instead.      */
/*============================================================================*/

::class GtkScale public subclass GtkRange

::method init
raise syntax 93.964 array ('A GtkScale class cannot be instantiated.')
return

::method 'digits='
use strict arg digits
call GrxScaleSetDigits self~pointer, digits
return

::method set_digits
use strict arg self~digits

::method digits
use strict arg
return GrxScaleGetDigits(self~pointer)

::method get_digits
return self~digits

::method 'value_pos='
use strict arg pos
call GrxScaleSetValuePos self~pointer, pos
return

::method set_value_pos
use strict arg self~value_pos

::method value_pos
use strict arg
return GrxScaleGetValuePos(self~pointer)

::method get_value_pos
return self~digits

::method signal_connect
use strict arg event
return self~signal_connect:super(event)


/*============================================================================*/
/* Class: GtkHScale                                                           */
/*============================================================================*/

::class GtkHScale public subclass GtkScale

::method init
use strict arg adj
self~pointer = GrxHScaleNew(adj~value, adj~lower, adj~upper, adj~step_inc,,
 adj~page_inc, adj~page_size)
return


/*============================================================================*/
/* Class: GtkHScale_With_Range                                                */
/*============================================================================*/

::class GtkHScale_With_Range public subclass GtkHScale

::method init
use strict arg min, max, step
self~pointer = GrxHScaleNewWithRange(min, max, step)
return


/*============================================================================*/
/* Class: GtkVScale                                                           */
/*============================================================================*/

::class GtkVScale public subclass GtkScale

::method init
use strict arg adj
self~pointer = GrxVScaleNew(adj~value, adj~lower, adj~upper, adj~step_inc,,
 adj~page_inc, adj~page_size)
return

::method signal_connect
use strict arg event
return self~signal_connect:super(event)


/*============================================================================*/
/* Class: GtkVScale_With_Range                                                */
/*============================================================================*/

::class GtkVScale_With_Range public subclass GtkVScale

::method init
use strict arg min, max, step
self~pointer = GrxVScaleNewWithRange(min, max, step)
return


/*============================================================================*/
/* Class: GtkSpinButton                                                       */
/*============================================================================*/

::class GtkSpinButton public subclass GtkEntry

::method init
use strict arg adj, climb_rate, digits
self~pointer = GrxSpinButtonNew(adj~value, adj~lower, adj~upper, adj~step_inc,,
 adj~page_inc, adj~page_size, climb_rate, digits)
return

::method signal_connect
use strict arg event
select
   when event = 'change_value' then call GrxLabelConnectSignal self~pointer, event
   when event = 'value_changed' then call GrxLabelConnectSignal self~pointer, event
   when event = 'wrapped' then call GrxLabelConnectSignal self~pointer, event
   otherwise return self~signal_connect:super(event)
   end
return .true

::method signal_change_value
return

::method signal_value_changed
return

::method signal_wrapped
return


/*============================================================================*/
/* Class: GtkImage                                                            */
/*============================================================================*/

::class GtkImage public subclass GtkWidget

::method init
use strict arg
self~pointer = GrxImageNew()
return

::method set_from_file
use strict arg filename
call GrxImageSetFromFile self~pointer, filename
return

::method set_from_stock
use strict arg stockname, size
call GrxImageSetFromStock self~pointer, stockname, size
return

::method set_from_icon_name
use strict arg iconname, size
call GrxImageSetFromIconName self~pointer, iconname, size
return


/*============================================================================*/
/* Class: GtkImage_From_File                                                  */
/*============================================================================*/

::class GtkImage_From_File public subclass GtkImage

::method init
self~init:super()
use strict arg self~set_from_file
return


/*============================================================================*/
/* Class: GtkImage_From_Stock                                                 */
/*============================================================================*/

::class GtkImage_From_Stock public subclass GtkImage

::method init
self~init:super()
use strict arg stockname, size
self~set_from_stock(stockname, size)
return


/*============================================================================*/
/* Class: GtkImage_From_Icon_Name                                             */
/*============================================================================*/

::class GtkImageFromIconName public subclass GtkImage

::method init
self~init:super()
use strict arg iconname, size
self~set_from_icon_name(iconname, size)
return


/*============================================================================*/
/* Class: GtkProgressBar                                                      */
/*============================================================================*/

::class GtkProgressBar public subclass GtkWidget

::method init
self~pointer = GrxProgressBarNew()
return

::method 'text='
use strict arg text
call GrxProgressBarSetText self~pointer, text
return

::method set_text
use strict arg self~text
return

::method 'fraction='
use strict arg fraction
call GrxProgressbarSetFraction self~pointer, fraction
return

::method set_fraction
use strict arg self~fraction
return


/*============================================================================*/
/* Class: GtkTextView                                                         */
/*============================================================================*/

::class GtkTextView public subclass GtkContainer

::method init
self~pointer = GrxTextViewNew()
return

::method buffer
use strict arg
ptr = GrxTextViewGetBuffer(self~pointer)
return .GtkTextBuffer~new(ptr)

::method get_buffer
use strict arg
return self~buffer

::method wrap_mode
use strict arg
return GrxTextViewGetWrapMode(self~pointer)

::method get_wrap_mode
use strict arg
return self~wrap_mode

::method 'wrap_mode='
use strict arg mode
call GrxTextViewSetWrapMode self~pointer, upper(mode)
return

::method set_wrap_mode
use strict arg self~wrap_mode
return

::method justification
use strict arg
return GrxTextViewGetJustification(self~pointer)

::method get_justification
use strict arg
return self~justification

::method 'justification='
use strict arg just
call GrxTextViewSetJustification self~pointer, upper(just)
return

::method set_justification
use strict arg self~justification
return

::method editable
use strict arg
return GrxTextViewGetEditable(self~pointer)

::method get_editable
use strict arg
return self~editable

::method 'editable='
use strict arg flag
call GrxTextViewSetEditable self~pointer, flag
return

::method set_editable
use strict arg self~editable
return

::method cursor_visible
use strict arg
return GrxTextViewGetCursorVisible(self~pointer)

::method get_cursor_visible
use strict arg
return self~cursor_visible

::method 'cursor_visible='
use strict arg flag
call GrxTextViewSetCursorVisible self~pointer, flag
return

::method set_cursor_visible
use strict arg self~cursor_visible
return

::method pixels_above_lines
use strict arg
return GrxTextViewGetPixelsAboveLines(self~pointer)

::method get_pixels_above_lines
use strict arg
return self~pixels_above_lines

::method 'pixels_above_lines='
use strict arg flag
call GrxTextViewSetPixelsAboveLines self~pointer, flag
return

::method set_pixels_above_lines
use strict arg self~pixels_above_lines
return

::method pixels_below_lines
use strict arg
return GrxTextViewGetPixelsBelowLines(self~pointer)

::method get_pixels_below_lines
use strict arg
return self~pixels_below_lines

::method 'pixels_below_lines='
use strict arg flag
call GrxTextViewSetPixelsBelowLines self~pointer, flag
return

::method set_pixels_below_lines
use strict arg self~pixels_below_lines
return

::method pixels_inside_wrap
use strict arg
return GrxTextViewGetPixelsInsideWrap(self~pointer)

::method get_pixels_inside_wrap
use strict arg
return self~pixels_inside_wrap

::method 'pixels_inside_wrap='
use strict arg flag
call GrxTextViewSetPixelsInsideWrap self~pointer, flag
return

::method set_pixels_inside_wrap
use strict arg self~pixels_inside_wrap
return

::method left_margin
use strict arg
return GrxTextViewGetLeftMargin(self~pointer)

::method get_left_margin
use strict arg
return self~left_margin

::method 'left_margin='
use strict arg flag
call GrxTextViewSetLeftMargin self~pointer, flag
return

::method set_left_margin
use strict arg self~left_margin
return

::method right_margin
use strict arg
return GrxTextViewGetRightMargin(self~pointer)

::method get_right_margin
use strict arg
return self~right_margin

::method 'right_margin='
use strict arg flag
call GrxTextViewSetRightMargin self~pointer, flag
return

::method set_right_margin
use strict arg self~right_margin
return


/*============================================================================*/
/* Class: GtkTextBuffer                                                       */
/*============================================================================*/

::class GtkTextBuffer public

::method init
expose pointer
if arg() = 1 then use strict arg pointer
else do
   use strict arg
   pointer = GrxTextBufferNew()  -- Fix this!
   end
return

::method 'text='
expose pointer
use strict arg text
ptr = GrxTextBufferSetText(pointer, text)
return

::method set_text
use strict arg self~text
return

::method text
expose pointer
use strict arg
return GrxTextBufferGetText(pointer)

::method get_text
use strict arg
return self~text

::method insert_text
expose pointer
use strict arg text
call GrxTextBufferInsertText pointer, text
return

::method cut_clipboard
expose pointer
use strict arg cb, flag
call GrxTextBufferCutClipboard pointer, cb~pointer, flag
return

::method copy_clipboard
expose pointer
use strict arg cb
call GrxTextBufferCopyClipboard pointer, cb~pointer
return

::method paste_clipboard
expose pointer
use strict arg cb, flag
call GrxTextBufferPasteClipboard pointer, cb~pointer, flag
return

::method signal_connect
use strict arg event
select
   when event = 'apply_tag' then call GrxTextBufferConnectSignal self~pointer, event
   when event = 'begin_user_action' then call GrxTextBufferConnectSignal self~pointer, event
   otherwise return self~signal_connect:super(event)
   end
return .true

::method signal_apply_tag
return

::method signal_begin_user_action
return


/*============================================================================*/
/*============================================================================*/
/*============================================================================*/
/* These are classes and routines specific to Glib.                           */
/*============================================================================*/
/*============================================================================*/
/*============================================================================*/


/*============================================================================*/
/* Routine: g_message                                                         */
/*============================================================================*/

::routine g_message public
use strict arg msg
call GrxMessage(msg)
return


/*============================================================================*/
/* Routine: g_warning                                                         */
/*============================================================================*/

::routine g_warning public
use strict arg msg
call GrxWarning(msg)
return


/*============================================================================*/
/* Routine: g_critical                                                        */
/*============================================================================*/

::routine g_critical public
use strict arg msg
call GrxCritical(msg)
return


/*============================================================================*/
/* Routine: g_Error                                                           */
/*============================================================================*/

::routine g_error public
use strict arg msg
call GrxError(msg)
return


/*============================================================================*/
/* Routine: g_debug                                                           */
/*============================================================================*/

::routine g_debug public
use strict arg msg
call GrxDebug(msg)
return


/*============================================================================*/
/* Routine: g_get_user_name                                                   */
/*============================================================================*/

::routine g_get_user_name public
use strict arg
return GrxGetUserName()


/*============================================================================*/
/* Routine: g_get_real_name                                                   */
/*============================================================================*/

::routine g_get_real_name public
use strict arg
return GrxGetRealName()


/*============================================================================*/
/* Routine: g_get_home_dir                                                    */
/*============================================================================*/

::routine g_get_home_dir public
use strict arg
return GrxGetHomeDir()


/*============================================================================*/
/* Routine: g_get_host_name                                                   */
/*============================================================================*/

::routine g_get_host_name public
use strict arg
return GrxGetHostName()


/*============================================================================*/
/* Class: GSList                                                              */
/*============================================================================*/

::class GSList public

::method init
use strict arg self~head = 0
return

::method uninit
call GrxGSListUninit self~head
return

::method append
-- append an item string to the list
use strict arg item
self~head = GrxGSListAppend(self~head, item)
return

::method head
-- return the head of the list
expose head
use strict arg
return head

::method 'head='
-- set the head of the list
expose head
use strict arg head
return

::method insert
-- append an item string to the list
use strict arg item, position
self~head = GrxGSListInsert(self~head, item, position)
return

::method toArray
-- convert the list to an array
use strict arg
ptr = self~head
list = .Array~new()
if ptr = 0 then return list
do while ptr <> '(nil)'
   list~append(GrxGSListItem(ptr))
   ptr = GrxGSListNext(ptr)
   end
return list

::method prepend
-- prepend an item string to the list
use strict arg item
self~head = GrxGSListPrepend(self~head, item)
return


/*============================================================================*/
/* Class: GList                                                               */
/*============================================================================*/

::class GList public subclass object

::method init
use strict arg self~head = 0
return

::method uninit
call GrxGListUninit self~head
return

::method append
-- append an item string to the list
use arg strict item
self~head = GrxGListAppend(self~head, item)
return

::method head
-- return the head of the list
expose head
use strict arg
return head

::method 'head='
-- set the head of the list
expose head
use strict arg head
return

::method insert
-- insert an item string to the list
use strict arg item, position
self~head = GrxGListInsert(self~head, item, position)
return

::method toArray
-- convert the list to an array
use strict arg
ptr = self~head
list = .Array~new()
if ptr = 0 then return list
do while ptr <> '(nil)'
   list~append(GrxGSListItem(ptr))
   ptr = GrxGListNext(ptr)
   end
return list

::method prepend
-- prepend an item string to the list
use strict arg item
self~head = GrxGListPrepend(self~head, item)
return


/*============================================================================*/
/*============================================================================*/
/*============================================================================*/
/* These are helper classes. Most of these just provide interfaces for other  */
/* classes to use. Some are just standalone classes.                          */
/*============================================================================*/
/*============================================================================*/
/*============================================================================*/


/*============================================================================*/
/* Class: GtkFileChooser                                                      */
/* Note:  This is strictly a mixin class. It should never be instantiated on  */
/*        its own. You should instantiate one of its subclasses instead.      */
/*============================================================================*/

::class GtkFileChooser public mixinclass Object

::method init
raise syntax 93.964 array ('A GtkFileChooser class cannot be instantiated.')
return

::method 'current_folder='
use strict arg dir
call GrxFileChooserSetCurrentFolder self~pointer, dir
return

::method set_current_folder
use strict arg self~current_folder
return

::method add_filter
use strict arg filter
call GrxFileChooserAddFilter self~pointer, filter~pointer
return

::method filename
return GrxFileChooserGetFilename(self~pointer)

::method get_filename
return self~filename

::method filenames
ptr = GrxFileChooserGetFileNames(self~pointer)
list = .GSList~new(ptr)
return list~toArray()

::method get_filenames
return self~filenames()

::method 'select_multiple='
use strict arg flag
call GrxFileChooserSetSelectMultiple self~pointer, flag
return

::method set_select_multiple
use strict arg self~select_multiple
return

::method signal_connect
use strict arg event
select
   when event = 'current_folder_changed' then call GrxFileChooserConnectSignal self~pointer, event
   when event = 'file_activated' then call GrxFileChooserConnectSignal self~pointer, event
   when event = 'selection_changed' then call GrxFileChooserConnectSignal self~pointer, event
   when event = 'update_preview' then call GrxFileChooserConnectSignal self~pointer, event
   otherwise return .false  -- this class has no parent
   end
return .true

::method signal_current_folder_changed
return

::method signal_file_activated
return

::method signal_selection_changed
return

::method signal_update_preview
return


/*============================================================================*/
/* Class: GtkFileFilter                                                       */
/*============================================================================*/

::class GtkFileFilter public

::method init
expose pointer
use strict arg
pointer = GrxFileFilterNew()
return

::method pointer
expose pointer
return pointer

::method 'name='
expose pointer
use strict arg name
call GrxFileFilterSetName pointer, name
return

::method set_name
expose pointer
use strict arg self~name
return

::method add_pattern
expose pointer
use strict arg pattern
call GrxFileFilterAddPattern pointer, pattern
return


/*============================================================================*/
/* Class: GtkClipboard                                                        */
/*============================================================================*/

::class GtkClipboard public

::method get class
use strict arg selection
return self~new(selection)

::method init
expose pointer
use strict arg selection
pointer = GrxClipboardGet(selection)
return

::method pointer
expose pointer
return pointer







