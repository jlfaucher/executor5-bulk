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


/*============================================================================*/
/* Routine: gtk_main                                                          */
/*============================================================================*/

::routine gtk_main public
call gtk_main_iteration
return


/*============================================================================*/
/* Routine: gtk_main_quit                                                     */
/*============================================================================*/

::routine gtk_main_quit public
call GrxMainQuit
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
return


/*============================================================================*/
/* Class: GTK                                                                 */
/* Note: This class should never be instatiated. It is here just provided     */
/*       a container for a host of GTK constants.                             */
/*============================================================================*/

::class gtk public

   -- GtkWindowType
::constant GTK_WINDOW_TOPLEVEL class 0
::constant GTK_WINDOW_POPUP class 1
   -- GtkButtonType
::constant GTK_BUTTONS_NONE class 0
::constant GTK_BUTTONS_OK class 1
::constant GTK_BUTTONS_CLOSE class 2
::constant GTK_BUTTONS_CANCEL class 3
::constant GTK_BUTTONS_YES_NO class 4
::constant GTK_BUTTONS_OK_CANCEL class 5
   -- GtkDialogFlags
::constant GTK_DIALOG_MODAL class 0
::constant GTK_DIALOG_DESTROY_WITH_PARENT class 1
::constant GTK_DIALOG_NO_SEPARATOR class 2
   -- GtkResponseType
::constant GTK_RESPONSE_NONE class -1
::constant GTK_RESPONSE_REJECT class -2
::constant GTK_RESPONSE_ACCEPT class -3
::constant GTK_RESPONSE_DELETE_EVENT class -4
::constant GTK_RESPONSE_OK class -5
::constant GTK_RESPONSE_CANCEL class -6
::constant GTK_RESPONSE_CLOSE class -7
::constant GTK_RESPONSE_YES class -8
::constant GTK_RESPONSE_NO class -9
::constant GTK_RESPONSE_APPLY class -10
::constant GTK_RESPONSE_HELP class -11
   -- GtkMessageType
::constant GTK_MESSAGE_INFO class 0
::constant GTK_MESSAGE_WARNING class 1
::constant GTK_MESSAGE_QUESTION class 2
::constant GTK_MESSAGE_ERROR class 3
::constant GTK_MESSAGE_OTHER class 4
   -- GTK stock string defines
::constant GTK_STOCK_ABOUT class 'gtk-about'
::constant GTK_STOCK_ADD class 'gtk-add'
::constant GTK_STOCK_APPLY class 'gtk-apply'
::constant GTK_STOCK_BOLD class 'gtk-bold'
::constant GTK_STOCK_CANCEL class 'gtk-cancel'
::constant GTK_STOCK_CDROM class 'gtk-cdrom'
::constant GTK_STOCK_CLEAR class 'gtk-clear'
::constant GTK_STOCK_CLOSE class 'gtk-close'
::constant GTK_STOCK_COLOR_PICKER class 'gtk-color-picker'
::constant GTK_STOCK_CONVERT class 'gtk-comvert'
::constant GTK_STOCK_CONNECT class 'gtk-connect'
::constant GTK_STOCK_COPY class 'gtk-copy'
::constant GTK_STOCK_CUT class 'gtk-cut'
::constant GTK_STOCK_DELETE class 'gtk-delete'
::constant GTK_STOCK_DIALOG_ERROR class 'gtk-dialog-error'
::constant GTK_STOCK_DIALOG_INFO class 'gtk-dialog-info'
::constant GTK_STOCK_DIALOG_QUESTION class 'gtk-dialog-question'
::constant GTK_STOCK_DIALOG_WARNING class 'gtk-dialog-warning'
::constant GTK_STOCK_DIRECTORY class 'gtk-directry'
::constant GTK_STOCK_DISCARD class 'gtk-discard'
::constant GTK_STOCK_DISCONNECT class 'gtk-disconnect'
::constant GTK_STOCK_DND class 'gtk-dnd'
::constant GTK_STOCK_DND_MULTIPLE class 'gtk-dnd-multiple'
::constant GTK_STOCK_EDIT class 'gtk-edit'
::constant GTK_STOCK_EXECUTE class 'gtk-execute'
::constant GTK_STOCK_FILE class 'gtk-file'
::constant GTK_STOCK_FIND class 'gtk-find'
::constant GTK_STOCK_FIND_AND_REPLACE class 'gtk-find-and-replace'
::constant GTK_STOCK_FLOPPY class 'gtk-floppy'
::constant GTK_STOCK_FULLSCREEN class 'gtk-fullscreen'
::constant GTK_STOCK_GOTO_BOTTOM class 'gtk-goto-bottom'
::constant GTK_STOCK_GOTO_FIRST class 'gtk-goto-first'
::constant GTK_STOCK_GOTO_LAST class 'gtk-goto-last'
::constant GTK_STOCK_GOTO_TOP class 'gtk-goto-top'
::constant GTK_STOCK_GO_BACK class 'gtk-go-back'
::constant GTK_STOCK_GO_DOWN class 'gtk-go-down'
::constant GTK_STOCK_GO_FORWARD class 'gtk-go-forward'
::constant GTK_STOCK_GO_UP class 'gtk-go-up'
::constant GTK_STOCK_HARDDISK class 'gtk-harddisk'
::constant GTK_STOCK_HELP class 'gtk-help'
::constant GTK_STOCK_HOME class 'gtk-home'
::constant GTK_STOCK_INDENT class 'gtk-indent'
::constant GTK_STOCK_INDEX class 'gtk-index'
::constant GTK_STOCK_INFO class 'gtk-info'
::constant GTK_STOCK_ITALIC class 'gtk-italic'
::constant GTK_STOCK_JUMP_TO class 'gtk-jump-to'
::constant GTK_STOCK_JUSTIFY_CENTER class 'gtk-justify-center'
::constant GTK_STOCK_JUSTIFY_FILL class 'gtk-justify-fill'
::constant GTK_STOCK_JUSTIFY_LEFT class 'gtk-justify-left'
::constant GTK_STOCK_JUSTIFY_RIGHT class 'gtk-justify-right'
::constant GTK_STOCK_LEAVE_FULLSCREEN class 'gtk-leave-fullscreen'
::constant GTK_STOCK_MEDIA_FORWARD class 'gtk-media-forward'
::constant GTK_STOCK_MEDIA_NEXT class 'gtk-media-nect'
::constant GTK_STOCK_MEDIA_PAUSE class 'gtk-media-pause'
::constant GTK_STOCK_MEDIA_PLAY class 'gtk-media-play'
::constant GTK_STOCK_MEDIA_PREVIOUS class 'gtk-media-previous'
::constant GTK_STOCK_MEDIA_RECORD class 'gtk-media-record'
::constant GTK_STOCK_MEDIA_REWIND class 'gtk-media-rewind'
::constant GTK_STOCK_MEDIA_STOP class 'gtk-media-stop'
::constant GTK_STOCK_MISSING_IMAGE class 'gtk-missing-image'
::constant GTK_STOCK_NETWORK class 'gtk-network'
::constant GTK_STOCK_NEW class 'gtk-new'
::constant GTK_STOCK_NO class 'gtk-no'
::constant GTK_STOCK_OK class 'gtk-ok'
::constant GTK_STOCK_OPEN class 'gtk-open'
::constant GTK_STOCK_OREINTATION_LANDSCAPE class 'gtk-orientation-landscape'
::constant GTK_STOCK_OREINTATION_PORTRAIT class 'gtk-orientation-portrait'
::constant GTK_STOCK_OREINTATION_REVERSE_LANDSCAPE class 'gtk-orientation-reverse-landscape'
::constant GTK_STOCK_OREINTATION_REVERSE_LANDSCAPE class 'gtk-orientation-reverse-portrait'
::constant GTK_STOCK_PASTE class 'gtk-paste'
::constant GTK_STOCK_PREFERENCES class 'gtk-preferences'
::constant GTK_STOCK_PRINT class 'gtk-print'
::constant GTK_STOCK_PRINT_PREVIEW class 'gtk-print-preview'
::constant GTK_STOCK_PROPERTIES class 'gtk-properties'
::constant GTK_STOCK_QUIT class 'gtk-quit'
::constant GTK_STOCK_REDO class 'gtk-redo'
::constant GTK_STOCK_REFRESH class 'gtk-refresh'
::constant GTK_STOCK_REMOVE class 'gtk-remove'
::constant GTK_STOCK_REVERT_TO_SAVED class 'gtk-revert-to-saved'
::constant GTK_STOCK_SAVE class 'gtk-save'
::constant GTK_STOCK_SAVE_AS class 'gtk-save-as'
::constant GTK_STOCK_SELECT_ALL class 'gtk-select-all'
::constant GTK_STOCK_SELECT_COLOR class 'gtk-select-color'
::constant GTK_STOCK_SELECT_FONT class 'gtk-select-font'
::constant GTK_STOCK_SORT_ASCENDING class 'gtk-sort-ascending'
::constant GTK_STOCK_SORT_DESCENDING class 'gtk-sort-descending'
::constant GTK_STOCK_SPELL_CHECK class 'gtk-spell-check'
::constant GTK_STOCK_STOP class 'gtk-stop'
::constant GTK_STOCK_STRIKETHROUGH class 'gtk-strikethrough'
::constant GTK_STOCK_UNDELETE class 'gtk-undelete'
::constant GTK_STOCK_UNDERLINE class 'gtk-underline'
::constant GTK_STOCK_UNDO class 'gtk-undo'
::constant GTK_STOCK_UNINDENT class 'gtk-unindent'
::constant GTK_STOCK_YES class 'gtk-yes'
::constant GTK_STOCK_ZOOM_100 class 'gtk-zoom-100'
::constant GTK_STOCK_ZOOM_FIT class 'gtk-zoom-fit'
::constant GTK_STOCK_ZOOM_IN class 'gtk-zoom-in'
::constant GTK_STOCK_ZOOM_OUT class 'gtk-zoom-out'
   -- GtkFileCoooserAction
