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

-- Make this class globally visible without requiring this package.
-- ICU4ooRexx needs this to register itself using registerICU4ooRexx.
.environment["REXXUNICODE"] = .RexxUnicode


-- ::options trace i

/******************************************************************************/
::class "RexxUnicode" public -
                      subclass RexxUnicodeServices -
                      inherit ICU4ooRexxInterface


/*
Native methods of RexxUnicodeServices
    RexxString *utf8procVersion();
    RexxString *unicodeVersion();

    RexxInteger *utf8DecodeCodepoint(RexxString *string, RexxInteger *indexB, VariableReference *refSizeB, VariableReference *refErrorCode, VariableReference *refErrorMsg);
    MutableBuffer *utf8EncodeCodepoint(RexxInteger *rexxCodepoint, MutableBuffer *destination, VariableReference *refSizeB);

    RexxString *utf8Transform(RexxObject **arguments, size_t argCount);
    // use strict arg string, casefold = .false, lump= .false, nlf = 0, normalization = 0, stripCC = .false, stripIgnorable= .false, stripMark = .false, stripNA = .false

    RexxInteger *graphemeBreak(ArrayClass *);

    RexxInteger *codepointBidiClass(RexxInteger *rexxCodepoint, VariableReference *refCode, VariableReference *refLabel); // enum
    RexxInteger *codepointBidiMirrored(RexxInteger *rexxCodepoint); // boolean
    RexxInteger *codepointBoundClass(RexxInteger *rexxCodepoint); // enum
    RexxInteger *codepointCategory(RexxInteger *rexxCodepoint, VariableReference *refCode, VariableReference *refLabel); // enum
    RexxInteger *codepointCharWidth(RexxInteger *rexxCodepoint); // integer
    RexxInteger *codepointCombiningClass(RexxInteger *rexxCodepoint); // integer 0..254
    RexxInteger *codepointControlBoundary(RexxInteger *rexxCodepoint); // boolean
    RexxInteger *codepointDecompositionType(RexxInteger *rexxCodepoint); // enum
    RexxInteger *codepointIgnorable(RexxInteger *rexxCodepoint); // boolean

    RexxInteger *codepointIsLower(RexxInteger *rexxCodepoint);
    RexxInteger *codepointIsUpper(RexxInteger *rexxCodepoint);
    RexxInteger *codepointToLower(RexxInteger *rexxCodepoint);
    RexxInteger *codepointToUpper(RexxInteger *rexxCodepoint);
    RexxInteger *codepointToTitle(RexxInteger *rexxCodepoint);
*/

/*******************************************************************************
Constants
*******************************************************************************/

::constant maxCodepoint 1114111

-- Code to pass to utf8Transform as the "normalization" argument:
::constant NFC 1
::constant NFD 2
::constant NFKC 3
::constant NFKD 4


::method info class
    messages = ,
    "ICU4ooRexxIsRegistered",,
    "maxCodepoint",,
    "systemIsLittleEndian",,
    "unicodeVersion",,
    "utf8procVersion"
    info = .StringTable~new
    do message over messages
        value = self~send(message)
        info~put(value, message)
    end

    return info


/*******************************************************************************
Character by name or by codepoint
*******************************************************************************/

-- For convenience
-- .RexxUnicode[key] is equivalent to .RexxUnicode~character(key)
::method "[]" class
    forward message "character"


