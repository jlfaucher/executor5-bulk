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
    RexxInteger *utf8DecodePreviousCodepoint(RexxString *string, RexxInteger *indexB, VariableReference *refSizeB, VariableReference *refErrorCode, VariableReference *refErrorMsg);
    MutableBuffer *utf8EncodeCodepoint(RexxInteger *rexxCodepoint, MutableBuffer *destination, VariableReference *refSizeB);

    RexxString *utf8Transform(RexxObject **arguments, size_t argCount);
    // use strict arg string, casefold = .false, lump= .false, nlf = 0, normalization = 0, stripCC = .false, stripIgnorable= .false, stripMark = .false, stripNA = .false

    RexxInteger *graphemeBreak(ArrayClass *array); // deprecated
    RexxInteger *graphemeBreak3(RexxInteger *rexxCodepoint1, RexxInteger *rexxCodepoint2, VariableReference *refState);

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
*/

/*******************************************************************************
Constants
*******************************************************************************/

::constant maxCodepoint 1114111

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
    return indexer~info


::method stringIsASCII class
    use strict arg string
    string = requestBaseString(string)
    -- No need to optimize, will be replaced by a native method .string~isASCII
    ASCII = xrange("00"x, "7F"x)
    return string~verify(ASCII) == 0


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

    not_wholenumber: raise syntax 88.900 array ("Code point must be in the range 0 to" self~maxCodepoint "("self~D2U(self~maxCodepoint)"); found" quoted(codepoint))
    error_range: raise syntax 88.900 array ("Invalid code point:" codepoint"; allowed range is 0 to" self~maxCodepoint "("self~D2U(self~maxCodepoint)")")


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


::method codepointUnicodeEscapeNotation class
    use strict arg codepoint, buffer=.nil
    self~checkCodepoint(codepoint, /*minusOneAllowed*/ .true)
    if .nil == buffer then do
        if codepoint < 0 then return "\UFFFFFFFF"
        if codepoint <= 65535 then return "\u" || codepoint~d2x~right(4, "0")
        return "\U" || codepoint~d2x~right(8, "0")
    end
    if codepoint < 0 then buffer~append("\UFFFFFFFF")
    else if codepoint <= 65535 then buffer~append("\u", codepoint~d2x~right(4, "0"))
    else buffer~append("\U", codepoint~d2x~right(8, "0"))
    return buffer


::method codepointIsPrintable class
    use strict arg codepoint
    self~checkCodepoint(codepoint, /*minusOneAllowed*/ .true)

    if codepoint < 0 then return .false

    /*
    The count of widths per category is calculated for Unicode 17.

    categoryWidth. (49 items)
    'Cc.0' :  65
    'Cf.0' :  169       -- 169 Cf codepoints have a width of 0
    'Cf.1' :  1         --   1 Cf codepoint has a width of 1
    'Cn.1' :  814730
    'Co.1' :  137468
    'Cs.0' :  2048
    'Ll.1' :  2257
    'Ll.2' :  26
    'Lm.1' :  379
    'Lm.2' :  31
    'Lo.1' :  17013
    'Lo.2' :  124049
    'Lt.1' :  31
    'Lu.1' :  1860
    'Lu.2' :  26
    'Mc.0' :  467
    'Mc.2' :  4
    'Me.0' :  13
    'Mn.0' :  2059
    'Nd.1' :  760
    'Nd.2' :  10
    'Nl.1' :  223
    'Nl.2' :  16
    'No.1' :  838
    'No.2' :  77
    'Pc.1' :  4
    'Pc.2' :  6
    'Pd.1' :  19
    'Pd.2' :  8
    'Pe.1' :  48
    'Pe.2' :  29
    'Pf.1' :  10
    'Pi.1' :  12
    'Po.1' :  592
    'Po.2' :  49
    'Ps.1' :  51
    'Ps.2' :  28
    'Sc.1' :  58
    'Sc.2' :  6
    'Sk.1' :  115
    'Sk.2' :  10
    'Sm.1' :  947
    'Sm.2' :  13
    'So.1' :  5154
    'So.2' :  2314
    'Zl.0' :  1
    'Zp.0' :  1
    'Zs.1' :  16
    'Zs.2' :  1

    Categories having codepoints of width 0 that are considered not printable:
        Cc  Control
        Cf  Format
        Cs  Surrogate
        Zl  Line_Separator          (to avoid line breaks in single-line output)
        Zp  Paragraph_Separator     (to avoid line breaks in single-line output)

    Categories containing codepoints of width 0 that are nevertheless considered
    printable because some fonts provide visible glyphs for them:
        Mc  Spacing_Mark
        Me  Enclosing_Mark
        Mn  Nonspacing_Mark

    */

    -- Mc, Me and Mn codepoints are considered printable,
    -- even though they have a display width of 0.
    -- Some fonts provide visible glyphs for them.
    self~codepointCategory(codepoint, >categoryCode)
    if "Mc Me Mn"~contains(categoryCode) then return .true

    -- Codepoints having a display width of 0 are considered non-printable
    width = self~codepointCharWidth(codepoint)
    if width == 0 then return .false

    -- Unassigned (Cn) and private-use (Co) codepoints are considered non-printable,
    -- even though they have a display width of 1.
    if "Cn Co"~contains(categoryCode) then return .false

    -- Other codepoints are considered printable.
    return .true


/*
Returns a printable string representation of this codepoint.
Printable codepoints are returned as their UTF-8 character representation;
non-printable codepoints are returned as a Unicode escape notation.
*/
::method codepointPrintableString class
    use strict arg codepoint, buffer=.nil
    self~checkCodepoint(codepoint, /*minusOneAllowed*/ .true)
    returnBuffer = .nil <> buffer
    if .nil == buffer then buffer = .MutableBuffer~new
    if self~codepointIsPrintable(codepoint) then do
        self~utf8EncodeCodepoint(codepoint, buffer)
    end
    else do
        -- I think it's better to use the \u or \U notation, instead of the \x{ notation
        self~codepointUnicodeEscapeNotation(codepoint, buffer)
    end
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

-- Gets a codepoint from a character name
-- Private helper for .RexxUnicode~stringUnescape and .RexxUnicode~codepoint
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
    indexer = .RexxUnicodeStringIndexer~new(identifier, /*codepointStorageLimit:*/ 1, /*graphemeStorageLimit:*/ 0, /*errorStorageLimit:*/ 0)
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
C2U C2X D2U U2C U2D
*******************************************************************************/

-- Returns an ASCII string containing a sequence of U+XXXX...
::method C2U class
    use strict arg string, buffer=.nil
    string = requestBaseString(string)
    codepoints = .RexxUnicodeCodepointSupplier~new(string)
    returnBuffer = .nil <> buffer
    if .nil == buffer then buffer = .MutableBuffer~new
    first = .true
    do while codepoints~available
        if \first then buffer~append(" ")
        first = .false
        codepoint = codepoints~item -- get the codepoint as a whole number
        UPlusCodepoint = self~D2U(codepoint, buffer)
        codepoints~next
    end
    if returnBuffer then return buffer
    return buffer~string


-- Specialized C2X: Inserts a space between the codepoints
::method C2X class
    use strict arg string, buffer=.nil
    string = requestBaseString(string)
    codepoints = .RexxUnicodeCodepointSupplier~new(string)
    returnBuffer = .nil <> buffer
    if .nil == buffer then buffer = .MutableBuffer~new
    first = .true
    do while codepoints~available
        if \first then buffer~append(" ")
        first = .false
        codepoint = codepoints~item("b") -- "b": get the byte sequence as a string
        buffer~append(codepoint~c2x)
        codepoints~next
    end
    if returnBuffer then return buffer
    return buffer~string


