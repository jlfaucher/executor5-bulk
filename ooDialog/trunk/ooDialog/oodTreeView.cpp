/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 1995, 2004 IBM Corporation. All rights reserved.             */
/* Copyright (c) 2005-2012 Rexx Language Association. All rights reserved.    */
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
 * oodTreeView.cpp
 *
 * Contains methods for the tree-view control.
 */
#include "ooDialog.hpp"     // Must be first, includes windows.h, commctrl.h, and oorexxapi.h

#include <shlwapi.h>

#include "APICommon.hpp"
#include "oodCommon.hpp"
#include "oodMessaging.hpp"
#include "oodControl.hpp"
#include "oodResources.hpp"


/**
 *  Methods for the .TreeView class.
 */
#define TREEVIEW_CLASS            "TreeView"

#define TVSTATE_ATTRIBUTE         "TV!STATEIMAGELIST"
#define TVNORMAL_ATTRIBUTE        "TV!NORMALIMAGELIST"

static CSTRING tvGetAttributeName(uint8_t type)
{
    switch ( type )
    {
        case TVSIL_STATE :
            return TVSTATE_ATTRIBUTE;
        case TVSIL_NORMAL :
        default :
            return TVNORMAL_ATTRIBUTE;
    }
}


static void parseTvModifyOpts(CSTRING opts, TVITEMEX *tvi)
{
    if ( StrStrI(opts, "NOTBOLD") != NULL )
    {
        tvi->stateMask |= TVIS_BOLD;
    }
    else if ( StrStrI(opts, "BOLD") != NULL )
    {
        tvi->state |= TVIS_BOLD;
        tvi->stateMask |= TVIS_BOLD;
    }

    if ( StrStrI(opts, "NOTDROP") != NULL )
    {
        tvi->stateMask |= TVIS_DROPHILITED;
    }
    else if ( StrStrI(opts, "DROP") != NULL )
    {
        tvi->state |= TVIS_DROPHILITED;
        tvi->stateMask |= TVIS_DROPHILITED;
    }

    if ( StrStrI(opts, "NOTSELECTED") != NULL )
    {
        tvi->stateMask |= TVIS_SELECTED;
    }
    else if ( StrStrI(opts, "SELECTED") != NULL )
    {
        tvi->state |= TVIS_SELECTED;
        tvi->stateMask |= TVIS_SELECTED;
    }

    if ( StrStrI(opts, "NOTCUT") != NULL )
    {
        tvi->stateMask |= TVIS_CUT;
    }
    else if ( StrStrI(opts, "CUT") != NULL )
    {
        tvi->state |= TVIS_CUT;
        tvi->stateMask |= TVIS_CUT;
    }

    if ( StrStrI(opts, "NOTEXPANDEDONCE") != NULL )
    {
        tvi->stateMask |= TVIS_EXPANDEDONCE;
    }
    else if ( StrStrI(opts, "EXPANDEDONCE") != NULL )
    {
        tvi->state |= TVIS_EXPANDEDONCE;
        tvi->stateMask |= TVIS_EXPANDEDONCE;
    }
    else if ( StrStrI(opts, "NOTEXPANDED") != NULL )
    {
        tvi->stateMask |= TVIS_EXPANDED;
    }
    else if ( StrStrI(opts, "EXPANDED") != NULL )
    {
        tvi->state |= TVIS_EXPANDED;
        tvi->stateMask |= TVIS_EXPANDED;
    }
    else if ( StrStrI(opts, "NOTEXPANDPARTIAL") != NULL )
    {
        tvi->stateMask |= TVIS_EXPANDPARTIAL;
    }
    else if ( StrStrI(opts, "EXPANDPARTIAL") != NULL )
    {
        tvi->state |= TVIS_EXPANDPARTIAL;
        tvi->stateMask |= TVIS_EXPANDPARTIAL;
    }

    if ( tvi->state != 0 || tvi->stateMask != 0 )
    {
        tvi->mask |= TVIF_STATE;
    }
}

/**
 * Finds the first tree-view item whose text matches the specified text.
 *
 * @param hTv
 * @param text
 *
 * @return HTREEITEM
 *
 * @note  Although the text for a tree-view item can be any length, only 260
 *        characters are displayed.  The old IBM code used a huge buffer, twice
 *        4096.  I think we should limit the length of text a user can assign to
 *        a tree-view item, but for now we'll just go with 4096.
 */
HTREEITEM tvFindItem(HWND hTv, CSTRING text)
{
    if ( *text == '\0' )
    {
        return NULL;
    }

    HTREEITEM hTreeItem = NULL;
    HTREEITEM root      = TreeView_GetRoot(hTv);
    TVITEM    tvi       = {0};
    char      buff[4096];

    tvi.hItem = root;
    while ( tvi.hItem != NULL )
    {
         tvi.mask = TVIF_HANDLE | TVIF_TEXT | TVIF_CHILDREN;
         tvi.pszText = buff;
         tvi.cchTextMax = sizeof(buff) - 1;
         if ( TreeView_GetItem(hTv, &tvi) )
         {
             if ( stricmp(tvi.pszText, text) == 0 )
             {
                 return tvi.hItem;
             }
             else
             {
                 if ( tvi.cChildren > 0 )
                 {
                     hTreeItem = TreeView_GetChild(hTv, tvi.hItem);
                 }
                 else
                 {
                     hTreeItem = TreeView_GetNextSibling(hTv, tvi.hItem);
                 }

                 while ( hTreeItem == NULL && tvi.hItem != NULL )
                 {
                     tvi.hItem = TreeView_GetParent(hTv, tvi.hItem);
                     hTreeItem = TreeView_GetNextSibling(hTv, tvi.hItem);
                     if ( hTreeItem == root )
                     {
                         return NULL;
                     }
                 }

                 if ( tvi.hItem == NULL )
                 {
                     return NULL;
                 }
                 tvi.hItem = hTreeItem;
             }
         }
         else
         {
             tvi.hItem = NULL;
         }
    }

    return NULL;
}

/**
 * Returns the lParam user data for the specified tree-view item as a Rexx
 * object
 *
 * @param hTree
 *
 * @return The Rexx object set as the user data, or the .nil object if no user
 *         data is set.
 */
static RexxObjectPtr getCurrentTviUserData(HWND hTree, HTREEITEM hTreeItem)
{
    TVITEMEX      tvi    = {TVIF_PARAM, hTreeItem};
    RexxObjectPtr result = TheNilObj;

    if ( TreeView_GetItem(hTree, &tvi) != 0 )
    {
        if ( tvi.lParam != 0 )
        {
            result = (RexxObjectPtr)tvi.lParam;
        }
    }
    return result;
}


