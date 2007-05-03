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
/* REXX Kernel                                                  ClassClass.hpp   */
/*                                                                            */
/* Primitive Class Class Definitions                                          */
/*                                                                            */
/******************************************************************************/
#ifndef Included_RexxClass
#define Included_RexxClass

#define REXX_DEFINED      0x00000001    /* this class is a native rexx class */
#define IMPORTED          0x00000002    /* this class was imported to som    */
#define MIXIN             0x00000004    /* this is a mixin class             */
#define HAS_UNINIT        0x00000008    /* this class has an uninit method   */
#define META_CLASS        0x00000010    /* this class is a meta class        */
#define PRIMITIVE_CLASS   0x00000020    /* this is a primitive class         */
#define PARENT_HAS_UNINIT 0x00000040

 class RexxPointer;
 class RootImage;

 class RexxClass : public RexxObject {
  public:
   inline RexxClass(){;};
   inline RexxClass(RESTORETYPE restoreType) { ; };

   inline void  operator delete(void *) {;}
   inline void *operator new(size_t, void *ptr) {return ptr;}
   inline void  operator delete(void *, void *) {;}
   void *operator new(size_t, size_t, RexxBehaviour *, RexxBehaviour *);
   inline void  operator delete(void *, size_t, RexxBehaviour *, RexxBehaviour *) {;}
   void live();
   void liveGeneral();
   void flatten(RexxEnvelope*);
   RexxObject *unflatten(RexxEnvelope*);
   RexxObject *makeProxy(RexxEnvelope*);
   bool        isEqual(RexxObject *);

   RexxObject * equal(RexxObject *);
   RexxObject * strictEqual(RexxObject *);
   RexxObject * notEqual(RexxObject *);
   RexxObject * setRexxDefined();
   RexxInteger *queryMixinClass();
   RexxString  *getId();
   RexxClass   *getBaseClass();
   RexxClass   *getMetaClass();
   RexxClass   *getSuperClass();
   RexxArray   *getSuperClasses();
   RexxArray   *getSubClasses();
   void         defmeths(RexxTable *);
   void         setInstanceBehaviour(RexxBehaviour *);
   RexxTable  *getInstanceBehaviourDictionary();
   RexxTable  *getBehaviourDictionary();
   RexxString *defaultName();
   void        subClassable(stringchar_t *, bool);
   RexxObject *defineMethod(RexxString *, RexxMethod *);
   RexxObject *defineMethods(RexxTable *);
   RexxObject *deleteMethod(RexxString *);
   RexxMethod *method(RexxString *);
   RexxSupplier *methods(RexxClass *);
   void        updateSubClasses();
   void        updateInstanceSubClasses();
   void        createClassBehaviour(RexxBehaviour *);
   void        createInstanceBehaviour(RexxBehaviour *);
   void        methodDictionaryMerge(RexxTable *, RexxTable *);
   RexxTable  *methodDictionaryCreate(RexxTable *, RexxClass *);
   RexxObject *inherit(RexxClass *, RexxClass *);
   RexxObject *uninherit(RexxClass *);
   RexxObject *enhanced(RexxObject **, size_t);
   RexxClass  *mixinclass(RexxString *, RexxClass *, RexxTable *);
   RexxClass  *subclass(RexxString *, RexxClass *, RexxTable *);
   RexxClass  *newRexx(RexxObject **args, size_t argCount);
   void        setMetaClass(RexxClass *);
   bool        instanceOf(RexxObject *o);
   bool        isCompatibleWith(RexxClass *other);
   RexxObject *isSubclassOf(RexxClass *other);

   inline bool         rexxDefined() { return (this->class_info & REXX_DEFINED) != 0; };
   inline bool         queryMixin()  { return (this->class_info & MIXIN) != 0; };
   inline bool         queryMeta()   { return (this->class_info & META_CLASS) != 0; };
   inline bool         uninitDefined()   { return (this->class_info & HAS_UNINIT) != 0; };
   inline bool         parentUninitDefined()   { return (this->class_info & PARENT_HAS_UNINIT) != 0; };
   inline void         parentHasUninit()   { this->class_info |= PARENT_HAS_UNINIT; };
   inline bool         isPrimitive() { return (this->class_info & PRIMITIVE_CLASS) != 0; }
   inline void         setMixinClass() { this->class_info |= MIXIN; }
   inline void         setNotPrimitive(){ this->class_info &= ~PRIMITIVE_CLASS; };
   inline RexxBehaviour *getInstanceBehaviour() {return this->instanceBehaviour;};
   inline void         setMeta() { this->class_info |= META_CLASS; }
          void         addSubClass(RexxClass *);
          void         removeSubclass(RexxClass *c);

                                       /* Subclassable and subclassed       */
    RexxString    *id;                 /* classes will have a name string   */
                                       /* class methods specific to this    */
                                       /* class                             */
    RexxTable     *classMethodDictionary;
                                       /* instances of this class inherit   */
    RexxBehaviour *instanceBehaviour;  /* this behaviour                    */
                                       /* methods added to this class       */
    RexxTable     *instanceMethodDictionary;
    RexxClass     *baseClass;          /* Baseclass of this class           */
    RexxArray     *metaClass;          /* Metaclass of this class           */
                                       /* Metaclass mdict                   */
    RexxArray     *metaClassMethodDictionary;
    RexxObjectTable *metaClassScopes;  /* Metaclass scopes                  */
                                       /* The superclass and any inherited  */
    RexxArray     *classSuperClasses;  /* mixins for class behaviour        */
                                       /* The superclass and any inherited  */
                                       /* mixins for instance behaviour     */
    RexxArray     *instanceSuperClasses;

    RexxList      *subClasses;         // our list of weak referenced subclasses
                                       /* class specific information        */
                                       /* defines for this field are at the */
    uint32_t class_info;               /* top of this header file           */

    // create the singleton instance
    static void createInstance(RootImage *rootImage);
    static RexxClass *classInstance;   // the singleton instance of the Class class

 };



                                       /* declare a class creation routine  */
                                       /* for classes with their own        */
                                       /* explicit class objects            */
#define CreateClassObject(name, className) Rexx##name::classInstance = (className *)new (0, The##name##ClassBehaviour, The##name##Behaviour) RexxClass;
                                       /* restore a class from its          */
                                       /* associated primitive behaviour    */
                                       /* (already restored by memory_init) */
#define RestoreClassObject(name, className) Rexx##name::classInstance = (className *)The##name##ClassBehaviour->restoreClass();
#endif
