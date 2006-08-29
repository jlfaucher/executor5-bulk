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
/* REXX Kernel                                              RexxActivity.hpp  */
/*                                                                            */
/* Primitive Activity Class Definitions                                       */
/*                                                                            */
/******************************************************************************/
#ifndef RexxActivity_Included
#define RexxActivity_Included

#include "RexxThread.hpp"
#include "ListClass.hpp"
#include "RexxInternalStack.hpp"
#include "RexxActivationStack.hpp"
#include "ExpressionStack.hpp"
#include "RexxLocalVariables.hpp"
#include "ExitHandler.hpp"
#include "SysActivity.hpp"
#include "ExitHandler.hpp"
#include "ActivityTable.hpp"
#include "RexxNativeActivation.hpp"

#include "oorexx.h"

#define QUEUE_FIFO 1
#define QUEUE_LIFO 2

class InterpreterInstance;
class ProtectedObject;
class RequiresElement;
class RexxActivity;
class RexxMessage;
class RexxInstruction;
class RootImage;
class RexxNativeActivation;
class RexxClass;

typedef enum
{
    RecursiveStringError,              // a recursion problem in error handling
    FatalError,                        // bad problem
    UnhandledCondition                 // we had an unhandled condition.
} ActivityException;

#include "ActivationApiContexts.hpp"

// settings defined for activations
class ACTIVATION_SETTINGS {            /* activation "global" settings      */
    public:
      inline ACTIVATION_SETTINGS()
      {
          digits = Numerics::DEFAULT_DIGITS;
          fuzz = Numerics::DEFAULT_FUZZ;
          form = Numerics::DEFAULT_FORM;
          exmode = false;
      }

      stringsize_t digits;                 /* numeric digits setting            */
      stringsize_t fuzz;                   /* numeric fuzz setting              */
      bool form;                           /* numeric form setting              */
      bool exmode;                         /* DBCS string processing flag       */
};                                         /* global activation settings        */


#include "ActivityState.hpp"

                                       /* information must be saved and     */
                                       /* restored on nested entries to the */
                                       /* interpreter that use the same     */
                                       /* activity                          */
