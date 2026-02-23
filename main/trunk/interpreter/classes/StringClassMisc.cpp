/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 1995, 2004 IBM Corporation. All rights reserved.             */
/* Copyright (c) 2005-2019 Rexx Language Association. All rights reserved.    */
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
/* REXX Kernel                                                                */
/*                                                                            */
/* Miscellaneous REXX string methods                                          */
/*                                                                            */
/******************************************************************************/
#include "RexxCore.h"
#include "StringClass.hpp"
#include "ActivityManager.hpp"
#include "StringUtil.hpp"
#include "LanguageParser.hpp"
#include "MethodArguments.hpp"
#include "MutableBufferClass.hpp"
#include "RoutineClass.hpp"
#include "VariableReference.hpp"

#include <ctype.h>

/**
 * Determine if a string is a valid symbol, and what type
 * of symbol.
 *
 * @return The indicator of its symbol status.
 */
StringSymbolType RexxString::isSymbol()
{
    // the language parser handles all of these rules
    return LanguageParser::scanSymbol(this);
}


/**
 * Test is a string is an abbreviation of a target string.
 *
 * @param info    The test string.  We're testing if info is an
 *                abbreviation of the receiving string.
 * @param _length The minumum length for being a valid abbreviation.
 *
 * @return .true if this qualfies as an abbreviation, .false if not.
 */
RexxObject *RexxString::abbrev(RexxString *info, RexxInteger *_length)
{
    // make sure this is a good string argument
    info = stringArgument(info, ARG_ONE);
    size_t len2 = info->getLength();

    // get the optional minimum length...the default is the length of the
    // test string.
    size_t checkLen = optionalLengthArgument(_length, len2, ARG_TWO);
    size_t len1 = getLength();

    // if info is a null string and the minimum length is zero, then
    // this passes.
    if (checkLen == 0 && len2 == 0)
    {
        return TheTrueObject;
    }

    // we can reject some possibilities based on length checks
    // alone:
    // 1) the receiving string is a null string and the info is non-null
    // 2) the info length is less than the minimum
    // 3) the info length is longer than the receiving length
    if (len1 == 0 || (len2 < checkLen) || (len1 < len2))
    {
        return TheFalseObject;
    }

    // we do a comparison for the info length
    return booleanObject(memcmp(getStringData(), info->getStringData(), len2) == 0);
}


/**
 * Test is a string is an abbreviation of a target string.
 *
 * @param info    The test string.  We're testing if info is an
 *                abbreviation of the receiving string.
 * @param _length The minumum length for being a valid abbreviation.
 *
 * @return .true if this qualfies as an abbreviation, .false if not.
 */
RexxObject *RexxString::caselessAbbrev(RexxString *info, RexxInteger *_length)
{
    // make sure this is a good string argument
    info = stringArgument(info, ARG_ONE);
    size_t len2 = info->getLength();

    // get the optional minimum length...the default is the length of the
    // test string.
    size_t checkLen = optionalLengthArgument(_length, len2, ARG_TWO);
    size_t len1 = getLength();

    // if info is a null string and the minimum length is zero, then
    // this passes.
    if (checkLen == 0 && len2 == 0)
    {
        return TheTrueObject;
    }

    // we can reject some possibilities based on length checks
    // alone:
    // 1) the receiving string is a null string and the info is non-null
    // 2) the info length is less than the minimum
    // 3) the info length is longer than the receiving length
    if (len1 == 0 || (len2 < checkLen) || (len1 < len2))
    {
        return TheFalseObject;
    }

    // we do a comparison for the info length
    return booleanObject(StringUtil::caselessCompare(getStringData(), info->getStringData(), len2) == 0);
}


/**
 * Compare two strings, returning the point where they differ.
 *
 * @param string2 the other comparison string.
 * @param pad     The optional pad character.
 *
 * @return The point of mismatch, or 0 if the two strings
 *         compare equal.
 */
RexxInteger *RexxString::compare(RexxString *other, RexxString *pad)
{
    size_t length1 = getLength();
    other = stringArgument(other, ARG_ONE);
    size_t length2 = other->getLength();
    char padChar = optionalPadArgument(pad, ' ', ARG_TWO);

    const char *string1;
    const char *string2;
    size_t leadLength;
    size_t padLength;

    // padding decisions and compare lengths are based on the shorter of
    // the two strings.
    if (length1 > length2)
    {
        string1 = getStringData();
        string2 = other->getStringData();
        leadLength = length2;
        padLength = length1 - length2;
    }
    else
    {
        string2 = getStringData();
        string1 = other->getStringData();
        leadLength = length1;
        padLength = length2 - length1;
    }

    // scan the leading length for a mismatch
    for (size_t i = 0; i < leadLength; i++)
    {
        // if the strings do not compare equal, return this index position
        // as the result.
        if (string1[i] != string2[i])
        {
            return new_integer(i + 1);
        }
    }

    // step to the trailing portion, if any
    string1 += leadLength;
    for (size_t i = 0; i < padLength; i++)
    {
        // if this does not match the pad character, return the position
        if (string1[i] != padChar)
        {
            return new_integer(leadLength + i + 1);
        }
    }

    // no mismatches found, return zero as a result
    return IntegerZero;
}


/**
 * Caseless version of the compare() method.
 *
 * @param string2 The string to compare to.
 * @param pad     The padding character used for length mismatches.
 *
 * @return 0 if the two strings are equal (with padding applied), otherwise
 *         it returns the mismatch position.
 */
