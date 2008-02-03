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
#ifndef PackageManager_Included
#define PackageManager_Included

#include "RexxCore.h"
#include "Package.hpp"

class PackageManager
{
public:
    static void        live();
    static void        liveGeneral();
    static Package    *getPackage(RexxString *name);
    static Package    *loadPackage(RexxString *name);
    static PNATIVEMETHOD      resolveMethodEntry(RexxString *packageName, RexxString *methodName);
    static RexxNativeCode    *resolveMethod(RexxString *packageName, RexxString *methodName);
    static PNATIVEFUNCTION   *resolveFunction(RexxString *function, RexxString *package, RexxString *procedure);
    static void        loadInternalPackage(RexxString *name, RexxPackageEntry *p);
    static void        createRootPackages();
    static void        restoreRootPackages(PackageManager *master);
    FunctionActivator *findFunction(RexxString *name);
    void        addFunction(RexxString *name, FunctionActivator *func);
    RexxObject *addRegisteredFunction(RexxString *name, RexxString *module, RexxString *proc);
    RexxObject *dropRegisteredFunction(RexxString *name);
    RexxObject *queryRegisteredFunction(RexxString *name);
    void        unload();

    static PackageManager *singleInstance;

protected:
    static RexxDirectory *packages;        // our loaded packages
    static RexxDirectory *packageFunctions; // table of functions loaded from packages
    static RexxDirectory *registeredFunctions;  // table of functions resolved by older registration mechanisms
    static Package *rexxPackage;           // generated internal rexx package;
};

#endif

