/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2010-2010 Rexx Language Association. All rights reserved.    */
/*                                                                            */
/* This program and the accompanying materials are made available under       */
/* the terms of the Common Public License v1.0 which accompanies this         */
/* distribution. A copy is also available at the following address:           */
/* http://www.ibm.com/developerworks/oss/CPLv1.0.htm                          */
/*                                                                            */
/* Redistribution and use in source and binary forms, with or                 */
/* without modification, are permitted provided that the following            */
/* conditions are met:                                                        */
/*                                                                            */
/* Redistributions of source code must retain the above copyright             */
/* notice, this list of conditions and the following disclaimer.              */
/* Redistributions in binary form must reproduce the above copyright          */
/* notice, this list of conditions and the following disclaimer in            */
/* the documentation and/or other materials provided with the distribution.   */
/*                                                                            */
/* Neither the name of Rexx Language Association nor the names                */
/* of its contributors may be used to endorse or promote products             */
/* derived from this software without specific prior written permission.      */
/*                                                                            */
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS        */
/* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT          */
/* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS          */
/* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT   */
/* OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,      */
/* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED   */
/* TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,        */
/* OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY     */
/* OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING    */
/* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS         */
/* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.               */
/*                                                                            */
/*----------------------------------------------------------------------------*/


-- These methods are taken from Vladimir Zabrodsky's web site
-- http://dhost.info/zabrodskyvlada/aat/index.html. Vladimir granted
-- permission to use the code on that web site under the terms of the CPL v1.0
-- on Nov 16, 2010. A copy of that grant resides in the ooRexx developer
-- mailing list archives.
--
-- Vladimir's code has been slightly modified, mostly for performance reasons,
-- to fit better into the ooRexx architecture. Most of the Procedures have been
-- converted to methods and other small changes have been made. But the code
-- is still easily recognisable.


::class ZabrodskyAAT public


/*----------------------------------------------------------------------------*/
/* Routine: gensub                                                            */
/*----------------------------------------------------------------------------*/
::method gensub class
use strict arg N, K
A = .array~new(K)
Z = .array~new(K)
if \N~datatype('W') then raise error 93.907 array(1, N) RETURN (Z)
if \K~datatype('W') then raise error 93.907 array(2, K) RETURN (Z)
if N < 0 then raise error 93.906 array(1, N) RETURN (Z)
if K > N then raise error 93.908 array(1, K, N) RETURN (Z)
do J = 1 to K
   A[J] = J
   end
P = 1
do while P >= 1
   SubSet = A[K]
   do J = K - 1 to 1 by -1
      SubSet = A[J] SubSet
      end
--   say SubSet
   Z~append(SubSet)
   if A[K] = N then P = P - 1
   else P = K
   if P >= 1 then do J = K to P by -1
      A[J] = A[P] + J - P + 1
      end
   end
return Z


/*----------------------------------------------------------------------------*/
/* Routine: permutation                                                       */
/*----------------------------------------------------------------------------*/
::method permutation class
use strict arg N
Z = .array~new()
if \N~datatype('W') then raise error 88.903 array(1, N) RETURN (Z)
if N < 2 then raise error 88.906 array(1, 2, N) RETURN (Z)
C. = 1
Pr. = 1
do J = 1 to N
   P.J = J
   end
C.N = 0
-- say DISPLAY(N)
Z~append(DISPLAY(N))
J = 1
do while J < N
   X = 0
   do J = 1 while C.J = N - J + 1
      Pr.J = \Pr.J; C.J = 1
      if Pr.J then X = X + 1
      end
   if J < N then do
      if Pr.J then K = C.J + X
      else K = N - J + 1 - C.J + X
      Kp1 = K + 1; W = P.K
      P.K = P.Kp1; P.Kp1 = W
--      say DISPLAY(N)
      Z~append(DISPLAY(N))
      C.J = C.J + 1
      end
   end
