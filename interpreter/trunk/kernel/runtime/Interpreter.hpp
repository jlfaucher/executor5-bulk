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
/*****************************************************************************/
/* REXX Windows Support                                                      */
/*                                                                           */
/* Main interpreter control.  This is the preferred location for all         */
/* platform independent global variables.                                    */
/* The interpreter does not instantiate an instance of this                  */
/* class, so most variables and methods should be static.                    */
/*                                                                           */
/*                                                                           */
/*****************************************************************************/

#ifndef RexxInterpreter_Included
#define RexxInterpreter_Included

#include "oorexx.h"
#include "SynchronizedBlock.hpp"
#include "DirectoryClass.hpp"
#include "SysThread.hpp"
#include "Interpreter.hpp"

class InterpreterInstance;
class InterpreterInstanceObject;
class ExitHandler;
class RexxMethod;
class RexxArray;
class RexxPointer;
class RexxString;
class RexxDirectory;
class FunctionActivator;
class PackageManager;


typedef struct {                       /* define for error table entries    */
 int   code;                           /* error message code                */
 int   msgid;                          /* error message number              */
} ErrorMessage;

// maximum number of pooled activities we'll keep for reuse.
#define MAX_ACTIVITY_POOL_SIZE 5

class RexxInterpreter : public RexxInternalObject
{
public:
friend class RexxMemory;
    // methods associated with actual interpreter instances
    // instances of RexxInterpreter are used as garbage collection
    // anchor points for all process-specific objects associated with
    // a processor instance.
    inline RexxInterpreter(RESTORETYPE restoreType) { ; };
    RexxInterpreter();

    inline void *operator new(size_t, void *ptr) {return ptr;}
    inline void  operator delete(void *, void *) {;}
    void *operator new(size_t);
    inline void  operator delete(void *) {;}
    void        live();
    void        liveGeneral();

    typedef enum
    {
        CREATE_IMAGE,                  // building the saved startup image
        RESTORE_IMAGE                  // restoring the startup image
    } InitializationReason;

    // the following methods are non-static methods that are exposed as
    // Rexx methods.
    RexxDirectory *RexxInterpreter::localEnvironment();
    static void runRexxProgram(RexxActivity *activity, stringchar_t *filename, size_t argCount, RxString *arguments,
        wholenumber_t callType, stringchar_t *initialAddress, RxString *instoreSource, RxString *instoreImage, RxString *result, RexxNumber *retCode);

    static bool createInstance(RexxInstance **instance, RexxThreadContext **context, ExitHandler *handlers, RexxOption *userData, InitializationReason reason = RESTORE_IMAGE);
    static void terminateInstance(InterpreterInstance *instance);
    static RexxReturnCode saveImage();
           bool removeInstance(InterpreterInstance *instance);
           void terminateActivityPool();
           RexxActivity *newActivity(bool createThread, SysThread::ThreadPriority priority, InterpreterInstance *instance);
    static void shutdownInstance(InterpreterInstance *instance);
    static RexxString *versionString();    // get the interpreter version string
    static void fatalError (const stringchar_t *desc);
    static wholenumber_t messageNumber(RexxString *);
    static RexxString *messageHeader(wholenumber_t);
    static RexxString *messageSubstitution(RexxString *, RexxArray *);
    static void shutdownEnvironment();
    static void initializeProcess(RootImage *rootImage);
                                       /* find an environment symbol        */
    static inline RexxObject *getEnvironment(RexxString *s) { return (theEnvironment->fastAt(s)); }
    static inline void        setEnvironment(RexxString *s, RexxObject *v) { theEnvironment->setEntry(s, v); }
    static inline RexxMethod *findPublicFunction(RexxString *name) { return (RexxMethod *)publicRoutines->entry(name); }
    static inline bool staticallyLoaded(RexxString *name) {
        return staticRequires->fastAt(name) != OREF_NULL;
    }
    static inline void setPackageManager(PackageManager *manager)
    {
        singleInstance->packageManager = manager;
    }
    static void   bootstrapClasses(RootImage *rootImage);
    static void   defineKernelMethod(stringchar_t  * name, RexxBehaviour * behaviour, PCPPM entryPoint, size_t arguments);
    static void   defineProtectedKernelMethod(stringchar_t  * name, RexxBehaviour * behaviour, PCPPM entryPoint, size_t arguments);
    static void   definePrivateKernelMethod(stringchar_t  * name, RexxBehaviour * behaviour, PCPPM entryPoint, size_t arguments);
    static bool   exportBaseClasses();
    static RexxString *addGlobalName(stringchar_t* value);
    static void   addPublicObject(stringchar_t* name, RexxObject  * object, RexxDirectory *dir);
    static void   shutdown();
    static void   addUninitObject(RexxObject *);
    static void   removeUninitObject(RexxObject *);
    static bool   isPendingUninit(RexxObject *);
    static void   uninitObject(RexxObject *);
    static inline void   checkUninitQueue() { if (singleInstance->pendingUninits > 0) runUninits(); }
    static void   runUninits();
    static bool   requestInterpreter(RexxActivity *activity);
    static void   lockInterpreter();
    static void   unlockInterpreter();
    static void   initializeInterpreter(InterpreterInstance *instance, InitializationReason reason);
    static void   createImage(InterpreterInstance *instance);
    static void   restoreImage();
    static void   createInterpreterTables();
    static void   copyObjectToRxString(RexxObject *value, RxString *target);
    static InterpreterInstanceObject *attachInstance(InterpreterInstance *);
           void   addInstance(InterpreterInstanceObject *instance);
    static void   activityTerminated(RexxActivity *activity);
    static bool   addToActivityPool(RexxActivity *activity);
    static RexxObject *resolveProxy(RexxString *name);
    static int32_t generateRandomNumberSeed();
    static FunctionActivator *findCachedExternalFunction(RexxString *target);
    static void addCachedExternalFunction(RexxString *target, FunctionActivator *activator);
    static RexxDirectory *getSourceStringTable();
    static RexxCallContext *getCurrentCallContext();
    static RexxReturnCode startRexxProgram(wholenumber_t argCount, RxString *arguments,
        stringchar_t *filename, RxString *instore, stringchar_t *initialCmd,
        wholenumber_t, RexxExit *exits, RexxReturnCode *retCode, RxString *retValue);
    static void decodeConditionData(RexxDirectory *conditionObj, ConditionData *condData);

