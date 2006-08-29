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
/* REXX Memory access                                                         */
/*                                                                            */
/* Root image object that is anchor to cross-process information              */
/*                                                                            */
/******************************************************************************/

#ifndef Included_RootImage
#define Included_RootImage

#include "RexxCore.h"

class RexxInterpreter;
class RexxObjectTable;
class RexxStack;
class RexxSaveStack;
class RexxDirectory;
class RexxPointer;
class RexxActivityManager;
class PackageManager;

// the first object created in the Rexx object heap.  This is the
// object that will constitute the root object set for the interpreter
// and is the first object stored in the Rexx image file.  When an
// image is restored, this anchors all of the oldspace object, as well as
// anchors root tables for global new-space tables (such as the old-to-new table);
class RootImage : public RexxInternalObject
{
public:
    inline RootImage(RESTORETYPE restoreType) { ; };
    RootImage();

    inline void *operator new(size_t, void *ptr) {return ptr;}
    inline void  operator delete(void *, void *) {;}
    void *operator new(size_t);
    inline void  operator delete(void *) {;}
    void        live();
    void        liveGeneral();

    void        removeProcess(RexxObject *id);
    void        addProcess(RexxObject *id, RexxInterpreter *process);
    void        prepareForImageSave();
    void        createProcessTables(bool imageRestore);
    void        addProxiedObject(RexxObject *object, RexxString *name);
    RexxString *getObjectProxy(RexxObject *object);
    RexxObject *getProxiedObject(RexxString *proxy);

    RexxDirectory *globalEnvironment;         // the cross-process, cross/instance global environment
    RexxDirectory *interpreterAnchor;         // global interpreter private objects
    RexxDirectory *systemObjects;             // the .system objects directory
    RexxArray     *globalNames;               // global name string table
    RexxDirectory *commonRetrievers;          // set of common variable retriever objects
    RexxDirectory *staticRequires;            // statically protected requires files
    RexxDirectory *publicRoutines;            // publicly loaded routines

    RexxInteger   *trueObject;                // saved .true object
    RexxInteger   *falseObject;               // saved .false object
    RexxObject    *nilObject;                 // saved .nil object
    RexxArray     *nullArray;                 // single convenience null array object
    RexxPointer   *nullPointer;               // single null pointer object

    RexxArray     *primitiveBehaviours;       // root behaviour set

    RexxClass     *classClass;                // single instance of class object
    RexxActivityManager *activityManager;     // the main activity manager object (contains cross-process tables)
    PackageManager  *packageManager;          // global library definitions

    RexxObjectTable *proxyToNameMap;          // map of proxied objects to their proxy string
    RexxDirectory   *nameToProxyMap;          // map proxy names to object instances

    // non-saved image elements follow here.  New versions of these are created
    // after an image restore and shared across all of the processed.
    WeakReference *weakReferenceList;         // weak reference list...global list
    RexxObjectTable *oldToNewTable;           // new generation references anchored in oldspace
    RexxStack  *liveStack;                    // stack used for GC pushes
    RexxStack  *flattenStack;                 // second stack used for object flattening operations
    RexxSaveStack    *saveStack;              // short-term memory protections
    RexxObjectTable  *saveTable;              // longer-term saved memory anchors
    RexxDirectory    *globalStringTable;      // global string table used during image build

    // This is the main interpreter tracking table
    RexxObjectTable *activeProcesses;         // table of active processes
};

#endif

