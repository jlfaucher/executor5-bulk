/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2014-2014 Rexx Language Association. All rights reserved.    */
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
 * ooConsole.cpp
 *
 * ...
 */

#include "ooConsole.hpp"

#include <stdio.h>
#include <string.h>
#include <shlwapi.h>

#include "APICommon.hpp"
#include "ooShapes.hpp"


/**
 * RexxPackageLoader function.
 *
 * The package loader function is called when the library package is first
 * loaded.  This makes it the ideal place for any initialization that must be
 * done prior to the Rexx program start up.  We call sqlite3_initialize() and
 * set up a few global values.
 *
 * @param c  Thread context pointer passed from the interpreter when this package
 *           is loaded.
 *
 * @return Nothing is returned
 */
void RexxEntry ooConsoleLoad(RexxThreadContext *c)
{
    if ( packageLoadHelper(c) )
    {
        c->DirectoryPut(TheDotLocalObj, c->NullString(), "ROUTINEERRORMESSAGE");
    }
}

/**
 * RexxPackageUnloader function.
 *
 * The package unloader function is called when the library package is unloaded
 * by the interpreter. The unloading process happens when the last interpreter
 * instance is destroyed during the last cleanup stages.
 *
 * We just do nothing at this time.
 *
 * @param c  Thread context pointer passed from the intepreter when this package
 *           is unloaded.
 *
 * @return Nothing is returned
 */
void RexxEntry ooConsoleUnload(RexxThreadContext *c)
{
    // Purposively do nothing
}




/**
 * Generic function to return either the compact or full version for the
 * ooConsole extension.
 *
 * This is not part of the SQLite library, it is an ooConsole only interface.
 * Used by both the object orientated (.ooConsole) and classic Rexx interfaces.
 *
 * @param c     Thread context we are operating in.
 * @param full  If true, the full version is returned, otherwise the one line
 *              version.
 *
 * @return A formatted string specifying the ooConsole version.
 */
RexxStringObject genGetVersion(RexxThreadContext *c, logical_t full, logical_t minimal)
{
    char   buf[512] = {'\0'};
    size_t bits = 32;

#ifdef __REXX64__
    bits = 64;
#endif

    if ( full )
    {
        char buf1[512];

        snprintf(buf1, sizeof(buf1), "ooConsole Built %s %s\n"
                                     "          Copyright (c) RexxLA %s.\n"
                                     "          All Rights Reserved.\n\n",
                 __DATE__, __TIME__, OOCONSOLE_COPYRIGHT_YEAR);
        strcat(buf, buf1);

        size_t rx = c->InterpreterVersion();

        snprintf(buf1, sizeof(buf1), "Rexx:     Open Object Rexx Version %d.%d.%d\n\n",
                 (rx >> 16) & 0xff, (rx >> 8) & 0xff, rx & 0x0000ff);
        strcat(buf, buf1);

    }
    else
    {
        if ( minimal )
        {
            snprintf(buf, sizeof(buf), "%d.%d.%d.%d\n",
                     OOCONSOLE_VER_MAJOR, OOCONSOLE_VER_MINOR, OOCONSOLE_VER_LEVEL, OOCONSOLE_VER_BUILD);
        }
        else
        {
            snprintf(buf, sizeof(buf), "ooConsole Version %d.%d.%d.%d (%d bit)\n",
                     OOCONSOLE_VER_MAJOR, OOCONSOLE_VER_MINOR, OOCONSOLE_VER_LEVEL, OOCONSOLE_VER_BUILD, bits);
        }
    }

    return c->String(buf);
}


/**
 *  Methods for the .ooConsole class.
 */
#define OOCONSOLE_CLASS    "ooConsole"

inline void size2coord(PORXSIZE s, PCOORD c)
{
    c->X = (uint16_t)s->cx;
    c->Y = (uint16_t)s->cy;
}

inline void rect2smallRect(PORXRECT r, PSMALL_RECT sr)
{
    sr->Left   = (uint16_t)r->left;
    sr->Top    = (uint16_t)r->top;
    sr->Right  = (uint16_t)r->right;
    sr->Bottom = (uint16_t)r->bottom;
}

/**
 * Checks for a NULL CSelf pointer and raises an exception if it is null.
 *
 * @param c
 * @param pCSelf
 *
 * @return pCooConsole
 */
static inline pCooConsole ensureCSelf(RexxMethodContext *c, void *pCSelf)
{
    if ( pCSelf == NULL )
    {
        baseClassInitializationException(c, "ooConsole");
    }
    return (pCooConsole)pCSelf;
}

