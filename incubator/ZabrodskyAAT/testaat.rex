#!/usr/bin/rexx
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


say
call test_gensub
call test_permutation
call test_fact
call test_stirling
call test_fib
call test_general_fib
call test_ncomb
call test_primes
call test_r2d
call test_d2r
call test_heron
call test_exp
call test_floor
call test_ceiling
call test_mod
call test_ln
call test_sin
call test_cos
call test_sqrt
call test_power
call test_repower
return


::requires 'ZabrodskyAAT.cls'


::routine test_gensub
say 'Testing GENSUB.'
ta = .array~new()
ta~append('1 2 3 4 5')
ta~append('1 2 3 4 6')
ta~append('1 2 3 5 6')
ta~append('1 2 4 5 6')
ta~append('1 3 4 5 6')
ta~append('2 3 4 5 6')
signal on error
ra = .ZabrodskyAAT~gensub(6, 5)
ok = 1
do i = 1 to ta~items()
   if ta[i] <> ra[i] then ok = 0
   end
if ok then say 'GENSUB succeeded.'
else say 'GENSUB was in error.'
say
return

error:
say 'GENSUB argument error.'
say
return


::routine test_permutation
say 'Testing PERMUTATION.'
ta = .array~new()
ta~append('1 2 3')
ta~append('2 1 3')
ta~append('2 3 1')
ta~append('3 2 1')
ta~append('3 1 2')
ta~append('1 3 2')
signal on error
ra = .ZabrodskyAAT~permutation(3)
ok = 1
do i = 1 to ta~items()
   if ta[i] <> ra[i] then ok = 0
   end
if ok then say 'PERMUTATION succeeded.'
else say 'PERMUTATION was in error.'
say
return

error:
say 'PERMUTATION argument error.'
say
return


::routine test_fact
say 'Testing FACT.'
numeric digits 32
ta = 8.1428599075279953478037134061743E+496713
signal on error
ra = .ZabrodskyAAT~fact(108001, digits())
if ta = ra then say 'FACT succeeded.'
else do
   say 'FACT was in error.'
   say 'expected' ta
   say 'actual  ' ra
   end
say
return

error:
say 'FACT argument error.'
say
return


::routine test_stirling
say 'Testing STIRLING.'
numeric digits 32
ta = 8.1428599075255713876213611726275E+496713
signal on error
ra = .ZabrodskyAAT~stirling(108001, digits())
if ta = ra then say 'STIRLING succeeded.'
else do
   say 'STIRLING was in error.'
   say 'expected' ta
   say 'actual  ' ra
   end
say
return

error:
say 'STIRLING argument error.'
say
return


::routine test_fib
say 'Testing FIB.'
numeric digits 32
signal on error
ra = .ZabrodskyAAT~fib(10000, digits())
-- say ra
say
return

error:
say 'FIB argument error.'
say
return


::routine test_general_fib
say 'Testing GENERAL_FIB.'
numeric digits 32
signal on error
ra = .ZabrodskyAAT~general_fib(10000, 2100, digits())
-- say ra
say
return

error:
say 'GENERAL_FIB argument error.'
say
return


::routine test_ncomb
say 'Testing NCOMB.'
numeric digits 32
signal on error
ra = .ZabrodskyAAT~ncomb(6, 4)
-- say ra
say
return

error:
say 'NCOMB argument error.'
say
return


::routine test_primes
say 'Testing PRIMES.'
numeric digits 32
signal on error
P = 300
ra = .ZabrodskyAAT~primes(P, digits())
if P = ra~items() then say 'PRIMES succeeded.'
else do
   say 'PRIMES was in error.'
   say 'expected' p
   say 'actual  ' ra~items()
   end
say
return

error:
say 'PRIMES argument error.'
say
return


::routine test_r2d
say 'Testing R2D.'
signal on error
P = 'MMX'
ra = .ZabrodskyAAT~r2d(P)
if ra = 2010 then say 'R2D succeeded.'
else do
   say 'R2D was in error.'
   say 'expected 2010'
   say 'actual  ' ra
   end
say
return

error:
say 'D2R argument error.'
say
return


::routine test_d2r
say 'Testing D2R.'
signal on error
P = 2010
ra = .ZabrodskyAAT~d2r(P)
if ra = 'MMX' then say 'D2R succeeded.'
else do
   say 'D2R was in error.'
   say 'expected MMX'
   say 'actual  ' ra
   end
say
return

error:
say 'D2R argument error.'
say
return


::routine test_heron
say 'Testing HERON.'
signal on error
numeric digits 16
ra = .ZabrodskyAAT~heron(2, 4.5, 6.1, digits())
if ra = 3.122883283121545 then say 'HERON succeeded.'
else do
   say 'HERON was in error.'
   say 'expected' 3.122883283121545
   say 'actual  ' ra
   end
