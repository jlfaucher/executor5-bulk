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

RexxInstance   *ThisInterpreter = NULL;
pCooConsole     ThisConsole = NULL;
RexxClassObject TheCharInfoClass = NULLOBJECT;
RexxClassObject TheInputRecordClass = NULLOBJECT;


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
        ThisInterpreter = c->instance;
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
 * Copies the data in a small rect to a .Rect Rexx object.
 *
 * @assumes The Rexx rect is known to be valid.
 */
inline void smallRect2rect(RexxMethodContext *c, PSMALL_RECT sr, RexxObjectPtr r)
{
    PORXRECT rect = rxGetRect(c, r, 0);
    if ( rect != NULL )
    {
        rect->left   = sr->Left;
        rect->top    = sr->Top;
        rect->right  = sr->Right;
        rect->bottom = sr->Bottom;
    }
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

static PCHAR_INFO rxGetCharInfo(RexxMethodContext *context, RexxObjectPtr ci, size_t argPos)
{
    if ( requiredClass(context->threadContext, ci, "CharInfo", argPos) )
    {
        return (PCHAR_INFO)context->ObjectToCSelf(ci);
    }
    return NULL;
}

static bool rxGetSmallRect(RexxMethodContext *context, RexxObjectPtr r, size_t argPos, PSMALL_RECT sr)
{
    PORXRECT rect = rxGetRect(context, r, argPos);
    if ( rect != NULL )
    {
        rect2smallRect(rect, sr);
        return true;
    }
    return false;
}

static bool rxGetPointerCoord(RexxMethodContext *context, RexxObjectPtr p, size_t argPos, PCOORD c)
{
    PORXPOINT pt = rxGetPoint(context, p, argPos);
    if ( pt != NULL )
    {
        point2coord(pt, c);
        return true;
    }
    return false;
}

static bool rxGetSizeCoord(RexxMethodContext *context, RexxObjectPtr s, size_t argPos, PCOORD c)
{
    PORXSIZE size = rxGetSize(context, s, argPos);
    if ( size != NULL )
    {
        size2coord(size, c);
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
 * Converts an array into a buffer of attributes.
 *
 * @param c
 * @param attrs
 * @param argPos
 * @param count
 *
 * @return uint16_t*
 */
uint16_t *array2attributes(RexxMethodContext *c, RexxArrayObject attrs, size_t argPos, uint32_t *count)
{
    uint32_t cnt = (uint32_t)c->ArrayItems(attrs);
    if ( cnt == 0 )
    {
        emptyArrayException(c->threadContext, (int)argPos);
        return NULL;
    }

    uint16_t *buf  = (uint16_t *)LocalAlloc(LPTR, cnt * sizeof(uint16_t));
    if ( buf == NULL )
    {
        outOfMemoryException(c->threadContext);
        return NULL;
    }

    for (size_t i = 1; i <= cnt; i++)
    {
        RexxObjectPtr rxObj = c->ArrayAt(attrs, i);
        if ( rxObj == NULLOBJECT )
        {
            sparseArrayException(c->threadContext, argPos, i);
            LocalFree(buf);
            return NULL;
        }
        buf[i - 1] = string2attributes(c->ObjectToStringValue(rxObj));
    }

    *count = cnt;
    return buf;
}

/**
 * Converts an array into a buffer of CHAR_INFO structs.
 *
 * @param c
 * @param charInfo
 * @param argPos
 *
 * @return CHAR_INFO*
 */
CHAR_INFO *array2charInfo(RexxMethodContext *c, RexxArrayObject charInfo, size_t argPos)
{
    uint32_t count = (uint32_t)c->ArrayItems(charInfo);
    if ( count == 0 )
    {
        emptyArrayException(c->threadContext, (int)argPos);
        return NULL;
    }

    CHAR_INFO *buf  = (CHAR_INFO *)LocalAlloc(LPTR, count * sizeof(CHAR_INFO));
    if ( buf == NULL )
    {
        outOfMemoryException(c->threadContext);
        return NULL;
    }

    for (size_t i = 1; i <= count; i++)
    {
        RexxObjectPtr rxObj = c->ArrayAt(charInfo, i);
        if ( rxObj == NULLOBJECT )
        {
            sparseArrayException(c->threadContext, argPos, i);
            LocalFree(buf);
            return NULL;
        }
        if ( ! c->IsOfType(rxObj, "CHARINFO") )
        {
            wrongObjInArrayException(c->threadContext, argPos, i, "a CharInfo", rxObj);
            LocalFree(buf);
            return NULL;
        }

        CHAR_INFO *chi = (CHAR_INFO *)c->ObjectToCSelf(rxObj);
        buf[i - 1].Attributes = chi->Attributes;
        buf[i - 1].Char       = chi->Char;
    }

    return buf;
}

/**
 * Converts an array into a buffer of INPUT_RECORD structs.
 *
 * @param c
 * @param inRecs
 * @param argPos
 *
 * @return INPUT_RECORDS*
 */
INPUT_RECORD *array2inRecord(RexxMethodContext *c, RexxArrayObject inRecs, size_t argPos, uint32_t *count)
{
    uint32_t cnt = (uint32_t)c->ArrayItems(inRecs);
    if ( cnt == 0 )
    {
        emptyArrayException(c->threadContext, (int)argPos);
        return NULL;
    }

    INPUT_RECORD *buf  = (INPUT_RECORD *)LocalAlloc(LPTR, cnt * sizeof(INPUT_RECORD));
    if ( buf == NULL )
    {
        outOfMemoryException(c->threadContext);
        return NULL;
    }

    for (size_t i = 1; i <= cnt; i++)
    {
        RexxObjectPtr rxObj = c->ArrayAt(inRecs, i);
        if ( rxObj == NULLOBJECT )
        {
            sparseArrayException(c->threadContext, argPos, i);
            LocalFree(buf);
            return NULL;
        }
        if ( ! c->IsOfType(rxObj, "INPUTRECORD") )
        {
            wrongObjInArrayException(c->threadContext, argPos, i, "an InputRecord", rxObj);
            LocalFree(buf);
            return NULL;
        }

        INPUT_RECORD *pir = (INPUT_RECORD *)c->ObjectToCSelf(rxObj);

        memcpy(&buf[i - 1], pir, sizeof(INPUT_RECORD));
    }

    *count = cnt;
    return buf;
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
 * Converts a string of strings separated by NULLs into an array of the strings.
 *
 * @param c
 * @param buffer
 * @param len
 *
 * @return RexxArrayObject
 */
static RexxArrayObject nullSeparatedStr2Array(RexxMethodContext *c, char *buffer, uint32_t len)
{
    RexxArrayObject a = c->NewArray(5);

    char *p = buffer;
   while ( p < buffer + len )
    {
        c->ArrayAppend(a, c->String(p));
        p = strchr(p, '\0');
        p++;
    }
    return a;
}

/**
 *  Converts a string to the control key state flags.
 *
 */
static uint16_t string2ctrlKeyState(CSTRING keyState)
{
    uint16_t state = 0;

    if ( StrStrI(keyState, "rAlt")       != NULL ) state |= RIGHT_ALT_PRESSED ;
    if ( StrStrI(keyState, "lAlt")       != NULL ) state |= LEFT_ALT_PRESSED  ;
    if ( StrStrI(keyState, "rCtrl")      != NULL ) state |= RIGHT_CTRL_PRESSED;
    if ( StrStrI(keyState, "lCtrl")      != NULL ) state |= LEFT_CTRL_PRESSED ;
    if ( StrStrI(keyState, "shift")      != NULL ) state |= SHIFT_PRESSED     ;
    if ( StrStrI(keyState, "numLock")    != NULL ) state |= NUMLOCK_ON        ;
    if ( StrStrI(keyState, "scrollLock") != NULL ) state |= SCROLLLOCK_ON     ;
    if ( StrStrI(keyState, "capsLock")   != NULL ) state |= CAPSLOCK_ON       ;
    if ( StrStrI(keyState, "enhanced")   != NULL ) state |= ENHANCED_KEY      ;

    return state;
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
 *  Converts the control signal type to a string.
 *
 */
static RexxStringObject ctrlType2string(RexxThreadContext *c, uint32_t ctrlType)
{
    char *buf;

    if ( ctrlType == CTRL_C_EVENT )             buf = "Ctrl-C ";
    else if ( ctrlType == CTRL_BREAK_EVENT )    buf = "Ctrl-Break";
    else if ( ctrlType == CTRL_SHUTDOWN_EVENT ) buf = "Ctrl-ShutDown";
    else if ( ctrlType == CTRL_LOGOFF_EVENT )   buf = "Ctrl-LogOff";
    else if ( ctrlType == CTRL_CLOSE_EVENT )    buf = "Ctrl-Close";

    return c->String(buf);
}

/**
 *  Converts a string to the control signal type. Note that only ctrl-c and
 *  ctrl-break can be sent.
 *
 */
static uint32_t string2ctrlType(RexxMethodContext *c, CSTRING str, size_t argPos)
{
    uint32_t ctrlType = (uint32_t)-1;

    if (      StrCmpI(str, "Ctrl-C")     == 0 ) ctrlType = CTRL_C_EVENT;
    else if ( StrCmpI(str, "Ctrl-Break") == 0 ) ctrlType = CTRL_BREAK_EVENT;
    else
    {
        wrongArgKeywordException(c, argPos, "Ctrl-C or Ctrl-Break", str);
    }

    return ctrlType;
}

/*
 *  Converts a string to the mouse button state flags.
 *
 */
static uint16_t string2buttonState(CSTRING buttonState)
{
    uint16_t state = 0;

    if ( StrStrI(buttonState, "leftMost1")     != NULL ) state |= FROM_LEFT_1ST_BUTTON_PRESSED;
    if ( StrStrI(buttonState, "rightMost")     != NULL ) state |= RIGHTMOST_BUTTON_PRESSED;
    if ( StrStrI(buttonState, "leftMost2")     != NULL ) state |= FROM_LEFT_2ND_BUTTON_PRESSED;
    if ( StrStrI(buttonState, "leftMost3")     != NULL ) state |= FROM_LEFT_3RD_BUTTON_PRESSED;
    if ( StrStrI(buttonState, "leftMost4")     != NULL ) state |= FROM_LEFT_4TH_BUTTON_PRESSED;

    return state;
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
 *  Converts a string to the mouse event flag.
 *
 */
static uint16_t string2eventFlag(CSTRING event)
{
    uint16_t flag = 0;

    if (      StrCmpI(event, "pressOrRelease") == 0 ) flag = 0;
    else if ( StrCmpI(event, "mouseMoved"    ) == 0 ) flag = MOUSE_MOVED;
    else if ( StrCmpI(event, "doubleClick"   ) == 0 ) flag = DOUBLE_CLICK;
    else if ( StrCmpI(event, "mouseVWheeled" ) == 0 ) flag = MOUSE_WHEELED;
    else if ( StrCmpI(event, "mouseHWheeled" ) == 0 ) flag = MOUSE_HWHEELED;

    return flag;
}

/**
 *  Converts the mouse event flag to a string.
 *
 */
static RexxStringObject eventFlag2string(RexxMethodContext *c, uint32_t flags)
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
    RexxStringObject eventFlags  = eventFlag2string(c, rec->dwEventFlags);

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
 * This is
 *
 * @author Administrator (05/25/2014)
 *
 * @param ctrlType
 *
 * @return BOOL
 */
BOOL WINAPI RexxHandler(DWORD ctrlType)
{
    BOOL response = FALSE;

    RexxThreadContext *c;
    if ( ThisInterpreter->AttachThread(&c) )
    {
        if ( ThisConsole != NULL )
        {
            RexxObjectPtr reply = c->SendMessage1(ThisConsole->rexxSelf, ThisConsole->handlerMethod,
                                                  ctrlType2string(c, ctrlType));

            int32_t logical = getLogical(c, reply);
            if ( logical == -1 )
            {
                notBooleanReplyException(c, ThisConsole->handlerMethod, reply == NULLOBJECT ? c->NullString() : reply);
                checkForCondition(c, false);
                response = FALSE;
            }
            else
            {
                response = (BOOL)logical;
            }
        }
        else
        {
            printf("ThisConsole == NULL\n");
        }
        c->DetachThread();
    }
    else
    {
        printf("Failed to attach thread\n");
    }

    return response;
}

/**
 *  Methods for the .ooConsole class.
 */
#define OOCONSOLE_CLASS    "ooConsole"


/** ooConsole::allocConsole  [class method]
 *
 */
RexxMethod1(RexxObjectPtr, oocon_allocConsole_cls, OPTIONAL_CSTRING, type)
{
    if ( AllocConsole() == 0 )
    {
        return context->UnsignedInt32(GetLastError());
    }

    return TheZeroObj;
}

/** ooConsole::attachConsole  [class method]
 *
 */
RexxMethod1(RexxObjectPtr, oocon_attachConsole_cls, OPTIONAL_CSTRING, type)
{
    if ( AttachConsole(ATTACH_PARENT_PROCESS) == 0 )
    {
        return context->UnsignedInt32(GetLastError());
    }

    return TheZeroObj;
}

/** ooConsole::freeConsole  [class method]
 *
 */
RexxMethod1(RexxObjectPtr, oocon_freeConsole_cls, OPTIONAL_CSTRING, type)
{
    if ( FreeConsole() == 0 )
    {
        return context->UnsignedInt32(GetLastError());
    }

    return TheZeroObj;
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

    pCooConsole pcon = (pCooConsole)context->BufferData((RexxBufferObject)cSelf);
    pcon->rexxSelf   = self;

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


/** ooConsole::addAlias()
 *
 *  Adds a console alias.
 *
 *  @return
 */
RexxMethod4(RexxObjectPtr, oocon_addAlias, CSTRING, alias, OPTIONAL_CSTRING, target, OPTIONAL_CSTRING, exe, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return TheFalseObj;
    }

    if ( argumentOmitted(3) )
    {
        exe = "rexx.exe";
    }

    if ( AddConsoleAlias((LPSTR)alias, (LPSTR)target, (LPSTR)exe) == 0 )
    {
        pcon->errRC = GetLastError();
        return TheFalseObj;
    }
    return TheTrueObj;
}


/** ooConsole::generateCtrlEvent()
 *
 *  Sets the input code page used by the console associated with the calling
 *  process.
 *
 *  @return  True on success, false on error.
 */
RexxMethod3(RexxObjectPtr, oocon_generateCtrlEvent, CSTRING, _event, OPTIONAL_uint32_t, groupID, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return TheFalseObj;
    }

    uint32_t event = string2ctrlType(context, _event, 1);
    if ( (int32_t)event == -1 )
    {
        return TheFalseObj;
    }

    if ( GenerateConsoleCtrlEvent(event, groupID) == 0 )
    {
        pcon->errRC = GetLastError();
        return TheFalseObj;
    }

    return TheTrueObj;
}


/** ooConsole::getAlias()
 *
 *  Gets the console alias.
 *
 *  @return
 */
RexxMethod3(RexxObjectPtr, oocon_getAlias, CSTRING, alias, CSTRING, exe, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return TheNilObj;
    }

    // We arbitrarily pick MAX_CONSOLETITLE (8000) bytes as the maximum length
    // for an alias ...
    char buf[MAX_CONSOLETITLE];

    if ( GetConsoleAlias((LPSTR)alias, buf, sizeof(buf), (LPSTR)exe) == 0 )
    {
        pcon->errRC = GetLastError();
        return TheNilObj;
    }
    return context->String(buf);
}


/** ooConsole::getAliases()
 *
 *  Gets all  the console aliases for the specified exe.
 *
 *  @return
 */
RexxMethod2(RexxObjectPtr, oocon_getAliases, CSTRING, exe, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return TheNilObj;
    }

    // See how much room we need
    uint32_t len = GetConsoleAliasesLength((LPSTR)exe);
    if ( len == 0 )
    {
        // There are no aliases.
        return context->NullString();
    }

    char buf[MAX_CONSOLETITLE];
    char *buffer = NULL;

    if ( len < MAX_CONSOLETITLE )
    {
        buffer = buf;
    }
    else
    {
        buffer = (char *)LocalAlloc(LPTR, len);
        if ( buffer == NULL )
        {
            outOfMemoryException(context->threadContext);
            pcon->errRC = ERROR_NOT_ENOUGH_MEMORY;
            return TheNilObj;
        }
    }

    if ( GetConsoleAliases(buffer, len, (LPSTR)exe) == 0 )
    {
        pcon->errRC = GetLastError();
        return TheNilObj;
    }

    RexxArrayObject result = nullSeparatedStr2Array(context, buffer, len);

    if ( len >= MAX_CONSOLETITLE )
    {
        LocalFree(buffer);
    }
    return result;
}


/** ooConsole::getAliasesLength()
 *
 *  Gets the length of the buffer needed to get the console aliases for the
 *  specified exe.
 *
 *  @return
 */
RexxMethod2(RexxObjectPtr, oocon_getAliasesLength, CSTRING, exe, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return TheNilObj;
    }
    return context->UnsignedInt32(GetConsoleAliasesLength((LPSTR)exe));
}


/** ooConsole::getAliasExes()
 *
 *  Gets all  the console aliases for the specified exe.
 *
 *  @return
 */
RexxMethod1(RexxObjectPtr, oocon_getAliasExes, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return TheNilObj;
    }

    // See how much room we need
    uint32_t len = GetConsoleAliasExesLength();
    if ( len == 0 )
    {
        // There are no aliases.
        return context->NullString();
    }

    char buf[MAX_CONSOLETITLE];
    char *buffer = NULL;

    if ( len < MAX_CONSOLETITLE )
    {
        buffer = buf;
    }
    else
    {
        buffer = (char *)LocalAlloc(LPTR, len);
        if ( buffer == NULL )
        {
            outOfMemoryException(context->threadContext);
            pcon->errRC = ERROR_NOT_ENOUGH_MEMORY;
            return TheNilObj;
        }
    }

    if ( GetConsoleAliasExes(buffer, len) == 0 )
    {
        pcon->errRC = GetLastError();
        return TheNilObj;
    }

    RexxArrayObject result = nullSeparatedStr2Array(context, buffer, len);

    if ( len >= MAX_CONSOLETITLE )
    {
        LocalFree(buffer);
    }
    return result;
}


/** ooConsole::getAliasExesLength()
 *
 *  Gets the length needed for the console aliase exes.
 *
 *  @return
 */
RexxMethod1(RexxObjectPtr, oocon_getAliasExesLength, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return TheNilObj;
    }
    return context->UnsignedInt32(GetConsoleAliasExesLength());
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
 *  @return  True on success, false on error.
 */
RexxMethod2(RexxObjectPtr, oocon_setCP, uint32_t, cp, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return TheFalseObj;
    }

    if ( SetConsoleCP(cp) == 0 )
    {
        pcon->errRC = GetLastError();
        return TheFalseObj;
    }

    return TheTrueObj;
}


