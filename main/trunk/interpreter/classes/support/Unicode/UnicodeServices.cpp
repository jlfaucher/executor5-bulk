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
/* UTF8Proc Duplications (because not exposed)                                */
/* Needed for optimization                                                    */
/*                                                                            */
/* grapheme_break_simple                                                      */
/* grapheme_break_extended                                                    */
/*                                                                            */
/******************************************************************************/

// Duplicated, to update if needed each time utf8proc is updated
static utf8proc_bool grapheme_break_simple(int lbc, int tbc) {
  return
    (lbc == UTF8PROC_BOUNDCLASS_START) ? true :       // GB1
    (lbc == UTF8PROC_BOUNDCLASS_CR &&                 // GB3
     tbc == UTF8PROC_BOUNDCLASS_LF) ? false :         // ---
    (lbc >= UTF8PROC_BOUNDCLASS_CR && lbc <= UTF8PROC_BOUNDCLASS_CONTROL) ? true :  // GB4
    (tbc >= UTF8PROC_BOUNDCLASS_CR && tbc <= UTF8PROC_BOUNDCLASS_CONTROL) ? true :  // GB5
    (lbc == UTF8PROC_BOUNDCLASS_L &&                  // GB6
     (tbc == UTF8PROC_BOUNDCLASS_L ||                 // ---
      tbc == UTF8PROC_BOUNDCLASS_V ||                 // ---
      tbc == UTF8PROC_BOUNDCLASS_LV ||                // ---
      tbc == UTF8PROC_BOUNDCLASS_LVT)) ? false :      // ---
    ((lbc == UTF8PROC_BOUNDCLASS_LV ||                // GB7
      lbc == UTF8PROC_BOUNDCLASS_V) &&                // ---
     (tbc == UTF8PROC_BOUNDCLASS_V ||                 // ---
      tbc == UTF8PROC_BOUNDCLASS_T)) ? false :        // ---
    ((lbc == UTF8PROC_BOUNDCLASS_LVT ||               // GB8
      lbc == UTF8PROC_BOUNDCLASS_T) &&                // ---
     tbc == UTF8PROC_BOUNDCLASS_T) ? false :          // ---
    (tbc == UTF8PROC_BOUNDCLASS_EXTEND ||             // GB9
     tbc == UTF8PROC_BOUNDCLASS_ZWJ ||                // ---
     tbc == UTF8PROC_BOUNDCLASS_SPACINGMARK ||        // GB9a
     lbc == UTF8PROC_BOUNDCLASS_PREPEND) ? false :    // GB9b
    (lbc == UTF8PROC_BOUNDCLASS_E_ZWG &&              // GB11 (requires additional handling below)
     tbc == UTF8PROC_BOUNDCLASS_EXTENDED_PICTOGRAPHIC) ? false : // ----
    (lbc == UTF8PROC_BOUNDCLASS_REGIONAL_INDICATOR &&          // GB12/13 (requires additional handling below)
     tbc == UTF8PROC_BOUNDCLASS_REGIONAL_INDICATOR) ? false :  // ----
    true; // GB999
}


// Duplicated, to update if needed each time utf8proc is updated
static utf8proc_bool grapheme_break_extended(int lbc, int tbc, int licb, int ticb, utf8proc_int32_t *state)
{
  if (state) {
    int state_bc, state_icb; /* boundclass and indic_conjunct_break state */
    if (*state == 0) { /* state initialization */
      state_bc = lbc;
      state_icb = licb == UTF8PROC_INDIC_CONJUNCT_BREAK_CONSONANT ? licb : UTF8PROC_INDIC_CONJUNCT_BREAK_NONE;
    }
    else { /* lbc and licb are already encoded in *state */
      state_bc = *state & 0xff;  // 1st byte of state is bound class
      state_icb = *state >> 8;   // 2nd byte of state is indic conjunct break
    }

    utf8proc_bool break_permitted = grapheme_break_simple(state_bc, tbc) &&
       !(state_icb == UTF8PROC_INDIC_CONJUNCT_BREAK_LINKER
        && ticb == UTF8PROC_INDIC_CONJUNCT_BREAK_CONSONANT); // GB9c

    // Special support for GB9c.  Don't break between two consonants
    // separated 1+ linker characters and 0+ extend characters in any order.
    // After a consonant, we enter LINKER state after at least one linker.
    if (ticb == UTF8PROC_INDIC_CONJUNCT_BREAK_CONSONANT
        || state_icb == UTF8PROC_INDIC_CONJUNCT_BREAK_CONSONANT
        || state_icb == UTF8PROC_INDIC_CONJUNCT_BREAK_EXTEND)
      state_icb = ticb;
    else if (state_icb == UTF8PROC_INDIC_CONJUNCT_BREAK_LINKER)
      state_icb = ticb == UTF8PROC_INDIC_CONJUNCT_BREAK_EXTEND ?
                  UTF8PROC_INDIC_CONJUNCT_BREAK_LINKER : ticb;

    // Special support for GB 12/13 made possible by GB999. After two RI
    // class codepoints we want to force a break. Do this by resetting the
    // second RI's bound class to UTF8PROC_BOUNDCLASS_OTHER, to force a break
    // after that character according to GB999 (unless of course such a break is
    // forbidden by a different rule such as GB9).
    if (state_bc == tbc && tbc == UTF8PROC_BOUNDCLASS_REGIONAL_INDICATOR)
      state_bc = UTF8PROC_BOUNDCLASS_OTHER;
    // Special support for GB11 (emoji extend* zwj / emoji)
    else if (state_bc == UTF8PROC_BOUNDCLASS_EXTENDED_PICTOGRAPHIC) {
      if (tbc == UTF8PROC_BOUNDCLASS_EXTEND) // fold EXTEND codepoints into emoji
        state_bc = UTF8PROC_BOUNDCLASS_EXTENDED_PICTOGRAPHIC;
      else if (tbc == UTF8PROC_BOUNDCLASS_ZWJ)
        state_bc = UTF8PROC_BOUNDCLASS_E_ZWG; // state to record emoji+zwg combo
      else
        state_bc = tbc;
    }
    else
      state_bc = tbc;

    *state = state_bc + (state_icb << 8);
    return break_permitted;
  }
  else
    return grapheme_break_simple(lbc, tbc);
}


/******************************************************************************/
/*                                                                            */
/* UTF8Proc Extensions                                                        */
/*                                                                            */
/* utf8proc_iterate_extended                                                  */
/* utf8proc_iterate_extended_backward                                         */
/*                                                                            */
/******************************************************************************/