-- Return an instance of RexxUnicodeCharacter
::method character class
    use strict arg name, -
                   by="DFHLNU", hexadecimal=.false -- can't be guessed: 100 in hexadecimal is different from 100 in decimal

    name = name~strip
    if name == "" then return self~characterByCodepoint(-1)

    -- is name an hexadecimal codepoint ?
    if by~caselessContains("H"), hexadecimal then do
        call remove_leading_zeros
        if name~length <= 6, datatype(name, "X") then do
            return self~characterByCodepoint(name~x2d)
        end
        signal invalid_hexadecimal_codepoint
    end

    -- is name a decimal codepoint ?
    if by~caselessContains("D"), datatype(name, "W") then do
        call remove_leading_zeros
        if name~length <= 7 then return self~characterByCodepoint(name)
        signal invalid_decimal_codepoint
    end

    -- is name a U+ codepoint ?
    if by~caselessContains("U"), name~caselessStartsWith("U+") then do
        name = name~substr(3)
        if name~length >= 4, name~length <= 6, datatype(name, "X") then do
            return self~characterByCodepoint(name~x2d)
        end
        signal invalid_u_plus
    end

    -- is name a character name?
    if by~caselessContains("N"), self~ICU4ooRexxIsRegistered then do
        looseMatchingIndex = name -- self~UAX44_LM2(name) returns a string that is NOT supported by ICU4C!
        codepoint = .ICU4ooRexx~u_charFromName(looseMatchingIndex, .ICU4ooRexx~U_UNICODE_CHAR_NAME)
        if codepoint == -1 then codepoint = .ICU4ooRexx~u_charFromName(looseMatchingIndex, .ICU4ooRexx~U_EXTENDED_CHAR_NAME)
        if codepoint == -1 then codepoint = .ICU4ooRexx~u_charFromName(looseMatchingIndex, .ICU4ooRexx~U_CHAR_NAME_ALIAS)
        if codepoint \== -1 then return self~characterByCodepoint(codepoint)
    end

    -- is name a code point label?
    /*
    code point labels are constructed by using a lowercase prefix derived from the code point type,
    followed by a hyphen-minus and then a 4- to 6-digit hexadecimal representation of the code point.
        Control         control-NNNN
        Reserved        reserved-NNNN
        Noncharacter    noncharacter-NNNN
        Private-Use     private-use-NNNN
        Surrogate       surrogate-NNNN
    U+0008 label is <control-0008>
    */
    if by~caselessContains("L"), name~left(1) == "<", name~right(1) == ">", name~pos(" ") == 0 then do
        hyphenPos = name~lastPos("-")
        if hyphenPos \== 0 then do
            parse var name "<" type =(hyphenPos) "-" hex ">"
            if "control reserved noncharacter private-use surrogate"~wordPos(type) \== 0,,
               hex~length >= 4, hex~length <= 6,,
               hex~datatype("X"),
            then do
                return self~characterByCodepoint(hex~x2d)
            end
        end
    end

    -- fallback: if the name is made of just one codepoint then search this codepoint
    if by~caselessContains("F") then do
        self~createIndexers(name, >c, >g, >e)
        if c~items == 1 then return self~characterByCodepoint(self~codepointAtIndexC(name, 1, c))
        if g~items == 1 then signal character_made_of_several_codepoints
    end

    if by~caselessContains("N"), \ self~ICU4ooRexxIsRegistered then signal ICU4ooRexx_not_loaded
    signal name_not_found

    remove_leading_zeros:
        firstNonZero = name~verify("0")
        if firstNonZero == 0 then name = "0" -- 0[..0] replaced by 0
        else if firstNonZero > 1 then name = name~substr(firstNonZero) -- remove all the leading zeros
        return

    invalid_hexadecimal_codepoint:          raise syntax 93.900 array("an hexadecimal codepoint is 1..8 hex digits, got '"name"'")
    invalid_decimal_codepoint:              raise syntax 93.900 array("a decimal codepoint is 1..7 digits, got '"name"'")
    invalid_u_plus:                         raise syntax 93.900 array("Expected U+ or u+ followed by 4..6 hex digits, got '"name"'")
    character_made_of_several_codepoints:   raise syntax 93.900 array("The character '"name"' is made of several codepoints:" self~c2x(name))
    ICU4ooRexx_not_loaded:                  raise syntax 93.900 array("Name '"name"' not found (ICU4ooRexx not loaded)")
    name_not_found:                         raise syntax 93.900 array("Name '"name"' not found")


-- Return an instance of RexxUnicodeCharacter
::method characterByCodepoint class
    use strict arg codepoint
    self~checkCodepoint(codepoint)
    return .RexxUnicodeCharacter~new(codepoint)


::method checkCodepoint class
    use strict arg codepoint, minusOneAllowed=.false
    .Validate~wholeNumber("codepoint", codepoint)
    if codepoint < minusOneAllowed~?(-1, 0) | codepoint > self~maxCodepoint then signal error_range
    return .true

    error_range:
    -- To be consistent with other checks, say the lower limit is 0, even if -1 is an allowed value
    raise syntax 23.900 array ("Invalid codepoint" codepoint "("self~D2U(codepoint)"). Allowed range is 0.."self~maxCodepoint "(0.."self~D2U(self~maxCodepoint)")")


