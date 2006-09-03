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
/* Interpreter                                                                */
/*                                                                            */
/* Weak reference support for Rexx memory management                          */
/*                                                                            */
/******************************************************************************/

#include "WeakReference.hpp"

void *WeakReference::operator new(size_t size)
/******************************************************************************/
/* Function:  Create a new pointer object                                     */
/******************************************************************************/
{
  WeakReference *newObject;            /* new object                        */

                                       /* Get new object                    */
  newObject = (WeakReference *) new_object(size);
                                       /* Give new object its behaviour     */
  BehaviourSet(newObject, TheWeakReferenceBehaviour);
  // these never get marked, so we disable the live method call.
  SetObjectHasNoReferences(newObject); /* this has no references            */
  return (void *)newObject;            /* return the new object             */
}

WeakReference::WeakReference(RexxObject *r)
/******************************************************************************/
/* Function:  Construct a non-notifying weak reference                        */
/******************************************************************************/
{
    OrefSet(this, referentObject, r);
}

void WeakReference::liveGeneral()
/******************************************************************************/
/* Function:  Generalized object marking                                      */
/******************************************************************************/
{
  setUpMemoryMarkGeneral

  // these references are only marked during a save or restore image process.
  // NOTE:  WeakReference objects saved in the Rexx image get removed from the
  // weak reference list and just become normal objects.  Since the weak references
  // can only contain references to other oldspace objects at that point, those
  // objects will never get collected.

  if (RexxMemory::memoryObject.savingImage() || RexxMemory::memoryObject.restoringImage())
  {
      memory_mark_general(referentObject);
  }

  cleanUpMemoryMarkGeneral
}

void WeakReference::flatten(RexxEnvelope *envelope)
/******************************************************************************/
/* Function:  Flatten an object                                               */
/******************************************************************************/
{
  setUpFlatten(WeakReference)

   flatten_reference(newThis->referentObject, envelope);

   // make sure the new version has nulled out list pointers
   newThis->nextReferenceList = OREF_NULL;

  cleanUpFlatten
}

RexxObject *WeakReference::unflatten(RexxEnvelope *envelope)
/******************************************************************************/
/* Function:  unflatten an object                                             */
/******************************************************************************/
{
    // if we still have a reference to handle, then add this to the
    // tracking list
    if (referentObject != OREF_NULL)
    {
        RexxMemory::memoryObject.addWeakReference(this);
    }
    return (RexxObject *)this;           /* return ourself.                   */
}

void WeakReference::clear()
/******************************************************************************/
/* Function:  clear an object reference, and potentially move to notification */
/* queue so the notification object can be "tapped"                           */
/******************************************************************************/
{
    // clear the reference out.
    OrefSet(this, referentObject, OREF_NULL);
}
