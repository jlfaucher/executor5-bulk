say "The control_boundary property is exposed by utf8proc but not used internally."
say "What's the purpose of this property?"
say

loop codepoint = 0 to .RexxUnicode~maxCodepoint
    if .RexxUnicode~codepointControlBoundary(codepoint) then say .RexxUnicodeCharacter~new(codepoint)
end

::requires "rxunicode.cls"
::requires "icu4oorexx.cls" -- optional, for character names
