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
return 'Beta .2'
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
            Added ability to retrieve grouping setting for either positive, negative, or both patterns
            Added ability to set grouping for either positive, negative, or both patterns
            Added ability to get just any one of the 3 patterns

To Do
    Ability to set any one of the 3 patterns without having to redefine the entire pattern
*/

-- get the grouping for the specified pattern
::method getGrouping
    expose neg_mask pos_mask groupingSeparator
    use arg which_one
    which_one = which_one~translate
    if arg(1,'o') then
        raise syntax 93.900 array('You Must Specify A Which Pattern To Report')
    which_one = which_one~left(1)
    if which_one~verify('PNB') <> 0 then
        raise syntax 93.900 array('You Must Which Pattern - "P"ositive, "N"egative, or "B"oth')
    select
        when which_one = 'P' then
            do
                if pos_mask~pos(groupingSeparator) > 0 then
                    output = .true
                else
                    output = .false
            end
        when which_one = 'N' then
            do
                if neg_mask~pos(groupingSeparator) > 0 then
                    output = .true
                else
                    output = .false
            end
        otherwise
            do
                output = ''
                if pos_mask~pos(groupingSeparator) > 0 then
                    output = output||.true
                else
                    output = output||.false
                if neg_mask~pos(groupingSeparator) > 0 then
                    output = output','.true
                else
                    output = output','.false
            end
    end
return output

-- set the grouping for the specified pattern
::method setGrouping
    expose neg_mask pos_mask groupingSeparator
    use arg which_one,oo
    if arg(1,'o') then
        raise syntax 93.900 array('You Must Specify A Which Pattern To Report')
    if arg(2,'o') then
        raise syntax 93.900 array('You Must Specify .true Or .false As The Second Argument')
    which_one = which_one~translate
    if which_one~verify('PNB') <> 0 then
        raise syntax 93.900 array('You Must Which Pattern - "P"ositive, "N"egative, or "B"oth')
    if \oo~datatype('O') then
        raise syntax 93.900 array('You Must Specify .true Or .false As The Second Argument')
    select
        when which_one = 'P' & oo & pos_mask~pos(groupingSeparator) = 0 then
            pos_mask = groupingSeparator||pos_mask
        when which_one = 'P' & \oo & pos_mask~pos(groupingSeparator) > 0 then
            pos_mask = pos_mask~changeStr(groupingSeparator,'')
        when which_one = 'N' & oo & neg_mask~pos(groupingSeparator) = 0 then
            neg_mask = groupingSeparator||neg_mask
        when which_one = 'N' & \oo & neg_mask~pos(groupingSeparator) > 0 then
            neg_mask = neg_mask~changeStr(groupingSeparator,'')
        otherwise
            nop
    end
return 0

-- get the pattern for the specified pattern
::method getPattern
    expose pos_prefix pos_mask pos_suffix neg_prefix neg_mask neg_suffix zero_prefix zero_mask zero_suffix have_zMask
    use arg which_one
    which_one = which_one~translate
    if arg(1,'o') then
        raise syntax 93.900 array('You Must Specify A Which Pattern To Report')
    which_one = which_one~left(1)
    if which_one~verify('PNZ') <> 0 then
        raise syntax 93.900 array('You Must Which Pattern - "P"ositive, "N"egative, or "Z"ero')
    select
        when which_one = 'P' then
            do
                pp = ''
                if pos_prefix \= '' then
                    pp = '"'pos_prefix'"'
                pm = ''
                if pos_mask \= '' then
                    pm = pos_mask
                ps = ''
                if pos_suffix \= '' then
                    ps = '"'pos_suffix'"'
                aPattern = pp||pm||ps
            end
        when which_one = 'N' then
            do
                np = ''
                if neg_prefix \= '' then
                    np = '"'neg_prefix'"'
                nm = ''
                if neg_mask \= '' then
                    nm = neg_mask
                ns = ''
                if neg_suffix \= '' then
                    ns = '"'neg_suffix'"'
                aPattern = np||nm||ns
            end
        otherwise
            do
                if have_zMask then
                    aPattern = '"'zero_mask'"'
                else
                    aPattern = 0
            end
    end
return aPattern

-- set the pattern for the specified pattern
::method setPattern
-- not completed

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
    expose decimalSeparator groupingSeparator
    use strict arg new_separator
    if new_separator~length <> 1 then
        raise syntax 93.900 array('Decimal Separator Must Have A Length of 1')
    if new_separator = groupingSeparator then
        raise syntax 93.900 array('Decimal Separator Can Not Be The Same As The Grouping Separator')
    decimalSeparator = new_separator
return 0

