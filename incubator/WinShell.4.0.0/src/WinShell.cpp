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

#include "WinShell.hpp"
#include "WinShellResources.h"

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
static RexxObjectPtr folderBrowse(RexxMethodContext *, PBROWSEINFO, bool);
LPITEMIDLIST         getPidlFromString(RexxMethodContext *, CSTRING, int, bool);
LPITEMIDLIST         getPidlFromObject(RexxMethodContext *, RexxObjectPtr obj, int);
static bool          pidlFromPath(LPCSTR, LPITEMIDLIST *);
static bool          pidlForSpecialFolder(int, LPITEMIDLIST *);
static RexxObjectPtr extractAllDefIcons(RexxMethodContext *, CSTRING);
inline HIMAGELIST    _getSelf(RexxMethodContext *);
static unsigned int  getImageListFlags(RexxMethodContext *, CSTRING, int);
static int           getListViewImageListType(RexxMethodContext *, CSTRING);
static logical_t setSystemImageList(RexxMethodContext *, bool);
inline void          setSFBAttribute(RexxMethodContext *, char *, CSTRING, char *);
static bool          checkOptionalWindow(RexxMethodContext *, HWND, int);
static void          fillInBrowseData(RexxMethodContext *, HWND, PBROWSEINFO, PBROWSE_DATA);
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

/**
 * 93.914
 * Method argument <argument> must be one of <values>; found "<value>"
 *
 * Method argument 1 must be one of array of icon handles or count of icons;
 * found "o"
 *
 * @param argNumber
 * @param acceptable
 * @param actual
 */
void wrongArgValueException(RexxMethodContext *c, int pos, const char *list, RexxObjectPtr actual)
{
    RexxArrayObject a = c->NewArray(3);
    c->ArrayPut(a, c->NewInteger(pos), 1);
    c->ArrayPut(a, c->NewStringFromAsciiz(list), 2);
    c->ArrayPut(a, actual, 3);

    c->RaiseExceptionArray(Rexx_Error_Incorrect_method_list, a);
}
void wrongArgValueException(RexxMethodContext *c, int pos, const char *list, const char *actual)
{
    wrongArgValueException(c, pos, list, c->NewStringFromAsciiz(actual));
}
inline void *wrongClassException(RexxMethodContext *c, int pos, const char *n)
{
    c->RaiseException2(Rexx_Error_Incorrect_method_noclass, c->NewInteger(pos), c->NewStringFromAsciiz(n));
    return NULL;
}

