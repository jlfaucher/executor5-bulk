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
#include "oodResources.hpp"
// #include "oodMessaging.hpp"
// #include "oodShared.hpp"


/**
 *  Methods for the .TbButton class.
 */
#define TBBUTTON_CLASS           "TbButton"

#define TBBUTTON_OBJ_MAGIC       "BPwcbLJ1c"
#define TBBUTTON_TEXT_MAX        260
#define ADD_BITMAP_SRC_OBJ       "Image object, ResourceImage object, or documented keyword"
#define ADDLOAD_SYSTEM_IMAGES    "StdLarge, StdSmall, ViewLarge, ViewSmall, HistLarge, HistSmall, HistNormal, HistHot, HistDisabled, or HistPressed"

enum ImageListType {iltNormal, iltHot, iltDisabled, iltPressed, iltInvalid};

#define TB_ILNORMAL_ATTRIBUTE    "TB!!NORMAL!IMAGELIST"
#define TB_ILHOT_ATTRIBUTE       "TB!!HOT!IMAGELIST"
#define TB_ILDISABLED_ATTRIBUTE  "TB!!DISABLED!IMAGELIST"
#define TB_ILPRESSED_ATTRIBUTE   "TB!!PRESSED!IMAGELIST"

inline CSTRING iltype2attrName(ImageListType t)
{
    switch (t)
    {
        case iltNormal   : return TB_ILNORMAL_ATTRIBUTE;
        case iltHot      : return TB_ILHOT_ATTRIBUTE;
        case iltDisabled : return TB_ILDISABLED_ATTRIBUTE;
        case iltPressed  : return TB_ILPRESSED_ATTRIBUTE;
    }
    return "";
}

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

/**
 * Converts a keyword to the proper IDB_* flag.
 *
 * @param keyword
 *
 * @return uint32_t
 */
uint32_t keyword2idb(RexxMethodContext *c, CSTRING keyword, size_t argPos)
{
    uint32_t val = OOD_INVALID_ITEM_ID;

    if (      StrCmpI(keyword, "STDSMALL"      ) == 0 ) val = IDB_STD_SMALL_COLOR      ;
    else if ( StrCmpI(keyword, "STDLARGE"      ) == 0 ) val = IDB_STD_LARGE_COLOR      ;
    else if ( StrCmpI(keyword, "VIEWSMALL"     ) == 0 ) val = IDB_VIEW_SMALL_COLOR     ;
    else if ( StrCmpI(keyword, "VIEWLARGE"     ) == 0 ) val = IDB_VIEW_LARGE_COLOR     ;
    else if ( StrCmpI(keyword, "HISTSMALL"     ) == 0 ) val = IDB_HIST_SMALL_COLOR     ;
    else if ( StrCmpI(keyword, "HISTLARGE"     ) == 0 ) val = IDB_HIST_LARGE_COLOR     ;
    else if ( StrCmpI(keyword, "HISTNORMAL"    ) == 0 ) val = IDB_HIST_NORMAL          ;
    else if ( StrCmpI(keyword, "HISTHOT"       ) == 0 ) val = IDB_HIST_HOT             ;
    else if ( StrCmpI(keyword, "HISTDISABLED"  ) == 0 ) val = IDB_HIST_DISABLED        ;
    else if ( StrCmpI(keyword, "HISTPRESSED"   ) == 0 ) val = IDB_HIST_PRESSED         ;
    else
    {
        wrongArgValueException(c->threadContext, argPos, ADDLOAD_SYSTEM_IMAGES, keyword);
    }

    return val;
}

/**
 * Converts a IDB_* flag to its keyword.
 *
 * @param c
 * @param flag
 *
 * @return A Rexx string object.
 */
 RexxStringObject idb2keyword(RexxMethodContext *c, uint32_t flag)
{
    if ( flag == IDB_STD_SMALL_COLOR    ) return c->String("STDSMALL"         );
    if ( flag == IDB_STD_LARGE_COLOR    ) return c->String("STDLARGE"         );
    if ( flag == IDB_VIEW_SMALL_COLOR   ) return c->String("VIEWSMALL"        );
    if ( flag == IDB_VIEW_LARGE_COLOR   ) return c->String("VIEWLARGE"        );
    if ( flag == IDB_HIST_SMALL_COLOR   ) return c->String("HISTSMALL"        );
    if ( flag == IDB_HIST_LARGE_COLOR   ) return c->String("HISTLARGE"        );
    if ( flag == IDB_HIST_NORMAL        ) return c->String("HISTNORMAL"       );
    if ( flag == IDB_HIST_HOT           ) return c->String("HISTHOT"          );
    if ( flag == IDB_HIST_DISABLED      ) return c->String("HISTDISABLED"     );
    if ( flag == IDB_HIST_PRESSED       ) return c->String("HISTPRESSED"      );
    return c->NullString();
}