RexxInteger *RexxString::caselessCompare(RexxString *other, RexxString *pad)
{
    size_t length1 = getLength();
    other = stringArgument(other, ARG_ONE);
    size_t length2 = other->getLength();
    // since this is caseless, uppercase the pad character
    char padChar = Utilities::toUpper(optionalPadArgument(pad, ' ', ARG_TWO));

    const char *string1;
    const char *string2;
    size_t leadLength;
    size_t padLength;

    // padding decisions and compare lengths are based on the shorter of
    // the two strings.
    if (length1 > length2)
    {
        string1 = getStringData();
        string2 = other->getStringData();
        leadLength = length2;
        padLength = length1 - length2;
    }
    else
    {
        string2 = getStringData();
        string1 = other->getStringData();
        leadLength = length1;
        padLength = length2 - length1;
    }

    // scan the leading length for a mismatch
    for (size_t i = 0; i < leadLength; i++)
    {
        // if the strings do not compare equal, return this index position
        // as the result.
        if (Utilities::toUpper(string1[i]) != Utilities::toUpper(string2[i]))
        {
            return new_integer(i + 1);
        }
    }

    // step to the trailing portion, if any
    string1 += leadLength;
    for (size_t i = 0; i < padLength; i++)
    {
        // if this does not match the pad character, return the position
        if (Utilities::toUpper(string1[i]) != padChar)
        {
            return new_integer(leadLength + i + 1);
        }
    }

    // no mismatches found, return zero as a result
    return IntegerZero;
}


/**
 * The copies method.
 *
 * @param _copies The number of copies of the string to make.
 *
 * @return The string with the copies.
 */
RexxString *RexxString::copies(RexxInteger *_copies)
{
    // the copy count is required, and must
    size_t count = nonNegativeArgument(_copies, ARG_ONE);
    size_t len = getLength();

    // if no copies have been requested or we're making copies of
    // a null string, the result is a null string
    if (count == 0 || len == 0)
    {
        return GlobalNames::NULLSTRING;
    }

    // if just a single copy requested, we can just return this string
    if (count == 1)
    {
        return this;
    }

    // get a result string large enough for the requested number of copies
    RexxString *retval = raw_string(len * count);
    StringBuilder builder(retval);

    // copy in count times
    while (count--)
    {
        builder.append(getStringData(), len);
    }
    return retval;
}


/**
 * String class DATATYPE method
 *
 * @param pType  The requested type.
 *
 * @return if a type is specified, returns .true or .false based on the type match.  Otherwise,
 *         returns "NUM" or "CHAR".
 */
RexxObject *RexxString::dataType(RexxString *pType)
{
    // if the type was specified, get the option character and perform the check
    if (pType != OREF_NULL)
    {
        int type = optionalOptionArgument(pType, 0, ARG_ONE);
        return StringUtil::dataType(this, type);
    }

    // no type specified, this is a check for number or char.  We call the
    // same validation code, but need a different return result.
    return (StringUtil::dataType(this, 'N') == TheTrueObject ? GlobalNames::NUM : GlobalNames::CHAR);
}


/**
 * Do a lastpos() search for a string.
 *
 * @param needle The search needle.
 * @param _start The starting position.
 *
 * @return the offset of the match position (origin 1).  Returns 0
 *         if no match was found.
 */
RexxInteger *RexxString::lastPosRexx(RexxString  *needle, RexxInteger *_start, RexxInteger *_range)
{
    return StringUtil::lastPosRexx(getStringData(), getLength(), needle, _start, _range);
}


/**
 * Rexx exported version of the caselessLastPos() method.
 *
 * @param needle The search needle.
 * @param _start The starting position.
 *
 * @return The match position.  0 means not found.
 */
RexxInteger *RexxString::caselessLastPosRexx(RexxString  *needle, RexxInteger *_start, RexxInteger *_range)
{
    // validate that this is a good string argument
    needle = stringArgument(needle, ARG_ONE);
    // find out where to start the search. The default is at the very end.
    size_t startPos = optionalPositionArgument(_start, getLength(), ARG_TWO);
    size_t range = optionalLengthArgument(_range, getLength(), ARG_THREE);
    // now perform the actual search.
    return new_integer(StringUtil::caselessLastPos(getStringData(), getLength(), needle, startPos, range));
}


/**
 * Primitive implementation of a lastpos search.
 *
 * @param needle The search needle.
 * @param start  The starting position (origin 1).
 *
 * @return Returns the last match position, searching back from the start
 *         position.  The starting position is the right-most character
 *         of the past possible match (as if the string was truncated
 *         at start).
 */
size_t RexxString::lastPos(RexxString  *needle, size_t _start)
{
    return StringUtil::lastPos(getStringData(), getLength(), needle, _start, getLength());
}


/**
 * Primitive implementation of a caseless lastpos search.
 *
 * @param needle The search needle.
 * @param start  The starting position (origin 1).
 *
 * @return Returns the last match position, searching back from the start
 *         position.  The starting position is the right-most character
 *         of the past possible match (as if the string was truncated
 *         at start).
 */
size_t RexxString::caselessLastPos(RexxString *needle, size_t _start)
{
    return StringUtil::caselessLastPos(getStringData(), getLength(), needle, _start, getLength());
}


/**
 * Count occurrences of one string in another.
 *
 * @param needle The string we're counting
 *
 * @return The count of occurrences in the target string.
 */
RexxInteger *RexxString::countStrRexx(RexxString *needle)
{
    needle = stringArgument(needle, ARG_ONE);
    // delegate the counting to the string util
    return new_integer(StringUtil::countStr(getStringData(), getLength(), needle, Numerics::MAX_WHOLENUMBER));
}


/**
 * Caselessly count occurrences of one string in another.
 *
 * @param needle The search needle.
 *
 * @return The string count.
 */
RexxInteger *RexxString::caselessCountStrRexx(RexxString *needle)
{
    needle = stringArgument(needle, ARG_ONE);
    // delegate the counting to the string util
    return new_integer(StringUtil::caselessCountStr(getStringData(), getLength(), needle, Numerics::MAX_WHOLENUMBER));
}


/**
 * Change occurrences of values in a string into another value.
 *
 * @param needle    The search needle we're changing.
 * @param newNeedle The replacement string
 * @param countArg  The optional count of occurrences to change.
 *
 * @return A new string with the replacements made.
 */
