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

#ifndef SHELLUTILS_HPP
#define SHELLUTILS_HPP

#pragma data_seg("shared_data")
static long globalInstances = 0;
#pragma data_seg()

static long threadInstances = 0;
static HINSTANCE thisModule = NULL;
static bool fileIconInitDone = false;

#define SHELL_DLL "ShellUtils.dll"

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
#define PPSF_REQUIRED             0x0008
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

/**
 * Tests if an ooRexx object is an array object.
 *
 * @param rxObject  The object to test.
 *
 * @return true if the object is an instance of an .array, otherwise false.
 */
inline bool is_array(RexxObject *rxObject)
{
  if ( rxObject != NULLOBJECT &&
       (RexxSend1(rxObject, "HASMETHOD", RexxString("DIMENSION")) == RexxTrue) &&
       (RexxSend1(rxObject, "HASMETHOD", RexxString("HASINDEX")) == RexxTrue))
    return true;
  else
    return false;
}

/**
 * Returns an upper-cased ascii-string copy of a ooRexx string.  The caller is
 * responsible for freeing memory.
 *
 * @param rxStr   The string to copy and upper-case.
 */
inline char *getUpperCase(STRING rxStr)
{
    char *s = _strdup(string_data(rxStr));
    return _strupr(s);
}

inline void systemServiceException(char *msg, const char *sub)
{
    if ( sub != NULL )
    {
        TCHAR buffer[128];
        _snprintf(buffer, sizeof(buffer), msg, sub);
        send_exception1(Error_System_service_user_defined, RexxArray1(RexxString(buffer)));
    }
    else
    {
        send_exception1(Error_System_service_user_defined, RexxArray1(RexxString(msg)));
    }
}

inline void systemServiceExceptionCode(const char *msg, const char *arg1, int code)
{
    TCHAR buffer[256];
    _snprintf(buffer, sizeof(buffer), msg, arg1, code);
    send_exception1(Error_System_service_user_defined, RexxArray1(RexxString(buffer)));
}

/**
 * 88.901
 * Missing argument; argument <argument> is required
 *
 * Missing argument; argument 2 is required
 *
 * @param position
 */
