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
 * oodShellObjects.cpp
 *
 * Contains ooDialog classes that are more Shell orientated.
 */
#include "ooDialog.hpp"     // Must be first, includes windows.h, commctrl.h, and oorexxapi.h

#include <shlobj.h>
#include <shobjidl.h>
#include <shlwapi.h>
#include <shellapi.h>
//#include <stdio.h>
#include "APICommon.hpp"
#include "oodCommon.hpp"
#include "oodShellObjects.hpp"


/**
 * Methods for the ooDialog .BrowseForFolder class.
 */
#define BROWSEFORFOLDER_CLASS  "BrowseForFolder"


static inline pCBrowseForFolder getSfbCSelf(RexxMethodContext *c, void * pCSelf)
{
    if ( pCSelf == NULL )
    {
        baseClassInitializationException(c);
    }
    return (pCBrowseForFolder)pCSelf;
}

/**
 * This is the Browse for Folder dialog call back function used by the
 * BrowseForFolder class to customize the appearance and behavior of the
 * standard dialog.
 */
int32_t CALLBACK BrowseCallbackProc(HWND hwnd, uint32_t uMsg, LPARAM lp, LPARAM pData)
{
    TCHAR szDir[MAX_PATH];
    pCBrowseForFolder pbff = NULL;

    switch ( uMsg )
    {
        case BFFM_INITIALIZED:
            pbff = (pCBrowseForFolder)pData;

            if ( pbff->dlgTitle != NULL )
            {
                SetWindowText(hwnd, pbff->dlgTitle);
            }
            if ( pbff->usePathForHint && pbff->startDir != NULL )
            {
                SetDlgItemText(hwnd, HINT_ID, pbff->startDir);
            }
            else if ( pbff->hint != NULL )
            {
                SetDlgItemText(hwnd, HINT_ID, pbff->hint);
            }

            // WParam should be TRUE when passing a path and should be FALSE
            // when passing a pidl.
            if ( pbff->startDir != NULL )
            {
                SendMessage(hwnd, BFFM_SETSELECTION, TRUE, (LPARAM)pbff->startDir);
            }
            break;

        case BFFM_SELCHANGED:
            pbff = (pCBrowseForFolder)pData;

            // Set the hint static control to the currently selected path if
            // wanted.
            if ( pbff->usePathForHint )
            {
                if ( SHGetPathFromIDList((LPITEMIDLIST)lp, szDir) )
                {
                    SetDlgItemText(hwnd, HINT_ID, szDir);
                }
            }
            break;
    }
    return 0;
}

static bool _PathIsFull(const char *path)
{
    if ( PathIsUNC(path) )
    {
        return true;
    }
    else if ( PathGetDriveNumber(path) != -1 )
    {
        return true;
    }
    return false;
}

