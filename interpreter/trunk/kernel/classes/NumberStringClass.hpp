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
/* REXX Kernel                                         NumberStringClass.hpp  */
/*                                                                            */
/* Primitive NumberString Class Definitions                                   */
/*                                                                            */
/******************************************************************************/
#ifndef Included_RexxNumberString
#define Included_RexxNumberString

#include "RexxActivity.hpp"      // required for access to the current numeric settings

const uint32_t NumFormScientific = 0x00000001;       /* Define Numeric form setting at Object*/
                                                     /*  creation time.                      */
const uint32_t NumberRounded     = 0x00000010;       /* Indicate the number was rounded once */
                                                     /*  at numDigits, avoid double rounding */

#define NumberStringRound(s,d) ((s)->roundUp(s,d))

 class RexxNumberStringBase : public RexxObject {
   public:
    inline RexxNumberStringBase() { ; }
    void   mathRound(stringchar_t *);
    stringchar_t *stripLeadingZeros(stringchar_t *);
    stringchar_t *adjustNumber(stringchar_t *, stringchar_t *, stringsize_t, stringsize_t);

    RexxString *stringObject;          /* converted string value          */
    uint16_t numFlags;                 /* Flags for use by the Numberstring met*/
    int16_t sign;                      /* sign for this number (-1 is neg)     */
    stringsize_t  numDigits;           /* Maintain a copy of digits setting of */
                                       /* From when object was created         */
    wholenumber_t exp;
    stringsize_t  length;
 };

 class RexxNumberString : public RexxNumberStringBase {
   public:
    inline void *operator new(size_t, void *ptr) {return ptr;}
    inline void  operator delete(void *, void *) {;}
    void         *operator new(size_t, size_t);
    inline void operator delete(void *, size_t) {;}
    RexxNumberString(size_t) ;
    inline RexxNumberString(RESTORETYPE restoreType) { ; };
    HashCode     hash();
    void         live();
    void         liveGeneral();
    void         flatten(RexxEnvelope *);

    bool         numberValue(wholenumber_t *result, size_t precision);
    bool         numberValue(wholenumber_t *result);
    bool         unsignedNumberValue(stringsize_t *result, size_t precision);
    bool         unsignedNumberValue(stringsize_t *result);
    bool         doubleValue(double *result);
    RexxNumberString *numberString();
    RexxInteger *integerValue(size_t);
    RexxString  *makeString();
    RexxInteger *hasMethod(RexxString *);
    RexxString  *primitiveMakeString();
    RexxString  *stringValue();
    bool         isLogical();
    bool         truthValue(wholenumber_t);

    bool        isEqual(RexxObject *);
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

    bool       unsignedNumber(size_t *);
    RexxNumberString *clone();
    void        setString(RexxString *);
    void        roundUp(stringsize_t);
    RexxString *formatRexx(RexxObject *, RexxObject *, RexxObject *, RexxObject *);
    RexxString *formatInternal(stringsize_t, stringsize_t, stringsize_t, stringsize_t, RexxNumberString *, stringsize_t, bool);
    RexxObject *operatorNot(RexxObject *);
    RexxObject *evaluate(RexxActivation *, RexxExpressionStack *);
    RexxObject *getValue(RexxActivation *context){return this;}
    RexxObject *getRealValue(RexxActivation *context){return this;}
    RexxObject *getValue(RexxVariableDictionary *dictionary){return this;}
    RexxObject *getRealValue(RexxVariableDictionary *dictionary){return this;}
    RexxObject *trunc(RexxObject *);
    RexxObject *truncInternal(size_t);
    RexxObject *unknown(RexxString *, RexxArray *);
    bool        isInstanceOf(RexxClass *);

    inline RexxNumberString *checkNumber(stringsize_t digits, bool rounding)
    {
     if (this->length > digits)            /* is the length larger than digits()*/
                                           /* need to allocate a new number     */
       return this->prepareNumber(digits, rounding);
     return this;                          /* no adjustment required            */
    }

    stringsize_t highBits(stringsize_t number);
    void subtractNumbers( RexxNumberString *larger, stringchar_t *largerPtr, wholenumber_t aLargerExp,
                           RexxNumberString *smaller, stringchar_t *smallerPtr, wholenumber_t aSmallerExp,
                           RexxNumberString *result, stringchar_t **resultPtr);
    stringchar_t *addMultiplier(stringchar_t *, stringsize_t, stringchar_t *, int);
    stringchar_t * subtractDivisor(stringchar_t *data1, size_t length1,
                           stringchar_t *data2, size_t length2,
                           stringchar_t *result, int Mult);
    stringchar_t * multiplyPower(stringchar_t *leftPtr, RexxNumberStringBase *left,
                         stringchar_t *rightPtr, RexxNumberStringBase *right,
                         stringchar_t *OutPtr, size_t OutLen, size_t NumberDigits);
    stringchar_t * dividePower(stringchar_t *AccumPtr, RexxNumberStringBase *Accum, stringchar_t *Output, size_t NumberDigits);
    stringchar_t * addToBaseSixteen(stringchar_t, stringchar_t *, stringchar_t *);
    stringchar_t * multiplyBaseSixteen(stringchar_t *, stringchar_t *);
    static stringchar_t * addToBaseTen(stringchar_t, stringchar_t *, stringchar_t *);
    static stringchar_t * multiplyBaseTen(stringchar_t *, stringchar_t *);
    RexxNumberString *prepareNumber(stringsize_t, bool);
    void              adjustPrecision(stringchar_t *, stringsize_t);
    void              adjustPrecision();
    inline void       checkPrecision() { if (length > numDigits) adjustPrecision(); }
    inline void       setNumericSettings(stringsize_t digits, bool form)
    {
        this->numDigits = digits;
        if (form == Numerics::FORM_SCIENTIFIC)
            this->numFlags |= NumFormScientific;
        else
            this->numFlags &= ~NumFormScientific;
    }

    void setupNumber();

    RexxNumberString *addSub(RexxNumberString *, int, size_t);
    RexxNumberString *plus(RexxObject *);
    RexxNumberString *minus(RexxObject *);
    RexxNumberString *multiply(RexxObject *);
    RexxNumberString *divide(RexxObject *);
    RexxNumberString *integerDivide(RexxObject *);
    RexxNumberString *remainder(RexxObject *);
    RexxNumberString *power(RexxObject *);
    RexxNumberString *Multiply(RexxNumberString *);
    RexxNumberString *Division(RexxNumberString *, int);
    RexxNumberString *abs();
    RexxInteger *Sign();
    RexxObject  *notOp();
    RexxNumberString *Max(RexxObject **, size_t);
    RexxNumberString *Min(RexxObject **, size_t);
    RexxNumberString *maxMin(RexxObject **, size_t, int);
    RexxObject *isInteger();
    RexxString *d2c(RexxObject *);
    RexxString *d2x(RexxObject *);
    RexxString *d2xD2c(RexxObject *, bool);
    RexxString *concat(RexxObject *);
    RexxString *concatBlank(RexxObject *);
    RexxObject *orOp(RexxObject *);
    RexxObject *andOp(RexxObject *);
    RexxObject *xorOp(RexxObject *);
    bool        checkIntegerDigits(stringsize_t numDigits, stringsize_t &numberLength, wholenumber_t &numberExponent, bool &carry);
    bool        createInteger(stringchar_t *thisnum, stringsize_t intlength, int carry, int sign, wholenumber_t *result);
    bool        createUnsignedValue(stringchar_t *thisnum, stringsize_t intlength, int carry, wholenumber_t exponent, size_t maxValue, size_t *result);
    bool        createUnsignedInt64Value(stringchar_t *thisnum, stringsize_t intlength, int carry, wholenumber_t exponent, uint64_t maxValue, uint64_t &result);
    bool        int64Value(int64_t *result, stringsize_t numDigits);
    bool        unsignedInt64Value(uint64_t *result, stringsize_t numDigits);
    void        formatWholeNumber(wholenumber_t);
    void        formatUnsignedNumber(stringsize_t);
    void        formatInt64(int64_t integer);
    void        formatUnsignedInt64(uint64_t integer);
    int         format(stringchar_t *, stringsize_t);
    inline void setZero() {
        this->number[0] = '\0';               /* Make value a zero.*/
        this->length = 1;                     /* Length is 1       */
        this->sign = 0;                       /* Make sign Zero.   */
        this->exp = 0;                        /* exponent is zero. */
    }

    static RexxNumberString *newInstance(double);
    static RexxNumberString *newInstance(wholenumber_t);
    static RexxNumberString *newInstance(int64_t);
    static RexxNumberString *newInstance(uint64_t);
    static RexxNumberString *newInstance(stringsize_t);
    static RexxNumberString *newInstance(stringchar_t *, stringsize_t);

    stringchar_t number[4];

    static PCPPM operatorMethods[];     // the primitive methods assigned to operators
    static RexxClass *classInstance;    // the single instance of the class
 };

// Make this a method.....
void AdjustPrecision(RexxNumberString *, stringchar_t *, int);

inline RexxNumberString *new_numberstring(stringchar_t *s, stringsize_t l)
{
    return RexxNumberString::newInstance(s, l);
}

inline RexxNumberString *new_numberstring(wholenumber_t n)
{
    return RexxNumberString::newInstance(n);
}

inline RexxNumberString *new_numberstring(stringsize_t n)
{
    return RexxNumberString::newInstance(n);
}

inline RexxNumberString *new_numberstring(int64_t n)
{
    return RexxNumberString::newInstance(n);
}

inline RexxNumberString *new_numberstring(uint64_t n)
{
    return RexxNumberString::newInstance(n);
}

inline RexxNumberString *new_numberstring(double n)
{
    return RexxNumberString::newInstance(n);
}

inline RexxNumberString *new_numberstring(float n)
{
    return RexxNumberString::newInstance((double)n);
}
#endif
