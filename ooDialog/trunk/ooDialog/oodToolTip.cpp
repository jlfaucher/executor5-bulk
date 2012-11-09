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
#include <WindowsX.h>

#include "APICommon.hpp"
#include "oodCommon.hpp"
#include "oodMessaging.hpp"
#include "oodControl.hpp"
#include "oodResources.hpp"


/**
 * Parse TTF_X keywords into the corresponding value specific to a TTN_NEEDTEXT
 * notification..
 *
 * @param flags   Keyword string to parse
 *
 * @return The combined flag value
 */
uint32_t keyword2ttdiFlags(CSTRING flags)
{
    uint32_t f = 0;

    if ( StrStrI(flags, "IDISHWND"   ) != NULL ) f |= TTF_IDISHWND;
    if ( StrStrI(flags, "RTLREADING" ) != NULL ) f |= TTF_RTLREADING;
    if ( StrStrI(flags, "DI_SETITEM" ) != NULL ) f |= TTF_DI_SETITEM;

    return f;
}

/**
  * Convert TTF_* flags into a string of keywwords specific to a TTN_NEEDTEXT
  * notification.
  *
  * @param flags
  *
  * @return RexxStringObject
  */
 RexxStringObject ttdiFlags2keyword(RexxThreadContext *c, uint32_t flags)
{
    char buf[512];
    *buf = '\0';

    if ( flags & TTF_IDISHWND   ) strcat(buf, "IDISHWND ");
    if ( flags & TTF_RTLREADING ) strcat(buf, "RTLREADING ");
    if ( flags & TTF_DI_SETITEM ) strcat(buf, "DI_SETITEM ");

    if ( *buf != '\0' )
    {
        *(buf + strlen(buf) - 1) = '\0';
    }
    return c->String(buf);
}

/**
 * Convert a mouse window message code to a name.
 */
inline RexxStringObject mousemsg2name(RexxThreadContext *c, uint32_t msg)
{
    switch ( msg )
    {
        case WM_MOUSEMOVE      : return c->String("mouseMove    ");
        case WM_LBUTTONDOWN    : return c->String("lButtonDown  ");
        case WM_LBUTTONUP      : return c->String("lButtonUp    ");
        case WM_LBUTTONDBLCLK  : return c->String("lButtonDblClk");
        case WM_RBUTTONDOWN    : return c->String("rButtonDown  ");
        case WM_RBUTTONUP      : return c->String("rButtonUp    ");
        case WM_RBUTTONDBLCLK  : return c->String("rButtonDblClk");
        case WM_MBUTTONDOWN    : return c->String("mButtonDown  ");
        case WM_MBUTTONUP      : return c->String("mButtonUp    ");
        case WM_MBUTTONDBLCLK  : return c->String("mButtonDblClk");
        case WM_XBUTTONDOWN    : return c->String("xButtonDown  ");
        case WM_XBUTTONUP      : return c->String("xButtonUp    ");
        case WM_XBUTTONDBLCLK  : return c->String("xButtonDblClk");
        case WM_MOUSEWHEEL     : return c->String("mouseWheel   ");
    }
    return c->String("unknown");
}

/**
 *  Methods for the .ToolTip class.
 */
#define TOOLTIP_CLASS             "ToolTip"


static bool lazyInitToolTipTable(RexxMethodContext *c, pCPlainBaseDialog pcpbd)
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
    pcpbd->ToolTipTab[pcpbd->TTT_nextIndex].hToolTip = hToolTip;
    pcpbd->ToolTipTab[pcpbd->TTT_nextIndex].rexxSelf = toolTip;
    pcpbd->TTT_nextIndex++;

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

    if ( StrStrI(flags, "ALWAYSTIP"     ) != NULL ) style |= TTS_ALWAYSTIP;
    if ( StrStrI(flags, "BALLOON"       ) != NULL ) style |= TTS_BALLOON;
    if ( StrStrI(flags, "NOANIMATE"     ) != NULL ) style |= TTS_NOANIMATE;
    if ( StrStrI(flags, "CLOSE"         ) != NULL ) style |= TTS_CLOSE;
    if ( StrStrI(flags, "NOFADE"        ) != NULL ) style |= TTS_NOFADE;
    if ( StrStrI(flags, "NOPREFIX"      ) != NULL ) style |= TTS_NOPREFIX;
    if ( StrStrI(flags, "USEVISUALSTYLE") != NULL ) style |= TTS_USEVISUALSTYLE;

done_out:
    return style;
}

/**
 * Parse TTF_X keywords into the corresponding value.
 *
 * @param flags   Keyword string to parse
 *
 * @return The combined flag value
 */
static uint32_t keyword2ttfFlags(CSTRING flags)
{
    uint32_t f = 0;

    if ( StrStrI(flags, "ABSOLUTE"   ) != NULL ) f |= TTF_ABSOLUTE;
    if ( StrStrI(flags, "CENTERTIP"  ) != NULL ) f |= TTF_CENTERTIP;
    if ( StrStrI(flags, "IDISHWND"   ) != NULL ) f |= TTF_IDISHWND;
    if ( StrStrI(flags, "PARSELINKS" ) != NULL ) f |= TTF_PARSELINKS;
    if ( StrStrI(flags, "RTLREADING" ) != NULL ) f |= TTF_RTLREADING;
    if ( StrStrI(flags, "SUBCLASS"   ) != NULL ) f |= TTF_SUBCLASS;
    if ( StrStrI(flags, "TRACK"      ) != NULL ) f |= TTF_TRACK;
    if ( StrStrI(flags, "TRANSPARENT") != NULL ) f |= TTF_TRANSPARENT;

    return f;
}

 /**
  * Convert TTF_* flags into a string of keywwords.
  *
  * @param flags
  *
  * @return RexxStringObject
  */
 RexxStringObject ttfFlags2keyword(RexxMethodContext *c, uint32_t flags)
{
    char buf[512];
    *buf = '\0';

    if ( flags & TTF_ABSOLUTE   ) strcat(buf, "ABSOLUTE ");
    if ( flags & TTF_CENTERTIP  ) strcat(buf, "CENTERTIP ");
    if ( flags & TTF_IDISHWND   ) strcat(buf, "IDISHWND ");
    if ( flags & TTF_PARSELINKS ) strcat(buf, "PARSELINKS ");
    if ( flags & TTF_RTLREADING ) strcat(buf, "RTLREADING ");
    if ( flags & TTF_SUBCLASS   ) strcat(buf, "SUBCLASS ");
    if ( flags & TTF_TRACK      ) strcat(buf, "TRACK ");
    if ( flags & TTF_TRANSPARENT) strcat(buf, "TRANSPARENT ");

    if ( *buf != '\0' )
    {
        *(buf + strlen(buf) - 1) = '\0';
    }
    return c->String(buf);
}


