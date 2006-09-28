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
/* REXX Kernel                                                  ObjectClass.hpp  */
/*                                                                            */
/* Primitive Object Class Definitions                                         */
/*                                                                            */
/******************************************************************************/
/******************************************************************************/
#ifndef Included_RexxObject
#define Included_RexxObject

#include "Numerics.hpp"

/* default size of the scope table...these are normally very small */
#define DEFAULT_SCOPE_SIZE  6

#define getAttributeIndex 0            /* location of getAttribute method   */
#define setAttributeIndex 1            /* location of setAttribute method   */

#define koper(name) RexxObject *name(RexxObject *);

  class RexxObject;
  class RexxClass;
  class RexxEnvelope;
  class RexxCompoundTail;
  class RexxCompoundElement;
  class RexxInternalStack;
  class RexxVariableDictionary;
  class RexxArray;
  class RexxNumberString;
  class RexxInteger;
  class RexxBehaviour;
  class RexxMethod;
  class RexxVariableBase;
  class RexxSource;
  class RexxMessage;
  class RexxDirectory;
  class RexxList;
  class WeakReference;
  class RexxActivity;
  class RexxString;
  class ProtectedObject;

                                       /* pointer to method function        */
 typedef RexxObject *  (VLARexxEntry RexxObject::*PCPPM) (...);
 #define CPPM(n) ((PCPPM)&n)

#define LiveMask            0xFFFCu
#define MarkMask            0x0003u

/* behaviour (used on restoreimage).  Note that this is overloaded */
/* with the mark bits.  We don't need to use these at the same */
/* time.  The IsNonPrimitive bit on has meaning in the saved image. */
/* The mark bits only have meaning once the image has been */
/* restored. */
#define IsNonPrimitive  0x0001u
#define MarkBit1        0x0001u    /* Second of the mark bits           */
#define MarkBit2        0x0002u    /* Second of the mark bits           */
#define ProxiedObject   0x0004u    /* This requires a proxy             */
#define ProxyObject     0x0008u    /* This object is a PROXY(String) Obj*/
#define OldSpaceBit     0x0010u    /* location of the OldSpace bit      */
#define NoRefBit        0x0020u    /* location of No References Bit.    */

// class used to define header information
// this is limited to 32-bit of information so it is portable across
// 32- and 64-bit implementations
class HeaderInfo
{
public:
    uint16_t flags;           // state flags
};


class ObjectHeader
{
public:
    inline ObjectHeader & operator= (ObjectHeader &h)
    {
        // copy the relevant state
        objectSize = h.objectSize;
        info.flags = h.info.flags;
        return *this;
    }

    inline size_t size() { return (size_t)objectSize; }
    inline void setSize(size_t l) { objectSize = (uintptr_t)l; }

