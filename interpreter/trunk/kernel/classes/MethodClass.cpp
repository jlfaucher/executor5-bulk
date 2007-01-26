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
/* REXX Kernel                                                  MethodClass.c    */
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
#include "Interpreter.hpp"
#include "FunctionActivator.hpp"
#include <ctype.h>


RexxClass *RexxMethod::classInstance = OREF_NULL;    // the singleton class instance

RexxMethod::RexxMethod(
    RexxCode *code)                    /* the associated Rexx code          */
/******************************************************************************/
/* Function:  Initialize a method object                                      */
/******************************************************************************/
{

  ClearObject(this);                   /* start out fresh                   */
  this->setFlags(0);                   /* clear all of the flags            */
                                       /* get the argument information      */
  OrefSet(this, this->code, code);     /* store the code                    */
  this->setRexxMethod();               /* turn on the REXX flag             */
  OrefSet(this, source, code->getSource());
}

RexxMethod::RexxMethod(
    size_t index,                      // the index of the entry point
    PCPPM entry,                       /* method entry point                */
    size_t argCount)                   // count of arguments
/******************************************************************************/
/* Function:  Initialize a method object                                      */
/******************************************************************************/
{

  ClearObject(this);                   /* start out fresh                   */
  this->setFlags(0);                   /* clear all of the flags            */
  this->setMethnum(index);             /* save the method code number       */
  this->cppEntry = entry;              /* set the entry point               */
  this->setArguments(argCount);        /* and the arguments                 */
}

RexxMethod::RexxMethod(
    RexxNativeCode *code,              /* associated method code            */
    RexxClass      *scope)             // the scope this belongs to (if any)
/******************************************************************************/
/* Function:  Initialize a method object                                      */
/******************************************************************************/
{
  ClearObject(this);                   /* start out fresh                   */
  this->setFlags(0);                   /* clear all of the flags            */
  this->setMethnum(0);                 /* no method number here,            */
  this->cppEntry = NULL,               /* set the entry point               */
  this->setArguments(0);               /* no defined arguments for native   */
                                       /* get the argument information      */
                                       /* store the code target object      */
  OrefSet(this, this->code, (RexxInternalObject *)code);
  this->setNativeMethod();             /* this is a native method           */
  this->setScope(scope);               // and set the owning scope, if any
}

void RexxMethod::live()
/******************************************************************************/
/* Function:  Normal garbage collection live marking                          */
/******************************************************************************/
{
  setUpMemoryMark
  memory_mark(this->source);
  memory_mark(this->scope);
  memory_mark(this->code);
  memory_mark(this->objectVariables);
  cleanUpMemoryMark
}

void RexxMethod::liveGeneral()
/******************************************************************************/
/* Function:  Generalized object marking                                      */
/******************************************************************************/
{
  setUpMemoryMarkGeneral
  memory_mark_general(this->source);
  memory_mark_general(this->scope);
  memory_mark_general(this->code);
  memory_mark_general(this->objectVariables);
  cleanUpMemoryMarkGeneral
                                       /* part of the saved image?          */
  if (RexxMemory::memoryObject.restoringImage()) { /* restoring the image?              */
    this->setInternal();               /* mark as an image method           */
                                       /* reset the method entry point      */
    this->cppEntry = exportedMethods[this->methnum()];
  }
}

void RexxMethod::flatten(RexxEnvelope *envelope)
/******************************************************************************/
/* Function:  Flatten an object                                               */
/******************************************************************************/
{
  setUpFlatten(RexxMethod)

   flatten_reference(newThis->source, envelope);
  flatten_reference(newThis->scope, envelope);
   flatten_reference(newThis->code, envelope);
   flatten_reference(newThis->objectVariables, envelope);

  cleanUpFlatten
}

RexxObject * RexxMethod::unflatten(RexxEnvelope *envelope)
/******************************************************************************/
/* Function:  unflatten an object                                             */
/******************************************************************************/
{
                                       /* Does the entry have a value?      */
                                       /* if not then we haven't unflattened*/
  if (this->code == OREF_NULL)         /* is this a kernel method?          */
                                       /* reset the method entry point      */
    this->cppEntry = exportedMethods[this->methnum()];
  return (RexxObject *)this;           /* return ourself.                   */
}

