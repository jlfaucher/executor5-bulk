/* decimalFormat.cls */
::class 'decimalFormat' public

::method init
    expose mask groupingSize decimalSeparator groupingSeparator
    use strict arg mask = '#,###.##'

-- Be sure ooRexx version is dated 20071030 or higher
    parse version ooRexxVer
    if date('s',ooRexxVer~subWord(3),'n') < 20071030 then
        raise syntax 3.903 array('decimalFormat.cls')

    if mask = '' then
        mask = '#,###.##'
    self~validateMask

    groupingSize = 3
    decimalSeparator  = '.'
    groupingSeparator = ','

::method getVersion
return 'Beta .4'
/*
Beta        11/15/07 AM
            Lee's original version sent only to Rick

Beta .1     11/15/07 PM
            Incorporated changes suggested by Rick, attributes vs. methods,
            Use verify to examine the parts of the mask
            Broke separators into decimalSeparator & groupingSeparator

Beta .2     11/16/07
            Combined verification of positive pattern & negative pattern into one method and preserved
            applicable error messages

            Added ability to get/set the following attributes
                Positive prefix  - pPrefix
                Positive pattern - pPattern
                Positive suffix  - pSuffix

                Negative prefix  - nPrefix
                Negative pattern - nPattern
                Negative suffix  - nSuffix

                Zero pattern     - zPattern

        The next 2 items may can be improved, but are sufficient for now.
            Added ability to retrieve grouping setting for either positive, negative, or both patterns
            Added ability to set grouping for either positive, negative, or both patterns

        Changed the way numeric digits is set from an arbitrary value of 30
        to a computed value based on the length of the input + the number of decimal places
        specified in the pattern.

Beta .3     11/17/07
        Created single method (buildMask) for duplicated code in the following
            pPrefix
            pPattern
            pSuffix

            nPrefix
            nPattern
            nSuffix

            zPattern


Beta .3.1   11/18/07
        Move check for decimalSeparator/groupingSeparator to just before they are applied.
        Ascertain that a comma does not follow a decimal in mask portion of patterns

Beta .3.2   11/19/07
        Fixed problem is setGrouping
        Disallowed .nil as a zero pattern

Beta .4     11/20/07
        Renamed the following to ease confusion
            pPattern    ->  pMask
            nPattern    ->  nMask
            zPattern    ->  zMask

        Removed getGrouping & setGrouping
            Added pGrouping & nGrouping

ToDo
    Recreate the WORD file for the documentation - some a-hole deleted it (me)

    Make note in documentation that .nil can not be used for zero pattern

    Study the return value of 0 some more!!!
*/

-- get/set positive grouping
::attribute pGrouping get
::attribute pGrouping set
    expose pGrouping pPrefix pMask pSuffix nPrefix nMask nSuffix zMask have_zMask
    use strict arg new_grouping
    if \new_grouping~datatype('O') then
        raise syntax 93.900 array('Positive Grouping Indicator Must Be A Logical Value')
    select
        when new_grouping & pGrouping then
            nop     -- Already .true
        when \new_grouping & \pGrouping then
            nop     -- Already .false
        when new_grouping then
            pMask = ','||pMask
        when \new_grouping then
            pMask = pMask~changeStr(',','')
        otherwise
            nop
    end
    self~buildMask
return 0

-- get/set negative grouping
::attribute nGrouping get
::attribute nGrouping set
    expose nGrouping pPrefix pMask pSuffix nPrefix nMask nSuffix zMask have_zMask
    use strict arg new_grouping
    if \new_grouping~datatype('O') then
        raise syntax 93.900 array('Negative Grouping Indicator Must Be A Logical Value')
    select
        when new_grouping & nGrouping then
            nop     -- Already .true
        when \new_grouping & \nGrouping then
            nop     -- Already .false
        when new_grouping then
            nMask = ','||nMask
        when \new_grouping then
            nMask = nMask~changeStr(',','')
        otherwise
            nop
    end
    self~buildMask
return 0

-- get/set postive prefix
::attribute pPrefix get
::attribute pPrefix set
    expose pPrefix pMask pSuffix nPrefix nMask nSuffix zMask have_zMask
    use strict arg new_pattern
    pPrefix = new_pattern
    self~buildMask
return 0

-- get/set positive pattern
::attribute pMask get
::attribute pMask set
    expose pPrefix pMask pSuffix nPrefix nMask nSuffix zMask have_zMask
    use strict arg new_pattern
    pMask = new_pattern
    self~buildMask
return 0

-- get/set positive suffix
::attribute pSuffix get
::attribute pSuffix set
    expose pPrefix pMask pSuffix nPrefix nMask nSuffix zMask have_zMask
    use strict arg new_pattern
    pSuffix = new_pattern
    self~buildMask
return 0

