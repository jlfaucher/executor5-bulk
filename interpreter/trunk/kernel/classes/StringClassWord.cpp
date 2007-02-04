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
/* REXX Kernel                                                  okbword.c     */
/*                                                                            */
/* Word-related REXX string methods                                           */
/*                                                                            */
/******************************************************************************/

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "RexxCore.h"
#include "StringClass.hpp"
#include "ASCIIDBCSStrings.hpp"
#include "ArgumentUtilities.hpp"                     /* Gneral purpose BIF Header file    */
#include "Interpreter.hpp"

/*********************************************************************/
/*                                                                   */
/*  Name:           SkipBlanks                                       */
/*                                                                   */
/*  Descriptive name:       Skip leading blanks and decrement count  */
/*                                                                   */
/*********************************************************************/

void SkipBlanks(
  stringchar_t *    *String,                    /* point to advance           */
  stringsize_t   *StringLength )             /* string length              */
{
  stringchar_t *    Scan;                       /* scan pointer               */
  stringsize_t   Length;                     /* length to scan             */

  Scan = *String;                      /* point to data              */
  Length = *StringLength;              /* get the length             */

  for (;Length; Length--) {            /* scan entire string         */
    if (*Scan != ' ')                  /* if not a space             */
      break;                           /* just quit the loop         */
    Scan++;                            /* step to next character     */
  }
                                       /* fell through, all blanks   */
  *String = Scan;                      /* set pointer one past       */
  *StringLength = Length;              /* update the length          */
}

/*********************************************************************/
/*                                                                   */
/*  Name:           SkipNonBlanks                                    */
/*                                                                   */
/*  Descriptive name:       Skip leading nonblanks and decrement     */
/*                          count                                    */
/*                                                                   */
/*********************************************************************/

void SkipNonBlanks(
  stringchar_t *    *String,                    /* point to advance           */
  stringsize_t   *StringLength )             /* string length              */
{
  stringchar_t *    Scan;                       /* scan pointer               */
  stringsize_t   Length;                     /* length to scan             */

  Scan = *String;                      /* point to data              */
  Length = *StringLength;              /* get the length             */

  for (;Length; Length--) {            /* scan entire string         */
    if (*Scan == ' ')                  /* if not a space             */
      break;                           /* just quit the loop         */
    Scan++;                            /* step to next character     */
  }
                                       /* fell through, all blanks   */
  *String = Scan;                      /* set pointer one past       */
  *StringLength = Length;              /* update the length          */
}

/*********************************************************************/
/*                                                                   */
/*   Subroutine Name:   WordCount                                    */
/*                                                                   */
/*   Descriptive Name:  Count words in a string                      */
/*                                                                   */
/*   Function:          return the count of words in a string.       */
/*                      The string may contain leading or trailing   */
/*                      blanks or any number of interword blanks     */
/*                                                                   */
/*********************************************************************/

stringsize_t  WordCount(
  stringchar_t *    String,                     /* string of words            */
  stringsize_t   StringLength )              /* string length              */
{
  stringsize_t   Count;                      /* Current count of words     */

  Count = 0;                           /* default to nothing         */
  if (StringLength) {                  /* if not a null string       */
    SkipBlanks(&String, &StringLength);/* skip any leading blanks    */

    while (StringLength) {             /* while still string ...     */
      Count++;                         /* account for this word      */
                                       /* now skip the non-blanks    */
      SkipNonBlanks(&String, &StringLength);
      if (!StringLength)               /* if done with the string    */
        break;                         /* we are finished            */
                                       /* skip to the next word      */
      SkipBlanks(&String, &StringLength);
    }                                  /* loop while still have chars*/
  }
  return  Count;                       /* done looping, return the   */
                                       /* count of words             */
}

/*********************************************************************/
/*                                                                   */
/*   Subroutine Name:   NextWord                                     */
/*                                                                   */
/*   Descriptive Name:  find next word in string                     */
/*                                                                   */
/*   Function:          searches for the next word in a string from  */
/*                      the current position (the pointer is         */
/*                      unchanged if already at a nonblank           */
/*                      character).  The return value is the updated */
/*                      pointer and the length of the word.          */
/*                      Returns a length of zero if no word was      */
/*                      found.                                       */
/*                                                                   */
/*********************************************************************/

