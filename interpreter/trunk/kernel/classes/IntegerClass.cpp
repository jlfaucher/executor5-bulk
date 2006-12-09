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
/* REXX Kernel                                                  IntegerClass.c       */
/*                                                                            */
/* Primitive Integer Class                                                    */
/*                                                                            */
/******************************************************************************/
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "RexxCore.h"
#include "StringClass.hpp"
#include "ArrayClass.hpp"
#include "RexxActivation.hpp"
#include "RexxActivity.hpp"
#include "ArgumentUtilities.hpp"
#include "Numerics.hpp"
#include "Interpreter.hpp"

RexxInteger *RexxIntegerClass::falseObject = OREF_NULL;          // singleton integer objects
RexxInteger *RexxIntegerClass::trueObject = OREF_NULL;

RexxInteger *RexxIntegerClass::zero = OREF_NULL;                 // the readily accessible integer objects
RexxInteger *RexxIntegerClass::one = OREF_NULL;
RexxInteger *RexxIntegerClass::two = OREF_NULL;
RexxInteger *RexxIntegerClass::three = OREF_NULL;
RexxInteger *RexxIntegerClass::four = OREF_NULL;
RexxInteger *RexxIntegerClass::five = OREF_NULL;
RexxInteger *RexxIntegerClass::six = OREF_NULL;
RexxInteger *RexxIntegerClass::seven = OREF_NULL;
RexxInteger *RexxIntegerClass::eight = OREF_NULL;
RexxInteger *RexxIntegerClass::nine = OREF_NULL;
                                       /* define an operator forwarding     */
                                       /* method                            */
#define string_forwarder_cpp(method)\
RexxObject *RexxInteger::##method(RexxObject *operand)\
 {\
     return (RexxObject *)this->string()->method(operand);\
 }


int RexxInteger::comp(RexxObject *other)
/******************************************************************************/
/* Function:  compare the value of two integers                               */
/******************************************************************************/
{
    requiredArg(other, ONE);             /* make sure this is really there    */
                                         /* able to compare here?             */
    if (isSameType(this, other) && numberDigits() == Numerics::DEFAULT_DIGITS)
                                         /* just return the difference        */
      return (int)(this->value - ((RexxInteger *)other)->value);
    else                                 /* do a numberstring compare         */
      return this->numberString()->comp(other);
  }

HashCode RexxInteger::hash()
/******************************************************************************/
/* Function:  retrieve the hash value of an integer object                    */
/******************************************************************************/
{
  RexxString * string;                 /* integer string value              */

  if (!isInteger(this))                /*  a nonprimitive object?           */
                                       /* see if == overridden.             */
    return this->sendMessage(OREF_STRICT_EQUAL)->requestString()->hash();
  else {
    if (this->hashvalue == 0) {        /* no hash generated yet?            */
      string = this->stringValue();    /* generate the string value         */
                                       /* get the string's hash value       */
      this->hashvalue = string->getStringHash();
    }
    return this->getHashValue();       /* return the string hash            */
  }
}

void RexxInteger::live()
/******************************************************************************/
/* Function:  Normal garbage collection live marking                          */
/******************************************************************************/
{
  setUpMemoryMark
  memory_mark(this->objectVariables);
  memory_mark(this->stringrep);
  cleanUpMemoryMark
}

void RexxInteger::liveGeneral()
/******************************************************************************/
/* Function:  Generalized object marking                                      */
/******************************************************************************/
{
  setUpMemoryMarkGeneral
  memory_mark_general(this->objectVariables);
  memory_mark_general(this->stringrep);
  cleanUpMemoryMarkGeneral
}

void RexxInteger::flatten(RexxEnvelope *envelope)
/******************************************************************************/
/* Function:  Flatten an object                                               */
/******************************************************************************/
{
  setUpFlatten(RexxInteger)

   flatten_reference(newThis->objectVariables, envelope);
   flatten_reference(newThis->stringrep, envelope);

  cleanUpFlatten
}

RexxString *RexxInteger::makeString()
/******************************************************************************/
/* Function:  Handle a REQUEST('STRING') request for a REXX integer object    */
/******************************************************************************/
{
  return this->stringValue();          /* return the string value           */
}

RexxInteger *RexxInteger::hasMethod(RexxString *methodName)
/******************************************************************************/
/* Function:  Handle a HASMETHOD request for an integer                       */
/******************************************************************************/
{
                                       /* return the string value's answer  */
  return this->stringValue()->hasMethod(methodName);
}

RexxString *RexxInteger::primitiveMakeString()
/******************************************************************************/
/* Function:  Handle a REQUEST('STRING') request for a REXX integer object    */
/******************************************************************************/
{
  stringchar_t  stringBuffer[24];      /* integer formatting buffer         */
  RexxString *string;                  /* string value                      */

  if (this->stringrep != OREF_NULL)    /* have a string already?            */
    return this->stringrep;            /* return it directly                */
                                       /* convert value into string         */
  Numerics::formatWholeNumber(this->wholeNumber(), stringBuffer);
                                       /* return as a string                */
  string = new_string(stringBuffer);
                                       /* cache this away for later         */
  OrefSet(this, this->stringrep, string);
  SetObjectHasReferences(this);        /* we now have references            */
  return string;                       /* return the new string             */
}

