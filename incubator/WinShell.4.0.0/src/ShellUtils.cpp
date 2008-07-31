/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2008-2008 Rexx Language Association. All rights reserved.    */
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

#define STRICT
#define _WIN32_DCOM
#define _WIN32_IE       0x0601
#define _WIN32_WINNT    0x0501
#define WINVER          0x0501

#include <windows.h>
#include <shlobj.h>
#include <shlwapi.h>
#include <shellapi.h>
#include <objbase.h>
#include <commctrl.h>

#include "oorexxapi.h"

#include <string>
#include <map>
#include <stdio.h>

#include "ShellUtils.hpp"
#include "ShellUtilsResources.h"

// Map strings representing constant defines to their int values.  For
// translating things like "CSIDL_DRIVES" from the user to the proper API value.
using namespace std;
typedef map<string, int, less<string> > String2Int;
String2Int *ConstantsMap = 0;


typedef enum {
    OMITTED_TYPE,
    NIL_TYPE,
    STRING_TYPE,
    BOOLEAN_TYPE,
    OBJECT_TYPE
} RXARG_TYPES;


typedef enum
{
    TO_DIRECTION,
    FROM_DIRECTION
} SFO_DIRECTION;


// Local function prototypes.
INT CALLBACK         BrowseCallbackProc(HWND, UINT, LPARAM, LPARAM);
static RexxObjectPtr    folderBrowse(PBROWSEINFO, bool);
static LPITEMIDLIST  getPossiblePIDL(RexxObjectPtr, size_t, size_t, size_t *);
static bool          pidlFromPath(LPCSTR, LPITEMIDLIST *);
static bool          pidlForSpecialFolder(int, LPITEMIDLIST *);
static RexxObjectPtr    extractAllDefIcons(const char *);
inline HIMAGELIST    _getSelf(void);
static unsigned int  getImageListFlags(STRING, size_t);
static int           getListViewImageListType(const char *);
static RexxObjectPtr    setSystemImageList(bool);
inline void          setSFBAttribute(char *, STRING, char *);
static HWND          getValidWindowHandle(STRING, size_t);
static void          fillInBrowseData(STRING, PBROWSEINFO, PBROWSE_DATA);
static void          addToFileList(SFO_DIRECTION, CSTRING);
static void          setFOFlags(STRING, bool, size_t);
static FILEOP_FLAGS  getFOFlags(void);
static void          setOwnerWnd(STRING, size_t);
static HWND          getOwnerWnd(void);
static RexxObjectPtr    doTheOp(UINT, size_t, STRING, STRING, STRING, STRING);
inline char         *getTitle(HWND, FILEOP_FLAGS);
static RXARG_TYPES   getRxArgType(RexxObjectPtr);
static LPWSTR        ansiToUnicodeLength(LPCSTR, size_t, size_t *);
inline LPWSTR        ansiToUnicode(LPCSTR str, size_t inSize);
static HRESULT       queryTrashCan(const char *, PDWORDLONG, PDWORDLONG);
static bool          recycleBinIsEmpty(const char *);
static bool          _PathIsFull(const char *);
static bool          _is32on64Bit(void);
static void          initMaps(void);
inline int           getConstantValue(const char *);

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\
 *    Functions copied from ooDialog
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

inline bool rxArgOmitted(RexxMethodContext * context, size_t index)
{
    return context->ArrayHasIndex(context->GetArguments(), index) == 0 ? true : false;
}

inline bool rxArgExists(RexxMethodContext * context, size_t index)
{
    return context->ArrayHasIndex(context->GetArguments(), index) == 1 ? true : false;
}

void wrongArgValueException(RexxMethodContext *c, int pos, const char *list, RexxObjectPtr actual)
{
    RexxArrayObject a = c->NewArray(3);
    c->ArrayAppend(a, c->NewInteger(pos));
    c->ArrayAppend(a, c->NewStringFromAsciiz(list));
    c->ArrayAppend(a, actual);

    c->RaiseExceptionArray(Rexx_Error_Incorrect_method_list, a);
}

void wrongArgValueException(RexxMethodContext *c, int pos, const char *list, const char *actual)
{
    wrongArgValueException(c, pos, list, c->NewStringFromAsciiz(actual));
}

/**
 * Returns an upper-cased copy of the string.
 *
 * @param str   The string to copy and upper case.
 *
 * @return      A pointer to a new string, or null on a memory allocation
 *              failure.
 *
 * The caller is responsible for freeing the returned string.
 */
