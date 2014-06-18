/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2007-2010 Rexx Language Association. All rights reserved.    */
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

::routine gtk_init public
call GrxInit
return


/*============================================================================*/
/* Routine: gtk_main                                                          */
/*============================================================================*/

::routine gtk_main public
call GrxMain
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
/* Note: This class should never be instatiated. It is here just as a         */
/*       container for a host of GTK constants.                               */
/*============================================================================*/

::class gtk public

   -- GtkWindowType
::constant GTK_WINDOW_TOPLEVEL 0
::constant GTK_WINDOW_POPUP 1
   -- GtkButtonType
::constant GTK_BUTTONS_NONE 0
::constant GTK_BUTTONS_OK 1
::constant GTK_BUTTONS_CLOSE 2
::constant GTK_BUTTONS_CANCEL 3
::constant GTK_BUTTONS_YES_NO 4
::constant GTK_BUTTONS_OK_CANCEL 5
   -- GtkDialogFlags
::constant GTK_DIALOG_MODAL 0
::constant GTK_DIALOG_DESTROY_WITH_PARENT 1
::constant GTK_DIALOG_NO_SEPARATOR 2
   -- GtkResponseType
::constant GTK_RESPONSE_NONE '-1'
::constant GTK_RESPONSE_REJECT '-2'
::constant GTK_RESPONSE_ACCEPT '-3'
::constant GTK_RESPONSE_DELETE_EVENT '-4'
::constant GTK_RESPONSE_OK '-5'
::constant GTK_RESPONSE_CANCEL '-6'
::constant GTK_RESPONSE_CLOSE '-7'
::constant GTK_RESPONSE_YES '-8'
::constant GTK_RESPONSE_NO '-9'
::constant GTK_RESPONSE_APPLY '-10'
::constant GTK_RESPONSE_HELP '-11'
   -- GtkMessageType
::constant GTK_MESSAGE_INFO 0
::constant GTK_MESSAGE_WARNING 1
::constant GTK_MESSAGE_QUESTION 2
::constant GTK_MESSAGE_ERROR 3
::constant GTK_MESSAGE_OTHER 4
   -- GTK stock string defines
::constant GTK_STOCK_ABOUT 'gtk-about'
::constant GTK_STOCK_ADD 'gtk-add'
::constant GTK_STOCK_APPLY 'gtk-apply'
::constant GTK_STOCK_BOLD 'gtk-bold'
::constant GTK_STOCK_CANCEL 'gtk-cancel'
::constant GTK_STOCK_CDROM 'gtk-cdrom'
::constant GTK_STOCK_CLEAR 'gtk-clear'
::constant GTK_STOCK_CLOSE 'gtk-close'
::constant GTK_STOCK_COLOR_PICKER 'gtk-color-picker'
::constant GTK_STOCK_CONVERT 'gtk-comvert'
::constant GTK_STOCK_CONNECT 'gtk-connect'
::constant GTK_STOCK_COPY 'gtk-copy'
::constant GTK_STOCK_CUT 'gtk-cut'
::constant GTK_STOCK_DELETE 'gtk-delete'
::constant GTK_STOCK_DIALOG_ERROR 'gtk-dialog-error'
::constant GTK_STOCK_DIALOG_INFO 'gtk-dialog-info'
::constant GTK_STOCK_DIALOG_QUESTION 'gtk-dialog-question'
::constant GTK_STOCK_DIALOG_WARNING 'gtk-dialog-warning'
::constant GTK_STOCK_DIRECTORY 'gtk-directry'
::constant GTK_STOCK_DISCARD 'gtk-discard'
::constant GTK_STOCK_DISCONNECT 'gtk-disconnect'
::constant GTK_STOCK_DND 'gtk-dnd'
::constant GTK_STOCK_DND_MULTIPLE 'gtk-dnd-multiple'
::constant GTK_STOCK_EDIT 'gtk-edit'
::constant GTK_STOCK_EXECUTE 'gtk-execute'
::constant GTK_STOCK_FILE 'gtk-file'
::constant GTK_STOCK_FIND 'gtk-find'
::constant GTK_STOCK_FIND_AND_REPLACE 'gtk-find-and-replace'
::constant GTK_STOCK_FLOPPY 'gtk-floppy'
::constant GTK_STOCK_FULLSCREEN 'gtk-fullscreen'
::constant GTK_STOCK_GOTO_BOTTOM 'gtk-goto-bottom'
::constant GTK_STOCK_GOTO_FIRST 'gtk-goto-first'
::constant GTK_STOCK_GOTO_LAST 'gtk-goto-last'
::constant GTK_STOCK_GOTO_TOP 'gtk-goto-top'
::constant GTK_STOCK_GO_BACK 'gtk-go-back'
::constant GTK_STOCK_GO_DOWN 'gtk-go-down'
::constant GTK_STOCK_GO_FORWARD 'gtk-go-forward'
::constant GTK_STOCK_GO_UP 'gtk-go-up'
::constant GTK_STOCK_HARDDISK 'gtk-harddisk'
::constant GTK_STOCK_HELP 'gtk-help'
::constant GTK_STOCK_HOME 'gtk-home'
::constant GTK_STOCK_INDENT 'gtk-indent'
::constant GTK_STOCK_INDEX 'gtk-index'
::constant GTK_STOCK_INFO 'gtk-info'
::constant GTK_STOCK_ITALIC 'gtk-italic'
::constant GTK_STOCK_JUMP_TO 'gtk-jump-to'
::constant GTK_STOCK_JUSTIFY_CENTER 'gtk-justify-center'
::constant GTK_STOCK_JUSTIFY_FILL 'gtk-justify-fill'
::constant GTK_STOCK_JUSTIFY_LEFT 'gtk-justify-left'
::constant GTK_STOCK_JUSTIFY_RIGHT 'gtk-justify-right'
::constant GTK_STOCK_LEAVE_FULLSCREEN 'gtk-leave-fullscreen'
::constant GTK_STOCK_MEDIA_FORWARD 'gtk-media-forward'
::constant GTK_STOCK_MEDIA_NEXT 'gtk-media-nect'
::constant GTK_STOCK_MEDIA_PAUSE 'gtk-media-pause'
::constant GTK_STOCK_MEDIA_PLAY 'gtk-media-play'
::constant GTK_STOCK_MEDIA_PREVIOUS 'gtk-media-previous'
::constant GTK_STOCK_MEDIA_RECORD 'gtk-media-record'
::constant GTK_STOCK_MEDIA_REWIND 'gtk-media-rewind'
::constant GTK_STOCK_MEDIA_STOP 'gtk-media-stop'
::constant GTK_STOCK_MISSING_IMAGE 'gtk-missing-image'
::constant GTK_STOCK_NETWORK 'gtk-network'
::constant GTK_STOCK_NEW 'gtk-new'
::constant GTK_STOCK_NO 'gtk-no'
::constant GTK_STOCK_OK 'gtk-ok'
::constant GTK_STOCK_OPEN 'gtk-open'
::constant GTK_STOCK_ORIENTATION_LANDSCAPE 'gtk-orientation-landscape'
::constant GTK_STOCK_ORIENTATION_PORTRAIT 'gtk-orientation-portrait'
::constant GTK_STOCK_ORIENTATION_REVERSE_LANDSCAPE 'gtk-orientation-reverse-portrait'
::constant GTK_STOCK_PASTE 'gtk-paste'
::constant GTK_STOCK_PREFERENCES 'gtk-preferences'
::constant GTK_STOCK_PRINT 'gtk-print'
::constant GTK_STOCK_PRINT_PREVIEW 'gtk-print-preview'
::constant GTK_STOCK_PROPERTIES 'gtk-properties'
::constant GTK_STOCK_QUIT 'gtk-quit'
::constant GTK_STOCK_REDO 'gtk-redo'
::constant GTK_STOCK_REFRESH 'gtk-refresh'
::constant GTK_STOCK_REMOVE 'gtk-remove'
::constant GTK_STOCK_REVERT_TO_SAVED 'gtk-revert-to-saved'
::constant GTK_STOCK_SAVE 'gtk-save'
::constant GTK_STOCK_SAVE_AS 'gtk-save-as'
::constant GTK_STOCK_SELECT_ALL 'gtk-select-all'
::constant GTK_STOCK_SELECT_COLOR 'gtk-select-color'
::constant GTK_STOCK_SELECT_FONT 'gtk-select-font'
::constant GTK_STOCK_SORT_ASCENDING 'gtk-sort-ascending'
::constant GTK_STOCK_SORT_DESCENDING 'gtk-sort-descending'
::constant GTK_STOCK_SPELL_CHECK 'gtk-spell-check'
::constant GTK_STOCK_STOP 'gtk-stop'
::constant GTK_STOCK_STRIKETHROUGH 'gtk-strikethrough'
::constant GTK_STOCK_UNDELETE 'gtk-undelete'
::constant GTK_STOCK_UNDERLINE 'gtk-underline'
::constant GTK_STOCK_UNDO 'gtk-undo'
::constant GTK_STOCK_UNINDENT 'gtk-unindent'
::constant GTK_STOCK_YES 'gtk-yes'
::constant GTK_STOCK_ZOOM_100 'gtk-zoom-100'
::constant GTK_STOCK_ZOOM_FIT 'gtk-zoom-fit'
::constant GTK_STOCK_ZOOM_IN 'gtk-zoom-in'
::constant GTK_STOCK_ZOOM_OUT 'gtk-zoom-out'
   -- GtkFileCoooserAction
::constant GTK_FILE_CHOOSER_ACTION_OPEN 0
::constant GTK_FILE_CHOOSER_ACTION_SAVE 1
::constant GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER 2
::constant GTK_FILE_CHOOSER_ACTION_CREATE_FOLDER 3
   -- GtkAssistancePageType
::constant GTK_ASSISTANT_PAGE_CONTENT 0
::constant GTK_ASSISTANT_PAGE_INTRO 1
::constant GTK_ASSISTANT_PAGE_CONFIRM 2
::constant GTK_ASSISTANT_PAGE_SUMMARY 3
::constant GTK_ASSISTANT_PAGE_PROGRESS 4
   -- GtkReliefStyle
::constant GTK_RELIEF_NORMAL 0
::constant GTK_RELIEF_HALF 1
::constant GTK_RELIEF_NONE 2
   -- GtkAtom
::constant GDK_SELECTION_PRIMARY 1
::constant GDK_SELECTION_SECONDARY 2
::constant GDK_SELECTION_CLIPBOARD 69
   -- GtkShadow
::constant GTK_SHADOW_NONE 0
::constant GTK_SHADOW_IN 1
::constant GTK_SHADOW_OUT 2
::constant GTK_SHADOW_ETCHED_IN 3
::constant GTK_SHADOW_ETCHED_OUT 4
   -- GtkPositionType
::constant GTK_POS_LEFT 0
::constant GTK_POS_RIGHT 1
::constant GTK_POS_TOP 2
::constant GTK_POS_BOTTOM 3
   -- GtkIconSize
::constant GTK_ICON_SIZE_INVALID 0
::constant GTK_ICON_SIZE_MENU 1
::constant GTK_ICON_SIZE_SMALL_TOOLBAR 2
::constant GTK_ICON_SIZE_LARGE_TOOLBAR 3
::constant GTK_ICON_SIZE_BUTTON 4
::constant GTK_ICON_SIZE_DND 5
::constant GTK_ICON_SIZE_DIALOG 6
   -- GtkJustification
::constant GTK_JUSTIFY_LEFT 0
::constant GTK_JUSTIFY_RIGHT 1
::constant GTK_JUSTIFY_CENTER 2
::constant GTK_JUSTIFY_FILL 3
   -- GtkPolicyType
::constant GTK_POLICY_ALWAYS 0
::constant GTK_POLICY_AUTOMATIC 1
::constant GTK_POLICY_NEVER 2
   -- GtkAttachOptions
::constant GTK_EXPAND 1
::constant GTK_SHRINK 2
::constant GTK_FILL 4
   -- GtkWrapMode
::constant GTK_WRAP_NONE 0
::constant GTK_WRAP_CHAR 1
::constant GTK_WRAP_WORD 2
::constant GTK_WRAP_WORD_CHAR 3
   -- GtkStateType
::constant GTK_STATE_NORMAL 0
::constant GTK_STATE_ACTIVE 1
::constant GTK_STATE_PRELIGHT 2
::constant GTK_STATE_SELECTED 3
::constant GTK_STATE_INSENSITIVE 4
   -- GtkPachDirection
::constant GTK_PACK_DIRECTION_LTR 0
::constant GTK_PACK_DIRECTION_RTL 1
::constant GTK_PACK_DIRECTION_TTB 2
::constant GTK_PACK_DIRECTION_BTT 3
   -- GtkOrientation
::constant GTK_ORIENTATION_HORIZONTAL 0
::constant GTK_ORIENTATION_VERTICAL 1
   -- GtkToolbarStyle
::constant GTK_TOOLBAR_ICONS 0
::constant GTK_TOOLBAR_TEXT 1
::constant GTK_TOOLBAR_BOTH 2
::constant GTK_TOOLBAR_BOTH_HORIZ 3
   -- GtkToolbarChildType
::constant GTK_TOOLBAR_CHILD_SPACE 0
::constant GTK_TOOLBAR_CHILD_BUTTON 1
::constant GTK_TOOLBAR_CHILD_TOGGLEBUTTON 2
::constant GTK_TOOLBAR_CHILD_RADIOBUTTON 3
::constant GTK_TOOLBAR_CHILD_WIDGET 4
   -- GtkToolbarSpaceStyle
::constant GTK_TOOLBAR_SPACE_EMPTY 0
::constant GTK_TOOLBAR_SPACE_LINE 1
   -- GtkCalendarDisplayOptions
::constant GTK_CALENDAR_SHOW_HEADING 1
::constant GTK_CALENDAR_SHOW_DAY_NAMES 2
::constant GTK_CALENDAR_NO_MONTH_CHANGE 4
::constant GTK_CALENDAR_SHOW_WEEK_NUMBERS 8
::constant GTK_CALENDAR_WEEK_START_MONDAY 16
::constant GTK_CALENDAR_SHOW_DETAILS 32
   -- GtkSelectionMode
::constant GTK_SELECTION_NONE 0
::constant GTK_SELECTION_SINGLE 1
::constant GTK_SELECTION_BROWSE 2
::constant GTK_SELECTION_MULTIPLE 3
   -- Pango scale factors
::constant PANGO_SCALE_XX_SMALL 0.5787037037037
::constant PANGO_SCALE_X_SMALL 0.6444444444444
::constant PANGO_SCALE_SMALL 0.8333333333333
::constant PANGO_SCALE_MEDIUM 1.0
::constant PANGO_SCALE_LARGE 1.2
::constant PANGO_SCALE_X_LARGE 1.4399999999999
::constant PANGO_SCALE_XX_LARGE 1.728
   -- PangoWeight
::constant PANGO_WEIGHT_ULTRALIGHT 200
::constant PANGO_WEIGHT_LIGHT 300
::constant PANGO_WEIGHT_NORMAL 400
::constant PANGO_WEIGHT_SEMIBOLD 600
::constant PANGO_WEIGHT_BOLD 700
::constant PANGO_WEIGHT_ULTRABOLD 800
::constant PANGO_WEIGHT_HEAVY 900
   -- PangoStyle
::constant PANGO_STYLE_NORMAL 0
::constant PANGO_STYLE_OBLIQUE 1
::constant PANGO_STYLE_ITALIC 2
   -- PangoUnderline
::constant PANGO_UNDERLINE_NONE 0
::constant PANGO_UNDERLINE_SINGLE 1
::constant PANGO_UNDERLINE_DOUBLE 2
::constant PANGO_UNDERLINE_LOW 3
::constant PANGO_UNDERLINE_ERROR 4
   -- Fundamental GTypes
::constant G_TYPE_INVALID 0
::constant G_TYPE_NONE 4
::constant G_TYPE_INTERFACE 8
::constant G_TYPE_CHAR 12
::constant G_TYPE_UCHAR 16
::constant G_TYPE_BOOLEAN 20
::constant G_TYPE_INT 24
::constant G_TYPE_UINT 28
::constant G_TYPE_LONG 32
::constant G_TYPE_ULONG 36
::constant G_TYPE_INT64 40
::constant G_TYPE_UINT64 44
::constant G_TYPE_ENUM 48
::constant G_TYPE_FLAGS 52
::constant G_TYPE_FLOAT 56
::constant G_TYPE_DOUBLE 60
::constant G_TYPE_STRING 64
::constant G_TYPE_POINTER 68
::constant G_TYPE_BOXED 72
::constant G_TYPE_PARAM 76
::constant G_TYPE_OBJECT 80
   -- GdkEventType
::constant GDK_NOTHING '-1'
::constant GDK_DELETE 0
::constant GDK_DESTROY 1
::constant GDK_EXPOSE 2
::constant GDK_MOTION_NOTIFY 3
::constant GDK_BUTTON_PRESS 4
::constant GDK_2BUTTON_PRESS 5
::constant GDK_3BUTTON_PRESS 6
::constant GDK_BUTTON_RELEASE 7
::constant GDK_KEY_PRESS 8
::constant GDK_KEY_RELEASE 9
::constant GDK_ENTER_NOTIFY 10
::constant GDK_LEAVE_NOTIFY 11
::constant GDK_FOCUS_CHANGE 12
::constant GDK_CONFIGURE 13
::constant GDK_MAP 14
::constant GDK_UNMAP 15
::constant GDK_PROPERTY_NOTIFY 16
::constant GDK_SELECTION_CLEAR 17
::constant GDK_SELECTION_REQUEST 18
::constant GDK_SELECTION_NOTIFY 19
::constant GDK_PROXIMITY_IN 20
::constant GDK_PROXIMITY_OUT 21
::constant GDK_DRAG_ENTER  22
::constant GDK_DRAG_LEAVE 23
::constant GDK_DRAG_MOTION 24
::constant GDK_DRAG_STATUS 25
::constant GDK_DROP_START 26
::constant GDK_DROP_FINISHED 27
::constant GDK_CLIENT_EVENT 28
::constant GDK_VISIBILITY_NOTIFY 29
::constant GDK_NO_EXPOSE 30
::constant GDK_SCROLL 31
::constant GDK_WINDOW_STATE 32
::constant GDK_SETTING 33
::constant GDK_OWNER_CHANGE 34
::constant GDK_GRAB_BROKEN 35
   -- GdkEventMask