RexxString *RexxInteger::stringValue()
/******************************************************************************/
/* Function:  Return the string value for an integer                          */
/******************************************************************************/
{
  stringchar_t stringBuffer[24];       /* integer formatting buffer         */
  RexxString *string;                  /* string value                      */

  if (this->stringrep != OREF_NULL)    /* have a string already?            */
    return this->stringrep;            /* return it directly                */
                                       /* convert value into string         */
  Numerics::formatWholeNumber(this->wholeNumber(), stringBuffer);
                                       /* return as a string                */
  string = new_string(stringBuffer);
                                       /* cache this away for later         */
  OrefSet(this, this->stringrep, string);
  SetObjectHasReferences(this);        /* we now have references            */
  return string;                       /* return the new string             */
}

void RexxInteger::copyIntoTail(RexxCompoundTail *tail)
/******************************************************************************/
/* Function:  Copy the value of an integer into a compound variable name      */
/******************************************************************************/
{
  stringchar_t stringBuffer[24];       /* integer formatting buffer         */

  if (this->stringrep != OREF_NULL) {  /* have a string already?            */
      /* copying directly from an existing string rep is faster */
      /* than formatting a new value and copying. */
      tail->append(stringrep->getStringData(), stringrep->getLength());
      return;
  }
  Numerics::formatWholeNumber(this->wholeNumber(), stringBuffer);
                                       /* append this to the buffer         */
  tail->append((stringchar_t *)stringBuffer, strlen((char *)stringBuffer));
}

bool RexxInteger::doubleValue(double *value)
/******************************************************************************/
/* Function:  Convert a string value to a double value;                       */
/******************************************************************************/
{
    // just let the compiler convert
    *value = (double)this->wholeNumber();
    return true;
}

RexxNumberString *RexxInteger::numberString()
/******************************************************************************/
/* Function:  Convert an integer into a numberstring value                    */
/******************************************************************************/
{
  if (this->stringrep != OREF_NULL)    /* have a cached string value?       */
                                       /* use its numberstring value        */
    return this->stringrep->numberString();
  else                                 /* create a new numberstring         */
    return new_numberstring(this->wholeNumber());
}

bool RexxInteger::numberValue(wholenumber_t *result)
/******************************************************************************/
/* Function:  Convert an integer object to a long value under the current     */
/*            precision.                                                      */
/******************************************************************************/
{
    // get the digits setting we use
    stringsize_t digits = Numerics::DEFAULT_DIGITS;
    // get the value for this
    wholenumber_t value = this->wholeNumber();
                                       /* is the long value expressable as a*/
                                       /*  whole number in REXX term.       */
    if (digits < Numerics::DEFAULT_DIGITS && Numerics::abs(value) >= (wholenumber_t)(digits * 10))
    {
        return false;                    /* nope, not a valid long.           */
    }
    *result = value;                     // return the value
    return true;                         // this was convertable
}

bool RexxInteger::numberValue(wholenumber_t *result, stringsize_t digits)
/******************************************************************************/
/* Function:  Convert an integer object to a long value under the current     */
/*            precision.                                                      */
/******************************************************************************/
{
    // get the value for this
    wholenumber_t value = this->wholeNumber();
                                       /* is the long value expressable as a*/
                                       /*  whole number in REXX term.       */
    if (digits < Numerics::DEFAULT_DIGITS && Numerics::abs(value) >= (wholenumber_t)(digits * 10)) {
        return false;                      /* nope, not a valid long.           */
    }
    *result = value;                     // return the value
    return true;                         // this was convertable
}

bool RexxInteger::unsignedNumberValue(stringsize_t *result)
/******************************************************************************/
/* Function:  Convert an integer object to a long value under the current     */
/*            precision.                                                      */
/******************************************************************************/
{
    // get the value for this
    stringsize_t value = this->stringSize();
                                       /* is the long value expressable as a*/
                                       /*  whole number in REXX terms.      */
    if (value >= (stringsize_t)Numerics::MAX_WHOLENUMBER)
    {
        return false;                    /* nope, not a valid long.           */
    }

    *result = value;                     // return the value
    return true;                         // this was convertable
}

bool RexxInteger::unsignedNumberValue(stringsize_t *result, stringsize_t digits)
/******************************************************************************/
/* Function:  Convert an integer object to a long value under the current     */
/*            precision.                                                      */
/******************************************************************************/
{
    // get the value for this
    stringsize_t value = this->stringSize();
                                       /* is the long value expressable as a*/
                                       /*  whole number in REXX term.       */
    if (digits < Numerics::DEFAULT_DIGITS && value >= (stringsize_t)(digits * 10))
    {
        return false;                      /* nope, not a valid long.           */
    }
    *result = value;                     // return the value
    return true;                         // this was convertable
}

RexxInteger *RexxInteger::integerValue(
    size_t digits)                     /* required precision (ignored)      */
/******************************************************************************/
/* Function:  Convert an integer to an integer (real easy!)                   */
/******************************************************************************/
{
  return this;                         /* just return directly              */
}

void RexxInteger::setString(
    RexxString *string )               /* new string value                  */
/******************************************************************************/
/* Function:  Add a string value to the string look-a-side.                   */
/******************************************************************************/
{
                                       /* set the strign                    */
   OrefSet(this, this->stringrep, string);
   SetObjectHasReferences(this);       /* we now have references            */
}



