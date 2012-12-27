/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2012-2012 Rexx Language Association. All rights reserved.    */
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
/*----------------------------------------------------------------------------*/

/**
 * Open Object Rexx ooDialog - Resizable dialog funtionality.
 */
#include "ooDialog.hpp"     // Must be first, includes windows.h, commctrl.h, and oorexxapi.h

//#include <stdio.h>
//#include <dlgs.h>
//#include <shlwapi.h>
//#include <WindowsX.h>
#include "APICommon.hpp"
#include "oodCommon.hpp"
//#include "oodControl.hpp"
#include "oodDeviceGraphics.hpp"
#include "oodMessaging.hpp"
//#include "oodMouse.hpp"
//#include "oodData.hpp"
//#include "oodPropertySheetDialog.hpp"

LRESULT CALLBACK RexxResizableDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if ( uMsg == WM_INITDIALOG )
    {
        pCPlainBaseDialog pcpbd = (pCPlainBaseDialog)lParam;
        if ( pcpbd == NULL )
        {
            // Theoretically impossible.  But ... if it should happen, abort.
            return endDialogPremature(pcpbd, hDlg, NoPCPBDpased);
        }

        if ( pcpbd->dlgProcContext == NULL )
        {
            RexxThreadContext *context;
            if ( ! pcpbd->interpreter->AttachThread(&context) )
            {
                // Again, this shouldn't happen ... but
                return endDialogPremature(pcpbd, hDlg, NoThreadAttach);
            }
            pcpbd->dlgProcContext = context;

            RexxSetProcessMessages(FALSE);
        }

        pcpbd->hDlg = hDlg;
        setWindowPtr(hDlg, GWLP_USERDATA, (LONG_PTR)pcpbd);

        if ( pcpbd->isCustomDrawDlg && pcpbd->idsNotChecked )
        {
            // We don't care what the outcome of this is, customDrawCheckIDs
            // will take care of aborting this dialog if the IDs are bad.
            customDrawCheckIDs(pcpbd);
        }

        printf("In InitDialog of resizable dialog, going to enumerate windows\n");

        return TRUE;
    }

    pCPlainBaseDialog pcpbd = (pCPlainBaseDialog)getWindowPtr(hDlg, GWLP_USERDATA);
    if ( pcpbd == NULL )
    {
        // A number of messages arrive before WM_INITDIALOG, we just ignore them.
        return FALSE;
    }

    if ( pcpbd->dlgProcContext == NULL )
    {
        if ( ! pcpbd->isActive )
        {
            return FALSE;
        }

        // Once again, theoretically impossible ...
        return endDialogPremature(pcpbd, hDlg, NoThreadContext);
    }

    if ( uMsg == WM_DESTROY )
    {
        // Under all normal circumstances, WM_DESTROY never gets here.  But if
        // it does, it is because of some unexplained / unanticpated error.
        // PostQuitMessage() will cause the window message loop to quit and
        // things should then (hopefully) unwind cleanly.
        PostQuitMessage(3);
        return TRUE;
    }

    bool msgEnabled = IsWindowEnabled(hDlg) ? true : false;

    // Do not search message table for WM_PAINT to improve redraw.
    if ( msgEnabled && uMsg != WM_PAINT && uMsg != WM_NCPAINT )
    {
        MsgReplyType searchReply = searchMessageTables(uMsg, wParam, lParam, pcpbd);
        if ( searchReply != ContinueProcessing )
        {
            // Note pre 4.0.1, we always returned FALSE, (pass on to the system
            // to process.) But, post 4.0.1 we sometimes reply TRUE, the message
            // has been handled.
            return (searchReply == ReplyTrue ? TRUE : FALSE);
        }
    }

    if ( uMsg >= WM_USER_REXX_FIRST && uMsg <= WM_USER_REXX_LAST )
    {
        return handleWmUser(pcpbd, hDlg, uMsg, wParam, lParam, false);
    }

    switch ( uMsg )
    {
        case WM_PAINT:
            return drawBackgroundBmp(pcpbd, hDlg);

        case WM_DRAWITEM:
            return drawBitmapButton(pcpbd, lParam, msgEnabled);

        case WM_CTLCOLORDLG:
            return handleDlgColor(pcpbd);

        case WM_CTLCOLORSTATIC:
        case WM_CTLCOLORBTN:
        case WM_CTLCOLOREDIT:
        case WM_CTLCOLORLISTBOX:
        case WM_CTLCOLORMSGBOX:
        case WM_CTLCOLORSCROLLBAR:
            return handleCtlColor(pcpbd, hDlg, uMsg, wParam, lParam);

        case WM_QUERYNEWPALETTE:
        case WM_PALETTECHANGED:
            return paletteMessage(pcpbd, hDlg, uMsg, wParam, lParam);

        default:
            break;
    }

    return FALSE;
}


