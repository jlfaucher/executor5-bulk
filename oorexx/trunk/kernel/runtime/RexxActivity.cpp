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
/* REXX Kernel                                                  RexxActivity  */
/*                                                                            */
/* Primitive Activity Class                                                   */
/*                                                                            */
/* NOTE:  activities are an execution time only object.  They are never       */
/*        flattened or saved in the image, and hence will never be in old     */
/*        space.  Because of this, activities "cheat" and do not use          */
/*        OrefSet to assign values to get better performance.  Care must be   */
/*        used to maintain this situation.                                    */
/*                                                                            */
/******************************************************************************/
#include <stdlib.h>
#include <ctype.h>
#include "RexxCore.h"
#include "StringClass.hpp"
#include "StackClass.hpp"
#include "RexxActivity.hpp"
#include "RexxActivation.hpp"
#include "MessageClass.hpp"
#include "ArrayClass.hpp"
#include "TableClass.hpp"
#include "DirectoryClass.hpp"
#include "SourceFile.hpp"
#include "RexxVariableDictionary.hpp"
#include "RexxCode.hpp"
#include "RexxInstruction.hpp"
#include "ActivityTable.hpp"
#include "RexxMemory.hpp"
#include "RexxVariableDictionary.hpp"
#include "PointerClass.hpp"
#include "Interpreter.hpp"
#include "SysInterpreter.hpp"
#include "InterpreterInstance.hpp"
#include "SysProcess.hpp"
#include "SysSignalBlock.hpp"

#define ACT_STACK_SIZE 10
#define ACTIVATION_CACHE_SIZE 5

RexxActivityManager *RexxActivityManager::singleInstance = NULL;  // singleton instance
RexxActivity *RexxActivityManager::currentActivity = OREF_NULL;   // the currently active thread
ActivityTable RexxActivityManager::processActivities;  // the thread-to-activity mapping table

RexxActivity::RexxActivity(
    bool createThread,                 /* we're running on a separate thread */
    SysThread::ThreadPriority priority,/* activity priority                 */
    InterpreterInstance *instance)     /* process local directory           */
/******************************************************************************/
/* Function:  Initialize an activity object                                   */
/*  Returned:  Nothing                                                        */
/******************************************************************************/
{
    ClearObject(this);                 /* globally clear the object         */
    this->hashvalue = HASHOREF(this);  /* set the hash value                */

    state = NEW_ACTIVITY;              // this is still in an unused state

    this->frameStack.init();           /* initialize the frame stack        */
    this->runsem.create();             /* create the run and                */
    this->guardsem.create();           /* guard semaphores                  */
    this->stackcheck = true;           /* start with stack checking enabled */
                                       /* set up current usage set          */
    this->settings = &RexxInterpreter::defaultSettings;
    this->priority = priority;         /* and the priority                  */
    setRandomSeed(RexxInterpreter::generateRandomNumberSeed());

    // clear out the activation information
    this->currentStackFrame = OREF_NULL;
    this->currentApiContext = OREF_NULL;

    // attach this to the associated instance
    attachToInstance(priority, instance);

    if (createThread)                  /* need to create a thread?          */
    {
        // this activity is waiting for work
        state = WAITING_FOR_ACTIVATION;
        this->runsem.reset();          /* set the run semaphore             */
        _thread.createThread(this);    // create the new thread for this to run on
    }
    else
    {                                  // attaching the activity to the current thread
        // mark this as attached, but not active yet
        state = QUIESCED;

        _thread.attachThread(this);
        // set the thread priority appropriately
        _thread.setPriority(this->priority);
                                       /* establish the stack base pointer  */
        this->nestedInfo.stackptr = _thread.getStackBase();
    }

    // add a native activation as the base of the thread, which is also used
    // to create the initial thread context.
    RexxNativeActivation *root = new RexxNativeActivation(this);
    pushNativeActivation(root);

    // we hook the context object to point back to us for retrieval
    threadContext.threadContext.instance = instance->getInstanceContext();
    threadContext.threadContext.functions = &threadContextFunctions;
    threadContext.owningActivity = this;
}

void RexxActivity::createMethodContext(MethodContext *context, RexxNativeActivation *owner)
{
    // hood this up with the activity
    context->threadContext.threadContext = &threadContext.threadContext;
    context->threadContext.functions = &methodContextFunctions;
    context->context = owner;
}

/**
 * Push a new API context to create an error return marker.
 */
void RexxActivity::pushNewContext()
{
    RexxNativeActivation *new_activation = new RexxNativeActivation (this);
                                         /* push it on the activity stack     */
    this->pushNativeActivation(new_activation);
}

/**
 * Pop the current API context.
 */
void RexxActivity::popContext()
{
    popNativeActivation(currentApiContext);        // and pop this off the top
}

void RexxActivity::createCallContext(
    CallContext *context)
/******************************************************************************/
/*  Function:  Release the kernel access because code is going to "leave"     */
/*             the kernel.  This prepares for this by adding a native         */
/*             activation on to the stack to act as a server for the          */
/*             external call.  This way new native methods do not need to     */
/*             be created just to get an activation created                   */
/******************************************************************************/
{
                                       /* create a new native activation    */
  RexxNativeActivation *new_activation = new RexxNativeActivation (this);
                                       /* push it on the activity stack     */
  this->pushNativeActivation(new_activation);
  // now complete the filling in process
  createCallContext(context, new_activation);
  releaseAccess();                     /* now give up control               */
}

/**
 * Clear any exceptions pending in the current context.
 */
void RexxActivity::clearException()
{
    currentApiContext->clearException();
}


void RexxActivity::destroyCallContext(
    CallContext *context)
/******************************************************************************/
/*  Function:  Destroy a context related to a native function call.           */
/******************************************************************************/
{
    // first of all, get the kernel lock back
    requestAccess();
    // now retrieve the activation...
    RexxNativeActivation *owner = context->context;

    owner->checkConditions();          // see if any conditions have been raised
    popNativeActivation(owner);        // and pop this off the top
}


void RexxActivity::createCallContext(CallContext *context, RexxNativeActivation *owner)
{
    // hood this up with the activity
    context->threadContext.threadContext = &threadContext.threadContext;
    context->threadContext.functions = &callContextFunctions;
    context->context = owner;
}

void RexxActivity::createExitContext(ExitContext *context, RexxNativeActivation *owner)
{
    // hood this up with the activity
    context->threadContext.threadContext = &threadContext.threadContext;
    context->threadContext.functions = &exitContextFunctions;
    context->context = owner;
}

void RexxActivity::createExitContext(ExitContext *context)
{
                                       /* create a new native activation    */
    RexxNativeActivation *new_activation = new RexxNativeActivation(this);
                                       /* push it on the activity stack     */
    this->pushNativeActivation(new_activation);
    createExitContext(context, new_activation);
    releaseAccess();                     /* now give up control               */
}

void RexxActivity::destroyExitContext(
    ExitContext *context)
/******************************************************************************/
/*  Function:  Process an exit return.  This pops the activation off of the   */
/*             stack, and also reacquires exclusive access.  If the exit      */
/*             raised an exception or condition, we raise it in the calling   */
/*             context now.                                                   */
/******************************************************************************/
{
    // first of all, get the kernel lock back
    requestAccess();
    // now retrieve the activation...
    RexxNativeActivation *owner = context->context;

    owner->checkConditions();          // see if any conditions have been raised
    popNativeActivation(owner);        // and pop this off the top
}

ActivityContext *RexxActivity::getActivityContext(ActivityContext **context)
{
    releaseAccess();                     /* now give up control               */
    return &threadContext.threadContext;
}

void RexxActivity::releaseActivityContext(
    ActivityContext *context)
/******************************************************************************/
/*  Function:  Process an exit return.  This pops the activation off of the   */
/*             stack, and also reacquires exclusive access.  If the exit      */
/*             raised an exception or condition, we raise it in the calling   */
/*             context now.                                                   */
/******************************************************************************/
{
    // first of all, get the kernel lock back
    requestAccess();
}

void RexxActivity::signalInstanceTermination()
{
    // post the run sem to wake up the main thread.
    runsem.post();
}

void RexxActivity::startTerminationProcess()
{
    // reset the run semaphore
    runsem.reset();
}

void RexxActivity::waitForInstanceTermination()
{
    // wait until the semaphore is posted
    runsem.wait();
}

void RexxActivity::activate()
{
    // just change the state...the real work happens when we give it back
    state = ACTIVATED;
    // and get the kernel lock before returning
    requestAccess();
}

ActivityState RexxActivity::setActivityState(ActivityState newState)
{
    ActivityState oldState = state;
    state = newState;
    return oldState;
}

/**
 * Deactivate an activity on return from a request.
 */
void RexxActivity::deactivate()
{
    if (deactivateActivity())
    {
        interpreter->activityDeactivated(this);
    }
    releaseAccess();
}

bool RexxActivity::deactivateActivity()
{
    // if we're exiting an attached thread (such as the main thread) at the
    // top level, we just put this in a sleeping state.
    if (nestedCount == 1)
    {
        state = QUIESCED;
        // and this activity is done.
        return true;
    }
    else
    {
        // get the state when we reentered from the api context frame
        state = currentApiContext->getActivityState();
        // we're still active
        return false;
    }
}

void RexxActivity::attachToInstance(
    SysThread::ThreadPriority priority,/* activity priority                 */
    InterpreterInstance *instance)     /* process local directory           */
/******************************************************************************/
/* Function:  Attach an existing pooled activity to a new instance in this    */
/*  process.                                                                  */
/******************************************************************************/
{
  this->interpreter = instance;        // make sure this is attached to the correct instance
  this->local = instance->getLocalEnvironment();  // set the local environment
  // copy the exit set
  instance->copyExits(nestedInfo.exits);

  this->priority = priority;           /* just set the priority             */
}

/**
 * Do detach processing for a thread context.
 */
void RexxActivity::detach()
{
    interpreter->removeActivity(this);
    detachInstance();
}

/**
 * Remove all references to the owning instance.
 */
void RexxActivity::detachInstance()
{
    // remove all attachments to instance data
    this->interpreter = NULL;
    this->local = OREF_NULL;
}

void RexxActivity::terminatePoolActivity()
/******************************************************************************/
/* Function:  Tell a thread in the activity pool to terminate                 */
/*                                                                            */
/******************************************************************************/
{
    terminating = true;                  // set the condition flag
    runsem.post();                       // post the semaphore to wake up the pool semaphore
}

void RexxActivity::runThread()
{
    _thread.startup();                   /* system specific thread init       */
                                         /* establish the stack base pointer  */
    nestedInfo.stackptr = _thread.getStackBase();
    {
        while (!terminating)             // run until we're told to shut down
        {
            try
            {
                // once we are up and running, we wait on our run semaphore until
                // we're woken up.  On wake up, we either have work to do, or
                // we've been asked to terminate.
                runsem.wait();

                // if we've been told to terminate, do termination processing and
                // exit.
                if (terminating)
                {
                    // indicate we're terminating
                    state = TERMINATING_ACTIVITY;
                    terminateActivity(NORMAL_TERMINATION);
                    break;
                }

                // set our priority to the current priority level
                _thread.setPriority(this->priority);

                // request the kernel access from this point...this will be
                // released after we've done post-dispatch cleanup
                requestAccess();

                // ok, we're doing real work here
                state = RUNNING_ACTIVITY;
                // our top activation (whatever that happens to be) is the
                // run target, so go dispatch it.
                ProtectedObject result;
                if (currentStackFrame != OREF_NULL)
                {
                    // we're dispatching Rexx code
                    currentStackFrame->dispatch(result);
                }
                else
                {
                    // running some native stuff
                    currentApiContext->dispatch(result);
                }
            }
            catch (ActivityException)    // we've had a termination event, raise an error
            {
                this->error();
            }

            // see if there are uninit methods we can run now.  This thread is free
            // so this is a good time to do it.
            RexxInterpreter::runUninits();

            // reset all of our semaphores until we loop back around again looking for
            // work.
            runsem.reset();
            guardsem.reset();

            // we're in waiting state now
            state = WAITING_FOR_ACTIVATION;
            // return this to the thread pool so it's available for reuse.
            if (!RexxInterpreter::addToActivityPool(this))
            {
                state = TERMINATING_ACTIVITY;
                // but if we're no longer pooling, just terminate this
                // thread.
                terminating = true;
            }
            // release the kernel lock now....we're unprotected when we return to the
            // dispatch loop.
            releaseAccess();
        }
    }
    // go through the thread shutdown procedures
    shutdown();
}


void *RexxActivity::operator new(size_t size)
{
    return new_object(size, TheActivityBehaviour);
}

