/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 1995, 2004 IBM Corporation. All rights reserved.             */
/* Copyright (c) 2005-2019 Rexx Language Association. All rights reserved.    */
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
/******************************************************************************/
/* REXX Windows Support                                          rexxutil.c   */
/*                                                                            */
/* Windows system utility functions                                           */
/*                                                                            */
/******************************************************************************/
/**********************************************************************
*                                                                     *
*   This program extends the REXX language by providing many          *
*   REXX external functions.                                          *
*   These functions are:                                              *
*       SysCls              -- Clear the screen in an OS/2 fullscreen *
*                              or windowed command prompt session.    *
*       SysCurPos           -- Set and/or Query the cursor position   *
*                              in an OS/2 fullscreen or windowed      *
*                              command prompt session.                *
*       SysCurState         -- Make the cursor visible or invisible   *
*                              in an OS/2 fullscreen or windowed      *
*                              command prompt session.                *
*       SysDriveInfo        -- Returns information about a specific   *
*                              drive.                                 *
*       SysDriveMap         -- Returns a list of the drives on the    *
*                              machine                                *
*       SysDropFuncs        -- Makes all functions in this package    *
*                              unknown to REXX.                       *
*       SysFileDelete       -- Deletes a file                         *
*       SysFileSearch       -- Searches for a file matching a given   *
*                              filespec.                              *
*       SysFileTree         -- Searches for files matching a given    *
*                              filespec, including files in           *
*                              subdirectories.                        *
*       SysGetKey           -- Returns one by of data indicating the  *
*                              key which was pressed,                 *
*                              in an OS/2 fullscreen or windowed      *
*                              command prompt session.                *
*       SysIni              -- Reads and/or updates entries in .INI   *
*                              files.                                 *
*       SysLoadFuncs        -- Makes all functions in this package    *
*                              known to REXX so REXX programs may     *
*                              call them.                             *
*       SysMkDir            -- Creates a directory                    *
*       SysWinVer           -- Returns the Win OS and Version number  *
*       SysVersion          -- Returns the OS and Version number      *
*       SysRmDir            -- Removes a directory                    *
*       SysSearchPath       -- Searches throught a specified path     *
*                              for a file.                            *
*       SysSleep            -- Suspends execution for a number of     *
*                              seconds and milliseconds.              *
*       SysTempFilename     -- Creates a unique filename              *
*       SysTextScreenRead   -- Reads characters from the screen,      *
*                              in an OS/2 fullscreen or windowed      *
*                              command prompt session.                *
*       SysTextScreenSize   -- Returns the size of the window in      *
*                              rows and columns,                      *
*                              in an OS/2 fullscreen or windowed      *
*                              command prompt session.                *
*       SysWaitNamedPipe    -- Wait on a named pipe.                  *
*       SysRegisterObjectClass -- Register a new object class         *
*       SysDeregisterObjectClass -- Remove class registration         *
*       SysQueryClassList   -- Get list of registered classes         *
*       SysCreateObject     -- Create an object instance              *
*       SysDestroyObject    -- Delete an object instance              *
*       SysSetObjectData    -- Change object settings data            *
*       SysBootDrive        -- Return the windows boot drive          *
*       SysSystemDirectory  -- Return the Windows system directory    *
*       SysQueryEAList      -- Return list of file EA names           *
*       SysWildCard         -- Perform file wild card editting        *
*       SysFileSystemType   -- Return drive file system type          *
*       SysVolumeLabel      -- Return the drive label                 *
*       SysAddFileHandle    -- Add file handles to a process          *
*       SysSetFileHandle    -- Set file handles for a process         *
*       SysCreateMutexSem   -- Create a Mutex semaphore               *
*       SysOpenMutexSem     -- Open a Mutex semaphore                 *
*       SysCloseMutexSem    -- Close a Mutex semaphore                *
*       SysRequestMutexSem  -- Request a Mutex semaphore              *
*       SysReleaseMutexSem  -- Release a Mutex semaphore              *
*       SysCreateEventSem   -- Create an Event semaphore              *
*       SysOpenEventSem     -- Open an Event semaphore                *
*       SysCloseEventSem    -- Close an Event semaphore               *
*       SysPostEventSem     -- Post an Event semaphore                *
*       SysPulseEventSem    -- Post and reset an Event semaphore      *
*       SysResetEventSem    -- Reset an Event semaphore               *
*       SysWaitEventSem     -- Wait on an Event semaphore             *
*       SysProcessType      -- Return type of process                 *
*       SysSetPriority      -- Set current thread priority            *
*       SysGetCollate       -- Get country/codepage collating sequence*
*       SysNationalLanguageCompare -- NLS strict compare              *
*       SysMapCase          -- NLS uppercasing                        *
*       SysSetProcessCodePage -- Set current code page                *
*       SysQueryProcessCodePage -- Get current code page              *
*       SysAddRexxMacro     -- Load program into macro space          *
*       SysDropRexxMacro    -- Drop program from macro space          *
*       SysReorderRexxMacro -- Reorder program in macro space         *
*       SysQueryRexxMacro   -- Query ordering of macro space program  *
*       SysClearRexxMacroSpace -- Remove all programs from macro space*
*       SysLoadRexxMacroSpace  -- Load a Rexx macro space             *
*       SysSaveRexxMacroSpace  -- Save a Rexx macro space             *
*       SysShutDownSystem   -- Shutdown the system                    *
*       SysSwitchSession    -- Switch to a named session              *
*       SysDropLibrary      -- Drop a function package                *
*       SysQueryProcess     -- Get information on current proc/thread *
*       SysDumpVariables    -- Dump current variables to a file       *
*       SysSetFileDateTime  -- Set the last modified date of a file   *
*       SysGetFileDateTime  -- Get the last modified date of a file   *
*       SysStemSort         -- sort a stem array                      *
*       SysStemDelete       -- delete items in a stem array           *
*       SysStemInsert       -- insert items into a stem array         *
*       SysStemCopy         -- copy items from one stem array to other*
*       SysUtilVersion      -- query version of REXXUTIL.DLL          *
*       SysWinFileEncrypt   -- Encrypt file on a W2K-NTFS             *
*       SysWinFileDecrypt   -- Decrypt file on a W2K-NTFS             *
*       SysGetErrortext     -- Retrieve textual desc. of error number *
*       SysWinGetDefaultPrinter -- retrieve default printer           *
*       SysWinGetPrinters   -- Obtain list of local printers          *
*       SysWinSetDefaultPrinter -- set the local default printer      *
*       SysFileCopy         -- Copy files on the file system          *
*       SysFileMove         -- Move / Rename files or directories     *
*       SysIsFile           -- Check for the existance of a file      *
*       SysIsFileDirectory  -- Check for the existance of a directory *
*       SysIsFileLink       -- Check for the existance of a link      *
*       SysIsFileCompressed   -- Check for a file to be compressed    *
*       SysIsFileEncrypted    -- Check for a file to be encrypted     *
*       SysIsFileNotContentIndexed -- Check if a file should be indexed *
*       SysIsFileOffline    -- Check if a file is offline             *
*       SysIsFileSparse     -- Check if a file is sparse              *
*       SysIsFileTemporary  -- Check if a file is temporary           *
*                                                                     *
**********************************************************************/

/* Include files */

#include "oorexxapi.h"
#include <memory.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <limits.h>
#include <shlwapi.h>
#include <math.h>                      // isnan(), HUGE_VAL
#include <versionhelpers.h>

#define OM_WAKEUP (WM_USER+10)
VOID CALLBACK SleepTimerProc( HWND, UINT, UINT, DWORD);

/*********************************************************************/
/*  Various definitions used by various functions.                   */
/*********************************************************************/

#define MAX_LABEL      13              /* max label length (sdrvinfo)*/
#define MAX_DIGITS     9               /* max digits in numeric arg  */
#define MAX            264             /* temporary buffer length    */
#define IBUF_LEN       4096            /* Input buffer length        */
#define MAX_READ       0x10000         /* full segment of buffer     */
#define CH_EOF         0x1A            /* end of file marker         */
#define CH_CR          '\r'            /* carriage return character  */
#define CH_NL          '\n'            /* new line character         */
#define AllocFlag      PAG_COMMIT | PAG_WRITE  /* for DosAllocMem    */
#define RNDFACTOR      1664525L
#define MAX_ENVVAR     1024
#define MAX_LINE_LEN   4096            /* max line length            */
#define MAX_CREATEPROCESS_CMDLINE (32 * 1024)

/*********************************************************************/
/*  Defines used by SysDriveMap                                      */
/*********************************************************************/

#define  USED           0
#define  FREE           1
#define  CDROM          2
#define  REMOTE         3
#define  LOCAL          4
#define  RAMDISK        5
#define  REMOVABLE      6

/*********************************************************************/
/* Defines uses by SysTree                                           */
/*********************************************************************/

#define  RECURSE        0x0002
#define  DO_DIRS        0x0004
#define  DO_FILES       0x0008
#define  NAME_ONLY      0x0010
#define  EDITABLE_TIME  0x0020
#define  LONG_TIME      0x0040   /* long time format for SysFileTree */
#define  CASELESS       0x0080
#define  RXIGNORE       2              /* Ignore attributes entirely */
#define  AllAtts        FILE_NORMAL | FILE_READONLY | FILE_HIDDEN | \
FILE_SYSTEM | FILE_DIRECTORY | FILE_ARCHIVED
#define  AllFiles       FILE_NORMAL | FILE_READONLY | FILE_HIDDEN | \
FILE_SYSTEM | FILE_ARCHIVED
#define  AllDirs        FILE_READONLY | FILE_HIDDEN | \
FILE_SYSTEM | FILE_ARCHIVED | MUST_HAVE_DIRECTORY | FILE_DIRECTORY

/*********************************************************************/
/* Define used for Unicode translation. Not present in early Windows */
/* SDKs.                                                             */
/*********************************************************************/
#ifndef WC_ERR_INVALID_CHARS
#define WC_ERR_INVALID_CHARS      0x00000080
#endif

// Defines for various SysFileTree buffer.
#define FNAMESPEC_BUF_EXTRA    8
#define FNAMESPEC_BUF_LEN      MAX_PATH + FNAMESPEC_BUF_EXTRA
#define FOUNDFILE_BUF_LEN      MAX_PATH
#define FILETIME_BUF_LEN       64
#define FILEATTR_BUF_LEN       16
#define FOUNDFILELINE_BUF_LEN  FOUNDFILE_BUF_LEN + FILETIME_BUF_LEN + FILEATTR_BUF_LEN


/*********************************************************************/
/* Structures used throughout REXXUTIL.C                             */
/*********************************************************************/

/*
 *  Data structure for SysFileTree.
 *
 *  Note that in Windows the MAX_PATH define includes the terminating null.
 */
typedef struct RxTreeData {
    size_t         count;                         // Number of found file lines
    RexxStemObject files;                         // Stem that holds results.
    char           fNameSpec[FNAMESPEC_BUF_LEN];  // File name portion of the search for file spec, may contain glob characters.
    char           foundFile[FOUNDFILE_BUF_LEN];  // Full path name of found file
    char           fileTime[FILETIME_BUF_LEN];    // Time and size of found file
    char           fileAttr[FILEATTR_BUF_LEN];    // File attribute string of found file
    char           foundFileLine[FOUNDFILELINE_BUF_LEN]; // Buffer for found file line, includes foundFile, fileTime, and fileAttr
    char          *dFNameSpec;                    // Starts out pointing at fNameSpec
    size_t         nFNameSpec;                    // CouNt of bytes in dFNameSpec buffer
} RXTREEDATA;

/*********************************************************************/
/* RxStemData                                                        */
/*   A class for manipulating repetive stem sets                     */
/*   stem variable.                                                  */
/*********************************************************************/

class RxStemData
{
    RxStemData(RexxStemObject s) : stem(s), count(0) { }

    RexxStemObject stem;               // the stem to process
    size_t count;                      // last set value
}


/*********************************************************************/
/* Saved character status                                            */
/*********************************************************************/
static   int   ExtendedFlag = 0;       /* extended character saved   */
static   char  ExtendedChar;           /* saved extended character   */

/*********************************************************************/
/* Numeric Error Return Strings                                      */
/*********************************************************************/

#define  NO_UTIL_ERROR    "0"          /* No error whatsoever        */
#define  ERROR_NOMEM      "2"          /* Insufficient memory        */
#define  ERROR_FILEOPEN   "3"          /* Error opening text file    */

/*********************************************************************/
/* Alpha Numeric Return Strings                                      */
/*********************************************************************/

#define  ERROR_RETSTR   "ERROR:"

/*********************************************************************/
/* Numeric Return calls                                              */
/*********************************************************************/

#define  INVALID_ROUTINE 40            /* Raise Rexx error           */
#define  VALID_ROUTINE    0            /* Successful completion      */

/*********************************************************************/
/* Some useful macros                                                */
/*********************************************************************/

#define BUILDRXSTRING(t, s) { \
  strcpy((t)->strptr,(s));\
  (t)->strlength = strlen((s)); \
}

#define RETVAL(retc) { \
  retstr->strlength = strlen(itoa(retc, retstr->strptr,10)); \
  return VALID_ROUTINE; \
}

/*********************************************************************/
/****************  REXXUTIL Supporting Functions  ********************/
/****************  REXXUTIL Supporting Functions  ********************/
/****************  REXXUTIL Supporting Functions  ********************/
/*********************************************************************/

void inline outOfMemoryException(RexxThreadContext *c)
{
    c->RaiseException1(Rexx_Error_System_service_user_defined, c->String("failed to allocate memory"));
}

/**
 * <routineName> argument <argPos> must not be a null string
 *
 * SysFileTree argument 2 must not be a null string
 *
 * @param c      Threade context we are operating in.
 * @param fName  Routine name.
 * @param pos    Argument position.
 */
void inline nullStringException(RexxThreadContext *c, CSTRING fName, size_t pos)
{
    c->RaiseException2(Rexx_Error_Incorrect_call_null, c->String(fName), c->StringSize(pos));
}

inline void safeLocalFree(void *p)
{
    if (p != NULL)
    {
        LocalFree(p);
    }
}

/**
 * Raises an exception for an unrecoverable system API failure.
 *
 * @param c    Call context we are operating in.
 * @param api  System API name.
 * @param rc   Return code from calling the API.
 */
static void systemServiceExceptionCode(RexxThreadContext *c, CSTRING api, uint32_t rc)
{
    char buf[256] = {0};
    _snprintf(buf, sizeof(buf),
             "system API %s() failed; rc: %d last error code: %d", api, rc, GetLastError());

    c->RaiseException1(Rexx_Error_System_service_user_defined, c->String(buf));
}

/**
 * Tests if the the current operating system version meets the specified
 * requirements. Really a front end to VerifyVersionInfo().  See MSDN docs for
 * type and condition flags.
 *
 * @param major       OS major number.
 * @param minor       OS minor number.
 * @param sp          Service pack level.
 * @param type        Further refines the test.  See MSDN for all the flags, but
 *                    for example there is VER_NT_WORKSTATION to differentiate
 *                    between NT desktop and NT server.
 * @param condition   The test condition.  Typical flags would be VER_EQUAL or
 *                    VER_GREATER_EQUAL.
 *
 * @return True if the condition is met by the current operating system, or
 *         false if not.
 */
static bool isWindowsVersion(DWORD major, DWORD minor, unsigned int sp, unsigned int type, unsigned int condition)
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
    {
        return true;
    }
    else
    {
        return false;
    }
}

inline bool isAtLeastVista(void)
{
    return isWindowsVersion(6, 0, 0, 0, VER_GREATER_EQUAL);
}


/********************************************************************
* Function:  SetFileMode(file, attributes)                          *
*                                                                   *
* Purpose:   Change file attribute bits                             *
*            without PM.                                            *
*                                                                   *
* RC:        0    -  File attributes successfully changed           *
*            1    -  Unable to change attributes                    *
*********************************************************************/
bool SetFileMode(
  const char *file,                    /* file name                  */
  size_t   attr )                      /* new file attributes        */
{

  DWORD         dwfileattrib;          /* file attributes            */

                                       /* get the file status        */
  if ((dwfileattrib = GetFileAttributes(file)) != 0xffffffff) {
                                       /* if worked                  */
                                       /* set the attributes         */
    if ((dwfileattrib = SetFileAttributes(file, (DWORD)attr)) != 0)
      return false;   /* give back success flag     */
    else
      return true;
  } else
    return true;
}


/****************************************************************
* Function: GetUniqueFileName(Template, Filler, file)           *
*                                                               *
* Purpose:  This function returns a unique temporary file name  *
*           given a template and a filler character.            *
*                                                               *
* Params:   CHAR* Template - The template.  Must contain at     *
*                            least one or more filler chars.    *
*                                                               *
*                            Example:  "C:\TEMP\FILE????.DAT    *
*                                                               *
*           CHAR Filler    - The character in the Template to   *
*                            be replaced with numbers.  For     *
*                            the above example, the filler char *
*                            would be '?'.                      *
*           CHAR* file     - file name produced (output)        *
*                                                               *
* Used By:  RxTempFileName()                                    *
****************************************************************/

VOID GetUniqueFileName(
  CHAR  *Template,
  CHAR   Filler,
  CHAR  *file)
{

  CHAR numstr[6];
  bool Unique = false;

  ULONG x,                             /* loop index                 */
        i,                             /*                            */
        j = 0,                         /* number of filler chars     */
                                       /* found                      */
        num,                           /* temporary random number    */
        start,                         /* first random number        */
        max = 1;                       /* maximum random number      */

  INT  seed;                           /* to get current time        */
  WIN32_FIND_DATA wfdFinfo;            /* Windows Find data struct   */
                                       /* Structure                  */
  SYSTEMTIME DT;                       /* The date and time structure*/
  UINT            fuErrorMode;         /* holds current file err mode*/
  HANDLE hSearch;                      /* handle of file if found    */

 /** Determine number of filler characters *                         */

  for (x = 0; Template[x] != 0; x++)

    if (Template[x] == Filler) {
      max = max *10;
      j++;
    }

 /** Return NULL string if less than 1 or greater than 4 *           */

  if (j == 0 || j > 5) {
    Unique = true;
    strcpy(file, "");
    return;
  }

 /** Get a random number in the appropriate range                    */

                                       /* Get the time               */
  GetSystemTime(&DT);                  /* via Windows                */

  seed = DT.wHour*60 + DT.wMinute;     /* convert to hundreths       */
  seed = seed*60 + DT.wSecond;
  seed = seed*100 + ( DT.wMilliseconds / (UINT)10 );
  seed = seed * RNDFACTOR + 1;
  num = (ULONG)seed % max;
  start = num;

 /** Do until a unique name is found                                 */

  while (!Unique) {

    /** Generate string which represents the number                  */

    switch (j) {
      case 1 :
        wsprintf(numstr, "%01u", num);
        break;
      case 2 :
        wsprintf(numstr, "%02u", num);
        break;
      case 3 :
        wsprintf(numstr, "%03u", num);
        break;
      case 4 :
        wsprintf(numstr, "%04u", num);
        break;
      case 5 :
        wsprintf(numstr, "%05u", num);
        break;
    }

    /** Subsitute filler characters with numeric string              */

    i = 0;

    for (x = 0; Template[x] != 0; x++)

      if (Template[x] == Filler)
        file[x] = numstr[i++];

      else
        file[x] = Template[x];
    file[x] = '\0';

    /** See if the file exists                                       */
                                       /* Disable Hard-Error popups  */
    fuErrorMode = SetErrorMode(SEM_NOOPENFILEERRORBOX);
    hSearch = FindFirstFile(file, &wfdFinfo);

    if (hSearch == INVALID_HANDLE_VALUE)/* file not found?           */
      Unique = true;                   /* got one                    */

    FindClose(hSearch);
    SetErrorMode(fuErrorMode);         /* Enable previous setting    */

    /** Make sure we are not wasting our time                        */

    num = (num+1)%max;

    if (num == start && !Unique) {
      Unique = true;
      strcpy(file, "");
    }
  }
}

/**********************************************************************
***             <<<<<< REXXUTIL Functions Follow >>>>>>>            ***
***             <<<<<< REXXUTIL Functions Follow >>>>>>>            ***
***             <<<<<< REXXUTIL Functions Follow >>>>>>>            ***
***             <<<<<< REXXUTIL Functions Follow >>>>>>>            ***
**********************************************************************/
/**********************************************************************
* Function:  SysCls                                                   *
*                                                                     *
* Syntax:    call SysCls                                              *
*                                                                     *
* Return:    NO_UTIL_ERROR - Successful.                              *
**********************************************************************/

RexxRoutine0(int, SysCls)
{
    DWORD dummy;
    COORD Home = { 0, 0 };                 /* Home coordinates on console*/
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo; /* Console information        */

    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    /* if in character mode       */
    if (GetConsoleScreenBufferInfo(hStdout, &csbiInfo))
    {
        FillConsoleOutputCharacter(hStdout, ' ',
                                   csbiInfo.dwSize.X * csbiInfo.dwSize.Y,
                                   Home, &dummy);
        SetConsoleCursorPosition(hStdout, Home);
    }

    return 0;
}

