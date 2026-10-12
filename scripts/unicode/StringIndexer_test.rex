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

counters. = 0
.local~counters = counters.

if arg(1) < 1 | arg(1) > 8 then do
    say "Invalid argument, expected 1..8, got" quoted(arg(1))
    return
end

.local~counters["regressionTests"] = arg(1) - 1

-- The string expressions are surounded by quotes because they will be displayed as-is
if arg(1) == 1 then call regressionTests '""'       -- 0 byte
if arg(1) == 2 then call regressionTests '"e"'      -- 1 byte
if arg(1) == 3 then call regressionTests '"é"'      -- 2 bytes
if arg(1) == 4 then call regressionTests '"€"'      -- 3 bytes
if arg(1) == 5 then call regressionTests '"🎅"'     -- 4 bytes
if arg(1) == 6 then call regressionTests '"F4"x "👨‍👩‍👧" "91"x'     -- starts and ends on error
if arg(1) == 7 then call regressionTests '"⦅⦅" || "F4 91"x "🎅" "F4"x "👨‍👩‍👧" "F4 91"x || "⦆⦆"'      -- Odd number of graphemes, codepoints, errors and bytes
if arg(1) == 8 then call regressionTests '"⦅⦅" || "F4 91"x "🎅" "F4 91"x "👨‍👩‍👧" "F4 91"x || "⦆⦆"'   -- Even number of graphemes, codepoints, errors and bytes

-- return
say
call charout , "counters."
call displayCollection .local~counters


::routine regressionTests
    .local~counters["regressionTests"] += 1
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
        (1, 1, errorCount, 1, 1, 1), -
        oneQuarterThreeQuarterLimits, -
        oneQuarterThreeQuarterLimits~copy~~put(errorCount, 3), -
        halfLimits, -
        halfLimits~copy~~put(errorCount, 3), -
        threeQuarterOneQuarterLimits, -
        threeQuarterOneQuarterLimits~copy~~put(errorCount, 3), -
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
                                    say "--" .local~counters["regressionTests"]"."counter1"."counter2"."counter3
                                    say "--" storageInfo(sg, sc, se, eg, ec, ee) incrementalText "forward binary search codepoints"
                                    indexer = newIndexer(expression, string, limits, incremental)
                                    call displayIndexerInitializationInfo indexer
                                    say "do i = 1 to indexer~string~length; indexer~codepointIndexC(i); end"
                                         do i = 1 to indexer~string~length; indexer~codepointIndexC(i); end
                                    call displayIndexerInfo indexer

                                    say
                                    say
                                    counter3 += 1
                                    say "--" .local~counters["regressionTests"]"."counter1"."counter2"."counter3
                                    say "--" storageInfo(sg, sc, se, eg, ec, ee) incrementalText "backward binary search codepoints"
                                    indexer = newIndexer(expression, string, limits, incremental)
                                    call displayIndexerInitializationInfo indexer
                                    say "do i = indexer~string~length to 1 by -1; indexer~codepointIndexC(i); end"
                                         do i = indexer~string~length to 1 by -1; indexer~codepointIndexC(i); end
                                    call displayIndexerInfo indexer

                                    say
                                    say
                                    counter3 += 1
                                    say "--" .local~counters["regressionTests"]"."counter1"."counter2"."counter3
                                    say "--" storageInfo(sg, sc, se, eg, ec, ee) incrementalText "forward binary search graphemes"
                                    indexer = newIndexer(expression, string, limits, incremental)
                                    call displayIndexerInitializationInfo indexer
                                    say "do i = 1 to indexer~string~length; indexer~graphemeIndexG(i); end"
                                         do i = 1 to indexer~string~length; indexer~graphemeIndexG(i); end
                                    call displayIndexerInfo indexer

                                    say
                                    say
                                    counter3 += 1
                                    say "--" .local~counters["regressionTests"]"."counter1"."counter2"."counter3
                                    say "--" storageInfo(sg, sc, se, eg, ec, ee) incrementalText "backward binary search graphemes"
                                    indexer = newIndexer(expression, string, limits, incremental)
                                    call displayIndexerInitializationInfo indexer
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
    .local~counters["newIndexer"] += 1
    use strict arg expression, string, limits, incremental
    sg = limits[1]
    sc = limits[2]
    se = limits[3]
    eg = limits[4]
    ec = limits[5]
    ee = limits[6]
    if incremental then do
        -- In incremental mode, the error limits are boolean-ish (zero --> false, non-zero --> true)
        say "indexer = .RexxUnicodeStringIndexer~newIncremental("expression"," sg"," sc"," se" \== 0," eg"," ec"," ee" \== 0)"
        return .RexxUnicodeStringIndexer~newIncremental(string, sg, sc, se \== 0, eg, ec, ee \== 0)
    end
    else do
        say "indexer = .RexxUnicodeStringIndexer~new("expression"," sg"," sc"," se"," eg"," ec"," ee")"
        return .RexxUnicodeStringIndexer~new(string, sg, sc, se, eg, ec, ee)
    end