/**
 * Test if the object is a value logical value as defined
 * for IF, WHEN, etc.
 *
 * @return Returns true if this is the integer 0 or 1, false otherwise.
 */
bool RexxInteger::isLogical()
{
    return this->value == 0 || this->value == 1;
}


bool RexxInteger::truthValue(
    wholenumber_t errorcode )          /* error to raise if not good        */
/******************************************************************************/
/* Function:  Determine the truth value of an integer object                  */
/******************************************************************************/
{
  if (this->value == 0)                /* have a zero?                      */
    return false;                      /* this is false                     */
  else if (this->value != 1)           /* how about a one?                  */
    reportException(errorcode, this);  /* report the error                  */
  return true;                         /* this is true                      */
}

/******************************************************************************/
/* Function:   Macro to forward a method against the numberstring value of    */
/*             an integer object.                                             */
/******************************************************************************/

#define integer_forward(s,m,o) ((s)->numberString()->m(o))

RexxObject *RexxInteger::unknown(
    RexxString *msgname,               /* unknown message name              */
    RexxArray *arguments)              /* arguments to the unknown message  */
/******************************************************************************/
/* Function:  Intercept unknown messages to an integer object and reissue     */
/*            them against the string value.                                  */
/******************************************************************************/
{
                                       /* just reissue this                 */
  return this->stringValue()->sendMessage(msgname, arguments);
}

RexxString *RexxInteger::concatBlank(
    RexxString *other )                /* other object for concatenation    */
/******************************************************************************/
/* Function:  Concatenate an object to an integer                             */
/******************************************************************************/
{
  requiredArg(other, ONE);            /* this is required                  */
  other = REQUEST_STRING(other);       /* ensure a string value             */
                                       /* do the concatenate                */
  return this->stringValue()->concatWith(other, ' ');
}

RexxString *RexxInteger::concat(
    RexxString *other )                /* other object for concatenation    */
/******************************************************************************/
/* Function:  Concatenate an object to an integer                             */
/******************************************************************************/
{
  requiredArg(other, ONE);            /* this is required                  */
  other = REQUEST_STRING(other);       /* ensure a string value             */
                                       /* do the concatenate                */
  return this->stringValue()->concat(other);
}

RexxObject *RexxInteger::plus(
    RexxInteger *other)                /* target other object               */
/******************************************************************************/
/* Function:  Add an integer to another object                                */
/******************************************************************************/
{
  wholenumber_t tempVal;               /* addition result                   */

                                       /* are we using default digits?      */
  if (numberDigits() != Numerics::DEFAULT_DIGITS )
                                       /* nope, we can't do integer arith   */
    return integer_forward(this, plus, other);
  if (other == OREF_NULL)              /* unary                             */
    return this;                       /* just return ourselves             */
  else {                               /* binary                            */
    if (isInteger(other)) {       /* adding two integers together?     */
                                       /* add the numbers                   */
      tempVal = this->value + other->value;
                                       /* result still within range?        */
      if (tempVal <= Numerics::MAX_WHOLENUMBER && tempVal >= Numerics::MIN_WHOLENUMBER)
        return new_integer(tempVal);   /* return as an integer number       */
    }
                                       /* need to do full arithmetic        */
    return integer_forward(this, plus, other);
  }
}

RexxObject *RexxInteger::minus(
    RexxInteger *other)                /* target other object               */
/******************************************************************************/
/* Function:  Subtract another object from an integer                         */
/******************************************************************************/
{
  wholenumber_t tempVal;               /* subtraction result                */

                                       /* are we using default digits?      */
  if (numberDigits() != Numerics::DEFAULT_DIGITS )
                                       /* nope, then we can do integer arith*/
    return integer_forward(this, minus, other);

  if (other == OREF_NULL) {            /* unary subtraction operator        */
    tempVal = -this->value;            /* negate the current number         */
    return new_integer(tempVal);       /* and return a new integer          */
  }
  else {                               /* binary subtraction operation      */
    if (isInteger(other)) {       /* subtracting two integers?         */
                                       /* subtract the numbers              */
      tempVal = this->value - other->value;
                                       /* result still within range?        */
      if (tempVal <= Numerics::MAX_WHOLENUMBER && tempVal >= Numerics::MIN_WHOLENUMBER)
        return new_integer(tempVal);   /* return as an integer number       */
    }
                                       /* need to do full arithmetic        */
    return integer_forward(this, minus, other);
  }
}

RexxObject *RexxInteger::multiply(
    RexxInteger *other)                /* target other object               */
/******************************************************************************/
/* Function:  Multiply an integer by another object                           */
/******************************************************************************/
{
  wholenumber_t otherval;              /* long value of other object        */
  wholenumber_t tempVal;               /* temp result value                 */

                                       /* are we using default digits?      */
  if (numberDigits() != Numerics::DEFAULT_DIGITS )
                                       /* nope, we can't do integer math    */
    return integer_forward(this, multiply, other);
  requiredArg(other, ONE);            /* make sure the argument is there   */
                                       /* is the other an integer and will  */
                                       /* the result be in a good range?    */
  if (isInteger(other) && Numerics::abs(this->value) <= 99999 &&
      Numerics::abs((otherval = other->value)) <= 9999) {
    tempVal = this->value * otherval;  /* multiply directly                 */
    return new_integer(tempVal);       /* and return as an integer          */
  }
  else                                 /* do the slow way                   */
    return integer_forward(this, multiply, other);
}