::constant GTK_FILE_CHOOSER_ACTION_OPEN class 0
::constant GTK_FILE_CHOOSER_ACTION_SAVE class 1
::constant GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER class 2
::constant GTK_FILE_CHOOSER_ACTION_CREATE_FOLDER class 3
   -- GtkAssistancePageType
::constant GTK_ASSISTANT_PAGE_CONTENT class 0
::constant GTK_ASSISTANT_PAGE_INTRO class 1
::constant GTK_ASSISTANT_PAGE_CONFIRM class 2
::constant GTK_ASSISTANT_PAGE_SUMMARY class 3
::constant GTK_ASSISTANT_PAGE_PROGRESS class 4
   -- GtkReliefType
::constant GTK_RELIEF_NORMAL class 0
::constant GTK_RELIEF_HALF class 1
::constant GTK_RELIEF_NONO class 2
   -- GtkAtom
::constant GTK_SELECTION_PRIMARY class 1
::constant GTK_SELECTION_SECONDARY class 2
::constant GTK_SELECTION_CLIPBOARD class 69
   -- GtkShadow
::constant GTK_SHADOW_NONE class 0
::constant GTK_SHADOW_IN class 1
::constant GTK_SHADOW_OUT class 2
::constant GTK_SHADOW_ETCHED_IN class 3
::constant GTK_SHADOW_ETCHED_OUT class 4
   -- GtkPositionType
::constant GTK_POS_LEFT class 0
::constant GTK_POS_RIGHT class 1
::constant GTK_POS_TOP class 2
::constant GTK_POS_BOTTOM class 3
   -- GtkIconSize
::constant GTK_ICON_SIZE_INVALID class 0
::constant GTK_ICON_SIZE_MENU class 1
::constant GTK_ICON_SIZE_SMALL_TOOLBAR class 2
::constant GTK_ICON_SIZE_LARGE_TOOLBAR class 3
::constant GTK_ICON_SIZE_BUTTON class 4
::constant GTK_ICON_SIZE_DND class 5
::constant GTK_ICON_SIZE_DIALOG class 6
   -- GtkJustification
::constant GTK_JUSTIFY_LEFT class 0
::constant GTK_JUSTIFY_RIGHT class 1
::constant GTK_JUSTIFY_CENTER class 2
::constant GTK_JUSTIFY_FILL class 3
   -- GtkPolicyType
::constant GTK_POLICY_ALWAYS class 0
::constant GTK_POLICY_AUTOMATIC class 1
::constant GTK_POLICY_NEVER class 2
   -- GtkAttachOptions
::constant GTK_EXPAND class 0
::constant GTK_SHRINK class 1
::constant GTK_FILL class 2
   -- GtkWrapMode
::constant GTK_WRAP_NONE class 0
::constant GTK_WRAP_CHAR class 1
::constant GTK_WRAP_WORD class 2
::constant GTK_WRAP_WORD_CHAR class 3
   -- GtkStateType
::constant GTK_STATE_NORMAL class 0
::constant GTK_STATE_ACTIVE class 1
::constant GTK_STATE_PRELIGHT class 2
::constant GTK_STATE_SELECTED class 3
::constant GTK_STATE_INSENSITIVE class 4
   -- Pango scale factors
::constant PANGO_SCALE_XX_SMALL class 0.5787037037037
::constant PANGO_SCALE_X_SMALL class 0.6444444444444
::constant PANGO_SCALE_SMALL class 0.8333333333333
::constant PANGO_SCALE_MEDIUM class 1.0
::constant PANGO_SCALE_LARGE class 1.2
::constant PANGO_SCALE_X_LARGE class 1.4399999999999
::constant PANGO_SCALE_XX_LARGE class 1.728
   -- PangoWeight
::constant PANGO_WEIGHT_ULTRALIGHT class 200
::constant PANGO_WEIGHT_LIGHT class 300
::constant PANGO_WEIGHT_NORMAL class 400
::constant PANGO_WEIGHT_SEMIBOLD class 600
::constant PANGO_WEIGHT_BOLD class 700
::constant PANGO_WEIGHT_ULTRABOLD class 800
::constant PANGO_WEIGHT_HEAVY class 900
   -- PangoStyle
::constant PANGO_STYLE_NORMAL class 0
::constant PANGO_STYLE_OBLIQUE class 1
::constant PANGO_STYLE_ITALIC class 2
   -- PangoUnderline
::constant PANGO_UNDERLINE_NONE class 0
::constant PANGO_UNDERLINE_SINGLE class 1
::constant PANGO_UNDERLINE_DOUBLE class 2
::constant PANGO_UNDERLINE_LOW class 3
::constant PANGO_UNDERLINE_ERROR class 4
   -- Fundamental GTypes
::constant G_TYPE_INVALID class 0
::constant G_TYPE_NONE class 1
::constant G_TYPE_INTERFACE class 2
::constant G_TYPE_CHAR class 3
::constant G_TYPE_UCHAR class 4
::constant G_TYPE_BOOLEAN class 5
::constant G_TYPE_INT class 6
::constant G_TYPE_UINT class 7
::constant G_TYPE_LONG class 8
::constant G_TYPE_ULONG class 9
::constant G_TYPE_INT64 class 10
::constant G_TYPE_UINT64 class 11
::constant G_TYPE_ENUM class 12
::constant G_TYPE_FLAGS class 13
::constant G_TYPE_FLOAT class 14
::constant G_TYPE_DOUBLE class 15
::constant G_TYPE_STRING class 16
::constant G_TYPE_POINTER class 17
::constant G_TYPE_BOXED class 18
::constant G_TYPE_PARAM class 19
::constant G_TYPE_OBJECT class 20


/*============================================================================*/
/* Class: GtkWidget                                                           */
/* Note:  This is strictly an abstract class. It should never be instantiated */
/*        on its own. You should instantiate one of its subclasses instead.   */
/*============================================================================*/

::class GtkWidget public

::METHOD init EXTERNAL "LIBRARY rexxgtk GrxWidgetNew"

-- This method should only be invoked by the C++ code. That is the only place
-- where it is actually useful.
::method pointer
expose !pointer
return !pointer

-- This attribute is not a part of the GTK system. It has been added to
-- support callbacks or any other use the user may have.
::attribute user_data

