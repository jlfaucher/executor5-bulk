/* decimalformatter.rex */

/*
This demo program will create a text file named df.txt
in the folder it is run from. Since there are 400+ tests
they will scroll beyond the size of your command prompt window.
To turn this off change the logging = .true to logging = .false

Depending on your Locale, the file will probably give
a better rendering of characters such as the
Euro symbol (€).

*/
    call SysCls
    signal on syntax name errorTrap
    logging = .true
    numeric digits 30
    hdr2 = 'UnFormatted'~right(20)-
           'Formatted'~right(20)-
           'Test #'~right(6)||-
           .endOfLine||-
           '-'~copies(20)-
           '-'~copies(20)-
           '-'~copies(6)

    test_ctr    = 0
    test_numbers = .array~of(10193390, 10193390.00, 10193390.49, ,
                             10193390.51, 10193390.513, 10193390.515, ,
                             0, .1, .11, .15, .114, .115, 110193390, ,
                             -10193390, -10193390.00, -10193390.49, ,
                             -10193390.51, -10193390.513, -10193390.515, ,
                             -0, -.1, -.11, -.15, -.114, -.115)

    if logging then
        do
            ostream = .stream~new('.\df.txt')
            ostream~open('Write Replace')
            ostream~lineout(date('s') '-' time('n'))
        end

/* Decimal Number Formatting */

    f = .decimalFormat~new('"$"#,##0.00;"-$"####.##;"[0]"')

    say 'Full'
    say f~Pattern
    say

    say 'pPattern'
    say f~pPattern
    f~pPattern = '0,000.00'
    say f~pPattern
    say

    say 'pPrefix'
    say f~pPrefix
    f~pPrefix = 'CD '
    say f~pPrefix
    say f~Pattern
    say

    say 'pSuffix'
    say '->'f~pSuffix'<-'
    f~pSuffix = ' CR'
    say '->'f~pSuffix'<-'
    say f~Pattern
    say

    say 'nPattern'
    say f~nPattern
    f~nPattern = '#,##0.00'
    say f~nPattern
    say

    say 'nPrefix'
    say f~nPrefix
    f~nPrefix = '-CD('
    say f~nPrefix
    say f~Pattern
    say

    say 'nSuffix'
    say '->'f~nSuffix'<-'
    f~nSuffix = ')'
    say '->'f~nSuffix'<-'
    say f~Pattern
    say

    say 'zPattern'
    say f~zPattern
    f~zPattern = .nil
    say '->'f~zPattern'<-'
    say f~Pattern
    say

    say 'All will now be set back to defaults'
    say

    f = .decimalFormat~new()

    dline = 'decimalFormat Version..:' f~getVersion
    say dline
    say
    if logging then
        do
            ostream~lineout(dline)
            ostream~lineout(' ')
        end

    call DoIt 1

    f~Pattern = '#'
    call DoIt 2

    f~Pattern = '#.##'
    call DoIt 3

    f~Pattern = '#.00'
    call DoIt 4

    f~Pattern = '0.00'
    call DoIt 5

    f~Pattern = '#,##.00'
    call DoIt 6

    f~Pattern = '"$"#,##0.00'
    call DoIt 7

    f~Pattern = '"$"#,##0.00;"-$"#,##0.00'
    call DoIt 8

    f~Pattern = '"$"#,##0.00;"-$"#,###.##'
    call DoIt 9

    f~Pattern = '"$"#,##0.00" DB";"-$"" CR"'
    call DoIt 10

    f~Pattern = '#,##0.00;"("")";'.nil
    call DoIt 11

    f~Pattern = '#,##0.00;"("")";"[0]"'
    call DoIt 12

    f~Pattern = '0.000'
    call DoIt 13

    f~GroupingSize = 4
    f~Pattern = '#,###.##'
    call DoIt 14

    f~GroupingSize = 4
    f~Pattern = '#,##0.00'
    call DoIt 15

    f~decimalSeparator = '^'
    f~groupingSeparator = '.'
    f~decimalSeparator = ','
    f~GroupingSize = 3
    f~Pattern = '"€"#,##0.00;"-€"#,##0.00;"[0]"'
    call DoIt 16

    if logging then
        ostream~close

exit

DoIt:
    use arg test_no
    hdr1 = test_no "Pattern Is ->('"f~Pattern"')<->Grouping Size Is ->" f~GroupingSize '<-'
    if hdr1~pos('The NIL object') > 0 then
        do
            parse var hdr1 pt1';'pt2';'.
            hdr1 = pt1";"pt2";'.nil)<->Grouping Size Is ->" f~GroupingSize '<-'
        end

    call doHdrs
    do i = 1 to test_numbers~items
        test_ctr += 1
        var = test_numbers[i]
        r = f~format(var)
        dline = var~right(20) r~right(20) test_ctr~right(6)
        say dline
        if logging then
            ostream~lineout(dline)
    end
    say
    if logging then
        ostream~lineout(' ')
return

doHdrs:
    say hdr1
    say hdr2
    if logging then
        do
            ostream~lineout(hdr1)
            ostream~lineout(hdr2)
        end
return

errorTrap:
    msg = '---->Syntax Error Trapped<----'
    obj = condition('o')
    msg = msg||.endOfLine||'Message'~right(11)'..:' obj['MESSAGE']
    msg = msg||.endOfLine||'Code'~right(11)'..:' obj['CODE']
    msg = msg||.endOfLine||'ErrorText'~right(11)'..:' obj['ERRORTEXT']
    msg = msg||.endOfLine||'Line #'~right(11)'..:' SIGL
    say msg
exit

::requires 'decimalFormat.cls'


