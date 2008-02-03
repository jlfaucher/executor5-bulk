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
/* REXX Kernel                                         RexxNativeMethod.cpp   */
/*                                                                            */
/* Primitive Method Class                                                     */
/*                                                                            */
/******************************************************************************/
#include <stdlib.h>
#include <string.h>
#include "RexxCore.h"
#include "StringClass.hpp"
#include "DirectoryClass.hpp"
#include "LibraryManager.hpp"
#include "RexxNativeActivation.hpp"
#include <ctype.h>


RexxNativeCode::RexxNativeCode(RexxString *_name, RexxString *_package)
{
    // and this is the information needed to resolve this again after an
    // image restore
    OrefSet(this, this->package, _package);
    OrefSet(this, this->name, _name);
}


/**
 * Initialize a native code entry from its package information.
 */
void RexxNativeCode::reinit()
{
    // go resolve this
    entry = PackageManager::resolve(package, name);
}


void RexxNativeCode::live(size_t liveMark)
/******************************************************************************/
/* Function:  Normal garbage collection live marking                          */
/******************************************************************************/
{
    memory_mark(this->package);
    memory_mark(this->name);
}


void RexxNativeCode::liveGeneral(int reason)
/******************************************************************************/
/* Function:  Generalized object marking                                      */
/******************************************************************************/
{
    memory_mark_general(this->package);
    memory_mark_general(this->name);
}


void RexxNativeMethod::flatten(RexxEnvelope *envelope)
/******************************************************************************/
/* Function:  Flatten an object                                               */
/******************************************************************************/
{
  setUpFlatten(RexxNativeMethod)

   flatten_reference(newThis->package, envelope);
   flatten_reference(newThis->name, envelope);
                                       /* Set entry to NUll for 2 reasons   */
                                       /* 1st force branch to 0 is not      */
                                       /*restored, 2 used to indicated if   */
                                       /* the method has bee unflattened    */
   newThis->entry = NULL;
  cleanUpFlatten
}


/**
 * Lazy resolve a native method.  This will be done on the first call
 * to a method definition that's been saved and restored.  On
 * restore, we have a potential situation where we need to get
 * the package reloaded and restored before we can resolve the entry
 * point.
 */
void RexxNativeMethod::resolve()
{
    // have the package manager resolve this for us before we make a call
    entry = PackageManager::resolveMethod(this, package, name);
}


/**
 * Run a method call (vs a straight program call).
 *
 * @param activity The current activity.
 * @param method   The method we're attached to.
 * @param receiver The method receiver object (the "self" object).
 * @param messageName
 *                 The name of the message used to invoke the method.
 * @param count    The count of arguments.
 * @param argPtr   The pointer to the arguments.
 * @param result   The protected object used to return the result.
 */
void RexxNativeMethod::run(RexxActivity *activity, RexxMethod *method, RexxObject *receiver, RexxString *messageName,
    size_t count, RexxObject **argPtr, ProtectedObject &result)
{
    // if this is NULL currently, we need to lazy resolve this entry
    if (entry == NULL)
    {
        resolve();
    }

    // create a new native activation
    RexxNativeActivation *newNActa = new RexxNativeActivation(activity, method);
    activity->pushStackFrame(newNActa);   /* push it on the activity stack     */
                                       /* and go run it                     */
    newNActa->run(this, receiver, messageName, count, argPtr, result);
}


void * RexxNativeMethod::operator new(
     size_t      size)                 /* object size                       */
/****************************************************************************/
/* Function:  Create a new Native method                                    */
/****************************************************************************/
{
    return new_object(size, T_NativeMethod);  // Get new object
}


void RexxNativeFunction::flatten(RexxEnvelope *envelope)
/******************************************************************************/
/* Function:  Flatten an object                                               */
/******************************************************************************/
{
  setUpFlatten(RexxNativeFunction)

   flatten_reference(newThis->package, envelope);
   flatten_reference(newThis->name, envelope);
                                       /* Set entry to NUll for 2 reasons   */
                                       /* 1st force branch to 0 is not      */
                                       /*restored, 2 used to indicated if   */
                                       /* the method has bee unflattened    */
   newThis->entry = NULL;
  cleanUpFlatten
}


/**
 * Lazy resolve a native method.  This will be done on the first call
 * to a method definition that's been saved and restored.  On
 * restore, we have a potential situation where we need to get
 * the package reloaded and restored before we can resolve the entry
 * point.
 */
void RexxNativeFunction::resolve()
{
    // have the package manager resolve this for us before we make a call
    entry = PackageManager::resolveFunction1(this, package, name);
}



/**
 * Run a method call (vs a straight program call).
 *
 * @param activity The current activity.
 * @param functionName
 *                 The name of the message used to invoke the method.
 * @param count    The count of arguments.
 * @param argPtr   The pointer to the arguments.
 * @param result   The protected object used to return the result.
 */
void RexxNativeFunction::call(RexxActivity *activity, RexxString *functionName, size_t count, RexxObject **argPtr, ProtectedObject &result)
{
    // if this is NULL currently, we need to lazy resolve this entry
    if (entry == NULL)
    {
        resolve();
    }

    // create a new native activation
    RexxNativeActivation *newNActa = new RexxNativeActivation(activity);
    activity->pushStackFrame(newNActa);   /* push it on the activity stack     */
                                       /* and go run it                     */
    newNActa->run(this, functionName, count, argPtr, result);
}


void * RexxNativeFunction::operator new(
     size_t      size)                 /* object size                       */
/****************************************************************************/
/* Function:  Create a new Native method                                    */
/****************************************************************************/
{
    return new_object(size, T_NativeFunction);  // Get new object
}


/**
 * Run a method call (vs a straight program call).
 *
 * @param activity The current activity.
 * @param functionName
 *                 The name of the message used to invoke the method.
 * @param count    The count of arguments.
 * @param argPtr   The pointer to the arguments.
 * @param result   The protected object used to return the result.
 */
void RegisteredFunction::call(RexxActivity *activity, RexxString *functionName, size_t count, RexxObject **argPtr, ProtectedObject &result)
{
    // Registered functions are always resolved at run time, so this does not
    // need to be done in a lazy fashion.

    // create a new native activation
    RexxNativeActivation *newNActa = new RexxNativeActivation(activity);
    activity->pushStackFrame(newNActa);   /* push it on the activity stack     */
                                       /* and go run it                     */
    newNActa->run(this, functionName, count, argPtr, result);
}


void * RegisteredFunction::operator new(
     size_t      size)                 /* object size                       */
/****************************************************************************/
/* Function:  Create a new Native method                                    */
/****************************************************************************/
{
    return new_object(size, T_RegisterdFunction);  // Get new object
}