/**
 * Gets the standard handles for this console.
 *
 * @param c
 * @param pCon
 *
 * @return bool
 */
static bool goodHandles(RexxMethodContext *c, pCooConsole pCon)
{
    pCon->errRC = 0;

    pCon->hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if ( pCon->hStdOut == INVALID_HANDLE_VALUE || pCon->hStdOut == NULL )
    {
        pCon->errRC = GetLastError();
        pCon->isValid = false;
        return false;
    }

    pCon->hStdIn  = GetStdHandle(STD_INPUT_HANDLE);
    if ( pCon->hStdIn == INVALID_HANDLE_VALUE || pCon->hStdIn == NULL )
    {
        pCon->errRC = GetLastError();
        pCon->isValid = false;
        return false;
    }

    pCon->hStdErr = GetStdHandle(STD_ERROR_HANDLE);
    if ( pCon->hStdErr == INVALID_HANDLE_VALUE || pCon->hStdErr == NULL )
    {
        pCon->errRC = GetLastError();
        pCon->isValid = false;
        return false;
    }

    pCon->isValid = true;
    return true;
}

/**
 * Checks both that the console CSelf is not null and that the console is
 * usable.  Usable means that the we are in a character mode window.
 *
 * @param c
 * @param pCSelf
 *
 * @return pCooConn
 */
static pCooConsole requiredConsole(RexxMethodContext *c, void *pCSelf)
{
    pCooConsole pCon = ensureCSelf(c, pCSelf);
    if ( pCon != NULL )
    {
        if ( pCon->isValid )
        {
            pCon->errRC = 0;
            if ( pCon->isLongTerm )
            {
                return pCon;
            }
            if ( goodHandles(c, pCon) )
            {
                return pCon;
            }
        }
        pCon = NULL;
    }
    return pCon;
}

/**
 * Parses the string 'handle' and returns which of the 3 standard handles the
 * user specifies.  The default if omitted is STDOUT.
 *
 * @param context
 * @param handle
 * @param pos
 *
 * @return The specified handle on success, or NULL on error.
 */
static HANDLE bufferHandle(RexxMethodContext *context, pCooConsole pcon, CSTRING handle, size_t pos)
{
    HANDLE h = pcon->hStdOut;
    if ( argumentExists(pos) )
    {
        if ( stricmp(handle, "OUT") == 0 ) h = pcon->hStdOut;
        else if ( stricmp(handle, "ERR") == 0 ) h = pcon->hStdErr;
        else if ( stricmp(handle, "IN") == 0 ) h = pcon->hStdIn;
        else
        {
            wrongArgValueException(context->threadContext, pos, "Out, Err, In", handle);
            return NULL;
        }
    }
    return h;
}

/** ooConsole::version  [class method]
 *
 *  @param type  [optional]  The style of the version output returned.  Keywords
 *               are, but only 1st letter is required:
 *
 *                 Compact
 *                 Full
 *                 OneLine
 *
 *               The defualt is OneLine
 */
RexxMethod1(RexxObjectPtr, oocon_version_cls, OPTIONAL_CSTRING, type)
{
    if ( argumentExists(1) )
    {
        switch ( toupper(*type) )
        {
            case 'O' :
                return genGetVersion(context->threadContext, FALSE, FALSE);
                break;

            case 'F' :
                return genGetVersion(context->threadContext, TRUE, FALSE);
                break;

            case 'C' :
                return genGetVersion(context->threadContext, FALSE, TRUE);
                break;

            default :
                wrongArgOptionException(context->threadContext, 1, VALID_VERSION_TYPES, type);
                return TheZeroObj;
                break;
        }
    }

    return genGetVersion(context->threadContext, FALSE, FALSE);
}

/** ooConsole::errRC
 *
 *  Attribute, get only.
 *
 */
RexxMethod1(int32_t, oocon_errRC_atr, CSELF, pCSelf) { return ((pCooConsole)pCSelf)->errRC; }

/** ooConsole::init()
 *
 *  @param If ... default is true.
 */