RexxString *RexxString::changeStr(RexxString *needle, RexxString *newNeedle, RexxInteger *countArg)
{
    // both needles must be string arguments
    needle = stringArgument(needle, ARG_ONE);
    newNeedle = stringArgument(newNeedle, ARG_TWO);

    // we'll only change up to a specified count.  If not there, we do everything.
    size_t count = optionalNonNegative(countArg, Numerics::MAX_WHOLENUMBER, ARG_THREE);
    // if no change is requested, return the original string.
    if (count == 0)
    {
        return this;
    }

    // find the number of matches (up to count) in the string
    count = StringUtil::countStr(getStringData(), getLength(), needle, count);

    // if we don't have anything we can change, return the original string.
    if (count == 0)
    {
        return this;
    }

    size_t needleLength = needle->getLength();
    size_t newLength = newNeedle->getLength();
    // get the proper size result string based on the number of changes we need to make.

    RexxString *result = raw_string(getLength() - (count * needleLength) + (count * newLength));
    StringBuilder builder(result);

    const char *source = getStringData();

    const char *newPtr = newNeedle->getStringData();
    size_t start = 0;
    // now make changes until we either hit our target count
    for (size_t i = 0; i < count; i++)
    {
        // find the next occurrence.  Since we already know how many
        // occurrences there are, we should never not get a hit
        size_t matchPos = pos(needle, start);
        // get the prefix length to copy
        size_t copyLength = (matchPos - 1) - start;
        builder.append(source + start, copyLength);
        // not changing to a null string?  copy over an occurrence of the new value.
        builder.append(newPtr, newLength);

        // step to the next search start position
        start = matchPos + needleLength - 1;
    }

    // have something left at the end, copy on to the end of the string
    builder.append(source + start, getLength() - start);
    return result;
}


/**
 * Change occurrences of values in a string into another value.
 *
 * @param needle    The search needle we're changing.
 * @param newNeedle The replacement string
 * @param countArg  The optional count of occurrences to change.
 *
 * @return A new string with the replacements made.
 */
RexxString *RexxString::caselessChangeStr(RexxString *needle, RexxString *newNeedle, RexxInteger *countArg)
{
    // both needles must be string arguments
    needle = stringArgument(needle, ARG_ONE);
    newNeedle = stringArgument(newNeedle, ARG_TWO);

    // we'll only change up to a specified count.  If not there, we do everything.
    size_t count = optionalNonNegative(countArg, Numerics::MAX_WHOLENUMBER, ARG_THREE);
    // if no change is requested, return the original string.
    if (count == 0)
    {
        return this;
    }

    // find the number of matches (up to count) in the string
    count = StringUtil::caselessCountStr(getStringData(), getLength(), needle, count);

    // if we don't have anything we can change, return the original string.
    if (count == 0)
    {
        return this;
    }

    size_t needleLength = needle->getLength();
    size_t newLength = newNeedle->getLength();
    // get the proper size result string based on the number of changes we need to make.

    RexxString *result = raw_string(getLength() - (count * needleLength) + (count * newLength));
    StringBuilder builder(result);

    const char *source = getStringData();

    const char *newPtr = newNeedle->getStringData();
    size_t start = 0;
    // now make changes until we either hit our target count
    for (size_t i = 0; i < count; i++)
    {
        // find the next occurrence.  Since we already know how many
        // occurrences there are, we should never not get a hit
        size_t matchPos = caselessPos(needle, start);
        // get the prefix length to copy
        size_t copyLength = (matchPos - 1) - start;
        builder.append(source + start, copyLength);
        // not changing to a null string?  copy over an occurrence of the new value.
        builder.append(newPtr, newLength);

        // step to the next search start position
        start = matchPos + needleLength - 1;
    }

    // have something left at the end, copy on to the end of the string
    builder.append(source + start, getLength() - start);
    return result;
}


/**
 * Search for an occurrence of one string in another.
 *
 * @param needle The search needle.
 * @param pstart The postion to start.
 * @param range  The end range of the search.
 *
 * @return 0 if the string is not found or the position of the first match.
 */
RexxInteger *RexxString::posRexx(RexxString  *needle, RexxInteger *pstart, RexxInteger *range)
{
    return StringUtil::posRexx(getStringData(), getLength(), needle, pstart, range);
}


/**
 * Test if a string contains a given string within a specified
 * range.
 *
 * @param needle The search needle
 * @param pstart The starting search position.
 * @param range  The length of the range
 *
 * @return .true if the string is found, .false otherwise.
 */
RexxObject *RexxString::containsRexx(RexxString  *needle, RexxInteger *pstart, RexxInteger *range)
{
    return StringUtil::containsRexx(getStringData(), getLength(), needle, pstart, range);
}


/**
 * Do a caseless search for one string in another.
 *
 * @param needle The search string.
 * @param pstart The starting position for the search.
 * @param range  A maximum range for the search.
 *
 * @return The index of any match position, or 0 if not found.
 */
RexxInteger *RexxString::caselessPosRexx(RexxString *needle, RexxInteger *pstart, RexxInteger *range)
{
    needle = stringArgument(needle, ARG_ONE);
    size_t _start = optionalPositionArgument(pstart, 1, ARG_TWO);
    size_t _range = optionalLengthArgument(range, getLength() - _start + 1, ARG_THREE);

    // return the match result as an integer object.
    return new_integer(StringUtil::caselessPos(getStringData(), getLength(), needle , _start - 1, _range));
}


/**
 * Do a caseless search for one string in another.
 *
 * @param needle The search string.
 * @param pstart The starting position for the search.
 * @param range  A maximum range for the search.
 *
 * @return .true if the string is found, .false otherwise.
 */
RexxObject *RexxString::caselessContains(RexxString *needle, RexxInteger *pstart, RexxInteger *range)
{
    needle = stringArgument(needle, ARG_ONE);
    size_t _start = optionalPositionArgument(pstart, 1, ARG_TWO);
    size_t _range = optionalLengthArgument(range, getLength() - _start + 1, ARG_THREE);

    return booleanObject(StringUtil::caselessPos(getStringData(), getLength(), needle , _start - 1, _range) > 0);
}


/**
 * Do a primitive level pos() search on a string.
 *
 * @param needle The search needle.
 * @param _start The starting position (origin 0)
 *
 * @return The match position (origin 1).  Returns 0 for no match.
 */
size_t RexxString::pos(RexxString *needle, size_t _start)
{
    return StringUtil::pos(getStringData(), getLength(), needle, _start, getLength());
}