/**
 * This is the window procedure used to subclass the edit control used for label
 * editing for the TreeView.  We need to add the want all keys for
 * WM_GETDLGCODE, or else ESC and ENTER close the dialog rather than end the
 * editing session.
 *
 * Unlike other ooDialog subclass procedures, we don't need any extra data for
 * this one.  dwData is null.  The edit control is destroyed by the OS as soon
 * as the label editing is ended, so this subclass is very transient.  We
 * recieve the WM_NCDESTROY message and remove the subclass.
 */
LRESULT CALLBACK CatchReturnSubProc(HWND hwnd, uint32_t msg, WPARAM wParam, LPARAM lParam, UINT_PTR id, DWORD_PTR dwData)
{
    switch ( msg )
    {
        case WM_GETDLGCODE:
            return(DLGC_WANTALLKEYS | DefSubclassProc(hwnd, msg, wParam, lParam));

        case WM_NCDESTROY:
        {
            RemoveWindowSubclass(hwnd, CatchReturnSubProc, id);
        }
    }

    return DefSubclassProc(hwnd, msg, wParam, lParam);
}


/**
 *  Subclasses the edit control that is created when the user begins editing a
 *  tree-view item label.  Without the subclass, if the user hits the ENTER or
 *  ESC key while editing, the dialog is closed.
 *
 *  This is an older ooDialog method which used to be used for a list-view also.
 *  It is not needed for a list-view.
 *
 *  @return  True on success, otherwise false.
 *
 *  @remarks  We are called from the window message processing loop, so we know
 *            we are in the proper thread.
 *
 *            We try to get the edit contol's ID. If we fail we use the
 *            tree-view's id.
 */
bool tvSubclassEdit(HWND hTV, HWND hEdit, uintptr_t tvID)
{
    if ( hEdit == NULL )
    {
        return false;
    }

    int _id      = GetDlgCtrlID(hEdit);
    uintptr_t id = _id < 1 ? tvID : _id;

    if ( SetWindowSubclass(hEdit, CatchReturnSubProc, id, NULL) == 0 )
    {
        return false;
    }

    return true;
}


/**
 * A tree view item sort callback function that works by invoking a method in
 * the Rexx dialog that does the actual comparison.
 *
 * @param lParam1
 * @param lParam2
 * @param lParamSort
 *
 * @return int32_t
 *
 * @remarks  The Rexx programmer will have had to set the lParam user data field
 *           for each tree view item of this to work.  If either lParam1 or
 *           lParam2 is null, indicating the user did not set a data value for
 *           the item, we simpley return 0.
 *
 *           Testing shows that this call back is always invoked on the dialog's
 *           window message processing thread, so we do no need to worry about
 *           doing an AttachThread().
 *
 *           Testing with the list-view compare function shows that doing
 *           ReleaseLocalReference, significantly increases the time it takes to
 *           sort 1000 items. Using the elapsed time count in the the Rexx shows
 *           shows an increase from 4.9 seconds to 6.9 seconds.  Testing with
 *           the tree-view sort function has not been doen.
 */
int32_t CALLBACK TvRexxCompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
    if ( lParam1 == NULL || lParam2 == NULL )
    {
        return 0;
    }

    pCRexxSort pcrs = (pCRexxSort)lParamSort;
    RexxThreadContext *c = pcrs->threadContext;

    RexxArrayObject args = c->ArrayOfThree((RexxObjectPtr)lParam1, (RexxObjectPtr)lParam2, pcrs->param);

    RexxObjectPtr reply = c->SendMessage(pcrs->rexxDlg, pcrs->method, args);
    c->ReleaseLocalReference(args);

    if ( msgReplyIsGood(c, pcrs->pcpbd, reply, pcrs->method, false) )
    {
        int32_t answer;
        if ( c->Int32(reply, &answer) )
        {
            c->ReleaseLocalReference(reply);
            return answer;
        }

        // We end the dialog on this error.
        wrongReplyRangeException(c, pcrs->method, INT32_MIN, INT32_MAX, reply);
        c->ReleaseLocalReference(reply);

        endDialogPremature(pcrs->pcpbd, pcrs->pcpbd->hDlg, RexxConditionRaised);
    }
    else
    {
        if ( reply != NULLOBJECT )
        {
            c->ReleaseLocalReference(reply);
        }
    }

    // There was some error if we are here ...
    return 0;
}

/**
 * Sets things up for a tree-view sort when the comparison function will invoke
 * a method in the Rexx dialog to do the actual sorting.
 *
 * @param context
 * @param sortInfo
 * @param pcdc
 *
 * @return logical_t
 *
 * @note Testing shows that using a method in the Rexx dialog to do the sort
 *       works well and seems fast with a smallish number of tree-view items.
 *
 *       However, the similar list-view sort, with large lists it is slow.  This
 *       can be seen with lists containing a 1000 item.  Haven't done any
 *       testing with a tree-view yet.
 *
 * @remarks  The TreeView_SortChildrenCB() documentes the third argument as
 *           being: fRecurse Reserved. Must be zero.  However, if it is 0 then
 *           the children of the item do not sort if the item is expanded.  They
 *           do sort if the item is closed up.  Testing shows that the compare
 *           function is never invoked.
 *
 *           But, if I make the 3rd argument TRUE, the the items always sort,
 *           whether the parent is closed or expanded.  Not sure what the deal
 *           is there, if it is just a MS doc problem, or what.
 */
logical_t rexxTreeViewSort(RexxMethodContext *context, HTREEITEM hItem, CSTRING method,
                           RexxObjectPtr sortInfo, pCDialogControl pcdc)
{
    pCRexxSort pcrs = pcdc->pcrs;
    if ( pcrs == NULL )
    {
        pcrs = (pCRexxSort)LocalAlloc(LPTR, sizeof(CRexxSort));
        if ( pcrs == NULL )
        {
            outOfMemoryException(context->threadContext);
            return FALSE;
        }
        pcdc->pcrs = pcrs;
    }

    safeLocalFree(pcrs->method);
    memset(pcrs, 0, sizeof(CRexxSort));

    pcrs->method = (char *)LocalAlloc(LPTR, strlen(method) + 1);
    if ( pcrs->method == NULL )
    {
        outOfMemoryException(context->threadContext);
        return FALSE;
    }

    strcpy(pcrs->method, method);
    pcrs->pcpbd         = pcdc->pcpbd;
    pcrs->rexxDlg       = pcdc->pcpbd->rexxSelf;
    pcrs->rexxCtrl      = pcdc->rexxSelf;
    pcrs->threadContext = pcdc->pcpbd->dlgProcContext;
    pcrs->hItem         = hItem;
    pcrs->param         = (argumentExists(3) ? sortInfo : TheNilObj);

    TVSORTCB tvscb;
    tvscb.hParent     = hItem;
    tvscb.lParam      = (LPARAM)pcrs;
    tvscb.lpfnCompare = TvRexxCompareFunc;

    return TreeView_SortChildrenCB(pcdc->hCtrl, &tvscb, TRUE);
}