uint32_t keyword2csidl(RexxMethodContext *c, CSTRING keyword, size_t argPos)
{
    uint32_t csidl = OOD_ID_EXCEPTION;

         if ( StrCmpI(keyword, "CSIDL_DESKTOP"                ) == 0 ) csidl = CSIDL_DESKTOP                ;
    else if ( StrCmpI(keyword, "CSIDL_INTERNET"               ) == 0 ) csidl = CSIDL_INTERNET               ;
    else if ( StrCmpI(keyword, "CSIDL_PROGRAMS"               ) == 0 ) csidl = CSIDL_PROGRAMS               ;
    else if ( StrCmpI(keyword, "CSIDL_CONTROLS"               ) == 0 ) csidl = CSIDL_CONTROLS               ;
    else if ( StrCmpI(keyword, "CSIDL_PRINTERS"               ) == 0 ) csidl = CSIDL_PRINTERS               ;
    else if ( StrCmpI(keyword, "CSIDL_PERSONAL"               ) == 0 ) csidl = CSIDL_PERSONAL               ;
    else if ( StrCmpI(keyword, "CSIDL_FAVORITES"              ) == 0 ) csidl = CSIDL_FAVORITES              ;
    else if ( StrCmpI(keyword, "CSIDL_STARTUP"                ) == 0 ) csidl = CSIDL_STARTUP                ;
    else if ( StrCmpI(keyword, "CSIDL_RECENT"                 ) == 0 ) csidl = CSIDL_RECENT                 ;
    else if ( StrCmpI(keyword, "CSIDL_SENDTO"                 ) == 0 ) csidl = CSIDL_SENDTO                 ;
    else if ( StrCmpI(keyword, "CSIDL_BITBUCKET"              ) == 0 ) csidl = CSIDL_BITBUCKET              ;
    else if ( StrCmpI(keyword, "CSIDL_STARTMENU"              ) == 0 ) csidl = CSIDL_STARTMENU              ;
    else if ( StrCmpI(keyword, "CSIDL_MYDOCUMENTS"            ) == 0 ) csidl = CSIDL_MYDOCUMENTS            ;
    else if ( StrCmpI(keyword, "CSIDL_MYMUSIC"                ) == 0 ) csidl = CSIDL_MYMUSIC                ;
    else if ( StrCmpI(keyword, "CSIDL_MYVIDEO"                ) == 0 ) csidl = CSIDL_MYVIDEO                ;
    else if ( StrCmpI(keyword, "CSIDL_DESKTOPDIRECTORY"       ) == 0 ) csidl = CSIDL_DESKTOPDIRECTORY       ;
    else if ( StrCmpI(keyword, "CSIDL_DRIVES"                 ) == 0 ) csidl = CSIDL_DRIVES                 ;
    else if ( StrCmpI(keyword, "CSIDL_NETWORK"                ) == 0 ) csidl = CSIDL_NETWORK                ;
    else if ( StrCmpI(keyword, "CSIDL_NETHOOD"                ) == 0 ) csidl = CSIDL_NETHOOD                ;
    else if ( StrCmpI(keyword, "CSIDL_FONTS"                  ) == 0 ) csidl = CSIDL_FONTS                  ;
    else if ( StrCmpI(keyword, "CSIDL_TEMPLATES"              ) == 0 ) csidl = CSIDL_TEMPLATES              ;
    else if ( StrCmpI(keyword, "CSIDL_COMMON_STARTMENU"       ) == 0 ) csidl = CSIDL_COMMON_STARTMENU       ;
    else if ( StrCmpI(keyword, "CSIDL_COMMON_PROGRAMS"        ) == 0 ) csidl = CSIDL_COMMON_PROGRAMS        ;
    else if ( StrCmpI(keyword, "CSIDL_COMMON_STARTUP"         ) == 0 ) csidl = CSIDL_COMMON_STARTUP         ;
    else if ( StrCmpI(keyword, "CSIDL_COMMON_DESKTOPDIRECTORY") == 0 ) csidl = CSIDL_COMMON_DESKTOPDIRECTORY;
    else if ( StrCmpI(keyword, "CSIDL_APPDATA"                ) == 0 ) csidl = CSIDL_APPDATA                ;
    else if ( StrCmpI(keyword, "CSIDL_PRINTHOOD"              ) == 0 ) csidl = CSIDL_PRINTHOOD              ;
    else if ( StrCmpI(keyword, "CSIDL_LOCAL_APPDATA"          ) == 0 ) csidl = CSIDL_LOCAL_APPDATA          ;
    else if ( StrCmpI(keyword, "CSIDL_ALTSTARTUP"             ) == 0 ) csidl = CSIDL_ALTSTARTUP             ;
    else if ( StrCmpI(keyword, "CSIDL_COMMON_ALTSTARTUP"      ) == 0 ) csidl = CSIDL_COMMON_ALTSTARTUP      ;
    else if ( StrCmpI(keyword, "CSIDL_COMMON_FAVORITES"       ) == 0 ) csidl = CSIDL_COMMON_FAVORITES       ;
    else if ( StrCmpI(keyword, "CSIDL_INTERNET_CACHE"         ) == 0 ) csidl = CSIDL_INTERNET_CACHE         ;
    else if ( StrCmpI(keyword, "CSIDL_COOKIES"                ) == 0 ) csidl = CSIDL_COOKIES                ;
    else if ( StrCmpI(keyword, "CSIDL_HISTORY"                ) == 0 ) csidl = CSIDL_HISTORY                ;
    else if ( StrCmpI(keyword, "CSIDL_COMMON_APPDATA"         ) == 0 ) csidl = CSIDL_COMMON_APPDATA         ;
    else if ( StrCmpI(keyword, "CSIDL_WINDOWS"                ) == 0 ) csidl = CSIDL_WINDOWS                ;
    else if ( StrCmpI(keyword, "CSIDL_SYSTEM"                 ) == 0 ) csidl = CSIDL_SYSTEM                 ;
    else if ( StrCmpI(keyword, "CSIDL_PROGRAM_FILES"          ) == 0 ) csidl = CSIDL_PROGRAM_FILES          ;
    else if ( StrCmpI(keyword, "CSIDL_MYPICTURES"             ) == 0 ) csidl = CSIDL_MYPICTURES             ;
    else if ( StrCmpI(keyword, "CSIDL_PROFILE"                ) == 0 ) csidl = CSIDL_PROFILE                ;
    else if ( StrCmpI(keyword, "CSIDL_SYSTEMX86"              ) == 0 ) csidl = CSIDL_SYSTEMX86              ;
    else if ( StrCmpI(keyword, "CSIDL_PROGRAM_FILESX86"       ) == 0 ) csidl = CSIDL_PROGRAM_FILESX86       ;
    else if ( StrCmpI(keyword, "CSIDL_PROGRAM_FILES_COMMON"   ) == 0 ) csidl = CSIDL_PROGRAM_FILES_COMMON   ;
    else if ( StrCmpI(keyword, "CSIDL_PROGRAM_FILES_COMMONX86") == 0 ) csidl = CSIDL_PROGRAM_FILES_COMMONX86;
    else if ( StrCmpI(keyword, "CSIDL_COMMON_TEMPLATES"       ) == 0 ) csidl = CSIDL_COMMON_TEMPLATES       ;
    else if ( StrCmpI(keyword, "CSIDL_COMMON_DOCUMENTS"       ) == 0 ) csidl = CSIDL_COMMON_DOCUMENTS       ;
    else if ( StrCmpI(keyword, "CSIDL_COMMON_ADMINTOOLS"      ) == 0 ) csidl = CSIDL_COMMON_ADMINTOOLS      ;
    else if ( StrCmpI(keyword, "CSIDL_ADMINTOOLS"             ) == 0 ) csidl = CSIDL_ADMINTOOLS             ;
    else if ( StrCmpI(keyword, "CSIDL_CONNECTIONS"            ) == 0 ) csidl = CSIDL_CONNECTIONS            ;
    else if ( StrCmpI(keyword, "CSIDL_COMMON_MUSIC"           ) == 0 ) csidl = CSIDL_COMMON_MUSIC           ;
    else if ( StrCmpI(keyword, "CSIDL_COMMON_PICTURES"        ) == 0 ) csidl = CSIDL_COMMON_PICTURES        ;
    else if ( StrCmpI(keyword, "CSIDL_COMMON_VIDEO"           ) == 0 ) csidl = CSIDL_COMMON_VIDEO           ;
    else if ( StrCmpI(keyword, "CSIDL_RESOURCES"              ) == 0 ) csidl = CSIDL_RESOURCES              ;
    else if ( StrCmpI(keyword, "CSIDL_RESOURCES_LOCALIZED"    ) == 0 ) csidl = CSIDL_RESOURCES_LOCALIZED    ;
    else if ( StrCmpI(keyword, "CSIDL_COMMON_OEM_LINKS"       ) == 0 ) csidl = CSIDL_COMMON_OEM_LINKS       ;
    else if ( StrCmpI(keyword, "CSIDL_CDBURN_AREA"            ) == 0 ) csidl = CSIDL_CDBURN_AREA            ;
    else if ( StrCmpI(keyword, "CSIDL_COMPUTERSNEARME"        ) == 0 ) csidl = CSIDL_COMPUTERSNEARME        ;
    else
    {
        invalidConstantException(c, argPos, INVALID_CONSTANT_MSG, "CSIDL", keyword);
    }

    return csidl;
}

