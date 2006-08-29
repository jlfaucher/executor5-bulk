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
/* REXX API support                                                           */
/*                                                                            */
/* Portable APIs provided by the Rexx interpreter.                            */
/*                                                                            */
/******************************************************************************/

#include "RexxCore.h"                    /* bring in global defines           */
#include "oorexx.h"
#include "InternalAPI.hpp"
#include "RexxNativeActivation.hpp"
#include "RexxActivation.hpp"
#include "RexxActivity.hpp"
#include "Interpreter.hpp"
#include "ExitHandler.hpp"
#include "SysInterpreter.hpp"


/**
 * Retrieve the interpreter version information as a string.
 *
 * @return An ascii-z string version descriptor.  This caller must
 *         free the descriptor using ooRexxFreeMemory().
 */
RexxStringPointer RexxEntry ooRexxGetVersionInformation()
{
  #ifdef _DEBUG
    char vbuf1[] = "Open Object Rexx %s Version 3.0.0 - Internal Test Version\nBuild date: ";
  #else
      char vbuf1[] = "Open Object Rexx %s Version 3.0.0\nBuild date: ";
  #endif
    char vbuf2[] = "\nCopyright (c) IBM Corporation 1995, 2004.\nCopyright (c) RexxLA 2005.\nAll Rights Reserved.";
    char vbuf3[] = "\nThis program and the accompanying materials";
    char vbuf4[] = "\nare made available under the terms of the Common Public License v1.0";
    char vbuf5[] = "\nwhich accompanies this distribution.";
    char vbuf6[] = "\nhttp://www.ibm.com/developerworks/oss/CPLv1.0.htm";
    size_t s1 = strlen(vbuf1);
    size_t s2 = strlen(vbuf2);
    size_t s3 = strlen(vbuf3);
    size_t s4 = strlen(vbuf4);
    size_t s5 = strlen(vbuf5);
    size_t s6 = strlen(vbuf6);
    size_t sd = strlen(__DATE__);
    char * ptr = (char *)SysInterpreter::allocateResultMemory(s1+s2+s3+s4+s5+s6+sd+1);
    if (ptr != NULL)
    {
        sprintf(ptr, "%s%s%s%s%s%s%s", s1, sd, s2, s3, s4, s5, s6);
    }
    return ptr;
}

/**
 * Run a Rexx program as the result of an API call
 *
 * @param argCount   The count of arguments passed to this program.
 * @param arguments  The array of arguments.
 * @param filename   The target source filename.
 * @param instore    The instore execution information.
 * @param initialCmd The initial address environment.
 * @param callType   The program call type.
 * @param exits      Exits used to run this program.
 * @param retCode
 * @param retValue
 *
 * @return A the Rexx error return code.  Returns zero if everything
 *         worked ok.
 */
RexxReturnCode RexxInterpreter::startRexxProgram(wholenumber_t argCount, RxString *arguments,
    stringchar_t *filename, RxString *instore, stringchar_t *initialCmd, wholenumber_t callType,
    RexxExit *exits, RexxReturnCode *retCode, RxString *retValue)
{
  RxString *instoreSource = NULL;
  RxString *instoreImage = NULL;

  if (instore != NULL)
  {
      instoreSource = &instore[0];
      instoreImage  = &instore[1];
  }

  ExitHandler handlers[RXNOOFEXITS + 1];   // our set of exit handlers

  // do we have exits?  resolve these before creating the instance
  if (exits != NULL)
  {
      for (int i = 0; exits[i].sysexit_code != RXENDLST; i++)
      {
          // resolve the entry point and call style information
          handlers[exits[i].sysexit_code].resolve(exits[i].sysexit_name);
      }
  }

  // create an interpreter instance...all of our activities are relative to this.
  RexxInstance *instance;
  RexxThreadContext *context;
  // if unable to create an instance, return the failure in system service code.
  if (!RexxInterpreter::createInstance(&instance, &context, handlers, NULL))
  {
      return -48;
  }

  // get the activity, and grab the lock.
  RexxActivity *activity = contextToActivity(context);
  activity->activate();

  RexxReturnCode rc = 0;

  // call the program.
  try
  {
      RexxInterpreter::runRexxProgram(activity, filename, (size_t)argCount, arguments,
          callType, initialCmd, instoreSource, instoreImage, retValue, retCode);
  }
  catch (ActivityException)
  {
      // display the error information and get the termination return code.
      rc = activity->error();
  }

  // we're done on this thread
  activity->deactivate();

  // terminate the interpreter instance.  This will wait for any threads
  // that might have been spawned by this instance.
  instance->Terminate();
  return -rc;                          /* return the error code (negated)   */
}