return Z
 
DISPLAY: procedure expose P.
parse arg N
Permutation = P.N
do J = N - 1 to 1 by -1
   Permutation = P.J Permutation
   end
return Permutation 


/*----------------------------------------------------------------------------*/
/* Routine: fact                                                              */
/*----------------------------------------------------------------------------*/
::method fact class
use strict arg N, P = 9
if \N~datatype('W') then raise error 88.903 array(1, N) RETURN (Z)
if \P~datatype('W') then raise error 88.903 array(2, P) RETURN (Z)
if N <= 0 then raise error 88.907 array(1, N) RETURN (Z)
if P > 9 then numeric digits P
F = 1
do J = 2 to N
   F = F * J
   end
return F 


/*----------------------------------------------------------------------------*/
/* Routine: stirling                                                          */
/*----------------------------------------------------------------------------*/
::method stirling class
use strict arg N, P, Q = 9
if \N~datatype('W') then raise error 88.903 array(1, N) RETURN (-1)
if \P~datatype('W') then raise error 88.903 array(2, P) RETURN (-1)
if N <= 0 then raise error 88.907 array(1, N) RETURN (Z)
if Q > 9 then numeric digits Q
Pi = self~PI(P)
E = self~E(P)
Sqrt2pin = self~SQRT(2 * Pi * N, P)
return Sqrt2pin * (N / E)**N * (1 + 1 / (12 * N))


/*----------------------------------------------------------------------------*/
/* Routine: fib                                                               */
/*----------------------------------------------------------------------------*/
::method fib class
use strict arg N, P = 9
if \N~datatype('W') then raise error 88.903 array(1, N) RETURN (-1)
if N = 0 | N = 1 then return N
if P > 9 then numeric digits P
A = 0
B = 1
do N - 1
   parse value(B (B + A)) with A B
   end
return B 


/*----------------------------------------------------------------------------*/
/* Routine: general_fib                                                       */
/*----------------------------------------------------------------------------*/
::method general_fib class
use strict arg N, P, Q = 9
if \N~datatype('W') then raise error 88.903 array(1, N) RETURN ('')
if \P~datatype('W') then raise error 88.903 array(2, P) RETURN ('')
if N = 1 then return 1
if Q > 9 then numeric digits Q
Sqrt5 = self~SQRT(5, P)
C = (1 + Sqrt5) / 2
return FORMAT(C ** N / Sqrt5,,0) 


/*----------------------------------------------------------------------------*/
/* Routine: ncomb                                                             */
/*----------------------------------------------------------------------------*/
::method ncomb class
use strict arg N, K
if \N~datatype('W') then raise error 88.903 array(1, N) RETURN ('')
if \K~datatype('W') then raise error 88.903 array(2, K) RETURN ('')
if N < 0 then raise error 88.907 array(1, N) RETURN ('')
if K < 0 | N < K then return 0
if K = N then return 1
if K > N - K then K = N - K
Nom = 1
do J = N - K + 1 to N
   Nom = Nom * J
   end
return Nom / self~FACT(K) 


/*----------------------------------------------------------------------------*/
/* Routine: primes                                                            */
/*----------------------------------------------------------------------------*/
::method primes class
use strict arg K, Q = 9
if \K~datatype('W') then raise error 88.903 array(1, K) RETURN ('')
if K < 0 then raise error 88.907 array(1, K) RETURN ('')
P = .array~new()
if Q > 9 then numeric digits Q
P[1] = 2
N = 3
do J = 2 to K
   P[J] = N
   do forever
      N = N + 2
      do L = 2 until R = 0
         Q = N % P[L]
         R = N // P[L]
         if Q <= P[L] & R <> 0 then iterate J
         end
      end
   end
return P


