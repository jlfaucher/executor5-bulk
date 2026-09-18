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

/*
Reference documentation (main branch)
https://html-preview.github.io/?url=https://github.com/jlfaucher/executor5-bulk/blob/main/main/trunk/extensions/unicode/rxunicode.html

Latest documentation (main branch)
https://html-preview.github.io/?url=https://github.com/jlfaucher/executor5-bulk/blob/main/main/trunk/extensions/unicode/README.html
*/


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
    RexxInteger *utf8DecodePreviousCodepoint(RexxString *string, RexxInteger *indexB, VariableReference *refSizeB, VariableReference *refErrorCode, VariableReference *refErrorMsg);

    MutableBuffer *utf8EncodeCodepoint(RexxInteger *rexxCodepoint, MutableBuffer *destination, VariableReference *refSizeB);

    RexxObject *utf8StringEscape(RexxString *string, RexxString *escapeBy, MutableBuffer *destination);
    RexxInteger *utf8StringInfo(RexxString *string, VariableReference *refGraphemeCount, VariableReference *refCodepointCount, VariableReference *refErrorCount, RexxInteger *rexxStopAtFirstError);
    RexxObject *utf8StringUnescape(RexxString *string, MutableBuffer *destination);
    RexxInteger *utf8StringWidth(RexxString *string, RexxInteger *indexB, RexxInteger *eastAsianContext);

    RexxString *utf8Transform(RexxObject **arguments, size_t argCount);
    // use strict arg string, casefold = .false, lump= .false, nlf = 0, normalization = 0, stripCC = .false, stripIgnorable= .false, stripMark = .false, stripNA = .false

    RexxInteger *graphemeBreak(RexxInteger *rexxCodepoint1, RexxInteger *rexxCodepoint2, VariableReference *refState);
    RexxInteger *graphemeBreakBackward(RexxString *string, RexxInteger *indexB, RexxInteger *rexxCodepoint1, RexxInteger *rexxCodepoint2);

    RexxInteger *codepointBidiClass(RexxInteger *rexxCodepoint, VariableReference *refCode, VariableReference *refLabel); // enum
    RexxInteger *codepointBidiMirrored(RexxInteger *rexxCodepoint); // boolean
    RexxInteger *codepointBoundClass(RexxInteger *rexxCodepoint, VariableReference *refCode, VariableReference *refLabel); // enum
    RexxInteger *codepointCategory(RexxInteger *rexxCodepoint, VariableReference *refCode, VariableReference *refLabel); // enum
    RexxInteger *codepointCharWidth(RexxInteger *rexxCodepoint); // integer
    // The integer 0..254 returned by codepointCombiningClass is the standard Unicode value
    RexxInteger *codepointCombiningClass(RexxInteger *rexxCodepoint, VariableReference *refCode, VariableReference *refLabel); // integer 0..254, enum
    RexxInteger *codepointControlBoundary(RexxInteger *rexxCodepoint); // boolean
    RexxInteger *codepointDecompositionType(RexxInteger *rexxCodepoint, VariableReference *refCode, VariableReference *refLabel); // enum
    RexxInteger *codepointEastAsianWidthIsAmbiguous(RexxInteger *rexxCodepoint); // boolean
    RexxInteger *codepointIgnorable(RexxInteger *rexxCodepoint); // boolean
    RexxInteger *codepointIndicConjunctBreak(RexxInteger *rexxCodepoint, VariableReference *refCode, VariableReference *refLabel); // enum

    RexxInteger *codepointIsLower(RexxInteger *rexxCodepoint);
    RexxInteger *codepointIsUpper(RexxInteger *rexxCodepoint);
    RexxInteger *codepointToLower(RexxInteger *rexxCodepoint);
    RexxInteger *codepointToUpper(RexxInteger *rexxCodepoint);
    RexxInteger *codepointToTitle(RexxInteger *rexxCodepoint);

    RexxObject *codepointAsciiPrintableString(RexxInteger *rexxCodepoint, MutableBuffer *destination);
    RexxObject *codepointUtf8PrintableString(RexxInteger *rexxCodepoint, MutableBuffer *destination);
    RexxInteger *codepointIsPrintable(RexxInteger *rexxCodepoint);
    RexxObject *codepointUnicodeEscapeNotation(RexxInteger *rexxCodepoint, MutableBuffer *destination);

     // The "utf8String" prefix is omitted from these method names to keep them short.
    RexxObject *C2U(RexxString *utf8String, MutableBuffer *destination);
    RexxObject *C2X(RexxString *utf8String, MutableBuffer *destination);

     // The "asciiString" prefix is omitted from these method names to keep them short.
    RexxObject *U2C(RexxString *asciiStringUPlus, MutableBuffer *destination);
    RexxInteger *U2D(RexxString *asciiStringUPlus, RexxInteger *indexB, RexxInteger *raiseError, VariableReference *refLength);

    // The "codepoint" prefix is omitted from these method names to keep them short.
    RexxObject *D2U(RexxInteger *rexxCodepoint, MutableBuffer *destination);

    // Helper for diagnostics
    RexxObject *sizeofUnicodeWidthTables();
*/

/*******************************************************************************
Constants
*******************************************************************************/

::constant maxCodepoint 1114111 -- ("10ffff"~x2d)

-- Value to pass as the `normalization` argument to utf8Transform (default: 0 no normalization):
::constant NFC 1
::constant NFD 2
::constant NFKC 3
::constant NFKD 4

-- Value to pass as the 'nlf' argument to utf8Transform (default: 0 no transformation):
::constant NLF2LF 1
::constant NLF2LS 2
::constant NLF2PS 3


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
Additional string services
*******************************************************************************/

::method stringInfo class
    if arg() == 2 then use strict arg string, >indexer
                  else use strict arg string
    string = requestBaseString(string)
    if arg() == 2 then indexer = .RexxUnicodeStringIndexer~new(string)          -- no limit, because indexer requested by user
                  else indexer = .RexxUnicodeStringIndexer~new(string, 0, 0, 0) -- no need to store indexes and errors
    return indexer~stringInfo


::method stringIsASCII class
    use strict arg string
    -- not needed: string = requestBaseString(string)
    return string~isASCII


::method stringToNFC class
    use strict arg string, casefold = .false, lump= .false, nlf = 0, stripCC = .false, stripIgnorable= .false, stripMark = .false, stripNA = .false
    string = requestBaseString(string)
    return self~utf8Transform(string, casefold, lump, nlf, self~NFC,  stripCC, stripIgnorable, stripMark, stripNA)


::method stringToNFD class
    use strict arg string, casefold = .false, lump= .false, nlf = 0, stripCC = .false, stripIgnorable= .false, stripMark = .false, stripNA = .false
    string = requestBaseString(string)
    return self~utf8Transform(string, casefold, lump, nlf, self~NFD,  stripCC, stripIgnorable, stripMark, stripNA)


::method stringToNFKC class
    use strict arg string, casefold = .false, lump= .false, nlf = 0, stripCC = .false, stripIgnorable= .false, stripMark = .false, stripNA = .false
    string = requestBaseString(string)
    return self~utf8Transform(string, casefold, lump, nlf, self~NFKC,  stripCC, stripIgnorable, stripMark, stripNA)


/*
NFKC preserves the original case of letters (e.g., 'A' remains 'A').
In contrast, NFKC_CF applies Unicode case-folding rules to map all characters to a case-insensitive form.
    Example: The German sharp s (ß) remains ß in NFKC but maps to ss in NFKC_CF.
    Example: The Greek final sigma (ς) maps to the standard sigma (σ) in NFKC_CF, whereas NFKC leaves it unchanged.
NFKC_CF is specifically optimized for identifier matching.
As part of the "Default Case Folding" algorithm, it removes ignorable characters (such as certain formatting marks or zero-width spaces).
NFKC retains these characters.
*/
::method stringToNFKC_CF class
    use strict arg string, lump= .false, nlf = 0, stripCC = .false, stripMark = .false, stripNA = .false
    string = requestBaseString(string)
    return self~utf8Transform(string, /*casefold*/ .true, lump, nlf, /*normalization*/ self~NFKC,  stripCC, /*stripIgnorable*/ .true, stripMark, stripNA)


::method stringToNFKD class
    use strict arg string, casefold = .false, lump= .false, nlf = 0, stripCC = .false, stripIgnorable= .false, stripMark = .false, stripNA = .false
    string = requestBaseString(string)
    return self~utf8Transform(string, casefold, lump, nlf, self~NFKD,  stripCC, stripIgnorable, stripMark, stripNA)


/*******************************************************************************
Additional codepoint services
*******************************************************************************/

-- Returns .true if codepoint is in the range 0...RexxUnicode~maxCodepoint
-- The range U+D800-U+DFFF is allowed (surrogates).
-- If minusOneAllowed is true then -1 is allowed (special value used in case of error).
::method checkCodepoint class
    use strict arg codepoint, minusOneAllowed=.false
    if \ datatype(codepoint, "W") then signal not_wholenumber
    if codepoint == -1, minusOneAllowed then return .true
    if codepoint < 0 | codepoint > self~maxCodepoint then signal error_range
    return .true

    error_range:
    not_wholenumber: raise syntax 88.900 array ("Code point must be in the range 0 to" self~maxCodepoint "("self~D2U(self~maxCodepoint)"); found" quoted(codepoint))
    -- error_range: raise syntax 88.900 array ("Invalid code point:" codepoint"; allowed range is 0 to" self~maxCodepoint "("self~D2U(self~maxCodepoint)")")


-- Returns the UTF-8 encoding of this codepoint.
::method codepointUTF8Encoding class
    use strict arg codepoint, buffer=.nil
    self~checkCodepoint(codepoint, /*minusOneAllowed*/ .true)
    returnBuffer = .nil <> buffer
    if .nil == buffer then buffer = .MutableBuffer~new
    if codepoint < 0 then nop -- the UTF-8 encoding is ""
    else self~utf8EncodeCodepoint(codepoint, buffer)
    if returnBuffer then return buffer
    return buffer~string


