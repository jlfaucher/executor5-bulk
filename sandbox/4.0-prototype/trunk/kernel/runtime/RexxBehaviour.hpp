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
/* REXX Kernel                                                  RexxBehaviour.hpp   */
/*                                                                            */
/* Primitive Behaviour Class Definitions                                      */
/*                                                                            */
/*******************************************************************************/
#ifndef Included_RexxBehaviour
#define Included_RexxBehaviour

class RexxObjectTable;
class RexxTable;
                                       /* This flags is true once a         */
                                       /* Behaviour is copied               */
#define NON_PRIMITIVE_BEHAVIOUR 0x0001
#define ENHANCED_OBJECT         0x0002 /* this is an enhanced object        */
#define BEHAVIOUR_NOT_RESOLVED  0x0010
#define RESET_BEHAVIOUR_RESOLVED  0xFFEF
#define INTERNALCLASS (1 << ((sizeof(uintptr_t) * 8) - 1))

 class RexxBehaviour : public RexxInternalObject {
  public:

  inline void *operator new(size_t, void *ptr) {return ptr;}
  inline void  operator delete(void *, void *) {;}
  void *operator new(size_t, size_t);
  inline void operator delete(void *, size_t) {;}

  RexxBehaviour(ClassTypeCode t, PCPPM *);
  inline RexxBehaviour() {;};
  inline RexxBehaviour(RESTORETYPE restoreType) { ; };
  void live();
  void liveGeneral();
  void flatten(RexxEnvelope*);
  RexxObject *copy();
  RexxObject *define(RexxString *, RexxMethod *);
  void        addMethod(RexxString *, RexxMethod *);
  void        removeMethod(RexxString *);
  RexxMethod *methodObject(RexxString *);
  RexxMethod *methodLookup( RexxString *);
  RexxMethod *getMethod( RexxString *);
  RexxObject *deleteMethod(RexxString *);
  void restore(RexxBehaviour *);
  RexxClass  *restoreClass();
  RexxObject *superScope( RexxObject *);
  RexxMethod *superMethod(RexxString *, RexxObject *);
  void        setMethodDictionaryScope( RexxObject *);
  RexxObject *setScopes( RexxObjectTable *);
  RexxObject *addScope( RexxObject *);
  RexxObject *mergeScope( RexxObject *);
  bool        checkScope( RexxObject *);
  void        subclass(RexxBehaviour *);

  void merge( RexxBehaviour *);
  void methodDictionaryMerge( RexxTable *);


   inline RexxObjectTable  *getScopes()       { return this->scopes; };
   inline RexxTable  *getMethodDictionary()   { return this->methodDictionary; };
   inline void        setMethodDictionary(RexxTable * m) { OrefSet(this, this->methodDictionary, m); };
   inline RexxTable  *getInstanceMethodDictionary()   { return this->instanceMethodDictionary; };
   inline void        setInstanceMethodDictionary(RexxTable * m) { OrefSet(this, this->instanceMethodDictionary, m); };
   inline RexxClass  *getCreateClass()        { return this->createClass;};
   inline void        setClass(RexxClass *c)  { OrefSet(this, this->createClass,  c); };
   inline ClassTypeCode typenum()             { return (ClassTypeCode)this->behaviourInfo.typeNum; };
   inline uint16_t flags()                    { return this->behaviourInfo.behaviourFlags; };
   inline void  setFlags(uint16_t v)         { this->behaviourInfo.behaviourFlags = v; };
   inline void  setTypenum(ClassTypeCode typenumber) { this->behaviourInfo.typeNum = (uint16_t)typenumber; };

   inline bool  isPrimitiveBehaviour()    {  return ! this->isNonPrimitiveBehaviour(); };
   inline bool  isNonPrimitiveBehaviour() {  return (this->behaviourInfo.behaviourFlags & NON_PRIMITIVE_BEHAVIOUR) != 0; };
   inline bool  isBehaviourResolved()     {  return ! this->isBehaviourNotResolved(); };
   inline bool  isBehaviourNotResolved()  {  return (this->behaviourInfo.behaviourFlags & BEHAVIOUR_NOT_RESOLVED) != 0; };
   inline bool  isEnhanced()              {  return (this->behaviourInfo.behaviourFlags & ENHANCED_OBJECT) != 0; };
   inline void  setBehaviourResolved()    {  this->behaviourInfo.behaviourFlags &= RESET_BEHAVIOUR_RESOLVED; };
   inline void  setBehaviourNotResolved() {  this->behaviourInfo.behaviourFlags |= BEHAVIOUR_NOT_RESOLVED; };
   inline void  setEnhanced()             {  this->behaviourInfo.behaviourFlags |= ENHANCED_OBJECT; };
   inline void  setNonPrimitiveBehaviour(){  this->behaviourInfo.behaviourFlags |= NON_PRIMITIVE_BEHAVIOUR; };
   inline void  resolveNonPrimitiveBehaviour()
   {
       if (this->isBehaviourNotResolved() ){
                                       /* Nope, turn off not resolved       */
           this->setBehaviourResolved();
                                       /* Resolve address of                */
                                       /*  all kernel methods for behav     */
                                       /*  all oper   methods for behav     */
           this->operatorMethods = primitiveBehaviours[this->typenum()].operatorMethods;
       }
   }
   inline RexxBehaviour *getSavedPrimitiveBehaviour()
   {
       uintptr_t behaviourID = (uintptr_t)this->typenum();
       // if this is an internal class, normalize this so we can
       // restore this to the correct value if we add additional internal classes.
       if (behaviourID > T_Last_Exported_Class)
       {
           behaviourID -= T_Last_Exported_Class;
           behaviourID |= INTERNALCLASS;
       }
       return (RexxBehaviour *)behaviourID;
   }

   static inline RexxBehaviour *restoreSavedPrimitiveBehaviour(RexxBehaviour *b)
   {
       uintptr_t behaviourID = (uintptr_t)b;
       // if this is an internal class, we need to convert back to
       // the relative internal class id
       if (behaviourID & INTERNALCLASS)
       {
           behaviourID &= ~INTERNALCLASS;    // turn off the internal marker
           behaviourID += T_Last_Exported_Class; // turn back into an internal class
       }
       return &primitiveBehaviours[behaviourID];          // translate back into proper behaviour
   }
   static void buildVirtualFunctionTable();

   RexxObjectTable  *scopes;           /* scopes table                      */
   RexxTable  *methodDictionary;       /* method dictionary                 */
   PCPPM      *operatorMethods;        /* operator look-a-side table        */
   RexxClass  *createClass;            /* class that created this object    */
                                       /* methods added via SETMETHOD       */
   RexxTable  *instanceMethodDictionary;
                                       /* table of primitive behaviours     */
   static RexxBehaviour primitiveBehaviours[T_Last_Primitive_Class + 1];
   static void *virtualFunctions[T_Last_Primitive_Class + 1];  /* table of virtual functions        */
 };