bool requiredClass(RexxMethodContext *context, RexxObjectPtr obj, const char *name, int argPos)
{
    RexxClassObject rxClass = context->FindContextClass(name);
    if ( ! context->IsInstanceOf(obj, rxClass) )
    {
        wrongClassException(context, argPos, name);
        return false;
    }
    return true;
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
 *  End of functions copied from ooDialog.  Start of functions to maybe move to
 *  common.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/**
 * 93.914
 * Method argument <argument> must be one of <values>; found "<value>"
 *
 * Method argument 1 must be one of the valid CSIDL_XXX constants; found "dog"
 *
 * @param argNumber
 * @param acceptable
 * @param actual
 */
void invalidConstantException(RexxMethodContext *c, int argNumber, char *msg,
                              const char *sub, RexxObjectPtr actual)
{
    TCHAR buffer[32];
    _snprintf(buffer, sizeof(buffer), msg, sub);

    wrongArgValueException(c, argNumber, buffer, actual);
}
void invalidConstantException(RexxMethodContext *c, int argNumber, char *msg, const char *sub, const char *actual)
{
    invalidConstantException(c, argNumber, msg, sub, c->NewStringFromAsciiz(actual));
}

bool checkOptionalWindow(RexxMethodContext *context, HWND hwnd, int argNumber)
{
    if ( hwnd != NULL && ! IsWindow(hwnd) )
    {
        badArgException(context, argNumber, WINDOW_HANDLE_MSG);
        return false;
    }
    return true;
}

/**
 * Methods for a .Size class.
 */
RexxMethod2(RexxObjectPtr, size_init, OPTIONAL_int32_t,  cx, OPTIONAL_int32_t, cy)
{
    RexxBufferObject obj = context->NewBuffer(sizeof(SIZE));
    context->SetObjectVariable("CSELF", obj);

    SIZE *s = (SIZE *)context->BufferData(obj);

    s->cx = rxArgExists(context, 1) ? cx : 0;
    s->cy = rxArgExists(context, 2) ? cy : s->cx;

    return NULLOBJECT;
}

RexxMethod1(int32_t, size_cx, CSELF, s) { return ((SIZE *)s)->cx; }
RexxMethod1(int32_t, size_cy, CSELF, s) { return ((SIZE *)s)->cy; }
RexxMethod2(RexxObjectPtr, size_setCX, CSELF, s, int32_t, cx) { ((SIZE *)s)->cx = cx; return NULLOBJECT; }
RexxMethod2(RexxObjectPtr, size_setCY, CSELF, s, int32_t, cy) { ((SIZE *)s)->cy = cy; return NULLOBJECT; }

PSIZE rxGetSize(RexxMethodContext *context, RexxObjectPtr s, int argPos)
{
    if ( requiredClass(context, s, "Size", argPos) )
    {
        return (PSIZE)context->ObjectToCSelf(s);
    }
    return NULL;
}

RexxObjectPtr rxNewSize(RexxMethodContext *context, long cx, long cy)
{
    RexxObjectPtr size = NULL;
    RexxClassObject SizeClass = context->FindContextClass("SIZE");
    if ( SizeClass != NULL )
    {
        size = context->SendMessage2(SizeClass, "NEW", context->NewInteger(cx), context->NewInteger(cy));
    }
    return size;
}


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\
 *  End of functions to move to common.
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
bool CommonInit(RexxMethodContext *context, bool useCOM)
{
    if ( (InterlockedIncrementRelease(&globalInstances) == 1) )
    {
        if ( ! _isAtLeastW2K() )
        {
            systemServiceException(context, REQUIRE_W2K_MSG, NULL);
            return false;
        }
    }

    if ( InterlockedIncrementRelease(&threadInstances) == 1 )
    {
        HRESULT  hr;
        INITCOMMONCONTROLSEX ctrlex;

        thisModule = GetModuleHandle(SHELL_DLL);
        if ( thisModule == NULL )
        {
            systemServiceExceptionCode(context, NO_HMODULE_MSG, SHELL_DLL);
            return false;
        }

        ctrlex.dwSize = sizeof(ctrlex);
        ctrlex.dwICC = ICC_WIN95_CLASSES | ICC_STANDARD_CLASSES;
        if ( ! InitCommonControlsEx(&ctrlex) )
        {
            systemServiceExceptionCode(context, NO_COMMCTRL_MSG, "Common Control Library");
            return false;
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
    return true;
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
    CommonInit(context, true);
    return NULLOBJECT;
}

/** WinShell::uninit()
 *
 *  The basic uninit for the object.
 */
RexxMethod1(RexxObjectPtr, WinShell_uninit, OSELF, self)
{
    CommonUninit(true);
    return NULLOBJECT;
}


/** WinShell::browseForFolder()
 *
 * Put up a plain Browse for Folder dialog and return the full qualified path of
 * the folder the user selects.
 *
 * @param   owner  OPTIONAL
 *   If specified, use this window as the owner window of the dialog.  This will
 *   disable the owner window until the user closes the dialog (which is usually
 *   what is wanted.)
 *
 * @return  Returns the fully qualified path of the folder the user selected, or
 *          the empty string if the user cancels the dialog.
 */
RexxMethod1(RexxObjectPtr, WinShell_browseForFolder, OPTIONAL_POINTER, owner)
{
	LPITEMIDLIST   pidlRoot = NULL;
    BROWSEINFO     bi = { 0 };
    RexxObjectPtr  rxResult = context->Nil();

    if ( ! checkOptionalWindow(context, (HWND)owner, 1) )
    {
        return rxResult;
    }

    if ( pidlForSpecialFolder(CSIDL_DRIVES, &pidlRoot) )
    {
        bi.hwndOwner = (HWND)owner;
        bi.pidlRoot  = pidlRoot;
        bi.ulFlags   = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_RETURNFSANCESTORS;

        rxResult = folderBrowse(context, &bi, true);
        CoTaskMemFree(pidlRoot);
    }
    else
    {
        systemServiceException(context, NO_ITEMID_MSG, "CSIDL_DRIVES");
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
RexxMethod2(RexxObjectPtr, WinShell_pathFromCSIDL, CSTRING, csidlConstant, OPTIONAL_logical_t, create)
{
	LPITEMIDLIST  pidl = NULL;
    RexxObjectPtr rxResult = context->Nil();

    char *pCsidlConstant = strdupupr(csidlConstant);
    int csidl = getConstantValue(pCsidlConstant);
    free(pCsidlConstant);

    if ( csidl == -1 )
    {
        invalidConstantException(context, 1, INVALID_CONSTANT_MSG, "CSIDL", csidlConstant);
        return rxResult;
    }

    if ( rxArgExists(context, 2) && create == TRUE )
    {
        csidl |= CSIDL_FLAG_CREATE;
    }

    if ( pidlForSpecialFolder(csidl, &pidl) )
    {
        TCHAR path[MAX_PATH];
        if ( SHGetPathFromIDList(pidl, path) )
        {
            rxResult = context->NewStringFromAsciiz(path);
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
RexxMethod1(RexxObjectPtr, WinShell_pathFromItemID, POINTER, idlPtr)
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
RexxMethod1(POINTER, WinShell_getItemID, CSTRING, name)
{
	return getPidlFromString(context, name, 1, true);
}


/** WinShell::getItemIDFromPath()
 *
 *  Obtains a pointer to an item ID list for the specified path.  This function
 *  will accept any string without checking that the string is in the format of
 *  a valid path name.
 *
 */
RexxMethod1(POINTER, WinShell_getItemIDFromPath, CSTRING, path)
{
	LPITEMIDLIST pidl = NULL;
	pidlFromPath(path, &pidl);
    return pidl;
}


/** WinShell::getItemIDFromCSIDL()
 *
 *  Obtains a pointer to an item ID list for the specified CSIDL constant.  This
 *  function will accept any string without checking that the string is in the
 *  format of a valid CSIDL constant.
 *
 *  @param    str  The CSIDL string.
 *  @return   pidl A ooRexx pointer object.  This may be the null pointer
 *            object.
 */
RexxMethod1(POINTER, WinShell_getItemIDFromCSIDL, CSTRING, str)
{
	LPITEMIDLIST pidl = NULL;

    int csidl = getConstantValue(str);
    if ( csidl != -1 )
    {
        pidlForSpecialFolder(csidl, &pidl);
    }
	return pidl;
}

/** WinShell::releaseItemID()
 *
 *  @return  This method does not return any value.
 *
 *  @note    CoTaskMemFree() handles null pointers.
 */
RexxMethod1(RexxObjectPtr, WinShell_releaseItemID, POINTER, idlPtr)
{
    CoTaskMemFree(idlPtr);
    return NULLOBJECT;
}


/** WinShell::openFindFiles()
 *
 *  Opens the
 *
 *  @return  The window handle of the find files dialog, or the negated system
 *           error code if the window handle could not be located.
 */
RexxMethod2(RexxObjectPtr, WinShell_openFindFiles, RexxObjectPtr, start, OPTIONAL_RexxObjectPtr, saved)
{
    RexxObjectPtr rxResult = context->Nil();
    bool folderWasPtr = false;
    bool savedWasPtr = false;
    LPITEMIDLIST pidlSaved = NULL;

    LPITEMIDLIST pidlFolder = getPidlFromObject(context, start, 1);
    if ( pidlFolder == NULL )
    {
        rxResult;
    }

    if ( context->IsPointer(start) )
    {
        folderWasPtr = true;
    }

    if ( saved != NULL )
    {
        // If the user passed in an arg, an exception is raised if it is not good.
        pidlSaved = getPidlFromObject(context, saved, 2);
        if ( pidlSaved == NULL )
        {
            return rxResult;
        }
        if ( context->IsPointer(saved) )
        {
            savedWasPtr = true;
        }
    }

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
        rxResult = context->NewPointer(hwnd);
    }
    else
    {
        rxResult = context->NewInteger(-(long)GetLastError());
    }

    if ( ! folderWasPtr )
    {
        CoTaskMemFree(pidlFolder);
    }
    if ( pidlSaved != NULL && ! savedWasPtr )
    {
        CoTaskMemFree(pidlSaved);
    }
    return rxResult;
}


/** WinShell::openFolder()
 *
 *  Intended for this API: SHOpenFolderAndSelectItems ??
 */
RexxMethod1(RexxObjectPtr, WinShell_openFolder, RexxObjectPtr, rxFolder)
{
    context->RaiseException(Rexx_Error_Unsupported_method);
    return NULLOBJECT;
}


/** WinShell::closeWindow()
 *
 */
RexxMethod1(uint32_t, WinShell_closeWindow, POINTER, hwnd)
{
    if ( ! SendNotifyMessage((HWND)hwnd, WM_SYSCOMMAND, SC_CLOSE, 0) )
    {
        return GetLastError();
    }
    return 0;
}


/** WinShell::clearRecentDocuments()
 *
 *  Clears the user's list of recent documents.
 *
 *  @return  This method does not return a value.
 *
 *  @note    SHAddToRecentDocs has no return code.
 */
RexxMethod0(RexxObjectPtr, WinShell_clearRecentDocuments)
{
    SHAddToRecentDocs(SHARD_PATH, NULL);
    return NULLOBJECT;
}


/** WinShell::addToRecentDocuments()
 *
 *  Adds a document to the user's list of recent documents.
 *
 *  @param  doc  Either a string with the path and file name of the document, or
 *               a PIDL that identifies the document's file object.  Note that
 *               when using a path, it must be a fully qualified path.
 *
 *  @return  This method does not return a value.
 *
 *  @note    SHAddToRecentDocs has no return code.
 *
 *           Passing in a null pointer object has the same effect as using the
 *           WinShell::clearRecentDocuments() method.
 */
RexxMethod1(RexxObjectPtr, WinShell_addToRecentDocuments, RexxObjectPtr, doc)
{
    if ( context->IsString(doc) )
    {
        const char *path = context->StringData((RexxStringObject)doc);
        if ( _PathIsFull(path) )
        {
            SHAddToRecentDocs(SHARD_PATH, path);
        }
        else
        {
            wrongFormatException(context, 1, doc);
        }
    }
    else if ( context->IsPointer(doc) )
    {
        LPITEMIDLIST pidl = (LPITEMIDLIST)context->PointerValue((RexxPointerObject)doc);
        SHAddToRecentDocs(SHARD_PIDL, pidl);
    }
    else
    {
        wrongFormatException(context, 1, doc);
    }

    return NULLOBJECT;
}


/** WinShell::_queryDiskSpace()  <private>
 *                                        TODO redo this function now that it is
 *                                        easy to return ooRexx objects.
 *
 *  Note that rxPath is not omitted (can not be omitted) because use strict arg
 *  is done in WinShell.cls.
 */
RexxMethod1(RexxObjectPtr, WinShell_queryDiskSpace_private, CSTRING, rxPath)
{
    ULARGE_INTEGER userFree, total, totalFree;
    TCHAR buffer[64];

    if ( SHGetDiskFreeSpaceEx(rxPath, &userFree, &total, &totalFree) )
    {
        _snprintf(buffer, 64, "%I64d %I64d %I64d", total, totalFree, userFree);
    }
    else
    {
        _snprintf(buffer, 64, "Err: %d", GetLastError());
    }
    return context->NewStringFromAsciiz(buffer);
}


/** WinShell::_queryRecycleBin()  <private>
 *                                               TODO redo this function.
 *  Determines the size (in bytes) and number of items in the Recycle Bin.  This
 *  can be either for all Recycle Bins, or the Recycle Bin a specific drive.
 *
 *  Note that rxPath does not need to be checked for an omitted argument because
 *  use strict arg is done in WinShell.cls.
 */
RexxMethod1(RexxObjectPtr, WinShell_queryRecycleBin_private, CSTRING, root)
{
    DWORDLONG bytes = 0, items = 0;
    HRESULT   hr;
    TCHAR     buffer[64];

    hr = queryTrashCan(root, &bytes, &items);
    if ( hr == S_OK )
    {
        _snprintf(buffer, 64, "%I64u %I64u", bytes, items);
    }
    else
    {
        _snprintf(buffer, 64, "Err: 0x%08x", hr);
    }
    return context->NewStringFromAsciiz(buffer);
}


/** WinShell::emptyRecycleBin()
 *
 *  Empties the recycle bin, either for a specific drive, or for all drives.
 *
 */
RexxMethod3(RexxObjectPtr, WinShell_emptyRecycleBin, OPTIONAL_CSTRING, root, OPTIONAL_logical_t, confirm,
            OPTIONAL_POINTER, owner)
{
    // The flags for no confirmation, which is the defualt.
    DWORD       flags = SHERB_NOCONFIRMATION | SHERB_NOPROGRESSUI | SHERB_NOSOUND;
    HRESULT     hr;
    RexxObjectPtr  rxResult = context->NewInteger(0);

    if ( root == NULL )
    {
        root = "";
    }
    if ( strlen(root) > 0 )
    {
        if ( ! _PathIsFull(root) )
        {
            wrongArgValueException(context, 1, "omitted, the empty string, or a fully qualified path name", root);
            return rxResult;
        }
    }

    if ( rxArgExists(context, 2) && confirm == FALSE )
    {
        flags = 0;
    }

    if ( ! checkOptionalWindow(context, (HWND)owner, 3) )
    {
        return rxResult;
    }

    if ( recycleBinIsEmpty(root) )
    {
        if ( flags == 0 )
        {
            ShellMessageBox(NULL, (HWND)owner, "The Recycle Bin is empty.",
                            (owner == NULL ? "ooRexx Windows Shell - Information" : NULL),
                            MB_TASKMODAL | MB_OK | MB_ICONINFORMATION);
        }
    }
    else
    {

        hr = SHEmptyRecycleBin((HWND)owner, root, flags);
        if ( hr != S_OK )
        {
            rxResult = hrToRx(context, hr);
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
RexxMethod2(POINTER, WinShell_getSharedIcon, CSTRING, rxPath, RexxObjectPtr, rxIndex )
{
    /**
     * Want to be able to load a: system icon, icon from this module, icon from
     * a resource only DLL loaded by say, ooDialog, and an icon from some other
     * loaded extension DLL, for instance ooDialog.dll.  So have a hModule arg
     * that can also be a keyword (like self) for this DLL, null for system DLL,
     * or actual hModule.
     */
    HICON hIcon = NULL;
    return hIcon;
}


/** WinShell::extractDefaultIcons()
 *
 *
 *
 *
 */
RexxMethod2(RexxObjectPtr, WinShell_extractDefaultIcons, CSTRING, path, OPTIONAL_int32_t, index)
{
    RexxObjectPtr rxResult;

    /**
     * If rxIndex is omitted, then an array of all the icons in the specified
     * file are returned.
     */
    if ( rxArgOmitted(context, index) )
    {
        return extractAllDefIcons(context, path);
    }

    HICON hSmall, hBig;

    HRESULT hr = SHDefExtractIcon(path, index, 0, &hBig, &hSmall, 0);
    if ( FAILED(hr) )
    {
        rxResult = hrToRx(context, hr);
    }
    else
    {
        rxResult = context->ArrayOfTwo(context->NewPointer(hBig), context->NewPointer(hSmall));
    }

    return rxResult;
}

static RexxObjectPtr extractAllDefIcons(RexxMethodContext *context, const char *path)
{
    RexxObjectPtr rxResult = context->Nil();

    /* Get the number of icons in the file. */
    unsigned int count = ExtractIconEx(path, -1, NULL, NULL, 0);

    if ( count > 0 )
    {
        HICON *pLarge = (HICON *)shellAlloc(count * sizeof(HICON *));
        HICON *pSmall = (HICON *)shellAlloc(count * sizeof(HICON *));

        if ( pLarge == NULL || pSmall == NULL )
        {
            systemServiceException(context, MEMORY_ALLOC_FAILED_MSG, NULL);
            return rxResult;
        }

        count = ExtractIconEx(path, 0, pLarge, pSmall, count);

        if ( count > 0 )
        {
            RexxArrayObject rxLarge = context->NewArray(count);
            RexxArrayObject rxSmall = context->NewArray(count);
            for ( unsigned int i = 0; i < count; i++ )
            {
                context->ArrayPut(rxLarge, context->NewPointer(pLarge[i]), i + 1);
                context->ArrayPut(rxSmall, context->NewPointer(pSmall[i]), i + 1);
            }
            rxResult = context->ArrayOfTwo(rxLarge, rxSmall);
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
RexxMethod2(RexxObjectPtr, WinShell_selectIcon, CSTRING, path, OPTIONAL_POINTER, owner)
{
    RexxObjectPtr rxResult = context->Nil();

    char pathBuffer[MAX_PATH];
    WCHAR wBuffer[MAX_PATH];

    if ( MultiByteToWideChar(CP_ACP, 0, path, -1, wBuffer, MAX_PATH) == 0 )
    {
        systemServiceException(context, UNICODE_ANSI_FAILED_MSG, NULL);
        return rxResult;
    }

    if ( ! checkOptionalWindow(context, (HWND)owner, 2) )
    {
        return rxResult;
    }
    int iconIndex = 0;

    if ( PickIconDlg((HWND)owner, wBuffer, MAX_PATH, &iconIndex) == 1 )
    {
        if ( WideCharToMultiByte(CP_ACP, 0, wBuffer, -1, pathBuffer, MAX_PATH, NULL, NULL) == 0)
        {
            systemServiceException(context, UNICODE_ANSI_FAILED_MSG, NULL);
            return rxResult;
        }

        rxResult = context->ArrayOfTwo(context->NewInteger(iconIndex), context->NewStringFromAsciiz(pathBuffer));
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
RexxMethod1(RexxObjectPtr, WinShell_releaseIcon, POINTER, iconPtr)
{
    HICON hIcon = (HICON)iconPtr;
    RexxObjectPtr rxResult = context->NewInteger(0);

    if ( hIcon != 0 )
    {
        if ( DestroyIcon(hIcon) == 0 )
        {
            rxResult = context->NewInteger(GetLastError());
        }
    }
    else
    {
        badArgException(context, 1, "icon handle");
    }

    return rxResult;
}


/** WinShell::about()
 *
 */
RexxMethod5(logical_t, WinShell_about, CSTRING, title, OPTIONAL_CSTRING, _app,
            OPTIONAL_CSTRING, other, OPTIONAL_POINTER, hwnd, OPTIONAL_POINTER, iconPtr)
{
    char        buffer[128];
    const char *app = NULL;

    if ( strlen(title) > 63 )
    {
        stringTooLongException(context, 1, 63, strlen(title));
        return FALSE;
    }

    if ( _app == NULLOBJECT )
    {
        app = title;
    }
    else
    {
        if ( strlen(_app) > 63 )
        {
            stringTooLongException(context, 2, 63, strlen(_app));
            return FALSE;
        }
        _snprintf(buffer, sizeof(buffer), "%s#%s", title, _app);
        app = buffer;
    }

    // We don't enforce a limit on the other string, but if the programmer makes
    // it too long it will be truncated.  hwnd, other, and icon can be null.

    if ( ShellAbout((HWND)hwnd, app, other, (HICON)iconPtr) )
    {
        return TRUE;
    }
    return FALSE;
}


/** WinShell::test()
 *
 * A temporary method of this WinShell.  Provides a simple way to try various
 * things from the ooRexx side.
 */
RexxMethod2(uint32_t, WinShell_test, OSELF, self, OPTIONAL_POINTER, hwnd)
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
    return 0;
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
#define IL_IMAGETYPE_ICON      0
#define IL_IMAGETYPE_BITMAP    1
#define IL_DEFAULT_CX          32
#define IL_DEFAULT_CY          32
#define IL_DEFAULT_FLAGS       ILC_COLOR32 | ILC_MASK
#define IL_DEFAULT_GROW        0


/** ImageList::init()
 *
 *  Initializes a new ImageList object.
 *
 *
 */
RexxMethod5(RexxObjectPtr, ImageList_init, CSTRING, type, RexxObjectPtr, rxImages, OPTIONAL_RexxObjectPtr, sizePtr,
            OPTIONAL_CSTRING, _flags, OPTIONAL_uint32_t, grow)
{
    bool isBitmap = false;
    bool createEmptyImageList = true;

    if ( ! CommonInit(context, false) )
    {
        return NULLOBJECT;
    }

    /** The type determines how the image list is created.  Some keywords
     *  indicate a short cut creation.  (And some keywords are not yet
     *  implemented, like creating duplicating an existing image list.)
     */
    if ( _stricmp("SYSTEMLARGE", type) == 0 )
    {
        setSystemImageList(context, true);
        return NULLOBJECT;
    }
    else if ( _stricmp("SYSTEMSMALL", type) == 0 )
    {
        setSystemImageList(context, true);
        return NULLOBJECT;
    }
    else if ( _stricmp("ICON", type) == 0 )
    {
        context->SetObjectVariable(IL_IMAGETYPE_ATTR, context->NewInteger(IL_IMAGETYPE_ICON));
    }
    else if ( _stricmp("BITMAP", type) == 0 )
    {
        context->SetObjectVariable(IL_IMAGETYPE_ATTR, context->NewInteger(IL_IMAGETYPE_BITMAP));
        isBitmap = true;
    }
    else
    {
        /** There are some other types to be implemented later.  Need to
         *  figure out how to do them.  Like ImageList_Duplicate,
         *  ImageList_LoadImage, ImageList_Merge, etc..
         */
        wrongArgValueException(context, 1, "SystemLarge, SystemSmall, Icon, or Bitmap", type);
        return NULLOBJECT;
    }

    /** If we are here, rxImages is required. */
    if ( rxImages == NULLOBJECT )
    {
        context->RaiseException1(Rexx_Error_Invalid_argument_noarg, context->NewInteger(2));
        return NULLOBJECT;
    }

    /* Check the optional args first. */
    int cx = IL_DEFAULT_CX;
    int cy = IL_DEFAULT_CY;

    if ( rxArgExists(context, 3) )
    {
        SIZE *size = rxGetSize(context, sizePtr, 3);
        if ( size == NULL )
        {
            return NULLOBJECT;
        }
        cx = size->cx;
        cy = size->cy;
    }

    if ( rxArgOmitted(context, 5) )
    {
        grow = IL_DEFAULT_GROW;
    }

    unsigned int flags = IL_DEFAULT_FLAGS;
    if ( _flags != NULL )
    {
        flags = getImageListFlags(context, _flags, 5);
        /** TODO should check here for ILC_PERITEMMIRROR and ILC_MIRROR,
         *  these flags are only available with Comctl32.dll version 6.
         */
    }

    /** rxImages can either be the count (size) of an empty image list to
     *  create, or it can be an array of image handles.  (Icons or bitmaps.)
     *  Anything else, at this time, is wrong.
     */
    int count;
    if ( context->IsInteger(rxImages) )
    {
        count = (int)context->IntegerValue((RexxIntegerObject)rxImages);
    }
    else if ( context->IsArray(rxImages) )
    {
        count = (int)context->ArraySize((RexxArrayObject)rxImages);
        createEmptyImageList = false;
    }
    else
    {
        wrongArgValueException(context, 2, "an array of image handles or a count of images", rxImages);
        return NULLOBJECT;
    }

    // It seems as though some APIs do not clear last error.
    SetLastError(0);

    HIMAGELIST himl = ImageList_Create(cx, cy, flags, count, grow);
    if ( himl == NULL )
    {
        systemServiceExceptionCode(context, NO_CREATE_MSG, "image list");
        return NULLOBJECT;
    }

    if ( ! createEmptyImageList )
    {
        VOID *image;
        int index;
        for ( int i = 1; i <= count; i++ )
        {
            image = context->ArrayAt((RexxArrayObject)rxImages, i);

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
    context->SetObjectVariable(IL_HANDLE_ATTR, context->NewPointer(himl));

    return NULLOBJECT;
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
    return NULLOBJECT;
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
RexxMethod0(logical_t, ImageList_release)
{
    HIMAGELIST himl = _getSelf(context);
    if ( himl == NULL )
    {
        return FALSE;
    }

    if ( ImageList_Destroy(himl) != 0 )
    {
        context->SetObjectVariable(IL_HANDLE_ATTR, context->NewPointer(NULL));
        return TRUE;
    }
    return FALSE;
}


/** ImageList::getBkColor()
 *
 *  Return the current background color for this image list.  The value is a
 *  COLORREF, for convenience the value can be returned split into its RGB
 *  values.
 */
RexxMethod1(RexxObjectPtr, ImageList_getBkColor, OPTIONAL_logical_t, useRGB)
{
    TCHAR buffer[32];
    HIMAGELIST himl = _getSelf(context);
    if ( himl == NULL )
    {
        return FALSE;
    }


    COLORREF ref = ImageList_GetBkColor(himl);

    if ( rxArgExists(context, 1) && useRGB == TRUE )
    {
        _snprintf(buffer, sizeof(buffer), "%d %d %d %08x",
                  GetRValue(ref), GetGValue(ref), GetBValue(ref), ref);
    }
    else
    {
        _snprintf(buffer, sizeof(buffer), "%08x", ref);
    }
    return context->NewStringFromAsciiz(buffer);
}


/** ImageList::setBkColor()
 *
 *  Sets the current background color for this image list.  The value is
 *  specified as a COLORREF.  The previous background color is returned.
 */
RexxMethod1(RexxObjectPtr, ImageList_setBkColor, OPTIONAL_logical_t, doRGB)
{
    TCHAR buffer[32];

    HIMAGELIST himl = _getSelf(context);
    if ( himl == NULL )
    {
        return FALSE;
    }
    COLORREF lastRef = ImageList_SetBkColor(himl, CLR_NONE);

    // TODO this function was never implemented correctly.

    if ( rxArgExists(context, 1) && doRGB == TRUE )
    {
        _snprintf(buffer, sizeof(buffer), "%d %d %d %08x",
                  GetRValue(lastRef), GetGValue(lastRef), GetBValue(lastRef), lastRef);
    }
    else
    {
        _snprintf(buffer, sizeof(buffer), "%08x", lastRef);
    }
    return context->NewStringFromAsciiz(buffer);
}


/** ImageList::getImageCount()
 *
 *  Return the number of images in this list.
 */
RexxMethod0(int32_t, ImageList_getImageCount)
{
    HIMAGELIST himl = _getSelf(context);
    if ( himl == NULL )
    {
        return 0;
    }
    return ImageList_GetImageCount(himl);
}


/** ImageList::getImageSize()  <ImageList_GetIconSize>
 *
 *  Return the width and height of images in this list.  All images in an image
 *  list have the same width and height.
 *                                         TODO return a .Size
 *  Returns .nil on error, otherwise returns a string with two tokens.  The
 *  first is the width (cx) and the second is the height (cy.)
 */
RexxMethod0(RexxObjectPtr, ImageList_getImageSize)
{
    HIMAGELIST himl = _getSelf(context);
    if ( himl == NULL )
    {
        return 0;
    }

    RexxObjectPtr rxResult = context->Nil();
    int cx, cy;

    if ( ImageList_GetIconSize(himl, &cx, &cy) )
    {
        TCHAR buffer[32];
        _snprintf(buffer, sizeof(buffer), "%d %d", cx, cy);
        rxResult = context->NewStringFromAsciiz(buffer);
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
RexxMethod2(RexxObjectPtr, ImageList_setListViewImages, POINTER, rxListView, OPTIONAL_CSTRING, size )
{
    int flag = LVSIL_NORMAL;
    HIMAGELIST himl = _getSelf(context);
    if ( himl == NULL )
    {
        return NULLOBJECT;
    }

    HWND hwnd = (HWND)rxListView;
    if ( ! IsWindow(hwnd) )
    {
        badArgException(context, 1, WINDOW_HANDLE_MSG);
        return NULLOBJECT;
    }


    if ( size != NULLOBJECT )
    {
        flag = getListViewImageListType(context, size);
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
    return context->NewPointer(oldImages);
}

inline HIMAGELIST _getSelf(RexxMethodContext *c)
{
    HIMAGELIST himl = (HIMAGELIST)c->PointerValue((RexxPointerObject)c->GetObjectVariable(IL_HANDLE_ATTR));
    if ( himl == NULL )
    {
        c->RaiseException1(Rexx_Error_Execution_user_defined, c->NewStringFromAsciiz(IMAGELIST_RELEASED_MSG));
    }
    return himl;
}

static unsigned int getImageListFlags(RexxMethodContext *context, const char *_flags, int argNumber)
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
            invalidConstantException(context, argNumber, INVALID_CONSTANT_MSG, "ILC", token);
            return -1;
        }
        ilcFlags |= flag;
        token = strtok(NULL, " ,|");
    }
    free(flags);

    /** If flag is still -1 then the string was empty. */
    if ( flag == -1 )
    {
        wrongArgValueException(context, argNumber, "the valid ILC_XXX constants", _flags);
        return -1;
    }

    return ilcFlags;
}


static int getListViewImageListType(RexxMethodContext *context, CSTRING size)
{
    int flag = -1;

    if ( _strnicmp("LVS", size, 3) == 0 )
    {
        flag = getConstantValue(size);
    }
    else
    {
        if ( _stricmp("NORMAL", size) == 0 )
        {
            flag = LVSIL_NORMAL;
        }
        else if ( _stricmp("SMALL", size) == 0 )
        {
            flag = LVSIL_SMALL;
        }
        else if ( _stricmp("STATE", size) == 0 )
        {
            flag = LVSIL_STATE;
        }
    }

    if ( flag == -1 )
    {
        wrongArgValueException(context, 1, "a valid LVSIL_XXX constant, Normal, Small, or State", size);
    }
    return flag;
}


static logical_t setSystemImageList(RexxMethodContext * context, bool setLarge)
{
    HIMAGELIST hLarge = NULL;
    HIMAGELIST hSmall = NULL;

    /* FileIconInit() only needs to be called one time per process. */
    if ( ! fileIconInitDone )
    {
        HMODULE hmod = LoadLibrary("shell32.dll");
        if ( hmod == NULL )
        {
            systemServiceExceptionCode(context, NO_HMODULE_MSG, "shell32.dll");
            return FALSE;
        }

        FILEICONINIT_PROC FileIconInit = (FILEICONINIT_PROC)GetProcAddress(hmod, (LPCSTR)FILEICONINIT_ORD);
        if ( FileIconInit == NULL )
        {
            systemServiceExceptionCode(context, NO_PROC_MSG, "FileIconInit");
            return FALSE;
        }

        FileIconInit(TRUE);
        fileIconInitDone = true;
    }

    if ( ! Shell_GetImageLists(&hLarge, &hSmall) )
    {
        systemServiceException(context, NO_IMAGELIST_MSG, setLarge ? "large" : "small");
        return FALSE;
    }

    context->SetObjectVariable(IL_HANDLE_ATTR,
                               setLarge ? context->NewPointer(hLarge) : context->NewPointer(hSmall));

    context->SetObjectVariable(IL_IMAGETYPE_ATTR, context->NewInteger(IL_IMAGETYPE_ICON));

    return TRUE;
}


/** class: ShellFileOp
 *
 * Instances of ShellFileOp are used to perform graphical shell file operations.
 * These operations can provide graphical confirmation and or progress dialogs.
 * In addition, the shell file operations are the only way to send items to the
 * trash can which gives the user the ability to undo deletes.
 *
 * The Windows shell provides a large number of options to how graphical file
 * operations can be done. These options are controlled by attributes of the
 * object.  The user of this class can either accept a predefined set of
 * attributes or specify exactly what they want by directly setting the FOF_XXX
 * flags.
 *
 * Using the predefined set of attributes allows for either a silent operation
 * or not silent.
 */



//                                  Attribute              Default        Meaning
//                                 ----------              -------       ----------
#define SFO_CONFIRM_ATTR          "CONFIRMATION"        // true          ! FOF_NOCONFIRMATION
#define SFO_CONFIRM_MKDIR_ATTR    "CONFIRMMKDIR"        // true          ! FOF_NOCONFIRMMKDIR
#define SFO_PROGRESS_ATTR         "PROGRESSGUI"         // true          ! FOF_SILENT
#define SFO_PROGRESS_SIMPLE_ATTR  "PROGRESSSIMPLEGUI"   // false         ! FOF_SIMPLEPROGRESS
#define SFO_ERRORUI_ATTR          "ERRORGUI"            // true          ! FOF_NOERRORUI
#define SFO_RECURSIVE_ATTR        "RECURSIVE"           // true          ! FOF_NORECURSION
#define SFO_PERMANENT_ATTR        "PERMANENT"           // false           FOF_ALLOWUNDO

#define SFO_MULTIDESTINATION      "MULTIDESTINATION"    // True or false (set to true if 2nd item added toList)

#define SFO_FROMLIST_ATTR         "FROMLIST"
#define SFO_TOLIST_ATTR           "TOLIST"
#define SFO_FOFLAGS_ATTR          "FOFLAGS"             // If not .nil, use flags not attributes
#define SFO_OWNERWINDOW_ATTR      "OWNERWINDOW"

#define SFO_USERCANCELED_ATTR     "USERCANCELED"        // .nil before file op.
#define SFO_PROGRESSTITLE_ATTR    "PROGRESSDLGTITLE"    // .nil until user sets it.

// Function prototypes for the ShellFileOp helper functions.
static bool          setTitle(RexxMethodContext *, RexxObjectPtr, int);
static bool          addToFileList(RexxMethodContext *, SFO_DIRECTION, CSTRING, int);
static bool          setFOFlags(RexxMethodContext *, RexxObjectPtr, int);
static FILEOP_FLAGS  getFOFlags(RexxMethodContext *);
static bool          setOwnerWnd(RexxMethodContext *, RexxObjectPtr, int);
static bool          getOwnerWnd(RexxMethodContext *, HWND *);
static logical_t     doTheOp(RexxMethodContext *, UINT, CSTRING, CSTRING, RexxObjectPtr, RexxObjectPtr, int);
inline CSTRING       getTitle(RexxMethodContext *, HWND, FILEOP_FLAGS);

/** ShellFileOp::init()
 *
 * Basic init for a ShellFileOp object.
 */
RexxMethod5(RexxObjectPtr, ShellFileOp_init, OPTIONAL_CSTRING, from, OPTIONAL_CSTRING, to, OPTIONAL_RexxObjectPtr, rxFlags,
            OPTIONAL_RexxObjectPtr, rxHwndOwner, OPTIONAL_RexxObjectPtr, rxTitle)
{
    if ( ! CommonInit(context, true) )
    {
        return NULLOBJECT;
    }

    // Set the from and to lists to the default of nil. Then the actual
    // arguments can simply be passed to the addTo function.
    context->SetObjectVariable(SFO_FROMLIST_ATTR, context->Nil());
    context->SetObjectVariable(SFO_TOLIST_ATTR, context->Nil());

    if ( ! addToFileList(context, FROM_DIRECTION, from, 1) )
    {
        return NULLOBJECT;
    }
    if ( ! addToFileList(context, TO_DIRECTION, to, 2) )
    {
        return NULLOBJECT;
    }

    // Set the default flag attributes.
    context->SetObjectVariable(SFO_CONFIRM_ATTR, context->True());
    context->SetObjectVariable(SFO_CONFIRM_MKDIR_ATTR, context->True());
    context->SetObjectVariable(SFO_PROGRESS_ATTR, context->True());
    context->SetObjectVariable(SFO_PROGRESS_SIMPLE_ATTR, context->False());
    context->SetObjectVariable(SFO_ERRORUI_ATTR, context->True());
    context->SetObjectVariable(SFO_RECURSIVE_ATTR, context->True());
    context->SetObjectVariable(SFO_PERMANENT_ATTR, context->False());
    context->SetObjectVariable(SFO_MULTIDESTINATION, context->False());
    context->SetObjectVariable(SFO_FOFLAGS_ATTR, context->Nil());

    // Set the default values for the other attributes.
    context->SetObjectVariable(SFO_USERCANCELED_ATTR, context->Nil());
    context->SetObjectVariable(SFO_PROGRESSTITLE_ATTR, context->Nil());

    if ( ! setFOFlags(context, rxFlags, 3) )
    {
        return NULLOBJECT;
    }
    if ( ! setOwnerWnd(context, rxHwndOwner, 4) )
    {
        return NULLOBJECT;
    }
    setTitle(context, rxTitle, 5);

    return NULLOBJECT;
}

/**
 * The following functions initiate the actual file operation.  All parameters
 * are optional and can be used to over-ride the current setttings of this
 * object.  Except for the to and from file options, which do not over-ride, but
 * rather are appended to the current lists.
 */


/** ShellFileOp::delete()
 */
RexxMethod3(logical_t, ShellFileOp_delete, OPTIONAL_CSTRING, file, OPTIONAL_RexxObjectPtr, rxFlags,
            OPTIONAL_RexxObjectPtr, rxHwndOwner)
{
    return doTheOp(context, FO_DELETE, file, NULLOBJECT, rxFlags, rxHwndOwner, 2);
}

/** ShellFileOp::copy()
 */
RexxMethod4(logical_t, ShellFileOp_copy, OPTIONAL_CSTRING, from, OPTIONAL_CSTRING, to,
            OPTIONAL_RexxObjectPtr, rxFlags, OPTIONAL_RexxObjectPtr, rxHwndOwner)
{
    return doTheOp(context, FO_COPY, from, to, rxFlags, rxHwndOwner, 3);
}

/** ShellFileOp::move()
 */
RexxMethod4(logical_t, ShellFileOp_move, OPTIONAL_CSTRING, from, OPTIONAL_CSTRING, to,
            OPTIONAL_RexxObjectPtr, rxFlags, OPTIONAL_RexxObjectPtr, rxHwndOwner)
{
    return doTheOp(context, FO_MOVE, from, to, rxFlags, rxHwndOwner, 3);
}

/** ShellFileOp::rename()
 */
RexxMethod4(logical_t, ShellFileOp_rename, OPTIONAL_CSTRING, from, OPTIONAL_CSTRING, to,
            OPTIONAL_RexxObjectPtr, rxFlags, OPTIONAL_RexxObjectPtr, rxHwndOwner)
{
    return doTheOp(context, FO_RENAME, from, to, rxFlags, rxHwndOwner, 3);
}

/**
 * The following functions are used to set the true / false attributes.
 */

/** ShellFileOp::confirmation=
 */
RexxMethod1(RexxObjectPtr, ShellFileOp_setConfirmation, logical_t, confirm)
{
    context->SetObjectVariable(SFO_CONFIRM_ATTR, (confirm ? context->True() : context->False()));
    return NULLOBJECT;
}

/** ShellFileOp::confirmMkDir=
 */
RexxMethod1(RexxObjectPtr, ShellFileOp_setConfirmMkDir, logical_t, set)
{
    context->SetObjectVariable(SFO_CONFIRM_MKDIR_ATTR, (set ? context->True() : context->False()));
    return NULLOBJECT;
}

/** ShellFileOp::progressGUI=
 */
RexxMethod1(RexxObjectPtr, ShellFileOp_setProgressGUI, logical_t, set)
{
    context->SetObjectVariable(SFO_PROGRESS_ATTR, (set ? context->True() : context->False()));
    return NULLOBJECT;
}

/** ShellFileOp::progressSimpleGUI=
 */
RexxMethod1(RexxObjectPtr, ShellFileOp_setProgressSimpleGUI, logical_t, set)
{
    context->SetObjectVariable(SFO_PROGRESS_SIMPLE_ATTR, (set ? context->True() : context->False()));
    return NULLOBJECT;
}

/** ShellFileOp::errorGUI=
 */
RexxMethod1(RexxObjectPtr, ShellFileOp_setErrorGUI, logical_t, set)
{
    context->SetObjectVariable(SFO_ERRORUI_ATTR, (set ? context->True() : context->False()));
    return NULLOBJECT;
}

/** ShellFileOp::recursive=
 */
RexxMethod1(RexxObjectPtr, ShellFileOp_setRecursive, logical_t, set)
{
    context->SetObjectVariable(SFO_RECURSIVE_ATTR, (set ? context->True() : context->False()));
    return NULLOBJECT;
}

/** ShellFileOp::permanent=
 */
RexxMethod1(RexxObjectPtr, ShellFileOp_setPermanent, logical_t, set)
{
    context->SetObjectVariable(SFO_PERMANENT_ATTR, (set ? context->True() : context->False()));
    return NULLOBJECT;
}

/** ShellFileOp::setFOFlags()
 *
 *  Sets or clears the FOF_xxx flags.  If the FOFlags attribute is .nil then the
 *  file operation options are determined from the FOF attributes (like
 *  recursive, etc..)  If not .nil then the value of this attribute is the true
 *  value to use for the fFlags member of the SHFILEOPSTRUCT struct.
 *
 *  The user clears this attribute by using no arg or .nil for the arg.  In
 *  addition, the user can send an arg of .true to get the default setting for a
 *  silent operation or .false to get the default setting for not silent.
 *
 *  @return  The old value of the FOFlags attribute is returned.
 *
 *  @note Exceptions are raised if the user sends the wrong arg or uses an
 *        invalid FOF_xxx flag.
 */
RexxMethod1(RexxObjectPtr, ShellFileOp_setFOFlags, OPTIONAL_RexxObjectPtr, rxFlags)
{
    RexxObjectPtr oldValue = context->GetObjectVariable(SFO_FOFLAGS_ATTR);

    if ( rxFlags == NULLOBJECT )
    {
        context->SetObjectVariable(SFO_FOFLAGS_ATTR, context->Nil());
    }
    else
    {
        setFOFlags(context, rxFlags, 1);
    }
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
 *  @return  The old value for
 */
RexxMethod1(RexxObjectPtr, ShellFileOp_setOwnerWnd, OPTIONAL_RexxObjectPtr, rxHwndOwner)
{
    RexxObjectPtr oldValue = context->GetObjectVariable(SFO_OWNERWINDOW_ATTR);
    setOwnerWnd(context, rxHwndOwner, 1);
    return oldValue;
}


/** ShellFileOp::setTitle()
 *
 *  Sets or clears the simple progress dialog window title.  The user can invoke
 *  setTitle() with no arg or with .nil to clear the current title.
 *
 *  This title is only used, (by the Windows API,) if the FOF_SIMPLEPROGRESS
 *  flag is set.
 *
 *  This method does not return a value.
 */
RexxMethod1(RexxObjectPtr, ShellFileOp_setTitle, OPTIONAL_RexxObjectPtr, rxTitle)
{
    setTitle(context, rxTitle, 1);
    return NULLOBJECT;
}


/** ShellFileOp::uninit()
 *
 *  The basic uninit for a ShellFileOp object.
 */
RexxMethod1(RexxObjectPtr, ShellFileOp_uninit, OSELF, self)
{
    CommonUninit(true);
    return context->Nil();
}


/**
 * Performs the shell file operation specified by the 'op' flag.
 *
 * @param context      ooRexx method context.
 * @param op           Flag specifing which file operation.
 * @param argNumber    Arg number of the rxFlags parameter.  Used for
 *                     exceptions.
 * @param from         From file list.  Can be null.
 * @param to           To file list.  Can be null.
 * @param rxFlags      File operation flags.  Can be null.  If not null, must be
 *                     .true, .nil, or a FOF flag string.
 * @param rxHwndOwner  Owner window of any dialog put up by the shell. Can be
 *                     null.  If not null, must be a valid window.
 *
 * @return TRUE for success, otherwise FALSE.
 */
static logical_t doTheOp(RexxMethodContext *context, UINT op, CSTRING from, CSTRING to,
                         RexxObjectPtr rxFlags, RexxObjectPtr rxHwndOwner, int argNumber)
{
    SHFILEOPSTRUCT sfos = { 0 };

    int fromArgNumber = (op == FO_DELETE ? argNumber - 1 : argNumber - 2);
    if ( ! addToFileList(context, FROM_DIRECTION, from, fromArgNumber) )
    {
        return FALSE;
    }

    if ( op != FO_DELETE )
    {
        if ( ! addToFileList(context, TO_DIRECTION, to, argNumber - 1) )
        {
            return FALSE;
        }
    }

    if ( ! setFOFlags(context, rxFlags, argNumber) )
    {
        return FALSE;
    }

    if ( rxHwndOwner != NULL && (setOwnerWnd(context, rxHwndOwner, argNumber + 1) == FALSE) )
    {
        return FALSE;
    }

    RexxObjectPtr list = context->GetObjectVariable(SFO_FROMLIST_ATTR);
    if ( list == context->Nil() )
    {
        context->RaiseException1(Rexx_Error_Incorrect_method,
                                 context->NewStringFromAsciiz("No source files have been specified for this shell file operation"));
        return FALSE;
    }
    sfos.pFrom  = context->StringData((RexxStringObject)list);

    if ( op != FO_DELETE )
    {
        list = context->GetObjectVariable(SFO_TOLIST_ATTR);
        if ( list == context->Nil() )
        {
            context->RaiseException1(Rexx_Error_Incorrect_method,
                                     context->NewStringFromAsciiz("No destination files have been specified for this shell file operation"));
            return FALSE;
        }
        sfos.pTo = context->StringData((RexxStringObject)list);
    }

    sfos.wFunc  = op;
    sfos.fFlags = getFOFlags(context);

    if ( ! getOwnerWnd(context, &sfos.hwnd) )
    {
        return FALSE;
    }

    sfos.lpszProgressTitle = getTitle(context, sfos.hwnd, sfos.fFlags);

    printf("FO flags: %x\n", sfos.fFlags);

    // Finally, do the shell file operation.
    logical_t success = (SHFileOperation(&sfos) == 0) ? TRUE : FALSE;

    context->SetObjectVariable(SFO_USERCANCELED_ATTR, sfos.fAnyOperationsAborted ? context->True() : context->False());

    if ( success )
    {
        // Only clear the lists on success.  That way the user can examine the
        // lists after an error.
        context->SetObjectVariable(SFO_FROMLIST_ATTR, context->Nil());
        context->SetObjectVariable(SFO_TOLIST_ATTR, context->Nil());
    }

    return success;
}

/**
 * Adds a file to either the from list or the to list.  These lists are
 * maintained with 2 trailing nulls to signal the end of the list and a single
 * null between items in the list.  Each item is turned into a fully qualified
 * path name.  (The Windows API requires this to work reliably.)
 *
 * @param direction  Whether the file goes on the from or the to list.
 * @param file       The path name to add.  This can be null, in which case do
 *                   nothing.
 *
 * @note  If file can not be turned into a fully qualified path name an
 *        exception is raised.
 */
static bool addToFileList(RexxMethodContext *context, SFO_DIRECTION direction, CSTRING file, int argPos)
{
    if ( file == NULL )
    {
        return true;
    }

    char inBuffer[MAX_PATH + 1];
    char outBuffer[MAX_PATH + 2];

    /* TODO why did I make a copy of file?  Is that needed? */
    pathdup(inBuffer, file);

    if ( PathSearchAndQualify(inBuffer, outBuffer, MAX_PATH) )
    {
        /* TODO Function was reworked for new APIs, need to retest this. */

        // Add the second terminating null and save the length which is needed
        // for the ooRexx string creation.
        size_t l = strlen(outBuffer) + 1;
        outBuffer[l++] = '\0';

        // Point newBuffer at the existing file name first.
        char *newBuffer = outBuffer;

        RexxObjectPtr list;
        if ( direction == FROM_DIRECTION )
        {
            list = context->GetObjectVariable(SFO_FROMLIST_ATTR);
        }
        else
        {
            list = context->GetObjectVariable(SFO_TOLIST_ATTR);
        }

        if ( list != context->Nil() )
        {
            // We are adding a file to an existing list.  If this is the
            // destination list, we need the multi destionation flag set.
            if ( direction == TO_DIRECTION )
            {
                context->SetObjectVariable(SFO_MULTIDESTINATION, context->True());
            }

            // The old list has an extra trailing null at the end.  We need to
            // get the true length of the existing ooRexx string.
            size_t oldL = context->StringLength((RexxStringObject)list);

            // Now we need a buffer big enough to hold the old list and the new
            // file (with the extra trailing null.)
            newBuffer = (char *)malloc(oldL + l);
            if ( newBuffer == NULL )
            {
                systemServiceException(context, MEMORY_ALLOC_FAILED_MSG, NULL);
                return false;
            }

            // Now have ooRexx copy the old list into the new buffer. I think
            // this copy removes the extra null.  TODO need to test.
            size_t copyL = context->StringGet((RexxStringObject)list, 0, newBuffer, oldL);

            // Now append the new file.
            memcpy((newBuffer + oldL), outBuffer, l);
            l += oldL;
        }

        list = context->NewString(newBuffer, l);
        if ( direction == FROM_DIRECTION )
        {
            context->SetObjectVariable(SFO_FROMLIST_ATTR, list);
        }
        else
        {
            context->SetObjectVariable(SFO_TOLIST_ATTR, list);
        }

        // Free memory if we allocated it.
        if ( newBuffer != outBuffer )
        {
            free(newBuffer);
        }
    }
    else
    {
        badArgException(context, argPos, QUALIFIED_PATH_MSG);
        return false;
    }
    return true;
}


/**
 * Sets the attributes that control the file operation flags.
 *
 * @param context     ooRexx method context.
 * @param rxFlags     The ooRexx object that controls how the flags are set.
 *                    This may be null and if so nothing is done.
 * @param argNumber   Argument number for exceptions.
 *
 * @return            True if an exception was raised, otherwise false.
 *
 * @note              This function may be called from doTheOp() where the
 *                    rxFlags arg is optional.  Because of this, if rxFlags is
 *                    null, we do not want to set the FOFlags attribute back to
 *                    .nil.
 */
static bool setFOFlags(RexxMethodContext *context, RexxObjectPtr rxFlags, int argNumber)
{
    if ( rxFlags == NULLOBJECT )
    {
        return true;
    }

    if ( context->IsString(rxFlags) )
    {
        char *flags = strdupupr(context->StringData((RexxStringObject)rxFlags));
        int   foFlag, foFlags = 0;

        // The user can separate tokens with either spaces, commas, or  the '|'
        // symbol.
        char *token = strtok(flags, " ,|");
        while( token != NULL )
        {
            foFlag = getConstantValue(token);
            if ( foFlag == -1 )
            {
                invalidConstantException(context, argNumber, INVALID_CONSTANT_MSG, "FOF", token);
                return false;
            }
            foFlags |= foFlag;
            token = strtok(NULL, " ,|");
        }
        free(flags);

        context->SetObjectVariable(SFO_FOFLAGS_ATTR, context->NewInteger(foFlags));
    }
    else if ( rxFlags == context->Nil())
    {
        context->SetObjectVariable(SFO_FOFLAGS_ATTR, context->Nil());
    }
    else if ( rxFlags == context->True() )
    {
        // This is the 'silent' arg.
        context->SetObjectVariable(SFO_CONFIRM_ATTR, context->False());
        context->SetObjectVariable(SFO_CONFIRM_MKDIR_ATTR, context->False());
        context->SetObjectVariable(SFO_PROGRESS_ATTR, context->False());
        context->SetObjectVariable(SFO_ERRORUI_ATTR, context->False());

        // Use of the boolan silent arg should negate the use of the pure
        // FOF_XXXX flags.
        context->SetObjectVariable(SFO_FOFLAGS_ATTR, context->Nil());
    }
    else if ( rxFlags == context->False() )
    {
        // This is the not 'silent' arg.
        context->SetObjectVariable(SFO_CONFIRM_ATTR, context->True());
        context->SetObjectVariable(SFO_CONFIRM_MKDIR_ATTR, context->True());
        context->SetObjectVariable(SFO_PROGRESS_ATTR, context->True());
        context->SetObjectVariable(SFO_ERRORUI_ATTR, context->True());

        // Use of the boolan silent arg should negate the use of the pure
        // FOF_XXXX flags.
        context->SetObjectVariable(SFO_FOFLAGS_ATTR, context->Nil());
    }
    else
    {
        wrongArgValueException(context, argNumber, ".nil, .true, .false, or valid FOF_XXX flags", rxFlags);
        return false;
    }
    return true;
}


static FILEOP_FLAGS getFOFlags(RexxMethodContext *context)
{
    FILEOP_FLAGS flags = 0;
    RexxObjectPtr rxFlags = context->GetObjectVariable(SFO_FOFLAGS_ATTR);

    if ( rxFlags == context->Nil() )
    {
        if ( context->GetObjectVariable(SFO_CONFIRM_ATTR) == context->False() )
        {
            flags |= FOF_NOCONFIRMATION;
        }
        if ( context->GetObjectVariable(SFO_CONFIRM_MKDIR_ATTR) == context->False() )
        {
            flags |= FOF_NOCONFIRMMKDIR;
        }
        if ( context->GetObjectVariable(SFO_CONFIRM_MKDIR_ATTR) == context->False() )
        {
            flags |= FOF_NOCONFIRMMKDIR;
        }
        if ( context->GetObjectVariable(SFO_PROGRESS_ATTR) == context->False() )
        {
            flags |= FOF_SILENT;
        }
        if ( context->GetObjectVariable(SFO_PROGRESS_SIMPLE_ATTR) == context->True() )
        {
            flags |= FOF_SIMPLEPROGRESS;
        }
        if ( context->GetObjectVariable(SFO_ERRORUI_ATTR) == context->False() )
        {
            flags |= FOF_NOERRORUI;
        }
        if ( context->GetObjectVariable(SFO_RECURSIVE_ATTR) == context->False() )
        {
            flags |= FOF_NORECURSION;
        }
        if ( context->GetObjectVariable(SFO_PERMANENT_ATTR) == context->False() )
        {
            flags |= FOF_ALLOWUNDO;
        }
    }
    else
    {
        flags = (FILEOP_FLAGS)context->IntegerValue((RexxIntegerObject)rxFlags);
    }

    if ( context->GetObjectVariable(SFO_MULTIDESTINATION) == context->True() )
    {
        flags |= FOF_MULTIDESTFILES;
    }

    return flags;
}

static bool setOwnerWnd(RexxMethodContext *context, RexxObjectPtr rxHwnd, int argNumber)
{
    if ( rxHwnd == NULL || rxHwnd == context->Nil() )
    {
        context->SetObjectVariable(SFO_OWNERWINDOW_ATTR, context->Nil());
    }
    else if ( context->IsPointer(rxHwnd) )
    {
        if ( ! IsWindow((HWND)context->PointerValue((RexxPointerObject)rxHwnd)) )
        {
            badArgException(context, argNumber, WINDOW_HANDLE_MSG);
            return false;
        }
        context->SetObjectVariable(SFO_OWNERWINDOW_ATTR, rxHwnd);
    }
    else
    {
        wrongArgValueException(context, argNumber, ".nil, or a valid window handle", rxHwnd);
        return false;
    }
    return true;
}


/**
 * Retrieves the window handle from the owner window attribute.
 *
 * @param  context  ooRexx method context.
 * @param  *hwnd    Handle is returned here.
 *
 * @return false if an exception is raised, otherwise true.
 *
 * @note   The owner window attribute is guarenteed to be set. If nil, which is
 *         valid, hwnd is set to null.  If the attribute is not nil, then the
 *         hwnd is checked to be sure it is still a valid window.  If it is not
 *         a valid window, an exception is raised.  This may be too harsh,
 *         because using an invalid window handle will not cause the file
 *         operation to fail.  But, this will prevent behavior that the user
 *         does not understand.
 */
static bool getOwnerWnd(RexxMethodContext *context, HWND *hwnd)
{
    *hwnd = NULL;
    RexxObjectPtr rxHwnd = context->GetObjectVariable(SFO_OWNERWINDOW_ATTR);

    if ( rxHwnd != context->Nil() )
    {
        *hwnd = (HWND)context->PointerValue((RexxPointerObject)rxHwnd);
        if ( ! IsWindow(*hwnd) )
        {
            *hwnd = NULL;
            systemServiceException(context, INVALID_WINDOW_MSG, NULL);
            return false;
        }
    }
    return true;
}


static bool setTitle(RexxMethodContext *context, RexxObjectPtr rxTitle, int argNumber)
{
    if ( rxTitle == NULL || rxTitle == context->Nil() )
    {
        context->SetObjectVariable(SFO_PROGRESSTITLE_ATTR, context->Nil());
    }
    else if ( context->IsString(rxTitle) )
    {
        context->SetObjectVariable(SFO_PROGRESSTITLE_ATTR, rxTitle);
    }
    else
    {
        wrongArgValueException(context, argNumber, ".nil, or a string", rxTitle);
        return false;
    }
    return true;
}


/**
 * Gets the title for the simple progress dialog.  This title is only used, (by
 * the Windows API,) if the FOF_SIMPLEPROGRESS flag is set.
 *
 * @param c
 * @param hwnd
 * @param flags
 *
 * @return      The title if appropriate, otherwise null.
 */
inline CSTRING getTitle(RexxMethodContext *c, HWND hwnd, FILEOP_FLAGS flags)
{
    if ( hwnd != NULL && (flags & FOF_SIMPLEPROGRESS) )
    {
        RexxObjectPtr t = c->GetObjectVariable(SFO_PROGRESSTITLE_ATTR);
        if ( t != c->Nil() )
        {
            return c->StringData((RexxStringObject)t);
        }
    }
    return NULL;
}

#define SIMPLE_FOLDER_BROWSE_CLASS "SimpleFolderBrowse"

#define BROWSE_TITLE       "ooRexx Browse for Folder"
#define BROWSE_BANNER      "Select (or create) the folder you want"
#define BROWSE_HINT        "If the folder you want does not exist you can create it"
#define BROWSE_START_DIR   ""

// Function prototypes for the SimpleFolderBrowse helper functions.
void freeRootPidl(RexxMethodContext *);

/** SimpleFolderBrowse::init()
 *
 * Initializes a SimpleFolderBrowse object.  The title, banner, and hint strings
 * all have default values that are different from the Windows default values.
 * If the user wants to have the default Windows values, they specify that by
 * setting them to either .nil or the empty string.
 */
RexxMethod4(RexxObjectPtr, SimpleFolderBrowse_init,
            OPTIONAL_CSTRING,  title,
            OPTIONAL_CSTRING,  banner,
            OPTIONAL_CSTRING,  hint,
            OPTIONAL_CSTRING,  startDir)
{
	LPITEMIDLIST pidl = NULL;

    if ( ! CommonInit(context, true) )
    {
        return NULLOBJECT;
    }

    // Set the default attributes for the browser.
    setSFBAttribute(context,"TITLE", title, BROWSE_TITLE);
    setSFBAttribute(context,"BANNER", banner, BROWSE_BANNER);
    setSFBAttribute(context,"HINT", hint, BROWSE_HINT);
    setSFBAttribute(context,"INITIALFOLDER", startDir, BROWSE_START_DIR);

    if ( pidlForSpecialFolder(CSIDL_DRIVES, &pidl) )
    {
        context->SetObjectVariable("ROOT", context->NewPointer(pidl));
    }
    else
    {
        systemServiceException(context, NO_ITEMID_MSG, "CSIDL_DRIVES");
    }

    return NULLOBJECT;
}

LPITEMIDLIST getPidlFromString(RexxMethodContext *context, CSTRING idl, int argPos, bool canFail)
{
    LPITEMIDLIST pidl = NULL;

    // See if it looks like a CSIDL_ constant.
    if ( strlen(idl) > 6 && StrCmpNI("CSIDL_", idl, 6) == 0 )
    {
        char *argTmp = strdupupr(idl);
        int csidl = getConstantValue(argTmp);
        free(argTmp);

        if ( csidl == -1 )
        {
            invalidConstantException(context, argPos, INVALID_CONSTANT_MSG, "CSIDL", idl);
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
        wrongFormatException(context, argPos, idl);
    }

    if ( pidl == NULL && ! canFail )
    {
        wrongFormatException(context, argPos, idl);
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

RexxMethod1(RexxObjectPtr, SimpleFolderBrowse_setRoot, RexxObjectPtr, obj)
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

/** SimpleFolderBrowse::getFolder()
 *
 * Puts up the customized Browse for Folder dialog using the atributes set for
 * this instance.  The optional arg is a window handle.  If specified that
 * window is used as the owner of the dialog.
 *
 * @param   hwnd  OPTIONAL  A window handle to be used as owner of the browse
 *          dialog.  If this arg is used, but is not a valid window handle, then
 *          an exception is raised.
 *
 * @return  Returns the fully qualified path of the folder the user selects, or
 *          the empty string if the user cancels.  In addition, if the user
 *          selects a virtual folder that has no file system path, .nil is
 *          returned.
 */
RexxMethod2(RexxObjectPtr, SimpleFolderBrowse_getFolder, OSELF, self, OPTIONAL_POINTER, owner)
{
    BROWSEINFO   bi = { 0 };
    BROWSE_DATA  bd = { 0 };

    if ( ! checkOptionalWindow(context, (HWND)owner, 1) )
    {
        return NULLOBJECT;
    }

    // NULL for hwnd works fine.
    fillInBrowseData(context, (HWND)owner, &bi, &bd);

    // Second arg is true - return path.
    return folderBrowse(context, &bi, true);
}


/** SimpleFolderBrowse::getItemID()
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
RexxMethod2(RexxObjectPtr, SimpleFolderBrowse_getItemID, OSELF, self, OPTIONAL_POINTER, owner)
{
    BROWSEINFO   bi = { 0 };
    BROWSE_DATA  bd = { 0 };

    if ( ! checkOptionalWindow(context, (HWND)owner, 1) )
    {
        return NULLOBJECT;
    }

    // NULL for owner works fine.
    fillInBrowseData(context, (HWND)owner, &bi, &bd);

    // Second arg is false - return PIDL, not path.
    return folderBrowse(context, &bi, false);
}


/** SimpleFolderBrowse::uninit()
 *
 * Does clean up for this SimpleFolderBrowse.  Frees the root PIDL and calls the
 * common uninit routine.
 */
RexxMethod1(RexxObjectPtr, SimpleFolderBrowse_uninit, OSELF, self)
{
    freeRootPidl(context);
    CommonUninit(true);
    return NULLOBJECT;
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


void freeRootPidl(RexxMethodContext *context)
{
    LPCITEMIDLIST pidl = getRootPidl(context);
    if ( pidl != NULL )
    {
        CoTaskMemFree((LPVOID)pidl);
    }
}

void setSFBAttribute(RexxMethodContext *context, char *name, CSTRING userVal, char *defaultVal)
{
    if ( userVal == NULL )
    {
        context->SetObjectVariable(name, context->NewStringFromAsciiz(defaultVal));
    }
    else if ( strlen(userVal) == 0 )
    {
        context->SetObjectVariable(name, context->Nil());
    }
    else
    {
        context->SetObjectVariable(name, context->NewStringFromAsciiz(userVal));
    }
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

    char outBuffer[MAX_PATH];

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
RexxMethod2(RexxStringObject, Path_compact, CSTRING, path, uint32_t, count)
{
    char inBuffer[MAX_PATH + 1];
    pathdup(inBuffer, path);

    char outBuffer[MAX_PATH];

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
RexxMethod1(logical_t, Path_isFull, CSTRING, path)
{
    return _PathIsFull(path) ? 1 : 0;
}

/** Path::isNetworkPath
 *
 *  Returns true if the specified path represents a network resource, otherwise
 *  returns false.
 */
RexxMethod1(logical_t, Path_isNetworkPath, CSTRING, path)
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
RexxMethod1(RexxStringObject, Path_getShortPath, CSTRING, path)
{
    char pathBuffer[MAX_PATH];
    WCHAR wBuffer[MAX_PATH];

    if ( MultiByteToWideChar(CP_ACP, 0, path, -1, wBuffer, MAX_PATH) == 0 )
    {
        systemServiceException(context, UNICODE_ANSI_FAILED_MSG, NULL);
        return NULL;
    }

    PathGetShortPath(wBuffer);

    if ( WideCharToMultiByte(CP_ACP, 0, wBuffer, -1, pathBuffer, MAX_PATH, NULL, NULL) == 0)
    {
        systemServiceException(context, UNICODE_ANSI_FAILED_MSG, NULL);
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
    return context->NewStringFromAsciiz(WIN_SHELL_VERSION);
}

/** Sh::is64Bit()
 *
 * Returns true if the current process is running in 64 bit mode, otherwise
 * false.
 */
RexxMethod0(RexxObjectPtr, Sh_is64Bit_class)
{
    if ( _is64Bit() )
        return context->True();
    return context->False();
}

/** Sh::is32on64Bit()
 *
 * Returns true if the current process is running in 32 bit mode on a 64 bit
 * operating system, otherwise false.
 */
RexxMethod0(RexxObjectPtr, Sh_is32on64Bit_class)
{
    if ( _is32on64Bit() )
        return context->True();
    return context->False();
}

/** Sh::isW2K()
 *
 * Returns true if the operating system is Windows 2000 (W2K), otherwise false.
 */
RexxMethod0(RexxObjectPtr, Sh_isW2K_class)
{
    if ( _isW2K() )
        return context->True();
    return context->False();
}

/** Sh::isAtLeastW2K()
 *
 * Returns true if the operating system is Windows 2000 (W2K), service pack 4,
 * or later. Otherwise returns false.
 */
RexxMethod0(RexxObjectPtr, Sh_isAtLeastW2K_class)
{
    if ( _isAtLeastW2K() )
        return context->True();
    return context->False();
}

/** Sh::isXP()
 *
 * Returns true if the operating system is Windows XP, otherwise false.
 */
RexxMethod0(RexxObjectPtr, Sh_isXP_class)
{
    if ( _isXP() )
        return context->True();
    return context->False();
}

/** Sh::isXP32()
 *
 * Returns true if the operating system is Windows XP 32 bit, otherwise false.
 */
RexxMethod0(RexxObjectPtr, Sh_isXP32_class)
{
    if ( _isXP32() )
        return context->True();
    return context->False();
}

/** Sh::isXP64()
 *
 * Returns true if the operating system is Windows XP 64 bit, otherwise false.
 */
RexxMethod0(RexxObjectPtr, Sh_isXP64_class)
{
    if ( _isXP64() )
        return context->True();
    return context->False();
}

/** Sh::isAtLeastXP()
 *
 * Returns true if the operating system is Windows XP, service pack 2, or later.
 * Otherwise returns false.
 */
RexxMethod0(RexxObjectPtr, Sh_isAtLeastXP_class)
{
    if ( _isAtLeastXP() )
        return context->True();
    return context->False();
}

/** Sh::isW2K3()
 *
 * Returns true if the operating system is Windows Server 2003 (W2K3), otherwise
 * false.
 */
RexxMethod0(RexxObjectPtr, Sh_isW2K3_class)
{
    if ( _isW2K3() )
        return context->True();
    return context->False();
}

/** Sh::isAtLeastW2K3()
 *
 * Returns true if the operating system is Windows Server 2003 (W2K3), service
 * pack 1, or later.  Otherwise returns false.
 */
RexxMethod0(RexxObjectPtr, Sh_isAtLeastW2K3_class)
{
    if ( _isAtLeastW2K3() )
        return context->True();
    return context->False();
}

/** Sh::isVista()
 *
 * Returns true if the operating system is Visata, otherwise false.
 */
RexxMethod0(RexxObjectPtr, Sh_isVista_class)
{
    if ( _isVista() )
        return context->True();
    return context->False();
}

/** Sh::isAtLeastVista()
 *
 * Returns true if the operating system is Vista, or later.  Otherwise returns
 * false.
 */
RexxMethod0(RexxObjectPtr, Sh_isAtLeastVista_class)
{
    if ( _isAtLeastVista() )
        return context->True();
    return context->False();
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
                rxResult = context->NewStringFromAsciiz(path);
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

static void fillInBrowseData(RexxMethodContext *context, HWND hwnd, PBROWSEINFO pBI, PBROWSE_DATA pBD)
{
	LPITEMIDLIST pidlRoot = NULL;
    PTCHAR       pHint;
    RexxObjectPtr   rxObject;

    pBI->hwndOwner = hwnd;
    pBI->ulFlags  = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE | BIF_RETURNFSANCESTORS;

    rxObject = context->GetObjectVariable("TITLE");
    if ( rxObject != context->Nil() )
    {
        pBD->dlgTitle = (char *)context->StringData((RexxStringObject)rxObject);
    }

    rxObject = context->GetObjectVariable("HINT");
    if ( rxObject != context->Nil() )
    {
        pBI->ulFlags |= BIF_UAHINT;
        pHint = (char *)context->StringData((RexxStringObject)rxObject);
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

    rxObject = context->GetObjectVariable("INITIALFOLDER");
    if ( rxObject != context->Nil() )
    {
        pBD->startDir = (char *)context->StringData((RexxStringObject)rxObject);
    }

    // Note that banner goes into the BROWSEINFO structure, not the BROWSE_DATA
    // structure.
    rxObject = context->GetObjectVariable("BANNER");
    if ( rxObject != context->Nil() )
    {
        pBI->lpszTitle = context->StringData((RexxStringObject)rxObject);
    }

    pBI->pidlRoot = getRootPidl(context);

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



REXX_METHOD_PROTOTYPE(size_init);
REXX_METHOD_PROTOTYPE(size_cx);
REXX_METHOD_PROTOTYPE(size_setCX);
REXX_METHOD_PROTOTYPE(size_cy);
REXX_METHOD_PROTOTYPE(size_setCY);

RexxMethodEntry winshell_methods[] = {
    REXX_METHOD(size_init,              size_init),
    REXX_METHOD(size_cx,                size_cx),
    REXX_METHOD(size_setCX,             size_setCX),
    REXX_METHOD(size_cy,                size_cy),
    REXX_METHOD(size_setCY,             size_setCY),
    REXX_LAST_METHOD()
};

RexxPackageEntry winshell_package_entry =
{
    STANDARD_PACKAGE_HEADER
    REXX_INTERPRETER_4_0_0,              // anything after 4.0.0 will work
    "WINSHELL",                          // name of the package
    "4.0",                               // package information
    NULL,                                // no load functions
    NULL,                                // no unload functions
    NULL,                                // the exported functions
    winshell_methods                     // the exported methods
};

// package loading stub.
OOREXX_GET_PACKAGE(winshell);

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

// Would need this to go into the WinShellTable:

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