/**
 * Do a primitive level pos() search on a string.
 *
 * @param needle The search needle.
 * @param _start The starting position (origin 0)
 *
 * @return The match position (origin 1).  Returns 0 for no match.
 */
size_t RexxString::caselessPos(RexxString *needle, size_t _start)
{
    return StringUtil::caselessPos(getStringData(), getLength(), needle, _start, getLength());
}


/**
 * Translate characters in a string into another character.
 *
 * @param tableo The output translate table.
 * @param tablei The input translate table,
 * @param pad    The pad character.
 * @param _start The starting position for the translate.
 * @param _range The length to apply the translation to.
 *
 * @return The string with the translations applied.
 */
RexxString *RexxString::translate(RexxString *tableo, RexxString *tablei, RexxString *pad,
    RexxInteger *_start, RexxInteger *_range)
{
    // if there are no input or output table specified, and no pad, this is just an uppercase.
    if (tableo == OREF_NULL && tablei == OREF_NULL && pad == OREF_NULL)
    {
        return upperRexx(_start, _range);
    }

    // get our tables...a null string is the default, which means we use the
    // pad character for everything.
    tableo = optionalStringArgument(tableo, GlobalNames::NULLSTRING, ARG_ONE);
    size_t outTableLength = tableo->getLength();

    tablei = optionalStringArgument(tablei, GlobalNames::NULLSTRING, ARG_TWO);
    size_t inTableLength = tablei->getLength();
    const char *inTable = tablei->getStringData();
    const char *outTable = tableo->getStringData();

    // now the pad character, which defaults to a blank
    char padChar = optionalPadArgument(pad, ' ', ARG_THREE);

    // get the optional start position and length
    size_t startPos = optionalPositionArgument(_start, 1, ARG_FOUR);
    size_t range = optionalLengthArgument(_range, getLength() - startPos + 1, ARG_FIVE);

    // if nothing to translate, we can return now
    if (startPos > getLength() || range == 0)
    {
        return this;
    }

    // cap the real range
    range = std::min(range, getLength() - startPos + 1);

    // get a new string using the original string data.  We'll make
    // changes in place in the new string
    RexxString *retval = new_string(getStringData(), getLength());
    char *scanPtr = retval->getWritableData() + startPos - 1;
    size_t scanLength = range;

    // now scan the range section
    while (scanLength--)
    {
        char ch = *scanPtr;

        size_t position;
        // if we have a real input table, then scan for the
        // character.
        if (tablei != GlobalNames::NULLSTRING)
        {
            // search for the character
            position = StringUtil::memPos(inTable, inTableLength, ch);
        }
        // the position is the character itself
        else
        {
            // no input table, so the position is the character itself
            position = ((size_t)ch) & 0xff;
        }

        if (position != (size_t)(-1))
        {
            // if the output table is large enough, use the table value, otherwise
            // use the pad character.
            if (position < outTableLength)
            {
                *scanPtr = *(outTable + position);
            }
            else
            {
                *scanPtr = padChar;
            }
        }
        scanPtr++;
    }
    return retval;
}


/**
 * Translate sequences of characters into another sequence of characters.
 * Append the translated sequences of characters to the destination mutable buffer.
 * The resulting string may be shorter or longer than the receiving string.
 *
 * tablei and tableo can be either a string or an array.
 *
 * If tablei is a string:
 *   - each character in the receiving string is searched in tablei.
 *   - If the character is found, the corresponding translation item in tableo
 *     is tried.
 * If tablei is an array, the matcher items are:
 *   - either a routine that maybe matches a sequence of characters,
 *   - or any non-NIL object whose string representation is used as a sequence
 *     of characters for matching in the receiving string.
 *   - or .NIL which is ignored
 *   - or OREF_NULL (an omitted item in the array) which is ignored.
 *   - If there is a match, the corresponding translation item in tableo is tried.
 *
 * If tableo is a string, the translation items are characters.
 * If tableo is an array, the translation items are:
 *   - either a routine that maybe appends a sequence of characters,
 *   - or any non-NIL object whose string representation is used as a sequence
 *     of characters to append. A string representation is always applicable.
 *   - or .NIL to stop the translation.
 *   - or OREF_NULL (an omitted item in the array) to stop the translation.
 *
 * Interpretation of the result returned by a routine:
 *           |    no result     | 0 or not a number | > 0 (offset of next position)
 * in tablei | stop translation | no match          | match
 * in tableo | stop translation | not applicable    | translation applied
 *
 * If there are duplicates in tablei:
 *   - the first (leftmost if string) occurrence is selected.
 *   - if the corresponding translation item is a routine and this routine
 *     returns 0, then the next occurrence in tablei is selected.
 *     Repeat until a translation item is applicable, or until there are no more
 *     occurences.
 *   - otherwise only the first corresponding translation item is used.
 *
 * If there is a match and the corresponding translation item is applicable, the
 * translation is appended to the destination. The next position in the receiving
 * string is updated:
 *   - Character matching: advance by one character.
 *   - String matching: advance by the length of the corresponding string.
 *   - Matching by routine: advance by the returned number (unless overriden)
 *   - Translation by routine: advance by the returned number (override)
 *
 * If there is no match or no applicable translation item, the current character
 * in the receiving string is appended to the destination. The next position in
 * the receiving string is advanced by one character.
 *
 * At the end of the translation, the current position in the receiving string
 * is returned in the optional refEnd variable reference.
 *
 * @param destination The destination of the translations (a mutable buffer)
 * @param tableo      The output translate table (either a string or an array).
 * @param tablei      The input translate table (either a string or an array).
 * @param pad         The pad character.
 * @param _start      The starting position for the translate.
 * @param _range      The length to apply the translation to.
 * @param refEnd      The ending position of the translation (output argument).
 *
 * @return The destination object with the translations applied.
 */
