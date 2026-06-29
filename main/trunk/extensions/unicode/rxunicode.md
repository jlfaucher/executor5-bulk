# Unicode support

The `RexxUnicodeServices` class is a native class that exposes the Unicode services
supported by ooRexx. Its current implementation is based on the [utf8proc][utf8proc]
library embedded in ooRexx.


`rxunicode.cls` is an optional package that defines:
- The `RexxUnicode` class, a subclass of `RexxUnicodeServices`.
- The `RexxUnicodeCharacter` class.
- The `RexxUnicodeCodepointSupplier` class.

> [!CAUTION]
> Testing these classes from ooRexxShell with TUTOR enabled showed that a
> defensive barrier is needed.
> The `requestBaseString` method ensures that any string passed as an argument
> is a `.String` instance, not an instance of a subclass of `.String`.
> No attempt is made to obtain a string from these arguments.
> This check is intended to detect accidental mixing of Unicode-aware objects
> with byte strings and to allow users to adapt their code accordingly
> (typically by requesting a `.String` from their TUTOR objects).
> 
> See the examples at the end of rxunicode.cls showing the internal errors that
> can occur when mixing graphemes and bytes.

**Note on terminology:** This documentation and the code comments use the term "codepoint"
instead of "code point", except when quoting definitions from the Unicode Standard.


## RexxUnicodeServices

The `RexxUnicodeServices` class cannot be instantiated.
Its services are provided through class methods.

This native class is always available; there is no need to load `rxunicode.cls`.

In the examples, you can replace `RexxUnicodeServices` by `RexxUnicode` if you loaded `rxunicode.cls`.


### Class methods

#### unicodeVersion

```
unicodeVersion()
```

Returns the Unicode version supported by the `utf8proc` library.

```
say .RexxUnicodeServices~unicodeVersion        -- 17.0.0 (for example)

```


#### utf8DecodeCodepoint

```
utf8DecodeCodepoint(
    utf8string,     -- (in) A UTF-8 string.
    indexB,         -- (in) The byte index (1-based) of the encoded codepoint in rexxString.
    >refSizeB,      -- (out, optional) The number of bytes read to decode the codepoint,
                    --                 negative if error,
                    --                 or 0 if indexB is outside the utf8string index range.
    >refErrorCode   -- (out, optional) The null string "" if a valid codepoint could be read,
                    --                 or the error code otherwise.
    >refErrorMsg    -- (out, optional) The null string "" if a valid codepoint could be read,
                    --                 or the error message otherwise.
    )
```

Returns the next codepoint (an integer) at position `indexB` of `utf8string`, or -1 in case of error.

`refSizeB` returns the size in bytes of the decoded codepoint.  
If `indexB` is outside the `utf8string` index range, the returned size is 0.  
In case of error, the returned size is negative, indicating the number of bytes to skip
to follow the U+FFFD Substitution of Maximal Subparts.  
`refErrorCode` and `refErrorMsg` provide a detailed description of the error.


Error codes and messages:

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
-- Example 1 - the decoding part of a CodePointSupplier
::method decode
    expose string indexB sizeB codepoint byteSequence
    use strict arg -- none
    codepoint = .RexxUnicodeServices~utf8DecodeCodepoint(string, indexB, >sizeB)
    if sizeB == 0 then codepoint = .nil -- end of string
    if sizeB < 0 then codepoint = "FFFD"~x2d -- the current byte sequence is invalid, return the replacement character
    sizeB = abs(sizeB)
    byteSequence = string~substr(indexB, sizeB)
```


```
-- Example 2 - indexation of codepoints for direct access
::method indexCodepoints class
    use strict arg string, >codepointIndexes, >errors
    string = requestBaseString(string)
    codepointIndexes = .array~new
    errors = .array~new
    indexB = 1
    previousCodepoint = -1
    previousCodepointIndexB = -1

    forever:
        codepoint = .RexxUnicodeServices~utf8DecodeCodepoint(string, indexB, >sizeB, >errorCode, >errorMsg)
        if sizeB == 0 then return -- End of string
        if sizeB < 0 then signal error

        codepointIndexes~append(indexB)
        previousCodepoint = codepoint
        previousCodepointIndexB = indexB
        indexB += sizeB
    signal forever

    error:
        errors~append("start byte-position" indexB ":" errorMsg)
        codepointIndexes~append(-indexB) -- a negative index means "error"
        previousCodepoint = codepoint
        previousCodepointIndexB = indexB
        indexB += abs(sizeB) -- skip the invalid bytes
        signal forever