-- get/set negative prefix
::attribute nPrefix get
::attribute nPrefix set
    expose pPrefix pMask pSuffix nPrefix nMask nSuffix zMask have_zMask
    use strict arg new_pattern
    nPrefix = new_pattern
    self~buildMask
return 0

-- get/set negative pattern
::attribute nMask get
::attribute nMask set
    expose pPrefix pMask pSuffix nPrefix nMask nSuffix zMask have_zMask
    use strict arg new_pattern
    nMask = new_pattern
    self~buildMask
return 0

-- get/set negative suffix
::attribute nSuffix get
::attribute nSuffix set
    expose pPrefix pMask pSuffix nPrefix nMask nSuffix zMask have_zMask
    use strict arg new_pattern
    nSuffix = new_pattern
    self~buildMask
return 0

::attribute zMask get
::attribute zMask set
    expose pPrefix pMask pSuffix nPrefix nMask nSuffix zMask have_zMask
    use strict arg new_pattern
    zMask = new_pattern
    have_zMask = .true
    if zMask == '' then
        have_zMask = .false
    self~buildMask
return 0

::method buildMask
    expose mask pPrefix pMask pSuffix nPrefix nMask nSuffix zMask have_zMask
    mask = ''
    if pPrefix \= '' then
        mask = '"'pPrefix'"'
    mask = mask||pMask
    if pSuffix \= '' then
        mask = mask||'"'pSuffix'"'
    mask = mask';'
    if nPrefix \= '' then
        mask = mask'"'nPrefix'"'
    mask = mask||nMask
    if nSuffix \= '' then
        mask = mask'"'nSuffix'"'
    if have_zMask then
        do
            mask = mask';'zMask
        end
    self~pattern = mask
return 0

-- get/set grouping size
::attribute groupingSize get
::attribute groupingSize set
    expose groupingSize
    use strict arg new_groupingSize
    .argUtil~validatePositive("grouping size", new_groupingSize)
    groupingSize = new_groupingSize
return 0

-- get/set decimalSeparator
::attribute decimalSeparator get
::attribute decimalSeparator set
    expose decimalSeparator
    use strict arg new_separator
    if new_separator~length <> 1 then
        raise syntax 93.900 array('Decimal Separator Must Have A Length of 1')
    decimalSeparator = new_separator

return 0

-- get/set groupingSeparator
::attribute groupingSeparator get
::attribute groupingSeparator set
    expose groupingSeparator decimalSeparator
    use strict arg new_separator
    if new_separator~length <> 1 then
        raise syntax 93.900 array('Grouping Separator Must Have A Length of 1')
   groupingSeparator = new_separator
return 0

-- get the complete current pattern
::attribute pattern get
    expose mask
return mask

-- set the complete pattern
::attribute pattern set
    expose mask
    use strict arg mask
    self~validateMask
return 0

-- This method is for internal use only
::method validateMask
    expose mask pPrefix pMask pSuffix nPrefix nMask nSuffix zero_prefix zMask zero_suffix have_zMask the_mask the_prefix the_suffix pGrouping nGrouping

-- see if there's a ; divider, if so can't be > 2
    if mask~countStr(';') > 2 then
        raise syntax 93.900 array('Pattern Can NOT Have More The 2 Semi-Colons')

-- break down the complete pattern into its parts
    parse var mask WpMask';'WnMask';'WzMask

-- examine the positive pattern
    self~examineMask(WpMask,'P')
-- load the postive pattern parts
    pMask = the_mask
    pPrefix = the_prefix
    pSuffix = the_suffix

-- if no negative pattern specified, use the same positive pattern
    if WnMask = '' then
        do
            nPrefix   = '-'pPrefix
            nMask     = pMask
            nSuffix   = pSuffix
            nGrouping = pGrouping
        end
    else
-- examine the negative pattern
        do
            self~examineMask(WnMask,'N')
-- load the negative pattern parts
            if the_mask = '' then
                nMask = pMask
            else
                nMask = the_mask

            if the_prefix = '' then
                nPrefix = pPrefix
            else
                nPrefix = the_prefix

            if the_suffix = '' then
                nSuffix = pSuffix
            else
                nSuffix = the_suffix
        end

-----------------------------------------------------------------------------------
-- examine all the pieces of the zMask

    if WzMask == 'The NIL object' then
        raise syntax 93.900 array('The Zero Pattern Can Not Be Set To .nil')

    if WzMask == '' then
        do
-- use the pos variables if no zero mask is specified
            zero_prefix = pPrefix
            zMask   = pMask
            zero_suffix = pSuffix
            have_zMask = .false
        end
    else
        do
