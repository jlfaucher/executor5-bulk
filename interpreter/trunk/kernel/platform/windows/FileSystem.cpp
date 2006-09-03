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
/* REXX Kernel                                                  winfile.c     */
/*                                                                            */
/* Windows specific file related routines.                                    */
/*                                                                            */
/******************************************************************************/

#include "RexxCore.h"
#include "StringClass.hpp"
#include "RexxBuffer.hpp"
#include "SysInterpreter.hpp"
#include "SysFileSystem.hpp"
#include "Interpreter.hpp"
#include <string.h>
#include <io.h>
#include <fcntl.h>
#include <conio.h>
#define DEFEXT ".REX"
#define TEMPEXT ".CMD"                 /* Alternate extension   */

/*********************************************************************/
/*                                                                   */
/*   Subroutine Name:   resolveProgramName                           */
/*                                                                   */
/*   Function:          Expand a filename to a fully resolved REXX   */
/*                                                                   */
/*                                                                   */
/*********************************************************************/
RexxString * SysInterpreter::resolveProgramName(
   RexxString * name,                  /* starting filename                 */
   RexxString * parent )               /* parent program                    */
{
  char *    extension;                 /* parent file extensions            */
  char *    extensionArray[3];         /* array of extensions to check      */
  size_t    extensionCount;            /* count of extensions               */

  extensionCount = 0;                  /* Count of extensions               */
  if (parent != OREF_NULL) {           /* have one from a parent activation?*/
                       /* check for a file extension        */
    extension = SysFileSystem::extractFileExtension((char *)parent->getStringData());
    if (extension != NULL)             /* have an extension?                */
                       /* record this                       */
      extensionArray[extensionCount++] = extension;
  }
                       /* next check for .REX               */
  extensionArray[extensionCount++] = DEFEXT;
                       /* then check for .CMD               */
  extensionArray[extensionCount++] = TEMPEXT;
                       /* go do the search                  */
  return locateProgram(name, extensionArray, extensionCount);
}


/*********************************************************************/
/*                                                                   */
/*   Subroutine Name:   getInitialAddressForFile                     */
/*                                                                   */
/*   Function:          Map a source file name to an initial address */
/*                      environment.                                 */
/*                                                                   */
/*********************************************************************/
RexxString * SysInterpreter::getInitialAddressForFile(
   RexxString * name)                  /* starting filename                 */
{
  char *    extension;                 /* parent file extensions            */
                       /* check for a file extension        */
  extension = SysFileSystem::extractFileExtension((char *)name->getStringData());
  if (extension != NULL)
  {
      // use the extension as the initial address environment
      return new_string(extension);
  }
  return OREF_NULL;
}

/*********************************************************************/
/*                                                                   */
/* FUNCTION    : LocateProgram                                       */
/*                                                                   */
/* DESCRIPTION : Finds out if file name is minimally correct. Finds  */
/*               out if file exists. If it exists, then produce      */
/*               fullpath name.                                      */
/*                                                                   */
/*********************************************************************/
RexxString * SysInterpreter::locateProgram(
  RexxString * inName,                 /* name of rexx proc to check        */
  char *       extensions[],           /* array of extensions to check      */
  size_t       extensionCount )        /* count of extensions               */
{
  char       tempName[MAX_PATH + 2];   /* temporary name buffer             */
  char       fullName[MAX_PATH + 2];   /* temporary name buffer             */
  char *       name;                   /* ASCII-Z version of the name       */
  char *       extension;              /* start of file extension           */
  size_t       i;                      /* loop counter                      */
  size_t       extensionSpace;         /* room for an extension             */

  bool         found;                  /* found the file                    */


  name = (char *)inName->getStringData(); /* point to the string data          */
  found = false;                       /* no name found yet                 */

  extension = SysFileSystem::extractFileExtension(name);  /* locate the file extension start   */

  if (extension  != NULL)             /* have an extension?                */
  {
                       /* get space left for an extension   */
    extensionSpace = sizeof(tempName) - strlen(name);
                       /* loop through the extensions list  */
    for (i = 0; !found && i < extensionCount; i++) {
                       /* copy over the name                */
      strncpy(tempName, name, sizeof(tempName));
                       /* copy over the extension           */
      strncat(tempName, extensions[i], extensionSpace);
                       /* check this version of the name    */
      found = SysFileSystem::searchFileName(tempName, fullName);
    }
  }
  if (!found)                          /* not found?  try without extensions*/
                                       /* check on the "raw" name last      */
    found = SysFileSystem::searchFileName(name, fullName);
  if (found)                           /* got one?                          */
                       /* get as a string object            */
    return new_string(fullName);
  else
    return OREF_NULL;                  /* this wasn't found                 */
}

