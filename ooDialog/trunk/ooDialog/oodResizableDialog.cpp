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
#include "oodControl.hpp"
#include "oodDeviceGraphics.hpp"
#include "oodMessaging.hpp"
#include "oodResizableDialog.hpp"

/**
 * Validates that the CSelf pointer for a ResizingAdmin object is not null, and
 * that the resizeInfo pointer is not null.
 *
 * @note  Originally we were going to have a separate CResizingAdmin CSelf.
 *        But, if the user sets up a class that inherits ResizingAdmin before
 *        inheriting PlainBaseDialog, dialog methods expecting CSelf to be
 *        CPlainBaseDialog, will end up with CResizingAdmin instead.  That blows
 *        up.
 *
 *        Granted it is a not common class set up, but it is a reasonable class
 *        set up.  Since the ResizeInfoDlg struct is in the CPlainBaseDialog
 *        struct, it is simplier to not use CResizingAdmin.
 */
inline pCPlainBaseDialog validateRACSelf(RexxMethodContext *c, void *CSelf, pResizeInfoDlg *pprid)
{
    if ( CSelf == NULL || ((pCPlainBaseDialog)CSelf)->resizeInfo == NULL)
    {
        baseClassInitializationException(c, "ResizingAdmin");
        return NULL;
    }

    *pprid = ((pCPlainBaseDialog)CSelf)->resizeInfo;
    return (pCPlainBaseDialog)CSelf;
}

/**
 * Add the needed WS_CLIPCHILDREN flag.  This dramatically reduces flicker.
 *
 * If we also add WS_THICKFRAME here, it appears to work in most cases.  But,
 * there is some problem if the window does not have the WS_MAXIMIZEBOX style.
 * However, if WS_THICKFRAME is added to the original dialolg template, things
 * work without the WS_MAXIMIZEBOX style.  So, for resizable dialogs, we add
 * that flag to the dialog template instead.
 *
 * @param hDlg
 */
inline void setResizableDlgStyle(HWND hDlg)
{
    SetWindowLong(hDlg, GWL_STYLE, GetWindowLong(hDlg, GWL_STYLE) | WS_CLIPCHILDREN);
}

/**
 * Raises an exception for an invalid pin to control ID set by the user.  The
 * control IDs can only be checked for validity after the dialog has been
 * created.
 *
 * @param c
 */
static void raiseInvalidPinToIDException(RexxThreadContext *c, uint32_t pinToID, uint32_t ctrlID)
{
    TCHAR buf[512];
    _snprintf(buf, sizeof(buf), "The resource ID (%d) for the pin to window edge of control ID (%d) is not valid",
              pinToID, ctrlID);
    executionErrorException(c, buf);
}

/**
 * Checks that all the pin to control IDs resolve to valid windows.  This must
 * be done before we enumerate all the control windows, so that it checks what
 * the user set up.
 *
 * @param c
 * @param hDlg
 * @param prid
 *
 * @return bool
 */
static bool verifyPinToWindows(RexxThreadContext *c, HWND hDlg, pResizeInfoDlg prid)
{
    for ( register size_t i = 0; i < prid->countCtrls; i++)
    {
        pControlEdges pce = &prid->riCtrls[i].edges;
        pEdge         p   = (pEdge)pce;
        for (size_t i = 0; i < 4; i++, p++)
        {
            uint32_t pinToID = p->pinToID;
            if ( pinToID != 0 && ! IsWindow(GetDlgItem(hDlg, pinToID)) )
            {
                raiseInvalidPinToIDException(c, pinToID, prid->riCtrls[i].id);
                checkForCondition(c, false);
                return false;
            }
        }
    }
    return true;
}

/**
 * Converts a pin to type keyword to the proper flag value.
 *
 * @param c
 * @param keyWord
 * @param pos
 * @param isLeftTop
 *
 * @return pinType_t
 */
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

/**
 * Converts a pin to edge keyword to the proper flag value.
 *
 * @param c
 * @param keyWord
 * @param pos
 *
 * @return pinnedEdge_t
 */
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

/**
 * Sets up the default left edge struct to the values specified.
 *
 * @param c
 * @param howPinned
 * @param whichEdge
 * @param prid
 *
 * @return Zero on success, one on error.  This Rexx object is passed on to the
 *         user as a reply from a method invocation.
 *
 * @note  Default edges can only be pinned to the dialog window itself.
 */
