/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2005-2026 Rexx Language Association. All rights reserved.    */
/*                                                                            */
/* This program and the accompanying materials are made available under       */
/* the terms of the Common Public License v1.0 which accompanies this         */
/* distribution. A copy is also available at the following address:           */
/* https://www.oorexx.org/license.html                                        */
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
/*                                                                            */
/* RexxUnicodeServices Class                                                  */
/*                                                                            */
/******************************************************************************/
#include "RexxCore.h"
#include "ProtectedObject.hpp"
#include "ActivityManager.hpp" // for reportException
#include "MethodArguments.hpp"
#include "VariableReference.hpp"
#include "MutableBufferClass.hpp"


#include "Unicode/utf8proc/utf8proc.h"
#include "UnicodeServices.hpp"

// singleton class instance
RexxClass *RexxUnicodeServicesClass::classInstance = OREF_NULL;


/**
 * Create initial bootstrap objects
 */
void RexxUnicodeServicesClass::createInstance()
{
    CLASS_CREATE(RexxUnicodeServices);
}


/**
 * The Rexx accessible class NEW method.  This raises an
 * error because RexxUnicodeServices has no instance.
 *
 * @param args   The NEW args
 * @param argc   The count of arguments
 *
 * @return Never returns.
 */
RexxObject *RexxUnicodeServicesClass::newRexx(RexxObject **init_args, size_t argCount)
{
    // This class has no instance...
    reportException(Error_Unsupported_new_method, ((RexxClass *)this)->getId());
    return TheNilObject;
}


/**
 * An override for the copy method because RexxUnicodeServices cannot be copied.
 *
 * @return Never returns.
 */
RexxObject *RexxUnicodeServicesClass::copyRexx()
{
    // This class cannot be copied.
    reportException(Error_Unsupported_copy_method, ((RexxClass *)this)->getId());
    return TheNilObject;
}


/**
 * NOT USED
 * Allocate a new RexxUnicodeServices object.
 *
 * @param size   The size of the object.
 *
 * @return The newly allocated object.
 */
void *RexxUnicodeServicesClass::operator new(size_t size)
{
    return new_object(size, T_RexxUnicodeServices);
}


/**
 * NOT USED
 * Perform garbage collection on a live object.
 *
 * @param liveMark The current live mark.
 */
void RexxUnicodeServicesClass::live(size_t liveMark)
{
    memory_mark(objectVariables);
}


/**
 * NOT USED
 * Perform generalized live marking on an object.  This is
 * used when mark-and-sweep processing is needed for purposes
 * other than garbage collection.
 *
 * @param reason The reason for the marking call.
 */
void RexxUnicodeServicesClass::liveGeneral(MarkReason reason)
{
    memory_mark_general(objectVariables);
}


/**
 * NOT USED
 * Flatten a source object.
 *
 * @param envelope The envelope that will hold the flattened object.
 */
void RexxUnicodeServicesClass::flatten(Envelope *envelope)
{
    setUpFlatten(RexxUnicodeServicesClass)
    flattenRef(objectVariables);
    cleanUpFlatten
}


// todo: move to RexxInfo
RexxInteger *RexxUnicodeServicesClass::systemIsLittleEndian()
{
    bool bigEndian = Interpreter::isBigEndian();
    return bigEndian ? TheFalseObject : TheTrueObject;
}


/******************************************************************************/
/*                                                                            */
/* Helpers                                                                    */
/*                                                                            */
/******************************************************************************/

RexxObject *get(RexxObject **arguments, size_t argCount, size_t index, RexxObject *defaultValue)
{
    if (index >= argCount) return defaultValue;
    RexxObject *value = arguments[index];
    return value != OREF_NULL ? value : defaultValue;
}


ssize_t integerRange(RexxObject *obj, ssize_t min, ssize_t max, RexxErrorCodes error, const char *errorMessage)
{
    if (obj != OREF_NULL)
    {
        RexxInteger *integer = obj->requestInteger();
        if (integer != TheNilObject)
        {
            wholenumber_t value = integer->getValue();
            if (value >= min && value <= max) return value;
        }
    }
    reportException(error, errorMessage);
    return 0; // To avoid warning, must return something (should never reach this line)
}


ssize_t integer(RexxObject *obj, const char *errorMessage)
{
    if (obj != OREF_NULL)
    {
        RexxInteger *integer = obj->requestInteger();
        if (integer != TheNilObject) return integer->getValue();
    }
    reportException(Error_Invalid_argument_user_defined, errorMessage);
    return 0; // To avoid warning, must return something (should never reach this line)
}


/******************************************************************************/
/*                                                                            */
/* UTF8Proc Extensions                                                        */
/*                                                                            */
/******************************************************************************/