LPCITEMIDLIST getRootPidl(RexxMethodContext *context)
{
    LPCITEMIDLIST pidl = NULL;
    RexxObjectPtr rxObject = context->GetObjectVariable("ROOT");
    if ( rxObject != context->Nil() )
    {
        pidl = (LPCITEMIDLIST)context->PointerValue((RexxPointerObject)rxObject);
    }
    return pidl;
}

/**
 * Sets one of the text attributes of the BrowseForFolder object.
 *
 * If rxValue is the .nil object or the empty string, then CSelf value of the
 * attribute is set to NULL, which basically causes the default value to be used
 * by the browse dialog.
 *
 * The attribute could already have a malloc'd string, in which case we need to
 * be sure and free it.
 *
 * @param c
 * @param pcbff
 * @param rxValue
 * @param type
 */
void setTextAttribute(RexxMethodContext *c, pCBrowseForFolder pcbff, RexxObjectPtr rxValue, BffAttributeType type)
{
    char    *newVal  = NULL;
    CSTRING  text    = NULL;
    size_t   len     = 0;

    if ( rxValue != TheNilObj )
    {
        text = c->ObjectToStringValue(rxValue);
        len  = strlen(text);
        if ( len == 0 )
        {
            text = NULL;
        }
    }

    if ( text != NULL )
    {
        newVal = (char *)LocalAlloc(LPTR, len + 1);
        if ( newVal == NULL )
        {
            outOfMemoryException(c->threadContext);
            return;
        }
        else
        {
            strcpy(newVal, text);
        }
    }

    switch ( type )
    {
        case DlgBanner :
            safeLocalFree(pcbff->banner);
            pcbff->banner = newVal;
            break;

        case DlgHint :
            safeLocalFree(pcbff->hint);
            pcbff->hint = newVal;
            break;

        case DlgStartDir :
            safeLocalFree(pcbff->startDir);
            pcbff->startDir = newVal;
            break;

        case DlgTitle :
            safeLocalFree(pcbff->dlgTitle);
            pcbff->dlgTitle = newVal;
            break;

        default :
            break;
    }
}

void freeRootPidl(RexxMethodContext *context)
{
    LPCITEMIDLIST pidl = getRootPidl(context);
    if ( pidl != NULL )
    {
        CoTaskMemFree((LPVOID)pidl);
    }
}