/*************************************************************************
* Function:  SysCurPos - positions cursor in DOS window                  *
*                                                                        *
* Syntax:    call SysCurPos [row, col]                                   *
*                                                                        *
* Params:    row   - row to place cursor on                              *
*            col   - column to place cursor on                           *
*                                                                        *
* Return:    row, col                                                    *
*************************************************************************/

RexxRoutine2(RexxStringObject, SysCurPos, OPTIONAL_stringsize_t, inrow, OPTIONAL_stringsize_t, incol)
{
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo; /* Console information        */


    if ((argumentExists(1) && argumentOmitted(2)) || (argumentExists(2) && argumentOmitted(1)))
    {
        context->InvalidRoutine();
        return NULL;
    }
    /* get handle to stdout       */
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

    /* get current position, and  */
    /* continue only if in        */
    /* character mode             */
    if (GetConsoleScreenBufferInfo(hStdout, &csbiInfo))
    {
        char buffer[256];

        sprintf(buffer, "%d %d", csbiInfo.dwCursorPosition.Y, csbiInfo.dwCursorPosition.X);

        if (argumentExists(2))
        {
            COORD newHome;                       /* Position to move cursor    */
            newHome.Y = (short)inrow;      /* convert to short form      */
            newHome.X = (short)incol;      /* convert to short form      */
                                           /* Set the cursor position    */
            SetConsoleCursorPosition(hStdout, newHome);
        }

        return context->NewStringFromAsciiz(buffer);
    }

    return context->NullString();
}

/*************************************************************************
* Function:  SysCurState                                                 *
*                                                                        *
* Syntax:    call SysCurState state                                      *
*                                                                        *
* Params:    state - Either 'ON' or 'OFF'.                               *
*                                                                        *
* Return:    NO_UTIL_ERROR - Successful.                                 *
*************************************************************************/

RexxRoutine1(int, SysCurState, CSTRING, option)
{
    CONSOLE_CURSOR_INFO CursorInfo;      /* info about cursor          */
                                         /* get handle to stdout       */
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    /* Get the cursor info        */
    GetConsoleCursorInfo(hStdout,&CursorInfo);

    /* Get state and validate     */
    if (stricmp(option, "ON") == 0)
    {
        CursorInfo.bVisible = true;
    }
    else if (stricmp(option, "OFF") == 0)
    {
        CursorInfo.bVisible = false;
    }
    else
    {
        // this is an error, raise the condition and return
        context->InvalidRoutine();
        return 0;
    }
    /* Set the cursor info        */
    SetConsoleCursorInfo(hStdout,&CursorInfo);
    return 0;                            /* no error on call           */
}


/*************************************************************************
* Function:  SysDriveInfo                                                *
*                                                                        *
* Syntax:    call SysDriveInfo drive                                     *
*                                                                        *
* Params:    drive - 'C', 'D', 'E', etc.                                 *
*                                                                        *
* Return:    disk free total label                                       *
*************************************************************************/

RexxRoutine1(RexxStringObject, SysDriveInfo, CSTRING, drive)
{
    CHAR   chFileSysType[MAX_PATH],      /*  File system name          */
        chVolumeName[MAX_PATH],       /*  volume label              */
        chDriveLetter[4];             /*  drive_letter + : + \ + \0 */
    BOOL   bGVIrc;                       /* rc from GVI                */

    /* GetDiskFreeSpace calculations */
    DWORD  dwSectorsPerCluster, dwBytesPerSector;
    DWORD  dwFreeClusters, dwClusters;
    BOOL   bGDFSrc;                      /* GDFS rc                    */
    UINT   errorMode;

    DWORD dwgle;
    uint64_t i64FreeBytesToCaller, i64TotalBytes, i64FreeBytes;

    size_t driveLength = strlen(drive);

    if (driveLength == 0 || driveLength > 2 || (driveLength == 2 && drive[1] != ';'))
    {
        context->InvalidRoutine();
        return NULLOBJECT;
    }

    // This must be a valid alphabetic character
    if (drive[0] < 'A' || drive[0] > 'z')
    {
        context->InvalidRoutine();
        return NULLOBJECT;
    }

    snprintf(chDriveLetter, sizeof(chDriveLetter), "%c:\\", drive[0]);

    errorMode = SetErrorMode(SEM_FAILCRITICALERRORS);
    /* get the volume name and file system type */
    bGVIrc = GetVolumeInformation(chDriveLetter,
                                  chVolumeName,
                                  (DWORD)MAX_PATH,
                                  NULL,
                                  NULL,
                                  NULL,
                                  chFileSysType,
                                  (DWORD)MAX_PATH);

    dwgle = GetLastError();

    /* use appropriate function */
    bGDFSrc = GetDiskFreeSpaceEx(chDriveLetter,
                                 (PULARGE_INTEGER)&i64FreeBytesToCaller,
                                 (PULARGE_INTEGER)&i64TotalBytes,
                                 (PULARGE_INTEGER)&i64FreeBytes);

    dwgle = GetLastError();
    SetErrorMode(errorMode);

    if (bGVIrc && bGDFSrc)
    {
        char retstr[256];
        /* use simplified display routine with 64 bit types */
        snprintf(retstr, sizeof(retstr),      // drive free total label
                 "%c%c  %-12I64u %-12I64u %-13s",
                 chDriveLetter[0], chDriveLetter[1],
                 i64FreeBytes, i64TotalBytes, chVolumeName);
        /* create return string       */
        return context->NewStringFromAsciiz(retstr);
    } else
    {
        return context->NullString();
    }
}

/*************************************************************************
* Function:  SysDriveMap                                                 *
*                                                                        *
* Syntax:    call SysDriveMap [drive] [,mode]                            *
*                                                                        *
* Params:    drive - 'C', 'D', 'E', etc.  The drive to start the search  *
*                     with.                                              *
*            mode  - Any of the following:  'FREE', 'USED', 'DETACHED',  *
*                                           'LOCAL', 'REMOTE'            *
*                                                                        *
* Return:    'A: B: C: D: ...'                                           *
*************************************************************************/

RexxRoutine2(RexxStringObject, SysDriveMap, CSTRING, drive, OPTIONAL_CSTRING, mode)
{
    CHAR     temp[MAX];                  /* Entire drive map built here*/

    CHAR     tmpstr[MAX];                /* Single drive entries built */
    /* here                       */
    CHAR     DeviceName[4];              /* Device name or drive letter*/
    /* string                     */
    DWORD    DriveMap;                   /* Drive map                  */
    ULONG    Ordinal;                    /* Ordinal of entry in name   */
    /* list                       */
    /* required                   */
    ULONG    dnum;                       /* Disk num variable          */
    ULONG    start = 3;                  /* Initial disk num           */
    ULONG    Mode = USED;                /* Query mode USED, FREE,     */
    /* LOCAL, etc                 */
    LONG     rc;                         /* OS/2 return codes          */
    UINT     errorMode;

    Ordinal = (ULONG)0;

    temp[0] = '\0';
    /* check starting drive letter*/
    size_t driveLength = strlen(drive);

    if (driveLength == 0 || driveLength > 2 || (driveLength == 2 && drive[1] != ';'))
    {
        context->InvalidRoutine();
        return NULLOBJECT;
    }
    // make sure this is in range
    ULONG start = toupper(drive[0]) - 'A' + 1;
    if (start < 1 || start > 26)
    {
        context->InvalidRoutine();
        return NULLOBJECT;
    }
    /* check the mode             */
    if (mode != NULL)
    {

        if (!stricmp(args[1].strptr, "FREE"))
        {
            Mode = FREE;
        } else if (!stricmp(args[1].strptr, "USED"))
        {
            Mode = USED;
        } else if (!stricmp(args[1].strptr, "RAMDISK"))
        {
            Mode = RAMDISK;
        } else if (!stricmp(args[1].strptr, "REMOTE"))
        {
            Mode = REMOTE;
        } else if (!stricmp(args[1].strptr, "LOCAL"))
        {
            Mode = LOCAL;
        } else if (!stricmp(args[1].strptr, "REMOVABLE"))
        {
            Mode = REMOVABLE;
        } else if (!stricmp(args[1].strptr, "CDROM"))
        {
            Mode = CDROM;
        } else
        {
            context->InvalidRoutine();
            return NULLOBJECT;
        }
    }
    /* perform the query          */
    DriveMap = GetLogicalDrives();
    DriveMap >>= start - 1;                  /* Shift to the first drive   */
    temp[0] = '\0';                      /* Clear temporary buffer     */

    for (dnum = start; dnum <= 26; dnum++)
    {

        /* Hey, we have a free drive  */
        if (!(DriveMap & (DWORD)1) && Mode == FREE)
        {
            snprintf(tmpstr, sizeof(tmpstr), "%c: ", dnum + 'A' - 1);
            strncat(temp, sizeof(temp), tmpstr);
        }
        /* Hey, we have a used drive  */
        else if ((DriveMap & (DWORD)1) && Mode == USED)
        {
            snprintf(tmpstr, sizeof(tmpstr), "%c: ", dnum + 'A' - 1);
            strncat(temp, sizeof(temp), tmpstr);
        } else if (DriveMap & (DWORD)1)
        {      /* Check specific drive info  */
            sprintf(DeviceName, "%c:\\", dnum + 'A' - 1);

            errorMode = SetErrorMode(SEM_FAILCRITICALERRORS);
            rc = (LONG)GetDriveType(DeviceName);
            SetErrorMode(errorMode);

            if (rc == DRIVE_REMOVABLE && Mode == REMOVABLE)
            {
                /* Hey, we have a removable   */
                /* drive                      */
                snprintf(tmpstr, sizeof(tmpstr), "%c: ", dnum + 'A' - 1);
                strncat(temp, sizeof(temp), tmpstr);
            } else if (rc == DRIVE_CDROM && Mode == CDROM)
            {
                snprintf(tmpstr, sizeof(tmpstr), "%c: ", dnum + 'A' - 1);
                strncat(temp, sizeof(temp), tmpstr);
            } else if (rc == DRIVE_FIXED && Mode == LOCAL)
            {
                snprintf(tmpstr, sizeof(tmpstr), "%c: ", dnum + 'A' - 1);
                strncat(temp, sizeof(temp), tmpstr);
            } else if (rc == DRIVE_REMOTE && Mode == REMOTE)
            {
                snprintf(tmpstr, sizeof(tmpstr), "%c: ", dnum + 'A' - 1);
                strncat(temp, sizeof(temp), tmpstr);
            } else if (rc == DRIVE_RAMDISK && Mode == RAMDISK)
            {
                snprintf(tmpstr, sizeof(tmpstr), "%c: ", dnum + 'A' - 1);
                strncat(temp, sizeof(temp), tmpstr);
            }
        }
        DriveMap >>= 1;                      /* Shift to the next drive    */
    }

    driveLength = strlen(temp);
    // if we are returning anything, then remove the last blank
    if (driveLength > 0)
    {
        driveLength--;
    }

    return context->NewString(temp, driveLength);
}



/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\
 *                                                                            *
 *   SysFileTree() implmentation and helper functions.                        *
 *                                                                            *
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/**
 * This is a SysFileTree specific function.
 *
 * @param c
 * @param pos
 * @param actual
 */
static void badSFTOptsException(RexxThreadContext *c, size_t pos, CSTRING actual)
{
    char buf[256] = {0};
    _snprintf(buf, sizeof(buf),
             "SysFileTree argument %zd must be a combination of F, D, B, S, T, L, I, or O; found \"%s\"",
             pos, actual);

    c->RaiseException1(Rexx_Error_Incorrect_call_user_defined, c->String(buf));
}

/**
 * This is a SysFile specific function.
 *
 * @param c
 * @param pos
 * @param actual
 */
static void badMaskException(RexxThreadContext *c, size_t pos, CSTRING actual)
{
    char buf[256] = {0};
    _snprintf(buf, sizeof(buf),
             "SysFileTree argument %zd must be 5 characters or less in length containing only '+', '-', or '*'; found \"%s\"",
             pos, actual);

    c->RaiseException1(Rexx_Error_Incorrect_call_user_defined, c->String(buf));
}

/**
 * Returns a value that is greater than 'need' by doubling 'have' until that
 * value is reached.
 */
inline size_t neededSize(size_t need, size_t have)
{
    while ( have < need )
    {
        have *= 2;
    }
    return have;
}

/**
 * Allocates a buffer that is at least twice as big as the buffer passed in.
 *
 * @param c             Call context we are operating in.
 * @param dBuf          Pointer to the buffer to reallocate
 * @param nBuf          Size of current dBuf buffer. Will be updated on return
 *                      to size of newly allocated buffer.
 * @param needed        Minimum size needed.
 * @param nStaticBuffer Size of original static buffer.
 *
 * @return True on success, false on memory allocation failure.
 *
 * @remarks  NOTE: that the pointer to the buffer to reallocate, may, or may
 *           not, be a pointer to a static buffer.  We must NOT try to free a
 *           static buffer and we MUST free a non-static buffer.
 */
static bool getBiggerBuffer(RexxCallContext *c, char **dBuf, size_t *nBuf, size_t needed, size_t nStaticBuffer)
{
    if ( *nBuf != nStaticBuffer )
    {
        LocalFree(*dBuf);
    }

    *nBuf = neededSize(needed, *nBuf);
    *dBuf = (char *)LocalAlloc(LPTR, *nBuf * sizeof(char));

    if ( *dBuf == NULL )
    {
        outOfMemoryException(c->threadContext);
        return false;
    }

    return true;
}

/**
 * Checks that attr is the same as that specified by the mask.
 *
 * @param mask
 * @param attr
 * @param options
 *
 * @return True for a match, otherwise false.
 */
static bool sameAttr(int32_t *mask, uint32_t attr, uint32_t options)
{
    if ( (options & DO_DIRS) && ! (options & DO_FILES) && ! (attr & FILE_ATTRIBUTE_DIRECTORY) )
    {
        return false;
    }
    if ( ! (options & DO_DIRS) && (options & DO_FILES) && (attr & FILE_ATTRIBUTE_DIRECTORY) )
    {
        return false;
    }
    if ( mask[0] == RXIGNORE )
    {
        return  true;
    }

    if ( mask[0] < 0 && attr & FILE_ATTRIBUTE_ARCHIVE )
    {
        return  false;
    }
    if ( mask[0] > 0 && ! (attr & FILE_ATTRIBUTE_ARCHIVE) )
    {
        return  false;
    }
    if ( mask[1] < 0 && attr & FILE_ATTRIBUTE_DIRECTORY )
    {
        return  false;
    }
    if ( mask[1] > 0 && ! (attr & FILE_ATTRIBUTE_DIRECTORY) )
    {
        return  false;
    }
    if ( mask[2] < 0 && attr & FILE_ATTRIBUTE_HIDDEN )
    {
        return  false;
    }
    if (mask[2] > 0 && ! (attr & FILE_ATTRIBUTE_HIDDEN) )
    {
        return  false;
    }
    if (mask[3] < 0 && attr & FILE_ATTRIBUTE_READONLY )
    {
        return  false;
    }
    if (mask[3] > 0 && ! (attr & FILE_ATTRIBUTE_READONLY) )
    {
        return  false;
    }
    if (mask[4] < 0 && attr & FILE_ATTRIBUTE_SYSTEM )
    {
        return  false;
    }
    if (mask[4] > 0 && ! (attr & FILE_ATTRIBUTE_SYSTEM) )
    {
        return  false;
    }

    return  true;
}

/**
 * Returns a new file attribute value given a mask of attributes to be changed
 * and the current attribute value.
 *
 * @param mask
 * @param attr
 *
 * @return New attribute value.
 */
static uint32_t newAttr(int32_t *mask, uint32_t attr)
{
    if ( mask[0] == RXIGNORE )
    {
      return  attr;
    }

    if ( mask[0] < 0 )
    {
        attr &= ~FILE_ATTRIBUTE_ARCHIVE;   // Clear
    }
    if ( mask[0] > 0 )
    {
        attr |= FILE_ATTRIBUTE_ARCHIVE;    // Set
    }
    if ( mask[1] < 0 )
    {
        attr &= ~FILE_ATTRIBUTE_DIRECTORY; // Clear
    }
    if ( mask[1] > 0 )
    {
        attr |= FILE_ATTRIBUTE_DIRECTORY;  // Set
    }
    if ( mask[2] < 0 )
    {
        attr &= ~FILE_ATTRIBUTE_HIDDEN;    // Clear
    }
    if ( mask[2] > 0 )
    {
        attr |= FILE_ATTRIBUTE_HIDDEN;     // Set
    }
    if ( mask[3] < 0 )
    {
        attr &= ~FILE_ATTRIBUTE_READONLY;  // Clear
    }
    if ( mask[3] > 0 )
    {
        attr |= FILE_ATTRIBUTE_READONLY;   // Set
    }
    if ( mask[4] < 0 )
    {
        attr &= ~FILE_ATTRIBUTE_SYSTEM;    // Clear
    }
    if ( mask[4] > 0 )
    {
        attr |= FILE_ATTRIBUTE_SYSTEM;     // Set
    }

    return  attr;
}

/**
 * Changes the file attributes of the specified file to those specified by attr.
 *
 * @param file  File to change the attributes of.
 *
 * @param attr  New file attributes.
 *
 * @return True on success, false on error.
 *
 * @remarks  Note that this function was named SetFileMode() in the old IBM
 * code.
 */
static bool setAttr(const char *file, uint32_t attr)
{
    if ( SetFileAttributes(file, attr) == 0 )
    {
        return false;
    }
    return true;
}


/**
 * This function is used by SysFileTree only.
 *
 * Formats the line for a found file and adds it to the stem containing all the
 * found files.
 *
 * @param c
 * @parm  path
 * @param treeData
 * @param newMask
 * @param options
 * @param wfd
 *
 * @return True on success, false on error.
 *
 * @remarks  We try to use the static buffers in treeData, but if they are not
 *  big enough, we allocate memory.  If we do allocate memory, we have to free
 *  it of course.  We can determine if the memory needs to be freed by checking
 *  that either nFoundFile, or nFoundFileLine, are the same size as they are
 *  originally set to, or not.
 *
 *  If the file search is a very deep recursion in the host file system, a very
 *  large number of String objects may be created in the single Call context of
 *  SysFileTree.  A reference to each created object is saved in a hash table to
 *  protect it from garbage collection, which can lead to a very large hash
 *  table.  To prevent the creation of a very large hash table, we create a temp
 *  object, pass that object to the interpreter, and then tell the interpreter
 *  the object no longer needs to be protected in this call context.
 */
