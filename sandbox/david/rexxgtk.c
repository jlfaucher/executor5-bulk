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

char * rexxgtk_argv[] = {"gtxrxdlg", "\0"};
int rexxgtk_argc = 1;


/*----------------------------------------------------------------------------*/
/* Local Definitions                                                          */
/*----------------------------------------------------------------------------*/

#if defined WIN32
BOOL WINAPI DllMain(HANDLE hinst, DWORD dwcallpurpose, LPVOID lpvResvd);
#else
int _init(void) __attribute__((constructor));
int _fini(void) __attribute__((destructor));
#endif


/*============================================================================*/
/* Private Functions                                                          */
/*============================================================================*/



/*============================================================================*/
/* Public Functions                                                           */
/*============================================================================*/


/*----------------------------------------------------------------------------*/
/* _init / DllMain - Module initialization routine                            */
/*----------------------------------------------------------------------------*/

#if defined WIN32

BOOL WINAPI DllMain(HANDLE hinst, DWORD dwcallpurpose, LPVOID lpvResvd)
{
    char ** argv = (char **)&rexxgtk_argv;

    if (dwcallpurpose == DLL_PROCESS_ATTACH) {
            gtk_set_locale ();
            gtk_init (&rexxgtk_argc, &argv);
    }

    return TRUE;
}

#else

int _init(void)
{
    int rc = 0;
    char ** argv = (char **)&rexxgtk_argv;

    g_thread_init(NULL);
    gdk_threads_init();
    gtk_set_locale ();
    gtk_init (&rexxgtk_argc, &argv);
//    grxInitEventQueue();

    return rc;
}

#endif


/*----------------------------------------------------------------------------*/
/* _fini - Module finialization routine                                       */
/*----------------------------------------------------------------------------*/

#if !defined WIN32

int _fini(void)
{

    return 0;
}