stringsize_t  NextWord(
  stringchar_t *  *String,                      /* input string               */
  stringsize_t *StringLength,                /* string length              */
  stringchar_t *  *NextString )                 /* next word position         */
{
  stringsize_t   WordStart;                  /* Starting point of word     */

  WordStart = 0;                       /* nothing moved yet          */
  if (*StringLength) {                 /* Something there?           */
    SkipBlanks(String, StringLength);  /* skip any leading blanks    */

    if (*StringLength) {               /* if still string ...        */
      WordStart = *StringLength;       /* save current length        */
      *NextString = *String;           /* save start position now    */
                                       /* skip the non-blanks        */
      SkipNonBlanks(NextString, StringLength);
      WordStart -= *StringLength;      /* adjust the word length     */
    }
  }
  return WordStart;                    /* return word length         */
}

/* the DELWORD function */
/******************************************************************************/
/* Arguments:  nth word to start deleting                                     */
/*             number of words to delete                                      */
/*                                                                            */
/*  Returned:  string, with length words deleted                              */
/******************************************************************************/
RexxString *RexxString::delWord(RexxInteger *position,
                                RexxInteger *plength)
{
  stringchar_t *       Current;                 /* current pointer position          */
  stringchar_t *       Word;                    /* current word pointer              */
  stringchar_t *       NextSite;                /* next word                         */
  stringsize_t      WordPos;                 /* needed word position              */
  stringsize_t      Count;                   /* count of words                    */
  stringsize_t      Length;                  /* remaining length                  */
  stringsize_t      WordLength;              /* word size                         */
  stringsize_t      FrontLength;             /* front substring                   */
  RexxString *Retval;                  /* return value                      */

  if (DBCS_SELF)                       /* need to use DBCS?                 */
                                       /* do the DBCS version               */
    return this->DBCSdelWord(position, plength);

                                       /* convert position to binary        */
  WordPos = positionArgument(position, ARG_ONE);
                                       /* get num of words to delete, the   */
                                       /* default is "a very large number"  */
  Count = optionalLengthArgument(plength, Numerics::MAX_WHOLENUMBER, ARG_TWO);

  Length = this->getLength();               /* get string length                 */
  if (!Length)                         /* null string?                      */
    Retval = OREF_NULLSTRING;          /* result is null also               */
  else if (!Count)                     /* deleting zero words?              */
    Retval = this;                     /* just use this string              */
  else {
    Word = (stringchar_t *)this->getStringData();    /* point to the string               */
                                       /* get the first word                */
    WordLength = NextWord(&Word, &Length, &NextSite);
    while (--WordPos && WordLength) {  /* loop until we reach tArget        */
      Word = NextSite;                 /* copy the start pointer            */
                                       /* get the next word                 */
      WordLength = NextWord(&Word, &Length, &NextSite);
    }
    if (WordPos)                       /* run out of words first            */
      Retval = this;                   /* return entire string              */
    else {                             /* count off number of words         */
                                       /* calculate front length            */
      FrontLength = Word - this->getStringData();
      while (--Count && WordLength) {  /* loop until we reach tArget        */
        Word = NextSite;               /* copy the start pointer            */
                                       /* get the next word                 */
        WordLength = NextWord(&Word, &Length, &NextSite);
      }
      if (Length)                      /* didn't use up the string          */
        SkipBlanks(&NextSite, &Length);/* skip over trailing blanks         */
                                       /* allocate return string            */
      Retval = raw_string(FrontLength + Length);
                                       /* point to data portion             */
      Current = Retval->getStringData();
      if (FrontLength) {               /* have a leading portion?           */
                                       /* copy into the result              */
        memcpy(Current, (stringchar_t *)this->getStringData(), FrontLength);
        Current += FrontLength;        /* step output position              */
      }
      if (Length)                      /* any string left?                  */
                                       /* copy what's left                  */
        memcpy(Current, NextSite, Length);
    }
  }
  return Retval;                       /* return deleted string             */
}