static bool formatFile(RexxCallContext *c, char *path, RXTREEDATA *treeData, int32_t *newMask,
                       uint32_t options, WIN32_FIND_DATA *wfd)
{
    SYSTEMTIME systime;
    FILETIME   ftLocal;

    char   *dFoundFile = treeData->foundFile;
    size_t  nFoundFile = sizeof(treeData->foundFile);

    int len = _snprintf(dFoundFile, nFoundFile, "%s%s", path, wfd->cFileName);
    if ( len < 0 || len == nFoundFile )
    {
        nFoundFile = strlen(path) + strlen(wfd->cFileName) + 1;
        dFoundFile = (char *)LocalAlloc(LPTR, nFoundFile);
        if ( dFoundFile == NULL )
        {
            outOfMemoryException(c->threadContext);
            return false;
        }

        // Buffer is sure to be big enough now, we we don't check the return.
        _snprintf(dFoundFile, nFoundFile, "%s%s", path, wfd->cFileName);
    }

    if ( options & NAME_ONLY )
    {
        RexxStringObject t = c->String(dFoundFile);

        // Add the file name to the stem and be done with it.
        treeData->count++;
        c->SetStemArrayElement(treeData->files, treeData->count, t);
        c->ReleaseLocalReference(t);

        if ( nFoundFile != sizeof(treeData->foundFile) )
        {
            LocalFree(dFoundFile);
        }
        return true;
    }

    // The file attributes need to be changed before we format the found file
    // line.

    uint32_t changedAttr = newAttr(newMask, wfd->dwFileAttributes);
    if ( changedAttr != wfd->dwFileAttributes )
    {
        // try to set the attributes, but if it fails, just use the exsiting.
        if ( ! setAttr(treeData->foundFile, changedAttr & ~FILE_ATTRIBUTE_DIRECTORY) )
        {
            changedAttr = wfd->dwFileAttributes;
        }
    }

    // Convert UTC to local file time, and then to system format.
    FileTimeToLocalFileTime(&wfd->ftLastWriteTime, &ftLocal);
    FileTimeToSystemTime(&ftLocal, &systime);

    // The fileTime buffer is 64 bytes, and the fileAtt buffer is 16 bytes.
    // Since we can count the characters put into the buffer here, there is
    // no need to check for buffer overflow.

    if ( options & LONG_TIME )
    {
        sprintf(treeData->fileTime, "%4d-%02d-%02d %02d:%02d:%02d  %10lu  ",
                systime.wYear,
                systime.wMonth,
                systime.wDay,
                systime.wHour,
                systime.wMinute,
                systime.wSecond,
                wfd->nFileSizeLow);
    }
    else
    {
        if ( options & EDITABLE_TIME )
        {
            sprintf(treeData->fileTime, "%02d/%02d/%02d/%02d/%02d  %10lu  ",
                    (systime.wYear + 100) % 100,
                    systime.wMonth,
                    systime.wDay,
                    systime.wHour,
                    systime.wMinute,
                    wfd->nFileSizeLow);
        }
        else
        {
            sprintf(treeData->fileTime, "%2d/%02d/%02d  %2d:%02d%c  %10lu  ",
                    systime.wMonth,
                    systime.wDay,
                    (systime.wYear + 100) % 100,
                    (systime.wHour < 13 && systime.wHour != 0 ?
                     systime.wHour : (abs(systime.wHour - (SHORT)12))),
                    systime.wMinute,
                    (systime.wHour < 12 || systime.wHour == 24) ? 'a' : 'p',
                    wfd->nFileSizeLow);
        }
    }

    sprintf(treeData->fileAttr, "%c%c%c%c%c  ",
           (changedAttr & FILE_ATTRIBUTE_ARCHIVE)   ? 'A' : '-',
           (changedAttr & FILE_ATTRIBUTE_DIRECTORY) ? 'D' : '-',
           (changedAttr & FILE_ATTRIBUTE_HIDDEN)    ? 'H' : '-',
           (changedAttr & FILE_ATTRIBUTE_READONLY)  ? 'R' : '-',
           (changedAttr & FILE_ATTRIBUTE_SYSTEM)    ? 'S' : '-');

    // Now format the complete line, allocating memory if we have to.

    char   *dFoundFileLine = treeData->foundFileLine;
    size_t  nFoundFileLine = sizeof(treeData->foundFileLine);

    len = _snprintf(dFoundFileLine, nFoundFileLine, "%s%s%s",
                    treeData->fileTime, treeData->fileAttr, dFoundFile);
    if ( len < 0 || len == nFoundFileLine )
    {
        nFoundFileLine = strlen(treeData->fileTime) + strlen(treeData->fileAttr) + nFoundFile + 1;
        dFoundFileLine = (char *)LocalAlloc(LPTR, nFoundFileLine);

        if ( dFoundFileLine == NULL )
        {
            outOfMemoryException(c->threadContext);
            if ( nFoundFile != sizeof(treeData->foundFile) )
            {
                LocalFree(dFoundFile);
            }
            return false;
        }
        // Buffer is sure to be big enough now so we don't check return.
        _snprintf(dFoundFileLine, nFoundFileLine, "%s%s%s", treeData->fileTime, treeData->fileAttr, dFoundFile);
    }

    // Place found file line in the stem.
    RexxStringObject t = c->String(dFoundFileLine);

    treeData->count++;
    c->SetStemArrayElement(treeData->files, treeData->count, t);
    c->ReleaseLocalReference(t);

    if ( nFoundFile != sizeof(treeData->foundFile) )
    {
        LocalFree(dFoundFile);
    }
    if ( nFoundFileLine != sizeof(treeData->foundFileLine) )
    {
        LocalFree(dFoundFileLine);
    }

    return true;
}

/**
 * Finds all files matching a file specification, formats a file name line and
 * adds the formatted line to a stem.  Much of the data to complete this
 * operation is contained in the treeData struct.
 *
 * This is a recursive function that may search through subdirectories if the
 * recurse option is used.
 *
 * @param c           Call context we are operating in.
 *
 * @param path        Current directory we are searching.
 *
 * @param treeData    Struct containing data pertaining to the search, such as
 *                    the file specification we are searching for, the stem to
 *                    put the results in, etc..
 *
 * @param targetMask  An array of integers which describe the source attribute
 *                    mask.  Only files with attributes matching this mask will
 *                    be found.
 *
 * @param newMask     An array of integers which describe the target attribute
 *                    mask.  Attributes of all found files will be changed / set
 *                    to the values specified by this mask.
 * @param options
 *
 * @return uint32_t
 *
 * @remarks  For both targetMask and newMask, each index of the mask corresponds
 *           to a different file attribute.  Each index and its associated
 *           attribute are as follows:
 *
 *                        mask[0] = FILE_ARCHIVED
 *                        mask[1] = FILE_DIRECTORY
 *                        mask[2] = FILE_HIDDEN
 *                        mask[3] = FILE_READONLY
 *                        mask[4] = FILE_SYSTEM
 *
 *           A negative value at a given index indicates that the attribute bit
 *           of the file is not set.  A positive number indicates that the
 *           attribute should be set. A value of 0 indicates a "Don't Care"
 *           setting.
 *
 *           A close reading of MSDN seems to indicate that as long as we are
 *           compiled for ANSI, which we are, that MAX_PATH is sufficiently
 *           large.  But, we will code for the possibility that it is not large
 *           enough, by mallocing dynamic memory if _snprintf indicates a
 *           failure.
 *
 *           We point dTmpFileName at the static buffer and nTmpFileName is set
 *           to the size of the buffer.  If we have to allocate memory,
 *           nTmpFileName will be set to the size we allocate and if
 *           nTmpFileName does not equal what it is originally set to, we know
 *           we have to free the allocated memory.
 */
static bool recursiveFindFile(RexxCallContext *c, char *path, RXTREEDATA *treeData,
                              int32_t *targetMask, int32_t *newMask, uint32_t options)
{
  WIN32_FIND_DATA  wfd;
  HANDLE           fHandle;
  char             tmpFileName[FNAMESPEC_BUF_LEN];
  char            *dTmpFileName = tmpFileName;       // Dynamic memory for tmpFileName, static memory to begin with.
  size_t           nTmpFileName = FNAMESPEC_BUF_LEN; // CouNt of bytes in dTmpFileName.
  int32_t          len;
  bool             result = true;

  len = _snprintf(dTmpFileName, nTmpFileName, "%s%s", path, treeData->dFNameSpec);
  if ( len < 0 || len == nTmpFileName )
  {
      nTmpFileName = strlen(path) + strlen(treeData->dFNameSpec) + 1;
      dTmpFileName = (char *)LocalAlloc(LPTR, nTmpFileName);
      if ( dTmpFileName == NULL )
      {
          outOfMemoryException(c->threadContext);
          result = false;
          goto done_out;
      }
      // buffer is sure to be big enough now, so we don't check the return.
      _snprintf(dTmpFileName, nTmpFileName, "%s%s", path, treeData->dFNameSpec);
  }

  fHandle = FindFirstFile(dTmpFileName, &wfd);
  if ( fHandle != INVALID_HANDLE_VALUE )
  {
      do
      {
          // Skip dot directories
          if ( strcmp(wfd.cFileName, ".") == 0 || strcmp(wfd.cFileName, "..") == 0 )
          {
              continue;
          }

          if ( sameAttr(targetMask, wfd.dwFileAttributes, options) )
          {
              if ( ! formatFile(c, path, treeData, newMask, options, &wfd) )
              {
                  FindClose(fHandle);
                  result = false;
                  goto done_out;
              }
          }
      } while ( FindNextFile(fHandle, &wfd) );

      FindClose(fHandle);
  }

  if ( options & RECURSE )
  {
      // Build new target spec.  Above, path + fileSpec fit into tmpFileName,
      // fileSpec is always longer than 1 character, so we are okay here.
      sprintf(dTmpFileName, "%s*", path);

      fHandle = FindFirstFile(dTmpFileName, &wfd);
      if ( fHandle != INVALID_HANDLE_VALUE )
      {
          do
          {
              // Skip non-directories and dot directories.
              if ( ! (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) ||
                   strcmp(wfd.cFileName, ".") == 0 || strcmp(wfd.cFileName, "..") == 0 )
              {
                  continue;
              }

              // Build the new directory file name.
              len = _snprintf(dTmpFileName, nTmpFileName, "%s%s\\", path, wfd.cFileName);
              if ( len < 0 || len == nTmpFileName )
              {
                  // We may need to free dTmpFileName if it is now allocated
                  // memory.
                  if ( nTmpFileName != FNAMESPEC_BUF_LEN )
                  {
                      LocalFree(dTmpFileName);
                  }

                  nTmpFileName = strlen(path) + strlen(wfd.cFileName) + 2;
                  dTmpFileName = (char *)LocalAlloc(LPTR, nTmpFileName);
                  if ( dTmpFileName == NULL )
                  {
                      outOfMemoryException(c->threadContext);
                      FindClose(fHandle);
                      result = false;
                      goto done_out;
                  }
                  // buffer is sure to be big enough now, so we don't check the
                  // return.
                  _snprintf(dTmpFileName, nTmpFileName, "%s%s\\", path, wfd.cFileName);
              }

              // Search the next level.
              if ( ! recursiveFindFile(c, tmpFileName, treeData, targetMask, newMask, options) )
              {
                  FindClose(fHandle);
                  result = false;
                  goto done_out;
              }
          }
          while (FindNextFile(fHandle, &wfd));

          FindClose(fHandle);
      }
  }

done_out:

    if ( nTmpFileName != FNAMESPEC_BUF_LEN )
    {
        safeLocalFree(dTmpFileName);
    }
    return result;
}

/**
 * This is a SysFileTree() specific function.  It is only called, indirectly
 * through getPath(), from SysFileTree().
 *
 * This function mimics the old IBM code.
 *
 * Leading spaces are stripped, in some cases. A file specification of "." is
 * changed to "*.*" and a file specification of ".." is changed to "..\*.*"
 *
 * Leading spaces in fSpec are stripped IFF the first character(s) after the
 * leading spaces:
 *
 *       is '\' or '/'
 *     or
 *       is '.\' or './'
 *     or
 *       is '..\' or '../'
 *     or
 *        is z:  (i.e., a drive letter)
 *
 * @param fSpec  The SysFileTree search specification
 *
 * @return A pointer to fSpec, possibly adjust to point to the first non-space
 *         character in the string.
 *
 * @side effects:  fSpec may be changed from "." to "*.*" or may be changed from
 *                 ".." to "..\*.*"
 *
 * @assumes:  The buffer for fSpec is large enough for the possible changes.
 */
static char *adjustFSpec(char *fSpec)
{
    size_t i = 0;

    // Skip leading blanks.
    while ( fSpec[i] == ' ' )
    {
        i++;
    }

    if ( i > 0 )
    {
        size_t len = strlen(fSpec);

        // This series of if statements could be combined in to on huge if, but
        // this is easier to comprehend:
        if ( fSpec[i] == '\\' || fSpec[i] == '/' )                         // The "\" case
        {
            fSpec = &fSpec[i];
        }
        else if ( fSpec[i] == '.' )
        {
            if ( i + 1 < len )
            {
                if ( fSpec[i + 1] == '\\' || fSpec[i + 1] == '/' )         // The ".\" case
                {
                    fSpec = &fSpec[i];
                }
                else if ( i + 2 < len )
                {
                    if ( fSpec[i + 1] == '.' &&
                         (fSpec[i + 2] == '\\' || fSpec[i + 2] == '/') )   // The "..\" case
                    {
                        fSpec = &fSpec[i];
                    }
                }
            }
        }
        else if ( i + 1 < len && fSpec[i + 1] == ':' )                     // The "z:' case
        {
            fSpec = &fSpec[i];
        }
    }

    if ( strcmp(fSpec, ".") == 0 )
    {
        // If fSpec is exactly "." then change it to "*.*"
        strcpy(fSpec, "*.*");
    }
    else if ( strcmp(fSpec, "..") == 0 )
    {
        // Else if fSpec is exactly ".." then change it to "..\*.*"
        strcpy(fSpec, "..\\*.*");
    }

    return fSpec;
}

static bool safeGetCurrentDirectory(RexxCallContext *c, char **pPath, size_t *pPathLen)
{
    size_t  pathLen = *pPathLen;
    char   *path    = *pPath;

    // Get the current directory.  First check that the path buffer is large
    // enough.
    uint32_t ret = GetCurrentDirectory(0, 0);
    if ( ret == 0 )
    {
        systemServiceExceptionCode(c->threadContext, "GetCurrentDirectory", GetLastError());
        return false;
    }

    // we might need to add a trailing backslash here, so make sure we leave enough room
    ret += FNAMESPEC_BUF_EXTRA;

    path = (char *)LocalAlloc(LPTR, ret);
    if ( path == NULL )
    {
        outOfMemoryException(c->threadContext);
        return false;
    }

    // Fix up our input path / pathLen variables now.  The input path buffer
    // is allocated memory, so we need to free it.
    LocalFree(*pPath);
    *pPath    = path;
    *pPathLen = ret;

    ret = GetCurrentDirectory(ret, path);
    if ( ret == 0 )
    {
        systemServiceExceptionCode(c->threadContext, "GetCurrentDirectory", GetLastError());
        return false;
    }

    return true;
}

static bool expandNonPath2fullPath(RexxCallContext *c, char *fSpec, char **pPath, size_t *pPathLen, int *lastSlashPos)
{
    char     *buf    = NULL;  // used to save current dir
    char      drv[3] = {0};   // used to change current drive
    uint32_t  ret    = 0;
    bool      result = false;

    // fSpec could be a drive designator.
    if ( fSpec[1] == ':' )
    {
        // Save the current drive and path, first get needed buffer size.
        ret = GetCurrentDirectory(0, 0);
        if ( ret == 0 )
        {
            systemServiceExceptionCode(c->threadContext, "GetCurrentDirectory", ret);
            goto done_out;
        }

        buf = (char *)LocalAlloc(LPTR, ret);
        if ( buf == NULL )
        {
            outOfMemoryException(c->threadContext);
            goto done_out;
        }

        ret = GetCurrentDirectory(ret, buf);
        if ( ret == 0 )
        {
            systemServiceExceptionCode(c->threadContext, "GetCurrentDirectory", ret);
            goto done_out;
        }

        // Just copy the drive letter and the colon, omit the rest.  This is
        // necessary e.g. for something like "I:*"
        memcpy(drv, fSpec, 2);

        // Change to the specified drive, get the current directory, then go
        // back to where we came from.
        SetCurrentDirectory(drv);
        bool success = safeGetCurrentDirectory(c, pPath, pPathLen);

        SetCurrentDirectory(buf);
        LocalFree(buf);
        buf = NULL;

        if ( ! success )
        {
            systemServiceExceptionCode(c->threadContext, "GetCurrentDirectory", ret);
            goto done_out;
        }

        // make drive the path
        *lastSlashPos = 1;
    }
    else
    {
        // No drive designator, just get the current directory.
        if ( ! safeGetCurrentDirectory(c, pPath, pPathLen) )
        {
            goto done_out;
        }
    }

    // If we need a trailing slash, add one.
    char *path = *pPath;
    if ( path[strlen(path) - 1] != '\\' )
    {
        strcat(path, "\\");
    }

    result = true;

done_out:
    safeLocalFree(buf);
    return result;
}


/**
 * Splits the path portion off from fSpec and returns it in the path buffer.
 *
 * When this function is called, there is always at least one slash in fSpec.
 *
 * @param c
 * @param fSpec
 * @param lastSlashPos
 * @param pPath
 * @param pPathLen
 *
 * @return bool
 *
 * @remarks  The size of the path buffer is guarenteed to be at least the string
 *           length of fSpec + FNAMESPEC_BUF_EXTRA (8) in size.  Or MAX (264)
 *           bytes in size.  Whichever is larger.  So path is big enough to
 *           contain all of fSpec + 7 characters.
 *
 *           We may have to enlarge the passed in path buffer.  If we do, we
 *           need to be sure and update the path buffer pointer and the path
 *           length. As long as we keep pPath and pPathLen correct, the caller
 *           will take care of freeing any memory.
 *
 *           But if we do change pPath, we need to free the buffer it was
 *           pointing to.
 */
static bool expandPath2fullPath(RexxCallContext *c, char *fSpec, size_t lastSlashPos, char **pPath, size_t *pPathLen)
{
    size_t l = 0;    // Used to calculate lengths of strings.

    char   *path    = *pPath;
    size_t  pathLen = *pPathLen;

    // If fSpec starts with a drive designator, then we have a full path. Copy
    // over the path portion, including the last slash, and null terminate it.
    if (fSpec[1] == ':')
    {
        l = lastSlashPos + 1;
        memcpy(path, fSpec, l);
        path[l] = '\0';
    }
    else
    {
        char fpath[MAX_PATH];
        char drive[_MAX_DRIVE];
        char dir[_MAX_DIR];
        char fname[_MAX_FNAME];
        char ext[_MAX_EXT];
        char lastChar;

        // fpath is the only buffer we need to worry about being too small.
        // Although, when compiled for ANSI, which we are, I really think it is
        // impossible for MAX_PATH to be too small.
        char   *dFPath = fpath;
        size_t  nFPath = MAX_PATH;

        if ( lastSlashPos == 0 )
        {
            // Only 1 slash at the beginning, get the full path.
            _fullpath(dFPath, "\\", nFPath);

            l = strlen(dFPath) + strlen(&fSpec[1]) + 1;
            if ( l > nFPath )
            {
                if ( ! getBiggerBuffer(c, &dFPath, &nFPath, l, nFPath) )
                {
                    return false;
                }
            }

            strcat(dFPath, &fSpec[1]);
        }
        else
        {
            // Chop off the path part by putting a null at the last slash, get
            // the full path, and then put the slash back.
            fSpec[lastSlashPos] = '\0';
            if ( _fullpath(dFPath, fSpec, nFPath) == NULL )
            {
                // This will double the buffer until either _fullpath()
                // succeeds, or we run out of memory.  If we go through the loop
                // more than once, and fail, we need to free memory allocated
                // for dFPath.  We fix fSpec on failure, but that is not really
                // needed, the caller(s) will just quit on failure of this
                // function.
                do
                {
                    if ( ! getBiggerBuffer(c, &dFPath, &nFPath, l, MAX_PATH) )
                    {
                        if ( nFPath != MAX_PATH )
                        {
                            LocalFree(dFPath);
                        }

                        fSpec[lastSlashPos] = '\\';
                        return false;
                    }
                } while ( _fullpath(dFPath, fSpec, nFPath) == NULL );
            }

            fSpec[lastSlashPos] = '\\';

            lastChar = dFPath[strlen(dFPath) - 1];
            if (lastChar != '\\' && lastChar != '/')
            {
                l = strlen(dFPath) + strlen(&fSpec[lastSlashPos]) + 1;
                if ( l > nFPath )
                {
                    if ( ! getBiggerBuffer(c, &dFPath, &nFPath, l, MAX_PATH) )
                    {
                        // If dFPath was allocated, free it.
                        if ( nFPath != MAX_PATH )
                        {
                            LocalFree(dFPath);
                        }
                        return false;
                    }
                }

                strcat(dFPath, &fSpec[lastSlashPos]);
            }
        }

        _splitpath(dFPath, drive, dir, fname, ext);

        l = strlen(drive) + strlen(dir) + 1;
        if ( l > pathLen )
        {
            if ( ! getBiggerBuffer(c, &path, &pathLen, l, pathLen) )
            {
                return false;
            }

            LocalFree(*pPath);
            *pPath    = path;
            *pPathLen = pathLen;
        }

        strcpy(path, drive);
        strcat(path, dir);

        // If path is invalid, (the empty string,) for some reason, copy the
        // path from fSpec.  That is from the start of the string up through the
        // last slash.  Then zero terminate it.  The path buffer is guaranteed
        // big enough for this, see the remarks.
        if ( strlen(path) == 0 )
        {
            memcpy(path, fSpec, lastSlashPos + 1);
            path[lastSlashPos + 1] = '\0';
        }

        // If we need a trailing slash, add it.  Again, the path buffer is
        // guaranteed to be big enough.
        if (path[strlen(path) - 1] != '\\')
        {
            strcat(path, "\\");
        }
    }

    return true;
}

/**
 * This is a SysFileTree() specific function..
 *
 * This function expands the file spec passed in to the funcition into its full
 * path name.  The full path name is then split into the path portion and the
 * file name portion.  The path portion is then returned in path and the file
 * name portion is returned in fileName.
 *
 * The path portion will end with the '\' char if fSpec contains a path.
 *
 * @param fSpec
 * @param path       Pointer to path buffer.  Path buffer is allocated memory,
 *                   not a static buffer.
 * @param filename
 * @param pathLen    Pointer to size of the path buffer.
 *
 * @remarks  On entry, the buffer pointed to by fSpec is guaranteed to be at
 *           least strlen(fSpec) + FNAMESPEC_BUF_EXTRA (8).  So, we can strcat
 *           to it at least 7 characters and still have it null terminated.
 *
 *           In addition, the path buffer is guarenteed to be at least that size
 *           also.
 */
