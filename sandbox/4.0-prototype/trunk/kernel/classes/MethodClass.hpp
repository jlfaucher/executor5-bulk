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
/* REXX Kernel                                                MethodClass.hpp */
/*                                                                            */
/* Primitive Kernel Method Class Definitions                                  */
/*                                                                            */
/******************************************************************************/
#ifndef Included_RexxMethod
#define Included_RexxMethod

#include "oorexx.h"
#include "RexxNativeCode.hpp"

#define PRIVATE_FLAG      0x01         /* private method                    */
#define UNGUARDED_FLAG    0x02         /* Method can run with GUARD OFF     */
#define INTERNAL_FLAG     0x04         /* method is part of saved image     */
#define REXX_CODE_METHOD  0x08         /* method is a REXX method           */
#define NATIVE_METHOD     0x10         /* method is a native C method       */
#define PROTECTED_FLAG    0x40         /* method is protected               */
#define KERNEL_CPP_METHOD 0x80         /* method is a kernel C++ meth       */

class RexxNativeCode;
class RexxCode;
class RexxClass;
class ProtectedObject;
class FunctionActivator;
class RexxInstruction;

// opaque kernel dispatching function
                                       /* pointer to method function        */
typedef RexxObject *  (VLARexxEntry *PMF)(RexxObject *,...);
                                       /* pointer to native method function */
typedef uint16_t *(RexxEntry *PNMF)(RexxMethodContext *, ValueDescriptor *);
                                       /* pointer to native function function*/
typedef uint16_t *(RexxEntry *PNFUNCTION)(RexxCallContext *, ValueDescriptor *);

/******************************************************************************/
/* Method arguments special codes                                             */
/******************************************************************************/

const int A_COUNT   = 127;            /* pass arguments as pointer/count pair */

#define MCPP   0                       /* C++ method start index            */
#define MSSCPP 0                       /* C++ class method start index      */


typedef struct _control {              /* meta date control info            */
    uint16_t magic;                    /* identifies as 'meta' prog         */
    uint16_t metaVersion;              /* version of the meta prog          */
    char     rexxVersion[40];          /* version of rexx intrpreter        */
    size_t   imageSize;                /* size of the method info           */
} FILE_CONTROL;                        /* saved control info                */

class RexxMethod : public RexxObject {
    friend class RexxActivation;
  public:
  inline void *operator new(size_t, void *ptr) {return ptr;}
  inline void  operator delete(void *, void *) {;}
  void *operator new(size_t);
  inline void operator delete(void *) {;}

  RexxMethod(RexxCode *);
  RexxMethod(RexxNativeCode *, RexxClass *scope = OREF_NULL);
  RexxMethod(size_t index, PCPPM entry, size_t argCount);
  inline RexxMethod(RESTORETYPE restoreType) { ; };

  void execute(RexxObject *, RexxObject *);
  void live();
  void liveGeneral();
  void flatten(RexxEnvelope*);
  RexxObject   *unflatten(RexxEnvelope *envelope);

  void          run(RexxActivity *,  RexxObject *, RexxString *,  size_t, RexxObject **, ProtectedObject &);
  void          call(RexxActivity *,  RexxString *,  RexxObject **, size_t, RexxString *, RexxString *, uint32_t, ProtectedObject &);
  void          callProgram(RexxActivity *activity, RexxString *routineName, RexxObject**arguments, size_t argcount, RexxString *calltype,
                    RexxString *environment, uint32_t context, ProtectedObject &result);
  void          callExternalRoutine(RexxActivity *activity, RexxString *routineName, RexxObject**arguments, size_t argcount,
                    RexxString *calltype, RexxString *environment, ProtectedObject &result);
  void          callNativeMethod(RexxActivity *activity, RexxString *routineName, RexxObject**arguments,
                    size_t argcount, ProtectedObject &result);
  void          callInternalRoutine(RexxActivity *activity, RexxInstruction *target, RexxObject**arguments, size_t argcount,
                    RexxActivation *parent, ProtectedObject &result);
  void          callInterpret(RexxActivity *activity, RexxActivation *parent, ProtectedObject &result, bool debug);
  RexxMethod   *newScope(RexxClass  *);
  void          setAttribute(RexxVariableBase *variable) {OrefSet(this, this->attribute, variable); }
  RexxVariableBase *getAttribute() { return this->attribute; };
  void          setScope(RexxClass  *);
  RexxSmartBuffer  *saveMethod();
  RexxBuffer  *save();
  RexxObject  *setUnGuardedRexx();
  RexxObject  *setGuardedRexx();
  RexxObject  *setPrivateRexx();
  RexxObject  *setProtectedRexx();
  RexxObject  *setSecurityManager(RexxObject *);
  FunctionActivator *getFunctionActivator();
  RexxString  *getRoutineName();
  void         setRoutineName(RexxString *);

   inline size_t methnum() {return this->methodInfo.methnum; };
   inline size_t arguments() {return this->methodInfo.arguments; };
   inline uint32_t  flags() {return this->methodInfo.flags; };
   inline void   setMethnum(size_t num) { this->methodInfo.methnum = (uint16_t)num; };
   inline void   setFlags(uint8_t newFlags) { this->methodInfo.flags = newFlags; };
   inline void   setArguments(size_t args) { this->methodInfo.arguments = (uint8_t)args; };

