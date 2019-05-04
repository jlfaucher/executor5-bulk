/* GMP mpz library */

/*
An ooRexx interface library to the GNU Multiple Precision Arithmetic Library
(GMP, https://gmplib.org/), more specifically to the high-level signed integer
arithmetic functions (mpz).

We implement both a method and a routine interface.  The method interface
is more ooRexx-like and easier to convert to from existing ooRexx code,
whereas the routine interface features the typical C-style which makes it
easy to convert from existing C code.  As the method interface creates
intermediate return instances, performance of the routine interface is
generally superior.  For comparison, on some arbitrary hardware, you might
see the following elapsed-time measurements:
  "n += 1":      11 microseconds    "mpz_add_ui n, n, 1":   3 microseconds
  "n~nextPrime": 21 microseconds    "mpz_next_prime n, n": 13 microseconds

For Windows, instead of GMP, we're using MPIR, a GMP drop-in replacement.
(on http://mpir.org/ download and unpack "Source zip file", download "Documentation"
and follow steps 2.4 Building with Microsoft Visual Studio:
1. from http://yasm.tortall.net/Download.html download "Win64 VS2010 .zip", unpack and
   copy vsyasm.exe to "C:\Program Files\yasm" (you'll need to create this directory)
   (the following used to work, but now doesn't seem to work any more:
    copy vsyasm.exe to "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\bin")
2. from unpacked MPIR source start build.vc14\mpir.sln (which opens VS 2015)
3. On VS 2015 pulldown "Build" select "Configuration Manager", select "Release/Debug"
   and "Win32/x64", then unselect all but a single dll_mpir project, e.g. dll_mpir_gc
4. press F7 ("Build Solution")
5. in the unpacked MPIR source, under dll\(Win32|x64)\Release|Debug) three files
   will have been built, which are needed to compile and run mpz:
   to compile: gmp.h, mpir.lib; to run: mpir.dll
)
I have neither tested building on Linux, nor building with GMP instead of MPIR.


GENERAL NOTES:

- For performance reasons and simplicity, this interface library tries to avoid
  creating intermediate Z instances for given arguments.  Because of this e. g.

    .z[1] + 1~copies(21)

  which is internally handled by mpz_add_ui, will raise "argument op2 must be
  zero or a positive whole number", as no attempt is made to convert 'op2'
  (which is too long even for uint64) to Z by creating an intermediate Z instance.
  We can of course manually create a Z instance to make this work:

    .z[1] + .z[1~copies(21)]


TO DO:

- add many missing mpz methods: e.g. congruent_2exp, file I/O
  5.11 Logical and Bit Manipulation Functions
  5.12 Input and Output Functions
- add function result(), which returns "lost" results from e. g. mod(), root(), remove()
- it seems that the isInstanceOf() test is relatively expensive (1 microsec each?):
  instead we might a) mark our 'MPZ' struct with (e. g.) "mpz"mpz' and use
  CSELF (or CselfToObject) to see if we can access such a struct, or NULL is returned
- add additional non-GMP convenience methods startsWith, endsWith(), contains()
- test cases
- multi-core chudnovsky
- also implement mpr, mpf (or better, mpfr)
- find out whether we can improve performance by checking arguments for
  'isInteger()' and 'isNumberString()' as ABS() bif does:
    if (isInteger(argument))
      return ((RexxInteger *)argument)->abs();
    else if (isNumberString(argument
      return((NumberString *)argument)->abs();
- can we catch exceptions? we currently explicitly check args instead, because
  there seems to be no portable way of catching zero-divide
- add public method to return shortened Z string representation
- we generally create new result Z instance before checking for valid argmuments; we might want to defer this (low prio)
- find out why we can't use the dynamic MPZ version, instead of the static one
- check how to supprt mpz_xxx_d functions


KNOWN BUGS:

- ooRexx issue with comparisons like .z[17] = 17, which don't call compareTo(); see [bugs:#1365]
  we currently circumvent this with explicit "=" and "\=" methods in mpzlib
  TO DO: fix bug?
- endsWith() doesn't work with negative numbers
- if we just make a single mpz function available by e. g. using
  ::routine mpz_add_ui public external 'library mpz z_rtn_add_ui'
  then the Z class hasn't initialized and calls fail with an exception
  z = .z[1]; call mpz_add_ui z, z, 3
  (the fact that .z[1] works without a ::class z defined, is a quirk here)
  (TO DO: detect NULL message insert)

*/

#include <string.h>
#include <time.h>                      // time()
#include "oorexxapi.h"
#include "gmp.h"
#include "mpz.hpp"



// ---------------------------------------------------------------------
// Global variables

// global class variable to check Rexx arguments against
RexxClassObject TheZClass = NULLOBJECT; // will be set in z_init_cls()

// global randstate variable
gmp_randstate_t RandomState;



// ---------------------------------------------------------------------
// Routine interface


// 5.2 Assignment Functions
// void mpz_set (mpz_t rop, const mpz_t op)
// void mpz_set_ui (mpz_t rop, unsigned long int op)
// void mpz_set_si (mpz_t rop, signed long int op)
// void mpz_set_d (mpz_t rop, double op)
// void mpz_set_q (mpz_t rop, const mpq_t op)
// void mpz_set_f (mpz_t rop, const mpf_t op)
// int mpz_set_str (mpz_t rop, const char *str, int base)
// void mpz_swap (mpz_t rop1, mpz_t rop2)
REXX_ROUTINE_2(RXO, set,       RXO, rop, RXO,     op)
REXX_ROUTINE_2(RXO, set_ui,    RXO, rop, RXULONG, op)
REXX_ROUTINE_2(RXO, set_si,    RXO, rop, RXLONG,  op)

/*
// void mpz_add_ui (mpz_t rop, const mpz_t op1, unsigned long int op2)
RexxRoutine3(RexxObjectPtr, z_rtn_add_ui, // routine mpz_add_ui
             RexxObjectPtr, rop,
             RexxObjectPtr, op1,
             MPZ_unsigned_long_int, op2)
{
  if (!context->IsInstanceOf(rop, TheZClass)) return wrongArgClassException(context, "rop", TheZClass);

  mpz_add_ui(OBJ_SELF_Z(rop), OBJ_SELF_Z(op1), op2); // rop := op1 + op2
  return NULLOBJECT;                   // no return value
}
*/

// 5.5 Arithmetic Functions
// void mpz_add (mpz_t rop, const mpz_t op1, const mpz_t op2)
// void mpz_add_ui (mpz_t rop, const mpz_t op1, unsigned long int op2)
// void mpz_sub (mpz_t rop, const mpz_t op1, const mpz_t op2)
// void mpz_sub_ui (mpz_t rop, const mpz_t op1, unsigned long int op2)
// void mpz_ui_sub (mpz_t rop, unsigned long int op1, const mpz_t op2)
// void mpz_mul (mpz_t rop, const mpz_t op1, const mpz_t op2)
// void mpz_mul_si (mpz_t rop, const mpz_t op1, long int op2)
// void mpz_mul_ui (mpz_t rop, const mpz_t op1, unsigned long int op2)
// void mpz_addmul (mpz_t rop, const mpz_t op1, const mpz_t op2)
// void mpz_addmul_ui (mpz_t rop, const mpz_t op1, unsigned long int op2)
// void mpz_submul (mpz_t rop, const mpz_t op1, const mpz_t op2)
// void mpz_submul_ui (mpz_t rop, const mpz_t op1, unsigned long int op2)
// void mpz_mul_2exp (mpz_t rop, const mpz_t op1, mp_bitcnt_t op2)
// void mpz_neg (mpz_t rop, const mpz_t op)
// void mpz_abs (mpz_t rop, const mpz_t op)
REXX_ROUTINE_3(RXO, add,       RXO, rop, RXO, op1, RXO,     op2)
REXX_ROUTINE_3(RXO, add_ui,    RXO, rop, RXO, op1, RXULONG, op2)
REXX_ROUTINE_3(RXO, sub,       RXO, rop, RXO, op1, RXO,     op2)
REXX_ROUTINE_3(RXO, sub_ui,    RXO, rop, RXO, op1, RXULONG, op2)
REXX_ROUTINE_3(RXO, ui_sub,    RXO, rop, RXULONG, op1, RXO, op2)
REXX_ROUTINE_3(RXO, mul,       RXO, rop, RXO, op1, RXO,     op2)
REXX_ROUTINE_3(RXO, mul_si,    RXO, rop, RXO, op1, RXLONG,  op2)
REXX_ROUTINE_3(RXO, mul_ui,    RXO, rop, RXO, op1, RXULONG, op2)
REXX_ROUTINE_3(RXO, addmul,    RXO, rop, RXO, op1, RXO,     op2)
REXX_ROUTINE_3(RXO, addmul_ui, RXO, rop, RXO, op1, RXULONG, op2)
REXX_ROUTINE_3(RXO, submul,    RXO, rop, RXO, op1, RXO,     op2)
REXX_ROUTINE_3(RXO, submul_ui, RXO, rop, RXO, op1, RXULONG, op2)
REXX_ROUTINE_3(RXO, mul_2exp,  RXO, rop, RXO, op1, RXULONG, op2)
REXX_ROUTINE_2(RXO, neg,       RXO, rop, RXO, op)
REXX_ROUTINE_2(RXO, abs,       RXO, rop, RXO, op)

// 5.6 Division Functions
// void mpz_cdiv_q (mpz_t q, const mpz_t n, const mpz_t d)
// void mpz_cdiv_r (mpz_t r, const mpz_t n, const mpz_t d)
// void mpz_cdiv_qr (mpz_t q, mpz_t r, const mpz_t n, const mpz_t d)
// unsigned long int mpz_cdiv_q_ui (mpz_t q, const mpz_t n, unsigned long int d)
// unsigned long int mpz_cdiv_r_ui (mpz_t r, const mpz_t n, unsigned long int d)
// unsigned long int mpz_cdiv_qr_ui (mpz_t q, mpz_t r, const mpz_t n, unsigned long int d)
// unsigned long int mpz_cdiv_ui (const mpz_t n, unsigned long int d)
// void mpz_cdiv_q_2exp (mpz_t q, const mpz_t n, mp_bitcnt_t b)
// void mpz_cdiv_r_2exp (mpz_t r, const mpz_t n, mp_bitcnt_t b)
REXX_ROUTINE_3(RXO, cdiv_q,     RXO, q,         RXO, n, RXO, d)
REXX_ROUTINE_3(RXO, cdiv_r,     RXO, q,         RXO, n, RXO, d)
REXX_ROUTINE_4(RXO, cdiv_qr,    RXO, q, RXO, r, RXO, n, RXO, d)

// void mpz_fdiv_q (mpz_t q, const mpz_t n, const mpz_t d)
// void mpz_fdiv_r (mpz_t r, const mpz_t n, const mpz_t d)
// void mpz_fdiv_qr (mpz_t q, mpz_t r, const mpz_t n, const mpz_t d)
// unsigned long int mpz_fdiv_q_ui (mpz_t q, const mpz_t n, unsigned long int d)
// unsigned long int mpz_fdiv_r_ui (mpz_t r, const mpz_t n, unsigned long int d)
// unsigned long int mpz_fdiv_qr_ui (mpz_t q, mpz_t r, const mpz_t n, unsigned long int d)
// unsigned long int mpz_fdiv_ui (const mpz_t n, unsigned long int d)
// void mpz_fdiv_q_2exp (mpz_t q, const mpz_t n, mp_bitcnt_t b)
// void mpz_fdiv_r_2exp (mpz_t r, const mpz_t n, mp_bitcnt_t b)

