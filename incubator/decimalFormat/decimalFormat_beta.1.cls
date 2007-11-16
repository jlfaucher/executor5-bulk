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
-- Separators are now broken into different attributes per Rick's suggestion
    decimalSeparator  = '.'
    groupingSeparator = ','

::method getVersion
return 'Beta .1'

-- Changed get/set method calls to attributes per Rick's suggestion
::attribute groupingSize get
::attribute groupingSize set
    expose groupingSize
    use strict arg new_groupingSize
    .argUtil~validatePositive("grouping size", new_groupingSize)
    groupingSize = new_groupingSize
return 0

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

::attribute pattern get
    expose mask
return mask

::attribute pattern set
    expose mask
    use strict arg mask
    self~validateMask
return 0

-- This method is for internal use only
::method validateMask
    expose mask pos_prefix pos_mask pos_suffix neg_prefix neg_mask neg_suffix zero_prefix zero_mask zero_suffix have_zMask

-- see if there's a ; divider, if so can't be > 2
    if mask~countStr(';') > 2 then
        raise syntax 93.900 array('Pattern Can NOT Have More The 2 Semi-Colons')

-- examine all the pieces of the pattern
    parse var mask Wpos_mask';'Wneg_mask';'Wzero_mask

-- examine all the pieces of the pos_mask
-- ascertain that single and/or double quotes are paired

-- RKM a lot of this validation gets repeated for each mask.  Seems like a good place for a private
-- method that can be called for each one.

-- ELP Will have to see how to return the appropiate error

-- ascertain that quotes are paired
    Wpos_mask = Wpos_mask~strip
    if Wpos_mask~pos("'") > 0 then
        do
            if Wpos_mask~countStr("'") // 2 \= 0 then
                raise syntax 93.900 array('Single Quotes Must Be Matched In The Positive Pattern')
        end

    if Wpos_mask~pos('"') > 0 then
        do
            if Wpos_mask~countStr('"') // 2 \= 0 then
                raise syntax 93.900 array('Double Quotes Must Be Matched In The Positive Pattern')
        end

-- see if there is a prefix
    first_char = Wpos_mask~left(1)
    pos_prefix = ''
    if first_char = '"' | first_char = "'" then
        do
-- this will be the pre-fix for a positive number
            parse var Wpos_mask (first_char)pos_prefix(first_char)Wpos_mask
        end

-- see if there is a suffix
    last_char = Wpos_mask~right(1)
    pos_suffix = ''
    if last_char = '"' | last_char = "'" then
        do
-- this will be the pre-fix for a positive number
            parse var Wpos_mask Wpos_mask(last_char)pos_suffix(last_char)
        end

-- can not be more than 1 '.' in the remaining pattern
    if Wpos_mask~countStr('.') > 1 then
        raise syntax 93.900 array('Positive Patterns Can Not Have More Than One Decimal (.)')

-- # can not follow 0 in m_int
    parse var Wpos_mask m_int'.'m_dec
    fp_z = m_int~pos('0')
    if fp_z > 0 then
        do
            if m_int~pos('#',fp_z) > 0 then
                raise syntax 93.900 array('A # Symbol Can Not Follow A 0 In The Integer Portion Of The Positive Pattern')
        end

-- 0 can not follow # in m_dec
    fp_p = m_dec~pos('#')
    if fp_p > 0 then
        do
            if m_dec~pos('0',fp_p) > 0 then
                raise syntax 93.900 array('A 0 Symbol Can Not Follow A # In The Decimal Portion Of The Positive Pattern')
        end

-- examine the characters in the remaining mask
    if Wpos_mask~verify('#,0.') <> 0 then
        raise syntax 93.900 array('The Mask Portion Of The Positive Pattern Can Not Contain Characters Other Than "#,0."')
    pos_mask = Wpos_mask

-----------------------------------------------------------------------------------
-- examine all the pieces of the neg_mask

    if Wneg_mask = '' then
        do
-- use the pos variables if no negative mask is specified
            neg_prefix = '-'pos_prefix
            neg_mask   = pos_mask
            neg_suffix = pos_suffix
        end
    else
        do
-- ascertain that single and/or double quotes are paired
            Wneg_mask = Wneg_mask~strip
            if Wneg_mask~pos("'") > 0 then
                do
                    if Wneg_mask~countStr("'") // 2 \= 0 then
                        raise syntax 93.900 array('Single Quotes Must Be Matched In The Negative Pattern')
                end

            if Wneg_mask~pos('"') > 0 then
                do
                    if Wneg_mask~countStr('"') // 2 \= 0 then
                        raise syntax 93.900 array('Double Quotes Must Be Matched In The Negative Pattern')
                end

-- see if there is a prefix
            first_char = Wneg_mask~left(1)
            if first_char = '"' | first_char = "'" then
                do
-- this will be the pre-fix for a negative number
                    parse var Wneg_mask (first_char)neg_prefix(first_char)Wneg_mask
                end

-- see if there is a suffix
            last_char = Wneg_mask~right(1)
            if last_char = '"' | last_char = "'" then
                do
-- this will be the pre-fix for a negative number
                    parse var Wneg_mask Wneg_mask(last_char)neg_suffix(last_char)
                end

-- can not be more than 1 '.' in the remaining pattern
            if Wneg_mask~countStr('.') > 1 then
                    raise syntax 93.900 array('Negative Patterns Can Not Have More Than One Decimal (.)')

-- this was to implement Rick's early suggestion that a pos/neg like pattern shouldn't have to be repeated
            if Wneg_mask = '' then
                Wneg_mask = pos_mask

            parse var Wneg_mask m_int'.'m_dec
-- # can not follow 0 in m_int
            fp_z = m_int~pos('0')
            if fp_z > 0 then
                do
                    if m_int~pos('#',fp_z) > 0 then
                        raise syntax 93.900 array('A # Symbol Can Not Follow A 0 In The Integer Portion Of The Negative Pattern')
                end

-- 0 can not follow # in m_dec
            fp_p = m_dec~pos('#')
            if fp_p > 0 then
                do
                    if m_dec~pos('0',fp_p) > 0 then
                        raise syntax 93.900 array('A 0 Symbol Can Not Follow A # In The Decimal Portion Of The Negative Pattern')
                end

-- examine the characters in the remaining mask
        if Wneg_mask~verify('#,0.') <> 0 then
            raise syntax 93.900 array('The Mask Portion Of The Positive Pattern Can Not Contain Characters Other Than "#,0."')
        neg_mask = Wneg_mask
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

::method format
    expose pos_prefix pos_mask pos_suffix neg_prefix neg_mask neg_suffix zero_prefix zero_mask zero_suffix input have_zMask decimalSeparator groupingSeparator
    use strict arg input
    numeric digits 30

-- RKM
-- NB:  I didn't use argutil here because it uses the default digits for everything.  Might be something I
-- want to add
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

-- ELP Fix (mutable buffer)
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
                if m_part2 \== '' & m_part_2~pos('0') < 1 then
                    do
                        input = input~strip('t','0')
                        input = input~strip('t','.')
                    end

                parse var input pt1'.'pt2
-- deal with pt1 - integer portion
                if m_part1~pos(',') > 0 then
                    do
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
                    output = pt1

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
                        output = op_pt1'.'op_pt2
                    end

                if num01 = 0 then
                    do
                        output = output~strip('l','0')
                        if output = '' then
                            output = 0
                    end
            end
    end
return output


