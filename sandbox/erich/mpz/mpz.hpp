// ---------------------------------------------------------------------
// typedefs, DEFINEs, helper routines, exception routines

// CSELF struct
typedef struct MPZ
{
  int    id;            // MPZ identification, see ID_MPZ below
  mpz_t  z;             // MPZ data
} mpz;

// we mark our CSELF struct with a magic number, so that methods
// and routines can make sure they receive a valid Z instance
#define ID_MPZ 8024173  // magic number to mark an 'MPZ' as such


// macro to simplify making object data available from a CSELF
// e. g. instead of MPZ *op1Self = (MPZ *)op1; op1Self->z
#define SELF_Z(arg)          ((MPZ *)arg)->z

// macro to simplify making object data available from a RexxObjectPtr
// e. g. instead of MPZ *ropSelf = (MPZ *)context->ObjectToCSelf(rop); ropSelf->z
#define OBJ_SELF_Z(arg)      ((MPZ *)context->ObjectToCSelf(arg))->z


// here we define which MPZ types (e. g. int, unsigned long int) should
// be which types used in a RexxMethod argument
#define MPZ_int                 int32_t
#define OPTIONAL_MPZ_int        OPTIONAL_int32_t


// here we define how MPZ types (un)signed long int should
// be converted from a RexxObject, e. g.
//    MPZ_unsigned_long_int result;
//    if (!context->OBJ_unsigned_long_int(op2, &result))

// we're using the [Unsigned]Int32/64() functions
// instead of the equivalent ObjectTo[Unsigned]Int32/64() functions
// as some exception routines below require conversion int -> Object

#ifdef _M_X64      // MSVC define for compilations that target x64 processors
#define MPZ_unsigned_long_int   uint64_t
#define OBJ_unsigned_long_int   UnsignedInt64    // converts Object <--> int
#define MPZ_long_int            int64_t          // signed long integer
#define OBJ_long_int            Int64            // signed long integer
#else
#define MPZ_unsigned_long_int   uint32_t
#define OBJ_unsigned_long_int   UnsignedInt32    // converts Object <--> int
#define MPZ_long_int            int32_t          // signed long integer
#define OBJ_long_int            Int32            // signed long integer
#endif

// this library version
// used in method version() and in STANDARD_PACKAGE_HEADER
#define MPZ_VERSION             "0.6." __DATE__ " " __TIME__ // temporary; should be e. g. "0.6.20160222"


// ---------------------------------------------------------------------
// Macros to convert GMP functions to RexxRoutine's

