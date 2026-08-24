Ported from Rust to C++ by `Claude Sonnet 5`.  
Project summary by `Claude Sonnet 5`.

# Porting unicode-rs/unicode-width to C++ for ooRexx — project summary

## Goal

ooRexx needs a display-width function equivalent to the Rust crate
[`unicode-width`](https://github.com/unicode-rs/unicode-width)'s
`str_width()`/`str_width_cjk()` — a UAX #11 width calculation that is
*grapheme/ligature/emoji-ZWJ-sequence aware*, not just a per-codepoint
`wcwidth`-style sum. This is a strictly richer algorithm than
`utf8proc_charwidth()` summed over codepoints (which is what an earlier,
simpler `utf8StringWidth` implementation did).

## Options considered

- **Option A (rejected as primary path):** hand-port the algorithm using
  ICU4C properties (`UCHAR_EAST_ASIAN_WIDTH`, `UCHAR_EMOJI_PRESENTATION`,
  etc.) as a substitute for unicode-width's generated tables. Produced a
  working `icu_width.hpp/.cpp` + ooRexx `RexxMethod` binding, but carried
  real fidelity risk: several `WidthInfo` bit-packing/flag-interaction
  details had to be guessed at (no ICU dependency allowed by policy: don't
  use ICU4C, `utf8proc` is fine).
- **Option B (chosen):** fork unicode-width's own Python table generator
  (`scripts/unicode.py`) into a new `unicode-cpp.py` that emits C++ instead
  of Rust. This reuses 100% of the *data derivation* logic (UCD parsing,
  table compaction, state-machine derivation) and only changes the `emit_*`
  (textual output) functions — eliminating the guesswork from Option A
  entirely, since the exact `WidthInfo` bit layout and match-arm data come
  straight from the generator itself.

## Architecture of unicode-width (as actually inspected, not guessed)

- `scripts/unicode.py`: downloads UCD files, builds a multi-level trie plus
  several small leaf tables, and has five `emit_*` functions
  (`emit_width_info`, `emit_props`, `emit_lookup`, `emit_tables`,
  `emit_tests`) that print Rust syntax around that data into `src/gen/*.rs`.
- Hand-written Rust files (`src/width_info.rs`, `src/props.rs`,
  `src/lookup.rs`) are **not** touched by the generator — they contain the
  actual control flow and needed a manual, one-time port.
- `WidthInfo` is a packed `u16` (not `u8` as first guessed). Its methods
  (`set_emoji_presentation`, `set_text_presentation`, `set_vs1_2_3`, etc.)
  have a "ligature-transparent carry" rule keyed on bit 13
  (`LIGATURE_TRANSPARENT_MASK = 0x2000`): setting a presentation-selector
  flag normally resets any in-progress "special sequence" state, *unless*
  the ligature-transparent bit is already set, in which case it's preserved
  so a combining-mark chain keeps propagating.
- `str_width()` scans the codepoint sequence **right to left**
  (`Iterator::rfold`), because `width_in_generic(c, next_info)` needs
  `next_info` — the state built up from codepoints *to the right* of `c` —
  to resolve lookahead-dependent sequences (ZWJ chains, variation
  selectors, flags) without true forward lookahead.
- `width_in_generic<const IS_CJK: bool>` returns a signed width delta
  (can be **negative**, e.g. Khmer coeng signs retract a previously-counted
  column) plus the new `WidthInfo` state, via a cascade: emoji-presentation
  carry-over → CJK-only solidus-overlay correction → control chars →
  (fast path if `next_info == DEFAULT`) → variation-selector handling →
  ligature-transparent pass-through → a large `(state, char)` match table
  covering Arabic Lam-Alef, Hebrew Alef-Lamed, Khmer coeng, Buginese,
  Tifinagh, Lisu, Old Turkic, Kirat Rai, and the full emoji ZWJ/regional-
  indicator-flag/keycap/tag-sequence state machine → fallback table lookup.

## What was built

### `unicode-cpp.py`
A fork of `scripts/unicode.py`. Per the user's explicit design constraint,
the diff against upstream is localized to renaming `emit_rust_file` →
`emit_cpp_file`, retargeting output paths, and rewriting the five `emit_*`
function bodies — all UCD-parsing/table-building code is untouched.

Key design decisions baked into the C++ output:
- **No Cargo-style "cjk" feature gate.** The Rust crate conditionally
  compiles CJK-only table rows/functions behind `#[cfg(feature = "cjk")]`.
  The C++ port always compiles both `lookup_width()` and
  `lookup_width_cjk()` (needed for the `template<bool IS_CJK>` design), so
  all `#[cfg(...)]` gating was simply dropped and tables are emitted in
  full unconditionally.
- `emit_tables` and `emit_lookup` each now write a **paired header +
  source** (`tables.hpp`/`.cpp`, `gen/lookup.hpp`/`.cpp`) instead of Rust's
  single-file-per-module output, since C++ needs declarations visible to
  other translation units.
- Rust's byte-packed `[u8; 3]` codepoint-range pairs became a plain
  `struct CodepointRange { uint32_t lo, hi; }` — same data, simpler
  representation.
- `emit_tests`'s Rust raw strings (`r#"..."#`) became escaped C++ string
  literals.

### Hand-written C++ files (`src-cpp/`)
Faithful ports of the hand-written Rust files, now C++11-compliant (see
"C++11 constraint" below):
- **`width_info.hpp`** — `WidthInfo` as a packed `uint16_t` with the same
  masks and carry-rule logic as Rust, transcribed exactly (no more
  guessing, once the real Rust source was available).
- **`props.hpp`/`props.cpp`** — declarations for the generated property
  predicates, plus the two hand-written predicates
  (`is_transparent_zero_width`, `is_solidus_transparent`) and a
  `range_pair_contains` binary-search helper.
- **`lookup.hpp`/`lookup.cpp`** — `template<bool IS_CJK>` port of
  `width_in_generic`'s full cascade, `str_width<IS_CJK>` (the
  right-to-left driving loop), and `utf8_str_width<IS_CJK>` (UTF-8 decode
  via `utf8proc_iterate`, **not** ICU). Explicit template instantiation for
  both `true`/`false` in the `.cpp`, declared `extern template` in the
  header.