// void mpz_tdiv_q (mpz_t q, const mpz_t n, const mpz_t d)
// void mpz_tdiv_r (mpz_t r, const mpz_t n, const mpz_t d)
// void mpz_tdiv_qr (mpz_t q, mpz_t r, const mpz_t n, const mpz_t d)
// unsigned long int mpz_tdiv_q_ui (mpz_t q, const mpz_t n, unsigned long int d)
// unsigned long int mpz_tdiv_r_ui (mpz_t r, const mpz_t n, unsigned long int d)
// unsigned long int mpz_tdiv_qr_ui (mpz_t q, mpz_t r, const mpz_t n, unsigned long int d)
// unsigned long int mpz_tdiv_ui (const mpz_t n, unsigned long int d)
// void mpz_tdiv_q_2exp (mpz_t q, const mpz_t n, mp_bitcnt_t b)
// void mpz_tdiv_r_2exp (mpz_t r, const mpz_t n, mp_bitcnt_t b)
REXX_ROUTINE_3(RXO, tdiv_q,     RXO, q,         RXO, n, RXO, d)
REXX_ROUTINE_3(RXO, tdiv_r,     RXO, q,         RXO, n, RXO, d)
REXX_ROUTINE_4(RXO, tdiv_qr,    RXO, q, RXO, r, RXO, n, RXO, d)

// void mpz_mod (mpz_t r, const mpz_t n, const mpz_t d)
// unsigned long int mpz_mod_ui (mpz_t r, const mpz_t n, unsigned long int d)
REXX_ROUTINE_3(RXO,     mod,    RXO, r, RXO, n, RXO,     d)
REXX_ROUTINE_3(RXULONG, mod_ui, RXO, r, RXO, n, RXULONG, d)

// void mpz_divexact (mpz_t q, const mpz_t n, const mpz_t d)
// void mpz_divexact_ui (mpz_t q, const mpz_t n, unsigned long d)
REXX_ROUTINE_3(RXO, divexact,    RXO, q, RXO, n, RXO,     d)
REXX_ROUTINE_3(RXO, divexact_ui, RXO, q, RXO, n, RXULONG, d)

// int mpz_divisible_p (const mpz_t n, const mpz_t d)
// int mpz_divisible_ui_p (const mpz_t n, unsigned long int d)
// int mpz_divisible_2exp_p (const mpz_t n, mp_bitcnt_t b)
REXX_ROUTINE_2(RXINT, divisible_p,      RXO, n, RXO,     d)
REXX_ROUTINE_2(RXINT, divisible_ui_p,   RXO, n, RXULONG, d)
REXX_ROUTINE_2(RXINT, divisible_2exp_p, RXO, n, RXULONG, b) // RXBIT

// int mpz_congruent_p (const mpz_t n, const mpz_t c, const mpz_t d)
// int mpz_congruent_ui_p (const mpz_t n, unsigned long int c, unsigned long int d)
// int mpz_congruent_2exp_p (const mpz_t n, const mpz_t c, mp_bitcnt_t b)
REXX_ROUTINE_3(RXINT, congruent_p,      RXO, n, RXO,     c, RXO,     d)
REXX_ROUTINE_3(RXINT, congruent_ui_p,   RXO, n, RXULONG, c, RXULONG, d)
REXX_ROUTINE_3(RXINT, congruent_2exp_p, RXO, n, RXO,     c, RXULONG, b) // RXBIT


// 5.7 Exponentiation Functions
// void mpz_powm (mpz_t rop, const mpz_t base, const mpz_t exp, const mpz_t mod)
// void mpz_powm_ui (mpz_t rop, const mpz_t base, unsigned long int exp, const mpz_t mod)
// void mpz_pow_ui (mpz_t rop, const mpz_t base, unsigned long int exp)
// void mpz_ui_pow_ui (mpz_t rop, unsigned long int base, unsigned long int exp)
REXX_ROUTINE_4(RXO, powm,      RXO, rop, RXO, base,     RXO, exp,     RXO, mod)
REXX_ROUTINE_4(RXO, powm_ui,   RXO, rop, RXO, base,     RXULONG, exp, RXO, mod)
REXX_ROUTINE_3(RXO, pow_ui,    RXO, rop, RXO, base,     RXULONG, exp)
REXX_ROUTINE_3(RXO, ui_pow_ui, RXO, rop, RXULONG, base, RXULONG, exp)


// 5.8 Root Extraction Functions
// int mpz_root (mpz_t rop, const mpz_t op, unsigned long int n)
// void mpz_rootrem (mpz_t root, mpz_t rem, const mpz_t u, unsigned long int n)
// void mpz_sqrt (mpz_t rop, const mpz_t op)
// void mpz_sqrtrem (mpz_t rop1, mpz_t rop2, const mpz_t op)
// int mpz_perfect_power_p (const mpz_t op)
// int mpz_perfect_square_p (const mpz_t op)
REXX_ROUTINE_3(RXINT, root,             RXO, rop,   RXO, op,           RXULONG, n)
REXX_ROUTINE_4(RXO,   rootrem,          RXO, root,  RXO, rem,  RXO, u, RXULONG, n)
REXX_ROUTINE_2(RXO,   sqrt,             RXO, rop,   RXO, op)
REXX_ROUTINE_3(RXO,   sqrtrem,          RXO, rop1,  RXO, rop2, RXO, op)
REXX_ROUTINE_1(RXINT, perfect_power_p,  RXO, op)
REXX_ROUTINE_1(RXINT, perfect_square_p, RXO, op)


// 5.9 Number Theoretic Functions
// int mpz_probab_prime_p (const mpz_t n, int reps)
// void mpz_nextprime (mpz_t rop, const mpz_t op)
// void mpz_gcd (mpz_t rop, const mpz_t op1, const mpz_t op2)
// unsigned long int mpz_gcd_ui (mpz_t rop, const mpz_t op1, unsigned long int op2)
// void mpz_gcdext (mpz_t g, mpz_t s, mpz_t t, const mpz_t a, const mpz_t b)
// void mpz_lcm (mpz_t rop, const mpz_t op1, const mpz_t op2)
// void mpz_lcm_ui (mpz_t rop, const mpz_t op1, unsigned long op2)
// int mpz_invert (mpz_t rop, const mpz_t op1, const mpz_t op2)
// int mpz_jacobi (const mpz_t a, const mpz_t b)
// int mpz_legendre (const mpz_t a, const mpz_t p)
// int mpz_kronecker (const mpz_t a, const mpz_t b)
// int mpz_kronecker_si (const mpz_t a, long b)
// int mpz_kronecker_ui (const mpz_t a, unsigned long b)
// int mpz_si_kronecker (long a, const mpz_t b)
// int mpz_ui_kronecker (unsigned long a, const mpz_t b)
// mp_bitcnt_t mpz_remove (mpz_t rop, const mpz_t op, const mpz_t f)
// void mpz_fac_ui (mpz_t rop, unsigned long int n)
// void mpz_2fac_ui (mpz_t rop, unsigned long int n)
// void mpz_mfac_uiui (mpz_t rop, unsigned long int n, unsigned long int m)
// void mpz_primorial_ui (mpz_t rop, unsigned long int n)
// void mpz_bin_ui (mpz_t rop, const mpz_t n, unsigned long int k)
// void mpz_bin_uiui (mpz_t rop, unsigned long int n, unsigned long int k)
// void mpz_fib_ui (mpz_t fn, unsigned long int n)
// void mpz_fib2_ui (mpz_t fn, mpz_t fnsub1, unsigned long int n)
// void mpz_lucnum_ui (mpz_t ln, unsigned long int n)
// void mpz_lucnum2_ui (mpz_t ln, mpz_t lnsub1, unsigned long int n)
REXX_ROUTINE_2(RXINT,   probab_prime_p, RXO, n,   RXINT,   reps)
REXX_ROUTINE_2(RXO,     nextprime,      RXO, rop, RXO,     op)
REXX_ROUTINE_3(RXO,     gcd,            RXO, rop, RXO,     op1, RXO,     op2)
REXX_ROUTINE_3(RXULONG, gcd_ui,         RXO, rop, RXO,     op1, RXULONG, op2)
REXX_ROUTINE_5(RXO,     gcdext,         RXO, g, RXO, s, RXO, t, RXO, a, RXO, b)
REXX_ROUTINE_3(RXO,     lcm,            RXO, rop, RXO,     op1, RXO,     op2)
REXX_ROUTINE_3(RXO,     lcm_ui,         RXO, rop, RXO,     op1, RXULONG, op2)
REXX_ROUTINE_3(RXINT,   invert,         RXO, rop, RXO,     op1, RXO,     op2)
REXX_ROUTINE_2(RXINT,   jacobi,         RXO, a,   RXO,     b)
REXX_ROUTINE_2(RXINT,   legendre,       RXO, a,   RXO,     p)
REXX_ROUTINE_2(RXINT,   kronecker,      RXO, a,   RXO,     b)
REXX_ROUTINE_2(RXINT,   kronecker_si,   RXO, a,   RXLONG,  b)
REXX_ROUTINE_2(RXINT,   kronecker_ui,   RXO, a,   RXULONG, b)
REXX_ROUTINE_2(RXINT,   si_kronecker,   RXLONG, a,  RXO, b)
REXX_ROUTINE_2(RXINT,   ui_kronecker,   RXULONG, a, RXO, b)
REXX_ROUTINE_3(RXULONG, remove,         RXO, rop, RXO,     op,  RXO,     f)
REXX_ROUTINE_2(RXO,     fac_ui,         RXO, rop, RXULONG, n) // RXBIT
REXX_ROUTINE_2(RXO,     2fac_ui,        RXO, rop, RXULONG, n)
REXX_ROUTINE_3(RXO,     mfac_uiui,      RXO, rop, RXULONG, n, RXULONG, m)
REXX_ROUTINE_2(RXO,     primorial_ui,   RXO, rop, RXULONG, n)
REXX_ROUTINE_3(RXO,     bin_ui,         RXO, rop, RXO,     n, RXULONG, k)
REXX_ROUTINE_3(RXO,     bin_uiui,       RXO, rop, RXULONG, n, RXULONG, k)
REXX_ROUTINE_2(RXO,     fib_ui,         RXO, fn,  RXULONG, n)
REXX_ROUTINE_3(RXO,     fib2_ui,        RXO, fn,  RXO, fnsub1, RXULONG, n)
REXX_ROUTINE_2(RXO,     lucnum_ui,      RXO, ln,  RXULONG, n)
REXX_ROUTINE_3(RXO,     lucnum2_ui,     RXO, ln,  RXO, lnsub1, RXULONG, n)

// 5.10 Comparison Functions
// int mpz_cmp (const mpz_t op1, const mpz_t op2)
// int mpz_cmp_d (const mpz_t op1, double op2)
// int mpz_cmp_si (const mpz_t op1, signed long int op2)
// int mpz_cmp_ui (const mpz_t op1, unsigned long int op2)
// int mpz_cmpabs (const mpz_t op1, const mpz_t op2)
// int mpz_cmpabs_d (const mpz_t op1, double op2)
// int mpz_cmpabs_ui (const mpz_t op1, unsigned long int op2)
// int mpz_sgn (const mpz_t op)
REXX_ROUTINE_2(RXINT, cmp,       RXO, op1, RXO,     op2)
//REXX_ROUTINE_2(RXINT, cmp_d,     RXO, op1, RXLONG,  op2) // RXLONG instead of double
REXX_ROUTINE_2(RXINT, cmp_si,    RXO, op1, RXLONG,  op2)
REXX_ROUTINE_2(RXINT, cmp_ui,    RXO, op1, RXULONG, op2)
REXX_ROUTINE_2(RXINT, cmpabs,    RXO, op1, RXO,     op2)
//REXX_ROUTINE_2(RXINT, cmpabs_d,  RXO, op1, RXO,     op2) // RXLONG instead of double
REXX_ROUTINE_2(RXINT, cmpabs_ui, RXO, op1, RXULONG, op2)
REXX_ROUTINE_1(RXINT, sgn,       RXO, op)