/** ooConsole::setCtrlHandler()
 *
 *  Sets the .
 *
 *  @return  True on success, false on error.
 */
RexxMethod3(RexxObjectPtr, oocon_setCtrlHandler, OPTIONAL_logical_t, add, OPTIONAL_CSTRING, method, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return TheFalseObj;
    }

    if ( argumentOmitted(1) )
    {
        add = TRUE;
    }

    PHANDLER_ROUTINE callBack = NULL;
    if ( argumentExists(2) )
    {
        callBack = RexxHandler;
    }

    if ( SetConsoleCtrlHandler(callBack, add ? TRUE : FALSE) == 0 )
    {
        pcon->errRC = GetLastError();
        return TheFalseObj;
    }

    if ( callBack != NULL )
    {
        if ( add )
        {
            pcon->handlerMethod = method;
            ThisConsole         = pcon;
        }
        else
        {
            pcon->handlerMethod = NULL;
            ThisConsole         = NULL;
        }
    }

    return TheTrueObj;
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


/** ScreenBuffer::fillOutputAttribute()
 *
 *  @retrun  The count actually written, or 0 on error.  Writing 0 attributes is
 *           specified as an error.
 */
RexxMethod4(uint32_t, sb_fillOutputAttribute, CSTRING, _attr, uint32_t, count, RexxObjectPtr, _dest, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return 0;
    }

    uint32_t actual = 0;
    uint32_t attr   = string2attributes(_attr);

    COORD dest;
    if ( ! rxGetPointerCoord(context, _dest, 3, &dest) )
    {
        pcon->errRC = ERROR_INVALID_PARAMETER;
        return 0;
    }

    if ( FillConsoleOutputAttribute(pcon->handle, attr, count, dest, (LPDWORD)&actual) == 0 )
    {
        pcon->errRC = GetLastError();
        return 0;
    }

    return actual;
}


