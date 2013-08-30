/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 1995, 2004 IBM Corporation. All rights reserved.             */
/* Copyright (c) 2005-2013 Rexx Language Association. All rights reserved.    */
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
 * oodBarControls.cpp
 *
 * Contains methods for the ProgressBar, ReBar, ScrollBar, and TrackBar dialog
 * controls.  Also the misc control UpDown
 */
#include "ooDialog.hpp"     // Must be first, includes windows.h, commctrl.h, and oorexxapi.h

#include <stdio.h>
#include <dlgs.h>
#include <malloc.h>
#include <limits.h>
#include <shlwapi.h>
#include "APICommon.hpp"
#include "oodCommon.hpp"
#include "oodControl.hpp"
#include "oodResources.hpp"
#include "oodShared.hpp"


/**
 * Methods for the ProgressBar class.
 */
#define PROGRESSBAR_CLASS   "ProgressBar"

static RexxDirectoryObject pbGetFullRange(RexxMethodContext *c, HWND hPB)
{
    PBRANGE pbr;
    SendMessage(hPB, PBM_GETRANGE, TRUE, (LPARAM)&pbr);

    RexxDirectoryObject d = c->NewDirectory();
    c->DirectoryPut(d, c->Int32(pbr.iLow), "MIN");
    c->DirectoryPut(d, c->Int32(pbr.iHigh), "MAX");

    return d;
}


/** ProgressBar::setFullRange()
 *
 *  Sets the range for the progress bar using the full 32-bit numbers for the
 *  range.
 *
 *  The range can be specified using one argument, a directory object, or two
 *  arguments
 *
 *  Form 1:
 *
 *  @param   aDirectory  [OPTIONAL]  The directory indexes "min" and "max" are
 *                       used to set the range.  The min index specifies the low
 *                       end of the range and the max index specifies the high
 *                       end of the range.  If either, (or both,) indexes are
 *                       omitted, the low end of the range is set to 0 and the
 *                       high end is set to 100, as appropriate to the missing
 *                       index(es).
 *
 *  Form 2:
 *
 *  @param   min   [OPTIONAL]  The low end of the range.  0 is the default.
 *  @param   max   [OPTIONAL]  The high end of the range.  100 is the default.
 *
 *  @return  The previous range as a directory object with the indexes min and
 *           max.
 *
 *  @remarks The getRange() method is maintained for backwards compatibility. it
 *           returns the range as a string.  For that method, the returned range
 *           is not necessarily correct if the previous range has been set using
 *           the full 32-bit numbers now allowed by the progress bar control.
 *           The returned numbers are restricted to 0xFFFF.
 *
 *           Use the getFullRange() method to get the correct range.
 */
RexxMethod4(RexxObjectPtr, pbc_setFullRange, OPTIONAL_RexxObjectPtr, minObj, OPTIONAL_int32_t, max, NAME, method, CSELF, pCSelf)
{
    RexxMethodContext *c = context;
    HWND hwnd = getDChCtrl(pCSelf);
    int32_t min = 0;

    RexxDirectoryObject result = NULLOBJECT;
    bool usingDirectory = false;
    bool fullRange = method[3] == 'F';

    if ( argumentExists(1) )
    {
        if ( c->IsOfType(minObj, "DIRECTORY") )
        {
            usingDirectory = true;
            if ( ! rxIntFromDirectory(context, (RexxDirectoryObject)minObj, "MIN", &min, 1, false) )
            {
                goto done_out;
            }
            max = 100;
            if ( ! rxIntFromDirectory(context, (RexxDirectoryObject)minObj, "MAX", &max, 1, false) )
            {
                goto done_out;
            }
        }
        else
        {
            if ( ! context->Int32(minObj, &min) )
            {
                wrongRangeException(context->threadContext, 1,  INT32_MIN, INT32_MAX, minObj);
            }
        }
    }

    if ( ! usingDirectory && argumentOmitted(2) )
    {
        max = 100;
    }

    if ( fullRange )
    {
        result = pbGetFullRange(context, hwnd);
    }

    uint32_t range = (uint32_t)SendMessage(hwnd, PBM_SETRANGE32, min, max);

    if ( ! fullRange )
    {
        result = context->NewDirectory();
        context->DirectoryPut(result, context->Int32(LOWORD(range)), "MIN");
        context->DirectoryPut(result, context->Int32(HIWORD(range)), "MAX");
    }

done_out:
    return result;
}

RexxMethod1(RexxObjectPtr, pbc_getFullRange, CSELF, pCSelf)
{
    return pbGetFullRange(context, getDChCtrl(pCSelf));
}

/**
 *  ProgressBar::setMarquee()  Turn marquee mode on or off.
 *
 *  @param   on     [Optional]  Stop or start marquee mode.  Default is to
 *                  start.
 *
 *  @param   pause  [Optional]  Time in milliseconds between updates.  Default
 *                  is 1000 (1 second.)
 *
 *  @return  True (always.)
 *
 *  Requires XP Common Controls version 6.0 or greater.
 */
RexxMethod3(logical_t, pbc_setMarquee, OPTIONAL_logical_t, on, OPTIONAL_uint32_t, pause, CSELF, pCSelf)
{
    if ( ! requiredComCtl32Version(context, "setMarquee", COMCTL32_6_0) )
    {
        return 0;
    }

    HWND hwnd = getDChCtrl(pCSelf);

    if ( ! hasStyle(hwnd, PBS_MARQUEE) )
    {
        wrongWindowStyleException(context, "progress bar", "PBS_MARQUEE");
        return 0;
    }

    if ( argumentOmitted(1) )
    {
        on = 1;
    }
    if ( argumentOmitted(2) )
    {
        pause = 1000;
    }

    /* The Windows message always returns 1, return 1 for .true (succeeded.) */
    SendMessage(hwnd, PBM_SETMARQUEE, on, pause);
    return 1;
}


/**
 * Methods for the ReBar class.
 */
#define REBAR_CLASS   "ReBar"


#define REBARBANDINFO_OBJ_MAGIC             "Mv1vc2bn@l"
#define REBARBAND_TEXT_MAX                  260