// 5.11 Logical and Bit Manipulation Functions
// void mpz_and (mpz_t rop, const mpz_t op1, const mpz_t op2)
// void mpz_ior (mpz_t rop, const mpz_t op1, const mpz_t op2)
// void mpz_xor (mpz_t rop, const mpz_t op1, const mpz_t op2)
// void mpz_com (mpz_t rop, const mpz_t op)
// mp_bitcnt_t mpz_popcount (const mpz_t op)
// mp_bitcnt_t mpz_hamdist (const mpz_t op1, const mpz_t op2)
// mp_bitcnt_t mpz_scan0 (const mpz_t op, mp_bitcnt_t starting_bit)
// mp_bitcnt_t mpz_scan1 (const mpz_t op, mp_bitcnt_t starting_bit)
// void mpz_setbit (mpz_t rop, mp_bitcnt_t bit_index)
// void mpz_clrbit (mpz_t rop, mp_bitcnt_t bit_index)
// void mpz_combit (mpz_t rop, mp_bitcnt_t bit_index)
// int mpz_tstbit (const mpz_t op, mp_bitcnt_t bit_index)
REXX_ROUTINE_3(RXO,     and,      RXO, rop, RXO, op1, RXO, op2)
REXX_ROUTINE_3(RXO,     ior,      RXO, rop, RXO, op1, RXO, op2)
REXX_ROUTINE_3(RXO,     xor,      RXO, rop, RXO, op1, RXO, op2)
REXX_ROUTINE_2(RXO,     com,      RXO, rop, RXO, op)
REXX_ROUTINE_1(RXULONG, popcount, RXO, op) // RXBIT
REXX_ROUTINE_2(RXULONG, hamdist,  RXO, op1, RXO, op2)
REXX_ROUTINE_2(RXULONG, scan0,    RXO, op1, RXULONG, starting_bit) // RXBIT
REXX_ROUTINE_2(RXULONG, scan1,    RXO, op1, RXULONG, starting_bit) // RXBIT
REXX_ROUTINE_2(RXO,     setbit,   RXO, rop, RXULONG, bit_index) // RXBIT
REXX_ROUTINE_2(RXO,     clrbit,   RXO, rop, RXULONG, bit_index) // RXBIT
REXX_ROUTINE_2(RXO,     combit,   RXO, rop, RXULONG, bit_index) // RXBIT
REXX_ROUTINE_2(RXO,     tstbit,   RXO, rop, RXULONG, bit_index) // RXBIT




// ---------------------------------------------------------------------
// Methods


// the init() class method runs during *class* initialization
// we'll do any one-off initialization here
RexxMethod1(RexxObjectPtr, z_init_cls, // class method init()
            OSELF, self)
{
  // we'll save the class object that we receive through self,
  // so that later on instance methods can use the quick test
  //   if (context->IsInstanceOf(op, TheZClass))
  TheZClass = (RexxClassObject)self;
  context->RequestGlobalReference(TheZClass);

  // we'll initialize our global 'RandomState' which may later
  // be used by random() methods ..
  // void gmp_randinit_default (gmp_randstate_t state)
  gmp_randinit_default(RandomState);   // initialize with default algorithm

  // .. and seed it
  // void gmp_randseed_ui (gmp_randstate_t state, unsigned long int seed)
  gmp_randseed_ui(RandomState, time(NULL)); // seed with seconds

  return NULLOBJECT;                   // no return value
}


// the of() method allows us to define a shortcut
//   .z[123] instead of .z~new(123)
RexxMethod1(RexxObjectPtr, z_of_cls,   // class method of()
            RexxObjectPtr, op)
{
  // init() will handle setting the initial value to 'op'
  return context->SendMessage1(TheZClass, "NEW", op);
}



// ---------------------------------------------------------------------
// 5.1 Initialization Functions
// https://gmplib.org/manual/Initializing-Integers.html


// void mpz_init (mpz_t x)
// void mpz_init_set (mpz_t rop, const mpz_t op)
// int mpz_init_set_str (mpz_t rop, const char *str, int base)
RexxMethod2(RexxObjectPtr, z_init,     // method init()
            OPTIONAL_RexxObjectPtr, op,
            OPTIONAL_MPZ_int, base)
{
  // we're using the CSELF concept discussed in rexxpg 8.13.3.4. "The
  // CSELF method type".  Three steps are needed:
  // 1. create a buffer the size of our C++ object variables 'MPZ'
  // 2. set this buffer as object variable CSELF
  // 3. overlay the buffer with our 'MPZ' struct
  RexxBufferObject rop = context->NewBuffer(sizeof(MPZ));
  context->SetObjectVariable("CSELF", rop);
  MPZ *ropSelf = (MPZ *)context->BufferData(rop);

  ropSelf->id = ID_MPZ;                // mark as 'MPZ' data

  // we support three init() forms:
  //   init(), handled by mpz_init()
  //   init(z), handled by mpz_init_set()
  //   init(str [, base] ), handled by mpz_init_set_str()
  if (argumentOmitted(1))              // no argument given?  just mpz_init()
  {
    // init() form
    mpz_init(ropSelf->z);              // z := 0
  }
  else if (context->IsInstanceOf(op, TheZClass))
  {
    // init(z) form
    if (argumentExists(2))             // base given?  this is an error for this path
    {
      // Too many arguments in invocation of method; &1 expected.
      tooManyArgumentsException(context, 1);
    }
    mpz_init_set(ropSelf->z, OBJ_SELF_Z(op));
  }
  else
  {
    // init(str [, base] ) form
    if (argumentOmitted(2))            // no base given?  assume 10
    {
      // instead of 10, we might assume 0, thus adding support for 0x/0d/0
      // (see mpz_set_str(): if base is 0, then the leading characters are used:
      //  0x and 0X for hexadecimal, 0b and 0B for binary, 0 for octal, or decimal otherwise.)
      base = 10;
    }

    // instead of trying to catch any invalid-base exception thrown by GMP,
    // we check 'base' for the range 0, 2-62
    if (base < 0 || base == 1 || base > 62)
    {
      // Argument &1 must be &2in the range 2-62; found "&3"
      return notValidBaseException(context, "base", "0 or ", base);
    }

    // z := str (in base); returns 0 if OK, -1 if str not a number in base
    if (mpz_init_set_str(ropSelf->z, context->ObjectToStringValue(op), base) != 0)
    {
      // &1 must be a valid number in base &2, found "&3"
      notValidNumberInBaseException(context, "str", op, base);
    }
  }
  return NULLOBJECT;                   // no return value
}


// void mpz_clear (mpz_t x)
RexxMethod1(RexxObjectPtr, z_uninit,   // method uninit()
            CSELF, rop)
{
//printf("z_uninit: entry\n");
//printf("z_uninit: freeing %zu bytes\n", mpz_sizeinbase(SELF_Z(rop), 2)/8);
  mpz_clear(SELF_Z(rop));              // free MPZ memory
//printf("z_uninit: return\n");
  return NULLOBJECT;                   // no return value
}

// void mpz_swap (mpz_t rop1, mpz_t rop2)
RexxMethod2(RexxObjectPtr, z_swap,     // method swap()
            RexxObjectPtr, rop2,
            CSELF, rop1)
{
  // rop2 must be a Z instance
  if (!context->IsInstanceOf(rop2, TheZClass))
  {
    // Method argument &1 must be of the &2 class.
    return wrongArgClassException(context, "op2", TheZClass);
  }
  mpz_swap(SELF_Z(rop1), OBJ_SELF_Z(rop2));
  return NULLOBJECT;                   // no return value
}



// ---------------------------------------------------------------------
// 5.2 Assignment Functions
// https://gmplib.org/manual/Assigning-Integers.html


// void mpz_set (mpz_t rop, const mpz_t op)
// int mpz_set_str (mpz_t rop, const char *str, int base)
RexxMethod3(RexxObjectPtr, z_set,      // method set()
            RexxObjectPtr, op,
            OPTIONAL_MPZ_int, base,
            CSELF, rop)
{
  // we support two set() forms:
  //   set(z), handled by mpz_set()
  //   set(str, [base]), handled by mpz_set_str()
  if (context->IsInstanceOf(op, TheZClass))
  {
    // set(z) form
    if (argumentExists(2))             // base given?  this is an error for this path
    {
      // Too many arguments in invocation of method; &1 expected.
      tooManyArgumentsException(context, 1);
    }
    mpz_set(SELF_Z(rop), OBJ_SELF_Z(op));
  }
  else
  {
    // set(str, [base]) form
    if (argumentOmitted(2))            // no base given?  assume 10
      base = 10;                       // we might also assume 0

    // instead of trying to catch any invalid-base exception thrown by GMP,
    // we check 'base' for the range 0, 2-62
    if (base < 0 || base == 1 || base > 62)
    {
      // Argument &1 must be &2in the range 2-62; found "&3"
      return notValidBaseException(context, "base", "0 or ", base);
    }

    // z := str (in base); returns 0 if OK, -1 if str not a number in base
    if (mpz_set_str(SELF_Z(rop), context->ObjectToStringValue(op), base) != 0)
    {
      // &1 must be a valid number in base &2, found "&3"
      notValidNumberInBaseException(context, "str", op, base);
    }
  }
  return NULLOBJECT;                   // no return value
}



// ---------------------------------------------------------------------
// 5.4 Conversion Functions
// https://gmplib.org/manual/Converting-Integers.html


// char * mpz_get_str (char *str, int base, const mpz_t op)
RexxMethod2(RexxStringObject, z_get,   // method get(), string()
            OPTIONAL_MPZ_int, base,
            CSELF, rop)
{
  // we support one get() form:
  //   get(z, [base] ), handled by mpz_get_str()
  if (argumentOmitted(2))              // no base given?  assume 10
  {
    base = 10;                         // we might also assume 0 (see above)
  }

  // instead of trying to catch any invalid-base exception thrown by GMP,
  // we check 'base' for the range 0, 2-62
  if (base < 0 || base == 1 || base > 62)
  {
    // Argument &1 must be &2in the range 2-62; found "&3"
    notValidBaseException(context, "base", "0 or ", base);
    return context->NullString();
  }
  return context->String(mpz_get_str(NULL, base, SELF_Z(rop)));
}



// ---------------------------------------------------------------------
// 5.5 Arithmetic Functions
// https://gmplib.org/manual/Integer-Arithmetic.html


// development test: w/o isInstance()
RexxMethod2(RexxObjectPtr, z_add2,      // method add2()
            OPTIONAL_RexxObjectPtr, op2,
            OSELF, op1)
{
  // we support three add() forms:
  //   add(), might appear as "+z", handled as a nop
  //   add(z), handled by mpz_add()
  //   add(unsigned), handled by mpz_add_ui()
  if (argumentOmitted(1))              // no op2 given?  handle as a nop
  {
    // add() or "+z" form
    return op1;                        // just return receiver
  }

  // create new result Z instance
  RexxObjectPtr rop = context->SendMessage0(TheZClass, "NEW");

  MPZ *op1Self = (MPZ *)context->ObjectToCSelf(op1);
  MPZ *op2Self = (MPZ *)context->ObjectToCSelf(op2);
  if (op2Self != NULL && op2Self->id == ID_MPZ)
  {
    // add(z) form
    mpz_add(OBJ_SELF_Z(rop), op1Self->z, op2Self->z); // rop := op1 + op2
  }
  else
  {
    // add(unsigned) form
    // note that 'unsigned' means, z~add(-1) won't work
    MPZ_unsigned_long_int op2_int;
    if (!context->OBJ_unsigned_long_int(op2, &op2_int))
    {
      // Method argument &1 must be zero or a positive whole number; found "&2".
      return notZeroOrPositiveException(context, "op2", op2);
    }
    mpz_add_ui(OBJ_SELF_Z(rop), op1Self->z, op2_int); // rop := op1 + op2
  }
  return rop;                          // return result instance
}