::routine ppRepresentation
    use strict arg value
    if .ExtensionDispatcher~isA(.class), .ExtensionDispatcher~hasMethod(value, "ppRepresentation") then return .ExtensionDispatcher~ppRepresentation(value)
    return value~string


::routine displayCollection
    use strict arg value
    if .nil == value then say " --" value
    else if \ value~isa(.Collection) then say " --" value
    else do
        if value~items == 0 then say " --" ppRepresentation(value)
        else do
            say "="
            say "/*"
            call dump2 value
            say "*/"
        end
    end


::routine displayIndexerInitializationInfo
    use strict arg indexer
    say "indexer~limits= --" ppRepresentation(indexer~limits)
    say "indexer~startGraphemeIndexes= --" ppRepresentation(indexer~startGraphemeIndexes)
    say "indexer~startCodepointIndexes= --" ppRepresentation(indexer~startCodepointIndexes)
    call charout , "indexer~startErrorMessages="; call displayCollection indexer~startErrorMessages
    say "indexer~endGraphemeIndexes= --" ppRepresentation(indexer~endGraphemeIndexes)
    say "indexer~endCodepointIndexes= --" ppRepresentation(indexer~endCodepointIndexes)
    call charout , "indexer~endErrorMessages="; call displayCollection indexer~endErrorMessages

    call invariant_i0sg_non_incremental indexer
    call invariant_i0sc_non_incremental indexer
    call invariant_i0eg_non_incremental indexer
    call invariant_i0ec_non_incremental indexer
    call invariant_i3_non_incremental indexer
    call invariant_i4_non_incremental indexer

    call invariant_i0sg_incremental indexer
    call invariant_i0sc_incremental indexer
    call invariant_i0eg_incremental indexer
    call invariant_i0ec_incremental indexer
    call invariant_i1_incremental indexer, /*atEndOfInitialization:*/ .true
    call invariant_i2_incremental indexer, /*atEndOfInitialization:*/ .true
    call invariant_i3_incremental indexer
    call invariant_i4_incremental indexer


::routine displayIndexerInfo
    use strict arg indexer
    say "indexer~startGraphemeIndexes= --" ppRepresentation(indexer~startGraphemeIndexes)
    say "indexer~startCodepointIndexes= --" ppRepresentation(indexer~startCodepointIndexes)
    call charout , "indexer~startErrorMessages="; call displayCollection indexer~startErrorMessages
    say "indexer~endGraphemeIndexes= --" ppRepresentation(indexer~endGraphemeIndexes)
    say "indexer~endCodepointIndexes= --" ppRepresentation(indexer~endCodepointIndexes)
    call charout , "indexer~endErrorMessages="; call displayCollection indexer~endErrorMessages

    call invariant_i0sg_non_incremental indexer
    call invariant_i0sc_non_incremental indexer
    call invariant_i0eg_non_incremental indexer
    call invariant_i0ec_non_incremental indexer
    call invariant_i3_non_incremental indexer
    call invariant_i4_non_incremental indexer

    call invariant_i1_incremental indexer, /*atEndOfInitialization:*/ .false
    call invariant_i2_incremental indexer, /*atEndOfInitialization:*/ .false
    call invariant_i3_incremental indexer
    call invariant_i4_incremental indexer