inline void missingArgException(size_t position)
{
    send_exception1(Error_Invalid_argument_noarg, RexxArray1(RexxInteger(position)));
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
inline void stringToLongException(int argNumber, int max, int actual)
{
    TCHAR buffer[32];
    REXXOBJECT array = RexxArray(3);

    _snprintf(buffer, sizeof(buffer), "%d characters in length", max);

    array_put(array, RexxInteger(argNumber), 1);
    array_put(array, RexxString(buffer), 2);
    array_put(array, RexxInteger(actual), 3);

    send_exception1(Error_Invalid_argument_toobig, array);
}

/**
 * 88.914
 * Argument <argument> must be of the <class> class
 *
 * Argument 2 must be of the String class
 *
 * @param position
 */
inline void notAStringException(size_t position)
{
    send_exception1(Error_Invalid_argument_noclass,
                    RexxArray2(RexxInteger(position), RexxString("String")));
}

/**
 * 89.915
 * Argument <argNumber> could not be converted to type <msg>
 *
 * @param argNumber
 * @param msg
 */
inline void badArgException(int argNumber, char * msg)
{
    send_exception1(Error_Invalid_argument_argType,
                    RexxArray2(RexxInteger(argNumber), RexxString(msg)));
}

/**
 * 88.916
 * Argument <argument> must be one of <values>; found "<value>"
 *
 * Argument 1 must be one of the valid CSIDL_XXX constants; found "dog"
 *
 * @param argNumber
 * @param acceptable
 * @param actual
 */
inline void invalidConstantException(int argNumber, char *msg, const char *sub, char *actual)
{
    TCHAR buffer[32];
    REXXOBJECT array = RexxArray(3);

    _snprintf(buffer, sizeof(buffer), msg, sub);

    array_put(array, RexxInteger(argNumber), 1);
    array_put(array, RexxString(buffer), 2);
    array_put(array, RexxString(actual), 3);

    send_exception1(Error_Invalid_argument_list, array);
}

/**
 * 88.916
 * Argument <argument> must be one of <values>; found "<value>"
 *
 * Argument 1 must be one of the valid CSIDL_XXX constants; found "dog"
 *
 * @param argNumber
 * @param acceptable
 * @param actual
 */
inline void invalidConstantExceptionRx(int argNumber, char *msg, const char *sub, REXXOBJECT actual)
{
    TCHAR buffer[32];
    REXXOBJECT array = RexxArray(3);

    _snprintf(buffer, sizeof(buffer), msg, sub);

    array_put(array, RexxInteger(argNumber), 1);
    array_put(array, RexxString(buffer), 2);
    array_put(array, actual, 3);

    send_exception1(Error_Invalid_argument_list, array);
}

/**
 * 88.916
 * Argument <argument> must be one of <values>; found "<value>"
 *
 * Argument 1 must be one of array of icon handles or count of icons; found "o"
 *
 * @param argNumber
 * @param acceptable
 * @param actual
 */
inline void wrongArgExceptionRx(int argNumber, char * acceptable, REXXOBJECT actual)
{
    REXXOBJECT array = RexxArray(3);
    array_put(array, RexxInteger(argNumber), 1);
    array_put(array, RexxString(acceptable), 2);
    array_put(array, actual, 3);

    send_exception1(Error_Invalid_argument_list, array);
}

/**
 * 88.918
 * Argument <argument> is not in a valid format; found "<value>"
 *
 * @param argNumber
 * @param rxActual
 */
inline void wrongFormatException(int argNumber, REXXOBJECT rxActual)
{
    send_exception1(Error_Invalid_argument_format,
                    RexxArray2(RexxInteger(argNumber), rxActual));
}


inline void requiredArg(REXXOBJECT rxObj, size_t argNumber)
{
    if ( rxObj == NULLOBJECT )
    {
        missingArgException(argNumber);
    }
}

inline char * requiredStringArg(REXXOBJECT rxObj, size_t argNumber)
{
    if ( rxObj == NULLOBJECT )
    {
        missingArgException(argNumber);
    }
    if ( ! _isstring(rxObj) )
    {
        notAStringException(argNumber);
    }
    return string_data(rxObj);
}

inline char * requiredStringArgUpper(REXXOBJECT rxObj, size_t argNumber)
{
    if ( rxObj == NULLOBJECT )
    {
        missingArgException(argNumber);
    }
    if ( ! _isstring(rxObj) )
    {
        notAStringException(argNumber);
    }
    return getUpperCase(rxObj);
}

inline size_t getPositive(REXXOBJECT rxInt, size_t argNumber)
{
    int arg;

    if ( ! _isinteger(rxInt) )
    {
        send_exception1(Error_Invalid_argument_number,
                        RexxArray2(RexxInteger(argNumber), rxInt));
    }

    arg = integer_value(rxInt);
    if ( arg < 1 )
    {
        send_exception1(Error_Invalid_argument_positive,
                        RexxArray2(RexxInteger(argNumber), rxInt));
    }
    return (size_t)arg;
}

inline size_t requiredPositiveArg(REXXOBJECT rxInt, size_t argNumber)
{
    if ( rxInt == NULLOBJECT )
    {
        missingArgException(argNumber);
    }
    return getPositive(rxInt, argNumber);
}

inline size_t optionalPositiveArg(REXXOBJECT rxInt, size_t defaultValue, size_t argNumber)
{
    if ( rxInt == NULLOBJECT )
    {
        return defaultValue;
    }
    return getPositive(rxInt, argNumber);
}

inline bool getBool(REXXOBJECT rxObj, size_t argNumber)
{
    if ( rxObj != RexxTrue && rxObj != RexxFalse )
    {
        wrongArgExceptionRx(argNumber, "true or false", rxObj);
    }
    return (rxObj == RexxTrue ? true : false);
}

inline bool requiredBoolArg(REXXOBJECT rxObj, size_t argNumber)
{
    if ( rxObj == NULLOBJECT )
    {
        missingArgException(argNumber);
    }
    return getBool(rxObj, argNumber);
}

inline bool optionalBoolArg(REXXOBJECT rxObj, bool defaultValue, size_t argNumber)
{
    if ( rxObj == NULLOBJECT )
    {
        return defaultValue;
    }
    return getBool(rxObj, argNumber);
}


inline STRING ptrToRx(void * ptr)
{
    TCHAR buffer[64];
    _snprintf(buffer, sizeof(buffer), "%p", ptr);
    return RexxString(buffer);
}

inline DWORD_PTR ptrFromRx(REXXOBJECT value)
{
    DWORD_PTR var = 0;
    if ( sscanf(string_data((RexxString *)value), "%p", &var) == 1 )
    {
        return var;
    }
    return 0;
}

/**
 * A temporary solution.  In ooDialog and WinSystm.cls Windows HANDLE types,
 * HWND, HMODULE, etc are returned to ooRexx as longs.  To be compatible with
 * 3.2.0 window handles are passed back and forth as longs.  For 3.x, this will
 * be fixed.
 */
inline HANDLE handleFromRx(STRING value)
{
    return (HANDLE)atol(string_data(value));
}

inline STRING handleToRx(HANDLE h)
{
    TCHAR buffer[64];
    _snprintf(buffer, sizeof(buffer), "%d", h);
    return RexxString(buffer);
}

inline STRING hrToRx(HRESULT hr)
{
    TCHAR buffer[32];

    _snprintf(buffer, 32, "0x%08x", hr);
    return RexxString(buffer);
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

#endif // not defined ShellUtils.hpp
