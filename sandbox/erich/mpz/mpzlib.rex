-- GMP MPZ class interface

------------------------------------------------------------------------
-- MPZ method interface

::class z public inherit Orderable Comparable

  -- constructor and miscellaneous
  ::method init         class external 'library mpz init_cls' -- cannot be private
  ::method "[]"         class external 'library mpz of_cls'
  ::method version      class external 'library mpz'
  ::method test         class external 'library mpz' -- development only
  ::attribute cself get                              -- development only

  -- 5.1 Initialization Functions
  -- https://gmplib.org/manual/Initializing-Integers.html
  ::method init         external 'library mpz'   -- cannot be private for .z~new() to work
  ::method uninit       external 'library mpz'   -- 'private' seems to give a giant perfomance hit
  ::method swap         external 'library mpz'

  -- 5.2 Assignment Functions
  -- https://gmplib.org/manual/Assigning-Integers.html
  ::method set          external 'library mpz'

  -- 5.4 Conversion Functions
  -- https://gmplib.org/manual/Converting-Integers.html
  ::method get          external 'library mpz'
  ::method string       external 'library mpz get'

  -- 5.5 Arithmetic Functions
  -- https://gmplib.org/manual/Integer-Arithmetic.html
  ::method "add2"          external 'library mpz'   -- development only

  ::method "+"          external 'library mpz add'
  ::method "-"          external 'library mpz sub'
  ::method "*"          external 'library mpz mul'
  ::method add          external 'library mpz'   -- alternate form
  ::method sub          external 'library mpz'   -- alternate form
  ::method mul          external 'library mpz'   -- alternate form
  ::method addMul       external 'library mpz'
  ::method subMul       external 'library mpz'
  ::method mul2exp      external 'library mpz'
  ::method abs          external 'library mpz'

  -- 5.6 Division Functions
  -- https://gmplib.org/manual/Integer-Division.html
  ::method "%"          external 'library mpz div'
  ::method div          external 'library mpz'   -- alternate form; allows ~div(d, C/F/T)
  ::method divExact     external 'library mpz'
  ::method divisible    external 'library mpz'
  ::method congruent    external 'library mpz'
  ::method "//"         external 'library mpz mod'
  ::method mod          external 'library mpz'   -- alternate form

  -- 5.7 Exponentiation Functions
  -- https://gmplib.org/manual/Integer-Exponentiation.html
  ::method "**"         external 'library mpz pow'
  ::method pow          external 'library mpz'   -- alternate form
  ::method powm         external 'library mpz'

  -- 5.8 Root Extraction Functions
  -- https://gmplib.org/manual/Integer-Roots.html
  ::method sqrt         external 'library mpz'
  ::method root         external 'library mpz'
  ::method perfectSquare external 'library mpz'
  ::method perfectPower external 'library mpz'

  -- 5.9 Number Theoretic Functions
  -- https://gmplib.org/manual/Number-Theoretic-Functions.html
  ::method probabPrime  external 'library mpz'
  ::method nextPrime    external 'library mpz'
  ::method gcd          external 'library mpz'
  ::method lcm          external 'library mpz'
  ::method remove       external 'library mpz'
  ::method fac          class external 'library mpz'
  ::method 2fac         class external 'library mpz'
  ::method mfac         class external 'library mpz'
  ::method primorial    class external 'library mpz'
  ::method invert       external 'library mpz'
  ::method bin          external 'library mpz'

  -- 5.10 Comparison Functions
  -- https://gmplib.org/manual/Integer-Comparisons.html
  ::method compareTo    external 'library mpz'
  ::method "="          -- fix for [bugs:1365]
    use strict arg other
    return self~compareTo(other) = 0
  ::method "\="         -- fix for [bugs:1365]
    use strict arg other
    return self~compareTo(other) \= 0
  ::method sgn          external 'library mpz'

  -- 5.11 Logical and Bit Manipulation Functions
  -- https://gmplib.org/manual/Integer-Logic-and-Bit-Fiddling.html
  ::method "&"          external 'library mpz and'
  ::method and          external 'library mpz'   -- alternate form
  ::method "|"          external 'library mpz ior'
  ::method ior          external 'library mpz'   -- alternate form
  ::method "&&"         external 'library mpz xor'
  ::method xor          external 'library mpz'   -- alternate form
  ::method com          external 'library mpz'

  -- 5.13 Random Number Functions
  -- https://gmplib.org/manual/Integer-Random-Numbers.html
  ::method random       class external 'library mpz'

  -- 5.15 Miscellaneous Functions
  -- https://gmplib.org/manual/Miscellaneous-Integer-Functions.html
  ::method even         external 'library mpz'
  ::method odd          external 'library mpz'
  ::method sizeInBase   external 'library mpz'

  -- Non-GMP convenience 'String'-like functions
  ::method endsWith     external 'library mpz'


