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


#ifndef REXXGTK_H
#define REXXGTK_H


/*----------------------------------------------------------------------------*/
/* Definitions                                                                */
/*----------------------------------------------------------------------------*/

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <oorexxapi.h>
#include <gtk/gtk.h>


/*----------------------------------------------------------------------------*/
/* Global variables                                                           */
/*----------------------------------------------------------------------------*/

extern const char * rexxgtk_argv[];
extern int rexxgtk_argc;

typedef struct _cbcb {
    RexxInstance  *instance;         // Rexx instance
    const char    *signal_name;      // Name of the ooRexx method
} cbcb;


/*----------------------------------------------------------------------------*/
/* Function Prototypes                                                        */
/*----------------------------------------------------------------------------*/

gboolean signal_GdkEvent(GtkWidget *window,
                   GdkEvent *event,
                   gpointer data);
gboolean signal_GdkEventKey(GtkWidget *window,
                   GdkEventKey *event,
                   gpointer data);
gboolean signal_GdkEventButton(GtkWidget *window,
                   GdkEventButton *event,
                   gpointer data);
gboolean signal_GdkEventScroll(GtkWidget *window,
                   GdkEventScroll *event,
                   gpointer data);
gboolean signal_GdkEventMotion(GtkWidget *window,
                   GdkEventMotion *event,
                   gpointer data);
gboolean signal_GdkEventExpose(GtkWidget *window,
                   GdkEventExpose *event,
                   gpointer data);
gboolean signal_GdkEventVisibility(GtkWidget *window,
                   GdkEventVisibility *event,
                   gpointer data);
gboolean signal_GdkEventCrossing(GtkWidget *window,
                   GdkEventCrossing *event,
                   gpointer data);
gboolean signal_GdkEventFocus(GtkWidget *window,
                   GdkEventFocus *event,
                   gpointer data);
gboolean signal_GdkEventConfigure(GtkWidget *window,
                   GdkEventConfigure *event,
                   gpointer data);
gboolean signal_GdkEventProperty(GtkWidget *window,
                   GdkEventProperty *event,
                   gpointer data);
gboolean signal_GdkEventSelection(GtkWidget *window,
                   GdkEventSelection *event,
                   gpointer data);
gboolean signal_GdkEventDND(GtkWidget *window,
                   GdkEventDND *event,
                   gpointer data);
gboolean signal_GdkEventProximity(GtkWidget *window,
                   GdkEventProximity *event,
                   gpointer data);
gboolean signal_GdkEventClient(GtkWidget *window,
                   GdkEventClient *event,
                   gpointer data);
gboolean signal_GdkEventNoExpose(GtkWidget *window,
                   GdkEventNoExpose *event,
                   gpointer data);
gboolean signal_GdkEventWindowState(GtkWidget *window,
                   GdkEventWindowState *event,
                   gpointer data);
gboolean signal_GdkEventSetting(GtkWidget *window,
                   GdkEventSetting *event,
                   gpointer data);
gboolean signal_GdkEventOwnerChange(GtkWidget *window,
                   GdkEventOwnerChange *event,
                   gpointer data);
gboolean signal_GdkEventGrabBroken(GtkWidget *window,
                   GdkEventGrabBroken *event,
                   gpointer data);


/*----------------------------------------------------------------------------*/
/* Method Prototypes                                                          */
/*----------------------------------------------------------------------------*/

