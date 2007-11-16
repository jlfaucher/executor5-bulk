-- examine all the pieces of the neg_mask
::method examineMask
    expose the_mask the_prefix the_suffix
    use strict arg the_mask,np
    np = np~translate

the_mask = the_mask~strip
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

if the_mask~pos('"') > 0 then
    do
        if the_mask~countStr('"') // 2 \= 0 then
            do
                if np = 'N' then
                    raise syntax 93.900 array('Double Quotes Must Be Matched In The Negative Pattern')
                else
                    raise syntax 93.900 array('Double Quotes Must Be Matched In The Positive Pattern')
    end

first_char = the_mask~left(1)
the_prefix = ''
if first_char = '"' | first_char = "'" then
    do
        parse var the_mask (first_char)the_prefix(first_char)the_mask
    end

last_char = the_mask~right(1)
the_suffix = ''
if last_char = '"' | last_char = "'" then
    do
        parse var the_mask the_mask(last_char)the_suffix(last_char)
    end

if the_mask~countStr('.') > 1 then
    do
        if np = 'N' then
            raise syntax 93.900 array('Negative Patterns Can Not Have More Than One Decimal (.)')
        else
            raise syntax 93.900 array('Positive Patterns Can Not Have More Than One Decimal (.)')
    end

parse var the_mask m_int'.'m_dec
fp_z = m_int~pos('0')
if fp_z > 0 then
    do
        if np = 'N' then
            do
                if m_int~pos('#',fp_z) > 0 then
                    raise syntax 93.900 array('A # Symbol Can Not Follow A 0 In The Integer Portion Of The Negative Pattern')
            end
        else
            do
                if m_int~pos('#',fp_z) > 0 then
                    raise syntax 93.900 array('A # Symbol Can Not Follow A 0 In The Integer Portion Of The Positive Pattern')
            end
    end

fp_p = m_dec~pos('#')
if fp_p > 0 then
    do
        if np = 'N' then
            do
                if m_dec~pos('0',fp_p) > 0 then
                    raise syntax 93.900 array('A 0 Symbol Can Not Follow A # In The Decimal Portion Of The Negative Pattern')
            end
        else
            do
                if m_dec~pos('0',fp_p) > 0 then
                    raise syntax 93.900 array('A 0 Symbol Can Not Follow A # In The Decimal Portion Of The Positive Pattern')
            end
    end

if the_mask~verify('#,0.') <> 0 then
    if np = 'N' then
        raise syntax 93.900 array('The Mask Portion Of The Negative Pattern Can Not Contain Characters Other Than "#,0."')
    else
        raise syntax 93.900 array('The Mask Portion Of The Positive Pattern Can Not Contain Characters Other Than "#,0."')