void RexxActivity::shutdown()
/******************************************************************************/
/* Function:  Perform final shutdown of an activity created as a new thread   */
/******************************************************************************/
{
    // this activity is terminating, but we might have some stuff to run
    // first, so grab the kernel lock one last time.
    requestAccess();

    // clean up any system resources used by this thread.
    runsem.close();
    guardsem.close();
    _thread.terminate();

    // now tell our instance we've ended...but before we release access
    // If we're in the process activity pool, we don't have an attached instance,
    // so we tap the interpreter instead.
    if (interpreter == OREF_NULL)
    {
        RexxInterpreter::activityTerminated(this);
    }
    else
    {
        interpreter->activityTerminated(this);
    }
    releaseAccess();                     /* and release the kernel lock       */
}



RexxObject *RexxActivity::getFromLocalEnvironment(RexxString *index)
{
       return local->at(index);
}

RexxDirectory *RexxActivity::getConditionInfo()
/******************************************************************************/
/* Function:  Get the current condition information, if any                   */
/******************************************************************************/
{
    // this is anchored on the api context
    return currentApiContext->getConditionInfo();
}

void RexxActivity::setConditionInfo(RexxDirectory *conditionObj)
/******************************************************************************/
/* Function:  Set the current condition information                           */
/******************************************************************************/
{
    // store this in the api context
    currentApiContext->setConditionInfo(conditionObj);
}


/**
 * Process a terminating error condition, including displaying
 * any messages.
 *
 * @return The error return code.
 */
wholenumber_t RexxActivity::error()
{
    // unwind any Rexx activations until we get back to the
    // native activation that is the base of the frame stack
    while (currentStackFrame != OREF_NULL)
    {
        // tell the activation to terminate
        currentStackFrame->termination();
        // get the previous frame in the activation stack
        currentStackFrame = currentStackFrame->getPreviousStackFrame();
    }
    resetSettings();                     // reset any setting stuff

    wholenumber_t rc = Error_Interpretation/1000;      /* set default return code           */
                                         /* did we get a condtion object?     */
    RexxDirectory *conditionObject = getConditionInfo();

    if (conditionObject != OREF_NULL)
    {
        /* force it to display               */
        display(conditionObject);
        /* get the failure return code       */
        conditionObject->at(OREF_RC)->numberValue(&rc);
    }
    return rc;                           /* return the error code             */
}

bool RexxActivity::raiseCondition(
    RexxString    *condition,          /* condition to raise                */
    RexxObject    *rc,                 /* return code value                 */
    RexxObject    *description,        /* description information           */
    RexxObject    *additional,         /* additional information            */
    RexxObject    *result,             /* result value                      */
    RexxDirectory *exobj)              /* exception object                  */
/******************************************************************************/
/* Function:   Raise an actual condition, causing termination for untrapped   */
/*             conditions                                                     */
/******************************************************************************/
{
    bool                handled;         /* this condition has been handled   */
    RexxDirectory      *conditionObj;    /* object for created condition      */


    handled = false;                     /* condition not handled yet         */
    if (exobj == OREF_NULL)
    {            /* need to create a condition object?*/
        conditionObj = new_directory();    /* get a new directory               */
                                           /* put in the condition name         */
        conditionObj->put(condition, OREF_CONDITION);
        /* fill in default description       */
        conditionObj->put(OREF_NULLSTRING, OREF_DESCRIPTION);
        /* fill in the propagation status    */
        conditionObj->put(TheFalseObject, OREF_PROPAGATED);
    }
    else
        conditionObj = exobj;              /* use the existing object           */
    if (rc != OREF_NULL)                 /* have an RC value?                 */
        conditionObj->put(rc, OREF_RC);    /* add to the condition argument     */
    if (description != OREF_NULL)        /* any description to add?           */
        conditionObj->put(description, OREF_DESCRIPTION);
    if (additional != OREF_NULL)         /* or additional information         */
        conditionObj->put(additional, OREF_ADDITIONAL);
    if (result != OREF_NULL)             /* given a return result?            */
        conditionObj->put(result, OREF_RESULT);

    // are we actually running stuff?
    if (currentStackFrame != OREF_NULL)
    {
        // give it a chance to handle this
        handled = currentStackFrame->trap(condition, conditionObj);
    }
    else
    {
        // we're at the bottom of the stack, store in the activation
        // so the APIs can retrieve it.
        currentApiContext->setConditionInfo(conditionObj);
    }

    /* Control will not return here if the condition was trapped via*/
    /* SIGNAL ON SYNTAX.  For CALL ON conditions, handled will be   */
    /* true if a trap is pending.                                   */

    return handled;                      /* this has been handled             */
}

void RexxActivity::reportHalt(         /* report a halt condition           */
    RexxString *description )          /* condition descriptive information */
/******************************************************************************/
/* Function:   Report a HALT condition, raising an error if not trapped.      */
/******************************************************************************/
{
                                       /* process as common condition       */
  if (!raiseCondition(OREF_HALT, OREF_NULL, description, OREF_NULL, OREF_NULL, OREF_NULL))
                                       /* raise as a syntax error           */
    reportException(Error_Program_interrupted_condition, OREF_HALT);
}

void RexxActivity::reportNoMethod(     /* report a NOMETHOD condition       */
    RexxString *message,               /* message that wasn't found         */
    RexxObject *receiver )             /* object that received the message  */
/******************************************************************************/
/* Function:   Report a NOMETHOD condition, raising an error if not trapped.  */
/******************************************************************************/
{
                                       /* process as common condition       */
  if (!raiseCondition(OREF_NOMETHOD, OREF_NULL, message, receiver, OREF_NULL, OREF_NULL))
                                       /* raise as a syntax error           */
    reportException(Error_No_method_name, receiver, message);
}

void RexxActivity::reportNoMethod(     /* report a NOMETHOD condition       */
    RexxObject *receiver )             /* object that received the message  */
/******************************************************************************/
/* Function:   Report a NOMETHOD condition, raising an error if not trapped.  */
/******************************************************************************/
{
    reportNoMethod(getLastMessageName(), receiver);
}

void RexxActivity::reportAnException(
    wholenumber_t errcode )            /* REXX error code                   */
/******************************************************************************/
/* Function:  Forward on an exception condition                               */
/******************************************************************************/
{
                                       /* send along with nothing           */
  this->raiseException(errcode, NULL, OREF_NULL, OREF_NULL, OREF_NULL, OREF_NULL);
}

void RexxActivity::reportAnException(
    wholenumber_t errcode,             /* REXX error code                   */
    RexxObject *substitution1 )        /* substitution information          */
/******************************************************************************/
/* Function:  Forward on an exception condition                               */
/******************************************************************************/
{
  this->raiseException(errcode, NULL, OREF_NULL, OREF_NULL, new_array(substitution1), OREF_NULL);
}

void RexxActivity::reportAnException(
    wholenumber_t errcode,             /* REXX error code                   */
    RexxObject *substitution1,         /* substitution information          */
    RexxObject *substitution2 )        /* substitution information          */
/******************************************************************************/
/* Function:  Forward on an exception condition                               */
/******************************************************************************/
{
  this->raiseException(errcode, NULL, OREF_NULL, OREF_NULL, new_array(substitution1, substitution2), OREF_NULL);
}

void RexxActivity::reportAnException(
    wholenumber_t errcode,             /* REXX error code                   */
    RexxObject *substitution1,         /* substitution information          */
    RexxObject *substitution2,         /* substitution information          */
    RexxObject *substitution3 )        /* substitution information          */
/******************************************************************************/
/* Function:  Forward on an exception condition                               */
/******************************************************************************/
{
  this->raiseException(errcode, NULL, OREF_NULL, OREF_NULL, new_array(substitution1, substitution2, substitution3), OREF_NULL);
}

void RexxActivity::reportAnException(
    wholenumber_t errcode,             /* REXX error code                   */
    RexxObject *substitution1,         /* substitution information          */
    RexxObject *substitution2,         /* substitution information          */
    RexxObject *substitution3,         /* substitution information          */
    RexxObject *substitution4 )        /* substitution information          */
/******************************************************************************/
/* Function:  Forward on an exception condition                               */
/******************************************************************************/
{
  this->raiseException(errcode, NULL, OREF_NULL, OREF_NULL, new_array(substitution1, substitution2, substitution3, substitution4), OREF_NULL);
}

void RexxActivity::reportAnException(
    wholenumber_t errcode,             /* REXX error code                   */
    const stringchar_t *string )       /* single string sustitution parm    */
/******************************************************************************/
/* Function:  Raise an error using a single REXX character string only        */
/*            as a substitution parameter                                     */
/******************************************************************************/
{
                                       /* convert and forward along         */
  this->reportAnException(errcode, new_string((char *)string));
}

void RexxActivity::reportAnException(
    wholenumber_t errcode,             /* REXX error code                   */
    const stringchar_t *string,        /* single string sustitution parm    */
    wholenumber_t  integer )           /* single integer substitution parm  */
/******************************************************************************/
/* Function:  Raise an error using a single REXX character string only        */
/*            as a substitution parameter                                     */
/******************************************************************************/
{
                                       /* convert and forward along         */
  this->reportAnException(errcode, new_string((char *)string), new_integer(integer));
}

void RexxActivity::reportAnException(
    wholenumber_t errcode,             /* REXX error code                   */
    const stringchar_t *string,        /* string value  sustitution parm    */
    wholenumber_t integer,             /* single integer substitution parm  */
    RexxObject   *obj)                 /* and object sub parm               */
/******************************************************************************/
/* Function:  Raise an error using a single REXX character string only        */
/*            as a substitution parameter                                     */
/******************************************************************************/
{
                                       /* convert and forward along         */
  this->reportAnException(errcode, new_string((char *)string), new_integer(integer), obj);
}

void RexxActivity::reportAnException(
    wholenumber_t errcode,             /* REXX error code                   */
    const stringchar_t *string,        /* string value  sustitution parm    */
    RexxObject   *obj,                 /* and object sub parm               */
    wholenumber_t integer)             /* single integer substitution parm  */
/******************************************************************************/
/* Function:  Raise an error using a single REXX character string only        */
/*            as a substitution parameter                                     */
/******************************************************************************/
{
                                       /* convert and forward along         */
  this->reportAnException(errcode, new_string((char *)string), obj, new_integer(integer));
}

void RexxActivity::reportAnException(
    wholenumber_t errcode,             /* REXX error code                   */
    const stringchar_t *string,        /* string value  sustitution parm    */
    RexxObject   *obj)                 /* and object sub parm               */
/******************************************************************************/
/* Function:  Raise an error using a single REXX character string only        */
/*            as a substitution parameter                                     */
/******************************************************************************/
{
                                       /* convert and forward along         */
  this->reportAnException(errcode, new_string((char *)string), obj);
}

void RexxActivity::reportAnException(
    wholenumber_t errcode,             /* REXX error code                   */
    wholenumber_t  integer )           /* single integer substitution parm  */
/******************************************************************************/
/* Function:  Raise an error using a single REXX integer value only           */
/*            as a substitution parameter                                     */
/******************************************************************************/
{
                                       /* convert and forward along         */
  this->reportAnException(errcode, new_integer(integer));
}

void RexxActivity::reportAnException(
    wholenumber_t errcode,             /* REXX error code                   */
    wholenumber_t  integer,            /* single integer substitution parm  */
    wholenumber_t  integer2)           /* single integer substitution parm  */
/******************************************************************************/
/* Function:  Raise an error using a single REXX integer value only           */
/*            as a substitution parameter                                     */
/******************************************************************************/
{
                                       /* convert and forward along         */
  this->reportAnException(errcode, new_integer(integer), new_integer(integer2));
}

void RexxActivity::reportAnException(
    wholenumber_t errcode,             /* REXX error code                   */
    wholenumber_t  a1,                 /* single integer substitution parm  */
    RexxObject *   a2)                 /* single object substitution parm   */
/******************************************************************************/
/* Function:  Raise an error using a single REXX integer value only           */
/*            as a substitution parameter                                     */
/******************************************************************************/
{
                                       /* convert and forward along         */
  this->reportAnException(errcode, new_integer(a1), a2);
}

/**
 * Create a condition object for an error condition and
 * raise the error in the current context.
 *
 * @param errcode    The Rexx error code for the condition.  This is a number with
 *                   both the major and minor code expressed as a 6-digit number.
 * @param location   Any location information for the exception (if available).
 * @param source     The source information for the current context.
 * @param description
 *                   An error description string.
 * @param additional Additional information used for insertion into error messages.
 * @param result     A result object added to the condition object.
 */
