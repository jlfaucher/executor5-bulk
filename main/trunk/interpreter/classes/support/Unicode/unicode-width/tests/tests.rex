-- Manual port from Rust to ooRexx.

-- Copyright 2012-2026 The Rust Project Developers. See the COPYRIGHT
-- file at the top-level directory of this distribution and at
-- http://rust-lang.org/COPYRIGHT.
--
-- Licensed under the Apache License, Version 2.0 <LICENSE-APACHE or
-- http://www.apache.org/licenses/LICENSE-2.0> or the MIT license
-- <LICENSE-MIT or http://opensource.org/licenses/MIT>, at your
-- option. This file may not be copied, modified, or distributed
-- except according to those terms.


-- Not using the ooRexx test framework, to keep the lines "assert_width!" unchanged.

testSuiteClass = .test_utf8StringWidth
testSuite = testSuiteClass~new

-- Launch the tests in the order of declarations.
-- Not using `do with index methodName over testSuiteClass~methods(.nil)`
-- because the order of methods is random.
do line over .context~package~source
    if line~word(1)~caselessEquals("::method") then do
        methodName = line~word(2)
        if methodName~caselessStartsWith("test_") then do
            say
            say "--" methodName
            testSuite~send(methodName)
        end
    end
end

::requires "rxunicode.cls"

::class test_utf8StringWidth

::method test_str
    self~assert_width!("ｈｅｌｌｏ", 10, 10);
    self~assert_width!("\0\0\0\x01\x01", 5, 5);
    self~assert_width!("", 0, 0);
    self~assert_width!("\u{2081}\u{2082}\u{2083}\u{2084}", 4, 8);


::method test_emoji
    self~assert_width!("👩", 2, 2); -- Woman
    self~assert_width!("🔬", 2, 2); -- Microscope
    self~assert_width!("👩‍🔬", 2, 2); -- Woman scientist


::method test_halfwidth_katakana
    self~assert_width!("ﾊﾟｸﾞ", 4, 4); -- Halfwidth Katakana letters Pa, Gu (pug dog)


-- From README
::method test_bad_devanagari
    -- Why do they use assert_eq! instead of assert_width!?
    -- assert_eq!("क".width(), 1); -- Devanagari letter Ka
    -- assert_eq!("ष".width(), 1); -- Devanagari letter Ssa
    -- assert_eq!("क्ष".width(), 2); -- Ka + Virama + Ssa
    self~assert_width!("क", 1); -- Devanagari letter Ka
    self~assert_width!("ष", 1); -- Devanagari letter Ssa
    self~assert_width!("क्ष", 2); -- Ka + Virama + Ssa


::method test_char
    self~assert_width!('ｈ', Some(2), Some(2));
    self~assert_width!('\x00', None, None);
    self~assert_width!('\x01', None, None);
    self~assert_width!('\u{2081}', Some(1), Some(2));


::method test_char2
    self~assert_width!('\x0A', None, None);
    self~assert_width!('w', Some(1), Some(1));
    self~assert_width!('ｈ', Some(2), Some(2));
    self~assert_width!('\u{AD}', Some(0), Some(0));
    self~assert_width!('\u{1160}', Some(0), Some(0));
    self~assert_width!('\u{a1}', Some(1), Some(2));
    self~assert_width!('\u{300}', Some(0), Some(0));
    self~assert_width!('\u{FF9E}', Some(1), Some(1));
    self~assert_width!('\u{FF9F}', Some(1), Some(1));


::method unicode_12
    self~assert_width!('\u{1F971}', Some(2), Some(2));


::method test_default_ignorable
    self~assert_width!('\u{1160}', Some(0), Some(0));
    self~assert_width!('\u{3164}', Some(0), Some(0));
    self~assert_width!('\u{FFA0}', Some(0), Some(0));
    self~assert_width!('\u{E0000}', Some(0), Some(0));


::method test_ambiguous
    self~assert_width!("\u{B7}", 1, 2);
    self~assert_width!("\u{0387}", 1, 2);
    self~assert_width!("\u{A8}", 1, 1);
    self~assert_width!("\u{02C9}", 1, 1);


::method test_jamo
    self~assert_width!('\u{1100}', Some(2), Some(2));
    self~assert_width!('\u{A97C}', Some(2), Some(2));
    -- Special case: U+115F HANGUL CHOSEONG FILLER
    self~assert_width!('\u{115F}', Some(2), Some(2));
    self~assert_width!('\u{1160}', Some(0), Some(0));
    self~assert_width!('\u{D7C6}', Some(0), Some(0));
    self~assert_width!('\u{11A8}', Some(0), Some(0));
    self~assert_width!('\u{D7FB}', Some(0), Some(0));


