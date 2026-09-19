say "/****************************************************/"
say "/* Test binary search with all storage combinations */"
say "/****************************************************/"
say

say "/*"
say "Storage combinations:"
say
say "g = grapheme"
say "c = codepoint"
say "e = error"
say "_ = separator between start and end storage"
say
say "gce_gce"
say "-------"
say "000_000     _"
say "000_001     _e"
say "000_010     _c"
say "000_011     _ce"
say "000_100     _g"
say "000_101     _ge"
say "000_110     _gc"
say "000_111     _gce"
say
say "<cut>"
say
say "cge_cge"
say "-------"
say "111_000     gce_"
say "111_001     gce_e"
say "111_010     gce_c"
say "111_011     gce_ce"
say "111_100     gce_g"
say "111_101     gce_ge"
say "111_110     gce_gc"
say "111_111     gce_gce"
say "*/"
say
say


-- The string expressions are surounded by quotes because they will be displayed as-is
call regressionTests '""'       -- 0 byte
call regressionTests '"e"'      -- 1 byte
call regressionTests '"é"'      -- 2 bytes
call regressionTests '"€"'      -- 3 bytes
call regressionTests '"🎅"'     -- 4 bytes
call regressionTests '"⦅⦅" || "F4 91"x "🎅" "F4"x "👨‍👩‍👧" "F4 91"x || "⦆⦆"'      -- Odd number of graphemes, codepoints, errors and bytes
call regressionTests '"⦅⦅" || "F4 91"x "🎅" "F4 91"x "👨‍👩‍👧" "F4 91"x || "⦆⦆"'   -- Even number of graphemes, codepoints, errors and bytes