/** ScreenBuffer::fillOutputCharacter()
 *
 *  @retrun  The count actually written, or 0 on error.  Writing 0 attributes is
 *           specified as an error.
 */
RexxMethod4(uint32_t, sb_fillOutputCharacter, CSTRING, _char, uint32_t, count, RexxObjectPtr, _dest, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return 0;
    }

    uint32_t actual = 0;
    char     c      = *_char;

    COORD dest;
    if ( ! rxGetPointerCoord(context, _dest, 3, &dest) )
    {
        pcon->errRC = ERROR_INVALID_PARAMETER;
        return 0;
    }

    if ( FillConsoleOutputCharacter(pcon->handle, c, count, dest, (LPDWORD)&actual) == 0 )
    {
        pcon->errRC = GetLastError();
        return 0;
    }

    return actual;
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


/** ScreenBuffer::readOutput()
 *
 *  Reads character and color attribute data from a rectangular block of
 *  character cells in a console screen buffer, and the function writes the data
 *  to a rectangular block at a specified location in the destination buffer.
 *
 */
RexxMethod4(RexxObjectPtr, sb_readOutput, RexxObjectPtr, _bufSize, RexxObjectPtr, _bufCoord,
            RexxObjectPtr, _readRect, CSELF, pCSelf)
{
    RexxObjectPtr result = TheNilObj;

    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        goto done_out;
    }

    COORD bufSize;
    if ( ! rxGetSizeCoord(context, _bufSize, 1, &bufSize) )
    {
        pcon->errRC = ERROR_INVALID_PARAMETER;
        goto done_out;
    }

    COORD bufCoord;
    if ( ! rxGetSizeCoord(context, _bufCoord, 2, &bufCoord) )
    {
        pcon->errRC = ERROR_INVALID_PARAMETER;
        goto done_out;
    }

    SMALL_RECT readRect;
    if ( ! rxGetSmallRect(context, _readRect, 3, &readRect) )
    {
        pcon->errRC = ERROR_INVALID_PARAMETER;
        goto done_out;
    }

    CHAR_INFO buf[MAX_CHARINFOBUFFER] = {0};
    if ( ReadConsoleOutput(pcon->handle, buf, bufSize, bufCoord, &readRect) == 0 )
    {
        pcon->errRC = GetLastError();
    }
    else
    {
        uint32_t lastCell = (readRect.Right - readRect.Left + 1) * (readRect.Bottom - readRect.Top + 1);
        RexxArrayObject a = context->NewArray(lastCell);

        for ( size_t i = 0; i < lastCell; i++ )
        {
            RexxBufferObject obj = context->NewBuffer(sizeof(CHAR_INFO));

            PCHAR_INFO pci = (PCHAR_INFO)context->BufferData(obj);
            pci->Char.AsciiChar = buf[i].Char.AsciiChar;
            pci->Attributes     = buf[i].Attributes;

            RexxObjectPtr ci = context->SendMessage1(TheCharInfoClass, "NEW", obj);
            context->ArrayAppend(a, ci);
        }

        smallRect2rect(context, &readRect, _readRect);

        result = a;
    }