/* the SPACE function */
/******************************************************************************/
/* Arguments:  number of pad characters between each word                     */
/*             pad character                                                  */
/*                                                                            */
/*  Returned:  string                                                         */
/******************************************************************************/
RexxString *RexxString::space(RexxInteger *space_count,
                              RexxString  *pad)
{
  stringsize_t      Spaces;                  /* requested spacing                 */
  stringchar_t       PadChar;                 /* pad character                     */
  stringchar_t *       Current;                 /* current pointer position          */
  stringchar_t *       Word;                    /* current word pointer              */
  stringchar_t *       NextSite;                /* next word                         */
  stringsize_t      Count;                   /* count of words                    */
  stringsize_t      WordSize;                /* size of words                     */
  stringsize_t      Length;                  /* remaining length                  */
  stringsize_t      WordLength;              /* word size                         */
  RexxString *Retval;                  /* return value                      */

  if (DBCS_MODE)                       /* need to use DBCS?                 */
                                       /* do the DBCS version               */
    return this->DBCSspace(space_count, pad);

                                       /* get the spacing count             */
  Spaces = optionalLengthArgument(space_count, 1, ARG_ONE);

                                       /* get the pad character             */
  PadChar = padArgument(pad, ' ', ARG_TWO);

  Length = this->getLength();               /* get the string length             */
  Count = 0;                           /* no words yet                      */
  WordSize = 0;                        /* no characters either              */
  Word = (stringchar_t *)this->getStringData();      /* point to the string               */
                                       /* get the first word                */
  WordLength = NextWord(&Word, &Length, &NextSite);

  while (WordLength) {                 /* loop until we reach tArget        */
    Count++;                           /* count the word                    */
    WordSize += WordLength;            /* add in the word length            */
    Word = NextSite;                   /* copy the start pointer            */
                                       /* get the next word                 */
    WordLength = NextWord(&Word, &Length, &NextSite);
  }
  if (!Count)                          /* no words?                         */
    Retval = OREF_NULLSTRING;          /* this is a null string             */
  else {                               /* real words                        */
    Count--;                           /* step back one                     */
                                       /* get space for output              */
    Retval = raw_string(WordSize + Count * Spaces);
                                       /* point to output area              */
    Current = Retval->getStringData();

    Length = this->getLength();             /* recover the length                */
    Word = (stringchar_t *)this->getStringData();    /* point to the string               */
                                       /* get the first word                */
    WordLength = NextWord(&Word, &Length, &NextSite);

    while (Count--) {                  /* loop for each word                */
                                       /* copy the word over                */
      memcpy(Current, Word, WordLength);
      Current += WordLength;           /* step over the word                */
      if (Spaces) {                    /* if have gaps...                   */
                                       /* fill in the pad chars             */
        memset(Current, PadChar, Spaces);
        Current += Spaces;             /* step over the pad chars           */
      }
      Word = NextSite;                 /* copy the start pointer            */
                                       /* get the next word                 */
      WordLength = NextWord(&Word, &Length, &NextSite);
    }
                                       /* copy the word over                */
    memcpy(Current, Word, WordLength);
  }
  return Retval;                       /* return spaced string              */
}

/* the SUBWORD function */
/******************************************************************************/
/* Arguments:  Starting word postion                                          */
/*             number of words                                                */
/*                                                                            */
/*  Returned:  string, contains the requested number of words from source     */
/******************************************************************************/
RexxString *RexxString::subWord(RexxInteger *position,
                                RexxInteger *plength)
{
  stringchar_t *       Word;                    /* current word pointer              */
  stringchar_t *       WordStart;               /* start of substring                */
  stringchar_t *       WordEnd;                 /* end of the substring              */
  stringchar_t *       NextSite;                /* next word                         */
  stringsize_t      WordPos;                 /* needed word position              */
  stringsize_t      Count;                   /* count of words                    */
  stringsize_t      Length;                  /* remaining length                  */
  stringsize_t      WordLength;              /* word size                         */
  RexxString *Retval;                  /* return value                      */

  if (DBCS_SELF)                       /* need to use DBCS?                 */
                                       /* do the DBCS version               */
    return this->DBCSsubWord(position, plength);

                                       /* convert position to binary        */
  WordPos = positionArgument(position, ARG_ONE);
                                       /* get num of words to delete, the   */
                                       /* default is "a very large number"  */
  Count = optionalLengthArgument(plength, Numerics::MAX_WHOLENUMBER, ARG_TWO);

  Length = this->getLength();               /* get Argument length               */
  if (!Length || !Count)               /* null string?                      */
    Retval = OREF_NULLSTRING;          /* result is null also               */
  else {
    Word = (stringchar_t *)this->getStringData();    /* point to the string               */
                                       /* get the first word                */
    WordLength = NextWord(&Word, &Length, &NextSite);
    while (--WordPos && WordLength) {  /* loop until we reach tArget        */
      Word = NextSite;                 /* copy the start pointer            */
                                       /* get the next word                 */
      WordLength = NextWord(&Word, &Length, &NextSite);
    }
    if (WordPos)                       /* run out of words first            */
      Retval = OREF_NULLSTRING;        /* again a null string               */
    else {                             /* count off number of words         */
      WordStart = Word;                /* save start position               */
      WordEnd = Word;                  /* default end is the same           */
                                       /* loop until we reach tArget        */
      while (Count-- && WordLength) {
        WordEnd = Word + WordLength;   /* point to the word end             */
        Word = NextSite;               /* copy the start pointer            */
                                       /* get the next word                 */
        WordLength = NextWord(&Word, &Length, &NextSite);
      }
                                       /* extract the substring             */
      Retval = (RexxString *)new_string(WordStart, WordEnd - WordStart);
    }
  }
  return Retval;                       /* return extracted string           */
}



