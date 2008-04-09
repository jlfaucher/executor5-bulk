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

#define VERSTRING(major,minor,rel) #major "." #minor "." #rel




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

/**
 * Function:  GrxVersion
 *
 * Return the library version number.
 *
 * @return        Version string
 */
RexxRoutine0(RexxObjectPtr, GrxVersion)
{

    return context->NewStringFromAsciiz(VERSTRING(VMAJOR,VMINOR,VREL));
}

/**
 * Function:  GrxEventsPending
 *
 * Return a boolean 1 if there are pending events.
 *
 * @return        1 or zero.
 */
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
 */
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
 */
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
 */
RexxRoutine0(int, GrxMainIteration)
{

    gtk_main_iteration();
    return 0;
}


// build the actual entry list
RexxRoutineEntry gtkobject_routines[] = {
    REXX_TYPED_ROUTINE(GrxVersion, GrxVersion),
    REXX_TYPED_ROUTINE(GrxEventsPending, GrxEventsPending),
    REXX_TYPED_ROUTINE(GrxMain, GrxMain),
    REXX_TYPED_ROUTINE(GrxMainQuit, GrxMainQuit),
    REXX_TYPED_ROUTINE(GrxMainIteration, GrxMainIteration),
    REXX_LAST_ROUTINE()
};


// build the actual entry list
RexxMethodEntry gtkobject_methods[] = {
    REXX_METHOD(GrxWidgetNew, GrxWidgetNew),
    REXX_METHOD(GrxWidgetUninit, GrxWidgetUninit),
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
    REXX_METHOD(GrxWidgetSignalConnect, GrxWidgetSignalConnect),
    REXX_METHOD(GrxWindowNew, GrxWindowNew),
    REXX_METHOD(GrxWindowGetTitle, GrxWindowGetTitle),
    REXX_METHOD(GrxWindowSetTitle, GrxWindowSetTitle),
    REXX_METHOD(GrxWindowSignalConnect, GrxWindowSignalConnect),
    REXX_METHOD(GrxContainerAdd, GrxContainerAdd),
    REXX_METHOD(GrxContainerRemove, GrxContainerRemove),
    REXX_METHOD(GrxContainerSetBorderWidth, GrxContainerSetBorderWidth),
    REXX_METHOD(GrxContainerPackStart, GrxContainerPackStart),
    REXX_METHOD(GrxContainerPackEnd, GrxContainerPackEnd),
    REXX_METHOD(GrxContainerSignalConnect, GrxContainerSignalConnect),
    REXX_METHOD(GrxVBoxNew, GrxVBoxNew),
    REXX_METHOD(GrxHBoxNew, GrxHBoxNew),
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
    REXX_METHOD(GrxButtonNewFromStock, GrxButtonNewFromStock),
    REXX_METHOD(GrxButtonSetLabel, GrxButtonSetLabel),
    REXX_METHOD(GrxButtonGetLabel, GrxButtonGetLabel),
    REXX_METHOD(GrxButtonSetRelief, GrxButtonSetRelief),
    REXX_METHOD(GrxButtonSetUnderline, GrxButtonSetUnderline),
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
    REXX_METHOD(GrxRadioButtonNewFromWidget, GrxRadioButtonNewFromWidget),
    REXX_METHOD(GrxRadioButtonGetGroup, GrxRadioButtonGetGroup),
    REXX_METHOD(GrxRadioButtonSetGroup, GrxRadioButtonSetGroup),
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
    REXX_LAST_METHOD()
};


RexxPackageEntry gtkobject_package_entry = {
    STANDARD_PACKAGE_HEADER
    "RexxGTK",                           // name of the package
//    "0.1",                               // package information
    VERSTRING(VMAJOR,VMINOR,VREL),       // package information
    NULL,                                // no load/unload functions
    NULL,
    gtkobject_routines,                  // the exported routines
    gtkobject_methods                    // the exported methods
};

// package loading stub.
OOREXX_GET_PACKAGE(gtkobject);
