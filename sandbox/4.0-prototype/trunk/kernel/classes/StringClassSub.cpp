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
/* REXX Kernel                                                  okbsubs.c     */
/*                                                                            */
/* substring oriented REXX string methods                                     */
/*                                                                            */
/******************************************************************************/

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "RexxCore.h"
#include "StringClass.hpp"
#include "ArgumentUtilities.hpp"
#include "Interpreter.hpp"

/* the CENTER function (and the CENTRE function) */
/******************************************************************************/
/* Arguments:  String len, string pad character                               */
/*                                                                            */
/*  Returned:  string                                                         */
/******************************************************************************/
RexxString *RexxString::center(RexxInteger *length,
                               RexxString  *pad)
{
  stringchar_t     PadChar;                    /* pad character                     */
  stringsize_t   LeftPad;                    /* required left pads                */
  stringsize_t   RightPad;                   /* required right pads               */
  stringsize_t   Space;                      /* result string size                */
  stringsize_t   Width;                      /* centering width                   */
  stringsize_t   Len;                        /* string length                     */
  RexxString *Retval;                  /* return string                     */

                                       /* see how long result should be     */
  Width = lengthArgument(length, ARG_ONE);

                                       /* Get pad character (optional) blank*/
                                       /*  is used if omitted.              */
  PadChar = padArgument(pad, ' ', ARG_TWO);
  Len = this->getLength();                  /* get length of input to center     */
  if (Width == Len)                    /* if input length and               */
                                       /* requested are  the same           */
    Retval = this;                     /* then copy input                   */

  else if (!Width)                     /* centered in zero?                 */
    Retval = OREF_NULLSTRING;          /* return a null string              */
  else {
    if (Width > Len) {                 /* otherwise                         */
                                       /* if requested larger               */
      LeftPad = (Width - Len) / 2;     /* get left pad count                */
      RightPad = (Width - Len)-LeftPad;/* and right pad count               */
      Space = RightPad + LeftPad + Len;/* total space required              */
                                       /* allocate space                    */
      Retval = (RexxString *)raw_string(Space);
                                       /* set left pad characters           */
      memset(Retval->getStringData(), PadChar, LeftPad);
      if (Len)                         /* something to copy?                */
                                       /* copy the string                   */
        memcpy(Retval->getStringData() + LeftPad,
               (stringchar_t *)this->getStringData(),
               Len);
                                       /* now the trailing pad chars        */
      memset(Retval->getStringData() + LeftPad + Len,
             PadChar,
             RightPad);
                                       /* done building the string          */
    }
    else {                             /* requested smaller than            */
                                       /* input                             */
      LeftPad = (Len - Width) / 2;     /* get left truncate count           */
                                       /* copy the data                     */
      Retval = (RexxString *)new_string((stringchar_t *)this->getStringData() + LeftPad, Width);
    }
  }
  return Retval;                       /* done, return output buffer        */
}