MutableBuffer *RexxString::translateInto(MutableBuffer *destination, RexxObject *tableo, RexxObject *tablei, RexxString *pad,
    RexxInteger *_start, RexxInteger *_range, VariableReference *refEnd)
{
    classArgument(destination, TheMutableBufferClass, "destination");

    // get our tables...

    size_t outTableLength = 0;
    Protected<RexxString> outString = (RexxString *)OREF_NULL;
    const char *outStringData = OREF_NULL;
    Protected<ArrayClass> outArray = (ArrayClass *)OREF_NULL;
    if (tableo == OREF_NULL || tableo->isInstanceOf(TheStringClass))
    {
        // a null string is the default, which means we use the
        // pad character for everything.
        outString = optionalStringArgument(tableo, GlobalNames::NULLSTRING, ARG_TWO);
        outTableLength = outString->getLength();
        outStringData = outString->getStringData();
    }
    else
    {
        outArray = (ArrayClass *)arrayArgument(tableo, ARG_TWO)->copy();
        outTableLength = outArray->size(); // yes! size(), not items()
        // requestString now, to avoid repetitive requests when translating
        for (size_t i=1; i <= outTableLength; i++)
        {
            RexxObject *item = (RexxObject *)outArray->get(i);
            if (item != OREF_NULL && item != TheNilObject && !item->isInstanceOf(TheRoutineClass))
            {
                Protected<RexxString> string = item->requestString();
                outArray->put(string, i);
            }
        }
    }

    size_t inTableLength = 0;
    Protected<RexxString> inString = (RexxString *)OREF_NULL;
    const char *inStringData = OREF_NULL;
    Protected<ArrayClass> inArray = (ArrayClass *)OREF_NULL;
    if (tablei == OREF_NULL || tablei->isInstanceOf(TheStringClass))
    {
        // a null string is the default, which means the position is the
        // character itself.
        inString = optionalStringArgument(tablei, GlobalNames::NULLSTRING, ARG_THREE);
        inTableLength = inString->getLength();
        inStringData = inString->getStringData();
    }
    else
    {
        inArray = (ArrayClass *)arrayArgument(tablei, ARG_TWO)->copy();
        inTableLength = inArray->size(); // yes! size(), not items()
        // requestString now, to avoid repetitive requests when translating
        for (size_t i=1; i <= inTableLength; i++)
        {
            RexxObject *item = (RexxObject *)inArray->get(i);
            if (item != OREF_NULL && item != TheNilObject && !item->isInstanceOf(TheRoutineClass))
            {
                Protected<RexxString> string = item->requestString();
                inArray->put(string, i);
            }
        }
    }

    // now the pad character, which defaults to a blank
    char padChar = optionalPadArgument(pad, ' ', ARG_FOUR);

    // get the optional start position and length
    size_t startPos = optionalPositionArgument(_start, 1, ARG_FIVE);
    size_t range = optionalLengthArgument(_range, getLength() - startPos + 1, ARG_SIX);

    // check the optional refEnd output argument
    if (refEnd != OREF_NULL) classArgument(refEnd, TheVariableReferenceClass, "end");

    // cap the real range
    range = std::min(range, getLength() - startPos + 1);

    // if there are no input or output table specified, and no pad, this is just an uppercase.
    if (tableo == OREF_NULL && tablei == OREF_NULL && pad == OREF_NULL)
    {
        destination->append(upperRexx(_start, _range));
        // returns the current position in the receiving string via a variable reference
        if (refEnd != OREF_NULL)
        {
            size_t pos = startPos + range;
            Protected<RexxInteger> rexxPos = new_integer(pos);
            refEnd->setValue(rexxPos);
        }
        return destination;
    }

    // if nothing to translate, we can return now
    if (startPos > getLength() || range == 0)
    {
        destination->append(this);
        // returns the current position in the receiving string via a variable reference
        if (refEnd != OREF_NULL)
        {
            Protected<RexxInteger> rexxStartPos = new_integer(startPos);
            refEnd->setValue(rexxStartPos);
        }
        return destination;
    }

    // copy the characters before startPos (one-based)
    if (startPos > 1) destination->append(this->getStringData(), startPos - 1);

    size_t pos = startPos; // one-based current position in the receiving string, will be passed to the routines
    const char *scanPtr = this->getStringData() + startPos - 1; // the equivalent of pos, as a pointer
    size_t scanLength = range;

    // The translation can be stopped by an omitted item or .NIL in tableo,
    // or by a routine that doesn't return a result.
    bool stopped = false;

    // now scan the range section
    while (scanLength && !stopped)
    {
        // In case of matching by character, the number of characters to skip is 1.
        // In case of matching by string, the number of characters to skip is the matched string length.
        // In case of matching by routine, the routine will return the number of matched characters to skip.
        // In case of translation by routine, the routine will return the number of characters to skip (overriding the number calculated during the matching).
        size_t matchLength = 1;

        // zero-based position in tablei.
        // if the corresponding translation item in tableo is a string then the first occurence is always applied.
        // if the corresponding translation item in tableo is a routine that is not applicable then try the next occurence
        // until a translation item is applicable, or until there are no more occurences.
        size_t position=0;
        bool translated = false;
        do {
            matchLength = 1; // reset, we try a new occurence

            // Step 1: Matching
            if (inStringData != OREF_NULL)
            {
                // case tablei is a string
                char ch = *scanPtr;

                // if we have a real input table, then scan for the
                // character.
                if (inString != GlobalNames::NULLSTRING)
                {
                    // search for the character
                    size_t previousPosition = position;
                    position = StringUtil::memPos(inStringData + previousPosition, inTableLength - position, ch);
                    // if found then the returned position is relative to inStringData + previousPosition, make it relative to inStringData
                    if (position != (size_t)(-1))
                    {
                        position += previousPosition;
                    }
                }
                // the position is the character itself
                else
                {
                    // no input table, so the position is the character itself
                    position = ((size_t)ch) & 0xff;
                }
            }
            else
            {
                // case tablei is an array (of strings or routines)
                bool match = false;
                RexxObject *item;
                for ( ; position < inTableLength; position++)
                {
                    item = (RexxObject *)inArray->get(position + 1);
                    if (item == OREF_NULL || item == TheNilObject) continue; // ignorable items
                    else if (item->isInstanceOf(TheRoutineClass))
                    {
                        // call (routine) this, pos
                        // if var("RESULT"), result \== 0 then matchLength = result
                        RoutineClass *routine = (RoutineClass *)item;
                        const size_t argumentsCount = 2; // not sure I can use std::size(arguments) (C++17)
                        RexxObject *arguments[argumentsCount];
                        arguments[0] = this;
                        Protected<RexxInteger> rexxPos = new_integer(pos);
                        arguments[1] = rexxPos;
                        ProtectedObject result;
                        routine->call(ActivityManager::currentActivity, routine->getName(), arguments, argumentsCount, result);
                        if (result == OREF_NULL)
                        {
                            // The routine did not return a result. That stops the translation.
                            stopped = true;
                            break;
                        }
                        wholenumber_t value = 0;
                        if (!result->numberValue(value, Numerics::DEFAULT_DIGITS))
                        {
                            value = 0;
                        }
                        // if result is not a number or result == 0 then the routine did not match
                        // otherwise we have match
                        if (value > 0)
                        {
                            match = true;
                            matchLength = value; // will skip all the characters matched by the routine
                            break; // break now, before position is incremented
                        }
                    }
                    else
                    {
                        RexxString *string = (RexxString *)item; // requestString already done
                        match = this->primitiveMatch(pos, string, 1, string->getLength());
                        if (match)
                        {
                            matchLength = string->getLength(); // will skip all the matched characters
                            break; // break now, before position is incremented
                        }
                    }
                }
                if (!match)
                {
                    position = (size_t)(-1);
                }
            }

            if (stopped) break;

            // Step 2: Translation
            if (position != (size_t)(-1))
            {
                // if the output table is large enough, use the table value, otherwise
                // use the pad character.
                if (position < outTableLength)
                {
                    if (outStringData != OREF_NULL)
                    {
                        // case tableo is a string
                        destination->append( *(outStringData + position) );
                        translated = true;
                    }
                    else
                    {
                        // case tableo is an array (of strings or routines)
                        RexxObject *item = (RexxObject *)outArray->get(position + 1);
                        if (item == OREF_NULL || item == TheNilObject)
                        {
                            // an omitted item or .NIL means stop translation
                            stopped = true;
                            break;
                        }
                        else if (item->isInstanceOf(TheRoutineClass))
                        {
                            // call (routine) destination, this, pad, pos, matchLength
                            // if var("RESULT"), result \== 0 then matchLength = result
                            RoutineClass *routine = (RoutineClass *)item;
                            const size_t argumentsCount = 5; // not sure I can use std::size(arguments) (C++17)
                            RexxObject *arguments[argumentsCount];
                            arguments[0] = destination;
                            arguments[1] = this;
                            arguments[2] = pad; // can be OREF_NULL
                            Protected<RexxInteger> rexxPos = new_integer(pos);
                            arguments[3] = rexxPos;
                            Protected<RexxInteger> rexxMatchLength = new_integer(matchLength);
                            arguments[4] = rexxMatchLength;
                            ProtectedObject result;
                            routine->call(ActivityManager::currentActivity, routine->getName(), arguments, argumentsCount, result);
                            if (result == OREF_NULL)
                            {
                                // The routine did not return a result. That stops the translation.
                                stopped = true;
                                break;
                            }
                            wholenumber_t value = 0;
                            if (!result->numberValue(value, Numerics::DEFAULT_DIGITS))
                            {
                                value = 0;
                            }
                            if (value == 0)
                            {
                                // if result is not a number or result == 0 then the routine was not applicable
                                // will try the next occurence in tablei, if any
                                position++;
                            }
                            else
                            {
                                translated = true;
                                matchLength = value;
                            }
                        }
                        else
                        {
                            // The translation item is a string
                            RexxString *string = (RexxString *)item; // requestString already done
                            destination->append(string);
                            translated = true;
                        }
                    }
                }
                else
                {
                    // No corresponding translation item in tableo
                    destination->append( padChar );
                    translated = true;
                }
            }
            else
            {
                // no match in tablei, the translation is the current character, unchanged
                destination->append(*scanPtr);
                translated = true;
            }
        } while (!translated && position < inTableLength && !stopped);

        if (stopped) break;

        pos += matchLength;
        scanPtr += matchLength;
        if (scanLength < matchLength) break;
        scanLength -= matchLength;
    }

    if (!stopped)
    {
        // copy the remaining characters
        size_t from = startPos + range; // one-based
        if (from <= this->getLength())
        {
            size_t length = this->getLength() - from + 1;
            destination->append(this->getStringData() + from - 1, length);
        }
    }

    // returns the current position in the receiving string via a variable reference
    if (refEnd != OREF_NULL)
    {
        Protected<RexxInteger> rexxPos = new_integer(pos);
        refEnd->setValue(rexxPos);
    }

    return destination;
}