RexxObject *RexxInteger::divide(
    RexxInteger *other)                /* target other object               */
/******************************************************************************/
/* Function:  Perform division (actually done as numberstring math)           */
/******************************************************************************/
{
                                       /* just forward this                 */
  return integer_forward(this, divide, other);
}

RexxObject *RexxInteger::integerDivide(
    RexxInteger *other)                /* target other object               */
/******************************************************************************/
/* Function:  Perform integer division                                        */
/******************************************************************************/
{
  wholenumber_t otherval;              /* long value of the other object    */
  wholenumber_t tempVal;               /* temporary result value            */

                                       /* are we using default digits?      */
  if (numberDigits() != Numerics::DEFAULT_DIGITS )
                                       /* nope, we can't do integer arith   */
    return integer_forward(this, integerDivide, other);
  requiredArg(other, ONE);            /* make sure this is really there    */

  if (isInteger(other)) {         /* is right object an integer?       */
                                       /* is right number 0?                */
    if ((otherval = other->value) != 0) {
      tempVal = this->value / otherval;/* nope, do the division....         */
      return new_integer(tempVal);     /* and return as an integer object   */
    }
    else                               /* yes, raise error.                 */
      reportException(Error_Overflow_zero);
  }
                                       /* not integer, forward to           */
                                       /*numberstring.                      */
  return integer_forward(this, integerDivide, other);
}

RexxObject *RexxInteger::remainder(
    RexxInteger *other)                /* target other object               */
/******************************************************************************/
/* Function:  Perform remainder division                                      */
/******************************************************************************/
{
  wholenumber_t otherval;              /* long value of the other object    */
  wholenumber_t tempVal;               /* temporary result value            */

                                       /* are we using default digits?      */
  if (numberDigits() != Numerics::DEFAULT_DIGITS )
                                       /* nope, we can't do integer arith   */
    return integer_forward(this, remainder, other);
  requiredArg(other, ONE);            /* make sure this is really there    */

  if (isInteger(other)) {         /* is right object an integer?       */
                                       /* is right number 0?                */
    if ((otherval = other->value) != 0) {
      tempVal = this->value % otherval;/* nope, do the division....         */
      return new_integer(tempVal);     /* and return as an integer object   */
    }
    else                               /* yes, raise error.                 */
      reportException(Error_Overflow_zero);
  }
                                       /* not integer, forward to           */
                                       /*numberstring.                      */
  return integer_forward(this, remainder, other);
}

RexxObject *RexxInteger::power(
    RexxObject *other)                 /* power exponent value              */
/******************************************************************************/
/* Function:  Process integer power operator (actually just a forwarder)      */
/******************************************************************************/
{
                                       /* just send along                   */
  return integer_forward(this, power, other);
}

bool RexxInteger::isEqual(
    RexxObject *other)                 /* other comparison object           */
/******************************************************************************/
/* Function:  Primitive strict equal\not equal method.  This determines       */
/*            only strict equality, not greater or less than values.          */
/******************************************************************************/
{
  if (!isPrimitive(this))              /* not a primitive?                  */
                                       /* do the full lookup compare        */
    return this->sendMessage(OREF_STRICT_EQUAL, other)->truthValue(Error_Logical_value_method);

  if (isInteger(other))           /* two integers?                     */
                                       /* just directly compare the values  */
    return this->value == ((RexxInteger *)other)->value;
                                       /* go do a string compare            */
  return this->stringValue()->isEqual(other);
}

int  RexxInteger::strictComp(
    RexxObject *other)                 /* other comparison value            */
/******************************************************************************/
/* Function:  Compare the two values.                                         */
/*                                                                            */
/*  Returned:  return <0 if other is greater than this                        */
/*             return  0 if this equals other                                 */
/*             return >0 if this is greater than other                        */
/******************************************************************************/
{
  requiredArg(other, ONE);            /* make sure this is really there    */
  if (isInteger(other))           /* string compare is simple          */
                                       /* just return their difference      */
    return (int)(this->value - ((RexxInteger *)other)->value);
  else                                 /* go do a string compare            */
    return this->stringValue()->strictComp((RexxString *)other);
}

RexxInteger *RexxInteger::strictEqual(
    RexxObject *other)                 /* other comparison value            */
/******************************************************************************/
/* Function:  Perform the primitive level "==" compare, including the hash    */
/*            value processing.                                               */
/******************************************************************************/
{
  HashCode hash;                       /* retrieved hash                    */

  if (other == OREF_NULL) {            /* asking for the hash value?        */
    hash = this->hash();               /* get the hash value                */
                                       /* create a string value             */
    return (RexxInteger *)new_string((stringchar_t *)&hash, sizeof(HashCode));
  }
  else
    return this->strictComp(other) == 0 ? TheTrueObject : TheFalseObject;
}

RexxInteger *RexxInteger::strictNotEqual(
    RexxObject *other)                 /* other comparison value            */
