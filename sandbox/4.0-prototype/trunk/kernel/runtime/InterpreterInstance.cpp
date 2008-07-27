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
/* Implementation of the InterpreterInstance class                            */
/*                                                                            */
/******************************************************************************/

#include "InterpreterInstance.hpp"
#include "Interpreter.hpp"
#include "SysInterpreter.hpp"

/**
 * Create a new Package object instance.
 *
 * @param size   Size of the object.
 *
 * @return Pointer to new object storage.
 */
void *InterpreterInstanceObject::operator new(size_t size)
{
    return new_object(size, TheInterpreterInstanceBehaviour);
}

void InterpreterInstanceObject::live()
/******************************************************************************/
/* Function:  Normal garbage collection live marking                          */
/******************************************************************************/
{
  setUpMemoryMark

  memory_mark(interpreter);
  memory_mark(localEnvironment);
  memory_mark(activeActivities);
  memory_mark(primeActivity);
  memory_mark(globalReferences);
  memory_mark(defaultAddress);

  cleanUpMemoryMark
}
void InterpreterInstanceObject::liveGeneral()
/******************************************************************************/
/* Function:  Generalized object marking                                      */
/******************************************************************************/
{
  setUpMemoryMarkGeneral

  memory_mark_general(interpreter);
  memory_mark_general(localEnvironment);
  memory_mark_general(activeActivities);
  memory_mark_general(primeActivity);
  memory_mark_general(globalReferences);
  memory_mark_general(defaultAddress);

  cleanUpMemoryMarkGeneral
}

/**
 * Initialize an interpreter instance.
 *
 * @param handlers The exit handlers
 * @param ud       User defined data pointer.
 */
InterpreterInstance::InterpreterInstance(ExitHandler *handlers)
{
    // if we have handlers, initialize the array
    if (handlers != NULL)
    {
        for (int i = 0; i <= RXNOOFEXITS + 1; i++)
        {
            exits[i] = handlers[i];
        }
    }
    // set up the interface vector
    interfaceContext.functions = &interfaceVector;
    interfaceContext.applicationData = NULL;
}

/**
 * Process interpreter instance options.
 *
 * @param options The list of defined options.
 *
 * @return True if the options were processed correctly, false otherwise.
 */
bool InterpreterInstance::processOptions(RexxOption *options)
{
    // options are, well, optional...if nothing specified, we're done.
    if (options == NULL)
    {
        return true;
    }
    // loop until we get to the last option item
    while (options->optionName != NULL)
    {
        // an initial address environment
        if (strcmp(options->optionName, INITIAL_ADDRESS_ENVIRONMENT) == 0)
        {
            instanceObject->defaultAddress = new_string(options->extra.value.value_CSTRING);
        }
        // application data
        else if (strcmp(options->optionName, APPLICATION_DATA) == 0)
        {
            interfaceContext.applicationData = options->extra.value.value_POINTER;
        }
        // unknown option, this is a failure to communicate.
        else
        {
            return false;
        }
        // step to the next option value
        options++;
    }
    return true;
}

/**
 * Make sure this instance is attached to the current process.
 */
void InterpreterInstance::attachToProcess()
{
    // is this the first thread attached to this object?
    if (instanceObject == OREF_NULL)
    {
        // ask the interpreter to create an instance object for us and attach it
        instanceObject = RexxInterpreter::attachInstance(this);
    }
}

/**
 * Attach a thread to an interpreter instance.
 *
 * @param tc     T returned thread context.
 *
 * @return True if the context was created, false for any failures.
 */
bool InterpreterInstance::attachThread(RexxThreadContext **tc)
{
    RexxActivity *activity = enterOnCurrentThread();
    *tc = activity->getThreadContext();
    activity->deactivate();
    return true;
}

/**
 * Enter on the current thread context, making sure the interpreter
 * lock is obtained.
 *
 * @return The activity object associated with this thread/instance
 *         combination.
 */
RexxActivity *InterpreterInstance::enterOnCurrentThread()
{
    ResourceSection lock;              // lock the outer control block access

    attachToProcess();                 // make sure the process knows about us

    RexxThread threadID;               // our thread information
    threadID.attachThread();           // initialize with thread info


    // see if this is a reentry on this thread for this instance.
    RexxActivity *activity = findActivity(threadID);

    // first entry on this thread?  Create an activity context for it.
    if (activity == OREF_NULL)
    {
        // we need to associate an activity with this instance/thread combo
        activity = TheActivityManager->attachActivity(this);
        // add this to our active activities list
        addActivity(activity);
    }
    // this will also get us the kernel lock, and take care of nesting
    activity->activate();
    // return the activity in case the caller needs it.
    return activity;
}

void InterpreterInstance::exitCurrentThread()
{
    ResourceSection lock;

    // we're the currently executing activity, so grab that.
    RexxActivity *activity = RexxInterpreter::currentActivity;

    // have we completely unwound with this activity?
    if (activity->deactivateActivity())
    {
        // if we're in a shutdown state, then we don't really quiesce these threads.
        if (terminating)
        {
            // remove this from our current list....this will also trigger the final
            // termination if this is the last one.
            removeActivity(activity);
        }
    }

    // have the activity release the lock
    activity->releaseAccess();
}

/**
 * Process deactivation of an activity.
 *
 * @param activity The terminating activity.
 */
void InterpreterInstance::activityDeactivated(RexxActivity *activity)
{
    // if we're in a shutdown state, then we don't really quiesce these threads.
    if (terminating)
    {
        // remove this from our current list....this will also trigger the final
        // termination if this is the last one.
        removeActivity(activity);
    }
}