void RexxActivity::raiseException(wholenumber_t errcode, LocationInfo *location,
    RexxSource *source, RexxString *description, RexxArray *additional,
    RexxObject    *result )
{
    // during error processing, we need to request the string value of message
    // substitution objects.  It is possible that the string process will also
    // cause a syntax error, resulting in a recursion loop.  We snip that off here,
    // by disallowing a nested error condition.
    if (requestingString)
    {
        throw RecursiveStringError;
    }

    // Skip over any forwarded activations. These technially no longer exist on
    // the execution stack.
    while (currentStackFrame != OREF_NULL && currentStackFrame->isForwarded())
    {
        // terminate this frame and remove
        currentStackFrame->termination();
        popStackFrame(currentStackFrame);
    }

    RexxDirectory *exobj = (RexxDirectory *)new_directory();
    ProtectedObject pe(exobj);

    // now get the primary and secondary codes from the error message.
    wholenumber_t primary = errcode / 1000;
    wholenumber_t secondary = errcode - (primary * 1000);
    char messageId[32];
                                         /* format the number (string) into   */
                                         /*  work buffer.                     */
    sprintf(messageId, "%d.%1d", primary, secondary);
    RexxString *code = new_string(messageId);   /* get the formatted code            */
    exobj->put(code, OREF_CODE);

    RexxInteger *rc = new_integer(primary);  /* get the primary message number    */
    exobj->put(rc, OREF_RC);             /* add the return code               */
                                         /* get the primary message text      */
    RexxString *errortext = SysInterpreter::messageText(primary * 1000);
    if (errortext == OREF_NULL)          /* no corresponding message          */
    {
                                         /* this is an error                  */
        reportException(Error_Execution_error_condition, code);
    }
    exobj->put(errortext, OREF_ERRORTEXT);
    RexxObject *message = TheNilObject;

    if (secondary != 0)
    {            /* have a secondary message to issue?*/
                 /* retrieve the secondary message    */
        message = SysInterpreter::messageText(errcode);
        if (message == OREF_NULL)          /* no corresponding message          */
        {
                                           /* this is an error                  */
            reportException(Error_Execution_error_condition, code);
        }
    }
    exobj->put(message, OREF_NAME_MESSAGE);

    setConditionInfo(exobj);             /* save this in the activity         */
                                         /* fill in the decimal error code    */

    if (additional == OREF_NULL)         /* no description given?             */
    {
                                         /* use a zero size array             */
        additional = (RexxArray *)TheNullArray->copy();
    }
    if (description == OREF_NULL)        /* no description?                   */
    {
        description = OREF_NULLSTRING;     /* use a null string                 */
    }

                                           /* fill in the arguments             */
    exobj->put(description, OREF_DESCRIPTION);
    exobj->put(additional, OREF_ADDITIONAL);
    exobj->put((RexxObject *)source, OREF_SOURCENAME);
    exobj->put(result, OREF_RESULT);

    RexxInteger *position = OREF_NULL;
    if (location != NULL)
    {              /* have clause information available?*/
                   /* add the line number information   */
        exobj->put(new_integer(location->line), OREF_POSITION);
    }
    // still have an active frame?
    else if (currentStackFrame != OREF_NULL)
    {
        /* get the activation position       */
        exobj->put(new_integer(currentStackFrame->currentLine()), OREF_POSITION);
    }

    RexxList *traceback = new_list();    /* create a traceback list           */
                                         /* add to the exception object       */
    exobj->put(traceback, OREF_TRACEBACK);
    if (source != OREF_NULL)             /* have source for this?             */
    {
                                         /* extract and add clause in error   */
        traceback->addLast(source->traceBack(location, 0, false));
    }
    RexxString *programname = OREF_NULLSTRING;
    /* have predecessors?                */
    if (currentStackFrame != OREF_NULL)
    {
        currentStackFrame->traceBack(traceback);  /* have them add lines to the list   */
    }
    if (source != OREF_NULL)             /* have source for this?             */
    {
        programname = source->getProgramName(); /* extract program name              */
    }
                                           /* have predecessors?                */
    else if (currentStackFrame != OREF_NULL)
    {
        /* extract program name from activa. */
        programname = currentStackFrame->programName();
    }
    exobj->put(programname, OREF_PROGRAM);
    exobj->put(OREF_SYNTAX, OREF_CONDITION);
    /* fill in the propagation status    */
    exobj->put(TheFalseObject, OREF_PROPAGATED);

    if (message != TheNilObject)
    {       /* have a secondary message?         */
            /* do required substitutions         */
        message = RexxInterpreter::messageSubstitution((RexxString *)message, additional);
        /* replace the original message text */
        exobj->put(message, OREF_NAME_MESSAGE);
    }
    /* process as common condition       */
    if (!this->raiseCondition(OREF_SYNTAX, OREF_NULL, OREF_NULL, OREF_NULL, OREF_NULL, exobj))
    {
        /* fill in the propagation status    */
        exobj->put(TheTrueObject, OREF_PROPAGATED);
        // actually have an activation left?
        if (currentStackFrame != OREF_NULL)
        {
            // still doing tracebacks?
            if (currentStackFrame->settings.traceindent > MAX_TRACEBACK_LIST)
            {
                traceback->addLast(new_string("     >...<"));
            }
            currentStackFrame->termination();        /* do activation termiantion process */
            popStackFrame(currentStackFrame);        /* pop the top activation off        */
        }
        raisePropagate(exobj);       /* pass on down the chain            */
    }
}


void RexxActivity::reraiseException(
    RexxDirectory *exobj )             /* previously created exception obj. */
/******************************************************************************/
/* Function:  Propagate a syntax condition and trapping to earlier levels     */
/******************************************************************************/
{
  RexxActivation *activation;          /* current activation                */
  RexxArray      *additional;          /* passed on information             */
  RexxObject     *errorcode;           /* full error code                   */
  RexxString     *message;             /* secondary error message           */
  wholenumber_t   errornumber;         /* binary error number               */
  wholenumber_t   primary;             /* primary message code              */
  stringchar_t    work[10];            /* temp buffer for formatting        */
  wholenumber_t   newVal;

  activation = currentStackFrame;      /* get the current activation        */
                                       /* have a target activation?         */
  if (activation != OREF_NULL)
  {
    newVal = activation->currentLine();/* get the activation position       */
    exobj->put(new_integer(newVal), OREF_POSITION);
                                       /* extract program name from activa. */
    exobj->put(activation->programName(), OREF_PROGRAM);
  }
  else {
    exobj->remove(OREF_POSITION);      /* remove the position               */
    exobj->remove(OREF_PROGRAM);       /* remove the program name           */
  }

  errorcode = exobj->at(OREF_CODE);    /* get the error code                */
                                       /* convert to a decimal              */
  errornumber = RexxInterpreter::messageNumber((RexxString *)errorcode);
                                       /* get the primary message number    */
  primary = (errornumber / 1000) * 1000;
  if (errornumber != primary) {        /* have an actual secondary message? */
                                       /* format the number (string) into   */
                                       /*  work buffer.                     */
    sprintf((char *)work,"%1d%3.3d", errornumber/1000, errornumber - primary);
    errornumber = atol((char *)work);  /* convert to a long value           */
                                       /* retrieve the secondary message    */
    message = SysInterpreter::messageText(errornumber);
                                       /* Retrieve any additional parameters*/
    additional = (RexxArray *)exobj->at(OREF_ADDITIONAL);
                                       /* do required substitutions         */
    message = RexxInterpreter::messageSubstitution(message, additional);
                                       /* replace the original message text */
    exobj->put(message, OREF_NAME_MESSAGE);
  }
  this->raisePropagate(exobj);         /* pass on down the chain            */
}

/**
 * Raise/propagate an error condition down the stack frame
 * chain.  This will terminate at the first native activation.
 * If the native activation is a method, exit, or call context,
 * we'll just store the information to allow the native code
 * a chance to examine or cancel the condition.
 *
 * @param conditionObj
 *               An already constructed condition object.
 */
void RexxActivity::raisePropagate(RexxDirectory *conditionObj)
{
    /* get the condition                 */
    RexxString *condition = (RexxString *)conditionObj->at(OREF_CONDITION);
    RexxList *traceback;                 /* traceback information             */
                                         /* propagating syntax errors?        */
    if (condition->strCompare(CHAR_SYNTAX))
    {
        /* get the traceback                 */
        traceback = (RexxList *)conditionObj->at(OREF_TRACEBACK);

    }
    else
    {
        traceback = OREF_NULL;             /* no trace back to process          */
    }

    /* loop to the top of the stack      */
    while (currentStackFrame != OREF_NULL)
    {
        /* give this one a chance to trap    */
        /* (will never return for trapped    */
        /* PROPAGATE conditions)             */
        currentStackFrame->trap(condition, conditionObj);
        /* this is a propagated condition    */
        conditionObj->put(TheTrueObject, OREF_PROPAGATED);
        if ((traceback != TheNilObject) && (currentStackFrame->settings.traceindent < MAX_TRACEBACK_LIST))  /* have a traceback? */
        {
            currentStackFrame->traceBack(traceback);/* add this to the traceback info    */
        }
        currentStackFrame->termination();  /* do activation termiantion process */
        popStackFrame(currentStackFrame);  // remove this from the stack
    }
    // this is unhandled, so we need to pass off to the API context.
    unhandledCondition(conditionObj);
}

RexxObject *RexxActivity::display(RexxDirectory *exobj)
                                       /* display exception object info     */
                                       /* target exception object           */
/******************************************************************************/
/* Function:  Display information from an exception object                    */
/******************************************************************************/
{
  RexxArray  *trace_back;              /* traceback information             */
  RexxList   *trace_backList;          /* traceback information             */
  RexxString *secondary;               /* secondary message                 */
  RexxString *text;                    /* constructed final message         */
  RexxObject *position;                /* position in program error occurred*/
  RexxString *programname;             /* name of program being run         */
  size_t    size;                      /* traceback array size              */
  size_t    i;                         /* loop counter                      */
  stringsize_t starttrc = 1;           /* number of first trace line        */
  wholenumber_t  errorCode;            /* error message code                */
  RexxObject *rc;

                                       /* get the traceback info            */
  trace_backList = (RexxList *)exobj->at(OREF_TRACEBACK);
  if (trace_backList != OREF_NULL) {   /* have a traceback?                 */
                                       /* convert to an array               */
    trace_back = trace_backList->makeArray();
                                       /* save from gc                      */
    ProtectedObject p1(trace_back);
                                       /* get the traceback size            */
    size = trace_back->size();

    for (i= starttrc; i <= size; i++) {       /* loop through the traceback starttrc */
      text = (RexxString *)trace_back->get(i);
                                       /* have a real line?                 */
      if (text != OREF_NULL && text != TheNilObject)
                                       /* write out the line                */
        this->traceOutput(this->currentStackFrame, text);
    }
  }
  rc = exobj->at(OREF_RC);             /* get the error code                */
                                       /* get the message number            */
  errorCode = RexxInterpreter::messageNumber((RexxString *)rc);
                                       /* get the header                    */
  text = RexxInterpreter::messageHeader(errorCode);
  if (text == OREF_NULL)               /* no header available?              */
                                       /* get the leading part              */
    text = SysInterpreter::messageText(Message_Translations_error);
  else                                 /* add to the message text           */
    text = text->concat(SysInterpreter::messageText(Message_Translations_error));
                                       /* get the name of the program       */
  programname = (RexxString *)exobj->at(OREF_PROGRAM);
                                       /* add on the error number           */
  text = text->concatWith(REQUEST_STRING(rc), ' ');
                                       /* if program exists, add the name   */
                                       /* of the program to the message     */
  if (programname != (RexxString *)OREF_NULL && programname != OREF_NULLSTRING) {
                                       /* add on the "running" part         */
    text = text->concatWith(SysInterpreter::messageText(Message_Translations_running), ' ');
                                       /* add on the program name           */
    text = text->concatWith(programname, ' ');
                                       /* Get the position/Line number info */
    position = exobj->at(OREF_POSITION);
    if (position != OREF_NULL) {       /* Do we have position/Line no info? */
                                       /* Yes, add on the "line" part       */
      text = text->concatWith(SysInterpreter::messageText(Message_Translations_line), ' ');
                                       /* add on the line number            */
      text = text->concatWith(REQUEST_STRING(exobj->at(OREF_POSITION)), ' ');
                                       /* add on the ":  "                  */
    }
  }
  text = text->concatWithCstring(":  ");
                                       /* and finally the error message     */
  text = text->concat((RexxString *)exobj->at(OREF_ERRORTEXT));
                                       /* write out the line                */
  this->traceOutput(this->currentStackFrame, text);
                                       /* get the secondary message         */
  secondary = (RexxString *)exobj->at(OREF_NAME_MESSAGE);
                                       /* have a real message?              */
  if (secondary != OREF_NULL && secondary != (RexxString *)TheNilObject) {
    rc = exobj->at(OREF_CODE);         /* get the error code                */
                                       /* get the message number            */
    errorCode = RexxInterpreter::messageNumber((RexxString *)rc);
                                       /* get the header                    */
    text = RexxInterpreter::messageHeader(errorCode);
    if (text == OREF_NULL)             /* no header available?              */
                                       /* get the leading part              */
      text = SysInterpreter::messageText(Message_Translations_error);
    else                               /* add to the message text           */
      text = text->concat(SysInterpreter::messageText(Message_Translations_error));
                                       /* add on the error number           */
    text = text->concatWith((RexxString *)rc, ' ');
                                       /* add on the ":  "                  */
    text = text->concatWithCstring(":  ");
                                       /* and finally the error message     */
    text = text->concat(secondary);
                                       /* write out the line                */
    this->traceOutput(this->currentStackFrame, text);
  }
  return TheNilObject;                 /* just return .nil                  */
}

