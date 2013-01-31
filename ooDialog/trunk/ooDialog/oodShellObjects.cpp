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
#include <shlwapi.h>
#include "APICommon.hpp"
#include "oodCommon.hpp"
#include "oodShellObjects.hpp"


/**
 * Methods for the ooDialog .BrowseForFolder class.
 */
#define BROWSEFORFOLDER_CLASS  "BrowseForFolder"

/**
 * Ensures the BrowseForFolder CSelf pointer is not null.
 *
 * @param c
 * @param pCSelf
 *
 * @return pCBrowseForFolder
 */
static pCBrowseForFolder getBffCSelf(RexxMethodContext *c, void * pCSelf)
{
    pCBrowseForFolder pcbff = (pCBrowseForFolder)pCSelf;
    if ( pcbff == NULL )
    {
        baseClassInitializationException(c);
    }
    return pcbff;
}

/**
 * This is the Browse for Folder dialog call back function used by the
 * BrowseForFolder and SimpleFolderBrowse classes to customize the appearance
 * and behavior of the standard dialog.
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
            if ( pbff->useHint )
            {
                if ( pbff->usePathForHint && pbff->startDir != NULL )
                {
                    SetDlgItemText(hwnd, HINT_ID, pbff->startDir);
                }
                else if ( pbff->hint != NULL )
                {
                    SetDlgItemText(hwnd, HINT_ID, pbff->hint);
                }
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

/**
 * Uses CoUninitialize() to uninit COM, when executing on the same thread as the
 * BrowseForFolder object was instantiated on.
 *
 * @param pcbff
 */
static void uninitCom(pCBrowseForFolder pcbff)
{
    if ( pcbff->countCoInitialized > 0 && pcbff->coThreadID == GetCurrentThreadId() )
    {
        do
        {
#ifdef _DEBUG
    printf("uninitCom() thread ID=%d\n", GetCurrentThreadId());
#endif
            CoUninitialize();
            pcbff->countCoInitialized--;
        } while ( pcbff->countCoInitialized > 0 );
    }
}

/**
 * Checks that a path is a full path name and not a relative path name.  We
 * define full path name here as a UNC path or a path that includes the drive
 * letter and colon.
 *
 * @param path
 *
 * @return bool
 */
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

/**
 * Converts a string of keywords to its CSIDL_xx value. Raises an exception if
 * keyword is not valid.
 *
 * @param c
 * @param keyword
 * @param argPos
 *
 * @return uint32_t
 */
