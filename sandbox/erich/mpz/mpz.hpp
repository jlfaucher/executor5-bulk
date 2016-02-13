// ---------------------------------------------------------------------
// typedefs, DEFINEs, helper routines, exception routines

// CSELF struct
typedef struct MPZ
{
  mpz_t  z; // MPZ data
} mpz;

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

#ifdef _M_X64
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
#define MPZ_VERSION             "0.0.4.20160113"



/*
TO DO: does each of those inline overloads need a 'return NULLOBJECT'?
*/

// ---------------------------------------------------------------------
// helper function for message insertion of a z argument
// returns 'z' as a string, if it's not too large
// returns first few digits of 'z' with ".. (nn digits)" appended, if 'z' is very large
inline RexxObjectPtr shortenedZ(RexxMethodContext *c, mpz_t z)
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
inline RexxObjectPtr notValidNumberInBaseException(RexxMethodContext *c, const char *arg, RexxObjectPtr number, MPZ_int base)
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
inline RexxObjectPtr notValidBaseException(RexxMethodContext *c, const char *arg, const char *zero, MPZ_int base)
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
inline RexxObjectPtr noRootExistsException(RexxMethodContext *c, const char *arg, const char *n, MPZ_long_int actual)
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
inline RexxObjectPtr mustNotBeZeroException(RexxMethodContext *c, const char *arg)
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
inline RexxObjectPtr negativeExponentException(RexxMethodContext *c, mpz_t exp)
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
inline RexxObjectPtr tooManyArgumentsException(RexxMethodContext *c, MPZ_long_int number)
{
  c->RaiseException1(Rexx_Error_Incorrect_method_maxarg, c->OBJ_long_int(number));
  return NULLOBJECT;
}


// ---------------------------------------------------------------------
// raises 93.905
// Method argument &1 must be a whole number; found "&2".
// e. g. Error 93.905:  Method argument d must be a whole number; found "X".
inline RexxObjectPtr invalidWholeNumberException(RexxMethodContext *c, const char *arg, RexxObjectPtr actual)
{
  c->RaiseException2(Rexx_Error_Incorrect_method_whole,
                     c->String(arg), actual);
  return NULLOBJECT;
}


// ---------------------------------------------------------------------
// raises 93.906
// Method argument &1 must be zero or a positive whole number; found "&2".
// e. g. Error 93.906:  Method argument op must be zero or a positive whole number; found "-1".
inline RexxObjectPtr notZeroOrPositiveException(RexxMethodContext *c, const char *arg, RexxObjectPtr actual)
{
  c->RaiseException2(Rexx_Error_Incorrect_method_nonnegative, c->String(arg), actual);
  return NULLOBJECT;
}

inline RexxObjectPtr notZeroOrPositiveException(RexxMethodContext *c, const char *arg, CSTRING actual)
{
  notZeroOrPositiveException(c, arg, c->String(actual));
  return NULLOBJECT;
}

inline RexxObjectPtr notZeroOrPositiveException(RexxMethodContext *c, const char *arg, MPZ_long_int actual)
{
  notZeroOrPositiveException(c, arg, c->OBJ_long_int(actual));
  return NULLOBJECT;
}

inline RexxObjectPtr notZeroOrPositiveException(RexxMethodContext *c, const char *arg, mpz_t actual)
{
  notZeroOrPositiveException(c, arg, shortenedZ(c, actual));
  return NULLOBJECT;
}


// ---------------------------------------------------------------------
// raises 93.907
// Method argument &1 must be a positive whole number; found "&2".
// e. g. Error 93.907:  Method argument n must be a positive whole number; found "0".
inline RexxObjectPtr notPositiveException(RexxMethodContext *c, const char *arg, RexxObjectPtr actual)
{
  c->RaiseException2(Rexx_Error_Incorrect_method_positive, c->String(arg), actual);
  return NULLOBJECT;
}

inline RexxObjectPtr notPositiveException(RexxMethodContext *c, const char *arg, CSTRING actual)
{
  notPositiveException(c, arg, c->String(actual));
  return NULLOBJECT;
}

inline RexxObjectPtr notPositiveException(RexxMethodContext *c, const char *arg, MPZ_long_int actual)
{
  notPositiveException(c, arg, c->OBJ_long_int(actual));
  return NULLOBJECT;
}

inline RexxObjectPtr notPositiveException(RexxMethodContext *c, const char *arg, mpz_t actual)
{
  notPositiveException(c, arg, shortenedZ(c, actual));
  return NULLOBJECT;
}


// ---------------------------------------------------------------------
// raises 93.914
// Method argument &1 must be one of &2; found "&3".
// e. g. Error 93.914:  Method argument round must be one of CFT; found "X".
inline RexxObjectPtr wrongArgValueException(RexxMethodContext *c, const char *arg, const char *list, RexxObjectPtr actual)
{
  c->RaiseException(Rexx_Error_Incorrect_method_list,
                    c->ArrayOfThree(c->String(arg), c->String(list), actual));
  return NULLOBJECT;
}

inline RexxObjectPtr wrongArgValueException(RexxMethodContext *c, const char *arg, const char *list, CSTRING actual)
{
  wrongArgValueException(c, arg, list, c->String(actual));
  return NULLOBJECT;
}


// ---------------------------------------------------------------------
// raises 93.948
// Method argument &1 must be of the &2 class.
// e. g. Error 93.948:  Method argument op2 must be of the Z class.
inline RexxObjectPtr wrongArgClassException(RexxMethodContext *c, const char *arg, RexxObjectPtr cls)
{
  c->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                     c->String(arg), c->SendMessage0(cls, "ID"));
  return NULLOBJECT;
}


// ---------------------------------------------------------------------
// raises 42.003
// Arithmetic overflow; divisor must not be zero.
// e. g. Error 42.3:  Arithmetic overflow; divisor must not be zero.
inline RexxObjectPtr divideByZeroException(RexxMethodContext *c)
{
  c->RaiseException0(Rexx_Error_Overflow_zero);
  return NULLOBJECT;
}
