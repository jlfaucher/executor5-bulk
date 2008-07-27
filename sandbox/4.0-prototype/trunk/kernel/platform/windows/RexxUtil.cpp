/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 1995, 2004 IBM Corporation. All rights reserved.             */
/* Copyright (c) 2005-2006 Rexx Language Association. All rights reserved.    */
/*                                                                            */
/* This program and the accompanying materials are made available under       */
/* the terms of the Common Public License v1.0 which accompanies this         */
/* distribution. A copy is also available at the following address:           */
/* http://www.ibm.com/developerworks/oss/CPLv1.0.htm                          */
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
/* REXX Windows Support                                                       */
/*                                                                            */
/* Windows system utility functions                                           */
/*                                                                            */
/******************************************************************************/

#include "oorexx.h"
#include <memory.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <math.h>
#include <limits.h>
#include "SysProcess.hpp"
#include "SysFileSystem.hpp"


/*********************************************************************/
/*  Various definitions used by various functions.                   */
/*********************************************************************/

#define  MAX_LABEL      13             // max label length (sdrvinfo)
#define  MAX_DIGITS     9              /* max digits in numeric arg  */
#define  IBUF_LEN       4096           /* Input buffer length        */
#define  MAX_READ       0x10000        /* full segment of buffer     */
#define  CH_EOF         0x1A           /* end of file marker         */
#define  CH_CR          '\r'           /* carriage return character  */
#define  CH_NL          '\n'           /* new line character         */
#define  AllocFlag      PAG_COMMIT | PAG_WRITE  /* for DosAllocMem   */
#define  RNDFACTOR      1664525L
#define MAX_ENVVAR      1024
#define  MAX_LINE_LEN   4096           /* max line length */

#define UTIL_VERS "2.13"

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
#define  LONG_TIME      0x0040  /* long time format for SysFileTree */
#define  RXIGNORE       2              /* Ignore attributes entirely */
#define  AllAtts        FILE_NORMAL | FILE_READONLY | FILE_HIDDEN | \
FILE_SYSTEM | FILE_DIRECTORY | FILE_ARCHIVED
#define  AllFiles       FILE_NORMAL | FILE_READONLY | FILE_HIDDEN | \
FILE_SYSTEM | FILE_ARCHIVED
#define  AllDirs        FILE_READONLY | FILE_HIDDEN | \
FILE_SYSTEM | FILE_ARCHIVED | MUST_HAVE_DIRECTORY | FILE_DIRECTORY


/*********************************************************************/
/* Numeric Error Return Strings                                      */
/*********************************************************************/

#define  NO_UTIL_ERROR    0            /* No error whatsoever        */
#define  ERROR_NOMEM      2            /* Insufficient memory        */
#define  ERROR_FILEOPEN   3            /* Error opening text file    */
#define  ERROR_RETSTR   "ERROR:"

/*********************************************************************/
/* Structures used throughout REXXUTIL.C                             */
/*********************************************************************/

/*********************************************************************/
/* RxTree Structure used by GetLine, OpenFile and CloseFile          */
/*********************************************************************/
typedef struct
{
  char *       buffer;                 /* file read buffer           */
  size_t       size;                   /* file size                  */
  size_t       data;                   /* data left in buffer        */
  size_t       residual;               /* size left to read          */
  char *       scan;                   /* current scan position      */
  HANDLE       handle;                 /* file handle                */
} GetFileData;

/*********************************************************************/
/* RxTree Structure used by SysTree.                                 */
/*********************************************************************/

typedef struct
{
    RexxCallContext *context;          // the routine call context
    size_t count;                      /* Number of lines processed  */
    RexxStemObject stem;               // the stem we're adding to
    char TargetSpec[MAX_PATH+1];       /* Target filespec            */
    char truefile[MAX_PATH+1];         /* expanded file name         */
    char Temp[MAX_PATH+80];            /* buffer for returned values */
    ULONG nattrib;                     /* New attrib, diff for each  */
} RxTreeData;


/*********************************************************************/
/* Saved character status                                            */
/*********************************************************************/
static   int   ExtendedFlag = 0;       /* extended character saved   */
static   UCHAR ExtendedChar;           /* saved extended character   */

/*********************************************************************/
/****************  REXXUTIL Supporting Functions  ********************/
/****************  REXXUTIL Supporting Functions  ********************/
/****************  REXXUTIL Supporting Functions  ********************/
/*********************************************************************/

int readNextBuffer( GetFileData *filedata );

/********************************************************************
* Function:  OpenFile(file, filedata)                               *
*                                                                   *
* Purpose:   Prepares a file for reading.                           *
*                                                                   *
* RC:        0     - file was opened successfully                   *
*            1     - file open error occurred                       *
*********************************************************************/

int myOpenFile(
   char        *file,                  /* file name                  */
   GetFileData *filedata )             /* global file information    */
{
   DWORD       dwSize;                 /* file status information    */

                                       /* try to open the file       */
  if ((filedata->handle = CreateFile(file, GENERIC_READ,
                            FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING,
                            FILE_FLAG_WRITE_THROUGH, 0))
                            == INVALID_HANDLE_VALUE)
    return 1;                          /* return failure             */

                                       /* retrieve the file size     */
  dwSize = GetFileSize(filedata->handle, NULL);
                                       /* if GetFileSize failed or   */
                                       /* size=0                     */
  if (dwSize == 0xffffffff || !dwSize) {
    CloseHandle(filedata->handle);     /* close the file             */
    return 1;                          /* and quit                   */
  }
  if (dwSize <= MAX_READ) {            /* less than a single buffer  */
                                       /* allocate buffer for file   */
    if (!(filedata->buffer = (char *)GlobalAlloc(GMEM_ZEROINIT | GMEM_FIXED, dwSize))) {
      CloseHandle(filedata->handle);   /* close the file             */
      return 1;
    }
    filedata->size = dwSize;           /* save file size             */
    filedata->residual = 0;            /* no left over information   */

    DWORD bytesRead;
                                       /* read the file in           */
    if (!ReadFile(filedata->handle, filedata->buffer, dwSize,
                &bytesRead, NULL)) {
      GlobalFree(filedata->buffer);    /* free the buffer            */
      CloseHandle(filedata->handle);   /* close the file             */
      return 1;
    }
    filedata->data = (size_t)bytesRead;
    filedata->scan = filedata->buffer; /* set position to beginning  */
  }
  else {                               /* need to read partial       */
                                       /* allocate buffer for read   */
    if (!(filedata->buffer = (char *)GlobalAlloc(GMEM_ZEROINIT | GMEM_FIXED, MAX_READ))) {
      CloseHandle(filedata->handle);   /* close the file             */
      return 1;
    }

    filedata->size = dwSize;           /* save file size             */
                                       /* and set remainder          */
    filedata->residual = filedata->size;
                                       /* read the file in           */
    if (readNextBuffer(filedata)) {
      GlobalFree(filedata->buffer);    /* free the buffer            */
      CloseHandle(filedata->handle);   /* close the file             */
      return 1;
    }
  }
  return 0;                            /* file is opened             */
}

/********************************************************************
* Function:  CloseFile(filedata)                                    *
*                                                                   *
* Purpose:   Close a file                                           *
*********************************************************************/
void closeFile(
   GetFileData *filedata )             /* global file information    */
{
  CloseHandle(filedata->handle);       /* close the file             */
  GlobalFree(filedata->buffer);        /* free the buffer            */
}

/********************************************************************
* Function:  ReadNextBuffer(filedata)                               *
*                                                                   *
* Purpose:   Reads the next buffer of data.                         *
*                                                                   *
* RC:        0       buffer was read                                *
*            1     - error occurred reading buffer                  *
*********************************************************************/
int readNextBuffer(
   GetFileData  *filedata )            /* global file information    */
{
  size_t    size;                      /* size to read               */
  DWORD     readSize;
                                       /* get size of this read      */
  size = min(MAX_READ, filedata->residual);

                                       /* read the file in           */
  if (!ReadFile(filedata->handle, filedata->buffer, (DWORD)size, &readSize, NULL))
      return 1;
  filedata->data = (size_t)readSize;

  if (filedata->data != size)          /* not get all of it?         */
    filedata->residual = 0;            /* no residual                */
  else                                 /* residual is remainder      */
    filedata->residual = filedata->residual - size;

  filedata->scan = filedata->buffer;   /* set position to beginning  */
  return 0;
}

/********************************************************************
* Function:  GetLine(line, size, filedata)                          *
*                                                                   *
* Purpose:   Reads a line of data using buffered reads.  At end of  *
*            file, zero is returned to indicate nothing left.       *
*                                                                   *
* RC:        true -  line was read successfully                     *
*            false - end of file was reached                        *
*********************************************************************/

int getLine(
   char        *line,                  /* returned line              */
   size_t       size,                  /* size of line buffer        */
   GetFileData *filedata )             /* file handle                */
{
   char *       scan;                  /* current scan pointer       */
   size_t       length;                /* line length                */
   size_t       copylength;            /* copied length              */


  if (!(filedata->data)) {             /* if out of current buffer   */
    if (filedata->residual) {          /* may be another buffer      */
      readNextBuffer(filedata);        /* try to read one            */
      if (!filedata->data)             /* nothing more?              */
        return 1;                      /* all done                   */
    }
    else
      return (1);                      /* return EOF condition       */
  }
                                       /* look for a carriage return */
  scan = (char *)memchr(filedata->scan, CH_NL, filedata->data);
  if (scan) {                          /* found one                  */
                                       /* calculate the length       */
    length = (ULONG)(scan - filedata->scan);
    copylength = min(length, size);    /* get length to copy         */
                                       /* copy over the data         */
    memcpy(line, filedata->scan, copylength);
    line[copylength] = '\0';           /* make into ASCIIZ string    */

    /* we don't want the CR character in the result string*/
    if ( line[copylength - 1] == CH_CR ) {
      line[copylength - 1] = '\0';
    } /* endif */

    filedata->data -= length + 1;      /* reduce the length          */
    filedata->scan = scan + 1;         /* set new scan point         */

    if (!filedata->data) {             /* all used up                */
      if (filedata->residual)          /* more to read               */
        readNextBuffer(filedata);      /* read the next buffer       */

    }
    return 0;                          /* this worked ok             */
  }
  else                                 /* ran off the end            */
  {
    /* now we have scanned the whole buffer, but didn't find LF.         */
    /* we have two situation that can appear:                            */
    /* 1.) size > filedata->data ==> there is still room in the working  */
    /*     buffer, we can see whether we have scanned the whole file     */
    /*     --> ReadNextBuffer, or this was it, and we return             */
    /* 2.) size < filedata->buffer ==> we have scanned to the end of the */
    /*     buffer, more than what would fit into it, but still we        */
    /*     haven't had a hit. So copy all elements into the buffer       */
    /*     read the next buffer, GetLine to get the next LF              */
    /*     and return what was put into buffer. Be ALWAYS AWARE that     */
    /*     that buffer limits to 2047 bytes, and that we only return up  */
    /*     to 2047 bytes of a line. The rest of the line is not returned */
    /*     and not checked for search argument. Nevertheless, this       */
    /*     garantees, that the line counter (argument 'N') corresponds   */
    /*     with the input file                                           */

                                       /* get length to copy         */
    if (size > filedata->data)
    {
       copylength = filedata->data;    /* copy the rest into linebuffer */
                                       /* copy over the data         */
       memcpy(line, filedata->scan, copylength);
       line[copylength] = '\0';           /* make into ASCIIZ string    */

    /* we don't want the CR character in the result string*/
    /* we have not found LF, so why look for CR                      */
     /* all data should be read, filedata->data must be zero          */
       filedata->data -= copylength;
     /* scan should be at the end                                     */
       filedata->scan += copylength;    /* set new scan point         */

    /* if no more data to read in the file, return OK     */
       if (!filedata->residual)
          return 0;
       else
          return getLine(line + copylength, size - copylength, filedata);
    }
    else        /* the line is full, scan until LF found but no copy */
    {
       copylength = min(size, filedata->data);
                                      /* copy over the data         */
       memcpy(line, filedata->scan, copylength);
       line[copylength] = '\0';           /* make into ASCIIZ string    */

       filedata->data  = 0;             /* no data in buffer          */
       filedata->scan += filedata->data;     /* set scan point to end */

       if (filedata->residual)          /* more to read               */
       {
           readNextBuffer(filedata);    /* read the next buffer       */
           return getLine(line + copylength, 0, filedata);
       }
       else
          return 0;
    }
  }
}

/********************************************************************
* Function:  setFileMode(file, attributes)                          *
*                                                                   *
* Purpose:   Change file attribute bits                             *
*            without PM.                                            *
*                                                                   *
* RC:        0    -  File attributes successfully changed           *
*            1    -  Unable to change attributes                    *
*********************************************************************/
int  setFileMode(
  char    *file,                       /* file name                  */
  ULONG    attr )                      /* new file attributes        */
{

  DWORD         dwfileattrib;          /* file attributes            */

                                       /* get the file status        */
  if ((dwfileattrib = GetFileAttributes(file)) != 0xffffffff) {
                                       /* if worked                  */
                                       /* set the attributes         */
    if ((dwfileattrib = SetFileAttributes(file,attr)) != 0)
      return 0;   /* give back success flag     */
    else
      return 1;
  } else
    return 1;
}