   inline bool   isGuarded()      {return (this->methodInfo.flags & UNGUARDED_FLAG) == 0; };
   inline bool   isInternal()     {return (this->methodInfo.flags & INTERNAL_FLAG) != 0; };
   inline bool   isPrivate()      {return (this->methodInfo.flags & PRIVATE_FLAG) != 0;}
   inline bool   isProtected()    {return (this->methodInfo.flags & PROTECTED_FLAG) != 0;}
   inline bool   isSpecial()      {return (this->methodInfo.flags & (PROTECTED_FLAG | PRIVATE_FLAG)) != 0;}

   inline bool   isRexxMethod()   {return (this->methodInfo.flags & REXX_CODE_METHOD) != 0; };
   inline RexxCode *getRexxCode()   { return (RexxCode *)this->code; }
   inline bool   isNativeMethod() {return (this->methodInfo.flags & NATIVE_METHOD) != 0; };
   inline bool   isCPPMethod()    {return (this->methodInfo.flags & KERNEL_CPP_METHOD) != 0; };

   inline void   setUnGuarded()    {this->methodInfo.flags |= UNGUARDED_FLAG;};
   inline void   setGuarded()      {this->methodInfo.flags &= ~UNGUARDED_FLAG;};
   inline void   setInternal()     {this->methodInfo.flags |= INTERNAL_FLAG;};
   inline void   setPrivate()      {this->methodInfo.flags |= (PRIVATE_FLAG | PROTECTED_FLAG);};
   inline void   setProtected()    {this->methodInfo.flags |= PROTECTED_FLAG;};
   inline void   setRexxMethod()   {this->methodInfo.flags |= REXX_CODE_METHOD; };
   inline void   setNativeMethod() {this->methodInfo.flags |= NATIVE_METHOD; };
   inline void   setCPPMethod()    {this->methodInfo.flags |= KERNEL_CPP_METHOD; };

   RexxInteger *Private() { return  (this->isPrivate() ? TheTrueObject : TheFalseObject); };
   inline RexxObject *getCode() {return (RexxObject *)this->code;}
   inline RexxClass  *getScope() {return this->scope;}

   static RexxMethod * createKernelMethod(PCPPM entryPoint, size_t arguments);
   static RexxMethod * createProtectedKernelMethod(PCPPM entryPoint, size_t arguments);
   static RexxMethod * createPrivateKernelMethod(PCPPM entryPoint, size_t arguments);
   static RexxMethod * getMacroCode(RexxString *name);
   static void         translateSource(stringchar_t *inputName, stringchar_t * outputName);
   static void         translateSource(stringchar_t *name, RxString *input, RxString * output);
   static void         loadRexxProgram(RexxString *name, ProtectedObject &method);
   static void         restoreProgram(RexxString *fileName, ProtectedObject &method);
          void         saveProgram(RexxString * fileName);
   static RexxMethod  *restoreProgramBuffer(RxString * inBuffer, RexxString * name);
   static RexxMethod  *restoreInstoreProgram(RxString *inBuffer, RexxString * name);
          void         saveProgramBuffer(RxString *outBuffer);
   static void         createMetaDescriptor(FILE_CONTROL *control, size_t bufferLength);
          void         saveTranslatedProgram(stringchar_t *file);
          RexxMethod  *newRexx(RexxObject **, size_t);
          RexxMethod  *newFileRexx(RexxString *);
   static RexxMethod  *newFile(RexxString *);

   static RexxMethod  *restoreSavedTranslatedProgram(RexxString *fileName, FILE *handle);
   static RexxMethod  *restoreTranslatedProgram(RexxString *fileName, FILE *handle);
   static RexxMethod  *processInstoreExecution(RxString *instoreSource, RxString *instoreImage, RexxString *name);
   static RexxMethod  *newRexxMethod(RexxSource *);
   static RexxMethod  *newRexxCode(RexxString *, RexxObject *, RexxObject *, RexxObject *a = OREF_NULL);
   static RexxMethod  *newRexxBuffer(RexxString *, RexxBuffer *);
   static RexxMethod  *newMethod(stringchar_t *, stringsize_t);
   static RexxMethod  *newEntry(PFN);
   static RexxMethod  *restore(RexxBuffer *, stringchar_t *);
   void setSource(RexxSource *);
   RexxSource *getSource() { return source; }
   RexxArray  *sourceRexx();
   RexxDirectory *getRoutines();
   RexxDirectory *getMethods();
   RexxDirectory *getClasses();
   inline PFN getNativeEntry() { return nativeCode->getEntry(); }
   RexxClass *resolveClass(RexxActivity *a, RexxString *name);
   static RexxClass *classInstance;    // the singleton class instance

protected:
   RexxClass  *scope;                  /* pointer to the method scope       */
   RexxVariableBase *attribute;        /* method attribute info             */
   union {
      RexxInternalObject *code;        /* associated "code" object          */
      RexxNativeCode     *nativeCode;  /* associated "code" object          */
      RexxCode           *rexxCode;    /* associated "code" object          */
   };
   PCPPM  cppEntry;                    /* C++ Method entry point.           */
   RexxSource *source;                 // the source object this method is associated with.

   static PCPPM exportedMethods[];     /* exported methods table            */
 };

#endif