RexxObject *RexxActivity::displayDebug(RexxDirectory *exobj)
                                       /* display exception object info     */
                                       /* target exception object           */
/******************************************************************************/
/* Function:  Display information from an exception object                    */
/******************************************************************************/
{
  RexxString *secondary;               /* secondary message                 */
  RexxString *text;                    /* constructed final message         */

                                       /* get the leading part              */
  text = SysInterpreter::messageText(Message_Translations_debug_error);
                                       /* add on the error number           */
  text = text->concatWith(REQUEST_STRING(exobj->at(OREF_RC)), ' ');
                                       /* add on the ":  "                  */
  text = text->concatWithCstring(":  ");
                                       /* and finally the error message     */
  text = text->concatWith((RexxString *)exobj->at(OREF_ERRORTEXT), ' ');
                                       /* write out the line                */
  this->traceOutput(this->currentStackFrame, text);
                                       /* get the secondary message         */
  secondary = (RexxString *)exobj->at(OREF_NAME_MESSAGE);
                                       /* have a real message?              */
  if (secondary != OREF_NULL && secondary != (RexxString *)TheNilObject) {
                                       /* get the leading part              */
    text = SysInterpreter::messageText(Message_Translations_debug_error);
                                       /* add on the error number           */
    text = text->concatWith((RexxString *)exobj->at(OREF_CODE), ' ');
                                       /* add on the ":  "                  */
    text = text->concatWithCstring(":  ");
                                       /* and finally the error message     */
    text = text->concat(secondary);
                                       /* write out the line                */
    this->traceOutput(this->currentStackFrame, text);
  }
  return TheNilObject;                 /* just return .nil                  */
}

void RexxActivity::live()
/******************************************************************************/
/* Function:  Normal garbage collection live marking                          */
/******************************************************************************/
{
  setUpMemoryMark
  memory_mark(this->currentStackFrame);
  memory_mark(this->currentApiContext);
  memory_mark(this->local);
  memory_mark(this->nextWaitingActivity);
  memory_mark(this->nextProcessActivity);
  memory_mark(this->waitingObject);
  memory_mark(this->lastMessageName);
  memory_mark(this->lastMethod);

  /* have the frame stack do its own marking. */
  frameStack.live();

  // and the platform-specific bit might also contain references.
  systemInfo.live();

  ProtectedObject *p = protectedObjects;
  while (protectedObjects != NULL)
  {
      memory_mark(p->protectedObject);
  }

  // now see if there are any thread-local objects needing protection.
  cleanUpMemoryMark
}
void RexxActivity::liveGeneral()
/******************************************************************************/
/* Function:  Generalized object marking                                      */
/******************************************************************************/
{
  setUpMemoryMarkGeneral
  memory_mark_general(this->local);
  memory_mark_general(this->currentStackFrame);
  memory_mark_general(this->currentApiContext);
  memory_mark_general(this->nextWaitingActivity);
  memory_mark_general(this->nextProcessActivity);
  memory_mark_general(this->waitingObject);

  /* have the frame stack do its own marking. */
  frameStack.liveGeneral();

  // and the platform-specific bit might also contain references.
  systemInfo.liveGeneral();

  ProtectedObject *p = protectedObjects;
  while (protectedObjects != NULL)
  {
      memory_mark_general(p->protectedObject);
  }

  cleanUpMemoryMarkGeneral
}

void RexxActivity::flatten(RexxEnvelope* envelope)
/******************************************************************************/
/* Function:  Flatten an object                                               */
/******************************************************************************/
{
                                       /* Activities don't get moved,       */
                                       /*  we just return OREF_NULL. we may */
                                       /*  create a special proxy for this  */
                                       /*  to re-establish an activation on */
                                       /*  system.                          */
  return;
}

void RexxActivity::run()
/******************************************************************************/
/* Function:  Release an activity to run                                      */
/******************************************************************************/
{
  this->guardsem.post();               /* and the guard semaphore           */
  this->runsem.post();                 /* post the run semaphore            */
  _thread.yield();                     /* yield the thread                  */
}

void RexxActivity::createLocalReference(RexxObject *obj)
{
    currentApiContext->createLocalReference(obj);
}

void RexxActivity::removeLocalReference(RexxObject *obj)
{
    currentApiContext->removeLocalReference(obj);
}


/**
 * Allocate a new activation and add it to the thread's activation
 * stack.
 *
 * @return A newly allocated RexxActivation.
 */
RexxActivation *RexxActivity::pushStackFrame()
{
    // perform stack space checks here
    stackSpace();
    RexxActivation *new_activation = new RexxActivation(this);
                                       // chain this to the current top activation
    new_activation->setPreviousStackFrame(currentStackFrame);
    currentStackFrame = new_activation;  /* set this as the top one           */
    resetSettings();                     // update the settings to reflect the new stack
    return new_activation;
}

void RexxActivity::setErrorListener(RexxMessage *l)
{
    // currently working from Rexx code?
    if (currentStackFrame != OREF_NULL)
    {
        // attach it to that
        currentStackFrame->setErrorListener(l);
    }
    else
    {
        // attach this to the native context
        currentApiContext->setErrorListener(l);
    }
}

void RexxActivity::pushNativeActivation(
    RexxNativeActivation *new_activation)      /* activation to add                 */
/******************************************************************************/
/* Function:  Push an activation onto the activity stack                      */
/******************************************************************************/
{
  // chain up the API contexts
  new_activation->setPreviousContext(currentApiContext, currentStackFrame);
  // this is the current API context now...
  currentApiContext = new_activation;
  // and from the standpoint of everybody else, there are no Rexx activations
  // going on.  All access to any predecessor is through the API context.
  currentStackFrame = OREF_NULL;
  //NB:  We leave the current settings alone here.  We're most likely doing this
  // because we're about the leave the interpreter anyway, which will set the global
  // stuff back to the defaults.
  nestedCount++;      // also keep track of the nesting level
}

void RexxActivity::resetSettings()
/******************************************************************************/
/* Function:  Reset the settings based on a framestack change                 */
/******************************************************************************/
{
    // no Rexx stack frames active?
    if (currentStackFrame == OREF_NULL)
    {
                                         /* use the default settings          */
        this->settings = &RexxInterpreter::defaultSettings;
    }
    else
    {
                                       /* get the activation settings       */
        settings = currentStackFrame->getGlobalSettings();
        // if we're the active activity, update the global settings
        if (RexxInterpreter::currentActivity == this)
        {
                                             /* update the active values          */
            RexxInterpreter::currentSettings = this->settings;
        }
    }
}

/**
 * Unwind the stack frame to a target depth location, terminating
 * each of the removed activations along the way.
 *
 * @param target The unwind target.
 */
void RexxActivity::unwindStackFrame(RexxActivation *target)
{
    RexxActivation *current = getCurrentContext();
    // unwind the activation stack until we get to the target one.
    while (current != target)
    {
        current->termination();
        // pop off of the stack and get the new activation
        current = popStackFrame(current);
    }
}


RexxActivation *RexxActivity::popStackFrame(
    RexxActivation *activation)      // the activation we're popping to
/******************************************************************************/
/* Function:  Remove an activation from the activity stack                    */
/******************************************************************************/
{
    // get the previous frame from the one we're popping
    currentStackFrame = activation->getPreviousStackFrame();
    // redo the settings
    resetSettings();
    return currentStackFrame;
}

void RexxActivity::popNativeActivation(
    RexxNativeActivation *activation)
/******************************************************************************/
/* Function:  Remove an activation from the activity stack                    */
/******************************************************************************/
{
    // get the previous frame from the one we're popping
    currentStackFrame = activation->getPreviousStackFrame();
    // and also pop the API context
    currentApiContext = activation->getPreviousApiContext();
    // redo the settings
    resetSettings();
    // reduce the nesting count
    nestedCount--;
}




void RexxActivity::exitKernel(
  RexxNativeActivation::ActivationType t) // the type of activation we're creating
/******************************************************************************/
/*  Function:  Release the kernel access because code is going to "leave"     */
/*             the kernel.  This prepares for this by adding a native         */
/*             activation on to the stack to act as a server for the          */
/*             external call.  This way new native methods do not need to     */
/*             be created just to get an activation created                   */
/******************************************************************************/
{
                                       /* create a new native activation    */
  RexxNativeActivation *new_activation = new RexxNativeActivation (this, currentStackFrame, t);
                                       /* push it on the activity stack     */
  this->pushNativeActivation(new_activation);
  releaseAccess();                     /* now give up control               */
}

void RexxActivity::enterKernel()
/******************************************************************************/
/*  Function:  Recover the kernel access and pop the native activation        */
/*             created by activity_exit_kernel from the activity stack.       */
/******************************************************************************/
{
    requestAccess();                     /* get the kernel lock back          */
    popNativeActivation(currentApiContext);  // and pop this off the top
}

void RexxActivity::checkDeadLock(
  RexxActivity *targetActivity)        /* activity currently reserving      */
/******************************************************************************/
/* Function:  Check for a circular wait dead lock error                       */
/******************************************************************************/
{
  RexxActivity *owningActivity;        /* activity owning the resource      */
                                       /* currently waiting on something?   */
  if (this->waitingObject != OREF_NULL)
  {
                                       /* waiting on a message object?      */
    if (isOfClass(Message, this->waitingObject))
                                       /* get the activity message is on    */
      owningActivity = ((RexxMessage *)this->waitingObject)->startActivity;
    else
                                       /* get the locking activity for vdict*/
      owningActivity = ((RexxVariableDictionary *)this->waitingObject)->reservingActivity;
                                       /* have a circular wait              */
    if (owningActivity == targetActivity)
                                       /* have a deaklock                   */
      reportException(Error_Execution_deadlock);
    if (owningActivity != OREF_NULL)   /* have a real activity?             */
                                       /* pass it along the chain           */
      owningActivity->checkDeadLock(targetActivity);
  }
}

void RexxActivity::waitReserve(
  RexxObject *resource )               /* resource we are waiting on        */
/******************************************************************************/
/* Function:  Wait for a new run event to occur                               */
/******************************************************************************/
{
  this->runsem.reset();                /* clear the run semaphore           */
  this->waitingObject = resource;      /* save the waiting resource         */
  // release access and wait
  {
      UnsafeBlock ublock;
      this->runsem.wait();             /* wait for the run to be posted     */
  }
}

void RexxActivity::guardWait()
/******************************************************************************/
/* Function:  Wait for a guard post event                                     */
/******************************************************************************/
{
    // release access and wait
    UnsafeBlock ublock;
    this->guardsem.wait();               /* wait on the guard semaphore       */
}

void RexxActivity::guardPost()
/******************************************************************************/
/* Function:  Post a guard expression wake up notice                          */
/******************************************************************************/
{
                                       /* make sure we have access to sem   */
  this->guardsem.open();               /* may be called from another process*/
  this->guardsem.post();               /* OK for it to already be posted    */
  this->guardsem.close();              /* release access to sem.            */
}

void RexxActivity::guardSet()
/******************************************************************************/
/* Function:  Clear a guard expression semaphore in preparation to perform a  */
/*            guard wait                                                      */
/******************************************************************************/
{
  this->guardsem.reset();              /* set up for guard call             */
}

void RexxActivity::postRelease()
/******************************************************************************/
/* Function:  Post an activities run semaphore                                */
/******************************************************************************/
{
  this->waitingObject = OREF_NULL;     /* no longer waiting                 */
                                       /* make sure we have access to sem   */
  this->runsem.open();                 /* may be called from another process*/
  this->runsem.post();                 /* OK for it to already be posted    */
  this->runsem.close();                /* release access to sem.            */
}

