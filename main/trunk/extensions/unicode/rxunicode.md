<p align="right" style="font-size:10px;">
<a style="font-size:10px;" href="https://github.com/jlfaucher/executor5-bulk/blob/main/main/trunk/extensions/unicode/rxunicode.md">Markdown</a><br/>
<a style="font-size:10px;" href="https://html-preview.github.io/?url=https://github.com/jlfaucher/executor5-bulk/blob/main/main/trunk/extensions/unicode/rxunicode.html">HTML</a>
</p>

# rxunicode Reference Documentation

The [`RexxUnicodeServices`](#RexxUnicodeServices) class is a native class that exposes the Unicode services
supported by `ooRexx`. Its current implementation is based on the [`utf8proc`][utf8proc]
library embedded in `ooRexx`.


`rxunicode.cls` is a package that defines:

- The [`RexxUnicode`](#RexxUnicode) class, a subclass of `RexxUnicodeServices`.
- The [`RexxUnicodeCodepointSupplier`](#RexxUnicodeCodepointSupplier) class.
- The [`RexxUnicodeGraphemeSupplier`](#RexxUnicodeGraphemeSupplier) class.
- The [`RexxUnicodeReverseCodepointSupplier`](#RexxUnicodeReverseCodepointSupplier) class.
- The [`RexxUnicodeReverseGraphemeSupplier`](#RexxUnicodeReverseGraphemeSupplier) class.
- The [`RexxUnicodeStringIndexer`](#RexxUnicodeStringIndexer) class.
- The [`RexxUnicodeCharacter`](#RexxUnicodeCharacter) class.
- The [`ICU4ooRexxInterface`](#ICU4ooRexxInterface) class.

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

**Note on feedback:** [As of 2026][feedback_2026]...



<!----------------------------------------------------------------------------->

<a id="RexxUnicodeServices"></a>

## 1.   RexxUnicodeServices Class

<!----------------------------------------------------------------------------->

The `RexxUnicodeServices` class cannot be instantiated.  
Its services are provided through class methods.

This native class is always available; there is no need to load `rxunicode.cls`.

In the examples, you can replace `RexxUnicodeServices` by `RexxUnicode` if you loaded `rxunicode.cls`.


<a id="Class_methods"></a>

### 1.1.   Class methods

Method-to-Unicode property mapping (PropertyAliases.txt):

<table>
    <thead>
      <tr>
        <th><small><small>Method</small></small></th>
        <th><small><small>Property type</small></small></th>
        <th><small><small>Short name</small></small></th>
        <th><small><small>Long name</small></small></th>
        <th><small><small>Comment</small></small></th>
      </tr>
    </thead>
    <tbody>
      <tr>
        <td><small><small><a href="#codepointBidiClass">codepointBidiClass</a></small></small></td>
        <td><small><small>Enumerated</small></small></td>
        <td><small><small>bc</small></small></td>
        <td><small><small>Bidi_Class</small></small></td>
        <td></td>
      </tr>
      <tr>
        <td><small><small><a href="#codepointBidiMirrored">codepointBidiMirrored</a></small></small></td>
        <td><small><small>Binary</small></small></td>
        <td><small><small>Bidi_M</small></small></td>
        <td><small><small>Bidi_Mirrored</small></small></td>
        <td></td>
      </tr>
      <tr>
        <td><small><small><a href="#codepointBoundClass">codepointBoundClass</a></small></small></td>
        <td><small><small>Enumerated</small></small></td>
        <td><small><small>GCB</small></small></td>
        <td><small><small>Grapheme_Cluster_Break</small></small></td>
        <td></td>
      </tr>
      <tr>
        <td><small><small><a href="#codepointCategory">codepointCategory</a></small></small></td>
        <td><small><small>Enumerated</small></small></td>
        <td><small><small>gc</small></small></td>
        <td><small><small>General_Category</small></small></td>
        <td></td>
      </tr>
      <tr>
        <td><small><small><a href="#codepointCharWidth">codepointCharWidth</a></small></small></td>
        <td></td>
        <td></td>
        <td></td>
        <td><small><small>Not a Unicode property</small></small></td>
      </tr>
      <tr>
        <td><small><small><a href="#codepointCombiningClass">codepointCombiningClass</a></small></small></td>
        <td><small><small>Enumerated</small></small></td>
        <td><small><small>ccc</small></small></td>
        <td><small><small>Canonical_Combining_Class</small></small></td>
        <td></td>
      </tr>
      <tr>
        <td><small><small><a href="#codepointControlBoundary">codepointControlBoundary</a></small></small></td>
        <td></td>
        <td></td>
        <td></td>
        <td><small><small>Not a Unicode property (this method may be deprecated)</small></small></td>
      </tr>
      <tr>
        <td><small><small><a href="#codepointDecompositionType">codepointDecompositionType</a></small></small></td>
        <td><small><small>Enumerated</small></small></td>
        <td><small><small>dt</small></small></td>
        <td><small><small>Decomposition_Type</small></small></td>
        <td></td>
      </tr>
      <tr>
        <td><small><small>codepointEastAsianWidth</small></small></td>
        <td><small><small>Enumerated</small></small></td>
        <td><small><small>ea</small></small></td>
        <td><small><small>East_Asian_Width</small></small></td>
        <td><small><small>Not available</small></small></td>
      </tr>
      <tr>
        <td><small><small><a href="#codepointEastAsianWidthIsAmbiguous">codepointEastAsianWidthIsAmbiguous</a></small></small></td>
        <td><small><small>Binary</small></small></td>
        <td></td>
        <td></td>
        <td><small><small>True if East Asian width class A</small></small></td>
      </tr>
      <tr>
        <td><small><small><a href="#codepointIgnorable">codepointIgnorable</a></small></small></td>
        <td><small><small>Binary</small></small></td>
        <td><small><small>DI</small></small></td>
        <td><small><small>Default_Ignorable_Code_Point</small></small></td>
        <td></td>
      </tr>
      <tr>
        <td><small><small><a href="#codepointIndicConjunctBreak">codepointIndicConjunctBreak</a></small></small></td>
        <td><small><small>Enumerated</small></small></td>
        <td><small><small>InCB</small></small></td>
        <td><small><small>Indic_Conjunct_Break</small></small></td>
        <td></td>
      </tr>
      <tr>
        <td><small><small><a href="#codepointIsLower">codepointIsLower</a></small></small></td>
        <td><small><small>Binary</small></small></td>
        <td><small><small>Lower</small></small></td>
        <td><small><small>Lowercase</small></small></td>
        <td></td>
      </tr>
      <tr>
        <td><small><small><a href="#codepointIsUpper">codepointIsUpper</a></small></small></td>
        <td><small><small>Binary</small></small></td>
        <td><small><small>Upper</small></small></td>
        <td><small><small>Uppercase</small></small></td>
        <td></td>
      </tr>
      <tr>
        <td><small><small><a href="#codepointToLower">codepointToLower</a></small></small></td>
        <td><small><small>String</small></small></td>
        <td><small><small>slc</small></small></td>
        <td><small><small>Simple_Lowercase_Mapping</small></small></td>
        <td></td>
      </tr>
      <tr>
        <td><small><small><a href="#codepointToTitle">codepointToTitle</a></small></small></td>
        <td><small><small>String</small></small></td>
        <td><small><small>stc</small></small></td>
        <td><small><small>Simple_Titlecase_Mapping</small></small></td>
        <td></td>
      </tr>
      <tr>
        <td><small><small><a href="#codepointToUpper">codepointToUpper</a></small></small></td>
        <td><small><small>String</small></small></td>
        <td><small><small>suc</small></small></td>
        <td><small><small>Simple_Uppercase_Mapping</small></small></td>
        <td></td>
      </tr>
    </tbody>
</table>


<a id="codepointBidiClass"></a>

#### 1.1.1.   codepointBidiClass

    .RexxUnicodeServices~codepointBidiClass(codepoint [, [>refCode] [, >refLabel]])

Returns the bidirectional character type of `codepoint` as an enumeration value.

- If provided, `refCode` receives the short name or `"?"`.
- If provided, `refLabel` receives the long name or `"Code point not explicitly listed for Bidi_Class"`.

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

<a id="codepointBidiMirrored"></a>

#### 1.1.2.   codepointBidiMirrored

    .RexxUnicodeServices~codepointBidiMirrored(codepoint)

Returns `.true` if the `Bidi_Mirrored` property value of `codepoint` is `Yes`.

[https://unicode.org/reports/tr9/#Mirroring][unicode_standard_annex_9_mirroring]


<a id="codepointBoundClass"></a>

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
.RexxUnicodeServices~codepointBoundClass("1F1E6"~x2d, >code, >label)=; code=; label=     -- 11; 'RI'; 'Regional_Indicator'
.RexxUnicodeServices~codepointBoundClass("FFFFFF"~x2d, >code, >label)=; code=; label=    --  1; 'XX'; 'Other'
.RexxUnicodeServices~codepointBoundClass(-1, >code, >label)=; code=; label=              --  1; 'XX'; 'Other'

```


<a id="codepointCategory"></a>

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


<a id="codepointCharWidth"></a>

#### 1.1.5.   codepointCharWidth

    .RexxUnicodeServices~codepointCharWidth(codepoint)

Given a codepoint, returns a character width analogous to `wcwidth(codepoint)`,
except that a width of 0 is returned for non-printable codepoints
instead of -1 as in `wcwidth`.


<a id="codepointCombiningClass"></a>

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


<a id="codepointControlBoundary"></a>

#### 1.1.7.   codepointControlBoundary

    .RexxUnicodeServices~codepointControlBoundary(codepoint)

Returns `.true` if `codepoint` belongs to the `Zl`, `Zp`, `Cc`, or `Cf` category, except for:

        (<?> \x{E2808C} U+200C Cf Format "ZERO WIDTH NON-JOINER")
        (<?> \x{E2808D} U+200D Cf Format "ZERO WIDTH JOINER")

> [!WARNING]  
> This rule is not used internally by `utf8proc`
> and does not appear in [Unicode Standard Annex #29][unicode_standard_annex_29].
> Therefore, this method may be deprecated in the future unless a use case is identified.


<a id="codepointDecompositionType"></a>

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


<a id="codepointEastAsianWidthIsAmbiguous"></a>

#### 1.1.9.   codepointEastAsianWidthIsAmbiguous

    .RexxUnicodeServices~codepointEastAsianWidthIsAmbiguous(codepoint)

Returns `.true` if the `East_Asian_Width` property value of `codepoint` is `"A"` (`"Ambiguous"`).

[https://www.unicode.org/reports/tr11/][unicode_standard_annex_11]


<a id="codepointIgnorable"></a>

#### 1.1.10.   codepointIgnorable

    .RexxUnicodeServices~codepointIgnorable(codepoint)

Returns the `Default_Ignorable_Code_Point` property value of `codepoint` (boolean value).


<a id="codepointIndicConjunctBreak"></a>

#### 1.1.11.   codepointIndicConjunctBreak

    .RexxUnicodeServices~codepointIndicConjunctBreak(codepoint [, [>refCode] [, >refLabel]])

Returns the `Indic_Conjunct_Break (InCB)` property value of `codepoint` as an enumeration value.

- If provided, `refCode` receives the short name or `"None"`.
- If provided, `refLabel` receives the long name or `"None"`.

The returned enumeration value is implementation-specific.  
For portability, use refCode instead.

**Enumeration values**

```rexx
-- https://www.unicode.org/reports/tr44/#Derivation_InCB
-- first column: short name
-- second column: long name
"None",         "None"
"Linker",       "Linker"
"Consonant",    "Consonant"
"Extend",       "Extend"
```

**Examples:**

```rexx
.RexxUnicode~codepointIndicConjunctBreak("0000"~x2d, >code, >label)=; code=; label=     --  0; 'None'; 'None'
.RexxUnicode~codepointIndicConjunctBreak("0300"~x2d, >code, >label)=; code=; label=     --  3; 'Extend'; 'Extend'
.RexxUnicode~codepointIndicConjunctBreak("0915"~x2d, >code, >label)=; code=; label=     --  2; 'Consonant'; 'Consonant'
.RexxUnicode~codepointIndicConjunctBreak("094D"~x2d, >code, >label)=; code=; label=     --  1; 'Linker'; 'Linker'
.RexxUnicode~codepointIndicConjunctBreak("FFFFFF"~x2d, >code, >label)=; code=; label=   --  0; 'None'; 'None'
.RexxUnicode~codepointIndicConjunctBreak(-1, >code, >label)=; code=; label=             --  0; 'None'; 'None'

```


<a id="codepointIsLower"></a>

#### 1.1.12.   codepointIsLower

    .RexxUnicodeServices~codepointIsLower(codepoint)

Returns `.true` if the codepoint corresponds to a lowercase character
and `.false` otherwise.



<a id="codepointIsUpper"></a>

#### 1.1.13.   codepointIsUpper

    .RexxUnicodeServices~codepointIsUpper(codepoint)

Returns `.true` if the codepoint corresponds to an uppercase character
and `.false` otherwise.


<a id="codepointToLower"></a>

#### 1.1.14.   codepointToLower

    .RexxUnicodeServices~codepointToLower(codepoint)

Returns the codepoint of the corresponding lowercase character.  
If `codepoint` has no lowercase mapping, returns `codepoint`.

This method uses the simple case mappings defined in `UnicodeData.txt`
and always returns a single codepoint.


<a id="codepointToTitle"></a>

#### 1.1.15.   codepointToTitle

    .RexxUnicodeServices~codepointToTitle(codepoint)

Returns the codepoint of the corresponding titlecase character.  
If `codepoint` has no titlecase mapping, returns `codepoint`.

This method uses the simple case mappings defined in `UnicodeData.txt`
and always returns a single codepoint.


<a id="codepointToUpper"></a>

#### 1.1.16.   codepointToUpper

    .RexxUnicodeServices~codepointToUpper(codepoint)

Returns the codepoint of the corresponding uppercase character.  
If `codepoint` has no uppercase mapping, returns `codepoint`.

This method uses the simple case mappings defined in `UnicodeData.txt`
and always returns a single codepoint.


<a id="graphemeBreak"></a>

#### 1.1.17.   graphemeBreak

    .RexxUnicodeServices~graphemeBreak(array)

Returns `.true` if there is a grapheme break between the two consecutive codepoints passed in `array`.

> [!WARNING]
> This method is deprecated. Use [`graphemeBreak3`](#graphemeBreak3) instead.
>
> Using an array for the persistent state can lead to subtle bugs
> when copying an object that uses such an array internally.
> By default, the copy is shallow, so both the original object and
> its copies refer to the same internal array. As a result, the
> state may be corrupted, depending on the sequence of calls made
> on the original object and its copies.

**Example:**

```rexx
-- `state` is persistent state. Its initial value must be 0.
-- `graphemeBreak` updates `state`.
array = (previousCodepoint, currentCodepoint, state)
break = .RexxUnicodeServices~graphemeBreak(array) -- true or false

```


<a id="graphemeBreak3"></a>

#### 1.1.18.   graphemeBreak3

    .RexxUnicodeServices~graphemeBreak3(codepoint1, codepoint2, >refState)

Returns `.true` if there is a grapheme break between the two consecutive codepoints.

**Example:**

```rexx
-- `state` is persistent state. Its initial value must be 0.
-- `graphemeBreak3` updates `state`.
state = 0
.RexxUnicodeServices~graphemeBreak3(13, 10, >state)=    -- 0    CR + LF is a grapheme, no grapheme break inside
state=                                                  -- 3
.RexxUnicodeServices~graphemeBreak3(10, 65, >state)=    -- 1    There is a grapheme break between LF and 'A'
state=                                                  -- 1

```


<a id="RexxUnicodeServices_new"></a>

#### 1.1.19.   new

This method raises an error because `RexxUnicodeServices` has no instance.


<a id="systemIsLittleEndian"></a>

#### 1.1.20.   systemIsLittleEndian

    .RexxUnicodeServices~systemIsLittleEndian

Returns `.true` if the system is little-endian.


<a id="unicodeVersion"></a>

#### 1.1.21.   unicodeVersion

    .RexxUnicodeServices~unicodeVersion

Returns the Unicode version supported by the `utf8proc` library.

**Example:**

```rexx
say .RexxUnicodeServices~unicodeVersion        -- 17.0.0 (for example)

```


<a id="utf8DecodeCodepoint"></a>

#### 1.1.22.   utf8DecodeCodepoint

```
.RexxUnicodeServices~utf8DecodeCodepoint(string, indexB [, [>refSizeB] [, [>refErrorCode] [, >refErrorMsg]]])

.RexxUnicodeServices~utf8DecodeCodepoint(
    string,         -- (in)             A UTF-8 string.
    indexB,         -- (in)             The byte index (1-based) of the encoded codepoint in string.
                    --                  Valid range: 1..length+1.
    >refSizeB,      -- (out, optional)  The number of bytes read to decode the codepoint:
                    --                      > 0 if no error,
                    --                      < 0 if error,
                    --                      or 0 if indexB is at the end of the string (indexB == length+1).
    >refErrorCode,  -- (out, optional)  The null string "" if a valid codepoint could be read,
                    --                  or the error code otherwise.
    >refErrorMsg    -- (out, optional)  The null string "" if a valid codepoint could be read,
                    --                  or the error message otherwise.
    )

```

Returns the next codepoint (a whole number) at byte index `indexB` of `string`, or `-1` in case of decoding error.

`refSizeB` receives the size in bytes of the decoded codepoint.  
`refSizeB` is `0` when the end of the string has been reached.  
If `indexB > length+1`, an error `"Argument indexB is an invalid position value"` is raised.  
If `indexB <= 0`, an error `"Argument indexB is an invalid position value"` is raised.  
In case of decoding error, the received size is negative, indicating the number of bytes to skip
to follow the `U+FFFD` Substitution of Maximal Subparts.  
`refErrorCode` and `refErrorMsg` receive a detailed description of the error.

**Error codes and messages:**

```
- "CONTINUATION_ERROR_RANGE"        "Invalid continuation byte %i ('%02X'x) at byte position %zu (codepoint > U+10FFFF)"
- "CONTINUATION_HIGH_SURROGATE"     "Invalid continuation byte %i ('%02X'x) at byte position %zu (high surrogate)"
- "CONTINUATION_LOW_SURROGATE"      "Invalid continuation byte %i ('%02X'x) at byte position %zu (low surrogate)"
- "CONTINUATION"                    "Invalid continuation byte %i ('%02X'x) at byte position %zu"
- "CONTINUATION_NON_SHORTEST_FORM"  "Invalid continuation byte %i ('%02X'x) at byte position %zu (non-shortest form)"
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
            error = "start byte position" indexB ":" errorMsg
            codepoint = "FFFD"~x2d -- the current byte sequence is invalid, return the replacement character
            indexB = -indexB -- same convention as .RexxUnicodeStringIndexer; a negative index indicates an invalid byte sequence
        end
        byteSequence = string~substr(abs(indexB), abs(sizeB))
    end

```


<a id="utf8DecodePreviousCodepoint"></a>

#### 1.1.23.   utf8DecodePreviousCodepoint

```
.RexxUnicodeServices~utf8DecodePreviousCodepoint(string, indexB, [, [>refSizeB] [, [>refErrorCode] [, >refErrorMsg]]])


.RexxUnicodeServices~utf8DecodePreviousCodepoint(
    string,         -- (in)             A UTF-8 string.
    indexB,         -- (in)             The byte index (1-based) of the boundary before which to decode.
                    --                  Valid range: 1..length+1.
                    --                  1 means "nothing precedes the start of the string".
    >refSizeB,      -- (out, optional)  The number of bytes read to decode the codepoint:
                    --                      > 0 if no error,
                    --                      < 0 if error,
                    --                      or 0 if indexB is at the beginning of the string (indexB == 1).
    >refErrorCode,  -- (out, optional)  The null string "" if a valid codepoint could be read,
                    --                  or the error code otherwise.
    >refErrorMsg    -- (out, optional)  The null string "" if a valid codepoint could be read,
                    --                  or the error message otherwise.
    )

```

Returns the codepoint (a whole number) immediately preceding the boundary at `indexB`,
or `-1` in case of decoding error.

The index has the same meaning as in `utf8DecodeCodepoint`: the 1-based byte position of a codepoint's first byte.  
This method returns whatever codepoint's bytes lie immediately before that position,
so passing the same `indexB` value to `utf8DecodeCodepoint` and `utf8DecodePreviousCodepoint`
gives you "this codepoint" and "the one right before it" respectively.  
Passing `indexB = length+1` returns the last codepoint of the string.

`refSizeB` receives the size in bytes of the decoded codepoint.  
`refSizeB` is `0` when the beginning of the string has been reached.  
If `indexB > length+1`, an error `"Argument indexB is an invalid position value"` is raised.  
If `indexB <= 0`, an error `"Argument indexB is an invalid position value"` is raised.  
In case of decoding error, the received size is negative, indicating the number of
bytes to skip to follow the `U+FFFD` Substitution of Maximal Subparts.  
`refErrorCode` and `refErrorMsg` receive a detailed description of the error.

**Error codes and messages:**

Same as `utf8DecodeCodepoint`.

**Examples:**

```rexx
-- Example 1
-- Empty string, correct call with indexB == length+1
.RexxUnicodeServices~utf8DecodePreviousCodepoint("", 1, >size, >errorCode, >errorMsg)=; size=; errorCode=; errorMsg=    -- -1; 0; ''; ''
-- Empty string, out of range call with indexB == 0
.RexxUnicodeServices~utf8DecodePreviousCodepoint("", 0, >size, >errorCode, >errorMsg)=; size=; errorCode=; errorMsg=    -- Argument indexB is an invalid position value; found "0".

```

```rexx
-- Example 2
string = "eé€🎅"
indexB = string~length + 1; indexB=                                                 -- 11
.RexxUnicodeServices~utf8DecodePreviousCodepoint(string, indexB, >size)=; size=     -- 127877; 4
indexB -= abs(size); indexB=                                                        -- 7
.RexxUnicodeServices~utf8DecodePreviousCodepoint(string, indexB, >size)=; size=     -- 8364; 3
indexB -= abs(size); indexB=                                                        -- 4
.RexxUnicodeServices~utf8DecodePreviousCodepoint(string, indexB, >size)=; size=     -- 233; 2
indexB -= abs(size); indexB=                                                        -- 2
.RexxUnicodeServices~utf8DecodePreviousCodepoint(string, indexB, >size)=; size=     -- 101; 1
indexB -= abs(size); indexB=                                                        -- 1        beginning of string
.RexxUnicodeServices~utf8DecodePreviousCodepoint(string, indexB, >size)=; size=     -- -1; 0    size == 0

-- indexB outside the string range 1..length+1
string = "eé€🎅"
.RexxUnicodeServices~utf8DecodePreviousCodepoint(string, 100)=      -- Invalid position argument specified; found "100".
.RexxUnicodeServices~utf8DecodePreviousCodepoint(string, 12)=       -- Invalid position argument specified; found "12".
.RexxUnicodeServices~utf8DecodePreviousCodepoint(string, 0)=        -- Invalid position argument specified; found "0".
.RexxUnicodeServices~utf8DecodePreviousCodepoint(string, -100)=     -- Argument indexB is an invalid position value; found "-100".

```

```rexx
-- Example 3
-- Invalid string
-- U+FFFD Substitution of Maximal Subparts
-- https://www.unicode.org/versions/Unicode17.0.0/core-spec/chapter-3/#G68202
string = "E1 80 E2 F0 91 92 F1 BF 41"x
indexB = string~length + 1; indexB=                                                                                             -- 10
.RexxUnicodeServices~utf8DecodePreviousCodepoint(string, indexB, >size, >errorCode, >errorMsg)=; size=; errorCode=; errorMsg=   -- 65; 1; ''; ''
indexB -= abs(size); indexB=                                                                                                    -- 9
.RexxUnicodeServices~utf8DecodePreviousCodepoint(string, indexB, >size, >errorCode, >errorMsg)=; size=; errorCode=; errorMsg=   -- -1; -2
/*
    -- 'CONTINUATION'
    -- 'Invalid continuation byte 65 (''41''x) at byte position 9'
*/
indexB -= abs(size); indexB=                                                                                                    -- 7
.RexxUnicodeServices~utf8DecodePreviousCodepoint(string, indexB, >size, >errorCode, >errorMsg)=; size=; errorCode=; errorMsg=   -- -1; -3
/*
    -- 'CONTINUATION'
    -- 'Invalid continuation byte 241 (''F1''x) at byte position 7'
*/
indexB -= abs(size); indexB=                                                                                                    -- 4
.RexxUnicodeServices~utf8DecodePreviousCodepoint(string, indexB, >size, >errorCode, >errorMsg)=; size=; errorCode=; errorMsg=   -- -1; -1
/*
    -- 'CONTINUATION'
    -- 'Invalid continuation byte 240 (''F0''x) at byte position 4'
*/
indexB -= abs(size); indexB=                                                                                                    -- 3
.RexxUnicodeServices~utf8DecodePreviousCodepoint(string, indexB, >size, >errorCode, >errorMsg)=; size=; errorCode=; errorMsg=   -- -1; -2
/*
    -- 'CONTINUATION'
    -- 'Invalid continuation byte 226 (''E2''x) at byte position 3'
*/
indexB -= abs(size); indexB=                                                                                                    -- 1                                                                                                  -- 3
.RexxUnicodeServices~utf8DecodePreviousCodepoint(string, indexB, >size, >errorCode, >errorMsg)=; size=; errorCode=; errorMsg=   -- -1; 0; ''; ''

```


<a id="utf8EncodeCodepoint"></a>

#### 1.1.24.   utf8EncodeCodepoint

```
.RexxUnicodeServices~utf8EncodeCodepoint(codepoint, destination [, >refSizeB])

.RexxUnicodeServices~utf8EncodeCodepoint(
    codepoint,      -- (in)             The codepoint to encode (a non-negative whole number).
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


<a id="utf8procVersion"></a>

#### 1.1.25.   utf8procVersion

    .RexxUnicodeServices~utf8procVersion

Returns the version of the `utf8proc` library.

**Example:**

```rexx
say .RexxUnicodeServices~utf8procVersion        -- 2.11.3 (for example)

```


<a id="utf8Transform"></a>

#### 1.1.26.   utf8Transform

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

##### 1.1.26.1.   'caseFold' argument

Performs unicode case folding, to be able to do a case-insensitive
string comparison.


##### 1.1.26.2.   'lump' argument

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


##### 1.1.26.3.   'nlf' argument

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


##### 1.1.26.4.   'normalization' argument

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


##### 1.1.26.5.   'stripCC' argument

Strips and/or converts control characters.

NLF-sequences are transformed into space, except if one of the
NLF2LS/PS/LF options is given. HorizontalTab (HT) and FormFeed (FF)
are treated as a NLF-sequence in this case.

All other control characters are simply removed.


##### 1.1.26.6.   'stripIgnorable' argument

Strips the characters whose property `Default_Ignorable_Code_Point` is true,
such as `SOFT-HYPHEN` or `ZERO-WIDTH-SPACE`.


##### 1.1.26.7.   'stripMark' argument

Strips all character markings.

This includes non-spacing, spacing and enclosing (i.e. accents) categories:

- `Mc` Spacing Mark
- `Me` Enclosing Mark
- `Mn` Nonspacing Mark

This option works only with a normalization applied.


##### 1.1.26.8.   'stripNA' argument

Strips the characters whose category is `Cn` Unassigned.


##### 1.1.26.9.   Examples of transformations

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




<!----------------------------------------------------------------------------->

<a id="RexxUnicode"></a>

## 2.   RexxUnicode Class

<!----------------------------------------------------------------------------->

The `RexxUnicode` class cannot be instantiated.  
Its services are provided through class methods.

`::requires "rxunicode.cls"`


### 2.1.   Methods inherited from the `ICU4ooRexxInterface` class

[`ICU4ooRexxInterface`](#ICU4ooRexxInterface) class.

Inherited methods:

- [`assertICU4ooRexxIsRegistered`](#ICU4ooRexxInterface_assertICU4ooRexxIsRegistered)
- [`ICU4ooRexxIsRegistered`](#ICU4ooRexxInterface_ICU4ooRexxIsRegistered)
- [`ICU4ooRexxPackage`](#ICU4ooRexxInterface_ICU4ooRexxPackage)
- [`registerICU4ooRexx`](#ICU4ooRexxInterface_registerICU4ooRexx)


### 2.2.   Methods inherited from the `RexxUnicodeServices` class

[`RexxUnicodeServices`](#RexxUnicodeServices) class.

Inherited methods:

- [`codepointBidiClass`](#codepointBidiClass)
- [`codepointBidiMirrored`](#codepointBidiMirrored)
- [`codepointBoundClass`](#codepointBoundClass)
- [`codepointCategory`](#codepointCategory)
- [`codepointCharWidth`](#codepointCharWidth)
- [`codepointCombiningClass`](#codepointCombiningClass)
- [`codepointControlBoundary`](#codepointControlBoundary)
- [`codepointDecompositionType`](#codepointDecompositionType)
- [`codepointEastAsianWidthIsAmbiguous`](#codepointEastAsianWidthIsAmbiguous)
- [`codepointIgnorable`](#codepointIgnorable)
- [`codepointIndicConjunctBreak`](#codepointIndicConjunctBreak)
- [`codepointIsLower`](#codepointIsLower)
- [`codepointIsUpper`](#codepointIsUpper)
- [`codepointToLower`](#codepointToLower)
- [`codepointToTitle`](#codepointToTitle)
- [`codepointToUpper`](#codepointToUpper)
- [`graphemeBreak`](#graphemeBreak)
- [`systemIsLittleEndian`](#systemIsLittleEndian)
- [`unicodeVersion`](#unicodeVersion)
- [`utf8DecodeCodepoint`](#utf8DecodeCodepoint)
- [`utf8DecodePreviousCodepoint`](#utf8DecodePreviousCodepoint)
- [`utf8EncodeCodepoint`](#utf8EncodeCodepoint)
- [`utf8procVersion`](#utf8procVersion)
- [`utf8Transform`](#utf8Transform)


### 2.3.   Constants

<a id="maxCodepoint"></a>

#### 2.3.1.   maxCodepoint

```rexx
::constant maxCodepoint 1114111
```


<a id="NFC_NFD_NFKC_NFKD"></a>

#### 2.3.2.   NFC, NFD, NFKC, NFKD

```rexx
-- Value to pass as the `normalization` argument to utf8Transform (default: 0 no normalization):
::constant NFC 1
::constant NFD 2
::constant NFKC 3
::constant NFKD 4
```


<a id="NLF2LF_NLF2LS_NLF2PS"></a>

#### 2.3.3.   NLF2LF, NLF2LS, NLF2PS

```rexx
-- Value to pass as the `nlf` argument to utf8Transform (default: 0 no transformation).
::constant NLF2LF 1
::constant NLF2LS 2
::constant NLF2PS 3
```

See the documentation of [utf8Transform](#utf8Transform) for a description of these values.



### 2.4.   Class methods

<a id="C2U"></a>

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


<a id="C2X"></a>

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


<a id="character"></a>

#### 2.4.3.   character

    .RexxUnicode~character(identifier)

Gets a RexxUnicodeCharacter from a codepoint identifier.  
See the [`codepoint`](#codepoint) method for a definition of "codepoint identifier".

**Examples:**

```rexx
-- Search by decimal value
.RexxUnicode~character(127877)=                 -- ("🎅" \x{F09F8E85} U+1F385 So Other_Symbol "FATHER CHRISTMAS")
.RexxUnicode~character("127877")=               -- ("🎅" \x{F09F8E85} U+1F385 So Other_Symbol "FATHER CHRISTMAS")
.RexxUnicode~character("1F385"~x2d)=            -- ("🎅" \x{F09F8E85} U+1F385 So Other_Symbol "FATHER CHRISTMAS")

```

```rexx
-- Search by U+ notation
.RexxUnicode~character("U+1F385")=              -- ("🎅" \x{F09F8E85} U+1F385 So Other_Symbol "FATHER CHRISTMAS")

```

```rexx
-- Search by name
.RexxUnicode~character("FATHER CHRISTMAS")=     -- ("🎅" \x{F09F8E85} U+1F385 So Other_Symbol "FATHER CHRISTMAS")

```

```rexx
-- search by loose name.
-- https://unicode.org/reports/tr44/#UAX44-LM2
.RexxUnicode~character(" Father christmas  ")=  -- ("🎅" \x{F09F8E85} U+1F385 So Other_Symbol "FATHER CHRISTMAS")

```

```rexx
-- Search directly a character
.RexxUnicode~character("🎅")=                   -- ("🎅" \x{F09F8E85} U+1F385 So Other_Symbol "FATHER CHRISTMAS")

```

```rexx
-- Only -1 is supported as a special value
-- All other negative codepoints are invalid
.RexxUnicode~character(-1)=                     -- (An invalid character)
.RexxUnicode~character(-2)=                     -- Invalid code point -2; allowed range is 0 to 1114111 (U+10FFFF).

```


<a id="checkCodepoint"></a>

#### 2.4.4.   checkCodepoint

    .RexxUnicode~checkCodepoint(codepoint, minusOneAllowed=.false)

Returns `.true` if `codepoint` is in the range 0...RexxUnicode~maxCodepoint.  
The range `U+D800-U+DFFF` is allowed (surrogates).  
If `minusOneAllowed` is true then -1 is allowed (special value used in case of error).


<a id="codepoint"></a>

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
.RexxUnicode~codepoint(127877)=                 -- 127877
.RexxUnicode~codepoint("127877")=               -- 127877
.RexxUnicode~codepoint("1F385"~x2d)=            -- 127877

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


<a id="codepointCharName"></a>

#### 2.4.6.   codepointCharName

    .RexxUnicode~codepointCharName(codepoint, raiseError=.false)

Returns the Unicode standard name for the given codepoint, or an empty string if the codepoint has no Unicode standard name.

This method requires the `ICU4ooRexx` class. If it is not loaded, the method returns an empty string or, if requested, raises an error.

**Examples:**

 ```rexx
.RexxUnicode~codepointCharName(2448~x2d)=       -- 'OCR DASH'
.RexxUnicode~codepointCharName("D800"~x2d)=     -- ''

```

```rexx
-- The special value -1 has no name
.RexxUnicode~codepointCharName(-1)=             -- ''

```


<a id="codepointCharNameAlias"></a>

#### 2.4.7.   codepointCharNameAlias

    .RexxUnicode~codepointCharNameAlias(codepoint, raiseError=.false)

Returns the Unicode name alias for the given codepoint, or an empty string if the codepoint has no Unicode name alias.

This method requires the `ICU4ooRexx` class. If it is not loaded, the method returns an empty string or, if requested, raises an error.

**Examples:**

 ```rexx
.RexxUnicode~codepointCharNameAlias(2448~x2d)=       -- 'MICR ON US SYMBOL'
.RexxUnicode~codepointCharNameAlias("D800"~x2d)=     -- ''

```

```rexx
-- The special value -1 has no name alias
.RexxUnicode~codepointCharNameAlias(-1)=             -- ''

 ```


<a id="codepointExtendedCharName"></a>

#### 2.4.8.   codepointExtendedCharName

    .RexxUnicode~codepointExtendedCharName(codepoint, raiseError=.false)

Returns the Unicode extended name for the given codepoint.

All Unicode characters have an extended name.  
An extended name is either the standard name if defined, or a codepoint label algorithmically generated.

This method requires the `ICU4ooRexx` class. If it is not loaded, the method returns an empty string or, if requested, raises an error.

**Examples:**

```rexx
.RexxUnicode~codepointExtendedCharName(2448~x2d)=   -- 'OCR DASH'
.RexxUnicode~codepointExtendedCharName("D800"~x2d)= -- '<lead surrogate-D800>'

```

```rexx
-- The special value -1 has no extended name
.RexxUnicode~codepointExtendedCharName(-1)=         -- ''

```


<a id="codepointIsPrintable"></a>

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


<a id="codepointPrintableString"></a>

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


<a id="codepointUnicodeEscapeNotation"></a>

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


<a id="codepointUTF8Encoding"></a>

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


<a id="D2U"></a>

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


<a id="h_UAX44_LM2"></a>

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


<a id="info"></a>

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


<a id="stringEscape"></a>

#### 2.4.16.   stringEscape

    .RexxUnicode~stringEscape(string, buffer=.nil)

Returns a string in which non-printable codepoints and invalid byte sequences are replaced with escape sequences.

- Invalid byte sequences are represented as escaped hexadecimal byte sequences: `\xXX` or `\x{XX..XX}`.
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


<a id="stringInfo"></a>

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
     1 : 'start byte position 1 : Invalid continuation byte 226 (''E2''x) at byte position 3'
     2 : 'start byte position 3 : Invalid continuation byte 240 (''F0''x) at byte position 4'
     3 : 'start byte position 4 : Invalid continuation byte 241 (''F1''x) at byte position 7'
     4 : 'start byte position 7 : Invalid continuation byte 65 (''41''x) at byte position 9'
    */

```


<a id="stringIsASCII"></a>

#### 2.4.18.   stringIsASCII

    .RexxUnicode~stringIsASCII(string)

Returns `.true` if the given string contains only characters <= "7F"x

**Examples:**

```rexx
.RexxUnicode~stringIsASCII("Noel")=     -- 1
.RexxUnicode~stringIsASCII("Noël")=     -- 0

```


<a id="stringToNFC"></a>

#### 2.4.19.   stringToNFC

    .RexxUnicode~stringToNFC(string, casefold = .false, lump= .false, nlf = 0, stripCC = .false, stripIgnorable= .false, stripMark = .false, stripNA = .false)

Returns the input string normalized to NFC, with optional transformations applied.

See the [`utf8Transform`](#utf8Transform) method for a description of the optional transformation parameters.

**Examples:**

```rexx
.RexxUnicode~stringToNFC("äöü äöü x̂ ϔ ﷺ baﬄe ß ς")=      -- 'äöü äöü x̂ ϔ ﷺ baﬄe ß ς'
.RexxUnicode~C2X(result)=                                  -- 'C3A4 C3B6 C3BC 20 C3A4 C3B6 C3BC 20 78 CC82 20 CF94 20 EFB7BA 20 62 61 EFAC84 65 20 C39F 20 CF82'

```


<a id="stringToNFD"></a>

#### 2.4.20.   stringToNFD

    .RexxUnicode~stringToNFD(string, casefold = .false, lump= .false, nlf = 0, stripCC = .false, stripIgnorable= .false, stripMark = .false, stripNA = .false)

Returns the input string normalized to NFD, with optional transformations applied.

See the [`utf8Transform`](#utf8Transform) method for a description of the optional transformation parameters.

**Examples:**

```rexx
.RexxUnicode~stringToNFD("äöü äöü x̂ ϔ ﷺ baﬄe ß ς")=       -- 'äöü äöü x̂ ϔ ﷺ baﬄe ß ς'
.RexxUnicode~C2X(result)=                                   -- '61 CC88 6F CC88 75 CC88 20 61 CC88 6F CC88 75 CC88 20 78 CC82 20 CF92 CC88 20 EFB7BA 20 62 61 EFAC84 65 20 C39F 20 CF82'

```


<a id="stringToNFKC"></a>

#### 2.4.21.   stringToNFKC

    .RexxUnicode~stringToNFKC(string, casefold = .false, lump= .false, nlf = 0, stripCC = .false, stripIgnorable= .false, stripMark = .false, stripNA = .false)

Returns the input string normalized to NFKC, with optional transformations applied.

See the [`utf8Transform`](#utf8Transform) method for a description of the optional transformation parameters.

**Examples:**

```rexx
.RexxUnicode~stringToNFKC("äöü äöü x̂ ϔ ﷺ baﬄe ß ς")=      -- 'äöü äöü x̂ Ϋ صلى الله عليه وسلم baffle ß ς'
.RexxUnicode~C2X(result)=                                   -- 'C3A4 C3B6 C3BC 20 C3A4 C3B6 C3BC 20 78 CC82 20 CEAB 20 D8B5 D984 D989 20 D8A7 D984 D984 D987 20 D8B9 D984 D98A D987 20 D988 D8B3 D984 D985 20 62 61 66 66 6C 65 20 C39F 20 CF82'

```


<a id="stringToNFKC_CF"></a>

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

See the [`utf8Transform`](#utf8Transform) method for a description of the optional transformation parameters.

**Examples:**

```rexx
.RexxUnicode~stringToNFKC_CF("äöü äöü x̂ ϔ ﷺ baﬄe ß ς")=       -- 'äöü äöü x̂ ϋ صلى الله عليه وسلم baffle ss σ'
.RexxUnicode~C2X(result)=                                       -- 'C3A4 C3B6 C3BC 20 C3A4 C3B6 C3BC 20 78 CC82 20 CF8B 20 D8B5 D984 D989 20 D8A7 D984 D984 D987 20 D8B9 D984 D98A D987 20 D988 D8B3 D984 D985 20 62 61 66 66 6C 65 20 73 73 20 CF83'

```


<a id="stringToNFKD"></a>

#### 2.4.23.   stringToNFKD

    .RexxUnicode~stringToNFKD(string, casefold = .false, lump= .false, nlf = 0, stripCC = .false, stripIgnorable= .false, stripMark = .false, stripNA = .false)

Returns the input string normalized to NFKD, with optional transformations applied.

See the [`utf8Transform`](#utf8Transform) method for a description of the optional transformation parameters.

**Examples:**

```rexx
.RexxUnicode~stringToNFKD("äöü äöü x̂ ϔ ﷺ baﬄe ß ς")=      -- 'äöü äöü x̂ Ϋ صلى الله عليه وسلم baffle ß ς'
.RexxUnicode~C2X(result)=                                   -- '61 CC88 6F CC88 75 CC88 20 61 CC88 6F CC88 75 CC88 20 78 CC82 20 CEA5 CC88 20 D8B5 D984 D989 20 D8A7 D984 D984 D987 20 D8B9 D984 D98A D987 20 D988 D8B3 D984 D985 20 62 61 66 66 6C 65 20 C39F 20 CF82'

```


<a id="stringUnescape"></a>

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


<a id="U2C"></a>

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


<a id="U2D"></a>

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


<a id="UAX44_LM2"></a>

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


<a id="UAX44_LM3"></a>

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




<!----------------------------------------------------------------------------->

<a id="RexxUnicodeCodepointSupplier"></a>

## 3.   RexxUnicodeCodepointSupplier Class

<!----------------------------------------------------------------------------->

A `RexxUnicodeCodepointSupplier` instance is an iterator created from a 
UTF-8 string that allows enumeration of the string's codepoints without an indexer.

Byte indexes are counted from the beginning of the string.  
They can be used with `String` methods and string builtin functions.

Codepoint indexes are counted from the beginning of the string.  

- They can be used with [`aRexxUnicodeCodepointSupplier~codepointAtIndexC`](#RexxUnicodeCodepointSupplier_codepointAtIndexC),
provided that the supplier can only be advanced sequentially with `next`.
- They can be used with [`aRexxUnicodeStringIndexer~codepointAtIndexC`](#RexxUnicodeStringIndexer_codepointAtIndexC).

See [`RexxUnicodeReverseCodepointSupplier`](#RexxUnicodeReverseCodepointSupplier)
for a supplier that enumerates codepoints in reverse order.

`::requires "rxunicode.cls"`

**Examples:**

```rexx
-- Example 1
-- the default index type is the codepoint index
-- the default item type is the codepoint as a whole number
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
.RexxUnicodeCodepointSupplier~new("noël👨‍👩‍👧🎅", "b", "x")==
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
-- Codepoint supplier providing the default index and returning the codepoint escaped string
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
-- Codepoint supplier providing the default index and returning the codepoint in U+ notation
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
-- Codepoint supplier providing the default index and returning the codepoint as a RexxUnicodeCharacter
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

-- Codepoint supplier providing the codepoint index and returning the codepoint as a whole number
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
-- Codepoint supplier providing the byte index and returning the codepoint escaped string
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
-- Codepoint supplier providing the byte index and returning the error message
.RexxUnicodeCodepointSupplier~new("E1 80 E2 F0 91 92 F1 BF 41"x, "b", "e")==
    /*
    a RexxUnicodeCodepointSupplier 
    -1 : 'start byte position 1 : Invalid continuation byte 226 (''E2''x) at byte position 3'
    -3 : 'start byte position 3 : Invalid continuation byte 240 (''F0''x) at byte position 4'
    -4 : 'start byte position 4 : Invalid continuation byte 241 (''F1''x) at byte position 7'
    -7 : 'start byte position 7 : Invalid continuation byte 65 (''41''x) at byte position 9'
     9 : ''
    */

```

### 3.1.   Class methods

The `RexxUnicodeCodepointSupplier` class defines no class methods of its own.


### 3.2.   Instance methods

<a id="RexxUnicodeCodepointSupplier_available"></a>

#### 3.2.1.   available

    aRexxUnicodeCodepointSupplier~available

Returns `.true` if a codepoint is available from the supplier (that is, if the `item` method would return a value).  
Returns `.false` if the supplier has already enumerated all codepoints.


<a id="RexxUnicodeCodepointSupplier_codepointAtIndexC"></a>

#### 3.2.2.   codepointAtIndexC

    aRexxUnicodeCodepointSupplier~codepointAtIndexC(indexC)

Convenience method.  
Advances the supplier to codepoint index `indexC` and returns the corresponding codepoint as a whole number.  
`indexC` must be a positive whole number.  
If `indexC` is less than the current codepoint index, the supplier raises an error (can only advance).

This method does not support negative indexes (counting from the end of the string).

See also [`aRexxUnicodeStringIndexer~codepointAtIndexC`](#RexxUnicodeStringIndexer_codepointAtIndexC).


<a id="RexxUnicodeCodepointSupplier_index"></a>

#### 3.2.3.   index

    aRexxUnicodeCodepointSupplier~index(type=defaultIndexType)

Returns the index of the current codepoint in the string.  
If no codepoint is available, that is, if `available` would return `.false`, the supplier raises an error.

The following index types are supported:

- If `type`is `""`, the codepoint index is returned.
- If `type` is `"b"`, the byte index is returned (it is negative if the byte sequence is invalid).

The default index type is specified when the supplier is created.

Byte indexes are counted from the beginning of the string.  
Codepoint indexes are counted from the beginning of the string.


<a id="RexxUnicodeCodepointSupplier_init"></a>

#### 3.2.4.   init

    aRexxUnicodeCodepointSupplier~init(string, defaultIndexType="", defaultItemType="")

Initializes a `RexxUnicodeCodepointSupplier` instance with the UTF-8 string `string`.

If specified, `defaultIndexType` defines the default index type returned by the `index` method.  
The default is `""` (codepoint index).  
See the [`index`](#RexxUnicodeCodepointSupplier_index) method for the list of possible values.

If specified, `defaultItemType` defines the default item type returned by the `item` method.  
The default value is `""` (codepoint as a whole number).  
See the [`item`](#RexxUnicodeCodepointSupplier_item) method for the list of possible values.


<a id="RexxUnicodeCodepointSupplier_item"></a>

#### 3.2.5.   item

    aRexxUnicodeCodepointSupplier~item(type=defaultItemType)

Returns the current codepoint in the string.  
If no codepoint is available, that is, if `available` would return `.false`, the supplier raises an error.

The following item types are supported:

- If `type` is `""`, the codepoint is returned as a whole number.
- If `type` is `"b"`, the codepoint byte sequence is returned as a string.
- If `type` is `"e"`, the error message is returned (an empty string if there is no error).
- If `type` is `"u"`, the codepoint is returned in `U+` notation.
- If `type` is `"x"`, the codepoint byte sequence is returned as hexadecimal digits.
- If `type` is `"\"`, the codepoint byte sequence is returned as an escaped string.
- If `type` is a class, a new instance of that class is returned, initialized with the result of `item("")`.

The default item type is specified when the supplier is created.


<a id="RexxUnicodeCodepointSupplier_next"></a>

#### 3.2.6.   next

    aRexxUnicodeCodepointSupplier~next

Moves to the next codepoint in the string.  
By repeatedly sending `next` to the supplier (as long as `available` returns `.true`), you can enumerate all codepoints in the string.  
If no codepoint is available, that is, if `available` would return `.false`, the supplier raises an error.




<!----------------------------------------------------------------------------->

<a id="RexxUnicodeGraphemeSupplier"></a>

## 4.   RexxUnicodeGraphemeSupplier Class

<!----------------------------------------------------------------------------->

A `RexxUnicodeGraphemeSupplier` instance is an iterator created from a 
UTF-8 string that allows enumeration of the string's graphemes without an indexer.

Byte indexes are counted from the beginning of the string.  
They can be used with `String` methods and string builtin functions.

Grapheme indexes are counted from the beginning of the string.  

- They can be used with [`aRexxUnicodeGraphemeSupplier~graphemeAtIndexG`](#RexxUnicodeGraphemeSupplier_graphemeAtIndexG),
provided that the supplier can only be advanced sequentially with `next`.
- They can be used with [`aRexxUnicodeStringIndexer~graphemeAtIndexG`](#RexxUnicodeStringIndexer_graphemeAtIndexG).

See [`RexxUnicodeReverseGraphemeSupplier`](#RexxUnicodeReverseGraphemeSupplier)
for a supplier that enumerates graphemes in reverse order.

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
-- Grapheme supplier providing the byte index and returning the grapheme byte sequence as hex digits
.RexxUnicodeGraphemeSupplier~new("noël👨‍👩‍👧🎅", "b", "x")==
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
-- Grapheme supplier providing the default index and returning the grapheme escaped string
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
-- Grapheme supplier providing the default index and returning the grapheme codepoints in U+ notation
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

-- Grapheme supplier providing the grapheme index and returning the grapheme escaped string
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
-- Grapheme supplier providing the byte index and returning the grapheme escaped string
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
-- Grapheme supplier providing the byte index and returning the error message
.RexxUnicodeGraphemeSupplier~new("E1 80 E2 F0 91 92 F1 BF 41"x, "b", "e")==
    /*
    a RexxUnicodeGraphemeSupplier 
    -1 : 'start byte position 1 : Invalid continuation byte 226 (''E2''x) at byte position 3'
    -3 : 'start byte position 3 : Invalid continuation byte 240 (''F0''x) at byte position 4'
    -4 : 'start byte position 4 : Invalid continuation byte 241 (''F1''x) at byte position 7'
    -7 : 'start byte position 7 : Invalid continuation byte 65 (''41''x) at byte position 9'
     9 : ''
    */

```

### 4.1.   Class methods

The `RexxUnicodeGraphemeSupplier` class defines no class methods of its own.


### 4.2.   Instance methods

<a id="RexxUnicodeGraphemeSupplier_available"></a>

#### 4.2.1.   available

    aRexxUnicodeGraphemeSupplier~available

Returns `.true` if a grapheme is available from the supplier (that is, if the `item` method would return a value).  
Returns `.false` if the supplier has already enumerated all graphemes.


<a id="RexxUnicodeGraphemeSupplier_graphemeAtIndexG"></a>

#### 4.2.2.   graphemeAtIndexG

    aRexxUnicodeGraphemeSupplier~graphemeAtIndexG(indexG)

Convenience method.  
Advances the supplier to grapheme index `indexG` and returns the corresponding grapheme as a string.  
If `indexG` is less than the current grapheme index, the supplier raises an error (can only advance).  
This method does not support negative indexes (counting from the end of the string).

See also [`aRexxUnicodeStringIndexer~graphemeAtIndexG`](#RexxUnicodeStringIndexer_graphemeAtIndexG).


<a id="RexxUnicodeGraphemeSupplier_index"></a>

#### 4.2.3.   index

    aRexxUnicodeGraphemeSupplier~index(type=defaultIndexType)

Returns the index of the current grapheme in the string.  
If no grapheme is available, that is, if `available` would return `.false`, the supplier raises an error.

The following index types are supported:

- If `type`is `""`, the grapheme index is returned.
- If `type` is `"b"`, the byte index is returned (it is negative if the byte sequence is invalid).

The default index type is specified when the supplier is created.

Byte indexes are counted from the beginning of the string.  
Grapheme indexes are counted from the beginning of the string.


<a id="RexxUnicodeGraphemeSupplier_init"></a>

#### 4.2.4.   init

    aRexxUnicodeGraphemeSupplier~init(string, defaultIndexType="", defaultItemType="")

Initializes a `RexxUnicodeGraphemeSupplier` instance with the UTF-8 string `string`.

If specified, `defaultIndexType` defines the default index type returned by the `index` method.  
The default is `""` (grapheme index).  
See the [`index`](#RexxUnicodeGraphemeSupplier_index) method for the list of possible values.

If specified, `defaultItemType` defines the default item type returned by the `item` method.  
The default value is `""` (grapheme as a string).  
See the [`item`](#RexxUnicodeGraphemeSupplier_item) method for the list of possible values.


<a id="RexxUnicodeGraphemeSupplier_item"></a>

#### 4.2.5.   item

    aRexxUnicodeGraphemeSupplier~item(type=defaultItemType)

Returns the current grapheme in the string.  
If no grapheme is available, that is, if `available` would return `.false`, the supplier raises an error.

The following item types are supported:

- If `type` is `""`, the grapheme is returned as a string.
- If `type` is `"b"`, the grapheme byte sequence is returned as a string (same as `""`).
- If `type` is `"e"`, the error message is returned (an empty string if there is no error).
- If `type` is `"u"`, the grapheme is returned in `U+` notation.
- If `type` is `"x"`, the grapheme byte sequence is returned as hexadecimal digits.
- If `type` is `"\"`, the grapheme byte sequence is returned as an escaped string.
- If `type` is a class, a new instance of that class is returned, initialized with the result of `item("")`.

The default item type is specified when the supplier is created.


<a id="RexxUnicodeGraphemeSupplier_next"></a>

#### 4.2.6.   next

    aRexxUnicodeGraphemeSupplier~next

Moves to the next grapheme in the string.  
By repeatedly sending `next` to the supplier (as long as `available` returns `.true`), you can enumerate all graphemes in the string.  
If no grapheme is available, that is, if `available` would return `.false`, the supplier raises an error.




<!----------------------------------------------------------------------------->

<a id="RexxUnicodeReverseCodepointSupplier"></a>

## 5.   RexxUnicodeReverseCodepointSupplier Class

<!----------------------------------------------------------------------------->

A `RexxUnicodeReverseCodepointSupplier` instance is an iterator created from a 
UTF-8 string that allows enumeration of the string's codepoints in reverse order
without an indexer.

Byte indexes are counted from the beginning of the string.  
They can be used with `String` methods and string builtin functions.

Codepoint indexes are counted from the end of the string.  

- They can be used with [`aRexxUnicodeReverseCodepointSupplier~codepointAtIndexC`](#RexxUnicodeReverseCodepointSupplier_codepointAtIndexC),
provided that the supplier can only be advanced sequentially with `next`. Here, "advance"
refers to moving the supplier cursor, not the direction in the original string.
- They can be used with [`aRexxUnicodeStringIndexer~codepointAtIndexC`](#RexxUnicodeStringIndexer_codepointAtIndexC)
by passing them as negated values.

See [`RexxUnicodeCodepointSupplier`](#RexxUnicodeCodepointSupplier)
for a supplier that enumerates codepoints in forward order.

`::requires "rxunicode.cls"`

**Examples:**

```rexx
-- Example 1
-- the default index type is the codepoint index
-- the default item type is the codepoint as a whole number
.RexxUnicodeReverseCodepointSupplier~new("noël👨‍👩‍👧🎅")==
    /*
    a RexxUnicodeReverseCodepointSupplier 
     1  :  127877
     2  :  128103
     3  :  8205
     4  :  128105
     5  :  8205
     6  :  128104
     7  :  108
     8  :  235
     9  :  111
     10 :  110
    */

```

```rexx
-- Example 2
-- It's possible to get other types of index and item
-- Here, we request the byte index and the codepoint byte sequence as hex digits
.RexxUnicodeReverseCodepointSupplier~new("noël👨‍👩‍👧🎅", "b", "x")==
    /*
    a RexxUnicodeReverseCodepointSupplier 
     24 : '\x{F09F8E85}'
     20 : '\x{F09F91A7}'
     17 : '\x{E2808D}'
     13 : '\x{F09F91A9}'
     10 : '\x{E2808D}'
     6  : '\x{F09F91A8}'
     5  : '\x6C'
     3  : '\x{C3AB}'
     2  : '\x6F'
     1  : '\x6E'
    */

```

```rexx
-- Example 3
-- Codepoint supplier providing the default index and returning the codepoint escaped string
.RexxUnicodeReverseCodepointSupplier~new("noël👨‍👩‍👧🎅", , "\")==
    /*
    a RexxUnicodeReverseCodepointSupplier 
     1  : '🎅'
     2  : '👧'
     3  : '\u200D'
     4  : '👩'
     5  : '\u200D'
     6  : '👨'
     7  : 'l'
     8  : 'ë'
     9  : 'o'
     10 : 'n'
    */

```

```rexx
-- Example 4
-- Codepoint supplier providing the default index and returning the codepoint in U+ notation
.RexxUnicodeReverseCodepointSupplier~new("noël👨‍👩‍👧🎅", , "u")==
    /*
    a RexxUnicodeReverseCodepointSupplier 
     1  : 'U+1F385'
     2  : 'U+1F467'
     3  : 'U+200D'
     4  : 'U+1F469'
     5  : 'U+200D'
     6  : 'U+1F468'
     7  : 'U+006C'
     8  : 'U+00EB'
     9  : 'U+006F'
     10 : 'U+006E'
    */

```

```rexx
-- Example 5
-- Codepoint supplier providing the default index and returning the codepoint as a RexxUnicodeCharacter
.RexxUnicodeReverseCodepointSupplier~new("noël👨‍👩‍👧🎅", , .RexxUnicodeCharacter)==
    /*
    a RexxUnicodeReverseCodepointSupplier 
     1  : ("🎅" \x{F09F8E85} U+1F385 So Other_Symbol "FATHER CHRISTMAS")
     2  : ("👧" \x{F09F91A7} U+1F467 So Other_Symbol "GIRL")
     3  : (<?> \x{E2808D} U+200D Cf Format "ZERO WIDTH JOINER")
     4  : ("👩" \x{F09F91A9} U+1F469 So Other_Symbol "WOMAN")
     5  : (<?> \x{E2808D} U+200D Cf Format "ZERO WIDTH JOINER")
     6  : ("👨" \x{F09F91A8} U+1F468 So Other_Symbol "MAN")
     7  : ("l" \x6C U+006C Ll Lowercase_Letter "LATIN SMALL LETTER L")
     8  : ("ë" \x{C3AB} U+00EB Ll Lowercase_Letter "LATIN SMALL LETTER E WITH DIAERESIS")
     9  : ("o" \x6F U+006F Ll Lowercase_Letter "LATIN SMALL LETTER O")
     10 : ("n" \x6E U+006E Ll Lowercase_Letter "LATIN SMALL LETTER N")
    */

```

```rexx
-- Example 6
-- Invalid string - U+FFFD Substitution of Maximal Subparts
-- https://www.unicode.org/versions/Unicode17.0.0/core-spec/chapter-3/#G68202

-- Codepoint supplier providing the codepoint index and returning the codepoint as a whole number
.RexxUnicodeReverseCodepointSupplier~new("E1 80 E2 F0 91 92 F1 BF 41"x)==
    /*
    a RexxUnicodeReverseCodepointSupplier 
     1 :  65
     2 :  65533
     3 :  65533
     4 :  65533
     5 :  65533
    */

```

```rexx
-- Example 7
-- Codepoint supplier providing the byte index and returning the codepoint escaped string
-- A negative byte index indicates an invalid byte sequence
.RexxUnicodeReverseCodepointSupplier~new("E1 80 E2 F0 91 92 F1 BF 41"x, "b", "\")==
    /*
    a RexxUnicodeReverseCodepointSupplier 
     9 : 'A'
    -7 : '\x{F1BF}'
    -4 : '\x{F09192}'
    -3 : '\xE2'
    -1 : '\x{E180}'
    */

```

```rexx
-- Example 8
-- Codepoint supplier providing the byte index and returning the error message
.RexxUnicodeReverseCodepointSupplier~new("E1 80 E2 F0 91 92 F1 BF 41"x, "b", "e")==
    /*
    a RexxUnicodeReverseCodepointSupplier 
     9 : ''
    -7 : 'start byte position 7 : Invalid continuation byte 65 (''41''x) at byte position 9'
    -4 : 'start byte position 4 : Invalid continuation byte 241 (''F1''x) at byte position 7'
    -3 : 'start byte position 3 : Invalid continuation byte 240 (''F0''x) at byte position 4'
    -1 : 'start byte position 1 : Invalid continuation byte 226 (''E2''x) at byte position 3'
    */

```

### 5.1.   Class methods

The `RexxUnicodeReverseCodepointSupplier` class defines no class methods of its own.


### 5.2.   Instance methods

<a id="RexxUnicodeReverseCodepointSupplier_available"></a>

#### 5.2.1.   available

    aRexxUnicodeReverseCodepointSupplier~available

Returns `.true` if a codepoint is available from the supplier (that is, if the `item` method would return a value).  
Returns `.false` if the supplier has already enumerated all codepoints.


<a id="RexxUnicodeReverseCodepointSupplier_codepointAtIndexC"></a>

#### 5.2.2.   codepointAtIndexC

    aRexxUnicodeReverseCodepointSupplier~codepointAtIndexC(indexC)

Convenience method.  
Advances the supplier to codepoint index `indexC` and returns the corresponding codepoint as a whole number.  
Here, "advance" refers to moving the supplier cursor, not the direction in the original string.  
`indexC` must be a positive whole number.  
If `indexC` is less than the current codepoint index, the supplier raises an error (can only advance).

This method does not support negative indexes (counting from the beginning of the string).

See also [`aRexxUnicodeStringIndexer~codepointAtIndexC`](#RexxUnicodeStringIndexer_codepointAtIndexC).


<a id="RexxUnicodeReverseCodepointSupplier_index"></a>

#### 5.2.3.   index

    aRexxUnicodeReverseCodepointSupplier~index(type=defaultIndexType)

Returns the index of the current codepoint in the string.  
If no codepoint is available, that is, if `available` would return `.false`, the supplier raises an error.

The following index types are supported:

- If `type`is `""`, the codepoint index is returned.
- If `type` is `"b"`, the byte index is returned (it is negative if the byte sequence is invalid).

The default index type is specified when the supplier is created.

Byte indexes are counted from the beginning of the string.  
Codepoint indexes are counted from the end of the string.


<a id="RexxUnicodeReverseCodepointSupplier_init"></a>

#### 5.2.4.   init

    aRexxUnicodeReverseCodepointSupplier~init(string, defaultIndexType="", defaultItemType="")

Initializes a `RexxUnicodeReverseCodepointSupplier` instance with the UTF-8 string `string`.

If specified, `defaultIndexType` defines the default index type returned by the `index` method.  
The default is `""` (codepoint index).  
See the [`index`](#RexxUnicodeReverseCodepointSupplier_index) method for the list of possible values.

If specified, `defaultItemType` defines the default item type returned by the `item` method.  
The default value is `""` (codepoint as a whole number).  
See the [`item`](#RexxUnicodeReverseCodepointSupplier_item) method for the list of possible values.


<a id="RexxUnicodeReverseCodepointSupplier_item"></a>

#### 5.2.5.   item

    aRexxUnicodeReverseCodepointSupplier~item(type=defaultItemType)

Returns the current codepoint in the string.  
If no codepoint is available, that is, if `available` would return `.false`, the supplier raises an error.

The following item types are supported:

- If `type` is `""`, the codepoint is returned as a whole number.
- If `type` is `"b"`, the codepoint byte sequence is returned as a string.
- If `type` is `"e"`, the error message is returned (an empty string if there is no error).
- If `type` is `"u"`, the codepoint is returned in `U+` notation.
- If `type` is `"x"`, the codepoint byte sequence is returned as hexadecimal digits.
- If `type` is `"\"`, the codepoint byte sequence is returned as an escaped string.
- If `type` is a class, a new instance of that class is returned, initialized with the result of `item("")`.

The default item type is specified when the supplier is created.


<a id="RexxUnicodeReverseCodepointSupplier_next"></a>

#### 5.2.6.   next

    aRexxUnicodeReverseCodepointSupplier~next

Moves to the next codepoint in the string, in reverse order.  
By repeatedly sending `next` to the supplier (as long as `available` returns
`.true`), you can enumerate all codepoints in the string, in reverse order.  
If no codepoint is available, that is, if `available` would return `.false`,
the supplier raises an error.




<!----------------------------------------------------------------------------->

<a id="RexxUnicodeReverseGraphemeSupplier"></a>

## 6.   RexxUnicodeReverseGraphemeSupplier Class

<!----------------------------------------------------------------------------->

A `RexxUnicodeReverseGraphemeSupplier` instance is an iterator created from a 
UTF-8 string that allows enumeration of the string's graphemes in reverse order
without an indexer.

Byte indexes are counted from the beginning of the string.  
They can be used with `String` methods and string builtin functions.

Grapheme indexes are counted from the end of the string.  

- They can be used with [`aRexxUnicodeReverseGraphemeSupplier~graphemeAtIndexG`](#RexxUnicodeReverseGraphemeSupplier_graphemeAtIndexG),
provided that the supplier can only be advanced sequentially with `next`. Here,
"advance" refers to moving the supplier cursor, not the direction in the original
string.
- They can be used with [`aRexxUnicodeStringIndexer~graphemeAtIndexG`](#RexxUnicodeStringIndexer_graphemeAtIndexG).

See [`RexxUnicodeGraphemeSupplier`](#RexxUnicodeGraphemeSupplier)
for a supplier that enumerates graphemes in forward order.

`::requires "rxunicode.cls"`

**Examples:**

```rexx
-- Example 1
-- the default index type is the grapheme index
-- the default item type is the grapheme as a string
.RexxUnicodeReverseGraphemeSupplier~new("noël👨‍👩‍👧🎅")==
    /*
    a RexxUnicodeReverseGraphemeSupplier 
     1 : '🎅'
     2 : '👨‍👩‍👧'
     3 : 'l'
     4 : 'ë'
     5 : 'o'
     6 : 'n'
    */

```

```rexx
-- Example 2
-- Grapheme supplier providing the byte index and returning the grapheme byte sequence as hex digits
.RexxUnicodeReverseGraphemeSupplier~new("noël👨‍👩‍👧🎅", "b", "x")==
    /*
    a RexxUnicodeReverseGraphemeSupplier 
     24 : '\x{F09F8E85}'
     6  : '\x{F09F91A8 E2808D F09F91A9 E2808D F09F91A7}'
     5  : '\x6C'
     3  : '\x{C3AB}'
     2  : '\x6F'
     1  : '\x6E'
    */

```

```rexx
-- Example 3
-- Grapheme supplier providing the default index and returning the grapheme escaped string
.RexxUnicodeReverseGraphemeSupplier~new("noël👨‍👩‍👧🎅", , "\")==
    /*
    a RexxUnicodeReverseGraphemeSupplier 
     1 : '🎅'
     2 : '👨\u200D👩\u200D👧'
     3 : 'l'
     4 : 'ë'
     5 : 'o'
     6 : 'n'
    */

```

```rexx
-- Example 4
-- Grapheme supplier providing the default index and returning the grapheme codepoints in U+ notation
.RexxUnicodeReverseGraphemeSupplier~new("noël👨‍👩‍👧🎅", , "u")==
    /*
    a RexxUnicodeReverseGraphemeSupplier 
     1 : 'U+1F385'
     2 : 'U+1F468 U+200D U+1F469 U+200D U+1F467'
     3 : 'U+006C'
     4 : 'U+00EB'
     5 : 'U+006F'
     6 : 'U+006E'
    */

```

```rexx
-- Example 5
-- Invalid string - U+FFFD Substitution of Maximal Subparts
-- https://www.unicode.org/versions/Unicode17.0.0/core-spec/chapter-3/#G68202

-- Grapheme supplier providing the grapheme index and returning the grapheme escaped string
.RexxUnicodeReverseGraphemeSupplier~new("E1 80 E2 F0 91 92 F1 BF 41"x, , "\")==
    /*
    a RexxUnicodeReverseGraphemeSupplier 
     1 : 'A'
     2 : '\x{F1BF}'
     3 : '\x{F09192}'
     4 : '\xE2'
     5 : '\x{E180}'
    */

```

```rexx
-- Example 6
-- Grapheme supplier providing the byte index and returning the grapheme escaped string
-- A negative byte index indicates an invalid byte sequence
.RexxUnicodeReverseGraphemeSupplier~new("E1 80 E2 F0 91 92 F1 BF 41"x, "b", "\")==
    /*
    a RexxUnicodeReverseGraphemeSupplier 
     9 : 'A'
    -7 : '\x{F1BF}'
    -4 : '\x{F09192}'
    -3 : '\xE2'
    -1 : '\x{E180}'
    */

```

```rexx
-- Example 7
-- Grapheme supplier providing the byte index and returning the error message
.RexxUnicodeReverseGraphemeSupplier~new("E1 80 E2 F0 91 92 F1 BF 41"x, "b", "e")==
    /*
    a RexxUnicodeReverseGraphemeSupplier 
     9 : ''
    -7 : 'start byte position 7 : Invalid continuation byte 65 (''41''x) at byte position 9'
    -4 : 'start byte position 4 : Invalid continuation byte 241 (''F1''x) at byte position 7'
    -3 : 'start byte position 3 : Invalid continuation byte 240 (''F0''x) at byte position 4'
    -1 : 'start byte position 1 : Invalid continuation byte 226 (''E2''x) at byte position 3'
    */

```

### 6.1.   Class methods

The `RexxUnicodeReverseGraphemeSupplier` class defines no class methods of its own.


### 6.2.   Instance methods

<a id="RexxUnicodeReverseGraphemeSupplier_available"></a>

#### 6.2.1.   available

    aRexxUnicodeReverseGraphemeSupplier~available

Returns `.true` if a grapheme is available from the supplier (that is, if the `item` method would return a value).  
Returns `.false` if the supplier has already enumerated all graphemes.


<a id="RexxUnicodeReverseGraphemeSupplier_graphemeAtIndexG"></a>

#### 6.2.2.   graphemeAtIndexG

    aRexxUnicodeReverseGraphemeSupplier~graphemeAtIndexG(indexG)

Convenience method.  
Advances the supplier to grapheme index `indexG` and returns the corresponding grapheme as a string.  
Here, "advance" refers to moving the supplier cursor, not the direction in the original string.  
If `indexG` is less than the current grapheme index, the supplier raises an error (can only advance).  
This method does not support negative indexes (counting from the beginning of the string).

See also [`aRexxUnicodeReverseGraphemeSupplier~graphemeAtIndexG`](#RexxUnicodeReverseGraphemeSupplier_graphemeAtIndexG).


<a id="RexxUnicodeReverseGraphemeSupplier_index"></a>

#### 6.2.3.   index

    aRexxUnicodeReverseGraphemeSupplier~index(type=defaultIndexType)

Returns the index of the current grapheme in the string.  
If no grapheme is available, that is, if `available` would return `.false`, the supplier raises an error.

The following index types are supported:

- If `type`is `""`, the grapheme index is returned.
- If `type` is `"b"`, the byte index is returned (it is negative if the byte sequence is invalid).

The default index type is specified when the supplier is created.

Byte indexes are counted from the beginning of the string.  
Grapheme indexes are counted from the end of the string.


<a id="RexxUnicodeReverseGraphemeSupplier_init"></a>

#### 6.2.4.   init

    aRexxUnicodeReverseGraphemeSupplier~init(string, defaultIndexType="", defaultItemType="")

Initializes a `RexxUnicodeReverseGraphemeSupplier` instance with the UTF-8 string `string`.

If specified, `defaultIndexType` defines the default index type returned by the `index` method.  
The default is `""` (grapheme index).  
See the [`index`](#RexxUnicodeReverseGraphemeSupplier_index) method for the list of possible values.

If specified, `defaultItemType` defines the default item type returned by the `item` method.  
The default value is `""` (grapheme as a string).  
See the [`item`](#RexxUnicodeReverseGraphemeSupplier_item) method for the list of possible values.


<a id="RexxUnicodeReverseGraphemeSupplier_item"></a>

#### 6.2.5.   item

    aRexxUnicodeReverseGraphemeSupplier~item(type=defaultItemType)

Returns the current grapheme in the string.  
If no grapheme is available, that is, if `available` would return `.false`, the supplier raises an error.

The following item types are supported:

- If `type` is `""`, the grapheme is returned as a string.
- If `type` is `"b"`, the grapheme byte sequence is returned as a string (same as `""`).
- If `type` is `"e"`, the error message is returned (an empty string if there is no error).
- If `type` is `"u"`, the grapheme is returned in `U+` notation.
- If `type` is `"x"`, the grapheme byte sequence is returned as hexadecimal digits.
- If `type` is `"\"`, the grapheme byte sequence is returned as an escaped string.
- If `type` is a class, a new instance of that class is returned, initialized with the result of `item("")`.

The default item type is specified when the supplier is created.


<a id="RexxUnicodeReverseGraphemeSupplier_next"></a>

#### 6.2.6.   next

    aRexxUnicodeReverseGraphemeSupplier~next

Moves to the next grapheme in the string, in reverse order.  
By repeatedly sending `next` to the supplier (as long as `available` returns 
`.true`), you can enumerate all graphemes in the string, in reverse order.  
If no grapheme is available, that is, if `available` would return `.false`, the
supplier raises an error.




<!----------------------------------------------------------------------------->

<a id="RexxUnicodeStringIndexer"></a>

## 7.   RexxUnicodeStringIndexer Class

<!----------------------------------------------------------------------------->

A `RexxUnicodeStringIndexer` instance provides direct access to codepoints and graphemes in a UTF-8 string.

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
     1 : 'start byte position 1 : Invalid continuation byte 145 (''91''x) at byte position 2 (code point > U+10FFFF)'
     2 : 'start byte position 2 : Invalid start byte 145 (''91''x) (non-shortest form)'
     3 : 'start byte position 3 : Invalid start byte 146 (''92''x) (non-shortest form)'
     4 : 'start byte position 4 : Invalid start byte 147 (''93''x) (non-shortest form)'
     5 : 'start byte position 5 : Invalid start byte 255 (''FF''x) (code point > U+10FFFF)'
     6 : 'start byte position 7 : Invalid start byte 128 (''80''x) (non-shortest form)'
     7 : 'start byte position 8 : Invalid start byte 191 (''BF''x) (non-shortest form)'
    */
indexer~codepointAtIndexC(3)=       -- 65533
.RexxUnicodeCharacter~new(65533)=   -- ("�" \x{EFBFBD} U+FFFD So Other_Symbol "REPLACEMENT CHARACTER")
indexer~graphemeAtIndexG(3)~c2x=    -- 92 (use ~c2x to not display an invalid byte sequence)

```


### 7.1.   Routines

<a id="requestRexxUnicodeStringIndexer"></a>

#### 7.1.1.   requestRexxUnicodeStringIndexer routine

    requestRexxUnicodeStringIndexer(object)

Returns a `RexxUnicodeStringIndexer` instance created from `object`.
If `object` is already a `RexxUnicodeStringIndexer` instance, returns it unchanged.


### 7.2.   Class methods

The `RexxUnicodeStringIndexer` class defines no class methods of its own.


### 7.3.   Instance methods

<a id="RexxUnicodeStringIndexer_codepointAtIndexC"></a>

#### 7.3.1.   codepointAtIndexC

    aRexxUnicodeStringIndexer~codepointAtIndexC(indexC)

Returns the codepoint at codepoint index `indexC` as a whole number.

`indexC` must be a positive or negative whole number.  
If `indexC` is negative, the codepoint index is counted from the end of the string.

If `abs(indexC)` exceeds the codepoint count, or if the requested index is not stored in the indexer, an error is raised.

For an explanation of storage, see [`aRexxUnicodeStringIndexer~init`](#RexxUnicodeStringIndexer_init).


<a id="RexxUnicodeStringIndexer_codepointCount"></a>

#### 7.3.2.   codepointCount

    aRexxUnicodeStringIndexer~codepointCount

Returns the codepoint count calculated during the full scan performed when the indexer was initialized.  
This result is not impacted by `codepointStorageLimit`.


<a id="RexxUnicodeStringIndexer_codepointIndexB"></a>

#### 7.3.3.   codepointIndexB

    aRexxUnicodeStringIndexer~codepointIndexB(indexC)

Returns the byte index corresponding to codepoint index `indexC`.  
It is negative if the codepoint's byte sequence is invalid.

`indexC` must be a positive or negative whole number.  
If `indexC` is negative, the codepoint index is counted from the end of the string.

If `abs(indexC)` exceeds the codepoint count, or if the requested index is not stored in the indexer, an error is raised.

For an explanation of storage, see [`aRexxUnicodeStringIndexer~init`](#RexxUnicodeStringIndexer_init).


<a id="RexxUnicodeStringIndexer_codepointIndexC"></a>

#### 7.3.4.   codepointIndexC

    aRexxUnicodeStringIndexer~codepointIndexC(indexB)

Returns the codepoint index corresponding to byte index `indexB`.  
It is negative if the byte index is not aligned with a codepoint boundary.

`indexB` must be a positive or negative whole number.  
If `indexB` is negative, the byte index is counted from the end of the string.

If `abs(indexB)` exceeds the byte string length, an error is raised.

Performs a binary search.
With sparse storage, a binary search may fail if the requested index is not stored.

For an explanation of storage, see [`aRexxUnicodeStringIndexer~init`](#RexxUnicodeStringIndexer_init).

**Examples**

```rexx
indexer = .RexxUnicodeStringIndexer~new("a👨‍👩‍👧b")
do i = 1 to indexer~string~length; say i~left(2)":" indexer~codepointIndexC(i); end
    /*
    1 : 1
    2 : 2
    3 : -2
    4 : -2
    5 : -2
    6 : 3
    7 : -3
    8 : -3
    9 : 4
    10: -4
    11: -4
    12: -4
    13: 5
    14: -5
    15: -5
    16: 6
    17: -6
    18: -6
    19: -6
    20: 7
    */

```

```rexx
-- Negative byte indexes are supported (negative index means "index from the end")
indexer = .RexxUnicodeStringIndexer~new("a👨‍👩‍👧b")
indexer~codepointIndexC(-1)=    -- 7
indexer~codepointIndexC(-2)=    -- -6
indexer~codepointIndexC(-5)=    -- 6

```

```rexx
-- Binary search for codeppoint indexes cannot work with sparse storage
indexer = .RexxUnicodeStringIndexer~new("a👨‍👩‍👧b", 0, 1, 0, 0, 1)
do i = 1 to indexer~string~length; say i~left(2)":" indexer~codepointIndexC(i); end
    /*
    Codepoint index 4 is not stored. Binary search for byte index 1 requires unlimited storage.
    */

```


<a id="RexxUnicodeStringIndexer_codepointIndexes"></a>

#### 7.3.5.   codepointIndexes

    aRexxUnicodeStringIndexer~codepointIndexes

Returns the array of stored codepoint indexes.

For an explanation of storage, see [`aRexxUnicodeStringIndexer~init`](#RexxUnicodeStringIndexer_init).


<a id="RexxUnicodeStringIndexer_codepointStorageLimit"></a>

#### 7.3.6.   codepointStorageLimit

    aRexxUnicodeStringIndexer~codepointStorageLimit

Returns the maximum number of codepoint indexes that can be stored in `codepointIndexes`.

For an explanation of storage, see [`aRexxUnicodeStringIndexer~init`](#RexxUnicodeStringIndexer_init).


<a id="RexxUnicodeStringIndexer_endCodepointIndexes"></a>

#### 7.3.7.   endCodepointIndexes

    aRexxUnicodeStringIndexer~endCodepointIndexes

Returns the array of codepoint indexes stored in the end storage, or `.nil` if `endCodepointStorageSize` is `0`.

Codepoint indexes are stored in reverse order: the first item is the last codepoint index.

For an explanation of storage, see [`aRexxUnicodeStringIndexer~init`](#RexxUnicodeStringIndexer_init).

**Example**

```rexx
indexer = .RexxUnicodeStringIndexer~new("a👨‍👩‍👧b", 5, 0, 0, 5)
indexer~codepointIndexes=       -- [ 1, 2, 6, 9, 13]
indexer~endCodepointIndexes=    -- [ 20, 16, 13, 9, 6]

```


<a id="RexxUnicodeStringIndexer_endCodepointStorageSize"></a>

#### 7.3.8.   endCodepointStorageSize

    aRexxUnicodeStringIndexer~endCodepointStorageSize

Returns the maximum number of codepoint indexes that can be stored in `endCodepointIndexes`.

For an explanation of storage, see [`aRexxUnicodeStringIndexer~init`](#RexxUnicodeStringIndexer_init).


<a id="RexxUnicodeStringIndexer_endErrors"></a>

#### 7.3.9.   endErrors

    aRexxUnicodeStringIndexer~endErrors

Returns the array of error messages stored in the end storage, or `.nil` if `endErrorStorageSize` is `0`.

Error messages are stored in reverse order: the first item corresponds to the last error encountered.

For an explanation of storage, see [`aRexxUnicodeStringIndexer~init`](#RexxUnicodeStringIndexer_init).

**Example**

```rexx
-- U+FFFD Substitution of Maximal Subparts
-- https://www.unicode.org/versions/Unicode17.0.0/core-spec/chapter-3/#G68064
indexer = .RexxUnicodeStringIndexer~new("F4 91 92 93 FF 41 80 BF 42"x, 0, 0, 5, 0, 0, 5)
indexer~errors==
    /*
    an Array (shape [5], 5 items)
     1 : 'start byte position 1 : Invalid continuation byte 145 (''91''x) at byte position 2 (code point > U+10FFFF)'
     2 : 'start byte position 2 : Invalid start byte 145 (''91''x) (non-shortest form)'
     3 : 'start byte position 3 : Invalid start byte 146 (''92''x) (non-shortest form)'
     4 : 'start byte position 4 : Invalid start byte 147 (''93''x) (non-shortest form)'
     5 : 'start byte position 5 : Invalid start byte 255 (''FF''x) (code point > U+10FFFF)'
    */

indexer~endErrors==
    /*
    an Array (shape [5], 5 items)
     1 : 'start byte position 8 : Invalid start byte 191 (''BF''x) (non-shortest form)'
     2 : 'start byte position 7 : Invalid start byte 128 (''80''x) (non-shortest form)'
     3 : 'start byte position 5 : Invalid start byte 255 (''FF''x) (code point > U+10FFFF)'
     4 : 'start byte position 4 : Invalid start byte 147 (''93''x) (non-shortest form)'
     5 : 'start byte position 3 : Invalid start byte 146 (''92''x) (non-shortest form)'
    */

```


<a id="RexxUnicodeStringIndexer_endErrorStorageSize"></a>

#### 7.3.10.   endErrorStorageSize

    aRexxUnicodeStringIndexer~endErrorStorageSize

Returns the maximum number of error messages that can be stored in `endErrors`.

For an explanation of storage, see [`aRexxUnicodeStringIndexer~init`](#RexxUnicodeStringIndexer_init).


<a id="RexxUnicodeStringIndexer_endGraphemeIndexes"></a>

#### 7.3.11.   endGraphemeIndexes

    aRexxUnicodeStringIndexer~endGraphemeIndexes

Returns the array of grapheme indexes stored in the end storage, or `.nil` if `endGraphemeStorageSize` is `0`.

Grapheme indexes are stored in reverse order: the first item is the last grapheme index.

For an explanation of storage, see [`aRexxUnicodeStringIndexer~init`](#RexxUnicodeStringIndexer_init).

**Example**

```rexx
indexer = .RexxUnicodeStringIndexer~new("a👨‍👩‍👧b", 0, 5, 0, 0, 5)
indexer~graphemeIndexes=        -- [ 1, 2, 20]
indexer~endGraphemeIndexes=     -- [ 20, 2, 1]

```


<a id="RexxUnicodeStringIndexer_endGraphemeStorageSize"></a>

#### 7.3.12.   endGraphemeStorageSize

    aRexxUnicodeStringIndexer~endGraphemeStorageSize

Returns the maximum number of grapheme indexes that can be stored in `endGraphemeIndexes`.

For an explanation of storage, see [`aRexxUnicodeStringIndexer~init`](#RexxUnicodeStringIndexer_init).


<a id="RexxUnicodeStringIndexer_errorCount"></a>

#### 7.3.13.   errorCount

    aRexxUnicodeStringIndexer~errorCount

Returns the error count calculated during the full scan performed when the indexer was initialized.

This result is not impacted by `errorStorageLimit`.


<a id="RexxUnicodeStringIndexer_errors"></a>

#### 7.3.14.   errors

    aRexxUnicodeStringIndexer~errors

Returns the array of stored error messages.

For an explanation of storage, see [`aRexxUnicodeStringIndexer~init`](#RexxUnicodeStringIndexer_init).


<a id="RexxUnicodeStringIndexer_errorStorageLimit"></a>

#### 7.3.15.   errorStorageLimit

    aRexxUnicodeStringIndexer~errorStorageLimit

Returns the maximum number of error messages that can be stored in `errors`.

For an explanation of storage, see [`aRexxUnicodeStringIndexer~init`](#RexxUnicodeStringIndexer_init).


<a id="RexxUnicodeStringIndexer_graphemeAtIndexG"></a>

#### 7.3.16.   graphemeAtIndexG

    aRexxUnicodeStringIndexer~graphemeAtIndexG(indexG)

Returns the grapheme at grapheme index `indexG` as a string.

`indexG` must be a positive or negative whole number.  
If `indexG` is negative, the grapheme index is counted from the end of the string.

If `abs(indexG)` exceeds the grapheme count, or if the requested index is not stored in the indexer, an error is raised.

For an explanation of storage, see [`aRexxUnicodeStringIndexer~init`](#RexxUnicodeStringIndexer_init).


<a id="RexxUnicodeStringIndexer_graphemeCount"></a>

#### 7.3.17.   graphemeCount

    aRexxUnicodeStringIndexer~graphemeCount

Returns the grapheme count calculated during the full scan performed when the indexer was initialized.

This result is not affected by `graphemeStorageLimit`.


<a id="RexxUnicodeStringIndexer_graphemeIndexB"></a>

#### 7.3.18.   graphemeIndexB

    aRexxUnicodeStringIndexer~graphemeIndexB(indexG)

Returns the byte index corresponding to grapheme index `indexG`.  
It is negative if the grapheme's byte sequence is invalid.

`indexG` must be a positive or negative whole number.  
If `indexG` is negative, the grapheme index is counted from the end of the string.

If `abs(indexG)` exceeds the grapheme count, or if the requested index is not stored in the indexer, an error is raised.

For an explanation of storage, see [`aRexxUnicodeStringIndexer~init`](#RexxUnicodeStringIndexer_init).


<a id="RexxUnicodeStringIndexer_graphemeIndexG"></a>

#### 7.3.19.   graphemeIndexG

    aRexxUnicodeStringIndexer~graphemeIndexG(indexB)

Returns the grapheme index corresponding to byte index `indexB`.  
It is negative if the byte index is not aligned with a grapheme boundary.

`indexB` must be a positive or negative whole number.  
If `indexB` is negative, the byte index is counted from the end of the string.

If `abs(indexB)` exceeds the byte string length, an error is raised.

Performs a binary search.
With sparse storage, a binary search may fail if the requested index is not stored.

For an explanation of storage, see [`aRexxUnicodeStringIndexer~init`](#RexxUnicodeStringIndexer_init).

**Examples**

```rexx
indexer = .RexxUnicodeStringIndexer~new("a👨‍👩‍👧b")
do i = 1 to indexer~string~length; say i~left(2)":" indexer~graphemeIndexG(i); end
    /*
    1 : 1
    2 : 2
    3 : -2
    4 : -2
    5 : -2
    6 : -2
    7 : -2
    8 : -2
    9 : -2
    10: -2
    11: -2
    12: -2
    13: -2
    14: -2
    15: -2
    16: -2
    17: -2
    18: -2
    19: -2
    20: 3
    */

```

```rexx
-- Negative byte indexes are supported (negative index means "index from the end")
indexer = .RexxUnicodeStringIndexer~new("a👨‍👩‍👧b")
indexer~graphemeIndexG(-1)=    -- 3
indexer~graphemeIndexG(-2)=    -- -2
indexer~graphemeIndexG(-19)=   -- 2

```

```rexx
-- Binary search for grapheme indexes cannot work with sparse storage
indexer = .RexxUnicodeStringIndexer~new("a👨‍👩‍👧b", 0, 1, 0, 0, 1)
do i = 1 to indexer~string~length; say i~left(2)":" indexer~graphemeIndexG(i); end
    /*
    Grapheme index 2 is not stored. Binary search for byte index 1 requires unlimited storage.
    */

```


<a id="RexxUnicodeStringIndexer_graphemeIndexes"></a>

#### 7.3.20.   graphemeIndexes

    aRexxUnicodeStringIndexer~graphemeIndexes

Returns the array of stored grapheme indexes.

For an explanation of storage, see [`aRexxUnicodeStringIndexer~init`](#RexxUnicodeStringIndexer_init).


<a id="RexxUnicodeStringIndexer_graphemeStorageLimit"></a>

#### 7.3.21.   graphemeStorageLimit

    aRexxUnicodeStringIndexer~graphemeStorageLimit

Returns the maximum number of grapheme indexes that can be stored in `graphemeIndexes`.

For an explanation of storage, see [`aRexxUnicodeStringIndexer~init`](#RexxUnicodeStringIndexer_init).


<a id="RexxUnicodeStringIndexer_info"></a>

#### 7.3.22.   info

    aRexxUnicodeStringIndexer~info

Returns a string providing information about the indexer's string:

- Whether it is ASCII or non-ASCII
- Grapheme count
- Codepoint count
- Byte count
- Error count

**Examples:**

```rexx
.RexxUnicodeStringIndexer~new("")~info=            -- '(ASCII, 0 grapheme, 0 codepoint, 0 byte, 0 error)'
.RexxUnicodeStringIndexer~new("e")~info=           -- '(ASCII, 1 grapheme, 1 codepoint, 1 byte, 0 error)'
.RexxUnicodeStringIndexer~new("é")~info=           -- '(not-ASCII, 1 grapheme, 1 codepoint, 2 bytes, 0 error)'
.RexxUnicodeStringIndexer~new("€")~info=           -- '(not-ASCII, 1 grapheme, 1 codepoint, 3 bytes, 0 error)'
.RexxUnicodeStringIndexer~new("🎅")~info=          -- '(not-ASCII, 1 grapheme, 1 codepoint, 4 bytes, 0 error)'
.RexxUnicodeStringIndexer~new("👨‍👩‍👧")~info=          -- '(not-ASCII, 1 grapheme, 5 codepoints, 18 bytes, 0 error)'

```

```rexx
-- Invalid string
-- U+FFFD Substitution of Maximal Subparts
-- https://www.unicode.org/versions/Unicode17.0.0/core-spec/chapter-3/#G68202
.RexxUnicodeStringIndexer~new("E1 80 E2 F0 91 92 F1 BF 41"x)~info=    -- '(not-ASCII, 5 graphemes, 5 codepoints, 9 bytes, 4 errors)'

```


<a id="RexxUnicodeStringIndexer_init"></a>

#### 7.3.23.   init

```
aRexxUnicodeStringIndexer~init(
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

Initializes a `RexxUnicodeStringIndexer` instance with the UTF-8 string `string`.

Storage limits can be specified when creating the indexer; by default, no limit is applied:

- `codepointStorageLimit` determines how many codepoint indexes can be stored in the array `codepointIndexes`.
- `graphemeStorageLimit` determines how many grapheme indexes can be stored in the array `graphemeIndexes`.
- `errorStorageLimit` determines how many error messages can be stored in the array `errors`.

End storage sizes can be specified when creating the indexer; by default, no end storage is allocated (0).

- `endCodepointStorageSize` determines how many codepoint indexes can be stored in the array `endCodepointIndexes`.
- `endGraphemeStorageSize` determines how many grapheme indexes can be stored in the array `endGraphemeIndexes`.
- `endErrorStorageSize` determines how many error messages can be stored in the array `endErrors`.

The storage limits and end storage sizes allow fine-tuning of memory usage.

End storage is useful when storage is limited;
it allows sparse storage, where only the starting and ending indexes are stored.

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
     1 : 'start byte position 1 : Invalid continuation byte 145 (''91''x) at byte position 2 (code point > U+10FFFF)'
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
indexer~errors[1]=          -- 'start byte position 1 : Invalid continuation byte 145 (''91''x) at byte position 2 (code point > U+10FFFF)'
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


<a id="RexxUnicodeStringIndexer_string"></a>

#### 7.3.24.   string

    aRexxUnicodeStringIndexer~string

Returns the string passed when creating the indexer.




<!----------------------------------------------------------------------------->

<a id="RexxUnicodeCharacter"></a>

## 8.   RexxUnicodeCharacter Class

<!----------------------------------------------------------------------------->

A RexxUnicodeCharacter instance is created from a codepoint and provides access 
to the properties of the corresponding Unicode character.

The special value -1 can be represented using a RexxUnicodeCharacter.

The class `RexxUnicodeCharacter` inherits the `Comparable` mixin class
and implements a `compareTo` method. Instances are compared by codepoint value.

`::requires "rxunicode.cls"`


### 8.1.   Class methods

<a id="RexxUnicodeCharacter_properties"></a>

#### 8.1.1.   properties

    .RexxUnicodeCharacter~properties

Returns an array of property names.

These property names can be used as message names sent to a `RexxUnicodeCharacter` instance to get the corresponding property values.

The `RexxUnicodeCharacter~info(codepoint)` method uses these property names to build a table of property-value.

**Examples**

```rexx
.RexxUnicodeCharacter~properties==
    /*
    an Array (shape [29], 26 items)
     1  : 'bidiClass'
     2  : 'bidiClassName'
     3  : 'bidiMirrored'
     4  : 'boundClass'
     5  : 'boundClassName'
     6  : 'category'
     7  : 'categoryName'
     8  : 'charWidth'
     9  : 'codepoint'
     10 : 'combiningClass'
     11 : 'combiningClassName'
     12 : 'combiningClassValue'
     13 : 'controlBoundary'
     14 : 'decompositionTypeName'
     15 : 'decompositionType'
     16 : 'eastAsianWidthIsAmbiguous'
     17 : 'extendedName'
     18 : 'ignorable'
     19 : 'isLower'
     20 : 'isUpper'
     21 : 'name'
     22 : 'nameAlias'
     23 : 'toLower'
     25 : 'toTitle'
     27 : 'toUpper'
     29 : 'UTF8'
    */

```
 
```rexx
c = .RexxUnicode~character("€"); do message over .RexxUnicodeCharacter~properties; say message":" c~send(message); end
    /*
    bidiClass: ET
    bidiClassName: European_Terminator
    bidiMirrored: 0
    boundClass: XX
    boundClassName: Other
    category: Sc
    categoryName: Currency_Symbol
    charWidth: 1
    codepoint: 8364
    combiningClass: NR
    combiningClassName: Not_Reordered
    combiningClassValue: 0
    controlBoundary: 0
    decompositionTypeName: Code point not explicitly listed for Decomposition_Type
    decompositionType: None
    eastAsianWidthIsAmbiguous: 1
    extendedName: EURO SIGN
    ignorable: 0
    isLower: 0
    isUpper: 0
    name: EURO SIGN
    nameAlias: 
    toLower: 8364
    toTitle: 8364
    toUpper: 8364
    UTF8: €
    */

```
 
 
### 8.2.   Instance methods

<a id="RexxUnicodeCharacter_bidiClass"></a>

#### 8.2.1.   bidiClass

    aRexxUnicodeCharacter~bidiClass

Returns the [bidirectional character type][unicode_standard_annex_9_bidirectional_character_types] as its short enumeration name.

See [`.RexxUnicodeServices~codepointBidiClass`](#codepointBidiClass) for the list of enumeration values.

**Examples:**

```rexx
.RexxUnicodeCharacter~new("0608"~x2d)~bidiClass=        --  'AL'
.RexxUnicodeCharacter~new("FFFFFF"~x2d)~bidiClass=      --  Invalid code point: 16777215; allowed range is 0 to 1114111 (U+10FFFF).
.RexxUnicodeCharacter~new(-1)~bidiClass=                --  '?'

```


<a id="RexxUnicodeCharacter_bidiClassName"></a>

#### 8.2.2.   bidiClassName

    aRexxUnicodeCharacter~bidiClassName

Returns the [bidirectional character type][unicode_standard_annex_9_bidirectional_character_types] as its long enumeration name.

See [`.RexxUnicodeServices~codepointBidiClass`](#codepointBidiClass) for the list of enumeration values.

**Examples:**

```rexx
.RexxUnicodeCharacter~new("0608"~x2d)~bidiClassName=        --  'Arabic_Letter'
.RexxUnicodeCharacter~new("FFFFFF"~x2d)~bidiClassName=      --  Invalid code point: 16777215; allowed range is 0 to 1114111 (U+10FFFF).
.RexxUnicodeCharacter~new(-1)~bidiClassName=                --  '?'

```


<a id="RexxUnicodeCharacter_bidiMirrored"></a>

#### 8.2.3.   bidiMirrored

    aRexxUnicodeCharacter~bidiMirrored

Returns `.true` if the `Bidi_Mirrored` property value is `Yes`.

[https://unicode.org/reports/tr9/#Mirroring][unicode_standard_annex_9_mirroring]


<a id="RexxUnicodeCharacter_boundClass"></a>

#### 8.2.4.   boundClass

    aRexxUnicodeCharacter~boundClass

Returns the `Grapheme_Cluster_Break` property value as its short enumeration name.

[https://www.unicode.org/reports/tr29/#Grapheme_Cluster_Boundaries][unicode_standard_annex_29_grapheme_cluster_boundaries]

See [`.RexxUnicodeServices~codepointBoundClass`](#codepointBoundClass) for the list of enumeration values.

**Examples:**

```rexx
.RexxUnicodeCharacter~new("1F1E6"~x2d)~boundClass=      --  'RI'
.RexxUnicodeCharacter~new("FFFFFF"~x2d)~boundClass=     --  Invalid code point: 16777215; allowed range is 0 to 1114111 (U+10FFFF).
.RexxUnicodeCharacter~new(-1)~boundClass=               --  '?'

```


<a id="RexxUnicodeCharacter_boundClassName"></a>

#### 8.2.5.   boundClassName

    aRexxUnicodeCharacter~boundClassName

Returns the `Grapheme_Cluster_Break` property value as its long enumeration name.

[https://www.unicode.org/reports/tr29/#Grapheme_Cluster_Boundaries][unicode_standard_annex_29_grapheme_cluster_boundaries]

See [`.RexxUnicodeServices~codepointBoundClass`](#codepointBoundClass) for the list of enumeration values.

**Examples:**

```rexx
.RexxUnicodeCharacter~new("1F1E6"~x2d)~boundClassName=      --  'Regional_Indicator'
.RexxUnicodeCharacter~new("FFFFFF"~x2d)~boundClassName=     --  Invalid code point: 16777215; allowed range is 0 to 1114111 (U+10FFFF).
.RexxUnicodeCharacter~new(-1)~boundClassName=               --  '?'

```


<a id="RexxUnicodeCharacter_category"></a>

#### 8.2.6.   category

    aRexxUnicodeCharacter~category

Returns the `General_Category` property value as its short enumeration name.

[https://www.unicode.org/versions/Unicode17.0.0/core-spec/chapter-4/#G124142][unicode_core_spec_general_category]

See [`.RexxUnicodeServices~codepointCategory`](#codepointCategory) for the list of enumeration values.

**Examples:**

```rexx
.RexxUnicodeCharacter~new("0903"~x2d)~category=     -- 'Mc'
.RexxUnicodeCharacter~new("FFFFFF"~x2d)~category=   --  Invalid code point: 16777215; allowed range is 0 to 1114111 (U+10FFFF).
.RexxUnicodeCharacter~new(-1)~category=             --  'Cn'

```


<a id="RexxUnicodeCharacter_categoryName"></a>

#### 8.2.7.   categoryName

    aRexxUnicodeCharacter~categoryName

Returns the `General_Category` property value as its long enumeration name.

[https://www.unicode.org/versions/Unicode17.0.0/core-spec/chapter-4/#G124142][unicode_core_spec_general_category]

See [`.RexxUnicodeServices~codepointCategory`](#codepointCategory) for the list of enumeration values.

**Examples:**

```rexx
.RexxUnicodeCharacter~new("0903"~x2d)~categoryName=     -- 'Spacing_Mark'
.RexxUnicodeCharacter~new("FFFFFF"~x2d)~categoryName=   --  Invalid code point: 16777215; allowed range is 0 to 1114111 (U+10FFFF).
.RexxUnicodeCharacter~new(-1)~categoryName=             --  'Unassigned'

```


<a id="RexxUnicodeCharacter_charWidth"></a>

#### 8.2.8.   charWidth

    aRexxUnicodeCharacter~charWidth

Returns a character width analogous to `wcwidth(self~codepoint)`,
except that a width of 0 is returned for non-printable codepoints
instead of -1 as in `wcwidth`.


<a id="RexxUnicodeCharacter_codepoint"></a>

#### 8.2.9.   codepoint

    aRexxUnicodeCharacter~codepoint

Returns the codepoint passed when creating the `RexxUnicodeCharacter` instance.


<a id="RexxUnicodeCharacter_combiningClass"></a>

#### 8.2.10.   combiningClass

    aRexxUnicodeCharacter~combiningClass

Returns the `Canonical_Combining_Class` property value as its short enumeration name.

[Combining classes][unicode_core_spec_combining_classes] in the
`Canonical_Combining_Class` field of `UnicodeData.txt` are
[numeric values][unicode_standard_annex_44_canonical_combining_class_values]
used by the Canonical Ordering Algorithm.  
Some values do not have a short name. In such cases, an empty string is returned.

See [`.RexxUnicodeServices~codepointCombiningClass`](#codepointCombiningClass) for the list of enumeration values.

**Examples:**

```rexx
.RexxUnicodeCharacter~new("031B"~x2d)~combiningClass=       --  'ATAR'
.RexxUnicodeCharacter~new("FFFFFF"~x2d)~combiningClass=     --  Invalid code point: 16777215; allowed range is 0 to 1114111 (U+10FFFF).
.RexxUnicodeCharacter~new(-1)~combiningClass=               --  '?'

```


<a id="RexxUnicodeCharacter_combiningClassName"></a>

#### 8.2.11.   combiningClassName

    aRexxUnicodeCharacter~combiningClassName

Returns the `Canonical_Combining_Class` property value as its long enumeration name.

[Combining classes][unicode_core_spec_combining_classes] in the
`Canonical_Combining_Class` field of `UnicodeData.txt` are
[numeric values][unicode_standard_annex_44_canonical_combining_class_values]
used by the Canonical Ordering Algorithm.  
Some values do not have a long name. In such cases, an empty string is returned.

See [`.RexxUnicodeServices~codepointCombiningClass`](#codepointCombiningClass) for the list of enumeration values.

**Examples:**

```rexx
.RexxUnicodeCharacter~new("031B"~x2d)~combiningClassName=       --  'Attached_Above_Right'
.RexxUnicodeCharacter~new("FFFFFF"~x2d)~combiningClassName=     --  Invalid code point: 16777215; allowed range is 0 to 1114111 (U+10FFFF).
.RexxUnicodeCharacter~new(-1)~combiningClassName=               --  '?'

```


<a id="RexxUnicodeCharacter_combiningClassValue"></a>

#### 8.2.12.   combiningClassValue

    aRexxUnicodeCharacter~combiningClassValue

Returns the `Canonical_Combining_Class` property value as a whole number.

[Combining classes][unicode_core_spec_combining_classes] in the
`Canonical_Combining_Class` field of `UnicodeData.txt` are
[numeric values][unicode_standard_annex_44_canonical_combining_class_values]
used by the Canonical Ordering Algorithm.  
The numeric values from `0` to `254` are standard Unicode values.  

See [`.RexxUnicodeServices~codepointCombiningClass`](#codepointCombiningClass) for the list of enumeration values.

**Examples:**

```rexx
.RexxUnicodeCharacter~new("031B"~x2d)~combiningClassValue=      --  216
.RexxUnicodeCharacter~new("FFFFFF"~x2d)~combiningClassValue=    --  Invalid code point: 16777215; allowed range is 0 to 1114111 (U+10FFFF).
.RexxUnicodeCharacter~new(-1)~combiningClassValue=              --  -1

```

<a id="RexxUnicodeCharacter_compareTo"></a>

#### 8.2.13.   compareTo

    aRexxUnicodeCharacter~compareTo(otherUnicodeCharacter)

Compares the target Unicode character with `otherUnicodeCharacter`.  
Instances are compared by codepoint value.

If the two codepoints are equal, 0 is returned.  
If the target codepoint is greater, 1 is returned.  
If the target codepoint is less, -1 is returned.

**Examples**

```rexx
c1 = .RexxUnicodeCharacter~new(64); c2 = .RexxUnicodeCharacter~new(64); c1~compareTo(c2)=   -- 0
c1 = .RexxUnicodeCharacter~new(65); c2 = .RexxUnicodeCharacter~new(64); c1~compareTo(c2)=   -- 1
c1 = .RexxUnicodeCharacter~new(64); c2 = .RexxUnicodeCharacter~new(65); c1~compareTo(c2)=   -- -1

```


<a id="RexxUnicodeCharacter_controlBoundary"></a>

#### 8.2.14.   controlBoundary

    aRexxUnicodeCharacter~controlBoundary

Returns `.true` if the character's codepoint belongs to the `Zl`, `Zp`, `Cc`, or `Cf` category, except for:

        (<?> \x{E2808C} U+200C Cf Format "ZERO WIDTH NON-JOINER")
        (<?> \x{E2808D} U+200D Cf Format "ZERO WIDTH JOINER")

> [!WARNING]  
> This rule is not used internally by `utf8proc`
> and does not appear in [Unicode Standard Annex #29][unicode_standard_annex_29].
> Therefore, this method may be deprecated in the future unless a use case is identified.


<a id="RexxUnicodeCharacter_decompositionType"></a>

#### 8.2.15.   decompositionType

    aRexxUnicodeCharacter~decompositionType

Returns the `Decomposition_Type` property value as its short enumeration name.

[https://unicode.org/reports/tr15/][unicode_standard_annex_15]

See [`.RexxUnicodeServices~codepointDecompositionType`](#codepointDecompositionType) for the list of enumeration values.

**Examples:**

```rexx
.RexxUnicodeCharacter~new("0000"~x2d)~decompositionType=        --  'None'
.RexxUnicodeCharacter~new("00A8"~x2d)~decompositionType=        --  'Compat'
.RexxUnicodeCharacter~new("00BC"~x2d)~decompositionType=        --  'Fraction'
.RexxUnicodeCharacter~new("FFFFFF"~x2d)~decompositionType=      --  Invalid code point: 16777215; allowed range is 0 to 1114111 (U+10FFFF).
.RexxUnicodeCharacter~new(-1)~decompositionType=                --  '?'

```


<a id="RexxUnicodeCharacter_decompositionTypeName"></a>

#### 8.2.16.   decompositionTypeName

    aRexxUnicodeCharacter~decompositionTypeName

Returns the `Decomposition_Type` property value as its long enumeration name.

[https://unicode.org/reports/tr15/][unicode_standard_annex_15]

See [`.RexxUnicodeServices~codepointDecompositionType`](#codepointDecompositionType) for the list of enumeration values.

**Examples:**

```rexx
.RexxUnicodeCharacter~new("0000"~x2d)~decompositionTypeName=        --  'Code point not explicitly listed for Decomposition_Type'
.RexxUnicodeCharacter~new("00A8"~x2d)~decompositionTypeName=        --  'Unspecified compatibility character'
.RexxUnicodeCharacter~new("00BC"~x2d)~decompositionTypeName=        --  'Vulgar fraction form'
.RexxUnicodeCharacter~new("FFFFFF"~x2d)~decompositionTypeName=      --  Invalid code point: 16777215; allowed range is 0 to 1114111 (U+10FFFF).
.RexxUnicodeCharacter~new(-1)~decompositionTypeName=                --  '?'

```


<a id="RexxUnicodeCharacter_eastAsianWidthIsAmbiguous"></a>

#### 8.2.17.   eastAsianWidthIsAmbiguous

    aRexxUnicodeCharacter~eastAsianWidthIsAmbiguous

Returns `.true` if the `East_Asian_Width` property value is `"A"` (`"Ambiguous"`).

[https://www.unicode.org/reports/tr11/][unicode_standard_annex_11]


<a id="RexxUnicodeCharacter_extendedName"></a>

#### 8.2.18.   extendedName

    aRexxUnicodeCharacter~extendedName

Returns the Unicode extended name, or an empty string if the `ICU4ooRexx` class is not loaded.

All Unicode characters have an extended name.  
An extended name is either the standard name if defined, or a codepoint label algorithmically generated.

**Examples:**

```rexx
.RexxUnicodeCharacter~new(2448~x2d)~extendedName=   -- 'OCR DASH'
.RexxUnicodeCharacter~new("D800"~x2d)~extendedName= -- '<lead surrogate-D800>'

```

```rexx
-- The special value -1 has no extended name
.RexxUnicodeCharacter~new(-1)~extendedName=         -- ''

```


<a id="RexxUnicodeCharacter_ignorable"></a>

#### 8.2.19.   ignorable

    aRexxUnicodeCharacter~ignorable

Returns the `Default_Ignorable_Code_Point` property value (boolean value).

[https://www.unicode.org/versions/Unicode17.0.0/core-spec/chapter-5/#G40025][unicode_core_spec_ignoring_characters]


<a id="RexxUnicodeCharacter_indicConjunctBreak"></a>

#### 8.2.20.   indicConjunctBreak

    aRexxUnicodeCharacter~indicConjunctBreak

Returns the `Indic_Conjunct_Break (InCB)` property value as its short enumeration name.

[https://www.unicode.org/reports/tr44/#Derivation_InCB][unicode_standard_annex_44_derivation_incb]

See [`.RexxUnicodeServices~codepointIndicConjunctBreak`](#codepointIndicConjunctBreak) for the list of enumeration values.

**Examples:**

```rexx
.RexxUnicodeCharacter~new("0000"~x2d)~indicConjunctBreak=           --  'None'
.RexxUnicodeCharacter~new("0300"~x2d)~indicConjunctBreak=           --  'Extend'
.RexxUnicodeCharacter~new("0915"~x2d)~indicConjunctBreak=           --  'Consonant'
.RexxUnicodeCharacter~new("094D"~x2d)~indicConjunctBreak=           --  'Linker'
.RexxUnicodeCharacter~new("FFFFFF"~x2d)~indicConjunctBreak=         --  Invalid code point: 16777215; allowed range is 0 to 1114111 (U+10FFFF).
.RexxUnicodeCharacter~new(-1)~indicConjunctBreak=                   --  '?'

```


<a id="RexxUnicodeCharacter_indicConjunctBreakName"></a>

#### 8.2.21.   indicConjunctBreakName

    aRexxUnicodeCharacter~indicConjunctBreakName

Returns the `Indic_Conjunct_Break (InCB)` property value as its long enumeration name.

[https://www.unicode.org/reports/tr44/#Derivation_InCB][unicode_standard_annex_44_derivation_incb]

See [`.RexxUnicodeServices~codepointIndicConjunctBreak`](#codepointIndicConjunctBreak) for the list of enumeration values.

**Examples:**

```rexx
.RexxUnicodeCharacter~new("0000"~x2d)~indicConjunctBreakName=       --  'None'
.RexxUnicodeCharacter~new("0300"~x2d)~indicConjunctBreakName=       --  'Extend'
.RexxUnicodeCharacter~new("0915"~x2d)~indicConjunctBreakName=       --  'Consonant'
.RexxUnicodeCharacter~new("094D"~x2d)~indicConjunctBreakName=       --  'Linker'
.RexxUnicodeCharacter~new("FFFFFF"~x2d)~indicConjunctBreakName=     --  Invalid code point: 16777215; allowed range is 0 to 1114111 (U+10FFFF).
.RexxUnicodeCharacter~new(-1)~indicConjunctBreakName=               --  '?'

```


<a id="RexxUnicodeCharacter_info"></a>

#### 8.2.22.   info

    aRexxUnicodeCharacter~info

Returns a string table whose keys are property names returned by `.RexxUnicodeCharacter~properties`.  
Each property name is used as a message name to retrieve the corresponding property value from the Unicode character.

The following properties have a special representation:

- The values of `codepoint`, `toLower`, `toTitle`, and `toUpper` are returned as `U+XXXX` strings (4 to 6 hexadecimal digits).
- The value of `UTF8` is returned as escaped hexadecimal bytes.

**Example**

```rexx
.RexxUnicode~character("€")~info=
    /*
    a StringTable (26 items)
    'bidiClass'                 : 'ET'
    'bidiClassName'             : 'European_Terminator'
    'bidiMirrored'              :  0
    'boundClass'                : 'XX'
    'boundClassName'            : 'Other'
    'category'                  : 'Sc'
    'categoryName'              : 'Currency_Symbol'
    'charWidth'                 :  1
    'codepoint'                 : 'U+20AC'
    'combiningClass'            : 'NR'
    'combiningClassName'        : 'Not_Reordered'
    'combiningClassValue'       :  0
    'controlBoundary'           :  0
    'decompositionType'         : 'None'
    'decompositionTypeName'     : 'Code point not explicitly listed for Decomposition_Type'
    'eastAsianWidthIsAmbiguous' :  1
    'extendedName'              : 'EURO SIGN'
    'ignorable'                 :  0
    'isLower'                   :  0
    'isUpper'                   :  0
    'name'                      : 'EURO SIGN'
    'nameAlias'                 : ''
    'toLower'                   : 'U+20AC'
    'toTitle'                   : 'U+20AC'
    'toUpper'                   : 'U+20AC'
    'UTF8'                      : '\x{E282AC}'
    */

```


<a id="RexxUnicodeCharacter_init"></a>

#### 8.2.23.   init

    aRexxUnicodeCharacter~init(codepoint)

Initializes a `RexxUnicodeCharacter` instance with the specified `codepoint`.


<a id="RexxUnicodeCharacter_isLower"></a>

#### 8.2.24.   isLower

    aRexxUnicodeCharacter~isLower

Returns `.true` if the Unicode character is a lowercase character
and `.false` otherwise.


<a id="RexxUnicodeCharacter_isUpper"></a>

#### 8.2.25.   isUpper

    aRexxUnicodeCharacter~isUpper

Returns `.true` if the Unicode character is an uppercase character
and `.false` otherwise.


<a id="RexxUnicodeCharacter_name"></a>

#### 8.2.26.   name

    aRexxUnicodeCharacter~name

Returns the Unicode standard name, or an empty string if the character has no Unicode standard name.

This method requires the `ICU4ooRexx` class. If it is not loaded, the method also returns an empty string.

**Examples:**

```rexx
.RexxUnicodeCharacter~new(2448~x2d)~name=   -- 'OCR DASH'
.RexxUnicodeCharacter~new("D800"~x2d)~name= -- ''

```

```rexx
-- The special value -1 has no name
.RexxUnicodeCharacter~new(-1)~name=         -- ''

```


<a id="RexxUnicodeCharacter_nameAlias"></a>

#### 8.2.27.   nameAlias

    aRexxUnicodeCharacter~nameAlias

Returns the Unicode name alias, or an empty string if the character has no Unicode name alias.

This method requires the `ICU4ooRexx` class. If it is not loaded, the method also returns an empty string.

**Examples:**

```rexx
.RexxUnicodeCharacter~new(2448~x2d)~nameAlias=   -- 'MICR ON US SYMBOL'
.RexxUnicodeCharacter~new("D800"~x2d)~nameAlias= -- ''

```

```rexx
-- The special value -1 has no name alias
.RexxUnicodeCharacter~new(-1)~nameAlias=         -- ''

```


<a id="RexxUnicodeCharacter_string"></a>

#### 8.2.28.   string

    aRexxUnicodeCharacter~string

Returns a string representation of the Unicode character.

Fields displayed in the string representation:

- If the character's codepoint is printable, the character's UTF-8 representation is displayed between quotes. Otherwise `<?>` is displayed.
- Displays the escaped hexadecimal bytes of the character's UTF-8 encoding.
- Displays the `U+` representation of the character's codepoint.
- Displays the category short name.
- Displays the category long name.
- Displays the preferred Unicode name (see [`preferredName`](#RexxUnicodeCharacter_preferredName)).

**Examples:**

```rexx
.RexxUnicodeCharacter~new(2448~x2d)~string=     -- '"⑈" \x{E29188} U+2448 So Other_Symbol "OCR DASH"'
.RexxUnicodeCharacter~new("D800"~x2d)~string=   -- '<?> \x{EDA080} U+D800 Cs Surrogate "<lead surrogate-D800>"'

```

```rexx
-- The special codepoint value -1 has its own string representation
.RexxUnicodeCharacter~new(-1)~string=         -- 'An invalid character'

```


<a id="RexxUnicodeCharacter_toLower"></a>

#### 8.2.29.   toLower

    aRexxUnicodeCharacter~toLower

Returns the codepoint of the corresponding lowercase character.  
If the Unicode character has no lowercase mapping, returns its codepoint.

This method uses the simple case mappings defined in `UnicodeData.txt`
and always returns a single codepoint.


<a id="RexxUnicodeCharacter_toLowerFull"></a>

#### 8.2.30.   toLowerFull (not supported)

    aRexxUnicodeCharacter~toLowerFull

Returns the codepoints of the corresponding lowercase mapping as an array.
If the Unicode character has no lowercase mapping, returns an array containing its codepoint.

This method uses the full case mappings defined in `SpecialCasing.txt`
and returns 1 to n codepoints.

Since `utf8proc` does not expose full case mappings, this method returns an array containing -1.


<a id="RexxUnicodeCharacter_toTitle"></a>

#### 8.2.31.   toTitle

    aRexxUnicodeCharacter~toTitle

Returns the codepoint of the corresponding titlecase character.  
If the Unicode character has no titlecase mapping, returns its codepoint.

This method uses the simple case mappings defined in `UnicodeData.txt`
and always returns a single codepoint.


<a id="RexxUnicodeCharacter_toTitleFull"></a>

#### 8.2.32.   toTitleFull (not supported)

    aRexxUnicodeCharacter~toTitleFull

Returns the codepoints of the corresponding titlecase mapping as an array.
If the Unicode character has no titlecase mapping, returns an array containing its codepoint.

This method uses the full case mappings defined in `SpecialCasing.txt`
and returns 1 to n codepoints.

Since `utf8proc` does not expose full case mappings, this method returns an array containing -1.


<a id="RexxUnicodeCharacter_toUpper"></a>

#### 8.2.33.   toUpper

    aRexxUnicodeCharacter~toUpper

Returns the codepoint of the corresponding uppercase character.  
If the Unicode character has no uppercase mapping, returns its codepoint.

This method uses the simple case mappings defined in `UnicodeData.txt`
and always returns a single codepoint.


<a id="RexxUnicodeCharacter_toUpperFull"></a>

#### 8.2.34.   toUpperFull (not supported)

    aRexxUnicodeCharacter~toUpperFull

Returns the codepoints of the corresponding uppercase mapping as an array.
If the Unicode character has no uppercase mapping, returns an array containing its codepoint.

This method uses the full case mappings defined in `SpecialCasing.txt`
and returns 1 to n codepoints.

Since `utf8proc` does not expose full case mappings, this method returns an array containing -1.


<a id="RexxUnicodeCharacter_preferredName"></a>

#### 8.2.35.   preferredName

    aRexxUnicodeCharacter~preferredName

Returns the preferred Unicode name:

- the Unicode standard name, if not empty;
- otherwise, the Unicode name alias, if not empty;
- otherwise, the Unicode extended name (never empty).

This preferred Unicode name is used in the string representation of the character.


<a id="RexxUnicodeCharacter_UTF8"></a>

#### 8.2.36.   UTF8

    aRexxUnicodeCharacter~UTF8

Returns a string representing the UTF-8 encoding of the character.




<!----------------------------------------------------------------------------->

<a id="ICU4ooRexxInterface"></a>

## 9.   ICU4ooRexxInterface Class

<!----------------------------------------------------------------------------->

The `ICU4ooRexxInterface` class provides the interface used by the `ICU4ooRexx`
[library][icu4oorexx_library] to register its services with [`RexxUnicode`](#RexxUnicode).

`ICU4ooRexxInterface` is a private mixin class inherited by `RexxUnicode`.
Its public methods are exposed through `RexxUnicode`.

`::requires "rxunicode.cls"`

### 9.1.   Class methods

<a id="ICU4ooRexxInterface_assertICU4ooRexxIsRegistered"></a>

#### 9.1.1.   assertICU4ooRexxIsRegistered

    .ICU4ooRexxInterface~assertICU4ooRexxIsRegistered(raiseError=.true)

Raises an error if `ICU4ooRexx` is not registered.  
If `raiseError` is `.false`, returns `.true` if `ICU4ooRexx` is registered, or `.false` otherwise.


<a id="ICU4ooRexxInterface_ICU4ooRexxIsRegistered"></a>

#### 9.1.2.   ICU4ooRexxIsRegistered

    .ICU4ooRexxInterface~ICU4ooRexxIsRegistered

Returns `.true` if `ICU4ooRexx` is registered, or `.false` otherwise.


<a id="ICU4ooRexxInterface_ICU4ooRexxPackage"></a>

#### 9.1.3.   ICU4ooRexxPackage

    .ICU4ooRexxInterface~ICU4ooRexxPackage

Returns the package object for `ICU4ooRexx` if it is registered, or `.nil` otherwise.


<a id="ICU4ooRexxInterface_registerICU4ooRexx"></a>

#### 9.1.4.   registerICU4ooRexx

    .ICU4ooRexxInterface~registerICU4ooRexx(package)

Registers the specified `ICU4ooRexx` package.

Possible error messages:

- `ICU4ooRexx` is already registered with a different package.
- Class `ICU4ooRexx` not found.
- The minimum required ICU4ooRexx version is `versionRequirement`; the current version is `version`.
- The `ICU4ooRexx` class does not understand the message `u_charName`.
- The `ICU4ooRexx` class does not understand the message `u_charFromName`.

**Examples**

```rexx
-- Example 1
-- Copied from the prolog of ICU4ooRexx.cls

-- If rxunicode.cls has been loaded by the user, register ICU4ooRexx.
-- This will make ICU4ooRexx available to the rxunicode package.
if .RexxUnicode~isa(.Class), .RexxUnicode~hasMethod("registerICU4ooRexx") then do
    .RexxUnicode~registerICU4ooRexx(.context~package)
end

```

```rexx
-- Example 2
-- The ICU4ooRexx package is registered by the user.

-- If rxunicode.cls is not loaded before ICU4ooRexx.cls, the automatic registration is not done.
.context~package~loadPackage("ICU4ooRexx.cls")
.context~package~loadPackage("rxunicode.cls")
say .RexxUnicode~ICU4ooRexxIsRegistered                             -- 0
ICU4ooRexxClass = .context~package~findPublicClass("ICU4ooRexx")
.RexxUnicode~registerICU4ooRexx(ICU4ooRexxClass~package)
say .RexxUnicode~ICU4ooRexxIsRegistered                             -- 1

```


### 9.2.   Instance methods

The `ICU4ooRexxInterface` class defines no instance methods of its own.


[examples_internal_errors]: https://github.com/jlfaucher/executor5-bulk/blob/82f9531bb7b6d4be62f33b201ba62e53a74be640/main/trunk/extensions/unicode/rxunicode.cls#L1725-L1769 "Examples of internal errors"
[feedback_2026]: https://github.com/jlfaucher/executor5-bulk/blob/main/main/trunk/extensions/unicode/feedback.png "Feedbak as of 2026"
[icu4oorexx_library]: https://github.com/jlfaucher/icu4oorexx "ICU4ooRexx Library"
[newline_guidelines]: https://www.unicode.org/versions/Unicode17.0.0/core-spec/chapter-5/#G10213 "Newline Guidelines"
[typographic_conventions_code_points]: https://www.unicode.org/versions/Unicode17.0.0/core-spec/appendix-a/#G7083 "Typographic Conventions - Code Points"
[uax44_lm2]: https://unicode.org/reports/tr44/#UAX44-LM2 "Loose matching rule UAX44-LM2"
[uax44_lm3]: https://unicode.org/reports/tr44/#UAX44-LM3 "Loose matching rule UAX44-LM3"
[unicode_core_spec_combining_classes]: https://www.unicode.org/versions/Unicode17.0.0/core-spec/chapter-4/#G32493 "Unicode Core Specification - Combining Classes"
[unicode_core_spec_general_category]: https://www.unicode.org/versions/Unicode17.0.0/core-spec/chapter-4/#G124142 "Unicode Core Specification - General Category"
[unicode_core_spec_ignoring_characters]: https://www.unicode.org/versions/Unicode17.0.0/core-spec/chapter-5/#G40025 "Unicode Core Specification - Ignoring Characters in Processing"
[unicode_standard_annex_9_bidirectional_character_types]: https://www.unicode.org/reports/tr9/#Bidirectional_Character_Types "Standard Annex #9 Unicode Bidirectional Algorithm - Bidirectional Character Types"
[unicode_standard_annex_9_mirroring]: https://unicode.org/reports/tr9/#Mirroring "Standard Annex #9 Unicode Bidirectional Algorithm - Mirroring"
[unicode_standard_annex_11]: https://www.unicode.org/reports/tr11/ "Standard Annex #11 East Asian Width"
[unicode_standard_annex_15]: https://unicode.org/reports/tr15/ "Unicode Normalization Forms"
[unicode_standard_annex_29]: https://www.unicode.org/reports/tr29/ "Standard Annex #29 Unicode Text Segmentation"
[unicode_standard_annex_29_grapheme_cluster_boundaries]: https://www.unicode.org/reports/tr29/#Grapheme_Cluster_Boundaries "Standard Annex #29 Unicode Text Segmentation - Grapheme Cluster Boundaries"
[unicode_standard_annex_44_canonical_combining_class_values]: https://www.unicode.org/reports/tr44/#Canonical_Combining_Class_Values "Unicode Character Database - Canonical Combining Class Values"
[unicode_standard_annex_44_derivation_incb]: https://www.unicode.org/reports/tr44/#Derivation_InCB "Unicode Character Database - Derivation of Indic_Conjunct_Break"
[utf8proc]: https://juliastrings.github.io/utf8proc/ "utf8proc"