static bool getPath(RexxCallContext *c, char *fSpec, char **path, char *filename, size_t *pathLen)
{
    size_t len;                     // length of filespec
    int    lastSlashPos;            // position of last slash

    fSpec = adjustFSpec(fSpec);

    // Find the position of the last slash in fSpec
    len = strlen(fSpec);

    // Get the maximum position of the last '\'
    lastSlashPos = (int)len;

    // Step back through fSpec until at its beginning or at a '\' or '/' character
    while ( fSpec[lastSlashPos] != '\\' && fSpec[lastSlashPos] != '/' && lastSlashPos >= 0 )
    {
        --lastSlashPos;
    }

    // If lastSlashPos is less than 0, then there is no backslash present in
    // fSpec.
    if ( lastSlashPos < 0 )
    {
        if ( ! expandNonPath2fullPath(c, fSpec, path, pathLen, &lastSlashPos) )
        {
            return false;
        }
    }
    else
    {
        if ( ! expandPath2fullPath(c, fSpec, lastSlashPos, path, pathLen) )
        {
            return false;
        }
    }

    // Get the file name from fSpec, the portion just after the last '\'
    if ( fSpec[lastSlashPos + 1] != '\0' )
    {
        // The position after the last slash is not the null terminator so there
        // is something to copy over to the file name segment.
        strcpy(filename, &fSpec[lastSlashPos + 1]);
    }
    else
    {
        // The last slash is the last character in fSpec, just use wildcards for
        // the file name segment.
        strcpy(filename, "*.*");
    }

    return true;
}

/**
 * This is a SysFileTree specific function.
 *
 * Determines the options by converting the character based argument to the
 * correct set of flags.
 *
 * @param c
 * @param opts
 * @param pOpts
 *
 * @return bool
 */
static bool goodOpts(RexxCallContext *c, char *opts, uint32_t *pOpts)
{
    uint32_t options = *pOpts;

    while ( *opts )
    {
        switch( toupper(*opts) )
        {
          case 'S':                      // recurse into subdirectories
              options |= RECURSE;
              break;

          case 'O':                      // only return names
              options |= NAME_ONLY;
              break;

          case 'T':                      // use short time format, ignored if L is used
            options |= EDITABLE_TIME;
            break;

          case 'L':                      // use long time format
              options |= LONG_TIME;
              break;

          case 'F':                      // include only files
              options &= ~DO_DIRS;
              options |= DO_FILES;
              break;

          case 'D':                      // include only directories
              options |= DO_DIRS;
              options &= ~DO_FILES;
              break;

          case 'B':                      // include both files and directories
              options |= DO_DIRS;
              options |= DO_FILES;
              break;

          case 'I':                      // case insensitive? no op on Windows
              break;

          default:                       // error, unknown option
            return false;
        }
        opts++;
    }

    *pOpts = options;
    return true;
}

/**
 * This is a SysFileTree() specific helper function.
 *
 * Set a mask of unsigned ints to what is specified by a mask of chars.
 *
 * @param c
 * @param msk
 * @param mask
 *
 * @return True on success, false on error.
 *
 * @remarks  If a character in position N is a '+' then the unsigned int at
 *           position N is set to 1.  This is turning it on.
 *
 *           If a character in position N is a '-' then the unsigned int at
 *           position N is set to -1.  This is turning it off.
 *
 *           If a character in position N is a '*' then the unsigned int at
 *           position N is set to 0.  This is saying ignore it, it doesn't
 *           matter what the attribute is.
 */
static bool goodMask(RexxCallContext *c, char *msk, int32_t *mask)
{
    uint32_t y = 0;

    while (*msk)
    {
        if ( *msk == '+' )
        {
            mask[y] = 1;
        }
        else if ( *msk == '-' )
        {
            mask[y] = -1;
        }
        else if (*msk == '*')
        {
            mask[y] = 0;
        }
        else
        {
            return false;
        }

        y++;
        msk++;
    }

    return true;
}

/**
 * This is a SysFileTree specific helper function.
 *
 * Checks the validity of an attribute mask argument and converts the character
 * based mask into an integer based mask.
 *
 * @param context
 * @param msk
 * @param mask
 * @param argPos
 *
 * @return bool
 */
static bool getMaskFromArg(RexxCallContext *context, char *msk, int32_t *mask, size_t argPos)
{
    if ( argumentExists(argPos) && strlen(msk) > 0 )
    {
        if ( strlen(msk) > 5 )
        {
            badMaskException(context->threadContext, argPos, msk);
            return false;
        }

        if ( ! goodMask(context, msk, mask) )
        {
            badMaskException(context->threadContext, argPos, msk);
            return false;
        }
    }
    else
    {
        mask[0] = RXIGNORE;
    }

    return true;
}

/**
 * This is a SysFileTree specific helper function.
 *
 * Checks the validity of the options argument to SysFileTree and converts the
 * character based argument to the proper set of flags.
 *
 * @param context
 * @param opts
 * @param options
 * @param argPos
 *
 * @return bool
 */
static bool getOptionsFromArg(RexxCallContext *context, char *opts, uint32_t *options, size_t argPos)
{
    *options = DO_FILES | DO_DIRS;

    if ( argumentExists(argPos) )
    {
        if ( strlen(opts) == 0 )
        {
            nullStringException(context->threadContext, "SysFileTree", argPos);
            return false;
        }

        if ( ! goodOpts(context, opts, options) )
        {
            badSFTOptsException(context->threadContext, argPos, opts);
            return false;
        }
    }

    return true;
}

/**
 * This is a SysFileTree specific helper function.
 *
 * Allocates and returns a buffer containing the file specification to search
 * for.
 *
 * The file specification consists of the search string as sent by the Rexx
 * user, with possibly some glob characters added.  The returned buffer is
 * bigger than the original string to accommodate these, possible, added
 * characters.  The number of bytes added to the buffer is 8, which is what the
 * original IBM code used.  8 is probably 1 byte more than needed, but there is
 * no reason that this needs to be exact, better too long than too short.
 *
 * If the file speicfication ends in a slash ('\') or a period ('.') or two
 * periods ('..'), then a wild card specification ('*.*') is appended.
 *
 * However, note that there is also the case where a single '.' at the end of
 * the file specification is not used as a directory specifier, but rather is
 * tacked on to the end of a file name.
 *
 * Windows has a sometimes used convention that a '.' at the end of a file name
 * can be used to indicate the file has no extension. For example, given a file
 * named: MyFile a command of "dir MyFile." will produce a listing of "MyFile".
 *
 * In this case we want to leave the file specification alone. that is, do not
 * append a "*.*". A command of "dir *." will produce a directory listing of all
 * files that do not have an extension.
 *
 * @param context
 * @param fSpec
 * @param fSpecLen     [returned]  The length of the original fSpec argument,
 *                     not the length of the allocated buffer.
 * @param fSpecBufLen  [returned]  The length of the length of the allocated
 *                     fSpec buffer.
 * @param argPos
 *
 * @return A string specifying the file pattern to search for.  The buffer
 *         holding the string is larger than the original input specify.
 *
 * @remarks  Caller is responsible for freeing memory.  Memory is allocated
 *           using LocalAlloc(), not malloc().
 *
 *           If the returned buffer is null, a condition has been raised.
 *
 *           FNAMESPEC_BUF_EXTRA (8) is sized to contain the terminating NULL.
 *           So the allocated buffer has room to concatenate 7 characters.
 */
static char *getFileSpecFromArg(RexxCallContext *context, CSTRING fSpec, size_t *fSpecLen,
                                size_t *fSpecBufLen, size_t argPos)
{
    size_t len = strlen(fSpec);
    if ( len == 0 )
    {
        nullStringException(context->threadContext, "SysFileTree", argPos);
        return NULL;
    }

    char *fileSpec = (char *)LocalAlloc(LPTR, len + FNAMESPEC_BUF_EXTRA);
    if ( fileSpec == NULL )
    {
        outOfMemoryException(context->threadContext);
        return NULL;
    }

    // Allocated buffer is zero filled (LPTR flag) already, no need to zero
    // terminate.
    memcpy(fileSpec, fSpec, len);

    if ( fileSpec[len - 1] == '\\' )
    {
        strcat(fileSpec, "*.*");
    }
    else if ( fileSpec[len - 1] == '.')
    {
        if ( len == 1 ||
             (len > 1  && (fileSpec[len - 2] == '\\' || fileSpec[len - 2] == '.')) )
        {
            strcat(fileSpec, "\\*.*");
        }
    }

    *fSpecLen    = len;
    *fSpecBufLen = len + FNAMESPEC_BUF_EXTRA;
    return fileSpec;
}

/**
 * This is a SysFileTree specific helper function.
 *
 * Allocates and returns a buffer large enough to contain the path to search
 * along.
 *
 *  We need a minimum size for the path buffer of at least MAX (264).  But the
 *  old code seemed to think fileSpecLen + FNAMESPEC_BUF_EXTRA could be longer
 *  than that.  I guess it could if the user put in a non-existent long file
 *  path.
 *
 *  The old code of checking fSpecLen is still used, but I'm unsure of its exact
 *  purpose.
 *
 * @param context
 * @param fSpecLen
 * @param pathLen
 *
 * @return A buffer the larger of MAX or fSpecLen + FNAMESPEC_BUF_EXTRA bytes in
 *         size.  Returns NULL on failure.
 *
 * @remarks  The caller is resposible for freeing the allocated memory.
 *
 *           LocalAlloc(), not malloc() is used for memory allocation.
 *
 *           Note that the path buffer is guarenteed to be FNAMESPEC_BUF_EXTRA
 *           (8) bytes larger than the fNameSpec buffer in the caller.  This is
 *           important in later checks for buffer overflow.
 */
static char *getPathBuffer(RexxCallContext *context, size_t fSpecLen, size_t *pathLen)
{
    size_t bufLen = MAX;

    if ( fSpecLen + FNAMESPEC_BUF_EXTRA > MAX )
    {
        bufLen = fSpecLen + FNAMESPEC_BUF_EXTRA;
    }

    *pathLen = bufLen;

    char *path = (char *)LocalAlloc(LPTR, bufLen);
    if ( path == NULL )
    {
        outOfMemoryException(context->threadContext);
    }

    return path;
}

/**
 * Tests for illegal file name characters in fSpec.
 *
 * @param fSpec
 *
 * @return bool
 *
 * @note  Double quotes in the file spec is not valid, spaces in file names do
 *        not need to be within quotes in the string passed to the Windows API.
 *
 *        A ':' is only valid if it is the second character. Technically a '*'
 *        and a '?' are not valid characters for a file name, but they are okay
 *        for fSpec. Same thing for '\' and '/', they are not valid in a file
 *        name, but they are valid in fSpec. A '/' is iffy. The Windows API
 *        accepts '/' as a directory separator, but most Rexx programmers
 *        probably don't know that.  Not sure if we should flag that as illegal
 *        or not.
 */
static bool illegalFileNameChars(char * fSpec)
{
    static char illegal[] = "<>|\"";

    for ( size_t i = 0; i < 4; i++ )
    {
        if ( strchr(fSpec, illegal[i]) != NULL )
        {
            return true;
        }
    }

    char *pos = strchr(fSpec, ':');
    if ( pos != NULL )
    {
        if ( ((int32_t)(pos - fSpec + 1)) != 2 )
        {
            return true;
        }
        if ( strchr(pos + 1, ':') != NULL )
        {
            return true;
        }
    }

    return false;
}
/**
 * SysFileTree() implementation.  Searches for files in a directory tree
 * matching the specified search pattern.
 *
 * @param  fSpec  [required] The search pattern, may contain glob characters
 *                 and full or partial path informattion. E.g., *.bat, or
 *                 ../../*.txt, or C:\temp.  May not contain illegal file name
 *                 characters which are: ", <, >, |, and :  The semicolon is
 *                 only legal if it is exactly the second character.  Do not use
 *                 a double quote in fSpec, it is not needed and is taken as a
 *                 character in a file name, which is an illegal character.
 *
 * @param  files  [required] A stem to contain the returned results.  On return,
 *                files.0 contains the count N of found files and files.1
 *                through files.N will contain the found files.
 *
 * @param  opts   [optional] Any combination of the following letters that
 *                specify how the search takes place, or how the returned found
 *                file line is formatted.  Case is not significant:
 *
 *                  'B' - Search for files and directories.
 *                  'D' - Search for directories only.
 *                  'F' - Search for files only.
 *                  'O' - Only output file names.
 *                  'S' - Recursively scan subdirectories.
 *                  'T' - Combine time & date fields into one.
 *                  'L' - Long time format
 *                  'I' - Case Insensitive search.
 *
 *                The defualt is 'B' using normal time (neither 'T' nor 'L'.)
 *                The 'I'option is meaningless on Windows.
 *
 * @param targetAttr  [optional] Target attribute mask.  Only files with these
 *                    attributes will be searched for.  The default is to ignore
 *                    the attributes of the files found, so all files found are
 *                    returned.
 *
 * @param newAttr     [optional] New attribute mask.  Each found file will have
 *                    its attributes set (changed) to match this mask.  The
 *                    default is to not change any attributes.
 *
 * @return  0 on success, non-zero on error.  For all errors, a condition is
 *          raised.
 *
 * @remarks  The original IBM code passed in fileSpec to recursiveFindFile(),
 *           but then never used it in recursiveFineFile.  So, that has been
 *           eliminated.
 *
 */
RexxRoutine5(uint32_t, SysFileTree, CSTRING, fSpec, RexxStemObject, files, OPTIONAL_CSTRING, opts,
             OPTIONAL_CSTRING, targetAttr, OPTIONAL_CSTRING, newAttr)
{
     uint32_t     result   = 1;        // Return value, 1 is an error.
     char        *fileSpec = NULL;     // File spec to search for.
     size_t       fSpecLen = 0;        // Length of the original fSpec string.
     size_t       fSpecBufLen = 0;     // Length of the allocated fSpec buffer.
     char        *path     = NULL;     // Path to search along.
     size_t       pathLen  = 0;        // Size of buffer holding path.
     RXTREEDATA   treeData = {0};      // Struct for data.

     context->SetStemArrayElement(files, 0, context->WholeNumber(0));

     treeData.files      = files;
     treeData.dFNameSpec = treeData.fNameSpec;
     treeData.nFNameSpec = FNAMESPEC_BUF_LEN;

     fileSpec = getFileSpecFromArg(context, fSpec, &fSpecLen, &fSpecBufLen, 1);
     if ( fileSpec == NULL )
     {
         goto done_out;
     }

     if ( illegalFileNameChars((char *)fSpec) )
     {
         result = ERROR_INVALID_NAME;
         goto done_out;
     }

     // Some, or all, of fileSpec will eventually be copied into
     // treeData.dFNameSpec. So, if we ensure that treeData.dFNameSpec is big
     // enough to hold fileSpec we do not need to worry about it any more.
     if ( fSpecBufLen >= FNAMESPEC_BUF_LEN )
     {
         if ( ! getBiggerBuffer(context, &treeData.dFNameSpec, &treeData.nFNameSpec, fSpecBufLen + 1, FNAMESPEC_BUF_LEN) )
         {
             goto done_out;
         }
     }

     path = getPathBuffer(context, fSpecLen, &pathLen);
     if ( path == NULL )
     {
         goto done_out;
     }

     uint32_t options = 0;
     if ( ! getOptionsFromArg(context, (char *)opts, &options, 3) )
     {
         goto done_out;
     }

     int32_t targetMask[5] = {0};    // Attribute mask of files to search for.
     int32_t newMask[5]    = {0};    // Attribute mask to set found files to.

     if ( ! getMaskFromArg(context, (char *)targetAttr, targetMask, 4) )
     {
         goto done_out;
     }

     if ( ! getMaskFromArg(context, (char *)newAttr, newMask, 5) )
     {
         goto done_out;
     }

     // Get the full path segment and the file name segment by expanding the
     // file specification string.  It seems highly unlikely, but possible, that
     // this could fail.
     if ( ! getPath(context, fileSpec, &path, treeData.dFNameSpec, &pathLen) )
     {
         goto done_out;
     }

     if ( recursiveFindFile(context, path, &treeData, targetMask, newMask, options) )
     {
         context->SetStemArrayElement(treeData.files, 0, context->WholeNumber(treeData.count));
         result = 0;
     }

done_out:
    safeLocalFree(fileSpec);
    safeLocalFree(path);
    if ( treeData.nFNameSpec != FNAMESPEC_BUF_LEN )
    {
        LocalFree(treeData.dFNameSpec);
    }
    return result;
}


/*************************************************************************
* Function:  SysGetKey                                                   *
*                                                                        *
* Syntax:    call SysGetKey [echo]                                       *
*                                                                        *
* Params:    echo - Either of the following:                             *
*                    'ECHO'   - Echo the inputted key (default).         *
*                    'NOECHO' - Do not echo the inputted key.            *
*                                                                        *
* Return:    The key striked.                                            *
*************************************************************************/

RexxRoutine1(RexxStringObject, SysGetKey, OPTIONAL_CSTRING echoOpt)
{
    int       tmp;                       /* Temp var used to hold      */
    /* keystroke value            */
    bool      echo = true;               /* Set to false if we         */

    if (numargs == 1)                  /* validate arguments         */
    {
        if (!_stricmp(echoOpt, "NOECHO"))
        {
            echo = false;
        }
        else if (_stricmp(args[0].strptr, "ECHO"))
        {
            context->InvalidRoutine();
            return NULL;
        }
    }
    if (ExtendedFlag)                  /* if have an extended        */
    {
        tmp = ExtendedChar;                /* get the second char        */
        ExtendedFlag = false;              /* do a real read next time   */
    }
    else
    {
        tmp = _getch();                    /* read a character           */

        /* If a function key or arrow */
        if ((tmp == 0x00) || (tmp == 0xe0))
        {
            ExtendedChar = _getch();         /* Read another character     */
            ExtendedFlag = true;
        }
        else
        {
            ExtendedFlag = false;
        }
    }
    if (echo)                            /* echoing?                   */
    {
        _putch(tmp);                       /* write the character back   */
    }

    return context->NewString(&tmp, 1);
}

/*************************************************************************
* Function:  SysIni                                                      *
*                                                                        *
* Syntax:    call SysIni [inifile], app [,key/stem] [,val/stem]          *
*                                                                        *
* Params:    inifile - INI file from which to query or write info.  The  *
*                       default is the current user INI file.            *
*            app     - Application title to work with.  May be either    *
*                       of the following:                                *
*                        'ALL:' - All app titles will be returned in the *
*                                  stem variable specified by the next   *
*                                  parameter.                            *
*                        other  - Specific app to work with.             *
*            key     - Key to work with.  May be any of the following:   *
*                        'ALL:'    - All key titles will be returned in  *
*                                     the stem variable specified by the *
*                                     next parameter.                    *
*                        'DELETE:' - All keys associated with the app    *
*                                     will be deleted.                   *
*                        other     - Specific key to work with.          *
*            val     - Key to work with. May be either of the following: *
*                        'DELETE:' - Delete app/key pair.                *
*                        other     - Set app/key pair info to data spec- *
*                                     ified.                             *
*            stem    - Name of stem variable in which to store results.  *
*                      Stem.0 = Number found (n).                        *
*                      Stem.1 - Stem.n = Entries found.                  *
*                                                                        *
* Return:    other          - Info queried from specific app/key pair.   *
*            ''             - Info queried and placed in stem or data    *
*                              deleted successfully.                     *
*            ERROR_NOMEM    - Out of memory.                             *
*            ERROR_RETSTR   - Error opening INI or querying/writing info.*
*************************************************************************/

