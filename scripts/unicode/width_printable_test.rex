say ".RexxUnicode~codepointCharWidth returns 0 for non-printable codepoints."
say ".RexxUnicode~codepointIsPrintable returns .false for non-printable codepoints."
say "Let's see whether they agree on which codepoints are non-printable..."
say
say "Divergences:"
say "0 1      All Cn codepoints (814730)"
say "0 1      All Co codepoints (137468)"

categoryWidth. = 0

loop codepoint = 0 to .RexxUnicode~maxCodepoint
    printable = .RexxUnicode~codepointIsPrintable(codepoint)
    width = .RexxUnicode~codepointCharWidth(codepoint)
    .RexxUnicode~codepointCategory(codepoint, >categoryCode)

    -- Count the occurences of width per category
    categoryWidth.categoryCode.width += 1

    -- Cn Unassigned (reserved, noncharacters)     (width: 814730 x 1)
    -- All Cn codepoints have width == 1
    -- That's a major divergence with codepointIsPrintable
    -- Too many, not listed
    if categoryCode == "Cn" then iterate

    -- Co Private_Use                              (width: 137468 x 1)
    -- All Co codepoints have width == 1
    -- That's a major divergence with codepointIsPrintable
    -- Too many, not listed
    if categoryCode == "Co" then iterate

    -- Divergence
    if (\printable & width \== 0) | (printable & width == 0) then do
        say printable width "    " .RexxUnicodeCharacter~new(codepoint)
    end
end

say
call sayCollection categoryWidth., "categoryWidth."


::routine sayCollection
    use strict arg coll, title=(coll~defaultName), comparator=.nil, iterateOverItem=.false, surroundItemByQuotes=.true, surroundIndexByQuotes=.true, unescapedCharacters="", maxCount=(9~copies(digits())) /*no limit*/, action=.nil
    call dump2 coll, title, comparator, iterateOverItem, surroundItemByQuotes, surroundIndexByQuotes, unescapedCharacters, maxCount, action
    say


::requires "rxunicode.cls"
::requires "icu4oorexx.cls" -- optional, for character names
::requires "procedural/dispatcher.cls"  -- optional, for a less verbose collection display
::requires "rgf_util2/rgf_util2.rex"