/*******************************************************************************
C2U C2X D2U U2D X2U
*******************************************************************************/

-- Return a sequence of U+XXXX... as string
::method C2U class
    use strict arg string
    codepoints = .CodepointSupplier~new(string)
    buffer = .MutableBuffer~new
    first = .true
    do while codepoints~available
        if \first then buffer~append(" ")
        first = .false
        codepoint = codepoints~item("c") -- "c": get the codepoint as integer
        UPlusCodepoint = self~D2U(codepoint)
        buffer~append(UPlusCodepoint)
        codepoints~next
    end
    return buffer~string


-- Specialized C2X: Insert a space between the codepoints
::method C2X class
    use strict arg string
    codepoints = .CodepointSupplier~new(string)
    buffer = .MutableBuffer~new
    first = .true
    do while codepoints~available
        if \first then buffer~append(" ")
        first = .false
        codepoint = codepoints~item("b") -- "b": get the byte sequence
        buffer~append(codepoint~c2x)
        codepoints~next
    end
    return buffer~string


/*
Appendix A - Notational conventions
In running text, an individual Unicode code point is expressed as U+n, where n is four to
six hexadecimal digits, using the digits 0–9 and uppercase letters A–F (for 10 through 15,
respectively). Leading zeros are omitted, unless the code point would have fewer than four
hexadecimal digits—for example, U+0001, U+0012, U+0123, U+1234, U+12345, U+102345
*/
::method D2U class
    use strict arg codepoint
    .Validate~wholeNumber("codepoint", codepoint)
    if codepoint == -1 then return "U+FFFFFF" -- in case of error (invalid)
    if codepoint < 65536 then return "U+" || codepoint~d2x(4)
    return "U+" || codepoint~d2x


::method U2D class
    use strict arg UPlusCodepoint -- "U+XXXX..."
    if UPlusCodepoint~startsWith("U+") then return UPlusCodepoint~substr(3)~x2d
    return -1


::method X2U class
    use strict arg hexadecimalCP
    codepoint = hexadecimalCP~x2d
    return self~D2U(codepoint)


/*******************************************************************************
Loose matching
*******************************************************************************/

/*
UAX44-LM2.
https://unicode.org/reports/tr44/#UAX44-LM2
*/
::method UAX44_LM2 class
    use strict arg string
    buffer = .MutableBuffer~new(string)
    /*
    1. remove all medial hyphens (except the medial hyphen in the name for U+1180
    HANGUL JUNGSEONG O-E).
    In this rule "medial hyphen" is to be construed as a hyphen occurring immediately
    between two letters in the normative Unicode character name, as published in
    the Unicode names list, and not to any hyphen that may transiently occur medially
    as a result of removing whitespace before removing hyphens in a particular
    implementation of matching.
    Thus the hyphen in the name U+10089 LINEAR B IDEOGRAM B107M HE-GOAT is medial,
    and should be ignored in loose matching,
    but the hyphen in the name U+0F39 TIBETAN MARK TSA -PHRU is not medial, and
    should not be ignored in loose matching.
    */
    buffer~space
    pos = 2 -- yes, 2, not 1
    do forever
        pos = buffer~verify("-", "M", pos)
        if pos == 0 then leave
        if buffer~subchar(pos-1)~verify(.string~alpha) == 0, buffer~subchar(pos+1)~verify(.string~alpha) == 0 then do
            if buffer~subchar(pos+2) == "", buffer~string~caselessEquals("HANGUL JUNGSEONG O-E") then leave
            buffer~replaceAt(" ", pos, 1)
        end
        else pos += 1 -- keep the hyphen. Ex: "TIBETAN MARK GTER YIG MGO -UM RNAM BCAD MA"
    end

    /*
    2. remove all whitespace and underscore characters
    */
    buffer~translate("", "_")
    buffer~space(0)

    /*
    3.apply toLowercase() to both strings
    */
    buffer~lower

    return buffer~string