::method test_prepended_concatenation_marks
    loop c over ( -
        '\u{0600}', -
        '\u{0601}', -
        '\u{0602}', -
        '\u{0603}', -
        '\u{0604}', -
        '\u{06DD}', -
        '\u{110BD}', -
        '\u{110CD}', -
    )
        self~assert_width!(c, Some(1), Some(1));
    end

    loop c over ('\u{0605}', '\u{070F}', '\u{0890}', '\u{0891}', '\u{08E2}')
        self~assert_width!(c, Some(0), Some(0));
    end


::method test_gcb_prepend
    self~assert_width!("ൎഉ", 1, 1);
    self~assert_width!("\u{11A89}", 0, 0);


::method test_interlinear_annotation_chars
    self~assert_width!('\u{FFF9}', Some(1), Some(1));
    self~assert_width!('\u{FFFA}', Some(1), Some(1));
    self~assert_width!('\u{FFFB}', Some(1), Some(1));


::method test_hieroglyph_format_controls
    self~assert_width!('\u{13430}', Some(1), Some(1));
    self~assert_width!('\u{13436}', Some(1), Some(1));
    self~assert_width!('\u{1343C}', Some(1), Some(1));


::method test_marks
    -- Nonspacing marks have 0 width
    self~assert_width!('\u{0301}', Some(0), Some(0));
    -- Enclosing marks have 0 width
    self~assert_width!('\u{20DD}', Some(0), Some(0));
    -- Some spacing marks have width 1
    self~assert_width!('\u{09CB}', Some(1), Some(1));
    -- But others have width 0
    self~assert_width!('\u{09BE}', Some(0), Some(0));


::method test_devanagari_caret
    self~assert_width!('\u{A8FA}', Some(0), Some(0));


::method test_solidus_overlay
    self~assert_width!("<\u{338}", 1, 2);
    self~assert_width!("=\u{338}", 1, 2);
    self~assert_width!(">\u{338}", 1, 2);
    self~assert_width!("=\u{301}\u{338}", 1, 2);
    self~assert_width!("=\u{338}\u{301}", 1, 2);
    self~assert_width!("=\u{FE0F}\u{338}", 1, 2);
    self~assert_width!("#\u{FE0F}\u{338}", 2, 2);
    self~assert_width!("#\u{338}\u{FE0F}", 1, 1);

    self~assert_width!("\u{06B8}\u{338}\u{0627}", 1, 1);
    self~assert_width!("\u{06B8}\u{338}\u{FE0E}\u{0627}", 1, 1);
    self~assert_width!("\u{06B8}\u{338}\u{FE0F}\u{0627}", 1, 1);
    self~assert_width!("\u{06B8}\u{FE0E}\u{338}\u{0627}", 1, 1);
    self~assert_width!("\u{06B8}\u{FE0F}\u{338}\u{0627}", 1, 1);

    self~assert_width!("=\u{338}\u{0627}", 2, 3);


::method test_emoji_presentation
    self~assert_width!('\u{0023}', Some(1), Some(1));
    self~assert_width!('\u{FE0F}', Some(0), Some(0));
    self~assert_width!("\u{0023}\u{FE0F}", 2, 2);
    self~assert_width!("a\u{0023}\u{FE0F}a", 4, 4);
    self~assert_width!("\u{0023}a\u{FE0F}", 2, 2);
    self~assert_width!("a\u{FE0F}", 1, 1);
    self~assert_width!("\u{0023}\u{0023}\u{FE0F}a", 4, 4);
    self~assert_width!("\u{002A}\u{FE0F}", 2, 2);
    self~assert_width!("\u{23F9}\u{FE0F}", 2, 2);
    self~assert_width!("\u{24C2}\u{FE0F}", 2, 2);
    self~assert_width!("\u{1F6F3}\u{FE0F}", 2, 2);
    self~assert_width!("\u{1F700}\u{FE0F}", 1, 1);
    self~assert_width!("\u{002A}\u{301}\u{FE0F}", 1, 1);
    self~assert_width!("\u{002A}\u{200D}\u{FE0F}", 1, 1);
    self~assert_width!("\u{002A}\u{FE0E}\u{FE0F}", 1, 1);


