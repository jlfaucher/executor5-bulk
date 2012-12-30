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

#include <shlwapi.h>
#include "APICommon.hpp"
#include "oodCommon.hpp"
#include "oodDeviceGraphics.hpp"
#include "oodMessaging.hpp"
#include "oodResizableDialog.hpp"

/**
 * Validates that the CSelf pointer for a ResizingAdmin object is not null and
 * that the resizeInfo pointer is not null.
 */
inline pCResizingAdmin validateRACSelf(RexxMethodContext *c, void *pcra, pResizeInfoDlg *pprid)
{
    if ( pcra == NULL || ((pCResizingAdmin)pcra)->resizeInfo == NULL)
    {
        baseClassInitializationException(c, "ResizingAdmin");
    }

    *pprid = ((pCResizingAdmin)pcra)->resizeInfo;
    return (pCResizingAdmin)pcra;
}

static pinType_t keyword2pinType(RexxMethodContext *c, CSTRING keyWord, size_t pos, bool isLeftTop)
{
    pinType_t pinType = notAPin;

    if ( isLeftTop )
    {
        if (      StrCmpI(keyWord, "PROPORTIONAL") == 0 ) pinType = proportionalPin;
        else if ( StrCmpI(keyWord, "STATIONARY") == 0   ) pinType = stationaryPin;
        else wrongArgKeywordException(c, pos, PIN_TYPE_SHORT_LIST, keyWord);

        return pinType;
    }

    if ( StrCmpI(keyWord,      "MYLEFT") == 0       ) pinType = myLeftPin;
    else if ( StrCmpI(keyWord, "MYTOP") == 0        ) pinType = myTopPin;
    else if ( StrCmpI(keyWord, "PROPORTIONAL") == 0 ) pinType = proportionalPin;
    else if ( StrCmpI(keyWord, "STATIONARY") == 0   ) pinType = stationaryPin;
    else wrongArgKeywordException(c, pos, PIN_TYPE_LIST, keyWord);

    return pinType;
}

static pinnedEdge_t keyword2pinnedEdge(RexxMethodContext *c, CSTRING keyWord, size_t pos)
{
    pinnedEdge_t edge = notAnEdge;

    if ( StrCmpI(keyWord,      "LEFT") == 0    ) edge = leftEdge;
    else if ( StrCmpI(keyWord, "TOP") == 0     ) edge = topEdge;
    else if ( StrCmpI(keyWord, "RIGHT") == 0   ) edge = rightEdge;
    else if ( StrCmpI(keyWord, "BOTTOM") == 0  ) edge = bottomEdge;
    else if ( StrCmpI(keyWord, "XCENTER") == 0 ) edge = xCenterEdge;
    else if ( StrCmpI(keyWord, "YCENTER") == 0 ) edge = yCenterEdge;
    else wrongArgKeywordException(c, pos, PINNED_EDGE_LIST, keyWord);

    return edge;
}

static RexxObjectPtr defaultLeft(RexxMethodContext *c, CSTRING howPinned, CSTRING whichEdge, pResizeInfoDlg prid)
{
    prid->defEdges.left.pinType   = keyword2pinType(c, howPinned, 1, true);
    prid->defEdges.left.pinToEdge = keyword2pinnedEdge(c, whichEdge, 2);

    if ( prid->defEdges.left.pinType == notAPin || prid->defEdges.left.pinToEdge == notAnEdge )
    {
        return TheOneObj;
    }
    return TheZeroObj;
}

static RexxObjectPtr defaultTop(RexxMethodContext *c, CSTRING howPinned, CSTRING whichEdge, pResizeInfoDlg prid)
{
    prid->defEdges.top.pinType   = keyword2pinType(c, howPinned, 1, true);
    prid->defEdges.top.pinToEdge = keyword2pinnedEdge(c, whichEdge, 2);

    if ( prid->defEdges.top.pinType == notAPin || prid->defEdges.top.pinToEdge == notAnEdge )
    {
        return TheOneObj;
    }
    return TheZeroObj;
}

