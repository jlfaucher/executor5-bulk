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
/*                                                                           */
/* Manage a created instance of the interpreter                               */
/*                                                                            */
/******************************************************************************/
#ifndef Included_InterpreterInstance_hpp
#define Included_Interpreterinstance_hpp

#include "RexxCore.h"
#include "ExitHandler.hpp"
#include "oorexx.h"

// an instance object used to anchor
class InterpreterInstanceObject : public RexxInternalObject
{
public:
    friend class InterpreterInstance;

    // methods associated with actual interpreter instances
    inline InterpreterInstanceObject(RESTORETYPE restoreType) { ; };
    InterpreterInstanceObject(InterpreterInstance *parent, RexxInterpreter *i);

    inline void *operator new(size_t, void *ptr) {return ptr;}
    inline void  operator delete(void *, void *) {;}
    void *operator new(size_t);
    inline void  operator delete(void *) {;}
    void        live();
    void        liveGeneral();
    InterpreterInstance *getInstance() { return instance; }
    RexxString *getDefaultAddress() { return defaultAddress; }

protected:

    InterpreterInstance *instance;           // our main instance object
    RexxInterpreter     *interpreter;        // our attached interpreter instance
    RexxDirectory       *localEnvironment;   // the local environment associated with this instance
    RexxObjectTable     *activeActivities;   // the activity table
    RexxActivity        *primeActivity;      // the primary activity
    RexxObjectTable     *globalReferences;   // our global reference table
    RexxString          *defaultAddress;     // the default address environment
};


class InterpreterInstance
{
    friend class RexxInterpreter;
public:

    InterpreterInstance(ExitHandler *handlers);
    void addActivity(RexxActivity *);
    void removeActivity(RexxActivity *);
    bool terminate();
    void waitForCompletion();
    void attachToProcess();
    RexxActivity *enterOnCurrentThread();
    bool attachThread(RexxThreadContext **);
    void exitCurrentThread();
    RexxActivity *findActivity(RexxThread &threadId);
    RexxDirectory *getLocalEnvironment();
    RexxInstance *getInstanceContext() { return (RexxInstance *)&interfaceContext; }
    void copyExits(ExitHandler *target);
    void activityTerminated(RexxActivity *activity);
    void halt();
    void setTrace(bool setting);
    void processExternalHalt();
    void activityDeactivated(RexxActivity *activity);
    void addGlobalReference(RexxObject *o);
    void removeGlobalReference(RexxObject *o);
    RexxString *getDefaultCommandEnvironment() { return instanceObject->getDefaultAddress(); }
    bool        processOptions(RexxOption *options);

protected:

    // The following MUST be the first declared field member of this class.
    // This is the reference handed out to callers, and we cast this back into
    // the class pointer on interface calls.
    RexxInstance interfaceContext;

    // the base hook for holding Rexx objects
    InterpreterInstanceObject *instanceObject;
    bool terminating;           // shutdown indicator
    bool terminated;            // last thread cleared indicator
    // array of system exits
    ExitHandler exits[RXNOOFEXITS + 1];
    RexxInterpreter *interpreterInstance;    // the process activity instance

    static RexxInstanceInterface interfaceVector;   // single interface vector instance
};

#endif