/**
 * Process a condition unhandled by Rexx code.
 *
 * @param conditionObj
 *               The constructed condition object.
 */
void RexxActivity::unhandledCondition(RexxDirectory *conditionObj)
{
    setConditionInfo(conditionObj);      // save the condition object
    throw UnhandledCondition;            // we have a fatal error condition
}

void RexxActivity::relinquish()
/******************************************************************************/
/*  Function: Relinquish to other system processes                            */
/******************************************************************************/
{
                                       /* other's waiting to go?            */
  if (TheActivityManager->waitingActivity() != OREF_NULL) {
                                       /* now join the line                 */
    TheActivityManager->addWaitingActivity(this, true);
  }
  SysProcess::relinquish();        /* now allow system stuff to run     */
}

// is yield ever called with OREF_NULL?  if so, create second version
void RexxActivity::yield()
/******************************************************************************/
/* Function:  Yield control so some other activity can run                    */
/******************************************************************************/
{
                                       /* other's waiting to go?            */
  if (TheActivityManager->waitingActivity() != OREF_NULL) {
                                       /* now join the line                 */
      TheActivityManager->addWaitingActivity(this, true);
  }
}

void RexxActivity::releaseAccess()
/******************************************************************************/
/* Function:  Release exclusive access to the kernel                          */
/******************************************************************************/
{
    RexxInterpreter::unlockInterpreter();
}

void RexxActivity::requestAccess()
/******************************************************************************/
/* Function:  Acquire exclusive access to the kernel                          */
/******************************************************************************/
{
    /* only one there?                   */
    if (TheActivityManager->waitingActivity() == OREF_NULL)
    {
        // try to get the execution lock
        if (RexxInterpreter::requestInterpreter(this))
        {
            return;

        }
    }
    /* can't get it, go stand in line    */
    TheActivityManager->addWaitingActivity(this, false);
}

void RexxActivity::stackSpace()
/******************************************************************************/
/* Function:  Make sure there is enough stack space to run a method           */
/******************************************************************************/
{
#ifdef STACKCHECK
  int  temp;                           /* if checking and there isn't room  */
  if (((char *)&temp - (char *)nestedInfo.stackptr) < MIN_C_STACK && this->stackcheck == true)
                                       /* go raise an exception             */
    reportException(Error_Control_stack_full);
#endif
}

RexxString *RexxActivity::getCurrentQueue()
/******************************************************************************/
/* Function:  Return the current queue object name                            */
/******************************************************************************/
{
  RexxString * queue;                  /* current queue object              */
  RexxString * queue_name;             /* name of the queue object          */

                                       /* get the default queue             */
  queue = (RexxString *)local->at(OREF_REXXQUEUE);

  if (queue == OREF_NULL)              /* no queue?                         */
    queue_name = OREF_SESSION;         /* use the default name              */
  else
    queue_name = queue->stringValue(); /* get the actual queue name         */
  return queue_name;                   /* return the name                   */
}

void RexxActivity::queryTrcHlt()
/******************************************************************************/
/* Function:  Determine if Halt or Trace System exits are set                 */
/*            and set a flag to indicate this.                                */
/******************************************************************************/
{                                      /* is HALT sys exit set              */
  if (this->nestedInfo.exits[RXHLT].isEnabled()) {
    this->nestedInfo.exitset = true;   /* set flag to indicate one is found */
    return;                            /* and return                        */
  }
                                       /* is TRACE sys exit set             */
  if (this->nestedInfo.exits[RXTRC].isEnabled()) {
    this->nestedInfo.exitset = true;   /* set flag to indicate one is found */
    return;                            /* and return                        */
  }

  if (this->nestedInfo.exits[RXDBG].isEnabled()) {
    this->nestedInfo.exitset = true;   /* set flag to indicate one is found */
    return;                            /* and return                        */
  }

  this->nestedInfo.exitset = false;    /* remember that none are set        */
  return;                              /* and return                        */
}

void RexxActivity::callInitializationExit(
    RexxActivation *activation)        /* sending activation                */
/******************************************************************************/
/* Function:   Calls the SysExitHandler method on the System Object to run    */
/*             the initialization system exit.                                */
/******************************************************************************/
{
    if (!isExitEnabled(RXINI))  // is the exit enabled?
    {
                                       /* call the handler                  */
        callExit(RXINI, RXINIEXT, NULL, true);
    }
}

void RexxActivity::callTerminationExit(
    RexxActivation *activation)        /* sending activation                */
/******************************************************************************/
/* Function:   Calls the SysExitHandler method on the System Object to run    */
/*             the termination system exit.                                   */
/******************************************************************************/
{
    if (!isExitEnabled(RXINI))  // is the exit enabled?
    {
                                           /* call the handler                  */
        callExit(RXTER, RXTEREXT, NULL, true);
    }
}

bool  RexxActivity::callSayExit(
    RexxActivation *activation,        /* sending activation                */
    RexxString     *sayoutput)         /* line to write out                 */
/******************************************************************************/
/* Function:   Calls the SysExitHandler method on the System Object to run    */
/*             the I/O system exit.                                           */
/******************************************************************************/
{
    if (!isExitEnabled(RXSIO))  // is the exit enabled?
    {
        RxSayParm     exit_parm;             /* exit parameters                   */
                                           /* make into RXSTRING form           */
        exit_parm.output_string = sayoutput;
                                           /* call the handler                  */
        return callExit(RXSIO, RXSIOSAY, (void *)&exit_parm, false);
    }
    return true;                         /* exit didn't handle                */
}

bool RexxActivity::callNovalueExit(
    RexxActivation *activation,        /* sending activation                */
    RexxString     *variableName,      /* name to look up                   */
    RexxObject    **value)             /* the returned value                */
/******************************************************************************/
/* Function:   Calls the novalue handler for uninitialized variables          */
/******************************************************************************/
{
    if (!isExitEnabled(RXNOVAL))  // is the exit enabled?
    {
        RxNovalueParm exit_parm;           /* exit parameters                   */
                                           /* make into RXSTRING form           */
        exit_parm.variable_name = variableName;
        exit_parm.variable_value = NULL;   /* no value at the start             */
                                           /* call the handler                  */
        if (!callExit(RXNOVAL, RXNOVALCALL, (void *)&exit_parm, false))
        {
            *value = (RexxObject *)exit_parm.variable_value;
            return false;
        }
    }
    return true;                         /* exit didn't handle                */
}

bool RexxActivity::callValueExit(
    RexxString     *selector,          /* name to look up                   */
    RexxString     *name,              /* name within the selector space    */
    RexxObject    **value,             /* the returned value                */
    RexxObject     *newValue)          /* optional newly assigned value     */
/******************************************************************************/
/* Function:   Calls the novalue handler for uninitialized variables          */
/******************************************************************************/
{
    if (!isExitEnabled(RXVALUE))  // is the exit enabled?
    {
        RxValueParm exit_parm;             /* exit parameters                   */
        // fill in the argument bits
        exit_parm.name = name;
        exit_parm.selector = selector;
        exit_parm.new_value = newValue;
        // make sure the oldValue is nulled out
        exit_parm.old_value = OREF_NULL;

        if (callExit(RXVALUE, RXVALUECALL, (void *)&exit_parm, false))
        {
            return true;                     /* this wasn't handled               */
        }

        // get the string value
        *value = (RexxString *)exit_parm.old_value;
        return false;                        /* this was handled                  */
    }
    return true;                         /* exit didn't handle                */
}

bool RexxActivity::callTraceExit(
    RexxActivation *activation,        /* sending activation                */
    RexxString     *traceoutput)       /* line to write out                 */
/******************************************************************************/
/* Function:   Calls the SysExitHandler method on the System Object to run    */
/*             the I/O system exit.                                           */
/******************************************************************************/
{
    if (!isExitEnabled(RXSIO))  // is the exit enabled?
    {
        RxTraceOutputParm exit_parm;         /* exit parameters                   */
                                       /* make into RXSTRING form           */
        exit_parm.output_string = traceoutput;
                                       /* call the handler                  */
        return callExit(RXSIO, RXSIOTRC, (void *)&exit_parm, false);
    }
    return true;                       /* exit didn't handle                */
}

bool RexxActivity::callPullExit(
    RexxActivation *activation,        /* sending activation                */
    RexxString **inputString)          /* returned input string             */
/******************************************************************************/
/* Function:   Calls the SysExitHandler method on the System Object to run    */
/*             the I/O system exit.                                           */
/******************************************************************************/
{
    if (!isExitEnabled(RXSIO))    // is the exit enabled?
    {
        RxTerminalPullParm exit_parm;      /* exit parameters                   */
                                           /* Pass along default RXSTRING       */
        exit_parm.input_string = OREF_NULL;
        if (callExit(RXSIO, RXSIOTRD, (void *)&exit_parm, false))
        {
            return true;                     /* this wasn't handled               */
        }

        // get the string value
        *inputString = (RexxString *)exit_parm.input_string;
        return false;                        /* this was handled                  */
    }
    return true;                             /* not handled                       */
}

bool RexxActivity::callDebugPullExit(
    RexxActivation *activation,        /* sending activation                */
    RexxString    **inputString)       /* returned input value              */
/******************************************************************************/
/* Function:   Calls the SysExitHandler method on the System Object to run    */
/*             the I/O system exit.                                           */
/******************************************************************************/
{
    if (!isExitEnabled(RXSIO))  // is the exit enabled?
    {
        RxDebugReadParm exit_parm;           /* exit parameters                   */
        exit_parm.input_string = OREF_NULL;
                                           /* call the handler                  */
        if (callExit(RXSIO, RXSIODTR, (void *)&exit_parm, false))
          return true;                     /* this wasn't handled               */
        // get the string value
        *inputString = (RexxString *)exit_parm.input_string;
        return false;                      /* this was handled                  */
    }
    return true;                         /* not handled                       */
}

bool RexxActivity::callFunctionExit(
    RexxActivation *activation,        /* calling activation                */
    int             type,              /* the type of call to make          */
    RexxString     *rname,             /* routine name                      */
    RexxObject     *calltype,          /* type of call                      */
    ProtectedObject &funcresult,       /* function result                   */
    RexxObject    **arguments,         /* argument array                    */
    size_t          argcount)          /* argument count                    */
/******************************************************************************/
/* Function:   Calls the SysExitHandler method on the System Object to run    */
/*             the function system exit.                                      */
/******************************************************************************/
{
    RxFunctionExitParm exit_parm;        /* exit parameters                   */
    /* need to do a security check?      */
    if (activation->hasSecurityManager())
    {
        RexxDirectory *securityArgs = new_directory();    /* get a directory item              */
        ProtectedObject p1(securityArgs);
        /* add the function name             */
        securityArgs->put(rname, OREF_NAME);
        /* and the arguments                 */
        securityArgs->put(new (argcount, arguments) RexxArray, OREF_ARGUMENTS);
        /* did manager handle this?          */
        if (activation->callSecurityManager(OREF_CALL, securityArgs))
        {
            /* get the return code               */
            funcresult = securityArgs->fastAt(OREF_RESULT);
            return false;                    /* we've handled this                */
        }
    }

    if (!isExitEnabled(type)) // is the exit enabled?
    {
        return true;                      // exit out of here now
    }

    exit_parm.rxfnc_flags.rxfferr = 0; /* Initialize error codes to zero */
    exit_parm.rxfnc_flags.rxffnfnd = 0;
    if (calltype == OREF_FUNCTIONNAME) /* function call?                    */
                                       /* set the function flag             */
        exit_parm.rxfnc_flags.rxffsub = 0;
    else
        /* this is the CALL instruction      */
        exit_parm.rxfnc_flags.rxffsub = 1;
    /* fill in the name parameter        */
    exit_parm.function_name = rname;
    /* fill in the current queue name    */
    exit_parm.queue_name = getCurrentQueue();
    /* Build arg array of RXSTRINGs      */
    /* get number of args                */
    exit_parm.argument_count = argcount;

    /* allocate enough memory for all arguments.           */
    /* At least one item needs to be allocated in order to avoid an error   */
    /* in the sysexithandler!                                               */
    exit_parm.arguments = (RexxObjectPtr *)arguments;
    exit_parm.return_value = NULL;
    /* call the handler                  */
    if (callExit(type, RXFNCCAL, (void *)&exit_parm, true))
    {
        return true;
    }

    if (exit_parm.rxfnc_flags.rxfferr) /* function error?                   */
                                       /* raise an error                    */
        reportException(Error_Incorrect_call_external, rname);
    /* Did we find the function??        */
    else if (exit_parm.rxfnc_flags.rxffnfnd)
        /* also an error                     */
        reportException(Error_Routine_not_found_name,rname);
    /* Was it a function call??          */
    if (exit_parm.return_value == NULL && calltype == OREF_FUNCTIONNAME)
        /* Have to return data               */
        reportException(Error_Function_no_data_function,rname);

    funcresult = (RexxObject *)exit_parm.return_value;
    return false;                      /* this was handled                  */
}