/**
 *  Methods for the .ReBarBandInfo class.
 */
#define REBARBANDINFO_CLASS           "ReBarBandInfo"


inline bool isRbbiInternalInit(RexxMethodContext *context, RexxObjectPtr child, CSTRING text)
{
    return argumentExists(1) && context->IsBuffer(child) && argumentExists(2) && strcmp(text, REBARBANDINFO_OBJ_MAGIC) == 0;
}

RexxObjectPtr getRbbiText(RexxMethodContext *c, LPREBARBANDINFO prbbi)
{
    if ( prbbi->lpText == NULL )
    {
        return TheNilObj;
    }

    return c->String(prbbi->lpText);
}

/**
 * Sets the text attribute for the ReBarBandInfo object.
 *
 * If the ReBarBandInfo is used to receive information, the lpText member has to
 * point to a buffer to recieve the text.  When setting text, we probably do not
 * need to allocate a buffer, we could just assign the pointer.  However, to
 * keep things simpler, we just always allocate a buffer.
 *
 * Also, we set an arbitrary length of the text to 260 characters.  This is
 * similar to the maximum length of a string that will be displayed in other
 * controls.
 *
 * @param c
 * @param pLVI
 * @param text
 *
 * @return bool
 */
bool setRbbiText(RexxMethodContext *c, LPREBARBANDINFO prbbi, CSTRING text, size_t argPos)
{
    if ( text != NULL )
    {
        size_t len = strlen(text);
        if ( len > REBARBAND_TEXT_MAX )
        {
            stringTooLongException(c->threadContext, argPos, REBARBAND_TEXT_MAX, len);
            return false;
        }
    }

    if ( prbbi->lpText == NULL )
    {
        prbbi->lpText = (char *)LocalAlloc(LPTR, REBARBAND_TEXT_MAX + 1);
        if ( prbbi->lpText == NULL )
        {
            outOfMemoryException(c->threadContext);
            return false;
        }
        prbbi->cch = REBARBAND_TEXT_MAX + 1;
    }

    prbbi->fMask |= RBBIM_TEXT;

    if ( text != NULL )
    {
        strcpy(prbbi->lpText, text);
    }
    return true;
}

/**
 * Converts a string of keywords to the proper RBBIM_* mask flag.
 *
 * @param flags
 *
 * @return uint32_t
 */
uint32_t keyword2rbbim(CSTRING flags)
{
    uint32_t val = 0;

    if ( StrStrI(flags, "BACKGROUND")      != NULL ) val |= RBBIM_BACKGROUND;
    if ( StrStrI(flags, "CHEVRONLOCATION") != NULL ) val |= RBBIM_CHEVRONLOCATION;
    if ( StrStrI(flags, "CHEVRONSTATE")    != NULL ) val |= RBBIM_CHEVRONSTATE;
    if ( StrStrI(flags, "COLORS")          != NULL ) val |= RBBIM_COLORS;
    if ( StrStrI(flags, "HEADERSIZE")      != NULL ) val |= RBBIM_HEADERSIZE;
    if ( StrStrI(flags, "ID")              != NULL ) val |= RBBIM_ID;
    if ( StrStrI(flags, "IDEALSIZE")       != NULL ) val |= RBBIM_IDEALSIZE;
    if ( StrStrI(flags, "IMAGE")           != NULL ) val |= RBBIM_IMAGE;
    if ( StrStrI(flags, "LPARAM")          != NULL ) val |= RBBIM_LPARAM;
    if ( StrStrI(flags, "SIZE")            != NULL ) val |= RBBIM_SIZE;
    if ( StrStrI(flags, "STYLE")           != NULL ) val |= RBBIM_STYLE;
    if ( StrStrI(flags, "TEXT")            != NULL ) val |= RBBIM_TEXT;

    if (      StrStrI(flags, "CHILDSIZE")!= NULL ) val |= RBBIM_CHILDSIZE;
    else if ( StrStrI(flags, "CHILD")    != NULL ) val |= RBBIM_CHILD;
    return val;
}


/**
 * Converts a set of RBBIM_* mask flags to their keyword string.
 *
 * @param c
 * @param flags
 *
 * @return A Rexx string object.
 */
 RexxStringObject rbbim2keyword(RexxMethodContext *c, uint32_t flags)
{
    char buf[256];
    *buf = '\0';

    if ( flags & RBBIM_BACKGROUND     ) strcat(buf, "BACKGROUND ");
    if ( flags & RBBIM_CHEVRONLOCATION) strcat(buf, "CHEVRONLOCATION ");
    if ( flags & RBBIM_CHEVRONSTATE   ) strcat(buf, "CHEVRONSTATE ");
    if ( flags & RBBIM_CHILD          ) strcat(buf, "CHILD ");
    if ( flags & RBBIM_CHILDSIZE      ) strcat(buf, "CHILDSIZE ");
    if ( flags & RBBIM_COLORS         ) strcat(buf, "COLORS ");
    if ( flags & RBBIM_HEADERSIZE     ) strcat(buf, "HEADERSIZE ");
    if ( flags & RBBIM_ID             ) strcat(buf, "ID ");
    if ( flags & RBBIM_IDEALSIZE      ) strcat(buf, "IDEALSIZE ");
    if ( flags & RBBIM_IMAGE          ) strcat(buf, "IMAGE ");
    if ( flags & RBBIM_LPARAM         ) strcat(buf, "LPARAM ");
    if ( flags & RBBIM_SIZE           ) strcat(buf, "SIZE ");
    if ( flags & RBBIM_STYLE          ) strcat(buf, "STYLE ");
    if ( flags & RBBIM_TEXT           ) strcat(buf, "TEXT ");

    if ( *buf != '\0' )
    {
        *(buf + strlen(buf) - 1) = '\0';
    }
    return c->String(buf);
}

/**
 * Converts a string of keywords to the proper RBB_* style flag.
 *
 * @param flags
 *
 * @return uint32_t
 */