void SysInterpreter::loadImage(
  char  *imageName,
  char *&imageBuffer,                  /* returned start of the image       */
  size_t &imageSize)                   /* size of the image                 */
/*******************************************************************/
/* Function:  Load the image into storage                          */
/*******************************************************************/
{
  char      fullName[MAX_PATH + 2];    /* temporary name buffer             */
  HANDLE    fileHandle;                /* open file access handle           */
  size_t    bytesRead;                 /* number of bytes read              */

  LPTSTR ppszFilePart=NULL;            // file name only in buffer

  if ( !SearchPath(NULL,                // search default order
          (LPCTSTR)imageName,  // @ of filename
          NULL,                // @ of extension, no default
          MAX_PATH,            // len of buffer
          (LPTSTR)fullName,    // buffer for found
          &ppszFilePart) )
    RexxInterpreter::fatalError((stringchar_t *)"no startup image");   /* can't find it                     */

                       /* try to open the file              */
  fileHandle = CreateFile(fullName, GENERIC_READ, FILE_SHARE_READ,
                          NULL, OPEN_EXISTING, FILE_FLAG_WRITE_THROUGH, NULL);

  if (fileHandle == INVALID_HANDLE_VALUE)
    RexxInterpreter::fatalError((stringchar_t *)"no startup image");   /* can't find it                     */
                       /* Read in the size of the image     */
  ReadFile(fileHandle, &imageSize, sizeof(size_t), (LPDWORD)&bytesRead, NULL);
  imageBuffer = RexxMemory::memoryObject.allocateImageBuffer(imageSize);
                       /* read in the image                 */
  ReadFile(fileHandle, imageBuffer, (DWORD)imageSize, (LPDWORD)imageSize, NULL);
  CloseHandle(fileHandle);                /* and close the file                */
}

RexxBuffer *SysInterpreter::readProgram(
  RexxString *file_name,               /* program file name                 */
  ProtectedObject &source)             // source object protector
/*******************************************************************/
/* Function:  Read a program into a buffer                         */
/*******************************************************************/
{
  HANDLE        fileHandle;            /* open file access handle           */
  size_t        buffersize;            /* size of read buffer               */
  RexxBuffer * buffer;                 /* buffer object to read file into   */
  BY_HANDLE_FILE_INFORMATION   status; /* file status information           */
  size_t       bytesRead;              /* number of bytes read              */

  {
      // NOTE:  releasing kernel access in this area
      UnsafeBlock ublock;
                           /* try to open the file              */
      fileHandle = CreateFile((LPCSTR)file_name->getStringData(), GENERIC_READ, FILE_SHARE_READ,
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
  source = buffer;                     // protect this at the caller's level

  {
      // NOTE:  releasing kernel access in this area
      UnsafeBlock ublock;
                           /* read in a buffer of data   */
      if (ReadFile(fileHandle, buffer->data, (DWORD)buffersize, (LPDWORD)&bytesRead, NULL) == 0) {
        return OREF_NULL;                  /* return nothing                    */
      }
      CloseHandle(fileHandle);                /* close the file now         */
  }
  return buffer;                       /* return the program buffer         */
}


RexxString *SysInterpreter::qualifyFileSystemName(
  RexxString * name)                   /* stream information block          */
/*******************************************************************/
/* Function:  Qualify a stream name for this system                */
/*******************************************************************/
{
   char nameBuffer[SysFileSystem::MaximumFileNameBuffer];

                       /* clear out the block               */
   memset(nameBuffer, 0, sizeof(nameBuffer));
   SysFileSystem::qualifyStreamName((char *)name->getStringData(), nameBuffer, sizeof(nameBuffer)); /* expand the full name              */
                       /* uppercase this                    */
   strupr(nameBuffer);
                       /* get the qualified file name       */
   return new_string(nameBuffer);
}
