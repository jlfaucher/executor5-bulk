/*
This demo/validation script is similar to RexxUnicodeServices_test.rex.
The helpers are not needed because they are provided by rxunicode.cls.

The RexxUnicode class is a subclass of the RexxUnicodeServices class.

Other classes provided by rxunicode.cls:
- The RexxUnicodeCharacter class
- The RexxUnicodeCodepointSupplier class
- The RexxUnicodeGraphemeSupplier class
- The RexxUnicodeStringIndexer class

*/

arg option .

signal on syntax name afterLoadingICU4ooRexx
-- Not using ::requires because optional.
-- When ICU4ooRexx is loaded after rxunicode.cls, it automatically registers
-- itself alongside rxunicode.cls.
.context~package~loadPackage("ICU4ooRexx.cls")
afterLoadingICU4ooRexx:
signal off syntax

signal skip

-- Error 93.967:  NEW method is not supported for the RexxUnicode class.
say .RexxUnicode~new

-- Error 93.970:  COPY method is not supported for object The RexxUnicode class.
say .RexxUnicode~copy

skip:

say
say "UTF8Proc version" .RexxUnicode~utf8procVersion
say "UTF8Proc Unicode version" .RexxUnicode~unicodeVersion
say

-- Don't display the ICU versions when testing regression
-- because the ICU versions are specific to each environment.
if option \== "--TESTREGRESSION" then do
    if .RexxUnicode~ICU4ooRexxIsRegistered then do
        say "ICU4ooRexx version" .ICU4ooRexx~version
        say "ICU4C compile-time version" .ICU4ooRexx~U_ICU_VERSION
        say "ICU4C runtime version" .ICU4ooRexx~u_getVersion
        say "ICU4C Unicode version" .ICU4ooRexx~u_getUnicodeVersion
        say
    end
end


say "************"
say "Encode UTF-8"
say "************"
say

mb = .MutableBuffer~new("Demo utf8EncodeCodepoint: ")
say "Initial value of the mutable buffer mb:" quoted(mb)
do codepoint over ("U+006F", "U+00EB", "U+20AC", "U+1F385")
    .RexxUnicode~utf8EncodeCodepoint(.RexxUnicode~U2D(codepoint), mb, >size)
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
    value = .RexxUnicode~codepointCategory(c, >code, >label)
    say ".RexxUnicode~codepointCategory(" || c || ") =" value code label
end
say
say

-- https://www.unicode.org/reports/tr9/#Table_Bidirectional_Character_Types
say "Bidirectional Character Types"
-- Values 1..n
-- The returned value is 0 when the codepoint is invalid.
-- There is no code and no label for 0, hence the "?" displayed.
do c over (-10, 0, 10, 100, 1000, 10000, 100000, 1000000, 10000000)
    value = .RexxUnicode~codepointBidiClass(c, >code, >label)
    say ".RexxUnicode~codepointBidiClass(" || c || ") =" value code label
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
say .RexxUnicode~C2X(string)
say

say "Performs unicode case folding (for case-insensitive string comparison)"
tstring = .RexxUnicode~utf8Transform(string, /*casefold:*/ .true)
say quoted(tstring)
say .RexxUnicode~C2X(tstring)
say

say "Strip ignorable characters such as SOFT-HYPHEN or ZERO-WIDTH-SPACE"
tstring = .RexxUnicode~utf8Transform(string, /*casefold:*/, /*lump:*/, /*nlf:*/, /*normalization:*/, /*stripCC:*/, /*stripIgnorable:*/ .true)
say quoted(tstring)
say .RexxUnicode~C2X(tstring)
say

say "Lumps certain characters together"
say "All the concerned characters become the same character, but still remain distinct characters"
tstring = .RexxUnicode~utf8Transform(string, /*casefold:*/, /*lump:*/ .true)
say quoted(tstring)
say .RexxUnicode~C2X(tstring)
say

say "Strips and/or converts control characters"
tstring = .RexxUnicode~utf8Transform(string, /*casefold:*/, /*lump:*/, /*nlf:*/, /*normalization:*/, /*stripCC:*/ .true)
say quoted(tstring)
say .RexxUnicode~C2X(tstring)
say

say "Strips all character markings"
say "This includes non-spacing, spacing and enclosing (i.e. accents)"
say "This option works only with normalization"
tstring = .RexxUnicode~utf8Transform(string, /*casefold:*/, /*lump:*/, /*nlf:*/, /*normalization:*/ 1, /*stripCC:*/, /*stripIgnorable:*/, /*stripMark*/ .true)
say quoted(tstring)
say .RexxUnicode~C2X(tstring)
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
say .RexxUnicode~C2X(string)
say

say "Normalization Form C"
say "Canonical Decomposition, followed by Canonical Composition"
say "Characters are decomposed and then recomposed by canonical equivalence."
tstring = .RexxUnicode~utf8Transform(string, /*casefold:*/, /*lump:*/, /*nlf:*/, /*normalization:*/ .RexxUnicode~NFC)
say quoted(tstring)
say .RexxUnicode~C2X(tstring)
say

say "Normalization Form D"
say "Canonical Decomposition"
say "Characters are decomposed by canonical equivalence, and multiple combining characters are arranged in a specific order."
tstring = .RexxUnicode~utf8Transform(string, /*casefold:*/, /*lump:*/, /*nlf:*/, /*normalization:*/ .RexxUnicode~NFD)
say quoted(tstring)
say .RexxUnicode~C2X(tstring)
say

say "Normalization Form KC"
say "Compatibility Decomposition, followed by Canonical Composition"
say "Characters are decomposed by compatibility, then recomposed by canonical equivalence."
tstring = .RexxUnicode~utf8Transform(string, /*casefold:*/, /*lump:*/, /*nlf:*/, /*normalization:*/ .RexxUnicode~NFKC)
say quoted(tstring)
say .RexxUnicode~C2X(tstring)
say

say "Normalization Form KD"
say "Compatibility Decomposition"
say "Characters are decomposed by compatibility, and multiple combining characters are arranged in a specific order."
tstring = .RexxUnicode~utf8Transform(string, /*casefold:*/, /*lump:*/, /*nlf:*/, /*normalization:*/ .RexxUnicode~NFKD)
say quoted(tstring)
say .RexxUnicode~C2X(tstring)
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

    indexer = .RexxUnicodeStringIndexer~new(string)
    say "startCodepointIndexes:" indexer~startCodepointIndexes~toString(, " ")
    say "startGraphemeIndexes:" indexer~startGraphemeIndexes~toString(, " ")

    if indexer~startErrorMessages~items \== 0 then do
        say "errors"
        do i=1 to indexer~startErrorMessages~items
            say i~right(3) ":" indexer~startErrorMessages[i]
        end
    end

    say "codepoints"
    do i=1 to indexer~startCodepointIndexes~items
        codepoint = indexer~codepointAtIndexC(i)
        say i~right(3) ":" .RexxUnicodeCharacter~new(codepoint)
    end

    say "graphemes"
    do i=1 to indexer~startGraphemeIndexes~items
        grapheme = indexer~graphemeAtIndexG(i)
        isMalformed = indexer~graphemeIndexB(i) < 0
        character = isMalformed~?("EFBFBD"x, grapheme) -- Use the replacement character if malformed
        say i~right(3) ":" character '"'.RexxUnicode~C2X(grapheme)'"x'
    end
    say
    say


::requires "rxunicode.cls"