/** TreeView::delete()
 *
 *
 *  @returns  0 on sucees, 1 on error, and -1 if hItem is not valid.
 *
 *  @remarks  The return codes for this are not optimal.  But they are what was
 *            documented and used in the original ooDialog from IBM.
 *
 */
RexxMethod2(RexxObjectPtr, tv_delete, CSTRING, _hItem, CSELF, pCSelf)
{
    pCDialogControl pcdc = validateDCCSelf(context, pCSelf);
    if ( pcdc == NULL )
    {
        return TheNegativeOneObj;
    }

    HTREEITEM hItem = (HTREEITEM)string2pointer(_hItem);
    if ( hItem == NULL )
    {
        return TheNegativeOneObj;
    }
    if ( TreeView_GetCount(pcdc->hCtrl) < 1 )
    {
        return TheNegativeOneObj;
    }

    unProtectControlUserData(context, pcdc, getCurrentTviUserData(pcdc->hCtrl, hItem));

    return TreeView_DeleteItem(pcdc->hCtrl, hItem) ? TheZeroObj : TheOneObj;
}

/** TreeView::deleteAll()
 *
 *  @returns  0 on success, 1 on error.  These are the original ooDialog return
 *            codes
 *
 */
RexxMethod1(RexxObjectPtr, tv_deleteAll, CSELF, pCSelf)
{
    pCDialogControl pcdc = validateDCCSelf(context, pCSelf);
    if ( pcdc == NULL )
    {
        return TheOneObj;
    }

    if ( pcdc->rexxBag != NULL )
    {
        context->SendMessage0(pcdc->rexxBag, "EMPTY");
    }

    return TreeView_DeleteAllItems(pcdc->hCtrl) ? TheZeroObj : TheOneObj;
}

/** TreeView::insert()
 *
 */
RexxMethod9(RexxObjectPtr, tv_insert, OPTIONAL_CSTRING, _hItem, OPTIONAL_CSTRING, _hAfter, OPTIONAL_CSTRING, label,
            OPTIONAL_int32_t, imageIndex, OPTIONAL_int32_t, selectedImage, OPTIONAL_CSTRING, opts, OPTIONAL_uint32_t, children,
            OPTIONAL_RexxObjectPtr, userData, CSELF, pCSelf)
{
    HWND hwnd  = getDChCtrl(pCSelf);

    TVINSERTSTRUCT  ins;
    TVITEMEX       *tvi = &ins.itemex;

    if ( argumentExists(1) )
    {
        if ( stricmp(_hItem, "ROOT") == 0 )
        {
            ins.hParent = TVI_ROOT;
        }
        else
        {
            ins.hParent = (HTREEITEM)string2pointer(_hItem);
        }
    }
    else
    {
        ins.hParent = TVI_ROOT;
    }

    if ( argumentExists(2) )
    {
        if ( stricmp(_hAfter,      "FIRST") == 0 ) ins.hInsertAfter = TVI_FIRST;
        else if ( stricmp(_hAfter, "SORT")  == 0 ) ins.hInsertAfter = TVI_SORT;
        else if ( stricmp(_hAfter, "LAST")  == 0 ) ins.hInsertAfter = TVI_LAST;
        else ins.hInsertAfter = (HTREEITEM)string2pointer(_hAfter);
    }
    else
    {
        ins.hInsertAfter = TVI_LAST;
    }

    memset(tvi, 0, sizeof(TVITEMEX));

    label         = (argumentOmitted(3) ? "" : label);
    imageIndex    = (argumentOmitted(4) ? -1 : imageIndex);
    selectedImage = (argumentOmitted(5) ? -1 : selectedImage);

    tvi->mask = TVIF_TEXT;
    tvi->pszText = (LPSTR)label;
    tvi->cchTextMax = (int)strlen(label);

    if ( imageIndex > -1 )
    {
        tvi->iImage = imageIndex;
        tvi->mask |= TVIF_IMAGE;
    }
    if ( selectedImage > -1 )
    {
        tvi->iSelectedImage = selectedImage;
        tvi->mask |= TVIF_SELECTEDIMAGE;
    }

    if ( argumentExists(6) )
    {
        if ( StrStrI(opts, "BOLD")     != NULL ) tvi->state |= TVIS_BOLD;
        if ( StrStrI(opts, "EXPANDED") != NULL ) tvi->state |= TVIS_EXPANDED;

        if ( tvi->state != 0 )
        {
            tvi->stateMask = tvi->state;
            tvi->mask |= TVIF_STATE;
        }
    }
    if ( children > 0 )
    {
        tvi->cChildren = children;
        tvi->mask |= TVIF_CHILDREN;
    }
    if ( argumentExists(8) && userData != TheNilObj)
    {
        protectControlUserData(context, (pCDialogControl)pCSelf, userData);

        tvi->lParam = (LPARAM)userData;
        tvi->mask |= TVIF_PARAM;
    }

    return pointer2string(context, TreeView_InsertItem(hwnd, &ins));
}


/** TreeView::itemInfo()
 *
 */
RexxMethod2(RexxObjectPtr, tv_itemInfo, CSTRING, _hItem, CSELF, pCSelf)
{
    HWND hwnd  = getDChCtrl(pCSelf);

    TVITEM tvi = {0};
    char buf[256];

    tvi.hItem = (HTREEITEM)string2pointer(_hItem);
    tvi.mask = TVIF_HANDLE | TVIF_TEXT | TVIF_STATE | TVIF_IMAGE | TVIF_CHILDREN | TVIF_SELECTEDIMAGE | TVIF_PARAM;
    tvi.pszText = buf;
    tvi.cchTextMax = 255;
    tvi.stateMask = TVIS_EXPANDED | TVIS_BOLD | TVIS_SELECTED | TVIS_EXPANDEDONCE | TVIS_DROPHILITED | TVIS_CUT | TVIS_EXPANDPARTIAL;

    if ( TreeView_GetItem(hwnd, &tvi) == 0 )
    {
        return TheNegativeOneObj;
    }

    RexxStemObject stem = context->NewStem("InternalTVItemInfo");

    context->SetStemElement(stem, "!TEXT", context->String(tvi.pszText));
    context->SetStemElement(stem, "!CHILDREN", (tvi.cChildren > 0 ? TheTrueObj : TheFalseObj));
    context->SetStemElement(stem, "!IMAGE", context->Int32(tvi.iImage));
    context->SetStemElement(stem, "!SELECTEDIMAGE", context->Int32(tvi.iSelectedImage));

    *buf = '\0';
    if ( tvi.state & TVIS_EXPANDED     ) strcat(buf, "EXPANDED ");
    if ( tvi.state & TVIS_BOLD         ) strcat(buf, "BOLD ");
    if ( tvi.state & TVIS_SELECTED     ) strcat(buf, "SELECTED ");
    if ( tvi.state & TVIS_EXPANDEDONCE ) strcat(buf, "EXPANDEDONCE ");
    if ( tvi.state & TVIS_DROPHILITED  ) strcat(buf, "INDROP ");
    if ( tvi.state & TVIS_CUT          ) strcat(buf, "CUT ");
    if ( tvi.state & TVIS_EXPANDPARTIAL) strcat(buf, "EXPANDPARTIAL ");
    if ( *buf != '\0' )
    {
        *(buf + strlen(buf) - 1) = '\0';
    }
    context->SetStemElement(stem, "!STATE", context->String(buf));

    context->SetStemElement(stem, "!USERDATA", tvi.lParam ? (RexxObjectPtr)tvi.lParam : TheNilObj);

    return stem;
}