#endif


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxLoadFuncs                                       */
/* Description: Register all functions in this library.                       */
/* Rexx Args:   None                                                          */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxLoadFuncs(char * Name,
                             const long Argc, const RXSTRING Argv[],
                             const char * Queuename, PRXSTRING Retstr)
{
    static const char *EntryPoint[] =
    {
        // Some general GTK functions
        "GrxVersion",
        "GrxMain",
        "GrxMainIteration",
        "GrxValue2ConstantString",
        "GrxConstantString2Value",

        // RexxGTK specific functions
        "GrxSetRexxQueueName",

        // GLib functions
        "GrxGetUserName",
        "GrxGetRealName",
        "GrxGetHomeDir",
        "GrxGetHostName",
        "GrxGSListUninit",
        "GrxGSListAppend",
        "GrxGSListPrepend",
        "GrxGSListInsert",
        "GrxGSListItem",
        "GrxGSListNext",
        "GrxGListUninit",
        "GrxGListAppend",
        "GrxGListPrepend",
        "GrxGListInsert",
        "GrxGListItem",
        "GrxGListNext",

        // Widget functions
        "GrxWidgetShow",
        "GrxWidgetShowAll",
        "GrxWidgetHide",
        "GrxWidgetHideAll",
        "GrxWidgetSetSize",
        "GrxWidgetDestroy",
        "GrxWidgetGrabFocus",
        "GrxWidgetSetFont",
        "GrxWidgetGetFont",
        "GrxWidgetGetAccessible",
        "GrxWidgetGetName",
        "GrxWidgetSetName",
        "GrxWidgetSetSensitive",
        "GrxWidgetConnectSignal",
        "GrxWidgetGetTopLevel",
        "GrxWidgetModifyBG",
        "GrxWidgetModifyFG",

        // Dialog functions
        "GrxDialogNew",
        "GrxDialogNewWithButtons",
        "GrxDialogRun",
        "GrxDialogAddButton",
        "GrxDialogSetHasSeparator",
        "GrxDialogSetDefaultResponse",
        "GrxDialogConnectSignal",
        "GrxMessageDialogNew",
        "GrxFontSelectionDialog",
        "GrxAboutDialogNew",
        "GrxAboutDialogSetProgramName",
        "GrxAboutDialogSetVersion",
        "GrxAboutDialogSetCopyright",
        "GrxAboutDialogSetComments",
        "GrxAboutDialogSetLicense",
        "GrxAboutDialogSetWrapLicense",
        "GrxAboutDialogSetWebsite",
        "GrxAboutDialogSetWebsiteLabel",
        "GrxAboutDialogSetAuthors",
        "GrxAboutDialogSetArtists",
        "GrxAboutDialogSetDocumenters",
        "GrxAboutDialogSetLogo",
        "GrxFileChooserDialogNew",

        // Button functions
        "GrxButtonNew",
        "GrxButtonSetLabel",
        "GrxButtonGetLabel",
        "GrxButtonUseUnderline",
        "GrxButtonNewFromStock",
        "GrxButtonConnectSignal",
        "GrxToggleButtonNew",
        "GrxToggleButtonGetMode",
        "GrxToggleButtonSetMode",
        "GrxToggleButtonGetActive",
        "GrxToggleButtonSetActive",
        "GrxToggleButtonGetInconsistent",
        "GrxToggleButtonSetInconsistent",
        "GrxToggleButtonConnectSignal",
        "GrxCheckButtonNew",
        "GrxRadioButtonNew",
        "GrxRadioButtonNewFromWidget",
        "GrxRadioButtonGetGroup",
        "GrxRadioButtonSetGroup",
        "GrxRadioButtonConnectSignal",
        "GrxColorButtonNew",
        "GrxColorButtonNewWithColor",
        "GrxColorButtonSetTitle",
        "GrxColorButtonGetColor",
        "GrxColorButtonSetColor",
        "GrxColorButtonConnectSignal",
        "GrxFileChooserButtonNew",
        "GrxFileChooserButtonConnectSignal",
        "GrxFontButtonNew",
        "GrxFontButtonSetTitle",
        "GrxFontButtonGetFontName",
        "GrxFontButtonConnectSignal",
        "GrxSpinButtonNew",
        "GrxSpinButtonNewWithRange",
        "GrxSpinButtonConnectSignal",

        // Container functions
        "GrxContainerAdd",
        "GrxContainerRemove",
        "GrxContainerSetBorderWidth",
        "GrxContainerConnectSignal",
        "GrxBoxPackStart",
        "GrxBoxPackEnd",
        "GrxVBoxNew",
        "GrxHBoxNew",
        "GrxPanedAdd1",
        "GrxPanedAdd2",
        "GrxPanedPack1",
        "GrxPanedPack2",
        "GrxHPanedNew",
        "GrxVPanedNew",
        "GrxTableNew",
        "GrxTableAttach",
        "GrxTableSetRowSpacings",
        "GrxTableSetColSpacings",
        "GrxFixedNew",
        "GrxFixedPut",
        "GrxFixedMove",
        "GrxExpanderNew",
        "GrxExpanderSetExpanded",
        "GrxExpanderSetLabel",
        "GrxExpanderGetLabel",
        "GrxExpanderUseUnderline",
        "GrxExpanderSetSpacing",
        "GrxExpanderConnectSignal",
        "GrxHandleBoxNew",
        "GrxHandleBoxSetShadowType",
        "GrxHandleBoxSetHandlePosition",
        "GrxHandleBoxSetSnapEdge",
        "GrxHandleBoxConnectSignal",
//      "GrxTextViewNew",
//      "GrxTextViewSetText",
//      "GrxTextViewGetText",
//      "GrxTextViewSetEditable",
//      "GrxTextViewSetWrapMode",
//      "GrxFrameNew",
//      "GrxScrolledWindowNew",
//      "GrxScrolledWindowSetPolicy",

        //Entry functions
        "GrxEntryNew",
        "GrxEntrySetMaxLength",
        "GrxEntryGetMaxLength",
        "GrxEntrySetText",
        "GrxEntryGetText",
        "GrxEntryGetVisibility",
        "GrxEntrySetVisibility",
        "GrxEntryGetInvisibleChar",
        "GrxEntrySetInvisibleChar",
        "GrxEntrySetWidthChars",

        // Label functions
        "GrxLabelNew",
        "GrxLabelSetText",
        "GrxLabelSetJustify",
        "GrxLabelSetSelectable",
        "GrxLabelConnectSignal",
//      "GrxHSeparatorNew",
//      "GrxVSeparatorNew",
//      "GrxComboNew",
//      "GrxComboSetPopdownStrings",
//      "GrxComboGetSelected",
//      "GrxComboSetValueInList",
//      "GrxImageNew",
//
//      "GrxAccessGetName",
//      "GrxAccessSetName",
//      "GrxAccessGetDescription",
//      "GrxAccessSetDescription",
//      "GrxAccessGetLayer",
//      "GrxAccessGetRole",
//      "GrxAccessGetRoleDescription",
//      "GrxAccessGetLocalRoleDescription",

        // Notebook functions
        "GrxNotebookNew",
        "GrxNotebookAppendPage",
        "GrxNotebookPrependPage",
        "GrxNotebookInsertPage",
        "GrxNotebookAppendPageMenu",
        "GrxNotebookPrependPageMenu",
        "GrxNotebookInsertPageMenu",
        "GrxNotebookRemovePage",
        "GrxNotebookSetShowBorder",
        "GrxNotebookSetShowTabs",
        "GrxNotebookSetTabPos",
        "GrxNotebookSetHomogeneousTabs",
        "GrxNotebookSetTabBorder",
        "GrxNotebookSetScrollable",
        "GrxNotebookPopupEnable",
        "GrxNotebookPopupDisable",
        "GrxNotebookGetCurrentPage",
        "GrxNotebookSetCurrentPage",

        // Window functions
        "GrxWindowNew",
        "GrxWindowSetTitle",
        "GrxWindowSetModal",
        "GrxWindowConnectSignal",

        // Range, Scale, and Slider functions
        "GrxRangeConnectSignal",
        "GrxScaleSetDigits",
        "GrxScaleGetDigits",
        "GrxScaleSetValuePos",
        "GrxScaleGetValuePos",
        "GrxHScaleNew",
        "GrxHScaleNewWithRange",
        "GrxVScaleNew",
        "GrxVScaleNewWithRange",

        // Image functions
        "GrxImageNew",
        "GrxImageSetFromFile",
        "GrxImageSetFromStock",
        "GrxImageSetFromIconName",

        // Interface Classes
        "GrxFileChooserSetCurrentFolder",
        "GrxFileChooserAddFilter",
        "GrxFileChooserGetFilename",
        "GrxFileChooserGetFilenames",
        "GrxFileChooserSetSelectMultiple",
        "GrxFileChooserConnectSignal",

        // Filter filter functions
        "GrxFileFilterNew",
        "GrxFileFilterSetName",
        "GrxFileFilterAddPattern",

        // Event box functions (not used)
        "GrxEventBoxNew",
        "GrxEventBoxSetAboveChild",
        "GrxEventBoxGetAboveChild",
        "GrxEventBoxSetVisibleWindow",
        "GrxEventBoxGetVisibleWindow",

    };

    int f;

    unsigned long retc = 0;

    /* Check for valid arguments */
    if (GrxCheckArgs(0, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    for(f=0; f< (sizeof(EntryPoint)/sizeof(const char *)); f++)
    {
       retc |= RexxRegisterFunctionDll((char *) EntryPoint[f], "rexxgtk", (char *) EntryPoint[f]);
    }

    g_snprintf(Retstr->strptr, RXAUTOBUFLEN, "%u", (unsigned int) retc);
    Retstr->strlength = strlen(Retstr->strptr);
    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxVersion                                         */
/* Description: Return the version string for the library.                    */
/* Rexx Args:   None                                                          */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxVersion(const char * Name,
                           const long Argc, const RXSTRING Argv[],
                           const char * Queuename, PRXSTRING Retstr)
{

    /* Check for valid arguments */
    if (GrxCheckArgs(0, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    g_snprintf(Retstr->strptr, RXAUTOBUFLEN, "%d.%d.%d", VMAJOR, VMINOR, VREL);
    Retstr->strlength = strlen(Retstr->strptr);
    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Internal Function: GrxCheckArgs                                            */
/* Description: Check for valid Rexx arguments.                               */
/* Rexx Args:   Number of expected arguments                                  */
/*              Number of real arguments                                      */
/*              The argument array                                            */
/*----------------------------------------------------------------------------*/

int GrxCheckArgs(
    const size_t expected_argc,      // number of expected arguments
    const size_t argc,               // actual number of arguments
    const RXSTRING argv[]) {         // the arguments
    size_t i;

    if (expected_argc != argc) {
        return 1;
    }
    for (i = 0; i < argc; i++) {
        if (!RXVALIDSTRING(argv[i])) {
            return 1;
        }
    }
    return 0;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxMain                                            */
/* Description: The GTK main loop.                                            */
/* Rexx Args:   None                                                          */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxMain(const char * Name,
                        const long Argc, const RXSTRING Argv[],
                        const char * Queuename, PRXSTRING Retstr)
{
    if (GrxCheckArgs(0, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }
    gtk_main();

    Retstr->strlength = 0;
    return 0;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxMainIteration                                   */
/* Description: The GTK main iteration loop.                                  */
/* Rexx Args:   None                                                          */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxMainIteration(const char * Name,
                                 const long Argc, const RXSTRING Argv[],
                                 const char * Queuename, PRXSTRING Retstr)
{
    if (GrxCheckArgs(0, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }
    gtk_main_iteration();
    Retstr->strlength = 0;
    return 0;
}