/*----------------------------------------------------------------------------*/
/* Routine: r2d                                                               */
/*----------------------------------------------------------------------------*/
::method r2d class
use strict arg Roman
Roman = Roman~upper()
RtoD.I = 1; RtoD.V = 5; RtoD.X = 10
RtoD.L = 50; RtoD.C = 100
RtoD.D = 500; RtoD.M = 1000
Decimal = 0; Rdigit = LEFT(Roman, 1)
Ddigit = RtoD.Rdigit
do J = 2 to LENGTH(Roman)
  Rdigit = SUBSTR(Roman, J, 1)
  Next = RtoD.Rdigit
  if Next > Ddigit
    then Decimal = Decimal - Ddigit
    else Decimal = Decimal + Ddigit
  Ddigit = Next
end
return Decimal + Ddigit 


/*----------------------------------------------------------------------------*/
/* Routine: d2r                                                               */
/*----------------------------------------------------------------------------*/
::method d2r class
use strict arg Decimal
if Decimal < 1 then raise error 88.907 array(1, Decimal) RETURN ('')
if Decimal > 3999 then raise error 88.908 array(1, 3999, Decimal) RETURN ('')
parse value RIGHT(FORMAT(Decimal, , 0), 4, 0),
  with A +1 B +1 C +1 D
parse value "1 1 1 1 3 4 4 4 4 7",
  with X.0 X.1 X.2 X.3 X.4 X.5 X.6 X.7 X.8 X.9
parse value "0 1 2 3 2 1 2 3 4 2",
  with Y.0 Y.1 Y.2 Y.3 Y.4 Y.5 Y.6 Y.7 Y.8 Y.9
R = "IIIVIIIXXXLXXXCCCDCCCMMM"
/*            1         2      */
/*   123456789012345678901234  */
return SUBSTR(R, X.A + 21, Y.A) || SUBSTR(R, X.B + 14, Y.B),
    || SUBSTR(R, X.C +  7, Y.C) || SUBSTR(R, X.D, Y.D)


/*----------------------------------------------------------------------------*/
/* Routine: heron                                                             */
/*----------------------------------------------------------------------------*/
::method heron class
use strict arg X, Y, Z, P = 9
numeric digits P
S = (X + Y + Z) / 2
return self~SQRT(S * (S - X) * (S - Y) * (S - Z), P) 


/*----------------------------------------------------------------------------*/
/* Routine: e                                                                 */
/*----------------------------------------------------------------------------*/
::method e class
return self~econst()


/*----------------------------------------------------------------------------*/
/* Routine: econst                                                            */
/*----------------------------------------------------------------------------*/
::method econst class
use strict arg
V = 2.718281828459045235360287471
V = V || 35266249775724709369995957496
V = V || 69676277240766303535475945713
V = V || 82178525166427427466391932003
V = V || 05992181741359662904357290033
V = V || 42952605956307381323286279434
V = V || 90763233829880753195251019012
return V 


/*----------------------------------------------------------------------------*/
/* Routine: exp                                                               */
/*----------------------------------------------------------------------------*/
::method exp class
use strict arg X, P = 9
if \X~datatype('N') then raise error 88.904 array(1, X) RETURN ('')
if \P~datatype('W') then raise error 88.907 array(2, P) RETURN ('')
if P > 9 then numeric digits P
Y = X % 1
if ABS(X - Y) > 0.5 then Y = Y + SIGN(X)
X = X - Y
Sum = 1; Term = 1
do J = 1
   Term = Term * X / J
   NewSum = Sum + Term
   if NewSum = Sum then leave
   Sum = NewSum
   end
if Y = 0 then return Sum
return (self~ECONST() ** Y) * Sum 


/*----------------------------------------------------------------------------*/
/* Routine: floor                                                             */
/*----------------------------------------------------------------------------*/
::method floor class
use strict arg F
if \F~datatype('N') then raise error 88.904 array(1, F) RETURN ('')
return TRUNC(F) - (F < 0) * (F <> TRUNC(F))
 

