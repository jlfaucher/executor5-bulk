# Unicode support

The `RexxUnicodeServices` class is a native class that exposes the Unicode services
supported by ooRexx. Its current implementation is based on the [utf8proc](https://juliastrings.github.io/utf8proc/)
library embedded in ooRexx.


`rxunicode.cls` is an optional package that defines:
- The `RexxUnicode` class, a subclass of `RexxUnicodeServices`.
- The `RexxUnicodeCodepointSupplier` class.
- The `RexxUnicodeGraphemeSupplier` class.
- The `RexxUnicodeStringIndexer` class.
- The `RexxUnicodeCharacter` class.
- The `ICU4ooRexxInterface` class.

> [!CAUTION]
> Testing these classes from `ooRexxShell` with `TUTOR` enabled showed that a
> defensive barrier is needed.
>
> In case of errors like
>
> - `Argument <name> class: expected String, found Text.     -- "Any string"`
> - `Argument <name> class: expected String, found Bytes.    -- "E0 80 80"x`
>
> use `tutor off` for a permanent workaround,  
> or use `~string` for a temporary workaround.
>
> The `requestBaseString` method ensures that any string passed as an argument
> is a `.String` instance, not an instance of a subclass of `.String`.
>
> No attempt is made to obtain a string from these arguments.
>
> This check is intended to detect accidental mixing of Unicode-aware objects
> with byte strings and to allow users to adapt their code accordingly
> (typically by requesting a `.String` from their TUTOR objects).
> 
> See the examples at the end of `rxunicode.cls` showing the internal errors that
> can occur when mixing graphemes and bytes.

**Note on terminology:** This documentation and the code comments use the term "codepoint"
instead of "code point", except when quoting definitions from the Unicode Standard.


## RexxUnicodeServices Class

The `RexxUnicodeServices` class cannot be instantiated.  
Its services are provided through class methods.

This native class is always available; there is no need to load `rxunicode.cls`.

In the examples, you can replace `RexxUnicodeServices` by `RexxUnicode` if you loaded `rxunicode.cls`.


### Class methods

#### codepointBidiClass

    RexxInteger *codepointBidiClass(RexxInteger *rexxCodepoint, VariableReference *refCode, VariableReference *refLabel); // enum

    .RexxUnicodeServices~codepointBidiClass(codepoint [, [>refCode] [, >refLabel]])


#### codepointBidiMirrored

    RexxInteger *codepointBidiMirrored(RexxInteger *rexxCodepoint); // boolean
    
    .RexxUnicodeServices~codepointBidiMirrored(codepoint)


#### codepointBoundClass

    RexxInteger *codepointBoundClass(RexxInteger *rexxCodepoint, VariableReference *refCode, VariableReference *refLabel); // enum

    .RexxUnicodeServices~codepointBoundClass(codepoint [, [>refCode] [, >refLabel]])


#### codepointCategory

    RexxInteger *codepointCategory(RexxInteger *rexxCodepoint, VariableReference *refCode, VariableReference *refLabel); // enum

    .RexxUnicodeServices~codepointCategory(codepoint [, [>refCode] [, >refLabel]])


#### codepointCharWidth

    RexxInteger *codepointCharWidth(RexxInteger *rexxCodepoint); // integer

    .RexxUnicodeServices~codepointCharWidth(codepoint)


#### codepointCombiningClass

    // The integer 0..254 returned by codepointCombiningClass is the standard Unicode value
    RexxInteger *codepointCombiningClass(RexxInteger *rexxCodepoint, VariableReference *refCode, VariableReference *refLabel); // integer 0..254, enum

    .RexxUnicodeServices~codepointCombiningClass(codepoint [, [>refCode] [, >refLabel]])


#### codepointControlBoundary

    RexxInteger *codepointControlBoundary(RexxInteger *rexxCodepoint); // boolean

    .RexxUnicodeServices~codepointControlBoundary(codepoint)


#### codepointDecompositionType

    RexxInteger *codepointDecompositionType(RexxInteger *rexxCodepoint, VariableReference *refCode, VariableReference *refLabel); // enum

    .RexxUnicodeServices~codepointDecompositionType(codepoint [, [>refCode] [, >refLabel]])


#### codepointIgnorable

    RexxInteger *codepointIgnorable(RexxInteger *rexxCodepoint); // boolean

    .RexxUnicodeServices~codepointIgnorable(codepoint)


#### codepointIsLower

    RexxInteger *codepointIsLower(RexxInteger *rexxCodepoint);

    .RexxUnicodeServices~codepointIsLower(codepoint)


#### codepointIsUpper

    RexxInteger *codepointIsUpper(RexxInteger *rexxCodepoint);

    .RexxUnicodeServices~codepointIsUpper(codepoint)


#### codepointToLower

    RexxInteger *codepointToLower(RexxInteger *rexxCodepoint);

    .RexxUnicodeServices~codepointToLower(codepoint)


#### codepointToTitle

    RexxInteger *codepointToTitle(RexxInteger *rexxCodepoint);

    .RexxUnicodeServices~codepointToTitle(codepoint)


#### codepointToUpper

    RexxInteger *codepointToUpper(RexxInteger *rexxCodepoint);

    .RexxUnicodeServices~codepointToUpper(codepoint)


#### graphemeBreak

    RexxInteger *graphemeBreak(ArrayClass *);

```rexx
-- `state` is persistent state. Its initial value must be `0`.
-- `graphemeBreak` updates `state`.
array = (previousCodepoint, currentCodepoint, state)
break = .RexxUnicodeServices~graphemeBreak(array) -- true or false
```

#### new

This method raises an error because `RexxUnicodeServices` has no instance.


#### SystemIsLittleEndian

    RexxInteger *systemIsLittleEndian();

    .RexxUnicodeServices~systemIsLittleEndian


#### unicodeVersion

    RexxString *unicodeVersion();

    .RexxUnicodeServices~unicodeVersion

Returns the Unicode version supported by the `utf8proc` library.

```rexx
say .RexxUnicodeServices~unicodeVersion        -- 17.0.0 (for example)

```


#### utf8DecodeCodepoint

```
utf8DecodeCodepoint(
    string,         -- (in)             A UTF-8 string.
    indexB,         -- (in)             The byte index (1-based) of the encoded codepoint in string.
    >refSizeB,      -- (out, optional)  The number of bytes read to decode the codepoint:
                    --                      > 0 if no error,
                    --                      < 0 if error,
                    --                      or 0 if indexB is outside the string index range.
    >refErrorCode   -- (out, optional)  The null string "" if a valid codepoint could be read,
                    --                  or the error code otherwise.
    >refErrorMsg    -- (out, optional)  The null string "" if a valid codepoint could be read,
                    --                  or the error message otherwise.
    )

.RexxUnicodeServices~utf8DecodeCodepoint(string, indexB [, [>refSizeB] [, [>refErrorCode] [, >refErrorMsg]]])
```

Returns the next codepoint (an integer) at position `indexB` of `string`, or -1 in case of error.

`refSizeB` returns the size in bytes of the decoded codepoint.  
If `indexB` is outside the `string` index range, the returned size is 0.  
In case of error, the returned size is negative, indicating the number of bytes to skip
to follow the U+FFFD Substitution of Maximal Subparts.  
`refErrorCode` and `refErrorMsg` provide a detailed description of the error.

Error codes and messages:

```
- "CONTINUATION_ERROR_RANGE"  
  "Invalid continuation byte %i ('%02X'x) at byte-position %zu (codepoint > U+10FFFF)"
- "CONTINUATION_HIGH_SURROGATE"  
  "Invalid continuation byte %i ('%02X'x) at byte-position %zu (high surrogate)"
- "CONTINUATION_LOW_SURROGATE"  
  "Invalid continuation byte %i ('%02X'x) at byte-position %zu (low surrogate)"
- "CONTINUATION"  
  "Invalid continuation byte %i ('%02X'x) at byte-position %zu"
- "CONTINUATION_NON_SHORTEST_FORM"  
  "Invalid continuation byte %i ('%02X'x) at byte-position %zu (non-shortest form)"
- "START_ERROR_RANGE"  
  "Invalid start byte %i ('%02X'x) (codepoint > U+10FFFF)"
- "START_NON_SHORTEST_FORM"  
  "Invalid start byte %i ('%02X'x) (non-shortest form)"
- "TRUNCATED"  
  "Truncated, expected %i bytes"
```

```rexx
-- The decoding part of the CodePointSupplier class
::method decode
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
            error = "start byte-position" indexB ":" errorMsg
            codepoint = "FFFD"~x2d -- the current byte sequence is invalid, return the replacement character
            indexB = -indexB -- same convention as .RexxUnicodeStringIndexer; a negative index indicates an invalid byte sequence
        end
        byteSequence = string~substr(abs(indexB), abs(sizeB))
    end
```


#### utf8EncodeCodepoint

```
utf8EncodeCodepoint(
    codepoint,      -- (in)             The codepoint to encode (an integer).
    destination,    -- (in-out)         The mutable buffer to update.
    >refSizeB       -- (out, optional)  The size of the encoded byte sequence (0..4)
                    --                  The size is 0 if the codepoint is not in the range 0..10FFFF.
    )

.RexxUnicodeServices~utf8EncodeCodepoint(codepoint, destination [, >refSizeB])
```

Appends a codepoint's UTF-8 encoding into the specified mutable buffer.  
The surrogates 0xd800-0xdfff are encoded, however these are actually invalid in UTF-8.

Returns the mutable buffer passed with the destination argument.


```rexx
mb = .MutableBuffer~new
do codepoint over ("006F"~x2d, "00EB"~x2d, "20AC"~x2d, "1F385"~x2d, "110000"~x2d)
    .RexxUnicodeServices~utf8EncodeCodepoint(codepoint, mb, >size)
    say "Append the UTF-8 encoding of" codepoint "to mb: size =" size "mb =" mb
end

/*
Output:
Append the UTF-8 encoding of 111 to mb: size = 1 mb = o
Append the UTF-8 encoding of 235 to mb: size = 2 mb = oë
Append the UTF-8 encoding of 8364 to mb: size = 3 mb = oë€
Append the UTF-8 encoding of 127877 to mb: size = 4 mb = oë€🎅
Append the UTF-8 encoding of 1114112 to mb: size = 0 mb = oë€🎅
*/
```


#### utf8procVersion

    RexxString *utf8procVersion();
    
    .RexxUnicodeServices~utf8procVersion

Returns the version of the `utf8proc` library.

```rexx
say .RexxUnicodeServices~utf8procVersion        -- 2.11.3 (for example)

```


#### utf8Transform

```
utf8Transform(
    string,                 -- The UTF-8 string to transform
    casefold = .false,      -- Unicode case folding
    lump= .false,           -- E.g. HYPHEN U+2010 and MINUS U+2212 to ASCII "-"
    nlf = 0,                -- 0=none, 1=NLF2LF, 2=NLF2LS, 3=NLF2PS
    normalization = 0,      -- 0=none, 1=NFC, 2=NFD, 3=NFKC, 4=NFKD
    stripCC = .false,       -- Strips and/or converts control characters
    stripIgnorable= .false, -- Strip ignorable characters
    stripMark = .false,     -- Strips all character markings (i.e. accents)
    stripNA = .false        -- Strips unassigned codepoints
    )

.RexxUnicodeServices~utf8Transform(string [, casefold = .false [, lump= .false [, nlf = 0 [, normalization = 0 [, stripCC = .false [, stripIgnorable= .false [, stripMark = .false [, stripNA = .false]]]]]]]])
```

Returns the transformed string.

##### caseFold

Performs unicode case folding, to be able to do a case-insensitive
string comparison.


##### lump

Maps certain characters to a common representative (i.e., several distinct characters produce the same output character).  
All the concerned characters become the same character, but still remain distinct characters.

E.g. `HYPHEN U+2010` and `MINUS U+2212` to ASCII "-".  
E.g. all space characters (general category Zs) to `U+0020`.

If `NLF2LF` is passed as the 'nlf' argument, this includes a transformation of paragraph and
line separators to ASCII line-feed (LF).  
(to clarify: does it mean that the impact of `NLF2LF` is different when `lump` is true?)


Mapping rules:

    U+0020      <-- all space characters (general category Zs)
    U+0027  '   <-- left/right single quotation mark U+2018..2019,
                    modifier letter apostrophe U+02BC,
                    modifier letter vertical line U+02C8
    U+002D  -   <-- all dash characters (general category Pd),
                    minus U+2212
    U+002F  /   <-- fraction slash U+2044,
                    division slash U+2215
    U+003A  :   <-- ratio U+2236
    U+003C  <   <-- single left-pointing angle quotation mark U+2039,
                    left-pointing angle bracket U+2329,
                    left angle bracket U+3008
    U+003E  >   <-- single right-pointing angle quotation mark U+203A,
                    right-pointing angle bracket U+232A,
                    right angle bracket U+3009
    U+005C  \   <-- set minus U+2216
    U+005E  ^   <-- modifier letter up arrowhead U+02C4,
                    modifier letter circumflex accent U+02C6,
                    caret U+2038,
                    up arrowhead U+2303
    U+005F  _   <-- all connector characters (general category Pc),
                    modifier letter low macron U+02CD
    U+0060  `   <-- modifier letter grave accent U+02CB
    U+007C  |   <-- divides U+2223
    U+007E  ~   <-- tilde operator U+223C


##### nlf

[https://www.unicode.org/versions/Unicode17.0.0/core-spec/chapter-5/#G10213](https://www.unicode.org/versions/Unicode17.0.0/core-spec/chapter-5/#G10213)

```rexx
-- Value to pass as the 'nlf' argument to utf8Transform (default: 0 no transformation).
-- These constants are defined in the RexxUnicode class.
-- If rxunicode.cls is not loaded, use the numeric value directly..
::constant NLF2LF 1
::constant NLF2LS 2
::constant NLF2PS 3
```

NLF2LF  
convert LF, CRLF, CR and NEL into LF.

NLF2LS  
Converts LF, CRLF, CR and NEL into LS.  
NLF-sequences (LF, CRLF, CR, NEL) are representing a line break,
and should be converted to the codepoint for line separation (LS).

NLF2PS  
Converts LF, CRLF, CR and NEL into PS.  
NLF-sequences (LF, CRLF, CR, NEL) are representing a paragraph break,
and should be converted to the codepoint for paragraph separation (PS).


##### normalization

```rexx
-- Value to pass as the `normalization` argument to utf8Transform (default: 0 no normalization).
-- These constants are defined in the RexxUnicode class.
-- If rxunicode.cls is not loaded, use the numeric value directly..
::constant NFC 1
::constant NFD 2
::constant NFKC 3
::constant NFKD 4
```

If `normalization` is not `0`, apply the requested normalization.


##### stripCC

Strips and/or converts control characters.

NLF-sequences are transformed into space, except if one of the
NLF2LS/PS/LF options is given. HorizontalTab (HT) and FormFeed (FF)
are treated as a NLF-sequence in this case.

All other control characters are simply removed.


##### stripIgnorable

Strips the characters whose property `Default_Ignorable_Code_Point = true`,
such as `SOFT-HYPHEN` or `ZERO-WIDTH-SPACE`.


##### stripMark

Strips all character markings.

This includes non-spacing, spacing and enclosing (i.e. accents) categories:

- Mc Spacing Mark
- Me Enclosing Mark
- Mn Nonspacing Mark

This option works only with a normalization applied.


##### stripNA

Strips the characters whose category is Cn Unassigned.


##### Examples of transformations

```rexx
string = "\N{<control-0007>}Le\N{IDEOGRAPHIC SPACE}\N{OGHAM SPACE MARK}\N{ZERO-WIDTH-SPACE}Père\t\N{HYPHEN}\N{SOFT-HYPHEN}\N{EN DASH}\N{EM DASH}Noël\x{EFB790}\r\n"
string = .RexxUnicode~unescape(string)

.RexxUnicodeCodepointSupplier~new(string, , .RexxUnicodeCharacter)==
    /*
    an Array (shape [22], 22 items)
     1  : (<?> \x{07} U+0007 Cc Control "<control-0007>")                                   <-- removable with STRIPCC:.true
     2  : ("L" \x{4C} U+004C Lu Uppercase_Letter "LATIN CAPITAL LETTER L")
     3  : ("e" \x{65} U+0065 Ll Lowercase_Letter "LATIN SMALL LETTER E")
     4  : ("　" \x{E38080} U+3000 Zs Space_Separator "IDEOGRAPHIC SPACE")                   <-- replaceable by " " with LUMP:.true
     5  : (" " \x{E19A80} U+1680 Zs Space_Separator "OGHAM SPACE MARK")                     <-- replaceable by " " with LUMP:.true
     6  : (<?> \x{E2808B} U+200B Cf Format "ZERO WIDTH SPACE")                              <-- removable by STRIPIGNORABLE:.TRUE
     7  : ("P" \x{50} U+0050 Lu Uppercase_Letter "LATIN CAPITAL LETTER P")
     8  : ("è" \x{C3A8} U+00E8 Ll Lowercase_Letter "LATIN SMALL LETTER E WITH GRAVE")
     9  : ("r" \x{72} U+0072 Ll Lowercase_Letter "LATIN SMALL LETTER R")
     10 : ("e" \x{65} U+0065 Ll Lowercase_Letter "LATIN SMALL LETTER E")
     11 : (<?> \x{09} U+0009 Cc Control "<control-0009>")                                   <-- replaceable by " " with STRIPCC:.true
     12 : ("‐" \x{E28090} U+2010 Pd Dash_Punctuation "HYPHEN")                              <-- replaceable by "-" with LUMP:.true
     13 : (<?> \x{C2AD} U+00AD Cf Format "SOFT HYPHEN")                                     <-- removable by STRIPIGNORABLE:.true
     14 : ("–" \x{E28093} U+2013 Pd Dash_Punctuation "EN DASH")                             <-- replaceable by "-" with LUMP:.true
     15 : ("—" \x{E28094} U+2014 Pd Dash_Punctuation "EM DASH")                             <-- replaceable by "-" with LUMP:.true
     16 : ("N" \x{4E} U+004E Lu Uppercase_Letter "LATIN CAPITAL LETTER N")
     17 : ("o" \x{6F} U+006F Ll Lowercase_Letter "LATIN SMALL LETTER O")
     18 : ("ë" \x{C3AB} U+00EB Ll Lowercase_Letter "LATIN SMALL LETTER E WITH DIAERESIS")
     19 : ("l" \x{6C} U+006C Ll Lowercase_Letter "LATIN SMALL LETTER L")
     20 : (<?> \x{EFB790} U+FDD0 Cn Unassigned "<noncharacter-FDD0>")                       <-- removeable with STRIPNA:.true
     21 : (<?> \x{0D} U+000D Cc Control "<control-000D>")
     22 : (<?> \x{0A} U+000A Cc Control "<control-000A>")                                   <-- CR+LF replaceable by " " with STRIPCC:.true
    */
    
string=; .RexxUnicode~C2U(string)=
-- '[07]Le　 ​Père[09]‐­–—Noël﷐[0D0A]'
-- 'U+0007 U+004C U+0065 U+3000 U+1680 U+200B U+0050 U+00E8 U+0072 U+0065 U+0009 U+2010 U+00AD U+2013 U+2014 U+004E U+006F U+00EB U+006C U+FDD0 U+000D U+000A'

-- Performs unicode case folding, to be able to do a case-insensitive string comparison.
tstring = .RexxUnicodeServices~utf8Transform(string, /*casefold:*/ .true)
tstring=; .RexxUnicode~C2U(tstring)=
-- '[07]le　 ​père[09]‐­–—noël﷐[0D0A]'
-- 'U+0007 U+006C U+0065 U+3000 U+1680 U+200B U+0070 U+00E8 U+0072 U+0065 U+0009 U+2010 U+00AD U+2013 U+2014 U+006E U+006F U+00EB U+006C U+FDD0 U+000D U+000A'

-- Strip "default ignorable characters" such as SOFT-HYPHEN or ZERO-WIDTH-SPACE.
tstring = .RexxUnicodeServices~utf8Transform(string, /*casefold:*/, /*lump:*/, /*nlf:*/, /*normalization:*/, /*stripCC*/, /*stripIgnorable:*/.true)
tstring=; .RexxUnicode~C2U(tstring)=
-- '[07]Le　 Père[09]‐–—Noël﷐[0D0A]'
-- 'U+0007 U+004C U+0065 U+3000 U+1680 U+0050 U+00E8 U+0072 U+0065 U+0009 U+2010 U+2013 U+2014 U+004E U+006F U+00EB U+006C U+FDD0 U+000D U+000A'

-- Lumps certain characters together.
tstring = .RexxUnicodeServices~utf8Transform(string, /*casefold:*/, /*lump:*/ .true)
tstring=; .RexxUnicode~C2U(tstring)=
-- '[07]Le  ​Père[09]-­--Noël﷐[0D0A]'
-- 'U+0007 U+004C U+0065 U+0020 U+0020 U+200B U+0050 U+00E8 U+0072 U+0065 U+0009 U+002D U+00AD U+002D U+002D U+004E U+006F U+00EB U+006C U+FDD0 U+000D U+000A'

-- NLF2LF: Convert LF, CRLF, CR and NEL into LF.
tstring = .RexxUnicodeServices~utf8Transform(string, /*casefold:*/, /*lump:*/, /*nlf:*/ 1)
tstring=; .RexxUnicode~C2U(tstring)=
-- '[07]Le　 ​Père[09]‐­–—Noël﷐[0A]'
-- 'U+0007 U+004C U+0065 U+3000 U+1680 U+200B U+0050 U+00E8 U+0072 U+0065 U+0009 U+2010 U+00AD U+2013 U+2014 U+004E U+006F U+00EB U+006C U+FDD0 U+000A'

-- NLF2LS: Convert LF, CRLF, CR and NEL into LS (U+2028 Zl 0 "LINE SEPARATOR").
tstring = .RexxUnicodeServices~utf8Transform(string, /*casefold:*/, /*lump:*/, /*nlf:*/ 2)
tstring=; .RexxUnicode~C2U(tstring)=
-- '[07]Le　 ​Père[09]‐­–—Noël﷐'
-- 'U+0007 U+004C U+0065 U+3000 U+1680 U+200B U+0050 U+00E8 U+0072 U+0065 U+0009 U+2010 U+00AD U+2013 U+2014 U+004E U+006F U+00EB U+006C U+FDD0 U+2028'

-- NLF2PS: convert LF, CRLF, CR and NEL into PS (U+2029 Zp 0 "PARAGRAPH SEPARATOR").
tstring = .RexxUnicodeServices~utf8Transform(string, /*casefold:*/, /*lump:*/, /*nlf:*/ 3)
tstring=; .RexxUnicode~C2U(tstring)=
-- '[07]Le　 ​Père[09]‐­–—Noël﷐ '
-- 'U+0007 U+004C U+0065 U+3000 U+1680 U+200B U+0050 U+00E8 U+0072 U+0065 U+0009 U+2010 U+00AD U+2013 U+2014 U+004E U+006F U+00EB U+006C U+FDD0 U+2029'

-- Strips and/or converts control characters.
tstring = .RexxUnicodeServices~utf8Transform(string, /*casefold:*/, /*lump:*/, /*nlf:*/, /*normalization:*/, /*stripCC*/ .true)
tstring=; .RexxUnicode~C2U(tstring)=
-- 'Le　 ​Père ‐­–—Noël﷐ '
-- 'U+004C U+0065 U+3000 U+1680 U+200B U+0050 U+00E8 U+0072 U+0065 U+0020 U+2010 U+00AD U+2013 U+2014 U+004E U+006F U+00EB U+006C U+FDD0 U+0020'

-- Strips all character markings.
-- This includes non-spacing, spacing and enclosing (i.e. accents).
-- This option works only with normalization.
tstring = .RexxUnicodeServices~utf8Transform(string, /*casefold:*/, /*lump:*/, /*nlf:*/, /*normalization:*/ 1, /*stripCC:*/, /*stripIgnorable:*/, /*stripMark:*/ .true)
tstring=; .RexxUnicode~C2U(tstring)=
-- '[07]Le　 ​Pere[09]‐­–—Noel﷐[0D0A]'
-- 'U+0007 U+004C U+0065 U+3000 U+1680 U+200B U+0050 U+0065 U+0072 U+0065 U+0009 U+2010 U+00AD U+2013 U+2014 U+004E U+006F U+0065 U+006C U+FDD0 U+000D U+000A'

-- Strips unassigned codepoints.
tstring = .RexxUnicodeServices~utf8Transform(string, /*casefold:*/, /*lump:*/, /*nlf:*/, /*normalization:*/, /*stripCC:*/, /*stripIgnorable:*/, /*stripMark:*/, /*stripNA*/ .true)
tstring=; .RexxUnicode~C2U(tstring)=
-- '[07]Le　 ​Père[09]‐­–—Noël[0D0A]'
-- 'U+0007 U+004C U+0065 U+3000 U+1680 U+200B U+0050 U+00E8 U+0072 U+0065 U+0009 U+2010 U+00AD U+2013 U+2014 U+004E U+006F U+00EB U+006C U+000D U+000A'

-- Application of several options.
tstring = .RexxUnicodeServices~utf8Transform(string, /*casefold:*/ .true, /*lump:*/ .true, /*nlf:*/, /*normalization:*/ 1, /*stripCC:*/ .true, /*stripIgnorable:*/ .true, /*stripMark:*/ .true, /*stripNA*/ .true)
tstring=; .RexxUnicode~C2U(tstring)=
-- 'le  pere ---noel '
-- 'U+006C U+0065 U+0020 U+0020 U+0070 U+0065 U+0072 U+0065 U+0020 U+002D U+002D U+002D U+006E U+006F U+0065 U+006C U+0020'

.RexxUnicodeCodepointSupplier~new(tstring, , .RexxUnicodeCharacter)==
    /*
     1  : ("l" \x{6C} U+006C Ll Lowercase_Letter "LATIN SMALL LETTER L")
     2  : ("e" \x{65} U+0065 Ll Lowercase_Letter "LATIN SMALL LETTER E")
     3  : (" " \x{20} U+0020 Zs Space_Separator "SPACE")                    <-- LUMP (was IDEOGRAPHIC SPACE)
     4  : (" " \x{20} U+0020 Zs Space_Separator "SPACE")                    <-- LUMP (was OGHAM SPACE MARK)
     5  : ("p" \x{70} U+0070 Ll Lowercase_Letter "LATIN SMALL LETTER P")
     6  : ("e" \x{65} U+0065 Ll Lowercase_Letter "LATIN SMALL LETTER E")
     7  : ("r" \x{72} U+0072 Ll Lowercase_Letter "LATIN SMALL LETTER R")
     8  : ("e" \x{65} U+0065 Ll Lowercase_Letter "LATIN SMALL LETTER E")
     9  : (" " \x{20} U+0020 Zs Space_Separator "SPACE")                    <-- STRIPCC (was TAB)
     10 : ("-" \x{2D} U+002D Pd Dash_Punctuation "HYPHEN-MINUS")            <-- LUMP (was HYPHEN)
     11 : ("-" \x{2D} U+002D Pd Dash_Punctuation "HYPHEN-MINUS")            <-- LUMP (was EN DASH)
     12 : ("-" \x{2D} U+002D Pd Dash_Punctuation "HYPHEN-MINUS")            <-- LUMP (was EM DASH)
     13 : ("n" \x{6E} U+006E Ll Lowercase_Letter "LATIN SMALL LETTER N")
     14 : ("o" \x{6F} U+006F Ll Lowercase_Letter "LATIN SMALL LETTER O")
     15 : ("e" \x{65} U+0065 Ll Lowercase_Letter "LATIN SMALL LETTER E")
     16 : ("l" \x{6C} U+006C Ll Lowercase_Letter "LATIN SMALL LETTER L")
     17 : (" " \x{20} U+0020 Zs Space_Separator "SPACE")                    <-- STRIPCC (was CR+LF)
    */
    
```


### Instance methods

The `RexxUnicodeServices` class defines no instance methods of its own.  
Since the class cannot be instantiated, the inherited instance methods from Object cannot be used either.


## RexxUnicode Class

The `RexxUnicode` class cannot be instantiated.  
Its services are provided through class methods.

`::requires "rxunicode.cls"`


### Methods inherited from the `ICU4ooRexxInterface` class

See the `ICU4ooRexxInterface` class.


### Methods inherited from the `RexxUnicodeServices` class

See the `RexxUnicodeServices` class.


### Constants

#### maxCodepoint

```rexx
::constant maxCodepoint 1114111
```


#### NFC, NFD, NFKC, NFKD

```rexx
-- Value to pass as the `normalization` argument to utf8Transform (default: 0 no normalization):
::constant NFC 1
::constant NFD 2
::constant NFKC 3
::constant NFKD 4
```


#### NLF2LF, NLF2LS, NLF2PS

```rexx
-- Value to pass as the 'nlf' argument to utf8Transform (default: 0 no transformation).
-- See the documentation of utf8Transform for a description of these values.
::constant NLF2LF 1
::constant NLF2LS 2
::constant NLF2PS 3
```

### Class methods

#### C2U

    .RexxUnicode~C2U(string, buffer=.nil)

Returns an ASCII string containing a sequence of U+XXXX (4 to 6 hex digits).  
If a buffer is passed as argument, the result is appended to the buffer.  
The result is either a string or the buffer passed as argument.


#### C2X

    .RexxUnicode~C2X(string, buffer=.nil)

Specialized C2X: Inserts a space between the codepoints.  
if a buffer is passed as argument, the result is appended to the buffer.  
The result is either a string or the buffer passed as argument.


#### character

    .RexxUnicode~character(identifier)

Gets a RexxUnicodeCharacter from a codepoint identifier.  
See the `codepoint` method for a definition of "codepoint identifier".

```rexx
-- Search by decimal value
.RexxUnicode~character("127877")=               -- ("🎅"  U+1F385 So Other_Symbol 2 "FATHER CHRISTMAS")

-- Search by U+ notation
.RexxUnicode~character("U+1F385")=              -- ("🎅"  U+1F385 So Other_Symbol 2 "FATHER CHRISTMAS")

-- Search by name
.RexxUnicode~character("FATHER CHRISTMAS")=     -- ("🎅"  U+1F385 So Other_Symbol 2 "FATHER CHRISTMAS")

-- search by loose name.
-- https://unicode.org/reports/tr44/#UAX44-LM2
.RexxUnicode~character(" Father christmas  ")=  -- ("🎅" \x{F09F8E85} U+1F385 So Other_Symbol "FATHER CHRISTMAS")

-- Search directly a character
.RexxUnicode~character("🎅")=                   -- ("🎅"  U+1F385 So Other_Symbol 2 "FATHER CHRISTMAS")

-- Only -1 is supported as a special codepoint
-- All other negative codepoints are invalid
.RexxUnicode~character(-1)=                     -- (An invalid character)
.RexxUnicode~character(-2)=                     -- Invalid code point -2; allowed range is 0 to 1114111 (U+10FFFF).
```


#### checkCodepoint

    .RexxUnicode~checkCodepoint(codepoint, minusOneAllowed=.false)

Returns .true if codepoint is in the range 0...RexxUnicode~maxCodepoint.  
The range U+D800-U+DFFF is allowed (surrogates).  
If minusOneAllowed is true then -1 is allowed (special value used in case of error).


#### codepoint

    .RexxUnicode~codepoint(identifier)

Gets a codepoint from a codepoint identifier.

A codepoint identifier may be:

- a decimal value (the codepoint itself),
- a U+ notation,
- a character name,
- or a UTF-8 encoded character.

```rexx
-- Search by decimal value
.RexxUnicode~codepoint("127877")=               -- 127877

-- Search by U+ notation
.RexxUnicode~codepoint("U+1F385")=              -- 127877

-- Search by name
.RexxUnicode~codepoint("FATHER CHRISTMAS")=     -- 127877

-- search by loose name.
-- https://unicode.org/reports/tr44/#UAX44-LM2
.RexxUnicode~codepoint(" Father christmas  ")=  -- 127877

-- Search directly a character
.RexxUnicode~codepoint("🎅")=                   -- 127877

-- Only -1 is supported as a special codepoint
-- All other negative codepoints are invalid
.RexxUnicode~codepoint(-1)=                     -- -1
.RexxUnicode~codepoint(-2)=                     -- Invalid code point -2; allowed range is 0 to 1114111 (U+10FFFF).
```


#### codepointCharName

    .RexxUnicode~codepointCharName(codepoint, raiseError=.false)

Returns the Unicode standard name for the given codepoint, or an empty string if not available.

This method requires the `ICU4ooRexx` class. If it is not loaded, the method returns an empty string or, if requested, raises an error.

 ```rexx
.RexxUnicode~codepointCharName(2448~x2d)=       -- 'OCR DASH'

-- The special codepoint -1 has no name
.RexxUnicode~codepointCharName(-1)=             -- ''
 ```

#### codepointCharNameAlias

    .RexxUnicode~codepointCharNameAlias(codepoint, raiseError=.false)

Returns the Unicode name alias for the given codepoint, or an empty string if not available.

This method requires the `ICU4ooRexx` class. If it is not loaded, the method returns an empty string or, if requested, raises an error.

 ```rexx
.RexxUnicode~codepointCharNameAlias(2448~x2d)=       -- 'MICR ON US SYMBOL'

-- The special codepoint -1 has no name alias
.RexxUnicode~codepointCharNameAlias(-1)=             -- ''
 ```


#### codepointExtendedCharName

    .RexxUnicode~codepointExtendedCharName(codepoint, raiseError=.false)

Returns the Unicode extended name alias for the given codepoint, or an empty string if not available.

All Unicode characters have an extended name.  
An extended name is either the standard name if defined, or a codepoint label algorithmically generated.

This method requires the `ICU4ooRexx` class. If it is not loaded, the method returns an empty string or, if requested, raises an error.

 ```rexx
.RexxUnicode~codepointExtendedCharName(2448~x2d)=   -- 'OCR DASH'

-- The special codepoint -1 has no name alias
.RexxUnicode~codepointExtendedCharName(-1)=         -- ''
 ```


#### codepointIsPrintable

    .RexxUnicode~codepointIsPrintable(codepoint)

Returns true if the codepoint is printable according to its category.

```rexx
.RexxUnicode~codepointIsPrintable(0)=             -- 0
.RexxUnicode~codepointIsPrintable(65)=            -- 1
.RexxUnicode~codepointIsPrintable("D800"~x2d)=    -- 0

-- The special codepoint -1 is not printable
.RexxUnicode~codepointIsPrintable(-1)=              -- 0
```

#### codepointPrintableString

    .RexxUnicode~codepointPrintableString( codepoint, buffer=.nil)

Returns a printable string representation for the given codepoint.  
Printable codepoints are returned as their UTF-8 character representation;
non-printable codepoints are returned as a Unicode escape notation.

If a buffer is passed as argument, the result is appended to the buffer.  
The result is either a string or the buffer passed as argument.

```rexx
.RexxUnicode~codepointPrintableString(0)=           -- '\u0000'
.RexxUnicode~codepointPrintableString(65)=          -- 'A'
.RexxUnicode~codepointPrintableString("D800"~x2d)=  -- '\uD800'

-- Since the special codepoint -1 is not printable, it is represented using Unicode escape notation
.RexxUnicode~codepointPrintableString(-1)=          -- '\UFFFFFFFF'
```


#### CODEPOINTUNICODEESCAPENOTATION


#### CODEPOINTUTF8ENCODING


#### D2U

    .RexxUnicode~D2U(codepoint, buffer=.nil)

Returns a U+XXXX string (4 to 6 hex digits) representing the codepoint passed as a decimal value.  
if a buffer is passed as argument, the result is appended to the buffer.  
The result is either a string or the buffer passed as argument.

The special codepoint -1 is represented using U+FFFFFF, even though the resulting value is not a valid Unicode scalar value

[https://www.unicode.org/versions/Unicode17.0.0/core-spec/appendix-a/#G7083](https://www.unicode.org/versions/Unicode17.0.0/core-spec/appendix-a/#G7083)


#### ESCAPE


#### H_UAX44_LM2


#### INFO


#### STRINGINFO


#### STRINGISASCII


#### STRINGTONFC


#### STRINGTONFD


#### STRINGTONFKC


#### STRINGTONFKC_CF


#### STRINGTONFKD


#### U2C

    .RexxUnicode~U2C(UPlusCodepoints, buffer=.nil)

Encodes an ASCII string of the form "U+XXXX.. U+XXXX.. ..." into a UTF-8 string.

The only valid separators are one or more spaces.

If a buffer is passed, the decoded characters are appended to it.  
The update is atomic: either all decoded characters are appended, or the buffer is left unchanged.  
If an error occurs, any characters appended during this call are discarded.

The result is either a string or the buffer passed as argument.


#### U2D

    .RexxUnicode~U2D(UPlusCodepoint, start=1, raiseError=.false [, >refLength]

Converts "U+XXXX.." to a decimal value.

If refLength is passed by the caller, extra characters after the U+XXXX.. are allowed.  
Otherwise UPlusCodepoint must be strictly "U+" followed by 4 to 6 hex digits.

The length of "U+XXXX.." is returned in refLength, allowing to skip it when parsing.

Returns the decimal value if no error occurs.  
Otherwise, returns -1 or, if requested, raises an error.


#### UAX44_LM2


#### UAX44_LM3


#### UNESCAPE


### Instance methods

The `RexxUnicode` class defines no instance methods of its own.
Since the class cannot be instantiated, the inherited instance methods from Object cannot be used either.


## RexxUnicodeCodepointSupplier Class

A `RexxUnicodeCodepointSupplier` instance is an iterator created from a 
UTF-8 string and allows enumeration of the string's codepoints.

`::requires "rxunicode.cls"`

### Class methods


### Instance methods

#### AVAILABLE


#### INDEX


#### INIT


#### ITEM


#### NEXT


## RexxUnicodeGraphemeSupplier Class

A `RexxUnicodeGraphemeSupplier` instance is an iterator created from a 
UTF-8 string and allows enumeration of the string's graphemes.

`::requires "rxunicode.cls"`

### Class methods


### Instance methods

#### AVAILABLE


#### INDEX


#### INIT


#### ITEM


#### NEXT


## RexxUnicodeStringIndexer Class

`::requires "rxunicode.cls"`

### Class methods


### Instance methods

#### CODEPOINTATINDEXC


#### CODEPOINTINDEXB


#### CODEPOINTINDEXES


#### ERRORS


#### GRAPHEMEATINDEXG


#### GRAPHEMEINDEXB


#### GRAPHEMEINDEXES


#### INIT


#### STRING


## RexxUnicodeCharacter Class

A RexxUnicodeCharacter instance is created from a codepoint and provides access 
to the properties of the corresponding Unicode character.

The special codepoint -1 can be represented using a RexxUnicodeCharacter.

`::requires "rxunicode.cls"`


### Class methods

#### PROPERTIES


### Instance methods

#### BIDICLASS


#### BIDICLASSNAME


#### BIDIMIRRORED


#### BOUNDCLASS


#### BOUNDCLASSNAME


#### CATEGORY


#### CATEGORYNAME


#### CHARWIDTH


#### CODEPOINT


#### COMBININGCLASS


#### COMBININGCLASSNAME


#### COMPARETO


#### CONTROLBOUNDARY


#### DECOMPOSITIONTYPE


#### DECOMPOSITIONTYPENAME


#### EXTENDEDNAME


#### IGNORABLE


#### INFO


#### INIT


#### ISLOWER


#### ISUPPER


#### NAME


#### NAMEALIAS


#### STRING


#### TOLOWER


#### TOLOWERFULL


#### TOTITLE


#### TOTITLEFULL


#### TOUPPER


#### TOUPPERFULL


#### UNAME


#### UTF8

## ICU4ooRexxInterface Class

`::requires "rxunicode.cls"`

### Class methods

#### ACTIVATE


#### ASSERTICU4OOREXXISREGISTERED


#### ICU4OOREXXISREGISTERED


#### ICU4OOREXXPACKAGE


#### REGISTERICU4OOREXX


### Instance methods