static uint32_t keyword2csidl(RexxMethodContext *c, CSTRING keyword, size_t argPos)
{
    uint32_t csidl = OOD_ID_EXCEPTION;

         if ( StrStrI(keyword, "CSIDL_DESKTOP"                ) != NULL ) csidl = CSIDL_DESKTOP                ;
    else if ( StrStrI(keyword, "CSIDL_INTERNET"               ) != NULL ) csidl = CSIDL_INTERNET               ;
    else if ( StrStrI(keyword, "CSIDL_PROGRAMS"               ) != NULL ) csidl = CSIDL_PROGRAMS               ;
    else if ( StrStrI(keyword, "CSIDL_CONTROLS"               ) != NULL ) csidl = CSIDL_CONTROLS               ;
    else if ( StrStrI(keyword, "CSIDL_PRINTERS"               ) != NULL ) csidl = CSIDL_PRINTERS               ;
    else if ( StrStrI(keyword, "CSIDL_PERSONAL"               ) != NULL ) csidl = CSIDL_PERSONAL               ;
    else if ( StrStrI(keyword, "CSIDL_FAVORITES"              ) != NULL ) csidl = CSIDL_FAVORITES              ;
    else if ( StrStrI(keyword, "CSIDL_STARTUP"                ) != NULL ) csidl = CSIDL_STARTUP                ;
    else if ( StrStrI(keyword, "CSIDL_RECENT"                 ) != NULL ) csidl = CSIDL_RECENT                 ;
    else if ( StrStrI(keyword, "CSIDL_SENDTO"                 ) != NULL ) csidl = CSIDL_SENDTO                 ;
    else if ( StrStrI(keyword, "CSIDL_BITBUCKET"              ) != NULL ) csidl = CSIDL_BITBUCKET              ;
    else if ( StrStrI(keyword, "CSIDL_STARTMENU"              ) != NULL ) csidl = CSIDL_STARTMENU              ;
    else if ( StrStrI(keyword, "CSIDL_MYDOCUMENTS"            ) != NULL ) csidl = CSIDL_MYDOCUMENTS            ;
    else if ( StrStrI(keyword, "CSIDL_MYMUSIC"                ) != NULL ) csidl = CSIDL_MYMUSIC                ;
    else if ( StrStrI(keyword, "CSIDL_MYVIDEO"                ) != NULL ) csidl = CSIDL_MYVIDEO                ;
    else if ( StrStrI(keyword, "CSIDL_DESKTOPDIRECTORY"       ) != NULL ) csidl = CSIDL_DESKTOPDIRECTORY       ;
    else if ( StrStrI(keyword, "CSIDL_DRIVES"                 ) != NULL ) csidl = CSIDL_DRIVES                 ;
    else if ( StrStrI(keyword, "CSIDL_NETWORK"                ) != NULL ) csidl = CSIDL_NETWORK                ;
    else if ( StrStrI(keyword, "CSIDL_NETHOOD"                ) != NULL ) csidl = CSIDL_NETHOOD                ;
    else if ( StrStrI(keyword, "CSIDL_FONTS"                  ) != NULL ) csidl = CSIDL_FONTS                  ;
    else if ( StrStrI(keyword, "CSIDL_TEMPLATES"              ) != NULL ) csidl = CSIDL_TEMPLATES              ;
    else if ( StrStrI(keyword, "CSIDL_COMMON_STARTMENU"       ) != NULL ) csidl = CSIDL_COMMON_STARTMENU       ;
    else if ( StrStrI(keyword, "CSIDL_COMMON_PROGRAMS"        ) != NULL ) csidl = CSIDL_COMMON_PROGRAMS        ;
    else if ( StrStrI(keyword, "CSIDL_COMMON_STARTUP"         ) != NULL ) csidl = CSIDL_COMMON_STARTUP         ;
    else if ( StrStrI(keyword, "CSIDL_COMMON_DESKTOPDIRECTORY") != NULL ) csidl = CSIDL_COMMON_DESKTOPDIRECTORY;
    else if ( StrStrI(keyword, "CSIDL_APPDATA"                ) != NULL ) csidl = CSIDL_APPDATA                ;
    else if ( StrStrI(keyword, "CSIDL_PRINTHOOD"              ) != NULL ) csidl = CSIDL_PRINTHOOD              ;
    else if ( StrStrI(keyword, "CSIDL_LOCAL_APPDATA"          ) != NULL ) csidl = CSIDL_LOCAL_APPDATA          ;
    else if ( StrStrI(keyword, "CSIDL_ALTSTARTUP"             ) != NULL ) csidl = CSIDL_ALTSTARTUP             ;
    else if ( StrStrI(keyword, "CSIDL_COMMON_ALTSTARTUP"      ) != NULL ) csidl = CSIDL_COMMON_ALTSTARTUP      ;
    else if ( StrStrI(keyword, "CSIDL_COMMON_FAVORITES"       ) != NULL ) csidl = CSIDL_COMMON_FAVORITES       ;
    else if ( StrStrI(keyword, "CSIDL_INTERNET_CACHE"         ) != NULL ) csidl = CSIDL_INTERNET_CACHE         ;
    else if ( StrStrI(keyword, "CSIDL_COOKIES"                ) != NULL ) csidl = CSIDL_COOKIES                ;
    else if ( StrStrI(keyword, "CSIDL_HISTORY"                ) != NULL ) csidl = CSIDL_HISTORY                ;
    else if ( StrStrI(keyword, "CSIDL_COMMON_APPDATA"         ) != NULL ) csidl = CSIDL_COMMON_APPDATA         ;
    else if ( StrStrI(keyword, "CSIDL_WINDOWS"                ) != NULL ) csidl = CSIDL_WINDOWS                ;
    else if ( StrStrI(keyword, "CSIDL_SYSTEM"                 ) != NULL ) csidl = CSIDL_SYSTEM                 ;
    else if ( StrStrI(keyword, "CSIDL_PROGRAM_FILES"          ) != NULL ) csidl = CSIDL_PROGRAM_FILES          ;
    else if ( StrStrI(keyword, "CSIDL_MYPICTURES"             ) != NULL ) csidl = CSIDL_MYPICTURES             ;
    else if ( StrStrI(keyword, "CSIDL_PROFILE"                ) != NULL ) csidl = CSIDL_PROFILE                ;
    else if ( StrStrI(keyword, "CSIDL_SYSTEMX86"              ) != NULL ) csidl = CSIDL_SYSTEMX86              ;
    else if ( StrStrI(keyword, "CSIDL_PROGRAM_FILESX86"       ) != NULL ) csidl = CSIDL_PROGRAM_FILESX86       ;
    else if ( StrStrI(keyword, "CSIDL_PROGRAM_FILES_COMMON"   ) != NULL ) csidl = CSIDL_PROGRAM_FILES_COMMON   ;
    else if ( StrStrI(keyword, "CSIDL_PROGRAM_FILES_COMMONX86") != NULL ) csidl = CSIDL_PROGRAM_FILES_COMMONX86;
    else if ( StrStrI(keyword, "CSIDL_COMMON_TEMPLATES"       ) != NULL ) csidl = CSIDL_COMMON_TEMPLATES       ;
    else if ( StrStrI(keyword, "CSIDL_COMMON_DOCUMENTS"       ) != NULL ) csidl = CSIDL_COMMON_DOCUMENTS       ;
    else if ( StrStrI(keyword, "CSIDL_COMMON_ADMINTOOLS"      ) != NULL ) csidl = CSIDL_COMMON_ADMINTOOLS      ;
    else if ( StrStrI(keyword, "CSIDL_ADMINTOOLS"             ) != NULL ) csidl = CSIDL_ADMINTOOLS             ;
    else if ( StrStrI(keyword, "CSIDL_CONNECTIONS"            ) != NULL ) csidl = CSIDL_CONNECTIONS            ;
    else if ( StrStrI(keyword, "CSIDL_COMMON_MUSIC"           ) != NULL ) csidl = CSIDL_COMMON_MUSIC           ;
    else if ( StrStrI(keyword, "CSIDL_COMMON_PICTURES"        ) != NULL ) csidl = CSIDL_COMMON_PICTURES        ;
    else if ( StrStrI(keyword, "CSIDL_COMMON_VIDEO"           ) != NULL ) csidl = CSIDL_COMMON_VIDEO           ;
    else if ( StrStrI(keyword, "CSIDL_RESOURCES"              ) != NULL ) csidl = CSIDL_RESOURCES              ;
    else if ( StrStrI(keyword, "CSIDL_RESOURCES_LOCALIZED"    ) != NULL ) csidl = CSIDL_RESOURCES_LOCALIZED    ;
    else if ( StrStrI(keyword, "CSIDL_COMMON_OEM_LINKS"       ) != NULL ) csidl = CSIDL_COMMON_OEM_LINKS       ;
    else if ( StrStrI(keyword, "CSIDL_CDBURN_AREA"            ) != NULL ) csidl = CSIDL_CDBURN_AREA            ;
    else if ( StrStrI(keyword, "CSIDL_COMPUTERSNEARME"        ) != NULL ) csidl = CSIDL_COMPUTERSNEARME        ;
    else
    {
        invalidConstantException(c, argPos, INVALID_CONSTANT_MSG, "CSIDL", keyword);
    }

    return csidl;
}