```


#### utf8EncodeCodepoint

```
utf8EncodeCodepoint(
    rexxCodepoint,  -- (in) The codepoint to encode (an integer).
    destination,    -- (in-out) The mutable buffer to update.
    >refSizeB       -- (out, optional) The size of the encoded byte sequence (0..4)
                    --                 The size is 0 if the codepoint is not in the
                    --                 range 0..10FFFF.
    )
```

Appends a codepoint's UTF-8 encoding into the specified mutable buffer.  
The surrogates 0xd800-0xdfff are encoded, however these are actually invalid in UTF-8.

Returns the mutable buffer passed with the destination argument.


```
mb = .MutableBuffer~new
do codepoint over ("006F"~x2d, "00EB"~x2d, "20AC"~x2d, "1F385"~x2d, "110000"~x2d)
    .RexxUnicodeServices~utf8EncodeCodepoint(codepoint, mb, >size)
    say "Append the UTF-8 encoding of" codepoint "to mb: size =" size "mb =" mb
end

/*
Append the UTF-8 encoding of 111 to mb: size = 1 mb = o
Append the UTF-8 encoding of 235 to mb: size = 2 mb = oë
Append the UTF-8 encoding of 8364 to mb: size = 3 mb = oë€
Append the UTF-8 encoding of 127877 to mb: size = 4 mb = oë€🎅
Append the UTF-8 encoding of 1114112 to mb: size = 0 mb = oë€🎅
*/
```


#### utf8procVersion

```
utf8procVersion()
```

Returns the version of the `utf8proc` library.

```
say .RexxUnicodeServices~utf8procVersion        -- 2.11.3 (for example)

```


#### utf8Transform

```
utf8Transform(
    utf8string,             -- The UTF-8 string to transform
    casefold = .false,      -- Unicode case folding
    lump= .false,           -- E.g. HYPHEN U+2010 and MINUS U+2212 to ASCII "-"
    nlf = 0,                -- Related to LF, CRLF, CR, NEL
    normalization = 0,      -- NFC=1, NFD=2, NFKC=3, NFKD=4
    stripCC = .false,       -- Strips and/or converts control characters
    stripIgnorable= .false, -- Strip ignorable characters
    stripMark = .false,     -- Strips all character markings (i.e. accents)
    stripNA = .false        -- Strips unassigned codepoints
    )