uint32_t keyword2rbbs(CSTRING flags)
{
    uint32_t val = 0;

    if ( StrStrI(flags, "BREAK")          != NULL ) val |= RBBS_BREAK         ;
    if ( StrStrI(flags, "CHILDEDGE")      != NULL ) val |= RBBS_CHILDEDGE     ;
    if ( StrStrI(flags, "FIXEDBMP")       != NULL ) val |= RBBS_FIXEDBMP      ;
    if ( StrStrI(flags, "FIXEDSIZE")      != NULL ) val |= RBBS_FIXEDSIZE     ;
    if ( StrStrI(flags, "GRIPPERALWAYS")  != NULL ) val |= RBBS_GRIPPERALWAYS ;
    if ( StrStrI(flags, "HIDDEN")         != NULL ) val |= RBBS_HIDDEN        ;
    if ( StrStrI(flags, "HIDETITLE")      != NULL ) val |= RBBS_HIDETITLE     ;
    if ( StrStrI(flags, "NOGRIPPER")      != NULL ) val |= RBBS_NOGRIPPER     ;
    if ( StrStrI(flags, "NOVERT")         != NULL ) val |= RBBS_NOVERT        ;
    if ( StrStrI(flags, "TOPALIGN")       != NULL ) val |= RBBS_TOPALIGN      ;
    if ( StrStrI(flags, "USECHEVRON")     != NULL ) val |= RBBS_USECHEVRON    ;
    if ( StrStrI(flags, "VARIABLEHEIGHT") != NULL ) val |= RBBS_VARIABLEHEIGHT;

    return val;
}


/**
 * Converts a set of RBB_* style flags to their keyword string.
 *
 * @param c
 * @param flags
 *
 * @return A Rexx string object.
 */
 RexxStringObject rbbs2keyword(RexxMethodContext *c, uint32_t flags)
{
    char buf[256];
    *buf = '\0';

    if ( flags & RBBS_BREAK           ) strcat(buf, "BREAK ");
    if ( flags & RBBS_CHILDEDGE       ) strcat(buf, "CHILDEDGE ");
    if ( flags & RBBS_FIXEDBMP        ) strcat(buf, "FIXEDBMP ");
    if ( flags & RBBS_FIXEDSIZE       ) strcat(buf, "FIXEDSIZE ");
    if ( flags & RBBS_GRIPPERALWAYS   ) strcat(buf, "GRIPPERALWAYS ");
    if ( flags & RBBS_HIDDEN          ) strcat(buf, "HIDDEN ");
    if ( flags & RBBS_HIDETITLE       ) strcat(buf, "HIDETITLE ");
    if ( flags & RBBS_NOGRIPPER       ) strcat(buf, "NOGRIPPER ");
    if ( flags & RBBS_NOVERT          ) strcat(buf, "NOVERT ");
    if ( flags & RBBS_TOPALIGN        ) strcat(buf, "TOPALIGN ");
    if ( flags & RBBS_USECHEVRON      ) strcat(buf, "USECHEVRON ");
    if ( flags & RBBS_VARIABLEHEIGHT  ) strcat(buf, "VARIABLEHEIGHT ");

    if ( *buf != '\0' )
    {
        *(buf + strlen(buf) - 1) = '\0';
    }
    return c->String(buf);
}


/** ReBarBandInfo::init()      [Class]
 *
 */
RexxMethod1(RexxObjectPtr, rbbi_init_cls, OSELF, self)
{
    if ( isOfClassType(context, self, REBARBANDINFO_CLASS) )
    {
        TheReBarBandInfoClass = (RexxClassObject)self;
        context->RequestGlobalReference(TheReBarBandInfoClass);
    }
    return NULLOBJECT;
}


/** ReBarBandInfo::uninit()
 *
 */
RexxMethod1(RexxObjectPtr, rbbi_unInit, CSELF, pCSelf)
{
#if 0
    printf("In rbbi_unInit() pCSelf=%p\n", pCSelf);
#endif

    if ( pCSelf != NULLOBJECT )
    {
        LPREBARBANDINFO prbbi = (LPREBARBANDINFO)pCSelf;

#if 0
    printf("In rbbi_unInit() pszText=%p\n", prbbi->lpText);
#endif
        safeLocalFree(prbbi->lpText);
    }
    return NULLOBJECT;
}

/** ReBarBandInfo::init()
 *
 *  Just temp for now.  We are not going to allow passing in every attribute,
 *  but we still need to figure out which attributes to accept / are the most
 *  useful.
 */