class NestedActivityInfo
{
public:
   char       *stackptr;               /* pointer to base of C stack        */
   int32_t     randomSeed;             /* random number seed                */
   bool        exitset;                /* halt/trace sys exit not set ==> 1 */
   ExitHandler exits[RXNOOFEXITS];     /* Array to hold system exits        */
};


                                       /* NOTE:  The following object       */
                                       /* definitions are only included in  */
                                       /* a module if the define            */
                                       /* INCL_ACTIVITY_DEFINITIONS is used */
                                       /* since they include data types that*/
                                       /* are not generally required (or    */
                                       /* available in other classes that   */
                                       /* might be using the activity class */
                                       /* methods                           */
 class RexxActivity : public RexxInternalObject
 {
  friend class ActivityTable;
  friend class RexxActivation;
  friend class RexxActivityManager;
  friend class ProtectedObject;
  friend class RequiresElement;
  public:

   typedef enum
   {
       PRIME_THREAD,                   // thread the interpreter instance was created on
       ATTACHED_THREAD,                // additional thread attached to the instance
       SPAWNED_THREAD                  // thread spawned via start or reply
   } ActivityType;


   typedef enum
   {
       NORMAL_TERMINATION = 0          // normal activity termination
   } ExitType;


   inline RexxActivity(RESTORETYPE restoreType) { ; };
   RexxActivity(bool, SysThread::ThreadPriority, InterpreterInstance *);

   inline void *operator new(size_t, void *ptr) {return ptr;}
   inline void  operator delete(void *, void *) {;}
   void *operator new(size_t);
   inline void  operator delete(void *) {;}

   void        live();
   void        liveGeneral();
   void        flatten(RexxEnvelope *);

   // main activity management methods
   void        run();
   void        runThread();
   void        unhandledCondition(RexxDirectory *);
   void        shutdown();

   // syncronization methods
   void        exitKernel(RexxNativeActivation::ActivationType);
   void        enterKernel();
   RexxObject *previous();
   void        waitReserve(RexxObject *);
   void        guardWait();
   void        guardPost();
   void        guardSet();
   void        checkDeadLock(RexxActivity *);
   void        postRelease();
   void        joinKernelQueue();
   void        relinquish();
   void        yield();
   void        releaseAccess();
   void        requestAccess();
   void        runProgram(RexxMethod *, RexxString *name, RexxString *calltype, RexxString *env, RexxObject **, size_t, ProtectedObject &);
   void        attachToInstance(SysThread::ThreadPriority priority, InterpreterInstance *instance);

   inline void setNextWaitingActivity(RexxActivity *next) { this->nextWaitingActivity = next; }
   inline void waitKernel() { this->runsem.wait(); }
   inline void clearWait()  { this->runsem.reset(); }
   inline bool isNested()   { return nestedCount > 1; }

   // activation stack management methods
   void        resetSettings();
   RexxActivation*  pushStackFrame();
   RexxActivation *popStackFrame(RexxActivation *);
   void        unwindStackFrame(RexxActivation *target);
   void        pushNativeActivation(RexxNativeActivation *);
   void        popNativeActivation(RexxNativeActivation *);
   void        pushNewContext();
   void        popContext();
   void        createMethodContext(MethodContext *context, RexxNativeActivation *owner);
   void        createCallContext(CallContext *context, RexxNativeActivation *owner);
   void        createExitContext(ExitContext *context, RexxNativeActivation *owner);
   void        createCallContext(CallContext *context);
   void        createExitContext(ExitContext *context);
   ActivityContext *getActivityContext();
   void        destroyCallContext(CallContext *context);
   void        destroyExitContext(ExitContext *context);
   void        releaseActivityContext(ActivityContext *context);

   // condition/error handling methods
   wholenumber_t error();
   bool        raiseCondition(RexxString *, RexxObject *, RexxObject *, RexxObject *, RexxObject *, RexxDirectory *);
   void        raiseException(wholenumber_t, LocationInfo *, RexxSource *, RexxString *, RexxArray *, RexxObject *);
   void        reportAnException(wholenumber_t, const stringchar_t *);
   void        reportAnException(wholenumber_t, const stringchar_t *, RexxObject *);
   void        reportAnException(wholenumber_t, const stringchar_t *, wholenumber_t);
   void        reportAnException(wholenumber_t, const stringchar_t *, wholenumber_t, RexxObject *);
   void        reportAnException(wholenumber_t, const stringchar_t *, RexxObject *, wholenumber_t);
   void        reportAnException(wholenumber_t, wholenumber_t);
   void        reportAnException(wholenumber_t, wholenumber_t, wholenumber_t);
   void        reportAnException(wholenumber_t, wholenumber_t, RexxObject *);
   void        reportAnException(wholenumber_t);
   void        reportAnException(wholenumber_t, RexxObject *);
   void        reportAnException(wholenumber_t, RexxObject *, RexxObject *);
   void        reportAnException(wholenumber_t, RexxObject *, RexxObject *, RexxObject *);
   void        reportAnException(wholenumber_t, RexxObject *, RexxObject *, RexxObject *, RexxObject *);
   void        reraiseException(RexxDirectory *);
   void        raisePropagate(RexxDirectory *);
   void        reportHalt(RexxString *description);
   void        reportNoMethod(RexxString *message, RexxObject *receiver);
   void        reportNoMethod(RexxObject *receiver);
   RexxObject *display(RexxDirectory *);
   RexxObject *displayDebug(RexxDirectory *);

   void        createLocalReference(RexxObject *obj);
   void        removeLocalReference(RexxObject *obj);
   RexxString *getCurrentQueue();
   RexxString *getDefaultCommandEnvironment();

   // thread-specific execution support
   void traceOutput(RexxActivation *, RexxString *);
   void sayOutput(RexxActivation *, RexxString *);
   void queue(RexxActivation *, RexxString *, int);
   RexxString *traceInput(RexxActivation *);
   RexxString *pullInput(RexxActivation *);
   RexxObject *lineOut(RexxString *);
   RexxString *lineIn(RexxActivation *);

   void        stackSpace();

   // Exit handling methods
   inline bool isExitSet() { return this->nestedInfo.exitset; }
   void queryTrcHlt();
   bool callExit(int function, int subfunction, void *exitbuffer, bool  enable);
   void callInitializationExit(RexxActivation *);
   void callTerminationExit(RexxActivation *);
   bool callSayExit(RexxActivation *, RexxString *);
   bool callTraceExit(RexxActivation *, RexxString *);
   bool callPullExit(RexxActivation *, RexxString **);
   bool callDebugPullExit(RexxActivation *, RexxString **);
   bool callFunctionExit(RexxActivation *, int exitid, RexxString *, RexxObject *, ProtectedObject &, RexxObject **, size_t);
   bool callSubcommandExit(RexxActivation *, RexxString *, RexxString *, RexxString **, ProtectedObject &);
   bool callQueuePullExit(RexxActivation *, RexxString **);
   bool callQueuePushExit(RexxActivation *, RexxString *, int);
   bool callQueueSizeExit(RexxActivation *, RexxInteger **);
   bool callQueueNameExit(RexxActivation *, RexxString **);
   bool callHaltTestExit(RexxActivation *);
   bool callHaltClearExit(RexxActivation *);
   bool callTraceTestExit(RexxActivation *, bool);
   bool callDebugTestExit(RexxActivation *, bool, bool);
   void callDebugSubroutineCall(RexxActivation *activation, bool enter);
   void callDebugLineLocate(RexxActivation *activation, RexxInstruction * instr);
   void callDebugSignal(RexxActivation *activation);
   bool callNovalueExit(RexxActivation *, RexxString *, RexxObject **);
   bool callValueExit(RexxString *, RexxString *, RexxObject**, RexxObject *);
   void terminateActivity(ExitType t);

   wholenumber_t messageSend(RexxObject *, RexxString *, size_t, RexxObject **, ProtectedObject &);
   wholenumber_t dispatchMethod(RexxObject *, DispatchMethod, void *);

   inline RexxDirectory *getLocalEnvironment() { return local; }
   RexxObject *getFromLocalEnvironment(RexxString *index);

   inline ACTIVATION_SETTINGS *getSettings () {return this->settings;}
          bool        checkRequiresStack(RexxString *program);
   inline void        setRandomSeed(int32_t seed) { this->nestedInfo.randomSeed = seed; };
          void        terminatePoolActivity();
          void        detachInstance();
          void        detach();
          void        activate();
          bool        deactivateActivity();
          void        deactivate();
          void        signalInstanceTermination();
          void        waitForInstanceTermination();
          void        startTerminationProcess();
          ActivityState setActivityState(ActivityState newState);

   inline void setExit(int exitNum, ExitHandler &exit)
   {
       this->nestedInfo.exits[exitNum] = exit;
   }

   inline ExitHandler &getExit(int exitNum)
   {
       return nestedInfo.exits[exitNum];
   }

   RexxDirectory *getConditionInfo();
   void setConditionInfo(RexxDirectory *);
   RexxCallContext *getCurrentCallContext();

   inline bool isExitEnabled(int exitNum) {return this->nestedInfo.exits[exitNum].isEnabled();}

   inline void clearExits() { memset((void *)&this->nestedInfo.exits, 0, sizeof(this->nestedInfo.exits)); }
   inline void saveNestedInfo(NestedActivityInfo *saveInfo) { memcpy((void *)saveInfo, (void *)&this->nestedInfo, sizeof(NestedActivityInfo)); }
   inline void restoreNestedInfo(NestedActivityInfo *saveInfo) { memcpy((void *)&this->nestedInfo, (void *)saveInfo, sizeof(NestedActivityInfo)); }
   inline void allocateStackFrame(RexxExpressionStack *stack, size_t entries)
   {
       stack->setFrame(frameStack.allocateFrame(entries), entries);
   }

   inline RexxObject **allocateFrame(size_t entries)
   {
       return frameStack.allocateFrame(entries);
   }

   inline void releaseStackFrame(RexxObject **frame)
   {
       frameStack.releaseFrame(frame);
   }

   inline void allocateLocalVariableFrame(RexxLocalVariables *locals)
   {
       locals->setFrame(frameStack.allocateFrame(locals->size));
   }

   inline RexxThreadContext *getThreadContext() { return &threadContext.threadContext; }
   inline RexxActivation *getCurrentContext()
   {
       return currentStackFrame;         // this is the current Rexx context
   }

   inline RexxNativeActivation *getApiContext()
   {
       return currentApiContext;         // this is the current Native activation
   }
   void clearException();

   void addGlobalReference(RexxObject *o);
   void removeGlobalReference(RexxObject *o);
   InterpreterInstance *getInstance() { return interpreter; }
   void setErrorListener(RexxMessage *);
   bool halt(RexxString *desc);
   bool setTrace(bool setting);
   bool isSameThread(RexxThread &t);
   RexxClass *resolveClass(RexxString *name);
   inline bool isQuiesced() { return state == QUIESCED; }
   inline void setRequestingString(bool s) { requestingString = s; stackcheck = !s; }

   inline void setLastMethod(RexxMethod *m) { lastMethod = m; }
   inline void setLastMethod(RexxString *n, RexxMethod *m) { lastMethod = m; lastMessageName = n; }
   inline void setLastMessageName(RexxString *n) { lastMessageName = n; }
   inline RexxMethod *getLastMethod() { return lastMethod; }
   inline RexxString *getLastMessageName() { return lastMessageName; }
   void initializeThreadInterface();

   static bool halt(RexxThread &id, RexxString *desc);
   static bool sysYield(RexxThread &id);
   static void processExternalHalt();
   static bool setTrace(RexxThread &id, bool setting);

   static void removeActivity(RexxActivity *a);
   static stringsize_t currentDigits();

   static void initializeInterfaces();
   static void yieldCurrentThread();

   ACTIVATION_SETTINGS *settings;      /* current activation setting values */

   SysActivity systemInfo;             // system-specific activity information


 protected:

   InterpreterInstance *interpreter;   // the interpreter instance we're attached to
   ActivityState        state;         // current state of the activity
   ActivityType         type;          // the usage type for the activity
   ActivityContext      threadContext; // the handed out activity context
   bool                 terminating;   // thread has been asked to terminate

   RexxActivationStack frameStack;     /* our stack used for activation frames */
   RexxDirectory      *local;          /* the local environment directory   */
                                       /* current REXX activation           */
   RexxActivation     *currentStackFrame;
   RexxNativeActivation *currentApiContext;   // top-most native activation
                                       // chain for the process mapping table
   RexxActivity       *nextProcessActivity;
                                       /* next element in the wait queue    */
   RexxActivity       *nextWaitingActivity;
   RexxObject         *waitingObject;  /* object activity is waiting on     */
   SysSemaphore  runsem;               /* activity run control semaphore    */
   RexxThread _thread;                 /* thread the activity's attached to */
   SysThread::ThreadPriority priority; /* activity priority value           */
   bool     stackcheck;                /* stack space is to be checked      */
   ExitType exitType;                  /* activity loop is to exit          */
   bool     requestingString;          /* in error handling currently       */
   SysSemaphore  guardsem;             /* guard expression semaphore        */
                                       /* system  on an Activity basis      */
   size_t   nestedCount;               /* extent of the nesting             */
   NestedActivityInfo nestedInfo;      /* info saved and restored on calls  */
   RexxString *lastMessageName;        /* last issued message               */
   RexxMethod *lastMethod;             /* last issued method object         */
   ProtectedObject *protectedObjects;  // list of stack-based object protectors
   RequiresElement *requiresElements;  // list of in-process requires file loading


   // structures containing the various interface vectors
   static RexxThreadInterface threadContextFunctions;
   static MethodContextInterface methodContextFunctions;
   static CallContextInterface callContextFunctions;
   static ExitContextInterface exitContextFunctions;
 };


 class RexxActivityManager : public RexxInternalObject
 {
  public:
   RexxActivityManager(RESTORETYPE restoreType) { ; };
   RexxActivityManager();

   inline void *operator new(size_t, void *ptr) {return ptr;}
   inline void  operator delete(void *, void *) {;}
   void *operator new(size_t);
   inline void  operator delete(void *) {;}

   void live();
   void liveGeneral();

   void addWaitingActivity(RexxActivity *a, bool release);
   inline RexxActivity *waitingActivity() { return this->firstWaitingActivity; }
   static RexxActivity *findActivityForCurrentThread();
   static RexxActivity *findActivity(RexxThread &t);
   static void createInstance(RootImage *rootImage);
   static void restoreInstance(RootImage *rootImage);
   static void addProcessActivity(RexxActivity *activity);
   static void removeProcessActivity(RexxActivity *activity);
   static RexxActivity *attachActivity(InterpreterInstance *instance);
   static RexxActivity *createActivity(InterpreterInstance *instance);

   static RexxActivityManager *singleInstance;  // singleton instance
   static RexxActivity *currentActivity;   // the currently active thread

protected:
                                       /* head of the waiting activity queue*/
   RexxActivity     *firstWaitingActivity;
                                       /* tail of the waiting activity queue*/
   RexxActivity     *lastWaitingActivity;
   static ActivityTable processActivities;  // the thread-to-activity mapping table
 };
                                       /* various exception/condition       */
                                       /* reporting routines                */