/*----------------------------------------------------------------------------*/
/* Routine: ceiling                                                           */
/*----------------------------------------------------------------------------*/
::method ceiling class
use strict arg C
if \C~datatype('N') then raise error 88.904 array(1, C) RETURN ('')
return TRUNC(C) + (C > 0) * (C <> TRUNC(C)) 
 

/*----------------------------------------------------------------------------*/
/* Routine: mod                                                               */
/*----------------------------------------------------------------------------*/
::method mod class
use strict arg X, Y
if \X~datatype('N') then raise error 88.904 array(1, X) RETURN ('')
if \Y~datatype('N') then raise error 88.904 array(2, Y) RETURN ('')
if Y = 0 then return X
return X - Y * self~FLOOR(X/Y)
 

/*----------------------------------------------------------------------------*/
/* Routine: ln                                                                */
/*----------------------------------------------------------------------------*/
::method ln class
use strict arg X, P = 9
if \X~datatype('N') then raise error 88.904 array(1, X) RETURN ('')
if \P~datatype('W') then raise error 88.907 array(2, P) RETURN ('')
numeric digits P
if X < 1 then return - self~LN(1 / X, P)
do M = 0 until (2 ** M) > X
   end
M = M - 1
Z = X / (2 ** M)
Zeta = (1 - Z) / (1 + Z)
N = Zeta
Ln = Zeta
Zetasup2 = Zeta * Zeta
do J = 1
   N = N * Zetasup2; NewLn = Ln + N / (2 * J + 1)
   if NewLn = Ln then return M * LN2P(P) - 2 * Ln
   Ln = NewLn
   end 

LN2P: procedure expose self
parse arg P
if P <= 200 then return self~LN2()
N = 1 / 3; Ln = N; Zetasup2 = 1 / 9
do J = 1
  N = N * Zetasup2; NewLn = Ln + N / (2 * J + 1)
  if NewLn = Ln then return 2 * Ln
  Ln = NewLn
end 
 

/*----------------------------------------------------------------------------*/
/* Routine: ln2                                                               */
/*----------------------------------------------------------------------------*/
::method ln2 class
use strict arg
V = 0.69314718055994530941723212145817656807
V = V || 5500134360255254120680009493393621969694
V = V || 7156058633269964186875420014810205706857
V = V || 3368552023575813055703267075163507596193
V = V || 0727570828371435190307038623891673471123350
return V 
 

/*----------------------------------------------------------------------------*/
/* Routine: ln10                                                              */
/*----------------------------------------------------------------------------*/
::method ln10 class
use strict arg
V = 2.30258509299404568401799145468436420760
V = V || 1101488628772976033327900967572609677352
V = V || 4802359972050895982983419677840422862486
V = V || 3340952546508280675666628736909878168948
V = V || 2907208325554680843799894826233198528393505
return V 


/*----------------------------------------------------------------------------*/
/* Routine: sin                                                               */
/*----------------------------------------------------------------------------*/
::method sin class
use strict arg X, P = 9
if \X~datatype('N') then raise error 88.904 array(1, X) RETURN ('')
if \P~datatype('W') then raise error 88.907 array(2, P) RETURN ('')
numeric digits P
Pi = self~PICONST()
Signum = 1
if X < 0 then do
   Signum = -1
   X = -X
   end
Pim2 = Pi * 2
X = X // Pim2
Pid2 = Pi / 2
if X > Pi then do
   X = X - Pi
   Signum = -Signum
   end
if X > Pid2 then X = Pi - X
Term = X
Xsup2 = X * X
Sum = X
F = 1
do J = 3 by 2
  Term = -Term * Xsup2 / (J * (J - 1))
  NewSum = Sum + Term
  if NewSum = Sum then return Signum * Sum
  Sum = NewSum
  end 