/**
 * Converts a string of keywords to its BIF_xx value. Raises an exception if
 * keyword is not valid.
 *
 * @param c
 * @param keyword
 * @param argPos
 *
 * @return uint32_t
 */
static uint32_t keywords2bif(RexxMethodContext *c, CSTRING keyword, size_t argPos)
{
    uint32_t bif = OOD_ID_EXCEPTION;

         if ( StrStrI(keyword, "BROWSEFILEJUNCTIONS") != NULL ) bif = BIF_BROWSEFILEJUNCTIONS;
    else if ( StrStrI(keyword, "BROWSEFORCOMPUTER"  ) != NULL ) bif = BIF_BROWSEFORCOMPUTER  ;
    else if ( StrStrI(keyword, "BROWSEFORPRINTER"   ) != NULL ) bif = BIF_BROWSEFORPRINTER   ;
    else if ( StrStrI(keyword, "BROWSEINCLUDEFILES" ) != NULL ) bif = BIF_BROWSEINCLUDEFILES ;
    else if ( StrStrI(keyword, "BROWSEINCLUDEURLS"  ) != NULL ) bif = BIF_BROWSEINCLUDEURLS  ;
    else if ( StrStrI(keyword, "DONTGOBELOWDOMAIN"  ) != NULL ) bif = BIF_DONTGOBELOWDOMAIN  ;
    else if ( StrStrI(keyword, "NEWDIALOGSTYLE"     ) != NULL ) bif = BIF_NEWDIALOGSTYLE     ;
    else if ( StrStrI(keyword, "NONEWFOLDERBUTTON"  ) != NULL ) bif = BIF_NONEWFOLDERBUTTON  ;
    else if ( StrStrI(keyword, "NOTRANSLATETARGETS" ) != NULL ) bif = BIF_NOTRANSLATETARGETS ;
    else if ( StrStrI(keyword, "RETURNFSANCESTORS"  ) != NULL ) bif = BIF_RETURNFSANCESTORS  ;
    else if ( StrStrI(keyword, "RETURNONLYFSDIRS"   ) != NULL ) bif = BIF_RETURNONLYFSDIRS   ;
    else if ( StrStrI(keyword, "SHAREABLE"          ) != NULL ) bif = BIF_SHAREABLE          ;
    else if ( StrStrI(keyword, "STATUSTEXT"         ) != NULL ) bif = BIF_STATUSTEXT         ;
    else if ( StrStrI(keyword, "UAHINT"             ) != NULL ) bif = BIF_UAHINT             ;
    else if ( StrStrI(keyword, "USENEWUI"           ) != NULL ) bif = BIF_USENEWUI           ;
    else
    {
        invalidConstantException(c, argPos, INVALID_CONSTANT_MSG, "BIF", keyword);
    }

    return bif;
}

/**
 * Parse the BIF_xx flags and returns the matching keyword string.
 *
 * @param c
 * @param bif
 *
 * @return RexxObjectPtr
 */
static RexxObjectPtr bif2keywords(RexxMethodContext *c, uint32_t bif)
{
    char buf[512] = { '\0' };

    if ( bif & BIF_BROWSEFILEJUNCTIONS) strcat(buf, "BROWSEFILEJUNCTIONS ");
    if ( bif & BIF_BROWSEFORCOMPUTER  ) strcat(buf, "BROWSEFORCOMPUTER "  );
    if ( bif & BIF_BROWSEFORPRINTER   ) strcat(buf, "BROWSEFORPRINTER "   );
    if ( bif & BIF_BROWSEINCLUDEFILES ) strcat(buf, "BROWSEINCLUDEFILES " );
    if ( bif & BIF_BROWSEINCLUDEURLS  ) strcat(buf, "BROWSEINCLUDEURLS "  );
    if ( bif & BIF_DONTGOBELOWDOMAIN  ) strcat(buf, "DONTGOBELOWDOMAIN "  );
    if ( bif & BIF_NEWDIALOGSTYLE     ) strcat(buf, "NEWDIALOGSTYLE "     );
    if ( bif & BIF_NONEWFOLDERBUTTON  ) strcat(buf, "NONEWFOLDERBUTTON "  );
    if ( bif & BIF_NOTRANSLATETARGETS ) strcat(buf, "NOTRANSLATETARGETS " );
    if ( bif & BIF_RETURNFSANCESTORS  ) strcat(buf, "RETURNFSANCESTORS "  );
    if ( bif & BIF_RETURNONLYFSDIRS   ) strcat(buf, "RETURNONLYFSDIRS "   );
    if ( bif & BIF_SHAREABLE          ) strcat(buf, "SHAREABLE "          );
    if ( bif & BIF_STATUSTEXT         ) strcat(buf, "STATUSTEXT "         );
    if ( bif & BIF_UAHINT             ) strcat(buf, "UAHINT "             );
    if ( bif & BIF_USENEWUI           ) strcat(buf, "USENEWUI "           );

    if ( buf[0] != '\0' )
    {
        buf[strlen(buf) - 1] = '\0';
    }
    return c->String(buf);
}