// void mpz_add (mpz_t rop, const mpz_t op1, const mpz_t op2)
// void mpz_add_ui (mpz_t rop, const mpz_t op1, unsigned long int op2)
RexxMethod2(RexxObjectPtr, z_add,      // method add()
            OPTIONAL_RexxObjectPtr, op2,
            CSELF, op1)
{
  // we support three add() forms:
  //   add(), might appear as "+z", handled as a nop
  //   add(z), handled by mpz_add()
  //   add(unsigned), handled by mpz_add_ui()
  if (argumentOmitted(1))              // no op2 given?  handle as a nop
  {
    // add() or "+z" form
    return context->GetSelf();         // just return receiver
  }

  // create new result Z instance
  RexxObjectPtr rop = context->SendMessage0(TheZClass, "NEW");

  if (context->IsInstanceOf(op2, TheZClass))
  {
    // add(z) form
    mpz_add(OBJ_SELF_Z(rop), SELF_Z(op1), OBJ_SELF_Z(op2)); // rop := op1 + op2
  }
  else
  {
    // add(unsigned) form
    // note that 'unsigned' means, z~add(-1) won't work
    MPZ_unsigned_long_int op2_int;
    if (!context->OBJ_unsigned_long_int(op2, &op2_int))
    {
      // Method argument &1 must be zero or a positive whole number; found "&2".
      return notZeroOrPositiveException(context, "op2", op2);
    }
    mpz_add_ui(OBJ_SELF_Z(rop), SELF_Z(op1), op2_int); // rop := op1 + op2
  }
  return rop;                          // return result instance
}


// void mpz_sub (mpz_t rop, const mpz_t op1, const mpz_t op2)
// void mpz_sub_ui (mpz_t rop, const mpz_t op1, unsigned long int op2)
// void mpz_ui_sub (mpz_t rop, unsigned long int op1, const mpz_t op2)
// void mpz_neg (mpz_t rop, const mpz_t op)
RexxMethod2(RexxObjectPtr, z_sub,      // method sub()
            OPTIONAL_RexxObjectPtr, op2,
            CSELF, op1)
{
  // create new result Z instance
  RexxObjectPtr rop = context->SendMessage0(TheZClass, "NEW");

  // we support three sub() forms:
  //   sub(), might appear as "-z", handled by mpz_neg()
  //   sub(z), handled by mpz_sub()
  //   sub(unsigned), handled by mpz_sub_ui()
  // (we don't support mpz_ui_sub() in the method interface)
  if (argumentOmitted(1))              // no op2 given?  handle as a nop
  {
    // sub() or "-z" form
    mpz_neg(OBJ_SELF_Z(rop), SELF_Z(op1)); // rop := - op1
  }
  else if (context->IsInstanceOf(op2, TheZClass))
  {
    // sub(z) form
    mpz_sub(OBJ_SELF_Z(rop), SELF_Z(op1), OBJ_SELF_Z(op2)); // rop := op1 - op2
  }
  else
  {
    // sub(unsigned) form
    // note that 'unsigned' means, z~sub(-1) won't work
    MPZ_unsigned_long_int op2_int;
    if (!context->OBJ_unsigned_long_int(op2, &op2_int))
    {
      // Method argument &1 must be zero or a positive whole number; found "&2".
      return notZeroOrPositiveException(context, "op2", op2);
    }
    mpz_sub_ui(OBJ_SELF_Z(rop), SELF_Z(op1), op2_int); // rop := op1 - op2
  }
  return rop;                          // return result instance
}


// void mpz_mul (mpz_t rop, const mpz_t op1, const mpz_t op2)
// void mpz_mul_si (mpz_t rop, const mpz_t op1, long int op2)
// Set rop to op1 times op2.
RexxMethod2(RexxObjectPtr, z_mul,      // method mul()
            RexxObjectPtr, op2,
            CSELF, op1)
{
  // create new result Z instance
  RexxObjectPtr rop = context->SendMessage0(TheZClass, "NEW");

  // we support two mul() forms:
  //   mul(z), handled by mpz_mul()
  //   mul(signed), handled by mpz_mul_si()
  // (we don't support mpz_mul_ui in addition)
  if (context->IsInstanceOf(op2, TheZClass))
  {
    // mul(z) form
    mpz_mul(OBJ_SELF_Z(rop), SELF_Z(op1), OBJ_SELF_Z(op2)); // rop := op1 * op2
  }
  else
  {
    // mul(signed) form
    MPZ_long_int op2_int;
    if (!context->OBJ_long_int(op2, &op2_int))
    {
      // Method argument &1 must be a whole number; found "&2".
      return invalidWholeNumberException(context, "op2", op2);
    }
    mpz_mul_si(OBJ_SELF_Z(rop), SELF_Z(op1), op2_int); // rop := op1 * op2
  }
  return rop;                          // return result instance
}


// void mpz_addmul (mpz_t rop, const mpz_t op1, const mpz_t op2)
// void mpz_addmul_ui (mpz_t rop, const mpz_t op1, unsigned long int op2)
// Set rop to rop + op1 times op2.
RexxMethod3(RexxObjectPtr, z_addMul,   // method addMul()
            RexxObjectPtr, op1,
            RexxObjectPtr, op2,
            CSELF, rop)
{
  // this method is a special case: 'rop' isn't a newly created return instance,
  // it's the receiver itself, which is modified

  // we support two addMul() forms:
  //   addMul(z), handled by mpz_addmul()
  //   addMul(unsigned), handled by mpz_addmul_ui
  if (context->IsInstanceOf(op2, TheZClass))
  {
    // addMul(z) form
    mpz_addmul(SELF_Z(rop), OBJ_SELF_Z(op1), OBJ_SELF_Z(op2)); // rop := rop + op1 * op2
  }
  else
  {
    // addMul(unsigned) form
    MPZ_unsigned_long_int op2_int;
    if (!context->OBJ_unsigned_long_int(op2, &op2_int))
    {
      // Method argument &1 must be zero or a positive whole number; found "&2".
      return notZeroOrPositiveException(context, "op2", op2);
    }
    mpz_addmul_ui(SELF_Z(rop), OBJ_SELF_Z(op1), op2_int); // rop := rop + op1 * op2
  }
  return context->GetSelf();           // for convenience, we return modified 'rop'

}


// void mpz_submul (mpz_t rop, const mpz_t op1, const mpz_t op2)
// void mpz_submul_ui (mpz_t rop, const mpz_t op1, unsigned long int op2)
// Set rop to rop - op1 times op2.
RexxMethod3(RexxObjectPtr, z_subMul,   // method subMul()
            RexxObjectPtr, op1,
            RexxObjectPtr, op2,
            CSELF, rop)
{
  // this method is a special case: 'rop' isn't a newly created return instance,
  // it's the receiver itself, which is modified

  // we support two subMul() forms:
  //   subMul(z), handled by mpz_submul()
  //   subMul(unsigned), handled by mpz_submul_ui
  if (context->IsInstanceOf(op2, TheZClass))
  {
    // subMul(z) form
    mpz_submul(SELF_Z(rop), OBJ_SELF_Z(op1), OBJ_SELF_Z(op2)); // rop := rop - op1 * op2
  }
  else
  {
    // subMul(unsigned) form
    MPZ_unsigned_long_int op2_int;
    if (!context->OBJ_unsigned_long_int(op2, &op2_int))
    {
      // Method argument &1 must be zero or a positive whole number; found "&2".
      return notZeroOrPositiveException(context, "op2", op2);
    }
    mpz_submul_ui(SELF_Z(rop), OBJ_SELF_Z(op1), op2_int); // rop := rop - op1 * op2
  }
  return context->GetSelf();           // for convenience, we return modified 'rop'
}


// void mpz_mul_2exp (mpz_t rop, const mpz_t op1, mp_bitcnt_t op2)
// Set rop to op1 times 2 raised to op2.
// This operation can also be defined as a left shift by op2 bits.
RexxMethod2(RexxObjectPtr, z_mul2exp,  // method mul2exp()
            size_t, op2,
            CSELF, op1)
{
  // create new result Z instance
  RexxObjectPtr rop = context->SendMessage0(TheZClass, "NEW");

  // we support one mul2exp() form:
  //   mul2exp(size), handled by mpz_mul_2exp()
  mpz_mul_2exp(OBJ_SELF_Z(rop), SELF_Z(op1), op2); // rop := op1 * 2 ^ op2
  return rop;                          // return result instance
}


//void mpz_abs (mpz_t rop, const mpz_t op)
RexxMethod1(RexxObjectPtr, z_abs,      // method abs()
            CSELF, op)
{
  // create new result Z instance
  RexxObjectPtr rop = context->SendMessage0(TheZClass, "NEW");

  mpz_abs(OBJ_SELF_Z(rop), SELF_Z(op)); // rop := |op|
  return rop;                          // return result instance
}



// ---------------------------------------------------------------------
// 5.6 Division Functions
// https://gmplib.org/manual/Integer-Division.html


// void mpz_cdiv_q (mpz_t q, const mpz_t n, const mpz_t d)
// unsigned long int mpz_cdiv_q_ui (mpz_t q, const mpz_t n, unsigned long int d)
// void mpz_cdiv_q_2exp (mpz_t q, const mpz_t n, mp_bitcnt_t b)
//
// void mpz_fdiv_q (mpz_t q, const mpz_t n, const mpz_t d)
// unsigned long int mpz_fdiv_q_ui (mpz_t q, const mpz_t n, unsigned long int d)
// void mpz_fdiv_q_2exp (mpz_t q, const mpz_t n, mp_bitcnt_t b)
//
// void mpz_tdiv_q (mpz_t q, const mpz_t n, const mpz_t d)
// unsigned long int mpz_tdiv_q_ui (mpz_t q, const mpz_t n, unsigned long int d)
RexxMethod3(RexxObjectPtr, z_div,      // method div()
            RexxObjectPtr, d,
            OPTIONAL_CSTRING, round,
            CSELF, n)
{
  // create new result Z instance
  RexxObjectPtr q = context->SendMessage0(TheZClass, "NEW");
  if (round == NULL)                   // no rounding method specified?
  {
    round = "T";                       // use "truncate" as default
  }

  // we support two div() forms:
  //   div(z, [C/F/T] ), handled by mpz_c/f/t/div_q()
  //   div(signed, [C/F/T] ), handled by mpz_c/f/t/div_q_ui()
  if (context->IsInstanceOf(d, TheZClass))
  {
    // instead of trying to catch any divide-by-zero exception thrown by GMP,
    // we check 'd' for zero
    MPZ *dSelf = (MPZ *)context->ObjectToCSelf(d);
    if (mpz_sgn(dSelf->z) == 0)        // d = 0 ?
    {
      // Arithmetic overflow; divisor must not be zero.
      return divideByZeroException(context);
    }

    // div(z, [C/F/T] ) form
    switch (round[0])                  // check only first character of 'round'
    {
      case 'c', 'C': { mpz_cdiv_q(OBJ_SELF_Z(q), SELF_Z(n), dSelf->z); break; } // ceil
      case 'f', 'F': { mpz_fdiv_q(OBJ_SELF_Z(q), SELF_Z(n), dSelf->z); break; } // floor
      case 't', 'T': { mpz_tdiv_q(OBJ_SELF_Z(q), SELF_Z(n), dSelf->z); break; } // truncate
      // Method argument &1 must be one of &2; found "&3".
      default: return wrongArgValueException(context, "round", "CFT", round);
    }
  }
  else
  {
    // div(signed, [C/F/T] ) form
    MPZ_long_int d_int;
    if (!context->OBJ_long_int(d, &d_int))
    {
      // Method argument &1 must be a whole number; found "&2".
      return invalidWholeNumberException(context, "d", d);
    }

    // instead of trying to catch any divide-by-zero exception thrown by GMP,
    // we check 'd_int' for zero
    if (d_int == 0)
    {
      // Arithmetic overflow; divisor must not be zero.
      return divideByZeroException(context);
    }
    switch (round[0])                  // check only first character of 'round'
    {
      case 'c', 'C': { mpz_cdiv_q_ui(OBJ_SELF_Z(q), SELF_Z(n), d_int); break; } // ceil
      case 'f', 'F': { mpz_fdiv_q_ui(OBJ_SELF_Z(q), SELF_Z(n), d_int); break; } // floor
      case 't', 'T': { mpz_tdiv_q_ui(OBJ_SELF_Z(q), SELF_Z(n), d_int); break; } // truncate
      // Method argument &1 must be one of &2; found "&3".
      default: return wrongArgValueException(context, "round", "CFT", round);
    }
  }
  return q;                            // return result instance
}


