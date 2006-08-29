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
/*  winextf.c - Methods to resolve external function calls.                   */
/*                                                                            */
/*  C methods:                                                                */
/*    sysBeep     - Method for the BEEP BIF                                   */
/*    sysDirectory- Method for the DIRECTORY BIF                              */
/*    sysMessageBox - Method to pop up message box                            */
/*                                                                            */
/******************************************************************************/
#include "RexxCore.h"                         /* global REXX definitions        */
#include "StringClass.hpp"
#include "oorexx.h"                         /* Include REXX header            */
#include "SysInterpreter.hpp"
#include <direct.h>

#define  MAX_FREQUENCY 32767
#define  MIN_FREQUENCY    37
#define  MAX_DURATION  60000
#define  MIN_DURATION      0

                                            /* FILESPEC function options      */
#define FILESPEC_DRIVE        'D'
#define FILESPEC_PATH         'P'
#define FILESPEC_NAME         'N'

/*********************************************************************/
/*                                                                   */
/*   Subroutine Name:   sysBeep                                      */
/*                                                                   */
/*   Descriptive Name:  BEEP function                                */
/*                                                                   */
/*   Function:          sounds the speaker at frequency Hertz for    */
/*                      specified duration (in milliseconds)         */
/*********************************************************************/

RexxFunction2(RexxStringObject, sysBeep, RexxNumber, frequency, RexxUnsignedNumber, duration)
{
                                       /* out of range?              */
  if (frequency > MAX_FREQUENCY || frequency < MIN_FREQUENCY || duration > MAX_DURATION || duration < MIN_DURATION)
  {
      context->InvalidRoutine();
      return NULLOBJECT;
  }

  Beep((DWORD)frequency, (DWORD)duration);  /* sound beep                 */
  return context->NullString();        /* always returns a null      */
}

/********************************************************************************************/
/* sysDirectory                                                                             */
/********************************************************************************************/
RexxFunction1(RexxStringObject, sysDirectory, CSTRING, dir)
{
  char buffer[MAX_PATH + 1];  // retrofit by IH
  int rc = 0;

  if (dir != NULL)
  {
      if ((strlen(dir) == 2) && (dir[1] == ':'))
         rc = _chdrive(toupper( dir[0] ) - 'A' + 1);
      else
         rc = _chdir(dir);
  }
  if (rc != 0 || _getcwd(buffer, MAX_PATH) == NULL)
  {
      return context->NullString();
  }
  else
  {
      return context->NewStringFromAsciiz(buffer);
  }
}


