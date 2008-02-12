/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 1995, 2004 IBM Corporation. All rights reserved.             */
/* Copyright (c) 2005-2006 Rexx Language Association. All rights reserved.    */
/*                                                                            */
/* This program and the accompanying materials are made available under       */
/* the terms of the Common Public License v1.0 which accompanies this         */
/* distribution. A copy is also available at the following address:           */
/* http://www.oorexx.org/license.html                          */
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
/* REXX Kernel                                               MethodClass.c    */
/*                                                                            */
/* Primitive Method Class                                                     */
/*                                                                            */
/******************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "RexxCore.h"
#include "StringClass.hpp"
#include "ArrayClass.hpp"
#include "RexxCode.hpp"
#include "RexxNativeCode.hpp"
#include "RexxActivity.hpp"
#include "RexxActivation.hpp"
#include "RexxNativeActivation.hpp"
#include "MethodClass.hpp"
#include "SourceFile.hpp"
#include "DirectoryClass.hpp"
#include "ProtectedObject.hpp"
#include "BufferClass.hpp"
#include "RexxInternalApis.h"
#include "RoutineClass.hpp"
#include <ctype.h>


// singleton class instance
RexxClass *RexxMethod::classInstance = OREF_NULL;

RexxMethod::RexxMethod(BaseCode *codeObj)
/******************************************************************************/
/* Function:  Initialize a method object                                      */
/******************************************************************************/
{
    this->clearObject();                 /* start out fresh                   */
    this->methodFlags = 0;               /* clear all of the flags            */
    OrefSet(this, this->code, codeObj);  /* store the code                    */
}

RexxMethod::RexxMethod(RexxSource *source)
/******************************************************************************/
/* Function:  Initialize a method object                                      */
/******************************************************************************/
{
    this->clearObject();                 /* start out fresh                   */
    this->methodFlags = 0;               /* clear all of the flags            */
    OrefSet(this, this->code, source->generateCode());  /* store the code                    */
}

void RexxMethod::live(size_t liveMark)
/******************************************************************************/
/* Function:  Normal garbage collection live marking                          */
/******************************************************************************/
{
    memory_mark(this->scope);
    memory_mark(this->code);
    memory_mark(this->objectVariables);
}

void RexxMethod::liveGeneral(int reason)
/******************************************************************************/
/* Function:  Generalized object marking                                      */
/******************************************************************************/
{
    memory_mark_general(this->scope);
    memory_mark_general(this->code);
    memory_mark_general(this->objectVariables);
}

void RexxMethod::flatten(RexxEnvelope *envelope)
/******************************************************************************/
/* Function:  Flatten an object                                               */
/******************************************************************************/
{
  setUpFlatten(RexxMethod)

   flatten_reference(newThis->scope, envelope);
   flatten_reference(newThis->code, envelope);
   flatten_reference(newThis->objectVariables, envelope);

  cleanUpFlatten
}


void RexxMethod::run(
    RexxActivity *activity,            /* activity running under            */
    RexxObject *receiver,              /* object receiving the message      */
    RexxString *msgname,               /* message to be run                 */
    size_t count,                      /* count of arguments                */
    RexxObject **argPtr,               /* arguments to the method           */
    ProtectedObject &result)           // the returned result
/******************************************************************************/
/* Function:  Run a method on an object                                       */
/******************************************************************************/
{
    ProtectedObject p(this);           // belt-and-braces to make sure this is protected
    // save this as the most recently executed method
    ActivityManager::currentActivity->setLastMethod(msgname, this);
    // just forward this to the code object
    code->run(activity, this, receiver, msgname, count, argPtr, result);
}


RexxMethod *RexxMethod::newScope(
    RexxClass  *_scope)                 /* new method scope                  */