// void mpz_divexact (mpz_t q, const mpz_t n, const mpz_t d)
// void mpz_divexact_ui (mpz_t q, const mpz_t n, unsigned long d)
RexxMethod2(RexxObjectPtr, z_divExact, // method divExact()
            RexxObjectPtr, d,
            CSELF, n)
{
  // create new result Z instance
  RexxObjectPtr q = context->SendMessage0(TheZClass, "NEW");

  // we support two divExact() forms:
  //   divExact(z), handled by mpz_divexact()
  //   divExact(signed), handled by mpz_divexact_ui()
  if (context->IsInstanceOf(d, TheZClass))
  {
    // divExact(z) form
    mpz_divexact(OBJ_SELF_Z(q), SELF_Z(n), OBJ_SELF_Z(d));
  }
  else
  {
    // divExact(signed) form
    MPZ_long_int d_int;
    if (!context->OBJ_long_int(d, &d_int))
    {
      // Method argument &1 must be a whole number; found "&2".
      return invalidWholeNumberException(context, "d", d);
    }
    mpz_divexact_ui(OBJ_SELF_Z(q), SELF_Z(n), d_int);
  }
  return q;                            // return result instance
}


// int mpz_divisible_p (const mpz_t n, const mpz_t d)
// int mpz_divisible_ui_p (const mpz_t n, unsigned long int d)
// int mpz_divisible_2exp_p (const mpz_t n, mp_bitcnt_t b)
RexxMethod2(int, z_divisible,          // method divisible()
            RexxObjectPtr, d,
            CSELF, n)
/* TO DO: add support for mpz_divisible_2exp_p() */
{
  int result;

  // we support two divisible() forms:
  //   divisible(z), handled by mpz_divisible_p()
  //   divisible(unsigned), handled by mpz_divisible_ui_p()
  if (context->IsInstanceOf(d, TheZClass))
  {
    // divisible(z) form
    result = mpz_divisible_p(SELF_Z(n), OBJ_SELF_Z(d));
  }
  else
  {
    // divisible(unsigned) form
    MPZ_unsigned_long_int d_int;
    if (!context->OBJ_unsigned_long_int(d, &d_int))
    {
      // Method argument &1 must be zero or a positive whole number; found "&2".
      notZeroOrPositiveException(context, "d", d);
      return 0;                        // cannot return NULLOBJECT
    }
    result = mpz_divisible_ui_p(SELF_Z(n), d_int);
  }
  // make sure that we only return exactly 0 or 1 (is this really needed?)
  return result == 0 ? 0 : 1;
}


// int mpz_congruent_p (const mpz_t n, const mpz_t c, const mpz_t d)
// int mpz_congruent_ui_p (const mpz_t n, unsigned long int c, unsigned long int d)
// int mpz_congruent_2exp_p (const mpz_t n, const mpz_t c, mp_bitcnt_t b)
RexxMethod3(int, z_congruent,          // method congruent()
            RexxObjectPtr, c,
            RexxObjectPtr, d,
            CSELF, n)
/* TO DO: add support for mpz_congruent_2exp_p() */
{
  int result;

  // we support four congruent() forms:
  //   congruent(z, z), handled by mpz_congruent_p()
  //   congruent(z, unsigned), handled by mpz_congruent_ui_p()
  //   congruent(unsigned, z), handled by mpz_congruent_ui_p()
  //   congruent(unsigned, unsigned), handled by mpz_congruent_ui_p()
  if (context->IsInstanceOf(c, TheZClass) && context->IsInstanceOf(d, TheZClass))
  {
    // congruent(z, z) form
    result = mpz_congruent_p(SELF_Z(n), OBJ_SELF_Z(c), OBJ_SELF_Z(d));
  }
  else
  {
    MPZ_unsigned_long_int c_int, d_int;

    // congruent(unsigned/z, unsigned/z) form
    if (!context->OBJ_unsigned_long_int(c, &c_int))
    {
      // Method argument &1 must be zero or a positive whole number; found "&2".
      notZeroOrPositiveException(context, "c", c);
      return 0;                        // cannot return NULLOBJECT
    }
    if (!context->OBJ_unsigned_long_int(d, &d_int))
    {
      // Method argument &1 must be zero or a positive whole number; found "&2".
      notZeroOrPositiveException(context, "d", d);
      return 0;                        // cannot return NULLOBJECT
    }
    result = mpz_congruent_ui_p(SELF_Z(n), c_int, d_int);
  }
  // make sure that we only return exactly 0 or 1 (is this really needed?)
  return result == 0 ? 0 : 1;
}


// void mpz_mod (mpz_t r, const mpz_t n, const mpz_t d)
// unsigned long int mpz_mod_ui (mpz_t r, const mpz_t n, unsigned long int d)
RexxMethod2(RexxObjectPtr, z_mod,      // method mod()
            RexxObjectPtr, d,
            CSELF, n)
{
  // create new result Z instance
  RexxObjectPtr r = context->SendMessage0(TheZClass, "NEW");

  // we support two mod() forms:
  //   mod(z), handled by mpz_mod()
  //   mod(unsigned), handled by mpz_mod_ui()
  if (context->IsInstanceOf(d, TheZClass))
  {
    // instead of trying to catch any divide-by-zero exception thrown by GMP,
    // we check 'd' for zero
    if (mpz_cmp_ui(OBJ_SELF_Z(d), 0) == 0)
    {
      // Method argument %s must not be zero.
      return mustNotBeZeroException(context, "d");
    }

    // mod(z) form
    mpz_mod(OBJ_SELF_Z(r), SELF_Z(n), OBJ_SELF_Z(d)); // returned remainder will be lost
  }
  else
  {
    // mod(unsigned) form
    MPZ_long_int d_int;
    if (!context->OBJ_long_int(d, &d_int))
    {
      // Method argument &1 must be a whole number; found "&2".
      return invalidWholeNumberException(context, "d", d);
    }

    // instead of trying to catch any divide-by-zero exception thrown by GMP,
    // we check 'd' for zero
    if (d_int == 0)
    {
      // Method argument %s must not be zero.
      return mustNotBeZeroException(context, "d");
    }

    mpz_mod_ui(OBJ_SELF_Z(r), SELF_Z(n), d_int);
  }
  return r;                            // return result instance
}



// ---------------------------------------------------------------------
// 5.7 Exponentiation Functions
// https://gmplib.org/manual/Integer-Exponentiation.html


// void mpz_powm (mpz_t rop, const mpz_t base, const mpz_t exp, const mpz_t mod)
// void mpz_powm_ui (mpz_t rop, const mpz_t base, unsigned long int exp, const mpz_t mod)
// Negative exp is supported if an inverse base^-1 mod mod exists
// If an inverse doesn’t exist then a divide by zero is raised.
RexxMethod3(RexxObjectPtr, z_powm,     // method powm()
            RexxObjectPtr, exp,
            RexxObjectPtr, mod,
            CSELF, base)
{
  // TO DO: OBJ_SELF_Z(mod/exp) are being called multiple times; should only do once

  // mod must be a Z instance
  if (!context->IsInstanceOf(mod, TheZClass))
  {
    // Method argument &1 must be of the &2 class.
    return wrongArgClassException(context, "mod", TheZClass);
  }

  // instead of trying to catch any divide-by-zero exception thrown by GMP,
  // we check 'mod' for zero
  if (mpz_cmp_ui(OBJ_SELF_Z(mod), 0) == 0)
  {
    // Method argument %s must not be zero.
    return mustNotBeZeroException(context, "mod");
  }

  // create new result Z instance
  RexxObjectPtr rop = context->SendMessage0(TheZClass, "NEW");

  // we support two powm() forms:
  //   powm(z, z), handled by mpz_powm_ui()
  //   powm(unsigned, z), handled by mpz_powm()
  // NOTE: although exp < 0 is allowed, this only works for the powm(z, z) form
  if (context->IsInstanceOf(exp, TheZClass))
  {
    // powm(z, z) form

    if (mpz_sgn(OBJ_SELF_Z(exp)) == -1)    // is 'exp' negative?
    {
      // "Negative exp is supported if an inverse base^-1 mod mod exists
      //  If an inverse doesn’t exist then a divide by zero is raised."
      // instead of trying to catch any divide-by-zero exception thrown by GMP,
      // we use mpz_invert(rop, base, mod) to test
      // If the inverse exists, the return value is non-zero
      if (mpz_invert(OBJ_SELF_Z(rop), SELF_Z(base), OBJ_SELF_Z(mod)) == 0)
      {
        // Negative exponent only allowed if inverse exists; found "&1"
        return negativeExponentException(context, OBJ_SELF_Z(exp));
      }
    }
    mpz_powm(OBJ_SELF_Z(rop), SELF_Z(base), OBJ_SELF_Z(exp), OBJ_SELF_Z(mod)); // rop := base ^ exp modulo mod
  }
  else
  {
    // powm(unsigned, z) form
    // NOTE: although exp < 0 is allowed, this only works for the powm(z, z) form
    MPZ_unsigned_long_int exp_int;
    if (!context->OBJ_unsigned_long_int(exp, &exp_int))
    {
      // Method argument &1 must be zero or a positive whole number; found "&2".
      return notZeroOrPositiveException(context, "exp", exp);
    }
    mpz_powm_ui(OBJ_SELF_Z(rop), SELF_Z(base), exp_int, OBJ_SELF_Z(mod)); // rop := base ^ exp modulo mod
  }

  return rop;                          // return result instance
}


// void mpz_pow_ui (mpz_t rop, const mpz_t base, unsigned long int exp)
RexxMethod2(RexxObjectPtr, z_pow,      // method pow()
            MPZ_unsigned_long_int, exp,
            CSELF, base)
{
  // create new result Z instance
  RexxObjectPtr rop = context->SendMessage0(TheZClass, "NEW");

  // we support one pow() form:
  //   pow(z), handled by mpz_pow_ui()
  // (we don't support mpz_ui_pow_ui() in the method interface)
  mpz_pow_ui(OBJ_SELF_Z(rop), SELF_Z(base), exp); // rop := base ^ exp
  return rop;                          // return result instance
}



// ---------------------------------------------------------------------
// 5.8 Root Extraction Functions
// https://gmplib.org/manual/Integer-Roots.html