/**
 * Obtains a pointer to an item ID list from a path string.
 *
 * This function is used because it will work on W2K (and all the way back to
 * Windows 95.)
 *
 * @param path   Get the item ID list for this path.
 * @param ppidl  The item ID list is returned in this variable.
 *
 * @return True on success, otherwise false.
 *
 * @note   If this function fails, *ppidl will be null on return.  This function
 *         does not raise an exception, that is left to the caller.
 */
static bool pidlFromPath(LPCSTR path, LPITEMIDLIST *ppidl)
{
   LPSHELLFOLDER pShellFolder = NULL;
   HRESULT       hr;
   WCHAR         wPath[MAX_PATH];

   *ppidl = NULL;

   if ( MultiByteToWideChar(CP_ACP, 0, path, -1, wPath, MAX_PATH) == 0 )
   {
       return false;
   }

   // Need the desktop IShellFolder interface.
   if ( SHGetDesktopFolder(&pShellFolder) != NOERROR )
   {
       return false;
   }

   // convert the path to an ITEMIDLIST
   hr = pShellFolder->ParseDisplayName(NULL, NULL, wPath, NULL, ppidl, NULL);

   pShellFolder->Release();

   if (FAILED(hr))
   {
      *ppidl = NULL;  // Docs are unclear as to the value of this on failure.
      return false;
   }
   return true;
}

static bool pidlForSpecialFolder(uint32_t csidl, LPITEMIDLIST *ppidl)
{
    HRESULT  hr;
    bool     success = true;

    hr = SHGetFolderLocation(NULL, csidl, NULL, 0, ppidl);
    if ( FAILED(hr) )
    {
        #ifdef _DEBUG
            printf("Failed to get location hr: 0x%08x csidl: 0x%08x\n", hr, csidl);
        #endif

        *ppidl = NULL;
        success = false;
    }
    return success;
}

/**
 * Converts a string into a LPITEMIDLIST.  The string must either be one of the
 * CSIDL_xxx constants or a full path name.
 *
 * @param c
 * @param idl
 * @param argPos
 * @param canFail
 *
 * @return LPITEMIDLIST
 *
 * @remarks  Ife th string starts with CSIDL_, but is not a recognized, an
 *           exception is raised.
 */
LPITEMIDLIST getPidlFromString(RexxMethodContext *c, CSTRING idl, size_t argPos, bool canFail)
{
    LPITEMIDLIST pidl = NULL;

    // See if it looks like a CSIDL_ constant.
    if ( strlen(idl) > 6 && StrCmpNI("CSIDL_", idl, 6) == 0 )
    {
        uint32_t csidl = keyword2csidl(c, idl, argPos);

        if ( csidl == OOD_ID_EXCEPTION )
        {
            return NULL;
        }
        pidlForSpecialFolder(csidl, &pidl);
    }
    else if ( _PathIsFull(idl) )
    {
        pidlFromPath(idl, &pidl);
    }
    else
    {
        wrongArgValueException(c->threadContext, argPos, "a CSIDL_xxx keyword or a full path name", idl);
        return NULL;
    }

    if ( pidl == NULL && ! canFail )
    {
        systemServiceException(c->threadContext, NO_ITEMID_MSG, idl);
    }
    return pidl;
}

LPITEMIDLIST getPidlFromObject(RexxMethodContext *context, RexxObjectPtr obj, int argPos)
{
    LPITEMIDLIST pidl = NULL;

    if ( context->IsString(obj) )
    {
        pidl = getPidlFromString(context, context->StringData((RexxStringObject)obj), argPos, false);
    }
    else if ( context->IsPointer(obj) )
    {
        pidl = (LPITEMIDLIST)context->PointerValue((RexxPointerObject)obj);
    }

    if ( pidl == NULL )
    {
        wrongFormatException(context, argPos, obj);
    }
    return pidl;
}

/**
 * Puts up the Browse for Folder dialog using the supplied browse info
 * structure.
 *
 * @param pBI
 * @param returnPath
 *
 * @return RexxObjectPtr
 */
static RexxObjectPtr folderBrowse(RexxMethodContext *context, PBROWSEINFO pBI, bool returnPath)
{
	LPITEMIDLIST  pidl = NULL;
    TCHAR         path[MAX_PATH];
    RexxObjectPtr rxResult = context->NullString();    // User canceled.

    pidl = SHBrowseForFolder(pBI);
    if ( pidl != NULL )
    {
        if ( returnPath )
        {
            if ( SHGetPathFromIDList(pidl, path) )
            {
                rxResult = context->String(path);
            }
            else
            {
                rxResult = context->Nil();
            }
            CoTaskMemFree(pidl);
        }
        else
        {
            rxResult = context->NewPointer(pidl);
        }
    }
    return rxResult;
}