::constant GDK_EXPOSURE_MASK 2
::constant GDK_POINTER_MOTION_MASK 4
::constant GDK_POINTER_MOTION_HINT_MASK 8
::constant GDK_BUTTON_MOTION_MASK 16
::constant GDK_BUTTON1_MOTION_MASK 32
::constant GDK_BUTTON2_MOTION_MASK 64
::constant GDK_BUTTON3_MOTION_MASK 128
::constant GDK_BUTTON_PRESS_MASK 256
::constant GDK_BUTTON_RELEASE_MASK 512
::constant GDK_KEY_PRESS_MASK 1024
::constant GDK_KEY_RELEASE_MASK 2048
::constant GDK_ENTER_NOTIFY_MASK 4096
::constant GDK_LEAVE_NOTIFY_MASK 8192
::constant GDK_FOCUS_CHANGE_MASK 16384
::constant GDK_STRUCTURE_MASK 32768
::constant GDK_PROPERTY_CHANGE_MASK 65536
::constant GDK_VISIBILITY_NOTIFY_MASK 131072
::constant GDK_PROXIMITY_IN_MASK 262144
::constant GDK_PROXIMITY_OUT_MASK 524288
::constant GDK_SUBSTRUCTURE_MASK 1048576
::constant GDK_SCROLL_MASK 2097152
::constant GDK_ALL_EVENTS_MASK 4194302
   -- GdkScrollDirection
::constant GDK_SCROLL_UP 0
::constant GDK_SCROLL_DOWN 1
::constant GDK_SCROLL_LEFT 2
::constant GDK_SCROLL_RIGHT 3
   -- GdkVisibilityState
::constant GDK_VISIBILITY_UNOBSCURED 0
::constant GDK_VISIBILITY_PARTIAL 1
::constant GDK_VISIBILITY_FULLY_OBSCURED 2
   -- GdkCrossingMode
::constant GDK_CROSSING_NORMAL 0
::constant GDK_CROSSING_GRAB 1
::constant GDK_CROSSING_UNGRAB 2
   -- GdkNotifyType
::constant GDK_NOTIFY_ANCESTOR 0
::constant GDK_NOTIFY_VIRTUAL 1
::constant GDK_NOTIFY_INFERIOR 2
::constant GDK_NOTIFY_NONLINEAR 3
::constant GDK_NOTIFY_NONLINEAR_VIRTUAL 4
::constant GDK_NOTIFY_UNKNOWN 5
   -- GdkPropertyState
::constant GDK_PROPERTY_NEW_VALUE 0
::constant GDK_PROPERTY_DEKETE 1
   -- GdkWindowState
::constant GDK_WINDOW_STATE_WITHDRAWN 1
::constant GDK_WINDOW_STATE_ICONIFIED 2
::constant GDK_WINDOW_STATE_MAXIMIZED 4
::constant GDK_WINDOW_STATE_STICKY 8
::constant GDK_WINDOW_STATE_FULLSCREEN 16
::constant GDK_WINDOW_STATE_ABOVE 32
::constant GDK_WINDOW_STATE_BELOW 64
   -- GdkSettingAction
::constant GDK_SETTING_ACTION_NEW 0
::constant GDK_SETTING_ACTION_CHANGED 1
::constant GDK_SETTING_ACTION_DELETED 2
   -- GdkOwnerChange
::constant GDK_OWNER_CHANGE_NEW_OWNER 0
::constant GDK_OWNER_CHANGE_DESTROY 1
::constant GDK_OWNER_CHANGE_CLOSE 2
   -- GdkMasks
::constant GDK_SHIFT_MASK    1
::constant GDK_LOCK_MASK      2
::constant GDK_CONTROL_MASK  4
::constant GDK_MOD1_MASK      8
::constant GDK_MOD2_MASK      16
::constant GDK_MOD3_MASK      32
::constant GDK_MOD4_MASK      64
::constant GDK_MOD5_MASK      128
::constant GDK_BUTTON1_MASK  256
::constant GDK_BUTTON2_MASK  512
::constant GDK_BUTTON3_MASK  1024
::constant GDK_BUTTON4_MASK  2048
::constant GDK_BUTTON5_MASK  4096
   -- GdkAccelFlags
::constant GTK_ACCEL_VISIBLE 1
::constant GTK_ACCEL_LOCKED  2
::constant GTK_ACCEL_MASK    7
   -- GdkKeySyms
::constant GDK_F1 'ffbe'x
::constant GDK_F2 'ffbf'x
::constant GDK_F3 'ffc0'x
::constant GDK_F4 'ffc1'x
::constant GDK_F5 'ffc2'x
::constant GDK_F6 'ffc3'x
::constant GDK_F7 'ffc4'x
::constant GDK_F8 'ffc5'x
::constant GDK_F9 'ffc6'x
::constant GDK_F10 'ffc7'x
::constant GDK_F11 'ffc8'x
::constant GDK_L1 'ffc8'x
::constant GDK_F12 'ffc9'x
::constant GDK_space '020'x
::constant GDK_exclam '021'x
::constant GDK_quotedbl '022'x
::constant GDK_numbersign '023'x
::constant GDK_dollar '024'x
::constant GDK_percent '025'x
::constant GDK_ampersand '026'x
::constant GDK_apostrophe '027'x
::constant GDK_quoteright '027'x
::constant GDK_parenleft '028'x
::constant GDK_parenright '029'x
::constant GDK_asterisk '02a'x
::constant GDK_plus '02b'x
::constant GDK_comma '02c'x
::constant GDK_minus '02d'x
::constant GDK_period '02e'x
::constant GDK_slash '02f'x
::constant GDK_0 '030'x
::constant GDK_1 '031'x
::constant GDK_2 '032'x
::constant GDK_3 '033'x
::constant GDK_4 '034'x
::constant GDK_5 '035'x
::constant GDK_6 '036'x
::constant GDK_7 '037'x
::constant GDK_8 '038'x
::constant GDK_9 '039'x
::constant GDK_colon '03a'x
::constant GDK_semicolon '03b'x
::constant GDK_less '03c'x
::constant GDK_equal '03d'x
::constant GDK_greater '03e'x
::constant GDK_question '03f'x
::constant GDK_at '040'x
::constant GDK_bracketleft '05b'x
::constant GDK_backslash '05c'x
::constant GDK_bracketright '05d'x
::constant GDK_asciicircum '05e'x
::constant GDK_underscore '05f'x
::constant GDK_grave '060'x
::constant GDK_quoteleft '060'x
::constant GDK_braceleft '07b'x
::constant GDK_bar '07c'x
::constant GDK_braceright '07d'x
::constant GDK_asciitilde '07e'x
-- The name of these constants have been slightly altered because ooRexx
-- does not differntiate between mixed case constants.
::constant GDK_UA '041'x
::constant GDK_UB '042'x
::constant GDK_UC '043'x
::constant GDK_UD '044'x
::constant GDK_UE '045'x
::constant GDK_UF '046'x
::constant GDK_UG '047'x
::constant GDK_UH '048'x
::constant GDK_UI '049'x
::constant GDK_UJ '04a'x
::constant GDK_UK '04b'x
::constant GDK_UL '04c'x
::constant GDK_UM '04d'x
::constant GDK_UN '04e'x
::constant GDK_UO '04f'x
::constant GDK_UP '050'x
::constant GDK_UQ '051'x
::constant GDK_UR '052'x
::constant GDK_US '053'x
::constant GDK_UT '054'x
::constant GDK_UU '055'x
::constant GDK_UV '056'x
::constant GDK_UW '057'x
::constant GDK_UX '058'x
::constant GDK_UY '059'x
::constant GDK_UZ '05a'x
::constant GDK_La '061'x
::constant GDK_Lb '062'x
::constant GDK_Lc '063'x
::constant GDK_Ld '064'x
::constant GDK_Le '065'x
::constant GDK_Lf '066'x
::constant GDK_Lg '067'x
::constant GDK_Lh '068'x
::constant GDK_Li '069'x
::constant GDK_Lj '06a'x
::constant GDK_Lk '06b'x
::constant GDK_Ll '06c'x
::constant GDK_Lm '06d'x
::constant GDK_Ln '06e'x
::constant GDK_Lo '06f'x
::constant GDK_Lp '070'x
::constant GDK_Lq '071'x
::constant GDK_Lr '072'x
::constant GDK_Ls '073'x
::constant GDK_Lt '074'x
::constant GDK_Lu '075'x
::constant GDK_Lv '076'x
::constant GDK_Lw '077'x
::constant GDK_Lx '078'x
::constant GDK_Ly '079'x
::constant GDK_Lz '07a'x

/*============================================================================*/
/* Class: GdkEvent                                                            */
/*============================================================================*/

::class GdkEvent public

::ATTRIBUTE type


/*============================================================================*/
/* Class: GdkEventAny                                                         */
/*============================================================================*/

::class GdkEventAny public subclass GdkEvent

::ATTRIBUTE window
::ATTRIBUTE send_event


/*============================================================================*/
/* Class: GdkEventKey                                                         */
/*============================================================================*/

::class GdkEventKey public subclass GdkEventAny

::ATTRIBUTE time
::ATTRIBUTE state
::ATTRIBUTE keyval
::ATTRIBUTE length
::ATTRIBUTE string
::ATTRIBUTE hardware_keycode
::ATTRIBUTE group
::ATTRIBUTE is_modifier


/*============================================================================*/
/* Class: GdkEventButton                                                      */
/*============================================================================*/

::class GdkEventButton public subclass GdkEventAny

::ATTRIBUTE time
::ATTRIBUTE x
::ATTRIBUTE y
::ATTRIBUTE axes
::ATTRIBUTE state
::ATTRIBUTE button
::ATTRIBUTE device
::ATTRIBUTE x_root
::ATTRIBUTE y_root


/*============================================================================*/
/* Class: GdkEventScroll                                                      */
/*============================================================================*/

::class GdkEventScroll public subclass GdkEventAny

::ATTRIBUTE time
::ATTRIBUTE x
::ATTRIBUTE y
::ATTRIBUTE state
::ATTRIBUTE direction
::ATTRIBUTE device
::ATTRIBUTE x_root
::ATTRIBUTE y_root


/*============================================================================*/
/* Class: GdkEventMotion                                                      */
/*============================================================================*/

::class GdkEventMotion public subclass GdkEventAny

::ATTRIBUTE time
::ATTRIBUTE x
::ATTRIBUTE y
::ATTRIBUTE axes
::ATTRIBUTE state
::ATTRIBUTE is_hint
::ATTRIBUTE device
::ATTRIBUTE x_root
::ATTRIBUTE y_root


/*============================================================================*/
/* Class: GdkEventExpose                                                      */
/*============================================================================*/

::class GdkEventExpose public subclass GdkEventAny

::ATTRIBUTE area
::ATTRIBUTE region
::ATTRIBUTE count


/*============================================================================*/
/* Class: GdkEventVisibility                                                  */
/*============================================================================*/

::class GdkEventVisibility public subclass GdkEventAny

::ATTRIBUTE state


/*============================================================================*/
/* Class: GdkEventCrossing                                                    */
/*============================================================================*/

::class GdkEventCrossing public subclass GdkEventAny

::ATTRIBUTE subwindow
::ATTRIBUTE time
::ATTRIBUTE x
::ATTRIBUTE y
::ATTRIBUTE x_root
::ATTRIBUTE y_root
::ATTRIBUTE mode
::ATTRIBUTE detail
::ATTRIBUTE focus
::ATTRIBUTE state


/*============================================================================*/
/* Class: GdkEventFocus                                                       */
/*============================================================================*/

::class GdkEventFocus public subclass GdkEventAny

::ATTRIBUTE in


/*============================================================================*/
/* Class: GdkEventConfigure                                                   */
/*============================================================================*/

::class GdkEventConfigure public subclass GdkEventAny

::ATTRIBUTE x
::ATTRIBUTE y
::ATTRIBUTE width
::ATTRIBUTE height


/*============================================================================*/
/* Class: GdkEventProperty                                                    */
/*============================================================================*/

::class GdkEventProperty public subclass GdkEventAny

::ATTRIBUTE atom
::ATTRIBUTE time
::ATTRIBUTE state


/*============================================================================*/
/* Class: GdkEventSelection                                                   */
/*============================================================================*/

::class GdkEventSelection public subclass GdkEventAny

::ATTRIBUTE selection
::ATTRIBUTE target
::ATTRIBUTE property
::ATTRIBUTE time
::ATTRIBUTE requestor


/*============================================================================*/
/* Class: GdkEventDND                                                         */
/*============================================================================*/

::class GdkEventDND public subclass GdkEventAny

::ATTRIBUTE context
::ATTRIBUTE time
::ATTRIBUTE property
::ATTRIBUTE x_root
::ATTRIBUTE y_root


/*============================================================================*/
/* Class: GdkEventProximity                                                   */
/*============================================================================*/

::class GdkEventProximity public subclass GdkEventAny

::ATTRIBUTE time
::ATTRIBUTE device


/*============================================================================*/
/* Class: GdkEventClient                                                      */
/*============================================================================*/

::class GdkEventClient public subclass GdkEventAny

::ATTRIBUTE message_type
::ATTRIBUTE data_format
::ATTRIBUTE data


/*============================================================================*/
/* Class: GdkEventNoExpose                                                    */
/*============================================================================*/

::class GdkEventNoExpose public subclass GdkEventAny


/*============================================================================*/
/* Class: GdkEventWindowState                                                 */
/*============================================================================*/

::class GdkEventWindowState public subclass GdkEventAny

::ATTRIBUTE changed_mask
::ATTRIBUTE new_window_state


/*============================================================================*/
/* Class: GdkEventSetting                                                     */
/*============================================================================*/

::class GdkEventSetting public subclass GdkEventAny

::ATTRIBUTE action
::ATTRIBUTE name


/*============================================================================*/
/* Class: GdkEventOwnerChange                                                 */
/*============================================================================*/

::class GdkEventOwnerChange public subclass GdkEventAny

::ATTRIBUTE owner
::ATTRIBUTE reason
::ATTRIBUTE selection
::ATTRIBUTE time
::ATTRIBUTE selection_time


/*============================================================================*/
/* Class: GdkEventGrabBroken                                                  */
/*============================================================================*/

::class GdkEventGrabBroken public subclass GdkEventAny

::ATTRIBUTE keyboard
::ATTRIBUTE implicit
::ATTRIBUTE grab_window


/*============================================================================*/
/* Class: GdkRectangle                                                        */
/*============================================================================*/

::class GdkRectangle public

::ATTRIBUTE x
::ATTRIBUTE y
::ATTRIBUTE width
::ATTRIBUTE height


/*============================================================================*/
/* Class: GtkWidget                                                           */
/* Note:  This is strictly an abstract class. It should never be instantiated */
/*        on its own. You should instantiate one of its subclasses instead.   */
/*============================================================================*/

::class GtkWidget public

::METHOD init EXTERNAL "LIBRARY oorexxgtk2 GrxWidgetNew"

-- This attribute is not a part of the GTK system. It has been added to
-- support callbacks or any other use the user may have.
::attribute user_data

-- This method is not a part of the GTK system. It has been added to
-- support creating GdkEvent objects from C++ methods.
-- Note: Do not make this method private!
::method create_event_obj
use strict arg clsname
return .context~package~findClass(clsname)~new()