/**
 * Perform a verify operation on a string object.
 *
 * @param ref    The reference set for the operation.
 * @param option The match/nomatch option.
 * @param _start The starting position.
 * @param range  The search range.
 *
 * @return The match/nomatch position, or zero.
 */
RexxInteger *RexxString::verify(RexxString *ref, RexxString *option, RexxInteger *_start, RexxInteger *range)
{
    return StringUtil::verify(getStringData(), getLength(), ref, option, _start, range);
}


/**
 * Test if regions within two strings match.
 *
 * @param start_  The starting compare position within the target string.  This
 *                must be within the bounds of the string.
 * @param other   The other compare string.
 * @param offset_ The starting offset of the compare string.  This must be
 *                within the string bounds.  The default start postion is 1.
 * @param len_    The length of the compare substring.  The length and the
 *                offset must specify a valid substring of other.  If not
 *                specified, this defaults to the substring from the
 *                offset to the end of the string.
 *
 * @return True if the two regions match, false for any mismatch.
 */
RexxObject *RexxString::match(RexxInteger *start_, RexxString *other, RexxInteger *offset_, RexxInteger *len_)
{
    size_t _start = positionArgument(start_, ARG_ONE);
    // the start position must be within the string bounds for any match
    // to be performed.
    if (_start > getLength())
    {
        return TheFalseObject;
    }
    other = stringArgument(other, ARG_TWO);

    size_t offset = optionalPositionArgument(offset_, 1, ARG_THREE);

    // other offset/positional problems are also always a false result
    if (offset_ != OREF_NULL && offset > other->getLength())
    {
        return TheFalseObject;
    }

    size_t len = optionalLengthArgument(len_, other->getLength() - offset + 1, ARG_FOUR);

    if ((offset + len - 1) > other->getLength())
    {
        return TheFalseObject;
    }

    return booleanObject(primitiveMatch(_start, other, offset, len));
}


