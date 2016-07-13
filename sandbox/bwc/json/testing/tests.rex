/* tests.rex */

parser = .json~new()

counters = .directory~new()
counters['pass'] = 0
counters['fail'] = 0

do type over .array~of('valid', 'invalid')
    call sysFileTree '.\'type'\*.json', 'file', 'fo'
    do i = 1 to file.0
        name = 'check_' || type
        call (name) parser, file.i
        retc = handle_result(result, file.i, counters)
    end
end

say 'Passed:' counters['pass']
say 'Failed:' counters['fail']

::requires 'json.cls'

::routine read_file
    use strict arg file
    fs = .stream~new(file)
    data = fs~charin(, fs~chars())
    fs~close()
    return data

::routine check_valid
    use strict arg parser, file

    data = read_file(file)

    signal on any name custom
    signal on error name custom
    signal on failure name custom
    signal on halt name custom
    signal on syntax name custom

    parser~fromJSON(data)
return .true
custom:
    signal off any
    signal off error
    signal off failure
    signal off halt
    signal off syntax
return .false

::routine check_invalid
    use strict arg parser, file

    data = read_file(file)

    signal on any name custom
    signal on error name custom
    signal on failure name custom
    signal on halt name custom
    signal on syntax name custom

    parser~fromJSON(data)
return .false
custom:
    signal off any
    signal off error
    signal off failure
    signal off halt
    signal off syntax
return .true

::routine handle_result
    use strict arg pass, file, counters
    if pass then do
        counters['pass'] += 1
    end
    else do
        counters['fail'] += 1
        name = filespec('n', file)
        say 'Failed "'name'"'
    end
return .nil