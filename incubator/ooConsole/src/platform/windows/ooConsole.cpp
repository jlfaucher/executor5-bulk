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
 * Checks that the current Os meets the minimum OS requirements for a method.
 * Raises an exception if the minimum is not meet.
 *
 * @param context
 * @param method
 * @param os name
 * @param os type
 *
 * @return True if the requirement is meet, otherwise false.
 */
static bool requiredOS(RexxMethodContext *context, const char *method, const char *osName, os_name_t os)
{
    bool ok = false;
    switch ( os )
    {
        case XP_OS :
            ok = _isAtLeastXP();
            break;

        case Vista_OS :
            ok = _isAtLeastVista();
            break;

        case Windows7_OS :
            ok = _isAtLeastWindows7();
            break;

        default :
            break;

    }
    if ( ! ok )
    {
        char buf[256];

        _snprintf(buf, sizeof(buf), "The %s() method requires Windows %s or later", method, osName);
        context->RaiseException1(Rexx_Error_Incorrect_method_user_defined, context->String(buf));
        return false;
    }
    return true;
}

/**
 * Allocates a buffer and converts a wide character (Unicode) string to an Ansi
 * string.
 *
 * @param wstr    The string to convert.
 *
 * @return The converted string, or null on error.
 *
 * @note  The caller is responsible for freeing the returned string.  Memory is
 *        allocated using LocalAlloc.
 *
 *        If NULL is returned, errRC is set to some error code.
 */
static char *unicode2ansi(LPWSTR wstr, pCooConsole pcon)
{
    if (wstr == NULL)
    {
        pcon->errRC = ERROR_INVALID_PARAMETER;
        return NULL;
    }

    char *ansiStr = NULL;
    int32_t neededLen = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);

    if ( neededLen != 0 )
    {
        ansiStr = (char *)LocalAlloc(LPTR, neededLen);
        if ( ansiStr == NULL )
        {
            pcon->errRC = GetLastError();
        }
        else if ( WideCharToMultiByte(CP_ACP, 0, wstr, -1, ansiStr, neededLen, NULL, NULL) == 0 )
        {
            /* conversion failed */
            pcon->errRC = GetLastError();
            LocalFree(ansiStr);
            ansiStr = NULL;
        }
    }
    else
    {
        pcon->errRC = GetLastError();
    }

    return ansiStr;
}

/**
 *  Puts an ANSI character string converted to a wide (Unicode) character string
 *  in the specified buffer.
 *
 *  This is a convenience function that assumes the caller has passed a buffer
 *  known to be big enough.
 *
 *  It works correctly for the empty string "" and is designed to treat a null
 *  pointer for text as an error.
 *
 * @param dest  Buffer in which to place the converted string.  Must be big
 *              enough.
 * @param text  The text to convert.
 * @param pHR   Pointer to a return code. This is set on error to the system
 *              error code, or to 0x8000ffff Catastrophic failure if text is
 *              null.
 *
 * @return The number of wide character values copied to the buffer.  On error
 *         this is zero.
 */
static uint32_t putUnicodeText(LPWORD dest, const char *text, HRESULT *pHR)
{
    uint32_t count = 0;
    if ( text == NULL )
    {
        *pHR = 0x8000ffff; // Catastrophic failure
    }
    else
    {
        int countWideChars = (int)strlen(text) + 1;

        count = MultiByteToWideChar(CP_ACP, 0, text, -1, (LPWSTR)dest, countWideChars);
        if ( count == 0 )
        {
            *pHR = HRESULT_FROM_WIN32(GetLastError());
        }
    }
    return count;
}


inline void size2coord(PORXSIZE s, PCOORD c)
{
    c->X = (uint16_t)s->cx;
    c->Y = (uint16_t)s->cy;
}