::METHOD destroy EXTERNAL "LIBRARY rexxgtk GrxWidgetDestroy"
::METHOD font EXTERNAL "LIBRARY rexxgtk GrxWidgetGetFont"
::METHOD get_font EXTERNAL "LIBRARY rexxgtk GrxWidgetGetFont"
::METHOD 'font=' EXTERNAL "LIBRARY rexxgtk GrxWidgetSetFont"
::METHOD set_font EXTERNAL "LIBRARY rexxgtk GrxWidgetSetFont"
::METHOD modify_font EXTERNAL "LIBRARY rexxgtk GrxWidgetSetFont"
::METHOD grab_focus EXTERNAL "LIBRARY rexxgtk GrxWidgetGrabFocus"
::METHOD hide EXTERNAL "LIBRARY rexxgtk GrxWidgetHide"
::METHOD hide_all EXTERNAL "LIBRARY rexxgtk GrxWidgetHideAll"
::METHOD set_size_request EXTERNAL "LIBRARY rexxgtk GrxWidgetSetSize"
::METHOD show EXTERNAL "LIBRARY rexxgtk GrxWidgetShow"
::METHOD show_all EXTERNAL "LIBRARY rexxgtk GrxWidgetShowAll"
::METHOD name EXTERNAL "LIBRARY rexxgtk GrxWidgetGetName"
::METHOD get_name EXTERNAL "LIBRARY rexxgtk GrxWidgetGetName"
::METHOD 'name=' EXTERNAL "LIBRARY rexxgtk GrxWidgetSetName"
::METHOD set_name EXTERNAL "LIBRARY rexxgtk GrxWidgetSetName"
::METHOD 'sensitive=' EXTERNAL "LIBRARY rexxgtk GrxWidgetSetSensitive"
::METHOD set_sensitive EXTERNAL "LIBRARY rexxgtk GrxWidgetSetSensitive"
::METHOD modify_fg EXTERNAL "LIBRARY rexxgtk GrxWidgetModifyFG"
::METHOD modify_bg EXTERNAL "LIBRARY rexxgtk GrxWidgetModifyBG"
::METHOD 'data=' EXTERNAL "LIBRARY rexxgtk GrxWidgetSetData"
::METHOD set_data EXTERNAL "LIBRARY rexxgtk GrxWidgetSetData"
::METHOD data EXTERNAL "LIBRARY rexxgtk GrxWidgetGetData"
::METHOD get_data EXTERNAL "LIBRARY rexxgtk GrxWidgetGetData"
::METHOD activate EXTERNAL "LIBRARY rexxgtk GrxWidgetActivate'
::METHOD reparent EXTERNAL "LIBRARY rexxgtk GrxWidgetReparent'
::METHOD is_focus EXTERNAL "LIBRARY rexxgtk GrxWidgetIsFocus'
::METHOD grab_default EXTERNAL "LIBRARY rexxgtk GrxWidgetGrabDefault'
::METHOD set_parent_window EXTERNAL "LIBRARY rexxgtk GrxWidgetSetParentWindow'
::METHOD get_parent_window EXTERNAL "LIBRARY rexxgtk GrxWidgetGetParentWindow'
::METHOD parent_window EXTERNAL "LIBRARY rexxgtk GrxWidgetGetParentWindow'
::METHOD get_toplevel EXTERNAL "LIBRARY rexxgtk GrxWidgetGetToplevel'
::METHOD toplevel EXTERNAL "LIBRARY rexxgtk GrxWidgetGetToplevel'
::METHOD get_ancestor EXTERNAL "LIBRARY rexxgtk GrxWidgetGetAncestor'
::METHOD ancestor EXTERNAL "LIBRARY rexxgtk GrxWidgetGetAncestor'
::METHOD is_ancestor EXTERNAL "LIBRARY rexxgtk GrxWidgetIsAncestor'
::METHOD 'direction=' EXTERNAL "LIBRARY rexxgtk GrxWidgetSetDirection'
::METHOD set_direction EXTERNAL "LIBRARY rexxgtk GrxWidgetSetDirection'
::METHOD get_direction EXTERNAL "LIBRARY rexxgtk GrxWidgetGetDirection'
::METHOD direction EXTERNAL "LIBRARY rexxgtk GrxWidgetGetDirection'
::METHOD get_default_direction EXTERNAL "LIBRARY rexxgtk GrxWidgetGetDefaultDirection'
::METHOD default_direction EXTERNAL "LIBRARY rexxgtk GrxWidgetGetDefaultDirection'
::METHOD queue_draw_area EXTERNAL "LIBRARY rexxgtk GrxWidgetQueueDrawArea"
::METHOD reset_shapes EXTERNAL "LIBRARY rexxgtk GrxWidgetResetShapes"
::METHOD set_scroll_adjustments EXTERNAL "LIBRARY rexxgtk GrxWidgetSetScrollAdjustments"
::METHOD mnemonic_activate EXTERNAL "LIBRARY rexxgtk GrxWidgetMnemonicActivate"
::METHOD get_accessible EXTERNAL "LIBRARY rexxgtk GrxWidgetGetAccessible"
::METHOD get_child_visible EXTERNAL "LIBRARY rexxgtk GrxWidgetGetChildVisible"
::METHOD child_visible EXTERNAL "LIBRARY rexxgtk GrxWidgetGetChildVisible"
::METHOD get_parent EXTERNAL "LIBRARY rexxgtk GrxWidgetGetParent"
::METHOD parent EXTERNAL "LIBRARY rexxgtk GrxWidgetGetParent"
::METHOD get_root_window EXTERNAL "LIBRARY rexxgtk GrxWidgetGetRootWindow"
::METHOD root_window EXTERNAL "LIBRARY rexxgtk GrxWidgetGetRootWindow"
::METHOD signal_connect EXTERNAL "LIBRARY rexxgtk GrxWidgetSignalConnect"

-- The following are the methods that can be connected to signals. By default
-- they do nothing.

::method signal_destroy
return


/*============================================================================*/
/* Class: GtkContainer                                                        */
/* Note:  This is strictly an abstract class. It should never be instantiated */
/*        on its own. You should instantiate one of its subclasses instead.   */
/*============================================================================*/

::class GtkContainer public subclass GtkWidget

::method init
return

::METHOD add EXTERNAL "LIBRARY rexxgtk GrxContainerAdd"
::METHOD remove EXTERNAL "LIBRARY rexxgtk GrxContainerRemove"
::METHOD set_border_width EXTERNAL "LIBRARY rexxgtk GrxContainerSetBorderWidth"
::METHOD 'border_width=' EXTERNAL "LIBRARY rexxgtk GrxContainerSetBorderWidth"
::METHOD signal_connect EXTERNAL "LIBRARY rexxgtk GrxContainerSignalConnect"

-- The following are the methods that can be connected to signals. By default
-- they do nothing.

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
/* Note:  This is strictly an abstract class. It should never be instantiated */
/*        on its own. You should instantiate one of its subclasses instead.   */
/*============================================================================*/

::class GtkBox public subclass GtkContainer

::method init
return

::METHOD pack_end EXTERNAL "LIBRARY rexxgtk GrxBoxPackEnd"

::method pack_end_defaults
-- insert an object into the box with default attributes
use strict arg mywidget
self~pack_end(myWidget, .true, .true, 0)
return

::METHOD pack_start EXTERNAL "LIBRARY rexxgtk GrxBoxPackEnd"

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

::METHOD init EXTERNAL "LIBRARY rexxgtk GrxHBoxNew"

::method signal_connect
use strict arg event
return self~signal_connect:super(event)


/*============================================================================*/
/* Class: GtkHBox_From_Pointer                                                */
/*============================================================================*/

::class GtkHBox_From_Pointer private subclass GtkHBox

::method init
return


/*============================================================================*/
/* Class: GtkVBox                                                             */
/*============================================================================*/

::class GtkVBox public subclass GtkBox

::METHOD init EXTERNAL "LIBRARY rexxgtk GrxVBoxNew"

::method signal_connect
use strict arg event
return self~signal_connect:super(event)


/*============================================================================*/
/* Class: GtkVBox_From_Pointer                                                */
/*============================================================================*/

::class GtkVBox_From_Pointer private subclass GtkVBox

::METHOD init EXTERNAL "LIBRARY rexxgtk GrxVBoxNewFromPointer"
return


