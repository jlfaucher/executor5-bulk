/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 1995, 2004 IBM Corporation. All rights reserved.             */
/* Copyright (c) 2005-2006 Rexx Language Association. All rights reserved.    */
/*                                                                            */
/* This program and the accompanying materials are made available under       */
/* the terms of the Common Public License v1.0 which accompanies this         */
/* distribution. A copy is also available at the following address:           */
/* http://www.oorexx.org/license.html                          */
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
/* REXX Kernel                                                  winfile.c     */
/*                                                                            */
/* Windows specific file related routines.                                    */
/*                                                                            */
/******************************************************************************/

#define  INCL_REXX_STREAM
#include "RexxCore.h"
#include "StringClass.hpp"
#include "BufferClass.hpp"
#include "RexxNativeAPI.h"
#include "ActivityManager.hpp"
#include "ProtectedObject.hpp"
#include "SysInterpreterInstance.hpp"
#include <string.h>
#include <io.h>
#include <fcntl.h>
#include <conio.h>
#define DEFEXT ".REX"
#define TEMPEXT ".CMD"                 /* Alternate extension   */
#define MAX_STDOUT_LENGTH     32767    /* max. amount of data to push to STDOUT @THU007A */ /* @HOL007M */
#include "StreamNative.h"              /* include the stream information    */

#define COMPILE_NEWAPIS_STUBS          /* Allows GetLongPathName to run on  */
#define WANT_GETLONGPATHNAME_WRAPPER   /* NT and Windows 95                 */
#include <NewAPIs.h>

bool FindFirstFile(const char *Name);
FILE * SysBinaryFilemode(FILE *, bool);
int SysFFlush(FILE *);
bool SysFileIsDevice(int fhandle);
int  SysPeekKeyboard(void);
int SysStat(const char * path, struct stat *buffer);
bool SysFileIsPipe(STREAM_INFO * stream_info);


/**
 * Extract directory information from a file name.
 *
 * @param file   The input file name.  If this represents a real source file,
 *               this will be fully resolved.
 *
 * @return The directory portion of the file name.  If the file name
 *         does not include a directory portion, then OREF_NULL is returned.
 */
RexxString *SystemInterpreter::extractDirectory(RexxString *file)
{
    const char *pathName = file->getStringData();
    const char *endPtr = pathName + file->getLength() - 1;

    // scan backwards looking for a directory delimiter.  This name should
    // be fully qualified, so we don't have to deal with drive letters
    while (pathName < endPtr)
    {
        // find the first directory element?
        if (*endPtr == '\\')
        {
            // extract the directory information, including the final delimiter
            // and return as a string object.
            return new_string(pathName, endPtr = pathName + 1);
        }
        endPtr--;
    }
    return OREF_NULL;      // not available
}


/**
 * Extract extension information from a file name.
 *
 * @param file   The input file name.  If this represents a real source file,
 *               this will be fully resolved.
 *
 * @return The extension portion of the file name.  If the file
 *         name does not include an extension portion, then
 *         OREF_NULL is returned.
 */
RexxString *SystemInterpreter::extractExtension(RexxString *file)
{
    const char *pathName = file->getStringData();
    const char *endPtr = pathName + file->getLength() - 1;

    // scan backwards looking for a directory delimiter.  This name should
    // be fully qualified, so we don't have to deal with drive letters
    while (pathName < endPtr)
    {
        // find the first directory element?
        if (*endPtr == '\\')
        {
            return OREF_NULL;    // found a directory portion before an extension...we're extensionless
        }
        // is this the extension dot?
        else if (*endPtr == '.')
        {
            // return everything from the period on.  Keeping the period on is a convenience.
            return new_string(endPtr);
        }
        endPtr--;
    }
    return OREF_NULL;      // not available
}


/**
 * Extract file nformation from a file name.
 *
 * @param file   The input file name.  If this represents a real source file,
 *               this will be fully resolved.
 *
 * @return The file portion of the file name.  If the file name
 *         does not include a directory portion, then the entire
 *         string is returned
 */
