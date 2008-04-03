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


-- Create and initialize the GTK quit flag
.local['!GTK_Quit'] = .false


/*============================================================================*/
/* Routine: gtk_main                                                          */
/*============================================================================*/

::routine gtk_main public
do while .local['!GTK_Quit'] = .false
   call gtk_main_iteration
   end
.local['!GTK_Quit'] = .false  -- we need this for recursive calls to this function
return


/*============================================================================*/
/* Routine: gtk_main_quit                                                     */
/*============================================================================*/

::routine gtk_main_quit public
.local['!GTK_Quit'] = .true
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
   -- Just return the id
   otherwise nop
   end
return id


/*============================================================================*/
/* Class: GtkWidget                                                           */
/*============================================================================*/

::class GtkWidget public

::METHOD init EXTERNAL "LIBRARY rexxgtk GrxWidgetNew"

::method uninit
-- don't destroy widgets by default!
-- but do remove it from the DB
call GrxRemoveWidget self
return

::method pointer
-- return the pointer object to the widget
expose !pointer
use strict arg
return !pointer

::method 'pointer=' private
expose !pointer
use strict arg !pointer

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
::METHOD modify_cursor EXTERNAL "LIBRARY rexxgtk GrxWidgetModifyCursor"
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
::METHOD error_bell EXTERNAL "LIBRARY rexxgtk GrxWidgetErrorBell"
::METHOD get_tooltip_markup EXTERNAL "LIBRARY rexxgtk GrxWidgetGetTooltipMarkup"
::METHOD tooltip_markup EXTERNAL "LIBRARY rexxgtk GrxWidgetGetTooltipMarkup"
::METHOD set_tooltip_markup EXTERNAL "LIBRARY rexxgtk GrxWidgetSetTooltipMarkup"
::METHOD 'tooltip_markup=' EXTERNAL "LIBRARY rexxgtk GrxWidgetSetTooltipMarkup"
::METHOD get_tooltip_text EXTERNAL "LIBRARY rexxgtk GrxWidgetGetTooltipText"
::METHOD tooltip_text EXTERNAL "LIBRARY rexxgtk GrxWidgetGetTooltipText"
::METHOD set_tooltip_text EXTERNAL "LIBRARY rexxgtk GrxWidgetSetTooltipText"
::METHOD 'tooltip_text=' EXTERNAL "LIBRARY rexxgtk GrxWidgetSetTooltipText"
::METHOD signal_connect EXTERNAL "LIBRARY rexxgtk GrxWidgetSignalConnect"

--
-- The following are the methods that can be connected to signals. By default
-- they do nothing.
--

::method signal_destroy
return