/******************************************************************************/
/* Function:  Strict inequality operation                                     */
/******************************************************************************/
{
                                       /* return strict compare result      */
  return (this->strictComp(other) != 0) ? TheTrueObject : TheFalseObject;
}

RexxInteger *RexxInteger::equal(
    RexxObject *other)                 /* other comparison value            */
/******************************************************************************/
/* Function:  non-strict "=" operator                                         */
/******************************************************************************/
{
  return this->comp(other) == 0 ? TheTrueObject : TheFalseObject;
}

RexxInteger *RexxInteger::notEqual(
    RexxObject *other)                 /* other comparison value            */
/******************************************************************************/
/* Function:  non-strict "\=" operator                                        */
/******************************************************************************/
{
  return this->comp(other) != 0 ? TheTrueObject : TheFalseObject;
}

RexxInteger *RexxInteger::isGreaterThan(
    RexxObject *other)                 /* other comparison value            */
/******************************************************************************/
/* Function:  non-strict ">" operator                                         */
/******************************************************************************/
{
  return this->comp(other) > 0 ? TheTrueObject : TheFalseObject;
}

RexxInteger *RexxInteger::isLessThan(
    RexxObject *other)                 /* other comparison value            */
/******************************************************************************/
/* Function:  non-strict "<" operator                                         */
/******************************************************************************/
{
  return this->comp(other) < 0 ? TheTrueObject : TheFalseObject;
}

RexxInteger *RexxInteger::isGreaterOrEqual(
    RexxObject *other)                 /* other comparison value            */
/******************************************************************************/
/* Function:  non-strict ">=" operator                                        */
/******************************************************************************/
{
  return this->comp(other) >= 0 ? TheTrueObject : TheFalseObject;
}

RexxInteger *RexxInteger::isLessOrEqual(
    RexxObject *other)                 /* other comparison value            */
/******************************************************************************/
/* Function:  non-strict "<=" operator                                        */
/******************************************************************************/
{
  return this->comp(other) <= 0 ? TheTrueObject : TheFalseObject;
}


RexxInteger *RexxInteger::strictGreaterThan(
    RexxObject *other)                 /* other comparison value            */
/******************************************************************************/
/* Function:  strict ">>" operator                                            */
/******************************************************************************/
{
  return this->strictComp(other) > 0 ? TheTrueObject : TheFalseObject;
}

RexxInteger *RexxInteger::strictLessThan(
    RexxObject *other)                 /* other comparison value            */
/******************************************************************************/
/* Function:  strict "<<" operator                                            */
/******************************************************************************/
{
  return this->strictComp(other) < 0 ? TheTrueObject : TheFalseObject;
}

RexxInteger *RexxInteger::strictGreaterOrEqual(
    RexxObject *other)                 /* other comparison value            */
/******************************************************************************/
/* Function:  strict ">>=" operator                                           */
/******************************************************************************/
{
  return this->strictComp(other) >= 0 ? TheTrueObject : TheFalseObject;
}

RexxInteger *RexxInteger::strictLessOrEqual(
    RexxObject *other)                 /* other comparison value            */
/******************************************************************************/
/* Function:  strict "<<=" operator                                           */
/******************************************************************************/
{
  return this->strictComp(other) <= 0 ? TheTrueObject : TheFalseObject;
}

RexxObject *RexxInteger::notOp()
/******************************************************************************/
/* Function:  Perform the logical not of an integer object                    */
/******************************************************************************/
{
                                       /* perform the operation             */
  return this->truthValue(Error_Logical_value_method) ? TheFalseObject : TheTrueObject;
}

RexxObject *RexxInteger::operatorNot(
    RexxObject *dummy )                /* dummy for polymorphic operators   */
/******************************************************************************/
/* Function:  Perform the logical not of an integer object                    */
/******************************************************************************/
{
                                       /* perform the operation             */
  return this->truthValue(Error_Logical_value_method) ? TheFalseObject : TheTrueObject;
}

RexxObject *RexxInteger::andOp(
    RexxObject *other)                 /* other logical value               */
/******************************************************************************/
/* Function:  Logically AND two objects together                              */
/******************************************************************************/
{
  RexxObject *otherTruth;              /* truth value of the other object   */

  requiredArg(other, ONE);            /* make sure the argument is there   */
                                       /* validate the boolean              */
  otherTruth = other->truthValue(Error_Logical_value_method) ? TheTrueObject : TheFalseObject;
                                       /* perform the operation             */
  return (!this->truthValue(Error_Logical_value_method)) ? TheFalseObject : otherTruth;
}

RexxObject *RexxInteger::orOp(
    RexxObject *other)                 /* other logical value               */
/******************************************************************************/
/* Function:  Logically OR two objects together                               */
/******************************************************************************/
{
  RexxObject *otherTruth;              /* truth value of the other object   */

  requiredArg(other, ONE);            /* make sure the argument is there   */
                                       /* validate the boolean              */
  otherTruth = other->truthValue(Error_Logical_value_method) ? TheTrueObject : TheFalseObject;
                                       /* perform the operation             */
  return (this->truthValue(Error_Logical_value_method)) ? TheTrueObject : otherTruth;
}

RexxObject *RexxInteger::xorOp(
    RexxObject *other)                 /* other logical value               */
