/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 1995, 2004 IBM Corporation. All rights reserved.             */
/* Copyright (c) 2005-2014 Rexx Language Association. All rights reserved.    */
/*                                                                            */
/* This program and the accompanying materials are made available under       */
/* the terms of the Common Public License v1.0 which accompanies this         */
/* distribution. A copy is also available at the following address:           */
/* https://www.oorexx.org/license.html                                        */
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
/******************************************************************************/
/*  winextf.c - Methods to resolve external function calls.                   */
/*                                                                            */
/*  C methods:                                                                */
/*    sysBeep     - Method for the BEEP BIF                                   */
/*    sysSetLocal - Method for the SETLOCAL BIF                               */
/*    sysEndLocal - Method for the ENDLOCAL BIF                               */
/*    sysDirectory- Method for the DIRECTORY BIF                              */
/*    SysExternalFunction- Method for searching/invoking an external function */
/*    sysMessageBox - Method to pop up message box                            */
/*                                                                            */
/******************************************************************************/
#include <process.h>
#include <direct.h>
#include <windows.h>
#include <CommCtrl.h>

#include "RexxCore.h"                         /* global REXX definitions        */
#include "StringClass.hpp"
#include "ArrayClass.hpp"
#include "Activity.hpp"
#include "RexxActivation.hpp"
#include "MethodClass.hpp"
#include "RexxInternalApis.h"          /* Get private REXXAPI API's         */
#include "ProtectedObject.hpp"
#include "StringUtil.hpp"
#include "PackageManager.hpp"
#include "SystemInterpreter.hpp"
#include "StemClass.hpp"
#include "NativeActivation.hpp"

// conflict between oorexx PlatformDefinitions.h and Microsoft dwrite.h
#ifdef delimiter
  #undef delimiter
#endif
#include "CustomMessageBoxW.h"

#define DIRLEN        256                   /* length of a directory          */

typedef struct _ENVENTRY {                  /* setlocal/endlocal structure    */
  size_t   DriveNumber;                     /* saved drive                    */
  char     Directory[MAX_PATH];             /* saved current directory        */
  char    *Environment;                     /* saved environment segment      */
  char     Variables[1];                    /* start of variable values       */
} ENVENTRY;


/******************************************************************************/
/* Name:       SysExternalFunction                                            */
/*                                                                            */
/* Notes:      Handles searching for and executing an external function.  The */
/*             search order is:                                               */
/*               1) Macro-space pre-order functions                           */
/*               2) Registered external functions                             */
/*               3) REXX programs with same extension (if applicable)         */
/*               4) REXX programs with default extension                      */
/*               5) Macro-space post-order functions                          */
/******************************************************************************/
bool SystemInterpreter::invokeExternalFunction(
  RexxActivation * activation,         /* Current Activation                */
  Activity   * activity,           /* activity in use                   */
  RexxString     * target,             /* Name of external function         */
  RexxObject    ** arguments,          /* Argument array                    */
  size_t           argcount,           /* count of arguments                */
  RexxString     * calltype,           /* Type of call                      */
  ProtectedObject &result)
{
  if (activation->callMacroSpaceFunction(target, arguments, argcount, calltype, RXMACRO_SEARCH_BEFORE, result))
  {
      return true;
  }
                                       /* no luck try for a registered func */
  if (PackageManager::callNativeRoutine(activity, target, arguments, argcount, result))
  {
      return true;
  }
                                       /* have activation do the call       */
  if (activation->callExternalRexx(target, arguments, argcount, calltype, result))
  {
      return true;
  }
                                       /* function.  If still not found,    */
                                       /* then raise an error               */
  if (activation->callMacroSpaceFunction(target, arguments, argcount, calltype, RXMACRO_SEARCH_AFTER, result))
  {
      return true;
  }

  return false;
}


/*********************************************************************/
/*                                                                   */
/*   Subroutine Name:   RestoreEnvironment                           */
/*                                                                   */
/*   Descriptive Name:  restores environment saved by Setlocal()     */
/*                                                                   */
/*   Function:          restores the environment variables, current  */
/*                      directory and drive.                         */
/*                                                                   */
/*********************************************************************/