RexxMethod2(RexxObjectPtr, oocon_init, OPTIONAL_logical_t, longTerm, OSELF, self)
{
    // Get a buffer for the ooConsole CSelf.
    RexxBufferObject cselfBuffer = context->NewBuffer(sizeof(CooConsole));
    if ( cselfBuffer == NULLOBJECT )
    {
        outOfMemoryException(context->threadContext);
        return NULLOBJECT;
    }

    context->SetObjectVariable("CSELF", cselfBuffer);

    pCooConsole pCon = (pCooConsole)context->BufferData(cselfBuffer);
    memset(pCon, 0, sizeof(CooConsole));

    if ( argumentOmitted(1) )
    {
        longTerm = TRUE;
    }
    pCon->isLongTerm = longTerm ? true : false;

    if ( ! goodHandles(context, pCon) )
    {
        pCon->isValid = false;
        return NULLOBJECT;
    }

    pCon->isValid    = true;

    return TheZeroObj;
}

/** ooConsole::uninit()
 *
 *  At this point, the only purpose of uninit()
 */
RexxMethod1(RexxObjectPtr, oocon_uninit, CSELF, pCSelf)
{
    if ( pCSelf != NULLOBJECT )
    {
        pCooConsole pCon = (pCooConsole)pCSelf;

#if OOCONSOLEDBG == 1
        printf("ooConsole::uninit()\n");
#endif
    }
    return TheZeroObj;
}

/** ooConsole::getCP()
 *
 *  Gets the input code page used by the console associated with the calling
 *  process.
 *
 *  @return  The codepage on success, .nil on error.
 */
RexxMethod1(RexxObjectPtr, oocon_getCP, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return TheNilObj;
    }

    return context->UnsignedInt32(GetConsoleCP());
}


/** ooConsole::getCursorInof()
 *
 *  Retrieves information about the size and visibility of the cursor for this
 *  console buffer.
 *
 *  @return  A .directory object on success, .nil on error.
 */
RexxMethod1(RexxObjectPtr, oocon_getCursorInfo, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return TheNilObj;
    }

    CONSOLE_CURSOR_INFO ci;
    if ( GetConsoleCursorInfo(pcon->hStdOut, &ci) == 0 )
    {
        pcon->errRC = GetLastError();
        return TheNilObj;
    }

    RexxDirectoryObject d = context->NewDirectory();
    context->DirectoryPut(d, context->Logical(ci.bVisible), "VISIBLE");
    context->DirectoryPut(d, context->UnsignedInt32(ci.dwSize), "SIZE");

    return d;
}


/** ooConsole::getDisplayMode()
 *
 *
 */
RexxMethod1(RexxObjectPtr, oocon_getDisplayMode, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return TheNilObj;
    }

    uint32_t mode;
    if ( GetConsoleDisplayMode((LPDWORD)&mode) == 0 )
    {
        pcon->errRC = GetLastError();
        return TheNilObj;
    }

    switch ( mode )
    {
        case 0 :
            return context->String("Not Full Screen");
        case CONSOLE_FULLSCREEN :
            return context->String("Full Screen");
        case CONSOLE_FULLSCREEN_HARDWARE :
            return context->String("Full Screen Hardware");
        default :
            return context->String("Error Unexpected.");
    }
}


/** ooConsole::getFontSize()
 *
 *
 *  @param  fullScreen [optional] If info is for a full screen window.  Default
 *                     is false.
 */
RexxMethod2(RexxObjectPtr, oocon_getFontSize, OPTIONAL_logical_t, fullScreen, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return TheNilObj;
    }

    CONSOLE_FONT_INFO  cfi;
    if ( GetCurrentConsoleFont(pcon->hStdOut, fullScreen ? TRUE : FALSE, &cfi) == 0 )
    {
        pcon->errRC = GetLastError();
        return TheNilObj;
    }

    COORD s = GetConsoleFontSize(pcon->hStdOut, cfi.nFont);
    if ( s.X == 0 && s.Y == 0 )
    {
        pcon->errRC = GetLastError();
        return TheNilObj;
    }

    return rxNewSize(context, s.X, s.Y);
}


/** ooConsole::getHistoryInfo()
 *
 *
 */
RexxMethod1(RexxObjectPtr, oocon_getHistoryInfo, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return TheNilObj;
    }

    CONSOLE_HISTORY_INFO  chi = {sizeof(CONSOLE_HISTORY_INFO)};
    if ( GetConsoleHistoryInfo(&chi) == 0 )
    {
        pcon->errRC = GetLastError();
        return TheNilObj;
    }

    RexxStringObject flag = chi.dwFlags == 1 ? context->String("No Duplicates") : context->String("Duplicates");

    RexxDirectoryObject d = context->NewDirectory();
    context->DirectoryPut(d, context->UnsignedInt32(chi.HistoryBufferSize), "COMMANDS");
    context->DirectoryPut(d, context->UnsignedInt32(chi.NumberOfHistoryBuffers), "BUFFERS");
    context->DirectoryPut(d, flag, "FLAG");

    return d;
}