::METHOD set_cself EXTERNAL "LIBRARY oorexxgtk2 GrxWidgetSetCSELF"
::METHOD destroy EXTERNAL "LIBRARY oorexxgtk2 GrxWidgetDestroy"
::METHOD font EXTERNAL "LIBRARY oorexxgtk2 GrxWidgetGetSetFont"
::METHOD get_font EXTERNAL "LIBRARY oorexxgtk2 GrxWidgetGetSetFont"
::METHOD 'font=' EXTERNAL "LIBRARY oorexxgtk2 GrxWidgetGetSetFont"
::METHOD set_font EXTERNAL "LIBRARY oorexxgtk2 GrxWidgetGetSetFont"
::METHOD modify_font EXTERNAL "LIBRARY oorexxgtk2 GrxWidgetGetSetFont"
::METHOD grab_focus EXTERNAL "LIBRARY oorexxgtk2 GrxWidgetGrabFocus"
::METHOD hide EXTERNAL "LIBRARY oorexxgtk2 GrxWidgetHide"
::METHOD hide_all EXTERNAL "LIBRARY oorexxgtk2 GrxWidgetHideAll"
::METHOD set_size_request EXTERNAL "LIBRARY oorexxgtk2 GrxWidgetSetSizeRequest"
::METHOD show EXTERNAL "LIBRARY oorexxgtk2 GrxWidgetShow"
::METHOD show_all EXTERNAL "LIBRARY oorexxgtk2 GrxWidgetShowAll"
::METHOD name EXTERNAL "LIBRARY oorexxgtk2 GrxWidgetGetSetName"
::METHOD get_name EXTERNAL "LIBRARY oorexxgtk2 GrxWidgetGetSetName"
::METHOD 'name=' EXTERNAL "LIBRARY oorexxgtk2 GrxWidgetGetSetName"
::METHOD set_name EXTERNAL "LIBRARY oorexxgtk2 GrxWidgetGetSetName"
::METHOD 'sensitive=' EXTERNAL "LIBRARY oorexxgtk2 GrxWidgetSetSensitive"
::METHOD set_sensitive EXTERNAL "LIBRARY oorexxgtk2 GrxWidgetSetSensitive"
::METHOD modify_fg EXTERNAL "LIBRARY oorexxgtk2 GrxWidgetModifyFG"
::METHOD modify_bg EXTERNAL "LIBRARY oorexxgtk2 GrxWidgetModifyBG"
::METHOD set_data EXTERNAL "LIBRARY oorexxgtk2 GrxWidgetSetData"
::METHOD get_data EXTERNAL "LIBRARY oorexxgtk2 GrxWidgetGetData"
::METHOD activate EXTERNAL "LIBRARY oorexxgtk2 GrxWidgetActivate"
::METHOD reparent EXTERNAL "LIBRARY oorexxgtk2 GrxWidgetReparent"
::METHOD is_focus EXTERNAL "LIBRARY oorexxgtk2 GrxWidgetIsFocus"
::METHOD grab_default EXTERNAL "LIBRARY oorexxgtk2 GrxWidgetGrabDefault"
::METHOD set_parent_window EXTERNAL "LIBRARY oorexxgtk2 GrxWidgetGetSetParentWindow"
::METHOD 'parent_window=' EXTERNAL "LIBRARY oorexxgtk2 GrxWidgetGetSetParentWindow"
::METHOD get_parent_window EXTERNAL "LIBRARY oorexxgtk2 GrxWidgetGetSetParentWindow"
::METHOD parent_window EXTERNAL "LIBRARY oorexxgtk2 GrxWidgetGetSetParentWindow"
::METHOD get_toplevel EXTERNAL "LIBRARY oorexxgtk2 GrxWidgetGetToplevel"
::METHOD toplevel EXTERNAL "LIBRARY oorexxgtk2 GrxWidgetGetToplevel"
::METHOD get_ancestor EXTERNAL "LIBRARY oorexxgtk2 GrxWidgetGetAncestor"
::METHOD ancestor EXTERNAL "LIBRARY oorexxgtk2 GrxWidgetGetAncestor"
::METHOD is_ancestor EXTERNAL "LIBRARY oorexxgtk2 GrxWidgetIsAncestor"
::METHOD 'direction=' EXTERNAL "LIBRARY oorexxgtk2 GrxWidgetGetSetDirection"
::METHOD set_direction EXTERNAL "LIBRARY oorexxgtk2 GrxWidgetGetSetDirection"
::METHOD get_direction EXTERNAL "LIBRARY oorexxgtk2 GrxWidgetGetSetDirection"
::METHOD direction EXTERNAL "LIBRARY oorexxgtk2 GrxWidgetGetSetDirection"
::METHOD get_default_direction EXTERNAL "LIBRARY oorexxgtk2 GrxWidgetGetDefaultDirection"
::METHOD default_direction EXTERNAL "LIBRARY oorexxgtk2 GrxWidgetGetDefaultDirection"
::METHOD queue_draw_area EXTERNAL "LIBRARY oorexxgtk2 GrxWidgetQueueDrawArea"
::METHOD reset_shapes EXTERNAL "LIBRARY oorexxgtk2 GrxWidgetResetShapes"
::METHOD set_scroll_adjustments EXTERNAL "LIBRARY oorexxgtk2 GrxWidgetSetScrollAdjustments"
::METHOD mnemonic_activate EXTERNAL "LIBRARY oorexxgtk2 GrxWidgetMnemonicActivate"
::METHOD get_accessible EXTERNAL "LIBRARY oorexxgtk2 GrxWidgetGetAccessible"
::METHOD get_child_visible EXTERNAL "LIBRARY oorexxgtk2 GrxWidgetGetChildVisible"
::METHOD child_visible EXTERNAL "LIBRARY oorexxgtk2 GrxWidgetGetChildVisible"
::METHOD get_parent EXTERNAL "LIBRARY oorexxgtk2 GrxWidgetGetParent"
::METHOD parent EXTERNAL "LIBRARY oorexxgtk2 GrxWidgetGetParent"
::METHOD get_menus EXTERNAL "LIBRARY oorexxgtk2 GrxWidgetGetMenus"
::METHOD add_accelerator EXTERNAL "LIBRARY oorexxgtk2 GrxWidgetAddAccelerator"
::METHOD remove_accelerator EXTERNAL "LIBRARY oorexxgtk2 GrxWidgetRemoveAccelerator"
::METHOD set_accel_path EXTERNAL "LIBRARY oorexxgtk2 GrxWidgetSetAccelPath"
::METHOD set_events EXTERNAL "LIBRARY oorexxgtk2 GrxWidgetSetEvents"
::METHOD 'events=' EXTERNAL "LIBRARY oorexxgtk2 GrxWidgetSetEvents"
::METHOD realize EXTERNAL "LIBRARY oorexxgtk2 GrxWidgetRealize"
::METHOD unrealize EXTERNAL "LIBRARY oorexxgtk2 GrxWidgetUnrealize"
::METHOD signal_connect EXTERNAL "LIBRARY oorexxgtk2 GrxWidgetSignalConnect"

-- The following are the methods that can be connected to signals. By default
-- they do nothing.

::method signal_destroy
return

::method signal_unrealize
return

::method signal_unmap
return

::method signal_state_changed
return

::method signal_show
return

::method signal_realize
return

::method signal_popup_menu
return

::method signal_parent_set
return

::method signal_move_focus
return

::method signal_mnemonic_activate
return

::method signal_map
return

::method signal_keynav_failed
return

::method signal_hiearchy_changed
return

::method signal_hide
return

::method signal_grab_notify
return

::method signal_grab_focus
return

::method signal_focus
return

::method signal_composited_changed
return

::method signal_accel_closures_changed
return

::method signal_button_press_event
return .false

::method signal_button_release_event
return .false

::method signal_client_event
return .false

::method signal_configure_event
return .false

::method signal_delete_event
return .false

::method signal_destroy_event
return .false

::method signal_enter_notify_event
return .false

::method signal_event
return .false

::method signal_event_after
return .false

::method signal_expose_event
return .false

::method signal_focus_in_event
return .false

::method signal_focus_out_event
return .false

::method signal_grab_broken_event
return .false

::method signal_key_press_event
return .false

::method signal_key_release_event
return .false

::method signal_leave_notify_event
return .false

::method signal_map_event
return .false

::method signal_motion_notify_event
return .false

::method signal_no_expose_event
return .false

::method signal_property_notify_event
return .false

::method signal_proximity_in_event
return .false

::method signal_proximity_out_event
return .false

::method signal_scroll_event
return .false

::method signal_selection_clear_event
return .false

::method signal_selection_notify_event
return .false

::method signal_selection_request_event
return .false

::method signal_visibility_notify_event
return .false


/*============================================================================*/
/* Class: GtkContainer                                                        */
/* Note:  This is strictly an abstract class. It should never be instantiated */
/*        on its own. You should instantiate one of its subclasses instead.   */
/*============================================================================*/

::class GtkContainer public subclass GtkWidget

::METHOD add EXTERNAL "LIBRARY oorexxgtk2 GrxContainerAdd"
::METHOD remove EXTERNAL "LIBRARY oorexxgtk2 GrxContainerRemove"
::METHOD set_border_width EXTERNAL "LIBRARY oorexxgtk2 GrxContainerSetBorderWidth"
::METHOD 'border_width=' EXTERNAL "LIBRARY oorexxgtk2 GrxContainerSetBorderWidth"
::METHOD signal_connect EXTERNAL "LIBRARY oorexxgtk2 GrxContainerSignalConnect"

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

::METHOD pack_end EXTERNAL "LIBRARY oorexxgtk2 GrxBoxPackEnd"

::method pack_end_defaults
use strict arg mywidget
self~pack_end(myWidget, .true, .true, 0)
return

::METHOD pack_start EXTERNAL "LIBRARY oorexxgtk2 GrxBoxPackStart"

::method pack_start_defaults
use strict arg mywidget
self~pack_start(mywidget, .true, .true, 0)
return


/*============================================================================*/
/* Class: GtkHBox                                                             */
/*============================================================================*/

::class GtkHBox public subclass GtkBox

::method init
if arg() = 1 then do
   use strict arg ptr
   self~init_1args(ptr)
   return
   end
use strict arg homog, spacing
self~init_2args(homog, spacing)
return

::METHOD init_1args private EXTERNAL "LIBRARY oorexxgtk2 GrxHBoxNewFromPtr"
::METHOD init_2args private EXTERNAL "LIBRARY oorexxgtk2 GrxHBoxNew"


/*============================================================================*/
/* Class: GtkVBox                                                             */
/*============================================================================*/

::class GtkVBox public subclass GtkBox

::method init
if arg() = 1 then do
   use strict arg ptr
   self~init_1args(ptr)
   return
   end
use strict arg homog, spacing
self~init_2args(homog, spacing)
return

::METHOD init_1args private EXTERNAL "LIBRARY oorexxgtk2 GrxVBoxNewFromPtr"
::METHOD init_2args private EXTERNAL "LIBRARY oorexxgtk2 GrxVBoxNew"


/*============================================================================*/
/* Class: GtkAlignment                                                        */
/*============================================================================*/

::class GtkAlignment public subclass GtkContainer

::METHOD init EXTERNAL "LIBRARY oorexxgtk2 GrxAlignmentNew"


/*============================================================================*/
/* Class: GtkPaned                                                            */
/* Note:  This is strictly an abstract class. It should never be instantiated */
/*        on its own. You should instantiate one of its subclasses instead.   */
/*============================================================================*/

::class GtkPaned public subclass GtkContainer

::METHOD add1 EXTERNAL "LIBRARY oorexxgtk2 GrxPanedAdd1"
::METHOD add2 EXTERNAL "LIBRARY oorexxgtk2 GrxPanedAdd2"
::METHOD pack1 EXTERNAL "LIBRARY oorexxgtk2 GrxPanedPack1"
::METHOD pack2 EXTERNAL "LIBRARY oorexxgtk2 GrxPanedPack2"


/*============================================================================*/
/* Class: GtkHPaned                                                           */
/*============================================================================*/

::class GtkHPaned public subclass GtkPaned

::METHOD init EXTERNAL "LIBRARY oorexxgtk2 GrxHPanedNew"


/*============================================================================*/
/* Class: GtkVPaned                                                           */
/*============================================================================*/

::class GtkVPaned public subclass GtkPaned

::METHOD init EXTERNAL "LIBRARY oorexxgtk2 GrxVPanedNew"


/*============================================================================*/
/* Class: GtkEventBox                                                         */
/*============================================================================*/

::class GtkEventBox public subclass GtkContainer

::METHOD init EXTERNAL "LIBRARY oorexxgtk2 GrxEventBoxNew"
::METHOD set_above_child EXTERNAL "LIBRARY oorexxgtk2 GrxEventBoxGetSetAboveChild"
::METHOD 'above_child=' EXTERNAL "LIBRARY oorexxgtk2 GrxEventBoxGetSetAboveChild"
::METHOD get_above_child EXTERNAL "LIBRARY oorexxgtk2 GrxEventBoxGetSetAboveChild"
::METHOD above_child EXTERNAL "LIBRARY oorexxgtk2 GrxEventBoxGetSetAboveChild"
::METHOD set_visible_window EXTERNAL "LIBRARY oorexxgtk2 GrxEventBoxGetSetVisibleWindow"
::METHOD 'visible_window=' EXTERNAL "LIBRARY oorexxgtk2 GrxEventBoxGetSetVisibleWindow"
::METHOD get_visible_window EXTERNAL "LIBRARY oorexxgtk2 GrxEventBoxGetSetVisibleWindow"
::METHOD visible_window EXTERNAL "LIBRARY oorexxgtk2 GrxEventBoxGetSetVisibleWindow"


/*============================================================================*/
/* Class: GtkWindow                                                           */
/*============================================================================*/

::class GtkWindow public subclass GtkContainer

::METHOD init EXTERNAL "LIBRARY oorexxgtk2 GrxWindowNew"
::METHOD 'title=' EXTERNAL "LIBRARY oorexxgtk2 GrxWindowSetTitle"
::METHOD set_title EXTERNAL "LIBRARY oorexxgtk2 GrxWindowSetTitle"
::METHOD 'modal=' EXTERNAL "LIBRARY oorexxgtk2 GrxWindowSetModal"
::METHOD set_modal EXTERNAL "LIBRARY oorexxgtk2 GrxWindowSetModal"
::METHOD add_accel_group EXTERNAL "LIBRARY oorexxgtk2 GrxWindowAddAccelGroup"
::METHOD signal_connect EXTERNAL "LIBRARY oorexxgtk2 GrxWindowSignalConnect"

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
/* Class: GtkScrolledWindow                                                   */
/*============================================================================*/

::class GtkScrolledWindow public subclass GtkContainer

::METHOD init EXTERNAL "LIBRARY oorexxgtk2 GrxScrolledWindowNew"
::METHOD hadjustment EXTERNAL "LIBRARY oorexxgtk2 GrxScrolledWindowGetHAdjustment"
::METHOD get_hadjustment EXTERNAL "LIBRARY oorexxgtk2 GrxScrolledWindowGetHAdjustment"
::METHOD vadjustment EXTERNAL "LIBRARY oorexxgtk2 GrxScrolledWindowGetVAdjustment"
::METHOD get_vadjustment EXTERNAL "LIBRARY oorexxgtk2 GrxScrolledWindowGetVAdjustment"
::METHOD set_policy EXTERNAL "LIBRARY oorexxgtk2 GrxScrolledWindowSetPolicy"
::METHOD add_with_viewport EXTERNAL "LIBRARY oorexxgtk2 GrxScrolledWindowAddWithViewport"
::METHOD signal_connect EXTERNAL "LIBRARY oorexxgtk2 GrxScrolledWindowSignalConnect"

-- This method is not a part of the GTK system. It has been added to
-- support creating GtkAdjustment objects from C++ methods.
::METHOD create_adj private
use strict arg ptr
return .GtkAdjustment~new(ptr)


-- The following are the methods that can be connected to signals. By default
-- they do nothing.

::method signal_move_focus_out
return

::method signal_scroll_child
return


/*============================================================================*/
/* Class: GtkViewport                                                         */
/*============================================================================*/

::class GtkViewport public subclass GtkContainer

::METHOD init EXTERNAL "LIBRARY oorexxgtk2 GrxViewportNew"
::METHOD signal_connect EXTERNAL "LIBRARY oorexxgtk2 GrxViewportSignalConnect"

-- The following are the methods that can be connected to signals. By default
-- they do nothing.

::method signal_set_scroll_adjustments
return

/*============================================================================*/
/* Class: GtkButton                                                           */
/*============================================================================*/

::class GtkButton public subclass GtkContainer

::method newWithLabel class
use strict arg label = ''
button = self~new()
button~label = label
return button

::method newWithMnemonic class
use strict arg label = ''
button = self~new()
button~label = label
button~use_underline = .true
return button

::method newFromStock class
use strict arg label = ''
button = self~new()
button~label = label
button~use_stock = .true
return button

::METHOD init EXTERNAL "LIBRARY oorexxgtk2 GrxButtonNew"
::METHOD label EXTERNAL "LIBRARY oorexxgtk2 GrxButtonGetSetLabel"
::METHOD get_label EXTERNAL "LIBRARY oorexxgtk2 GrxButtonGetSetLabel"
::METHOD 'label=' EXTERNAL "LIBRARY oorexxgtk2 GrxButtonGetSetLabel"
::METHOD set_label EXTERNAL "LIBRARY oorexxgtk2 GrxButtonGetSetLabel"
::METHOD set_use_underline EXTERNAL "LIBRARY oorexxgtk2 GrxButtonUseUnderline"
::METHOD 'use_underline=' EXTERNAL "LIBRARY oorexxgtk2 GrxButtonUseUnderline"
::METHOD 'relief=' EXTERNAL "LIBRARY oorexxgtk2 GrxButtonSetRelief"
::METHOD set_relief EXTERNAL "LIBRARY oorexxgtk2 GrxButtonSetRelief"
::METHOD set_use_stock EXTERNAL "LIBRARY oorexxgtk2 GrxButtonSetUseStock"
::METHOD 'use_stock=' EXTERNAL "LIBRARY oorexxgtk2 GrxButtonSetUseStock"
::METHOD signal_connect EXTERNAL "LIBRARY oorexxgtk2 GrxButtonSignalConnect"

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
/* Class: GtkToggleButton                                                      */
/*============================================================================*/

::class GtkToggleButton public subclass GtkButton

::method newWithLabel class
use strict arg label = ''
button = self~new()
button~label = label
return button

::method newWithMnemonic class
use strict arg label = ''
button = self~new()
button~label = label
button~use_underline = .true
return button

::METHOD init EXTERNAL "LIBRARY oorexxgtk2 GrxToggleButtonNew"
::METHOD mode EXTERNAL "LIBRARY oorexxgtk2 GrxToggleButtonGetSetMode"
::METHOD get_mode EXTERNAL "LIBRARY oorexxgtk2 GrxToggleButtonGetSetMode"
::METHOD set_mode EXTERNAL "LIBRARY oorexxgtk2 GrxToggleButtonGetSetMode"
::METHOD 'mode=' EXTERNAL "LIBRARY oorexxgtk2 GrxToggleButtonGetSetMode"
::METHOD active EXTERNAL "LIBRARY oorexxgtk2 GrxToggleButtonGetSetActive"
::METHOD get_active EXTERNAL "LIBRARY oorexxgtk2 GrxToggleButtonGetSetActive"
::METHOD 'active=' EXTERNAL "LIBRARY oorexxgtk2 GrxToggleButtonGetSetActive"
::METHOD set_active EXTERNAL "LIBRARY oorexxgtk2 GrxToggleButtonGetSetActive"
::METHOD inconsistent EXTERNAL "LIBRARY oorexxgtk2 GrxToggleButtonGetSetInconsistent"
::METHOD get_inconsistent EXTERNAL "LIBRARY oorexxgtk2 GrxToggleButtonGetSetInconsistent"
::METHOD set_inconsistent EXTERNAL "LIBRARY oorexxgtk2 GrxToggleButtonGetSetInconsistent"
::METHOD 'inconsistent=' EXTERNAL "LIBRARY oorexxgtk2 GrxToggleButtonGetSetInconsistent"
::METHOD signal_connect EXTERNAL "LIBRARY oorexxgtk2 GrxToggleButtonSignalConnect"