say
return

error:
say 'HERON argument error.'
say
return


::routine test_exp
say 'Testing EXP.'
signal on error
numeric digits 16
P = 2.1879653
ra = .ZabrodskyAAT~exp(P, digits())
if ra = 8.917051119945531 then say 'EXP succeeded.'
else do
   say 'EXP was in error.'
   say 'expected' 8.917051119945531
   say 'actual  ' ra
   end
say
return

error:
say 'EXP argument error.'
say
return


::routine test_floor
say 'Testing FLOOR.'
signal on error
P = 2.1879653
ra = .ZabrodskyAAT~floor(P)
if ra = 2 then say 'FLOOR succeeded.'
else do
   say 'FLOOR was in error.'
   say 'expected' 2
   say 'actual  ' ra
   end
say
return

error:
say 'FLOOR argument error.'
say
return


::routine test_ceiling
say 'Testing FLOOR.'
signal on error
P = 2.1879653
ra = .ZabrodskyAAT~ceiling(P)
if ra = 3 then say 'CEILING succeeded.'
else do
   say 'CEILING was in error.'
   say 'expected' 3
   say 'actual  ' ra
   end
say
return

error:
say 'CEILING argument error.'
say
return


::routine test_mod
say 'Testing MOD.'
signal on error
ok = 1
ra = .ZabrodskyAAT~mod(5, -3)
if ra <> -1 then do
   ok = 0
   say 'MOD was in error.'
   say 'expected -1'
   say 'actual  ' ra
   end
ra = .ZabrodskyAAT~mod(18, -3)
if ra <> 0 then do
   ok = 0
   say 'MOD was in error.'
   say 'expected 0'
   say 'actual  ' ra
   end
ra = .ZabrodskyAAT~mod(0.11, -0.1)
if ra <> -0.09 then do
   ok = 0
   say 'MOD was in error.'
   say 'expected 0'
   say 'actual  ' ra
   end
if ok then say 'MOD succeded.'
say
return

error:
say 'CEILING argument error.'
say
return


::routine test_ln
say 'Testing LN.'
signal on error
P = 20
ra = .ZabrodskyAAT~ln(P, digits())
if ra = 2.99573227 then say 'LN succeded.'
else do
   ok = 0
   say 'LN was in error.'
   say 'expected 2.99573227'
   say 'actual  ' ra
   end
say
return

error:
say 'LN argument error.'
say
return


::routine test_sin
say 'Testing SIN.'
signal on error
P = 20.556
ra = .ZabrodskyAAT~sin(P, digits())
if ra = 0.990813948 then say 'SIN succeded.'
else do
   ok = 0
   say 'SIN was in error.'
   say 'expected 0.990813948'
   say 'actual  ' ra
   end
say
return

error:
say 'SIN argument error.'
say
return


::routine test_cos
say 'Testing COS.'
signal on error
P = 20.556
ra = .ZabrodskyAAT~cos(P, digits())
if ra = -0.135232130 then say 'COS succeded.'
else do
   ok = 0
   say 'COS was in error.'
   say 'expected -0.135232130'
   say 'actual  ' ra
   end
say
return

error:
say 'COS argument error.'
say
return                 556
ra = .


::routine test_sqrt
say 'Testing SQRT.'
signal on error
P = 20.556
ra = .ZabrodskyAAT~sqrt(P, digits())
if ra = 4.53387252 then say 'SQRT succeded.'
else do
   ok = 0
   say 'SQRT was in error.'
   say 'expected 4.53387252'
   say 'actual  ' ra
   end
say
return

error:
say 'SQRT argument error.'
say
return


::routine test_power
say 'Testing POWER.'
signal on error
numeric digits 20
ra = .ZabrodskyAAT~power(1 + 1 / 1E+9, 1E+9, digits())
if ra = 2.7182818270997410950 then say 'POWER succeded.'
else do
   ok = 0
   say 'POWER was in error.'
   say 'expected 2.7182818270997410950'
   say 'actual  ' ra
   end
say
return

error:
say 'POWER argument error.'
say
return


::routine test_repower
say 'Testing REPOWER.'
signal on error
numeric digits 20
ra = .ZabrodskyAAT~repower(2.666666, 5, digits())
if ra = 134.847568065927901213498 then say 'REPOWER succeded.'
else do
   ok = 0
   say 'REPOWER was in error.'
   say 'expected 134.847568065927901213498'
   say 'actual  ' ra
   end
say
return

error:
say 'REPOWER argument error.'
say
return