void RexxMethod::run(
    RexxActivity *activity,            /* activity running under            */
    RexxObject *receiver,              /* object receiving the message      */
    RexxString *msgname,               /* message to be run                 */
    size_t count,                      /* count of arguments                */
    RexxObject **arguments,            /* arguments to the method           */
    ProtectedObject &result)
/******************************************************************************/
/* Function:  Run a method on an object                                       */
/******************************************************************************/
{
  size_t i;                            /* loop counter                      */
  RexxObject * argument_list[7];       /* arguments removed from the array  */
  RexxActivation *newacta;             /* newly created activation          */
  RexxNativeActivation *newNActa;      /* newly created Native activation   */
  PCPPM cppEntry;                      /* kernel method entry point         */
  RexxActivation * Parent;

  if (this->code == OREF_NULL) {       /* directly to a kernel method?      */
    cppEntry = this->cppEntry;         /* get the entry point               */
                                       /* expecting an array?               */
                                       /* expecting a pointer/count pair?   */
    if (this->arguments() == A_COUNT) {
                                       /* we can pass this right on         */
      result = (receiver->*((PCPPMC1)cppEntry))(arguments, count);
    }
    else {                             /* receiver expects fixed arguments  */
      if (count > this->arguments())   /* too many arguments?               */
        reportException(Error_Incorrect_method_maxarg, this->arguments());
      if (count < this->arguments()) { /* need to pad these out?            */
        for (i = 0; i < count; i++)    /* copy over the arguments so we     */
                                       /* don't clobber things in the caller*/
          argument_list[i] = arguments[i];
                                       /* null out any missing arguments    */
        for (i = count; i < this->arguments(); i++)
          argument_list[i] = OREF_NULL;
        arguments = &argument_list[0]; /* point at the new argument list    */
      }

      switch (this->arguments()) {     /* switch based on number of args    */

        case 0:                        /* zero                              */
          result = (receiver->*((PCPPM0)cppEntry))();
          break;

        case 1:
          result = (receiver->*((PCPPM1)cppEntry))(arguments[0]);
          break;

        case 2:
          result = (receiver->*((PCPPM2)cppEntry))(arguments[0], arguments[1]);
          break;

        case 3:
          result = (receiver->*((PCPPM3)cppEntry))(arguments[0], arguments[1], arguments[2]);
          break;

        case 4:
          result = (receiver->*((PCPPM4)cppEntry))(arguments[0], arguments[1], arguments[2], arguments[3]);
          break;

        case 5:
          result = (receiver->*((PCPPM5)cppEntry))(arguments[0], arguments[1], arguments[2],
              arguments[3], arguments[4]);
          break;

        case 6:
          result = (receiver->*((PCPPM6)cppEntry))(arguments[0], arguments[1], arguments[2],
              arguments[3], arguments[4], arguments[5]);
          break;

        case 7:
          result = (receiver->*((PCPPM7)cppEntry))(arguments[0], arguments[1], arguments[2],
              arguments[3], arguments[4], arguments[5], arguments[6]);
          break;

          default:
          reportException(Error_Interpretation);
          break;
      }
    }
    return;                            /* and return the result             */
  }
  else if (this->isRexxMethod()) {     /* written in REXX?                  */
                                       /* add to the activity stack         */
    newacta = activity->pushStackFrame();
    // initialize for a method call.
    newacta->initMethodCall(receiver, this, msgname);
    newacta->debugMethodEnter();
                                       /* run the method and return result  */
    if (!newacta->run(arguments, count, OREF_NULL, result))
    {
        // only pop this stack frame if this was not a reply return
        activity->popStackFrame(newacta);
    }

    newacta->debugMethodExit();
    activity->yield();        /* yield control now */
  }
  else {                               /* native activation                 */
                                       /* create a new native activation    */
    newNActa = new RexxNativeActivation(receiver, this, activity, msgname);
    activity->pushNativeActivation(newNActa);          /* push it on the activity stack     */
                                       /* and go run it                     */
    newNActa->run(count, arguments, result);
    // and pop of the activation now that we're complete
    activity->popNativeActivation(newNActa);
  }
}