--------------------------------------------------------------------------------
-- Invariants
--------------------------------------------------------------------------------

::routine negativeItems
    use strict arg array
    if .nil == array then return 0
    count = 0
    do item over array
        if item < 0 then count += 1
    end
    return count


/*
non-incremental
i0sg startGraphemeIndexes~items = min(startGraphemeStorageLimit, graphemeCount)
*/
::routine invariant_i0sg_non_incremental
    use strict arg indexer

    if indexer~isIncremental then return
    .local~counters["invariant_i0sg_non_incremental"] += 1

    startGraphemeIndexes = indexer~startGraphemeIndexes
    startGraphemeStorageLimit = indexer~limits[1]
    graphemeCount = indexer~graphemeCount

    invariant = startGraphemeIndexes~items == min(startGraphemeStorageLimit, graphemeCount)
    if invariant == .false then do
        say "invariant_i0sg_non_incremental failed"
        say startGraphemeIndexes~items" == min("startGraphemeStorageLimit", "graphemeCount")"
    end


/*
incremental
i0sg startGraphemeIndexes~items = min(startGraphemeStorageLimit, graphemeCount)
*/
::routine invariant_i0sg_incremental
    use strict arg indexer

    if \ indexer~isIncremental then return
    .local~counters["invariant_i0sg_incremental"] += 1

    startGraphemeIndexes = indexer~startGraphemeIndexes
    startGraphemeStorageLimit = indexer~limits[1]
    graphemeCount = indexer~graphemeCount

    invariant = startGraphemeIndexes~items == min(startGraphemeStorageLimit, graphemeCount)
    if invariant == .false then do
        say "invariant_i0sg_incremental failed"
        say startGraphemeIndexes~items" == min("startGraphemeStorageLimit", "graphemeCount")"
    end


/*
non-incremental
i0sc startCodepointIndexes~items = min(startCodepointStorageLimit, codepointCount)
*/
::routine invariant_i0sc_non_incremental
    use strict arg indexer

    if indexer~isIncremental then return
    .local~counters["invariant_i0sc_non_incremental"] += 1

    startCodepointIndexes = indexer~startCodepointIndexes
    startCodepointStorageLimit = indexer~limits[2]
    codepointCount = indexer~codepointCount

    invariant = startCodepointIndexes~items == min(startCodepointStorageLimit, codepointCount)
    if invariant == .false then do
        say "invariant_i0sc_non_incremental failed"
        say startCodepointIndexes~items" == min("startCodepointStorageLimit", "codepointCount")"
    end


/*
incremental
i0sc startCodepointIndexes~items = min(startCodepointStorageLimit, codepointCount)
*/
::routine invariant_i0sc_incremental
    use strict arg indexer

    if \ indexer~isIncremental then return
    .local~counters["invariant_i0sc_incremental"] += 1

    startCodepointIndexes = indexer~startCodepointIndexes
    startCodepointStorageLimit = indexer~limits[2]
    codepointCount = indexer~codepointCount

    invariant = startCodepointIndexes~items == min(startCodepointStorageLimit, codepointCount)
    if invariant == .false then do
        say "invariant_i0sc_incremental failed"
        say startCodepointIndexes~items" == min("startCodepointStorageLimit", "codepointCount")"
    end


/*
non-incremental
i0eg endGraphemeIndexes~items = min(endGraphemeStorageLimit, graphemeCount)
*/
::routine invariant_i0eg_non_incremental
    use strict arg indexer

    if indexer~isIncremental then return
    .local~counters["invariant_i0eg_non_incremental"] += 1

    endGraphemeIndexes = indexer~endGraphemeIndexes
    endGraphemeStorageLimit = indexer~limits[4]
    graphemeCount = indexer~graphemeCount

    invariant = endGraphemeIndexes~items == min(endGraphemeStorageLimit, graphemeCount)
    if invariant == .false then do
        say "invariant_i0eg_non_incremental failed"
        say endGraphemeIndexes~items" == min("endGraphemeStorageLimit", "graphemeCount")"
    end