RexxString *SystemInterpreter::extractFile(RexxString *file)
{
    const char *pathName = file->getStringData();
    const char *endPtr = pathName + file->getLength() - 1;

    // scan backwards looking for a directory delimiter.  This name should
    // be fully qualified, so we don't have to deal with drive letters
    while (pathName < endPtr)
    {
        // find the first directory element?
        if (*endPtr == '\\')
        {
            // extract the directory information, including the final delimiter
            // and return as a string object.
            return new_string(endPtr);
        }
        endPtr--;
    }
    return file;     // this is all filename
}


/**
 * Resolve a program for intial loading or a subroutine call.
 *
 * @param _name      The target name.  This can be fully qualified, or a simple name
 *                   without an extension.
 * @param _parentDir The directory of the file of our calling program.  The first place
 *                   we'll look is in the same directory as the program.
 * @param _parentExtension
 *                   The extension our calling program has.  If there is an extension,
 *                   we'll use that version first before trying any of the default
 *                   extensions.
 *
 * @return A string version of the file name, if found.  Returns OREF_NULL if
 *         the program cannot be found.
 */
RexxString *SysInterpreterInstance::resolveProgram(RexxString *_name, RexxString *_parentDir, RexxString *_parentExtension)
{
    char resolvedName[CCHMAXPATH + 2];    // finally resolved name

    const char *name = _name->getStringData();
    const char *parentDir = _parentDir == OREF_NULL ? NULL : _parentDir->getStringData();
    const char *parentExtension = _parentExtension == OREF_NULL ? NULL : _parentExtension->getStringData();

    SysSearchPath searchPath(parentDir, parentExtension);


    // if the file already has an extension, this dramatically reduces the number
    // of searches we need to make.
    if (hasExtension(name))
    {
        if (searchName(name, searchPath.path, NULL, resolvedName))
        {
            return new_string(resolvedName);
        }
        return OREF_NULL;
    }

    // if we have a parent extension provided, use that in preference to any default searches
    if (parentExtension != NULL)
    {
        if (searchName(name, searchPath.path, parentExtension, resolvedName))
        {
            return new_string(resolvedName);
        }

    }

    // ok, now time to try each of the individual extensions along the way.
    size_t count = searchExtensions->size();
    for (size_t i = 1; i <= count; i++)
    {
        RexxString *ext = (RexxString *)searchExtensions->get(i);

        if (searchName(name, searchPath.path, ext->getStringData(), resolvedName))
        {
            return new_string(resolvedName);
        }
    }

    return OREF_NULL;
}


/**
 * Test if a filename has an extension.
 *
 * @param name   The name to check.
 *
 * @return true if an extension was found on the file, false if there
 *         is no extension.
 */
bool SysInterpreterInstance::hasExtension(const char *name)
{
    const char *endPtr = name + strlen(name) - 1

    // scan backwards looking for a directory delimiter.  This name should
    // be fully qualified, so we don't have to deal with drive letters
    while (name < endPtr)
    {
        // find the first directory element?
        if (*endPtr == '/')
        {
            return false;        // found a directory portion before an extension...we're extensionless
        }
        // is this the extension dot?
        else if (*endPtr == '.')
        {
            // return everything from the period on.  Keeping the period on is a convenience.
            return true;
        }
        endPtr--;
    }
    return false;          // not available
}


/**
 * Do a search for a single variation of a filename.
 *
 * @param name      The name to search for.
 * @param directory A specific directory to look in first (can be NULL).
 * @param extension A potential extension to add to the file name (can be NULL).
 * @param resolvedName
 *                  The buffer used to return the resolved file name.
 *
 * @return true if the file was located.  A true returns indicates the
 *         resolved file name has been placed in the provided buffer.
 */
bool SysInterpreterInstance::searchName(const char *name, const char *path, const char *extension, char *resolvedName)
{
    UnsafeBlock releaser;
    // this is for building a temporary name
    char       tempName[CCHMAXPATH + 2];

    // construct the search name, potentially adding on an extension
    strncpy(tempName, name, sizeof(tempName));
    if (extension != OREF)
    {
        strncat(tempName, extension, sizeof(tempName));
    }

    // check the file as is first
    if (checkCurrentFile(tempName, resolvedName))
    {
        return true;
    }

    if (searchPath(name, path, extension, resolvedName))
    {
        return true;
    }
}


