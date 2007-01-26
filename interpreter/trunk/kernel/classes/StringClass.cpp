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
/* REXX Kernel                                                  StringClass.c    */
/*                                                                            */
/* Primitive String Class                                                     */
/*                                                                            */
/******************************************************************************/
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "RexxCore.h"
#include "StringClass.hpp"
#include "DirectoryClass.hpp"
#include "RexxActivation.hpp"
#include "RexxActivity.hpp"
#include "ASCIIDBCSStrings.hpp"
#include "Interpreter.hpp"
#include "ArgumentUtilities.hpp"
#include "QueueClass.hpp"


RexxClass *RexxString::classInstance = OREF_NULL;    // the singleton class instance

HashCode RexxString::hash()
/******************************************************************************/
/* Function:  retrieve the hash value of a string object                      */
/******************************************************************************/
{
  if (!isString(this))            /*  a nonprimitive object?           */
                                       /* see if == overridden.             */
    return this->sendMessage(OREF_STRICT_EQUAL)->requestString()->getStringHash();
  else
  {
      return this->getStringHash();      /* return the string hash            */
  }
}

void RexxString::live()
/******************************************************************************/
/* Function:  Normal garbage collection live marking                          */
/******************************************************************************/
{
  setUpMemoryMark
  memory_mark(this->NumberString);
  memory_mark(this->objectVariables);
  cleanUpMemoryMark
}

void RexxString::liveGeneral()
/******************************************************************************/
/* Function:  Generalized object marking                                      */
/******************************************************************************/
{
  setUpMemoryMarkGeneral
  memory_mark_general(this->NumberString);
  memory_mark_general(this->objectVariables);
  cleanUpMemoryMarkGeneral
}

void RexxString::flatten(RexxEnvelope *envelope)
/******************************************************************************/
/* Function:  Flatten an object                                               */
/******************************************************************************/
{
  setUpFlatten(RexxString)

   flatten_reference(newThis->NumberString, envelope);
   flatten_reference(newThis->objectVariables, envelope);

  cleanUpFlatten
}

RexxObject *RexxString::unflatten(RexxEnvelope *envelope)
/******************************************************************************/
/* Function:  unflatten an object                                             */
/******************************************************************************/
{
  RexxObject *newSelf;

  if (this->header.isProxyObject()) {       /* is this a proxy object?              */
                                            /* Yes, then we need to run the code    */
                                            /*  to generate a new object            */
    newSelf = envelope->queryProxy(this);
    if (!newSelf) {                         /* not in the table?                    */
                                            /* get the proxy from the environment   */
      newSelf = RexxInterpreter::resolveProxy(this);
                                            /* and add to the proxy table           */
      envelope->addProxy(this, newSelf);
    }
  }
  else
    newSelf = this->RexxObject::unflatten(envelope);
  return newSelf;                           /* return the new version               */
}

RexxString *RexxString::stringValue()
/******************************************************************************/
/* Function:  Return the primitive string value of this object                */
/******************************************************************************/
{
  if (isString(this))             /* already a primitive string?       */
    return this;                       /* just return our selves            */
  else                                 /* need to build a new string        */
    return new_string(this->getStringData(), this->getLength());
}

RexxString  *RexxString::makeString()
/******************************************************************************/
/* Function:  Handle a REQUEST('STRING') request for a REXX string object     */
/******************************************************************************/
{
  if (isPrimitive(this))               /* really a primitive string?        */
    return this;                       /* this is easy                      */
  else                                 /* need to create a new string       */
    return new_string(this->getStringData(), this->getLength());
}


void RexxString::copyIntoTail(RexxCompoundTail *tail)
/******************************************************************************/
/* Function:  Handle a tail construction request for an internal object       */
/******************************************************************************/
{
                                       /* copy this directly into the tail */
    tail->append((stringchar_t *)this->getStringData(), this->getLength());
}


RexxString  *RexxString::primitiveMakeString()
/******************************************************************************/
/* Function:  Handle a REQUEST('STRING') request for a REXX string object     */
/******************************************************************************/
{
  return this;                         /* this is easy                      */
}

bool RexxString::numberValue(wholenumber_t *result, stringsize_t digits)
/******************************************************************************/
/* Function:  Convert a string object to a long value.  Returns false         */
/*            it will not convert.                                            */
/******************************************************************************/
{
  RexxNumberString *numberstring;      /* converted numberstring version    */

  if (!(isString(this)))          /* subclassed string object?         */
                                       /* get the string value's long value */
    return this->requestString()->numberValue(result, digits);
  numberstring = this->fastNumberString(); /* get the number string version     */
  if (numberstring != OREF_NULL )      /* convert ok?                       */
                                       /* convert to integer with proper    */
                                       /* precision                         */
    return numberstring->numberValue(result, digits);
  else
    return false;                        /* return the "not value long" value */
}

bool RexxString::numberValue(wholenumber_t *result)
/******************************************************************************/
/* Function:  Convert a string object to a long value.  Returns false         */
/*            it will not convert.                                            */
/******************************************************************************/
{
  RexxNumberString *numberstring;      /* converted numberstring version    */

  if (!(isString(this)))          /* subclassed string object?         */
                                       /* get the string value's long value */
    return this->requestString()->numberValue(result);
  numberstring = this->fastNumberString(); /* get the number string version     */
  if (numberstring != OREF_NULL )      /* convert ok?                       */
                                       /* convert to integer with proper    */
                                       /* precision                         */
    return numberstring->numberValue(result);
  else
    return false;                        /* return the "not value long" value */
}

bool RexxString::unsignedNumberValue(stringsize_t *result, stringsize_t digits)
/******************************************************************************/
/* Function:  Convert a string object to a long value.  Returns false         */
/*            it will not convert.                                            */
/******************************************************************************/
{
  RexxNumberString *numberstring;      /* converted numberstring version    */

  if (!(isString(this)))          /* subclassed string object?         */
                                       /* get the string value's long value */
    return this->requestString()->unsignedNumberValue(result, digits);
  numberstring = this->fastNumberString(); /* get the number string version     */
  if (numberstring != OREF_NULL )      /* convert ok?                       */
                                       /* convert to integer with proper    */
                                       /* precision                         */
    return numberstring->unsignedNumberValue(result, digits);
  else
    return false;                        /* return the "not value long" value */
}

bool RexxString::unsignedNumberValue(stringsize_t *result)
/******************************************************************************/
/* Function:  Convert a string object to a long value.  Returns false         */
/*            it will not convert.                                            */
/******************************************************************************/
{
  RexxNumberString *numberstring;      /* converted numberstring version    */

  if (!(isString(this)))               /* subclassed string object?         */
                                       /* get the string value's long value */
    return this->requestString()->unsignedNumberValue(result);
  numberstring = this->fastNumberString(); /* get the number string version     */
  if (numberstring != OREF_NULL )      /* convert ok?                       */
                                       /* convert to integer with proper    */
                                       /* precision                         */
    return numberstring->unsignedNumberValue(result);
  else
    return false;                        /* return the "not value long" value */
}

bool RexxString::doubleValue(double *result)
/******************************************************************************/
/* Function:  Convert a string object to a double value                       */
/******************************************************************************/
{
  RexxNumberString *numberDouble;      /* converted number string           */

  numberDouble = this->fastNumberString(); /* convert String to Numberstring    */
  if (numberDouble != OREF_NULL)       /* Did we get a numberstring?        */
    return numberDouble->doubleValue(result);/* Yup, convert it to double         */
  else
    return false;                      /* not number string, so NODOUBLE    */
}

