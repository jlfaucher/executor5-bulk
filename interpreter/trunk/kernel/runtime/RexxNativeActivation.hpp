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
/* REXX Kernel                                      RexxNativeActivation.hpp  */
/*                                                                            */
/* Primitive Native Activation Class Definitions                              */
/*                                                                            */
/******************************************************************************/
#ifndef Included_RexxNativeActivation
#define Included_RexxNativeActivation

#include "ActivationBase.hpp"
#include "RexxActivity.hpp"
#include "MethodClass.hpp"
#include "RexxActivity.hpp"
#include "ActivityState.hpp"
#include "ActivationApiContexts.hpp"

class ProtectedObject;
class RexxActivity;

#define MAX_NATIVE_ARGUMENTS 16

class RexxNativeActivation : public RexxActivationBase {
 public:
  typedef enum
  {
      PROGRAM_ACTIVATION,            // toplevel program entry
      METHOD_ACTIVATION,             // normal method call
      FUNCTION_ACTIVATION,           // function call activation
      EXIT_ACTIVATION,               // an exit activation
      EXTERNAL_FUNCTION,             // invoking a non-method external function
      EXTERNAL_COMMAND               // invoking an external command
  } ActivationType;


  inline void *operator new(size_t, void *ptr) {return ptr;}
  inline void  operator delete(void *, void *) {;}
  void *operator new(size_t);
  inline void operator delete(void *) {;}

  inline RexxNativeActivation(RESTORETYPE restoreType) { ; };
  inline RexxNativeActivation() {;};
         RexxNativeActivation(RexxObject *, RexxMethod *, RexxActivity *, RexxString *);
         RexxNativeActivation(RexxMethod *, RexxActivity *, RexxString *);
         RexxNativeActivation(RexxActivity *);
         RexxNativeActivation(RexxActivity *a, RexxActivation * c, ActivationType t);

  void live();
  void liveGeneral();
  void flatten(RexxEnvelope *);
  void run(size_t, RexxObject **, ProtectedObject &);
  void call(PNFUNCTION entryPoint, size_t count, RexxObject **list, ProtectedObject &result);
  void createLocalReference(RexxObject *);
  void removeLocalReference(RexxObject *);
  RexxVariableDictionary *methodVariables();
  char * cstring(RexxObject *);
  double getDoubleValue(RexxObject *);
  void  *cself();
  void  *buffer();
  void  *pointer(RexxObject *);
  void   dispatch(ProtectedObject &);
  RexxObject *getReceiver() {return  this->u_receiver;}
  void   traceBack(RexxList *);
  stringsize_t  digits();
  stringsize_t   fuzz();
  bool   form();
  void   setDigits(stringsize_t);
  void   setFuzz(stringsize_t);
  void   setForm(bool);
  void   guardOff();
  void   guardOn();
  bool   trap (RexxString *, RexxDirectory *);
  void   setErrorListener(RexxMessage *);

  inline void setConditionInfo(RexxDirectory *info) { conditionobj = info; }
  inline RexxDirectory *getConditionInfo() { return conditionobj; }
  inline void clearException() { conditionobj = OREF_NULL; }
  inline void   termination() { this->guardOff();}
  inline RexxMethod *getMethod()        {return this->method;}
  RexxSupplier *getAllContextVariables();
  void          processArguments(size_t argcount, RexxObject **arglist, uint16_t *argumentTypes, ValueDescriptor *descriptors, size_t maximumArgumentCount);
  void          reportSignatureError();
  void          reportStemError(size_t position, RexxObject *object);
  wholenumber_t wholeNumberValue(RexxObject *o, size_t position, wholenumber_t maxValue, wholenumber_t minValue);
  stringsize_t  unsignedNumberValue(RexxObject *o, size_t position, stringsize_t maxValue);
  int64_t       int64Value(RexxObject *o, size_t position);
  uint64_t      unsignedInt64Value(RexxObject *o, size_t position);
  RexxObject   *getContextStem(RexxString *name);
  RexxStem     *resolveStemVariable(RexxObject *s);
  RexxObject   *valueToObject(ValueDescriptor *value);
  bool          objectToValue(RexxObject *o, ValueDescriptor *value);

