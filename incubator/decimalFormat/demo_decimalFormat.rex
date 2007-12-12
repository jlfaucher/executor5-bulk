/* demo_decimalFormat.rex */
    call SysCls

-- Grouping applied
-- 2 decimal places, if needed (input value is rounded)
    f = .decimalFormat~new(',.##')
    say f~format(1234.567)      --> 1,234.57
    say f~format(1234.5)        --> 1,234.5
    say
    say f~format(-1234.567)     --> -1,234.57
    say f~format(-1234.5)       --> -1,234.5
    say

-- Grouping applied
-- 2 decimal places, even if not needed
    f = .decimalFormat~new(',.00')
    say f~format(1234.56)       --> 1,234.56
    say f~format(1234.5)        --> 1,234.50
    say
    say f~format(-1234.56)      --> -1,234.56
    say f~format(-1234.5)       --> -1,234.50
    say

-- No Grouping
-- leading zero if needed
-- 2 decimal places, even if not needed
-- 1 leading zero, if needed
    f = .decimalFormat~new('0.00')
    say f~format(1234.56)       --> 1234.56
    say f~format(1234.5)        --> 1234.50
    say f~format(3/6)           --> 0.50
    say
    say f~format(-1234.56)      --> -1234.56
    say f~format(-1234.5)       --> -1234.50
    say f~format(0 - (3/6))     --> -0.50
    say

-- Grouping
-- 2 trailing zeros, if needed
-- 1 leading zero, if needed
-- US currency
    f = .decimalFormat~new('"$",0.00')
    say f~format(1234.56)       --> 1,234.56
    say f~format(1234.5)        --> 1,234.50
    say f~format(3/6)           --> 0.50
    say
    say f~format(-1234.56)      --> -1,234.56
    say f~format(-1234.5)       --> -1,234.50
    say f~format(0 - (3/6))     --> -0.50
    say

-- 2 trailing zeros, if needed
-- 1 leading zero, if needed
-- Accounting
    f = .decimalFormat~new(',0.00" DB";,0.00" CR"')
    say f~format(1234.56)       --> 1,234.56 DB
    say f~format(1234.5)        --> 1,234.50 DB
    say f~format(3/6)           --> 0.50 DB
    say
    say f~format(-1234.56)      --> -1,234.56 CR
    say f~format(-1234.5)       --> -1,234.50 CR
    say f~format(0 - (3/6))     --> -0.50 CR
    say

-- 2 trailing zeros, if needed
-- 1 leading zero, if needed
-- Euro currency
    f = .decimalFormat~new('"€",0.00;"-€"')
    f~groupingSeparator = '.'
    f~decimalSeparator = ','
    say f~format(1234.56)       --> €1.234,56
    say f~format(1234.5)        --> €1.234,50
    say f~format(3/6)           --> €0,50
    say
    say f~format(-1234.56)      --> -€1.234,56
    say f~format(-1234.5)       --> -€1.234,50
    say f~format(0 - (3/6))     --> -€0,50
    say

-- Group positive numbers
-- No grouping for negative numbers
-- Return [0] for zero values
    f = .decimalFormat~new(',0.00;"-"0.00;"[0]"')
    say f~format(1234.56)       --> 1,234.56
    say f~format(-1234)         --> -1234.00
    say f~format(0)             --> [0]
    say

-- Group all numbers
-- Place parens around negative numbers
    f = .decimalFormat~new(',0.00;"("")"')
    say f~format(1234.56)       --> 1,234.56
    say f~format(-1234)         --> (1,234.00)
    say f~format(0)             --> 0.00

::requires 'decimalFormat.cls'