RexxNumberString *RexxString::numberString()
/******************************************************************************/
/* Function:   Convert a String Object into a Number Object                   */
/******************************************************************************/
{
  RexxString       *newSelf;           /* converted string value            */
  RexxNumberString *numberString;      /* numberstring value of string      */

  if (this->nonNumeric())              /* Did we already try and convert to */
                                       /* to a numberstring and fail?       */
   return OREF_NULL;                   /* Yes, no need to try agian.        */

  if (this->NumberString != OREF_NULL) /* see if we have already converted  */
    return this->NumberString;         /* return the numberString Object.   */

  if (!isString(this)) {          /* not truly a string type?          */
    newSelf = this->requestString();   /* do the conversion                 */
                                       /* get a new numberstring Obj        */
    OrefSet(newSelf, newSelf->NumberString, new_numberstring(newSelf->getStringData(), newSelf->getLength()));
                                       /* save the number string            */
    numberString = newSelf->NumberString;
    if (numberString != OREF_NULL)     /* Did number convert OK?            */
      SetObjectHasReferences(newSelf); /* Make sure we are sent Live...     */
  }
  else {                               /* real primitive string             */
                                       /* get a new numberstring Obj        */
    OrefSet(this, this->NumberString, new_numberstring(this->getStringData(), this->getLength()));
    numberString = this->NumberString; /* save the number string            */
    if (numberString == OREF_NULL)     /* Did number convert OK?            */
      this->setNonNumeric();           /* mark as a nonnumeric              */
    else {
      SetObjectHasReferences(this);    /* Make sure we are sent Live...     */
                                       /* connect the string and number     */
      this->NumberString->setString(this);
    }
  }
  return numberString;                 /* return the numberString Object.   */
}

RexxNumberString *RexxString::createNumberString()
/******************************************************************************/
/* Function:   Convert a String Object into a Number Object                   */
/******************************************************************************/
{
  RexxString       *newSelf;           /* converted string value            */
  RexxNumberString *numberString;      /* numberstring value of string      */

  if (!isString(this)) {          /* not truly a string type?          */
    newSelf = this->requestString();   /* do the conversion                 */
                                       /* get a new numberstring Obj        */
    OrefSet(newSelf, newSelf->NumberString, new_numberstring(newSelf->getStringData(), newSelf->getLength()));
                                       /* save the number string            */
    numberString = newSelf->NumberString;
    if (numberString != OREF_NULL)     /* Did number convert OK?            */
      SetObjectHasReferences(newSelf); /* Make sure we are sent Live...     */
  }
  else {                               /* real primitive string             */
                                       /* get a new numberstring Obj        */
    OrefSet(this, this->NumberString, new_numberstring(this->getStringData(), this->getLength()));
    numberString = this->NumberString; /* save the number string            */
    if (numberString == OREF_NULL)     /* Did number convert OK?            */
      this->setNonNumeric();           /* mark as a nonnumeric              */
    else {
      SetObjectHasReferences(this);    /* Make sure we are sent Live...     */
                                       /* connect the string and number     */
      this->NumberString->setString(this);
    }
  }
  return numberString;                 /* return the numberString Object.   */
}

stringsize_t RexxString::get(stringsize_t start, stringchar_t *  buffer, stringsize_t bufl)
/******************************************************************************/
/* Function:  Get a section of a string and copy it into a buffer             */
/******************************************************************************/
{
    size_t copylen = 0;

    if (start < this->length)
    {
        if (bufl <= this->length-start)
        {
            copylen = bufl - 1;
        }
        else
        {
            copylen = this->length-start;
            *(buffer+copylen) = '\0';
        }
        memcpy(buffer,this->stringData+start,(size_t)copylen);
    }
    else
    {
        *buffer = '\0';
    }

    return copylen + 1;
}

RexxObject *RexxString::lengthRexx()
/******************************************************************************/
/* Function:  Return the length of a string as an integer object              */
/******************************************************************************/
{
  stringsize_t tempLen;

  if (DBCS_SELF)                       /* need to use DBCS?                 */
                                       /* return the DBCS count             */
    return this->DBCSlength();
  tempLen = this->getLength();
                                       /* return string byte length         */
  return (RexxObject *) new_integer(tempLen);
}

bool RexxString::isEqual(
    RexxObject *otherObj)              /* other comparison object           */
/******************************************************************************/
/* Function:  Primitive strict equal\not equal method.  This determines       */
/*            only strict equality, not greater or less than values.          */
/******************************************************************************/
{
  stringsize_t  otherLen;                    /* length of the other string        */
  RexxString *other;                   /* converted string object           */

  requiredArg(otherObj, ONE);         /* this is required.                 */
  if (!isPrimitive(this))              /* not a primitive?                  */
                                       /* do the full lookup compare        */
    return this->sendMessage(OREF_STRICT_EQUAL, otherObj)->truthValue(Error_Logical_value_method);

  other = REQUEST_STRING(otherObj);    /* force into string form            */
  if (DBCS_MODE) {                     /* need to use DBCS?                 */
    ValidDBCS(this);                   /* validate the string               */
    ValidDBCS(other);                  /* and the other string too          */
  }

  otherLen = other->getLength();            /* get length of second string.      */
                                       /* do quick compare on the hash      */
  if (this->hashvalue != other->hashvalue)
    return false;                      /* can't be equal                    */
  if (otherLen != this->getLength())        /* lengths different?                */
    return false;                      /* also unequal                      */
                                       /* now compare the actual string     */
  return !memcmp(this->getStringData(), other->getStringData(), otherLen);
}

bool RexxString::isEqual(
    char *otherObj)                    /* other comparison object           */
/******************************************************************************/
/* Function:  Primitive strict equal\not equal method.  This determines       */
/*            only strict equality, not greater or less than values.          */
/******************************************************************************/
{
  stringsize_t  otherLen;              /* length of the other string        */

  otherLen = strlen(otherObj);              /* get length of second string.      */

  if (otherLen != this->getLength())        /* lengths different?                */
    return false;                      /* also unequal                      */
                                       /* now compare the actual string     */
  return !memcmp(this->getStringData(), otherObj, otherLen);
}

bool RexxString::primitiveIsEqual(
    RexxObject *otherObj)              /* other comparison object           */
/******************************************************************************/
/* Function:  Primitive strict equal\not equal method.  This determines       */
/*            only strict equality, not greater or less than values.          */
/******************************************************************************/
{
  stringsize_t  otherLen;                    /* length of the other string        */
  RexxString *other;                   /* converted string object           */

  requiredArg(otherObj, ONE);         /* this is required.                 */
  other = REQUEST_STRING(otherObj);    /* force into string form            */
  if (DBCS_MODE) {                     /* need to use DBCS?                 */
    ValidDBCS(this);                   /* validate the string               */
    ValidDBCS(other);                  /* and the other string too          */
  }

  otherLen = other->getLength();            /* get length of second string.      */
                                       /* do quick compare on the hash      */
  if (this->hashvalue != other->hashvalue)
    return false;                      /* can't be equal                    */
  if (otherLen != this->getLength())        /* lengths different?                */
    return false;                      /* also unequal                      */
                                       /* now compare the actual string     */
  return !memcmp(this->getStringData(), other->getStringData(), otherLen);
}

