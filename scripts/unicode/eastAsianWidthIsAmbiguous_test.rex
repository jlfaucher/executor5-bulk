limit = 400
say "List the Unicode characters whose east asian width is ambiguous."
say "Limit =" limit "characters per width"
say
say "Some characters have an ambiguous width of 0. Why?"
say "It's not a bug, this is consistent with EastAsianWidth.txt."
say

count0 = 0
count1 = 0
count2 = 0
countOther = 0
loop codepoint = 0 to .RexxUnicode~maxCodepoint
    if .RexxUnicode~codepointEastAsianWidthIsAmbiguous(codepoint) then do
        character = .RexxUnicodeCharacter~new(codepoint)
        width = character~charWidth
        select case width
            when 0 then do
                count0 += 1
                if count0 <= limit then say character ":" width
                end
            when 1 then do
                count1 += 1
                if count1 <= limit then say character ":" width
                end
            when 2 then do
                count2 += 1
                if count2 <= limit then say character ":" width
                end
            otherwise do
                countOther += 1
                if countOther <= limit then say character ":" width
                end
        end
    end
end
say count0 "ambiguous widths 0"
say count1 "ambiguous widths 1"
say count2 "ambiguous widths 2"
say countOther "ambiguous widths other"

::requires "rxunicode.cls"
::requires "icu4oorexx.cls" -- optional, for character names

/*
AI feedback:
In the Unicode Character Database, the East_Asian_Width property has six specific values:
Ambiguous, Fullwidth, Halfwidth, Narrow, Wide, and Neutral.
Ambiguous characters are those that resolve to either wide (2 cells) or narrow (1 cell)
based on whether they are in an East Asian context or not.
Zero-width characters are format characters (General Category Cf) or combining characters
(General Category Mn or Me) that are explicitly defined to occupy 0 cells.


https://www.unicode.org/Public/UNIDATA/EastAsianWidth.txt


https://stackoverflow.com/questions/3634627/how-to-know-the-preferred-display-width-in-columns-of-unicode-characters
How to know the preferred display width (in columns) of Unicode characters?


https://www.unicode.org/Public/UNIDATA/emoji-zwj-sequences.txt
*/