/*============================================================================*/
/* Class: GtkAlignment                                                        */
/*============================================================================*/

::class GtkAlignment public subclass GtkContainer

::METHOD init EXTERNAL "LIBRARY rexxgtk GrxAlignmentNew"

::method signal_connect
use strict arg event
return self~signal_connect:super(event)


/*============================================================================*/
/* Class: GtkPaned                                                            */
/* Note:  This is strictly an abstract class. It should never be instantiated */
/*        on its own. You should instantiate one of its subclasses instead.   */
/*============================================================================*/

::class GtkPaned public subclass GtkContainer

::method init
return

::METHOD add1 EXTERNAL "LIBRARY rexxgtk GrxPanedAdd1"
::METHOD add2 EXTERNAL "LIBRARY rexxgtk GrxPanedAdd2"
::METHOD pack1 EXTERNAL "LIBRARY rexxgtk GrxPanedPack1"
::METHOD pack2 EXTERNAL "LIBRARY rexxgtk GrxPanedPack2"

-- The following are the methods that can be connected to signals. By default
-- they do nothing.

::method signal_connect
use strict arg event
return self~signal_connect:super(event)


/*============================================================================*/
/* Class: GtkHPaned                                                           */
/*============================================================================*/

::class GtkHPaned public subclass GtkPaned

::METHOD init EXTERNAL "LIBRARY rexxgtk GrxHPanedNew"

::method signal_connect
use strict arg event
return self~signal_connect:super(event)


/*============================================================================*/
/* Class: GtkVPaned                                                           */
/*============================================================================*/

::class GtkVPaned public subclass GtkPaned

::METHOD init EXTERNAL "LIBRARY rexxgtk GrxVPanedNew"

::method signal_connect
use strict arg event
return self~signal_connect:super(event)


/*============================================================================*/
/* Class: GtkWindow                                                           */
/*============================================================================*/

::class GtkWindow public subclass GtkContainer

::METHOD init EXTERNAL "LIBRARY rexxgtk GrxWindowNew"
::METHOD 'title=' EXTERNAL "LIBRARY rexxgtk GrxWindowSetTitle"
::METHOD set_title EXTERNAL "LIBRARY rexxgtk GrxWindowSetTitle"
::METHOD 'modal=' EXTERNAL "LIBRARY rexxgtk GrxWindowSetModal"
::METHOD set_modal EXTERNAL "LIBRARY rexxgtk GrxWindowSetModal"
::METHOD signal_connect EXTERNAL "LIBRARY rexxgtk GrxWindowSignalConnect"

-- The following are the methods that can be connected to signals. By default
-- they do nothing.

::method signal_activate_default
return

::method signal_activate_focus
return

::method signal_keys_changed
return

::method signal_set_focus
return

/*============================================================================*/
/* Class: GtkButton                                                           */
/*============================================================================*/

::class GtkButton public subclass GtkContainer

::METHOD init_p private EXTERNAL "LIBRARY rexxgtk GrxButtonNew"

::method init
use strict arg self~label = ''
self~init_p
return

::METHOD label EXTERNAL "LIBRARY rexxgtk GrxButtonGetLabel"
::METHOD get_label EXTERNAL "LIBRARY rexxgtk GrxButtonGetLabel"
::METHOD 'label=' EXTERNAL "LIBRARY rexxgtk GrxButtonSetLabel"
::METHOD set_label EXTERNAL "LIBRARY rexxgtk GrxButtonSetLabel"
::METHOD set_use_underline EXTERNAL "LIBRARY rexxgtk GrxButtonUseUnderline"
::METHOD 'relief=' EXTERNAL "LIBRARY rexxgtk GrxButtonSetRelief"
::METHOD set_relief EXTERNAL "LIBRARY rexxgtk GrxButtonSetRelief"
::METHOD signal_connect EXTERNAL "LIBRARY rexxgtk GrxButtonSignalConnect"

-- The following are the methods that can be connected to signals. By default
-- they do nothing.

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
/* Class: GtkButton_From_Stock                                                */
/*============================================================================*/

::class GtkButton_From_Stock public subclass GtkButton

::METHOD init EXTERNAL "LIBRARY rexxgtk GrxButtonNewFromStock"


/*============================================================================*/
/* Class: GtkToggleButton                                                      */
/*============================================================================*/

::class GtkToggleButton public subclass GtkButton

::METHOD init EXTERNAL "LIBRARY rexxgtk GrxToggleButtonNew"
::METHOD mode EXTERNAL "LIBRARY rexxgtk GrxToggleButtonGetMode"
::METHOD get_mode EXTERNAL "LIBRARY rexxgtk GrxToggleButtonGetMode"
::METHOD set_mode EXTERNAL "LIBRARY rexxgtk GrxToggleButtonSetMode"
::METHOD 'mode=' EXTERNAL "LIBRARY rexxgtk GrxToggleButtonSetMode"
::METHOD active EXTERNAL "LIBRARY rexxgtk GrxToggleButtonGetActive"
::METHOD get_active EXTERNAL "LIBRARY rexxgtk GrxToggleButtonGetActive"
::METHOD active EXTERNAL "LIBRARY rexxgtk GrxToggleButtonSetActive"
::METHOD 'active=' EXTERNAL "LIBRARY rexxgtk GrxToggleButtonSetActive"
::METHOD inconsistent EXTERNAL "LIBRARY rexxgtk GrxToggleButtonGetInconsistent"
::METHOD get_inconsistent EXTERNAL "LIBRARY rexxgtk GrxToggleButtonGetInconsistent"
::METHOD set_inconsistent EXTERNAL "LIBRARY rexxgtk GrxToggleButtonSetInconsistent"
::METHOD 'inconsistent=' EXTERNAL "LIBRARY rexxgtk GrxToggleButtonSetInconsistent"
::METHOD signal_connect EXTERNAL "LIBRARY rexxgtk GrxToggleButtonSignalConnect"

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

::METHOD init EXTERNAL "LIBRARY rexxgtk GrxCheckButtonNew"

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

::METHOD init EXTERNAL "LIBRARY rexxgtk GrxRadioButtonNew"
::METHOD group EXTERNAL "LIBRARY rexxgtk GrxRadioButtonGetGroup"
::METHOD get_group EXTERNAL "LIBRARY rexxgtk GrxRadioButtonGetGroup"
::METHOD set_group EXTERNAL "LIBRARY rexxgtk GrxRadioButtonSetGroup"
::METHOD 'group=' EXTERNAL "LIBRARY rexxgtk GrxRadioButtonSetGroup"
::METHOD signal_connect EXTERNAL "LIBRARY rexxgtk GrxRadioButtonSignalConnect"

-- The following are the methods that can be connected to signals. By default
-- they do nothing.

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

::METHOD init EXTERNAL "LIBRARY rexxgtk GrxRadioButtonFromWidget"

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

::METHOD init EXTERNAL "LIBRARY rexxgtk GrxColorButtonNew"
::METHOD 'title=' EXTERNAL "LIBRARY rexxgtk GrxColorButtonSetTitle"
::METHOD set_title EXTERNAL "LIBRARY rexxgtk GrxColorButtonSetTitle"
::METHOD color EXTERNAL "LIBRARY rexxgtk GrxColorButtonGetColor"
::METHOD get_color EXTERNAL "LIBRARY rexxgtk GrxColorButtonGetColor"
::METHOD set_color EXTERNAL "LIBRARY rexxgtk GrxColorButtonSetColor"
::METHOD 'color=' EXTERNAL "LIBRARY rexxgtk GrxColorButtonSetColor"
::METHOD signal_connect EXTERNAL "LIBRARY rexxgtk GrxColorButtonSetColor"

-- The following are the methods that can be connected to signals. By default
-- they do nothing.

::method signal_color_set
return


/*============================================================================*/
/* Class: GtkFileChooserButton                                                */
/*============================================================================*/

::class GtkFileChooserButton public subclass GtkHBox inherit GtkFileChooser

::METHOD init EXTERNAL "LIBRARY rexxgtk GrxFileChooserButtonNew"
::METHOD signal_connect_p private EXTERNAL "LIBRARY rexxgtk GrxFileChooserButtonSignalConnect"