RexxRoutine4(RexxStringObject, SysIni, OPTIONAL_CSTRING, IniFile, CSTRING, App, OPTIONAL_CSTRING, key, OPTIONAL_CSTRING, val)
{
    bool        wildCard = false;        /* Set to true if a wildcard  */
                                         /* operation                  */
    bool        queryApps;               /* Set to true if a query     */
                                         /* operation                  */
    bool        terminate = true;        /* perform WinTerminate call  */
    size_t      buffersize;              /* return buffer size         */

    RXSTEMDATA  ldp(context);            // used to manipulate the stem variable for return values.


    // the ini file is optional and defaults to WIN.INI
    if (IniFile == NULL)
    {
        IniFile = "WIN.INI";
    }

    // if the key was not specified, use a null string
    if (key == NULL)
    {
        key = "";
    }

    // Process first off of the app key. This could be a keyword command, which changes
    // the meaning of the following arguments.

    // All is a wild card value asking for everything
    if (_stricmp(app, "ALL:") == 0)
    {

        // the third arg is required and is the stem variable name.
        // the 4th argument cannot be specified
        if (argumentOmitted(3) || argumentExists(4))
        {
            context->InvalidRoutine();
            return 0;
        }
        // try to get the stem variable, raise an error if invalid
        if (!ldp.setStem(key))
        {
            context->InvalidRoutine();
            return 0;
        }
    }



    // all argument variables now have values...time to decode what
    // was specified

    // all is a special app name, this wants everything
    if (_stricmp(app, "ALL:") == 0)
    {
        RXSTEMDATA  stemVariable(context);            // used to manipulate the stem variable for return values.

        // the third arg is required and is the stem variable name.
        // the 4th argument cannot be specified
        if (argumentOmitted(3) || argumentExists(4))
        {
            context->InvalidRoutine();
            return 0;
        }

        // try to get the stem variable, raise an error if invalid
        if (!ldp.setStem(key))
        {
            context->InvalidRoutine();
            return 0;
        }

        size_t lSize = 0x0000ffffL;
        // Allocate a large buffer for retrieving the information
        char *returnVal = (char *)GlobalAlloc(GPTR, lSize);
        if (returnVal == NULL)
        {
            return context->NewStringFromAsciiz(ERROR_NOMEM);
        }
        // now retrieve the application names.
        lSize = GetPrivateProfileString(NULL, NULL, "", returnVal, (DWORD)lSize, IniFile);
        // zero indicates there was an error
        if (lSize == 0)
        {
            GlobalFree(returnVal);
            return context->NewStringFromAsciiz(ERROR_RETSTR);
        }

        // parse this up into a list added to the stem variable
        stemVariable.setList(returnVal);
        return context->NullString;
    }

    // ok, this is targetted at a particular app. Now decode what sort of operation this is.

    // this could be a request for all keys of a given app
    // the val argument must be a stem variable name.
    if (!_stricmp(key, "ALL:"))
    {
        RXSTEMDATA  stemVariable(context);            // used to manipulate the stem variable for return values.

        // we need a stem variable name
        if (argumentOmitted(4) || !stemVariable.setStem(val))
        {
            context->InvalidRoutine();
            return 0;
        }

        size_t lSize = 0x0000ffffL;
        // Allocate a large buffer for retrieving the information
        char *returnVal = (char *)GlobalAlloc(GPTR, lSize);
        if (returnVal == NULL)
        {
            return context->NewStringFromAsciiz(ERROR_NOMEM);
        }

        // Retrieve all keys for a specific application
        lSize = GetPrivateProfileString(app, NULL, "", returnVal, (DWORD)lSize, IniFile);

        // zero indicates there was an error
        if (lSize == 0)
        {
            GlobalFree(returnVal);
            return context->NewStringFromAsciiz(ERROR_RETSTR);
        }

        // parse this up into a list added to the stem variable
        stemVariable.setList(returnVal);
        return context->NullString;
    }

    // this could be a DELETE: operation for a particular application
    if (_stricmp(key, "DELETE:")) == 0)
    {
        // A request to delete all keys for a given application
        if (!WritePrivateProfileString(app, NULL, NULL, IniFile))
        {
            return context->NewStringFromAsciiz(ERROR_RETSTR);
        }
        else
        {
            return context->NullString;
        }
    }

    // OK, if we got this far, then we're working with a specific key for a specific app.
    // This is either a retrieval operation, a delete request, or a set request.

    // a NULL value argument is a retrieval request. This is the return value.
    if (val == NULL)
    {
        size_t lSize = 0x0000ffffL;
        // Allocate a large buffer for retrieving the information
        char *returnVal = (char *)GlobalAlloc(GPTR, lSize);
        if (returnVal == NULL)
        {
            return context->NewStringFromAsciiz(ERROR_NOMEM);
        }

        // Retrieve just the given key value
        lSize = GetPrivateProfileString(app, key, "", returnVal, (DWORD)lSize, IniFile);

        // zero indicates there was an error
        if (lSize == 0)
        {
            GlobalFree(returnVal);
            return context->NewStringFromAsciiz(ERROR_RETSTR);
        }

        RexxStringObject ret = context->NewString(returnValue, lSize);
        GlobalFree(returnVal);
        return ret;
    }

    // this could be a key deletion request
    if (_stricmp(val, "DELETE:")) == 0)
    {
        // A request to delete all keys for a given application
        if (!WritePrivateProfileString(app, key, NULL, IniFile))
        {
            return context->NewStringFromAsciiz(ERROR_RETSTR);
        }
        else
        {
            return context->NullString;
        }
    }

    // we have a value specified and it is not the special key marker, this is a set operation
    if (!WritePrivateProfileString(app, key, val, IniFile))
    {
        return context->NewStringFromAsciiz(ERROR_RETSTR);
    }
    else
    {
        return context->NullString;
    }
}


/*************************************************************************
* Function:  SysMkDir                                                    *
*                                                                        *
* Syntax:    call SysMkDir dir                                           *
*                                                                        *
* Params:    dir - Directory to be created.                              *
*                                                                        *
* Return:    NO_UTIL_ERROR                                               *
*            Return code from CreateDirectory()                          *
*************************************************************************/

RexxRoutine1(int, SysMkDir, CSTRING, dir)
{
    return CreateDirectory(dir, NULL) != 0 ? 0 : GetLastError();
}


/*************************************************************************
* Function:  SysGetErrortext                                             *
*                                                                        *
* Syntax:    call SysGetErrortext errnumber                              *
*                                                                        *
* Params:    errnumber - error number to be described                    *
*                                                                        *
* Return:    Description or empty string                                 *
*************************************************************************/

RexxRoutine1(RexxStringObject, SysGetErrorText, int32_t, errnum)
{
    char *errmsg;

    if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,NULL,errnum,0,(LPSTR)&errmsg,64,NULL) == 0)
    {
        return context->NullString();
        retstr->strptr[0] = 0x00;
    }
    else
    {                               /* succeeded                  */
        size_t length = strlen(errmsg);

        // FormatMessage returns strings with trailing CrLf, which we want removed
        if (length >= 1 && errmsg[length - 1] == 0x0a)
        {
          length--;
        }
        if (length >= 1 && errmsg[length - 1] == 0x0d)
        {
          length--;
        }

        RexxStringObject ret = context->NewString(errmsg, length);
        LocalFree(errmsg);

        return ret;
}


/*************************************************************************
* Function:  SysWinEncryptFile (W2K only)                                *
*                                                                        *
* Syntax:    call SysWinEncryptFile filename                             *
*                                                                        *
* Params:    filename - file to be encrypted                             *
*                                                                        *
* Return:    NO_UTIL_ERROR                                               *
*            Return code from EncryptFile()                              *
*************************************************************************/

RexxRoutine1(uint32_t, SysWinEncryptFile, CSTRING, fileName)
{
    ULONG rc = EncryptFile(fileName);

    return rc != 0 ? rc : GetLastError();
}

/*************************************************************************
* Function:  SysWinDecryptFile (W2K only)                                *
*                                                                        *
* Syntax:    call SysWinDecryptFile filename                             *
*                                                                        *
* Params:    filename - file to be decrypted                             *
*                                                                        *
* Return:    NO_UTIL_ERROR                                               *
*            Return code from DecryptFile()                              *
*************************************************************************/

RexxRoutine1(uint32_t, SysWinDecryptFile, CSTRING, fileName)
{
    ULONG rc = DecryptFile(fileName);

    return rc != 0 ? rc : GetLastError();
}


/*************************************************************************
* Function:  SysWinVer                                                   *
*                                                                        *
* Syntax:    call SysWinVer                                              *
*                                                                        *
* Return:    Windows Version                                             *
*************************************************************************/

RexxRoutine0(RexxStringObject, SysWinVer)
{
     char windowsDir[256];
     char kernel32[256];

     // get the value of the WINDOWS environment variable
     DWORD windowsDirLength = GetEnvironmentVariable("windir", windowsDir, sizeof(windowsDir));

     // this should be there, but use the likely default if it isn't.
     if (windowsDirLength == 0)
     {
         strcpy("C:\\Windows", windowsDir);
     }

     // get the full path name of the kernel32.dll
     snprintf(kernel32, sizeof(kernel32), "%s\\System32\\kernel32.dll", windowsDir);

     // MS has deprecated GetVersionEx(). The only way to get the real version
     // information now is by querying the version information of one of the system dlls.
     DWORD  verHandle = 0;
     UINT   size      = 0;
     LPBYTE lpBuffer  = NULL;
     // get the size of the version information for this dll.
     DWORD  verSize   = GetFileVersionInfoSize(kernel32, &verHandle);

     if (verSize != NULL)
     {
         LPSTR verData = new char[verSize];

         // get the version information
         if (GetFileVersionInfo(kernel32, verHandle, verSize, verData))
         {
             // the query the specific version information
             if (VerQueryValue(verData,"\\",(void **)&lpBuffer, &size))
             {
                 if (size > 0)
                 {
                     VS_FIXEDFILEINFO *verInfo = (VS_FIXEDFILEINFO *)lpBuffer;
                     if (verInfo->dwSignature == 0xfeef04bd)
                     {
                         char retstr[256];

                         // Doesn't matter if you are on 32 bit or 64 bit,
                         // DWORD is always 32 bits, so first two revision numbers
                         // come from dwFileVersionMS, last two come from dwFileVersionLS
                         snprintf(retstr, sizeof(retstr), "Windows %d.%d.%d",
                             ( verInfo->dwFileVersionMS >> 16 ) & 0xffff,
                             ( verInfo->dwFileVersionMS >>  0 ) & 0xffff,
                             ( verInfo->dwFileVersionLS >> 16 ) & 0xffff);

                         delete[] verData;

                         return context->NewStringFromAsciiz(retstr);
                     }
                 }
             }
         }
         delete[] verData;
     }

     // just return a NULL if not able to get this
     return context->NullString();
}


/*************************************************************************
* Function:  SysSearchPath                                               *
*                                                                        *
* Syntax:    call SysSearchPath path, file [, options]                   *
*                                                                        *
* Params:    path - Environment variable name which specifies a path     *
*                    to be searched (ie 'PATH', 'DPATH', etc).           *
*            file - The file to search for.                              *
*            options -  'C' - Current directory search first (default).  *
*                       'N' - No Current directory search. Only searches *
*                             the path as specified.                     *
*                                                                        *
* Return:    other  - Full path and filespec of found file.              *
*            ''     - Specified file not found along path.               *
*************************************************************************/

RexxRoutine3(RexxStringObject, SysSearchPath, CSTRING, path, CSTRING, file, OPTIONAL_CSTRING, options)
{
    char     szFullPath[_MAX_PATH];      /* returned file name         */
    char     szCurDir[_MAX_PATH];        /* current directory          */
    char     *szEnvStr = NULL;

    LPTSTR pszOnlyFileName;              /* parm for searchpath        */
    LPTSTR lpPath = NULL;                /* ptr to search path+        */
    UINT   errorMode;

    char opt = 'C'; // this is the default
    if (options != NULL)
    {
        opt = toupper(options[0]);
        if (opt != 'C' && opt != 'N')
        {
            context->InvalidRoutine();
            return NULL;
        }
    }

    szEnvStr = (LPTSTR) malloc(sizeof(char) * MAX_ENVVAR);
    if (szEnvStr != NULL)
    {
        DWORD charCount = GetEnvironmentVariable(path, szEnvStr, MAX_ENVVAR);
        if (charCount == 0)
        {
            *szEnvStr = '\0';
        }
        else if (charCount > MAX_ENVVAR)
        {
            szEnvStr = (LPTSTR) realloc(szEnvStr, sizeof(char) * charCount);
            if (szEnvStr != NULL)
            {
                DWORD charCount2 = GetEnvironmentVariable(args[0].strptr, szEnvStr, charCount);
                if (charCount2 == 0 || charCount2 > charCount)
                {
                    *szEnvStr = '\0';
                }
            }
        }
    }

    if (opt == 'N')
    {
        lpPath = (szEnvStr == NULL) ? NULL : strdup(szEnvStr);
    }
    else if (opt == 'C')
    {
        /* search current directory   */
        DWORD charCount = GetCurrentDirectory(_MAX_PATH, szCurDir);
        if (charCount == 0 || charCount > _MAX_PATH)
        {
            szCurDir[0] = '\0';
        }

        if (szEnvStr != NULL)
        {
            lpPath = (LPTSTR) malloc(sizeof(char) * (strlen(szCurDir) + 1 + strlen(szEnvStr) + 1));
            if (lpPath != NULL)
            {
                strcpy(lpPath, szCurDir);
                strcat(lpPath, ";");
                strcat(lpPath, szEnvStr);
            }
        }
        else
        {
            lpPath = strdup(szCurDir);
        }
    }

    /* use DosSearchPath          */

    DWORD charCount = 0;
    if (lpPath != NULL)
    {
        errorMode = SetErrorMode(SEM_FAILCRITICALERRORS);
        charCount = SearchPath(
                           (LPCTSTR)lpPath,              /* path srch, NULL will+      */
                           (LPCTSTR)file,                /* address if filename        */
                           NULL,                         /* filename contains .ext     */
                           _MAX_PATH,                    /* size of fullname buffer    */
                           szFullPath,                   /* where to put results       */
                           &pszOnlyFileName);
        SetErrorMode(errorMode);
    }
    if (charCount == 0 || charCount > _MAX_PATH)
    {
        szFullPath[0]='\0';              /* set to NULL if failure     */
    }

    RexxStringObject retstr = context->NewStringFromAsciiz(szFullPath);
    free(szEnvStr);
    free(lpPath);
    return retstr;
}


/*************************************************************************
* Function:  SysSleep                                                    *
*                                                                        *
* Syntax:    call SysSleep secs                                          *
*                                                                        *
* Params:    secs - Number of seconds to sleep.                          *
*                   must be in the range 0 .. 2147483                    *
*                                                                        *
* Return:    0                                                           *
*************************************************************************/
RexxRoutine1(int, SysSleep, RexxStringObject, delay)
{
  double seconds;
  // try to convert the provided delay to a valid floating point number
  if (context->ObjectToDouble(delay, &seconds) == 0 ||
      isnan(seconds) || seconds == HUGE_VAL || seconds == -HUGE_VAL)
  {
      // 88.902 The &1 argument must be a number; found "&2"
      context->RaiseException2(Rexx_Error_Invalid_argument_number, context->String("delay"), delay);
      return 1;
  }

  // according to MSDN the maximum is USER_TIMER_MAXIMUM (0x7FFFFFFF) milliseconds,
  // which translates to 2147483.647 seconds
  if (seconds < 0.0 || seconds > 2147483.0)
  {
      // 88.907 The &1 argument must be in the range &2 to &3; found "&4"
      context->RaiseException(Rexx_Error_Invalid_argument_range,
          context->ArrayOfFour(context->String("delay"),
          context->String("0"), context->String("2147483"), delay));
      return 1;
  }

  // convert to milliseconds, no overflow possible
  LONG milliseconds = (LONG) (seconds * 1000);

  /** Using Sleep with a long timeout risks sleeping on a thread with a message
   *  queue, which can make the system sluggish, or possibly deadlocked.  If the
   *  sleep is longer than 333 milliseconds use a window timer to avoid this
   *  risk.
   */
  if ( milliseconds > 333 )
  {
      if ( !(SetTimer(NULL, 0, milliseconds, (TIMERPROC) SleepTimerProc)) )
      {
          // no timer available, need to abort
          context->RaiseException1(Rexx_Error_System_resources_user_defined,
              context->String("System resources exhausted: cannot start timer"));
          return 1;
      }

      MSG msg;
      while ( GetMessage (&msg, NULL, 0, 0) )
      {
          if ( msg.message == OM_WAKEUP )  /* If our message, exit loop       */
              break;
          TranslateMessage( &msg );
          DispatchMessage ( &msg );
      }
  }
  else
  {
      Sleep(milliseconds);
  }

  return 0;
}

/*********************************************************************
 *                                                                   *
 *  Routine   : SleepTimerProc                                       *
 *                                                                   *
 *  Purpose   : callback routine for SetTimer set in SysSleep        *
 *  Notes     :                                                      *
 *  Arguments : hwnd - window handle                                 *
 *              uMsg - WM_TIMER message                              *
 *              idEvent - timer identifier                           *
 *              dwtime - current system time                         *
 *  Returns   :                                                      *
 *                                                                   *
 *********************************************************************/
 VOID CALLBACK SleepTimerProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime) {
   DWORD ThreadId;
   KillTimer(NULL, idEvent);       /* kill the timer that just ended */
   ThreadId = GetCurrentThreadId();
   PostThreadMessage(ThreadId, OM_WAKEUP, 0 , 0L); /* send ourself the wakeup message*/
 }

/*************************************************************************
* Function:  SysTempFileName                                             *
*                                                                        *
* Syntax:    call SysTempFileName template [,filler]                     *
*                                                                        *
* Params:    template - Description of filespec desired.  For example:   *
*                        C:\TEMP\FILE.???                                *
*            filler   - A character which when found in template will be *
*                        replaced with random digits until a unique file *
*                        or directory is found.  The default character   *
*                        is '?'.                                         *
*                                                                        *
* Return:    other - Unique file/directory name.                         *
*            ''    - No more files exist given specified template.       *
*************************************************************************/

RexxRoutine2(RexxStringObject, SysTempFileName, CSTRING, template, OPTIONAL_CSTRING, fillerOpt)
{
    CHAR   filler = '?';                     /* filler character           */

    if (fillerOpt != NULL)
    {
        if (strlen(fillerOpt) != 1)
        {
            context->InvalidRoutine();
            return NULLOBJECT;
        }
        filler = fillerOpt[0];
    }

    char retstr[PATH_MAX];
    /* get the file id            */
    GetUniqueFileName(const_cast<char *>(template), filler, retstr);

    return context->NewStringFromAsciiz(retstr);
}


/*************************************************************************
* Function:  SysTextScreenRead                                           *
*                                                                        *
* Syntax:    call SysTextScreenRead row, col [,len]                      *
*                                                                        *
* Params:    row - Horizontal row on the screen to start reading from.   *
*                   The row at the top of the screen is 0.               *
*            col - Vertical column on the screen to start reading from.  *
*                   The column at the left of the screen is 0.           *
*            len - The number of characters to read.  The default is the *
*                   rest of the screen.                                  *
*                                                                        *
* Return:    Characters read from text screen.                           *
*************************************************************************/
RexxRoutine3(RexxStringObject, SysTextScreenRead, int, row, int, col, OPTIONAL_int, len)
{
    int    lPos,lPosOffSet;              /* positioning                */
                                         /* (132x50)                   */
    int    lBufferLen = 16000;           /* default: 200x80 characters */

    COORD coordLine;                     /* coordinates of where to    */
                                         /* read characters from       */
    DWORD dwCharsRead,dwSumCharsRead;    /* Handle to Standard Out     */
    HANDLE hStdout;
    CONSOLE_SCREEN_BUFFER_INFO csbiInfo; /* Console information        */

    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

    if (!GetConsoleScreenBufferInfo(hStdout, &csbiInfo))
    {
        context->InvalidRoutine();
        return NULLOBJECT;
    }

    if (argumentOmitted(3))               /* check the length           */
    {
        len = csbiInfo.dwSize.Y * csbiInfo.dwSize.X;
    }

    coordLine.X = (short)col;
    coordLine.Y = (short)row;

    char buffer[256];
    char *ptr = buffer;

    if (len > sizeof(buffer))
    {
        // allocate a new buffer
        ptr = (char *)malloc(len);
        if (ptr == NULL)
        {
            context->InvalidRoutine();
            return NULL;
        }
    }

    if (len < lBufferLen)
    {
        lBufferLen = len;
    }

    lPos = 0;                                     /* current position */
    lPosOffSet = row * csbiInfo.dwSize.X + col;   /* add offset if not started at beginning */
    dwSumCharsRead = 0;

    while (lPos < len )
    {

        if (!ReadConsoleOutputCharacter(hStdout, &ptr[lPos], lBufferLen, coordLine, &dwCharsRead))
        {
            if (ptr != buffer) {
                free(ptr);
            }
            context->InvalidRoutine();
            return NULL;
        }


        lPos = lPos + lBufferLen;
        coordLine.Y = (short)((lPos + lPosOffSet) / csbiInfo.dwSize.X);
        coordLine.X = (short)((lPos + lPosOffSet) % csbiInfo.dwSize.X);
        dwSumCharsRead = dwSumCharsRead + dwCharsRead;
    }

    RexxStringObject result = context->NewString(ptr, dwSumCharsRead);
    if (ptr != buffer) {
        free(ptr);
    }
    return result;
}

/*************************************************************************
* Function:  SysTextScreenSize                                           *
*                                                                        *
* Syntax:    call SysTextScreenSize [option], [rows, colummns]          *
*            call SysTextScreenSize [option], [top, left, bottom, right] *
*                                                                        *
* Params:    option - "BUFFERSIZE", "WINDOWRECT", "MAXWINDOWSIZE"        *
*               "BUFFERSIZE" (default) return or set console buffer size *
*               "WINDOWRECT" return or set windows position              *
*               "MAXWINDOWSIZE" return maximum window size               *
*            lines, columns - set buffer size to lines by columns        *
*            top, left, bottom, right - set window size and position     *
*                                                                        *
* Return:    "BUFFERSIZE" or "MAXWINDOWSIZE": rows columns               *
*            "WINDOWRECT": top left bottom right                         *
*************************************************************************/