static void fillInBrowseInfo(RexxMethodContext *context, PBROWSEINFO pBI, pCBrowseForFolder pcbff)
{
    pBI->hwndOwner = pcbff->hOwner;
    pBI->ulFlags  = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_RETURNFSANCESTORS;

    if ( pcbff->hint != NULL )
    {
        pBI->ulFlags |= BIF_UAHINT;

        if ( StrCmpI("PATH", pcbff->hint) == 0 )
        {
            pcbff->usePathForHint = true;
            pcbff->hint           = BFF_HINT;
        }
    }

    if ( pcbff->banner != NULL )
    {
        pBI->lpszTitle = pcbff->banner;
    }

    pBI->pidlRoot = pcbff->root;

    pBI->lpfn   = BrowseCallbackProc;
    pBI->lParam = (LPARAM)pcbff;
}

/** BrowseForFolder::banner                  [attribute]
 */
RexxMethod1(RexxObjectPtr, bff_banner, CSELF, pCSelf)
{
    pCBrowseForFolder pcbff = (pCBrowseForFolder)getSfbCSelf(context, pCSelf);
    if ( pcbff != NULL && pcbff->banner != NULL )
    {
        return context->String(pcbff->banner);
    }
    return context->NullString();
}
RexxMethod2(RexxObjectPtr, bff_setBanner, RexxObjectPtr, hint, CSELF, pCSelf)
{
    pCBrowseForFolder pcbff = (pCBrowseForFolder)getSfbCSelf(context, pCSelf);
    if ( pcbff != NULL )
    {
        setTextAttribute(context, pcbff, hint, DlgHint);
    }
    return NULLOBJECT;
}

/** BrowseForFolder::dlgTitle                  [attribute]
 */
RexxMethod1(RexxObjectPtr, bff_dlgTitle, CSELF, pCSelf)
{
    pCBrowseForFolder pcbff = (pCBrowseForFolder)getSfbCSelf(context, pCSelf);
    if ( pcbff != NULL && pcbff->dlgTitle != NULL )
    {
        return context->String(pcbff->dlgTitle);
    }
    return context->NullString();
}
RexxMethod2(RexxObjectPtr, bff_setDlgTitle, RexxObjectPtr, dlgTitle, CSELF, pCSelf)
{
    pCBrowseForFolder pcbff = (pCBrowseForFolder)getSfbCSelf(context, pCSelf);
    if ( pcbff != NULL )
    {
        setTextAttribute(context, pcbff, dlgTitle, DlgTitle);
    }
    return NULLOBJECT;
}

/** BrowseForFolder::hint                  [attribute]
 */
RexxMethod1(RexxObjectPtr, bff_hint, CSELF, pCSelf)
{
    pCBrowseForFolder pcbff = (pCBrowseForFolder)getSfbCSelf(context, pCSelf);
    if ( pcbff != NULL && pcbff->hint != NULL )
    {
        return context->String(pcbff->hint);
    }
    return context->NullString();
}
RexxMethod2(RexxObjectPtr, bff_setHint, RexxObjectPtr, hint, CSELF, pCSelf)
{
    pCBrowseForFolder pcbff = (pCBrowseForFolder)getSfbCSelf(context, pCSelf);
    if ( pcbff != NULL )
    {
        setTextAttribute(context, pcbff, hint, DlgHint);
    }
    return NULLOBJECT;
}

/** BrowseForFolder::root                  [attribute]
 */
RexxMethod1(RexxObjectPtr, bff_root, CSELF, pCSelf)
{
    pCBrowseForFolder pcbff = (pCBrowseForFolder)getSfbCSelf(context, pCSelf);
    if ( pcbff != NULL )
    {
        return context->NullString();
        //return context->String(pcbff->root);  Really want to return display name for pidl
    }
    return context->NullString();
}
RexxMethod2(RexxObjectPtr, bff_setRoot, CSTRING, root, CSELF, pCSelf)
{
    pCBrowseForFolder pcbff = (pCBrowseForFolder)getSfbCSelf(context, pCSelf);
    if ( pcbff != NULL )
    {
        // setSfbTitle(context, pcbff, title);  TODO
    }
    return NULLOBJECT;
}

/** BrowseForFolder::startDir                  [attribute]
 */
RexxMethod1(RexxObjectPtr, bff_startDir, CSELF, pCSelf)
{
    pCBrowseForFolder pcbff = (pCBrowseForFolder)getSfbCSelf(context, pCSelf);
    if ( pcbff != NULL && pcbff->startDir != NULL )
    {
        return context->String(pcbff->startDir);
    }
    return context->NullString();
}
RexxMethod2(RexxObjectPtr, bff_setStartDir, RexxObjectPtr, startDir, CSELF, pCSelf)
{
    pCBrowseForFolder pcbff = (pCBrowseForFolder)getSfbCSelf(context, pCSelf);
    if ( pcbff != NULL )
    {
        setTextAttribute(context, pcbff, startDir, DlgStartDir);
    }
    return NULLOBJECT;
}