::method signal_toggled
return


/*============================================================================*/
/* Class: GtkCheckbutton                                                      */
/*============================================================================*/

::class GtkCheckbutton public subclass GtkToggleButton

::method newWithLabel class
self~init:super
use strict arg label = ''
check = self~new()
check~label = label
return check

::method newWithMnemonic class
self~init:super
use strict arg label = ''
check = self~new()
check~label = label
check~use_underline = .true
return check

::METHOD init EXTERNAL "LIBRARY oorexxgtk2 GrxCheckButtonNew"


/*============================================================================*/
/* Class: GtkRadioButton                                                      */
/*============================================================================*/

::class GtkRadioButton public subclass GtkCheckbutton

::method newWithLabel class
use strict arg widget, label
return .GtkRadioButton~new(widget, label)

::method newWithMnemonic class
use strict arg widget, label
rb = .GtkRadioButton~new(widget, label)
rb~use_underline = .true
return rb
                         
::METHOD init EXTERNAL "LIBRARY oorexxgtk2 GrxRadioButtonNew"
::METHOD signal_connect EXTERNAL "LIBRARY oorexxgtk2 GrxRadioButtonSignalConnect"

-- The following are the methods that can be connected to signals. By default
-- they do nothing.

::method signal_group_changed
return


/*============================================================================*/
/* Class: GtkColorButton                                                      */
/*============================================================================*/

::class GtkColorButton public subclass GtkButton

::METHOD newWithColor class
use strict arg color
return .GtkColorButton~new(color)

::METHOD init EXTERNAL "LIBRARY oorexxgtk2 GrxColorButtonNew"
::METHOD 'title=' EXTERNAL "LIBRARY oorexxgtk2 GrxColorButtonSetTitle"
::METHOD set_title EXTERNAL "LIBRARY oorexxgtk2 GrxColorButtonSetTitle"
::METHOD color EXTERNAL "LIBRARY oorexxgtk2 GrxColorButtonGetSetColor"
::METHOD get_color EXTERNAL "LIBRARY oorexxgtk2 GrxColorButtonGetSetColor"
::METHOD set_color EXTERNAL "LIBRARY oorexxgtk2 GrxColorButtonGetSetColor"
::METHOD 'color=' EXTERNAL "LIBRARY oorexxgtk2 GrxColorButtonGetSetColor"
::METHOD signal_connect EXTERNAL "LIBRARY oorexxgtk2 GrxColorButtonSignalConnect"

-- The following are the methods that can be connected to signals. By default
-- they do nothing.

::method signal_color_set
return


/*============================================================================*/
/* Class: GtkFileChooserButton                                                */
/*============================================================================*/

::class GtkFileChooserButton public subclass GtkHBox inherit GtkFileChooser

::METHOD init EXTERNAL "LIBRARY oorexxgtk2 GrxFileChooserButtonNew"
::METHOD signal_connect_p private EXTERNAL "LIBRARY oorexxgtk2 GrxFileChooserButtonSignalConnect"

::METHOD signal_connect
use strict arg event
-- first try the inherited GtkFileChooser class hiearchy first
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

::METHOD newWithFont class
use strict arg font
button = self~new()
button~set_font(font)
return button

::METHOD init EXTERNAL "LIBRARY oorexxgtk2 GrxFontButtonNew"
::METHOD 'title=' EXTERNAL "LIBRARY oorexxgtk2 GrxFontButtonSetTitle"
::METHOD set_title EXTERNAL "LIBRARY oorexxgtk2 GrxFontButtonSetTitle"
::METHOD font_name EXTERNAL "LIBRARY oorexxgtk2 GrxFontButtonGetFontName"
::METHOD get_font_name EXTERNAL "LIBRARY oorexxgtk2 GrxFontButtonGetFontName"
::METHOD signal_connect EXTERNAL "LIBRARY oorexxgtk2 GrxFontButtonSignalConnect"

-- The following are the methods that can be connected to signals. By default
-- they do nothing.

::method signal_font_set
return


/*============================================================================*/
/* Class: GtkTable                                                            */
/*============================================================================*/

::class GtkTable public subclass GtkContainer

::METHOD init EXTERNAL "LIBRARY oorexxgtk2 GrxTableNew"

::METHOD 'row_spacings=' EXTERNAL "LIBRARY oorexxgtk2 GrxTableSetRowSpacings"
::METHOD set_row_spacings EXTERNAL "LIBRARY oorexxgtk2 GrxTableSetRowSpacings"
::METHOD 'col_spacings=' EXTERNAL "LIBRARY oorexxgtk2 GrxTableSetColSpacings"
::METHOD set_col_spacings EXTERNAL "LIBRARY oorexxgtk2 GrxTableSetColSpacings"
::METHOD attach EXTERNAL "LIBRARY oorexxgtk2 GrxTableAttach"

::method attach_defaults
use strict arg widget, left, right, top, bottom
self~attach(widget, left, right, top, bottom,5, 5, 0, 0)
return


/*============================================================================*/
/* Class: GtkComboBox                                                         */
/*============================================================================*/

::class GtkComboBox public subclass GtkWidget

::METHOD init EXTERNAL "LIBRARY oorexxgtk2 GrxComboBoxNew"
::METHOD append_text EXTERNAL "LIBRARY oorexxgtk2 GrxComboBoxAppendText"
::METHOD get_active_text EXTERNAL "LIBRARY oorexxgtk2 GrxComboBoxGetActiveText"
::METHOD active_text EXTERNAL "LIBRARY oorexxgtk2 GrxComboBoxGetActiveText"
::METHOD get_active EXTERNAL "LIBRARY oorexxgtk2 GrxComboBoxGetSetActive"
::METHOD active EXTERNAL "LIBRARY oorexxgtk2 GrxComboBoxGetSetActive"
::METHOD set_active EXTERNAL "LIBRARY oorexxgtk2 GrxComboBoxGetSetActive"
::METHOD 'active=' EXTERNAL "LIBRARY oorexxgtk2 GrxComboBoxGetSetActive"
::METHOD signal_connect EXTERNAL "LIBRARY oorexxgtk2 GrxComboBoxSignalConnect"

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
/* Class: GtkComboBoxText                                                     */
/*============================================================================*/

::class GtkComboBoxText public subclass GtkComboBox

::METHOD init EXTERNAL "LIBRARY oorexxgtk2 GrxComboBoxNewText"


/*============================================================================*/
/* Class: GtkEntry                                                            */
/*============================================================================*/

::class GtkEntry public subclass GtkWidget

::METHOD init EXTERNAL "LIBRARY oorexxgtk2 GrxEntryNew"
::METHOD max_length EXTERNAL "LIBRARY oorexxgtk2 GrxEntryGetSetMaxLength"
::METHOD get_max_length EXTERNAL "LIBRARY oorexxgtk2 GrxEntryGetSetMaxLength"
::METHOD set_max_length EXTERNAL "LIBRARY oorexxgtk2 GrxEntryGetSetMaxLength"
::METHOD 'max_length=' EXTERNAL "LIBRARY oorexxgtk2 GrxEntryGetSetMaxLength"
::METHOD set_width_chars EXTERNAL "LIBRARY oorexxgtk2 GrxEntrySetWidthChars"
::METHOD 'width_chars=' EXTERNAL "LIBRARY oorexxgtk2 GrxEntrySetWidthChars"
::METHOD text EXTERNAL "LIBRARY oorexxgtk2 GrxEntryGetSetText"
::METHOD get_text EXTERNAL "LIBRARY oorexxgtk2 GrxEntryGetSetText"
::METHOD set_text EXTERNAL "LIBRARY oorexxgtk2 GrxEntryGetSetText"
::METHOD 'text=' EXTERNAL "LIBRARY oorexxgtk2 GrxEntryGetSetText"
::METHOD visibility EXTERNAL "LIBRARY oorexxgtk2 GrxEntryGetSetVisibility"
::METHOD get_visibility EXTERNAL "LIBRARY oorexxgtk2 GrxEntryGetSetVisibility"
::METHOD set_visibility EXTERNAL "LIBRARY oorexxgtk2 GrxEntryGetSetVisibility"
::METHOD 'visibility=' EXTERNAL "LIBRARY oorexxgtk2 GrxEntryGetSetVisibility"
::METHOD invisible_char EXTERNAL "LIBRARY oorexxgtk2 GrxEntryGetSetInvisibleChar"
::METHOD get_invisible_char EXTERNAL "LIBRARY oorexxgtk2 GrxEntryGetSetInvisibleChar"
::METHOD set_invisible_char EXTERNAL "LIBRARY oorexxgtk2 GrxEntryGetSetInvisibleChar"
::METHOD 'invisible_char=' EXTERNAL "LIBRARY oorexxgtk2 GrxEntryGetSetInvisibleChar"
::METHOD signal_connect EXTERNAL "LIBRARY oorexxgtk2 GrxEntrySignalConnect"

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

::METHOD newWithRange class
use strict arg p1, p2, p3
return .GtkSpinButton~new(p1, p2, p3)

::METHOD init
use strict arg p1, p2, p3
if p1~isInstanceOf(.GtkAdjustment) then self~init_3args(p1, p2, p3)
else self~init_range(p1, p2, p3)
return

::METHOD init_3args private EXTERNAL "LIBRARY oorexxgtk2 GrxSpinButtonNew"
::METHOD init_range private EXTERNAL "LIBRARY oorexxgtk2 GrxSpinButtonNewWithRange"

::METHOD set_digits EXTERNAL "LIBRARY oorexxgtk2 GrxSpinButtonSetDigits"
::METHOD 'digits=' EXTERNAL "LIBRARY oorexxgtk2 GrxSpinButtonSetDigits"
::METHOD signal_connect EXTERNAL "LIBRARY oorexxgtk2 GrxSpinButtonSignalConnect"

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

::METHOD init EXTERNAL "LIBRARY oorexxgtk2 GrxDialogNewWithButtons"

::method 'vbox=' private
expose vbox
use strict arg ptr
vbox = .GtkVBox~new(ptr)
return

::method vbox
expose vbox
return vbox

::METHOD 'has_separator=' EXTERNAL "LIBRARY oorexxgtk2 GrxDialogSetHasSeparator"
::METHOD set_has_separator EXTERNAL "LIBRARY oorexxgtk2 GrxDialogSetHasSeparator"
::METHOD 'default_response=' EXTERNAL "LIBRARY oorexxgtk2 GrxDialogSetDefaultResponse"
::METHOD set_default_response EXTERNAL "LIBRARY oorexxgtk2 GrxDialogSetDefaultResponse"
::METHOD add_button EXTERNAL "LIBRARY oorexxgtk2 GrxDialogAddButton"
-- Note: the name of this method cannot be 'run' as that would override the
-- private 'run' instance method of the ooRexx Object class.
::METHOD dialog_run EXTERNAL "LIBRARY oorexxgtk2 GrxDialogRun"
::METHOD signal_connect EXTERNAL "LIBRARY oorexxgtk2 GrxDialogSignalConnect"

-- The following are the methods that can be connected to signals. By default
-- they do nothing.

::method signal_close
return

::method signal_response
return


/*============================================================================*/
/* Class: GtkMessageDialog                                                    */
/*============================================================================*/

::class GtkMessageDialog public subclass GtkDialog

::METHOD init EXTERNAL "LIBRARY oorexxgtk2 GrxMessageDialogNew"


/*============================================================================*/
/* Class: GtkColorSelectionDialog                                             */
/*============================================================================*/

::class GtkColorSelectionDialog public subclass GtkDialog

::METHOD init EXTERNAL "LIBRARY oorexxgtk2 GrxColorSelectionDialogNew"
::METHOD color EXTERNAL "LIBRARY oorexxgtk2 GrxColorSelectionDialogGetColor"
::METHOD get_color EXTERNAL "LIBRARY oorexxgtk2 GrxColorSelectionDialogGetColor"


/*============================================================================*/
/* Class: GtkFontSelectionDialog                                              */
/*============================================================================*/

::class GtkFontSelectionDialog public subclass GtkDialog

::METHOD init EXTERNAL "LIBRARY oorexxgtk2 GrxFontSelectionDialogNew"
::METHOD 'font_name=' EXTERNAL "LIBRARY oorexxgtk2 GrxFontSelectionDialogGetSetFontName"
::METHOD set_font_name EXTERNAL "LIBRARY oorexxgtk2 GrxFontSelectionDialogGetSetFontName"
::METHOD font_name EXTERNAL "LIBRARY oorexxgtk2 GrxFontSelectionDialogGetSetFontName"
::METHOD get_font_name EXTERNAL "LIBRARY oorexxgtk2 GrxFontSelectionDialogGetSetFontName"
::METHOD 'preview_text=' EXTERNAL "LIBRARY oorexxgtk2 GrxFontSelectionDialogSetPreviewText"
::METHOD set_preview_text EXTERNAL "LIBRARY oorexxgtk2 GrxFontSelectionDialogSetPreviewText"


/*============================================================================*/
/* Class: GtkAboutDialog                                                      */
/*============================================================================*/

::class GtkAboutDialog public subclass GtkDialog

::METHOD init EXTERNAL "LIBRARY oorexxgtk2 GrxAboutDialogNew"
::METHOD 'name=' EXTERNAL "LIBRARY oorexxgtk2 GrxAboutDialogSetName"
::METHOD set_name EXTERNAL "LIBRARY oorexxgtk2 GrxAboutDialogSetName"
::METHOD 'version=' EXTERNAL "LIBRARY oorexxgtk2 GrxAboutDialogSetVersion"
::METHOD set_version EXTERNAL "LIBRARY oorexxgtk2 GrxAboutDialogSetVersion"
::METHOD 'copyright=' EXTERNAL "LIBRARY oorexxgtk2 GrxAboutDialogSetCopyright"
::METHOD set_copyright EXTERNAL "LIBRARY oorexxgtk2 GrxAboutDialogSetCopyright"
::METHOD 'comments=' EXTERNAL "LIBRARY oorexxgtk2 GrxAboutDialogSetComments"
::METHOD set_comments EXTERNAL "LIBRARY oorexxgtk2 GrxAboutDialogSetComments"
::METHOD 'license=' EXTERNAL "LIBRARY oorexxgtk2 GrxAboutDialogSetLicense"
::METHOD set_license EXTERNAL "LIBRARY oorexxgtk2 GrxAboutDialogSetLicense"
::METHOD 'wrap_license=' EXTERNAL "LIBRARY oorexxgtk2 GrxAboutDialogSetWrapLicense"
::METHOD set_wrap_license EXTERNAL "LIBRARY oorexxgtk2 GrxAboutDialogSetWrapLicense"
::METHOD 'website=' EXTERNAL "LIBRARY oorexxgtk2 GrxAboutDialogSetWebsite"
::METHOD set_website EXTERNAL "LIBRARY oorexxgtk2 GrxAboutDialogSetWebsite"
::METHOD 'website_label=' EXTERNAL "LIBRARY oorexxgtk2 GrxAboutDialogSetWebsiteLabel"
::METHOD set_website_label EXTERNAL "LIBRARY oorexxgtk2 GrxAboutDialogSetWebsiteLabel"
::METHOD set_authors EXTERNAL "LIBRARY oorexxgtk2 GrxAboutDialogSetAuthors"
::METHOD set_artists EXTERNAL "LIBRARY oorexxgtk2 GrxAboutDialogSetArtists"
::METHOD set_documentors EXTERNAL "LIBRARY oorexxgtk2 GrxAboutDialogSetDocumentors"
::METHOD 'logo=' EXTERNAL "LIBRARY oorexxgtk2 GrxAboutDialogSetLogo"
::METHOD set_logo EXTERNAL "LIBRARY oorexxgtk2 GrxAboutDialogSetLogo"


/*============================================================================*/
/* Class: GtkFileChooserDialog                                                */
/*============================================================================*/

::class GtkFileChooserDialog public subclass GtkDialog inherit GtkFileChooser

::METHOD init EXTERNAL "LIBRARY oorexxgtk2 GrxFileChooserDialogNew"


/*============================================================================*/
/* Class: GtkAssistant                                                        */
/*============================================================================*/

::class GtkAssistant public subclass GtkWindow

::METHOD init EXTERNAL "LIBRARY oorexxgtk2 GrxAssistantNew"
::METHOD set_current_page EXTERNAL "LIBRARY oorexxgtk2 GrxAssistantGetSetCurrentPage"
::METHOD 'current_page=' EXTERNAL "LIBRARY oorexxgtk2 GrxAssistantGetSetCurrentPage"
::METHOD get_current_page EXTERNAL "LIBRARY oorexxgtk2 GrxAssistantGetSetCurrentPage"
::METHOD current_page EXTERNAL "LIBRARY oorexxgtk2 GrxAssistantGetSetCurrentPage"
::METHOD get_nth_page EXTERNAL "LIBRARY oorexxgtk2 GrxAssistantGetNthPage"
::METHOD nth_page EXTERNAL "LIBRARY oorexxgtk2 GrxAssistantGetNthPage"
::METHOD prepend_page EXTERNAL "LIBRARY oorexxgtk2 GrxAssistantPrependPage"
::METHOD append_page EXTERNAL "LIBRARY oorexxgtk2 GrxAssistantAppendPage"
::METHOD insert_page EXTERNAL "LIBRARY oorexxgtk2 GrxAssistantInsertPage"
::METHOD set_page_type EXTERNAL "LIBRARY oorexxgtk2 GrxAssistantSetPageType"
::METHOD set_page_complete EXTERNAL "LIBRARY oorexxgtk2 GrxAssistantSetPageComplete"
::METHOD set_page_header_image EXTERNAL "LIBRARY oorexxgtk2 GrxAssistantSetPageHeaderImage"
::METHOD set_page_side_image EXTERNAL "LIBRARY oorexxgtk2 GrxAssistantSetPageSideImage"
::METHOD set_page_title EXTERNAL "LIBRARY oorexxgtk2 GrxAssistantSetPageTitle"
::METHOD signal_connect EXTERNAL "LIBRARY oorexxgtk2 GrxAssistantSignalConnect"

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