RexxRoutine5(RexxStringObject, SysTextScreenSize,
    OPTIONAL_CSTRING, optionString,
    OPTIONAL_stringsize_t, rows, OPTIONAL_stringsize_t, columns,
    OPTIONAL_stringsize_t, rows2, OPTIONAL_stringsize_t, columns2)
{
    // check for valid option
    typedef enum { BUFFERSIZE, WINDOWRECT, MAXWINDOWSIZE } console_option;
    console_option option;
    if (optionString == NULL || stricmp(optionString, "BUFFERSIZE") == 0)
    {
        option = BUFFERSIZE;
    }
    else if (stricmp(optionString, "WINDOWRECT") == 0)
    {
        option = WINDOWRECT;
    }
    else if (stricmp(optionString, "MAXWINDOWSIZE") == 0)
    {
        option = MAXWINDOWSIZE;
    }
    else
    {
        context->InvalidRoutine();
        return 0;
    }

    // check for valid SET arguments: either none, or two more, or four more
    size_t setArgs;
    bool omitted45 = argumentOmitted(4) && argumentOmitted(5);
    bool exists23 = argumentExists(2) && argumentExists(3);
    if (argumentOmitted(2) && argumentOmitted(3) && omitted45)
    {
        setArgs = 0;
    }
    else if (exists23 && omitted45)
    {
        setArgs = 2;
    }
    else if (exists23 && argumentExists(4) && argumentExists(5))
    {
        setArgs = 4;
    }
    else
    {
        context->InvalidRoutine();
        return 0;
    }

    // check that all SET arguments fit a SHORT
    if (!(setArgs == 0 ||
         (setArgs == 2 && rows <= SHRT_MAX && columns <= SHRT_MAX) ||
         (setArgs == 4 && rows <= SHRT_MAX && columns <= SHRT_MAX && rows2 <= SHRT_MAX && columns2 <= SHRT_MAX)))
    {
        context->InvalidRoutine();
        return 0;
    }

    // real work starts here
    CONSOLE_SCREEN_BUFFER_INFO csbi; // console screen buffer information
    char buffer[100];

    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

    if (setArgs == 0)
    {
        // this is a GET requset, retrieve console information
        if (GetConsoleScreenBufferInfo(hStdout, &csbi) == NULL)
        {
            // console not in character mode, return two or four zeroes
            return context->NewStringFromAsciiz(option == WINDOWRECT ? "0 0 0 0" : "0 0");
        }
    }

    if (option == BUFFERSIZE && setArgs == 0)
    {
        // this is a BUFFERSIZE GET, returns two values
        sprintf(buffer, "%d %d", csbi.dwSize.Y, csbi.dwSize.X);
    }
    else if (option == WINDOWRECT && setArgs == 0)
    {
        // this is a WINDOWRECT GET, returns four values
        sprintf(buffer, "%d %d %d %d", csbi.srWindow.Top, csbi.srWindow.Left, csbi.srWindow.Bottom, csbi.srWindow.Right);
    }
    else if (option == MAXWINDOWSIZE && setArgs == 0)
    {
        // this is a MAXWINDOWSIZE GET, returns two values
        sprintf(buffer, "%d %d", csbi.dwMaximumWindowSize.Y, csbi.dwMaximumWindowSize.X);
    }
    else if (option == BUFFERSIZE && setArgs == 2)
    {
        // this is a BUFFERSIZE SET, requires two more arguments
        COORD consoleBuffer;
        consoleBuffer.Y = (SHORT)rows;
        consoleBuffer.X = (SHORT)columns;
        BOOL code = SetConsoleScreenBufferSize(hStdout, consoleBuffer);
        sprintf(buffer, "%d", code == 0 ? GetLastError() : 0);
    }
    else if (option == WINDOWRECT  && setArgs == 4)
    {
        // this is a WINDOWRECT  SET, requires four more arguments
        SMALL_RECT consoleWindow;
        consoleWindow.Top =    (SHORT)rows;
        consoleWindow.Left =   (SHORT)columns;
        consoleWindow.Bottom = (SHORT)rows2;
        consoleWindow.Right =  (SHORT)columns2;
        BOOL code = SetConsoleWindowInfo(hStdout, 1, &consoleWindow);
        sprintf(buffer, "%d", code == 0 ? GetLastError() : 0);
    }
    else
    {
        context->InvalidRoutine();
        return 0;
    }

    // return the buffer as result
    return context->NewStringFromAsciiz(buffer);
}

/*************************************************************************
* Function:  RxWinExec                                                   *
*                                                                        *
* Syntax:    call RxWinExec command,CmdShow                              *
*                                                                        *
*                                                                        *
* Parms:     command    - program to execute                             *
*            CmdShow    - Any of the SW_ type values in winuser.h        *
*                         SW_SHOW                 5                      *
*                         SW_HIDE                 0                      *
*                         SW_MINIMIZE etc...      6                      *
*                         numeric values...                              *
*                                                                        *
* Return:    Process ID or Error code                                    *
*************************************************************************/

RexxRoutine2(uint32_t, RxWinExec, CSTRING, command, OPTIONAL_CSTRING, show)
{
    int         CmdShow = SW_SHOWNORMAL; /* show window style flags    */
    int         index;                   /* table index                */
    ULONG       pid;                     /* PID or error return code   */
    size_t      length;                  /* length of option           */
    STARTUPINFO si;
    PROCESS_INFORMATION procInfo;

    // Show window types.
    PSZ    show_styles[] =
    {
        "SHOWNORMAL",
        "SHOWNOACTIVATE",
        "SHOWMINNOACTIVE",
        "SHOWMINIMIZED",
        "SHOWMAXIMIZED",
        "HIDE",
        "MINIMIZE"
    };

    // Show window styles.
    ULONG  show_flags[] =
    {
        SW_SHOWNORMAL,
        SW_SHOWNOACTIVATE,
        SW_SHOWMINNOACTIVE,
        SW_SHOWMINIMIZED,
        SW_SHOWMAXIMIZED,
        SW_HIDE,
        SW_MINIMIZE
    };

    if (strlen(command) > MAX_CREATEPROCESS_CMDLINE)
    {
        context->InvalidRoutine();
        return 0;
    }
    // Show type can be one and only one of the SW_XXX constants.
    if (show != NULL)
    {
        // Get length of option and search the style table.

        for ( index = 0; index < sizeof(show_styles)/sizeof(PSZ); index++ )
        {
            if ( _stricmp(show, show_styles[index]) == 0)
            {
                CmdShow = show_flags[index];
                break;
            }
        }

        if ( index == sizeof(show_styles)/sizeof(PSZ) )
        {
            context->InvalidRoutine();
            return 0;
        }
    }

    ZeroMemory(&procInfo, sizeof(procInfo));
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = (WORD)CmdShow;

    if ( CreateProcess(NULL, (LPSTR)args[0].strptr, NULL, NULL, FALSE, 0, NULL,
                       NULL, &si, &procInfo ) )
    {
        pid = procInfo.dwProcessId;
    }
    else
    {
        pid = GetLastError();
        if ( pid > 31 )
        {
            // Maintain compatibility to versions < ooRexx 3.1.2
            pid = (ULONG)-((int)pid);
        }
    }

    // Close process / thread handles as they are not used / needed.
    CloseHandle(procInfo.hProcess);
    CloseHandle(procInfo.hThread);

    return pid;
}


/*************************************************************************
* Function:  SysBootDrive                                                *
*                                                                        *
* Syntax:    drive = SysBootDrive()                                      *
*                                                                        *
* Params:    none                                                        *
*                                                                        *
* Return:    'A: B: C: D: ...'                                           *
*************************************************************************/

RexxRoutine0(RexxStringObject, SysBootDrive)
{
    char retstr[PATH_MAX];

    if (GetSystemDirectory(retstr, sizeof(retstr)) > 0)
    {
        return context->NewString(retstr, 1);
    }
    else
    {
       return context->NullString();
    }
}


/*************************************************************************
* Function:  SysSystemDirectory                                          *
*                                                                        *
* Syntax:    drive = SysSystemDirectory()                                *
*                                                                        *
* Params:    none                                                        *
*                                                                        *
* Return:    'C:\WINDOWS ...'                                            *
*************************************************************************/

RexxRoutine0(RexxStringObject, SysSystemDirectory)
{
    char retstr[PATH_MAX];

    if (GetSystemDirectory(retstr, sizeof(retstr)) > 0)
    {
        return context->NewStringFromAsciiz(retstr);
    }
    else
    {
       return context->NullString();
    }
}


/*************************************************************************
* Function:  SysFileSystemType                                           *
*                                                                        *
* Syntax:    result = SysFileSystemType("drive")                         *
*                                                                        *
* Params:    drive - drive letter (in form of 'D:')                      *
*        or  none - current drive                                        *
*                                                                        *
* Return:    result - File System Name attached to the specified drive   *
*                     (FAT, HPFS ....)                                   *
*            '' - Empty string in case of any error                      *
*************************************************************************/

RexxRoutine1(RexxStringObject, SysFileSystemType, OPTIONAL_CSTRING, drive)
{
    CHAR chDriveLetter[4];
    UINT errorMode;

    if (drive != NULL)
    {
        size_t driveLen = strlen(drive);

        if (driveLen == 0 || driveLen > 2 || driveLen == 2 && drive[1] != ':'))
        {
            context->InvalidRoutine();
            return NULLOBJECT;
        }
        snprintf(chDriveLetter, sizeof(chDriveLetter), "%c:\\", drive[0]);
        drive = chDriveLetter;
    }

    errorMode = SetErrorMode(SEM_FAILCRITICALERRORS);

    char fileSystemName[PATH_MAX];

    RexxStringObject result = context->NullString();

    if (GetVolumeInformation(
            drive,    // address of root directory of the file system
            NULL,    // address of name of the volume
            0,    // length of lpVolumeNameBuffer
            NULL,    // address of volume serial number
            NULL,    // address of system's maximum filename length
            NULL,    // address of file system flags
            fileSystemName,    // address of name of file system
            sizeof(FileSystemName)     // length of lpFileSystemNameBuffer
            ))
    {
        result = context->NewStringFromAsciiz(fileSystemName);
    }

    SetErrorMode(errorMode);
    return result;
}


/*************************************************************************
* Function:  SysVolumeLabel                                              *
*                                                                        *
* Syntax:    result = SysVolumeLabel("drive")                            *
*                                                                        *
* Params:    drive - drive letter (in form of 'D:')                      *
*        or  none - current drive                                        *
*                                                                        *
* Return     '' - Empty string in case of any error                      *
*************************************************************************/

RexxRoutine1(RexxStringObject, SysVolumeLabel, OPTIONAL_CSTRING, drive)
{
    CHAR chDriveLetter[4];
    if (drive != NULL)
    {
        size_t driveLen = strlen(drive);

        if (driveLen == 0 || driveLen > 2 || (driveLen == 2 && drive[1] != ':'))
        {
            context->InvalidRoutine();
            return NULLOBJECT;
        }
        snprintf(chDriveLetter, sizeof(chDriveLetter), "%c:\\", drive[0]);
        drive = chDriveLetter;
    }
    char volumeName[PATH_MAX];

    RexxStringObject result = context->NullString();

    if (GetVolumeInformation(
            drive,           /* address of root directory of the file system */
            volumeName,                      /*address of name of the volume */
            sizeof(volumeName),              /* length of lpVolumeNameBuffer */
            NULL,                         /* address of volume serial number */
            NULL,             /* address of system's maximum filename length */
            NULL,                            /* address of file system flags */
            NULL,                          /* address of name of file system */
            0                            /* length of lpFileSystemNameBuffer */
            ))
    {
        result = context->NewStringFromAsciiz(fileSystemName);
    }

    return result;
}


/*************************************************************************
* Function:  SysCreateMutexSem                                           *
*                                                                        *
* Syntax:    handle = SysCreateMutexSem(<name>)                          *
*                                                                        *
* Params:    name  - optional name for a mutex semaphore                 *
*                                                                        *
* Return:    handle - token used as a mutex handle for                   *
*                     SysRequestMutexSem, SysReleaseMutexSem,            *
*                     SysCloseMutexSem, and SysOpenEventSem              *
*            '' - Empty string in case of any error                      *
*************************************************************************/

RexxRoutine1(RexxObjectPtr, SysCreateMutexSem, OPTIONAL_CSTRING, name)
{
    HANDLE    handle;                    /* mutex handle               */
    SECURITY_ATTRIBUTES sa={sizeof(SECURITY_ATTRIBUTES), NULL, true};

    handle = 0;                          /* zero the handle            */
    if (name != NULL)                  /* request for named sem      */
    {
        /* create it by name          */
        handle = CreateMutex(&sa, false, name);
        if (!handle)                            /* may already be created     */
        {
            /* try to open it             */
            handle = OpenMutex(MUTEX_ALL_ACCESS, true, name);
        }
    }
    else                                 /* unnamed semaphore          */
    {
        handle = CreateMutex(&sa, false, NULL);
    }

    if (handle == NULL) {
        return context->NullString();
    }

    return context->Uintptr((uintptr_t)handle);
}


/*************************************************************************
* Function:  SysOpenMutexSem                                             *
*                                                                        *
* Syntax:    result = SysOpenMutexSem(name)                              *
*                                                                        *
* Params:    name - name of the mutex semaphore                          *
*                                                                        *
* Return:    result - handle to the mutex                                *
*************************************************************************/

RexxRoutine1(uintptr_t, SysOpenMutexSem, CSTRING, name)
{
    SECURITY_ATTRIBUTES sa={sizeof(SECURITY_ATTRIBUTES), NULL, true};

                                       /* get a binary handle        */
    return (uintptr_t) OpenMutex(MUTEX_ALL_ACCESS, true, name); /* try to open it             */
}

/*************************************************************************
* Function:  SysReleaseMutexSem                                          *
*                                                                        *
* Syntax:    result = SysReleaseMutexSem(handle)                         *
*                                                                        *
* Params:    handle - token returned from SysCreateMutexSem              *
*                                                                        *
* Return:    result - return code from ReleaseMutex                      *
*************************************************************************/

RexxRoutine1(int, SysReleaseMutexSem, uintptr_t, h)
{
    return !ReleaseMutex((HANDLE)h) ? GetLastError() : 0;
}

/*************************************************************************
* Function:  SysCloseMutexSem                                            *
*                                                                        *
* Syntax:    result = SysCloseMutexSem(handle)                           *
*                                                                        *
* Params:    handle - token returned from SysCreateMutexSem              *
*                                                                        *
* Return:    result - return code from CloseHandle                       *
*************************************************************************/

RexxRoutine1(int, SysCloseMutexSem, uintptr_t, h)
{
    return !ReleaseMutex((HANDLE)h) ? GetLastError() : 0;
}

/*************************************************************************
* Function:  SysRequestMutexSem                                          *
*                                                                        *
* Syntax:    result = SysRequestMutexSem(handle, <timeout>)              *
*                                                                        *
* Params:    handle - token returned from SysCreateMutexSem              *
*                                                                        *
* Return:    result - return code from WaitForSingleObject               *
*************************************************************************/

RexxRoutine2(int, SysRequestMutexSem, uintptr_t, h, OPTIONAL_int, timeout)
{
    if (argumentOmitted(2))
    {
        timeout = INFINITE;       /* default is no timeout      */
    }
    int rc = WaitForSingleObject((HANDLE)h, timeout);
    if (rc == WAIT_FAILED)
    {
        return GetLastError();
    }
    else
    {
        return rc;                         /* format the return code     */
    }
}

/*************************************************************************
* Function:  SysCreateEventSem                                           *
*                                                                        *
* Syntax:    handle = SysCreateEventSem(<name>,<manual>)                 *
*                                                                        *
* Params:    name  - optional name for a event semaphore                 *
*            any second argument means manual reset event                *
* Return:    handle - token used as a event sem handle for               *
*                     SysPostEventSem, SysClearEventSem,                 *
*                     SysCloseEventSem, and SysOpenEventSem              *
*            '' - Empty string in case of any error                      *
*************************************************************************/

RexxRoutine2(RexxObjectPtr, SysCreateEventSem, OPTIONAL_CSTRING, name, OPTIONAL_CSTRING, reset)
{
    HANDLE    handle;                    /* mutex handle               */
    SECURITY_ATTRIBUTES sa={sizeof(SECURITY_ATTRIBUTES), NULL, true};
    bool      manual;

    handle = 0;                          /* zero the handle            */
    if (reset != NULL)
    {
        manual = true;
    }
    else
    {
        manual = false;
    }

    if (name != NULL)
    {                                    /* request for named sem      */
                                         /* create it by name          */
        handle = CreateEvent(&sa, manual, false, name);
        if (!handle)                       /* may already be created     */
        {
                                           /* try to open it             */
            handle = OpenEvent(EVENT_ALL_ACCESS, true, name);
        }
    }
    else                                 /* unnamed semaphore          */
    {
        handle = CreateEvent(&sa, manual, false, NULL);
    }

    if (handle == NULL) {
        return context->NullString();
    }

    return context->Uintptr((uintptr_t)handle);
}

/*************************************************************************
* Function:  SysOpenEventSem                                             *
*                                                                        *
* Syntax:    result = SysOpenEventSem(name)                              *
*                                                                        *
* Params:    name - name of the event semaphore                          *
*                                                                        *
* Return:    result - return code from OpenEvent                         *
*************************************************************************/

RexxRoutine1(uintptr_t, SysOpenEventSem, CSTRING, name)
{
    SECURITY_ATTRIBUTES sa={sizeof(SECURITY_ATTRIBUTES), NULL, true};

                                       /* get a binary handle        */
    return (uintptr_t)OpenEvent(EVENT_ALL_ACCESS, true, name); /* try to open it             */
}

/*************************************************************************
* Function:  SysPostEventSem                                             *
*                                                                        *
* Syntax:    result = SysPostEventSem(handle)                            *
*                                                                        *
* Params:    handle - token returned from SysCreateEventSem              *
*                                                                        *
* Return:    result - return code from SetEvent                          *
*************************************************************************/

RexxRoutine1(int, SysPostEventSem, uintptr_t, h)
{
    return !SetEvent((HANDLE)h) ? GetLastError() : 0;
}

/*************************************************************************
* Function:  SysResetEventSem                                            *
*                                                                        *
* Syntax:    result = SysResetEventSem(handle)                           *
*                                                                        *
* Params:    handle - token returned from SysCreateEventSem              *
*                                                                        *
* Return:    result - return code from ResetEvent                        *
*************************************************************************/

RexxRoutine1(int, SysResetEventSem, uintptr_t, h)
{
    return !ResetEvent((HANDLE)h) ? GetLastError() : 0;
}


/*************************************************************************
* Function:  SysPulseEventSem                                            *
*                                                                        *
* Syntax:    result = SysPulseEventSem(handle)                           *
*                                                                        *
* Params:    handle - token returned from SysCreateEventSem              *
*                                                                        *
* Return:    result - return code from PulseEvent                        *
*************************************************************************/

RexxRoutine1(int, SysPulseEventSem, uintptr_t, h)
{
    return !PulseEvent((HANDLE)h) ? GetLastError() : 0;
}


/*************************************************************************
* Function:  SysCloseEventSem                                            *
*                                                                        *
* Syntax:    result = SysCloseEventSem(handle)                           *
*                                                                        *
* Params:    handle - token returned from SysCreateEventSem              *
*                                                                        *
* Return:    result - return code from CloseHandle                       *
*************************************************************************/

RexxRoutine1(int, SysCloseEventSem, uintptr_t, h)
{
    return !CloseHandle((HANDLE)h) ? GetLastError() : 0;
}

/*************************************************************************
* Function:  SysWaitEventSem                                             *
*                                                                        *
* Syntax:    result = SysWaitEventSem(handle, <timeout>)                 *
*                                                                        *
* Params:    handle - token returned from SysWaitEventSem                *
*                                                                        *
* Return:    result - return code from WaitForSingleObject               *
*************************************************************************/

RexxRoutine2(int, SysWaitEventSem, uintptr_t, h, OPTIONAL_int, timeout)
{
    if (!argumentExists(2))
    {
        timeout = INFINITE;       /* default is no timeout      */
    }
    /* request the semaphore      */
    int rc = WaitForSingleObject((HANDLE)h, timeout);
    if (rc == WAIT_FAILED)
    {
        return GetLastError();
    }
    else
    {
        return rc;                         /* format the return code     */
    }
}


/*************************************************************************
* Function:  SysSetPriority                                              *
*                                                                        *
* Syntax:    result = SysSetPriority(Class, Level)                       *
*                                                                        *
* Params: Class - The priority class (0-3 or HIGH,REALTIME,NORMAL,IDLE)  *
*         Level  - Amount to change (-15 to +15 or IDLE, LOWEST,...)     *
*                                                                        *
*************************************************************************/

