/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2013-2013 Rexx Language Association. All rights reserved.    */
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
 * oodStatusBar.cpp
 *
 * Implementation for the StatusBar dialog control.
 */
#include "ooDialog.hpp"     // Must be first, includes windows.h, commctrl.h, and oorexxapi.h

#include <stdio.h>
#include <shlwapi.h>

#include "APICommon.hpp"
#include "oodCommon.hpp"
#include "oodControl.hpp"
#include "oodResources.hpp"
//#include "oodMessaging.hpp"
//#include "oodShared.hpp"


/**
 * Methods for the StatusBar class.
 */
#define STATUSBAR_CLASS        "StatusBar"

#define STB_ICON_ATTRIBUTE     "STB!!ICONS!!ATTRIBUTE"

uint32_t keyword2sbars(CSTRING flags)
{
    uint32_t val = 0;

    if ( flags != NULL )
    {
        if ( StrStrI(flags, "SIZEGRIP"    ) != NULL ) val |= SBARS_SIZEGRIP    ;
        if ( StrStrI(flags, "TOOLTIPS"    ) != NULL ) val |= SBARS_TOOLTIPS    ;
        if ( StrStrI(flags, "NONE"        ) != NULL ) val  = 0;
    }

    return val;
}

/**
 * Converts a set of SBARS_* StatusBar style flags to their keyword string.
 *
 * @param c
 * @param flags
 *
 * @return A Rexx string object.
 */
 RexxStringObject sbars2keyword(RexxMethodContext *c, uint32_t flags)
{
    char buf[256];
    *buf = '\0';

    if ( flags & SBARS_SIZEGRIP     ) strcat(buf, "SIZEGRIP "     );
    if ( flags & SBARS_TOOLTIPS     ) strcat(buf, "TOOLTIPS "     );

    if ( *buf == '\0' )
    {
        strcpy(buf, "nil");
    }
    else
    {
        *(buf + strlen(buf) - 1) = '\0';
    }
    return c->String(buf);
}

RexxObjectPtr stbGetIcon(RexxMethodContext *c, uint32_t index)
{
    RexxArrayObject icons = (RexxArrayObject)c->GetObjectVariable(STB_ICON_ATTRIBUTE);
    if ( icons == NULLOBJECT )
    {
        return TheNilObj;
    }

    RexxObjectPtr icon = c->ArrayAt(icons, index);
    return icon == NULLOBJECT ? TheNilObj : icon;
}

RexxObjectPtr stbStoreIcon(RexxMethodContext *c, RexxObjectPtr icon, uint32_t index)
{
    RexxArrayObject icons = (RexxArrayObject)c->GetObjectVariable(STB_ICON_ATTRIBUTE);
    if ( icons == NULLOBJECT )
    {
        icons = c->NewArray(5);
        c->SetObjectVariable(STB_ICON_ATTRIBUTE, icons);
        c->ArrayPut(icons, icon, index);
        return TheNilObj;
    }

    RexxObjectPtr existing = stbGetIcon(c, index);

    c->ArrayPut(icons, icon, index);
    return existing;
}

/** StatusBar::getBorders()
 *
 *  Returns an array of integers containing the borders of the status bar.  The
 *  item at index 1 is the  width of the horizontal border, the item at index 2
 *  is the width of the vertical border, and the item at index 3 is the width of
 *  the border between rectangles.
 *
 *  Returns the .nil object on error.
 */
RexxMethod1(RexxObjectPtr, stb_getBorders, CSELF, pCSelf)
{
    int32_t borders[3];

    if (SendMessage(getDChCtrl(pCSelf), SB_GETBORDERS, 0, (LPARAM)&borders) )
    {
        RexxArrayObject rxBorders = context->ArrayOfThree(context->Int32(borders[0]),
                                                          context->Int32(borders[1]),
                                                          context->Int32(borders[2]));
        return rxBorders;
    }
    return TheNilObj;
}