::METHOD init EXTERNAL "LIBRARY oorexxgtk2 GrxLabelNew"
::METHOD get_justify EXTERNAL "LIBRARY oorexxgtk2 GrxLabelGetSetJustify"
::METHOD justify EXTERNAL "LIBRARY oorexxgtk2 GrxLabelGetSetJustify"
::METHOD 'justify=' EXTERNAL "LIBRARY oorexxgtk2 GrxLabelGetSetJustify"
::METHOD set_justify EXTERNAL "LIBRARY oorexxgtk2 GrxLabelGetSetJustify"
::METHOD set_label EXTERNAL "LIBRARY oorexxgtk2 GrxLabelGetSetLabel"
::METHOD 'label=' EXTERNAL "LIBRARY oorexxgtk2 GrxLabelGetSetLabel"
::METHOD get_label EXTERNAL "LIBRARY oorexxgtk2 GrxLabelGetSetLabel"
::METHOD label EXTERNAL "LIBRARY oorexxgtk2 GrxLabelGetSetLabel"
::METHOD set_selectable EXTERNAL "LIBRARY oorexxgtk2 GrxLabelSetSelectable"
::METHOD 'selectable=' EXTERNAL "LIBRARY oorexxgtk2 GrxLabelSetSelectable"
::METHOD signal_connect EXTERNAL "LIBRARY oorexxgtk2 GrxLabelSignalConnect"

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

::METHOD init EXTERNAL "LIBRARY oorexxgtk2 GrxFixedNew"
::METHOD put EXTERNAL "LIBRARY oorexxgtk2 GrxFixedPut"
::METHOD move EXTERNAL "LIBRARY oorexxgtk2 GrxFixedMove"


/*============================================================================*/
/* Class: GtkExpander                                                         */
/*============================================================================*/

::class GtkExpander public subclass GtkContainer

::method newWithMnemonic class
use strict arg label = ''
exp = self~new(label)
exp~use_underline = .true
return exp

::METHOD init EXTERNAL "LIBRARY oorexxgtk2 GrxExpanderNew"
::METHOD set_expanded EXTERNAL "LIBRARY oorexxgtk2 GrxExpanderSetExpanded"
::METHOD 'expanded=' EXTERNAL "LIBRARY oorexxgtk2 GrxExpanderSetExpanded"
::METHOD label EXTERNAL "LIBRARY oorexxgtk2 GrxExpanderGetSetLabel"
::METHOD get_label EXTERNAL "LIBRARY oorexxgtk2 GrxExpanderGetSetLabel"
::METHOD set_label EXTERNAL "LIBRARY oorexxgtk2 GrxExpanderGetSetLabel"
::METHOD 'label=' EXTERNAL "LIBRARY oorexxgtk2 GrxExpanderGetSetLabel"
::METHOD set_use_underline EXTERNAL "LIBRARY oorexxgtk2 GrxExpanderSetUseUnderline"
::METHOD 'use_underline=' EXTERNAL "LIBRARY oorexxgtk2 GrxExpanderSetUseUnderline"
::METHOD set_spacing EXTERNAL "LIBRARY oorexxgtk2 GrxExpanderSetSpacing"
::METHOD 'spacing=' EXTERNAL "LIBRARY oorexxgtk2 GrxExpanderSetSpacing"
::METHOD signal_connect EXTERNAL "LIBRARY oorexxgtk2 GrxExpanderSignalConnect"

-- The following are the methods that can be connected to signals. By default
-- they do nothing.

::method signal_activate
return


/*============================================================================*/
/* Class: GtkHandleBox                                                        */
/*============================================================================*/

::class GtkHandleBox public subclass GtkContainer

::METHOD init EXTERNAL "LIBRARY oorexxgtk2 GrxHandleBoxNew"
::METHOD set_shadow_type EXTERNAL "LIBRARY oorexxgtk2 GrxHandleBoxSetShadowType"
::METHOD 'shadow_type=' EXTERNAL "LIBRARY oorexxgtk2 GrxHandleBoxSetShadowType"
::METHOD set_position EXTERNAL "LIBRARY oorexxgtk2 GrxHandleBoxSetPosition"
::METHOD 'position=' EXTERNAL "LIBRARY oorexxgtk2 GrxHandleBoxSetPosition"
::METHOD set_snap_edge EXTERNAL "LIBRARY oorexxgtk2 GrxHandleBoxSetSnapEdge"
::METHOD 'snap_edge=' EXTERNAL "LIBRARY oorexxgtk2 GrxHandleBoxSetSnapEdge"
::METHOD signal_connect EXTERNAL "LIBRARY oorexxgtk2 GrxHandleBoxSignalConnect"

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

::METHOD init EXTERNAL "LIBRARY oorexxgtk2 GrxNotebookNew"
::METHOD append_page EXTERNAL "LIBRARY oorexxgtk2 GrxNotebookAppendPage"
::METHOD prepend_page EXTERNAL "LIBRARY oorexxgtk2 GrxNotebookPrependPage"
::METHOD insert_page EXTERNAL "LIBRARY oorexxgtk2 GrxNotebookInsertPage"
::METHOD append_page_menu EXTERNAL "LIBRARY oorexxgtk2 GrxNotebookAppendPageMenu"
::METHOD prepend_page_menu EXTERNAL "LIBRARY oorexxgtk2 GrxNotebookPrependPageMenu"
::METHOD insert_page_menu EXTERNAL "LIBRARY oorexxgtk2 GrxNotebookInsertPageMenu"
::METHOD remove_page EXTERNAL "LIBRARY oorexxgtk2 GrxNotebookRemovePage"
::METHOD set_show_border EXTERNAL "LIBRARY oorexxgtk2 GrxNotebookSetShowBorder"
::METHOD 'show_border=' EXTERNAL "LIBRARY oorexxgtk2 GrxNotebookSetShowBorder"
::METHOD set_show_tabs EXTERNAL "LIBRARY oorexxgtk2 GrxNotebookSetShowTabs"
::METHOD 'show_tabs=' EXTERNAL "LIBRARY oorexxgtk2 GrxNotebookSetShowTabs"
::METHOD set_tab_pos EXTERNAL "LIBRARY oorexxgtk2 GrxNotebookSetTabPos"
::METHOD 'tab_pos=' EXTERNAL "LIBRARY oorexxgtk2 GrxNotebookSetTabPos"
::METHOD set_homogeneous_tabs EXTERNAL "LIBRARY oorexxgtk2 GrxNotebookSetHomogeneousTabs"
::METHOD 'homogeneous_tabs=' EXTERNAL "LIBRARY oorexxgtk2 GrxNotebookSetHomogeneousTabs"
::METHOD set_tab_border EXTERNAL "LIBRARY oorexxgtk2 GrxNotebookSetTabBorder"
::METHOD 'tab_border=' EXTERNAL "LIBRARY oorexxgtk2 GrxNotebookSetTabBorder"
::METHOD set_scrollable EXTERNAL "LIBRARY oorexxgtk2 GrxNotebookSetScrollable"
::METHOD 'scrollable=' EXTERNAL "LIBRARY oorexxgtk2 GrxNotebookSetScrollable"
::METHOD popup_enable EXTERNAL "LIBRARY oorexxgtk2 GrxNotebookPopupEnable"
::METHOD popup_disable EXTERNAL "LIBRARY oorexxgtk2 GrxNotebookPopupDisable"
::METHOD set_current_page EXTERNAL "LIBRARY oorexxgtk2 GrxNotebookGetSetCurrentPage"
::METHOD 'current_page=' EXTERNAL "LIBRARY oorexxgtk2 GrxNotebookGetSetCurrentPage"
::METHOD get_current_page EXTERNAL "LIBRARY oorexxgtk2 GrxNotebookGetSetCurrentPage"
::METHOD current_page EXTERNAL "LIBRARY oorexxgtk2 GrxNotebookGetSetCurrentPage"
::METHOD signal_connect EXTERNAL "LIBRARY oorexxgtk2 GrxNotebookSignalConnect"

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

::METHOD signal_connect EXTERNAL "LIBRARY oorexxgtk2 GrxRangeSignalConnect"

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

::METHOD set_digits EXTERNAL "LIBRARY oorexxgtk2 GrxScaleGetSetDigits"
::METHOD 'digits=' EXTERNAL "LIBRARY oorexxgtk2 GrxScaleGetSetDigits"
::METHOD get_digits EXTERNAL "LIBRARY oorexxgtk2 GrxScaleGetSetDigits"
::METHOD digits EXTERNAL "LIBRARY oorexxgtk2 GrxScaleGetSetDigits"
::METHOD set_value_pos EXTERNAL "LIBRARY oorexxgtk2 GrxScaleGetSetValuePos"
::METHOD 'value_pos=' EXTERNAL "LIBRARY oorexxgtk2 GrxScaleGetSetValuePos"
::METHOD get_value_pos EXTERNAL "LIBRARY oorexxgtk2 GrxScaleGetSetValuePos"
::METHOD value_pos EXTERNAL "LIBRARY oorexxgtk2 GrxScaleGetSetValuePos"


/*============================================================================*/
/* Class: GtkHScale                                                           */
/*============================================================================*/

::class GtkHScale public subclass GtkScale

::method newWithRange class
use strict arg min, max, step
return .GtkHScale~new(min, max, step)

::METHOD init
if arg() = 3 then do
   use strict arg min, max, step
   self~init_3args(min, max, step)
   return
   end
use strict arg value, lower, upper, step, page, pagesz
self~init_6args(value, lower, upper, step, page, pagesz)
return

::METHOD init_6args private EXTERNAL "LIBRARY oorexxgtk2 GrxHScaleNew"
::METHOD init_3args private EXTERNAL "LIBRARY oorexxgtk2 GrxHScaleNewWithRange"



/*============================================================================*/
/* Class: GtkVScale                                                           */
/*============================================================================*/

::class GtkVScale public subclass GtkScale

::method newWithRange class
use strict arg min, max, step
return .GtkVScale~new(min, max, step)

::METHOD init
if arg() = 3 then do
   use strict arg min, max, step
   self~init_3args(min, max, step)
   return
   end
use strict arg value, lower, upper, step, page, pagesz
self~init_6args(value, lower, upper, step, page, pagesz)
return

::METHOD init_6args private EXTERNAL "LIBRARY oorexxgtk2 GrxVScaleNew"
::METHOD init_3args private EXTERNAL "LIBRARY oorexxgtk2 GrxVScaleNewWithRange"


/*============================================================================*/
/* Class: GtkImage                                                            */
/*============================================================================*/

::class GtkImage public subclass GtkWidget

::method newFromFile class
use strict arg filename
img = .GtkImage~new()
img~set_from_file(filename)
return img

::method newFromStock class
use strict arg stockid, size
img = .GtkImage~new()
img~set_from_stock(stockid, size)
return img

::method newFromIconName class
use strict arg iconname, size
img = .GtkImage~new()
img~set_from_icon_name(iconname, size)
return img

::METHOD init EXTERNAL "LIBRARY oorexxgtk2 GrxImageNew"
::METHOD set_from_file EXTERNAL "LIBRARY oorexxgtk2 GrxImageSetFromFile"
::METHOD set_from_stock EXTERNAL "LIBRARY oorexxgtk2 GrxImageSetFromStock"
::METHOD set_from_icon_name EXTERNAL "LIBRARY oorexxgtk2 GrxImageSetFromIconName"


/*============================================================================*/
/* Class: GtkProgressBar                                                      */
/*============================================================================*/

::class GtkProgressBar public subclass GtkWidget

::METHOD init EXTERNAL "LIBRARY oorexxgtk2 GrxProgressBarNew"
::METHOD set_text EXTERNAL "LIBRARY oorexxgtk2 GrxProgressBarSetText"
::METHOD 'text=' EXTERNAL "LIBRARY oorexxgtk2 GrxProgressBarSetText"
::METHOD set_fraction EXTERNAL "LIBRARY oorexxgtk2 GrxProgressBarSetFraction"
::METHOD set_pulse_step EXTERNAL "LIBRARY oorexxgtk2 GrxProgressBarSetPulseStep"
::METHOD pulse EXTERNAL "LIBRARY oorexxgtk2 GrxProgressBarPulse"
::METHOD 'fraction=' EXTERNAL "LIBRARY oorexxgtk2 GrxProgressBarSetFraction"


/*============================================================================*/
/* Class: GtkTextView                                                         */
/*============================================================================*/

::class GtkTextView public subclass GtkContainer

::METHOD init EXTERNAL "LIBRARY oorexxgtk2 GrxTextViewNew"
::METHOD get_buffer EXTERNAL "LIBRARY oorexxgtk2 GrxTextViewGetBuffer"
::METHOD buffer EXTERNAL "LIBRARY oorexxgtk2 GrxTextViewGetBuffer"
::METHOD get_wrap_mode EXTERNAL "LIBRARY oorexxgtk2 GrxTextViewGetSetWrapMode"
::METHOD wrap_mode EXTERNAL "LIBRARY oorexxgtk2 GrxTextViewGetSetWrapMode"
::METHOD 'wrap_mode=' EXTERNAL "LIBRARY oorexxgtk2 GrxTextViewGetSetWrapMode"
::METHOD set_wrap_mode EXTERNAL "LIBRARY oorexxgtk2 GrxTextViewGetSetWrapMode"
::METHOD get_justification EXTERNAL "LIBRARY oorexxgtk2 GrxTextViewGetSetJustification"
::METHOD justification EXTERNAL "LIBRARY oorexxgtk2 GrxTextViewGetSetJustification"
::METHOD set_justification EXTERNAL "LIBRARY oorexxgtk2 GrxTextViewGetSetJustification"
::METHOD 'justification=' EXTERNAL "LIBRARY oorexxgtk2 GrxTextViewGetSetJustification"
::METHOD get_editable EXTERNAL "LIBRARY oorexxgtk2 GrxTextViewGetSetEditable"
::METHOD editable EXTERNAL "LIBRARY oorexxgtk2 GrxTextViewGetSetEditable"
::METHOD set_editable EXTERNAL "LIBRARY oorexxgtk2 GrxTextViewGetSetEditable"
::METHOD 'editable=' EXTERNAL "LIBRARY oorexxgtk2 GrxTextViewGetSetEditable"
::METHOD get_cursor_visible EXTERNAL "LIBRARY oorexxgtk2 GrxTextViewGetSetCursorVisible"
::METHOD cursor_visible EXTERNAL "LIBRARY oorexxgtk2 GrxTextViewGetSetCursorVisible"
::METHOD set_cursor_visible EXTERNAL "LIBRARY oorexxgtk2 GrxTextViewGetSetCursorVisible"
::METHOD 'cursor_visible=' EXTERNAL "LIBRARY oorexxgtk2 GrxTextViewGetSetCursorVisible"
::METHOD get_pixels_above_lines EXTERNAL "LIBRARY oorexxgtk2 GrxTextViewGetSetPixelsAboveLines"
::METHOD pixels_above_lines EXTERNAL "LIBRARY oorexxgtk2 GrxTextViewGetSetPixelsAboveLines"
::METHOD set_pixels_above_lines EXTERNAL "LIBRARY oorexxgtk2 GrxTextViewGetSetPixelsAboveLines"
::METHOD 'pixels_above_lines=' EXTERNAL "LIBRARY oorexxgtk2 GrxTextViewGetSetPixelsAboveLines"
::METHOD get_pixels_below_lines EXTERNAL "LIBRARY oorexxgtk2 GrxTextViewGetSetPixelsBelowLines"
::METHOD pixels_below_lines EXTERNAL "LIBRARY oorexxgtk2 GrxTextViewGetSetPixelsBelowLines"
::METHOD set_pixels_below_lines EXTERNAL "LIBRARY oorexxgtk2 GrxTextViewGetSetPixelsBelowLines"
::METHOD 'pixels_below_lines=' EXTERNAL "LIBRARY oorexxgtk2 GrxTextViewGetSetPixelsBelowLines"
::METHOD get_pixels_inside_wrap EXTERNAL "LIBRARY oorexxgtk2 GrxTextViewGetSetPixelsInsideWrap"
::METHOD pixels_inside_wrap EXTERNAL "LIBRARY oorexxgtk2 GrxTextViewGetSetPixelsInsideWrap"
::METHOD set_pixels_inside_wrap EXTERNAL "LIBRARY oorexxgtk2 GrxTextViewGetSetPixelsInsideWrap"
::METHOD 'pixels_inside_wrap=' EXTERNAL "LIBRARY oorexxgtk2 GrxTextViewGetSetPixelsInsideWrap"
::METHOD get_left_margin EXTERNAL "LIBRARY oorexxgtk2 GrxTextViewGetSetLeftMargin"
::METHOD left_margin EXTERNAL "LIBRARY oorexxgtk2 GrxTextViewGetSetLeftMargin"
::METHOD set_left_margin EXTERNAL "LIBRARY oorexxgtk2 GrxTextViewGetSetLeftMargin"
::METHOD 'left_margin=' EXTERNAL "LIBRARY oorexxgtk2 GrxTextViewGetSetLeftMargin"
::METHOD get_right_margin EXTERNAL "LIBRARY oorexxgtk2 GrxTextViewGetSetRightMargin"
::METHOD right_margin EXTERNAL "LIBRARY oorexxgtk2 GrxTextViewGetSetRightMargin"
::METHOD set_right_margin EXTERNAL "LIBRARY oorexxgtk2 GrxTextViewGetSetRightMargin"
::METHOD 'right_margin=' EXTERNAL "LIBRARY oorexxgtk2 GrxTextViewGetSetRightMargin"
::METHOD add_child_at_offset EXTERNAL "LIBRARY oorexxgtk2 GrxTextViewAddChildAtOffset"
::METHOD signal_connect EXTERNAL "LIBRARY oorexxgtk2 GrxTextViewSignalConnect"

