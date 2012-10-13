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
 * oodToolTipp.cpp
 *
 * Contains methods for the ToolTip control.
 */
#include "ooDialog.hpp"     // Must be first, includes windows.h, commctrl.h, and oorexxapi.h

#include <shlwapi.h>

#include "APICommon.hpp"
#include "oodCommon.hpp"
#include "oodMessaging.hpp"
#include "oodControl.hpp"
#include "oodResources.hpp"


/**
 *  Methods for the .ToolTip class.
 */
#define TOOLTIP_CLASS             "ToolTip"


bool lazyInitToolTipTable(RexxMethodContext *c, pCPlainBaseDialog pcpbd)
{
    pcpbd->ToolTipTab = (TOOLTIPTABLEENTRY *)LocalAlloc(LPTR, sizeof(TOOLTIPTABLEENTRY) * DEF_MAX_TTT_ENTRIES);
    if ( ! pcpbd->ToolTipTab )
    {
        outOfMemoryException(c->threadContext);
        return false;
    }
    pcpbd->TTT_size      = DEF_MAX_TTT_ENTRIES;
    pcpbd->TTT_nextIndex = 0;

    return true;
}

/**
 * Adds a new tool tip entry to the tool tip table, allocating a bigger table if
 * needed.
 *
 * @param c
 * @param pcpbd
 * @param toolTip
 * @param hToolTip
 * @param id
 *
 * @return bool
 */
static bool addToolTipToTable(RexxMethodContext *c, pCPlainBaseDialog pcpbd, RexxObjectPtr toolTip, HWND hToolTip, uint32_t id)
{
    if ( pcpbd->TTT_nextIndex >= pcpbd->TTT_size )
    {
        HLOCAL temp = LocalReAlloc(pcpbd->ToolTipTab, sizeof(TOOLTIPTABLEENTRY) * pcpbd->TTT_size * 2, LMEM_ZEROINIT | LMEM_MOVEABLE);
        if ( temp == NULL )
        {
            char buf[512];
            _snprintf(buf, sizeof(buf), "ToolTip controles have exceeded the maximum\n"
                       "number of allocated table entries, and the table\n"
                       "could not be expanded.\n\n"
                       "  System error code: %d\n\n"
                       "No ToolTip control can be added.", GetLastError());

            MessageBox(NULL, buf, "Error", MB_OK | MB_ICONHAND);
            return false;
        }

        pcpbd->TTT_size *= 2;
        pcpbd->ToolTipTab = (TOOLTIPTABLEENTRY *)temp;
    }

    setWindowPtr(hToolTip, GWLP_USERDATA, (LONG_PTR)toolTip);
    c->SendMessage1(pcpbd->rexxSelf, "PUTCONTROL", toolTip);

    pcpbd->ToolTipTab[pcpbd->TTT_nextIndex].id = id;
    pcpbd->ToolTipTab[pcpbd->TTT_nextIndex++].rexxSelf = toolTip;

    return true;
}


/**
 *  Parses a list of tool tip style keywords and retuns the corresponding style
 *  flag.
 *
 * @param flags
 *
 * @return uint32_t
 *
 * @remarks  flags may be null.
 */
static uint32_t parseToolTipStyle(CSTRING flags)
{
    uint32_t style = WS_POPUP;

    if ( flags == NULL )
    {
        style |=  TTS_NOPREFIX | TTS_ALWAYSTIP;
        goto done_out;
    }

    if ( StrStrI(flags, "") != NULL ) style |= TTS_ALWAYSTIP;
    if ( StrStrI(flags, "") != NULL ) style |= TTS_BALLOON;
    if ( StrStrI(flags, "") != NULL ) style |= TTS_NOANIMATE;
    if ( StrStrI(flags, "") != NULL ) style |= TTS_CLOSE;
    if ( StrStrI(flags, "") != NULL ) style |= TTS_NOFADE;
    if ( StrStrI(flags, "") != NULL ) style |= TTS_NOPREFIX;
    if ( StrStrI(flags, "") != NULL ) style |= TTS_USEVISUALSTYLE;

done_out:
    return style;
}


/**
 * Creates the Windows tool tip control and instantiates the Rexx ToolTip
 * object.
 *
 * @param context
 * @param rxID
 * @param styleFlags
 * @param pcpbd
 *
 * @return The Rexx ToolTip object on success, the .nil object on error.
 *
 * @note  Tool tip controls are different than most other Window controls in
 *        that they are actually popup windows owned by the dialog, rather than
 *        child windows of the dialog.  Because of this we need to keep track of
 *        them by adding them to a table.
 *
 *        Because they are popup windows, we can not find an existing tool tip
 *        through GetDlgItem().  So, we add each created tool tip to a table and
 *        look up an existing tool tip through its ID.
 */