/** ooConsole::getMode()
 *
 *
 */
RexxMethod2(RexxObjectPtr, oocon_getMode, OPTIONAL_CSTRING, handle, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return TheNilObj;
    }

    HANDLE h = bufferHandle(context, pcon, handle, 1);
    if ( h == NULL )
    {
        return TheNilObj;
    }

    uint32_t mode;
    if ( GetConsoleMode(h, (LPDWORD)&mode) == 0 )
    {
        pcon->errRC = GetLastError();
        return TheNilObj;
    }

    char buf[512] = {'\0'};
    if ( h == pcon->hStdIn )
    {
        if ( (mode & ENABLE_ECHO_INPUT)      == ENABLE_ECHO_INPUT )      strcat(buf, "EchoInput ");
        if ( (mode & ENABLE_INSERT_MODE)     == ENABLE_INSERT_MODE )     strcat(buf, "InsertMode ");
        if ( (mode & ENABLE_LINE_INPUT)      == ENABLE_LINE_INPUT )      strcat(buf, "LineInput ");
        if ( (mode & ENABLE_MOUSE_INPUT)     == ENABLE_MOUSE_INPUT )     strcat(buf, "MouseInput ");
        if ( (mode & ENABLE_PROCESSED_INPUT) == ENABLE_PROCESSED_INPUT ) strcat(buf, "ProcessedInput ");
        if ( (mode & ENABLE_QUICK_EDIT_MODE) == ENABLE_QUICK_EDIT_MODE ) strcat(buf, "EditMode ");
        if ( (mode & ENABLE_WINDOW_INPUT)    == ENABLE_WINDOW_INPUT )    strcat(buf, "WindowInput ");
    }
    else
    {
        if ( (mode & ENABLE_PROCESSED_OUTPUT)   == ENABLE_PROCESSED_OUTPUT )   strcat(buf, "ProcessedOutput ");
        if ( (mode & ENABLE_WRAP_AT_EOL_OUTPUT) == ENABLE_WRAP_AT_EOL_OUTPUT ) strcat(buf, "WrapAtEolOuput ");
    }

    if ( *buf != '\0' )
    {
        *(buf + strlen(buf) - 1) = '\0';
    }
    return context->String(buf);
}


/** ooConsole::getProcessList()
 *
 *
 */
RexxMethod1(RexxObjectPtr, oocon_getProcessList, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return TheNilObj;
    }

    uint32_t list[MAX_CONSOLEPIDS];
    uint32_t count = GetConsoleProcessList((LPDWORD)list, MAX_CONSOLEPIDS);
    if ( count == 0 )
    {
        pcon->errRC = GetLastError();
        return TheNilObj;
    }
    if ( count > MAX_CONSOLEPIDS )
    {
        char buf[512];
        snprintf(buf, sizeof(buf), "More than %d processes are attached to this console.", MAX_CONSOLEPIDS);
        executionErrorException(context->threadContext, buf);
        pcon->errRC = ERROR_NOT_ENOUGH_MEMORY;
        return TheNilObj;
    }

    RexxArrayObject pids = context->NewArray(count);

    for (size_t i = 0; i < count; i++)
    {
        context->ArrayPut(pids, context->UnsignedInt32(list[i]), i + 1);
    }

    return pids;
}


/** ooConsole::getOutputCP()
 *
 *  Gets the output code page used by the console associated with the calling
 *  process.
 *
 *  @return  The codepage on success, .nil on error.
 */
RexxMethod1(RexxObjectPtr, oocon_getOutputCP, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return TheNilObj;
    }
    return context->UnsignedInt32(GetConsoleOutputCP());
}


/** ooConsole::getOriginalTitle()
 *
 *  Gets the original title of the console window
 *
 */
RexxMethod1(RexxObjectPtr, oocon_getOriginalTitle, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return TheNilObj;
    }

    char buf[MAX_CONSOLETITLE];
    uint32_t count = GetConsoleOriginalTitle(buf, MAX_CONSOLETITLE);
    if ( count == 0 )
    {
        pcon->errRC = GetLastError();
        return TheNilObj;
    }
    if ( count > MAX_CONSOLETITLE )
    {
        char buf[512];
        snprintf(buf, sizeof(buf), "Title is longer than %d characters.", MAX_CONSOLETITLE);
        executionErrorException(context->threadContext, buf);
        pcon->errRC = ERROR_NOT_ENOUGH_MEMORY;
        return TheNilObj;
    }

    return context->String(buf);
}


