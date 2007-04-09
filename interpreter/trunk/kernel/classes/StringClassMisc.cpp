/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 1995, 2004 IBM Corporation. All rights reserved.             */
/* Copyright (c) 2005-2006 Rexx Language Association. All rights reserved.    */
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
/******************************************************************************/
/* REXX Kernel                                                  okbmisc.c     */
/*                                                                            */
/* Miscellaneous REXX string methods                                          */
/*                                                                            */
/******************************************************************************/

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "RexxCore.h"
#include "StringClass.hpp"
#include "ASCIIDBCSStrings.hpp"
#include "ArgumentUtilities.hpp"                     /* General purpose BIF Header file   */
#include "SourceFile.hpp"
#include "Interpreter.hpp"
#include "SysInterpreter.hpp"


stringchar_t * Memcpbrk(
  stringchar_t *    String,            /* search string                     */
  stringchar_t *    Set,               /* reference set                     */
  stringsize_t   Length )              /* size of string                    */
/*********************************************************************/
/*  Function:  Find first occurence of set nonmember in block        */
/*********************************************************************/
{
  stringchar_t *    Retval;                     /* returned value                    */

  Retval = NULL;                       /* nothing found yet                 */
  while (Length--) {                   /* search through string             */
                                       /* find a match in ref set?          */
    if (*String == '\0' || !strchr((char *)Set, *String)) {
      Retval = String;                 /* copy position                     */
      break;                           /* quit the loop                     */
    }
    String++;                          /* step the pointer                  */
  }
  return Retval;                       /* return matched position           */
}

bool valSet(
  stringchar_t *    String,            /* string to validate                */
  stringsize_t    Length,              /* string length                     */
  stringchar_t *     Set,              /* character set                     */
  int       Modulus,                   /* smallest group size               */
  stringsize_t   *PackedSize )         /* total packed size                 */
/*********************************************************************/
/*  Function:               Validate blocks in string                */
/*                                                                   */
/*                          A string is considered valid if consists */
/*                          of zero or more characters belonging to  */
/*                          the null-terminated C string set in      */
/*                          groups of size modulus.  The first group */
/*                          may have fewer than modulus characters.  */
/*                          The groups are optionally separated by   */
/*                          one or more blanks.                      */
/*********************************************************************/
{
  stringchar_t    c;                   /* current character                 */
  stringsize_t   Count;                /* # set members found               */
  stringchar_t *   Current;            /* current location                  */
  int      SpaceFound;                 /* space found yet?                  */
  stringsize_t   Residue;              /* if space_found, # set members     */
  bool     rc;                         /* return code                       */

  rc = false;                          /* default to failure                */
  if (*String != ' ') {                /* if no leading blank               */
    SpaceFound = 0;                    /* set initial space flag            */
    Count = 0;                         /* start count with zero             */
    Current = (stringchar_t *)String;  /* point to start                    */

    rc = true;                         /* default to good now               */
    for (; Length; Length--) {         /* process entire string             */
      c = *Current++;                  /* get char and step pointer         */
                                       /* if c in set                       */
      if (c != '\0' && strchr((char *)Set, c) != NULL)
        Count++;                       /* bump count                        */
      else {
        if (c == ' ') {                /* if c blank                        */
          if (!SpaceFound) {           /* if 1st blank                      */
                                       /* save position                     */
            Residue = (Count % Modulus);
            SpaceFound = 1;            /* we have the first space           */
          }
                                       /* else if bad position              */
          else if (Residue != (Count % Modulus)) {
            rc = false;                /* this is an error                  */
            break;                     /* report error                      */
          }
        }
        else {
          rc = false;                  /* this is an error                  */
          break;                       /* report error                      */
        }
      }
    }
    if (rc) {                          /* still good?                       */
      if (c == ' ')                    /* if trailing blank                 */
        rc = false;                    /* report error                      */
      else if (SpaceFound && (Count % Modulus) != Residue)
        rc = false;                    /* grouping problem                  */
      else
        *PackedSize = Count;           /* return count of chars             */
    }
  }
  return rc;                           /* return success/failure            */
}