RexxMethod6(RexxObjectPtr, rbbi_init, OPTIONAL_RexxObjectPtr, _child, OPTIONAL_CSTRING, text,
             OPTIONAL_CSTRING, style, OPTIONAL_CSTRING, mask, OPTIONAL_int32_t, imageIndex,
             OPTIONAL_RexxObjectPtr, itemData)
{
    if ( isRbbiInternalInit(context, _child, text) )
    {
        context->SetObjectVariable("CSELF", _child);
        return NULLOBJECT;
    }

    RexxBufferObject obj = context->NewBuffer(sizeof(REBARBANDINFO));
    context->SetObjectVariable("CSELF", obj);

    LPREBARBANDINFO prbbi = (LPREBARBANDINFO)context->BufferData(obj);
    memset(prbbi, 0, sizeof(REBARBANDINFO));
    prbbi->cbSize = sizeof(REBARBANDINFO);

#if 0
    // We check if the user is setting the mask, and what values she sets,
    // first.  This allows use to allocate the needed buffers if the LvItem
    // object is going to be used to retrieve information.
    if ( argumentExists(5) )
    {
        lvi->mask = keyword2lvif(mask);
    }

    if ( argumentExists(1) )
    {
        int32_t index;
        if ( ! context->Int32(_index, &index) || index < 0 )
        {
            wrongRangeException(context->threadContext, 1, 0, INT_MAX, _index);
            return NULLOBJECT;
        }

        lvi->iItem = index;
    }
    else
    {
        lvi->iItem = 0;
    }

    if ( argumentExists(2) )
    {
        if ( ! setLviText(context, lvi, text, 2) )
        {
            return NULLOBJECT;
        }
    }
    else
    {
        // Check if the user has set the LVIF_TEXT flag.
        if ( lvi->mask & LVIF_TEXT )
        {
            // The empty string tells setLviText() to allocate a buffer.
            if ( ! setLviText(context, lvi, "", 2) )
            {
                return NULLOBJECT;
            }
        }
    }

    lvi->mask |= LVIF_IMAGE;
    if ( argumentExists(3) )
    {
        lvi->iImage = imageIndex < I_IMAGENONE ? I_IMAGENONE : imageIndex;
    }
    else
    {
        lvi->iImage = I_IMAGENONE;
    }

    if ( argumentExists(4) )
    {
        setLviUserData(context, lvi, itemData);
    }

    if ( argumentExists(6) )
    {
        lvi->state = keyword2lvis(itemState, false);
        lvi->mask |= LVIF_STATE;
    }

    if ( argumentExists(7) )
    {
        // The stateMask uses the exact same flags as the item state, and the
        // mask member does not need to be set for this.
        lvi->stateMask = keyword2lvis(itemStateMask, true);
    }

    if ( argumentExists(8) )
    {
        lvi->iIndent = indent;
        lvi->mask |= LVIF_INDENT;
    }

    if ( argumentExists(9) )
    {
        setLviGroupID(context, lvi, groupID);
    }
    else
    {
        if ( ComCtl32Version >= COMCTL32_6_0 )
        {
            lvi->iGroupId = I_GROUPIDNONE;
            lvi->mask |= LVIF_GROUPID;
        }
    }

    if ( argumentExists(10) )
    {
        setLviColumns(context, lvi, columns, 10);
    }
    else
    {
        // Check if the user has set the LVIF_COLUMNS flag.
        if ( lvi->mask & LVIF_COLUMNS )
        {
            if ( ! allocLviColumns(context, lvi) )
            {
                return NULLOBJECT;
            }
        }
    }

    // We want the user to be able to create the LvItem object to recieve data.
    // So we check if the user has set the LVIF_COLFMT flag.
    if ( lvi->mask & LVIF_COLFMT )
    {
        if ( ! allocLviColFmt(context, lvi) )
        {
            return NULLOBJECT;
        }
    }
#endif

    return NULLOBJECT;
}

/** ReBarBandInfo::bitmapBack                [attribute]
 */
RexxMethod1(RexxObjectPtr, rbbi_bitmapBack, CSELF, cSelf)
{
    LPREBARBANDINFO prbbi = (LPREBARBANDINFO)cSelf;
    if ( prbbi->hbmBack != NULL )
    {
        SIZE s = {0};
        return rxNewValidImage(context, prbbi->hbmBack, IMAGE_BITMAP, &s, LR_DEFAULTCOLOR, false);
    }
    return TheNilObj;
}
RexxMethod2(RexxObjectPtr, rbbi_setBitmapBack, RexxObjectPtr, _bitmap, CSELF, cSelf)
{
    LPREBARBANDINFO prbbi = (LPREBARBANDINFO)cSelf;

    if ( ! context->IsOfType(_bitmap, "IMAGE") )
    {
        wrongClassException(context->threadContext, 1, "Image", _bitmap);
    }
    else
    {
        POODIMAGE oodImage = rxGetImageBitmap(context, _bitmap, 1);
        if ( oodImage != NULL )
        {
            prbbi->hbmBack = (HBITMAP)oodImage->hImage;
            prbbi->fMask |= RBBIM_BACKGROUND;
        }
    }
    return NULLOBJECT;
}

/** ReBarBandInfo::chevronLocation          [attribute]
 */
RexxMethod1(RexxObjectPtr, rbbi_chevronLocation, CSELF, cSelf)
{
    LPREBARBANDINFO prbbi = (LPREBARBANDINFO)cSelf;

    if ( prbbi->rcChevronLocation.left < 1 && prbbi->rcChevronLocation.top < 1 &&
         prbbi->rcChevronLocation.right < 1 && prbbi->rcChevronLocation.bottom < 1 )
    {
        return TheNilObj;
    }
    return rxNewRect(context, prbbi->rcChevronLocation.left, prbbi->rcChevronLocation.top,
                     prbbi->rcChevronLocation.right, prbbi->rcChevronLocation.bottom);
}
RexxMethod2(RexxObjectPtr, rbbi_setChevronLocation, RexxObjectPtr, _rect, CSELF, cSelf)
{
    LPREBARBANDINFO prbbi = (LPREBARBANDINFO)cSelf;

    RECT *r = rxGetRect(context, _rect, 1);
    if ( r != NULL )
    {
        prbbi->rcChevronLocation.left   = r->left;
        prbbi->rcChevronLocation.top    = r->top;
        prbbi->rcChevronLocation.right  = r->right;
        prbbi->rcChevronLocation.bottom = r->bottom;
        prbbi->fMask |= RBBIM_CHEVRONLOCATION;
    }
    return NULLOBJECT;
}

/** ReBarBandInfo::chevronState                [attribute]
 */
RexxMethod1(RexxStringObject, rbbi_chevronState, CSELF, cSelf)
{
    LPREBARBANDINFO prbbi = (LPREBARBANDINFO)cSelf;
    return stateSystem2keyword(context, prbbi->uChevronState);
}
RexxMethod2(RexxObjectPtr, rbbi_setChevronState, CSTRING, state, CSELF, cSelf)
{
    LPREBARBANDINFO prbbi = (LPREBARBANDINFO)cSelf;
    prbbi->uChevronState = keyword2stateSystem(state);
    prbbi->fMask |= RBBIM_CHEVRONSTATE;
    return NULLOBJECT;
}

/** ReBarBandInfo::child             [attribute]
 *
 *  We return .nil if we fail to get the Rexx child control.  But, really that
 *  shouldn't happen if hChild is not NULL.
 */