/**
 * Call a top-level program execution.  This is a call that
 * will be the bottom of the execution stack.
 *
 * @param activity  The activity this is running under.
 * @param routineName
 *                  The name of the program.
 * @param arguments The arguments to the program.
 * @param argcount  The argument count.
 * @param calltype  The type call being made (COMMAND, SCRIPT, etc.).
 * @param environment
 *                  The initial command environment.
 * @param context   The call context type.
 * @param result    A protected object reference used to return a result.
 */
void RexxMethod::callProgram(RexxActivity *activity, RexxString *routineName, RexxObject**arguments,
    size_t argcount, RexxString *calltype, RexxString *environment, uint32_t context, ProtectedObject &result)
{
    if (!this->isRexxMethod())
    {
        // cannot make direct calls to internal methods.
        reportException(Error_Incorrect_call);
    }
    // get a new activation from the activity.
    RexxActivation *newacta = activity->pushStackFrame();

    newacta->initTopLevelCall(this, calltype, environment, context);
    /* run the method and return result  */
    newacta->run(arguments, argcount, OREF_NULL, result);

    activity->popStackFrame(newacta);
}


/**
 * Call a method as an external routine or function.
 *
 * @param activity  The activity the call's being made on.
 * @param routineName
 *                  The name this routine was called with.
 * @param arguments The arguments to the call.
 * @param argcount  The count of arguments.
 * @param calltype  The type of call (FUNCTION or ROUTINE).
 * @param environment
 *                  The inherited environment string.
 * @param result    A protected object for returning the routine result.
 */
void RexxMethod::callExternalRoutine(RexxActivity *activity, RexxString *routineName, RexxObject**arguments, size_t argcount,
    RexxString *calltype, RexxString *environment, ProtectedObject &result)
{
    if (this->isRexxMethod())
    {
        // get a new activation from the activity.
        RexxActivation *newacta = activity->pushStackFrame();

        newacta->debugMethodEnter();
        newacta->initTopLevelCall(this, calltype, environment, EXTERNALCALL);

        /* run the method and return result  */
        newacta->run(arguments, argcount, OREF_NULL, result);
        newacta->debugMethodExit();
        activity->popStackFrame(newacta);
    }
    else if (this->isNativeMethod())
    {
        callNativeMethod(activity, routineName, arguments, argcount, result);

    }
    else
    {
        // cannot make direct calls to internal methods.
        reportException(Error_Incorrect_call);
    }
}


/**
 * Call an internal routine or function.
 *
 * @param activity  The activity this is running on.
 * @param target    The target first instruction of the call (a label).
 * @param arguments The arguments to the call.
 * @param argcount  The argument count.
 * @param parent    The parent activation.
 * @param result    A protected object used for the return value.
 */
void RexxMethod::callInternalRoutine(RexxActivity *activity, RexxInstruction *target, RexxObject**arguments, size_t argcount,
    RexxActivation *parent, ProtectedObject &result)
{
    // internal routines are always in Rexx
    if (!this->isRexxMethod())
    {
        reportException(Error_Incorrect_call);
    }
    // get a new activation from the activity.
    RexxActivation *newacta = activity->pushStackFrame();

    newacta->initInternalCall(parent, this);
    newacta->debugDisableStepOver();
    parent->debugEnterSubroutine();
    /* run the method and return result  */
    newacta->run(arguments, argcount, target, result);
    parent->debugLeaveSubroutine();
    newacta->debugPassTrace2Parent(parent);
    activity->popStackFrame(newacta);
}


/**
 * Run a method created for an interpret instruction.
 *
 * @param activity The activity we're running on.
 * @param parent   The parent context of the interpret.
 * @param result   A protected object for a return result (not typical, but this
 *                 could be an interpreted return or exit.
 * @param debug    Whether this interpret is an interpret instruction or a debug pause.
 */
void RexxMethod::callInterpret(RexxActivity *activity, RexxActivation *parent, ProtectedObject &result, bool debug)
{
    // get a new activation from the activity.
    RexxActivation *newacta = activity->pushStackFrame();
    if (debug)
    {
        newacta->initDebugCall(parent, this);

    }
    else
    {
        newacta->initInterpret(parent, this);
    }
    /* run the method and return result  */
    newacta->run(NULL, 0, OREF_NULL, result);
    activity->popStackFrame(newacta);
}