int RexxString::isSymbol()
/*********************************************************************/
/*                                                                   */
/*   Function:         determines valid rexx symbols and returns     */
/*                     a type indicator for valid symbols that can   */
/*                     be passed on to the dictionary routines.      */
/*                                                                   */
/*********************************************************************/
{
  stringchar_t *     Scan;             /* string scan pointer               */
  stringsize_t     Compound;           /* count of periods                  */
  stringsize_t     i;                  /* loop counter                      */
  stringchar_t *     Linend;           /* end of line                       */
  int        Type;                     /* return type                       */

                                       /* name too long                     */
                                       /* or too short                      */
  if (this->getLength() > MAX_SYMBOL_LENGTH || this->getLength() == 0)
    return STRING_BAD_VARIABLE;        /* set a bad type                    */

                                       /* step to end                       */
  Linend = (stringchar_t *)this->getStringData() + this->getLength();

  Compound = 0;                        /* set compound name is no           */
  Scan = (stringchar_t *)this->getStringData();     /* save start position               */
                                       /* while still part of symbol        */
  while (Scan < Linend && RexxSource::isSymbol(*Scan)) {

    if (*Scan == '.')                  /* a dot found..                     */
      Compound++;                      /* indicate a compound var           */

    Scan++;                            /* step the pointer                  */
  }                                    /* len of symbol                     */
                                       /* now check for exponent            */
  if (((Scan + 1) < Linend) &&
      (*Scan == '-' || *Scan == '+') &&
      (isdigit(this->getStringData()[0]) || *Scan == '.') &&
      (toupper(*(Scan - 1)) == 'E')) {
    Scan++;                            /* step to next                      */

    while (Scan < Linend) {            /* while more characters             */
      if (!isdigit(*Scan))             /* if not a digit                    */
        return STRING_BAD_VARIABLE;    /* this isn't valid                  */
      Scan++;                          /* step to next char                 */
    }
  }
  if (Scan < Linend)                   /* use the entire string?            */
    return STRING_BAD_VARIABLE;        /* no, can't be good                 */
                                       /* now determine symbol type         */
                                       /* possible number?                  */
  if (this->getStringData()[0] == '.' || isdigit(this->getStringData()[0])) {

                                       /* only a period?                    */
    if (Compound == 1 && this->getLength() == 1)
      Type = STRING_LITERAL_DOT;       /* yes, set the token type           */
    else if (Compound > 1)             /* too many periods?                 */
      Type = STRING_LITERAL;           /* yes, just a literal token         */
    else {                             /* check for a real number           */
      Type = STRING_NUMERIC;           /* assume numeric for now            */
      Scan = (stringchar_t *)this->getStringData(); /* point to symbol                   */
                                       /* scan symbol, validating           */
      for (i = this->getLength() ; i; i-- ) {
        if (!isdigit(*Scan) &&         /* if not a digit and                */
            *Scan != '.')              /* and not a period...               */
          break;                       /* finished                          */
        Scan++;                        /* step to next character            */
      }
      if (i > 1 &&                     /* if tripped over an 'E'            */
          toupper(*Scan) == 'E') {     /* could be exponential              */
        Scan++;                        /* step past E                       */
        i--;                           /* count the character               */
                                       /* +/- case already validated        */
        if (*Scan != '+' && *Scan != '-') {
          for(; i; i--) {              /* scan rest of symbol               */
            if (!isdigit(*Scan)) {     /* if not a digit...                 */
              Type = STRING_LITERAL;   /* not a number                      */
              break;
            }
            Scan++;                    /* step to next character            */
          }
        }
      }
      else if (i)                      /* literal if stuff left             */
        Type = STRING_LITERAL;         /* yes, just a literal token         */
    }
  }

  else if (!Compound) {                /* not a compound so...              */
    Type = STRING_NAME;                /* set the token type                */
  }
                                       /* is it a stem?                     */
  else if (Compound == 1 && *(Scan - 1) == '.')
    Type = STRING_STEM;                /* yes, set the token type           */
  else
    Type = STRING_COMPOUND_NAME;       /* otherwise just plain              */
                                       /* compound                          */
  return Type;                         /* return the type info              */
}

RexxObject *DataType(
     RexxString *String,               /* string value                      */
     stringchar_t       Option )       /* requested option                  */