/**
 * Try to locate a file using just the raw name passed in, as
 * opposed to searching along a path for the name.
 *
 * @param name   The name to use for the search.
 *
 * @return An RexxString version of the file name, iff the file was located. Returns
 *         OREF_NULL if the file did not exist.
 */
bool SysInterpreterInstance::checkCurrentFile(const char *name, char *resolvedName)
{
    size_t nameLength = strlen(name); /* get length of incoming name       */

    // make sure we have a valid length for even searching
    if (nameLength < 1 || nameLength > CCHMAXPATH)
    {
        return false;
    }

    // check the name in the current directory
    unsigned int errorMode = SetErrorMode(SEM_FAILCRITICALERRORS);
    LPTSTR ppszFilePart=NULL;            // file name only in buffer

    if (GetFullPathName(name, CCHMAXPATH, (LPTSTR)resolvedName, &ppszFilePart))
    {
        DWORD fileAttrib = GetFileAttributes((LPTSTR)resolvedName);

        // if this is a real file vs. a directory, make sure we return
        // the long name value in the correct casing
        if (fileAttrib != FILE_ATTRIBUTE_DIRECTORY)
        {
            getLongName(resolvedName, CCHMAXPATH);
            SetErrorMode(errorMode);
            return true;
        }
    }
    return false;        // not found
}


/**
 * Do a path search for a file.
 *
 * @param name      The name to search for.
 * @param path      The search path to use.
 * @param extension Any extension that should be added to the search (can be NULL).
 * @param resolvedName
 *                  A buffer used for returning the resolved name.
 *
 * @return Returns true if the file was located.  If true, the resolvedName
 *         buffer will contain the returned name.
 */
bool SysInterpreterInstance::searchPath(const char *name, const char *path, const char *extension, char *resolvedName)
{
    unsigned int errorMode = SetErrorMode(SEM_FAILCRITICALERRORS);

    LPTSTR ppszFilePart=NULL;            // file name only in buffer
    if (SearchPath((LPCTSTR)path, (LPCTSTR)name, (LPCTSTR)extension, CCHMAXPATH, (LPTSTR)resolvedlName, &ppszFilePart))
    {
        DWORD fileAttrib = GetFileAttributes((LPTSTR)fullName);

        // if this is a real file vs. a directory, make sure we return
        // the long name value in the correct casing
        if (fileAttrib != FILE_ATTRIBUTE_DIRECTORY)
        {
            getLongName(resolvedName, CCHMAXPATH);
            SetErrorMode(errorMode);
            return true;
        }
    }
    return false;        // not found
}


/**
 * Get the actual name value of a located file, in the exact case
 * used on the harddrive.
 *
 * @param fullName The buffer used for the name.
 * @param size     The size of the buffer.
 */
void SysInterpreterInstance::getLongName(char *fullName, size_t size)
{
  DWORD           length;
  WIN32_FIND_DATA findData;
  HANDLE          hFind;
  char           *p;

  if ( nSize >= CCHMAXPATH)
  {
      DWORD length = GetLongPathName(fullName, fullName, size);

      if ( 0 < length && length <= size )
      {
          WIN32_FIND_DATA findData;
          HANDLE hFind = FindFirstFile(fullName, &findData);
          if ( hFind != INVALID_HANDLE_VALUE )
          {
              p = strrchr(fullName, '\\');
              if ( p )
              {
                  strcpy(++p, findData . cFileName);
              }
              FindClose(hFind);
          }
      }
  }
  return;
}

// retrofit by IH
void SysLoadImage(
  char **imageBuffer,                  /* returned start of the image       */
  size_t *imageSize )                  /* size of the image                 */