/**
 * Call a native method.
 *
 * @param activity  The current activity.
 * @param routineName
 *                  The name this is called under.
 * @param arguments The routine arguments.
 * @param argcount  The count of arguments.
 * @param result    A protected object used for the return result.
 */
void RexxMethod::callNativeMethod(RexxActivity *activity, RexxString *routineName, RexxObject**arguments, size_t argcount, ProtectedObject &result)
{
    /* create a new native activation    */
    RexxNativeActivation *newNActa = new RexxNativeActivation(this, activity, routineName);
    activity->pushNativeActivation(newNActa);          /* push it on the activity stack     */
    newNActa->call((PNFUNCTION)this->nativeCode->getEntry(), argcount, arguments, result);
    // and pop of the activation now that we're complete
    activity->popNativeActivation(newNActa);
}



void RexxMethod::loadRexxProgram(RexxString *name, ProtectedObject &method)
/******************************************************************************/
/* Function:  Load a Rexx program from the file system                        */
/******************************************************************************/
{
    // first try to restore a pre-tokenized version of this
    restoreProgram(name, method);

    if ((RexxObject *)method == OREF_NULL)
    {
                                         /* go translate the image            */
        method = RexxMethod::newFile(name);
        // get the method object
        RexxMethod *methodObject = (RexxMethod *)(RexxObject *)method;
        // if we're automatically saving images, save it now
        methodObject->saveProgram(name);
    }
}

RexxMethod *RexxMethod::newScope(
    RexxClass  *scope)                 /* new method scope                  */
/******************************************************************************/
/* Function:  Create a new method with a given scope                          */
/******************************************************************************/
{
  RexxMethod *newMethod;               /* the copied method                 */

  if (this->scope == OREF_NULL) {      /* nothing set yet?                  */
    OrefSet(this, this->scope, scope); /* just set it directly              */
    return this;                       /* and pass back unchanged           */
  }
  else {
                                       /* copy the method                   */
    newMethod= (RexxMethod *)this->copy();
                                       /* give the method the new scope     */
    OrefSet(newMethod, newMethod->scope, scope);
    return newMethod;                  /* and return it                     */
  }
}

RexxArray  *RexxMethod::sourceRexx()
/******************************************************************************/
/* Function:  Return an array of source strings that represent this method    */
/******************************************************************************/
{
  if (this->isRexxMethod())            /* this written in REXX?             */
                                       /* return associated source          */
    return this->rexxCode->sourceRexx();
  else                                 /* kernel/native code            */
                                       /* this is always a null array       */
    return (RexxArray *)TheNullArray->copy();
}

RexxObject *RexxMethod::setSecurityManager(
    RexxObject *manager)               /* supplied security manager         */
/******************************************************************************/
/* Function:  Associate a security manager with a method's source             */
/******************************************************************************/
{
  if (this->isRexxMethod()) {          /* this written in REXX?             */
                                       /* return associated source          */
    this->rexxCode->u_source->setSecurityManager(manager);
    return TheTrueObject;              /* this worked ok                    */
  }
  else                                 /* kernel/native code            */
    return TheFalseObject;             /* nothing to set security on        */
}

void RexxMethod::setScope(
    RexxClass  *scope)                 /* scope for the method              */
