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
/* REXX Kernel                                                                */
/*                                                                            */
/* Classes to manage caching of external function calls                       */
/*                                                                            */
/******************************************************************************/

#include "FunctionActivator.hpp"
#include "SysInterpreter.hpp"
#include "RexxActivation.hpp"
#include "RexxNativeActivation.hpp"
#include "RexxActivity.hpp"
#include "rexx.h"
#include "WeakReference.hpp"

/**
 * Create a new object instance.
 *
 * @param size   Size of the object.
 *
 * @return Pointer to new object storage.
 */
void *NativeActivator::operator new(size_t size)
{
    return new_object(size, TheNativeActivatorBehaviour);
}

/**
 * Create a new object instance.
 *
 * @param size   Size of the object.
 *
 * @return Pointer to new object storage.
 */
void *ClassicNativeActivator::operator new(size_t size)
{
    return new_object(size, TheClassicNativeActivatorBehaviour);
}

/**
 * Create a new object instance.
 *
 * @param size   Size of the object.
 *
 * @return Pointer to new object storage.
 */
void *ScriptNativeActivator::operator new(size_t size)
{
    return new_object(size, TheScriptNativeActivatorBehaviour);
}

/**
 * Create a new object instance.
 *
 * @param size   Size of the object.
 *
 * @return Pointer to new object storage.
 */
void *ObjectNativeActivator::operator new(size_t size)
{
    return new_object(size, TheObjectNativeActivatorBehaviour);
}

/**
 * Create a new object instance.
 *
 * @param size   Size of the object.
 *
 * @return Pointer to new object storage.
 */
void *TypedNativeActivator::operator new(size_t size)
{
    return new_object(size, TheTypedNativeActivatorBehaviour);
}

/**
 * Create a new object instance.
 *
 * @param size   Size of the object.
 *
 * @return Pointer to new object storage.
 */
void *MethodActivator::operator new(size_t size)
{
    return new_object(size, TheMethodActivatorBehaviour);
}

/**
 * Create a new object instance.
 *
 * @param size   Size of the object.
 *
 * @return Pointer to new object storage.
 */
void *RexxFunctionActivator::operator new(size_t size)
{
    return new_object(size, TheRexxFunctionActivatorBehaviour);
}

/**
 * Create a new object instance.
 *
 * @param size   Size of the object.
 *
 * @return Pointer to new object storage.
 */
void *WeakRexxActivator::operator new(size_t size)
{
    return new_object(size, TheWeakRexxActivatorBehaviour);
}

void ClassicNativeActivator::call(RexxActivation *activation, RexxObject ** arguments,
    size_t argcount, RexxString *calltype, ProtectedObject &result)
{
    /* allocate enough memory for all arguments */
    /* at least one item needs to be allocated to prevent error reporting */
    RXSTRING *argrxarray = (RXSTRING *) SysInterpreter::allocateResultMemory(sizeof(RxString) * max(argcount , 1));
    if (argrxarray == OREF_NULL)    /* memory error?                   */
    {
        reportException(Error_System_resources);
    }
    /* create RXSTRING arguments         */
    for (size_t argindex = 0; argindex < argcount; argindex++)
    {
        /* get the next argument             */
        RexxString *argument = (RexxString *)arguments[argindex];
        if (argument != OREF_NULL)
        {   /* have an argument?                 */
            /* force to string form              */
            argument = argument->stringValue();
            // replace in argArray to help protect from being GC'd.
            // We're replacing this reference in place, since we've already
            // determined this is the call target.  We won't be messing up the
            // argument list for any other callers.
            arguments[argindex] = argument;
            MAKERXSTRING(argrxarray[argindex], argument->getStringData(), argument->getLength());
        }
        else
        {                           /* have an omitted argument          */
            /* give it zero length               */
            argrxarray[argindex].strlength = 0;
            /* and a zero pointer                */
            argrxarray[argindex].strptr = NULL;
        }
    }
                                         /* default return code buffer        */
    stringchar_t default_return_buffer[DEFRXSTRING];
    /* get the current queue name        */
    char *queuename = (char *)activation->activity->getCurrentQueue()->getStringData();
    char *functionName = (char *)name->getStringData();

    RXSTRING funcresult;
    /* make the RXSTRING result          */
    MAKERXSTRING(funcresult, default_return_buffer, sizeof(default_return_buffer));

    // our calling context
    CallContext context;

/* CRITICAL window here -->>  ABSOLUTELY NO KERNEL CALLS ALLOWED            */

    // create the calling context (including a native activation), and
    // release access to the interpreter
    activation->activity->createCallContext(&context);

    RexxFunctionHandler *functionAddress = (RexxFunctionHandler *)entryPoint;
    RexxReturnCode functionRc = (*functionAddress)(functionName, (unsigned long)argcount, argrxarray, queuename, (RXSTRING *)&funcresult);
    // now reenter for Rexx code execution again
    activation->activity->destroyCallContext(&context);

/* END CRITICAL window here -->>  kernel calls now allowed again            */

    SysInterpreter::releaseResultMemory(argrxarray);

    if (functionRc == 0)
    {           /* If good rc from function          */
        if (funcresult.strptr)
        {       /* If we have a result, return it    */
            /* make a string result              */
            result = new_string((stringchar_t *)funcresult.strptr, funcresult.strlength);
            /* user give us a new buffer?        */
            if (funcresult.strptr != (RexxStringPointer)default_return_buffer )
            {
                /* free it                           */
                SysInterpreter::releaseResultMemory(funcresult.strptr);
            }
        }
        else
        {
            result = OREF_NULL;         /* nothing returned                  */
        }
    }
    else                                /* Bad rc from function, signal      */
    {
        reportException(Error_Incorrect_call_external, name);
    }
}