## C++11 constraint (discovered mid-project, now the binding target)

ooRexx's `CMakeLists.txt` has `set(CMAKE_CXX_STANDARD 11)`. A bump to 17 has
been requested on the oorexx-devel mailing list but will take time. All
code above targets C++11:
- **No digit-separator literals** (`0b0000'1000'...`) — plain hex/binary.
- **No `constexpr` on `WidthInfo`**, since C++11 constexpr functions allow
  only a single return statement and `WidthInfo` isn't an aggregate/literal
  type usable in a `constexpr` array anyway. Dropped `constexpr` everywhere
  it isn't load-bearing (nothing here needs compile-time evaluation) rather
  than contorting the logic into ternary chains.
- **No `if constexpr`** (C++17) — replaced with plain `if`; since `IS_CJK`
  is a template non-type parameter, the compiler still folds it to one
  branch per instantiation regardless.
- **No `std::optional`** (C++17) — the one function that used it
  (`single_char_width`) was unused scaffolding and was deleted rather than
  reworked.
- **Known-but-not-yet-fixed:** the generated `lookup_width`/`lookup_width_cjk`
  bodies use `case LO ... HI:` (GCC/Clang range-case extension), which is
  not standard C++ at any version and will fail under MSVC — relevant
  because the project cross-compiles for Windows ARM/x64. Flagged, not yet
  addressed; would need `lookup_fns()` in `unicode-cpp.py` rewritten to
  emit `if`/`else if` range checks instead of `switch`.

## Build integration notes (from actual debugging)

- Real include path for `utf8proc.h` in this tree:
  `"Unicode/utf8proc/utf8proc.h"` (not `<utf8proc.h>`).
- `gen/tables_test.cpp` currently fails to build (its two `WidthInfo` test
  arrays were `constexpr`, same C++11 issue as above) and was removed from
  `CMakeLists.txt` as a stopgap. **Fixed in `unicode-cpp.py`** (arrays are
  now plain `const`, not `constexpr`) — should compile clean on the next
  regeneration; re-add to `CMakeLists.txt` then.
- The generator was run successfully by the user locally against live UCD
  data (network in this chat's sandbox can't reach `unicode.org`, so the
  generator itself was never executed here — only written and statically
  reviewed).

## Status at end of this chat

- Full build succeeded ("build ok!").
- User is about to start functional testing.
- Suggested validation strategy (not yet executed): write a throwaway Rust
  binary that dumps `width()`/`width_cjk()` for every scalar value plus a
  curated set of multi-codepoint ZWJ/flag/tag/ligature sequences, diff
  against the C++ port's output for the same inputs. More reliable than
  code review for catching orchestration-logic bugs in the hand-ported
  cascade specifically (table-level fidelity is close to guaranteed by
  construction, since both are generated from the same UCD snapshot).

## Open follow-ups

1. Fix `gen/tables_test.cpp` regeneration end-to-end (generator-side fix
   done; needs a regenerate + re-add to `CMakeLists.txt` + rebuild to
   confirm).
   --> done by the user, build ok.
2. Decide whether/when to replace the GCC/Clang `case LO ... HI:` extension
   in generated `lookup_width`/`lookup_width_cjk` with portable
   `if`/`else if` chains, ahead of the Windows ARM/x64 MSVC build.
3. Run the black-box conformance comparison against the real Rust crate
   before trusting this on adversarial emoji/ligature input.
4. Revisit whether to restore `constexpr`/C++14-ish niceties once/if
   `CMAKE_CXX_STANDARD` is bumped past 11 — not necessary for correctness,
   but was the original (reverted) design intent.

## Actions taken by the user

- Keep the original copyright and license notices in all files ported from the
  Rust crate `unicode-width`.
- Re-add `gen/tables_test.cpp` to `CMakeLists.txt`. The build is ok.
  This file is not currently part of the regression tests.
- Replace GCC/Clang range-case extension with portable `if`/`else if` chains.
  The build is ok on Windows.
- Port manually `tests/tests.rs` to ooRexx. These tests are incorporated in the
  Executor5-bulk Unicode tests. All tests are ok.
- Compare `utf8proc` widths (equivalent of `wcwidth`) with `unicode-width` results.
  There are MANY differences!
  Still to clarify by the user: why `unicode-width` differs from `utf8proc`.

        Control characters:
            utf8proc returns 0.
            unicode-width return None for char, 1 for string.
        Surrogate characters:
            utf8proc returns 0.
            unicode-width return 1.
            utf8StringWidth returns 3 because a surrogate character alone is invalid, replaced by 3 replacement characters.
        Unassigned characters:
            utf8proc returns 1 (default value).
            unicode-width returns 0, 1 or 2.
        and 2008 other differences...