/********************************************************************
* Function:  mystrstr(haystack, needle, hlen, nlen, sensitive)      *
*                                                                   *
* Purpose:   Determines if the string 'needle' is in the            *
*            string 'haystack' by returning it's position or        *
*            a NULL if not found.  The length of haystack and       *
*            needle are given by 'hlen' and 'nlen' respectively.    *
*                                                                   *
*            If 'sensitive' is true, then the search is case        *
*            sensitive, else it is case insensitive.                *
*                                                                   *
* RC:        num  - The pos where needle was found.                 *
*            NULL - needle not found.                               *
*                                                                   *
* Used By:   SysFileSearch()                                        *
*********************************************************************/

size_t mystrstr(
  char   *haystack,
  char   *needle,
  size_t  hlen,
  size_t  nlen,
  bool    sensitive)

{
  char line[MAX_LINE_LEN];
  char target[MAX_LINE_LEN];
  ULONG p;
 /* Copy line  - Change nulls to spaces and uppercase if needed      */

  for (p = 0; p < hlen; p++) {

    if (haystack[p] == '\0')
      line[p] = ' ';
    else if (sensitive)
      line[p] = haystack[p];
    else line[p] = (char)toupper(haystack[p]);
  }
  line[p] = '\0';

 /* Copy target  - Change nulls to spaces and uppercase if needed    */

  for (p = 0; p < nlen; p++) {

    if (needle[p] == '\0')
      target[p] = ' ';
    else if (sensitive)
      target[p] = needle[p];
    else target[p] = (char)toupper(needle[p]);
  }
  target[p] = '\0';

  return ((size_t)strstr(line, target));
}


/*****************************************************************
* Function:  getpath(string, path, filename)                     *
*                                                                *
* Purpose:  This function gets the PATH and FILENAME of the file *
*           target contained in STRING.  The path will end with  *
*           the '\' char if a path is supplied.                  *
*                                                                *
*****************************************************************/

void getpath(
  char *string,
  char *path,
  char *filename)
{
  size_t len;                          /* length of filespec         */
  size_t LastSlashPos;                 /* position of last slash     */
  char   szBuff[SysFileSystem::MaximumFileNameBuffer];             /* used to save current dir   */
  char   drv[3];                       /* used to change dir         */
  size_t i=0;

  while (string[i] == ' ') i++;                  /* skip leading blanks, */
  if (i) {
      len = strlen(string);                /* Get length of full file    */
      if ((string[i] == '\\' || string[i] == '/') ||  /* if first after blank is \ */
          (string[i] == '.' &&
            ((i<len && (string[i+1] == '\\' || string[i+1] == '/')) ||  /* or .\ */
            (i+1<len && string[i+1] == '.' && (string[i+2] == '\\' || string[i+2] == '/')))) ||  /* or ..\ */
            (i<len && string[i+1] == ':'))  /* z: */
                string = &string[i];
  }

  if (!strcmp(string, "."))            /* period case?               */
    strcpy(string, "*.*");             /* make it a *.* request      */
  else if (!strcmp(string, ".."))      /* double period case?        */
    strcpy(string, "..\\*.*");         /* make it a ..\*.* request   */
  len = strlen(string);                /* Get length of full file    */
                                       /* spec                       */
  LastSlashPos = len;                  /* Get max pos of last '\'    */

    /* Step back through string until at begin or at '\' char        */

  while (string[LastSlashPos] != '\\' && string[LastSlashPos] != '/' && LastSlashPos >= 0)
    --LastSlashPos;
  if (LastSlashPos < 0) {              /* no backslash, may be drive */
    if (string[1] == ':') {
      len = MAX_PATH;                  /* set max length             */
                                       /* Save the current drive     */
                                       /* and path                   */
      GetCurrentDirectory(sizeof(szBuff), szBuff);
      /* just copy the drive letter and the colon, omit the rest */
      /* (necessary i.g. if "I:*" is used */
      memcpy(drv, string, 2);
      drv[2] = '\0';

      SetCurrentDirectory(drv);     /* change to specified drive     */
                                       /* Get current directory      */
      GetCurrentDirectory((DWORD)len, path);
      SetCurrentDirectory(szBuff);     /* go back to where we were   */
                                       /* need a trailing slash?     */
      if (path[strlen(path) - 1] != '\\')
        strcat(path, "\\");            /* add a trailing slash       */
      LastSlashPos = 1;                /* make drive the path        */
    }
    else {
                                        /* Get current directory      */
      GetCurrentDirectory(MAX_PATH, path);                                                                     /* Get current directory      */
                                       /* need a trailing slash?     */
      if (path[strlen(path) - 1] != '\\')
        strcat(path, "\\");            /* add a trailing slash       */
    }
  }
  else {                               /* have a path                */
    if (string[1] == ':') {            /* have a drive?              */
                                       /* copy over the path         */
      memcpy(path, string, LastSlashPos+1);
      path[LastSlashPos+1] = '\0';     /* make into an ASCII-Z string*/
    }
    else {
      char fpath[MAX_PATH];
      char drive[_MAX_DRIVE];
      char dir[_MAX_DIR];
      char fname[_MAX_FNAME];
      char ext[_MAX_EXT];
      char lastc;


      if (LastSlashPos == 0)    /* only one backslash at the beginning */
      {
          _fullpath(fpath, "\\", MAX_PATH);  /* Get full path              */
          strcat(fpath, &string[1]);
      }
      else
      {

          string[LastSlashPos] = '\0';     /* chop off the path          */
          _fullpath(fpath, string, MAX_PATH);  /* Get full path              */
          string[LastSlashPos] = '\\';     /* put the slash back         */
          lastc = fpath[strlen(fpath)-1];
          if (lastc != '\\' && lastc != '/')
              strcat(fpath, &string[LastSlashPos]);
      }
      _splitpath( fpath, drive, dir, fname, ext );

      strcpy(path, drive);
      strcat(path, dir);

      if (!strlen(path)) {             /* invalid path?              */
                                       /* copy over the path         */
         memcpy(path, string, LastSlashPos+1);
         path[LastSlashPos+1] = '\0';  /* make into an ASCII-Z string*/
      }
                                       /* need a trailing slash?     */
      if (path[strlen(path) - 1] != '\\')
        strcat(path, "\\");            /* add a trailing slash       */
    }
  }

    /* Get file name from filespec (just after last '\')             */
  if (string[LastSlashPos+1])          /* have a real name?          */
                                       /* copy it over               */
    strcpy(filename, &string[LastSlashPos+1]);
  else
    strcpy(filename, "*.*");           /* just use wildcards         */
}


/*********************************************************************/
/* Function: ULONG SameAttr(mask, attr)                              */
/*                                                                   */
/* Purpose:  Returns the value true if the attribute is identical to */
/*           that specified by the mask.  If not the same, then      */
/*           returns the value false.                                */
/*                                                                   */
/*********************************************************************/

ULONG SameAttr(
  INT   *mask,
  ULONG  attr,
  ULONG  options)
{

                                                /* if only want directories */
                                                /* and is not a directory   */
  if ((options&DO_DIRS) && !(options&DO_FILES) && !(attr&FILE_ATTRIBUTE_DIRECTORY))
     return false;
                                                /* if only want files and   */
                                                /* is a directory           */
  if (!(options&DO_DIRS) && (options&DO_FILES) && (attr&FILE_ATTRIBUTE_DIRECTORY))
     return false;

  if (mask[0] == RXIGNORE)
     return  true;


  if (mask[0] < 0 && attr&FILE_ATTRIBUTE_ARCHIVE)
    return  false;

  if (mask[0] > 0 && !(attr&FILE_ATTRIBUTE_ARCHIVE))
    return  false;

  if (mask[1] < 0 && attr&FILE_ATTRIBUTE_DIRECTORY)
    return  false;

  if (mask[1] > 0 && !(attr&FILE_ATTRIBUTE_DIRECTORY))
    return  false;

  if (mask[2] < 0 && attr&FILE_ATTRIBUTE_HIDDEN)
    return  false;

  if (mask[2] > 0 && !(attr&FILE_ATTRIBUTE_HIDDEN))
    return  false;

  if (mask[3] < 0 && attr&FILE_ATTRIBUTE_READONLY)
    return  false;

  if (mask[3] > 0 && !(attr&FILE_ATTRIBUTE_READONLY))
    return  false;

  if (mask[4] < 0 && attr&FILE_ATTRIBUTE_SYSTEM)
    return  false;

  if (mask[4] > 0 && !(attr&FILE_ATTRIBUTE_SYSTEM))
    return  false;

  return  true;
}


/*********************************************************************/
/* Function: ULONG NewAttr(mask, attr)                              */
/*                                                                   */
/* Purpose:  Returns the new file attribute, given the mask of       */
/*           attributes to be cleared/set and the current attribute  */
/*           settings.                                               */
/*                                                                   */
/*********************************************************************/

ULONG NewAttr(
  INT   *mask,
  ULONG  attr)
{


  if (mask[0] == RXIGNORE)
    return  attr;

  if (mask[0] < 0)
    attr &= ~FILE_ATTRIBUTE_ARCHIVE;            /* Clear                      */

  if (mask[0] > 0)
    attr |= FILE_ATTRIBUTE_ARCHIVE;             /* Set                        */

  if (mask[1] < 0)
    attr &= ~FILE_ATTRIBUTE_DIRECTORY;           /* Clear                      */

  if (mask[1] > 0)
    attr |= FILE_ATTRIBUTE_DIRECTORY;            /* Set                        */

  if (mask[2] < 0)
    attr &= ~FILE_ATTRIBUTE_HIDDEN;              /* Clear                      */

  if (mask[2] > 0)
    attr |= FILE_ATTRIBUTE_HIDDEN;               /* Set                        */

  if (mask[3] < 0)
    attr &= ~FILE_ATTRIBUTE_READONLY;            /* Clear                      */

  if (mask[3] > 0)
    attr |= FILE_ATTRIBUTE_READONLY;             /* Set                        */

  if (mask[4] < 0)
    attr &= ~FILE_ATTRIBUTE_SYSTEM;              /* Clear                      */

  if (mask[4] > 0)
    attr |= FILE_ATTRIBUTE_SYSTEM;               /* Set                        */
  return  attr;
}

/*********************************************************************/
/* Function: int   formatFile(                                       */
/*                                                                   */
/* Purpose:  Returns the new file attribute, given the mask of       */
/*           attributes to be cleared/set and the current attribute  */
/*           settings.                                               */
/*                                                                   */
/*********************************************************************/

int formatFile(
  RxTreeData   *ldp,                   /* Pointer to local data      */
  int          *smask,                 /* Mask of attributes to      */
                                       /* search for                 */
  int          *dmask,                 /* Mask of attributes to set  */
  ULONG         options,               /* Search and output format   */
  WIN32_FIND_DATA *wfd )               /* Find File data struct      */

{
  SYSTEMTIME systime;
  FILETIME ftLocal;
  ULONG nattrib;                       /* New file attributes        */


/* File-attributes need to be changed independent of the             */
/* output format                                                     */

  nattrib = NewAttr((int *)dmask,  wfd->dwFileAttributes);
                                       /* need to change?            */
  if (nattrib != wfd->dwFileAttributes)
                                    /* try to set attributes          */
     if (setFileMode(ldp->truefile, nattrib&~FILE_ATTRIBUTE_DIRECTORY))
        nattrib = wfd->dwFileAttributes;/* use old ones if it failed  */

  if (options&NAME_ONLY)               /* name only?                 */
    strcpy(ldp->Temp, ldp->truefile);  /* just copy it over          */

  else {
                                       /* Convert UTC to Local File  */
                                       /* Time,  and then to system  */
                                       /* format.                    */
    FileTimeToLocalFileTime(&wfd->ftLastWriteTime,&ftLocal);
    FileTimeToSystemTime(&ftLocal, &systime);

    if (options&LONG_TIME)             /* need the long time format? */
                                       /* format as such             */
      sprintf(ldp->Temp, "%4d-%02d-%02d %02d:%02d:%02d  %10lu  ",
        systime.wYear,
        systime.wMonth,
        systime.wDay,
        systime.wHour,
        systime.wMinute,
        systime.wSecond,
        wfd->nFileSizeLow);
    else
    {
      if (options&EDITABLE_TIME)       /* need the "smushed" form?   */
                                       /* format as such             */
      wsprintf(ldp->Temp, "%02d/%02d/%02d/%02d/%02d  %10lu  ",
        (systime.wYear+100)%100,
        systime.wMonth,
        systime.wDay,
        systime.wHour,
        systime.wMinute,
        wfd->nFileSizeLow);
      else                           /* give the pretty form       */
        wsprintf(ldp->Temp, "%2d/%02d/%02d  %2d:%02d%c  %10lu  ",
          systime.wMonth,
          systime.wDay,
          (systime.wYear+100)%100,
          (systime.wHour < 13 && systime.wHour != 0 ?
          systime.wHour:
          (abs(systime.wHour-(SHORT)12))),
          systime.wMinute,
          ((systime.wHour < 12 ||
          systime.wHour == 24)?'a':'p'),
          wfd->nFileSizeLow);
    }
                                       /* get the attributes            */

                                       /* format the attributes now  */
    wsprintf(ldp->Temp, "%s%c%c%c%c%c  %s", ldp->Temp,
      ((nattrib&FILE_ATTRIBUTE_ARCHIVE)?'A':'-'),
      ((nattrib&FILE_ATTRIBUTE_DIRECTORY)?'D':'-'),
      ((nattrib&FILE_ATTRIBUTE_HIDDEN)?'H':'-'),
      ((nattrib&FILE_ATTRIBUTE_READONLY)?'R':'-'),
      ((nattrib&FILE_ATTRIBUTE_SYSTEM)?'S':'-'),
      ldp->truefile);
  }

  ldp->count++;
  ldp->context->SetStemArrayElement(ldp->stem, ldp->count, ldp->context->NewStringFromAsciiz(ldp->Temp));
  return 0;                            /* good return                */
}