::method test_text_presentation
    self~assert_width!('\u{FE0E}', Some(0), Some(0));
    self~assert_width!('\u{2648}', Some(2), Some(2));
    self~assert_width!("\u{2648}\u{FE0E}", 1, 2);
    self~assert_width!("\u{2648}\u{FE0E}\u{FE0F}", 1, 2);
    self~assert_width!("\u{2648}\u{FE0F}\u{FE0E}", 2, 2);
    self~assert_width!("\u{1F21A}\u{FE0E}", 2, 2);
    self~assert_width!("\u{0301}\u{FE0E}", 0, 0);
    self~assert_width!("a\u{FE0E}", 1, 1);
    self~assert_width!("𘀀\u{FE0E}", 2, 2);
    self~assert_width!("\u{2648}\u{0301}\u{FE0E}", 2, 2);
    self~assert_width!("\u{2648}\u{200D}\u{FE0E}", 2, 2);


::method test_control_line_break
    self~assert_width!('\u{2028}', Some(1), Some(1));
    self~assert_width!('\u{2029}', Some(1), Some(1));
    self~assert_width!('\r', None, None);
    self~assert_width!('\n', None, None);
    self~assert_width!("\r", 1, 1);
    self~assert_width!("\n", 1, 1);
    self~assert_width!("\r\n", 1, 1);
    self~assert_width!("\0", 1, 1);
    self~assert_width!("1\t2\r\n3\u{85}4", 7, 7);
    self~assert_width!("\r\u{FE0F}\n", 2, 2);
    self~assert_width!("\r\u{200D}\n", 2, 2);


/*
-- Not applicable to ooRexx.
-- ooRexx has only a String type.
#[test]
fn char_str_consistent() {
    let mut s = String::with_capacity(4);
    for c in '\0'..=char::MAX {
        s.clear();
        s.push(c);
        assert_eq!(c.width().unwrap_or(1), s.width());
        #[cfg(feature = "cjk")]
        assert_eq!(c.width_cjk().unwrap_or(1), s.width_cjk());
    }
}
*/


::method test_lisu_tones
    chars  = '\u{A4F8}', '\u{A4F9}', '\u{A4FA}', '\u{A4FB}', '\u{A4FC}', '\u{A4FD}'
    chars1 = '\u{A4F8}', '\u{A4F9}', '\u{A4FA}', '\u{A4FB}'
    chars2 =                                                 '\u{A4FC}', '\u{A4FD}'
    loop c over chars
        self~assert_width!(c, 1, 1);
    end
    loop c1 over chars
        loop c2 over chars
            s= c1 || c2
            select
                when chars1~hasItem(c1), chars2~hasItem(c2) then self~assert_width!(s, 1, 1)
                otherwise self~assert_width!(s, 2, 2)
            end
        end
    end

    self~assert_width!("ꓪꓹ", 2, 2);
    self~assert_width!("ꓪꓹꓼ", 2, 2);
    self~assert_width!("ꓪꓹ\u{FE0F}ꓼ", 2, 2);
    self~assert_width!("ꓪꓹ\u{200D}ꓼ", 2, 2);
    self~assert_width!("ꓪꓹꓼ\u{FE0F}", 2, 2);
    self~assert_width!("ꓪꓹ\u{0301}ꓼ", 3, 3);
    self~assert_width!("ꓪꓹꓹ", 3, 3);
    self~assert_width!("ꓪꓼꓼ", 3, 3);


::method test_hebrew_alef_lamed
    self~assert_width!("\u{05D0}", 1, 1);
    self~assert_width!("\u{05DC}", 1, 1);
    self~assert_width!("\u{05D0}\u{05DC}", 2, 2);
    self~assert_width!("\u{05D0}\u{200D}\u{05DC}", 1, 1);
    self~assert_width!( -
        "\u{05D0}\u{200D}\u{200D}\u{200D}\u{200D}\u{200D}\u{200D}\u{200D}\u{05DC}", -
        1, -
        1, -
    );
    self~assert_width!("\u{05D0}\u{05D0}\u{200D}\u{05DC}", 2, 2);
    self~assert_width!( -
        "\u{05D0}\u{05D0}\u{200D}\u{200D}\u{200D}\u{200D}\u{200D}\u{200D}\u{05DC}", -
        2, -
        2, -
    );
    self~assert_width!("\u{05D0}\u{FE0F}\u{200D}\u{FE0F}\u{05DC}\u{FE0F}", 1, 1);
    self~assert_width!("\u{05D0}\u{FE0E}\u{200D}\u{FE0E}\u{05DC}\u{FE0E}", 1, 1);