/******************************************************************************/
/* Function:  Logically XOR two objects together                              */
/******************************************************************************/
{
  bool truth;                          /* converted other argument          */

  requiredArg(other, ONE);            /* make sure the argument is there   */
                                       /* get as a boolean                  */
  truth = other->truthValue(Error_Logical_value_method);
                                       /* first one false?                  */
  if (!this->truthValue(Error_Logical_value_method))
                                       /* value is always the second        */
    return truth ? TheTrueObject : TheFalseObject;
  else                                 /* value is inverse of second        */
    return (truth) ? TheFalseObject : TheTrueObject;
}

RexxObject *RexxInteger::abs()
/******************************************************************************/
/* Function:  Take the absolute value of an integer object                    */
/******************************************************************************/
{
                                       /* working under the default digits? */
 if (numberDigits() == Numerics::DEFAULT_DIGITS) {
     /* if we're already positive, this is a quick return */
     if (value >= 0) {
         return this;
     }
     return new_integer(-value);       /* return as an integer object       */
 }
 else
   return this->numberString()->abs(); /* do a numberstring operation       */
}

RexxObject *RexxInteger::sign()
/******************************************************************************/
/* Function:  SIGN() function on an integer object                            */
/******************************************************************************/
{
  RexxObject *result;                  /* returned result                   */

 if (this->value > 0)                  /* positive number?                  */
   result = IntegerOne;                /* result is "1"                     */
 else if (this->value < 0)             /* negative number?                  */
  result = new_integer(-1);            /* result is "-1"                    */
 else
  result = IntegerZero;                /* exactly zero                      */
 return result;                        /* return the value                  */
}

RexxObject *RexxInteger::Max(
    RexxObject **args,                 /* array of comparison values        */
    size_t argCount)                   /* count of arguments                */
/******************************************************************************/
/* Function:  Perform MAX function on integer objects                         */
/******************************************************************************/
{
  wholenumber_t  value;                /* current working value             */
  wholenumber_t  maxvalue;             /* current maximum                   */
  size_t       arg;                    /* current arg position              */
  RexxObject * argument;               /* current argument object           */
  bool       AllIntegers;            /* have all integers                 */


                                       /* are we using default digits?      */
  if (numberDigits() != Numerics::DEFAULT_DIGITS )
                                       /* nope, we can't do integer max.    */
   return this->numberString()->Max(args, argCount);

  if (argCount < 1)                    /* no comparisons to do?             */
    return (RexxObject *)this;         /* just return this as the result    */

  maxvalue = this->value;              /* assume first number is our max.   */

  AllIntegers = true;                  /* assume all numbers are integers.. */
                                       /* check each numbers to see if      */
                                       /* larger than the max.              */
  for (arg = 0; arg < argCount && AllIntegers; arg++) {
    argument = args[arg];              /* get next argument element         */

    if (argument == OREF_NULL)         /* was argument missging ?           */
                                       /* Yes, report the error.            */
      reportException(Error_Incorrect_method_noarg, arg);

    if (isInteger(argument)) {    /* is this an INTEGER object?        */
                                       /* yes, gets its value.              */
      value = ((RexxInteger *)argument)->value;
      if (value > maxvalue)            /* is this number larger than max?   */
        maxvalue = value;              /* yes, it is our new max.           */
    }
    else {                             /* not an integer, compare isn't     */
      AllIntegers = false;             /* so simple.                        */
      break;
    }
  }

  if (AllIntegers) {                   /* were all the objects integers?    */
    return new_integer(maxvalue);      /* yes, then max is our max number   */
  }
  else {
                                       /* not all integers, convert into a  */
                                       /* NumberString, and let NumberString*/
                                       /* figure this out.                  */
    return this->numberString()->Max(args, argCount);
  }
}

RexxObject *RexxInteger::Min(
    RexxObject **args,                 /* array of comparison values        */
    size_t argCount)                   /* count of arguments                */
/******************************************************************************/
/* Function:  Perform MAX function on integer objects                         */
/******************************************************************************/
{
  wholenumber_t  value;                /* current working value             */
  wholenumber_t  minvalue;             /* current minimum                   */
  size_t       arg;                    /* current arg position              */
  RexxObject * argument;               /* current argument object           */
  bool       AllIntegers;            /* have all integers                 */


                                       /* are we using default digits?      */
  if (numberDigits() != Numerics::DEFAULT_DIGITS )
                                       /* nope, we can't do integer max.    */
   return this->numberString()->Min(args, argCount);

  if (argCount < 1)                    /* no comparisons to do?             */
    return (RexxObject *)this;         /* just return this as the result    */

  minvalue = this->value;              /* assume first number is our min.   */

  AllIntegers = true;                  /* assume all numbers are integers.. */
                                       /* check each numbers to see if      */
                                       /* larger than the max.              */
  for (arg = 0; arg < argCount && AllIntegers; arg++) {
    argument = args[arg];              /* get next argument element         */

    if (argument == OREF_NULL)         /* was argument missging ?           */
                                       /* Yes, report the error.            */
      reportException(Error_Incorrect_method_noarg, arg);

    if (isInteger(argument)) {    /* is this an INTEGER object?        */
                                       /* yes, gets its value.              */
      value = ((RexxInteger *)argument)->value;
      if (value < minvalue)            /* is this number larger than min?   */
        minvalue = value;              /* yes, it is our new max.           */
    }
    else {                             /* not an integer, compare isn't     */
      AllIntegers = false;             /* so simple.                        */
      break;
    }
  }

  if (AllIntegers) {                   /* were all the objects integers?    */
    return new_integer(minvalue);      /* yes, then max is our max number   */
  }
  else {
                                       /* not all integers, convert into a  */
                                       /* NumberString, and let NumberString*/
                                       /* figure this out.                  */
    return this->numberString()->Min(args, argCount);
  }
}