/**
 * Generic function to fill in the hwnd and uID fields of the tool info struct.
 * Called from several of the methods that deal with tool tips.
 *
 * If rxObj is a ToolInfo object, then we have all the information we need.
 *
 * When rxObj is not a ToolInfo object, then if uID is present than we assume
 * rxObj is a dialog object and uID is some type of resource ID. Otherwise rxObj
 * must be a dialog control.
 *
 * @param c
 * @param rxObj
 * @param uID
 * @param pTI
 * @param hwndSupplier
 * @param uIDSupplier
 *
 * @return True on success, false on failure.
 *
 * @remarks  In some cases this function is invoked when instantiating a new
 *           ToolInfo object.  In those cases, it is convenient to return the
 *           hwndSupplier and uIDSupplier, and rxObj will never be a Rexx
 *           ToolInfo object.
 *
 *           When rxObj is already a Rexx ToolInfo, then the caller has no need
 *           of hwndSupplier or uIDSupplier and so we completely ignore
 *           hwndSupplier and uIDSupplier.
 */
bool genericToolID(RexxMethodContext *c, RexxObjectPtr rxObj, RexxObjectPtr uID, LPTOOLINFO pTI,
                   RexxObjectPtr *hwndSupplier, RexxObjectPtr *uIDSupplier)
{
    bool success = false;

    if ( c->IsOfType(rxObj, "TOOLINFO") )
    {
        LPTOOLINFO pToolInfo = (LPTOOLINFO)c->ObjectToCSelf(rxObj);

        pTI->hwnd = pToolInfo->hwnd;
        pTI->uId  = pToolInfo->uId;

        success = true;
        goto done_out;
    }
    else
    {
        if ( uID != NULLOBJECT )
        {
            if ( ! requiredClass(c->threadContext, rxObj, "PLAINBASEDIALOG", 1) )
            {
                goto done_out;
            }
            pCPlainBaseDialog pcpbd = dlgToCSelf(c, rxObj);

            uint32_t id = oodResolveSymbolicID(c->threadContext, pcpbd->rexxSelf, uID, -1, 2, false);
            if ( id == OOD_ID_EXCEPTION  )
            {
                goto done_out;
            }

            pTI->hwnd = pcpbd->hDlg;
            pTI->uId  = id;

            if ( hwndSupplier != NULL )
            {
                *hwndSupplier = rxObj;
                *uIDSupplier  = uID;
            }
        }
        else
        {
            if ( ! requiredClass(c->threadContext, rxObj, "DIALOGCONTROL", 1) )
            {
                goto done_out;
            }
            pCDialogControl pcdc = controlToCSelf(c, rxObj);

            pTI->hwnd = pcdc->hDlg;
            pTI->uId  = (UINT_PTR)pcdc->hCtrl;

            if ( hwndSupplier != NULL )
            {
                *hwndSupplier = pcdc->oDlg;
                *uIDSupplier  = rxObj;
            }
        }

        success = true;
    }

done_out:
    return success;
}


/**
 * Attempts to return a Rexx object that represens the proper ID for a tool tip
 * tool.
 *
 * @param c
 * @param pTI
 *
 * @return RexxObjectPtr
 *
 * @remarks  The tool tip tool ID can either be a dialog control window handle,
 *           or a unique number.  There is no real way to tell whether the
 *           number in pTI->uID is a HWND or a number.
 *
 *           So we test to try and determine if id is a dialog control window
 *           handle.  If it seems to be a dialog control, it should have the
 *           Rexx dialog control object in the window words.  Finally, if we try
 *           to get the Rexx object, but it doesn't seem to be there, we just
 *           punt and call it a number.
 */
RexxObjectPtr getToolIDFromToolInfo(RexxMethodContext *c, LPTOOLINFO pTI)
{
    RexxObjectPtr rxID = TheNilObj;
    UINT_PTR      id   = pTI->uId;

    SetLastError(0);
    if ( IsWindow((HWND)id) && GetDlgCtrlID((HWND)id) != 0 && GetLastError() == 0 )
    {
        rxID = (RexxObjectPtr)getWindowPtr((HWND)id, GWLP_USERDATA);
        if ( rxID == NULLOBJECT )
        {
            rxID = c->Uintptr(id);
        }
    }
    else
    {
        rxID = c->Uintptr(id);
    }
    return rxID;
}

inline void freeRelayData(pRelayEventData pData)
{
    safeLocalFree(pData->method);
    safeLocalFree(pData);
}

