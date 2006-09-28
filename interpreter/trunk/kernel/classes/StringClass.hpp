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
/* REXX Kernel                                               StringClass.hpp  */
/*                                                                            */
/* Primitive String Class Definition                                          */
/*                                                                            */
/******************************************************************************/
#ifndef Included_RexxString
#define Included_RexxString

#include "RexxActivity.hpp"
#include "NumberStringClass.hpp"
#include "IntegerClass.hpp"
#include "oorexx.h"
#include "SysUtil.hpp"
                                       /* return values from the is_symbol  */
                                       /* validation method                 */
#define  STRING_BAD_VARIABLE   0
#define  STRING_STEM           1
#define  STRING_COMPOUND_NAME  2
#define  STRING_LITERAL        3
#define  STRING_LITERAL_DOT    4
#define  STRING_NUMERIC        5
#define  STRING_NAME           6

#define  STRING_HASLOWER       0x01    /* string does contain lowercase     */
#define  STRING_NOLOWER        0x02    /* string does not contain lowercase */
#define  STRING_NONNUMERIC     0x04    /* string is non-numeric             */
#define  STRING_NODBCS         0x08    /* string has no DBCS characters     */

#define  INITIAL_NAME_SIZE     10      /* first name table allocation       */
#define  EXTENDED_NAME_SIZE    10      /* amount to extend table by         */

 class RexxString : public RexxObject {
  public:
   inline RexxString() {;} ;
   inline RexxString(RESTORETYPE restoreType) { ; };

   void        live();
   void        liveGeneral();
   void        flatten(RexxEnvelope *envelope);
   RexxObject *unflatten(RexxEnvelope *);

   HashCode     hash();
   inline HashCode getStringHash()
   {
       HashCode h = this->getHashValue(); // see if we've generated a hash code yet
       if (h == 0)                        // if we've never generated this, the code is zero
       {
           stringsize_t len = this->getLength();

           // the hash code is generated from all of the string characters.
           // we do this in a lazy fashion, since most string objects never need to
           // calculate a hash code, particularly the long ones.
           // This hashing algorithm is very similar to that used for Java strings.
           for (stringsize_t i = 0; i < len; i++)
           {
               h = 31 * h + this->stringData[i];
           }
           this->hashvalue = h;
       }
       return h;
   }

   bool         numberValue(wholenumber_t *result, size_t precision);
   bool         numberValue(wholenumber_t *result);
   bool         unsignedNumberValue(stringsize_t *result, size_t precision);
   bool         unsignedNumberValue(stringsize_t *result);
   bool         doubleValue(double *result);
   RexxNumberString *numberString();
   RexxInteger *integerValue(size_t);
   RexxString  *makeString();
   RexxString  *primitiveMakeString();
   void         copyIntoTail(RexxCompoundTail *buffer);
   RexxString  *stringValue();
   bool         isLogical();
   bool         truthValue(wholenumber_t);

   bool        isEqual(RexxObject *);
   bool        isEqual(char *);
   bool        primitiveIsEqual(RexxObject *);
   int         strictComp(RexxObject *);
   int         comp(RexxObject *);
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

   stringsize_t get(stringsize_t, stringchar_t *, stringsize_t);
   RexxObject *lengthRexx();
   RexxString *concatRexx(RexxObject *);
   RexxString *concat(RexxString *);
   RexxString *concatToCstring(char *);
   RexxString *concatWithCstring(char *);
   RexxString *concatBlank(RexxObject *);
   bool        checkLower();
   RexxString *upper();
   RexxString *stringTrace();
   RexxString *lower();
   void        setNumberString(RexxObject *);
   RexxString *concatWith(RexxString *, stringchar_t);

   RexxObject *plus(RexxObject *right);
   RexxObject *minus(RexxObject *right);
   RexxObject *multiply(RexxObject *right);
   RexxObject *divide(RexxObject *right);
   RexxObject *integerDivide(RexxObject *right);
   RexxObject *remainder(RexxObject *right);
   RexxObject *power(RexxObject *right);
   RexxObject *abs();
   RexxObject *sign();
   RexxObject *notOp();
   RexxObject *operatorNot(RexxObject *);
   RexxObject *andOp(RexxObject *);
   RexxObject *orOp(RexxObject *);
   RexxObject *xorOp(RexxObject *);
   RexxObject *Max(RexxObject **args, size_t argCount);
   RexxObject *Min(RexxObject **args, size_t argCount);
   RexxObject *trunc(RexxInteger *decimals);
   RexxObject *format(RexxObject *Integers, RexxObject *Decimals, RexxObject *MathExp, RexxObject *ExpTrigger);
   RexxObject *isInteger();
   RexxObject *logicalOperation(RexxObject *, RexxObject *, int);
   RexxString *extract(stringsize_t offset, stringsize_t sublength) { return newString(this->getStringData() + offset, sublength); }
   RexxObject *evaluate(RexxActivation *, RexxExpressionStack *);
   RexxObject *getValue(RexxActivation *context) {return this;}
   RexxObject *getRealValue(RexxActivation *context) {return this;}
   RexxObject *getValue(RexxVariableDictionary *context) {return this;}
   RexxObject *getRealValue(RexxVariableDictionary *context) {return this;}
                                       /* the following methods are in    */
                                       /* OKBSUBS                         */
   RexxString  *center(RexxInteger *, RexxString *);
   RexxString  *delstr(RexxInteger *, RexxInteger *);
   RexxString  *insert(RexxString *, RexxInteger *, RexxInteger *, RexxString *);
   RexxString  *left(RexxInteger *, RexxString *);
   RexxString  *overlay(RexxString *, RexxInteger *, RexxInteger *, RexxString *);
   RexxString  *reverse();
   RexxString  *right(RexxInteger *, RexxString *);
   RexxString  *strip(RexxString *, RexxString *);
   RexxString  *substr(RexxInteger *, RexxInteger *, RexxString *);
   RexxString  *delWord(RexxInteger *, RexxInteger *);
   RexxString  *space(RexxInteger *, RexxString *);
   RexxString  *subWord(RexxInteger *, RexxInteger *);
   RexxString  *word(RexxInteger *);
   RexxInteger *wordIndex(RexxInteger *);
   RexxInteger *wordLength(RexxInteger *);
   RexxInteger *wordPos(RexxString *, RexxInteger *);
   RexxInteger *words();
                                       /* the following methods are in    */
                                       /* OKBMISC                         */
   RexxString  *changeStr(RexxString *, RexxString *);
   RexxInteger *abbrev(RexxString *, RexxInteger *);
   RexxInteger *compare(RexxString *, RexxString *);
   RexxString  *copies(RexxInteger *);
   RexxObject  *dataType(RexxString *);
   RexxInteger *lastPos(RexxString *, RexxInteger *);
   RexxInteger *posRexx(RexxString *, RexxInteger *);
   RexxString  *translate(RexxString *, RexxString *, RexxString *);
   RexxInteger *verify(RexxString *, RexxString *, RexxInteger *);
   RexxInteger *countStrRexx(RexxString *);
   stringsize_t countStr(RexxString *);
                                       /* the following methods are in    */
                                       /* OKBBITS                         */
   RexxString  *bitAnd(RexxString *, RexxString *);
   RexxString  *bitOr(RexxString *, RexxString *);
   RexxString  *bitXor(RexxString *, RexxString *);
                                       /* the following methods are in    */
                                       /* OKBCONV                         */
   RexxString  *b2x();
   RexxString  *c2d(RexxInteger *);
   RexxString  *c2x();
   RexxString  *encodeBase64();
   RexxString  *decodeBase64();
   RexxString  *d2c(RexxInteger *);
   RexxString  *d2x(RexxInteger *);
   RexxString  *x2b();
   RexxString  *x2c();
   RexxString  *x2d(RexxInteger *);
   RexxString  *x2dC2d(RexxInteger *, bool);

   RexxArray   *makeArray(RexxString *);
   void         copyIntoRxString(RxString *target);

/****************************************************************************/
/*                                                                          */
/*      RexxString Methods in OKBMISC.C                                     */
/*                                                                          */
/****************************************************************************/
   int         isSymbol();
   stringsize_t pos(RexxString *, stringsize_t);
   stringsize_t caselessPos(RexxString *, stringsize_t);

   stringsize_t       validDBCS();
   RexxString  *DBCSreverse();
   RexxString  *DBCSsubstr(RexxInteger *, RexxInteger *, RexxString *);
   RexxString  *DBCSdelstr(RexxInteger *, RexxInteger *);
   RexxString  *DBCSsubWord(RexxInteger *, RexxInteger *);
   RexxString  *DBCSdelWord(RexxInteger *, RexxInteger *);
   RexxString  *DBCSstrip(RexxString *, RexxString *);
   RexxInteger *DBCSlength();
   RexxInteger *DBCSwordPos(RexxString *, RexxInteger *);
   RexxInteger *DBCSdatatype(int);
   RexxInteger *DBCScompare(RexxString *, RexxString *);
   RexxString  *DBCScopies(RexxInteger *);
   RexxInteger *DBCSabbrev(RexxString *, RexxInteger *);
   RexxString  *DBCSspace(RexxInteger *, RexxString *);
   RexxString  *DBCSleft(RexxInteger *, RexxString *);
   RexxString  *DBCSright(RexxInteger *, RexxString *);
   RexxString  *DBCScenter(RexxInteger *, RexxString *);
   RexxString  *DBCSinsert(RexxString *, RexxInteger *, RexxInteger *, RexxString *);
   RexxString  *DBCSoverlay(RexxString *, RexxInteger *, RexxInteger *, RexxString *);
   RexxInteger *DBCSverify(RexxString *, RexxString *, RexxInteger *);
   RexxInteger *DBCSwords();
   stringsize_t DBCSpos(RexxString *, stringsize_t);
   stringsize_t DBCScaselessPos(RexxString *, stringsize_t);
   RexxInteger *DBCSlastPos(RexxString *, RexxInteger *);
   RexxInteger *DBCSwordIndex(RexxInteger *);
   RexxInteger *DBCSwordLength(RexxInteger *);
   RexxString  *DBCSword(RexxInteger *);
   RexxString  *DBCStranslate(RexxString *, RexxString *, RexxString *);
   RexxString  *dbLeft(RexxInteger *, RexxString *, RexxString *);
   RexxString  *dbRight(RexxInteger *, RexxString *, RexxString *);
   RexxString  *dbCenter(RexxInteger *, RexxString *, RexxString *);
   RexxString  *dbRleft(RexxInteger *, RexxString *);
   RexxString  *dbRright(RexxInteger *, RexxString *);
   RexxString  *dbToDbcs();
   RexxString  *dbToSbcs();
   RexxInteger *dbValidate(RexxString *);
   RexxInteger *dbWidth(RexxString *);
   RexxString  *dbAdjust(RexxString *);
   RexxString  *dbBracket();
   RexxString  *dbUnBracket();
   int          DBCSstringCompare(RexxString *);
   stringsize_t DBCSmovePointer(stringsize_t, int, stringsize_t);
   inline void finish(stringsize_t l) { length = l; }

/* Inline_functions */

   inline stringsize_t  getLength() { return this->length; };
   inline stringchar_t *getStringData() { return this->stringData; };
   inline void put(stringsize_t s, const void *b, stringsize_t l) { memcpy((this->getStringData()+s),b,(size_t)l); };
   inline void set(stringsize_t s, stringchar_t c, stringsize_t l) { memset((this->getStringData()+s), c, (size_t)l); };
   inline char getChar(stringsize_t p) { return *(this->getStringData()+p); };
   inline char putChar(stringsize_t p, stringchar_t c) { return *(this->getStringData()+p) = c; };
   inline bool upperOnly() {return (this->Attributes&STRING_NOLOWER) != 0;};
   inline bool hasLower() {return (this->Attributes&STRING_HASLOWER) != 0;};
   inline void setUpperOnly() {this->Attributes |= STRING_NOLOWER; };
   inline void setHasLower() {this->Attributes |= STRING_HASLOWER;};
   inline bool nonNumeric() {return (this->Attributes&STRING_NONNUMERIC) != 0;};
   inline void setNonNumeric() {this->Attributes |= STRING_NONNUMERIC;};
   inline bool strCompare(const stringchar_t *s) {return this->memCompare((s), strlen((const char *)(s))) == 0;};
   inline bool strICompare(const stringchar_t * s) { return (size_t)this->getLength() == strlen((const char *)(s)) && SysUtil::stricmp((const char *)s, (const char *)this->getStringData()) == 0;}
   inline bool memCompare(const stringchar_t *s, stringsize_t l) { return l == this->getLength() && !memcmp((const char *)(s), (const char *)(this->getStringData()), l); }
   inline bool memCompare(RexxString *other) { return other->getLength() == this->getLength() && !memcmp((char *)other->getStringData(), (char *)this->getStringData(), length); }
   inline void memCopy(stringchar_t *s) { memcpy(s, stringData, length); }
   inline bool noDBCS() { return (Attributes&STRING_NODBCS) != 0; }
   static RexxArray  *createStringArray(stringchar_t *start, stringsize_t length, stringchar_t separator);

   RexxNumberString *createNumberString();

   inline RexxNumberString *fastNumberString() {
       if (this->nonNumeric())              /* Did we already try and convert to */
                                            /* to a numberstring and fail?       */
        return OREF_NULL;                   /* Yes, no need to try agian.        */

       if (this->NumberString != OREF_NULL) /* see if we have already converted  */
         return this->NumberString;         /* return the numberString Object.   */
       return createNumberString();         /* go build the number string version */
   }

   inline int sortCompare(RexxString *other) {
       stringsize_t compareLength = length;
       if (compareLength > other->getLength()) {
           compareLength = other->getLength();
       }
       int result = memcmp(stringData, other->getStringData(), compareLength);
       if (result == 0) {
           if (length > other->getLength()) {
               result = 1;
           }
           else if (length < other->getLength()) {
               result = -1;
           }
       }
       return result;
   }

   inline int sortCaselessCompare(RexxString *other) {
       stringsize_t compareLength = length;
       if (compareLength > other->getLength()) {
           compareLength = other->getLength();
       }
       int result = SysUtil::memicmp(stringData, other->getStringData(), compareLength);
       if (result == 0) {
           if (length > other->getLength()) {
               result = 1;
           }
           else if (length < other->getLength()) {
               result = -1;
           }
       }
       return result;
   }

   inline int sortCompare(RexxString *other, stringsize_t startCol, stringsize_t colLength) {
       int result = 0;
       if ((startCol < length ) && (startCol < other->getLength())) {
           stringsize_t stringLength = length;
           if (stringLength > other->getLength()) {
               stringLength = other->getLength();
           }
           stringLength = stringLength - startCol + 1;
           stringsize_t compareLength = colLength;
           if (compareLength > stringLength) {
               compareLength = stringLength;
           }

           result = memcmp(stringData + startCol, other->getStringData() + startCol, compareLength);
           if (result == 0 && stringLength < colLength) {
               if (length > other->getLength()) {
                   result = 1;
               }
               else if (length < other->getLength()) {
                   result = -1;
               }
           }
       }
       else {
           if (length == other->getLength()) {
               result = 0;
           }
           else {
               result = length < other->getLength() ? -1 : 1;
           }
       }
       return result;
   }

   inline int sortCaselessCompare(RexxString *other, stringsize_t startCol, stringsize_t colLength) {
       int result = 0;
       if ((startCol < length ) && (startCol < other->getLength())) {
           stringsize_t stringLength = length;
           if (stringLength > other->getLength()) {
               stringLength = other->getLength();
           }
           stringLength = stringLength - startCol + 1;
           stringsize_t compareLength = colLength;
           if (compareLength > stringLength) {
               compareLength = stringLength;
           }

           result = SysUtil::memicmp(stringData + startCol, other->getStringData() + startCol, compareLength);
           if (result == 0 && stringLength < colLength) {
               if (length > other->getLength()) {
                   result = 1;
               }
               else if (length < other->getLength()) {
                   result = -1;
               }
           }
       }
       else {
           if (length == other->getLength()) {
               result = 0;
           }
           else {
               result = length < other->getLength() ? -1 : 1;
           }
       }
       return result;
   }

   RexxString *newRexx(RexxObject **, stringsize_t);

   static RexxString *newString(const stringchar_t *, stringsize_t);
   static RexxString *newUpperString(const stringchar_t *, stringsize_t);
   static RexxString *newString(double d);
   static RexxString *newString(double d, stringsize_t precision);
   static RexxString *rawString(stringsize_t);
   static RexxString *newProxy(RexxString *);

   static RexxClass *classInstance;    // the singleton class instance
   static PCPPM operatorMethods[];     // the primitive methods assigned to operators

protected:
   stringsize_t length;                /* string length                   */
   RexxNumberString *NumberString;     /* lookaside information           */
   uint32_t Attributes;                /* string attributes               */
   stringchar_t stringData[4];         /* Start of the string data part   */
 };