RexxMethod1(RexxObjectPtr, rbbi_child, CSELF, cSelf)
{
    LPREBARBANDINFO prbbi = (LPREBARBANDINFO)cSelf;

    HWND hChild = prbbi->hwndChild;
    if ( hChild != NULL )
    {
        RexxObjectPtr rxControl = (RexxObjectPtr)getWindowPtr(hChild, GWLP_USERDATA);
        if ( rxControl != NULLOBJECT )
        {
            pCDialogControl pcdc = controlToCSelf(context->threadContext, rxControl);
            if ( pcdc != NULL )
            {
                return pcdc->rexxSelf;
            }
        }
    }
    return TheNilObj;
}
RexxMethod2(RexxObjectPtr, rbbi_setChild, RexxObjectPtr, _child, CSELF, cSelf)
{
    LPREBARBANDINFO prbbi = (LPREBARBANDINFO)cSelf;

    pCDialogControl pcdc = requiredDlgControlCSelf(context, _child, 1);
    if ( pcdc != NULLOBJECT )
    {
        prbbi->hwndChild = pcdc->hCtrl;
        prbbi->fMask |= RBBIM_CHILD;
    }
    return NULLOBJECT;
}

/** ReBarBandInfo::clrBack                 [attribute]
 */
RexxMethod1(uint32_t, rbbi_clrBack, CSELF, prbbi)
{
    return ((LPREBARBANDINFO)prbbi)->clrBack;
}
RexxMethod2(RexxObjectPtr, rbbi_setClrBack, uint32_t, bg, CSELF, prbbi)
{
    ((LPREBARBANDINFO)prbbi)->clrBack = bg;
    ((LPREBARBANDINFO)prbbi)->fMask   |= RBBIM_COLORS;
    return NULLOBJECT;
}

/** ReBarBandInfo::clrFore                 [attribute]
 */
RexxMethod1(uint32_t, rbbi_clrFore, CSELF, prbbi)
{
    return ((LPREBARBANDINFO)prbbi)->clrFore;
}
RexxMethod2(RexxObjectPtr, rbbi_setClrFore, uint32_t, fg, CSELF, prbbi)
{
    ((LPREBARBANDINFO)prbbi)->clrFore = fg;
    ((LPREBARBANDINFO)prbbi)->fMask   |= RBBIM_COLORS;
    return NULLOBJECT;
}

/** ReBarBandInfo::cx                  [attribute]
 */
RexxMethod1(uint32_t, rbbi_cx, CSELF, prbbi)
{
    return ((LPREBARBANDINFO)prbbi)->cx;
}
RexxMethod2(RexxObjectPtr, rbbi_setCx, uint32_t, cx, CSELF, prbbi)
{
    ((LPREBARBANDINFO)prbbi)->cx    = cx;
    ((LPREBARBANDINFO)prbbi)->fMask |= RBBIM_SIZE;
    return NULLOBJECT;
}

/** ReBarBandInfo::cxHeader              [attribute]
 */
RexxMethod1(uint32_t, rbbi_cxHeader, CSELF, prbbi)
{
    return ((LPREBARBANDINFO)prbbi)->cxHeader;
}
RexxMethod2(RexxObjectPtr, rbbi_setCxHeader, uint32_t, cx, CSELF, prbbi)
{
    ((LPREBARBANDINFO)prbbi)->cxHeader = cx;
    ((LPREBARBANDINFO)prbbi)->fMask    |= RBBIM_HEADERSIZE;
    return NULLOBJECT;
}

/** ReBarBandInfo::cxIdeal              [attribute]
 */
RexxMethod1(uint32_t, rbbi_cxIdeal, CSELF, prbbi)
{
    return ((LPREBARBANDINFO)prbbi)->cxIdeal;
}
RexxMethod2(RexxObjectPtr, rbbi_setCxIdeal, uint32_t, cx, CSELF, prbbi)
{
    ((LPREBARBANDINFO)prbbi)->cxIdeal = cx;
    ((LPREBARBANDINFO)prbbi)->fMask  |= RBBIM_IDEALSIZE;
    return NULLOBJECT;
}

/** ReBarBandInfo::cxMinChild              [attribute]
 */
RexxMethod1(uint32_t, rbbi_cxMinChild, CSELF, prbbi)
{
    return ((LPREBARBANDINFO)prbbi)->cxMinChild;
}
RexxMethod2(RexxObjectPtr, rbbi_setCxMinChild, uint32_t, cx, CSELF, prbbi)
{
    ((LPREBARBANDINFO)prbbi)->cxMinChild  = cx;
    ((LPREBARBANDINFO)prbbi)->fMask      |= RBBIM_CHILDSIZE;
    return NULLOBJECT;
}

/** ReBarBandInfo::cyChild              [attribute]
 */
RexxMethod1(uint32_t, rbbi_cyChild, CSELF, prbbi)
{
    return ((LPREBARBANDINFO)prbbi)->cyChild;
}
RexxMethod2(RexxObjectPtr, rbbi_setCyChild, uint32_t, cy, CSELF, prbbi)
{
    ((LPREBARBANDINFO)prbbi)->cyChild = cy;
    ((LPREBARBANDINFO)prbbi)->fMask  |= RBBIM_CHILDSIZE;
    return NULLOBJECT;
}

/** ReBarBandInfo::cyIntegral              [attribute]
 */
RexxMethod1(uint32_t, rbbi_cyIntegral, CSELF, prbbi)
{
    return ((LPREBARBANDINFO)prbbi)->cyIntegral;
}
RexxMethod2(RexxObjectPtr, rbbi_setCyIntegral, uint32_t, cy, CSELF, prbbi)
{
    ((LPREBARBANDINFO)prbbi)->cyIntegral = cy;
    ((LPREBARBANDINFO)prbbi)->fMask     |= RBBIM_CHILDSIZE;
    return NULLOBJECT;
}

/** ReBarBandInfo::cyMaxChild              [attribute]
 */