/*
Appendix A - Notational conventions
In running text, an individual Unicode code point is expressed as U+n, where n is four to
six hexadecimal digits, using the digits 0–9 and uppercase letters A–F (for 10 through 15,
respectively). Leading zeros are omitted, unless the code point would have fewer than four
hexadecimal digits—for example, U+0001, U+0012, U+0123, U+1234, U+12345, U+102345
*/
::method D2U class
    use strict arg codepoint, buffer=.nil
    self~checkCodepoint(codepoint, /*minusOneAllowed*/ .true)
    if .nil == buffer then do
        if codepoint < 0 then return "U+FFFFFF" -- in case of error (invalid)
        if codepoint < 65536 then return "U+" || codepoint~d2x(4)
        return "U+" || codepoint~d2x
    end
    else do
        if codepoint < 0 then buffer~append("U+FFFFFF") -- in case of error (invalid)
        else do
            buffer~append("U+")
            if codepoint < 65536 then buffer~append(codepoint~d2x(4))
            else buffer~append(codepoint~d2x)
        end
        return buffer
    end


/*
Encodes an ASCII string of the form "U+XXXX.. U+XXXX.. ..." into a UTF-8 string.
The only valid separators are one or more spaces.
If a buffer is passed, the decoded characters are appended to it.
The update is atomic: either all decoded characters are appended, or the buffer is left unchanged.
If an error occurs, any characters appended during this call are discarded.
*/
::method U2C class
    use strict arg UPlusCodepoints, buffer=.nil
    UPlusCodepoints = requestBaseString(UPlusCodepoints, "UPlusCodepoints")
    returnBuffer = .nil <> buffer
    if .nil == buffer then buffer = .MutableBuffer~new
    bufferInitialLength = buffer~length
    pos = 1
    do while pos <= UPlusCodepoints~length
        pos = UPlusCodepoints~verify(" ", "N", pos)
        if pos == 0 then leave -- end of UPlusCodepoints string
        codepoint = self~U2D(UPlusCodepoints, pos, /*raiseError*/ .false, >length)
        if codepoint == -1 then do
            -- U+ decoding error, discard the characters appended so far
            buffer~delete(bufferInitialLength + 1)
            -- redo the decoding in mode raise error
            codepoint = self~U2D(UPlusCodepoints, pos, /*raiseError*/ .true, >length)
            -- no return here
        end
        -- utf8proc implementation note:
        -- Note: we allow encoding 0xd800-0xdfff here, so as not to change
        -- the API, however, these are actually invalid in UTF-8
        self~utf8EncodeCodepoint(codepoint, buffer, >size)
        if size == 0 then do
            -- UTF-8 encoding error, discard the characters appended so far
            buffer~delete(bufferInitialLength + 1)
            signal encoding_error
        end
        pos += length
    end
    if returnBuffer then return buffer
    return buffer~string

    encoding_error: raise syntax 98.900 array ("Cannot UTF-8 encode code point" UPlusCodepoints~substr(pos, length))


