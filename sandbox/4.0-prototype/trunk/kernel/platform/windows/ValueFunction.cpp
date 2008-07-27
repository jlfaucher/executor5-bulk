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
/*********************************************************************/
/*                                                                   */
/*   Subroutine Name:   SysValue                                     */
/*                                                                   */
/*   Function:          process the VALUE function selector          */
/*                      function                                     */
/*                                                                   */
/*********************************************************************/

#include <windows.h>
#include <stdlib.h>

#include "RexxCore.h"
#include "StringClass.hpp"
#include "SysInterpreter.hpp"

static const stringchar_t *SELECTOR = (stringchar_t *)"ENVIRONMENT";    /* environment selector               */
static const stringchar_t *AXENGINE1 = (stringchar_t *)"WSHPROPERTY";   /* scripting engine selector          */
static const stringchar_t *AXENGINE2 = (stringchar_t *)"WSHTYPELIB";    /* scripting engine selector          */
static const stringchar_t *AXENGINE3 = (stringchar_t *)"WSHENGINE";     /* scripting engine selector          */

/*********************************************************************/
/*                                                                   */
/*   Subroutine Name:   SysValue                                     */
/*                                                                   */
/*   Descriptive Name:  set value of environment variable            */
/*                                                                   */
/*********************************************************************/

RexxObject * SysInterpreter::valueFunction(
    RexxString * name,                 /* variable name                     */
    RexxObject * newValue,             /* new assigned value                */
    RexxString * selector )            /* variable selector                 */
{
  /* GetEnvironmentVariable will not alloc memory for OldValue ! */
  char        *oldValue = NULL;        /* old environment value             */
  RexxObject * retval;                 /* returned old name                 */
  DWORD        dwSize;                 /* size of env. variable             */

  selector = selector->upper();        /* upper case the selector           */
  name = name->upper();                /* and the name too                  */

  if (selector->strCompare(SELECTOR)) {/* selector ENVIRONMENT?             */

    /* get the size of the environment variable and allocate buffer         */
    dwSize = GetEnvironmentVariable((LPCTSTR)name->getStringData(), NULL, 0);
    if (dwSize)
    {
      oldValue = (char *) SysInterpreter::allocateResultMemory(dwSize);
                                         /* scan for the variable           */
      if (oldValue && GetEnvironmentVariable((LPCTSTR)name->getStringData(),oldValue,dwSize) )
      {
                                         /* have a value already?           */
        retval = (RexxObject*) new_string(oldValue);
        SysInterpreter::releaseResultMemory(oldValue);
      }
      else
        retval = OREF_NULLSTRING;        /* otherwise, return null            */
    }
    else
      retval = OREF_NULLSTRING;

    if (newValue != OREF_NULL)           /* have a new value?                 */
    {
        // if this is the nil object, then just remove the variable
        if ((RexxObject *)newValue == TheNilObject)
        {
            SetEnvironmentVariable((LPCTSTR)name->getStringData(), NULL);
        }
        else
        {
            SetEnvironmentVariable((LPCTSTR)name->getStringData(),
                (LPCTSTR)REQUIRED_STRING(newValue, ARG_TWO)->getStringData());
        }
    }
  }

  // see if there's an exit that want's to handle this
  else if (contextActivity()->callValueExit(selector, name, &retval, newValue))
  {
      // the exit passed, this is an error
      reportException(Error_Incorrect_call_selector, selector);
  }
  return retval;                       /* return old value                  */
}

