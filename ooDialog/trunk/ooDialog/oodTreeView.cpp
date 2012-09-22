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

RexxMethod8(RexxObjectPtr, tv_insert, OPTIONAL_CSTRING, _hItem, OPTIONAL_CSTRING, _hAfter, OPTIONAL_CSTRING, label,
            OPTIONAL_int32_t, imageIndex, OPTIONAL_int32_t, selectedImage, OPTIONAL_CSTRING, opts, OPTIONAL_uint32_t, children,
            CSELF, pCSelf)
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

    return pointer2string(context, TreeView_InsertItem(hwnd, &ins));
}

RexxMethod7(int32_t, tv_modify, OPTIONAL_CSTRING, _hItem, OPTIONAL_CSTRING, label, OPTIONAL_int32_t, imageIndex,
            OPTIONAL_int32_t, selectedImage, OPTIONAL_CSTRING, opts, OPTIONAL_uint32_t, children, CSELF, pCSelf)
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
    if ( argumentExists(4) && imageIndex > -1 )
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

    return (TreeView_SetItem(hwnd, &tvi) == 0 ? 1 : 0);
}


RexxMethod2(RexxObjectPtr, tv_itemInfo, CSTRING, _hItem, CSELF, pCSelf)
{
    HWND hwnd  = getDChCtrl(pCSelf);

    TVITEM tvi = {0};
    char buf[256];

    tvi.hItem = (HTREEITEM)string2pointer(_hItem);
    tvi.mask = TVIF_HANDLE | TVIF_TEXT | TVIF_STATE | TVIF_IMAGE | TVIF_CHILDREN | TVIF_SELECTEDIMAGE;
    tvi.pszText = buf;
    tvi.cchTextMax = 255;
    tvi.stateMask = TVIS_EXPANDED | TVIS_BOLD | TVIS_SELECTED | TVIS_EXPANDEDONCE | TVIS_DROPHILITED | TVIS_CUT;

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
    if ( *buf != '\0' )
    {
        *(buf + strlen(buf) - 1) = '\0';
    }
    context->SetStemElement(stem, "!STATE", context->String(buf));

    return stem;
}


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


/** TreeView::select()
 *  TreeView::makeFirstVisible()
 *  TreeView::dropHighLight()
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
                    // list-view font and then add it back in.  Not sure if
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
RexxMethod2(RexxObjectPtr, tvcds_getUserData, uint32_t, reply, CSELF, pCSelf)
{
    return ((pCTvCustomDrawSimple)pCSelf)->userData;
}