/** StatusBar::getIcon()
 *
 *
 *  @remarks  Note that we are going to retrieve the icon Image object stored in
 *            our attribute array, so we don't want to decrement the index.
 */
RexxMethod2(RexxObjectPtr, stb_getIcon, uint32_t, index, CSELF, pCSelf)
{
    return stbGetIcon(context, index);
}

/** StatusBar::getParts()
 *
 *  Returns the number of parts in the status bar, and optionally, the
 *  co-ordinate of the right edge of each part
 *
 *  @param edges  [optional] [in / out] An array object in which the right edge
 *                of each part is returned.  If this argument is present, it
 *                will have indexes filled in with the right co-ordinate of each
 *                part.  The item at index 1 will have the right co-ordinate of
 *                the first part, the item at index 2 will have the right edge
 *                of the second part, etc.. Up to the numbe of parts.  Existing
 *                items at those indexes will be over-written.  It is suggested
 *                that the programmer use an empty array, but not required.
 *
 *  @return Returns the count of parts in this status bar, always.
 */
RexxMethod2(uint32_t, stb_getParts, OPTIONAL_RexxArrayObject, rxEdges, CSELF, pCSelf)
{
    RexxMethodContext *c = context;

    uint32_t count = (uint32_t)SendMessage(getDChCtrl(pCSelf), SB_GETPARTS, 0, 0);
    if ( argumentExists(1) )
    {
        int32_t *edges = (int32_t *)LocalAlloc(LPTR, count * sizeof(int32_t));
        if ( edges == NULL )
        {
            outOfMemoryException(context->threadContext);
            goto done_out;
        }

        SendMessage(getDChCtrl(pCSelf), SB_GETPARTS, count, (LPARAM)edges);
        for ( size_t i = 0; i < count; i++ )
        {
            c->ArrayPut(rxEdges, c->Int32(edges[i]), i + 1);
        }
    }

done_out:
    return count;
}

/** StatusBar::getRect()
 *
 *  Returns a .Rect object that contains the bounding rectangle of the specified
 *  part in this status bar.
 *
 *  @param index  [required] The one-based index of the part.
 *
 *  @return Returns a .Rect object with the bounding rectangle of the pare, or
 *          .nil on error.
 */
RexxMethod2(RexxObjectPtr, stb_getRect, uint32_t, index, CSELF, pCSelf)
{
    RECT r;

    index--;
    if ( SendMessage(getDChCtrl(pCSelf), SB_GETRECT, index, (LPARAM)&r) )
    {
        return rxNewRect(context, &r);
    }
    return TheNilObj;
}

/** StatusBar::isSimple()
 *
 *
 */
RexxMethod1(logical_t, stb_isSimple, CSELF, pCSelf)
{
    return SendMessage(getDChCtrl(pCSelf), SB_ISSIMPLE, 0, 0);
}

/** StatusBar::setBkColor()
 *
 *
 */
RexxMethod2(uint32_t, stb_setBkColor, uint32_t, clr, CSELF, pCSelf)
{
    return (uint32_t)SendMessage(getDChCtrl(pCSelf), SB_SETBKCOLOR, 0, clr);
}

/** StatusBar::setIcon()
 *
 *
 *  @returns  The existing icon, or .nil if there was no existing icon, on
 *            success.  Returns -1 if there was an error.
 */
