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
 * oodToolBar.cpp
 *
 * Implementation for the ToolBar dialog control.
 */
#include "ooDialog.hpp"     // Must be first, includes windows.h, commctrl.h, and oorexxapi.h

#include <stdio.h>
#include <shlwapi.h>

#include "APICommon.hpp"
#include "oodCommon.hpp"
#include "oodControl.hpp"
// #include "oodMessaging.hpp"
// #include "oodResources.hpp"
// #include "oodShared.hpp"


/**
 *  Methods for the .TbButton class.
 */
#define TBBUTTON_CLASS           "TbButton"

#define TBBUTTON_OBJ_MAGIC       "BPwcbLJ1c"
#define TBBUTTON_TEXT_MAX        260


inline bool isTbbInternalInit(RexxMethodContext *context, RexxObjectPtr rxCmdID, CSTRING text)
{
    return argumentExists(1) && context->IsBuffer(rxCmdID) && argumentExists(2) && strcmp(text, TBBUTTON_OBJ_MAGIC) == 0;
}

/**
 * Converts a string of keywords to the proper TBSTATE_* state flag.
 *
 * @param flags
 *
 * @return uint32_t
 */
uint32_t keyword2tbstate(CSTRING flags)
{
    uint32_t val = 0;

    if (      StrStrI(flags, "CHECKED"       ) != NULL ) val |= TBSTATE_CHECKED       ;
    else if ( StrStrI(flags, "ELLIPSES"      ) != NULL ) val |= TBSTATE_ELLIPSES      ;
    else if ( StrStrI(flags, "ENABLED"       ) != NULL ) val |= TBSTATE_ENABLED       ;
    else if ( StrStrI(flags, "HIDDEN"        ) != NULL ) val |= TBSTATE_HIDDEN        ;
    else if ( StrStrI(flags, "INDETERMINATE" ) != NULL ) val |= TBSTATE_INDETERMINATE ;
    else if ( StrStrI(flags, "MARKED"        ) != NULL ) val |= TBSTATE_MARKED        ;
    else if ( StrStrI(flags, "PRESSED"       ) != NULL ) val |= TBSTATE_PRESSED       ;
    else if ( StrStrI(flags, "WRAP"          ) != NULL ) val |= TBSTATE_WRAP          ;

    return val;
}


/**
 * Converts a set of TBSTATE_* state flags to their keyword string.
 *
 * @param c
 * @param flags
 *
 * @return A Rexx string object.
 */
 RexxStringObject tbstate2keyword(RexxMethodContext *c, uint32_t flags)
{
    char buf[256];  // This is big enough.
    *buf = '\0';

    if ( flags & TBSTATE_CHECKED        ) strcat(buf, "CHECKED"          );
    if ( flags & TBSTATE_ELLIPSES       ) strcat(buf, "ELLIPSES "        );
    if ( flags & TBSTATE_ENABLED        ) strcat(buf, "ENABLED "         );
    if ( flags & TBSTATE_HIDDEN         ) strcat(buf, "HIDDEN "          );
    if ( flags & TBSTATE_INDETERMINATE  ) strcat(buf, "INDETERMINATE "   );
    if ( flags & TBSTATE_MARKED         ) strcat(buf, "MARKED "          );
    if ( flags & TBSTATE_PRESSED        ) strcat(buf, "PRESSED "         );
    if ( flags & TBSTATE_WRAP           ) strcat(buf, "WRAP "            );

    if ( *buf != '\0' )
    {
        *(buf + strlen(buf) - 1) = '\0';
    }
    return c->String(buf);
}

/**
 * Converts a string of keywords to the proper BTNS_* style flag.
 *
 * @param flags
 *
 * @return uint32_t
 */