/* the DELSTR function */
/******************************************************************************/
/* Arguments:  Starting position of string to be deleted                      */
/*             length of string to be deleted                                 */
/*  Returned:  string                                                         */
/******************************************************************************/
RexxString *RexxString::delstr(RexxInteger *position,
                               RexxInteger *length)
{
  RexxString *Retval;                  /* return value:                     */
  stringsize_t   BackLen;                    /* end string section                */
  stringsize_t   StringLen;                  /* original string length            */
  stringsize_t   DeleteLen;                  /* deleted length                    */
  stringsize_t   DeletePos;                  /* delete position                   */
  stringchar_t *    Current;                    /* current copy position             */

  StringLen = this->getLength();            /* get string length                 */
                                       /* get start string position         */
  DeletePos = positionArgument(position, ARG_ONE);
                                       /* get the length to delete          */
  DeleteLen = optionalLengthArgument(length, StringLen - DeletePos + 1, ARG_TWO);

  if (DeletePos > StringLen)           /* beyond string bounds?             */
    Retval = this;                     /* return string unchanged           */

  else {                               /* need to actually delete           */
    DeletePos--;                       /* make position origin zero         */
                                       /* deleting more than string?        */
    if (DeleteLen >= (StringLen - DeletePos))
      BackLen = 0;                     /* no back part                      */
    else                               /* find length to delete             */
      BackLen = StringLen - (DeletePos + DeleteLen);
                                       /* allocate result string            */
    Retval = (RexxString *)raw_string(DeletePos + BackLen);
                                       /* point to string part              */
    Current = Retval->getStringData();
    if (DeletePos) {                   /* have a front part?                */
                                       /* copy it                           */
      memcpy(Current, (stringchar_t *)this->getStringData(), DeletePos);
      Current += DeletePos;            /* step past the front               */
    }

    if (BackLen) {                     /* have a trailing part              */
                                       /* copy that over                    */
      memcpy(Current,
             (stringchar_t *)this->getStringData() + DeletePos + DeleteLen,
             BackLen);
    }
  }
  return Retval;                       /* return the new string             */
}

/* the INSERT function */
/******************************************************************************/
/* Arguments:  string to be inserted                                          */
/*             position in self to place new string                           */
/*             length of new string to insert, padded if necessary            */
/*             pad character to use.                                          */
/*  Returned:  string                                                         */
/******************************************************************************/
RexxString *RexxString::insert(RexxString  *newStrObj,
                               RexxInteger *position,
                               RexxInteger *length,
                               RexxString  *pad)
{
  RexxString *Retval;                  /* return string                     */
  RexxString *newStr;                  /* return string                     */
  stringchar_t     PadChar;                    /* HugeString for Padding char       */
  stringsize_t   ReqLenChar;                 /* Actual req char len of new.       */
  stringsize_t   ReqPadChar;                 /* Actual req char len of new.       */
  stringsize_t   ReqLeadPad;                 /* Actual req char len of new.       */
  stringsize_t   TargetSize;                 /* byte size of target string        */
  stringsize_t   NCharLen;                   /* Char len of new HugeString.       */
  stringsize_t   TCharLen;                   /* Char len of target HugeStr.       */
  stringsize_t   FCharLen;                   /* Char len of front portion.        */
  stringsize_t   BCharLen;                   /* Char len of back portion.         */
  stringsize_t   BuffSiz;                    /* Estimated result area size.       */
  stringsize_t   NChar;                      /* Character position.               */
  stringchar_t *    Current;                    /* current copy location             */

  TCharLen = this->getLength();             /* get the target string length      */
                                       /* get the needle string (and length)*/
  newStr = (RexxString *)stringArgument(newStrObj, ARG_ONE);
  NCharLen = newStr->getLength();
                                       /* use optionalLength for starting  */
                                       /* position becase a value of 0 IS   */
                                       /* valid for INSERT                  */
  NChar = optionalLengthArgument(position, 0, ARG_TWO);
                                       /* get the optional length, using the*/
                                       /* needle length as the defaul       */
  ReqLenChar = optionalLengthArgument(length, NCharLen, ARG_THREE);

                                       /*  is used if omitted.              */
  PadChar = padArgument(pad, ' ', ARG_FOUR);
  ReqLeadPad = 0;                      /* set lead pad to zero              */
  TargetSize = TCharLen;               /* copy the target size              */

  if (NChar == 0) {                    /* inserting at the front?           */
    ReqLeadPad = 0;                    /* no leading pads                   */
    FCharLen = 0;                      /* no front part                     */
    BCharLen = TCharLen;               /* trailer is entire target          */
  }
  else if (NChar >= TCharLen) {        /* need leading pads?                */
    ReqLeadPad = (NChar - TCharLen);   /* calculate needed                  */
    FCharLen = TCharLen;               /* front part is all                 */
    BCharLen = 0;                      /* trailer is nothing                */
  }
  else {                               /* have a split                      */
    ReqLeadPad = 0;                    /* no leading pad                    */
    FCharLen = NChar;                  /* NChar front chars                 */
    BCharLen = TCharLen - NChar;       /* and some trailer too              */
  }
  NCharLen = min(NCharLen, ReqLenChar);/* truncate new, if needed           */
  ReqPadChar = ReqLenChar - NCharLen;  /* calculate pad chars               */
                                       /* calculate result size             */
  BuffSiz = NCharLen + TargetSize + ReqPadChar + ReqLeadPad;
  Retval = raw_string(BuffSiz);        /* allocate the result               */
  Current = Retval->getStringData();        /* point to start                    */

  if (FCharLen) {                      /* have leading chars                */
                                       /* copy the leading part             */
    memcpy(Current, (stringchar_t *)this->getStringData(), FCharLen);
    Current += FCharLen;               /* step copy location                */
  }
  if (ReqLeadPad) {                    /* if required leading pads          */
                                       /* add the pads now                  */
    memset(Current, PadChar, ReqLeadPad);
    Current += ReqLeadPad;             /* step the output pointer           */
  }

  if (NCharLen) {                      /* new string to copy?               */
                                       /* copy the inserted part            */
    memcpy(Current, newStr->getStringData(), NCharLen);
    Current += NCharLen;               /* step copy location                */
  }

  if (ReqPadChar) {                    /* if required trailing pads         */
                                       /* add the pads now                  */
    memset(Current, PadChar, ReqPadChar);
    Current += ReqPadChar;             /* step the output pointer           */
  }

  if (BCharLen) {                      /* have trailing chars               */
                                       /* copy the leading part             */
    memcpy(Current, (stringchar_t *)this->getStringData() + FCharLen, BCharLen);
  }
  return Retval;                       /* Return the new string             */
}