/*
incremental
i0eg endGraphemeIndexes~items = min(endGraphemeStorageLimit, graphemeCount)
*/
::routine invariant_i0eg_incremental
    use strict arg indexer

    if \ indexer~isIncremental then return
    .local~counters["invariant_i0eg_incremental"] += 1

    endGraphemeIndexes = indexer~endGraphemeIndexes
    endGraphemeStorageLimit = indexer~limits[4]
    graphemeCount = indexer~graphemeCount

    invariant = endGraphemeIndexes~items == min(endGraphemeStorageLimit, graphemeCount)
    if invariant == .false then do
        say "invariant_i0eg_incremental failed"
        say endGraphemeIndexes~items" == min("endGraphemeStorageLimit", "graphemeCount")"
    end


/*
non-incremental
i0ec endCodepointIndexes~items = min(endCodepointStorageLimit, codepointCount)
*/
::routine invariant_i0ec_non_incremental
    use strict arg indexer

    if indexer~isIncremental then return
    .local~counters["invariant_i0ec_non_incremental"] += 1

    endCodepointIndexes = indexer~endCodepointIndexes
    endCodepointStorageLimit = indexer~limits[5]
    codepointCount = indexer~codepointCount

    invariant = endCodepointIndexes~items == min(endCodepointStorageLimit, codepointCount)
    if invariant == .false then do
        say "invariant_i0ec_non_incremental failed"
        say endCodepointIndexes~items" == min("endCodepointStorageLimit", "codepointCount")"
    end


/*
incremental
i0ec endCodepointIndexes~items = min(endCodepointStorageLimit, codepointCount)
*/
::routine invariant_i0ec_incremental
    use strict arg indexer

    if \ indexer~isIncremental then return
    .local~counters["invariant_i0ec_incremental"] += 1

    endCodepointIndexes = indexer~endCodepointIndexes
    endCodepointStorageLimit = indexer~limits[5]
    codepointCount = indexer~codepointCount

    invariant = endCodepointIndexes~items == min(endCodepointStorageLimit, codepointCount)
    if invariant == .false then do
        say "invariant_i0ec_incremental failed"
        say endCodepointIndexes~items" == min("endCodepointStorageLimit", "codepointCount")"
    end


