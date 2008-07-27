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
/* REXX Kernel                                                  IntegerClass.hpp     */
/*                                                                            */
/* Primitive Integer Class Definitions                                        */
/*                                                                            */
/******************************************************************************/
#ifndef Included_RexxInteger
#define Included_RexxInteger

#define INTEGERCACHESIZE 100
#define MAX_INTEGER_LENGTH 10

class RootImage;

class RexxInteger : public RexxObject {
 public:
  inline RexxInteger(RESTORETYPE restoreType) { ; };
  inline RexxInteger(wholenumber_t intValue) { this->value = intValue; this->hashvalue = 0; };

  inline void *operator new(size_t, void *ptr) {return ptr;}
  inline void  operator delete(void *, void *) {;}
  void *operator new(size_t);
  inline void operator delete(void *) {;}
  void live();
  void liveGeneral();
  void flatten(RexxEnvelope*);
  HashCode      hash();

  bool         numberValue(wholenumber_t *result, size_t precision);
  bool         numberValue(wholenumber_t *result);
  bool         unsignedNumberValue(stringsize_t *result, size_t precision);
  bool         unsignedNumberValue(stringsize_t *result);
  RexxNumberString *numberString();
  bool         doubleValue(double *result);
  RexxInteger *integerValue(stringsize_t);
  RexxString  *makeString();
  void         copyIntoTail(RexxCompoundTail *);
  RexxInteger *hasMethod(RexxString *);
  RexxString  *primitiveMakeString();
  RexxString  *stringValue();
  bool         isLogical();
  bool         truthValue(wholenumber_t);
  bool         isInstanceOf(RexxClass *);
  RexxMethod   *instanceMethod(RexxString *);
  RexxSupplier *instanceMethods(RexxClass *);

  bool        isEqual(RexxObject *);
  int         strictComp(RexxObject *);
  int         comp(RexxObject *other);

  RexxInteger *equal(RexxObject *);
  RexxInteger *strictEqual(RexxObject *);
  RexxInteger *notEqual(RexxObject *);
  RexxInteger *strictNotEqual(RexxObject *);
  RexxInteger *isGreaterThan(RexxObject *);
  RexxInteger *isLessThan(RexxObject *);
  RexxInteger *isGreaterOrEqual(RexxObject *);
  RexxInteger *isLessOrEqual(RexxObject *);
  RexxInteger *strictGreaterThan(RexxObject *);
  RexxInteger *strictLessThan(RexxObject *);
  RexxInteger *strictGreaterOrEqual(RexxObject *);
  RexxInteger *strictLessOrEqual(RexxObject *);

  RexxObject *unknown(RexxString *, RexxArray *);
  RexxObject *plus(RexxInteger *);
  RexxObject *minus(RexxInteger *);
  RexxObject *multiply(RexxInteger *);
  RexxObject *divide(RexxInteger *);
  RexxObject *integerDivide(RexxInteger *);
  RexxObject *remainder(RexxInteger *);
  RexxObject *power(RexxObject *);
  RexxObject *notOp();
  RexxObject *operatorNot(RexxObject *);
  RexxObject *andOp(RexxObject *);
  RexxObject *orOp(RexxObject *);
  RexxObject *xorOp(RexxObject *);

  RexxObject *abs();
  RexxObject *sign();
  RexxObject *Max(RexxObject **, size_t);
  RexxObject *Min(RexxObject **, size_t);
  RexxObject *trunc(RexxObject *);
  RexxObject *format(RexxObject *, RexxObject *, RexxObject *, RexxObject *);
  RexxObject *d2c(RexxObject *);
  RexxObject *d2x(RexxObject *);
  RexxObject *evaluate(RexxActivation *, RexxExpressionStack *);
  RexxObject *getValue(RexxActivation *);
  RexxObject *getRealValue(RexxActivation *);
  RexxObject *getValue(RexxVariableDictionary *);
  RexxObject *getRealValue(RexxVariableDictionary *);
  RexxString *concat(RexxString *);
  RexxString *concatBlank(RexxString *);
  void        setString(RexxString *string);
                                       /* numberstring operator forwarders  */
  koper (integer_operator_not)

  inline wholenumber_t wholeNumber() {return this->value;}
  inline wholenumber_t stringSize() {return (stringsize_t)this->value;}
  inline wholenumber_t incrementValue() {return ++this->value;}
  inline wholenumber_t decrementValue() {return --this->value;}
  inline RexxString *getStringrep() {return this->stringrep;}

  static PCPPM operatorMethods[];     // the primitive methods assigned to operators
protected:
  RexxString *stringrep;               /* integer string representation     */
  wholenumber_t value;                 /* actual integer value              */
};

class RexxIntegerClass : public RexxClass {
 public:
  RexxIntegerClass(RESTORETYPE restoreType) { ; };

  inline void *operator new(size_t, void *ptr) {return ptr;}
  inline void  operator delete(void *, void *) {;}
  void *operator new (size_t);
  inline void operator delete (void *) {;}
  void *operator new(size_t size, wholenumber_t size1, RexxBehaviour *classBehave, RexxBehaviour *instance) { return new (size, classBehave, instance) RexxClass; }
  inline void operator delete(void *, wholenumber_t, RexxBehaviour *, RexxBehaviour *) {;}
  RexxIntegerClass();
  inline RexxInteger *newCache(wholenumber_t value)
  {
      if (value >= 0 && value < INTEGERCACHESIZE)
      {
          return this->integercache[value];
      }
      else
      {
          return new RexxInteger (value);
      }
  }

  void live();
  void liveGeneral();
  static void createInstance(RootImage *);
  static void restoreInstance(RootImage *);
  static RexxIntegerClass *classInstance;   // the singleton instance
                                     /* array of fast aloocation integers 0-99      */
  RexxInteger *integercache[INTEGERCACHESIZE];

  static RexxInteger *falseObject;          // singleton integer objects
  static RexxInteger *trueObject;

  static RexxInteger *zero;                 // the readily accessible integer objects
  static RexxInteger *one;
  static RexxInteger *two;
  static RexxInteger *three;
  static RexxInteger *four;
  static RexxInteger *five;
  static RexxInteger *six;
  static RexxInteger *seven;
  static RexxInteger *eight;
  static RexxInteger *nine;
};

// object creation functions
inline RexxInteger *new_counter(wholenumber_t v)
{
    return new RexxInteger(v);
}

// object creation functions
inline RexxInteger *new_integer(wholenumber_t v)
{
    return TheIntegerClass->newCache(v);
}

#endif