/** ooConsole::getTitle()
 *
 *  Gets the title of the console window
 *
 */
RexxMethod1(RexxObjectPtr, oocon_getTitle, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return TheNilObj;
    }

    char buf[MAX_CONSOLETITLE];
    uint32_t count = GetConsoleTitle(buf, MAX_CONSOLETITLE);
    if ( count == 0 )
    {
        pcon->errRC = GetLastError();
        return TheNilObj;
    }
    if ( count > MAX_CONSOLETITLE )
    {
        char buf[512];
        snprintf(buf, sizeof(buf), "Title is longer than %d characters.", MAX_CONSOLETITLE);
        executionErrorException(context->threadContext, buf);
        pcon->errRC = ERROR_NOT_ENOUGH_MEMORY;
        return TheNilObj;
    }

    return context->String(buf);
}


/** ooConsole::getWindow()
 *
 *  Gets the window handle of the console window
 *
 */
RexxMethod1(RexxObjectPtr, oocon_getWindow, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return TheNilObj;
    }

    HWND hwnd = GetConsoleWindow();
    if ( hwnd == NULL )
    {
        pcon->errRC = GetLastError();
        return TheNilObj;
    }

    return pointer2string(context, hwnd);
}


/** ooConsole::getWindowRect()
 *
 *
 */
RexxMethod1(RexxObjectPtr, oocon_getWindowRect, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return TheNilObj;
    }

    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
    if ( GetConsoleScreenBufferInfo(pcon->hStdOut, &csbiInfo) == 0 )
    {
        pcon->errRC = GetLastError();
        return TheNilObj;
    }

    return rxNewRect(context, csbiInfo.srWindow.Left, csbiInfo.srWindow.Top, csbiInfo.srWindow.Right,
                     csbiInfo.srWindow.Bottom);
}


/** ooConsole::getWindowSize()
 *
 *
 */
RexxMethod1(RexxObjectPtr, oocon_getWindowSize, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return TheNilObj;
    }

    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
    if ( GetConsoleScreenBufferInfo(pcon->hStdOut, &csbiInfo) == 0 )
    {
        pcon->errRC = GetLastError();
        return TheNilObj;
    }

    return rxNewSize(context,
                     csbiInfo.srWindow.Right - csbiInfo.srWindow.Left + 1,
                     csbiInfo.srWindow.Bottom - csbiInfo.srWindow.Top + 1);
}


/** ooConsole::getScreenBufferSize()
 *
 *
 */
RexxMethod1(RexxObjectPtr, oocon_getScreenBufferSize, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return TheNilObj;
    }

    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
    if ( GetConsoleScreenBufferInfo(pcon->hStdOut, &csbiInfo) == 0 )
    {
        pcon->errRC = GetLastError();
        return TheNilObj;
    }

    return rxNewSize(context, csbiInfo.dwSize.X, csbiInfo.dwSize.Y);
}


/** ooConsole::setCP()
 *
 *  Sets the input code page used by the console associated with the calling
 *  process.
 *
 *  @return  0 on success, .nil on error.
 */
RexxMethod2(RexxObjectPtr, oocon_setCP, uint32_t, cp, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return TheNilObj;
    }

    if ( SetConsoleCP(cp) == 0 )
    {
        pcon->errRC = GetLastError();
        return TheNilObj;
    }

    return TheZeroObj;
}


/** ooConsole::setCursorInof()
 *
 *  Sets the size and / or visibility of the cursor for this console buffer.
 *
 *  @return  True on success, false on error.
 */
RexxMethod3(RexxObjectPtr, oocon_setCursorInfo, OPTIONAL_logical_t, visible, OPTIONAL_uint8_t, size, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return TheNilObj;
    }

    CONSOLE_CURSOR_INFO ci;
    if ( GetConsoleCursorInfo(pcon->hStdOut, &ci) == 0 )
    {
        pcon->errRC = GetLastError();
        return TheFalseObj;
    }

    if ( argumentExists(1) )
    {
        ci.bVisible = visible ? TRUE : FALSE;
    }
    if ( argumentExists(2) )
    {
        ci.dwSize = size;
    }

    if ( SetConsoleCursorInfo(pcon->hStdOut, &ci) == 0 )
    {
        pcon->errRC = GetLastError();
        return TheFalseObj;
    }

    return TheTrueObj;
}