-- ascertain that single and/or double quotes are paired
            have_zMask = .true
            if WzMask~pos("'") > 0 then
                do
                    if WzMask~countStr("'") // 2 \= 0 then
                        raise syntax 93.900 array('Single Quotes Must Be Matched In The Zero Pattern')
                end

            if WzMask~pos('"') > 0 then
                do
                    if WzMask~countStr('"') // 2 \= 0 then
                        raise syntax 93.900 array('Double Quotes Must Be Matched In The Zero Pattern')
                end
-- set up used variables and strip the quotes
            zero_prefix = ''
            zMask   = WzMask~strip('b',"'")
            zMask   = zMask~strip('b','"')
            zero_suffix = ''
        end

-- This method is for internal use only
::method examineMask
    expose the_mask the_prefix the_suffix pGrouping nGrouping
    use strict arg the_mask,np
    np = np~translate

-- will be either the positive or negative pattern
    the_mask = the_mask~strip

-- be sure single quotes are paired
    if the_mask~pos("'") > 0 then
        do
            if the_mask~countStr("'") // 2 \= 0 then
                do
                    if np = 'N' then
                        raise syntax 93.900 array('Single Quotes Must Be Matched In The Negative Pattern')
                    else
                        raise syntax 93.900 array('Single Quotes Must Be Matched In The Positive Pattern')
                end
        end

-- be sure double quotes are paired
    if the_mask~pos('"') > 0 then
        do
            if the_mask~countStr('"') // 2 \= 0 then
                do
                    if np = 'N' then
                        raise syntax 93.900 array('Double Quotes Must Be Matched In The Negative Pattern')
                    else
                        raise syntax 93.900 array('Double Quotes Must Be Matched In The Positive Pattern')
                end
        end

-- examine the first character, if it is a quote, there must be a prefix
    first_char = the_mask~left(1)
    the_prefix = ''
    if first_char = '"' | first_char = "'" then
        do
            parse var the_mask (first_char)the_prefix(first_char)the_mask
        end

-- examine the last character, if it is a quote, there must be a suffix
    last_char = the_mask~right(1)
    the_suffix = ''
    if last_char = '"' | last_char = "'" then
        do
            parse var the_mask the_mask(last_char)the_suffix(last_char)
        end

-- can't be but one . in the entire pattern
    if the_mask~countStr('.') > 1 then
        do
            if np = 'N' then
                raise syntax 93.900 array('Negative Patterns Can Not Have More Than One Decimal (.)')
            else
                raise syntax 93.900 array('Positive Patterns Can Not Have More Than One Decimal (.)')
        end

-- break the pattern into the integer and decimal parts
    parse var the_mask m_int'.'m_dec

-- a # can not follow a 0 in the interger part
    fp_z = m_int~pos('0')
    if fp_z > 0 then
        do
            if m_int~pos('#',fp_z) > 0 then
                do
                    if np = 'N' then
                        raise syntax 93.900 array('A # Symbol Can Not Follow A 0 In The Integer Portion Of The Negative Pattern')
                    else
                        raise syntax 93.900 array('A # Symbol Can Not Follow A 0 In The Integer Portion Of The Positive Pattern')
                end
        end

-- a 0 can not follow a # in the decimal part
    fp_p = m_dec~pos('#')
    if fp_p > 0 then
        do
            if m_dec~pos('0',fp_p) > 0 then
                do
                    if np = 'N' then
                        raise syntax 93.900 array('A 0 Symbol Can Not Follow A # In The Decimal Portion Of The Negative Pattern')
                    else
                        raise syntax 93.900 array('A 0 Symbol Can Not Follow A # In The Decimal Portion Of The Positive Pattern')
                end
        end

-- verify that what is left consists only of the #,0. characters
    if the_mask~verify('#,0.') <> 0 then
        if np = 'N' then
            raise syntax 93.900 array('The Mask Portion Of The Negative Pattern Can Not Contain Characters Other Than "#,0."')
        else
            raise syntax 93.900 array('The Mask Portion Of The Positive Pattern Can Not Contain Characters Other Than "#,0."')

-- verify that there is no more than 1 decimal in the pattern mask
    first_d = the_mask~pos('.')
    if first_d  > 0 then
        do
            next_c = the_mask~pos(',',first_d+1)
            if next_c > 0 then
            if np = 'N' then
                raise syntax 93.900 array('A Comma Can Not Follow A Decimal In The Mask Portion Of The Negative Pattern')
            else
                raise syntax 93.900 array('A Comma Can Not Follow A Decimal In The Mask Portion Of The Positive Pattern')
        end

    select
        when np = 'P' & the_mask~pos(',') > 0 then
            pGrouping = .true
        when np = 'P' & the_mask~pos(',') < 1 then
            pGrouping = .false
        when np = 'N' & the_mask~pos(',') > 0 then
            nGrouping = .true
        when np = 'N' & the_mask~pos(',') < 1 then
            nGrouping = .false
        otherwise
            nop
    end



return 0