/*******************************************************************/
/* Function:  Load the image into storage                          */
/*******************************************************************/
{
  char      FullName[CCHMAXPATH + 2];  /* temporary name buffer             */
  HANDLE    fileHandle;                /* open file access handle           */
  DWORD     bytesRead;                 /* number of bytes read              */

  LPTSTR ppszFilePart=NULL;            // file name only in buffer

  if ( !SearchPath(NULL,                // search default order
          (LPCTSTR)BASEIMAGE,  // @ of filename
          NULL,                // @ of extension, no default
          CCHMAXPATH,          // len of buffer
          (LPTSTR)FullName,    // buffer for found
          &ppszFilePart) )
    logic_error("no startup image");   /* can't find it                     */

                       /* try to open the file              */
  fileHandle = CreateFile(FullName, GENERIC_READ, FILE_SHARE_READ,
                          NULL, OPEN_EXISTING, FILE_FLAG_WRITE_THROUGH, NULL);

  if (fileHandle == INVALID_HANDLE_VALUE)
    logic_error("no startup image");   /* can't find it                     */
                       /* Read in the size of the image     */
  ReadFile(fileHandle, imageSize, sizeof(size_t), &bytesRead, NULL);
  *imageBuffer = memoryObject.allocateImageBuffer(*imageSize);
                       /* read in the image                 */
  ReadFile(fileHandle, *imageBuffer, (DWORD)*imageSize, &bytesRead, NULL);
  // set this to the actual size read.
  *imageSize = bytesRead;
  CloseHandle(fileHandle);                /* and close the file                */
}

// retrofit by IH
RexxBuffer *SysReadProgram(
  const char *file_name)               /* program file name                 */
/*******************************************************************/
/* Function:  Read a program into a buffer                         */
/*******************************************************************/
{
  HANDLE        fileHandle;             /* open file access handle           */
  size_t   buffersize;                 /* size of read buffer               */
  RexxBuffer * buffer;                 /* buffer object to read file into   */
  BY_HANDLE_FILE_INFORMATION   status; /* file status information           */
  DWORD        bytesRead;              /* number of bytes read              */

  {
      UnsafeBlock releaser;
                           /* try to open the file              */
      fileHandle = CreateFile(file_name, GENERIC_READ, FILE_SHARE_READ,
                              NULL, OPEN_EXISTING, FILE_FLAG_WRITE_THROUGH, NULL);
      if (fileHandle == INVALID_HANDLE_VALUE)
      {
        return OREF_NULL;                  /* return nothing                    */
      }
                           /* retrieve the file size            */
      GetFileInformationByHandle(fileHandle, &status);
  }
  buffersize = status.nFileSizeLow;    /* get the file size                 */
  buffer = new_buffer(buffersize);     /* get a buffer object               */
  ProtectedObject p(buffer);
  {
      UnsafeBlock releaser;

                           /* read in a buffer of data   */
      if (ReadFile(fileHandle, buffer->address(), (DWORD)buffersize, &bytesRead, NULL) == 0) {
        return OREF_NULL;                  /* return nothing                    */
      }
      CloseHandle(fileHandle);                /* close the file now         */
      return buffer;                       /* return the program buffer         */
  }
}

void SysQualifyStreamName(
  STREAM_INFO *stream_info )           /* stream information block          */
/*******************************************************************/
/* Function:  Qualify a stream name for this system                */
/*******************************************************************/
{
  LPTSTR  lpszLastNamePart;
  unsigned int errorMode;
                       /* already expanded?                 */
  if (stream_info->full_name_parameter[0] != '\0')
    return;                            /* nothing more to do                */
                       /* copy the name to full area        */
  strcpy(stream_info->full_name_parameter, stream_info->name_parameter);

  size_t namelen = strlen(stream_info->full_name_parameter);
                       /* name end in a colon?              */
  if (stream_info->full_name_parameter[namelen - 1] == ':') {
      // this could be the drive letter.  If so, make it the root of the current drive.
      if (namelen == 2)
      {
          strcat(stream_info->full_name_parameter, "\\");
      }
      else
      {
          // potentially a device, we need to remove the colon.
          stream_info->full_name_parameter[namelen - 1] = '\0';
          return;                            /* all finished                      */

      }
  }

  /* GetFullPathName doesn't work for COM? names without colon */
  if (RUNNING_95)
  {
      if (!strnicmp(stream_info->full_name_parameter, "com",3)
          && (stream_info->full_name_parameter[3] > '0') && (stream_info->full_name_parameter[3] <= '9'))
          return;
  }
                       /* get the fully expanded name       */
  errorMode = SetErrorMode(SEM_FAILCRITICALERRORS);
  GetFullPathName(stream_info->full_name_parameter, sizeof(stream_info->full_name_parameter), stream_info->full_name_parameter, &lpszLastNamePart);
  SetErrorMode(errorMode);
}