// get the local environment object associated with the instance.
// if this is the first request for this environment, one is created.
// NOTE:  The calling activity must own the kernel lock when this is called.
RexxDirectory *InterpreterInstance::getLocalEnvironment()
{
    return instanceObject->localEnvironment;
}

void InterpreterInstance::addActivity(RexxActivity *activity)
{
    ResourceSection lock;

    // add to the active activity table
    instanceObject->activeActivities->put((RexxObject *)activity, (RexxObject *)activity);

    // if this is the first activity added, make it the prime one.
    if (instanceObject->primeActivity == OREF_NULL)
    {
        instanceObject->primeActivity = activity;
    }
    // add this to the global mapping table.
    TheActivityManager->addProcessActivity(activity);
}

void InterpreterInstance::removeActivity(RexxActivity *activity)
{
    ResourceSection lock;

    // remove this from the list
    instanceObject->activeActivities->remove((RexxObject *)activity);
    // remove this from the global mapping table.
    TheActivityManager->removeProcessActivity(activity);

    if (instanceObject->activeActivities->items() == 0)
    {
        // no more activities, we're finished
        terminated = true;
        // if we're in termination wait, signal it's done
        if (terminating)
        {
            // trip the event semaphore on the prime activity to signal we're finished
            // with thread cleanup activities
            instanceObject->primeActivity->signalInstanceTermination();
        }
    }
}

RexxActivity *InterpreterInstance::findActivity(RexxThread &threadId)
{
    ResourceSection lock;
    HashLink index;
    RexxActivity *activity;


    RexxObjectTable * table = instanceObject->activeActivities;

    for (index = table->first(); table->index(index) != OREF_NULL; index = table->next(index))
    {
        activity = (RexxActivity *)table->value(index);
        if (activity->isSameThread(threadId))
        {
            return activity;
        }
    }

    return OREF_NULL;
}

bool InterpreterInstance::terminate()
{
    RexxThread threadID;              // our thread information
    threadID.attachThread();          // initialize with thread info
    {
        ResourceSection lock;
        // termination can only be done by the creating thread, and only
        // if the prime activity is quiesced.

        if (!instanceObject->primeActivity->isSameThread(threadID) || !instanceObject->primeActivity->isQuiesced())
        {
            return false;
        }

        // remove the prime activity from the active list first
        removeActivity(instanceObject->primeActivity);

        if (terminated)
        {
            return true;
        }

        terminating = true;              // we're now terminating
        // prepare to exit on this activity
        instanceObject->primeActivity->startTerminationProcess();
    }

    // now loop, waiting for all of the threads to complete
    for (;;)
    {
        {
            ResourceSection lock;     // get the global resource lock
            // if we have no active activities, then we're finished
            if (terminated)
            {
                return true;          // we're done!
            }
        }
        // wait for any instance to finish up so we can check to see if we're
        // finished.
        instanceObject->primeActivity->waitForInstanceTermination();
    }
}

InterpreterInstanceObject::InterpreterInstanceObject(InterpreterInstance *i, RexxInterpreter *rexx)
{
    // save link back to the parent instance structure
    instance = i;
    interpreter = rexx;
    // create instance-specific objects
    localEnvironment = new_directory();
    activeActivities = new_object_table();
    primeActivity = OREF_NULL;
    defaultAddress = SysInterpreter::getDefaultCommandEnvironment();
}

void InterpreterInstance::copyExits(ExitHandler *target)
{
    for (int i = 0; i < RXNOOFEXITS + 1; i++)
    {
        target[i] = exits[i];
    }
}

void InterpreterInstance::activityTerminated(RexxActivity *activity)
{
    // remove the activity from the active list
    removeActivity(activity);
}


/**
 * Raise a halt condition in all threads associated with this
 * interpreter instance.
 *
 * @return
 */
void InterpreterInstance::halt()
{
    ResourceSection lock;              // lock activity changes

    HashLink index;
    RexxObjectTable * table = instanceObject->activeActivities;

    for (index = table->first(); table->index(index) != OREF_NULL; index = table->next(index))
    {
        RexxActivity *activity = (RexxActivity *)table->value(index);
        activity->halt(OREF_NULL);
    }
}


void InterpreterInstance::setTrace(
    bool  on_or_off )                 /* trace on/off flag                 */
/******************************************************************************/
/* Function:   Flip on a bit in a target activities top activation            */
/******************************************************************************/
{
    ResourceSection lock;              // lock activity changes
    HashLink index;

    RexxObjectTable * table = instanceObject->activeActivities;

    for (index = table->first(); table->index(index) != OREF_NULL; index = table->next(index))
    {
        RexxActivity *activity = (RexxActivity *)table->value(index);
        activity->setTrace(on_or_off);
    }
}

void InterpreterInstance::processExternalHalt()
/******************************************************************************/
/* Function:   Process an asynchronous halt event.                            */
/******************************************************************************/
{
    HashLink index;

    RexxObjectTable * table = instanceObject->activeActivities;

    for (index = table->first(); table->index(index) != OREF_NULL; index = table->next(index))
    {
        RexxActivity *activity = (RexxActivity *)table->value(index);
        activity->halt(OREF_NULL);
        activity->systemInfo.halt();
    }
}


/**
 * Add an object to the global references table.
 *
 * @param o      The added object.
 */
void InterpreterInstance::addGlobalReference(RexxObject *o)
{
    if (o != OREF_NULL)
    {
        instanceObject->globalReferences->put(o, o);
    }
}

/**
 * Remove the global reference protection from an object.
 *
 * @param o      The protected object.
 */
void InterpreterInstance::removeGlobalReference(RexxObject *o)
{
    if (o != OREF_NULL)
    {
        instanceObject->globalReferences->remove(o);
    }
}
