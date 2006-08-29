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
/* Unix specific external function lookup and Unix built-in functions.        */
/*                                                                            */
/******************************************************************************/

/******************************************************************************/
/*  aixextf.c - Methods to resolve external function calls.                   */
/*                                                                            */
/******************************************************************************/
#include <stdio.h>                          /* Get printf, FILE type, etc.    */
#include <string.h>                         /* Get strcpy, strcat, etc.       */
#include <ctype.h>                          /* Get toupper                    */
#include <unistd.h>                         /* get getcwd routine/environ     */
#include <limits.h>                         /* Get PATH_MAX                   */
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <pwd.h>

#include "RexxCore.h"                         /* global REXX definitions        */
#include "StringClass.hpp"
#include "ArrayClass.hpp"
#include "RexxActivity.hpp"
#include "RexxActivation.hpp"
#include "MethodClass.hpp"
#include "SourceFile.hpp"
#include "InternalAPI.hpp"                      /* Lot's of useful REXX macros    */
#include "oorexx.h"                           /* Include REXX header            */
#include "SysInterpreter.hpp"

#define FILESPEC_PATH         'P'
#define FILESPEC_NAME         'N'
#define CCHMAXPATH PATH_MAX+1

extern char **environ;

extern char achRexxCurDir[ CCHMAXPATH+2 ];  /* Save current working direct    */

typedef struct _ENVENTRY {                  /* setlocal/endlocal structure    */
  unsigned long size;                       /* size of the saved memory       */
} ENVENTRY;

int putflag = 0;                            /* static or dynamic env memory   */

/*********************************************************************/
/*                                                                   */
/*   Subroutine Name:   sysBeep                                      */
/*                                                                   */
/*   Descriptive Name:  BEEP function                                */
/*                                                                   */
/*   Function:          sounds the speaker if possible, else flash   */
/*                      the screen                                   */
/*********************************************************************/

RexxMethod2(RexxStringObject, sysBeep, RexxNumber, Frequency, RexxNumber, Duration)
{
                                        /* console beep for Unix     */
  printf("\a");
  return context->NullString();         /* always returns a null      */
}


/********************************************************************
* Function:  resolve_tilde(path)                                    *
*                                                                   *
* Purpose:   Resolves path names including '~'.                     *
*                                                                   *
* RC:        Returns the absolute path in new allocated space.      *
*                                                                   *
*********************************************************************/
char * resolve_tilde(char * path)
{
    char * st;
    char *  home_dir = NULL;            /* home directory path        */
    char *  dir_buf = NULL;             /* full directory path        */
    char *  slash;
    struct passwd *ppwd;
    int alloc_flag = 0;

    st = path;
                                       /* if no user name            */
    if(*(st) == '~' && (*(st+1) == '\0' || *(st+1) == '/'|| *(st+1) == ' ' )){
      if(*(st+1) == '/'){              /* if there is a path         */
        st +=2;                        /* jump over '~/'             */
                                       /* get home directory path    */
        home_dir = getenv("HOME");     /* from the environment       */
        if(!home_dir)                  /* if no home dir info        */
          return (0);
                                       /* get space for the buf      */
        dir_buf = (char *)malloc(strlen(home_dir)+strlen(st)+2);
        if(!dir_buf)
          return (0);
                               /* merge the strings          */
        sprintf(dir_buf, "%s/%s", home_dir, st);
        st = dir_buf;                  /* directory change to        */
        alloc_flag = 1;
      }
      else{
                                       /* get home directory path    */
        home_dir = getenv("HOME");     /* from the environment       */
                                       /* get space for the buf      */
        dir_buf = (char *)malloc(strlen(home_dir)+2);
        if(!dir_buf)
          return (0);
        sprintf(dir_buf, "%s/", home_dir);
        st = dir_buf;                  /* directory change to        */
        alloc_flag = 1;
      }
    }
    else if(*(st) == '~'){             /* cmd is '~username...'      */
      st++;                            /* jump over '~'              */
      slash = strchr(st,'/');          /* search for '/'             */
      if(!slash){                      /* if no '/'                  */
                                       /* rest of string is username */
        ppwd = getpwnam(st);           /* get info about the user    */
                                       /* get space for the buf      */
    if(ppwd == NULL){                  /* no user                    */
      return 0;                        /* nothing happend            */
    }
        dir_buf = (char *)malloc(strlen(ppwd->pw_dir)+2);
        if(!dir_buf)
          return (0);
                                       /* merge the strings          */
        sprintf(dir_buf, "%s/", ppwd->pw_dir);
        alloc_flag = 1;
      }
      else{                            /* there is a slash           */
        *slash = '\0';                 /* teminate to get username   */
        ppwd = getpwnam(st);           /* get info about the user    */
        slash++;                       /* step over the slash        */
                                       /* get space for the buf      */
        dir_buf = (char *)malloc(strlen(ppwd->pw_dir)+strlen(slash)+2);
        if(!dir_buf)
          return (0);
                                       /* merge the strings          */
        sprintf(dir_buf, "%s/%s", ppwd->pw_dir, slash);
        alloc_flag = 1;
      }
      st = dir_buf;                    /* directory change to        */
    }
    if(!alloc_flag)
      return 0;                        /* nothing happend            */
    return st;
}