RexxObject *RexxInteger::trunc(
    RexxObject *decimals)              /* number of decimal digits requested*/
/******************************************************************************/
/* Function:  Act as a front end for the actual TRUNC REXX method             */
/******************************************************************************/
{
                                       /* just forward to numberstring      */
  return this->numberString()->trunc(decimals);
}

RexxObject *RexxInteger::format(
  RexxObject *Integers,                /* space for integer part            */
  RexxObject *Decimals,                /* number of decimals required       */
  RexxObject *MathExp,                 /* the exponent size                 */
  RexxObject *ExpTrigger )             /* the exponent trigger              */
/******************************************************************************/
/* Function:  Act as a front end for the actual FORMAT REXX method            */
/******************************************************************************/
{
  return this->numberString()->formatRexx(Integers, Decimals, MathExp, ExpTrigger);
}

RexxObject *RexxInteger::d2c(
    RexxObject *length)                /* length of result                  */
/******************************************************************************/
/* Function:  Convert a decimal number string into a character string         */
/******************************************************************************/
{
                                       /* format as a string value          */
  return this->numberString()->d2xD2c(length, true);
}

RexxObject *RexxInteger::d2x(
    RexxObject *length)                /* length of result                  */
/******************************************************************************/
/* Function:  Convert a decimal number string into a hexadecimal string       */
/******************************************************************************/
{
                                       /* format as a string value          */
  return this->numberString()->d2xD2c(length, false);
}

RexxObject  *RexxInteger::evaluate(
    RexxActivation      *context,      /* current activation context        */
    RexxExpressionStack *stack )       /* evaluation stack                  */
/******************************************************************************/
/* Function:  Polymorphic method that makes integer a polymorphic expression  */
/*            term for literals                                               */
/******************************************************************************/
{
  stack->push(this);                   /* place on the evaluation stack     */
                                       /* trace if necessary                */
  context->traceIntermediate(this, TRACE_PREFIX_LITERAL);
  return this;                         /* also return the result            */
}


RexxObject  *RexxInteger::getValue(
    RexxActivation *context)           /* current activation context        */
/******************************************************************************/
/* Function:  Polymorphic get_value function used with expression terms       */
/******************************************************************************/
{
  return (RexxObject *)this;           /* just return this value            */
}


RexxObject  *RexxInteger::getValue(
    RexxVariableDictionary *context)   /* current activation context        */
/******************************************************************************/
/* Function:  Polymorphic get_value function used with expression terms       */
/******************************************************************************/
{
  return (RexxObject *)this;           /* just return this value            */
}


RexxObject  *RexxInteger::getRealValue(
    RexxActivation *context)           /* current activation context        */
/******************************************************************************/
/* Function:  Polymorphic get_value function used with expression terms       */
/******************************************************************************/
{
  return (RexxObject *)this;           /* just return this value            */
}


RexxObject  *RexxInteger::getRealValue(
    RexxVariableDictionary *context)   /* current activation context        */
/******************************************************************************/
/* Function:  Polymorphic get_value function used with expression terms       */
/******************************************************************************/
{
  return (RexxObject *)this;           /* just return this value            */
}

/* **************************************** */
/*  Integer class methods begin here .....  */
/* **************************************** */

RexxIntegerClass::RexxIntegerClass()
/******************************************************************************/
/* This method will pre-allocate 100 integer objects, 0-99.  These will then  */
/*  be used when ever a request for an integer between 0 and 99 is requested  */
/*  this should help reduce some of our memory requirements and trips through */
/*  memory_new.                                                               */
/******************************************************************************/
{
 int  i;                               /* loop counter                      */

 for (i=0;i<INTEGERCACHESIZE ;i++ ) {  /* now create all our cached integers*/
   OrefSet(this, this->integercache[i], new  RexxInteger (i));
   /* force the item to create its string value too.  This can save */
   /* us a lot of time when string indices are used for compound */
   /* variables and also eliminate a bunch of old-new table */
   /* references. */
   this->integercache[i]->stringValue();
 }
                                       /* If first one through, generate all   */
  zero           = integercache[0];   /*  static integers we want to use...   */
  one            = integercache[1];   /* This will allow us to use the static */
  two            = integercache[1];   /* integers instead of having to do a   */
  three          = integercache[1];   /* new_integer every time....           */
  four           = integercache[1];
  five           = integercache[1];
  six            = integercache[1];
  seven          = integercache[1];
  eight          = integercache[1];
  nine           = integercache[1];

                                       /* avoid that through caching        */
                                       /* TheTrueObject == IntegerOne etc.  */
  trueObject  = new RexxInteger((wholenumber_t)true);
  falseObject = new RexxInteger((wholenumber_t)false);
}