/** BrowseForFolder::uninit()
 *
 * Does clean up for this BrowseForFolder.  Frees the root PIDL and calls the
 * common uninit routine.
 */
RexxMethod1(RexxObjectPtr, bff_uninit, CSELF, pCSelf)
{
    pCBrowseForFolder pcbff = (pCBrowseForFolder)getSfbCSelf(context, pCSelf);
    if ( pcbff != NULL )
    {
        if ( pcbff->root != NULL )
        {
            CoTaskMemFree((LPVOID)pcbff->root);
        }

        safeLocalFree(pcbff->banner);
        safeLocalFree(pcbff->dlgTitle);
        safeLocalFree(pcbff->hint);
        safeLocalFree(pcbff->startDir);

        if ( pcbff->countCoInitialized > 0 )
        {
            do
            {
                CoUninitialize();
                pcbff->countCoInitialized--;
            } while (pcbff->countCoInitialized > 0);
        }
    }
    return NULLOBJECT;
}

/** BrowseForFolder::init()
 *
 * Initializes a BrowseForFolder object.  The title, banner, and hint strings
 * all have default values that are different from the Windows default values.
 * If the user wants to have the default Windows values, they specify that by
 * setting them to either .nil or the empty string.
 */
RexxMethod4(RexxObjectPtr, bff_init, OPTIONAL_RexxObjectPtr, title, OPTIONAL_RexxObjectPtr, banner, OPTIONAL_RexxObjectPtr, hint,
            OPTIONAL_RexxObjectPtr, startDir)
{
    RexxBufferObject obj = context->NewBuffer(sizeof(CBrowseForFolder));
    context->SetObjectVariable("CSELF", obj);

    pCBrowseForFolder pcbff = (pCBrowseForFolder)context->BufferData(obj);
    memset(pcbff, 0, sizeof(CBrowseForFolder));

    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    #ifdef _DEBUG
        printf("CoInitializeEx returns: 0x%08x\n", hr);
    #endif

    pcbff->countCoInitialized = 1;
    pcbff->needCoUninitialize = true;  // TODO probably don't need this field.

    // Set the default attributes for the browser.
    if ( argumentOmitted(1) )
    {
        title = context->String(BFF_TITLE);
    }
    setTextAttribute(context, pcbff, title, DlgTitle);

    if ( argumentOmitted(2) )
    {
        banner = context->String(BFF_BANNER);
    }
    setTextAttribute(context, pcbff, banner, DlgBanner);

    if ( argumentOmitted(3) )
    {
        hint = context->String(BFF_HINT);
    }
    setTextAttribute(context, pcbff, hint, DlgHint);

    if ( argumentOmitted(4) )
    {
        startDir = context->String(BFF_STARTDIR);
    }
    setTextAttribute(context, pcbff, startDir, DlgStartDir);

	LPITEMIDLIST pidl = NULL;
    if ( pidlForSpecialFolder(CSIDL_DRIVES, &pidl) )
    {
        pcbff->root = pidl;
    }
    else
    {
        systemServiceException(context->threadContext, NO_ITEMID_MSG, "CSIDL_DRIVES");
    }

    return NULLOBJECT;
}

// TODO temp name for setRoot
RexxMethod1(RexxObjectPtr, bff_getItemID, RexxObjectPtr, obj)
{
    RexxObjectPtr root = context->Nil();

    if ( context->Nil() == obj )
    {
        ;  // Do nothing, root is already set to Nil.
    }
    else if ( context->IsString(obj) )
    {
        if ( obj != context->NullString() )
        {
            LPITEMIDLIST pidl = getPidlFromString(context, context->StringData((RexxStringObject)obj), 1, false);
            if ( pidl == NULL )
            {
                return NULLOBJECT;
            }
            root = context->NewPointer(pidl);
        }
    }
    else if ( context->IsPointer(obj) )
    {
        if ( context->PointerValue((RexxPointerObject)obj) != NULL )
        {
            root = obj;
        }
    }
    else
    {
        wrongFormatException(context, 1, obj);
        return NULLOBJECT;
    }

    // Free an existing root pidl.  The function handles the .nil case.
    freeRootPidl(context);

    context->SetObjectVariable("ROOT", root);
    return NULLOBJECT;
}

/** BrowseForFolder::getFolder()
 *
 * Puts up the customized Browse for Folder dialog using the atributes set for
 * this instance.  The optional arg is a window handle.  If specified that
 * window is used as the owner of the dialog.
 *
 * @param   owner  OPTIONAL  A Rexx dialog to be used as owner of the browse
 *          dialog.  If this arg is used, but is not a valid PlainBaseDialog an
 *          exception is raised.
 *
 * @return  Returns the fully qualified path of the folder the user selects, or
 *          the empty string if the user cancels.  In addition, if the user
 *          selects a virtual folder that has no file system path, .nil is
 *          returned.
 */