/*
incremental
i1 When the start codepoint storage does not hold all codepoints
(startCodepointStorageCount < codepointCount), it ends on a grapheme boundary.
*/
::routine invariant_i1_incremental
    use strict arg indexer, atEndOfInitialization

    if \ indexer~isIncremental then return

    startGraphemeStorageEnabled = indexer~limits[1] \== 0
    startGraphemeStorageCount = indexer~startGraphemeStorageCount
    startGraphemeIndexes = indexer~startGraphemeIndexes

    codepointCount = indexer~codepointCount

    startCodepointStorageEnabled = indexer~limits[2] \== 0
    startCodepointStorageCount = indexer~startCodepointStorageCount
    startCodepointIndexes = indexer~startCodepointIndexes

    endCodepointStorageCount = indexer~endCodepointStorageCount
    endGraphemeStorageCount = indexer~endGraphemeStorageCount

    -- If nothing is stored, it's either because both storages are disabled or because the string is empty
    if startGraphemeStorageCount == 0, startCodepointStorageCount == 0 then do
        -- Nothing stored
        .local~counters["invariant_i1_incremental #0"] += 1
        if \ startGraphemeStorageEnabled & \ startCodepointStorageEnabled then return -- both storages are disabled
        if indexer~string~length == 0 then return -- the string is empty
        signal failed_0
    end

    -- Here, at least one storage is not empty
    -- if both storages are enabled then both storages are not empty
    if startGraphemeStorageEnabled & startCodepointStorageEnabled then do
        .local~counters["invariant_i1_incremental #1"] += 1
        invariant = startGraphemeStorageCount \== 0 & startCodepointStorageCount \== 0
        if \ invariant then signal failed_1
    end

    if startCodepointStorageCount == codepointCount then do
        -- The start codepoint storage holds all codepoints
        if startGraphemeStorageEnabled & endGraphemeStorageCount == 0 then do
            -- The following invariants can be true only if all the stored graphemes are currently in the start grapheme storage
            .local~counters["invariant_i1_incremental #2"] += 1
            -- The last start codepoint index is the index of the last codepoint in the last start grapheme.
            -- not true if the start grapheme storage is one grapheme behind the start codepoint storage.
            lastStartGrapheme = indexer~graphemeAtIndexG(startGraphemeIndexes~items)
            lastCodepointIndex_in_lastStartGrapheme = .RexxUnicodeStringIndexer~new(lastStartGrapheme, 0, 0, 0, 0, 1, 0)~endCodepointIndexes[1]
            invariant = abs(startCodepointIndexes~lastItem) == abs(startGraphemeIndexes~lastItem) + abs(lastCodepointIndex_in_lastStartGrapheme) - 1
            if \ invariant then do
                -- since the previous invariant is false, we are in the case "one grapheme behind".
                -- The last but first start codepoint index is the index of the last codepoint in the last start grapheme
                -- Example:
                    -- 6.62.16.2
                    -- "gce_ge" Incremental backward binary search codepoints
                    -- indexer = .RexxUnicodeStringIndexer~newIncremental("F4"x "👨‍👩‍👧" "91"x, 3, 6, 2 \== 0, 2, 0, 1 \== 0)
                lastButFirstIndex = startCodepointIndexes~items - 1
                invariant = abs(startCodepointIndexes[lastButFirstIndex]) == abs(startGraphemeIndexes~lastItem) + abs(lastCodepointIndex_in_lastStartGrapheme) - 1
                if \ invariant then signal failed_2
            end
        end
    end
    else do
        -- The start codepoint storage does not hold all codepoints
        if (atEndOfInitialization | startGraphemeStorageEnabled) & (startCodepointStorageCount >= 2 & endCodepointStorageCount == 0) then do
            -- If not at end of initialization, the following invariant is true only if the graphemes are scanned and no codepoint is stored in the end storage
            .local~counters["invariant_i1_incremental #3"] += 1
            -- The start codepoint storage ends on a grapheme boundary
            state = 0 -- maybe too simplistic, will see
            codepoint1 = indexer~codepointAtIndexC(startCodepointIndexes~items - 1)     -- last but one start codepoint
            codepoint2 = indexer~codepointAtIndexC(startCodepointIndexes~items)         -- last start codepoint
            invariant = .RexxUnicode~graphemeBreak(codepoint1, codepoint2, >state)
            if \ invariant then signal failed_3
        end
    end

    return

    failed_0:
        say "invariant_i1_incremental #0 failed"
        say startGraphemeStorageEnabled" & "startCodepointStorageEnabled
        say indexer~string~length
        return
    failed_1:
        say "invariant_i1_incremental #1 failed"
        say startGraphemeStorageCount" \== 0 & "startCodepointStorageCount" \== 0"
        return
    failed_2:
        say "invariant_i1_incremental #2 failed"
        say atEndOfInitialization
        say "abs("startCodepointIndexes~lastItem") == abs("startGraphemeIndexes~lastItem") + abs("lastCodepointIndex_in_lastStartGrapheme") - 1"
        return
    failed_3:
        say "invariant_i1_incremental #3 failed"
        say atEndOfInitialization
        say ".RexxUnicode~graphemeBreak("codepoint1", "codepoint2", >state) == .false"
        return