/******************************************************************************/
/* Name:       RexxTranslateProgram                                           */
/*                                                                            */
/* Arguments:  inFile   - Input source program                                */
/*             outFile  - Output file name                                    */
/*                                                                            */
/******************************************************************************/
RexxReturnCode RexxEntry ooRexxCompileProgram(
   RexxStringPointer  inFile,          /* input source program              */
   RexxStringPointer  outFile)         /* output file name                  */
{
    // create an interpreter instance...all of our activities are relative to this.
    RexxInstance *instance;
    RexxThreadContext *context;
    // if unable to create an instance, return the failure in system service code.
    if (!RexxInterpreter::createInstance(&instance, &context, NULL, NULL))
    {
        return -48;
    }

    // get the activity, and grab the lock.
    RexxActivity *activity = contextToActivity(context);
    activity->activate();

    RexxReturnCode retCode = 0;
    // call the program.
    try
    {
                                         /* just do the translation step      */
        RexxMethod::translateSource((stringchar_t *)inFile, (stringchar_t *)outFile);
    }
    catch (ActivityException)
    {
        // this is a an unreadable error
        retCode = 3;
    }

    // we're done on this thread
    activity->deactivate();

    // terminate the interpreter instance.  This will wait for any threads
    // that might have been spawned by this instance.
    instance->Terminate();
    return retCode;                      /* return the error code             */
}


/******************************************************************************/
/* Name:       RexxTranslateSource                                            */
/*                                                                            */
/* Arguments:  inFile   - Input instore source                                */
/*             outFile  - Output rxstring                                     */
/*                                                                            */
/******************************************************************************/
RexxReturnCode RexxEntry ooRexxTranslateSource(
    RexxStringPointer name,             // name of the program
    RxString *inFile,                   /* input source program              */
    RxString *outFile)                  /* output file name                  */
{
    // create an interpreter instance...all of our activities are relative to this.
    RexxInstance *instance;
    RexxThreadContext *context;
    // if unable to create an instance, return the failure in system service code.
    if (!RexxInterpreter::createInstance(&instance, &context, NULL, NULL))
    {
        return -48;
    }

    // get the activity, and grab the lock.
    RexxActivity *activity = contextToActivity(context);
    activity->activate();

    RexxReturnCode retCode = 0;
    // call the program.
    try
    {
                                         /* just do the translation step      */
        RexxMethod::translateSource((stringchar_t*)name, inFile, outFile);
    }
    catch (ActivityException)
    {
        // this is a an unreadable error
        retCode = 3;
    }

    // we're done on this thread
    activity->deactivate();

    // terminate the interpreter instance.  This will wait for any threads
    // that might have been spawned by this instance.
    instance->Terminate();
    return retCode;                      /* return the error code             */
}

/**
 * Create an interpreter instance at the API level.
 *
 * @param instance The returned instance pointer.
 * @param context  The initial thread context for this instance.
 * @param exits    A list of user exits to be used for this context.
 * @param userData A pointer to a user defined data area.
 *
 * @return RXAPI_OK if the instance was created, RXAPI_MEMFAIL for any
 *         creation errors.
 */
RexxReturnCode RexxEntry ooRexxCreateInterpreter(RexxInstance **instance, RexxThreadContext **context, RexxExitCallback *exits, RexxOption *options)
{
    ExitHandler handlers[RXNOOFEXITS + 1];   // our set of exit handlers
    // do we have exits?  resolve these before creating the instance
    if (exits != NULL)
    {
        for (int i = 0; exits[i].exit_code != RXENDLST; i++)
        {
            // resolve the entry point and call style information
            handlers[exits[i].exit_code].setEntryPoint(exits[i].entry_point);
        }
    }
    return RexxInterpreter::createInstance(instance, context, handlers, options) ? RXAPI_OK : RXAPI_MEMFAIL;
}


/**
 * Internal API used to force an image save during the build
 * process.
 *
 * @return The error code, if any, if there is a build failure.
 */
RexxReturnCode RexxEntry ooRexxSaveImage()
{
    return RexxInterpreter::saveImage();
}