inline void reportCondition(RexxString *condition, RexxObject *description) { RexxActivityManager::currentActivity->raiseCondition(condition, OREF_NULL, description, OREF_NULL, OREF_NULL, OREF_NULL); }
inline void reportNovalue(RexxString *description) { reportCondition(OREF_NOVALUE, description); }
inline void reportNostring(RexxString *description) { reportCondition(OREF_NOSTRING, description); }

inline void reportException(wholenumber_t error)
{
    RexxActivityManager::currentActivity->reportAnException(error);
}

inline void reportException(wholenumber_t error, RexxArray *args)
{
    RexxActivityManager::currentActivity->raiseException(error, NULL, OREF_NULL, OREF_NULL, args, OREF_NULL);
}

inline void reportException(wholenumber_t error, RexxObject *a1)
{
    RexxActivityManager::currentActivity->reportAnException(error, a1);
}

inline void reportException(wholenumber_t error, wholenumber_t a1)
{
    RexxActivityManager::currentActivity->reportAnException(error, a1);
}

inline void reportException(wholenumber_t error, wholenumber_t a1, wholenumber_t a2)
{
    RexxActivityManager::currentActivity->reportAnException(error, a1, a2);
}

inline void reportException(wholenumber_t error, wholenumber_t a1, RexxObject *a2)
{
    RexxActivityManager::currentActivity->reportAnException(error, a1, a2);
}