inline void point2coord(PORXPOINT p, PCOORD c)
{
    c->X = (uint16_t)p->x;
    c->Y = (uint16_t)p->y;
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

static bool rxGetSmallRect(RexxMethodContext *context, RexxObjectPtr r, size_t argPos, PSMALL_RECT sm)
{
    PORXRECT rect = rxGetRect(context, r, argPos);
    if ( rect != NULL )
    {
        rect2smallRect(rect, sr);
        return true;
    }
    return false;
}

/**
 *  Converts a console character attribute to a Rexx string.
 *
 */
static RexxStringObject indicator2string(RexxMethodContext *c, uint32_t indicator)
{
    char buf[512] = {'\0'};

    if ( indicator == CONSOLE_NO_SELECTION )
    {
        strcat(buf, "NoSelection ");
    }
    else
    {
        if ( (indicator & CONSOLE_MOUSE_DOWN)            == CONSOLE_MOUSE_DOWN   )          strcat(buf, "MouseDown ");
        if ( (indicator & CONSOLE_MOUSE_SELECTION)       == CONSOLE_MOUSE_SELECTION )       strcat(buf, "MouseSelection ");
        if ( (indicator & CONSOLE_SELECTION_IN_PROGRESS) == CONSOLE_SELECTION_IN_PROGRESS ) strcat(buf, "InProgress ");
        if ( (indicator & CONSOLE_SELECTION_NOT_EMPTY)   == CONSOLE_SELECTION_NOT_EMPTY )   strcat(buf, "NotEmpty ");
    }

    if ( *buf != '\0' )
    {
        *(buf + strlen(buf) - 1) = '\0';
    }
    return c->String(buf);
}

/**
 *  Converts a font family flag to a Rexx string.
 *
 */
static RexxStringObject family2string(RexxMethodContext *c, uint32_t family)
{
    char *buf = "NoMatch";

    if (      family == FF_DECORATIVE ) buf = "Decorative";
    else if ( family == FF_DONTCARE )   buf = "DontCare";
    else if ( family == FF_MODERN )     buf = "Modern";
    else if ( family == FF_ROMAN )      buf = "Roman";
    else if ( family == FF_SCRIPT )     buf = "Script";
    else if ( family == FF_SWISS ) 	    buf = "Swiss";

    return c->String(buf);
}

/**
 *  Converts a Rexx string to a font family flag.
 *
 */
static uint32_t string2family(RexxMethodContext *c, RexxObjectPtr _family, size_t argPos)
{
    uint32_t family = FF_DONTCARE;
    char     *buf   = (char *)c->ObjectToStringValue(_family);

    if (      StrCmpI(buf, "Decorative") == 0 ) family = FF_DECORATIVE;
    else if ( StrCmpI(buf, "DontCare"  ) == 0 ) family = FF_DONTCARE;
    else if ( StrCmpI(buf, "Modern"    ) == 0 ) family = FF_MODERN;
    else if ( StrCmpI(buf, "Roman"     ) == 0 ) family = FF_ROMAN;
    else if ( StrCmpI(buf, "Script"    ) == 0 ) family = FF_SCRIPT;
    else if ( StrCmpI(buf, "Swiss"     ) == 0 ) family = FF_SWISS;
    else family = FF_DONTCARE;

    return family;
}

/**
 *  Converts a font weight flag to a Rexx string.
 *
 */
static RexxStringObject weight2string(RexxMethodContext *c, uint32_t weight)
{
    char *buf = "NoMatch";

    if (      weight == FW_DONTCARE   )     buf = "DontCare";
    else if ( weight == FW_THIN       )     buf = "Thin";
    else if ( weight == FW_EXTRALIGHT )     buf = "ExtraLight";
    else if ( weight == FW_LIGHT      )     buf = "Light";
    else if ( weight == FW_NORMAL     )     buf = "Normal";
    else if ( weight == FW_MEDIUM     )     buf = "Medium";
    else if ( weight == FW_SEMIBOLD   )     buf = "SemiBold";
    else if ( weight == FW_BOLD       )     buf = "Bold";
    else if ( weight == FW_EXTRABOLD  )     buf = "ExtraBold";
    else if ( weight == FW_HEAVY      )     buf = "Heavy";

    return c->String(buf);
}

/**
 *  Converts a Rexx String to a font weight flag.
 *
 */
static uint32_t string2weight(RexxMethodContext *c, RexxObjectPtr _weight, size_t argPos)
{
    uint32_t weight = FW_DONTCARE;
    char     *buf   = (char *)c->ObjectToStringValue(_weight);

    if (      StrCmpI(buf, "DontCare"  ) == 0  ) weight = FW_DONTCARE;
    else if ( StrCmpI(buf, "Thin"      ) == 0  ) weight = FW_THIN;
    else if ( StrCmpI(buf, "ExtraLight") == 0  ) weight = FW_EXTRALIGHT;
    else if ( StrCmpI(buf, "Light"     ) == 0  ) weight = FW_LIGHT;
    else if ( StrCmpI(buf, "Normal"    ) == 0  ) weight = FW_NORMAL;
    else if ( StrCmpI(buf, "Medium"    ) == 0  ) weight = FW_MEDIUM;
    else if ( StrCmpI(buf, "SemiBold"  ) == 0  ) weight = FW_SEMIBOLD;
    else if ( StrCmpI(buf, "Bold"      ) == 0  ) weight = FW_BOLD;
    else if ( StrCmpI(buf, "ExtraBold" ) == 0  ) weight = FW_EXTRABOLD;
    else if ( StrCmpI(buf, "Heavy"     ) == 0  ) weight = FW_HEAVY;
    else weight = FW_DONTCARE;

    return weight;
}

/**
 *  Converts a console character attribute to a Rexx string.
 *
 */
static RexxStringObject attribute2string(RexxMethodContext *c, uint32_t attr)
{
    char buf[512] = {'\0'};

    if ( (attr & BACKGROUND_BLUE)      == BACKGROUND_BLUE      ) strcat(buf, "BgBlue ");
    if ( (attr & BACKGROUND_RED)       == BACKGROUND_RED       ) strcat(buf, "BgRed ");
    if ( (attr & BACKGROUND_GREEN)     == BACKGROUND_GREEN     ) strcat(buf, "BgGreen ");
    if ( (attr & BACKGROUND_INTENSITY) == BACKGROUND_INTENSITY ) strcat(buf, "BgIntensity ");
    if ( (attr & FOREGROUND_BLUE)      == FOREGROUND_BLUE      ) strcat(buf, "FgBlue ");
    if ( (attr & FOREGROUND_RED)       == FOREGROUND_RED       ) strcat(buf, "FgRed ");
    if ( (attr & FOREGROUND_GREEN)     == FOREGROUND_GREEN     ) strcat(buf, "FgGreen ");
    if ( (attr & FOREGROUND_INTENSITY) == FOREGROUND_INTENSITY ) strcat(buf, "FgIntensity ");

    if ( *buf != '\0' )
    {
        *(buf + strlen(buf) - 1) = '\0';
    }
    return c->String(buf);
}

/**
 *  Converts a string to a console character attribute.
 *
 */
static uint32_t string2attributes(CSTRING attr)
{
    uint32_t attributes = 0;

    if ( StrStrI(attr, "BgBlue")      != NULL ) attributes |= BACKGROUND_BLUE;
    if ( StrStrI(attr, "BgRed")       != NULL ) attributes |= BACKGROUND_RED;
    if ( StrStrI(attr, "BgGreen")     != NULL ) attributes |= BACKGROUND_GREEN;
    if ( StrStrI(attr, "BgIntensity") != NULL ) attributes |= BACKGROUND_INTENSITY;
    if ( StrStrI(attr, "FgBlue")      != NULL ) attributes |= FOREGROUND_BLUE;
    if ( StrStrI(attr, "Fgred")       != NULL ) attributes |= FOREGROUND_RED;
    if ( StrStrI(attr, "FgGreen")     != NULL ) attributes |= FOREGROUND_GREEN;
    if ( StrStrI(attr, "FgIntensity") != NULL ) attributes |= FOREGROUND_INTENSITY;

    return attributes;
}

/**
 *  Converts a string containing input record types to a flag.
 *
 */
static uint32_t string2inputRecs(CSTRING types)
{
    uint32_t recTypes = 0;

    if ( StrStrI(types, "All")      != NULL )
    {
        recTypes = KEY_EVENT | MOUSE_EVENT | FOCUS_EVENT | MENU_EVENT | WINDOW_BUFFER_SIZE_EVENT;
    }
    else
    {
        if ( StrStrI(types, "KEY")    != NULL ) recTypes |= KEY_EVENT;
        if ( StrStrI(types, "MOUSE")  != NULL ) recTypes |= MOUSE_EVENT;
        if ( StrStrI(types, "FOCUS")  != NULL ) recTypes |= FOCUS_EVENT;
        if ( StrStrI(types, "MENU")   != NULL ) recTypes |= MENU_EVENT;
        if ( StrStrI(types, "WINDOW") != NULL ) recTypes |= WINDOW_BUFFER_SIZE_EVENT;
    }

    return recTypes;
}

/**
 *  Converts the control key state flags to a string.
 *
 */
static RexxStringObject ctrlKeyState2string(RexxMethodContext *c, uint32_t ctrlState)
{
    char buf[512] = {'\0'};

    if ( (ctrlState & RIGHT_ALT_PRESSED ) ) strcat(buf, "rAlt ");
    if ( (ctrlState & LEFT_ALT_PRESSED  ) ) strcat(buf, "lAlt ");
    if ( (ctrlState & RIGHT_CTRL_PRESSED) ) strcat(buf, "rCtrl ");
    if ( (ctrlState & LEFT_CTRL_PRESSED ) ) strcat(buf, "lCtrl ");
    if ( (ctrlState & SHIFT_PRESSED     ) ) strcat(buf, "shift ");
    if ( (ctrlState & NUMLOCK_ON        ) ) strcat(buf, "numLock ");
    if ( (ctrlState & SCROLLLOCK_ON     ) ) strcat(buf, "scrollLock ");
    if ( (ctrlState & CAPSLOCK_ON       ) ) strcat(buf, "capsLock ");
    if ( (ctrlState & ENHANCED_KEY      ) ) strcat(buf, "enhanced ");

    if ( *buf != '\0' )
    {
        *(buf + strlen(buf) - 1) = '\0';
    }
    return c->String(buf);
}

/**
 *  Converts the mouse button state flags to a string.
 *
 */
static RexxStringObject buttonState2string(RexxMethodContext *c, uint32_t button)
{
    char buf[512] = {'\0'};

    if ( (button & FROM_LEFT_1ST_BUTTON_PRESSED ) ) strcat(buf, "leftMost1 ");
    if ( (button & RIGHTMOST_BUTTON_PRESSED     ) ) strcat(buf, "rightMost ");
    if ( (button & FROM_LEFT_2ND_BUTTON_PRESSED ) ) strcat(buf, "leftMost2 ");
    if ( (button & FROM_LEFT_3RD_BUTTON_PRESSED ) ) strcat(buf, "leftMost3 ");
    if ( (button & FROM_LEFT_4TH_BUTTON_PRESSED ) ) strcat(buf, "leftMost4 ");

    if ( *buf != '\0' )
    {
        *(buf + strlen(buf) - 1) = '\0';
    }
    return c->String(buf);
}

/**
 *  Converts the mouse event flag to a string.
 *
 */
static RexxStringObject eventFlags2string(RexxMethodContext *c, uint32_t flags)
{
    char buf[64] = {'\0'};

    if (      flags == 0 )              strcpy(buf, "pressOrRelease");
    else if ( flags == MOUSE_MOVED )    strcpy(buf, "mouseMoved");
    else if ( flags == DOUBLE_CLICK )   strcpy(buf, "doubleClick");
    else if ( flags == MOUSE_WHEELED )  strcpy(buf, "mouseVWheeled");
    else if ( flags == MOUSE_HWHEELED ) strcpy(buf, "mouseHWheeled");

    return c->String(buf);
}

/**
 * Adds a directory object to the data array that contains the information in
 * the KEY_EVENT record.
 *
 * @param c
 * @param data
 * @param rec
 */
static void addKeyInputRec(RexxMethodContext *c, RexxArrayObject data, KEY_EVENT_RECORD *rec)
{
    RexxDirectoryObject d = c->NewDirectory();

    RexxStringObject flag = ctrlKeyState2string(c, rec->dwControlKeyState);

    char buf[2] = {'\0'};
    buf[0]      = rec->uChar.AsciiChar;

    c->DirectoryPut(d, c->String("key"), "EVENT");
    c->DirectoryPut(d, c->Logical(rec->bKeyDown), "KEYDOWN");
    c->DirectoryPut(d, c->UnsignedInt32(rec->wVirtualKeyCode), "VKEYCODE");
    c->DirectoryPut(d, c->UnsignedInt32(rec->wVirtualScanCode), "VSCANCODE");
    c->DirectoryPut(d, c->UnsignedInt32(rec->wRepeatCount), "REPEATCOUNT");
    c->DirectoryPut(d, c->String(buf), "CHAR");
    c->DirectoryPut(d, flag, "CONTROLKEYS");

    c->ArrayAppend(data, d);
}

/**
 * Adds a directory object to the data array that contains the information in
 * the MOUSE_EVENT record.
 *
 * @param c
 * @param data
 * @param rec
 */
static void addMouseInputRec(RexxMethodContext *c, RexxArrayObject data, MOUSE_EVENT_RECORD *rec)
{
    RexxDirectoryObject d = c->NewDirectory();

    RexxObjectPtr    point       = rxNewPoint(c, rec->dwMousePosition.X, rec->dwMousePosition.Y);
    RexxStringObject buttonState = buttonState2string(c, rec->dwButtonState);
    RexxStringObject ctrlState   = ctrlKeyState2string(c, rec->dwControlKeyState);
    RexxStringObject eventFlags  = eventFlags2string(c, rec->dwEventFlags);

    c->DirectoryPut(d, c->String("mouse"), "EVENT");
    c->DirectoryPut(d, point, "POSITION");
    c->DirectoryPut(d, buttonState, "BUTTONSTATE");
    c->DirectoryPut(d, ctrlState, "CONTROLKEYS");
    c->DirectoryPut(d, eventFlags, "EVENTFLAGS");

    c->ArrayAppend(data, d);
}

/**
 * Adds a directory object to the data array that contains the information in
 * the FOCUS_EVENT record.
 *
 * @param c
 * @param data
 * @param rec
 */
static void addFocusInputRec(RexxMethodContext *c, RexxArrayObject data, FOCUS_EVENT_RECORD *rec)
{
    RexxDirectoryObject d = c->NewDirectory();

    c->DirectoryPut(d, c->String("focus"), "EVENT");
    c->DirectoryPut(d, c->Logical(rec->bSetFocus), "SETFOCUS");
    c->ArrayAppend(data, d);
}

/**
 * Adds a directory object to the data array that contains the information in
 * the MENU_EVENT record.
 *
 * @param c
 * @param data
 * @param rec
 */
static void addMenuInputRec(RexxMethodContext *c, RexxArrayObject data, MENU_EVENT_RECORD *rec)
{
    RexxDirectoryObject d = c->NewDirectory();

    c->DirectoryPut(d, c->String("menu"), "EVENT");
    c->DirectoryPut(d, c->UnsignedInt32(rec->dwCommandId), "ID");
    c->ArrayAppend(data, d);
}

/**
 * Adds a directory object to the data array that contains the information in
 * the WINDOW_BUFFER_SIZE record.
 *
 * @param c
 * @param data
 * @param rec
 */
static void addWindowInputRec(RexxMethodContext *c, RexxArrayObject data, WINDOW_BUFFER_SIZE_RECORD *rec)
{
    RexxDirectoryObject d = c->NewDirectory();

    RexxObjectPtr size = rxNewSize(c, rec->dwSize.X, rec->dwSize.Y);

    c->DirectoryPut(d, c->String("window"), "EVENT");
    c->DirectoryPut(d, size, "SIZE");
    c->ArrayAppend(data, d);
}

/**
 * Builds a Rexx array of records representing the passed in C++ array of
 * records.
 *
 * @param c
 * @param inBuf
 * @param countRead
 * @param filter
 *
 * @return RexxArrayObject
 *
 * @note  This function is used for both peek() and readInput().
 */
static RexxObjectPtr createInputRecords(RexxMethodContext *c, INPUT_RECORD *inBuf, uint32_t countRead,
                                        uint32_t filter, pCooConsole pcon)
{
    RexxArrayObject data = c->NewArray(countRead);

    for ( size_t i = 0; i < countRead; i++ )
    {
        switch( inBuf[i].EventType )
        {
            case KEY_EVENT :
            {
                if ( filter & KEY_EVENT )
                {
                    KEY_EVENT_RECORD rec = inBuf[i].Event.KeyEvent;
                    addKeyInputRec(c, data, &rec);
                }

                break;
            }

            case MOUSE_EVENT :
            {
                if ( filter & MOUSE_EVENT )
                {
                    MOUSE_EVENT_RECORD rec = inBuf[i].Event.MouseEvent;
                    addMouseInputRec(c, data, &rec);
                }

                break;
            }

            case WINDOW_BUFFER_SIZE_EVENT :
            {
                if ( filter & WINDOW_BUFFER_SIZE_EVENT )
                {
                    WINDOW_BUFFER_SIZE_RECORD rec = inBuf[i].Event.WindowBufferSizeEvent;
                    addWindowInputRec(c, data, &rec);
                }

                break;
            }

            case FOCUS_EVENT:
            {
                if ( filter & FOCUS_EVENT )
                {
                    FOCUS_EVENT_RECORD rec = inBuf[i].Event.FocusEvent;
                    addFocusInputRec(c, data, &rec);
                }

                break;
            }

            case MENU_EVENT:
            {
                if ( filter & MENU_EVENT )
                {
                    MENU_EVENT_RECORD rec = inBuf[i].Event.MenuEvent;
                    addMenuInputRec(c, data, &rec);
                }

                break;
            }

            default:
            {
                pcon->errRC = ERROR_INVALID_DATA;
                return TheNilObj;
            }
        }
    }

    return data;
}

/**
 * Gets the standard handle for this console.
 *
 * @param c
 * @param pcon
 *
 * @return bool
 *
 * @assumes  The caller has checked for a 'created' screen buffer object
 */
static bool goodHandle(RexxMethodContext *c, pCooConsole pcon)
{
    pcon->errRC = 0;

    switch ( pcon->type )
    {
        case  STDOUTPUT:
            pcon->handle = GetStdHandle(STD_OUTPUT_HANDLE);
            if ( pcon->handle == INVALID_HANDLE_VALUE || pcon->handle == NULL )
            {
                pcon->errRC = GetLastError();
                pcon->isValid = false;
                return false;
            }
            break;

        case STDERROR :
            pcon->handle = GetStdHandle(STD_ERROR_HANDLE);
            if ( pcon->handle == INVALID_HANDLE_VALUE || pcon->handle == NULL )
            {
                pcon->errRC = GetLastError();
                pcon->isValid = false;
                return false;
            }
            break;

        case STDINPUT :
            pcon->handle  = GetStdHandle(STD_INPUT_HANDLE);
            if ( pcon->handle == INVALID_HANDLE_VALUE || pcon->handle == NULL )
            {
                pcon->errRC = GetLastError();
                pcon->isValid = false;
                return false;
            }
            break;

        default :
            // This realy can't happen ...
            pcon->errRC = ERROR_INVALID_HANDLE;
            pcon->isValid = false;
            return false;
    }

    pcon->isValid = true;
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
    pCooConsole pcon = ensureCSelf(c, pCSelf);
    if ( pcon != NULL )
    {
        if ( pcon->isValid )
        {
            pcon->errRC = 0;
            if ( pcon->isCreated )
            {
                return pcon;
            }
            if ( goodHandle(c, pcon) )
            {
                return pcon;
            }
        }
        pcon = NULL;
    }
    return pcon;
}

/**
 * Parses the string 'handle' and returns which of the 3 'real' standard handles
 * the user specifies.  The default if omitted is STDOUT.
 *
 * @param context
 * @param handle
 * @param pos
 *
 * @return The specified handle on success, .nil on an OS error, or NULL on user
 *         error.
 *
 * @note The 'real' handle simply means that we explicitly query the handle,
 *       rather that returning the 'stashed' handle.
 */
static RexxObjectPtr realBufferHandle(RexxMethodContext *context, pCooConsole pcon, CSTRING handle, size_t pos)
{
    HANDLE h;
    if ( argumentExists(pos) )
    {
        if ( stricmp(handle, "OUT") == 0 )
        {
             h = GetStdHandle(STD_OUTPUT_HANDLE);
        }
        else if ( stricmp(handle, "ERR") == 0 )
        {
            h = GetStdHandle(STD_ERROR_HANDLE);
        }
        else if ( stricmp(handle, "IN") == 0 )
        {
            h = GetStdHandle(STD_INPUT_HANDLE);
        }
        else
        {
            wrongArgValueException(context->threadContext, pos, "Out, Err, In", handle);
            return NULL;
        }
    }
    else
    {
        h = GetStdHandle(STD_OUTPUT_HANDLE);
    }

    if ( h == INVALID_HANDLE_VALUE )
    {
        pcon->errRC = GetLastError();
        return TheNilObj;
    }
    return pointer2string(context, h);
}

/**
 * Set the specified standard handle.  The default if the user omitted the
 * handler speicifier is STDOUT.
 *
 * @param context
 * @param handle
 * @param which
 * @param pos
 *
 * @return True on success and false on error.
 */
static bool setRealBufferHandle(RexxMethodContext *context, pCooConsole pcon, HANDLE handle, CSTRING which, size_t pos)
{
    uint32_t type = STD_OUTPUT_HANDLE;
    if ( argumentExists(pos) )
    {
        if ( stricmp(which, "OUT") == 0 )
        {
             type = STD_OUTPUT_HANDLE;
        }
        else if ( stricmp(which, "ERR") == 0 )
        {
            type = STD_ERROR_HANDLE;
        }
        else if ( stricmp(which, "IN") == 0 )
        {
            type = STD_INPUT_HANDLE;
        }
        else
        {
            wrongArgValueException(context->threadContext, pos, "Out, Err, In", which);
            return false;
        }
    }

    if ( SetStdHandle(type, handle) == 0 )
    {
        pcon->errRC = GetLastError();
        return false;
    }
    return true;
}

static bool directoryPoint2coord(RexxMethodContext *c, RexxDirectoryObject info, CSTRING index, COORD *coord)
{
    RexxObjectPtr rxObj = c->DirectoryAt(info, index);
    if ( rxObj != NULLOBJECT )
    {
        if ( ! c->IsOfType(rxObj, "POINT") )
        {
            wrongObjInDirectoryException(c->threadContext, 1, index, "POINT", rxObj);
            return false;
        }
        PORXPOINT p = (PORXPOINT)c->ObjectToCSelf(rxObj);
        coord->X = (SHORT)p->x;
        coord->Y = (SHORT)p->y;
    }
    return true;
}

static bool directorySize2coord(RexxMethodContext *c, RexxDirectoryObject info, CSTRING index, COORD *coord)
{
    RexxObjectPtr rxObj = c->DirectoryAt(info, index);
    if ( rxObj != NULLOBJECT )
    {
        if ( ! c->IsOfType(rxObj, "SIZE") )
        {
            wrongObjInDirectoryException(c->threadContext, 1, index, "SIZE", rxObj);
            return false;
        }
        PORXSIZE s = (PORXSIZE)c->ObjectToCSelf(rxObj);
        coord->X = (SHORT)s->cx;
        coord->Y = (SHORT)s->cy;
    }
    return true;
}

static bool directoryRect2sRect(RexxMethodContext *c, RexxDirectoryObject info, CSTRING index, SMALL_RECT *rect)
{
    RexxObjectPtr rxObj = c->DirectoryAt(info, index);
    if ( rxObj != NULLOBJECT )
    {
        if ( ! c->IsOfType(rxObj, "RECT") )
        {
            wrongObjInDirectoryException(c->threadContext, 1, index, "RECT", rxObj);
            return false;
        }
        PORXRECT r = (PORXRECT)c->ObjectToCSelf(rxObj);
        rect->Left   = (SHORT)r->left;
        rect->Top    = (SHORT)r->top;
        rect->Right  = (SHORT)r->right;
        rect->Bottom = (SHORT)r->bottom;
    }
    return true;
}


/**
 *
 * The init() method implementation for StdOutput, StdError, and StdInput.
 *
 * @param c
 * @param super
 * @param type
 * @param hOutput
 *
 * @return RexxObjectPtr
 */
static RexxObjectPtr genericStdInit(RexxMethodContext *c, RexxClassObject super, console_type_t type, RexxObjectPtr hOutput)
{
    // Get a buffer for the ooConsole CSelf.
    RexxBufferObject cselfBuffer = c->NewBuffer(sizeof(CooConsole));
    if ( cselfBuffer == NULLOBJECT )
    {
        outOfMemoryException(c->threadContext);
        return TheOneObj;
    }

    pCooConsole pcon = (pCooConsole)c->BufferData(cselfBuffer);
    memset(pcon, 0, sizeof(CooConsole));
    pcon->isValid = true;
    pcon->type    = type;

    // Forward the init message to our superclass
    RexxArrayObject newArgs = c->ArrayOfOne(cselfBuffer);
    RexxObjectPtr   result  = c->ForwardMessage(NULL, NULL, super, newArgs);

    if ( result == TheZeroObj )
    {
        if ( hOutput == NULLOBJECT )
        {
            uint32_t handleType = STD_OUTPUT_HANDLE;
            switch ( type )
            {
                case STDERROR : handleType = STD_ERROR_HANDLE;
                case STDINPUT : handleType = STD_INPUT_HANDLE;
            }

            pcon->handle = GetStdHandle(handleType);
            if ( pcon->handle == INVALID_HANDLE_VALUE || pcon->handle == NULL )
            {
                pcon->errRC = GetLastError();
                pcon->isValid = false;
                result = TheOneObj;
            }
        }
        else
        {
            if ( ! c->IsPointer(hOutput) )
            {
                wrongClassException(c->threadContext, 1, "Pointer");
                pcon->errRC = ERROR_INVALID_HANDLE;
                pcon->isValid = false;
                result = TheOneObj;
            }
            else
            {
                pcon->handle = (HANDLE)c->PointerValue((RexxPointerObject)hOutput);
                pcon->isCreated = true;
            }
        }
    }
    return result;
}


/**
 *  Methods for the .ooConsole class.
 */
#define OOCONSOLE_CLASS    "ooConsole"


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


/** ooConsole::handle
 *
 *  Attribute, get only.
 *
 */
RexxMethod1(RexxObjectPtr, oocon_handle_atr, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return TheNilObj;
    }

    return context->NewPointer(pcon->handle);
}