::method test_arabic_lam_alef
    self~assert_width!("\u{0644}", 1, 1);
    self~assert_width!("\u{06B8}", 1, 1);

    self~assert_width!("\u{0623}", 1, 1);
    self~assert_width!("\u{0627}", 1, 1);

    self~assert_width!("\u{0644}\u{0623}", 1, 1);
    self~assert_width!("\u{0644}\u{0627}", 1, 1);
    self~assert_width!("\u{06B8}\u{0623}", 1, 1);
    self~assert_width!("\u{06B8}\u{0627}", 1, 1);

    self~assert_width!("\u{0644}\u{065F}\u{065E}\u{0623}", 1, 1);
    self~assert_width!("\u{0644}\u{065F}\u{065E}\u{0627}", 1, 1);
    self~assert_width!("\u{06B8}\u{065F}\u{065E}\u{0623}", 1, 1);
    self~assert_width!("\u{06B8}\u{065F}\u{065E}\u{0627}", 1, 1);

    self~assert_width!("\u{06B8}\u{FE0E}\u{0627}", 1, 1);
    self~assert_width!("\u{06B8}\u{FE0F}\u{0627}", 1, 1);
    self~assert_width!("\u{06B8}\u{17B5}\u{0627}", 1, 1);

    self~assert_width!("\u{0644}\u{0644}\u{0623}", 2, 2);
    self~assert_width!("\u{0644}\u{0644}\u{0627}", 2, 2);
    self~assert_width!("\u{06B8}\u{06B8}\u{0623}", 2, 2);
    self~assert_width!("\u{06B8}\u{06B8}\u{0627}", 2, 2);

    self~assert_width!("\u{0644}\u{200D}\u{0623}", 2, 2);
    self~assert_width!("\u{0644}\u{200D}\u{0627}", 2, 2);
    self~assert_width!("\u{06B8}\u{200D}\u{0623}", 2, 2);
    self~assert_width!("\u{06B8}\u{200D}\u{0627}", 2, 2);

    self~assert_width!("\u{0644}\u{1E94B}\u{0623}", 3, 3);
    self~assert_width!("\u{0644}\u{1E94B}\u{0627}", 3, 3);
    self~assert_width!("\u{06B8}\u{1E94B}\u{0623}", 3, 3);
    self~assert_width!("\u{06B8}\u{1E94B}\u{0627}", 3, 3);


::method test_buginese_a_i_ya
    self~assert_width!("\u{1A15}", 1, 1);
    self~assert_width!("\u{1A17}", 0, 0);
    self~assert_width!("\u{1A10}", 1, 1);

    self~assert_width!("\u{1A15}\u{1A17}\u{200D}\u{1A10}", 1, 1);
    self~assert_width!( -
        "\u{1A15}\u{1A17}\u{200D}\u{200D}\u{200D}\u{200D}\u{1A10}", -
        1, -
        1, -
    );
    self~assert_width!("\u{1A15}\u{1A17}\u{200D}\u{338}", 1, 1);
    self~assert_width!("\u{1A15}\u{FE0E}\u{1A17}\u{200D}", 1, 1);
    self~assert_width!("\u{1A15}\u{FE0F}\u{1A17}\u{200D}", 1, 1);
    self~assert_width!("\u{1A15}\u{1A17}\u{FE0E}\u{200D}", 1, 1);
    self~assert_width!("\u{1A15}\u{1A17}\u{FE0F}\u{200D}", 1, 1);
    self~assert_width!("\u{1A15}\u{1A17}\u{200D}\u{FE0E}", 1, 1);
    self~assert_width!("\u{1A15}\u{1A17}\u{200D}\u{FE0F}", 1, 1);
    self~assert_width!( -
        "\u{1A15}\u{17B5}\u{200D}\u{FE0E}\u{1A17}\u{200D}\u{FE0F}\u{200D}\u{FE0F}", -
        1, -
        1, -
    );

    self~assert_width!("\u{1A15}\u{1A15}\u{1A17}\u{200D}\u{1A10}", 2, 2);
    self~assert_width!( -
        "\u{1A15}\u{1A15}\u{1A17}\u{200D}\u{200D}\u{200D}\u{200D}\u{1A10}", -
        2, -
        2, -
    );

    self~assert_width!("\u{1A15}\u{1A17}\u{1A10}", 2, 2);
    self~assert_width!("\u{1A15}\u{200D}\u{1A10}", 2, 2);
    self~assert_width!("\u{1A15}\u{1A10}", 2, 2);
    self~assert_width!("\u{1A15}\u{1A17}\u{1A17}\u{200D}\u{1A10}", 2, 2);
    self~assert_width!("\u{1A15}\u{1A17}\u{338}\u{200D}\u{1A10}", 2, 2);