/*****************************************************************************
* Function: RecursiveFindFile( FileSpec, path, lpd, smask, dmask, options )  *
*                                                                            *
* Purpose:  Finds all files starting with FileSpec, and will look down the   *
*           directory tree if required.                                      *
*                                                                            *
* Params:   FileSpec - ASCIIZ string which designates filespec to search     *
*                       for.                                                 *
*           path     - ASCIIZ string for current path                        *
*                                                                            *
*           ldp      - Pointer to local data structure.                      *
*                                                                            *
*           smask    - Array of integers which describe the source attribute *
*                       mask.  Only files with attributes matching this mask *
*                       will be found.                                       *
*                                                                            *
*           dmask    - Array of integers which describe the target attribute *
*                       mask.  Attributes of all found files will be set     *
*                       using this mask.                                     *
*                                                                            *
*             Note:  Both source and targets mask are really arrays of       *
*                    integers.  Each index of the mask corresponds           *
*                    to a different file attribute.  Each indexe and         *
*                    its associated attribute follows:                       *
*                                                                            *
*                         mask[0] = FILE_ARCHIVED                            *
*                         mask[1] = FILE_DIRECTORY                           *
*                         mask[2] = FILE_HIDDEN                              *
*                         mask[3] = FILE_READONLY                            *
*                         mask[4] = FILE_SYSTEM                              *
*                                                                            *
*                    A negative value at a given index indicates that        *
*                    the attribute bit of the file is not set.  A positive   *
*                    number indicates that the attribute should be set.      *
*                    A value of 0 indicates a "Don't Care" setting.          *
*                                                                            *
*           options  - The search/output options.  The following options     *
*                       may be ORed together when calling this function:     *
*                                                                            *
*                    RECURSE     - Indicates that function should search     *
*                                   all child subdirectories recursively.    *
*                    DO_DIRS     - Indicates that directories should be      *
*                                   included in the search.                  *
*                    DO_FILES    - Indicates that files should be included   *
*                                   in the search.                           *
*                    NAME_ONLY   - Indicates that the output should be       *
*                                   restricted to filespecs only.            *
*                    EDITABLE_TIME - Indicates time and date fields should   *
*                                   be output as one timestamp.              *
*                    LONG_TIME   - Indicates time and date fields should     *
*                                   be output as one long formatted timestamp*
*                                                                            *
*****************************************************************************/

int  RecursiveFindFile(
  PSZ         FileSpec,                /* Filespecs to search for    */
  PSZ         path,                    /* current directory          */
  RxTreeData *ldp,                     /* Pointer to local data      */
  int        *smask,                   /* Mask of attributes to      */
                                       /* search for                 */
  int        *dmask,                   /* Mask of attributes to set  */
  ULONG       options )                /* Search and output format   */
                                       /* options                    */
{


  WIN32_FIND_DATA wfd;                 /* Find File data struct      */

  char  staticBuffer[MAX_PATH+1];      /* dynamic memory   */
  char  *tempfile = staticBuffer;      /* Used to hold temp file name*/
  HANDLE fHandle;                      /* search handle used by      */
                                       /* FindFirstFile()            */

  /* other changes not flagged (see all tempfile checks) */
                                       /* if > than static mem, use  */
                                       /* dynamic mem. dynamic mem   */
                                       /* must be FREED before func  */
                                       /* returns...                 */
  size_t maxsize = strlen(path) + strlen(ldp->TargetSpec);
                                       /* build spec name            */
  if (maxsize >= MAX_PATH) {
    tempfile = (char*) malloc(sizeof(char)*(maxsize+1));
  }
  wsprintf(tempfile, "%s%s", path, ldp->TargetSpec);
  if ((fHandle = FindFirstFile(tempfile,&wfd)) != INVALID_HANDLE_VALUE) {

                                       /* Get the rest of the files  */
    do {
                                       /* dot directory?             */
      if (!strcmp(wfd.cFileName, ".") ||
          !strcmp(wfd.cFileName, ".."))
          continue;                    /* skip this one              */
                                       /* got requested attributes?  */
      if (SameAttr(smask, wfd.dwFileAttributes, options)) {
                                       /* build the full name        */
        wsprintf(ldp->truefile, "%s%s", path,wfd.cFileName);
                                       /* passed back ok?            */
        if (formatFile(ldp, smask, dmask, options, &wfd)) {
          if (tempfile != staticBuffer) free(tempfile);
          return 1;                    /* error on non-zero          */
        }
      }
    } while (FindNextFile(fHandle,&wfd));
    FindClose(fHandle);
  }

  if (options&RECURSE) {               /* need to recurse?           */
    wsprintf(tempfile, "%s*", path);    /* build new target spec      */
                                       /* and have some              */
    if ((fHandle = FindFirstFile(tempfile,&wfd)) != INVALID_HANDLE_VALUE) {
      do {
                                       /* dot directory?             */
        if (!strcmp(wfd.cFileName, ".") ||
            !strcmp(wfd.cFileName, ".."))
          continue;                    /* skip this one              */
        maxsize = strlen(path) + strlen(wfd.cFileName) + 1;
        if (maxsize >= MAX_PATH) {
          if (tempfile != staticBuffer) {
            free(tempfile);
          }
          tempfile = (char*) malloc(sizeof(char)*(maxsize+1));
        }
                                       /* build the new directory    */
        wsprintf(tempfile, "%s%s\\", path, wfd.cFileName);
                                       /* search the next level      */
        if (RecursiveFindFile(ldp->TargetSpec, tempfile, ldp,
            smask, dmask, options)) {
          if (tempfile != staticBuffer) free(tempfile);
          return 1;                    /* error on non-zero          */
        }
      } while (FindNextFile(fHandle,&wfd));
      FindClose(fHandle);
    }
  }
  if (tempfile != staticBuffer) free(tempfile);
  return 0;                            /* finished                   */
}

/****************************************************************
* Function: GetUniqueFileName(Template, Filler, file)           *
*                                                               *
* Purpose:  This function returns a unique temporary file name  *
*           given a template and a filler character.            *
*                                                               *
* Params:   char* Template - The template.  Must contain at     *
*                            least one or more filler chars.    *
*                                                               *
*                            Example:  "C:\TEMP\FILE????.DAT    *
*                                                               *
*           char Filler    - The character in the Template to   *
*                            be replaced with numbers.  For     *
*                            the above example, the filler char *
*                            would be '?'.                      *
*           char* file     - file name produced (output)        *
*                                                               *
* Used By:  RxTempFileName()                                    *
****************************************************************/