/****************************************************************************/
/* sysDirectory                                                             */
/****************************************************************************/
RexxMethod1(RexxStringObject, sysDirectory, CSTRING, dir)
{
//char buffer[CCHMAXPATH+2];
  int rc;
  char * rdir;                           /* resolved path */

  rc = 0;
  if (dir != NULL){              /* if new directory is not null,     */
    if(*dir == '~'){
      rdir = resolve_tilde(dir);
      rc = chdir(rdir);
      free(rdir);
    }
    else
      rc = chdir(dir);                   /* change to the new directory     */
  }

  if (!getcwd(achRexxCurDir, CCHMAXPATH) || (rc != 0)) /* Get current working direct */
  {
     strncpy( achRexxCurDir, getenv("PWD"), CCHMAXPATH);
     achRexxCurDir[CCHMAXPATH - 1] = '\0';
     if ((achRexxCurDir[0] != '/' ) || (rc != 0))
         return context->NullString();         /* no directory returned     */
  }
  return context->NewStringFromAsciiz(achRexxCurDir); /* Return the current directory*/
}


/*****************************************************************************/
/* sysFilespec                                                               */
/*****************************************************************************/
RexxMethod2 (RexxStringObject, sysFilespec, CSTRING, Option, CSTRING, Name)
{
  long       NameLength;               /* file name length                  */
  char *      ScanPtr;                  /* scanning pointer                  */
  char *      EndPtr;                   /* end of string                     */
  char *      PathPtr;                  /* path pointer                      */
  char *      PathEnd;                  /* path end pointer                  */

                                       /* required arguments missing?       */
  if (Option == NULL || strlen(Option) == 0 || Name == NULL)
                                       /* raise an error                    */
  context->RaiseException(Error_Incorrect_call);

  NameLength = strlen(Name);           /* get filename length               */

  EndPtr = Name + (NameLength - 1);    /* point to last character           */
  PathEnd = strrchr(Name, '/');        /* find the last slash in Name       */

  switch (toupper(*Option)) {            /* process each option               */
      case FILESPEC_PATH:                /* extract the path                  */
      {
         if (PathEnd)                    /* if there is a path spec. , return */
                                         /* up to and including last slash.   */
                                         /* else return OREF_NULLSTRING       */
           return context->NewString(Name, PathEnd - Name + 1);
      }
      break;                           /* finished                            */

      case FILESPEC_NAME:              /* extract the file name               */
      {                                /* everything to right of slash        */
         if ((PathEnd) && (PathEnd != EndPtr))
            return context->NewString(PathEnd + 1, EndPtr - PathEnd);

         if (!PathEnd)                 /* there was no path spec.             */
            return context->NewStringFromAsciiz(Name);
      }
      break;                           /* finished                          */

    default:                           /* unknown option                    */
                                       /* raise an error                    */
        context->RaiseException(Error_Incorrect_call);
  }
  return context->NullString();         /* returns a null on an error       */
}


