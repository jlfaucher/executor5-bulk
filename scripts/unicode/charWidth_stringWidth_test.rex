/****
Compare
.RexxUnicodeServices~codepointCharWidth (utf8proc)
with
.RexxUnicodeServices~utf8StringWidth (Rust crate unicode-width).
List the Unicode characters for which the two methods give different results.


***************************
There are MANY differences!
***************************


Control characters:
    utf8proc returns 0.
    unicode-width return None for char, 1 for string.
    utf8StringWidth returns 1.

Surrogate characters:
    utf8proc returns 0.
    unicode-width return 1.
    utf8StringWidth returns 3 because a surrogate character alone is invalid, replaced by 3 replacement characters.
    <?> \x{EDA080} U+D800 Cs Surrogate "<lead surrogate-D800>" --> width 0 <> 3
    ...
    <?> \x{EDBFBF} U+DFFF Cs Surrogate "<trail surrogate-DFFF>" --> width 0 <> 3

Unassigned characters:
    utf8proc returns 1 (default value).
    unicode-width returns 0, 1 or 2.

    Widths returned by unicode-width:
    - Each character is always listed twice
      (one line for width and one line for width_cjk).
    - Intervals are indicated with "...".
    - Each line displays the escaped notation twice because unassigned characters
      are not printable. When the character is printable, the second  escaped
      notation is replaced by the printed character.


    width     \u{2065} '\u{2065}' Some(0) 0
    width_cjk \u{2065} '\u{2065}' Some(0) 0

    width     \u{fa6e} '\u{fa6e}' Some(2) 2
    width_cjk \u{fa6e} '\u{fa6e}' Some(2) 2
    ...
    width     \u{faff} '\u{faff}' Some(2) 2
    width_cjk \u{faff} '\u{faff}' Some(2) 2

    width     \u{fff0} '\u{fff0}' Some(0) 0
    width_cjk \u{fff0} '\u{fff0}' Some(0) 0
    ...
    width     \u{fff8} '\u{fff8}' Some(0) 0
    width_cjk \u{fff8} '\u{fff8}' Some(0) 0

    width     \u{2a6e0} '\u{2a6e0}' Some(2) 2
    width_cjk \u{2a6e0} '\u{2a6e0}' Some(2) 2
    ...
    width     \u{2fffd} '\u{2fffd}' Some(2) 2
    width_cjk \u{2fffd} '\u{2fffd}' Some(2) 2

    width     \u{2fffe} '\u{2fffe}' Some(1) 1
    width_cjk \u{2fffe} '\u{2fffe}' Some(1) 1
    width     \u{2ffff} '\u{2ffff}' Some(1) 1
    width_cjk \u{2ffff} '\u{2ffff}' Some(1) 1

    width     \u{323b0} '\u{323b0}' Some(2) 2
    width_cjk \u{323b0} '\u{323b0}' Some(2) 2
    ...
    width     \u{3fffd} '\u{3fffd}' Some(2) 2
    width_cjk \u{3fffd} '\u{3fffd}' Some(2) 2

    width     \u{3fffe} '\u{3fffe}' Some(1) 1
    width_cjk \u{3fffe} '\u{3fffe}' Some(1) 1
    ...
    width     \u{dffff} '\u{dffff}' Some(1) 1
    width_cjk \u{dffff} '\u{dffff}' Some(1) 1

    width     \u{e0000} '\u{e0000}' Some(0) 0
    width_cjk \u{e0000} '\u{e0000}' Some(0) 0
    ...
    width     \u{e0fff} '\u{e0fff}' Some(0) 0
    width_cjk \u{e0fff} '\u{e0fff}' Some(0) 0

    width     \u{e1000} '\u{e1000}' Some(1) 1
    width_cjk \u{e1000} '\u{e1000}' Some(1) 1
    ...
    width     \u{effff} '\u{effff}' Some(1) 1
    width_cjk \u{effff} '\u{effff}' Some(1) 1

    width     \u{f0000} '\u{f0000}' Some(1) 1
    width_cjk \u{f0000} '\u{f0000}' Some(2) 2
    ...
    width     \u{10fffd} '\u{10fffd}' Some(1) 1
    width_cjk \u{10fffd} '\u{10fffd}' Some(2) 2

    width     \u{10fffe} '\u{10fffe}' Some(1) 1
    width_cjk \u{10fffe} '\u{10fffe}' Some(1) 1
    width     \u{10ffff} '\u{10ffff}' Some(1) 1
    width_cjk \u{10ffff} '\u{10ffff}' Some(1) 1

--------------------------------------------------------------------------------

****/