/**
 * Constructs the proper bitmap index from the inputs.
 *
 * @param ilID
 * @param offset
 * @param argPos
 *
 * @return A bitmap index on success, on error return OOD_ID_EXCEPTION
 */
int32_t constructBitmapID(RexxMethodContext *context, RexxObjectPtr rxBitmapID, uint32_t ilID,
                          uint32_t offset, size_t argPos)
{
    int32_t index;

    if ( context->Int32(rxBitmapID, &index) )
    {
        if ( index > 0 )
        {
            index--;
        }
    }
    else
    {
        index = oodGlobalID(context, rxBitmapID, argPos, true);
        if ( index == OOD_ID_EXCEPTION )
        {
            return OOD_INVALID_ITEM_ID;
        }
    }

    index += offset;
    return MAKELONG(index, ilID);
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

bool setTbbBitmapID(RexxMethodContext *c, LPTBBUTTON ptbb, RexxObjectPtr rxBitmapID, size_t argPos)
{
    ptbb->iBitmap = constructBitmapID(c, rxBitmapID, 0, 0, argPos);
    if ( ptbb->iBitmap == OOD_INVALID_ITEM_ID )
    {
        ptbb->iBitmap = I_IMAGENONE;
        return false;
    }
    return true;
}

/**
 * A TbButton object is not related to a dialog.  The user has to use the global
 * .constDir to use symbolic IDs.
 *
 * @param c
 * @param ptbb
 * @param rxCmdID
 * @param argPos
 *
 * @return bool
 */
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
            OPTIONAL_CSTRING, state, OPTIONAL_RexxObjectPtr, itemData, OPTIONAL_RexxObjectPtr, rxBitmapID)
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

    if ( argumentExists(6) )
    {
        setTbbBitmapID(context, ptbb, rxBitmapID, 6);
    }
    else
    {
        ptbb->iBitmap = I_IMAGENONE;
    }

done_out:
    return NULLOBJECT;
}

/** TbButton::bitmapID                [attribute]
 */