/** TreeView::getImageList()
 *
 *  Gets the tree-view's specifed image list.
 *
 *  @param  type [optional] Identifies which image list to get, normal, or
 *               state. Normal is the default.
 *
 *  @return  The image list, if it exists, otherwise .nil.
 */
RexxMethod2(RexxObjectPtr, tv_getImageList, OPTIONAL_uint8_t, type, OSELF, self)
{
    if ( argumentOmitted(1) )
    {
        type = TVSIL_NORMAL;
    }
    else if ( type != TVSIL_STATE && type != TVSIL_NORMAL )
    {
        return invalidTypeException(context->threadContext, 2, "TVSIL_XXX flag");
    }

    RexxObjectPtr result = context->GetObjectVariable(tvGetAttributeName(type));
    if ( result == NULLOBJECT )
    {
        result = TheNilObj;
    }
    return result;
}


/** TreeView::getItemData()
 *
 *  Returns the user data associated with the specified tree-view item, or .nil
 *  if there is no user data associated.
 *
 *  @param  hItem  [required]  The handle of the item whose user data is to be
 *                 retrieved.
 *
 *  @return  Returns the associated user data, or .nil if there is no associated
 *           data.
 */
RexxMethod2(RexxObjectPtr, tv_getItemData, CSTRING, _hItem, CSELF, pCSelf)
{
    HWND      hwnd  = getDChCtrl(pCSelf);
    HTREEITEM hItem = (HTREEITEM)string2pointer(_hItem);

    return getCurrentTviUserData(hwnd, hItem);
}


/** TreeView::getItemHeight()
 *
 *  Returns the current height of the tree-view items
 *  if there is no user data associated.
 *
 *  @return  Returns the current height of the tree-view items in pixels.
 *
 *  @remarks  The tree-view uses the value returned for the height of all items.
 *            The value itself is the height of a single item.  See the
 *            TreeView::setItemHeight() method.
 */
RexxMethod1(int32_t, tv_getItemHeight, CSELF, pCSelf)
{
    HWND hwnd  = getDChCtrl(pCSelf);
    return TreeView_GetItemHeight(hwnd);
}


/** TreeView::getItemRect()
 *
 *  Retrieves the bounding rectangle for a tree-view item and indicates whether
 *  the item is visible.
 *
 *  @param  hItem  [required]  The handle of the item whose bounding rectangle
 *                 is to be retrieved.
 *
 *  @param  rect   [required in / out]  A .Rect object used to return the
 *                 bounding rectangle co-ordinates.  Co-ordinates are client
 *                 co-ordinaates.
 *
 *  @param  textOnly  [optional]  If textOnly is true, the bounding rectangle
 *                    returned is for the text portion only of the item.  If
 *                    false, the rectangle includes the entire line the item
 *                    occupies in the tree-view control.  If the argument is
 *                    omitted, the default is true.
 *
 *  @return  Returns true if the item specified is visible and the bounding
 *           rectangle is filled in.  Returns false if the item is not visible
 *           and the bounding rectangle is not filled in.
 *
 *  @remarks  The co-ordinates of the bounding rectangle are only filled in if
 *            the item is visible.
 */
RexxMethod4(logical_t, tv_getItemRect, CSTRING, _hItem, RexxObjectPtr, _rect, OPTIONAL_logical_t, textOnly, CSELF, pCSelf)
{
    if ( ! context->IsOfType(_rect, "RECT") )
    {
        wrongClassException(context->threadContext, 2, "Rect", _rect);
        return FALSE;
    }

    if ( argumentOmitted(3) )
    {
        textOnly = TRUE;
    }

    HWND      hwnd  = getDChCtrl(pCSelf);
    HTREEITEM hItem = (HTREEITEM)string2pointer(_hItem);
    PRECT     rect  = (PRECT)context->ObjectToCSelf(_rect);

    return TreeView_GetItemRect(hwnd, hItem, rect, textOnly);
}


/** TreeView::getItemPartRect()
 *
 *  Retrieves the largest possible bounding rectangle that constitutes the "hit
 *  zone" for a specified part of a tree-view item.
 *
 *  @param  hItem  [required]  The handle of the item whose bounding rectangle
 *                 is to be retrieved.
 *
 *  @param  rect   [required in / out]  A .Rect object used to return the
 *                 bounding rectangle co-ordinates.  Co-ordinates are client
 *                 co-ordinates
 *
 *  @param  part   [optional]  A keyword specifying which part of the item the
 *                 bounding rectangle is to be returned for.  Currently the
 *                 operating system only supplies one part ID, the button ID.
 *                 Because of this, the "part" argument is simply ignored and
 *                 the button ID is used.
 *
 *
 *  @return  Returns true on success, otherwise false.
 *
 *  @notes   Requires Windows Vista or later.
 *
 *           The largest possible bounding rectangle is such that for every
 *           (x,y) co-ordinate within the rectangle, a click by the user at that
 *           co-ordinate would constitute a hit on that part of the item.  Note
 *           that the only part is the button part.
 *
 *  @remarks  This does not work due to a bug in the Windows SDK.  Leaving the
 *            code here in case the bug is fixed.  If this is added back in,
 *            then tv_getItemPartRect will need to be added back to
 *            oodPackageEntry.cpp and TreeView.cls.
 */
#if 0
RexxMethod4(logical_t, tv_getItemPartRect, CSTRING, _hItem, RexxObjectPtr, _rect, OPTIONAL_CSTRING, part, CSELF, pCSelf)
{
    if ( ! context->IsOfType(_rect, "RECT") )
    {
        wrongClassException(context->threadContext, 2, "Rect", _rect);
        return FALSE;
    }

    if ( requiredOS(context, "getItemPartRect", "Vista", Vista_OS) )
    {
        HWND      hwnd  = getDChCtrl(pCSelf);
        HTREEITEM hItem = (HTREEITEM)string2pointer(_hItem);
        PRECT     rect  = (PRECT)context->ObjectToCSelf(_rect);

        return TreeView_GetItemPartRect(hwnd, hItem, rect, TVGIPR_BUTTON);
    }
    return FALSE;
}
#endif