/* the LEFT function */
/******************************************************************************/
/* Arguments:  String len, string pad character                               */
/*                                                                            */
/*  Returned:  string                                                         */
/******************************************************************************/
RexxString *RexxString::left(RexxInteger *length,
                             RexxString  *pad)
{
  stringchar_t      PadChar;                   /* pad character                     */
  stringsize_t    Size;                      /* requested size                    */
  stringsize_t    Length;                    /* string length                     */
  RexxString *Retval;                  /* returned result                   */
  stringchar_t *     Current;                   /* current copy location             */
  stringsize_t    CopyLength;                /* length to copy                    */

                                       /* get the target length             */
  Size = lengthArgument(length, ARG_ONE);

                                       /*  is used if omitted.              */
  PadChar = padArgument(pad, ' ', ARG_TWO);
  Length = this->getLength();               /* get input length                  */

  if (!Size)                           /* requesting zero bytes?            */
    Retval = OREF_NULLSTRING;          /* return a null string              */
  else {
    Retval = raw_string(Size);         /* allocate a result string          */
    CopyLength = min(Length, Size);    /* adjust the length                 */
                                       /* point to data part                */
    Current = Retval->getStringData();
    if (CopyLength) {                  /* have real data?                   */
                                       /* copy it                           */
      memcpy(Current, (stringchar_t *)this->getStringData(), CopyLength);
      Current += CopyLength;           /* bump the pointer                  */
    }
    if (Size > Length)                 /* need to pad?                      */
                                       /* pad the string                    */
      memset(Current, PadChar, Size - Length);
  }
  return Retval;                       /* return string piece               */
}