/** ooConsole::init()
 *
 *  @param cSelf  The cSelf buffer for this console object.
 */
RexxMethod2(RexxObjectPtr, oocon_init, RexxObjectPtr, cSelf, OSELF, self)
{
    if ( ! context->IsBuffer(cSelf) )
    {
        wrongClassException(context->threadContext, 1, "Buffer");
        return TheOneObj;
    }

    context->SetObjectVariable("CSELF", cSelf);
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
        pCooConsole pcon = (pCooConsole)pCSelf;

#if OOCONSOLEDBG == 1
        printf("ooConsole::uninit() pCSelf=%p\n", pCSelf);
#endif
        if ( pcon->isCreated )
        {
            CloseHandle(pcon->handle);
            pcon->handle = NULL;
            pcon->isValid = false;
            pcon->isCreated = false;
        }
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

    if ( ! requiredOS(context, "getHistoryInfo", "Vista", Vista_OS) )
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


/** ooConsole::getNumberOfMouseButtons()
 *
 *
 */
RexxMethod1(RexxObjectPtr, oocon_getNumberOfMouseButtons, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return TheNilObj;
    }

    uint32_t count;
    if ( GetNumberOfConsoleMouseButtons((LPDWORD)&count) == 0 )
    {
        pcon->errRC = GetLastError();
        return TheNilObj;
    }

    return context->UnsignedInt32(count);
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


/** ooConsole::getSelectionInfo()
 *
 *
 */
RexxMethod1(RexxObjectPtr, oocon_getSelectionInfo , CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return TheNilObj;
    }

     CONSOLE_SELECTION_INFO csi;
    if ( GetConsoleSelectionInfo(&csi) == 0 )
    {
        pcon->errRC = GetLastError();
        return TheNilObj;
    }

    RexxDirectoryObject d = context->NewDirectory();
    context->DirectoryPut(d, indicator2string(context, csi.dwFlags) , "INDICATOR");
    context->DirectoryPut(d, rxNewPoint(context, csi.dwSelectionAnchor.X, csi.dwSelectionAnchor.Y) , "ANCHOR");
    context->DirectoryPut(d, rxNewRect(context, csi.srSelection.Left, csi.srSelection.Top,
                                       csi.srSelection.Right, csi.srSelection.Bottom), "SELECTION");

    return d;
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


/** ooConsole::setHistoryInfo()
 *
 *
 */
RexxMethod4(RexxObjectPtr, oocon_setHistoryInfo, OPTIONAL_RexxObjectPtr, bufOrDir, OPTIONAL_uint32_t, commands,
            OPTIONAL_logical_t, duplicates, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return TheFalseObj;
    }

    if ( ! requiredOS(context, "setHistoryInfo", "Vista", Vista_OS) )
    {
        return TheFalseObj;
    }

    CONSOLE_HISTORY_INFO  chi = {sizeof(CONSOLE_HISTORY_INFO)};
    if ( GetConsoleHistoryInfo(&chi) == 0 )
    {
        pcon->errRC = GetLastError();
        return TheFalseObj;
    }

    RexxMethodContext *c = context;
    if ( argumentExists(1) &&  c->IsDirectory(bufOrDir))
    {
        if ( argumentExists(2) || argumentExists(3) )
        {
            tooManyArgsException(c->threadContext, 1);
            return TheFalseObj;
        }

        RexxDirectoryObject dir = (RexxDirectoryObject)bufOrDir;

        RexxObjectPtr rxObj = c->DirectoryAt(dir, "BUFFERS");
        if ( rxObj != NULLOBJECT )
        {
            if ( ! c->ObjectToUnsignedInt32(rxObj, &chi.NumberOfHistoryBuffers) )
            {
                wrongObjInDirectoryException(context->threadContext, 1, "BUFFERS",
                                             "a whole number in the range 0 to 4294967295", rxObj);
                return TheFalseObj;
            }
        }

        rxObj = c->DirectoryAt(dir, "COMMANDS");
        if ( rxObj != NULLOBJECT )
        {
            if ( ! c->ObjectToUnsignedInt32(rxObj, &chi.HistoryBufferSize) )
            {
                wrongObjInDirectoryException(context->threadContext, 1, "COMMANDS",
                                             "a whole number in the range 0 to 4294967295", rxObj);
                return TheFalseObj;
            }
        }

        rxObj = c->DirectoryAt(dir, "FLAG");
        if ( rxObj != NULLOBJECT )
        {
            CSTRING val = c->ObjectToStringValue(rxObj);

            if ( StrCmpI(val, "Duplicates") == 0 ) chi.dwFlags = 0;
            else if ( StrCmpI(val, "No Duplicates") == 0 ) chi.dwFlags = HISTORY_NO_DUP_FLAG;
            else
            {
                wrongObjInDirectoryException(context->threadContext, 1, "FLAG",
                                             "the string \"Duplicates\" or \"No Duplicates\"", rxObj);
                return TheFalseObj;
            }
        }
    }
    else
    {
        if ( argumentExists(1) )
        {
            if ( ! c->ObjectToUnsignedInt32(bufOrDir, &chi.NumberOfHistoryBuffers) )
            {
                notUnsignedInt32Exception(context, 1, bufOrDir);
                return TheFalseObj;
            }
        }
        if ( argumentExists(2) )
        {
            chi.HistoryBufferSize = commands;
        }
        if ( argumentExists(3) )
        {
            chi.dwFlags = duplicates ? 0 : HISTORY_NO_DUP_FLAG;
        }
    }

    if ( SetConsoleHistoryInfo(&chi) == 0 )
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


/**
 * Internal use only.
 *
 * Transient method used to test various things as they come up.
 *
 */
RexxMethod2(int, oocon_test, uint32_t, t, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return 1;
    }

    printf("No tests at this time\n");

    return 0;
}