static RexxObjectPtr defaultLeft(RexxMethodContext *c, CSTRING howPinned, CSTRING whichEdge, pResizeInfoDlg prid)
{
    prid->defEdges.left.pinType   = keyword2pinType(c, howPinned, 1, true);
    prid->defEdges.left.pinToEdge = keyword2pinnedEdge(c, whichEdge, 2);
    prid->defEdges.left.pinToID   = IDC_DEFAULT_PINTO_WINDOW;

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
    prid->defEdges.top.pinToID   = 0;

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
    prid->defEdges.right.pinToID   = 0;

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
    prid->defEdges.bottom.pinToID   = 0;

    if ( prid->defEdges.bottom.pinType == notAPin || prid->defEdges.bottom.pinToEdge == notAnEdge )
    {
        return TheOneObj;
    }
    return TheZeroObj;
}

/**
 * Returns the dialog control resize info struct for the specified control ID.
 *
 * @param prid
 * @param ctrlID
 *
 * @return A pointer to the struct when found, otherwise null.
 *
 * @remarks  Resize info is added to the table before the underlying dialog is
 *           created.  At that time, the resource ID must be greater than 0.
 *           Then, resize info is also added during WM_INITDIALOG.  At that
 *           time, the ID could be -1 for a static control.
 *
 *           We *always* return NULL for -1.  We assume that if the caller uses
 *           -1 for the control ID, the caller knows that there is no existing
 *           resize info for the control.  This is true during the enum windows
 *           function, InitializeAllControlsProc().  The caller is responsible
 *           for any other use.
 */
pResizeInfoCtrl getControlInfo(pResizeInfoDlg prid, uint32_t ctrlID)
{
    if ( ctrlID != (uint32_t)-1 )
    {
        for ( register size_t i = 0; i < prid->countCtrls; i++)
        {
            if ( prid->riCtrls[i].id == ctrlID )
            {
                return &prid->riCtrls[i];
            }
        }
    }

    return NULL;
}

/**
 * Allocates a new dialog control resize info structure and assigns it to the
 * end of table.
 *
 * @param c
 * @param prid
 * @param ctrlID
 *
 * @return A pointer to the newly allocated structure on success, null on error.
 *
 * @assumes The table has already been checked and does not contain a struct for
 *          this control.
 */
pResizeInfoCtrl allocCtrlInfo(RexxThreadContext *c, pResizeInfoDlg prid, int32_t ctrlID, HWND hCtrl, oodControl_t ctrlType)
{
    size_t index = prid->countCtrls;
    if ( index >= prid->tableSize )
    {
        HLOCAL temp = LocalReAlloc(prid->riCtrls, sizeof(ResizeInfoCtrl) * prid->tableSize * 2, LMEM_ZEROINIT | LMEM_MOVEABLE);
        if ( temp == NULL )
        {
            MessageBox(0, "Resizable control entrires have exceeded the maximum\n"
                          "number of allocated table entries, and the table could\n"
                          "not be expanded.\n\n"
                          "No more control entries can be added.\n",
                       "Error", MB_OK | MB_ICONHAND);

            return NULL;
        }

        prid->tableSize *= 2;
        prid->riCtrls = (pResizeInfoCtrl)temp;
    }

    pResizeInfoCtrl ric = &prid->riCtrls[index];
    prid->countCtrls++;

    memcpy(&ric->edges, &prid->defEdges, sizeof(ControlEdges));

    ric->hCtrl    = hCtrl;
    ric->id       = ctrlID;
    ric->ctrlType = ctrlType;

    return ric;
}

/**
 * Allocates the ResizeInfoDlg struct and sets up the default values.  Also
 * passes on the CSelf buffer to be set as a context variable for the
 * ResizingAdmin object.
 *
 * @param c
 * @param pcpbd
 * @param cselfBuffer
 *
 * @return bool
 *
 * @note that cselfBuffer is the Rexx buffer object containing CPlainBaseDialog.
 */