#define DECODE_ERROR_1(ident, arg1, ret) \
{ \
    if (errcode != NULL) snprintf(errcode, errcodeSize, #ident); \
    if (errmsg != NULL) snprintf(errmsg, errmsgSize, ident##___MSG1, (arg1)); \
    return ret; \
}

#define DECODE_ERROR_2(ident, arg1, arg2, ret) \
{ \
    if (errcode != NULL) snprintf(errcode, errcodeSize, #ident); \
    if (errmsg != NULL) snprintf(errmsg, errmsgSize, ident##___MSG2, (arg1), (arg2)); \
    return ret; \
}

#define DECODE_ERROR_3(ident, arg1, arg2, arg3, ret) \
{ \
    if (errcode != NULL) snprintf(errcode, errcodeSize, #ident); \
    if (errmsg != NULL) snprintf(errmsg, errmsgSize, ident##___MSG3, (arg1), (arg2), (arg3)); \
    return ret; \
}

// Example:
// To return the error message CONTINUATION_ERROR_RANGE___MSG3,
// use the macro DECODE_ERROR_3 because the message takes 3 arguments,
// and pass the ident CONTINUATION_ERROR_RANGE, the 3 arguments and the returned value.
#define CONTINUATION_ERROR_RANGE___MSG3 "Invalid continuation byte %i ('%02X'x) at byte position %zu (code point > U+10FFFF)"
#define CONTINUATION_HIGH_SURROGATE___MSG3 "Invalid continuation byte %i ('%02X'x) at byte position %zu (high surrogate)"
#define CONTINUATION_LOW_SURROGATE___MSG3 "Invalid continuation byte %i ('%02X'x) at byte position %zu (low surrogate)"
#define CONTINUATION___MSG3 "Invalid continuation byte %i ('%02X'x) at byte position %zu"
#define CONTINUATION_NON_SHORTEST_FORM___MSG3 "Invalid continuation byte %i ('%02X'x) at byte position %zu (non-shortest form)"
#define START_ERROR_RANGE___MSG2 "Invalid start byte %i ('%02X'x) (code point > U+10FFFF)"
#define START_NON_SHORTEST_FORM___MSG2 "Invalid start byte %i ('%02X'x) (non-shortest form)"
#define TRUNCATED___MSG1 "Truncated, expected %i bytes"

/**
 * utf8proc_iterate returns only UTF8PROC_ERROR_INVALIDUTF8 in the case of a malformed byte sequence.
 * For accurate analysis, more detailed errors must be returned.
 * To correctly support "U+FFFD Substitution of Maximal Subparts", the number of bytes to be replaced by U+FFFD must be returned.
 *
 * Reads a single codepoint from the UTF-8 sequence being pointed to by `str`.
 * The maximum number of bytes read is `strlen`, unless `strlen` is
 * negative (in which case up to 4 bytes are read).
 *
 * If a valid codepoint could be read, it is stored in the variable
 * pointed to by `dst`, otherwise that variable will be set to -1.
 *
 * Modifications:
 *
 * The returned value is always a number of bytes:
 * - either the length in bytes of the decoded codepoint (>= 0)
 * - or the number of bytes to skip and replace by U+FFFD (< 0)
 *
 * New "index" argument for accurate error messages:
 * 1-based index of the current codepoint in the Rexx string (str = string + index - 1).
 * May be 0 if errmsg is NULL (in this case, index is not used).
 *
 * New "errcode" out-argument to return an error code.
 * May be NULL if no error code is needed.
 *
 * New "errmsg" out-argument to return a detailed error message.
 * May be NULL if no error message is needed.
 */
#define utf_cont(ch)  (((ch) & 0xc0) == 0x80)
utf8proc_ssize_t utf8proc_iterate_extended(
  const utf8proc_uint8_t *str, utf8proc_ssize_t strlen, utf8proc_int32_t *dst,
  size_t index = 0,
  char *errcode = NULL, size_t errcodeSize = 0,
  char *errmsg = NULL, size_t errmsgSize = 0
) {
  utf8proc_int32_t uc;
  const utf8proc_uint8_t *end;

  *dst = -1;
  if (errcode != NULL) *errcode = '\0';
  if (errmsg != NULL) *errmsg = '\0';

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
     if (!utf_cont(*str)) DECODE_ERROR_3(CONTINUATION, *str, *str, index + 1, -1);
     if (uc == 0xed && *str > 0x9f)
     {   // case #1
         if (*str <= 0xaf) DECODE_ERROR_3(CONTINUATION_HIGH_SURROGATE, *str, *str, index + 1, -1)
         else DECODE_ERROR_3(CONTINUATION_LOW_SURROGATE, *str, *str, index + 1, -1);
     }
     if (uc == 0xe0 && *str < 0xa0) DECODE_ERROR_3(CONTINUATION_NON_SHORTEST_FORM, *str, *str, index + 1, -1); // case #2 — non-shortest form

     if (str + 1 >= end) DECODE_ERROR_1(TRUNCATED, 3, -2);
     if (!utf_cont(str[1])) DECODE_ERROR_3(CONTINUATION, str[1], str[1], index + 2, -2)

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
  if (!utf_cont(*str)) DECODE_ERROR_3(CONTINUATION, *str, *str, index + 1, -1);
  if (uc == 0xf0 && *str < 0x90) DECODE_ERROR_3(CONTINUATION_NON_SHORTEST_FORM, *str, *str, index + 1, -1); // case #3
  if (uc == 0xf4 && *str >= 0x90) DECODE_ERROR_3(CONTINUATION_ERROR_RANGE, *str, *str, index + 1, -1); // case #4

  if (str + 1 >= end) DECODE_ERROR_1(TRUNCATED, 4, -2);
  if (!utf_cont(str[1])) DECODE_ERROR_3(CONTINUATION, str[1], str[1], index + 2, -2);

  if (str + 2 >= end) DECODE_ERROR_1(TRUNCATED, 4, -3);
  if (!utf_cont(str[2])) DECODE_ERROR_3(CONTINUATION, str[2], str[2], index + 3, -3);

  // Make sure in correct range (0x10000 - 0x10ffff)
  // if (uc == 0xf0) {
  //   if (*str < 0x90) return UTF8PROC_ERROR_INVALIDUTF8; // case #3
  // } else if (uc == 0xf4) {
  //   if (*str > 0x8f) return UTF8PROC_ERROR_INVALIDUTF8; // case #4
  // }
  *dst = ((uc & 7)<<18) | ((*str & 0x3f)<<12) | ((str[1] & 0x3f)<<6) | (str[2] & 0x3f);
  return 4;
}


/**
 * utf8proc_iterate_extended_backward — backward counterpart to utf8proc_iterate_extended,
 * with exact error-code/message parity with a full forward pass.
 *
 * Decodes the single subpart (valid code point, or maximal ill-formed subpart per
 * "U+FFFD Substitution of Maximal Subparts") that ends immediately before `str_end`.
 *
 * str_start   lower bound of the whole buffer (never read before this).
 * str_end     ONE PAST the last byte of the subpart to decode. To decode the very
 *             last code point of a Rexx string, pass str_end = string + stringLength.
 *             Chain successive calls with: next_str_end = str_end - abs(return value).
 * buffer_end  ONE PAST the last byte of the WHOLE buffer (str_start + full length).
 *             Constant across a backward iteration; used only to give
 *             utf8proc_iterate_extended the same trailing context a forward pass
 *             would have had, so TRUNCATED vs CONTINUATION-type errors are reported
 *             identically to forward decoding. Equal to str_end on the first call.
 * dst         If a valid codepoint could be read, it is stored in the variable
 *             pointed to by `dst`, otherwise that variable will be set to -1.
 * end_index   1-based index of str_end for accurate error messages
 *             (str_end = str_start + end_index - 1).
 *             May be 0 if errmsg is NULL (in this case, end_index is not used).
 * errcode     out-argument to return an error code.
 *             May be NULL if no error code is needed.
 * errmsg      out-argument to return a detailed error message.
 *             May be NULL if no error message is needed.
 *
 * Precondition: str_end must be a genuine subpart boundary as produced by a real
 * forward pass over [str_start, buffer_end) — e.g. buffer_end itself, or a value
 * returned by chaining this function. Passing an arbitrary str_end is undefined
 * with respect to message accuracy (length/boundary logic remains safe regardless,
 * by the argument below, but is only meaningful under this precondition).
 *
 * Return value: same convention as utf8proc_iterate_extended.
 */
utf8proc_ssize_t utf8proc_iterate_extended_backward(
  const utf8proc_uint8_t *str_start,
  const utf8proc_uint8_t *str_end,
  const utf8proc_uint8_t *buffer_end,
  utf8proc_int32_t *dst,
  size_t end_index = 0,
  char *errcode = NULL, size_t errcodeSize = 0,
  char *errmsg = NULL, size_t errmsgSize = 0
) {
  const utf8proc_uint8_t *anchor;
  int back;
  utf8proc_ssize_t result;
  size_t anchor_index;

  *dst = -1;
  if (errcode != NULL) *errcode = '\0';
  if (errmsg != NULL) *errmsg = '\0';

  if (str_end <= str_start) return 0;

  /*
   * Scan back from str_end-1 over continuation bytes (10xxxxxx), at most 3
   * positions: no subpart is ever longer than 4 bytes (1 lead + up to 3
   * continuations). Every byte we step over here is, by this very condition,
   * confirmed to be a continuation-pattern byte (0x80-0xBF) — this fact is
   * relied on below.
   */
  anchor = str_end - 1;
  back = 0;
  while (anchor > str_start && back < 3 && utf_cont(*anchor)) {
    anchor--;
    back++;
  }

  /* Decode from anchor with the REAL remaining buffer, not just up to str_end,
   * so any TRUNCATED/CONTINUATION-type message matches what a forward pass
   * over the whole buffer would have produced at this position. */
  anchor_index = (end_index == 0) ? end_index : end_index - (size_t)(str_end - anchor);
  result = utf8proc_iterate_extended(anchor, buffer_end - anchor, dst,
                                      anchor_index, errcode, errcodeSize,
                                      errmsg, errmsgSize);

  if (anchor + (result >= 0 ? result : -result) == str_end) {
    /* anchor's subpart lands exactly on str_end: this is the real subpart. */
    return result;
  }

  /*
   * Mismatch: anchor's subpart ends before str_end. Every byte strictly
   * between anchor and str_end is confirmed continuation-pattern (0x80-0xBF)
   * by the scan above, and such bytes can never be valid lead bytes -- so
   * str_end-1 is necessarily an isolated 1-byte subpart of its own. Decode
   * it standalone, still with full buffer_end context for message accuracy
   * (though for a continuation-pattern byte this never actually needs
   * lookahead, since it always fails the uc < 0xc2 check immediately).
   */
  anchor = str_end - 1;
  anchor_index = (end_index == 0) ? end_index : end_index - 1;
  result = utf8proc_iterate_extended(anchor, buffer_end - anchor, dst,
                                      anchor_index, errcode, errcodeSize,
                                      errmsg, errmsgSize);

  return result;
}


/******************************************************************************/
/*                                                                            */
/* UTF8Proc Extensions                                                        */
/*                                                                            */
/* utf8proc_grapheme_break_backward                                           */
/*                                                                            */
/******************************************************************************/

/*
Position convention:

A "pos" pointer denotes a *boundary* in the buffer, not a codepoint itself.
utf8proc_iterate_extended_backward(str_start, pos, ...) decodes whatever
codepoint's bytes lie immediately before that boundary.
pos == str_start means "nothing precedes here".

In utf8proc_grapheme_break_backward(str_start, buffer_end, pos1, codepoint1,
codepoint2), `pos1` is the address of codepoint1's first byte -- the
boundary at codepoint1's left edge. Passing pos1 to
utf8proc_iterate_extended_backward decodes whatever codepoint immediately
precedes codepoint1, which is exactly the starting point the lookback
helpers (GB9c, GB11, GB12/13) need to walk further left past codepoint1.
*/

/*
UAX #29, Unicode Text Segmentation
https://www.unicode.org/reports/tr29/
https://www.unicode.org/reports/tr29/#Random_Access

Iterates backward from a KNOWN boundary (default: one past the end of the
string, i.e. the whole string's right edge). This is deliberately not a
random-access primitive -- per UAX #29 6.4, starting from an arbitrary,
unverified offset would require a separate "safe point" search. Starting
from a genuine boundary avoids that; what remains is the residual cost
described below.

Rules GB3, GB6-GB9b are simple, direction-symmetric pairwise lookups: no
lookback needed, forward or backward. Three rules are not:
  GB9c  (Indic conjunct clusters)
  GB11  (emoji ZWJ sequences)
  GB12/13 (regional indicator / flag sequences)
These are left-anchored: correctly resolving them going backward requires
rescanning the run of Extend/Linker/RI scalars each time one is
encountered, since (unlike forward iteration) there's no running "parity"
or "seen a linker" state carried over from a previous step to reuse. This
is not an implementation gap on our part -- Swift's stdlib has the exact
same property and documents it as accepted, non-quadratic-in-practice
behavior (see StringGraphemeBreaking.swift, _previousGraphemeClusterBoundary).
https://github.com/swiftlang/swift/blob/main/stdlib/public/core/StringGraphemeBreaking.swift
*/

/*
RI is genuinely quadratic — here's why

precedingRIRunLengthIsOdd triggers on every internal pair of a homogeneous RI run,
because its trigger condition is "both sides are the same class" (prop1==RI && prop2==RI).
In a run of N consecutive RI codepoints, essentially every adjacent pair re-fires the check,
and each firing rescans the whole preceding RI stretch from scratch.
That's O(N) calls × O(N) average scan = O(N²).

Repeat unit: a single Regional Indicator Symbol Letter,
e.g. 🇦 = U+1F1E6 (UTF-8: F0 9F 87 A6). Just repeat it:
🇦🇦🇦🇦🇦...🇦        (N copies of U+1F1E6)
This isn't a sequence of valid flag pairs semantically — it doesn't matter,
GB12/13 only cares about the boundclass RI, not validity as a country code.
*/

/*
GB9c and GB11 are not actually quadratic

isConsonantThroughLinkerRun and isPictographicBeforeZwj trigger on an asymmetric transition
— "arriving at a Consonant" / "arriving at a Pictograph" — not on two sides matching.
Critically, hitting any Consonant (or Pictograph) during the backward scan halts it unconditionally,
regardless of whether the rule's specific condition (a Linker in between, etc.) is satisfied.
That means: for consonants c₁ < c₂ < ... < cₖ in the string, the scan triggered by cᵢ
can never run past cᵢ₋₁ — it stops there. Since each pair is tested exactly once
during a full traversal, these per-consonant scan intervals are disjoint and telescope
to O(n) total, no matter how the consonants are spaced.
Same argument for pictographs and GB11.

So a single call can still be O(run length) — worth demonstrating for perf-testing
one grapheme decode's worst case — but it can't accumulate to O(n²) over a full
string traversal the way RI does.

GB9c single-call example: a long run of ZWJ (boundclass ZWJ, InCB=None — satisfies
isInCBExtendOrLinker) with no Consonant anywhere in it, terminated by one Consonant:
U+200D repeated N times, then U+0915 (क, DEVANAGARI LETTER KA)
UTF-8: E2 80 8D × N, then E0 A4 95.
The single pair (last ZWJ, KA) triggers one O(N) scan that runs all the way to
string start without finding a Consonant, then returns "break allowed."

GB11 single-call example: a long run of a plain combining mark (Extend, not pictographic)
with no pictograph in it, then a ZWJ, then a pictograph:
U+0301 (combining acute accent) repeated N times, then U+200D, then U+1F600 (😀)
UTF-8: CC 81 × N, then E2 80 8D, then F0 9F 98 80.
If you want to see the "each call bounded, still linear overall" behavior rather
than one big call, chain several ZWJ^k + Consonant (or Extend^k + ZWJ + Pictograph)
blocks back to back — each block's scan is bounded by k and stops at the previous
block's terminator, so M blocks of length k cost O(M·k) = O(n), not O(n²).
*/


/**
 * isInCBExtendOrLinker
 *
 * true if codepoint is InCB=Extend or InCB=Linker (i.e. Extend/ZWJ but
 * not itself a Consonant or Linker) -- same derivation Swift uses for
 * _isInCBExtend.
 */
static utf8proc_bool isInCBExtendOrLinker(utf8proc_int32_t codepoint)
{
    const utf8proc_property_t *p = utf8proc_get_property(codepoint);

    if (p->indic_conjunct_break == UTF8PROC_INDIC_CONJUNCT_BREAK_LINKER)
        return true;

    if (p->boundclass != UTF8PROC_BOUNDCLASS_EXTEND &&
        p->boundclass != UTF8PROC_BOUNDCLASS_ZWJ)
        return false;

    return p->indic_conjunct_break != UTF8PROC_INDIC_CONJUNCT_BREAK_CONSONANT;
}


/**
 * isPictographicBeforeZwj
 *
 * GB11 lookback: walk left from the ZWJ (whose left boundary starts at
 * zwjStartPos) over an Extend* run to see whether an
 * Extended_Pictographic codepoint started it.
 */
static utf8proc_bool isPictographicBeforeZwj(
    const utf8proc_uint8_t *str_start,
    const utf8proc_uint8_t *zwjStartPos,
    const utf8proc_uint8_t *buffer_end)
{
    const utf8proc_uint8_t *pos = zwjStartPos;

    for (;;) {
        if (pos <= str_start) return false;

        utf8proc_int32_t cp;
        utf8proc_ssize_t sizeB = utf8proc_iterate_extended_backward(str_start, pos, buffer_end, &cp);
        if (sizeB < 0) return false; /* an error boundary can't be pictographic */

        const utf8proc_property_t *p = utf8proc_get_property(cp);

        if (p->boundclass == UTF8PROC_BOUNDCLASS_EXTENDED_PICTOGRAPHIC ||
            p->boundclass == UTF8PROC_BOUNDCLASS_E_ZWG)
            return true;

        if (p->boundclass != UTF8PROC_BOUNDCLASS_EXTEND)
            return false; /* run ended without finding one */

        pos -= sizeB;
    }
}


/**
 * isConsonantThroughLinkerRun
 *
 * GB9c lookback: walk left from the Extend/Linker run looking for an
 * InCB=Consonant, requiring at least one InCB=Linker en route.
 * `firstCodepoint` is codepoint1 itself (already confirmed
 * Extend/Linker by the caller) and must be checked for Linker-ness
 * too -- it's part of the run, not something to skip past before
 * scanning starts.
 */
static utf8proc_bool isConsonantThroughLinkerRun(
    const utf8proc_uint8_t *str_start,
    utf8proc_int32_t firstCodepoint,
    const utf8proc_uint8_t *runEndPos,
    const utf8proc_uint8_t *buffer_end)
{
    const utf8proc_property_t *pFirst = utf8proc_get_property(firstCodepoint);
    utf8proc_bool seenLinker = (pFirst->indic_conjunct_break == UTF8PROC_INDIC_CONJUNCT_BREAK_LINKER);

    const utf8proc_uint8_t *pos = runEndPos;

    for (;;) {
        if (pos <= str_start) return false;

        utf8proc_int32_t cp;
        utf8proc_ssize_t sizeB = utf8proc_iterate_extended_backward(str_start, pos, buffer_end, &cp);
        if (sizeB < 0) return false;

        const utf8proc_property_t *p = utf8proc_get_property(cp);

        if (p->indic_conjunct_break == UTF8PROC_INDIC_CONJUNCT_BREAK_CONSONANT)
            return seenLinker;

        if (p->indic_conjunct_break == UTF8PROC_INDIC_CONJUNCT_BREAK_LINKER)
            seenLinker = true;
        else if (!isInCBExtendOrLinker(cp))
            return false; /* run ended, no consonant */

        pos -= sizeB;
    }
}


/**
 * precedingRIRunLengthIsOdd
 *
 * GB12/13 lookback: count the contiguous run of Regional_Indicator
 * scalars ending at (and including) the scalar whose left edge is
 * riStartPos. No-break holds iff that count is odd.
 *
 * This is O(run length) rather than O(1), so a string that's
 * pathologically almost-all REGIONAL_INDICATOR scalars would make
 * backward iteration quadratic over that stretch.
 */
static utf8proc_bool precedingRIRunLengthIsOdd(
    const utf8proc_uint8_t *str_start,
    const utf8proc_uint8_t *riStartPos,
    const utf8proc_uint8_t *buffer_end)
{
    const utf8proc_uint8_t *pos = riStartPos;
    int count = 0;

    for (;;) {
        if (pos <= str_start) break;

        utf8proc_int32_t cp;
        utf8proc_ssize_t sizeB = utf8proc_iterate_extended_backward(str_start, pos, buffer_end, &cp);
        if (sizeB < 0) break; /* error */

        const utf8proc_property_t *p = utf8proc_get_property(cp);
        if (p->boundclass != UTF8PROC_BOUNDCLASS_REGIONAL_INDICATOR)
            break; /* run ends here */

        count += 1;
        pos -= sizeB;
    }

    return (count % 2) == 1;
}


/**
 * utf8proc_grapheme_break_backward
 *
 * Direct-from-table pairwise rules (GB3, GB6-GB9b), plus dispatch to
 * the three lookback-requiring rules (GB9c, GB11, GB12/13). Order
 * matches rule precedence in UAX #29 3.1.1: first match wins.
 *
 *   str_start   lower bound of the whole buffer.
 *   buffer_end  one past the last byte of the whole buffer.
 *   pos1        the address of codepoint1's first byte (its left edge).
 *               Passing this to utf8proc_iterate_extended_backward decodes
 *               whatever codepoint immediately precedes codepoint1.
 *   codepoint1  the codepoint to the left of the candidate break.
 *   codepoint2  the codepoint to the right of the candidate break.
 */
utf8proc_bool utf8proc_grapheme_break_backward(
    const utf8proc_uint8_t *str_start,
    const utf8proc_uint8_t *buffer_end,
    const utf8proc_uint8_t *pos1,
    utf8proc_int32_t codepoint1,
    utf8proc_int32_t codepoint2)
{
    const utf8proc_property_t *p1 = utf8proc_get_property(codepoint1);
    const utf8proc_property_t *p2 = utf8proc_get_property(codepoint2);
    int prop1 = p1->boundclass;
    int prop2 = p2->boundclass;

    /* GB3: do not break CR x LF */
    if (prop1 == UTF8PROC_BOUNDCLASS_CR && prop2 == UTF8PROC_BOUNDCLASS_LF)
        return false;

    /* GB4: break after Control | CR | LF */
    if (prop1 == UTF8PROC_BOUNDCLASS_CONTROL ||
        prop1 == UTF8PROC_BOUNDCLASS_CR ||
        prop1 == UTF8PROC_BOUNDCLASS_LF)
        return true;

    /* GB5: break before Control | CR | LF */
    if (prop2 == UTF8PROC_BOUNDCLASS_CONTROL ||
        prop2 == UTF8PROC_BOUNDCLASS_CR ||
        prop2 == UTF8PROC_BOUNDCLASS_LF)
        return true;

    /* GB6/7/8: do not break within Hangul syllable sequences */
    if (prop1 == UTF8PROC_BOUNDCLASS_L &&
        (prop2 == UTF8PROC_BOUNDCLASS_L || prop2 == UTF8PROC_BOUNDCLASS_V ||
         prop2 == UTF8PROC_BOUNDCLASS_LV || prop2 == UTF8PROC_BOUNDCLASS_LVT))
        return false;

    if ((prop1 == UTF8PROC_BOUNDCLASS_LV || prop1 == UTF8PROC_BOUNDCLASS_V) &&
        (prop2 == UTF8PROC_BOUNDCLASS_V || prop2 == UTF8PROC_BOUNDCLASS_T))
        return false;

    if ((prop1 == UTF8PROC_BOUNDCLASS_LVT || prop1 == UTF8PROC_BOUNDCLASS_T) &&
        prop2 == UTF8PROC_BOUNDCLASS_T)
        return false;

    /* GB9: do not break before Extend | ZWJ */
    if (prop2 == UTF8PROC_BOUNDCLASS_EXTEND || prop2 == UTF8PROC_BOUNDCLASS_ZWJ)
        return false;

    /* GB9a: do not break before SpacingMark */
    if (prop2 == UTF8PROC_BOUNDCLASS_SPACINGMARK)
        return false;

    /* GB9b: do not break after Prepend */
    if (prop1 == UTF8PROC_BOUNDCLASS_PREPEND)
        return false;

    /* GB11: Extended_Pictographic Extend* ZWJ x Extended_Pictographic */
    if (prop1 == UTF8PROC_BOUNDCLASS_ZWJ &&
        (prop2 == UTF8PROC_BOUNDCLASS_EXTENDED_PICTOGRAPHIC ||
         prop2 == UTF8PROC_BOUNDCLASS_E_ZWG)) {
        return !isPictographicBeforeZwj(str_start, pos1, buffer_end);
    }

    /* GB9c: InCB=Consonant [Extend|Linker]* Linker [Extend|Linker]* x InCB=Consonant */
    if (p2->indic_conjunct_break == UTF8PROC_INDIC_CONJUNCT_BREAK_CONSONANT &&
        (prop1 == UTF8PROC_BOUNDCLASS_EXTEND || prop1 == UTF8PROC_BOUNDCLASS_ZWJ) &&
        isInCBExtendOrLinker(codepoint1)) {
        return !isConsonantThroughLinkerRun(str_start, codepoint1, pos1, buffer_end);
    }

    /* GB12/13: do not break within RI RI if an odd number of RI precede the break */
    if (prop1 == UTF8PROC_BOUNDCLASS_REGIONAL_INDICATOR &&
        prop2 == UTF8PROC_BOUNDCLASS_REGIONAL_INDICATOR) {
        return precedingRIRunLengthIsOdd(str_start, pos1, buffer_end);
    }

    /* GB999: otherwise, break */
    return true;
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
 * @param string        (in)            A UTF-8 string.
 * @param indexB        (in)            The byte index (1-based) of the encoded codepoint in rexxString
 * @param refSizeB      (out, optional) The number of bytes read to decode the codepoint (negative if error),
 *                                      or 0 if indexB is at the end of the string (indexB == length+1).
 * @param refErrorCode  (out, optional) The null string "" if a valid codepoint could be read,
 *                                      or the error code otherwise.
 * @param refErrorMsg   (out, optional) The null string "" if a valid codepoint could be read,
 *                                      or the error message otherwise.
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
    if (string->classObject() != TheStringClass)
    {
        Protected<RexxString> errmsg = new_string("Argument string class: expected String, found ");
        errmsg = errmsg->concat(string->classObject()->getId());
        reportException(Error_Invalid_argument_user_defined, errmsg);
    }
    // if (string->classObject() != TheStringClass) reportException(Error_Invalid_argument_noclass, "string", TheStringClass->getId());

    size_t index = positionArgument(indexB, "indexB"); // 1-based, range 1..length+1
    if (index > string->getLength() + 1) reportException(Error_Incorrect_method_position, index); // length+1 is accepted, but not beyond
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

    if (refSizeB != OREF_NULL && size != 0)
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
 * Given a string and a byte index treated as a boundary, return the codepoint
 * immediately preceding that boundary, and its size in bytes.
 *
 * The index has the same meaning as in utf8DecodeCodepoint: the 1-based byte
 * position of a codepoint's first byte. This method returns whatever codepoint's
 * bytes lie immediately before that position -- so passing the same indexB value
 * to utf8DecodeCodepoint and utf8DecodePreviousCodepoint gives you "this codepoint"
 * and "the one right before it" respectively. Passing indexB = length+1 returns
 * the last codepoint of the string.
 *
 * @param string        (in)            A UTF-8 string.
 * @param indexB        (in)            The byte index (1-based) of the boundary before which
 *                                      to decode. Valid range: 1..length+1. (1 means "nothing
 *                                      precedes the start of the string".)
 * @param refSizeB      (out, optional) The number of bytes read to decode the codepoint (negative if error),
 *                                      or 0 if indexB is at the begining of the string (indexB == 1).
 * @param refErrorCode  (out, optional) The null string "" if a valid codepoint could be read,
 *                                      or the error code otherwise.
 * @param refErrorMsg   (out, optional) The null string "" if a valid codepoint could be read,
 *                                      or the error message otherwise.
 *
 * @return The codepoint if a valid codepoint could be read,
 *         or -1 otherwise.
 */
RexxInteger *RexxUnicodeServicesClass::utf8DecodePreviousCodepoint(RexxString *string, RexxInteger *indexB, VariableReference *refSizeB, VariableReference *refErrorCode, VariableReference *refErrorMsg)
{
    // Check arguments

    requiredArgument(string, "string");
    if (string->classObject() != TheStringClass)
    {
        Protected<RexxString> errmsg = new_string("Argument string class: expected String, found ");
        errmsg = errmsg->concat(string->classObject()->getId());
        reportException(Error_Invalid_argument_user_defined, errmsg);
    }

    size_t index = positionArgument(indexB, "indexB"); // 1-based; boundary, not a byte offset
    if (index > string->getLength() + 1) reportException(Error_Incorrect_method_position, index); // length+1 is accepted, but not beyond
    if (refSizeB != OREF_NULL) classArgument(refSizeB, TheVariableReferenceClass, "refSizeB");
    if (refErrorCode != OREF_NULL) classArgument(refErrorCode, TheVariableReferenceClass, "refErrorCode");
    if (refErrorMsg != OREF_NULL) classArgument(refErrorMsg, TheVariableReferenceClass, "refErrorMsg");

    // Default output values
    if (refSizeB != OREF_NULL) refSizeB->setValue(RexxInteger::integerZero);
    if (refErrorCode != OREF_NULL) refErrorCode->setValue(GlobalNames::NULLSTRING);
    if (refErrorMsg != OREF_NULL) refErrorMsg->setValue(GlobalNames::NULLSTRING);

    // Cannot happen, already caught when checking the arguments
    // if (index > string->getLength() + 1) return RexxInteger::integerMinusOne;

    // Note: index == 1 (nothing precedes the start) is handled naturally below:
    // str_end == str_start makes utf8proc_iterate_extended_backward return 0.

    const utf8proc_uint8_t *str = (const utf8proc_uint8_t *) string->getStringData();
    const utf8proc_uint8_t *str_start  = str;
    const utf8proc_uint8_t *buffer_end = str + string->getLength();
    const utf8proc_uint8_t *str_end    = str + (index - 1); // 0-based boundary
    utf8proc_int32_t codepoint;
    const size_t errcodeSize = 50;
    char errcode[errcodeSize];
    const size_t errmsgSize = 200;
    char errmsg[errmsgSize];
    utf8proc_ssize_t size = utf8proc_iterate_extended_backward(str_start, str_end, buffer_end, &codepoint, index, errcode, errcodeSize, errmsg, errmsgSize);

    if (refSizeB != OREF_NULL && size != 0)
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
 * @param rexxCodepoint (in)            The codepoint to encode.
 * @param destination   (in-out)        The mutable buffer to update.
 * @param refSizeB      (out, optional) The size of the encoded byte sequence (0..4)
 *                                      The size is 0 if the codepoint is not in the
 *                                      range 0..10FFFF
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
 * Performs a full scan of a UTF-8 string.
 * Returns .true if `string` is a valid UTF-8 string.
 * Returns additional information through reference variables.
 *
 * @param string            (in)                A UTF-8 string.
 * @param refCodepointCount (out, optional)     The count of codepoints.
 * @param refGraphemeCount  (out, optional)     The count of graphemes.
 * @param refErrorCount     (out, optional)     The count of errors.
 *
 * @return .true if `string` is a valid UTF-8 string, .false otherwise.
 */
RexxInteger *RexxUnicodeServicesClass::utf8StringInfo(RexxString *string, VariableReference *refGraphemeCount, VariableReference *refCodepointCount, VariableReference *refErrorCount, RexxInteger *rexxStopAtFirstError)
{
    // Check arguments

    requiredArgument(string, "string");
    if (string->classObject() != TheStringClass)
    {
        Protected<RexxString> errmsg = new_string("Argument string class: expected String, found ");
        errmsg = errmsg->concat(string->classObject()->getId());
        reportException(Error_Invalid_argument_user_defined, errmsg);
    }

    if (refGraphemeCount != OREF_NULL) classArgument(refGraphemeCount, TheVariableReferenceClass, "refGraphemeCount");
    if (refCodepointCount != OREF_NULL) classArgument(refCodepointCount, TheVariableReferenceClass, "refCodepointCount");
    if (refErrorCount != OREF_NULL) classArgument(refErrorCount, TheVariableReferenceClass, "refErrorCount");

    bool stopAtFirstError = false;
    if (rexxStopAtFirstError != OREF_NULL) stopAtFirstError = (bool)integerRange(rexxStopAtFirstError, 0, 1, Error_Logical_value_user_defined, "Value of argument stopAtFirstError must be 0 or 1");

    const utf8proc_uint8_t *start = (const utf8proc_uint8_t *) string->getStringData();
    utf8proc_ssize_t remainingLength = string->getLength();

    size_t graphemeCount = 0;
    size_t codepointCount = 0;
    size_t errorCount = 0;

    const utf8proc_uint8_t *str = start;
    const utf8proc_uint8_t *firstInvalidByteSequence = NULL;

    if (refGraphemeCount == OREF_NULL)
    {
        // Fast path, no need to count graphemes

        for (;;)
        {
            utf8proc_int32_t codepoint;
            utf8proc_ssize_t sizeB = utf8proc_iterate_extended(str, remainingLength, &codepoint);
            if (sizeB == 0) break;
            if (sizeB < 0)
            {
                // Here, codepoint == -1
                errorCount += 1;

                if (firstInvalidByteSequence == NULL) firstInvalidByteSequence = str;
                if (stopAtFirstError) break;

                codepointCount += 1;
                sizeB = -sizeB;
            }
            else
            {
                codepointCount += 1;
            }
            str += sizeB;
            remainingLength -= sizeB;
        }
    }
    else
    {
        // Slower path, must count graphemes
        utf8proc_int32_t previousCodepoint = -1;
        const utf8proc_property_t *previousCodepointProperty = NULL;

        utf8proc_int32_t codepoint = -1;
        const utf8proc_property_t *codepointProperty = NULL;

        utf8proc_int32_t graphemeBreakState = 0;

        for (;;)
        {
            utf8proc_ssize_t sizeB = utf8proc_iterate_extended(str, remainingLength, &codepoint);
            if (sizeB == 0) break;
            if (sizeB < 0)
            {
                // Here, codepoint == -1, so previousCodepoint will become -1
                codepointProperty = NULL;
                errorCount += 1;

                if (firstInvalidByteSequence == NULL) firstInvalidByteSequence = str;
                if (stopAtFirstError) break;

                codepointCount += 1;
                graphemeCount += 1;
                graphemeBreakState = 0;
                sizeB = -sizeB;
            }
            else
            {
                // optim 2: if codepoint == previousCodepoint then no need to retrieve the property record of codepoint.
                if (codepoint != previousCodepoint) codepointProperty = utf8proc_get_property(codepoint); // must get it now, will be assigned to previousCodepointProperty

                codepointCount += 1;
                if (previousCodepoint < 0)
                {
                    // First codepoint or Error recovery
                    graphemeCount += 1;
                }
                else
                {
                    // optim 1: reuse the property record of previousCodepoint instead of retrieving 2 property records at each iteration.

                    // Here, previousCodepoint is >= 0, so previousCodepointProperty has already the right value
                    utf8proc_bool graphemeBreak = grapheme_break_extended(  previousCodepointProperty->boundclass,
                                                                            codepointProperty->boundclass,
                                                                            previousCodepointProperty->indic_conjunct_break,
                                                                            codepointProperty->indic_conjunct_break,
                                                                            &graphemeBreakState);

                    //utf8proc_bool graphemeBreak = utf8proc_grapheme_break_stateful(previousCodepoint, codepoint, &graphemeBreakState);
                    if (graphemeBreak) graphemeCount += 1;
                }
            }
            previousCodepoint = codepoint;
            previousCodepointProperty = codepointProperty;
            str += sizeB;
            remainingLength -= sizeB;
        }
    }

    if (refGraphemeCount != OREF_NULL)
    {
        RexxInteger *rexxGraphemeCount = new_integer(graphemeCount); // Protected<RexxString> not needed
        refGraphemeCount->setValue(rexxGraphemeCount);
    }

    if (refCodepointCount != OREF_NULL)
    {
        RexxInteger *rexxCodepointCount = new_integer(codepointCount); // Protected<RexxInteger> not needed
        refCodepointCount->setValue(rexxCodepointCount);
    }

    if (refErrorCount != OREF_NULL)
    {
        RexxInteger *rexxErrorCount = new_integer(errorCount); // Protected<RexxString> not needed
        refErrorCount->setValue(rexxErrorCount);
    }

    return (errorCount == 0) ? RexxInteger::integerZero : new_integer(firstInvalidByteSequence - start + 1);
}


/**
 * Calculates the width in console columns of a UTF-8 string.
 * If `string` is an invalid UTF-8 string, the width is limited to the portion before the first invalid byte sequence.
 * Returns width information through reference variables.
 *
 * @param string                            (in)                A UTF-8 string.
 * @param refGraphemeWidthSum               (out, optional)     The sum of grapheme widths, where the width of a grapheme is the maximum codepoint width within the grapheme cluster.
 * @param refCodepointWidthSum              (out, optional)     The sum of codepoint widths.
 * @param refGraphemeEastAsianWidthSum      (out, optional)     The sum of grapheme East Asian widths, where the width of a grapheme is the maximum codepoint East Asian width within the grapheme cluster.
 * @param refCodepointEastAsianWidthSum     (out, optional)     The sum of codepoint East Asian widths, where ambiguous-width codepoints have a width of 2.
 *
 * @return .true if `string` is a valid UTF-8 string, .false otherwise.
 */
RexxInteger *RexxUnicodeServicesClass::utf8StringWidth(RexxString *string, VariableReference *refGraphemeWidthSum, VariableReference *refCodepointWidthSum, VariableReference *refGraphemeEastAsianWidthSum, VariableReference *refCodepointEastAsianWidthSum)
{
    // Check arguments

    requiredArgument(string, "string");
    if (string->classObject() != TheStringClass)
    {
        Protected<RexxString> errmsg = new_string("Argument string class: expected String, found ");
        errmsg = errmsg->concat(string->classObject()->getId());
        reportException(Error_Invalid_argument_user_defined, errmsg);
    }

    if (refGraphemeWidthSum != OREF_NULL) classArgument(refGraphemeWidthSum, TheVariableReferenceClass, "refGraphemeWidthSum");
    if (refCodepointWidthSum != OREF_NULL) classArgument(refCodepointWidthSum, TheVariableReferenceClass, "refCodepointWidthSum");
    if (refGraphemeEastAsianWidthSum != OREF_NULL) classArgument(refGraphemeEastAsianWidthSum, TheVariableReferenceClass, "refGraphemeEastAsianWidthSum");
    if (refCodepointEastAsianWidthSum != OREF_NULL) classArgument(refCodepointEastAsianWidthSum, TheVariableReferenceClass, "refCodepointEastAsianWidthSum");

    const utf8proc_uint8_t *str = (const utf8proc_uint8_t *) string->getStringData();
    utf8proc_size_t length = string->getLength();

    size_t graphemeWidthSum = 0;
    size_t codepointWidthSum = 0;
    size_t graphemeEastAsianWidthSum = 0;
    size_t codepointEastAsianWidthSum = 0;

    bool validUtf8 = true;

    if (refGraphemeWidthSum != OREF_NULL)
    {
        RexxInteger *rexxGraphemeWidthSum = new_integer(graphemeWidthSum); // Protected<RexxString> not needed
        refGraphemeWidthSum->setValue(rexxGraphemeWidthSum);
    }

    if (refCodepointWidthSum != OREF_NULL)
    {
        RexxInteger *rexxCodepointWidthSum = new_integer(codepointWidthSum); // Protected<RexxString> not needed
        refCodepointWidthSum->setValue(rexxCodepointWidthSum);
    }

    if (refGraphemeEastAsianWidthSum != OREF_NULL)
    {
        RexxInteger *rexxGraphemeEastAsianWidthSum = new_integer(graphemeEastAsianWidthSum); // Protected<RexxString> not needed
        refGraphemeEastAsianWidthSum->setValue(rexxGraphemeEastAsianWidthSum);
    }

    if (refCodepointEastAsianWidthSum != OREF_NULL)
    {
        RexxInteger *rexxCodepointEastAsianWidthSum = new_integer(codepointEastAsianWidthSum); // Protected<RexxString> not needed
        refCodepointEastAsianWidthSum->setValue(rexxCodepointEastAsianWidthSum);
    }

   return (validUtf8) ? TheTrueObject : TheFalseObject;
}


/**
 * @deprecated Using an array for the persistent state can lead to subtle bugs
 *             when copying an object that uses such an array internally.
 *             By default, the copy is shallow, so both the original object and
 *             its copies refer to the same internal array. As a result, the
 *             state may be corrupted, depending on the sequence of calls made
 *             on the original object and its copies.
 *
 * Given a pair of consecutive codepoints, return whether a grapheme break is
 * permitted between them.
 *
 * @param array An array of 3 items:
 *     codepoint1 (in)     The first codepoint.
 *     codepoint2 (in)     The second codepoint.
 *     state      (in-out) Initial value must be 0.
 *
 * @return .true if a grapheme break is permitted, .false otherwise.
 */
RexxInteger *RexxUnicodeServicesClass::graphemeBreak(ArrayClass *array)
{
    // No need to use arrayArgument, we really want an array already GC protected by the caller
    classArgument(array, TheArrayClass, "1"); // ARG_ONE not supported
    // array = arrayArgument(array, ARG_ONE);
    // ProtectedObject p(array);
    utf8proc_int32_t codepoint1 = (utf8proc_int32_t)integerRange((RexxObject *)array->get(1), 0, SSIZE_MAX, Error_Invalid_argument_user_defined, "GraphemeBreak: The first code point must be a non negative integer");
    utf8proc_int32_t codepoint2 = (utf8proc_int32_t)integerRange((RexxObject *)array->get(2), 0, SSIZE_MAX, Error_Invalid_argument_user_defined, "GraphemeBreak: The second code point must be a non negative integer");
    utf8proc_int32_t state =      (utf8proc_int32_t)integerRange((RexxObject *)array->get(3), 0, SSIZE_MAX, Error_Invalid_argument_user_defined, "GraphemeBreak:The state must be a non negative integer");
    utf8proc_bool graphemeBreak = utf8proc_grapheme_break_stateful(codepoint1, codepoint2, &state);
    array->put(new_integer(state), 3); // Output argument
    return graphemeBreak ? TheTrueObject : TheFalseObject;
}


/**
 * Given a pair of consecutive codepoints, return whether a grapheme break is
 * permitted between them.
 *
 * @param codepoint1 (in)     The first codepoint.
 * @param codepoint2 (in)     The second codepoint.
 * @param refState   (in-out) Initial value must be 0.
 *
 * @return .true if a grapheme break is permitted, .false otherwise.
 */
RexxInteger *RexxUnicodeServicesClass::graphemeBreak3(RexxInteger *rexxCodepoint1, RexxInteger *rexxCodepoint2, VariableReference *refState)
{
    requiredArgument(rexxCodepoint1, "codepoint1");
    utf8proc_int32_t codepoint1 = (utf8proc_int32_t)integer(rexxCodepoint1, "codepoint1 must be an integer");

    requiredArgument(rexxCodepoint2, "codepoint2");
    utf8proc_int32_t codepoint2 = (utf8proc_int32_t)integer(rexxCodepoint2, "codepoint2 must be an integer");

    classArgument(refState, TheVariableReferenceClass, "refState");
    utf8proc_int32_t state = (utf8proc_int32_t)integerRange(refState->getValue(), 0, SSIZE_MAX, Error_Invalid_argument_user_defined, "GraphemeBreak:The state must be a non negative integer");

    utf8proc_bool graphemeBreak = utf8proc_grapheme_break_stateful(codepoint1, codepoint2, &state);

    RexxInteger *rexxState = new_integer(state); // Protected<RexxInteger> not needed
    refState->setValue(rexxState);
    return graphemeBreak ? TheTrueObject : TheFalseObject;
}


/**
 * graphemeBreakBackward — backward counterpart to graphemeBreak3.
 *
 * Given a pair of consecutive codepoints, return whether a grapheme break
 * is permitted between them when processing the pair backward.
 *
 * @param string     (in)       A UTF-8 string.
 * @param indexB     (in)       The byte index (1-based) of codepoint1.
 * @param codepoint1 (in)       The first codepoint.
 * @param codepoint2 (in)       The second codepoint, immediatly following codepoint1 in forward order.
 *
 * @return .true if a grapheme break is permitted, .false otherwise.
 */
RexxInteger *RexxUnicodeServicesClass::graphemeBreakBackward(RexxString *string, RexxInteger *indexB, RexxInteger *rexxCodepoint1, RexxInteger *rexxCodepoint2)
{
    // Check arguments

    requiredArgument(string, "string");
    if (string->classObject() != TheStringClass)
    {
        Protected<RexxString> errmsg = new_string("Argument string class: expected String, found ");
        errmsg = errmsg->concat(string->classObject()->getId());
        reportException(Error_Invalid_argument_user_defined, errmsg);
    }

    size_t index = positionArgument(indexB, "indexB"); // 1-based; boundary, not a byte offset
    // Since we have already decoded 2 codepoints, index cannot be length+1 or length or length-1
    // I think the test should be (index > string->getLength() - 2) but an additional test should be made: (string->getLength() > 2)
    // For the moment, I keep the test (index > string->getLength() + 1)
    if (index > string->getLength() + 1) reportException(Error_Incorrect_method_position, index); // length+1 is accepted, but not beyond

    requiredArgument(rexxCodepoint1, "codepoint1");
    utf8proc_int32_t codepoint1 = (utf8proc_int32_t)integer(rexxCodepoint1, "codepoint1 must be an integer");

    requiredArgument(rexxCodepoint2, "codepoint2");
    utf8proc_int32_t codepoint2 = (utf8proc_int32_t)integer(rexxCodepoint2, "codepoint2 must be an integer");

    const utf8proc_uint8_t *str = (const utf8proc_uint8_t *) string->getStringData();
    const utf8proc_uint8_t *str_start  = str;
    const utf8proc_uint8_t *buffer_end = str + string->getLength();
    const utf8proc_uint8_t *str_end    = str + (index - 1); // 0-based boundary

    utf8proc_bool graphemeBreak = utf8proc_grapheme_break_backward(str_start, buffer_end, str_end, codepoint1, codepoint2);

    return graphemeBreak ? TheTrueObject : TheFalseObject;
}


/*
utf8proc property field not exposed, internal use:
    utf8proc_uint16_t decomp_seqindex;
    utf8proc_uint16_t casefold_seqindex;
    utf8proc_uint16_t uppercase_seqindex;
    utf8proc_uint16_t lowercase_seqindex;
    utf8proc_uint16_t titlecase_seqindex;
    utf8proc_uint16_t comb_index;
    unsigned comp_exclusion:1;
*/


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


// https://www.unicode.org/reports/tr44/#Canonical_Combining_Class_Values
//     Ccc or CCC?
//     For use in regular expression matching, fixed position classes (ccc=10 through ccc=199)
//     which actually occur in the Unicode Character Database for any version are given predictable aliases
//     of the form "Ccc10", "Ccc11", and so forth.
//     The complete list of such aliases which are actually defined can be found in PropertyValueAliases.txt.
//
// PropertyValueAliases.txt
//   CCC
//
// Decision: CCC in the table below.
const char *Canonical_Combining_Class[] =
{
    // first column (numeric value) from PropertyValueAliases.txt
    // second column (short name) from PropertyValueAliases.txt
    // third column (long name) from PropertyValueAliases.txt
    // fourth column (description) from https://www.unicode.org/reports/tr44/#CCC_Values_Table
    /* 0 */   "NR",     "Not_Reordered",         // Spacing and enclosing marks; also many vowel and consonant signs, even if nonspacing
    /* 1 */   "OV",     "Overlay",               // Marks which overlay a base letter or symbol
    /* 2 */   "",       "",
    /* 3 */   "",       "",
    /* 4 */   "",       "",
    /* 5 */   "",       "",
    /* 6 */   "HANR",   "Han_Reading",           // Diacritic reading marks for CJK unified ideographs
    /* 7 */   "NK",     "Nukta",                 // Diacritic nukta marks in Brahmi-derived scripts
    /* 8 */   "KV",     "Kana_Voicing",          // Hiragana/Katakana voicing marks
    /* 9 */   "VR",     "Virama",                // Viramas
    /* 10 */  "CCC10",  "CCC10",                 // Start of fixed position classes
    /* 11 */  "CCC11",  "CCC11",
    /* 12 */  "CCC12",  "CCC12",
    /* 13 */  "CCC13",  "CCC13",
    /* 14 */  "CCC14",  "CCC14",
    /* 15 */  "CCC15",  "CCC15",
    /* 16 */  "CCC16",  "CCC16",
    /* 17 */  "CCC17",  "CCC17",
    /* 18 */  "CCC18",  "CCC18",
    /* 19 */  "CCC19",  "CCC19",
    /* 20 */  "CCC20",  "CCC20",
    /* 21 */  "CCC21",  "CCC21",
    /* 22 */  "CCC22",  "CCC22",
    /* 23 */  "CCC23",  "CCC23",
    /* 24 */  "CCC24",  "CCC24",
    /* 25 */  "CCC25",  "CCC25",
    /* 26 */  "CCC26",  "CCC26",
    /* 27 */  "CCC27",  "CCC27",
    /* 28 */  "CCC28",  "CCC28",
    /* 29 */  "CCC29",  "CCC29",
    /* 30 */  "CCC30",  "CCC30",
    /* 31 */  "CCC31",  "CCC31",
    /* 32 */  "CCC32",  "CCC32",
    /* 33 */  "CCC33",  "CCC33",
    /* 34 */  "CCC34",  "CCC34",
    /* 35 */  "CCC35",  "CCC35",
    /* 36 */  "CCC36",  "CCC36",
    /* 37 */  "CCC37",  "CCC37",
    /* 38 */  "CCC38",  "CCC38",
    /* 39 */  "CCC39",  "CCC39",
    /* 40 */  "CCC40",  "CCC40",
    /* 41 */  "CCC41",  "CCC41",
    /* 42 */  "CCC42",  "CCC42",
    /* 43 */  "CCC43",  "CCC43",
    /* 44 */  "CCC44",  "CCC44",
    /* 45 */  "CCC45",  "CCC45",
    /* 46 */  "CCC46",  "CCC46",
    /* 47 */  "CCC47",  "CCC47",
    /* 48 */  "CCC48",  "CCC48",
    /* 49 */  "CCC49",  "CCC49",
    /* 50 */  "CCC50",  "CCC50",
    /* 51 */  "CCC51",  "CCC51",
    /* 52 */  "CCC52",  "CCC52",
    /* 53 */  "CCC53",  "CCC53",
    /* 54 */  "CCC54",  "CCC54",
    /* 55 */  "CCC55",  "CCC55",
    /* 56 */  "CCC56",  "CCC56",
    /* 57 */  "CCC57",  "CCC57",
    /* 58 */  "CCC58",  "CCC58",
    /* 59 */  "CCC59",  "CCC59",
    /* 60 */  "CCC60",  "CCC60",
    /* 61 */  "CCC61",  "CCC61",
    /* 62 */  "CCC62",  "CCC62",
    /* 63 */  "CCC63",  "CCC63",
    /* 64 */  "CCC64",  "CCC64",
    /* 65 */  "CCC65",  "CCC65",
    /* 66 */  "CCC66",  "CCC66",
    /* 67 */  "CCC67",  "CCC67",
    /* 68 */  "CCC68",  "CCC68",
    /* 69 */  "CCC69",  "CCC69",
    /* 70 */  "CCC70",  "CCC70",
    /* 71 */  "CCC71",  "CCC71",
    /* 72 */  "CCC72",  "CCC72",
    /* 73 */  "CCC73",  "CCC73",
    /* 74 */  "CCC74",  "CCC74",
    /* 75 */  "CCC75",  "CCC75",
    /* 76 */  "CCC76",  "CCC76",
    /* 77 */  "CCC77",  "CCC77",
    /* 78 */  "CCC78",  "CCC78",
    /* 79 */  "CCC79",  "CCC79",
    /* 80 */  "CCC80",  "CCC80",
    /* 81 */  "CCC81",  "CCC81",
    /* 82 */  "CCC82",  "CCC82",
    /* 83 */  "CCC83",  "CCC83",
    /* 84 */  "CCC84",  "CCC84",
    /* 85 */  "CCC85",  "CCC85",
    /* 86 */  "CCC86",  "CCC86",
    /* 87 */  "CCC87",  "CCC87",
    /* 88 */  "CCC88",  "CCC88",
    /* 89 */  "CCC89",  "CCC89",
    /* 90 */  "CCC90",  "CCC90",
    /* 91 */  "CCC91",  "CCC91",
    /* 92 */  "CCC92",  "CCC92",
    /* 93 */  "CCC93",  "CCC93",
    /* 94 */  "CCC94",  "CCC94",
    /* 95 */  "CCC95",  "CCC95",
    /* 96 */  "CCC96",  "CCC96",
    /* 97 */  "CCC97",  "CCC97",
    /* 98 */  "CCC98",  "CCC98",
    /* 99 */  "CCC99",  "CCC99",
    /* 100 */ "CCC100", "CCC100",
    /* 101 */ "CCC101", "CCC101",
    /* 102 */ "CCC102", "CCC102",
    /* 103 */ "CCC103", "CCC103",
    /* 104 */ "CCC104", "CCC104",
    /* 105 */ "CCC105", "CCC105",
    /* 106 */ "CCC106", "CCC106",
    /* 107 */ "CCC107", "CCC107",
    /* 108 */ "CCC108", "CCC108",
    /* 109 */ "CCC109", "CCC109",
    /* 110 */ "CCC110", "CCC110",
    /* 111 */ "CCC111", "CCC111",
    /* 112 */ "CCC112", "CCC112",
    /* 113 */ "CCC113", "CCC113",
    /* 114 */ "CCC114", "CCC114",
    /* 115 */ "CCC115", "CCC115",
    /* 116 */ "CCC116", "CCC116",
    /* 117 */ "CCC117", "CCC117",
    /* 118 */ "CCC118", "CCC118",
    /* 119 */ "CCC119", "CCC119",
    /* 120 */ "CCC120", "CCC120",
    /* 121 */ "CCC121", "CCC121",
    /* 122 */ "CCC122", "CCC122",
    /* 123 */ "CCC123", "CCC123",
    /* 124 */ "CCC124", "CCC124",
    /* 125 */ "CCC125", "CCC125",
    /* 126 */ "CCC126", "CCC126",
    /* 127 */ "CCC127", "CCC127",
    /* 128 */ "CCC128", "CCC128",
    /* 129 */ "CCC129", "CCC129",
    /* 130 */ "CCC130", "CCC130",
    /* 131 */ "CCC131", "CCC131",
    /* 132 */ "CCC132", "CCC132",
    /* 133 */ "CCC133", "CCC133",
    /* 134 */ "CCC134", "CCC134",
    /* 135 */ "CCC135", "CCC135",
    /* 136 */ "CCC136", "CCC136",
    /* 137 */ "CCC137", "CCC137",
    /* 138 */ "CCC138", "CCC138",
    /* 139 */ "CCC139", "CCC139",
    /* 140 */ "CCC140", "CCC140",
    /* 141 */ "CCC141", "CCC141",
    /* 142 */ "CCC142", "CCC142",
    /* 143 */ "CCC143", "CCC143",
    /* 144 */ "CCC144", "CCC144",
    /* 145 */ "CCC145", "CCC145",
    /* 146 */ "CCC146", "CCC146",
    /* 147 */ "CCC147", "CCC147",
    /* 148 */ "CCC148", "CCC148",
    /* 149 */ "CCC149", "CCC149",
    /* 150 */ "CCC150", "CCC150",
    /* 151 */ "CCC151", "CCC151",
    /* 152 */ "CCC152", "CCC152",
    /* 153 */ "CCC153", "CCC153",
    /* 154 */ "CCC154", "CCC154",
    /* 155 */ "CCC155", "CCC155",
    /* 156 */ "CCC156", "CCC156",
    /* 157 */ "CCC157", "CCC157",
    /* 158 */ "CCC158", "CCC158",
    /* 159 */ "CCC159", "CCC159",
    /* 160 */ "CCC160", "CCC160",
    /* 161 */ "CCC161", "CCC161",
    /* 162 */ "CCC162", "CCC162",
    /* 163 */ "CCC163", "CCC163",
    /* 164 */ "CCC164", "CCC164",
    /* 165 */ "CCC165", "CCC165",
    /* 166 */ "CCC166", "CCC166",
    /* 167 */ "CCC167", "CCC167",
    /* 168 */ "CCC168", "CCC168",
    /* 169 */ "CCC169", "CCC169",
    /* 170 */ "CCC170", "CCC170",
    /* 171 */ "CCC171", "CCC171",
    /* 172 */ "CCC172", "CCC172",
    /* 173 */ "CCC173", "CCC173",
    /* 174 */ "CCC174", "CCC174",
    /* 175 */ "CCC175", "CCC175",
    /* 176 */ "CCC176", "CCC176",
    /* 177 */ "CCC177", "CCC177",
    /* 178 */ "CCC178", "CCC178",
    /* 179 */ "CCC179", "CCC179",
    /* 180 */ "CCC180", "CCC180",
    /* 181 */ "CCC181", "CCC181",
    /* 182 */ "CCC182", "CCC182",
    /* 183 */ "CCC183", "CCC183",
    /* 184 */ "CCC184", "CCC184",
    /* 185 */ "CCC185", "CCC185",
    /* 186 */ "CCC186", "CCC186",
    /* 187 */ "CCC187", "CCC187",
    /* 188 */ "CCC188", "CCC188",
    /* 189 */ "CCC189", "CCC189",
    /* 190 */ "CCC190", "CCC190",
    /* 191 */ "CCC191", "CCC191",
    /* 192 */ "CCC192", "CCC192",
    /* 193 */ "CCC193", "CCC193",
    /* 194 */ "CCC194", "CCC194",
    /* 195 */ "CCC195", "CCC195",
    /* 196 */ "CCC196", "CCC196",
    /* 197 */ "CCC197", "CCC197",
    /* 198 */ "CCC198", "CCC198",
    /* 199 */ "CCC199", "CCC199",                // End of fixed position classes
    /* 200 */ "ATBL",   "Attached_Below_Left",   // Marks attached at the bottom left
    /* 201 */ "",       "",
    /* 202 */ "ATB",    "Attached_Below",        // Marks attached directly below
    /* 203 */ "",       "",
    /* 204 */ "",       "",                      // Marks attached at the bottom right
    /* 205 */ "",       "",
    /* 206 */ "",       "",
    /* 207 */ "",       "",
    /* 208 */ "",       "",                      // Marks attached to the left
    /* 209 */ "",       "",
    /* 210 */ "",       "",                      // Marks attached to the right
    /* 211 */ "",       "",
    /* 212 */ "",       "",                      // Marks attached at the top left
    /* 213 */ "",       "",
    /* 214 */ "ATA",    "Attached_Above",        // Marks attached directly above
    /* 215 */ "",       "",
    /* 216 */ "ATAR",   "Attached_Above_Right",  // Marks attached at the top right
    /* 217 */ "",       "",
    /* 218 */ "BL",     "Below_Left",            // Distinct marks at the bottom left
    /* 219 */ "",       "",
    /* 220 */ "B",      "Below",                 // Distinct marks directly below
    /* 221 */ "",       "",
    /* 222 */ "BR",     "Below_Right",           // Distinct marks at the bottom right
    /* 223 */ "",       "",
    /* 224 */ "L",      "Left",                  // Distinct marks to the left
    /* 225 */ "",       "",
    /* 226 */ "R",      "Right",                 // Distinct marks to the right
    /* 227 */ "",       "",
    /* 228 */ "AL",     "Above_Left",            // Distinct marks at the top left
    /* 229 */ "",       "",
    /* 230 */ "A",      "Above",                 // Distinct marks directly above
    /* 231 */ "",       "",
    /* 232 */ "AR",     "Above_Right",           // Distinct marks at the top right
    /* 233 */ "DB",     "Double_Below",          // Distinct marks subtending two bases
    /* 234 */ "DA",     "Double_Above",          // Distinct marks extending above two bases
    /* 235 */ "",       "",
    /* 236 */ "",       "",
    /* 237 */ "",       "",
    /* 238 */ "",       "",
    /* 239 */ "",       "",
    /* 240 */ "IS",     "Iota_Subscript",        // Greek iota subscript only
    /* 241 */ "",       "",
    /* 242 */ "",       "",
    /* 243 */ "",       "",
    /* 244 */ "",       "",
    /* 245 */ "",       "",
    /* 246 */ "",       "",
    /* 247 */ "",       "",
    /* 248 */ "",       "",
    /* 249 */ "",       "",
    /* 250 */ "",       "",
    /* 251 */ "",       "",
    /* 252 */ "",       "",
    /* 253 */ "",       "",
    /* 254 */ "",       ""
};

RexxInteger *RexxUnicodeServicesClass::codepointCombiningClass(RexxInteger *rexxCodepoint, VariableReference *refCode, VariableReference *refLabel)
{
    requiredArgument(rexxCodepoint, "codepoint");
    utf8proc_int32_t codepoint = (utf8proc_int32_t)integer(rexxCodepoint, "codepoint must be an integer");
    if (refCode != OREF_NULL) classArgument(refCode, TheVariableReferenceClass, "refCode");
    if (refLabel != OREF_NULL) classArgument(refLabel, TheVariableReferenceClass, "refLabel");

    const utf8proc_property_t *property = utf8proc_get_property(codepoint);
    utf8proc_propval_t combining_class = property->combining_class; // integer 0..254
    utf8proc_propval_t index = combining_class;

    // Check array out-of-bounds
    int count = sizeof(Canonical_Combining_Class) / sizeof(Canonical_Combining_Class[0]);
    bool rangeOk = index >= 0 && index < count/2;

    if (refCode != OREF_NULL)
    {
        const char *strCode = rangeOk ? Canonical_Combining_Class[2 * index] : "?";
        RexxString *rexxCode = new_string(strCode); // Protected<RexxString> not needed
        refCode->setValue(rexxCode);
    }

    if (refLabel != OREF_NULL)
    {
        const char *strLabel = rangeOk ? Canonical_Combining_Class[1 + 2 * index] : "?";
        RexxString *rexxLabel = new_string(strLabel); // Protected<RexxString> not needed
        refLabel->setValue(rexxLabel);
    }

    return new_integer(combining_class); // see utf8proc_category_t
}


// https://www.unicode.org/reports/tr9/#Table_Bidirectional_Character_Types
// DerivedBidiClass.txt
//   #  All code points not explicitly listed for Bidi_Class
//   #  have the value Left_To_Right (L).
//   Not clear! utf8proc returns 0
//     # 0590..05FF Hebrew
//     # @missing: 0590..05FF; Right_To_Left
//     ...
//     # @missing: 1EF00..1EFFF; Right_To_Left
// PropertyValueAliases.txt
//   Define the short names and long names
const char *Bidirectional_Character_Types[]=
{
    // first column (short name) from PropertyValueAliases.txt and DerivedBidiClass.txt
    // second column (long name) from PropertyValueAliases.txt and DerivedBidiClass.txt
    // third column (description) from https://www.unicode.org/reports/tr9/#Table_Bidirectional_Character_Types
    "L",    "Left_To_Right",            // Left-to-Right
    "LRE",  "Left_To_Right_Embedding",  // Left-to-Right Embedding
    "LRO",  "Left_To_Right_Override",   // Left-to-Right Override
    "R",    "Right_To_Left",            // Right-to-Left
    "AL",   "Arabic_Letter",            // Right-to-Left Arabic
    "RLE",  "Right_To_Left_Embedding",  // Right-to-Left Embedding
    "RLO",  "Right_To_Left_Override",   // Right-to-Left Override
    "PDF",  "Pop_Directional_Format",   // Pop Directional Format
    "EN",   "European_Number",          // European Number
    "ES",   "European_Separator",       // European Number Separator
    "ET",   "European_Terminator",      // European Number Terminator
    "AN",   "Arabic_Number",            // Arabic Number
    "CS",   "Common_Separator",         // Common Number Separator
    "NSM",  "Nonspacing_Mark",          // Nonspacing Mark
    "BN",   "Boundary_Neutral",         // Boundary Neutral
    "B",    "Paragraph_Separator",      // Paragraph Separator
    "S",    "Segment_Separator",        // Segment Separator
    "WS",   "White_Space",              // Whitespace
    "ON",   "Other_Neutral",            // Other Neutrals
    "LRI",  "Left_To_Right_Isolate",    // Left-to-Right Isolate
    "RLI",  "Right_To_Left_Isolate",    // Right-to-Left Isolate
    "FSI",  "First_Strong_Isolate",     // First Strong Isolate
    "PDI",  "Pop_Directional_Isolate"   // Pop Directional Isolate
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
        // I don't use "L" as default value because DerivedBidiClass.txt defines many exceptions.
        // Problem: these exceptions are all in comments... So, do they apply or not?
        const char *strCode = rangeOk ? Bidirectional_Character_Types[2 * index] : "?";
        RexxString *rexxCode = new_string(strCode); // Protected<RexxString> not needed
        refCode->setValue(rexxCode);
    }

    if (refLabel != OREF_NULL)
    {
        const char *strLabel = rangeOk ? Bidirectional_Character_Types[1 + 2 * index] : "Code point not explicitly listed for Bidi_Class";
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


// https://www.unicode.org/reports/tr44/#Formatting_Tags_Table --> strange codes
// DerivedDecompositionType.txt --> good codes
//   #  All code points not explicitly listed for Decomposition_Type
//   #  have the value None.
const char *Compatibility_Formatting_Tags[]=
{
    "Font", "Font variant",
    "Nobreak", "No-break version of a space or hyphen",
    "Initial", "Initial presentation form (Arabic)",
    "Medial", "Medial presentation form (Arabic)",
    "Final", "Final presentation form (Arabic)",
    "Isolated", "Isolated presentation form (Arabic)",
    "Circle", "Encircled form",
    "Super", "Superscript form",
    "Sub", "Subscript form",
    "Vertical", "Vertical layout presentation form",
    "Wide", "Wide (or zenkaku) compatibility character",
    "Narrow", "Narrow (or hankaku) compatibility character",
    "Small", "Small variant form (CNS compatibility)",
    "Square", "CJK squared font variant",
    "Fraction", "Vulgar fraction form",
    "Compat", "Unspecified compatibility character"
};

RexxInteger *RexxUnicodeServicesClass::codepointDecompositionType(RexxInteger *rexxCodepoint, VariableReference *refCode, VariableReference *refLabel)
{
    requiredArgument(rexxCodepoint, "codepoint");
    utf8proc_int32_t codepoint = (utf8proc_int32_t)integer(rexxCodepoint, "codepoint must be an integer");
    if (refCode != OREF_NULL) classArgument(refCode, TheVariableReferenceClass, "refCode");
    if (refLabel != OREF_NULL) classArgument(refLabel, TheVariableReferenceClass, "refLabel");

    const utf8proc_property_t *property = utf8proc_get_property(codepoint);
    utf8proc_propval_t decomp_type = property->decomp_type; // 1..16
    utf8proc_propval_t index = decomp_type - 1;

    // Check array out-of-bounds
    int count = sizeof(Compatibility_Formatting_Tags) / sizeof(Compatibility_Formatting_Tags[0]);
    bool rangeOk = index >= 0 && index < count/2;

    if (refCode != OREF_NULL)
    {
        const char *strCode = rangeOk ? Compatibility_Formatting_Tags[2 * index] : "None";
        RexxString *rexxCode = new_string(strCode); // Protected<RexxString> not needed
        refCode->setValue(rexxCode);
    }

    if (refLabel != OREF_NULL)
    {
        const char *strLabel = rangeOk ? Compatibility_Formatting_Tags[1 + 2 * index] : "Code point not explicitly listed for Decomposition_Type";
        RexxString *rexxLabel = new_string(strLabel); // Protected<RexxString> not needed
        refLabel->setValue(rexxLabel);
    }

    return new_integer(decomp_type); // see utf8proc_decomp_type_t
}


// Indic_Conjunct_Break property. (TR44)
// This property defines values used in Grapheme Cluster Break algorithm in [UAX29].
// https://unicode.org/reports/tr29/
// See Derivation of Indic_Conjunct_Break for an explanation of its derivation.
// https://www.unicode.org/reports/tr44/#Derivation_InCB
// The default value for InCB is None
const char *Indic_Conjunct_Break[]=
{
    "None", "None",
    "Linker", "Linker",
    "Consonant", "Consonant",
    "Extend", "Extend"
};

RexxInteger *RexxUnicodeServicesClass::codepointIndicConjunctBreak(RexxInteger *rexxCodepoint, VariableReference *refCode, VariableReference *refLabel)
{
    requiredArgument(rexxCodepoint, "codepoint");
    utf8proc_int32_t codepoint = (utf8proc_int32_t)integer(rexxCodepoint, "codepoint must be an integer");
    if (refCode != OREF_NULL) classArgument(refCode, TheVariableReferenceClass, "refCode");
    if (refLabel != OREF_NULL) classArgument(refLabel, TheVariableReferenceClass, "refLabel");

    const utf8proc_property_t *property = utf8proc_get_property(codepoint);
    utf8proc_propval_t indic_conjunct_break = property->indic_conjunct_break; // 0..3
    utf8proc_propval_t index = indic_conjunct_break;

    // Check array out-of-bounds
    int count = sizeof(Indic_Conjunct_Break) / sizeof(Indic_Conjunct_Break[0]);
    bool rangeOk = index >= 0 && index < count/2;

    if (refCode != OREF_NULL)
    {
        const char *strCode = rangeOk ? Indic_Conjunct_Break[2 * index] : "None";
        RexxString *rexxCode = new_string(strCode); // Protected<RexxString> not needed
        refCode->setValue(rexxCode);
    }

    if (refLabel != OREF_NULL)
    {
        const char *strLabel = rangeOk ? Indic_Conjunct_Break[1 + 2 * index] : "None";
        RexxString *rexxLabel = new_string(strLabel); // Protected<RexxString> not needed
        refLabel->setValue(rexxLabel);
    }

    return new_integer(indic_conjunct_break); // see utf8proc_indic_conjunct_break_t
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
}


// This enumerated type is not available with utf8proc.
// But the boolean ambiguous_width is available: true when East Asian width class A.
//
// East_Asian_Width property.
// PropertyValueAliases.txt
// EastAsianWidth.txt
// #  - All code points, assigned or unassigned, that are not listed
// #      explicitly are given the value "N".
// #  - The unassigned code points in the following blocks default to "W":
// #         CJK Unified Ideographs Extension A: U+3400..U+4DBF
// #         CJK Unified Ideographs:             U+4E00..U+9FFF
// #         CJK Compatibility Ideographs:       U+F900..U+FAFF
// #  - All undesignated code points in Planes 2 and 3, whether inside or
// #      outside of allocated blocks, default to "W":
// #         Plane 2:                            U+20000..U+2FFFD
// #         Plane 3:                            U+30000..U+3FFFD
// #
const char *East_Asian_Width[]=
{
    "A",    "Ambiguous",
    "F",    "Fullwidth",
    "H",    "Halfwidth",
    "N",    "Neutral",
    "Na",   "Narrow",
    "W",    "Wide"
};

RexxInteger *RexxUnicodeServicesClass::codepointEastAsianWidthIsAmbiguous(RexxInteger *rexxCodepoint)
{
    requiredArgument(rexxCodepoint, "codepoint");
    utf8proc_int32_t codepoint = (utf8proc_int32_t)integer(rexxCodepoint, "codepoint must be an integer");
    const utf8proc_property_t *property = utf8proc_get_property(codepoint);
    return property->ambiguous_width ? TheTrueObject : TheFalseObject;
}

// Boundclass property. (TR29)
// auxiliary/GraphemeBreakProperty.txt
//   #  All code points not explicitly listed for Grapheme_Cluster_Break
//   #  have the value Other (XX).
// PropertyValueAliases.txt
//     # Grapheme_Cluster_Break (GCB)
const char *Grapheme_Cluster_Break[]=
{
    "Start",    "Start",
    "XX",       "Other",
    "CR",       "CR",
    "LF",       "LF",
    "CN",       "Control",
    "EX",       "Extend",
    "L",        "L",
    "V",        "V",
    "T",        "T",
    "LV",       "LV",
    "LVT",      "LVT",
    "RI",       "Regional_Indicator",
    "SM",       "SpacingMark",
    "PP",       "Prepend",
    "ZWJ",      "ZWJ",                  // Zero Width Joiner

  /* the following are no longer used in Unicode 11, but we keep
     the constants here for backward compatibility */
    "EB",       "E_Base",               // Emoji Base
    "EM",       "E_Modifier",           // Emoji Modifier
    "GAZ",      "Glue_After_Zwj",
    "EBG",      "E_Base_GAZ",           // E_BASE + GLUE_AFTER_ZJW

  /* the Extended_Pictographic property is used in the Unicode 11
     grapheme-boundary rules, so we store it in the boundclass field */
    "EP",       "Extended_Pictographic",
    "EZWG",     "E_ZWG"                 // UTF8PROC_BOUNDCLASS_EXTENDED_PICTOGRAPHIC + ZWJ
};

RexxInteger *RexxUnicodeServicesClass::codepointBoundClass(RexxInteger *rexxCodepoint, VariableReference *refCode, VariableReference *refLabel)
{
    requiredArgument(rexxCodepoint, "codepoint");
    utf8proc_int32_t codepoint = (utf8proc_int32_t)integer(rexxCodepoint, "codepoint must be an integer");
    if (refCode != OREF_NULL) classArgument(refCode, TheVariableReferenceClass, "refCode");
    if (refLabel != OREF_NULL) classArgument(refLabel, TheVariableReferenceClass, "refLabel");

    const utf8proc_property_t *property = utf8proc_get_property(codepoint);
    utf8proc_propval_t boundclass = property->boundclass; // 0..19
    utf8proc_propval_t index = boundclass;

    // Check array out-of-bounds
    int count = sizeof(Grapheme_Cluster_Break) / sizeof(Grapheme_Cluster_Break[0]);
    bool rangeOk = index >= 0 && index < count/2;

    if (refCode != OREF_NULL)
    {
        const char *strCode = rangeOk ? Grapheme_Cluster_Break[2 * index] : "XX";
        RexxString *rexxCode = new_string(strCode); // Protected<RexxString> not needed
        refCode->setValue(rexxCode);
    }

    if (refLabel != OREF_NULL)
    {
        const char *strLabel = rangeOk ? Grapheme_Cluster_Break[1 + 2 * index] : "Other";
        RexxString *rexxLabel = new_string(strLabel); // Protected<RexxString> not needed
        refLabel->setValue(rexxLabel);
    }

    return new_integer(boundclass); // see utf8proc_boundclass_t
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
