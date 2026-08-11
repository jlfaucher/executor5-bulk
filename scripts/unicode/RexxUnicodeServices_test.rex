/*
The RexxUnicodeServices class is native.
No need to require something.

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
*/

signal skip

-- Error 93.967:  NEW method is not supported for the RexxUnicodeServices class.
say .RexxUnicodeServices~new

-- Error 93.970:  COPY method is not supported for object The RexxUnicodeServices class.
say .RexxUnicodeServices~copy

skip:

say
say "UTF8Proc version" .RexxUnicodeServices~utf8procVersion
say "Unicode version" .RexxUnicodeServices~unicodeVersion
say
say


say "************"
say "Encode UTF-8"
say "************"
say

mb = .MutableBuffer~new("Demo utf8EncodeCodepoint: ")
say "Initial value of the mutable buffer mb:" quoted(mb)
do codepoint over ("U+006F", "U+00EB", "U+20AC", "U+1F385")
    .RexxUnicodeServices~utf8EncodeCodepoint(U2D(codepoint), mb, >size)
    say "Append the UTF-8 encoding of" codepoint "to mb: size =" size "mb =" quoted(mb)
end
say
say


say "************"
say "Decode UTF-8"
say "************"
say

call analyze "noël👩‍👨‍👩‍👧🎅"

call analyze "63 C3 B4 74 65 CC 81 F0 9F 91 8D"x -- côté👍


say "************************************"
say "Detect malformed UTF-8 byte sequence"
say "************************************"
say

say "Invalid start byte (non-shortest form)"
call analyze       "B4 74 65 CC 81 F0 9F 91 8D"x, /*display:*/.false

say "Invalid start byte (codepoint > U+10FFFF)"
call analyze       "F5 74 65 CC 81 F0 9F 91 8D"x, /*display:*/.false

say "Invalid continuation byte"
call analyze "63 C3    74 65 CC 81 F0 9F 91 8D"x, /*display:*/.false

say "Invalid continuation byte (non-shortest form)"
call analyze "63 E0 9F 81"x, /*display:*/.false

say "Invalid continuation byte (codepoint > U+10FFFF)"
call analyze "63 F4 90 91 8D"x, /*display:*/.false

say "Invalid continuation byte (high surrogate)"
call analyze "ED A0 80"x, /*display:*/.false -- U+D800

say "Invalid continuation byte (low surrogate)"
call analyze "ED B0 81"x, /*display:*/.false -- U+DC01

say "Truncated"
call analyze "63 C3 B4 74 65 CC 81 F0 9F 91"x, /*display:*/.false


say "***************************************"
say "U+FFFD Substitution of Maximal Subparts"
say "***************************************"
say

-- https://www.unicode.org/versions/Unicode17.0.0/core-spec/chapter-3/#G67519
say "Table 3-8. U+FFFD for Non-Shortest Form Sequences"
call analyze "C0 AF E0 80 BF F0 81 82 41"x, /*display:*/.false

-- https://www.unicode.org/versions/Unicode17.0.0/core-spec/chapter-3/#G67520
say "Table 3-9. U+FFFD for Ill-Formed Sequences for Surrogates"
call analyze "ED A0 80 ED BF BF ED AF 41"x, /*display:*/.false

-- https://www.unicode.org/versions/Unicode17.0.0/core-spec/chapter-3/#G68064
say "Table 3-10. U+FFFD for Other Ill-Formed Sequences"
call analyze "F4 91 92 93 FF 41 80 BF 42"x, /*display:*/.false

-- https://www.unicode.org/versions/Unicode17.0.0/core-spec/chapter-3/#G67520
say "Table 3-11. U+FFFD for Truncated Sequences"
call analyze "E1 80 E2 F0 91 92 F1 BF 41"x, /*display:*/.false


say "********************"
say "Codepoint properties"
say "********************"
say

-- https://www.unicode.org/versions/Unicode17.0.0/core-spec/chapter-4/#G124142
say "General Category"
-- Values 0..n
do c over (-10, 0, 10, 100, 1000, 10000, 100000, 1000000, 10000000)
    value = .RexxUnicodeServices~codepointCategory(c, >code, >label)
    say ".RexxUnicodeServices~codepointCategory(" || c || ") =" value code label