int RexxString::comp(RexxObject *other)
/******************************************************************************/
/* Function:  Do a value comparison of two strings for the non-strict         */
/*            comparisons.  This returns for the compares:                    */
/*                                                                            */
/*             a value < 0 when this is smaller than other                    */
/*             a value   0 when this is equal to other                        */
/*             a value > 0 when this is larger than other                     */
/******************************************************************************/
{
  RexxString *second;                  /* string value of other             */
  RexxNumberString *firstNum;          /* numberstring value of this        */
  RexxNumberString *secondNum;         /* numberstring value of other       */
  stringchar_t * firstStart;           /* comparison start pointer          */
  stringchar_t * secondStart;          /* other start pointer               */
  stringsize_t firstLen;               /* this compare length               */
  stringsize_t secondLen;              /* other compare length              */
  int  result;                         /* compare result                    */

                                       /* We need to see if the objects can */
                                       /* be Converted to NumberString Objs */
                                       /* 1st, this way we know if the COMP */
                                       /* method of number String will      */
                                       /* succeed.  Will only fail if an    */
                                       /* object cannot be represented as a */
                                       /* number.  This is important since  */
                                       /* NumberString calls String to do   */
                                       /* the compare if it can't, since    */
                                       /* this is the method NumberString   */
                                       /* will call, we must make sure a    */
                                       /* call to NumberString succeeds or  */
                                       /* we will get into a loop.          */
  requiredArg(other, ONE);            /* make sure we have a real argument */
                                       /* try and convert both numbers      */
  if (((firstNum = this->fastNumberString()) != OREF_NULL) && ((secondNum = other->numberString()) != OREF_NULL ))
                                       /* yes, send converted numbers and do*/
                                       /* the compare                       */
    return (int) firstNum->comp(secondNum);
  second = REQUEST_STRING(other);      /* yes, get a string object.         */
  if (DBCS_MODE)                       /* need to use DBCS?                 */
                                       /* do the DBCS comparison            */
     return this->DBCSstringCompare(second);
                                       /* objects are converted.  now strip */
                                       /* any leading/trailing blanks.      */

  firstLen = this->getLength();             /* get the initial length            */
  firstStart = (stringchar_t *)this->getStringData(); /* and starting position           */

  secondLen = second->getLength();          /* get length of second string.      */
  secondStart = (stringchar_t *)second->getStringData(); /* get pointer to start of data */

                                       /* while we have leading blanks.     */
  while (firstLen > 0 && *firstStart == ch_BLANK) {
   firstStart++;                       /* ignore character and look at next */
   firstLen--;                         /* and string is now one char less.  */
  }
                                       /* while we have leading blanks.     */
  while (secondLen > 0 && *secondStart == ch_BLANK) {
   secondStart++;                      /* ignore character and look at next */
   secondLen--;                        /* and string is now one char less.  */
  }

  if (firstLen >= secondLen) {         /* determine the longer string.      */
                                       /* first string is larger,           */

                                       /* do a memory compare of strings,   */
                                       /* use length of smaller string.     */
    result = memcmp(firstStart, secondStart, (size_t) secondLen);
                                       /* equal but different lengths?      */
    if ((result == 0) && (firstLen != secondLen)) {
                                       /* point to first remainder char     */
      firstStart = (stringchar_t *) firstStart + secondLen;
      while (firstLen-- > secondLen) { /* while still have more to compare  */
                                       /* subtract a blank                  */
        result = *firstStart++ - ch_BLANK;
        if (result != 0)               /* not equal?                        */
          break;                       /* all done                          */
      }
    }
  }

  else {                               /* The length of second obj is longer*/
                                       /* do memory compare of strings, use */
                                       /*  length of smaller string.        */
    result = memcmp(firstStart, secondStart, (size_t) firstLen);
    if  (result == 0) {                /* if strings compared equal, we have*/
                                       /* we need to compare the trailing   */
                                       /* part with blanks                  */
      secondStart = (stringchar_t *) secondStart + firstLen;
      while (secondLen-- > firstLen) { /* while the longer string stills has*/
                                       /* subtract from a blank             */
        result = ch_BLANK - *secondStart++;
        if (result != 0)               /* not equal?                        */
          break;                       /* all done                          */
      }
    }
  }
  return result;                       /* return the compare result         */
}

int  RexxString::strictComp(RexxObject *otherObj)
/******************************************************************************/
/* Function:  Do a strict comparison of two strings.  This returns:           */
/*                                                                            */
/*             a value < 0 when this is smaller than other                    */
/*             a value   0 when this is equal to other                        */
/*             a value > 0 when this is larger than other                     */
/******************************************************************************/
{
  stringchar_t *otherData;             /* the other character data          */
  stringsize_t otherLen;               /* length of the other string        */
  int  result;                         /* compare result                    */
  RexxString *other;                   /* converted string value            */

  requiredArg(otherObj, ONE);         /* this is required.                 */
  other = REQUEST_STRING(otherObj);    /* force into string form            */
  if (DBCS_MODE) {                     /* need to use DBCS?                 */
    ValidDBCS(this);                   /* validate the string               */
    ValidDBCS(other);                  /* and the other string too          */
  }

  otherLen = other->getLength();            /* get length of second string.      */
  otherData = other->getStringData();       /* get pointer to start of data.     */

  if (this->getLength() >= otherLen) {      /* determine the longer string.      */
                                       /* first string is larger,           */
                                       /* do a memory compare of strings,   */
                                       /* use length of smaller string.     */
    result = memcmp(this->getStringData(), otherData, (size_t) otherLen);
                                       /* if strings are equal, and         */
                                       /* are not equal, the self is greater*/
    if ((result == 0) && (this->getLength() > otherLen))
      result = 1;                      /* otherwise they are equal.         */
  }
  else {                               /* The length of second obj is longer*/
                                       /* do memory compare of strings, use */
                                       /*  length of smaller string.        */
    result = memcmp(this->getStringData(), otherData, (size_t) this->getLength());
    if  (result == 0)                  /* if stings compared equal,         */
      result = -1;                     /*  then the other string is bigger. */
  }
  return result;                       /* finished, return our result       */
}

RexxObject *RexxString::plus(RexxObject *right)
/******************************************************************************/
/* Function:  String addition...performed by RexxNumberString                 */
/******************************************************************************/
{
  RexxNumberString *numstr;            /* converted number string           */

                                       /* non-numeric?                      */
  if ((numstr = this->fastNumberString()) == OREF_NULL)
                                       /* this is a conversion error        */
    reportException(Error_Conversion_operator, this);
  return numstr->plus(right);          /* have numberstring do this         */
}

RexxObject *RexxString::minus(RexxObject *right)
/******************************************************************************/
/* Function:  String subtraction...performed by RexxNumberString              */
/******************************************************************************/
{
  RexxNumberString *numstr;            /* converted number string           */

                                       /* non-numeric?                      */
  if ((numstr = this->fastNumberString()) == OREF_NULL)
                                       /* this is a conversion error        */
    reportException(Error_Conversion_operator, this);
  return numstr->minus(right);         /* have numberstring do this         */
}

RexxObject *RexxString::multiply(RexxObject *right)
/******************************************************************************/
/* Function:  String multiplication...performed by RexxNumberString           */
/******************************************************************************/
{
  RexxNumberString *numstr;            /* converted number string           */

                                       /* non-numeric?                      */
  if ((numstr = this->fastNumberString()) == OREF_NULL)
                                       /* this is a conversion error        */
    reportException(Error_Conversion_operator, this);
  return numstr->multiply(right);      /* have numberstring do this         */
}

RexxObject *RexxString::divide(RexxObject *right)
/******************************************************************************/
/* Function:  String division...performed by RexxNumberString                 */
/******************************************************************************/
{
  RexxNumberString *numstr;            /* converted number string           */

                                       /* non-numeric?                      */
  if ((numstr = this->fastNumberString()) == OREF_NULL)
                                       /* this is a conversion error        */
    reportException(Error_Conversion_operator, this);
  return numstr->divide(right);        /* have numberstring do this         */
}

RexxObject *RexxString::integerDivide(RexxObject *right)
/******************************************************************************/
/* Function:  String division...performed by RexxNumberString                 */
/******************************************************************************/
{
  RexxNumberString *numstr;            /* converted number string           */

                                       /* non-numeric?                      */
  if ((numstr = this->fastNumberString()) == OREF_NULL)
                                       /* this is a conversion error        */
    reportException(Error_Conversion_operator, this);
  return numstr->integerDivide(right); /* have numberstring do this         */
}

RexxObject *RexxString::remainder(RexxObject *right)
/******************************************************************************/
/* Function:  String division...performed by RexxNumberString                 */
/******************************************************************************/
{
  RexxNumberString *numstr;            /* converted number string           */

                                       /* non-numeric?                      */
  if ((numstr = this->fastNumberString()) == OREF_NULL)
                                       /* this is a conversion error        */
    reportException(Error_Conversion_operator, this);
  return numstr->remainder(right);     /* have numberstring do this         */
}

RexxObject *RexxString::power(RexxObject *right)
/******************************************************************************/
/* Function:  String division...performed by RexxNumberString                 */
/******************************************************************************/
{
  RexxNumberString *numstr;            /* converted number string           */

                                       /* non-numeric?                      */
  if ((numstr = this->fastNumberString()) == OREF_NULL)
                                       /* this is a conversion error        */
    reportException(Error_Conversion_operator, this);
  return numstr->power(right);         /* have numberstring do this         */
}

RexxObject *RexxString::abs(void)
/******************************************************************************/
/* Function:  String absolute value...performed by RexxNumberString           */
/******************************************************************************/
{
  RexxNumberString *numstr;            /* converted number string           */

                                       /* non-numeric?                      */
  if ((numstr = this->fastNumberString()) == OREF_NULL)
                                       /* this is a conversion error        */
    reportException(Error_Incorrect_method_string_nonumber, (stringchar_t *)"ABS", this);
  return numstr->abs();                /* have numberstring do this         */
}

RexxObject *RexxString::sign(void)
/******************************************************************************/
/* Function:  String sign value...performed by RexxNumberString               */
/******************************************************************************/
{
  RexxNumberString *numstr;            /* converted number string           */

                                       /* non-numeric?                      */
  if ((numstr = this->fastNumberString()) == OREF_NULL)
                                       /* this is a conversion error        */
    reportException(Error_Incorrect_method_string_nonumber, (stringchar_t *)"SIGN", this);
  return numstr->Sign();               /* have numberstring do this         */
}

