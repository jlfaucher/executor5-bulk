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
/****************************************************************************/
/* REXX Kernel                                                  okutil.c    */
/*                                                                          */
/* Utility Functions                                                        */
/*                                                                          */
/****************************************************************************/

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "RexxCore.h"
#include "StringClass.hpp"
#include "RexxActivity.hpp"
#include "ArrayClass.hpp"
#include "RexxNativeActivation.hpp"
#include "Interpreter.hpp"

void RexxInterpreter::fatalError (const stringchar_t *desc)
/******************************************************************************/
/* Function:  Raise a fatal logic error                                       */
/******************************************************************************/
{
  printf("Logic error: %s\n", (char *)desc);
  exit(RC_LOGIC_ERROR);
}


void missingArgument(
    size_t  argumentPosition)          /* position of the missing argument  */
/******************************************************************************/
/* Function:  Raise an error for a missing argument, given the target         */
/*            position.                                                       */
/******************************************************************************/
{
                                       /* just raise the error              */
  reportException(Error_Incorrect_method_noarg, (wholenumber_t)argumentPosition);
}

int caselessCompare(                   /* do a caseless memory comparison   */
     const char   *string1,            /* first compare string              */
     const char   *string2,            /* second compare string             */
     size_t        length)             /* comparison length                 */
/******************************************************************************/
/* Function:  Compare two strings, ignoring differences in case               */
/******************************************************************************/
{

    // look for the first character not equal with the folding
    for (size_t index = 0; index < length; index++)
    {
        int result = (((int)toupper(string1[index])) & 0xff) - (((int)toupper(string2[index])) & 0xff);
        if (result != 0)
        {
            return result;
        }
    }
    // these are equal if we fall through
    return 0;
}

char * mempbrk(
  const char *string,           /* string                     */
  const char *set,              /* reference set                     */
  size_t      length )          /* size of string                    */
/*********************************************************************/
/*  Function:  Find first occurence of set member in memory          */
/*********************************************************************/
{
  while (length-- > 0) {               /* search through string             */

    if (strchr(set, *string)) {        /* find a match in ref set?          */
      return (char *)string;           /* return current position           */
    }
    string++;                          /* step the pointer                  */
  }
  return NULL;                         /* return matched position           */
}