RexxMethod2(RexxObjectPtr, tv_getSpecificItem, NAME, method, CSELF, pCSelf)
{
    HWND hwnd = getDChCtrl(pCSelf);
    HTREEITEM result = NULL;

    switch ( *method )
    {
        case 'R' :
            result = TreeView_GetRoot(hwnd);
            break;
        case 'S' :
            result = TreeView_GetSelection(hwnd);
            break;
        case 'D' :
            result = TreeView_GetDropHilight(hwnd);
            break;
        case 'F' :
            result = TreeView_GetFirstVisible(hwnd);
            break;
    }
    return pointer2string(context, result);
}


/** TreeView::getNextItem()
 *
 *
 */
RexxMethod3(RexxObjectPtr, tv_getNextItem, CSTRING, _hItem, NAME, method, CSELF, pCSelf)
{
    HWND      hwnd  = getDChCtrl(pCSelf);
    HTREEITEM hItem = (HTREEITEM)string2pointer(_hItem);
    uint32_t  flag  = TVGN_PARENT;

    if ( strcmp(method, "PARENT")               == 0 ) flag = TVGN_PARENT;
    else if ( strcmp(method, "CHILD")           == 0 ) flag = TVGN_CHILD;
    else if ( strcmp(method, "NEXT")            == 0 ) flag = TVGN_NEXT;
    else if ( strcmp(method, "NEXTVISIBLE")     == 0 ) flag = TVGN_NEXTVISIBLE;
    else if ( strcmp(method, "PREVIOUS")        == 0 ) flag = TVGN_PREVIOUS;
    else if ( strcmp(method, "PREVIOUSVISIBLE") == 0 ) flag = TVGN_PREVIOUSVISIBLE;

    return pointer2string(context, TreeView_GetNextItem(hwnd, hItem, flag));
}


/** TreeView::itemText()
 *
 *
 *
 *  @note  Although the tree-view allows any length of text to be stored for an
 *         item, it only displays the first 260 characters.  So, here that is
 *         all we return.
 */
RexxMethod2(RexxObjectPtr, tv_itemText, CSTRING, _hItem, CSELF, pCSelf)
{
    char buf[261];
    HWND  hwnd = getDChCtrl(pCSelf);

    TVITEMEX tvi   = {0};
    tvi.hItem      = (HTREEITEM)string2pointer(_hItem);
    tvi.mask       = TVIF_HANDLE | TVIF_TEXT;
    tvi.pszText    = buf;
    tvi.cchTextMax = 261;

    RexxObjectPtr result = context->NullString();
    if ( TreeView_GetItem(hwnd, &tvi) != 0 )
    {
        result = context->String(tvi.pszText);
    }

    return result;
}


/** TreeView::modify()
 *
 *
 */
RexxMethod8(int32_t, tv_modify, OPTIONAL_CSTRING, _hItem, OPTIONAL_CSTRING, label, OPTIONAL_int32_t, imageIndex,
            OPTIONAL_int32_t, selectedImage, OPTIONAL_CSTRING, opts, OPTIONAL_uint32_t, children,
            OPTIONAL_RexxObjectPtr, userData, CSELF, pCSelf)
{
    HWND hwnd  = getDChCtrl(pCSelf);

    TVITEMEX tvi = {0};

    if ( argumentExists(1) )
    {
        tvi.hItem = (HTREEITEM)string2pointer(_hItem);
    }
    else
    {
        tvi.hItem = TreeView_GetSelection(hwnd);
    }

    if ( tvi.hItem == NULL )
    {
        return -1;
    }
    tvi.mask = TVIF_HANDLE;

    if ( argumentExists(2) )
    {
        tvi.pszText = (LPSTR)label;
        tvi.cchTextMax = (int)strlen(label);
        tvi.mask |= TVIF_TEXT;
    }
    if ( argumentExists(3) && imageIndex > -1 )
    {
        tvi.iImage = imageIndex;
        tvi.mask |= TVIF_IMAGE;
    }
    if ( argumentExists(4) && selectedImage > -1 )
    {
        tvi.iSelectedImage = selectedImage;
        tvi.mask |= TVIF_SELECTEDIMAGE;
    }
    if ( argumentExists(5) && *opts != '\0' )
    {
        parseTvModifyOpts(opts, &tvi);
    }
    if ( argumentExists(6) )
    {
        tvi.cChildren = (children > 0 ? 1 : 0);
        tvi.mask |= TVIF_CHILDREN;
    }
    if ( argumentExists(7) && userData != TheNilObj)
    {
        RexxObjectPtr oldData = getCurrentTviUserData(hwnd, tvi.hItem);

        unProtectControlUserData(context, (pCDialogControl)pCSelf, oldData);
        protectControlUserData(context, (pCDialogControl)pCSelf, userData);

        tvi.lParam = (LPARAM)userData;
        tvi.mask |= TVIF_PARAM;
    }

    return (TreeView_SetItem(hwnd, &tvi) == 0 ? 1 : 0);
}


/** TreeView::dropHighLight()
 *  TreeView::makeFirstVisible()
 *  TreeView::select()
 */
RexxMethod3(RexxObjectPtr, tv_selectItem, OPTIONAL_CSTRING, _hItem, NAME, method, CSELF, pCSelf)
{
    HWND      hwnd  = getDChCtrl(pCSelf);
    HTREEITEM hItem = NULL;
    uint32_t  flag;

    if ( argumentExists(1) )
    {
        hItem = (HTREEITEM)string2pointer(_hItem);
    }

    switch ( *method )
    {
        case 'S' :
            flag = TVGN_CARET;
            break;
        case 'M' :
            flag = TVGN_FIRSTVISIBLE;
            break;
        default:
            flag = TVGN_DROPHILITE;
    }
    return (TreeView_Select(hwnd, hItem, flag) ? TheZeroObj : TheOneObj);
}


/** TreeView::expand()
 *  TreeView::collapse()
 *  TreeView::collapseAndReset()
 *  TreeView::toggle()
 */
RexxMethod3(RexxObjectPtr, tv_expand, CSTRING, _hItem, NAME, method, CSELF, pCSelf)
{
    HWND      hwnd  = getDChCtrl(pCSelf);
    HTREEITEM hItem = (HTREEITEM)string2pointer(_hItem);
    uint32_t  flag  = TVE_EXPAND;

    if ( *method == 'C' )
    {
        flag = (method[8] == 'A' ? (TVE_COLLAPSERESET | TVE_COLLAPSE) : TVE_COLLAPSE);
    }
    else if ( *method == 'T' )
    {
        flag = TVE_TOGGLE;
    }
    return (TreeView_Expand(hwnd, hItem, flag) ? TheZeroObj : TheOneObj);
}