/*
i2 When the start and end codepoint storages together do not hold all codepoints
(startCodepointStorageCount + endCodepointStorageCount < codepointCount),
the end codepoint storage ends on a grapheme boundary.
*/
::routine invariant_i2_incremental
    use strict arg indexer, atEndOfInitialization

    if \ indexer~isIncremental then return

    startGraphemeStorageCount = indexer~startGraphemeStorageCount

    endGraphemeStorageEnabled = indexer~limits[4] \== 0
    endGraphemeStorageCount = indexer~endGraphemeStorageCount
    endGraphemeIndexes = indexer~endGraphemeIndexes

    codepointCount = indexer~codepointCount

    startCodepointStorageCount = indexer~startCodepointStorageCount

    endCodepointStorageEnabled = indexer~limits[5] \== 0
    endCodepointStorageCount = indexer~endCodepointStorageCount
    endCodepointIndexes = indexer~endCodepointIndexes

    -- If nothing is stored, it's either because both storages are disabled or because the string is empty
    if endGraphemeStorageCount == 0, endCodepointStorageCount == 0 then do
        -- Nothing stored
        .local~counters["invariant_i2_incremental #0"] += 1
        if \ endGraphemeStorageEnabled & \ endCodepointStorageEnabled then return -- both storages are disabled
        if indexer~string~length == 0 then return -- the string is empty
        signal failed_0
    end

    -- Here, at least one storage is not empty
    -- if both storages are enabled then both storages are not empty
    if endGraphemeStorageEnabled & endCodepointStorageEnabled then do
        .local~counters["invariant_i2_incremental #1"] += 1
        invariant = endGraphemeStorageCount \== 0 & endCodepointStorageCount \== 0
        if \ invariant then signal failed_1
    end

    if startCodepointStorageCount + endCodepointStorageCount == codepointCount then do
        -- The start and end codepoint storages together hold all codepoints
        -- No invariant here, the codepoint storage can end in the middle of a grapheme
    end
    else do
        -- The start and end codepoint storages together do not hold all codepoints
        -- Here, no test atEndOfInitialization because the backward scan can use either a reverse grapheme supplier or a reverse codepoint supplier
        if endGraphemeStorageEnabled & (endCodepointStorageCount >= 2 & startCodepointStorageCount == 0) then do
            -- The following invariant is true only if the graphemes are scanned and no codepoint is stored in the start storage
            .local~counters["invariant_i2_incremental #3"] += 1
            -- The end codepoint storage ends on a grapheme boundary
            state = 0 -- maybe too simplistic, will see
            codepoint1 = indexer~codepointAtIndexC(-endCodepointIndexes~items)          -- last end codepoint
            codepoint2 = indexer~codepointAtIndexC(-(endCodepointIndexes~items - 1))    -- last but one end codepoint
            invariant = .RexxUnicode~graphemeBreak(codepoint1, codepoint2, >state)
            if \ invariant then signal failed_3
        end
    end

    return

    failed_0:
        say "invariant_i2_incremental #0 failed"
        say endGraphemeStorageEnabled" & "endCodepointStorageEnabled
        say indexer~string~length
        return
    failed_1:
        say "invariant_i2_incremental #1 failed"
        say endGraphemeStorageCount" \== 0 & "endCodepointStorageCount" \== 0"
        return
    failed_3:
        say "invariant_i2_incremental #3 failed"
        say ".RexxUnicode~graphemeBreak("codepoint1", "codepoint2", >state) == .false"
        return


/*
non-incremental
i3 startErrorMessages~items == min(startErrorStorageLimit, errorCount), even if no grapheme or codepoint index is stored.
*/
::routine invariant_i3_non_incremental
    use strict arg indexer

    if indexer~isIncremental then return
    .local~counters["invariant_i3_non_incremental"] += 1

    startErrorMessages = indexer~startErrorMessages
    startErrorStorageLimit = indexer~limits[3]
    errorCount = indexer~errorCount

    invariant = startErrorMessages~items == min(startErrorStorageLimit, errorCount)
    if invariant == .false then do
        say "invariant_i3_non_incremental failed"
        say startErrorMessages~items" == min("startErrorStorageLimit", "errorCount")"
    end


