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

    style |= TTS_BALLOON;

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
 *  @param flags  [optional] Keywords for the TTF_* flags.  If omitted flags are
 *                automatically set to TTF_IDISHWND | TTF_SUBCLASS.  If not
 *                omitted, flags are set to whatever is specified.  However,
 *                TTF_IDISHWND is always set.  (Because uID is always set to
 *                hwnd of tool.
 *
 *  @param text   [optional] Text for the tool.  If omitted, or the empty
 *                string, or the string: TEXTCALLBACK then the tool tip sends
 *                the NEEDTEXT notification and the program supplies the text.
 *
 *                The maximum length of the text is 1023 characters, which
 *                includes any possible 0x0D0A sequences.
 *
 *  @param userData  [optional]  A user data value to be associated with the
 *                   tool.  Note that the value is associated with the tool, not
 *                   the tool tip.
 *
 *  @return  True on success, false on error.
 *
 *  @notes    Many methods require the indentity of the tool.  A tool is id is
 *            defined by a hwnd and a unique ID. For addTool(), the identity
 *            will always be the hwnd of the dialog and the unique ID is always
 *            the hwnd of the tool (the dialog control passed in as the tool
 *            arg.)
 */
RexxMethod5(logical_t, tt_addTool, RexxObjectPtr, tool, OPTIONAL_CSTRING, _flags, OPTIONAL_CSTRING, text,
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

    uint32_t flags = TTF_IDISHWND;
    if ( argumentOmitted(2) )
    {
        flags |= TTF_SUBCLASS;
    }
    else
    {
        flags |= keyword2ttfFlags(_flags);
    }

    if ( argumentOmitted(3) )
    {
        text = "";
    }

    size_t l = strlen(text);
    if ( l >= 1024 )
    {
        stringTooLongException(context->threadContext, 3, 1024, l);
        return false;
    }
    if ( l == 0 || StrCmpI("TEXTCALLBACK", text) == 0 )
    {
        text = LPSTR_TEXTCALLBACK;
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

    RexxMethodContext *c = context;
    if ( ! requiredClass(context->threadContext, toolInfo, "ToolInfo", 1) )
    {
        return FALSE;
    }

    LPTOOLINFO pTI = (LPTOOLINFO)c->ObjectToCSelf(toolInfo);

    return SendMessage(pcdc->hCtrl, TTM_ADDTOOL, 0, (LPARAM)pTI);
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
RexxMethod3(uint32_t, tt_trackActivate, RexxObjectPtr, tool, OPTIONAL_logical_t, activate, CSELF, pCSelf)
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
        activate = TRUE;
    }

    TOOLINFO ti = { sizeof(ti) };

    ti.uId      = (UINT_PTR)pcdcTool->hCtrl;
    ti.hwnd     = pcdc->hDlg;

    SendMessage(pcdc->hCtrl, TTM_TRACKACTIVATE, activate, (LPARAM)&ti);

    return 0;
}


/** ToolTip::trackPosition()
 *
 *
 */
RexxMethod3(uint32_t, tt_trackPosition, int32_t, x, int32_t, y, CSELF, pCSelf)
{
    pCDialogControl pcdc = validateDCCSelf(context, pCSelf);
    if ( pcdc == NULL )
    {
        return FALSE;
    }

    SendMessage(pcdc->hCtrl, TTM_TRACKPOSITION, 0, (LPARAM)MAKELONG(x, y));

    return 0;
}



/**
 *  Methods for the .ToolInfo class.
 */
#define TOOLINFO_CLASS            "ToolInfo"

#define TOOLINFO_MEMALLOCATED     "TEXT_MEMORY_IS_ALLOCATED"


/**
 * Generic function to fill in the hwnd and uID fiels of the tool info struct.
 * Called from several of the methods that deal with tool tips.
 *
 * If uID is present than we assume hwndObj is a dialog object and uID is some
 * type of resource ID.  Otherwise hwndObj must be a dialog control.
 *
 * @param c
 * @param hwndObj
 * @param uID
 * @param pTI
 *
 * @return bool
 */
bool genericToolID(RexxMethodContext *c, RexxObjectPtr hwndObj, RexxObjectPtr uID, LPTOOLINFO pTI)
{
    bool success = false;

    if ( uID != NULLOBJECT )
    {
        if ( ! requiredClass(c->threadContext, hwndObj, "PLAINBASEDIALOG", 1) )
        {
            goto done_out;
        }
        pCPlainBaseDialog pcpbd = dlgToCSelf(c, hwndObj);

        uint32_t id = oodResolveSymbolicID(c->threadContext, pcpbd->rexxSelf, uID, -1, 2, false);
        if ( id == OOD_ID_EXCEPTION  )
        {
            goto done_out;
        }

        pTI->hwnd = pcpbd->hDlg;
        pTI->uId  = id;
    }
    else
    {
        if ( ! requiredClass(c->threadContext, hwndObj, "DIALOGCONTROL", 1) )
        {
            goto done_out;
        }
        pCDialogControl pcdc = controlToCSelf(c, hwndObj);

        pTI->hwnd = pcdc->hDlg;
        pTI->uId  = (UINT_PTR)pcdc->hCtrl;
    }

    success = true;

done_out:
    return success;
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

    if ( ! genericToolID(context, hwndObj, _uID, pTI) )
    {
        goto done_out;
    }

    result = context->SendMessage2(self, "NEW", tiBuf, TheTrueObj);

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
    printf("In ti_unInit() lpszText=%p\n", pTI->lpszText != NULL ? pTI->lpszText : "null" );
#endif

        RexxObjectPtr memAllocated = context->GetObjectVariable(TOOLINFO_MEMALLOCATED);
        if ( memAllocated == TheTrueObj )
        {
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
 *
 *  @param  _resource  [reserved]  This is reserved for a future enhancement.
 *                     If we ever had the ability to use string resources to the
 *                     .Resource class, then this argument will be a .Resource
 *                     object and text will be changed to a Rexx object.  At
 *                     that time, if _resource is used, text will be assumed to
 *                     be a resource identifier.  For now it is ignored.
 *
 *  @remarks  When creating from native code, remember to use 2nd arg, but it
 *            will be ignored.
 */
RexxMethod7(RexxObjectPtr, ti_init, RexxObjectPtr, dlg, OPTIONAL_RexxObjectPtr, _uID, OPTIONAL_CSTRING, _flags, OPTIONAL_CSTRING, text,
            OPTIONAL_RexxObjectPtr, userData, OPTIONAL_RexxObjectPtr, _rect, OPTIONAL_RexxObjectPtr, _resource)
{
    if ( context->IsBuffer(dlg) )
    {
        context->SetObjectVariable("CSELF", dlg);
        goto done_out;
    }

    RexxMethodContext *c = context;
    RexxBufferObject obj = context->NewBuffer(sizeof(TOOLINFO));
    context->SetObjectVariable("CSELF", obj);

    LPTOOLINFO pTI = (LPTOOLINFO)context->BufferData(obj);
    memset(pTI, 0, sizeof(TOOLINFO));

    if ( ! genericToolID(context, dlg, _uID, pTI) )
    {
        goto done_out;
    }

    if ( argumentExists(3) )
    {
        pTI->uFlags = keyword2ttfFlags(_flags);
    }

    if ( argumentExists(4) )
    {
        size_t l = strlen(text);
        if ( l >= 1024 )
        {
            stringTooLongException(context->threadContext, 3, 1024, l);
            goto done_out;
        }

        if ( l == 0 || StrCmpI("TEXTCALLBACK", text) == 0 )
        {
            pTI->lpszText = LPSTR_TEXTCALLBACK;
        }
        else
        {
            pTI->lpszText = (LPSTR)text;
        }
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