```

The surrogates 0xd800-0xdfff are encoded, however these are actually invalid in UTF-8.

typedef enum {
  /** The given UTF-8 input is NULL terminated. */
  UTF8PROC_NULLTERM  = (1<<0),
  /** Unicode Versioning Stability has to be respected. */
  UTF8PROC_STABLE    = (1<<1),
  /** Compatibility decomposition (i.e. formatting information is lost). */
  UTF8PROC_COMPAT    = (1<<2),
  /** Return a result with composed characters. */
  UTF8PROC_COMPOSE   = (1<<3),
  /** Return a result with decomposed characters. */
  UTF8PROC_DECOMPOSE = (1<<4),
  /** Strip "default ignorable characters" such as SOFT-HYPHEN or ZERO-WIDTH-SPACE. */
  UTF8PROC_IGNORE    = (1<<5),
  /** Return an error, if the input contains unassigned codepoints. */
  UTF8PROC_REJECTNA  = (1<<6),
  /**
   * Indicating that NLF-sequences (LF, CRLF, CR, NEL) are representing a
   * line break, and should be converted to the codepoint for line
   * separation (LS).
   */
  UTF8PROC_NLF2LS    = (1<<7),
  /**
   * Indicating that NLF-sequences are representing a paragraph break, and
   * should be converted to the codepoint for paragraph separation
   * (PS).
   */
  UTF8PROC_NLF2PS    = (1<<8),
  /** Indicating that the meaning of NLF-sequences is unknown. */
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
  /**
   * Inserts 0xFF bytes at the beginning of each sequence which is
   * representing a single grapheme cluster (see UAX#29).
   */
  UTF8PROC_CHARBOUND = (1<<11),
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
} utf8proc_option_t;


/** Decompose a codepoint into an array of codepoints.
 *
 * @param codepoint the codepoint.
 * @param dst the destination buffer.
 * @param bufsize the size of the destination buffer.
 * @param options one or more of the following flags:
 * - @ref UTF8PROC_REJECTNA  - return an error if `codepoint` is unassigned
 * - @ref UTF8PROC_IGNORE    - strip "default ignorable" codepoints
 * - @ref UTF8PROC_CASEFOLD  - apply Unicode casefolding
 * - @ref UTF8PROC_COMPAT    - replace certain codepoints with their
 *                             compatibility decomposition
 * - @ref UTF8PROC_CHARBOUND - insert 0xFF bytes before each grapheme cluster
 * - @ref UTF8PROC_LUMP      - lump certain different codepoints together
 * - @ref UTF8PROC_STRIPMARK - remove all character marks
 * - @ref UTF8PROC_STRIPNA   - remove unassigned codepoints
 * @param last_boundclass
 * Pointer to an integer variable containing
 * the previous codepoint's (boundclass + indic_conjunct_break << 1) if the @ref UTF8PROC_CHARBOUND
 * option is used.  If the string is being processed in order, this can be initialized to 0 for
 * the beginning of the string, and is thereafter updated automatically.  Otherwise, this parameter is ignored.
 *
 * In the current version of utf8proc, the maximum destination buffer with the @ref UTF8PROC_DECOMPOSE
 * option is 4 elements (or double that with @ref UTF8PROC_CHARBOUND), so this is a good default size.
 * However, this may increase in future Unicode versions, so you should always check the return value
 * as described below.
 *
 * @return
 * In case of success, the number of codepoints written is returned; in case
 * of an error, a negative error code is returned (utf8proc_errmsg()).
 * @par
 * If the number of written codepoints would be bigger than `bufsize`, the
 * required buffer size is returned, while the buffer will be overwritten with
 * undefined data.
 */


 * @param buffer the (native-endian UTF-32) unicode codepoints to re-encode.
 * @param length the length (in codepoints) of the buffer.
 * @param options a bitwise or (`|`) of one or more of the following flags:
 * - @ref UTF8PROC_NLF2LS  - convert LF, CRLF, CR and NEL into LS
 * - @ref UTF8PROC_NLF2PS  - convert LF, CRLF, CR and NEL into PS
 * - @ref UTF8PROC_NLF2LF  - convert LF, CRLF, CR and NEL into LF
 * - @ref UTF8PROC_STRIPCC - strip or convert all non-affected control characters
 * - @ref UTF8PROC_COMPOSE - try to combine decomposed codepoints into composite
 *                           codepoints
 * - @ref UTF8PROC_STABLE  - prohibit combining characters that would violate
 *                           the unicode versioning stability


```
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
```










```
    -- Illustration of transformations with this string:
    string = "\N{BEL}Le\N{IDEOGRAPHIC SPACE}\N{OGHAM SPACE MARK}\N{ZERO-WIDTH-SPACE}Père\t\N{HYPHEN}\N{SOFT-HYPHEN}\N{EN DASH}\N{EM DASH}Noël\x{EFB790}\r\n"

    text~UnicodeCharacters==
        an Array (shape [22], 22 items)
         1  : ( ""    U+0007 Cc 0 "", "ALERT", "BEL" )                                                  <-- removable with STRIPCC:.true
         2  : ( "L"   U+004C Lu 1 "LATIN CAPITAL LETTER L" )
         3  : ( "e"   U+0065 Ll 1 "LATIN SMALL LETTER E" )
         4  : ( "　"  U+3000 Zs 2 "IDEOGRAPHIC SPACE" )                                                 <-- replaceable by " " with LUMP:.true
         5  : ( " "   U+1680 Zs 1 "OGHAM SPACE MARK" )                                                  <-- replaceable by " " with LUMP:.true
         6  : ( "​"    U+200B Cf 0 "ZERO WIDTH SPACE", "ZWSP" )                                          <-- removable by STRIPIGNORABLE:.TRUE
         7  : ( "P"   U+0050 Lu 1 "LATIN CAPITAL LETTER P" )
         8  : ( "è"   U+00E8 Ll 1 "LATIN SMALL LETTER E WITH GRAVE" )
         9  : ( "r"   U+0072 Ll 1 "LATIN SMALL LETTER R" )
         10 : ( "e"   U+0065 Ll 1 "LATIN SMALL LETTER E" )
         11 : ( ""    U+0009 Cc 0 "", "CHARACTER TABULATION", "HORIZONTAL TABULATION", "HT", "TAB" )    <-- replaceable by " " with STRIPCC:.true
         12 : ( "‐"   U+2010 Pd 1 "HYPHEN" )                                                            <-- replaceable by "-" with LUMP:.true
         13 : ( "­"   U+00AD Cf 1 "SOFT HYPHEN", "SHY" )                                                <-- removable by STRIPIGNORABLE:.true
         14 : ( "–"   U+2013 Pd 1 "EN DASH" )                                                           <-- replaceable by "-" with LUMP:.true
         15 : ( "—"   U+2014 Pd 1 "EM DASH" )                                                           <-- replaceable by "-" with LUMP:.true
         16 : ( "N"   U+004E Lu 1 "LATIN CAPITAL LETTER N" )
         17 : ( "o"   U+006F Ll 1 "LATIN SMALL LETTER O" )
         18 : ( "ë"   U+00EB Ll 1 "LATIN SMALL LETTER E WITH DIAERESIS" )
         19 : ( "l"   U+006C Ll 1 "LATIN SMALL LETTER L" )
         20 : ( "﷐"   U+FDD0 Cn 1 "" )                                                                 <-- removeable with STRIPNA:.true
         21 : ( ""    U+000D Cc 0 "", "CARRIAGE RETURN", "CR" )
         22 : ( ""    U+000A Cc 0 "", "LINE FEED", "NEW LINE", "END OF LINE", "LF", "NL", "EOL" )       <-- CR+LF replaceable by " " with STRIPCC:.true

    text=                                                               -- T'[07]Le　 ​Père[09]‐­–—Noël﷐[0D0A]'

    -- Performs unicode case folding, to be able to do a case-insensitive string comparison.
    .Unicode~utf8proc_transform(text~string, casefold:.true)=           --  '[07]le　 ​père[09]‐­–—noël﷐[0D0A]'

    -- Strip "default ignorable characters" such as SOFT-HYPHEN or ZERO-WIDTH-SPACE
    .Unicode~utf8proc_transform(text~string, stripIgnorable:.true)=     --  '[07]Le　 Père[09]‐–—Noël﷐[0D0A]'

    -- Lumps certain characters together. See lump.md for details:
    -- https://github.com/JuliaStrings/utf8proc/blob/master/lump.md
    -- E.g. HYPHEN U+2010 and MINUS U+2212 to ASCII "-"
    -- jlf: I was expecting to have only one space and one "-" but that's not the case
    -- Seems working as designed... All the concerned characters become the same character, but still remain distinct characters.
    .Unicode~utf8proc_transform(text~string, lump:.true)=               --  '[07]Le  ​Père[09]-­--Noël﷐[0D0A]'
         4  : ( " "   U+0020 Zs 1 "SPACE", "SP" )
         5  : ( " "   U+0020 Zs 1 "SPACE", "SP" )
         6  : ( "​"    U+200B Cf 0 "ZERO WIDTH SPACE", "ZWSP" )
         12 : ( "-"   U+002D Pd 1 "HYPHEN-MINUS" )
         13 : ( "­"   U+00AD Cf 1 "SOFT HYPHEN", "SHY" )
         14 : ( "-"   U+002D Pd 1 "HYPHEN-MINUS" )
         15 : ( "-"   U+002D Pd 1 "HYPHEN-MINUS" )

    -- NLF2LF: Convert LF, CRLF, CR and NEL into LF
    .Unicode~utf8proc_transform(text~string, NLF:1)=                    --  '[07]Le　 ​Père[09]‐­–—Noël﷐[0A]'

    -- NLF2LS: Convert LF, CRLF, CR and NEL into LS (U+2028 Zl 0 "LINE SEPARATOR")
    .Unicode~utf8proc_transform(text~string, NLF:2)=                    --  '[07]Le　 ​Père[09]‐­–—Noël﷐
