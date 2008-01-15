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
/* REXX Windows Support                                                      */
/*                                                                           */
/* Main interpreter control.  This is the preferred location for all         */
/* platform independent global variables.                                    */
/* The interpreter does not instantiate an instance of this                  */
/* class, so most variables and methods should be static.                    */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/

#include "Interpreter.hpp"
#include "ActivityManager.hpp"
#include "ListClass.hpp"


// global resource lock
SMTX Interpreter::resourceLock = 0;


RexxList *Interpreter::interpreterInstances = OREF_NULL;

void Interpreter::live(size_t liveMark)
{
    memory_mark(interpreterInstances);
}

void Interpreter::liveGeneral(int reason)
{
  if (!memoryObject.savingImage())
  {
      memory_mark_general(interpreterInstances);
  }
}

void Interpreter::processStartup()
{
    // the locks get create in order
    createLocks();
    ActivityManager::createLocks();
    RexxMemory::createLocks()
}

void Interpreter::processShutdown()
{
    // we destroy the locks in reverse order
    RexxMemory::closeLocks()
    ActivityManager::closeLocks();
    closeLocks();
}


/**
 * Perform interpreter startup processing.
 *
 * @param mode   The startup mode.  This indicates whether we're saving the
 *               image or in shutdown mode.
 */
void Interpreter::startInterpreter(InterpreterStartupMode mode)
{
    ResourceSection lock;

    // has everything been shutdown?
    if (!isActive())
    {
        // TODO:  Make sure
        setbuf(stdout, NULL);              // turn off buffering for the output streams
        setbuf(stderr, NULL);
        SystemInterpreter::startInterpreter();   // perform system specific initialization
        // initialize the memory manager , and restore the
        // memory image
        RexxMemory::initialize(mode == RUN_MODE);
        ActivityManager::startup();      // go create the local enviroment.
        // create our instances list
        interpreterInstances = new_list();
    }
    // we're live now
    active = true;
}


/**
 * Terminate the global interpreter environment, shutting down
 * all of the interpreter instances that we can and releasing
 * the object heap memory.
 *
 * @return true if everything was shutdown, false if there are reasons
 *         why this can't be shutdown.
 */
bool Interpreter::terminateInterpreter()
{
    {
        ResourceLock lock;      // lock in this section
        // if already shutdown, then we've got a quick return
        if (!isActive())
        {
            return true;
        }

        // we can only shutdown interpreter instances from the
        // threads that created them.  If we have active instances,
        // this is a "no can do" situation
        if (interpreterInstances->items() != 0)
        {
            return false;
        }
    }

    // we need to wait for the activity manager to tell us everything is
    // ready to go, but without holding the resource lock
    ActivityManager::waitForTermination();
    {
        // no initialized interpreter environment any more.
        active = false;
        ResourceLock lock;      // lock in this section
        SystemInterpreter::terminateInterpreter();

        // now shutdown the memory object
        memoryObject.shutdown();
        RexxList *interpreterInstances = OREF_NULL;
    }
    return true;
}


/**
 * This is a simple test of the active flag.
 *
 * @return true if the interpreter environment is active, false if it
 *         needs to be initializied before use.
 */
bool Interpreter::isActive()
{
    return active;
}


InterpreterInstance *Interpreter::createInterpreterInstance(PRXSYSEXIT exits, const char *defaultEnvironment)
{
    // make sure we initialize the global environment if it hasn't already been done
    startInterpreter();
    // get a new root activity for this instance.  This might result in pushing a prior level down the
    // stack
    RexxActivity *rootActivity = ActivityManager::getRootActivity();
    // ok, we have an active activity here, so now we can allocate a new instance and bootstrap everything.
    {
        InterpreterInstance *instance = new InterpreterInstance();
        ResourceSection lock;

        // add this to the active list
        interpreterInstances->append((RexxObject *)instance);

    }

    // now that this is protected from garbage collection, go and initialize everything
    instance->initialize(rootActivity, exits, defaultEnvironment);
    return instance;
}


bool Interpreter::terminateInterpreterInstance(InterpreterInstance *instance)
{
    // this might not be in a state where it can be terminated
    if (!instance->terminate())
    {
        return false;
    }

    ResourceLock lock;

    interpreterInstances->removeItem((RexxObject *)instance);
}