RexxMethod2(RexxObjectPtr, bff_getFolder, OPTIONAL_RexxObjectPtr, owner, CSELF, pCSelf)
{
    pCBrowseForFolder pcbff = (pCBrowseForFolder)getSfbCSelf(context, pCSelf);
    if ( pcbff == NULL )
    {
        return NULLOBJECT;
    }

    if ( argumentExists(1) )
    {
        pCPlainBaseDialog pcpbd = requiredPlainBaseDlg(context, owner, 1);
        if ( pcpbd == NULL )
        {
            return NULLOBJECT;
        }

        pcbff->hOwner = pcpbd->hDlg;
    }

    BROWSEINFO   bi = { 0 };

    fillInBrowseInfo(context, &bi, pcbff);

    // Second arg is true - return path.
    return folderBrowse(context, &bi, true);
}


/** BrowseForFolder::getItemID()
 *
 * Put up the Browse for Folder dialog and return the PIDL the user selects.
 *
 * @param   hwnd  OPTIONAL  A window handle to be used as owner of the browse
 *          dialog.  If this arg is used, but is not a valid window handle, then
 *          an exception is raised.
 *
 * @return  Returns the pointer to item ID list the user selected, or the empty
 *          string if the user cancels the dialog.
 */
/*
RexxMethod2(RexxObjectPtr, bff_getItemID, OSELF, self, OPTIONAL_POINTER, owner)
{
    BROWSEINFO   bi = { 0 };
    BROWSE_DATA  bd = { 0 };

    if ( ! checkOptionalWindow(context, (HWND)owner, 1) )
    {
        return NULLOBJECT;
    }

    // NULL for owner works fine.
    fillInBrowseInfo(context, (HWND)owner, &bi, &bd);

    // Second arg is false - return PIDL, not path.
    return folderBrowse(context, &bi, false);
}
*/


/**
 * Example code for GetDisplayName() using SHCreateItemFromIDList to create a
 * IShellItem from a IDL.   Worked Windows 7 64 bit
 *
 * On XP, having SHCreateItemFromIDlist in the code prevents shell.dll from
 * being loaded, which prevents oodialog.dll from being loaded.  This can be
 * worked around using GetProcAddress().  But, that would be cumbersome if using
 * a number of the 'Vista or later' shell APIs.
 */
RexxMethod1(RexxObjectPtr, bff_test, CSELF, pCSelf)
{
    pCBrowseForFolder pcbff = (pCBrowseForFolder)getSfbCSelf(context, pCSelf);
    if ( pcbff == NULL )
    {
        return NULLOBJECT;
    }

    if ( ! requiredOS(context, "bff_test SHCreateItemFromIDList()", "Vista", Vista_OS) )
    {
        return NULLOBJECT;
    }

    PIDLIST_ABSOLUTE pidlSystem;

    HINSTANCE hinst = LoadLibrary(TEXT("Shell32.dll"));
    if ( hinst == NULL )
    {
        printf("LoadLibrary() failed\n");
        return NULLOBJECT;
    }
    SHCreateItemFromIDListPROC pSHCreateItemFromIDList;

    pSHCreateItemFromIDList = (SHCreateItemFromIDListPROC)GetProcAddress(hinst, "SHCreateItemFromIDList");
    if ( pSHCreateItemFromIDList == NULL )
    {
        printf("GetProcAddress() failed\n");
        return NULLOBJECT;
    }

    // simpler version using IShellItem
    HRESULT hr = SHGetFolderLocation(NULL, CSIDL_SYSTEM, NULL, NULL, &pidlSystem);
    if ( SUCCEEDED(hr) )
    {
        IShellItem *psi;

        //hr = SHCreateItemFromIDList(pidlSystem, IID_PPV_ARGS(&psi));
        hr = pSHCreateItemFromIDList(pidlSystem, IID_PPV_ARGS(&psi));
        if ( SUCCEEDED(hr) )
        {
            PWSTR pszName;
            hr = psi->GetDisplayName(SIGDN_NORMALDISPLAY, &pszName);
            if (SUCCEEDED(hr))
            {
                wprintf(L"Normal Display - %s\n", pszName);
                CoTaskMemFree(pszName);
            }
            psi->Release();
        }

        ILFree(pidlSystem);
    }

    return NULLOBJECT;
}


/**
 * Trial code IFileDialog used to browse for folder.  Works Win 7 64 bit.
 *
 * Fails on XP because CLSID_FileOpenDialog is not available.  A simple
 * requiredOS() fixes that problem.
 *
 */