/**
 *  Methods for the .ScreenBuffer class.
 */
#define SCREENBUFFER_CLASS    "ScreenBuffer"


/** ScreenBuffer::close()
 *
 *  Closes the handle for this screen buffer.  Note, this should only be done
 *  with screen buffers that were created through createScreenBuffer().
 *
 *  @return  True on success, false on error.
 */
RexxMethod1(RexxObjectPtr, sb_close, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return TheFalseObj;
    }

    if ( CloseHandle(pcon->handle) == 0 )
    {
        pcon->errRC = GetLastError();
        return TheFalseObj;
    }

    pcon->handle = NULL;
    pcon->isValid = false;
    pcon->isCreated = false;

    return TheTrueObj;
}


/** ScreenBuffer::createScreenBuffer()
 *
 *  Creates a new screen buffer
 *
 *  @return  A new .Stdoutput object that represents the new screen buffer on
 *           success, .nil on error.
 *
 *  @note    Use close() to destroy the new screen buffer.
 */
RexxMethod1(RexxObjectPtr, sb_createScreenBuffer, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return TheNilObj;
    }

    HANDLE hNewOut = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    if ( hNewOut == INVALID_HANDLE_VALUE )
    {
        pcon->errRC = GetLastError();
        return TheNilObj;
    }

    RexxClassObject stdOutClass = rxGetContextClass(context, "STDOUTPUT");
    return context->SendMessage1(stdOutClass, "NEW", context->NewPointer(hNewOut));
}


/** ScreenBuffer::getCursorInof()
 *
 *  Retrieves information about the size and visibility of the cursor for this
 *  console buffer.
 *
 *  @return  A .directory object on success, .nil on error.
 */
RexxMethod1(RexxObjectPtr, sb_getCursorInfo, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return TheNilObj;
    }

    CONSOLE_CURSOR_INFO ci;
    if ( GetConsoleCursorInfo(pcon->handle, &ci) == 0 )
    {
        pcon->errRC = GetLastError();
        return TheNilObj;
    }

    RexxDirectoryObject d = context->NewDirectory();
    context->DirectoryPut(d, context->Logical(ci.bVisible), "VISIBLE");
    context->DirectoryPut(d, context->UnsignedInt32(ci.dwSize), "SIZE");

    return d;
}


/** ScreenBuffer::getFontInfo()
 *
 *
 *  @param  fullScreen [optional] If info is for a full screen window.  Default
 *                     is false.
 */
RexxMethod2(RexxObjectPtr, sb_getFontInfo, OPTIONAL_logical_t, fullScreen, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return TheNilObj;
    }

    if ( ! requiredOS(context, "getFontInfo", "Vista", Vista_OS) )
    {
        return TheNilObj;
    }

    CONSOLE_FONT_INFOEX  cfi = {sizeof(CONSOLE_FONT_INFOEX)};
    if ( GetCurrentConsoleFontEx(pcon->handle, fullScreen ? TRUE : FALSE, &cfi) == 0 )
    {
        pcon->errRC = GetLastError();
        return TheNilObj;
    }

    char *fName = unicode2ansi(cfi.FaceName, pcon);
    if ( fName == NULL )
    {
        return TheNilObj;
    }
    RexxStringObject faceName = context->String(fName);
    LocalFree(fName);

    RexxDirectoryObject d = context->NewDirectory();
    context->DirectoryPut(d, context->UnsignedInt32(cfi.nFont), "FONTINDEX");
    context->DirectoryPut(d, rxNewSize(context, cfi.dwFontSize.X, cfi.dwFontSize.Y), "FONTSIZE");
    context->DirectoryPut(d, family2string(context, cfi.FontFamily), "FONTFAMILY");
    context->DirectoryPut(d, weight2string(context, cfi.FontWeight), "FONTWEIGHT");
    context->DirectoryPut(d, faceName, "FONTNAME");

    return d;
}