::METHOD signal_connect
use strict arg event
-- first try the GtkFileChooser class hiearchy first
if self~signal_connect:.GtkFileChooser(event) = .true then return .true
-- now try the standard hiearchy
return self~signal_connect_p

-- The following are the methods that can be connected to signals. By default
-- they do nothing.

::method signal_file_set
return


/*============================================================================*/
/* Class: GtkFontButton                                                       */
/*============================================================================*/

::class GtkFontButton public subclass GtkButton

::METHOD init EXTERNAL "LIBRARY rexxgtk GrxFontButtonNew"
::METHOD 'title=' EXTERNAL "LIBRARY rexxgtk GrxFontButtonSetTitle"
::METHOD set_title EXTERNAL "LIBRARY rexxgtk GrxFontButtonSetTitle"
::METHOD font_name EXTERNAL "LIBRARY rexxgtk GrxFontButtonGetFontName"
::METHOD get_font_name EXTERNAL "LIBRARY rexxgtk GrxFontButtonGetFontName"
::METHOD signal_connect EXTERNAL "LIBRARY rexxgtk GrxFontButtonSignalConnect"

-- The following are the methods that can be connected to signals. By default
-- they do nothing.

::method signal_font_set
return


/*============================================================================*/
/* Class: GtkTable                                                            */
/*============================================================================*/

::class GtkTable public subclass GtkContainer

::METHOD init EXTERNAL "LIBRARY rexxgtk GrxTableNew"
::METHOD attach EXTERNAL "LIBRARY rexxgtk GrxTableAttach"

::method attach_defaults
use strict arg widget, left, right, top, bottom
self~attach(widget, left, right, top, bottom,5, 5, 0, 0)
return

::METHOD 'row_spacings=' EXTERNAL "LIBRARY rexxgtk GrxTableSetRowSpacings"
::METHOD set_row_spacings EXTERNAL "LIBRARY rexxgtk GrxTableSetRowSpacings"
::METHOD 'col_spacings=' EXTERNAL "LIBRARY rexxgtk GrxTableSetColSpacings"
::METHOD set_col_spacings EXTERNAL "LIBRARY rexxgtk GrxTableSetColSpacings"

::method signal_connect
use strict arg event
return self~signal_connect:super(event)


/*============================================================================*/
/* Class: GtkComboBox                                                         */
/*============================================================================*/

::class GtkComboBox public subclass GtkWidget

::METHOD init EXTERNAL "LIBRARY rexxgtk GrxComboBoxNew"
::METHOD append_text EXTERNAL "LIBRARY rexxgtk GrxComboBoxAppendText"
::METHOD get_active_text EXTERNAL "LIBRARY rexxgtk GrxComboBoxGetActiveText"
::METHOD active_text EXTERNAL "LIBRARY rexxgtk GrxComboBoxGetActiveText"
::METHOD get_active EXTERNAL "LIBRARY rexxgtk GrxComboBoxGetActive"
::METHOD active EXTERNAL "LIBRARY rexxgtk GrxComboBoxGetActive"
::METHOD set_active EXTERNAL "LIBRARY rexxgtk GrxComboBoxSetActive"
::METHOD 'active=' EXTERNAL "LIBRARY rexxgtk GrxComboBoxSetActive"
::METHOD signal_connect EXTERNAL "LIBRARY rexxgtk GrxComboBoxSignalConnect"

-- The following are the methods that can be connected to signals. By default
-- they do nothing.

::method signal_changed
return

::method signal_move_active
return

::method signal_popdown
return

::method signal_popup
return


/*============================================================================*/
/* Class: GtkComboBox_Text                                                    */
/*============================================================================*/

::class GtkComboBox_Text public subclass GtkComboBox

::METHOD init EXTERNAL "LIBRARY rexxgtk GrxComboBoxNewText"


/*============================================================================*/
/* Class: GtkEntry                                                            */
/*============================================================================*/

::class GtkEntry public subclass GtkWidget

::METHOD init EXTERNAL "LIBRARY rexxgtk GrxEntryNew"
::METHOD max_length EXTERNAL "LIBRARY rexxgtk GrxEntryGetMaxLength"
::METHOD get_max_length EXTERNAL "LIBRARY rexxgtk GrxEntryGetMaxLength"
::METHOD set_max_length EXTERNAL "LIBRARY rexxgtk GrxEntrySetMaxLength"
::METHOD 'max_length=' EXTERNAL "LIBRARY rexxgtk GrxEntrySetMaxLength"
::METHOD set_width_chars EXTERNAL "LIBRARY rexxgtk GrxEntrySetWidthChars"
::METHOD 'width_chars=' EXTERNAL "LIBRARY rexxgtk GrxEntrySetWidthChars"
::METHOD text EXTERNAL "LIBRARY rexxgtk GrxEntryGetText"
::METHOD get_text EXTERNAL "LIBRARY rexxgtk GrxEntryGetText"
::METHOD set_text EXTERNAL "LIBRARY rexxgtk GrxEntrySetText"
::METHOD 'text=' EXTERNAL "LIBRARY rexxgtk GrxEntrySetText"
::METHOD visibility EXTERNAL "LIBRARY rexxgtk GrxEntryGetVisibility"
::METHOD get_visibility EXTERNAL "LIBRARY rexxgtk GrxEntryGetVisibility"
::METHOD set_visibility EXTERNAL "LIBRARY rexxgtk GrxEntrySetVisibility"
::METHOD 'visibility=' EXTERNAL "LIBRARY rexxgtk GrxEntrySetVisibility"
::METHOD invisile_char EXTERNAL "LIBRARY rexxgtk GrxEntryGetInvisibleChar"
::METHOD get_invisile_char EXTERNAL "LIBRARY rexxgtk GrxEntryGetInvisibleChar"
::METHOD set_invisile_char EXTERNAL "LIBRARY rexxgtk GrxEntrySetInvisibleChar"
::METHOD 'invisile_char=' EXTERNAL "LIBRARY rexxgtk GrxEntrySetInvisibleChar"
::METHOD signal_connect EXTERNAL "LIBRARY rexxgtk GrxEntrySignalConnect"

-- The following are the methods that can be connected to signals. By default
-- they do nothing.

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
/* Class: GtkSpinButton                                                       */
/*============================================================================*/

::class GtkSpinButton public subclass GtkEntry

::METHOD init EXTERNAL "LIBRARY rexxgtk GrxSpinButtonNew"
::METHOD sifnal_connect EXTERNAL "LIBRARY rexxgtk GrxSpinButtonSignalConnect"

-- The following are the methods that can be connected to signals. By default
-- they do nothing.

::method signal_change_value
return

::method signal_value_changed
return

::method signal_wrapped
return


/*============================================================================*/
/* Class: GtkDialog                                                           */
/*============================================================================*/

::class GtkDialog public subclass GtkWindow

::METHOD init EXTERNAL "LIBRARY rexxgtk GrxDialogNew"

::method vbox
expose vbox
return vbox

::METHOD 'has_separator=' EXTERNAL "LIBRARY rexxgtk GrxDialogSetHasSeparator"
::METHOD set_has_separator EXTERNAL "LIBRARY rexxgtk GrxDialogSetHasSeparator"
::METHOD 'default_response=' EXTERNAL "LIBRARY rexxgtk GrxDialogSetDefaultResponse"
::METHOD set_default_response EXTERNAL "LIBRARY rexxgtk GrxDialogSetDefaultResponse"
::METHOD add_button EXTERNAL "LIBRARY rexxgtk GrxDialogAddButton"
-- Note: the name of this method cannot be 'run' as that would override the
-- private 'run' instance method of the ooRexx Object class.
::METHOD dialog_run EXTERNAL "LIBRARY rexxgtk GrxDialogRun"
::METHOD signal_connect EXTERNAL "LIBRARY rexxgtk GrxDialogSignalConnect"

-- The following are the methods that can be connected to signals. By default
-- they do nothing.

::method signal_close
return

::method signal_response
return


/*============================================================================*/
/* Class: GtkDialog_With_Buttons                                              */
/*============================================================================*/

::class GtkDialog_With_Buttons public subclass GtkDialog