inline void reportException(wholenumber_t error, const stringchar_t *a1, RexxObject *a2)
{
    RexxActivityManager::currentActivity->reportAnException(error, a1, a2);
}

inline void reportException(wholenumber_t error, const stringchar_t *a1)
{
    RexxActivityManager::currentActivity->reportAnException(error, a1);
}

inline void reportException(wholenumber_t error, const stringchar_t *a1, wholenumber_t a2)
{
    RexxActivityManager::currentActivity->reportAnException(error, a1, a2);
}

inline void reportException(wholenumber_t error, const stringchar_t *a1, wholenumber_t a2, RexxObject *a3)
{
    RexxActivityManager::currentActivity->reportAnException(error, a1, a2, a3);
}

inline void reportException(wholenumber_t error, const stringchar_t *a1, RexxObject *a2, wholenumber_t a3)
{
    RexxActivityManager::currentActivity->reportAnException(error, a1, a2, a3);
}

inline void reportException(wholenumber_t error, RexxObject *a1, RexxObject *a2)
{
    RexxActivityManager::currentActivity->reportAnException(error, a1, a2);
}

inline void reportException(wholenumber_t error, RexxObject *a1, RexxObject *a2, RexxObject *a3)
{
    RexxActivityManager::currentActivity->reportAnException(error, a1, a2, a3);
}