    inline void makeProxiedObject() { info.flags |= ProxiedObject; }
    inline bool requiresProxyObject() { return (info.flags & ProxiedObject) != 0; }
    inline void makeProxy() { info.flags |= ProxyObject; }
    inline bool isProxyObject() { return (info.flags & ProxyObject) != 0; }
    inline void clearObjectMark() { info.flags &= LiveMask; }
    inline void setObjectMark(uint16_t mark) { clearObjectMark(); info.flags |= mark; }
    inline bool isObjectMarked(uint32_t mark) { return (info.flags & mark) != 0; }
    inline bool isObjectLive(uint32_t mark) { return (info.flags & MarkMask) == mark; }
    inline bool isObjectDead(uint32_t mark) { return (info.flags & MarkMask) != mark; }
    inline void clear() { objectSize = 0; info.flags = 0; }
    inline void setOldSpace() { info.flags |= OldSpaceBit; }
    inline void clearOldSpace() { info.flags &= ~OldSpaceBit; }
    inline bool isOldSpace() { return (info.flags & OldSpaceBit) != 0; }
    inline void setNoReferences() { info.flags |= NoRefBit; }
    inline void setHasReferences() { info.flags &= ~NoRefBit; }
    inline bool hasReferences() { return (info.flags & NoRefBit) == 0; }
    inline bool hasNoReferences() { return (info.flags & NoRefBit) != 0; }
    inline void setNonPrimitive() { info.flags |= IsNonPrimitive; }
    inline void clearNonPrimitive() { info.flags &= ~IsNonPrimitive; }
    inline bool isNonPrimitive() { return (info.flags & IsNonPrimitive) != 0; }

protected:
    uintptr_t objectSize;              // allocated size of the object
    union
    {
        HeaderInfo info;               // the object flag/type information
        uintptr_t  sizePadding;        // padding to make sure this is a full pointer size
    };

};

  // structures used for hashValue union overloads
                                         /*  as "overLoading" of hashValue  */
  typedef struct {
    uint16_t typeNum;
    uint16_t behaviourFlags;
  } BEHAVIOURINFO;

  typedef struct {
    uint16_t methnum;                     /* kernel method number            */
    uint8_t  arguments;
    uint8_t  flags;                       /* flag information                */
  } METHODINFO;

  typedef struct {
    stringchar_t  type;                   /* name of the instruction           */
    uint8_t flags;                        /* general flag area                 */
    int16_t general;                      /* general reusable short value      */
  } INSTRUCTIONINFO;


  class RexxVirtualBase {              /* create first base level class     */
                                       /* dummy virtual function to force   */
                                       /* the virtual function table to a   */
                                       /* specific location.  Different     */
                                       /* compilers place the virtual       */
                                       /* function table pointer at         */
                                       /* different locations.  This forces */
                                       /* to the front location             */
  protected:
     virtual ~RexxVirtualBase() { ; }
     virtual void      baseVirtual() {;}
  };
                                       /* Base Object REXX class            */
  class RexxInternalObject : public RexxVirtualBase{
    public:

     inline void *operator new(size_t, void *ptr) {return ptr;}
     inline void  operator delete(void *, void *) {;}
     inline void  operator delete(void *) {;}
     void * operator new(size_t, RexxClass *);
     inline void operator delete(void *, RexxClass *) {;}
     void * operator new(size_t, RexxClass *, RexxObject **, size_t);
     inline void operator delete(void *, RexxClass *, RexxObject **, size_t) {;}
     inline RexxInternalObject() {;};
                                       /* Following constructor used to     */
                                       /*  reconstruct the Virtual          */
                                       /*  Functiosn table.                 */
                                       /* So it doesn't need to do anything */
     inline RexxInternalObject(RESTORETYPE restoreType) { ; };
     virtual ~RexxInternalObject() {;};

     inline operator RexxObject*() { return (RexxObject *)this; };
                                       /* the following are virtual         */
                                       /* functions required for every      */
                                       /* class                             */
     virtual void         live() {;}
     virtual void         liveGeneral() {;}
     virtual void         flatten(RexxEnvelope *) {;}

     virtual RexxObject  *unflatten(RexxEnvelope *) { return (RexxObject *)this; };
     virtual RexxObject  *makeProxy(RexxEnvelope *);
     virtual RexxObject  *copy();
     virtual RexxObject  *evaluate(RexxActivation *, RexxExpressionStack *) { return OREF_NULL; }
     virtual RexxObject  *getValue(RexxActivation *) { return OREF_NULL; }
     virtual RexxObject  *getRealValue(RexxActivation *) { return OREF_NULL; }
     virtual RexxObject  *getValue(RexxVariableDictionary *) { return OREF_NULL; }
     virtual RexxObject  *getRealValue(RexxVariableDictionary *) { return OREF_NULL; }
     virtual void         uninit() {;}
     virtual HashCode      hash()  { return this->hashvalue; }
             HashCode      getHashValue()  { return this->hashvalue; }

     virtual bool         isLogical();
     virtual bool         truthValue(wholenumber_t);
     virtual RexxString  *makeString();
     virtual void         copyIntoTail(RexxCompoundTail *buffer);
     virtual RexxString  *primitiveMakeString();
     virtual RexxArray   *makeArray();
     virtual RexxString  *stringValue();

     virtual bool         numberValue(wholenumber_t *result, size_t precision);
     virtual bool         numberValue(wholenumber_t *result);
     virtual bool         unsignedNumberValue(stringsize_t *result, size_t precision);
     virtual bool         unsignedNumberValue(stringsize_t *result);
     virtual bool         doubleValue(double *result);
     virtual RexxNumberString *numberString();
     virtual RexxInteger *integerValue(stringsize_t);
     virtual bool         numberValueNoNOSTRING(wholenumber_t *result) { return this->numberValue(result);};
     virtual bool         doubleValueNoNOSTRING(double *result) { return this->doubleValue(result);};

     virtual bool       isEqual(RexxObject *);

             RexxObject  *hasUninit();
             void         removedUninit();
             void         printObject();


     RexxBehaviour *behaviour;         /* the object's behaviour            */
     ObjectHeader   header;            /* memory management header          */
                                       /* Following defined as union to     */
                                       /*  Allow for overloading of         */
                                       /*  hashValue usage/value by         */
                                       /*  other classes.                   */
     union {
       HashCode hashvalue;             /* Default usage.                    */
       size_t u_size;                  /* used by many variable sized Objs  */
       BEHAVIOURINFO behaviourInfo;    /* Used by RexxBehaviour.            */
       METHODINFO    methodInfo;       /* Used by RexxMethod                */
       stringsize_t      lineNumber;   /* instruction line number info      */
       RexxMethod *u_method;           /* executed method - Activation      */
       RexxSource *u_source;           /* Source code object - RexxMethod   */
       RexxObject *u_receiver;         /* Receiver object - RexxNativeCode  */
       RexxObject *variableValue;      /* value of variable - OKVAR         */
                                       /* name of function/message          */
       RexxString *u_name;             /* Name for Stem Variable - RexxStem */
       RexxString *variableName;       /* retriever of indirect variable    */
       RexxString *stem;               /* name of stem - OTVSTEM            */
       RexxObject *u_left_term;        /* left term of the operator         */
                                       /* variable retriever                */
       RexxVariableBase *variableObject;
     };
  };