/**
 * Test if regions within two strings match.
 *
 * @param start_  The starting compare position within the target string.  This
 *                must be within the bounds of the string.
 * @param other   The other compare string.
 * @param offset_ The starting offset of the compare string.  This must be
 *                within the string bounds.  The default start postion is 1.
 * @param len_    The length of the compare substring.  The length and the
 *                offset must specify a valid substring of other.  If not
 *                specified, this defaults to the substring from the
 *                offset to the end of the string.
 *
 * @return True if the two regions match, false for any mismatch.
 */
RexxObject *RexxString::caselessMatch(RexxInteger *start_, RexxString *other, RexxInteger *offset_, RexxInteger *len_)
{
    size_t _start = positionArgument(start_, ARG_ONE);
    // the start position must be within the string bounds for any match
    // to be performed.
    if (_start > getLength())
    {
        return TheFalseObject;
    }
    other = stringArgument(other, ARG_TWO);

    size_t offset = optionalPositionArgument(offset_, 1, ARG_THREE);

    // other offset/positional problems are also always a false result
    if (offset_ != OREF_NULL && offset > other->getLength())
    {
        return TheFalseObject;
    }

    size_t len = optionalLengthArgument(len_, other->getLength() - offset + 1, ARG_FOUR);

    if ((offset + len - 1) > other->getLength())
    {
        return TheFalseObject;
    }

    return booleanObject(primitiveCaselessMatch(_start, other, offset, len));
}


/**
 * Test if a string starts with another string
 *
 * @param other  The other compare string.
 *
 * @return True if the string starts with the other, false otherwise.
 */
RexxObject *RexxString::startsWithRexx(RexxString *other)
{
    other = stringArgument(other, "match");
    return booleanObject(primitiveMatch(1, other, 1, other->getLength()));
}


/**
 * Test if a string starts with another string
 *
 * @param other  The other compare string.
 *
 * @return True if the string starts with the other, false otherwise.
 */
RexxObject *RexxString::caselessStartsWithRexx(RexxString *other)
{
    other = stringArgument(other, "match");
    return booleanObject(primitiveCaselessMatch(1, other, 1, other->getLength()));
}


/**
 * Test if a string ends with another string
 *
 * @param other  The other compare string.
 *
 * @return True if the string starts with the other, false otherwise.
 */
RexxObject *RexxString::endsWithRexx(RexxString *other)
{
    other = stringArgument(other, "match");

    // we need to check this here, because the calculated offset might be wrong
    if (other->getLength() > getLength())
    {
        return TheFalseObject;
    }

    return booleanObject(primitiveMatch(getLength() - other->getLength() + 1, other, 1, other->getLength()));
}


/**
 * Test if a string ends with another string
 *
 * @param other  The other compare string.
 *
 * @return True if the string starts with the other, false otherwise.
 */
RexxObject *RexxString::caselessEndsWithRexx(RexxString *other)
{
    other = stringArgument(other, "match");

    // we need to check this here, because the calculated offset might be wrong
    if (other->getLength() > getLength())
    {
        return TheFalseObject;
    }

    return booleanObject(primitiveCaselessMatch(getLength() - other->getLength() + 1, other, 1, other->getLength()));
}


/**
 * Perform a compare of regions of two string objects.  Returns
 * true if the two regions match, returns false for mismatches.
 *
 * @param start  The starting offset within the target string.
 * @param other  The source string for the compare.
 * @param offset The offset of the substring of the other string to use.
 * @param len    The length of the substring to compare.
 *
 * @return True if the regions match, false otherwise.
 */
bool RexxString::primitiveMatch(size_t _start, RexxString *other, size_t offset, size_t len)
{
    _start--;      // make the starting point origin zero
    offset--;

    // if the match is not possible in the target string, just return false now.
    if ((_start + len) > getLength() || len == 0)
    {
        return false;
    }

    return memcmp(getStringData() + _start, other->getStringData() + offset, len) == 0;
}


/**
 * Perform a caselesee compare of regions of two string objects.
 * Returns true if the two regions match, returns false for
 * mismatches.
 *
 * @param start  The starting offset within the target string.
 * @param other  The source string for the compare.
 * @param offset The offset of the substring of the other string to use.
 * @param len    The length of the substring to compare.
 *
 * @return True if the regions match, false otherwise.
 */
bool RexxString::primitiveCaselessMatch(size_t _start, RexxString *other, size_t offset, size_t len)
{
    _start--;      // make the starting point origin zero
    offset--;

    // if the match is not possible in the target string, just return false now.
    if ((_start + len) > getLength() || len == 0)
    {
        return false;
    }

    return StringUtil::caselessCompare(getStringData() + _start, other->getStringData() + offset, len) == 0;
}


/**
 * Compare a single character at a give position against
 * a set of characters to see if any of the characters is
 * a match.
 *
 * @param position_ The character position
 * @param matchSet  The set to compare against.
 *
 * @return true if the character at the give position is any of the characters,
 *         false if none of them match.
 */
RexxObject *RexxString::matchChar(RexxInteger *position_, RexxString *matchSet)
{
    size_t position = positionArgument(position_, ARG_ONE);
    // the start position must be within the string bounds for any match
    // to be performed.
    if (position > getLength())
    {
        return TheFalseObject;
    }
    matchSet = stringArgument(matchSet, ARG_TWO);

    size_t _setLength = matchSet->getLength();
    char         _matchChar = getChar(position - 1);

    // iterate through the match set looking for a match
    for (size_t i = 0; i < _setLength; i++)
    {
        if (_matchChar == matchSet->getChar(i))
        {
            return TheTrueObject;
        }
    }
    return TheFalseObject;
}


/**
 * Compare a single character at a give position against
 * a set of characters to see if any of the characters is
 * a match.
 *
 * @param position_ The character position
 * @param matchSet  The set to compare against.
 *
 * @return true if the character at the give position is any of the characters,
 *         false if none of them match.
 */
