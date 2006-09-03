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
/* REXX Kernel                                                  okbconv.c     */
/*                                                                            */
/* REXX string conversion methods                                             */
/*                                                                            */
/******************************************************************************/
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "RexxCore.h"
#include "StringClass.hpp"
#include "RexxBuffer.hpp"
#include "ArgumentUtilities.hpp"                     /* Gneral purpose BIF Header file    */
#include "Interpreter.hpp"
#include "NumberStringMath.hpp"

/*********************************************************************/
/*                                                                   */
/*      Returns:                The numerical value of the hex       */
/*                              digit (between 0 and 15, inclusive). */
/*                              If the argument is not a valid       */
/*                              hexadecimal digit, then 0 is         */
/*                              returned, but no error is reported.  */
/*                                                                   */
/*********************************************************************/

stringchar_t hexDigitToInt(
  stringchar_t ch)                     /* input hex digit                   */
{
  stringchar_t Retval;                 /* return value                      */

  if (isdigit(ch))                     /* if real digit                     */
    Retval = ch - '0';                 /* convert that                      */
  else
    Retval = toupper(ch) - 'A' + 10;   /* convert alphabetic                */
  return Retval;                       /* return conversion                 */
}

/*********************************************************************/
/*                                                                   */
/*      Returns:                The value of the buffer contents     */
/*                              interpreted as the binary expansion  */
/*                              of a byte, with most significant     */
/*                              bit in s[0] and least significant    */
/*                              bit in s[7].                         */
/*                                                                   */
/*********************************************************************/

stringchar_t packByte(
  stringchar_t * String )              /* string to pack                    */
{
  stringchar_t Result;                 /* returned byte                     */
  int   i;

  Result = 0;                          /* start off at zero                 */
  for (i = 0; i < 8; i++)              /* loop thru 8 chars                 */
    if (String[i] == '1')              /* if 'bit' set                      */
      Result |= ((stringchar_t)1<<((stringchar_t)7-i));/* or with mask                      */
  return Result;                       /* return packed byte                */
}

/*********************************************************************/
/*                                                                   */
/*      Returns:                The value of the buffer contents     */
/*                              interpreted as the binary expansion  */
/*                              of a byte, with most significant     */
/*                              bit in s[0] and least significant    */
/*                              bit in s[7].                         */
/*                                                                   */
/*********************************************************************/

stringchar_t packNibble(
  stringchar_t * String )                       /* string to pack                    */
{
  stringchar_t  Buf[8];                        /* temporary buffer                  */
  int   i;                             /* table index                       */

  memset(Buf, '0', 4);                 /* set first 4 bytes to zero         */
  memcpy(Buf+4, String, 4);            /* copy next 4 bytes                 */
  i = (int)packByte(Buf);              /* pack to a single byte             */
  return HEX_DIGITS[i];                /* convert to a printable character  */
}

/*********************************************************************/
/*                                                                   */
/*  Name:                   PackByte2                                */
/*                                                                   */
/*  Descriptive name:       Pack 2 0123456789ABCDEFabcdef chars into */
/*                          byte                                     */
/*                                                                   */
/*  Returns:                The value of the buffer contents         */
/*                          interpreted as the hex expansion         */
/*                          of a byte, with most significant         */
/*                          nibble in s[0] and least significant     */
/*                          nibble in s[2].                          */
/*                                                                   */
/*********************************************************************/

stringchar_t packByte2(
  stringchar_t *    Byte )             /* location to pack                  */
{
  int      Nibble1;                    /* first nibble                      */
  int      Nibble2;                    /* second nibble                     */

                                       /* convert each digit                */
  Nibble1 = hexDigitToInt(Byte[0]);
  Nibble2 = hexDigitToInt(Byte[1]);
                                       /* combine the two digits            */

  return (stringchar_t)((Nibble1<<4) | Nibble2);
}