inline void reportException(wholenumber_t error, RexxObject *a1, RexxObject *a2, RexxObject *a3, RexxObject *a4)
{
    RexxActivityManager::currentActivity->reportAnException(error, a1, a2, a3, a4);
}

class ProtectedObject
{
friend class RexxActivity;
public:
    inline ProtectedObject() : protectedObject(OREF_NULL), next(NULL)
    {
        next = RexxActivityManager::currentActivity->protectedObjects;
        RexxActivityManager::currentActivity->protectedObjects = this;
    }

    inline ProtectedObject(RexxObject *o) : protectedObject(o), next(NULL)
    {
        next = RexxActivityManager::currentActivity->protectedObjects;
        RexxActivityManager::currentActivity->protectedObjects = this;
    }

    inline ProtectedObject(RexxInternalObject *o) : protectedObject((RexxObject *)o), next(NULL)
    {
        next = RexxActivityManager::currentActivity->protectedObjects;
        RexxActivityManager::currentActivity->protectedObjects = this;
    }

    inline ~ProtectedObject()
    {
        // remove ourselves from the list and give this object a
        // little hold protection.
        RexxActivityManager::currentActivity->protectedObjects = next;
        RexxMemory::memoryObject.holdObject(protectedObject);
    }

    inline ProtectedObject & operator=(RexxObject *o)
    {
        protectedObject = o;
        return *this;
    }

    // cast conversion operators for some very common uses of protected object.
    inline operator RexxObject *()
    {
        return protectedObject;
    }

    inline operator RexxString *()
    {
        return (RexxString *)protectedObject;
    }

    inline operator RexxMethod *()
    {
        return (RexxMethod *)protectedObject;
    }