RexxRoutine2(int, SysSetPriority, RexxObjectPtr, classArg, RexxObjectPtr, levelArg)
{
    HANDLE process = GetCurrentProcess();
    HANDLE thread = GetCurrentThread();

    DWORD     iclass=-1;
    wholenumber_t classLevel;               /* priority class             */
    if (context->WholeNumber(classArg, &classLevel))
    {
        switch (classLevel)
        {
            case 0: iclass = IDLE_PRIORITY_CLASS;
                break;
            case 1: iclass = NORMAL_PRIORITY_CLASS;
                break;
            case 2: iclass = HIGH_PRIORITY_CLASS;
                break;
            case 3: iclass = REALTIME_PRIORITY_CLASS;
            default:
                context->InvalidRoutine();
                return 0;
        }
    }
    else
    {
        const char *classStr = context->ObjectToStringValue(classArg);

        if (stricmp(classStr, "REALTIME") == 0)
        {
            iclass = REALTIME_PRIORITY_CLASS;
        }
        else if (stricmp(classStr, "HIGH") == 0)
        {
            iclass = HIGH_PRIORITY_CLASS;
        }
        else if (!stricmp(classStr, "NORMAL") == 0)
        {
            iclass = NORMAL_PRIORITY_CLASS;
        }
        else if (stricmp(classStr, "IDLE") == 0)
        {
            iclass = IDLE_PRIORITY_CLASS;
        }
        else
        {
            context->InvalidRoutine();
            return 0;
        }
    }


    wholenumber_t level;                    /* priority level             */
    if (context->WholeNumber(levelArg, &level))
    {
        if (level < -15 || level > 15)
        {
            context->InvalidRoutine();
            return 0;
        }
    }
    else
    {
        const char *levelStr = context->ObjectToStringValue(levelArg);

        if (stricmp(levelStr, "ABOVE_NORMAL") == 0)
        {
            level = THREAD_PRIORITY_ABOVE_NORMAL;
        }
        else if (stricmp(levelStr, "BELOW_NORMAL") == 0)
        {
            level = THREAD_PRIORITY_BELOW_NORMAL;
        }
        else if (stricmp(levelStr, "HIGHEST") == 0)
        {
            level = THREAD_PRIORITY_HIGHEST;
        }
        else if (stricmp(levelStr, "LOWEST") == 0)
        {
            level = THREAD_PRIORITY_LOWEST;
        }
        else if (stricmp(levelStr, "NORMAL") == 0)
        {
            level = THREAD_PRIORITY_NORMAL;
        }
        else if (stricmp(levelStr, "IDLE") == 0)
        {
            level = THREAD_PRIORITY_IDLE;
        }
        else if (stricmp(levelStr, "TIME_CRITICAL") == 0)
        {
            level = THREAD_PRIORITY_TIME_CRITICAL;
        }
        else
        {
            context->InvalidRoutine();
            return 0;
        }
    }

    int rc = SetPriorityClass(process, iclass);
    if (rc)
    {
        rc = SetThreadPriority(thread, (int)level);
    }

    return rc != 0 ? 0 : GetLastError();
}


/*************************************************************************
* Function:  SysQueryProcess                                             *
*                                                                        *
* Params:    "PID" - (default) returns current process ID                *
*            "TID" - (default) returns current thread ID                 *
*            "PPRIO" - (default) returns current process priority        *
*            "TPRIO" - (default) returns current thread priority         *
*            "PTIME" - (default) returns current process times           *
*            "TTIME" - (default) returns current thread times            *
*************************************************************************/

RexxRoutine1(RexxObjectPtr, SysQueryProcess, OPTIONAL_CSTRING, option)
{
    if (option == NULL || stricmp(option, "PID") == 0)
    {
        return context->WholeNumber(GetCurrentProcessId());
    }
    if (stricmp(option, "TID") == 0)
    {
        return context->WholeNumber(GetCurrentThreadId());
    }
    if (stricmp(option, "PPRIO") == 0)
    {
        LONG p;
        p = GetPriorityClass(GetCurrentProcess());

        switch (p)
        {
            case HIGH_PRIORITY_CLASS:
                return context->String("HIGH");
            case IDLE_PRIORITY_CLASS:
                return context->String("IDLE");
            case NORMAL_PRIORITY_CLASS:
                return context->String("NORMAL");
            case REALTIME_PRIORITY_CLASS:
                return context->String("REALTIME");
            default:
                return context->String("UNKNOWN");
        }
    }
    if (stricmp(option, "TPRIO") == 0)
    {
        LONG p;
        p = GetThreadPriority(GetCurrentThread());

        switch (p)
        {
            case THREAD_PRIORITY_ABOVE_NORMAL:
                return context->String("ABOVE_NORMAL");
            case THREAD_PRIORITY_BELOW_NORMAL:
                return context->String("BELOW_NORMAL");
            case THREAD_PRIORITY_HIGHEST:
                return context->String("HIGHEST");
            case THREAD_PRIORITY_IDLE:
                return context->String("IDLE");
            case THREAD_PRIORITY_LOWEST:
                return context->String("LOWEST");
                break;
            case THREAD_PRIORITY_NORMAL:
                return context->String("NORMAL");
                break;
            case THREAD_PRIORITY_TIME_CRITICAL:
                return context->String("TIME_CRITICAL");
            default:
                return context->String("UNKNOWN");
        }
    }
    if (stricmp(option, "PTIME") == 0 || stricmp(option, "TTIME") == 0)
    {
        FILETIME createT, kernelT, userT, dummy;
        SYSTEMTIME createST, kernelST, userST;
        BOOL ok;

        if (*option == 'T' || *option == 't')
        {
            ok = GetThreadTimes(GetCurrentThread(), &createT,&dummy,&kernelT, &userT);
        }
        else
        {
            ok = GetProcessTimes(GetCurrentProcess(), &createT,&dummy,&kernelT, &userT);
        }

        if (ok)
        {
            FileTimeToLocalFileTime(&createT, &createT);
            FileTimeToSystemTime(&createT, &createST);
            FileTimeToSystemTime(&kernelT, &kernelST);
            FileTimeToSystemTime(&userT, &userST);

            char buffer[256];

            wsprintf(buffer, "Create: %4.4d/%2.2d/%2.2d %d:%2.2d:%2.2d:%3.3d  "\
                     "Kernel: %d:%2.2d:%2.2d:%3.3d  User: %d:%2.2d:%2.2d:%3.3d",
                     createST.wYear,createST.wMonth,createST.wDay,createST.wHour,createST.wMinute,
                     createST.wSecond,createST.wMilliseconds,
                     kernelST.wHour,kernelST.wMinute,kernelST.wSecond,kernelST.wMilliseconds,
                     userST.wHour,userST.wMinute,userST.wSecond,userST.wMilliseconds);

            return context->String(buffer);
        }
    }
    context->InvalidRoutine();
    return NULLOBJECT;
}


/** SysShutDownSystem()
 *
 *  Interface to the InitiateSystemShutdown() API on Windows.
 *
 *  @param  computer         The name of the computer to shut down.  If omitted
 *                           or the empty string, the local machine is shut
 *                           down.  Otherwise this is the name of a remote
 *                           machine to shut down.
 *  @param  message          If timout is not 0, a shut down dialog is displayed
 *                           on the machine being shut down, naming the user who
 *                           initiated the shut down, a timer counting down the
 *                           seconds until the machine is shut down, and
 *                           prompting the local user to log off. This parametr
 *                           can be an additional message to add to the dialog
 *                           box.  It can be ommitted if no additional message
 *                           is desired.
 *  @param  timeout          Number of seconds to display the shut down dialog.
 *                           If this is 0 no dialog is displayed.  The default
 *                           is 30 seconds, see the remarks below.  The user can
 *                           force a 0 timeout by explicitly specifying 0.
 *  @param  forceAppsClosed  If true applications with unsaved data are forcibly
 *                           closed.  If false, the user is presented with a
 *                           dialog telling the user to close the applcation(s).
 *  @param  reboot           If true, the system is rebooted, if false the
 *                           system is shut down.
 *
 *  @remarks Note that prior to 4.0.0, the defaults for all arguments were
 *           exactly what the value of each parameter is if omitted.
 *
 *           machine         == NULL
 *           message         == NULL
 *           timeout         == 0
 *           forceAppsClosed == false
 *           reboot          == false
 *
 *           Because of this, there would be no need to check if any argument is
 *           ommitted or not.  However, the consequences of having a 0 timeout
 *           value are severe if the system has an application open with unsaved
 *           data.  Therefore for 4.0.0 and on the default time out value is
 *           changed to 30 (seconds.)
 */
RexxRoutine5(uint32_t, SysShutDownSystem, OPTIONAL_CSTRING, computer, OPTIONAL_CSTRING, message, OPTIONAL_uint32_t, timeout,
             OPTIONAL_logical_t, forceAppsClosed, OPTIONAL_logical_t, reboot)
{
    uint32_t result = 0;

    HANDLE hToken = NULL;
    TOKEN_PRIVILEGES tkp;

    // First we get the token for the current process so we can add the proper
    // shutdown privilege.
    if ( OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken) == 0 )
    {
        result = GetLastError();
        goto done_out;
    }

    // Get the locally unique identifier for the shutdown privilege we need,
    // local or remote, depending on what the user specified.
    LPCTSTR privilegeName = (computer == NULL || *computer == '\0') ? SE_SHUTDOWN_NAME : SE_REMOTE_SHUTDOWN_NAME;
    if ( LookupPrivilegeValue(NULL, privilegeName, &tkp.Privileges[0].Luid) == 0 )
    {
        result = GetLastError();
        goto done_out;
    }

    // We are only going to adjust 1 privilege and we are going to enable it.
    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    // The return from this function can not accurately be used to determine if
    // it failed or not.  Instead we need to use GetLastError to determine
    // success.
    AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, NULL, 0);
    result = GetLastError();
    if ( result != ERROR_SUCCESS )
    {
        goto done_out;
    }

    // Do not shut down in 0 seconds by default.
    if ( argumentOmitted(3) )
    {
        timeout = 30;
    }

    // Now just call the API with the parameters specified by the user.
    if ( InitiateSystemShutdown((LPSTR)computer, (LPSTR)message, timeout, (BOOL)forceAppsClosed, (BOOL)reboot) == 0 )
    {
        result = GetLastError();
    }

    // Finally, restore the shutdown privilege for this process to disabled.
    tkp.Privileges[0].Attributes = 0;
    AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, NULL, 0);

done_out:
    if ( hToken != NULL )
    {
        CloseHandle(hToken);
    }
    return result;
}

/*************************************************************************
* Function:  SysSwitchSession                                            *
*                                                                        *
* Syntax:    result = SysSwitchSession(name)                             *
*                                                                        *
* Params:    name   - name of target session                             *
*                                                                        *
* Return:    OS/2 error return code                                      *
*************************************************************************/