bool RexxActivity::callSubcommandExit(
    RexxActivation *activation,       /* issuing activation                */
    RexxString *cmdname,              /* command name                      */
    RexxString *environment,          /* environment                       */
    RexxString **conditions,          /* failure/error conditions status   */
    ProtectedObject &cmdresult)       /* function result                   */
/******************************************************************************/
/* Function:   Calls the SysExitHandler method on the System Object to run    */
/*             the command system exit.                                       */
/******************************************************************************/
{
    RxCommandExitParm exit_parm;         /* exit parameters                   */
    RexxDirectory *securityArgs;         /* security check arguments          */

                                         /* need to do a security check?      */
    if (activation->hasSecurityManager())
    {
        securityArgs = new_directory();    /* get a directory item              */
        ProtectedObject p1(securityArgs);
        /* add the command                   */
        securityArgs->put(cmdname, OREF_COMMAND);
        /* and the target                    */
        securityArgs->put(environment, OREF_ADDRESS);
        /* did manager handle this?          */
        if (activation->callSecurityManager(OREF_COMMAND, securityArgs))
        {
            /* get the return code               */
            cmdresult = securityArgs->fastAt(OREF_RC);
            if ((RexxObject *)cmdresult == OREF_NULL)     /* no return code provide?           */
                cmdresult = IntegerZero;      /* use a zero return code            */
                                               /* failure indicated?                */
            if (securityArgs->fastAt(OREF_FAILURENAME) != OREF_NULL)
                *conditions = OREF_FAILURENAME;/* raise a FAILURE condition         */
                                               /* how about an error condition?     */
            else if (securityArgs->fastAt(OREF_ERRORNAME) != OREF_NULL)
                *conditions = OREF_ERRORNAME;  /* raise an ERROR condition          */
            return false;                    /* we've handled this                */
        }
    }
    if (!isExitEnabled(RXFNC))  // is the exit enabled?
    {
        return true;                     // exit out of here now
    }
    /* Start building the exit block     */
    exit_parm.rxcmd_flags.rxfcfail = 0;/* Initialize failure/error to zero  */
    exit_parm.rxcmd_flags.rxfcerr = 0;
    /* fill in the environment parm      */
    exit_parm.address_name = environment;
    exit_parm.command_string = cmdname;
    exit_parm.return_value = OREF_NULL;

    /* call the handler                  */
    if (callExit(RXCMD, RXCMDHST, (void *)&exit_parm, true))
        return true;                     /* this wasn't handled               */
    if (exit_parm.rxcmd_flags.rxfcfail)/* need to raise failure condition?  */

        *conditions = OREF_FAILURENAME;  /* raise an FAILURE condition        */
                                         /* Did we find the function??        */
    else if (exit_parm.rxcmd_flags.rxfcerr)
        /* Need to raise error condition?    */
        *conditions = OREF_ERRORNAME;    /* raise an ERROR condition          */
    // get the string value
    cmdresult = exit_parm.return_value;
    return false;                      /* this was handled                  */
}

bool  RexxActivity::callQueuePullExit(
    RexxActivation *activation,        /* sending activation                */
    RexxString **inputString)          /* returned input string             */
/******************************************************************************/
/* Function:   Calls the SysExitHandler method on the System Object to run    */
/*             the External Data queue system exit.                           */
/******************************************************************************/
{
    if (!isExitEnabled(RXMSQ))  // is the exit enabled?
    {
        return true;                     // exit out of here now
    }
    RxQueuePullParm exit_parm;           /* exit parameters                   */
    exit_parm.queue_item = OREF_NULL;
                                       /* call the handler                  */
    if (callExit(RXMSQ, RXMSQPLL, (void *)&exit_parm, false))
    {
        return true;                     /* this wasn't handled               */
    }
    *inputString = (RexxString *)exit_parm.queue_item;
    return false;                      /* this was handled                  */
}

bool  RexxActivity::callQueuePushExit(
    RexxActivation *activation,        /* sending activation                */
    RexxString *inputString,           /* string to be placed on the queue  */
    int lifo_flag)                     /* flag to indicate LIFO or FIFO     */
/******************************************************************************/
/* Function:   Calls the SysExitHandler method on the System Object to run    */
/*             the External Data queue system exit.                           */
/******************************************************************************/
{
    if (!isExitEnabled(RXMSQ))  // is the exit enabled?
    {
        return true;                     // exit out of here now
    }
    RxQueuePushParm exit_parm;         /* exit parameters                   */

    if (lifo_flag == QUEUE_LIFO)       /* LIFO queuing requested?           */
                                       /* set the flag appropriately        */
      exit_parm.rxmsq_flags.rxfmlifo = 1;
    else
                                       /* this is a FIFO request            */
      exit_parm.rxmsq_flags.rxfmlifo = 0;
                                       /* make into RXSTRING form           */
    exit_parm.queue_item = inputString;
                                       /* call the handler                  */
    return callExit(RXMSQ, RXMSQPSH, (void *)&exit_parm, false);
}

bool  RexxActivity::callQueueSizeExit(
    RexxActivation *activation,        /* sending activation                */
    RexxInteger **returnsize)          /* returned queue size               */
/******************************************************************************/
/* Function:   Calls the SysExitHandler method on the System Object to run    */
/*             the External Data queue system exit.                           */
/******************************************************************************/
{
    if (!isExitEnabled(RXMSQ))  // is the exit enabled?
    {
        return true;                     // exit out of here now
    }
    RxQueueSizeParm exit_parm;         /* exit parameters                   */
    stringsize_t  tempSize;

                                       /* call the handler                  */
    if (callExit(RXMSQ, RXMSQSIZ, (void *)&exit_parm, false))
        return true;                   /* this wasn't handled               */
                                       /* Get queue size and return it      */
    tempSize = exit_parm.rxmsq_size;   /* temporary place holder for new_integer */
    *returnsize = new_integer(tempSize);

    return false;                      /* this was handled                  */
}

bool  RexxActivity::callQueueNameExit(
    RexxActivation *activation,        /* sending activation                */
    RexxString    **inputString )      /* name of external queue            */
/******************************************************************************/
/* Function:   Calls the SysExitHandler method on the System Object to run    */
/*             the External Data queue system exit.                           */
/******************************************************************************/
{
    if (!isExitEnabled(RXMSQ))  // is the exit enabled?
    {
        return true;                     // exit out of here now
    }
    RxSetQueueNameParm exit_parm;        /* exit parameters                   */
    exit_parm.queue_name = (RexxStringObject)*inputString;
                                       /* call the handler                  */
    if (callExit(RXMSQ, RXMSQNAM, (void *)&exit_parm, false))
      return true;                     /* this wasn't handled               */
    *inputString = (RexxString *)exit_parm.queue_name;
    return false;                      /* this was handled                  */
}

bool RexxActivity::callHaltTestExit(
    RexxActivation *activation)        /* sending activation                */
/******************************************************************************/
/* Function:   Calls the SysExitHandler method on the System Object to run    */
/*             the Test Halt system exit.                                     */
/******************************************************************************/
{
    if (!isExitEnabled(RXHLT))  // is the exit enabled?
    {
        return true;                     // exit out of here now
    }
    RxHaltTestParm exit_parm;            /* exit parameters                   */

                                       /* Clear halt so as not to be sloppy */
    exit_parm.rxhlt_flags.rxfhhalt = 0;
                                       /* call the handler                  */
    if (callExit(RXHLT, RXHLTTST, (void *)&exit_parm, false))
      return true;                     /* this wasn't handled               */
                                       /* Was halt requested?               */
    if (exit_parm.rxhlt_flags.rxfhhalt == 1)
    {
                                       /* Then honor the halt request       */
      activation->halt(OREF_NULL);
    }

    return false;                      /* this was handled                  */
}

bool RexxActivity::callHaltClearExit(
    RexxActivation *activation)        /* sending activation                */
/******************************************************************************/
/* Function:   Calls the SysExitHandler method on the System Object to run    */
/*             the Clear Halt system exit.                                    */
/******************************************************************************/
{
    if (!isExitEnabled(RXHLT))  // is the exit enabled?
    {
        return true;                     // exit out of here now
    }
                                       /* call the handler                  */
    return callExit(RXHLT, RXHLTCLR, (void *)NULL, false);
}

bool  RexxActivity::callTraceTestExit(
     RexxActivation *activation,       /* sending activation                */
     bool currentsetting)              /* sending activation                */
/******************************************************************************/
/* Function:   Calls the SysExitHandler method on the System Object to run    */
/*             the Test external trace indicator system exit.                 */
/******************************************************************************/
{
    if (!isExitEnabled(RXTRC))  // is the exit enabled?
    {
        return true;                     // exit out of here now
    }
    RxTraceTestParm exit_parm;           /* exit parameters                   */

                                       /* get the exit handler              */
                                       /* Clear Trace bit before  call      */
    exit_parm.rxtrc_flags.rxftrace = 0;
                                       /* call the handler                  */
    if (callExit(RXTRC, RXTRCTST, (void *)&exit_parm, false))
      return true;                     /* this wasn't handled               */
                                       /* if not tracing, and now it is     */
                                       /* requsted                          */
    if ((currentsetting == 0) &&  (exit_parm.rxtrc_flags.rxftrace == 1)) {
                                       /* call routine to handle this       */
      activation->externalTraceOn();
    }
    else                               /* if currently tracing, and now     */
                                       /* requsted to stop                  */
      if ((currentsetting != 0) &&  (exit_parm.rxtrc_flags.rxftrace != 1)) {
                                       /* call routine to handle this       */
        activation->externalTraceOff();
      }
    return false;                    /* this was handled                  */
}



bool  RexxActivity::callDebugTestExit(
     RexxActivation *activation,       /* sending activation                */
     bool currentsetting,
     bool endStepOut_Over)
/******************************************************************************/
/* Function:   Calls the SysExitHandler method on the System Object to run    */
/*             the Test external trace indicator system exit.                 */
/******************************************************************************/
{
    if (!isExitEnabled(RXDBG))  // is the exit enabled?
    {
        return true;                     // exit out of here now
    }
    RxDebugTestParm exit_parm;           /* exit parameters                   */

    if (!activation->isTraceable())
        return true;
    if (endStepOut_Over)
        exit_parm.rxdbg_flags.rxftrace = RXDBGENDSTEP;
    else
        exit_parm.rxdbg_flags.rxftrace = (currentsetting != 0);

    exit_parm.filename = activation->getProgramName();
    if (activation->getCurrent() != OREF_NULL)
        exit_parm.line_number = activation->currentLine();
    else
        exit_parm.line_number = 0;
    exit_parm.routine_name = NULL;

    /* call the handler                  */
    if (callExit(RXDBG, RXDBGTST, (void *)&exit_parm, false))
        return true;                     /* this wasn't handled               */
                                         /* if not tracing, and now it is     */
                                         /* requsted                          */
    switch (exit_parm.rxdbg_flags.rxftrace)
    {
        case RXDBGSTEPIN: activation->externalDebugStepIn();
            return false;                    /* this was handled                  */
        case RXDBGSTEPOVER: activation->externalDebugStepOver();
            return false;                    /* this was handled                  */
        case RXDBGSTEPOUT: activation->externalDebugStepOut();
            return false;                    /* this was handled                  */
        case RXDBGLOCATELINE: return false;
        case RXDBGSTEPAGAIN: activation->externalDebugStepAgain();
            return false;                    /* this was handled                  */
        case RXDBGRECURSIVEOFF: activation->externalDebugAllOffRecursive();    /* switch off current activation and all parents */
            return false;                    /* this was handled                  */
        default:activation->externalDebugTraceOff();
            return false;                    /* this was handled                  */
    }
}


void RexxActivity::callDebugSubroutineCall(RexxActivation *activation, bool enter)
/******************************************************************************/
/* Function:   Calls the SysExitHandler method on the System Object to run    */
/*             the subroutine call debug system exit.                         */
/******************************************************************************/
{
    if (isExitEnabled(RXDBG))  // is the exit enabled?
    {
        RxDebugTestParm exit_parm;             /* exit parameters                   */
        LocationInfo  location;              /* location of the clause            */

        exit_parm.rxdbg_flags.rxftrace = 0;
        RexxString *filename = activation->getProgramName();
        exit_parm.line_number = activation->currentLine();
        activation->getCurrent()->getLocation(&location); /* get the instruction location      */

        if (activation->source)
        {
            RexxString *routine = activation->source->extract(&location);
            exit_parm.routine_name = (RexxStringObject)routine;
        }
        else
        {
            // pass a NULL string as a not-there indicator
            exit_parm.routine_name = (RexxStringObject)OREF_NULLSTRING;
        }
        // go call the exit
        callExit(RXDBG, (enter ? RXDBGENTERSUB : RXDBGLEAVESUB), (void *)&exit_parm, false);
    }
}