/******************************************************************************/
/* Function:  Process the OVERLAY function/method                             */
/******************************************************************************/
RexxString *RexxString::overlay(
    RexxString  *newStrObj,            /* overlayed string                  */
    RexxInteger *position,             /* overlay position                  */
    RexxInteger *length,               /* overlay length                    */
    RexxString  *pad)                  /* pad character to use.             */
{
  RexxString *Retval;                  /* return string                     */
  RexxString *newStr;                  /* return string                     */
  stringsize_t   OverlayPos;                 /* overlay position                  */
  stringsize_t   OverlayLen;                 /* overlay length                    */
  stringsize_t   NewLen;                     /* length of overlay string          */
  stringsize_t   TargetLen;                  /* target length                     */
  stringsize_t   FrontLen;                   /* front length                      */
  stringsize_t   BackLen;                    /* back length                       */
  stringsize_t   FrontPad;                   /* front pad length                  */
  stringsize_t   BackPad;                    /* back pad length                   */
  stringchar_t     PadChar;                    /* pad character                     */
  stringchar_t *    Current;                    /* current copy location             */

  TargetLen = this->getLength();            /* get the haystack length           */
                                       /* get the overlay string value      */
  newStr = (RexxString *)stringArgument(newStrObj, ARG_ONE);
  NewLen = newStr->getLength();
                                       /* get the overlay position          */
  OverlayPos = optionalPositionArgument(position, 1, ARG_TWO);
                                       /* get final overlay length          */
  OverlayLen = optionalLengthArgument(length, NewLen, ARG_THREE);
                                       /*  is used if omitted.              */
  PadChar = padArgument(pad, ' ', ARG_FOUR);

  if (OverlayLen > NewLen)             /* need to pad?                      */
    BackPad = OverlayLen - NewLen;     /* get the pad size                  */
  else {                               /* need to truncate                  */
    NewLen = OverlayLen;               /* used specified length             */
    BackPad = 0;                       /* no back padding                   */
  }

  if (OverlayPos > TargetLen) {        /* overlaying past the end?          */
                                       /* get front padding                 */
    FrontPad = OverlayPos - TargetLen - 1;
    FrontLen = TargetLen;              /* copy entire string                */
  }
  else {                               /* overlay is within bounds          */
    FrontPad = 0;                      /* no padding here                   */
    FrontLen = OverlayPos - 1;         /* just copy the front part          */
  }
                                       /* fall off the back side?           */
  if (OverlayPos + OverlayLen > TargetLen)
    BackLen = 0;                       /* no back part                      */
  else
                                       /* calculate the back part           */
    BackLen = TargetLen - (OverlayPos + OverlayLen - 1);
                                       /* allocate result string            */
  Retval = raw_string(FrontLen + BackLen + FrontPad + OverlayLen);

  Current = Retval->getStringData();        /* get copy location                 */

  if (FrontLen) {                      /* something in front?               */
                                       /* copy the front part               */
    memcpy(Current, (stringchar_t *)this->getStringData(), FrontLen);
    Current += FrontLen;               /* step the pointer                  */
  }

  if (FrontPad) {                      /* padded in front?                  */
    memset(Current, PadChar, FrontPad);/* set the pad characters            */
    Current += FrontPad;               /* step the pointer                  */
  }

  if (NewLen) {                        /* non-null new string?              */
                                       /* copy the string                   */
    memcpy(Current, newStr->getStringData(), NewLen);
    Current += NewLen;                 /* step the pointer                  */
  }

  if (BackPad) {                       /* padded in back?                   */
                                       /* set the pad characters            */
    memset(Current, PadChar, BackPad);
    Current += BackPad;                /* step the pointer                  */
  }

  if (BackLen) {                       /* trailing part?                    */
                                       /* copy the string                   */
    memcpy(Current, (stringchar_t *)this->getStringData() + OverlayPos + OverlayLen - 1, BackLen);
  }
  return Retval;                       /* return new string                 */
}

/* the REVERSE function */
/******************************************************************************/
/* Arguments:  none                                                           */
/*                                                                            */
/*  Returned:  string reversed.                                               */
/******************************************************************************/
RexxString *RexxString::reverse()
{
  RexxString *Retval;                  /* temp pointer for reversal       */
  stringsize_t     Length;                   /* string length                   */
  stringchar_t *      String;                   /* current location                */
  stringchar_t *      End;                      /* string end position             */

  Length = this->getLength();               /* get first argument              */
  if (Length) {                        /* if really data                  */
    Retval = raw_string(Length);       /* get result storage              */
                                       /* get new string pointer          */
    String = Retval->getStringData();
                                       /* point to end of original        */
    End = (stringchar_t *)this->getStringData() + Length - 1;

    while (Length--)                   /* reverse entire string           */
      *String++ = *End--;              /* copy a single char              */
  }
  else                                 /* if null input                     */
    Retval = OREF_NULLSTRING;          /* return null output                */
  return Retval;                       /* return the reversed string        */
}