/** ooConsole::setDisplayMode()
 *
 *
 *  @return  True or false.
 */
RexxMethod3(RexxObjectPtr, oocon_setDisplayMode, CSTRING, _mode, OPTIONAL_RexxObjectPtr, _size, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return TheFalseObj;
    }

    PORXSIZE size = NULL;
    uint32_t mode = 0;
    COORD    coord;

    if ( argumentExists(2) )
    {
        size = rxGetSize(context, _size, 2);
        if ( size == NULL )
        {
            return TheFalseObj;
        }
    }

    if (      StrStrI(_mode, "FULLSCREEN") != NULL ) mode |= CONSOLE_FULLSCREEN_MODE;
    else if ( StrStrI(_mode, "WINDOWED"  ) != NULL ) mode |= CONSOLE_WINDOWED_MODE;

    if ( SetConsoleDisplayMode(pcon->hStdIn, mode, &coord) == 0 )
    {
        pcon->errRC = GetLastError();
        return TheFalseObj;
    }

    if ( argumentExists(2) )
    {
        size->cx = coord.X;
        size->cy = coord.Y;
    }
    return TheTrueObj;
}


/** ooConsole::setHistoryInfo()
 *
 *
 */
RexxMethod4(RexxObjectPtr, oocon_setHistoryInfo, OPTIONAL_uint32_t, buffers, OPTIONAL_uint32_t, commands,
            OPTIONAL_logical_t, duplicates, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return TheFalseObj;
    }

    CONSOLE_HISTORY_INFO  chi = {sizeof(CONSOLE_HISTORY_INFO)};
    if ( GetConsoleHistoryInfo(&chi) == 0 )
    {
        pcon->errRC = GetLastError();
        return TheFalseObj;
    }

    if ( argumentExists(1) )
    {
        chi.NumberOfHistoryBuffers = buffers;
    }
    if ( argumentExists(2) )
    {
        chi.HistoryBufferSize = commands;
    }
    if ( argumentExists(3) )
    {
        chi.dwFlags = duplicates ? 0 : HISTORY_NO_DUP_FLAG;
    }

    if ( SetConsoleHistoryInfo(&chi) == 0 )
    {
        pcon->errRC = GetLastError();
        return TheFalseObj;
    }

    return TheTrueObj;
}


/** ooConsole::setMode()
 *
 *
 *  @return True on success, false on error.
 */
RexxMethod3(RexxObjectPtr, oocon_setMode, OPTIONAL_CSTRING, handle, CSTRING, _mode, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return TheFalseObj;
    }

    HANDLE h = bufferHandle(context, pcon, handle, 1);
    if ( h == NULL )
    {
        return TheFalseObj;
    }

    uint32_t mode = 0;
    if ( h == pcon->hStdIn )
    {
        mode = ENABLE_EXTENDED_FLAGS;
        if ( StrStrI(_mode, "EchoInput") != NULL    )  mode |= ENABLE_ECHO_INPUT;
        if ( StrStrI(_mode, "InsertMode") != NULL   )  mode |= ENABLE_INSERT_MODE;
        if ( StrStrI(_mode, "LineInput") != NULL    )  mode |= ENABLE_LINE_INPUT;
        if ( StrStrI(_mode, "MouseInput") != NULL   )  mode |= ENABLE_MOUSE_INPUT;
        if ( StrStrI(_mode, "ProcessedInput") != NULL) mode |= ENABLE_PROCESSED_INPUT;
        if ( StrStrI(_mode, "EditMode") != NULL     )  mode |= ENABLE_QUICK_EDIT_MODE;
        if ( StrStrI(_mode, "WindowInput") != NULL  )  mode |= ENABLE_WINDOW_INPUT;
    }
    else
    {
        if ( StrStrI(_mode, "ProcessedOutput") !=NULL ) mode |= ENABLE_PROCESSED_OUTPUT;
        if ( StrStrI(_mode, "WrapAtEolOuput")  !=NULL ) mode |= ENABLE_WRAP_AT_EOL_OUTPUT;
    }

    if ( SetConsoleMode(h, mode) == 0 )
    {
        pcon->errRC = GetLastError();
        return TheFalseObj;
    }

    return TheTrueObj;
}


/** ooConsole::setOutputCP()
 *
 *  Sets the output code page used by the console associated with the calling
 *  process.
 *
 *  @return  0 on success, .nil on error.
 */
