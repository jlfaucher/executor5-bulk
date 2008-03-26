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


-- Create the Rexx queue we will use for callbacks
.local['!GTK_Queue'] = RxQueue('Create', 'GTKQUEUE23')
call GrxSetRexxQueueName .local['!GTK_Queue']

-- Create the widget database
.local['!GTK_Database'] = .directory~new()

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
old_queue = RxQueue('Set', .local['!GTK_Queue'])
do while queued() > 0
   pull pointer msg args
   widget = .local['!GTK_Database']~at(pointer)
   if widget <> .nil then do
      widget~run_method(msg, args)
      end
   end
call RxQueue 'Set', old_queue
return


/*============================================================================*/
/* Routine: GTK_Constant                                                      */
/*============================================================================*/

::routine GTK_Constant public
use strict arg id
tid = upper(id)
select
   -- GtkWindowType
   when tid = 'GTK_WINDOW_TOPLEVEL' then return 0
   when tid = 'GTK_WINDOW_POPUP' then return 1
   -- GtkButtonType
   when tid = 'GTK_BUTTONS_NONE' then return 0
   when tid = 'GTK_BUTTONS_OK' then return 1
   when tid = 'GTK_BUTTONS_CLOSE' then return 2
   when tid = 'GTK_BUTTONS_CANCEL' then return 3
   when tid = 'GTK_BUTTONS_YES_NO' then return 4
   when tid = 'GTK_BUTTONS_OK_CANCEL' then return 5
   -- GtkDialogFlags
   when tid = 'GTK_DIALOG_MODAL' then return 2**0
   when tid = 'GTK_DIALOG_DESTROY_WITH_PARENT' then return 2**1
   when tid = 'GTK_DIALOG_NO_SEPARATOR' then return 2**2
   -- GtkResponseType
   when tid = 'GTK_RESPONSE_NONE' then return -1
   when tid = 'GTK_RESPONSE_REJECT' then return -2
   when tid = 'GTK_RESPONSE_ACCEPT' then return -3
   when tid = 'GTK_RESPONSE_DELETE_EVENT' then return -4
   when tid = 'GTK_RESPONSE_OK' then return -5
   when tid = 'GTK_RESPONSE_CANCEL' then return -6
   when tid = 'GTK_RESPONSE_CLOSE' then return -7
   when tid = 'GTK_RESPONSE_YES' then return -8
   when tid = 'GTK_RESPONSE_NO' then return -9
   when tid = 'GTK_RESPONSE_APPLY' then return -10
   when tid = 'GTK_RESPONSE_HELP' then return -11
   -- GtkMessageType
   when tid = 'GTK_MESSAGE_INFO' then return 0
   when tid = 'GTK_MESSAGE_WARNING' then return 1
   when tid = 'GTK_MESSAGE_QUESTION' then return 2
   when tid = 'GTK_MESSAGE_ERROR' then return 3
   when tid = 'GTK_MESSAGE_OTHER' then return 4
   -- GTK stock string defines
   when tid = 'GTK_STOCK_ABOUT' then return 'gtk-about'
   when tid = 'GTK_STOCK_ADD' then return 'gtk-add'
   when tid = 'GTK_STOCK_APPLY' then return 'gtk-apply'
   when tid = 'GTK_STOCK_BOLD' then return 'gtk-bold'
   when tid = 'GTK_STOCK_CANCEL' then return 'gtk-cancel'
   when tid = 'GTK_STOCK_CDROM' then return 'gtk-cdrom'
   when tid = 'GTK_STOCK_CLEAR' then return 'gtk-clear'
   when tid = 'GTK_STOCK_CLOSE' then return 'gtk-close'
   when tid = 'GTK_STOCK_COLOR_PICKER' then return 'gtk-color-picker'
   when tid = 'GTK_STOCK_CONVERT' then return 'gtk-comvert'
   when tid = 'GTK_STOCK_CONNECT' then return 'gtk-connect'
   when tid = 'GTK_STOCK_COPY' then return 'gtk-copy'
   when tid = 'GTK_STOCK_CUT' then return 'gtk-cut'
   when tid = 'GTK_STOCK_DELETE' then return 'gtk-delete'
   when tid = 'GTK_STOCK_DIALOG_ERROR' then return 'gtk-dialog-error'
   when tid = 'GTK_STOCK_DIALOG_INFO' then return 'gtk-dialog-info'
   when tid = 'GTK_STOCK_DIALOG_QUESTION' then return 'gtk-dialog-question'
   when tid = 'GTK_STOCK_DIALOG_WARNING' then return 'gtk-dialog-warning'
   when tid = 'GTK_STOCK_DIRECTORY' then return 'gtk-directry'
   when tid = 'GTK_STOCK_DISCARD' then return 'gtk-discard'
   when tid = 'GTK_STOCK_DISCONNECT' then return 'gtk-disconnect'
   when tid = 'GTK_STOCK_DND' then return 'gtk-dnd'
   when tid = 'GTK_STOCK_DND_MULTIPLE' then return 'gtk-dnd-multiple'
   when tid = 'GTK_STOCK_EDIT' then return 'gtk-edit'
   when tid = 'GTK_STOCK_EXECUTE' then return 'gtk-execute'
   when tid = 'GTK_STOCK_FILE' then return 'gtk-file'
   when tid = 'GTK_STOCK_FIND' then return 'gtk-find'
   when tid = 'GTK_STOCK_FIND_AND_REPLACE' then return 'gtk-find-and-replace'
   when tid = 'GTK_STOCK_FLOPPY' then return 'gtk-floppy'
   when tid = 'GTK_STOCK_FULLSCREEN' then return 'gtk-fullscreen'
   when tid = 'GTK_STOCK_GOTO_BOTTOM' then return 'gtk-goto-bottom'
   when tid = 'GTK_STOCK_GOTO_FIRST' then return 'gtk-goto-first'
   when tid = 'GTK_STOCK_GOTO_LAST' then return 'gtk-goto-last'
   when tid = 'GTK_STOCK_GOTO_TOP' then return 'gtk-goto-top'
   when tid = 'GTK_STOCK_GO_BACK' then return 'gtk-go-back'
   when tid = 'GTK_STOCK_GO_DOWN' then return 'gtk-go-down'
   when tid = 'GTK_STOCK_GO_FORWARD' then return 'gtk-go-forward'
   when tid = 'GTK_STOCK_GO_UP' then return 'gtk-go-up'
   when tid = 'GTK_STOCK_HARDDISK' then return 'gtk-harddisk'
   when tid = 'GTK_STOCK_HELP' then return 'gtk-help'
   when tid = 'GTK_STOCK_HOME' then return 'gtk-home'
   when tid = 'GTK_STOCK_INDENT' then return 'gtk-indent'
   when tid = 'GTK_STOCK_INDEX' then return 'gtk-index'
   when tid = 'GTK_STOCK_INFO' then return 'gtk-info'
   when tid = 'GTK_STOCK_ITALIC' then return 'gtk-italic'
   when tid = 'GTK_STOCK_JUMP_TO' then return 'gtk-jump-to'
   when tid = 'GTK_STOCK_JUSTIFY_CENTER' then return 'gtk-justify-center'
   when tid = 'GTK_STOCK_JUSTIFY_FILL' then return 'gtk-justify-fill'
   when tid = 'GTK_STOCK_JUSTIFY_LEFT' then return 'gtk-justify-left'
   when tid = 'GTK_STOCK_JUSTIFY_RIGHT' then return 'gtk-justify-right'
   when tid = 'GTK_STOCK_LEAVE_FULLSCREEN' then return 'gtk-leave-fullscreen'
   when tid = 'GTK_STOCK_MEDIA_FORWARD' then return 'gtk-media-forward'
   when tid = 'GTK_STOCK_MEDIA_NEXT' then return 'gtk-media-nect'
   when tid = 'GTK_STOCK_MEDIA_PAUSE' then return 'gtk-media-pause'
   when tid = 'GTK_STOCK_MEDIA_PLAY' then return 'gtk-media-play'
   when tid = 'GTK_STOCK_MEDIA_PREVIOUS' then return 'gtk-media-previous'
   when tid = 'GTK_STOCK_MEDIA_RECORD' then return 'gtk-media-record'
   when tid = 'GTK_STOCK_MEDIA_REWIND' then return 'gtk-media-rewind'
   when tid = 'GTK_STOCK_MEDIA_STOP' then return 'gtk-media-stop'
   when tid = 'GTK_STOCK_MISSING_IMAGE' then return 'gtk-missing-image'
   when tid = 'GTK_STOCK_NETWORK' then return 'gtk-network'
   when tid = 'GTK_STOCK_NEW' then return 'gtk-new'
   when tid = 'GTK_STOCK_NO' then return 'gtk-no'
   when tid = 'GTK_STOCK_OK' then return 'gtk-ok'
   when tid = 'GTK_STOCK_OPEN' then return 'gtk-open'
   when tid = 'GTK_STOCK_OREINTATION_LANDSCAPE' then return 'gtk-orientation-landscape'
   when tid = 'GTK_STOCK_OREINTATION_PORTRAIT' then return 'gtk-orientation-portrait'
   when tid = 'GTK_STOCK_OREINTATION_REVERSE_LANDSCAPE' then return 'gtk-orientation-reverse-landscape'
   when tid = 'GTK_STOCK_OREINTATION_REVERSE_LANDSCAPE' then return 'gtk-orientation-reverse-portrait'
   when tid = 'GTK_STOCK_PASTE' then return 'gtk-paste'
   when tid = 'GTK_STOCK_PREFERENCES' then return 'gtk-preferences'
   when tid = 'GTK_STOCK_PRINT' then return 'gtk-print'
   when tid = 'GTK_STOCK_PRINT_PREVIEW' then return 'gtk-print-preview'
   when tid = 'GTK_STOCK_PROPERTIES' then return 'gtk-properties'
   when tid = 'GTK_STOCK_QUIT' then return 'gtk-quit'
   when tid = 'GTK_STOCK_REDO' then return 'gtk-redo'
   when tid = 'GTK_STOCK_REFRESH' then return 'gtk-refresh'
   when tid = 'GTK_STOCK_REMOVE' then return 'gtk-remove'
   when tid = 'GTK_STOCK_REVERT_TO_SAVED' then return 'gtk-revert-to-saved'
   when tid = 'GTK_STOCK_SAVE' then return 'gtk-save'
   when tid = 'GTK_STOCK_SAVE_AS' then return 'gtk-save-as'
   when tid = 'GTK_STOCK_SELECT_ALL' then return 'gtk-select-all'
   when tid = 'GTK_STOCK_SELECT_COLOR' then return 'gtk-select-color'
   when tid = 'GTK_STOCK_SELECT_FONT' then return 'gtk-select-font'
   when tid = 'GTK_STOCK_SORT_ASCENDING' then return 'gtk-sort-ascending'
   when tid = 'GTK_STOCK_SORT_DESCENDING' then return 'gtk-sort-descending'
   when tid = 'GTK_STOCK_SPELL_CHECK' then return 'gtk-spell-check'
   when tid = 'GTK_STOCK_STOP' then return 'gtk-stop'
   when tid = 'GTK_STOCK_STRIKETHROUGH' then return 'gtk-strikethrough'
   when tid = 'GTK_STOCK_UNDELETE' then return 'gtk-undelete'
   when tid = 'GTK_STOCK_UNDERLINE' then return 'gtk-underline'
   when tid = 'GTK_STOCK_UNDO' then return 'gtk-undo'
   when tid = 'GTK_STOCK_UNINDENT' then return 'gtk-unindent'
   when tid = 'GTK_STOCK_YES' then return 'gtk-yes'
   when tid = 'GTK_STOCK_ZOOM_100' then return 'gtk-zoom-100'
   when tid = 'GTK_STOCK_ZOOM_FIT' then return 'gtk-zoom-fit'
   when tid = 'GTK_STOCK_ZOOM_IN' then return 'gtk-zoom-in'
   when tid = 'GTK_STOCK_ZOOM_OUT' then return 'gtk-zoom-out'
   -- GtkFileCoooserAction
   when tid = 'GTK_FILE_CHOOSER_ACTION_OPEN' then return 0
   when tid = 'GTK_FILE_CHOOSER_ACTION_SAVE' then return 1
   when tid = 'GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER' then return 2
   when tid = 'GTK_FILE_CHOOSER_ACTION_CREATE_FOLDER' then return 3
   -- GtkAssistancePageType
   when tid = 'GTK_ASSISTANT_PAGE_CONTENT' then return 0
   when tid = 'GTK_ASSISTANT_PAGE_INTRO' then return 1
   when tid = 'GTK_ASSISTANT_PAGE_CONFIRM' then return 2
   when tid = 'GTK_ASSISTANT_PAGE_SUMMARY' then return 3
   when tid = 'GTK_ASSISTANT_PAGE_PROGRESS' then return 4
   -- GtkReliefType
   when tid = 'GTK_RELIEF_NORMAL' then return 0
   when tid = 'GTK_RELIEF_HALF' then return 1
   when tid = 'GTK_RELIEF_NONO' then return 2
   -- GtkAtom
   when tid = 'GTK_SELECTION_PRIMARY' then return 1
   when tid = 'GTK_SELECTION_SECONDARY' then return 2
   when tid = 'GTK_SELECTION_CLIPBOARD' then return 69
   -- GtkShadow
   when tid = 'GTK_SHADOW_NONE' then return 0
   when tid = 'GTK_SHADOW_IN' then return 1
   when tid = 'GTK_SHADOW_OUT' then return 2
   when tid = 'GTK_SHADOW_ETCHED_IN' then return 3
   when tid = 'GTK_SHADOW_ETCHED_OUT' then return 4
   -- GtkPositionType
   when tid = 'GTK_POS_LEFT' then return 0
   when tid = 'GTK_POS_RIGHT' then return 1
   when tid = 'GTK_POS_TOP' then return 2
   when tid = 'GTK_POS_BOTTOM' then return 3
   -- GtkIconSize
   when tid = 'GTK_ICON_SIZE_INVALID' then return 0
   when tid = 'GTK_ICON_SIZE_MENU' then return 1
   when tid = 'GTK_ICON_SIZE_SMALL_TOOLBAR' then return 2
   when tid = 'GTK_ICON_SIZE_LARGE_TOOLBAR' then return 3
   when tid = 'GTK_ICON_SIZE_BUTTON' then return 4
   when tid = 'GTK_ICON_SIZE_DND' then return 5
   when tid = 'GTK_ICON_SIZE_DIALOG' then return 6
   -- GtkJustification
   when tid = 'GTK_JUSTIFY_LEFT' then return 0
   when tid = 'GTK_JUSTIFY_RIGHT' then return 1
   when tid = 'GTK_JUSTIFY_CENTER' then return 2
   when tid = 'GTK_JUSTIFY_FILL' then return 3
   -- GtkPolicyType
   when tid = 'GTK_POLICY_ALWAYS' then return 0
   when tid = 'GTK_POLICY_AUTOMATIC' then return 1
   when tid = 'GTK_POLICY_NEVER' then return 2
   -- GtkAttachOptions
   when tid = 'GTK_EXPAND' then return 2**0
   when tid = 'GTK_SHRINK' then return 2**1
   when tid = 'GTK_FILL' then return 2**2
   -- GtkWrapMode
   when tid = 'GTK_WRAP_NONE' then return 0
   when tid = 'GTK_WRAP_CHAR' then return 1
   when tid = 'GTK_WRAP_WORD' then return 2
   when tid = 'GTK_WRAP_WORD_CHAR' then return 3
   -- GtkStateType
   when tid = 'GTK_STATE_NORMAL' then return 0
   when tid = 'GTK_STATE_ACTIVE' then return 1
   when tid = 'GTK_STATE_PRELIGHT' then return 2
   when tid = 'GTK_STATE_SELECTED' then return 3
   when tid = 'GTK_STATE_INSENSITIVE' then return 4
   -- Pango scale factors
   when tid = 'PANGO_SCALE_XX_SMALL' then return 0.5787037037037
   when tid = 'PANGO_SCALE_X_SMALL' then return 0.6444444444444
   when tid = 'PANGO_SCALE_SMALL' then return 0.8333333333333
   when tid = 'PANGO_SCALE_MEDIUM' then return 1.0
   when tid = 'PANGO_SCALE_LARGE' then return 1.2
   when tid = 'PANGO_SCALE_X_LARGE' then return 1.4399999999999
   when tid = 'PANGO_SCALE_XX_LARGE' then return 1.728
   -- PangoWeight
   when tid = 'PANGO_WEIGHT_ULTRALIGHT' then return 200
   when tid = 'PANGO_WEIGHT_LIGHT' then return 300
   when tid = 'PANGO_WEIGHT_NORMAL' then return 400
   when tid = 'PANGO_WEIGHT_SEMIBOLD' then return 600
   when tid = 'PANGO_WEIGHT_BOLD' then return 700
   when tid = 'PANGO_WEIGHT_ULTRABOLD' then return 800
   when tid = 'PANGO_WEIGHT_HEAVY' then return 900
   -- PangoStyle
   when tid = 'PANGO_STYLE_NORMAL' then return 0
   when tid = 'PANGO_STYLE_OBLIQUE' then return 1
   when tid = 'PANGO_STYLE_ITALIC' then return 2
   -- PangoUnderline
   when tid = 'PANGO_UNDERLINE_NONE' then return 0
   when tid = 'PANGO_UNDERLINE_SINGLE' then return 1
   when tid = 'PANGO_UNDERLINE_DOUBLE' then return 2
   when tid = 'PANGO_UNDERLINE_LOW' then return 3
   when tid = 'PANGO_UNDERLINE_ERROR' then return 4
   -- Fundamental GTypes
   when tid = 'G_TYPE_INVALID' then return 0
   when tid = 'G_TYPE_NONE' then return 1
   when tid = 'G_TYPE_INTERFACE' then return 2
   when tid = 'G_TYPE_CHAR' then return 3
   when tid = 'G_TYPE_UCHAR' then return 4
   when tid = 'G_TYPE_BOOLEAN' then return 5
   when tid = 'G_TYPE_INT' then return 6
   when tid = 'G_TYPE_UINT' then return 7
   when tid = 'G_TYPE_LONG' then return 8
   when tid = 'G_TYPE_ULONG' then return 9
   when tid = 'G_TYPE_INT64' then return 10
   when tid = 'G_TYPE_UINT64' then return 11
   when tid = 'G_TYPE_ENUM' then return 12
   when tid = 'G_TYPE_FLAGS' then return 13
   when tid = 'G_TYPE_FLOAT' then return 14
   when tid = 'G_TYPE_DOUBLE' then return 15
   when tid = 'G_TYPE_STRING' then return 16
   when tid = 'G_TYPE_POINTER' then return 17
   when tid = 'G_TYPE_BOXED' then return 18
   when tid = 'G_TYPE_PARAM' then return 19
   when tid = 'G_TYPE_OBJECT' then return 20
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
::METHOD signal_connect EXTERNAL "LIBRARY rexxgtk GrxWidgetSignalConnect"

--
-- The following are the methods that can be connected to signals. By default
-- they do nothing.
--

::method signal_destroy
return