// some handy functions for doing cstring/RexxString manipulations

 inline void * rmemcpy(void *t, RexxString *s, size_t len)
 {
     return memcpy(t, (char *)s->getStringData(), len);
 }

 inline int rmemcmp(const void *t, RexxString *s, size_t len)
 {
     return memcmp(t, s->getStringData(), len);
 }

 inline char * rstrcpy(char *t, RexxString *s)
 {
     return strcpy(t, (char *)s->getStringData());
 }

 inline char * rstrcat(char *t, RexxString *s)
 {
     return strcat(t, (char *)s->getStringData());
 }

 inline int rstrcmp(char *t, RexxString *s)
 {
     return strcmp(t, (char *)s->getStringData());
 }


// String creation inline functions

inline RexxString *new_string(const stringchar_t *s, stringsize_t l)
{
    return RexxString::newString(s, l);
}

inline RexxString *raw_string(stringsize_t l)
{
    return RexxString::rawString(l);
}

inline RexxString *new_string(double d)
{
    return RexxString::newString(d);
}

inline RexxString *new_string(double d, stringsize_t precision)
{
    return RexxString::newString(d, precision);
}

inline RexxString *new_string(const char *string)
{
    return new_string((const stringchar_t *)string, strlen(string));
}

inline RexxString *new_string(const stringchar_t *string)
{
    return new_string(string, strlen((const char *)string));
}