/*********************************************************************/
/* Function:  Perform primitive datatype validation                  */
/*********************************************************************/
{
  stringsize_t      Len;               /* validated string length           */
  RexxObject *Answer;                  /* validation result                 */
  RexxObject *Temp;                    /* temporary value                   */
  stringchar_t *       Scanp;          /* string data pointer               */
  stringsize_t      Count;             /* hex nibble count                  */
  int         Type;                    /* validated symbol type             */
  RexxNumberString *TempNum;

  Len = String->getLength();                /* get validated string len          */
  Option = toupper(Option);            /* get the first character           */

                                       /* assume failure on checking        */
  Answer = (RexxObject *)TheFalseObject;
                                       /* get a scan pointer                */
  Scanp = (stringchar_t *)String->getStringData();

  switch (Option) {                    /* based on type to confirm          */

    case DATATYPE_ALPHANUMERIC:        /* Alphanumeric                      */
                                       /* all in the set?                   */
      if (Len != 0 && !Memcpbrk(Scanp, ALPHANUM, Len))
                                       /* this is a good string             */
        Answer = (RexxObject *)TheTrueObject;
      break;

    case DATATYPE_BINARY:              /* Binary string                     */
                                       /* validate the string               */
      if (Len == 0 || valSet(Scanp, Len, (stringchar_t *)BINARI, 4, &Count))
                                       /* this is a good string             */
        Answer = (RexxObject *)TheTrueObject;
      break;

    case DATATYPE_MIXED_DBCS:          /* Mixed SBCS/DBCS string            */
                                       /* pass on to the DBCS function      */
      Answer = String->DBCSdatatype(DATATYPE_MIXED_DBCS);
      break;

    case DATATYPE_PURE_DBCS:           /* Pure DBCS string                  */
                                       /* pass on to the DBCS function      */
      Answer = String->DBCSdatatype(DATATYPE_PURE_DBCS);
      break;

    case DATATYPE_LOWERCASE:           /* Lowercase                         */
      if (Len != 0 && !Memcpbrk(Scanp, LOWER_ALPHA, Len))
                                       /* this is a good string             */
        Answer = (RexxObject *)TheTrueObject;
      break;

    case DATATYPE_UPPERCASE:           /* Uppercase                         */
      if (Len != 0 && !Memcpbrk(Scanp, UPPER_ALPHA, Len))
                                       /* this is a good string             */
        Answer = (RexxObject *)TheTrueObject;
      break;

    case DATATYPE_MIXEDCASE:           /* Mixed case                        */
      if (Len != 0 && !Memcpbrk(Scanp, MIXED_ALPHA, Len))
                                       /* this is a good string             */
        Answer = (RexxObject *)TheTrueObject;
      break;

    case DATATYPE_WHOLE_NUMBER:        /* Whole number                      */
                                       /* validate as a number              */
      TempNum = String->numberString();
      if (TempNum != OREF_NULL) {      /* valid number?                     */
                                       /* force rounding to current digits  */
        TempNum = (RexxNumberString *)TempNum->plus(IntegerZero);
                                       /* check for integer then            */
        Answer = TempNum->isInteger();
      }
      break;

    case DATATYPE_NUMBER:              /* Number                            */
                                       /* validate as a number              */
      Temp = (RexxObject *)String->numberString();
      if (Temp != OREF_NULL)           /* valid number?                     */
                                       /* got a good one                    */
        Answer = (RexxObject *)TheTrueObject;
      break;

      case DATATYPE_9DIGITS:             /* NUMERIC DIGITS 9 number           */
      {
          wholenumber_t temp;
                                           /* good long number                  */
          if (String->numberValue(&temp, Numerics::DEFAULT_DIGITS))
          {
                                           /* say it's good                     */
            Answer = (RexxObject *)TheTrueObject;
          }
          break;
      }


    case DATATYPE_HEX:                 /* heXadecimal                       */
                                       /* validate the string               */
      if (Len == 0 || valSet(Scanp, Len, (stringchar_t *)HEX_CHAR_STR, 2, &Count))
                                       /* valid hexadecimal                 */
        Answer = (RexxObject *)TheTrueObject;
      break;

    case DATATYPE_SYMBOL:              /* Symbol                            */
                                       /* validate the symbol               */
      if (String->isSymbol() != STRING_BAD_VARIABLE)
                                       /* is a valid symbol                 */
        Answer = (RexxObject *)TheTrueObject;
      break;

    case DATATYPE_VARIABLE:            /* Variable                          */

                                       /* validate the symbol               */
      Type = String->isSymbol();
                                       /* a valid variable type?            */
      if (Type == STRING_NAME ||
          Type == STRING_STEM ||
          Type == STRING_COMPOUND_NAME)
                                       /* is a valid symbol                 */
        Answer = (RexxObject *)TheTrueObject;
      break;

      case DATATYPE_LOGICAL:           // Test for a valid logical.
          if (Len != 1 || (*Scanp != '1' && *Scanp != '0'))
          {
              Answer = TheFalseObject;
          }
          else
          {
              Answer = TheTrueObject;
          }

          break;

    default  :                         /* unsupported option                */
      reportException(Error_Incorrect_method_option, (stringchar_t *)"ABCDLMNOSUVWX9", new_string((stringchar_t *)&Option,1));
  }
  return Answer;                       /* return validation answer          */
}

RexxInteger *RexxString::abbrev(
    RexxString *info,                  /* target compared value             */
    RexxInteger *length)               /* minimum length                    */
/******************************************************************************/
/*  Function:  ABBREV string method                                           */
/******************************************************************************/
{
  stringsize_t   Len1;                 /* length of string1                 */
  stringsize_t   Len2;                 /* length of string1                 */
  stringsize_t   ChkLen;               /* required check length             */
  int      rc;                         /* compare result                    */

  if (DBCS_MODE)                       /* need to use DBCS?                 */
                                       /* do the DBCS version               */
    return this->DBCSabbrev(info, length);

  info = stringArgument(info, ARG_ONE);    /* process the information string    */
  Len2 = info->getLength();                 /* get the length also               */
                                       /* get the optional check length     */
                                       /* get the optional check length     */
  ChkLen = optionalLengthArgument(length, Len2, ARG_TWO);
  Len1 = this->getLength();                 /* get this length                   */

  if (ChkLen == 0 && Len2 == 0)        /* if null string match              */
    rc = 1;                            /* then we have an abbrev            */
  else if (Len1 == 0 ||                /* len 1 is zero,                    */
      (Len2 < ChkLen) ||               /* or second is too short            */
      (Len1 < Len2))                   /* or second is too long             */
    rc = 0;                            /* not an abbreviation               */

  else                                 /* now we have to check it           */
                                       /* do the comparison                 */
    rc = !(memcmp((stringchar_t *)this->getStringData(), info->getStringData(), Len2));
                                       /* return proper string value        */
  return (rc) ? IntegerOne : IntegerZero;
}

RexxInteger *RexxString::compare(
    RexxString *string2,               /* other string to compare against   */
    RexxString *pad)                   /* optional padding character        */