/********************************************************************************************/
/* sysFilespec                                                                              */
/********************************************************************************************/
RexxFunction2(RexxStringObject, sysFilespec, CSTRING, option, CSTRING, name)
{
  size_t nameLength;                   /* file name length                  */
  char *scanPtr;                       /* scanning pointer                  */
  char *endPtr;                        /* end of string                     */
  char *pathPtr;                       /* path pointer                      */
  char *pathEnd;                       /* path end pointer                  */

  nameLength = strlen(name);           /* get filename length               */

  endPtr = name + nameLength;          /* point to last character           */

  switch (toupper(*option)) {          /* process each option               */

    case FILESPEC_DRIVE:               /* extract the drive                 */
      if (nameLength > 0) {            /* have a real string?               */
                                       /* scan for the character            */
        scanPtr = (char *)memchr(name, ':', nameLength);
        if (scanPtr != NULL)           /* found one?                        */
        {
            return context->NewString(name, scanPtr - name + 1);
        }
      }
      break;

    case FILESPEC_PATH:                /* extract the path                  */
      if (nameLength > 0) {            /* have a real string?               */
                                       /* find colon or backslash           */
        scanPtr = mempbrk(name, ":\\/", nameLength);
        if (scanPtr != NULL) {
          if (*scanPtr == ':') {       /* found a colon?                    */
            scanPtr++;                 /* step past the colon               */
            if (scanPtr < endPtr) {    /* not last character?               */
              pathEnd = NULL;          /* no end here                       */
                                       /* search for backslashes            */
              pathPtr = mempbrk(scanPtr, "\\/", endPtr - scanPtr);
              while (pathPtr != NULL) {  /* while more backslashes            */
                pathEnd = pathPtr;     /* save the position                 */
                                       /* search for more backslashes       */
                pathPtr++;             /* step past the last match          */
                pathPtr = mempbrk(pathPtr, "\\/", endPtr - pathPtr);
              }
              if (pathEnd != NULL)     /* have backslashes?                 */
              {
                  return context->NewString(scanPtr, pathEnd - scanPtr + 1);
              }
            }
          }
          else {
            pathPtr = scanPtr;         /* save start position               */
            pathEnd = pathPtr;         /* CHM - defect 85: save end pos.    */
            pathPtr++;                 /* step past first one               */
                                       /* search for backslashes            */
            pathPtr = mempbrk(pathPtr, "\\/", endPtr - pathPtr);
            while (pathPtr) {          /* while more backslashes            */
              pathEnd = pathPtr;       /* save the position                 */
              pathPtr++;               /* step past the last match          */
                                       /* search for more backslashes       */
              pathPtr = mempbrk(pathPtr, "\\/", endPtr - pathPtr);
            }
                                       /* extract the path                  */
            return context->NewString(name, pathEnd - name + 1);
          }
        }
      }
      break;                           /* finished                          */

    case FILESPEC_NAME:                /* extract the file name             */
      if (nameLength > 0) {            /* filename null string?             */
                                       /* find colon or backslash           */
        scanPtr = mempbrk(name, ":\\/", nameLength);
        if (scanPtr != NULL) {
          if (*scanPtr == ':') {       /* found a colon?                    */
            scanPtr++;                 /* step past the colon               */
            pathEnd = scanPtr;         /* save current position             */
            pathPtr = mempbrk(scanPtr, "\\/", endPtr - scanPtr);
            while (pathPtr) {          /* while more backslashes            */
              pathPtr++;               /* step past the last match          */
              pathEnd = pathPtr;       /* save the position                 */
                                       /* search for more backslashes       */
              pathPtr = mempbrk(pathPtr, "\\/", endPtr - pathPtr);
            }
            if (pathEnd < endPtr)      /* stuff to return?                  */
            {
                return context->NewString(pathEnd, endPtr - pathEnd);
            }
          }
          else {
            pathPtr = scanPtr + 1;     /* save start position               */
            pathEnd = pathPtr;         /* step past first one               */
                                       /* search for backslashes            */
            pathPtr = mempbrk(pathPtr, "\\/", endPtr - pathPtr);
            while (pathPtr != NULL) {  /* while more backslashes            */
              pathPtr++;               /* step past the last match          */
              pathEnd = pathPtr;       /* save the position                 */
                                       /* search for more backslashes       */
              pathPtr = mempbrk(pathPtr, "\\/", endPtr - pathPtr);
            }
            if (pathEnd < endPtr)      /* stuff to return?                  */
            {
                return context->NewString(pathEnd, endPtr - pathEnd);
            }
          }
        }
        else
        {
            // entire string is the result
            return context->NewStringFromAsciiz(name);
        }
      }
      break;                           /* finished                          */

    default:                           /* unknown option                    */
      context->InvalidRoutine();       // this is an error
      return NULLOBJECT;
  }
  return NULLOBJECT;                   /* return extracted part             */
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
/*                                                                   */
/*********************************************************************/
RexxFunction4(int, sysMessageBox, CSTRING, text, OPTIONAL_CSTRING, title, OPTIONAL_CSTRING, button, OPTIONAL_CSTRING, icon)
{
  ULONG       style;                   /* window style flags         */
  int         maxCnt;                  /* Max loop count             */
  int         index;                   /* table index                */

  PSZ    Button_Styles[] =             /* message box button styles  */
    {"OK",
     "OKCANCEL",
     "RETRYCANCEL",
     "ABORTRETRYIGNORE",
     "YESNO",
     "YESNOCANCEL"};

   ULONG  Button_Flags[] =                /* message box button styles  */
       {MB_OK,
        MB_OKCANCEL,
        MB_RETRYCANCEL,
        MB_ABORTRETRYIGNORE,
        MB_YESNO,
        MB_YESNOCANCEL};

   PSZ    Icon_Styles[] =                 /* message box icon styles    */
       {"HAND",
        "QUESTION",
        "EXCLAMATION",
        "ASTERISK",
        "INFORMATION",
        "WARNING",
        "ERROR",
        "QUERY",
        "NONE",
        "STOP"};

   ULONG  Icon_Flags[] =                  /* message box icon styles    */
       {MB_ICONHAND,
        MB_ICONQUESTION,
        MB_ICONEXCLAMATION,
        MB_ICONASTERISK,
        MB_ICONINFORMATION,
        MB_ICONWARNING,
        MB_ICONERROR,
        MB_ICONQUESTION,
        0,
        MB_ICONSTOP};

                                       /* set initial style flags    */
  style = MB_SETFOREGROUND;            /* make this foreground       */

  if (button == NULL)
  {
      style |= MB_OK;                  /* set default button style?  */
  }
  else {                               /* check various button styles*/
                                       /* get the number of styles   */
                                       /* search style table         */
    maxCnt = sizeof(Button_Styles) / sizeof(PSZ);

    for (index = 0; index < maxCnt; index++) {
                                       /* find a match?               */
      if (!stricmp(button, Button_Styles[index])) {
        style += Button_Flags[index];  /* add to the style            */
        break;
      }
    }
    if (index == maxCnt)               /* if not found raise error          */
    {
        context->InvalidRoutine();
        return 0;
    }
  }

  if (icon == NULL)
  {
      style |= MB_OK;    /* set default icon style?           */
  }
  else {                               /* check various button styles*/
    maxCnt = sizeof(Icon_Styles)/sizeof(PSZ);
                                       /* search style table                */
    for (index = 0; index < maxCnt; index++) {
                                       /* find a match?                     */
      if (!stricmp(icon,Icon_Styles[index])) {
        style |= Icon_Flags[index];    /* add to the style                  */
        break;
      }
    }
    if (index == maxCnt)               /* if not found raise error          */
    {
        context->InvalidRoutine();
        return 0;
    }
  }

  return MessageBox(NULL,                //hWndOwner
                  text,                // Text
                  title,               // Title
                  style);              // Styles
}


/**
 * Push a new environment for the SysSetLocal() BIF.
 *
 * @param context The current activation context.
 *
 * @return Always returns FALSE.  This is a NOP on Windows.
 */
RexxObject *SysInterpreter::pushEnvironment(RexxActivation *context)
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
RexxObject *SysInterpreter::popEnvironment(RexxActivation *context)
{
    return TheFalseObject;
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

void SysInterpreter::restoreEnvironment(
  void *currentEnv)                   /* saved environment          */
{
}
