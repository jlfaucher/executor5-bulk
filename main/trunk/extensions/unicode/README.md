<p align="right" style="font-size:10px;">
<a style="font-size:10px;" href="https://github.com/jlfaucher/executor5-bulk/blob/main/main/trunk/extensions/unicode/README.md">Markdown</a><br/>
<a style="font-size:10px;" href="https://html-preview.github.io/?url=https://github.com/jlfaucher/executor5-bulk/blob/main/main/trunk/extensions/unicode/README.html">HTML</a>
</p>

# Unicode support

## RexxUnicodeServices class

The `RexxUnicodeServices` class is a native class that acts as a facade for

- the [`utf8proc`][utf8proc] C library
- the [`unicode-width`][unicode_width] Rust crate ported to C++

Both are embedded in ooRexx.

Demo / Validation:

- [`RexxUnicodeServices_test.rex`][RexxUnicodeServices_test]: no dependencies; the foundations are provided by ooRexx only
- [`RexxUnicodeServices_test-out-ref.txt`][RexxUnicodeServices_test_output]: script's output


## rxunicode library

The `rxunicode` library implements the `RexxUnicode` class, a subclass of `RexxUnicodeServices`.

Demo / Validation:

- [`RexxUnicode_test.rex`][RexxUnicode_test]: depends on `RexxUnicode` (optionally using ICU4ooRexx)
- [`RexxUnicode_test-out-ref.txt`][RexxUnicode_test_output]: script's output

The `rxunicode` library provides additional classes built on the `RexxUnicodeServices` and `RexxUnicode` classes.

### Reference documentation

- [Markdown documentation][rxunicode_documentation_md]
- [HTML documentation][rxunicode_documentation_html]

### Examples

- [rxunicode documentation examples][rxunicode_documentation_examples]
- [Unicode diary examples][unicode_diary_examples]


### Technical documentation

- `utf8proc`

    - [Code review][_code_review_utf8proc_txt]  
      <!-- The NBSP after this comment is intentional; it adds spacing between bullets in the generated HTML --> 

- `unicode-width`

    - [Rust-to-C++ by Claude Sonnet 5][_Claude_summary_Rust_to_Cpp_port_md]
    - [Rules applied to determine character width][_ChatGPT_summary_Unicode_Width_Rules_html]
    - [Code review][_code_review_unicode_width_txt]


## ICU4ooRexx library

The [ICU4ooRexx][ICU4ooRexx] library wraps the `ICU4C` C/C++ API.

At present, it provides only a minimal wrapper around the ICU4C C/C++ API:

- Get a character name from a codepoint
- Get a codepoint from a character name

This library is optional.

The `RexxUnicode` class has no strong dependency on `ICU4ooRexx`, but it can take advantage of it, if `ICU4ooRexx` was loaded by the user.

If you don't have `ICU4C` installed and want to try it, follow the instructions [here][ICU4C_installation].




[_ChatGPT_summary_Unicode_Width_Rules_html]: https://github.com/jlfaucher/executor5-bulk/blob/main/main/trunk/interpreter/classes/support/Unicode/unicode-width/_ChatGPT_summary-Unicode_Width_Rules.html "_ChatGPT_summary-Unicode_Width_Rules.html"
[_ChatGPT_summary_Unicode_Width_Rules_md]: https://github.com/jlfaucher/executor5-bulk/blob/main/main/trunk/interpreter/classes/support/Unicode/unicode-width/_ChatGPT_summary-Unicode_Width_Rules.md "_ChatGPT_summary-Unicode_Width_Rules.md"
[_Claude_summary_Rust_to_Cpp_port_md]: https://github.com/jlfaucher/executor5-bulk/blob/main/main/trunk/interpreter/classes/support/Unicode/unicode-width/_Claude_summary-Rust_to_C++_port.md "_Claude_summary-Rust_to_C++_port.md"
[_code_review_unicode_width_txt]: https://github.com/jlfaucher/executor5-bulk/blob/main/main/trunk/interpreter/classes/support/Unicode/unicode-width/_code_review-unicode-width.txt "_code_review-unicode-width.txt"
[_code_review_utf8proc_txt]: https://github.com/jlfaucher/executor5-bulk/tree/main/main/trunk/interpreter/classes/support/Unicode/utf8proc/_code_review-utf8proc.txt "_code_review-utf8proc.txt"
[ICU4C_installation]: https://github.com/jlfaucher/icu4oorexx#icu4c-installation "ICU4C installation"
[ICU4ooRexx]: https://github.com/jlfaucher/icu4oorexx "ICU4ooRexx"
[RexxUnicode]: https://github.com/jlfaucher/executor5-bulk/blob/main/main/trunk/extensions/unicode/rxunicode.md#2rexxunicode-class "RexxUnicode class"
[RexxUnicode_test]: https://github.com/jlfaucher/executor5-bulk/blob/main/scripts/unicode/RexxUnicode_test.rex "RexxUnicode_test.rex"
[RexxUnicode_test_output]: https://github.com/jlfaucher/executor5-bulk/blob/main/scripts/unicode/RexxUnicode_test-out-ref.txt "RexxUnicode_test-out-ref.txt"
[RexxUnicodeServices]: https://github.com/jlfaucher/executor5-bulk/blob/main/main/trunk/extensions/unicode/rxunicode.md#1rexxunicodeservices-class "RexxUnicodeServices class"
[RexxUnicodeServices_test]: https://github.com/jlfaucher/executor5-bulk/blob/main/scripts/unicode/RexxUnicodeServices_test.rex "RexxUnicodeServices_test.rex"
[RexxUnicodeServices_test_output]: https://github.com/jlfaucher/executor5-bulk/blob/main/scripts/unicode/RexxUnicodeServices_test-out-ref.txt "RexxUnicodeServices_test-out-ref.txt"
[rxunicode_documentation_examples]: https://html-preview.github.io/?url=https://github.com/jlfaucher/executor5-bulk/blob/main/scripts/unicode/rxunicode_examples-out.html "rxunicode documentation examples"
[rxunicode_documentation_md]: https://github.com/jlfaucher/executor5-bulk/blob/main/main/trunk/extensions/unicode/rxunicode.md "rxunicode markdown documentation"
[rxunicode_documentation_html]: https://html-preview.github.io/?url=https://github.com/jlfaucher/executor5-bulk/blob/main/main/trunk/extensions/unicode/rxunicode.html "rxunicode HTML documentation"
[unicode_diary_examples]: https://html-preview.github.io/?url=https://github.com/jlfaucher/executor5-bulk/blob/main/scripts/unicode/diary_examples-out.html "Unicode diary examples"
[unicode_width]: https://github.com/unicode-rs/unicode-width "unicode-width"
[utf8proc]: https://juliastrings.github.io/utf8proc/ "utf8proc"