/******************************************************************************/
/*  Function:  String class COMPARE method/function.                          */
/******************************************************************************/
{
  stringchar_t    PadChar;                    /* pad character                     */
  stringsize_t   MisMatch;                   /* mismatch location                 */
  RexxInteger *Retval;                 /* returned result                   */
  stringchar_t *    String1;                    /* string 1 pointer                  */
  stringchar_t *    String2;                    /* string 2 pointer                  */
  stringsize_t   Lead;                       /* leading length                    */
  stringsize_t   Remainder;                  /* trailing length                   */
  stringsize_t   i;                          /* loop index                        */
  stringsize_t   Length1;                    /* first string length               */
  stringsize_t   Length2;                    /* second string length              */

  if (DBCS_MODE)                       /* need to use DBCS?                 */
                                       /* do the DBCS version               */
    return this->DBCScompare(string2, pad);

  Length1 = this->getLength();              /* get this strings length           */
                                       /* validate the compare string       */
  string2 = stringArgument(string2, ARG_ONE);
  Length2 = string2->getLength();           /* get the length also               */
  PadChar = padArgument(pad, ' ', ARG_TWO);/* get the pad character             */
  if (Length1 > Length2) {             /* first longer?                     */
    String1 = (stringchar_t *)this->getStringData();     /* make arg 1 first string           */
                                       /* arg 2 is second string            */
    String2 = string2->getStringData();
    Lead = Length2;                    /* get shorter length                */
    Remainder = Length1 - Lead;        /* get trailing size                 */
  }
  else {                               /* make arg 2 first string           */
    String1 = string2->getStringData();
    String2 = (stringchar_t *)this->getStringData(); /* arg 1 is second string            */
    Lead = Length1;                    /* get shorter length                */
    Remainder = Length2 - Lead;        /* get trailing size                 */
  }
  MisMatch = 0;                        /* assume they are equal             */
  i = 0;                               /* set the start                     */
  while (i < Lead) {                   /* if have leading compare           */
    if (String1[i] != String2[i]) {    /* not the same?                     */
      MisMatch = i + 1;                /* save position, origin one         */
      break;                           /* exit the loop                     */
    }
    i++;                               /* step the index                    */
  }
  if (!MisMatch && Remainder) {        /* need to handle padding?           */
    String1 += Lead;                   /* step to remainder                 */
    for (i = 0; i < Remainder; i++) {  /* scan the remainder                */
      if (String1[i] != PadChar) {     /* pad mismatch?                     */
        MisMatch = Lead + i + 1;       /* get mismatch position             */
        break;                         /* finished                          */
      }
    }
  }
  if (!MisMatch)
    Retval = IntegerZero;              /* this is zero                      */
  else
    Retval = new_integer(MisMatch);    /* make an integer return value      */
  return Retval;                       /* return result string              */
}

RexxString *RexxString::copies(RexxInteger *copies)
/******************************************************************************/
/* Function:  String class COPIES method/function                             */
/******************************************************************************/
{
  stringsize_t   Count;                      /* copies count                      */
  RexxString *Retval;                  /* return value                      */
  stringsize_t   Len;                        /* copy string length                */
  stringchar_t *    Temp;                       /* copy location                     */

  if (DBCS_SELF)                       /* need to use DBCS?                 */
    ValidDBCS(this);                   /* validate the DBCS string          */
  requiredArg(copies, ONE);           /* the count is required             */
                                       /* get the copies count              */
  Count = copies->requiredNonNegative(ARG_ONE);
  Len = this->getLength();                  /* get argument length               */

  if (Count == 0 ||                    /* no copies requested?              */
      Len == 0 )                       /* or copying a null string          */
    Retval = OREF_NULLSTRING;          /* just a null string                */
  else {                               /* get storage size                  */
                                       /* allocate storage needed           */
                                       /* allocate storage needed           */
    Retval = (RexxString *)raw_string(Len * Count);

    if (Len == 1) {                    /* if only 1 char long               */
                                       /* just do this with memset          */
      memset(Retval->getStringData(), this->getStringData()[0], Count);
    }
                                       /* if any copies                     */
    else {
                                       /* point to the string               */
      Temp = Retval->getStringData();
      while (Count--) {                /* copy 2 thru n copies              */
                                       /* copy the string                   */
        memcpy(Temp, (stringchar_t *)this->getStringData(), Len);
        Temp += Len;
      }
    }
  }
  return Retval;                       /* return copied string              */
}

RexxObject *RexxString::dataType(RexxString *ptype)
/******************************************************************************/
/* Function:  String class DATATYPE method/function                           */
/******************************************************************************/
{
 stringchar_t type;

 if (ptype != OREF_NULL) {             /* see if type was specified?        */
                                       /* yes, specified, get 1st char      */
  type = optionArgument(ptype, 0, ARG_ONE);
                                       /* and call datatype routine to      */
  return DataType(this, type);         /* determine if its type specified.  */
 }
                                       /* type not specified, see if its a  */
                                       /* valid number                      */
 return (DataType(this, 'N') == TheTrueObject
         ? new_string((stringchar_t *)"NUM",3)         /* if so we return NUM               */
         : new_string((stringchar_t *)"CHAR",4));      /* otherwise we return CHAR          */
}