#define OREFSHIFT (sizeof(void *) - 1)
                                       /* generate hash value from OREF     */
inline uintptr_t HASHOREF(RexxInternalObject *r) { return ((uintptr_t)r) >> OREFSHIFT; }

class RootImage;

class RexxObject : public RexxInternalObject {
  public:
    inline void *operator new(size_t, void *ptr) {return ptr;}
    inline void  operator delete(void *, void *) {;}
    inline void  operator delete(void *) {;}

    void * operator new(size_t, RexxClass *);
    void * operator new(size_t, RexxClass *, RexxObject **, size_t);
    inline void   operator delete(void *, RexxClass *) { ; }
    inline void   operator delete(void *, RexxClass *, RexxObject **, size_t) { ; }

     static void createInstance(RootImage *rootImage);
     static void restoreInstance(RootImage *rootImage);
                                       // Followin are used to create new objects.
                                       // Assumed that the message is sent to a class Object
                                       // These may move to RexxClass in the future......
     RexxObject *newRexx(RexxObject **arguments, size_t argCount);
     RexxObject *newObject() {return new ((RexxClass *)this) RexxObject; };

     operator RexxInternalObject*() { return (RexxInternalObject *)this; };
     inline RexxObject(){;};
                                       /* Following constructor used to   */
                                       /*  reconstruct the Virtual        */
                                       /*  Functiosn table.               */
                                       /* So it doesn't need to do anythin*/
     inline RexxObject(RESTORETYPE restoreType) { ; };
     virtual ~RexxObject(){;};

     virtual RexxMethod  *methodObject(RexxString *);
     virtual RexxObject  *defMethod(RexxString *, RexxMethod *, RexxString *a = OREF_NULL);
     virtual RexxString  *defaultName();
     virtual RexxObject  *unknown(RexxString *msg, RexxArray *args){return OREF_NULL;};
     virtual RexxInteger *hasMethod(RexxString *msg);
             bool         hasMethod(stringchar_t *msg);
             bool         hasUninitMethod();