RexxMethod1(uint32_t, rbbi_cyMaxChild, CSELF, prbbi)
{
    return ((LPREBARBANDINFO)prbbi)->cyMaxChild;
}
RexxMethod2(RexxObjectPtr, rbbi_setCyMaxChild, uint32_t, cy, CSELF, prbbi)
{
    ((LPREBARBANDINFO)prbbi)->cyMaxChild = cy;
    ((LPREBARBANDINFO)prbbi)->fMask     |= RBBIM_CHILDSIZE;
    return NULLOBJECT;
}

/** ReBarBandInfo::cyMinChild              [attribute]
 */
RexxMethod1(uint32_t, rbbi_cyMinChild, CSELF, prbbi)
{
    return ((LPREBARBANDINFO)prbbi)->cyMinChild;
}
RexxMethod2(RexxObjectPtr, rbbi_setCyMinChild, uint32_t, cy, CSELF, prbbi)
{
    ((LPREBARBANDINFO)prbbi)->cyMinChild = cy;
    ((LPREBARBANDINFO)prbbi)->fMask      |= RBBIM_CHILDSIZE;
    return NULLOBJECT;
}

/** ReBarBandInfo::id          [attribute]
 */
RexxMethod1(uint32_t, rbbi_id, CSELF, prbbi)
{
    return ((LPREBARBANDINFO)prbbi)->wID;
}
RexxMethod2(RexxObjectPtr, rbbi_setId, uint32_t, id, CSELF, prbbi)
{
    ((LPREBARBANDINFO)prbbi)->wID    = id;
    ((LPREBARBANDINFO)prbbi)->fMask |= RBBIM_ID;
    return NULLOBJECT;
}

/** ReBarBandInfo::imageIndex             [attribute]
 */
RexxMethod1(int32_t, rbbi_imageIndex, CSELF, prbbi)
{
    return ((LPREBARBANDINFO)prbbi)->iImage;
}
RexxMethod2(RexxObjectPtr, rbbi_setImageIndex, int32_t, imageIndex, CSELF, prbbi)
{
    ((LPREBARBANDINFO)prbbi)->iImage  = imageIndex < 0 ? I_IMAGENONE : imageIndex;
    ((LPREBARBANDINFO)prbbi)->fMask  |= RBBIM_IMAGE;
    return NULLOBJECT;
}

/** ReBarBandInfo::itemData               [attribute]
 */
RexxMethod1(RexxObjectPtr, rbbi_itemData, CSELF, prbbi)
{
    return (RexxObjectPtr)((LPREBARBANDINFO)prbbi)->lParam;
}
RexxMethod2(RexxObjectPtr, rbbi_setItemData, RexxObjectPtr, userData, CSELF, prbbi)
{
    ((LPREBARBANDINFO)prbbi)->lParam = (LPARAM)userData;
    ((LPREBARBANDINFO)prbbi)->fMask |= RBBIM_LPARAM;
    return NULLOBJECT;
}

/** ReBarBandInfo::mask                   [attribute]
 */
RexxMethod1(RexxStringObject, rbbi_mask, CSELF, prbbi)
{
    return rbbim2keyword(context, ((LPREBARBANDINFO)prbbi)->fMask);
}
RexxMethod2(RexxObjectPtr, rbbi_setMask, CSTRING, mask, CSELF, prbbi)
{
    ((LPREBARBANDINFO)prbbi)->fMask = keyword2rbbim(mask);
    return NULLOBJECT;
}

/** ReBarBandInfo::style                   [attribute]
 */
RexxMethod1(RexxStringObject, rbbi_style, CSELF, prbbi)
{
    return rbbs2keyword(context, ((LPREBARBANDINFO)prbbi)->fStyle);
}
RexxMethod2(RexxObjectPtr, rbbi_setStyle, CSTRING, style, CSELF, prbbi)
{
    ((LPREBARBANDINFO)prbbi)->fStyle = keyword2rbbs(style);
    ((LPREBARBANDINFO)prbbi)->fMask |= RBBIM_STYLE;
    return NULLOBJECT;
}

/** ReBarBandInfo::text                   [attribute]
 */
RexxMethod1(RexxObjectPtr, rbbi_text, CSELF, prbbi)
{
    return getRbbiText(context, (LPREBARBANDINFO)prbbi);
}
RexxMethod2(RexxObjectPtr, rbbi_setText, CSTRING, text, CSELF, prbbi)
{
    setRbbiText(context, (LPREBARBANDINFO)prbbi, text, 1);
    return NULLOBJECT;
}



/**
 * Methods for the ScrollBar class.
 */
#define SCROLLBAR_CLASS   "ScrollBar"


RexxMethod4(logical_t, sb_setRange, int32_t, min, int32_t, max, OPTIONAL_logical_t, redraw, CSELF, pCSelf)
{
    oodResetSysErrCode(context->threadContext);
    redraw = (argumentOmitted(3) ? TRUE : redraw);

    SCROLLINFO si = {0};
    si.cbSize = sizeof(SCROLLINFO);
    si.fMask = SIF_RANGE;
    si.nMax = max;
    si.nMin = min;

    if ( SetScrollInfo(getDChCtrl(pCSelf), SB_CTL, &si, (BOOL)redraw) == 0 )
    {
        oodSetSysErrCode(context->threadContext);
        return 1;
    }
    return 0;
}


RexxMethod1(RexxObjectPtr, sb_getRange, CSELF, pCSelf)
{
    oodResetSysErrCode(context->threadContext);

    RexxDirectoryObject result = context->NewDirectory();
    context->DirectoryPut(result, TheNegativeOneObj, "MIN");
    context->DirectoryPut(result, TheNegativeOneObj, "MAX");

    SCROLLINFO si = {0};
    si.cbSize = sizeof(SCROLLINFO);
    si.fMask = SIF_RANGE;

    if ( GetScrollInfo(getDChCtrl(pCSelf), SB_CTL, &si) == 0 )
    {
        oodSetSysErrCode(context->threadContext);
    }
    else
    {
        context->DirectoryPut(result, context->Int32(si.nMin), "MIN");
        context->DirectoryPut(result, context->Int32(si.nMax), "MAX");
    }
    return result;
}