RexxObject *RexxString::Max(RexxObject **arguments, size_t argCount)
/******************************************************************************/
/* Function:  String max value...performed by RexxNumberString                */
/******************************************************************************/
{
  RexxNumberString *numstr;            /* converted number string           */

                                       /* non-numeric?                      */
  if ((numstr = this->fastNumberString()) == OREF_NULL)
                                       /* this is a conversion error        */
    reportException(Error_Incorrect_method_string_nonumber, (stringchar_t *)"MAX", this);
  /* have numberstring do this         */
  return numstr->Max(arguments, argCount);
}

RexxObject *RexxString::Min(RexxObject **arguments, size_t argCount)
/******************************************************************************/
/* Function:  String min value...performed by RexxNumberString                */
/******************************************************************************/
{
  RexxNumberString *numstr;            /* converted number string           */

                                       /* non-numeric?                      */
  if ((numstr = this->fastNumberString()) == OREF_NULL)
                                       /* this is a conversion error        */
    reportException(Error_Incorrect_method_string_nonumber, (stringchar_t *)"MIN", this);
  /* have numberstring do this         */
  return numstr->Min(arguments, argCount);
}

RexxObject *RexxString::trunc(RexxInteger *decimals)
/******************************************************************************/
/* Function:  String Trunc...performed by RexxNumberString                    */
/******************************************************************************/
{
  RexxNumberString *numstr;            /* converted number string           */

                                       /* non-numeric?                      */
  if ((numstr = this->fastNumberString()) == OREF_NULL)
                                       /* this is a conversion error        */
    reportException(Error_Incorrect_method_string_nonumber, (stringchar_t *)"TRUNC", this);
  return numstr->trunc(decimals);      /* have numberstring do this         */
}

RexxObject *RexxString::format(RexxObject *Integers, RexxObject *Decimals, RexxObject *MathExp, RexxObject *ExpTrigger)
/******************************************************************************/
/* Function:  String Format...performed by RexxNumberString                   */
/******************************************************************************/
{
  RexxNumberString *numstr;            /* converted number string           */

                                       /* non-numeric?                      */
  if ((numstr = this->fastNumberString()) == OREF_NULL)
                                       /* this is a conversion error        */
    reportException(Error_Incorrect_method_string_nonumber, (stringchar_t *)"FORMAT", this);
                                       /* have numberstring do this         */
  return numstr->formatRexx(Integers, Decimals, MathExp, ExpTrigger);
}

RexxInteger *RexxString::strictEqual(RexxObject *other)
/******************************************************************************/
/* Function:  Strict ("==") equality operator...also returns the hash value   */
/*            if sent with no other object                                    */
/******************************************************************************/
{
  if (other == OREF_NULL)              /* asking for the hash value?        */
                                       /* create a string value             */
    return (RexxInteger *)new_string((stringchar_t *)&this->hashvalue, sizeof(this->hashvalue));
  else
    return this->primitiveIsEqual(other) ? TheTrueObject : TheFalseObject;
}

RexxInteger *RexxString::strictNotEqual(RexxObject *other)
/******************************************************************************/
/* Function:  Strict ("\==") inequality operator                              */
/******************************************************************************/
{
  return !this->primitiveIsEqual(other) ? TheTrueObject : TheFalseObject;
}

RexxInteger *RexxString::equal(RexxObject *other)
/******************************************************************************/
/* Function:  Non-strict ("=") string equality operator                       */
/******************************************************************************/
{
  return ((this->comp(other) == 0) ? TheTrueObject : TheFalseObject);
}

RexxInteger *RexxString::notEqual(RexxObject *other)
/******************************************************************************/
/* Function:  Non-Strict ("\=") string inequality operator                    */
/******************************************************************************/
{
  return ((this->comp(other) != 0) ? TheTrueObject : TheFalseObject);
}

RexxInteger *RexxString::isGreaterThan(RexxObject *other)
/******************************************************************************/
/* Function:  Non-strict greater than operator (">")                          */
/******************************************************************************/
{
  return ((this->comp(other) > 0) ? TheTrueObject : TheFalseObject);
}

RexxInteger *RexxString::isLessThan(RexxObject *other)
/******************************************************************************/
/* Function:  Non-strict less than operatore ("<")                            */
/******************************************************************************/
{
  return ((this->comp(other) < 0) ? TheTrueObject : TheFalseObject);
}

RexxInteger *RexxString::isGreaterOrEqual(RexxObject *other)
/******************************************************************************/
/* Function:  Non-strict greater than or equal operator (">=" or "\<")        */
/******************************************************************************/
{
  return ((this->comp(other) >= 0) ? TheTrueObject : TheFalseObject);
}

RexxInteger *RexxString::isLessOrEqual(RexxObject *other)
/******************************************************************************/
/* Function:  Non-strict less than or equal operator ("<=" or "\>")           */
/******************************************************************************/
{
  return ((this->comp(other) <= 0) ? TheTrueObject : TheFalseObject);
}

RexxInteger *RexxString::strictGreaterThan(RexxObject *other)
/******************************************************************************/
/* Function:  Strict greater than comparison (">>")                           */
/******************************************************************************/
{
  return (this->strictComp(other) > 0) ? TheTrueObject : TheFalseObject;
}

RexxInteger *RexxString::strictLessThan(RexxObject *other)
/******************************************************************************/
/* Function:  Strict less than comparison ("<<")                              */
/******************************************************************************/
{
  return (this->strictComp(other) < 0) ? TheTrueObject : TheFalseObject;
}

RexxInteger *RexxString::strictGreaterOrEqual(RexxObject *other)
/******************************************************************************/
/* Function:  Strict greater than or equal to comparison (">>=" or "\<<")     */
/******************************************************************************/
{
  return (this->strictComp(other) >= 0) ? TheTrueObject : TheFalseObject;
}

RexxInteger *RexxString::strictLessOrEqual(RexxObject *other)
/******************************************************************************/
/* Function:  Strict less than or equal to operatore ("<<=" or "\>>")         */
/******************************************************************************/
{
  return (this->strictComp(other) <= 0) ? TheTrueObject : TheFalseObject;
}

RexxString *RexxString::concat(RexxString *other)
/******************************************************************************/
/* Function:  Concatenate two strings together                                */
/******************************************************************************/
{
  stringsize_t len1;                         /* length of first string            */
  stringsize_t len2;                         /* length of second string           */
  RexxString *result;                  /* result string                     */
  stringchar_t * data;                          /* character pointer                 */

  len1 = this->getLength();                 /* get this length                   */
  len2 = other->getLength();                /* and the other length              */
                                       /* create a new string               */
  result = (RexxString *)raw_string(len1+len2);
  data = result->getStringData();           /* point to the string data          */
  if (len1 != 0) {                     /* have real data?                   */
                                       /* copy the front part               */
    memcpy(data, this->getStringData(), len1);
    data += len1;                      /* step past the length              */
  }
  if (len2 != 0)                       /* have a second length              */
                                       /* and the second part               */
    memcpy(data, other->getStringData(), len2);
  return result;                       /* return the result                 */

}

RexxString *RexxString::concatRexx(RexxObject *otherObj)
/******************************************************************************/
/* Function:  Rexx level concatenate...requires conversion and checking       */
/******************************************************************************/
{
  stringsize_t len1;                         /* length of first string            */
  stringsize_t len2;                         /* length of second string           */
  RexxString *result;                  /* result string                     */
  RexxString *other;
  stringchar_t * data;                          /* character pointer                 */

  requiredArg(otherObj, ONE);         /* this is required.                 */
                                       /* ensure a string value             */
  other = (RexxString *)REQUEST_STRING(otherObj);

  /* added error checking for NULL pointer (from NilObject) */
  if (other == OREF_NULL)
    reportException(Error_Incorrect_method_nostring, IntegerOne);

  if (DBCS_MODE) {                     /* need to use DBCS?                 */
    ValidDBCS(this);                   /* validate the string               */
    ValidDBCS(other);                  /* and the other string too          */
  }

                                       /* the following logic also appears  */
                                       /* in string_concat, but is repeated */
                                       /* here because this is a VERY high  */
                                       /* use function                      */
  len1 = this->getLength();                 /* get this length                   */
  len2 = other->getLength();                /* and the other length              */
                                       /* create a new string               */
  result = (RexxString *)raw_string(len1+len2);
  data = result->getStringData();           /* point to the string data          */
  if (len1 != 0) {                     /* have real data?                   */
                                       /* copy the front part               */
    memcpy(data, this->getStringData(), len1);
    data += len1;                      /* step past the length              */
  }
  if (len2 != 0)                       /* have a second length              */
                                       /* and the second part               */
    memcpy(data, other->getStringData(), len2);
  return result;                       /* return the result                 */
}