RexxString *SysQualifyFileSystemName(
  RexxString * name)                   /* stream information block          */
/*******************************************************************/
/* Function:  Qualify a stream name for this system                */
/*******************************************************************/
{
   STREAM_INFO stream_info;            /* stream information                */

                       /* clear out the block               */
   memset(&stream_info, 0, sizeof(STREAM_INFO));
                       /* initialize stream info structure  */
   strncpy(stream_info.name_parameter, name->getStringData(), path_length+10);
   strcpy(&stream_info.name_parameter[path_length+11], "\0");
   SysQualifyStreamName(&stream_info); /* expand the full name              */
                       /* uppercase this                    */
   strupr(stream_info.full_name_parameter);
                       /* get the qualified file name       */
   return new_string(stream_info.full_name_parameter);
}


bool SearchFirstFile(
  const char *Name)                     /* name of file with wildcards       */
{
   HANDLE FindHandle;
   WIN32_FIND_DATA FindData;
   unsigned int errorMode;

   errorMode = SetErrorMode(SEM_FAILCRITICALERRORS);
   FindHandle = FindFirstFile(Name, &FindData);
   SetErrorMode(errorMode);

   if (FindHandle != INVALID_HANDLE_VALUE)
   {
      FindClose(FindHandle);
      if ((FindData.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM)
      || (FindData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
      || (FindData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
     return false;
      else return true;
   }
   else return false;
}


FILE * SysBinaryFilemode(FILE * sfh, bool fRead)
{
     _setmode( _fileno( sfh ), _O_BINARY );
     return sfh;
}


bool SysFileIsDevice(int fhandle)
{
  if( _isatty( fhandle ) )
     return true;
   else
     return false;
}

int SysPeekKeyboard(void)
{
   return (_kbhit() != 0) ? 1 : 0;
}


int SysStat(const char * path, struct stat *buffer)
{
   unsigned int errorMode;
   int    retstat;

   errorMode = SetErrorMode(SEM_FAILCRITICALERRORS);
   retstat = stat(path, buffer);
   SetErrorMode(errorMode);
   return retstat;
}


bool SysFileIsPipe(STREAM_INFO * stream_info)
{
   struct _stat buf;

   if (_fstat( stream_info->fh, &buf )) return false;
   else
       return (buf.st_mode & _S_IFIFO) != 0;
}


int  SysTellPosition(STREAM_INFO * stream_info)
{
    if (SysFileIsDevice(stream_info->fh) || SysFileIsPipe(stream_info)) return -1;
    return ftell(stream_info->stream_file);
}

/* strem_info->stream_file -> sfile, tesul != length */
size_t line_write_check(const char * buffer, size_t length, FILE * sfile)
{
   size_t result;
   result = fwrite(buffer,1,length,sfile);
   if ((result != length) && (ferror(sfile)) && (errno == ENOMEM))
   {
     size_t ulMod;
     size_t ulTempValue;
     const char *pTemp = buffer;
     clearerr(sfile);  /* clear memory err, give a new chance */
     ulTempValue  = length / MAX_STDOUT_LENGTH;
     ulMod        = length % MAX_STDOUT_LENGTH;
     while ((ulTempValue > 0) && (!ferror(sfile)))
     {
        result += fwrite(pTemp,1,MAX_STDOUT_LENGTH, sfile);
        pTemp += MAX_STDOUT_LENGTH;
        ulTempValue--;
     }
     if (!ferror(sfile))
       result += fwrite(pTemp,1,ulMod, sfile);
   }
   return result;
}