/*
UAX44-LM3
https://unicode.org/reports/tr44/#UAX44-LM3
*/
::method UAX44_LM3 class
    use strict arg string
    buffer = .MutableBuffer~new(string)
    /*
    Ignore case, whitespace, underscore ('_'), hyphens, and any initial prefix string "is".

    "linebreak" is equivalent to "Line_Break" or "Line-break"
    "lb=BA" is equivalent to "lb=ba" or "LB=BA"
    "Script=Greek" is equivalent to "Script=isGreek" or "Script=Is_Greek"
    */
    buffer~translate("", "_-")
    buffer~space(0)
    buffer~lower
    if buffer~pos("is", 1, 2) == 1 then buffer~delete(1, 2)
    return buffer~string


/*******************************************************************************
Direct access to codepoints and graphemes
*******************************************************************************/

-- Temporary implementation, to be replaced by a native indexer
::method createIndexers class
    use strict arg string, >codepointIndexes, >graphemeIndexes, >errors
    string = string~string
    codepointIndexes = .array~new
    graphemeIndexes = .array~new
    errors = .array~new
    indexB = 1
    previousCodepoint = -1
    previousCodepointIndexB = -1
    graphemeBreakArgs = (-1, -1, 0) -- codepoint1, codepoint2, state. Will use the same array at each iteration.

    forever:
        codepoint = self~utf8DecodeCodepoint(string, indexB, >sizeB, >errorCode, >errorMsg)
        if sizeB == 0 then return -- End of string
        if sizeB < 0 then signal error

        codepointIndexes~append(indexB)
        if previousCodepoint < 0 then graphemeIndexes~append(indexB) -- First codepoint or Error recovery
        else do
            graphemeBreakArgs[1] = previousCodepoint
            graphemeBreakArgs[2] = codepoint
            if self~graphemeBreak(graphemeBreakArgs) then graphemeIndexes~append(indexB)
        end
        previousCodepoint = codepoint
        previousCodepointIndexB = indexB
        indexB += sizeB
    signal forever

    error:
        errors~append("start byte-position" indexB ":" errorMsg)
        codepointIndexes~append(-indexB) -- a negative index means "error"
        graphemeIndexes~append(-indexB) -- idem
        graphemeBreakArgs[3] = 0 -- reset the extended grapheme state
        previousCodepoint = codepoint
        previousCodepointIndexB = indexB
        indexB += abs(sizeB) -- skip the invalid bytes
        signal forever


-- Convert a codepoint index into a byte index
::method codepointIndexB class
    use strict arg string, indexC, codepointIndexes
    if indexC < 1 then raise syntax 23.900 array (self~description "invalid codepoint index:" indexC)
    if indexC > codepointIndexes~items then return string~length + 1
    return codepointIndexes[indexC]


-- Return the codepoint at position indexC
::method codepointAtIndexC class
    use strict arg string, indexC, codepointIndexes
    string = string~string
    startB = self~codepointIndexB(string, indexC, codepointIndexes)
    absStartB = abs(startB) -- startB can be negative (meaning current byte sequence is invalid)

    if startB < 0 then return "FFFD"~x2d -- the current byte sequence is invalid, return the replacement character
    return self~utf8DecodeCodepoint(string, absStartB)


-- Convert a grapheme index into a byte index
::method graphemeIndexB class
    use strict arg string, indexC, graphemeIndexes
    if indexC < 1 then raise syntax 23.900 array (self~description "invalid grapheme index:" indexC)
    if indexC > graphemeIndexes~items then return string~length + 1
    return graphemeIndexes[indexC]


-- Return the grapheme at position indexC
::method graphemeAtIndexC class
    use strict arg string, indexC, graphemeIndexes
    startB = self~graphemeIndexB(string, indexC, graphemeIndexes)
    nextB = self~graphemeIndexB(string, indexC+1, graphemeIndexes)
    absStartB = abs(startB) -- startB can be negative (meaning current byte sequence is invalid)
    absNextB = abs(nextB) -- nextB can be negative (meaning next byte sequence is invalid)
    return string~substr(absStartB, absNextB - absStartB) -- yes, always the current byte sequence, even if invalid


/******************************************************************************/
::class "RexxUnicodeCharacter" public inherit Comparable
/******************************************************************************/

::attribute codepoint get
::attribute name get


::method init
    expose codepoint name
    use strict arg codepoint, name=""
    .RexxUnicode~checkCodepoint(codepoint)
    if name == "", .RexxUnicode~ICU4ooRexxIsRegistered then do
        name = .ICU4ooRexx~u_charName(codepoint, .ICU4ooRexx~U_UNICODE_CHAR_NAME)
        if name == "" then .ICU4ooRexx~u_charName(codepoint, .ICU4ooRexx~U_EXTENDED_CHAR_NAME)
        if name == "" then .ICU4ooRexx~u_charName(codepoint, .ICU4ooRexx~U_CHAR_NAME_ALIAS)
    end