RexxObjectPtr createToolTip(RexxMethodContext *context, RexxObjectPtr rxID, CSTRING styleFlags, pCPlainBaseDialog pcpbd)
{
    oodResetSysErrCode(context->threadContext);

    RexxObjectPtr toolTip = TheNilObj;
    CREATETOOLTIP ctt     = {0};

    uint32_t id = oodResolveSymbolicID(context->threadContext, pcpbd->rexxSelf, rxID, -1, 1, true);
    if ( id == OOD_ID_EXCEPTION  )
    {
        goto out;
    }

    if ( pcpbd->ToolTipTab == NULL && ! lazyInitToolTipTable(context, pcpbd) )
    {
        goto out;
    }

    register size_t i;
    for ( i = 0; i < pcpbd->TTT_nextIndex; i++ )
    {
        if ( pcpbd->ToolTipTab[i].id == id )
        {
            toolTip = pcpbd->ToolTipTab[i].rexxSelf;
            break;
        }
    }

    if ( toolTip != TheNilObj )
    {
        goto out;
    }

    RexxClassObject controlCls = oodClass4controlType(context, winToolTip);
    if ( controlCls == NULLOBJECT )
    {
        goto out;
    }

    uint32_t style = parseToolTipStyle(styleFlags);
    HWND hDlg      = pcpbd->hDlg;
    HWND hToolTip  = NULL;

    // Tool tips need to be created on the same thread as the dialog window procedure.
    if ( isDlgThread(pcpbd) )
    {
        hToolTip = CreateWindowEx(WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL, style, CW_USEDEFAULT, CW_USEDEFAULT,
                                  CW_USEDEFAULT, CW_USEDEFAULT, hDlg, NULL, pcpbd->hInstance, NULL);
        ctt.errRC = GetLastError();
    }
    else
    {
        ctt.hInstance = pcpbd->hInstance;
        ctt.style     = style;
        hToolTip      = (HWND)SendMessage(hDlg, WM_USER_CREATETOOLTIP, (WPARAM)&ctt, 0);
    }

    if ( hToolTip == NULL )
    {
        oodSetSysErrCode(context->threadContext, ctt.errRC);
        goto out;
    }

    SetWindowPos(hToolTip, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

    PNEWCONTROLPARAMS pArgs = (PNEWCONTROLPARAMS)malloc(sizeof(NEWCONTROLPARAMS));
    if ( pArgs == NULL )
    {
        outOfMemoryException(context->threadContext);
        goto out;
    }

    pArgs->isCatDlg    = false;
    pArgs->controlType = winToolTip;
    pArgs->hwnd        = hToolTip;
    pArgs->pcpbd       = pcpbd;
    pArgs->id          = id;

    RexxObjectPtr result = context->SendMessage1(controlCls, "NEW", context->NewPointer(pArgs));
    free(pArgs);

    if ( result != NULLOBJECT && result != TheNilObj )
    {
        if ( addToolTipToTable(context, pcpbd, result, hToolTip, id) )
        {
            toolTip = result;
        }
    }

out:
    return toolTip;
}


/** ToolTip::activate()
 *
 *
 */
RexxMethod2(uint32_t, tt_activate, OPTIONAL_logical_t, activate, CSELF, pCSelf)
{
    pCDialogControl pcdc = validateDCCSelf(context, pCSelf);
    if ( pcdc == NULL )
    {
        return FALSE;
    }

    if ( argumentOmitted(1) )
    {
        activate = TRUE;
    }

    SendMessage(pcdc->hCtrl, TTM_ACTIVATE, activate, 0);

    return 0;
}


/** ToolTip::addTool()
 *
 *
 */
RexxMethod5(logical_t, tt_addTool, RexxObjectPtr, tool, OPTIONAL_CSTRING, flags, OPTIONAL_CSTRING, text,
            OPTIONAL_RexxObjectPtr, userData, CSELF, pCSelf)
{
    pCDialogControl pcdc = validateDCCSelf(context, pCSelf);
    if ( pcdc == NULL )
    {
        return FALSE;
    }

    if ( ! requiredClass(context->threadContext, tool, "DIALOGCONTROL", 1) )
    {
        return FALSE;
    }

    pCDialogControl pcdcTool = controlToCSelf(context, tool);

    TOOLINFO ti = {0};

    ti.cbSize   = sizeof(ti);
    ti.uFlags   = TTF_IDISHWND | TTF_SUBCLASS;
    ti.uId      = (UINT_PTR)pcdcTool->hCtrl;
    ti.hwnd     = pcdc->hDlg;
    ti.lpszText = (LPSTR)text;
    ti.lParam   = (LPARAM)(argumentOmitted(4) ? TheNilObj : userData);

    return SendMessage(pcdc->hCtrl, TTM_ADDTOOL, 0, (LPARAM)&ti);
}


/** ToolTip::addToolEx()
 *
 *
 */
RexxMethod5(logical_t, tt_addToolEx, RexxObjectPtr, tool, OPTIONAL_CSTRING, flags, OPTIONAL_CSTRING, text,
            OPTIONAL_RexxObjectPtr, userData, CSELF, pCSelf)
{
    pCDialogControl pcdc = validateDCCSelf(context, pCSelf);
    if ( pcdc == NULL )
    {
        return FALSE;
    }

    if ( ! requiredClass(context->threadContext, tool, "DIALOGCONTROL", 1) )
    {
        return FALSE;
    }

    pCDialogControl pcdcTool = controlToCSelf(context, tool);

    TOOLINFO ti = {0};

    ti.cbSize   = sizeof(ti);
    ti.uFlags   = TTF_IDISHWND | TTF_SUBCLASS;
    ti.uId      = (UINT_PTR)pcdcTool->hCtrl;
    ti.hwnd     = pcdc->hDlg;
    ti.lpszText = (LPSTR)text;
    ti.lParam   = (LPARAM)(argumentOmitted(4) ? TheNilObj : userData);

    return SendMessage(pcdc->hCtrl, TTM_ADDTOOL, 0, (LPARAM)&ti);
}


/** ToolTip::addToolRect()
 *
 *
 */
RexxMethod5(logical_t, tt_addToolRect, RexxObjectPtr, tool, OPTIONAL_CSTRING, flags, OPTIONAL_CSTRING, text,
            OPTIONAL_RexxObjectPtr, userData, CSELF, pCSelf)
{
    pCDialogControl pcdc = validateDCCSelf(context, pCSelf);
    if ( pcdc == NULL )
    {
        return FALSE;
    }

    if ( ! requiredClass(context->threadContext, tool, "DIALOGCONTROL", 1) )
    {
        return FALSE;
    }

    pCDialogControl pcdcTool = controlToCSelf(context, tool);

    TOOLINFO ti = {0};

    ti.cbSize   = sizeof(ti);
    ti.uFlags   = TTF_IDISHWND | TTF_SUBCLASS;
    ti.uId      = (UINT_PTR)pcdcTool->hCtrl;
    ti.hwnd     = pcdc->hDlg;
    ti.lpszText = (LPSTR)text;
    ti.lParam   = (LPARAM)(argumentOmitted(4) ? TheNilObj : userData);

    return SendMessage(pcdc->hCtrl, TTM_ADDTOOL, 0, (LPARAM)&ti);
}


/** ToolTip::getToolInfo()
 *
 *
 *  @remarks  It seems as though both ti.hwnd and ti.uId must be set to the
 *            values they were set to in addTool, or the get tool info message
 *            fails.  Not sure how to have the user convey this info to this
 *            method ??
 */
RexxMethod2(logical_t, tt_getToolInfo, RexxObjectPtr, toolID, CSELF, pCSelf)
{
    pCDialogControl pcdc = validateDCCSelf(context, pCSelf);
    if ( pcdc == NULL )
    {
        return FALSE;
    }
    RexxMethodContext *c = context;
    uint32_t        resourceID = 0;
    pCDialogControl pcdcTool   = NULL;

    if ( c->IsOfType(toolID, "DIALOGCONTROL") )
    {
        printf("Tool ID is a dialog control\n");
        pcdcTool = controlToCSelf(context, toolID);
    }
    else if ( c->IsOfType(toolID, "PLAINBASEDIALOG") )
    {
        printf("Tool ID is a dialog\n");
    }
    else if ( c->UnsignedInt32(toolID, &resourceID))
    {
        printf("Tool ID is an id=%d\n", resourceID);
    }
    else
    {
        CSTRING str = c->ObjectToStringValue(toolID);
        HWND hwnd = (HWND)string2pointer(str);
        if ( IsWindow(hwnd) )
        {
            printf("Tool ID is a window handle=%p\n", hwnd);

        }
        else
        {
            printf("Tool ID is NOT recognized (%s)\n", str);
        }
    }

    char buf[1024];
    TOOLINFO ti = {0};

    ti.cbSize   = sizeof(ti);
    ti.uId      = (UINT_PTR)pcdcTool->hCtrl;
    ti.hwnd     = pcdcTool->pcpbd->hDlg;
    ti.lpszText = buf;

    if ( SendMessage(pcdc->hCtrl, TTM_GETTOOLINFO, 0, (LPARAM)&ti) )
    {
        printf("Found tool text=%s hinst=%p lParam=%p hwnd=%p uFlags=0x%x uId=%p, rect.top=%d\n",
              ti.lpszText, ti.hinst, ti.lParam, ti.hwnd, ti.uFlags, ti.uId, ti.rect.top);
    }
    else
    {
        printf("TTM_GETTOOLINFO message failed\n");
    }
    return true;
}


/** ToolTip::setMaxTipWidth()
 *
 *
 */
RexxMethod2(int32_t, tt_setMaxTipWidth, int32_t, max, CSELF, pCSelf)
{
    pCDialogControl pcdc = validateDCCSelf(context, pCSelf);
    if ( pcdc == NULL )
    {
        return 0;
    }
    return (int32_t)SendMessage(pcdc->hCtrl, TTM_SETMAXTIPWIDTH, 0, max);
}