/*
incremental
i3 If the start error storage is enabled:
startErrorMessages~items == max(negStartGrapheme, negStartCodepoint)
except when startErrorStorageCount + endErrorStorageCount == errorCount.
*/
::routine invariant_i3_incremental
    use strict arg indexer

    if \ indexer~isIncremental then return

    -- The start error storage must be enabled
    startErrorStorage = indexer~limits[3]
    if \ startErrorStorage then return

    -- This exception must be tested
    if indexer~startErrorStorageCount + indexer~endErrorStorageCount == indexer~errorCount then return

    .local~counters["invariant_i3_incremental"] += 1

    startErrorMessages = indexer~startErrorMessages
    negStartGrapheme = negativeItems(indexer~startGraphemeIndexes)
    negStartCodepoint = negativeItems(indexer~startCodepointIndexes)

    invariant = startErrorMessages~items == max(negStartGrapheme, negStartCodepoint)
    if invariant == .false then do
        say "invariant_i3_incremental failed"
        say startErrorMessages~items" == max("negStartGrapheme", "negStartCodepoint")"
    end


/*
non-incremental
i4 endErrorMessages~items == min(endErrorStorageLimit, max(negEndGrapheme, negEndCodepoint))
except when startErrorStorageCount + endErrorStorageCount == errorCount
*/
::routine invariant_i4_non_incremental
    use strict arg indexer

    if indexer~isIncremental then return
    .local~counters["invariant_i4_non_incremental"] += 1

    endErrorMessages = indexer~endErrorMessages
    endErrorStorageLimit = indexer~limits[6]
    negEndGrapheme = negativeItems(indexer~endGraphemeIndexes)
    negEndCodepoint = negativeItems(indexer~endCodepointIndexes)

    -- Not tested for the moment:
    -- except when startErrorStorageCount + endErrorStorageCount == errorCount
    invariant = endErrorMessages~items == min(endErrorStorageLimit, max(negEndGrapheme, negEndCodepoint))
    if invariant == .false then do
        say "invariant_i4_non_incremental failed"
        say indexer~endErrorMessages~items" == min("endErrorStorageLimit", max("negEndGrapheme", "negEndCodepoint"))"
    end


/*
incremental
i4 If the end error storage is enabled:
endErrorMessages~items == max(negEndGrapheme, negEndCodepoint)
except when startErrorStorageCount + endErrorStorageCount == errorCount.
*/
::routine invariant_i4_incremental
    use strict arg indexer

    if \ indexer~isIncremental then return

    -- The end error storage must be enabled
    endErrorStorage = indexer~limits[6]
    if \ endErrorStorage then return

    -- This exception must be tested
    if indexer~startErrorStorageCount + indexer~endErrorStorageCount == indexer~errorCount then return

    .local~counters["invariant_i4_incremental"] += 1

    endErrorMessages = indexer~endErrorMessages
    negEndGrapheme = negativeItems(indexer~endGraphemeIndexes)
    negEndCodepoint = negativeItems(indexer~endCodepointIndexes)

    invariant = endErrorMessages~items == max(negEndGrapheme, negEndCodepoint)
    if invariant == .false then do
        say "invariant_i4_incremental failed"
        say endErrorMessages~items" == max("negEndGrapheme", "negEndCodepoint")"
    end


::requires "rxunicode.cls"
::requires "procedural/dispatcher.cls"
::requires "rgf_util2.rex"


/*
Fixed:

-- indexer = .RexxUnicodeStringIndexer~newIncremental("⦅⦅" || "F4 91"x "🎅" "F4 91"x "👨‍👩‍👧" "F4 91"x || "⦆⦆", 0, 1, 0, 1, 0, 0)
  1764 *-*             supplierIndexG = startGraphemeIndexes~items + 1
  1882 *-*             item = self~incrementalCodepointIndexB(indexC)
   137 *-*                   indexer~codepointIndexC(i)
    41 *-* call regressionTests '"⦅⦅" || "F4 91"x "🎅" "F4 91"x "👨‍👩‍👧" "F4 91"x || "⦆⦆"'
Error 97 running /Users/Shared/local/rexx/oorexx/build/executor5-bulk.main/main/trunk/macos-arm64/clang/release/delivery/bin/rxunicode.cls line 1764:  Object method not found.
Error 97.1:  Object "The NIL object" does not understand message "ITEMS".

*/