/** TreeView::find()
 *
 *  Finds the first item with the specified text.
 *
 */
RexxMethod2(RexxObjectPtr, tv_find, CSTRING, text, CSELF, pCSelf)
{
    HWND hwnd  = getDChCtrl(pCSelf);

    HTREEITEM hItem = tvFindItem(hwnd, text);
    return pointer2string(context, hItem);
}


/** TreeView::hitTestInfo()
 *
 *  Determine the location of a point relative to the tree-view control.
 *
 *  @param  pHit  The position, x and y co-ordinates of the point to test.  This
 *                can be specified in two forms.
 *
 *      Form 1:  arg 1 is a .Point object.
 *      Form 2:  arg 1 is the x co-ordinate and arg2 is the y co-ordinate.
 *
 *  @return  A directory object containing the result of the test in these
 *           indexes:
 *
 *             hItem     The handle to the tree-view item that occupies the
 *                       point. This will be 0, if there is no item at the
 *                       point.
 *
 *             location  A string of blank separated keywords describing the
 *                       location of the point.  For instance, the string might
 *                       be "ONITEM ONLABEL" or it could be "ABOVE TORIGHT" if
 *                       the point is not on the client area of the tree-view at
 *                       all.
 *
 *  @note  Any x, y coordinates will work.  I.e. -6000, -7000 will work. The
 *         hItem will be 0 and location will be "ABOVE TOLEFT"
 */
RexxMethod2(RexxObjectPtr, tv_hitTestInfo, ARGLIST, args, CSELF, pCSelf)
{
    HWND hwnd = getDChCtrl(pCSelf);

    size_t sizeArray;
    size_t argsUsed;
    POINT  point;
    if ( ! getPointFromArglist(context, args, &point, 1, 2, &sizeArray, &argsUsed) )
    {
        return NULLOBJECT;
    }

    if ( argsUsed == 1 && sizeArray == 2)
    {
        return tooManyArgsException(context->threadContext, 1);
    }

    TVHITTESTINFO hti;
    hti.pt.x = point.x;
    hti.pt.y = point.y;

    HTREEITEM hItem = TreeView_HitTest(hwnd, &hti);

    RexxDirectoryObject result = context->NewDirectory();

    context->DirectoryPut(result, pointer2string(context, hti.hItem), "HITEM");

    char buf[128];
    *buf = '\0';

    if ( hti.flags & TVHT_ABOVE          ) strcat(buf, "ABOVE ");
    if ( hti.flags & TVHT_BELOW          ) strcat(buf, "BELOW ");
    if ( hti.flags & TVHT_NOWHERE        ) strcat(buf, "NOWHERE ");
    if ( hti.flags & TVHT_ONITEM         ) strcat(buf, "ONITEM ");
    if ( hti.flags & TVHT_ONITEMBUTTON   ) strcat(buf, "ONBUTTON ");
    if ( hti.flags & TVHT_ONITEMICON     ) strcat(buf, "ONICON ");
    if ( hti.flags & TVHT_ONITEMINDENT   ) strcat(buf, "ONINDENT ");
    if ( hti.flags & TVHT_ONITEMLABEL    ) strcat(buf, "ONLABEL ");
    if ( hti.flags & TVHT_ONITEMRIGHT    ) strcat(buf, "ONRIGHT ");
    if ( hti.flags & TVHT_ONITEMSTATEICON) strcat(buf, "ONSTATEICON ");
    if ( hti.flags & TVHT_TOLEFT         ) strcat(buf, "TOLEFT ");
    if ( hti.flags & TVHT_TORIGHT        ) strcat(buf, "TORIGHT ");

    if ( *buf != '\0' )
    {
        *(buf + strlen(buf) - 1) = '\0';
    }
    context->DirectoryPut(result, context->String(buf),"LOCATION");
    return result;
}


/** TreeView::removeItemData()
 *
 */
RexxMethod2(RexxObjectPtr, tv_removeItemData, CSTRING, _hItem, CSELF, pCSelf)
{
    pCDialogControl pcdc = validateDCCSelf(context, pCSelf);
    if ( pcdc == NULL )
    {
        return TheNilObj;
    }

    HTREEITEM hItem = (HTREEITEM)string2pointer(_hItem);

    RexxObjectPtr oldUserData = getCurrentTviUserData(pcdc->hCtrl, hItem);

    RexxObjectPtr result = getCurrentTviUserData(pcdc->hCtrl, hItem);
    if ( result != TheNilObj )
    {
        TVITEMEX tvi = {TVIF_PARAM, hItem};

        if ( TreeView_SetItem(pcdc->hCtrl, &tvi) )
        {
            unProtectControlUserData(context, pcdc, result);
        }
        else
        {
            // Not removed, set result back to the .nil ojbect.
            result = TheNilObj;
        }
    }

    return result;
}

/** TreeView::setImageList()
 *
 *  Sets or removes one of a tree-view's image lists.
 *
 *  @param ilSrc  The image list source. Either an .ImageList object that
 *                references the image list to be set, or a single bitmap from
 *                which the image list is constructed, or .nil.  If ilSRC is
 *                .nil, an existing image list, if any is removed.
 *
 *  @param width  [optional]  This arg serves two purposes.  If ilSrc is .nil or
 *                an .ImageList object, this arg indentifies which of the
 *                tree-views image lists is being set, normal, or state. The
 *                default is TVSI_NORMAL.
 *
 *                If ilSrc is a bitmap, then this arg is the width of a single
 *                image.  The default is the height of the actual bitmap.
 *
 *  @param height [optional]  This arg is only used if ilSrc is a bitmap, in which case it
 *                is the height of the bitmap.  The default is the height of the
 *                actual bitmap
 *
 *  @return       Returns the exsiting .ImageList object if there is one, or
 *                .nil if there is not an existing object.
 *
 *  @note  When the ilSrc is a single bitmap, an image list is created from the
 *         bitmap.  This method is not as flexible as if the programmer created
 *         the image list herself.  The bitmap must be a number of images, all
 *         the same size, side-by-side in the bitmap.  The width of a single
 *         image determines the number of images.  The image list is created
 *         using the ILC_COLOR8 flag, only.  No mask can be used.  No room is
 *         reserved for adding more images to the image list, etc..
 *
 *         The image list can only be assigned to the normal image list.  There
 *         is no way to use the image list for the state image list.
 */
