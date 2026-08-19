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

#include "props.hpp"
#include "gen/tables.hpp"
#include "gen/lookup.hpp"

namespace unicode_width {

bool range_pair_contains(const RangePair *ranges, std::size_t len, std::uint8_t v) {
    std::size_t lo = 0, hi = len;
    while (lo < hi) {
        std::size_t mid = lo + (hi - lo) / 2;
        if (v < ranges[mid].lo) {
            hi = mid;
        } else if (v > ranges[mid].hi) {
            lo = mid + 1;
        } else {
            return true;
        }
    }
    return false;
}

// Ported 1:1 from src/props.rs's is_transparent_zero_width(): a character
// is zero-width-and-transparent unless it appears in the generated
// NON_TRANSPARENT_ZERO_WIDTHS exception list.
bool is_transparent_zero_width(char32_t c) {
    if (lookup_width(c).first != 0) {
        // Not zero-width
        return false;
    }
    std::uint32_t cp = (std::uint32_t)c;
    std::size_t lo = 0, hi = NON_TRANSPARENT_ZERO_WIDTHS_LEN;
    while (lo < hi) {
        std::size_t mid = lo + (hi - lo) / 2;
        const auto &entry = NON_TRANSPARENT_ZERO_WIDTHS[mid];
        if (cp < entry.lo) {
            hi = mid;
        } else if (cp > entry.hi) {
            lo = mid + 1;
        } else {
            return false; // found in the exception list -> not transparent
        }
    }
    return true;
}

// Ported 1:1 from src/props.rs's is_solidus_transparent() (CJK-only in the
// Rust crate, behind `#[cfg(feature = "cjk")]`).
bool is_solidus_transparent(char32_t c) {
    if (is_ligature_transparent(c)) {
        return true;
    }
    std::uint32_t cp = (std::uint32_t)c;
    std::size_t lo = 0, hi = SOLIDUS_TRANSPARENT_LEN;
    while (lo < hi) {
        std::size_t mid = lo + (hi - lo) / 2;
        const auto &entry = SOLIDUS_TRANSPARENT[mid];
        if (cp < entry.lo) {
            hi = mid;
        } else if (cp > entry.hi) {
            lo = mid + 1;
        } else {
            return true;
        }
    }
    return false;
}

} // namespace unicode_width
