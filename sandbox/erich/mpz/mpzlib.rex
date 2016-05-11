-- GMP MPZ class interface

------------------------------------------------------------------------
-- MPZ method interface

::class z public inherit Orderable Comparable
  -- class methods
  ::method init         class external 'library mpz init_cls' -- cannot be private
  ::method "[]"         class external 'library mpz of_cls'
  ::method version      class external 'library mpz'
  ::method test         class external 'library mpz' -- development dummy

  -- instance methods (plus class methods like fac() or primorial() )

  -- 5.1 Initialization Functions
  -- https://gmplib.org/manual/Initializing-Integers.html
  ::method init         external 'library mpz' -- cannot be private for .z~new() to work
  ::method uninit       external 'library mpz' -- 'private' seems to give a giant perfomance hit
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
  ::method "addx"         external 'library mpz' -- development test

  ::method "+"          external 'library mpz add'
  ::method "-"          external 'library mpz sub'
  ::method "*"          external 'library mpz mul'
  ::method add          external 'library mpz' -- alternate form
  ::method sub          external 'library mpz' -- alternate form
  ::method mul          external 'library mpz' -- alternate form
  ::method addMul       external 'library mpz'
  ::method subMul       external 'library mpz'
  ::method mul2exp      external 'library mpz'
  ::method abs          external 'library mpz'

  -- 5.6 Division Functions
  -- https://gmplib.org/manual/Integer-Division.html
  ::method "%"          external 'library mpz div'
  ::method div          external 'library mpz' -- alternate form; allows ~div(d, C/F/T)
  ::method divExact     external 'library mpz'
  ::method divisible    external 'library mpz'
  ::method congruent    external 'library mpz'
  ::method "//"         external 'library mpz mod'

  -- 5.7 Exponentiation Functions
  -- https://gmplib.org/manual/Integer-Exponentiation.html
  ::method "**"         external 'library mpz pow'
  ::method pow          external 'library mpz' -- alternate form
  ::method powm         external 'library mpz'

  -- 5.8 Root Extraction Functions
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
  ::method sgn          external 'library mpz'

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

::routine add       public external 'library mpz rtn_add' 
::routine add_ui    public external 'library mpz rtn_add_ui'
::routine sub       public external 'library mpz rtn_sub' 
::routine sub_ui    public external 'library mpz rtn_sub_ui'
::routine mul       public external 'library mpz rtn_mul' 
::routine mul_si    public external 'library mpz rtn_mul_si'
::routine mul_ui    public external 'library mpz rtn_mul_ui'
::routine addmul    public external 'library mpz rtn_addmul'
::routine addmul_ui public external 'library mpz rtn_addmul_ui'
::routine submul    public external 'library mpz rtn_submul'
::routine submul_ui public external 'library mpz rtn_submul_ui'
::routine cdiv_q    public external 'library mpz rtn_cdiv_q'
::routine cdiv_r    public external 'library mpz rtn_cdiv_r'
::routine cdiv_qr   public external 'library mpz rtn_cdiv_qr'
::routine powm      public external 'library mpz rtn_powm'
::routine powm_ui   public external 'library mpz rtn_powm_ui'
::routine pow_ui    public external 'library mpz rtn_pow_ui'
::routine ui_pow_ui public external 'library mpz rtn_ui_pow_ui'

::routine nextprime public external 'library mpz rtn_nextprime'
::routine gcd       public external 'library mpz rtn_gcd'
::routine fac_ui    public external 'library mpz rtn_fac_ui'