RexxMethod4(RexxObjectPtr, tv_setImageList, RexxObjectPtr, ilSrc,
            OPTIONAL_int32_t, width, OPTIONAL_int32_t, height, CSELF, pCSelf)
{
    oodResetSysErrCode(context->threadContext);
    HWND hwnd = getDChCtrl(pCSelf);

    HIMAGELIST himl = NULL;
    int type = TVSIL_NORMAL;
    RexxObjectPtr imageList = NULLOBJECT;

    if ( ilSrc == TheNilObj )
    {
        imageList = ilSrc;
        if ( argumentExists(2) )
        {
            type = width;
        }
    }
    else if ( context->IsOfType(ilSrc, "IMAGELIST") )
    {
        imageList = ilSrc;
        himl = rxGetImageList(context, imageList, 1);
        if ( himl == NULL )
        {
            goto err_out;
        }
        if ( argumentExists(2) )
        {
            type = width;
        }
    }
    else
    {
        imageList = oodILFromBMP(context, &himl, ilSrc, width, height, hwnd);
        if ( imageList == NULLOBJECT )
        {
            goto err_out;
        }
    }

    if ( type != TVSIL_STATE && type != TVSIL_NORMAL )
    {
        invalidTypeException(context->threadContext, 2, "TVSIL_XXX flag");
        goto err_out;
    }

    TreeView_SetImageList(hwnd, himl, type);
    return rxSetObjVar(context, tvGetAttributeName(type), imageList);

err_out:
    return NULLOBJECT;
}


/** TreeView::setItemData()
 *
 *  Assigns a user data value to the specified tree-view item.
 *
 *  @param  hItem  [required]  The handle of the item whose user data is to be
 *                 set.
 *
 *  @param  data   [optional]  The user data to be set. If this argument is
 *                 omitted, the current user data, if any, is removed.
 *
 *  @return  Returns the previous user data object for the specified tree-view
 *           item, if there was a user data object, or .nil if there wasn't.
 *
 *           On error, .nil is returned.  An error is very unlikely.  An error
 *           can be checked for by examining the .systemErrorCode object.
 *
 *  @notes  Sets the .systemErrorCode.  On error set to:
 *
 *          156  ERROR_SIGNAL_REFUSED The recipient process has refused the
 *          signal.
 *
 *          This is not a system error, the code is just used here to indicate a
 *          tree-view error when setting the user data.  The tree-view provides
 *          no information on why it failed.
 */
RexxMethod3(RexxObjectPtr, tv_setItemData, CSTRING, _hItem, OPTIONAL_RexxObjectPtr, data, CSELF, pCSelf)
{
    oodResetSysErrCode(context->threadContext);

    pCDialogControl pcdc = validateDCCSelf(context, pCSelf);
    if ( pcdc == NULL )
    {
        return TheNilObj;
    }

    HTREEITEM hItem = (HTREEITEM)string2pointer(_hItem);

    RexxObjectPtr oldUserData = getCurrentTviUserData(pcdc->hCtrl, hItem);

    TVITEMEX tvi = {0};
    tvi.hItem  = hItem;
    tvi.mask   = TVIF_PARAM;

    if ( argumentExists(2) )
    {
        tvi.lParam = (LPARAM)data;

        if ( TreeView_SetItem(pcdc->hCtrl, &tvi) )
        {
            unProtectControlUserData(context, pcdc, oldUserData);
            protectControlUserData(context, pcdc, data);
        }
        else
        {
            oldUserData = TheNilObj;
            oodSetSysErrCode(context->threadContext, 156);
        }
    }
    else
    {
        if ( TreeView_SetItem(pcdc->hCtrl, &tvi) )
        {
            unProtectControlUserData(context, pcdc, oldUserData);
        }
        else
        {
            oldUserData = TheNilObj;
            oodSetSysErrCode(context->threadContext, 156);
        }

    }

    return oldUserData;
}


/** TreeView::setItemHeight()
 *
 *  Sets the height of the tree-view items.
 *
 *  @param   cyItem  [required]  The neew height for every item in the tree
 *                   view, in pixels.  Heights less than 1 will be set to 1. If
 *                   this argument is not even, it will be rounded down to the
 *                   nearest even value. If this argument is -1, the control
 *                   will revert to using its default item height.
 *
 *  @return  Returns the previous height of the tree-view items in pixels.
 *
 *  @remarks  The tree-view uses the value for the height of all items. The
 *            value itself is the height of a single item.  See the
 *            TreeView::getItemHeight() method.
 */
RexxMethod2(int32_t, tv_setItemHeight, int32_t, cyItem, CSELF, pCSelf)
{
    HWND hwnd  = getDChCtrl(pCSelf);
    return TreeView_SetItemHeight(hwnd, cyItem);
}


/** TreeView::sortChildrenCB()
 *
 *  Tells the tree-view to sort the children of the specified parent item using
 *  a call back into a compare function in the Rexx dialog.
 *
 *  @param hParent   The parent item whose children will be sorted.
 *
 *  @param method    The name of the method in the Rexx dialog that will be
 *                   invoked by the tree-view control.
 *
 */
RexxMethod4(logical_t, tv_sortChildrenCB, CSTRING, _hItem, CSTRING, method, OPTIONAL_RexxObjectPtr, param, CSELF, pCSelf)
{
    pCDialogControl pcdc = validateDCCSelf(context, pCSelf);
    if ( pcdc == NULL )
    {
        return FALSE;
    }

    HTREEITEM hItem = (HTREEITEM)string2pointer(_hItem);

    return rexxTreeViewSort(context, hItem, method, param, pcdc);
}


/**
 *  Methods for the .TvCustomDrawSimple class.
 */
#define TVCUSTOMDRAWSIMPLE_CLASS                "TvCustomDrawSimple"


/**
 * Handles the processing for the tree-view custom draw event for the basic
 * case. That is, the user wants to change text color or font, for a tree-view
 * item.
 *
 * @param c
 * @param methodName
 * @param lParam
 * @param pcpbd
 *
 * @return MsgReplyType
 *
 * @notes  In testing the list-view custom draw, we saw what seemed to be hangs
 *         if there was an uncleared condition on entry.   That seemed to be
 *         fixed by checking for a condition and immediately returning
 *         CDRF_DODEFAULT if a condition was detected.
 *
 *         The simple case is to only respond to item prepaint or subitem
 *         prepaint. If the user returns .false from the event handler, this has
 *         the effect of returning CDFR_DODEFAULT to the list-view.
 *
 *         If the user returns .true, we not check the reply value in the
 *         CTvCustomDrawSimple struct, assuming the user has set the value to
 *         what they actually want. The user should use either CDRF_NEWFONT, or,
 *         CDRF_NOTIFYSUBITEMDRAW or (CDRF_NOTIFYSUBITEMDRAW | CDRF_NEFONT.)
 *         When .true is returned the colors are updated in the NMLVCUSTOMDRAW
 *         struct. If hFont in the LvCustomDrawSimple object is not null, the
 *         font is selected into the device context, which has the effect of
 *         changing the font.
 */
