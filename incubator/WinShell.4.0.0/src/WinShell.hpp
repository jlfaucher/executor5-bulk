/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2008 Rexx Language Association. All rights reserved.         */
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

#ifndef WINSHELL_HPP
#define WINSHELL_HPP

#pragma data_seg("shared_data")
static long globalInstances = 0;
#pragma data_seg()

static long threadInstances = 0;
static HINSTANCE thisModule = NULL;
static bool fileIconInitDone = false;

#define SHELL_DLL "WinShell.dll"

#define HINT_ID 0x00003749

#define REQUIRE_W2K_MSG           "the WinShell objects require Windows 2000 SP 4 or later"
#define MEMORY_ALLOC_FAILED_MSG   "failed to allocate memory"
#define STRING_TO_PTR_FAILED_MSG  "failed to convert string to pointer"
#define UNICODE_ANSI_FAILED_MSG   "conversion between UNICODE and ANSI failed"
#define INVALID_WINDOW_MSG        "the owner window handle is not valid"
#define WINDOW_HANDLE_MSG         "\"a valid window handle\""
#define QUALIFIED_PATH_MSG        "\"a fully qualified path\""
#define ITEMIDLIST_HANDLE_MSG     "\"a handle to an item ID list\""
#define IMAGELIST_HANDLE_MSG      "\"a handle to an image list\""
#define IMAGELIST_RELEASED_MSG    "an image list can not be used after it has been released"
#define NO_IMAGELIST_MSG          "the Windows Shell did not return the system image list for %s icons"
#define NO_ITEMID_MSG             "the Windows Shell did not return the item ID for %s"
#define NO_HMODULE_MSG            "failed to obtain %s module handle; OS error code %d"
#define NO_PROC_MSG               "failed to obtain %s procedure address; OS error code %d"
#define NO_COMMCTRL_MSG           "failed to initialize %s; OS error code %d"
#define NO_CREATE_MSG             "failed to create %s; OS error code %d"
#define INVALID_CONSTANT_MSG      "the valid %s_XXX constants"

#define PPSF_PATH                 0x0001
#define PPSF_CSIDL                0x0002
#define PPSF_POINTER              0x0004
#define PPSF_ALL_BUT_NIL          0x000F
#define PPSF_NIL                  0x0010
#define PPSF_EMPTY_STRING         0x0020
#define PPSF_ALL                  0x003F
#define PPSF_NULL_OK              0x0040
#define PPSF_OMITTED              0x0080
#define PPSF_OBJECT               0x0100

/** Type defs for Shell functions exported by ordinal only. */
#define FILEICONINIT_ORD  660
typedef BOOL (WINAPI * FILEICONINIT_PROC)(BOOL flag);

typedef struct _BROWSE_DATA
{
    char *startDir;
    char *dlgTitle;
    char *hint;
    bool  usePathForHint;
} BROWSE_DATA, *PBROWSE_DATA;

static bool _isVersion(DWORD, DWORD, unsigned int, unsigned int, unsigned int);

inline bool _is64Bit(void)
{
#if defined(_WIN64)
    return true;
#else
    return false;
#endif
}

inline bool _isW2K(void)
{
    return _isVersion(5, 0, 0, 0, VER_EQUAL);
}

inline BOOL _isAtLeastW2K(void)
{
    return _isVersion(5, 0, 4, 0, VER_GREATER_EQUAL);
}

inline bool _isXP(void)
{
    return (_isVersion(5, 1, 0, 0, VER_EQUAL) || _isVersion(5, 2, 0, VER_NT_WORKSTATION, VER_EQUAL));
}

inline bool _isXP32(void)
{
    return _isVersion(5, 1, 0, 0, VER_EQUAL);
}

inline bool _isXP64(void)
{
    return _isVersion(5, 2, 0, VER_NT_WORKSTATION, VER_EQUAL);
}

inline BOOL _isAtLeastXP(void)
{
    return _isVersion(5, 1, 2, 0, VER_GREATER_EQUAL);
}