-- get/set groupingSeparator
::attribute groupingSeparator get
::attribute groupingSeparator set
    expose groupingSeparator decimalSeparator
    use strict arg new_separator
    if new_separator~length <> 1 then
        raise syntax 93.900 array('Grouping Separator Must Have A Length of 1')
    if new_separator = decimalSeparator then
        raise syntax 93.900 array('Grouping Separator Can Not Be The Same As The Decimal Separator')
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
    expose mask pos_prefix pos_mask pos_suffix neg_prefix neg_mask neg_suffix zero_prefix zero_mask zero_suffix have_zMask the_mask the_prefix the_suffix

-- see if there's a ; divider, if so can't be > 2
    if mask~countStr(';') > 2 then
        raise syntax 93.900 array('Pattern Can NOT Have More The 2 Semi-Colons')

-- break down the complete pattern into its parts
    parse var mask Wpos_mask';'Wneg_mask';'Wzero_mask

-- examine the positive pattern
    self~examineMask(Wpos_mask,'P')
-- load the postive pattern parts
    pos_mask = the_mask
    pos_prefix = the_prefix
    pos_suffix = the_suffix

-- if no negative pattern specified, use the same positive pattern
    if Wneg_mask = '' then
        do
            neg_prefix = '-'pos_prefix
            neg_mask   = pos_mask
            neg_suffix = pos_suffix
        end
    else
-- examine the negative pattern
        do
            self~examineMask(Wneg_mask,'N')
-- load the negative pattern parts
            if the_mask = '' then
                neg_mask = pos_mask
            else
                neg_mask = the_mask

            if the_prefix = '' then
                neg_prefix = pos_prefix
            else
                neg_prefix = the_prefix

            if the_suffix = '' then
                neg_suffix = pos_suffix
            else
                neg_suffix = the_suffix
        end

-----------------------------------------------------------------------------------
-- examine all the pieces of the zero_mask
    if Wzero_mask = '' then
        do
-- use the pos variables if no zero mask is specified
            zero_prefix = pos_prefix
            zero_mask   = pos_mask
            zero_suffix = pos_suffix
            have_zMask = .false
        end
    else
        do
-- ascertain that single and/or double quotes are paired
            have_zMask = .true
            if Wzero_mask~pos("'") > 0 then
                do
                    if Wzero_mask~countStr("'") // 2 \= 0 then
                        raise syntax 93.900 array('Single Quotes Must Be Matched In The Zero Pattern')
                end

            if Wzero_mask~pos('"') > 0 then
                do
                    if Wzero_mask~countStr('"') // 2 \= 0 then
                        raise syntax 93.900 array('Double Quotes Must Be Matched In The Zero Pattern')
                end
-- set up used variables and strip the quotes
            zero_prefix = ''
            zero_mask   = Wzero_mask~strip('b',"'")
            zero_mask   = zero_mask~strip('b','"')
            zero_suffix = ''
        end

-- This method is for internal use only
::method examineMask
    expose the_mask the_prefix the_suffix
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
return 0

-- do the actual formatting of the input number
::method format
    expose pos_prefix pos_mask pos_suffix neg_prefix neg_mask neg_suffix zero_prefix zero_mask zero_suffix input have_zMask decimalSeparator groupingSeparator
    use strict arg input
-- an arbitrary value for now
    numeric digits 30
    if \input~datatype('n') then
        raise syntax 93.904 array(1,arg(1))

-- perform all masking based on the absolute value of the input, but save the input for later testing
    save_input = input
    input = input~abs()

-- determine which mask to use
    select
        when save_input~abs = 0 & have_zMask then
            do
                output = zero_mask
            end
        when save_input = 0 then
            do
                output = self~format2(zero_mask)
                output = zero_prefix||output||zero_suffix
            end
        when save_input > 0 then
            do
                output = self~format2(pos_mask)
                output = pos_prefix||output||pos_suffix
            end
        when save_input < 0 then
            do
                output = self~format2(neg_mask)
                output = neg_prefix||output||neg_suffix
-- the format in ~format2 may have produced a 0 value
                if output == '-0' then
                    output = 0
            end
        otherwise
            nop
    end

-- ELP Fix (possibly use a mutable buffer)
    if groupingSeparator \= ',' then
        output = output~changeStr(',',.endOfLine)
    if decimalSeparator \= '.' then
        output = output~changeStr('.',decimalSeparator)
    output = output~changeStr(.endOfLine,groupingSeparator)
return output

-- internal use only
::method format2
    expose output input output zero_mask have_zMask groupingSize
    use strict arg mask2use
    numeric digits 30
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
            output = zero_mask
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