#if 0
RexxMethod1(RexxObjectPtr, bff_test, CSELF, pCSelf)
{
    pCBrowseForFolder pcbff = (pCBrowseForFolder)getSfbCSelf(context, pCSelf);
    if ( pcbff == NULL )
    {
        return NULLOBJECT;
    }
    printf("bff_test() enter\n");

    if ( ! requiredOS(context, Vista_OS, "bff_test CLSID_FileOpenDialog", "Vista") )
    {
        return NULLOBJECT;
    }

    IFileDialog *pfd;

    HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pfd));
    if (SUCCEEDED(hr))
    {
        DWORD dwOptions;
        if (SUCCEEDED(pfd->GetOptions(&dwOptions)))
        {
            pfd->SetOptions(dwOptions | FOS_PICKFOLDERS);
        }
        if (SUCCEEDED(pfd->Show(NULL)))
        {
            IShellItem *psi;
            if (SUCCEEDED(pfd->GetResult(&psi)))
            {
                PWSTR pszPath;

                if(!SUCCEEDED(psi->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, &pszPath)))
                {
                    MessageBox(NULL, "GetIDListName() failed", NULL, NULL);
                }
                else
                {
                    wprintf(L"Got folder - %s\n", pszPath);
                    CoTaskMemFree(pszPath);
                }
                psi->Release();
            }
        }
        pfd->Release();
    }
    else
    {
        printf("CoCreateInstance returns: 0x%08x\n", hr);
    }

    return NULLOBJECT;
}
#endif


/**
 * Methods for the ooDialog .SimpleFolderBrowse class.
 */
#define SIMPLEFOLDERBROWSE_CLASS  "SimpleFolderBrowse"


/** SimpleFolderBrowse::getFolder()  [class method]
 *
 * SimpleFolderBrowse has only 1 method, getFolder().  The method is a class
 * method and would be used like so:
 *
 * folder = .SimplerFolderBrowse(...)
 * say 'User picked:' folder
 *
 * The arguments to the method can set non-default values for the dialog title,
 * dialog banner, dialog hint, dialog start directory, and root of the tree.
 * With no arguments the default SHBrowseForFolder is shown.
 *
 */
RexxMethod6(RexxObjectPtr, sfb_getFolder, OPTIONAL_CSTRING, title, OPTIONAL_CSTRING, banner, OPTIONAL_CSTRING, hint,
            OPTIONAL_CSTRING, startDir, OPTIONAL_CSTRING, root, OPTIONAL_RexxObjectPtr, owner)
{
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    if ( FAILED(hr) )
    {
        systemServiceExceptionComCode(context->threadContext, COM_API_FAILED_MSG, "CoInitializeEx", hr);
        return NULLOBJECT;
    }

    pCBrowseForFolder pcbff  = NULL;
    RexxObjectPtr     result = TheNilObj;
    BROWSEINFO        bi     = { 0 };

    if ( argumentExists(1) || argumentExists(3) || argumentExists(4) )
    {
        pcbff = (pCBrowseForFolder)LocalAlloc(LPTR, sizeof(CBrowseForFolder));
        if ( pcbff == NULL )
        {
            outOfMemoryException(context->threadContext);
            goto done_out;
        }

        bi.lParam = (LPARAM)pcbff;
        bi.lpfn   = BrowseCallbackProc;
    }

    bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_RETURNFSANCESTORS;

    if ( argumentExists(1) )
    {
        if ( *title != '\0' )
        {
            pcbff->dlgTitle = (char *)title;
        }
    }

    if ( argumentExists(2) )
    {
        if ( *banner != '\0' )
        {
            bi.lpszTitle = title;
        }
    }

    if ( argumentExists(3) )
    {
        bi.ulFlags |= BIF_UAHINT;

        if ( StrCmpI("PATH", hint) == 0 )
        {
            pcbff->usePathForHint = true;
            pcbff->hint           = "";
        }
        else
        {
            pcbff->hint = (char *)hint;
        }
    }

    if ( argumentExists(4) )
    {
        if ( *startDir != '\0' )
        {
            pcbff->startDir = (char *)startDir;
        }
    }

    if ( argumentExists(5) )
    {
        bi.pidlRoot = getPidlFromString(context, root, 5, false);
        if ( bi.pidlRoot == NULL )
        {
            goto done_out;
        }
    }

    if ( argumentExists(6) )
    {
        pCPlainBaseDialog pcpbd = requiredPlainBaseDlg(context, owner, 1);
        if ( pcpbd == NULL )
        {
            goto done_out;
        }

        bi.hwndOwner = pcpbd->hDlg;
    }

    // If the user only sent empty strings as arguments, we do not need the
    // call back function
    if ( pcbff && pcbff->dlgTitle == NULL && pcbff->hint == NULL && pcbff->startDir == NULL )
    {
        bi.lParam = NULL;
        bi.lpfn   = NULL;
    }

     result = folderBrowse(context, &bi, true);

done_out:
    if ( bi.pidlRoot != NULL )
    {

        CoTaskMemFree((LPVOID)bi.pidlRoot);
    }
    safeLocalFree(pcbff);
    CoUninitialize();

    return result;
}