done_out:
    return result;
}


/** ScreenBuffer::readOutputAttribute()
 *
 *  Reads a number of character attributes from consecutive cells of a console
 *  screen buffer into a buffer, beginning at a specified location.
 *
 */
RexxMethod3(RexxObjectPtr, sb_readOutputAttribute, uint32_t, count, RexxObjectPtr, _dest, CSELF, pCSelf)
{
    RexxObjectPtr result = TheNilObj;

    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        goto done_out;
    }

    COORD dest;
    if ( ! rxGetPointerCoord(context, _dest, 2, &dest) )
    {
        pcon->errRC = ERROR_INVALID_PARAMETER;
        goto done_out;
    }

    uint16_t buf[MAX_READBUFFER];
    uint32_t countRead = 0;
    if ( ReadConsoleOutputAttribute(pcon->handle, buf, count, dest, (LPDWORD)&countRead) == 0 )
    {
        pcon->errRC = GetLastError();
    }
    else
    {
        RexxArrayObject a = context->NewArray(count);
        for ( size_t i = 0; i < countRead; i++ )
        {
            context->ArrayAppend(a, attribute2string(context, buf[i]));
        }
        result = a;
    }

done_out:
    return result;
}


/** ScreenBuffer::readOutputCharacter()
 *
 *  Reads a number of charactersfrom consecutive cells of a console screen
 *  buffer into a buffer, beginning at a specified location.
 *
 */
RexxMethod3(RexxObjectPtr, sb_readOutputCharacter, uint32_t, count, RexxObjectPtr, _location, CSELF, pCSelf)
{
    RexxObjectPtr result = TheNilObj;

    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        goto done_out;
    }

    COORD location;
    if ( ! rxGetPointerCoord(context, _location, 2, &location) )
    {
        pcon->errRC = ERROR_INVALID_PARAMETER;
        goto done_out;
    }

    char buf[MAX_READBUFFER] = {'\0'};
    uint32_t countRead = 0;
    if ( ReadConsoleOutputAttribute(pcon->handle, (LPWORD)buf, count, location, (LPDWORD)&countRead) == 0 )
    {
        pcon->errRC = GetLastError();
    }
    else
    {
        result = context->NewString(buf, countRead);
    }

done_out:
    return result;
}


/** ScreenBuffer::scrollScreenBuffer()
 *
 *  Sets the input code page used by the console associated with the calling
 *  process.
 *
 *  @return  0 on success, .nil on error.
 */
RexxMethod5(RexxObjectPtr, sb_scrollScreenBuffer, RexxObjectPtr, _scrollRect, RexxObjectPtr, _destPos,
            RexxObjectPtr, _charInfo, OPTIONAL_RexxObjectPtr, _clipRect, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return TheFalseObj;
    }

    SMALL_RECT scrollRect;
    if ( ! rxGetSmallRect(context, _scrollRect, 1, &scrollRect) )
    {
        return TheFalseObj;
    }

    COORD destPos;
    if ( ! rxGetPointerCoord(context, _destPos, 2, &destPos) )
    {
        return TheFalseObj;
    }

    PCHAR_INFO chari = rxGetCharInfo(context, _charInfo, 3);
    if ( chari == NULL )
    {
        return TheFalseObj;
    }

    SMALL_RECT clipRect;
    PSMALL_RECT pClipRect = NULL;
    if ( argumentExists(4) )
    {
        if ( ! rxGetSmallRect(context, _clipRect, 4, &clipRect) )
        {
            return TheFalseObj;
        }
        pClipRect = &clipRect;
    }

    if ( ScrollConsoleScreenBuffer(pcon->handle, &scrollRect, pClipRect, destPos, chari) == 0 )
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
    if ( ! rxGetSmallRect(context, _rect, 1, &sRect) )
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

/** ScreenBuffer::writeOutput()
 *
 *  Writes character and color attribute data to a specified rectangular block
 *  of character cells in a console screen buffer. The data to be written is
 *  taken from a correspondingly sized rectangular block at a specified location
 *  in the source buffer.
 *
 */
RexxMethod5(RexxObjectPtr, sb_writeOutput, RexxArrayObject, charInfo, RexxObjectPtr, _bufSize,
            RexxObjectPtr, _bufCoord, RexxObjectPtr, _writeRect, CSELF, pCSelf)
{
    RexxObjectPtr result = TheFalseObj;

    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        goto done_out;
    }

    COORD bufSize;
    if ( ! rxGetSizeCoord(context, _bufSize, 2, &bufSize) )
    {
        pcon->errRC = ERROR_INVALID_PARAMETER;
        goto done_out;
    }

    COORD bufCoord;
    if ( ! rxGetSizeCoord(context, _bufCoord, 3, &bufCoord) )
    {
        pcon->errRC = ERROR_INVALID_PARAMETER;
        goto done_out;
    }

    SMALL_RECT writeRect;
    if ( ! rxGetSmallRect(context, _writeRect, 4, &writeRect) )
    {
        pcon->errRC = ERROR_INVALID_PARAMETER;
        goto done_out;
    }

    CHAR_INFO *buf = array2charInfo(context, charInfo, 1);
    if ( buf == NULL )
    {
        goto done_out;
    }

    if ( WriteConsoleOutput(pcon->handle, buf, bufSize, bufCoord, &writeRect) == 0 )
    {
        pcon->errRC = GetLastError();
    }
    else
    {
        result = TheTrueObj;
    }

    LocalFree(buf);
    smallRect2rect(context, &writeRect, _writeRect);

done_out:
    return result;
}


/** ScreenBuffer::writeOutputAttribute()
 *
 *  Copies a number of character attributes to consecutive cells of a console
 *  screen buffer, beginning at a specified location.
 *
 */