// void mpz_sqrt (mpz_t rop, const mpz_t op)
RexxMethod1(RexxObjectPtr, z_sqrt,     // method sqrt()
            CSELF, op)
{
  // instead of trying to catch any op-less-than-zero exception thrown by GMP,
  // we check 'op' for negative
  if (mpz_sgn(SELF_Z(op)) == -1)       // is 'op' negative?
  {
    // Method argument &1 must be zero or a positive whole number; found "&2".
    return notZeroOrPositiveException(context, "op", SELF_Z(op));
  }

  // create new result Z instance
  RexxObjectPtr rop = context->SendMessage0(TheZClass, "NEW");
  mpz_sqrt(OBJ_SELF_Z(rop), SELF_Z(op));
  return rop;                          // return result instance
}


// int mpz_root (mpz_t rop, const mpz_t op, unsigned long int n)
RexxMethod2(RexxObjectPtr, z_root,     // method root()
            MPZ_unsigned_long_int, n,
            CSELF, op)
{
  // instead of trying to catch any no-root-exists exception thrown by GMP,
  // we check 'op' and 'n' for appropriate values
  // we require n >= 1; the method entry asserts n > 0, but we need to exclude n = 0
  if (n < 1)
  {
    // Method argument &1 must be a positive whole number; found "&2".
    return notPositiveException(context, "n", n);
  }
  // if 'op' < 0 then 'n' must be odd
  if (mpz_sgn(SELF_Z(op)) == -1 && !(n & 1))       // op < 0 and n not odd?
  {
      // For argument &1 a root only exists for &2; found "&3"
      return noRootExistsException(context, "op < 0", "odd n", n);
  }

  // create new result Z instance
  RexxObjectPtr rop = context->SendMessage0(TheZClass, "NEW");

  mpz_root(OBJ_SELF_Z(rop), SELF_Z(op), n); // mpz_root() result will be lost
  return rop;                          // return result instance
}


// int mpz_perfect_power_p (const mpz_t op)
RexxMethod1(int, z_perfectPower,       // method perfectPower()
            CSELF, op)
{
  return mpz_perfect_power_p(SELF_Z(op));
}


// int mpz_perfect_square_p (const mpz_t op)
RexxMethod1(int, z_perfectSquare,      // method perfectSquare()
            CSELF, op)
{
  return mpz_perfect_square_p(SELF_Z(op));
}


/*
void mpz_rootrem (mpz_t root, mpz_t rem, const mpz_t u, unsigned long int n)
Set root to the truncated integer part of the nth root of u. Set rem to the remainder, u-root**n.

void mpz_sqrtrem (mpz_t rop1, mpz_t rop2, const mpz_t op)
Set rop1 to the truncated integer part of the square root of op, like mpz_sqrt. Set rop2 to the remainder op-rop1*rop1, which will be zero if op is a perfect square.
If rop1 and rop2 are the same variable, the results are undefined.
*/



// ---------------------------------------------------------------------
// 5.9 Number Theoretic Functions
// https://gmplib.org/manual/Number-Theoretic-Functions.html


// int mpz_probab_prime_p (const mpz_t n, int reps)
RexxMethod2(int, z_probabPrime,        // method probabPrime()
            OPTIONAL_MPZ_int, reps,
            CSELF, rop)
{
  if (argumentOmitted(1))              // no reps given?  assume 25
    reps = 25;
  // returns 2 if n is definitely prime,
  // returns 1 if n is probably prime (without being certain),
  // or returns 0 if n is definitely non-prime
  return mpz_probab_prime_p(SELF_Z(rop), reps);
}


// void mpz_nextprime (mpz_t rop, const mpz_t op)
RexxMethod1(RexxObjectPtr, z_nextPrime, // method nextPrime(z)
            CSELF, op)
{
  // create new result Z instance
  RexxObjectPtr rop = context->SendMessage0(TheZClass, "NEW");

  mpz_nextprime(OBJ_SELF_Z(rop), SELF_Z(op));
  return rop;                          // return result instance
}


// void mpz_gcd (mpz_t rop, const mpz_t op1, const mpz_t op2)
// Set rop to the greatest common divisor of op1 and op2.
// (we don't support mpz_gcd_ui() )
RexxMethod2(RexxObjectPtr, z_gcd,      // method gcd()
            RexxObjectPtr, op2,
            CSELF, op1)
{
  // 'op2' must be a Z instance
  if (!context->IsInstanceOf(op2, TheZClass))
  {
    // Method argument &1 must be of the &2 class.
    return wrongArgClassException(context, "op2", TheZClass);
  }

  // create new result Z instance
  RexxObjectPtr rop = context->SendMessage0(TheZClass, "NEW");

  mpz_gcd(OBJ_SELF_Z(rop), SELF_Z(op1), OBJ_SELF_Z(op2)); // rop := gcd(op1, op2)
  return rop;                          // return result instance
}


// void mpz_lcm (mpz_t rop, const mpz_t op1, const mpz_t op2)
// void mpz_lcm_ui (mpz_t rop, const mpz_t op1, unsigned long op2)
// Set rop to the least common multiple of op1 and op2. rop is always positive,
// irrespective of the signs of op1 and op2.
// rop will be zero if either op1 or op2 is zero.
RexxMethod2(RexxObjectPtr, z_lcm,      // method lcm()
            RexxObjectPtr, op2,
            CSELF, op1)
{
  // create new result Z instance
  RexxObjectPtr rop = context->SendMessage0(TheZClass, "NEW");

  // we support two lcm() forms:
  //   lcm(z), handled by mpz_lcm
  //   lcm(unsigned), handled by mpz_lcm_ui
  if (context->IsInstanceOf(op2, TheZClass))
  {
    // lcm(z)
    mpz_lcm(OBJ_SELF_Z(rop), SELF_Z(op1), OBJ_SELF_Z(op2)); // rop := lcm(op1, op2)
  }
  else
  {
    // lcm(unsigned) form
    MPZ_unsigned_long_int op2_int;
    if (!context->OBJ_unsigned_long_int(op2, &op2_int))
    {
      // Method argument &1 must be zero or a positive whole number; found "&2".
      return notZeroOrPositiveException(context, "op2", op2);
    }
    mpz_lcm_ui(OBJ_SELF_Z(rop), SELF_Z(op1), op2_int); // rop := lcm(op1, op2)
  }
  return rop;                          // return result instance
}


// mp_bitcnt_t mpz_remove (mpz_t rop, const mpz_t op, const mpz_t f)
// Remove all occurrences of the factor f from op and store the result in rop.
// The return value is how many such occurrences were removed.
RexxMethod2(RexxObjectPtr, z_remove,   // method remove()
            RexxObjectPtr, f,
            CSELF, op)
{
  // 'f' must be a Z instance
  if (!context->IsInstanceOf(f, TheZClass))
  {
    // Method argument &1 must be of the &2 class.
    return wrongArgClassException(context, "f", TheZClass);
  }

  // create new result Z instance
  RexxObjectPtr rop = context->SendMessage0(TheZClass, "NEW");

  // mpz_remove() result value (how many factors) will be lost
  mpz_remove(OBJ_SELF_Z(rop), SELF_Z(op), OBJ_SELF_Z(f)); // rop := op % f^a
  return rop;                          // return result instance
}


// void mpz_fac_ui (mpz_t rop, unsigned long int n)
// Set rop to the factorial of n
RexxMethod1(RexxObjectPtr, z_fac_cls,  // class method fac()
            MPZ_unsigned_long_int, n)
{
  // as mpz_fac_ui() has no Z class receiver object, we have to
  // implement this as a class method, .z~fac(unsigned)
  RexxObjectPtr rop = context->SendMessage0(TheZClass, "NEW");
  mpz_fac_ui(OBJ_SELF_Z(rop), n);      // rop := n!
  return rop;                          // return result instance
}


// void mpz_2fac_ui (mpz_t rop, unsigned long int n)
// Set rop to the double-factorial n!!
RexxMethod1(RexxObjectPtr, z_2fac_cls, // class method 2fac()
            MPZ_unsigned_long_int, n)
{
  // as mpz_2fac_ui() has no Z class receiver object, we have to
  // implement this as a class method, .z~2fac(unsigned)
  RexxObjectPtr rop = context->SendMessage0(TheZClass, "NEW");
  mpz_2fac_ui(OBJ_SELF_Z(rop), n);     // rop := n!!
  return rop;                          // return result instance
}


// void mpz_mfac_uiui (mpz_t rop, unsigned long int n, unsigned long int m)
// Set rop to the m-multi-factorial n!^(m).
RexxMethod2(RexxObjectPtr, z_mfac_cls, // class method mfac()
            MPZ_unsigned_long_int, n,
            MPZ_unsigned_long_int, m)
{
  // as mpz_mfac_uiui() has no Z class receiver object, we have to
  // implement this as a class method, .z~mfac(unsigned)
  RexxObjectPtr rop = context->SendMessage0(TheZClass, "NEW");
  mpz_mfac_uiui(OBJ_SELF_Z(rop), n, m); // rop := n!^m
  return rop;                          // return result instance
}


// void mpz_primorial_ui (mpz_t rop, unsigned long int n)
// Set rop to the primorial of n, i.e. the product of all positive prime numbers <=n.
RexxMethod1(RexxObjectPtr, z_primorial_cls, // class method primorial()
            MPZ_unsigned_long_int, n)
{
  // as mpz_primorial_ui() has no Z class receiver object, we have to
  // implement this as a class method, .z~primorial(unsigned)
  RexxObjectPtr rop = context->SendMessage0(TheZClass, "NEW");
  mpz_primorial_ui(OBJ_SELF_Z(rop), n); // rop := product primes <= n
  return rop;                          // return result instance
}



// int mpz_invert (mpz_t rop, const mpz_t op1, const mpz_t op2)
// Compute the inverse of op1 modulo op2 and put the result in rop.
// If the inverse exists, the return value is non-zero and rop will satisfy 0 <= rop < abs(op2)
// (with rop = 0 possible only when abs(op2) = 1, i.e., in the somewhat degenerate zero ring).
// If an inverse doesn’t exist the return value is zero and rop is undefined.
// The behaviour of this function is undefined when op2 is zero
RexxMethod2(RexxObjectPtr, z_invert,   // method invert()
            RexxObjectPtr, op2,
            CSELF, op1)
{
  int result;

  // 'op2' must be a Z instance
  if (!context->IsInstanceOf(op2, TheZClass))
  {
    // Method argument &1 must be of the &2 class.
    return wrongArgClassException(context, "op2", TheZClass);
  }

  // "function is undefined when op2 is zero"
  // we simply ignore above, as mpz_invert() returns 0

  // create new result Z instance
  RexxObjectPtr rop = context->SendMessage0(TheZClass, "NEW");

  // rop := invert(op1, op2) such that (op1 * rop) mod op2 == 1
  result = mpz_invert(OBJ_SELF_Z(rop), SELF_Z(op1), OBJ_SELF_Z(op2));
  if (result == 0)                     // no inverse exists?
  {
    return context->NullString();      // return empty string
  }
  else
  {
    return rop;                        // if an inverse exists, we'll return 'rop'
  }
}


// void mpz_bin_ui (mpz_t rop, const mpz_t n, unsigned long int k)
// void mpz_bin_uiui (mpz_t rop, unsigned long int n, unsigned long int k)
// Compute the binomial coefficient n over k and store the result in rop.
// Negative values of n are supported by mpz_bin_ui
RexxMethod2(RexxObjectPtr, z_bin,      // method bin()
            MPZ_unsigned_long_int, k,
            CSELF, n)
{
  // create new result Z instance
  RexxObjectPtr rop = context->SendMessage0(TheZClass, "NEW");

  // we support one bin() forms:
  //   bin(unsigned), handled by mpz_bin_ui
  //   (we don't support mpz_bin_uiui in the method interface)
  mpz_bin_ui(OBJ_SELF_Z(rop), SELF_Z(n), k); // rop := bin(n, k)
  return rop;                          // return result instance
}