/** ScreenBuffer::getFontSize()
 *
 *
 *  @param  fullScreen [optional] If info is for a full screen window.  Default
 *                     is false.
 */
RexxMethod2(RexxObjectPtr, sb_getFontSize, OPTIONAL_logical_t, fullScreen, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return TheNilObj;
    }

    CONSOLE_FONT_INFO  cfi;
    if ( GetCurrentConsoleFont(pcon->handle, fullScreen ? TRUE : FALSE, &cfi) == 0 )
    {
        pcon->errRC = GetLastError();
        return TheNilObj;
    }

    COORD s = GetConsoleFontSize(pcon->handle, cfi.nFont);
    if ( s.X == 0 && s.Y == 0 )
    {
        pcon->errRC = GetLastError();
        return TheNilObj;
    }

    return rxNewSize(context, s.X, s.Y);
}


/** ScreenBuffer::getLargestWindowSize()
 *
 *
 */
RexxMethod1(RexxObjectPtr, sb_getLargestWindowSize, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return TheNilObj;
    }

    COORD coord = GetLargestConsoleWindowSize(pcon->handle);
    if ( coord.X == 0 && coord.Y == 0 )
    {
        pcon->errRC = GetLastError();
        return TheNilObj;
    }

    return rxNewSize(context, coord.X, coord.Y);
}


/** ScreenBuffer::getMode()
 *
 *
 */
RexxMethod1(RexxObjectPtr, sb_getMode, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return TheNilObj;
    }

    uint32_t mode;
    if ( GetConsoleMode(pcon->handle, (LPDWORD)&mode) == 0 )
    {
        pcon->errRC = GetLastError();
        return TheNilObj;
    }

    char buf[512] = {'\0'};
    if ( (mode & ENABLE_PROCESSED_OUTPUT)   == ENABLE_PROCESSED_OUTPUT )   strcat(buf, "ProcessedOutput ");
    if ( (mode & ENABLE_WRAP_AT_EOL_OUTPUT) == ENABLE_WRAP_AT_EOL_OUTPUT ) strcat(buf, "WrapAtEolOuput ");

    if ( *buf != '\0' )
    {
        *(buf + strlen(buf) - 1) = '\0';
    }
    return context->String(buf);
}


/** ScreenBuffer::getScreenBufferInfo()
 *
 *
 */
RexxMethod1(RexxObjectPtr, sb_getScreenBufferInfo, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return TheNilObj;
    }

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if ( GetConsoleScreenBufferInfo(pcon->handle, &csbi) == 0 )
    {
        pcon->errRC = GetLastError();
        return TheNilObj;
    }

    RexxObjectPtr attributes = attribute2string(context, csbi.wAttributes);
    RexxObjectPtr winRect    = rxNewRect(context, csbi.srWindow.Left, csbi.srWindow.Top,
                                         csbi.srWindow.Right, csbi.srWindow.Bottom);

    RexxDirectoryObject d = context->NewDirectory();
    context->DirectoryPut(d, rxNewPoint(context, csbi.dwCursorPosition.X, csbi.dwCursorPosition.Y), "CURSORPOS");
    context->DirectoryPut(d, rxNewSize(context, csbi.dwMaximumWindowSize.X, csbi.dwMaximumWindowSize.Y), "MAXWINDOWSIZE");
    context->DirectoryPut(d, rxNewSize(context, csbi.dwSize.X, csbi.dwSize.Y), "BUFFERSIZE");
    context->DirectoryPut(d, winRect, "WINDOWRECT");
    context->DirectoryPut(d, attributes, "ATTRIBUTES");

    return d;
}


/** ScreenBuffer::getScreenBufferInfoEx()
 *
 *
 */
RexxMethod1(RexxObjectPtr, sb_getScreenBufferInfoEx, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return TheNilObj;
    }

    if ( ! requiredOS(context, "getScreenBufferInfoEx", "Vista", Vista_OS) )
    {
        return TheNilObj;
    }

    CONSOLE_SCREEN_BUFFER_INFOEX csbi = {sizeof(CONSOLE_SCREEN_BUFFER_INFOEX)};
    if ( GetConsoleScreenBufferInfoEx(pcon->handle, &csbi) == 0 )
    {
        pcon->errRC = GetLastError();
        return TheNilObj;
    }

    RexxArrayObject table = context->NewArray(16);
    for (size_t i = 0; i < 16; i++)
    {
        context->ArrayPut(table, context->UnsignedInt32(csbi.ColorTable[i]), i + 1);
    }

    RexxObjectPtr attributes = attribute2string(context, csbi.wAttributes);
    RexxObjectPtr winRect    = rxNewRect(context, csbi.srWindow.Left, csbi.srWindow.Top,
                                         csbi.srWindow.Right, csbi.srWindow.Bottom);

    RexxDirectoryObject d = context->NewDirectory();
    context->DirectoryPut(d, rxNewPoint(context, csbi.dwCursorPosition.X, csbi.dwCursorPosition.Y), "CURSORPOS");
    context->DirectoryPut(d, rxNewSize(context, csbi.dwMaximumWindowSize.X, csbi.dwMaximumWindowSize.Y), "MAXWINDOWSIZE");
    context->DirectoryPut(d, rxNewSize(context, csbi.dwSize.X, csbi.dwSize.Y), "BUFFERSIZE");
    context->DirectoryPut(d, winRect, "WINDOWRECT");
    context->DirectoryPut(d, attributes, "ATTRIBUTES");
    context->DirectoryPut(d, table, "COLORTABLE");
    context->DirectoryPut(d, context->Logical(csbi.bFullscreenSupported), "FULLSCREENSUPPORT");

    return d;
}


/** ScreenBuffer::getScreenBufferSize()
 *
 *
 */
RexxMethod1(RexxObjectPtr, sb_getScreenBufferSize, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return TheNilObj;
    }

    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
    if ( GetConsoleScreenBufferInfo(pcon->handle, &csbiInfo) == 0 )
    {
        pcon->errRC = GetLastError();
        return TheNilObj;
    }

    return rxNewSize(context, csbiInfo.dwSize.X, csbiInfo.dwSize.Y);
}


/** ScreenBuffer::getWindowRect()
 *
 *
 */
RexxMethod1(RexxObjectPtr, sb_getWindowRect, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return TheNilObj;
    }

    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
    if ( GetConsoleScreenBufferInfo(pcon->handle, &csbiInfo) == 0 )
    {
        pcon->errRC = GetLastError();
        return TheNilObj;
    }

    return rxNewRect(context, csbiInfo.srWindow.Left, csbiInfo.srWindow.Top, csbiInfo.srWindow.Right,
                     csbiInfo.srWindow.Bottom);
}


/** ScreenBuffer::getWindowSize()
 *
 *
 */
RexxMethod1(RexxObjectPtr, sb_getWindowSize, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return TheNilObj;
    }

    CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
    if ( GetConsoleScreenBufferInfo(pcon->handle, &csbiInfo) == 0 )
    {
        pcon->errRC = GetLastError();
        return TheNilObj;
    }

    return rxNewSize(context,
                     csbiInfo.srWindow.Right - csbiInfo.srWindow.Left + 1,
                     csbiInfo.srWindow.Bottom - csbiInfo.srWindow.Top + 1);
}


/** ScreenBuffer::scrollScreenBuffer()
 *
 *  Sets the input code page used by the console associated with the calling
 *  process.
 *
 *  @return  0 on success, .nil on error.
 */
RexxMethod5(RexxObjectPtr, sb_scrollScreenBuffer, RexxObjectPtr, _scrollRect, RexxObjectPtr, _destPos,
            RexxObjectPtr, charInfo, OPTIONAL_RexxObjectPtr, clipRect, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return TheNilObj;
    }

    SMALL_RECT scrollRect;
    if ( ! rxGetSmallRect(context, _scrollRect, 1, &scrollRect) )
    {
        return TheNilObj;
    }

    if ( ScrollConsoleScreenBuffer(pcon->handle, scrollRect) == 0 )
    {
        pcon->errRC = GetLastError();
        return TheFalseObj;
    }

    return TheTrueObj;
}


/** ScreenBuffer::setActiveScreenBuffer()
 *
 *  Sets the input code page used by the console associated with the calling
 *  process.
 *
 *  @return  0 on success, .nil on error.
 */
RexxMethod1(RexxObjectPtr, sb_setActiveScreenBuffer, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return TheNilObj;
    }

    if ( SetConsoleActiveScreenBuffer(pcon->handle) == 0 )
    {
        pcon->errRC = GetLastError();
        return TheFalseObj;
    }

    return TheTrueObj;
}


/** ScreenBuffer::setCursorInfo()
 *
 *  Sets the size and / or visibility of the cursor for this console buffer.
 *
 *  @return  True on success, false on error.
 */
RexxMethod3(RexxObjectPtr, sb_setCursorInfo, OPTIONAL_logical_t, visible, OPTIONAL_uint8_t, size, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return TheNilObj;
    }

    CONSOLE_CURSOR_INFO ci;
    if ( GetConsoleCursorInfo(pcon->handle, &ci) == 0 )
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

    if ( SetConsoleCursorInfo(pcon->handle, &ci) == 0 )
    {
        pcon->errRC = GetLastError();
        return TheFalseObj;
    }

    return TheTrueObj;
}


/** ScreenBuffer::setCursorPosition()
 *
 *  Sets the position of the cursor for this console buffer.
 *
 *  @return  True on success, false on error.
 */
RexxMethod2(RexxObjectPtr, sb_setCursorPosition, RexxObjectPtr, pos, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return TheFalseObj;
    }

    PORXPOINT p = rxGetPoint(context, pos, 1);
    if ( p == NULL )
    {
        return TheFalseObj;
    }
    COORD coord;
    point2coord(p, &coord);

    if ( SetConsoleCursorPosition (pcon->handle, coord) == 0 )
    {
        pcon->errRC = GetLastError();
        return TheFalseObj;
    }

    return TheTrueObj;
}


/** ScreenBuffer::setDisplayMode()
 *
 *
 *  @return  True or false.
 */