void RexxIntegerClass::live()
/******************************************************************************/
/* Function:  Normal garbage collection live marking                          */
/******************************************************************************/
{
  int  i;                              /* loop counter                      */

  this->RexxClass::live();             /* do RexxClass level marking        */

  setUpMemoryMark
                                       /* now mark the cached integers      */
  for (i = 0;i < INTEGERCACHESIZE ;i++ ) {
   memory_mark(this->integercache[i]);
  }
  cleanUpMemoryMark
}

void RexxIntegerClass::liveGeneral()
/******************************************************************************/
/* Function:  Generalized object marking                                      */
/******************************************************************************/
{
  int  i;                              /* loop counter                      */

  this->RexxClass::liveGeneral();      /* do RexxClass level marking        */

  setUpMemoryMarkGeneral
                                       /* now mark the cached integers      */
  for (i = 0;i < INTEGERCACHESIZE ;i++ ) {
   memory_mark_general(this->integercache[i]);
  }
  cleanUpMemoryMarkGeneral
}

void *RexxInteger::operator new(size_t size)
/******************************************************************************/
/* Function:  Create a new integer object                                     */
/******************************************************************************/
{
  RexxObject * newObject;              /* newly create object               */

  newObject = new_object(size);        /* get a new object                  */
                                       /* add in the integer behaviour, and */
                                       /* make sure old2new knows about it  */
  BehaviourSet(newObject, TheIntegerBehaviour);
  ClearObject(newObject);              /* clear the object                  */
  SetObjectHasNoReferences(newObject); /* Tell GC, not to bother with Live  */
  return newObject;                    /* return the new object.            */
}

// the class singleton instance
RexxIntegerClass *RexxIntegerClass::classInstance = NULL;

void RexxIntegerClass::createInstance(RootImage *rootImage)
/******************************************************************************/
/* Function:  Create the integer class and set up the integer cache           */
/******************************************************************************/
{
                                       /* Create the Integer class object   */
                                       /*  its asubclass of the CLASS class,*/
                                       /*  and needs to override the NEW    */
                                       /*  method to provide caching        */
                                       /*  support for integers.            */
  TheIntegerClass = createClassObject(Integer, RexxIntegerClass);
                                       /*  initialize our static array of   */
                                       /*  cached integers                  */
  new (TheIntegerClass) RexxIntegerClass();

  // save true and false in the result set
  rootImage->trueObject = trueObject;
  rootImage->falseObject = falseObject;
}

void RexxIntegerClass::restoreInstance(RootImage *rootImage)
/******************************************************************************/
/* Function:  Create the integer class and set up the integer cache           */
/******************************************************************************/
{
  zero           = classInstance->integercache[0];   /*  static integers we want to use...   */
  one            = classInstance->integercache[1];   /* This will allow us to use the static */
  two            = classInstance->integercache[1];   /* integers instead of having to do a   */
  three          = classInstance->integercache[1];   /* new_integer every time....           */
  four           = classInstance->integercache[1];
  five           = classInstance->integercache[1];
  six            = classInstance->integercache[1];
  seven          = classInstance->integercache[1];
  eight          = classInstance->integercache[1];
  nine           = classInstance->integercache[1];
}



PCPPM RexxInteger::operatorMethods[] = {  /* integer operator methods          */
   NULL,                               /* first entry not used              */
   (PCPPM)&RexxInteger::plus,
   (PCPPM)&RexxInteger::minus,
   (PCPPM)&RexxInteger::multiply,
   (PCPPM)&RexxInteger::divide,
   (PCPPM)&RexxInteger::integerDivide,
   (PCPPM)&RexxInteger::remainder,
   (PCPPM)&RexxInteger::power,
   (PCPPM)&RexxInteger::concat,
   (PCPPM)&RexxInteger::concat, /* Duplicate entry neccessary        */
   (PCPPM)&RexxInteger::concatBlank,
   (PCPPM)&RexxInteger::equal,
   (PCPPM)&RexxInteger::notEqual,
   (PCPPM)&RexxInteger::isGreaterThan,
   (PCPPM)&RexxInteger::isLessOrEqual,
   (PCPPM)&RexxInteger::isLessThan,
   (PCPPM)&RexxInteger::isGreaterOrEqual,
                              /* Duplicate entry neccessary        */
   (PCPPM)&RexxInteger::isGreaterOrEqual,
   (PCPPM)&RexxInteger::isLessOrEqual,
   (PCPPM)&RexxInteger::strictEqual,
   (PCPPM)&RexxInteger::strictNotEqual,
   (PCPPM)&RexxInteger::strictGreaterThan,
   (PCPPM)&RexxInteger::strictLessOrEqual,
   (PCPPM)&RexxInteger::strictLessThan,
   (PCPPM)&RexxInteger::strictGreaterOrEqual,
                              /* Duplicate entry neccessary        */
   (PCPPM)&RexxInteger::strictGreaterOrEqual,
   (PCPPM)&RexxInteger::strictLessOrEqual,
   (PCPPM)&RexxInteger::notEqual,
   (PCPPM)&RexxInteger::notEqual,
   (PCPPM)&RexxInteger::andOp,
   (PCPPM)&RexxInteger::orOp,
   (PCPPM)&RexxInteger::xorOp,
   (PCPPM)&RexxInteger::operatorNot,
};