::routine regressionTests
    use strict arg expression

    interpret "string =" expression
    stringInfo = .rexxUnicode~stringInfo(string)
    stringInfoWidth = .RexxUnicode~utf8StringWidth(stringInfo)

    estring = .RexxUnicodeServices~utf8StringEscape(string)
    qestring = quoted(estring)
    qestringWidth = .RexxUnicode~utf8StringWidth(qestring)

    width = max(stringInfoWidth, qestringWidth)

    if stringInfoWidth < width then stringInfo ||= " "~copies(width - stringInfoWidth)
    if qestringWidth < width then qestring ||= " "~copies(width - qestringWidth)

    say
    say
    say "/* " || "*"~copies(width) || " */"
    say "/* " || qestring || " */"
    say "/* " || stringInfo || " */"
    say "/* " || "*"~copies(width) || " */"

    .RexxUnicode~utf8StringInfo(string, >graphemeCount, >codepointCount, >errorCount)

    -- Storage limits that are tested:
    -- 0, 1, one-quarter, half, three-quarter, full

    -- Use floor for start, ceiling for end, to have more items on the end side.

    oneQuarterThreeQuarterLimits = -
        (graphemeCount / 4)~floor, -        -- startOneQuarterGraphemeLimit
        (codepointCount / 4)~floor, -       -- startOneQuarterCodepointLimit
        (errorCount / 4)~floor, -           -- startOneQuarterErrorLimit
        (graphemeCount *3 / 4)~ceiling, -   -- endThreeQuarterGraphemeLimit
        (codepointCount *3 / 4)~ceiling, -  -- endThreeQuarterCodepointLimit
        (errorCount *3 / 4)~ceiling         -- endThreeQuarterErrorLimit

    halfLimits = -
        (graphemeCount / 2)~floor, -        -- startHalfGraphemeLimit
        (codepointCount / 2)~floor, -       -- startHalfCodepointLimit
        (errorCount / 2)~floor, -           -- startHalfErrorLimit
        (graphemeCount / 2)~ceiling, -      -- endHalfGraphemeLimit
        (codepointCount / 2)~ceiling, -     -- endHalfCodepointLimit
        (errorCount / 2)~ceiling            -- endHalfErrorLimit

    threeQuarterOneQuarterLimits = -
        (graphemeCount *3 / 4)~floor, -     -- startThreeQuarterGraphemeLimit
        (codepointCount *3 / 4)~floor, -    -- startThreeQuarterCodepointLimit
        (errorCount *3 / 4)~floor, -        -- startThreeQuarterErrorLimit
        (graphemeCount / 4)~ceiling, -      -- endOneQuarterGraphemeLimit
        (codepointCount / 4)~ceiling, -     -- endOneQuarterCodepointLimit
        (errorCount / 4)~ceiling            -- endOneQuarterErrorLimit

    fullLimits = -
        graphemeCount, -                    -- startFullGraphemeLimit
        codepointCount, -                   -- startFullCodepointLimit
        errorCount, -                       -- startFullErrorLimit
        graphemeCount, -                    -- endFullGraphemeLimit
        codepointCount, -                   -- endFullCodepointLimit
        errorCount                          -- endFullErrorLimit

    allLimits = -
        (1, 1, 1, 1, 1, 1), -
        oneQuarterThreeQuarterLimits, -
        halfLimits, -
        threeQuarterOneQuarterLimits, -
        fullLimits

    counter1 = 0
    do sg = 0 to 1
        do sc = 0 to 1
            do se = 0 to 1
                do eg = 0 to 1
                    do ec = 0 to 1
                        do ee = 0 to 1

                            counter1 += 1

                            counter2 = 0
                            selectedLimits = selectLimits(allLimits, sg, sc, se, eg, ec, ee)
                            do limits over selectedLimits

                                do incremental = 0 to 1
                                    incrementalText = incremental~?("Incremental", "Non-incremental")
                                    counter2 += 1
                                    counter3 = 0

                                    say
                                    say
                                    counter3 += 1
                                    say "--" counter1"."counter2"."counter3
                                    say "--" storageInfo(sg, sc, se, eg, ec, ee) incrementalText "forward binary search codepoints"
                                    indexer = newIndexer(expression, string, limits, incremental)
                                    say "do i = 1 to indexer~string~length; indexer~codepointIndexC(i); end"
                                         do i = 1 to indexer~string~length; indexer~codepointIndexC(i); end
                                    call displayIndexerInfo indexer

                                    say
                                    say
                                    counter3 += 1
                                    say "--" counter1"."counter2"."counter3
                                    say "--" storageInfo(sg, sc, se, eg, ec, ee) incrementalText "backward binary search codepoints"
                                    indexer = newIndexer(expression, string, limits, incremental)
                                    say "do i = indexer~string~length to 1 by -1; indexer~codepointIndexC(i); end"
                                         do i = indexer~string~length to 1 by -1; indexer~codepointIndexC(i); end
                                    call displayIndexerInfo indexer

                                    say
                                    say
                                    counter3 += 1
                                    say "--" counter1"."counter2"."counter3
                                    say "--" storageInfo(sg, sc, se, eg, ec, ee) incrementalText "forward binary search graphemes"
                                    indexer = newIndexer(expression, string, limits, incremental)
                                    say "do i = 1 to indexer~string~length; indexer~graphemeIndexG(i); end"
                                         do i = 1 to indexer~string~length; indexer~graphemeIndexG(i); end
                                    call displayIndexerInfo indexer

                                    say
                                    say
                                    counter3 += 1
                                    say "--" counter1"."counter2"."counter3
                                    say "--" storageInfo(sg, sc, se, eg, ec, ee) incrementalText "backward binary search graphemes"
                                    indexer = newIndexer(expression, string, limits, incremental)
                                    say "do i = indexer~string~length to 1 by -1; indexer~graphemeIndexG(i); end"
                                         do i = indexer~string~length to 1 by -1; indexer~graphemeIndexG(i); end
                                    call displayIndexerInfo indexer

                                end
                            end
                        end
                    end
                end
            end
        end
    end


::routine storageInfo
    use strict arg sg, sc, se, eg, ec, ee

    info = ""

    -- Start storage
    if sg > 0 then info ||= "g"
    if sc > 0 then info ||= "c"
    if se > 0 then info ||= "e"

    info ||= "_"

    -- End storage
    if eg > 0 then info ||= "g"
    if ec > 0 then info ||= "c"
    if ee > 0 then info ||= "e"

    return quoted(info)