/* the RIGHT function */
/******************************************************************************/
/* Arguments:  length of result                                               */
/*             pad character to use if needed.                                */
/*                                                                            */
/*  Returned:  string right justified.                                        */
/******************************************************************************/
RexxString *RexxString::right(RexxInteger *length,
                              RexxString  *pad)
{
  stringchar_t      PadChar;                   /* pad character                     */
  stringsize_t    Size;                      /* requested size                    */
  stringsize_t    Length;                    /* string length                     */
  RexxString *Retval;                  /* returned result                   */
  stringchar_t *     Current;                   /* current copy location             */
  stringsize_t    CopyLength;                /* length to copy                    */

                                       /* get the target length             */
  Size = lengthArgument(length, ARG_ONE);

                                       /*  is used if omitted.              */
  PadChar = padArgument(pad, ' ', ARG_TWO);
  Length = this->getLength();               /* get input length                  */

  if (!Size)                           /* requesting zero bytes?            */
                                       /* return a null string              */
    Retval = OREF_NULLSTRING;
  else {
    Retval = raw_string(Size);         /* allocate a result string          */
    CopyLength = min(Length, Size);    /* adjust the length                 */
                                       /* point to data part                */
    Current = Retval->getStringData();
    if (Size > Length) {               /* need to pad?                      */
                                       /* pad the string                    */
      memset(Current, PadChar, Size - Length);
      Current += Size - Length;        /* bump the pointer                  */
    }
    if (CopyLength)                    /* have real data?                   */
                                       /* copy it                           */
      memcpy(Current,
             (stringchar_t *)this->getStringData() + Length - CopyLength,
             CopyLength);
  }
  return Retval;                       /* return string piece               */
}

/* the STRIP function */
/******************************************************************************/
/* Arguments:  option, where to strip the characters                          */
/*             the character to strip.                                        */
/*                                                                            */
/*  Returned:  string                                                         */
/******************************************************************************/
RexxString *RexxString::strip(RexxString *option,
                              RexxString *stripchar)
{
  stringchar_t *       Back;                    /* pointer to back part              */
  stringchar_t *       Front;                   /* pointer to front part             */
  stringsize_t      Length;                  /* length of the string              */
  stringchar_t        RemoveChar;              /* character to remove               */
  stringchar_t        Option;                  /* strip option                      */
  RexxString *Retval;                  /* return value                      */

                                       /* get the option character          */
  Option = optionArgument(option, STRIP_BOTH, ARG_ONE);
  if (Option != STRIP_TRAILING &&      /* must be a valid option            */
        Option != STRIP_LEADING &&
        Option != STRIP_BOTH )
  reportException(Error_Incorrect_method_option, (stringchar_t *)"BLT", option);
                                       /* go get the character we are going */
                                       /* to be stripping.                  */
  RemoveChar = padArgument(stripchar, ' ', ARG_TWO);

  Front = (stringchar_t *)this->getStringData();     /* point to string start             */
  Length = this->getLength();               /* get the length                    */

                                       /* need to strip leading?            */
  if (Option == STRIP_LEADING || Option == STRIP_BOTH) {

    while (Length) {                   /* while more string                 */
      if (*Front != RemoveChar)        /* done stripping?                   */
        break;                         /* quit                              */
      Front++;                         /* step the pointer                  */
      Length--;                        /* reduce the length                 */
    }
  }

                                       /* need to strip trailing?           */
  if (Option == STRIP_TRAILING || Option == STRIP_BOTH) {
    Back = Front + Length - 1;         /* point to the end                  */
    while (Length) {                   /* while more string                 */
      if (*Back != RemoveChar)         /* done stripping?                   */
        break;                         /* quit                              */
      Back--;                          /* step the pointer back             */
      Length--;                        /* reduce the length                 */
    }
  }

  if (Length)                          /* have anything left?               */
    Retval = new_string(Front, Length);/* extract remaining piece           */
  else
    Retval = OREF_NULLSTRING;          /* nothing left, us a null           */
  return Retval;                       /* return stripped string            */
}