/**
 * Sets one of the text attributes of the BrowseForFolder object.
 *
 * If rxValue is the .nil object or the empty string, then the CSelf value of
 * the attribute is set to NULL, which basically causes the default value to be
 * used by the browse dialog.
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
            if ( newVal != NULL )
            {
                pcbff->useHint = true;
            }
            else
            {
                pcbff->useHint = pcbff->usePathForHint ? true : false;
            }

            if ( pcbff->useHint )
            {
                pcbff->bifFlags |= BIF_UAHINT;
            }
            else
            {
                pcbff->bifFlags &= ~BIF_UAHINT;
            }
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

   if ( MultiByteToWideChar(CP_ACP, 0, path, -1, wPath, MAX_PATH) != NULL )
   {
       return false;
   }

   // Need the desktop IShellFolder interface.
   if ( SHGetDesktopFolder(&pShellFolder) != S_OK )
   {
       return false;
   }

   // convert the path to an ITEMIDLIST
   hr = pShellFolder->ParseDisplayName(NULL, NULL, wPath, NULL, ppidl, NULL);

   pShellFolder->Release();

   if ( FAILED(hr) )
   {
      *ppidl = NULL;  // Docs are unclear as to the value of this on failure.
      return false;
   }
   return true;
}

/**
 * Takes a CSIDL_xxx constant and returns its item ID list pointer.
 *
 * @param csidl
 * @param ppidl
 *
 * @return bool
 */
static bool pidlForSpecialFolder(uint32_t csidl, LPITEMIDLIST *ppidl)
{
    HRESULT  hr;
    bool     success = true;

    hr = SHGetFolderLocation(NULL, csidl, NULL, 0, ppidl);
    if ( FAILED(hr) )
    {
        *ppidl = NULL;
        success = false;
    }
    return success;
}


/**
 * Invokes CoUninitialize() once on the current thread. However, if the current
 * thread is the same thread as instantiation of the BrowseForFolder object,
 * then the inovocation will be dependent on the state flags.
 *
 * @param pcbff
 *
 * @return True if CoUnitialize() was invoked on the original thread of
 *         instantiation, otherwise false.
 */
static RexxObjectPtr releaseCOM(pCBrowseForFolder pcbff)
{
    if ( GetCurrentThreadId() == pcbff->coThreadID )
    {
        uninitCom(pcbff);
        return TheTrueObj;
    }
    else
    {
        CoUninitialize();
        return TheFalseObj;
    }
}


/**
 * Handles a CoInitializeEx() request on the same thread as the BrowseForFolder
 * object was created on.
 *
 * @param c
 * @param pcbff
 *
 * @return True for a successful CoInitializeEx() otherwise false.
 *
 * @assumes That the caller has already checked pcbff->coThreadID and we are in
 *          fact in the same thread.
 */
static RexxObjectPtr reInitCOM(RexxMethodContext *c, pCBrowseForFolder pcbff)
{
    if ( pcbff->countCoInitialized > 0 )
    {
        return TheFalseObj;
    }

    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    if ( hr == S_OK )
    {
        pcbff->countCoInitialized++;
        return TheTrueObj;
    }
    else if ( hr == S_FALSE )
    {
        // This should be impossible.
        CoUninitialize();
    }
    return TheFalseObj;
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
        wrongArgValueException(c->threadContext, argPos, WRONG_IDL_TYPE_LIST_SHORT, idl);
        return NULL;
    }

    if ( pidl == NULL && ! canFail )
    {
        systemServiceException(c->threadContext, NO_ITEMID_MSG, idl);
    }
    return pidl;
}

/**
 * Extracts a pointer to an item ID list from a Rexx object sent as arg to a
 * method.  The object must resolve to a CSIDL_xxx constant, a full path, or a
 * Pointer object.
 *
 * Not used for now, not sure that we need this.
 *
 * @param c
 * @param obj
 * @param argPos
 *
 * @return LPITEMIDLIST
 */