RexxString *RexxString::concatToCstring(char * other)
/******************************************************************************/
/* Function:  Concatenate a string object onto an ASCII-Z string              */
/******************************************************************************/
{
  stringsize_t len1;                         /* length of first string            */
  stringsize_t len2;                         /* length of ASCII-Z string          */
  RexxString *result;                  /* result string                     */

  len1 = this->getLength();                 /* get this length                   */
  len2 = strlen(other);                /* and the other length              */
                                       /* create a new string               */
  result = (RexxString *)raw_string(len1+len2);
                                       /* copy the front part               */
  memcpy(result->getStringData(), other, len2);
                                       /* and the second part               */
  memcpy(result->getStringData() + len2, this->getStringData(), len1);
  return result;
}

RexxString *RexxString::concatWithCstring(char * other)
/******************************************************************************/
/* Function:  Concatenate an ASCII-Z string onto a string object              */
/******************************************************************************/
{
  stringsize_t len1;                         /* length of first string            */
  stringsize_t len2;                         /* length of ASCII-Z string          */
  RexxString *result;                  /* result string                     */

  len1 = this->getLength();                 /* get this length                   */
  len2 = strlen(other);                /* and the other length              */
                                       /* create a new string               */
  result = (RexxString *)raw_string(len1+len2);
                                       /* copy the string object            */
  memcpy(result->getStringData(), this->getStringData(), len1);
                                       /* copy the ASCII-Z string           */
  memcpy(result->getStringData() + len1, other, len2);
  return result;
}

RexxString *RexxString::concatBlank(RexxObject *otherObj)
/******************************************************************************/
/* Function:  Concatenate two strings with a blank in between                 */
/******************************************************************************/
{
  stringsize_t len1;                         /* length of first string            */
  stringsize_t len2;                         /* length of second string           */
  RexxString *result;                  /* result string                     */
  RexxString *other;                   /* result string                     */
  stringchar_t * data;                          /* character pointer                 */

  requiredArg(otherObj, ONE);         /* this is required.                 */
                                       /* ensure a string value             */
  other = (RexxString *)REQUEST_STRING(otherObj);

  /* added error checking for NULL pointer (from NilObject) */
  if (other == OREF_NULL)
    reportException(Error_Incorrect_method_nostring, IntegerOne);

  if (DBCS_MODE) {                     /* need to use DBCS?                 */
    ValidDBCS(this);                   /* validate the string               */
    ValidDBCS(other);                  /* and the other string too          */
  }

                                       /* ensure a string value             */
  other = (RexxString *)REQUEST_STRING(otherObj);

  /* added error checking for NULL pointer (from NilObject) */
  if (other == OREF_NULL)
    reportException(Error_Incorrect_method_nostring, IntegerOne);
                                       /* the following logic also appears  */
                                       /* in string_concat_with, but is     */
                                       /* repeated here because this is a   */
                                       /* VERY high use function            */
  len1 = this->getLength();                 /* get this length                   */
  len2 = other->getLength();                /* and the other length              */
                                       /* create a new string               */
  result = (RexxString *)raw_string(len1+len2+1);
  data = result->getStringData();           /* point to the string data          */
  if (len1 != 0) {                     /* have a first string?              */
                                       /* copy the front part               */
    memcpy(data, this->getStringData(), len1);
    data += len1;                      /* step past the length              */
  }
  *data++ = ' ';                       /* stuff in the seperating blank     */
  if (len2 != 0)                       /* have a second string?             */
                                       /* and the second part               */
    memcpy(data, other->getStringData(), len2);
  return result;
}



/**
 * Test if the object is a value logical value as defined
 * for IF, WHEN, etc.
 *
 * @return Returns true if this is the integer 0 or 1, false otherwise.
 */
bool RexxString::isLogical()
{
    RexxString *testString;              /* string to test                    */

    if (!isString(this))                 /*  a nonprimitive object?           */
    {
        testString = this->requestString();/* get the real string value         */

    }
    else
    {
        testString = this;               /* just use the string directly      */
    }

    if (testString->getLength() != 1)    // logicals must be just a single character
    {
        return false;
    }

    // must be either '0' or '1' to be valid
    return testString->getChar(0) == '0' || testString->getChar(0) == '1' ;
}


bool RexxString::truthValue(wholenumber_t errorCode)
/******************************************************************************/
/* Function:  Determine the truth value of a string object, raising the       */
/*            given error if bad.                                             */
/******************************************************************************/
{
  RexxString *testString;              /* string to test                    */

  if (!isString(this))            /*  a nonprimitive object?           */
    testString = this->requestString();/* get the real string value         */
  else
    testString = this;                 /* just use the string directly      */
  if (testString->getLength() != 1)    /* not exactly 1 character long?     */
                                       /* report the error                  */
    reportException(errorCode, testString);
  if (*(testString->getStringData()) == '0')/* exactly '0'?                      */
      return false;                    /* have a false                      */
                                       /* not exactly '1'?                  */
  else if (!(*(testString->getStringData()) == '1'))
    reportException(errorCode, this);  /* report the error                  */
  return true;                         /* this is true                      */
}

bool RexxString::checkLower()
/******************************************************************************/
/* Function:  Tests for existence of lowercase characters                     */
/******************************************************************************/
{
  stringchar_t * data;                         /* current data pointer              */
  stringchar_t * endData;                      /* end location                      */

  data = (stringchar_t *)this->getStringData();     /* point to the string               */
  endData = data + this->getLength();       /* set the end point                 */

  while (data < endData) {             /* loop through entire string        */
    if (*data != toupper(*data)) {     /* have something to uppercase?      */
      this->setHasLower();             /* remember we have this             */
      return true;                     /* just return now                   */
    }
    data++;                            /* step the position                 */
  }
                                       /* no lowercase?                     */
  this->setUpperOnly();                /* set the upper only attribute      */
  return false;                        /* return then translation flag      */
}

RexxString *RexxString::upper()
/******************************************************************************/
/* Function:  Translate a string to uppercase...will only create a new        */
/*            string if characters actually have to be translated.            */
/******************************************************************************/
{
  RexxString *newstring;               /* newly created string              */

  if (DBCS_SELF) {                     /* need to use DBCS?                 */
    ValidDBCS(this);                   /* validate the string               */
                                       /* actually have DBCS and need to    */
                                       /* uppercase?                        */
    if (!noDBCS() & !upperOnly()) {
                                       /* create a new string               */
      newstring = new_string((stringchar_t *)this->getStringData(), this->getLength());
                                       /* do DBCS uppercasing               */
      DBCS_MemUpper(STRPTR(newstring), STRLEN(newstring));
                                       /* rebuild the hash value            */
                                       /* flag the string as uppercased     */
      newstring->setUpperOnly();
      return newstring;                /* return the new string             */
    }                                  /* (single byte only falls through)  */
  }
                                       /* something to uppercase?           */
  if (!this->upperOnly() && (this->hasLower() || this->checkLower()))
  {
      // just construct a new string here.
      return new_upper_string(getStringData(), getLength());
  }
  return this;                         /* return this unchanged             */
}