RexxMethod3(logical_t, sb_setPosition, int32_t, pos, OPTIONAL_logical_t, redraw, CSELF, pCSelf)
{
    oodResetSysErrCode(context->threadContext);
    redraw = (argumentOmitted(2) ? TRUE : redraw);

    SCROLLINFO si = {0};
    si.cbSize = sizeof(SCROLLINFO);
    si.fMask = SIF_POS;
    si.nPos = pos;

    if ( SetScrollInfo(getDChCtrl(pCSelf), SB_CTL, &si, (BOOL)redraw) == 0 )
    {
        oodSetSysErrCode(context->threadContext);
        return 0;
    }
    return 1;
}

RexxMethod1(int32_t, sb_getPosition, CSELF, pCSelf)
{
    oodResetSysErrCode(context->threadContext);

    SCROLLINFO si = {0};
    si.cbSize = sizeof(SCROLLINFO);
    si.fMask = SIF_POS;

    if ( GetScrollInfo(getDChCtrl(pCSelf), SB_CTL, &si) == 0 )
    {
        oodSetSysErrCode(context->threadContext);
    }
    return si.nPos;
}


/**
 * Methods for the TrackBar class.  Note that this is the .Slider class in
 * ooDialog.  yghr.
 */
#define TRACK_BAR_CLASS  "TrackBar"
#define SLIDER_CLASS     "Slider"


/** TrackBar::getRange()
 *
 */
RexxMethod1(RexxObjectPtr, tb_getRange, CSELF, pCSelf)
{
    RexxDirectoryObject result = context->NewDirectory();
    HWND hCtrl = getDChCtrl(pCSelf);

    context->DirectoryPut(result, context->Intptr((intptr_t)SendMessage(hCtrl, TBM_GETRANGEMIN, 0,0)), "MIN");
    context->DirectoryPut(result, context->Intptr((intptr_t)SendMessage(hCtrl, TBM_GETRANGEMAX, 0,0)), "MAX");

    return result;
}


/** TrackBar::getSelRange()
 *
 */
RexxMethod1(RexxObjectPtr, tb_getSelRange, CSELF, pCSelf)
{
    RexxDirectoryObject result = context->NewDirectory();
    HWND hCtrl = getDChCtrl(pCSelf);

    context->DirectoryPut(result, context->Intptr((intptr_t)SendMessage(hCtrl, TBM_GETSELSTART, 0,0)), "START");
    context->DirectoryPut(result, context->Intptr((intptr_t)SendMessage(hCtrl, TBM_GETSELEND, 0,0)), "END");

    return result;
}



/**
 * Methods for the UpDown class.
 */
#define UP_DOWN_CLASS      "UpDown"


/** UpDown::deltaPosReply() [Class method]
 *
 *  Constructs the (proper) reply object for a DELTAPOS UpDown event
 *  notification.
 *
 *  The Rexx programmer should use this method as the reply to the notification.
 *
 *  @param change    [optional] If true, the delta position message is to be
 *                   canceled or modified.  If false, there is no change to the
 *                   delta position message, and the other arguments are always
 *                   ignored. The default is false.
 *
 *  @param cancel    [optional] If true, the delta position message is canceled
 *                   completely.  If false the message is not canceled, and the
 *                   delta position is set to newDelta.  The default is false.
 *
 *  @param newDelta  [optional] The amount by which the current position of the
 *                   updown control is changed.  The default is 1.  This
 *                   argument is only used if change is true and cancel is
 *                   false.
 */
RexxMethod3(RexxObjectPtr, ud_deltaPosReply_cls, OPTIONAL_logical_t, change, OPTIONAL_logical_t, cancel, OPTIONAL_int32_t, newDelta)
{
    if ( ! change )
    {
        return TheFalseObj;
    }

    RexxBufferObject _dpr = context->NewBuffer(sizeof(DELTAPOSREPLY));
    if ( _dpr == NULLOBJECT )
    {
        return TheFalseObj;
    }

    PDELTAPOSREPLY pdpr = (PDELTAPOSREPLY)context->BufferData(_dpr);
    pdpr->change = true;
    pdpr->cancel = cancel ? true : false;

    if ( ! pdpr->cancel )
    {
        pdpr->newDelta = argumentExists(1) ? newDelta : 1;
    }
    return _dpr;
}


/** UpDown::setRange()
 *
 *  Sets the range, (minimum and maximum,) for the up down control.
 *
 *  Unlike other some other controls, the maximum position may be less than the
 *  minimum, and in that case clicking the up arrow button decreases the current
 *  position. To put it another way, up means moving towards the maximum
 *  position.
 *
 *  The range can be specified using one argument, a directory object, or two
 *  arguments
 *
 *  Form 1:
 *
 *  @param   aDirectory  [OPTIONAL]  The directory indexes "min" and "max" are
 *                       used to set the range.  The min index specifies the
 *                       minium postition and the max index specifies the
 *                       maximum position.  If an index is omitted, the
 *                       corresponding default for that index is set.  0 for the
 *                       minimum position and 100 for the maximum.
 *
 *  Form 2:
 *
 *  @param   min   [OPTIONAL]  The minimum position.  0 is the default.
 *  @param   max   [OPTIONAL]  The maximum.  100 is the default.
 *
 *  @return  0 always.
 *
 */
RexxMethod3(RexxObjectPtr, ud_setRange, OPTIONAL_RexxObjectPtr, minObj, OPTIONAL_int32_t, max, CSELF, pCSelf)
{
    HWND hwnd = getDChCtrl(pCSelf);

    int32_t min = 0;
    bool usingDirectory = false;

    if ( argumentExists(1) )
    {
        if ( context->IsOfType(minObj, "DIRECTORY") )
        {
            usingDirectory = true;
            if ( ! rxIntFromDirectory(context, (RexxDirectoryObject)minObj, "MIN", &min, 1, false) )
            {
                goto done_out;
            }
            max = 100;
            if ( ! rxIntFromDirectory(context, (RexxDirectoryObject)minObj, "MAX", &max, 1, false) )
            {
                goto done_out;
            }
        }
        else
        {
            if ( ! context->Int32(minObj, &min) )
            {
                wrongRangeException(context->threadContext, 1,  INT32_MIN, INT32_MAX, minObj);
            }
        }
    }

    if ( ! usingDirectory && argumentOmitted(2) )
    {
        max = 100;
    }
    SendMessage(hwnd, UDM_SETRANGE32, min, max);

done_out:
    return TheZeroObj;
}