RexxMethod1(int32_t, tbb_bitmapID, CSELF, pCSelf)
{
    return ((LPTBBUTTON)pCSelf)->iBitmap;
}
RexxMethod2(RexxObjectPtr, tbb_setBitmapID, RexxObjectPtr, rxBitmapID, CSELF, pCSelf)
{
    setTbbBitmapID(context, (LPTBBUTTON)pCSelf, rxBitmapID, 1);
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

RexxMethod4(RexxObjectPtr, tbb_assignBitmapID, RexxObjectPtr, rxBitmapID, OPTIONAL_uint32_t, ilID,
            OPTIONAL_uint32_t, offset, CSELF, pCSelf)
{
    LPTBBUTTON ptbb = (LPTBBUTTON)pCSelf;
    ptbb->iBitmap = constructBitmapID(context, rxBitmapID, ilID, offset, 1);
    if ( ptbb->iBitmap == OOD_INVALID_ITEM_ID )
    {
        ptbb->iBitmap = I_IMAGENONE;
    }
    return NULLOBJECT;
}


/**
 * Methods for the ToolBar class.
 */
#define TOOLBAR_CLASS   "ToolBar"



/**
 * Adds some symbolic IDs that can be used when working with a toolbar to the
 * specified constDir.
 *
 * @param c
 * @param constDir
 */
void putToolBarSymbols(RexxMethodContext *c, RexxDirectoryObject constDir)
{
    c->DirectoryPut(constDir, c->Int32(STD_CUT            ), "STD_CUT"            );
    c->DirectoryPut(constDir, c->Int32(STD_COPY           ), "STD_COPY"           );
    c->DirectoryPut(constDir, c->Int32(STD_PASTE          ), "STD_PASTE"          );
    c->DirectoryPut(constDir, c->Int32(STD_UNDO           ), "STD_UNDO"           );
    c->DirectoryPut(constDir, c->Int32(STD_REDOW          ), "STD_REDOW"          );
    c->DirectoryPut(constDir, c->Int32(STD_DELETE         ), "STD_DELETE"         );
    c->DirectoryPut(constDir, c->Int32(STD_FILENEW        ), "STD_FILENEW"        );
    c->DirectoryPut(constDir, c->Int32(STD_FILEOPEN       ), "STD_FILEOPEN"       );
    c->DirectoryPut(constDir, c->Int32(STD_FILESAVE       ), "STD_FILESAVE"       );
    c->DirectoryPut(constDir, c->Int32(STD_PRINTPRE       ), "STD_PRINTPRE"       );
    c->DirectoryPut(constDir, c->Int32(STD_PROPERTIES     ), "STD_PROPERTIES"     );
    c->DirectoryPut(constDir, c->Int32(STD_HELP           ), "STD_HELP"           );
    c->DirectoryPut(constDir, c->Int32(STD_FIND           ), "STD_FIND"           );
    c->DirectoryPut(constDir, c->Int32(STD_REPLACE        ), "STD_REPLACE"        );
    c->DirectoryPut(constDir, c->Int32(STD_PRINT          ), "STD_PRINT"          );
    c->DirectoryPut(constDir, c->Int32(VIEW_LARGEICONS    ), "VIEW_LARGEICONS"    );
    c->DirectoryPut(constDir, c->Int32(VIEW_SMALLICONS    ), "VIEW_SMALLICONS"    );
    c->DirectoryPut(constDir, c->Int32(VIEW_LIST          ), "VIEW_LIST"          );
    c->DirectoryPut(constDir, c->Int32(VIEW_DETAILS       ), "VIEW_DETAILS"       );
    c->DirectoryPut(constDir, c->Int32(VIEW_SORTNAME      ), "VIEW_SORTNAME"      );
    c->DirectoryPut(constDir, c->Int32(VIEW_SORTSIZE      ), "VIEW_SORTSIZE"      );
    c->DirectoryPut(constDir, c->Int32(VIEW_SORTDATE      ), "VIEW_SORTDATE"      );
    c->DirectoryPut(constDir, c->Int32(VIEW_SORTTYPE      ), "VIEW_SORTTYPE"      );
    c->DirectoryPut(constDir, c->Int32(VIEW_PARENTFOLDER  ), "VIEW_PARENTFOLDER"  );
    c->DirectoryPut(constDir, c->Int32(VIEW_NETCONNECT    ), "VIEW_NETCONNECT"    );
    c->DirectoryPut(constDir, c->Int32(VIEW_NETDISCONNECT ), "VIEW_NETDISCONNECT" );
    c->DirectoryPut(constDir, c->Int32(VIEW_NEWFOLDER     ), "VIEW_NEWFOLDER"     );
    c->DirectoryPut(constDir, c->Int32(VIEW_VIEWMENU      ), "VIEW_VIEWMENU"      );
    c->DirectoryPut(constDir, c->Int32(HIST_BACK          ), "HIST_BACK"          );
    c->DirectoryPut(constDir, c->Int32(HIST_FORWARD       ), "HIST_FORWARD"       );
    c->DirectoryPut(constDir, c->Int32(HIST_FAVORITES     ), "HIST_FAVORITES"     );
    c->DirectoryPut(constDir, c->Int32(HIST_ADDTOFAVORITES), "HIST_ADDTOFAVORITES");
    c->DirectoryPut(constDir, c->Int32(HIST_VIEWTREE      ), "HIST_VIEWTREE"      );
    c->DirectoryPut(constDir, c->Int32(I_IMAGECALLBACK    ), "I_IMAGECALLBACK"    );
    c->DirectoryPut(constDir, c->Int32(I_IMAGENONE        ), "I_IMAGENONE"        );
}

/** ToolBar::addBitmap()
 *
 *  Adds one or more images to the list of button images available for a
 *  toolbar.
 *
 *  @param  src  [required] Source of the bitmap.  This can be a ResourceImage
 *               object, a bitmap .Image object, or the special keyword
 *               hInstCommCtrl, case not significant.
 *
 *  @param count [optional] The count / number of images in the bitmap.  If src
 *               is an Image or a ResourceImage, then count is required. If src
 *               is hInstCommCtrl, then count is ignored and can be omitted.
 *
 *  @param id    [optional] The resource ID for the image. id is optional and
 *               ignored if src is an Image object.  It is required for a
 *               ResourceImage or hInstCommCtrl.
 *
 *               For a ResourceImage, id may be numeric or symbolic.  To use a
 *               symbolic ID, the symbol must be in the global .constDir.
 *
 *               For hInstCommCtrl id must be one of the four following
 *               keywords, case is not significant: STDLARGE, STDSMALL,
 *               VIEWLARGE, or VIEWSMALL.
 *
 *  @return  The index of the first new image, or -1 on error.
 *
 *  @note  If an Image object is the source, do not release the image until
 *         after the dialog is ended, or the replaceBitmap() method has been
 *         used to replace the bitmap.  Otherwise the tool bar is destroyed.
 */
RexxMethod4(int32_t, tb_addBitmap, RexxObjectPtr, src, OPTIONAL_uint32_t, count, OPTIONAL_RexxObjectPtr, rxID,
            CSELF, pCSelf)
{
    RexxMethodContext *c = context;
    TBADDBITMAP tbab   = { 0 };
    int32_t     result = -1;

    if ( c->IsOfType(src, "IMAGE") )
    {
        if ( argumentOmitted(2) )
        {
            missingArgException(context, 2);
            goto done_out;
        }

        POODIMAGE poi = rxGetImageBitmap(context, src, 1);
        if ( poi == NULL )
        {
            goto done_out;
        }
        if ( ! poi->isValid )
        {
            nullObjectException(context->threadContext, "Image", 1);
            goto done_out;
        }
        tbab.nID = (UINT_PTR)poi->hImage;
    }
    else if ( c->IsOfType(src, "RESOURCEIMAGE") )
    {
        if ( argumentOmitted(3) )
        {
            missingArgException(context, 3);
        }

        PRESOURCEIMAGE pri = rxGetResourceImage(context, src, 1);
        if ( pri == NULL )
        {
            goto done_out;
        }
        if ( ! pri->isValid )
        {
            nullObjectException(context->threadContext, "ResourceImage", 1);
            goto done_out;
        }

        tbab.nID = oodGlobalID(context, rxID, 3, true);
        if ( tbab.nID == OOD_ID_EXCEPTION )
        {
            goto done_out;
        }
        tbab.hInst = pri->hMod;
    }
    else
    {
        CSTRING srcWord = c->ObjectToStringValue(src);
        if ( StrCmpI(srcWord, "HINSTCOMMCTRL") != 0 )
        {
            wrongArgValueException(context->threadContext, 1, ADD_BITMAP_SRC_OBJ, src);
            goto done_out;
        }

        if ( argumentOmitted(3) )
        {
            missingArgException(context, 3);
            goto done_out;
        }
        tbab.hInst = HINST_COMMCTRL;

        CSTRING keyword = c->ObjectToStringValue(rxID);

        tbab.nID = keyword2idb(context, keyword, 3);
        if ( tbab.nID == OOD_INVALID_ITEM_ID )
        {
            goto done_out;
        }
    }

    result = (int32_t)SendMessage(getDChCtrl(pCSelf), TB_ADDBITMAP, count, (LPARAM)&tbab);

done_out:
    return result;
}

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

/** ToolBar::loadImages()
 *
 *  Loads system-defined button images into a toolbar control's image list.
 *
 *  @param  [required]  A keyword specifying which system define images to load.
 *
 *  @return  A number ????
 */
RexxMethod2(int32_t, tb_loadImages, CSTRING, _index, CSELF, pCSelf)
{
    uint32_t index = keyword2idb(context, _index, 1);
    if ( index == OOD_INVALID_ITEM_ID )
    {
        return -1;
    }

    return (int32_t)SendMessage(getDChCtrl(pCSelf), TB_LOADIMAGES, index, (LPARAM)HINST_COMMCTRL);
}

/** ToolBar::setBitmapSize()
 *
 *  Sets the size, in pixels, of the bitmapped images to be added to this
 *  toolbar.
 *
 *  @param  size  [required]  The size, width and height, of the bitmap images.
 *           This can be specified in two forms.
 *
 *      Form 1:  arg 1 is a .Size object.
 *
 *      Form 2:  arg 1 is the width and arg2 is the height.
 *
 *  @return  True on success, false on error.
 *
 *  @note  The size can only be set before adding any bitmaps to the toolbar. If
 *         the program does not explicitly set the bitmap size, the size
 *         defaults to 16 by 15 pixels.
 *
 *  @remarks  Toolbar icons: 16x16, 24x24, 32x32. Note that toolbar icons are
 *            always flat, not 3D, even at the 32x32 size.  From MSDN.
 */
RexxMethod2(RexxObjectPtr, tb_setBitmapSize, ARGLIST, args, CSELF, pCSelf)
{
    RexxObjectPtr result = TheFalseObj;

    pCDialogControl pcdc = validateDCCSelf(context, pCSelf);
    if ( pcdc == NULL )
    {
        goto done_out;
    }
    HWND hwnd = pcdc->hCtrl;

    size_t sizeArray;
    size_t argsUsed;
    POINT  point;
    if ( ! getSizeFromArglist(context, args, &point, 1, 2, &sizeArray, &argsUsed) )
    {
        goto done_out;
    }

    // Check arg count against expected.
    if ( sizeArray > argsUsed )
    {
        tooManyArgsException(context->threadContext, argsUsed);
        goto done_out;
    }

    if ( SendMessage(hwnd, TB_SETBITMAPSIZE, 0, MAKELPARAM(point.x , point.y)) )
    {
        result = TheTrueObj;
    }

done_out:
    return result;
}