inline RexxString *new_string(const RexxStringPointer string)
{
    return new_string((stringchar_t *)string, strlen((char *)string));
}

inline RexxString *new_string(stringchar_t cc)
{
    return new_string(&cc, 1);
}

inline RexxString *new_string(char c)
{
    return new_string((stringchar_t *)&c, 1);
}

inline RexxString *new_string(RxString *s)
{
    return new_string((stringchar_t *)s->strptr, s->strlength);
}

inline RexxString *new_string(RxString &s)
{
    return new_string((stringchar_t *)s.strptr, s.strlength);
}

inline RexxString *new_proxy(RexxString *name)
{

    return RexxString::newProxy(name);
}

inline RexxString *new_upper_string(const stringchar_t *s, stringsize_t l)
{
    return RexxString::newUpperString(s, l);
}

inline RexxString *new_upper_string(const char *string)
{
    return new_upper_string((const stringchar_t *)string, strlen(string));
}

inline RexxString *new_upper_string(const stringchar_t *string)
{
    return new_upper_string(string, strlen((const char *)string));
}

inline RexxString *new_upper_string(RxString *s)
{
    return new_upper_string((stringchar_t *)s->strptr, s->strlength);
}

inline RexxString *new_upper_string(const RexxStringPointer string)
{
    return new_upper_string((stringchar_t *)string, strlen((char *)string));
}

#endif