/** UpDown::getRange()
 *
 *
 */
RexxMethod1(RexxObjectPtr, ud_getRange, CSELF, pCSelf)
{
    RexxDirectoryObject result = context->NewDirectory();
    HWND hCtrl = getDChCtrl(pCSelf);

    int32_t min, max;
    SendMessage(hCtrl, UDM_GETRANGE32, (WPARAM)&min, (LPARAM)&max);

    RexxMethodContext *c = context;
    context->DirectoryPut(result, context->Int32(min), "MIN");
    context->DirectoryPut(result, context->Int32(max), "MAX");

    return result;
}


/** UpDown::getBuddy()
 *
 *  Returns the buddy dialog control ojbect, or .nil if there is no buddy.
 *
 *  @remarks  createControlFromHwnd() can handle a null window handle, or a
 *            winUnknown type for the control type.  In both cases, it simply
 *            returns the .nil object.  This is what we want, so we don't check
 *            for those things.
 */
RexxMethod1(RexxObjectPtr, ud_getBuddy, CSELF, pCSelf)
{
    HWND hUpDown = getDChCtrl(pCSelf);

    HWND hBuddy = (HWND)SendMessage(hUpDown, UDM_GETBUDDY, 0, 0);
    oodControl_t ctrl = controlHwnd2controlType(hBuddy);

    return createControlFromHwnd(context, (pCDialogControl)pCSelf, hBuddy, ctrl, true);
}


/** UpDown::setBuddy()
 *
 *  Sets the buddy control for the up-down and returns the old buddy, if there
 *  was one.
 *
 */
RexxMethod2(RexxObjectPtr, ud_setBuddy, RexxObjectPtr, buddy, CSELF, pCSelf)
{
    RexxObjectPtr result = TheNilObj;

    if ( requiredClass(context->threadContext, buddy, "DIALOGCONTROL", 1) )
    {
        pCDialogControl pcdc    = controlToCSelf(context, buddy);
        HWND            hUpDown = getDChCtrl(pCSelf);

        HWND hOldBuddy = (HWND)SendMessage(hUpDown, UDM_SETBUDDY, (WPARAM)pcdc->hCtrl, 0);

        oodControl_t ctrl = controlHwnd2controlType(hOldBuddy);
        result = createControlFromHwnd(context, (pCDialogControl)pCSelf, hOldBuddy, ctrl, true);
    }
    return result;
}


/** UpDown::getPosition()
 *
 *
 *  @note  Sets the .SystemErrorCode.  If the returned position is not correct,
 *         the .SystemErrorCode is set to
 *
 *         ERROR_INVALID_DATA (13)  "The data is invalid"

 */
RexxMethod1(int32_t, ud_getPosition, CSELF, pCSelf)
{
    oodResetSysErrCode(context->threadContext);
    HWND hCtrl = getDChCtrl(pCSelf);

    BOOL error;
    int32_t pos = (int32_t)SendMessage(hCtrl, UDM_GETPOS32, 0, (LPARAM)&error);
    if ( error )
    {
        oodSetSysErrCode(context->threadContext, ERROR_INVALID_DATA);
    }
   return pos;
}


RexxMethod1(RexxArrayObject, ud_getAcceleration, CSELF, pCSelf)
{
    HWND hCtrl = getDChCtrl(pCSelf);
    LPUDACCEL pUDA = NULL;

    size_t count = (size_t)SendMessage(hCtrl, UDM_GETACCEL, 0, NULL);
    RexxArrayObject result = context->NewArray(count);

    pUDA = (LPUDACCEL)malloc(count * sizeof(UDACCEL));
    if ( pUDA == NULL )
    {
        outOfMemoryException(context->threadContext);
        goto done_out;
    }

    SendMessage(hCtrl, UDM_GETACCEL, count, (LPARAM)pUDA);
    for ( size_t i = 0; i < count; i++ )
    {
        RexxDirectoryObject d = context->NewDirectory();
        context->DirectoryPut(d, context->UnsignedInt32(pUDA[i].nSec), "SECONDS");
        context->DirectoryPut(d, context->UnsignedInt32(pUDA[i].nInc), "INCREMENT");
        context->ArrayPut(result, d, i + 1);
    }

done_out:
    safeFree(pUDA);
    return result;
}


RexxMethod2(logical_t, ud_setAcceleration, RexxArrayObject, vals, CSELF, pCSelf)
{
    RexxMethodContext *c = context;
    HWND hCtrl = getDChCtrl(pCSelf);
    LPUDACCEL pUDA = NULL;

    size_t count = c->ArrayItems(vals);
    if ( count < 1 )
    {
        emptyArrayException(c->threadContext, 1);
        goto done_out;
    }

    pUDA = (LPUDACCEL)malloc(count * sizeof(UDACCEL));
    if ( pUDA == NULL )
    {
        outOfMemoryException(context->threadContext);
        goto done_out;
    }

    RexxDirectoryObject d;
    uint32_t secs, incr;

    for ( size_t i = 1; i <= count; i++ )
    {
        if ( ! rxDirectoryFromArray(context, vals, i, &d, 1) )
        {
            goto done_out;
        }
        if ( ! rxNumberFromDirectory(context, (RexxDirectoryObject)d, "SECONDS", &secs, 1, true) )
        {
            goto done_out;
        }
        if ( ! rxNumberFromDirectory(context, (RexxDirectoryObject)d, "INCREMENT", &incr, 1, true) )
        {
            goto done_out;
        }

        pUDA[i - 1].nInc = incr;
        pUDA[i - 1].nSec = secs;
    }

    SendMessage(hCtrl, UDM_SETACCEL, count, (LPARAM)pUDA);

done_out:
    safeFree(pUDA);
    return 0;
}