RexxInteger *RexxString::lastPosRexx(RexxString  *needle, RexxInteger *start)
{
    // if DBCS mode is turned on...pass it on.
    if (DBCS_MODE)
    {
        return this->DBCSlastPos(needle, start);
    }
    needle = REQUIRED_STRING(needle, ARG_ONE);
    // find out where to start the search. The default is at the very end.
    stringsize_t startPos = optional_position(start, getLength(), ARG_TWO);
    // now perform the actual search.
    return new_integer(lastPos(needle, startPos));
}


RexxInteger *RexxString::caselessLastPosRexx(RexxString  *needle, RexxInteger *start)
{
    // validate that this is a good string argument
    needle = REQUIRED_STRING(needle, ARG_ONE);
    // find out where to start the search. The default is at the very end.
    stringsize_t startPos = optional_position(start, getLength(), ARG_TWO);
    // now perform the actual search.
    return new_integer(caselessLastPos(needle, startPos));
}



/**
 * Primitive implementation of a lastpos search.
 *
 * @param needle The search needle.
 * @param start  The starting position (origin 1).
 *
 * @return Returns the last match position, searching back from the start
 *         position.  The starting position is the right-most character
 *         of the past possible match (as if the string was truncated
 *         at start).
 */
stringsize_t RexxString::lastPos(RexxString  *needle, stringsize_t start)
{
    stringsize_t haystackLen = this->getLength();          /* get the haystack length           */
    stringsize_t needleLen = needle->getLength();          /* and get the length too            */

    // no match possible if either string is null
    if (needleLen == 0 || haystackLen == 0)
    {
        return 0;
    }
    else
    {
        // get the start position for the search.
        start = min(start, haystackLen);
                                         /* do the search                     */
        stringchar_t * matchLocation = lastPos((stringchar_t *)needle->getStringData(), needleLen, (stringchar_t * )this->getStringData(), haystackLen);
        if (matchLocation == NULL)
        {
            return 0;
        }
        else
        {
            return matchLocation - (stringchar_t *)this->getStringData() + 1;
        }
    }
}


/**
 * Primitive implementation of a caseless lastpos search.
 *
 * @param needle The search needle.
 * @param start  The starting position (origin 1).
 *
 * @return Returns the last match position, searching back from the start
 *         position.  The starting position is the right-most character
 *         of the past possible match (as if the string was truncated
 *         at start).
 */
stringsize_t RexxString::caselessLastPos(RexxString *needle, stringsize_t start)
{
    stringsize_t haystackLen = this->getLength();          /* get the haystack length           */
    stringsize_t needleLen = needle->getLength();          /* and get the length too            */

    // no match possible if either string is null
    if (needleLen == 0 || haystackLen == 0)
    {
        return 0;
    }
    else
    {
        // get the start position for the search.
        start = min(start, haystackLen);
                                         /* do the search                     */
        stringchar_t * matchLocation = caselessLastPos((stringchar_t *)needle->getStringData(), needleLen, (stringchar_t * )this->getStringData(), haystackLen);
        if (matchLocation == NULL)
        {
            return 0;
        }
        else
        {
            return matchLocation - (stringchar_t *)this->getStringData() + 1;
        }
    }
}


/**
 * Absolutely most primitive version of a lastpos search.  This
 * version searches directly in a buffer rather than a Rexx
 * String.
 *
 * @param needle    Pointer to the needle string.
 * @param needleLen Length of the needle string.
 * @param haystack  The pointer to the haystack string.
 * @param haystackLen
 *                  The length of the haystack string.
 *
 * @return A pointer to the match location or NULL if there is no match.
 */
stringchar_t * RexxString::lastPos(stringchar_t * needle, stringsize_t needleLen, stringchar_t *  haystack, stringsize_t haystackLen)
{
    // if the needle's longer than the haystack, no chance of a match
    if (needleLen > haystackLen)
    {
        return NULL;
    }
    // set the search startpoing point relative to the end of the search string
    haystack = haystack + haystackLen - needleLen;
    // this is the possible number of compares we might need to perform
    stringsize_t count = haystackLen - needleLen + 1;
    // now scan backward
    while (count > 0)
    {
        // got a match at this position, return it directly
        if (memcmp(haystack, needle, needleLen) == 0)
        {
            return haystack;
        }
        // decrement count and position
        count--;
        haystack--;
    }
    return NULL;   // nothing to see here folks, move along
}


/**
 * Absolutely most primitive version of a caseless lastpos
 * search. This version searches directly in a buffer rather
 * than a Rexx String.
 *
 * @param needle    Pointer to the needle string.
 * @param needleLen Length of the needle string.
 * @param haystack  The pointer to the haystack string.
 * @param haystackLen
 *                  The length of the haystack string.
 *
 * @return A pointer to the match location or NULL if there is no match.
 */
