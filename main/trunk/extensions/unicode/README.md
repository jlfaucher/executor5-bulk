# Unicode support

## RexxUnicodeServices class

The `RexxUnicodeServices` class is a native class that acts as a facade for the [`utf8proc`][utf8proc] library embedded in ooRexx.

Demo / Validation:

- [`RexxUnicodeServices_test.rex`][RexxUnicodeServices_test]: no dependencies; the foundations are provided by ooRexx only.
- [`RexxUnicodeServices_test-out-ref.txt`][RexxUnicodeServices_test_output]: script's output.


## rxunicode library

The `rxunicode` library implements the `RexxUnicode` class, a subclass of `RexxUnicodeServices`.

Demo / Validation:

- [`RexxUnicode_test.rex`][RexxUnicode_test]: depends on `RexxUnicode` (optionally using ICU4ooRexx).
- [`RexxUnicode_test-out-ref.txt`][RexxUnicode_test_output]: script's output.

The `rxunicode` library provides additional classes built on the `RexxUnicodeServices` and `RexxUnicode` classes.

**Reference documentation:**

- [Markdown documentation][rxunicode_documentation_md].
- [HTML documentation][rxunicode_documentation_html].

**Examples**

- [rxunicode documentation examples][rxunicode_documentation_examples].
- [Unicode diary examples][unicode_diary_examples].


## ICU4ooRexx library

The [ICU4ooRexx][ICU4ooRexx] library wraps the `ICU4C` C/C++ API.

At present, it provides only a minimal wrapper around the ICU4C C/C++ API:

- Get a character name from a codepoint.
- Get a codepoint from a character name.

This library is optional.

The `RexxUnicode` class has no strong dependency on `ICU4ooRexx`, but it can take advantage of it, if `ICU4ooRexx` was loaded by the user.

If you don't have `ICU4C` installed and want to try it, follow the instructions [here][ICU4C_installation].




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
[utf8proc]: https://juliastrings.github.io/utf8proc/ "utf8proc"
