testCase = 1
signal on syntax

-- Valid UTF-8 characters
1: s = "say a {= 1"; say; say "1:" s; interpret s      -- Error 13.1:  Incorrect character in program "{" ('7B'X).
2: s = "say a ¬= 1"; say; say "2:" s; interpret s      -- Error 13.1:  Incorrect character in program "¬" ('C2AC'X).
3: s = "say a €= 1"; say; say "3:" s; interpret s      -- Error 13.1:  Incorrect character in program "€" ('E282AC'X).
4: s = "say a 🦖= 1"; say; say "4:" s; interpret s     -- Error 13.1:  Incorrect character in program "🦖" ('F09FA696'X).

-- Invalid UTF-8 characters

-- Lonely start character: a valid UTF-8 start byte (0xC0) that is not followed by the required continuation bytes.
5: s = '"say a " || "C0"x || "= 1"'; say; say "5:" s; interpret "s = "|| s; interpret s    -- Error 13.1:  Incorrect character in program "?" ('C0'X).

-- Invalid single-byte sequences: 0x80 to 0xBF (these are only valid as continuation bytes, not standalone).
6: s = '"say a " || "80"x || "= 1"'; say; say "6:" s; interpret "s = " || s; interpret s    -- Error 13.1:  Incorrect character in program "?" ('80'X).

7: exit

syntax:
    call sayCondition condition("O")
    testCase += 1
    signal on syntax
    signal value testCase


::routine sayCondition
    use strict arg condition, showStackFrames = .true
    if condition == .nil then return

    traceback = condition~traceback
    stackFrames = condition~stackFrames
    if showStackFrames then call sayStackFrames stackFrames

    if condition~condition <> "SYNTAX" then .error~say(condition~condition)
    if condition~description <> .nil, condition~description <> "" then .error~say(condition~description)

    -- For SYNTAX conditions
    if condition~message <> .nil then .error~say(condition~message)
    else if condition~errortext <> .nil then .error~say(condition~errortext)
    if condition~code <> .nil then .error~say("Error code=" condition~code)


::routine sayStackFrames
    use strict arg stackFrames
    if .nil == stackFrames then return
    supplier = stackFrames~supplier
    do while supplier~available
        stackFrame = supplier~item
        .error~say(stackFrame~traceLine)
        supplier~next
    end

/*
Output on macOS:

1: say a {= 1
     5 *-* say a {
     5 *-* interpret s      -- Error 13.1:  Incorrect character in program "{" ('7B'X).
Incorrect character in program "{" ('7B'X).
Error code= 13.1

2: say a ¬= 1
     6 *-* say a ¬
     6 *-* interpret s      -- Error 13.1:  Incorrect character in program "¬" ('C2AC'X).
Incorrect character in program "¬" ('C2AC'X).
Error code= 13.1

3: say a €= 1
     7 *-* say a €
     7 *-* interpret s      -- Error 13.1:  Incorrect character in program "€" ('E282AC'X).
Incorrect character in program "€" ('E282AC'X).
Error code= 13.1

4: say a 🦖= 1
     8 *-* say a 🦖
     8 *-* interpret s     -- Error 13.1:  Incorrect character in program "🦖" ('F09FA696'X).
Incorrect character in program "🦖" ('F09FA696'X).
Error code= 13.1

5: "say a " || "C0"x || "= 1"
    13 *-* say a ?
    13 *-* interpret s    -- Error 13.1:  Incorrect character in program "?" ('C0'X).
Incorrect character in program "?" ('C0'X).
Error code= 13.1

6: "say a " || "80"x || "= 1"
    16 *-* say a ?
    16 *-* interpret s    -- Error 13.1:  Incorrect character in program "?" ('80'X).
Incorrect character in program "?" ('80'X).
Error code= 13.1

*/