RexxRoutine1(int, SysSwitchSession, CSTRING, name)
{
    HWND hwnd = FindWindow(NULL, name);

    if (hwnd)
    {
        if (!SetForegroundWindow(hwnd))
        {
            return GetLastError();
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 1;
    }
}

/*************************************************************************
* Function:  SysWaitNamedPipe                                            *
*                                                                        *
* Syntax:    result = SysWaitNamedPipe(name, timeout)                    *
*                                                                        *
* Params:    name - name of the pipe                                     *
*            timeout - amount of time to wait.                           *
*                                                                        *
* Return:    Return code from WaitNamedPipe                              *
*************************************************************************/

RexxRoutine2(int, SysWaitNamedPipe, CSTRING, name, OPTIONAL_int, timeout)
{
    if (argumentOmitted(2))
    {
        timeout = NMPWAIT_USE_DEFAULT_WAIT;
    }
    else
    {
        if (timeout < -1)
        {
            context->InvalidRoutine();
            return 0;
        }
        if (timeout == 0)
        {
            timeout = NMPWAIT_USE_DEFAULT_WAIT;
        }
        else if (timeout == -1)
        {
            timeout = NMPWAIT_WAIT_FOREVER;
        }
    }

    if (WaitNamedPipe(name, timeout))
    {
        return 0;
    }
    else
    {
        return GetLastError();
    }
}


/*************************************************************************
* Function:  SysSetFileDateTime                                          *
*                                                                        *
* Syntax:    result = SysSetFileDateTime(filename [,newdate] [,newtime]) *
*                                                                        *
* Params:    filename - name of the file to update                       *
*            newdate  - new date to set in format YYYY-MM-DD (YYYY>1800) *
*            newtime  - new time to set in format HH:MM:SS               *
*                                                                        *
* Return:    0 - file date/time was updated correctly                    *
*            -1 - failure attribute update                               *
*************************************************************************/

RexxRoutine3(int, SysSetFileDateTime, CSTRING, name, OPTIONAL_CSTRING, newdate, OPTIONAL_CSTRING, newtime)
{
    BOOL      fOk = FALSE;
    FILETIME  sFileTime;
    FILETIME  sLocalFileTime;
    SYSTEMTIME sLocalSysTime;

    /* open output file for read/write for update */
    HANDLE setFile = CreateFile(name, GENERIC_WRITE | GENERIC_READ,
                                FILE_SHARE_READ | FILE_SHARE_WRITE,
                                NULL, OPEN_EXISTING, FILE_FLAG_WRITE_THROUGH |
                                FILE_FLAG_BACKUP_SEMANTICS, NULL);
    if (setFile && (setFile != INVALID_HANDLE_VALUE))
    {
        fOk = GetFileTime(setFile, NULL, NULL, &sFileTime);
        fOk &= FileTimeToLocalFileTime(&sFileTime, &sLocalFileTime);
        fOk &= FileTimeToSystemTime(&sLocalFileTime, &sLocalSysTime);
        if (fOk)
        {
            /* file date/time could be read, now parse the new date/time */
            if (newdate != NULL)
            {
                /* parse new date */
                if (sscanf(newdate, "%4hu-%2hu-%2hu", &sLocalSysTime.wYear,
                           &sLocalSysTime.wMonth, &sLocalSysTime.wDay) != 3)
                {
                    fOk = false;
                }

                if (sLocalSysTime.wYear < 1800)
                {
                    fOk = false;
                }
            }

            if (newtime != NULL)
            {
                /* parse new time */
                if (sscanf(newtime, "%2hu:%2hu:%2hu", &sLocalSysTime.wHour,
                           &sLocalSysTime.wMinute, &sLocalSysTime.wSecond) != 3)
                {
                    fOk = false;
                }
            }

            if (newdate == NULL && newtime == NULL)
            {
                /* we set the timestamp to the current time and date */
                GetLocalTime(&sLocalSysTime);
            }

            if (fOk)
            {
                fOk &= SystemTimeToFileTime(&sLocalSysTime, &sLocalFileTime);
                fOk &= LocalFileTimeToFileTime(&sLocalFileTime, &sFileTime);
                fOk &= SetFileTime(setFile, NULL, NULL, &sFileTime);
            }
        }

        CloseHandle(setFile);
    }

    return fOk ? 0 : 1;
}

/*************************************************************************
* Function:  SysGetFileDateTime                                          *
*                                                                        *
* Syntax:    result = SysGetFileDateTime(filename [,timesel])            *
* Params:    filename - name of the file to query                        *
*            timesel  - What filetime to query: Created/Access/Write     *
*                                                                        *
* Return:    -1 - file date/time query failed                            *
*            other - date and time as YYYY-MM-DD HH:MM:SS                *
*************************************************************************/

RexxRoutine2(RexxObjectPtr, SysGetFileDateTime, CSTRING, name, OPTIONAL_CSTRING, selector)
{
    FILETIME  sFileTime;
    FILETIME  sLocalFileTime;
    FILETIME  *psFileCreated = NULL;
    FILETIME  *psFileAccessed = NULL;
    FILETIME  *psFileWritten = NULL;
    SYSTEMTIME sLocalSysTime;

    if (selector != NULL)
    {
        switch (selector[0])
        {
            case 'c':
            case 'C':
                psFileCreated = &sFileTime;
                break;
            case 'a':
            case 'A':
                psFileAccessed = &sFileTime;
                break;
            case 'w':
            case 'W':
                psFileWritten = &sFileTime;
                break;
            default:
                context->InvalidRoutine();
                return NULLOBJECT;
        }
    }
    else
    {
        psFileWritten = &sFileTime;
    }

    /* open file for read to query time */
    HANDLE setFile = CreateFile(name, FILE_READ_ATTRIBUTES,
                                FILE_SHARE_READ | FILE_SHARE_WRITE,
                                NULL, OPEN_EXISTING, FILE_FLAG_WRITE_THROUGH |
                                FILE_FLAG_BACKUP_SEMANTICS, NULL);
    if (setFile != INVALID_HANDLE_VALUE)
    {
        BOOL fOk = GetFileTime(setFile, psFileCreated, psFileAccessed, psFileWritten);
        CloseHandle(setFile);
        fOk &= FileTimeToLocalFileTime(&sFileTime, &sLocalFileTime);
        fOk &= FileTimeToSystemTime(&sLocalFileTime, &sLocalSysTime);

        if (fOk)
        {
            char buffer[256];
            sprintf(buffer, "%4d-%02d-%02d %02d:%02d:%02d",
                    sLocalSysTime.wYear,
                    sLocalSysTime.wMonth,
                    sLocalSysTime.wDay,
                    sLocalSysTime.wHour,
                    sLocalSysTime.wMinute,
                    sLocalSysTime.wSecond);
            return context->String(buffer);
        }
    }
    return context->WholeNumber(-1);

}


/*************************************************************************
* Function:  SysUtilVersion                                              *
*                                                                        *
* Syntax:    Say SysUtilVersion                                          *
*                                                                        *
* Return:    REXXUTIL.DLL Version                                        *
*************************************************************************/

RexxRoutine0(RexxStringObject, SysUtilVersion)
{
    char buffer[256];
                                       /* format into the buffer     */
    sprintf(buffer, "%d.%d.%d", ORX_VER, ORX_REL, ORX_MOD);
    return context->String(buffer);
}

/**
 * Check if the dwFlags arguement to WideCharToMultiByte() can be used by the
 * specified code page.  See MSDN documentation for WideCharToMultiByte() for
 * clarification.  This is used by SysFromUnicode()
 *
 * @param cp  Code page to check.
 *
 * @return Return true if dwFlags can be non-zero, return false if dwFlags must
 *         be zero.
 */
static bool canUseWideCharFlags(UINT cp)
{
    if ( cp == CP_SYMBOL || cp == CP_UTF7 || cp == CP_UTF8 )
    {
        return false;
    }
    if ( 50220 <= cp && cp <= 50222  )
    {
        return false;
    }
    if ( cp == 50225 || cp == 50227 || cp == 50229 || cp == 52936 || cp == 54936 )
    {
        return false;
    }
    if ( 57002 <= cp && cp <= 57011  )
    {
        return false;
    }
    return true;
}

/*************************************************************************
* Function:  SysFromUnicode                                              *
*            Converts a UNICODE string to an ASCII string                *
*                                                                        *
* Syntax:    result = SysFromUniCode(string,CodePage,MappingFlags,       *
*                                    DefaultChar, outstem.)              *
*                                                                        *
* Params:    string       - unicode string to be converted               *
*            Codepage     - target codepage                              *
*            MappingFlags - Mapping flags                                *
*            DefaultChar  - default for unmappable chars                 *
*             outstem.    - stem containg the result                     *
*              .!USEDDEFAULTCHAR - 1: character used as default          *
*              .!TEXT     - converted text                               *
*                                                                        *
*                                                                        *
* Return:    0 - successfull completetion                                *
*            error code from WideCharToMultiByte                         *

  The following are the OEM code-page identifiers.

    437  MS-DOS United States
    708  Arabic (ASMO 708)
    709  Arabic (ASMO 449+, BCON V4)
    710  Arabic (Transparent Arabic)
    720  Arabic (Transparent ASMO)
    737  Greek (formerly 437G)
    775  Baltic
    850  MS-DOS Multilingual (Latin I)
    852  MS-DOS Slavic (Latin II)
    855  IBM Cyrillic (primarily Russian)
    857  IBM Turkish
    860  MS-DOS Portuguese
    861  MS-DOS Icelandic
    862  Hebrew
    863  MS-DOS Canadian-French
    864  Arabic
    865  MS-DOS Nordic
    866  MS-DOS Russian (former USSR)
    869  IBM Modern Greek
    874  Thai
    932  Japan
    936  Chinese (PRC, Singapore)
    949  Korean
    950  Chinese (Taiwan; Hong Kong SAR, PRC)
    1361 Korean (Johab)

  The following are the ANSI code-page identifiers.

    874  Thai
    932  Japan
    936  Chinese (PRC, Singapore)
    949  Korean
    950  Chinese (Taiwan; Hong Kong SAR, PRC)
    1200 Unicode (BMP of ISO 10646)
    1250 Windows 3.1 Eastern European
    1251 Windows 3.1 Cyrillic
    1252 Windows 3.1 Latin 1 (US, Western Europe)
    1253 Windows 3.1 Greek
    1254 Windows 3.1 Turkish
    1255 Hebrew
    1256 Arabic
    1257 Baltic

  COMPOSITECHECK :
    Convert composite characters to precomposed characters.

  DISCARDNS :
    Discard nonspacing characters during conversion.

  SEPCHARS :
    Generate separate characters during conversion. This is the default conversion behavior.

  DEFAULTCHAR :
    Replace exceptions with the default character during conversion.

*************************************************************************/

RexxRoutine5(int, SysFromUniCode, RexxStringObject, sourceString, OPTIONAL_CSTRING, codePageOpt,
    OPTIONAL_CSTRING, mappingFlags, OPTIONAL_CSTRING, defaultChar, RexxStemObject, stem)
{
    const char *source = context->StringData(sourceString);
    size_t sourceLength = context->StringLength(sourceString);

    UINT  codePage;
    /* evaluate codepage          */
    if (codePageOpt == NULL)
    {
        codePage = GetOEMCP();
    }
    else
    {
        if (_stricmp(codePageOpt, "THREAD_ACP") == 0)
        {
            codePage = CP_THREAD_ACP;
        }
        else if (_stricmp(codePageOpt,"ACP") == 0)
        {
            codePage = CP_ACP;
        }
        else if (_stricmp(codePageOpt,"MACCP") == 0)
        {
            codePage = CP_MACCP;
        }
        else if (_stricmp(codePageOpt,"OEMCP") == 0)
        {
            codePage = CP_OEMCP;
        }
        else if (_stricmp(codePageOpt,"SYMBOL") == 0)
        {
            codePage = CP_SYMBOL;
        }
        else if (_stricmp(codePageOpt,"UTF7") == 0)
        {
            codePage = CP_UTF7;
        }
        else if (_stricmp(codePageOpt,"UTF8") == 0)
        {
            codePage = CP_UTF8;
        }
        else
        {
            codePage = atoi(codePageOpt);
        }
    }

    DWORD dwFlags = 0;
    /* evaluate the mapping flags */
    if (mappingFlags != NULL && *mappingFlags != '\0' )
    {
        /* The WC_SEPCHARS, WC_DISCARDNS, and WC_DEFAULTCHAR flags must also
         * specify the WC_COMPOSITECHECK flag.  So, we add that for the user if
         * they skipped it. Those 4 flags are only available for code pages <
         * 50000, excluding 42 (CP_SYMBOL).  See the remarks section in the MSDN
         * docs for clarification.
         */
        if ( codePage < 50000 && codePage != CP_SYMBOL )
        {
            if ( StrStrI(mappingFlags, "COMPOSITECHECK") != NULL )
            {
                dwFlags |= WC_COMPOSITECHECK;
            }
            if ( StrStrI(mappingFlags, "SEPCHARS") != NULL )
            {
                dwFlags |= WC_SEPCHARS | WC_COMPOSITECHECK;
            }
            if ( StrStrI(mappingFlags, "DISCARDNS") != NULL )
            {
                dwFlags |= WC_DISCARDNS| WC_COMPOSITECHECK;
            }
            if ( StrStrI(mappingFlags, "DEFAULTCHAR") != NULL )
            {
                dwFlags |= WC_DEFAULTCHAR | WC_COMPOSITECHECK;
            }
        }

        if ( StrStrI(mappingFlags, "NO_BEST_FIT") != NULL )
        {
            dwFlags |= WC_NO_BEST_FIT_CHARS;
        }

        if ( StrStrI(mappingFlags, "ERR_INVALID") != NULL )
        {
            if ( codePage == CP_UTF8 && isAtLeastVista() )
            {
                dwFlags |= WC_ERR_INVALID_CHARS;
            }
        }
        else if ( dwFlags == 0 && ! (codePage < 50000 && codePage != CP_SYMBOL) )
        {
            context->InvalidRoutine();
            return 0;
        }
    }

    /* evaluate default charcter  */
    const char  *strDefaultChar = NULL;
    BOOL  bUsedDefaultChar = FALSE;
    BOOL  *pUsedDefaultChar = &bUsedDefaultChar;

    if (defaultChar != NULL && (dwFlags & WC_DEFAULTCHAR) == WC_DEFAULTCHAR)
    {
        strDefaultChar = defaultChar;
    }
    else
    {
        /* use our own default character rather than relying on the windows default */
        strDefaultChar = "?";
    }

    /* There are a number of invalid combinations of arguments to
     *  WideCharToMultiByte(), see the MSDN docs. Eliminate them here.
     */
    if ( codePage == CP_UTF8 && dwFlags == WC_ERR_INVALID_CHARS)
    {
        strDefaultChar = NULL;
        pUsedDefaultChar = NULL;
    }
    else if ( ! canUseWideCharFlags(codePage) )
    {
        dwFlags = 0;
        strDefaultChar = NULL;
        pUsedDefaultChar = NULL;
    }

    /* Allocate space for the string, to allow double zero byte termination */
    char *strptr = (char *)GlobalAlloc(GMEM_FIXED|GMEM_ZEROINIT, sourceLength + 4);
    if (strptr == NULL)
    {
        context->InvalidRoutine();
        return 0;
    }
    memcpy(strptr, source, sourceLength);

    /* Query the number of bytes required to store the Dest string */
    int iBytesNeeded = WideCharToMultiByte( codePage,
                                        dwFlags,
                                        (LPWSTR) strptr,
                                        (int)(sourceLength/2),
                                        NULL,
                                        0,
                                        NULL,
                                        NULL);

    if (iBytesNeeded == 0)
    {
        GlobalFree(strptr);
        return GetLastError();
    }

        // hard error, stop
    char *str = (char *)GlobalAlloc(GMEM_FIXED|GMEM_ZEROINIT, iBytesNeeded + 4);
    if (str == NULL)
    {
        context->InvalidRoutine();
        return 0;
    }

        /* Do the conversion */
    int iBytesDestination = WideCharToMultiByte(codePage,           // codepage
                                            dwFlags,                // conversion flags
                                            (LPWSTR) strptr,        // source string
                                            (int)(sourceLength/2),  // source string length
                                            str,                    // target string
                                            (int)iBytesNeeded,      // size of target buffer
                                            strDefaultChar,
                                            pUsedDefaultChar);

    if (iBytesDestination == 0) // call to function fails
    {
        GlobalFree(str);          //  free allocated string
        GlobalFree(strptr);       // free allocated string
        return GetLastError();    // return error from function call
    }

    // set whether the default character was used in the output stem
    if (bUsedDefaultChar)
    {
        context->SetStemElement(stem, "!USEDDEFAULTCHAR", context->True());
    }
    else
    {
        context->SetStemElement(stem, "!USEDDEFAULTCHAR", context->False());
    }

    context->SetStemElement(stem, "!TEXT", context->String(str, iBytesNeeded));
    GlobalFree(strptr);          // free allocated string
    GlobalFree(str);             // free allocated string
    return 0;
}

/*
* Syntax:    result = SysToUniCode(string,CodePage,MappingFlags,outstem.)
*/
/*************************************************************************
* Function:  SysToUnicode                                                *
*            Converts an ASCII to UNICODE                                *
*                                                                        *
* Syntax:    result = SysToUniCode(string,CodePage,MappingFlags,outstem.)*
*                                                                        *
* Params:    string       - ascii string to be converted                 *
*            Codepage     - target codepage                              *
*            MappingFlags - Mapping flags                                *
*             outstem.    - stem containg the result                     *
*              .!TEXT     - converted text                               *
*                                                                        *
* Return:    0 - successfull completetion                                *
*            error code from WideCharToMultiByteToWideChars              *

  For available codepages see function SysFromUniCode.

  Additional parameters for codepages:

    ACP        ANSI code page
    MACCP      Macintosh code page
    OEMCP      OEM code page
    SYMBOL     Windows 2000: Symbol code page (42)
    THREAD_ACP Windows 2000: The current thread's ANSI code page
    UTF7       Windows NT 4.0 and Windows 2000: Translate using UTF-7
    UTF8       Windows NT 4.0 and Windows 2000: Translate using UTF-8.
               When this is set, dwFlags must be zero.

    PRECOMPOSED       Always use precomposed characters-that is, characters
                      in which a base character and a nonspacing character
                      have a single character value.
                      This is the default translation option.
                      Cannot be used with MB_COMPOSITE.
    COMPOSITE         Always use composite characters that is,
                      characters in which a base character and a nonspacing
                      character have different character values.
                      Cannot be used with MB_PRECOMPOSED.
    ERR_INVALID_CHARS If the function encounters an invalid input character,
                      it fails and GetLastError returns
                      ERROR_NO_UNICODE_TRANSLATION.
    USEGLYPHCHARS     Use glyph characters instead of control characters.



*************************************************************************/
RexxRoutine4(int, SysToUniCode, RexxStringObject, source, OPTIONAL_CSTRING, codePageOpt,
    OPTIONAL_CSTRING, mappingFlags, RexxStemObject, stem)
{
    // evaluate codepage
    UINT   codePage;
    if (codePageOpt == NULL)
    {
        codePage = GetOEMCP();
    }
    else
    {
        if (_stricmp(codePageOpt,"THREAD_ACP") == 0)
        {
            codePage = CP_THREAD_ACP;
        }
        else if (_stricmp(codePageOpt,"ACP") == 0)
        {
            codePage = CP_ACP;
        }
        else if (_stricmp(codePageOpt,"MACCP") == 0)
        {
            codePage = CP_MACCP;
        }
        else if (_stricmp(codePageOpt,"OEMCP") == 0)
        {
            codePage = CP_OEMCP;
        }
        else if (_stricmp(codePageOpt,"SYMBOL") == 0)
        {
            codePage = CP_SYMBOL;
        }
        else if (_stricmp(codePageOpt,"UTF7") == 0)
        {
            codePage = CP_UTF7;
        }
        else if (_stricmp(codePageOpt,"UTF8") == 0)
        {
            codePage = CP_UTF8;
        }
        else
        {
            codePage = atoi(codePageOpt);
        }
    }

    DWORD  dwFlags = 0;
    // evaluate the mapping flags
    if (mappingFlags != NULL)
    {
        if (mystrstr(mappingFlags, "PRECOMPOSED"))
        {
            dwFlags |= MB_PRECOMPOSED;
        }
        if (mystrstr(mappingFlags, "COMPOSITE"))
        {
            dwFlags  |= MB_COMPOSITE;
        }
        if (mystrstr(mappingFlags, "ERR_INVALID"))
        {
            dwFlags |= MB_ERR_INVALID_CHARS;
        }
        if (mystrstr(mappingFlags, "USEGLYPHCHARS"))
        {
            dwFlags |= MB_USEGLYPHCHARS;
        }
        if (dwFlags == 0)
        {
            context->InvalidRoutine();
            return 0;
        }
    }

    /* Query the number of bytes required to store the Dest string */
    ULONG ulWCharsNeeded = MultiByteToWideChar( codePage, dwFlags,
        context->StringData(source), (int)context->StringLength(source), NULL, NULL);

    if (ulWCharsNeeded == 0)
    {
        return GetLastError();
    }

    ULONG ulDataLen = (ulWCharsNeeded)*2;

    LPWSTR lpwstr = (LPWSTR)GlobalAlloc(GMEM_FIXED|GMEM_ZEROINIT, ulDataLen+4);

    // hard error, stop
    if (lpwstr == NULL)
    {
        context->InvalidRoutine();
        return 0;
    }


    /* Do the conversion */
    ulWCharsNeeded = MultiByteToWideChar(codePage,  dwFlags,
        context->StringData(source), (int)context->StringLength(source),
        lpwstr, ulWCharsNeeded);

    if (ulWCharsNeeded == 0) // call to function fails
    {
        GlobalFree(lpwstr);       // free allocated string
        return GetLastError();
    }

    context->SetStemElement(stem, "!TEXT", context->String((const char *)lpwstr, ulDataLen));
    GlobalFree(lpwstr);        // free allocated string
    return 0;
}

/*************************************************************************
* Function:  SysWinGetPrinters                                           *
*                                                                        *
* Syntax:    call SysWinGetPrinters stem.                                *
*                                                                        *
* Params:    stem. - stem to store infos in                              *
*                                                                        *
* Return:    error number                                                *
*************************************************************************/

RexxRoutine1(uint32_t, SysWinGetPrinters, RexxStemObject, stem)
{
    DWORD realSize = 0;
    DWORD entries = 0;
    DWORD currentSize = 10*sizeof(PRINTER_INFO_2)*sizeof(char);
    char *pArray = (char*) malloc(sizeof(char)*currentSize);

    while (true)
    {
        if (EnumPrinters(PRINTER_ENUM_LOCAL|PRINTER_ENUM_CONNECTIONS, NULL, 2, (LPBYTE)pArray,
                         currentSize, &realSize, &entries) == 0)
        {
            // this is not a failure if we get ERROR_INSUFFICIENT_BUFFER
            DWORD rc = GetLastError();
            if ( rc != ERROR_INSUFFICIENT_BUFFER )
            {
                free(pArray);
                return rc;
            }
        }
        if ( currentSize >= realSize )
        {
            break;
        }
        currentSize = realSize;
        realSize = 0;
        pArray = (char*) realloc(pArray, sizeof(char)*currentSize);
    }

    PRINTER_INFO_2 *pResult = (PRINTER_INFO_2*) pArray;

    // set stem.0 to the number of entries then add all the found printers
    context->SetStemArrayElement(stem, 0, context->WholeNumber(entries));
    while ( entries-- )
    {
        char  szBuffer[256];
        sprintf(szBuffer,"%s,%s,%s", pResult[entries].pPrinterName, pResult[entries].pDriverName,
                pResult[entries].pPortName);
        context->SetStemArrayElement(stem, entries + 1, context->String(szBuffer));
    }
    free(pArray);
    return 0;          // a little reversed...success is false, failure is true
}

/*************************************************************************
* Function:  SysWinGetDefaultPrinter                                     *
*                                                                        *
* Syntax:    call SysWinGetDefaultPrinter                                *
*                                                                        *
* Params:    none                                                        *
*                                                                        *
* Return:    string describing default printer                           *
*************************************************************************/

RexxRoutine0(RexxStringObject, SysWinGetDefaultPrinter)
{
    char buffer[256];
    buffer[0] = '\0';

    GetProfileString("Windows", "DEVICE", ",,,", buffer, sizeof(buffer));
    return context->String(buffer);
}


/*************************************************************************
* Function:  SysWinSetDefaultPrinter                                     *
*                                                                        *
* Syntax:    call SysWinSetDefaultPrinter printer                        *
*                                                                        *
* Params:    string describing default printer                           *
*                                                                        *
* Return:    0 on success, otherwise the OS system error number.         *
*************************************************************************/
RexxRoutine1(int, SysWinSetDefaultPrinter, CSTRING, printer)
{
    int count = 0;

    // Two forms of input are allowed.  The old form of
    // "Printername,Drivername,Portname" and for W2K or later just the printer
    // name.  Count the commas to determine which form this might be.
    for ( size_t i = 0; printer[i] != '\0'; i++ )
    {
        if (printer[i] == ',' )
        {
            count++;
        }
    }

    if (count != 0 && count != 2)
    {
        context->InvalidRoutine();
        return 0;
    }
    SetLastError(0);

    if (count == 0 )
    {
        // This is W2K or later and the user specified just the printer name.
        // This code will work on W2K through Vista.
        if (SetDefaultPrinter(printer) != 0)
        {
            return 0;
        }
        else
        {
            return GetLastError();
        }
    }
    else
    {
        // The user still specified the old format. Microssoft
        // only provides WriteProfileString() for backward compatibility to
        // 16-bit Windows, and at some point this may no longer be supported.
        // But it still appears to work on XP.
        if (WriteProfileString("Windows", "DEVICE", printer) == 0)
        {
            return 0;
        }
        else
        {
            if ( SendMessageTimeout(HWND_BROADCAST, WM_SETTINGCHANGE, 0L, 0L, SMTO_NORMAL, 1000, NULL) == 0 )
            {
                // If a window just timed out, then GetLastError() will return 0
                // and the user will get the succes code.  If GetLastError()
                // does not return 0, then chances are something really is
                // wrong.
                return 0;
            }
            else
            {
                return GetLastError();
            }
        }
    }
}


/*************************************************************************
* Function:  SysFileCopy                                                 *
*                                                                        *
* Syntax:    call SysFileCopy FROMfile TOfile                            *
*                                                                        *
* Params:    FROMfile - file to be copied.                               *
*            TOfile - target file of copy operation.                     *
*                                                                        *
* Return:    Return code from CopyFile() function.                       *
*************************************************************************/

RexxRoutine2(int, SysFileCopy, CSTRING, fromFile, CSTRING, toFile)
{
    return CopyFile(fromFile, toFile, 0) ? 0 : GetLastError();
}

/*************************************************************************
* Function:  SysFileMove                                                 *
*                                                                        *
* Syntax:    call SysFileMove FROMfile TOfile                            *
*                                                                        *
* Params:    FROMfile - file to be moved.                                *
*            TOfile - target file of move operation.                     *
*                                                                        *
* Return:    Return code from MoveFile() function.                       *
*************************************************************************/

RexxRoutine2(int, SysFileMove, CSTRING, fromFile, CSTRING, toFile)
{
    return MoveFile(fromFile, toFile) ? 0 : GetLastError();
}

/*************************************************************************
* Function:  SysIsFileCompressed                                         *
*                                                                        *
* Syntax:    call SysIsFileCompressed file                               *
*                                                                        *
* Params:    file - file to check if it is compressed.                   *
*                                                                        *
* Return:    Logical.                                                    *
*************************************************************************/

RexxRoutine1(logical_t, SysIsFileCompressed, CSTRING, file)
{
    DWORD dwAttrs = GetFileAttributes(file);
    return (dwAttrs != 0xffffffff) && (dwAttrs & FILE_ATTRIBUTE_COMPRESSED);
}

/*************************************************************************
* Function:  SysIsFileEncrypted                                          *
*                                                                        *
* Syntax:    call SysIsFileEncrypted file                                *
*                                                                        *
* Params:    file - file to check if it is Encrypted.                    *
*                                                                        *
* Return:    Logical.                                                    *
*************************************************************************/

RexxRoutine1(logical_t, SysIsFileEncrypted, CSTRING, file)
{
    DWORD dwAttrs = GetFileAttributes(file);
    return (dwAttrs != 0xffffffff) && (dwAttrs & FILE_ATTRIBUTE_ENCRYPTED);
}

/*************************************************************************
* Function:  SysIsFileNotContentIndexed                                  *
*                                                                        *
* Syntax:    call SysIsFileNotContentIndexed file                        *
*                                                                        *
* Params:    file - file to check if it is to be indexed by the indexing *
*             service.                                                   *
*                                                                        *
* Return:    Logical.                                                    *
*************************************************************************/

RexxRoutine1(logical_t, SysIsFileNotContentIndexed, CSTRING, file)
{
    DWORD dwAttrs = GetFileAttributes(file);
    return (dwAttrs != 0xffffffff) && (dwAttrs & FILE_ATTRIBUTE_NOT_CONTENT_INDEXED);
}

/*************************************************************************
* Function:  SysIsFileOffline                                            *
*                                                                        *
* Syntax:    call SysIsFileOffline file                                  *
*                                                                        *
* Params:    file - file to check if it is offline                       *
*                                                                        *
* Return:    Logical.                                                    *
*************************************************************************/

RexxRoutine1(logical_t, SysIsFileOffline, CSTRING, file)
{
    DWORD dwAttrs = GetFileAttributes(file);
    return (dwAttrs != 0xffffffff) && (dwAttrs & FILE_ATTRIBUTE_OFFLINE);
}

/*************************************************************************
* Function:  SysIsFileSparse                                             *
*                                                                        *
* Syntax:    call SysIsFileSparse file                                   *
*                                                                        *
* Params:    file - file to check if it is sparse                        *
*                                                                        *
* Return:    Logical.                                                    *
*************************************************************************/

RexxRoutine1(logical_t, SysIsFileSparse, CSTRING, file)
{
    DWORD dwAttrs = GetFileAttributes(file);
    return (dwAttrs != 0xffffffff) && (dwAttrs & FILE_ATTRIBUTE_SPARSE_FILE);
}


/*************************************************************************
* Function:  SysIsFileTemporary                                          *
*                                                                        *
* Syntax:    call SysIsFileTemporary file                                *
*                                                                        *
* Params:    file - file to check if it is temporary                     *
*                                                                        *
* Return:    Logical.                                                    *
*************************************************************************/

RexxRoutine1(logical_t, SysIsFileTemporary, CSTRING, file)
{
    DWORD dwAttrs = GetFileAttributes(file);
    return (dwAttrs != 0xffffffff) && (dwAttrs & FILE_ATTRIBUTE_TEMPORARY);
}


/*************************************************************************
* Function:  SysGetLongPathName                                          *
*            Converts the specified path to its long form                *
*                                                                        *
* Syntax:    longPath = SysGetLongPathName(path)                         *
*                                                                        *
* Params:    path - a path to an existing file                           *
*                                                                        *
* Return:    longPath - path converted to its long form                  *
*                       NULL string if path doesn't exist or call fails  *
*************************************************************************/

RexxRoutine1(RexxStringObject, SysGetLongPathName, CSTRING, path)
{
    CHAR  longPath[MAX];                 // long version of path
    DWORD code = GetLongPathName(path, longPath, MAX);
    if ((code == 0) || (code >= MAX))    // call failed of buffer too small
    {
        return context->NullString();
    } else
    {
        return context->NewStringFromAsciiz(longPath);
    }
}


/*************************************************************************
* Function:  SysGetShortPathName                                         *
*            Converts the specified path to its short form               *
*                                                                        *
* Syntax:    shortPath = SysGetShortPathName(path)                       *
*                                                                        *
* Params:    path - a path to an existing file                           *
*                                                                        *
* Return:    shortPath - path converted to its short form                *
*                        NULL string if path doesn't exist or call fails *
*************************************************************************/

RexxRoutine1(RexxStringObject, SysGetShortPathName, CSTRING, path)
{
    CHAR  shortPath[MAX];                // short version of path
    DWORD code = GetShortPathName(path, shortPath, MAX);
    if ((code == 0) || (code >= MAX))    // call failed of buffer too small
    {
        return context->NullString();
    } else
    {
        return context->NewStringFromAsciiz(shortPath);
    }
}