RexxArray *RexxString::extractWords(RexxInteger *position, RexxInteger *plength)
{
    // process the input arguments
    stringsize_t wordPos = optionalPositionArgument(position, ARG_ONE, 1);
    stringsize_t count = optionalLengthArgument(plength, Numerics::MAX_WHOLENUMBER, ARG_TWO);

    stringsize_t strLength = getLength();

    // if this is a null string, or we've been asked to return zero words, return an
    // empty array
    if (strLength == 0 || count == 0)
    {
        return new_array((arraysize_t)0);
    }


    // need to scan for words
    stringchar_t *word = getStringData();
    stringchar_t *nextSite = NULL;
                                       /* get the first word                */
    stringsize_t wordLength = NextWord(&word, &strLength, &nextSite);
    // ok, loop until we either run out of string or hit our target word position.
    while (--wordPos && wordLength != 0)
    {
        // just shift to the next word and rescan
        word = nextSite;
        wordLength = NextWord(&word, &strLength, &nextSite);
    }

    // didn't even make it to the target, so return an empty array
    if (wordPos > 0)
    {
        return new_array((arraysize_t)0);
    }

    // get a list accumulator
    RexxList *list = new_list();
    ProtectedObject p1(list);

    // now extract each word and add to our list
    while (count > 0 && wordLength > 0)
    {
        // add the current word to the accumulator as a string
        list->addLast(new_string(word, wordLength));
        // now scan for another word
        word = nextSite;
        wordLength = NextWord(&word, &wordLength, &nextSite);
    }
    // convert the accumulator to an array item.
    return list->makeArray();
}


/* the WORD function */
/******************************************************************************/
/* Arguments:  which word we want.                                            */
/*                                                                            */
/*  Returned:  string, containing nth word.                                   */
/******************************************************************************/
RexxString *RexxString::word(RexxInteger *position)
{
  stringchar_t *       Word;                    /* current word pointer              */
  stringchar_t *       NextSite;                /* next word                         */
  stringsize_t      WordPos;                 /* needed word position              */
  stringsize_t      Length;                  /* remaining length                  */
  stringsize_t      WordLength;              /* word size                         */
  RexxString *Retval;                  /* return value                      */

  if (DBCS_SELF)                       /* need to use DBCS?                 */
                                       /* do the DBCS version               */
    return this->DBCSword(position);

                                       /* convert position to binary        */
  WordPos = positionArgument(position, ARG_ONE);

  Length = this->getLength();               /* get Argument length               */
  if (!Length)                         /* null string?                      */
    Retval = OREF_NULLSTRING;          /* result is null also               */
  else {
    Word = (stringchar_t *)this->getStringData();    /* point to the string               */
                                       /* get the first word                */
    WordLength = NextWord(&Word, &Length, &NextSite);
    while (--WordPos && WordLength) {  /* loop until we reach target        */
      Word = NextSite;                 /* copy the start pointer            */
                                       /* get the next word                 */
      WordLength = NextWord(&Word, &Length, &NextSite);
    }
    if (WordLength)                    /* have a word                       */
                                       /* extract the string                */
      Retval = (RexxString *)new_string(Word, WordLength);
    else
      Retval = OREF_NULLSTRING;        /* no word, return a null            */
  }
  return Retval;                       /* return extracted string           */
}