-- do the actual formatting of the input number
::method format
    expose pPrefix pMask pSuffix nPrefix nMask nSuffix zero_prefix zMask zero_suffix input have_zMask decimalSeparator groupingSeparator
    use strict arg input

-- set numeric digits to the length of input + the number of zeros in the decimal part of the patterns
    adder = 0
    parse var pMask .'.'dec_p
    ph0 = dec_p~countStr('0')
    if ph0 > adder then
        adder = ph0
    parse var nMask .'.'dec_p
    ph0 = dec_p~countStr('0')
    if ph0 > adder then
        adder = ph0
    if input~length + adder > 9 then
        numeric digits input~length + adder

    if \input~datatype('n') then
        raise syntax 93.904 array(1,arg(1))

-- perform all masking based on the absolute value of the input, but save the input for later testing
    save_input = input
    input = input~abs()

-- determine which mask to use
    select
        when save_input~abs = 0 & have_zMask then
            do
                output = zMask
            end
        when save_input = 0 then
            do
                output = self~format2(zMask)
                output = zero_prefix||output||zero_suffix
            end
        when save_input > 0 then
            do
                output = self~format2(pMask)
                output = pPrefix||output||pSuffix
            end
        when save_input < 0 then
            do
                output = self~format2(nMask)
                output = nPrefix||output||nSuffix
-- the format in ~format2 may have produced a 0 value
                if output == '-0' then
                    output = 0
            end
        otherwise
            nop
    end

-- ELP Fix (possibly use a mutable buffer)

-- ihere
    if groupingSeparator = decimalSeparator then
        raise syntax 93.900 array('Grouping & Decimal Separators Can Not Be The Same')

    if groupingSeparator \= ',' then
        output = output~changeStr(',',.endOfLine)
    if decimalSeparator \= '.' then
        output = output~changeStr('.',decimalSeparator)
    output = output~changeStr(.endOfLine,groupingSeparator)
return output

-- internal use only
::method format2
    expose output input output zMask have_zMask groupingSize
    use strict arg mask2use

-- set numeric digits to the length of input + the number of zeros in the decimal part of the patterns
    adder = 0
    parse var pMask .'.'dec_p
    ph0 = dec_p~countStr('0')
    if ph0 > adder then
        adder = ph0
    parse var nMask .'.'dec_p
    ph0 = dec_p~countStr('0')
    if ph0 > adder then
        adder = ph0
    if input~length + adder > 9 then
        numeric digits input~length + adder

    parse var mask2use m_part1'.'m_part2
-- format input based on the length of the decimal portion of the mask
    if m_part2 \== '' then
        do
            input = input~format(,m_part2~length)
        end
    else
        input = input~format(,0)

    select
        when input = 0 & \have_zMask then
            output = 0
        when input = 0 then
            output = zMask
        otherwise
            do
-- strip any trailing 0 or . from the formated result is no decimal places specified
                if m_part2 \== '' & m_part_2~pos('0') < 1 then
                    do
                        input = input~strip('t','0')
                        input = input~strip('t','.')
                    end

                parse var input pt1'.'pt2
-- deal with pt1 - integer portion
                if m_part1~pos(',') > 0 then
                    do
-- we need grouping
                        i_int = pt1~reverse
                        output = ''
                        do while i_int <> ''
                            parse var i_int thousand_part =(groupingSize+1) i_int
                            if output == '' then
                                output = thousand_part
                            else
                                output = output','thousand_part
                        end
                        output = output~reverse
                    end
                else
-- no grouping needed
                    output = pt1

-- pad with any 0 place holders - save our results in op_pt1
                width = m_part1~length
                num01 = m_part1~countStr('0')
                if num01 > 0 then
                    do
                        if output~length < num01 then
                            do until output~length = num01
                                output = '0'||output
                            end
                    end

                if output = '' then
                    output = 0

                if num0 < 1 then
                    output = output~strip('l','0')

                op_pt1 = output

-- deal with pt2 - decimal portion
                if pt2 = '' then
                    do
-- no decimal characters after format, so place 0 place holders
                        num02 = m_part2~countStr('0')
                        if num02 < 1 then
                            output = op_pt1
                        else
                            do
                                output = op_pt1'.'||'0'~copies(num02)
                            end
                    end
                else
                    do
-- pad decimal characters with 0 place holders if any and place the results in op_pt2
                        op_pt2 = pt2
                        width = m_part2~length
                        num02 = m_part2~countStr('0')
                        if num02 > 1 then
                            do
                                if op_pt2~length < width then
                                    do until op_pt2~length = width
                                        op_pt2 = op_pt2||'0'
                                    end
                            end
-- put the pieces together
                        output = op_pt1'.'op_pt2
                    end

                if num01 = 0 then
                    do
                        output = output~strip('l','0')
-- format may have produced a 0 value
                        if output = '' then
                            output = 0
                    end
            end
    end
return output


