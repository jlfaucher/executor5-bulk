# rxunicode Reference Documentation

The `RexxUnicodeServices` class is a native class that exposes the Unicode services
supported by `ooRexx`. Its current implementation is based on the [`utf8proc`][utf8proc]
library embedded in `ooRexx`.


`rxunicode.cls` is an optional package that defines:

- The `RexxUnicode` class, a subclass of `RexxUnicodeServices`.
- The `RexxUnicodeCodepointSupplier` class.
- The `RexxUnicodeGraphemeSupplier` class.
- The `RexxUnicodeStringIndexer` class.
- The `RexxUnicodeCharacter` class.
- The `ICU4ooRexxInterface` class.

> [!CAUTION]  
> Testing these classes from `ooRexxShell` with `TUTOR` enabled showed that a
> defensive barrier is needed. This problem is not limited to `TUTOR` classes;
> it can occur with any `String` subclass that overrides `~length` or `~substr`.
>
> In case of errors like
>
> - `Argument <name> class: expected String, found Text.`
> - `Argument <name> class: expected String, found Bytes.`
>
> use `tutor off` for a permanent workaround,  
> or use `~string` for a temporary workaround.
>
> The `requestBaseString` routine ensures that any string passed as an argument
> is a `.String` instance, not an instance of a subclass of `.String`.
>
> No attempt is made to obtain a string from these arguments.
>
> This check is intended to detect accidental mixing of Unicode-aware objects
> with byte strings and to allow users to adapt their code accordingly
> (typically by requesting a `.String` from their Unicode objects).
> 
> See the [examples][examples_internal_errors] at the end of `rxunicode.cls`
> showing the internal errors that can occur when mixing graphemes and bytes.

**Note on terminology:** This documentation and the code comments use the term "codepoint"
instead of "code point", except when quoting definitions from the Unicode Standard.

**Note on examples:** The special syntax `expression=` or `expression==` is a feature of `ooRexxShell`.
It is a shortcut for displaying the result of `expression`.
Collections are displayed either in compact form (when the expression ends with `=`)
or in expanded form (when it ends with `==`).
If you are using `rexxtry` or running a script, use `say expression` instead.


## 1.   RexxUnicodeServices Class

The `RexxUnicodeServices` class cannot be instantiated.  
Its services are provided through class methods.

This native class is always available; there is no need to load `rxunicode.cls`.

In the examples, you can replace `RexxUnicodeServices` by `RexxUnicode` if you loaded `rxunicode.cls`.


### 1.1.   Class methods

Method-to-Unicode property mapping (PropertyAliases.txt):

<table>
    <thead>
      <tr>
        <th><code>Method</code></th>
        <th><code>Property type</code></th>
        <th><code>Short name</code></th>
        <th><code>Long name</code></th>
        <th><code>Comment</code></th>
      </tr>
    </thead>
    <tbody>
      <tr>
        <td><code>codepointBidiClass</code></td>
        <td><code>Enumerated</code></td>
        <td><code>bc</code></td>
        <td><code>Bidi_Class</code></td>
        <td></td>
      </tr>
      <tr>
        <td><code>codepointBidiMirrored</code></td>
        <td><code>Binary</code></td>
        <td><code>Bidi_M</code></td>
        <td><code>Bidi_Mirrored</code></td>
        <td></td>
      </tr>
      <tr>
        <td><code>codepointBoundClass</code></td>
        <td><code>Enumerated</code></td>
        <td><code>GCB</code></td>
        <td><code>Grapheme_Cluster_Break</code></td>
        <td></td>
      </tr>
      <tr>
        <td><code>codepointCategory</code></td>
        <td><code>Enumerated</code></td>
        <td><code>gc</code></td>
        <td><code>General_Category</code></td>
        <td></td>
      </tr>
      <tr>
        <td><code>codepointCharWidth</code></td>
        <td></td>
        <td></td>
        <td></td>
        <td><code>not a Unicode property</code></td>
      </tr>
      <tr>
        <td><code>codepointCombiningClass</code></td>
        <td><code>Enumerated</code></td>
        <td><code>ccc</code></td>
        <td><code>Canonical_Combining_Class</code></td>
        <td></td>
      </tr>
      <tr>
        <td><code>codepointControlBoundary</code></td>
        <td></td>
        <td></td>
        <td></td>
        <td><code>not a Unicode property (this method is likely to be deprecated)</code></td>
      </tr>
      <tr>
        <td><code>codepointDecompositionType</code></td>
        <td><code>Enumerated</code></td>
        <td><code>td</code></td>
        <td><code>Decomposition_Type</code></td>
        <td></td>
      </tr>
      <tr>
        <td><code>codepointEastAsianWidth</code></td>
        <td><code>Enumerated</code></td>
        <td><code>ea</code></td>
        <td><code>East_Asian_Width</code></td>
        <td><code>Not available</code></td>
      </tr>
      <tr>
        <td><code>codepointEastAsianWidthIsAmbiguous</code></td>
        <td><code>Binary</code></td>
        <td></td>
        <td></td>
        <td><code>True if East Asian width class A</code></td>
      </tr>
      <tr>
        <td><code>codepointIgnorable</code></td>
        <td><code>Binary</code></td>
        <td><code>DI</code></td>
        <td><code>Default_Ignorable_Code_Point</code></td>
        <td></td>
      </tr>
      <tr>
        <td><code>codepointIsLower</code></td>
        <td><code>Binary</code></td>
        <td><code>Lower</code></td>
        <td><code>Lowercase</code></td>
        <td></td>
      </tr>
      <tr>
        <td><code>codepointIsUpper</code></td>
        <td><code>Binary</code></td>
        <td><code>Upper</code></td>
        <td><code>Uppercase</code></td>
        <td></td>
      </tr>
      <tr>
        <td><code>codepointToLower</code></td>
        <td><code>String</code></td>
        <td><code>slc</code></td>
        <td><code>Simple_Lowercase_Mapping</code></td>
        <td></td>
      </tr>
      <tr>
        <td><code>codepointToTitle</code></td>
        <td><code>String</code></td>
        <td><code>stc</code></td>
        <td><code>Simple_Titlecase_Mapping</code></td>
        <td></td>
      </tr>
      <tr>
        <td><code>codepointToUpper</code></td>
        <td><code>String</code></td>
        <td><code>suc</code></td>
        <td><code>Simple_Uppercase_Mapping</code></td>
        <td></td>
      </tr>
    </tbody>
</table>


#### 1.1.1.   codepointBidiClass

    .RexxUnicodeServices~codepointBidiClass(codepoint [, [>refCode] [, >refLabel]])

Returns the bidirectional character type of `codepoint` as an enumeration value.

- If provided, `refCode` receives the short name or `"?"`.
- If provided, `refLabel` receives the long name or`"Code point not explicitly listed for Bidi_Class"`.

The returned enumeration value is implementation-specific.  
For portability, use refCode instead.

**Enumeration values**

```rexx
-- first column: short name (from PropertyValueAliases.txt and DerivedBidiClass.txt)
-- second column: long name (from PropertyValueAliases.txt and DerivedBidiClass.txt)
-- third column: description (from https://www.unicode.org/reports/tr9/#Table_Bidirectional_Character_Types)
"AL",   "Arabic_Letter"             -- Right-to-Left Arabic
"AN",   "Arabic_Number"             -- Arabic Number
"B",    "Paragraph_Separator"       -- Paragraph Separator
"BN",   "Boundary_Neutral"          -- Boundary Neutral
"CS",   "Common_Separator"          -- Common Number Separator
"EN",   "European_Number"           -- European Number
"ES",   "European_Separator"        -- European Number Separator
"ET",   "European_Terminator"       -- European Number Terminator
"FSI",  "First_Strong_Isolate"      -- First Strong Isolate
"L",    "Left_To_Right"             -- Left-to-Right
"LRE",  "Left_To_Right_Embedding"   -- Left-to-Right Embedding
"LRI",  "Left_To_Right_Isolate"     -- Left-to-Right Isolate
"LRO",  "Left_To_Right_Override"    -- Left-to-Right Override
"NSM",  "Nonspacing_Mark"           -- Nonspacing Mark
"ON",   "Other_Neutral"             -- Other Neutrals
"PDF",  "Pop_Directional_Format"    -- Pop Directional Format
"PDI",  "Pop_Directional_Isolate"   -- Pop Directional Isolate
"R",    "Right_To_Left"             -- Right-to-Left
"RLE",  "Right_To_Left_Embedding"   -- Right-to-Left Embedding
"RLI",  "Right_To_Left_Isolate"     -- Right-to-Left Isolate
"RLO",  "Right_To_Left_Override"    -- Right-to-Left Override
"S",    "Segment_Separator"         -- Segment Separator
"WS",   "White_Space"               -- Whitespace
```

**Examples:**

```rexx
.RexxUnicodeServices~codepointBidiClass("0608"~x2d, >code, >label)=; code=; label=      --  5; 'AL'; 'Arabic_Letter'
.RexxUnicodeServices~codepointBidiClass("FFFFFF"~x2d, >code, >label)=; code=; label=    --  0; '?'; 'Code point not explicitly listed for Bidi_Class'
.RexxUnicodeServices~codepointBidiClass(-1, >code, >label)=; code=; label=              --  0; '?'; 'Code point not explicitly listed for Bidi_Class'

```

#### 1.1.2.   codepointBidiMirrored

    .RexxUnicodeServices~codepointBidiMirrored(codepoint)

Returns `.true` if the `Bidi_Mirrored` property value of `codepoint` is `Yes`.