------------------------------------------------------------------------
-- MPZ routine interface

-- https://gmplib.org/manual/Assigning-Integers.html
-- 5.2 Assignment Functions
::routine mpz_set              public external 'library mpz rtn_set'
::routine mpz_set_ui           public external 'library mpz rtn_set_ui'
::routine mpz_set_si           public external 'library mpz rtn_set_si'

-- 5.5 Arithmetic Functions
-- https://gmplib.org/manual/Integer-Arithmetic.html

::routine mpz_add              public external 'library mpz rtn_add'
::routine mpz_add_ui           public external 'library mpz rtn_add_ui'
::routine mpz_sub              public external 'library mpz rtn_sub'
::routine mpz_sub_ui           public external 'library mpz rtn_sub_ui'
::routine mpz_ui_sub           public external 'library mpz rtn_ui_sub'
::routine mpz_mul              public external 'library mpz rtn_mul'
::routine mpz_mul_si           public external 'library mpz rtn_mul_si'
::routine mpz_mul_ui           public external 'library mpz rtn_mul_ui'
::routine mpz_addmul           public external 'library mpz rtn_addmul'
::routine mpz_addmul_ui        public external 'library mpz rtn_addmul_ui'
::routine mpz_submul           public external 'library mpz rtn_submul'
::routine mpz_submul_ui        public external 'library mpz rtn_submul_ui'
::routine mpz_mul_2exp         public external 'library mpz rtn_mul_2exp'
::routine mpz_neg              public external 'library mpz rtn_neg'
::routine mpz_abs              public external 'library mpz rtn_abs'

-- 5.6 Division Functions
-- https://gmplib.org/manual/Integer-Division.html
::routine mpz_cdiv_q           public external 'library mpz rtn_cdiv_q'
::routine mpz_cdiv_r           public external 'library mpz rtn_cdiv_r'
::routine mpz_cdiv_qr          public external 'library mpz rtn_cdiv_qr'

::routine mpz_tdiv_q           public external 'library mpz rtn_tdiv_q'
::routine mpz_tdiv_r           public external 'library mpz rtn_tdiv_r'
::routine mpz_tdiv_qr          public external 'library mpz rtn_tdiv_qr'

::routine mpz_mod              public external 'library mpz rtn_mod'
::routine mpz_mod_ui           public external 'library mpz rtn_mod_ui'
::routine mpz_divexact_ui      public external 'library mpz rtn_divexact_ui'
::routine mpz_divexact         public external 'library mpz rtn_divexact'

-- 5.7 Exponentiation Functions
-- https://gmplib.org/manual/Integer-Exponentiation.html
::routine mpz_powm             public external 'library mpz rtn_powm'
::routine mpz_powm_ui          public external 'library mpz rtn_powm_ui'
::routine mpz_pow_ui           public external 'library mpz rtn_pow_ui'
::routine mpz_ui_pow_ui        public external 'library mpz rtn_ui_pow_ui'