LPITEMIDLIST getPidlFromObject(RexxMethodContext *c, RexxObjectPtr obj, size_t argPos)
{
    LPITEMIDLIST pidl = NULL;

    if ( c->IsString(obj) )
    {
        CSTRING idl = c->ObjectToStringValue(obj);
        pidl = getPidlFromString(c, idl, argPos, false);
    }
    else if ( c->IsPointer(obj) )
    {
        pidl = (LPITEMIDLIST)c->PointerValue((RexxPointerObject)obj);
        if ( pidl == NULL )
        {
            nullObjectException(c->threadContext, "pointer to an Item ID List", argPos);
        }
    }
    else
    {
        wrongArgValueException(c->threadContext, argPos, WRONG_IDL_TYPE_LIST, obj);
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

    if ( ! returnPath && rxResult == context->NullString() )
    {
        rxResult = TheNilObj;
    }
    return rxResult;
}

/**
 * Fills in the browse info struct using the attributes of this BrowseForFolder
 * object.
 *
 * @param context
 * @param pBI
 * @param pcbff
 *
 * @remarks  We treat a few things about the BIF_xx flags special here.  The
 *           user can change the flags to what they want, but we always use
 *           BIF_NEWDIALOGSTYLE.  So, we always add it here as a precaution in
 *           case it was dropped.  A similar thing for BIF_UAHINT, if useHint is
 *           true, we always add it.
 *
 *           In the code we try to keep bifFlags accurate so that if the user
 *           accesses the attribute, they get the right info.  But, the logic is
 *           a little convoluted ... so we take a little extra precaution.
 */
static void fillInBrowseInfo(RexxMethodContext *context, PBROWSEINFO pBI, pCBrowseForFolder pcbff)
{
    pBI->hwndOwner = pcbff->hOwner;
    pBI->ulFlags   = pcbff->bifFlags;

    pBI->ulFlags |= BIF_NEWDIALOGSTYLE;

    if ( pcbff->useHint )
    {
        pBI->ulFlags |= BIF_UAHINT;
    }

    if ( pcbff->banner != NULL )
    {
        pBI->lpszTitle = pcbff->banner;
    }

    pBI->pidlRoot = pcbff->root;

    pBI->lpfn   = BrowseCallbackProc;
    pBI->lParam = (LPARAM)pcbff;
}

/**
 * Common code for the getFolder() and getItemIDList() Rexx methods.  The only
 * difference between the 2 methods is that getFolder() returns the  selected
 * folder as a fils system path and getItemIDList() returns the raw pidl.
 *
 * @param c
 * @param pCSelf
 * @param reuse
 * @param getPath
 *
 * @return RexxObjectPtr
 *
 * @remarks  By default we do the CoUnitialize after the browse for folder
 *           dialog is closed.  The user should be advised in the docs that the
 *           simplest thing to do is instantiate, configure, and get the folder
 *           on one thread.  For other usage patterns, the user then becomes
 *           responsible for matching CoInitializeEx() / CoUnitialize().  The
 *           BrowseForFolder class has sufficient methods for that to be done in
 *           Rexx, but the user will need to take care.
 */
RexxObjectPtr getFolderOrIDL(RexxMethodContext *c, void *pCSelf, logical_t reuse, bool getPath)
{
    pCBrowseForFolder pcbff = (pCBrowseForFolder)getBffCSelf(c, pCSelf);
    if ( pcbff == NULL )
    {
        return NULLOBJECT;
    }

    BROWSEINFO   bi = { 0 };

    fillInBrowseInfo(c, &bi, pcbff);

    // If second arg is true - return path.
    RexxObjectPtr result = folderBrowse(c, &bi, getPath);

    if ( ! reuse )
    {
        releaseCOM(pcbff);
    }

    return result;
}

/**
 * Returns the display name for the specified item ID list.
 *
 * This should return some name for any shell item, virtual folder or not.  We
 * first try to get a full path name, but if that fails we try a again using the
 * flag for destop absolute editing.
 *
 * @param c
 * @param pidl
 *
 * @return RexxObjectPtr
 *
 * @note  Sets the .systemErrorCode
 *
 * @remarks  The entry point for SHCreateItemFromIDList is not present on XP
 *           systems.  So we use GetProcAddress() to use it dynamically at run
 *           time.  If we don't ooDialog won't load on XP.
 */
RexxObjectPtr getBestDisplayName(RexxMethodContext *c, PIDLIST_ABSOLUTE pidl)
{
    oodResetSysErrCode(c->threadContext);

    RexxObjectPtr result = TheNilObj;

    HINSTANCE hinst = LoadLibrary(TEXT("Shell32.dll"));
    if ( hinst == NULL )
    {
        oodSetSysErrCode(c->threadContext);
        goto done_out;
    }

    SHCreateItemFromIDListPROC pSHCreateItemFromIDList = (SHCreateItemFromIDListPROC)GetProcAddress(hinst, "SHCreateItemFromIDList");
    if ( pSHCreateItemFromIDList == NULL )
    {
        oodSetSysErrCode(c->threadContext);
        goto done_out;
    }

    IShellItem *psi;

    HRESULT hr = pSHCreateItemFromIDList(pidl, IID_PPV_ARGS(&psi));
    if ( SUCCEEDED(hr) )
    {
        PWSTR pszName;

        // We first try to get a full path name
        hr = psi->GetDisplayName(SIGDN_FILESYSPATH, &pszName);
        if ( SUCCEEDED(hr) )
        {
            result = unicode2string(c, pszName);
            CoTaskMemFree(pszName);
        }
        else
        {
            // Probably a virtual file, try again
            hr = psi->GetDisplayName(SIGDN_DESKTOPABSOLUTEEDITING, &pszName);
            if ( SUCCEEDED(hr) )
            {
                result = unicode2string(c, pszName);
            }
            else
            {
                oodSetSysErrCode(c->threadContext);
            }

            CoTaskMemFree(pszName);
        }

        psi->Release();
    }
    else
    {
        oodSetSysErrCode(c->threadContext, hr);
    }

done_out:
    if ( hinst != NULL )
    {
        FreeLibrary(hinst);
    }

    return result;
}



/** BrowseForFolder::banner                  [attribute]
 */
RexxMethod1(RexxObjectPtr, bff_banner, CSELF, pCSelf)
{
    pCBrowseForFolder pcbff = (pCBrowseForFolder)getBffCSelf(context, pCSelf);
    if ( pcbff != NULL && pcbff->banner != NULL )
    {
        return context->String(pcbff->banner);
    }
    return context->NullString();
}
RexxMethod2(RexxObjectPtr, bff_setBanner, RexxObjectPtr, hint, CSELF, pCSelf)
{
    pCBrowseForFolder pcbff = (pCBrowseForFolder)getBffCSelf(context, pCSelf);
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
    pCBrowseForFolder pcbff = (pCBrowseForFolder)getBffCSelf(context, pCSelf);
    if ( pcbff != NULL && pcbff->dlgTitle != NULL )
    {
        return context->String(pcbff->dlgTitle);
    }
    return context->NullString();
}
RexxMethod2(RexxObjectPtr, bff_setDlgTitle, RexxObjectPtr, dlgTitle, CSELF, pCSelf)
{
    pCBrowseForFolder pcbff = (pCBrowseForFolder)getBffCSelf(context, pCSelf);
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
    pCBrowseForFolder pcbff = (pCBrowseForFolder)getBffCSelf(context, pCSelf);
    if ( pcbff != NULL && pcbff->hint != NULL )
    {
        return context->String(pcbff->hint);
    }
    return context->NullString();
}
RexxMethod2(RexxObjectPtr, bff_setHint, RexxObjectPtr, hint, CSELF, pCSelf)
{
    pCBrowseForFolder pcbff = (pCBrowseForFolder)getBffCSelf(context, pCSelf);
    if ( pcbff != NULL )
    {
        setTextAttribute(context, pcbff, hint, DlgHint);
    }
    return NULLOBJECT;
}

/** BrowseForFolder:options                  [attribute]
 */
RexxMethod1(RexxObjectPtr, bff_options, CSELF, pCSelf)
{
    pCBrowseForFolder pcbff = (pCBrowseForFolder)getBffCSelf(context, pCSelf);
    if ( pcbff != NULL )
    {
        return bif2keywords(context, pcbff->bifFlags);
    }
    return context->NullString();
}
RexxMethod2(RexxObjectPtr, bff_setOptions, CSTRING, opts, CSELF, pCSelf)
{
    pCBrowseForFolder pcbff = (pCBrowseForFolder)getBffCSelf(context, pCSelf);
    if ( pcbff != NULL )
    {
        pcbff->bifFlags = keywords2bif(context, opts, 1) | BIF_NEWDIALOGSTYLE;
        if ( pcbff->useHint )
        {
            pcbff->bifFlags |= BIF_UAHINT;
        }
        else
        {
            pcbff->bifFlags &= ~BIF_UAHINT;
        }
    }
    return NULLOBJECT;
}

/** BrowseForFolder:owner                  [attribute]
 */
RexxMethod1(RexxObjectPtr, bff_owner, CSELF, pCSelf)
{
    pCBrowseForFolder pcbff = (pCBrowseForFolder)getBffCSelf(context, pCSelf);
    if ( pcbff != NULL )
    {
        return pcbff->rexxOwner == NULLOBJECT ? TheNilObj : pcbff->rexxOwner;
    }
    return TheNilObj;
}
RexxMethod2(RexxObjectPtr, bff_setOwner, RexxObjectPtr, owner, CSELF, pCSelf)
{
    pCBrowseForFolder pcbff = (pCBrowseForFolder)getBffCSelf(context, pCSelf);
    if ( pcbff != NULL )
    {
        pCPlainBaseDialog pcpbd = requiredPlainBaseDlg(context, owner, 1);
        if ( pcpbd != NULL )
        {
            pcbff->rexxOwner = owner;
            pcbff->hOwner    = pcpbd->hDlg;
        }
    }
    return NULLOBJECT;
}

/** BrowseForFolder::root                  [attribute]
 */
RexxMethod1(RexxObjectPtr, bff_root, CSELF, pCSelf)
{
    pCBrowseForFolder pcbff = (pCBrowseForFolder)getBffCSelf(context, pCSelf);
    if ( pcbff != NULL && pcbff->root != NULL )
    {
        return context->NewPointer(pcbff->root);
    }
    return TheNilObj;
}
RexxMethod2(RexxObjectPtr, bff_setRoot, RexxObjectPtr, root, CSELF, pCSelf)
{
    pCBrowseForFolder pcbff = (pCBrowseForFolder)getBffCSelf(context, pCSelf);
    if ( pcbff == NULL )
    {
        return NULLOBJECT;
    }

    // Setting root to the .nil ojbect, or the empty string is the way to remove
    // a root setting.
    LPITEMIDLIST pidl = NULL;

    if ( root == TheNilObj )
    {
        ;  // Do not need to do anything pidl is already NULL.
    }
    else if ( context->IsPointer(root) )
    {
        pidl = (LPITEMIDLIST)context->PointerValue((RexxPointerObject)root);
    }
    else
    {
        CSTRING idl = context->ObjectToStringValue(root);

        // If the empty string, we do nothing, pidl is already NULL
        if ( *idl != '\0' )
        {
            pidl = getPidlFromString(context, idl, 1, false);
            if ( pidl == NULL )
            {
                return NULLOBJECT;
            }
        }
    }

    if ( pcbff->root != NULL )
    {
        CoTaskMemFree((LPVOID)pcbff->root);
    }
    pcbff->root = pidl;

    return NULLOBJECT;
}

/** BrowseForFolder::startDir                  [attribute]
 */
RexxMethod1(RexxObjectPtr, bff_startDir, CSELF, pCSelf)
{
    pCBrowseForFolder pcbff = getBffCSelf(context, pCSelf);
    if ( pcbff != NULL && pcbff->startDir != NULL )
    {
        return context->String(pcbff->startDir);
    }
    return context->NullString();
}
RexxMethod2(RexxObjectPtr, bff_setStartDir, RexxObjectPtr, startDir, CSELF, pCSelf)
{
    pCBrowseForFolder pcbff = getBffCSelf(context, pCSelf);
    if ( pcbff != NULL )
    {
        setTextAttribute(context, pcbff, startDir, DlgStartDir);
    }
    return NULLOBJECT;
}

/** BrowseForFolder::usePathForHint                  [attribute]
 */
RexxMethod1(RexxObjectPtr, bff_usePathForHint, CSELF, pCSelf)
{
    pCBrowseForFolder pcbff = getBffCSelf(context, pCSelf);
    if ( pcbff != NULL )
    {
        return context->Logical(pcbff->usePathForHint);
    }
    return context->NullString();
}
RexxMethod2(RexxObjectPtr, bff_setUsePathForHint, logical_t, usePath, CSELF, pCSelf)
{
    pCBrowseForFolder pcbff = getBffCSelf(context, pCSelf);
    if ( pcbff != NULL )
    {
        if ( usePath )
        {
            pcbff->usePathForHint = true;
            pcbff->useHint = true;
        }
        else
        {
            pcbff->usePathForHint = false;
            if ( pcbff->hint == NULL )
            {
                pcbff->useHint = false;
            }
        }
    }
    return NULLOBJECT;
}

/** BrowseForFolder::initialThread               [attribute]
 *
 *  Returns the thread ID of the thread this BrowseForFolder object was
 *  instantiated on, the ID of the thread that COM was initialized on.
 */
RexxMethod1(uint32_t, bff_initialThread, CSELF, pCSelf)
{
    pCBrowseForFolder pcbff = getBffCSelf(context, pCSelf);
    if ( pcbff != NULL )
    {
        return pcbff->coThreadID;
    }
    return 0;
}

/** BrowseForFolder::uninit()
 *
 * Does clean up for this BrowseForFolder.  Frees the root PIDL and calls the
 * common uninit routine.
 */
RexxMethod1(RexxObjectPtr, bff_uninit, CSELF, pCSelf)
{
    pCBrowseForFolder pcbff = getBffCSelf(context, pCSelf);
    if ( pcbff != NULL )
    {
#ifdef _DEBUG
    printf("bff_uninit()\n");
#endif
        if ( pcbff->root != NULL )
        {
            CoTaskMemFree((LPVOID)pcbff->root);
            pcbff->root = NULL;
        }

        safeLocalFree(pcbff->banner);
        safeLocalFree(pcbff->dlgTitle);
        safeLocalFree(pcbff->hint);
        safeLocalFree(pcbff->startDir);

        uninitCom(pcbff);
    }
    return NULLOBJECT;
}

/** BrowseForFolder::init()
 *
 * Initializes a BrowseForFolder object.  The title, banner, and hint strings
 * all have default values that are different from the Windows default values.
 * If the user wants to have the default Windows values, they specify that by
 * setting them to either .nil or the empty string.
 *
 * @remarks  MSDN says you have to use CoInitialize or SHBrowseForFolder will
 *           fail.  But it seems to work without it?
 *
 *           If a BrowseForFolder object has been previously instantiated and
 *           COM not released then CoInitializeEx will return S_FALSE, meaning
 *           COM is already initialized on this thread. We immediately do a
 *           CoUninitialize to decrement the reference counter fo this case.
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
    if ( hr == S_FALSE )
    {
        CoUninitialize();
    }
    else if ( hr == S_OK )
    {
        pcbff->countCoInitialized = 1;
        pcbff->coThreadID = GetCurrentThreadId();
    }
    else
    {
        systemServiceExceptionComCode(context->threadContext, COM_API_FAILED_MSG, "CoInitializeEx", hr);
        return NULLOBJECT;
    }

    pcbff->bifFlags = DEFAULT_BIF_FLAGS;

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

/** BrowseForFolder::initCOM
 *
 *  Invokes CoInitalizeEx() on the current thread, with some forethought.  If
 *  the current thread is the initial thread that this object was instantiated
 *  on, then the invocation is only done if needed.
 *
 *  If we are on the initial thread and the countCoInitialized is 0 the
 *  initialize is done and the count incremented.  If the count is not 0, the
 *  initialize is skipped.
 *
 *  If we are on another thread, the CoInitializeEx is always done, but the
 *  return is checked.  If it is S_FALSE, that indicates the CoInitializeEx has
 *  already been done on this thread and we immediately do an unitialize to keep
 *  things balanced.
 *
 *  If the user makes use of these methods, he assumes the responsibility of
 *  keeping CoInitializedEx / CoUnitialize() in balance *and* in invoking the
 *  releaseCOM method when he is done with this object.
 */
RexxMethod1(RexxObjectPtr, bff_initCOM, CSELF, pCSelf)
{
    pCBrowseForFolder pcbff = (pCBrowseForFolder)getBffCSelf(context, pCSelf);
    if ( pcbff == NULL )
    {
        return NULLOBJECT;
    }

    if ( pcbff->coThreadID == GetCurrentThreadId() )
    {
        return reInitCOM(context, pcbff);
    }

    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    if ( hr == S_FALSE )
    {
        CoUninitialize();
        return TheFalseObj;
    }
    else if ( hr == S_OK )
    {
        return TheTrueObj;
    }
    else
    {
        systemServiceExceptionComCode(context->threadContext, COM_API_FAILED_MSG, "CoInitializeEx", hr);
    }
    return TheFalseObj;
}

/** BrowseForFolder::getDisplayName()
 *
 * Returns the display name for an item ID list.
 *
 * @param   pidl  [required]  A handle, a pointer, to an item ID list
 *
 * @return  Returns the full display name for the item ID list on success, the
 *          .nil object on error.
 *
 * @note  Sets the system error code.
 */
RexxMethod2(RexxObjectPtr, bff_getDisplayName, POINTER, pidl, CSELF, pCSelf)
{
    pCBrowseForFolder pcbff = (pCBrowseForFolder)getBffCSelf(context, pCSelf);
    if ( pcbff == NULL )
    {
        return NULLOBJECT;
    }

    if ( ! requiredOS(context, "getDisplayName", "Vista", Vista_OS) )
    {
        return NULLOBJECT;
    }

    return getBestDisplayName(context, (PIDLIST_ABSOLUTE)pidl);
}


/** BrowseForFolder::getFolder()
 *
 * Puts up the customized Browse for Folder dialog using the atributes set for
 * this instance.
 *
 * @param   reuse  [OPTIONAL]  By default COM is unitialized on the return from
 *          this methd.  If reuse is true, COM is not unitialized and it becomes
 *          the programmer's responsibility to invoke release when the
 *          BrowseForFolder object is no longer needed.
 *
 * @return  Returns the fully qualified path of the folder the user selects, or
 *          the empty string if the user cancels.  In addition, if the user
 *          selects a virtual folder that has no file system path, .nil is
 *          returned.
 */
RexxMethod2(RexxObjectPtr, bff_getFolder, OPTIONAL_logical_t, reuse, CSELF, pCSelf)
{
    return getFolderOrIDL(context, pCSelf, reuse, true);
}


/** BrowseForFolder::getItemIDList()
 *
 * Puts up the customized Browse for Folder dialog using the atributes set for
 * this instance.
 *
 * @param   reuse  [OPTIONAL]  By default COM is unitialized on the return from
 *          this methd.  If reuse is true, COM is not unitialized and it becomes
 *          the programmer's responsibility to invoke release when the
 *          BrowseForFolder object is no longer needed.
 *
 * @return  Returns the pointer to item ID list the user selected, or the .nil
 *          object if the user cancels the dialog.
 */
RexxMethod2(RexxObjectPtr, bff_getItemIDList, OPTIONAL_logical_t, reuse, CSELF, pCSelf)
{
    return getFolderOrIDL(context, pCSelf, reuse, false);
}


/** BrowseForFolder::releaseCom
 *
 *  Calls CoUninitialize() on the current thread.  This is to allow the user to
 *  reuse the BrowseForFolder object.
 *
 *  By default CoUninitialize() will be called from getFolder() and the object
 *  will be 'dead'.  The user can signal getFolder() to not call
 *  CoUninitialize(), in which case, it becomes the user's responsibility to
 *  call release.
 *
 *  @return True if the thread CoUnitialize() was called on was the same thread
 *          as this object was first created on.  Returns false if the thread
 *          CoUnitialize() was called on another thread.  CoUnitialize() is
 *          *always* called.  Unless we are on the same thread as this object
 *           was instantiated on and the count initialized indicates we have
 *           already closed out COM.
 */
RexxMethod1(RexxObjectPtr, bff_releaseCOM, CSELF, pCSelf)
{
    pCBrowseForFolder pcbff = (pCBrowseForFolder)getBffCSelf(context, pCSelf);
    if ( pcbff == NULL )
    {
        return NULLOBJECT;
    }
    return releaseCOM(pcbff);
}

/** BrowseForFolder::releaseItemIDList()
 *
 *  Releases a handle, a pointer, to an item ID list.  This must be done for
 *  each handle returned from the getItemIDList() method and must *not* be done
 *  for the root item ID list.
 *
 */
RexxMethod2(RexxObjectPtr, bff_releaseItemIDList, POINTER, pidl, CSELF, pCSelf)
{
    pCBrowseForFolder pcbff = (pCBrowseForFolder)getBffCSelf(context, pCSelf);
    if ( pcbff == NULL )
    {
        return NULLOBJECT;
    }
    CoTaskMemFree(pidl);

    return TheZeroObj;
}

/**
 * Example code for GetDisplayName() using SHCreateItemFromIDList to create a
 * IShellItem from a IDL.   Worked Windows 7 64 bit
 *
 * On XP, having SHCreateItemFromIDlist in the code prevents shell.dll from
 * being loaded, which prevents oodialog.dll from being loaded.  This can be
 * worked around using GetProcAddress().  But, that would be cumbersome if using
 * a number of the 'Vista or later' shell APIs.
 *
 * This has now been integrated inot the BrowseForFolder::getDisplayName method.
 */
#if 0
RexxMethod1(RexxObjectPtr, bff_test, CSELF, pCSelf)
{
    pCBrowseForFolder pcbff = (pCBrowseForFolder)getBffCSelf(context, pCSelf);
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

    HRESULT hr = SHGetFolderLocation(NULL, CSIDL_PRINTERS, NULL, NULL, &pidlSystem);
    if ( SUCCEEDED(hr) )
    {
        IShellItem *psi;

        hr = pSHCreateItemFromIDList(pidlSystem, IID_PPV_ARGS(&psi));
        if ( SUCCEEDED(hr) )
        {
            PWSTR pszName;
            hr = psi->GetDisplayName(SIGDN_DESKTOPABSOLUTEEDITING, &pszName);
            if (SUCCEEDED(hr))
            {
                wprintf(L"Desktop Absolute Editing Display - %s\n", pszName);
                CoTaskMemFree(pszName);
            }
            else
            {
                printHResultErr("GetDisplayName", hr);
            }
            psi->Release();
        }
        else
        {
            printHResultErr("SHCreateItemFromIDList", hr);
        }

        ILFree(pidlSystem);
    }
    else
    {
        printHResultErr("ShGetFolderLocation", hr);
    }

    return NULLOBJECT;
}
#endif


/**
 * Trial code IFileDialog used to browse for folder.  Works Win 7 64 bit.
 *
 * Fails on XP because CLSID_FileOpenDialog is not available.  A simple
 * requiredOS() fixes that problem.
 *
 */
RexxMethod1(RexxObjectPtr, bff_test, CSELF, pCSelf)
{
    pCBrowseForFolder pcbff = (pCBrowseForFolder)getBffCSelf(context, pCSelf);
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
    if ( SUCCEEDED(hr) )
    {
        DWORD dwOptions;

        hr = pfd->GetOptions(&dwOptions);
        if ( SUCCEEDED(hr) )
        {
            pfd->SetOptions(dwOptions | FOS_PICKFOLDERS);

            hr = pfd->Show(NULL);
            if ( SUCCEEDED(hr) )
            {
                IShellItem *psi;

                hr = pfd->GetResult(&psi);
                if ( SUCCEEDED(hr) )
                {
                    PWSTR pszPath;

                    hr = psi->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, &pszPath);
                    if( SUCCEEDED(hr) )
                    {
                        wprintf(L"Got folder - %s\n", pszPath);
                        CoTaskMemFree(pszPath);
                    }
                    else
                    {
                        printHResultErr("GetDisplayName", hr);
                    }
                    psi->Release();
                }
                else
                {
                    printHResultErr("GetResult", hr);
                }
            }
            else
            {
                printHResultErr("Show", hr);
            }
        }
        else
        {
            printHResultErr("GetOptions", hr);
        }
        pfd->Release();
    }
    else
    {
        printHResultErr("CoCreateInstance", hr);
    }

    return NULLOBJECT;
}


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

    bi.ulFlags = DEFAULT_BIF_FLAGS;

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
            bi.lpszTitle = banner;
        }
    }

    if ( argumentExists(3) )
    {
        bi.ulFlags |= BIF_UAHINT;

        pcbff->useHint = true;
        if ( StrStrI("PATH", hint) != NULL )
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