/******************************************************************************/
/* Function:  Create a new method with a given scope                          */
/******************************************************************************/
{
  RexxMethod *newMethod;               /* the copied method                 */

  if (this->scope == OREF_NULL) {      /* nothing set yet?                  */
    OrefSet(this, this->scope, _scope); /* just set it directly              */
    return this;                       /* and pass back unchanged           */
  }
  else {
                                       /* copy the method                   */
    newMethod= (RexxMethod *)this->copy();
                                       /* give the method the new scope     */
    OrefSet(newMethod, newMethod->scope, _scope);
    return newMethod;                  /* and return it                     */
  }
}


RexxObject *RexxMethod::setSecurityManager(
    RexxObject *manager)               /* supplied security manager         */
/******************************************************************************/
/* Function:  Associate a security manager with a method's source             */
/******************************************************************************/
{
    return code->setSecurityManager(manager);
}

void RexxMethod::setScope(
    RexxClass  *_scope)                /* scope for the method              */
/******************************************************************************/
/* Function:  Set a scope for a method without making a copy of the method    */
/*            object.                                                         */
/******************************************************************************/
{
  OrefSet(this, this->scope, _scope);  /* just set the scope                */
}

RexxObject *RexxMethod::setUnGuardedRexx()
/******************************************************************************/
/* Function:  Flag a method as being an unguarded method                      */
/******************************************************************************/
{
  this->setUnGuarded();                /* turn on the UNGUARDED state     */
  return OREF_NULL;                    /* return nothing                    */
}

RexxObject *RexxMethod::setGuardedRexx( )
/******************************************************************************/
/* Function:  Flag a method as being a guarded method (the default)           */
/******************************************************************************/
{
  this->setGuarded();                  /* flip back to the GUARDED state    */
  return OREF_NULL;                    /* return nothing                    */
}

RexxObject *RexxMethod::setPrivateRexx()
/******************************************************************************/
/* Function:  Flag a method as being a private method                         */
/******************************************************************************/
{
  this->setPrivate();                  /* turn on the PRIVATE flag          */
  return OREF_NULL;                    /* always return nothing             */
}

RexxObject *RexxMethod::setProtectedRexx()
/******************************************************************************/
/* Function:  Flag a method as being a private method                         */
/******************************************************************************/
{
  this->setProtected();                /* turn on the PROTECTED flag        */
  return OREF_NULL;                    /* always return nothing             */
}

/**
 * Return the Guarded setting for a method object.
 *
 * @return .true if the method is guarded.  .false otherwise.
 */
RexxObject *RexxMethod::isGuardedRexx( )
{
    return isGuarded() ? TheTrueObject : TheFalseObject;
}

/**
 * Return the Private setting for a method object.
 *
 * @return .true if the method is private.  .false otherwise.
 */
RexxObject *RexxMethod::isPrivateRexx( )
{
    return isPrivate() ? TheTrueObject : TheFalseObject;
}

/**
 * Return the Protected setting for a method object.
 *
 * @return .true if the method is protected.  .false otherwise.
 */
RexxObject *RexxMethod::isProtectedRexx( )
{
    return isProtected() ? TheTrueObject : TheFalseObject;
}

RexxSmartBuffer *RexxMethod::saveMethod()
/******************************************************************************/
/* Function: Flatten translated method into a buffer for storage into EA's etc*/
/******************************************************************************/
{
  RexxEnvelope *envelope;              /* envelope for flattening           */
                                       /* Get new envelope object           */
  envelope = new RexxEnvelope;
  ProtectedObject p(envelope);
                                       /* now pack up the envelope for      */
                                       /* saving.                           */
  envelope->pack(this);
  return envelope->getBuffer();        /* return the buffer                 */
}

void *RexxMethod::operator new (size_t size)
/******************************************************************************/
/* Function:  create a new method instance                                    */
/******************************************************************************/
{
                                         /* get a new method object           */
    RexxObject *newObj = new_object(size, T_Method);
    return newObj;                       /* Initialize this new method        */
}


RexxMethod *RexxMethod::newRexxMethod(
    RexxSource *source,                /* source object for the method      */
    RexxClass  *scope)                 /* scope to use                      */
