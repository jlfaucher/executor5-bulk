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
/*****************************************************************************/
/* REXX Unix/Linux Support                                                   */
/*                                                                           */
/* System interpreter control.  This is the preferred location for all       */
/* system specific global variables, as well as specialized.  Processing     */
/* functions.  The interpreter does not instantiate an instance of this      */
/* class, so most variables and methods should be static.  Each platform     */
/* needs to provide at least a placeholder implementation of this class.     */
/*                                                                           */
/*****************************************************************************/

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "RexxCore.h"
#include "SysInterpreter.hpp"
#include "Interpreter.hpp"

// exception handler switch
bool SysInterpreter::handleExceptions = true;
bool SysInterpreter::signaled = false;
bool SysInterpreter::timerElapsed = false;
bool SysInterpreter::running = false;
int SysInterpreter::resourceBundle = 0;
int SysInterpreter::timerOwner = 0;
const char *SysInterpreter::metaVersion = "UNIX ";

/**
 * Do one-time process specific intialization of the environment.
 *
 * @param module The Library module handle for the interpreter.  Used to resolve
 *               text resources later on.
 */
void SysInterpreter::initializeProcessEnvironment(int module)
{
    resourceBundle = module;
    RexxInterpreter::initializeProcessEnvironment();
}


/**
 * Do process-termination processing to make sure process-specific
 * resources are cleaned up properly.
 */
void SysInterpreter::terminateProcessEnvironment()
{
    running = false;
    RexxInterpreter::terminateProcessEnvironment();
}


bool SysInterpreter::savingPrograms()
/******************************************************************************/
/* Function:  Check to see if program saving is enabled for this user         */
/******************************************************************************/
{
    char  *savetok;
    /* don't save tokens if environment variable isn't set */
    savetok = getenv("RXSAVETOKENS");
    return strcmp("YES",savetok) == 0;
}

/**
 * Do system specific environment shutdown.
 */
void SysInterpreter::shutdownEnvironment()
{
    stopTimeSlice();         // shutdown the timeslice thread
}


/**
 * Return the default command environment for this platform.
 *
 * @return The string version of the default command environment.
 */
RexxString *SysInterpreter::getDefaultCommandEnvironment()
{
    return new_string("bash");
}