    static void initializeProcessEnvironment();
    static void terminateProcessEnvironment();
    static inline void addPendingUninit() { singleInstance->pendingUninits++; }
    static void processExternalHalt();

    static bool initialized;               // the interpreter is initialized
    static RexxDirectory *theEnvironment;  // the global environment object
    static RexxDirectory *theKernel;       // the global kernel environment

    static RexxDirectory *publicRoutines;  // the public routines directory
    static RexxDirectory *staticRequires;  // the statically loaded requires directory
    static RexxDirectory *commonRetrievers;  // the predefined rexx variable objects

    static RexxActivity *currentActivity;  // expose current activity object

    static SysMutex executionLock;         // the exclusive execution lock
    static SysMutex resourceLock;          // global control block mutex
    static SysMutex initializationLock;    // semaphore to protect startup/shutdown phases
    static SysSemaphore terminationSem;    // shutdown completion semaphore
    static SysCriticalSection criticalSection;   // critical section for thread sync
                                           // generic default settings structure
    static ACTIVATION_SETTINGS defaultSettings;
                                           // default active settings
    static ACTIVATION_SETTINGS *currentSettings;
    static bool DBCS_codepage;             // DBCS code page possible
    static uint32_t codepage;              // current activity codepage
    static stringchar_t DBCS_table[256];   // current DBCS vector table
    static ErrorMessage messageMappingTable[];   // mapping of error codes to message number

protected:
    static RexxInterpreter *singleInstance; // the single instance for this process

    static RexxObject *processID;           // the process identifier...also the process anchor point
    static RootImage  *rootImage;           // single cross process anchor point
    static int32_t     randomSeed;          // the random number randomizer

    // NON-static variable versions are in the singleton instance allocated from the
    // Rexx heap.  Garbage collection is performed on these objects

    RexxString      *versionID;             // the version identifier string.
    PackageManager  *packageManager;        // the package manager for this process.
    RexxDirectory   *cachedFunctions;       // the process-local function cache
    RexxObjectTable *globalReferences;      // our global reference table
    RexxObjectTable *instanceObjects;       // the table of instance objects
    RexxObjectTable *activityPool;          // pooled threads available to this activity.
    size_t           activityPoolSize;      // number of activities we're waiting to shutdown

    RexxObjectTable  *uninitTable;          // the uninit tables...units have process affinity
    size_t            pendingUninits;       // objects waiting to have uninits run
    bool              processingUninits;    // true when we are processing the uninit table
};


 inline wholenumber_t numberDigits() { return RexxInterpreter::currentSettings->digits; }
 inline wholenumber_t numberFuzz()   { return RexxInterpreter::currentSettings->fuzz; }
 inline bool          numberForm()   { return RexxInterpreter::currentSettings->form; }
 inline wholenumber_t numberFuzzyDigits() { return RexxInterpreter::currentSettings->digits - RexxInterpreter::currentSettings->fuzz; }

class CriticalBlock : public SynchronizedBlock
{
public:
    inline CriticalBlock() : SynchronizedBlock(RexxInterpreter::criticalSection) {;}
};

class ResourceSection
{
public:
    ResourceSection()
    {
        RexxInterpreter::resourceLock.request();
    }

    ~ResourceSection()
    {
        RexxInterpreter::resourceLock.release();
    }
};

class ExecutionSection
{
public:
    ExecutionSection()
    {
        RexxInterpreter::lockInterpreter();
    }

    ~ExecutionSection()
    {
        RexxInterpreter::unlockInterpreter();
    }
};

class InitializationSection
{
public:
    InitializationSection()
    {
        RexxInterpreter::initializationLock.request();
    }

    ~InitializationSection()
    {
        RexxInterpreter::initializationLock.release();
    }
};

#endif