void RexxActivity::callDebugLineLocate(RexxActivation *activation, RexxInstruction * instr)
/******************************************************************************/
/* Function:   Calls the SysExitHandler method on the System Object to run    */
/*             the locate line debug system exit.                             */
/******************************************************************************/
{
    if (isExitEnabled(RXDBG))  // is the exit enabled?
    {
        // not every instruction is traceable...if not, ignore this
        if (!activation->isTraceable())
        {
            return;
        }

        RxDebugTestParm   exit_parm;             /* exit parameters                   */

        exit_parm.rxdbg_flags.rxftrace = 0;

        exit_parm.filename = (RexxStringObject)activation->getProgramName();

        exit_parm.line_number = instr->lineNumber;
                                           /* call the handler                  */
        callExit(RXDBG, RXDBGLOCATELINE, (void *)&exit_parm, false);
    }

}


void RexxActivity::callDebugSignal(RexxActivation *activation)
/******************************************************************************/
/* Function:   Calls the SysExitHandler method on the System Object to run    */
/*             the call stack signal system exit.                             */
/******************************************************************************/
{
    if (isExitEnabled(RXDBG))  // is the exit enabled?
    {
        RxDebugTestParm   exit_parm;       /* exit parameters                   */
        exit_parm.rxdbg_flags.rxftrace = 0;
        exit_parm.filename = OREF_NULL;
        exit_parm.line_number = 0;
        exit_parm.routine_name = OREF_NULL;
                                           /* call the handler                  */
        callExit(RXDBG, RXDBGSIGNAL, (void *)&exit_parm, false);
    }
}

void  RexxActivity::traceOutput(       /* write a line of trace information */
      RexxActivation *activation,      /* sending activation                */
      RexxString *line)                /* line to write out                 */
/******************************************************************************/
/* Function:  Write a line of trace output to the .ERROR stream               */
/******************************************************************************/
{
  RexxObject *stream;                  /* target stream                     */

  line = line->stringTrace();          /* get traceable form of this        */
                                       /* if exit declines the call         */
  if (this->callTraceExit(activation, line)) {
                                       /* get the default output stream     */
    stream = this->local->at(OREF_ERRORNAME);
                                       /* have .local set up yet?           */
    if (stream != OREF_NULL && stream != TheNilObject)
                                       /* do the lineout                    */
      stream->sendMessage(OREF_LINEOUT, line);
    else                               /* use the "real" default stream     */
      this->lineOut(line);
  }
}

void RexxActivity::sayOutput(          /* write a line of say information   */
     RexxActivation *activation,       /* sending activation                */
     RexxString *line)                 /* line to write out                 */
/******************************************************************************/
/* Function:  Write a line of SAY output to the .OUTPUT stream                */
/******************************************************************************/
{
  RexxObject *stream;                  /* target stream                     */

                                       /* if exit declines the call         */
  if (this->callSayExit(activation, line)) {
                                       /* get the default output stream     */
    stream = this->local->at(OREF_OUTPUT);
                                       /* have .local set up yet?           */
    if (stream != OREF_NULL && stream != TheNilObject)
                                       /* do the lineout                    */
      stream->sendMessage(OREF_SAY, line);
    else                               /* use the "real" default stream     */
      this->lineOut(line);
  }
}

RexxString *RexxActivity::traceInput(  /* read a line of trace input        */
     RexxActivation *activation)       /* sending activation                */
/******************************************************************************/
/* Function:  Read a line for interactive debug input                         */
/******************************************************************************/
{
  RexxString *value;                   /* returned value                    */
  RexxObject *stream;                  /* target stream                     */

                                       /* if exit declines the call         */
  if (this->callDebugPullExit(activation, &value)) {
                                       /* get the input stream              */
    stream = this->local->at(OREF_INPUT);
    if (stream != OREF_NULL) {         /* have a stream?                    */
                                       /* read from it                      */
      value = (RexxString *)stream->sendMessage(OREF_LINEIN);
                                       /* get something real?               */
      if (value == (RexxString *)TheNilObject)
        value = OREF_NULLSTRING;       /* just us a null string if not      */
    }
    else
      value = OREF_NULLSTRING;         /* default to a null string          */
  }
  return value;                        /* return value from exit            */
}

RexxString *RexxActivity::pullInput(   /* read a line of pull input         */
     RexxActivation *activation)       /* sending activation                */
/******************************************************************************/
/* Function:  Read a line for the PULL instruction                            */
/******************************************************************************/
{
  RexxString *value;                   /* returned value                    */
  RexxObject *stream;                  /* target stream object              */

                                       /* if exit declines call             */
  if (this->callPullExit(activation, &value)) {
                                       /* get the external data queue       */
    stream = this->local->at(OREF_REXXQUEUE);
    if (stream != OREF_NULL) {         /* have a data queue?                */
                                       /* pull from the queue               */
      value = (RexxString *)stream->sendMessage(OREF_PULL);
                                       /* get something real?               */
      if (value == (RexxString *)TheNilObject)
                                       /* read from the linein stream       */
        value = this->lineIn(activation);
    }
  }
  return value;                        /* return the read value             */
}

RexxObject *RexxActivity::lineOut(
    RexxString *line)                  /* line to write out                 */
/******************************************************************************/
/* Function:  Write a line out to the real default I/O stream                 */
/******************************************************************************/
{
  stringsize_t length;                 /* length to write out               */
  stringchar_t * data;                 /* data pointer                      */

  length = line->getLength();          /* get the string length and the     */
  data = line->getStringData();        /* data pointer                      */
  printf("%.*s\n",(int)length, data);  /* print it                          */
  return (RexxObject *)IntegerZero;    /* return on residual count          */
}

RexxString *RexxActivity::lineIn(
    RexxActivation *activation)        /* sending activation                */
/******************************************************************************/
/* Function:  Read a line for the PARSE LINEIN instruction                    */
/******************************************************************************/
{
  RexxObject *stream;                  /* input stream                      */
  RexxString *value;                   /* returned value                    */

                                       /* if exit declines call             */
  if (this->callPullExit(activation, &value)) {
                                       /* get the input stream              */
    stream = this->local->at(OREF_INPUT);
    if (stream != OREF_NULL) {         /* have a stream?                    */
                                       /* read from it                      */
      value = (RexxString *)stream->sendMessage(OREF_LINEIN);
                                       /* get something real?               */
      if (value == (RexxString *)TheNilObject)
        value = OREF_NULLSTRING;       /* just use a null string if not     */
    }
    else
      value = OREF_NULLSTRING;         /* default to a null string          */
  }
  return value;                        /* return value from exit            */
}

void RexxActivity::queue(              /* write a line to the queue         */
     RexxActivation *activation,       /* sending activation                */
     RexxString *line,                 /* line to write                     */
     int order)                        /* LIFO or FIFO order                */
/******************************************************************************/
/* Function:  Write a line to the external data queue                         */
/******************************************************************************/
{
  RexxObject *queue;                   /* target queue                      */

                                       /* if exit declines call             */
  if (this->callQueuePushExit(activation, line, order)) {
                                       /* get the default queue             */
    queue = this->local->at(OREF_REXXQUEUE);
    if (queue != OREF_NULL) {          /* have a data queue?                */
                                       /* pull from the queue               */
      if (order == QUEUE_LIFO)         /* push instruction?                 */
                                       /* push a line                       */
        queue->sendMessage(OREF_PUSH, (RexxObject *)line);
      else
                                       /* queue a line                      */
        queue->sendMessage(OREF_QUEUENAME, (RexxObject *)line);
     }
   }
}

void  RexxActivity::terminateActivity(ExitType e)
/******************************************************************************/
/* Function: Mark this FREE activity for termination.  Set its exit flag to 1 */
/*   and POST its run semaphore.                                              */
/******************************************************************************/
{
  this->exitType;                      /* Activity should exit          */
  this->runsem.post();                 /* let him run so he knows to exi*/
}

void *RexxActivityManager::operator new(size_t size)
/******************************************************************************/
/* Function:  Create a new library object                                     */
/******************************************************************************/
{
    return new_object(size, TheActivityManagerBehaviour);
}

// split this into createInstance()/restoreInstance() code
RexxActivityManager::RexxActivityManager()
/******************************************************************************/
/* Function:  Initialize the activity class                                   */
/******************************************************************************/
{
#ifdef FIXEDTIMERS
          /* start the control thread the first time a concurrent thread is used */
      SysInterpreter::startTimeSlice();/* Start a new timeSlice                       */
#endif
}

void RexxActivityManager::live()
{
  setUpMemoryMark
  memory_mark(this->firstWaitingActivity);
  memory_mark(this->lastWaitingActivity);
  cleanUpMemoryMark
}

void RexxActivityManager::liveGeneral()
{
  if (!RexxMemory::memoryObject.savingImage()) {
    setUpMemoryMarkGeneral
    memory_mark_general(this->firstWaitingActivity);
    memory_mark_general(this->lastWaitingActivity);
    cleanUpMemoryMarkGeneral
  }
  else {
    OrefSet(this,this->firstWaitingActivity,OREF_NULL);
    OrefSet(this,this->lastWaitingActivity,OREF_NULL);
  }
}

bool RexxActivity::halt(
     RexxString * description )        /* description to use                */
/******************************************************************************/
/* Function:   Flip on a bit in a target activities top activation            */
/******************************************************************************/
{
                                 /* get the current activation        */
    RexxActivation *activation = getCurrentContext();
                                 /* got an activation?                */
    if (activation != OREF_NULL) {
                                 /* process the halt                  */
        activation->halt(description);
        return true;             /* this actually worked              */
    }
    return false;
}


bool RexxActivity::halt(
     RexxThread   &targetThread,       /* target thread                     */
     RexxString  *description )        /* description to use                */
/******************************************************************************/
/* Function:   Flip on a bit in a target activities top activation            */
/******************************************************************************/
{
  ResourceSection lock;                // we need to synch on global resources

  bool result = false;                 /* return value                      */

  RexxActivity *activity = TheActivityManager->findActivity(targetThread);
                                       /* Activity exist for thread?        */
  if (activity != OREF_NULL) {
      result = activity->halt(description); // go halt it
  }
  return result;                       /* return the result                 */
}


/**
 * Check the stack of in-process requires loads to make sure
 * we don't have a circular dependency.
 *
 * @param program The name of the next loaded program.
 *
 * @return true if this program is already being loaded, false if it is
 *         ok to proceed.
 */
bool RexxActivity::checkRequiresStack(RexxString *program)
{
    RequiresElement *current = requiresElements;

    while (current != NULL)
    {
        if (program->isEqual(current->name))
        {
            return true;
        }
    }
    return false;
}

bool RexxActivity::setTrace(
    RexxThread   &targetThread,        /* target thread                     */
    bool  on_or_off )                 /* trace on/off flag                 */
/******************************************************************************/
/* Function:   Flip on a bit in a target activities top activation            */
/******************************************************************************/
{
  ResourceSection lock;                // we need to synch on global resources

  RexxActivity *activity = TheActivityManager->findActivity(targetThread);
                                   /* Activity exist for thread?        */
  if (activity != OREF_NULL)
  {
      return activity->setTrace(on_or_off); // go set the trace
  }
  return false;                        /* return the result                 */
}


bool RexxActivity::setTrace(
    bool  on_or_off )                 /* trace on/off flag                 */
/******************************************************************************/
/* Function:   Flip on a bit in a target activities top activation            */
/******************************************************************************/
{
                                       /* get the current activation        */
  RexxActivation *activation = this->getCurrentContext();
                                       /* got an activation?                */
  if (activation != OREF_NULL)
  {
      if (on_or_off)               /* turning this on?                  */
                                   /* turn tracing on                   */
        activation->externalTraceOn();
      else
                                   /* turn tracing off                  */
        activation->externalTraceOff();
  }
  return true;
}

void RexxActivity::yieldCurrentThread()
/******************************************************************************/
/* Function:   Signal an activation to yield control                          */
/******************************************************************************/
{
  ResourceSection lock;                // we need to synch on global resources

  if (RexxActivityManager::currentActivity != OREF_NULL) {  /* something working?                */
                                       /* get the current activation        */
    RexxActivation *activation = RexxActivityManager::currentActivity->getCurrentContext();
                                       /* got an activation?                */
    if (activation != OREF_NULL)
    {
                                       /* tell it to yield                  */
      activation->yield();
    }
  }
}