RexxMethod3(RexxObjectPtr, sb_setDisplayMode, CSTRING, _mode, OPTIONAL_RexxObjectPtr, _size, CSELF, pCSelf)
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

    if ( SetConsoleDisplayMode(pcon->handle, mode, &coord) == 0 )
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


/** ScreenBuffer::setFontInfo()
 *
 *
 *  @param  fullScreen [optional] If info is for a full screen window.  Default
 *                     is false.
 */
RexxMethod3(RexxObjectPtr, sb_setFontInfo, RexxObjectPtr, d, OPTIONAL_logical_t, fullScreen, CSELF, pCSelf)
{
    CONSOLE_FONT_INFOEX  cfi    = {sizeof(CONSOLE_FONT_INFOEX)};
    RexxObjectPtr        result = TheFalseObj;

    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        goto done_out;
    }

    if ( ! requiredOS(context, "setFontInfo", "Vista", Vista_OS) )
    {
        goto done_out;
    }

    RexxDirectoryObject info = rxGetDirectory(context, d, 1);
    if ( info == NULLOBJECT )
    {
        goto done_out;
    }

    if ( GetCurrentConsoleFontEx(pcon->handle, fullScreen ? TRUE : FALSE, &cfi) == 0 )
    {
        pcon->errRC = GetLastError();
        goto done_out;
    }

    RexxMethodContext *c = context;
    RexxObjectPtr rxObj = c->DirectoryAt(info, "FONTNAME");
    if ( rxObj != NULLOBJECT )
    {
        CSTRING fName = c->ObjectToStringValue(rxObj);
        if ( strlen(fName) >= 32 )
        {
            stringTooLongException(c->threadContext, 1, 31, strlen(fName));
            goto done_out;
        }

        HRESULT hr;
        if ( putUnicodeText((LPWORD)&cfi.FaceName, fName, &hr) == 0 )
        {
            pcon->errRC = hr;
            goto done_out;
        }
    }

    rxObj = c->DirectoryAt(info, "FONTINDEX");
    if ( rxObj != NULLOBJECT )
    {
        uint32_t index;
        if ( ! c->UnsignedInt32(rxObj, &index) )
        {
            wrongObjInDirectoryException(c->threadContext, 1, "FONTINDEX", "Non-negative whole number", rxObj);
            goto done_out;
        }
        cfi.nFont = index;
    }

    if ( ! directorySize2coord(context, info, "FONTSIZE", &cfi.dwFontSize) )
    {
        goto done_out;
    }

    rxObj = c->DirectoryAt(info, "FONTFAMILY");
    if ( rxObj != NULLOBJECT )
    {
        cfi.FontFamily = string2family(context, rxObj, 1);
    }

    rxObj = c->DirectoryAt(info, "FONTWEIGHT");
    if ( rxObj != NULLOBJECT )
    {
        cfi.FontWeight = string2weight(context, rxObj, 1);
    }

    if ( SetCurrentConsoleFontEx(pcon->handle, fullScreen ? TRUE : FALSE, &cfi) == 0 )
    {
        pcon->errRC = GetLastError();
        goto done_out;
    }

    result = TheTrueObj;

done_out:
    return result;
}


/** ScreenBuffer::setMode()
 *
 *
 *  @return True on success, false on error.
 */
RexxMethod2(RexxObjectPtr, sb_setMode, CSTRING, _mode, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return TheFalseObj;
    }

    uint32_t mode = 0;
    if ( StrStrI(_mode, "ProcessedOutput") !=NULL ) mode |= ENABLE_PROCESSED_OUTPUT;
    if ( StrStrI(_mode, "WrapAtEolOuput")  !=NULL ) mode |= ENABLE_WRAP_AT_EOL_OUTPUT;

    if ( SetConsoleMode(pcon->handle, mode) == 0 )
    {
        pcon->errRC = GetLastError();
        return TheFalseObj;
    }

    return TheTrueObj;
}


/** ScreenBuffer::setScreenBufferInfoEx()
 *
 *
 */
RexxMethod2(RexxObjectPtr, sb_setScreenBufferInfoEx, RexxObjectPtr, _info, CSELF, pCSelf)
{
    RexxObjectPtr result = TheFalseObj;
    CONSOLE_SCREEN_BUFFER_INFOEX csbi = {sizeof(CONSOLE_SCREEN_BUFFER_INFOEX)};

    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        goto done_out;
    }

    if ( ! requiredOS(context, "getScreenBufferInfoEx", "Vista", Vista_OS) )
    {
        goto done_out;
    }

    RexxDirectoryObject info = rxGetDirectory(context, _info, 1);
    if ( info == NULLOBJECT )
    {
        goto done_out;
    }

    if ( GetConsoleScreenBufferInfoEx(pcon->handle, &csbi) == 0 )
    {
        pcon->errRC = GetLastError();
        goto done_out;
    }

    RexxObjectPtr _attributes = context->DirectoryAt(info, "ATTRIBUTES");
    if ( _attributes != NULLOBJECT )
    {
        CSTRING attributes = context->ObjectToStringValue(_attributes);
        csbi.wAttributes = string2attributes(attributes);
    }
    RexxObjectPtr _fullScreen = context->DirectoryAt(info, "FULSCREENSUPPORT");
    if ( _fullScreen != NULLOBJECT )
    {
        logical_t fullScreen;
        if ( ! context->Logical(_fullScreen, &fullScreen) )
        {
            wrongObjInDirectoryException(context->threadContext, 1, "FULLSCREENSUPPORT", "LOGICAL", _fullScreen);
            goto done_out;
        }
        csbi.bFullscreenSupported = (BOOL)fullScreen;
    }
    RexxObjectPtr _colTable = context->DirectoryAt(info, "COLORTABLE");
    if ( _colTable != NULLOBJECT )
    {
        if ( ! context->IsArray(_colTable) )
        {
            wrongObjInDirectoryException(context->threadContext, 1, "COLORTABLE", "ARRAY", _colTable);
            goto done_out;
        }
        RexxArrayObject colTable = (RexxArrayObject)_colTable;

        size_t count = context->ArrayItems(colTable);
        if ( count != 16 )
        {
            arrayWrongSizeException(context->threadContext, count, 16, 1);
            goto done_out;
        }

        uint32_t colRef;
        for (size_t i = 1; i <= 16; i++)
        {
            RexxObjectPtr rxObj = context->ArrayAt(colTable, i);
            if ( ! context->UnsignedInt32(rxObj, &colRef) )
            {
                wrongObjInArrayException(context->threadContext, 1, i, "a COLORREF");
                goto done_out;
            }
            csbi.ColorTable[i - 1] = colRef;
        }
    }
    if ( ! directoryPoint2coord(context, info, "CURSORPOS", &csbi.dwCursorPosition) )
    {
        goto done_out;
    }
    if ( ! directorySize2coord(context, info, "MAXWINDOWSIZE", &csbi.dwMaximumWindowSize) )
    {
        goto done_out;
    }
    if ( ! directorySize2coord(context, info, "BUFFERSIZE", &csbi.dwSize) )
    {
        goto done_out;
    }
    if ( ! directoryRect2sRect(context, info, "WINDOWRECT", &csbi.srWindow) )
    {
        goto done_out;
    }

    csbi.srWindow.Right++;
    csbi.srWindow.Bottom++;

    if ( SetConsoleScreenBufferInfoEx(pcon->handle, &csbi) == 0 )
    {
        pcon->errRC = GetLastError();
        goto done_out;
    }
    result = TheTrueObj;

done_out:
    return result;
}


/** ScreenBuffer::setScreenBuffersize()
 *
 *
 */
RexxMethod2(RexxObjectPtr, sb_setScreenBufferSize, ARGLIST, args, CSELF, pCSelf)
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
    if ( SetConsoleScreenBufferSize(pcon->handle, coord) == 0 )
    {
        pcon->errRC = GetLastError();
        return TheNilObj;
    }

    return TheZeroObj;
}


/** ScreenBuffer::setTextAttribute()
 *
 *  Sets the attributes of characters written to the console window.
 *
 */
RexxMethod2(RexxObjectPtr, sb_setTextAttribute, CSTRING, attr, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return TheFalseObj;
    }

    uint32_t attributes = string2attributes(attr);
    if ( SetConsoleTextAttribute(pcon->handle, attributes) == 0 )
    {
        pcon->errRC = GetLastError();
        return TheFalseObj;
    }

    return TheTrueObj;
}


/** ScreenBuffer::setWindowRect()
 *
 *
 */
RexxMethod3(RexxObjectPtr, sb_setWindowRect, RexxObjectPtr, _rect, OPTIONAL_logical_t, absolute, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return TheNilObj;
    }

    SMALL_RECT sRect;
    if ( ! rxGetSmallRect(context, _rect, 1, &sRect)
    {
        return TheNilObj;
    }

    if ( SetConsoleWindowInfo(pcon->handle, absolute ? TRUE : FALSE, &sRect) == 0 )
    {
        pcon->errRC = GetLastError();
        return TheNilObj;
    }

    return TheZeroObj;
}


/** ScreenBuffer::setWindowSize()
 *
 *
 */
RexxMethod2(RexxObjectPtr, sb_setWindowSize, RexxObjectPtr, _size, CSELF, pCSelf)
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
    if ( GetConsoleScreenBufferInfo(pcon->handle, &csbiInfo) == 0 )
    {
        pcon->errRC = GetLastError();
        return TheNilObj;
    }

    SMALL_RECT sRect;

    sRect.Left   = csbiInfo.srWindow.Left;
    sRect.Top    = csbiInfo.srWindow.Top;
    sRect.Right  = csbiInfo.srWindow.Left + (uint16_t)size->cx - 1;
    sRect.Bottom = csbiInfo.srWindow.Top + (uint16_t)size->cy - 1;

    if ( SetConsoleWindowInfo(pcon->handle, TRUE, &sRect) == 0 )
    {
        pcon->errRC = GetLastError();
        return TheNilObj;
    }

    return TheZeroObj;
}


/** ScreenBuffer::write()
 *
 *  Writes a string to a screen buffer.
 *
 */
