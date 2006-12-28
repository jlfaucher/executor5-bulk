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
/* Rexx Unix Support                                                          */
/*                                                                            */
/* Unix specific file related routines.                                       */
/*                                                                            */
/******************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "RexxCore.h"
#include "StringClass.hpp"
#include "RexxBuffer.hpp"
#include "Interpreter.hpp"
#include "SysInterpreter.hpp"
#include "SysFileSystem.hpp"
#include "SysUtil.hpp"
#include <string.h>
#include <stdio.h>
#include <stddef.h>
#include <sys/stat.h>
#include <limits.h>
#if defined(OPSYS_SUN)
#include <sys/filio.h>
#endif
#include <sys/ioctl.h>
#include <sys/types.h>
#include <unistd.h>
#ifdef HAVE_STROPTS_H
# include <stropts.h>
#endif

#define CCHMAXPATH PATH_MAX+1
#define DEFEXT  ".CMD"           /* leave default for AIX REXX programs too */
#define DEFEXT1  ".cmd"

extern char achRexxCurDir[ CCHMAXPATH+2 ];    /* Save current working direct */

char * SysFileExtension(char *);
char * SearchFileName(char *, char);
FILE * SysBinaryFilemode(FILE *);

/*********************************************************************/
/*                                                                   */
/*   Subroutine Name:   resolveProgramName                           */
/*                                                                   */
/*   Function:          Expand a filename to a fully resolved REXX   */
/*                                                                   */
/*                                                                   */
/*********************************************************************/
RexxString *  SysInterpreter::resolveProgramName(
   RexxString * name,                  /* starting filename                 */
   RexxString * parent)                /* parent program                    */
{
  char     *extension;                 /* parent file extensions            */
  const char *extensionArray[3];         /* array of extensions to check      */
  size_t    extensionCount;            /* count of extensions               */

  extensionCount = 0;                  /* Count of extensions               */
  if (parent != OREF_NULL) {           /* have one from a parent activation?*/
                                       /* check for a file extension        */
    extension = SysFileExtension((char *)parent->getStringData());
    if (extension != NULL)             /* have an extension?                */
                                       /* record this                       */
      extensionArray[extensionCount++] = extension;
  }
                                       /* then check for .CMD               */
  extensionArray[extensionCount++] = DEFEXT;
                                       /* and check for .cmd                */
  extensionArray[extensionCount++] = DEFEXT1;
                                       /* go do the search                  */
  return SysInterpreter::locateProgram(name, extensionArray, extensionCount);
}


/*********************************************************************/
/*                                                                   */
/* FUNCTION    : SysFileExtension                                    */
/*                                                                   */
/* DESCRIPTION : Looks for a file extension in given string. Returns */
/*               the ext in PSZ form. If no file ext returns an      */
/*               empty pointer.                                      */
/*                                                                   */
/*********************************************************************/