  void   resetNext();
  bool   fetchNext(RexxString **name, RexxObject **value);
  inline wholenumber_t  nextVariable()     {return this->nextvariable;}
  inline RexxStem   *nextStem()         {return this->nextstem;}
  inline RexxVariableDictionary *nextCurrent()     {return this->nextcurrent;}
  inline RexxCompoundElement *compoundElement() {return this->compoundelement; }
  inline void        setNextVariable(wholenumber_t value)           {this->nextvariable = value;}
  inline void        setNextCurrent(RexxVariableDictionary *vdict)     {this->nextcurrent = vdict;}
  inline void        setNextStem(RexxStem *localstem)     {this->nextstem = localstem;}
  inline void        setCompoundElement(RexxCompoundElement *element)     {this->compoundelement = element;}
  inline ActivityState getActivityState() { return nestedState; }
  inline void        setActivityState(ActivityState s) { nestedState = s; }
  inline void        setPreviousContext(RexxNativeActivation *previousApi, RexxActivation *previousStack)
  {
      previousApiContext = previousApi;
      previousStackFrame = previousStack;
  }
  inline RexxActivation *getPreviousStackFrame() { return previousStackFrame; }
  inline RexxNativeActivation *getPreviousApiContext() { return previousApiContext; }
         void checkConditions();
  RexxReturnCode copyValue(RexxObject * value, RxString * rxstring, RexxStringLength *length);
  RexxVariableBase *getVariableRetriever(RexxActivation *activation, RxString *name, bool direct);
  RexxVariableBase *getVariableRetriever(stringchar_t *name);
  wholenumber_t getContextVariable(RxString *name, RxString *value);
  wholenumber_t setContextVariable(RxString *name, RxString *value);
  wholenumber_t dropContextVariable(RxString *name);
  RexxObject *getContextVariable(stringchar_t *name);
  void setContextVariable(stringchar_t *name, RexxObject *value);
  void dropContextVariable(stringchar_t *name);
  RexxObject *getObjectVariable(stringchar_t *name);
  void setObjectVariable(stringchar_t *name, RexxObject *value);
  void dropObjectVariable(stringchar_t *name);
  RexxReturnCode variablePoolInterface(VariableRequest *requests);
  RexxVariableBase *variablePoolGetVariable(VariableRequest *pshvblock, bool symbolic);
  void variablePoolFetchVariable(VariableRequest *pshvblock);
  void variablePoolSetVariable(VariableRequest *pshvblock);
  void variablePoolDropVariable(VariableRequest *pshvblock);
  void variablePoolNextVariable(VariableRequest *pshvblock);
  void variablePoolFetchPrivate(VariableRequest *pshvblock);
  void variablePoolRequest(VariableRequest *pshvblock);
  RexxCallContext *getCurrentCallContext();
  RexxArray *getArguments();
  RexxObject *getArgument(size_t index);
  inline stringchar_t *getMessageName() { return msgname->getStringData(); }
  inline RexxObject *getSelf() { return u_receiver; }
  RexxObject *getSuper();
  RexxActivation *getContext() { return context; }

protected:
  RexxMethod     *method;              /* Method to run                     */
  RexxString     *msgname;             /* name of the message running       */
  RexxActivity   *activity;            /* current activity                  */
  RexxActivation *context;             // the calling context
  RexxObject    **arglist;             /* copy of the argument list         */
  RexxArray      *argArray;            /* optionally create argument array  */
  RexxObjectTable *savelist;           /* list of saved objects             */
  RexxMessage    *errorListener;       /* an object to notify if excep occur*/
  RexxObject     *result;              /* result from RexxRaise call        */
                                       /* running object variable pool      */
  RexxVariableDictionary *objectVariables;
  wholenumber_t   nextvariable;        /* next variable to retrieve         */
  RexxVariableDictionary *nextcurrent; /* current processed vdict           */
  RexxCompoundElement *compoundelement;/* current compound variable value   */
  RexxStem *      nextstem;            /* our working stem variable         */
  size_t          argcount;            /* size of the argument list         */
  int             object_scope;        /* reserve/release state of variables*/
  ActivationType  activationType;      // the type of activation
  RexxDirectory      *conditionobj;    // condition object for current context
                                       // the nested state for reentry
  ActivityState nestedState;
  RexxActivation *previousStackFrame;  // the Rexx stack frame before us
                                       // the previous native frame
  RexxNativeActivation *previousApiContext;
  CallContext callContext;             // a call context used for api reentries
};
#endif