// object creation function
inline RexxBehaviour *new_behaviour(size_t typenum)
{
    return new (typenum) RexxBehaviour;
}

/******************************************************************************/
/* Typed method invocation macros                                             */
/******************************************************************************/


inline RexxObject * callOperatorMethod(RexxObject *object, size_t methodOffset, RexxObject *argument)
{
  PCPPM cppEntry;                      /* kernel method entry point         */

                                       /* get the entry point               */
  cppEntry = object->behaviour->operatorMethods[methodOffset];
                                       /* go issue the method               */
  return (object->*((PCPPM1)cppEntry))(argument);
}
                                       /* current object's behaviour        */
#define objectType(r) (((RexxObject *)(r))->behaviour)
#define objectTypeNumber(r) (objectType(r)->typenum())
                                       /* check the object type             */
#define isOfClass(t,r) (objectType(r) == The##t##Behaviour)
#define hasTypeNumber(t, r) (objectTypeNumber(r) == T_##t)
#define isSameType(me, you) (objectType(me) == objectType(you))

// some very common class tests
inline bool isString(RexxObject *o) { return isOfClass(String, o); }
inline bool isInteger(RexxObject *o) { return isOfClass(Integer, o); }
inline bool isArray(RexxObject *o) { return isOfClass(Array, o); }
inline bool isStem(RexxObject *o) { return isOfClass(Stem, o); }
inline bool isActivation(RexxObject *o) { return isOfClass(Activation, o); }
inline bool isMethod(RexxObject *o) { return isOfClass(Method, o); }

/* assign a new behaviour */
inline void BehaviourSet(RexxInternalObject *o, RexxBehaviour *b) { o->behaviour = b; }
inline void *new_object(size_t s, RexxBehaviour *b)
{
    RexxObject *o = new_object(s);
    BehaviourSet(o, b);
    return (void *)o;
}

/* restore/set the VirtualFunction table for the specified Object */
inline void setVirtualFunctions(void *o, uint32_t t) { (*((void **)o) = RexxBehaviour::virtualFunctions[t]); }
inline void setObjectVirtualFunctions(void *o) { (*((void **)o) = RexxBehaviour::virtualFunctions[T_Object]); }

/******************************************************************************/
/* Function:  Test for primitive method status of an object                   */
/******************************************************************************/
inline bool isPrimitive(RexxObject *object) { return object->behaviour->isPrimitiveBehaviour(); }

/******************************************************************************/
/* Class object creation macro                                                */
/******************************************************************************/

#define createClassObject(c,t)            The##c##Class = new (sizeof(t), The##c##ClassBehaviour, The##c##Behaviour) t

#endif