char * SysFileExtension(
  char *     Name )                     /* file name                         */
{
  char *     Scan;                      /* scanning pointer                  */
  int       Length;                    /* extension length                  */

  Scan = strrchr(Name, '/');           /* have a path?                      */
  if (Scan)                            /* find one?                         */
    Scan++;                            /* step over last slash in path      */
  else
    Scan = Name;                       /* no path, use name                 */

    /* Look for the last occurence of period in the name. If not            */
    /* found OR if found and the chars after the last period are all        */
    /* periods or spaces, then we do not have an extension.                 */

  if ((!(Scan = strrchr(Scan, '.'))) || strspn(Scan, ". ") == strlen(Scan))
    return NULL;                       /* just return a null                */

  Scan++;                              /* step over the period              */
  Length = strlen(Scan);               /* calculate residual length         */
  if (!Length)                         /* if no residual length             */
    return  NULL;                      /* so return null extension          */
  return --Scan;                       /* return extension position         */
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

RexxString *  SysInterpreter::locateProgram(
  RexxString * InName,                 /* name of rexx proc to check        */
  const char * Extensions[],           /* array of extensions to check      */
  size_t       ExtensionCount )        /* count of extensions               */
{
  stringchar_t TempName[CCHMAXPATH + 2]; /* temporary name buffer           */
  char *      Name;                    /* ASCII-Z version of the name       */
  char *      Extension;               /* start of file extension           */
  char *      Result;                  /* returned name                     */
  size_t     i;                        /* loop counter                      */
  long       ExtensionSpace;           /* room for an extension             */

  Name = (char *)InName->getStringData(); /* point to the string data       */

                                       /* extract extension from name       */
  Extension = SysFileExtension(Name);  /* locate the file extension start   */

  if (Extension)                       /* have an extension?                */
    ExtensionCount = 0;                /* no further extension processing   */
  Result = SearchFileName(Name, 'P');  /* check on the "raw" name first     */
  if (Result == OREF_NULL) {           /* not found?  try adding extensions */
                                       /* get space left for an extension   */
    ExtensionSpace = sizeof(TempName) - strlen(Name);
                                       /* loop through the extensions list  */
    for (i = 0; Result == OREF_NULL && i < ExtensionCount; i++) {
                                       /* copy over the name                */
      strncpy((char *)TempName, Name, sizeof(TempName));
                                       /* copy over the extension           */
      strncat((char *)TempName, Extensions[i], ExtensionSpace);
                                       /* check on the "raw" name first     */
      Result = SearchFileName((char *)TempName, 'P'); /* PATH search         */
    }
  }
  if ( Result == OREF_NULL )
     return (RexxString *)Result;      /* return     name  not found        */
  else
     return new_string(Result);       /* return the name                   */
}

/****************************************************************************/
/*                                                                          */
/* FUNCTION    : SearchFileName                                             */
/*                                                                          */
/* DESCRIPTION : Search for a given filename, returning the fully           */
/*               resolved name if it is found.                              */
/*               Control char A <=> Always return PATH+NAME w/o PATH search */
/*               Control char P <=> PATH search for existing file name      */
/*                                                                          */
/****************************************************************************/

char *SearchFileName(
  const char *Name,                    /* name of rexx proc to check         */
  char       chCont )                  /* Control char for search and output */
{
  static char achFullName[CCHMAXPATH + 2];/* temporary name buffer           */
  char       achTempName[CCHMAXPATH + 2]; /* temporary name buffer           */
  char *     p;
  char *     q;
  char *     enddir;
  char *     pszPath;

  long       NameLength;               /* length of name                    */
  struct stat dummy;                   /* structure for stat system calls   */
  bool       found=0;

  NameLength = strlen(Name);           /* get length of incoming name       */
                                       /* if name is too small or big       */
  if (NameLength < 1 || NameLength > CCHMAXPATH)
    return OREF_NULL;                  /* then Not a rexx proc name         */

  /* If the name contains a '/', then there is directory info in the name.  */
  /* Get the absolute directory name not via chdir and getcwd, because of   */
  /* performance problems (lines, linein and lineout directory resolution). */

  if ((enddir=strrchr(Name,'/')))      /* If there's directory info, enddir */
  {                                    /* points to end of directory name   */
    p = &achTempName[0];
    memcpy(p, Name, enddir-Name);      /* Copy path from name to p          */
    *(p+(enddir-Name))   = '\0';       /* Null-terminate it                 */
    *(p+(enddir-Name)+1) = '\0';       /* Null-terminate the search of >.<  */
    achFullName[0] = '\0';             /* Reset                             */

    switch(*p)
    {
      case '~':
           if ( *(p+1) == '\0' )
           {
              strcpy( achFullName, getenv("HOME"));
              strncat( achFullName, Name+1,
                                         ( CCHMAXPATH - strlen(achFullName)) );
              break;
           }
           if ( *(p+1) == '/' )
           {
              strcpy( achFullName, getenv("HOME"));
              p = p + 2;
           }
      case '.':
           if ( *(p+1) == '\0' )
           {
              strcpy(achFullName, achRexxCurDir);
              strncat( achFullName, (Name+1),
                                         ( CCHMAXPATH - strlen(achFullName)) );
              break;
           }
           if ( *(p+1) == '/' )
           {
              strcpy(achFullName, achRexxCurDir);
              p = p + 2;
           }
           if ( ( *(p+1) == '.' ) && ( *(p+2) == '\0' ) )
           {
              p = p + 2;
              if ( achFullName[0] == '\0' )
              {
                enddir=strrchr(achRexxCurDir,'/');              /* Copy path */
                memcpy(achFullName, achRexxCurDir, enddir-(&achRexxCurDir[0]));
                achFullName[enddir-(&achRexxCurDir[0])] = '\0'; /* Terminate */
              } else {
                enddir=strrchr(achFullName,'/');              /* Copy path */
                achFullName[enddir-(&achFullName[0])] = '\0'; /* Terminate */
              }
              strncat( achFullName, (Name+(p-(&achTempName[0]))),
                                         ( CCHMAXPATH - strlen(achFullName)) );
              break;
           }
           if ( ( *(p+1) == '.' ) && ( *(p+2) == '/' ) )
           {
              p = p + 3;
              if ( achFullName[0] == '\0' )
              {
                enddir=strrchr(achRexxCurDir,'/');              /* Copy path */
                memcpy(achFullName, achRexxCurDir, enddir-(&achRexxCurDir[0]));
                achFullName[enddir-(&achRexxCurDir[0])] = '\0'; /* Terminate */
              } else {
                enddir=strrchr(achFullName,'/');              /* Copy path */
                achFullName[enddir-(&achFullName[0])] = '\0'; /* Terminate */
              }
              for ( ;
                   ( ( *p == '.' ) && ( *(p+1) == '.' ) );
                   p = p + 3)
              {
                 if (enddir==strrchr(achFullName,'/'))           /* Copy path */
                   achFullName[enddir-(&achFullName[0])] = '\0'; /* Terminate */
              }
           }
           strncat( achFullName, (Name+(p-(&achTempName[0]))-1),
                                         ( CCHMAXPATH - strlen(achFullName)) );
        break;
      default:
              strcpy(achFullName, Name);
    }  /* endswitch */

    if (stat(achFullName,&dummy))             /* look for file              */
      found = 0;                              /* Give up - we can't find it */
    else                                      /*                            */
      found = 1;                              /* Tell user we found it      */

    if (found || (chCont == 'A'))
      return (char *)achFullName;
    else
      return OREF_NULL;

  }                                       /* End if dir info present    */

  /* Otherwise, there's no directory info, so we must use the PATH      */
  /*             environment variables to find the file.                */

  if ((!stat(Name, &dummy)) || (chCont == 'A'))  /* First try current dir      */
  {
    strcpy(achFullName, achRexxCurDir);   /* Copy current directory in  */
    strcat(achFullName,"/");              /* Put in a final slash       */
    strcat(achFullName, Name);            /* Now add name to end        */
    found = 1;                            /* Tell caller we found it    */
  }
  if (!found && (chCont == 'P' ))         /* Not in current dir         */
  {
    pszPath = getenv("PATH");             /* Get PATH                   */
    if (!pszPath)                         /* No PATH or REXXPATH?       */
    {
      return OREF_NULL;                   /* Didn't find the file       */
    }
    /* Now we have one contiguous string with all the directories       */
    /* that must be searched listed in order.                           */
    NameLength = strlen(pszPath);
    found = 0;
                                          /* For every dir in searchpath*/
    for (p=pszPath, q = strchr(p,':');
         p < pszPath+NameLength;
         p = q+1, q = strchr(p,':')) {
      if (!q)                             /* Is there a terminating ':'?*/
        q = p + strlen(p);                /* Make q point to \0         */
      memcpy(achTempName, p, q-p);        /* Copy this dir to tempname  */
      achTempName[q-p] = '/';             /* End dir with slash         */
      strcpy(&achTempName[q-p+1], Name);  /* Append name                */

      /* If we do find this file, it's possible that the directory      */
      /* that we used may not have been a complete directory            */
      /* specification from the root.  If so, as above, we must         */
      /* get the full directory.                                        */
      if ( achTempName[0] == '~' )
      {
          strcpy( achFullName, getenv("HOME"));
          strcat( achFullName, &achTempName[1]);
      }
      else
         strcpy(achFullName, achTempName);

      if (!stat(achFullName, &dummy))     /* If file is found,          */
      {
        found = 1;                        /* Tell user we found it      */
        break;                            /* Break out of loop          */
      }  /* endif */
    }  /* endfor */
  } /* endif */

  if (found)
    return (char *)achFullName;
  else
    return OREF_NULL;

}

void SysInterpreter::loadImage(char *name, char *&imageBuffer, size_t &imageSize)
/*******************************************************************/
/* Function : Load the image into storage                          */
/*******************************************************************/
{
  FILE *image = NULL;
  char      *fullname;

  fullname = SearchFileName(BASEIMAGE, 'P');  /* PATH search         */

  if (fullname != NULL )
    image = fopen(fullname, "rb");/* try to open the file              */
  else
    RexxInterpreter::fatalError((const stringchar_t *)"no startup image");   /* open failure                      */

                                       /* Read in the size of the image     */
  if(!fread(&imageSize, 1, sizeof(long), image))
    RexxInterpreter::fatalError((const stringchar_t *)"could not check the size of the image");

  /* Create an object the size of the  */
  /* image. We will be overwriting the */
  /* object header.                    */
  imageBuffer = (char *)RexxMemory::memoryObject.allocateImageBuffer(imageSize);
                                       /* read in the image, store the      */
                                       /* the size read                     */
  if (!(imageSize = fread(imageBuffer, 1, imageSize, image)))
    RexxInterpreter::fatalError((const stringchar_t*)"could not read in the image");
  fclose(image);                       /* and close the file                */
}


RexxBuffer *SysReadProgram(
  char * file_name)                     /* program file name                 */
/*******************************************************************/
/* Function:  Read a program into a buffer                         */
/*******************************************************************/
{
  FILE    *handle;                     /* open file access handle           */
  size_t   buffersize;                 /* size of read buffer               */
  RexxBuffer * buffer;                 /* buffer object to read file into   */
  RexxActivity*activity;               /* the current activity              */


  {
      // NOTE:  releasing kernel access in this area
      UnsafeBlock ublock;
      handle = fopen(file_name, "rb");     /* open as a binary file             */
      if (handle == NULL){                 /* open error?                       */
        return OREF_NULL;                  /* return nothing                    */
      }

      if (fileno(handle) == (FOPEN_MAX - 2)){      /* open error?                       */
        return OREF_NULL;                  /* return nothing                    */
      }

      fseek(handle, 0, SEEK_END);          /* seek to the file end              */
      buffersize = ftell(handle);          /* get the file size                 */
      fseek(handle, 0, SEEK_SET);          /* seek back to the file beginning   */
  }
  // We now have kernel access again...we're allowed to allocate objects now
  buffer = new_buffer(buffersize);     /* get a buffer object               */
  ProtectedObject p1(buffer);
  {
      // NOTE:  releasing kernel access in this area
      UnsafeBlock ublock;
                                           /* read the entire file in one shot  */
      fread(buffer->data, 1, buffersize, handle);
      fclose(handle);                      /* close the file                    */

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
   SysUtil::strupr(nameBuffer);
                       /* get the qualified file name       */
   return new_string(nameBuffer);
}