-- This method is not a part of the GTK system. It has been added to
-- support creating GtkTextBuffer objects from C++ methods.
::Method create_textbuffer private
use strict arg ptr
return .GtkTextBuffer~new(ptr)

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


/*============================================================================*/
/* Class: GtkTextBuffer                                                       */
/*============================================================================*/

::class GtkTextBuffer public

::METHOD init EXTERNAL "LIBRARY oorexxgtk2 GrxTextBufferNew"
::METHOD set_text EXTERNAL "LIBRARY oorexxgtk2 GrxTextBufferGetSetText"
::METHOD 'text=' EXTERNAL "LIBRARY oorexxgtk2 GrxTextBufferGetSetText"
::METHOD get_text EXTERNAL "LIBRARY oorexxgtk2 GrxTextBufferGetSetText"
::METHOD text EXTERNAL "LIBRARY oorexxgtk2 GrxTextBufferGetSetText"
::METHOD insert_text EXTERNAL "LIBRARY oorexxgtk2 GrxTextBufferInsertText"
::METHOD cut_clipboard EXTERNAL "LIBRARY oorexxgtk2 GrxTextBufferCutClipboard"
::METHOD copy_clipboard EXTERNAL "LIBRARY oorexxgtk2 GrxTextBufferCopyClipboard"
::METHOD paste_clipboard EXTERNAL "LIBRARY oorexxgtk2 GrxTextBufferPasteClipboard"
::METHOD create_tag_int EXTERNAL "LIBRARY oorexxgtk2 GrxTextBufferCreateTagInt"
::METHOD create_tag_float EXTERNAL "LIBRARY oorexxgtk2 GrxTextBufferCreateTagFloat"
::METHOD forward_search EXTERNAL "LIBRARY oorexxgtk2 GrxTextBufferForwardSearch"
::METHOD forward_search_next EXTERNAL "LIBRARY oorexxgtk2 GrxTextBufferForwardSearchNext"
::METHOD apply_tag_by_name EXTERNAL "LIBRARY oorexxgtk2 GrxTextBufferApplyTagByName"
::METHOD remove_all_tags EXTERNAL "LIBRARY oorexxgtk2 GrxTextBufferRemoveAllTags"
::METHOD insert_image EXTERNAL "LIBRARY oorexxgtk2 GrxTextBufferInsertImage"
::METHOD signal_connect EXTERNAL "LIBRARY oorexxgtk2 GrxTextBufferSignalConnect"

-- The following are the methods that can be connected to signals. By default
-- they do nothing.

::method signal_begin_user_action
return

::method signal_changed
return

::method signal_end_user_action
return

::method signal_modified_changed
return


/*============================================================================*/
/* Class: GtkAdjustment                                                       */
/*============================================================================*/

::class GtkAdjustment public

::method init
if arg() = 1 then do
   use strict arg ptr
   self~init_1args(ptr)
   return
   end
use strict arg value, upper, lower, step, page, pagesz
self~init_6args(value, upper, lower, step, page, pagesz)
return

::METHOD init_1args private EXTERNAL "LIBRARY oorexxgtk2 GrxAdjustmentNewFromPtr"
::METHOD init_6args private EXTERNAL "LIBRARY oorexxgtk2 GrxAdjustmentNew"

::METHOD set_value EXTERNAL "LIBRARY oorexxgtk2 GrxAdjustmentGetSetValue"
::METHOD 'value=' EXTERNAL "LIBRARY oorexxgtk2 GrxAdjustmentGetSetValue"
::METHOD get_value EXTERNAL "LIBRARY oorexxgtk2 GrxAdjustmentGetSetValue"
::METHOD value EXTERNAL "LIBRARY oorexxgtk2 GrxAdjustmentGetSetValue"
::METHOD clamp_page EXTERNAL "LIBRARY oorexxgtk2 GrxAdjustmentClampPage"
::METHOD signal_connect EXTERNAL "LIBRARY oorexxgtk2 GrxAdjustmentSignalConnect"

-- The following are the methods that can be connected to signals. By default
-- they do nothing.

::method signal_changed
return

::method signal_value_changed
return


/*============================================================================*/
/* Class: GtkListStore                                                        */
/*============================================================================*/

::class GtkListStore public

::METHOD init EXTERNAL "LIBRARY oorexxgtk2 GrxListStoreNew"
::METHOD append EXTERNAL "LIBRARY oorexxgtk2 GrxListStoreAppend"
::METHOD set_value EXTERNAL "LIBRARY oorexxgtk2 GrxListStoreSetValue"


/*============================================================================*/
/* Class: GtkMenuShell                                                        */
/* Note:  This is strictly a base class. It should never be instantiated on   */
/*        its own. You should instantiate one of its subclasses instead.      */
/*============================================================================*/

::class GtkMenuShell public subclass GtkContainer

::METHOD append EXTERNAL "LIBRARY oorexxgtk2 GrxMenuShellAppend"
::METHOD prepend EXTERNAL "LIBRARY oorexxgtk2 GrxMenuShellPrepend"
::METHOD insert EXTERNAL "LIBRARY oorexxgtk2 GrxMenuShellInsert"
::METHOD deactivate EXTERNAL "LIBRARY oorexxgtk2 GrxMenuShellDeactivate"
::METHOD select_item EXTERNAL "LIBRARY oorexxgtk2 GrxMenuShellSelectItem"
::METHOD select_first EXTERNAL "LIBRARY oorexxgtk2 GrxMenuShellSelectFirst"
::METHOD deselect EXTERNAL "LIBRARY oorexxgtk2 GrxMenuShellDeselect"
::METHOD activate_item EXTERNAL "LIBRARY oorexxgtk2 GrxMenuShellActivateItem"
::METHOD cancel EXTERNAL "LIBRARY oorexxgtk2 GrxMenuShellCancel"
::METHOD set_take_focus EXTERNAL "LIBRARY oorexxgtk2 GrxMenuShellSetTakeFocus"
::METHOD get_take_focus EXTERNAL "LIBRARY oorexxgtk2 GrxMenuShellGetTakeFocus"
::METHOD signal_connect EXTERNAL "LIBRARY oorexxgtk2 GrxMenuShellSignalConnect"

-- The following are the methods that can be connected to signals. By default
-- they do nothing.

::method signal_activate_current
return

::method signal_cancel
return

::method signal_cycle_focus
return

::method signal_deactivate
return

::method signal_move_current
return

::method signal_move_selected
return .false

::method signal_selection_done
return


/*============================================================================*/
/* Class: GtkMenu                                                             */
/*============================================================================*/

::class GtkMenu public subclass GtkMenuShell

::METHOD init EXTERNAL "LIBRARY oorexxgtk2 GrxMenuNew"
::METHOD reorder_child EXTERNAL "LIBRARY oorexxgtk2 GrxMenuReorderChild"
::METHOD attach EXTERNAL "LIBRARY oorexxgtk2 GrxMenuAttach"
::METHOD popup EXTERNAL "LIBRARY oorexxgtk2 GrxMenuPopup"
::METHOD set_accel_group EXTERNAL "LIBRARY oorexxgtk2 GrxMenuGetSetAccelGroup"
::METHOD 'accel_group=' EXTERNAL "LIBRARY oorexxgtk2 GrxMenuGetSetAccelGroup"
::METHOD get_accel_group EXTERNAL "LIBRARY oorexxgtk2 GrxMenuGetSetAccelGroup"
::METHOD accel_group EXTERNAL "LIBRARY oorexxgtk2 GrxMenuGetSetAccelGroup"
::METHOD set_accel_path EXTERNAL "LIBRARY oorexxgtk2 GrxMenuSetAccelPath"
::METHOD 'accel_path=' EXTERNAL "LIBRARY oorexxgtk2 GrxMenuSetAccelPath"
::METHOD set_title EXTERNAL "LIBRARY oorexxgtk2 GrxMenuGetSetTitle"
::METHOD 'title=' EXTERNAL "LIBRARY oorexxgtk2 GrxMenuGetSetTitle"
::METHOD get_title EXTERNAL "LIBRARY oorexxgtk2 GrxMenuGetSetTitle"
::METHOD title EXTERNAL "LIBRARY oorexxgtk2 GrxMenuGetSetTitle"
::METHOD set_tearoff_state EXTERNAL "LIBRARY oorexxgtk2 GrxMenuSetTearoffState"
::METHOD 'tearoff_state=' EXTERNAL "LIBRARY oorexxgtk2 GrxMenuSetTearoffState"
::METHOD get_tearoff_state EXTERNAL "LIBRARY oorexxgtk2 GrxMenuGetTearoffState"
::METHOD tearoff_state EXTERNAL "LIBRARY oorexxgtk2 GrxMenuGetTearoffState"
::METHOD popdown EXTERNAL "LIBRARY oorexxgtk2 GrxMenuPopdown"
::METHOD reposition EXTERNAL "LIBRARY oorexxgtk2 GrxMenuReposition"
::METHOD set_active EXTERNAL "LIBRARY oorexxgtk2 GrxMenuGetSetActive"
::METHOD 'active=' EXTERNAL "LIBRARY oorexxgtk2 GrxMenuGetSetActive"
::METHOD get_active EXTERNAL "LIBRARY oorexxgtk2 GrxMenuGetSetActive"
::METHOD active EXTERNAL "LIBRARY oorexxgtk2 GrxMenuGetSetActive"
::METHOD attach_to_widget EXTERNAL "LIBRARY oorexxgtk2 GrxMenuAttachToWidget"
::METHOD detach EXTERNAL "LIBRARY oorexxgtk2 GrxMenuDetach"
::METHOD get_attach_widget EXTERNAL "LIBRARY oorexxgtk2 GrxMenuGetAttachWidget"
::METHOD signal_connect EXTERNAL "LIBRARY oorexxgtk2 GrxMenuSignalConnect"

-- The following are the methods that can be connected to signals. By default
-- they do nothing.

::method signal_move_scroll
return


/*============================================================================*/
/* Class: GtkMenuItem                                                         */
/*============================================================================*/

::class GtkMenuItem public subclass GtkContainer

::method newWithLabel class
use strict arg label
return self~new(label, .false)

::method newWithMnemonic class
use strict arg label
return self~new(label, .true)

::METHOD init EXTERNAL "LIBRARY oorexxgtk2 GrxMenuItemNew"
::METHOD set_right_justified EXTERNAL "LIBRARY oorexxgtk2 GrxMenuItemGetSetRightJustified"
::METHOD 'right_justified=' EXTERNAL "LIBRARY oorexxgtk2 GrxMenuItemGetSetRightJustified"
::METHOD get_right_justified EXTERNAL "LIBRARY oorexxgtk2 GrxMenuItemGetSetRightJustified"
::METHOD right_justified EXTERNAL "LIBRARY oorexxgtk2 GrxMenuItemGetSetRightJustified"
::METHOD set_submenu EXTERNAL "LIBRARY oorexxgtk2 GrxMenuItemSetSubmenu"
::METHOD remove_submenu EXTERNAL "LIBRARY oorexxgtk2 GrxMenuItemRemoveSubmenu"
::METHOD set_accel_path EXTERNAL "LIBRARY oorexxgtk2 GrxMenuItemSetAccelPath"
::METHOD 'accel_path=' EXTERNAL "LIBRARY oorexxgtk2 GrxMenuItemSetAccelPath"
::METHOD select EXTERNAL "LIBRARY oorexxgtk2 GrxMenuItemSelect"
::METHOD deselect EXTERNAL "LIBRARY oorexxgtk2 GrxMenuItemDeselect"
::METHOD signal_connect EXTERNAL "LIBRARY oorexxgtk2 GrxMenuItemSignalConnect"

-- The following are the methods that can be connected to signals. By default
-- they do nothing.

::method signal_activate
return

::method signal_activate_item
return

::method signal_toggle_size_allocate
return

::method signal_toggle_size_request
return


/*============================================================================*/
/* Class: GtkImageMenuItem                                                    */
/*============================================================================*/

::class GtkImageMenuItem public subclass GtkMenuItem

::method newFromStock class
use strict arg label, accelgrp
return .GtkImageMenuItem~new(label, 2, accelgrp)

::method newWithMnemonic class
use strict arg label
return .GtkImageMenuItem~new(label, 1)

::METHOD init EXTERNAL "LIBRARY oorexxgtk2 GrxImageMenuItemNew"


/*============================================================================*/
/* Class: GtkMenuBar                                                          */
/*============================================================================*/

::class GtkMenuBar public subclass GtkMenuShell

::METHOD init EXTERNAL "LIBRARY oorexxgtk2 GrxMenuBarNew"
::METHOD append EXTERNAL "LIBRARY oorexxgtk2 GrxMenuBarAppend"
::METHOD prepend EXTERNAL "LIBRARY oorexxgtk2 GrxMenuBarPrepend"
::METHOD insert EXTERNAL "LIBRARY oorexxgtk2 GrxMenuBarInsert"
::METHOD set_pack_direction EXTERNAL "LIBRARY oorexxgtk2 GrxMenuBarGetSetPackDirection"
::METHOD 'pack_direction=' EXTERNAL "LIBRARY oorexxgtk2 GrxMenuBarGetSetPackDirection"
::METHOD get_pack_direction EXTERNAL "LIBRARY oorexxgtk2 GrxMenuBarGetSetPackDirection"
::METHOD pack_direction EXTERNAL "LIBRARY oorexxgtk2 GrxMenuBarGetSetPackDirection"
::METHOD set_child_pack_direction EXTERNAL "LIBRARY oorexxgtk2 GrxMenuBarGetSetChildPackDirection"
::METHOD 'child_pack_direction=' EXTERNAL "LIBRARY oorexxgtk2 GrxMenuBarGetSetChildPackDirection"
::METHOD get_child_pack_direction EXTERNAL "LIBRARY oorexxgtk2 GrxMenuBarGetSetChildPackDirection"
::METHOD child_pack_direction EXTERNAL "LIBRARY oorexxgtk2 GrxMenuBarGetSetChildPackDirection"


/*============================================================================*/
/* Class: GtkToolbar                                                          */
/*============================================================================*/

::class GtkToolbar public subclass GtkContainer

::METHOD init EXTERNAL "LIBRARY oorexxgtk2 GrxToolbarNew"
::METHOD insert EXTERNAL "LIBRARY oorexxgtk2 GrxToolbarInsert"
::METHOD get_item_index EXTERNAL "LIBRARY oorexxgtk2 GrxToolbarGetItemIndex"
::METHOD item_index EXTERNAL "LIBRARY oorexxgtk2 GrxToolbarGetItemIndex"
::METHOD get_n_items EXTERNAL "LIBRARY oorexxgtk2 GrxToolbarGetNItems"
::METHOD n_items EXTERNAL "LIBRARY oorexxgtk2 GrxToolbarGetNItems"
::METHOD get_nth_item EXTERNAL "LIBRARY oorexxgtk2 GrxToolbarGetNthItem"
::METHOD nth_item EXTERNAL "LIBRARY oorexxgtk2 GrxToolbarGetNthItem"
::METHOD get_drop_index EXTERNAL "LIBRARY oorexxgtk2 GrxToolbarGetDropIndex"
::METHOD drop_index EXTERNAL "LIBRARY oorexxgtk2 GrxToolbarGetDropIndex"
::METHOD set_drop_highlight_item EXTERNAL "LIBRARY oorexxgtk2 GrxToolbarSetDropHighlightItem"
::METHOD set_show_arrow EXTERNAL "LIBRARY oorexxgtk2 GrxToolbarGetSetShowArrow"
::METHOD 'show_arrow=' EXTERNAL "LIBRARY oorexxgtk2 GrxToolbarGetSetShowArrow"
::METHOD get_show_arrow EXTERNAL "LIBRARY oorexxgtk2 GrxToolbarGetSetShowArrow"
::METHOD show_arrow EXTERNAL "LIBRARY oorexxgtk2 GrxToolbarGetSetShowArrow"
::METHOD set_orientation EXTERNAL "LIBRARY oorexxgtk2 GrxToolbarGetSetOrientation"
::METHOD 'orientation=' EXTERNAL "LIBRARY oorexxgtk2 GrxToolbarGetSetOrientation"
::METHOD get_orientation EXTERNAL "LIBRARY oorexxgtk2 GrxToolbarGetSetOrientation"
::METHOD orientation EXTERNAL "LIBRARY oorexxgtk2 GrxToolbarGetSetOrientation"
::METHOD set_tooltips EXTERNAL "LIBRARY oorexxgtk2 GrxToolbarSetTooltips"
::METHOD set_icon_size EXTERNAL "LIBRARY oorexxgtk2 GrxToolbarGetSetIconSize"
::METHOD 'icon_size=' EXTERNAL "LIBRARY oorexxgtk2 GrxToolbarGetSetIconSize"
::METHOD get_icon_size EXTERNAL "LIBRARY oorexxgtk2 GrxToolbarGetSetIconSize"
::METHOD icon_size EXTERNAL "LIBRARY oorexxgtk2 GrxToolbarGetSetIconSize"
::METHOD unset_icon_size EXTERNAL "LIBRARY oorexxgtk2 GrxToolbarUnsetIconSize"
::METHOD get_style EXTERNAL "LIBRARY oorexxgtk2 GrxToolbarGetSetStyle"
::METHOD style EXTERNAL "LIBRARY oorexxgtk2 GrxToolbarGetSetStyle"
::METHOD set_style EXTERNAL "LIBRARY oorexxgtk2 GrxToolbarGetSetStyle"
::METHOD 'style=' EXTERNAL "LIBRARY oorexxgtk2 GrxToolbarGetSetStyle"
::METHOD get_relief_style EXTERNAL "LIBRARY oorexxgtk2 GrxToolbarGetReliefStyle"
::METHOD relief_style EXTERNAL "LIBRARY oorexxgtk2 GrxToolbarGetReliefStyle"
::METHOD get_tooltips EXTERNAL "LIBRARY oorexxgtk2 GrxToolbarGetTooltips"
::METHOD tooltips EXTERNAL "LIBRARY oorexxgtk2 GrxToolbarGetTooltips"
::METHOD append_item EXTERNAL "LIBRARY oorexxgtk2 GrxToolbarAppendItem"
::METHOD prepend_item EXTERNAL "LIBRARY oorexxgtk2 GrxToolbarPrependItem"
::METHOD insert_item EXTERNAL "LIBRARY oorexxgtk2 GrxToolbarInsertItem"
::METHOD append_space EXTERNAL "LIBRARY oorexxgtk2 GrxToolbarAppendSpace"
::METHOD prepend_space EXTERNAL "LIBRARY oorexxgtk2 GrxToolbarPrependSpace"
::METHOD insert_space EXTERNAL "LIBRARY oorexxgtk2 GrxToolbarInsertSpace"
::METHOD append_element EXTERNAL "LIBRARY oorexxgtk2 GrxToolbarAppendElement"
::METHOD prepend_element EXTERNAL "LIBRARY oorexxgtk2 GrxToolbarPrependElement"
::METHOD insert_element EXTERNAL "LIBRARY oorexxgtk2 GrxToolbarInsertElement"
::METHOD append_widget EXTERNAL "LIBRARY oorexxgtk2 GrxToolbarAppendWidget"
::METHOD prepend_widget EXTERNAL "LIBRARY oorexxgtk2 GrxToolbarPrependWidget"
::METHOD insert_widget EXTERNAL "LIBRARY oorexxgtk2 GrxToolbarInsertWidget"
::METHOD insert_stock EXTERNAL "LIBRARY oorexxgtk2 GrxToolbarInsertStock"
::METHOD remove_space EXTERNAL "LIBRARY oorexxgtk2 GrxToolbarRemoveSpace"
::METHOD unset_style EXTERNAL "LIBRARY oorexxgtk2 GrxToolbarUnsetStyle"
::METHOD signal_connect EXTERNAL "LIBRARY oorexxgtk2 GrxToolbarSignalConnect"