/*********************************************************************/
/*                                                                   */
/*   Subroutine Name:   SysRestoreEnvironment                        */
/*                                                                   */
/*   Descriptive Name:  restores environment saved by Setlocal()     */
/*                                                                   */
/*   Function:          restores the environment variables, current  */
/*                      directory and drive.                         */
/*                                                                   */
/*********************************************************************/

void SysInterpreter::restoreEnvironment(
  void *CurrentEnv)                    /* saved environment          */
{
  char *current;                       /* ptr to saved environment   */
  unsigned long size;                  /* size of the saved space    */
  unsigned long length;                /* string length              */
  char *begin;                         /* begin of saved space       */
  char **Environment;                  /* environment pointer        */

  char *  del = NULL;                   /* ptr to old unused memory   */
  char * Env_Var_String;                /* enviornment entry          */
  char   namebufsave[256],namebufcurr[256];
  char *  np;
  int i;

  Environment = environ;               /* get the current environment*/

  begin = current = (char *)CurrentEnv; /* get the saved space        */
  size = ((ENVENTRY*)current)->size;   /* first read out the size    */
  current += 4;                        /* update the pointer         */
  if(chdir(current) == -1)             /* restore the curr dir       */
      reportException(Error_System_service_service, (const stringchar_t *)"ERROR CHANGING DIRECTORY");
  current += strlen(current);          /* update the pointer         */
  current++;                           /* jump over '\0'             */
  if(!putflag){                        /* first change in the        */
                                       /* environment ?              */
    /* copy all entries to dynamic memory                            */
                                       /*for all entries in the env  */
    for(;*Environment != NULL;Environment++){
      length = strlen(*Environment)+1; /* get the size of the string */
                                       /* and alloc space for it     */
      Env_Var_String = (char *)malloc(length);
      memcpy(Env_Var_String,*Environment,length);/* copy the string  */
      putenv(Env_Var_String);          /* and make it part of env    */
    }
    putflag = 1;                       /* prevent do it again        */
  }
                                       /* Loop through the saved env */
                                       /* entries and restore them   */
  for(;((unsigned long)(current-begin))<size;current+=(strlen(current)+1)){
    Environment = environ;             /* get the environment        */
    del = NULL;
    np = current;
                                       /* extract the the name       */
                                       /* from the saved enviroment  */
    for(i=0;(*np!='=')&&(i<255);np++,i++){
      memcpy(&(namebufsave[i]),np,1);  /* copy the character         */
    }
    memcpy(&(namebufsave[i]),"\0",1);  /* copy the terminator        */
                                       /* find the entry in the env  */
    for(;*Environment != NULL;Environment++){
      np = *Environment;
                                       /* extract the the name       */
                                       /* from the current env       */
      for(i=0;(*np!='=')&&(i<255);np++,i++){
        memcpy(&(namebufcurr[i]),np,1);/* copy the character         */
      }
      memcpy(&(namebufcurr[i]),"\0",1);/* copy the terminator        */

      if(!strcmp(namebufsave,namebufcurr)){/* have a match ?         */
        del = *Environment;            /* remember it for deletion   */
        break;                         /* found, so get out of here  */
      }
    }
    if(putenv(current) == -1)
      reportException(Error_System_service_service, (const stringchar_t *)"ERROR RESTORING ENVIRONMENT VARIABLE");
    if(del)                            /* if there was an old entry  */
      free(del);                       /* free it                    */
  }
}