::METHOD init EXTERNAL "LIBRARY rexxgtk GrxDialogNewWithButtons"


/*============================================================================*/
/* Class: GtkMessageDialog                                                    */
/*============================================================================*/

::class GtkMessageDialog public subclass GtkDialog

::METHOD init EXTERNAL "LIBRARY rexxgtk GrxMessageDialogNew"


/*============================================================================*/
/* Class: GtkColorSelectionDialog                                             */
/*============================================================================*/

::class GtkColorSelectionDialog public subclass GtkDialog

::METHOD init EXTERNAL "LIBRARY rexxgtk GrxColorSelectionDialogNew"
::METHOD color EXTERNAL "LIBRARY rexxgtk GrxColorSelectionDialogGetColor"
::METHOD get_color EXTERNAL "LIBRARY rexxgtk GrxColorSelectionDialogGetColor"


/*============================================================================*/
/* Class: GtkFontSelectionDialog                                              */
/*============================================================================*/

::class GtkFontSelectionDialog public subclass GtkDialog

::METHOD init EXTERNAL "LIBRARY rexxgtk GrxFontSelectionDialogNew"
::METHOD 'font_name=' EXTERNAL "LIBRARY rexxgtk GrxFontSelectionDialogSetFontName"
::METHOD set_font_name EXTERNAL "LIBRARY rexxgtk GrxFontSelectionDialogSetFontName"
::METHOD font_name EXTERNAL "LIBRARY rexxgtk GrxFontSelectionDialogGetFontName"
::METHOD get_font_name EXTERNAL "LIBRARY rexxgtk GrxFontSelectionDialogGetFontName"
::METHOD 'preview_text=' EXTERNAL "LIBRARY rexxgtk GrxFontSelectionDialogSetPreviewText"
::METHOD set_preview_text EXTERNAL "LIBRARY rexxgtk GrxFontSelectionDialogSetPreviewText"


/*============================================================================*/
/* Class: GtkAboutDialog                                                      */
/*============================================================================*/

::class GtkAboutDialog public subclass GtkDialog

::METHOD init EXTERNAL "LIBRARY rexxgtk GrxAboutDialogNew"
::METHOD 'program_name=' EXTERNAL "LIBRARY rexxgtk GrxAboutDialogSetProgramName"
::METHOD set_program_name EXTERNAL "LIBRARY rexxgtk GrxAboutDialogSetProgramName"
::METHOD 'version=' EXTERNAL "LIBRARY rexxgtk GrxAboutDialogSetVersion"
::METHOD set_version EXTERNAL "LIBRARY rexxgtk GrxAboutDialogSetVersion"
::METHOD 'copyright=' EXTERNAL "LIBRARY rexxgtk GrxAboutDialogSetCopyright"
::METHOD set_copyright EXTERNAL "LIBRARY rexxgtk GrxAboutDialogSetCopyright"
::METHOD 'comments=' EXTERNAL "LIBRARY rexxgtk GrxAboutDialogSetComments"
::METHOD set_comments EXTERNAL "LIBRARY rexxgtk GrxAboutDialogSetComments"
::METHOD 'license=' EXTERNAL "LIBRARY rexxgtk GrxAboutDialogSetLicense"
::METHOD set_license EXTERNAL "LIBRARY rexxgtk GrxAboutDialogSetLicense"
::METHOD 'wrap_license=' EXTERNAL "LIBRARY rexxgtk GrxAboutDialogSetWrapLicense"
::METHOD set_wrap_license EXTERNAL "LIBRARY rexxgtk GrxAboutDialogSetWrapLicense"
::METHOD 'website=' EXTERNAL "LIBRARY rexxgtk GrxAboutDialogSetWebsite"
::METHOD set_website EXTERNAL "LIBRARY rexxgtk GrxAboutDialogSetWebsite"
::METHOD 'website_label=' EXTERNAL "LIBRARY rexxgtk GrxAboutDialogSetWebsiteLabel"
::METHOD set_website_label EXTERNAL "LIBRARY rexxgtk GrxAboutDialogSetWebsiteLabel"
::METHOD set_authors EXTERNAL "LIBRARY rexxgtk GrxAboutDialogSetAuthors"
::METHOD set_artists EXTERNAL "LIBRARY rexxgtk GrxAboutDialogSetArtists"
::METHOD set_documentors EXTERNAL "LIBRARY rexxgtk GrxAboutDialogSetDocumentors"
::METHOD 'logo=' EXTERNAL "LIBRARY rexxgtk GrxAboutDialogSetLogo"
::METHOD set_logo EXTERNAL "LIBRARY rexxgtk GrxAboutDialogSetLogo"


/*============================================================================*/
/* Class: GtkFileChooserDialog                                                */
/*============================================================================*/

::class GtkFileChooserDialog public subclass GtkDialog inherit GtkFileChooser

::METHOD init EXTERNAL "LIBRARY rexxgtk GrxFileChooserDialogNew"


/*============================================================================*/
/* Class: GtkAssistant                                                        */
/*============================================================================*/

::class GtkAssistant public subclass GtkWindow

::METHOD init EXTERNAL "LIBRARY rexxgtk GrxAssistantNew"
::METHOD set_current_page EXTERNAL "LIBRARY rexxgtk GrxAssistantSetCurrentPage"
::METHOD 'current_page=' EXTERNAL "LIBRARY rexxgtk GrxAssistantSetCurrentPage"
::METHOD get_current_page EXTERNAL "LIBRARY rexxgtk GrxAssistantGetCurrentPage"
::METHOD current_page EXTERNAL "LIBRARY rexxgtk GrxAssistantGetCurrentPage"
::METHOD get_nth_page EXTERNAL "LIBRARY rexxgtk GrxAssistantGetNthPage"
::METHOD nth_page EXTERNAL "LIBRARY rexxgtk GrxAssistantGetNthPage"
::METHOD prepend_page EXTERNAL "LIBRARY rexxgtk GrxAssistantPrependPage"
::METHOD append_page EXTERNAL "LIBRARY rexxgtk GrxAssistantAppendPage"
::METHOD insert_page EXTERNAL "LIBRARY rexxgtk GrxAssistantInsertPage"
::METHOD set_page_type EXTERNAL "LIBRARY rexxgtk GrxAssistantSetPageType"
::METHOD set_page_complete EXTERNAL "LIBRARY rexxgtk GrxAssistantSetPageComplete"
::METHOD set_page_complete EXTERNAL "LIBRARY rexxgtk GrxAssistantSetPageComplete"
::METHOD set_page_header_image EXTERNAL "LIBRARY rexxgtk GrxAssistantSetPageHeaderImage"
::METHOD set_page_side_image EXTERNAL "LIBRARY rexxgtk GrxAssistantSetPageSideImage"
::METHOD signal_connect EXTERNAL "LIBRARY rexxgtk GrxAssistantSignalConnect"

-- The following are the methods that can be connected to signals. By default
-- they do nothing.

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

::METHOD init EXTERNAL "LIBRARY rexxgtk GrxLabelNew"
::METHOD get_justify EXTERNAL "LIBRARY rexxgtk GrxLabelGetJustify"
::METHOD justify EXTERNAL "LIBRARY rexxgtk GrxLabelGetJustify"
::METHOD 'justify=' EXTERNAL "LIBRARY rexxgtk GrxLabelSetJustify"
::METHOD set_justify EXTERNAL "LIBRARY rexxgtk GrxLabelSetJustify"
::METHOD set_text EXTERNAL "LIBRARY rexxgtk GrxLabelSetText"
::METHOD 'text=' EXTERNAL "LIBRARY rexxgtk GrxLabelSetText"
::METHOD set_selectable EXTERNAL "LIBRARY rexxgtk GrxLabelSetSelectable"
::METHOD 'selectable=' EXTERNAL "LIBRARY rexxgtk GrxLabelSetSelectable"
::METHOD signal_connect EXTERNAL "LIBRARY rexxgtk GrxLabelSignalConnect"

-- The following are the methods that can be connected to signals. By default
-- they do nothing.

::method signal_copy_clipboard
return

::method signal_copy_move_cursor
return

::method signal_copy_populate_popup
return


/*============================================================================*/
/* Class: GtkFixed                                                            */
/*============================================================================*/