void ObjectNativeActivator::call(RexxActivation *activation, RexxObject ** arguments,
    size_t argcount, RexxString *calltype, ProtectedObject &result)
{
    RexxObject *resultObject = OREF_NULL;

    // our calling context
    CallContext context;

/* CRITICAL window here -->>  ABSOLUTELY NO KERNEL CALLS ALLOWED            */

    // create the calling context (including a native activation), and
    // release access to the interpreter
    activation->activity->createCallContext(&context);

    ooRexxObjectFunctionHandler *functionAddress = (ooRexxObjectFunctionHandler *)entryPoint;
    RexxReturnCode functionRc = (*functionAddress)((RexxCallContext *)&context, argcount,
        (RexxObjectPtr *)arguments, (RexxObjectPtr *)&resultObject);

    // anchor this as the protected object before releasing the context that is
    // protecting this
    result = resultObject;
    // now reenter for Rexx code execution again
    activation->activity->destroyCallContext(&context);

/* END CRITICAL window here -->>  kernel calls now allowed again            */

    // raise an error is one was signaled
    if (functionRc != 0)
    {
        reportException(Error_Incorrect_call_external, name);
    }
}

void ScriptNativeActivator::call(RexxActivation *activation, RexxObject ** arguments,
    size_t argcount, RexxString *calltype, ProtectedObject &result)
{
    RexxObject *resultObject = OREF_NULL;

    // our calling context
    CallContext context;

/* CRITICAL window here -->>  ABSOLUTELY NO KERNEL CALLS ALLOWED            */

    // create the calling context (including a native activation), and
    // release access to the interpreter
    activation->activity->createCallContext(&context);

    ooRexxScriptFunctionHandler *functionAddress = (ooRexxScriptFunctionHandler *)entryPoint;
    RexxReturnCode functionRc = (*functionAddress)((RexxCallContext *)&context, argcount,
        (RexxObjectPtr *)arguments, descriptor, (RexxObjectPtr *)&resultObject);

    // anchor this as the protected object before releasing the context that is
    // protecting this
    result = resultObject;
    // now reenter for Rexx code execution again
    activation->activity->destroyCallContext(&context);

/* END CRITICAL window here -->>  kernel calls now allowed again            */

    // raise an error is one was signaled
    if (functionRc != 0)
    {
        reportException(Error_Incorrect_call_external, name);
    }
}


void TypedNativeActivator::call(RexxActivation *activation, RexxObject ** arguments,
    size_t argcount, RexxString *calltype, ProtectedObject &result)
{
    RexxActivity *activity = activation->activity;

    // this sort of call is handled by native activation, so push one and hand it of
    // to the call method.
    RexxNativeActivation *newNActa = new RexxNativeActivation(activity, activation, RexxNativeActivation::FUNCTION_ACTIVATION);
    activity->pushNativeActivation(newNActa);          /* push it on the activity stack     */
    newNActa->call((PNFUNCTION)entryPoint, argcount, arguments, result);
    // and pop of the activation now that we're complete
    activity->popNativeActivation(newNActa);
}

void MethodActivator::call(RexxActivation *activation, RexxObject ** arguments,
    size_t argcount, RexxString *calltype, ProtectedObject &result)
{
    RexxActivity *activity = activation->activity;
    // pass this off directly to the method object
    // TODO:  need to sort out the calltype/environment
    method->callExternalRoutine(activity, name, arguments, argcount, calltype, OREF_NULL, result);
}