// ---------------------------------------------------------------------
// 5.10 Comparison Functions
// https://gmplib.org/manual/Integer-Comparisons.html


// int mpz_cmp (const mpz_t op1, const mpz_t op2)
// int mpz_cmp_si (const mpz_t op1, signed long int op2)
RexxMethod2(int, z_compareTo,          // method compareTo()
            RexxObjectPtr ,op2,
            CSELF, op1)
{
  int  result;

  // we support two compareTo() forms:
  //   compareTo(z), handled by mpz_cmp()
  //   compareTo(signed), handled by mpz_cmp_si()
  // (we don't support mpz_cmp_ui in addition)
  if (context->IsInstanceOf(op2, TheZClass))
  {
    // compare(z) form
    result = mpz_cmp(SELF_Z(op1), OBJ_SELF_Z(op2));
    // make sure that we only return exactly -1, 0, or 1 (is this really needed?)
    return result < 0 ? -1 : result > 0;
  }
  else
  {
    // compare(signed) form
    // BUG: there seems to be an ooRexx issue with comparisons
    //      like .z[17] = 17, which doesn't call compareTo()
    MPZ_long_int op2_int;
    if (!context->OBJ_long_int(op2, &op2_int))
    {
      // Method argument &1 must be a whole number; found "&2".
      invalidWholeNumberException(context, "op2", op2);
      return 0;                        // cannot return NULLOBJECT
    }
    result = mpz_cmp_si(SELF_Z(op1), op2_int);
    // make sure that we only return exactly -1, 0, or 1 (is this really needed?)
    return result < 0 ? -1 : result > 0;
  }
}


//  int mpz_sgn (const mpz_t op)
RexxMethod1(int, z_sgn,                // method sgn()
            CSELF, op)
{
  return mpz_sgn(SELF_Z(op));
}



// ---------------------------------------------------------------------
// 5.11 Logical and Bit Manipulation Functions
// https://gmplib.org/manual/Integer-Logic-and-Bit-Fiddling.html
// void mpz_and (mpz_t rop, const mpz_t op1, const mpz_t op2)
// void mpz_ior (mpz_t rop, const mpz_t op1, const mpz_t op2)
// void mpz_xor (mpz_t rop, const mpz_t op1, const mpz_t op2)
// void mpz_com (mpz_t rop, const mpz_t op)
// mp_bitcnt_t mpz_popcount (const mpz_t op)
// mp_bitcnt_t mpz_hamdist (const mpz_t op1, const mpz_t op2)
// mp_bitcnt_t mpz_scan0 (const mpz_t op, mp_bitcnt_t starting_bit)
// mp_bitcnt_t mpz_scan1 (const mpz_t op, mp_bitcnt_t starting_bit)
// void mpz_setbit (mpz_t rop, mp_bitcnt_t bit_index)
// void mpz_clrbit (mpz_t rop, mp_bitcnt_t bit_index)
// void mpz_combit (mpz_t rop, mp_bitcnt_t bit_index)
// int mpz_tstbit (const mpz_t op, mp_bitcnt_t bit_index)


// void mpz_and (mpz_t rop, const mpz_t op1, const mpz_t op2)
RexxMethod2(RexxObjectPtr, z_and,      // method and()
            RexxObjectPtr, op2,
            CSELF, op1)
{
  CHECK_IF_Z_RXORXO(op2)               // op2 must be a Z instance
  // create new result Z instance
  RexxObjectPtr rop = context->SendMessage0(TheZClass, "NEW");

  mpz_and(OBJ_SELF_Z(rop), SELF_Z(op1), OBJ_SELF_Z(op2));
  return rop;                          // return result instance
}


// void mpz_ior (mpz_t rop, const mpz_t op1, const mpz_t op2)
RexxMethod2(RexxObjectPtr, z_ior,      // method ior()
            RexxObjectPtr, op2,
            CSELF, op1)
{
  CHECK_IF_Z_RXORXO(op2)               // op2 must be a Z instance
  // create new result Z instance
  RexxObjectPtr rop = context->SendMessage0(TheZClass, "NEW");

  mpz_ior(OBJ_SELF_Z(rop), SELF_Z(op1), OBJ_SELF_Z(op2));
  return rop;                          // return result instance
}


// void mpz_xor (mpz_t rop, const mpz_t op1, const mpz_t op2)
RexxMethod2(RexxObjectPtr, z_xor,      // method xor()
            RexxObjectPtr, op2,
            CSELF, op1)
{
  CHECK_IF_Z_RXORXO(op2)               // op2 must be a Z instance
  // create new result Z instance
  RexxObjectPtr rop = context->SendMessage0(TheZClass, "NEW");

  mpz_xor(OBJ_SELF_Z(rop), SELF_Z(op1), OBJ_SELF_Z(op2));
  return rop;                          // return result instance
}


// void mpz_com (mpz_t rop, const mpz_t op)
RexxMethod1(RexxObjectPtr, z_com,      // method com()
            CSELF, op)
{
  // create new result Z instance
  RexxObjectPtr rop = context->SendMessage0(TheZClass, "NEW");

  mpz_com(OBJ_SELF_Z(rop), SELF_Z(op));
  return rop;                          // return result instance
}



// ---------------------------------------------------------------------
// 5.13 Random Number Functions
// https://gmplib.org/manual/Integer-Random-Numbers.html

// void mpz_urandomb (mpz_t rop, gmp_randstate_t state, mp_bitcnt_t n)
// void mpz_urandomm (mpz_t rop, gmp_randstate_t state, const mpz_t n)
// void mpz_rrandomb (mpz_t rop, gmp_randstate_t state, mp_bitcnt_t n)
RexxMethod1(RexxObjectPtr, z_random_cls, // class method random()
            size_t, n)
/* TO DO:
support arg1=z in addition to size_t
add arg2: either "u" or "r" ( u/r/randomb() )
add arg3: seed
  // void gmp_randseed (gmp_randstate_t state, const mpz_t seed)
  // void gmp_randseed_ui (gmp_randstate_t state, unsigned long int seed)
*/
{
  // as mpz_?random?() has no Z class receiver object, we have to
  // implement this as a class method, .z~random(n)
  RexxObjectPtr rop = context->SendMessage0(TheZClass, "NEW");
  mpz_urandomb(OBJ_SELF_Z(rop), RandomState, n);
  return rop;                          // return result instance
}



// ---------------------------------------------------------------------
// 5.15 Miscellaneous Functions
// https://gmplib.org/manual/Miscellaneous-Integer-Functions.html


// int mpz_even_p (const mpz_t op)
RexxMethod1(logical_t, z_even,         // method even(z)
            CSELF, op)
{
  return mpz_even_p(SELF_Z(op));
}


// int mpz_odd_p (const mpz_t op)
RexxMethod1(logical_t, z_odd,          // method odd(z)
            CSELF, op)
{
  return mpz_odd_p(SELF_Z(op));
}


// size_t mpz_sizeinbase (const mpz_t op, int base)
RexxMethod2(size_t, z_sizeInBase,      // method sizeInBase( [base] )
            OPTIONAL_MPZ_int, base,
            CSELF, op)
{
  if (argumentOmitted(1))              // no base given?  assume 10
  {
    base = 10;
  }

  // instead of trying to catch any invalid-base exception thrown by GMP,
  // we check 'base' for the range 2-62 (0 isn't allowed)
  if (base < 2 || base > 62)
  {
    // Argument &1 must be &2in the range 2-62; found "&3"
    notValidBaseException(context, "base", "", base);
    return 0;                          // cannot return NULLOBJECT
  }

  return mpz_sizeinbase(SELF_Z(op), base);
}



// ---------------------------------------------------------------------
// 3.8 Useful Macros and Constants
// https://gmplib.org/manual/Useful-Macros-and-Constants.html

// const int mp_bits_per_limb
// const char * const gmp_version
RexxMethod0(RexxStringObject, z_version_cls) // class method version()
{
  char buffer[200];

  // below GMP_VERSION will give the version from the (static) gmp.h,
  // not from the dynamic mpir.dll, which we really would like to know
  // but, alas, I can't make 'gmp_version' work as doumented
  // see https://groups.google.com/forum/#!topic/mpir-devel/02_8WL41aq0
  // (neither do 'mpir_version', 'mp_bits_per_limb', and 'gmp_errno' work for me)
  snprintf(buffer, sizeof(buffer), "mpz %s, GMP %s %d bit\0", MPZ_VERSION, GMP_VERSION, GMP_NUMB_BITS);
  return context->String(buffer);      // create string object and return it
/* generates four LNK2019: unresolved external symbol errors
int e = gmp_errno;
char c = gmp_version[0];
char d = mpir_version[0];
size_t b = mp_bits_per_limb;
snprintf(buffer, sizeof(buffer), "e %d gmp_v %c mpir_v %c bits %zu\0", e, c, d, b);
*/
}


// ---------------------------------------------------------------------
// Non-GMP convenience 'String'-like functions

RexxMethod2(logical_t, z_endsWith,     // method endsWith()
            RexxObjectPtr, tail,
            CSELF, op)
{
  mpz_t    mod;
  size_t   size;
  int      base = 10;
  char     *mod_string;
  bool     result;

  // TO DO: calculate OBJ_SELF_Z(tail) only once
  MPZ *tailSelf = (MPZ *)context->ObjectToCSelf(tail); // calculate OBJ_SELF_Z(tail) only once

  // we support two endsWith() forms:
  //   endsWith(z)
  //   endsWith(string)
  // note that .z[-999]~endsWith(99) == .true, but .z[-999]~endsWith(-99) == .false
  if (context->IsInstanceOf(tail, TheZClass))
  {
    // endsWith(z) form

    // op < tail?  return false
    if (mpz_cmpabs(SELF_Z(op), OBJ_SELF_Z(tail)) < 0)
    {
      return 0;                        // if |op| < |tail|, op cannot end with tail
    }

/* BUG
.z[-123]~endsWith(23 / .z[23]) == .false
.z[-123]~endsWith() == .false
*/

    // for large 'op' it should be faster to compare op mod base^size to tail
    // instead of converting 'op' to a string and test for 'tail'
    size = mpz_sizeinbase(OBJ_SELF_Z(tail), base); // either exact or 1 too big
    mpz_init(mod);
    mpz_ui_pow_ui(mod, base, size - 1); // first test with mod := 10^(size - 1)
    if (mpz_cmpabs(mod, OBJ_SELF_Z(tail)) <= 0) // too small?
      mpz_mul_ui(mod, mod, 10);        // use mod := 10^size
    mpz_mod(mod, SELF_Z(op), mod);     // 'mod' contains trailing digits of 'op'
    // if mod == tail, return true, otherwise return false
    result = mpz_cmpabs(mod, OBJ_SELF_Z(tail)) == 0 ? 1 : 0;
  }
  else
  {
    // endsWith(string) form

    // assuming that 'tail' typically is short in comparison to 'op',
    // we compare op mod base^size against 'tail' in string form
    RexxStringObject tailString = context->ObjectToString(tail);
    size = context->StringLength(tailString);
    mpz_init(mod);
    mpz_ui_pow_ui(mod, base, size);    // mod := 10^size
    mpz_mod(mod, SELF_Z(op), mod);     // 'mod' contains trailing digits of 'op'
    mod_string = mpz_get_str(NULL, base, mod); // convert 'mod' to string
    result = strncmp(mod_string, context->StringData(tailString), size) == 0;
  }
  mpz_clear(mod);

  return result;
}