/* the WORDINDEX function */
/******************************************************************************/
/* Arguments:  word we want position of.                                      */
/*                                                                            */
/*  Returned:  integer,  actual char position of nth word                     */
/******************************************************************************/
RexxInteger *RexxString::wordIndex(RexxInteger *position)
{
  stringchar_t *       Word;                    /* current word pointer              */
  stringchar_t *       NextSite;                /* next word                         */
  stringsize_t      Length;                  /* string length                     */
  stringsize_t      WordLength;              /* word length                       */
  stringsize_t      WordPos;                 /* desired word position             */
  RexxInteger *Retval;                 /* return value                      */
  stringsize_t      tempIndex;

  if (DBCS_SELF)                       /* need to use DBCS?                 */
                                       /* do the DBCS version               */
    return this->DBCSwordIndex(position);

  Length = this->getLength();               /* get the string length             */
                                       /* convert count to binary           */
  WordPos = positionArgument(position, ARG_ONE);
  Word = this->getStringData();             /* point to word data                */

                                       /* get the first word                */
  WordLength = NextWord(&Word, &Length, &NextSite);
  while (--WordPos && WordLength) {    /* loop until we reach target        */
    Word = NextSite;                   /* copy the start pointer            */
                                       /* get the next word                 */
    WordLength = NextWord(&Word, &Length, &NextSite);
  }
  if (WordLength == 0)                 /* ran out of string                 */
    Retval = IntegerZero;              /* no index                          */
  else {                               /* calc the word index               */
    tempIndex = Word - this->getStringData() + 1;
    Retval = new_integer(tempIndex);
  }
  return Retval;                       /* return appopriate position        */
}

/* the WORDLENGTH function */
/******************************************************************************/
/* Arguments:  nth word we want length of                                     */
/*                                                                            */
/*  Returned:  integer, length of nth word                                    */
/******************************************************************************/
RexxInteger *RexxString::wordLength(RexxInteger *position)
{
  stringchar_t *       Word;                    /* current word pointer              */
  stringchar_t *       NextSite;                /* next word                         */
  stringsize_t      Length;                  /* string length                     */
  stringsize_t      WordLength;              /* word length                       */
  stringsize_t      WordPos;                 /* desired word position             */

  if (DBCS_SELF)                       /* need to use DBCS?                 */
                                       /* do the DBCS version               */
    return this->DBCSwordLength(position);

  Length = this->getLength();               /* get the string length             */
                                       /* convert count to binary           */
  WordPos = positionArgument(position , ARG_ONE);
  Word = this->getStringData();             /* point to word data                */

                                       /* get the first word                */
  WordLength = NextWord(&Word, &Length, &NextSite);
  while (--WordPos && WordLength) {    /* loop until we reach target        */
    Word = NextSite;                   /* copy the start pointer            */
                                       /* get the next word                 */
    WordLength = NextWord(&Word, &Length, &NextSite);
  }
  return new_integer(WordLength);      /* return the word length            */
}