RexxObject *RexxString::caselessMatchChar(RexxInteger *position_, RexxString *matchSet)
{
    size_t position = positionArgument(position_, ARG_ONE);
    // the start position must be within the string bounds for any match
    // to be performed.
    if (position > getLength())
    {
        return TheFalseObject;
    }
    matchSet = stringArgument(matchSet, ARG_TWO);

    size_t _setLength = matchSet->getLength();
    char         _matchChar = getChar(position - 1);
    _matchChar = Utilities::toUpper(_matchChar);

    // iterate through the match set looking for a match, using a
    // caseless compare
    for (size_t i = 0; i < _setLength; i++)
    {
        if (_matchChar == Utilities::toUpper(matchSet->getChar(i)))
        {
            return TheTrueObject;
        }
    }
    return TheFalseObject;
}


/**
 * Do a sorting comparison of two strings.
 *
 * @param other  The other compare string.
 * @param start_ The starting compare position within the target string.
 * @param len_   The length of the compare substring.
 *
 * @return True if the two regions match, false for any mismatch.
 */
RexxInteger *RexxString::compareToRexx(RexxString *other, RexxInteger *start_, RexxInteger *len_)
{
    other = stringArgument(other, ARG_ONE);

    size_t _start = optionalPositionArgument(start_, 1, ARG_TWO);
    size_t len = optionalLengthArgument(len_, std::max(getLength(), other->getLength()) - _start + 1, ARG_THREE);

    return new_integer(primitiveCompareTo(other, _start, len));
}


/**
 * Perform a compare of regions of two string objects.  Returns
 * -1, 0, 1 based on the relative ordering of the two strings.
 *
 * @param other  The source string for the compare.
 * @param start  The starting offset within the target string.
 * @param len    The length of the substring to compare.
 *
 * @return -1 if the target string is less than, 0 if the two strings are
 *         equal, 1 if the target string is the greater.
 */
wholenumber_t RexxString::primitiveCompareTo(RexxString *other, size_t _start, size_t len)
{
    size_t myLength = getLength();
    size_t otherLength = other->getLength();

    // if doing the compare outside of the string length, we're less than the other string
    // unless the start is
    if (_start > myLength)
    {
        return _start > otherLength ? 0 : -1;
    }
    // if beyond the other length, they we're the larger
    if (_start > otherLength)
    {
        return 1;
    }

    _start--;      // make the starting point origin zero

    myLength = std::min(len, myLength - _start);
    otherLength = std::min(len, otherLength - _start);

    len = std::min(myLength, otherLength);

    wholenumber_t result = memcmp(getStringData() + _start, other->getStringData() + _start, len);

    // if they compare equal, then they are only
    if (result == 0)
    {
        if (myLength == otherLength)
        {
            return 0;
        }
        else if (myLength > otherLength)
        {
            return 1;
        }
        else
        {
            return -1;
        }
    }
    else if (result > 0)
    {
        return 1;
    }
    else
    {
        return -1;
    }
}


/**
 * Perform a compare of regions of two string objects.  Returns
 * -1, 0, 1 based on the relative ordering of the two strings.
 *
 * @param other  The source string for the compare.
 *
 * @return -1 if the target string is less than, 0 if the two strings are
 *         equal, 1 if the target string is the greater.
 */
wholenumber_t RexxString::primitiveCompareTo(RexxString *other)
{
    size_t myLength = getLength();
    size_t otherLength = other->getLength();

    size_t len = std::min(getLength(), other->getLength());

    wholenumber_t result = memcmp(getStringData(), other->getStringData(), len);

    // if they compare equal, then they are only
    if (result == 0)
    {
        if (myLength == otherLength)
        {
            return 0;
        }
        else if (myLength > otherLength)
        {
            return 1;
        }
        else
        {
            return -1;
        }
    }
    else if (result > 0)
    {
        return 1;
    }
    else
    {
        return -1;
    }
}


/**
 * Do a sorting comparison of two strings.
 *
 * @param other  The other compare string.
 * @param start_ The starting compare position within the target string.
 * @param len_   The length of the compare substring.
 *
 * @return True if the two regions match, false for any mismatch.
 */
RexxInteger *RexxString::caselessCompareToRexx(RexxString *other, RexxInteger *start_, RexxInteger *len_)
{
    other = stringArgument(other, ARG_ONE);

    size_t _start = optionalPositionArgument(start_, 1, ARG_TWO);
    size_t len = optionalLengthArgument(len_, std::max(getLength(), other->getLength()) - _start + 1, ARG_THREE);

    return new_integer(primitiveCaselessCompareTo(other, _start, len));
}




/**
 * Perform a compare of regions of two string objects.  Returns
 * -1, 0, 1 based on the relative ordering of the two strings.
 *
 * @param other  The source string for the compare.
 * @param start  The starting offset within the target string.
 * @param len    The length of the substring to compare.
 *
 * @return -1 if the target string is less than, 0 if the two strings are
 *         equal, 1 if the target string is the greater.
 */
wholenumber_t RexxString::primitiveCaselessCompareTo(RexxString *other, size_t _start, size_t len)
{
    size_t myLength = getLength();
    size_t otherLength = other->getLength();

    // if doing the compare outside of the string length, we're less than the other string
    // unless the start is
    if (_start > myLength)
    {
        return _start > otherLength ? 0 : -1;
    }
    // if beyond the other length, they we're the larger
    if (_start > otherLength)
    {
        return 1;
    }

    _start--;      // make the starting point origin zero

    myLength = std::min(len, myLength - _start);
    otherLength = std::min(len, otherLength - _start);

    len = std::min(myLength, otherLength);

    wholenumber_t result = StringUtil::caselessCompare(getStringData() + _start, other->getStringData() + _start, len);

    // if they compare equal, then they are only
    if (result == 0)
    {
        if (myLength == otherLength)
        {
            return 0;
        }
        else if (myLength > otherLength)
        {
            return 1;
        }
        else
        {
            return -1;
        }
    }
    else if (result > 0)
    {
        return 1;
    }
    else
    {
        return -1;
    }
}
