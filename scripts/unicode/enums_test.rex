say "For each enumerated type, collect the first codepoint of each enum value:"
say "- codepointBidiClass"
say "- codepointBoundClass"
say "- codepointCategory"
say "- codepointCombiningClass"
say "- codepointDecompositionType"
say

bidiClasses = .Directory~new
boundClasses = .Directory~new
categories = .Directory~new
combiningClasses = .Directory~new
decompositionTypes = .Directory~new

loop codepoint = 0 to .RexxUnicode~maxCodepoint
    .RexxUnicode~codepointBidiClass(codepoint, >code, >label)
    if \ bidiClasses~hasIndex(code) then bidiClasses[code] = .RexxUnicodeCharacter~new(codepoint)

    .RexxUnicode~codepointBoundClass(codepoint, >code, >label)
    if \ boundClasses~hasIndex(code) then boundClasses[code] = .RexxUnicodeCharacter~new(codepoint)

    .RexxUnicode~codepointCategory(codepoint, >code, >label)
    if \ categories~hasIndex(code) then categories[code] = .RexxUnicodeCharacter~new(codepoint)

    .RexxUnicode~codepointCombiningClass(codepoint, >code, >label)
    if \ combiningClasses~hasIndex(code) then combiningClasses[code] = .RexxUnicodeCharacter~new(codepoint)

    .RexxUnicode~codepointDecompositionType(codepoint, >code, >label)
    if \ decompositionTypes~hasIndex(code) then decompositionTypes[code] = .RexxUnicodeCharacter~new(codepoint)
end

call sayCollection bidiClasses, "bidiClasses"
call sayCollection boundClasses, "boundClasses"
call sayCollection categories, "categories"
call sayCollection combiningClasses, "combiningClasses"
call sayCollection decompositionTypes, "decompositionTypes"


::routine sayCollection
    use strict arg coll, title=(coll~defaultName), comparator=.nil, iterateOverItem=.false, surroundItemByQuotes=.true, surroundIndexByQuotes=.true, unescapedCharacters="", maxCount=(9~copies(digits())) /*no limit*/, action=.nil
    call dump2 coll, title, comparator, iterateOverItem, surroundItemByQuotes, surroundIndexByQuotes, unescapedCharacters, maxCount, action
    say


::requires "rxunicode.cls"
::requires "icu4oorexx.cls" -- optional, for character names
::requires "procedural/dispatcher.cls"  -- optional, for a less verbose collection display
::requires "rgf_util2/rgf_util2.rex"