LRESULT CALLBACK RelayEventSubclassProc(HWND hwnd, uint32_t msg, WPARAM wParam, LPARAM lParam, UINT_PTR id, DWORD_PTR dwData)
{
    pRelayEventData pData = (pRelayEventData)dwData;

    ///*
    if ( (msg >= WM_MOUSEFIRST && msg <= WM_MOUSELAST) || msg == WM_NCMOUSEMOVE)
    {
        RexxThreadContext *c = pData->pcpbd->dlgProcContext;

        RexxObjectPtr rxPoint = rxNewPoint(c, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        RexxObjectPtr rxMsg   = mousemsg2name(c, msg);

        RexxArrayObject args = c->ArrayOfThree(pData->rxToolTip, rxPoint, rxMsg);

        RexxObjectPtr reply = c->SendMessage(pData->pcpbd->rexxSelf, pData->method, args);

        MSG _msg;
        _msg.hwnd = hwnd;
        _msg.message = msg;
        _msg.wParam = wParam;
        _msg.lParam = lParam;
        _msg.pt.x   = GET_X_LPARAM(lParam);
        _msg.pt.y   = GET_Y_LPARAM(lParam);
        SendMessage(pData->hToolTip, TTM_RELAYEVENT, 0, (LPARAM)&_msg);
    }
    //*/

    if ( msg == WM_NOTIFY )
    {
        uint32_t code = ((NMHDR *)lParam)->code;
        if ( code <= TTN_FIRST && code >= TTN_LAST )
        {
            printf("Got TTN msg code=%d GDIW=%d Show=%d, Pop=%d GDIA=%d GDI=%d\n",
                   code, TTN_GETDISPINFOW, TTN_SHOW, TTN_POP, TTN_GETDISPINFOA, TTN_GETDISPINFO);
        }

        switch ( code )
        {
            case TTN_SHOW :
            {
                LPARAM lp = DefSubclassProc(hwnd, msg, wParam, lParam);

                HWND hwndToolTip = ((NMHDR *)lParam)->hwndFrom;

                RECT rc;
                GetWindowRect(hwndToolTip, &rc);
                printf("Current rect left=%d top=%d old lParam=%d hwndToolTip=%p\n", rc.left, rc.top, lp, hwndToolTip);

                rc.left += 35;
                rc.top  += 35;
                SetWindowPos(hwndToolTip,
                 NULL,
                 rc.left, rc.top,
                 0, 0,
                 SWP_NOSIZE|SWP_NOZORDER|SWP_NOACTIVATE);

            }   return TRUE;

            case TTN_GETDISPINFOW :
            {
                NMTTDISPINFO *nmtdi = (NMTTDISPINFO *)lParam;
                printf("GetDispInfo szText=%s lpszText=%s\n",
                       nmtdi->szText ? nmtdi->szText : "null",
                       nmtdi->lpszText ? nmtdi->lpszText : "null");

                RexxThreadContext *c = pData->pcpbd->dlgProcContext;

                RexxObjectPtr rxReply = c->SendMessage0(pData->pcpbd->rexxSelf, "onGetDispInfo");
                CSTRING newText = c->ObjectToStringValue(rxReply);


                nmtdi->lpszText = (LPSTR)ansi2unicode(newText);
                //nmtdi->lpszText = (LPSTR)L"Test of me";
                //putUnicodeText((LPWORD)nmtdi->szText, "Test of me");
                //strcpy(nmtdi->szText, "Test of me");
            }   return TRUE;

            default :
                break;

        }
        /*
        printf("Got WM_NOTIFY code=%d TTNSHOW=%x DispInfoW=%d\n", code, TTN_SHOW, TTN_GETDISPINFOW);
        if ( code == TTN_SHOW )
        {
            printf("Got WM_NOTIFY and TTN_SHOW\n");
        }
        */

    }
    if ( msg == WM_NCDESTROY )
    {
        /* The window is being destroyed, remove the subclass, clean up memory.
         * Note that with the current ooDialog architecture, this message never
         * gets here.  Freeing the subclass data struct has to be done in the
         * dialog control uninit().  TODO FIX THIS, we never free the data
         * struct.
         */
        RemoveWindowSubclass(hwnd, RelayEventSubclassProc, pData->id);
        freeRelayData(pData);
    }

    return DefSubclassProc(hwnd, msg, wParam, lParam);
}


/** PlainBaseDialog::newToolTip()
 *
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
 * @note  All other dialog controls are instantiated through pbdlg_newControl
 *        which carries the legacy baggage of having to accomadate the
 *        deprecated CategoryDialog.  The newer ToolTip control has a number of
 *        differences from other dialog controls, so it has its own 'new' method
 *        here.  The newToolTip() method is still a PlainBaseDialog method, we
 *        just put it here to keep the ToolTip stuff together.  We need to
 *        remember that the context is not the DialogControl context, it is the
 *        PlainBaseDialog context.
 *
 *        Tool tip controls are different than most other Window controls in
 *        that they are actually popup windows owned by the dialog, rather than
 *        child windows of the dialog.  Because of this we need to keep track of
 *        them by adding them to a table.
 *
 *        Because they are popup windows, we can not find an existing tool tip
 *        through GetDlgItem().  So, we add each created tool tip to a table and
 *        look up an existing tool tip through its ID.
 */
RexxMethod3(RexxObjectPtr, pbdlg_newToolTip, RexxObjectPtr, rxID, OPTIONAL_CSTRING, styleFlags, CSELF, pCSelf)
{
    oodResetSysErrCode(context->threadContext);

    RexxObjectPtr toolTip = TheNilObj;
    CREATETOOLTIP ctt     = {0};

    if ( pCSelf == NULL )
    {
        baseClassInitializationException(context);
        goto out;
    }
    pCPlainBaseDialog pcpbd = (pCPlainBaseDialog)pCSelf;

    uint32_t id = oodResolveSymbolicID(context->threadContext, pcpbd->rexxSelf, rxID, -1, 1, true);
    if ( id == OOD_ID_EXCEPTION  )
    {
        goto out;
    }

    if ( pcpbd->ToolTipTab == NULL && ! lazyInitToolTipTable(context, pcpbd) )
    {
        goto out;
    }

    PTOOLTIPTABLEENTRY ptte = findToolTipForID(pcpbd, id);
    if ( ptte != NULL )
    {
        toolTip = ptte->rexxSelf;
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

        SetWindowPos(hToolTip, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
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
 *  Activates or deactivates this tool tip.
 *
 *  @param activate  [optional] If true activates this tool tip, if false
 *                   deactivates this tool tip.  The default if omitted is true.
 *
 *  @return  0, always.
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
 *  Adds a tool to this tool tip.
 *
 *  @param tool   [required]  Must be a dialog control.  Use addToolRect() or
 *                addToolEx() for other tooltip uses.
 *
 *  @param text   [optional] Text for the tool.  If omitted, or the empty
 *                string, or the string: TEXTCALLBACK then the tool tip sends
 *                the NEEDTEXT notification and the program supplies the text.
 *
 *                The maximum length of the text is 1023 characters, which
 *                includes any possible 0x0D0A sequences.
 *
 *  @param flags  [optional] Keywords for the TTF_* flags.  If omitted flags are
 *                automatically set to TTF_IDISHWND | TTF_SUBCLASS.  If not
 *                omitted, flags are set to whatever is specified.  However,
 *                TTF_IDISHWND is always set.  (Because uID is always set to
 *                hwnd of tool.
 *
 *  @param userData  [optional]  A user data value to be associated with the
 *                   tool.  Note that the value is associated with the tool, not
 *                   the tool tip.
 *
 *  @return  True on success, false on error.
 *
 *  @notes    Many methods require the indentity of the tool.  A tool id is
 *            defined by a hwnd and a unique ID. For addTool(), the identity
 *            will always be the hwnd of the owner dialog of the tool (the
 *            dialog control passed in as the tool arg) and the unique ID is
 *            always the hwnd of the tool (the dialog control passed in as the
 *            tool arg.)
 */
RexxMethod5(logical_t, tt_addTool, RexxObjectPtr, tool, OPTIONAL_CSTRING, text, OPTIONAL_CSTRING, _flags,
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

    if ( argumentOmitted(2) )
    {
        text = "";
    }

    size_t l = strlen(text);
    if ( l > MAX_TOOLINFO_TEXT_LENGTH )
    {
        stringTooLongException(context->threadContext, 2, MAX_TOOLINFO_TEXT_LENGTH, l);
        return false;
    }
    if ( l == 0 || StrCmpI("TEXTCALLBACK", text) == 0 )
    {
        text = LPSTR_TEXTCALLBACK;
    }

    uint32_t flags = TTF_IDISHWND;
    if ( argumentOmitted(3) )
    {
        flags |= TTF_SUBCLASS;
    }
    else
    {
        flags |= keyword2ttfFlags(_flags);
    }

    TOOLINFO ti = { sizeof(ti) };

    ti.uFlags   = flags;
    ti.uId      = (UINT_PTR)pcdcTool->hCtrl;
    ti.hwnd     = pcdc->hDlg;
    ti.lpszText = (LPSTR)text;
    ti.lParam   = (LPARAM)(argumentExists(4) ? userData : 0);

    return SendMessage(pcdc->hCtrl, TTM_ADDTOOL, 0, (LPARAM)&ti);
}


/** ToolTip::addToolEx()
 *
 *
 */
RexxMethod2(logical_t, tt_addToolEx, RexxObjectPtr, toolInfo, CSELF, pCSelf)
{
    pCDialogControl pcdc = validateDCCSelf(context, pCSelf);
    if ( pcdc == NULL )
    {
        return FALSE;
    }

    if ( ! requiredClass(context->threadContext, toolInfo, "ToolInfo", 1) )
    {
        return FALSE;
    }

    LPTOOLINFO pTI = (LPTOOLINFO)context->ObjectToCSelf(toolInfo);

    return SendMessage(pcdc->hCtrl, TTM_ADDTOOL, 0, (LPARAM)pTI);
}


/** ToolTip::addToolRect()
 *
 *  Adds a tool that uses a rectangular area in the dialog as its trigger point.
 *
 *  @param  dlg       The dialog the tool belongs to.
 *  @param  id        An ID that uniquely identifies the tool being added.
 *  @param  rect      The rectangle, relative to the client area of the dialog,
 *                    that the tool tip shows for.
 *  @param  text      The text for the tool tip.
 *  @param  flags     The flags for the tool tip.
 *  @param  userData  User data to associate with the tool.
 *
 */
RexxMethod7(logical_t, tt_addToolRect, RexxObjectPtr, dlg, RexxObjectPtr, rxID, RexxObjectPtr, _rect,
            OPTIONAL_CSTRING, text, OPTIONAL_CSTRING, _flags, OPTIONAL_RexxObjectPtr, userData, CSELF, pCSelf)
{
    pCDialogControl pcdc = validateDCCSelf(context, pCSelf);
    if ( pcdc == NULL )
    {
        return FALSE;
    }

    if ( ! requiredClass(context->threadContext, dlg, "PLAINBASEDIALOG", 1) )
    {
        return FALSE;
    }
    pCPlainBaseDialog pcpbd = dlgToCSelf(context, dlg);

    uint32_t id = oodResolveSymbolicID(context->threadContext, pcpbd->rexxSelf, rxID, -1, 2, false);
    if ( id == OOD_ID_EXCEPTION  )
    {
        return FALSE;
    }

    PRECT pRect = rxGetRect(context, _rect, 2);
    if ( pRect == NULL )
    {
        return FALSE;
    }

    if ( argumentOmitted(4) )
    {
        text = "";
    }

    size_t l = strlen(text);
    if ( l > MAX_TOOLINFO_TEXT_LENGTH )
    {
        stringTooLongException(context->threadContext, 4, MAX_TOOLINFO_TEXT_LENGTH, l);
        return false;
    }
    if ( l == 0 || StrCmpI("TEXTCALLBACK", text) == 0 )
    {
        text = LPSTR_TEXTCALLBACK;
    }

    uint32_t flags = TTF_SUBCLASS;
    if ( argumentExists(5) )
    {
        flags |= keyword2ttfFlags(_flags);
        flags &= ~TTF_IDISHWND;
    }

    TOOLINFO ti = {sizeof(ti)};

    ti.uFlags   = flags;
    ti.hwnd     = pcdc->hDlg;
    ti.uId      = id;
    ti.lpszText = (LPSTR)text;

    CopyRect(&ti.rect, pRect);

    if ( argumentExists(6) )
    {
        ti.lParam   = (LPARAM)userData;
    }
    return SendMessage(pcdc->hCtrl, TTM_ADDTOOL, 0, (LPARAM)&ti);
}


/** ToolTip::adjustRect()
 *
 *  Calculates a ToolTip control's text display rectangle from its window
 *  rectangle, or the ToolTip window rectangle needed to display a specified
 *  text display rectangle.
 *
 *  @param rect  [required] A Rectangle object used to specify the rectangle to
 *               adjust.  On a successful return, the co-ordinates in the
 *               rectangle will be adjusted as scecified byt the larger arguent.
 *
 *  @param larger  [optional] True or false to specify how the rectangle is
 *                 adjusted.  If omitted, the default is false.
 *
 *                 If true, rect is used to specify a text-display rectangle and
 *                 it receives the corresponding window rectangle.  The received
 *                 rectangle is *larger* in this case.  If false, rect is used
 *                 to specify a window rectangle and it receives the
 *                 corresponding text display rectangle.
 */
RexxMethod3(logical_t, tt_adjustRect, RexxObjectPtr, _rect, logical_t, larger, CSELF, pCSelf)
{
    pCDialogControl pcdc = validateDCCSelf(context, pCSelf);
    if ( pcdc == NULL )
    {
        return FALSE;
    }

    PRECT r = rxGetRect(context, _rect, 1);
    if ( r == NULL )
    {
        return FALSE;
    }

    return SendMessage(pcdc->hCtrl, TTM_ADJUSTRECT, larger, (LPARAM)r);
}


/** ToolTip::delTool()
 *
 *
 *  @return  Returns 0 always.
 *
 */
RexxMethod3(uint32_t, tt_delTool, RexxObjectPtr, toolID, OPTIONAL_RexxObjectPtr, uID, CSELF, pCSelf)
{
    TOOLINFO ti = { sizeof(ti) };

    pCDialogControl pcdc = validateDCCSelf(context, pCSelf);
    if ( pcdc == NULL )
    {
        goto done_out;
    }

    if ( ! genericToolID(context, toolID, uID, &ti, NULL, NULL) )
    {
        goto done_out;
    }

    SendMessage(pcdc->hCtrl, TTM_DELTOOL, 0, (LPARAM)&ti);

done_out:
    return 0;
}


/** ToolTip::getCurrentToolInfo()
 *
 *
 */
RexxMethod1(RexxObjectPtr, tt_getCurrentToolInfo, CSELF, pCSelf)
{
    RexxObjectPtr result = TheNilObj;

    pCDialogControl pcdc = validateDCCSelf(context, pCSelf);
    if ( pcdc == NULL )
    {
        goto done_out;
    }

    RexxMethodContext *c = context;

    RexxBufferObject tiBuf = context->NewBuffer(sizeof(TOOLINFO));
    if ( tiBuf == NULLOBJECT )
    {
        outOfMemoryException(context->threadContext);
        goto done_out;
    }

    LPTOOLINFO pTI = (LPTOOLINFO)context->BufferData(tiBuf);
    memset(pTI, 0, sizeof(TOOLINFO));

    pTI->cbSize   = sizeof(TOOLINFO);
    pTI->lpszText = (char *)LocalAlloc(LPTR, MAX_TOOLINFO_TEXT_LENGTH + 1);
    if ( pTI->lpszText == NULL )
    {
        outOfMemoryException(context->threadContext);
        goto done_out;
    }

    if ( SendMessage(pcdc->hCtrl, TTM_GETCURRENTTOOL, 0, (LPARAM)pTI) )
    {
        RexxClassObject tiCls = rxGetContextClass(context, "TOOLINFO");
        if ( tiCls != NULLOBJECT )
        {
            if ( pTI->hwnd != pcdc->hDlg )
            {
                printf("getCurrentToolInfo() tool info hwnd=%p does not match dialog hwnd=%p\n", pTI->hwnd, pcdc->hDlg);
            }

            RexxObjectPtr toolID = getToolIDFromToolInfo(context, pTI);

            RexxArrayObject args = context->NewArray(5);
            context->ArrayPut(args, tiBuf, 1);
            context->ArrayPut(args, toolID, 2);
            context->ArrayPut(args, pcdc->oDlg, 5);

            result = context->SendMessage(tiCls, "NEW", args);
            if ( result == NULLOBJECT )
            {
                result = TheNilObj;
            }
            else
            {
                context->SendMessage1(result, "TEXTMEMORYISALLOCATED=", TheTrueObj);
            }
        }
    }

    if ( result == TheNilObj )
    {
        safeLocalFree(pTI->lpszText);
    }

done_out:
    return result;
}


/** ToolTip::enumTools()
 *
 *
 */
RexxMethod2(RexxObjectPtr, tt_enumTools, OPTIONAL_uint32_t, index, CSELF, pCSelf)
{
    pCDialogControl pcdc = validateDCCSelf(context, pCSelf);
    if ( pcdc == NULL )
    {
        return TheNilObj;
    }

    if ( argumentExists(1) )
    {
        if ( index < 1 )
        {
            wrongRangeException(context, 1, 1, UINT32_MAX, index);
            return TheNilObj;
        }
        index--;
    }
    else
    {
        index = 1;
    }

    TOOLINFO ti = {sizeof(ti)};
    ti.lpszText = (char *)LocalAlloc(LPTR, 81);

    if ( SendMessage(pcdc->hCtrl, TTM_ENUMTOOLS, index, (LPARAM)&ti) )
    {
        printf("Got enum tools tool info text=%p flags =0x%x hwnd=%p uID=%p\n", ti.lpszText, ti.uFlags, ti.hwnd, ti.uId);
    }
    else
    {
        printf("Did NOT get enum tools tool info\n");
    }

    return TheNilObj;
}


/** ToolTip::getToolCount()
 *
 *
 */
RexxMethod1(uint32_t, tt_getToolCount, CSELF, pCSelf)
{
    pCDialogControl pcdc = validateDCCSelf(context, pCSelf);
    if ( pcdc == NULL )
    {
        return FALSE;
    }

    return (uint32_t)SendMessage(pcdc->hCtrl, TTM_GETTOOLCOUNT, 0, 0);
}


/** ToolTip::getToolInfo()
 *
 *
 *  @remarks  It seems as though both ti.hwnd and ti.uId must be set to the
 *            values they were set to in addTool, or the get tool info message
 *            fails.  Not sure how to have the user convey this info to this
 *            method ??
 */
RexxMethod2(logical_t, tt_getToolInfo, RexxObjectPtr, toolInfo, CSELF, pCSelf)
{
    pCDialogControl pcdc = validateDCCSelf(context, pCSelf);
    if ( pcdc == NULL )
    {
        return FALSE;
    }

    if ( ! requiredClass(context->threadContext, toolInfo, "TOOLINFO", 1) )
    {
        return FALSE;
    }

    LPTOOLINFO pTI = (LPTOOLINFO)context->ObjectToCSelf(toolInfo);

    return SendMessage(pcdc->hCtrl, TTM_GETTOOLINFO, 0, (LPARAM)pTI);
}


/** ToolTip::popUp()
 *
 *
 */
RexxMethod1(uint32_t, tt_popUp, CSELF, pCSelf)
{
    pCDialogControl pcdc = validateDCCSelf(context, pCSelf);
    if ( pcdc == NULL )
    {
        return FALSE;
    }

    SendMessage(pcdc->hCtrl, TTM_POPUP, 0, 0);

    return 0;
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


/** ToolTip::trackActivate()
 *
 *
 */
RexxMethod4(uint32_t, tt_trackActivate, RexxObjectPtr, toolID, OPTIONAL_RexxObjectPtr, uID,
            OPTIONAL_logical_t, activate, CSELF, pCSelf)
{
    TOOLINFO ti = { sizeof(ti) };

    pCDialogControl pcdc = validateDCCSelf(context, pCSelf);
    if ( pcdc == NULL )
    {
        goto done_out;
    }

    if ( ! genericToolID(context, toolID, uID, &ti, NULL, NULL) )
    {
        goto done_out;
    }

    if ( argumentOmitted(3) )
    {
        activate = TRUE;
    }

    SendMessage(pcdc->hCtrl, TTM_TRACKACTIVATE, activate, (LPARAM)&ti);

done_out:
    return 0;
}


/** ToolTip::trackPosition()
 *
 *  Sets the position of a tracking ToolTip.
 *
 *  @param  coordinates  The position (x, y) to set.
 *
 *      Form 1:  A .Point object.
 *      Form 2:  x, y
 *
 *  @return  0, always.
 *
 */
RexxMethod2(uint32_t, tt_trackPosition, ARGLIST, args, CSELF, pCSelf)
{
    pCDialogControl pcdc = validateDCCSelf(context, pCSelf);
    if ( pcdc == NULL )
    {
        return 0;;
    }

    size_t countArgs;
    size_t argsUsed;
    POINT  point;
    if ( ! getPointFromArglist(context, args, &point, 1, 2, &countArgs, &argsUsed) )
    {
        return 0;
    }

    if ( countArgs > argsUsed )
    {
        tooManyArgsException(context->threadContext, argsUsed + 1);
        return 0;
    }

    SendMessage(pcdc->hCtrl, TTM_TRACKPOSITION, 0, (LPARAM)MAKELONG(point.x, point.y));

    return 0;
}


RexxMethod3(logical_t, tt_useRelayEvent, RexxObjectPtr, toolObject, OPTIONAL_CSTRING, method, CSELF, pCSelf)
{
    pCDialogControl pcdc = validateDCCSelf(context, pCSelf);
    if ( pcdc == NULL )
    {
        return FALSE;
    }

    pCDialogControl subClassCtrl = NULL;

    RexxMethodContext *c = context;
    if ( context->IsOfType(toolObject, "PLAINBASEDIALOG") )
    {
        userDefinedMsgException(context->threadContext, 1, "useRelayEvent() is not implemented for dialogs yet");
        return FALSE;
    }
    else if ( context->IsOfType(toolObject, "DIALOGCONTROL") )
    {
        subClassCtrl = controlToCSelf(context, toolObject);
    }
    else
    {
        wrongClassListException(context->threadContext, 1, "PlainBaseDialog or DialogControl", toolObject);
        return FALSE;
    }

    if ( argumentOmitted(2) )
    {
        method = "onMouseRelayEvent";
    }

    pRelayEventData pSCData = (pRelayEventData)LocalAlloc(LPTR, sizeof(RelayEventData));
    if ( pSCData == NULL )
    {
        outOfMemoryException(c->threadContext);
        return FALSE;
    }

    pSCData->method = (char *)LocalAlloc(LPTR, strlen(method) + 1);
    if ( pSCData->method == NULL )
    {
        outOfMemoryException(c->threadContext);
        return FALSE;
    }
    strcpy(pSCData->method, method);

    pSCData->pcpbd     = subClassCtrl->pcpbd;
    pSCData->pcdc      = subClassCtrl;
    pSCData->hCtrl     = subClassCtrl->hCtrl;
    pSCData->hToolTip  = pcdc->hCtrl;
    pSCData->rxToolTip = pcdc->rexxSelf;
    pSCData->id        = subClassCtrl->id;

    BOOL success;
    if ( isDlgThread(pcdc->pcpbd) )
    {
        success = SetWindowSubclass(pcdc->hCtrl, RelayEventSubclassProc, pcdc->id, (DWORD_PTR)pSCData);
    }
    else
    {
        success = (BOOL)SendMessage(pcdc->pcpbd->hDlg, WM_USER_SUBCLASS, (WPARAM)RelayEventSubclassProc, (LPARAM)pSCData);
    }

    if ( ! success )
    {
        LocalFree(pSCData->method);
        LocalFree(pSCData);
        systemServiceExceptionCode(c->threadContext, API_FAILED_MSG, "SetWindowSubclass");
        return FALSE;
    }

    return TRUE;
}



/**
 *  Methods for the .ToolInfo class.
 */
#define TOOLINFO_CLASS            "ToolInfo"

// The context variable names used by the ToolInfo object to keep track of
// things.
#define TOOLINFO_MEMALLOCATED_VAR     "TEXT_MEMORY_IS_ALLOCATED"
#define TOOLINFO_HWND_OBJECT_VAR      "HWND_SUPPLYING_OBJECT"
#define TOOLINFO_UID_OBJECT_VAR       "UID_SUPPLYING_OBJECT"


/**
 * Allocates a buffer for the tool info struct and copies the specified text
 * into it.
 *
 * @param c
 * @param pTI
 * @param text
 * @param argPos
 *
 * @return bool
 */
static bool setToolInfoText(RexxMethodContext *c, LPTOOLINFO pTI, CSTRING text, size_t argPos)
{
    size_t l = strlen(text);
    if ( l > MAX_TOOLINFO_TEXT_LENGTH )
    {
        stringTooLongException(c->threadContext, argPos, MAX_TOOLINFO_TEXT_LENGTH, l);
        return false;
    }
    if ( l == 0 )
    {
        nullStringMethodException(c, argPos);
        return false;
    }

    if ( StrCmpI("TEXTCALLBACK", text) == 0 )
    {
        pTI->lpszText = LPSTR_TEXTCALLBACK;
    }
    else
    {
        pTI->lpszText = (LPSTR)LocalAlloc(LPTR, MAX_TOOLINFO_TEXT_LENGTH + 1);
        if ( pTI->lpszText == NULL )
        {
            outOfMemoryException(c->threadContext);
            return false;
        }
        c->SetObjectVariable(TOOLINFO_MEMALLOCATED_VAR, TheTrueObj);
        strcpy(pTI->lpszText, text);
    }
    return true;
}


/** ToolInfo::fromIdEx()   [class]
 *
 *  @param  hwndObj  [required]  The object used for the hwnd of the tool info
 *                   struct create a .ToolInfo from native code.  I
 *
 *  @param  uID
 *
 *  @param  text   [optional]
 *
 *  @param  flags  [optional]
 *
 *  @param  _resource  [reserved]  This is reserved for a future enhancement.
 *                     If we ever had the ability to use string resources to the
 *                     .Resource class, then this argument will be a .Resource
 *                     object and text will be changed to a Rexx object.  At
 *                     that time, if _resource is used, text will be assumed to
 *                     be a resource identifier.  For now it is ignored.
 *
 *  @remarks  When creating from native code, set arg 1 to the TOOLINFO buffer,
 *            arg 2 to the object supplying the uID field of the struct, and
 *            arg 5 to the object supplying the hwnd field of the struct. The
 *            hwnd and uID supplying objects are set as context variables so
 *            that they can be returned from the attributes of the ToolInfo
 *            object.
 */
RexxMethod8(RexxObjectPtr, ti_fromIdEx_cls, RexxObjectPtr, hwndObj, RexxObjectPtr, _uID, OPTIONAL_CSTRING, text,
            OPTIONAL_CSTRING, _flags, OPTIONAL_RexxObjectPtr, userData, OPTIONAL_RexxObjectPtr, _rect,
            OPTIONAL_RexxObjectPtr, _resource, OSELF, self)
{
    RexxObjectPtr    result = NULLOBJECT;
    RexxBufferObject tiBuf  = context->NewBuffer(sizeof(TOOLINFO));
    LPTOOLINFO       pTI    = (LPTOOLINFO)context->BufferData(tiBuf);

    memset(pTI, 0, sizeof(TOOLINFO));
    pTI->cbSize = sizeof(TOOLINFO);

    pCPlainBaseDialog pcbd = NULL;
    RexxMethodContext *c = context;
    if (  c->IsOfType(hwndObj, "PLAINBASEDIALOG") )
    {
        pcbd = dlgToCSelf(context, hwndObj);
        pTI->hwnd = pcbd->hDlg;
    }
    else if ( c->IsOfType(hwndObj, "DIALOGCONTROL") )
    {
        pCDialogControl pcdc = controlToCSelf(context, hwndObj);
        pTI->hwnd = pcdc->hCtrl;
        pcbd = pcdc->pcpbd;
    }
    else
    {
        userDefinedMsgException(context->threadContext, 1, "Bad hwnd obj");
        goto done_out;
    }

    if ( c->IsOfType(_uID, "DIALOGCONTROL") )
    {
        pCDialogControl pcdc = controlToCSelf(context, hwndObj);
        pTI->uId = (uintptr_t)pcdc->hCtrl;
    }
    else
    {
        uint32_t id = oodResolveSymbolicID(c->threadContext, pcbd->rexxSelf, _uID, -1, 2, false);
        if ( id == OOD_ID_EXCEPTION  )
        {
            goto done_out;
        }

        pTI->uId  = id;
    }

    if ( argumentExists(3) )
    {
        if ( ! setToolInfoText(context, pTI, text, 4) )
        {
            goto done_out;
        }
    }

    if ( argumentExists(4) )
    {
        pTI->uFlags = keyword2ttfFlags(_flags);
    }

    if ( argumentExists(5) )
    {
        pTI->lParam = (LPARAM)userData;
    }

    if ( argumentExists(6) )
    {
        PRECT r = rxGetRect(context, _rect, 6);
        if ( r == NULL )
        {
            goto done_out;
        }
        CopyRect(&pTI->rect, r);
    }

    RexxArrayObject args = context->NewArray(5);
    context->ArrayPut(args, tiBuf, 1);
    context->ArrayPut(args, _uID, 2);
    context->ArrayPut(args, hwndObj, 5);

    result = context->SendMessage(self, "NEW", args);


done_out:
    return result;
}

/** ToolInfo::fromID()     [class]
 *
 *  @remarks  It's tempting to allocate a buffer here to receive text in the
 *            tool info struct.  But, really the more probable use cases are to
 *            instantiate this object to use as a tool identifier.  So, we only
 *            allocate the buffer in the getToolInfo() method, because that is
 *            the only time the buffer is needed.
 */
RexxMethod3(RexxObjectPtr, ti_fromID_cls, RexxObjectPtr, hwndObj, OPTIONAL_RexxObjectPtr, _uID, OSELF, self)
{
    RexxObjectPtr    result = NULLOBJECT;
    RexxBufferObject tiBuf  = context->NewBuffer(sizeof(TOOLINFO));
    LPTOOLINFO       pTI    = (LPTOOLINFO)context->BufferData(tiBuf);

    memset(pTI, 0, sizeof(TOOLINFO));
    pTI->cbSize = sizeof(TOOLINFO);

    RexxObjectPtr hwndSupplier;
    RexxObjectPtr uIDSupplier;

    if ( ! genericToolID(context, hwndObj, _uID, pTI, &hwndSupplier, &uIDSupplier) )
    {
        goto done_out;
    }

    RexxArrayObject args = context->NewArray(5);
    context->ArrayPut(args, tiBuf, 1);
    context->ArrayPut(args, uIDSupplier, 2);
    context->ArrayPut(args, hwndSupplier, 5);

    result = context->SendMessage(self, "NEW", args);

done_out:
    return result;
}



/** ToolInfo::uninit()
 *
 */
RexxMethod1(RexxObjectPtr, ti_unInit, CSELF, pCSelf)
{
#if 0
    printf("In ti_unInit() pCSelf=%p\n", pCSelf);
#endif

    if ( pCSelf != NULLOBJECT )
    {
        LPTOOLINFO pTI = (LPTOOLINFO)pCSelf;

#if 0
    printf("In ti_unInit() lpszText=%p\n", pTI->lpszText != NULL ? pTI->lpszText : "null");
#endif

        if ( context->GetObjectVariable(TOOLINFO_MEMALLOCATED_VAR) == TheTrueObj )
        {
#if 0
    printf("In ti_unInit() will safeLocalFree() on text\n");
#endif
            safeLocalFree(pTI->lpszText);
        }
    }
    return NULLOBJECT;
}

/** ToolInfo::init()
 *
 *  @param  dlg    [required]  Dialog ojbect.  May be a Buffer object to create
 *                 a .ToolInfo from native code.  I
 *
 *  @param  uID    [optional]
 *
 *  @param  text   [optional]
 *
 *  @param  flags  [optional]
 *
 *  @param  _resource  [reserved]  This is reserved for a future enhancement.
 *                     If we ever had the ability to use string resources to the
 *                     .Resource class, then this argument will be a .Resource
 *                     object and text will be changed to a Rexx object.  At
 *                     that time, if _resource is used, text will be assumed to
 *                     be a resource identifier.  For now it is ignored.
 *
 *  @remarks  When creating from native code, set arg 1 to the TOOLINFO buffer,
 *            arg 2 to the object supplying the uID field of the struct, and
 *            arg 5 to the object supplying the hwnd field of the struct. The
 *            hwnd and uID supplying objects are set as context variables so
 *            that they can be returned from the attributes of the ToolInfo
 *            object.
 */
RexxMethod7(RexxObjectPtr, ti_init, RexxObjectPtr, dlg, OPTIONAL_RexxObjectPtr, _uID, OPTIONAL_CSTRING, text,
            OPTIONAL_CSTRING, _flags, OPTIONAL_RexxObjectPtr, userData, OPTIONAL_RexxObjectPtr, _rect,
            OPTIONAL_RexxObjectPtr, _resource)
{
    if ( context->IsBuffer(dlg) )
    {
        context->SetObjectVariable("CSELF", dlg);
        context->SetObjectVariable(TOOLINFO_HWND_OBJECT_VAR, userData);
        context->SetObjectVariable(TOOLINFO_UID_OBJECT_VAR, _uID);
        goto done_out;
    }

    RexxMethodContext *c = context;
    RexxBufferObject obj = context->NewBuffer(sizeof(TOOLINFO));
    context->SetObjectVariable("CSELF", obj);

    LPTOOLINFO pTI = (LPTOOLINFO)context->BufferData(obj);
    memset(pTI, 0, sizeof(TOOLINFO));

    RexxObjectPtr hwndSupplier;
    RexxObjectPtr uIDSupplier;

    if ( ! genericToolID(context, dlg, _uID, pTI, &hwndSupplier, &uIDSupplier) )
    {
        goto done_out;
    }
    context->SetObjectVariable(TOOLINFO_HWND_OBJECT_VAR, hwndSupplier);
    context->SetObjectVariable(TOOLINFO_UID_OBJECT_VAR, uIDSupplier);

    if ( argumentExists(3) )
    {
        if ( ! setToolInfoText(context, pTI, text, 4) )
        {
            goto done_out;
        }
    }

    if ( argumentExists(4) )
    {
        pTI->uFlags = keyword2ttfFlags(_flags);
    }

    if ( argumentExists(5) )
    {
        pTI->lParam = (LPARAM)userData;
    }

    if ( argumentExists(6) )
    {
        PRECT r = rxGetRect(context, _rect, 6);
        if ( r == NULL )
        {
            goto done_out;
        }
        CopyRect(&pTI->rect, r);
    }

done_out:
    return NULLOBJECT;
}

/** ToolInfo::flags                [attribute]
 */
RexxMethod1(RexxStringObject, ti_flags, CSELF, pTI)
{
    return ttfFlags2keyword(context, ((LPTOOLINFO)pTI)->uFlags);
}
RexxMethod2(RexxObjectPtr, ti_setFlags, CSTRING, flags, CSELF, pTI)
{
    ((LPTOOLINFO)pTI)->uFlags = keyword2ttfFlags(flags);
    return NULLOBJECT;
}

/** ToolInfo::hwndObj                [attribute]
 *
 *  Note, no setter for this attribute.
 */
RexxMethod1(RexxObjectPtr, ti_hwndObj, CSELF, pTI)
{
    RexxObjectPtr hwndObj = context->GetObjectVariable(TOOLINFO_HWND_OBJECT_VAR);
    return (hwndObj ? hwndObj : TheNilObj);
}

/** ToolInfo::idObj                [attribute]
 *
 *  Note, no setter for this attribute.
 */
RexxMethod1(RexxObjectPtr, ti_idObj, CSELF, pTI)
{
    RexxObjectPtr idObj = context->GetObjectVariable(TOOLINFO_UID_OBJECT_VAR);
    return (idObj ? idObj : TheNilObj);
}

/** ToolInfo::rect                [attribute]
 *
 *  Note that currently we are returning .nil for an empty rect.  But, maybe we
 *  should just always return a Rect object, and let the user decide what to do
 *  with an empty Rect?
 */
RexxMethod1(RexxObjectPtr, ti_rect, CSELF, cSelf)
{
    LPTOOLINFO pTI = (LPTOOLINFO)cSelf;
    PRECT pRect = &pTI->rect;

    if ( IsRectEmpty(pRect) )
    {
        return TheNilObj;
    }
    else
    {
        return rxNewRect(context, pRect);
    }
}
RexxMethod2(RexxObjectPtr, ti_setRect, RexxObjectPtr, rect, CSELF, cSelf)
{
    LPTOOLINFO pTI = (LPTOOLINFO)cSelf;
    PRECT pRect = &pTI->rect;

    PRECT r = rxGetRect(context, rect, 1);
    if ( r != NULL )
    {
        CopyRect(&pTI->rect, r);
    }
    return NULLOBJECT;
}

/** ToolInfo::resource                [attribute]
 *
 *  Note that the resource attribute is reserved at this time.  If the .Resource
 *  class is enhanced to work with string resources (a good possibility) then
 *  this attribute will be used for th hinst field of the tool info struct.
 *
 *  For now, we always return .nil and do not allow the user to set the
 *  attribute.
 */
RexxMethod1(RexxObjectPtr, ti_resource, CSELF, cSelf)
{
    return TheNilObj;
}

/** ToolInfo::text                [attribute]
 *
 *  @note  When getting the text attribute we need to determine if the call back
 *         feature is in use.  It is also possible that pTI->lpszText is NULL.
 *
 *         When setting the text attribute, it could be that new text is
 *         replacing existing text.  In that case, we need to determine if the
 *         old text was allocated, and if so, free the old text buffer.
 */
RexxMethod1(RexxObjectPtr, ti_text, CSELF, cSelf)
{
    LPTOOLINFO pTI = (LPTOOLINFO)cSelf;

    if ( pTI->lpszText == NULL )
    {
        return context->NullString();
    }
    else if ( pTI->lpszText == LPSTR_TEXTCALLBACK )
    {
        return context->String("TextCallBack");
    }
    else
    {
        return context->String(pTI->lpszText);
    }
}
RexxMethod2(RexxObjectPtr, ti_setText, CSTRING, text, CSELF, cSelf)
{
    LPTOOLINFO pTI = (LPTOOLINFO)cSelf;

    if ( context->GetObjectVariable(TOOLINFO_MEMALLOCATED_VAR) == TheTrueObj )
    {
        safeLocalFree(pTI->lpszText);
    }
    setToolInfoText(context, pTI, text, 1);
    return NULLOBJECT;
}

/** ToolInfo::userData                [attribute]
 */
RexxMethod1(RexxObjectPtr, ti_userData, CSELF, pTI)
{
    return ( ((LPTOOLINFO)pTI)->lParam ? (RexxObjectPtr)((LPTOOLINFO)pTI)->lParam : TheNilObj );
}
RexxMethod2(RexxObjectPtr, ti_setUserData, RexxObjectPtr, userData, CSELF, pTI)
{
    ((LPTOOLINFO)pTI)->lParam = (LPARAM)userData;
    return NULLOBJECT;
}


/**
 * Internal use only.  Allows native code to instantiate a new ToolInfo object
 * and set the text memory is allocted flag.
 */
RexxMethod1(RexxObjectPtr, ti_setTextMemoryIsAllocated, RexxObjectPtr, allocated)
{
    printf("In setTextMemoryIsAllocated\n");
    context->SetObjectVariable(TOOLINFO_MEMALLOCATED_VAR, allocated == TheTrueObj ? TheTrueObj : TheFalseObj);
    return NULLOBJECT;
}