-- Unicode standard name, can be ""
::method codepointCharName class
    use strict arg codepoint, raiseError=.false
    self~checkCodepoint(codepoint, /*minusOneAllowed*/ .true)
    if codepoint < 0 then return ""
    name = ""
    if self~assertICU4ooRexxIsRegistered(raiseError) then do
        name = .ICU4ooRexx~u_charName(codepoint)
    end
    return name


-- All Unicode characters have an extended name.
-- An extended name is either the standard name if defined, or a codepoint label algorithmically generated.
::method codepointExtendedCharName class
    use strict arg codepoint, raiseError=.false
    self~checkCodepoint(codepoint, /*minusOneAllowed*/ .true)
    if codepoint < 0 then return ""
    name = ""
    if self~assertICU4ooRexxIsRegistered(raiseError) then do
        name = .ICU4ooRexx~h_extendedCharName(codepoint)
    end
    return name


-- Unicode name alias, can be "".
::method codepointCharNameAlias class
    use strict arg codepoint, raiseError=.false
    self~checkCodepoint(codepoint, /*minusOneAllowed*/ .true)
    if codepoint < 0 then return ""
    name = ""
    if self~assertICU4ooRexxIsRegistered(raiseError) then do
        name = .ICU4ooRexx~h_charNameAlias(codepoint)
    end
    return name


/*******************************************************************************
Codepoint from identifier
*******************************************************************************/

-- Gets a codepoint from a character name.
-- Private helper for .RexxUnicodeServices~utf8StringUnescape, .RexxUnicode~stringUnescape and .RexxUnicode~codepoint.
-- The native utf8StringUnescape method cannot call a private method when the native method is called outside an ooRexx method context.
-- Workaround: Call the native utf8StringUnescape method from an ooRexx method of RexxUnicode.
::method codepointFromName class private
    use strict arg name
    name = requestBaseString(name, "name")

    codepoint = -1
    if self~ICU4ooRexxIsRegistered, self~stringIsASCII(name) then do
        -- Keep this cascade of search, even if h_charFromLooseName is supposed to cover all possible cases.
        -- Counterexamples where h_charFromLooseName fails to find the codepoint
        -- but h_charFromExtendedName succeeds:
        --   say .RexxUnicode~character("<control-000>")    -- (<?> \x{00} U+0000 Cc Control "<control-0000>")
        --   say .RexxUnicode~character("<control-00>")     -- (<?> \x{00} U+0000 Cc Control "<control-0000>")
        --   say .RexxUnicode~character("<control-0>")      -- (<?> \x{00} U+0000 Cc Control "<control-0000>")
        codepoint = .ICU4ooRexx~h_charFromExtendedName(name) -- The extended name space includes the standard name space
        if codepoint == -1 then codepoint = .ICU4ooRexx~h_charFromNameAlias(name)
        if codepoint == -1 then codepoint = .ICU4ooRexx~h_charFromLooseName(name)
    end
    return codepoint


/*
Gets a codepoint from a codepoint identifier.
A codepoint identifier may be:
- a decimal value (the codepoint itself),
- a U+ notation,
- a character name,
- or a UTF-8 encoded character.
*/
::method codepoint class
    use strict arg identifier
    identifier = requestBaseString(identifier, "identifier")

    -- is identifier a decimal codepoint ?
    if datatype(identifier, "W") then do
        codepoint = identifier
        self~checkCodepoint(codepoint, /*minusOneAllowed*/ .true)
        return codepoint
    end

    -- is identifier a U+ codepoint ?
    if identifier~caselessStartsWith("U+") then do
        codepoint = self~U2D(identifier, /*start*/ 1, /*raiseError*/ .true)
        self~checkCodepoint(codepoint, /*minusOneAllowed*/ .true)
        return codepoint
    end

    -- is identifier a character name?
    codepoint = self~codepointFromName(identifier)
    if codepoint \== -1 then return codepoint

    -- fallback: if the identifier is made of just one codepoint then return this codepoint
    indexer = .RexxUnicodeStringIndexer~new(identifier, /*startCodepointStorageLimit:*/ 1, /*startGraphemeStorageLimit:*/ 0, /*startErrorStorageLimit:*/ 0)
    if indexer~codepointCount == 1 then return indexer~codepointAtIndexC(1)

    additional = ""
    if \ self~ICU4ooRexxIsRegistered then additional = " (ICU4ooRexx not loaded)"
    raise syntax 98.900 array("Name" quoted(identifier) "not found" || additional)


-- Gets a RexxUnicodeCharacter from a codepoint identifier.
-- See the `codepoint` method for a definition of "codepoint identifier".
::method character class
    use strict arg identifier
    identifier = requestBaseString(identifier, "identifier")
    codepoint = self~codepoint(identifier)
    return .RexxUnicodeCharacter~new(codepoint)


/*******************************************************************************
Loose matching
*******************************************************************************/

/*
UAX44-LM2.
https://unicode.org/reports/tr44/#UAX44-LM2
*/
::method UAX44_LM2 class
    use strict arg name
    name = requestBaseString(name, "name")
    if \ self~stringIsASCII(name) then raise syntax 22.900 array("Name must be an ASCII string; found" quoted(name))
    buffer = .MutableBuffer~new(name)
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
UAX44_LM2 also available as a native method on ICU4ooRexx (faster):
.ICU4ooRexx~h_UAX44_LM2
*/
::method h_UAX44_LM2 class
    use strict arg name
    name = requestBaseString(name, "name")
    if self~ICU4ooRexxIsRegistered then forward to .ICU4ooRexx
    forward message "UAX44_LM2"


/*
UAX44-LM3
https://unicode.org/reports/tr44/#UAX44-LM3
*/
::method UAX44_LM3 class
    use strict arg name
    name = requestBaseString(name, "name")
    if \ self~stringIsASCII(name) then raise syntax 22.900 array("Name must be an ASCII string; found" quoted(name))
    buffer = .MutableBuffer~new(name)
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
Escape characters
*******************************************************************************/

/*
Supported escape characters:
    \N{Unicode name}    Character name in the Unicode database
    \u{X..X}            Unicode scalar value, 1-8 hex digits
    \uXXXX              Unicode scalar value, exactly 4 hex digits
    \UXXXXXXXX          Unicode scalar value, exactly 8 hex digits
    \x{X..X}            Arbitrary number of hex digits
    \xXX                1 byte, exactly 2 hex digits

How other languages are supporting escape characters for Unicode:
    \N{Unicode name}    Character name in the Unicode database (Python, Perl, C++23)
    \o{X..X}            Arbitrary number of octal digits (C++23)
    \u{nnnn ...}        One or more Unicode characters, each 1–6 hex digits, space-separated (Ruby)
    \u{X..X}            Unicode scalar value, 1-8 hex digits (C++23, Swift)
    \u{X..X}            Unicode scalar value, 1–6 hex digits (Rust)
    \u{X..X}            Unicode scalar value, 1+ hex digits (beyond \uFFFF) (JavaScript (ES2015+))
    \u{X..X}            hexadecimal Unicode scalar value UTF-8 encoded (1 or more digits) (Zig, PHP 7+)
    \uXXXX              Unicode scalar value, exactly 4 hex digits (C, C++, C#, D, Go, Java, JavaScript, JSON, Julia, Netrexx, Python, Ruby)
    \UXXXXXXXX          Unicode scalar value, exactly 8 hex digits (C, C++, C#, D, Go, Julia, Python)
    \x{X..X}            Arbitrary number of hex digits (C++23, Perl)
    \xXX                1 byte, exactly 2 hex digits (D, Go, JavaScript, Netrexx, Python, Ruby, Zig)
    \xXX                1 byte, exactly 2 hex digits, ASCII range only (0x00–0x7F) (Rust)
    \xX..X              Variable-length hex digits (1–4) (C#)
    \NNN                Character/byte denoted by up to 3 octal digits (terminates early at first non-octal digit) (C, C++, D, Go, Python, Ruby)

    No language lets you write U+XXXX (conventionally 4–6 hex digits) inside a string literal;
    each one wraps that same numeric value in its own escape grammar instead (\u{1F600}, \U0001F600, etc.).

*/

-- Escape characters are normally handled in string literals at parse time.
-- This method handles them at run time instead.
-- Keep this method!
-- It calls the method .RexxUnicodeServices~utf8StringUnescape from the right context,
-- avoiding this error:
-- Object "The RexxUnicode class" cannot accept private message "CODEPOINTFROMNAME" from this context.
::method utf8StringUnescape class
    forward class (super) -- inherited native method


-- Escape characters are normally handled in string literals at parse time.
-- This method handles them at run time instead.
-- This method is kept for compatibility with older versions of `rxunicode.cls`.
::method stringUnescape class
    forward class (super) message "utf8StringUnescape" -- inherited native method


-- This method is kept for compatibility with older versions of `rxunicode.cls`.
::method stringEscape class
    forward message "utf8StringEscape" -- inherited native method


/******************************************************************************/
::class "CodepointSupplierHelper" private mixinclass Supplier
/******************************************************************************/

::method getIndexType class private
    use strict arg type
    if type == "" then return ""
    if type~caselessMatchChar(1, "B") then return type~left(1)~lower
    raise syntax 88.900 array ("Expected an empty index type or one starting with 'B'; found" quoted(type))