/******************************************************************************/
/* Function:  Convert a new source object to a method with the given scope    */
/******************************************************************************/
{


                                       /* create a new method object        */
  RexxMethod *newMethod = new RexxMethod(source);
  if (scope != OREF_NULL)              /* given a scope too?                */
  {
      newMethod->setScope(scope);        /* set the scope                     */
  }
  return newMethod;                    /* return the new method object      */
}


/**
 * Static method used for constructing new method objects in
 * various contexts (such as the define method on the Class class).
 *
 * @param pgmname  The name of the method we're creating.
 * @param source   The method source (either a string or an array).
 * @param position The position used for reporting errors.  This is the position
 *                 of the source argument for the calling method context.
 * @param parentScope
 *                 The parent code we inherit routine scope from.  This overrides
 *                 anything that might be defined in single method code.
 *
 * @return The constructed method object.
 */
RexxMethod *RexxMethod::newMethodObject(RexxString *pgmname, RexxObject *source, RexxObject *position, RexxSource *parentSource)
{
    // request this as an array.  If not convertable, then we'll use it as a string
    RexxArray *newSourceArray = source->requestArray();
    /* couldn't convert?                 */
    if (newSourceArray == (RexxArray *)TheNilObject)
    {
        /* get the string representation     */
        RexxString *sourceString = source->makeString();
        /* got back .nil?                    */
        if (sourceString == (RexxString *)TheNilObject)
        {
            /* raise an error                    */
            reportException(Error_Incorrect_method_no_method, position);
        }
        /* wrap an array around the value    */
        newSourceArray = new_array(sourceString);
    }
    else                                 /* have an array, make sure all      */
    {
        /* is it single dimensional?         */
        if (newSourceArray->getDimension() != 1)
        {
            /* raise an error                    */
            reportException(Error_Incorrect_method_noarray, position);
        }
        /*  element are strings.             */
        /* Make a source array safe.         */
        ProtectedObject p(newSourceArray);
        /* Make sure all elements in array   */
        for (size_t counter = 1; counter <= newSourceArray->size(); counter++)
        {
            /* Get element as string object      */
            RexxString *sourceString = newSourceArray ->get(counter)->makeString();
            /* Did it convert?                   */
            if (sourceString == (RexxString *)TheNilObject)
            {
                /* and report the error.             */
                reportException(Error_Incorrect_method_nostring_inarray, IntegerTwo);
            }
            else
            {
                /* itsa string add to source array   */
                newSourceArray ->put(sourceString, counter);
            }
        }
    }

    /* create a source object            */
    RexxSource *newSource = new RexxSource (pgmname, newSourceArray);

    ProtectedObject p(newSource);
    RexxMethod *result = RexxMethod::newRexxMethod(newSource, OREF_NULL);

    // if we've been provided with a scope, use it
    if (parentSource == OREF_NULL)
    {
        // see if we have an active context and use the current source as the basis for the lookup
        RexxActivation *currentContext = ActivityManager::currentActivity->getCurrentRexxFrame();
        if (currentContext != OREF_NULL)
        {
            parentSource = currentContext->getSource();
        }
    }

    // if there is a parent source, then merge in the scope information
    if (parentSource != OREF_NULL)
    {
        newSource->inheritSourceContext(parentSource);
    }

    return result;
}


RexxMethod *RexxMethod::newRexx(
    RexxObject **init_args,            /* subclass init arguments           */
    size_t       argCount)             /* number of arguments passed        */