#define REXX_ROUTINE_1(ret, name, t1, n1) \
RexxRoutine1(ret, z_rtn_##name, t1, n1) \
{ CHECK_IF_Z_##ret##t1(n1) \
  RETURN_IF_##ret mpz_##name(MAKE_ARG_##t1(n1)); \
  RETURN_NULL_IF_##ret \
}
#define REXX_ROUTINE_2(ret, name, t1, n1, t2, n2) \
RexxRoutine2(ret, z_rtn_##name, t1, n1, t2, n2) \
{ CHECK_IF_Z_##ret##t1(n1) \
  CHECK_IF_Z_##ret##t2(n2) \
  RETURN_IF_##ret mpz_##name(MAKE_ARG_##t1(n1), MAKE_ARG_##t2(n2)); \
  RETURN_NULL_IF_##ret \
}
#define REXX_ROUTINE_3(ret, name, t1, n1, t2, n2, t3, n3) \
RexxRoutine3(ret, z_rtn_##name, t1, n1, t2, n2, t3, n3) \
{ CHECK_IF_Z_##ret##t1(n1) \
  CHECK_IF_Z_##ret##t2(n2) \
  CHECK_IF_Z_##ret##t3(n3) \
  RETURN_IF_##ret mpz_##name(MAKE_ARG_##t1(n1), MAKE_ARG_##t2(n2), MAKE_ARG_##t3(n3)); \
  RETURN_NULL_IF_##ret \
}
#define REXX_ROUTINE_4(ret, name, t1, n1, t2, n2, t3, n3, t4, n4) \
RexxRoutine4(ret, z_rtn_##name, t1, n1, t2, n2, t3, n3, t4, n4) \
{ CHECK_IF_Z_##ret##t1(n1) \
  CHECK_IF_Z_##ret##t2(n2) \
  CHECK_IF_Z_##ret##t3(n3) \
  CHECK_IF_Z_##ret##t4(n4) \
  RETURN_IF_##ret mpz_##name(MAKE_ARG_##t1(n1), MAKE_ARG_##t2(n2), MAKE_ARG_##t3(n3), MAKE_ARG_##t4(n4)); \
  RETURN_NULL_IF_##ret \
}
#define REXX_ROUTINE_5(ret, name, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5) \
RexxRoutine5(ret, z_rtn_##name, t1, n1, t2, n2, t3, n3, t4, n4, t5, n5) \
{ CHECK_IF_Z_##ret##t1(n1) \
  CHECK_IF_Z_##ret##t2(n2) \
  CHECK_IF_Z_##ret##t3(n3) \
  CHECK_IF_Z_##ret##t4(n4) \
  CHECK_IF_Z_##ret##t5(n5) \
  RETURN_IF_##ret mpz_##name(MAKE_ARG_##t1(n1), MAKE_ARG_##t2(n2), MAKE_ARG_##t3(n3), MAKE_ARG_##t4(n4), MAKE_ARG_##t5(n5)); \
  RETURN_NULL_IF_##ret \
}

#define CHECK_IF_Z_RXORXO(NAME)     if (!context->IsInstanceOf(NAME, TheZClass)) return wrongArgClassException(context, #NAME, TheZClass);
#define CHECK_IF_Z_RXORXULONG(NAME)
#define CHECK_IF_Z_RXORXLONG(NAME)
#define CHECK_IF_Z_RXORXLONG(NAME)
#define CHECK_IF_Z_RXORXINT(NAME)
#define CHECK_IF_Z_RXINTRXO(NAME)   if (!context->IsInstanceOf(NAME, TheZClass)) { wrongArgClassException(context, #NAME, TheZClass); return 0; }
#define CHECK_IF_Z_RXINTRXULONG(NAME)
#define CHECK_IF_Z_RXINTRXLONG(NAME)
#define CHECK_IF_Z_RXINTRXLONG(NAME)
#define CHECK_IF_Z_RXINTRXINT(NAME)
#define CHECK_IF_Z_RXULONGRXO(NAME) if (!context->IsInstanceOf(NAME, TheZClass)) { wrongArgClassException(context, #NAME, TheZClass); return 0; }
#define CHECK_IF_Z_RXULONGRXULONG(NAME)
#define CHECK_IF_Z_RXULONGRXLONG(NAME)
#define CHECK_IF_Z_RXULONGRXLONG(NAME)
#define CHECK_IF_Z_RXULONGRXINT(NAME)

#define MAKE_ARG_RXO(NAME)      OBJ_SELF_Z(NAME)
#define MAKE_ARG_RXULONG(NAME)  NAME
#define MAKE_ARG_RXLONG(NAME)   NAME
#define MAKE_ARG_RXINT(NAME)    NAME

#define RETURN_IF_RXO
#define RETURN_IF_RXINT         return
#define RETURN_IF_RXULONG       return

#define RETURN_NULL_IF_RXO      return NULLOBJECT;
#define RETURN_NULL_IF_RXINT
#define RETURN_NULL_IF_RXULONG

#define RXO                     RexxObjectPtr
#define RXULONG                 MPZ_unsigned_long_int
#define RXLONG                  MPZ_long_int
#define RXINT                   MPZ_int

// ---------------------------------------------------------------------
// Macro to ease entering REXX_TYPED_ROUTINE's

#define TYPED_ROUTINE(name)     REXX_TYPED_ROUTINE(rtn_##name, z_rtn_##name)



// ---------------------------------------------------------------------
// helper function for message insertion of a z argument
// returns 'z' as a string, if it's not too large
// returns first few digits of 'z' with ".. (nn digits)" appended, if 'z' is very large
RexxObjectPtr shortenedZ(RexxMethodContext *c, mpz_t z)
{
  mpz_t    first_few, many_trailing;
  size_t   z_digits;
  size_t   FEW = 20;
  char     buffer[200];

  z_digits = mpz_sizeinbase(z, 10);    // how many digts does 'z' have?
  if (z_digits <= FEW)                 // small enough?  we're done
    return c->CString(mpz_get_str(NULL, 10, z));

  // too large, chop off some trailing digits and replace with ".. (nn digits)"
  mpz_init_set(first_few, z);         // first_few := op
  mpz_init_set_ui(many_trailing, 10); // many_trailing := 10
  mpz_pow_ui(many_trailing, many_trailing, z_digits - FEW); // many_trailing := 10^(almost all digits)
  mpz_tdiv_q(first_few, first_few, many_trailing); // cut off many trailing digits
  snprintf(buffer, sizeof(buffer), "%s.. (%zd digits)", mpz_get_str(NULL, 10, first_few), z_digits);
  return c->String(buffer);
}


// ---------------------------------------------------------------------
// raises 93.900
// Argument &1 must be a valid number in base &2, found "&3"
// e. g. Error 93.900:  Argument op must be a valid number in base 2, found "123".
RexxObjectPtr notValidNumberInBaseException(RexxMethodContext *c, const char *arg, RexxObjectPtr number, MPZ_int base)
{
  char buffer[200];

  snprintf(buffer, sizeof(buffer),
    "Argument %s must be a valid number in base %ld, found \"%s\"", arg, base, c->CString(number));
  c->RaiseException1(Rexx_Error_Incorrect_method_user_defined, c->String(buffer));
  return NULLOBJECT;
}


// ---------------------------------------------------------------------
// raises 93.900
// Argument &1 must be &2in the range 2-62; found "&3"
// e. g. Error 93.900:  Argument base must be 0 or in the range 2-62; found "-1".
// e. g. Error 93.900:  Argument base must be in the range 2-62; found "0".
RexxObjectPtr notValidBaseException(RexxMethodContext *c, const char *arg, const char *zero, MPZ_int base)
{
  char buffer[200];

  snprintf(buffer, sizeof(buffer),
    "Argument %s must be %sin the range 2-62; found \"%ld\"", arg, zero, base);
  c->RaiseException1(Rexx_Error_Incorrect_method_user_defined, c->String(buffer));
  return NULLOBJECT;
}


// ---------------------------------------------------------------------
// raises 93.900
// For argument &1 a root only exists for &2; found "&3"
// e. g. Error 93.900:  For argument op < 0 a root only exists for odd n; found "2".
RexxObjectPtr noRootExistsException(RexxMethodContext *c, const char *arg, const char *n, MPZ_long_int actual)
{
  char buffer[200];

  snprintf(buffer, sizeof(buffer),
    "For argument %s a root only exists for %s; found \"%Id\"", arg, n, actual); // %Id required for clean x86 and x64 compile
  c->RaiseException1(Rexx_Error_Incorrect_method_user_defined, c->String(buffer));
  return NULLOBJECT;
}


// ---------------------------------------------------------------------
// raises 93.900
// Method argument &1 must not be zero
// e. g. Error 93.900:  Method argument mod must not be zero.
RexxObjectPtr mustNotBeZeroException(RexxMethodContext *c, const char *arg)
{
  char buffer[200];

  snprintf(buffer, sizeof(buffer),
    "Method argument %s must not be zero", arg);
  c->RaiseException1(Rexx_Error_Incorrect_method_user_defined, c->String(buffer));
  return NULLOBJECT;
}


// ---------------------------------------------------------------------
// raises 93.900
// Negative exponent only allowed if inverse exists; found "&1"
// e. g. Error 93.900:  Error 93.900:  Negative exponent only allowed if inverse exists; found "-100".
RexxObjectPtr negativeExponentException(RexxMethodContext *c, mpz_t exp)
{
  char buffer[200];

  snprintf(buffer, sizeof(buffer),
    "Negative exponent only allowed if inverse exists; found \"%s\"",
    c->StringData((RexxStringObject)shortenedZ(c, exp)));
  c->RaiseException1(Rexx_Error_Incorrect_method_user_defined, c->String(buffer));
  return NULLOBJECT;
}


// ---------------------------------------------------------------------
// raises 93.902
// Too many arguments in invocation of method; &1 expected.
// e. g. Error 93.902:  Too many arguments in invocation of method; 1 expected.
RexxObjectPtr tooManyArgumentsException(RexxMethodContext *c, MPZ_long_int number)
{
  c->RaiseException1(Rexx_Error_Incorrect_method_maxarg, c->OBJ_long_int(number));
  return NULLOBJECT;
}


// ---------------------------------------------------------------------
// raises 93.905
// Method argument &1 must be a whole number; found "&2".
// e. g. Error 93.905:  Method argument d must be a whole number; found "X".
RexxObjectPtr invalidWholeNumberException(RexxMethodContext *c, const char *arg, RexxObjectPtr actual)
{
  c->RaiseException2(Rexx_Error_Incorrect_method_whole,
                     c->String(arg), actual);
  return NULLOBJECT;
}


// ---------------------------------------------------------------------
// raises 93.906
// Method argument &1 must be zero or a positive whole number; found "&2".
// e. g. Error 93.906:  Method argument op must be zero or a positive whole number; found "-1".
RexxObjectPtr notZeroOrPositiveException(RexxMethodContext *c, const char *arg, RexxObjectPtr actual)
{
  c->RaiseException2(Rexx_Error_Incorrect_method_nonnegative, c->String(arg), actual);
  return NULLOBJECT;
}

RexxObjectPtr notZeroOrPositiveException(RexxMethodContext *c, const char *arg, CSTRING actual)
{
  notZeroOrPositiveException(c, arg, c->String(actual));
  return NULLOBJECT;
}

RexxObjectPtr notZeroOrPositiveException(RexxMethodContext *c, const char *arg, MPZ_long_int actual)
{
  notZeroOrPositiveException(c, arg, c->OBJ_long_int(actual));
  return NULLOBJECT;
}

RexxObjectPtr notZeroOrPositiveException(RexxMethodContext *c, const char *arg, mpz_t actual)
{
  notZeroOrPositiveException(c, arg, shortenedZ(c, actual));
  return NULLOBJECT;
}


// ---------------------------------------------------------------------
// raises 93.907
// Method argument &1 must be a positive whole number; found "&2".
// e. g. Error 93.907:  Method argument n must be a positive whole number; found "0".
RexxObjectPtr notPositiveException(RexxMethodContext *c, const char *arg, RexxObjectPtr actual)
{
  c->RaiseException2(Rexx_Error_Incorrect_method_positive, c->String(arg), actual);
  return NULLOBJECT;
}

RexxObjectPtr notPositiveException(RexxMethodContext *c, const char *arg, CSTRING actual)
{
  notPositiveException(c, arg, c->String(actual));
  return NULLOBJECT;
}

RexxObjectPtr notPositiveException(RexxMethodContext *c, const char *arg, MPZ_long_int actual)
{
  notPositiveException(c, arg, c->OBJ_long_int(actual));
  return NULLOBJECT;
}

RexxObjectPtr notPositiveException(RexxMethodContext *c, const char *arg, mpz_t actual)
{
  notPositiveException(c, arg, shortenedZ(c, actual));
  return NULLOBJECT;
}


// ---------------------------------------------------------------------
// raises 93.914
// Method argument &1 must be one of &2; found "&3".
// e. g. Error 93.914:  Method argument round must be one of CFT; found "X".
RexxObjectPtr wrongArgValueException(RexxMethodContext *c, const char *arg, const char *list, RexxObjectPtr actual)
{
  c->RaiseException(Rexx_Error_Incorrect_method_list,
                    c->ArrayOfThree(c->String(arg), c->String(list), actual));
  return NULLOBJECT;
}

RexxObjectPtr wrongArgValueException(RexxMethodContext *c, const char *arg, const char *list, CSTRING actual)
{
  wrongArgValueException(c, arg, list, c->String(actual));
  return NULLOBJECT;
}


// ---------------------------------------------------------------------
// raises 93.948
// Method argument &1 must be of the &2 class.
// e. g. Error 93.948:  Method argument op2 must be of the Z class.
RexxObjectPtr wrongArgClassException(RexxMethodContext *c, const char *arg, RexxObjectPtr cls)
{
  c->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                     c->String(arg), c->SendMessage0(cls, "ID"));
  return NULLOBJECT;
}

// copy for use from RexxRoutine instead of RexxMethod
RexxObjectPtr wrongArgClassException(RexxCallContext *c, const char *arg, RexxObjectPtr cls)
{
  c->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                     c->String(arg), c->SendMessage0(cls, "ID"));
  return NULLOBJECT;
}


// ---------------------------------------------------------------------
// raises 42.003
// Arithmetic overflow; divisor must not be zero.
// e. g. Error 42.3:  Arithmetic overflow; divisor must not be zero.
RexxObjectPtr divideByZeroException(RexxMethodContext *c)
{
  c->RaiseException0(Rexx_Error_Overflow_zero);
  return NULLOBJECT;
}
