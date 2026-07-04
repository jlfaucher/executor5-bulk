-- https://sourceforge.net/p/oorexx/bugs/2060/

tc = 0
signal on syntax

-- 0 arg + 1 extra
1:  tc += 1; say "tc =" tc; call r0; say result
2:  tc += 1; say "tc =" tc; call r0 1; say result

-- 1 arg + 1 extra
3:  tc += 1; say "tc =" tc; call r1; say result
4:  tc += 1; say "tc =" tc; call r1 , ; say result
5:  tc += 1; say "tc =" tc; call r1 , 2; say result
6:  tc += 1; say "tc =" tc; call r1 1; say result
7:  tc += 1; say "tc =" tc; call r1 1, ; say result
8:  tc += 1; say "tc =" tc; call r1 1, 2; say result

-- 1 arg + 1 extra
9:  tc += 1; say "tc =" tc; call rd; say result
10: tc += 1; say "tc =" tc; call rd , ; say result
11: tc += 1; say "tc =" tc; call rd , 2; say result
12: tc += 1; say "tc =" tc; call rd 1; say result
13: tc += 1; say "tc =" tc; call rd 1, ; say result
14: tc += 1; say "tc =" tc; call rd 1, 2; say result

-- 2 args + 1 extra
15: tc += 1; say "tc =" tc; call r00; say result
16: tc += 1; say "tc =" tc; call r00 , ; say result
17: tc += 1; say "tc =" tc; call r00 , , ; say result
18: tc += 1; say "tc =" tc; call r00 , , 3; say result
19: tc += 1; say "tc =" tc; call r00 , 2; say result
20: tc += 1; say "tc =" tc; call r00 , 2, ; say result
21: tc += 1; say "tc =" tc; call r00 , 2, 3; say result
22: tc += 1; say "tc =" tc; call r00 1; say result
23: tc += 1; say "tc =" tc; call r00 1, ; say result
24: tc += 1; say "tc =" tc; call r00 1, , ; say result
25: tc += 1; say "tc =" tc; call r00 1, , 3; say result
26: tc += 1; say "tc =" tc; call r00 1, 2; say result
27: tc += 1; say "tc =" tc; call r00 1, 2, ; say result
28: tc += 1; say "tc =" tc; call r00 1, 2, 3; say result

-- todo: call r01, use call r00 as model (14 calls), adjust the labels (29: to 42:)

-- todo: call r0d, use call r00 as model (14 calls), adjust the labels (43: to 56:)

-- todo: call r10, use call r00 as model (14 calls), adjust the labels (57: to 70:)

-- todo: call r11, use call r00 as model (14 calls), adjust the labels (71: to 84:)

-- todo: call r1d, use call r00 as model (14 calls), adjust the labels (85: to 98:)

-- todo: call rd0, use call r00 as model (14 calls), adjust the labels (99: to 112:)

-- todo: call rd1, use call r00 as model (14 calls), adjust the labels (113: to 126:)

-- todo: call rdd, use call r00 as model (14 calls), adjust the labels (127: to 140:)

-- From here, we reach the test cases with 4 arguments on caller side.
-- I won't detail each test case, you should be able to deduce them.
-- todo:
-- call the routines r000, r001, r00d, etc. up to rddd (see the routines below).
-- For each routine, there are 30 calls. They are described below, in the comment "caller side: 3 args + 1 extra".
-- Adjust the labels.

-- todo: replace 29: by the right label (latest declared + 1)
29: exit

syntax:
    call sayCondition condition("O")
    signal on syntax
    signal value tc + 1


::routine sayCondition
    use strict arg condition
    if condition == .nil then return

    if condition~condition <> "SYNTAX" then .error~say(finalDot(condition~condition))
    if condition~description <> .nil, condition~description <> "" then .error~say(finalDot(condition~description))

    -- For SYNTAX conditions
    if condition~message <> .nil then .error~say(finalDot(condition~message))
    else if condition~errortext <> .nil then .error~say(finalDot(condition~errortext))
    if condition~code <> .nil then .error~say("Error code=" condition~code)

-- ooRexx 4.2 doesnt display a final dot.
-- ooRexx 5.2 displays a final dot.
-- Must harmonize for diff.
::routine finalDot
    use strict arg string
    if string~right(1) = "." then return string
    return string"."


::routine r0
    say arg(); use strict arg; return arg()

::routine r1
    say arg(); use strict arg a1; return arg()

::routine rd
    say arg(); use strict arg a1=1; return arg()

::routine r00
    say arg(); use strict arg , ; return arg()