uint32_t keyword2btns(CSTRING flags)
{
    uint32_t val      = 0;
    char     buf[512] = {'\0'};

    // Add a space to the end of the string, then we can test for "keywored "
    _snprintf(buf, sizeof(buf), "%s ", flags);

    if ( StrStrI(flags, "BUTTON "        ) != NULL ) val |= BTNS_BUTTON        ;
    if ( StrStrI(flags, "SEP "           ) != NULL ) val |= BTNS_SEP           ;
    if ( StrStrI(flags, "CHECK "         ) != NULL ) val |= BTNS_CHECK         ;
    if ( StrStrI(flags, "GROUP "         ) != NULL ) val |= BTNS_GROUP         ;
    if ( StrStrI(flags, "CHECKGROUP "    ) != NULL ) val |= BTNS_CHECKGROUP    ;
    if ( StrStrI(flags, "DROPDOWN "      ) != NULL ) val |= BTNS_DROPDOWN      ;
    if ( StrStrI(flags, "AUTOSIZE "      ) != NULL ) val |= BTNS_AUTOSIZE      ;
    if ( StrStrI(flags, "NOPREFIX "      ) != NULL ) val |= BTNS_NOPREFIX      ;
    if ( StrStrI(flags, "SHOWTEXT "      ) != NULL ) val |= BTNS_SHOWTEXT      ;
    if ( StrStrI(flags, "WHOLEDROPDOWN " ) != NULL ) val |= BTNS_WHOLEDROPDOWN ;

    return val;
}


/**
 * Converts a set of BTNS_* style flags to their keyword string.
 *
 * @param c
 * @param flags
 *
 * @return A Rexx string object.
 */
 RexxStringObject btns2keyword(RexxMethodContext *c, uint32_t flags)
{
    char buf[256];
    *buf = '\0';

    if ( flags & BTNS_BUTTON          ) strcat(buf, "BUTTON "         );
    if ( flags & BTNS_SEP             ) strcat(buf, "SEP "            );
    if ( flags & BTNS_CHECK           ) strcat(buf, "CHECK "          );
    if ( flags & BTNS_GROUP           ) strcat(buf, "GROUP "          );
    if ( flags & BTNS_CHECKGROUP      ) strcat(buf, "CHECKGROUP "     );
    if ( flags & BTNS_DROPDOWN        ) strcat(buf, "DROPDOWN "       );
    if ( flags & BTNS_AUTOSIZE        ) strcat(buf, "AUTOSIZE "       );
    if ( flags & BTNS_NOPREFIX        ) strcat(buf, "NOPREFIX "       );
    if ( flags & BTNS_SHOWTEXT        ) strcat(buf, "SHOWTEXT "       );
    if ( flags & BTNS_WHOLEDROPDOWN   ) strcat(buf, "WHOLEDROPDOWN "  );

    if ( *buf != '\0' )
    {
        *(buf + strlen(buf) - 1) = '\0';
    }
    return c->String(buf);
}

RexxObjectPtr getTbbText(RexxMethodContext *c, LPTBBUTTON ptbb)
{
    if ( ptbb->iString == NULL )
    {
        return TheNilObj;
    }

    return c->String((CSTRING)ptbb->iString);
}

/**
 * Sets the text attribute for the TbButton object.
 *
 * If the TbButton is used to receive information, the iString member has to
 * point to a buffer to recieve the text.  When setting text, we probably do not
 * need to allocate a buffer, we could just assign the pointer.  However, to
 * keep things simpler, we just always allocate a buffer.
 *
 * Also, we set an arbitrary length of the text to 260 characters.  This is
 * similar to the maximum length of a string that will be displayed in other
 * controls.
 *
 * @param c
 * @param ptbb
 * @param text
 *
 * @return bool
 */
bool setTbbText(RexxMethodContext *c, LPTBBUTTON ptbb, CSTRING text, size_t argPos)
{
    if ( text != NULL )
    {
        size_t len = strlen(text);
        if ( len > TBBUTTON_TEXT_MAX )
        {
            stringTooLongException(c->threadContext, argPos, TBBUTTON_TEXT_MAX, len);
            return false;
        }
    }

    if ( ptbb->iString == NULL )
    {
        ptbb->iString = (INT_PTR)LocalAlloc(LPTR, TBBUTTON_TEXT_MAX + 1);
        if ( ptbb->iString == NULL )
        {
            outOfMemoryException(c->threadContext);
            return false;
        }
    }

    if ( text != NULL )
    {
        strcpy((char *)ptbb->iString, text);
    }
    return true;
}

bool setTbbCmdID(RexxMethodContext *c, LPTBBUTTON ptbb, RexxObjectPtr rxCmdID, size_t argPos)
{
    ptbb->idCommand = oodGlobalID(c, rxCmdID, argPos, true);
    if ( ptbb->idCommand == OOD_ID_EXCEPTION )
    {
        return false;
    }
    return true;
}


/** TbButton::init()      [Class]
 *
 */
RexxMethod1(RexxObjectPtr, tbb_init_cls, OSELF, self)
{
    if ( isOfClassType(context, self, TBBUTTON_CLASS) )
    {
        TheTbButtonClass = (RexxClassObject)self;
        context->RequestGlobalReference(TheTbButtonClass);
    }
    return NULLOBJECT;
}


