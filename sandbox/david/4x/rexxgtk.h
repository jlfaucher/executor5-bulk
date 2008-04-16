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

extern char * rexxgtk_argv[];
extern int rexxgtk_argc;

typedef struct _cbcb {
    RexxInstance  *instance;         // Rexx instance
    char          *signal_name;      // Name of the ooRexx method
} cbcb;


/*----------------------------------------------------------------------------*/
/* Function Prototypes                                                        */
/*----------------------------------------------------------------------------*/

REXX_METHOD_PROTOTYPE(GrxWidgetNew)
REXX_METHOD_PROTOTYPE(GrxWidgetUninit)
REXX_METHOD_PROTOTYPE(GrxWidgetShow)
REXX_METHOD_PROTOTYPE(GrxWidgetShowAll)
REXX_METHOD_PROTOTYPE(GrxWidgetHide)
REXX_METHOD_PROTOTYPE(GrxWidgetHideAll)
REXX_METHOD_PROTOTYPE(GrxWidgetSetSizeRequest)
REXX_METHOD_PROTOTYPE(GrxWidgetDestroy)
REXX_METHOD_PROTOTYPE(GrxWidgetGrabFocus)
REXX_METHOD_PROTOTYPE(GrxWidgetSetFont)
REXX_METHOD_PROTOTYPE(GrxWidgetGetFont)
REXX_METHOD_PROTOTYPE(GrxWidgetSetName)
REXX_METHOD_PROTOTYPE(GrxWidgetGetName)
REXX_METHOD_PROTOTYPE(GrxWidgetSetSensitive)
REXX_METHOD_PROTOTYPE(GrxWidgetModifyBG)
REXX_METHOD_PROTOTYPE(GrxWidgetModifyFG)
REXX_METHOD_PROTOTYPE(GrxWidgetSetData)
REXX_METHOD_PROTOTYPE(GrxWidgetGetData)
REXX_METHOD_PROTOTYPE(GrxWidgetActivate)
REXX_METHOD_PROTOTYPE(GrxWidgetReparent)
REXX_METHOD_PROTOTYPE(GrxWidgetIsFocus)
REXX_METHOD_PROTOTYPE(GrxWidgetGrabDefault)
REXX_METHOD_PROTOTYPE(GrxWidgetSetParentWindow)
REXX_METHOD_PROTOTYPE(GrxWidgetGetParentWindow)
REXX_METHOD_PROTOTYPE(GrxWidgetGetToplevel)
REXX_METHOD_PROTOTYPE(GrxWidgetIsAncestor)
REXX_METHOD_PROTOTYPE(GrxWidgetGetAncestor)
REXX_METHOD_PROTOTYPE(GrxWidgetGetDirection)
REXX_METHOD_PROTOTYPE(GrxWidgetSetDirection)
REXX_METHOD_PROTOTYPE(GrxWidgetGetDefaultDirection)
REXX_METHOD_PROTOTYPE(GrxWidgetSetDefaultDirection)
REXX_METHOD_PROTOTYPE(GrxWidgetQueueDrawArea)
REXX_METHOD_PROTOTYPE(GrxWidgetResetShapes)
REXX_METHOD_PROTOTYPE(GrxWidgetSetScrollAdjustments)
REXX_METHOD_PROTOTYPE(GrxWidgetMnemonicActivate)
REXX_METHOD_PROTOTYPE(GrxWidgetGetAccessible)
REXX_METHOD_PROTOTYPE(GrxWidgetGetChildVisible)
REXX_METHOD_PROTOTYPE(GrxWidgetGetParent)
REXX_METHOD_PROTOTYPE(GrxWidgetSignalConnect)
REXX_METHOD_PROTOTYPE(GrxWindowNew)
REXX_METHOD_PROTOTYPE(GrxWindowGetTitle)
REXX_METHOD_PROTOTYPE(GrxWindowSetTitle)
REXX_METHOD_PROTOTYPE(GrxWindowSignalConnect)
REXX_METHOD_PROTOTYPE(GrxContainerAdd)
REXX_METHOD_PROTOTYPE(GrxContainerRemove)
REXX_METHOD_PROTOTYPE(GrxContainerSetBorderWidth)
REXX_METHOD_PROTOTYPE(GrxContainerPackStart)
REXX_METHOD_PROTOTYPE(GrxContainerPackEnd)
REXX_METHOD_PROTOTYPE(GrxContainerSignalConnect)
REXX_METHOD_PROTOTYPE(GrxVBoxNew)
REXX_METHOD_PROTOTYPE(GrxHBoxNew)
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
REXX_METHOD_PROTOTYPE(GrxButtonNewFromStock)
REXX_METHOD_PROTOTYPE(GrxButtonSetLabel)
REXX_METHOD_PROTOTYPE(GrxButtonGetLabel)
REXX_METHOD_PROTOTYPE(GrxButtonSetRelief)
REXX_METHOD_PROTOTYPE(GrxButtonSetUnderline)
REXX_METHOD_PROTOTYPE(GrxButtonSignalConnect)
REXX_METHOD_PROTOTYPE(GrxToggleButtonNew)
REXX_METHOD_PROTOTYPE(GrxToggleButtonGetMode)
REXX_METHOD_PROTOTYPE(GrxToggleButtonSetMode)
REXX_METHOD_PROTOTYPE(GrxToggleButtonGetActive)
REXX_METHOD_PROTOTYPE(GrxToggleButtonSetActive)
REXX_METHOD_PROTOTYPE(GrxToggleButtonGetInconsistent)
REXX_METHOD_PROTOTYPE(GrxToggleButtonSetInconsistent)
REXX_METHOD_PROTOTYPE(GrxToggleButtonSignalConnect)
REXX_METHOD_PROTOTYPE(GrxCheckButtonNew)
REXX_METHOD_PROTOTYPE(GrxRadioButtonNew)
REXX_METHOD_PROTOTYPE(GrxRadioButtonNewFromWidget)
REXX_METHOD_PROTOTYPE(GrxRadioButtonGetGroup)
REXX_METHOD_PROTOTYPE(GrxRadioButtonSetGroup)
REXX_METHOD_PROTOTYPE(GrxRadioButtonSignalConnect)
REXX_METHOD_PROTOTYPE(GrxColorButtonNew)
REXX_METHOD_PROTOTYPE(GrxColorButtonSetTitle)
REXX_METHOD_PROTOTYPE(GrxColorButtonGetColor)
REXX_METHOD_PROTOTYPE(GrxColorButtonSetColor)
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
REXX_METHOD_PROTOTYPE(GrxComboBoxGetActive)
REXX_METHOD_PROTOTYPE(GrxComboBoxSetActive)
REXX_METHOD_PROTOTYPE(GrxComboBoxSignalConnect)
REXX_METHOD_PROTOTYPE(GrxDialogNew)
REXX_METHOD_PROTOTYPE(GrxDialogNewWithButtons)
REXX_METHOD_PROTOTYPE(GrxDialogAddButton)
REXX_METHOD_PROTOTYPE(GrxDialogSetHasSeparator)
REXX_METHOD_PROTOTYPE(GrxDialogSetDefaultResponse)
REXX_METHOD_PROTOTYPE(GrxDialogRun)
REXX_METHOD_PROTOTYPE(GrxDialogSignalConnect)
REXX_METHOD_PROTOTYPE(GrxMessageDialogNew)
REXX_METHOD_PROTOTYPE(GrxFileChooserDialogNew)
REXX_METHOD_PROTOTYPE(GrxFontSelectionDialogNew)
REXX_METHOD_PROTOTYPE(GrxFontSelectionDialogSetFontName)
REXX_METHOD_PROTOTYPE(GrxFontSelectionDialogGetFontName)
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
REXX_METHOD_PROTOTYPE(GrxEntrySetMaxLength)
REXX_METHOD_PROTOTYPE(GrxEntryGetMaxLength)
REXX_METHOD_PROTOTYPE(GrxEntrySetText)
REXX_METHOD_PROTOTYPE(GrxEntryGetText)
REXX_METHOD_PROTOTYPE(GrxEntrySetWidthChars)
REXX_METHOD_PROTOTYPE(GrxEntrySetVisibility)
REXX_METHOD_PROTOTYPE(GrxEntryGetVisibility)
REXX_METHOD_PROTOTYPE(GrxEntryGetInvisibleChar)
REXX_METHOD_PROTOTYPE(GrxEntrySetInvisibleChar)
REXX_METHOD_PROTOTYPE(GrxEntrySignalConnect)
REXX_METHOD_PROTOTYPE(GrxSpinButtonNew)
REXX_METHOD_PROTOTYPE(GrxSpinButtonNewWithRange)
REXX_METHOD_PROTOTYPE(GrxSpinButtonSignalConnect)
REXX_METHOD_PROTOTYPE(GrxAssistantNew)
REXX_METHOD_PROTOTYPE(GrxAssistantSetCurrentPage)
REXX_METHOD_PROTOTYPE(GrxAssistantGetCurrentPage)
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
REXX_METHOD_PROTOTYPE(GrxLabelSetText)
REXX_METHOD_PROTOTYPE(GrxLabelSetJustify)
REXX_METHOD_PROTOTYPE(GrxLabelGetJustify)
REXX_METHOD_PROTOTYPE(GrxLabelSetSelectable)
REXX_METHOD_PROTOTYPE(GrxLabelSignalConnect)
REXX_METHOD_PROTOTYPE(GrxFixedNew)
REXX_METHOD_PROTOTYPE(GrxFixedPut)
REXX_METHOD_PROTOTYPE(GrxFixedMove)
REXX_METHOD_PROTOTYPE(GrxExpanderNew)
REXX_METHOD_PROTOTYPE(GrxExpanderSetExpanded)
REXX_METHOD_PROTOTYPE(GrxExpanderSetLabel)
REXX_METHOD_PROTOTYPE(GrxExpanderGetLabel)
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
REXX_METHOD_PROTOTYPE(GrxNotebookGetCurrentPage)
REXX_METHOD_PROTOTYPE(GrxNotebookSetCurrentPage)
REXX_METHOD_PROTOTYPE(GrxNotebookSignalConnect)
REXX_METHOD_PROTOTYPE(GrxScaleSetDigits)
REXX_METHOD_PROTOTYPE(GrxScaleGetDigits)
REXX_METHOD_PROTOTYPE(GrxScaleSetValuePos)
REXX_METHOD_PROTOTYPE(GrxScaleGetValuePos)
REXX_METHOD_PROTOTYPE(GrxHScaleNew)
REXX_METHOD_PROTOTYPE(GrxHScaleNewWithRange)
REXX_METHOD_PROTOTYPE(GrxVScaleNew)
REXX_METHOD_PROTOTYPE(GrxVScaleNewWithRange)
REXX_METHOD_PROTOTYPE(GrxRangeSignalConnect)
REXX_METHOD_PROTOTYPE(GrxImageNew)
REXX_METHOD_PROTOTYPE(GrxImageSetFromFile)
REXX_METHOD_PROTOTYPE(GrxImageSetFromStock)
REXX_METHOD_PROTOTYPE(GrxImageSetFromIconName)
 
 #endif /* REXXGTK_H */
 