::routine r01
    say arg(); use strict arg , a2; return arg()

::routine r0d
    say arg(); use strict arg , a2=2; return arg()

::routine r10
    say arg(); use strict arg a1, ; return arg()

::routine r11
    say arg(); use strict arg a1, a2; return arg()

::routine r1d
    say arg(); use strict arg a1, a2=2; return arg()

::routine rd0
    say arg(); use strict arg a1=1, ; return arg()

::routine rd1
    say arg(); use strict arg a1=1, a2; return arg()

::routine rdd
    say arg(); use strict arg a1=1, a2=1; return arg()

::routine r000
    say arg(); use strict arg , , ; return arg()

::routine r001
    say arg(); use strict arg , , a3; return arg()

::routine r00d
    say arg(); use strict arg , , a3=3; return arg()

::routine r010
    say arg(); use strict arg , a2, ; return arg()

::routine r011
    say arg(); use strict arg , a2, a3; return arg()

::routine r01d
    say arg(); use strict arg , a2, a3=3; return arg()

::routine r0d0
    say arg(); use strict arg , a2=2, ; return arg()

::routine r0d1
    say arg(); use strict arg , a2=2, a3; return arg()

::routine r0dd
    say arg(); use strict arg , a2=2, a3=3; return arg()

::routine r100
    say arg(); use strict arg a1, , ; return arg()

::routine r101
    say arg(); use strict arg a1, , a3; return arg()

::routine r10d
    say arg(); use strict arg a1, , a3=3; return arg()

::routine r110
    say arg(); use strict arg a1, a2, ; return arg()

::routine r111
    say arg(); use strict arg a1, a2, a3; return arg()

::routine r11d
    say arg(); use strict arg a1, a2, a3=3; return arg()

::routine r1d0
    say arg(); use strict arg a1, a2=2, ; return arg()

::routine r1d1
    say arg(); use strict arg a1, a2=2, a3; return arg()

::routine r1dd
    say arg(); use strict arg a1, a2=2, a3=3; return arg()

::routine rd00
    say arg(); use strict arg a1=1, , ; return arg()

::routine rd01
    say arg(); use strict arg a1=1, , a3; return arg()

::routine rd0d
    say arg(); use strict arg a1=1, , a3=3; return arg()

::routine rd10
    say arg(); use strict arg a1=1, a2, ; return arg()

::routine rd11
    say arg(); use strict arg a1=1, a2, a3; return arg()

::routine rd1d
    say arg(); use strict arg a1=1, a2, a3=3; return arg()

::routine rdd0
    say arg(); use strict arg a1=1, a2=2, ; return arg()

::routine rdd1
    say arg(); use strict arg a1=1, a2=2, a3; return arg()

::routine rddd
    say arg(); use strict arg a1=1, a2=2, a3=3; return arg()


/*
callee side
trinary: 0 1 d
    0   name not declared
    1   name declared
    d   name declared with default value

-- callee side: 1 arg
-- 3 combinations
0
1
d

-- callee side: 2 args
-- 3*3 combinations
00
01
0d
10
11
1d
d0
d1
dd

-- callee side: 3 args
-- 3*3*3 combinations
000
001
00d
010
011
01d
0d0
0d1
0dd
100
101
10d
110
111
11d
1d0
1d1
1dd
d00
d01
d0d
d10
d11
d1d
dd0
dd1
ddd
*/


/*
caller side: 2 args + 1 extra
    0: the argument is omitted
    1: the argument is passed

000
r
r ,
r , ,

001
r , , 3

010
r , 2
r , 2,

011
r , 2, 3

100
r 1
r 1,
r 1, ,

101
r 1, , 3

110
r 1, 2
r 1, 2,

111
r 1, 2, 3
*/

/*
caller side: 3 args + 1 extra
    0: the argument is omitted
    1: the argument is passed

0000
r
r ,
r , ,
r , , ,

0001
r , , , 4

0010
r , , 3
r , , 3,

0011
r , , 3, 4

0100
r , 2
r , 2,
r , 2, ,

0101
r , 2, , 4

0110
r , 2, 3
r , 2, 3,

0111
r , 2, 3, 4

1000
r 1
r 1,
r 1, ,
r 1, , ,

1001
r 1, , , 4

1010
r 1, , 3
r 1, , 3,

1011
r 1, , 3, 4

1100
r 1, 2
r 1, 2,
r 1, 2, ,

1101
r 1, 2, , 4

1110
r 1, 2, 3
r 1, 2, 3,

1111
r 1, 2, 3, 4
*/