stringchar_t * RexxString::caselessLastPos(stringchar_t * needle, stringsize_t needleLen, stringchar_t *  haystack, stringsize_t haystackLen)
{
    // if the needle's longer than the haystack, no chance of a match
    if (needleLen > haystackLen)
    {
        return NULL;
    }
    // set the search startpoing point relative to the end of the search string
    haystack = haystack + haystackLen - needleLen;
    // this is the possible number of compares we might need to perform
    stringsize_t count = haystackLen - needleLen + 1;
    // now scan backward
    while (count > 0)
    {
        // got a match at this position, return it directly
        if (CaselessCompare(haystack, needle, needleLen) == 0)
        {
            return haystack;
        }
        // decrement count and position
        count--;
        haystack--;
    }
    return NULL;   // nothing to see here folks, move along
}

stringsize_t RexxString::countStr(RexxString *needle)
/******************************************************************************/
/* Function:  Count occurrences of one string in another.                     */
/******************************************************************************/
{
  stringsize_t count;                        /* count of strings                  */
  stringsize_t match;                        /* last match position               */
  stringsize_t needlelength ;                /* length of the needle              */

  count = 0;                           /* no matches yet                    */
                                       /* get the length of the needle      */
  needlelength = needle->getLength();
                                       /* get the first match position      */
  match = this->pos(needle, 0);
  while (match != 0) {                 /* while we're getting matches       */
    count = count + 1;                 /* count this match                  */
                                       /* do the next search                */
    match = this->pos(needle, match + needlelength - 1);
  }
  return count;                        /* return the match count            */
}

RexxInteger *RexxString::countStrRexx(RexxString *needle)
/******************************************************************************/
/* Function:  Count occurrences of one string in another.                     */
/******************************************************************************/
{
  stringsize_t count;                        /* count of strings                  */

                                       /* force needle to a string          */
  needle = REQUIRED_STRING(needle, ARG_ONE);
  count = this->countStr(needle);      /* do the counting                   */
  return new_integer(count);           /* return the count as an object     */
}

RexxString *RexxString::changeStr(RexxString *needle,
                                  RexxString *newNeedle)
/******************************************************************************/
/* Function:  Change strings into another string.                             */
/******************************************************************************/
{
  stringsize_t start;                        /* converted start position          */
  stringsize_t match;                        /* last match position               */
  stringsize_t needleLength;                 /* length of the needle              */
  stringsize_t newLength;                    /* length of the replacement string  */
  stringsize_t matches;                      /* number of replacements            */
  stringsize_t copyLength;                   /* length to copy                    */
  stringchar_t * source;                        /* point to the string source        */
  stringchar_t * copy;                          /* current copy position             */
  stringchar_t * newPtr;                        /* pointer to replacement data       */
  RexxString *result;                  /* returned result string            */
                                       /* force needle to a string          */
  needle = REQUIRED_STRING(needle, ARG_ONE);
                                       /* newneedle must be a string two    */
  newNeedle = REQUIRED_STRING(newNeedle, ARG_TWO);
  matches = this->countStr(needle);    /* find the number of replacements   */
  needleLength = needle->getLength();       /* get the length of the needle      */
  newLength = newNeedle->getLength();       /* and the replacement length        */
                                       /* get a proper sized string         */
  result = (RexxString *)raw_string(this->getLength() - (matches * needleLength) + (matches * newLength));
  copy = result->getStringData();           /* point to the copy location        */
  source = this->getStringData();           /* and out own data                  */
                                       /* and the string to replace         */
  newPtr = newNeedle->getStringData();
  start = 0;                           /* set a zero starting point         */
  for (;;) {                           /* loop forever                      */
    match = this->pos(needle, start);  /* look for the next occurrence      */
    if (match == 0)                    /* not found?                        */
      break;                           /* get out of here                   */
    copyLength = (match - 1) - start;  /* get the next length to copy       */
    if (copyLength != 0) {             /* something to copy?                */
                                       /* add on the next string section    */
      memcpy(copy, source + start, copyLength);
      copy += copyLength;              /* step over the copied part         */
    }
    if (newLength != 0) {              /* something to replace with?        */
      memcpy(copy, newPtr, newLength); /* copy over the new segment         */
      copy += newLength;               /* and step it over also             */
    }
    start = match + needleLength - 1;  /* step to the next position         */
  }
  if (start < this->getLength())            /* some remainder left?              */
                                       /* add it on                         */
    memcpy(copy, source + start, this->getLength() - start);
  return result;                       /* finished                          */
}


/**
 * Extract the substring that occurs before a needle match postion.
 *
 * @param needle The search needle.  If this is a null string, it will match to the end.
 *
 * @return The substring before the match needle match position.  If the
 *         needle is not found, the entire string is returned.  Like the
 *         parse instruction, a null string matches to the end.
 */
RexxString *RexxString::before(RexxString  *needle, RexxInteger *start)
{

    // the needle must be a string value
    needle = REQUIRED_STRING(needle, ARG_ONE);

    stringsize_t searchStart = optionalPositionArgument(start, 1, ARG_TWO);

    // if this a null string, it matches everything.
    if (needle->getLength() == 0)
    {
        if (searchStart == 1)
        {
            return this;
        }
        else {
            return this->extract(searchStart - 1, getLength() - (searchStart - 1));
        }
    }

    // look for the first match
    stringsize_t match = this->pos(needle, searchStart - 1);
    // no match, we return the entire string
    if (match == 0)
    {
        if (searchStart == 1)
        {
            return this;
        }
        else {
            return this->extract(searchStart - 1, getLength() - (searchStart - 1));
        }
    }
    else
    {
        // just extract the substring
        return this->extract(searchStart - 1, match - searchStart);
    }
}