::method string
    expose codepoint name
    if codepoint < 32 then string = "" -- the control characters are not displayed
                      else string = self~UTF8
    string = quoted(string) " "~copies(3 - self~charWidth)
    string ||= .RexxUnicode~D2U(codepoint) self~category self~categoryName self~charWidth quoted(name)
    return string


::method UTF8
    expose codepoint
    use strict arg -- none
    return .RexxUnicode~utf8EncodeCodepoint(codepoint, .MutableBuffer~new)~string


-- http://www.unicode.org/reports/tr44/#General_Category_Values
::method category
    expose codepoint
    use strict arg -- none
    .RexxUnicode~codepointCategory(codepoint, >refCode)
    return refCode


-- http://www.unicode.org/reports/tr44/#General_Category_Values
::method categoryName
    expose codepoint
    use strict arg -- none
    .RexxUnicode~codepointCategory(codepoint, , >refLabel)
    return refLabel


-- http://www.unicode.org/reports/tr44/#Canonical_Combining_Class_Values
::method combiningClass
    expose codepoint
    use strict arg -- none
    return .RexxUnicode~codepointCombiningClass(codepoint)


-- http://www.unicode.org/reports/tr44/#Bidi_Class_Values
::method bidiClass
    expose codepoint
    use strict arg -- none
    .RexxUnicode~codepointBidiClass(codepoint, >refCode)
    return refCode


-- http://www.unicode.org/reports/tr44/#Bidi_Class_Values
::method bidiClassName
    expose codepoint
    use strict arg -- none
    .RexxUnicode~codepointBidiClass(codepoint, , >refLabel)
    return refLabel


-- https://unicode.org/reports/tr9/
::method bidiMirrored
    expose codepoint
    use strict arg -- none
    return .RexxUnicode~codepointBidiMirrored(codepoint)


-- https://unicode.org/reports/tr15/
::method decompositionType
    expose codepoint
    use strict arg -- none
    -- TODO enum code
    return .RexxUnicode~codepointDecompositionType(codepoint)


-- https://unicode.org/reports/tr15/
::method decompositionTypeName
    expose codepoint
    use strict arg -- none
    -- TODO enum label
    return "?"


-- http://www.unicode.org/review/pr-5.html
-- http://unicode.org/L2/L2002/02368-default-ignorable.html
::method ignorable
    expose codepoint
    use strict arg -- none
    return .RexxUnicode~codepointIgnorable(codepoint)


-- http://unicode.org/reports/tr29/tr29-6.html
::method controlBoundary
    expose codepoint
    use strict arg -- none
    return .RexxUnicode~codepointControlBoundary(codepoint)


-- Width of the displayed character (not a Unicode property)
::method charWidth
    expose codepoint
    use strict arg -- none
    return .RexxUnicode~codepointCharWidth(codepoint)


-- https://unicode.org/reports/tr29/
::method boundClass
    expose codepoint
    use strict arg -- none
    -- TOTO enum code
    return .RexxUnicode~codepointBoundClass(codepoint)


-- https://unicode.org/reports/tr29/
::method boundClassName
    expose codepoint
    use strict arg -- none
    -- TODO enum label
    return "?"


-- Simple case mappings in UnicodeData.txt, returns a single codepoint
::method toLower
    expose codepoint
    use strict arg -- none
    return .RexxUnicode~codepointToLower(codepoint)


-- Full case mappings in SpecialCasing.txt, returns 1..n codepoints
::method toLowerFull
    expose codepoint
    use strict arg -- none
    -- TODO
    return -1


-- Simple case mappings in UnicodeData.txt, returns a single codepoint
::method toUpper
    expose codepoint
    use strict arg -- none
    return .RexxUnicode~codepointToUpper(codepoint)


-- Full case mappings in SpecialCasing.txt, returns 1..n codepoints
::method toUpperFull
    expose codepoint
    use strict arg -- none
    -- TODO
    return -1


-- Simple case mappings in UnicodeData.txt, returns a single codepoint
::method toTitle
    expose codepoint
    use strict arg -- none
    return .RexxUnicode~codepointToTitle(codepoint)