/*********************************************************************/
/*                                                                   */
/*  Name:                   validateSet                              */
/*                                                                   */
/*  Descriptive name:       Validate blocks in string                */
/*                                                                   */
/*                          A string is considered valid if consists */
/*                          of zero or more characters belonging to  */
/*                          the null-terminated C string set in      */
/*                          groups of size modulus.  The first group */
/*                          may have fewer than modulus characters.  */
/*                          The groups are optionally separated by   */
/*                          one or more blanks.                      */
/*                                                                   */
/*********************************************************************/

stringsize_t validateSet(
  stringchar_t * String,               /* string to validate                */
  stringsize_t Length,                 /* string length                     */
  stringchar_t *Set,                   /* character set                     */
  int       Modulus,                   /* smallest group size               */
  bool      Hex )                      /* HEX or BIN flag                   */
{
  stringchar_t    c;                   /* current character                 */
  stringsize_t   Count;                /* # set members found               */
  stringchar_t *    Current;           /* current location                  */
  stringchar_t *    SpaceLocation;     /* location of last space            */
  int      SpaceFound;                 /* space found yet?                  */
  stringsize_t   Residue;              /* if space_found, # set             */
                                       /* members                           */

  if (*String == ch_SPACE)             /* if no leading blank               */
    if (Hex)                           /* hex version?                      */
                                       /* raise the hex message             */
      reportException(Error_Incorrect_method_hexblank, IntegerOne);
    else
                                       /* need the binary version           */
      reportException(Error_Incorrect_method_binblank, IntegerOne);
  SpaceFound = 0;                      /* set initial space flag            */
  Count = 0;                           /* start count with zero             */
  Current = String;                    /* point to start                    */

  for (; Length; Length--) {           /* process entire string             */
    c = *Current++;                    /* get char and step pointer         */
                                       /* if c in set                       */
    if (c != '\0' && strchr((char *)Set, c) != NULL)
      Count++;                         /* bump count                        */
    else {
      if (c == ch_SPACE) {             /* if c blank                        */
        SpaceLocation = Current;       /* save the space location           */
        if (!SpaceFound) {             /* if 1st blank                      */
                                       /* save position                     */
          Residue = (Count % Modulus);
          SpaceFound = 1;              /* we have the first space           */
        }
                                       /* else if bad position              */
        else if (Residue != (Count % Modulus)) {
          if (Hex)                     /* hex version?                      */
                                       /* raise the hex message             */
            reportException(Error_Incorrect_method_hexblank, SpaceLocation - String);
          else
                                       /* need the binary version           */
            reportException(Error_Incorrect_method_binblank, SpaceLocation - String);
        }
      }
      else {

        if (Hex)                       /* hex version?                      */
                                       /* raise the hex message             */
          reportException(Error_Incorrect_method_invhex, new_string((stringchar_t *)&c, 1));
        else
          reportException(Error_Incorrect_method_invbin, new_string((stringchar_t *)&c, 1));
      }
    }
  }
                                       /* if trailing blank or grouping bad */
  if (c == ch_SPACE || SpaceFound && (Count % Modulus) != Residue) {
    if (Hex)                           /* hex version?                      */
                                       /* raise the hex message             */
      reportException(Error_Incorrect_method_hexblank, SpaceLocation - String);
    else
                                       /* need the binary version           */
      reportException(Error_Incorrect_method_binblank, SpaceLocation - String);
  }
  return Count;                        /* return count of chars             */
}

/*********************************************************************/
/*                                                                   */
/*  Descriptive name:       Scan string for next members of          */
/*                          character set                            */
/*                                                                   */
/*********************************************************************/
stringsize_t  ChGetSm(
  stringchar_t *     Destination,               /* destination string         */
  stringchar_t *     Source,                    /* source string              */
  stringsize_t    Length,                    /* length of string           */
  stringsize_t    Count,                     /* size of string             */
  stringchar_t *     Set,                       /* allowed set of chars       */
  stringsize_t   *ScannedSize)               /* size scanned off           */
{
  stringchar_t     c;                         /* current scanned character  */
  stringchar_t *     Current;                   /* current scan pointer       */
  stringsize_t    Found;                     /* number of characters found */
  stringsize_t    Scanned;                   /* number of character scanned*/

  Scanned = 0;                         /* nothing scanned yet        */
  Found = 0;                           /* nothing found yet          */
  Current = Source;                    /* get pointer to string      */

  for (; Length; Length--) {           /* scan entire string         */
    c = *Current++;                    /* get char and step pointer  */
    Scanned++;                         /* remember scan count        */
                                       /* if c in set                       */
    if (c != '\0' && strchr((char *)Set, c) != NULL) {
      *Destination++ = c;              /* copy c                     */
      if (++Found == Count)            /* if all found               */
        break;                         /* we are all done            */
    }
  }
  *ScannedSize = Scanned;              /* return characters scanned  */
  return Found;                        /* and number found           */
}