end
say
say

-- https://www.unicode.org/reports/tr9/#Table_Bidirectional_Character_Types
say "Bidirectional Character Types"
-- Values 1..n
-- The returned value is 0 when the codepoint is invalid.
-- There is no code and no label for 0, hence the "?" displayed.
do c over (-10, 0, 10, 100, 1000, 10000, 100000, 1000000, 10000000)
    value = .RexxUnicodeServices~codepointBidiClass(c, >code, >label)
    say ".RexxUnicodeServices~codepointBidiClass(" || c || ") =" value code label
end
say
say


say "***********************"
say "Unicode transformations"
say "***********************"
say

say "method utf8Transform"
say "use strict arg string,"
say "               casefold = .false,      -- Unicode case folding"
say "               lump= .false,           -- E.g. HYPHEN U+2010 and MINUS U+2212 to ASCII ""-"""
say "               nlf = 0,                -- Related to LF, CRLF, CR, NEL"
say "               normalization = 0,      -- NFC=1, NFD=2, NFKC=3, NFKD=4"
say "               stripCC = .false,       -- Strips and/or converts control characters"
say "               stripIgnorable= .false, -- Strip ignorable characters"
say "               stripMark = .false,     -- Strips all character markings (i.e. accents)"
say "               stripNA = .false        -- Strips unassigned codepoints"
say
say "This string will be used as input for the transformations:"
say 'string = "Le\N{IDEOGRAPHIC SPACE}\N{ZERO-WIDTH-SPACE}Père\t\N{HYPHEN}\N{SOFT-HYPHEN}Noël"'
say "Character names and escape characters are not supported by UTF8Proc. Use hexadecimal notation:"
string = "4C 65 E38080 E2808B 50 C3A8 72 65 09 E28090 C2AD 4E 6F C3AB 6C"
say "string = " quoted(string)
string = "4C 65 E38080 E2808B 50 C3A8 72 65 09 E28090 C2AD 4E 6F C3AB 6C"x
say
call analyze string

say "Initial string"
say quoted(string)
say codepointsC2X(string)
say

say "Performs unicode case folding (for case-insensitive string comparison)"
tstring = .RexxUnicodeServices~utf8Transform(string, /*casefold:*/ .true)
say quoted(tstring)
say codepointsC2X(tstring)
say

say "Strip ignorable characters such as SOFT-HYPHEN or ZERO-WIDTH-SPACE"
tstring = .RexxUnicodeServices~utf8Transform(string, /*casefold:*/, /*lump:*/, /*nlf:*/, /*normalization:*/, /*stripCC:*/, /*stripIgnorable:*/ .true)
say quoted(tstring)
say codepointsC2X(tstring)
say

say "Lumps certain characters together"
say "All the concerned characters become the same character, but still remain distinct characters"
tstring = .RexxUnicodeServices~utf8Transform(string, /*casefold:*/, /*lump:*/ .true)
say quoted(tstring)
say codepointsC2X(tstring)
say

say "Strips and/or converts control characters"
tstring = .RexxUnicodeServices~utf8Transform(string, /*casefold:*/, /*lump:*/, /*nlf:*/, /*normalization:*/, /*stripCC:*/ .true)
say quoted(tstring)
say codepointsC2X(tstring)
say

say "Strips all character markings"
say "This includes non-spacing, spacing and enclosing (i.e. accents)"
say "This option works only with normalization"
tstring = .RexxUnicodeServices~utf8Transform(string, /*casefold:*/, /*lump:*/, /*nlf:*/, /*normalization:*/ 1, /*stripCC:*/, /*stripIgnorable:*/, /*stripMark*/ .true)
say quoted(tstring)
say codepointsC2X(tstring)
say
say


say "**********************"
say "Unicode normalizations"
say "**********************"
say

say 'Some remarks about the string used in the next demo:'
say '- the first "äöü" is NFC, the second "äöü" is NFD.'
say '- "x̂" is two codepoints in any normalization.'
say '- "ϔ" normalization forms are all different.'
say '- "ﷺ" is one of the worst cases regarding the expansion factor in NFKS/NFKS: 18x'
say '- "baﬄe"~text~subchar(3)=     -- ""ﬄ"'
say '  "baﬄe"~text~upper=          -- "BAﬄE", not "BAFFLE".'
say '  The ligature disappears in NFK[CD] but not in NF[CD].'
say

