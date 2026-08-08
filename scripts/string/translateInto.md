<p align="right" style="font-size:10px;">
<a style="font-size:10px;" href="https://github.com/jlfaucher/executor5-bulk/blob/main/scripts/string/translateInto.md">Markdown</a><br/>
<a style="font-size:10px;" href="https://html-preview.github.io/?url=https://github.com/jlfaucher/executor5-bulk/blob/main/scripts/string/translateInto.html">HTML</a>
</p>

# String class

## translateInto

The `translateInto` method of the String class  is a generalization of the
`translate` method.

    string~translate(                  [tableo], [tablei], [pad], [n], [length]           )
    string~translateInto( destination, [tableo], [tablei], [pad], [n], [length], [refEnd] )


Translates sequences of characters into another sequence of characters.  
Appends the translated sequences of characters to `destination`.  
Currently, `destination` must be a mutable buffer, but it could be any object
supporting the`append` method at C++ level.  
The resulting string may be shorter or longer than the receiving string.

`tablei` and `tableo` can be either a string or an array.

If `tablei` is a string:

  - each character in the receiving string is searched in `tablei`.
  - If the character is found, the corresponding translation item in `tableo`
    is tried.

If `tablei` is an array, the matcher items are:

  - either a routine that may match a sequence of characters,
  - or any `non-NIL` object whose string representation is used as a sequence
    of characters for matching in the receiving string.
  - or `.NIL` which is ignored
  - or `OREF_NULL` (an omitted item in the array) which is ignored.
  - If there is a match, the corresponding translation item in `tableo` is tried.

If `tableo` is a string, the translation items are characters.

If `tableo` is an array, the translation items are:

  - either a routine that may append a sequence of characters,
  - or any `non-NIL` object whose string representation is used as a sequence
    of characters to append. A string representation is always applicable.
  - or `.NIL` to stop the translation.
  - or `OREF_NULL` (an omitted item in the array) to stop the translation.

Arguments passed to a matcher routine in `tablei`:

    string: The string that received the "translateInto" message.
    pos:    The current matching position in string (first character).

Arguments passed to a translation routine in `tableo`:

    destination: The destination of the translation.
                 The routine can append text to it.
    string:      The string that received the "translateInto" message.
    pad:         The pad string passed with the "translateInto" message, can be
                 omitted.
    pos:         The current matching position in string (first character).
    matchLength: The length of the current matched substring. The routine can
                 either return this length or return a different length.

Interpretation of the result returned by a routine:

              |    no result     | 0 or not a number | > 0 (offset of next position)
    in tablei | stop translation | no match          | match
    in tableo | stop translation | not applicable    | translation applied

If there are duplicates in `tablei`:

  - the first (leftmost if string) occurrence is selected.
  - if the corresponding translation item is a routine and this routine
    returns 0, then the next occurrence in `tablei` is selected.
    Repeat until a translation item is applicable, or until there are no more
    occurences.
  - otherwise only the first corresponding translation item is used.

If there is a match and the corresponding translation item is applicable, the
translation is appended to `destination`. The next position in the receiving
string is updated:

  - Character matching: advance by one character.
  - String matching: advance by the length of the corresponding string.
  - Matching by routine: advance by the returned number (unless overriden)
  - Translation by routine: advance by the returned number (override)

If there is no match or no applicable translation item, the current character
in the receiving string is appended to `destination`. The next position in
the receiving string is advanced by one character.

At the end of the translation, the current position in the receiving string
is returned in the optional `refEnd` variable reference.

### Example 1