RexxString *RexxString::stringTrace()
/******************************************************************************/
/* Function:  Translate a string to "traceable" form, removing non-displayable*/
/*            characters                                                      */
/******************************************************************************/
{
  RexxString *newCopy;                 /* new copy of string                */
  stringchar_t *    Current;                   /* current string location           */
  stringsize_t    i;                         /* string length                     */
  bool      NonDisplay;                /* have non-displayables             */

  i = this->getLength();                    /* get the length                    */
  Current = (stringchar_t *)this->getStringData();  /* point to the start                */
  NonDisplay = false;                  /* no non-displayable characters     */

  for (; i > 0; i--) {                 /* loop for the entire string        */
    if (DBCS_SELF) {                   /* this string have DBCS characters  */
      if (IsDBCS(*Current)) {          /* leave DBCS alone                  */
        i--;                           /* reduce length one more            */
        Current++;                     /* and the pointer too               */
      }
                                       /* control character?                */
      else if ((stringchar_t)*Current < (stringchar_t)' ') {
        NonDisplay = true;             /* got a non-displayable             */
        break;                         /* get out of here                   */
       }
    }
    else                               /*no DBCS characters                 */
                                       /* control character?                */
    if ((stringchar_t)*Current < (stringchar_t)' ') {
      NonDisplay = true;               /* got a non-displayable             */
      break;                           /* get out of here                   */
    }
    Current++;                         /* step the pointer                  */
  }
  if (!NonDisplay)                     /* all displayable?                  */
    return this;                       /* leave unchanged                   */
                                       /* copy the string                   */
  newCopy = (RexxString *) this->copy();
  i = newCopy->getLength();                 /* get the length                    */
                                       /* point to the start                */
  Current = (stringchar_t *)newCopy->getStringData();

  for (; i > 0; i--) {                 /* loop for the entire string        */
    if(DBCS_MODE) {
      if (IsDBCS(*Current)) {          /* leave DBCS alone                  */
        i--;                           /* reduce length one more            */
        Current++;                     /* and the pointer too               */
       }
                                       /* control character?                */
      else if ((stringchar_t)*Current < (stringchar_t)' ' && (stringchar_t)*Current != '\t')
        *Current = '?';                /* yes, change to question           */
      Current++;                       /* step the pointer                  */
    }
                                       /* no DBCS characters                */
                                       /* control character?                */
    else if ((stringchar_t)*Current < (stringchar_t)' ' && (stringchar_t)*Current != '\t')
        *Current = '?';                /* yes, change to question           */
      Current++;                       /* step the pointer                  */
  }
  return newCopy;                      /* return the converted string       */
}


RexxString *RexxString::lower()
/******************************************************************************/
/* Function:  Translate a string to lower case                                */
/******************************************************************************/
{
  RexxString *newstring;               /* newly created string              */
  stringchar_t * data;                         /* current data pointer              */
  stringchar_t * outdata;                      /* output data                       */
  stringsize_t i;                            /* loop counter                      */
  bool   translate;                    /* translation required              */

  if (DBCS_SELF) {                     /* need to use DBCS?                 */
    ValidDBCS(this);                   /* validate the string               */
    if (!noDBCS()) {                   /* actually have DBCS characters?    */
                                       /* create a new string               */
      newstring = (RexxString *)raw_string(this->getLength());
                                       /* copy the data                     */
      memcpy(STRPTR(newstring), STRPTR(this), STRLEN(this));
                                       /* do DBCS lowercasing               */
      DBCS_MemLower(STRPTR(newstring), STRLEN(newstring));
                                       /* rebuild the hash value            */
      return newstring;                /* return the new string             */
    }                                  /* (single byte only falls through)  */
  }

  data = (stringchar_t *)this->getStringData();     /* point to the string               */
  translate = false;                   /* no translation required           */

  for (i = 0; i < this->getLength(); i++) { /* loop through entire string        */
    if (*data != tolower(*data)) {     /* have something to lowercase?      */
      translate = true;                /* flag it                           */
      break;                           /* stop at the first one             */
    }
    data++;                            /* step the position                 */
  }
  if (translate) {                     /* something to uppercase?           */
                                       /* create a new string               */
    newstring = (RexxString *)raw_string(this->getLength());
    data = (stringchar_t *)this->getStringData();   /* point to the data start           */
                                       /* point to output data              */
    outdata = (stringchar_t *)newstring->getStringData();
                                       /* loop through entire string        */
    for (i = 0; i < this->getLength(); i++) {
      *outdata = tolower(*data);       /* copy the lowercase character      */
      data++;                          /* step the position                 */
      outdata++;                       /* and the output position           */
    }
  }
  else
    newstring = this;                  /* return untranslated string        */
  return newstring;                    /* return the new copy               */
}


/**
 * Rexx exported method stub for the lower() method.
 *
 * @param start  The optional starting location.  Defaults to the first character
 *               if not specified.
 * @param length The length to convert.  Defaults to the segment from the start
 *               position to the end of the string.
 *
 * @return A new string object with the case conversion applied.
 */
RexxString *RexxString::lowerRexx(RexxInteger *start, RexxInteger *length)
{
    stringsize_t startPos = optionalPositionArgument(start, 1, ARG_ONE) - 1;
    stringsize_t rangeLength = optionalLengthArgument(length, getLength(), ARG_TWO);

    // if we're starting beyond the end bounds, return unchanged
    if (startPos > getLength())
    {
        return this;
    }

    rangeLength = min(rangeLength, getLength() - startPos);

    // a zero length value is also a non-change.
    if (rangeLength == 0)
    {
        return this;
    }

    return lower(startPos, rangeLength);
}


/**
 * Rexx exported method stub for the upper() method.
 *
 * @param start  The optional starting location.  Defaults to the first character
 *               if not specified.
 * @param length The length to convert.  Defaults to the segment from the start
 *               position to the end of the string.
 *
 * @return A new string object with the case conversion applied.
 */
RexxString *RexxString::upperRexx(RexxInteger *start, RexxInteger *length)
{
    stringsize_t startPos = optionalPositionArgument(start, 1, ARG_ONE) - 1;
    stringsize_t rangeLength = optionalLengthArgument(length, getLength(), ARG_TWO);

    // if we're starting beyond the end bounds, return unchanged
    if (startPos > getLength())
    {
        return this;
    }

    rangeLength = min(rangeLength, getLength() - startPos);

    // a zero length value is also a non-change.
    if (rangeLength == 0)
    {
        return this;
    }

    return upper(startPos, rangeLength);
}



/**
 * Lowercase a portion of a Rexx string, returning a new string object.  This
 * method assumes the offset and length are already valid
 * for this string object.
 *
 * @param start  The starting offset of the segment to lowercase (origin 0).
 *
 * @param length The length to lowercase.
 *
 * @return A new string object with the case conversion applied.
 */
RexxString *RexxString::lower(stringsize_t offset, stringsize_t length)
{
    // get a copy of the string
    RexxString *newstring = extract(0, getLength());

    stringchar_t *data = (stringchar_t *)newstring->getStringData() + offset;
    // now uppercase in place
    for (stringsize_t i = 0; i < length; i++) {
        *data = tolower(*data);
        data++;
    }
    return newstring;
}



/**
 * Uppercase a portion of a Rexx string, returning a new string
 * object.  This method assumes the offset and length are
 * already valid for this string object.
 *
 * @param start  The starting offset of the segment to uppercase
 *               (origin 0).
 *
 * @param length The length to lowercase.
 *
 * @return A new string object with the case conversion applied.
 */
RexxString *RexxString::upper(stringsize_t offset, stringsize_t length)
{
    // get a copy of the string
    RexxString *newstring = extract(0, getLength());

    stringchar_t *data = (stringchar_t *)newstring->getStringData() + offset;
    // now uppercase in place
    for (stringsize_t i = 0; i < length; i++) {
        *data = tolower(*data);
        data++;
    }
    return newstring;
}


RexxInteger *RexxString::integerValue(
    stringsize_t digits)                     /* precision to use                  */
/******************************************************************************/
/* Function:  Convert a string object to an integer.  Returns .nil for        */
/*            failures.                                                       */
/******************************************************************************/
{
  RexxNumberString *numberString;      /* string's numberstring version     */
  RexxInteger *newInteger;             /* returned integer string           */

                                       /* Force String conversion through   */
                                       /* NumberString                      */
                                       /* get the number string version     */
  if ((numberString = this->fastNumberString()) != OREF_NULL ) {
                                       /* try for an integer                */
    newInteger = numberString->integerValue(digits);
                                       /* did it convert?                   */
    if (newInteger != TheNilObject && newInteger->getStringrep() == OREF_NULL)
      newInteger->setString(this);     /* connect the string value          */
    return newInteger;                 /* return the new integer            */
  }
  else
    return (RexxInteger *)TheNilObject;/* return .nil for failures          */
}

void RexxString::setNumberString(RexxObject *NumberRep)
/******************************************************************************/
/* Function:  Set a number string value on to the string                      */
/******************************************************************************/
{

  OrefSet(this, this->NumberString, (RexxNumberString *)NumberRep);

  if (NumberRep != OREF_NULL)          /* actually get one?                 */
   SetObjectHasReferences(this);       /* Make sure we are sent Live...     */
  else
   SetObjectHasNoReferences(this);     /* no more references                */
  return;
}