inline bool _isW2K3(void)
{
    return (_isVersion(5, 2, 0, VER_NT_DOMAIN_CONTROLLER, VER_EQUAL) ||
            _isVersion(5, 2, 0, VER_NT_SERVER, VER_EQUAL));
}

inline BOOL _isAtLeastW2K3(void)
{
    return (_isVersion(5, 2, 1, VER_NT_DOMAIN_CONTROLLER, VER_EQUAL) ||
            _isVersion(5, 2, 1, VER_NT_SERVER, VER_EQUAL));
}

inline bool _isVista(void)
{
    return _isVersion(6, 0, 0, 0, VER_EQUAL);
}

inline BOOL _isAtLeastVista(void)
{
    return _isVersion(6, 0, 0, 0, VER_GREATER_EQUAL);
}

inline void systemServiceException(RexxMethodContext *context, char *msg)
{
    context->RaiseException1(Rexx_Error_System_service_user_defined, context->NewStringFromAsciiz(msg));
}

inline void systemServiceException(RexxMethodContext *context, char *msg, const char *sub)
{
    if ( sub != NULL )
    {
        TCHAR buffer[128];
        _snprintf(buffer, sizeof(buffer), msg, sub);
        systemServiceException(context, buffer);
    }
    else
    {
        systemServiceException(context, msg);
    }
}

inline void systemServiceExceptionCode(RexxMethodContext *context, const char *msg, const char *arg1)
{
    TCHAR buffer[256];
    _snprintf(buffer, sizeof(buffer), msg, arg1, GetLastError());
    systemServiceException(context, buffer);
}

/**
 * 88.906
 * argument <argument> must not exceed <limit>; found "<value>"
 *
 * argument 1 must not exceed 63 characters in length; found "76"
 *
 * @param argNumber
 * @param max
 * @param actual
 */
inline void stringTooLongException(RexxMethodContext *context, int argNumber, int max, size_t actual)
{
    TCHAR buffer[32];
    RexxArrayObject array = context->NewArray(3);

    _snprintf(buffer, sizeof(buffer), "%d characters in length", max);

    context->ArrayPut(array, context->NewInteger(argNumber), 1);
    context->ArrayPut(array, context->NewStringFromAsciiz(buffer), 2);
    context->ArrayPut(array, context->NewInteger(actual), 3);

    context->RaiseExceptionArray(Rexx_Error_Invalid_argument_toobig, array);
}

/**
 * 89.915
 * Argument <argNumber> could not be converted to type <msg>
 *
 * @param argNumber
 * @param msg
 */
inline void badArgException(RexxMethodContext *context, int argNumber, char * msg)
{
    context->RaiseException2(Rexx_Error_Invalid_argument_argType,
                             context->NewInteger(argNumber),
                             context->NewStringFromAsciiz(msg));
}

/**
 * 88.918
 * Argument <argument> is not in a valid format; found "<value>"
 *
 * @param argNumber
 * @param rxActual
 */
inline void wrongFormatException(RexxMethodContext *c, int argNumber, RexxObjectPtr rxActual)
{
    c->RaiseException2(Rexx_Error_Invalid_argument_format, c->NewInteger(argNumber), rxActual);
}
inline void wrongFormatException(RexxMethodContext *c, int argNumber, char * actual)
{
    wrongFormatException(c, argNumber, c->NewStringFromAsciiz(actual));
}

inline RexxObjectPtr hrToRx(RexxMethodContext *c, HRESULT hr)
{
    TCHAR buffer[32];

    _snprintf(buffer, 32, "0x%08x", hr);
    return c->NewStringFromAsciiz(buffer);
}

inline void *shellAlloc(size_t size)
{
  void  *p = NULL;

  if ( size )
  {
    p = calloc(size, 1);
  }
  return p;
}

inline void shellFree(void *p)
{
  if ( p )
  {
      free(p);
  }
}

#endif // not defined WinShell.hpp