::routine selectLimits
    use strict arg allLimits, sg, sc, se, eg, ec, ee

    say
    say
    say "-- allLimits (calculated once):"
    do limit over allLimits
        say "--" ppRepresentation(limit)
    end

    if sg + sc + se + eg + ec + ee == 0 then do
        limits = .array~of( (0, 0, 0, 0, 0, 0) )
    end
    else do
        limits = .array~new
        limitsSet = .set~new
        limitsSet~put(ppRepresentation((0, 0, 0, 0, 0, 0))) -- don't select this test case, doubling.
        -- Do not select doublings
        do limit over allLimits
            /*
            -- Don't do that!
            -- I keep this code to remember how stupid I am.
            -- This code modify the sg, sc, se, eg, ec, ee variables at each iteration!
            -- Shame on me!

            sg = sg * limit[1]
            sc = sc * limit[2]
            se = se * limit[3]
            eg = eg * limit[4]
            ec = ec * limit[5]
            ee = ee * limit[6]
            limit = sg, sc, se, eg, ec, ee
            */

            /*
            -- Don't do that!
            -- I keep this code to remember how stupid I am.
            -- I found this to be a clever way to multiply the limits, but...
            -- This code modify the allLimits array created at the begining of the script!
            -- Shame on me!

            limit[1] *= sg
            limit[2] *= sc
            limit[3] *= se
            limit[4] *= eg
            limit[5] *= ec
            limit[6] *= ee
            */

            -- At last! This code is correct, no side effect...
            sgl = sg * limit[1]
            scl = sc * limit[2]
            sel = se * limit[3]
            egl = eg * limit[4]
            ecl = ec * limit[5]
            eel = ee * limit[6]
            limit = sgl, scl, sel, egl, ecl, eel

            ppLimit = ppRepresentation(limit)
            if limitsSet~hasIndex(ppLimit) then iterate -- already selected
            limitsSet~put(ppLimit)
            limits~append(limit)
        end
    end

    say
    say
    say "-- Selected limits for the test case" storageInfo(sg, sc, se, eg, ec, ee)
    do limit over limits
        say "--" ppRepresentation(limit)
    end
    return limits


::routine newIndexer
    use strict arg expression, string, limits, incremental
    sg = limits[1]
    sc = limits[2]
    se = limits[3]
    eg = limits[4]
    ec = limits[5]
    ee = limits[6]
    say "indexer = .RexxUnicodeStringIndexer~new("expression"," sg"," sc"," se"," eg"," ec"," ee"," incremental~?(".true", ".false")")"
    return .RexxUnicodeStringIndexer~new(string, sg, sc, se, eg, ec, ee, incremental)


::routine ppRepresentation
    use strict arg value
    if .ExtensionDispatcher~isA(.class), .ExtensionDispatcher~hasMethod(value, "ppRepresentation") then return .ExtensionDispatcher~ppRepresentation(value)
    return value~string


::routine displayCollection
    use strict arg value
    if .nil == value then say "--" value
    else if \ value~isa(.Collection) then say "--" value
    else do
        if value~items == 0 then say "--" ppRepresentation(value)
        else do
            say
            say "/*"
            call dump2 value
            say "*/"
        end
    end


::routine displayIndexerInfo
    use strict arg indexer
    say "indexer~startGraphemeIndexes= --" ppRepresentation(indexer~startGraphemeIndexes)
    say "indexer~startCodepointIndexes= --" ppRepresentation(indexer~startCodepointIndexes)
    call charout , "indexer~startErrorMessages== "; call displayCollection indexer~startErrorMessages
    say "indexer~endGraphemeIndexes= --" ppRepresentation(indexer~endGraphemeIndexes)
    say "indexer~endCodepointIndexes= --" ppRepresentation(indexer~endCodepointIndexes)
    call charout , "indexer~endErrorMessages== "; call displayCollection indexer~endErrorMessages


::requires "rxunicode.cls"
::requires "procedural/dispatcher.cls"
::requires "rgf_util2.rex"


/*
Fixed:

-- indexer = .RexxUnicodeStringIndexer~new("⦅⦅" || "F4 91"x "🎅" "F4 91"x "👨‍👩‍👧" "F4 91"x || "⦆⦆", 0, 1, 0, 1, 0, 0, .true)
  1764 *-*             supplierIndexG = startGraphemeIndexes~items + 1
  1882 *-*             item = self~incrementalCodepointIndexB(indexC)
   137 *-*                   indexer~codepointIndexC(i)
    41 *-* call regressionTests '"⦅⦅" || "F4 91"x "🎅" "F4 91"x "👨‍👩‍👧" "F4 91"x || "⦆⦆"'
Error 97 running /Users/Shared/local/rexx/oorexx/build/executor5-bulk.main/main/trunk/macos-arm64/clang/release/delivery/bin/rxunicode.cls line 1764:  Object method not found.
Error 97.1:  Object "The NIL object" does not understand message "ITEMS".

*/