::method test_tifinagh_biconsonants
    self~assert_width!("\u{2D4F}", 1, 1);
    self~assert_width!("\u{2D3E}", 1, 1);
    self~assert_width!("\u{2D7F}", 1, 1);

    self~assert_width!("\u{2D4F}\u{200D}\u{2D3E}", 1, 1);
    self~assert_width!("\u{2D4F}\u{2D7F}\u{2D3E}", 1, 1);
    self~assert_width!("\u{2D4F}\u{200D}\u{2D3E}", 1, 1);
    self~assert_width!( -
        "\u{2D4F}\u{FE0F}\u{200D}\u{2D7F}\u{FE0E}\u{200D}\u{17B5}\u{2D3E}", -
        1, -
        1, -
    );

    self~assert_width!("\u{2D4F}\u{301}\u{2D7F}\u{2D3E}", 3, 3);
    self~assert_width!("\u{2D4F}\u{301}\u{200D}\u{2D3E}", 2, 2);
    self~assert_width!("\u{2D4F}\u{2D3E}", 2, 2);
    self~assert_width!("\u{2D4F}\u{2D7F}\u{2D7F}\u{2D3E}", 4, 4);
    self~assert_width!("\u{2D7F}\u{2D3E}", 2, 2);
    self~assert_width!("\u{2D7F}\u{2D7F}\u{2D66}", 3, 3);
    self~assert_width!("\u{2D66}\u{2D7F}\u{2D3E}", 3, 3);


::method test_old_turkic_ligature
    self~assert_width!("\u{10C32}", 1, 1);
    self~assert_width!("\u{10C03}", 1, 1);
    self~assert_width!("\u{10C32}\u{10C03}", 2, 2);

    self~assert_width!("\u{10C32}\u{200D}\u{10C03}", 1, 1);
    self~assert_width!("\u{10C32}\u{FE0F}\u{200D}\u{FE0E}\u{10C03}", 1, 1);

    self~assert_width!("\u{10C32}\u{2D7F}\u{10C03}", 3, 3);
    self~assert_width!("\u{10C32}\u{0301}\u{200D}\u{10C03}", 2, 2);
    self~assert_width!("\u{10C03}\u{200D}\u{10C32}", 2, 2);
    self~assert_width!("\u{200D}\u{10C32}", 1, 1);


::method test_khmer_coeng
    self~assert_width!("ល", 1, 1);
    self~assert_width!("ង", 1, 1);
    self~assert_width!("លង", 2, 2);
    self~assert_width!("ល្ង", 1, 1);

    say "Start iteration from U+0000 to U+10ffff, showing only failed assertions"
    loop c = 0 to .RexxUnicode~maxCodepoint
        if   "1780"~x2d <= c & c <= "1782"~x2d | "1784"~x2d <= c & c <= "1787"~x2d -
           | "1789"~x2d <= c & c <= "178C"~x2d | "178E"~x2d <= c & c <= "1793"~x2d -
           | "1795"~x2d <= c & c <= "1798"~x2d | "179B"~x2d <= c & c <= "179D"~x2d -
           | "17A0"~x2d == c | "17A2"~x2d == c | "17A7"~x2d == c -
           | "17AB"~x2d <= c & c <= "17AC"~x2d | "17AF"~x2d == c -
        then do
            self~assert_width!("\u{17D2}\u{"c~d2x"}", 0, 0, , /*sayOnlyFailed:*/ .true)
            self~assert_width!("\u{17D2}\u{200D}\u{200D}\u{"c~d2x"}", 0, 0, , /*sayOnlyFailed:*/ .true)
        end
        else do
            cString = .RexxUnicodeServices~utf8EncodeCodepoint(c, .MutableBuffer~new)~string
            cWidth = .RexxUnicodeServices~utf8StringWidth(cString)
            cEastAsianWidth = .RexxUnicodeServices~utf8StringWidth(cString, , /*eastAsianContext:*/ .true)
            self~assert_width!("\u{17D2}\u{"c~d2x"}", cWidth, cEastAsianWidth, , /*sayOnlyFailed:*/ .true)
        end
    end
    say "End iteration from U+0000 to U+10ffff"


::method test_khmer_qaa
    self~assert_width!("\u{17A4}", 2, 2);
    self~assert_width!("\u{17A2}\u{17A6}", 2, 2);


::method test_khmer_sign_beyyal
    self~assert_width!("\u{17D8}", 3, 3);
    self~assert_width!("\u{17D4}\u{179B}\u{17D4}", 3, 3);


::method test_emoji_modifier
    self~assert_width!("\u{1F46A}", 2, 2);
    self~assert_width!("\u{1F3FB}", 2, 2);
    self~assert_width!("\u{1F46A}\u{1F3FB}", 2, 2);
    self~assert_width!("\u{1F46A}\u{200D}\u{200D}\u{1F3FB}", 4, 4);