// ---------------------------------------------------------------------
// Test

RexxMethod3(RexxObjectPtr, z_test_cls, // class method test()
            RexxObjectPtr, op,
            OPTIONAL_int64_t, i64,
            OPTIONAL_int32_t, i32
//          MPZ_unsigned_long_int, ilong
           )
{

  MPZ *opSelf = (MPZ *)context->ObjectToCSelf(op);
  if (opSelf == NULL)
    printf("z_test_cls: opSelf == NULL\n");
  else if (opSelf->id = ID_MPZ)
    printf("z_test_cls: id = 'mpz'\n");
  else
    printf("z_test_cls: id <> 'mpz', but %d\n", opSelf->id);

  if (context->IsInstanceOf(op, TheZClass))
  {
    printf("z_test_cls: op IsInstanceOf MPZ: %s\n", mpz_get_str(NULL, 10, opSelf->z)); // OBJ_SELF_Z(op)
  }
  else
  {
    printf("z_test_cls: op is string: %s\n", context->ObjectToStringValue(op));
  }
  if (argumentExists(2))
    printf("z_test_cls: i64 is: %lld\n", i64);
  if (argumentExists(3))
    printf("z_test_cls: i32 is: %ld\n", i32);
//if (argumentExists(4))
//  printf("z_test_cls: ilong is: %Id\n", ilong); // %Id required for clean x86 and x64 compile

  return NULLOBJECT;                   // no return value
}




// routine naming is somewhat confusing. let's take 'add_ui' as an example:
// C GMP name              mpz_add_ui
// C RexxRoutine(n) name   z_rtn_add_ui
// REXX_TYPED_ROUTINE name rtn_add_ui (*)
// ooRexx ::routine name   mpz_add_ui
//
// (*) it would be nice, if the REXX_TYPED_ROUTINE name was also called 'mpz_add_ui'
// but that won't work as it collides with the original C GMP name 'mpz_add_ui'
// so we call it 'rtn_add_ui' and let the ooRexx directive
// ::routine mpz_add_ui public external 'library mpz rtn_add_ui'
// do the renaming
RexxRoutineEntry mpz_routines[] = {

  // 5.2 Assignment Functions
  TYPED_ROUTINE(set            ),
  TYPED_ROUTINE(set_ui         ),
  TYPED_ROUTINE(set_si         ),

  // 5.5 Arithmetic Functions
  TYPED_ROUTINE(add            ),
  TYPED_ROUTINE(add_ui         ),
  TYPED_ROUTINE(sub            ),
  TYPED_ROUTINE(sub_ui         ),
  TYPED_ROUTINE(ui_sub         ),
  TYPED_ROUTINE(mul            ),
  TYPED_ROUTINE(mul_si         ),
  TYPED_ROUTINE(mul_ui         ),
  TYPED_ROUTINE(addmul         ),
  TYPED_ROUTINE(addmul_ui      ),
  TYPED_ROUTINE(submul         ),
  TYPED_ROUTINE(submul_ui      ),
  TYPED_ROUTINE(mul_2exp       ),
  TYPED_ROUTINE(neg            ),
  TYPED_ROUTINE(abs            ),

  // 5.6 Division Functions
  TYPED_ROUTINE(cdiv_q         ),
  TYPED_ROUTINE(cdiv_r         ),
  TYPED_ROUTINE(cdiv_qr        ),
  TYPED_ROUTINE(tdiv_q         ),
  TYPED_ROUTINE(tdiv_r         ),
  TYPED_ROUTINE(tdiv_qr        ),
  TYPED_ROUTINE(mod            ),
  TYPED_ROUTINE(mod_ui         ),
  TYPED_ROUTINE(divexact       ),
  TYPED_ROUTINE(divexact_ui    ),
  TYPED_ROUTINE(divisible_p    ),
  TYPED_ROUTINE(divisible_ui_p ),
  TYPED_ROUTINE(divisible_2exp_p ),
  TYPED_ROUTINE(congruent_p    ),
  TYPED_ROUTINE(congruent_ui_p ),
  TYPED_ROUTINE(congruent_2exp_p ),

  // 5.7 Exponentiation Functions
  TYPED_ROUTINE(powm           ),
  TYPED_ROUTINE(powm_ui        ),
  TYPED_ROUTINE(pow_ui         ),
  TYPED_ROUTINE(ui_pow_ui      ),

  // 5.8 Root Extraction Functions
  TYPED_ROUTINE(root           ),
  TYPED_ROUTINE(rootrem        ),
  TYPED_ROUTINE(sqrt           ),
  TYPED_ROUTINE(sqrtrem        ),
  TYPED_ROUTINE(perfect_power_p ),
  TYPED_ROUTINE(perfect_square_p ),

  // 5.9 Number Theoretic Functions
  TYPED_ROUTINE(probab_prime_p ),
  TYPED_ROUTINE(nextprime      ),
  TYPED_ROUTINE(gcd            ),
  TYPED_ROUTINE(gcd_ui         ),
  TYPED_ROUTINE(gcdext         ),
  TYPED_ROUTINE(lcm            ),
  TYPED_ROUTINE(lcm_ui         ),
  TYPED_ROUTINE(invert         ),
  TYPED_ROUTINE(jacobi         ),
  TYPED_ROUTINE(legendre       ),
  TYPED_ROUTINE(kronecker      ),
  TYPED_ROUTINE(kronecker_si   ),
  TYPED_ROUTINE(kronecker_ui   ),
  TYPED_ROUTINE(si_kronecker   ),
  TYPED_ROUTINE(ui_kronecker   ),
  TYPED_ROUTINE(remove         ),
  TYPED_ROUTINE(fac_ui         ),
  TYPED_ROUTINE(fac_ui         ),
  TYPED_ROUTINE(2fac_ui        ),
  TYPED_ROUTINE(mfac_uiui      ),
  TYPED_ROUTINE(primorial_ui   ),
  TYPED_ROUTINE(bin_ui         ),
  TYPED_ROUTINE(bin_uiui       ),
  TYPED_ROUTINE(fib_ui         ),
  TYPED_ROUTINE(fib2_ui        ),
  TYPED_ROUTINE(lucnum_ui      ),
  TYPED_ROUTINE(lucnum2_ui     ),

  // 5.10 Comparison Functions
  TYPED_ROUTINE(cmp            ),
  //PED_ROUTINE(cmp_d          ),
  TYPED_ROUTINE(cmp_si         ),
  TYPED_ROUTINE(cmp_ui         ),
  TYPED_ROUTINE(cmpabs         ),
  //PED_ROUTINE(cmpabs_d       ),
  TYPED_ROUTINE(cmpabs_ui      ),
  TYPED_ROUTINE(sgn            ),

  // 5.11 Logical and Bit Manipulation Functions
  TYPED_ROUTINE(and            ),
  TYPED_ROUTINE(ior            ),
  TYPED_ROUTINE(xor            ),
  TYPED_ROUTINE(com            ),
  TYPED_ROUTINE(popcount       ),
  TYPED_ROUTINE(hamdist        ),
  TYPED_ROUTINE(scan0          ),
  TYPED_ROUTINE(scan1          ),
  TYPED_ROUTINE(setbit         ),
  TYPED_ROUTINE(clrbit         ),
  TYPED_ROUTINE(combit         ),
  TYPED_ROUTINE(tstbit         ),


  REXX_LAST_ROUTINE()
};


// method naming is different from above routine naming.  let's take 'add' as an example:
// C GMP names             mpz_add, mpz_add_ui
// C RexxMethod(n) name    z_add
// REXX_METHOD name        add (*)
// ooRexx ::routine names  '+', add
//
// (*) by naming the REXX_METHOD 'add' we can use the ooRexx directive
// ::routine add public external 'library mpz'
RexxMethodEntry mpz_methods[] = {
  // class methods
  REXX_METHOD(init_cls,      z_init_cls),
  REXX_METHOD(of_cls,        z_of_cls),
  REXX_METHOD(version,       z_version_cls),
  REXX_METHOD(test,          z_test_cls),

  // instance methods

  // 5.1 Initialization Functions
  REXX_METHOD(init,          z_init),
  REXX_METHOD(uninit,        z_uninit),
  REXX_METHOD(swap,          z_swap),

  // 5.2 Assignment Functions
  REXX_METHOD(set,           z_set),

  // 5.4 Conversion Functions
  REXX_METHOD(get,           z_get),

  // 5.5 Arithmetic Functions
  REXX_METHOD(add2,          z_add2), // development only

  REXX_METHOD(add,           z_add),
  REXX_METHOD(sub,           z_sub),
  REXX_METHOD(mul,           z_mul),
  REXX_METHOD(addMul,        z_addMul),
  REXX_METHOD(subMul,        z_subMul),
  REXX_METHOD(mul2exp,       z_mul2exp),
  REXX_METHOD(abs,           z_abs),

  // 5.6 Division Functions
  REXX_METHOD(div,           z_div),
  REXX_METHOD(divExact,      z_divExact),
  REXX_METHOD(divisible,     z_divisible),
  REXX_METHOD(congruent,     z_congruent),
  REXX_METHOD(mod,           z_mod),

  // 5.7 Exponentiation Functions
  REXX_METHOD(powm,          z_powm),
  REXX_METHOD(pow,           z_pow),

  // 5.8 Root Extraction Functions
  REXX_METHOD(sqrt,          z_sqrt),
  REXX_METHOD(root,          z_root),
  REXX_METHOD(perfectPower,  z_perfectPower),
  REXX_METHOD(perfectSquare, z_perfectSquare),

  // 5.9 Number Theoretic Functions
  REXX_METHOD(probabPrime,   z_probabPrime),
  REXX_METHOD(nextPrime,     z_nextPrime),
  REXX_METHOD(gcd,           z_gcd),
  REXX_METHOD(lcm,           z_lcm),
  REXX_METHOD(remove,        z_remove),
  REXX_METHOD(fac,           z_fac_cls),
  REXX_METHOD(2fac,          z_2fac_cls),
  REXX_METHOD(mfac,          z_mfac_cls),
  REXX_METHOD(primorial,     z_primorial_cls),
  REXX_METHOD(invert,        z_invert),
  REXX_METHOD(bin,           z_bin),

  // 5.10 Comparison Functions
  REXX_METHOD(compareTo,     z_compareTo),
  REXX_METHOD(sgn,           z_sgn),

  // 5.11 Logical and Bit Manipulation Functions
  REXX_METHOD(and,           z_and),
  REXX_METHOD(ior,           z_ior),
  REXX_METHOD(xor,           z_xor),
  REXX_METHOD(com,           z_com),

  // 5.13 Random Number Functions
  REXX_METHOD(random,        z_random_cls),

  // 5.15 Miscellaneous Functions
  REXX_METHOD(even,          z_even),
  REXX_METHOD(odd,           z_odd),
  REXX_METHOD(sizeInBase,    z_sizeInBase),

  // Non-GMP convenience 'String'-like functions
  REXX_METHOD(endsWith,      z_endsWith),

  REXX_LAST_METHOD()
};


RexxPackageEntry mpz_package_entry = {
  STANDARD_PACKAGE_HEADER
  REXX_INTERPRETER_4_0_0,              // anything after 4.0.0 will work
  "mpz",                               // name of the package
  MPZ_VERSION,                         // package information
  NULL,                                // no load/unload functions
  NULL,
  mpz_routines,                        // the exported routines
  mpz_methods                          // the exported methods
};

// package loading stub.
OOREXX_GET_PACKAGE(mpz);