/** TbButton::uninit()
 *
 */
RexxMethod1(RexxObjectPtr, tbb_unInit, CSELF, pCSelf)
{
#if 0
    printf("In tbb_unInit() pCSelf=%p\n", pCSelf);
#endif

    if ( pCSelf != NULLOBJECT )
    {
        LPTBBUTTON ptbb = (LPTBBUTTON)pCSelf;

#if 0
    printf("In tbb_unInit() iString=%p\n", ptbb->iString);
#endif
        safeLocalFree((void *)ptbb->iString);
    }
    return NULLOBJECT;
}

/** TbButton::init()
 *
 *
 *  @param  cmdID
 *  @param  text
 *  @param  style
 *  @param  state
 *  @param  itemDate
 *  @param  bitmapID
 */
RexxMethod6(RexxObjectPtr, tbb_init, OPTIONAL_RexxObjectPtr, rxCmdID, OPTIONAL_CSTRING, text, OPTIONAL_CSTRING, style,
            OPTIONAL_CSTRING, state, OPTIONAL_RexxObjectPtr, itemData, OPTIONAL_int32_t, bitmapID)
{
    if ( isTbbInternalInit(context, rxCmdID, text) )
    {
        context->SetObjectVariable("CSELF", rxCmdID);
        goto done_out;
    }

    RexxBufferObject obj = context->NewBuffer(sizeof(TBBUTTON));
    context->SetObjectVariable("CSELF", obj);

    LPTBBUTTON ptbb = (LPTBBUTTON)context->BufferData(obj);
    memset(ptbb, 0, sizeof(TBBUTTON));

    if ( argumentExists(1) )
    {
        if ( ! setTbbCmdID(context, ptbb, rxCmdID, 1) )
        {
            goto done_out;
        }
    }

    // We always create a buffer to recieve text.  If the user omits the text
    // argument, the buffer will be created and set to the empty string.
    if ( ! setTbbText(context, ptbb, text, 2) )
    {
        goto done_out;
    }

    if ( argumentExists(3) )
    {
        ptbb->fsStyle = keyword2btns(style);
    }

    if ( argumentExists(4) )
    {
        ptbb->fsState = keyword2tbstate(state);
    }

    if ( argumentExists(5) )
    {
        ptbb->dwData = (DWORD_PTR)itemData;
    }

    ptbb->iBitmap = argumentExists(6) ? bitmapID : I_IMAGENONE;

done_out:
    return NULLOBJECT;
}

/** TbButton::bitmapID                [attribute]
 */
RexxMethod1(int32_t, tbb_bitmapID, CSELF, pCSelf)
{
    return ((LPTBBUTTON)pCSelf)->iBitmap;
}
RexxMethod2(RexxObjectPtr, tbb_setBitmapID, int32_t, bitmapID, CSELF, pCSelf)
{
    ((LPTBBUTTON)pCSelf)->iBitmap = bitmapID;
    return NULLOBJECT;
}

/** TbButton::cmdID                 [attribute]
 */
RexxMethod1(uint32_t, tbb_cmdID, CSELF, pCSelf)
{
    return (uint32_t)((LPTBBUTTON)pCSelf)->dwData;
}
RexxMethod2(RexxObjectPtr, tbb_setCmdID, RexxObjectPtr, rxCmdID, CSELF, pCSelf)
{
    setTbbCmdID(context, (LPTBBUTTON)pCSelf, rxCmdID, 1);
    return NULLOBJECT;
}

/** TbButton::itemData               [attribute]
 */
RexxMethod1(RexxObjectPtr, tbb_itemData, CSELF, pCSelf)
{
    LPTBBUTTON ptbb = (LPTBBUTTON)pCSelf;
    return ((ptbb->dwData == NULL) ? TheNilObj : (RexxObjectPtr)ptbb->dwData);
}
RexxMethod2(RexxObjectPtr, tbb_setItemData, RexxObjectPtr, userData, CSELF, pCSelf)
{
    ((LPTBBUTTON)pCSelf)->dwData = (DWORD_PTR)userData;
    return NULLOBJECT;
}

/** TbButton::state                   [attribute]
 */