'

    -- NLF2PS: convert LF, CRLF, CR and NEL into PS (U+2029 Zp 0 "PARAGRAPH SEPARATOR")
    .Unicode~utf8proc_transform(text~string, NLF:3)=                    --  '[07]Le　 ​Père[09]‐­–—Noël﷐
'

    -- Strips and/or converts control characters.
    .Unicode~utf8proc_transform(text~string, stripCC:.true)=            --  'Le　 ​Père ‐­–—Noël﷐ '

    -- Strips all character markings.
    -- This includes non-spacing, spacing and enclosing (i.e. accents).
    -- This option works only with normalization.
    .Unicode~utf8proc_transform(text~string, stripMark:.true, normalization:1)=  --  '[07]Le　 ​Pere[09]‐­–—Noel﷐[0D0A]'

    -- Strips unassigned codepoints.
    .Unicode~utf8proc_transform(text~string, stripNA:.true)=            --  '[07]Le　 ​Père[09]‐­–—Noël[0D0A]'

    -- Application of several options (abbreviated names)
    .Unicode~utf8proc_transform(text~string, casef:.true, lump:.true, norm:1, stripi:.true, stripc:.true, stripm:.true, stripn:.true)= --  'le  pere ---noel '
        an Array (shape [17], 17 items)
         1  : ( "l"   U+006C Ll 1 "LATIN SMALL LETTER L" )
         2  : ( "e"   U+0065 Ll 1 "LATIN SMALL LETTER E" )
         3  : ( " "   U+0020 Zs 1 "SPACE", "SP" )               <-- LUMP (was IDEOGRAPHIC SPACE)
         4  : ( " "   U+0020 Zs 1 "SPACE", "SP" )               <-- LUMP (was OGHAM SPACE MARK)
         5  : ( "p"   U+0070 Ll 1 "LATIN SMALL LETTER P" )
         6  : ( "e"   U+0065 Ll 1 "LATIN SMALL LETTER E" )
         7  : ( "r"   U+0072 Ll 1 "LATIN SMALL LETTER R" )
         8  : ( "e"   U+0065 Ll 1 "LATIN SMALL LETTER E" )
         9  : ( " "   U+0020 Zs 1 "SPACE", "SP" )               <-- STRIPCC (was TAB)
         10 : ( "-"   U+002D Pd 1 "HYPHEN-MINUS" )              <-- LUMP (was HYPHEN)
         11 : ( "-"   U+002D Pd 1 "HYPHEN-MINUS" )              <-- LUMP (was EN DASH)
         12 : ( "-"   U+002D Pd 1 "HYPHEN-MINUS" )              <-- LUMP (was EM DASH)
         13 : ( "n"   U+006E Ll 1 "LATIN SMALL LETTER N" )
         14 : ( "o"   U+006F Ll 1 "LATIN SMALL LETTER O" )
         15 : ( "e"   U+0065 Ll 1 "LATIN SMALL LETTER E" )
         16 : ( "l"   U+006C Ll 1 "LATIN SMALL LETTER L" )
         17 : ( " "   U+0020 Zs 1 "SPACE", "SP" )               <-- STRIPCC (was CR+LF)

```



-------------------------------------------------


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

### Instance methods


## RexxUnicode

The `RexxUnicode` class cannot be instantiated.
Its services are provided through class methods.

`::requires "rxunicode.cls"`


### Methods inherited from the `RexxUnicodeServices` class

See the `RexxUnicodeServices` class.


### Class methods

### Instance methods


## RexxUnicodeCharacter

A RexxUnicodeCharacter instance is created from a codepoint and provides access 
to the properties of the corresponding Unicode character.

`::requires "rxunicode.cls"`

### Class methods

### Instance methods


## RexxUnicodeCodepointSupplier

A `RexxUnicodeCodepointSupplier` instance is an iterator created from a 
UTF-8 string and allows enumeration of the string's codepoints.

`::requires "rxunicode.cls"`

### Class methods

### Instance methods


[utf8proc]: https://juliastrings.github.io/utf8proc/