string = "äöü äöü x̂ ϔ ﷺ baﬄe"
call analyze string

say "Initial string"
say quoted(string)
say codepointsC2X(string)
say

say "NFC=1"
say "Normalization Form C"
say "Canonical Decomposition, followed by Canonical Composition"
say "Characters are decomposed and then recomposed by canonical equivalence."
tstring = .RexxUnicodeServices~utf8Transform(string, /*casefold:*/, /*lump:*/, /*nlf:*/, /*normalization:*/ 1)
say quoted(tstring)
say codepointsC2X(tstring)
say

say "NFD=2"
say "Normalization Form D"
say "Canonical Decomposition"
say "Characters are decomposed by canonical equivalence, and multiple combining characters are arranged in a specific order."
tstring = .RexxUnicodeServices~utf8Transform(string, /*casefold:*/, /*lump:*/, /*nlf:*/, /*normalization:*/ 2)
say quoted(tstring)
say codepointsC2X(tstring)
say

say "NFKC=3"
say "Normalization Form KC"
say "Compatibility Decomposition, followed by Canonical Composition"
say "Characters are decomposed by compatibility, then recomposed by canonical equivalence."
tstring = .RexxUnicodeServices~utf8Transform(string, /*casefold:*/, /*lump:*/, /*nlf:*/, /*normalization:*/ 3)
say quoted(tstring)
say codepointsC2X(tstring)
say

say "NFKD=4"
say "Normalization Form KD"
say "Compatibility Decomposition"
say "Characters are decomposed by compatibility, and multiple combining characters are arranged in a specific order."
tstring = .RexxUnicodeServices~utf8Transform(string, /*casefold:*/, /*lump:*/, /*nlf:*/, /*normalization:*/ 4)
say quoted(tstring)
say codepointsC2X(tstring)
say


/*******************************************************************************
Demo helper
*******************************************************************************/

::routine analyze
    use strict arg string, display=.true
    if display then say "Analyze" quoted(string)
    call charout , "indexes: "
    do i=1 to string~length
        call charout , i~right(2, "0") || " "
    end
    say
    call charout , "bytes:   "
    do i=1 to string~length
        byte = string~subchar(i)
        call charout , byte~c2x || " "
    end
    say

    call createIndexers string, >codepointIndexes, >graphemeIndexes, >errors
    say "codepointIndexes:" codepointIndexes~toString(, " ")
    say "graphemeIndexes:" graphemeIndexes~toString(, " ")

    if errors~items \== 0 then do
        say "errors"
        do i=1 to errors~items
            say i~right(3) ":" errors[i]
        end
    end

    say "codepoints"
    do i=1 to codepointIndexes~items
        codepoint = codepointAtIndexC(string, i, codepointIndexes)
        category = .RexxUnicodeServices~codepointCategory(codepoint, >code, >label)
        say i~right(3) ":" D2U(codepoint)~left(8) code label
    end

    say "graphemes"
    do i=1 to graphemeIndexes~items
        grapheme = graphemeAtIndexC(string, i, graphemeIndexes)
        isMalformed = graphemeIndexB(string, i, graphemeIndexes) < 0
        character = isMalformed~?("EFBFBD"x, grapheme) -- Use the replacement character if malformed
        say i~right(3) ":" character '"'codepointsC2X(grapheme)'"x'
    end
    say
    say


/*******************************************************************************
Codepoint supplier
*******************************************************************************/

::class "CodepointSupplier" public subclass Supplier

::method init
    expose string indexB indexC
    use strict arg string
    empty = .array~new(0) -- No need to create a copy, the string is immutable
    self~init:super(empty, empty)
    indexB = 1
    indexC = 1
    self~decode


::method decode
    expose string indexB sizeB codepoint byteSequence
    use strict arg -- none
    codepoint = .RexxUnicodeServices~utf8DecodeCodepoint(string, indexB, >sizeB)
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
    if self~available then return (type == "c")~?(indexC, indexB)
    -- error


::method item
    expose codepoint byteSequence
    use strict arg type="c" -- "c" to get the codepoint, "b" to get the byte sequence
    if self~available then return (type == "c")~?(codepoint, byteSequence)
    -- error