/*********************************************************************/
/*                                                                   */
/*   Subroutine Name:   BuildEnvlist                                 */
/*                                                                   */
/*   Descriptive Name:  Build saved environment block                */
/*                                                                   */
/*   Function:          Builds a block containing all of the         */
/*                      environment variables, the current drive     */
/*                      and the current directory.                   */
/*                                                                   */
/*********************************************************************/
static RexxObject * BuildEnvlist()
{
  RexxObject    *newBuffer;            /* Buffer object to hold env  */
  char          **Environment;         /* environment pointer        */
  size_t        size = 0;              /* size of the new buffer     */
  char          *curr_dir;             /* current directory          */
  char          *New;                  /* starting address of buffer */

  Environment = environ;               /* get the ptr to the environ */

  for(;*Environment != NULL;Environment++){
    size += strlen(*Environment);      /* calculate the size for all */
    size++;                            /* environment variables+'\0' */
  }                                    /* now get current dir        */
  if(!size)
    return NULLOBJECT;                 /* this worked ok                    */
  if (!(curr_dir=(char *)malloc(CCHMAXPATH+2)))/* malloc storage for cwd*/
    reportException(Error_System_service);

  if (!getcwd(curr_dir,CCHMAXPATH))    /* get current directory      */
  {
     strncpy( achRexxCurDir, getenv("PWD"), CCHMAXPATH);
     achRexxCurDir[CCHMAXPATH - 1] = '\0';
     if (achRexxCurDir[0] != '/' )
       reportException(Error_System_service);/* Complain if it fails*/
  }
  size += strlen(curr_dir);            /* add the space for curr dir */
  size++;                              /* and its terminating '\0'   */
  size += 4;                           /* this is for the size itself*/
                                       /* Now we have the size for   */
                                       /* allocating the new buffer  */
  newBuffer = (RexxObject *)new_buffer(NULL, size);  /* let's do it     */
                                       /* Get starting address of buf*/
  New = (char*)newBuffer;
  ((ENVENTRY*)New)->size = size;       /* first write the size       */
  New +=4;                             /* update the pointer         */
                                       /* now write the curr dir     */
  memcpy(New,curr_dir,strlen(curr_dir));
  New += strlen(curr_dir);             /* update the pointer         */
  memcpy(New,"\0",1);                  /* write the terminator       */
  New++;                               /* update the pointer         */
  Environment = environ;               /* reset to begin of environ  */
                                       /* Loop through environment   */
                                       /* and copy all entries to the*/
                                       /* buffer, each terminating   */
                                       /* with '\0'                  */
  for(;*Environment != NULL;Environment++){
                                       /* copy the entry             */
    memcpy(New,*Environment,strlen(*Environment));
    New += strlen(*Environment);       /* update the pointer         */
    memcpy(New,"\0",1);                /* write the terminator       */
    New++;                             /* update the pointer         */
  }
  free(curr_dir);                      /* free curr dir buffer       */
  return newBuffer;                    /* return the pointer         */
}


/**
 * Push a new environment for the SysSetLocal() BIF.
 *
 * @param context The current activation context.
 *
 * @return Returns TRUE if the environment was successfully pushed.
 */
RexxObject *SysInterpreter::pushEnvironment(RexxActivation *context)
{
    RexxObject     *Current;             /* new save block                    */

    Current = BuildEnvlist();            /* build the new save block          */
    if (NULLOBJECT == Current)           /* if unsuccessful return zero       */
      return TheFalseObject;
    else {
                                         /* Have Native Actiovation           */
      context->pushEnvironment(Current);          /*  update environemnt list          */
      return TheTrueObject;              /* this returns one                  */
    }
}

/**
 * Pop an environment for the SysEndLocal() BIF.
 *
 * @param context The current activation context.
 *
 * @return Always returns FALSE.  This is a NOP on Windows.
 */
RexxObject *SysInterpreter::popEnvironment(RexxActivation *context)
{
        RexxObject    *Current;          /* new save block                    */

    Current =  context->popEnvironment();/*  block, if ixisted.               */
    if (TheNilObject == Current)         /* nothing saved?                    */
      return TheFalseObject;             /* return failure value              */
    else {
                                         /* restore everything                */
      restoreEnvironment(Current);
      return TheTrueObject;              /* this worked ok                    */
    }
}