void SystemInterpreter::restoreEnvironment(void *CurrentEnv)
{
}


/**
 * Helper function for sysMessageBox().  Parses the other message box style
 * argument and adds the styles specified.
 *
 * @param other  The 'additional message box styles' argument to RxMessageBox().
 *               This can be more than one style keyword, separated by spaces.
 *
 * @param style  Pointer to the MessageBox style flags collected so far. On
 *               return this will be augmented with the additional styles.
 *
 * @param help   Pointer to the help flag. Will be set to true if "HELP".
 *
 * @param custom Pointer to the custom flag. Will be set to true if "CUSTOM".
 *
 * @return True on no error, false if the user passed an unrecognizable keyword
 *         and a condition should be raised.
 *
 * @note   Assumes other is not NULL.
 */
static bool addMBStyle(CSTRING other, ULONG *style, bool *help, bool *custom)
{
    char *token;
    AutoFree str = strdup(other);
    if (str == NULL)
    {
        return false;
    }

    ULONG extraFlag[] =
    {
        // Default button category.  MB_DEFBUTTON1 is the default
        MB_DEFBUTTON2,
        MB_DEFBUTTON3,
        MB_DEFBUTTON4,
        // Modal category.  MB_APPLMODAL is the default.
        MB_SYSTEMMODAL,
        MB_TASKMODAL,
        // Miscellaneous category.  There is no default here.  MB_SETFOREGROUND
        // is already set.  MB_SERVICE_NOTIFICATION_NT3X makes no sense since NT
        // is not supported
        MB_DEFAULT_DESKTOP_ONLY,
        MB_RIGHT,
        MB_RTLREADING,
        MB_TOPMOST,
        MB_SERVICE_NOTIFICATION
    };

    const char *extra[] =
    {
        // Default button category.
        "DEFBUTTON2",
        "DEFBUTTON3",
        "DEFBUTTON4",
        // Modal category.
        "SYSTEMMODAL",
        "TASKMODAL",
        // Miscellaneous category.
        "DEFAULTDESKTOP",
        "RIGHT",
        "RTLREADING",
        "TOPMOST",
        "SERVICENOTIFICATION"
    };

    ULONG extraStyle = 0;
    int i;
    int count = sizeof(extra) / sizeof(const char *);

    token = strtok(str, " ");
    while ( token != NULL )
    {
        for ( i = 0; i < count; i += 1 )
        {
            if ( !stricmp(token, extra[i]) )
            {
                extraStyle |= extraFlag[i];
                break;
            }
            if ( !stricmp(token, "HELP") )
            {
                *help = true;
                break;
            }
            if ( !stricmp(token, "CUSTOM") )
            {
                *custom = true;
                break;
            }
        }

        if ( i == count )
        {
            // User sent a bad keyword.
            return false;
        }

        token = strtok(NULL, " ");
    }

    *style = *style | extraStyle;
    return true;
}

/**
 * Helper function for sysMessageBox().
 * Convert to Unicode using the external function "SysToUnicode".
 *
 * @param context        The current activation context.
 * @param string         The string to convert to Unicode.
 * @param codePage       The string's code page passed to SysToUnicode,
 *                       or "UNICODE" if the string is already UTF-16.
 * @param translateFlags The flags passed to SysToUnicode.
 * @param stem           The stem passed to SysToUnicode.
 * @param tail           The tail to protect the string returned by SysToUnicode.
 * @param errorCode      The address of the variable to return the error code.
 * @return               The UTF-16 string, or NULL.
 */