::method next
    expose indexB sizeB indexC
    use strict arg -- none
    indexB += sizeB
    indexC += 1
    self~decode


/*******************************************************************************
Helpers for direct access to codepoints and graphemes
*******************************************************************************/

-- Demonstrate how to iterate over the codepoints and graphemes
::routine createIndexers
    use strict arg string, >codepointIndexes, >graphemeIndexes, >errors
    codepointIndexes = .array~new
    graphemeIndexes = .array~new
    errors = .array~new
    indexB = 1
    previousCodepoint = -1
    previousCodepointIndexB = -1
    graphemeBreakArgs = (-1, -1, 0) -- codepoint1, codepoint2, state. Will use the same array at each iteration.

    forever:
        codepoint = .RexxUnicodeServices~utf8DecodeCodepoint(string, indexB, >sizeB, >errorCode, >errorMsg)
        if sizeB == 0 then return -- End of string
        if sizeB < 0 then signal error

        codepointIndexes~append(indexB)
        if previousCodepoint < 0 then graphemeIndexes~append(indexB) -- First codepoint or Error recovery
        else do
            graphemeBreakArgs[1] = previousCodepoint
            graphemeBreakArgs[2] = codepoint
            if .RexxUnicodeServices~graphemeBreak(graphemeBreakArgs) then graphemeIndexes~append(indexB)
        end
        previousCodepoint = codepoint
        previousCodepointIndexB = indexB
        indexB += sizeB
    signal forever

    error:
        errors~append("start byte position" indexB ":" errorMsg)
        codepointIndexes~append(-indexB) -- a negative index means "error"
        graphemeIndexes~append(-indexB) -- idem
        graphemeBreakArgs[3] = 0 -- reset the extended grapheme state
        previousCodepoint = codepoint
        previousCodepointIndexB = indexB
        indexB += abs(sizeB) -- skip the invalid bytes
        signal forever


-- Convert a codepoint index into a byte index
::routine codepointIndexB
    use strict arg string, indexC, codepointIndexes
    if indexC < 1 then raise syntax 23.900 array (self~description "invalid codepoint index:" indexC)
    if indexC > codepointIndexes~items then return string~length + 1
    return codepointIndexes[indexC]


-- Return the codepoint at position indexC
::routine codepointAtIndexC
    use strict arg string, indexC, codepointIndexes
    startB = codepointIndexB(string, indexC, codepointIndexes)
    absStartB = abs(startB) -- startB can be negative (meaning current byte sequence is invalid)

    if startB < 0 then return "FFFD"~x2d -- the current byte sequence is invalid, return the replacement character
    return .RexxUnicodeServices~utf8DecodeCodepoint(string, absStartB)


-- Convert a grapheme index into a byte index
::routine graphemeIndexB
    use strict arg string, indexC, graphemeIndexes
    if indexC < 1 then raise syntax 23.900 array (self~description "invalid grapheme index:" indexC)
    if indexC > graphemeIndexes~items then return string~length + 1
    return graphemeIndexes[indexC]


-- Return the grapheme at position indexC
::routine graphemeAtIndexC
    use strict arg string, indexC, graphemeIndexes
    startB = graphemeIndexB(string, indexC, graphemeIndexes)
    nextB = graphemeIndexB(string, indexC+1, graphemeIndexes)
    absStartB = abs(startB) -- startB can be negative (meaning current byte sequence is invalid)
    absNextB = abs(nextB) -- nextB can be negative (meaning next byte sequence is invalid)
    return string~substr(absStartB, absNextB - absStartB) -- yes, always the current byte sequence, even if invalid


/*******************************************************************************
Other helpers
*******************************************************************************/

::routine quoted
    use strict arg string
    return '"'string'"'


-- Specialized C2X: Insert a space between the codepoints
::routine codepointsC2X
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


::routine D2U
    use strict arg codepoint
    if codepoint < 65536 then return "U+" || codepoint~d2x(4)
    return "U+" || codepoint~d2x


::routine U2D
    use strict arg UPlusCodepoint -- "U+XXXX..."
    if UPlusCodepoint~startsWith("U+") then return UPlusCodepoint~substr(3)~x2d
    return UPlusCodepoint