-- 5.8 Root Extraction Functions
-- https://gmplib.org/manual/Integer-Roots.html
::routine mpz_root             public external 'library mpz rtn_root'
::routine mpz_rootrem          public external 'library mpz rtn_rootrem'
::routine mpz_sqrt             public external 'library mpz rtn_sqrt'
::routine mpz_sqrtrem          public external 'library mpz rtn_sqrtrem'
::routine mpz_perfect_power_p  public external 'library mpz rtn_perfect_power_p'
::routine mpz_perfect_square_p public external 'library mpz rtn_perfect_square_p'

-- 5.9 Number Theoretic Functions
-- https://gmplib.org/manual/Number-Theoretic-Functions.html
::routine mpz_probab_prime_p   public external 'library mpz rtn_probab_prime_p'
::routine mpz_nextprime        public external 'library mpz rtn_nextprime'
::routine mpz_gcd              public external 'library mpz rtn_gcd'
::routine mpz_gcd_ui           public external 'library mpz rtn_gcd_ui'
::routine mpz_gcdext           public external 'library mpz rtn_gcdext'
::routine mpz_lcm              public external 'library mpz rtn_lcm'
::routine mpz_lcm_ui           public external 'library mpz rtn_lcm_ui'
::routine mpz_invert           public external 'library mpz rtn_invert'
::routine mpz_jacobi           public external 'library mpz rtn_jacobi'
::routine mpz_legendre         public external 'library mpz rtn_legendre'
::routine mpz_kronecker        public external 'library mpz rtn_kronecker'
::routine mpz_kronecker_si     public external 'library mpz rtn_kronecker_si'
::routine mpz_kronecker_ui     public external 'library mpz rtn_kronecker_ui'
::routine mpz_si_kronecker     public external 'library mpz rtn_si_kronecker'
::routine mpz_ui_kronecker     public external 'library mpz rtn_ui_kronecker'
::routine mpz_remove           public external 'library mpz rtn_remove'
::routine mpz_fac_ui           public external 'library mpz rtn_fac_ui'
::routine mpz_2fac_ui          public external 'library mpz rtn_2fac_ui'
::routine mpz_mfac_uiui        public external 'library mpz rtn_mfac_uiui'

-- 5.10 Comparison Functions
-- https://gmplib.org/manual/Integer-Comparisons.html
::routine mpz_cmp              public external 'library mpz rtn_cmp'
--routine mpz_cmp_d            public external 'library mpz rtn_cmp_d'
::routine mpz_cmp_si           public external 'library mpz rtn_cmp_si'
::routine mpz_cmp_ui           public external 'library mpz rtn_cmp_ui'
::routine mpz_cmpabs           public external 'library mpz rtn_cmpabs'
--routine mpz_cmpabs_d         public external 'library mpz rtn_cmpabs_d'
::routine mpz_cmpabs_ui        public external 'library mpz rtn_cmpabs_ui'
::routine mpz_sgn              public external 'library mpz rtn_sgn'

-- 5.11 Logical and Bit Manipulation Functions
-- https://gmplib.org/manual/Integer-Logic-and-Bit-Fiddling.html
::routine mpz_and              public external 'library mpz rtn_and'
::routine mpz_ior              public external 'library mpz rtn_ior'
::routine mpz_xor              public external 'library mpz rtn_xor'
::routine mpz_com              public external 'library mpz rtn_com'
::routine mpz_popcount         public external 'library mpz rtn_popcount'
::routine mpz_hamdist          public external 'library mpz rtn_hamdist'
::routine mpz_scan0            public external 'library mpz rtn_scan0'
::routine mpz_scan1            public external 'library mpz rtn_scan1'
::routine mpz_setbit           public external 'library mpz rtn_setbit'
::routine mpz_clrbit           public external 'library mpz rtn_clrbit'
::routine mpz_combit           public external 'library mpz rtn_combit'
::routine mpz_tstbit           public external 'library mpz rtn_tstbit'


