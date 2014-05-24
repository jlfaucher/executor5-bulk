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

#ifndef ooConsole_Included
#define ooConsole_Included

#ifdef _WIN32
    #define NEED_DLL_MAIN
    #include "winOS.hpp"
    #undef  NEED_DLL_MAIN
#else
    #include "unixOS.hpp"
#endif

#include "oorexxapi.h"

#define VALID_VERSION_TYPES       "[O]neLine [F]ull [C]ompact [L]ibVersion [N]umber [S]ourceID"
#define NO_LOCAL_ENVIRONMENT_MSG  "the .local environment was not found"

// MSDN docs say the maximum for a console title is 64 K.  But, passing a length
// of 65536 into the title functions results in an error.  So does 65500, and
// 65000.  60000 works.  But, we make it 4000 and in the implmentation, if it
// fails, we just fail it.
#define MAX_CONSOLETITLE             4000

// In a similar fashion, the GetConsoleProcessList() function fails if we pass
// in an array of 64K uint32_t types.  But, if we pass in 64K / 4 uint32_t types
// it works.  1000 PIDs seems more than adequate.
#define MAX_CONSOLEPIDS              1000

// Similar thing for the buffer for ReadConsole.  Trial and error shows that
// 31366 works and 31367 fails
#define MAX_READBUFFER               31366

// Trial and error, 3136 works, 3137 fails
#define MAX_INPUTRECORDS             3136

static bool _isVersion(DWORD major, DWORD minor, unsigned int sp, unsigned int type, unsigned int condition);

// Enum for a Windows OS, don't need many right now.
typedef enum
{
    XP_OS, Vista_OS, Windows7_OS
} os_name_t;

inline bool _isAtLeastXP(void)
{
    return _isVersion(5, 1, 2, 0, VER_GREATER_EQUAL);
}

inline bool _isAtLeastVista(void)
{
    return _isVersion(6, 0, 0, 0, VER_GREATER_EQUAL);
}

inline bool _isAtLeastWindows7(void)
{
    return _isVersion(6, 1, 0, 0, VER_GREATER_EQUAL);
}

// Enum for the type of console
typedef enum
{
    STDINPUT, STDOUTPUT, STDERROR
} console_type_t;

/* Struct for the ooConsole object CSelf. */
typedef struct _ooConsoleCSelf {
    HANDLE          handle;
    console_type_t  type;
    uint32_t        errRC;        // Error code
    bool            isValid;      // Is a valid object
    bool            isCreated;    // Is a created screen buffer
} CooConsole;
typedef CooConsole *pCooConsole;



#endif  /* ooConsole_Included */