     RexxObject *init();
     void        uninit();
     void live();
     void liveGeneral();
     void flatten(RexxEnvelope *);
     RexxObject  *copy();
// see ObjectClass.c for defect description
#if defined(REXX_DEBUG)
     HashCode     hash() { fprintf(stderr,"*** RexxObject::hash called ***\n"); }
#endif
     bool         isLogical();
     bool         truthValue(wholenumber_t);
     virtual bool numberValue(wholenumber_t *result, size_t precision);
     virtual bool numberValue(wholenumber_t *result);
     virtual bool unsignedNumberValue(stringsize_t *result, size_t precision);
     virtual bool unsignedNumberValue(stringsize_t *result);
     bool         numberValueNoNOSTRING(wholenumber_t *result, size_t precision);
     bool         numberValueNoNOSTRING(wholenumber_t *result);
     RexxNumberString *numberString();
     bool         doubleValue(double *result);
     bool         doubleValueNoNOSTRING(double *result);
     RexxInteger *integerValue(stringsize_t);
     RexxString  *makeString();
     RexxString  *primitiveMakeString();
     void         copyIntoTail(RexxCompoundTail *buffer);
     RexxArray   *makeArray();
     RexxString  *stringValue();
     RexxString  *requestString();
     RexxString  *requestStringNoNOSTRING();
     RexxInteger *requestInteger(stringsize_t);
     inline RexxInteger *requestInteger() { return this->requestInteger(Numerics::DEFAULT_DIGITS); }
     bool         requestNumber(wholenumber_t *, stringsize_t);
     bool         requestNumber(wholenumber_t *);
     RexxArray   *requestArray();
     RexxString  *requiredString(size_t);
     RexxInteger *requiredInteger(size_t, stringsize_t);
     wholenumber_t requiredNumber(size_t, stringsize_t precision = Numerics::DEFAULT_DIGITS);
     stringsize_t  requiredPositive(size_t, stringsize_t precision = Numerics::DEFAULT_DIGITS);
     stringsize_t  requiredNonNegative(size_t, size_t precision = Numerics::DEFAULT_DIGITS);

     bool       isEqual(RexxObject *);
     RexxString  *objectName();
     RexxObject  *objectNameEquals(RexxObject *);
     RexxClass   *classObject();
     RexxObject  *setMethod(RexxString *, RexxMethod *, RexxString *a = OREF_NULL);
     RexxObject  *unsetMethod(RexxString *);
     RexxObject  *requestRexx(RexxString *);
     RexxMessage *start(RexxObject **, size_t);
     RexxString  *objectReference();
     RexxObject  *run(RexxObject **, size_t);

     void         messageSend(RexxActivity *, RexxString *, size_t, RexxObject **, ProtectedObject &);
     void         messageSend(RexxActivity *, RexxString *, size_t, RexxObject **, RexxObject *, ProtectedObject &);
     RexxMethod  *checkPrivate(RexxActivity *, RexxMethod *);
     void         processUnknown(RexxActivity *, RexxString *, size_t, RexxObject **, ProtectedObject &);
     void         processProtectedMethod(RexxActivity *, RexxString *, size_t, RexxObject **, ProtectedObject &);
     RexxObject  *sendMessage(RexxActivity *, RexxString *, RexxArray *);
     RexxObject  *sendMessage(RexxActivity *a, RexxString *message);
     RexxObject  *sendMessage(RexxActivity *a, RexxString *message, RexxObject **args, size_t argCount);
     RexxObject  *sendMessage(RexxActivity *a, RexxString *message, RexxObject *argument1);
     RexxObject  *sendMessage(RexxActivity *, RexxString *, RexxObject *, RexxObject *);
     RexxObject  *sendMessage(RexxActivity *, RexxString *, RexxObject *, RexxObject *, RexxObject *);
     RexxObject  *sendMessage(RexxActivity *, RexxString *, RexxObject *, RexxObject *, RexxObject *, RexxObject *);
     RexxObject  *sendMessage(RexxActivity *, RexxString *, RexxObject *, RexxObject *, RexxObject *, RexxObject *, RexxObject *);

     RexxObject  *sendMessage(RexxString *, RexxArray *);
     RexxObject  *sendMessage(RexxString *message);
     RexxObject  *sendMessage(RexxString *message, RexxObject **args, size_t argCount);
     RexxObject  *sendMessage(RexxString *message, RexxObject *argument1);
     RexxObject  *sendMessage(RexxString *, RexxObject *, RexxObject *);
     RexxObject  *sendMessage(RexxString *, RexxObject *, RexxObject *, RexxObject *);
     RexxObject  *sendMessage(RexxString *, RexxObject *, RexxObject *, RexxObject *, RexxObject *);
     RexxObject  *sendMessage(RexxString *, RexxObject *, RexxObject *, RexxObject *, RexxObject *, RexxObject *);

                                       // Following are internal OREXX methods
     RexxObject  *defMethods(RexxDirectory *);
     void         setObjectVariable(RexxString *, RexxObject *, RexxObject *);
     RexxObject  *getObjectVariable(RexxString *, RexxObject *);
     RexxObject  *setAttribute(RexxObject *);
     RexxObject  *getAttribute();
     void         addObjectVariables(RexxVariableDictionary *);
     void         copyObjectVariables(RexxObject *newObject);
     RexxObject  *superScope(RexxObject *);
     RexxMethod  *superMethod(RexxString *, RexxObject *);
     RexxObject  *mdict();
     RexxObject  *setMdict(RexxObject *);
     inline RexxBehaviour *behaviourObject() { return this->behaviour; }