    inline operator RexxArray *()
    {
        return (RexxArray *)protectedObject;
    }

    // this conversion helps the parsing process protect objects
    inline operator RexxInstruction *()
    {
        return (RexxInstruction *)protectedObject;
    }

    inline operator void *()
    {
        return (void *)protectedObject;
    }

protected:
    RexxObject *protectedObject;
    ProtectedObject *next;
};


class RequiresElement
{
friend class RexxActivity;
public:
    inline RequiresElement(RexxString *n) : name(n), next(NULL)
    {
        next = RexxActivityManager::currentActivity->requiresElements;
        RexxActivityManager::currentActivity->requiresElements = this;
    }
    inline ~RequiresElement()
    {
        // remove ourselves from the list
        RexxActivityManager::currentActivity->requiresElements = next;
    }

protected:
    RexxString *name;
    RequiresElement *next;
};

inline RexxActivity *contextActivity()
{
    return RexxActivityManager::currentActivity;
}

class UnsafeBlock
{
public:
    UnsafeBlock()
    {
        activity = RexxActivityManager::currentActivity;
        activity->releaseAccess();
    }

    ~UnsafeBlock()
    {
        activity->requestAccess();
    }
protected:
    RexxActivity *activity;
};


inline RexxActivity *new_activity(RexxActivity *activity)
{
    return new RexxActivity(true, SysThread::MEDIUM_PRIORITY, activity->getInstance());
}


class NativeContextBlock
{
public:
    inline NativeContextBlock (RexxThreadContext *context, RexxNativeActivation *n)
    {
        // save the activity and the activation context
        activity = ((ActivityContext *)context)->owningActivity;
        self = n;
        // we're entering the kernel now...kernel calls are now safe.
        activity->requestAccess();
    }

    inline NativeContextBlock (RexxThreadContext *context)
    {
        // save the activity and the activation context
        activity = ((ActivityContext *)context)->owningActivity;
        self = activity->getApiContext();
        // we're entering the kernel now...kernel calls are now safe.
        activity->requestAccess();
    }

    inline ~NativeContextBlock()
    {
        // release the kernel lock
        activity->releaseAccess();
    }

    inline RexxObject *protect(RexxObject *o)
    {
        self->createLocalReference(o);
        return o;
    }

    RexxNativeActivation *self;
    RexxActivity         *activity;
};

class ThreadApiBlock : public NativeContextBlock
{
    inline ThreadApiBlock(RexxThreadContext *c) : NativeContextBlock(c) { ; }
};

class MethodApiBlock : public NativeContextBlock
{
    inline MethodApiBlock(RexxMethodContext *c) : NativeContextBlock(c->threadContext, ((MethodContext *)c)->context) { ; }
};

class CallApiBlock : public NativeContextBlock
{
    inline CallApiBlock(RexxMethodContext *c) : NativeContextBlock(c->threadContext, ((CallContext *)c)->context) { ; }
};

class ExitApiBlock : public NativeContextBlock
{
    inline ExitApiBlock(RexxMethodContext *c) : NativeContextBlock(c->threadContext, ((ExitContext *)c)->context) { ; }
};

/**
 * Convert an API context to into the top native activation
 * context associated with the thread.
 *
 * @param c      The source API context.
 *
 * @return A Native activation context that is the anchor point for the
 *         API activity.
 */
inline RexxNativeActivation *contextToActivation(RexxThreadContext *c)
{
    return contextToActivity(c)->getApiContext();
}


/**
 * Convert an API context to into the top native activation
 * context associated with the thread.
 *
 * @param c      The source API context.
 *
 * @return A Native activation context that is the anchor point for the
 *         API activity.
 */
inline RexxNativeActivation *contextToActivation(RexxCallContext *c)
{
    return ((CallContext *)c)->context;
}


/**
 * Convert an API context to into the top native activation
 * context associated with the thread.
 *
 * @param c      The source API context.
 *
 * @return A Native activation context that is the anchor point for the
 *         API activity.
 */
inline RexxNativeActivation *contextToActivation(RexxExitContext *c)
{
    return ((ExitContext *)c)->context;
}


/**
 * Convert an API context to into the top native activation
 * context associated with the thread.
 *
 * @param c      The source API context.
 *
 * @return A Native activation context that is the anchor point for the
 *         API activity.
 */
inline RexxNativeActivation *contextToActivation(RexxMethodContext *c)
{
    return ((MethodContext *)c)->context;
}

#endif