/* the SUBSTR function */
/******************************************************************************/
/* Arguments:  String position for substr                                     */
/*             requested length of new string                                 */
/*             pad character to use, if necessary                             */
/*                                                                            */
/*  Returned:  string, sub string of original.                                */
/******************************************************************************/
RexxString *RexxString::substr(RexxInteger *position,
                               RexxInteger *length,
                               RexxString  *pad)
{
  stringchar_t    PadChar;                    /* pad character                     */
  stringsize_t   Position;                   /* start position                    */
  stringsize_t   Length;                     /* required length                   */
  stringsize_t   StringLength;               /* input string length               */
  stringsize_t   SubstrLength;               /* length plucked from string        */
  stringsize_t   PadCount;                   /* number of pad characters          */
  RexxString *Retval;                  /* returned value                    */

                                       /* get starting position             */
  Position = positionArgument(position, ARG_ONE) - 1;
  StringLength = this->getLength();         /* get the string length             */
  if (StringLength == 0)               /* string of zerolength              */
    Length = 0;                        /* no partial to handle              */
  else if (StringLength >= Position)   /* pos within the string?            */
    Length = StringLength - Position;  /* length is remainder               */
  else
    Length = 0;                        /* string is used up                 */
                                       /* go get length or use default if   */
                                       /* not supplied.                     */
  Length = optionalLengthArgument(length, Length, ARG_TWO);
                                       /* go get optional pad character,    */
                                       /*  is used if omitted.              */
  PadChar = padArgument(pad, ' ', ARG_THREE);

  if (Length == 0)                     /* nothing to return?                */
    Retval = OREF_NULLSTRING;          /* return a null string              */
  else {
    if (Position > StringLength) {     /* start past the string?            */
      SubstrLength = 0;                /* nothing to copy                   */
      PadCount = Length;               /* just pad characters               */
    }
    else {                             /* have something to copy            */
                                       /* get string size                   */
      SubstrLength = min(Length, StringLength - Position);
                                       /* and number of pad chars           */
      PadCount = Length - SubstrLength;
    }
    Retval = raw_string(Length);       /* get result string                 */
    if (SubstrLength)                  /* data to copy?                     */
                                       /* yes, copy over                    */
      memcpy(Retval->getStringData(),
             (stringchar_t *)this->getStringData() + Position,
             SubstrLength);
    if (PadCount)                      /* padding needed?                   */
                                       /* add the pad characters            */
      memset(Retval->getStringData() + SubstrLength,
             PadChar,
             PadCount);
  }
  return Retval;                       /* return extracted string           */
}



/**
 * Extract a single character from a string object.
 * Returns a null string if the specified position is
 * beyond the bounds of the string.
 *
 * @param positionArg
 *               The position of the target  character.  Must be a positive
 *               whole number.
 *
 * @return Returns the single character at the target position.
 *         Returns a null string if the position is beyond the end
 *         of the string.
 */
RexxString *RexxString::subchar(RexxInteger *positionArg)
{
    // the starting position isn't optional
    stringsize_t position = get_position(positionArg, ARG_ONE) - 1;
    stringsize_t stringLength = this->length;

    // beyond the bounds, this is a null string
    if (position >= stringLength)
    {
        return OREF_NULLSTRING;
    }
    // return the single character
    return this->extract(position, 1);
}