char *strdupupr(const char *str)
{
    char *retStr = NULL;
    if ( str )
    {
        size_t l = strlen(str);
        retStr = (char *)malloc(l);
        if ( retStr )
        {
            char *p;
            for ( p = retStr; *str; ++str )
            {
                if ( ('a' <= *str) && (*str <= 'z') )
                {
                    *p++ = *str - ('a' - 'A');
                }
                else
                {
                    *p++ = *str;
                }
            }
            *p = '\0';
        }
    }
    return retStr;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\
 *    End of functions copied from ooDialog
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/**
 * Copy bytes, restricting the amount copied to the max path size.
 *
 * @param dest  Destination for copy.
 * @param src   Source of copy.
 *
 * @assumes Destination is at least MAX_PATH + 1 in size.
 */
void pathdup(char *dest, const char *src)
{
    size_t l = strlen(src);
    l = l > MAX_PATH ? MAX_PATH : l;
    memcpy(dest, src, l);
    dest[l] = '\0';
}


/**
 * Most of the objects in the Windows Shell framework need COM initialized.
 * All objects use CommonInit in their ~init() methods to ensure that the
 * correct initial set up is done.
 *
 * Objects that do not need COM should indicate so by passing in false.  These
 * object then need to be sure and also use false in the CommonUnint call.
 *
 * The minimum required operating system is W2K with service pack 4.  This
 * ensures these minimum shell DLLs: Shell32.dll: 5.0 Shlwapi.dll: 5.0, and
 * Comctl32.dll: 5.81.
 *
 * Each creation of a Windows Shell object is counted, both per thread and
 * globally. CoInitializeEx() and CoUninitialize() must be called on a
 * per-thread basis.
 */
void CommonInit(bool useCOM)
{
    if ( (InterlockedIncrementRelease(&globalInstances) == 1) )
    {
        if ( ! _isAtLeastW2K() )
        {
            systemServiceException(REQUIRE_W2K_MSG, NULL);
        }
    }

    if ( InterlockedIncrementRelease(&threadInstances) == 1 )
    {
        HRESULT  hr;
        INITCOMMONCONTROLSEX ctrlex;

        thisModule = GetModuleHandle(SHELL_DLL);
        if ( thisModule == NULL )
        {
            systemServiceExceptionCode(NO_HMODULE_MSG, SHELL_DLL, GetLastError());
        }

        ctrlex.dwSize = sizeof(ctrlex);
        ctrlex.dwICC = ICC_WIN95_CLASSES | ICC_STANDARD_CLASSES;
        if ( ! InitCommonControlsEx(&ctrlex) )
        {
            systemServiceExceptionCode(NO_COMMCTRL_MSG, "Common Control Library", GetLastError());
        }

        initMaps();

        if ( useCOM )
        {
            hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
            #ifdef _DEBUG
                printf("CoInitializeEx returns: 0x%08x\n", hr);
            #endif
        }
    }
}


void CommonUninit(bool useCOM)
{
    if ( InterlockedDecrementRelease(&threadInstances) == 0 )
    {
        delete ConstantsMap;

        if ( useCOM )
        {
            CoUninitialize();
        }
    }
    InterlockedDecrementRelease(&globalInstances);
}

/** Some useful Shell function not implemented.
 *  SHFormatDrive puts up the Format Drive dialog
 *  SHGetFileInfo file info, mostly can get icons.
 *
 */

#define FINDFILES_WINDOW_CLASS      "CabinetWClass"
#define FINDFILES_WINDOW_TITLE      "Search Results"

/** WinShell::init()
 *
 * Basic init for a WinShell object.
 */
RexxMethod1(RexxObjectPtr, WinShell_init, OSELF, self)
{
    CommonInit(true);
    return RexxNil;
}

/** WinShell::uninit()
 *
 *  The basic uninit for the object.
 */
RexxMethod1(RexxObjectPtr, WinShell_uninit, OSELF, self)
{
    CommonUninit(true);
    return RexxNil;
}


/** WinShell::browseForFolder()
 *
 * Put up a plain Browse for Folder dialog and return the full qualified path of
 * the folder the user selects.
 *
 * @param   rxHwnd  OPTIONAL
 *   If specified, use this window as the owner window of the dialog.  This will
 *   disable the owner window until the user closes the dialog (which is usually
 *   what is wanted.)
 *
 * @return  Returns the fully qualified path of the folder the user selected, or
 *          the empty string if the user cancels the dialog.
 */
RexxMethod1(RexxObjectPtr, WinShell_browseForFolder, STRING, rxHwnd)
{
	LPITEMIDLIST pidlRoot = NULL;
    BROWSEINFO   bi = { 0 };
    RexxObjectPtr   rxResult = RexxNil;

    if ( pidlForSpecialFolder(CSIDL_DRIVES, &pidlRoot) )
    {
        bi.hwndOwner =  getValidWindowHandle(rxHwnd, 1);
        bi.pidlRoot  = pidlRoot;
        bi.ulFlags   = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_RETURNFSANCESTORS;

        rxResult = folderBrowse(&bi, true);
        CoTaskMemFree(pidlRoot);
    }
    else
    {
        systemServiceException(NO_ITEMID_MSG, "CSIDL_DRIVES");
    }
    return rxResult;
}


/** WinShell::pathFromCSIDL()
 *
 *  Given a CSIDL constant, returns the file system path for that folder.  Some
 *  CSIDL constants are for virtual folders and will not have a file system
 *  path.
 *
 *  @param  csidlConstant  REQUIRED
 *    The string name of the CSIDL_XXX constant, i.e. "CSIDL_DRIVES"
 *
 *  @param  create         OPTIONAL
 *    Tell the Windows Shell to create the special folder if it does not exist.
 *    The default is false, do not create.
 *
 *  @return  The file system path for the folder corresponding to the CSIDL
 *           constant, or .nil if there is no file system path for the constant.
 */
RexxMethod2(RexxObjectPtr, WinShell_pathFromCSIDL, CSTRING, csidlConstant, RexxObjectPtr, rxCreate)
{
    RexxMethodContext *context;
	LPITEMIDLIST pidl = NULL;
    RexxObjectPtr   rxResult = RexxNil;

    char *pCsidlConstant = strdupupr(csidlConstant);
    int csidl = getConstantValue(pCsidlConstant);
    free(pCsidlConstant);

    if ( csidl == -1 )
    {
        invalidConstantExceptionRx(1, INVALID_CONSTANT_MSG, "CSIDL", csidlConstant);
    }

    if ( optionalBoolArg(rxCreate, false, 2) )
    {
        csidl |= CSIDL_FLAG_CREATE;
    }

    if ( pidlForSpecialFolder(csidl, &pidl) )
    {
        TCHAR path[MAX_PATH];
        if ( SHGetPathFromIDList(pidl, path) )
        {
            rxResult = RexxString(path);
        }
        CoTaskMemFree(pidl);
    }

    return rxResult;
}

/** WinShell::pathFromItemID()
 *
 *  Given a pointer to an item ID list, returns the file system path for that
 *  folder.
 *
 *  @param  rxPIDL  REQUIRED
 *    The pointer to the item ID list.  This pointer is not freed, it belongs to
 *    the caller.
 */
RexxMethod1(RexxObjectPtr, WinShell_pathFromItemID, RexxObjectPtr, POINTER idlPtr)
{
	LPITEMIDLIST pidl = (LPITEMIDLIST)idlPtr;

    TCHAR path[MAX_PATH];
    if ( SHGetPathFromIDList(pidl, path) )
    {
        return context->NewStringFromAsciiz(path);
    }
    return context->Nil();
}

/** WinShell::getItemID
 *
 * Obtains a pointer to an item ID list for the specified path name or CSIDL
 * constant.
 *
 * The user must check for 0 on return. There are some valid reasons why the
 * pointer may not be obtained, so an exception is not raised.  If the
 * programmer uses the 0 pointer, an exception will then be raised.
 *
 * However, if a fully qualified path name is used, an exception will be raised
 * if the path does not exist. To try and get a PIDL from any string, use the
 * WinShell_getItemIDFromPath function.
 *
 * @return Pointer to item ID list on success or 0 on failure.
 */
RexxMethod1(RexxObjectPtr, WinShell_getItemID, RexxObjectPtr, rxObj)
{
    size_t source = 0;
	LPITEMIDLIST pidl = getPossiblePIDL(rxObj, 1, PPSF_PATH | PPSF_CSIDL | PPSF_NULL_OK, &source);

    if ( pidl == NULL )
    {
        return RexxInteger(0);
    }
    else
    {
        return ptrToRx(pidl);
    }
}


/** WinShell::getItemIDFromPath()
 *
 *  Obtains a pointer to an item ID list for the specified path.  This function
 *  will accept any string without checking that the string is in the format of
 *  a valid path name.
 *
 */
RexxMethod1(RexxObjectPtr, WinShell_getItemIDFromPath, STRING, rxObj)
{
    requiredArg(rxObj, 1);

	LPITEMIDLIST pidl;
	pidlFromPath(string_data(rxObj), &pidl);

    if ( pidl == NULL )
    {
        return RexxInteger(0);
    }
    else
    {
        return ptrToRx(pidl);
    }
}


/** WinShell::getItemIDFromCSIDL()
 *
 *  Obtains a pointer to an item ID list for the specified CSIDL constans.  This
 *  function will accept any string without checking that the string is in the
 *  format of a valid CSIDL constant.
 *
 */
RexxMethod1(RexxObjectPtr, WinShell_getItemIDFromCSIDL, STRING, rxObj)
{
    requiredArg(rxObj, 1);

    RexxObjectPtr rxResult = RexxInteger(-1);
	LPITEMIDLIST pidl;

    int csidl = getConstantValue(string_data(rxObj));
    if ( csidl != -1 && pidlForSpecialFolder(csidl, &pidl) )
    {
        if ( pidl == NULL )
        {
            rxResult = RexxInteger(0);
        }
        else
        {
            rxResult = ptrToRx(pidl);
        }
    }
	return rxResult;
}


/** WinShell::releaseItemID()
 *
 */
RexxMethod1(RexxObjectPtr, WinShell_releaseItemID, STRING, rxPIDL)
{
    requiredArg(rxPIDL, 1);

    LPITEMIDLIST pidl = (LPITEMIDLIST)ptrFromRx(rxPIDL);
    if ( pidl == 0 )
    {
        badArgException(1, ITEMIDLIST_HANDLE_MSG);
    }

    CoTaskMemFree(pidl);

    return RexxInteger(0);
}


/** WinShell::openFindFiles()
 *
 *  Opens the
 */
RexxMethod2(RexxObjectPtr, WinShell_openFindFiles, STRING, rxStart, STRING, rxSaved)
{
    RexxObjectPtr rxResult = RexxNil;
    size_t folderSource = 0;
    size_t savedSource = 0;
    LPITEMIDLIST pidlFolder = getPossiblePIDL(rxStart, 1, PPSF_ALL_BUT_NIL, &folderSource);
    LPITEMIDLIST pidlSaved = getPossiblePIDL(rxSaved, 2, PPSF_PATH | PPSF_POINTER, &savedSource);

    /* SHFindFiles is documented as returning true if the dialog is shown,
     * otherwise false.  However, it seems to always return false.  So the
     * return is ignored.
     */
    BOOL success = SHFindFiles(pidlFolder, pidlSaved);
    HWND hwnd = FindWindow(FINDFILES_WINDOW_CLASS, FINDFILES_WINDOW_TITLE);

    if ( hwnd != NULL )
    {
        if ( SetForegroundWindow(hwnd) == 0 )
        {
            // The window did not come to the foreground, try flashing it.
            FLASHWINFO fwi;
            fwi.cbSize = sizeof(fwi);
            fwi.dwFlags = FLASHW_ALL;
            fwi.dwTimeout = 0;
            fwi.hwnd = hwnd;
            fwi.uCount = 10;

            FlashWindowEx(&fwi);
        }
        rxResult = handleToRx(hwnd);
    }
    else
    {
        rxResult = RexxInteger(-(long)GetLastError());
    }

    if ( folderSource != PPSF_POINTER )
    {
        CoTaskMemFree(pidlFolder);
    }
    if ( pidlSaved != NULL && savedSource != PPSF_POINTER )
    {
        CoTaskMemFree(pidlSaved);
    }
    return rxResult;
}


/** WinShell::openFolder()
 *
 */
RexxMethod1(RexxObjectPtr, WinShell_openFolder, STRING, rxFolder)
{
    return RexxNil;
}


/** WinShell::closeWindow()
 *
 */
RexxMethod1(RexxObjectPtr, WinShell_closeWindow, STRING, rxHwnd)
{
    requiredArg(rxHwnd, 1);

    RexxObjectPtr rxResult = RexxInteger(0);
    HWND hwnd = (HWND)handleFromRx(rxHwnd);

    if ( ! SendNotifyMessage(hwnd, WM_SYSCOMMAND, SC_CLOSE, 0) )
    {
        rxResult = RexxInteger(GetLastError());
    }
    return rxResult;
}


/** WinShell::clearRecentDocuments()
 *
 */
RexxMethod0(RexxObjectPtr, WinShell_clearRecentDocuments)
{
    // SHAddToRecentDocs has no return code.
    SHAddToRecentDocs(SHARD_PATH, NULL);
    return RexxInteger(0);
}


/** WinShell::addToRecentDocuments()
 *
 *  I think the intent was that rxDoc could be a PIDL, CSIDL_, .nil, or empty
 *  string.
 *
 */
RexxMethod1(RexxObjectPtr, WinShell_addToRecentDocuments, STRING, rxDoc)
{
    size_t source = 0;
	LPITEMIDLIST pidl = getPossiblePIDL(rxDoc, 1, PPSF_POINTER | PPSF_NULL_OK, &source);

    if ( pidl == NULL )
    {
        SHAddToRecentDocs(SHARD_PATH, string_data((RexxString *)rxDoc));
    }
    else
    {
        SHAddToRecentDocs(SHARD_PIDL, pidl);
    }

    // SHAddToRecentDocs has no return code.
    return RexxInteger(0);
}


/** WinShell::_queryDiskSpace()  <private>
 *
 *
 *  Note that rxPath is not omitted (can not be omitted) because use strict arg
 *  is done in WinShell.cls.
 */
RexxMethod1(RexxObjectPtr, WinShell_queryDiskSpace_private, STRING, rxPath)
{
    ULARGE_INTEGER userFree, total, totalFree;
    TCHAR buffer[64];

    if ( SHGetDiskFreeSpaceEx(string_data(rxPath), &userFree, &total, &totalFree) )
    {
        _snprintf(buffer, 64, "%I64d %I64d %I64d", total, totalFree, userFree);
    }
    else
    {
        _snprintf(buffer, 64, "Err: %d", GetLastError());
    }
    return RexxString(buffer);
}


/** WinShell::_queryRecycleBin()  <private>
 *
 *  Determines the size (in bytes) and number of items in the Recycle Bin.  This
 *  can be either for all Recycle Bins, or the Recycle Bin a specific drive.
 *
 *  Note that rxPath does not need to be checked for an omitted argument because
 *  use strict arg is done in WinShell.cls.
 */
RexxMethod1(RexxObjectPtr, WinShell_queryRecycleBin_private, STRING, rxPath)
{
    DWORDLONG bytes = 0, items = 0;
    HRESULT   hr;
    TCHAR     buffer[64];

    char *root = string_data(rxPath);

    hr = queryTrashCan(root, &bytes, &items);
    if ( hr == S_OK )
    {
        _snprintf(buffer, 64, "%I64u %I64u", bytes, items);
    }
    else
    {
        _snprintf(buffer, 64, "Err: 0x%08x", hr);
    }
    return RexxString(buffer);
}


/** WinShell::emptyRecycleBin()
 *
 *  Empties the recycle bin, either for a specific drive, or for all drives.
 *
 */
RexxMethod3(RexxObjectPtr, WinShell_emptyRecycleBin, OPTIONAL_CSTRING, root, RexxObjectPtr, rxConfirm, STRING, rxHwnd)
{
    // The flags for no confirmation, which is the defualt.
    DWORD       flags = SHERB_NOCONFIRMATION | SHERB_NOPROGRESSUI | SHERB_NOSOUND;
    HRESULT     hr;
    RexxObjectPtr  rxResult = RexxInteger(0);

    if ( root == NULL )
    {
        root = "";
    }
    if ( strlen(root) > 0 )
    {
        if ( ! _PathIsFull(root) )
        {
            wrongArgValueException(1, "omitted, the empty string, or a fully qualified path name", root);
        }
    }

    if ( optionalBoolArg(rxConfirm, false, 2) )
    {
        flags = 0;
    }

    HWND hwnd = getValidWindowHandle(rxHwnd, 3);

    if ( recycleBinIsEmpty(root) )
    {
        if ( flags == 0 )
        {
            ShellMessageBox(NULL, hwnd, "The Recycle Bin is empty.",
                            (hwnd == NULL ? "ooRexx Windows Shell - Information" : NULL),
                            MB_TASKMODAL | MB_OK | MB_ICONINFORMATION);
        }
    }
    else
    {

        hr = SHEmptyRecycleBin(hwnd, root, flags);
        if ( hr != S_OK )
        {
            rxResult = hrToRx(hr);
        }
    }
    return rxResult;
}


/** WinShell::loadIcon()
 *
 * Need doc.  The idea is that loadIcon will be used to get icon handles that
 * then need to be released while getSharedIcon will be used to get icon handles
 * that should *not* be released.  Then it will be up to the ooRexx programmer
 * to keep track of what should and should not be released.
 *
 *
 *
 */
RexxMethod2(POINTER, WinShell_loadIcon, CSTRING, path, uint32_t, index)
{
    return ExtractIcon(NULL, path, index);
}


/** WinShell::getSharedIcon()
 *
 *  Need doc.  see loadIcon for some details.
 *
 *
 *
 */
RexxMethod2(RexxObjectPtr, WinShell_getSharedIcon, STRING, rxPath, RexxObjectPtr, rxIndex )
{
    /**
     * Want to be able to load a: system icon, icon from this module, icon from
     * a resource only DLL loaded by say, ooDialog, and an icon from some other
     * loaded extension DLL, for instance ooDialog.dll.  So have a hModule arg
     * that can also be a keyword (like self) for this DLL, null for system DLL,
     * or actual hModule.
     */
    HICON hIcon = NULL;
    return handleToRx(hIcon);
}


/** WinShell::extractDefaultIcons()
 *
 *
 *
 *
 */
RexxMethod2(RexxObjectPtr, WinShell_extractDefaultIcons, STRING, rxPath, RexxObjectPtr, rxIndex )
{
    RexxObjectPtr rxResult;

    requiredArg(rxPath, 1);
    char *path = string_data(rxPath);

    /**
     * If rxIndex is omitted or .nil, then an array of all the icons in the
     * specified file are returned.
     */
    if ( rxIndex == NULLOBJECT || rxIndex == RexxNil )
    {
        return extractAllDefIcons(path);
    }

    int index = integer_value(rxIndex);
    HICON hSmall, hBig;

    HRESULT hr = SHDefExtractIcon(path, index, 0, &hBig, &hSmall, 0);
    if ( FAILED(hr) )
    {
        rxResult = hrToRx(hr);
    }
    else
    {
        rxResult = RexxArray2(handleToRx(hBig), handleToRx(hSmall));
    }

    return rxResult;
}

static RexxObjectPtr extractAllDefIcons(const char *path)
{
    RexxObjectPtr rxResult = RexxNil;

    /* Get the number of icons in the file. */
    unsigned int count = ExtractIconEx(path, -1, NULL, NULL, 0);

    if ( count > 0 )
    {
        HICON *pLarge = (HICON *)shellAlloc(count * sizeof(HICON *));
        HICON *pSmall = (HICON *)shellAlloc(count * sizeof(HICON *));

        if ( pLarge == NULL || pSmall == NULL )
        {
            systemServiceException(MEMORY_ALLOC_FAILED_MSG, NULL);
        }

        count = ExtractIconEx(path, 0, pLarge, pSmall, count);

        if ( count > 0 )
        {
            RexxObjectPtr rxLarge = RexxArray(count);
            RexxObjectPtr rxSmall = RexxArray(count);
            for ( unsigned int i = 0; i < count; i++ )
            {
                array_put(rxLarge, handleToRx(pLarge[i]), i + 1);
                array_put(rxSmall, handleToRx(pSmall[i]), i + 1);
            }
            rxResult = RexxArray2(rxLarge, rxSmall);
        }
        shellFree(pLarge);
        shellFree(pSmall);
    }

    return rxResult;
}


/** WinShell::selectIcon()
 *
 *  Puts up the Change Icon dialog allowing the user to select an icon from an
 *  icon file.
 *
 *
 *
 */
RexxMethod2(RexxObjectPtr, WinShell_selectIcon, CSTRING, path, STRING, rxHwnd)
{
    RexxObjectPtr rxResult = RexxNil;

    char pathBuffer[MAX_PATH];
    WCHAR wBuffer[MAX_PATH];

    if ( MultiByteToWideChar(CP_ACP, 0, path, -1, wBuffer, MAX_PATH) == 0 )
    {
        systemServiceException(UNICODE_ANSI_FAILED_MSG, NULL);
    }

    HWND hwnd = getValidWindowHandle(rxHwnd, 2);
    int iconIndex = 0;

    if ( PickIconDlg(hwnd, wBuffer, MAX_PATH, &iconIndex) == 1 )
    {
        if ( WideCharToMultiByte(CP_ACP, 0, wBuffer, -1, pathBuffer, MAX_PATH, NULL, NULL) == 0)
        {
            systemServiceException(UNICODE_ANSI_FAILED_MSG, NULL);
            return rxResult;
        }

        rxResult = RexxArray2(RexxInteger(iconIndex), RexxString(buffer));
    }

    return rxResult;
}


/** WinShell::releaseIcon()
 *
 *  Releases an icon handle.  Certain icons must be released (should be
 *  released.)  While other icons (shared icons) must not (should not) be
 *  released.  There is not a good way to be sure the ooRexx programmer is doing
 *  the right thing.
 *
 *
 */
RexxMethod1(RexxObjectPtr, WinShell_releaseIcon, STRING, rxIcon)
{
    requiredArg(rxIcon, 1);

    RexxObjectPtr rxResult = RexxInteger(0);

    HICON hIcon = (HICON)handleFromRx(rxIcon);
    if ( hIcon != 0 )
    {
        if ( DestroyIcon(hIcon) == 0 )
        {
            rxResult = RexxInteger(GetLastError());
        }
    }
    else
    {
        badArgException(1, "icon handle");
    }

    return rxResult;
}


/** WinShell::about()
 *
 */
RexxMethod5(RexxObjectPtr, WinShell_about, STRING, rxTitle, STRING, rxApp,
            STRING, rxOther, STRING, rxHwnd, STRING, rxIcon)
{
    char  buffer[128];
    char *app = NULL;
    char *other = NULL;
    HICON icon = NULL;

    requiredArg(rxTitle, 1);

    if ( string_length(rxTitle) > 63 )
    {
        stringToLongException(1, 63, string_length(rxTitle));
    }

    if ( rxApp == NULLOBJECT )
    {
        app = string_data(rxTitle);
    }
    else
    {
        if ( string_length(rxApp) > 63 )
        {
            stringToLongException(2, 63, string_length(rxApp));
        }
        _snprintf(buffer, sizeof(buffer), "%s#%s", string_data(rxTitle), string_data(rxApp));
        app = buffer;
    }

    // We don't enforce a limit on the other string, but if the programmer makes
    // it too long it will be truncated.
    if ( rxOther != NULLOBJECT )
    {
        other = string_data(rxOther);
    }

    HWND hwnd = getValidWindowHandle(rxHwnd, 4);

    if ( rxIcon != NULLOBJECT )
    {
        icon = (HICON)handleFromRx(rxIcon);
    }

    if ( ShellAbout(hwnd, app, other, icon) )
    {
        return RexxTrue;
    }
    return RexxFalse;
}


/** WinShell::test()
 *
 * A temporary method of this WinShell.  Provides a simple way to try various
 * things from the ooRexx side.
 */
RexxMethod2(RexxObjectPtr, WinShell_test, OSELF, self, STRING, hwnd)
{

    if ( hwnd == NULLOBJECT )
    {
        printf("Arg 1 is null\n");
    }
    else
    {
        printf("Arg 1 is not null\n");
    }

    printf("In WinShell::test(), no more tests.\n");
    return RexxNil;
}

static LPITEMIDLIST getPossiblePIDL(RexxObjectPtr rxArg, size_t argNumber, size_t flags, size_t * pSource)
{
	LPITEMIDLIST pidl = NULL;
    bool         success = false;

    if ( rxArg == NULLOBJECT )
    {
        *pSource = PPSF_OMITTED;
        return NULL;
    }

    if ( rxArg == RexxNil )
    {
        if ( !( flags & PPSF_NIL) )
        {
            wrongFormatException(argNumber, rxArg);
        }

        *pSource = PPSF_NIL;
        return NULL;
    }

    char   *arg = requiredStringArg(rxArg, argNumber);
    size_t  len = string_length((RexxString *)rxArg);

    if ( (flags & PPSF_EMPTY_STRING) && len == 0 )
    {
        *pSource = PPSF_EMPTY_STRING;
        success = true;
    }
    else if ( (flags & PPSF_CSIDL) && (len > 6) && (StrCmpNI("CSIDL_", arg, 6) == 0) )
    {
        char *argTmp = strdupupr(context->StringData((RexxStringObject)rxArg));
        int csidl = getConstantValue(argTmp);
        free(argTmp);

        if ( csidl == -1 )
        {
            invalidConstantExceptionRx(argNumber, INVALID_CONSTANT_MSG, "CSIDL", rxArg);
        }

        *pSource = PPSF_CSIDL;
        success = pidlForSpecialFolder(csidl, &pidl);
    }
    else if ( (flags & PPSF_PATH) && _PathIsFull(arg) )
    {
        *pSource = PPSF_PATH;
        success = pidlFromPath(arg, &pidl);
    }
    else if ( (flags & PPSF_POINTER) && ((_is64Bit() && len == 16) || (! _is64Bit() && len == 8)) )
    {
        pidl = (LPITEMIDLIST)ptrFromRx(rxArg);
        if ( pidl == 0 )
        {
            systemServiceException(STRING_TO_PTR_FAILED_MSG, NULL);
        }

        *pSource = PPSF_POINTER;
        success = true;
    }
    else if ( flags & PPSF_NULL_OK )
    {
        success = true;
    }
    else
    {
        wrongFormatException(argNumber, rxArg);
    }

    if ( (! success) && ! (flags & PPSF_NULL_OK) )
    {
        wrongFormatException(argNumber, rxArg);
    }
    return pidl;
}


static bool recycleBinIsEmpty(const char *root)
{
    DWORDLONG   bytes = 0;
    DWORDLONG   items = 0;
    HRESULT     hr;
    const char *_root = root;

    if ( _isW2K() )
    {
        if ( root == NULL || strlen(root) == 0 )
        {
            _root = "C:\\";
        }
    }

    hr = queryTrashCan(_root, &bytes, &items);

    if ( hr == S_OK && bytes == 0 && items == 0 )
    {
        return true;
    }
    return false;
}

static HRESULT queryTrashCan(const char * root, PDWORDLONG size, PDWORDLONG items)
{
    SHQUERYRBINFO sqbi = { 0 };
    HRESULT       hr;

    sqbi.cbSize = sizeof(sqbi);
    hr = SHQueryRecycleBin(root, &sqbi);
    if ( hr == S_OK )
    {
        *size = sqbi.i64Size;
        *items = sqbi.i64NumItems;
    }

    return hr;
}


/* class: ImageList- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\
    A representation of the ImageList control
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#define IL_HANDLE_ATTR         "HANDLE"
#define IL_IMAGETYPE_ATTR      "IMAGETYPE"
#define IL_IMAGETYPE_ICON      RexxInteger(0)
#define IL_IMAGETYPE_BITMAP    RexxInteger(1)
#define DEFAULT_CX             32
#define DEFAULT_CY             32
#define DEFAULT_FLAGS          ILC_COLOR32 | ILC_MASK
#define DEFAULT_GROW           0


/** ImageList::init()
 *
 *  Initializes a new ImageList object.
 *
 *
 */
RexxMethod6(RexxObjectPtr, ImageList_init, CSTRING, type, RexxObjectPtr, rxImages,
            RexxObjectPtr, rxX, RexxObjectPtr, rxY, OPTIONAL_CSTRING, _flags, RexxObjectPtr, rxGrow)
{
    bool isBitmap = false;
    bool createEmptyImageList = true;

    CommonInit(false);

    /** The type determines how the image list is created.  Some keywords
     *  indicate a short cut creation.  (And some keywords are not yet
     *  implemented, like creating duplicating an existing image list.)
     */
    if ( stricmp("SYSTEMLARGE", type) == 0 )
    {
        return setSystemImageList(true);
    }
    else if ( stricmp("SYSTEMSMALL", type) == 0 )
    {
        return setSystemImageList(true);
    }
    else if ( stricmp("ICON", type) == 0 )
    {
        REXX_SETVAR(IL_IMAGETYPE_ATTR, IL_IMAGETYPE_ICON);
    }
    else if ( stricmp("BITMAP", type) == 0 )
    {
        REXX_SETVAR(IL_IMAGETYPE_ATTR, IL_IMAGETYPE_BITMAP);
        isBitmap = true;
    }
    else
    {
        /** There are some other types to be implemented later.  Need to
         *  figure out how to do them.  Like ImageList_Duplicate,
         *  ImageList_LoadImage, ImageList_Merge, etc..
         */
        wrongArgValueException(1, "SystemLarge, SystemSmall, Icon, or Bitmap", type);
    }

    /** If we are here, rxImages is required. */
    requiredArg(rxImages, 2);

    /* Check the optional args first. */
    int cx   = optionalPositiveArg(rxX, DEFAULT_CX, 3);
    int cy   = optionalPositiveArg(rxY, DEFAULT_CY, 4);
    int grow = optionalPositiveArg(rxGrow, DEFAULT_GROW, 6);

    unsigned int flags = DEFAULT_FLAGS;
    if ( _flags != NULL )
    {
        flags = getImageListFlags(_flags, 5);
        /** DFX TODO should check here for ILC_PERITEMMIRROR and ILC_MIRROR,
         *  these flags are only available with Comctl32.dll version 6.
         */
    }

    RexxMethodContext *context;
    /** rxImages can either be the count (size) of an empty image list to
     *  create, or it can be an array of image handles.  (Icons or bitmaps.)
     *  Anything else, at this time, is wrong.
     */
    int count;
    if ( context->IsInteger(rxImages) )
    {
        count = requiredPositiveArg(rxImages, 2);
    }
    else if ( context->IsArray(rxImages) )
    {
        count = context->ArraySize(rxImages);
        createEmptyImageList = false;
    }
    else
    {
        wrongArgValueException(2, "an array of image handles or a count of images", rxImages);
    }

    // It seems as though some APIs do not clear last error.
    SetLastError(0);

    HIMAGELIST himl = ImageList_Create(cx, cy, flags, count, grow);
    if ( himl == NULL )
    {
        systemServiceExceptionCode(NO_CREATE_MSG, "image list", GetLastError());
    }

    if ( ! createEmptyImageList )
    {
        VOID *image;
        int index;
        for ( int i = 1; i <= count; i++ )
        {
            image = handleFromRx(array_at(rxImages, i));

            if ( isBitmap )
            {
                /** TODO for bitmaps, there is also the ?? what was this
                 *  comment about?
                 */
                index = ImageList_Add(himl, (HBITMAP)image, NULL);
            }
            else
            {
                index = ImageList_AddIcon(himl, (HICON)image);
            }

            /** What to do on error?  An exception seems to harsh, but ...  Also
             *  the icon or bitmap handle can be deleted now, but we don't know
             *  if it is shared or not.  For instance, a user could create an
             *  image list using the system icons.  Bitmaps are less likely to
             *  be shared and maybe they should be deleted?
             */
            //printf("Added image: %p index: %d last err: %d\n", image, index, GetLastError());
        }
    }
    REXX_SETVAR(IL_HANDLE_ATTR, ptrToRx(himl));

    return RexxNil;
}


/** ImageList::uninit()
 *
 *  The basic uninit for the object.
 */
RexxMethod0(RexxObjectPtr, ImageList_uninit)
{
    /** TODO Should we destroy the image list on unint??  We need some
     *  housekeeping to determine if image list is shared or not.
     */

    CommonUninit(false);
    return RexxNil;
}


/** ImageList::destroy()  Class method.
 *
 *  Destroys (frees, releases) an image list using the supplied handle.
 */
RexxMethod1(logical_t, ImageList_destroy_class, POINTER, ptr)
{
    HIMAGELIST himl = (HIMAGELIST)ptr;

    /** TODO add code to check that the supplied handle is not the handle of
     *  this image list.
     */

    if ( ImageList_Destroy(himl) == 0 )
    {
        return 0;
    }
    return 1;
}


/** ImageList::release()
 *
 *  Free the underlying image list that this object represents.  Once this is
 *  done, this object is no longer usable.
 */
RexxMethod0(RexxObjectPtr, ImageList_release)
{
    HIMAGELIST himl = _getSelf();
    RexxObjectPtr rxSuccess = RexxFalse;

    if ( ImageList_Destroy(himl) != 0 )
    {
        REXX_SETVAR(IL_HANDLE_ATTR, ptrToRx(NULL));
        rxSuccess = RexxTrue;
    }

    return rxSuccess;
}


/** ImageList::getBkColor()
 *
 *  Return the current background color for this image list.  The value is a
 *  COLORREF, for convenience the value can be returned split into its RGB
 *  values.
 */
RexxMethod1(RexxObjectPtr, ImageList_getBkColor, RexxObjectPtr, rxUseRGB)
{
    TCHAR buffer[32];
    HIMAGELIST himl  = _getSelf();
    bool       doRGB = optionalBoolArg(rxUseRGB, false, 1);
    COLORREF   ref   = ImageList_GetBkColor(himl);

    if ( doRGB )
    {
        _snprintf(buffer, sizeof(buffer), "%d %d %d %08x",
                  GetRValue(ref), GetGValue(ref), GetBValue(ref), ref);
    }
    else
    {
        _snprintf(buffer, sizeof(buffer), "%08x", ref);
    }
    return RexxString(buffer);
}


/** ImageList::setBkColor()
 *
 *  Sets the current background color for this image list.  The value is
 *  specified as a COLORREF.  The previous background color is returned.
 */
RexxMethod1(RexxObjectPtr, ImageList_setBkColor, RexxObjectPtr, rxUseRGB)
{
    TCHAR buffer[32];
    HIMAGELIST himl    = _getSelf();
    bool       doRGB   = optionalBoolArg(rxUseRGB, false, 1);
    COLORREF   lastRef = ImageList_SetBkColor(himl, CLR_NONE);

    if ( doRGB )
    {
        _snprintf(buffer, sizeof(buffer), "%d %d %d %08x",
                  GetRValue(lastRef), GetGValue(lastRef), GetBValue(lastRef), lastRef);
    }
    else
    {
        _snprintf(buffer, sizeof(buffer), "%08x", lastRef);
    }
    return RexxString(buffer);
}


/** ImageList::getImageCount()
 *
 *  Return the number of images in this list.
 */
RexxMethod0(RexxObjectPtr, ImageList_getImageCount)
{
    HIMAGELIST himl = _getSelf();
    return RexxInteger(ImageList_GetImageCount(himl));
}


/** ImageList::getImageSize()  <ImageList_GetIconSize>
 *
 *  Return the width and height of images in this list.  All images in an image
 *  list have the same width and height.
 *
 *  Returns .nil on error, otherwise returns a string with two tokens.  The
 *  first is the width (cx) and the second is the height (cy.)
 */
RexxMethod0(RexxObjectPtr, ImageList_getImageSize)
{
    HIMAGELIST himl = _getSelf();
    RexxObjectPtr rxResult = RexxNil;
    int cx, cy;

    if ( ImageList_GetIconSize(himl, &cx, &cy) )
    {
        TCHAR buffer[32];
        _snprintf(buffer, sizeof(buffer), "%d %d", cx, cy);
        rxResult = RexxString(buffer);
    }
    return rxResult;
}


/** ImageList::setListViewImages()
 *
 *  Assigns this ImageList to a ListView control.
 *
 *  Note that ooDialog in 3.2.0 does not support setting image lists directly in
 *  the list view control.  ooDialog in 3.x will support it (as soon as I add
 *  it.)   If support in ooDialog is added, this function may become redundent.
 *
 */
RexxMethod2(RexxObjectPtr, ImageList_setListViewImages, STRING, rxListView, OPTIONAL_CSTRING, size )
{
    requiredArg(rxListView, 1);

    HWND hwnd = getValidWindowHandle(rxListView, 2);
    int flag = LVSIL_NORMAL;
    HIMAGELIST himl = _getSelf();

    if ( size != NULLOBJECT )
    {
        flag = getListViewImageListType(size);
        if ( flag == -1 )
        {
            return NULLOBJECT;
        }
    }

    HIMAGELIST oldImages = ListView_SetImageList(hwnd, himl, flag);

    /**
     * If the list view does not have the LVS_SHAREIMAGELISTS style, the old
     * image list needs to be destroyed.  Do it here, let the user do it ??
     */
    return ptrToRx(oldImages);
}

inline HIMAGELIST _getSelf(void)
{
    HIMAGELIST himl = (HIMAGELIST)ptrFromRx(REXX_GETVAR(IL_HANDLE_ATTR));
    if ( himl == NULL )
    {
        send_exception1(Error_Execution_user_defined, RexxArray1(RexxString(IMAGELIST_RELEASED_MSG)));
    }
    return himl;
}

static unsigned int getImageListFlags(const char *_flags, size_t argNumber)
{
    char *flags = strdupupr(_flags);
    unsigned int flag = -1;
    unsigned int ilcFlags = 0;

    char *token = strtok(flags, " ,|");
    while( token != NULL )
    {
        flag = getConstantValue(token);
        if ( flag == -1 )
        {
            invalidConstantException(argNumber, INVALID_CONSTANT_MSG, "ILC", token);
            return -1;
        }
        ilcFlags |= flag;
        token = strtok(NULL, " ,|");
    }
    free(flags);

    /** If flag is still -1 then the string was empty. */
    if ( flag == -1 )
    {
        wrongArgValueException(argNumber, "the valid ILC_XXX constants", _flags);
        return -1;
    }

    return ilcFlags;
}


static int getListViewImageListType(const char *size)
{
    int flag = -1;

    if ( strnicmp("LVS", size, 3) == 0 )
    {
        flag = getConstantValue(type);
    }
    else
    {
        if ( stricmp("NORMAL", size) == 0 )
        {
            flag = LVSIL_NORMAL;
        }
        else if ( stricmp("SMALL", size) == 0 )
        {
            flag = LVSIL_SMALL;
        }
        else if ( stricmp("STATE", size) == 0 )
        {
            flag = LVSIL_STATE;
        }
    }

    if ( flag == -1 )
    {
        wrongArgExceptionRx(1, "a valid LVSIL_XXX constant, Normal, Small, or State", size);
    }
    return flag;
}


static RexxObjectPtr setSystemImageList(bool setLarge)
{
    HIMAGELIST hLarge = NULL;
    HIMAGELIST hSmall = NULL;

    /* FileIconInit() only needs to be called one time per process. */
    if ( ! fileIconInitDone )
    {
        HMODULE hmod = LoadLibrary("shell32.dll");
        if ( hmod == NULL )
        {
            systemServiceExceptionCode(NO_HMODULE_MSG, "shell32.dll", GetLastError());
        }

        FILEICONINIT_PROC FileIconInit = (FILEICONINIT_PROC)GetProcAddress(hmod, (LPCSTR)FILEICONINIT_ORD);
        if ( FileIconInit == NULL )
        {
            systemServiceExceptionCode(NO_PROC_MSG, "FileIconInit", GetLastError());
        }

        FileIconInit(TRUE);
        fileIconInitDone = true;
    }

    if ( ! Shell_GetImageLists(&hLarge, &hSmall) )
    {
        systemServiceException(NO_IMAGELIST_MSG, setLarge ? "large" : "small");
    }

    REXX_SETVAR(IL_HANDLE_ATTR, setLarge ? ptrToRx(hLarge) : ptrToRx(hSmall));
    REXX_SETVAR(IL_IMAGETYPE_ATTR, IL_IMAGETYPE_ICON);

    return RexxNil;
}


/* class: ShellFileOp- - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\

\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

//                                  Attribute              Default        Meaning
//                                 ----------              -------       ----------
#define SFO_CONFIRM_ATTR          "CONFIRMATION"        // true          ! FOF_NOCONFIRMATION
#define SFO_CONFIRM_MKDIR_ATTR    "CONFIRMMKDIR"        // true          ! FOF_NOCONFIRMMKDIR
#define SFO_PROGRESS_ATTR         "PROGRESSGUI"         // true          ! FOF_SILENT
#define SFO_PROGRESS_SIMPLE_ATTR  "PROGRESSSIMPLEGUI"   // false         ! FOF_SIMPLEPROGRESS
#define SFO_ERRUR_ATTR            "ERRORGUI"            // true          ! FOF_NOERRORUI
#define SFO_RECURSIVE_ATTR        "RECURSIVE"           // true          ! FOF_NORECURSION
#define SFO_PERMANENT_ATTR        "PERMANENT"           // false           FOF_ALLOWUNDO

#define SFO_MULTIDESTINATION      "MULTIDESTINATION"    // True or false (set to true if 2nd item added toList)

#define SFO_FROMLIST_ATTR         "FROMLIST"
#define SFO_TOLIST_ATTR           "TOLIST"
#define SFO_FOFLAGS_ATTR          "FOFLAGS"             // If not .nil, use flags not attributes
#define SFO_OWNERWINDOW_ATTR      "OWNERWINDOW"

#define SFO_USERCANCELED_ATTR     "USERCANCELED"        // .nil before file op.
#define SFO_PROGRESSTITLE_ATTR    "PROGRESSDLGTITLE"    // .nil until user sets it.

/** ShellFileOp::init()
 *
 * Basic init for a ShellFileOp object.
 */
RexxMethod4(RexxObjectPtr, ShellFileOp_init, CSTRING, from, CSTRING, to, RexxObjectPtr, rxFlags,
            STRING, rxHwndOwner)
{
    CommonInit(true);

    // Set the from and to lists to the default of nil. Then the actual
    // arguments can simply be passed to the addTo function.
    REXX_SETVAR(SFO_FROMLIST_ATTR, RexxNil);
    REXX_SETVAR(SFO_TOLIST_ATTR, RexxNil);

    addToFileList(FROM_DIRECTION, from);
    addToFileList(TO_DIRECTION, to);

    // Set the default flag attributes.
    REXX_SETVAR(SFO_CONFIRM_ATTR, RexxTrue);
    REXX_SETVAR(SFO_CONFIRM_MKDIR_ATTR, RexxTrue);
    REXX_SETVAR(SFO_PROGRESS_ATTR, RexxTrue);
    REXX_SETVAR(SFO_PROGRESS_SIMPLE_ATTR, RexxFalse);
    REXX_SETVAR(SFO_ERRUR_ATTR, RexxTrue);
    REXX_SETVAR(SFO_RECURSIVE_ATTR, RexxTrue);
    REXX_SETVAR(SFO_PERMANENT_ATTR, RexxFalse);
    REXX_SETVAR(SFO_MULTIDESTINATION, RexxFalse);

    setFOFlags(rxFlags, true, 3);
    setOwnerWnd(rxHwndOwner, 4);

    // Set the default values for the other attributes.
    REXX_SETVAR(SFO_USERCANCELED_ATTR, RexxNil);
    REXX_SETVAR(SFO_PROGRESSTITLE_ATTR, RexxNil);

    return RexxNil;
}

RexxMethod3(RexxObjectPtr, ShellFileOp_delete, CSTRING, file, STRING, rxFlags, STRING, rxHwndOwner)
{
    return doTheOp(FO_DELETE, 2, file, NULLOBJECT, rxFlags, rxHwndOwner);
}


RexxMethod4(RexxObjectPtr, ShellFileOp_copy, CSTRING, from, CSTRING, to, STRING, rxFlags, STRING, rxHwndOwner)
{
    return doTheOp(FO_COPY, 3, from, to, rxFlags, rxHwndOwner);
}


RexxMethod4(RexxObjectPtr, ShellFileOp_move, CSTRING, from, CSTRING, to, STRING, rxFlags, STRING, rxHwndOwner)
{
    return doTheOp(FO_MOVE, 3, from, to, rxFlags, rxHwndOwner);
}


RexxMethod4(RexxObjectPtr, ShellFileOp_rename, CSTRING, from, CSTRING, to, STRING, rxFlags, STRING, rxHwndOwner)
{
    return doTheOp(FO_RENAME, 3, from, to, rxFlags, rxHwndOwner);
}


RexxMethod1(RexxObjectPtr, ShellFileOp_setFOFlags, RexxObjectPtr, rxFlags)
{
    RexxObjectPtr oldValue = REXX_GETVAR(SFO_FOFLAGS_ATTR);
    setFOFlags(rxFlags, true, 1);
    return oldValue;
}


/** ShellFileOp::setOwnerWnd()
 *
 *  Sets or clears the owner window handle.  The user can invoke setOwnerWnd()
 *  with no arg or with .nil to clear the current window handle.
 *
 *  If the arg is not .nil, then it is checked to determine that it is a valid
 *  window handle and an exception is raised if it is not.
 *
 */
RexxMethod1(RexxObjectPtr, ShellFileOp_setOwnerWnd, RexxObjectPtr, rxHwndOwner)
{
    RexxObjectPtr oldValue = REXX_GETVAR(SFO_OWNERWINDOW_ATTR);
    setOwnerWnd(rxHwndOwner, 1);
    return oldValue;
}


/** ShellFileOp::uninit()
 *
 *  The basic uninit for a ShellFileOp object.
 */
RexxMethod1(RexxObjectPtr, ShellFileOp_uninit, OSELF, self)
{
    CommonUninit(true);
    return RexxNil;
}


/**
 * Adds a file to either the from list or the to list.  These lists are
 * maintained with 2 trailing nulls to signal the end of the list and a single
 * null between items in the list.  Each item is a fully qualified path name.
 *
 * @param direction  Whether the file goes on the from or the to list.
 * @param rxFile     The path name to add.
 */
static void addToFileList(SFO_DIRECTION direction, CSTRING file)
{
    char inBuffer[MAX_PATH + 1];
    pathdup(inBuffer, file);

    char outBuffer[MAX_PATH + 2]

    if ( PathSearchAndQualify(inBuffer, outBuffer, MAX_PATH) )
    {
        /* TODO Function was reworked for new APIs, need to recheck this. */
        size_t l = strlen(outBuffer) + 1;
        *(outBuffer + l++) = '\0';
        *(outBuffer + l) = '\0';

        RexxObjectPtr  list = RexxNil;
        char       *newBuffer = NULL;

        if ( direction == FROM_DIRECTION )
        {
            list = REXX_GETVAR(SFO_FROMLIST_ATTR);
        }
        else
        {
            list = REXX_GETVAR(SFO_TOLIST_ATTR);
        }

        if ( list != RexxNil )
        {
            char   *oldList = string_data((RexxString *)list);
            size_t  oldL = string_length((RexxString *)list);

            if ( direction == TO_DIRECTION )
            {
                REXX_SETVAR(SFO_MULTIDESTINATION, RexxTrue);
            }
            // Not sure if the ooRexx kernel tacks on an extra trailing
            // null when a new string is created.  But, there will be room
            // for one in the buffer.
            newBuffer = (char *)GlobalAlloc(GMEM_FIXED, oldL + l);

            if ( newBuffer == NULL )
            {
                GlobalFree(outBuffer);
                systemServiceException(MEMORY_ALLOC_FAILED_MSG, NULL);
            }

            // oldList has 2 trailing nulls, there needs to only be 1 null
            // between items.
            oldL--;
            memcpy(newBuffer, oldList, oldL);
            memcpy((newBuffer + oldL), outBuffer, l);

            char *pTmp = outBuffer;
            l = oldL + l;
            outBuffer = newBuffer;

            GlobalFree(pTmp);
        }

        if ( direction == FROM_DIRECTION )
        {
            REXX_SETVAR(SFO_FROMLIST_ATTR, RexxStringL(outBuffer, l));
        }
        else
        {
            REXX_SETVAR(SFO_TOLIST_ATTR, RexxStringL(outBuffer, l));
        }
    }
    else
    {
        badArgException(1, QUALIFIED_PATH_MSG);
    }
}


static void setFOFlags(RexxObjectPtr rxFlags, bool nullResets, size_t argNumber)
{
    switch ( getRxArgType(rxFlags) )
    {
        case OMITTED_TYPE :
            if ( nullResets )
            {
                REXX_SETVAR(SFO_FOFLAGS_ATTR, RexxNil);
            }
            break;

        case NIL_TYPE :
            REXX_SETVAR(SFO_FOFLAGS_ATTR, RexxNil);
            break;

        case BOOLEAN_TYPE :
            if ( rxFlags == RexxTrue )
            {
                /** If this is a boolean it is the silent arg.  If true, there
                 * are no GUIs put up and no confirmation messages.  If false,
                 * there is no change.
                 */
                REXX_SETVAR(SFO_CONFIRM_ATTR, RexxFalse);
                REXX_SETVAR(SFO_CONFIRM_MKDIR_ATTR, RexxFalse);
                REXX_SETVAR(SFO_PROGRESS_ATTR, RexxFalse);
                REXX_SETVAR(SFO_ERRUR_ATTR, RexxFalse);
            }

            /** Use of the boolan silent arg should negate the use of the pure
             * FOF_XXXX flags.
             */
            REXX_SETVAR(SFO_FOFLAGS_ATTR, RexxNil);
            break;

        case STRING_TYPE :
            {
                RexxMethodContext *context;
                char *flags = strdupupr(context->StringData((RexxStringObject)rxFlags));
                int   foFlag, foFlags = 0;

                /** The user can separate tokens with either spaces, commas, or
                 *  the '|' symbol.
                 */
                char *token = strtok(flags, " ,|");
                while( token != NULL )
                {
                    foFlag = getConstantValue(token);
                    if ( foFlag == -1 )
                    {
                        invalidConstantException(argNumber, INVALID_CONSTANT_MSG, "FOF", token);
                    }
                    foFlags |= foFlag;
                    token = strtok(NULL, " ,|");
                }
                free(flags);

                REXX_SETVAR(SFO_FOFLAGS_ATTR, RexxInteger(foFlags));
            }
            break;

        default :
            wrongArgExceptionRx(argNumber, ".nil, .true, .false, or valid FOF_XXX flags", rxFlags);
            break;
    }
}


static FILEOP_FLAGS getFOFlags(void)
{
    FILEOP_FLAGS flags = 0;
    RexxObjectPtr rxFlags = REXX_GETVAR(SFO_FOFLAGS_ATTR);

    if ( rxFlags == RexxNil )
    {
        if ( REXX_GETVAR(SFO_CONFIRM_ATTR) == RexxFalse )
        {
            flags |= FOF_NOCONFIRMATION;
        }
        if ( REXX_GETVAR(SFO_CONFIRM_MKDIR_ATTR) == RexxFalse )
        {
            flags |= FOF_NOCONFIRMMKDIR;
        }
        if ( REXX_GETVAR(SFO_CONFIRM_MKDIR_ATTR) == RexxFalse )
        {
            flags |= FOF_NOCONFIRMMKDIR;
        }
        if ( REXX_GETVAR(SFO_PROGRESS_ATTR) == RexxFalse )
        {
            flags |= FOF_SILENT;
        }
        if ( REXX_GETVAR(SFO_PROGRESS_SIMPLE_ATTR) == RexxTrue )
        {
            flags |= FOF_SIMPLEPROGRESS;
        }
        if ( REXX_GETVAR(SFO_ERRUR_ATTR) == RexxFalse )
        {
            flags |= FOF_NOERRORUI;
        }
        if ( REXX_GETVAR(SFO_RECURSIVE_ATTR) == RexxFalse )
        {
            flags |= FOF_NORECURSION;
        }
        if ( REXX_GETVAR(SFO_PERMANENT_ATTR) == RexxFalse )
        {
            flags |= FOF_ALLOWUNDO;
        }
    }
    else
    {
        flags = (WORD)integer_value(rxFlags);
    }

    if ( REXX_GETVAR(SFO_MULTIDESTINATION) == RexxTrue )
    {
        flags |= FOF_MULTIDESTFILES;
    }

    return flags;
}


static void setOwnerWnd(RexxObjectPtr rxHwnd, size_t argNumber)
{
    switch ( getRxArgType(rxHwnd) )
    {
        case OMITTED_TYPE :
        case NIL_TYPE :
            REXX_SETVAR(SFO_OWNERWINDOW_ATTR, RexxNil);
            break;

        case STRING_TYPE :
            {
                /** getValidWindowHandle is just used to check that the value is
                 *  The attribute is set using the ooRexx string.  (An exception
                 *  is thrown if the hwnd is no good.)
                 */
                HWND hwnd = getValidWindowHandle(rxHwnd, argNumber);
                REXX_SETVAR(SFO_OWNERWINDOW_ATTR, rxHwnd);
            }
            break;

        default :
            wrongArgExceptionRx(argNumber, ".nil, or a valid window handle", rxHwnd);
            break;
    }
}


static HWND getOwnerWnd(void)
{
    HWND hwnd = NULL;
    RexxObjectPtr rxHwnd = REXX_GETVAR(SFO_OWNERWINDOW_ATTR);

    if ( rxHwnd != RexxNil )
    {
        // The window handle was good at the time the user set it.  It is
        // possbile that it is no longer good.  Should there be an exception
        // raised?  It won't actually do any harm to use a bad handle.  But
        // there could be some behavior that the user does not understand.
        hwnd = (HWND)handleFromRx(rxHwnd);
        if ( ! IsWindow(hwnd) )
        {
            systemServiceException(INVALID_WINDOW_MSG, NULL);
        }
    }
    return hwnd;
}


static RXARG_TYPES getRxArgType(RexxObjectPtr rxArg)
{
    RXARG_TYPES type = OMITTED_TYPE;

    if ( rxArg != NULLOBJECT )
    {
        if ( rxArg == RexxNil )
        {
            type = NIL_TYPE;
        }
        else if ( rxArg == RexxTrue || rxArg == RexxFalse )
        {
            type = BOOLEAN_TYPE;
        }
        else if ( _isstring(rxArg) )
        {
            type = STRING_TYPE;
        }
        else
        {
            type = OBJECT_TYPE;
        }
    }
    return type;
}


static RexxObjectPtr doTheOp(UINT op, size_t argNumber, CSTRING from, CSTRING to,
                          STRING rxFlags, STRING rxHwndOwner)
{
    SHFILEOPSTRUCT sfos = { 0 };

    addToFileList(FROM_DIRECTION, from);
    addToFileList(TO_DIRECTION, to);
    setFOFlags(rxFlags, false, argNumber);
    setOwnerWnd(rxHwndOwner, argNumber + 1);

    sfos.pFrom  = string_data(REXX_GETVAR(SFO_FROMLIST_ATTR));
    sfos.wFunc  = op;
    sfos.fFlags = getFOFlags();
    sfos.hwnd   = getOwnerWnd();

    sfos.lpszProgressTitle = getTitle(sfos.hwnd, sfos.fFlags);

    if ( op != FO_DELETE )
    {
        sfos.pTo = string_data(REXX_GETVAR(SFO_TOLIST_ATTR));
    }
    printf("FO flags: %x\n", sfos.fFlags);

    int rc = SHFileOperation(&sfos);
    RexxObjectPtr rxSuccess = (rc == 0) ? RexxTrue : RexxFalse;

    REXX_SETVAR(SFO_USERCANCELED_ATTR, sfos.fAnyOperationsAborted ? RexxTrue : RexxFalse);

    if ( rc != 0 )
    {
        // Don't clear the lists on failure.  That way the user can examine the
        // lists after an error.
        return RexxFalse;
    }

    // Clear the lists.
    REXX_SETVAR(SFO_FROMLIST_ATTR, RexxNil);
    REXX_SETVAR(SFO_TOLIST_ATTR, RexxNil);

    return rxSuccess;
}

inline char *getTitle(HWND hwnd, FILEOP_FLAGS flags)
{
    if ( hwnd != NULL && (flags & FOF_SIMPLEPROGRESS) && _isstring(REXX_GETVAR(SFO_PROGRESSTITLE_ATTR)) )
    {
        return string_data(REXX_GETVAR(SFO_PROGRESSTITLE_ATTR));
    }
    return NULL;
}

#define BROWSE_TITLE       "ooRexx Browse for Folder"
#define BROWSE_BANNER      "Select (or create) the folder you want"
#define BROWSE_HINT        "If the folder you want does not exist you can create it"
#define BROWSE_START_DIR   ""

/** SimpleFolderBrowse::init()
 *
 * Initializes a SimpleFolderBrowse object.  The title, banner, and hint strings
 * all have default values that are different from the Windows default values.
 * If the user wants to have the default Windows values, they specify that by
 * setting them to either .nil or the empty string.
 */
RexxMethod4(RexxObjectPtr, SimpleFolderBrowse_init,
            STRING,    title,
            STRING,    banner,
            STRING,    hint,
            STRING,    startDir)
{
	LPITEMIDLIST pidl = NULL;

    CommonInit(true);

    // Set the default attributes for the browser.
    setSFBAttribute("TITLE", title, BROWSE_TITLE);
    setSFBAttribute("BANNER", banner, BROWSE_BANNER);
    setSFBAttribute("HINT", hint, BROWSE_HINT);
    setSFBAttribute("INITIALFOLDER", startDir, BROWSE_START_DIR);

    if ( pidlForSpecialFolder(CSIDL_DRIVES, &pidl) )
    {
        REXX_SETVAR("ROOT", ptrToRx(pidl));
    }
    else
    {
        systemServiceException(NO_ITEMID_MSG, "CSIDL_DRIVES");
    }

    return RexxNil;
}


RexxMethod1(RexxObjectPtr, SimpleFolderBrowse_setRoot_private, RexxObjectPtr, rxObj)
{
    size_t source = 0;

    /* The possible pidl is required, but setting it to .nil or the empty string
     * is valid.
     */
    LPITEMIDLIST pidl = getPossiblePIDL(rxObj, 1, PPSF_ALL, &source);

    // If there is already a root pidl, not .nil, then free it.
    RexxObjectPtr rxObject = REXX_GETVAR("ROOT");
    if ( rxObject != RexxNil )
    {
        LPITEMIDLIST pidlRoot = (LPITEMIDLIST)ptrFromRx(rxObject);
        if ( pidlRoot != NULL )
        {
            CoTaskMemFree(pidlRoot);
        }
    }

    if ( pidl == NULL )
    {
        REXX_SETVAR("ROOT", RexxNil);
    }
    else
    {
        REXX_SETVAR("ROOT", ptrToRx(pidl));
    }
    return RexxNil;
}



/** SimpleFolderBrowse::getFolder()
 *
 * Puts up the customized Browse for Folder dialog using the atributes set for
 * this instance.  The optional arg is a window handle.  If specified that
 * window is used as the owner of the dialog.
 *
 * @param   rxHwnd  OPTIONAL  A window handle to be used as owner of the browse
 *          dialog.  If this arg is used, but is not a valid window handle, then
 *          an exception is raised.
 *
 * @return  Returns the fully qualified path of the folder the user selects, or
 *          the empty string if the user cancels.  In addition, if the user
 *          selects a virtual folder that has no file system path, .nil is
 *          returned.
 */
RexxMethod2(RexxObjectPtr, SimpleFolderBrowse_getFolder, OSELF, self, STRING, rxHwnd)
{
    BROWSEINFO   bi = { 0 };
    BROWSE_DATA  bd = { 0 };

    fillInBrowseData(rxHwnd, &bi, &bd);

    // Second arg is true - return path.
    return folderBrowse(&bi, true);
}


/** SimpleFolderBrowse::getItemID()
 *
 * Put up the Browse for Folder dialog and return the PIDL the user selects.
 *
 * @param   rxHwnd  OPTIONAL  If specified, use this window as the owner window
 * of the dialog.
 *
 * @return  Returns the pointer to item ID list the user selected, or the empty
 *          string if the user cancels the dialog.
 */
RexxMethod2(RexxObjectPtr, SimpleFolderBrowse_getItemID, OSELF, self, STRING, rxHwnd)
{
    BROWSEINFO   bi = { 0 };
    BROWSE_DATA  bd = { 0 };

    fillInBrowseData(rxHwnd, &bi, &bd);

    // Second arg is false - return PIDL, not path.
    return folderBrowse(&bi, false);
}


/** SimpleFolderBrowse::uninit()
 *
 * Does clean up for this SimpleFolderBrowse.  Frees the root PIDL and calls the
 * common uninit routine.
 */
RexxMethod1(RexxObjectPtr, SimpleFolderBrowse_uninit, OSELF, self)
{

    RexxObjectPtr rxObject = REXX_GETVAR("ROOT");
    if ( rxObject != RexxNil )
    {
        LPITEMIDLIST pidl = (LPITEMIDLIST)ptrFromRx(rxObject);
        if ( pidl != NULL )
        {
            CoTaskMemFree(pidl);
        }
    }

    CommonUninit(true);
    return RexxNil;
}


inline void setSFBAttribute(char * name, STRING userVal, char * defaultVal)
{
    if ( userVal == NULLOBJECT )
        REXX_SETVAR(name, RexxString(defaultVal));
    else if ( string_length(userVal) == 0 )
        REXX_SETVAR(name, RexxNil);
    else
        REXX_SETVAR(name, userVal);

}

/** Path::makePretty()
 *
 * Makes a path all lowercase characters so it has a consistent appearance.
 * This is a rather foolish function, it only changes the path if the whole path
 * is in uppercase.
 */
RexxMethod1(RexxStringObject, Path_makePretty, CSTRING, path)
{
    char inBuffer[MAX_PATH + 1];
    pathdup(inBuffer, path);

    PathMakePretty(inBuffer);
    return context->NewStringFromAsciiz(inBuffer);
}

/** Path::canonicalize()
 *
 * Resoloves the ..\ and .\ sequences in a path name.
 */
RexxMethod1(RexxStringObject, Path_canonicalize, CSTRING, path)
{
    char inBuffer[MAX_PATH + 1];
    pathdup(inBuffer, path);

    outBuffer[MAX_PATH];

    if ( PathCanonicalize(outBuffer, inBuffer) )
    {
        return context->NewStringFromAsciiz(outBuffer);
    }
    else
    {
        return context->NewStringFromAsciiz(inBuffer);
    }
}

/** Path::compact()  (PathCompactPathEx)
 *
 * Compacts a path name to the specified width.
 */
RexxMethod2(RexxStringObject, Path_compact, CSTRING, path, size_t, count)
{
    char inBuffer[MAX_PATH + 1];
    pathdup(inBuffer, path);

    outBuffer[MAX_PATH];

    if ( PathCompactPathEx(outBuffer, inBuffer, count, 0) )
    {
        return context->NewStringFromAsciiz(outBuffer);
    }
    else
    {
        return context->NewStringFromAsciiz(inBuffer);
    }
}

/** Path::searchAndQualify()
 *
 * Takes a path and returns a fully qualified path name.  Works for already
 * qualified path names, relative path names or single file names.  The returned
 * path does not necessarily exist.
 */
RexxMethod1(RexxStringObject, Path_searchAndQualify, CSTRING, path)
{
    char inBuffer[MAX_PATH + 1];
    pathdup(inBuffer, path);

    char outBuffer[MAX_PATH];

    // In testing, I don't ever see this fail.  Not sure what it takes to have
    // it return false.
    if ( PathSearchAndQualify(inBuffer, outBuffer, MAX_PATH) )
    {
        return context->NewStringFromAsciiz(outBuffer);
    }
    else
    {
        return context->NewStringFromAsciiz(inBuffer);
    }
}

/** Path::quoteSpaces()
 *
 *  Quotes a path if and only if it has spaces in it.
 */
RexxMethod1(RexxStringObject, Path_quoteSpaces, CSTRING, path)
{
    char inBuffer[MAX_PATH + 1];
    pathdup(inBuffer, path);

    PathQuoteSpaces(inBuffer);
    return context->NewStringFromAsciiz(inBuffer);
}

/** Path::unquoteSpaces()
 *
 *  Removes the quotes from a quoted path name.
 */
RexxMethod1(RexxStringObject, Path_unquoteSpaces, CSTRING, path)
{
    char inBuffer[MAX_PATH + 1];
    pathdup(inBuffer, path);

    PathUnquoteSpaces(inBuffer);
    return context->NewStringFromAsciiz(inBuffer);
}

/** Path::removeBackslash()
 *
 *  Removes the trailing backslash from a path if there is one.
 */
RexxMethod1(RexxStringObject, Path_removeBackslash, CSTRING, path)
{
    char inBuffer[MAX_PATH + 1];
    pathdup(inBuffer, path);

    PathRemoveBackslash(inBuffer);
    return context->NewStringFromAsciiz(inBuffer);
}

/** Path::addBackslash()
 *
 *  Ads the trailing backslash from a path if it needs it.
 */
RexxMethod1(RexxStringObject, Path_addBackslash, CSTRING, path)
{
    char inBuffer[MAX_PATH + 1];
    pathdup(inBuffer, path);

    PathAddBackslash(inBuffer);
    return context->NewStringFromAsciiz(inBuffer);
}

/** Path::removeFileSpec()
 *
 *  Removes the trailing file name and backslash from a path, if there it is
 *  there.
 */
RexxMethod1(RexxStringObject, Path_removeFileSpec, CSTRING, path)
{
    char inBuffer[MAX_PATH + 1];
    pathdup(inBuffer, path);

    PathRemoveFileSpec(inBuffer);
    return context->NewStringFromAsciiz(inBuffer);
}

/** Path::exists  (PathFileExists)
 *
 *  Returns true if the specified path name is valid otherwise false.  This
 *  works for both file name paths and directory paths on the local file system.
 *  It also works on network drives that are mounted under a drive letter.  It
 *  does not work for UNC path names.
 *
 *  Note that the Windows API is PathFileExists.  In this case the name is
 *  changed to avoid confusion in users that are unaware of the Windows API.
 */
RexxMethod1(logical_t, Path_exists, CSTRING, path)
{
    return PathFileExists(path) ? 1 : 0;
}

/** Path::isDirectory
 *
 *  Returns true if the specified path is a directory otherwise false.
 */
RexxMethod1(logical_t, Path_isDirectory, CSTRING, path)
{
    return PathIsDirectory(path) ? 1 : 0;
}

/** Path::isDirectoryEmpty
 *
 *  Returns true if the specified directory is empty otherwise false.
 */
RexxMethod1(logical_t, Path_isDirectoryEmpty, CSTRING, path)
{
    return PathIsDirectoryEmpty(path) ? 1 : 0;
}

/** Path::isRoot
 *
 *  Returns true if the specified path is a root path, otherwise returns false.
 */
RexxMethod1(logical_t, Path_isRoot, CSTRING, path)
{
    return PathIsRoot(path) ? 1 : 0;
}

/** Path::isFull
 *
 *  Returns true if the specified path is fully qualified, otherwise returns
 *  false.
 *
 *  Note that there is no corresponding lightweight Path function.  To be full,
 *  the path must be a UNC path name or a path name that includes the drive
 *  letter.  This is what is needed to be able to get a pointer to an item ID
 *  list from the desktop folder.  A path name of say, \Windows, is not
 *  sufficient.  It is necessary to use C:\Windows.
 */
RexxMethod1(logical_t, Path_isFull, STRING, path)
{
    return _PathIsFull(path) ? 1 : 0;
}

/** Path::isNetworkPath
 *
 *  Returns true if the specified path represents a network resource, otherwise
 *  returns false.
 */
RexxMethod1(logical_t, Path_isNetworkPath, STRING, path)
{
    return PathIsNetworkPath(path) ? 1 : 0;
}

/** Path::isUNC
 *
 *  Returns true if the specified path ?, otherwise
 *  returns false.
 */
RexxMethod1(logical_t, Path_isUNC, CSTRING, path)
{
    return PathIsUNC(path) ? 1 : 0;
}

/** Path::isUNCServer
 *
 *  Returns true if the specified path ?, otherwise
 *  returns false.
 */
RexxMethod1(logical_t, Path_isUNCServer, CSTRING, path)
{
    return PathIsUNCServer(path) ? 1 : 0;
}

/** Path::isUNCServerShare
 *
 *  Returns true if the specified path ?, otherwise
 *  returns false.
 */
RexxMethod1(logical_t, Path_isUNCServerShare, CSTRING, path)
{
    return PathIsUNCServerShare(path) ? 1 : 0;
}


/** Path::getShortPath
 *
 *  Given a long path name, converts and returns its short path name.
 */
RexxMethod1(CSTRING, Path_getShortPath, CSTRING, path)
{
    char pathBuffer[MAX_PATH];
    WCHAR wBuffer[MAX_PATH];

    if ( MultiByteToWideChar(CP_ACP, 0, path, -1, wBuffer, MAX_PATH) == 0 )
    {
        systemServiceException(UNICODE_ANSI_FAILED_MSG, NULL);
        return NULL;
    }

    PathGetShortPath(wBuffer);

    if ( WideCharToMultiByte(CP_ACP, 0, wBuffer, -1, pathBuffer, MAX_PATH, NULL, NULL) == 0)
    {
        systemServiceException(UNICODE_ANSI_FAILED_MSG, NULL);
        return NULL;
    }

    return context->NewStringFromAsciiz(pathBuffer);
}

/** Path::getLongPath
 *
 *  Given a short path name, converts and returns its long path name.
 */
RexxMethod1(RexxStringObject, Path_getLongPath, CSTRING, path)
{
    char buffer[MAX_PATH];

    GetLongPathName(path, buffer, MAX_PATH);
    return context->NewStringFromAsciiz(buffer);
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


#define WIN_SHELL_VERSION    "Windows Shell for ooRexx Version 1.0.0.0 "

/** Sh::version()
 *
 * Return the WinShell version.
 */
RexxMethod0(RexxObjectPtr, Sh_version_class)
{
    return RexxString(WIN_SHELL_VERSION);
}

/** Sh::is64Bit()
 *
 * Returns true if the current process is running in 64 bit mode, otherwise
 * false.
 */
RexxMethod0(RexxObjectPtr, Sh_is64Bit_class)
{
    if ( _is64Bit() )
        return RexxTrue;
    return RexxFalse;
}

/** Sh::is32on64Bit()
 *
 * Returns true if the current process is running in 32 bit mode on a 64 bit
 * operating system, otherwise false.
 */
RexxMethod0(RexxObjectPtr, Sh_is32on64Bit_class)
{
    if ( _is32on64Bit() )
        return RexxTrue;
    return RexxFalse;
}

/** Sh::isW2K()
 *
 * Returns true if the operating system is Windows 2000 (W2K), otherwise false.
 */
RexxMethod0(RexxObjectPtr, Sh_isW2K_class)
{
    if ( _isW2K() )
        return RexxTrue;
    return RexxFalse;
}

/** Sh::isAtLeastW2K()
 *
 * Returns true if the operating system is Windows 2000 (W2K), service pack 4,
 * or later. Otherwise returns false.
 */
RexxMethod0(RexxObjectPtr, Sh_isAtLeastW2K_class)
{
    if ( _isAtLeastW2K() )
        return RexxTrue;
    return RexxFalse;
}

/** Sh::isXP()
 *
 * Returns true if the operating system is Windows XP, otherwise false.
 */
RexxMethod0(RexxObjectPtr, Sh_isXP_class)
{
    if ( _isXP() )
        return RexxTrue;
    return RexxFalse;
}

/** Sh::isXP32()
 *
 * Returns true if the operating system is Windows XP 32 bit, otherwise false.
 */
RexxMethod0(RexxObjectPtr, Sh_isXP32_class)
{
    if ( _isXP32() )
        return RexxTrue;
    return RexxFalse;
}

/** Sh::isXP64()
 *
 * Returns true if the operating system is Windows XP 64 bit, otherwise false.
 */
RexxMethod0(RexxObjectPtr, Sh_isXP64_class)
{
    if ( _isXP64() )
        return RexxTrue;
    return RexxFalse;
}

/** Sh::isAtLeastXP()
 *
 * Returns true if the operating system is Windows XP, service pack 2, or later.
 * Otherwise returns false.
 */
RexxMethod0(RexxObjectPtr, Sh_isAtLeastXP_class)
{
    if ( _isAtLeastXP() )
        return RexxTrue;
    return RexxFalse;
}

/** Sh::isW2K3()
 *
 * Returns true if the operating system is Windows Server 2003 (W2K3), otherwise
 * false.
 */
RexxMethod0(RexxObjectPtr, Sh_isW2K3_class)
{
    if ( _isW2K3() )
        return RexxTrue;
    return RexxFalse;
}

/** Sh::isAtLeastW2K3()
 *
 * Returns true if the operating system is Windows Server 2003 (W2K3), service
 * pack 1, or later.  Otherwise returns false.
 */
RexxMethod0(RexxObjectPtr, Sh_isAtLeastW2K3_class)
{
    if ( _isAtLeastW2K3() )
        return RexxTrue;
    return RexxFalse;
}

/** Sh::isVista()
 *
 * Returns true if the operating system is Visata, otherwise false.
 */
RexxMethod0(RexxObjectPtr, Sh_isVista_class)
{
    if ( _isVista() )
        return RexxTrue;
    return RexxFalse;
}

/** Sh::isAtLeastVista()
 *
 * Returns true if the operating system is Vista, or later.  Otherwise returns
 * false.
 */
RexxMethod0(RexxObjectPtr, Sh_isAtLeastVista_class)
{
    if ( _isAtLeastVista() )
        return RexxTrue;
    return RexxFalse;
}

static HWND getValidWindowHandle(STRING rxHwnd, size_t argNumber)
{
    HWND hwnd = NULL;

    if ( rxHwnd != NULLOBJECT )
    {
        hwnd = (HWND)handleFromRx(rxHwnd);
        if ( ! IsWindow(hwnd) )
        {
            badArgException(argNumber, WINDOW_HANDLE_MSG);
        }
    }
    return hwnd;
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
static RexxObjectPtr folderBrowse(PBROWSEINFO pBI, bool returnPath)
{
	LPITEMIDLIST pidl = NULL;
    TCHAR        path[MAX_PATH];
    RexxObjectPtr   rxResult = RexxString("");    // User canceled.

    pidl = SHBrowseForFolder(pBI);
    if ( pidl != NULL )
    {
        if ( returnPath )
        {
            if ( SHGetPathFromIDList(pidl, path) )
                rxResult = RexxString(path);
            else
                rxResult = RexxNil;
            CoTaskMemFree(pidl);
        }
        else
        {
            rxResult = ptrToRx(pidl);
        }
    }
    return rxResult;
}

static void fillInBrowseData(STRING rxHwnd, PBROWSEINFO pBI, PBROWSE_DATA pBD)
{
	LPITEMIDLIST pidlRoot = NULL;
    PTCHAR       pHint;
    RexxObjectPtr   rxObject;

    pBI->hwndOwner = getValidWindowHandle(rxHwnd, 1);
    pBI->ulFlags  = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_RETURNFSANCESTORS;

    rxObject = REXX_GETVAR("TITLE");
    if ( rxObject != RexxNil )
        pBD->dlgTitle = string_data((RexxString *)rxObject);

    rxObject = REXX_GETVAR("HINT");
    if ( rxObject != RexxNil )
    {
        pBI->ulFlags |= BIF_UAHINT;
        pHint = string_data((RexxString *)rxObject);
        if ( _stricmp("PATH", pHint) == 0 )
        {
            pBD->usePathForHint = true;
            pBD->hint = BROWSE_HINT;
        }
        else
        {
            pBD->hint = pHint;
        }
    }

    rxObject = REXX_GETVAR("INITIALFOLDER");
    if ( rxObject != RexxNil )
        pBD->startDir = string_data((RexxString *)rxObject);

    // Note that banner goes into the BROWSEINFO structure, not the BROWSE_DATA
    // structure.
    rxObject = REXX_GETVAR("BANNER");
    if ( rxObject != RexxNil )
        pBI->lpszTitle = string_data((RexxString *)rxObject);

    rxObject = REXX_GETVAR("ROOT");
    if ( rxObject != RexxNil )
    {
        pBI->pidlRoot = (LPCITEMIDLIST)ptrFromRx(rxObject);
        if ( pBI->pidlRoot == NULL )
        {
            systemServiceException(STRING_TO_PTR_FAILED_MSG, NULL);
        }
    }

    pBI->lpfn   = BrowseCallbackProc;
    pBI->lParam = (LPARAM)pBD;
}

/**
 * The Browse for Folder dialog call back function used by SimpleBrowseFolder to
 * customize the appearance and behavior of the standard dialog.
 */
INT CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lp, LPARAM pData)
{
    TCHAR szDir[MAX_PATH];
    PBROWSE_DATA data = NULL;

    switch ( uMsg )
    {
        case BFFM_INITIALIZED:
            data = (PBROWSE_DATA)pData;

            if ( data->dlgTitle != NULL )
                SetWindowText(hwnd, data->dlgTitle);
            if ( data->hint != NULL )
                SetDlgItemText(hwnd, HINT_ID, data->hint);

            // WParam should be TRUE when passing a path and should be FALSE
            // when passing a pidl.
            if ( data->startDir != NULL )
                SendMessage(hwnd, BFFM_SETSELECTION, TRUE, (LPARAM)data->startDir);
            break;

        case BFFM_SELCHANGED:
            data = (PBROWSE_DATA)pData;

            // Set the hint static control to the currently selected path if
            // wanted.
            if ( data->usePathForHint )
            {
                if ( SHGetPathFromIDList((LPITEMIDLIST) lp ,szDir) )
                {
                    SetDlgItemText(hwnd, HINT_ID, szDir);
                }
            }
            break;
    }
    return 0;
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
 */
static bool pidlFromPath(LPCSTR path, LPITEMIDLIST *ppidl)
{
   LPSHELLFOLDER pShellFolder = NULL;
   HRESULT       hr;
   LPWSTR        wPath = ansiToUnicode(path, strlen(path) + 1);

   if ( ! wPath )
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
   shellFree(wPath);

   if (FAILED(hr))
   {
      *ppidl = NULL;  // Docs are unclear as to the value of this on failure.
      return false;
   }
   return true;
}

static bool pidlForSpecialFolder(int csidl, LPITEMIDLIST *ppidl)
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

inline LPWSTR ansiToUnicode(LPCSTR str, size_t inSize)
{
    return ansiToUnicodeLength(str, inSize, NULL);
}


/**
 * Convert an ANSI string to a wide character (Unicode) string.
 *
 * @param str      The string to convert, zero-length null-terminated strings
 *                 are acceptable.
 *
 * @param inSize   The size of the ANSI string, this should include the
 *                 null-terminator.  For a zero-length string this value would
 *                 be 1.
 *
 * @param outSize  If not null, the number of wide characters in the converted
 *                 Unicode string is returned here.
 *
 * @return  On success, a pointer to the allocated, converted string, otherwise
 *          null.  The caller is repsonsible for freeing this memory.
 */
static LPWSTR ansiToUnicodeLength(LPCSTR str, size_t inSize, size_t *outSize)
{
    size_t    newSize;
    LPWSTR    pStr = NULL;

    if ( (str == NULL) || (inSize == 0) )
    {
        return pStr;
    }

    newSize = MultiByteToWideChar(CP_ACP, 0, str, (int)inSize, NULL, 0);
    if ( newSize )
    {
        pStr = (LPOLESTR)shellAlloc(newSize * sizeof(WCHAR));
        if ( pStr )
        {
            newSize = MultiByteToWideChar(CP_ACP, 0, str, (int)inSize, pStr, (int)newSize);
            if ( newSize == 0 )
            {
                // Conversion failed.
                shellFree(pStr);
                pStr = NULL;
            }

            if ( outSize )
                *outSize = newSize;
        }
    }

    return pStr;
}


static bool _is32on64Bit(void)
{
    if ( _isAtLeastXP() )
    {
        BOOL isWow64 = FALSE;
        typedef BOOL (WINAPI *PFNISWOW)(HANDLE, PBOOL);

        PFNISWOW fnIsWow64Process = (PFNISWOW)GetProcAddress(GetModuleHandle("kernel32"), "IsWow64Process");

        if ( fnIsWow64Process != NULL)
        {
            if ( fnIsWow64Process(GetCurrentProcess(), &isWow64) && isWow64 )
            {
                return true;
            }
        }
    }
    return false;
}

static bool _isVersion(DWORD major, DWORD minor, unsigned int sp, unsigned int type, unsigned int condition)
{
    OSVERSIONINFOEX ver;
    DWORDLONG       mask = 0;
    DWORD           testForMask = VER_MAJORVERSION | VER_MINORVERSION;

    ZeroMemory(&ver, sizeof(OSVERSIONINFOEX));

    ver.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
    ver.dwMajorVersion = major;
    ver.dwMinorVersion = minor;

    VER_SET_CONDITION(mask, VER_MAJORVERSION, condition);
    VER_SET_CONDITION(mask, VER_MINORVERSION, condition);

    if ( condition != VER_EQUAL )
    {
        ver.wServicePackMajor = sp;
        testForMask |= VER_SERVICEPACKMAJOR;
        VER_SET_CONDITION(mask, VER_SERVICEPACKMAJOR, condition);
    }

    if ( type != 0 )
    {
        ver.wProductType = type;
        testForMask |= VER_PRODUCT_TYPE;
        VER_SET_CONDITION(mask, VER_PRODUCT_TYPE, condition);
    }

    if ( VerifyVersionInfo(&ver, testForMask, mask) )
        return true;
    else
        return false;
}


/**
 * Initializes the constant string to int value map.
 */
static void initMaps(void)
{
    ConstantsMap = new String2Int;

    // Map all the CSIDL constant strings to their int value.
    ConstantsMap->insert(String2Int::value_type("CSIDL_DESKTOP", 0x0000));
    ConstantsMap->insert(String2Int::value_type("CSIDL_INTERNET", 0x0001));
    ConstantsMap->insert(String2Int::value_type("CSIDL_PROGRAMS", 0x0002));
    ConstantsMap->insert(String2Int::value_type("CSIDL_CONTROLS", 0x0003));
    ConstantsMap->insert(String2Int::value_type("CSIDL_PRINTERS", 0x0004));
    ConstantsMap->insert(String2Int::value_type("CSIDL_PERSONAL", 0x0005));
    ConstantsMap->insert(String2Int::value_type("CSIDL_FAVORITES", 0x0006));
    ConstantsMap->insert(String2Int::value_type("CSIDL_STARTUP", 0x0007));
    ConstantsMap->insert(String2Int::value_type("CSIDL_RECENT", 0x0008));
    ConstantsMap->insert(String2Int::value_type("CSIDL_SENDTO", 0x0009));
    ConstantsMap->insert(String2Int::value_type("CSIDL_BITBUCKET", 0x000a));
    ConstantsMap->insert(String2Int::value_type("CSIDL_STARTMENU", 0x000b));
    ConstantsMap->insert(String2Int::value_type("CSIDL_MYDOCUMENTS", 0x000c));
    ConstantsMap->insert(String2Int::value_type("CSIDL_MYMUSIC", 0x000d));
    ConstantsMap->insert(String2Int::value_type("CSIDL_MYVIDEO", 0x000e));
    ConstantsMap->insert(String2Int::value_type("CSIDL_DESKTOPDIRECTORY", 0x0010));
    ConstantsMap->insert(String2Int::value_type("CSIDL_DRIVES", 0x0011));
    ConstantsMap->insert(String2Int::value_type("CSIDL_NETWORK", 0x0012));
    ConstantsMap->insert(String2Int::value_type("CSIDL_NETHOOD", 0x0013));
    ConstantsMap->insert(String2Int::value_type("CSIDL_FONTS", 0x0014));
    ConstantsMap->insert(String2Int::value_type("CSIDL_TEMPLATES", 0x0015));
    ConstantsMap->insert(String2Int::value_type("CSIDL_COMMON_STARTMENU", 0x0016));
    ConstantsMap->insert(String2Int::value_type("CSIDL_COMMON_PROGRAMS", 0x0017));
    ConstantsMap->insert(String2Int::value_type("CSIDL_COMMON_STARTUP", 0x0018));
    ConstantsMap->insert(String2Int::value_type("CSIDL_COMMON_DESKTOPDIRECTORY", 0x0019));
    ConstantsMap->insert(String2Int::value_type("CSIDL_APPDATA", 0x001a));
    ConstantsMap->insert(String2Int::value_type("CSIDL_PRINTHOOD", 0x001b));
    ConstantsMap->insert(String2Int::value_type("CSIDL_LOCAL_APPDATA", 0x001c));
    ConstantsMap->insert(String2Int::value_type("CSIDL_ALTSTARTUP", 0x001d));
    ConstantsMap->insert(String2Int::value_type("CSIDL_COMMON_ALTSTARTUP", 0x001e));
    ConstantsMap->insert(String2Int::value_type("CSIDL_COMMON_FAVORITES", 0x001f));
    ConstantsMap->insert(String2Int::value_type("CSIDL_INTERNET_CACHE", 0x0020));
    ConstantsMap->insert(String2Int::value_type("CSIDL_COOKIES", 0x0021));
    ConstantsMap->insert(String2Int::value_type("CSIDL_HISTORY", 0x0022));
    ConstantsMap->insert(String2Int::value_type("CSIDL_COMMON_APPDATA", 0x0023));
    ConstantsMap->insert(String2Int::value_type("CSIDL_WINDOWS", 0x0024));
    ConstantsMap->insert(String2Int::value_type("CSIDL_SYSTEM", 0x0025));
    ConstantsMap->insert(String2Int::value_type("CSIDL_PROGRAM_FILES", 0x0026));
    ConstantsMap->insert(String2Int::value_type("CSIDL_MYPICTURES", 0x0027));
    ConstantsMap->insert(String2Int::value_type("CSIDL_PROFILE", 0x0028));
    ConstantsMap->insert(String2Int::value_type("CSIDL_SYSTEMX86", 0x0029));
    ConstantsMap->insert(String2Int::value_type("CSIDL_PROGRAM_FILESX86", 0x002a));
    ConstantsMap->insert(String2Int::value_type("CSIDL_PROGRAM_FILES_COMMON", 0x002b));
    ConstantsMap->insert(String2Int::value_type("CSIDL_PROGRAM_FILES_COMMONX86", 0x002c));
    ConstantsMap->insert(String2Int::value_type("CSIDL_COMMON_TEMPLATES", 0x002d));
    ConstantsMap->insert(String2Int::value_type("CSIDL_COMMON_DOCUMENTS", 0x002e));
    ConstantsMap->insert(String2Int::value_type("CSIDL_COMMON_ADMINTOOLS", 0x002f));
    ConstantsMap->insert(String2Int::value_type("CSIDL_ADMINTOOLS", 0x0030));
    ConstantsMap->insert(String2Int::value_type("CSIDL_CONNECTIONS", 0x0031));
    ConstantsMap->insert(String2Int::value_type("CSIDL_COMMON_MUSIC", 0x0035));
    ConstantsMap->insert(String2Int::value_type("CSIDL_COMMON_PICTURES", 0x0036));
    ConstantsMap->insert(String2Int::value_type("CSIDL_COMMON_VIDEO", 0x0037));
    ConstantsMap->insert(String2Int::value_type("CSIDL_RESOURCES", 0x0038));
    ConstantsMap->insert(String2Int::value_type("CSIDL_RESOURCES_LOCALIZED", 0x0039));
    ConstantsMap->insert(String2Int::value_type("CSIDL_COMMON_OEM_LINKS", 0x003a));
    ConstantsMap->insert(String2Int::value_type("CSIDL_CDBURN_AREA", 0x003b));
    ConstantsMap->insert(String2Int::value_type("CSIDL_COMPUTERSNEARME", 0x003d));
    ConstantsMap->insert(String2Int::value_type("CSIDL_FLAG_CREATE", 0x8000));
    ConstantsMap->insert(String2Int::value_type("CSIDL_FLAG_DONT_VERIFY", 0x4000));
    ConstantsMap->insert(String2Int::value_type("CSIDL_FLAG_NO_ALIAS", 0x1000));
    ConstantsMap->insert(String2Int::value_type("CSIDL_FLAG_PER_USER_INIT", 0x0800));
    ConstantsMap->insert(String2Int::value_type("CSIDL_FLAG_MASK", 0xFF00));

    // Flags for SHFILEOPSTRUCT
    ConstantsMap->insert(String2Int::value_type("FOF_MULTIDESTFILES", 0x0001));
    ConstantsMap->insert(String2Int::value_type("FOF_CONFIRMMOUSE", 0x0002));
    ConstantsMap->insert(String2Int::value_type("FOF_SILENT", 0x0004));
    ConstantsMap->insert(String2Int::value_type("FOF_RENAMEONCOLLISION", 0x0008));
    ConstantsMap->insert(String2Int::value_type("FOF_NOCONFIRMATION", 0x0010));
    ConstantsMap->insert(String2Int::value_type("FOF_WANTMAPPINGHANDLE", 0x0020));
    ConstantsMap->insert(String2Int::value_type("FOF_ALLOWUNDO", 0x0040));
    ConstantsMap->insert(String2Int::value_type("FOF_FILESONLY", 0x0080));
    ConstantsMap->insert(String2Int::value_type("FOF_SIMPLEPROGRESS", 0x0100));
    ConstantsMap->insert(String2Int::value_type("FOF_NOCONFIRMMKDIR", 0x0200));
    ConstantsMap->insert(String2Int::value_type("FOF_NOERRORUI", 0x0400));
    ConstantsMap->insert(String2Int::value_type("FOF_NOCOPYSECURITYATTRIBS", 0x0800));
    ConstantsMap->insert(String2Int::value_type("FOF_NORECURSION", 0x1000));
    ConstantsMap->insert(String2Int::value_type("FOF_NO_CONNECTED_ELEMENTS", 0x2000));
    ConstantsMap->insert(String2Int::value_type("FOF_WANTNUKEWARNING", 0x4000));
    ConstantsMap->insert(String2Int::value_type("FOF_NORECURSEREPARSE", 0x8000));

    // List View Image List types
    ConstantsMap->insert(String2Int::value_type("LVSIL_NORMAL", 0x0000));
    ConstantsMap->insert(String2Int::value_type("LVSIL_SMALL", 0x0001));
    ConstantsMap->insert(String2Int::value_type("LVSIL_STAT", 0x0002));

    // Image List Create flags
    ConstantsMap->insert(String2Int::value_type("ILC_MASK", 0x0001));
    ConstantsMap->insert(String2Int::value_type("ILC_COLOR", 0x0000));
    ConstantsMap->insert(String2Int::value_type("ILC_COLORDDB", 0x00FE));
    ConstantsMap->insert(String2Int::value_type("ILC_COLOR4", 0x0004));
    ConstantsMap->insert(String2Int::value_type("ILC_COLOR8", 0x0008));
    ConstantsMap->insert(String2Int::value_type("ILC_COLOR16", 0x0010));
    ConstantsMap->insert(String2Int::value_type("ILC_COLOR24", 0x0018));
    ConstantsMap->insert(String2Int::value_type("ILC_COLOR32", 0x0020));
    ConstantsMap->insert(String2Int::value_type("ILC_PALETTE", 0x0800));
    ConstantsMap->insert(String2Int::value_type("ILC_MIRROR", 0x2000));
    ConstantsMap->insert(String2Int::value_type("ILC_PERITEMMIRROR", 0x8000));
}

/**
 * Look up the int value of a string.
 */
inline int getConstantValue(const char * str)
{
    String2Int::iterator itr;
    itr = ConstantsMap->find(str);
    if ( itr != ConstantsMap->end() )
        return itr->second;

    return -1;
}


// Below here are, commented out, saved functions.

#if defined(UNUSED_FUNCTIONS)

// I don't really think we need a DllMain
#ifdef __cplusplus
extern "C" {
#endif

bool APIENTRY DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
   if (fdwReason == DLL_PROCESS_ATTACH)
   {
      printf("DLL process attach Thread: %x\n", GetCurrentThreadId());
   }
   else if (fdwReason == DLL_PROCESS_DETACH)
   {
      printf("DLL process detach\n");
   }
   return true;
}

#ifdef __cplusplus
}
#endif


// The start of a customizable Browse for Folder dialog implemente as an
// external function.  Using the ShellSimpleBrowse is much easier to keep track
// of settings, etc.  Providing this function as an external function does not
// seem to have any real advantages.

// Would need this to go into the ShellUtilsTable:

//    BROWSEDIALOG_FUNC,

#define BROWSEDIALOG_FUNC         "BrowseDialog"

// And this to go into the exports section of the *.def file.

//  BROWSEDIALOG       =   BrowseDialog

/** BrowseDialog()
 *    Puts up the common Browse for Folder dialog to allow the user to select
 *    a directory.  The dialog is customized according to input and can be used
 *    to return either the path name of the selected folder or the pointer to
 *    the item ID list.
 *
 *  Syntax in ooRexx:
 *    ret = BrowseDialog(type, hwnd, banner, dlgTitle, hint, initialDir, root)
 *
 * argv[0]  type        I | P  Return item ID pointer or path name.
 *
 * argv[1]  hwnd        Handle of the parent window.
 *
 * argv[2]  banner      The text above the tree control
 *
 * argv[3]  dlgTitle    A custom title for the dialog.
 *
 * argv[4]  hint        Text for the hint.
 *
 * argv[5]  initialDir  Unfold the tree to start at this node.
 *
 * argv[6]  root        Make this the root of the tree.
 *
 * Returns to ooRexx:
 *
 *       0  User canceled.
 *    path  If arg 1 is P
 *    PIDL  If arg 1 is I
 *      -1  Invalid argument.
 *   other  ?
 */
APIRET APIENTRY BrowseDialog(
    const char     *funcName,
    const size_t    argc,
    const RXSTRING  argv[],
    const char     *qName,
    PRXSTRING       retStr)
{
    int code = 0;
	LPITEMIDLIST pidl     = NULL;
	LPITEMIDLIST pidlRoot = NULL;
	BROWSEINFO   bi       = { 0 };
    BROWSE_DATA  bd       = { 0 };
    TCHAR        buffer[MAX_PATH];
    BOOL         success = FALSE;
    bool         returnPath;

    if ( argc < 1 )
    {
        return codeToRexx(-1, retStr);
    }

    if ( argv[0].strptr[0] == 'I' )
    {
        returnPath = false;
    }
    else if ( argv[0].strptr[0] == 'P' )
    {
        returnPath = true;
    }
    else
    {
        return codeToRexx(-1, retStr);
    }

    pidl = (LPITEMIDLIST)pointerFromRexx(argv[2]);
    printf("pointerFromRexx using %s pidl: %p\n", argv[2].strptr, pidl);

    printf("Browse count: %d\n", argc);
    for ( size_t i = 0; i < argc; i++ )
    {
        printf("Arg %d len: %d value: %s\n", i, argv[i].strlength, argv[i].strptr);
    }

	//bi.hwndOwner      = hwnd;
	bi.hwndOwner      = NULL;
	bi.pszDisplayName = buffer;
	//bi.pidlRoot       = pidlRoot;
	bi.pidlRoot       = NULL;
	//bi.lpszTitle      = szTitle;
    //bi.lpfn           = BrowseCallbackProc;
	//bi.ulFlags        = BIF_RETURNONLYFSDIRS | BIF_USENEWUI;
	//bi.ulFlags        = BIF_RETURNONLYFSDIRS | BIF_UAHINT | BIF_NEWDIALOGSTYLE;
	bi.ulFlags        = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_RETURNFSANCESTORS;

    pidl = SHBrowseForFolder(&bi);
	if ( pidl != NULL )
	{
        printf("Path from displayName buffer: %s\n", buffer);
		success = SHGetPathFromIDList(pidl, buffer);
        printf("Buffer after convert: %s\n", buffer);
		CoTaskMemFree(pidl);
	}



    if ( success )
    {
        sprintf(retStr->strptr, "%s", buffer);
        retStr->strlength = strlen(retStr->strptr);
    }
    else
    {
        sprintf(retStr->strptr, "%d", code);
        retStr->strlength = strlen(retStr->strptr);
    }

    return 0;
}

/**
 * This is a simplier pidlFromPath() function, but will only work on XP or
 * later.
 */
static bool pidlFromPath(LPCSTR path, LPITEMIDLIST *ppidl)
{
   HRESULT  hr;
   LPWSTR   p = ansiToUnicode(path, strlen(path) + 1);

   if ( ! p )
   {
       return false;
   }

   hr = SHParseDisplayName(p, NULL, ppidl, NULL, NULL);
   shellFree(p);

   if (FAILED(hr))
   {
      return false;
   }
   return true;
}

#endif