/*----------------------------------------------------------------------------*/
/* Routine: cos                                                               */
/*----------------------------------------------------------------------------*/
::method cos class
use strict arg X, P = 9
if \X~datatype('N') then raise error 88.904 array(1, X) RETURN ('')
if \P~datatype('W') then raise error 88.907 array(2, P) RETURN ('')
numeric digits P
Pi = self~PICONST()
Signum = 1
X = ABS(X)
Pim2 = Pi * 2
X = X // Pim2
Pid2 = Pi / 2
if X > Pi then do
   X = X - Pi
   Signum = -Signum
    end
if X > Pid2 then do
   X = Pi - X
   Signum = -Signum
   end
Term = 1
Xsup2 = X * X
Sum = 1
F = 1
do J = 2 by 2
  Term = -Term * Xsup2 / (J * (J - 1))
  NewSum = Sum + Term
  if NewSum = Sum then return Signum * Sum
  Sum = NewSum
  end


/*----------------------------------------------------------------------------*/
/* Routine: sqrt                                                              */
/*----------------------------------------------------------------------------*/
::method sqrt class
use strict arg N, P = 9
if \N~datatype('N') then raise error 88.904 array(1, N) RETURN ('')
if \P~datatype('W') then raise error 88.907 array(2, P) RETURN ('')
if N < 0 then raise error 88.906 array(1, N) RETURN ('')
numeric digits P
parse value FORMAT(N,,,,0) with N "E" Exp
if Exp = "" then Exp = 0
if (Exp // 2) <> 0 then if Exp > 0 then do
   N = N * 10
   Exp = Exp - 1
   end
else do
   N = N / 10
   Exp = Exp + 1
   end
X = 0.5 * (N + 1)
do forever
   NewX = 0.5 * (X + N / X)
   if X = NewX then return X * 10 ** (Exp % 2)
   X = NewX
   end


/*----------------------------------------------------------------------------*/
/* Routine: power                                                             */
/*----------------------------------------------------------------------------*/
::method power class
use strict arg X, Z, P = 9
if \X~datatype('N') then raise error 88.904 array(1, X) RETURN ('')
if \Z~datatype('N') then raise error 88.904 array(2, Z) RETURN ('')
if \P~datatype('W') then raise error 88.907 array(3, P) RETURN ('')
numeric digits P
if Z < 0 then raise error 88.906 array(1, Z) RETURN ('')
Pwr = 1
do forever
   if Z // 2 then Pwr = Pwr * X
   Z = Z % 2
   if Z = 0 then return Pwr
   X = X * X
   end 


/*----------------------------------------------------------------------------*/
/* Routine: power                                                             */
/*----------------------------------------------------------------------------*/
::method repower class
use strict arg X, Z, P = 9
if \X~datatype('N') then raise error 88.904 array(1, X) RETURN ('')
if \Z~datatype('W') then raise error 88.905 array(2, Z) RETURN ('')
if \P~datatype('W') then raise error 88.907 array(3, P) RETURN ('')
P = P + 4
numeric digits P
if DATATYPE(Z, "W")
  then
    if X = 0 & Z <= 0
      then call ERROR "REPOWER: Result is undefined"
      else return X ** Z
  else
    select
      when X > 0 then return self~EXP(Z * self~LN(X, P), P)
      when X = 0 & Z > 0 then return 0
      otherwise
        call ERROR "REPOWER: Result is undefined"
    end
 
ERROR: say ARG(1); exit 


/*----------------------------------------------------------------------------*/
/* Routine: pi                                                                */
/*----------------------------------------------------------------------------*/
::method pi class
use strict arg
return self~piconst()


/*----------------------------------------------------------------------------*/
/* Routine: piconst                                                           */
/*----------------------------------------------------------------------------*/
::method piconst class
use strict arg
V = 3.14159265358979323846264338327950288419
V = V || 7169399375105820974944592307816406286208
V = V || 9986280348253421170679821480865132823066
V = V || 4709384460955058223172535940812848111745
V = V || 028410270193852110555964462294895493038196
return V 