/*
Rust script
    use unicode_width::{UnicodeWidthChar, UnicodeWidthStr};

    fn main() {
        let mut s = String::with_capacity(4);
        for c in '\0'..=char::MAX {
            s.clear();
            s.push(c);
            println!("width     {} {:?} {:?} {:?}", c.escape_unicode(), c, c.width(), s.width());
            println!("width_cjk {} {:?} {:?} {:?}", c.escape_unicode(), c, c.width_cjk(), s.width_cjk());
        }
    }


BBEdit regular expression to search the next line not containing "Some(1)":
    ^((?!Some\(1\)).)*$


Analysis with Xcode:
    -e '.context~package~loadPackage( "rxunicode.cls" ); say .RexxUnicodeServices~utf8StringWidth(.RexxUnicode~stringUnescape( "\u{2065}" ))'

    Remember:
    Surrounding the -e expression with single quotes instead of double quotes is mandatory!
    Otherwise, you would need to escape the backslash ("\\u{2065}") to protect it from the shell.

    These widths are always retrieved from
        std::pair<std::uint8_t, WidthInfo> ret = lookup_width_generic<IS_CJK>(c);
        return std::make_pair((std::int8_t)ret.first, ret.second);
            return lookup_width(c);
                std::uint8_t t1_offset = WIDTH_ROOT[cp >> 13];
                std::uint8_t t2_offset = WIDTH_MIDDLE[t1_offset][cp >> 7 & 0x3F];
                std::uint8_t packed_widths = WIDTH_LEAVES[t2_offset][cp >> 2 & 0x1F];
                std::uint8_t width = (packed_widths >> (2 * (cp & 0b11))) & 0b11;
                if (width < 3) {
                    return {width, WidthInfo::DEFAULT};
                }
                // width == 3: Specific rules (never activated for the unassigned characters)
    Conclusion:
    The widths of unassigned characters are always directly retrieved from tables.
*/


call source_doc
count = 0
loop codepoint = 0 to .RexxUnicode~maxCodepoint
    .RexxUnicodeServices~codepointCategory( codepoint, >refCode)
    if refCode == "Cc" then iterate -- Control
    if refCode == "Cn" then iterate -- Unassigned
    if refCode == "Cs" then iterate -- Surrogate

    -- utf8proc
    width1 = .RexxUnicodeServices~codepointCharWidth(codepoint)
    eastAsianWidthIsAmbiguous = .RexxUnicodeServices~codepointEastAsianWidthIsAmbiguous(codepoint)
    -- An ambiguous width is two columns wide in East Asian context; one column wide otherwise.
    eastAsianWidth1 = eastAsianWidthIsAmbiguous~?(width1 + 1, width1)

    -- unicode-width
    string = .RexxUnicodeServices~utf8EncodeCodepoint(codepoint, .MutableBuffer~new)~string
    width2 = .RexxUnicodeServices~utf8StringWidth(string)
    eastAsianWidth2 = .RexxUnicodeServices~utf8StringWidth(string, , /*eastAsianContext:*/ .true)

    if width1 \== width2 then do
        count += 1
        say .RexxUnicodeCharacter~new(codepoint)~string~left(100) "--> width         " width1 "<>" width2
    end
    if eastAsianWidth1 \== eastAsianWidth2 then do
        count += 1
        say .RexxUnicodeCharacter~new(codepoint)~string~left(100) "--> eastAsianWidth" eastAsianWidth1 "<>" eastAsianWidth2
    end
end
s = (count > 1)~?("s", "")
say count "difference"s


::routine source_doc
    -- The text is taken from the first /**** ... ****/ comment at the begining of the source file.
    use strict arg -- none
    display = 0
    loop line = 1 to .context~package~sourceSize
        sourceLine = .context~package~sourceLine(line)
        if sourceLine~startsWith("****/") then leave
        if display == 1 then say sourceLine
        if sourceLine~startsWith("/****") then display = 1
    end


::requires "rxunicode.cls"
::requires "icu4oorexx.cls" -- optional, for character names