REXX_METHOD_PROTOTYPE(GrxWidgetNew)
REXX_METHOD_PROTOTYPE(GrxWidgetSetCSELF)
REXX_METHOD_PROTOTYPE(GrxWidgetShow)
REXX_METHOD_PROTOTYPE(GrxWidgetShowAll)
REXX_METHOD_PROTOTYPE(GrxWidgetHide)
REXX_METHOD_PROTOTYPE(GrxWidgetHideAll)
REXX_METHOD_PROTOTYPE(GrxWidgetSetSizeRequest)
REXX_METHOD_PROTOTYPE(GrxWidgetDestroy)
REXX_METHOD_PROTOTYPE(GrxWidgetGrabFocus)
REXX_METHOD_PROTOTYPE(GrxWidgetGetSetFont)
REXX_METHOD_PROTOTYPE(GrxWidgetGetSetName)
REXX_METHOD_PROTOTYPE(GrxWidgetSetSensitive)
REXX_METHOD_PROTOTYPE(GrxWidgetModifyBG)
REXX_METHOD_PROTOTYPE(GrxWidgetModifyFG)
REXX_METHOD_PROTOTYPE(GrxWidgetSetData)
REXX_METHOD_PROTOTYPE(GrxWidgetGetData)
REXX_METHOD_PROTOTYPE(GrxWidgetActivate)
REXX_METHOD_PROTOTYPE(GrxWidgetReparent)
REXX_METHOD_PROTOTYPE(GrxWidgetIsFocus)
REXX_METHOD_PROTOTYPE(GrxWidgetGrabDefault)
REXX_METHOD_PROTOTYPE(GrxWidgetGetSetParentWindow)
REXX_METHOD_PROTOTYPE(GrxWidgetGetToplevel)
REXX_METHOD_PROTOTYPE(GrxWidgetIsAncestor)
REXX_METHOD_PROTOTYPE(GrxWidgetGetAncestor)
REXX_METHOD_PROTOTYPE(GrxWidgetGetSetDirection)
REXX_METHOD_PROTOTYPE(GrxWidgetGetDefaultDirection)
REXX_METHOD_PROTOTYPE(GrxWidgetSetDefaultDirection)
REXX_METHOD_PROTOTYPE(GrxWidgetQueueDrawArea)
REXX_METHOD_PROTOTYPE(GrxWidgetResetShapes)
REXX_METHOD_PROTOTYPE(GrxWidgetSetScrollAdjustments)
REXX_METHOD_PROTOTYPE(GrxWidgetMnemonicActivate)
REXX_METHOD_PROTOTYPE(GrxWidgetGetAccessible)
REXX_METHOD_PROTOTYPE(GrxWidgetGetChildVisible)
REXX_METHOD_PROTOTYPE(GrxWidgetGetParent)
REXX_METHOD_PROTOTYPE(GrxWidgetGetMenus)
REXX_METHOD_PROTOTYPE(GrxWidgetSetEvents)
REXX_METHOD_PROTOTYPE(GrxWidgetAddAccelerator)
REXX_METHOD_PROTOTYPE(GrxWidgetRemoveAccelerator)
REXX_METHOD_PROTOTYPE(GrxWidgetSetAccelPath)
REXX_METHOD_PROTOTYPE(GrxWidgetRealize)
REXX_METHOD_PROTOTYPE(GrxWidgetUnrealize)
REXX_METHOD_PROTOTYPE(GrxWidgetSignalConnect)
REXX_METHOD_PROTOTYPE(GrxWindowNew)
REXX_METHOD_PROTOTYPE(GrxWindowGetTitle)
REXX_METHOD_PROTOTYPE(GrxWindowSetTitle)
REXX_METHOD_PROTOTYPE(GrxWindowGetModal)
REXX_METHOD_PROTOTYPE(GrxWindowSetModal)
REXX_METHOD_PROTOTYPE(GrxWindowAddAccelGroup)
REXX_METHOD_PROTOTYPE(GrxWindowSignalConnect)
REXX_METHOD_PROTOTYPE(GrxContainerAdd)
REXX_METHOD_PROTOTYPE(GrxContainerRemove)
REXX_METHOD_PROTOTYPE(GrxContainerSetBorderWidth)
REXX_METHOD_PROTOTYPE(GrxContainerSignalConnect)
REXX_METHOD_PROTOTYPE(GrxBoxPackStart)
REXX_METHOD_PROTOTYPE(GrxBoxPackEnd)
REXX_METHOD_PROTOTYPE(GrxVBoxNew)
REXX_METHOD_PROTOTYPE(GrxVBoxNewFromPtr)
REXX_METHOD_PROTOTYPE(GrxHBoxNew)
REXX_METHOD_PROTOTYPE(GrxHBoxNewFromPtr)
REXX_METHOD_PROTOTYPE(GrxAlignmentNew)
REXX_METHOD_PROTOTYPE(GrxViewportNew)
REXX_METHOD_PROTOTYPE(GrxViewportSignalConnect)
REXX_METHOD_PROTOTYPE(GrxPanedAdd1)
REXX_METHOD_PROTOTYPE(GrxPanedAdd2)
REXX_METHOD_PROTOTYPE(GrxPanedPack1)
REXX_METHOD_PROTOTYPE(GrxPanedPack2)
REXX_METHOD_PROTOTYPE(GrxVPanedNew)
REXX_METHOD_PROTOTYPE(GrxHPanedNew)
REXX_METHOD_PROTOTYPE(GrxButtonNew)
REXX_METHOD_PROTOTYPE(GrxButtonSetUseStock)
REXX_METHOD_PROTOTYPE(GrxButtonGetSetLabel)
REXX_METHOD_PROTOTYPE(GrxButtonSetRelief)
REXX_METHOD_PROTOTYPE(GrxButtonUseUnderline)
REXX_METHOD_PROTOTYPE(GrxButtonSignalConnect)
REXX_METHOD_PROTOTYPE(GrxToggleButtonNew)
REXX_METHOD_PROTOTYPE(GrxToggleButtonGetSetMode)
REXX_METHOD_PROTOTYPE(GrxToggleButtonGetSetActive)
REXX_METHOD_PROTOTYPE(GrxToggleButtonGetSetInconsistent)
REXX_METHOD_PROTOTYPE(GrxToggleButtonSignalConnect)
REXX_METHOD_PROTOTYPE(GrxCheckButtonNew)
REXX_METHOD_PROTOTYPE(GrxRadioButtonNew)
REXX_METHOD_PROTOTYPE(GrxRadioButtonSignalConnect)
REXX_METHOD_PROTOTYPE(GrxColorButtonNew)
REXX_METHOD_PROTOTYPE(GrxColorButtonSetTitle)
REXX_METHOD_PROTOTYPE(GrxColorButtonGetSetColor)
REXX_METHOD_PROTOTYPE(GrxColorButtonSignalConnect)
REXX_METHOD_PROTOTYPE(GrxFileChooserButtonNew)
REXX_METHOD_PROTOTYPE(GrxFileChooserButtonSignalConnect)
REXX_METHOD_PROTOTYPE(GrxFontButtonNew)
REXX_METHOD_PROTOTYPE(GrxFontButtonSetTitle)
REXX_METHOD_PROTOTYPE(GrxFontButtonGetFontName)
REXX_METHOD_PROTOTYPE(GrxFontButtonSignalConnect)
REXX_METHOD_PROTOTYPE(GrxTableNew)
REXX_METHOD_PROTOTYPE(GrxTableAttach)
REXX_METHOD_PROTOTYPE(GrxTableSetRowSpacings)
REXX_METHOD_PROTOTYPE(GrxTableSetColSpacings)
REXX_METHOD_PROTOTYPE(GrxComboBoxNew)
REXX_METHOD_PROTOTYPE(GrxComboBoxNewText)
REXX_METHOD_PROTOTYPE(GrxComboBoxAppendText)
REXX_METHOD_PROTOTYPE(GrxComboBoxGetSetActive)
REXX_METHOD_PROTOTYPE(GrxComboBoxGetActiveText)
REXX_METHOD_PROTOTYPE(GrxComboBoxSignalConnect)
REXX_METHOD_PROTOTYPE(GrxDialogNewWithButtons)
REXX_METHOD_PROTOTYPE(GrxDialogAddButton)
REXX_METHOD_PROTOTYPE(GrxDialogSetHasSeparator)
REXX_METHOD_PROTOTYPE(GrxDialogSetDefaultResponse)
REXX_METHOD_PROTOTYPE(GrxDialogRun)
REXX_METHOD_PROTOTYPE(GrxDialogSignalConnect)
REXX_METHOD_PROTOTYPE(GrxMessageDialogNew)
REXX_METHOD_PROTOTYPE(GrxFileChooserDialogNew)
REXX_METHOD_PROTOTYPE(GrxFontSelectionDialogNew)
REXX_METHOD_PROTOTYPE(GrxFontSelectionDialogGetSetFontName)
REXX_METHOD_PROTOTYPE(GrxFontSelectionDialogSetPreviewText)
REXX_METHOD_PROTOTYPE(GrxAboutDialogNew)
REXX_METHOD_PROTOTYPE(GrxAboutDialogSetName)
REXX_METHOD_PROTOTYPE(GrxAboutDialogSetVersion)
REXX_METHOD_PROTOTYPE(GrxAboutDialogSetCopyright)
REXX_METHOD_PROTOTYPE(GrxAboutDialogSetComments)
REXX_METHOD_PROTOTYPE(GrxAboutDialogSetLicense)
REXX_METHOD_PROTOTYPE(GrxAboutDialogSetWrapLicense)
REXX_METHOD_PROTOTYPE(GrxAboutDialogSetWebsite)
REXX_METHOD_PROTOTYPE(GrxAboutDialogSetWebsiteLabel)
REXX_METHOD_PROTOTYPE(GrxAboutDialogSetAuthors)
REXX_METHOD_PROTOTYPE(GrxAboutDialogSetArtists)
REXX_METHOD_PROTOTYPE(GrxAboutDialogSetDocumentors)
REXX_METHOD_PROTOTYPE(GrxAboutDialogSetLogo)
REXX_METHOD_PROTOTYPE(GrxColorSelectionDialogNew)
REXX_METHOD_PROTOTYPE(GrxColorSelectionDialogGetColor)
REXX_METHOD_PROTOTYPE(GrxEntryNew)
REXX_METHOD_PROTOTYPE(GrxEntryGetSetMaxLength)
REXX_METHOD_PROTOTYPE(GrxEntryGetSetText)
REXX_METHOD_PROTOTYPE(GrxEntrySetWidthChars)
REXX_METHOD_PROTOTYPE(GrxEntryGetSetVisibility)
REXX_METHOD_PROTOTYPE(GrxEntryGetSetVisibility)
REXX_METHOD_PROTOTYPE(GrxEntryGetSetInvisibleChar)
REXX_METHOD_PROTOTYPE(GrxEntrySignalConnect)
REXX_METHOD_PROTOTYPE(GrxSpinButtonNew)
REXX_METHOD_PROTOTYPE(GrxSpinButtonSetDigits)
REXX_METHOD_PROTOTYPE(GrxSpinButtonNewWithRange)
REXX_METHOD_PROTOTYPE(GrxSpinButtonSignalConnect)
REXX_METHOD_PROTOTYPE(GrxAssistantNew)
REXX_METHOD_PROTOTYPE(GrxAssistantGetSetCurrentPage)
REXX_METHOD_PROTOTYPE(GrxAssistantGetNthPage)
REXX_METHOD_PROTOTYPE(GrxAssistantPrependPage)
REXX_METHOD_PROTOTYPE(GrxAssistantAppendPage)
REXX_METHOD_PROTOTYPE(GrxAssistantInsertPage)
REXX_METHOD_PROTOTYPE(GrxAssistantSetPageType)
REXX_METHOD_PROTOTYPE(GrxAssistantSetPageTitle)
REXX_METHOD_PROTOTYPE(GrxAssistantSetPageComplete)
REXX_METHOD_PROTOTYPE(GrxAssistantSetPageHeaderImage)
REXX_METHOD_PROTOTYPE(GrxAssistantSetPageSideImage)
REXX_METHOD_PROTOTYPE(GrxAssistantSignalConnect)
REXX_METHOD_PROTOTYPE(GrxLabelNew)
REXX_METHOD_PROTOTYPE(GrxLabelGetSetLabel)
REXX_METHOD_PROTOTYPE(GrxLabelGetSetJustify)
REXX_METHOD_PROTOTYPE(GrxLabelSetSelectable)
REXX_METHOD_PROTOTYPE(GrxLabelSignalConnect)
REXX_METHOD_PROTOTYPE(GrxFixedNew)
REXX_METHOD_PROTOTYPE(GrxFixedPut)
REXX_METHOD_PROTOTYPE(GrxFixedMove)
REXX_METHOD_PROTOTYPE(GrxExpanderNew)
REXX_METHOD_PROTOTYPE(GrxExpanderSetExpanded)
REXX_METHOD_PROTOTYPE(GrxExpanderGetSetLabel)
REXX_METHOD_PROTOTYPE(GrxExpanderSetUseUnderline)
REXX_METHOD_PROTOTYPE(GrxExpanderSetSpacing)
REXX_METHOD_PROTOTYPE(GrxExpanderSignalConnect)
REXX_METHOD_PROTOTYPE(GrxHandleBoxNew)
REXX_METHOD_PROTOTYPE(GrxHandleBoxSetShadowType)
REXX_METHOD_PROTOTYPE(GrxHandleBoxSetPosition)
REXX_METHOD_PROTOTYPE(GrxHandleBoxSetSnapEdge)
REXX_METHOD_PROTOTYPE(GrxHandleBoxSignalConnect)
REXX_METHOD_PROTOTYPE(GrxNotebookNew)
REXX_METHOD_PROTOTYPE(GrxNotebookAppendPage)
REXX_METHOD_PROTOTYPE(GrxNotebookPrependPage)
REXX_METHOD_PROTOTYPE(GrxNotebookInsertPage)
REXX_METHOD_PROTOTYPE(GrxNotebookAppendPageMenu)
REXX_METHOD_PROTOTYPE(GrxNotebookPrependPageMenu)
REXX_METHOD_PROTOTYPE(GrxNotebookInsertPageMenu)
REXX_METHOD_PROTOTYPE(GrxNotebookRemovePage)
REXX_METHOD_PROTOTYPE(GrxNotebookSetShowBorder)
REXX_METHOD_PROTOTYPE(GrxNotebookSetShowTabs)
REXX_METHOD_PROTOTYPE(GrxNotebookSetTabPos)
REXX_METHOD_PROTOTYPE(GrxNotebookSetHomogeneousTabs)
REXX_METHOD_PROTOTYPE(GrxNotebookSetTabBorder)
REXX_METHOD_PROTOTYPE(GrxNotebookSetScrollable)
REXX_METHOD_PROTOTYPE(GrxNotebookPopupEnable)
REXX_METHOD_PROTOTYPE(GrxNotebookPopupDisable)
REXX_METHOD_PROTOTYPE(GrxNotebookGetSetCurrentPage)
REXX_METHOD_PROTOTYPE(GrxNotebookSignalConnect)
REXX_METHOD_PROTOTYPE(GrxScaleGetSetDigits)
REXX_METHOD_PROTOTYPE(GrxScaleGetSetValuePos)
REXX_METHOD_PROTOTYPE(GrxHScaleNew)
REXX_METHOD_PROTOTYPE(GrxHScaleNewWithRange)
REXX_METHOD_PROTOTYPE(GrxVScaleNew)
REXX_METHOD_PROTOTYPE(GrxVScaleNewWithRange)
REXX_METHOD_PROTOTYPE(GrxRangeSignalConnect)
REXX_METHOD_PROTOTYPE(GrxImageNew)
REXX_METHOD_PROTOTYPE(GrxImageSetFromFile)
REXX_METHOD_PROTOTYPE(GrxImageSetFromStock)
REXX_METHOD_PROTOTYPE(GrxImageSetFromIconName)
REXX_METHOD_PROTOTYPE(GrxProgressBarNew)
REXX_METHOD_PROTOTYPE(GrxProgressBarSetFraction)
REXX_METHOD_PROTOTYPE(GrxProgressBarSetPulseStep)
REXX_METHOD_PROTOTYPE(GrxProgressBarPulse)
REXX_METHOD_PROTOTYPE(GrxProgressBarSetText)
REXX_METHOD_PROTOTYPE(GrxTextViewNew)
REXX_METHOD_PROTOTYPE(GrxTextViewGetBuffer)
REXX_METHOD_PROTOTYPE(GrxTextViewGetSetWrapMode)
REXX_METHOD_PROTOTYPE(GrxTextViewGetSetJustification)
REXX_METHOD_PROTOTYPE(GrxTextViewGetSetEditable)
REXX_METHOD_PROTOTYPE(GrxTextViewGetSetCursorVisible)
REXX_METHOD_PROTOTYPE(GrxTextViewGetSetPixelsAboveLines)
REXX_METHOD_PROTOTYPE(GrxTextViewGetSetPixelsBelowLines)
REXX_METHOD_PROTOTYPE(GrxTextViewGetSetPixelsInsideWrap)
REXX_METHOD_PROTOTYPE(GrxTextViewGetSetLeftMargin)
REXX_METHOD_PROTOTYPE(GrxTextViewGetSetRightMargin)
REXX_METHOD_PROTOTYPE(GrxTextViewAddChildAtOffset)
REXX_METHOD_PROTOTYPE(GrxTextViewSignalConnect)
REXX_METHOD_PROTOTYPE(GrxTextBufferNew)
REXX_METHOD_PROTOTYPE(GrxTextBufferGetSetText)
REXX_METHOD_PROTOTYPE(GrxTextBufferInsertText)
REXX_METHOD_PROTOTYPE(GrxTextBufferCutClipboard)
REXX_METHOD_PROTOTYPE(GrxTextBufferCopyClipboard)
REXX_METHOD_PROTOTYPE(GrxTextBufferPasteClipboard)
REXX_METHOD_PROTOTYPE(GrxTextBufferForwardSearch)
REXX_METHOD_PROTOTYPE(GrxTextBufferForwardSearchNext)
REXX_METHOD_PROTOTYPE(GrxTextBufferCreateTagInt)
REXX_METHOD_PROTOTYPE(GrxTextBufferCreateTagFloat)
REXX_METHOD_PROTOTYPE(GrxTextBufferApplyTagByName)
REXX_METHOD_PROTOTYPE(GrxTextBufferRemoveAllTags)
REXX_METHOD_PROTOTYPE(GrxTextBufferInsertImage)
REXX_METHOD_PROTOTYPE(GrxTextBufferSignalConnect)
REXX_METHOD_PROTOTYPE(GrxGSListNew)
REXX_METHOD_PROTOTYPE(GrxGSListUninit)
REXX_METHOD_PROTOTYPE(GrxGSListAppend)
REXX_METHOD_PROTOTYPE(GrxGSListPrepend)
REXX_METHOD_PROTOTYPE(GrxGSListInsert)
REXX_METHOD_PROTOTYPE(GrxGSListItem)
REXX_METHOD_PROTOTYPE(GrxGSListFirst)
REXX_METHOD_PROTOTYPE(GrxGSListNext)
REXX_METHOD_PROTOTYPE(GrxGListNew)
REXX_METHOD_PROTOTYPE(GrxGListUninit)
REXX_METHOD_PROTOTYPE(GrxGListAppend)
REXX_METHOD_PROTOTYPE(GrxGListPrepend)
REXX_METHOD_PROTOTYPE(GrxGListInsert)
REXX_METHOD_PROTOTYPE(GrxGListItem)
REXX_METHOD_PROTOTYPE(GrxGListFirst)
REXX_METHOD_PROTOTYPE(GrxGListNext)
REXX_METHOD_PROTOTYPE(GrxFileChooserSetCurrentFolder)
REXX_METHOD_PROTOTYPE(GrxFileChooserSetSelectMultiple)
REXX_METHOD_PROTOTYPE(GrxFileChooserAddFilter)
REXX_METHOD_PROTOTYPE(GrxFileChooserGetFilename)
REXX_METHOD_PROTOTYPE(GrxFileChooserGetFilenames)
REXX_METHOD_PROTOTYPE(GrxFileChooserSignalConnect)
REXX_METHOD_PROTOTYPE(GrxFileFilterNew)
REXX_METHOD_PROTOTYPE(GrxFileFilterSetName)
REXX_METHOD_PROTOTYPE(GrxFileFilterAddPattern)
REXX_METHOD_PROTOTYPE(GrxClipboardNew)
REXX_METHOD_PROTOTYPE(GrxAdjustmentNew)
REXX_METHOD_PROTOTYPE(GrxAdjustmentNewFromPtr)
REXX_METHOD_PROTOTYPE(GrxAdjustmentGetSetValue)
REXX_METHOD_PROTOTYPE(GrxAdjustmentClampPage)
REXX_METHOD_PROTOTYPE(GrxAdjustmentSignalConnect)
REXX_METHOD_PROTOTYPE(GrxScrolledWindowNew)
REXX_METHOD_PROTOTYPE(GrxScrolledWindowGetHAdjustment)
REXX_METHOD_PROTOTYPE(GrxScrolledWindowGetVAdjustment)
REXX_METHOD_PROTOTYPE(GrxScrolledWindowSetPolicy)
REXX_METHOD_PROTOTYPE(GrxScrolledWindowAddWithViewport)
REXX_METHOD_PROTOTYPE(GrxScrolledWindowSignalConnect)
REXX_METHOD_PROTOTYPE(GrxListStoreNew)
REXX_METHOD_PROTOTYPE(GrxListStoreAppend)
REXX_METHOD_PROTOTYPE(GrxListStoreSetValue)
REXX_METHOD_PROTOTYPE(GrxEventBoxNew)
REXX_METHOD_PROTOTYPE(GrxEventBoxGetSetAboveChild)
REXX_METHOD_PROTOTYPE(GrxEventBoxGetSetVisibleWindow)
REXX_METHOD_PROTOTYPE(GrxMenuShellAppend)
REXX_METHOD_PROTOTYPE(GrxMenuShellPrepend)
REXX_METHOD_PROTOTYPE(GrxMenuShellInsert)
REXX_METHOD_PROTOTYPE(GrxMenuShellDeactivate)
REXX_METHOD_PROTOTYPE(GrxMenuShellSelectItem)
REXX_METHOD_PROTOTYPE(GrxMenuShellSelectFirst)
REXX_METHOD_PROTOTYPE(GrxMenuShellDeselect)
REXX_METHOD_PROTOTYPE(GrxMenuShellActivateItem)
REXX_METHOD_PROTOTYPE(GrxMenuShellCancel)
REXX_METHOD_PROTOTYPE(GrxMenuShellSetTakeFocus)
REXX_METHOD_PROTOTYPE(GrxMenuShellGetTakeFocus)
REXX_METHOD_PROTOTYPE(GrxMenuShellSignalConnect)
REXX_METHOD_PROTOTYPE(GrxMenuNew)
REXX_METHOD_PROTOTYPE(GrxMenuReorderChild)
REXX_METHOD_PROTOTYPE(GrxMenuAttach)
REXX_METHOD_PROTOTYPE(GrxMenuPopup)
REXX_METHOD_PROTOTYPE(GrxMenuGetSetAccelGroup)
REXX_METHOD_PROTOTYPE(GrxMenuSetAccelPath)
REXX_METHOD_PROTOTYPE(GrxMenuGetSetTitle)
REXX_METHOD_PROTOTYPE(GrxMenuSetTearoffState)
REXX_METHOD_PROTOTYPE(GrxMenuGetTearoffState)
REXX_METHOD_PROTOTYPE(GrxMenuPopdown)
REXX_METHOD_PROTOTYPE(GrxMenuReposition)
REXX_METHOD_PROTOTYPE(GrxMenuGetSetActive)
REXX_METHOD_PROTOTYPE(GrxMenuAttachToWidget)
REXX_METHOD_PROTOTYPE(GrxMenuDetach)
REXX_METHOD_PROTOTYPE(GrxMenuGetAttachWidget)
REXX_METHOD_PROTOTYPE(GrxMenuSignalConnect)
REXX_METHOD_PROTOTYPE(GrxMenuItemNew)
REXX_METHOD_PROTOTYPE(GrxMenuItemGetSetRightJustified)
REXX_METHOD_PROTOTYPE(GrxMenuItemSetSubmenu)
REXX_METHOD_PROTOTYPE(GrxMenuItemRemoveSubmenu)
REXX_METHOD_PROTOTYPE(GrxMenuItemSetAccelPath)
REXX_METHOD_PROTOTYPE(GrxMenuItemSelect)
REXX_METHOD_PROTOTYPE(GrxMenuItemDeselect)
REXX_METHOD_PROTOTYPE(GrxMenuItemSignalConnect)
REXX_METHOD_PROTOTYPE(GrxSeparatorMenuItemNew)
REXX_METHOD_PROTOTYPE(GrxAccelGroupNew)
REXX_METHOD_PROTOTYPE(GrxAccelGroupSignalConnect)
REXX_METHOD_PROTOTYPE(GrxStatusbarNew)
REXX_METHOD_PROTOTYPE(GrxStatusbarGetContextId)
REXX_METHOD_PROTOTYPE(GrxStatusbarPush)
REXX_METHOD_PROTOTYPE(GrxStatusbarPop)
REXX_METHOD_PROTOTYPE(GrxStatusbarRemove)
REXX_METHOD_PROTOTYPE(GrxStatusbarGetSetHasResizeGrip)
REXX_METHOD_PROTOTYPE(GrxStatusbarSignalConnect)
REXX_METHOD_PROTOTYPE(GrxMenuBarNew)
REXX_METHOD_PROTOTYPE(GrxMenuBarAppend)
REXX_METHOD_PROTOTYPE(GrxMenuBarPrepend)
REXX_METHOD_PROTOTYPE(GrxMenuBarInsert)
REXX_METHOD_PROTOTYPE(GrxMenuBarGetSetPackDirection)
REXX_METHOD_PROTOTYPE(GrxMenuBarGetSetChildPackDirection)
REXX_METHOD_PROTOTYPE(GrxImageMenuItemNew)
REXX_METHOD_PROTOTYPE(GrxToolbarNew)
REXX_METHOD_PROTOTYPE(GrxToolbarInsert)
REXX_METHOD_PROTOTYPE(GrxToolbarGetItemIndex)
REXX_METHOD_PROTOTYPE(GrxToolbarGetNItems)
REXX_METHOD_PROTOTYPE(GrxToolbarGetNthItem)
REXX_METHOD_PROTOTYPE(GrxToolbarGetDropIndex)
REXX_METHOD_PROTOTYPE(GrxToolbarSetDropHighlightItem)
REXX_METHOD_PROTOTYPE(GrxToolbarGetSetShowArrow)
REXX_METHOD_PROTOTYPE(GrxToolbarGetSetOrientation)
REXX_METHOD_PROTOTYPE(GrxToolbarSetTooltips)
REXX_METHOD_PROTOTYPE(GrxToolbarUnsetIconSize)
REXX_METHOD_PROTOTYPE(GrxToolbarGetSetStyle)
REXX_METHOD_PROTOTYPE(GrxToolbarGetSetIconSize)
REXX_METHOD_PROTOTYPE(GrxToolbarGetTooltips)
REXX_METHOD_PROTOTYPE(GrxToolbarGetReliefStyle)
REXX_METHOD_PROTOTYPE(GrxToolbarAppendItem)
REXX_METHOD_PROTOTYPE(GrxToolbarPrependItem)
REXX_METHOD_PROTOTYPE(GrxToolbarInsertItem)
REXX_METHOD_PROTOTYPE(GrxToolbarAppendSpace)
REXX_METHOD_PROTOTYPE(GrxToolbarPrependSpace)
REXX_METHOD_PROTOTYPE(GrxToolbarInsertSpace)
REXX_METHOD_PROTOTYPE(GrxToolbarAppendElement)
REXX_METHOD_PROTOTYPE(GrxToolbarPrependElement)
REXX_METHOD_PROTOTYPE(GrxToolbarInsertElement)
REXX_METHOD_PROTOTYPE(GrxToolbarAppendWidget)
REXX_METHOD_PROTOTYPE(GrxToolbarPrependWidget)
REXX_METHOD_PROTOTYPE(GrxToolbarInsertWidget)
REXX_METHOD_PROTOTYPE(GrxToolbarInsertStock)
REXX_METHOD_PROTOTYPE(GrxToolbarRemoveSpace)
REXX_METHOD_PROTOTYPE(GrxToolbarUnsetStyle)
REXX_METHOD_PROTOTYPE(GrxToolbarSignalConnect)
REXX_METHOD_PROTOTYPE(GrxToolItemNew)
REXX_METHOD_PROTOTYPE(GrxToolItemGetSetHomogeneous)
REXX_METHOD_PROTOTYPE(GrxToolItemGetSetExpand)
REXX_METHOD_PROTOTYPE(GrxToolItemSetTooltip)
REXX_METHOD_PROTOTYPE(GrxToolItemGetSetUseDragWindow)
REXX_METHOD_PROTOTYPE(GrxToolItemGetSetVisibleHorizontal)
REXX_METHOD_PROTOTYPE(GrxToolItemGetSetVisibleVertical)
REXX_METHOD_PROTOTYPE(GrxToolItemGetSetIsImportant)
REXX_METHOD_PROTOTYPE(GrxToolItemGetIconSize)
REXX_METHOD_PROTOTYPE(GrxToolItemGetOrientation)
REXX_METHOD_PROTOTYPE(GrxToolItemGetToolbarStyle)
REXX_METHOD_PROTOTYPE(GrxToolItemGetReliefStyle)
REXX_METHOD_PROTOTYPE(GrxToolItemRetrieveProxyMenuItem)
REXX_METHOD_PROTOTYPE(GrxToolItemGetProxyMenuItem)
REXX_METHOD_PROTOTYPE(GrxToolItemSetProxyMenuItem)
REXX_METHOD_PROTOTYPE(GrxToolItemRebuildMenu)
REXX_METHOD_PROTOTYPE(GrxToolItemSignalConnect)
REXX_METHOD_PROTOTYPE(GrxToolButtonNew)
REXX_METHOD_PROTOTYPE(GrxToolButtonNewFromStock)
REXX_METHOD_PROTOTYPE(GrxToolButtonGetSetLabel)
REXX_METHOD_PROTOTYPE(GrxToolButtonSetUseUnderline)
REXX_METHOD_PROTOTYPE(GrxToolButtonGetUseUnderline)
REXX_METHOD_PROTOTYPE(GrxToolButtonGetSetStockId)
REXX_METHOD_PROTOTYPE(GrxToolButtonGetSetIconName)
REXX_METHOD_PROTOTYPE(GrxToolButtonGetSetIconWidget)
REXX_METHOD_PROTOTYPE(GrxToolButtonGetSetLabelWidget)
REXX_METHOD_PROTOTYPE(GrxToolButtonSignalConnect)
REXX_METHOD_PROTOTYPE(GrxSeparatorToolItemNew)
REXX_METHOD_PROTOTYPE(GrxSeparatorToolItemGetSetDraw)
REXX_METHOD_PROTOTYPE(GrxMenuToolButtonNew)
REXX_METHOD_PROTOTYPE(GrxMenuToolButtonGetSetMenu)
REXX_METHOD_PROTOTYPE(GrxMenuToolButtonSignalConnect)
REXX_METHOD_PROTOTYPE(GrxCellRendererSetFixedSize)
REXX_METHOD_PROTOTYPE(GrxCellRendererSignalConnect)
REXX_METHOD_PROTOTYPE(GrxCellRendererTextNew)
REXX_METHOD_PROTOTYPE(GrxCellRendererTextSignalConnect)
REXX_METHOD_PROTOTYPE(GrxCellRendererPixbufNew)
REXX_METHOD_PROTOTYPE(GrxCellRendererProgressNew)
REXX_METHOD_PROTOTYPE(GrxCellRendererToggleNew)
REXX_METHOD_PROTOTYPE(GrxCellRendererToggleGetSetRadio)
REXX_METHOD_PROTOTYPE(GrxCellRendererToggleGetSetActive)
REXX_METHOD_PROTOTYPE(GrxCellRendererToggleSignalConnect)
REXX_METHOD_PROTOTYPE(GrxCellRendererAccelNew)
REXX_METHOD_PROTOTYPE(GrxCellRendererAccelSignalConnect)
REXX_METHOD_PROTOTYPE(GrxCellRendererComboNew)
REXX_METHOD_PROTOTYPE(GrxCellRendererComboSignalConnect)
REXX_METHOD_PROTOTYPE(GrxTreeViewNew)
REXX_METHOD_PROTOTYPE(GrxTreeViewAppendColumn)
REXX_METHOD_PROTOTYPE(GrxTreeViewInsertColumn)
REXX_METHOD_PROTOTYPE(GrxTreeViewRemoveColumn)
REXX_METHOD_PROTOTYPE(GrxTreeViewGetSetModel)
REXX_METHOD_PROTOTYPE(GrxTreeViewGetSelection)
REXX_METHOD_PROTOTYPE(GrxTreeViewExpandAll)
REXX_METHOD_PROTOTYPE(GrxTreeViewSignalConnect)
REXX_METHOD_PROTOTYPE(GrxTreeViewColumnNew)
REXX_METHOD_PROTOTYPE(GrxTreeViewColumnGetSetTitle)
REXX_METHOD_PROTOTYPE(GrxTreeViewColumnPackStart)
REXX_METHOD_PROTOTYPE(GrxTreeViewColumnPackEnd)
REXX_METHOD_PROTOTYPE(GrxTreeViewColumnSetAttribute)
REXX_METHOD_PROTOTYPE(GrxTreeViewColumnSignalConnect)
REXX_METHOD_PROTOTYPE(GrxTreeStoreNew)
REXX_METHOD_PROTOTYPE(GrxTreeStoreAppend)
REXX_METHOD_PROTOTYPE(GrxTreeStoreRemove)
REXX_METHOD_PROTOTYPE(GrxTreeStoreSetValue)
REXX_METHOD_PROTOTYPE(GrxTreePathNew)
REXX_METHOD_PROTOTYPE(GrxTreePathToString)
REXX_METHOD_PROTOTYPE(GrxTreePathUp)
REXX_METHOD_PROTOTYPE(GrxTreePathDown)
REXX_METHOD_PROTOTYPE(GrxTreePathPrev)
REXX_METHOD_PROTOTYPE(GrxTreePathNext)
REXX_METHOD_PROTOTYPE(GrxTreePathAppendIndex)
REXX_METHOD_PROTOTYPE(GrxTreePathPrependIndex)
REXX_METHOD_PROTOTYPE(GrxTreePathGetDepth)
REXX_METHOD_PROTOTYPE(GrxTreePathIsAncestor)
REXX_METHOD_PROTOTYPE(GrxTreePathIsDescendant)
REXX_METHOD_PROTOTYPE(GrxTreeRowReferenceNew)
REXX_METHOD_PROTOTYPE(GrxTreeRowReferenceValid)
REXX_METHOD_PROTOTYPE(GrxTreeRowReferenceGetPath)
REXX_METHOD_PROTOTYPE(GrxTreeModelNew)
REXX_METHOD_PROTOTYPE(GrxTreeModelGetNColumns)
REXX_METHOD_PROTOTYPE(GrxTreeModelGetColumnType)
REXX_METHOD_PROTOTYPE(GrxTreeModelGetIter)
REXX_METHOD_PROTOTYPE(GrxTreeModelGetIterFromString)
REXX_METHOD_PROTOTYPE(GrxTreeModelGetValue)
REXX_METHOD_PROTOTYPE(GrxTreeModelIterNext)
REXX_METHOD_PROTOTYPE(GrxTreeModelGetPath)
REXX_METHOD_PROTOTYPE(GrxTreeModelHasParent)
REXX_METHOD_PROTOTYPE(GrxTreeModelGetParent)
REXX_METHOD_PROTOTYPE(GrxCalendarNew)
REXX_METHOD_PROTOTYPE(GrxCalendarSelectMonth)
REXX_METHOD_PROTOTYPE(GrxCalendarSelectDay)
REXX_METHOD_PROTOTYPE(GrxCalendarMarkDay)
REXX_METHOD_PROTOTYPE(GrxCalendarUnmarkDay)
REXX_METHOD_PROTOTYPE(GrxCalendarClearMarks)
REXX_METHOD_PROTOTYPE(GrxCalendarGetDisplayOptions)
REXX_METHOD_PROTOTYPE(GrxCalendarSetDisplayOptions)
REXX_METHOD_PROTOTYPE(GrxCalendarGetDate)
REXX_METHOD_PROTOTYPE(GrxCalendarSignalConnect)
REXX_METHOD_PROTOTYPE(GrxTreeSelectionNewFromPtr)
REXX_METHOD_PROTOTYPE(GrxTreeSelectionGetSelectedRows)
REXX_METHOD_PROTOTYPE(GrxTreeSelectionSetMode)
REXX_METHOD_PROTOTYPE(GrxTreeSelectionSignalConnect)
 
 #endif /* REXXGTK_H */
 
