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
/*   Function:  Miscellaneous system specific routines               */
/*                                                                   */
/*********************************************************************/
#include "RexxCore.h"
#include "StringClass.hpp"
#include "DirectoryClass.hpp"
#include "RexxActivity.hpp"
#include "RexxActivation.hpp"
#include <signal.h>
#include "ActivityTable.hpp"
#include "SysInterpreter.hpp"
#include "Interpreter.hpp"

bool SysInterpreter::processBreakEvent(int ctrlType)
/******************************************************************************/
/* Arguments:  Report record, registration record, context record,            */
/*             dispatcher context                                             */
/*                                                                            */
/* DESCRIPTION : For Control Break conditions issue a halt to activation      */
/*               Control-C or control-Break is pressed.                       */
/*                                                                            */
/*  Returned:  Action code                                                    */
/******************************************************************************/
{
  if (ctrlType == CTRL_C_EVENT)
  {
      // if we've already processed one of these, pass and let the system handle it.
      if (signaled)
      {
          return false;
      }
      signaled = true;
  }

  // tell all Rexx threads to hald.
  RexxInterpreter::processExternalHalt();
  return true;      /* ignore signal */
}

RexxObject *SysInterpreter::getProcessName( void )
/******************************************************************************/
/* Function:  Get a referenceable name for this process                       */
/******************************************************************************/
{
  DWORD pname;
  pname = GetCurrentProcessId();
  return new_integer(pname);
}


RexxString *SysInterpreter::getSystemName()
/******************************************************************************/
/* Function: Get System Name                                                  */
/******************************************************************************/
{
  if (SysProcess::running98())
  {
      return new_string("Windows98");
  }
  else
  {
      return new_string("WindowsNT");
  }
}

void SysInterpreter::termination()
/******************************************************************************/
/* Function:   Perform system specific termination.                           */
/******************************************************************************/
{
}


void SysInterpreter::initialize()
/******************************************************************************/
/* Function:   Perform system specific initialization.                        */
/******************************************************************************/
{
}

RexxString *SysInterpreter::getVersion(void)
/******************************************************************************/
/* Function:   Return the system specific version identifier that is stored   */
/*             in the image.                                                  */
/******************************************************************************/
{
  CHAR chVerBuf[8];                   // buffer for version
  OSVERSIONINFO vi;
  // dont forget to change sysmeths.cmd

  vi.dwOSVersionInfoSize = sizeof(vi);  // if not set --> violation error

  GetVersionEx(&vi);              // get version with extended api
                                       /* format into the buffer            */
  wsprintf(chVerBuf,"%i.%02i",(int)vi.dwMajorVersion,(int)vi.dwMinorVersion);
  return new_string(chVerBuf);     /* return as a string                */
}

/*********************************************************************/
/*                                                                   */
/*   Subroutine Name:   SysRelinquish                                */
/*                                                                   */
/*   Function:          Performs peekmsg to yield to other processes */
/*                                                                   */
/*********************************************************************/

void SysInterpreter::relinquish()
{
    SysProcess::relinquish();
}

void SysInterpreter::programSetup(
  RexxActivation *activation)          /* current running program           */
/******************************************************************************/
/* Function:  Do system specific program setup                                */
/******************************************************************************/
{
    TCHAR RxTraceBuf[8];

    /* scan current environment,         */
    if (GetEnvironmentVariable("RXTRACE", RxTraceBuf, 8))
    {
        if (!stricmp(RxTraceBuf, "ON"))    /* request to turn on?               */
                                           /* turn on tracing                   */
            activation->setTrace(TRACE_RESULTS, DEBUG_ON);
    }
}