/**
 * Extract the substring that occurs before a last needle match
 * postion.
 *
 * @param needle The search needle.  If this is a null string,
 *               it will match to the beginning.
 *
 * @return The substring before the match needle match position.  If the
 *         needle is not found, the entire string is returned.  Like the
 *         parse instruction, a null string matches to the end.
 */
RexxString *RexxString::beforeLast(RexxString  *needle)
{

    // the needle must be a string value
    needle = REQUIRED_STRING(needle, ARG_ONE);

    // if this a null string, it matches everything.
    if (needle->getLength() == 0)
    {
        return this;
    }
    // look for the first match
    stringsize_t match = this->pos(needle, 0);
    // no match, we return the entire string
    if (match == 0)
    {
        return this;
    }
    else
    {
        // just extract the substring
        return this->extract(0, match - 1);
    }
}




/**
 * Extract the substring that occurs after a needle match
 * postion.
 *
 * @param needle The search needle.  If this is a null string, it will match to the end.
 *
 * @return The substring before the match needle match position.  If the
 *         needle is not found, a null string is returned. Like
 *         the parse instruction, a null string matches to the
 *         end.
 */
RexxString *RexxString::after(RexxString  *needle, RexxInteger *start)
{
    // the needle must be a string value
    needle = REQUIRED_STRING(needle, ARG_ONE);

    stringsize_t searchStart = optionalPositionArgument(start, 0, ARG_TWO);

    // if this a null string, it matches to the end, so there's no
    // "after life"
    if (needle->getLength() == 0)
    {
        return OREF_NULLSTRING;
    }

    // look for the first match
    stringsize_t match = this->pos(needle, searchStart - 1);
    // no match, we return a null string
    if (match == 0)
    {
        if (searchStart == 1)
        {
            return this;
        }
        else {
            return this->extract(searchStart - 1, getLength() - searchStart + 1);
        }
    }
    else
    {
        // convert this to a 0-based offset for the extract, positioned after the needle.
        match = match + needle->getLength() - 1;
        stringsize_t length = this->getLength() - match;

        if (length == 0)
        {
            return OREF_NULLSTRING;
        }
        else
        {
            // just extract the substring
            return this->extract(match, length);

        }
    }
}

stringsize_t RexxString::memPos(
  stringchar_t *  String,                      /* search string                     */
  stringsize_t Length,                       /* string length                     */
  stringchar_t  Char )                        /* target character                  */
/*********************************************************************/
/*  Function:  offset of first occurrence of char in string          */
/*********************************************************************/
{
  stringchar_t *  Scan;                        /* scan location                     */
  stringsize_t Position;                     /* matched position                  */

  Position = -1;                       /* default to no match               */
                                       /* while in the string               */
  for (Scan = String; Length; Length--) {
    if (*Scan == Char) {               /* find a match?                     */
      Position = Scan - String;        /* return difference                 */
      break;                           /* quit the loop                     */
    }
    Scan++;                            /* step the position                 */
  }
  return Position;                     /* return match position             */
}

RexxString *RexxString::translate(
    RexxString *tableo,                /* output table                      */
    RexxString *tablei,                /* input table                       */
    RexxString *pad)                   /* pad character                     */
/******************************************************************************/
/*  Function:  String class TRANSLATE method/function                         */
/******************************************************************************/
{
  RexxString *Retval;                  /* return value                      */
  stringchar_t *    OutTable;                  /* output table                      */
  stringsize_t    OutTableLength;            /* length of output table            */
  stringchar_t *    InTable;                   /* input table                       */
  stringchar_t *    ScanPtr;                   /* scanning pointer                  */
  stringsize_t    ScanLength;                /* scanning length                   */
  stringsize_t    InTableLength;             /* length of input table             */
  stringchar_t     PadChar;                   /* pad character                     */
  stringchar_t     ch;                        /* current character                 */
  stringsize_t    Position;                  /* table position                    */

  if (DBCS_MODE)                       /* need to use DBCS?                 */
                                       /* do the DBCS version               */
    return (RexxString *)this->DBCStranslate(tableo, tablei, pad);


                                       /* just a simple uppercase?          */
 if (tableo == OREF_NULL && tablei == OREF_NULL && pad == OREF_NULL)
   return this->upper();               /* return the uppercase version      */
                                       /* validate the tables               */
                                       /* validate the tables               */
 tableo = optionalStringArgument(tableo, OREF_NULLSTRING, ARG_ONE);
 OutTableLength = tableo->getLength();      /* get the table length              */
                                       /* input table too                   */
 tablei = optionalStringArgument(tablei, OREF_NULLSTRING, ARG_TWO);
 InTableLength = tablei->getLength();       /* get the table length              */
 InTable = (stringchar_t *)tablei->getStringData(); /* point at the input table          */
 OutTable = (stringchar_t *)tableo->getStringData();/* and the output table              */
                                       /* get the pad character             */
  PadChar = padArgument(pad, ' ', ARG_THREE);
                                       /* allocate space for answer         */
                                       /* and copy the string               */
 Retval = new_string((stringchar_t *)this->getStringData(), this->getLength());
 ScanPtr = (stringchar_t *)Retval->getStringData(); /* point to data                     */
 ScanLength = this->getLength();            /* get the length too                */

 while (ScanLength--) {                /* spin thru input                   */
   ch = (stringchar_t)*ScanPtr;               /* get a character                   */

   if (tablei != OREF_NULLSTRING)      /* input table specified?            */
                                       /* search for the character          */
     Position = memPos(InTable, InTableLength, ch);
   else
     Position = (stringsize_t)ch;            /* position is the character value   */
   if (Position != -1) {               /* found in the table?               */
     if (Position < OutTableLength)    /* in the output table?              */
                                       /* convert the character             */
       *ScanPtr = *(OutTable + Position);
     else
       *ScanPtr = PadChar;             /* else use the pad character        */
   }
   ScanPtr++;                          /* step the pointer                  */
 }
 return Retval;                        /* return translated string          */
}