void RexxFunctionActivator::call(RexxActivation *activation, RexxObject ** arguments,
    size_t argcount, RexxString *calltype, ProtectedObject &result)
{
    MethodActivator::call(activation, arguments, argcount, calltype, result);

    RexxActivity *activity = activation->activity;
    // pass this off directly to the method object
    // TODO:  need to sort out the calltype/environment
    method->callExternalRoutine(activity, name, arguments, argcount, calltype, OREF_NULL, result);
    // a call to an external Rexx method requires merging the required stuff
    activation->mergeRequired(method);
}

void MethodActivator::live()
/******************************************************************************/
/* Function:  Normal garbage collection live marking                          */
/******************************************************************************/
{
  setUpMemoryMark
  memory_mark(this->name);
  memory_mark(this->method);
  cleanUpMemoryMark
}

void MethodActivator::liveGeneral()
/******************************************************************************/
/* Function:  Generalized object marking                                      */
/******************************************************************************/
{
  setUpMemoryMarkGeneral
  memory_mark_general(this->name);
  memory_mark_general(this->method);
  cleanUpMemoryMarkGeneral
}

void WeakRexxActivator::live()
/******************************************************************************/
/* Function:  Normal garbage collection live marking                          */
/******************************************************************************/
{
  setUpMemoryMark
  memory_mark(this->name);
  memory_mark(this->resolvedMethod);
  memory_mark(this->fileName);
  cleanUpMemoryMark
}

void WeakRexxActivator::liveGeneral()
/******************************************************************************/
/* Function:  Generalized object marking                                      */
/******************************************************************************/
{
  setUpMemoryMarkGeneral
  memory_mark_general(this->name);
  memory_mark_general(this->resolvedMethod);
  memory_mark_general(this->fileName);
  cleanUpMemoryMarkGeneral
}

void FunctionActivator::live()
/******************************************************************************/
/* Function:  Normal garbage collection live marking                          */
/******************************************************************************/
{
  setUpMemoryMark
  memory_mark(this->name);
  cleanUpMemoryMark
}

void FunctionActivator::liveGeneral()
/******************************************************************************/
/* Function:  Generalized object marking                                      */
/******************************************************************************/
{
  setUpMemoryMarkGeneral
  memory_mark_general(this->name);
  cleanUpMemoryMarkGeneral
}
/**
 * A flatten method for RexxFunctionActivators, which is the
 * only variety we ever flatten in a source object.
 *
 * @param envelope The target envelope for the flatten.
 */
void RexxFunctionActivator::flatten(RexxEnvelope * envelope)
{
  setUpFlatten(RexxFunctionActivator)

   flatten_reference(newThis->name, envelope);
   flatten_reference(newThis->method, envelope);

  cleanUpFlatten
}

WeakRexxActivator::WeakRexxActivator(RexxString *n, RexxString *f, RexxMethod *m) : RexxFunctionActivator(n, m), fileName(f)
{
    // create a weak reference and anchor it here.  The superclass constructor
    // also anchors this in the method field.  After the first call, this will be
    // cleared out, allowing the WeakReference to perform its magic.
    WeakReference *ref = new WeakReference(m);
    OrefSet(this, resolvedMethod, ref);
}

void WeakRexxActivator::call(RexxActivation *activation, RexxObject ** arguments,
    size_t argcount, RexxString *calltype, ProtectedObject &result)
{
    // see if we still have this around
    RexxMethod *realMethod = (RexxMethod *)resolvedMethod->get();
    // has this been garbage collected?
    if (realMethod == OREF_NULL)
    {
        ProtectedObject pMethod;
        // try to load the program
        RexxMethod::loadRexxProgram(fileName, pMethod);

        realMethod = (RexxMethod *)(RexxObject *)pMethod;
        if (realMethod == OREF_NULL)
        {
            // can't re-read this for some reason, report the error
            reportException(Error_Program_unreadable_name, fileName);
        }
        // create a new weak reference object
        WeakReference *newRef = new WeakReference(realMethod);
        // and set this
        OrefSet(this, resolvedMethod, newRef);
    }
    // set this in the method field so we can call
    OrefSet(this, method, realMethod);

    // now have the super class do the calling
    RexxFunctionActivator::call(activation, arguments, argcount, calltype, result);

    // after the call, we null out this value so the weak reference can be GC'd if needed
    OrefSet(this, method, OREF_NULL);
}