::method getItemType class private
    use strict arg type
    if type~isa(.Class) then return type
    if type == "" then return ""
    if type~caselessMatchChar(1, "BEUX\") then return type~left(1)~lower
    raise syntax 88.900 array ("Expected an empty item type or one starting with 'B', 'E', 'U', 'X', or '\', or a Class object; found" quoted(type))


::method setDefaultIndexType
    expose defaultIndexType
    use strict arg indexType
    defaultIndexType = self~class~getIndexType(indexType)


::method setDefaultItemType
    expose defaultItemType
    use strict arg itemType
    defaultItemType = self~class~getItemType(itemType)


::method available
    use strict arg -- none
    return .nil \== self~codepoint


::method index
    expose defaultIndexType
    /*
    Different index types can be returned:
    - If `type`is `""`, the codepoint index is returned (the default index type).
    - If `type` is `"b"`, the byte index is returned (it is negative if the byte sequence is invalid).
    */
    use strict arg type=(defaultIndexType)
    if self~available then do
        type = self~class~getIndexType(type)
        if type == "" then return self~indexC
        if type == "b" then return self~indexB -- can be negative
    end
    else raise syntax 93.937 -- No more supplier items available


::method item
    expose defaultItemType
    /*
    type serves both as:
    - a helper for requesting different codepoint representations,
    - and a workaround for the lack of higher-order functions such as ~each{...} when type is a class.

    Possible values:
    - "" to return the codepoint as a whole number (default)
    - "b" to return the codepoint byte sequence as a string
    - "e" to return the error message or ""
    - "u" to return the codepoint in U+ notation
    - "x" to return the codepoint byte sequence as hexadecimal digits
    - "\" to return the codepoint byte sequence as an escaped string
    - a class to return a new instance of that class, initialized with the result of item("")
    */
    use strict arg type=(defaultItemType)
    if self~available then do
        type = self~class~getItemType(type)
        if type == "" then return self~codepoint
        if type == "b" then return self~byteSequence -- YES! ALWAYS return the raw byte sequence
        if type == "e" then return self~error
        if type == "u" then return .RexxUnicode~D2U(self~codepoint)
        if type == "x" then return escapedHexdigits(self~byteSequence~c2x)
        if type == "\" then return .RexxUnicode~stringEscape(self~byteSequence)
        if type~isa(.Class) then return type~new(self~codepoint)
    end
    else raise syntax 93.937 -- No more supplier items available


-- Returns the codepoint at position indexC (can only advance)
::method codepointAtIndexC
    use strict arg index
    if index < 0 then raise syntax 88.900 array ("Invalid code point index:" index"; negative indexes are not supported")
    if index == 0 then raise syntax 88.900 array ("Invalid code point index:" index)
    if index < self~indexC then raise syntax 88.900 array ("Invalid code point index:" index "is less than the current index" self~indexC "(the supplier can only advance)")
    loop while self~indexC \== index
        self~next
    end
    return self~item


/******************************************************************************/
::class "RexxUnicodeCodepointSupplier" public subclass Supplier -
                                              inherit CodepointSupplierHelper
/******************************************************************************/

-- Codepoint supplier for UTF-8 strings without pre-indexing

-- Using a mixin class requires giving access to these (normally) private variables.
::attribute byteSequence get package    -- a string of length 1..4, or "" if no more codepoint available
::attribute codepoint get package       -- a non-negative whole number, or .nil if no more codepoint available
::attribute error get package           -- "" if no error, otherwise error description
::attribute indexB get package          -- 1..length+1, counted from the beginning of the string, negative if the codepoint byte sequence is invalid
::attribute indexC get package          -- 1-based, counted from the beginning of the string

::method init
    expose string indexB indexC
    use strict arg string, defaultIndexType="", defaultItemType=""
    string = requestBaseString(string, "string")
    self~setDefaultIndexType(defaultIndexType)
    self~setDefaultItemType(defaultItemType)

    empty = .array~new(0) -- No need to create a copy, the string is immutable
    self~init:super(empty, empty)
    indexB = 1
    indexC = 1
    self~decode


::method decode private
    expose string indexB codepoint byteSequence error
    use strict arg -- none
    error = ""
    codepoint = .RexxUnicode~utf8DecodeCodepoint(string, indexB, >sizeB, >errorCode, >errorMsg)
    if sizeB == 0 then do
        codepoint = .nil -- end of string
        byteSequence = ""
    end
    else do
        if sizeB < 0 then do
            error = "start byte position" indexB ":" errorMsg
            codepoint = "FFFD"~x2d -- the current byte sequence is invalid, return the replacement character
            indexB = -indexB -- same convention as .RexxUnicodeStringIndexer; a negative byte index indicates an invalid byte sequence
        end
        byteSequence = string~substr(abs(indexB), abs(sizeB))
    end


::method next
    expose indexB indexC byteSequence
    use strict arg -- none
    if self~available then do
        indexB = abs(indexB)
        indexB += byteSequence~length
        indexC += 1
        self~decode
    end
    else raise syntax 93.937 -- No more supplier items available


/******************************************************************************/
::class "RexxUnicodeReverseCodepointSupplier" public subclass Supplier -
                                                     inherit CodepointSupplierHelper
/******************************************************************************/

-- Reverse codepoint supplier for UTF-8 strings without pre-indexing

-- Using a mixin class requires giving access to these (normally) private variables.
::attribute byteSequence get package    -- a string of length 1..4, or "" if no more codepoint available
::attribute codepoint get package       -- a non-negative whole number, or .nil if no more codepoint available
::attribute error get package           -- "" if no error, otherwise error description
::attribute indexB get package          -- 1..length+1, counted from the beginning of the string, negative if the codepoint byte sequence is invalid
::attribute indexC get package          -- 1-based, counted from the end of the string

::method init
    expose string boundaryIndexB indexB indexC
    use strict arg string, defaultIndexType="", defaultItemType=""
    string = requestBaseString(string, "string")
    self~setDefaultIndexType(defaultIndexType)
    self~setDefaultItemType(defaultItemType)

    empty = .array~new(0) -- No need to create a copy, the string is immutable
    self~init:super(empty, empty)
    boundaryIndexB = string~length + 1 -- The byte index from the begining (1-based) of the boundary before which to decode
    indexB = boundaryIndexB -- indexB from the begining (yes!)
    indexC = 1 -- indexC from the end
    self~decodePrevious


::method decodePrevious private
    expose string boundaryIndexB indexB codepoint byteSequence error
    use strict arg -- none
    error = ""
    codepoint = .RexxUnicode~utf8DecodePreviousCodepoint(string, boundaryIndexB, >sizeB, >errorCode, >errorMsg)
    if sizeB == 0 then do
        codepoint = .nil -- end of string
        byteSequence = ""
    end
    else do
        indexB = abs(indexB) - abs(sizeB)
        if sizeB < 0 then do
            error = "start byte position" indexB ":" errorMsg
            codepoint = "FFFD"~x2d -- the current byte sequence is invalid, return the replacement character
            indexB = -indexB -- same convention as .RexxUnicodeStringIndexer; a negative byte index indicates an invalid byte sequence
        end
        byteSequence = string~substr(abs(indexB), abs(sizeB))
    end


::method next
    expose boundaryIndexB indexB indexC
    use strict arg -- none
    if self~available then do
        -- boundaryIndexB -= byteSequence~length
        boundaryIndexB = abs(indexB) -- move the right edge to the codepoint we just found
        indexC += 1
        self~decodePrevious
    end
    else raise syntax 93.937 -- No more supplier items available


/******************************************************************************/
::class "GraphemeSupplierHelper" private mixinclass Supplier
/******************************************************************************/

::method getIndexType class private
    use strict arg type
    if type == "" then return ""
    if type~caselessMatchChar(1, "B") then return type~left(1)~lower
    raise syntax 88.900 array ("Expected an empty index type or one starting with 'B'; found" quoted(type))


::method getItemType class private
    use strict arg type
    if type~isa(.Class) then return type
    if type == "" then return ""
    if type~caselessMatchChar(1, "BEUX\") then return type~left(1)~lower
    raise syntax 88.900 array ("Expected an empty item type or one starting with 'B', 'E', 'U', 'X', or '\', or a Class object; found" quoted(type))


::method setDefaultIndexType
    expose defaultIndexType
    use strict arg indexType
    defaultIndexType = self~class~getIndexType(indexType)


::method setDefaultItemType
    expose defaultItemType
    use strict arg itemType
    defaultItemType = self~class~getItemType(itemType)


::method available
    use strict arg -- none
    return .nil \== self~grapheme


::method index
    expose defaultIndexType
    /*
    Different index types can be returned:
    - If `type`is `""`, the grapheme index is returned (the default index type).
    - If `type` is `"b"`, the byte index is returned (it is negative if the byte sequence is invalid).
    */
    use strict arg type=(defaultIndexType)
    if self~available then do
        type = self~class~getIndexType(type)
        if type == "" then return self~indexG
        if type == "b" then return self~graphemeIndexB -- (can be negative)
    end
    else raise syntax 93.937 -- No more supplier items available


::method item
    expose defaultItemType
    /*
    type serves both as:
    - a helper for requesting different grapheme representations,
    - and a workaround for the lack of higher-order functions like ~each{...} when type is a class.

    Possible values:
    - "" to return the grapheme as a string (default option)
    - "b" to return the grapheme byte sequence as a string (same as "")
    - "e" to return the error message or ""
    - "u" to return the grapheme codepoints in U+ notation
    - "x" to return the grapheme byte sequence as hexadecimal digits
    - "\" to return the grapheme byte sequence as an escaped string
    - a class to return a new instance of that class, initialized with the result of item("")
    */
    use strict arg type=(defaultItemType)
    if self~available then do
        type = self~class~getItemType(type)
        if type == "" then return self~grapheme
        if type == "b" then return self~grapheme -- YES! ALWAYS return the raw byte sequence
        if type == "e" then return self~error
        if type == "u" then return .RexxUnicode~C2U(self~grapheme)
        if type == "x" then return escapedHexdigits(.RexxUnicode~C2X(self~grapheme))
        if type == "\" then return .RexxUnicode~stringEscape(self~grapheme)
        if type~isa(.Class) then return type~new(self~grapheme)
    end
    else raise syntax 93.937 -- No more supplier items available


-- Returns the grapheme at position indexG (can only advance)
::method graphemeAtIndexG
    use strict arg index
    if index < 0 then raise syntax 88.900 array ("Invalid grapheme index:" index"; negative indexes are not supported")
    if index == 0 then raise syntax 88.900 array ("Invalid grapheme index:" index)
    if index < self~indexG then raise syntax 88.900 array ("Invalid grapheme index:" index "is less than the current index" self~indexG "(the supplier can only advance)")
    loop while self~indexG \== index
        self~next
    end
    return self~item


/******************************************************************************/
::class "RexxUnicodeGraphemeSupplier" public subclass Supplier -
                                             inherit GraphemeSupplierHelper
/******************************************************************************/

-- Grapheme supplier for UTF-8 strings without pre-indexing

-- Using a mixin class requires giving access to these (normally) private variables.
::attribute error get package           -- "" if no error, otherwise error description
::attribute grapheme get package        -- a string of length 1..n, or .nil if no more grapheme available
::attribute graphemeIndexB get package  -- 1..length+1, counted from the beginning of the string, negative if the grapheme byte sequence is invalid
::attribute indexG get package          -- 1-based, counted from the beginning of the string

::method init
    expose isFirstStep string indexG -
           indexer graphemeAction codepointAction

    use strict arg string, defaultIndexType="", defaultItemType="", -
                   indexer=.nil, graphemeAction=.nil, codepointAction=.nil
    string = requestBaseString(string, "string")
    self~setDefaultIndexType(defaultIndexType)
    self~setDefaultItemType(defaultItemType)

    empty = .array~new(0) -- No need to create a copy, the string is immutable
    self~init:super(empty, empty)

    indexG = 1
    isFirstStep = .true
    self~decode


/*
This method is called from ~init and from ~next.

Simplification by Claude.
Key simplification versus the original: a grapheme is delimited by two
boundaries (its start and its end/next-start), and each call to ~decode only
ever needs to resolve ONE boundary before it can return - the other boundary
is always already known (either from the previous call, or, for the very
first grapheme, from the first step phase below). So there's no need for a
rotating 2-slot buffer: a single "pendingIndexB/pendingError" slot, filled by
the shared scanBoundary helper, is enough. ~next simply promotes that pending
boundary into graphemeIndexB/error before resuming here.

"Locate the start of grapheme 1" and "locate the end of the current grapheme"
are the same operation - scanBoundary is used for both.
*/
::method decode private
    expose isFirstStep string grapheme graphemeIndexB error -
           /* scan state, shared with scanBoundary */ -
           indexB previousCodepoint graphemeBreakState pendingIndexB pendingError -
           indexer graphemeAction

    use strict arg -- none

    if isFirstStep then do
        indexB = 1
        previousCodepoint = -1
        graphemeBreakState = 0
        self~scanBoundary                  -- locates the start of the first grapheme (.nil if the string is empty)
        graphemeIndexB = pendingIndexB
        error = pendingError
        isFirstStep = .false
    end

    if graphemeIndexB~isNil then do    -- nothing left to deliver (empty string, or already exhausted)
        grapheme = .nil
        return                         -- grapheme ready for use (.nil): ~available will now report .false
    end

    self~scanBoundary                  -- locates the end of the current grapheme (.nil at end of string)
    if pendingIndexB~isNil then grapheme = string~substr(abs(graphemeIndexB))
    else grapheme = string~substr(abs(graphemeIndexB), abs(pendingIndexB) - abs(graphemeIndexB))
    if .nil \== indexer, .nil \== graphemeAction then indexer~send(graphemeAction, error \== "", abs(graphemeIndexB), grapheme~length, error)
    -- grapheme, graphemeIndexB and error are ready for use;
    -- on reentry from ~next, graphemeIndexB/error have been set from pendingIndexB/pendingError
    return


-- Advances indexB (decoding codepoints one at a time, tracking previousCodepoint
-- and graphemeBreakState for the extended grapheme break algorithm) until either
-- a grapheme boundary is found or the string is exhausted.
-- Result is left in pendingIndexB/pendingError:
--   pendingIndexB = .nil                     -> end of string, nothing more to scan
--   pendingIndexB = byte position (>0)       -> boundary at a validly-decoded codepoint
--   pendingIndexB = -(byte position)         -> boundary at an invalid byte sequence; pendingError describes it
::method scanBoundary private
    expose string indexB previousCodepoint graphemeBreakState pendingIndexB pendingError -
           indexer codepointAction

    use strict arg -- none

    loop forever
        codepoint = .RexxUnicode~utf8DecodeCodepoint(string, indexB, >sizeB, >errorCode, >errorMsg)

        if sizeB == 0 then do                  -- end of string
            pendingIndexB = .nil
            pendingError  = ""
            return
        end

        if sizeB < 0 then do                   -- invalid byte sequence: always its own boundary
            pendingIndexB = -indexB
            pendingError  = "start byte position" indexB ":" errorMsg
            if .nil \== indexer, .nil \== codepointAction then indexer~send(codepointAction, .true, indexB, -sizeB, codepoint, pendingError)
            indexB += abs(sizeB)                -- skip the invalid bytes
            previousCodepoint = codepoint
            graphemeBreakState = 0              -- reset the extended grapheme state
            return
        end
        else do
            if .nil \== indexer, .nil \== codepointAction then indexer~send(codepointAction, .false, indexB, sizeB, codepoint, "")
        end

        -- graphemeBreak must NOT be called when previousCodepoint < 0 (nothing to compare against yet)
        if previousCodepoint < 0 then isBoundary = 1
        else isBoundary = .RexxUnicode~graphemeBreak(previousCodepoint, codepoint, >graphemeBreakState)

        boundaryHere = indexB
        previousCodepoint = codepoint
        indexB += sizeB

        if isBoundary then do
            pendingIndexB = boundaryHere
            pendingError  = ""
            return
        end
        -- else: no break here, keep scanning (loop continues)
    end


::method next
    expose error indexG graphemeIndexB pendingIndexB pendingError

    use strict arg -- none
    if self~available then do
        indexG += 1
        graphemeIndexB = pendingIndexB
        error = pendingError
        self~decode
    end
    else raise syntax 93.937 -- No more supplier items available


/******************************************************************************/
::class "RexxUnicodeReverseGraphemeSupplier" public subclass Supplier -
                                                    inherit GraphemeSupplierHelper
/******************************************************************************/

-- Reverse grapheme supplier for UTF-8 strings without pre-indexing.
-- Initial ooRexx code by Claude Sonnet 5 medium, reviewed and adapted.

-- Using a mixin class requires giving access to these (normally) private variables.
::attribute error get package           -- "" if no error, otherwise error description
::attribute grapheme get package        -- a string of length 1..n, or .nil if no more grapheme available
::attribute graphemeIndexB get package  -- 1..length+1, counted from the beginning of the string, negative if the grapheme byte sequence is invalid
::attribute indexG get package          -- 1-based, counted from the end of the string

::method init
    expose string indexB indexG error -
           indexer graphemeAction codepointAction

    use strict arg string, defaultIndexType="", defaultItemType="", -
                   indexer=.nil, graphemeAction=.nil, codepointAction=.nil
    string = requestBaseString(string, "string")
    self~setDefaultIndexType(defaultIndexType)
    self~setDefaultItemType(defaultItemType)

    empty = .array~new(0) -- No need to create a copy, the string is immutable
    self~init:super(empty, empty)

    -- Unlike the reverse codepoint supplier, I don't use a variable named "boundaryIndexB".
    -- "indexB" can be used, because the grapheme byte position is managed with "graphemeIndexB".
    indexB = string~length + 1 -- boundary (1-based): the grapheme we decode ends just before this
    indexG = 1 -- count from the end
    error = ""
    self~decodePrevious


::method next
    expose indexB indexG graphemeIndexB
    use strict arg -- none
    if self~available then do
        indexB = abs(graphemeIndexB)   -- move the right edge to the grapheme we just found
        indexG += 1
        self~decodePrevious
    end
    else raise syntax 93.937 -- No more supplier items available


-- Decode the grapheme cluster ending immediately before `indexB`, and set
-- indexB's new left edge into graphemeIndexB. grapheme = .nil at start of text.
::method decodePrevious private
    expose string indexB grapheme graphemeIndexB error -
           indexer graphemeAction codepointAction

    use strict arg -- none

debug = .false -- keep it for the moment
if debug then say
if debug then say "Enter decodePrevious with indexB =" indexB
    error = ""

    -- ok, it's an alternative way to detect the beginning of string,
    -- the other way being to test if sizeB == 0 after utf8DecodePreviousCodepoint.
    if indexB <= 1 then do
        grapheme = .nil
        graphemeIndexB = .nil
if debug then say "Leave decodePrevious with graphemeIndexB =" graphemeIndexB", grapheme =" grapheme
        return
    end

    -- here, indexB is the index of the codepoint AFTER the codepoint to decode
    codepoint2 = .RexxUnicode~utf8DecodePreviousCodepoint(string, indexB, >sizeB, >errorCode, >errorMsg)
if debug then say "    utf8DecodePreviousCodepoint with indexB =" indexB "returned codepoint2 =" codepoint2 "of size" sizeB
    cursor = indexB - abs(sizeB)      -- start of the rightmost scalar so far

    -- An ill-formed byte's maximal subpart is always its own grapheme cluster
    -- (mirrors the forward supplier's error handling: no merging across an error).
    if sizeB < 0 then do
        graphemeIndexB = -cursor -- same convention as .RexxUnicodeStringIndexer; a negative byte index indicates an invalid byte sequence
        grapheme = string~substr(cursor, indexB - cursor)
        error = "start byte position" cursor ":" errorMsg
        if .nil \== indexer, .nil \== codepointAction then indexer~send(codepointAction, .true, cursor, -sizeB, codepoint2, error)
        if .nil \== indexer, .nil \== graphemeAction then indexer~send(graphemeAction, .true, cursor, grapheme~length, error)
if debug then say "Leave decodePrevious with graphemeIndexB =" graphemeIndexB", grapheme =" grapheme~c2x
        return
    end
    else do
        if .nil \== indexer, .nil \== codepointAction then indexer~send(codepointAction, .false, cursor, sizeB, codepoint2, "")
    end

    loop
        if cursor <= 1 then leave    -- start of text: always a boundary

        -- here, cursor is the index of the codepoint AFTER the codepoint to decode
        codepoint1 = .RexxUnicode~utf8DecodePreviousCodepoint(string, cursor, >sizeB1, >errorCode1, >errorMsg1)
if debug then say "    utf8DecodePreviousCodepoint with cursor =" cursor "returned codepoint1 =" codepoint1 "of size" sizeB
        pos1 = cursor - abs(sizeB1)

        if sizeB1 < 0 then leave    -- adjacent error is always its own cluster: break here

        graphemeBreakBackward = .RexxUnicode~graphemeBreakBackward(string, pos1, codepoint1, codepoint2)
if debug then say "    graphemeBreakBackward with pos1 =" pos1", codepoint1 =" codepoint1", codepoint2 =" codepoint2 "returned" graphemeBreakBackward
        if graphemeBreakBackward then leave

        -- No break: extend the grapheme leftward and keep going
        if .nil \== indexer, .nil \== codepointAction then indexer~send(codepointAction, .false, pos1, -sizeB1, codepoint1, "")
        cursor = pos1
        codepoint2 = codepoint1
        prop2 = prop1
    end

    graphemeIndexB = cursor
    grapheme = string~substr(cursor, indexB - cursor)
    error = ""
    if .nil \== indexer, .nil \== graphemeAction then indexer~send(graphemeAction, .false, cursor, grapheme~length, "")
if debug then say "Leave decodePrevious with graphemeIndexB =" graphemeIndexB", grapheme =" grapheme~c2x


/******************************************************************************/
::class "RexxUnicodeStringIndexer" public
/******************************************************************************/

/*
Direct access to codepoints and graphemes.
This is a temporary implementation, to be replaced by a native indexer.

The memory usage of the native grapheme indexer will be roughly 15% of the string size (12.5% for a bit vector and 2.5% for a two-level summary table).
Same memory usage for the native codepoint indexer.
The memory usage of the invalid byte flags will be between 0% and 12.5% of the string size (bit vector).
*/

::attribute string get
::attribute codepointCount get          -- count after full scan, not impacted by the start storage limits
::attribute graphemeCount get           -- count after full scan, not impacted by the start storage limits
::attribute errorCount get              -- count after full scan, not impacted by the start storage limits


::method startCodepointIndexes
    expose startCodepointIndexes
    -- Currently, the internal storage `startCodepointIndexes` (the exposed variable) is an ooRexx array,
    -- but it will be replaced by a C++ bitVector + summaryTable.
    -- The external representation `startCodepointIndexes` (this method) will always return a copy of the internal storage as an array.
    if .nil == startCodepointIndexes then return .array~new
    return startCodepointIndexes~copy


::method startCodepointStorageCount
    expose startCodepointIndexes
    if .nil == startCodepointIndexes then return 0
    return startCodepointIndexes~items


::method endCodepointIndexes
    expose endCodepointIndexes
    -- Currently, the internal storage `endCodepointIndexes` (the exposed variable) is an ooRexx array,
    -- but it will be replaced by a C++ bitVector + summaryTable.
    -- The external representation `endCodepointIndexes` (this method) will always return a copy of the internal storage as an array.
    if .nil == endCodepointIndexes then return .array~new
    return endCodepointIndexes~copy


::method endCodepointStorageCount
    expose endCodepointIndexes
    if .nil == endCodepointIndexes then return 0
    return endCodepointIndexes~items


::method startGraphemeIndexes
    expose startGraphemeIndexes
    -- Currently, the internal storage `startGraphemeIndexes` (the exposed variable) is an ooRexx array,
    -- but it will be replaced by a C++ bitVector + summaryTable.
    -- The external representation `startGraphemeIndexes` (this method) will always return a copy of the internal storage as an array.
    if .nil == startGraphemeIndexes then return .array~new
    return startGraphemeIndexes~copy


::method startGraphemeStorageCount
    expose startGraphemeIndexes
    if .nil == startGraphemeIndexes then return 0
    return startGraphemeIndexes~items


::method endGraphemeIndexes
    expose endGraphemeIndexes
    -- Currently, the internal storage `endGraphemeIndexes` (the exposed variable) is an ooRexx array,
    -- but it will be replaced by a C++ bitVector + summaryTable.
    -- The external representation `endGraphemeIndexes` (this method) will always return a copy of the internal storage as an array.
    if .nil == endGraphemeIndexes then return .array~new
    return endGraphemeIndexes~copy


::method endGraphemeStorageCount
    expose endGraphemeIndexes
    if .nil == endGraphemeIndexes then return 0
    return endGraphemeIndexes~items


::method startErrorMessages
    expose startErrorMessages
    -- Currently, the internal storage `startErrorMessages` (the exposed variable) is an ooRexx array,
    -- but it will be replaced by a C++ bitVector + table of error messages.
    -- The external representation `startErrorMessages` (this method) will always return a copy of the internal storage as an array.
    if .nil == startErrorMessages then return .array~new
    return startErrorMessages~copy


::method startErrorStorageCount
    expose startErrorMessages
    if .nil == startErrorMessages then return 0
    return startErrorMessages~items


::method endErrorMessages
    expose endErrorMessages
    -- Currently, the internal storage `endErrorMessages` (the exposed variable) is an ooRexx array,
    -- but it will be replaced by a C++ bitVector + table of error messages.
    -- The external representation `endErrorMessages` (this method) will always return a copy of the internal storage as an array.
    if .nil == endErrorMessages then return .array~new
    return endErrorMessages~copy


::method endErrorStorageCount
    expose endErrorMessages
    if .nil == endErrorMessages then return 0
    return endErrorMessages~items


::method init
    expose string -
           startCodepointStorageLimit startGraphemeStorageLimit startErrorStorageLimit -
           endCodepointStorageLimit endGraphemeStorageLimit endErrorStorageLimit -
           startCodepointIndexes endCodepointIndexes -
           codepointCount -
           startGraphemeIndexes startLastGraphemeNextIndex endGraphemeIndexes -
           graphemeCount -
           startErrorMessages endErrorMessages -
           errorCount

    use strict arg string, -
                   startCodepointStorageLimit = (.RexxInfo~internalMaxNumber), startGraphemeStorageLimit = (.RexxInfo~internalMaxNumber), startErrorStorageLimit = (.RexxInfo~internalMaxNumber), -
                   endCodepointStorageLimit = 0, endGraphemeStorageLimit = 0, endErrorStorageLimit = 0
    string = requestBaseString(string)

    codepointCount = 0
    graphemeCount = 0
    errorCount = 0

    startCodepointIndexes = .nil
    startGraphemeIndexes = .nil
    startErrorMessages = .nil

    -- Extra index to let calculate the grapheme's length of the grapheme at startGraphemeStorageLimit.
    -- This index is not put in startGraphemeIndexes to keep startGraphemeIndexes's items <= startGraphemeStorageLimit.
    startLastGraphemeNextIndex = string~length + 1 -- will be updated during the scan

    /*
    Remember:
    - no need of endLastGraphemeNextIndex for endGraphemeIndexes
      because it's always string~length + 1.
    - no need of startLastCodepointNextIndex for startCodepointIndexes
      because the UTF-8 encoding gives the length (the codepoint is decoded to return it as a wholenumber).
    - no need of endLastCodepointNextIndex for endCodepointIndexes, for the same reason.
    */

    endCodepointIndexes = .nil
    endGraphemeIndexes = .nil
    endErrorMessages = .nil

    /***************************/
    /* Fast path if no storage */
    /***************************/

    if startCodepointStorageLimit == 0, startGraphemeStorageLimit == 0, startErrorStorageLimit == 0 -
       endCodepointStorageLimit == 0, endGraphemeStorageLimit == 0, endErrorStorageLimit == 0 -
    then do
        -- No storage, fast path
        .RexxUnicodeServices~utf8StringInfo(string, >graphemeCount, >codepointCount, >errorCount)
        return
    end

    /**********************/
    /* Forward indexation */
    /**********************/

    if startCodepointStorageLimit > 0 then startCodepointIndexes = .array~new
    if startGraphemeStorageLimit > 0 then startGraphemeIndexes = .array~new
    if startErrorStorageLimit > 0 then startErrorMessages = .array~new

    s = .RexxUnicodeGraphemeSupplier~new(string, , , self, "startGraphemeAction", "startCodepointAction")
    do while s~available
        -- full scan
        s~next
    end

    /***********************/
    /* Backward indexation */
    /***********************/

    /*
        startCodepointStorageLimit      endCodepointStorageLimit    codepointCount      adjusted endCodepointStorageLimit
        max                             any                         any                 N/A
        0                               max                         any                 codepointCount
        10                              10                          5                   N/A
        10                              10                          15                  5
        10                              2                           15                  2
    */
    if startCodepointStorageLimit < codepointCount then do
        -- Some codepoints indexes are not stored
        endCodepointStorageLimit = min(endCodepointStorageLimit, codepointCount - startCodepointStorageLimit)
        if endCodepointStorageLimit > 0 then endCodepointIndexes = .array~new(endCodepointStorageLimit)
    end

    if startGraphemeStorageLimit < graphemeCount then do
        -- Some graphemes indexes are not stored
        endGraphemeStorageLimit = min(endGraphemeStorageLimit, graphemeCount - startGraphemeStorageLimit)
        if endGraphemeStorageLimit > 0 then endGraphemeIndexes = .array~new(endGraphemeStorageLimit)
    end

    if startErrorStorageLimit < errorCount then do
        -- Some errors are not stored
        endErrorStorageLimit = min(endErrorStorageLimit, errorCount - startErrorStorageLimit)
        if endErrorStorageLimit > 0 then endErrorMessages = .array~new(endErrorStorageLimit)
    end

    s = .RexxUnicodeReverseGraphemeSupplier~new(string, , , self, "endGraphemeAction", "endCodepointAction")
    do while s~available
        -- stop when all end storages are full
        full = .true
        if .nil \== endCodepointIndexes, endCodepointIndexes~items < endCodepointStorageLimit then full = .false
        if .nil \== endGraphemeIndexes, endGraphemeIndexes~items < endGraphemeStorageLimit then full = .false
        if .nil \== endErrorMessages, endErrorMessages~items < endErrorStorageLimit then full = .false
        if full then leave

        s~next
    end


-- Called by a grapheme supplier for every valid codepoint  and every invalid byte sequence
::method startCodepointAction package
    expose codepointCount -
           startCodepointStorageLimit startCodepointIndexes -
           errorCount -
           startErrorStorageLimit startErrorMessages
    -- trace i
    use strict arg error /* boolean */, indexB /* >0 */, sizeB, codepoint /* integer */, errorMsg=""

    if error then do
        -- The byte sequence is invalid
        errorCount += 1
        if .nil \== startErrorMessages, startErrorMessages~items < startErrorStorageLimit then startErrorMessages~append(errorMsg)

        -- stopAtFirstError is not supported by a supplier, but is supported by a native iterator
        if .true /* \ stopAtFirstError */ then do
            codepointCount += 1
            if .nil \== startCodepointIndexes, startCodepointIndexes~items < startCodepointStorageLimit then startCodepointIndexes~append(-indexB) -- a negative index means "error"
        end
    end
    else do
        codepointCount += 1
        if .nil \== startCodepointIndexes, startCodepointIndexes~items < startCodepointStorageLimit then startCodepointIndexes~append(indexB)
    end


-- Called by a grapheme supplier for every grapheme  and every invalid byte sequence
::method startGraphemeAction package
    expose graphemeCount -
           startGraphemeStorageLimit startGraphemeIndexes startLastGraphemeNextIndex
    -- trace i
    use strict arg error /* boolean */, indexB /* >0 */, sizeB, errorMsg=""

    if error then do
         -- stopAtFirstError is not supported by a supplier, but is supported by a native iterator
        if .true /* \ stopAtFirstError */ then do
            graphemeCount += 1
            if .nil \== startGraphemeIndexes, startGraphemeIndexes~items < startGraphemeStorageLimit then do
                startGraphemeIndexes~append(-indexB) -- a negative index means "error"
                startLastGraphemeNextIndex = indexB + sizeB
            end
        end
    end
    else do
        graphemeCount += 1
        if .nil \== startGraphemeIndexes, startGraphemeIndexes~items < startGraphemeStorageLimit then do
            startGraphemeIndexes~append(indexB)
            startLastGraphemeNextIndex = indexB + sizeB
        end
    end


-- Called by a reverse grapheme supplier for every valid codepoint  and every invalid byte sequence
::method endCodepointAction package
    expose endCodepointStorageLimit endCodepointIndexes -
           endErrorStorageLimit endErrorMessages
    -- trace i
    use strict arg error /* boolean */, indexB /* >0 */, sizeB, codepoint /* integer */, errorMsg=""

    if error then do
        -- The byte sequence is invalid
        if .nil \== endErrorMessages, endErrorMessages~items < endErrorStorageLimit then endErrorMessages~append(errorMsg)
        -- stopAtFirstError is not supported by a supplier, but is supported by a native iterator
        if .true /* \ stopAtFirstError */ then do
            if .nil \== endCodepointIndexes, endCodepointIndexes~items < endCodepointStorageLimit then endCodepointIndexes~append(-indexB) -- a negative index means "error"
        end
    end
    else do
        if .nil \== endCodepointIndexes, endCodepointIndexes~items < endCodepointStorageLimit then endCodepointIndexes~append(indexB)
    end


-- Called by a reverse grapheme supplier for every grapheme  and every invalid byte sequence
::method endGraphemeAction package
    expose endGraphemeStorageLimit endGraphemeIndexes
    -- trace i
    use strict arg error /* boolean */, indexB /* >0 */, sizeB, errorMsg=""

    if error then do
         -- stopAtFirstError is not supported by a supplier, but is supported by a native iterator
        if .true /* \ stopAtFirstError */ then do
            if .nil \== endGraphemeIndexes, endGraphemeIndexes~items < endGraphemeStorageLimit then endGraphemeIndexes~append(-indexB) -- a negative index means "error"
        end
    end
    else do
        if .nil \== endGraphemeIndexes, endGraphemeIndexes~items < endGraphemeStorageLimit then endGraphemeIndexes~append(indexB)
    end


-- Converts a codepoint index to a byte index
::method codepointIndexB
    expose string startCodepointIndexes endCodepointIndexes codepointCount
    use strict arg indexC, raiseError=.true

    signedIndexC = indexC -- used in error messages
    if indexC == 0 then signal invalid_codepoint_index
    else if indexC < 0 then do
        -- Index from the end
        indexC = codepointCount - abs(indexC) + 1
        if indexC <= 0 then signal codepoint_index_too_large
    end
    else do
        -- Index from the start
        -- Here, indexC > 0
        if indexC > codepointCount then signal codepoint_index_too_large
    end

    if .nil \== endCodepointIndexes,  endCodepointIndexes~items \== 0 then do
        /*
        We have a non-empty end storage.
        Assuming a string made of 10 codepoints, a start storage limit of 3, an end storage limit of 4.
        Indexes of codepoints in string     1 2 3 4 5 6 7 8 9 10
        Indexes of startCodepointIndexes    1 2 3
        Items   of startCodepointIndexes    1 2 3
        Indexes of endCodepointIndexes                  4 3 2 1
        Items   of endCodepointIndexes                  7 8 9 10
        */

        -- See if the end storage contains the requested index
        firstEndIndexMinus1 = codepointCount - endCodepointIndexes~items
        if indexC > firstEndIndexMinus1 then do
            -- The requested index is in the end storage
            endIndexC = codepointCount - indexC + 1 -- converts an index in the range 7..10 to an index in the range 4..1
            return endCodepointIndexes[endIndexC]
        end
    end

    -- Here, no end storage (either because not requested, or because the string is empty), or end storage not applicable for the request index
    items = 0
    if .nil \== startCodepointIndexes then items = startCodepointIndexes~items
    if indexC > items then signal codepoint_index_is_not_stored
    return startCodepointIndexes[indexC]

    invalid_codepoint_index:        if raiseError then raise syntax 88.900 array ("Invalid code point index:" signedIndexC) ; else return .nil
    codepoint_index_too_large:      if raiseError then raise syntax 88.900 array ("Invalid code point index:" signedIndexC "exceeds the code point count" codepointCount) ; else return .nil
    codepoint_index_is_not_stored:  if raiseError then raise syntax 88.900 array ("Code point index" signedIndexC "is not stored") ; else return .nil


-- Returns the codepoint index (1-based) corresponding to byte index (1-based) `indexB`.
-- It is .nil if the codepoint index, or any of the intermediate codepoint indexes are not stored.
-- It is negative if the byte index is not aligned with a codepoint boundary.
-- It is .nil if the codepoint index, or any of the intermediate codepoint indexes are not stored.
-- Performs a binary search.
-- Supports negative byte indexes and sparse storage.
::method codepointIndexC
    expose startCodepointIndexes endCodepointIndexes codepointCount
    use strict arg indexB

    signedIndexB = indexB -- used in error messages
    if indexB == 0 then signal invalid_codepoint_index
    else if indexB < 0 then do
        -- Index from the end
        indexB = self~string~length - abs(indexB) + 1
        if indexB <= 0 then signal codepoint_index_too_large
        -- here, we are sure that indexB is in the range 1..self~string~length
    end
    else do
        -- Index from the start
        -- Here, indexB > 0
        if indexB > self~string~length then signal codepoint_index_too_large
    end

    -- Here, codepointCount can't be zero because self~string is not empty.
    -- If self~string was empty, an error has been raised because abs(indexB), which is not 0, can't be > self~string~length

    indexC1 = 1
    indexC2 = codepointCount
    do forever
        if indexC1 > indexC2 then return -indexC2 -- -- negative when not aligned with the codepoint index
        indexC = (indexC1 + indexC2) % 2
        item = .nil
        if .nil \== startCodepointIndexes then item = startCodepointIndexes[indexC]
        if .nil == item, .nil \== endCodepointIndexes then item = endCodepointIndexes[codepointCount - indexC + 1]
        if .nil == item then return .nil -- not stored
        item = abs(item) -- the stored index can be negative to indicate an invalid byte sequence
        if item == indexB then return indexC
        if item > indexB then indexC2 = indexC - 1
                         else indexC1 = indexC + 1
    end
    return -- should never reach here

    invalid_codepoint_index:    raise syntax 88.900 array ("Invalid byte index:" signedIndexB)
    codepoint_index_too_large:  raise syntax 88.900 array ("Invalid byte index:" signedIndexB "exceeds the byte string length" self~string~length)


-- Returns the codepoint at position indexC
::method codepointAtIndexC
    expose string startCodepointIndexes
    use strict arg indexC
    startB = self~codepointIndexB(indexC)
    if startB < 0 then return "FFFD"~x2d -- the current byte sequence is invalid, return the replacement character
    return .RexxUnicode~utf8DecodeCodepoint(string, startB) -- yes, must decode to return the codepoint as a whole number


-- Converts a grapheme index to a byte index.
-- When `next` is .true, return the byte index of the next grapheme index (this is
-- different from asking the byte index of indexG + 1, because indexG + 1 may be
-- considered too large).
::method graphemeIndexB
    expose string startGraphemeIndexes startLastGraphemeNextIndex endGraphemeIndexes graphemeCount
    use strict arg indexG, next = .false, raiseError=.true

    signedIndexG = indexG -- used in error messages
    if indexG <= 0 then do
        -- Index from the end
        indexG = graphemeCount - abs(indexG) + 1

        if signedIndexG == 0 then signal invalid_grapheme_index
        if indexG <= 0 then signal grapheme_index_too_large
        if indexG > graphemeCount then signal grapheme_index_too_large
    end
    else do
        -- Index from the start
        -- Here, indexG > 0
        if indexG > graphemeCount then signal grapheme_index_too_large
    end

    if next then indexG += 1

    if .nil \== endGraphemeIndexes, endGraphemeIndexes~items \== 0 then do
        /*
        We have a non-empty end storage.
        Assuming a string made of 10 graphemes, a start storage limit of 3, an end storage limit of 4.
        Indexes of graphemes in string      1 2 3 4 5 6 7 8 9 10
        Indexes of startGraphemeIndexes     1 2 3
        Items   of startGraphemeIndexes     1 2 3
        Indexes of endGraphemeIndexes                   1 2 3 4
        Items   of endGraphemeIndexes                   7 8 9 10
        */

        -- See if the end storage contains the requested index
        firstEndIndexMinus1 = graphemeCount - endGraphemeIndexes~items
        if indexG > firstEndIndexMinus1 then do
            -- The requested index is in the end storage

            -- This special case can happen when `next` is .true
            if indexG == (graphemeCount + 1) then return string~length + 1 -- byte index of the grapheme after the grapheme at graphemeCount

            endIndexG = graphemeCount - indexG + 1 -- converts an index in the range 7..10 to an index in the range 4..1
            return endGraphemeIndexes[endIndexG]
        end
    end

    -- Here, no end storage (either because not requested, or because the string is empty), or end storage not applicable for the request index

    if .nil == startGraphemeIndexes then signal codepoint_index_is_not_stored

    -- This special case can happen when `next` is .true
    if indexG == (startGraphemeIndexes~items + 1) then return startLastGraphemeNextIndex

    if indexG > startGraphemeIndexes~items then signal grapheme_index_is_not_stored
    return startGraphemeIndexes[indexG]

    invalid_grapheme_index:         if raiseError then raise syntax 88.900 array ("Invalid grapheme index:" signedIndexG) ; else return .nil
    grapheme_index_too_large:       if raiseError then raise syntax 88.900 array ("Invalid grapheme index:" signedIndexG "exceeds the grapheme count" graphemeCount) ; else return .nil
    grapheme_index_is_not_stored:   if raiseError then raise syntax 88.900 array ("Grapheme index" signedIndexG "is not stored") ; else return .nil


-- Returns the grapheme index corresponding to byte index `indexB`.
-- It is negative if the byte index is not aligned with a grapheme boundary.
-- It is .nil if the grapheme index, or any of the intermediate grapheme indexes are not stored.
-- Performs a binary search.
-- Supports negative byte indexes and sparse storage, even though the latter does not make much sense.
::method graphemeIndexG
    expose startGraphemeIndexes endGraphemeIndexes graphemeCount
    use strict arg indexB

    signedIndexB = indexB -- used in error messages
    if indexB <= 0 then do
        -- Index from the end
        indexB = self~string~length - abs(indexB) + 1
        if signedIndexB == 0 then signal invalid_byte_index
        if indexB <= 0 then signal byte_index_too_large
        if indexB > self~string~length then signal byte_index_too_large
    end
    else do
        -- Index from the start
        -- Here, indexB > 0
        if indexB > self~string~length then signal byte_index_too_large
    end

    indexG1 = 1
    indexG2 = graphemeCount
    do forever
        if indexG1 > indexG2 then return -indexG2 -- -- negative when not aligned with the grapheme index
        indexG = (indexG1 + indexG2) % 2
        item = .nil
        if .nil \== startGraphemeIndexes then item = startGraphemeIndexes[indexG]
        if .nil == item, .nil \== endGraphemeIndexes then item = endGraphemeIndexes[graphemeCount - indexG + 1]
        if .nil == item then return .nil
        item = abs(item) -- the stored index can be negative to indicate an invalid byte sequence
        if item == indexB then return indexG
        if item > indexB then indexG2 = indexG - 1
                         else indexG1 = indexG + 1
    end
    return -- should never reach here

    invalid_byte_index:         raise syntax 88.900 array ("Invalid byte index:" signedIndexB)
    byte_index_too_large:       raise syntax 88.900 array ("Invalid byte index:" signedIndexB "exceeds the byte string length" self~string~length)


-- Returns the grapheme at position indexG
::method graphemeAtIndexG
    expose string
    use strict arg indexG
    startB = self~graphemeIndexB(indexG)
    nextB = self~graphemeIndexB(indexG, /*next*/ .true)
    absStartB = abs(startB) -- startB can be negative (meaning current byte sequence is invalid)
    absNextB = abs(nextB) -- nextB can be negative (meaning next byte sequence is invalid)
    return string~substr(absStartB, absNextB - absStartB) -- yes, always the current byte sequence, even if invalid


-- Returns a string providing information about the indexer's string
::method stringInfo
    use strict arg -- none

    string = self~string
    graphemeCount = self~graphemeCount
    codepointCount = self~codepointCount
    errorCount = self~errorCount

    info =    "(" ||,
              .RexxUnicode~stringIsASCII(string)~?("ASCII", "not-ASCII")"," -
              graphemeCount "grapheme" || singularPlural(graphemeCount)"," -
              codepointCount "codepoint" || singularPlural(codepointCount)"," -
              string~length "byte" || singularPlural(string~length)"," -
              errorCount "error" || singularPlural(errorCount) -
              || ")"
    return info


-- Returns a RexxUnicodeStringIndexer instance created from `object`.
-- If `object` is already a RexxUnicodeStringIndexer instance, returns it unchanged.
::routine requestRexxUnicodeStringIndexer public
    use strict arg object

    if object~isA(.RexxUnicodeStringIndexer) then return object
    return .RexxUnicodeStringIndexer~new(object)


/******************************************************************************/
::class "RexxUnicodeCharacter" public inherit Comparable
/******************************************************************************/

::attribute codepoint get -- special value -1 for invalid character

/*
preferredName is the "best" Unicode name to display for the character.
Use the standard name when not empty,
otherwise use the alias when not empty (ICU gives only one alias,
  even if NameAliases.txt defines several aliases),
otherwise use the extended name (never empty)
*/
::attribute preferredName get


::method init
    expose codepoint preferredName
    use strict arg codepoint
    .RexxUnicode~checkCodepoint(codepoint, /*minusOneAllowed*/ .true)
    preferredName = self~name
    if preferredName == "" then preferredName = self~nameAlias
    if preferredName == "" then preferredName = self~extendedName


::method string
    expose codepoint preferredName
    use strict arg -- none
    if codepoint == -1 then return "An invalid character"
    utf8 = self~UTF8
    if .RexxUnicode~codepointIsPrintable(codepoint) then
        string = quoted(utf8)
    else
        string = "<?>" -- can't print the character
    return string -
           escapedHexdigits(utf8~c2x) -
           .RexxUnicode~D2U(codepoint) -
           self~category self~categoryName -
           quoted(preferredName)


::method name
    expose codepoint
    use strict arg -- none
    if codepoint == -1 then return ""
    return .RexxUnicode~codepointCharName(codepoint)


::method extendedName
    expose codepoint
    use strict arg -- none
    if codepoint == -1 then return ""
    return .RexxUnicode~codepointExtendedCharName(codepoint)


::method nameAlias
    expose codepoint
    use strict arg -- none
    if codepoint == -1 then return ""
    return .RexxUnicode~codepointCharNameAlias(codepoint)


::method UTF8
    expose codepoint
    use strict arg -- none
    if codepoint == -1 then return ""
    return .RexxUnicode~utf8EncodeCodepoint(codepoint, .MutableBuffer~new)~string


-- http://www.unicode.org/reports/tr44/#General_Category_Values
::method category
    expose codepoint
    use strict arg -- none
    if codepoint == -1 then return "Cn"
    .RexxUnicode~codepointCategory(codepoint, >code)
    return code


-- http://www.unicode.org/reports/tr44/#General_Category_Values
::method categoryName
    expose codepoint
    use strict arg -- none
    if codepoint == -1 then return "Unassigned"
    .RexxUnicode~codepointCategory(codepoint, , >label)
    return label


-- http://www.unicode.org/reports/tr44/#Canonical_Combining_Class_Values
::method combiningClass
    expose codepoint
    use strict arg -- none
    if codepoint == -1 then return "?"
    .RexxUnicode~codepointCombiningClass(codepoint, >code)
    return code


-- http://www.unicode.org/reports/tr44/#Canonical_Combining_Class_Values
::method combiningClassName
    expose codepoint
    use strict arg -- none
    if codepoint == -1 then return "?"
    .RexxUnicode~codepointCombiningClass(codepoint, , >label)
    return label


-- http://www.unicode.org/reports/tr44/#Canonical_Combining_Class_Values
::method combiningClassValue
    expose codepoint
    use strict arg -- none
    if codepoint == -1 then return -1
    -- yes, return the numeric value, it's a Unicode standard value
    return .RexxUnicode~codepointCombiningClass(codepoint)


-- http://www.unicode.org/reports/tr44/#Bidi_Class_Values
::method bidiClass
    expose codepoint
    use strict arg -- none
    if codepoint == -1 then return "?"
    .RexxUnicode~codepointBidiClass(codepoint, >code)
    return code


-- http://www.unicode.org/reports/tr44/#Bidi_Class_Values
::method bidiClassName
    expose codepoint
    use strict arg -- none
    if codepoint == -1 then return "?"
    .RexxUnicode~codepointBidiClass(codepoint, , >label)
    return label


-- https://unicode.org/reports/tr9/
::method bidiMirrored
    expose codepoint
    use strict arg -- none
    if codepoint == -1 then return .false
    return .RexxUnicode~codepointBidiMirrored(codepoint)


-- https://unicode.org/reports/tr15/
::method decompositionType
    expose codepoint
    use strict arg -- none
    if codepoint == -1 then return "?"
    .RexxUnicode~codepointDecompositionType(codepoint, >code)
    return code


-- https://unicode.org/reports/tr15/
::method decompositionTypeName
    expose codepoint
    use strict arg -- none
    if codepoint == -1 then return "?"
    .RexxUnicode~codepointDecompositionType(codepoint, , >label)
    return label


-- http://www.unicode.org/review/pr-5.html
-- http://unicode.org/L2/L2002/02368-default-ignorable.html
::method ignorable
    expose codepoint
    use strict arg -- none
    if codepoint == -1 then return .true
    return .RexxUnicode~codepointIgnorable(codepoint)


-- https://www.unicode.org/reports/tr11/
::method eastAsianWidthIsAmbiguous
    expose codepoint
    use strict arg -- none
    if codepoint == -1 then return .false
    return .RexxUnicode~codepointEastAsianWidthIsAmbiguous(codepoint)


-- https://www.unicode.org/reports/tr29/
::method controlBoundary
    expose codepoint
    use strict arg -- none
    if codepoint == -1 then return .false
    return .RexxUnicode~codepointControlBoundary(codepoint)


-- Width of the printed character (not a Unicode property)
::method charWidth
    expose codepoint
    use strict arg -- none
    if codepoint == -1 then return -1
    return .RexxUnicode~codepointCharWidth(codepoint)


-- https://unicode.org/reports/tr29/
::method boundClass
    expose codepoint
    use strict arg -- none
    if codepoint == -1 then return "?"
    .RexxUnicode~codepointBoundClass(codepoint, >code)
    return code


-- https://unicode.org/reports/tr29/
::method boundClassName
    expose codepoint
    use strict arg -- none
    if codepoint == -1 then return "?"
    .RexxUnicode~codepointBoundClass(codepoint, , >label)
    return label


-- https://www.unicode.org/reports/tr44/#Derivation_InCB
::method indicConjunctBreak
    expose codepoint
    use strict arg -- none
    if codepoint == -1 then return "?"
    .RexxUnicode~codepointIndicConjunctBreak(codepoint, >code)
    return code


-- https://www.unicode.org/reports/tr44/#Derivation_InCB
::method indicConjunctBreakName
    expose codepoint
    use strict arg -- none
    if codepoint == -1 then return "?"
    .RexxUnicode~codepointIndicConjunctBreak(codepoint, , >label)
    return label


-- Simple case mappings in UnicodeData.txt, returns a single codepoint
::method toLower
    expose codepoint
    use strict arg -- none
    if codepoint == -1 then return -1
    return .RexxUnicode~codepointToLower(codepoint)


-- Full case mappings in SpecialCasing.txt, returns 1..n codepoints
::method toLowerFull
    expose codepoint
    use strict arg -- none
    if codepoint == -1 then return .array~of(-1)
    -- TODO
    return .array~of(-1)


-- Simple case mappings in UnicodeData.txt, returns a single codepoint
::method toUpper
    expose codepoint
    use strict arg -- none
    if codepoint == -1 then return -1
    return .RexxUnicode~codepointToUpper(codepoint)


-- Full case mappings in SpecialCasing.txt, returns 1..n codepoints
::method toUpperFull
    expose codepoint
    use strict arg -- none
    if codepoint == -1 then return .array~of(-1)
    -- TODO
    return .array~of(-1)


-- Simple case mappings in UnicodeData.txt, returns a single codepoint
::method toTitle
    expose codepoint
    use strict arg -- none
    if codepoint == -1 then return -1
    return .RexxUnicode~codepointToTitle(codepoint)


-- Full case mappings in SpecialCasing.txt, returns 1..n codepoints
::method toTitleFull
    expose codepoint
    use strict arg -- none
    if codepoint == -1 then return .array~of(-1)
    -- TODO
    return .array~of(-1)


::method isLower
    expose codepoint
    use strict arg -- none
    if codepoint == -1 then return .false
    return .RexxUnicode~codepointIsLower(codepoint)


::method isUpper
    expose codepoint
    use strict arg -- none
    if codepoint == -1 then return .false
    return .RexxUnicode~codepointIsUpper(codepoint)


::method properties class
    -- TODO: align with UCD PropertyAliases.txt
    -- https://unicode-org.github.io/icu/userguide/strings/properties.html
    expose properties
    if \ var("properties") then properties =,
        "bidiClass",,
        "bidiClassName",,
        "bidiMirrored",,
        "boundClass",,
        "boundClassName",,
        "category",,
        "categoryName",,
        "charWidth",,
        "codepoint",,
        "combiningClass",,
        "combiningClassName",,
        "combiningClassValue",,
        "controlBoundary",,
        "decompositionTypeName",,
        "decompositionType",,
        "eastAsianWidthIsAmbiguous",,
        "extendedName",,
        "ignorable",,
        "indicConjunctBreak",,
        "isLower",,
        "isUpper",,
        "name",,
        "nameAlias",,
        "toLower",,
        /*"toLowerFull"*/,
        "toTitle",,
        /*"toTitleFull"*/,
        "toUpper",,
        /*"toUpperFull"*/,
        "UTF8"
    return properties


::method propertiesSpecialDisplay class private
    expose specialDisplay
    use strict arg -- none
    if \ var("specialDisplay") then do
        specialDisplay = .StringTable~new
        specialDisplay["codepoint"] =   "value = .RexxUnicode~D2U(self~codepoint)"
        specialDisplay["toLower"] =     "value = .RexxUnicode~D2U(self~toLower)"
        -- specialDisplay["toLowerFull"=   "value = .RexxUnicode~?2U(self~toLowerFull)"
        specialDisplay["toTitle"] =     "value = .RexxUnicode~D2U(self~toTitle)"
        -- specialDisplay["toTitleFull"] = "value = .RexxUnicode~D2U(self~toTitleFull)"
        specialDisplay["toUpper"] =     "value = .RexxUnicode~D2U(self~toUpper)"
        -- specialDisplay["toUpperFull"] = "value = .RexxUnicode~D2U(self~toUpperFull)"
        specialDisplay["UTF8"] =        "value = escapedHexdigits(self~UTF8~c2x)"
    end
    return specialDisplay


::method info
    use strict arg -- none
    properties = .StringTable~new
    do property over self~class~properties
        specialDisplay = self~class~propertiesSpecialDisplay[property]
        if .nil \== specialDisplay then interpret specialDisplay
        else value = self~send(property)
        properties~put(value, property)
    end
    return properties


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
        -- Don't activate next check. TUTOR can require ICU4ooRexx, the package will be different.
        -- if ICU4ooRexxPackage \== package then signal registered_different_package
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

    registered_different_package: raise syntax 48.900 array("ICU4ooRexx is already registered with a different package")
    class_not_found:              raise syntax 48.900 array("Class" quoted(className) "not found")
    version_requirement:          raise syntax 48.900 array("The minimum required ICU4ooRexx version is" versionRequirement || "; the current version is" ICU4ooRexx~version)
    method_not_found:             raise syntax 48.900 array("The ICU4ooRexx class does not understand the message" quoted(methodName))


::method assertICU4ooRexxIsRegistered class
    use strict arg raiseError=.true
    if self~ICU4ooRexxIsRegistered then return .true
    if \ raiseError then return .false
    raise syntax 48.900 array("The ICU4ooRexx class is not registered")


/*******************************************************************************
Other helpers
*******************************************************************************/

::routine quoted public -- keep it public, used in demos
    use strict arg string, quote='"'
    string = string~string -- can be called for any object, get its string representation
    return quote || string~changeStr(quote, quote||quote)  || quote


::routine escapedHexdigits private
    use strict arg hexdigits
    -- The 0xNNNN notation is easier to read,
    -- but the \x{NNNN} can be unescaped and supports an arbitrary number of N.
    if hexdigits == "" then return ""
    if hexdigits~length == 2 then return "\x" || hexdigits
    return "\x{" || hexdigits || "}"

    -- CAUTION: this notation is not adapted to hexdigits containing spaces!
    return "0x" || hexdigits

    -- Don't use the Rexx "XXXX"x notation because less readable when embedded in a string
    return quoted(hexdigits) || "x"


::routine singularPlural private
    use strict arg number, singularText="", pluralText="s"
    if abs(number) <= 1 then return singularText
    return pluralText


::routine singularPluralCount private
    use strict arg number, singularText="", pluralText="s"
    return number singularPlural(number, singularText, pluralText)


/*******************************************************************************
Subclasses of .String are not allowed
*******************************************************************************/

/*
Encountered a strange problem in ooRexxShell when TUTOR is loaded and active.

.RexxUnicode~c2x("€") returned 'E282AC2020' when TUTOR was active, and 'E282AC' when TUTOR was not active.
This problem is not limited to TUTOR; it can occur with any object that overrides ~length or ~substr.

The root cause lies in the .RexxUnicodeCodepointSupplier~decode method, which calls utf8DecodeCodepoint.
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

/*
An other problem in ooRexxShell when TUTOR is loaded and active.
    s = "a👨‍👩‍👧b"
    .RexxUnicode~createIndexers(s, >c, >g, >e)
    c=                                              -- [ 1, 2, 6, 9, 13, 16, 20]
    g=                                              -- [ 1, 2, 20]
    .RexxUnicode~graphemeAtIndexG(s, 3, g)=         -- Value of FOR expression in DO or LOOP instruction must be zero or a positive whole number; found "-16".

Call stack:
    TUTOR/Unicode.cls
       734 *-* Do i = n For length
    rxunicode.cls
       613 *-* return string~substr(absStartB, absNextB - absStartB)
    oorexxshell.rex
       557 *-* result = .RexxUnicode~graphemeAtIndexG((U:Default(s)), (U:Default(3)), (U:Default(g))) ;

graphemeAtIndexG(s, 3, g) calculates
    startB = graphemeIndexB(s, 3, g)    --> g[3] == 20
    nextB = graphemeIndexB(s, 4, g)     --> string~length + 1, which should be 21, but `s` is a Text object: TUTOR returns 4.
    absStartB = abs(startB)             --> 20
    absNextB = abs(nextB)               --> 4, instead of 21
    return s~substr(absStartB, absNextB - absStartB) --> should be substr(s, 20, 1), but becomes substr(s, 20, -16)
*/

-- Ensure that a string is a .String instance, not an instance of a subclass of .String.
-- No attempt is made to obtain a string from the object.
-- This check is intended to detect accidental mixing of Unicode-aware objects with byte strings.
::routine requestBaseString private
    use strict arg object, argumentName="string"

    -- Permissive
    -- return object~string

    -- Strict
    if object~class == .String then return object
    raise syntax 88.900 array ("Argument" argumentName "class: expected String, found" object~class~id)
    --raise syntax 88.914 array (argumentName, .String~id)