/******************************************************************************/
/* Function:  Create a new method from REXX code contained in a string or an  */
/*            array                                                           */
/******************************************************************************/
{
    RexxObject *pgmname;                 /* method name                       */
    RexxObject *source;                  /* Array or string object            */
    RexxMethod *newMethod;               /* newly created method object       */
    RexxObject *option = OREF_NULL;
    size_t initCount = 0;                /* count of arguments we pass along  */

                                         /* break up the arguments            */

    process_new_args(init_args, argCount, &init_args, &initCount, 2, (RexxObject **)&pgmname, (RexxObject **)&source);
    /* get the method name as a string   */
    RexxString *nameString = REQUIRED_STRING(pgmname, ARG_ONE);
    required_arg(source, TWO);           /* make sure we have the second too  */

    RexxSource *sourceContext = OREF_NULL;
    // retrieve extra parameter if exists
    if (initCount != 0)
    {
        process_new_args(init_args, initCount, &init_args, &initCount, 1, (RexxObject **)&option, NULL);
        if (isOfClass(Method, option))
        {
            sourceContext = ((RexxMethod *)option)->getSourceObject();
        }
        else if (isOfClass(Routine, option))
        {
            sourceContext = ((RoutineClass *)option)->getSourceObject();
        }
        else
        {
            // this must be a string (or convertable) and have a specific value
            option = option->requestString();
            if (option == TheNilObject)
            {
                reportException(Error_Incorrect_method_argType, IntegerThree, "Method/String object");
            }
            // default given? set option to NULL (see code below)
            if (!((RexxString *)option)->strICompare("PROGRAMSCOPE"))
            {
                reportException(Error_Incorrect_call_list, "NEW", IntegerThree, "\"PROGRAMSCOPE\", Method object", option);
            }
        }
    }
    /* go create a method                */
    newMethod = RexxMethod::newMethodObject(nameString, source, IntegerTwo, sourceContext);
    ProtectedObject p(newMethod);
    /* Give new object its behaviour     */
    newMethod->setBehaviour(((RexxClass *)this)->getInstanceBehaviour());
    if (((RexxClass *)this)->hasUninitDefined())          /* does object have an UNINT method  */
    {
        newMethod->hasUninit();              /* Make sure everyone is notified.   */
    }
    /* now send an INIT message          */
    newMethod->sendMessage(OREF_INIT, init_args, initCount);
    return newMethod;                    /* return the new method             */
}


RexxMethod *RexxMethod::newFileRexx(RexxString *filename)
/******************************************************************************/
/* Function:  Create a method from a fully resolved file name                 */
/******************************************************************************/
{
                                       /* get the method name as a string   */
  filename = REQUIRED_STRING(filename, ARG_ONE);
                                       /* create a source object            */
  RexxSource *source = RexxSource::classNewFile(filename);
  ProtectedObject p(source);
                                       /* finish up processing of this      */
  RexxMethod * newMethod = newRexxMethod(source, (RexxClass *)TheNilObject);
  ProtectedObject p2(newMethod);
                                       /* Give new object its behaviour     */
  newMethod->setBehaviour(((RexxClass *)this)->getInstanceBehaviour());
   if (((RexxClass *)this)->hasUninitDefined())    /* does object have an UNINT method  */
   {
     newMethod->hasUninit();           /* Make sure everyone is notified.   */
   }
                                       /* now send an INIT message          */
  newMethod->sendMessage(OREF_INIT);
  return newMethod;
}


RexxMethod *RexxMethod::newRexxBuffer(
      RexxString *pgmname,             /* file name to process              */
      RexxBuffer *source,              /* String or buffer with source      */
      RexxClass  *scope)               /* Scope for this method             */
/******************************************************************************/
/* Function:  Build a new method object from buffered REXX source             */
/******************************************************************************/
{
  if (source == OREF_NULL)             /* didn't get source?                */
  {
                                       /* raise an error                    */
      reportException(Error_Incorrect_method_noarg, IntegerTwo);
  }
                                       /* create a source object            */
  RexxSource *newSource = RexxSource::classNewBuffered(pgmname, source);
  // we need to protect this source object until parsing is complete
  ProtectedObject p(newSource);
                                       /* now complete method creation      */
  return newRexxMethod(newSource, scope);
}


RexxMethod *RexxMethod::restore(
    RexxBuffer *buffer,                /* buffer containing the method      */
    char *startPointer)                /* first character of the method     */