RexxMethod3(uint32_t, sb_writeOutputAttribute, RexxArrayObject, attrs, RexxObjectPtr, _dest, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return 0;
    }

    COORD dest;
    if ( ! rxGetPointerCoord(context, _dest, 2, &dest) )
    {
        pcon->errRC = ERROR_INVALID_PARAMETER;
        return 0;
    }

    uint32_t count;
    uint16_t *buf = array2attributes(context, attrs, 1, &count);
    if ( buf == NULL )
    {
        return 0;
    }

    uint32_t countWrote = 0;
    if ( WriteConsoleOutputAttribute(pcon->handle, buf, count, dest, (LPDWORD)&countWrote) == 0 )
    {
        pcon->errRC = GetLastError();
        countWrote  = 0;
    }

    LocalFree(buf);
    return countWrote;
}


/** ScreenBuffer::writeOutputCharacter()
 *
 *  Copies a number of character to consecutive cells of a console screen
 *  buffer, beginning at a specified location.
 *
 */
RexxMethod3(uint32_t, sb_writeOutputCharacter, CSTRING, chars, RexxObjectPtr, _dest, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return 0;
    }

    COORD dest;
    if ( ! rxGetPointerCoord(context, _dest, 2, &dest) )
    {
        pcon->errRC = ERROR_INVALID_PARAMETER;
        return 0;
    }

    uint32_t count = (uint32_t)strlen(chars);
    uint32_t countWrote = 0;
    if ( WriteConsoleOutputCharacter(pcon->handle, chars, count, dest, (LPDWORD)&countWrote) == 0 )
    {
        pcon->errRC = GetLastError();
        countWrote  = 0;
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


/** StdInput::writeInput()
 *
 *  Writes data, in the form of input records, directly to the console input
 *  buffer.
 *
 */
RexxMethod2(RexxObjectPtr, stdin_writeInput, RexxArrayObject, recs, CSELF, pCSelf)
{
    pCooConsole pcon = requiredConsole(context, pCSelf);
    if ( pcon == NULL )
    {
        return TheZeroObj;
    }

    RexxMethodContext *c = context;

    uint32_t count;
    uint32_t countWritten = 0;

    INPUT_RECORD *inBuf = array2inRecord(context, recs, 1, &count);
    if ( inBuf == NULL )
    {
        return TheZeroObj;
    }

    if ( WriteConsoleInput(pcon->handle, inBuf, count, (LPDWORD)&countWritten) == 0 )
    {
        pcon->errRC = GetLastError();
        return TheZeroObj;
    }

    return context->UnsignedInt32(countWritten);
}



/**
 * Methods for the .CharInfo class.
 */
#define CHARINFO_CLASS  "CharInfo"


RexxMethod1(RexxObjectPtr, char_init_cls, OSELF, self)
{
    if ( isOfClassType(context, self, CHARINFO_CLASS) )
    {
        TheCharInfoClass = (RexxClassObject)self;
        context->RequestGlobalReference(TheCharInfoClass);
    }
    return NULLOBJECT;
}

RexxMethod2(RexxObjectPtr, char_from_cls, OPTIONAL_CSTRING,  chari, OPTIONAL_CSTRING, attr)
{
    RexxBufferObject obj = context->NewBuffer(sizeof(CHAR_INFO));

    PCHAR_INFO pci = (PCHAR_INFO)context->BufferData(obj);
    pci->Char.AsciiChar = argumentExists(1) ? *chari : ' ';
    pci->Attributes     = argumentExists(2) ? string2attributes(attr) : FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;

    RexxObjectPtr charInfoObj = context->SendMessage1(TheCharInfoClass, "NEW", obj);

    return charInfoObj;
}
RexxMethod1(RexxObjectPtr, char_init, RexxObjectPtr,  cSelfBuf)
{
    if ( context->IsBuffer(cSelfBuf) )
    {
        context->SetObjectVariable("CSELF", cSelfBuf);
    }
    else
    {
        wrongClassException(context->threadContext, 1, "Buffer");
    }

    return NULLOBJECT;
}
RexxMethod1(RexxObjectPtr, char_char, CSELF, p)
{
    char buf[2] = {'\0'};
    buf[0] = ((PCHAR_INFO)p)->Char.AsciiChar;
    return context->String(buf);
}
RexxMethod1(RexxObjectPtr, char_attribute, CSELF, p)
{
     return attribute2string(context, ((PCHAR_INFO)p)->Attributes);
}
RexxMethod2(RexxObjectPtr, char_setChar, CSTRING, chari, CSELF, p)
{
    ((PCHAR_INFO)p)->Char.AsciiChar = *chari;
    return NULLOBJECT;
}
RexxMethod2(RexxObjectPtr, char_setAttribute, CSTRING, attr, CSELF, p)
{
    ((PCHAR_INFO)p)->Attributes = string2attributes(attr);
    return NULLOBJECT;
}


/**
 * Methods for the .InputRecord class.
 */
#define INPUTRECORD_CLASS  "InputRecord"


RexxMethod1(RexxObjectPtr, inrec_init_cls, OSELF, self)
{
    if ( isOfClassType(context, self, INPUTRECORD_CLASS) )
    {
        TheInputRecordClass = (RexxClassObject)self;
        context->RequestGlobalReference(TheInputRecordClass);
    }
    return NULLOBJECT;
}

RexxMethod1(RexxObjectPtr, inrec_init, RexxObjectPtr,  cSelfBuf)
{
    if ( context->IsBuffer(cSelfBuf) )
    {
        context->SetObjectVariable("CSELF", cSelfBuf);
    }
    else
    {
        wrongClassException(context->threadContext, 1, "Buffer");
    }

    return NULLOBJECT;
}

RexxMethod1(RexxObjectPtr, inrec_controlKeys, CSELF, p)
{
    PINPUT_RECORD pir = (PINPUT_RECORD)p;

    if ( pir->EventType == KEY_EVENT )
    {
        return ctrlKeyState2string(context, pir->Event.KeyEvent.dwControlKeyState);
    }
    else if ( pir->EventType == MOUSE_EVENT )
    {
        return ctrlKeyState2string(context, pir->Event.MouseEvent.dwControlKeyState);
    }
    return TheNilObj;
}

RexxMethod2(RexxObjectPtr, inrec_setControlKeys, CSTRING, ctrlKeys, CSELF, p)
{
    PINPUT_RECORD pir = (PINPUT_RECORD)p;

    if ( pir->EventType == KEY_EVENT )
    {
        pir->Event.KeyEvent.dwControlKeyState = string2ctrlKeyState(ctrlKeys);
    }
    else if ( pir->EventType == MOUSE_EVENT )
    {
        pir->Event.MouseEvent.dwControlKeyState = string2ctrlKeyState(ctrlKeys);
    }
    return NULLOBJECT;
}

RexxMethod6(RexxObjectPtr, inrec_keyFrom_cls, CSTRING, chari, logical_t, keyDown, uint16_t, vKeyCode,
            uint16_t, vScanCode, uint16_t, repeatCount, CSTRING, controlKeys)
{
    RexxBufferObject obj = context->NewBuffer(sizeof(INPUT_RECORD));

    PINPUT_RECORD pir = (PINPUT_RECORD)context->BufferData(obj);
    pir->EventType = KEY_EVENT;
    pir->Event.KeyEvent.uChar.AsciiChar = *chari;
    pir->Event.KeyEvent.bKeyDown = keyDown ? TRUE : FALSE;
    pir->Event.KeyEvent.wVirtualKeyCode = vKeyCode;
    pir->Event.KeyEvent.wVirtualScanCode = vScanCode;
    pir->Event.KeyEvent.wRepeatCount = repeatCount;
    pir->Event.KeyEvent.dwControlKeyState = string2ctrlKeyState(controlKeys);

    RexxObjectPtr inRecObj = context->SendMessage1(TheInputRecordClass, "NEW", obj);

    return inRecObj;
}

RexxMethod1(RexxObjectPtr, inrec_char, CSELF, p)
{
    PINPUT_RECORD pir = (PINPUT_RECORD)p;

    if ( pir->EventType == KEY_EVENT )
    {
        char buf[2] = {'\0'};
        buf[0] = pir->Event.KeyEvent.uChar.AsciiChar;
        return context->String(buf);
    }
    return TheNilObj;
}
RexxMethod2(RexxObjectPtr, inrec_setChar, CSTRING, chari, CSELF, p)
{
    PINPUT_RECORD pir = (PINPUT_RECORD)p;

    if ( pir->EventType == KEY_EVENT )
    {
        pir->Event.KeyEvent.uChar.AsciiChar = *chari;
    }
    return NULLOBJECT;
}

RexxMethod1(RexxObjectPtr, inrec_keyDown, CSELF, p)
{
    PINPUT_RECORD pir = (PINPUT_RECORD)p;

    if ( pir->EventType == KEY_EVENT )
    {
        return pir->Event.KeyEvent.bKeyDown ? TheTrueObj : TheFalseObj;
    }
    return TheNilObj;
}

RexxMethod2(RexxObjectPtr, inrec_setKeyDown, logical_t, keyDown, CSELF, p)
{
    PINPUT_RECORD pir = (PINPUT_RECORD)p;

    if ( pir->EventType == KEY_EVENT )
    {
        pir->Event.KeyEvent.bKeyDown = keyDown ? TRUE : FALSE;
    }
    return NULLOBJECT;
}

RexxMethod1(RexxObjectPtr, inrec_vKeyCode, CSELF, p)
{
    PINPUT_RECORD pir = (PINPUT_RECORD)p;

    if ( pir->EventType == KEY_EVENT )
    {
        return context->UnsignedInt32(pir->Event.KeyEvent.wVirtualKeyCode);
    }
    return TheNilObj;
}

RexxMethod2(RexxObjectPtr, inrec_setVKeyCode, uint32_t, vKeyCode, CSELF, p)
{
    PINPUT_RECORD pir = (PINPUT_RECORD)p;

    if ( pir->EventType == KEY_EVENT )
    {
        pir->Event.KeyEvent.wVirtualKeyCode = vKeyCode;
    }
    return NULLOBJECT;
}

RexxMethod1(RexxObjectPtr, inrec_vScanCode, CSELF, p)
{
    PINPUT_RECORD pir = (PINPUT_RECORD)p;

    if ( pir->EventType == KEY_EVENT )
    {
        return context->UnsignedInt32(pir->Event.KeyEvent.wVirtualScanCode);
    }
    return TheNilObj;
}

RexxMethod2(RexxObjectPtr, inrec_setVScanCode, uint32_t, vScanCode, CSELF, p)
{
    PINPUT_RECORD pir = (PINPUT_RECORD)p;

    if ( pir->EventType == KEY_EVENT )
    {
        pir->Event.KeyEvent.wVirtualScanCode = vScanCode;
    }
    return NULLOBJECT;
}

RexxMethod1(RexxObjectPtr, inrec_repeatCount, CSELF, p)
{
    PINPUT_RECORD pir = (PINPUT_RECORD)p;

    if ( pir->EventType == KEY_EVENT )
    {
        return context->UnsignedInt32(pir->Event.KeyEvent.wRepeatCount);
    }
    return TheNilObj;
}

RexxMethod2(RexxObjectPtr, inrec_setRepeatCount, uint32_t, repeatCount, CSELF, p)
{
    PINPUT_RECORD pir = (PINPUT_RECORD)p;

    if ( pir->EventType == KEY_EVENT )
    {
        pir->Event.KeyEvent.wRepeatCount = repeatCount;
    }
    return NULLOBJECT;
}

RexxMethod4(RexxObjectPtr, inrec_mouseFrom_cls, RexxObjectPtr, position, CSTRING, buttonState, CSTRING, controlKeys,
            CSTRING, eventFlag)
{
    COORD pos;
    if ( ! rxGetPointerCoord(context, position, 1, &pos) )
    {
        return NULLOBJECT;
    }

    RexxBufferObject obj = context->NewBuffer(sizeof(INPUT_RECORD));

    PINPUT_RECORD pir = (PINPUT_RECORD)context->BufferData(obj);
    pir->EventType = MOUSE_EVENT;
    pir->Event.MouseEvent.dwMousePosition   = pos;
    pir->Event.MouseEvent.dwButtonState     = string2buttonState(buttonState);
    pir->Event.MouseEvent.dwControlKeyState = string2ctrlKeyState(controlKeys);
    pir->Event.MouseEvent.dwEventFlags      = string2eventFlag(eventFlag);

    RexxObjectPtr inRecObj = context->SendMessage1(TheInputRecordClass, "NEW", obj);

    return inRecObj;
}

RexxMethod1(RexxObjectPtr, inrec_position, CSELF, p)
{
    PINPUT_RECORD pir = (PINPUT_RECORD)p;

    if ( pir->EventType == MOUSE_EVENT )
    {
        return rxNewPoint(context, pir->Event.MouseEvent.dwMousePosition.X, pir->Event.MouseEvent.dwMousePosition.Y);
    }
    return TheNilObj;
}
RexxMethod2(RexxObjectPtr, inrec_setPosition, RexxObjectPtr, position, CSELF, p)
{
    PINPUT_RECORD pir = (PINPUT_RECORD)p;

    if ( pir->EventType == MOUSE_EVENT )
    {
        COORD pos;
        if ( ! rxGetPointerCoord(context, position, 1, &pos) )
        {
            return NULLOBJECT;
        }
        pir->Event.MouseEvent.dwMousePosition = pos;
    }
    return NULLOBJECT;
}

RexxMethod1(RexxObjectPtr, inrec_buttonState, CSELF, p)
{
    PINPUT_RECORD pir = (PINPUT_RECORD)p;

    if ( pir->EventType == MOUSE_EVENT )
    {
        return buttonState2string(context, pir->Event.MouseEvent.dwButtonState);
    }
    return TheNilObj;
}

RexxMethod2(RexxObjectPtr, inrec_setButtonState, CSTRING, state, CSELF, p)
{
    PINPUT_RECORD pir = (PINPUT_RECORD)p;

    if ( pir->EventType == MOUSE_EVENT )
    {
        pir->Event.MouseEvent.dwButtonState = string2buttonState(state);
    }
    return NULLOBJECT;
}

RexxMethod1(RexxObjectPtr, inrec_eventFlag, CSELF, p)
{
    PINPUT_RECORD pir = (PINPUT_RECORD)p;

    if ( pir->EventType == MOUSE_EVENT )
    {
        return eventFlag2string(context, pir->Event.MouseEvent.dwEventFlags);
    }
    return TheNilObj;
}

RexxMethod2(RexxObjectPtr, inrec_setEventFlag, CSTRING, event, CSELF, p)
{
    PINPUT_RECORD pir = (PINPUT_RECORD)p;

    if ( pir->EventType == MOUSE_EVENT )
    {
        pir->Event.MouseEvent.dwEventFlags = string2eventFlag(event);
    }
    return NULLOBJECT;
}

RexxMethod1(RexxObjectPtr, inrec_windowFrom_cls, RexxObjectPtr, _size)
{
    COORD size;
    if ( ! rxGetPointerCoord(context, _size, 1, &size) )
    {
        return NULLOBJECT;
    }

    RexxBufferObject obj = context->NewBuffer(sizeof(INPUT_RECORD));

    PINPUT_RECORD pir = (PINPUT_RECORD)context->BufferData(obj);
    pir->EventType = WINDOW_BUFFER_SIZE_EVENT;
    pir->Event.WindowBufferSizeEvent.dwSize = size;

    RexxObjectPtr inRecObj = context->SendMessage1(TheInputRecordClass, "NEW", obj);

    return inRecObj;
}

RexxMethod1(RexxObjectPtr, inrec_size, CSELF, p)
{
    PINPUT_RECORD pir = (PINPUT_RECORD)p;

    if ( pir->EventType == WINDOW_BUFFER_SIZE_EVENT )
    {
        return rxNewPoint(context, pir->Event.WindowBufferSizeEvent.dwSize.X, pir->Event.WindowBufferSizeEvent.dwSize.Y);
    }
    return TheNilObj;
}
RexxMethod2(RexxObjectPtr, inrec_setSize, RexxObjectPtr, _size, CSELF, p)
{
    PINPUT_RECORD pir = (PINPUT_RECORD)p;

    if ( pir->EventType == WINDOW_BUFFER_SIZE_EVENT )
    {
        COORD size;
        if ( ! rxGetPointerCoord(context, _size, 1, &size) )
        {
            return NULLOBJECT;
        }
        pir->Event.WindowBufferSizeEvent.dwSize = size;
    }
    return NULLOBJECT;
}

RexxMethod1(RexxObjectPtr, inrec_focusFrom_cls, logical_t, setFocus)
{
    RexxBufferObject obj = context->NewBuffer(sizeof(INPUT_RECORD));

    PINPUT_RECORD pir = (PINPUT_RECORD)context->BufferData(obj);
    pir->EventType = FOCUS_EVENT;
    pir->Event.FocusEvent.bSetFocus = setFocus ? TRUE : FALSE;

    RexxObjectPtr inRecObj = context->SendMessage1(TheInputRecordClass, "NEW", obj);

    return inRecObj;
}

RexxMethod1(RexxObjectPtr, inrec_setFocus, CSELF, p)
{
    PINPUT_RECORD pir = (PINPUT_RECORD)p;

    if ( pir->EventType == FOCUS_EVENT )
    {
        return context->Logical(pir->Event.FocusEvent.bSetFocus);
    }
    return TheNilObj;
}
RexxMethod2(RexxObjectPtr, inrec_setSetFocus, logical_t, setFocus, CSELF, p)
{
    PINPUT_RECORD pir = (PINPUT_RECORD)p;

    if ( pir->EventType == FOCUS_EVENT )
    {
        pir->Event.FocusEvent.bSetFocus = setFocus ? TRUE : FALSE;
    }
    return NULLOBJECT;
}

RexxMethod1(RexxObjectPtr, inrec_menuFrom_cls, uint32_t, ID)
{
    RexxBufferObject obj = context->NewBuffer(sizeof(INPUT_RECORD));

    PINPUT_RECORD pir = (PINPUT_RECORD)context->BufferData(obj);
    pir->EventType = MENU_EVENT;
    pir->Event.MenuEvent.dwCommandId = ID;

    RexxObjectPtr inRecObj = context->SendMessage1(TheInputRecordClass, "NEW", obj);

    return inRecObj;
}

RexxMethod1(RexxObjectPtr, inrec_ID, CSELF, p)
{
    PINPUT_RECORD pir = (PINPUT_RECORD)p;

    if ( pir->EventType == MENU_EVENT )
    {
        return context->UnsignedInt32(pir->Event.MenuEvent.dwCommandId);
    }
    return TheNilObj;
}
RexxMethod2(RexxObjectPtr, inrec_setID, uint32_t, ID, CSELF, p)
{
    PINPUT_RECORD pir = (PINPUT_RECORD)p;

    if ( pir->EventType == MENU_EVENT )
    {
        pir->Event.MenuEvent.dwCommandId = ID;
    }
    return NULLOBJECT;
}


// .ooConsole
REXX_METHOD_PROTOTYPE(oocon_allocConsole_cls);
REXX_METHOD_PROTOTYPE(oocon_attachConsole_cls);
REXX_METHOD_PROTOTYPE(oocon_freeConsole_cls);
REXX_METHOD_PROTOTYPE(oocon_version_cls);

REXX_METHOD_PROTOTYPE(oocon_errRC_atr);
REXX_METHOD_PROTOTYPE(oocon_handle_atr);
REXX_METHOD_PROTOTYPE(oocon_init);
REXX_METHOD_PROTOTYPE(oocon_uninit);
REXX_METHOD_PROTOTYPE(oocon_addAlias);
REXX_METHOD_PROTOTYPE(oocon_generateCtrlEvent);
REXX_METHOD_PROTOTYPE(oocon_getAlias);
REXX_METHOD_PROTOTYPE(oocon_getAliases);
REXX_METHOD_PROTOTYPE(oocon_getAliasesLength);
REXX_METHOD_PROTOTYPE(oocon_getAliasExes);
REXX_METHOD_PROTOTYPE(oocon_getAliasExesLength);
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
REXX_METHOD_PROTOTYPE(oocon_setCtrlHandler);
REXX_METHOD_PROTOTYPE(oocon_setHistoryInfo);
REXX_METHOD_PROTOTYPE(oocon_setOutputCP);
REXX_METHOD_PROTOTYPE(oocon_setTitle);

REXX_METHOD_PROTOTYPE(oocon_test);


// .ScreenBuffer
REXX_METHOD_PROTOTYPE(sb_close);
REXX_METHOD_PROTOTYPE(sb_createScreenBuffer);
REXX_METHOD_PROTOTYPE(sb_fillOutputAttribute);
REXX_METHOD_PROTOTYPE(sb_fillOutputCharacter);
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
REXX_METHOD_PROTOTYPE(sb_readOutput);
REXX_METHOD_PROTOTYPE(sb_readOutputAttribute);
REXX_METHOD_PROTOTYPE(sb_readOutputCharacter);
REXX_METHOD_PROTOTYPE(sb_scrollScreenBuffer);
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
REXX_METHOD_PROTOTYPE(sb_writeOutput);
REXX_METHOD_PROTOTYPE(sb_writeOutputAttribute);
REXX_METHOD_PROTOTYPE(sb_writeOutputCharacter);

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

// .CharInfo
REXX_METHOD_PROTOTYPE(char_init_cls);
REXX_METHOD_PROTOTYPE(char_from_cls);
REXX_METHOD_PROTOTYPE(char_init);
REXX_METHOD_PROTOTYPE(char_char);
REXX_METHOD_PROTOTYPE(char_attribute);
REXX_METHOD_PROTOTYPE(char_setChar);
REXX_METHOD_PROTOTYPE(char_setAttribute);

// .InputRecord
REXX_METHOD_PROTOTYPE(inrec_init_cls);
REXX_METHOD_PROTOTYPE(inrec_init);
REXX_METHOD_PROTOTYPE(inrec_controlKeys);
REXX_METHOD_PROTOTYPE(inrec_setControlKeys);
REXX_METHOD_PROTOTYPE(inrec_keyFrom_cls);
REXX_METHOD_PROTOTYPE(inrec_char);
REXX_METHOD_PROTOTYPE(inrec_setChar);
REXX_METHOD_PROTOTYPE(inrec_keyDown);
REXX_METHOD_PROTOTYPE(inrec_setKeyDown);
REXX_METHOD_PROTOTYPE(inrec_vKeyCode);
REXX_METHOD_PROTOTYPE(inrec_setVKeyCode);
REXX_METHOD_PROTOTYPE(inrec_vScanCode);
REXX_METHOD_PROTOTYPE(inrec_setVScanCode);
REXX_METHOD_PROTOTYPE(inrec_repeatCount);
REXX_METHOD_PROTOTYPE(inrec_setRepeatCount);
REXX_METHOD_PROTOTYPE(inrec_mouseFrom_cls);
REXX_METHOD_PROTOTYPE(inrec_position);
REXX_METHOD_PROTOTYPE(inrec_setPosition);
REXX_METHOD_PROTOTYPE(inrec_buttonState);
REXX_METHOD_PROTOTYPE(inrec_setButtonState);
REXX_METHOD_PROTOTYPE(inrec_eventFlag);
REXX_METHOD_PROTOTYPE(inrec_setEventFlag);
REXX_METHOD_PROTOTYPE(inrec_windowFrom_cls);
REXX_METHOD_PROTOTYPE(inrec_size);
REXX_METHOD_PROTOTYPE(inrec_setSize);
REXX_METHOD_PROTOTYPE(inrec_focusFrom_cls);
REXX_METHOD_PROTOTYPE(inrec_setFocus);
REXX_METHOD_PROTOTYPE(inrec_setSetFocus);
REXX_METHOD_PROTOTYPE(inrec_menuFrom_cls);
REXX_METHOD_PROTOTYPE(inrec_ID);
REXX_METHOD_PROTOTYPE(inrec_setID);


RexxMethodEntry ooConsole_methods[] = {
    REXX_METHOD(oocon_allocConsole_cls,           oocon_allocConsole_cls),
    REXX_METHOD(oocon_attachConsole_cls,          oocon_attachConsole_cls),
    REXX_METHOD(oocon_freeConsole_cls,            oocon_freeConsole_cls),
    REXX_METHOD(oocon_version_cls,                oocon_version_cls),
    REXX_METHOD(oocon_errRC_atr,                  oocon_errRC_atr),
    REXX_METHOD(oocon_handle_atr,                 oocon_handle_atr),
    REXX_METHOD(oocon_init,                       oocon_init),
    REXX_METHOD(oocon_uninit,                     oocon_uninit),
    REXX_METHOD(oocon_addAlias,                   oocon_addAlias),
    REXX_METHOD(oocon_generateCtrlEvent,          oocon_generateCtrlEvent),
    REXX_METHOD(oocon_getAlias,                   oocon_getAlias),
    REXX_METHOD(oocon_getAliases,                 oocon_getAliases),
    REXX_METHOD(oocon_getAliasesLength,           oocon_getAliasesLength),
    REXX_METHOD(oocon_getAliasExes,               oocon_getAliasExes),
    REXX_METHOD(oocon_getAliasExesLength,         oocon_getAliasExesLength),
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
    REXX_METHOD(oocon_setCtrlHandler,             oocon_setCtrlHandler),
    REXX_METHOD(oocon_setHistoryInfo,             oocon_setHistoryInfo),
    REXX_METHOD(oocon_setOutputCP,                oocon_setOutputCP),
    REXX_METHOD(oocon_setTitle,	                  oocon_setTitle),

    REXX_METHOD(oocon_test,                       oocon_test),

    REXX_METHOD(sb_close,                         sb_close),
    REXX_METHOD(sb_createScreenBuffer,            sb_createScreenBuffer),
    REXX_METHOD(sb_fillOutputAttribute,           sb_fillOutputAttribute),
    REXX_METHOD(sb_fillOutputCharacter,           sb_fillOutputCharacter),
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
    REXX_METHOD(sb_readOutput,                    sb_readOutput),
    REXX_METHOD(sb_readOutputAttribute,           sb_readOutputAttribute),
    REXX_METHOD(sb_readOutputCharacter,           sb_readOutputCharacter),
    REXX_METHOD(sb_scrollScreenBuffer,            sb_scrollScreenBuffer),
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
    REXX_METHOD(sb_writeOutput,                   sb_writeOutput),
    REXX_METHOD(sb_writeOutputAttribute,          sb_writeOutputAttribute),
    REXX_METHOD(sb_writeOutputCharacter,          sb_writeOutputCharacter),

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
    REXX_METHOD(stdin_writeInput,                 stdin_writeInput),

    // .CharInfo
    REXX_METHOD(char_init_cls,                    char_init_cls),
    REXX_METHOD(char_from_cls,                      char_from_cls),
    REXX_METHOD(char_init,                        char_init),
    REXX_METHOD(char_char,                        char_char),
    REXX_METHOD(char_attribute,                   char_attribute),
    REXX_METHOD(char_setChar,                     char_setChar),
    REXX_METHOD(char_setAttribute,                char_setAttribute),

    // .InputRecord
    REXX_METHOD(inrec_init_cls,                   inrec_init_cls),
    REXX_METHOD(inrec_init,                       inrec_init),
    REXX_METHOD(inrec_controlKeys,                inrec_controlKeys),
    REXX_METHOD(inrec_setControlKeys,             inrec_setControlKeys),
    REXX_METHOD(inrec_keyFrom_cls,                inrec_keyFrom_cls),
    REXX_METHOD(inrec_char,                       inrec_char),
    REXX_METHOD(inrec_setChar,                    inrec_setChar),
    REXX_METHOD(inrec_keyDown,                    inrec_keyDown),
    REXX_METHOD(inrec_setKeyDown,                 inrec_setKeyDown),
    REXX_METHOD(inrec_vKeyCode,                   inrec_vKeyCode),
    REXX_METHOD(inrec_setVKeyCode,                inrec_setVKeyCode),
    REXX_METHOD(inrec_vScanCode,                  inrec_vScanCode),
    REXX_METHOD(inrec_setVScanCode,               inrec_setVScanCode),
    REXX_METHOD(inrec_repeatCount,                inrec_repeatCount),
    REXX_METHOD(inrec_setRepeatCount,             inrec_setRepeatCount),
    REXX_METHOD(inrec_mouseFrom_cls,              inrec_mouseFrom_cls),
    REXX_METHOD(inrec_position,                   inrec_position),
    REXX_METHOD(inrec_setPosition,                inrec_setPosition),
    REXX_METHOD(inrec_buttonState,                inrec_buttonState),
    REXX_METHOD(inrec_setButtonState,             inrec_setButtonState),
    REXX_METHOD(inrec_eventFlag,                  inrec_eventFlag),
    REXX_METHOD(inrec_setEventFlag,               inrec_setEventFlag),
    REXX_METHOD(inrec_windowFrom_cls,             inrec_windowFrom_cls),
    REXX_METHOD(inrec_size,                       inrec_size),
    REXX_METHOD(inrec_setSize,                    inrec_setSize),
    REXX_METHOD(inrec_focusFrom_cls,              inrec_focusFrom_cls),
    REXX_METHOD(inrec_setFocus,                   inrec_setFocus),
    REXX_METHOD(inrec_setSetFocus,                inrec_setSetFocus),
    REXX_METHOD(inrec_menuFrom_cls,               inrec_menuFrom_cls),
    REXX_METHOD(inrec_ID,                         inrec_ID),
    REXX_METHOD(inrec_setID,                      inrec_setID),

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
