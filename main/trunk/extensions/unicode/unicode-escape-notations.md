# Unicode Escape Character Notations Across Languages

## Named character escapes

| Notation | Meaning | Languages |
|---|---|---|
| `\N{Unicode name}` | Character name in the Unicode database | Python, Perl, C++23 |
| `\&name;` | HTML5 named character entity (different convention — semicolon-terminated, not braced) | D |
| `\c[NAME]` / `\c[U+XXXX]` | Named or numeric character (square brackets, not braces) | Raku (Perl 6) |

## Arbitrary-length hex / octal escapes

| Notation | Meaning | Languages |
|---|---|---|
| `\o{X..X}` | Arbitrary number of octal digits | C++23 |
| `\x{X..X}` | Arbitrary number of hex digits | C++23, Perl |
| `\u{X..X}` | Unicode scalar value, 1–8 hex digits | C++23, Swift |
| `\u{X..X}` | Unicode scalar value, 1–6 hex digits (only brace-hex form Rust has — no `\x{...}`) | Rust |
| `\u{X..X}` | Unicode scalar value, 1+ hex digits (beyond `\uFFFF`) | JavaScript (ES2015+) |
| `\u{X..X}` | Unicode scalar value, hex digits, UTF-8 encoded | Zig, PHP (PHP 7+) |
| `\u{nnnn ...}` | One or more Unicode characters, each 1–6 hex digits, space-separated | Ruby |
| `\xH..H` | Variable-length hex digits (1–4), *not* fixed-width like the row below | C# |

## Fixed-width hex escapes

| Notation | Meaning | Languages |
|---|---|---|
| `\uXXXX` | Unicode scalar value, exactly 4 hex digits | C, C++ (pre-C++23), C#, D, Go, Java, JavaScript, JSON, Julia, Netrexx, Python, Ruby |
| `\UXXXXXXXX` | Unicode scalar value, exactly 8 hex digits | C, C++ (pre-C++23), C#, D, Go, Julia, Python |
| `\xNN` | 1 byte, exactly 2 hex digits | D, Go, JavaScript, Netrexx, Python, Ruby, Zig |
| `\xNN` | 1 byte, exactly 2 hex digits, ASCII range only (`0x00`–`0x7F`) | Rust |

## Octal escapes

| Notation | Meaning | Languages |
|---|---|---|
| `\NNN` | Character/byte denoted by up to 3 octal digits (terminates early at first non-octal digit) | C, C++, D, Go, Python, Ruby |

---

## Notes

- **Why is there no `\XNNNN` (uppercase `X`, hex digits)?** Not just an oversight — uppercase `\X` is already a reserved symbol in most Unicode-aware regex engines (Perl, PCRE-derived engines, R/ICU-based regex, and others), where it means "match a whole extended grapheme cluster" — a user-perceived character that may span several combined code points (e.g. a base letter plus accent marks). Reusing `\X` as a hex-digit escape would collide with that established meaning, so no language has claimed it for that purpose.
- **Why isn't `U+XXXX` a row in this table?** `U+XXXX` (conventionally 4–6 hex digits) is the Unicode Standard's own notation for referring to code points in *prose and documentation* — not a string/character escape sequence in any programming language. The 4–6 digit range simply reflects the code point space: U+0000–U+FFFF (the original Basic Multilingual Plane) needs 4 digits, U+10000–U+10FFFF (the supplementary planes) needs 5–6. No language lets you write `U+1F600` inside a string literal; each one wraps that same numeric value in its own escape grammar instead (`\u{1F600}`, `\U0001F600`, etc.), which is why every row above is either fixed-width or has its own distinct bound — none of them is *specifically* "4 to 6."
- **`\u{Unicode name}` / `\U{Unicode name}`** (from the original draft) don't correspond to any known language feature. Every language that supports *named* escapes uses `\N{name}` (or D's `\&name;`, or Raku's `\c[NAME]`) — the `\u{...}`/`\U{...}` brace forms are reserved for hex digits everywhere they appear. These two entries were likely leftover placeholders and can be dropped.
- D's named-entity escape is its own family: it uses HTML5 entity names (e.g. `\&euro;` → €, `\&copy;` → ©) rather than Unicode character names, and is delimited with `&...;` rather than braces.
- Raku's bracket convention (`[...]`) is distinct from both the brace (`{...}`) and ampersand-semicolon (`&...;`) conventions used elsewhere.
- C#'s `\xH..H` is unusual in being hex but *variable*-length (1–4 digits) rather than fixed — it doesn't cleanly belong in either the "arbitrary-length" or "fixed-width" buckets, but it's closer to the fixed-width family in spirit (Unicode-codepoint-only) and limited compared to C++23's truly unbounded `\x{...}`.