/******************************************************************************/
/* Function: Unflatten a translated method.  Passed a buffer object containing*/
/*           the method                                                       */
/******************************************************************************/
{
                                       /* Get new envelope object           */
  RexxEnvelope *envelope  = new RexxEnvelope;
  ProtectedObject p(envelope);
                                       /* now puff up the method object     */
  envelope->puff(buffer, startPointer);
                                       /* The receiver object is an envelope*/
                                       /* whose receiver is the actual      */
                                       /* method object we're restoring     */
  return (RexxMethod *)envelope->getReceiver();
}


RexxMethod *RexxMethod::newFile(
    RexxString *filename)              /* name of the target file           */
/******************************************************************************/
/* Function:  Create a method from a fully resolved file name                 */
/******************************************************************************/
{
                                       /* create a source object            */
  RexxSource *source = RexxSource::classNewFile(filename);
  ProtectedObject p(source);
                                       /* finish up processing of this      */
  return newRexxMethod(source, (RexxClass *)TheNilObject);
}



/**
 * Run this code as a method invocation.
 *
 * @param activity  The current activity.
 * @param method    The method we're invoking.
 * @param receiver  The method target object.
 * @param msgname   The name the method was invoked under.
 * @param argCount  The count of arguments.
 * @param arguments The argument pointer.
 * @param result    The returned result.
 */
void BaseCode::run(RexxActivity *activity, RexxMethod *method, RexxObject *receiver, RexxString *msgname, size_t argCount, RexxObject **arguments, ProtectedObject &result)
{
    // The subcasses decide which of run and call are allowed
    reportException(Error_Interpretation);
}


/**
 * Invoke a code element as a call target.  This form is generally
 * only used for calls from Rexx code to Rexx code or for top level
 * program invocation.
 *
 * @param activity  The activity we're running under.
 * @param msgname   The name of the program or name used to invoke the routine.
 * @param arguments The arguments to the method.
 * @param argcount  The count of arguments.
 * @param ct        The call context.
 * @param env       The current address environment.
 * @param context   The type of call being made (program call, internal call, interpret,
 *                  etc.)
 * @param result    The returned result.
 */
void BaseCode::call(RexxActivity *activity, RoutineClass *routine, RexxString *msgname, RexxObject **arguments, size_t argcount, RexxString *ct, RexxString *env, int context, ProtectedObject &result)
{
    // the default for this is the simplified call.   This is used by Rexx code to make calls to
    // both Rexx programs and native routines, so the polymorphism simplifies the processing.
    call(activity, routine, msgname, arguments, argcount, result);
}


/**
 * Simplified call form used for calling from Rexx code to native code.
 *
 * @param activity  The current activity.
 * @param msgname   The name of the call.
 * @param arguments the call arguments.
 * @param argcount  The count of arguments.
 * @param result    The returned result.
 */
void BaseCode::call(RexxActivity *activity, RoutineClass *routine, RexxString *msgname, RexxObject **arguments, size_t argcount, ProtectedObject &result)
{
    // The subcasses decide which of run and call are allowed
    reportException(Error_Interpretation);
}


/**
 * Return source informaton for a BaseCode object.  If not
 * representing an element in a source file, this returns
 * an empty array.
 *
 * @return A null array.
 */
RexxArray *BaseCode::getSource()
{
                                       /* this is always a null array       */
    return (RexxArray *)TheNullArray->copy();
}


/**
 * Set the security manager in the code source context.
 *
 * @param manager The new security manager.
 *
 * @return Returns true if the manager could be set.  Non-Rexx code objects
 *         just return false unconditionally.
 */
RexxObject *BaseCode::setSecurityManager(RexxObject *manager)
/******************************************************************************/
/* Function:  Associate a security manager with a method's source             */
/******************************************************************************/
{
    // the default is just to return a failure
    return TheFalseObject;
}


/**
 * Retrieve the source object associated with a code object.
 *
 * @return
 */
RexxSource *BaseCode::getSourceObject()
{
    return OREF_NULL;
}