[https://unicode.org/reports/tr9/#Mirroring][unicode_standard_annex_9_mirroring]


#### 1.1.3.   codepointBoundClass

    .RexxUnicodeServices~codepointBoundClass(codepoint [, [>refCode] [, >refLabel]])

Returns the `Grapheme_Cluster_Break` property value of `codepoint` as an enumeration value.

- If provided, `refCode` receives the short name.
- If provided, `refLabel` receives the long name.

The returned enumeration value is implementation-specific.  
For portability, use refCode instead.

**Enumeration values**

```rexx
-- https://www.unicode.org/reports/tr29/#Grapheme_Cluster_Boundaries
-- first column: short name (from PropertyValueAliases.txt)
-- second column: long name (from PropertyValueAliases.txt and auxiliary/GraphemeBreakProperty.txt)
"CN",       "Control"
"CR",       "CR"
"EX",       "Extend"
"L",        "L"
"LF",       "LF"
"LV",       "LV"
"LVT",      "LVT"
"PP",       "Prepend"
"RI",       "Regional_Indicator"
"SM",       "SpacingMark"
"Start",    "Start"
"T",        "T"
"V",        "V"
"XX",       "Other"
"ZWJ",      "ZWJ"                   -- Zero Width Joiner

/* the following are no longer used in Unicode 11, but we keep
 the constants here for backward compatibility */
"EB",       "E_Base"                -- Emoji Base
"EBG",      "E_Base_GAZ"            -- E_BASE + GLUE_AFTER_ZJW
"EM",       "E_Modifier"            -- Emoji Modifier
"GAZ",      "Glue_After_Zwj"

/* the Extended_Pictographic property is used in the Unicode 11
 grapheme-boundary rules */
"EP",       "Extended_Pictographic"
"EZWG",     "E_ZWG"
```

**Examples:**

```rexx
.RexxUnicodeServices~codepointBoundClass("AC01"~x2d, >code, >label)=; code=; label=      --  10; 'LVT'; 'LVT'
.RexxUnicodeServices~codepointBoundClass("FFFFFF"~x2d, >code, >label)=; code=; label=    --  1; 'XX'; 'Other'
.RexxUnicodeServices~codepointBoundClass(-1, >code, >label)=; code=; label=              --  1; 'XX'; 'Other'

```


#### 1.1.4.   codepointCategory

    .RexxUnicodeServices~codepointCategory(codepoint [, [>refCode] [, >refLabel]])

Returns the `General_Category` property value of `codepoint` as an enumeration value.

- If provided, `refCode` receives the short name.
- If provided, `refLabel` receives the long name.

The returned enumeration value is implementation-specific.  
For portability, use refCode instead.

**Enumeration values**

```rexx
-- https://www.unicode.org/versions/Unicode17.0.0/core-spec/chapter-4/#G124142
-- first column: short name
-- second column: long name
"Cc", "Control"
"Cf", "Format"
"Cn", "Unassigned"
"Co", "Private_Use"
"Cs", "Surrogate"
"Ll", "Lowercase_Letter"
"Lm", "Modifier_Letter"
"Lo", "Other_Letter"
"Lt", "Titlecase_Letter"
"Lu", "Uppercase_Letter"
"Mc", "Spacing_Mark"
"Me", "Enclosing_Mark"
"Mn", "Nonspacing_Mark"
"Nd", "Decimal_Number"
"Nl", "Letter_Number"
"No", "Other_Number"
"Pc", "Connector_Punctuation"
"Pd", "Dash_Punctuation"
"Pe", "Close_Punctuation"
"Pf", "Final_Punctuation"
"Pi", "Initial_Punctuation"
"Po", "Other_Punctuation"
"Ps", "Open_Punctuation"
"Sc", "Currency_Symbol"
"Sk", "Modifier_Symbol"
"Sm", "Math_Symbol"
"So", "Other_Symbol"
"Zl", "Line_Separator"
"Zp", "Paragraph_Separator"
"Zs", "Space_Separator"
```

**Examples:**

```rexx
.RexxUnicodeServices~codepointCategory("0903"~x2d, >code, >label)=; code=; label=      --  7; 'Mc'; 'Spacing_Mark'
.RexxUnicodeServices~codepointCategory("FFFFFF"~x2d, >code, >label)=; code=; label=    --  0; 'Cn'; 'Unassigned'
.RexxUnicodeServices~codepointCategory(-1, >code, >label)=; code=; label=              --  0; 'Cn'; 'Unassigned'

```


#### 1.1.5.   codepointCharWidth

    .RexxUnicodeServices~codepointCharWidth(codepoint)

Given a codepoint, returns a character width analogous to `wcwidth(codepoint)`,
except that a width of 0 is returned for non-printable codepoints
instead of -1 as in `wcwidth`.


#### 1.1.6.   codepointCombiningClass

    .RexxUnicodeServices~codepointCombiningClass(codepoint [, [>refCode] [, >refLabel]])

Returns the `Canonical_Combining_Class` property value of `codepoint`.

The values in the `Canonical_Combining_Class` field in `UnicodeData.txt` are numeric values
used by the Canonical Ordering Algorithm. Some of those values also have symbolic labels
defined as property value aliases to make their intended use easier to understand. 

- If provided, `refCode` receives the short name.
- If provided, `refLabel` receives the long name.

The numeric values from `0` to `254` returned by `codepointCombiningClass` are standard Unicode values.  
Some values have neither a `refCode` nor a `refLabel`.

**Enumeration values**

```rexx
-- https://www.unicode.org/reports/tr44/#Canonical_Combining_Class_Values
-- first column (numeric value) from PropertyValueAliases.txt
-- second column (short name) from PropertyValueAliases.txt
-- third column (long name) from PropertyValueAliases.txt
-- fourth column (description) from https://www.unicode.org/reports/tr44/#CCC_Values_Table
/* 0 */   "NR",     "Not_Reordered"         -- Spacing and enclosing marks; also many vowel and consonant signs, even if nonspacing
/* 1 */   "OV",     "Overlay"               -- Marks which overlay a base letter or symbol
/* 2 */   "",       ""
/* 3 */   "",       ""
/* 4 */   "",       ""
/* 5 */   "",       ""
/* 6 */   "HANR",   "Han_Reading"           -- Diacritic reading marks for CJK unified ideographs
/* 7 */   "NK",     "Nukta"                 -- Diacritic nukta marks in Brahmi-derived scripts
/* 8 */   "KV",     "Kana_Voicing"          -- Hiragana/Katakana voicing marks
/* 9 */   "VR",     "Virama"                -- Viramas

/* 10 */  "CCC10",  "CCC10"                 -- Start of fixed position classes
/* 11 to 198 */
/* 199 */ "CCC199", "CCC199"                -- End of fixed position classes

/* 200 */ "ATBL",   "Attached_Below_Left"   -- Marks attached at the bottom left
/* 201 */ "",       ""
/* 202 */ "ATB",    "Attached_Below"        -- Marks attached directly below
/* 203 */ "",       ""
/* 204 */ "",       ""                      -- Marks attached at the bottom right
/* 205 */ "",       ""
/* 206 */ "",       ""
/* 207 */ "",       ""
/* 208 */ "",       ""                      -- Marks attached to the left
/* 209 */ "",       ""
/* 210 */ "",       ""                      -- Marks attached to the right
/* 211 */ "",       ""
/* 212 */ "",       ""                      -- Marks attached at the top left
/* 213 */ "",       ""
/* 214 */ "ATA",    "Attached_Above"        -- Marks attached directly above
/* 215 */ "",       ""
/* 216 */ "ATAR",   "Attached_Above_Right"  -- Marks attached at the top right
/* 217 */ "",       ""
/* 218 */ "BL",     "Below_Left"            -- Distinct marks at the bottom left
/* 219 */ "",       ""
/* 220 */ "B",      "Below"                 -- Distinct marks directly below
/* 221 */ "",       ""
/* 222 */ "BR",     "Below_Right"           -- Distinct marks at the bottom right
/* 223 */ "",       ""
/* 224 */ "L",      "Left"                  -- Distinct marks to the left
/* 225 */ "",       ""
/* 226 */ "R",      "Right"                 -- Distinct marks to the right
/* 227 */ "",       ""
/* 228 */ "AL",     "Above_Left"            -- Distinct marks at the top left
/* 229 */ "",       ""
/* 230 */ "A",      "Above"                 -- Distinct marks directly above
/* 231 */ "",       ""
/* 232 */ "AR",     "Above_Right"           -- Distinct marks at the top right
/* 233 */ "DB",     "Double_Below"          -- Distinct marks subtending two bases
/* 234 */ "DA",     "Double_Above"          -- Distinct marks extending above two bases
/* 235 */ "",       ""
/* 236 */ "",       ""
/* 237 */ "",       ""
/* 238 */ "",       ""
/* 239 */ "",       ""
/* 240 */ "IS",     "Iota_Subscript"        -- Greek iota subscript only
/* 241 */ "",       ""
/* 242 */ "",       ""
/* 243 */ "",       ""
/* 244 */ "",       ""
/* 245 */ "",       ""
/* 246 */ "",       ""
/* 247 */ "",       ""
/* 248 */ "",       ""
/* 249 */ "",       ""
/* 250 */ "",       ""
/* 251 */ "",       ""
/* 252 */ "",       ""
/* 253 */ "",       ""
/* 254 */ "",       ""
```

**Examples:**

```rexx
.RexxUnicode~codepointCombiningClass("031B"~x2d, >code, >label)=; code=; label=     --  216; 'ATAR'; 'Attached_Above_Right'
.RexxUnicode~codepointCombiningClass("FFFFFF"~x2d, >code, >label)=; code=; label=   --  0; 'NR'; 'Not_Reordered'
.RexxUnicode~codepointCombiningClass(-1, >code, >label)=; code=; label=             --  0; 'NR'; 'Not_Reordered'

```


#### 1.1.7.   codepointControlBoundary

    .RexxUnicodeServices~codepointControlBoundary(codepoint)

Returns `.true` if `codepoint` belongs to the `Zl`, `Zp`, `Cc`, or `Cf` category, except for:

        (<?> \x{E2808C} U+200C Cf Format "ZERO WIDTH NON-JOINER")
        (<?> \x{E2808D} U+200D Cf Format "ZERO WIDTH JOINER")

> [!WARNING]  
> This rule is not used internally by `utf8proc`
> and does not appear in [Unicode Standard Annex #29][unicode_standard_annex_29].
> Therefore, this method is likely to be deprecated unless a use case is identified.


#### 1.1.8.   codepointDecompositionType

    .RexxUnicodeServices~codepointDecompositionType(codepoint [, [>refCode] [, >refLabel]])

Returns the `Decomposition_Type` property value of `codepoint` as an enumeration value.

- If provided, `refCode` receives the short name or `"None"`.
- If provided, `refLabel` receives the long name or `"Code point not explicitly listed for Decomposition_Type"`.

The returned enumeration value is implementation-specific.  
For portability, use refCode instead.

**Enumeration values**

```rexx
-- https://unicode.org/reports/tr15/
-- first column: short name (from DerivedDecompositionType.txt)
-- second column: long name (from DerivedDecompositionType.txt)
"Font",         "Font variant"
"Nobreak",      "No-break version of a space or hyphen"
"Initial",      "Initial presentation form (Arabic)"
"Medial",       "Medial presentation form (Arabic)"
"Final",        "Final presentation form (Arabic)"
"Isolated",     "Isolated presentation form (Arabic)"
"Circle",       "Encircled form"
"Super",        "Superscript form"
"Sub",          "Subscript form"
"Vertical",     "Vertical layout presentation form"
"Wide",         "Wide (or zenkaku) compatibility character"
"Narrow",       "Narrow (or hankaku) compatibility character"
"Small",        "Small variant form (CNS compatibility)"
"Square",       "CJK squared font variant"
"Fraction",     "Vulgar fraction form"
"Compat",       "Unspecified compatibility character"
```

**Examples:**

```rexx
.RexxUnicode~codepointDecompositionType("0000"~x2d, >code, >label)=; code=; label=      --  0; 'None'; 'Code point not explicitly listed for Decomposition_Type'
.RexxUnicode~codepointDecompositionType("00A8"~x2d, >code, >label)=; code=; label=      --  16; 'Compat'; 'Unspecified compatibility character'
.RexxUnicode~codepointDecompositionType("00BC"~x2d, >code, >label)=; code=; label=      --  15; 'Fraction'; 'Vulgar fraction form'
.RexxUnicode~codepointDecompositionType("FFFFFF"~x2d, >code, >label)=; code=; label=    --  0; 'None'; 'Code point not explicitly listed for Decomposition_Type'
.RexxUnicode~codepointDecompositionType(-1, >code, >label)=; code=; label=              --  0; 'None'; 'Code point not explicitly listed for Decomposition_Type'

```


#### 1.1.9.   codepointEastAsianWidthIsAmbiguous

    .RexxUnicodeServices~codepointEastAsianWidthIsAmbiguous(codepoint)

Returns `.true` if the `East_Asian_Width` property value of `codepoint` is `"A" "Ambiguous"`.

[https://www.unicode.org/reports/tr11/][unicode_standard_annex_11]



#### 1.1.10.   codepointIgnorable

    .RexxUnicodeServices~codepointIgnorable(codepoint)

Returns the `Default_Ignorable_Code_Point` property value of `codepoint` (boolean value).


#### 1.1.11.   codepointIsLower

    .RexxUnicodeServices~codepointIsLower(codepoint)

Returns `.true` if the codepoint corresponds to a lower-case character
and `.false` otherwise.



#### 1.1.12.   codepointIsUpper

    .RexxUnicodeServices~codepointIsUpper(codepoint)

Returns `.true` if the codepoint corresponds to an upper-case character
and `.false` otherwise.


#### 1.1.13.   codepointToLower

    .RexxUnicodeServices~codepointToLower(codepoint)

Returns the codepoint of the corresponding lower-case character.  
If `codepoint` has no lower-case mapping, returns `codepoint`.

This method uses the simple case mappings defined in `UnicodeData.txt`
and always returns a single codepoint.


#### 1.1.14.   codepointToTitle

    .RexxUnicodeServices~codepointToTitle(codepoint)

Returns the codepoint of the corresponding title-case character.  
If `codepoint` has no title-case mapping, returns `codepoint`.

This method uses the simple case mappings defined in `UnicodeData.txt`
and always returns a single codepoint.


#### 1.1.15.   codepointToUpper

    .RexxUnicodeServices~codepointToUpper(codepoint)

Returns the codepoint of the corresponding upper-case character.  
If `codepoint` has no upper-case mapping, returns `codepoint`.

This method uses the simple case mappings defined in `UnicodeData.txt`
and always returns a single codepoint.


#### 1.1.16.   graphemeBreak

    .RexxUnicodeServices~graphemeBreak(array)

Returns `.true` if there is a grapheme break between the two consecutive codepoints passed in `array`.

**Example:**

```rexx
-- `state` is persistent state. Its initial value must be 0.
-- `graphemeBreak` updates `state`.
array = (previousCodepoint, currentCodepoint, state)
break = .RexxUnicodeServices~graphemeBreak(array) -- true or false

```

#### 1.1.17.   new

This method raises an error because `RexxUnicodeServices` has no instance.


#### 1.1.18.   systemIsLittleEndian

    .RexxUnicodeServices~systemIsLittleEndian

Returns `.true` if the system is little-endian.


#### 1.1.19.   unicodeVersion

    .RexxUnicodeServices~unicodeVersion

Returns the Unicode version supported by the `utf8proc` library.

**Example:**

```rexx
say .RexxUnicodeServices~unicodeVersion        -- 17.0.0 (for example)

```


#### 1.1.20.   utf8DecodeCodepoint

```
.RexxUnicodeServices~utf8DecodeCodepoint(string, indexB [, [>refSizeB] [, [>refErrorCode] [, >refErrorMsg]]])

.RexxUnicodeServices~utf8DecodeCodepoint(
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

```

Returns the next codepoint (an integer) at position `indexB` of `string`, or -1 in case of error.

`refSizeB` receives the size in bytes of the decoded codepoint.  
If `indexB` is outside the `string` index range, the received size is 0.  
In case of error, the received size is negative, indicating the number of bytes to skip
to follow the `U+FFFD` Substitution of Maximal Subparts.  
`refErrorCode` and `refErrorMsg` receive a detailed description of the error.

**Error codes and messages:**

```
- "CONTINUATION_ERROR_RANGE"        "Invalid continuation byte %i ('%02X'x) at byte-position %zu (codepoint > U+10FFFF)"
- "CONTINUATION_HIGH_SURROGATE"     "Invalid continuation byte %i ('%02X'x) at byte-position %zu (high surrogate)"
- "CONTINUATION_LOW_SURROGATE"      "Invalid continuation byte %i ('%02X'x) at byte-position %zu (low surrogate)"
- "CONTINUATION"                    "Invalid continuation byte %i ('%02X'x) at byte-position %zu"
- "CONTINUATION_NON_SHORTEST_FORM"  "Invalid continuation byte %i ('%02X'x) at byte-position %zu (non-shortest form)"
- "START_ERROR_RANGE"               "Invalid start byte %i ('%02X'x) (codepoint > U+10FFFF)"
- "START_NON_SHORTEST_FORM"         "Invalid start byte %i ('%02X'x) (non-shortest form)"
- "TRUNCATED"                       "Truncated, expected %i bytes"
```

**Example:**

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


#### 1.1.21.   utf8EncodeCodepoint

```
.RexxUnicodeServices~utf8EncodeCodepoint(codepoint, destination [, >refSizeB])

.RexxUnicodeServices~utf8EncodeCodepoint(
    codepoint,      -- (in)             The codepoint to encode (an integer).
    destination,    -- (in-out)         The mutable buffer to update.
    >refSizeB       -- (out, optional)  The size of the encoded byte sequence (0..4)
                    --                  The size is 0 if the codepoint is not in the range 0..10FFFF.
    )
```

Appends a codepoint's UTF-8 encoding into the specified mutable buffer.  
The surrogates 0xd800-0xdfff are encoded, however these are actually invalid in UTF-8.

Returns the mutable buffer passed with the destination argument.

If provided, `refSizeB` receives the size of the encoded byte sequence (0..4).


**Example:**

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


#### 1.1.22.   utf8procVersion

    .RexxUnicodeServices~utf8procVersion

Returns the version of the `utf8proc` library.

**Example:**

```rexx
say .RexxUnicodeServices~utf8procVersion        -- 2.11.3 (for example)

```


#### 1.1.23.   utf8Transform

```
.RexxUnicodeServices~utf8Transform(string [, casefold = .false [, lump= .false [, nlf = 0 [, normalization = 0 [, stripCC = .false [, stripIgnorable= .false [, stripMark = .false [, stripNA = .false]]]]]]]])

.RexxUnicodeServices~utf8Transform(
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
```

Returns the transformed string.

##### 1.1.23.1.   'caseFold' argument

Performs unicode case folding, to be able to do a case-insensitive
string comparison.


##### 1.1.23.2.   'lump' argument

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


##### 1.1.23.3.   'nlf' argument

[https://www.unicode.org/versions/Unicode17.0.0/core-spec/chapter-5/#G10213][newline_guidelines]

```rexx
-- Value to pass as the 'nlf' argument to utf8Transform (default: 0 no transformation).
-- These constants are declared in the RexxUnicode class.
-- If rxunicode.cls is not loaded, use the numeric value directly..
::constant NLF2LF 1
::constant NLF2LS 2
::constant NLF2PS 3
```

`NLF2LF`  
converts LF, CRLF, CR and NEL into LF.

`NLF2LS`  
Converts LF, CRLF, CR and NEL into LS.  
NLF sequences (LF, CRLF, CR, NEL) represent a line break and are converted
to the Unicode Line Separator (LS) codepoint.

`NLF2PS`  
Converts LF, CRLF, CR and NEL into PS.  
NLF sequences (LF, CRLF, CR, NEL) represent a paragraph break and are converted
to the Unicode Paragraph Separator (PS) codepoint.


##### 1.1.23.4.   'normalization' argument

```rexx
-- Value to pass as the `normalization` argument to utf8Transform (default: 0 no normalization).
-- These constants are declared in the RexxUnicode class.
-- If rxunicode.cls is not loaded, use the numeric value directly.
::constant NFC 1
::constant NFD 2
::constant NFKC 3
::constant NFKD 4
```

If `normalization` is not `0`, apply the requested normalization.


##### 1.1.23.5.   'stripCC' argument

Strips and/or converts control characters.

NLF-sequences are transformed into space, except if one of the
NLF2LS/PS/LF options is given. HorizontalTab (HT) and FormFeed (FF)
are treated as a NLF-sequence in this case.

All other control characters are simply removed.


##### 1.1.23.6.   'stripIgnorable' argument

Strips the characters whose property `Default_Ignorable_Code_Point` is true,
such as `SOFT-HYPHEN` or `ZERO-WIDTH-SPACE`.


##### 1.1.23.7.   'stripMark' argument

Strips all character markings.

This includes non-spacing, spacing and enclosing (i.e. accents) categories:

- `Mc` Spacing Mark
- `Me` Enclosing Mark
- `Mn` Nonspacing Mark

This option works only with a normalization applied.


##### 1.1.23.8.   'stripNA' argument

Strips the characters whose category is `Cn` Unassigned.


##### 1.1.23.9.   Examples of transformations

```rexx
string = "\N{<control-0007>}Le\N{IDEOGRAPHIC SPACE}\N{OGHAM SPACE MARK}\N{ZERO-WIDTH-SPACE}Père\t\N{HYPHEN}\N{SOFT-HYPHEN}\N{EN DASH}\N{EM DASH}Noël\x{EFB790}\r\n"
string = .RexxUnicode~stringUnescape(string)

.RexxUnicodeCodepointSupplier~new(string, , .RexxUnicodeCharacter)==
    /*
    an Array (shape [22], 22 items)
     1  : (<?> \x07 U+0007 Cc Control "<control-0007>")                                     <-- removable with STRIPCC:.true
     2  : ("L" \x4C U+004C Lu Uppercase_Letter "LATIN CAPITAL LETTER L")
     3  : ("e" \x65 U+0065 Ll Lowercase_Letter "LATIN SMALL LETTER E")
     4  : ("　" \x{E38080} U+3000 Zs Space_Separator "IDEOGRAPHIC SPACE")                   <-- replaceable by " " with LUMP:.true
     5  : (" " \x{E19A80} U+1680 Zs Space_Separator "OGHAM SPACE MARK")                     <-- replaceable by " " with LUMP:.true
     6  : (<?> \x{E2808B} U+200B Cf Format "ZERO WIDTH SPACE")                              <-- removable by STRIPIGNORABLE:.TRUE
     7  : ("P" \x50 U+0050 Lu Uppercase_Letter "LATIN CAPITAL LETTER P")
     8  : ("è" \x{C3A8} U+00E8 Ll Lowercase_Letter "LATIN SMALL LETTER E WITH GRAVE")
     9  : ("r" \x72 U+0072 Ll Lowercase_Letter "LATIN SMALL LETTER R")
     10 : ("e" \x65 U+0065 Ll Lowercase_Letter "LATIN SMALL LETTER E")
     11 : (<?> \x09 U+0009 Cc Control "<control-0009>")                                     <-- replaceable by " " with STRIPCC:.true
     12 : ("‐" \x{E28090} U+2010 Pd Dash_Punctuation "HYPHEN")                              <-- replaceable by "-" with LUMP:.true
     13 : (<?> \x{C2AD} U+00AD Cf Format "SOFT HYPHEN")                                     <-- removable by STRIPIGNORABLE:.true
     14 : ("–" \x{E28093} U+2013 Pd Dash_Punctuation "EN DASH")                             <-- replaceable by "-" with LUMP:.true
     15 : ("—" \x{E28094} U+2014 Pd Dash_Punctuation "EM DASH")                             <-- replaceable by "-" with LUMP:.true
     16 : ("N" \x4E U+004E Lu Uppercase_Letter "LATIN CAPITAL LETTER N")
     17 : ("o" \x6F U+006F Ll Lowercase_Letter "LATIN SMALL LETTER O")
     18 : ("ë" \x{C3AB} U+00EB Ll Lowercase_Letter "LATIN SMALL LETTER E WITH DIAERESIS")
     19 : ("l" \x6C U+006C Ll Lowercase_Letter "LATIN SMALL LETTER L")
     20 : (<?> \x{EFB790} U+FDD0 Cn Unassigned "<noncharacter-FDD0>")                       <-- removeable with STRIPNA:.true
     21 : (<?> \x0D U+000D Cc Control "<control-000D>")
     22 : (<?> \x0A U+000A Cc Control "<control-000A>")                                     <-- CR+LF replaceable by " " with STRIPCC:.true
    */

string=; .RexxUnicode~C2U(string)=
-- '[07]Le　 ​Père[09]‐­–—Noël﷐[0D0A]'
-- 'U+0007 U+004C U+0065 U+3000 U+1680 U+200B U+0050 U+00E8 U+0072 U+0065 U+0009 U+2010 U+00AD U+2013 U+2014 U+004E U+006F U+00EB U+006C U+FDD0 U+000D U+000A'

```

```rexx
-- Example 1
-- Performs unicode case folding, to be able to do a case-insensitive string comparison.
tstring = .RexxUnicodeServices~utf8Transform(string, /*casefold:*/ .true)
tstring=; .RexxUnicode~C2U(tstring)=
-- '[07]le　 ​père[09]‐­–—noël﷐[0D0A]'
-- 'U+0007 U+006C U+0065 U+3000 U+1680 U+200B U+0070 U+00E8 U+0072 U+0065 U+0009 U+2010 U+00AD U+2013 U+2014 U+006E U+006F U+00EB U+006C U+FDD0 U+000D U+000A'

```

```rexx
-- Example 2
-- Strip "default ignorable characters" such as SOFT-HYPHEN or ZERO-WIDTH-SPACE.
tstring = .RexxUnicodeServices~utf8Transform(string, /*casefold:*/, /*lump:*/, /*nlf:*/, /*normalization:*/, /*stripCC*/, /*stripIgnorable:*/.true)
tstring=; .RexxUnicode~C2U(tstring)=
-- '[07]Le　 Père[09]‐–—Noël﷐[0D0A]'
-- 'U+0007 U+004C U+0065 U+3000 U+1680 U+0050 U+00E8 U+0072 U+0065 U+0009 U+2010 U+2013 U+2014 U+004E U+006F U+00EB U+006C U+FDD0 U+000D U+000A'

```

```rexx
-- Example 3
-- Lumps certain characters together.
tstring = .RexxUnicodeServices~utf8Transform(string, /*casefold:*/, /*lump:*/ .true)
tstring=; .RexxUnicode~C2U(tstring)=
-- '[07]Le  ​Père[09]-­--Noël﷐[0D0A]'
-- 'U+0007 U+004C U+0065 U+0020 U+0020 U+200B U+0050 U+00E8 U+0072 U+0065 U+0009 U+002D U+00AD U+002D U+002D U+004E U+006F U+00EB U+006C U+FDD0 U+000D U+000A'

```

```rexx
-- Example 4
-- NLF2LF: Convert LF, CRLF, CR and NEL into LF.
tstring = .RexxUnicodeServices~utf8Transform(string, /*casefold:*/, /*lump:*/, /*nlf:*/ 1)
tstring=; .RexxUnicode~C2U(tstring)=
-- '[07]Le　 ​Père[09]‐­–—Noël﷐[0A]'
-- 'U+0007 U+004C U+0065 U+3000 U+1680 U+200B U+0050 U+00E8 U+0072 U+0065 U+0009 U+2010 U+00AD U+2013 U+2014 U+004E U+006F U+00EB U+006C U+FDD0 U+000A'

```

```rexx
-- Example 5
-- NLF2LS: Convert LF, CRLF, CR and NEL into LS (U+2028 Zl 0 "LINE SEPARATOR").
tstring = .RexxUnicodeServices~utf8Transform(string, /*casefold:*/, /*lump:*/, /*nlf:*/ 2)
tstring=; .RexxUnicode~C2U(tstring)=
-- '[07]Le　 ​Père[09]‐­–—Noël﷐'
-- 'U+0007 U+004C U+0065 U+3000 U+1680 U+200B U+0050 U+00E8 U+0072 U+0065 U+0009 U+2010 U+00AD U+2013 U+2014 U+004E U+006F U+00EB U+006C U+FDD0 U+2028'

```

```rexx
-- Example 6
-- NLF2PS: convert LF, CRLF, CR and NEL into PS (U+2029 Zp 0 "PARAGRAPH SEPARATOR").
tstring = .RexxUnicodeServices~utf8Transform(string, /*casefold:*/, /*lump:*/, /*nlf:*/ 3)
tstring=; .RexxUnicode~C2U(tstring)=
-- '[07]Le　 ​Père[09]‐­–—Noël﷐ '
-- 'U+0007 U+004C U+0065 U+3000 U+1680 U+200B U+0050 U+00E8 U+0072 U+0065 U+0009 U+2010 U+00AD U+2013 U+2014 U+004E U+006F U+00EB U+006C U+FDD0 U+2029'

```

```rexx
-- Example 7
-- Strips and/or converts control characters.
tstring = .RexxUnicodeServices~utf8Transform(string, /*casefold:*/, /*lump:*/, /*nlf:*/, /*normalization:*/, /*stripCC*/ .true)
tstring=; .RexxUnicode~C2U(tstring)=
-- 'Le　 ​Père ‐­–—Noël﷐ '
-- 'U+004C U+0065 U+3000 U+1680 U+200B U+0050 U+00E8 U+0072 U+0065 U+0020 U+2010 U+00AD U+2013 U+2014 U+004E U+006F U+00EB U+006C U+FDD0 U+0020'

```

```rexx
-- Example 8
-- Strips all character markings.
-- This includes non-spacing, spacing and enclosing (i.e. accents).
-- This option works only with normalization.
tstring = .RexxUnicodeServices~utf8Transform(string, /*casefold:*/, /*lump:*/, /*nlf:*/, /*normalization:*/ 1, /*stripCC:*/, /*stripIgnorable:*/, /*stripMark:*/ .true)
tstring=; .RexxUnicode~C2U(tstring)=
-- '[07]Le　 ​Pere[09]‐­–—Noel﷐[0D0A]'
-- 'U+0007 U+004C U+0065 U+3000 U+1680 U+200B U+0050 U+0065 U+0072 U+0065 U+0009 U+2010 U+00AD U+2013 U+2014 U+004E U+006F U+0065 U+006C U+FDD0 U+000D U+000A'

```

```rexx
-- Example 9
-- Strips unassigned codepoints.
tstring = .RexxUnicodeServices~utf8Transform(string, /*casefold:*/, /*lump:*/, /*nlf:*/, /*normalization:*/, /*stripCC:*/, /*stripIgnorable:*/, /*stripMark:*/, /*stripNA*/ .true)
tstring=; .RexxUnicode~C2U(tstring)=
-- '[07]Le　 ​Père[09]‐­–—Noël[0D0A]'
-- 'U+0007 U+004C U+0065 U+3000 U+1680 U+200B U+0050 U+00E8 U+0072 U+0065 U+0009 U+2010 U+00AD U+2013 U+2014 U+004E U+006F U+00EB U+006C U+000D U+000A'

```

```rexx
-- Example 10
-- Application of several options.
tstring = .RexxUnicodeServices~utf8Transform(string, /*casefold:*/ .true, /*lump:*/ .true, /*nlf:*/, /*normalization:*/ 1, /*stripCC:*/ .true, /*stripIgnorable:*/ .true, /*stripMark:*/ .true, /*stripNA*/ .true)
tstring=; .RexxUnicode~C2U(tstring)=
-- 'le  pere ---noel '
-- 'U+006C U+0065 U+0020 U+0020 U+0070 U+0065 U+0072 U+0065 U+0020 U+002D U+002D U+002D U+006E U+006F U+0065 U+006C U+0020'

.RexxUnicodeCodepointSupplier~new(tstring, , .RexxUnicodeCharacter)==
    /*
     1  : ("l" \x6C U+006C Ll Lowercase_Letter "LATIN SMALL LETTER L")
     2  : ("e" \x65 U+0065 Ll Lowercase_Letter "LATIN SMALL LETTER E")
     3  : (" " \x20 U+0020 Zs Space_Separator "SPACE")                    <-- LUMP (was IDEOGRAPHIC SPACE)
     4  : (" " \x20 U+0020 Zs Space_Separator "SPACE")                    <-- LUMP (was OGHAM SPACE MARK)
     5  : ("p" \x70 U+0070 Ll Lowercase_Letter "LATIN SMALL LETTER P")
     6  : ("e" \x65 U+0065 Ll Lowercase_Letter "LATIN SMALL LETTER E")
     7  : ("r" \x72 U+0072 Ll Lowercase_Letter "LATIN SMALL LETTER R")
     8  : ("e" \x65 U+0065 Ll Lowercase_Letter "LATIN SMALL LETTER E")
     9  : (" " \x20 U+0020 Zs Space_Separator "SPACE")                    <-- STRIPCC (was TAB)
     10 : ("-" \x2D U+002D Pd Dash_Punctuation "HYPHEN-MINUS")            <-- LUMP (was HYPHEN)
     11 : ("-" \x2D U+002D Pd Dash_Punctuation "HYPHEN-MINUS")            <-- LUMP (was EN DASH)
     12 : ("-" \x2D U+002D Pd Dash_Punctuation "HYPHEN-MINUS")            <-- LUMP (was EM DASH)
     13 : ("n" \x6E U+006E Ll Lowercase_Letter "LATIN SMALL LETTER N")
     14 : ("o" \x6F U+006F Ll Lowercase_Letter "LATIN SMALL LETTER O")
     15 : ("e" \x65 U+0065 Ll Lowercase_Letter "LATIN SMALL LETTER E")
     16 : ("l" \x6C U+006C Ll Lowercase_Letter "LATIN SMALL LETTER L")
     17 : (" " \x20 U+0020 Zs Space_Separator "SPACE")                    <-- STRIPCC (was CR+LF)
    */
    
```


### 1.2.   Instance methods

The `RexxUnicodeServices` class defines no instance methods of its own.


## 2.   RexxUnicode Class

The `RexxUnicode` class cannot be instantiated.  
Its services are provided through class methods.

`::requires "rxunicode.cls"`


### 2.1.   Methods inherited from the `ICU4ooRexxInterface` class

See the `ICU4ooRexxInterface` class.


### 2.2.   Methods inherited from the `RexxUnicodeServices` class

See the `RexxUnicodeServices` class.


### 2.3.   Constants

#### 2.3.1.   maxCodepoint

```rexx
::constant maxCodepoint 1114111
```


#### 2.3.2.   NFC, NFD, NFKC, NFKD

```rexx
-- Value to pass as the `normalization` argument to utf8Transform (default: 0 no normalization):
::constant NFC 1
::constant NFD 2
::constant NFKC 3
::constant NFKD 4
```


#### 2.3.3.   NLF2LF, NLF2LS, NLF2PS

```rexx
-- Value to pass as the `nlf` argument to utf8Transform (default: 0 no transformation).
-- See the documentation of utf8Transform for a description of these values.
::constant NLF2LF 1
::constant NLF2LS 2
::constant NLF2PS 3
```

### 2.4.   Class methods

#### 2.4.1.   C2U

    .RexxUnicode~C2U(string, buffer=.nil)

Returns an ASCII string containing a sequence of `U+XXXX` (4 to 6 hex digits).

If a buffer is passed as an argument, the resulting string is appended to the buffer, and the buffer is returned.

**Examples:**

```rexx
.RexxUnicode~C2U("Noël 🎅")=                        -- 'U+004E U+006F U+00EB U+006C U+0020 U+1F385'
.RexxUnicode~C2U("Noël 🎅", .MutableBuffer~new)=    -- M'U+004E U+006F U+00EB U+006C U+0020 U+1F385'

```

```rexx
-- Invalid string
-- U+FFFD Substitution of Maximal Subparts
-- https://www.unicode.org/versions/Unicode17.0.0/core-spec/chapter-3/#G68064
.RexxUnicode~C2U("F4 91 92 93 FF 41 80 BF 42"x)=    -- 'U+FFFD U+FFFD U+FFFD U+FFFD U+FFFD U+0041 U+FFFD U+FFFD U+0042'
.RexxUnicode~C2X("F4 91 92 93 FF 41 80 BF 42"x)=    -- 'F4 91 92 93 FF 41 80 BF 42'
-- https://www.unicode.org/versions/Unicode17.0.0/core-spec/chapter-3/#G68202
.RexxUnicode~C2U("E1 80 E2 F0 91 92 F1 BF 41"x)=    -- 'U+FFFD U+FFFD U+FFFD U+FFFD U+0041'
.RexxUnicode~C2X("E1 80 E2 F0 91 92 F1 BF 41"x)=    -- 'E180 E2 F09192 F1BF 41'

```


#### 2.4.2.   C2X

    .RexxUnicode~C2X(string, buffer=.nil)

Returns a string representing the encoded codepoints of `string` in hexadecimal.  
The codepoints are separated by a single space.

If a buffer is passed as an argument, the resulting string is appended to the buffer, and the buffer is returned.

**Examples:**

```rexx
.RexxUnicode~C2X("Noël 🎅")=                        -- '4E 6F C3AB 6C 20 F09F8E85'
.RexxUnicode~C2X("Noël 🎅", .MutableBuffer~new)=    -- M'4E 6F C3AB 6C 20 F09F8E85'

```


#### 2.4.3.   character

    .RexxUnicode~character(identifier)

Gets a RexxUnicodeCharacter from a codepoint identifier.  
See the `codepoint` method for a definition of "codepoint identifier".

**Examples:**

```rexx
-- Search by decimal value
.RexxUnicode~character("127877")=               -- ("🎅"  U+1F385 So Other_Symbol 2 "FATHER CHRISTMAS")

```

```rexx
-- Search by U+ notation
.RexxUnicode~character("U+1F385")=              -- ("🎅"  U+1F385 So Other_Symbol 2 "FATHER CHRISTMAS")

```

```rexx
-- Search by name
.RexxUnicode~character("FATHER CHRISTMAS")=     -- ("🎅"  U+1F385 So Other_Symbol 2 "FATHER CHRISTMAS")

```

```rexx
-- search by loose name.
-- https://unicode.org/reports/tr44/#UAX44-LM2
.RexxUnicode~character(" Father christmas  ")=  -- ("🎅" \x{F09F8E85} U+1F385 So Other_Symbol "FATHER CHRISTMAS")

```

```rexx
-- Search directly a character
.RexxUnicode~character("🎅")=                   -- ("🎅"  U+1F385 So Other_Symbol 2 "FATHER CHRISTMAS")

```

```rexx
-- Only -1 is supported as a special value
-- All other negative codepoints are invalid
.RexxUnicode~character(-1)=                     -- (An invalid character)
.RexxUnicode~character(-2)=                     -- Invalid code point -2; allowed range is 0 to 1114111 (U+10FFFF).

```


#### 2.4.4.   checkCodepoint

    .RexxUnicode~checkCodepoint(codepoint, minusOneAllowed=.false)

Returns `.true` if `codepoint` is in the range 0...RexxUnicode~maxCodepoint.  
The range `U+D800-U+DFFF` is allowed (surrogates).  
If `minusOneAllowed` is true then -1 is allowed (special value used in case of error).


#### 2.4.5.   codepoint

    .RexxUnicode~codepoint(identifier)

Gets a codepoint from a codepoint identifier.

A codepoint identifier may be:

- a decimal value (the codepoint itself),
- a `U+` notation,
- a character name,
- or a UTF-8 encoded character.

**Examples:**

```rexx
-- Search by decimal value
.RexxUnicode~codepoint("127877")=               -- 127877

```

```rexx
-- Search by U+ notation
.RexxUnicode~codepoint("U+1F385")=              -- 127877

```

```rexx
-- Search by name
.RexxUnicode~codepoint("FATHER CHRISTMAS")=     -- 127877

```

```rexx
-- search by loose name.
-- https://unicode.org/reports/tr44/#UAX44-LM2
.RexxUnicode~codepoint(" Father christmas  ")=  -- 127877

```

```rexx
-- Search directly a character
.RexxUnicode~codepoint("🎅")=                   -- 127877

```

```rexx
-- Only -1 is supported as a special value
-- All other negative codepoints are invalid
.RexxUnicode~codepoint(-1)=                     -- -1
.RexxUnicode~codepoint(-2)=                     -- Invalid code point -2; allowed range is 0 to 1114111 (U+10FFFF).

```


#### 2.4.6.   codepointCharName

    .RexxUnicode~codepointCharName(codepoint, raiseError=.false)

Returns the Unicode standard name for the given codepoint, or an empty string if not available.

This method requires the `ICU4ooRexx` class. If it is not loaded, the method returns an empty string or, if requested, raises an error.

**Examples:**

 ```rexx
.RexxUnicode~codepointCharName(2448~x2d)=       -- 'OCR DASH'

```

```rexx
-- The special value -1 has no name
.RexxUnicode~codepointCharName(-1)=             -- ''

```


#### 2.4.7.   codepointCharNameAlias

    .RexxUnicode~codepointCharNameAlias(codepoint, raiseError=.false)

Returns the Unicode name alias for the given codepoint, or an empty string if not available.

This method requires the `ICU4ooRexx` class. If it is not loaded, the method returns an empty string or, if requested, raises an error.

**Examples:**

 ```rexx
.RexxUnicode~codepointCharNameAlias(2448~x2d)=       -- 'MICR ON US SYMBOL'

```

```rexx
-- The special value -1 has no name alias
.RexxUnicode~codepointCharNameAlias(-1)=             -- ''

 ```


#### 2.4.8.   codepointExtendedCharName

    .RexxUnicode~codepointExtendedCharName(codepoint, raiseError=.false)

Returns the Unicode extended name alias for the given codepoint, or an empty string if not available.

All Unicode characters have an extended name.  
An extended name is either the standard name if defined, or a codepoint label algorithmically generated.

This method requires the `ICU4ooRexx` class. If it is not loaded, the method returns an empty string or, if requested, raises an error.

**Examples:**

```rexx
.RexxUnicode~codepointExtendedCharName(2448~x2d)=   -- 'OCR DASH'

```

```rexx
-- The special value -1 has no name alias
.RexxUnicode~codepointExtendedCharName(-1)=         -- ''

```


#### 2.4.9.   codepointIsPrintable

    .RexxUnicode~codepointIsPrintable(codepoint)

Returns `.true` if the codepoint is printable.

Rules:

- Codepoints having a display width of 0 are considered non-printable.
- `Mc`, `Me` and `Mn` codepoints are considered printable, even though they have
  a display width of 0, because some fonts provide visible glyphs for them.
- Unassigned (`Cn`) and private-use (`Co`) codepoints are considered non-printable,
  even though they have a display width of 1.
- Other codepoints are considered printable.

**Examples:**

```rexx
.RexxUnicode~codepointIsPrintable(0)=             -- 0
.RexxUnicode~codepointIsPrintable(65)=            -- 1
.RexxUnicode~codepointIsPrintable("D800"~x2d)=    -- 0

```

```rexx
-- The special value -1 is not printable
.RexxUnicode~codepointIsPrintable(-1)=            -- 0

```


#### 2.4.10.   codepointPrintableString

    .RexxUnicode~codepointPrintableString( codepoint, buffer=.nil)

Returns a printable string representation for the given codepoint.

Printable codepoints are returned as their UTF-8 character representation;
non-printable codepoints are returned as a Unicode escape notation.

If a buffer is passed as an argument, the resulting string is appended to the buffer, and the buffer is returned.

**Examples:**

```rexx
.RexxUnicode~codepointPrintableString(0)=           -- '\u0000'
.RexxUnicode~codepointPrintableString(65)=          -- 'A'
.RexxUnicode~codepointPrintableString("D800"~x2d)=  -- '\uD800'

```

```rexx
-- Since the special value -1 is not printable, it is represented using Unicode escape notation.
.RexxUnicode~codepointPrintableString(-1)=          -- '\UFFFFFFFF'

```


#### 2.4.11.   codepointUnicodeEscapeNotation

    .RexxUnicode~codepointUnicodeEscapeNotation(codepoint, buffer=.nil)

Returns the Unicode escape notation `\uXXXX` or `\UXXXXXXXX` for the given codepoint.

If a buffer is passed as an argument, the resulting string is appended to the buffer, and the buffer is returned.

**Examples:**

```rexx
.RexxUnicode~codepointUnicodeEscapeNotation("D800"~x2d)=    -- '\uD800'
.RexxUnicode~codepointUnicodeEscapeNotation("1F385"~x2d)=   -- '\U0001F385'
.RexxUnicode~codepointUnicodeEscapeNotation(-1)=            -- '\UFFFFFFFF'

```

```rexx
.RexxUnicode~codepointUnicodeEscapeNotation("D800"~x2d, .MutableBuffer~new)=    -- M'\uD800'

```


#### 2.4.12.   codepointUTF8Encoding

    .RexxUnicode~codepointUTF8Encoding(codepoint, buffer=.nil)

Returns the UTF-8 encoding for the given codepoint.

If a buffer is passed as an argument, the resulting string is appended to the buffer, and the buffer is returned.

**Examples:**

```rexx
.RexxUnicode~codepointUTF8Encoding(0)~c2x=          -- 00
.RexxUnicode~codepointUTF8Encoding(65)~c2x=         -- 41
.RexxUnicode~codepointUTF8Encoding(2448~x2d)~c2x=   -- 'E29188'

.RexxUnicode~codepointUTF8Encoding(.RexxUnicode~maxCodepoint)~c2x=      -- 'F48FBFBF'
.RexxUnicode~codepointUTF8Encoding(.RexxUnicode~maxCodepoint + 1)~c2x=  -- Invalid codepoint 1114112; allowed range is 0 to 1114111 (U+10FFFF).

```


#### 2.4.13.   D2U

    .RexxUnicode~D2U(codepoint, buffer=.nil)

Returns a `U+XXXX` string (4 to 6 hex digits) representing the codepoint passed as a decimal value.

If a buffer is passed as an argument, the resulting string is appended to the buffer, and the buffer is returned.

The special value -1 is represented using `U+FFFFFF`, even though the resulting value is not a valid Unicode scalar value

[https://www.unicode.org/versions/Unicode17.0.0/core-spec/appendix-a/#G7083][typographic_conventions_code_points]

**Examples:**

```rexx
.RexxUnicode~D2U(235)=                              -- 'U+00EB'
.RexxUnicode~D2U(235, .MutableBuffer~new)=          -- M'U+00EB'
.RexxUnicode~D2U(127877)=                           -- 'U+1F385'
.RexxUnicode~D2U(127877, .MutableBuffer~new)=       -- M'U+1F385'

.RexxUnicode~D2U(.RexxUnicode~maxCodepoint)=        -- 'U+10FFFF'
.RexxUnicode~D2U(.RexxUnicode~maxCodepoint + 1)=    -- Invalid codepoint 1114112; allowed range is 0 to 1114111 (U+10FFFF).
.RexxUnicode~D2U(-1)=                               -- 'U+FFFFFF'

```


#### 2.4.14.   h_UAX44_LM2

    .RexxUnicode~h_UAX44_LM2(name)

`h_UAX44_LM2` delegates either to `ICU4ooRexx`, if registered,
or to `.RexxUnicode~UAX44_LM2` (slower, but always available).

**Examples:**

```rexx
.RexxUnicode~h_UAX44_LM2(" MICR on US SYMBOL")=     -- 'micronussymbol'

```

```rexx
-- ASCII name only
.RexxUnicode~h_UAX44_LM2("Père Noël")=              -- Name must be an ASCII string; found "Père Noël".

```


#### 2.4.15.   info

    .RexxUnicode~info

Returns a `StringTable` containing information about the Unicode environment.

**Example output**

```rexx
.RexxUnicode~info=
    /*
    a StringTable (5 items)
    'ICU4ooRexxIsRegistered' :  1
    'maxCodepoint'           :  1114111
    'systemIsLittleEndian'   :  1
    'unicodeVersion'         : '17.0.0'
    'utf8procVersion'        : '2.11.3'
    */

```


#### 2.4.16.   stringEscape

    .RexxUnicode~stringEscape(string, buffer=.nil)

Returns a string in which non-printable codepoints and invalid byte sequences are replaced with escape sequences.

- Invalid byte sequences are represented as escaped hexadecimal byte sequences: `\XX` or `\x{XX..XX}`.
- Non-printable codepoints are represented using the standard escape sequences (`\a`, `\b`, `\t`, `\n`, `\v`, `\f`, `\r`) when applicable;  
  otherwise, Unicode escape notation (`\uXXXX` or `\UXXXXXXXX`) is used.

If a buffer is passed as an argument, the resulting string is appended to the buffer, and the buffer is returned.

**Examples:**

```rexx
.RexxUnicode~stringEscape(xrange("00"x, "FF"x))=          -- '\u0000\u0001\u0002\u0003\u0004\u0005\u0006\a\b\t\n\v\f\r\u000E\u000F\u0010\u0011\u0012\u0013\u0014\u0015\u0016\u0017\u0018\u0019\u001A\u001B\u001C\u001D\u001E\u001F !"#$%&''()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~\u007F\x80\x81\x82\x83\x84\x85\x86\x87\x88\x89\x8A\x8B\x8C\x8D\x8E\x8F\x90\x91\x92\x93\x94\x95\x96\x97\x98\x99\x9A\x9B\x9C\x9D\x9E\x9F\xA0\xA1\xA2\xA3\xA4\xA5\xA6\xA7\xA8\xA9\xAA\xAB\xAC\xAD\xAE\xAF\xB0\xB1\xB2\xB3\xB4\xB5\xB6\xB7\xB8\xB9\xBA\xBB\xBC\xBD\xBE\xBF\xC0\xC1\xC2\xC3\xC4\xC5\xC6\xC7\xC8\xC9\xCA\xCB\xCC\xCD\xCE\xCF\xD0\xD1\xD2\xD3\xD4\xD5\xD6\xD7\xD8\xD9\xDA\xDB\xDC\xDD\xDE\xDF\xE0\xE1\xE2\xE3\xE4\xE5\xE6\xE7\xE8\xE9\xEA\xEB\xEC\xED\xEE\xEF\xF0\xF1\xF2\xF3\xF4\xF5\xF6\xF7\xF8\xF9\xFA\xFB\xFC\xFD\xFE\xFF'

.RexxUnicode~stringEscape(.RexxUnicode~U2C("U+10FFF"))=   -- '\U00010FFF'

```

```rexx
.RexxUnicode~stringEscape(.RexxUnicode~stringUnescape("\N{<lead surrogate-D800>}"))=    -- '\xED\xA0\x80'
/*
A lead surrogate is invalid in a UTF-8 string (would be valid in a WTF-8 string).
(<?> \x{EDA080} U+D800 Cs Surrogate "<lead surrogate-D800>")
A codepoint supplier returns 3 codepoints for U+D800:
    1 : 'U+FFFD'
    2 : 'U+FFFD'
    3 : 'U+FFFD'
Codepoint byte sequence as hexadecimal digits (option "h):
    1 : '\xED'
    2 : '\xA0'
    3 : '\x80'
Escaping U+D800 is '\xED\xA0\x80' because \x{EDA080} can't be UTF-8 decoded as U+D800.
*/

```


#### 2.4.17.   stringInfo

    .RexxUnicode~stringInfo(string [, >indexer])

Returns a string providing information about the given string:

- Whether it is ASCII or non-ASCII
- Grapheme count
- Codepoint count
- Byte count
- Error count

If provided, `indexer` receives a `RexxUnicodeStringIndexer` instance.

**Examples:**

```rexx
.RexxUnicode~stringInfo("")=            -- '(ASCII, 0 grapheme, 0 codepoint, 0 byte, 0 error)'
.RexxUnicode~stringInfo("e")=           -- '(ASCII, 1 grapheme, 1 codepoint, 1 byte, 0 error)'
.RexxUnicode~stringInfo("é")=           -- '(not-ASCII, 1 grapheme, 1 codepoint, 2 bytes, 0 error)'
.RexxUnicode~stringInfo("€")=           -- '(not-ASCII, 1 grapheme, 1 codepoint, 3 bytes, 0 error)'
.RexxUnicode~stringInfo("🎅")=          -- '(not-ASCII, 1 grapheme, 1 codepoint, 4 bytes, 0 error)'
.RexxUnicode~stringInfo("👨‍👩‍👧")=          -- '(not-ASCII, 1 grapheme, 5 codepoints, 18 bytes, 0 error)'

```

```rexx
-- Invalid string
-- U+FFFD Substitution of Maximal Subparts
-- https://www.unicode.org/versions/Unicode17.0.0/core-spec/chapter-3/#G68202
.RexxUnicode~stringInfo("E1 80 E2 F0 91 92 F1 BF 41"x, >indexer)=    -- '(not-ASCII, 5 graphemes, 5 codepoints, 9 bytes, 4 errors)'
indexer~errors==
    /*
    an Array (shape [4], 4 items)
     1 : 'start byte-position 1 : Invalid continuation byte 128 (''80''x) at byte-position 3'
     2 : 'start byte-position 3 : Invalid continuation byte 240 (''F0''x) at byte-position 4'
     3 : 'start byte-position 4 : Invalid continuation byte 145 (''91''x) at byte-position 7'
     4 : 'start byte-position 7 : Invalid continuation byte 191 (''BF''x) at byte-position 9'
    */

```


#### 2.4.18.   stringIsASCII

    .RexxUnicode~stringIsASCII(string)

Returns `.true` if the given string contains only characters <= "7F"x

**Examples:**

```rexx
.RexxUnicode~stringIsASCII("Noel")=     -- 1
.RexxUnicode~stringIsASCII("Noël")=     -- 0

```


#### 2.4.19.   stringToNFC

    .RexxUnicode~stringToNFC(string, casefold = .false, lump= .false, nlf = 0, stripCC = .false, stripIgnorable= .false, stripMark = .false, stripNA = .false)

Returns the input string normalized to NFC, with optional transformations applied.

See the `utf8Transform` method for a description of the optional transformation parameters.

**Examples:**

```rexx
.RexxUnicode~stringToNFC("äöü äöü x̂ ϔ ﷺ baﬄe ß ς")=      -- 'äöü äöü x̂ ϔ ﷺ baﬄe ß ς'
.RexxUnicode~C2X(result)=                                  -- 'C3A4 C3B6 C3BC 20 C3A4 C3B6 C3BC 20 78 CC82 20 CF94 20 EFB7BA 20 62 61 EFAC84 65 20 C39F 20 CF82'

```


#### 2.4.20.   stringToNFD

    .RexxUnicode~stringToNFD(string, casefold = .false, lump= .false, nlf = 0, stripCC = .false, stripIgnorable= .false, stripMark = .false, stripNA = .false)

Returns the input string normalized to NFD, with optional transformations applied.

See the `utf8Transform` method for a description of the optional transformation parameters.

**Examples:**

```rexx
.RexxUnicode~stringToNFD("äöü äöü x̂ ϔ ﷺ baﬄe ß ς")=       -- 'äöü äöü x̂ ϔ ﷺ baﬄe ß ς'
.RexxUnicode~C2X(result)=                                   -- '61 CC88 6F CC88 75 CC88 20 61 CC88 6F CC88 75 CC88 20 78 CC82 20 CF92 CC88 20 EFB7BA 20 62 61 EFAC84 65 20 C39F 20 CF82'

```


#### 2.4.21.   stringToNFKC

    .RexxUnicode~stringToNFKC(string, casefold = .false, lump= .false, nlf = 0, stripCC = .false, stripIgnorable= .false, stripMark = .false, stripNA = .false)

Returns the input string normalized to NFKC, with optional transformations applied.

See the `utf8Transform` method for a description of the optional transformation parameters.

**Examples:**

```rexx
.RexxUnicode~stringToNFKC("äöü äöü x̂ ϔ ﷺ baﬄe ß ς")=      -- 'äöü äöü x̂ Ϋ صلى الله عليه وسلم baffle ß ς'
.RexxUnicode~C2X(result)=                                   -- 'C3A4 C3B6 C3BC 20 C3A4 C3B6 C3BC 20 78 CC82 20 CEAB 20 D8B5 D984 D989 20 D8A7 D984 D984 D987 20 D8B9 D984 D98A D987 20 D988 D8B3 D984 D985 20 62 61 66 66 6C 65 20 C39F 20 CF82'

```


#### 2.4.22.   stringToNFKC_CF

    .RexxUnicode~stringToNFKC_CF(string, lump= .false, nlf = 0, stripCC = .false, stripMark = .false, stripNA = .false)

Returns the input string normalized to NFKC_CF, with optional transformations applied.

NFKC preserves the original case of letters (e.g., 'A' remains 'A').  
In contrast, NFKC_CF applies Unicode case-folding rules to map all characters to a case-insensitive form.

- Example: The German sharp s ('ß') remains 'ß' in NFKC but maps to 'ss' in NFKC_CF.
- Example: The Greek final sigma ('ς') maps to the standard sigma ('σ') in NFKC_CF, whereas NFKC leaves it unchanged.

NFKC_CF is specifically optimized for identifier matching.  
As part of the "Default Case Folding" algorithm, it removes ignorable characters (such as certain formatting marks or zero-width spaces).  
NFKC retains these characters.

See the `utf8Transform` method for a description of the optional transformation parameters.

**Examples:**

```rexx
.RexxUnicode~stringToNFKC_CF("äöü äöü x̂ ϔ ﷺ baﬄe ß ς")=       -- 'äöü äöü x̂ ϋ صلى الله عليه وسلم baffle ss σ'
.RexxUnicode~C2X(result)=                                       -- 'C3A4 C3B6 C3BC 20 C3A4 C3B6 C3BC 20 78 CC82 20 CF8B 20 D8B5 D984 D989 20 D8A7 D984 D984 D987 20 D8B9 D984 D98A D987 20 D988 D8B3 D984 D985 20 62 61 66 66 6C 65 20 73 73 20 CF83'

```


#### 2.4.23.   stringToNFKD

    .RexxUnicode~stringToNFKD(string, casefold = .false, lump= .false, nlf = 0, stripCC = .false, stripIgnorable= .false, stripMark = .false, stripNA = .false)

Returns the input string normalized to NFKD, with optional transformations applied.

See the `utf8Transform` method for a description of the optional transformation parameters.

**Examples:**

```rexx
.RexxUnicode~stringToNFKD("äöü äöü x̂ ϔ ﷺ baﬄe ß ς")=      -- 'äöü äöü x̂ Ϋ صلى الله عليه وسلم baffle ß ς'
.RexxUnicode~C2X(result)=                                   -- '61 CC88 6F CC88 75 CC88 20 61 CC88 6F CC88 75 CC88 20 78 CC82 20 CEA5 CC88 20 D8B5 D984 D989 20 D8A7 D984 D984 D987 20 D8B9 D984 D98A D987 20 D988 D8B3 D984 D985 20 62 61 66 66 6C 65 20 C39F 20 CF82'

```


#### 2.4.24.   stringUnescape

    .RexxUnicode~stringUnescape(string, buffer=.nil)

Returns a string in which escape sequences are replaced with their corresponding values.

If a buffer is passed as an argument, the resulting string is appended to the buffer, and the buffer is returned.

Escape sequences are normally handled in string literals at parse time.
This method handles them at run time instead.

**Supported escape sequences**

    \\                  "\"
    \a                  "07"x   audible bell (BEL)
    \b                  "08"x   backspace (BS)
    \f                  "0C"x   form feed (FF)
    \n                  "0A"x   line feed (LF)
    \r                  "0D"x   carriage return (CR)
    \t                  "09"x   horizontal tab (HT)
    \v                  "0B"x   vertical tab (VT)

    \N{Unicode name}    Character name in the Unicode database
    \u{X..X}            Unicode scalar value, 1 to 8 hex digits
    \uXXXX              Unicode scalar value, exactly 4 hex digits
    \UXXXXXXXX          Unicode scalar value, exactly 8 hex digits
    \x{X..X}            Arbitrary number of hex digits
    \xXX                One byte, exactly 2 hex digits

**Examples**

```rexx
.RexxUnicode~stringUnescape("\\")=            -- '\'
.RexxUnicode~stringUnescape("a\\")=           -- 'a\'
.RexxUnicode~stringUnescape("\\b")=           -- '\b'
.RexxUnicode~stringUnescape("a\\b")=          -- 'a\b'

```

```rexx
.RexxUnicode~stringUnescape("\\", .MutableBuffer~new)=            -- M'\'
.RexxUnicode~stringUnescape("a\\", .MutableBuffer~new)=           -- M'a\'
.RexxUnicode~stringUnescape("\\b", .MutableBuffer~new)=           -- M'\b'
.RexxUnicode~stringUnescape("a\\b", .MutableBuffer~new)=          -- M'a\b'

```

```rexx
.RexxUnicode~stringUnescape("\a")~c2x=    -- 07
.RexxUnicode~stringUnescape("\b")~c2x=    -- 08
.RexxUnicode~stringUnescape("\f")~c2x=    -- '0C'
.RexxUnicode~stringUnescape("\n")~c2x=    -- '0A'
.RexxUnicode~stringUnescape("\r")~c2x=    -- '0D'
.RexxUnicode~stringUnescape("\t")~c2x=    -- 09
.RexxUnicode~stringUnescape("\v")~c2x=    -- '0B'

```

```rexx
.RexxUnicode~stringUnescape("\N{OCR DASH}")=                  -- '⑈'
.RexxUnicode~stringUnescape("\N{MICR ON US SYMBOL}")=         -- '⑈'
.RexxUnicode~stringUnescape("\N{<control-0000>}")=            -- '[00]'
.RexxUnicode~stringUnescape("\N{<lead surrogate-D800>}")~c2x= -- 'EDA080' (use ~c2x to not display an invalid byte sequence)

```

```rexx
.RexxUnicode~stringUnescape("\u{1D4D0}\u0042\U0001D4D2\x{F09D9393}\xF0\x9D\x93\x94")=     -- '𝓐B𝓒𝓓𝓔'

```


#### 2.4.25.   U2C

    .RexxUnicode~U2C(UPlusCodepoints, buffer=.nil)

Encodes an ASCII string of the form `"U+XXXX.. U+XXXX.. ..."` into a UTF-8 string.

The only valid separators are one or more spaces.

If a buffer is passed, the decoded characters are appended to it.  
The update is atomic: either all decoded characters are appended, or the buffer is left unchanged.  
If an error occurs, any characters appended during this call are discarded.

The result is either a string or the buffer passed as argument.

**Examples:**

```rexx
.RexxUnicode~U2C("U+004E U+006F U+00EB U+006C U+0020 U+1F385")=                         -- 'Noël 🎅'
.RexxUnicode~U2C("U+004E U+006F U+00EB U+006C U+0020 U+1F385", .MutableBuffer~new)=     -- M'Noël 🎅'

```

```rexx
.RexxUnicode~U2C("U+004E U+006F U+FFFFFF U+00EB")=                                      -- Cannot UTF-8 encode codepoint U+FFFFFF.

```

```rexx
-- The update is atomic when passing a buffer:
-- either all decoded characters are appended, or the buffer is left unchanged.
b = .MutableBuffer~new("🤶 ")
.RexxUnicode~U2C("U+004E U+006F U+FFFFFF U+00EB", b)=   -- Cannot UTF-8 encode codepoint U+FFFFFF.
b=                                                      -- M'🤶 '

```


#### 2.4.26.   U2D

    .RexxUnicode~U2D(UPlusCodepoint, start=1, raiseError=.false [, >refLength]

Converts `"U+XXXX.."` to a decimal value.

If `refLength` is provided by the caller, extra characters after the `U+XXXX..` are allowed.  
Otherwise UPlusCodepoint must be strictly `"U+"` followed by 4 to 6 hex digits.

If provided, `refLength` receives the length of `"U+XXXX.."`, allowing to skip it when parsing.

Returns the decimal value if no error occurs.  
Otherwise, returns -1 or, if requested, raises an error.

**Examples:**

```rexx
.RexxUnicode~U2D("U+00EB")=                                                                 -- 235
.RexxUnicode~U2D("U+00EB ")=                                                                -- -1
.RexxUnicode~U2D("U+00EB ", /*start*/ 1, /*raiseError*/ .true)=                             -- Expected U+ or u+ followed by 4..6 hex digits; found "U+00EB ".
.RexxUnicode~U2D("U+00EB ", /*start*/ 1, /*raiseError*/ .true, >length)=; length=           -- 235; 6
.RexxUnicode~U2D("U+0020 U+1F385 ", /*start*/ 7, /*raiseError*/ .false)=                    -- -1
.RexxUnicode~U2D("U+0020 U+1F385 ", /*start*/ 7, /*raiseError*/ .true, >length)=; length=   -- Expected U+ or u+ followed by 4..6 hex digits; found " U+1F385 ".
.RexxUnicode~U2D("U+0020 U+1F385 ", /*start*/ 8, /*raiseError*/ .true, >length)=; length=   -- 127877; 7

```


#### 2.4.27.   UAX44_LM2

    .RexxUnicode~UAX44_LM2(string)

Returns a string ready for comparison using the [loose matching rule UAX44-LM2][uax44_lm2] where
case, whitespace, underscore ('_'), and all medial hyphens except the hyphen in `U+1180` HANGUL JUNGSEONG O-E
are removed.

**Examples:**

```rexx
.RexxUnicode~UAX44_LM2(" MICR on US SYMBOL")=       -- 'micronussymbol'

```

```rexx
-- ASCII name only
.RexxUnicode~UAX44_LM2("Père Noël")=                -- Name must be an ASCII string; found "Père Noël".

```


#### 2.4.28.   UAX44_LM3

    .RexxUnicode~UAX44_LM3(string)

Returns a string ready for comparison using the [loose matching rule UAX44-LM3][uax44_lm3] where
case, whitespace, underscore ('_'), hyphens, and any initial prefix string "is"
are removed.

**Examples:**

```rexx
.RexxUnicode~UAX44_LM3("Upper case")=               -- 'uppercase'
.RexxUnicode~UAX44_LM3(" Is Upper case")=           -- 'uppercase'
.RexxUnicode~UAX44_LM3("Is Pere Noel")=             -- 'perenoel'

```

```rexx
-- ASCII name only
.RexxUnicode~UAX44_LM3("Is Père Noël")=             -- Name must be an ASCII string; found "Is Père Noël".

```


### 2.5.   Instance methods

The `RexxUnicode` class defines no instance methods of its own.


## 3.   RexxUnicodeCodepointSupplier Class

A `RexxUnicodeCodepointSupplier` instance is an iterator created from a 
UTF-8 string and allows enumeration of the string's codepoints without indexer.

`::requires "rxunicode.cls"`

**Examples:**

```rexx
-- Example 1
-- the default index type is the codepoint index
-- the default item type is the codepoint as an integer
.RexxUnicodeCodepointSupplier~new("noël👨‍👩‍👧🎅")==
    /*
    a RexxUnicodeCodepointSupplier 
     1  :  110
     2  :  111
     3  :  235
     4  :  108
     5  :  128104
     6  :  8205
     7  :  128105
     8  :  8205
     9  :  128103
     10 :  127877
    */

```

```rexx
-- Example 2
-- It's possible to get other types of index and item
-- Here, we request the byte index and the codepoint byte sequence as hex digits
.RexxUnicodeCodepointSupplier~new("noël👨‍👩‍👧🎅", "b", "h")==
    /*
    a RexxUnicodeCodepointSupplier 
     1  : '\x6E'
     2  : '\x6F'
     3  : '\x{C3AB}'
     5  : '\x6C'
     6  : '\x{F09F91A8}'
     10 : '\x{E2808D}'
     13 : '\x{F09F91A9}'
     17 : '\x{E2808D}'
     20 : '\x{F09F91A7}'
     24 : '\x{F09F8E85}'
    */

```

```rexx
-- Example 3
-- Codepoint supplier providing the default index and the codepoint escaped string
.RexxUnicodeCodepointSupplier~new("noël👨‍👩‍👧🎅", , "\")==
    /*
    a RexxUnicodeCodepointSupplier 
     1  : 'n'
     2  : 'o'
     3  : 'ë'
     4  : 'l'
     5  : '👨'
     6  : '\u200D'
     7  : '👩'
     8  : '\u200D'
     9  : '👧'
     10 : '🎅'
    */

```

```rexx
-- Example 4
-- Codepoint supplier providing the default index and the codepoint in U+ notation
.RexxUnicodeCodepointSupplier~new("noël👨‍👩‍👧🎅", , "u")==
    /*
    a RexxUnicodeCodepointSupplier 
     1  : 'U+006E'
     2  : 'U+006F'
     3  : 'U+00EB'
     4  : 'U+006C'
     5  : 'U+1F468'
     6  : 'U+200D'
     7  : 'U+1F469'
     8  : 'U+200D'
     9  : 'U+1F467'
     10 : 'U+1F385'
    */

```

```rexx
-- Example 5
-- Codepoint supplier providing the default index and the codepoint as a RexxUnicodeCharacter
.RexxUnicodeCodepointSupplier~new("noël👨‍👩‍👧🎅", , .RexxUnicodeCharacter)==
    /*
    a RexxUnicodeCodepointSupplier 
     1  : ("n" \x6E U+006E Ll Lowercase_Letter "LATIN SMALL LETTER N")
     2  : ("o" \x6F U+006F Ll Lowercase_Letter "LATIN SMALL LETTER O")
     3  : ("ë" \x{C3AB} U+00EB Ll Lowercase_Letter "LATIN SMALL LETTER E WITH DIAERESIS")
     4  : ("l" \x6C U+006C Ll Lowercase_Letter "LATIN SMALL LETTER L")
     5  : ("👨" \x{F09F91A8} U+1F468 So Other_Symbol "MAN")
     6  : (<?> \x{E2808D} U+200D Cf Format "ZERO WIDTH JOINER")
     7  : ("👩" \x{F09F91A9} U+1F469 So Other_Symbol "WOMAN")
     8  : (<?> \x{E2808D} U+200D Cf Format "ZERO WIDTH JOINER")
     9  : ("👧" \x{F09F91A7} U+1F467 So Other_Symbol "GIRL")
     10 : ("🎅" \x{F09F8E85} U+1F385 So Other_Symbol "FATHER CHRISTMAS")
    */

```

```rexx
-- Example 6
-- Invalid string - U+FFFD Substitution of Maximal Subparts
-- https://www.unicode.org/versions/Unicode17.0.0/core-spec/chapter-3/#G68202

-- Codepoint supplier providing the codepoint index and the codepoint as an integer
.RexxUnicodeCodepointSupplier~new("E1 80 E2 F0 91 92 F1 BF 41"x)==
    /*
    a RexxUnicodeCodepointSupplier
     1 :  65533
     2 :  65533
     3 :  65533
     4 :  65533
     5 :  65
    */

```

```rexx
-- Example 7
-- Codepoint supplier providing the byte index and the codepoint escaped string
-- A negative byte index indicates an invalid byte sequence
.RexxUnicodeCodepointSupplier~new("E1 80 E2 F0 91 92 F1 BF 41"x, "b", "\")==
    /*
    a RexxUnicodeCodepointSupplier 
    -1 : '\x{E180}'
    -3 : '\xE2'
    -4 : '\x{F09192}'
    -7 : '\x{F1BF}'
     9 : 'A'
    */

```

```rexx
-- Example 8
-- Codepoint supplier providing the byte index and the error message
.RexxUnicodeCodepointSupplier~new("E1 80 E2 F0 91 92 F1 BF 41"x, "b", "e")==
    /*
    a RexxUnicodeCodepointSupplier 
    -1 : 'start byte-position 1 : Invalid continuation byte 128 (''80''x) at byte-position 3'
    -3 : 'start byte-position 3 : Invalid continuation byte 240 (''F0''x) at byte-position 4'
    -4 : 'start byte-position 4 : Invalid continuation byte 145 (''91''x) at byte-position 7'
    -7 : 'start byte-position 7 : Invalid continuation byte 191 (''BF''x) at byte-position 9'
     9 : ''
    */

```

### 3.1.   Class methods

The `RexxUnicodeCodepointSupplier` class defines no class methods of its own.


### 3.2.   Instance methods

#### 3.2.1.   available

    .RexxUnicodeCodepointSupplier~available

Returns `.true` if a codepoint is available from the supplier (that is, if the `item` method would return a value).  
Returns `.false` if the supplier has already enumerated all codepoints.


#### 3.2.2.   codepointAtIndexC

    .RexxUnicodeCodepointSupplier~codepointAtIndexC(indexC)

Convenience method.  
Advances the supplier to codepoint index `indexC` and returns the corresponding codepoint as an integer.  
If `indexC` is less than the current codepoint index, the supplier raises an error (can only advance).  
This method does not support negative indexes (indexes from the end of the string).


#### 3.2.3.   index

    .RexxUnicodeCodepointSupplier~index(type=defaultIndexType)

Returns the index of the current codepoint in the string.  
If no codepoint is available, that is, if `available` would return `.false`, the supplier raises an error.

The following index types are supported:

- If `type` is `"b"`, the byte index is returned (it is negative if the byte sequence is invalid).
- If `type`is `"c"`, the codepoint index is returned (the default index type).

The default index type is specified when the supplier is created.


#### 3.2.4.   init

    .RexxUnicodeCodepointSupplier~init(string, defaultIndexType="c", defaultItemType="c")

Initializes a `RexxUnicodeCodepointSupplier` instance for the UTF-8 string `string`.

If specified, `defaultIndexType` defines the default index type returned by the `index` method.  
The default is `"c"` (codepoint index).  
See the `index` method for the list of possible values.

If specified, `defaultItemType` defines the default item type returned by the `item` method.  
The default value is `"c"` (codepoint as an integer).  
See the `item` method for the list of possible values.


#### 3.2.5.   item

    .RexxUnicodeCodepointSupplier~item(type=defaultItemType)

Returns the current codepoint in the string.  
If no codepoint is available, that is, if `available` would return `.false`, the supplier raises an error.

The following item types are supported:

- If `type` is `"b"`, the codepoint byte sequence is returned as a string.
- If `type` is `"c"`, the codepoint is returned as an integer (default).
- If `type` is `"e"`, the error message is returned (an empty string if there is no error).
- If `type` is `"h"`, the codepoint byte sequence is returned as hexadecimal digits.
- If `type` is `"u"`, the codepoint is returned in `U+` notation.
- If `type` is `"\"`, the codepoint byte sequence is returned as an escaped string.
- If `type` is a class, a new instance of that class is returned, initialized with the result of `item("c")`.

The default item type is specified when the supplier is created.


#### 3.2.6.   next

    .RexxUnicodeCodepointSupplier~next

Moves to the next codepoint in the string.  
By repeatedly sending `next` to the supplier (as long as `available` returns `.true`), you can enumerate all codepoints in the string.  
If no codepoint is available, that is, if `available` would return `.false`, the supplier raises an error.


## 4.   RexxUnicodeGraphemeSupplier Class

A `RexxUnicodeGraphemeSupplier` instance is an iterator created from a 
UTF-8 string and allows enumeration of the string's graphemes without indexer.

`::requires "rxunicode.cls"`

**Examples:**

```rexx
-- Example 1
-- the default index type is the grapheme index
-- the default item type is the grapheme as a string
.RexxUnicodeGraphemeSupplier~new("noël👨‍👩‍👧🎅")==
    /*
    a RexxUnicodeGraphemeSupplier 
     1 : 'n'
     2 : 'o'
     3 : 'ë'
     4 : 'l'
     5 : '👨‍👩‍👧'
     6 : '🎅'
    */

```

```rexx
-- Example 2
-- Grapheme supplier providing the byte index and the grapheme byte sequence as hex digits
.RexxUnicodeGraphemeSupplier~new("noël👨‍👩‍👧🎅", "b", "h")==
    /*
    a RexxUnicodeGraphemeSupplier 
     1  : '\x6E'
     2  : '\x6F'
     3  : '\x{C3AB}'
     5  : '\x6C'
     6  : '\x{F09F91A8 E2808D F09F91A9 E2808D F09F91A7}'
     24 : '\x{F09F8E85}'
    */

```

```rexx
-- Example 3
-- Grapheme supplier providing the default index and the grapheme escaped string
.RexxUnicodeGraphemeSupplier~new("noël👨‍👩‍👧🎅", , "\")==
    /*
    a RexxUnicodeGraphemeSupplier 
     1 : 'n'
     2 : 'o'
     3 : 'ë'
     4 : 'l'
     5 : '👨\u200D👩\u200D👧'
     6 : '🎅'
    */

```

```rexx
-- Example 4
-- Grapheme supplier providing the default index and the grapheme codepoints in U+ notation
.RexxUnicodeGraphemeSupplier~new("noël👨‍👩‍👧🎅", , "u")==
    /*
    a RexxUnicodeGraphemeSupplier 
     1 : 'U+006E'
     2 : 'U+006F'
     3 : 'U+00EB'
     4 : 'U+006C'
     5 : 'U+1F468 U+200D U+1F469 U+200D U+1F467'
     6 : 'U+1F385'
    */

```

```rexx
-- Example 5
-- Invalid string - U+FFFD Substitution of Maximal Subparts
-- https://www.unicode.org/versions/Unicode17.0.0/core-spec/chapter-3/#G68202

-- Grapheme supplier providing the grapheme index and the grapheme escaped string
.RexxUnicodeGraphemeSupplier~new("E1 80 E2 F0 91 92 F1 BF 41"x, , "\")==
    /*
    a RexxUnicodeGraphemeSupplier
     1 : '\x{E180}'
     2 : '\xE2'
     3 : '\x{F09192}'
     4 : '\x{F1BF}'
     5 : 'A'
    */

```

```rexx
-- Example 6
-- Grapheme supplier providing the byte index and the grapheme escaped string
-- A negative byte index indicates an invalid byte sequence
.RexxUnicodeGraphemeSupplier~new("E1 80 E2 F0 91 92 F1 BF 41"x, "b", "\")==
    /*
    a RexxUnicodeGraphemeSupplier 
    -1 : '\x{E180}'
    -3 : '\xE2'
    -4 : '\x{F09192}'
    -7 : '\x{F1BF}'
     9 : 'A'
    */

```

```rexx
-- Example 7
-- Grapheme supplier providing the byte index and the error message
.RexxUnicodeGraphemeSupplier~new("E1 80 E2 F0 91 92 F1 BF 41"x, "b", "e")==
    /*
    a RexxUnicodeGraphemeSupplier 
    -1 : 'start byte-position 1 : Invalid continuation byte 128 (''80''x) at byte-position 3'
    -3 : 'start byte-position 3 : Invalid continuation byte 240 (''F0''x) at byte-position 4'
    -4 : 'start byte-position 4 : Invalid continuation byte 145 (''91''x) at byte-position 7'
    -7 : 'start byte-position 7 : Invalid continuation byte 191 (''BF''x) at byte-position 9'
     9 : ''
    */

```

### 4.1.   Class methods

The `RexxUnicodeGraphemeSupplier` class defines no class methods of its own.


### 4.2.   Instance methods

#### 4.2.1.   available

    .RexxUnicodeGraphemeSupplier~available

Returns `.true` if a grapheme is available from the supplier (that is, if the `item` method would return a value).  
Returns `.false` if the supplier has already enumerated all graphemes.


#### 4.2.2.   graphemeAtIndexG

    .RexxUnicodeCodepointSupplier~graphemeAtIndexG(indexG)

Convenience method.  
Advances the supplier to grapheme index `indexG` and returns the corresponding grapheme as a string.  
If `indexG` is less than the current grapheme index, the supplier raises an error (can only advance).  
This method does not support negative indexes (indexes from the end of the string).


#### 4.2.3.   index

    .RexxUnicodeGraphemeSupplier~index(type=defaultIndexType)

Returns the index of the current grapheme in the string.  
If no grapheme is available, that is, if `available` would return `.false`, the supplier raises an error.

The following index types are supported:

- If `type` is `"b"`, the byte index is returned (it is negative if the byte sequence is invalid).
- If `type`is `"g"`, the grapheme index is returned (the default index type).

The default index type is specified when the supplier is created.

#### 4.2.4.   init

    .RexxUnicodeGraphemeSupplier~init(string, defaultIndexType="g", defaultItemType="g")

Initializes a `RexxUnicodeGraphemeSupplier` instance for the UTF-8 string `string`.

If specified, `defaultIndexType` defines the default index type returned by the `index` method.  
The default is `"g"` (grapheme index).  
See the `index` method for the list of possible values.

If specified, `defaultItemType` defines the default item type returned by the `item` method.  
The default value is `"g"` (grapheme as a string).  
See the `item` method for the list of possible values.


#### 4.2.5.   item

    .RexxUnicodeGraphemeSupplier~item(type=defaultItemType)

Returns the current grapheme in the string.  
If no grapheme is available, that is, if `available` would return `.false`, the supplier raises an error.

The following item types are supported:

- If `type` is `"b"`, the grapheme byte sequence is returned as a string (same as `"g"`).
- If `type` is `"e"`, the error message is returned (an empty string if there is no error).
- If `type` is `"g"`, the grapheme is returned as a string (default).
- If `type` is `"h"`, the grapheme byte sequence is returned as hexadecimal digits.
- If `type` is `"u"`, the grapheme is returned in `U+` notation.
- If `type` is `"\"`, the grapheme byte sequence is returned as an escaped string.
- If `type` is a class, a new instance of that class is returned, initialized with the result of `item("g")`.

The default item type is specified when the supplier is created.


#### 4.2.6.   next

    .RexxUnicodeGraphemeSupplier~next

Moves to the next grapheme in the string.  
By repeatedly sending `next` to the supplier (as long as `available` returns `.true`), you can enumerate all graphemes in the string.  
If no grapheme is available, that is, if `available` would return `.false`, the supplier raises an error.


## 5.   RexxUnicodeStringIndexer Class

A `RexxUnicodeStringIndexer` instance provides direct access to codepoints and graphemes in a string.

This is a temporary implementation that will be replaced by a native indexer.

`::requires "rxunicode.cls"`

**Examples:**

```rexx
-- Example 1
indexer = .RexxUnicodeStringIndexer~new("a👨‍👩‍👧b")
indexer=                            -- (a👨‍👩‍👧b)
indexer~codepointIndexes=           -- [ 1, 2, 6, 9, 13, 16, 20]
indexer~graphemeIndexes=            -- [ 1, 2, 20]
indexer~errors=                     -- an Array (no shape, 0 items)
indexer~codepointAtIndexC(3)=       -- 8205
.RexxUnicodeCharacter~new(8205)=    -- (<?> \x{E2808D} U+200D Cf Format "ZERO WIDTH JOINER")
indexer~graphemeAtIndexG(3)=        -- 'b'

```

```rexx
-- Example 2
-- U+FFFD Substitution of Maximal Subparts
-- https://www.unicode.org/versions/Unicode17.0.0/core-spec/chapter-3/#G68064
indexer = .RexxUnicodeStringIndexer~new("F4 91 92 93 FF 41 80 BF 42"x)
indexer~codepointIndexes=           -- [-1,-2,-3,-4,-5, 6,-7,-8, 9]
indexer~graphemeIndexes=            -- [-1,-2,-3,-4,-5, 6,-7,-8, 9]
indexer~errors==
    /*
    an Array (shape [7], 7 items)
     1 : 'start byte-position 1 : Invalid continuation byte 145 (''91''x) at byte-position 2 (code point > U+10FFFF)'
     2 : 'start byte-position 2 : Invalid start byte 145 (''91''x) (non-shortest form)'
     3 : 'start byte-position 3 : Invalid start byte 146 (''92''x) (non-shortest form)'
     4 : 'start byte-position 4 : Invalid start byte 147 (''93''x) (non-shortest form)'
     5 : 'start byte-position 5 : Invalid start byte 255 (''FF''x) (code point > U+10FFFF)'
     6 : 'start byte-position 7 : Invalid start byte 128 (''80''x) (non-shortest form)'
     7 : 'start byte-position 8 : Invalid start byte 191 (''BF''x) (non-shortest form)'
    */
indexer~codepointAtIndexC(3)=       -- 65533
.RexxUnicodeCharacter~new(65533)=   -- ("�" \x{EFBFBD} U+FFFD So Other_Symbol "REPLACEMENT CHARACTER")
indexer~graphemeAtIndexG(3)~c2x=    -- 92 (use ~c2x to not display an invalid byte sequence)

```


### 5.1.   Class methods

The `RexxUnicodeStringIndexer` class defines no class methods of its own.


### 5.2.   Instance methods

#### 5.2.1.   CODEPOINTATINDEXC


#### 5.2.2.   codepointCount

    .RexxUnicodeStringIndexer~codepointCount

Returns the codepoint count calculated during the full scan performed when the indexer was initialized.  
This result is not impacted by `codepointStorageLimit`.


#### 5.2.3.   CODEPOINTINDEXB


#### 5.2.4.   CODEPOINTINDEXES


#### 5.2.5.   codepointStorageLimit


#### 5.2.6.   ENDCODEPOINTINDEXES


#### 5.2.7.   ENDCODEPOINTSTORAGESIZE


#### 5.2.8.   ENDERRORS


#### 5.2.9.   ENDERRORSTORAGESIZE


#### 5.2.10.   ENDGRAPHEMEINDEXES


#### 5.2.11.   ENDGRAPHEMESTORAGESIZE


#### 5.2.12.   errorCount

    .RexxUnicodeStringIndexer~errorCount

Returns the error count calculated during the full scan performed when the indexer was initialized.  
This result is not impacted by `errorStorageLimit`.


#### 5.2.13.   ERRORS


#### 5.2.14.   errorStorageLimit


#### 5.2.15.   GRAPHEMEATINDEXG


#### 5.2.16.   graphemeCount

    .RexxUnicodeStringIndexer~graphemeCount

Returns the grapheme count calculated during the full scan performed when the indexer was initialized.  
This result is not affected by `graphemeStorageLimit`.

#### 5.2.17.   GRAPHEMEINDEXB


#### 5.2.18.   GRAPHEMEINDEXES


#### 5.2.19.   graphemeStorageLimit


#### 5.2.20.   init

```
.RexxUnicodeStringIndexer~init(
    string,

    -- Storage limits
    codepointStorageLimit = (.RexxInfo~internalMaxNumber),
    graphemeStorageLimit = (.RexxInfo~internalMaxNumber),
    errorStorageLimit = (.RexxInfo~internalMaxNumber),
    
    -- End storage sizes
    endCodepointStorageSize = 0,
    endGraphemeStorageSize = 0,
    endErrorStorageSize = 0
    )
```

Initializes a `RexxUnicodeStringIndexer` instance for the UTF-8 string `string`.

Storage limits can be specified when creating the indexer; by default, no limit is applied:

- `codepointStorageLimit`
- `graphemeStorageLimit`
- `errorStorageLimit`

End storage sizes can be specified when creating the indexer; by default, no end storage is allocated (0).

- `endCodepointStorageSize`
- `endGraphemeStorageSize`
- `endErrorStorageSize`

The storage limits and end storage sizes allow fine-tuning of memory usage.

There is nothing wrong with specifying an end storage size without specifying the corresponding storage limit, but doing so creates a redundant storage allocation.

**Examples**

```rexx
-- Example 1
-- If the identifier is a valid UTF-8 string and is made of just one codepoint then return this codepoint.
indexer = .RexxUnicodeStringIndexer~new(identifier, /*codepointStorageLimit:*/ 1, /*graphemeStorageLimit:*/ 0, /*errorStorageLimit:*/ 0)
if indexer~errorCount == 0, indexer~codepointCount == 1 then return indexer~codepointAtIndexC(1)

```

```rexx
-- Example 2
-- U+FFFD Substitution of Maximal Subparts
-- https://www.unicode.org/versions/Unicode17.0.0/core-spec/chapter-3/#G68064
indexer = .RexxUnicodeStringIndexer~new("F4 91 92 93 FF 41 80 BF 42"x, -
                                        /*codepointStorageLimit:*/ 4, -
                                        /*graphemeStorageLimit:*/ 2, -
                                        /*errorStorageLimit:*/ 1)

-- The specified codepoint storage limit is 4
indexer~codepointCount=             -- 9
indexer~codepointStorageLimit=      -- 4
indexer~codepointIndexes=           -- [-1,-2,-3,-4]
indexer~codepointAtIndexC(3)=       -- 65533
indexer~codepointAtIndexC(5)=       -- Code point index 5 is not stored.

-- The specified grapheme storage limit is 2
indexer~graphemeCount=              -- 9
indexer~graphemeStorageLimit=       -- 2
indexer~graphemeIndexes=            -- [-1,-2]
indexer~graphemeAtIndexG(2)~c2x=    -- 91 (use ~c2x to not display an invalid byte sequence)
indexer~graphemeAtIndexG(3)~c2x=    -- Grapheme index 3 is not stored.

-- The specified error storage limit is 1
indexer~errorCount=                 -- 7
indexer~errorStorageLimit=          -- 1
indexer~errors==
    /*
    an Array (shape [1], 1 items)
     1 : 'start byte-position 1 : Invalid continuation byte 145 (''91''x) at byte-position 2 (code point > U+10FFFF)'
    */

```

```rexx
-- Example 3
-- Collect counters, no need to store indexes and errors
string = "F4 91 92 93 FF 41 80 BF 42"x || "Joyeux Noël 👨‍👩‍👧"
indexer = .RexxUnicodeStringIndexer~new(string, 0, 0, 0)
indexer~graphemeCount=      -- 22
indexer~codepointCount=     -- 26
indexer~errorCount=         -- 7

```

```rexx
-- Example 4
-- Gets the five last grapheme indexes.
-- In case of error, reports only the first error.
string = "F4 91 92 93 FF 41 80 BF 42"x || "Joyeux Noël 👨‍👩‍👧"
indexer = .RexxUnicodeStringIndexer~new(string, /*codepointStorageLimit:*/ 0, /*graphemeStorageLimit:*/ 0, /*errorStorageLimit:*/ 1, /*endCodepointStorageSize:*/ 0, /*endGraphemeStorageSize:*/ 5)
indexer~errorCount=         -- 7
indexer~errors~items=       -- 1
indexer~errors[1]=          -- 'start byte-position 1 : Invalid continuation byte 145 (''91''x) at byte-position 2 (code point > U+10FFFF)'
indexer~endGraphemeIndexes= -- [ 23, 22, 21, 19, 18]
-- Pass -endIndexG, where a negative index means "index from the end"
do endIndexG=1 to indexer~endGraphemeIndexes~items; say endIndexG":" indexer~graphemeAtIndexG(-endIndexG); end
    /*
    1: 👨‍👩‍👧
    2:  
    3: l
    4: ë
    5: o
    */
    
```


#### 5.2.21.   STRING


## 6.   RexxUnicodeCharacter Class

A RexxUnicodeCharacter instance is created from a codepoint and provides access 
to the properties of the corresponding Unicode character.

The special value -1 can be represented using a RexxUnicodeCharacter.

`::requires "rxunicode.cls"`


### 6.1.   Class methods

#### 6.1.1.   PROPERTIES


### 6.2.   Instance methods

#### 6.2.1.   BIDICLASS


#### 6.2.2.   BIDICLASSNAME


#### 6.2.3.   BIDIMIRRORED


#### 6.2.4.   BOUNDCLASS


#### 6.2.5.   BOUNDCLASSNAME


#### 6.2.6.   CATEGORY


#### 6.2.7.   CATEGORYNAME


#### 6.2.8.   CHARWIDTH


#### 6.2.9.   CODEPOINT


#### 6.2.10.   COMBININGCLASS


#### 6.2.11.   COMBININGCLASSNAME


#### 6.2.12.   COMPARETO


#### 6.2.13.   CONTROLBOUNDARY


#### 6.2.14.   DECOMPOSITIONTYPE


#### 6.2.15.   DECOMPOSITIONTYPENAME


#### 6.2.16.   EXTENDEDNAME


#### 6.2.17.   IGNORABLE


#### 6.2.18.   INFO


#### 6.2.19.   INIT


#### 6.2.20.   ISLOWER


#### 6.2.21.   ISUPPER


#### 6.2.22.   NAME


#### 6.2.23.   NAMEALIAS


#### 6.2.24.   STRING


#### 6.2.25.   TOLOWER


#### 6.2.26.   TOLOWERFULL


#### 6.2.27.   TOTITLE


#### 6.2.28.   TOTITLEFULL


#### 6.2.29.   TOUPPER


#### 6.2.30.   TOUPPERFULL


#### 6.2.31.   UNAME


#### 6.2.32.   UTF8

## 7.   ICU4ooRexxInterface Class

`::requires "rxunicode.cls"`

### 7.1.   Class methods

#### 7.1.1.   ACTIVATE


#### 7.1.2.   ASSERTICU4OOREXXISREGISTERED


#### 7.1.3.   ICU4OOREXXISREGISTERED


#### 7.1.4.   ICU4OOREXXPACKAGE


#### 7.1.5.   REGISTERICU4OOREXX


### 7.2.   Instance methods



[examples_internal_errors]: https://github.com/jlfaucher/executor5-bulk/blob/82f9531bb7b6d4be62f33b201ba62e53a74be640/main/trunk/extensions/unicode/rxunicode.cls#L1725-L1769 "Examples of internal errors"
[newline_guidelines]: https://www.unicode.org/versions/Unicode17.0.0/core-spec/chapter-5/#G10213 "Newline Guidelines"
[typographic_conventions_code_points]: https://www.unicode.org/versions/Unicode17.0.0/core-spec/appendix-a/#G7083 "Typographic Conventions - Code Points"
[unicode_standard_annex_9_mirroring]: https://unicode.org/reports/tr9/#Mirroring "Standard Annex #9 Unicode Bidirectional Algorithm - Mirroring"
[unicode_standard_annex_11]: https://www.unicode.org/reports/tr11/ "Standard Annex #11 East Asian Width"
[unicode_standard_annex_29]: https://www.unicode.org/reports/tr29/ "Standard Annex #29 Unicode Text Segmentation"
[uax44_lm2]: https://unicode.org/reports/tr44/#UAX44-LM2 "Loose matching rule UAX44-LM2"
[uax44_lm3]: https://unicode.org/reports/tr44/#UAX44-LM3 "Loose matching rule UAX44-LM3"
[utf8proc]: https://juliastrings.github.io/utf8proc/ "utf8proc"
