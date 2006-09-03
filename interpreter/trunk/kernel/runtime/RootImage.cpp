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
/* root object in the Rexx memory image                                       */
/*                                                                            */
/******************************************************************************/

#include "RootImage.hpp"
#include "DirectoryClass.hpp"

/**
 * Create a new RootImage object instance.
 *
 * @param size   Size of the object.
 *
 * @return Pointer to new object storage.
 */
void *RootImage::operator new(size_t size)
{
    return new_object(size, TheRootImageBehaviour);
}


/**
 * Base constructor for a RootImage object.
 */
RootImage::RootImage()
{

}

void RootImage::live()
/******************************************************************************/
/* Function:  Normal garbage collection live marking                          */
/******************************************************************************/
{
  setUpMemoryMark
  memory_mark(activeProcesses);      // do this one first, hits the majority of the objects
  memory_mark(globalEnvironment);
  memory_mark(interpreterAnchor);
  memory_mark(proxyToNameMap);
  memory_mark(nameToProxyMap);
  memory_mark(globalNames);
  memory_mark(staticRequires);
  memory_mark(publicRoutines);
  memory_mark(trueObject);
  memory_mark(falseObject);
  memory_mark(nilObject);
  memory_mark(nullArray);
  memory_mark(nullPointer);
  memory_mark(primitiveBehaviours);
  memory_mark(classClass);
  memory_mark(activityManager);
  memory_mark(packageManager);
  memory_mark(oldToNewTable);
  memory_mark(liveStack);
  memory_mark(flattenStack);
  memory_mark(saveStack);
  memory_mark(saveTable);
  memory_mark(globalStringTable);

  // weak references are not marked...these are processed after all other marking
  // is completed.

  cleanUpMemoryMark
}
void RootImage::liveGeneral()
/******************************************************************************/
/* Function:  Generalized object marking                                      */
/******************************************************************************/
{
  setUpMemoryMarkGeneral
  memory_mark_general(activeProcesses);      // do this one first, hits the majority of the objects
  memory_mark_general(globalEnvironment);
  memory_mark_general(interpreterAnchor);
  memory_mark_general(proxyToNameMap);
  memory_mark_general(nameToProxyMap);
  memory_mark_general(globalNames);
  memory_mark_general(staticRequires);
  memory_mark_general(publicRoutines);
  memory_mark_general(trueObject);
  memory_mark_general(falseObject);
  memory_mark_general(nilObject);
  memory_mark_general(nullArray);
  memory_mark_general(nullPointer);
  memory_mark_general(primitiveBehaviours);
  memory_mark_general(classClass);
  memory_mark_general(activityManager);
  memory_mark_general(packageManager);
  memory_mark_general(oldToNewTable);
  memory_mark_general(liveStack);
  memory_mark_general(flattenStack);
  memory_mark_general(saveStack);
  memory_mark_general(saveTable);
  memory_mark_general(globalStringTable);

  cleanUpMemoryMarkGeneral
}


void RootImage::prepareForImageSave()
/******************************************************************************/
/* Function:  Prepare this object for an image save. All objects that are     */
/* not to be included in the save image are nulled out prior to the save      */
/******************************************************************************/
{
    // just null out all of the runtime-transient references
    oldToNewTable = OREF_NULL;
    liveStack = OREF_NULL;
    flattenStack = OREF_NULL;
    saveStack = OREF_NULL;
    saveTable = OREF_NULL;
    activeProcesses = OREF_NULL;
    globalStringTable = OREF_NULL;

    // we're going to do a GC immediately after this, so we need to
    // process the weak image list...do NOT null that one out.
}


void RootImage::createProcessTables(bool imageRestore)
/******************************************************************************/
/* Function:  To new process creation activities.                             */
/******************************************************************************/
{
    // liveStack and saveStack have already been created.  The flattenStack is
    // deferred until the first request.

    // create the process anchor table
    activeProcesses = new_object_table();
    // if this is an
    if (imageRestore)
    {
        // create a table to map references from old space to new space
        oldToNewTable = new_object_table();
    }
    else
    {
        // we also need a global string table to intern strings
        globalStringTable = new_directory();
        proxyToNameMap = new_object_table();
        nameToProxyMap = new_directory();
    }
}

void RootImage::addProxiedObject(RexxObject *object, RexxString *name)
{
    // cross index these objects in both tables.
    proxyToNameMap->put(name, object);
    nameToProxyMap->put(object, name);
    // mark this object as proxied
    object->header.makeProxiedObject();
}

RexxString *RootImage::getObjectProxy(RexxObject *object)
{
    RexxString *proxy = (RexxString *)proxyToNameMap->get(object);
    if (proxy != OREF_NULL)
    {
        // create a proxy object from this string
        return new_proxy(proxy);
    }
    return OREF_NULL;
}

RexxObject *RootImage::getProxiedObject(RexxString *proxy)
{
    return nameToProxyMap->get(proxy);
}

/**
 * Add a new interpreter instance to the active process table.
 *
 * @param id       The process identifier for this instance.
 * @param instance The interpreter object instance anchoring the process specific
 *                 heap space.
 */
void RootImage::addProcess(RexxObject *id, RexxInterpreter *instance)
{
    activeProcesses->put((RexxObject *)instance, id);
}

/**
 * Remove the process specific anchor object from the global table.
 *
 * @param id     The process identifier.
 */
void RootImage::removeProcess(RexxObject *id)
{
    activeProcesses->remove(id);
}