/* the WORDPOS function */
/******************************************************************************/
/* Arguments:  phrase of words to lookfor                                     */
/*             which word to satrt looking at.                                */
/*                                                                            */
/*  Returned:  position of 1st word in source matching phrase                 */
/******************************************************************************/
RexxInteger *RexxString::wordPos(RexxString  *phrase,
                                 RexxInteger *pstart)
{
  RexxInteger *Retval;                 /* return value                      */
  stringchar_t *    Needle;                     /* start of needle string            */
  stringchar_t *    Haystack;                   /* current haystack positon          */
  stringchar_t *    NextNeedle;                 /* next search position              */
  stringchar_t *    NextHaystack;               /* next search position              */
  stringsize_t   Count;                      /* current haystack word pos         */
  stringsize_t   NeedleWords;                /* needle word count                 */
  stringsize_t   HaystackWords;              /* haystack word count               */
  stringsize_t   HaystackScanLength;         /* length to scan                    */
  stringsize_t   NeedleScanLength;           /* length of scan                    */
  stringsize_t   HaystackWordLength;         /* haystack word length              */
  stringsize_t   NeedleWordLength;           /* needle word length                */
  stringsize_t   SearchCount;                /* possible searches to do           */
  stringsize_t   FirstNeedle;                /* length of first needle word       */
  stringsize_t   NeedleLength;               /* length of needle                  */
  stringsize_t   HaystackLength;             /* length of haystack                */
  stringchar_t *    NeedlePosition;             /* temporary pointers for            */
  stringchar_t *    HaystackPosition;           /* the searches                      */
  stringchar_t *    NextHaystackPtr;            /* pointer to next word              */
  stringchar_t *    NextNeedlePtr;              /* pointer to next word              */
  stringsize_t   i;                          /* loop counter                      */

  if (DBCS_MODE)                       /* need to use DBCS?                 */
                                       /* do the DBCS version               */
    return this->DBCSwordPos(phrase, pstart);

  phrase = stringArgument(phrase, ARG_ONE);/* get the phrase we are looking for */
  NeedleLength = phrase->getLength();       /* get the length also               */
                                       /* get starting position, the default*/
                                       /* is the first word                 */
  Count = optionalPositionArgument(pstart, 1, ARG_TWO);

  Needle = phrase->getStringData();         /* get friendly pointer              */
  Haystack = (stringchar_t *)this->getStringData();  /* and the second also               */
  HaystackLength = this->getLength();       /* get the haystack length           */
                                       /* count the words in needle         */
  NeedleWords = WordCount(Needle, NeedleLength);
                                       /* and haystack                      */
  HaystackWords = WordCount(Haystack, HaystackLength);
                                       /* if search string is longer        */
                                       /* or no words in search             */
                                       /* or count is longer than           */
                                       /* haystack                          */
  if (NeedleWords > (HaystackWords - Count + 1) ||
      NeedleWords == 0 ||
      Count > HaystackWords) {
    Retval = IntegerZero;              /* can't find anything,              */
  }
  else {                               /* need to search                    */
                                       /* point at first word               */
    HaystackWordLength = NextWord(&Haystack, &HaystackLength, &NextHaystack);
                                       /* now skip over count-1             */
    for (i = Count - 1; i && HaystackWordLength != 0; i--) {
      Haystack = NextHaystack;         /* step past current word            */
                                       /* find the next word                */
      HaystackWordLength = NextWord(&Haystack, &HaystackLength, &NextHaystack);
    }
                                       /* get number of searches            */
    SearchCount = (HaystackWords - NeedleWords - Count) + 2;
                                       /* position at first needle          */
    FirstNeedle = NextWord(&Needle, &NeedleLength, &NextNeedle);
                                       /* loop for the possible times       */
    for (; SearchCount; SearchCount--) {
      NeedleWordLength = FirstNeedle;  /* set the length                    */
      NeedlePosition = Needle;         /* get the start of phrase           */
      HaystackPosition = Haystack;     /* and the target string loop        */
                                       /* for needlewords                   */
      NextHaystackPtr = NextHaystack;  /* copy nextword information         */
      NextNeedlePtr = NextNeedle;
                                       /* including the lengths             */
      HaystackScanLength = HaystackLength;
      NeedleScanLength = NeedleLength;

      for (i = NeedleWords; i; i--) {  /* possible times                    */

        if (HaystackWordLength !=      /* if wrong length, then it          */
            NeedleWordLength)          /* can't be a match...just           */
          break;                       /* leave the loop                    */

        if (memcmp(NeedlePosition,     /* if the two words don't            */
            HaystackPosition,          /* match, then no sense              */
            NeedleWordLength))         /* checking the rest                 */
          break;                       /* get out fast.                     */

                                       /* the last words matched, so        */
                                       /* continue searching.               */

                                       /* set new search information        */
        HaystackPosition = NextHaystackPtr;
        NeedlePosition = NextNeedlePtr;
                                       /* Scan off the next word            */
        HaystackWordLength = NextWord(&HaystackPosition,
                                      &HaystackScanLength,
                                      &NextHaystackPtr);
                                       /* repeat for the needle             */
        NeedleWordLength = NextWord(&NeedlePosition,
                                    &NeedleScanLength,
                                    &NextNeedlePtr);
      }

      if (!i)                          /* all words matched, we             */
        break;                         /* found it                          */
      Haystack = NextHaystack;         /* set the search position           */
                                       /* step to next haytack pos          */
      HaystackWordLength = NextWord(&Haystack,
                                    &HaystackLength,
                                    &NextHaystack);
      Count++;                         /* remember the word position        */
    }

    if (SearchCount)                   /* if we haven't scanned the         */
                                       /* entire string                     */
    Retval = new_integer(Count);       /* return the position               */

    else                               /* it wasn't found, just             */
      Retval = IntegerZero;            /* return a zero.                    */
  }
  return Retval;                       /* return the position               */
}

/* the WORDS function */
/******************************************************************************/
/* Arguments:  none                                                           */
/*                                                                            */
/*  Returned:  integer, number os words in source                             */
/******************************************************************************/
RexxInteger *RexxString::words()
{
  stringsize_t tempCount;
  if (DBCS_SELF)                       /* need to use DBCS?                 */
                                       /* do the DBCS version               */
    return this->DBCSwords();

                                       /* return count of words      */
  tempCount = WordCount((stringchar_t *)this->getStringData(), this->getLength());
  return new_integer(tempCount);
}