static RexxObjectPtr defaultRight(RexxMethodContext *c, CSTRING howPinned, CSTRING whichEdge, pResizeInfoDlg prid)
{
    prid->defEdges.right.pinType   = keyword2pinType(c, howPinned, 1, false);
    prid->defEdges.right.pinToEdge = keyword2pinnedEdge(c, whichEdge, 2);

    if ( prid->defEdges.right.pinType == notAPin || prid->defEdges.right.pinToEdge == notAnEdge )
    {
        return TheOneObj;
    }
    return TheZeroObj;
}

static RexxObjectPtr defaultBottom(RexxMethodContext *c, CSTRING howPinned, CSTRING whichEdge, pResizeInfoDlg prid)
{
    prid->defEdges.bottom.pinType   = keyword2pinType(c, howPinned, 1, false);
    prid->defEdges.bottom.pinToEdge = keyword2pinnedEdge(c, whichEdge, 2);

    if ( prid->defEdges.bottom.pinType == notAPin || prid->defEdges.bottom.pinToEdge == notAnEdge )
    {
        return TheOneObj;
    }
    return TheZeroObj;
}

bool allocateResizeInfo(RexxMethodContext *c, pCPlainBaseDialog pcpbd)
{
    printf("In allocateResizeInfo()\n");

    pResizeInfoDlg prid = (pResizeInfoDlg)LocalAlloc(LPTR, sizeof(ResizeInfoDlg));
    if ( prid == NULL )
    {
        goto err_out;
    }

    prid->riCtrls = (pResizeInfoCtrl)LocalAlloc(LPTR, DEFAULT_RESIZABLE_CONTROLS * sizeof(ResizeInfoCtrl));
    if ( prid->riCtrls == NULL )
    {
        goto err_out;
    }
    prid->tableSize = DEFAULT_RESIZABLE_CONTROLS;

    prid->defEdges.left.pinToEdge   = leftEdge;
    prid->defEdges.left.pinType     = proportionalPin;
    prid->defEdges.top.pinToEdge    = topEdge;
    prid->defEdges.top.pinType      = proportionalPin;
    prid->defEdges.right.pinToEdge  = rightEdge;
    prid->defEdges.right.pinType    = proportionalPin;
    prid->defEdges.bottom.pinToEdge = bottomEdge;
    prid->defEdges.bottom.pinType   = proportionalPin;

    pcpbd->resizeInfo = prid;

    RexxObjectPtr reply = c->SendMessage1(pcpbd->rexxSelf, "INITRESIZING", c->NewPointer(pcpbd));
    if ( reply != TheZeroObj )
    {
        goto err_out;
    }

    return true;

err_out:
    outOfMemoryException(c->threadContext);
    if ( prid != NULL )
    {
        safeLocalFree(prid->riCtrls);
    }
    safeLocalFree(prid);
    pcpbd->resizeInfo = NULL;

    return false;
}


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


/** ResizingAdmin::defaultLeft()
 ** ResizingAdmin::defaultTop()
 ** ResizingAdmin::defaultRight()
 ** ResizingAdmin::defaultBottom()
 *
 */
RexxMethod4(RexxObjectPtr, ra_defaultSide, CSTRING, howPinned, CSTRING, whichEdge, NAME, method, CSELF, pCSelf)
{
    pResizeInfoDlg  prid = NULL;
    pCResizingAdmin pcra = validateRACSelf(context, pCSelf, &prid);
    if ( pcra == NULL )
    {
        return TheOneObj;
    }

    if ( ! prid->inDefineSizing )
    {
        return methodCanOnlyBeInvokedException(context, method, "during the defineSizing method", pcra->pcpbd->rexxSelf);
    }

    switch ( method[7] )
    {
        case 'L' : return defaultLeft(context, howPinned, whichEdge, prid);
        case 'T' : return defaultTop(context, howPinned, whichEdge, prid);
        case 'R' : return defaultRight(context, howPinned, whichEdge, prid);
        case 'B' : return defaultBottom(context, howPinned, whichEdge, prid);
    }
    return TheOneObj;
}