::method test_emoji_zwj
    self~assert_width!("🧑‍🤝‍🧑", 2, 2);

    self~assert_width!("🇮🇱🕊️🇵🇸", 6, 6);
    self~assert_width!("🇵🇸\u{200D}🕊️\u{200D}🇮🇱", 2, 2);
    self~assert_width!("🇮🇱\u{200D}🕊️\u{200D}\u{200D}🇵🇸", 4, 4);
    self~assert_width!("🇵🇸\u{200D}\u{200D}🕊️\u{200D}🇮🇱", 4, 4);

    self~assert_width!("🇦🇦\u{200D}🇦🇦", 2, 2);
    self~assert_width!("🇦🇦\u{200D}🇦🇦🇦", 3, 3);
    self~assert_width!("🇦🇦\u{200D}🇦🇦🇦", 3, 3);

    self~assert_width!("🇦🇦\u{200D}\u{200D}🇦🇦", 4, 4);
    self~assert_width!("🇦🇦\u{200D}🇦\u{200D}🇦🇦", 5, 5);
    self~assert_width!("🇦🇦\u{200D}🇦🇦\u{200D}🇦🇦", 2, 2);
    self~assert_width!("🇦🇦\u{200D}🇦🇦🇦\u{200D}🇦🇦", 5, 5);
    self~assert_width!("🇦🇦\u{200D}🇦🇦🇦🇦\u{200D}🇦🇦", 4, 4);
    self~assert_width!("🇦🇦\u{200D}🇦🇦🇦🇦🇦\u{200D}🇦🇦", 7, 7);
    self~assert_width!("🇦🇦\u{200D}🇦🇦🇦🇦🇦🇦\u{200D}🇦🇦", 6, 6);
    self~assert_width!("🇦🇦\u{200D}🇦🇦🇦🇦🇦🇦🇦\u{200D}🇦🇦", 9, 9);

    self~assert_width!("🏴󠁧󠁢󠁷󠁬󠁳󠁿", 2, 2);
    self~assert_width!("🏴󠁧󠁢󠁥󠁮󠁧󠁿\u{200D}🏴󠁧󠁢󠁳󠁣󠁴󠁿\u{200D}🏴󠁧󠁢󠁷󠁬󠁳󠁿", 2, 2);

    self~assert_width!("🇦👪\u{200D}🏿", 3, 3);
    self~assert_width!("🇦🏿\u{200D}🏿", 3, 3);

    self~assert_width!('🏴', Some(2), Some(2));
    self~assert_width!("\u{E0031}", 0, 0);
    self~assert_width!("\u{E0063}", 0, 0);
    self~assert_width!("\u{E007F}", 0, 0);
    self~assert_width!("🏴\u{200D}Ⓜ️", 2, 2);
    self~assert_width!("🏴\u{E0031}\u{200D}Ⓜ️", 4, 4);
    self~assert_width!("🏴\u{E0063}\u{200D}Ⓜ️", 4, 4);
    self~assert_width!("🏴\u{E007F}\u{200D}Ⓜ️", 4, 4);
    self~assert_width!("🏴\u{E0031}\u{E007F}\u{200D}Ⓜ️", 4, 4);
    self~assert_width!("🏴\u{E0031}\u{E0031}\u{E007F}\u{200D}Ⓜ️", 4, 4);
    self~assert_width!("🏴\u{E0031}\u{E0031}\u{E0031}\u{E007F}\u{200D}Ⓜ️", 2, 2);
    self~assert_width!( -
        "🏴\u{E0031}\u{E0031}\u{E0031}\u{E0031}\u{E007F}\u{200D}Ⓜ️", -
        4, -
        4, -
    );
    self~assert_width!( -
        "🏴\u{E0031}\u{E0031}\u{E0031}\u{E0063}\u{E007F}\u{200D}Ⓜ️", -
        2, -
        2, -
    );
    self~assert_width!( -
        "🏴\u{E0031}\u{E0031}\u{E0031}\u{E0063}\u{E0063}\u{E007F}\u{200D}Ⓜ️", -
        2, -
        2, -
    );
    self~assert_width!( -
        "🏴\u{E0031}\u{E0031}\u{E0031}\u{E0063}\u{E0063}\u{E0063}\u{E007F}\u{200D}Ⓜ️", -
        2, -
        2, -
    );
    self~assert_width!( -
        "🏴\u{E0031}\u{E0031}\u{E0031}\u{E0063}\u{E0063}\u{E0063}\u{E0063}\u{E007F}\u{200D}Ⓜ️", -
        2, -
        2, -
    );
    self~assert_width!( -
        "🏴\u{E0031}\u{E0031}\u{E0031}\u{E0063}\u{E0063}\u{E0063}\u{E0063}\u{E0063}\u{E007F}\u{200D}Ⓜ️", -
        4, -
        4, -
    );
    self~assert_width!("🏴\u{E0063}\u{E0063}\u{E007F}\u{200D}Ⓜ️", 4, 4);
    self~assert_width!("🏴\u{E0063}\u{E0063}\u{E0063}\u{E007F}\u{200D}Ⓜ️", 2, 2);
    self~assert_width!( -
        "🏴\u{E0063}\u{E0063}\u{E0063}\u{E0063}\u{E007F}\u{200D}Ⓜ️", -
        2, -
        2, -
    );
    self~assert_width!( -
        "🏴\u{E0063}\u{E0063}\u{E0063}\u{E0063}\u{E0063}\u{E007F}\u{200D}Ⓜ️", -
        2, -
        2, -
    );
    self~assert_width!( -
        "🏴\u{E0063}\u{E0063}\u{E0063}\u{E0063}\u{E0063}\u{E0063}\u{E007F}\u{200D}Ⓜ️", -
        2, -
        2, -
    );
    self~assert_width!( -
        "🏴\u{E0063}\u{E0063}\u{E0063}\u{E0063}\u{E0063}\u{E0063}\u{E0063}\u{E007F}\u{200D}Ⓜ️", -
        4, -
        4, -
    );

    self~assert_width!("a\u{200D}🏴󠁧󠁢󠁷󠁬󠁳󠁿", 3, 3);
    self~assert_width!("👪\u{200D}a", 3, 3);
    self~assert_width!("a\u{200D}a", 2, 2);

    self~assert_width!("*\u{FE0F}", 2, 2);
    self~assert_width!("*\u{20E3}", 1, 1);
    self~assert_width!("*️⃣", 2, 2);
    self~assert_width!("*\u{FE0F}", 2, 2);
    self~assert_width!("*\u{20E3}\u{FE0F}", 1, 1);
    self~assert_width!("*️⃣\u{200D}👪", 2, 2);
    self~assert_width!("*\u{20E3}\u{FE0F}\u{200D}👪", 3, 3);
    self~assert_width!("*\u{20E3}\u{200D}👪", 3, 3);
    self~assert_width!("*\u{FE0F}\u{200D}👪", 2, 2);
    self~assert_width!("*️⃣\u{20E3}\u{200D}👪", 4, 4);
    self~assert_width!("*\u{FE0F}\u{FE0F}\u{20E3}\u{200D}👪", 4, 4);

    self~assert_width!( -
        "🇦👪\u{200D}🏿\u{200D}👪🏻\u{200D}Ⓜ️\u{200D}*\u{FE0F}\u{200D}🇦🇦\u{200D}🏴󠁧󠁢󠁷󠁬󠁳󠁿\u{200D}👪", -
        3, -
        3, -
    );