RexxString *RexxString::concatWith(RexxString *other,
                                   stringchar_t between)
/******************************************************************************/
/* Function:  Concatenate two strings with a single character between         */
/******************************************************************************/
{
  stringsize_t len1;                   /* length of first string            */
  stringsize_t len2;                   /* length of second string           */
  RexxString *result;                  /* result string                     */
  stringchar_t * data;                          /* character pointer                 */

  len1 = this->getLength();            /* get this length                   */
  len2 = other->getLength();           /* and the other length              */
                                       /* create a new string               */
  result = (RexxString *)raw_string(len1+len2+1);
  data = result->getStringData();      /* point to the string data          */
  if (len1 != 0) {                     /* have a first string?              */
                                       /* copy the front part               */
    memcpy(data, this->getStringData(), len1);
    data += len1;                      /* step past the length              */
  }
  *data++ = between;                   /* stuff in the seperating char      */
  if (len2 != 0)                       /* have a second string?             */
                                       /* and the second part               */
    memcpy(data, other->getStringData(), len2);
  return result;
}

RexxObject *RexxString::andOp(RexxObject *other)
/******************************************************************************/
/* Function:  Logical AND of a string with another logical value              */
/******************************************************************************/
{
  RexxObject *otherTruth;              /* truth value of the other object   */

  requiredArg(other, ONE);            /* make sure the argument is there   */
                                       /* validate the boolean              */
  otherTruth = other->truthValue(Error_Logical_value_method) ? TheTrueObject : TheFalseObject;
                                       /* perform the operation             */
  return (!this->truthValue(Error_Logical_value_method)) ? TheFalseObject : otherTruth;
}

RexxObject *RexxString::orOp(RexxObject *other)
/******************************************************************************/
/* Function:  Logical OR of a string with another logical value               */
/******************************************************************************/
{
  RexxObject *otherTruth;              /* truth value of the other object   */

  requiredArg(other, ONE);            /* make sure the argument is there   */
                                       /* validate the boolean              */
  otherTruth = other->truthValue(Error_Logical_value_method) ? TheTrueObject : TheFalseObject;
                                       /* perform the operation             */
  return (this->truthValue(Error_Logical_value_method)) ? TheTrueObject : otherTruth;
}

RexxObject *RexxString::xorOp(RexxObject *other)
/******************************************************************************/
/* Function:  Logical XOR of a string with another logical value              */
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

RexxArray *RexxString::makeArray(RexxString *divider)
/******************************************************************************/
/* Function:  Split string into an array                                      */
/******************************************************************************/
{
  // get the separator to use
  stringchar_t separator = padArgument(divider, '\n', ARG_ONE);
  // call the part that chops the strings up using the string data (used by the
  // mutablebuffer class also);
  return createStringArray(getStringData(), getLength(), separator);
}

RexxArray *RexxString::createStringArray(stringchar_t *start, stringsize_t length, stringchar_t separator)
/******************************************************************************/
/* Function:  Split string into an array                                      */
/******************************************************************************/
{
    RexxQueue *strings = new_queue();    /* save each string in a queue       */
    ProtectedObject p2(strings);         // which we need to protect
    stringchar_t *end = start + length;  // set our end marker

    while (start < end)
    {
        stringchar_t *tmp = start;
        /* search for separator character    */
        while (tmp < end && *tmp != separator)
        {
            tmp++;
        }
        strings->queue(new_string(start, tmp - start));
        // step to the next scan position
        start = tmp + 1;
        // if we're separating on newline characters, check to see if this is a \n\r sequence
        if (separator == 'n')
        {
            // step over the second part of the line terminator
            if (start < end && *start == '\r')
            {
                start++;
            }
        }
    }
    // now convert this in an arra
    return strings->makeArray();
}

RexxObject *RexxString::notOp()
/******************************************************************************/
/* Function:  Logical NOT of a string                                         */
/******************************************************************************/
{
  return this->truthValue(Error_Logical_value_method) ? (RexxObject *)TheFalseObject : (RexxObject *)TheTrueObject;
}

RexxObject *RexxString::operatorNot(RexxObject *other)
/******************************************************************************/
/* Function:  Logical NOT of a string                                         */
/******************************************************************************/
{
  return this->truthValue(Error_Logical_value_method) ? (RexxObject *)TheFalseObject : (RexxObject *)TheTrueObject;
}

RexxObject *RexxString::isInteger()
/******************************************************************************/
/* Function:  Test if this string is an integer value                         */
/******************************************************************************/
{
  stringchar_t *digitPtr;
  stringsize_t digitsLeft;

    digitPtr = (stringchar_t *)this->getStringData();
    digitsLeft = this->getLength();

                                       /* Skip all leading blanks           */
    for (; digitsLeft && *digitPtr == ch_BLANK; ++digitPtr, --digitsLeft) ;

    if (digitsLeft){                   /* Still Digits left ?               */
      if (*digitPtr == ch_PLUS || *digitPtr == ch_MINUS) {
                                       /* need to move past the sign and    */
                                       /*  remove any remaining blanks.     */
       for (++digitPtr, --digitsLeft;
            digitsLeft && *digitPtr == ' ';
            ++digitPtr, --digitsLeft) ;
                                       /* Yes, skip any blanks              */
       if (!digitsLeft)                /* Did we reach end of data ?        */
                                       /* Yes, not valid                    */
        return (RexxObject *) TheFalseObject;
      }
                                       /* we are now ready to check for     */
                                       /*digits                             */
     for (; digitsLeft && *digitPtr >= ch_ZERO && *digitPtr <= ch_NINE;
           ++digitPtr, --digitsLeft) ;
                                       /* found our first non-digit, or end */
                                       /* is it a decimal point?            */
     if ( digitsLeft && *digitPtr == ch_PERIOD) {
      digitPtr++;                      /* Yes, see if remaining digits are 0*/
      digitsLeft--;
      for (; digitsLeft && *digitPtr == ch_ZERO; ++digitPtr, --digitsLeft) ;
     }
                                       /* if chars left make sure all are   */
                                       /* blanks.                           */
     for (; digitsLeft && *digitPtr == ch_BLANK; ++digitPtr, --digitsLeft) ;
                                       /* skipped all trailing blanks.      */
                                       /* we better be at the end of the    */
                                       /* string, otherwise its invalid.    */
     if (!digitsLeft)
                                       /* yes its the end, return true      */
      return (RexxObject *) TheTrueObject;
    }

                                       /* all other cases are invalid....   */
    return (RexxObject *) TheFalseObject;
}

RexxObject *RexxString::evaluate(
    RexxActivation      *context,      /* current activation context        */
    RexxExpressionStack *stack )       /* evaluation stack                  */
/******************************************************************************/
/* Function:  Polymorphic method that makes string a polymorphic expression   */
/*            term for string literals.                                       */
/******************************************************************************/
{

  stack->push((RexxObject *)this);     /* place on the evaluation stack     */
                                       /* trace if necessary                */
  context->traceIntermediate((RexxObject *)this, TRACE_PREFIX_LITERAL);
  return this;                         /* also return the result            */
}

RexxString *RexxString::newString(const stringchar_t * string, stringsize_t length)
/******************************************************************************/
/* Function:  Allocate (and initialize) a string object                       */
/******************************************************************************/
{
  RexxString *newString;               /* new string object                 */
  stringsize_t      size2;                   /* allocated size                    */

                                       /* calculate the size                */
                                       /* STRINGOBJ - excess chars (3)      */
                                       /* + length. only sub 3 to allow     */
                                       /* for terminating NULL              */
  size2 = sizeof(RexxString) - (sizeof(char) * 3) + length;
                                       /* allocate the new object           */
  newString = (RexxString *)new_object(size2);
                                       /* set the behaviour from the class*/
  BehaviourSet(newString, TheStringBehaviour);
                                       /* set the virtual function table    */
  setVirtualFunctions(newString, T_String);
                                       /* clear the front part              */
  ClearObjectLength(newString, sizeof(RexxString));
  newString->length = length;          /* save the length                   */
  newString->hashvalue = 0;            // make sure the hash value is zeroed
                                       /* Null terminate, allows faster     */
                                       /* conversion to ASCII-Z string      */
  *(newString->getStringData() + length) = '\0';
                                       /* copy it over                      */
  memcpy(newString->getStringData(),string,(size_t)length);
                                       /* by  default, we don't need Live   */
  SetObjectHasNoReferences(newString); /*sent                               */
                                       /* NOTE: That if we can set          */
                                       /*  this->NumebrString elsewhere     */
                                       /*we need to mark ourselves as       */
  return newString;                    /*having OREFs                       */
}