-- The following are the methods that can be connected to signals. By default
-- they do nothing.

::method signal_focus_home_or_end
return .false

::method signal_orientation_changed
return

::method signal_popup_context_menu
return .false

::method signal_style_changed
return


/*============================================================================*/
/* Class: GtkToolItem                                                         */
/*============================================================================*/

::class GtkToolItem public subclass GtkContainer

::method newFromStock class
use strict arg sid
return .GtkToolItem~new(sid)

::METHOD init EXTERNAL "LIBRARY oorexxgtk2 GrxToolItemNew"
::METHOD set_homogeneous EXTERNAL "LIBRARY oorexxgtk2 GrxToolItemGetSetHomogeneous"
::METHOD 'homogeneous=' EXTERNAL "LIBRARY oorexxgtk2 GrxToolItemGetSetHomogeneous"
::METHOD get_homogeneous EXTERNAL "LIBRARY oorexxgtk2 GrxToolItemGetSetHomogeneous"
::METHOD homogeneous EXTERNAL "LIBRARY oorexxgtk2 GrxToolItemGetSetHomogeneous"
::METHOD set_expand EXTERNAL "LIBRARY oorexxgtk2 GrxToolItemGetSetExpand"
::METHOD 'expand=' EXTERNAL "LIBRARY oorexxgtk2 GrxToolItemGetSetExpand"
::METHOD get_expand EXTERNAL "LIBRARY oorexxgtk2 GrxToolItemGetSetExpand"
::METHOD expand EXTERNAL "LIBRARY oorexxgtk2 GrxToolItemGetSetExpand"
::METHOD set_tooltip EXTERNAL "LIBRARY oorexxgtk2 GrxToolItemSetTooltip"
::METHOD set_use_drag_window EXTERNAL "LIBRARY oorexxgtk2 GrxToolItemGetSetUseDragWindow"
::METHOD 'use_drag_window=' EXTERNAL "LIBRARY oorexxgtk2 GrxToolItemGetSetUseDragWindow"
::METHOD get_use_drag_window EXTERNAL "LIBRARY oorexxgtk2 GrxToolItemGetSetUseDragWindow"
::METHOD use_drag_window EXTERNAL "LIBRARY oorexxgtk2 GrxToolItemGetSetUseDragWindow"
::METHOD set_visible_horizontal EXTERNAL "LIBRARY oorexxgtk2 GrxToolItemGetSetVisibleHorizontal"
::METHOD 'visible_horizontal=' EXTERNAL "LIBRARY oorexxgtk2 GrxToolItemGetSetVisibleHorizontal"
::METHOD get_visible_horizontal EXTERNAL "LIBRARY oorexxgtk2 GrxToolItemGetSetVisibleHorizontal"
::METHOD visible_horizontal EXTERNAL "LIBRARY oorexxgtk2 GrxToolItemGetSetVisibleHorizontal"
::METHOD set_visible_vertical EXTERNAL "LIBRARY oorexxgtk2 GrxToolItemGetSetVisibleVertical"
::METHOD 'visible_vertical=' EXTERNAL "LIBRARY oorexxgtk2 GrxToolItemGetSetVisibleVertical"
::METHOD get_visible_vertical EXTERNAL "LIBRARY oorexxgtk2 GrxToolItemGetSetVisibleVertical"
::METHOD visible_vertical EXTERNAL "LIBRARY oorexxgtk2 GrxToolItemGetSetVisibleVertical"
::METHOD set_is_important EXTERNAL "LIBRARY oorexxgtk2 GrxToolItemGetSetIsImportant"
::METHOD 'is_important=' EXTERNAL "LIBRARY oorexxgtk2 GrxToolItemGetSetIsImportant"
::METHOD get_is_important EXTERNAL "LIBRARY oorexxgtk2 GrxToolItemGetSetIsImportant"
::METHOD is_important EXTERNAL "LIBRARY oorexxgtk2 GrxToolItemGetSetIsImportant"
::METHOD get_icon_size EXTERNAL "LIBRARY oorexxgtk2 GrxToolItemGetIconSize"
::METHOD icon_size EXTERNAL "LIBRARY oorexxgtk2 GrxToolItemGetIconSize"
::METHOD get_orientation EXTERNAL "LIBRARY oorexxgtk2 GrxToolItemGetOrientation"
::METHOD orientation EXTERNAL "LIBRARY oorexxgtk2 GrxToolItemGetOrientation"
::METHOD get_toolbar_style EXTERNAL "LIBRARY oorexxgtk2 GrxToolItemGetToolbarStyle"
::METHOD toolbar_style EXTERNAL "LIBRARY oorexxgtk2 GrxToolItemGetToolbarStyle"
::METHOD get_relief_style EXTERNAL "LIBRARY oorexxgtk2 GrxToolItemGetReliefStyle"
::METHOD relief_style EXTERNAL "LIBRARY oorexxgtk2 GrxToolItemGetReliefStyle"
::METHOD retrieve_proxy_menu_item EXTERNAL "LIBRARY oorexxgtk2 GrxToolItemRetrieveProxyMenuItem"
::METHOD get_proxy_menu_item EXTERNAL "LIBRARY oorexxgtk2 GrxToolItemGetProxyMenuItem"
::METHOD proxy_menu_item EXTERNAL "LIBRARY oorexxgtk2 GrxToolItemGetProxyMenuItem"
::METHOD set_proxy_menu_item EXTERNAL "LIBRARY oorexxgtk2 GrxToolItemSetProxyMenuItem"
::METHOD rebuild_menu EXTERNAL "LIBRARY oorexxgtk2 GrxToolItemRebuildMenu"
::METHOD signal_connect EXTERNAL "LIBRARY oorexxgtk2 GrxToolItemSignalConnect"

-- The following are the methods that can be connected to signals. By default
-- they do nothing.

::method signal_create_menu_proxy
return .false

::method signal_set_tooltip
return .false

::method signal_toolbar_reconfigured
return


/*============================================================================*/
/* Class: GtkSeparatorMenuItem                                                     */
/*============================================================================*/

::class GtkSeparatorMenuItem public subclass GtkMenuItem

::METHOD init EXTERNAL "LIBRARY oorexxgtk2 GrxSeparatorMenuItemNew"


/*============================================================================*/
/* Class: GtkToolButton                                                       */
/*============================================================================*/

::class GtkToolButton public subclass GtkToolItem

::method newFromStock class
use strict arg sid
return .GtkToolButton~new(sid)

::METHOD init
if arg() = 1 then do
   use strict arg sid
   self~init_1args(sid)
   return
   end
use strict arg icon, label
self~init_1args(icon, label)
return

::METHOD init_2args private EXTERNAL "LIBRARY oorexxgtk2 GrxToolButtonNew"
::METHOD init_1args private EXTERNAL "LIBRARY oorexxgtk2 GrxToolButtonNewFromStock"

::METHOD get_label EXTERNAL "LIBRARY oorexxgtk2 GrxToolButtonGetSetLabel"
::METHOD label EXTERNAL "LIBRARY oorexxgtk2 GrxToolButtonGetSetLabel"
::METHOD set_label EXTERNAL "LIBRARY oorexxgtk2 GrxToolButtonGetSetLabel"
::METHOD 'label=' EXTERNAL "LIBRARY oorexxgtk2 GrxToolButtonGetSetLabel"
::METHOD set_label_widget EXTERNAL "LIBRARY oorexxgtk2 GrxToolButtonGetSetLabelWidget"
::METHOD 'label_widget=' EXTERNAL "LIBRARY oorexxgtk2 GrxToolButtonGetSetLabelWidget"
::METHOD get_use_underline EXTERNAL "LIBRARY oorexxgtk2 GrxToolButtonGetUseUnderline"
::METHOD use_underline EXTERNAL "LIBRARY oorexxgtk2 GrxToolButtonGetUseUnderline"
::METHOD set_use_underline EXTERNAL "LIBRARY oorexxgtk2 GrxToolButtonSetUseUnderline"
::METHOD 'use_underline=' EXTERNAL "LIBRARY oorexxgtk2 GrxToolButtonSetUseUnderline"
::METHOD get_stock_id EXTERNAL "LIBRARY oorexxgtk2 GrxToolButtonGetSetStockId"
::METHOD stock_id EXTERNAL "LIBRARY oorexxgtk2 GrxToolButtonGetSetStockId"
::METHOD set_stock_id EXTERNAL "LIBRARY oorexxgtk2 GrxToolButtonGetSetStockId"
::METHOD 'stock_id=' EXTERNAL "LIBRARY oorexxgtk2 GrxToolButtonGetSetStockId"
::METHOD get_icon_name EXTERNAL "LIBRARY oorexxgtk2 GrxToolButtonGetSetIconName"
::METHOD icon_name EXTERNAL "LIBRARY oorexxgtk2 GrxToolButtonGetSetIconName"
::METHOD set_icon_name EXTERNAL "LIBRARY oorexxgtk2 GrxToolButtonGetSetIconName"
::METHOD 'icon_name=' EXTERNAL "LIBRARY oorexxgtk2 GrxToolButtonGetSetIconName"
::METHOD get_icon_widget EXTERNAL "LIBRARY oorexxgtk2 GrxToolButtonGetSetIconWidget"
::METHOD icon_widget EXTERNAL "LIBRARY oorexxgtk2 GrxToolButtonGetSetIconWidget"
::METHOD set_icon_widget EXTERNAL "LIBRARY oorexxgtk2 GrxToolButtonGetSetIconWidget"
::METHOD 'icon_widget=' EXTERNAL "LIBRARY oorexxgtk2 GrxToolButtonGetSetIconWidget"
::METHOD get_label_widget EXTERNAL "LIBRARY oorexxgtk2 GrxToolButtonGetSetLabelWidget"
::METHOD label_widget EXTERNAL "LIBRARY oorexxgtk2 GrxToolButtonGetSetLabelWidget"
::METHOD signal_connect EXTERNAL "LIBRARY oorexxgtk2 GrxToolButtonSignalConnect"

-- The following are the methods that can be connected to signals. By default
-- they do nothing.

::method signal_clicked
return


/*============================================================================*/
/* Class: GtkMenuToolButton                                                   */
/*============================================================================*/

::class GtkMenuToolButton public subclass GtkToolButton

::METHOD new_from_stock class
use strict arg id
obj = .GtkMenuToolButton~new(.nil, .nil)
obj~set_stock_id(id)
return obj

::METHOD init EXTERNAL "LIBRARY oorexxgtk2 GrxMenuToolButtonNew"
::METHOD get_menu EXTERNAL "LIBRARY oorexxgtk2 GrxMenuToolButtonGetSetMenu"
::METHOD menu EXTERNAL "LIBRARY oorexxgtk2 GrxMenuToolButtonGetSetMenu"
::METHOD set_menu EXTERNAL "LIBRARY oorexxgtk2 GrxMenuToolButtonGetSetMenu"
::METHOD 'menu=' EXTERNAL "LIBRARY oorexxgtk2 GrxMenuToolButtonGetSetMenu"
::METHOD signal_connect EXTERNAL "LIBRARY oorexxgtk2 GrxMenuToolButtonSignalConnect"

-- The following are the methods that can be connected to signals. By default
-- they do nothing.

::method signal_show_menu
return


/*============================================================================*/
/* Class: GtkSeparatorToolItem                                                */
/*============================================================================*/

::class GtkSeparatorToolItem public subclass GtkToolItem

::METHOD init EXTERNAL "LIBRARY oorexxgtk2 GrxSeparatorToolItemNew"
::METHOD set_draw EXTERNAL "LIBRARY oorexxgtk2 GrxSeparatorToolItemGetSetDraw"
::METHOD 'draw=' EXTERNAL "LIBRARY oorexxgtk2 GrxSeparatorToolItemGetSetDraw"
::METHOD get_draw EXTERNAL "LIBRARY oorexxgtk2 GrxSeparatorToolItemGetSetDraw"
::METHOD draw EXTERNAL "LIBRARY oorexxgtk2 GrxSeparatorToolItemGetSetDraw"


/*============================================================================*/
/* Class: GtkAccelGroup                                                       */
/*============================================================================*/

::class GtkAccelGroup public

::METHOD init EXTERNAL "LIBRARY oorexxgtk2 GrxAccelGroupNew"
::METHOD signal_connect EXTERNAL "LIBRARY oorexxgtk2 GrxAccelGroupSignalConnect"

-- The following are the methods that can be connected to signals. By default
-- they do nothing.

::method signal_accel_activate
return .false

::method signal_accel_changed
return


/*============================================================================*/
/* Class: GtkStatusbar                                                        */
/*============================================================================*/

::class GtkStatusbar public subclass GtkHBox

::METHOD init EXTERNAL "LIBRARY oorexxgtk2 GrxStatusbarNew"
::METHOD get_context_id EXTERNAL "LIBRARY oorexxgtk2 GrxStatusbarGetContextId"
::METHOD push EXTERNAL "LIBRARY oorexxgtk2 GrxStatusbarPush"
::METHOD pop EXTERNAL "LIBRARY oorexxgtk2 GrxStatusbarPop"
::METHOD remove EXTERNAL "LIBRARY oorexxgtk2 GrxStatusbarRemove"
::METHOD set_has_resize_grip EXTERNAL "LIBRARY oorexxgtk2 GrxStatusbarGetSetHasResizeGrip"
::METHOD 'has_resize_grip=' EXTERNAL "LIBRARY oorexxgtk2 GrxStatusbarGetSetHasResizeGrip"
::METHOD get_has_resize_grip EXTERNAL "LIBRARY oorexxgtk2 GrxStatusbarGetSetHasResizeGrip"
::METHOD has_resize_grip EXTERNAL "LIBRARY oorexxgtk2 GrxStatusbarGetSetHasResizeGrip"
::METHOD signal_connect EXTERNAL "LIBRARY oorexxgtk2 GrxStatusbarSignalConnect"

-- The following are the methods that can be connected to signals. By default
-- they do nothing.

::method signal_text_popped
return

::method signal_text_pushed
return


/*============================================================================*/
/* Class: GtkCellRenderer                                                     */
/* Note:  This is strictly a base class. It should never be instantiated on   */
/*        its own. You should instantiate one of its subclasses instead.      */
/*============================================================================*/

::class GtkCellRenderer public

::METHOD set_fixed_size EXTERNAL "LIBRARY oorexxgtk2 GrxCellRendererSetFixedSize"
::METHOD signal_connect EXTERNAL "LIBRARY oorexxgtk2 GrxCellRendererSignalConnect"

-- The following are the methods that can be connected to signals. By default
-- they do nothing.

::method signal_editing_canceled
return

::method signal_editing_started
return


/*============================================================================*/
/* Class: GtkCellRendererText                                                 */
/*============================================================================*/

::class GtkCellRendererText public subclass GtkCellRenderer

::METHOD init EXTERNAL "LIBRARY oorexxgtk2 GrxCellRendererTextNew"
::METHOD signal_connect EXTERNAL "LIBRARY oorexxgtk2 GrxCellRendererTextSignalConnect"

-- The following are the methods that can be connected to signals. By default
-- they do nothing.

::method signal_edited
return


/*============================================================================*/
/* Class: GtkCellRendererPixbuf                                               */
/*============================================================================*/

::class GtkCellRendererPixbuf public subclass GtkCellRenderer

::METHOD init EXTERNAL "LIBRARY oorexxgtk2 GrxCellRendererPixbufNew"