/*
Converts "U+XXXX.." to a decimal value.
If refLength is passed by the caller, extra characters after the U+XXXX.. are allowed.
Otherwise UPlusCodepoint must be strictly "U+" followed by 4 to 6 hex digits.
The length of "U+XXXX.." is returned in refLength, allowing to skip it when parsing.
In case of error, returns -1 by default or raises an error if requested.
*/
::method U2D class
    argRefLength = arg() == 4 -- default value not supported for variable references
    if argRefLength then use strict arg UPlusCodepoint, start=1, raiseError=.false, >refLength
                    else use strict arg UPlusCodepoint, start=1, raiseError=.false
    UPlusCodepoint = requestBaseString(UPlusCodepoint, "UPlusCodepoint")

    refLength = 0
    if UPlusCodepoint~substr(start, 2)~caselessEquals("U+") then do
        endHexDigits = UPlusCodepoint~verify(.String~xdigit, "N", start + 2)
        if endHexDigits \== 0 then do
            if \argRefLength then signal invalid_u_plus -- extra characters not allowed
            refLength = endHexDigits - start -- including "U+"
            hexDigits = UPlusCodepoint~substr(start + 2, refLength - 2)
        end
        else do
            refLength = UPlusCodepoint~length -- including "U+"
            hexDigits = UPlusCodepoint~substr(start + 2)
        end
        if hexDigits~length >= 4, hexDigits~length <= 6 then return hexDigits~x2d
        signal invalid_u_plus -- invalid number of hexadecimal digits
    end

    invalid_u_plus:
    if raiseError then raise syntax 23.900 array("Expected U+ or u+ followed by 4..6 hex digits; found" quoted(UPlusCodepoint~substr(start)))
    return -1


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
::method StringUnescape class
    use strict arg string, buffer=.nil
    string = requestBaseString(string, "string")
    pos = string~pos("\")
    if pos == 0 then do
        -- don't create intermediate buffer if nothing to unescape
        if .nil == buffer then return string
        return buffer~~append(string)
    end
    returnBuffer = .nil \== buffer
    if .nil == buffer then buffer = .MutableBuffer~new(, string~length)
    from = 1
    do until pos == 0
        -- Here, pos is the index of a '\' in string.
        slashPos = pos -- used in error messages

        -- Append into buffer the part of the string
        -- from the end of the previous escape character (or 1)
        -- and the current escape character
        substring = string~substr(from, pos - from)
        buffer~append(substring)

        pos += 1 -- skip '\'
        character = string~subchar(pos) -- escape type letter '\', 'a', ..., 'N', 'u', 'U', 'x'
        pos += 1 -- skip the escape type letter
        select
            when character == "\" then buffer~append("\")   -- escaped \
            when character == "a" then buffer~append("07"x) -- audible bell (BEL)
            when character == "b" then buffer~append("08"x) -- backspace (BS)
            when character == "f" then buffer~append("0C"x) -- form feed (FF)
            when character == "n" then buffer~append("0A"x) -- linefeed (LF)
            when character == "r" then buffer~append("0D"x) -- carriage return (CR)
            when character == "t" then buffer~append("09"x) -- horizontal tab (HT)
            when character == "v" then buffer~append("0B"x) -- vertical tab (VT)

            when character == "N" then do
                -- \N{Unicode name}    Character name in the Unicode database
                if string~subchar(pos) \== "{" then signal expected_name
                pos += 1 -- skip {
                first = pos
                pos = string~pos("}", pos)
                if pos == 0 then signal expected_name
                length = pos - first
                if length == 0 then signal expected_name
                pos += 1 -- skip }
                name = string~substr(first, length)
                codepoint = self~codepointFromName(name)
                if codepoint == -1 then signal name_not_found
                self~utf8EncodeCodepoint(codepoint, buffer, >size)
                if size == 0 then signal name_encoding_error
            end

            when character == "u" then do
                -- \u{X..X}            Unicode scalar value, 1-8 hex digits
                -- \uXXXX              Unicode scalar value, exactly 4 hex digits
                if string~subchar(pos) == "{" then do
                    pos += 1 -- skip {
                    first = pos
                    pos = string~pos("}", pos)
                    if pos == 0 then signal expected_4_or_1_8_hexdigits
                    length = pos - first
                    if length == 0 then signal expected_4_or_1_8_hexdigits
                    pos += 1 -- skip }
                    hexdigits = string~substr(first, length)
                    hexdigits = hexdigits~space(0) -- remove all spaces
                    hexdigits = hexdigits~strip("L", "0") -- remove leading zeros
                    if hexdigits~length == 0 | hexdigits~length > 8 then signal expected_4_or_1_8_hexdigits
                    if \hexdigits~datatype("X") then signal expected_4_or_1_8_hexdigits
                    numeric digits 10 -- to support FFFFFFFF, even if way too big for a codepoint
                    codepoint = hexdigits~x2d
                    self~utf8EncodeCodepoint(codepoint, buffer, >size)
                    if size == 0 then signal encoding_1_8_hexdigits_error
                end

                else do
                    -- \uXXXX
                    length = min(4, string~length - pos + 1)
                    if length \== 4 then signal expected_4_or_1_8_hexdigits
                    hexdigits = string~substr(pos, 4)
                    if \hexdigits~datatype("X") then signal expected_4_or_1_8_hexdigits
                    codepoint = hexdigits~x2d
                    self~utf8EncodeCodepoint(codepoint, buffer, >size)
                    if size == 0 then signal encoding_1_4_hexdigits_error
                    pos += 4
                end
            end

            when character == "U" then do
                -- \UXXXXXXXX          Unicode scalar value, exactly 8 hex digits
                length = min(8, string~length - pos + 1)
                if length \== 8 then signal expected_8_hexdigits
                hexdigits = string~substr(pos, 8)
                if \hexdigits~datatype("X") then signal expected_8_hexdigits
                numeric digits 10 -- to support FFFFFFFF, even if way too big for a codepoint
                codepoint = hexdigits~x2d
                self~utf8EncodeCodepoint(codepoint, buffer, >size)
                if size == 0 then signal encoding_1_8_hexdigits_error
                pos += 8
            end

            when character == "x" then do
                -- \x{X..X}         Arbitrary number of hex digits
                -- \xXX             1 byte, exactly 2 hex digits
                if string~subchar(pos) == "{" then do
                    pos += 1 -- skip {
                    first = pos
                    pos = string~pos("}", pos)
                    if pos == 0 then signal expected_2_or_1_n_hexdigits
                    length = pos - first
                    if length == 0 then signal expected_2_or_1_n_hexdigits
                    pos += 1 -- skip }
                    hexdigits = string~substr(first, length)
                    if \hexdigits~datatype("X") then signal expected_2_or_1_n_hexdigits
                    bytes = hexdigits~x2c -- 1..n bytes
                    buffer~append(bytes)
                end

                else do
                    -- \xXX
                    length = min(2, string~length - pos + 1)
                    if length \== 2 then signal expected_2_or_1_n_hexdigits
                    hexdigits = string~substr(pos, 2)
                    if \hexdigits~datatype("X") then signal expected_2_or_1_n_hexdigits
                    bytes = hexdigits~x2c -- 2 bytes
                    buffer~append(bytes)
                    pos += 2
                end

            end

            otherwise signal invalid_escape_character
        end
        from = pos
        pos = string~pos("\", from)
    end
    buffer~append(string~substr(from))

    if returnBuffer then return buffer
    return buffer~string

    invalid_escape_character:           raise syntax 23.900 array("Invalid escape character" quoted("\"character, "'") "at position" slashPos)

    expected_name:                      raise syntax 23.900 array("Expected {a Unicode character name} after \"character "at position" slashPos)
    name_not_found:                     additional = ""
                                        if \ self~ICU4ooRexxIsRegistered then additional = " (ICU4ooRexx not loaded)"
                                        raise syntax 98.900 array("Name" quoted(name) "at position" slashPos "not found" || additional)

    -- not used, to delete
    /*
    expected_1_8_hexdigits:             raise syntax 23.900 array("Expected {1..8 hex digits} after \"character "at position" slashPos)
    expected_2_hexdigits:               raise syntax 23.900 array("Expected 2 hex digits after \"character "at position" slashPos)
    expected_4_hexdigits:               raise syntax 23.900 array("Expected 4 hex digits after \"character "at position" slashPos)
    expected_1_n_hexdigits:             raise syntax 23.900 array("Expected {1..n hex digits} after \"character "at position" slashPos)
    */

    expected_8_hexdigits:               raise syntax 23.900 array("Expected 8 hex digits after \"character "at position" slashPos)
    expected_4_or_1_8_hexdigits:        raise syntax 23.900 array("Expected either 4 hex digits or {1..8 hex digits} after \"character "at position" slashPos)
    expected_2_or_1_n_hexdigits:        raise syntax 23.900 array("Expected either 2 hex digits or {1..n hex digits (whitespace allowed)} after \"character "at position" slashPos)

    name_encoding_error:                raise syntax 22.900 array ("Cannot UTF-8 encode code point" codepoint "\"character"{"name"} at position" slashPos)
    encoding_1_8_hexdigits_error:       raise syntax 22.900 array ("Cannot UTF-8 encode code point" codepoint "\"character"{"hexdigits"} at position" slashPos)
    encoding_1_4_hexdigits_error:       raise syntax 22.900 array ("Cannot UTF-8 encode code point" codepoint "\"character || hexdigits "at position" slashPos)


::method stringEscape class
    use strict arg string, buffer=.nil
    string = requestBaseString(string, "string")

    returnBuffer = .nil \== buffer
    if .nil == buffer then buffer = .MutableBuffer~new(, string~length)

    codepoints = .RexxUnicodeCodepointSupplier~new(string)
    do while codepoints~available
        codepoint = codepoints~item -- whole number
        bytes = codepoints~item("b") -- byte sequence as string
        error = codepoints~item("e")
        if error \== "" then do
            xbytes = bytes~c2x
            if xbytes~length == 2 then buffer~append("\x", xbytes)
                                  else buffer~append("\x{", xbytes, "}")
        end
        else do
            select case codepoint
                when 7 then buffer~append("\a")
                when 8 then buffer~append("\b")
                when 9 then buffer~append("\t")
                when 10 then buffer~append("\n")
                when 11 then buffer~append("\v")
                when 12 then buffer~append("\f")
                when 13 then buffer~append("\r")
                when 92 then buffer~append("\\")
                otherwise .RexxUnicode~codepointPrintableString(codepoint, buffer)
            end
        end
        codepoints~next
    end

    if returnBuffer then return buffer
    return buffer~string


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
    expose string indexB indexG -
           /* coroutine internal state */ decodeNextStep

    use strict arg string, defaultIndexType="", defaultItemType=""
    string = requestBaseString(string, "string")
    self~setDefaultIndexType(defaultIndexType)
    self~setDefaultItemType(defaultItemType)

    empty = .array~new(0) -- No need to create a copy, the string is immutable
    self~init:super(empty, empty)

    indexB = 1  -- current codepoint byte position
    indexG = 1
    decodeNextStep = "START"
    self~decode


/*
This method is called from ~init and from ~next.
In both cases, nextGraphemeIndexB is set to .nil.

This method is convoluted! I had difficulties to implement it...
Using a coroutine design helped me to finalize the implementation.
I keep the logic of .RexxUnicodeStringIndexer~init, but with several entry/exit.
Maybe it can be simplified...
*/
::method decode private
    expose string indexB grapheme graphemeIndexB nextGraphemeIndexB error nextError -
           /* coroutine internal state */ -
           decodeNextStep previousCodepoint graphemeBreakState -
           codepoint sizeB errorCode errorMsg
    use strict arg -- none

    -- coroutine
    signal value decodeNextStep

    START:
    graphemeIndexB = .nil
    nextGraphemeIndexB = .nil
    previousCodepoint = -1
    graphemeBreakState = 0
    error = ""
    nextError = ""

    forever:
        if .nil \== nextGraphemeIndexB then do
            grapheme = string~substr(abs(graphemeIndexB), abs(nextGraphemeIndexB) - abs(graphemeIndexB))
            decodeNextStep = "NEXT_CODEPOINT"
            return -- grapheme, graphemeIndexB and error ready for use
            -- on reentry from ~next, graphemeIndexB == nextGraphemeIndexB and nextGraphemeIndexB == .nil
        end

        NEXT_CODEPOINT:
        -- here, nextGraphemeIndexB is necessarily .nil
        codepoint = .RexxUnicode~utf8DecodeCodepoint(string, indexB, >sizeB, >errorCode, >errorMsg)
        if sizeB == 0 then signal end_of_string
        if sizeB < 0 then signal error

        if previousCodepoint < 0 then call appendGraphemeIndex indexB -- First codepoint or Error recovery
        else do
            graphemeBreak  = .RexxUnicode~graphemeBreak3(previousCodepoint, codepoint, >graphemeBreakState)
            if graphemeBreak then call appendGraphemeIndex indexB
        end
        previousCodepoint = codepoint
        indexB += sizeB
        signal forever

    END_OF_STRING:
        -- here, nextGraphemeIndexB is necessarily .nil
        -- but graphemeIndexB can be not .nil or .nil
        if .nil \== graphemeIndexB then grapheme = string~substr(abs(graphemeIndexB))
        else grapheme = .nil -- ~next will no longer reenter because ~available will return .false
        decodeNextStep = "END_OF_STRING"
        return -- grapheme, graphemeIndexB and error ready for use
        -- on reentry from ~next, graphemeIndexB == .nil and nextGraphemeIndexB == .nil

    error:
        -- here, nextGraphemeIndexB is necessarily .nil
        errorMsg = "start byte position" indexB ":" errorMsg
        call appendGraphemeIndex -indexB, errorMsg -- same convention as .RexxUnicodeStringIndexer; a negative byte index indicates an invalid byte sequence
        graphemeBreakState = 0 -- reset the extended grapheme state
        previousCodepoint = codepoint
        indexB += abs(sizeB) -- skip the invalid bytes
        signal forever

    appendGraphemeIndex:
        use strict arg newIndex, newError=""

        if graphemeIndexB~isNil then do
            graphemeIndexB = newIndex
            error = newError
        end
        else if nextGraphemeIndexB~isNil then do
            nextGraphemeIndexB = newIndex
            nextError = newError
        end
        else do
            -- shift
            graphemeIndexB = nextGraphemeIndexB
            nextGraphemeIndexB = newIndex
            error = nextError
            nextError = newError
        end

        return


::method next
    expose error indexG graphemeIndexB nextError nextGraphemeIndexB

    use strict arg -- none
    if self~available then do
        indexG += 1
        graphemeIndexB = nextGraphemeIndexB
        nextGraphemeIndexB = .nil
        error = nextError
        nextError = ""
        self~decode
    end
    else raise syntax 93.937 -- No more supplier items available


/******************************************************************************/
::class "RexxUnicodeReverseGraphemeSupplier" public subclass Supplier -
                                                    inherit GraphemeSupplierHelper
/******************************************************************************/

-- Reverse grapheme supplier for UTF-8 strings without pre-indexing.
-- Initial code by Claude Sonnet 5 medium, reviewed and adapted.

/*
I'm assuming your .RexxUnicode class exposes (or you'll add, alongside graphemeBreak)
a handful of property-query primitives, since I don't have visibility into your existing C-layer API surface for these:

AI requirement:
    .RexxUnicode~graphemeClusterBreakProperty(codepoint) → one of 'CR' 'LF' 'CONTROL' 'EXTEND' 'ZWJ' 'REGIONAL_INDICATOR' 'PREPEND' 'SPACINGMARK' 'L' 'V' 'T' 'LV' 'LVT' 'OTHER'
replaced by:
    .RexxUnicode~codepointBoundClass(codepoint, >boundClass)
    boundClass values to test (on the right, if different):
        'CR'
        'LF'
        'CONTROL'               'CN'
        'EXTEND'                'EX'
        'ZWJ'
        'REGIONAL_INDICATOR'    'RI'
        'PREPEND'               'PP'
        'SPACINGMARK'           'SM'
        'L'
        'V'
        'T'
        'LV'
        'LVT'
        'OTHER'                 'XX'

AI requirement:
    .RexxUnicode~isExtendedPictographic(codepoint) → boolean
replaced by:
    .RexxUnicode~codepointBoundClass(codepoint, >boundClass)
    boundClass == "EP" | boundClass == "EZWG"
        "EP",       "Extended_Pictographic"
        "EZWG",     "E_ZWG"

AI requirement:
    .RexxUnicode~isInCBConsonant(codepoint) → boolean
replaced by:
    .RexxUnicodeServices~codepointIndicConjunctBreak(codepoint, >indicConjunctBreak)
    indicConjunctBreak == "Consonant"

AI requirement:
    .RexxUnicode~isInCBLinker(codepoint) → boolean
replaced by:
    .RexxUnicodeServices~codepointIndicConjunctBreak(codepoint, >indicConjunctBreak)
    indicConjunctBreak == "Linker"
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
A few things worth flagging:

JLF:
There is no countPrecedingRegionalIndicators in the code.
Is it precedingRIRunLengthIsOdd?

countPrecedingRegionalIndicators intentionally counts starting from the scalar
itself, not from scalar1's position — it's called with pos1, the end of scalar1
(i.e. cursor before the step), so its first backward decode re-reads scalar1
itself, folding it into the count. That matches the definition:
parity is of "how many RI's including this one, back to the start of the run."

Error handling mirrors your forward supplier exactly: a maximal ill-formed subpart
is always its own grapheme, isolated from neighbors on both sides, and the three
lookback helpers all treat hitting an error boundary as "run terminates, no match"
— consistent with your forward code's graphemeBreakState = 0 reset on error.

This is a plain sketch, not benchmarked — like Swift's version,
precedingRIRunLengthIsOdd is O(run length) rather than O(1),
so a string that's pathologically almost-all REGIONAL_INDICATOR scalars would
make backward iteration quadratic over that stretch, same as forward would be
fast but backward wouldn't. That's an accepted, documented trade-off in the
reference implementation, not a bug to chase down — but worth a comment in your
code so a future reader (including future-you) doesn't mistake it for an oversight.
*/

-- Using a mixin class requires giving access to these (normally) private variables.
::attribute error get package           -- "" if no error, otherwise error description
::attribute grapheme get package        -- a string of length 1..n, or .nil if no more grapheme available
::attribute graphemeIndexB get package  -- 1..length+1, counted from the beginning of the string, negative if the grapheme byte sequence is invalid
::attribute indexG get package          -- 1-based, counted from the end of the string

::method init
    expose string indexB indexG error
    use strict arg string, defaultIndexType="", defaultItemType=""
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
    expose string indexB grapheme graphemeIndexB error
    use strict arg -- none

    error = ""

    -- ok, it's an alternative way to detect the beginning of string,
    -- the other way being to test if sizeB == 0 after utf8DecodePreviousCodepoint.
    if indexB <= 1 then do
        grapheme = .nil
        graphemeIndexB = .nil
        return
    end

    codepoint2 = .RexxUnicode~utf8DecodePreviousCodepoint(string, indexB, >sizeB, >errorCode, >errorMsg)
    cursor = indexB - abs(sizeB)      -- start of the rightmost scalar so far

    -- An ill-formed byte's maximal subpart is always its own grapheme cluster
    -- (mirrors the forward supplier's error handling: no merging across an error).
    if sizeB < 0 then do
        graphemeIndexB = -cursor -- same convention as .RexxUnicodeStringIndexer; a negative byte index indicates an invalid byte sequence
        grapheme = string~substr(cursor, indexB - cursor)
        error = "start byte position" cursor ":" errorMsg
        return
    end

    -- prop2 = .RexxUnicode~graphemeClusterBreakProperty(codepoint2)
    .RexxUnicode~codepointBoundClass(codepoint2, >prop2)

    loop
        if cursor <= 1 then leave    -- start of text: always a boundary

        codepoint1 = .RexxUnicode~utf8DecodePreviousCodepoint(string, cursor, >sizeB1, >errorCode1, >errorMsg1)
        pos1 = cursor - abs(sizeB1)

        if sizeB1 < 0 then leave    -- adjacent error is always its own cluster: break here

        -- prop1 = .RexxUnicode~graphemeClusterBreakProperty(codepoint1)
        .RexxUnicode~codepointBoundClass(codepoint1, >prop1)

        if self~shouldBreakBackward(codepoint1, prop1, pos1, codepoint2, prop2) then leave

        -- No break: extend the grapheme leftward and keep going
        cursor = pos1
        codepoint2 = codepoint1
        prop2 = prop1
    end

    graphemeIndexB = cursor
    grapheme = string~substr(cursor, indexB - cursor)
    error = ""


-- Direct-from-table pairwise rules (GB3, GB6-GB9b), plus dispatch to the
-- three lookback-requiring rules (GB9c, GB11, GB12/13). Order matches rule
-- precedence in UAX #29 3.1.1: first match wins.
::method shouldBreakBackward private
    expose string
    use strict arg codepoint1, prop1, pos1, codepoint2, prop2

    -- GB3: do not break CR x LF
    if prop1 == "CR", prop2 == "LF" then return .false

    -- GB4: break after Control | CR | LF
    if prop1 == "CN" | prop1 == "CR" | prop1 == "LF" then return .true

    -- GB5: break before Control | CR | LF
    if prop2 == "CN" | prop2 == "CR" | prop2 == "LF" then return .true

    -- GB6/7/8: do not break within Hangul syllable sequences
    if prop1 == "L", (prop2 == "L" | prop2 == "V" | prop2 == "LV" | prop2 == "LVT") then return .false
    if (prop1 == "LV" | prop1 == "V"), (prop2 == "V" | prop2 == "T") then return .false
    if (prop1 == "LVT" | prop1 == "T"), prop2 == "T" then return .false

    -- GB9: do not break before Extend | ZWJ
    if prop2 == "EX" | prop2 == "ZWJ" then return .false

    -- GB9a: do not break before SpacingMark
    if prop2 == "SM" then return .false

    -- GB9b: do not break after Prepend
    if prop1 == "PP" then return .false

    -- GB11: Extended_Pictographic Extend* ZWJ x Extended_Pictographic
    if prop1 == "ZWJ", -
       (prop2 == "EP" | prop2 == "EZWG") /*.RexxUnicode~isExtendedPictographic(codepoint2)*/ then -
        return \self~isPictographicBeforeZwj(pos1)

    -- GB9c: InCB=Consonant [Extend|Linker]* Linker [Extend|Linker]* x InCB=Consonant
    .RexxUnicodeServices~codepointIndicConjunctBreak(codepoint2, >indicConjunctBreak2)
    if indicConjunctBreak2 == "Consonant" /*.RexxUnicode~isInCBConsonant(codepoint2)*/ , -
       (prop1 == "EX" | prop1 == "ZWJ"), -
       self~isInCBExtendOrLinker(codepoint1) then -
        return \self~isConsonantThroughLinkerRun(codepoint1, pos1)

    -- GB12/13: do not break within RI RI if an odd number of RI precede the break
    if prop1 == "RI", prop2 == "RI" then -
        return self~precedingRIRunLengthIsOdd(pos1)

    -- GB999: otherwise, break
    return .true


-- true if codepoint is InCB=Extend or InCB=Linker (i.e. Extend/ZWJ but not itself
-- a Consonant or Linker) -- same derivation Swift uses for _isInCBExtend.
::method isInCBExtendOrLinker private
    use strict arg codepoint
    .RexxUnicodeServices~codepointIndicConjunctBreak(codepoint, >indicConjunctBreak)
    if indicConjunctBreak == "Linker" /*.RexxUnicode~isInCBLinker(codepoint)*/ then return .true
    -- prop = .RexxUnicode~graphemeClusterBreakProperty(codepoint)
    .RexxUnicode~codepointBoundClass(codepoint, >prop)
    if prop \== "EX", prop \== "ZWJ" then return .false
    -- return \.RexxUnicode~isInCBConsonant(codepoint)
    return indicConjunctBreak \== "Consonant"


-- GB11 lookback: walk left from the ZWJ (whose right boundary ends at zwjEndPos)
-- over an Extend* run to see whether an Extended_Pictographic started it.
::method isPictographicBeforeZwj private
    expose string
    use strict arg zwjEndPos
    pos = zwjEndPos
    loop
        if pos <= 1 then return .false
        cp = .RexxUnicode~utf8DecodePreviousCodepoint(string, pos, >sizeB)
        if sizeB < 0 then return .false          -- an error boundary can't be pictographic

        -- if .RexxUnicode~isExtendedPictographic(cp) then return .true
        .RexxUnicode~codepointBoundClass(cp, >boundClass)
        if boundClass == "EP" | boundClass == "EZWG" then return .true

        -- prop = .RexxUnicode~graphemeClusterBreakProperty(cp)
        .RexxUnicode~codepointBoundClass(cp, >prop)
        if prop \== "EX" then return .false   -- run ended without finding one
        pos = pos - abs(sizeB)
    end


-- GB9c lookback: walk left from the Extend/Linker run looking for an
-- InCB=Consonant, requiring at least one InCB=Linker en route. `firstCodepoint`
-- is codepoint1 itself (already confirmed Extend/Linker by the caller) and must
-- be checked for Linker-ness too -- it's part of the run, not something to skip
-- past before scanning starts.
/*
--------------------------------------------------------------------------------
Bug 2 — isConsonantThroughLinkerRun: doesn't actually require a Linker
    if indicConjunctBreak == "Consonant" then return .true

GB9c requires at least one InCB=Linker between the two consonants —
[Extend|Linker]* Linker [Extend|Linker]*, not just any run of Extend/Linker.
The current code returns success the moment it finds a Consonant behind the run,
without ever checking whether a Linker was actually part of that run.
So U+0915 U+0951 U+0915 (consonant, Extend, consonant — no Linker at all) walks
back from the Extend, immediately hits the first consonant, and reports
"yes, conjunct" — merging everything, when it should report "no" and let GB999
break there.

It needs a flag, set only when a genuine Linker is seen, and the success return
must be conditioned on it: seenLinker.

This is why your other GB9c cases (the "kshi" example and the
double-Extend-around-a-linker case) already passed despite the bug — they all
happen to contain a genuine Linker, so the missing check never got exercised.
Test #3 was specifically constructed as the negative case, and it's exactly the
one that catches it — good test design, that's precisely what it was for.

isPictographicBeforeZwj (GB11) doesn't have an analogous issue — it has no
"must have seen X" requirement, just "walk back over Extend* until pictographic
or non-Extend," so there's no missing-flag failure mode there, consistent with
none of your GB11 cases showing up in the failures.
*/
/*
--------------------------------------------------------------------------------
Bug 3 - "U+0915 U+094D U+0937 U+093F" regression
        a RexxUnicodeReverseGraphemeSupplier
         7 : 'U+0937 U+093F'
         1 : 'U+0915 U+094D'
should be
       a RexxUnicodeReverseGraphemeSupplier
         1 : 'U+0915 U+094D U+0937 U+093F'

it's in isConsonantThroughLinkerRun, and it's a boundary-condition bug rather
than a logic-inversion like the last two: the function starts scanning before
codepoint1, so it never actually checks whether codepoint1 itself is the Linker.

Trace through U+0915 U+094D U+0937 U+093F (KA, Virama, SSA, vowel sign I):
the backward walk reaches the pair codepoint1 = U+094D (the Linker),
codepoint2 = U+0937 (Consonant). isInCBExtendOrLinker(codepoint1) correctly
confirms U+094D qualifies, so the GB9c branch fires and calls:
    return \self~isConsonantThroughLinkerRun(pos1)

where pos1 is the position before U+094D — i.e., the scan starts by decoding
past the Linker and lands straight on U+0915 (Consonant), having never inspected
U+094D for Linker-ness at all. Since seenLinker is still .false at that point,
it returns .false → forces a break right where there shouldn't be one. That's
exactly the regression: 094D | 0937 splits.

The fix is to seed seenLinker from codepoint1 itself before scanning further
back, since codepoint1 is already known (by the caller) to be part of the run:
    add `firstCodepoint` argument and test its IndicConjunctBreak property.
    modify `shouldBreakBackward` to pass codepoint1.

Retracing the "kshi" case with this fix:
firstCodepoint = U+094D is checked first — Linker, so seenLinker = .true
immediately — then the loop decodes back to U+0915, sees Consonant, and returns
.true. No break. Matches forward.

And it doesn't reopen the negative test you caught last time (U+0915 U+0951
U+0915, no linker): there, firstCodepoint = U+0951 is Extend, not Linker, so
seenLinker stays .false going in, the loop then hits U+0915 (Consonant) and
correctly returns .false → break, as it should.

I also re-traced the two-Extend-around-a-linker case (U+0915 U+0951 U+094D
U+0951 U+0915) with this fix to make sure it doesn't regress — it still merges
into one grapheme, since the Linker gets picked up on the second call to
isConsonantThroughLinkerRun (this time seeded from a plain Extend codepoint,
seenLinker = .false initially, but the scan then walks past the actual U+094D
and sets it .true there before reaching the leading consonant).
*/
::method isConsonantThroughLinkerRun private
    expose string
    use strict arg firstCodepoint, runEndPos

    .RexxUnicodeServices~codepointIndicConjunctBreak(firstCodepoint, >indicConjunctBreak)
    seenLinker = (indicConjunctBreak == "Linker")

    pos = runEndPos
    loop
        if pos <= 1 then return .false
        cp = .RexxUnicode~utf8DecodePreviousCodepoint(string, pos, >sizeB)
        if sizeB < 0 then return .false

        -- if .RexxUnicode~isInCBConsonant(cp) then return .true
        .RexxUnicodeServices~codepointIndicConjunctBreak(cp, >indicConjunctBreak)
        if indicConjunctBreak == "Consonant" then return seenLinker   -- <-- was unconditional .true

        if indicConjunctBreak == "Linker" then seenLinker = .true
        else if \self~isInCBExtendOrLinker(cp) then return .false     -- run ended, no consonant

        pos = pos - abs(sizeB)
    end


-- GB12/13 lookback: count the contiguous run of Regional_Indicator scalars
-- ending at (and including) the scalar whose right edge is riEndPos.
-- No-break holds iff that count is odd.
--
-- precedingRIRunLengthIsOdd is O(run length) rather than O(1),
-- so a string that's pathologically almost-all REGIONAL_INDICATOR scalars would
-- make backward iteration quadratic over that stretch.
/*
--------------------------------------------------------------------------------
Bug 1 — precedingRIRunLengthIsOdd: the loop condition is inverted
    if boundClass == "RI" then leave

This says "stop counting as soon as you find an RI" — backwards. The whole point
of the loop is to count the run of RI's; it should stop when it finds something
that isn't RI (that's where the run ends), and increment the count while it is RI.

As written, the loop almost always terminates on its very first decode (since
that first decode very often is an RI, given it's being called at an RI/RI
junction), so count stays near 0 regardless of the actual run length — which is
why every RI test merged the whole run into a single grapheme instead of pairing
it up: the parity check was comparing against a count that never reflected the
real run.

The rest of the RI logic is correct as-is, including the (unnegated) call site
return self~precedingRIRunLengthIsOdd(pos1) — I checked that one carefully since
it looked suspicious at first, but the parity math works out: pos1 is scalar1's
start, so the count naturally excludes scalar1 itself, giving "how many RI
precede scalar1"; break-iff-that's-odd is the correct condition once the loop
itself is fixed. Tracing your test #3 (U+1F1EB U+1F1F7 U+1F1E9) through the
corrected version end-to-end reproduces the forward result exactly, pair (a,b)
then singleton c.
*/
::method precedingRIRunLengthIsOdd private
    expose string
    use strict arg riEndPos
    pos = riEndPos
    count = 0
    loop
        if pos <= 1 then leave
        cp = .RexxUnicode~utf8DecodePreviousCodepoint(string, pos, >sizeB)
        if sizeB < 0 then leave -- error
        -- if .RexxUnicode~graphemeClusterBreakProperty(cp) \== "REGIONAL_INDICATOR" then leave
        .RexxUnicode~codepointBoundClass(cp, >boundClass)
        if boundClass \== "RI" then leave        -- <-- was "==": run ends here
        count += 1
        pos = pos - abs(sizeB)
    end
    return count // 2 == 1


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

::attribute codepointStorageLimit get
::attribute codepointIndexes get        -- limitable storage
::attribute codepointCount get          -- count after full scan, not impacted by codepointStorageLimit
::attribute endCodepointStorageSize get
::attribute endCodepointIndexes get

::attribute graphemeStorageLimit get
::attribute graphemeIndexes get         -- limitable storage
::attribute graphemeCount get           -- count after full scan, not impacted by graphemeStorageLimit
::attribute endGraphemeStorageSize get
::attribute endGraphemeIndexes get

::attribute errorStorageLimit get
::attribute errors get                  -- limitable storage
::attribute errorCount get              -- count after full scan, not impacted by errorStorageLimit
::attribute endErrorStorageSize get
::attribute endErrors get

::method init
    expose string -
           codepointStorageLimit codepointIndexes codepointCount -
           endCodepointStorageSize endCodepointIndexes -
           graphemeStorageLimit graphemeIndexes lastGraphemeSuccessorIndex graphemeCount -
           endGraphemeStorageSize endGraphemeIndexes -
           errorStorageLimit errors errorCount -
           endErrorStorageSize endErrors

    use strict arg string, -
                   codepointStorageLimit = (.RexxInfo~internalMaxNumber), graphemeStorageLimit = (.RexxInfo~internalMaxNumber), errorStorageLimit = (.RexxInfo~internalMaxNumber), -
                   endCodepointStorageSize = 0, endGraphemeStorageSize = 0, endErrorStorageSize = 0
    string = requestBaseString(string)

    codepointIndexes = .array~new
    codepointCount = 0

    graphemeIndexes = .array~new
    graphemeCount = 0
    -- hidden index to let calculate the grapheme's length of the grapheme at graphemeStorageLimit.
    -- This index is not put in graphemeIndexes to keep graphemeIndexes's items <=  graphemeStorageLimit.
    lastGraphemeSuccessorIndex = string~length + 1

    errors = .array~new
    errorCount = 0


    endCodepointIndexes = .nil
    if endCodepointStorageSize > 0 then do
        -- if codepointStorageLimit == .RexxInfo~internalMaxNumber then raise syntax 88.900 array ("Specifying endCodepointStorageSize is allowed only if the regular storage is limited")
        endCodepointIndexes = .CircularQueue~new(endCodepointStorageSize)
    end

    endGraphemeIndexes = .nil
    if endGraphemeStorageSize > 0 then do
        -- if graphemeStorageLimit == .RexxInfo~internalMaxNumber then raise syntax 88.900 array ("Specifying endGraphemeStorageSize is allowed only if the regular storage is limited")
        endGraphemeIndexes = .CircularQueue~new(endGraphemeStorageSize)
    end

    endErrors = .nil
    if endErrorStorageSize > 0 then do
        -- if errorStorageLimit == .RexxInfo~internalMaxNumber then raise syntax 88.900 array ("Specifying endErrorStorageSize is allowed only if the regular storage is limited")
        endErrors = .CircularQueue~new(endErrorStorageSize)
    end

    indexB = 1
    previousCodepoint = -1
    graphemeBreakState = 0

    forever:
        codepoint = .RexxUnicode~utf8DecodeCodepoint(string, indexB, >sizeB, >errorCode, >errorMsg)
        if sizeB == 0 then do
            -- End of string
            -- Convert the circular queues to arrays in Lifo order, ready for indexing from the end.
            -- Illustration with codepoints:
            --     index -1: indexer~codepointAtIndexC(indexer~codepointCount)          first codepoint from the end
            --     index -2: indexer~codepointAtIndexC(indexer~codepointCount - 1)      second codepoint from the end
            --     index -3: indexer~codepointAtIndexC(indexer~codepointCount - 2)      third codepoint from the end
            --     etc.
            if .nil \== endCodepointIndexes then endCodepointIndexes = endCodepointIndexes~makeArray("L")
            if .nil \== endGraphemeIndexes then endGraphemeIndexes = endGraphemeIndexes~makeArray("L")
            if .nil \== endErrors then endErrors = endErrors~makeArray("L")
            return
        end
        if sizeB < 0 then signal error

        codepointCount += 1
        if codepointCount <= codepointStorageLimit then codepointIndexes~append(indexB)
        if endCodepointStorageSize > 0 then endCodepointIndexes~append(indexB)

        if previousCodepoint < 0 then do
            graphemeCount += 1
            if graphemeCount <= graphemeStorageLimit then graphemeIndexes~append(indexB) -- First codepoint or Error recovery
            if graphemeCount == (graphemeStorageLimit + 1) then lastGraphemeSuccessorIndex = indexB
            if endGraphemeStorageSize > 0 then endGraphemeIndexes~append(indexB)
        end
        else do
            if .RexxUnicode~graphemeBreak3(previousCodepoint, codepoint, >graphemeBreakState) then do
                graphemeCount += 1
                if graphemeCount <= graphemeStorageLimit then graphemeIndexes~append(indexB)
                if graphemeCount == (graphemeStorageLimit + 1) then lastGraphemeSuccessorIndex = indexB
                if endGraphemeStorageSize > 0 then endGraphemeIndexes~append(indexB)
            end
        end
        previousCodepoint = codepoint
        indexB += sizeB
        signal forever

    error:
        errorCount += 1
        errorMsg = "start byte position" indexB ":" errorMsg
        if errorCount <= errorStorageLimit then errors~append(errorMsg)
        if endErrorStorageSize > 0 then endErrors~append(errorMsg)

        codepointCount += 1
        if codepointCount <= codepointStorageLimit then codepointIndexes~append(-indexB) -- a negative index means "error"
        if endCodepointStorageSize > 0 then endCodepointIndexes~append(-indexB)

        graphemeCount += 1
        if graphemeCount <= graphemeStorageLimit then graphemeIndexes~append(-indexB) -- a negative index means "error"
        if graphemeCount == (graphemeStorageLimit + 1) then lastGraphemeSuccessorIndex = -indexB
        if endGraphemeStorageSize > 0 then endGraphemeIndexes~append(-indexB)

        graphemeBreakState = 0 -- reset the extended grapheme state
        previousCodepoint = codepoint
        indexB += abs(sizeB) -- skip the invalid bytes
        signal forever


-- Converts a codepoint index to a byte index
::method codepointIndexB
    expose string codepointIndexes endCodepointIndexes codepointCount codepointStorageLimit
    use strict arg indexC

    signedIndexC = indexC -- used in error messages
    if indexC <= 0 then do
        -- Index from the end
        indexC = codepointCount - abs(indexC) + 1
        if signedIndexC == 0 then raise syntax 88.900 array ("Invalid code point index:" signedIndexC)
        if indexC <= 0 then raise syntax 88.900 array ("Invalid code point index:" signedIndexC "exceeds the code point count" codepointCount)
        if indexC > codepointCount then raise syntax 88.900 array ("Invalid code point index:" signedIndexC "exceeds the code point count" codepointCount)
    end
    else do
        -- Index from the start
        -- Here, indexC > 0
        if indexC > codepointCount then raise syntax 88.900 array ("Invalid code point index:" indexC "exceeds the code point count" codepointCount)
    end

    if .nil \== endCodepointIndexes,  endCodepointIndexes~items \== 0 then do
        /*
        We have a non-empty end storage.
        Assuming a string made of 10 codepoints, a storage limit of 3, an end storage size of 4.
        Indexes of codepoints in string     1 2 3 4 5 6 7 8 9 10
        Indexes of codepointIndexes         1 2 3
        Items   of codepointIndexes         1 2 3
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
    if indexC > codepointIndexes~items then do
        if codepointCount > codepointIndexes~items then do
            -- Limited storage
            if indexC <= codepointCount then raise syntax 88.900 array ("Code point index" signedIndexC "is not stored")
        end
        -- indexC is too large because the string has not enough codepoints.
        raise syntax 88.900 array ("Invalid code point index:" signedIndexC "exceeds the codepoint count" codepointIndexes~items)
    end
    return codepointIndexes[indexC]


-- Returns the codepoint index corresponding to byte index `indexB`.
-- It is negative if the byte index is not aligned with a codepoint boundary.
-- Performs a binary search.
-- Supports negative byte indexes and sparse storage, even though the latter does not make much sense.
::method codepointIndexC
    expose codepointIndexes endCodepointIndexes codepointCount
    use strict arg indexB

    signedIndexB = indexB -- used in error messages
    if indexB <= 0 then do
        -- Index from the end
        indexB = self~string~length - abs(indexB) + 1
        if signedIndexB == 0 then raise syntax 88.900 array ("Invalid byte index:" signedIndexB)
        if indexB <= 0 then raise syntax 88.900 array ("Invalid byte index:" signedIndexB "exceeds the byte string length" self~string~length)
        if indexB > self~string~length then raise syntax 88.900 array ("Invalid byte index:" signedIndexB "exceeds the byte string length" self~string~length)
    end
    else do
        -- Index from the start
        -- Here, indexB > 0
        if indexB > self~string~length then raise syntax 88.900 array ("Invalid byte index:" indexB "exceeds the byte string length" self~string~length)
    end

    indexC1 = 1
    indexC2 = codepointCount
    do forever
        if indexC1 > indexC2 then return -indexC2 -- -- negative when not aligned with the codepoint index
        indexC = (indexC1 + indexC2) % 2
        item = codepointIndexes[indexC]
        if .nil == item, .nil \== endCodepointIndexes then item = endCodepointIndexes[codepointCount - indexC + 1]
        if .nil == item then raise syntax 88.900 array ("Codepoint index" indexC "is not stored. Binary search for byte index" signedIndexB "requires unlimited storage")
        item = abs(item) -- the stored index can be negative to indicate an invalid byte sequence
        if item == indexB then return indexC
        if item > indexB then indexC2 = indexC - 1
                         else indexC1 = indexC + 1
    end
    return -- should never reach here


-- Returns the codepoint at position indexC
::method codepointAtIndexC
    expose string codepointIndexes
    use strict arg indexC
    startB = self~codepointIndexB(indexC)
    if startB < 0 then return "FFFD"~x2d -- the current byte sequence is invalid, return the replacement character
    return .RexxUnicode~utf8DecodeCodepoint(string, startB) -- yes, must decode to return the codepoint as a whole number


-- Converts a grapheme index to a byte index
::method graphemeIndexB
    expose string graphemeIndexes lastGraphemeSuccessorIndex endGraphemeIndexes graphemeCount
    use strict arg indexG, plus1 = .false

    signedIndexG = indexG -- used in error messages
    if indexG <= 0 then do
        -- Index from the end
        indexG = graphemeCount - abs(indexG) + 1

        -- Special case, to not raise an error when the user requests the grapheme at position -1 (last grapheme).
        -- This method is called internally with indexG == 0 and plus1 == .true when the index -1 was specified
        if plus1, indexG == (graphemeCount + 1) then nop -- no error
        else do
            if signedIndexG == 0 then raise syntax 88.900 array ("Invalid grapheme index:" signedIndexG)
            if indexG <= 0 then raise syntax 88.900 array ("Invalid grapheme index:" signedIndexG "exceeds the grapheme count" graphemeCount)
            if indexG > graphemeCount then raise syntax 88.900 array ("Invalid grapheme index:" signedIndexG "exceeds the grapheme count" graphemeCount)
        end
    end
    else do
        -- Index from the start
        -- Here, indexG > 0

        -- Special case, to not raise an error when the user requests the grapheme exactly at graphemeCount position (last grapheme).
        -- This method is called internally with indexG == (graphemeCount + 1) and plus1 == .true
        if plus1, indexG == (graphemeCount + 1) then nop -- no error
        else if indexG > graphemeCount then raise syntax 88.900 array ("Invalid grapheme index:" signedIndexG "exceeds the grapheme count" graphemeCount)
    end

    if .nil \== endGraphemeIndexes,  endGraphemeIndexes~items \== 0 then do
        /*
        We have a non-empty end storage.
        Assuming a string made of 10 graphemes, a storage limit of 3, an end storage size of 4.
        Indexes of graphemes in string      1 2 3 4 5 6 7 8 9 10
        Indexes of graphemeIndexes          1 2 3
        Items   of graphemeIndexes          1 2 3
        Indexes of endGraphemeIndexes                   1 2 3 4
        Items   of endGraphemeIndexes                   7 8 9 10
        */

        -- See if the end storage contains the requested index
        firstEndIndexMinus1 = graphemeCount - endGraphemeIndexes~items
        if indexG > firstEndIndexMinus1 then do
            -- The requested index is in the end storage

            -- special case, to not raise an error when the user requests the grapheme exactly at graphemeCount position (last grapheme).
            if plus1, indexG == (graphemeCount + 1) then do
                -- Don't return lastGraphemeSuccessorIndex,
                -- because here we need the successor of the last grapheme,
                -- not the successor of the last stored grapheme.
                -- lastGraphemeSuccessorIndex is different from string~length + 1 when the storage is limited.
                return string~length + 1 -- byte index of the grapheme after the grapheme at graphemeCount
            end

            endIndexG = graphemeCount - indexG + 1 -- converts an index in the range 7..10 to an index in the range 4..1
            return endGraphemeIndexes[endIndexG]
        end
    end

    -- Here, no end storage (either because not requested, or because the string is empty), or end storage not applicable for the request index

    -- special case, to not raise an error when the user requests the grapheme exactly at graphemeIndexes~items position (last stored grapheme).
    if plus1, indexG == (graphemeIndexes~items + 1) then return lastGraphemeSuccessorIndex

    if indexG > graphemeIndexes~items then do
        if graphemeCount > graphemeIndexes~items then do
            -- Limited storage
            if indexG <= graphemeCount then raise syntax 88.900 array ("Grapheme index" signedIndexG "is not stored")
        end
        -- indexG is too large because the string has not enough codepoints.
        raise syntax 88.900 array ("Invalid grapheme index:" signedIndexG "exceeds the grapheme count" graphemeIndexes~items)
    end
    return graphemeIndexes[indexG]


-- Returns the grapheme index corresponding to byte index `indexB`.
-- It is negative if the byte index is not aligned with a grapheme boundary.
-- Performs a binary search.
-- Supports negative byte indexes and sparse storage, even though the latter does not make much sense.
::method graphemeIndexG
    expose graphemeIndexes endGraphemeIndexes graphemeCount
    use strict arg indexB

    signedIndexB = indexB -- used in error messages
    if indexB <= 0 then do
        -- Index from the end
        indexB = self~string~length - abs(indexB) + 1
        if signedIndexB == 0 then raise syntax 88.900 array ("Invalid byte index:" signedIndexB)
        if indexB <= 0 then raise syntax 88.900 array ("Invalid byte index:" signedIndexB "exceeds the byte string length" self~string~length)
        if indexB > self~string~length then raise syntax 88.900 array ("Invalid byte index:" signedIndexB "exceeds the byte string length" self~string~length)
    end
    else do
        -- Index from the start
        -- Here, indexB > 0
        if indexB > self~string~length then raise syntax 88.900 array ("Invalid byte index:" indexB "exceeds the byte string length" self~string~length)
    end

    indexG1 = 1
    indexG2 = graphemeCount
    do forever
        if indexG1 > indexG2 then return -indexG2 -- -- negative when not aligned with the grapheme index
        indexG = (indexG1 + indexG2) % 2
        item = graphemeIndexes[indexG]
        if .nil == item, .nil \== endGraphemeIndexes then item = endGraphemeIndexes[graphemeCount - indexG + 1]
        if .nil == item then raise syntax 88.900 array ("Grapheme index" indexG "is not stored. Binary search for byte index" signedIndexB "requires unlimited storage")
        item = abs(item) -- the stored index can be negative to indicate an invalid byte sequence
        if item == indexB then return indexG
        if item > indexB then indexG2 = indexG - 1
                         else indexG1 = indexG + 1
    end
    return -- should never reach here


-- Returns the grapheme at position indexG
::method graphemeAtIndexG
    expose string
    use strict arg indexG
    startB = self~graphemeIndexB(indexG)
    nextB = self~graphemeIndexB(indexG+1, /*plus1:*/ .true)
    absStartB = abs(startB) -- startB can be negative (meaning current byte sequence is invalid)
    absNextB = abs(nextB) -- nextB can be negative (meaning next byte sequence is invalid)
    return string~substr(absStartB, absNextB - absStartB) -- yes, always the current byte sequence, even if invalid


-- Returns a string providing information about the indexer's string
::method info
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