/**
 * Allocate an initialize a string object that will also
 * contain only uppercase characters.  This allows a creation
 * and uppercase operation to be done in one shot, without
 * requiring two string objects to be created.
 *
 * @param string The source string data.
 * @param length The length of the string data.
 *
 * @return A newly constructed string object.
 */
RexxString *RexxString::newUpperString(const stringchar_t * string, stringsize_t length)
{
    RexxString *newString;               /* new string object                 */
    stringsize_t      size2;                   /* allocated size                    */

    /* calculate the size                */
    /* STRINGOBJ - excess chars (3)      */
    /* + length. only sub 3 to allow     */
    /* for terminating NULL              */
    size2 = sizeof(RexxString) - (sizeof(char) * 3) + length;
    /* allocate the new object           */
    newString = (RexxString *)new_object(size2);
    /* set the behaviour from the class*/
    BehaviourSet(newString, TheStringBehaviour);
    /* set the virtual function table    */
    setVirtualFunctions(newString, T_String);
    /* clear the front part              */
    ClearObjectLength(newString, sizeof(RexxString));
    newString->length = length;          /* save the length                   */
    newString->hashvalue = 0;            // make sure the hash value is zeroed
                                         /* create a new string               */
                                         /* point to output data              */
    stringchar_t *outdata = (stringchar_t *)newString->getStringData();
    // set the input markers
    stringchar_t *indata = (stringchar_t *)string;
    stringchar_t *endData = indata + length;
    while (indata < endData)             /* loop through entire string        */
    {
        *outdata = toupper(*indata);     /* copy the uppercase character      */
        indata++;                        /* step the position                 */
        outdata++;                       /* and the output position           */
    }
    newString->setUpperOnly();           /* flag the string as uppercased     */
                                         /* Null terminate, allows faster     */
                                         /* conversion to ASCII-Z string      */
    *(newString->getStringData() + length) = '\0';
    /* by  default, we don't need Live   */
    SetObjectHasNoReferences(newString); /*sent                               */
                                         /* NOTE: That if we can set          */
                                         /*  this->NumebrString elsewhere     */
                                         /*we need to mark ourselves as       */
    return newString;                    /*having OREFs                       */
}

RexxString *RexxString::rawString(stringsize_t length)
/******************************************************************************/
/* Function:  Allocate (and initialize) an empty string object                */
/******************************************************************************/
{
  RexxString *newString;               /* new string object                 */
  stringsize_t      size2;                   /* allocated size                    */

                                       /* calculate the size                */
                                       /* STRINGOBJ - excess chars (3)      */
                                       /* + length. only sub 3 to allow     */
                                       /* for terminating NULL              */
  size2 = sizeof(RexxString) - (sizeof(char) * 3) + length;
                                       /* allocate the new object           */
  newString = (RexxString *)new_object(size2);
                                       /* set the behaviour from the class*/
  BehaviourSet(newString, TheStringBehaviour);
                                       /* set the virtual function table    */
  setVirtualFunctions(newString, T_String);
                                       /* clear the front part              */
  ClearObjectLength(newString, sizeof(RexxString));
  newString->length = length;          /* save the length                   */
  newString->hashvalue = 0;            // make sure the hash value is zeroed
                                       /* Null terminate, allows faster     */
                                       /* conversion to ASCII-Z string      */
  *(newString->getStringData() + length) = '\0';
                                       /* by  default, we don't need Live   */
  SetObjectHasNoReferences(newString); /*sent                               */
                                       /* NOTE: That if we can set          */
                                       /*  this->NumebrString elsewhere     */
                                       /*we need to mark ourselves as       */
  return newString;                    /*having OREFs                       */
}


RexxString *RexxString::newString(double number)
/******************************************************************************/
/* Function: Create a string from a double value                              */
/******************************************************************************/
{
                                       /* get double as a number string.    */
  return new_numberstring(number)->stringValue();
}

/**
 * Convert a double value to a string using the provided
 * precision.
 *
 * @param number    The number to convert.
 * @param precision The precision requested for the result.
 *
 * @return A string value of the converted result.
 */
RexxString *RexxString::newString(double number, stringsize_t precision)
{
    if (number == 0)                     /* zero result?               */
    {
        return new_string("0");
    }
    else
    {
        char buffer[64];
        // format as a string
        _gcvt(number, (int)precision, buffer);
        size_t len = strlen(buffer);
        // if the last character is a decimal, we remove that
        if (buffer[len - 1] == '.')
        {
            len--;
        }
        return new_string(buffer);
    }
}

RexxString *RexxString::newProxy(RexxString *string)
/******************************************************************************/
/* Function:  Create a proxy object from this string                          */
/******************************************************************************/
{
    // copy the string object, because we're going to mark the header
    RexxString *sref = (RexxString *)string->copy();
    sref->header.makeProxy();
    return sref;
}

RexxString *RexxString::newRexx(RexxObject **init_args, size_t argCount)
/******************************************************************************/
/* Arguments: Subclass init arguments                                         */
/* Function:  Create a new string value (used primarily for subclasses)       */
/******************************************************************************/
{
  RexxString *string;                  /* string value                      */

                                       /* break up the arguments            */
  processNewArgs(init_args, argCount, &init_args, &argCount, 1, (RexxObject **)&string, NULL);
                                       /* force argument to string value    */
  string = (RexxString *)REQUIRED_STRING(string, ARG_ONE);
                                       /* create a new string object        */
  string = new_string(string->getStringData(), string->getLength());
  BehaviourSet(string, TheStringClass->instanceBehaviour);
  if (((RexxClass *)this)->uninitDefined()) {
    string->hasUninit();
  }
                                       /* Initialize the new instance       */
  string->sendMessage(OREF_INIT, init_args, argCount);
  return string;                       /* return the new string             */
}


                                       /* following tables define quick     */
                                       /* access methods for operator       */
                                       /* methods.  The methods here MUST   */
                                       /* be defined in the same order as   */
                                       /* the operator defines in Token.h*/

PCPPM RexxString::operatorMethods[] = {      /* table of string operator methods  */
   NULL,                               /* first entry not used              */
   (PCPPM)&RexxString::plus,
   (PCPPM)&RexxString::minus,
   (PCPPM)&RexxString::multiply,
   (PCPPM)&RexxString::divide,
   (PCPPM)&RexxString::integerDivide,
   (PCPPM)&RexxString::remainder,
   (PCPPM)&RexxString::power,
   (PCPPM)&RexxString::concatRexx,
   (PCPPM)&RexxString::concatRexx,
   (PCPPM)&RexxString::concatBlank,
   (PCPPM)&RexxString::equal,
   (PCPPM)&RexxString::notEqual,
   (PCPPM)&RexxString::isGreaterThan,
   (PCPPM)&RexxString::isLessOrEqual,
   (PCPPM)&RexxString::isLessThan,
   (PCPPM)&RexxString::isGreaterOrEqual,
                              /* Duplicate entry neccessary        */
   (PCPPM)&RexxString::isGreaterOrEqual,
   (PCPPM)&RexxString::isLessOrEqual,
   (PCPPM)&RexxString::strictEqual,
   (PCPPM)&RexxString::strictNotEqual,
   (PCPPM)&RexxString::strictGreaterThan,
   (PCPPM)&RexxString::strictLessOrEqual,
   (PCPPM)&RexxString::strictLessThan,
   (PCPPM)&RexxString::strictGreaterOrEqual,
                              /* Duplicate entry neccessary        */
   (PCPPM)&RexxString::strictGreaterOrEqual,
   (PCPPM)&RexxString::strictLessOrEqual,
   (PCPPM)&RexxString::notEqual,
   (PCPPM)&RexxString::notEqual, /* Duplicate entry neccessary        */
   (PCPPM)&RexxString::andOp,
   (PCPPM)&RexxString::orOp,
   (PCPPM)&RexxString::xorOp,
   (PCPPM)&RexxString::operatorNot,
};