-- Full case mappings in SpecialCasing.txt, returns 1..n codepoints
::method toTitleFull
    expose codepoint
    use strict arg -- none
    -- TODO
    return -1


::method isLower
    expose codepoint
    use strict arg -- none
    return .RexxUnicode~codepointIsLower(codepoint)


::method isUpper
    expose codepoint
    use strict arg -- none
    return .RexxUnicode~codepointIsUpper(codepoint)


::method properties class
    -- TODO: align with UCD PropertyAliases.txt
    expose properties
    if \ var("properties") then properties =,
        "bidiClass",,
        "bidiClassName",,
        "bidiMirrored",,
        ,, -- "boundClass",,
        ,, -- "boundClassName",,
        "category",,
        "categoryName",,
        "charWidth",,
        "codepoint",,
        "combiningClass",,
        "controlBoundary",,
        ,, -- "decompositionTypeName",,
        ,, -- "decompositionType",,
        "ignorable",,
        "isLower",,
        "isUpper",,
        "name",,
        "toLower",,
        ,, -- "toLowerFull",,
        "toTitle",,
        ,, -- "toTitleFull",,
        "toUpper",,
        ,, -- "toUpperFull",,
        "UTF8"
    return properties


::method propertiesSpecialDisplay class
    expose specialDisplay
    if \ var("specialDisplay") then do
        specialDisplay = .StringTable~new
        specialDisplay["codepoint"] =   "value = .RexxUnicode~C2U(self~UTF8)"
        specialDisplay["toLower"] =     "value = .RexxUnicode~D2U(self~toLower)"
        -- specialDisplay["toLowerFull"=   "value = .RexxUnicode~?2U(self~toLowerFull)"
        specialDisplay["toTitle"] =     "value = .RexxUnicode~D2U(self~toTitle)"
        -- specialDisplay["toTitleFull"] = "value = .RexxUnicode~D2U(self~toTitleFull)"
        specialDisplay["toUpper"] =     "value = .RexxUnicode~D2U(self~toUpper)"
        -- specialDisplay["toUpperFull"] = "value = .RexxUnicode~D2U(self~toUpperFull)"
        specialDisplay["UTF8"] =        "value = '0x'self~UTF8~c2x"
    end
    return specialDisplay


::method info
    properties = .StringTable~new
    do property over self~class~properties
        specialDisplay = self~class~propertiesSpecialDisplay[property]
        if .nil \== specialDisplay then interpret specialDisplay
        else value = self~send(property)
        properties~put(value, property)
    end
    return properties


-- For convenience
-- .RexxUnicodeCharacter[key] is equivalent to .RexxUnicode~character(key)
::method "[]" class
    forward message "character" to (.RexxUnicode)


::method compareTo
    use arg other
    cp1 = self~codepoint
    cp2 = other~codepoint
    if cp1 < cp2 then return -1
    if cp1 > cp2 then return 1
    return 0


/******************************************************************************/
::class "ICU4ooRexxInterface" mixinclass Object private
/******************************************************************************/

::attribute ICU4ooRexxIsRegistered class get
::attribute ICU4ooRexxPackage class get


::method activate class
    expose ICU4ooRexxIsRegistered ICU4ooRexxPackage
    use strict arg -- none
    ICU4ooRexxIsRegistered = .false
    ICU4ooRexxPackage = .nil


-- This method is called when ICU4ooRexx.cls is loaded
::method registerICU4ooRexx class
    expose ICU4ooRexxIsRegistered ICU4ooRexxPackage
    use strict arg package

    .Validate~classType("package", package, .Package)

    -- Sanity checks

    if ICU4ooRexxIsRegistered then do
        if ICU4ooRexxPackage \== package then signal registered_different_package
        return -- already registered
    end

    className = "ICU4ooRexx" -- keep it! used in error message
    TheICU4ooRexxClass = package~findPublicClass(className)
    if TheICU4ooRexxClass~isNil then signal class_not_found

    parse var version major "." minor "." patch .

    versionRequirement = "0.0.1"
    if TheICU4ooRexxClass~majorVersion < 0 then signal version_requirement
    if TheICU4ooRexxClass~minorVersion < 0 then signal version_requirement
    if TheICU4ooRexxClass~patchVersion < 1 then signal version_requirement

    methodName = "u_charName" -- keep it! used in error message
    if \ TheICU4ooRexxClass~hasMethod(methodName) then signal method_not_found

    methodName = "u_charFromName" -- keep it! used in error message
    if \ TheICU4ooRexxClass~hasMethod(methodName) then signal method_not_found

    -- All good, we can register ICU4ooRexx
    ICU4ooRexxIsRegistered = .true
    ICU4ooRexxPackage = package
    self~package~addPackage(package) -- make the ICU4ooRexx classes and routines visible from this package

    return

    registered_different_package: raise syntax 93.900 array("ICU4ooRexx is already registered with a different package")
    class_not_found:              raise syntax 93.900 array("Class" quoted(className) "not found")
    version_requirement:          raise syntax 93.900 array("Minimum ICU4ooRexx version must be" versionRequirement || ", current version is" ICU4ooRexx~version)
    method_not_found:             raise syntax 93.900 array("The ICU4ooRexx class does not understand message" quoted(methodName))


/******************************************************************************/
::class "CodepointSupplier" public subclass Supplier
/******************************************************************************/

::method init
    expose string indexB indexC
    use strict arg string
    empty = .array~new(0) -- No need to create a copy, the string is immutable
    self~init:super(empty, empty)

    /*
    Encountered a strange problem with TUTOR...
    .RexxUnicode~c2x("€") returned 'E282AC2020' when TUTOR was active, and 'E282AC' when TUTOR was not active.
    This problem is not limited to TUTOR; it can occur with any object that overrides ~length and ~substr.
    The root cause lies in the decode method, which calls utf8DecodeCodepoint.
    utf8DecodeCodepoint takes a string as its first argument and, if it is not a String, requests a string representation from the object.
    So far, so good, but...
    utf8DecodeCodepoint returns 3 as the byte count for the requested codepoint (which is correct).
    The decode method uses this byte count to obtain the substring corresponding to the codepoint.
    BOOM!
    In the decode method, the string is not a String; it is a Text object (a TUTOR class).
    Its length is 1 because Text operates on graphemes.
    substr(string, 1, 3) is therefore padded with two spaces...
    Fix applied:
    a) Obtain the string representation of the string argument when initializing the supplier.
    b) In utf8DecodeCodepoint, reject any argument that is not an actual String (subclasses are not accepted).
    */

    string = string~string
    indexB = 1
    indexC = 1
    self~decode


::method decode
    expose string indexB sizeB codepoint byteSequence
    use strict arg -- none
    codepoint = .RexxUnicode~utf8DecodeCodepoint(string, indexB, >sizeB)
    if sizeB == 0 then codepoint = .nil -- end of string
    if sizeB < 0 then codepoint = "FFFD"~x2d -- the current byte sequence is invalid, return the replacement character
    sizeB = abs(sizeB)
    byteSequence = string~substr(indexB, sizeB)


::method available
    expose codepoint
    use strict arg -- none
    return .nil \== codepoint


::method index
    expose indexB indexC
    use strict arg type="c" -- "c" to get the codepoint index, "b" to get the byte index
    if self~available then do
        if type == "c" then return indexC
        if type == "b" then return indexB
    end
    -- error


::method item
    expose codepoint byteSequence
    use strict arg type="c" -- "c" to get the codepoint as integer, "b" to get the byte sequence as string
    if self~available then do
        if type == "c" then return codepoint
        if type == "b" then return byteSequence
    end
    -- error


::method next
    expose indexB sizeB indexC
    use strict arg -- none
    indexB += sizeB
    indexC += 1
    self~decode


/*******************************************************************************
Other helpers
*******************************************************************************/

::routine quoted public
    use strict arg string, quote='"'
    return quote || string~changeStr(quote, quote||quote)  || quote


-- For convenience
-- .RexxUnicode(key) is equivalent to .RexxUnicode[key],
-- which is equivalent to .RexxUnicode~character(key)
::routine .RexxUnicode public
    return .RexxUnicode~sendWith("character", .context~args)


-- For convenience
-- .RexxUnicodeCharacter(key) is equivalent to .RexxUnicodeCharacter[key],
-- which is equivalent to .RexxUnicode~character(key)
::routine .RexxUnicodeCharacter public
    return .RexxUnicode~sendWith("character", .context~args)