VOID GetUniqueFileName(
  char  *Template,
  char   Filler,
  char  *file)
{

  char numstr[6];
  bool Unique = false;

  ULONG x,                             /* loop index                 */
        i,                             /*                            */
        j = 0,                         /* number of filler chars     */
                                       /* found                      */
        num,                           /* temporary random number    */
        start,                         /* first random number        */
        max = 1;                       /* maximum random number      */

  INT  seed;                           /* to get current time        */
  WIN32_FIND_DATA wfdFinfo;            // Windows Find data struct
                                       /* Structure                  */
  SYSTEMTIME DT;                       /* The date and time structure*/
  UINT            fuErrorMode;         /* holds current file err mode*/
  HANDLE hSearch;                      // handle of file if found

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
  GetSystemTime(&DT);                   /* via Windows                */

  seed = DT.wHour*60 + DT.wMinute;      /* convert to hundreths       */
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

    if (hSearch == INVALID_HANDLE_VALUE)/* file not found?            */
      Unique = true;                    /* got one                    */

    FindClose(hSearch);
    SetErrorMode(fuErrorMode);          // Enable previous setting

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

RexxFunction0(int, SysCls)
{
  HANDLE hStdout;                      /* Handle to Standard Out     */
  DWORD dummy;
  COORD Home = {0, 0};                 /* Home coordinates on console*/
  CONSOLE_SCREEN_BUFFER_INFO csbiInfo; /* Console information        */

  hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
                                       /* if in character mode       */
  if (GetConsoleScreenBufferInfo(hStdout, &csbiInfo)) {
    FillConsoleOutputCharacter( hStdout, ' ',
                                csbiInfo.dwSize.X * csbiInfo.dwSize.Y,
                                Home, &dummy );
    SetConsoleCursorPosition(hStdout, Home);

  }
  return 0;                            // this worked ok
}

/*************************************************************************
* Function:  SysCurPos - positions cursor in OS/2 session                *
*                                                                        *
* Syntax:    call SysCurPos [row, col]                                   *
*                                                                        *
* Params:    row   - row to place cursor on                              *
*            col   - column to place cursor on                           *
*                                                                        *
* Return:    row, col                                                    *
*************************************************************************/

RexxFunction2(RexxStringObject, SysCurPos, OPTIONAL_RexxUnsignedNumber, inrow, OPTIONAL_RexxUnsignedNumber, incol)
{
  COORD NewHome;                       /* Position to move cursor    */
  CONSOLE_SCREEN_BUFFER_INFO csbiInfo; /* Console information        */
  HANDLE hStdout;                      /* Handle to Standard Out     */


  if ((argumentExists(2) && argumentOmitted(3)) || (argumentExists(3) && argumentOmitted(2)))
  {
      context->InvalidRoutine();
      return NULL;
  }
                                       /* get handle to stdout       */
  hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

                                       /* get current position, and  */
                                       /* continue only if in        */
                                       /* character mode             */
  if (GetConsoleScreenBufferInfo(hStdout, &csbiInfo))
  {
    char buffer[256];

    sprintf(buffer, "%d %d", csbiInfo.dwCursorPosition.Y, csbiInfo.dwCursorPosition.X);

    if (argumentExists(2))
    {
        NewHome.Y = (SHORT)inrow;      /* convert to short form      */
        NewHome.X = (SHORT)incol;      /* convert to short form      */
                                       /* Set the cursor position    */
        SetConsoleCursorPosition(hStdout, NewHome);
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

RexxFunction1(int, SysCurState, CSTRING, option)
{
  CONSOLE_CURSOR_INFO CursorInfo;      /* info about cursor          */
  HANDLE hStdout;                      /* Handle to Standard Out     */

                                       /* get handle to stdout       */
  hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
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

RexxFunction1(RexxStringObject, SysDriveInfo, CSTRING, drive)
{
  char   chFileSysType[MAX_PATH],      //  File system name
         chVolumeName[MAX_PATH],       //  volume label
         chDriveLetter[4];             //  drive_letter + : + \ + \0
  BOOL   bGVIrc;                       // rc from GVI

                                       // GetDiskFreeSpace calculations
  BOOL   bGDFSrc;                      // GDFS rc
  UINT   errorMode;

  DWORD dwgle;
  unsigned __int64 freeBytesToCaller,
                   totalBytes,
                   freeBytes;

  // this must either be a single letter or a letter and a colon
  size_t len = strlen(drive);
  if (len < 1 || len > 2)
  {
      // raise error and quit
      context->InvalidRoutine();
      return NULLOBJECT;
  }

  // if second letter isn't : bye
  if (len == 2 && drive[1] != ':')
  {
      // raise error and quit
      context->InvalidRoutine();
      return NULLOBJECT;
  }

  // check it is in range
  if (drive[0] < 'A' || drive[0] > 'z')
  {
      // raise error and quit
      context->InvalidRoutine();
      return NULLOBJECT;
  }
  chDriveLetter[0]=drive[0];        // letter was passed in
  // add a colon and backslash to meet GetVolumeInfo requirements.
  strcpy(chDriveLetter + 1, ":\\");

  errorMode = SetErrorMode(SEM_FAILCRITICALERRORS);
                                       // get the volume name and file system type
  bGVIrc=GetVolumeInformation(chDriveLetter,
                           chVolumeName,
                           (DWORD)MAX_PATH,
                           NULL,
                           NULL,
                           NULL,
                           chFileSysType,
                           (DWORD)MAX_PATH);

  dwgle=GetLastError();

  bGDFSrc = GetDiskFreeSpaceEx(chDriveLetter,
            (PULARGE_INTEGER) &freeBytesToCaller,
            (PULARGE_INTEGER) &totalBytes,
            (PULARGE_INTEGER) &freeBytes);

  dwgle=GetLastError();
  SetErrorMode(errorMode);

  if (bGVIrc && bGDFSrc) {

    char buffer[256];
    /* use simplified display routine with 64 bit types */
    sprintf(buffer,                    // drive free total label
            "%c%c  %-12I64u %-12I64u %-13s",
            chDriveLetter[0], chDriveLetter[1],
            freeBytes, totalBytes, chVolumeName);
    return context->NewStringFromAsciiz(buffer);
  }
  else
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

RexxFunction2(RexxStringObject, SysDriveMap, OPTIONAL_CSTRING, drive, OPTIONAL_CSTRING, mode)
{

  char     temp[256];                  /* Entire drive map built here*/

  char     tmpstr[256];                /* Single drive entries built */
                                       /* here                       */
  char     DeviceName[3];              /* Device name or drive letter*/
                                       /* string                     */
  DWORD    DriveMap;                   /* Drive map                  */
  ULONG    Ordinal;                    /* Ordinal of entry in name   */
                                       /* list                       */
                                       /* required                   */
  size_t   dnum;                       /* Disk num variable          */
  size_t   start = 3;                  /* Initial disk num           */
  ULONG    Mode = USED;                /* Query mode USED, FREE,     */
                                       /* LOCAL, etc                 */
  LONG     rc;                         /* OS/2 return codes          */
  UINT     errorMode;

  Ordinal = 0;

  temp[0] = '\0';

                                       /* check starting drive letter*/
  if (drive != NULL)
  {
    if ((strlen(drive) == 2 && drive[1] != ':') ||
        strlen(drive) > 2 || strlen(drive) == 0)
    {
        // raise error and quit
        context->InvalidRoutine();
        return NULLOBJECT;
    }
    start = toupper(*drive) - 'A' + 1;
  }
  // in range?
  if (start < 1 || start > 26)
  {
      // raise error and quit
      context->InvalidRoutine();
      return NULLOBJECT;
  }
                                       /* check the mode             */
  if (mode != NULL)
  {
    if (stricmp(mode, "FREE") == 0)
      Mode = FREE;
    else if (stricmp(mode, "USED") == 0)
      Mode = USED;
    else if (stricmp(mode, "RAMDISK") == 0)
      Mode = RAMDISK;
    else if (stricmp(mode, "REMOTE") == 0)
      Mode = REMOTE;
    else if (stricmp(mode, "LOCAL") == 0)
      Mode = LOCAL;
    else if (stricmp(mode, "REMOVABLE") == 0)
      Mode = REMOVABLE;
    else if (stricmp(mode, "CDROM") == 0)
      Mode = CDROM;
    else
    {
        // raise error and quit
        context->InvalidRoutine();
        return NULLOBJECT;
    }
  }
                                       /* perform the query          */
  DriveMap = GetLogicalDrives();
  DriveMap>>=start-1;                  /* Shift to the first drive   */
  temp[0] = '\0';                      /* Clear temporary buffer     */

  for (dnum = start; dnum <= 26; dnum++) {

                                       /* Hey, we have a free drive  */
    if (!(DriveMap&(DWORD)1) && Mode == FREE) {
      sprintf(tmpstr, "%c: ", dnum+'A'-1);
      strcat(temp, tmpstr);
    }
                                       /* Hey, we have a used drive  */
    else if ((DriveMap&(DWORD)1) && Mode == USED) {
      sprintf(tmpstr, "%c: ", dnum+'A'-1);
      strcat(temp, tmpstr);
    }

    else if (DriveMap&(DWORD)1) {      /* Check specific drive info  */
      sprintf(DeviceName, "%c:\\", dnum+'A'-1);

      errorMode = SetErrorMode(SEM_FAILCRITICALERRORS);
      rc = (LONG)GetDriveType(DeviceName);
      SetErrorMode(errorMode);

      if (rc == DRIVE_REMOVABLE && Mode == REMOVABLE) {
                                       /* Hey, we have a removable   */
                                       /* drive                      */
        sprintf(tmpstr, "%c: ", dnum+'A'-1);
        strcat(temp, tmpstr);
      }

      else if (rc == DRIVE_CDROM && Mode == CDROM) {
        sprintf(tmpstr, "%c: ", dnum+'A'-1);
        strcat(temp, tmpstr);
      }

      else if (rc == DRIVE_FIXED && Mode == LOCAL) {
        sprintf(tmpstr, "%c: ", dnum+'A'-1);
        strcat(temp, tmpstr);
      }

      else if (rc == DRIVE_REMOTE && Mode == REMOTE) {
        sprintf(tmpstr, "%c: ", dnum+'A'-1);
        strcat(temp, tmpstr);
      }

      else if (rc == DRIVE_RAMDISK && Mode == RAMDISK) {
        sprintf(tmpstr, "%c: ", dnum+'A'-1);
        strcat(temp, tmpstr);
      }
    }
    DriveMap>>=1;                      /* Shift to the next drive    */
  }

  // if we have something, there is a trailing blank we need to remove
  if (*temp != '\0')
  {
      temp[strlen(temp) - 2] = '\0';
  }

  return context->NewStringFromAsciiz(temp);
}

/*************************************************************************
* Function:  SysFileDelete                                               *
*                                                                        *
* Syntax:    call SysFileDelete file                                     *
*                                                                        *
* Params:    file - file to be deleted.                                  *
*                                                                        *
* Return:    Return code from DosDelete() function.                      *
*************************************************************************/

RexxFunction1(int, SysFileDelete, CSTRING, name)
{
  return !DeleteFile(name) ? GetLastError() : 0;
}

/*************************************************************************
* Function:  SysFileSearch                                               *
*                                                                        *
* Syntax:    call SysFileSearch target, file, stem [, options]           *
*                                                                        *
* Params:    target  - String to search for.                             *
*            file    - Filespec to search.                               *
*            stem    - Stem variable name to place results in.           *
*            options - Any combo of the following:                       *
*                       'C' - Case sensitive search (non-default).       *
*                       'N' - Preceed each found string in result stem   *
*                              with it line number in file (non-default).*
*                                                                        *
* Return:    NO_UTIL_ERROR   - Successful.                               *
*            ERROR_NOMEM     - Out of memory.                            *
*************************************************************************/

RexxFunction4(int, SysFileSearch, CSTRING, target, CSTRING, file, RexxStemObject, stem, OPTIONAL_CSTRING, opts)
{
    char        line[MAX_LINE_LEN];      /* Line read from file        */
    char        ibuf[1024];              /* Input buffer               */
    size_t      ptr;                     /* Pointer to char str found  */
    size_t      num = 0;                 /* Line number                */
    size_t      len;                     /* Length of string           */
    size_t      len2;                    /* Length of string           */
    size_t      count = 0;               // count of returned items
    size_t      vlen;                    // length of returned line buffer
    int         rc = 0;                  /* Return code of this func   */
    bool        linenums = false;        /* Set true for linenums in   */
                                         /* output                     */
    bool        sensitive = false;       /* Set true for case-sens     */
                                         /* search                     */
    char *      buffer_pointer;          /* current buffer pointer     */
    GetFileData filedata;                /* file read information      */

    buffer_pointer = NULL;               /* nothing in buffer          */

    if (opts != NULL)                    /* process options            */
    {
        if (strstr(opts, "N") || strstr(opts, "n"))
            linenums = true;

        if (strstr(opts, "C") || strstr(opts, "c"))
            sensitive = true;
    }

    // open the file
    if (myOpenFile(file, &filedata))
    {
        return ERROR_FILEOPEN;
    }
    /* do the search...found lines*/
    /* are saved in stem vars     */
    while (!getLine(line, MAX_LINE_LEN - 1, &filedata))
    {
        len = strlen(line);
        num++;
        ptr = mystrstr(line, target, len, strlen(target), sensitive);

        if (ptr != '\0')
        {

            if (linenums)
            {
                wsprintf(ibuf, "%d ", num);
                len2 = strlen(ibuf);
                memcpy(ibuf+len2, line, min(len, sizeof(ibuf) - len2));
                vlen = min(sizeof(ibuf), len+len2);
            }
            else
            {
                memcpy(ibuf, line, len);
                vlen = len;
            }
            count++;

            if (ibuf[vlen - 1] == '\n')
            {
                vlen--;
            }
            context->SetStemArrayElement(stem, count, context->NewString(ibuf, vlen));
        }
    }

    closeFile(&filedata);                /* Close that file            */
    context->SetStemArrayElement(stem, 0, context->NumberToObject(count));
    return 0;
}

/*************************************************************************
* Function:  SysFileTree                                                 *
*                                                                        *
* Syntax:    call SysFileTree filespec, stem [, options]                 *
*                                                                        *
* Params:    filespec - Filespec to search for (may include * and ?).    *
*            stem     - Name of stem var to store results in.            *
*            options  - Any combo of the following:                      *
*                        'B' - Search for files and directories.         *
*                        'D' - Search for directories only.              *
*                        'F' - Search for files only.                    *
*                        'O' - Only output file names.                   *
*                        'S' - Recursively scan subdirectories.          *
*                        'T' - Combine time & date fields into one.      *
*                        'L' - Long time format                          *
*                                                                        *
* Return:    NO_UTIL_ERROR   - Successful.                               *
*            ERROR_NOMEM     - Out of memory.                            *
*************************************************************************/

RexxFunction5(int, SysFileTree, CSTRING, filespec, RexxStemObject, stem, OPTIONAL_CSTRING, opts, OPTIONAL_CSTRING, mask, OPTIONAL_CSTRING, newMask)
{
    char        buff1[SysFileSystem::MaximumFileNameBuffer];  /* buffer1 ...we may need to  */
    char        buff2[SysFileSystem::MaximumFileNameBuffer];              /* buffer2 ...alloc new mem...*/
    char       *FileSpec = buff1;        /* File spec to look for      */
    char       *path = buff2;            /* path to search along       */
    ULONG       options;                 /* Mask of options            */
    int         y;                       /* Temp counter (II)          */
    int         smask[5];                /* Source attribute mask      */
    int         dmask[5];                /* Target attribute mask      */
    RxTreeData  ldp;                     /* local data                 */

    ldp.context = context;               // pass along the context with the tree structure

    options = DO_FILES|DO_DIRS;          /* Clear if we should not     */
                                         /* display files              */
    smask[0] = RXIGNORE;                 /* No mask unless specified   */
    dmask[0] = RXIGNORE;                 /* No mask unless specified   */
    path[0] = '\0';                      /* no path yet                */

    size_t speclen = strlen(filespec);

    if (speclen > SysFileSystem::MaximumFileNameBuffer)
    {
        context->InvalidRoutine();
        return 0;
    }
    /* initialize data area       */
    ldp.count = 0;
    /* get file spec              */
    strcpy(FileSpec, filespec);

    /** If FileSpec ends in \ then append *.*  *                    */

    if (FileSpec[speclen - 1] == '\\')
    {
        strcat(FileSpec, "*.*");
    }
    /* in case of '.' or '..'     */
    /* append wildcard '\*.*'     */
    else if (FileSpec[speclen - 1] == '.')
    {
        strcat(FileSpec, "\\*.*");
    }

    if (opts != NULL)                    /* check third option         */
    {
        // while more characters
        while (*opts)
        {
            switch (toupper(*opts))
            {      /* process each option        */
                case 'S':                      /* recurse on subdirectories  */
                    options |= RECURSE;          /* Should we recurse          */
                    break;

                case 'O':                      /* only return names          */
                    options |= NAME_ONLY;        /* Should include names only  */
                    break;

                case 'T':                      /* editable time?             */
                    options |= EDITABLE_TIME;    /* create editable timestamp  */
                    break;

                case 'L':                      /* long time format?          */
                    options |= LONG_TIME;        /* create timestamp           */
                    break;

                case 'F':                      /* include only files?        */
                    options &= ~DO_DIRS;         /* Should not include dirs !  */
                    options |= DO_FILES;         /* Should include files !     */
                    break;

                case 'D':                      /* include only directories?  */
                    options |= DO_DIRS;          /* Should include dirs !      */
                    options &= ~DO_FILES;        /* Should not include files ! */
                    break;

                case 'B':                      /* include both files and dirs*/
                    options |= DO_DIRS;          /* Should include dirs !      */
                    options |= DO_FILES;         /* Should include files !     */
                    break;

                default:                       /* unknown option             */
                    context->InvalidRoutine();
                    return 0;
            }
            opts;                            /* step the option pointer    */
        }
    }

    if (mask != NULL)
    {                  /* check fourth option        */

        smask[0] = smask[1] = smask[2] = smask[3] = smask[4] = 0;
        if (strlen(mask) > 5)
        {            /* too long to be good?       */
            if (FileSpec != buff1)
            {         /* raise an error             */
                free(FileSpec);
                free(path);
            }
            context->InvalidRoutine();
            return 0;
        }
        y = 0;                             /* starting at the first      */
        while (*mask)
        {                  /* while still in the string  */

            if (*mask == '+')                /* turn it on?                */
                smask[y] = 1;                  /* set mask appropriately     */

            else if (*mask == '-')           /* turning it off?            */
                smask[y] = -1;                 /* use a negative             */
            else if (*mask == '*')           /* don't care?                */
                smask[y] = 0;                  /* that stays zero            */
            else
            {                           /* invalid setting            */
                context->InvalidRoutine();
                return 0;
            }
            y++;                             /* step to the next attribute */
            mask++;                          /* step the pointer           */
        }
    }

    if (newMask != NULL)
    {               /* check fifth argument       */
        dmask[0] = dmask[1] = dmask[2] = dmask[3] = dmask[4] = 0;
        if (strlen(newMask) > 5)
        {         /* too long to be good?       */
            context->InvalidRoutine();
            return 0;
        }
        y = 0;                             /* starting at the first      */
        while (*newMask)
        {                 /* while still in the string  */
            if (*newMask == '+')             /* turn it on?                */
                dmask[y] = 1;                  /* set mask appropriately     */

            else if (*newMask == '-')        /* turning it off?            */
                dmask[y] = -1;                 /* use a negative             */
            else if (*newMask == '*')        /* don't care?                */
                dmask[y] = 0;                  /* that stays zero            */
            else
            {                           /* invalid setting            */
                context->InvalidRoutine();
                return 0;
            }
            y++;                             /* step to the next attribute */
            newMask++;                       /* step the pointer           */
        }
        dmask[1] = 0;                      /* Ignore directory bit of    */
                                           /* destination mask           */
    }
    /* get path and name          */
    getpath(FileSpec, path, ldp.TargetSpec);
    /* recursively search         */
    if (RecursiveFindFile(FileSpec, path, &ldp, smask, dmask, options))
    {
        context->InvalidRoutine();
        return 0;
    }

    context->SetStemArrayElement(stem, 0, context->NumberToObject(ldp.count));
    /* did we allocate memory?    */
    return 0;
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

RexxFunction1(RexxStringObject, SysGetKey, OPTIONAL_CSTRING, opt)
{
  int       tmp;                       /* Temp var used to hold      */
                                       /* keystroke value            */
  bool      echo = true;               /* Set to false if we         */
                                       /* shouldn't echo             */

  if (opt != NULL)                 /* validate arguments         */
  {
      if (stricmp(opt, "NOECHO") == 0)
        echo = false;
      else if (stricmp(opt, "ECHO") != 0)
      {
          context->InvalidRoutine();
          return NULLOBJECT;
      }
  }
  if (ExtendedFlag) {                  /* if have an extended        */
    tmp = ExtendedChar;                /* get the second char        */
    ExtendedFlag = false;              /* do a real read next time   */
  }
  else {
    tmp = _getch();                    /* read a character           */

    if (SysProcess::running98())
    {
       /* The _getch() function and therefore the underlying OS function      */
       /* ReadConsoleInput does not works correctly for W´95 and 98.          */
       /* The function should read the console and remove the read character  */
       /* from the console input buffer. But it does not really remove it !   */
       /* For this a second ReadConsoleInput is executed.                     */
       /* Under NT 4.0 it works correctly                                     */

       HANDLE       hStdin = 0;
       INPUT_RECORD ConInpRec;
       DWORD        dNum;

       if (!(tmp == 0x00) || (tmp == 0xe0))          //if not an extended char
       {

         hStdin = GetStdHandle(STD_INPUT_HANDLE);    //get the console input handle

         ReadConsoleInput( hStdin,                   //read again one character from
                            &ConInpRec,              //the console input buffer
                            1,
                            &dNum );
       }
    }

                                       /* If a function key or arrow */
    if ((tmp == 0x00) || (tmp == 0xe0)) {
      ExtendedChar = _getch();         /* Read another character     */
      ExtendedFlag = true;
    }
    else
      ExtendedFlag = false;
  }
  if (echo)                            /* echoing?                   */
    _putch(tmp);                       /* write the character back   */

  char buffer[6];
  wsprintf(buffer, "%c", tmp);

  return context->NewStringFromAsciiz(buffer);
}

bool resolveStemObject(RexxCallContext *context, RexxObjectPtr stemBase, RexxStemObject stem)
{
    if (stemBase != NULL)
    {
        // go resolve the name
        stem = context->ResolveStemVariable(stemBase);
        if (stem != NULLOBJECT)
        {
            return true;
        }
    }
    context->InvalidRoutine();
    return false;
}


/**
 * Return all of the application names in the target INI file.
 *
 * @param context  Current call context.
 * @param iniFile  Source INI file.
 * @param stemBase The stem used to return the values, in either name or direct stem form.
 *
 * @return 0 if everything worked ok, non-zero for errors.
 */
RexxObjectPtr getAllApplicationNames(RexxCallContext *context, char *iniFile, RexxObjectPtr stemBase)
{
    RexxStemObject stem = NULL;
    if (!resolveStemObject(context, stemBase, stem))
    {
        return NULLOBJECT;
    }

    long   lSize = 0x0000ffffL;
    char *val;
    /* Allocate a large buffer    */
    if (!(val = (char *)GlobalAlloc(GPTR, lSize)))
    {
        return context->NumberToObject(ERROR_NOMEM);
    }
    /* Retrieve the names of all  */
    /* applications.              */
    lSize = GetPrivateProfileString(NULL, NULL, "", val, lSize, iniFile);
    if (lSize <= 0)
    {
        return context->NewStringFromAsciiz(ERROR_RETSTR);
    }

    size_t x = 0;
    size_t count = 0;

    do
    {
/* Copy string terminated by \0 to Temp.  Last string will end     */
/* in \0\0 and thus have a length of 0.                            */
        size_t len = 0;

        char *next = &val[x];          /* point to string            */
        len = strlen(next);            /* get string length          */
                                       /* if non-zero length, then   */
                                       /* set the stem element       */
        if (len != 0)
        {
            x += (len+1);                /* Increment pointer past the */
                                         /* new string                 */
            count++;
            context->SetStemArrayElement(stem, count, context->NewString(next, len));
        }
    }

    while (val[x] != '\0');

    GlobalFree(val);

    context->SetStemArrayElement(stem, 0, context->NumberToObject(count));
    return context->FalseObject();
}

/**
 * Return all of the application keys associated with an application.
 *
 * @param context  Current call context.
 * @param iniFile  Source INI file.
 * @param app      The application key name.
 * @param stemBase The stem used to return the values, in either name or direct stem form.
 *
 * @return 0 if everything worked ok, non-zero for errors.
 */
RexxObjectPtr getAllApplicationKeys(RexxCallContext *context, char *iniFile, char *app, RexxObjectPtr stemBase)
{
    RexxStemObject stem = NULL;
    if (!resolveStemObject(context, stemBase, stem))
    {
        return NULLOBJECT;
    }

    long   lSize = 0x0000ffffL;
    char *val;
    /* Allocate a large buffer    */
    if (!(val = (char *)GlobalAlloc(GPTR, lSize)))
    {
        return context->NumberToObject(ERROR_NOMEM);
    }
                                       /* Retrieve all keys for an   */
                                       /* application                */
    lSize = GetPrivateProfileString(app, NULL, "", val, lSize, iniFile);
    if (lSize <= 0)
    {
        return context->NewStringFromAsciiz(ERROR_RETSTR);
    }

    size_t x = 0;
    size_t count = 0;

    do
    {
/* Copy string terminated by \0 to Temp.  Last string will end     */
/* in \0\0 and thus have a length of 0.                            */
        size_t len = 0;

        char *next = &val[x];          /* point to string            */
        len = strlen(next);            /* get string length          */
                                       /* if non-zero length, then   */
                                       /* set the stem element       */
        if (len != 0)
        {
            x += (len+1);                /* Increment pointer past the */
                                         /* new string                 */
            count++;
            context->SetStemArrayElement(stem, count, context->NewString(next, len));
        }
    }

    while (val[x] != '\0');

    GlobalFree(val);

    context->SetStemArrayElement(stem, 0, context->NumberToObject(count));
    return 0;
}

/**
 * Delete all of the application keys associated with an application.
 *
 * @param context Current call context.
 * @param iniFile Source INI file.
 * @param app     The application key name.
 *
 * @return 0 if everything worked ok, non-zero for errors.
 */
RexxObjectPtr deleteAllApplicationKeys(RexxCallContext *context, char *iniFile, char *app)
{
                                       /* Delete application and all */
                                       /* associated keys            */
    return context->NumberToObject(WritePrivateProfileString(app, NULL, NULL, iniFile));
}


/**
 * Get a single application key.
 *
 * @param context Current call context.
 * @param iniFile Source INI file.
 * @param app     The application key name.
 * @param key     The value of the key.
 *
 * @return 0 if everything worked ok, non-zero for errors.
 */
RexxObjectPtr getSingleKey(RexxCallContext *context, char *iniFile, char *app, char *key)
{
    long lSize = 0x0000ffffL;
    char *val;
    /* Allocate a large buffer    */
    if (!(val = (char *)GlobalAlloc(GPTR, lSize)))
    {
        return context->NumberToObject(ERROR_NOMEM);
    }
                                     /* Retrieve a single key value*/
    lSize = GetPrivateProfileString(app, key, "", val, lSize, iniFile);

    if (lSize <= 0)
    {
        GlobalFree(val);
        return context->NewStringFromAsciiz(ERROR_RETSTR);
    }
    RexxObjectPtr result = context->NewString(val, lSize);
    GlobalFree(val);
    return result;
}

/**
 * Delete a single application key.
 *
 * @param context Current call context.
 * @param iniFile Source INI file.
 * @param app     The application key name.
 * @param key     The value of the key.
 *
 * @return 0 if everything worked ok, non-zero for errors.
 */
RexxObjectPtr deleteSingleKey(RexxCallContext *context, char *iniFile, char *app, char *key)
{
    return context->NumberToObject(WritePrivateProfileString(app, key, NULL, iniFile));
}

/**
 * Set a single application key.
 *
 * @param context Current call context.
 * @param iniFile Source INI file.
 * @param app     The application key name.
 * @param key     The value of the key.
 * @param val     The new key value.
 *
 * @return 0 if everything worked ok, non-zero for errors.
 */
RexxObjectPtr setSingleKey(RexxCallContext *context, char *iniFile, char *app, char *key, char *val)
{
    return context->NumberToObject(WritePrivateProfileString(app, key, val, iniFile));
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

RexxFunction4(RexxObjectPtr, SysIni, OPTIONAL_CSTRING, iniFile, CSTRING, app, RexxObjectPtr, key, OPTIONAL_RexxObjectPtr, val)
{
  if (iniFile == NULL)                 /* not specified?             */
  {
      iniFile = "WIN.INI";               /* default to WIN.INI         */
  }
                                       /* Check KEY and APP values   */
                                       /* for "WildCard"             */
  if (stricmp(app, "ALL:") == 0)
  {
      return getAllApplicationNames(context, iniFile, key);
  }

  // the key value will only be a string from this point forward
  char *keyValue = context->ObjectToStringValue(key);

  if (stricmp(keyValue, "ALL:") == 0)
  {
      return getAllApplicationKeys(context, iniFile, app, val);
  }
  if (stricmp(keyValue, "DELETE:") == 0)
  {
      return deleteAllApplicationKeys(context, iniFile, app);
  }
  // past here, the key value represents a key name...now we need to check the
  // next operation and see what we're supposed to do with the key.

  if (val == NULLOBJECT)
  {
      return getSingleKey(context, iniFile, app, keyValue);
  }

  char *valString = context->ObjectToStringValue(val);

  if (stricmp(valString, "DELETE:") == 0)
  {
      return deleteSingleKey(context, iniFile, app, keyValue);
  }

  return setSingleKey(context, iniFile, app, keyValue, valString);
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

RexxFunction1(int, SysMkDir, CSTRING, dir)
{
  ULONG rc = CreateDirectory(dir, NULL); /* make the directory */
  return rc == 0 ? 0 : GetLastError();
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

RexxFunction1(RexxStringObject, SysGetErrortext, int, errnum)
{
  char  *errmsg;

  if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM,NULL,errnum,0,(LPSTR)&errmsg,64,NULL) == 0)
  {
      return context->NullString();
  }
  else
  {
    return context->NewStringFromAsciiz(errmsg);
  }
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

RexxFunction1(int, SysWinEncryptFile, CSTRING, name)
{

  ULONG  rc;                           /* Ret code of func           */
  OSVERSIONINFO vi;

  vi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

  if (rc = GetVersionEx(&vi)) {
    // allow this only on W2K
    if (vi.dwPlatformId == VER_PLATFORM_WIN32_NT && vi.dwMajorVersion > 4)
    {
        rc = EncryptFile(name);
    }
    else
    {
      rc = 0;
      SetLastError(ERROR_CANNOT_MAKE);
    }
  }
  return rc == 0 ? 0 : GetLastError();
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

RexxFunction1(int, SysWinDecryptFile, CSTRING, name)
{

  ULONG  rc;                           /* Ret code of func           */
  OSVERSIONINFO vi;

    vi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

  if (rc = GetVersionEx(&vi)) {
    // allow this only on W2K
    if (vi.dwPlatformId == VER_PLATFORM_WIN32_NT && vi.dwMajorVersion > 4)
      rc = DecryptFile(name, 0);
    else {
      rc = 0;
      SetLastError(ERROR_CANNOT_MAKE);
    }
  }
  return rc == 0 ? 0 : GetLastError();
}


/*************************************************************************
* Function:  SysWinVer                                                   *
*                                                                        *
* Syntax:    call SysWinVer                                              *
*                                                                        *
* Return:    Windows Version                                             *
*************************************************************************/

RexxFunction0(RexxStringObject, SysWinVer)
{

  OSVERSIONINFO vi;                     // Changed by IH
  char chVerBuf[12];

  vi.dwOSVersionInfoSize = sizeof(vi);  // if not set --> violation error

  GetVersionEx(&vi);              // get version with extended api
  if (vi.dwPlatformId == VER_PLATFORM_WIN32s) strcpy(chVerBuf, "Windows");     // Windows 3.1
  else
    if (vi.dwPlatformId == VER_PLATFORM_WIN32_NT) strcpy(chVerBuf, "WindowsNT"); // Windows NT
  else strcpy(chVerBuf, "Windows95");                                // Windows 95


  char buffer[100];
                                       /* format into the buffer            */
  wsprintf(buffer, "%s %lu.%02lu",
             chVerBuf,
             vi.dwMajorVersion,
             vi.dwMinorVersion);

  return context->NewStringFromAsciiz(buffer);
}


/*************************************************************************
* Function:  SysVersion                                                  *
*                                                                        *
* Syntax:    call SysVersion                                             *
*                                                                        *
* Return:    Windows Version                                             *
*************************************************************************/

RexxFunction0(RexxStringObject, SysVersion)
{

  OSVERSIONINFO vi;                     // Changed by IH
  char chVerBuf[12];

  vi.dwOSVersionInfoSize = sizeof(vi);  // if not set --> violation error

  GetVersionEx(&vi);              // get version with extended api
  if (vi.dwPlatformId == VER_PLATFORM_WIN32s) strcpy(chVerBuf, "Windows");     // Windows 3.1
  else
    if (vi.dwPlatformId == VER_PLATFORM_WIN32_NT) strcpy(chVerBuf, "WindowsNT"); // Windows NT
  else strcpy(chVerBuf, "Windows95");                                // Windows 95


  char buffer[100];
                                       /* format into the buffer            */
  wsprintf(buffer, "%s %lu.%02lu",
             chVerBuf,
             vi.dwMajorVersion,
             vi.dwMinorVersion);

  return context->NewStringFromAsciiz(buffer);
}

/*************************************************************************
* Function:  SysRmDir                                                    *
*                                                                        *
* Syntax:    call SysRmDir dir                                           *
*                                                                        *
* Params:    dir - Directory to be removed.                              *
*                                                                        *
* Return:    NO_UTIL_ERROR                                               *
*            Return code from RemoveDirectory()                          *
*************************************************************************/

RexxFunction1(int, SysRmDir, CSTRING, dir)
{
  return RemoveDirectory(dir) == 0 ? 0 : GetLastError();
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

RexxFunction3(RexxStringObject, SysSearchPath, CSTRING, path, CSTRING, file, OPTIONAL_CSTRING, option)
{
  UCHAR    szFullPath[_MAX_PATH];      /* returned file name         */
  UCHAR    szCurDir[MAX_ENVVAR + _MAX_PATH];        /* current directory          */
  UCHAR    szEnvStr[MAX_ENVVAR];

  LPTSTR pszOnlyFileName;              // parm for searchpath
  LPTSTR lpPath;                       // ptr to search path+
  UINT   errorMode;

                                       // search current directory
  GetCurrentDirectory(_MAX_PATH, (LPSTR)szCurDir);
  lpPath=strcat((LPSTR)szCurDir,";");         //  and specified path

  if (GetEnvironmentVariable(path, (LPSTR)szEnvStr, MAX_ENVVAR))
     lpPath=strcat((char *)szCurDir, (char *)szEnvStr);    /* szEnvStr instead of lpEnv */

  if (option != NULL) {                /* process options            */

    if ((*option == 'N') || (*option == 'n'))
    {
      GetEnvironmentVariable(path, (LPSTR)szEnvStr, MAX_ENVVAR);
      lpPath = (LPSTR)szEnvStr;
    }
    else if ((*option == 'C') || (*option == 'c'));
                                       /* search current 1st(default)*/
    else
    {
        context->InvalidRoutine();
        return NULL;
    }
  }

  errorMode = SetErrorMode(SEM_FAILCRITICALERRORS);
  if (0 == SearchPath(
         (LPCTSTR)lpPath,                  // path srch, NULL will+
         (LPCTSTR)file,                    // address if filename
         NULL,                             // filename contains .ext
         _MAX_PATH,                        // size of fullname buffer
         (LPSTR)szFullPath,                       // where to put results
         (LPSTR *)&pszOnlyFileName))
      szFullPath[0]='\0';                  // set to NULL if failure

  SetErrorMode(errorMode);

  return context->NewStringFromAsciiz((RexxStringPointer)szFullPath);
}


/*************************************************************************
* Function:  SysSleep                                                    *
*                                                                        *
* Syntax:    call SysSleep secs                                          *
*                                                                        *
* Params:    secs - Number of seconds to sleep.                          *
*                                                                        *
* Return:    NO_UTIL_ERROR                                               *
*************************************************************************/

RexxFunction1(int, SysSleep, double, time)
{
    SysProcess::sleep((int)(time * 1000.0));
    return 0;
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

RexxFunction2(RexxStringObject, SysTempFileName, CSTRING, nameTemplate, OPTIONAL_CSTRING, fillerArg)
{

  char   filler = '?';                       /* filler character           */

  // was the filler given?
  if (fillerArg != NULL)
  {
      filler = *fillerArg;
  }
  char buffer[256];

  GetUniqueFileName(nameTemplate, filler, buffer);
  return context->NewStringFromAsciiz(buffer);
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
RexxFunction3(RexxStringObject, SysTextScreenRead, int, row, int, col, OPTIONAL_int, len)
{
  LONG  lPos,lPosOffSet;               /* positioning                */
                                       /* (132x50)                   */
  LONG lBufferLen = 16000;             /* default: 200x80 characters */

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

  coordLine.X = (SHORT)col;
  coordLine.Y = (SHORT)row;

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
    lBufferLen = len;

  lPos = 0;                                     /* current position */
  lPosOffSet = row * csbiInfo.dwSize.X + col;   /* add offset if not started at beginning */
  dwSumCharsRead = 0;

  while (lPos < len ) {

    if (!ReadConsoleOutputCharacter(hStdout, &ptr[lPos], lBufferLen, coordLine, &dwCharsRead))
    {
        context->InvalidRoutine();
        return NULL;
    }


    lPos = lPos + lBufferLen;
    coordLine.Y = (SHORT)((lPos + lPosOffSet) / csbiInfo.dwSize.X);
    coordLine.X = (SHORT)((lPos + lPosOffSet) % csbiInfo.dwSize.X);
    dwSumCharsRead = dwSumCharsRead + dwCharsRead;
  }

  return context->NewString(ptr, dwSumCharsRead);
}

/*************************************************************************
* Function:  SysTextScreenSize                                           *
*                                                                        *
* Syntax:    call SysTextScreenSize                                      *
*                                                                        *
* Return:    Size of screen in row and columns returned as:  row, col    *
*************************************************************************/

RexxFunction0(RexxStringObject, SysTextScreenSize)
{

  char buffer[100];
  HANDLE    hStdout;                   /* Handle to Standard Out     */

  CONSOLE_SCREEN_BUFFER_INFO csbiInfo; /* Console information        */

  hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
                                     /* if in character mode       */
  if (GetConsoleScreenBufferInfo(hStdout, &csbiInfo)) {

    wsprintf(buffer, "%d %d", csbiInfo.dwSize.Y, csbiInfo.dwSize.X);
    return context->NewStringFromAsciiz(buffer);
  }
  else
  {
      return context->NewStringFromAsciiz("0 0");
  }
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
* Return:    Error code                                                  *
*************************************************************************/

RexxFunction2(int, RxWinExec, CSTRING, command, OPTIONAL_CSTRING, show)
{

  ULONG       CmdShow;                 /* show style flags to winexec*/
  INT         index;                   /* table index                */

PSZ    show_styles[] =                 /* winexec show types         */
    {"SHOWNORMAL",
     "SHOWNOACTIVATE",
     "SHOWMINNOACTIVE",
     "SHOWMINIMIZED",
     "SHOWMAXIMIZED",
     "HIDE",
     "MINIMIZE"
     };

ULONG  show_flags[] =                  /* winexec show styles        */
    {SW_SHOWNORMAL,
     SW_SHOWNOACTIVATE,
     SW_SHOWMINNOACTIVE,
     SW_SHOWMINIMIZED,
     SW_SHOWMAXIMIZED,
     SW_HIDE,
     SW_MINIMIZE
    };


    // check the size limits
  if (strlen(command) > MAX_PATH)
  {
      context->InvalidRoutine();
      return 0;
  }

  CmdShow=0;                           /* initialize show flags      */
                                       /* validate arguments         */
  if (show == NULL)                    /* no valid button style?     */
    CmdShow += SW_SHOWNORMAL;          /* set default show style     */
  else {                               /* check various button styles*/
                                       /* search style table         */
    for (index = 0; index < sizeof(show_styles) / sizeof(PSZ); index++)
    {
        if (stricmp(show, show_styles[index]) == 0)
        {
            CmdShow += show_flags[index];  /* add to the style           */
            break;
        }
    }
                                       /* not found?                 */
    if (index == sizeof(show_styles)/sizeof(PSZ))
    {
        context->InvalidRoutine();
        return 0;
    }
  }
  return WinExec((LPSTR)command, CmdShow);
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

RexxFunction0(RexxStringObject, SysBootDrive)
{
  char buffer[256];

  // if we can get the system directory, just return the drive bit
  if (GetSystemDirectory(buffer, 255) > 0)
  {
      return context->NewString(buffer, 2);
  }

  return context->NullString();
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

RexxFunction0(RexxStringObject, SysSystemDirectory)
{
  char buffer[256];

  if (GetSystemDirectory(buffer, 255) <= 0)
  {
      // make this a null string if there's an error
      buffer[0] = '\0';
  }

  return context->NewStringFromAsciiz(buffer);
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

RexxFunction1(RexxStringObject, SysFileSystemType, OPTIONAL_CSTRING, drive)
{
  char chDriveLetter[4];

  if (drive != NULL)
  {
      size_t len = strlen(drive);

      if (len == 0 || len > 2 || (len == 2 && drive[1] != ':'))
      {
          context->InvalidRoutine();
          return NULLOBJECT;
      }
      chDriveLetter[0]=*drive;                     // letter was passed in
      chDriveLetter[1]=':';
      chDriveLetter[2]='\\';          // need to add \ because of
      chDriveLetter[3]='\0';        // bug in getvolumeinfo
      drive = chDriveLetter;
  }

  UINT errorMode = SetErrorMode(SEM_FAILCRITICALERRORS);
  char buffer[256];

  ULONG rc = GetVolumeInformation(
    drive,    // address of root directory of the file system
    NULL,    // address of name of the volume
    0,    // length of lpVolumeNameBuffer
    NULL,    // address of volume serial number
    NULL,    // address of system's maximum filename length
    NULL,    // address of file system flags
    buffer,  // address of name of file system
    sizeof(buffer)  // length of lpFileSystemNameBuffer
  );
  SetErrorMode(errorMode);

  if (rc == 0)
  {
      return context->NewStringFromAsciiz(buffer);
  }
  return context->NullString();
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

RexxFunction1(RexxStringObject, SysVolumeLabel, OPTIONAL_CSTRING, drive)
{
  char chDriveLetter[4];

  if (drive != NULL)
  {
      size_t len = strlen(drive);

      if (len == 0 || len > 2 || (len == 2 && drive[1] != ':'))
      {
          context->InvalidRoutine();
          return NULLOBJECT;
      }
      chDriveLetter[0]=*drive;                     // letter was passed in
      chDriveLetter[1]=':';
      chDriveLetter[2]='\\';          // need to add \ because of
      chDriveLetter[3]='\0';        // bug in getvolumeinfo
      drive = chDriveLetter;
  }

  char buffer[256];

  if (GetVolumeInformation(
    drive,    // address of root directory of the file system
    buffer,   //address of name of the volume
    sizeof(buffer),  // length of lpVolumeNameBuffer
    NULL,    // address of volume serial number
    NULL,    // address of system's maximum filename length
    NULL,    // address of file system flags
    NULL,    // address of name of file system
    0     // length of lpFileSystemNameBuffer
  ))
  {
      return context->NewStringFromAsciiz(buffer);
  }
  return context->NullString();
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

RexxFunction1(uintptr_t, SysCreateMutexSem, OPTIONAL_CSTRING, name)
{
  HANDLE    handle;                    /* mutex handle               */
  SECURITY_ATTRIBUTES sa={sizeof(SECURITY_ATTRIBUTES), NULL, true};

  handle = 0;                          /* zero the handle            */
  if (name != NULL) {                  /* request for named sem      */
                                       /* create it by name          */
    handle = CreateMutex(&sa, false, name);
    if (!handle)                            /* may already be created     */
                                       /* try to open it             */
      handle = OpenMutex(MUTEX_ALL_ACCESS, true, name);
  }
  else                                 /* unnamed semaphore          */
    handle = CreateMutex(&sa, false, NULL);
  return (uintptr_t)handle;
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

RexxFunction1(uintptr_t, SysOpenMutexSem, CSTRING, name)
{
  SECURITY_ATTRIBUTES sa={sizeof(SECURITY_ATTRIBUTES), NULL, true};

                                       /* get a binary handle        */
  return (uintptr_t)OpenMutex(MUTEX_ALL_ACCESS, true, name); /* try to open it             */
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

RexxFunction1(int, SysReleaseMutexSem, uintptr_t, h)
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

RexxFunction1(int, SysCloseMutexSem, uintptr_t, h)
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

RexxFunction2(int, SysRequestMutexSem, uintptr_t, h, OPTIONAL_int, timeout)
{
  int       rc;                        /* creation return code       */
  if (argumentOmitted(2))
  {
      timeout = INFINITE;       /* default is no timeout      */
  }
  rc = WaitForSingleObject((HANDLE)h, timeout);
  if (rc == WAIT_FAILED)
    return GetLastError();
  else
    return rc;                         /* format the return code     */
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

RexxFunction2(uintptr_t, SysCreateEventSem, OPTIONAL_CSTRING, name, OPTIONAL_CSTRING, reset)
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
                                       /* try to open it             */
      handle = OpenEvent(EVENT_ALL_ACCESS, true, name);
  }
  else                                 /* unnamed semaphore          */
    handle = CreateEvent(&sa, manual, false, NULL);

  return (uintptr_t)handle;
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

RexxFunction1(uintptr_t, SysOpenEventSem, CSTRING, name)
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

RexxFunction1(int, SysPostEventSem, uintptr_t, h)
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

RexxFunction1(int, SysResetEventSem, uintptr_t, h)
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

RexxFunction1(int, SysPulseEventSem, uintptr_t, h)
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

RexxFunction1(int, SysCloseEventSem, uintptr_t, h)
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

RexxFunction2(int, SysWaitEventSem, uintptr_t, h, OPTIONAL_int, timeout)
{
  int       rc;                        /* creation return code       */

  if (!argumentExists(2))
  {
      timeout = INFINITE;       /* default is no timeout      */
  }
                                       /* request the semaphore      */
  rc = WaitForSingleObject((HANDLE)h, timeout);
  if (rc == WAIT_FAILED)
    return GetLastError();
  else
    return rc;                         /* format the return code     */
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

RexxFunction2(int, SysSetPriority, RexxObjectPtr, classArg, RexxObjectPtr, levelArg)
{
    RexxNumber classLevel;               /* priority class             */
    RexxNumber level;                    /* priority level             */
    HANDLE    process;
    HANDLE    thread;
    DWORD     iclass=-1;

    process = GetCurrentProcess();
    thread = GetCurrentThread();

    if (context->ObjectToNumber(classArg, &classLevel))
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
        char *classStr = context->ObjectToStringValue(classArg);

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


    if (context->ObjectToNumber(levelArg, &level))
    {
        if (level < -15 || level > 15)
        {
            context->InvalidRoutine();
            return 0;
        }
    }
    else
    {
        char *levelStr = context->ObjectToStringValue(levelArg);

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

    return rc == 0 ? 0 : GetLastError();
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

RexxFunction1(RexxObjectPtr, SysQueryProcess, OPTIONAL_CSTRING, option)
{
  if (option == NULL || stricmp(option, "PID") == 0)
  {
      return context->NumberToObject(GetCurrentProcessId());
  }
  if (stricmp(option, "TID") == 0)
  {
      return context->NumberToObject(GetCurrentThreadId());
  }
  if (stricmp(option, "PPRIO") == 0)
  {
      LONG p;
      p = GetPriorityClass(GetCurrentProcess());

      switch(p) {
        case HIGH_PRIORITY_CLASS:
            return context->NewStringFromAsciiz("HIGH");
        case IDLE_PRIORITY_CLASS:
            return context->NewStringFromAsciiz("IDLE");
        case NORMAL_PRIORITY_CLASS:
            return context->NewStringFromAsciiz("NORMAL");
        case REALTIME_PRIORITY_CLASS:
            return context->NewStringFromAsciiz("REALTIME");
        default:
            return context->NewStringFromAsciiz("UNKNOWN");
      }
  }
  if (stricmp(option, "TPRIO") == 0)
  {
      LONG p;
      p = GetThreadPriority(GetCurrentThread());

      switch(p) {
        case THREAD_PRIORITY_ABOVE_NORMAL:
            return context->NewStringFromAsciiz("ABOVE_NORMAL");
        case THREAD_PRIORITY_BELOW_NORMAL:
            return context->NewStringFromAsciiz("BELOW_NORMAL");
        case THREAD_PRIORITY_HIGHEST:
            return context->NewStringFromAsciiz("HIGHEST");
        case THREAD_PRIORITY_IDLE:
            return context->NewStringFromAsciiz("IDLE");
        case THREAD_PRIORITY_LOWEST:
            return context->NewStringFromAsciiz("LOWEST");
            break;
        case THREAD_PRIORITY_NORMAL:
            return context->NewStringFromAsciiz("NORMAL");
            break;
        case THREAD_PRIORITY_TIME_CRITICAL:
            return context->NewStringFromAsciiz("TIME_CRITICAL");
        default:
            return context->NewStringFromAsciiz("UNKNOWN");
      }
  }
  if (stricmp(option, "PTIME") == 0 || stricmp(option, "TTIME") == 0)
  {
      FILETIME createT, kernelT, userT, dummy;
      SYSTEMTIME createST, kernelST, userST;
      BOOL ok;

      if (*option == 'T' || *option == 't')
          ok = GetThreadTimes(GetCurrentThread(), &createT,&dummy,&kernelT, &userT);
      else
          ok = GetProcessTimes(GetCurrentProcess(), &createT,&dummy,&kernelT, &userT);

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

          return context->NewStringFromAsciiz(buffer);
      }
  }
  context->InvalidRoutine();
  return NULLOBJECT;
}

/**********************************************************************
* Function:  SysShutDownSystem                                        *
*                                                                     *
* Syntax:    call SysShutDownSystem(<computer>,<message>,<timeout>,<appclose>,<reboot> *
*                                                                     *
* Params:    <computer> - name of the remote machine ('' = local)     *
*            <message>  - message for dialog                          *
*            <timeout>  - time to display message                     *
*            <appclose> - no dialog "save unsaved data"               *
*            <reboot>   - reboot the system                           *
*                                                                     *
* Return:    success (1) or failure (0) string                        *
**********************************************************************/

RexxFunction5(int, SysShutDownSystem, OPTIONAL_CSTRING, machine, OPTIONAL_CSTRING, message,
    OPTIONAL_uint32_t, timeout, OPTIONAL_RexxBoolean, appclose, OPTIONAL_RexxBoolean, reboot)
{
/* Display the shutdown dialog box and start the time-out countdown. */

  if (!InitiateSystemShutdown(
     machine,    // address of name of computer to shut down
     message,    // address of message to display in dialog box
     timeout,    // time to display dialog box
     appclose,   // force applications with unsaved changes flag
     reboot     // reboot flag
  ))
  {
      return GetLastError();
  }
  return 0;
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

RexxFunction1(int, SysSwitchSession, CSTRING, name)
{
  HWND hwnd;

  hwnd = FindWindow(NULL, name);

  if (hwnd)
  {
     if (!SetForegroundWindow(hwnd))
        return GetLastError();
     else
        return 0;
  }
  else
     return 1;
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

RexxFunction2(int, SysWaitNamedPipe, CSTRING, name, OPTIONAL_int, timeout)
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
      return 0;
  else
      return GetLastError();
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

RexxFunction3(int, SysSetFileDateTime, CSTRING, filename, OPTIONAL_CSTRING, newdate, OPTIONAL_CSTRING, newtime)
{
  BOOL      fOk = TRUE;
  HANDLE    setFile = NULL;
  FILETIME  sFileTime;
  FILETIME  sLocalFileTime;
  SYSTEMTIME sLocalSysTime;

  /* open output file for read/write for update */
  setFile = CreateFile(filename, GENERIC_WRITE | GENERIC_READ,
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
          fOk = false;

        if (sLocalSysTime.wYear < 1800)
          fOk = false;
      }

      if (newtime != NULL)
      {
        /* parse new time */
        if (sscanf(newtime, "%2hu:%2hu:%2hu", &sLocalSysTime.wHour,
                   &sLocalSysTime.wMinute, &sLocalSysTime.wSecond) != 3)
          fOk = false;
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
  else
    fOk = false;


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

RexxFunction2(RexxStringObject, SysGetFileDateTime, CSTRING, filename, OPTIONAL_CSTRING, selector)
{
    WIN32_FILE_ATTRIBUTE_DATA attributes;
    FILETIME  *fileTime = &attributes.ftLastWriteTime;

    if (GetFileAttributesEx((LPCSTR)filename, GetFileExInfoStandard, &attributes) == 0)
    {
        return context->NewStringFromAsciiz("-1");
    }

    if (selector != NULL)
    {
        switch (*selector)
        {
            case 'c':
            case 'C':
                fileTime = &attributes.ftCreationTime;
                break;
            case 'a':
            case 'A':
                fileTime = &attributes.ftLastAccessTime;
                break;
            case 'w':
            case 'W':
                fileTime = &attributes.ftLastWriteTime;
                break;
            default:
                context->InvalidRoutine();
                return NULLOBJECT;
        }
    }

    FILETIME  localFileTime;
    SYSTEMTIME localSysTime;

    FileTimeToLocalFileTime(fileTime, &localFileTime);
    FileTimeToSystemTime(&localFileTime, &localSysTime);
    char buffer[256];

    sprintf(buffer, "%4d-%02d-%02d %02d:%02d:%02d", localSysTime.wYear,
        localSysTime.wMonth, localSysTime.wDay, localSysTime.wHour,
        localSysTime.wMinute, localSysTime.wSecond);
    return context->NewStringFromAsciiz(buffer);
}

/*************************************************************************
* Function:  SysFromUnicode                                              *
*            Converts an UNICODE to an ASCII string                      *
*                                                                        *
* Syntax:    result = SysFromUniCode(string,CodePage,MappingFlags,       *
*                                    DefaultChar, outstem.)              *
*                                                                        *
* Params:    string       - unicode string to be converted               *
*            Codepage     - target codepage                              *
*            MappingFlags - Mapping flags                                *
*            DefaultChar  - default for unmappable chars                 *
*             outstem.    - stem containg the result                     *
*              .!USEDDEFAULTchar - 1: used; 2: not used                  *
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

  SEchar *S :
    Generate separate characters during conversion. This is the default conversion behavior.

  DEFAULTchar :
    Replace exceptions with the default character during conversion.

*************************************************************************/
RexxFunction5(int, SysFromUniCode, RexxStringObject, source, OPTIONAL_CSTRING, codepage, OPTIONAL_CSTRING, flags,
    OPTIONAL_CSTRING, defaultChar, RexxStemObject, stem)
{
    int   iBytesDestination;
    ULONG iBytesNeeded;
    DWORD dwFlags = 0;
    char  chDefaultChar = NULL;
    BOOL  bUsedDefaultChar = NULL;
    int codePage;
    char  szUsedDefChar[2];

    char *sourceStr = context->StringData(source);
    size_t length = context->StringLength(source);

    // evaluate codepage
    if (codepage != NULL)
        codePage = GetOEMCP();
    else
    {
        if (strstr(codepage, "THREAD_ACP"))
            codePage = CP_THREAD_ACP;
        else if (strstr(codepage, "ACP"))
            codePage = CP_ACP;
        else if (strstr(codepage, "MACCP"))
            codePage = CP_MACCP;
        else if (strstr(codepage, "OEMCP"))
            codePage = CP_OEMCP;
        else if (strstr(codepage, "SYMBOL"))
            codePage = CP_SYMBOL;
        else if (strstr(codepage, "UTF7"))
            codePage = CP_UTF7;
        else if (strstr(codepage, "UTF8"))
            codePage = CP_UTF8;
        else
            // assume this is a numeric codepage.
            codePage = atoi(codepage);
    }

    // evaluate the mapping flags
    if (flags != NULL)
    {
        if (strstr(flags, "COMPOSITECHECK")) dwFlags |= WC_COMPOSITECHECK;
        if (strstr(flags, "SEPCHARS"))       dwFlags |= WC_SEPCHARS;
        if (strstr(flags, "DISCARDNS"))      dwFlags |= WC_DISCARDNS;
        if (strstr(flags, "DEFAULTCHAR"))    dwFlags |= WC_DEFAULTCHAR;
        if (dwFlags == 0)
        {
            context->InvalidRoutine();
            return 0;
        }
    }

    // evaluate default charcter
    if (defaultChar != NULL)
    {
        chDefaultChar = *defaultChar;
    }

    /* Query the number of bytes required to store the Dest string */
    iBytesNeeded = WideCharToMultiByte( codePage,
        dwFlags,
        (LPWSTR) sourceStr,
        length,
        NULL,
        0,
        NULL,
        NULL);

    if (iBytesNeeded == 0) return GetLastError();  // call to function fails

    char * resultStr = (char *)GlobalAlloc(GMEM_FIXED|GMEM_ZEROINIT, (iBytesNeeded + 4));

    // hard error, stop
    if (resultStr == NULL)
    {
        context->InvalidRoutine();
        return 0;
    }

    if ( codePage == CP_UTF8 )
    {
        /* Do the conversion */
        /* in case of UTF8, the documentation says: When CP_UTF8 is set, dwFlags must be zero  */
        /* and both lpDefaultChar and lpUsedDefaultChar must be NULL.                          */

        iBytesDestination = WideCharToMultiByte(codePage,               //codepage
            dwFlags,                //conversion flags
            (LPWSTR) sourceStr,      //source string
            length,                  //source string length
            resultStr,              //target string
            iBytesNeeded,           //size of target buffer
            NULL,
            NULL);
    }
    else
    {
        /* Do the conversion */
        iBytesDestination = WideCharToMultiByte(codePage,               //codepage
            dwFlags,                //conversion flags
            (LPWSTR) sourceStr,
            length,                 // len,                     //source string length
            resultStr,              //target string
            iBytesNeeded,           //size of target buffer
            &chDefaultChar,
            &bUsedDefaultChar);

    }
    if (iBytesDestination == 0) // call to function fails
    {
        GlobalFree(resultStr);    //  free allocated string
        return GetLastError();    // return error from function call
    }

    // convert the default character flag to an character
    itoa(bUsedDefaultChar,szUsedDefChar,10);
    szUsedDefChar[1] ='\0';

    context->SetStemElement(stem, "!USEDDEFAULTchar", context->NewStringFromAsciiz(szUsedDefChar));
    context->SetStemElement(stem, "!TEST", context->NewString((RexxStringPointer)resultStr, (RexxStringLength)iBytesDestination));

    GlobalFree(resultStr);       // free allocated string
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

    PRECOMPOSED       Always use precomposed characters—that is, characters
                      in which a base character and a nonspacing character
                      have a single character value.
                      This is the default translation option.
                      Cannot be used with MB_COMPOSITE.
    COMPOSITE         Always use composite characters that is,
                      characters in which a base character and a nonspacing
                      character have different character values.
                      Cannot be used with MB_PRECOMPOSED.
    ERR_INVALID_charS If the function encounters an invalid input character,
                      it fails and GetLastError returns
                      ERROR_NO_UNICODE_TRANSLATION.
    USEGLYPHcharS     Use glyph characters instead of control characters.



*************************************************************************/
RexxFunction4(int, SysToUniCode, CSTRING, input, OPTIONAL_CSTRING, codepage, OPTIONAL_CSTRING, flags, RexxStemObject, stem)
{
  ULONG  ulDataLen, ulWCharsNeeded;
  DWORD  dwFlags = 0;
  UINT   codePage;
  LPWSTR lpwstr = NULL;

  // evaluate codepage
  if (codepage == NULL || strlen(codepage) == 0)
  {
      codePage = GetOEMCP();
  }
  else
  {
    if (strstr(codepage, "THREAD_ACP") == 0)
      codePage = CP_THREAD_ACP;
    else if (strstr(codepage, "ACP") == 0)
      codePage = CP_ACP;
    else if (strstr(codepage, "MACCP") == 0)
      codePage = CP_MACCP;
    else if (strstr(codepage, "OEMCP") == 0)
      codePage = CP_OEMCP;
    else if (strstr(codepage, "SYMBOL") == 0)
      codePage = CP_SYMBOL;
    else if (strstr(codepage, "UTF7") == 0)
      codePage = CP_UTF7;
    else if (strstr(codepage, "UTF8") == 0)
      codePage = CP_UTF8;
    else
      codePage = atoi(codepage);
  }

  // evaluate the mapping flags
  if (flags != NULL)
  {
    if (strstr(flags, "PRECOMPOSED"))   dwFlags |= MB_PRECOMPOSED;
    if (strstr(flags, "COMPOSITE"))     dwFlags  |= MB_COMPOSITE;
    if (strstr(flags, "ERR_INVALID"))   dwFlags |= MB_ERR_INVALID_CHARS;
    if (strstr(flags, "USEGLYPHCHARS")) dwFlags |= MB_USEGLYPHCHARS;
    if (dwFlags == 0)
    {
        context->InvalidRoutine();
        return 0;
    }
  }
  char buffer[256];

  /* Query the number of bytes required to store the Dest string */
  ulWCharsNeeded = MultiByteToWideChar( codePage,
                                        dwFlags,
                                        buffer,
                                        sizeof(buffer),
                                        NULL,
                                        NULL);

  if (ulWCharsNeeded == 0) return GetLastError();  // call to function fails

  ulDataLen = (ulWCharsNeeded)*2;

  // hard error, stop
  if (!(lpwstr = (LPWSTR)GlobalAlloc(GMEM_FIXED|GMEM_ZEROINIT, ulDataLen+4)))
  {
      context->InvalidRoutine();
      return 0;
  }


  /* Do the conversion */
  ulWCharsNeeded = MultiByteToWideChar( codePage,
                                        dwFlags,
                                        input,
                                        (int)strlen(input),
                                        lpwstr,
                                        ulWCharsNeeded);

  if (ulWCharsNeeded == 0) // call to function fails
  {
     GlobalFree(lpwstr);       // free allocated string
     return GetLastError();    // return error from function call
  }

  context->SetStemElement(stem, "!TEXT", context->NewString((RexxStringPointer)lpwstr, ulDataLen));

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

RexxFunction1(int, SysWinGetPrinters, RexxStemObject, stem)
{
  DWORD realSize = 0;
  DWORD entries = 0;
  BOOL  fSuccess = FALSE;
  char  szBuffer[256];
  PRINTER_INFO_2 *pResult;
  DWORD currentSize = 10*sizeof(PRINTER_INFO_2)*sizeof(char);
  char *pArray = (char*) malloc(sizeof(char)*currentSize);

  while (fSuccess == FALSE) {
    fSuccess = EnumPrinters(PRINTER_ENUM_LOCAL|PRINTER_ENUM_CONNECTIONS, NULL, 2, (LPBYTE)pArray, currentSize, &realSize, &entries);
    if (currentSize < realSize) {
      currentSize = realSize;
      realSize = 0;
      pArray = (char*) realloc(pArray, sizeof(char)*currentSize);
      fSuccess = FALSE;
    } else
      fSuccess = TRUE;
  }
  pResult = (PRINTER_INFO_2*) pArray;

  context->SetStemArrayElement(stem, 0, context->NumberToObject(entries));

  while (entries--)
  {
      sprintf(szBuffer,"%s,%s,%s",pResult[entries].pPrinterName,pResult[entries].pDriverName,pResult[entries].pPortName);
      context->SetStemArrayElement(stem, entries+1, context->NewStringFromAsciiz(szBuffer));
  }
  free(pArray);

  return fSuccess==TRUE ? 0 : 1;
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

RexxFunction0(RexxStringObject, SysWinGetDefaultPrinter)
{

  DWORD  errnum = 0;
  OSVERSIONINFO osv;

  // What version of Windows are you running?
  osv.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
  GetVersionEx(&osv);

  // If Windows 95 or 98, use EnumPrinters...
  if (osv.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS) {
    DWORD dwNeeded = 0;
    DWORD dwReturned;
    PRINTER_INFO_2 *printerInfo = NULL;

    // find out how much memory is needed
    EnumPrinters(PRINTER_ENUM_DEFAULT, NULL, 2, NULL, 0, &dwNeeded, &dwReturned);
    if (dwNeeded == 0)
    {
        context->InvalidRoutine();
        return NULLOBJECT;
    }

    printerInfo = (PRINTER_INFO_2*) malloc(sizeof(char)*dwNeeded);
    if (!printerInfo)
    {
        context->InvalidRoutine();
        return NULLOBJECT;
    }

    // fill in info
    if (! EnumPrinters(PRINTER_ENUM_DEFAULT, NULL, 2, (LPBYTE) printerInfo, dwNeeded, &dwNeeded, &dwReturned) ) {
      free(printerInfo);
      {
          context->InvalidRoutine();
          return NULLOBJECT;
      }
    }

    char printerName[256];
    lstrcpy(printerName, printerInfo->pPrinterName);

    free(printerInfo);
    return context->NewStringFromAsciiz(printerName);

  }
  else
  {
      char printerName[256];
      // NT / W2K:
      GetProfileString("Windows", "DEVICE", ",,,", printerName, sizeof(printerName));
      return context->NewStringFromAsciiz(printerName);
  }
}

/*************************************************************************
* Function:  SysWinSetDefaultPrinter                                     *
*                                                                        *
* Syntax:    call SysWinSetDefaultPrinter printer                        *
*                                                                        *
* Params:    string describing default printer                           *
*                                                                        *
* Return:    error number                                                *
*************************************************************************/

RexxFunction1(int, SysWinSetDefaultPrinter, CSTRING, name)
{
    DWORD  errnum = 0;
    size_t count = 0;
    OSVERSIONINFO osv;
    size_t commas = 0;

    /* just make sure the input string has valid format:
       it has to contain at least two commas */

    size_t length = strlen(name);
    for (count = 0; count < length; count++)
    {
        if (name[count] == ',')
            commas++;
    }

    if (commas < 2)
    {
        context->InvalidRoutine();
        return 0;
    }

    // What version of Windows are you running?
    osv.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx(&osv);

    // If Windows 95 or 98, use EnumPrinters...
    if (osv.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS)
    {
        // make a copy so we don't munge the real Rexx string
        char * pPrinterName = strdup(name);
        if (pPrinterName == NULL)
        {
            context->InvalidRoutine();
            return 0;
        }
        BOOL   bFlag;
        HANDLE hPrinter = NULL;
        PRINTER_INFO_2 *printerInfo = NULL;
        DWORD  dwNeeded = 0;

        for (count = 0; count < length; count++)
        {
            if (pPrinterName[count] == ',')
            {
                pPrinterName[count] = 0x00; // we only need the name
                break;
            }
        }

        // Open this printer so you can get information about it...
        bFlag = OpenPrinter(pPrinterName, &hPrinter, NULL);
        // we're done with the name
        free(pPrinterName);

        if (!bFlag || !hPrinter)
        {
            context->InvalidRoutine();
            return 0;
        }

        // The first GetPrinter() tells you how big our buffer should
        // be in order to hold ALL of PRINTER_INFO_2. Note that this will
        // usually return false. This only means that the buffer (the 3rd
        // parameter) was not filled in. You don't want it filled in here...
        GetPrinter(hPrinter, 2, 0, 0, &dwNeeded);
        if (dwNeeded == 0)
        {
            ClosePrinter(hPrinter);
            context->InvalidRoutine();
            return 0;
        }

        // Allocate enough space for PRINTER_INFO_2...
        printerInfo = (PRINTER_INFO_2*) malloc(sizeof(char)*dwNeeded);
        if (!printerInfo)
        {
            ClosePrinter(hPrinter);
            context->InvalidRoutine();
            return 0;
        }

        // The second GetPrinter() will fill in all the current information
        // so that all you need to do is modify what you're interested in...
        bFlag = GetPrinter(hPrinter, 2, (LPBYTE) printerInfo, dwNeeded, &dwNeeded);
        if (!bFlag)
        {
            ClosePrinter(hPrinter);
            free(printerInfo);
            context->InvalidRoutine();
            return 0;
        }

        // Set default printer attribute for this printer...
        printerInfo->Attributes |= PRINTER_ATTRIBUTE_DEFAULT;
        bFlag = SetPrinter(hPrinter, 2, (LPBYTE) printerInfo, 0);
        if (bFlag)
            SendNotifyMessage(HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM) "windows");

        ClosePrinter(hPrinter);
        free(printerInfo);
    }
    else
    {
        // NT / W2K
        WriteProfileString("Windows", "DEVICE", name);
        SendNotifyMessage(HWND_BROADCAST, WM_SETTINGCHANGE, 0, (LPARAM) "windows");
    }

    return GetLastError();
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

RexxFunction2(int, SysFileCopy, CSTRING, fromFile, CSTRING, toFile)
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

RexxFunction2(int, SysFileMove, CSTRING, fromFile, CSTRING, toFile)
{
    return MoveFile(fromFile, toFile) ? 0 : GetLastError();
}

/*************************************************************************
* Function:  SysFileExist                                                *
*                                                                        *
* Syntax:    call SysFileExist file                                      *
*                                                                        *
* Params:    file - file to check existance of.                          *
*                                                                        *
* Return:    Logical.                                                    *
*************************************************************************/

RexxFunction1(RexxBoolean, SysIsFile, CSTRING, file)
{
    DWORD dwAttrs = GetFileAttributes(file);
    return(dwAttrs == 0xffffffff) || (dwAttrs & (FILE_ATTRIBUTE_DIRECTORY | FILE_ATTRIBUTE_REPARSE_POINT)) ? 0 : 1;
}

/*************************************************************************
* Function:  SysDirExist                                                 *
*                                                                        *
* Syntax:    call SysDirExist dir                                        *
*                                                                        *
* Params:    dir - dir to check existance of.                            *
*                                                                        *
* Return:    Logical.                                                    *
*************************************************************************/

RexxFunction1(RexxBoolean, SysIsFileDirectory, CSTRING, file)
{
    DWORD dwAttrs = GetFileAttributes(file);
    return(dwAttrs != 0xffffffff) && (dwAttrs & FILE_ATTRIBUTE_DIRECTORY) ? 1 : 0;
}

/*************************************************************************
* Function:  SysIsFileLink                                               *
*                                                                        *
* Syntax:    call SysIsFileLink file                                     *
*                                                                        *
* Params:    file - file to check if it is a Link (Alias).               *
*                                                                        *
* Return:    Logical.                                                    *
*************************************************************************/

RexxFunction1(RexxBoolean, SysIsFileLink, CSTRING, file)
{
    DWORD dwAttrs = GetFileAttributes(file);
    return(dwAttrs != 0xffffffff) && (dwAttrs & FILE_ATTRIBUTE_REPARSE_POINT) ? 1 : 0;
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

RexxFunction1(RexxBoolean, SysIsFileCompressed, CSTRING, file)
{
    DWORD dwAttrs = GetFileAttributes(file);
    return(dwAttrs != 0xffffffff) && (dwAttrs & FILE_ATTRIBUTE_COMPRESSED) ? 1 : 0;
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

RexxFunction1(RexxBoolean, SysIsFileEncrypted, CSTRING, file)
{
    DWORD dwAttrs = GetFileAttributes(file);
    return(dwAttrs != 0xffffffff) && (dwAttrs & FILE_ATTRIBUTE_ENCRYPTED) ? 1 : 0;
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

RexxFunction1(RexxBoolean, SysIsFileNotContentIndexed, CSTRING, file)
{
    DWORD dwAttrs = GetFileAttributes(file);
    return(dwAttrs != 0xffffffff) && (dwAttrs & FILE_ATTRIBUTE_NOT_CONTENT_INDEXED) ? 1 : 0;
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

RexxFunction1(RexxBoolean, SysIsFileOffline, CSTRING, file)
{
    DWORD dwAttrs = GetFileAttributes(file);
    return(dwAttrs != 0xffffffff) && (dwAttrs & FILE_ATTRIBUTE_OFFLINE) ? 1 : 0;
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

RexxFunction1(RexxBoolean, SysIsFileSparse, CSTRING, file)
{
    DWORD dwAttrs = GetFileAttributes(file);
    return(dwAttrs != 0xffffffff) && (dwAttrs & FILE_ATTRIBUTE_SPARSE_FILE) ? 1 : 0;
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

RexxFunction1(RexxBoolean, SysIsFileTemporary, CSTRING, file)
{
    DWORD dwAttrs = GetFileAttributes(file);
    return(dwAttrs != 0xffffffff) && (dwAttrs & FILE_ATTRIBUTE_TEMPORARY) ? 1 : 0;
}