::method test_emoji_file -- emoji_test_file
    package_dir = .File~new(.context~package~name)~parent
    norm_file = .File~new("emoji-test.txt", package_dir)
    norm_file_lines = .File~readLines(norm_file)
    if norm_file_lines~items == 0 then do
        .error~say("*** KO File not found:" norm_file)
        .error~say("run `unicode-cpp.py` first to download `emoji-test.txt`")
        .error~say("or download directly https://www.unicode.org/Public/{UNICODE_VERSION}/emoji/emoji-test.txt")
        .error~say("(replace {UNICODE_VERSION} by the value of UNICODE_VERSION in `unicode-cpp.py`)")
        return
    end
    loop line over norm_file_lines
        if line == "" | line~startsWith("#") then iterate
        parse var line cps ";" status "#" emoji_description
        status = status~strip
        if status~startsWith("fully-qualified") | status~startsWith("component") then do
            buffer = .MutableBuffer~new
            loop cp over cps~subWords
                buffer~append("\u{", cp, "}")
            end
            emoji = buffer~string
            self~assert_width!(emoji, 2, 2, emoji_description~strip)
        end
    end

::method ambiguous_line_break
    self~assert_width!("\u{24EA}", 1, 2);
    self~assert_width!("\u{2616}", 1, 2);
    self~assert_width!("\u{2780}", 1, 2);



