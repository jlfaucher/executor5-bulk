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


#include "rexxgtk.h"


/*----------------------------------------------------------------------------*/
/* Global variables                                                           */
/*----------------------------------------------------------------------------*/

char * rexxgtk_argv[] = {"rexxgtk", "\0"};
int rexxgtk_argc = 1;


/*----------------------------------------------------------------------------*/
/* Local Definitions                                                          */
/*----------------------------------------------------------------------------*/

#define VERSTRING(major,minor,rel) #major "." #minor "." #rel


/*============================================================================*/
/* Private Functions                                                          */
/*============================================================================*/



/*============================================================================*/
/* Public Functions                                                           */
/*============================================================================*/

/**
 * Function:  GrxVersion
 *
 * Return the library version number.
 *
 * @return        Version string
 */
RexxRoutine0(RexxObjectPtr, GrxVersion)
{
    char verstr[20];
    g_snprintf(verstr, sizeof(verstr), "%d.%d.%d", VMAJOR, VMINOR, VREL);
    return context->NewStringFromAsciiz(verstr);
}

/**
 * Function:  GrxInit
 *
 * Initializa the library.
 */
RexxRoutine0(int, GrxInit)
{

    int rc = 0;
    char ** argv = (char **)&rexxgtk_argv;

    g_thread_init(NULL);
    gdk_threads_init();
    gtk_set_locale ();
    gtk_init (&rexxgtk_argc, &argv);
}

/**
 * Function:  GrxEventsPending
 *
 * Return a boolean 1 if there are pending events.
 *
 * @return        1 or zero.
 **/
RexxRoutine0(int, GrxEventsPending)
{

    return (int) gtk_events_pending();
}

/**
 * Function:  GrxMain
 *
 * Executes the GTK main loop.
 *
 * @return        Zero.
 **/
RexxRoutine0(int, GrxMain)
{

    gtk_main();
    return 0;
}

/**
 * Function:  GrxMainQuit
 *
 * Exits the GTK main loop.
 *
 * @return        Zero.
 **/
RexxRoutine0(int, GrxMainQuit)
{

    gtk_main_quit();
    return 0;
}

/**
 * Function:  GrxMainIteration
 *
 * Executes one iteration of the GTK main loop.
 *
 * @return        Zero.
 **/
RexxRoutine0(int, GrxMainIteration)
{

    gtk_main_iteration();
    return 0;
}

/**
 * Routine: GrxMessage
 *
 * Log a message.
 *
 * @param msg     The message
 *
 * @return        Zero.
 **/
RexxRoutine1(int,                      // Return type
             GrxMessage,               // Routine name
             CSTRING, msg)             // Message
{

    g_message(msg);
	
    return 0;
}

/**
 * Routine: GrxWarning
 *
 * Log a warning message.
 *
 * @param msg     The message
 *
 * @return        Zero.
 **/
RexxRoutine1(int,                      // Return type
             GrxWarning,               // Routine name
             CSTRING, msg)             // Message
{

    g_warning(msg);
	
    return 0;
}

/**
 * Routine: GrxCritical
 *
 * Log a critical message.
 *
 * @param msg     The message
 *
 * @return        Zero.
 **/
RexxRoutine1(int,                      // Return type
             GrxCritical,              // Routine name
             CSTRING, msg)             // Message
{

    g_critical(msg);
	
    return 0;
}

/**
 * Routine: GrxError
 *
 * Log an error message.
 *
 * @param msg     The message
 *
 * @return        Zero.
 **/
RexxRoutine1(int,                      // Return type
             GrxError,                 // Routine name
             CSTRING, msg)             // Message
{

    g_error(msg);
	
    return 0;
}

/**
 * Routine: GrxDebug
 *
 * Log a debug message.
 *
 * @param msg     The message
 *
 * @return        Zero.
 **/
RexxRoutine1(int,                      // Return type
             GrxDebug,                 // Routine name
             CSTRING, msg)             // Message
{

    g_debug(msg);
	
    return 0;
}

/**
 * Routine: GrxGetUserName
 *
 * Get the current user name.
 *
 * @return        User name
 **/
RexxRoutine0(CSTRING,                  // Return type
             GrxGetUserName)           // Routine name
{

    return g_get_user_name();
}

/**
 * Routine: GrxGetRealName
 *
 * Get the current real name.
 *
 * @return        Real name
 **/
RexxRoutine0(CSTRING,                  // Return type
             GrxGetRealName)           // Routine name
{

    return g_get_real_name();
}

/**
 * Routine: GrxGetHomeDir
 *
 * Get the user's home directory.
 *
 * @return        Home directory
 **/
RexxRoutine0(CSTRING,                  // Return type
             GrxGetHomeDir)            // Routine name
{

    return g_get_home_dir();
}

/**
 * Routine: GrxGetHostName
 *
 * Get the machine host name.
 *
 * @return        Host name
 **/
RexxRoutine0(CSTRING,                  // Return type
             GrxGetHostName)           // Routine name
{

    return g_get_host_name();
}


// build the actual entry list
RexxRoutineEntry gtkobject_routines[] = {
    REXX_TYPED_ROUTINE(GrxVersion, GrxVersion),
    REXX_TYPED_ROUTINE(GrxInit, GrxInit),
    REXX_TYPED_ROUTINE(GrxEventsPending, GrxEventsPending),
    REXX_TYPED_ROUTINE(GrxMain, GrxMain),
    REXX_TYPED_ROUTINE(GrxMainQuit, GrxMainQuit),
    REXX_TYPED_ROUTINE(GrxMainIteration, GrxMainIteration),
    REXX_TYPED_ROUTINE(GrxMessage, GrxMessage),
    REXX_TYPED_ROUTINE(GrxWarning, GrxWarning),
    REXX_TYPED_ROUTINE(GrxCritical, GrxCritical),
    REXX_TYPED_ROUTINE(GrxError, GrxError),
    REXX_TYPED_ROUTINE(GrxDebug, GrxDebug),
    REXX_TYPED_ROUTINE(GrxGetUserName, GrxGetUserName),
    REXX_TYPED_ROUTINE(GrxGetRealName, GrxGetRealName),
    REXX_TYPED_ROUTINE(GrxGetHomeDir, GrxGetHomeDir),
    REXX_TYPED_ROUTINE(GrxGetHostName, GrxGetHostName),
    REXX_LAST_ROUTINE()
};