RexxMethod2(uint32_t, sb_write, CSTRING, msg, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return 0;
    }

    uint32_t countWrote = 0;
    uint32_t count = (uint32_t)strlen(msg);
    if ( WriteConsole(pcon->handle, msg, count, (LPDWORD)&countWrote, NULL) == 0 )
    {
        pcon->errRC = GetLastError();
    }

    return countWrote;
}



/**
 *  Methods for the .StdOutput class.
 */
#define STDOUTPUT_CLASS    "StdOutput"


/** StdOutput::init()
 *
 *  @param
 */
RexxMethod2(RexxObjectPtr, stdout_init, OPTIONAL_RexxObjectPtr, hOutput, SUPER, super)
{
    return genericStdInit(context, super, STDOUTPUT, hOutput);
}


/** StdOutput::getStdHandle()
 *
 *  Gets the standard handle of this screen buffer.
 *
 */
RexxMethod1(RexxObjectPtr, stdout_getStdHandle, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return TheNilObj;
    }

    return context->NewPointer(GetStdHandle(STD_OUTPUT_HANDLE));
}


/** StdOutput::setStdHandle()
 *
 *  Sets the standard handle for this screen buffer.
 *
 */
RexxMethod2(RexxObjectPtr, stdout_setStdHandle, POINTER, handle, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return TheFalseObj;
    }

    HANDLE oldHandle = pcon->handle;

    if ( SetStdHandle(STD_OUTPUT_HANDLE, handle) == 0 )
    {
        oldHandle = NULL;
    }
    /*
    else
    {
        pcon->handle = handle;
    }
    */

    return context->NewPointer(oldHandle);
}



/**
 *  Methods for the .StdError class.
 */
#define STDERROR_CLASS    "StdError"


/** StdError::init()
 *
 *  @param
 */
RexxMethod1(RexxObjectPtr, stderr_init, SUPER, super)
{
    return genericStdInit(context, super, STDERROR, NULLOBJECT);
}


/** StdError::getStdHandle()
 *
 *  Gets the standard handle of this screen buffer.
 *
 */
RexxMethod1(RexxObjectPtr, stderr_getStdHandle, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return TheNilObj;
    }

    return context->NewPointer(GetStdHandle(STD_ERROR_HANDLE));
}


/** StdError::setStdHandle()
 *
 *  Sets the standard handle for this screen buffer.
 *
 */
RexxMethod2(RexxObjectPtr, stderr_setStdHandle, POINTER, handle, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return TheFalseObj;
    }

    HANDLE oldHandle = pcon->handle;

    if ( SetStdHandle(STD_ERROR_HANDLE, handle) == 0 )
    {
        oldHandle = NULL;
    }
    else
    {
        pcon->handle = handle;
    }

    return context->NewPointer(oldHandle);
}



/**
 *  Methods for the .StdInput class.
 */
#define STDINPUT_CLASS    "StdInput"



/** StdInput::init()
 *
 *  @param
 */
RexxMethod1(RexxObjectPtr, stdin_init, SUPER, super)
{
    return genericStdInit(context, super, STDINPUT, NULLOBJECT);
}


/** StdInput::flushInputBuffer()
 *
 *
 */
RexxMethod1(RexxObjectPtr, stdin_flushInputBuffer, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return TheFalseObj;
    }

    if ( FlushConsoleInputBuffer(pcon->handle) == 0 )
    {
        pcon->errRC = GetLastError();
        return TheFalseObj;
    }

    return TheTrueObj;
}


/** StdInput::getStdHandle()
 *
 *  Gets the standard handle of this input buffer.
 *
 */
RexxMethod1(RexxObjectPtr, stdin_getStdHandle, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return TheNilObj;
    }

    return context->NewPointer(GetStdHandle(STD_INPUT_HANDLE));
}


/** StdInput::getMode()
 *
 *
 */
RexxMethod1(RexxObjectPtr, stdin_getMode, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return TheNilObj;
    }

    uint32_t mode;
    if ( GetConsoleMode(pcon->handle, (LPDWORD)&mode) == 0 )
    {
        pcon->errRC = GetLastError();
        return TheNilObj;
    }

    char buf[512] = {'\0'};
    if ( (mode & ENABLE_ECHO_INPUT)      == ENABLE_ECHO_INPUT )      strcat(buf, "EchoInput ");
    if ( (mode & ENABLE_INSERT_MODE)     == ENABLE_INSERT_MODE )     strcat(buf, "InsertMode ");
    if ( (mode & ENABLE_LINE_INPUT)      == ENABLE_LINE_INPUT )      strcat(buf, "LineInput ");
    if ( (mode & ENABLE_MOUSE_INPUT)     == ENABLE_MOUSE_INPUT )     strcat(buf, "MouseInput ");
    if ( (mode & ENABLE_PROCESSED_INPUT) == ENABLE_PROCESSED_INPUT ) strcat(buf, "ProcessedInput ");
    if ( (mode & ENABLE_QUICK_EDIT_MODE) == ENABLE_QUICK_EDIT_MODE ) strcat(buf, "EditMode ");
    if ( (mode & ENABLE_WINDOW_INPUT)    == ENABLE_WINDOW_INPUT )    strcat(buf, "WindowInput ");

    if ( *buf != '\0' )
    {
        *(buf + strlen(buf) - 1) = '\0';
    }
    return context->String(buf);
}


/** StdInput::getNumberOfInputEvents()
 *
 *
 */
RexxMethod1(RexxObjectPtr, stdin_getNumberOfInputEvents, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return TheNilObj;
    }

    uint32_t count;
    if ( GetNumberOfConsoleInputEvents(pcon->handle, (LPDWORD)&count) == 0 )
    {
        pcon->errRC = GetLastError();
        return TheNilObj;
    }

    return context->UnsignedInt32(count);
}

/** StdInput::peekInput()
 *
 *  Reads inpupt records from the input buffer, but leaves the buffer
 *  undisturbed.
 *
 */
RexxMethod3(RexxObjectPtr, stdin_peekInput, OPTIONAL_uint32_t, count, OPTIONAL_CSTRING, _filter, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return TheNilObj;
    }

    INPUT_RECORD inBuf[MAX_INPUTRECORDS];
    uint32_t countRead = 0;
    if ( count == 0 || count > MAX_INPUTRECORDS )
    {
        count = MAX_INPUTRECORDS;
    }

    uint32_t filter = KEY_EVENT;
    if ( argumentExists(2) )
    {
        filter = string2inputRecs(_filter);
    }

    if ( PeekConsoleInput(pcon->handle, inBuf, count, (LPDWORD)&countRead) == 0 )
    {
        pcon->errRC = GetLastError();
        return TheNilObj;
    }

    return createInputRecords(context, inBuf, countRead, filter, pcon);
}


/** StdInput::read()
 *
 *  Reads a string from the input buffer.
 *
 */
RexxMethod3(RexxObjectPtr, stdin_read, OPTIONAL_uint32_t, count, OPTIONAL_RexxObjectPtr, d, CSELF, pCSelf)
{
    RexxObjectPtr result = TheNilObj;

    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        goto done_out;
    }

    char buf[MAX_READBUFFER] = {'\0'};
    uint32_t countRead       = 0;
    if ( count == 0 || count >= MAX_READBUFFER )
    {
        count = MAX_READBUFFER;
    }

    if ( ReadConsole(pcon->handle, buf, count, (LPDWORD)&countRead, NULL) == 0 )
    {
        pcon->errRC = GetLastError();
        goto done_out;
    }

    result = context->NewString(buf, countRead);

    if ( argumentExists(2) )
    {
        if ( ! context->IsOfType(d, "DIRECTORY") )
        {
            wrongClassException(context->threadContext, 2, "Directory", d);
            result = TheNilObj;
            goto done_out;
        }

        context->DirectoryPut((RexxDirectoryObject)d, result, "DATA");
        context->DirectoryPut((RexxDirectoryObject)d, context->UnsignedInt32(countRead), "CHARSREAD");
    }

done_out:
    return result;
}


/** StdInput::readInput()
 *
 *  Reads, and removes, inpupt records from the input buffer.
 *
 */
RexxMethod3(RexxObjectPtr, stdin_readInput, OPTIONAL_uint32_t, count, OPTIONAL_CSTRING, _filter, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return TheNilObj;
    }

    INPUT_RECORD inBuf[MAX_INPUTRECORDS];
    uint32_t countRead = 0;
    if ( count == 0 || count > MAX_INPUTRECORDS )
    {
        count = MAX_INPUTRECORDS;
    }

    uint32_t filter = KEY_EVENT;
    if ( argumentExists(2) )
    {
        filter = string2inputRecs(_filter);
    }

    if ( ReadConsoleInput(pcon->handle, inBuf, count, (LPDWORD)&countRead) == 0 )
    {
        pcon->errRC = GetLastError();
        return TheNilObj;
    }

    return createInputRecords(context, inBuf, countRead, filter, pcon);
}


/** StdInput::setMode()
 *
 *
 *  @return True on success, false on error.
 */
RexxMethod2(RexxObjectPtr, stdin_setMode, CSTRING, _mode, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return TheFalseObj;
    }

    uint32_t mode = ENABLE_EXTENDED_FLAGS;
    if ( StrStrI(_mode, "EchoInput") != NULL    )  mode |= ENABLE_ECHO_INPUT;
    if ( StrStrI(_mode, "InsertMode") != NULL   )  mode |= ENABLE_INSERT_MODE;
    if ( StrStrI(_mode, "LineInput") != NULL    )  mode |= ENABLE_LINE_INPUT;
    if ( StrStrI(_mode, "MouseInput") != NULL   )  mode |= ENABLE_MOUSE_INPUT;
    if ( StrStrI(_mode, "ProcessedInput") != NULL) mode |= ENABLE_PROCESSED_INPUT;
    if ( StrStrI(_mode, "EditMode") != NULL     )  mode |= ENABLE_QUICK_EDIT_MODE;
    if ( StrStrI(_mode, "WindowInput") != NULL  )  mode |= ENABLE_WINDOW_INPUT;

    if ( SetConsoleMode(pcon->handle, mode) == 0 )
    {
        pcon->errRC = GetLastError();
        return TheFalseObj;
    }

    return TheTrueObj;
}