/*============================================================================*/
/* Class: GtkCellRendererProgress                                             */
/*============================================================================*/

::class GtkCellRendererProgress public subclass GtkCellRenderer

::METHOD init EXTERNAL "LIBRARY oorexxgtk2 GrxCellRendererProgressNew"


/*============================================================================*/
/* Class: GtkCellRendererToggle                                               */
/*============================================================================*/

::class GtkCellRendererToggle public subclass GtkCellRenderer

::METHOD init EXTERNAL "LIBRARY oorexxgtk2 GrxCellRendererToggleNew"
::METHOD set_radio EXTERNAL "LIBRARY oorexxgtk2 GrxCellRendererToggleGetSetRadio"
::METHOD 'radio=' EXTERNAL "LIBRARY oorexxgtk2 GrxCellRendererToggleGetSetRadio"
::METHOD get_radio EXTERNAL "LIBRARY oorexxgtk2 GrxCellRendererToggleGetSetRadio"
::METHOD radio EXTERNAL "LIBRARY oorexxgtk2 GrxCellRendererToggleGetSetRadio"
::METHOD set_active EXTERNAL "LIBRARY oorexxgtk2 GrxCellRendererToggleGetSetActive"
::METHOD 'active=' EXTERNAL "LIBRARY oorexxgtk2 GrxCellRendererToggleGetSetActive"
::METHOD get_active EXTERNAL "LIBRARY oorexxgtk2 GrxCellRendererToggleGetSetActive"
::METHOD active EXTERNAL "LIBRARY oorexxgtk2 GrxCellRendererToggleGetSetActive"
::METHOD signal_connect EXTERNAL "LIBRARY oorexxgtk2 GrxCellRendererToggleSignalConnect"

-- The following are the methods that can be connected to signals. By default
-- they do nothing.

::method signal_toggled
return


/*============================================================================*/
/* Class: GtkCellRendererAccel                                                */
/*============================================================================*/

::class GtkCellRendererAccel public subclass GtkCellRendererText

::METHOD init EXTERNAL "LIBRARY oorexxgtk2 GrxCellRendererAccelNew"
::METHOD signal_connect EXTERNAL "LIBRARY oorexxgtk2 GrxCellRendererAccelSignalConnect"

-- The following are the methods that can be connected to signals. By default
-- they do nothing.

::method signal_accel_cleared
return

::method signal_accel_edited
return


/*============================================================================*/
/* Class: GtkCellRendererCombo                                                */
/*============================================================================*/

::class GtkCellRendererCombo public subclass GtkCellRendererText

::METHOD init EXTERNAL "LIBRARY oorexxgtk2 GrxCellRendererComboNew"
::METHOD signal_connect EXTERNAL "LIBRARY oorexxgtk2 GrxCellRendererComboSignalConnect"

-- The following are the methods that can be connected to signals. By default
-- they do nothing.

::method signal_accel_changed
return


/*============================================================================*/
/* Class: GtkTreeView                                                         */
/*============================================================================*/

::class GtkTreeView public subclass GtkContainer

::METHOD init EXTERNAL "LIBRARY oorexxgtk2 GrxTreeViewNew"
::METHOD append_column EXTERNAL "LIBRARY oorexxgtk2 GrxTreeViewAppendColumn"
::METHOD insert_column EXTERNAL "LIBRARY oorexxgtk2 GrxTreeViewInsertColumn"
::METHOD remove_column EXTERNAL "LIBRARY oorexxgtk2 GrxTreeViewRemoveColumn"
::METHOD set_model EXTERNAL "LIBRARY oorexxgtk2 GrxTreeViewGetSetModel"
::METHOD 'model=' EXTERNAL "LIBRARY oorexxgtk2 GrxTreeViewGetSetModel"
::METHOD get_model EXTERNAL "LIBRARY oorexxgtk2 GrxTreeViewGetSetModel"
::METHOD model EXTERNAL "LIBRARY oorexxgtk2 GrxTreeViewGetSetModel"
::METHOD get_selection EXTERNAL "LIBRARY oorexxgtk2 GrxTreeViewGetSelection"
::METHOD expand_all EXTERNAL "LIBRARY oorexxgtk2 GrxTreeViewExpandAll"
::METHOD signal_connect EXTERNAL "LIBRARY oorexxgtk2 GrxTreeViewSignalConnect"

::method create_tree_model private
use strict arg ptr
return .GtkTreeModel~new(ptr)

::method create_tree_selection private
use strict arg ptr
return .GtkTreeSelection~new(ptr)

-- The following are the methods that can be connected to signals. By default
-- they do nothing.

::method signal_columns_changed
return

::method signal_cursor_changed
return

::method signal_expand_collapse_cursor_row
return .false

::method signal_move_cursor
return .false

::method signal_row_activated
return

::method signal_row_collapsed
return

::method signal_row_expanded
return

::method signal_select_all
return .false

::method signal_select_cursor_parent
return .false

::method signal_select_cursor_row
return .false

::method signal_set_scroll_adjustments
return

::method signal_start_interactive_search
return .false

::method signal_test_collapse_row
return .false

::method signal_test_expand_row
return .false

::method signal_toggle_cursor_row
return .false

::method signal_unselect_all
return .false


/*============================================================================*/
/* Class: GtkTreeRowReference                                                 */
/*============================================================================*/

::class GtkTreeRowReference public

::METHOD init EXTERNAL "LIBRARY oorexxgtk2 GrxTreeRowReferenceNew"
::METHOD valid EXTERNAL "LIBRARY oorexxgtk2 GrxTreeRowReferenceValid"
::METHOD get_path EXTERNAL "LIBRARY oorexxgtk2 GrxTreeRowReferenceGetPath"

::method create_tree_path private
use strict arg ptr
return .GtkTreePath~new(ptr)


/*============================================================================*/
/* Class: GtkTreeViewColumn                                                   */
/*============================================================================*/

::class GtkTreeViewColumn public

::METHOD newWithAttributes class
use arg title, renderer
obj = .GtkTreeViewColumn~new()
obj~set_title(title)
obj~pack_start(renderer, .false)
do i = 3 to arg() by 2
   obj~set_attribute(renderer, arg(i), arg(i + 1))
   end
return obj

::METHOD init EXTERNAL "LIBRARY oorexxgtk2 GrxTreeViewColumnNew"
::METHOD set_title EXTERNAL "LIBRARY oorexxgtk2 GrxTreeViewColumnGetSetTitle"
::METHOD 'title=' EXTERNAL "LIBRARY oorexxgtk2 GrxTreeViewColumnGetSetTitle"
::METHOD get_title EXTERNAL "LIBRARY oorexxgtk2 GrxTreeViewColumnGetSetTitle"
::METHOD title EXTERNAL "LIBRARY oorexxgtk2 GrxTreeViewColumnGetSetTitle"
::METHOD pack_start EXTERNAL "LIBRARY oorexxgtk2 GrxTreeViewColumnPackStart"
::METHOD pack_end EXTERNAL "LIBRARY oorexxgtk2 GrxTreeViewColumnPackEnd"
::METHOD set_attribute EXTERNAL "LIBRARY oorexxgtk2 GrxTreeViewColumnSetAttribute"
::METHOD signal_connect EXTERNAL "LIBRARY oorexxgtk2 GrxTreeViewColumnSignalConnect"

-- The following are the methods that can be connected to signals. By default
-- they do nothing.

::method signal_clicked
return


/*============================================================================*/
/* Class: GtkTreeStore                                                        */
/*============================================================================*/

::class GtkTreeStore public

::METHOD init EXTERNAL "LIBRARY oorexxgtk2 GrxTreeStoreNew"
::METHOD append EXTERNAL "LIBRARY oorexxgtk2 GrxTreeStoreAppend"
::METHOD set_value EXTERNAL "LIBRARY oorexxgtk2 GrxTreeStoreSetValue"
::METHOD remove EXTERNAL "LIBRARY oorexxgtk2 GrxTreeStoreRemove"


/*============================================================================*/
/* Class: GtkTreeSelection                                                    */
/*============================================================================*/

::class GtkTreeSelection public

::METHOD init EXTERNAL "LIBRARY oorexxgtk2 GrxTreeSelectionNewFromPtr"
::METHOD get_selected_rows EXTERNAL "LIBRARY oorexxgtk2 GrxTreeSelectionGetSelectedRows"
::METHOD set_mode EXTERNAL "LIBRARY oorexxgtk2 GrxTreeSelectionSetMode"
::METHOD 'mode=' EXTERNAL "LIBRARY oorexxgtk2 GrxTreeSelectionSetMode"
::METHOD signal_connect EXTERNAL "LIBRARY oorexxgtk2 GrxTreeSelectionSignalConnect"

::method create_tree_row_reference private
use strict arg ptr
return .GtkTreeRowReference~new(ptr)

-- The following are the methods that can be connected to signals. By default
-- they do nothing.

::method signal_changed
return


/*============================================================================*/
/* Class: GtkCalendar                                                         */
/*============================================================================*/

::class GtkCalendar public subclass GtkWidget

::METHOD init EXTERNAL "LIBRARY oorexxgtk2 GrxCalendarNew"
::METHOD select_month EXTERNAL "LIBRARY oorexxgtk2 GrxCalendarSelectMonth"
::METHOD select_day EXTERNAL "LIBRARY oorexxgtk2 GrxCalendarSelectDay"
::METHOD mark_day EXTERNAL "LIBRARY oorexxgtk2 GrxCalendarMarkDay"
::METHOD unmark_day EXTERNAL "LIBRARY oorexxgtk2 GrxCalendarUnmarkDay"
::METHOD clear_marks EXTERNAL "LIBRARY oorexxgtk2 GrxCalendarClearMarks"
::METHOD get_display_options EXTERNAL "LIBRARY oorexxgtk2 GrxCalendarGetDisplayOptions"
::METHOD set_display_options EXTERNAL "LIBRARY oorexxgtk2 GrxCalendarSetDisplayOptions"
::METHOD get_date EXTERNAL "LIBRARY oorexxgtk2 GrxCalendarGetDate"
::METHOD signal_connect EXTERNAL "LIBRARY oorexxgtk2 GrxCalendarSignalConnect"

-- The following are the methods that can be connected to signals. By default
-- they do nothing.

::method signal_day_selected
return

::method signal_day_selected_double_click
return

::method signal_month_changed
return

::method signal_next_month
return

::method signal_next_year
return

::method signal_prev_month
return

::method signal_prev_year
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

::METHOD init EXTERNAL "LIBRARY oorexxgtk2 GrxGSListNew"
::METHOD uninit EXTERNAL "LIBRARY oorexxgtk2 GrxGSListUninit"
::METHOD append EXTERNAL "LIBRARY oorexxgtk2 GrxGSListAppend"
::METHOD head EXTERNAL "LIBRARY oorexxgtk2 GrxGSListFirst"
::METHOD item EXTERNAL "LIBRARY oorexxgtk2 GrxGSListItem"
::METHOD next EXTERNAL "LIBRARY oorexxgtk2 GrxGSListNext"
::METHOD insert EXTERNAL "LIBRARY oorexxgtk2 GrxGSListInsert"
::METHOD prepend EXTERNAL "LIBRARY oorexxgtk2 GrxGSListPrepend"

::method toArray
-- convert the list to an array
use strict arg
ptr = self~head
list = .Array~new()
if \ptr~isNull() then return list
do while \ptr~isNull()
   list~append(GrxGSListItem(ptr))
   ptr = GrxGSListNext(ptr)
   end
return list



/*============================================================================*/
/* Class: GList                                                               */
/*============================================================================*/

::class GList public subclass object

::METHOD init EXTERNAL "LIBRARY oorexxgtk2 GrxGListNew"
::METHOD uninit EXTERNAL "LIBRARY oorexxgtk2 GrxGListUninit"
::METHOD append EXTERNAL "LIBRARY oorexxgtk2 GrxGListAppend"
::METHOD head EXTERNAL "LIBRARY oorexxgtk2 GrxGListFirst"
::METHOD item EXTERNAL "LIBRARY oorexxgtk2 GrxGListItem"
::METHOD next EXTERNAL "LIBRARY oorexxgtk2 GrxGListNext"
::METHOD insert EXTERNAL "LIBRARY oorexxgtk2 GrxGListInsert"
::METHOD prepend EXTERNAL "LIBRARY oorexxgtk2 GrxGListPrepend"

::method toArray
-- convert the list to an array
use strict arg
ptr = self~head
list = .Array~new()
if \ptr~isNull() then return list
do while \ptr~isNull()
   list~append(GrxGSListItem(ptr))
   ptr = GrxGListNext(ptr)
   end
return list


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

::METHOD set_current_folder EXTERNAL "LIBRARY oorexxgtk2 GrxFileChooserSetCurrentFolder"
::METHOD 'current_folder=' EXTERNAL "LIBRARY oorexxgtk2 GrxFileChooserSetCurrentFolder"
::METHOD add_filter EXTERNAL "LIBRARY oorexxgtk2 GrxFileChooserAddFilter"
::METHOD get_filename EXTERNAL "LIBRARY oorexxgtk2 GrxFileChooserGetFilename"
::METHOD filename EXTERNAL "LIBRARY oorexxgtk2 GrxFileChooserGetFilename"
::METHOD filenames EXTERNAL "LIBRARY oorexxgtk2 GrxFileChooserGetFilenames"
::METHOD get_filenames EXTERNAL "LIBRARY oorexxgtk2 GrxFileChooserGetFilenames"
::METHOD set_select_multiple EXTERNAL "LIBRARY oorexxgtk2 GrxFileChooserSetSelectMultiple"
::METHOD 'select_multiple=' EXTERNAL "LIBRARY oorexxgtk2 GrxFileChooserSetSelectMultiple"
::METHOD signal_connect EXTERNAL "LIBRARY oorexxgtk2 GrxFileChooserSignalConnect"

-- The following are the methods that can be connected to signals. By default
-- they do nothing.

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

::METHOD init EXTERNAL "LIBRARY oorexxgtk2 GrxFileFilterNew"
::METHOD set_name EXTERNAL "LIBRARY oorexxgtk2 GrxFileFilterSetName"
::METHOD 'name=' EXTERNAL "LIBRARY oorexxgtk2 GrxFileFilterSetName"
::METHOD add_pattern EXTERNAL "LIBRARY oorexxgtk2 GrxFileFilterAddPattern"


/*============================================================================*/
/* Class: GtkClipboard                                                        */
/*============================================================================*/

::class GtkClipboard public

::METHOD init EXTERNAL "LIBRARY oorexxgtk2 GrxClipboardNew"


/*============================================================================*/
/* Class: GtkTreePath                                                         */
/*============================================================================*/

::class GtkTreePath public

::method newFromString
use strict arg spath
return .GtkTreePath~new(spath)

::METHOD init EXTERNAL "LIBRARY oorexxgtk2 GrxTreePathNew"
::METHOD to_string EXTERNAL "LIBRARY oorexxgtk2 GrxTreePathToString"
::METHOD up EXTERNAL "LIBRARY oorexxgtk2 GrxTreePathUp"
::METHOD down EXTERNAL "LIBRARY oorexxgtk2 GrxTreePathDown"
::METHOD prev EXTERNAL "LIBRARY oorexxgtk2 GrxTreePathPrev"
::METHOD next EXTERNAL "LIBRARY oorexxgtk2 GrxTreePathNext"
::METHOD prepend_index EXTERNAL "LIBRARY oorexxgtk2 GrxTreePathPrependIndex"
::METHOD append_index EXTERNAL "LIBRARY oorexxgtk2 GrxTreePathAppendIndex"
::METHOD get_depth EXTERNAL "LIBRARY oorexxgtk2 GrxTreePathGetDepth"
::METHOD depth EXTERNAL "LIBRARY oorexxgtk2 GrxTreePathGetDepth"
::METHOD is_ancestor EXTERNAL "LIBRARY oorexxgtk2 GrxTreePathIsAncestor"
::METHOD is_descendant EXTERNAL "LIBRARY oorexxgtk2 GrxTreePathIsDescendant"

::method create_tree_model private
use strict arg ptr
return .GtkTreeModel~new(ptr)


/*============================================================================*/
/* Class: GtkTreeModel                                                        */
/*============================================================================*/

::class GtkTreeModel public

::METHOD init EXTERNAL "LIBRARY oorexxgtk2 GrxTreeModelNew"
::METHOD get_n_columns EXTERNAL "LIBRARY oorexxgtk2 GrxTreeModelGetNColumns"
::METHOD n_columns EXTERNAL "LIBRARY oorexxgtk2 GrxTreeModelGetNColumns"
::METHOD get_column_type EXTERNAL "LIBRARY oorexxgtk2 GrxTreeModelGetColumnType"
::METHOD column_type EXTERNAL "LIBRARY oorexxgtk2 GrxTreeModelGetColumnType"
::METHOD get_iter EXTERNAL "LIBRARY oorexxgtk2 GrxTreeModelGetIter"
::METHOD get_iter_from_string EXTERNAL "LIBRARY oorexxgtk2 GrxTreeModelGetIterFromString"
::METHOD get_value EXTERNAL "LIBRARY oorexxgtk2 GrxTreeModelGetValue"
::METHOD value EXTERNAL "LIBRARY oorexxgtk2 GrxTreeModelGetValue"
::METHOD iter_next EXTERNAL "LIBRARY oorexxgtk2 GrxTreeModelIterNext"
::METHOD get_path EXTERNAL "LIBRARY oorexxgtk2 GrxTreeModelGetPath"
::METHOD path EXTERNAL "LIBRARY oorexxgtk2 GrxTreeModelGetPath"
::METHOD has_parent EXTERNAL "LIBRARY oorexxgtk2 GrxTreeModelHasParent"
::METHOD get_parent EXTERNAL "LIBRARY oorexxgtk2 GrxTreeModelGetParent"

::method create_tree_path private
use strict arg ptr
return .GtkTreePath~new(ptr)