bool allocateResizeInfo(RexxMethodContext *c, pCPlainBaseDialog pcpbd, RexxBufferObject cselfBuffer)
{
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

    prid->minSizeIsInitial = true;

    pcpbd->resizeInfo = prid;

    RexxObjectPtr reply = c->SendMessage1(pcpbd->rexxSelf, "INITRESIZING", cselfBuffer);
    if ( reply != TheZeroObj )
    {
        goto err_out;
    }

    if ( TheConstDirUsage == globalNever )
    {
        RexxDirectoryObject constDir = (RexxDirectoryObject)c->SendMessage0(pcpbd->rexxSelf, "CONSTDIR");
        if ( constDir != NULLOBJECT )
        {
            c->DirectoryPut(constDir, c->Int32(IDC_DEFAULT_PINTO_WINDOW), "IDC_DEFAULT_PINTO_WINDOW");
        }
    }
    else
    {
        c->DirectoryPut(TheConstDir, c->Int32(IDC_DEFAULT_PINTO_WINDOW), "IDC_DEFAULT_PINTO_WINDOW");
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

int calcRectSize(pinnedEdge_t e, RECT *rect)
{
    switch ( e )
    {
        case leftEdge :
        case rightEdge :
        case xCenterEdge :
            return rect->right - rect->left;

        case topEdge :
        case bottomEdge :
        case yCenterEdge :
            return rect->bottom - rect->top;

        default :
            break;
    }
    printf("calcRectSize() major error\n");
    return 0;
}

RECT *getRect(uint32_t id, pResizeInfoDlg prid, bool initial)
{
    if ( id == 0 )
    {
        return initial ? &prid->originalRect : &prid->currentRect;
    }

    for ( register size_t i = 0; i < prid->countCtrls; i++)
    {
        if ( prid->riCtrls[i].id == id )
        {
            return initial ? &prid->riCtrls[i].originalRect : &prid->riCtrls[i].currentRect;
        }
    }
    printf("BLOW UP!!\n");
    return NULL;
}

void recalcSizePosition(pResizeInfoDlg prid, pResizeInfoCtrl ric)
{
    RECT *pinToRectInitial;
    RECT *pinToRectCurrent;

    pEdge edge = &ric->edges.left;
    pinToRectInitial = getRect(edge->pinToID, prid, true);
    pinToRectCurrent = getRect(edge->pinToID, prid, false);

    int oldSize = calcRectSize(leftEdge, pinToRectInitial);
    int newSize = calcRectSize(leftEdge, pinToRectCurrent);
    double factor = (double)newSize / oldSize;

#pragma warning(disable:4244)

    ric->currentRect.left = pinToRectCurrent->left + ((ric->originalRect.left - pinToRectInitial->left) * (factor));

    //printf("ID=%d, original left=%d, new left=%d\n", ric->id, ric->originalRect.left, ric->currentRect.left);

    edge = &ric->edges.top;
    pinToRectInitial = getRect(edge->pinToID, prid, true);
    pinToRectCurrent = getRect(edge->pinToID, prid, false);

    oldSize = calcRectSize(topEdge, pinToRectInitial);
    newSize = calcRectSize(topEdge, pinToRectCurrent);
    factor  = (double)newSize / oldSize;
    ric->currentRect.top = pinToRectCurrent->top + ((ric->originalRect.top - pinToRectInitial->top) * (factor));

    //printf("ID=%d, original top=%d, new top=%d\n", ric->id, ric->originalRect.top, ric->currentRect.top);

    edge = &ric->edges.right;
    pinToRectInitial = getRect(edge->pinToID, prid, true);
    pinToRectCurrent = getRect(edge->pinToID, prid, false);

    oldSize = calcRectSize(rightEdge, pinToRectInitial);
    newSize = calcRectSize(rightEdge, pinToRectCurrent);
    factor  = (double)newSize / oldSize;
    ric->currentRect.right = pinToRectCurrent->right + ((ric->originalRect.right - pinToRectInitial->right) * (factor));

    //printf("ID=%d, original right=%d, new right=%d\n", ric->id, ric->originalRect.right, ric->currentRect.right);

    edge = &ric->edges.bottom;
    pinToRectInitial = getRect(edge->pinToID, prid, true);
    pinToRectCurrent = getRect(edge->pinToID, prid, false);

    oldSize = calcRectSize(bottomEdge, pinToRectInitial);
    newSize = calcRectSize(bottomEdge, pinToRectCurrent);
    factor  = (double)newSize / oldSize;
    ric->currentRect.bottom = pinToRectCurrent->bottom + ((ric->originalRect.bottom - pinToRectInitial->bottom) * (factor));

#pragma warning(default:4244)

    //printf("ID=%d, original bottom=%d, new bottom=%d\n", ric->id, ric->originalRect.bottom, ric->currentRect.bottom);

    return;
}

HDWP deferPosition(HDWP hdwp, pResizeInfoCtrl ric)
{
    // Make sure the window exists.
    if ( ! IsWindow(ric->hCtrl) )
    {
        return hdwp;
    }

    int32_t left   = ric->currentRect.left;
    int32_t top    = ric->currentRect.top;
    int32_t right  = ric->currentRect.right;
    int32_t bottom = ric->currentRect.bottom;

    int32_t width  = right - left;
    int32_t height = bottom - top;

    // Only defer visible winodws.
    if ( IsWindowVisible(ric->hCtrl) )
    {
        return DeferWindowPos(hdwp, ric->hCtrl, NULL, left, top, width, height, SWP_NOZORDER | SWP_NOACTIVATE);
    }

    // For not visible windows, just move them.
    MoveWindow(ric->hCtrl, left, top, width, height, FALSE);

    return hdwp;
}

bool resizeAll(pResizeInfoDlg prid)
{
    // Use defer window postion to resize / repositon all controls at once.
    HDWP hdwp = BeginDeferWindowPos((int32_t)prid->countCtrls);
    if ( hdwp == NULL )
    {
        return false;
    }

    for ( size_t i = 0; i < prid->countCtrls; i++)
    {
        hdwp = deferPosition(hdwp, &prid->riCtrls[i]);
        if ( hdwp == NULL )
        {
            return false;
        }
    }

    return EndDeferWindowPos(hdwp) ? true : false;
}

BOOL resizeAndPosition(pCPlainBaseDialog pcpbd, HWND hDlg, long cx, long cy)
{
    pResizeInfoDlg prid = pcpbd->resizeInfo;

    prid->currentRect.right  = cx;
    prid->currentRect.bottom = cy;
    for ( size_t i = 0; i < prid->countCtrls; i++)
    {
        recalcSizePosition(prid, &prid->riCtrls[i]);
    }

    return resizeAll(prid) ? TRUE : FALSE;
}

/**
 * This is the callback function for our EnumChildWindows() call in
 * WM_INITDIALOG.  For each child window
 *
 * @param hCtrl
 * @param lParam
 *
 * @return True to continue the enumeration, false to halt the enumeration.
 *
 * @note  We only want direct children of the dialog.  Some controls have their
 *        own child windows.  These are enumerated by EnumChildWindows, but we
 *        can not work with them.  For instance, a list-view with a column
 *        header has the header control as a child.  We can not work with the
 *        header control.  So we skip any child window whose parent is not the
 *        dialog.
 */
BOOL CALLBACK InitializeAllControlsProc(HWND hCtrl, LPARAM lParam)
{
    pCPlainBaseDialog pcpbd = (pCPlainBaseDialog)lParam;
    pResizeInfoDlg    prid  = pcpbd->resizeInfo;

    if ( GetParent(hCtrl) != pcpbd->hDlg )
    {
        return TRUE;
    }

    oodControl_t ctrlType = control2controlType(hCtrl);
    int          ctrlID   = GetDlgCtrlID(hCtrl);

    if ( ctrlType == winGroupBox )
    {
        SetWindowLong(hCtrl, GWL_EXSTYLE, GetWindowLong(hCtrl, GWL_EXSTYLE) | WS_EX_TRANSPARENT);
    }

    pResizeInfoCtrl ric = getControlInfo(prid, ctrlID);
    if ( ric == NULL )
    {
        ric = allocCtrlInfo(pcpbd->dlgProcContext, prid, ctrlID, hCtrl, ctrlType);
        if ( ric == NULL )
        {
            prid->haveError = true;
            return FALSE;
        }
    }
    else
    {
        ric->hCtrl    = hCtrl;
        ric->ctrlType = ctrlType;
    }

    GetWindowRect(hCtrl, &ric->originalRect);
    MapWindowPoints(NULL, pcpbd->hDlg, (POINT *)&ric->originalRect, 2);

#ifdef EXTRA_DEBUG
    printf("Control hwnd=%p type=%s id=%d rect=(%d, %d, %d, %d)\n", hCtrl, controlType2controlName(ctrlType), ric->id,
           ric->originalRect.left, ric->originalRect.top, ric->originalRect.right, ric->originalRect.bottom);
#endif

    return TRUE;
}

/**
 *  Called from WM_INITDIALOG to finish the initialization of a resizable
 *  dialog.
 *
 *  Checks that the user did not set a dialog control's pin to window with an
 *  invalid ID. Gets the original window rectangles for the dialog and the
 *  controls. Enumerates all child windows of the dialog to either fix up their
 *  existing control info structs, or to add a record for any controls not
 *  specified by the user.  And does any other needed miscellaneous set up.
 *
 * @param hDlg
 * @param c
 * @param pcpbd
 *
 * @return LRESULT
 */
LRESULT initializeResizableDialog(HWND hDlg, RexxThreadContext *c, pCPlainBaseDialog pcpbd)
{
    pResizeInfoDlg prid = pcpbd->resizeInfo;

    if ( ! verifyPinToWindows(c, hDlg, prid) )
    {
        endDialogPremature(pcpbd, hDlg, RexxConditionRaised);
        return FALSE;
    }

    setResizableDlgStyle(hDlg);

    GetClientRect(hDlg, &prid->originalRect);

    if ( prid->minSizeIsInitial )
    {
        prid->minSize.cx  = prid->originalRect.right;
        prid->minSize.cy  = prid->originalRect.bottom;
        prid->haveMinSize = true;
    }

    EnumChildWindows(hDlg, InitializeAllControlsProc, (LPARAM)pcpbd);

    return prid->haveError ? FALSE : TRUE;
}

/**
 * The window message processing procedure for dialogs that have inherited
 * ResizingAdmin.
 *
 * This is based on the normal Rexx dialog message processing procedure,
 * RexxDlgProc() except that it intercepts WM_xxx messages related to resizing.
 * These messages are processed here, always, and they are never used to invoke
 * a Rexx dialog method.  This implies that if the user were to connect, say
 * WM_SIZE, no method would be invoked.
 *
 * Currently there is no message processing procedure specifically for
 * ControlDialog dialogs that might inherit ResizingAdmin.  Resizable
 * ControlDialog dialogs will probably not work.
 *
 * @param hDlg
 * @param uMsg
 * @param wParam
 * @param lParam
 *
 * @return LRESULT CALLBACK
 */
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

        return initializeResizableDialog(hDlg, pcpbd->dlgProcContext, pcpbd);
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

    // We first deal with resizable stuff, then handle the rest with the normal
    // ooDialog process.
    pResizeInfoDlg prid = pcpbd->resizeInfo;
    switch ( uMsg )
    {
        case WM_ENTERSIZEMOVE :
            prid->inSizeOrMove = true;
            return FALSE;

        case WM_NCCALCSIZE :
            if ( wParam && prid->inSizeOrMove )
            {
                NCCALCSIZE_PARAMS * nccs_params = (NCCALCSIZE_PARAMS *)lParam;

                // Have the default window procedure calculate the client
                // co-ordinates.
                DefWindowProc(hDlg, uMsg, FALSE, (LPARAM)&nccs_params->rgrc[0]);

                // Set the source & target rectangles to be the same.
                nccs_params->rgrc[1] = nccs_params->rgrc[2];

                return WVR_ALIGNLEFT | WVR_ALIGNTOP;
            }
            return FALSE;

        case WM_SIZE :
            if ( prid->inSizeOrMove || wParam == SIZE_MAXIMIZED || wParam == SIZE_RESTORED )
            {
                return resizeAndPosition(pcpbd, hDlg, LOWORD(lParam), HIWORD(lParam));
            }
            return FALSE;

        case WM_EXITSIZEMOVE :
            prid->inSizeOrMove = false;
            return FALSE;

        case WM_GETMINMAXINFO :
        {
            MINMAXINFO *pmmi = (MINMAXINFO *)lParam;

            if ( prid->haveMinSize )
            {
                pmmi->ptMinTrackSize.x = prid->minSize.cx;
                pmmi->ptMinTrackSize.y = prid->minSize.cy;
            }

            if ( prid->haveMaxSize )
            {
                pmmi->ptMaxTrackSize.x = prid->maxSize.cx;
                pmmi->ptMaxTrackSize.y = prid->maxSize.cy;
            }

            return TRUE;
        }

        default :
            break;
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


/** Resizing::maxSize          [attribute]
 */
RexxMethod1(RexxObjectPtr, ra_maxSize, CSELF, pCSelf)
{
    pResizeInfoDlg    prid = NULL;
    pCPlainBaseDialog pcpbd = validateRACSelf(context, pCSelf, &prid);
    if ( pcpbd == NULL )
    {
        return NULLOBJECT;
    }

    return rxNewSize(context, &prid->maxSize);
}
RexxMethod2(RexxObjectPtr, ra_setMaxSize, RexxObjectPtr, _size, CSELF, pCSelf)
{
    pResizeInfoDlg    prid = NULL;
    pCPlainBaseDialog pcpbd = validateRACSelf(context, pCSelf, &prid);
    if ( pcpbd == NULL )
    {
        return NULLOBJECT;
    }

    PSIZE s = rxGetSize(context, _size, 1);
    if ( s != NULL )
    {
        prid->maxSize.cx = s->cx;
        prid->maxSize.cy = s->cy;
        prid->haveMaxSize;
    }

    return NULLOBJECT;
}

/** Resizing::minSize          [attribute]
 */
RexxMethod1(RexxObjectPtr, ra_minSize, CSELF, pCSelf)
{
    pResizeInfoDlg    prid = NULL;
    pCPlainBaseDialog pcpbd = validateRACSelf(context, pCSelf, &prid);
    if ( pcpbd == NULL )
    {
        return NULLOBJECT;
    }

    return rxNewSize(context, &prid->minSize);
}
RexxMethod2(RexxObjectPtr, ra_setMinSize, RexxObjectPtr, _size, CSELF, pCSelf)
{
    pResizeInfoDlg    prid = NULL;
    pCPlainBaseDialog pcpbd = validateRACSelf(context, pCSelf, &prid);
    if ( pcpbd == NULL )
    {
        return NULLOBJECT;
    }

    PSIZE s = rxGetSize(context, _size, 1);
    if ( s != NULL )
    {
        prid->minSize.cx = s->cx;
        prid->minSize.cy = s->cy;
        prid->haveMinSize;
        prid->minSizeIsInitial = false;
    }

    return NULLOBJECT;
}

/** ResizingAdmin::defaultLeft()
 ** ResizingAdmin::defaultTop()
 ** ResizingAdmin::defaultRight()
 ** ResizingAdmin::defaultBottom()
 *
 */
RexxMethod4(RexxObjectPtr, ra_defaultSide, CSTRING, howPinned, CSTRING, whichEdge, NAME, method, CSELF, pCSelf)
{
    pResizeInfoDlg    prid = NULL;
    pCPlainBaseDialog pcpbd = validateRACSelf(context, pCSelf, &prid);
    if ( pcpbd == NULL )
    {
        return TheOneObj;
    }

    if ( ! prid->inDefineSizing )
    {
        return methodCanOnlyBeInvokedException(context, method, "during the defineSizing method", pcpbd->rexxSelf);
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
    pResizeInfoDlg    prid  = NULL;
    pCPlainBaseDialog pcpbd = validateRACSelf(context, pCSelf, &prid);
    if ( pcpbd == NULL )
    {
        return TheOneObj;
    }

    if ( ! prid->inDefineSizing )
    {
        return methodCanOnlyBeInvokedException(context, "defaultSizing", "during the defineSizing method", pcpbd->rexxSelf);
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


/** ResizingAdmin::initResizing()  [private]
 *
 *  Used to set the CSelf Rexx buffer object as a context variable of this
 *  object.
 *
 *  @note For internal use only.
 */
RexxMethod2(RexxObjectPtr, ra_initResizing, RexxObjectPtr, arg, OSELF, self)
{
    RexxMethodContext *c = context;

    if ( ! context->IsBuffer(arg) )
    {
        baseClassInitializationException(context, "ResizingAdmin");
        return TheOneObj;
    }

    context->SetObjectVariable("CSELF", arg);

    return TheZeroObj;
}