/** StdInput::setStdHandle()
 *
 *  Sets the standard handle for this input buffer.
 *
 */
RexxMethod2(RexxObjectPtr, stdin_setStdHandle, POINTER, handle, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return TheFalseObj;
    }

    HANDLE oldHandle = pcon->handle;

    if ( SetStdHandle(STD_INPUT_HANDLE, handle) == 0 )
    {
        oldHandle = NULL;
    }
    else
    {
        pcon->handle = handle;
    }

    return context->NewPointer(oldHandle);
}



// .ooConsole
REXX_METHOD_PROTOTYPE(oocon_version_cls);

REXX_METHOD_PROTOTYPE(oocon_errRC_atr);
REXX_METHOD_PROTOTYPE(oocon_handle_atr);
REXX_METHOD_PROTOTYPE(oocon_init);
REXX_METHOD_PROTOTYPE(oocon_uninit);
REXX_METHOD_PROTOTYPE(oocon_getCP);
REXX_METHOD_PROTOTYPE(oocon_getDisplayMode);
REXX_METHOD_PROTOTYPE(oocon_getHistoryInfo);
REXX_METHOD_PROTOTYPE(oocon_getNumberOfMouseButtons);
REXX_METHOD_PROTOTYPE(oocon_getOriginalTitle);
REXX_METHOD_PROTOTYPE(oocon_getOutputCP);
REXX_METHOD_PROTOTYPE(oocon_getTitle);
REXX_METHOD_PROTOTYPE(oocon_getProcessList);
REXX_METHOD_PROTOTYPE(oocon_getSelectionInfo);
REXX_METHOD_PROTOTYPE(oocon_getWindow);
REXX_METHOD_PROTOTYPE(oocon_setCP);
REXX_METHOD_PROTOTYPE(oocon_setHistoryInfo);
REXX_METHOD_PROTOTYPE(oocon_setOutputCP);
REXX_METHOD_PROTOTYPE(oocon_setTitle);

REXX_METHOD_PROTOTYPE(oocon_test);


// .ScreenBuffer
REXX_METHOD_PROTOTYPE(sb_close);
REXX_METHOD_PROTOTYPE(sb_createScreenBuffer);
REXX_METHOD_PROTOTYPE(sb_getCursorInfo);
REXX_METHOD_PROTOTYPE(sb_getFontInfo);
REXX_METHOD_PROTOTYPE(sb_getFontSize);
REXX_METHOD_PROTOTYPE(sb_getLargestWindowSize);
REXX_METHOD_PROTOTYPE(sb_getMode);
REXX_METHOD_PROTOTYPE(sb_getScreenBufferInfo);
REXX_METHOD_PROTOTYPE(sb_getScreenBufferInfoEx);
REXX_METHOD_PROTOTYPE(sb_getScreenBufferSize);
REXX_METHOD_PROTOTYPE(sb_getWindowRect);
REXX_METHOD_PROTOTYPE(sb_getWindowSize);
REXX_METHOD_PROTOTYPE(sb_setActiveScreenBuffer);
REXX_METHOD_PROTOTYPE(sb_setCursorInfo);
REXX_METHOD_PROTOTYPE(sb_setCursorPosition);
REXX_METHOD_PROTOTYPE(sb_setDisplayMode);
REXX_METHOD_PROTOTYPE(sb_setFontInfo);
REXX_METHOD_PROTOTYPE(sb_setMode);
REXX_METHOD_PROTOTYPE(sb_setScreenBufferInfoEx);
REXX_METHOD_PROTOTYPE(sb_setScreenBufferSize);
REXX_METHOD_PROTOTYPE(sb_setTextAttribute);
REXX_METHOD_PROTOTYPE(sb_setWindowRect);
REXX_METHOD_PROTOTYPE(sb_setWindowSize);
REXX_METHOD_PROTOTYPE(sb_write);

// .StdOutput
REXX_METHOD_PROTOTYPE(stdout_init);
REXX_METHOD_PROTOTYPE(stdout_getStdHandle);
REXX_METHOD_PROTOTYPE(stdout_setStdHandle);

// .StdError
REXX_METHOD_PROTOTYPE(stderr_init);
REXX_METHOD_PROTOTYPE(stderr_getStdHandle);
REXX_METHOD_PROTOTYPE(stderr_setStdHandle);


// .StdInput
REXX_METHOD_PROTOTYPE(stdin_init);
REXX_METHOD_PROTOTYPE(stdin_flushInputBuffer);
REXX_METHOD_PROTOTYPE(stdin_getStdHandle);
REXX_METHOD_PROTOTYPE(stdin_getMode);
REXX_METHOD_PROTOTYPE(stdin_getNumberOfInputEvents);
REXX_METHOD_PROTOTYPE(stdin_peekInput);
REXX_METHOD_PROTOTYPE(stdin_read);
REXX_METHOD_PROTOTYPE(stdin_readInput);
REXX_METHOD_PROTOTYPE(stdin_setMode);
REXX_METHOD_PROTOTYPE(stdin_setStdHandle);




RexxMethodEntry ooConsole_methods[] = {
    REXX_METHOD(oocon_version_cls,                oocon_version_cls),
    REXX_METHOD(oocon_errRC_atr,                  oocon_errRC_atr),
    REXX_METHOD(oocon_handle_atr,                 oocon_handle_atr),
    REXX_METHOD(oocon_init,                       oocon_init),
    REXX_METHOD(oocon_uninit,                     oocon_uninit),
    REXX_METHOD(oocon_getCP,                      oocon_getCP),
    REXX_METHOD(oocon_getDisplayMode,             oocon_getDisplayMode),
    REXX_METHOD(oocon_getHistoryInfo,             oocon_getHistoryInfo),
    REXX_METHOD(oocon_getNumberOfMouseButtons,    oocon_getNumberOfMouseButtons),
    REXX_METHOD(oocon_getOriginalTitle,           oocon_getOriginalTitle),
    REXX_METHOD(oocon_getOutputCP,                oocon_getOutputCP),
    REXX_METHOD(oocon_getProcessList,             oocon_getProcessList),
    REXX_METHOD(oocon_getTitle,                   oocon_getTitle),
    REXX_METHOD(oocon_getSelectionInfo,           oocon_getSelectionInfo),
    REXX_METHOD(oocon_getWindow,                  oocon_getWindow),
    REXX_METHOD(oocon_setCP,                      oocon_setCP),
    REXX_METHOD(oocon_setHistoryInfo,             oocon_setHistoryInfo),
    REXX_METHOD(oocon_setOutputCP,                oocon_setOutputCP),
    REXX_METHOD(oocon_setTitle,	                  oocon_setTitle),

    REXX_METHOD(oocon_test,                       oocon_test),

    REXX_METHOD(sb_close,                         sb_close),
    REXX_METHOD(sb_createScreenBuffer,            sb_createScreenBuffer),
    REXX_METHOD(sb_getCursorInfo,		          sb_getCursorInfo),
    REXX_METHOD(sb_getFontInfo,                   sb_getFontInfo),
    REXX_METHOD(sb_getFontSize,                   sb_getFontSize),
    REXX_METHOD(sb_getLargestWindowSize,          sb_getLargestWindowSize),
    REXX_METHOD(sb_getMode,                       sb_getMode),
    REXX_METHOD(sb_getScreenBufferInfo,           sb_getScreenBufferInfo),
    REXX_METHOD(sb_getScreenBufferInfoEx,         sb_getScreenBufferInfoEx),
    REXX_METHOD(sb_getScreenBufferSize,           sb_getScreenBufferSize),
    REXX_METHOD(sb_getWindowRect,                 sb_getWindowRect),
    REXX_METHOD(sb_getWindowSize,                 sb_getWindowSize),
    REXX_METHOD(sb_setActiveScreenBuffer,         sb_setActiveScreenBuffer),
    REXX_METHOD(sb_setCursorInfo,		          sb_setCursorInfo),
    REXX_METHOD(sb_setCursorPosition,             sb_setCursorPosition),
    REXX_METHOD(sb_setDisplayMode,                sb_setDisplayMode),
    REXX_METHOD(sb_setFontInfo,                   sb_setFontInfo),
    REXX_METHOD(sb_setMode,                       sb_setMode),
    REXX_METHOD(sb_setScreenBufferInfoEx,         sb_setScreenBufferInfoEx),
    REXX_METHOD(sb_setScreenBufferSize,           sb_setScreenBufferSize),
    REXX_METHOD(sb_setTextAttribute,              sb_setTextAttribute),
    REXX_METHOD(sb_setWindowRect,                 sb_setWindowRect),
    REXX_METHOD(sb_setWindowSize,                 sb_setWindowSize),
    REXX_METHOD(sb_write,                         sb_write),

    REXX_METHOD(stdout_init,                      stdout_init),
    REXX_METHOD(stdout_getStdHandle,              stdout_getStdHandle),
    REXX_METHOD(stdout_setStdHandle,              stdout_setStdHandle),

    REXX_METHOD(stderr_init,                      stderr_init),
    REXX_METHOD(stderr_getStdHandle,              stderr_getStdHandle),
    REXX_METHOD(stderr_setStdHandle,              stderr_setStdHandle),

    REXX_METHOD(stdin_init,                       stdin_init),
    REXX_METHOD(stdin_flushInputBuffer,           stdin_flushInputBuffer),
    REXX_METHOD(stdin_getStdHandle,               stdin_getStdHandle),
    REXX_METHOD(stdin_getMode,                    stdin_getMode),
    REXX_METHOD(stdin_getNumberOfInputEvents,     stdin_getNumberOfInputEvents),
    REXX_METHOD(stdin_peekInput,                  stdin_peekInput),
    REXX_METHOD(stdin_read,                       stdin_read),
    REXX_METHOD(stdin_readInput,                  stdin_readInput),
    REXX_METHOD(stdin_setMode,                    stdin_setMode),
    REXX_METHOD(stdin_setStdHandle,               stdin_setStdHandle),

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