static LPCWSTR toUnicode(RexxActivation *context, RexxString *string, RexxString *codePage, RexxString *translateFlags, StemClass *stem, const char *tail, wholenumber_t *errorCode)
{
    *errorCode = 0;

    if (string == NULL) return (LPCWSTR)NULL;

    size_t stringLength =  string->getLength();
    if (stringLength == 0)
    {
        static const char *empty = "\0\0";
        return (LPCWSTR)empty;
    }

    if (codePage->strCaselessCompare("UNICODE"))
    {
        // The string is already UTF-16, no need to convert to unicode
        return (LPCWSTR)string->getStringData();
    }

    Protected<RexxString> functionName = new_string("SYSTOUNICODE");
    RexxObject *arguments[] = {string, codePage, translateFlags, stem};
    size_t argumentsCount = sizeof(arguments) / sizeof(RexxObject *);
    ProtectedObject result;
    SystemInterpreter::invokeExternalFunction(context, context->getActivity(), functionName, arguments, argumentsCount, GlobalNames::FUNCTION, result);

    *errorCode = ((RexxInteger *)(RexxObject *)result)->getValue();
    if (*errorCode != 0)
    {
        return (LPCWSTR)NULL;
    }

    RexxString *stringUTF16 = (RexxString *)stem->getElement("!TEXT");
    // protect because !TEXT will be overwritten on the next call
    stem->setElement(tail, stringUTF16);
    // It's safe to return this stringData because the string is protected by the stem
    return (LPCWSTR)stringUTF16->getStringData();
}