// build the actual entry list
RexxMethodEntry gtkobject_methods[] = {
    REXX_METHOD(GrxWidgetNew, GrxWidgetNew),
//    REXX_METHOD(GrxWidgetUninit, GrxWidgetUninit),
    REXX_METHOD(GrxWidgetShow, GrxWidgetShow),
    REXX_METHOD(GrxWidgetShowAll, GrxWidgetShowAll),
    REXX_METHOD(GrxWidgetHide, GrxWidgetHide),
    REXX_METHOD(GrxWidgetHideAll, GrxWidgetHideAll),
    REXX_METHOD(GrxWidgetSetSizeRequest, GrxWidgetSetSizeRequest),
    REXX_METHOD(GrxWidgetDestroy, GrxWidgetDestroy),
    REXX_METHOD(GrxWidgetGrabFocus, GrxWidgetGrabFocus),
    REXX_METHOD(GrxWidgetSetFont, GrxWidgetSetFont),
    REXX_METHOD(GrxWidgetGetFont, GrxWidgetGetFont),
    REXX_METHOD(GrxWidgetSetName, GrxWidgetSetName),
    REXX_METHOD(GrxWidgetGetName, GrxWidgetGetName),
    REXX_METHOD(GrxWidgetSetSensitive, GrxWidgetSetSensitive),
    REXX_METHOD(GrxWidgetModifyBG, GrxWidgetModifyBG),
    REXX_METHOD(GrxWidgetModifyFG, GrxWidgetModifyFG),
    REXX_METHOD(GrxWidgetSetData, GrxWidgetSetData),
    REXX_METHOD(GrxWidgetGetData, GrxWidgetGetData),
    REXX_METHOD(GrxWidgetActivate, GrxWidgetActivate),
    REXX_METHOD(GrxWidgetReparent, GrxWidgetReparent),
    REXX_METHOD(GrxWidgetIsFocus, GrxWidgetIsFocus),
    REXX_METHOD(GrxWidgetGrabDefault, GrxWidgetGrabDefault),
    REXX_METHOD(GrxWidgetSetParentWindow, GrxWidgetSetParentWindow),
    REXX_METHOD(GrxWidgetGetParentWindow, GrxWidgetGetParentWindow),
    REXX_METHOD(GrxWidgetGetToplevel, GrxWidgetGetToplevel),
    REXX_METHOD(GrxWidgetIsAncestor, GrxWidgetIsAncestor),
    REXX_METHOD(GrxWidgetGetAncestor, GrxWidgetGetAncestor),
    REXX_METHOD(GrxWidgetGetDirection, GrxWidgetGetDirection),
    REXX_METHOD(GrxWidgetSetDirection, GrxWidgetSetDirection),
    REXX_METHOD(GrxWidgetGetDefaultDirection, GrxWidgetGetDefaultDirection),
    REXX_METHOD(GrxWidgetSetDefaultDirection, GrxWidgetSetDefaultDirection),
    REXX_METHOD(GrxWidgetQueueDrawArea, GrxWidgetQueueDrawArea),
    REXX_METHOD(GrxWidgetResetShapes, GrxWidgetResetShapes),
    REXX_METHOD(GrxWidgetSetScrollAdjustments, GrxWidgetSetScrollAdjustments),
    REXX_METHOD(GrxWidgetMnemonicActivate, GrxWidgetMnemonicActivate),
    REXX_METHOD(GrxWidgetGetAccessible, GrxWidgetGetAccessible),
    REXX_METHOD(GrxWidgetGetChildVisible, GrxWidgetGetChildVisible),
    REXX_METHOD(GrxWidgetGetParent, GrxWidgetGetParent),
    REXX_METHOD(GrxWidgetGetMenus, GrxWidgetGetMenus),
    REXX_METHOD(GrxWidgetSetEvents, GrxWidgetSetEvents),
    REXX_METHOD(GrxWidgetAddAccelerator, GrxWidgetAddAccelerator),
    REXX_METHOD(GrxWidgetRemoveAccelerator, GrxWidgetRemoveAccelerator),
    REXX_METHOD(GrxWidgetSetAccelPath, GrxWidgetSetAccelPath),
    REXX_METHOD(GrxWidgetRealize, GrxWidgetRealize),
    REXX_METHOD(GrxWidgetUnrealize, GrxWidgetUnrealize),
    REXX_METHOD(GrxWidgetSignalConnect, GrxWidgetSignalConnect),
    REXX_METHOD(GrxWindowNew, GrxWindowNew),
    REXX_METHOD(GrxWindowGetTitle, GrxWindowGetTitle),
    REXX_METHOD(GrxWindowSetTitle, GrxWindowSetTitle),
    REXX_METHOD(GrxWindowGetModal, GrxWindowGetModal),
    REXX_METHOD(GrxWindowSetModal, GrxWindowSetModal),
    REXX_METHOD(GrxWindowAddAccelGroup, GrxWindowAddAccelGroup),
    REXX_METHOD(GrxWindowSignalConnect, GrxWindowSignalConnect),
    REXX_METHOD(GrxContainerAdd, GrxContainerAdd),
    REXX_METHOD(GrxContainerRemove, GrxContainerRemove),
    REXX_METHOD(GrxContainerSetBorderWidth, GrxContainerSetBorderWidth),
    REXX_METHOD(GrxContainerSignalConnect, GrxContainerSignalConnect),
    REXX_METHOD(GrxBoxPackStart, GrxBoxPackStart),
    REXX_METHOD(GrxBoxPackEnd, GrxBoxPackEnd),
    REXX_METHOD(GrxVBoxNew, GrxVBoxNew),
    REXX_METHOD(GrxVBoxNewFromPtr, GrxVBoxNewFromPtr),
    REXX_METHOD(GrxHBoxNew, GrxHBoxNew),
    REXX_METHOD(GrxHBoxNewFromPtr, GrxHBoxNewFromPtr),
    REXX_METHOD(GrxAlignmentNew, GrxAlignmentNew),
    REXX_METHOD(GrxViewportNew, GrxViewportNew),
    REXX_METHOD(GrxViewportSignalConnect, GrxViewportSignalConnect),
    REXX_METHOD(GrxPanedAdd1, GrxPanedAdd1),
    REXX_METHOD(GrxPanedAdd2, GrxPanedAdd2),
    REXX_METHOD(GrxPanedPack1, GrxPanedPack1),
    REXX_METHOD(GrxPanedPack2, GrxPanedPack2),
    REXX_METHOD(GrxVPanedNew, GrxVPanedNew),
    REXX_METHOD(GrxHPanedNew, GrxHPanedNew),
    REXX_METHOD(GrxButtonNew, GrxButtonNew),
    REXX_METHOD(GrxButtonSetUseStock, GrxButtonSetUseStock),
    REXX_METHOD(GrxButtonSetLabel, GrxButtonSetLabel),
    REXX_METHOD(GrxButtonGetLabel, GrxButtonGetLabel),
    REXX_METHOD(GrxButtonSetRelief, GrxButtonSetRelief),
    REXX_METHOD(GrxButtonUseUnderline, GrxButtonUseUnderline),
    REXX_METHOD(GrxButtonSignalConnect, GrxButtonSignalConnect),
    REXX_METHOD(GrxToggleButtonNew, GrxToggleButtonNew),
    REXX_METHOD(GrxToggleButtonGetMode, GrxToggleButtonGetMode),
    REXX_METHOD(GrxToggleButtonSetMode, GrxToggleButtonSetMode),
    REXX_METHOD(GrxToggleButtonGetActive, GrxToggleButtonGetActive),
    REXX_METHOD(GrxToggleButtonSetActive, GrxToggleButtonSetActive),
    REXX_METHOD(GrxToggleButtonGetInconsistent, GrxToggleButtonGetInconsistent),
    REXX_METHOD(GrxToggleButtonSetInconsistent, GrxToggleButtonSetInconsistent),
    REXX_METHOD(GrxToggleButtonSignalConnect, GrxToggleButtonSignalConnect),
    REXX_METHOD(GrxCheckButtonNew, GrxCheckButtonNew),
    REXX_METHOD(GrxRadioButtonNew, GrxRadioButtonNew),
    REXX_METHOD(GrxRadioButtonSignalConnect, GrxRadioButtonSignalConnect),
    REXX_METHOD(GrxColorButtonNew, GrxColorButtonNew),
    REXX_METHOD(GrxColorButtonSetTitle, GrxColorButtonSetTitle),
    REXX_METHOD(GrxColorButtonGetColor, GrxColorButtonGetColor),
    REXX_METHOD(GrxColorButtonSetColor, GrxColorButtonSetTitle),
    REXX_METHOD(GrxColorButtonSignalConnect, GrxColorButtonSignalConnect),
    REXX_METHOD(GrxFileChooserButtonNew, GrxFileChooserButtonNew),
    REXX_METHOD(GrxFileChooserButtonSignalConnect, GrxFileChooserButtonSignalConnect),
    REXX_METHOD(GrxFontButtonNew, GrxFontButtonNew),
    REXX_METHOD(GrxFontButtonSetTitle, GrxFontButtonSetTitle),
    REXX_METHOD(GrxFontButtonGetFontName, GrxFontButtonGetFontName),
    REXX_METHOD(GrxFontButtonSignalConnect, GrxFontButtonSignalConnect),
    REXX_METHOD(GrxTableNew, GrxTableNew),
    REXX_METHOD(GrxTableAttach, GrxTableAttach),
    REXX_METHOD(GrxTableSetRowSpacings, GrxTableSetRowSpacings),
    REXX_METHOD(GrxTableSetColSpacings, GrxTableSetColSpacings),
    REXX_METHOD(GrxComboBoxNew, GrxComboBoxNew),
    REXX_METHOD(GrxComboBoxNewText, GrxComboBoxNewText),
    REXX_METHOD(GrxComboBoxAppendText, GrxComboBoxAppendText),
    REXX_METHOD(GrxComboBoxGetActive, GrxComboBoxGetActive),
    REXX_METHOD(GrxComboBoxSetActive, GrxComboBoxSetActive),
    REXX_METHOD(GrxComboBoxGetActiveText, GrxComboBoxGetActiveText),
    REXX_METHOD(GrxComboBoxSignalConnect, GrxComboBoxSignalConnect),
    REXX_METHOD(GrxDialogNewWithButtons, GrxDialogNewWithButtons),
    REXX_METHOD(GrxDialogAddButton, GrxDialogAddButton),
    REXX_METHOD(GrxDialogSetHasSeparator, GrxDialogSetHasSeparator),
    REXX_METHOD(GrxDialogSetDefaultResponse, GrxDialogSetDefaultResponse),
    REXX_METHOD(GrxDialogRun, GrxDialogRun),
    REXX_METHOD(GrxDialogSignalConnect, GrxDialogSignalConnect),
    REXX_METHOD(GrxMessageDialogNew, GrxMessageDialogNew),
    REXX_METHOD(GrxFileChooserDialogNew, GrxFileChooserDialogNew),
    REXX_METHOD(GrxFontSelectionDialogNew, GrxFontSelectionDialogNew),
    REXX_METHOD(GrxFontSelectionDialogSetFontName, GrxFontSelectionDialogSetFontName),
    REXX_METHOD(GrxFontSelectionDialogGetFontName, GrxFontSelectionDialogGetFontName),
    REXX_METHOD(GrxFontSelectionDialogSetpreviewText, GrxFontSelectionDialogSetPreviewText),
    REXX_METHOD(GrxAboutDialogNew, GrxAboutDialogNew),
    REXX_METHOD(GrxAboutDialogSetName, GrxAboutDialogSetName),
    REXX_METHOD(GrxAboutDialogSetVersion, GrxAboutDialogSetVersion),
    REXX_METHOD(GrxAboutDialogSetCopyright, GrxAboutDialogSetCopyright),
    REXX_METHOD(GrxAboutDialogSetComments, GrxAboutDialogSetComments),
    REXX_METHOD(GrxAboutDialogSetLicense, GrxAboutDialogSetLicense),
    REXX_METHOD(GrxAboutDialogSetWrapLicense, GrxAboutDialogSetWrapLicense),
    REXX_METHOD(GrxAboutDialogSetWebsite, GrxAboutDialogSetWebsite),
    REXX_METHOD(GrxAboutDialogSetWebsiteLabel, GrxAboutDialogSetWebsiteLabel),
    REXX_METHOD(GrxAboutDialogSetAuthors, GrxAboutDialogSetAuthors),
    REXX_METHOD(GrxAboutDialogSetArtists, GrxAboutDialogSetArtists),
    REXX_METHOD(GrxAboutDialogSetDocumentors, GrxAboutDialogSetDocumentors),
    REXX_METHOD(GrxAboutDialogSetLogo, GrxAboutDialogSetLogo),
    REXX_METHOD(GrxColorSelectionDialogNew, GrxColorSelectionDialogNew),
    REXX_METHOD(GrxColorSelectionDialogGetColor, GrxColorSelectionDialogGetColor),
    REXX_METHOD(GrxEntryNew, GrxEntryNew),
    REXX_METHOD(GrxEntrySetMaxLength, GrxEntrySetMaxLength),
    REXX_METHOD(GrxEntryGetMaxLength, GrxEntryGetMaxLength),
    REXX_METHOD(GrxEntrySetText, GrxEntrySetText),
    REXX_METHOD(GrxEntryGetText, GrxEntryGetText),
    REXX_METHOD(GrxEntrySetWidthChars, GrxEntrySetWidthChars),
    REXX_METHOD(GrxEntrySetVisibility, GrxEntrySetVisibility),
    REXX_METHOD(GrxEntryGetVisibility, GrxEntryGetVisibility),
    REXX_METHOD(GrxEntryGetInvisibleChar, GrxEntryGetInvisibleChar),
    REXX_METHOD(GrxEntrySetInvisibleChar, GrxEntrySetInvisibleChar),
    REXX_METHOD(GrxEntrySignalConnect, GrxEntrySignalConnect),
    REXX_METHOD(GrxSpinButtonNew, GrxSpinButtonNew),
    REXX_METHOD(GrxSpinButtonNewWithRange, GrxSpinButtonNewWithRange),
    REXX_METHOD(GrxSpinButtonSignalConnect, GrxSpinButtonSignalConnect),
    REXX_METHOD(GrxAssistantNew, GrxAssistantNew),
    REXX_METHOD(GrxAssistantSetCurrentPage, GrxAssistantSetCurrentPage),
    REXX_METHOD(GrxAssistantGetCurrentPage, GrxAssistantGetCurrentPage),
    REXX_METHOD(GrxAssistantGetNthPage, GrxAssistantGetNthPage),
    REXX_METHOD(GrxAssistantPrependPage, GrxAssistantPrependPage),
    REXX_METHOD(GrxAssistantAppendPage, GrxAssistantAppendPage),
    REXX_METHOD(GrxAssistantInsertPage, GrxAssistantInsertPage),
    REXX_METHOD(GrxAssistantSetPageType, GrxAssistantSetPageType),
    REXX_METHOD(GrxAssistantSetPageTitle, GrxAssistantSetPageTitle),
    REXX_METHOD(GrxAssistantSetPageComplete, GrxAssistantSetPageComplete),
    REXX_METHOD(GrxAssistantSetPageHeaderImage, GrxAssistantSetPageHeaderImage),
    REXX_METHOD(GrxAssistantSetPageSideImage, GrxAssistantSetPageSideImage),
    REXX_METHOD(GrxAssistantSignalConnect, GrxAssistantSignalConnect),
    REXX_METHOD(GrxLabelNew, GrxLabelNew),
    REXX_METHOD(GrxLabelSetLabel, GrxLabelSetLabel),
    REXX_METHOD(GrxLabelGetLabel, GrxLabelGetLabel),
    REXX_METHOD(GrxLabelSetJustify, GrxLabelSetJustify),
    REXX_METHOD(GrxLabelGetJustify, GrxLabelGetJustify),
    REXX_METHOD(GrxLabelSetSelectable, GrxLabelSetSelectable),
    REXX_METHOD(GrxLabelSignalConnect, GrxLabelSignalConnect),
    REXX_METHOD(GrxFixedNew, GrxFixedNew),
    REXX_METHOD(GrxFixedPut, GrxFixedPut),
    REXX_METHOD(GrxFixedMove, GrxFixedMove),
    REXX_METHOD(GrxExpanderNew, GrxExpanderNew),
    REXX_METHOD(GrxExpanderSetExpanded, GrxExpanderSetExpanded),
    REXX_METHOD(GrxExpanderSetLabel, GrxExpanderSetLabel),
    REXX_METHOD(GrxExpanderGetLabel, GrxExpanderGetLabel),
    REXX_METHOD(GrxExpanderSetUseUnderline, GrxExpanderSetUseUnderline),
    REXX_METHOD(GrxExpanderSetSpacing, GrxExpanderSetSpacing),
    REXX_METHOD(GrxExpanderSignalConnect, GrxExpanderSignalConnect),
    REXX_METHOD(GrxHandleBoxNew, GrxHandleBoxNew),
    REXX_METHOD(GrxHandleBoxSetShadowType, GrxHandleBoxSetShadowType),
    REXX_METHOD(GrxHandleBoxSetPosition, GrxHandleBoxSetPosition),
    REXX_METHOD(GrxHandleBoxSetSnapEdge, GrxHandleBoxSetSnapEdge),
    REXX_METHOD(GrxHandleBoxSignalConnect, GrxHandleBoxSignalConnect),
    REXX_METHOD(GrxNotebookNew, GrxNotebookNew),
    REXX_METHOD(GrxNotebookAppendPage, GrxNotebookAppendPage),
    REXX_METHOD(GrxNotebookPrependPage, GrxNotebookPrependPage),
    REXX_METHOD(GrxNotebookInsertPage, GrxNotebookInsertPage),
    REXX_METHOD(GrxNotebookAppendPageMenu, GrxNotebookAppendPageMenu),
    REXX_METHOD(GrxNotebookPrependPageMenu, GrxNotebookPrependPageMenu),
    REXX_METHOD(GrxNotebookInsertPageMenu, GrxNotebookInsertPageMenu),
    REXX_METHOD(GrxNotebookRemovePage, GrxNotebookRemovePage),
    REXX_METHOD(GrxNotebookSetShowBorder, GrxNotebookSetShowBorder),
    REXX_METHOD(GrxNotebookSetShowTabs, GrxNotebookSetShowTabs),
    REXX_METHOD(GrxNotebookSetTabPos, GrxNotebookSetTabPos),
    REXX_METHOD(GrxNotebookSetHomogeneousTabs, GrxNotebookSetHomogeneousTabs),
    REXX_METHOD(GrxNotebookSetTabBorder, GrxNotebookSetTabBorder),
    REXX_METHOD(GrxNotebookSetScrollable, GrxNotebookSetScrollable),
    REXX_METHOD(GrxNotebookPopupEnable, GrxNotebookPopupEnable),
    REXX_METHOD(GrxNotebookPopupDisable, GrxNotebookPopupDisable),
    REXX_METHOD(GrxNotebookGetCurrentPage, GrxNotebookGetCurrentPage),
    REXX_METHOD(GrxNotebookSetCurrentPage, GrxNotebookSetCurrentPage),
    REXX_METHOD(GrxNotebookSignalConnect, GrxNotebookSignalConnect),
    REXX_METHOD(GrxScaleSetDigits, GrxScaleSetDigits),
    REXX_METHOD(GrxScaleGetDigits, GrxScaleGetDigits),
    REXX_METHOD(GrxScaleSetValuePos, GrxScaleSetValuePos),
    REXX_METHOD(GrxScaleGetValuePos, GrxScaleGetValuePos),
    REXX_METHOD(GrxHScaleNew, GrxHScaleNew),
    REXX_METHOD(GrxHScaleNewWithRange, GrxHScaleNewWithRange),
    REXX_METHOD(GrxVScaleNew, GrxVScaleNew),
    REXX_METHOD(GrxVScaleNewWithRange, GrxVScaleNewWithRange),
    REXX_METHOD(GrxRangeSignalConnect, GrxRangeSignalConnect),
    REXX_METHOD(GrxImageNew, GrxImageNew),
    REXX_METHOD(GrxImageSetFromFile, GrxImageSetFromFile),
    REXX_METHOD(GrxImageSetFromStock, GrxImageSetFromStock),
    REXX_METHOD(GrxImageSetFromIconName, GrxImageSetFromIconName),
    REXX_METHOD(GrxProgressBarNew, GrxProgressBarNew),
    REXX_METHOD(GrxProgressBarSetFraction, GrxProgressBarSetFraction),
    REXX_METHOD(GrxProgressBarSetPulseStep, GrxProgressBarSetPulseStep),
    REXX_METHOD(GrxProgressBarPulse, GrxProgressBarPulse),
    REXX_METHOD(GrxProgressBarSetText, GrxProgressBarSetText),
    REXX_METHOD(GrxTextViewNew, GrxTextViewNew),
    REXX_METHOD(GrxTextViewGetBuffer, GrxTextViewGetBuffer),
    REXX_METHOD(GrxTextViewSetWrapMode, GrxTextViewSetWrapMode),
    REXX_METHOD(GrxTextViewGetWrapMode, GrxTextViewGetWrapMode),
    REXX_METHOD(GrxTextViewSetJustification, GrxTextViewSetJustification),
    REXX_METHOD(GrxTextViewGetJustification, GrxTextViewGetJustification),
    REXX_METHOD(GrxTextViewSetEditable, GrxTextViewSetEditable),
    REXX_METHOD(GrxTextViewGetEditable, GrxTextViewGetEditable),
    REXX_METHOD(GrxTextViewSetCursorVisible, GrxTextViewSetCursorVisible),
    REXX_METHOD(GrxTextViewGetCursorVisible, GrxTextViewGetCursorVisible),
    REXX_METHOD(GrxTextViewSetPixelsAboveLines, GrxTextViewSetPixelsAboveLines),
    REXX_METHOD(GrxTextViewGetPixelsAboveLines, GrxTextViewGetPixelsAboveLines),
    REXX_METHOD(GrxTextViewSetPixelsBelowLines, GrxTextViewSetPixelsBelowLines),
    REXX_METHOD(GrxTextViewGetPixelsBelowLines, GrxTextViewGetPixelsBelowLines),
    REXX_METHOD(GrxTextViewSetPixelsInsideWrap, GrxTextViewSetPixelsInsideWrap),
    REXX_METHOD(GrxTextViewGetPixelsInsideWrap, GrxTextViewGetPixelsInsideWrap),
    REXX_METHOD(GrxTextViewSetLeftMargin, GrxTextViewSetLeftMargin),
    REXX_METHOD(GrxTextViewGetLeftMargin, GrxTextViewGetLeftMargin),
    REXX_METHOD(GrxTextViewSetRightMargin, GrxTextViewSetRightMargin),
    REXX_METHOD(GrxTextViewGetRightMargin, GrxTextViewGetRightMargin),
    REXX_METHOD(GrxTextViewAddChildAtOffset, GrxTextViewAddChildAtOffset),
    REXX_METHOD(GrxTextViewSignalConnect, GrxTextViewSignalConnect),
    REXX_METHOD(GrxTextBufferNew, GrxTextBufferNew),
    REXX_METHOD(GrxTextBufferSetText, GrxTextBufferSetText),
    REXX_METHOD(GrxTextBufferInsertText, GrxTextBufferInsertText),
    REXX_METHOD(GrxTextBufferGetText, GrxTextBufferGetText),
    REXX_METHOD(GrxTextBufferCutClipboard, GrxTextBufferCutClipboard),
    REXX_METHOD(GrxTextBufferCopyClipboard, GrxTextBufferCopyClipboard),
    REXX_METHOD(GrxTextBufferPasteClipboard, GrxTextBufferPasteClipboard),
    REXX_METHOD(GrxTextBufferForwardSearch, GrxTextBufferForwardSearch),
    REXX_METHOD(GrxTextBufferForwardSearchNext, GrxTextBufferForwardSearchNext),
    REXX_METHOD(GrxTextBufferCreateTagInt, GrxTextBufferCreateTagInt),
    REXX_METHOD(GrxTextBufferCreateTagFloat, GrxTextBufferCreateTagFloat),
    REXX_METHOD(GrxTextBufferApplyTagByName, GrxTextBufferApplyTagByName),
    REXX_METHOD(GrxTextBufferRemoveAllTags, GrxTextBufferRemoveAllTags),
    REXX_METHOD(GrxTextBufferInsertImage, GrxTextBufferInsertImage),
    REXX_METHOD(GrxTextBufferSignalConnect, GrxTextBufferSignalConnect),
    REXX_METHOD(GrxGSListNew, GrxGSListNew),
    REXX_METHOD(GrxGSListUninit, GrxGSListUninit),
    REXX_METHOD(GrxGSListAppend, GrxGSListAppend),
    REXX_METHOD(GrxGSListPrepend, GrxGSListPrepend),
    REXX_METHOD(GrxGSListInsert, GrxGSListInsert),
    REXX_METHOD(GrxGSListItem, GrxGSListItem),
    REXX_METHOD(GrxGSListFirst, GrxGSListFirst),
    REXX_METHOD(GrxGSListNext, GrxGSListNext),
    REXX_METHOD(GrxGListNew, GrxGListNew),
    REXX_METHOD(GrxGListUninit, GrxGListUninit),
    REXX_METHOD(GrxGListAppend, GrxGListAppend),
    REXX_METHOD(GrxGListPrepend, GrxGListPrepend),
    REXX_METHOD(GrxGListInsert, GrxGListInsert),
    REXX_METHOD(GrxGListItem, GrxGListItem),
    REXX_METHOD(GrxGListFirst, GrxGListFirst),
    REXX_METHOD(GrxGListNext, GrxGListNext),
    REXX_METHOD(GrxFileChooserSetCurrentFolder, GrxFileChooserSetCurrentFolder),
    REXX_METHOD(GrxFileChooserSetSelectMultiple, GrxFileChooserSetSelectMultiple),
    REXX_METHOD(GrxFileChooserAddFilter, GrxFileChooserAddFilter),
    REXX_METHOD(GrxFileChooserGetFilename, GrxFileChooserGetFilename),
    REXX_METHOD(GrxFileChooserGetFilenames, GrxFileChooserGetFilenames),
    REXX_METHOD(GrxFileChooserSignalConnect, GrxFileChooserSignalConnect),
    REXX_METHOD(GrxFileFilterNew, GrxFileFilterNew),
    REXX_METHOD(GrxFileFilterSetName, GrxFileFilterSetName),
    REXX_METHOD(GrxFileFilterAddPattern, GrxFileFilterAddPattern),
    REXX_METHOD(GrxClipboardNew, GrxClipboardNew),
    REXX_METHOD(GrxAdjustmentNew, GrxAdjustmentNew),
    REXX_METHOD(GrxAdjustmentNewFromPtr, GrxAdjustmentNewFromPtr),
    REXX_METHOD(GrxAdjustmentGetValue, GrxAdjustmentGetValue),
    REXX_METHOD(GrxAdjustmentSetValue, GrxAdjustmentSetValue),
    REXX_METHOD(GrxAdjustmentClampPage, GrxAdjustmentClampPage),
    REXX_METHOD(GrxAdjustmentSignalConnect, GrxAdjustmentSignalConnect),
    REXX_METHOD(GrxScrolledWindowNew, GrxScrolledWindowNew),
    REXX_METHOD(GrxScrolledWindowGetHAdjustment, GrxScrolledWindowGetHAdjustment),
    REXX_METHOD(GrxScrolledWindowGetVAdjustment, GrxScrolledWindowGetVAdjustment),
    REXX_METHOD(GrxScrolledWindowSetPolicy, GrxScrolledWindowSetPolicy),
    REXX_METHOD(GrxScrolledWindowAddWithViewport, GrxScrolledWindowAddWithViewport),
    REXX_METHOD(GrxScrolledWindowSignalConnect, GrxScrolledWindowSignalConnect),
    REXX_METHOD(GrxListStoreNew, GrxListStoreNew),
    REXX_METHOD(GrxListStoreAppend, GrxListStoreAppend),
    REXX_METHOD(GrxListStoreSetValue, GrxListStoreSetValue),
    REXX_METHOD(GrxEventBoxNew, GrxEventBoxNew),
    REXX_METHOD(GrxEventBoxSetAboveChild, GrxEventBoxSetAboveChild),
    REXX_METHOD(GrxEventBoxGetAboveChild, GrxEventBoxGetAboveChild),
    REXX_METHOD(GrxEventBoxSetVisibleWindow, GrxEventBoxSetVisibleWindow),
    REXX_METHOD(GrxEventBoxGetVisibleWindow, GrxEventBoxGetVisibleWindow),
    REXX_METHOD(GrxMenuShellAppend, GrxMenuShellAppend),
    REXX_METHOD(GrxMenuShellPrepend, GrxMenuShellPrepend),
    REXX_METHOD(GrxMenuShellInsert, GrxMenuShellInsert),
    REXX_METHOD(GrxMenuShellDeactivate, GrxMenuShellDeactivate),
    REXX_METHOD(GrxMenuShellSelectItem, GrxMenuShellSelectItem),
    REXX_METHOD(GrxMenuShellSelectFirst, GrxMenuShellSelectFirst),
    REXX_METHOD(GrxMenuShellDeselect, GrxMenuShellDeselect),
    REXX_METHOD(GrxMenuShellActivateItem, GrxMenuShellActivateItem),
    REXX_METHOD(GrxMenuShellCancel, GrxMenuShellCancel),
    REXX_METHOD(GrxMenuShellSetTakeFocus, GrxMenuShellSetTakeFocus),
    REXX_METHOD(GrxMenuShellGetTakeFocus, GrxMenuShellGetTakeFocus),
    REXX_METHOD(GrxMenuShellSignalConnect, GrxMenuShellSignalConnect),
    REXX_METHOD(GrxMenuNew, GrxMenuNew),
    REXX_METHOD(GrxMenuReorderChild, GrxMenuReorderChild),
    REXX_METHOD(GrxMenuAttach, GrxMenuAttach),
    REXX_METHOD(GrxMenuPopup, GrxMenuPopup),
    REXX_METHOD(GrxMenuSetAccelGroup, GrxMenuSetAccelGroup),
    REXX_METHOD(GrxMenuGetAccelGroup, GrxMenuGetAccelGroup),
    REXX_METHOD(GrxMenuSetAccelPath, GrxMenuSetAccelPath),
    REXX_METHOD(GrxMenuSetTitle, GrxMenuSetTitle),
    REXX_METHOD(GrxMenuGetTitle, GrxMenuGetTitle),
    REXX_METHOD(GrxMenuSetTearoffState, GrxMenuSetTearoffState),
    REXX_METHOD(GrxMenuGetTearoffState, GrxMenuGetTearoffState),
    REXX_METHOD(GrxMenuPopdown, GrxMenuPopdown),
    REXX_METHOD(GrxMenuReposition, GrxMenuReposition),
    REXX_METHOD(GrxMenuSetActive, GrxMenuSetActive),
    REXX_METHOD(GrxMenuGetActive, GrxMenuGetActive),
    REXX_METHOD(GrxMenuAttachToWidget, GrxMenuAttachToWidget),
    REXX_METHOD(GrxMenuDetach, GrxMenuDetach),
    REXX_METHOD(GrxMenuGetAttachWidget, GrxMenuGetAttachWidget),
    REXX_METHOD(GrxMenuSignalConnect, GrxMenuSignalConnect),
    REXX_METHOD(GrxMenuItemNew, GrxMenuItemNew),
    REXX_METHOD(GrxMenuItemSetRightJustified, GrxMenuItemSetRightJustified),
    REXX_METHOD(GrxMenuItemGetRightJustified, GrxMenuItemGetRightJustified),
    REXX_METHOD(GrxMenuItemSetSubmenu, GrxMenuItemSetSubmenu),
    REXX_METHOD(GrxMenuItemRemoveSubmenu, GrxMenuItemRemoveSubmenu),
    REXX_METHOD(GrxMenuItemSetAccelPath, GrxMenuItemSetAccelPath),
    REXX_METHOD(GrxMenuItemSelect, GrxMenuItemSelect),
    REXX_METHOD(GrxMenuItemDeselect, GrxMenuItemDeselect),
    REXX_METHOD(GrxMenuItemSignalConnect, GrxMenuItemSignalConnect),
    REXX_METHOD(GrxImageMenuItemNew, GrxImageMenuItemNew),
    REXX_METHOD(GrxMenuBarNew, GrxMenuBarNew),
    REXX_METHOD(GrxMenuBarAppend, GrxMenuBarAppend),
    REXX_METHOD(GrxMenuBarPrepend, GrxMenuBarPrepend),
    REXX_METHOD(GrxMenuBarInsert, GrxMenuBarInsert),
    REXX_METHOD(GrxMenuBarSetPackDirection, GrxMenuBarSetPackDirection),
    REXX_METHOD(GrxMenuBarGetPackDirection, GrxMenuBarGetPackDirection),
    REXX_METHOD(GrxMenuBarSetChildPackDirection, GrxMenuBarSetChildPackDirection),
    REXX_METHOD(GrxMenuBarGetChildPackDirection, GrxMenuBarGetChildPackDirection),
    REXX_METHOD(GrxSeparatorMenuItemNew, GrxSeparatorMenuItemNew),
    REXX_METHOD(GrxAccelGroupNew, GrxAccelGroupNew),
    REXX_METHOD(GrxAccelGroupSignalConnect, GrxAccelGroupSignalConnect),
    REXX_METHOD(GrxStatusbarNew, GrxStatusbarNew),
    REXX_METHOD(GrxStatusbarGetContextId, GrxStatusbarGetContextId),
    REXX_METHOD(GrxStatusbarPush, GrxStatusbarPush),
    REXX_METHOD(GrxStatusbarPop, GrxStatusbarPop),
    REXX_METHOD(GrxStatusbarRemove, GrxStatusbarRemove),
    REXX_METHOD(GrxStatusbarSetHasResizeGrip, GrxStatusbarSetHasResizeGrip),
    REXX_METHOD(GrxStatusbarGetHasResizeGrip, GrxStatusbarGetHasResizeGrip),
    REXX_METHOD(GrxStatusbarSignalConnect, GrxStatusbarSignalConnect),
    REXX_METHOD(GrxToolbarNew, GrxToolbarNew),
    REXX_METHOD(GrxToolbarInsert, GrxToolbarInsert),
    REXX_METHOD(GrxToolbarGetItemIndex, GrxToolbarGetItemIndex),
    REXX_METHOD(GrxToolbarGetNItems, GrxToolbarGetNItems),
    REXX_METHOD(GrxToolbarGetNthItem, GrxToolbarGetNthItem),
    REXX_METHOD(GrxToolbarGetDropIndex, GrxToolbarGetDropIndex),
    REXX_METHOD(GrxToolbarSetDropHighlightItem, GrxToolbarSetDropHighlightItem),
    REXX_METHOD(GrxToolbarSetShowArrow, GrxToolbarSetShowArrow),
    REXX_METHOD(GrxToolbarSetOrientation, GrxToolbarSetOrientation),
    REXX_METHOD(GrxToolbarSetTooltips, GrxToolbarSetTooltips),
    REXX_METHOD(GrxToolbarUnsetIconSize, GrxToolbarUnsetIconSize),
    REXX_METHOD(GrxToolbarGetShowArrow, GrxToolbarGetShowArrow),
    REXX_METHOD(GrxToolbarGetOrientation, GrxToolbarGetOrientation),
    REXX_METHOD(GrxToolbarGetStyle, GrxToolbarGetStyle),
    REXX_METHOD(GrxToolbarGetIconSize, GrxToolbarGetIconSize),
    REXX_METHOD(GrxToolbarGetTooltips, GrxToolbarGetTooltips),
    REXX_METHOD(GrxToolbarGetReliefStyle, GrxToolbarGetReliefStyle),
    REXX_METHOD(GrxToolbarAppendItem, GrxToolbarAppendItem),
    REXX_METHOD(GrxToolbarPrependItem, GrxToolbarPrependItem),
    REXX_METHOD(GrxToolbarInsertItem, GrxToolbarInsertItem),
    REXX_METHOD(GrxToolbarAppendSpace, GrxToolbarAppendSpace),
    REXX_METHOD(GrxToolbarPrependSpace, GrxToolbarPrependSpace),
    REXX_METHOD(GrxToolbarInsertSpace, GrxToolbarInsertSpace),
    REXX_METHOD(GrxToolbarAppendElement, GrxToolbarAppendElement),
    REXX_METHOD(GrxToolbarPrependElement, GrxToolbarPrependElement),
    REXX_METHOD(GrxToolbarInsertElement, GrxToolbarInsertElement),
    REXX_METHOD(GrxToolbarAppendWidget, GrxToolbarAppendWidget),
    REXX_METHOD(GrxToolbarPrependWidget, GrxToolbarPrependWidget),
    REXX_METHOD(GrxToolbarInsertWidget, GrxToolbarInsertWidget),
    REXX_METHOD(GrxToolbarSetStyle, GrxToolbarSetStyle),
    REXX_METHOD(GrxToolbarInsertStock, GrxToolbarInsertStock),
    REXX_METHOD(GrxToolbarSetIconSize, GrxToolbarSetIconSize),
    REXX_METHOD(GrxToolbarRemoveSpace, GrxToolbarRemoveSpace),
    REXX_METHOD(GrxToolbarUnsetStyle, GrxToolbarUnsetStyle),
    REXX_METHOD(GrxToolbarSignalConnect, GrxToolbarSignalConnect),
    REXX_METHOD(GrxToolItemNew, GrxToolItemNew),
    REXX_METHOD(GrxToolItemSetHomogeneous, GrxToolItemSetHomogeneous),
    REXX_METHOD(GrxToolItemGetHomogeneous, GrxToolItemGetHomogeneous),
    REXX_METHOD(GrxToolItemSetExpand, GrxToolItemSetExpand),
    REXX_METHOD(GrxToolItemGetExpand, GrxToolItemGetExpand),
    REXX_METHOD(GrxToolItemSetTooltip, GrxToolItemSetTooltip),
    REXX_METHOD(GrxToolItemSetUseDragWindow, GrxToolItemSetUseDragWindow),
    REXX_METHOD(GrxToolItemGetUseDragWindow, GrxToolItemGetUseDragWindow),
    REXX_METHOD(GrxToolItemSetVisibleHorizontal, GrxToolItemSetVisibleHorizontal),
    REXX_METHOD(GrxToolItemGetVisibleHorizontal, GrxToolItemGetVisibleHorizontal),
    REXX_METHOD(GrxToolItemSetVisibleVertical, GrxToolItemSetVisibleVertical),
    REXX_METHOD(GrxToolItemGetVisibleVertical, GrxToolItemGetVisibleVertical),
    REXX_METHOD(GrxToolItemSetIsImportant, GrxToolItemSetIsImportant),
    REXX_METHOD(GrxToolItemGetIsImportant, GrxToolItemGetIsImportant),
    REXX_METHOD(GrxToolItemGetIconSize, GrxToolItemGetIconSize),
    REXX_METHOD(GrxToolItemGetOrientation, GrxToolItemGetOrientation),
    REXX_METHOD(GrxToolItemGetToolbarStyle, GrxToolItemGetToolbarStyle),
    REXX_METHOD(GrxToolItemGetReliefStyle, GrxToolItemGetReliefStyle),
    REXX_METHOD(GrxToolItemRetrieveProxyMenuItem, GrxToolItemRetrieveProxyMenuItem),
    REXX_METHOD(GrxToolItemGetProxyMenuItem, GrxToolItemGetProxyMenuItem),
    REXX_METHOD(GrxToolItemSetProxyMenuItem, GrxToolItemSetProxyMenuItem),
    REXX_METHOD(GrxToolItemRebuildMenu, GrxToolItemRebuildMenu),
    REXX_METHOD(GrxToolItemSignalConnect, GrxToolItemSignalConnect),
    REXX_METHOD(GrxToolButtonNew, GrxToolButtonNew),
    REXX_METHOD(GrxToolButtonNewFromStock, GrxToolButtonNewFromStock),
    REXX_METHOD(GrxToolButtonSetLabel, GrxToolButtonSetLabel),
    REXX_METHOD(GrxToolButtonGetLabel, GrxToolButtonGetLabel),
    REXX_METHOD(GrxToolButtonSetUseUnderline, GrxToolButtonSetUseUnderline),
    REXX_METHOD(GrxToolButtonGetUseUnderline, GrxToolButtonGetUseUnderline),
    REXX_METHOD(GrxToolButtonSetStockId, GrxToolButtonSetStockId),
    REXX_METHOD(GrxToolButtonGetStockId, GrxToolButtonGetStockId),
    REXX_METHOD(GrxToolButtonSetIconName, GrxToolButtonSetIconName),
    REXX_METHOD(GrxToolButtonGetIconName, GrxToolButtonGetIconName),
    REXX_METHOD(GrxToolButtonSetIconWidget, GrxToolButtonSetIconWidget),
    REXX_METHOD(GrxToolButtonGetIconWidget, GrxToolButtonGetIconWidget),
    REXX_METHOD(GrxToolButtonSetLabelWidget, GrxToolButtonSetLabelWidget),
    REXX_METHOD(GrxToolButtonGetLabelWidget, GrxToolButtonGetLabelWidget),
    REXX_METHOD(GrxToolButtonSignalConnect, GrxToolButtonSignalConnect),
    REXX_METHOD(GrxSeparatorToolItemNew, GrxSeparatorToolItemNew),
    REXX_METHOD(GrxSeparatorToolItemSetDraw, GrxSeparatorToolItemSetDraw),
    REXX_METHOD(GrxSeparatorToolItemGetDraw, GrxSeparatorToolItemGetDraw),
    REXX_METHOD(GrxMenuToolButtonNew, GrxMenuToolButtonNew),
    REXX_METHOD(GrxMenuToolButtonSetMenu, GrxMenuToolButtonSetMenu),
    REXX_METHOD(GrxMenuToolButtonGetMenu, GrxMenuToolButtonGetMenu),
    REXX_METHOD(GrxMenuToolButtonSignalConnect, GrxMenuToolButtonSignalConnect),
    REXX_METHOD(GrxCellRendererSetFixedSize, GrxCellRendererSetFixedSize),
    REXX_METHOD(GrxCellRendererSignalConnect, GrxCellRendererSignalConnect),
    REXX_METHOD(GrxCellRendererTextNew, GrxCellRendererTextNew),
    REXX_METHOD(GrxCellRendererTextSignalConnect, GrxCellRendererTextSignalConnect),
    REXX_METHOD(GrxCellRendererPixbufNew, GrxCellRendererPixbufNew),
    REXX_METHOD(GrxCellRendererProgressNew, GrxCellRendererProgressNew),
    REXX_METHOD(GrxCellRendererToggleNew, GrxCellRendererToggleNew),
    REXX_METHOD(GrxCellRendererToggleSetRadio, GrxCellRendererToggleSetRadio),
    REXX_METHOD(GrxCellRendererToggleGetRadio, GrxCellRendererToggleGetRadio),
    REXX_METHOD(GrxCellRendererToggleSetActive, GrxCellRendererToggleSetActive),
    REXX_METHOD(GrxCellRendererToggleGetActive, GrxCellRendererToggleGetActive),
    REXX_METHOD(GrxCellRendererToggleSignalConnect, GrxCellRendererToggleSignalConnect),
    REXX_METHOD(GrxCellRendererAccelNew, GrxCellRendererAccelNew),
    REXX_METHOD(GrxCellRendererAccelSignalConnect, GrxCellRendererAccelSignalConnect),
    REXX_METHOD(GrxCellRendererComboNew, GrxCellRendererComboNew),
    REXX_METHOD(GrxCellRendererComboSignalConnect, GrxCellRendererComboSignalConnect),
    REXX_METHOD(GrxTreeViewNew, GrxTreeViewNew),
    REXX_METHOD(GrxTreeViewAppendColumn, GrxTreeViewAppendColumn),
    REXX_METHOD(GrxTreeViewInsertColumn, GrxTreeViewInsertColumn),
    REXX_METHOD(GrxTreeViewRemoveColumn, GrxTreeViewRemoveColumn),
    REXX_METHOD(GrxTreeViewGetModel, GrxTreeViewGetModel),
    REXX_METHOD(GrxTreeViewSetModel, GrxTreeViewSetModel),
    REXX_METHOD(GrxTreeViewExpandAll, GrxTreeViewExpandAll),
    REXX_METHOD(GrxTreeViewSignalConnect, GrxTreeViewSignalConnect),
    REXX_METHOD(GrxTreeViewColumnNew, GrxTreeViewColumnNew),
    REXX_METHOD(GrxTreeViewColumnSetTitle, GrxTreeViewColumnSetTitle),
    REXX_METHOD(GrxTreeViewColumnGetTitle, GrxTreeViewColumnGetTitle),
    REXX_METHOD(GrxTreeViewColumnPackStart, GrxTreeViewColumnPackStart),
    REXX_METHOD(GrxTreeViewColumnPackEnd, GrxTreeViewColumnPackEnd),
    REXX_METHOD(GrxTreeViewColumnSetAttribute, GrxTreeViewColumnSetAttribute),
    REXX_METHOD(GrxTreeViewColumnSignalConnect, GrxTreeViewColumnSignalConnect),
    REXX_METHOD(GrxTreeStoreNew, GrxTreeStoreNew),
    REXX_METHOD(GrxTreeStoreAppend, GrxTreeStoreAppend),
    REXX_METHOD(GrxTreeStoreSetValue, GrxTreeStoreSetValue),
    REXX_METHOD(GrxTreePathNew, GrxTreePathNew),
    REXX_METHOD(GrxTreePathToString, GrxTreePathToString),
    REXX_METHOD(GrxTreePathUp, GrxTreePathUp),
    REXX_METHOD(GrxTreePathDown, GrxTreePathDown),
    REXX_METHOD(GrxTreePathPrevious, GrxTreePathPrevious),
    REXX_METHOD(GrxTreePathNext, GrxTreePathNext),
    REXX_METHOD(GrxTreePathAppendIndex, GrxTreePathAppendIndex),
    REXX_METHOD(GrxTreePathPrependIndex, GrxTreePathPrependIndex),
    REXX_METHOD(GrxTreePathGetDepth, GrxTreePathGetDepth),
    REXX_METHOD(GrxTreePathIsAncestor, GrxTreePathIsAncestor),
    REXX_METHOD(GrxTreePathIsDescendant, GrxTreePathIsDescendant),
    REXX_METHOD(GrxTreeRowReferenceNew, GrxTreeRowReferenceNew),
    REXX_METHOD(GrxTreeRowReferenceValid, GrxTreeRowReferenceValid),
    REXX_METHOD(GrxTreeModelNewFromPtr, GrxTreeModelNewFromPtr),
    REXX_METHOD(GrxTreeModelGetNColumns, GrxTreeModelGetNColumns),
    REXX_METHOD(GrxTreeModelGetColumnType, GrxTreeModelGetColumnType),
    REXX_LAST_METHOD()
};


RexxPackageEntry gtkobject_package_entry = {
    STANDARD_PACKAGE_HEADER
    REXX_INTERPRETER_4_0_0,              // anything after 4.0.0 will work
    "RexxGTK",                           // name of the package
    VERSTRING(VMAJOR,VMINOR,VREL),       // package information
    NULL,                                // no load/unload functions
    NULL,
    gtkobject_routines,                  // the exported routines
    gtkobject_methods                    // the exported methods
};

// package loading stub.
OOREXX_GET_PACKAGE(gtkobject);