RexxMethod2(RexxObjectPtr, oocon_setOutputCP, uint32_t, cp, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return TheNilObj;
    }

    if ( SetConsoleOutputCP(cp) == 0 )
    {
        pcon->errRC = GetLastError();
        return TheNilObj;
    }

    return TheZeroObj;
}


/** ooConsole::setScreenBuffersize()
 *
 *
 */
RexxMethod2(RexxObjectPtr, oocon_setScreenBufferSize, ARGLIST, args, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return TheNilObj;
    }

    size_t sizeArray;
    size_t argsUsed;
    ORXSIZE  size;
    if ( ! getSizeFromArglist(context, args, (PORXPOINT)&size, 1, 2, &sizeArray, &argsUsed) )
    {
        return TheNilObj;
    }

    // Check arg count against expected.
    if ( sizeArray > argsUsed )
    {
        tooManyArgsException(context->threadContext, argsUsed);
        return TheNilObj;
    }

    COORD coord;
    size2coord(&size, &coord);
    if ( SetConsoleScreenBufferSize(pcon->hStdOut, coord) == 0 )
    {
        pcon->errRC = GetLastError();
        return TheNilObj;
    }

    return TheZeroObj;
}


/** ooConsole::setTitle()
 *
 *  Sets the title of the console window
 *
 */
RexxMethod2(RexxObjectPtr, oocon_setTitle, CSTRING, title, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return TheFalseObj;
    }

    if ( SetConsoleTitle(title) == 0 )
    {
        pcon->errRC = GetLastError();
        return TheFalseObj;
    }

    return TheTrueObj;
}


/** ooConsole::setWindowRect()
 *
 *
 */
RexxMethod3(RexxObjectPtr, oocon_setWindowRect, RexxObjectPtr, _rect, OPTIONAL_logical_t, absolute, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return TheNilObj;
    }

    PORXRECT rect = rxGetRect(context, _rect, 1);
    if ( rect == NULLOBJECT )
    {
        return TheNilObj;
    }

    SMALL_RECT sRect;
    rect2smallRect(rect, &sRect);
    if ( SetConsoleWindowInfo(pcon->hStdOut, absolute ? TRUE : FALSE, &sRect) == 0 )
    {
        pcon->errRC = GetLastError();
        return TheNilObj;
    }

    return TheZeroObj;
}


/** ooConsole::setWindowSize()
 *
 *
 */
RexxMethod2(RexxObjectPtr, oocon_setWindowSize, RexxObjectPtr, _size, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return TheNilObj;
    }

    PORXSIZE size = rxGetSize(context, _size, 1);
    if ( size == NULLOBJECT )
    {
        return TheNilObj;
    }

    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
    if ( GetConsoleScreenBufferInfo(pcon->hStdOut, &csbiInfo) == 0 )
    {
        pcon->errRC = GetLastError();
        return TheNilObj;
    }

    SMALL_RECT sRect;

    sRect.Left   = csbiInfo.srWindow.Left;
    sRect.Top    = csbiInfo.srWindow.Top;
    sRect.Right  = csbiInfo.srWindow.Left + (uint16_t)size->cx - 1;
    sRect.Bottom = csbiInfo.srWindow.Top + (uint16_t)size->cy - 1;

    if ( SetConsoleWindowInfo(pcon->hStdOut, TRUE, &sRect) == 0 )
    {
        pcon->errRC = GetLastError();
        return TheNilObj;
    }

    return TheZeroObj;
}


/**
 * Internal use only.
 *
 * Transient method used to test various things as they come up.
 *
 */
RexxMethod1(int, oocon_test, ARGLIST, args)
{
    RexxMethodContext *c = context;

    size_t size = c->ArraySize(args);

    printf("No tests at this time\n");
    return 0;
}




// .ooConsole
REXX_METHOD_PROTOTYPE(oocon_version_cls);

REXX_METHOD_PROTOTYPE(oocon_errRC_atr);