/*********************************************************************/
/*                                                                   */
/*   Descriptive Name:  pack a string of 'hex' digits in place       */
/*                                                                   */
/*   Function:          take two alpha chars and make into one byte  */
/*                                                                   */
/*********************************************************************/

RexxString *packHex(
  stringchar_t *     String,           /* packed string                     */
  stringsize_t    StringLength )       /* packed string                     */
{
  stringsize_t   Nibbles;              /* count of nibbles to pack          */
  stringsize_t   n;
  stringchar_t *    Source;            /* pack source                       */
  stringchar_t *    Destination;       /* packing destination               */
  stringsize_t   b;                    /* nibble odd count                  */
  stringchar_t     Buf[8];             /* temp pack buffer                  */
  stringsize_t   jjj;                  /* copies nibbles                    */
  RexxString *Retval;                  /* result value                      */

  if (StringLength) {                  /* if not a null string              */
    Source = String;                   /* get pointer                       */
                                       /* validate the information          */
    Nibbles = validateSet(Source, StringLength, HEX_CHAR_STR, 2, true);
                                       /* get a result string               */
    Retval = raw_string((Nibbles + 1) / 2);
                                       /* initialize destination            */
    Destination = Retval->getStringData();

    while (Nibbles > 0) {              /* while chars to process            */

      b = Nibbles%2;                   /* get nibbles for next byte         */
      if (b == 0)                      /* even number                       */
        b = 2;                         /* use two bytes                     */
      else                             /* odd number,                       */
        memset(Buf, '0', 2);           /* pad with zeroes                   */

      jjj = 2 - b;                     /* copy nibbles into buff            */
      ChGetSm(Buf+jjj, Source, StringLength, b, HEX_CHAR_STR, &n);
      *Destination++ = packByte2(Buf); /* pack into destination             */
      Source += n;                     /* advance source location           */
      StringLength -= n;               /* reduce the length                 */
      Nibbles -= b;                    /* decrement the count               */
    }
                                       /* done building the string          */
  }
  else
                                       /* this is a null string             */
    Retval = OREF_NULLSTRING;
  return Retval;                       /* return the packed string          */
}

/*********************************************************************/
/*                                                                   */
/*      Descriptive name:       convert nibble to 4 '0'/'1' chars    */
/*                                                                   */
/*      Outputs:                p[0], ..., p[3]: the four '0'/'1'    */
/*                              chars representing the nibble        */
/*                                                                   */
/*      Note:                   No terminating null character is     */
/*                              produced                             */
/*                                                                   */
/*********************************************************************/

void unpackNibble(
  int    Val,                          /* nibble to unpack           */
  stringchar_t * p )                   /* nibble output location     */
{
  p[0] = (stringchar_t)(((Val)&8)?'1':'0');
  p[1] = (stringchar_t)(((Val)&4)?'1':'0');
  p[2] = (stringchar_t)(((Val)&2)?'1':'0');
  p[3] = (stringchar_t)(((Val)&1)?'1':'0');
}

/**
 * Convert the character string into the same string with the
 * characters converted into a Base64 encoding.
 *
 * @return The string reformatted into a Base64 encoding.
 */