MsgReplyType tvSimpleCustomDraw(RexxThreadContext *c, CSTRING methodName, LPARAM lParam, pCPlainBaseDialog pcpbd)
{
    LPNMTVCUSTOMDRAW tvcd  = (LPNMTVCUSTOMDRAW)lParam;
    LPARAM           reply = CDRF_DODEFAULT;

    if ( c->CheckCondition() )
    {
        goto done_out;
    }

    if ( tvcd->nmcd.dwDrawStage == CDDS_PREPAINT )
    {
        reply = CDRF_NOTIFYITEMDRAW;
    }
    else if ( tvcd->nmcd.dwDrawStage == CDDS_ITEMPREPAINT )
    {
        RexxBufferObject tvcdsBuf = c->NewBuffer(sizeof(CTvCustomDrawSimple));
        if ( tvcdsBuf == NULLOBJECT )
        {
            outOfMemoryException(c);
            endDialogPremature(pcpbd, pcpbd->hDlg, RexxConditionRaised);
            goto done_out;
        }

        pCTvCustomDrawSimple pctvcds = (pCTvCustomDrawSimple)c->BufferData(tvcdsBuf);
        memset(pctvcds, 0, sizeof(CTvCustomDrawSimple));

        pctvcds->drawStage = tvcd->nmcd.dwDrawStage;
        pctvcds->item      = (HTREEITEM)tvcd->nmcd.dwItemSpec;
        pctvcds->id        = (uint32_t)((NMHDR *)lParam)->idFrom;
        pctvcds->level     = tvcd->iLevel;
        pctvcds->userData  = tvcd->nmcd.lItemlParam ? (RexxObjectPtr)tvcd->nmcd.lItemlParam : TheNilObj;

        RexxObjectPtr custDrawSimple = c->SendMessage1(TheTvCustomDrawSimpleClass, "NEW", tvcdsBuf);
        if ( custDrawSimple != NULLOBJECT )
        {
            RexxObjectPtr msgReply = c->SendMessage1(pcpbd->rexxSelf, methodName, custDrawSimple);

            msgReply = requiredBooleanReply(c, pcpbd, msgReply, methodName, false);
            if ( msgReply == TheTrueObj )
            {
                tvcd->clrText   = pctvcds->clrText;
                tvcd->clrTextBk = pctvcds->clrTextBk;

                if ( pctvcds->hFont != NULL )
                {
                    // An example I've seen deletes the old font.  Doesn't seem
                    // appropriate for ooRexx.  The user would need to save the
                    // tree-view font and then add it back in.  Not sure if
                    // there is a resource leak here.
                    HFONT hOldFont = (HFONT)SelectObject(tvcd->nmcd.hdc, pctvcds->hFont);
                }
                reply = pctvcds->reply;
            }

            c->ReleaseLocalReference(custDrawSimple);
        }
        c->ReleaseLocalReference(tvcdsBuf);
    }

done_out:
    setWindowPtr(pcpbd->hDlg, DWLP_MSGRESULT, (LPARAM)reply);
    return ReplyTrue;
}


/** TvCustomDrawSimple::init()     [class]
 *
 *
 */
RexxMethod1(RexxObjectPtr, tvcds_init_cls, OSELF, self)
{
    if ( isOfClassType(context, self, TVCUSTOMDRAWSIMPLE_CLASS) )
    {
        TheTvCustomDrawSimpleClass = (RexxClassObject)self;
        context->RequestGlobalReference(TheTvCustomDrawSimpleClass);
    }
    return NULLOBJECT;
}

/** TvCustomDrawSimple::init()
 *
 */
RexxMethod2(RexxObjectPtr, tvcds_init, RexxObjectPtr, cself, OSELF, self)
{
    if ( context->IsBuffer(cself) )
    {
        context->SetObjectVariable("CSELF", cself);
    }
    else
    {
        baseClassInitializationException(context, "TvCustomDrawSimple");
    }
    return NULLOBJECT;
}

/** TvCustomDrawSimple::clrText    [attribute]
 */
RexxMethod2(uint32_t, tvcds_setClrText, uint32_t, clrText, CSELF, pCSelf)
{
    ((pCTvCustomDrawSimple)pCSelf)->clrText = clrText;
    return NULLOBJECT;
}

/** TvCustomDrawSimple::clrTextBk  [attribute]
 */
RexxMethod2(RexxObjectPtr, tvcds_setClrTextBk, uint32_t, clrTextBk, CSELF, pCSelf)
{
    ((pCTvCustomDrawSimple)pCSelf)->clrTextBk = clrTextBk;
    return NULLOBJECT;
}

/** TvCustomDrawSimple::drawStage  [attribute]
 */
RexxMethod1(uint32_t, tvcds_getDrawStage, CSELF, pCSelf)
{
    return ((pCTvCustomDrawSimple)pCSelf)->drawStage;
}

/** TvCustomDrawSimple::font       [attribute]
 */
RexxMethod2(RexxObjectPtr, tvcds_setFont, POINTERSTRING, font, CSELF, pCSelf)
{
    ((pCTvCustomDrawSimple)pCSelf)->hFont = (HFONT)font;
    return NULLOBJECT;
}

/** TvCustomDrawSimple::id         [attribute]
 */
RexxMethod1(uint32_t, tvcds_getID, CSELF, pCSelf)
{
    return ((pCTvCustomDrawSimple)pCSelf)->id;
}

/** TvCustomDrawSimple::item       [attribute]
 */
RexxMethod1(RexxStringObject, tvcds_getItem, CSELF, pCSelf)
{
    return pointer2string(context, ((pCTvCustomDrawSimple)pCSelf)->item);
}

/** TvCustomDrawSimple::reply      [attribute]
 */
RexxMethod2(RexxObjectPtr, tvcds_setReply, uint32_t, reply, CSELF, pCSelf)
{
    ((pCTvCustomDrawSimple)pCSelf)->reply = reply;
    return NULLOBJECT;
}

/** TvCustomDrawSimple::level      [attribute]
 */
RexxMethod1(uint32_t, tvcds_getLevel, CSELF, pCSelf)
{
    return ((pCTvCustomDrawSimple)pCSelf)->level + 1;
}

/** TvCustomDrawSimple::userData   [attribute]
 */
RexxMethod1(RexxObjectPtr, tvcds_getUserData, CSELF, pCSelf)
{
    RexxObjectPtr data = (RexxObjectPtr)((pCTvCustomDrawSimple)pCSelf)->userData;
    return data == NULLOBJECT ? TheNilObj : data;
}


