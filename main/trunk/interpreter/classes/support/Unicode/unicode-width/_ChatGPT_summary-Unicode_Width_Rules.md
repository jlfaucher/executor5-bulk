<p align="right" style="font-size:10px;">
<a style="font-size:10px;" href="https://github.com/jlfaucher/executor5-bulk/blob/main/main/trunk/interpreter/classes/support/Unicode/unicode-width/_ChatGPT_summary-Unicode_Width_Rules.md">Markdown</a><br/>
<a style="font-size:10px;" href="https://html-preview.github.io/?url=https://github.com/jlfaucher/executor5-bulk/blob/main/main/trunk/interpreter/classes/support/Unicode/unicode-width/_ChatGPT_summary-Unicode_Width_Rules.html">HTML</a>
</p>

# Summarize Unicode Width Rules

**User:** Anonymous  
**Created:** 8/25/2026 13:27:01  
**Updated:** 8/26/2026 17:38:17  
**Exported:** 8/26/2026 17:40:59  
**Link:** [https://chatgpt.com/c/6a8d7c03-0244-83eb-abe4-b727ea4378cd](https://chatgpt.com/c/6a8d7c03-0244-83eb-abe4-b727ea4378cd)  

# Prompt:
25/08/2026, 13:26:58

Please makes a summary of the rules applied by the attached python script `unicode.py` to fill the WIDTH_LEAVES tables.
This script is part of the Rust crate `unicode-width`.

I don't need a technical description of the storage in WIDTH_ROOT (or WIDTH_ROOT_CJK), WIDTH_MIDDLE and WIDTH_LEAVES.
I want the rules based on the Unicode tables which give a character's width.
I don't need the rules (based on a state machine) used to calculate a string's witdh.

To give you the context:
I compared the character widths returned by utf8proc (derived from wcwidth) with the character widths returned by the Rust crate unicode-width.
There are many differences.

        Control characters:
            utf8proc returns 0.
            unicode-width return None for chars, 1 for strings. This my comment "jlf" below.
        Surrogate characters:
            utf8proc returns 0.
            unicode-width return 1.
        Unassigned characters:
            utf8proc returns 1 (default value).
            unicode-width returns 0, 1 or 2.
        and 2008 other differences that I don't list (for the moment)...

"jlf" comment:

It's not clear why they decided to return

```
'\x00'.width = None
"\0".width = 1
```

See the laconic answer "this is intentional, and according to the spec".
in https://github.com/unicode-rs/unicode-width/issues/87
This answer does not explain why returning None for control characters.

This logic is implemented outside `unicode.py`, in this hand-written function:

```
fn single_char_width_generic<const IS_CJK: bool>(c: char) -> Option<usize> {
    if c < '\u{7F}' {
        if c >= '\u{20}' {
            // U+0020 to U+007F (exclusive) are single-width ASCII codepoints
            Some(1)
        } else {
            // U+0000 to U+0020 (exclusive) are control codes
            None
        }
    } else if c >= '\u{A0}' {
        // No characters >= U+00A0 are control codes, so we can consult the lookup tables
        Some(lookup_width_generic::<IS_CJK>(c).0.into())
    } else {
        // U+007F to U+00A0 (exclusive) are control codes
        None
    }
}
```

[Attachment: unicode.py]

## Response:
25/08/2026, 13:26:59

> **Summarized Unicode width rules, leaf mappings, and UAX16 overrides**
>
> 
>
> Worked for 18s

Yes. Looking specifically at how `unicode.py` constructs the per-codepoint width maps that ultimately become `WIDTH_LEAVES`, the rules can be summarized much more simply than the table-generation machinery suggests.

The important point is that **`WIDTH_LEAVES` is essentially a compressed representation of two per-codepoint width maps**:

- the normal/non-CJK map
- the CJK map

The stored values are `0`, `1`, `2`, or `3` (`SPECIAL`). `unicode.py`

## 1. Start with Unicode EastAsianWidth

The fundamental width classification comes from **`EastAsianWidth.txt` (UAX #11)**.

The script reduces the six East Asian Width classes as follows:

| EastAsianWidth | Normal context | CJK context |
|---|---:|---:|
| `N` Neutral | 1 | 1 |
| `Na` Narrow | 1 | 1 |
| `H` Halfwidth | 1 | 1 |
| `W` Wide | 2 | 2 |
| `F` Fullwidth | 2 | 2 |
| `A` Ambiguous | 1 | 2 |

In other words:

- `N`, `Na`, `H` → narrow
- `W`, `F` → wide
- `A` → narrow normally, wide in CJK

This is explicitly how the script maps the EAW classes. `unicode.py` `unicode.py`

There is also an important default:

> A codepoint not explicitly covered by `EastAsianWidth.txt` is treated as `Neutral`, therefore width 1.

So **unassigned codepoints are not inherently given width 0 or 2 by their absence from the Unicode table**. The initial default is width 1. `unicode.py`

This is already an important difference from your utf8proc observation about unassigned characters.

---

## 2. Some `Ambiguous` characters are forced back to narrow

After loading `EastAsianWidth.txt`, the script modifies the `A` classification.

### `LineBreak=AI`

Characters with:

```text
LineBreak=AI
```

are made `Ambiguous`. `unicode.py`

Then, however:

### Ambiguous letters and modifier symbols are made narrow

If an ambiguous character has general category:

```text
Lu Ll Lt Lm Lo Sk
```

it is changed from ambiguous to narrow.

So an `A` character that is a letter or modifier symbol does **not** become width 2 in CJK. `unicode.py`

There are also two explicit EAW adjustments:

- U+0387 GREEK ANO TELEIA → ambiguous
- characters canonically equivalent to an ambiguous character decomposing with U+0338 inherit the ambiguous classification. `unicode.py`

So the EAW rule is not simply "read `EastAsianWidth.txt`."

---

## 3. Zero-width characters override EAW completely

Before applying the normal/CJK distinction, the script builds a separate `zero-width` property.

If a character is classified zero-width, **both maps receive width 0**, regardless of its East Asian Width. `unicode.py`

The principal Unicode-property rules are:

### `Default_Ignorable_Code_Point`

A character with:

```text
Default_Ignorable_Code_Point
```

is zero-width.

### `Grapheme_Extend`

A character with:

```text
Grapheme_Extend
```

is zero-width.

The script notes that this includes the usual `Mn` and `Me` characters, plus some `Mc` characters needed for canonical-equivalence consistency. `unicode.py`

### `Hangul_Syllable_Type=V` or `T`

Hangul vowel and trailing jamo are zero-width.

The rationale is that decomposed Hangul is composed of:

```text
Leading_Jamo + Vowel_Jamo + optional Trailing_Jamo
```

and only the leading jamo contributes the width of the resulting wide grapheme. `unicode.py`

---

## 4. There are explicit exceptions to those zero-width properties

The script deliberately overrides the Unicode-derived zero-width classification for some characters.

### U+FF9E and U+FF9F

These are `Grapheme_Extend`, but they are **not** treated as zero-width because they are halfwidth Katakana sound marks occupying their own terminal cell. `unicode.py`

### U+115F HANGUL CHOSEONG FILLER

It is `Default_Ignorable_Code_Point`, so would normally be zero-width, but is changed to **width 2** because of its role in decomposed Hangul. `unicode.py`

### U+2D7F TIFINAGH CONSONANT JOINER

It is explicitly made **non-zero-width**, because it is only invisible in a particular joining context. `unicode.py`

There are also explicit zero-width characters such as:

- U+070F SYRIAC ABBREVIATION MARK
- U+0605
- U+0890
- U+0891
- U+08E2
- characters selected by the `Grapheme_Cluster_Break=Prepend` minus `Prepended_Concatenation_Mark` rule
- U+A8FA DEVANAGARI CARET. `unicode.py`

So the zero-width rule is essentially:

> **EAW determines the basic width, except that a separate set of Unicode-defined or explicitly identified zero-width characters overrides it.**

---

## 5. Normal vs CJK width

Once those two classifications have been established, the basic map is constructed:

```text
if zero-width:
    width = 0
else:
    normal:
        EAW Wide → 2
        everything else → 1

    CJK:
        EAW Narrow → 1
        everything else → 2
```

That second part is another way of expressing:

```text
normal:  W/F = 2, everything else = 1
CJK:     N/Na/H = 1, everything else = 2
```

Thus **Ambiguous is 1 outside CJK and 2 in CJK**, as expected. `unicode.py`

---

## 6. Then a relatively small set of characters gets `SPECIAL`

This is important for understanding `WIDTH_LEAVES`.

The leaf value `3` does **not** mean "width 3". It means:

> **this codepoint requires special handling elsewhere.**

The script defines:

```text
ZERO   → table value 0
NARROW → table value 1
WIDE   → table value 2
everything else → SPECIAL (table value 3)
```

`unicode.py`

The special classifications include codepoints selected from Unicode properties such as:

- `Joining_Group=Alef`
- `Regional_Indicator`
- `Emoji_Modifier`
- `Emoji_Presentation`

and specific Unicode/script cases. `unicode.py`

Examples include:

- LF U+000A
- Hebrew Lamed U+05DC
- `Joining_Group=Alef`
- certain Khmer letters
- U+17A4
- U+17D8
- U+1A10
- Tifinagh consonants
- U+A4FC/U+A4FD
- U+FE0F
- U+10C03
- U+16D67/U+16D68
- all `Emoji_Presentation` characters
- all `Emoji_Modifier` characters
- all `Regional_Indicator` characters. `unicode.py`

Some of these have a normal standalone width of 1 or 2, but their **table entry is `SPECIAL`** because their width can participate in special sequence handling. The script explicitly defines, for example, emoji modifiers and emoji presentation characters as width 2 when alone. `unicode.py`

So if you are comparing **single-character widths**, it is important not to interpret the `3` in `WIDTH_LEAVES` as a width of 3.

---

## 7. Explicit CJK-only / non-CJK-only exceptions

There are four particularly relevant variation-selector/combining-mark overrides:

### CJK only

- U+0338 COMBINING LONG SOLIDUS OVERLAY
- U+FE00 VARIATION SELECTOR-1
- U+FE02 VARIATION SELECTOR-3

These are assigned special states only in the CJK map. `unicode.py`

### Non-CJK only

- U+FE01 VARIATION SELECTOR-2
- U+FE0E VARIATION SELECTOR-15

These are assigned special states only in the non-CJK map. `unicode.py`

The standalone width of the variation selectors is 0. U+0338 is also width 0 when considered alone. `unicode.py`

---

## 8. The resulting rules, condensed

If I reduce `unicode.py` to rules relevant to **the width assigned to an individual codepoint**, I would describe it this way:

### Base width

1. Read **EastAsianWidth.txt**.
2. `N`, `Na`, `H` → width 1.
3. `W`, `F` → width 2.
4. `A` → width 1 normally, width 2 in CJK.
5. Codepoints not explicitly present in EAW → `N` → width 1. `unicode.py`

### EAW corrections

6. `LineBreak=AI` → ambiguous.
7. Ambiguous characters whose general category is `Lu`, `Ll`, `Lt`, `Lm`, `Lo`, or `Sk` → narrow.
8. U+0387 → ambiguous.
9. Certain canonically equivalent characters involving U+0338 inherit ambiguity. `unicode.py`

### Zero-width override

10. `Default_Ignorable_Code_Point` → width 0.
11. `Grapheme_Extend` → width 0.
12. `Hangul_Syllable_Type=V` or `T` → width 0.
13. Additional explicitly identified Unicode cases → width 0.
14. Some exceptions are explicitly removed from zero-width, notably U+FF9E, U+FF9F, U+115F and U+2D7F. `unicode.py` `unicode.py`

### Special cases

15. A set of codepoints identified through Unicode properties such as `Emoji_Presentation`, `Emoji_Modifier`, `Regional_Indicator`, and `Joining_Group=Alef`, plus various explicitly listed script-specific characters, are stored as **SPECIAL** rather than 0/1/2. `unicode.py`
16. SPECIAL does **not** mean width 3; it means the codepoint needs the hand-written special handling.

---

## In terms of your utf8proc comparison

This gives a useful way to categorize the differences you are seeing.

Your observation:

> **utf8proc: unassigned → 1**  
> **unicode-width: unassigned → 0, 1 or 2**

is not explained by an "unassigned-character rule" in `unicode.py`. In fact, **the direct default for codepoints absent from `EastAsianWidth.txt` is width 1**. `unicode.py`

Therefore, when an unassigned codepoint gets 0 or 2 in `unicode-width`, the likely explanation is that **some other Unicode property/rule applies to that codepoint**-for example `Default_Ignorable_Code_Point`, `Grapheme_Extend`, or an EAW assignment/range that covers it-not simply that it is unassigned.

And your observation about controls is **outside these generated width tables**: the generated table can contain special handling for LF, but the `None` result for an individual control character comes from the hand-written `single_char_width_generic()` function you quoted. That function deliberately intercepts U+0000-U+001F and U+007F-U+009F before consulting the generated lookup table.

So I would keep these two layers separate in your comparison:

**`unicode.py` / `WIDTH_LEAVES`:**
> Unicode-property-based character width classification: EAW + zero-width properties + explicit Unicode/script/emoji exceptions.

**hand-written `single_char_width_generic()`:**
> Special API behavior for C0/C1 controls (`None` for a single `char`).

That distinction is important if your goal is eventually to explain your **2008 other differences** systematically. `unicode.py`

# Prompt:
25/08/2026, 13:45:44

Please find attached all my notes regarding the differences between `utf8proc` and `unicode-width`.
The 2008 other differences are at the end of the file.
Format of a line:
character description
\-->
type of width: width or eastAsianWidth 
utf8proc width
<>
unicode-width width

[Attachment: charWidth_stringWidth_test-out.txt]

## Response:
25/08/2026, 13:45:45

> **Analyzed and classified full 2008 file records**
>
> 
>
> Worked for 13s

Yes. I went through the attached comparison file, including the complete final section containing the **2008 differences**. The structure you described is clear: each difference is either a `width` difference or an `eastAsianWidth` difference, and the two values are `utf8proc <> unicode-width`. `charWidth_stringWidth_test-out.txt`

There is a very strong pattern in the 2008 differences.

## Main conclusion

The 2008 differences are **not 2008 independent special cases**. They fall into a relatively small number of systematic categories.

## 1. Combining marks: `Mc` vs `Mn`

This is by far one of the largest groups.

`unicode-width` returns **0** for many characters with General Category:

```text
Mc  Spacing_Mark
```

while utf8proc returns **1**.

Examples include:

- Devanagari vowel signs
- Bengali vowel signs
- Gurmukhi vowel signs
- Gujarati vowel signs
- Oriya vowel signs
- Tamil vowel signs
- Telugu vowel signs
- Kannada vowel signs
- Malayalam vowel signs
- Sinhala vowel signs
- Myanmar vowel signs
- Limbu vowel signs
- Tai Tham vowel signs
- Balinese vowel signs
- Sundanese vowel signs
- Batak vowel signs
- Lepcha vowel signs
- and many more.

For example, the file shows U+0903, U+093E, U+093F, U+0940, etc. as:

```text
width          0 <> 1
eastAsianWidth 0 <> 1
```

`charWidth_stringWidth_test-out.txt`

and the same pattern continues through many scripts. `charWidth_stringWidth_test-out.txt`

### Why?

This corresponds directly to one of the important rules in `unicode.py` that we discussed earlier:

> `unicode-width` treats `Grapheme_Extend` characters as zero-width, and its implementation deliberately includes certain `Mc` characters in this treatment.

So this is **not an East Asian Width difference** in the usual sense. It is a consequence of `unicode-width` adding a zero-width rule on top of EAW.

This category alone accounts for a substantial fraction of your 2008 differences.

---

## 2. Variation Selectors

Another very large and particularly clear group is the variation selectors.

The file contains:

```text
U+FE00 ... U+FE0F
```

and, at the end, the supplementary variation selectors:

```text
U+E0100 ... U+E01EF
```

All of these show:

```text
eastAsianWidth 1 <> 0
```

for the comparison in the file. `charWidth_stringWidth_test-out.txt`

The supplementary selectors produce the same pattern, through VS-256. `charWidth_stringWidth_test-out.txt`

This is therefore another **large block of differences that has a single explanation**:

> `unicode-width` treats Variation Selectors as zero-width, whereas utf8proc's character-width calculation leaves them at width 1.

This is especially interesting because it is **not merely an EAW disagreement**. The variation selectors are being overridden by the zero-width machinery.

---

## 3. Characters with `Default_Ignorable_Code_Point`

There is another broad class of differences where `unicode-width` returns 0 and utf8proc returns 1.

For example:

```text
U+0600 ARABIC NUMBER SIGN
U+0601 ARABIC SIGN SANAH
U+0602 ARABIC FOOTNOTE MARKER
U+0603 ARABIC SIGN SAFHA
U+0604 ARABIC SIGN SAMVAT
U+06DD ARABIC END OF AYAH
```

all appear as:

```text
width          0 <> 1
eastAsianWidth 0 <> 1
```

`charWidth_stringWidth_test-out.txt`

Likewise:

```text
U+2028 LINE SEPARATOR
U+2029 PARAGRAPH SEPARATOR
```

are treated as width 0 by `unicode-width`. `charWidth_stringWidth_test-out.txt`

So another useful category is:

> **Unicode-width applies a Unicode-property-based zero-width rule to characters that utf8proc/wcwidth considers width 1.**

---

## 4. Hangul compatibility / archaic Hangul

There is a particularly interesting opposite case.

For the unassigned/archaic Hangul Jamo in:

```text
U+D7B0 ... U+D7FB
```

the file shows:

```text
width          1 <> 0
eastAsianWidth 1 <> 0
```

for many characters. `charWidth_stringWidth_test-out.txt`

These are:

```text
HANGUL JUNGSEONG ...
HANGUL JONGSEONG ...
```

and are therefore another systematic group rather than isolated exceptions.

This is associated with `unicode-width`'s treatment of Hangul Jamo and its zero-width rules for the decomposed-Hangul model.

---

## 5. Explicit zero-width exceptions

There are several isolated or small groups where the same fundamental difference appears, but the character is handled explicitly rather than through a broad property.

Examples:

### U+00AD SOFT HYPHEN

```text
width          1 <> 0
eastAsianWidth 2 <> 0
```

`charWidth_stringWidth_test-out.txt`

### U+2D7F TIFINAGH CONSONANT JOINER

```text
width          0 <> 1
eastAsianWidth 0 <> 1
```

`charWidth_stringWidth_test-out.txt`

### U+A8FA DEVANAGARI CARET

```text
width          1 <> 0
eastAsianWidth 1 <> 0
```

`charWidth_stringWidth_test-out.txt`

### U+111C2 / U+111C3

These are also:

```text
width          1 <> 0
eastAsianWidth 1 <> 0
```

`charWidth_stringWidth_test-out.txt`

These are best regarded as **explicit `unicode-width` zero-width decisions**, rather than trying to group them with the large `Mc` category.

---

## 6. East Asian Width `A` differences

This is the other major class, and it is quite different from the zero-width cases.

For example:

```text
U+00A8 DIAERESIS
U+00AA FEMININE ORDINAL INDICATOR
U+00B4 ACUTE ACCENT
U+00B8 CEDILLA
U+00BA MASCULINE ORDINAL INDICATOR
```

and a very large collection of Latin, Greek, Cyrillic, modifier, mathematical and other characters show:

```text
eastAsianWidth 2 <> 1
```

`charWidth_stringWidth_test-out.txt`

The Greek block is especially obvious:

```text
U+0391 ... U+03C9
```

has `unicode-width` CJK width 1 while utf8proc reports 2. `charWidth_stringWidth_test-out.txt`

Cyrillic produces the same pattern. `charWidth_stringWidth_test-out.txt`

This is directly explained by the `unicode.py` rule we discussed:

> Characters with EAW `A` are normally width 1, but width 2 in CJK.

But `unicode-width` additionally changes many ambiguous **letters and modifier symbols** to narrow.

Consequently, this category can be summarized as:

> **unicode-width's interpretation of EAW=Ambiguous differs from utf8proc's EAW handling, especially because Unicode-width forces many ambiguous letters/modifier characters to narrow.**

This is a substantial source of your `eastAsianWidth` differences.

---

## 7. EAW differences in the opposite direction

There is also the reverse pattern:

```text
eastAsianWidth 1 <> 2
```

For example:

```text
U+2150 ... U+215A
```

(vulgar fractions), several mathematical symbols, circled/dingbat characters, etc. `charWidth_stringWidth_test-out.txt`

These are cases where:

```text
utf8proc      = 1
unicode-width = 2
```

for the CJK calculation.

This is again fundamentally an **EAW=Ambiguous interpretation** difference.

---

## 8. Hangul characters going in the opposite direction

There are also:

```text
U+302E HANGUL SINGLE DOT TONE MARK
U+302F HANGUL DOUBLE DOT TONE MARK
U+3164 HANGUL FILLER
```

with:

```text
utf8proc      = 2
unicode-width = 0
```

`charWidth_stringWidth_test-out.txt`

This is especially useful because it demonstrates that the differences aren't simply:

> "utf8proc uses wcwidth, unicode-width uses EAW."

`unicode-width` has additional semantic rules which can **override a character's EAW-derived width completely**.

---

## 9. What the 2008 differences really reduce to

I would therefore reorganize your observations into these categories:

| Category | Typical difference | Main reason |
|---|---:|---|
| `Mc` spacing marks | `0 <> 1` | `unicode-width` treats many `Mc` as zero-width |
| `Mn` / combining marks | `0 <> 1` | zero-width / grapheme-extension treatment |
| Variation Selectors | `0 <> 1` | zero-width |
| Default-Ignorable characters | `0 <> 1` | `Default_Ignorable_Code_Point` |
| Other explicit zero-width characters | `0 <> 1` | `unicode-width` exceptions |
| Hangul Jamo | `0 <> 1` | Hangul-specific handling |
| EAW Ambiguous → narrow | `2 <> 1` | `unicode-width` changes many `A` characters to narrow |
| EAW Ambiguous → wide | `1 <> 2` | CJK interpretation of `A` |
| EAW overridden to zero | `2 <> 0`, `1 <> 0` | zero-width rule overrides EAW |
| Explicit Hangul/special cases | `2 <> 0`, etc. | hand-coded Unicode-width exceptions |

So the **2008 differences are largely explained by perhaps 5-8 rules**, not thousands of arbitrary exceptions.

---

## One particularly important observation

Your file also reveals something that is easy to miss if we only look at `unicode.py` in isolation:

### `unicode-width` considers many `Mc` characters zero-width.

That is a major semantic difference from traditional `wcwidth` behavior.

For example, your file has:

```text
U+0C01 TELUGU SIGN CANDRABINDU       width 0 <> 1
U+0C02 TELUGU SIGN ANUSVARA          width 0 <> 1
U+0C03 TELUGU SIGN VISARGA           width 0 <> 1
U+0C41 TELUGU VOWEL SIGN U            width 0 <> 1
U+0C42 TELUGU VOWEL SIGN UU           width 0 <> 1
```

`charWidth_stringWidth_test-out.txt`

and the same thing occurs across many Indic and Southeast Asian scripts.

That is probably the **single most important explanation for the unexpectedly large number of differences** you are seeing.

---

## And there is a useful distinction for your future analysis

I would separate the differences into **three layers**:

### Layer 1 - `unicode.py` / generated character classification

These include:

- EAW
- `Default_Ignorable_Code_Point`
- `Grapheme_Extend`
- Hangul syllable type
- explicit zero-width exceptions
- EAW `A` corrections
- special Unicode properties.

### Layer 2 - hand-written single-character handling

This is where your:

```text
'\x00'.width == None
```

comes from. It is **not part of the generated `WIDTH_LEAVES` classification**.

### Layer 3 - string-width state machine

This accounts for things such as emoji sequences, RI pairs, combining sequences, etc., and should indeed be kept separate from the present comparison.

Your file itself confirms that distinction: it starts by documenting the control-character and surrogate behavior separately, then the generated per-character differences, and finally the 2008-character comparison list. `charWidth_stringWidth_test-out.txt`

If your objective is ultimately to determine **which of the utf8proc/wcwidth rules could be changed to make ooRexx's `codepointCharWidth()` agree more closely with `unicode-width`**, I think the next useful step is to turn these 2008 entries into a **categorized count**, e.g.:

```text
Mc / zero-width                 N
Mn / Grapheme_Extend             N
Default_Ignorable                N
Variation Selectors              N
Hangul                           N
EAW Ambiguous → 1               N
EAW Ambiguous → 2               N
explicit exceptions              N
other                             N
-----------------------------------
total                           2008
```

That would tell us quantitatively which rules account for the differences, rather than just identifying the patterns qualitatively.











# Prompt:
26/08/2026, 14:03:07

Please classify those **2008 records according to the actual rules in** **`unicode.py`**, rather than by General Category alone.
Please also explain which actual rules in `unicode.py` gives width=0 or 1 or 2 for the unassigned characters (you have the list of unassigned characters with their corresponding width in the attached file `charWidth_stringWidth_test-out.txt`.

## Response:
26/08/2026, 14:03:08

I’ll redo the classification from the 2008 records only, preserving the `utf8proc <> unicode-width` direction, then trace the unassigned-character widths back to specific `unicode.py` rules.

I’ve confirmed the 2008-record boundary and the direction of both operands. I’m now using the actual `load_east_asian_widths()`, `load_zero_widths()`, and `load_width_maps()` logic from the attached script—not the earlier General Category shortcut.

Yes. With your corrected interpretation of the line format, the classification becomes quite clean.

I have now treated the **2,008 comparison records only**: the two surrogate records are excluded, leaving **1,312 distinct codepoints**. The records break down into a small number of actual `unicode.py` rules.

## 1. The 2,008 records classified by the actual `unicode.py` rules

The key part of `unicode.py` is:

1. `EastAsianWidth.txt` is reduced to `NARROW`, `WIDE`, or `AMBIGUOUS`.
2. `LineBreak=AI` is changed to `AMBIGUOUS`.
3. Ambiguous letters/modifier symbols (`Lu`, `Ll`, `Lt`, `Lm`, `Lo`, `Sk`) are forced back to `NARROW`.
4. A separate `zero_width` map overrides EAW completely.
5. Normal width is `WIDE` only for EAW `W`; otherwise it is `NARROW`.
6. CJK width is `NARROW` only for EAW `N`; otherwise it is `WIDE`. `unicode.py` `unicode.py`

Applied to the **1,312 distinct codepoints** in your comparison:

| Actual `unicode.py` rule | Distinct codepoints | Comparison records |
|---|---:|---:|
| `zero_width` override | **622** | **876** |
| EAW `N` → narrow | **440** | **880** |
| EAW `A` + letter/modifier → narrow | **193** | **193** |
| `LineBreak=AI` → ambiguous, plus U+0387 | **55** | **55** |
| Explicit U+17A4 → wide | **1** | **2** |
| Explicit U+17D8 → width 3 | **1** | **2** |
| **Total** | **1,312** | **2,008** |

The record counts add up exactly:

```text
876 + 880 + 193 + 55 + 2 + 2 = 2008
```

This is much more meaningful than categorizing by General Category.

---

## 2. The 622 zero-width codepoints

These are the codepoints for which `load_zero_widths()` wins over the EAW-derived width.

The function explicitly says that zero width comes from `Default_Ignorable_Code_Point`, `Grapheme_Extend`, Hangul `V`/`T`, plus specific extra cases. `unicode.py`

For the **622 codepoints occurring in your differences**, the disjoint breakdown is:

| Zero-width rule | Codepoints |
|---|---:|
| `Grapheme_Extend` | **372** |
| `Hangul_Syllable_Type=V or T` | **232** |
| `Grapheme_Cluster_Break=Prepend` minus `Prepended_Concatenation_Mark` | **14** |
| residual `Default_Ignorable_Code_Point` | **3** |
| explicit U+A8FA DEVANAGARI CARET | **1** |
| **Total** | **622** |

### 2.1 `Grapheme_Extend`: 372

This consists of:

- **368 `Mn`** characters
- **4 `Mc`** characters:
  - U+302E
  - U+302F
  - U+16FF0
  - U+16FF1

The script explicitly says that `Grapheme_Extend` contains the usual `Mn`/`Me` characters **plus some `Mc` characters** needed for canonical-equivalence consistency. `unicode.py`

The Unicode data confirms, for example, that U+16FF0..U+16FF1 are `Grapheme_Extend`. ([gcc.gnu.org](https://gcc.gnu.org/pipermail/libstdc%2B%2B-cvs/2024q4/042118.html?utm_source=chatgpt.com))

### 2.2 Hangul V/T: 232

The script explicitly loads `HangulSyllableType.txt` and sets all `Vowel_Jamo` and `Trailing_Jamo` to zero width. `unicode.py`

This explains the large:

```text
U+D7B0...
U+D7CB...
U+1160..U+11FF
```

groups in your output.

### 2.3 `Prepend` minus `Prepended_Concatenation_Mark`: 14

The script takes `Grapheme_Cluster_Break=Prepend`, removes characters that already have `Prepended_Concatenation_Mark`, and makes what remains zero-width. `unicode.py`

The 14 codepoints in your differences are:

```text
U+0D4E
U+111C2..U+111C3
U+113D1
U+1193F
U+11941
U+11A84..U+11A89
U+11D46
U+11F02
```

Unicode 17's GraphemeBreakProperty data indeed includes these as `Prepend`. ([GitHub](https://github.com/latex3/unicode-data/blob/main/GraphemeBreakProperty.txt?utm_source=chatgpt.com))

### 2.4 Residual Default-Ignorable: 3

Among your differences, these are:

```text
U+00AD  SOFT HYPHEN
U+3164  HANGUL FILLER
U+FFA0  HALFWIDTH HANGUL FILLER
```

They get zero width from the initial `Default_Ignorable_Code_Point` loading. The script specifically mentions Hangul fillers in this context. `unicode.py`

### 2.5 Explicit U+A8FA

`unicode.py` explicitly says:

```python
zw_map[0xA8FA] = True
```

for DEVANAGARI CARET. `unicode.py`

So this one is not derived from EAW.

---

## 3. The 440 codepoints that become width 1

These are particularly interesting because they explain a huge fraction of your differences.

All 440 have **EAW=N** in the data used by this logic, and none is in the zero-width map.

`load_east_asian_widths()` treats:

```text
N
Na
H
```

as `NARROW`. `unicode.py`

Then `load_width_maps()` does:

```python
if zw:
    ...
else:
    if eaw == WIDE:
        not_ea = WIDE
    else:
        not_ea = NARROW

    if eaw == NARROW:
        ea = NARROW
    else:
        ea = WIDE
```

So EAW=N gives:

```text
normal = 1
CJK    = 1
```

The 440 include:

```text
398 Mc
27 Cf
11 Cn
2 Mn
1 Zl
1 Zp
```

That is exactly why your large `Mc` group has:

```text
utf8proc       0
unicode-width  1
```

for the `width` records.

For example:

```text
U+0903 DEVANAGARI SIGN VISARGA
--> width 0 <> 1
```

means precisely:

```text
utf8proc      = 0
unicode-width = 1
```

and `unicode.py` gets its `1` from **EAW=N plus `zero_width=False`**. `charWidth_stringWidth_test-out.txt`

This is the major difference we were getting backwards earlier.

---

## 4. The 193 EAW-A characters forced to narrow

This group is especially clean.

All **193** have EAW `A`, and their General Categories are:

```text
Ll  104
Lu   70
Sk   10
Lm    7
Lo    2
```

The script explicitly applies:

```python
if width_map[cp] == AMBIGUOUS:
    width_map[cp] = NARROW
```

for:

```text
Lu Ll Lt Lm Lo Sk
```

`unicode.py`

This is the reason Greek, Cyrillic, many Latin letters, modifier letters, etc. produce:

```text
utf8proc       2
unicode-width  1
```

in your `eastAsianWidth` comparisons.

For example, the Greek alphabet in your file is full of:

```text
eastAsianWidth 2 <> 1
```

`charWidth_stringWidth_test-out.txt`

This is **not** a generic "EAW=A → width 1" rule. It is specifically:

> EAW=A, followed by the script's special rule that ambiguous letters and modifier symbols are forced to NARROW.

---

## 5. The 55 characters that remain ambiguous

There are 56 records with:

```text
eastAsianWidth 1 <> 2
```

but one of them is U+17A4, which is explicitly overridden to `WIDE`.

So **55 codepoints** are actually in this category.

They are:

- **54** characters made ambiguous by `LineBreak=AI`
- **U+0387 GREEK ANO TELEIA**, explicitly made ambiguous.

The script says:

```python
# Characters with ambiguous line breaking are ambiguous
LineBreak = AI
```

and then separately:

```python
width_map[0x0387] = EastAsianWidth.AMBIGUOUS
```

`unicode.py`

These are the cases where the resulting widths are:

```text
normal = 1
CJK    = 2
```

because `AMBIGUOUS` is defined exactly that way. `unicode.py`

The list includes, for example:

```text
U+2150..U+215A
U+219A
U+219B
U+21AE
U+21CE
U+21CF
U+2204
...
U+2780..U+2793
U+1F10B
U+1F10C
```

The current Unicode 17 EAW file still lists many of these as `N`, which is precisely why the `LineBreak=AI` override matters. For example, U+2150..U+2152 and U+2155..U+215A are EAW=N in `EastAsianWidth.txt`. ([Unicode](https://www.unicode.org/Public/17.0.0/ucd/EastAsianWidth.txt))

---

## 6. U+17A4 - explicit WIDE

This one does not come from EAW processing.

`unicode.py` explicitly contains:

```python
([0x17A4], WidthState.WIDE)
```

`unicode.py`

Therefore:

```text
normal = 2
CJK    = 2
```

while your comparison shows:

```text
utf8proc       1
unicode-width  2
```

for both width types.

So this is one of the truly explicit exceptions.

---

## 7. U+17D8 - explicit width 3

This is the only width-3 case in your 2008 records.

The script explicitly contains:

```python
([0x17D8], WidthState.THREE)
```

and `WidthState.THREE.width_alone()` returns 3. `unicode.py`

Thus:

```text
utf8proc       1
unicode-width  3
```

for both normal and CJK comparisons.

And, importantly, **3 in the generated table is not a width**; it is the `SPECIAL` table encoding. The actual width is obtained from the `WidthState.THREE` special case. `unicode.py`

---

## 8. The resulting picture

So the 1,312 distinct codepoints in your 2,008 records are essentially:

```text
622  zero-width override
440  EAW=N → width 1
193  EAW=A + letter/modifier → width 1
 55  LineBreak=AI / U+0387 → EAW=A → 1/2
  1  U+17A4 → explicit width 2
  1  U+17D8 → explicit width 3
----
1312
```

That is a much more faithful description of what `unicode.py` is doing.

---

## 9. Your unassigned-character question

This part is particularly interesting.

The "Unassigned characters" section in your file gives these examples/ranges and the widths returned by `unicode-width`. `charWidth_stringWidth_test-out.txt`

The widths are **not coming from a special "unassigned character" rule**.

They fall naturally out of the two rules above:

> **EAW classification + zero-width override.**

Unicode 17's EAW data explicitly says that codepoints not otherwise listed are `N`, but there are special default-W ranges for CJK ideographs and unassigned codepoints in Planes 2 and 3. ([Unicode](https://www.unicode.org/Public/17.0.0/ucd/EastAsianWidth.txt))

### U+2065 → width 0

Your file:

```text
U+2065  Some(0)
width_cjk ... Some(0)
```

`charWidth_stringWidth_test-out.txt`

U+2065 is unassigned, but it has:

```text
Default_Ignorable_Code_Point
```

So `load_zero_widths()` sets it to ZERO, overriding EAW. Unicode explicitly documents U+2065 and the 2060..206F range as Default Ignorable, including unassigned codepoints. ([Unicode](https://www.unicode.org/versions/Unicode17.0.0/core-spec/chapter-5/?utm_source=chatgpt.com))

Therefore:

```text
unassigned + Default_Ignorable
→ width 0
```

### U+FFF0..U+FFF8 → width 0

Your file shows:

```text
U+FFF0..U+FFF8 → 0
```

`charWidth_stringWidth_test-out.txt`

Those unassigned codepoints are also `Default_Ignorable_Code_Point`. Unicode 17 explicitly identifies `FFF0..FFF8` as a Default Ignorable range, including unassigned codepoints. ([Unicode](https://www.unicode.org/versions/Unicode17.0.0/core-spec/chapter-5/?utm_source=chatgpt.com))

So again:

```text
Default_Ignorable
→ zero-width
```

### U+E0000..U+E0FFF → width 0

Your file:

```text
U+E0000..U+E0FFF → 0
```

`charWidth_stringWidth_test-out.txt`

Unicode specifically says the entire `E0000..E0FFF` range is Default Ignorable, including currently unassigned codepoints. ([Unicode](https://www.unicode.org/versions/Unicode17.0.0/core-spec/chapter-5/?utm_source=chatgpt.com))

Therefore:

```text
Default_Ignorable
→ 0
```

---

### U+FA6E..U+FAFF → width 2

Your file gives:

```text
U+FA6E..U+FAFF → 2
```

`charWidth_stringWidth_test-out.txt`

These are unassigned codepoints inside the **CJK Compatibility Ideographs** range.

Unicode's East Asian Width rules explicitly classify unassigned codepoints in `F900..FAFF` as Wide. ([Unicode](https://www.unicode.org/reports/tr11/?utm_source=chatgpt.com))

Therefore:

```text
unassigned
+ CJK Compatibility Ideographs range
+ EAW = W
→ unicode.py WIDE
→ width 2
```

There is **no special unassigned rule** here.

---

### U+2A6E0..U+2FFFD → width 2

Your file:

```text
U+2A6E0..U+2FFFD → 2
```

`charWidth_stringWidth_test-out.txt`

These are in Plane 2.

Unicode 17 specifies that **all undesignated codepoints in Plane 2 from U+20000 through U+2FFFD default to Wide**. ([Unicode](https://www.unicode.org/Public/17.0.0/ucd/EastAsianWidth.txt))

So:

```text
unassigned
+ Plane 2 CJK range
+ EAW = W
→ width 2
```

---

### U+323B0..U+3FFFD → width 2

Same principle:

```text
U+30000..U+3FFFD
```

is the corresponding Plane 3 range.

Unicode says undesignated codepoints there default to Wide. ([Unicode](https://www.unicode.org/Public/17.0.0/ucd/EastAsianWidth.txt))

Thus:

```text
→ width 2
```

---

### U+2FFFE..U+2FFFF → width 1

Your file shows:

```text
U+2FFFE → 1
U+2FFFF → 1
```

`charWidth_stringWidth_test-out.txt`

These are immediately outside the `20000..2FFFD` Wide range.

They therefore fall back to:

```text
EAW = N
→ NARROW
→ width 1
```

This is an excellent example of why **the exact range endpoint matters**.

The same happens with:

```text
U+3FFFE
U+3FFFF
```

which are outside the Plane 3 `...3FFFD` Wide range. `charWidth_stringWidth_test-out.txt`

---

### U+DFFFF → width 1

Same mechanism.

It isn't in one of the special CJK-wide ranges and isn't zero-width, so it gets the default:

```text
EAW = N
→ width 1
```

---

### U+E1000..U+EFFFF → width 1

Your file has:

```text
U+E0000..U+E0FFF → 0
U+E1000..U+EFFFF → 1
```

`charWidth_stringWidth_test-out.txt`

This is another very instructive boundary.

Only:

```text
E0000..E0FFF
```

is the Default-Ignorable reserved range. Once you pass `E0FFF`, the remaining unassigned codepoints revert to the normal default:

```text
EAW=N
→ width 1
```

---

### U+F0000..U+10FFFD → width 1 normally, 2 in CJK

Your file shows:

```text
width       U+F0000..U+10FFFD → 1
width_cjk   U+F0000..U+10FFFD → 2
```

`charWidth_stringWidth_test-out.txt`

These are **Private Use** codepoints, rather than ordinary unassigned codepoints.

Unicode's UAX #11 says private-use characters default to **Ambiguous**. ([Unicode](https://www.unicode.org/reports/tr11/?utm_source=chatgpt.com))

And `unicode.py` defines:

```text
AMBIGUOUS = 1 normally, 2 in CJK
```

`unicode.py`

So:

```text
Private Use
→ EAW=A
→ normal width 1
→ CJK width 2
```

This is probably the most important explanation for the apparent "unassigned characters have width 2" observation.

They aren't getting width 2 because they're unassigned. **They're getting width 2 because they are Private Use / EAW Ambiguous in CJK context.**

Finally:

```text
U+10FFFE
U+10FFFF
```

are outside the private-use range and fall back to EAW=N, hence width 1. `charWidth_stringWidth_test-out.txt`

---

## 10. So the unassigned-character rule can be stated very simply

For the unassigned examples in your file, `unicode.py` effectively does this:

```text
if Default_Ignorable_Code_Point:
    width = 0

else if EAW = W:
    width = 2

else if EAW = A:
    width = 1 normally
    width = 2 in CJK

else:
    width = 1
```

And **EAW itself has special defaults for unassigned codepoints**:

```text
CJK Compatibility Ideographs     → W
CJK Unified Ideographs ranges    → W
Plane 2, through U+2FFFD         → W
Plane 3, through U+3FFFD         → W
Private Use                      → A
everything else                  → N
```

Unicode 17 explicitly documents those unassigned/Private-Use EAW rules. ([Unicode](https://www.unicode.org/reports/tr11/?utm_source=chatgpt.com))

There is therefore **no mysterious "unicode-width assigns arbitrary widths to unassigned characters" rule**. The widths 0, 1 and 2 all have a direct explanation from the normal property-processing pipeline.

One caveat: your comparison output itself appears to have been generated with **older Unicode data than the attached `unicode.py`**. For example, the output calls U+113D1 unassigned, while Unicode 16/17 data treats it as Tulu-Tigalari REPHA. The current `GraphemeBreakProperty` data lists it as `Prepend`. ([gnu.googlesource.com](https://gnu.googlesource.com/gcc/%2B/refs/heads/releases/gcc-15/contrib/unicode/GraphemeBreakProperty.txt?utm_source=chatgpt.com)) That version mismatch matters for a few individual codepoints, but it does **not** change the overall rule classification above.

Note JLF: I made mistakes when I collected manually the intervals. The `charWidth_stringWidth_test.rex` script has been updated to show clearly the intervals.

---

**Sources:**

- [[gcc r15-4134] contrib, libcpp, libstdc++: Update to Unicode 16.0](https://gcc.gnu.org/pipermail/libstdc%2B%2B-cvs/2024q4/042118.html?utm_source=chatgpt.com)
- [unicode-data/GraphemeBreakProperty.txt at main · latex3/unicode-data · GitHub](https://github.com/latex3/unicode-data/blob/main/GraphemeBreakProperty.txt?utm_source=chatgpt.com)
- [](https://www.unicode.org/Public/17.0.0/ucd/EastAsianWidth.txt)
- [contrib/unicode/GraphemeBreakProperty.txt - gcc - Git at Google](https://gnu.googlesource.com/gcc/%2B/refs/heads/releases/gcc-15/contrib/unicode/GraphemeBreakProperty.txt?utm_source=chatgpt.com)



---
Powered by [ChatGPT Exporter](https://www.chatgptexporter.com)