RexxString *RexxString::encodeBase64()
{
    stringsize_t inc[3];
    int i;
    static const char cb64[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";


    stringsize_t inputLength = this->getLength();     /* get length of string              */
    if (inputLength == 0)               /* null string?                      */
    {
        return OREF_NULLSTRING;
    }
    /* figure out the output string length */
    stringsize_t outputLength = (inputLength / 3) * 4;
    if (inputLength % 3 > 0)
    {
        outputLength += 4;
    }
    /* allocate output string */
    RexxString *retval = raw_string(outputLength);
    stringchar_t *source = this->getStringData();  /* point to converted string         */
    /* point to output area              */
    stringchar_t *destination = retval->getStringData();
    while (inputLength > 0)
    {              /* while more string                 */
        for (i = 0; i < 3; i++)
        {        /* get the next 3 characters         */
            if (inputLength)
            {             /*    from the input string          */
                inc[i] = *source;
                inputLength--;
                source++;
            }
            else
            {
                inc[i] = '\0';
            }
        }
        /* now perform the base64 conversion to the next 4 output string chars */
        *destination = cb64[ inc[0] >> 2 ];
        destination++;
        *destination = cb64[ ((inc[0] & 0x03) << 4) | ((inc[1] & 0xf0) >> 4) ];
        destination++;
        *destination = (stringchar_t) (inputLength > 1 ? cb64[ ((inc[1] & 0x0f) << 2) | ((inc[2] & 0xc0) >> 6) ] : '=');
        destination++;
        *destination = (stringchar_t) (inputLength > 2 ? cb64[ inc[2] & 0x3f ] : '=');
        destination++;
    }                                  /* done building the string          */
    return retval;                       /* return converted string           */
}


/**
 * Reverse the effect of an encodebase64 operation, converting
 * a string in Base64 format into a "normal" character string.
 *
 * @return The converted character string.
 */
RexxString *RexxString::decodeBase64()
{
    unsigned int i, j;
    static const char cb64[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";


    stringsize_t inputLength = this->getLength();     /* get length of string              */
    if (inputLength == 0)                    /* null string?                      */
    {
        return OREF_NULLSTRING;          // this encodes as a null string
    }
    stringchar_t *source = (stringchar_t *)this->getStringData();
    /* figure out the output string length */
    stringsize_t outputLength = (inputLength / 4) * 3;
    if (*(source + inputLength - 3) == '=')
    {
        outputLength -= 2;
    }
    else if (*(source + inputLength - 1) == '=')
    {
        outputLength -= 1;
    }
    /* allocate output string */
    RexxString *retval = raw_string(outputLength);
    /* point to output area              */
    stringchar_t *destination = retval->getStringData();
    while (inputLength)
    {              /* while more string                 */
        for (i = 0; i < 4; i++)
        {
            for (j = 0; j < 64; j++)
            {
                if (*(cb64 + j) == *(source + i))
                {
                    break;
                }
            }
            // if we don't find the digit, this might be an invalid string.
            if (j == 64)
            {
                if (*(source + i) == '=' && inputLength <= 4) {
                    /* this means we are done building the output string */
                    break;
                }
                else {
                    /* we found an invalid char in the middle of the input string */
                    reportException(Error_Incorrect_method_invbase64);
                }
            }
            /* j is now the 6-bit value we need for building our output string */
            switch (i)
            {
                case 0:
                    *destination = (stringchar_t)(j << 2);
                    break;
                case 1:
                    *destination |= (stringchar_t)(j >> 4);
                    destination++;
                    *destination = (stringchar_t)(j << 4);
                    break;
                case 2:
                    *destination |= (stringchar_t)(j >> 2);
                    destination++;
                    *destination |= (stringchar_t)(j << 4);
                    break;
                case 3:
                    *destination |= (stringchar_t)j;
                    break;
                default: /* not really necessary but here anyway */
                    break;
            }
        }
        source += 4;
        inputLength -= 4;
    }                                  /* done building the string          */
    return retval;                       /* return converted string           */
}

RexxString *RexxString::c2x()
/******************************************************************************/
/* Function:  Process the string C2X method/function                          */
/******************************************************************************/
{
  stringsize_t InputLength;            /* length of converted string        */
  RexxString *Retval;                  /* return value                      */
  stringchar_t *Source;                /* input string pointer              */
  stringchar_t *Destination;           /* output string pointer             */
  stringchar_t  ch;                    /* current character                 */

  InputLength = this->getLength();     /* get length of string              */
  if (!InputLength)                    /* null string?                      */
    Retval = OREF_NULLSTRING;          /* converts to a null string         */
  else {                               /* real data to convert              */
                                       /* allocate output string            */
    Retval = raw_string(InputLength * 2);
    Source = (stringchar_t *)this->getStringData();  /* point to converted string         */
                                       /* point to output area              */
    Destination = Retval->getStringData();
    while (InputLength--) {            /* while more string                 */
      ch = *Source++;                  /* get next character                */
                 /***********************************************************/
                 /* get upper nibble after shifting out lower nibble and do */
                 /* logical ANDING with F to convert to integer then convert*/
                 /* to hex value and put it in destination                  */
                 /***********************************************************/
      *Destination++ = intToHexDigit((stringchar_t)((stringchar_t)ch>>4) & (stringchar_t)0xF);
                 /***********************************************************/
                 /* logical AND with F to convert lower nibble to integer   */
                 /* then convert to hex value and put it in destination     */
                 /***********************************************************/
      *Destination++ = intToHexDigit((stringchar_t)ch  & (stringchar_t)0xF);
    }
                                       /* done building the string          */
  }
  return Retval;                       /* return converted string           */
}

RexxString *RexxString::d2c(RexxInteger *length)
/******************************************************************************/
/* Function:  Process the string D2C method/function                          */
/******************************************************************************/
{
  RexxNumberString *numberstring;      /* converted number string version   */

                                       /* convert to a numberstring         */
  numberstring = this->numberString();
  if (numberstring == OREF_NULL)       /* not a valid number?               */
                                       /* report this                       */
    reportException(Error_Incorrect_method_d2c, this);
                                       /* format as a string value          */
  return numberstring->d2xD2c(length, true);
}

RexxString *RexxString::d2x(RexxInteger *length)
/******************************************************************************/
/* Function:  Process the string D2X method/function                          */
/******************************************************************************/
{
  RexxNumberString  *numberstring;     /* converted number string version   */

                                       /* convert to a numberstring         */
  numberstring = this->numberString();
  if (numberstring == OREF_NULL)       /* not a valid number?               */
                                       /* report this                       */
    reportException(Error_Incorrect_method_d2x, this);
                                       /* format as a string value          */
  return numberstring->d2xD2c(length, false);
}

RexxString *RexxString::x2c()
/******************************************************************************/
/* Function:  Process the string X2C method/function                          */
/******************************************************************************/
{
  stringsize_t   InputLength;                /* length of converted string */
  RexxString *Retval;                  /* return value               */

  InputLength = this->getLength();          /* get length of string       */
  if (!InputLength)                    /* null string?               */
    Retval = OREF_NULLSTRING;          /* converts to a null string         */

  else                                 /* real data to convert       */
                                       /* try to pack the data       */
    Retval = packHex((stringchar_t *)this->getStringData(), InputLength);
  return Retval;                       /* return the packed string   */
}

RexxString *RexxString::x2d(RexxInteger *length)
/******************************************************************************/
/* Function:  Process the string X2D method/function                          */
/******************************************************************************/
{
                                       /* forward to the common routine     */
  return this->x2dC2d(length, false);
}


RexxString *RexxString::x2dC2d(RexxInteger *length,
                               bool type )
/******************************************************************************/
/* Function:  Common X2D/X2C processing routine                               */
/******************************************************************************/
{
  stringsize_t     ResultSize;         /* size of result string             */
  stringsize_t     TempSize;           /* temporary size value              */
  int        ch;                       /* addition character                */
  stringsize_t     StringLength;       /* input string length               */
  stringchar_t *      Scan;            /* scan pointer                      */
  stringchar_t *      HighDigit;       /* high digit position               */
  stringchar_t *      Accumulator;     /* accumulator pointer               */
  bool       Negative;                 /* have a negative number            */
  RexxString *String;                  /* converted string                  */
  stringchar_t *      StringPtr;       /* string value pointer              */
  stringsize_t     BytePosition;       /* position of high byte             */
  stringsize_t     NibblePosition;     /* position of high nibble           */
  stringsize_t     DecLength;          /* length of accumulator             */
  stringsize_t     TempLength;         /* length of accumulator             */
  RexxString *Retval;                  /* function return value             */
  RexxBuffer *Buffer;                  /* first math buffer                 */
  stringsize_t     CurrentDigits;      /* current digits setting            */

  CurrentDigits = numberDigits();     /* get the current digits setting    */
  StringLength = this->getLength();         /* get Argument string length        */
                                       /* get the target length             */
  ResultSize = optionalLengthArgument(length, -1, ARG_ONE);
  if (!ResultSize)                     /* zero requested                    */
    return (RexxString *)IntegerZero;  /* always returns zero               */

  String = this;                       /* use this string directly          */
  StringPtr = this->getStringData();        /* get a string pointer              */
  NibblePosition = 0;                  /* assume an even nibble number      */

  if (type == true) {                  /* dealing with character?           */
    if (ResultSize == -1) {            /* no size specified?                */
      Negative = false;                /* can't be negative                 */
      ResultSize = StringLength;       /* use entire string                 */
    }
    else {                             /* have to check for negative        */
      if (ResultSize > StringLength)   /* longer than string?               */
        Negative = false;              /* can't be negative                 */
      else {                           /* have to check sign bit            */
                                       /* step to byte position             */
        StringPtr += StringLength - ResultSize;
        StringLength = ResultSize;     /* adjust the size down              */

        if (*StringPtr & (stringchar_t)0x80) {/* first bit on?                     */
          Negative = true;             /* this is a negative number         */
                                       /* copy the string                   */
          String = (RexxString *)this->copy();
                                       /* point to the string               */
          StringPtr = String->getStringData() + this->getLength() - ResultSize;
        }
        else                           /* still a positive number           */
          Negative = false;            /* remember for later                */
      }
    }
  }
  else {                               /* x2d function                      */
                                       /* pack the string                   */
    String = (RexxString *)packHex(StringPtr, StringLength);
                                       /* get the packed length             */
    StringLength = String->getLength();
                                       /* point to the packed data          */
    StringPtr = String->getStringData();
    if (ResultSize == -1) {            /* no size specified?                */
      Negative = false;                /* can't be negative                 */
      ResultSize = StringLength;       /* use entire string                 */
    }
    else {                             /* have to check for negative        */

      BytePosition = ResultSize / 2;   /* Get position of sign bit          */
                                       /* get nibble position               */
      NibblePosition = ResultSize % 2;
                                       /* Get result size                   */
      ResultSize = (BytePosition + NibblePosition);
      if (ResultSize > StringLength) { /* longer than string?               */
        Negative = false;              /* can't be negative                 */
        NibblePosition = 0;            /* leave the high nibble alone       */
      }
      else {                           /* have to check sign bit            */
                                       /* step to byte position             */
        StringPtr += StringLength - ResultSize;
        StringLength = ResultSize;     /* adjust the size down              */

        if ((NibblePosition &&         /* odd number of nibbles             */
                                       /* and low nibble negative?          */
            *StringPtr & (stringchar_t)0x08) ||
            (!NibblePosition &&        /* or even number of nibbles         */
            *StringPtr & (stringchar_t)0x80)) /* and high nibble negative?         */
          Negative = true;             /* this is a negative number         */
        else                           /* still a positive number           */
          Negative = false;            /* remember for later                */
      }
    }
  }

  if (Negative) {                      /* need to negate string?            */
    Scan = StringPtr;                  /* copy the pointer                  */
    TempSize = StringLength;           /* copy the size                     */

    while (TempSize--) {               /* reverse each byte                 */
                                       /* exclusive or with foxes           */
      *Scan = (stringchar_t)(*Scan ^ (stringchar_t)0xff);
      Scan++;                          /* step the pointer                  */
    }
                                       /* point to the first byte           */
    Scan = StringPtr + StringLength - 1;
    TempSize = StringLength;           /* copy the size                     */
    while (TempSize--) {               /* now add one to the number         */
      ch = (stringchar_t)*Scan;               /* get the character                 */
      ch++;                            /* increment                         */
      if (ch <= (stringchar_t)0xff) {         /* no carry over?                    */
        *Scan = (stringchar_t)ch;             /* set value back                    */
        break;                         /* we're finished                    */
      }
      else {                           /* carried out                       */
        *Scan = 0;                     /* this is zero now                  */
        Scan--;                        /* step back one pointer             */
      }
    }
  }
  if (NibblePosition)                  /* Odd number of nibbles?            */
    *StringPtr &= 0x0f;                /* zero out the highest nibble       */

  Scan = StringPtr;                    /* point to the string               */
                                       /* allocate a temp buffer            */
  Buffer = (RexxBuffer *)new_buffer(CurrentDigits + OVERFLOWSPACE + 1);
                                       /* set accumulator pointer           */
  Accumulator = Buffer->data + CurrentDigits + OVERFLOWSPACE;
                                       /* clear the buffer                  */
  memset(Buffer->data,
         '\0',
         CurrentDigits + OVERFLOWSPACE + 1);
  HighDigit = Accumulator - 1;         /* set initial high point            */

  while (StringLength--) {             /* while more digits                 */
    ch = *Scan++;                      /* get the character                 */
                                       /* add high order nibble             */
    HighDigit = RexxNumberString::addToBaseTen((stringchar_t)((unsigned)ch & (unsigned)0xf0)>>4, Accumulator, HighDigit);
                                       /* multiply by 16                    */
    HighDigit = RexxNumberString::multiplyBaseTen(Accumulator, HighDigit);
                                       /* get accumulator length            */
    DecLength = (Accumulator - HighDigit);
    if (DecLength > CurrentDigits) {   /* grown too long?                   */
      if (type == true)                /* c2d version?                      */
        reportException(Error_Incorrect_method_c2dbig, CurrentDigits);
      else                             /* this is the x2d function          */
        reportException(Error_Incorrect_method_x2dbig, CurrentDigits);
    }
                                       /* add high order nibble             */
    HighDigit = RexxNumberString::addToBaseTen((stringchar_t)ch & (stringchar_t)0x0f, Accumulator, HighDigit);
    if (StringLength != 0)             /* not the last one?                 */
                                       /* multiply by 16                    */
      HighDigit = RexxNumberString::multiplyBaseTen(Accumulator, HighDigit);
                                       /* get accumulator length            */
    DecLength = (Accumulator - HighDigit);
    if (DecLength > CurrentDigits) {   /* grown too long?                   */
      if (type == true)                /* c2d version?                      */
        reportException(Error_Incorrect_method_c2dbig, CurrentDigits);
      else                             /* this is the x2d function          */
        reportException(Error_Incorrect_method_x2dbig, CurrentDigits);
    }
  }
                                       /* get accumulator length            */
  DecLength = (Accumulator - HighDigit);
  TempLength = DecLength;              /* copy the length                   */
  Scan = HighDigit + 1;                /* point to the first digit          */
  while (TempLength--) {               /* make into real digits again       */
                                       /* add zero to each digit            */
    *Scan = (stringchar_t)(*Scan + (stringchar_t)'0');
    Scan++;                            /* step the pointer                  */
  }

  ResultSize = DecLength;              /* get the result size               */
  if (Negative)                        /* negative number?                  */
    ResultSize++;                      /* add in space for the sign         */
  Retval = raw_string(ResultSize);     /* allocate output buffer            */
  Scan = Retval->getStringData();           /* point to output location          */
  if (Negative)                        /* need a sign?                      */
    *Scan++ = '-';                     /* add to the front                  */
                                       /* copy in the number                */
  memcpy(Scan, Accumulator - DecLength + 1, DecLength);
  return Retval;                       /* return converted string           */
}

RexxString *RexxString::b2x()
/******************************************************************************/
/* Function:  Common B2X processing routine                                   */
/******************************************************************************/
{
  RexxString *Retval;                  /* function result                   */
  stringsize_t   Bits;                       /* number of bits in string          */
  stringchar_t *    Source;                     /* current source pointer            */
  stringchar_t *    Destination;                /* destination pointer               */
  stringsize_t   Excess;                     /* section boundary                  */
  stringchar_t     Nibble[4];                  /* current nibble string             */
  stringsize_t   Jump;                       /* string movement offset            */
  stringsize_t   Length;                     /* total string length               */

  if (this->getLength() == 0)               /* null input, i.e. zerolength       */
                                       /* string                            */
    Retval = OREF_NULLSTRING;          /* return null                       */
  else {                               /* need to do conversion             */
                                       /* validate the string               */
    Bits = validateSet((stringchar_t *)this->getStringData(), this->getLength(), (stringchar_t *)"01", 4, false);
                                       /* allocate space for result         */
    Retval = raw_string((Bits + 3) / 4);
                                       /* point to the data                 */
    Destination = Retval->getStringData();
    Source = (stringchar_t *)this->getStringData();  /* point to the source               */
    Length = this->getLength();             /* get the string length             */

    while (Bits > 0) {                 /* process the string                */
      Excess = Bits % 4;               /* calculate section size            */
      if (Excess == 0)                 /* zero is a multiple of 4           */
        Excess = 4;                    /* so use 4                          */
      else
        memset(Nibble, '0', 4);        /* pad the nibble with zeroes        */
      ChGetSm(&Nibble[0] + (4 - Excess), Source, Length, Excess, (stringchar_t *)"01", &Jump);
                                       /* pack into destination             */
      *Destination++ = packNibble((stringchar_t *)Nibble);
      Source += Jump;                  /* advance source pointer            */
      Length -= Jump;                  /* reduce remaining length           */
      Bits -= Excess;                  /* reduce remaining amount           */
    }
                                       /* done building the string          */
  }
  return Retval;                       /* return packed string              */
}

RexxString *RexxString::c2d(RexxInteger *length)
/******************************************************************************/
/* Function:  Common C2D processing routine                                   */
/******************************************************************************/
{
                                       /* forward to the common routine     */
  return this->x2dC2d(length, true);
}

RexxString *RexxString::x2b()
/******************************************************************************/
/* Function:  Common X2B processing routine                                   */
/******************************************************************************/
{
  RexxString *Retval;                  /* function result                   */
  stringsize_t   Nibbles;              /* nibbles in hex string             */
  stringchar_t *    Source;            /* current source pointer            */
  stringchar_t *    Destination;       /* destination pointer               */
  stringchar_t     Nibble[4];          /* current nibble string             */
  stringchar_t    ch;                  /* current string character          */
  int      Val;                        /* converted nible                   */

  if (this->getLength() == 0)               /* null input, i.e. zerolength       */
                                       /* string                            */
    Retval = OREF_NULLSTRING;          /* return null                       */
  else {                               /* have real data to pack            */
    Nibbles = validateSet((stringchar_t *)this->getStringData(), this->getLength(), HEX_CHAR_STR, 2, true);
    Retval = raw_string(Nibbles * 4);  /* allocate result string            */
                                       /* point to the data                 */
    Destination = Retval->getStringData();
    Source = (stringchar_t *)this->getStringData();  /* point to the source               */

    while (Nibbles > 0) {              /* while still string to pack        */
      ch = *Source++;                  /* get current char and bump         */
                                       /* pointer                           */
      if (ch != ch_SPACE) {            /* if not a filler space             */
        Val = hexDigitToInt(ch);       /* convert hex to int first          */
        unpackNibble(Val, Nibble);     /* then convert to binary            */
                                       /* digits                            */
                                       /* copy to the destination           */
        memcpy(Destination, (stringchar_t *)Nibble, 4);
        Destination += 4;              /* bump destination pointer          */
        Nibbles--;                     /* Reduce nibbles count              */
      }
    }
                                       /* done building the string          */
  }
  return Retval;                       /* return the expanded string        */
}