RexxInteger *RexxString::verify(
    RexxString  *ref,                  /* compare reference string          */
    RexxString  *option,               /* Match/NoMatch option              */
    RexxInteger *start)                /* optional starg position           */
/******************************************************************************/
/*  Function:  String class VERIFY function                                   */
/******************************************************************************/
{
  stringsize_t    StartPos;                  /* start position                    */
  stringsize_t    StringLen;                 /* length of string                  */
  stringsize_t    Position;                  /* returned position                 */
  stringsize_t    ReferenceLen;              /* length of reference set           */
  stringsize_t    Temp;                      /* temporary scan length             */
  RexxInteger *Retval;                 /* return value                      */
  stringchar_t      Option;                    /* verify option                     */
  stringchar_t *     Reference;                 /* reference pointer                 */
  stringchar_t *     Current;                   /* current scan position             */
  stringchar_t      ch;                        /* scan character                    */
  bool      Match;                     /* found a match                     */

  if (DBCS_MODE)                       /* need to use DBCS?                 */
                                       /* do the DBCS version               */
    return this->DBCSverify(ref, option, start);

  ref = stringArgument(ref, ARG_ONE);      /* get the reference string          */
  ReferenceLen = ref->getLength();          /* get a length also                 */
                                       /* get the option, default 'Nomatch' */
  Option = optionArgument(option, VERIFY_NOMATCH, ARG_TWO);
  if (Option != VERIFY_MATCH &&        /* options are 'Match' and           */
      Option != VERIFY_NOMATCH)        /* 'NoMatch'                         */
                                       /* not that either, then its an error*/
    reportException(Error_Incorrect_method_option, (stringchar_t *)"MN", option);

                                       /* get starting position             */
  StartPos = optionalPositionArgument(start, 1, ARG_THREE);
  StringLen = this->getLength();            /* get the string length             */
  if (StartPos > StringLen)            /* beyond end of string?             */
    Retval = IntegerZero;              /* couldn't find it                  */
  else {
                                       /* point at start position           */
    Current = (stringchar_t *)this->getStringData() + StartPos - 1;
    StringLen -= (StartPos - 1);       /* reduce the length                 */
    Position = 0;                      /* haven't found it yet              */

    if (!ReferenceLen) {               /* if verifying a nullstring         */
      if (Option == VERIFY_MATCH)      /* can't match at all                */
        Retval = IntegerZero;          /* so return zero                    */
      else
        Retval = new_integer(StartPos);/* non-match at start position       */
    }
    else {                             /* need to really search             */
      while (StringLen--) {            /* while input left                  */
        ch = *Current++;               /* get next char                     */
                                       /* get reference string              */
        Reference = ref->getStringData();
        Temp = ReferenceLen;           /* copy the reference length         */
        Match = false;                 /* no match yet                      */

        while (Temp--) {               /* spin thru reference               */
          if (ch == *Reference++) {    /* in reference ?                    */
            Match = true;              /* had a match                       */
            break;                     /* quit the loop                     */
          }
        }
                                       /* have needed matching?             */
        if ((Match && Option == VERIFY_MATCH) ||
            (!Match && Option == VERIFY_NOMATCH)) {
                                       /* calculate the position            */
          Position = Current - (stringchar_t *)this->getStringData();
          break;                       /* done searching                    */
        }
      }
                                       /* format the position               */
      Retval = Position ? new_integer(Position) : IntegerZero;
    }
  }
  return Retval;                       /* return formatted number           */
}


void RexxString::copyIntoRxString(RxString *target)
{
    // get our length
    stringsize_t length = getLength() + 1;
    // not able to copy in directly?
    if (length > target->strlength || target->strptr == NULL)
    {
        target->strptr = (char *)SysInterpreter::allocateResultMemory(length);

    }
                                       /* yes, copy the data (including the */
                                       /* terminating null implied by the   */
                                       /* use of length + 1                 */
    // now copy the data, including a terminating null
    memcpy(target->strptr, getStringData(), length);
    target->strlength = length - 1;
}