/******************************************************************************/
/* Function:  Set a scope for a method without making a copy of the method    */
/*            object.                                                         */
/******************************************************************************/
{
  OrefSet(this, this->scope, scope);   /* just set the scope                */
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

RexxSmartBuffer *RexxMethod::saveMethod()
/******************************************************************************/
/* Function: Flatten translated method into a buffer for storage into EA's etc*/
/******************************************************************************/
{
  RexxEnvelope *envelope;              /* envelope for flattening           */
  RexxSmartBuffer   *envelopeBuffer;   /* enclosing buffer                  */

                                       /* Get new envelope object           */
  envelope  = (RexxEnvelope *)new RexxEnvelope;
  ProtectedObject p1(envelope);

                                       /* now pack up the envelope for      */
                                       /* saving.                           */
  envelope->pack(OREF_NULL, this, OREF_NULL, OREF_NULL);
                                       /* pull out the buffer               */
  envelopeBuffer = envelope->getBuffer();
  return envelopeBuffer;               /* return the buffer                 */
}


/**
 * Flatten a method into a buffer object, returning the buffer.
 *
 * @return A buffer object with the flattened method.
 */
RexxBuffer *RexxMethod::save()
{
    RexxSmartBuffer *buf = saveMethod();
    return buf->getBuffer();
}

void *RexxMethod::operator new (size_t size)
/******************************************************************************/
/* Function:  create a new method instance                                    */
/******************************************************************************/
{
    return new_object(size, TheMethodClass->getInstanceBehaviour());
}


RexxMethod *RexxMethod::newRexxMethod(
    RexxSource *source)                /* source object for the method      */
/******************************************************************************/
/* Function:  Convert a new source object to a method with the given scope    */
/******************************************************************************/
{
    // create a new method object from the source
    return source->method();
}

RexxMethod *RexxMethod::newRexxCode(
    RexxString *pgmname,               /* name of the program               */
    RexxObject *source,                /* string or array source            */
    RexxObject *position,              /* argument position                 */
    RexxObject *option)
/******************************************************************************/
/* Function:  Create a new method from REXX code contained in a string or an  */
/*            array                                                           */
/******************************************************************************/
{
  RexxArray  *newSourceArray;          /* created source object             */
  RexxSource *newSource;               /* created source object             */
  RexxString *sourceString;
  RexxMethod *result;
  size_t counter;

                                       /* request an array version          */
  newSourceArray = source->requestArray();
                                       /* couldn't convert?                 */
  if (newSourceArray == (RexxArray *)TheNilObject) {
                                       /* get the string representation     */
    sourceString = source->makeString();
                                       /* got back .nil?                    */
    if (sourceString == (RexxString *)TheNilObject)
                                       /* raise an error                    */

      reportException(Error_Incorrect_method_no_method, position);
                                       /* wrap an array around the value    */
    newSourceArray = new_array(sourceString);
  }
  else {                               /* have an array, make sure all      */
                                       /* is it single dimensional?         */
    if (newSourceArray->getDimension() != 1)
                                       /* raise an error                    */
      reportException(Error_Incorrect_method_noarray, position);
                                       /*  element are strings.             */
                                       /* Make a source array safe.         */
    ProtectedObject p1(newSourceArray);
                                       /* Make sure all elements in array   */
    for (counter = 1; counter <= newSourceArray->size(); counter++) {
                                       /* Get element as string object      */
      sourceString = newSourceArray ->get(counter)->makeString();
                                       /* Did it convert?                   */
      if (sourceString == (RexxString *)TheNilObject) {
                                       /* and report the error.             */
        reportException(Error_Incorrect_method_nostring_inarray, IntegerTwo);
      }
      else {
                                       /* itsa string add to source array   */
        newSourceArray ->put(sourceString, counter);
      }
    }
  }
                                       /* create a source object            */
  newSource = new RexxSource (pgmname, newSourceArray);
  ProtectedObject p1(newSource);
                                       /* now complete method creation      */
  if (option != OREF_NULL) {
    if (isMethod(option)) {
      result = newRexxMethod(newSource);
      result->code->u_source->routines = ((RexxMethod*) option)->code->u_source->routines;
      result->code->u_source->public_routines = ((RexxMethod*) option)->code->u_source->public_routines;
    } else {
      if (!isString(option))
        reportException(Error_Incorrect_method_argType, IntegerThree, new_string("Method/String object") );
      else {
        // default given? set option to NULL (see code below)
        if (!stricmp("PROGRAMSCOPE",(char *)((RexxString*) option)->getStringData()))
          option = NULL;
        else
          reportException(Error_Incorrect_call_list, new_string("NEW"), IntegerThree, new_string("\"PROGRAMSCOPE\", Method object"), option);
      }
    }
  }
  // option NULL => set default: Program Scope.
  if (option == NULL) {
    result = newRexxMethod(newSource);
    // new default: insert program scope into method object
    result->code->u_source->routines = RexxActivityManager::currentActivity->getCurrentContext()->source->routines;
    result->code->u_source->public_routines = RexxActivityManager::currentActivity->getCurrentContext()->source->public_routines;
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
  RexxString *pgmname;                 /* method name                       */
  RexxObject *source;                  /* Array or string object            */
  RexxMethod *newMethod;               /* newly created method object       */
  RexxObject *option = OREF_NULL;
  size_t initCount = 0;                /* count of arguments we pass along  */

                                       /* break up the arguments            */

  processNewArgs(init_args, argCount, &init_args, &initCount, 2, (RexxObject **)&pgmname, (RexxObject **)&source);
                                       /* get the method name as a string   */
  pgmname = REQUIRED_STRING(pgmname, ARG_ONE);
  requiredArg(source, TWO);           /* make sure we have the second too  */
  // retrieve extra parameter if exists
  if (initCount != 0)
    processNewArgs(init_args, initCount, &init_args, &initCount, 1, (RexxObject **)&option, NULL);
                                       /* go create a method                */
  newMethod = this->newRexxCode(pgmname, source, IntegerTwo, option);
   if (TheMethodClass->uninitDefined()) {/* does object have an UNINT method  */
     newMethod->hasUninit();           /* Make sure everyone is notified.   */
   }
                                       /* now send an INIT message          */
  newMethod->sendMessage(RexxActivityManager::currentActivity, OREF_INIT, init_args, initCount);
  return newMethod;                    /* return the new method             */
}


RexxMethod *RexxMethod::newRexxBuffer(
      RexxString *pgmname,             /* file name to process              */
      RexxBuffer *source)              /* String or buffer with source      */
/******************************************************************************/
/* Function:  Build a new method object from buffered REXX source             */
/******************************************************************************/
{
  RexxSource *newSource;               /* new source object                 */

  if (source == OREF_NULL)             /* didn't get source?                */
                                       /* raise an error                    */
    reportException(Error_Incorrect_method_noarg, IntegerTwo);
                                       /* create a source object            */
  newSource = RexxSource::classNewBuffered(pgmname, source);
  ProtectedObject p1(newSource);
                                       /* now complete method creation      */
  return newRexxMethod(newSource);
}


RexxMethod *RexxMethod::newMethod(stringchar_t *source, stringsize_t length)
{
    RexxBuffer *buffer = new_buffer(source, length);
                                       /* create a source object            */
    RexxSource *newSource = RexxSource::classNewBuffered(OREF_NULLSTRING, buffer);
    ProtectedObject p1(newSource);
                                       /* now complete method creation      */
    return newSource->method();
}


RexxMethod *RexxMethod::newEntry(PFN entry)
                        /* routine entry point               */
/******************************************************************************/
/* Function:  Create a native method from an entry point                      */
/******************************************************************************/
{
  RexxNativeCode *newCode;             /* associated REXX code object       */

                                       /* create a new code object          */
  newCode = new RexxNativeCode (entry);
                                       /* get a new method object           */
  return new RexxMethod(newCode, OREF_NULL);
}


RexxMethod *RexxMethod::restore(
    RexxBuffer *buffer,                /* buffer containing the method      */
    stringchar_t *startPointer)        /* first character of the method     */
/******************************************************************************/
/* Function: Unflatten a translated method.  Passed a buffer object containing*/
/*           the method                                                       */
/******************************************************************************/
{

  RexxEnvelope *envelope;              /* containing envelope               */

                                       /* Get new envelope object           */
  envelope  = new RexxEnvelope;
  ProtectedObject p1(envelope);
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
  RexxSource *source;                  /* created source object             */
                                       /* create a source object            */
  source = RexxSource::classNewFile(filename);
  ProtectedObject p1(source);
                                       /* finish up processing of this      */
  return newRexxMethod(source);
}


/**
 * The exported NEWFILE method of the MethodClass.
 *
 * @param filename The source file name.
 *
 * @return The constructed method object.
 */
RexxMethod *RexxMethod::newFileRexx(RexxString *filename)
{
    return newFile(filename);
}

RexxMethod * RexxMethod::createKernelMethod(
    PCPPM           entryPoint,        /* method entry point                */
    size_t          arguments)         /* count of arguments                */
/******************************************************************************/
/* Function:  Create a primitive, C++ method object                           */
/******************************************************************************/
{
                                       /* create a new kernel method        */
  return new RexxMethod(0, entryPoint, arguments);
}

RexxMethod * RexxMethod::createProtectedKernelMethod(
    PCPPM           entryPoint,        /* method entry point                */
    size_t          arguments)         /* count of arguments                */
/******************************************************************************/
/* Function:  Create a primitive, C++ method object                           */
/******************************************************************************/
{
  RexxMethod *method;                  /* created method                    */
                                       /* create a new kernel method        */
  method = new RexxMethod(0, entryPoint, arguments);
  method->setProtected();              /* make this protected               */
  return method;                       /* return the method                 */
}

RexxMethod * RexxMethod::createPrivateKernelMethod(
    PCPPM           entryPoint,        /* method entry point                */
    size_t          arguments)         /* count of arguments                */
/******************************************************************************/
/* Function:  Create a primitive, C++ method object                           */
/******************************************************************************/
{
  RexxMethod *method;                  /* created method                    */
                                       /* create a new kernel method        */
  method = new RexxMethod(0, entryPoint, arguments);
  method->setProtected();              /* make this protected               */
  method->setPrivate();                /* and private                       */
  return method;                       /* return the method                 */
}


/**
 * Get a function activator for a method added to a program
 * ::ROUTINE list.
 *
 * @return OREF_NULL if this method is not set up to be a routine,
 *         otherwise a function activator appropriate to the type is
 *         returned.
 */
FunctionActivator *RexxMethod::getFunctionActivator()
{
    // no code object attached?  This cannot be called as a function
    if (code == OREF_NULL)
    {
        return OREF_NULL;
    }

    // if this is Rexx code, make one directly.
    if (isRexxMethod())
    {
        return ((RexxCode *)code)->getFunctionActivator(this);
    }
    else
    {
        return ((RexxNativeCode *)code)->getFunctionActivator();
    }
}

/**
 * Set a routine name for a Rexx method.
 *
 * @param name   The name of the routine.
 */
void RexxMethod::setRoutineName(RexxString *name)
{
    if (isRexxMethod())
    {
        ((RexxCode *)code)->setRoutineName(name);
        // force a function activator to be created now.
        getFunctionActivator();
    }
}


/**
 * Get the routine name for this method.  Returns NULL if this
 * is not actually a routine.
 *
 * @return The string routine name.
 */
RexxString *RexxMethod::getRoutineName()
{
    if (isRexxMethod())
    {
        return ((RexxCode *)code)->getRoutineName();
    }
    return OREF_NULL;
}


/**
 * Attach this native code object to its defining source.
 *
 * @param s      The source object where this method or routine was defined.
 */
void RexxMethod::setSource(RexxSource *s)
{
    OrefSet(this, source, s);
}


/**
 * Retrieve the public routines associated with this method's
 * source object.
 *
 * @return A directory object for the public routines.
 */
RexxDirectory *RexxMethod::getRoutines()
{
    if (source != OREF_NULL)
    {
        return source->getRoutinesApi();
    }
    return OREF_NULL;
}


/**
 * Retrieve the unattached methods associated with this method's
 * source object.
 *
 * @return A directory object for the public methods.
 */
RexxDirectory *RexxMethod::getMethods()
{
    if (source != OREF_NULL)
    {
        return source->getMethodsApi();
    }
    return OREF_NULL;
}


/**
 * Retrieve the classes defined in this method's source file.
 *
 * @return A directory object for the classes.
 */
RexxDirectory *RexxMethod::getClasses()
{
    if (source != OREF_NULL)
    {
        return source->getClassesApi();
    }
    return OREF_NULL;
}


/**
 * Resolve a class from the current method context source file.
 *
 * @param name   The target class name.
 *
 * @return A resolved class object.
 */
RexxClass *RexxMethod::resolveClass(RexxActivity *activity, RexxString *name)
{
    if (source != OREF_NULL)
    {
        return source->resolveClass(activity, name);
    }
    return OREF_NULL;
}