     int16_t      ptype();
     stringchar_t *idString();
     RexxString  *id();
     RexxMethod  *methodLookup(RexxString *name );
     RexxVariableDictionary *getObjectVariables(RexxObject *);
     RexxObject  *equal(RexxObject *);
     RexxInteger *notEqual(RexxObject *other);
     RexxObject  *strictEqual(RexxObject *);
     RexxInteger *strictNotEqual(RexxObject *other);

     RexxString  *stringRexx();
     RexxObject  *makeStringRexx();
     RexxObject  *makeArrayRexx();
     RexxString  *defaultNameRexx();
     RexxObject  *copyRexx();
     RexxObject  *unknownRexx(RexxString *, RexxArray *);
     RexxObject  *hasMethodRexx(RexxString *);
     bool       callSecurityManager(RexxString *, RexxDirectory *);

 // Define operator methods here.

   koper  (operator_plus)
   koper  (operator_minus)
   koper  (operator_multiply)
   koper  (operator_divide)
   koper  (operator_integerDivide)
   koper  (operator_remainder)
   koper  (operator_power)
   koper  (operator_abuttal)
   koper  (operator_concat)
   koper  (operator_concatBlank)
   koper  (operator_equal)
   koper  (operator_notEqual)
   koper  (operator_isGreaterThan)
   koper  (operator_isBackslashGreaterThan)
   koper  (operator_isLessThan)
   koper  (operator_isBackslashLessThan)
   koper  (operator_isGreaterOrEqual)
   koper  (operator_isLessOrEqual)
   koper  (operator_strictEqual)
   koper  (operator_strictNotEqual)
   koper  (operator_strictGreaterThan)
   koper  (operator_strictBackslashGreaterThan)
   koper  (operator_strictLessThan)
   koper  (operator_strictBackslashLessThan)
   koper  (operator_strictGreaterOrEqual)
   koper  (operator_strictLessOrEqual)
   koper  (operator_lessThanGreaterThan)
   koper  (operator_greaterThanLessThan)
   koper  (operator_and)
   koper  (operator_or)
   koper  (operator_xor)
   koper  (operator_not)
                                       /* data converstion and validation   */
                                       /* routines                          */
   static void processNewArgs(RexxObject **, size_t, RexxObject ***, size_t *, size_t, RexxObject **, RexxObject **);

   RexxVariableDictionary *objectVariables;   /* set of object variables           */
   static RexxObject *nilObject;              // the nil object singleton
   static RexxClass  *classInstance;          // singleton class instance
   static PCPPM operatorMethods[];                  // object operator methods
};


/******************************************************************************/
/* Method pointer special types                                               */
/******************************************************************************/

 typedef RexxObject *  (RexxObject::*PCPPM0)();
 typedef RexxObject *  (RexxObject::*PCPPM1)(RexxObject *);
 typedef RexxObject *  (RexxObject::*PCPPM2)(RexxObject *, RexxObject *);
 typedef RexxObject *  (RexxObject::*PCPPM3)(RexxObject *, RexxObject *, RexxObject *);
 typedef RexxObject *  (RexxObject::*PCPPM4)(RexxObject *, RexxObject *, RexxObject *, RexxObject *);
 typedef RexxObject *  (RexxObject::*PCPPM5)(RexxObject *, RexxObject *, RexxObject *, RexxObject *, RexxObject *);
 typedef RexxObject *  (RexxObject::*PCPPM6)(RexxObject *, RexxObject *, RexxObject *, RexxObject *, RexxObject *, RexxObject *);
 typedef RexxObject *  (RexxObject::*PCPPM7)(RexxObject *, RexxObject *, RexxObject *, RexxObject *, RexxObject *, RexxObject *, RexxObject *);
 typedef RexxObject *  (RexxObject::*PCPPMA1)(RexxArray *);
 typedef RexxObject *  (RexxObject::*PCPPMC1)(RexxObject **, size_t);

 typedef int          (RexxObject::*DispatchMethod)(RexxActivity *, void *);


class RexxNilObject : public RexxObject {
  public:
     RexxNilObject();
     inline RexxNilObject(RESTORETYPE restoreType) { ; };

     RexxString * nilString();
};

#endif