```rexx
inArray = "00"x,-
          "07"x,-
          .routines~match_cntrl,-
          "<script>",-
          "<LOL>"

outArray = "0️⃣",-
           "7️⃣",-
           .routines~translate_cntrl,-
           .routines~translate_to_script_font,-     -- up to </script>
           "😂"

string = "My name is <script>Bond, James Bond</script> <LOL>! My public ID is " || "000007"x || ". My secret ID is " || "01000500020807"x || "."
translation = string~translateInto(.mutableBuffer~new, outArray, inArray)

translation == "My name is 𝓑𝓸𝓷𝓭, 𝓙𝓪𝓶𝓮𝓼 𝓑𝓸𝓷𝓭 😂! My public ID is 0️⃣0️⃣7️⃣. My secret ID is \u00010️⃣\u00050️⃣\u0002\u00087️⃣."


::routine match_cntrl
    use strict arg string, pos
    -- The returned value is not a boolean, it's a number of matched characters:
    -- 0 if no match
    -- 1 if match
    return .string~cntrl~contains(string~subchar(pos))


::routine translate_cntrl
    use strict arg buffer, string, pad=" ", pos, matchLength
    byte = string~subchar(pos)
    buffer~append("\u00")
    buffer~append(byte~c2x)
    return 1 -- advance by 1 character


::routine translate_to_script_font
    use strict arg buffer, string, pad=" ", pos, matchLength
    parse value string with "<script>" text "</script>" .
    alpha_sb = "𝓐","𝓑","𝓒","𝓓","𝓔","𝓕","𝓖","𝓗","𝓘","𝓙","𝓚","𝓛","𝓜","𝓝","𝓞","𝓟","𝓠","𝓡","𝓢","𝓣","𝓤","𝓥","𝓦","𝓧","𝓨","𝓩",-
               "𝓪","𝓫","𝓬","𝓭","𝓮","𝓯","𝓰","𝓱","𝓲","𝓳","𝓴","𝓵","𝓶","𝓷","𝓸","𝓹","𝓺","𝓻","𝓼","𝓽","𝓾","𝓿","𝔀","𝔁","𝔂","𝔃"
    text_sb = text~translateInto(buffer, alpha_sb, .string~alpha)
    return "<script>"~length + text~length + "</script>"~length
```

### Example 2

```rexx
-- Translate a JSON string, starting at position 15.
-- The next unescaped double quote will stop the translation.
-- Its position will be collected with the `endTranslation` variable passed by reference.
-- Some strings contain control characters, displayed in hexadecimal between []

inArray = '\"',-
          '\\',-
          '\/',-
          '\b',-
          '\f',-
          '\n',-
          '\r',-
          '\t',-
          '\u00',-
          '\u',-
          '"'

outArray = '"',-
           '\',-
           '/',-
           '08'x,-
           '0C'x,-
           '0A'x,-
           '0D'x,-
           '09'x,-
           .routines~translate_backslash_u00_2_hexdigits,-
           .routines~translate_backslash_u_4_hexdigits,-
           .nil /* .nil or omitted item means stop */

string = '[ { "key1" : "My name is \"Bond\", \"James Bond\".\r\nMy ID is \"007\"." }]'
cols =   '123456789012345........................................................^'
translation = string~translateInto(.mutableBuffer~new, outArray, inArray, /*pad*/, 15, /*length*/, >endTranslation)

translation == '[ { "key1" : "My name is "Bond", "James Bond".[0D0A]My ID is "007".'
endTranslation == 72


::routine translate_backslash_u00_2_hexdigits
    use strict arg buffer, string, pad=" ", pos, matchLength
    -- "\u00XX" where "\u00" is already matched
    if string~verify("0123456789ABCDEFabcdef", "NOMATCH", pos + 4, 2) == 0 then do
        hex = string[pos + 4, 2]
        if hex~length == 2 then do
            buffer~append(hex~x2c)
            return 6 -- advance by 6 characters
        end
    end
    raise user parseError array("Invalid escape sequence")


::routine translate_backslash_u_4_hexdigits
    use strict arg buffer, string, pad=" ", pos, matchLength
    -- "\uXXXX" where "\u" is already matched
    if string~verify("0123456789ABCDEFabcdef", "NOMATCH", pos + 2, 4) == 0 then do
        escapeSequence = string[pos, 6]
        if escapeSequence~length == 6 then do
            buffer~append(escapeSequence) -- keep as-is
            return 6 -- advance by 6 characters
        end
    end
    raise user parseError array("Invalid escape sequence")
```

### More examples

Script [`translateInto_test.rex`](https://github.com/jlfaucher/executor5-bulk/blob/main/scripts/string/translateInto_test.rex)  
Script output [`translateInto_test-out-ref.txt`](https://github.com/jlfaucher/executor5-bulk/blob/main/scripts/string/translateInto_test-out-ref.txt)
