-- http://www.craig-wood.com/nick/articles/pi-chudnovsky/

use arg digits = 10000
/*
    Compute int(pi * 10**digits)

    This is done using Chudnovsky's series with binary splitting
*/
call time "r"
z13591409 = .z[13591409]; z545140134 = .z[545140134]
    numeric digits 30
    C = .z[640320]
    C3_OVER_24 = C**3 % 24
    -- how many terms to compute
    DIGITS_PER_TERM = math.log10(C3_OVER_24%6%2%6)
    N = trunc(digits/DIGITS_PER_TERM + 1)
say "digits/term =" DIGITS_PER_TERM", N =" N 
    -- Calculate P(0,N) and Q(0,N)
    split = bs(0, N); P = split[1]; Q = split[2]; T = split[3] 
    one_squared = .z[10]**(2*digits)
    sqrtC = (one_squared*10005)~sqrt()
    pi = (Q*426880*sqrtC) % T
    elapsed = format(time("e"),,2)
    pi = pi~string
    say elapsed pi~length "digits" pi~left(50) ".."
    exit

    bs: procedure expose C3_OVER_24 z13591409 z545140134; use arg a, b 
        /*
        Computes the terms for binary splitting the Chudnovsky infinite series

        a(a) = +/- (13591409 + 545140134*a)
        p(a) = (6*a-5)*(2*a-1)*(6*a-1)
        b(a) = 1
        q(a) = a*a*a*C3_OVER_24

        returns P(a,b), Q(a,b) and T(a,b)
        */
        if b - a == 1 then do
            -- Directly compute P(a,a+1), Q(a,a+1) and T(a,a+1)
            if a == 0 then do
                Pab = .z[1]; Qab = .z[Pab]
            end; else; do
                Pab = .z[6*a-5]*(2*a-1)*(6*a-1)
                Qab = .z[a]*a*a*C3_OVER_24
            end
            Tab = Pab * (z13591409 + z545140134*a) -- a(a) * p(a)
            if a // 2 = 1 then
                Tab = -Tab
        end
        else do
            -- Recursively compute P(a,b), Q(a,b) and T(a,b)
            -- m is the midpoint of a and b
            m = (a + b) % 2
            -- Recursively calculate P(a,m), Q(a,m) and T(a,m)
            split = bs(a, m); Pam = split[1]; Qam = split[2]; Tam = split[3] 
            -- Recursively calculate P(m,b), Q(m,b) and T(m,b)
            split = bs(m, b); Pmb = split[1]; Qmb = split[2]; Tmb = split[3]
            -- Now combine
            Pab = Pam * Pmb
            Qab = Qam * Qmb
            Tab = Qmb * Tam + Pam * Tmb
        end
        return .Array~of(Pab, Qab, Tab)

--::options trace r
::routine math.log10 public external "library rxmath RxCalcLog10"
::requires mpzlib