RexxMethod5(RexxObjectPtr, ra_defaultSizing, OPTIONAL_RexxArrayObject, left, OPTIONAL_RexxArrayObject, top,
            OPTIONAL_RexxArrayObject, right, OPTIONAL_RexxArrayObject, bottom, CSELF, pCSelf)
{
    RexxMethodContext *c = context;

    pResizeInfoDlg  prid = NULL;
    pCResizingAdmin pcra = validateRACSelf(context, pCSelf, &prid);
    if ( pcra == NULL )
    {
        return TheOneObj;
    }

    if ( ! prid->inDefineSizing )
    {
        return methodCanOnlyBeInvokedException(context, "defaultSizing", "during the defineSizing method", pcra->pcpbd->rexxSelf);
    }

    RexxObjectPtr rxHowPinned;
    RexxObjectPtr rxWhichEdge;

    if ( argumentExists(1) )
    {
        rxHowPinned = context->ArrayAt(left, 1);
        rxWhichEdge = context->ArrayAt(left, 2);
        if ( rxHowPinned == NULLOBJECT )
        {
            return sparseArrayException(context->threadContext, 1, 1);
        }
        if ( rxWhichEdge == NULLOBJECT )
        {
            return sparseArrayException(context->threadContext, 1, 2);
        }

        if ( defaultLeft(context, context->ObjectToStringValue(rxHowPinned), context->ObjectToStringValue(rxWhichEdge), prid) == TheOneObj )
        {
            return NULLOBJECT;
        }
    }

    if ( argumentExists(2) )
    {
        rxHowPinned = context->ArrayAt(top, 1);
        rxWhichEdge = context->ArrayAt(top, 2);
        if ( rxHowPinned == NULLOBJECT )
        {
            return sparseArrayException(context->threadContext, 2, 1);
        }
        if ( rxWhichEdge == NULLOBJECT )
        {
            return sparseArrayException(context->threadContext, 2, 2);
        }

        if ( defaultTop(context, context->ObjectToStringValue(rxHowPinned), context->ObjectToStringValue(rxWhichEdge), prid) == TheOneObj )
        {
            return NULLOBJECT;
        }
    }

    if ( argumentExists(3) )
    {
        rxHowPinned = context->ArrayAt(right, 1);
        rxWhichEdge = context->ArrayAt(right, 2);
        if ( rxHowPinned == NULLOBJECT )
        {
            return sparseArrayException(context->threadContext, 3, 1);
        }
        if ( rxWhichEdge == NULLOBJECT )
        {
            return sparseArrayException(context->threadContext, 3, 2);
        }

        if ( defaultRight(context, context->ObjectToStringValue(rxHowPinned), context->ObjectToStringValue(rxWhichEdge), prid) == TheOneObj )
        {
            return NULLOBJECT;
        }
    }

    if ( argumentExists(4) )
    {
        rxHowPinned = context->ArrayAt(bottom, 1);
        rxWhichEdge = context->ArrayAt(bottom, 2);
        if ( rxHowPinned == NULLOBJECT )
        {
            return sparseArrayException(context->threadContext, 4, 1);
        }
        if ( rxWhichEdge == NULLOBJECT )
        {
            return sparseArrayException(context->threadContext, 4, 2);
        }

        if ( defaultBottom(context, context->ObjectToStringValue(rxHowPinned), context->ObjectToStringValue(rxWhichEdge), prid) == TheOneObj )
        {
            return NULLOBJECT;
        }
    }

    return TheZeroObj;
}


/** ResizingAdmin::initResizing()  [private ??]
 */
RexxMethod2(RexxObjectPtr, ra_initResizing, POINTER, arg, OSELF, self)
{
    pCPlainBaseDialog pcpbd = (pCPlainBaseDialog)arg;
    printf("In ra_initResizing()\n");

    RexxBufferObject praBuffer = context->NewBuffer(sizeof(CResizingAdmin));
    if ( praBuffer == NULLOBJECT )
    {
        return TheOneObj;
    }

    pCResizingAdmin pcra = (pCResizingAdmin)context->BufferData(praBuffer);
    memset(pcra, 0, sizeof(CResizingAdmin));

    pcra->pcpbd      = pcpbd;
    pcra->rexxSelf   = self;
    pcra->resizeInfo = pcpbd->resizeInfo;

    context->SetObjectVariable("CSELF", praBuffer);

    return TheZeroObj;
}