::class GtkFixed public subclass GtkContainer

::METHOD init EXTERNAL "LIBRARY rexxgtk GrxFixedNew"
::METHOD put EXTERNAL "LIBRARY rexxgtk GrxFixedPut"
::METHOD move EXTERNAL "LIBRARY rexxgtk GrxFixedMove"

::method signal_connect
use strict arg event
return self~signal_connect:super(event)


/*============================================================================*/
/* Class: GtkExpander                                                         */
/*============================================================================*/

::class GtkExpander public subclass GtkContainer

::METHOD init EXTERNAL "LIBRARY rexxgtk GrxExpanderNew"
::METHOD set_expanded EXTERNAL "LIBRARY rexxgtk GrxExpanderSetExpanded"
::METHOD 'expanded=' EXTERNAL "LIBRARY rexxgtk GrxExpanderSetExpanded"
::METHOD label EXTERNAL "LIBRARY rexxgtk GrxExpanderGetLabel"
::METHOD get_label EXTERNAL "LIBRARY rexxgtk GrxExpanderGetLabel"
::METHOD set_label EXTERNAL "LIBRARY rexxgtk GrxExpanderSetLabel"
::METHOD 'label=' EXTERNAL "LIBRARY rexxgtk GrxExpanderSetLabel"
::METHOD set_use_underline EXTERNAL "LIBRARY rexxgtk GrxExpanderSetUseUnderline"
::METHOD 'use_underline=' EXTERNAL "LIBRARY rexxgtk GrxExpanderSetUseUnderline"
::METHOD set_spacing EXTERNAL "LIBRARY rexxgtk GrxExpanderSetSpacing"
::METHOD 'spacing=' EXTERNAL "LIBRARY rexxgtk GrxExpanderSetSpacing"
::METHOD signal_connect EXTERNAL "LIBRARY rexxgtk GrxExpanderSignalSpacing"

-- The following are the methods that can be connected to signals. By default
-- they do nothing.

::method signal_activate
return


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

::METHOD init EXTERNAL "LIBRARY rexxgtk GrxHandleBoxNew"
::METHOD set_shadow_type EXTERNAL "LIBRARY rexxgtk GrxHandleBoxSetShadowType"
::METHOD 'shadow_type=' EXTERNAL "LIBRARY rexxgtk GrxHandleBoxSetShadowType"
::METHOD set_position EXTERNAL "LIBRARY rexxgtk GrxHandleBoxSetPosition"
::METHOD 'position=' EXTERNAL "LIBRARY rexxgtk GrxHandleBoxSetPosition"
::METHOD set_snap_edge EXTERNAL "LIBRARY rexxgtk GrxHandleBoxSetSnapEdge"
::METHOD 'snap_edge=' EXTERNAL "LIBRARY rexxgtk GrxHandleBoxSetSnapEdge"
::METHOD signal_connect EXTERNAL "LIBRARY rexxgtk GrxHandleBoxSignalConnect"

-- The following are the methods that can be connected to signals. By default
-- they do nothing.

::method signal_child_attached
return

::method signal_child_detached
return


/*============================================================================*/
/* Class: GtkNotebook                                                         */
/*============================================================================*/

::class GtkNotebook public subclass GtkContainer

::METHOD init EXTERNAL "LIBRARY rexxgtk GrxNotebookNew"
::METHOD append_page EXTERNAL "LIBRARY rexxgtk GrxNotebookAppendPage"
::METHOD prepend_page EXTERNAL "LIBRARY rexxgtk GrxNotebookPrependPage"
::METHOD insert_page EXTERNAL "LIBRARY rexxgtk GrxNotebookInsertPage"
::METHOD append_page_menu EXTERNAL "LIBRARY rexxgtk GrxNotebookAppendPageMenu"
::METHOD prepend_page_menu EXTERNAL "LIBRARY rexxgtk GrxNotebookPrependPageMenu"
::METHOD insert_page_menu EXTERNAL "LIBRARY rexxgtk GrxNotebookInsertPageMenu"
::METHOD remove_page EXTERNAL "LIBRARY rexxgtk GrxNotebookRemovePage"
::METHOD set_show_border EXTERNAL "LIBRARY rexxgtk GrxNotebookSetShowBorder"
::METHOD 'show_border=' EXTERNAL "LIBRARY rexxgtk GrxNotebookSetShowBorder"
::METHOD set_show_tabs EXTERNAL "LIBRARY rexxgtk GrxNotebookSetShowTabs"
::METHOD 'show_tabs=' EXTERNAL "LIBRARY rexxgtk GrxNotebookSetShowTabs"
::METHOD set_tab_pos EXTERNAL "LIBRARY rexxgtk GrxNotebookSetTabPos"
::METHOD 'tab_pos=' EXTERNAL "LIBRARY rexxgtk GrxNotebookSetTabPos"
::METHOD set_homogeneous_tabs EXTERNAL "LIBRARY rexxgtk GrxNotebookSetHomogeneousTabs"
::METHOD 'homogeneous_tabs=' EXTERNAL "LIBRARY rexxgtk GrxNotebookSetHomogeneousTabs"
::METHOD set_tab_border EXTERNAL "LIBRARY rexxgtk GrxNotebookSetTabBorder"
::METHOD 'tab_border=' EXTERNAL "LIBRARY rexxgtk GrxNotebookSetTabBorder"
::METHOD set_scrollable EXTERNAL "LIBRARY rexxgtk GrxNotebookSetScrollable"
::METHOD 'scrollable=' EXTERNAL "LIBRARY rexxgtk GrxNotebookSetScrollable"
::METHOD popup_enable EXTERNAL "LIBRARY rexxgtk GrxNotebookPopupEnable"
::METHOD popup_disable EXTERNAL "LIBRARY rexxgtk GrxNotebookPopupDisable"
::METHOD set_current_page EXTERNAL "LIBRARY rexxgtk GrxNotebookSetCurrentPage"
::METHOD 'current_page=' EXTERNAL "LIBRARY rexxgtk GrxNotebookSetCurrentPage"
::METHOD get_current_page EXTERNAL "LIBRARY rexxgtk GrxNotebookGetCurrentPage"
::METHOD current_page EXTERNAL "LIBRARY rexxgtk GrxNotebookGetCurrentPage"
::METHOD signal_connect EXTERNAL "LIBRARY rexxgtk GrxNotebookSignalConnect"

-- The following are the methods that can be connected to signals. By default
-- they do nothing.

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
/* Class: GtkRange                                                            */
/* Note:  This is strictly a base class. It should never be instantiated on   */
/*        its own. You should instantiate one of its subclasses instead.      */
/*============================================================================*/

::class GtkRange public subclass GtkWidget

::METHOD signal_connect EXTERNAL "LIBRARY rexxgtk GrxRangeSignalConnect"

-- The following are the methods that can be connected to signals. By default
-- they do nothing.

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

::METHOD set_digits EXTERNAL "LIBRARY rexxgtk GrxScaleSetDigits"
::METHOD 'digits=' EXTERNAL "LIBRARY rexxgtk GrxScaleSetDigits"
::METHOD get_digits EXTERNAL "LIBRARY rexxgtk GrxScaleGetDigits"
::METHOD digits EXTERNAL "LIBRARY rexxgtk GrxScaleGetDigits"
::METHOD set_value_pos EXTERNAL "LIBRARY rexxgtk GrxScaleSetValuePos"
::METHOD 'value_pos=' EXTERNAL "LIBRARY rexxgtk GrxScaleSetValuePos"
::METHOD get_value_pos EXTERNAL "LIBRARY rexxgtk GrxScaleGetValuePos"
::METHOD value_pos EXTERNAL "LIBRARY rexxgtk GrxScaleGetValuePos"

::method signal_connect
use strict arg event
return self~signal_connect:super(event)


/*============================================================================*/
/* Class: GtkHScale                                                           */
/*============================================================================*/

::class GtkHScale public subclass GtkScale

::METHOD init EXTERNAL "LIBRARY rexxgtk GrxHScaleNew"


/*============================================================================*/
/* Class: GtkHScale_With_Range                                                */
/*============================================================================*/

::class GtkHScale_With_Range public subclass GtkHScale

