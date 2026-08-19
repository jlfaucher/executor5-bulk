// Ported from Rust to C++ by Claude Sonnet 5.

// Copyright 2012-2026 The Rust Project Developers. See the COPYRIGHT
// file at the top-level directory of this distribution and at
// http://rust-lang.org/COPYRIGHT.
//
// Licensed under the Apache License, Version 2.0 <LICENSE-APACHE or
// http://www.apache.org/licenses/LICENSE-2.0> or the MIT license
// <LICENSE-MIT or http://opensource.org/licenses/MIT>, at your
// option. This file may not be copied, modified, or distributed
// except according to those terms.

#include "lookup.hpp"
#include "props.hpp"
#include "gen/lookup.hpp"
#include <stdexcept>

extern "C" {
#include "Unicode/utf8proc/utf8proc.h"
}

namespace unicode_width {

template <bool IS_CJK>
static inline std::pair<std::uint8_t, WidthInfo> lookup_width_generic(char32_t c) {
    // NOTE: plain `if`, not `if constexpr` (C++17). IS_CJK is a template
    // non-type parameter, so this still resolves to a single branch per
    // instantiation - the compiler folds it away just the same.
    if (IS_CJK) {
        return lookup_width_cjk(c);
    } else {
        return lookup_width(c);
    }
}

// Faithful transcription of width_in_generic<const IS_CJK: bool> from
// src/lookup.rs. Every branch below corresponds 1:1 to a branch there;
// Rust's `matches!`/tuple-match syntax becomes explicit comparisons and a
// switch, but the control flow and the set of WidthInfo transitions are
// unchanged.
template <bool IS_CJK>
std::pair<std::int8_t, WidthInfo> width_in_generic(char32_t c, WidthInfo next_info) {
    if (next_info.is_emoji_presentation()) {
        if (starts_emoji_presentation_seq(c)) {
            std::int8_t width = next_info.is_zwj_emoji_presentation() ? 0 : 2;
            return std::make_pair(width, WidthInfo::EMOJI_PRESENTATION);
        } else {
            next_info = next_info.unset_emoji_presentation();
        }
    }

    if (IS_CJK) {
        if ((next_info == WidthInfo::COMBINING_LONG_SOLIDUS_OVERLAY ||
             next_info == WidthInfo::SOLIDUS_OVERLAY_ALEF) &&
            (c == U'<' || c == U'=' || c == U'>')) {
            return std::make_pair((std::int8_t)2, WidthInfo::DEFAULT);
        }
    }

    if (c <= 0xA0) {
        if (c == U'\n') return std::make_pair((std::int8_t)1, WidthInfo::LINE_FEED);
        if (c == U'\r' && next_info == WidthInfo::LINE_FEED) {
            return std::make_pair((std::int8_t)0, WidthInfo::DEFAULT);
        }
        return std::make_pair((std::int8_t)1, WidthInfo::DEFAULT);
    }

    // Fast path
    if (next_info != WidthInfo::DEFAULT) {
        if (c == 0xFE0F) return std::make_pair((std::int8_t)0, next_info.set_emoji_presentation());

        if (IS_CJK) {
            if (c == 0xFE00 || c == 0xFE02) {
                return std::make_pair((std::int8_t)0, next_info.set_vs1_2_3());
            }
        } else {
            if (c == 0xFE01) return std::make_pair((std::int8_t)0, next_info.set_vs1_2_3());
            if (c == 0xFE0E) return std::make_pair((std::int8_t)0, next_info.set_text_presentation());
            if (next_info.is_text_presentation()) {
                if (starts_non_ideographic_text_presentation_seq(c)) {
                    return std::make_pair((std::int8_t)1, WidthInfo::DEFAULT);
                } else {
                    next_info = next_info.unset_text_presentation();
                }
            }
        }

        if (next_info.is_vs1_2_3()) {
            if (c == 0x2018 || c == 0x2019 || c == 0x201C || c == 0x201D) {
                return std::make_pair(IS_CJK ? (std::int8_t)1 : (std::int8_t)2, WidthInfo::DEFAULT);
            } else {
                next_info = next_info.unset_vs1_2_3();
            }
        }
        if (next_info.is_ligature_transparent()) {
            if (c == 0x200D) {
                return std::make_pair((std::int8_t)0, next_info.set_zwj_bit());
            } else if (is_ligature_transparent(c)) {
                return std::make_pair((std::int8_t)0, next_info);
            }
        }

        // COMBINING_LONG_SOLIDUS_OVERLAY, transparent-to-solidus char (CJK only)
        if (IS_CJK) {
            if (next_info == WidthInfo::COMBINING_LONG_SOLIDUS_OVERLAY &&
                is_solidus_transparent(c)) {
                return std::make_pair((std::int8_t)lookup_width_generic<IS_CJK>(c).first,
                                       WidthInfo::COMBINING_LONG_SOLIDUS_OVERLAY);
            }
            if (next_info == WidthInfo::JOINING_GROUP_ALEF && c == 0x0338) {
                return std::make_pair((std::int8_t)0, WidthInfo::SOLIDUS_OVERLAY_ALEF);
            }
        }
        // Arabic Lam-Alef ligature
        if (next_info == WidthInfo::JOINING_GROUP_ALEF && is_joining_group_lam(c)) {
            return std::make_pair((std::int8_t)0, WidthInfo::DEFAULT);
        }
        if (IS_CJK) {
            if (next_info == WidthInfo::SOLIDUS_OVERLAY_ALEF && is_joining_group_lam(c)) {
                return std::make_pair((std::int8_t)0, WidthInfo::DEFAULT);
            }
        }
        if (next_info == WidthInfo::JOINING_GROUP_ALEF && is_transparent_zero_width(c)) {
            return std::make_pair((std::int8_t)0, WidthInfo::JOINING_GROUP_ALEF);
        }

        // Hebrew Alef-ZWJ-Lamed ligature
        if (next_info == WidthInfo::ZWJ_HEBREW_LETTER_LAMED && c == 0x05D0) {
            return std::make_pair((std::int8_t)0, WidthInfo::DEFAULT);
        }

        // Khmer coeng signs
        if (next_info == WidthInfo::KHMER_COENG_ELIGIBLE_LETTER && c == 0x17D2) {
            return std::make_pair((std::int8_t)-1, WidthInfo::DEFAULT);
        }

        // Buginese <a, -i> ZWJ ya ligature
        if (next_info == WidthInfo::ZWJ_BUGINESE_LETTER_YA && c == 0x1A17) {
            return std::make_pair((std::int8_t)0, WidthInfo::BUGINESE_VOWEL_SIGN_I_ZWJ_LETTER_YA);
        }
        if (next_info == WidthInfo::BUGINESE_VOWEL_SIGN_I_ZWJ_LETTER_YA && c == 0x1A15) {
            return std::make_pair((std::int8_t)0, WidthInfo::DEFAULT);
        }

        // Tifinagh bi-consonants
        if ((next_info == WidthInfo::TIFINAGH_CONSONANT ||
             next_info == WidthInfo::ZWJ_TIFINAGH_CONSONANT) && c == 0x2D7F) {
            return std::make_pair((std::int8_t)1, WidthInfo::TIFINAGH_JOINER_CONSONANT);
        }
        if (next_info == WidthInfo::ZWJ_TIFINAGH_CONSONANT &&
            ((c >= 0x2D31 && c <= 0x2D65) || c == 0x2D6F)) {
            return std::make_pair((std::int8_t)0, WidthInfo::DEFAULT);
        }
        if (next_info == WidthInfo::TIFINAGH_JOINER_CONSONANT &&
            ((c >= 0x2D31 && c <= 0x2D65) || c == 0x2D6F)) {
            return std::make_pair((std::int8_t)-1, WidthInfo::DEFAULT);
        }

        // Lisu tone letter combinations
        if (next_info == WidthInfo::LISU_TONE_LETTER_MYA_NA_JEU &&
            c >= 0xA4F8 && c <= 0xA4FB) {
            return std::make_pair((std::int8_t)0, WidthInfo::DEFAULT);
        }

        // Old Turkic ligature
        if (next_info == WidthInfo::ZWJ_OLD_TURKIC_LETTER_ORKHON_I && c == 0x10C32) {
            return std::make_pair((std::int8_t)0, WidthInfo::DEFAULT);
        }

        // Emoji modifier
        if (next_info == WidthInfo::EMOJI_MODIFIER && is_emoji_modifier_base(c)) {
            return std::make_pair((std::int8_t)0, WidthInfo::EMOJI_PRESENTATION);
        }

        // Regional indicator
        if ((next_info == WidthInfo::REGIONAL_INDICATOR ||
             next_info == WidthInfo::SEVERAL_REGIONAL_INDICATOR) &&
            c >= 0x1F1E6 && c <= 0x1F1FF) {
            return std::make_pair((std::int8_t)1, WidthInfo::SEVERAL_REGIONAL_INDICATOR);
        }

        // ZWJ emoji
        if ((next_info == WidthInfo::EMOJI_PRESENTATION ||
             next_info == WidthInfo::SEVERAL_REGIONAL_INDICATOR ||
             next_info == WidthInfo::EVEN_REGIONAL_INDICATOR_ZWJ_PRESENTATION ||
             next_info == WidthInfo::ODD_REGIONAL_INDICATOR_ZWJ_PRESENTATION ||
             next_info == WidthInfo::EMOJI_MODIFIER) && c == 0x200D) {
            return std::make_pair((std::int8_t)0, WidthInfo::ZWJ_EMOJI_PRESENTATION);
        }
        if (next_info == WidthInfo::ZWJ_EMOJI_PRESENTATION && c == 0x20E3) {
            return std::make_pair((std::int8_t)0, WidthInfo::KEYCAP_ZWJ_EMOJI_PRESENTATION);
        }
        if (next_info == WidthInfo::VS16_ZWJ_EMOJI_PRESENTATION &&
            starts_emoji_presentation_seq(c)) {
            return std::make_pair((std::int8_t)0, WidthInfo::EMOJI_PRESENTATION);
        }
        if (next_info == WidthInfo::VS16_KEYCAP_ZWJ_EMOJI_PRESENTATION &&
            ((c >= U'0' && c <= U'9') || c == U'#' || c == U'*')) {
            return std::make_pair((std::int8_t)0, WidthInfo::EMOJI_PRESENTATION);
        }
        if (next_info == WidthInfo::ZWJ_EMOJI_PRESENTATION &&
            c >= 0x1F1E6 && c <= 0x1F1FF) {
            return std::make_pair((std::int8_t)1, WidthInfo::REGIONAL_INDICATOR_ZWJ_PRESENTATION);
        }
        if ((next_info == WidthInfo::REGIONAL_INDICATOR_ZWJ_PRESENTATION ||
             next_info == WidthInfo::ODD_REGIONAL_INDICATOR_ZWJ_PRESENTATION) &&
            c >= 0x1F1E6 && c <= 0x1F1FF) {
            return std::make_pair((std::int8_t)-1, WidthInfo::EVEN_REGIONAL_INDICATOR_ZWJ_PRESENTATION);
        }
        if (next_info == WidthInfo::EVEN_REGIONAL_INDICATOR_ZWJ_PRESENTATION &&
            c >= 0x1F1E6 && c <= 0x1F1FF) {
            return std::make_pair((std::int8_t)3, WidthInfo::ODD_REGIONAL_INDICATOR_ZWJ_PRESENTATION);
        }
        if (next_info == WidthInfo::ZWJ_EMOJI_PRESENTATION &&
            c >= 0x1F3FB && c <= 0x1F3FF) {
            return std::make_pair((std::int8_t)0, WidthInfo::EMOJI_MODIFIER);
        }
        if (next_info == WidthInfo::ZWJ_EMOJI_PRESENTATION && c == 0xE007F) {
            return std::make_pair((std::int8_t)0, WidthInfo::TAG_END_ZWJ_EMOJI_PRESENTATION);
        }
        if (next_info == WidthInfo::TAG_END_ZWJ_EMOJI_PRESENTATION &&
            c >= 0xE0061 && c <= 0xE007A) {
            return std::make_pair((std::int8_t)0, WidthInfo::TAG_A1_END_ZWJ_EMOJI_PRESENTATION);
        }
        if (next_info == WidthInfo::TAG_A1_END_ZWJ_EMOJI_PRESENTATION &&
            c >= 0xE0061 && c <= 0xE007A) {
            return std::make_pair((std::int8_t)0, WidthInfo::TAG_A2_END_ZWJ_EMOJI_PRESENTATION);
        }
        if (next_info == WidthInfo::TAG_A2_END_ZWJ_EMOJI_PRESENTATION &&
            c >= 0xE0061 && c <= 0xE007A) {
            return std::make_pair((std::int8_t)0, WidthInfo::TAG_A3_END_ZWJ_EMOJI_PRESENTATION);
        }
        if (next_info == WidthInfo::TAG_A3_END_ZWJ_EMOJI_PRESENTATION &&
            c >= 0xE0061 && c <= 0xE007A) {
            return std::make_pair((std::int8_t)0, WidthInfo::TAG_A4_END_ZWJ_EMOJI_PRESENTATION);
        }
        if (next_info == WidthInfo::TAG_A4_END_ZWJ_EMOJI_PRESENTATION &&
            c >= 0xE0061 && c <= 0xE007A) {
            return std::make_pair((std::int8_t)0, WidthInfo::TAG_A5_END_ZWJ_EMOJI_PRESENTATION);
        }
        if (next_info == WidthInfo::TAG_A5_END_ZWJ_EMOJI_PRESENTATION &&
            c >= 0xE0061 && c <= 0xE007A) {
            return std::make_pair((std::int8_t)0, WidthInfo::TAG_A6_END_ZWJ_EMOJI_PRESENTATION);
        }
        if ((next_info == WidthInfo::TAG_END_ZWJ_EMOJI_PRESENTATION ||
             next_info == WidthInfo::TAG_A1_END_ZWJ_EMOJI_PRESENTATION ||
             next_info == WidthInfo::TAG_A2_END_ZWJ_EMOJI_PRESENTATION ||
             next_info == WidthInfo::TAG_A3_END_ZWJ_EMOJI_PRESENTATION ||
             next_info == WidthInfo::TAG_A4_END_ZWJ_EMOJI_PRESENTATION) &&
            c >= 0xE0030 && c <= 0xE0039) {
            return std::make_pair((std::int8_t)0, WidthInfo::TAG_D1_END_ZWJ_EMOJI_PRESENTATION);
        }
        if (next_info == WidthInfo::TAG_D1_END_ZWJ_EMOJI_PRESENTATION &&
            c >= 0xE0030 && c <= 0xE0039) {
            return std::make_pair((std::int8_t)0, WidthInfo::TAG_D2_END_ZWJ_EMOJI_PRESENTATION);
        }
        if (next_info == WidthInfo::TAG_D2_END_ZWJ_EMOJI_PRESENTATION &&
            c >= 0xE0030 && c <= 0xE0039) {
            return std::make_pair((std::int8_t)0, WidthInfo::TAG_D3_END_ZWJ_EMOJI_PRESENTATION);
        }
        if ((next_info == WidthInfo::TAG_A3_END_ZWJ_EMOJI_PRESENTATION ||
             next_info == WidthInfo::TAG_A4_END_ZWJ_EMOJI_PRESENTATION ||
             next_info == WidthInfo::TAG_A5_END_ZWJ_EMOJI_PRESENTATION ||
             next_info == WidthInfo::TAG_A6_END_ZWJ_EMOJI_PRESENTATION ||
             next_info == WidthInfo::TAG_D3_END_ZWJ_EMOJI_PRESENTATION) &&
            c == 0x1F3F4) {
            return std::make_pair((std::int8_t)0, WidthInfo::EMOJI_PRESENTATION);
        }
        if (next_info == WidthInfo::ZWJ_EMOJI_PRESENTATION &&
            lookup_width_generic<IS_CJK>(c).second == WidthInfo::EMOJI_PRESENTATION) {
            return std::make_pair((std::int8_t)0, WidthInfo::EMOJI_PRESENTATION);
        }

        if (next_info == WidthInfo::KIRAT_RAI_VOWEL_SIGN_E) {
            if (c == 0x16D63) return std::make_pair((std::int8_t)0, WidthInfo::DEFAULT);
            if (c == 0x16D67) return std::make_pair((std::int8_t)0, WidthInfo::KIRAT_RAI_VOWEL_SIGN_AI);
            if (c == 0x16D68) return std::make_pair((std::int8_t)1, WidthInfo::KIRAT_RAI_VOWEL_SIGN_E);
            if (c == 0x16D69) return std::make_pair((std::int8_t)0, WidthInfo::DEFAULT);
        }
        if (next_info == WidthInfo::KIRAT_RAI_VOWEL_SIGN_AI && c == 0x16D63) {
            return std::make_pair((std::int8_t)0, WidthInfo::DEFAULT);
        }

        // Fallback: fall through to the plain lookup below.
    }

    std::pair<std::uint8_t, WidthInfo> ret = lookup_width_generic<IS_CJK>(c);
    return std::make_pair((std::int8_t)ret.first, ret.second);
}

#if 0
template <bool IS_CJK>
std::int64_t str_width(const std::vector<char32_t> &codepoints) {
    WidthInfo info = WidthInfo::DEFAULT;
    std::int64_t sum = 0;
    for (std::vector<char32_t>::const_reverse_iterator it = codepoints.rbegin();
         it != codepoints.rend(); ++it) {
        std::pair<std::int8_t, WidthInfo> result = width_in_generic<IS_CJK>(*it, info);
        sum += result.first;
        info = result.second;
    }
    return sum < 0 ? 0 : sum; // defensive; well-formed input shouldn't go negative
}
#endif

#if 0
template <bool IS_CJK>
std::int64_t utf8_str_width(const std::uint8_t *data, std::size_t length) {
    std::vector<char32_t> cps;
    cps.reserve(length);

    std::size_t i = 0;
    while (i < length) {
        utf8proc_int32_t cp;
        utf8proc_ssize_t consumed = utf8proc_iterate(data + i, (utf8proc_ssize_t)(length - i), &cp);
        if (consumed < 0) {
            throw std::runtime_error("invalid UTF-8 sequence");
        }
        cps.push_back((char32_t)cp);
        i += (std::size_t)(consumed > 0 ? consumed : 1);
    }
    return str_width<IS_CJK>(cps);
}
#endif

// Explicit instantiations - the only two IS_CJK values that exist.
template std::pair<std::int8_t, WidthInfo> width_in_generic<false>(char32_t, WidthInfo);
template std::pair<std::int8_t, WidthInfo> width_in_generic<true>(char32_t, WidthInfo);
#if 0
template std::int64_t str_width<false>(const std::vector<char32_t> &);
template std::int64_t str_width<true>(const std::vector<char32_t> &);
template std::int64_t utf8_str_width<false>(const std::uint8_t *, std::size_t);
template std::int64_t utf8_str_width<true>(const std::uint8_t *, std::size_t);
#endif

} // namespace unicode_width