void RexxActivityManager::createInstance(RootImage *rootImage)
/******************************************************************************/
/* Function:  Create the activity class during save image processing          */
/******************************************************************************/
{
    singleInstance = new RexxActivityManager();
  // add this to the root image
    rootImage->activityManager = TheActivityManager;
}

void RexxActivityManager::restoreInstance(RootImage *rootImage)
/******************************************************************************/
/* Function:  Restore the activity class during start up                      */
/******************************************************************************/
{
    TheActivityManager = rootImage->activityManager;
}


RexxActivity *RexxActivityManager::findActivityForCurrentThread()
/******************************************************************************/
/* Function:  Locate the activity associated with a thread                    */
/******************************************************************************/
{
    RexxThread threadID;               // our thread information
    threadID.attachThread();          // initialize with thread info
    return processActivities.getActivityForThread(threadID);
}


RexxActivity *RexxActivityManager::findActivity(RexxThread &t)
/******************************************************************************/
/* Function:  Locate the activity associated with a thread                    */
/******************************************************************************/
{
                                        // pull it from the table
    return processActivities.getActivityForThread(t);
}

bool RexxActivity::isSameThread(RexxThread &t)
{
    return _thread.equals(t);
}


void RexxActivityManager::addProcessActivity(RexxActivity *activity)
/******************************************************************************/
/* Function:  Add an activity to the thread-to_process mapping table          */
/******************************************************************************/
{
                                       /* Add new activity to localacts     */
    processActivities.add(activity);
}

void RexxActivityManager::removeProcessActivity(RexxActivity *activity)
/******************************************************************************/
/* Function:  Locate the activity associated with a thread                    */
/******************************************************************************/
{
    processActivities.remove(activity);
}


stringsize_t RexxActivity::currentDigits()
/******************************************************************************/
/* Function:  Locate the activity associated with a thread                    */
/******************************************************************************/
{
  /* give me the numeric digits settings of the current actitity       */

    RexxActivity *activity = TheActivityManager->findActivityForCurrentThread();
    if (activity != OREF_NULL)
    {
        RexxActivation *activation = activity->getCurrentContext();
        if (activation != OREF_NULL)
        {
            return activation->digits();
        }
        return activation->digits();
    }
    return Numerics::DEFAULT_DIGITS;
}

RexxActivity *RexxActivityManager::attachActivity(InterpreterInstance *instance)
/******************************************************************************/
/* Function:  Get an activity for the current thread.  ne) for the thread     */
/*            we are about to enter kernel to send a message.  We return      */
/*            the activity object to be used to send the message.             */
/*            The routine requests kernel access on the new activity before   */
/*            returning.                                                      */
/******************************************************************************/
{
    RexxInterpreter::lockInterpreter();   // get the interpreter lock
                                           /* have more pools been added since  */
                                           /* we left the kernel ?              */
#ifdef SHARED
    // have the memory object sync pools if we're going cross process.
    RexxMemory::memoryObject.syncMemoryPools();
#endif
    /* Get a new activity object.        */
    RexxActivity *activityObject = new RexxActivity(false, SysThread::MEDIUM_PRIORITY, instance);
    RexxInterpreter::unlockInterpreter(); /* release kernel semaphore          */
    return activityObject;               /* Return the activity for thread    */
}


RexxActivity *RexxActivityManager::createActivity(InterpreterInstance *instance)
/******************************************************************************/
/* Function:  Create a new activity and associate with the current thread     */
/*            and the specified interpreter instance.                         */
/*            The routine requests kernel access on the new activity before   */
/*            returning.                                                      */
/******************************************************************************/
{
    RexxInterpreter::lockInterpreter();   // get the interpreter lock
#ifdef SHARED
    // have the memory object sync pools if we're going cross process.
    RexxMemory::memoryObject.syncMemoryPools();
#endif
    // get a new activity and associate with this instance
    RexxActivity *activityObject = new RexxActivity(false, SysThread::MEDIUM_PRIORITY, instance);
    RexxInterpreter::unlockInterpreter(); /* release kernel semaphore          */
                                       /* Reacquire kernel semaphore on new */
                                       /*  activity.                        */
    activityObject->requestAccess();
    return activityObject;
}

wholenumber_t RexxActivity::messageSend(
    RexxObject      *receiver,         /* target object                     */
    RexxString      *msgname,          /* name of the message to process    */
    size_t           count,            /* count of arguments                */
    RexxObject     **arguments,        /* array of arguments                */
    ProtectedObject &result )          /* message result                    */
/******************************************************************************/
/* Function:    send a message (with message lookup) to an object.  This      */
/*              method will do any needed activity setup before hand.         */
/******************************************************************************/
{
  wholenumber_t rc;                    /* message return code               */
  rc = 0;                              /* default to a clean return         */
  result = OREF_NULL;                  /* default to no return value        */
  {
      SysSignalBlock exreg(this);          // register signal handlers
      try
      {
                                           /* issue a straight message send     */
          receiver->messageSend(this, msgname, count, arguments, result);

      }
      catch (ActivityException)
      {
          rc = this->error();                /* do error cleanup                  */
      }
  }
  return rc;                           /* return the error code             */
}

wholenumber_t RexxActivity::dispatchMethod(
    RexxObject      *receiver,         // receiving object
    DispatchMethod   method,           // invoked internal object
    void            *methodData)       // opaque message data
/******************************************************************************/
/* Function:    send a message (with message lookup) to an object.  This      */
/*              method will do any needed activity setup before hand.         */
/******************************************************************************/
{
  {
      SysSignalBlock exreg(this);          // register signal handlers
      try
      {
         // directly invoke this method
         (receiver->*((DispatchMethod)method))(this, methodData);
      }
      catch (ActivityException)
      {
          return this->error();            /* do error cleanup                  */
      }
      return 0;
  }
}

/******************************************************************************/
/* Arguments:  System Exit name                                               */
/*             System Exit function code                                      */
/*             System Exit subfunction code                                   */
/*             Exit specific parm buffer                                      */
/*                                                                            */
/* Returned:   Return code (one of) -                                         */
/*               RXEXIT_HANDLED (exit ran and handled task)                   */
/*               RXEXIT_NOT_HANDLED (exit ran and declined task)              */
/*               RXEXIT_RAISE_ERROR (exit didn't run or raised an error)      */
/*                                                                            */
/* Notes:      Inovkes a system exit routine.                                 */
/******************************************************************************/
bool RexxActivity::callExit(
  int   function,                      /* major function                      */
  int   subfunction,                   /* minor exit function                 */
  void *exitbuffer,                    /* exit specific arguments             */
  bool  enable )                       /* enable variable pool                */
{
  int   rc;                            /* exit return code                    */

  rc = nestedInfo.exits[function].call(this, function, subfunction, exitbuffer);

                                       /* got an error case?                  */
  if (rc == RXEXIT_RAISE_ERROR || rc < 0) {
    if (function == RXSIO) {           /* this the I/O function?              */
                                       /* disable the I/O exit from here to   */
                                       /* prevent recursive error conditions  */
      nestedInfo.exits[RXSIO].disable();
    }
    if (function != RXTER)             /* not the termination exit?           */
                                       /* go raise an error                   */
      reportException(Error_System_service);
  }

  return rc == RXEXIT_HANDLED;         /* Did exit handle task?               */
}


void RexxActivityManager::addWaitingActivity(
    RexxActivity *newActivity,         /* new activity to add to the queue  */
    bool          release )            /* need to release the run semaphore */
/******************************************************************************/
/* Function:  Add an activity to the round robin wait queue                   */
/******************************************************************************/
{
    {
        ResourceSection lock;          // synchronize on resource tables
        // NOTE:  The following assignments intentionally do not use OrefSet
        // because we do have yet have kernel access, we can't allow memory to
        // allocate a new counter object for this.  That would lead to memory
        // corruption and unpredictable traps

        // if nobody is waiting currently, we just insert this as both the
        // first and last waiting elememt
        if (this->firstWaitingActivity == OREF_NULL)
        {
                                             /* this is the head of the chain     */
          this->firstWaitingActivity = newActivity;
                                             /* and the tail                      */
          this->lastWaitingActivity = newActivity;
        }
        // somebody got here first, so move to the end of the line
        else
        {
            // chain up with the rest of the waiting activities
            this->lastWaitingActivity->setNextWaitingActivity(newActivity);
            this->lastWaitingActivity = newActivity;
            // clear the run semaphore so the other activity has something
            // to wait on and test against
            newActivity->clearWait();

        }
    }

    // if the current semaphore owner is the one waiting, we need to release
    // the execution lock so somebody else can get it
    if (release)
    {
        RexxInterpreter::unlockInterpreter();
    }
    // now yield control, and wait until we get back to the head of the list

    newActivity->_thread.yield();
    newActivity->waitKernel();

    // now request the execution lock again
    RexxInterpreter::lockInterpreter();

    {
        ResourceSection lock;                // lock resources in this section again

        // this->firstWaitingActivity will be released, so set first to next of first
        if (this->firstWaitingActivity != OREF_NULL)
        {
            this->firstWaitingActivity = this->firstWaitingActivity->nextWaitingActivity;
            // have we removed the last activity?
            if (this->firstWaitingActivity == OREF_NULL)
            {
                // make sure we clear the end of the queue
                this->lastWaitingActivity = OREF_NULL;
            }
            else
            {
                // ok, this guy gets to wake up and wait for the
                // execution lock
                this->firstWaitingActivity->postRelease();
            }
        }
        // if we are here, newActivity must have been this->firstWaitingActivity
        // sometime before and therefore we can set next pointer to NULL
        // without disturbing the linked list

        newActivity->setNextWaitingActivity(OREF_NULL);
    }

    // we are the new current activity, so set it so and also update
    // the current settings pointer
    currentActivity = newActivity;
    RexxInterpreter::currentSettings = newActivity->settings;
#ifdef SHARED
    // have the memory object sync pools if we're going cross process.
    RexxMemory::memoryObject.syncMemoryPools();
#endif
}

/**
 * Retrieve a call context associated with this thread.  This
 * is just delegated to the current native API context
 * activation, which will either return the context pointer
 * or NULL if this is not an appropriate time for the call.
 *
 * @return A call context used for API reentries, or NULL if this is
 *         an inappopriate time for the call.
 */
RexxCallContext *RexxActivity::getCurrentCallContext()
{
    return currentApiContext->getCurrentCallContext();
}


/**
 * Add an object to the global references table.
 *
 * @param o      The added object.
 */
void RexxActivity::addGlobalReference(RexxObject *o)
{
    interpreter->addGlobalReference(o);
}

/**
 * Remove the global reference protection from an object.
 *
 * @param o      The protected object.
 */
void RexxActivity::removeGlobalReference(RexxObject *o)
{
    interpreter->removeGlobalReference(o);
}

/**
 * Resolve a class object from the environment chain.
 *
 * @param name   The required class name.
 *
 * @return A resolved class object.  Returns OREF_NULL if not located.
 */
RexxClass *RexxActivity::resolveClass(RexxString *name)
{
    RexxClass *classObject = (RexxClass *)getFromLocalEnvironment(name);
    if (classObject == OREF_NULL)      /* still not found?                  */
    {
        classObject = (RexxClass *)RexxInterpreter::getEnvironment(name);
    }
    // return the object;
    return classObject;
}

/**
 * Complete inittialization of the inteface functions to fill
 * in the commonly used object references.
 */
void RexxActivity::initializeInterfaces()
{
    threadContextFunctions.RexxNil = TheNilObject;
    threadContextFunctions.RexxTrue = TheTrueObject;
    threadContextFunctions.RexxFalse = TheFalseObject;
    threadContextFunctions.RexxNullString = OREF_NULLSTRING;
}


/**
 * Run a method as a program.
 *
 * @param method    The source method.
 * @param name      The name we're running under.
 * @param callType  The type of call.
 * @param arguments The program arguments.
 * @param argcount  The number of arguments.
 * @param result    The returned result object.
 */
void RexxActivity::runProgram(RexxMethod *method, RexxString *name, RexxString *callType, RexxString *env, RexxObject **arguments, size_t argCount, ProtectedObject &result)
{
    /* go run the method                 */
    method->callTopLevelProgram(this, name, arguments, argCount, callType, env, result);
}

/**
 * Get the default command environement defined for this instance.
 *
 * @return The string value of the default command environment.
 */
RexxString *RexxActivity::getDefaultCommandEnvironment()
{
    return interpreter->getDefaultCommandEnvironment();
}