::METHOD init EXTERNAL "LIBRARY rexxgtk GrxHScaleNewWithRange"


/*============================================================================*/
/* Class: GtkVScale                                                           */
/*============================================================================*/

::class GtkVScale public subclass GtkScale

::METHOD init EXTERNAL "LIBRARY rexxgtk GrxVScaleNew"

::method signal_connect
use strict arg event
return self~signal_connect:super(event)


/*============================================================================*/
/* Class: GtkVScale_With_Range                                                */
/*============================================================================*/

::class GtkVScale_With_Range public subclass GtkVScale

::METHOD init EXTERNAL "LIBRARY rexxgtk GrxVScaleNewWithRange"


/*============================================================================*/
/* Class: GtkImage                                                            */
/*============================================================================*/

::class GtkImage public subclass GtkWidget

::METHOD init EXTERNAL "LIBRARY rexxgtk GrxImageNew"
::METHOD set_from_file EXTERNAL "LIBRARY rexxgtk GrxImageSetFromFile"
::METHOD set_from_stock EXTERNAL "LIBRARY rexxgtk GrxImageSetFromStock"
::METHOD set_from_icon_name EXTERNAL "LIBRARY rexxgtk GrxImageSetFromIconName"


/*============================================================================*/
/* Class: GtkImage_From_File                                                  */
/*============================================================================*/

::class GtkImage_From_File public subclass GtkImage

::method init
self~init:super()
use strict arg filename
self~set_from_file(filename)
return


/*============================================================================*/
/* Class: GtkImage_From_Stock                                                 */
/*============================================================================*/

::class GtkImage_From_Stock public subclass GtkImage

::method init
self~init:super()
use strict arg stockid, size
self~set_from_stock(stockid, size)
return


/*============================================================================*/
/* Class: GtkImage_From_Icon_Name                                             */
/*============================================================================*/

::class GtkImage_From_Icon_Name public subclass GtkImage

::method init
self~init:super()
use strict arg iconname, size
self~set_from_icon_name(iconname, size)
return


/*============================================================================*/
/* Class: GtkProgressBar                                                      */
/*============================================================================*/

::class GtkProgressBar public subclass GtkWidget

::METHOD init EXTERNAL "LIBRARY rexxgtk GrxProgressBarNew"
::METHOD set_text EXTERNAL "LIBRARY rexxgtk GrxProgressBarSetText"
::METHOD 'text=' EXTERNAL "LIBRARY rexxgtk GrxProgressBarSetText"
::METHOD set_fraction EXTERNAL "LIBRARY rexxgtk GrxProgressBarSetFraction"
::METHOD 'fraction=' EXTERNAL "LIBRARY rexxgtk GrxProgressBarSetFraction"


/*============================================================================*/
/* Class: GtkTextView                                                         */
/*============================================================================*/

::class GtkTextView public subclass GtkContainer

::METHOD init EXTERNAL "LIBRARY rexxgtk GrxTextViewNew"
::METHOD get_buffer EXTERNAL "LIBRARY rexxgtk GrxTextViewGetBuffer"
::METHOD buffer EXTERNAL "LIBRARY rexxgtk GrxTextViewGetBuffer"
::METHOD get_wrap_mode EXTERNAL "LIBRARY rexxgtk GrxTextViewGetWrapMode"
::METHOD wrap_mode EXTERNAL "LIBRARY rexxgtk GrxTextViewGetWrapMode"
::METHOD 'wrap_mode=' EXTERNAL "LIBRARY rexxgtk GrxTextViewSetWrapMode"
::METHOD set_wrap_mode EXTERNAL "LIBRARY rexxgtk GrxTextViewSetWrapMode"
::METHOD get_justification EXTERNAL "LIBRARY rexxgtk GrxTextViewGetJustification"
::METHOD justification EXTERNAL "LIBRARY rexxgtk GrxTextViewGetJustification"
::METHOD set_justification EXTERNAL "LIBRARY rexxgtk GrxTextViewSetJustification"
::METHOD 'justification=' EXTERNAL "LIBRARY rexxgtk GrxTextViewSetJustification"
::METHOD get_editable EXTERNAL "LIBRARY rexxgtk GrxTextViewGetEditable"
::METHOD editable EXTERNAL "LIBRARY rexxgtk GrxTextViewGetEditable"
::METHOD set_editable EXTERNAL "LIBRARY rexxgtk GrxTextViewSetEditable"
::METHOD 'editable=' EXTERNAL "LIBRARY rexxgtk GrxTextViewSetEditable"
::METHOD get_cursor_visible EXTERNAL "LIBRARY rexxgtk GrxTextViewGetCursorVisible"
::METHOD cursor_visible EXTERNAL "LIBRARY rexxgtk GrxTextViewGetCursorVisible"
::METHOD set_cursor_visible EXTERNAL "LIBRARY rexxgtk GrxTextViewSetCursorVisible"
::METHOD 'cursor_visible=' EXTERNAL "LIBRARY rexxgtk GrxTextViewSetCursorVisible"
::METHOD get_pixels_above_lines EXTERNAL "LIBRARY rexxgtk GrxTextViewGetPixelsAboveLines"
::METHOD pixels_above_lines EXTERNAL "LIBRARY rexxgtk GrxTextViewGetPixelsAboveLines"
::METHOD set_pixels_above_lines EXTERNAL "LIBRARY rexxgtk GrxTextViewSetPixelsAboveLines"
::METHOD 'pixels_above_lines=' EXTERNAL "LIBRARY rexxgtk GrxTextViewSetPixelsAboveLines"
::METHOD get_pixels_below_lines EXTERNAL "LIBRARY rexxgtk GrxTextViewGetPixelsBelowLines"
::METHOD pixels_below_lines EXTERNAL "LIBRARY rexxgtk GrxTextViewGetPixelsBelowLines"
::METHOD set_pixels_below_lines EXTERNAL "LIBRARY rexxgtk GrxTextViewSetPixelsBelowLines"
::METHOD 'pixels_below_lines=' EXTERNAL "LIBRARY rexxgtk GrxTextViewSetPixelsBelowLines"
::METHOD get_pixels_inside_wrap EXTERNAL "LIBRARY rexxgtk GrxTextViewGetPixelsInsideWrap"
::METHOD pixels_inside_wrap EXTERNAL "LIBRARY rexxgtk GrxTextViewGetPixelsInsideWrap"
::METHOD set_pixels_inside_wrap EXTERNAL "LIBRARY rexxgtk GrxTextViewSetPixelsInsideWrap"
::METHOD 'pixels_inside_wrap=' EXTERNAL "LIBRARY rexxgtk GrxTextViewSetPixelsInsideWrap"
::METHOD get_left_margin EXTERNAL "LIBRARY rexxgtk GrxTextViewGetLeftMargin"
::METHOD left_margin EXTERNAL "LIBRARY rexxgtk GrxTextViewGeLeftMargin"
::METHOD set_left_margin EXTERNAL "LIBRARY rexxgtk GrxTextViewSetLeftMargin"
::METHOD 'left_margin=' EXTERNAL "LIBRARY rexxgtk GrxTextViewSetLeftMargin"
::METHOD get_right_margin EXTERNAL "LIBRARY rexxgtk GrxTextViewGetRightMargin"
::METHOD right_margin EXTERNAL "LIBRARY rexxgtk GrxTextViewGeRightMargin"
::METHOD set_right_margin EXTERNAL "LIBRARY rexxgtk GrxTextViewSetRightMargin"
::METHOD 'right_margin=' EXTERNAL "LIBRARY rexxgtk GrxTextViewSetRightMargin"
::METHOD add_child_at_offset EXTERNAL "LIBRARY rexxgtk GrxTextViewAddChildAtOffset"
::METHOD signal_connect EXTERNAL "LIBRARY rexxgtk GrxTextViewSignalConnect"

-- The following are the methods that can be connected to signals. By default
-- they do nothing.

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

::method signal_move_viewport
return

::method signal_page_horizontally
return

::method signal_paste_clipboard
return

::method signal_populate_popup
return

::method signal_select_all
return

::method signal_set_anchor
return

::method signal_set_scroll_adjustments
return

::method signal_toggle_cursor_visible
return

::method signal_toggle_overwrite
return


