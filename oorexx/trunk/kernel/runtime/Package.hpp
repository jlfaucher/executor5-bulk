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
/* Primitive Rexx function/method package                                     */
/*                                                                            */
/******************************************************************************/
#ifndef Package_Included
#define Package_Included

#include "RexxCore.h"
#include "SysLibrary.hpp"
#include "oorexx.h"

class PackageManager;

typedef ooRexxPackageEntry * (RexxEntry *PACKAGE_LOADER)();

class Package : public RexxInternalObject
{
public:
    inline void *operator new(size_t, void *ptr) {return ptr;}
    inline void  operator delete(void *, void *) {;}
    void *operator new(size_t);
    inline void  operator delete(void *) {;}

    Package(RexxString *name);
    Package(RexxString *n, PackageManager *m, ooRexxPackageEntry *p);
    inline Package(RESTORETYPE restoreType) { ; };

    void   live();
    void   liveGeneral();
    bool   load(PackageManager *manager);
    void   unload();
    ooRexxPackageEntry *getPackageTable();
    void   loadPackage(PackageManager *manager, ooRexxPackageEntry *p);
    void   loadFunctions(PackageManager *manager, ooRexxFunctionEntry *table);
    void   loadMethods(PackageManager *manager, ooRexxMethodEntry *table);
    FunctionActivator *resolveFunction(RexxString *name, RexxString *procedure);
    FunctionActivator *createActivator(RexxString *name, PFN entry, int style);
    ooRexxMethodEntry *locateMethodEntry(RexxString *name);
    ooRexxFunctionEntry *locateFunctionEntry(RexxString *name);
    RexxNativeCode *resolveMethod(RexxString *name);
    PFN             resolveMethodEntry(RexxString *name);
    void   refreshPackage(PackageManager *manager);
    void   refreshPackage(PackageManager *manager, ooRexxPackageEntry *p);
    inline bool isLoaded() { return loaded; }
    inline bool isInternal() { return internal; }
    inline bool isPackage() { return ispackage; }

protected:

    ooRexxPackageEntry *package;  // loaded package information
    RexxString *libraryName;   // the name of the library
    RexxDirectory *functions;  // loaded functions
    RexxDirectory *methods;    // loaded functions
    SysLibrary  lib;           // the library management handle
    bool        ispackage;     // this is a valid package vs. classic oldstyle
    bool        loaded;        // we've at least been able to load the library
    bool        internal;      // this is an internal package...no library load required.
};

class PackageManager : public RexxInternalObject
{
public:
    inline PackageManager(RESTORETYPE restoreType) {;}
    PackageManager();

    inline void *operator new(size_t, void *ptr) {return ptr;}
    inline void  operator delete(void *, void *) {;}
    void *operator new(size_t);
    inline void  operator delete(void *) {;}

    void        live();
    void        liveGeneral();
    bool        isMaster() { return masterPackage; }
    void        setMaster() { masterPackage = true; }
    Package    *getPackage(RexxString *name);
    Package    *loadPackage(RexxString *name);
    PFN         resolveMethodEntry(RexxString *packageName, RexxString *methodName);
    RexxNativeCode *resolveMethod(RexxString *packageName, RexxString *methodName);
    FunctionActivator *resolveFunction(RexxString *function, RexxString *package, RexxString *procedure);
    RexxNativeCode *resolveNativeFunction(RexxString *function, RexxString *packageName, RexxString *procedure);
    void        loadInternalPackage(RexxString *name, ooRexxPackageEntry *p);
    void        createRootPackages();
    void        restoreRootPackages(PackageManager *master);
    FunctionActivator *findFunction(RexxString *name);
    void        addFunction(RexxString *name, FunctionActivator *func);
    RexxObject *addRegisteredFunction(RexxString *name, RexxString *module, RexxString *proc);
    RexxObject *dropRegisteredFunction(RexxString *name);
    RexxObject *queryRegisteredFunction(RexxString *name);
    void        unload();

    static void createInstance(RootImage *);
    static void restoreInstance(RootImage *);

    static PackageManager *singleInstance;

protected:
    bool masterPackage;             // this is the master package created during image save
    RexxDirectory *packages;        // our loaded packages
    RexxDirectory *functions;       // table of functions loaded from packages
    Package *rexxPackage;           // generated internal rexx package;
    Package *rexxutilPackage;       // generated internal rexxutil package.

    static ooRexxPackageEntry *rexxPackageTable;      // Rexx internal methods and functions.
    static ooRexxPackageEntry *rexxutilPackageTable;  // The rexxutil functions.
};

#endif