::method test_vs1_vs2_vs3
    self~assert_width!('\u{FE00}', Some(0), Some(0));
    self~assert_width!('\u{FE01}', Some(0), Some(0));
    self~assert_width!('\u{FE02}', Some(0), Some(0));

    say "Start iteration from U+0000 to U+10ffff, showing only failed assertions"
    loop c = 0 to .RexxUnicode~maxCodepoint
        if "2018"~x2d == c | "2019"~x2d == c | "201C"~x2d == c | "201D"~x2d == c then do
            self~assert_width!("\u{"c~d2x"}", Some(1), Some(2), , /*sayOnlyFailed:*/ .true)
            self~assert_width!("\u{"c~d2x"}\u{FE00}", 1, 1, , /*sayOnlyFailed:*/ .true)
            self~assert_width!("\u{"c~d2x"}\u{FE00}\u{FE01}", 1, 1, , /*sayOnlyFailed:*/ .true)
            self~assert_width!("\u{"c~d2x"}\u{FE01}", 2, 2, , /*sayOnlyFailed:*/ .true)
            self~assert_width!("\u{"c~d2x"}\u{FE01}\u{FE00}", 2, 2, , /*sayOnlyFailed:*/ .true)
            self~assert_width!("\u{"c~d2x"}\u{FE02}", 1, 1, , /*sayOnlyFailed:*/ .true)
            self~assert_width!("\u{"c~d2x"}\u{FE02}\u{FE01}", 1, 1, , /*sayOnlyFailed:*/ .true)
        end
        else do
            cString = .RexxUnicodeServices~utf8EncodeCodepoint(c, .MutableBuffer~new)~string
            -- too slow (not native) cString = .RexxUnicode~stringUnescape("\u{"c~d2x"}")
            cWidth = .RexxUnicodeServices~utf8StringWidth(cString)
            cEastAsianWidth = .RexxUnicodeServices~utf8StringWidth(cString, , /*eastAsianContext:*/ .true)
            self~assert_width!("\u{"c~d2x"}\u{FE00}", cWidth, cEastAsianWidth, , /*sayOnlyFailed:*/ .true)
            self~assert_width!("\u{"c~d2x"}\u{FE01}", cWidth, cEastAsianWidth, , /*sayOnlyFailed:*/ .true)
            self~assert_width!("\u{"c~d2x"}\u{FE02}", cWidth, cEastAsianWidth, , /*sayOnlyFailed:*/ .true)
        end
    end
    say "End iteration from U+0000 to U+10ffff"


/*
-- Not applicable to ooRexx

#[test]
fn test_char_iter() {
    assert_eq!(char_iter_width(['a', 'b', '🔬'].into_iter()), 4)
}

// Test traits are unsealed

#[cfg(feature = "cjk")]
#[allow(dead_code)]
struct Foo;

#[cfg(feature = "cjk")]
impl UnicodeWidthChar for Foo {
    fn width(self) -> Option<usize> {
        Some(0)
    }

    fn width_cjk(self) -> Option<usize> {
        Some(0)
    }
}

#[cfg(feature = "cjk")]
impl UnicodeWidthStr for Foo {
    fn width(&self) -> usize {
        0
    }

    fn width_cjk(&self) -> usize {
        0
    }
}

*/

::method assert_width!
    use strict arg escapedString, width, eastAsianWidth=.nil, description="", sayOnlyFailed=.false

    -- 1 is the correct value for None when testing the width of a string
    -- See char_str_consistent:
    -- assert_eq!(c.width().unwrap_or(1), s.width());
    if width == "NONE" then width = 1
    if eastAsianWidth == "NONE" then eastAsianWidth = 1

    .validate~classType("escapedString", escapedString, .String)
    .validate~nonNegativeWholeNumber("width", width)
    if .nil \== eastAsianWidth then .validate~nonNegativeWholeNumber("eastAsianWidth", eastAsianWidth)
    .validate~classType("description", description, .String)
    .validate~logical("sayOnlyFailed", sayOnlyFailed)

    string = .RexxUnicode~stringUnescape(escapedString)

    stringWidth = .RexxUnicodeServices~utf8StringWidth(string)
    output = "utf8StringWidth(" quoted(escapedString) ") =" stringWidth
    failed = (stringWidth \== width)
    if failed then output = output "*** KO, expected" width
    if description \== "" then output = output "#" description
    if sayOnlyFailed then do
        if failed then say output
    end
    else say output

    if .nil \== eastAsianWidth then do
        stringEastAsianWidth = .RexxUnicodeServices~utf8StringWidth(string, , /*eastAsianContext:*/ .true)
        output = "utf8StringWidth(" quoted(escapedString) ", /*eastAsianContext:*/ .true ) =" stringEastAsianWidth
        failed = (stringEastAsianWidth \== eastAsianWidth)
        if failed then output = output "*** KO, expected" eastAsianWidth
        if description \== "" then output = output "#" description
        if sayOnlyFailed then do
            if failed then say output
        end
        else say output
    end


-- To keep the assertions testing `Some(value)` unchanged
::routine some
    use strict arg value
    return value