#define DECODE_ERROR_1(ident, arg1, ret) \
{ \
    snprintf(errcode, errcodeSize, #ident); \
    snprintf(errmsg, errmsgSize, ident##___MSG1, (arg1)); \
    return ret; \
}

#define DECODE_ERROR_2(ident, arg1, arg2, ret) \
{ \
    snprintf(errcode, errcodeSize, #ident); \
    snprintf(errmsg, errmsgSize, ident##___MSG2, (arg1), (arg2)); \
    return ret; \
}

#define DECODE_ERROR_3(ident, arg1, arg2, arg3, ret) \
{ \
    snprintf(errcode, errcodeSize, #ident); \
    snprintf(errmsg, errmsgSize, ident##___MSG3, (arg1), (arg2), (arg3)); \
    return ret; \
}

// Example:
// To return the error message CONTINUATION_ERROR_RANGE___MSG3,
// use the macro DECODE_ERROR_3 because the message takes 3 arguments,
// and pass the ident CONTINUATION_ERROR_RANGE, the 3 arguments and the returned value.
#define CONTINUATION_ERROR_RANGE___MSG3 "Invalid continuation byte %i ('%02X'x) at byte-position %zu (codepoint > U+10FFFF)"
#define CONTINUATION_HIGH_SURROGATE___MSG3 "Invalid continuation byte %i ('%02X'x) at byte-position %zu (high surrogate)"
#define CONTINUATION_LOW_SURROGATE___MSG3 "Invalid continuation byte %i ('%02X'x) at byte-position %zu (low surrogate)"
#define CONTINUATION___MSG3 "Invalid continuation byte %i ('%02X'x) at byte-position %zu"
#define CONTINUATION_NON_SHORTEST_FORM___MSG3 "Invalid continuation byte %i ('%02X'x) at byte-position %zu (non-shortest form)"
#define START_ERROR_RANGE___MSG2 "Invalid start byte %i ('%02X'x) (codepoint > U+10FFFF)"
#define START_NON_SHORTEST_FORM___MSG2 "Invalid start byte %i ('%02X'x) (non-shortest form)"
#define TRUNCATED___MSG1 "Truncated, expected %i bytes"

/**
 * utf8proc_iterate returns only UTF8PROC_ERROR_INVALIDUTF8 in the case of a malformed byte sequence.
 * For accurate analysis, more detailed errors must be returned.
 * To correctly support "U+FFFD Substitution of Maximal Subparts", the number of bytes to be replaced by U+FFFD must be returned.
 *
 * Modifications:
 *
 * The returned value is always a number of bytes:
 * - either the length in bytes of the decoded codepoint (>= 0)
 * - or the number of bytes to skip and replace by U+FFFD (< 0)
 *
 * New "index" argument for accurate error messages:
 * 1-based index in the Rexx string of the current codepoint (str = string + index - 1)
 *
 * New "errcode" out-argument to return an error code.
 *
 * New "errmsg" out-argument to return a detailed error message.
 */
#define utf_cont(ch)  (((ch) & 0xc0) == 0x80)
utf8proc_ssize_t utf8proc_iterate_extended(
  const utf8proc_uint8_t *str, utf8proc_ssize_t strlen, utf8proc_int32_t *dst,
  size_t index,
  char errcode[], size_t errcodeSize,
  char errmsg[], size_t errmsgSize
) {
  utf8proc_int32_t uc;
  const utf8proc_uint8_t *end;

  *dst = -1;
  *errcode = '\0';
  *errmsg = '\0';

  if (!strlen) return 0;

  end = str + ((strlen < 0) ? 4 : strlen);
  uc = *str++;
  if (uc < 0x80) {
    *dst = uc;
    return 1;
  }

  // Must be between 0xc2 and 0xf4 inclusive to be valid
  // if ((utf8proc_uint32_t)(uc - 0xc2) > (0xf4-0xc2)) return UTF8PROC_ERROR_INVALIDUTF8;
  if (uc < 0xc2) DECODE_ERROR_2(START_NON_SHORTEST_FORM, uc, uc, -1);
  if (uc > 0xf4) DECODE_ERROR_2(START_ERROR_RANGE, uc, uc, -1);

  if (uc < 0xe0) {         // 2-byte sequence 110xxxxx (C0..DF but only C2..DF is valid)
     // Must have valid continuation character
     // if (str >= end || !utf_cont(*str)) return UTF8PROC_ERROR_INVALIDUTF8;
     if (str >= end) DECODE_ERROR_1(TRUNCATED, 2, -1);
     if (!utf_cont(*str)) DECODE_ERROR_3(CONTINUATION, *str, *str, index + 1, -1);
     *dst = ((uc & 0x1f)<<6) | (*str & 0x3f);
     return 2;
  }

  if (uc < 0xf0) {        // 3-byte sequence 1110xxxx
     // if ((str + 1 >= end) || !utf_cont(*str) || !utf_cont(str[1]))
     //    return UTF8PROC_ERROR_INVALIDUTF8;
     if (str >= end) DECODE_ERROR_1(TRUNCATED, 3, -1);
     if (str + 1 >= end) DECODE_ERROR_1(TRUNCATED, 3, -2);
     if (!utf_cont(*str)) DECODE_ERROR_3(CONTINUATION, *str, *str, index + 1, -1);
     if (uc == 0xed && *str > 0x9f)
     {   // case #1
         if (*str <= 0xaf) DECODE_ERROR_3(CONTINUATION_HIGH_SURROGATE, *str, *str, index + 1, -1)
         else DECODE_ERROR_3(CONTINUATION_LOW_SURROGATE, *str, *str, index + 1, -1);
     }
     if (uc == 0xe0 && *str < 0xa0) DECODE_ERROR_3(CONTINUATION_NON_SHORTEST_FORM, *str, *str, index + 1, -1); // case #2 — non-shortest form
     if (!utf_cont(str[1])) DECODE_ERROR_3(CONTINUATION, *str, *str, index + 2, -2)

     // Check for surrogate chars
     // if (uc == 0xed && *str > 0x9f)
     //     return UTF8PROC_ERROR_INVALIDUTF8; // case #1

     uc = ((uc & 0xf)<<12) | ((*str & 0x3f)<<6) | (str[1] & 0x3f);
     // if (uc < 0x800)
     //    return UTF8PROC_ERROR_INVALIDUTF8; // case #2
     *dst = uc;
     return 3;
  }

  // 4-byte sequence 11110xxx
  // Must have 3 valid continuation characters
  // if ((str + 2 >= end) || !utf_cont(*str) || !utf_cont(str[1]) || !utf_cont(str[2]))
  //    return UTF8PROC_ERROR_INVALIDUTF8;
  if (str >= end) DECODE_ERROR_1(TRUNCATED, 4, -1);
  if (str + 1 >= end) DECODE_ERROR_1(TRUNCATED, 4, -2);
  if (str + 2 >= end) DECODE_ERROR_1(TRUNCATED, 4, -3);
  if (!utf_cont(*str)) DECODE_ERROR_3(CONTINUATION, *str, *str, index + 1, -1);
  if (uc == 0xf0 && *str < 0x90) DECODE_ERROR_3(CONTINUATION_NON_SHORTEST_FORM, *str, *str, index + 1, -1); // case #3
  if (uc == 0xf4 && *str >= 0x90) DECODE_ERROR_3(CONTINUATION_ERROR_RANGE, *str, *str, index + 1, -1); // case #4
  if (!utf_cont(str[1])) DECODE_ERROR_3(CONTINUATION, *str, *str, index + 2, -2);
  if (!utf_cont(str[2])) DECODE_ERROR_3(CONTINUATION, *str, *str, index + 3, -3);

  // Make sure in correct range (0x10000 - 0x10ffff)
  // if (uc == 0xf0) {
  //   if (*str < 0x90) return UTF8PROC_ERROR_INVALIDUTF8; // case #3
  // } else if (uc == 0xf4) {
  //   if (*str > 0x8f) return UTF8PROC_ERROR_INVALIDUTF8; // case #4
  // }
  *dst = ((uc & 7)<<18) | ((*str & 0x3f)<<12) | ((str[1] & 0x3f)<<6) | (str[2] & 0x3f);
  return 4;
}


/******************************************************************************/
/*                                                                            */
/* UTF8Proc Services                                                          */
/*                                                                            */
/******************************************************************************/

void raiseError(utf8proc_ssize_t errcode)
{
    const char *errmsg = utf8proc_errmsg(errcode);
    switch (errcode)
    {
        case UTF8PROC_ERROR_NOMEM:
        case UTF8PROC_ERROR_OVERFLOW:
            reportException(Error_System_resources_user_defined, errmsg);
        case UTF8PROC_ERROR_INVALIDUTF8:
        case UTF8PROC_ERROR_NOTASSIGNED:
            reportException(Error_Invalid_character_string_user_defined, errmsg);
        case UTF8PROC_ERROR_INVALIDOPTS:
            reportException(Error_Invalid_argument_user_defined, errmsg);
        default:
            reportException(Error_System_service_user_defined, errmsg);
    }
}


RexxString *RexxUnicodeServicesClass::utf8procVersion()
{
    return new_string(utf8proc_version());
}


RexxString *RexxUnicodeServicesClass::unicodeVersion()
{
    return new_string(utf8proc_unicode_version());
}


/**
 * Given a string and a byte index, return a codepoint and its size in bytes.
 *
 * @param rexxString    (in) An UTF-8 string.
 * @param rexxIndexB    (in) The byte index (1-based) of the encoded codepoint in rexxString
 * @param refSizeB      (out) The number of bytes read to decode the codepoint (negative if error),
 *                            or 0 if rexxIndexB is outside the rexxString index range.
 * @param refErrorCode  (out) The null string "" if a valid codepoint could be read,
 *                            or the error code otherwise.
 * @param refErrorMsg   (out) The null string "" if a valid codepoint could be read,
 *                            or the error message otherwise.
 *
 * @return The codepoint if a valid codepoint could be read,
 *         or -1 otherwise.
 */
RexxInteger *RexxUnicodeServicesClass::utf8DecodeCodepoint(RexxString *string, RexxInteger *indexB, VariableReference *refSizeB, VariableReference *refErrorCode, VariableReference *refErrorMsg)
{
    // Check arguments

    // Yes! Accept only a real string because the size returned with refSizeB must be applied on a real string, not on a Text instance (for example).
        // RexxString *pstring = stringArgument(string, "string"); // Protected<RexxString> not needed
        // classArgument(string, TheStringClass, "string"); // not enough restrictive because an instance of a subclass is accepted
    requiredArgument(string, "string");
    if (string->classObject() != TheStringClass) reportException(Error_Invalid_argument_noclass, "string", TheStringClass->getId());

    size_t index = positionArgument(indexB, "indexB"); // 1-based, range 1..n
    if (refSizeB != OREF_NULL) classArgument(refSizeB, TheVariableReferenceClass, "refSizeB");
    if (refErrorCode != OREF_NULL) classArgument(refErrorCode, TheVariableReferenceClass, "refErrorCode");
    if (refErrorMsg != OREF_NULL) classArgument(refErrorMsg, TheVariableReferenceClass, "refErrorMsg");

    // Default output values
    if (refSizeB != OREF_NULL) refSizeB->setValue(RexxInteger::integerZero);
    if (refErrorCode != OREF_NULL) refErrorCode->setValue(GlobalNames::NULLSTRING);
    if (refErrorMsg != OREF_NULL) refErrorMsg->setValue(GlobalNames::NULLSTRING);

    if (index > string->getLength()) return RexxInteger::integerMinusOne;

    // cast needed because cannot initialize a variable of type 'const utf8proc_uint8_t *' (aka 'const unsigned char *') with an rvalue of type 'const char *
    const utf8proc_uint8_t *str = (const utf8proc_uint8_t *) string->getStringData();
    utf8proc_ssize_t pos = index - 1; // 0-based
    utf8proc_ssize_t remainingLength = string->getLength() - pos;
    utf8proc_int32_t codepoint;
    const size_t errcodeSize = 50;
    char errcode[errcodeSize];
    const size_t errmsgSize = 200;
    char errmsg[errmsgSize];
    utf8proc_ssize_t size = utf8proc_iterate_extended(str + pos, remainingLength, &codepoint, index, errcode, errcodeSize, errmsg, errmsgSize);

    if (refSizeB != OREF_NULL && refSizeB != 0)
    {
        RexxInteger *rexxSize = new_integer(size); // Protected<RexxInteger> not needed
        refSizeB->setValue(rexxSize);
    }

    if (refErrorCode != OREF_NULL && *errcode != '\0' )
    {
        RexxString *rexxErrcode = new_string(errcode); // Protected<RexxString> not needed
        refErrorCode->setValue(rexxErrcode);
    }

    if (refErrorMsg != OREF_NULL && *errmsg != '\0')
    {
        RexxString *rexxErrmsg = new_string(errmsg); // Protected<RexxString> not needed
        refErrorMsg->setValue(rexxErrmsg);
    }

    return new_integer(codepoint);
}


/**
 * Append a codepoint's UTF-8 encoding into the specified mutable buffer.
 *
 * @param rexxCodepoint (in)     The codepoint to encode.
 * @param destination   (in-out) The mutable buffer to update.
 * @param refSizeB      (out)    The size of the encoded byte sequence (0..4)
 *                               The size is 0 if the codepoint is not in the
 *                               range 0..10FFFF
 *
 * @return The mutable buffer passed with the destination argument.
 */
MutableBuffer *RexxUnicodeServicesClass::utf8EncodeCodepoint(RexxInteger *rexxCodepoint, MutableBuffer *destination, VariableReference *refSizeB)
{
    // Check arguments
    requiredArgument(rexxCodepoint, "codepoint");
    utf8proc_int32_t codepoint = (utf8proc_int32_t)integer(rexxCodepoint, "codepoint must be an integer");
    classArgument(destination, TheMutableBufferClass, "destination");
    if (refSizeB != OREF_NULL) classArgument(refSizeB, TheVariableReferenceClass, "refSizeB");

    // Default output values
    if (refSizeB != OREF_NULL) refSizeB->setValue(RexxInteger::integerZero);

    char buffer[4];
    utf8proc_ssize_t size = utf8proc_encode_char(codepoint, (utf8proc_uint8_t *)buffer);

    if (refSizeB != OREF_NULL && refSizeB != 0)
    {
        RexxInteger *rexxSize = new_integer(size); // Protected<RexxInteger> not needed
        refSizeB->setValue(rexxSize);
    }

    destination->append(buffer, size);
    return destination;
}


/**
 * Given a pair of consecutive codepoints, return whether a grapheme break is
 * permitted between them.
 *
 * @param array An array of 3 items:
 *     codepoint1 [IN]     The first codepoint.
 *     codepoint2 [IN]     The second codepoint.
 *     state      [IN OUT] Initial value must be 0.
 *
 * @return .true if a grapheme break is permitted, .false otherwise.
 */
RexxInteger *RexxUnicodeServicesClass::graphemeBreak(ArrayClass *array)
{
    // No need to use arrayArgument, we really want an array already GC protected by the caller
    classArgument(array, TheArrayClass, "1"); // ARG_ONE not supported
    // array = arrayArgument(array, ARG_ONE);
    // ProtectedObject p(array);
    utf8proc_int32_t codepoint1 = (utf8proc_int32_t)integerRange((RexxObject *)array->get(1), 0, SSIZE_MAX, Error_Invalid_argument_user_defined, "GraphemeBreak: The first codepoint must be a non negative integer");
    utf8proc_int32_t codepoint2 = (utf8proc_int32_t)integerRange((RexxObject *)array->get(2), 0, SSIZE_MAX, Error_Invalid_argument_user_defined, "GraphemeBreak: The second codepoint must be a non negative integer");
    utf8proc_int32_t state =      (utf8proc_int32_t)integerRange((RexxObject *)array->get(3), 0, SSIZE_MAX, Error_Invalid_argument_user_defined, "GraphemeBreak:The state must be a non negative integer");
    utf8proc_bool graphemeBreak = utf8proc_grapheme_break_stateful(codepoint1, codepoint2, &state);
    array->put(new_integer(state), 3); // Output argument
    return graphemeBreak ? TheTrueObject : TheFalseObject;
}


// https://www.unicode.org/versions/Unicode17.0.0/core-spec/chapter-4/#G124142
const char *General_Category[] =
{
    "Cn", "Unassigned",
    "Lu", "Uppercase_Letter",
    "Ll", "Lowercase_Letter",
    "Lt", "Titlecase_Letter",
    "Lm", "Modifier_Letter",
    "Lo", "Other_Letter",
    "Mn", "Nonspacing_Mark",
    "Mc", "Spacing_Mark",
    "Me", "Enclosing_Mark",
    "Nd", "Decimal_Number",
    "Nl", "Letter_Number",
    "No", "Other_Number",
    "Pc", "Connector_Punctuation",
    "Pd", "Dash_Punctuation",
    "Ps", "Open_Punctuation",
    "Pe", "Close_Punctuation",
    "Pi", "Initial_Punctuation",
    "Pf", "Final_Punctuation",
    "Po", "Other_Punctuation",
    "Sm", "Math_Symbol",
    "Sc", "Currency_Symbol",
    "Sk", "Modifier_Symbol",
    "So", "Other_Symbol",
    "Zs", "Space_Separator",
    "Zl", "Line_Separator",
    "Zp", "Paragraph_Separator",
    "Cc", "Control",
    "Cf", "Format",
    "Cs", "Surrogate",
    "Co", "Private_Use"
};

RexxInteger *RexxUnicodeServicesClass::codepointCategory(RexxInteger *rexxCodepoint, VariableReference *refCode, VariableReference *refLabel)
{
    requiredArgument(rexxCodepoint, "codepoint");
    utf8proc_int32_t codepoint = (utf8proc_int32_t)integer(rexxCodepoint, "codepoint must be an integer");
    if (refCode != OREF_NULL) classArgument(refCode, TheVariableReferenceClass, "refCode");
    if (refLabel != OREF_NULL) classArgument(refLabel, TheVariableReferenceClass, "refLabel");

    const utf8proc_property_t *property = utf8proc_get_property(codepoint);
    utf8proc_propval_t category = property->category; // 0..n
    utf8proc_propval_t index = category;

    // Check array out-of-bounds
    int count = sizeof(General_Category) / sizeof(General_Category[0]);
    bool rangeOk = index >= 0 && index < count/2;

    if (refCode != OREF_NULL)
    {
        const char *strCode = rangeOk ? General_Category[2 * index] : "?";
        RexxString *rexxCode = new_string(strCode); // Protected<RexxString> not needed
        refCode->setValue(rexxCode);
    }

    if (refLabel != OREF_NULL)
    {
        const char *strLabel = rangeOk ? General_Category[1 + 2 * index] : "?";
        RexxString *rexxLabel = new_string(strLabel); // Protected<RexxString> not needed
        refLabel->setValue(rexxLabel);
    }

    return new_integer(category); // see utf8proc_category_t
}


RexxInteger *RexxUnicodeServicesClass::codepointCombiningClass(RexxInteger *rexxCodepoint)
{
    requiredArgument(rexxCodepoint, "codepoint");
    utf8proc_int32_t codepoint = (utf8proc_int32_t)integer(rexxCodepoint, "codepoint must be an integer");
    const utf8proc_property_t *property = utf8proc_get_property(codepoint);
    return new_integer(property->combining_class); // see utf8proc_category_t
}


// https://www.unicode.org/reports/tr9/#Table_Bidirectional_Character_Types
const char *Bidirectional_Character_Types[]=
{
    "L", "Left-to-Right",
    "LRE", "Left-to-Right Embedding",
    "LRO", "Left-to-Right Override",
    "R", "Right-to-Left",
    "AL", "Right-to-Left Arabic",
    "RLE", "Right-to-Left Embedding",
    "RLO", "Right-to-Left Override",
    "PDF", "Pop Directional Format",
    "EN", "European Number",
    "ES", "European Number Separator",
    "ET", "European Number Terminator",
    "AN", "Arabic Number",
    "CS", "Common Number Separator",
    "NSM", "Nonspacing Mark",
    "BN", "Boundary Neutral",
    "B", "Paragraph Separator",
    "S", "Segment Separator",
    "WS", "Whitespace",
    "ON", "Other Neutrals",
    "LRI", "Left-to-Right Isolate",
    "RLI", "Right-to-Left Isolate",
    "FSI", "First Strong Isolate",
    "PDI", "Pop Directional Isolate"
};

RexxInteger *RexxUnicodeServicesClass::codepointBidiClass(RexxInteger *rexxCodepoint, VariableReference *refCode, VariableReference *refLabel)
{
    requiredArgument(rexxCodepoint, "codepoint");
    utf8proc_int32_t codepoint = (utf8proc_int32_t)integer(rexxCodepoint, "codepoint must be an integer");
    if (refCode != OREF_NULL) classArgument(refCode, TheVariableReferenceClass, "refCode");
    if (refLabel != OREF_NULL) classArgument(refLabel, TheVariableReferenceClass, "refLabel");

    const utf8proc_property_t *property = utf8proc_get_property(codepoint);
    utf8proc_propval_t bidi_class = property->bidi_class; // 1..n
    utf8proc_propval_t index = bidi_class - 1;

    // Check array out-of-bounds
    int count = sizeof(Bidirectional_Character_Types) / sizeof(Bidirectional_Character_Types[0]);
    bool rangeOk = index >= 0 && index < count/2;

    if (refCode != OREF_NULL)
    {
        const char *strCode = rangeOk ? Bidirectional_Character_Types[2 * index] : "?";
        RexxString *rexxCode = new_string(strCode); // Protected<RexxString> not needed
        refCode->setValue(rexxCode);
    }

    if (refLabel != OREF_NULL)
    {
        const char *strLabel = rangeOk ? Bidirectional_Character_Types[1 + 2 * index] : "?";
        RexxString *rexxLabel = new_string(strLabel); // Protected<RexxString> not needed
        refLabel->setValue(rexxLabel);
    }

    return new_integer(bidi_class); // see utf8proc_category_t
}


RexxInteger *RexxUnicodeServicesClass::codepointBidiMirrored(RexxInteger *rexxCodepoint)
{
    requiredArgument(rexxCodepoint, "codepoint");
    utf8proc_int32_t codepoint = (utf8proc_int32_t)integer(rexxCodepoint, "codepoint must be an integer");
    const utf8proc_property_t *property = utf8proc_get_property(codepoint);
    return property->bidi_mirrored ? TheTrueObject : TheFalseObject;
}


#if 0 // TODO
/** Decomposition type. */
typedef enum {
  UTF8PROC_DECOMP_TYPE_FONT      = 1, /**< Font */
  UTF8PROC_DECOMP_TYPE_NOBREAK   = 2, /**< Nobreak */
  UTF8PROC_DECOMP_TYPE_INITIAL   = 3, /**< Initial */
  UTF8PROC_DECOMP_TYPE_MEDIAL    = 4, /**< Medial */
  UTF8PROC_DECOMP_TYPE_FINAL     = 5, /**< Final */
  UTF8PROC_DECOMP_TYPE_ISOLATED  = 6, /**< Isolated */
  UTF8PROC_DECOMP_TYPE_CIRCLE    = 7, /**< Circle */
  UTF8PROC_DECOMP_TYPE_SUPER     = 8, /**< Super */
  UTF8PROC_DECOMP_TYPE_SUB       = 9, /**< Sub */
  UTF8PROC_DECOMP_TYPE_VERTICAL = 10, /**< Vertical */
  UTF8PROC_DECOMP_TYPE_WIDE     = 11, /**< Wide */
  UTF8PROC_DECOMP_TYPE_NARROW   = 12, /**< Narrow */
  UTF8PROC_DECOMP_TYPE_SMALL    = 13, /**< Small */
  UTF8PROC_DECOMP_TYPE_SQUARE   = 14, /**< Square */
  UTF8PROC_DECOMP_TYPE_FRACTION = 15, /**< Fraction */
  UTF8PROC_DECOMP_TYPE_COMPAT   = 16, /**< Compat */
} utf8proc_decomp_type_t;
#endif

RexxInteger *RexxUnicodeServicesClass::codepointDecompositionType(RexxInteger *rexxCodepoint)
{
    requiredArgument(rexxCodepoint, "codepoint");
    utf8proc_int32_t codepoint = (utf8proc_int32_t)integer(rexxCodepoint, "codepoint must be an integer");
    const utf8proc_property_t *property = utf8proc_get_property(codepoint);
    return new_integer(property->decomp_type); // see utf8proc_decomp_type_t

    /* not returned, internal use
    utf8proc_uint16_t decomp_seqindex;
    utf8proc_uint16_t casefold_seqindex;
    utf8proc_uint16_t uppercase_seqindex;
    utf8proc_uint16_t lowercase_seqindex;
    utf8proc_uint16_t titlecase_seqindex;
    utf8proc_uint16_t comb_index;
    unsigned bidi_mirrored:1;
    unsigned comp_exclusion:1;
    */
}


RexxInteger *RexxUnicodeServicesClass::codepointIgnorable(RexxInteger *rexxCodepoint)
{
    requiredArgument(rexxCodepoint, "codepoint");
    utf8proc_int32_t codepoint = (utf8proc_int32_t)integer(rexxCodepoint, "codepoint must be an integer");
    const utf8proc_property_t *property = utf8proc_get_property(codepoint);
    return property->ignorable ? TheTrueObject : TheFalseObject;
}


RexxInteger *RexxUnicodeServicesClass::codepointControlBoundary(RexxInteger *rexxCodepoint)
{
    requiredArgument(rexxCodepoint, "codepoint");
    utf8proc_int32_t codepoint = (utf8proc_int32_t)integer(rexxCodepoint, "codepoint must be an integer");
    const utf8proc_property_t *property = utf8proc_get_property(codepoint);
    return property->control_boundary ? TheTrueObject : TheFalseObject;
}


RexxInteger *RexxUnicodeServicesClass::codepointCharWidth(RexxInteger *rexxCodepoint)
{
    requiredArgument(rexxCodepoint, "codepoint");
    utf8proc_int32_t codepoint = (utf8proc_int32_t)integer(rexxCodepoint, "codepoint must be an integer");
    const utf8proc_property_t *property = utf8proc_get_property(codepoint);
    return new_integer(property->charwidth);

    /* not returned, not used?
    unsigned pad:2;
    */
}


#if 0 // TODO
/** Boundclass property. (TR29) */
typedef enum {
  UTF8PROC_BOUNDCLASS_START              =  0, /**< Start */
  UTF8PROC_BOUNDCLASS_OTHER              =  1, /**< Other */
  UTF8PROC_BOUNDCLASS_CR                 =  2, /**< Cr */
  UTF8PROC_BOUNDCLASS_LF                 =  3, /**< Lf */
  UTF8PROC_BOUNDCLASS_CONTROL            =  4, /**< Control */
  UTF8PROC_BOUNDCLASS_EXTEND             =  5, /**< Extend */
  UTF8PROC_BOUNDCLASS_L                  =  6, /**< L */
  UTF8PROC_BOUNDCLASS_V                  =  7, /**< V */
  UTF8PROC_BOUNDCLASS_T                  =  8, /**< T */
  UTF8PROC_BOUNDCLASS_LV                 =  9, /**< Lv */
  UTF8PROC_BOUNDCLASS_LVT                = 10, /**< Lvt */
  UTF8PROC_BOUNDCLASS_REGIONAL_INDICATOR = 11, /**< Regional indicator */
  UTF8PROC_BOUNDCLASS_SPACINGMARK        = 12, /**< Spacingmark */
  UTF8PROC_BOUNDCLASS_PREPEND            = 13, /**< Prepend */
  UTF8PROC_BOUNDCLASS_ZWJ                = 14, /**< Zero Width Joiner */

  /* the following are no longer used in Unicode 11, but we keep
     the constants here for backward compatibility */
  UTF8PROC_BOUNDCLASS_E_BASE             = 15, /**< Emoji Base */
  UTF8PROC_BOUNDCLASS_E_MODIFIER         = 16, /**< Emoji Modifier */
  UTF8PROC_BOUNDCLASS_GLUE_AFTER_ZWJ     = 17, /**< Glue_After_ZWJ */
  UTF8PROC_BOUNDCLASS_E_BASE_GAZ         = 18, /**< E_BASE + GLUE_AFTER_ZJW */

  /* the Extended_Pictographic property is used in the Unicode 11
     grapheme-boundary rules, so we store it in the boundclass field */
  UTF8PROC_BOUNDCLASS_EXTENDED_PICTOGRAPHIC = 19,
  UTF8PROC_BOUNDCLASS_E_ZWG = 20, /* UTF8PROC_BOUNDCLASS_EXTENDED_PICTOGRAPHIC + ZWJ */
} utf8proc_boundclass_t;
#endif

RexxInteger *RexxUnicodeServicesClass::codepointBoundClass(RexxInteger *rexxCodepoint)
{
    requiredArgument(rexxCodepoint, "codepoint");
    utf8proc_int32_t codepoint = (utf8proc_int32_t)integer(rexxCodepoint, "codepoint must be an integer");
    const utf8proc_property_t *property = utf8proc_get_property(codepoint);
    return new_integer(property->boundclass); // see utf8proc_boundclass_t
}


RexxInteger *RexxUnicodeServicesClass::codepointToLower(RexxInteger *rexxCodepoint)
{
    requiredArgument(rexxCodepoint, "codepoint");
    utf8proc_int32_t codepoint = (utf8proc_int32_t)integer(rexxCodepoint, "codepoint must be an integer");
    return new_integer(utf8proc_tolower(codepoint));
}


RexxInteger *RexxUnicodeServicesClass::codepointToUpper(RexxInteger *rexxCodepoint)
{
    requiredArgument(rexxCodepoint, "codepoint");
    utf8proc_int32_t codepoint = (utf8proc_int32_t)integer(rexxCodepoint, "codepoint must be an integer");
    return new_integer(utf8proc_toupper(codepoint));
}


RexxInteger *RexxUnicodeServicesClass::codepointToTitle(RexxInteger *rexxCodepoint)
{
    requiredArgument(rexxCodepoint, "codepoint");
    utf8proc_int32_t codepoint = (utf8proc_int32_t)integer(rexxCodepoint, "codepoint must be an integer");
    return new_integer(utf8proc_totitle(codepoint));
}


RexxInteger *RexxUnicodeServicesClass::codepointIsLower(RexxInteger *rexxCodepoint)
{
    requiredArgument(rexxCodepoint, "codepoint");
    utf8proc_int32_t codepoint = (utf8proc_int32_t)integer(rexxCodepoint, "codepoint must be an integer");
    return utf8proc_islower(codepoint) ? TheTrueObject : TheFalseObject;

}


RexxInteger *RexxUnicodeServicesClass::codepointIsUpper(RexxInteger *rexxCodepoint)
{
    requiredArgument(rexxCodepoint, "codepoint");
    utf8proc_int32_t codepoint = (utf8proc_int32_t)integer(rexxCodepoint, "codepoint must be an integer");
    return utf8proc_isupper(codepoint) ? TheTrueObject : TheFalseObject;

}


// utf8proc helper
RexxString *normalize(RexxString *string, utf8proc_option_t options)
{
    utf8proc_uint8_t *retval;
    string = stringArgument(string, ARG_ONE);
    const utf8proc_uint8_t *str = (const utf8proc_uint8_t *)string->getStringData();
    utf8proc_ssize_t strlength = (utf8proc_ssize_t)string->getLength();
    utf8proc_ssize_t reslength = utf8proc_map(str, strlength, &retval, options);
    if (reslength < 0) raiseError(reslength); // here, reslength is an error code
    // Not so easy to optimize memory allocation...
    // utf8proc_map allocates a buffer of 32-bit codepoints
    // and then reuse this same buffer to convert to utf-8
    // In the end, the buffer is reallocated to shrink it.
    RexxString *result = new_string((const char *)retval, reslength);
    free(retval);
    return result;
}


// Can't use one argument per option because the number of arguments would be 1+8=9, whereas the limit in CPPCode::run is 7
// todo: backport the named arguments from Executor.
RexxString *RexxUnicodeServicesClass::utf8Transform(RexxObject **arguments, size_t argCount)
{
    // use strict arg string, casefold = .false, lump= .false, nlf = 0, normalization = 0, stripCC = .false, stripIgnorable= .false, stripMark = .false, stripNA = .false

    if (argCount < 1 ) missingArgument("string");
    RexxString *string = stringArgument(arguments[0], "string");

    RexxObject *rexxCasefold =          TheFalseObject; // default value = .false
    RexxObject *rexxLump =              TheFalseObject; // default value = .false
    RexxObject *rexxNLF =               IntegerZero;    // default value = 0 (0=none, 1=NLF2LF, 2=NLF2LS, 3=NLF2PS)
    RexxObject *rexxNormalization =     IntegerZero;    // default value = 0 (0=none, 1=NFC, 2=NFD, 3=NFKC, 4=NFKD)
    RexxObject *rexxStripCC =           TheFalseObject; // default value = .false
    RexxObject *rexxStripIgnorable =    TheFalseObject; // default value = .false
    RexxObject *rexxStripMark =         TheFalseObject; // default value = .false
    RexxObject *rexxStripNA =           TheFalseObject; // default value = .false

    rexxCasefold =          get(arguments, argCount, 1, rexxCasefold);          // GlobalNames::CASEFOLD
    rexxLump =              get(arguments, argCount, 2, rexxLump);              // GlobalNames::LUMP
    rexxNLF =               get(arguments, argCount, 3, rexxNLF);               // GlobalNames::NLF
    rexxNormalization =     get(arguments, argCount, 4, rexxNormalization);     // GlobalNames::NORMALIZATION
    rexxStripCC =           get(arguments, argCount, 5, rexxStripCC);           // GlobalNames::STRIPCC
    rexxStripIgnorable =    get(arguments, argCount, 6, rexxStripIgnorable);    // GlobalNames::STRIPIGNORABLE
    rexxStripMark =         get(arguments, argCount, 7, rexxStripMark);         // GlobalNames::STRIPMARK
    rexxStripNA =           get(arguments, argCount, 8, rexxStripNA);           // GlobalNames::STRIPNA

    ssize_t casefold =      integerRange(rexxCasefold,          0, 1, Error_Logical_value_user_defined,     "Transform: value of argument \"casefold\" must be 0 or 1");
    ssize_t lump =          integerRange(rexxLump,              0, 1, Error_Logical_value_user_defined,     "Transform: value of argument \"lump\" must be 0 or 1");
    ssize_t nlf =           integerRange(rexxNLF,               0, 3, Error_Invalid_argument_user_defined,  "Transform: value of argument \"nlf\" must be 0..3");
    ssize_t normalization = integerRange(rexxNormalization,     0, 4, Error_Invalid_argument_user_defined,  "Transform: value of argument \"normalization\" must be 0..4");
    ssize_t stripCC =       integerRange(rexxStripCC,           0, 1, Error_Logical_value_user_defined,     "Transform: value of argument \"stripCC\" must be 0 or 1");
    ssize_t stripIgnorable= integerRange(rexxStripIgnorable,    0, 1, Error_Logical_value_user_defined,     "Transform: value of argument \"stripIgnorable\" must be 0 or 1");
    ssize_t stripMark =     integerRange(rexxStripMark,         0, 1, Error_Logical_value_user_defined,     "Transform: value of argument \"stripMark\" must be 0 or 1");
    ssize_t stripNA =       integerRange(rexxStripNA,           0, 1, Error_Logical_value_user_defined,     "Transform: value of argument \"stripNA\" must be 0 or 1");

    int                     options = 0;
    if (casefold)           options |= UTF8PROC_CASEFOLD;
    if (lump)               options |= UTF8PROC_LUMP;
    if (nlf == 1)           options |= UTF8PROC_NLF2LF;
    if (nlf == 2)           options |= UTF8PROC_NLF2LS;
    if (nlf == 3)           options |= UTF8PROC_NLF2PS;
    if (normalization == 1) options |= UTF8PROC_STABLE | UTF8PROC_COMPOSE;                      // NFC
    if (normalization == 2) options |= UTF8PROC_STABLE | UTF8PROC_DECOMPOSE;                    // NFD
    if (normalization == 3) options |= UTF8PROC_STABLE | UTF8PROC_COMPOSE | UTF8PROC_COMPAT;    // NFKC
    if (normalization == 4) options |= UTF8PROC_STABLE | UTF8PROC_DECOMPOSE | UTF8PROC_COMPAT;  // NFKD
    if (stripCC)            options |= UTF8PROC_STRIPCC;
    if (stripIgnorable)     options |= UTF8PROC_IGNORE;
    if (stripMark)          options |= UTF8PROC_STRIPMARK;
    if (stripNA)            options |= UTF8PROC_STRIPNA;

    return normalize(string, utf8proc_option_t(options));
}

#if 0 // options that can be passed with utf8proc_transform

  /** Strip "default ignorable characters" such as SOFT-HYPHEN or ZERO-WIDTH-SPACE. */
  UTF8PROC_IGNORE    = (1<<5),

  /**
   * Indicating that NLF-sequences (LF, CRLF, CR, NEL) are representing a
   * line break, and should be converted to the codepoint for line
   * separation (LS).
   */
   // convert LF, CRLF, CR and NEL into LS
  UTF8PROC_NLF2LS    = (1<<7),

  /**
   * Indicating that NLF-sequences are representing a paragraph break, and
   * should be converted to the codepoint for paragraph separation
   * (PS).
   */
   // convert LF, CRLF, CR and NEL into PS
  UTF8PROC_NLF2PS    = (1<<8),

  /** Indicating that the meaning of NLF-sequences is unknown. */
  // convert LF, CRLF, CR and NEL into LF
  UTF8PROC_NLF2LF    = (UTF8PROC_NLF2LS | UTF8PROC_NLF2PS),

  /** Strips and/or convers control characters.
   *
   * NLF-sequences are transformed into space, except if one of the
   * NLF2LS/PS/LF options is given. HorizontalTab (HT) and FormFeed (FF)
   * are treated as a NLF-sequence in this case.  All other control
   * characters are simply removed.
   */
  UTF8PROC_STRIPCC   = (1<<9),

  /**
   * Performs unicode case folding, to be able to do a case-insensitive
   * string comparison.
   */
  UTF8PROC_CASEFOLD  = (1<<10),

  /** Lumps certain characters together.
   *
   * E.g. HYPHEN U+2010 and MINUS U+2212 to ASCII "-". See lump.md for details.
   *
   * If NLF2LF is set, this includes a transformation of paragraph and
   * line separators to ASCII line-feed (LF).
   */
  UTF8PROC_LUMP      = (1<<12),

  /** Strips all character markings.
   *
   * This includes non-spacing, spacing and enclosing (i.e. accents).
   * @note This option works only with @ref UTF8PROC_COMPOSE or
   *       @ref UTF8PROC_DECOMPOSE
   */
  UTF8PROC_STRIPMARK = (1<<13),

  /**
   * Strip unassigned codepoints.
   */
  UTF8PROC_STRIPNA    = (1<<14),

#endif