REXX_METHOD_PROTOTYPE(oocon_init);
REXX_METHOD_PROTOTYPE(oocon_uninit);
REXX_METHOD_PROTOTYPE(oocon_getCP);
REXX_METHOD_PROTOTYPE(oocon_getCursorInfo);
REXX_METHOD_PROTOTYPE(oocon_getDisplayMode);
REXX_METHOD_PROTOTYPE(oocon_getFontSize);
REXX_METHOD_PROTOTYPE(oocon_getHistoryInfo);
REXX_METHOD_PROTOTYPE(oocon_getMode);
REXX_METHOD_PROTOTYPE(oocon_getOriginalTitle);
REXX_METHOD_PROTOTYPE(oocon_getOutputCP);
REXX_METHOD_PROTOTYPE(oocon_getTitle);
REXX_METHOD_PROTOTYPE(oocon_getProcessList);
REXX_METHOD_PROTOTYPE(oocon_getWindow);
REXX_METHOD_PROTOTYPE(oocon_getScreenBufferSize);
REXX_METHOD_PROTOTYPE(oocon_getWindowRect);
REXX_METHOD_PROTOTYPE(oocon_getWindowSize);
REXX_METHOD_PROTOTYPE(oocon_setCP);
REXX_METHOD_PROTOTYPE(oocon_setCursorInfo);
REXX_METHOD_PROTOTYPE(oocon_setDisplayMode);
REXX_METHOD_PROTOTYPE(oocon_setHistoryInfo);
REXX_METHOD_PROTOTYPE(oocon_setMode);
REXX_METHOD_PROTOTYPE(oocon_setOutputCP);
REXX_METHOD_PROTOTYPE(oocon_setScreenBufferSize);
REXX_METHOD_PROTOTYPE(oocon_setTitle);
REXX_METHOD_PROTOTYPE(oocon_setWindowRect);
REXX_METHOD_PROTOTYPE(oocon_setWindowSize);

REXX_METHOD_PROTOTYPE(oocon_test);

RexxMethodEntry ooConsole_methods[] = {
    REXX_METHOD(oocon_version_cls,                oocon_version_cls),
    REXX_METHOD(oocon_errRC_atr,                  oocon_errRC_atr),
    REXX_METHOD(oocon_init,                       oocon_init),
    REXX_METHOD(oocon_uninit,                     oocon_uninit),
    REXX_METHOD(oocon_getCP,                      oocon_getCP),
    REXX_METHOD(oocon_getCursorInfo,		      oocon_getCursorInfo),
    REXX_METHOD(oocon_getDisplayMode,             oocon_getDisplayMode),
    REXX_METHOD(oocon_getFontSize,                oocon_getFontSize),
    REXX_METHOD(oocon_getHistoryInfo,             oocon_getHistoryInfo),
    REXX_METHOD(oocon_getMode,                    oocon_getMode),
    REXX_METHOD(oocon_getOriginalTitle,           oocon_getOriginalTitle),
    REXX_METHOD(oocon_getOutputCP,                oocon_getOutputCP),
    REXX_METHOD(oocon_getProcessList,             oocon_getProcessList),
    REXX_METHOD(oocon_getTitle,                   oocon_getTitle),
    REXX_METHOD(oocon_getWindow,                  oocon_getWindow),
    REXX_METHOD(oocon_getScreenBufferSize,        oocon_getScreenBufferSize),
    REXX_METHOD(oocon_getWindowRect,              oocon_getWindowRect),
    REXX_METHOD(oocon_getWindowSize,              oocon_getWindowSize),
    REXX_METHOD(oocon_setCP,                      oocon_setCP),
    REXX_METHOD(oocon_setCursorInfo,		      oocon_setCursorInfo),
    REXX_METHOD(oocon_setDisplayMode,             oocon_setDisplayMode),
    REXX_METHOD(oocon_setHistoryInfo,             oocon_setHistoryInfo),
    REXX_METHOD(oocon_setMode,                    oocon_setMode),
    REXX_METHOD(oocon_setOutputCP,                oocon_setOutputCP),
    REXX_METHOD(oocon_setScreenBufferSize,        oocon_setScreenBufferSize),
    REXX_METHOD(oocon_setTitle,	                  oocon_setTitle),
    REXX_METHOD(oocon_setWindowRect,              oocon_setWindowRect),
    REXX_METHOD(oocon_setWindowSize,              oocon_setWindowSize),
    REXX_METHOD(oocon_test,                       oocon_test),
    REXX_LAST_METHOD()
};

RexxPackageEntry ooConsole_package_entry =
{
    STANDARD_PACKAGE_HEADER
    REXX_INTERPRETER_4_1_0,              // needs at least the 4.1.0 interpreter
    "ooConsole",                         // name of the package
    "0.0.1",                             // package information
    ooConsoleLoad,                       // package load function
    ooConsoleUnload,                     // package unload function
    NULL,                                // no exported functions
    ooConsole_methods                    // the exported methods
};

OOREXX_GET_PACKAGE(ooConsole);