RexxMethod3(RexxObjectPtr, stb_setIcon, RexxObjectPtr, icon, uint32_t, index, CSELF, pCSelf)
{
    RexxMethodContext *c = context;
    RexxObjectPtr existing = TheNegativeOneObj;
    index--;

    if ( c->IsOfType(icon, "IMAGE") )
    {
        POODIMAGE poi = rxGetImageIcon(context, icon, 1);
        if ( poi == NULL )
        {
            goto done_out;
        }
        if ( ! poi->isValid )
        {
            nullObjectException(context->threadContext, "Image", 1);
            goto done_out;
        }

        if ( SendMessage(getDChCtrl(pCSelf), SB_SETICON, index, (LPARAM)poi->hImage) )
        {
            existing = stbStoreIcon(context, icon, index + 1);
        }
        goto done_out;
    }
    else if ( icon == TheNilObj )
    {
        if ( SendMessage(getDChCtrl(pCSelf), SB_SETICON, index, NULL) )
        {
            // This will put .nil at the index, which is good.
            existing = stbStoreIcon(context, icon, index + 1);
        }
        goto done_out;
    }
    else
    {
        wrongArgValueException(c->threadContext, 1, "an icon image or .nil", icon);
    }

done_out:
    return existing;
}

/** StatusBar::setMinHeight()
 *
 *
 */
RexxMethod2(uint32_t, stb_setMinHeight, uint32_t, height, CSELF, pCSelf)
{
    SendMessage(getDChCtrl(pCSelf), SB_SETMINHEIGHT, height, 0);
    SendMessage(getDChCtrl(pCSelf), WM_SIZE, 0, 0);
    return 0;
}

/** StatusBar::setParts()
 *
 *  Sets the number of parts in a status window and the coordinate of the right
 *  edge of each part.
 *
 *  @param  rightEdge  [required]  An array of integers.  Each item specifies
 *                     the right edge, in client co-oridnates, in pixels, of a
 *                     part.
 *
 *                     -1 specifies the part extends to the right edge of client
 *                     area.
 *
 *  @return  True on success, otherwise false.
 *
 *  @note  The array must be non-sparse and contain only integers.
 */
RexxMethod2(logical_t, stb_setParts, RexxArrayObject, _edges, CSELF, pCSelf)
{
    logical_t result = FALSE;
    int32_t   *edges = NULL;

    size_t count = context->ArrayItems(_edges);
    if ( count == 0 )
    {
        emptyArrayException(context->threadContext, 1);
        goto done_out;
    }

    edges = (int32_t *)LocalAlloc(LPTR, count * sizeof(int32_t));
    if ( edges == NULL )
    {
        outOfMemoryException(context->threadContext);
        goto done_out;
    }

    for ( size_t i = 1; i <= count; i++ )
    {
        int32_t e;

        RexxObjectPtr rxE = context->ArrayAt(_edges, i);
        if ( rxE == NULLOBJECT )
        {
            sparseArrayException(context->threadContext, 1, i);
            goto done_out;
        }
        if ( ! context->Int32(rxE, &e) )
        {
            wrongObjInArrayException(context->threadContext, 1, i, "a whole number less than 2147483647", rxE);
            goto done_out;
        }

        edges[i - 1] = e;
    }

    if ( SendMessage(getDChCtrl(pCSelf), SB_SETPARTS, count, (LPARAM)edges) )
    {
        result = TRUE;
    }

done_out:
    return result;
}

/** StatusBar::setText()
 *
 *  Sets
 *
 *  @param  text       [required]  The text for the specified part.
 *
 *  @param  index      [required]  The 1-based index of the part.
 *
 *  @param  style      [optional]  List of keywords that specify the style of
 *                     the status bar.  If this argument is omitted the status
 *                     bar has its default style.
 *
 *  @return  True on success, false otherwise.
 */
RexxMethod4(logical_t, stb_setText, CSTRING, text, uint32_t, index, OPTIONAL_CSTRING, _style, CSELF, pCSelf)
{
    uint32_t style = keyword2sbars(_style);

    index--;
    index |= style;
    return SendMessage(getDChCtrl(pCSelf), SB_SETTEXT, (WPARAM)index, (LPARAM)text);
}

/** StatusBar::simple()
 *
 *
 */
RexxMethod2(uint32_t, stb_simple, logical_t, simple, CSELF, pCSelf)
{
    SendMessage(getDChCtrl(pCSelf), SB_SIMPLE, simple, 0);
    return 0;
}