RexxMethod1(RexxStringObject, tbb_state, CSELF, pCSelf)
{
    return tbstate2keyword(context, ((LPTBBUTTON)pCSelf)->fsState);
}
RexxMethod2(RexxObjectPtr, tbb_setState, CSTRING, state, CSELF, pCSelf)
{
    ((LPTBBUTTON)pCSelf)->fsState = keyword2tbstate(state);
    return NULLOBJECT;
}

/** TbButton::style                   [attribute]
 */
RexxMethod1(RexxStringObject, tbb_style, CSELF, pCSelf)
{
    return btns2keyword(context, ((LPTBBUTTON)pCSelf)->fsStyle);
}
RexxMethod2(RexxObjectPtr, tbb_setStyle, CSTRING, style, CSELF, pCSelf)
{
    ((LPTBBUTTON)pCSelf)->fsStyle = keyword2btns(style);
    return NULLOBJECT;
}

/** TbButton::text                   [attribute]
 */
RexxMethod1(RexxObjectPtr, tbb_text, CSELF, pCSelf)
{
    return getTbbText(context, (LPTBBUTTON)pCSelf);
}
RexxMethod2(RexxObjectPtr, tbb_setText, CSTRING, text, CSELF, pCSelf)
{
    setTbbText(context, (LPTBBUTTON)pCSelf, text, 1);
    return NULLOBJECT;
}



/**
 * Methods for the ToolBar class.
 */
#define TOOLBAR_CLASS   "ToolBar"



/** ToolBar::addButtons()
 */
RexxMethod2(RexxObjectPtr, tb_addButtons, RexxArrayObject, buttons, CSELF, pCSelf)
{
    RexxMethodContext *c     = context;
    RexxObjectPtr result     = TheFalseObj;
    LPTBBUTTON    pTbButtons = NULL;

    size_t count = c->ArrayItems(buttons);
    if ( count == 0 )
    {
        emptyArrayException(c->threadContext, 1);
        goto done_out;
    }

    pTbButtons = (LPTBBUTTON)LocalAlloc(LPTR, count * sizeof(TBBUTTON));
    if ( pTbButtons == NULL )
    {
        outOfMemoryException(c->threadContext);
        goto done_out;
    }

    for ( size_t i = 1; i <= count; i++ )
    {
        LPTBBUTTON pttb;

        RexxObjectPtr rxTbb = c->ArrayAt(buttons, i);
        if ( rxTbb == NULLOBJECT )
        {
            sparseArrayException(c->threadContext, 1, i);
            goto done_out;
        }
        if ( ! c->IsOfType(rxTbb, TBBUTTON_CLASS) )
        {
            wrongObjInArrayException(c->threadContext, 1, i, "a TbButton object", rxTbb);
            goto done_out;
        }

        pttb = (LPTBBUTTON)context->ObjectToCSelf(rxTbb);
        memcpy(pTbButtons + i - 1, pttb, sizeof(TBBUTTON));
    }

    if ( SendMessage(getDChCtrl(pCSelf), TB_ADDBUTTONS, count, (LPARAM)pTbButtons) )
    {
        result = TheTrueObj;
    }

done_out:
    safeLocalFree(pTbButtons);
    return result;
}

/** ToolBar::autoSize()
 */
RexxMethod1(RexxObjectPtr, tb_autoSize, CSELF, pCSelf)
{
    SendMessage(getDChCtrl(pCSelf), TB_AUTOSIZE, 0, 0);
    return TheZeroObj;
}

/** ToolBar::buttonCount()
 */
RexxMethod1(uint32_t, tb_buttonCount, CSELF, pCSelf)
{
    return (uint32_t)SendMessage(getDChCtrl(pCSelf), TB_BUTTONCOUNT, 0, 0);
}

/** ToolBar::buttonStructSize()
 */
RexxMethod1(RexxObjectPtr, tb_buttonStructSize, CSELF, pCSelf)
{
    SendMessage(getDChCtrl(pCSelf), TB_BUTTONSTRUCTSIZE, sizeof(TBBUTTON), 0);
    return TheZeroObj;
}

/** ToolBar::insertButton()
 */
RexxMethod3(logical_t, tb_insertButton, RexxObjectPtr, tbButton, uint32_t, index, CSELF, pCSelf)
{
    if ( requiredClass(context->threadContext, tbButton, "TBBUTTON", 1) )
    {
        index--;
        return SendMessage(getDChCtrl(pCSelf), TB_INSERTBUTTON, index, (LPARAM)context->ObjectToCSelf(tbButton));
    }
    return FALSE;
}