/*********************************************************************/
/*                                                                   */
/*   Subroutine Name:   RxMessageBox                                 */
/*                                                                   */
/*   Descriptive Name:  RxMessageBox function                        */
/*                                                                   */
/*   Function:          pops up a PM message box when called from    */
/*                      a PM session.                                */
/*   Parameters:                                                     */
/*        Input:                                                     */
/*                      Text   = The message box text.               */
/*                      Title  = The message box title.              */
/*                      Button = The message box button style.       */
/*                      Icon   = The message box icon style.         */
/*                      Other  = Additional message box styles.      */
/*                      CodePage = The text and title code page.     */
/*                      TranslateFlags = flags for SysToUnicode.     */
/*                                                                   */
/*   Notes:             If codePage is omitted or is an empty string,*/
/*                      the text and title (which should be ANSI     */
/*                      strings) are converted from ACP to Unicode   */
/*                      by the system.                               */
/*                                                                   */
/*                      If codePage is "UNICODE", the text and title */
/*                      (which should be UTF-16 strings) are         */
/*                      displayed as-is.                             */
/*                                                                   */
/*                      For any other value of codepage, the text    */
/*                      and title (which should be strings encoded   */
/*                      in codepage) are converted from codepage to  */
/*                      Unicode using the Rexx utility SysToUnicode. */
/*                                                                   */
/*********************************************************************/
RexxRoutine7(int, sysMessageBox, RexxStringObject, text, OPTIONAL_RexxStringObject, title,
             OPTIONAL_CSTRING, button, OPTIONAL_CSTRING, icon, OPTIONAL_CSTRING, other,
             OPTIONAL_RexxStringObject, codePage, OPTIONAL_RexxStringObject, translateFlags)
{
    ULONG       style;                   /* window style flags         */
    int         maxCnt;                  /* Max loop count             */
    int         index;                   /* table index                */

    INITCOMMONCONTROLSEX ctrlex;
    ctrlex.dwSize = sizeof(ctrlex);
    ctrlex.dwICC = ICC_WIN95_CLASSES | ICC_DATE_CLASSES;
    InitCommonControlsEx(&ctrlex);

    PSZ Button_Styles[] =                /* message box button styles  */
    {
        "ABORTRETRYIGNORE",
        "CANCELTRYCONTINUE",
        "HELP",
        "OK",
        "OKCANCEL",
        "RETRYCANCEL",
        "YESNO",
        "YESNOCANCEL"
    };

    ULONG Button_Flags[] =               /* message box button styles  */
    {
        MB_ABORTRETRYIGNORE,
        MB_CANCELTRYCONTINUE,
        MB_HELP,
        MB_OK,
        MB_OKCANCEL,
        MB_RETRYCANCEL,
        MB_YESNO,
        MB_YESNOCANCEL
    };

    PSZ Icon_Styles[] =                  /* message box icon styles    */
    {
        "EXCLAMATION",
        "WARNING",
        "INFORMATION",
        "ASTERISK",
        "QUESTION",
        "STOP",
        "ERROR",
        "HAND",
        "QUERY",
        "NONE"
    };

    ULONG Icon_Flags[] =                 /* message box icon styles    */
    {
        MB_ICONEXCLAMATION,
        MB_ICONWARNING,
        MB_ICONINFORMATION,
        MB_ICONASTERISK,
        MB_ICONQUESTION,
        MB_ICONSTOP,
        MB_ICONERROR,
        MB_ICONHAND,
        MB_ICONQUESTION,
        0
    };

    // Always try to make the message box the foreground.
    style = MB_SETFOREGROUND;

    if ( button == NULL )
    {
        // The default is an OK message box.
        style |= MB_OK;
    }
    else
    {
        // Get the number of styles and search the button style table.
        maxCnt = sizeof(Button_Styles) / sizeof(PSZ);
        for ( index = 0; index < maxCnt; index++ )
        {
            if ( !stricmp(button, Button_Styles[index]) )
            {
                // Found a match.  Only 1 button style can be used, so break.
                style |= Button_Flags[index];
                break;
            }
        }
        if ( index == maxCnt )
        {
            // User specified an invalid button style word.
            context->InvalidRoutine();
            return 0;
        }
    }

    if ( icon != NULL )
    {
        // There is no default icon.  The user can also explicitly specify no
        // icon by using the NONE keyword.
        maxCnt = sizeof(Icon_Styles)/sizeof(PSZ);
        for ( index = 0; index < maxCnt; index += 1 )
        {
            if ( !stricmp(icon,Icon_Styles[index]) )
            {
                // Found a match.  Only 1 icon stle can be used, so break.
                style |= Icon_Flags[index];
                break;
            }
        }
        if ( index == maxCnt )
        {
            // User specified an invalid icon style word.
            context->InvalidRoutine();
            return 0;
        }

    }

    bool help = false;
    bool custom = false;
    if ( other != NULL )
    {
        if ( ! addMBStyle(other, &style, &help, &custom) )
        {
            // User specified an invalid key word for one of the extra styles.
            context->InvalidRoutine();
            return 0;
        }
    }
    if (help) style |= MB_HELP;

    if (codePage != NULL && context->StringLength(codePage) != 0)
    {
        // The user provided a code page, let's switch to Unicode...
        RexxActivation *activation = contextToActivation(context)->findRexxContext();
        if (activation == OREF_NULL)
        {
            // activation can be NULL, when no parent Rexx context.
            // In this case, it's not possible to call SYSTOUNICODE.
            context->InvalidRoutine();
            return 0;
        }
        // Declared here, not in toUnicode, to protect the result of toUnicode
        Protected<StemClass> pstem = new StemClass(OREF_NULL);

        wholenumber_t textError = 0;
        LPCWSTR textW = toUnicode(activation, (RexxString *)text, (RexxString *)codePage, (RexxString *)translateFlags, (StemClass *)pstem, "PROTECTED_TEXTW", &textError);

        wholenumber_t titleError = 0;
        LPCWSTR titleW = toUnicode(activation, (RexxString *)title, (RexxString *)codePage, (RexxString *)translateFlags, (StemClass *)pstem, "PROTECTED_TITLEW", &titleError);

        if (textError != 0 || titleError != 0)
        {
            context->InvalidRoutine();
            return 0;
        }

        if (custom) return CustomMessageBoxW(NULL, textW, titleW, style);
        return MessageBoxW(NULL, textW, titleW, style);
    }

    // Legacy ACP (ANSI code page)
    const char *textA = text ? context->StringData(text) : NULL;
    const char *titleA = title ? context->StringData(title) : NULL;
    return MessageBoxA(NULL, textA, titleA, style);
}


/**
 * Push a new environment for the SysSetLocal() BIF.
 *
 * @param context The current activation context.
 *
 * @return Returns TRUE if the environment was successfully pushed.
 */
RexxObject *SystemInterpreter::pushEnvironment(RexxActivation *context)
{
    return TheFalseObject;
}

/**
 * Pop an environment for the SysEndLocal() BIF.
 *
 * @param context The current activation context.
 *
 * @return Always returns FALSE.  This is a NOP on Windows.
 */
RexxObject *SystemInterpreter::popEnvironment(RexxActivation *context)
{
    return TheFalseObject;             /* return failure value              */
}
