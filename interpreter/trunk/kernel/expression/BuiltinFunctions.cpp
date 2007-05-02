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
/* REXX Translator                                              BuiltinFunctions.c      */
/*                                                                            */
/* Builtin Function Execution Stubs                                           */
/*                                                                            */
/******************************************************************************/

#include <stdlib.h>
#include <ctype.h>
#include "RexxCore.h"
#include "StringClass.hpp"
#include "DirectoryClass.hpp"
#include "ArrayClass.hpp"
#include "RexxVariableDictionary.hpp"
#include "RexxActivation.hpp"
#include "RexxActivity.hpp"
#include "ExpressionBaseVariable.hpp"
#include "SourceFile.hpp"
#include "BuiltinFunctions.hpp"
#include "Numerics.hpp"
#include "SysInterpreter.hpp"
#include "Interpreter.hpp"
#include "Package.hpp"


/* checks if pad is a single character string */
inline void checkPadArgument(stringchar_t *pFuncName, RexxObject *position, RexxString *pad)
{
  if (pad == OREF_NULL) return;
  if (pad->getLength() != 1)
    reportException(Error_Incorrect_call_pad, new_string(pFuncName), position, pad);
}

#define CENTER_MIN 2
#define CENTER_MAX 3
#define CENTER_string 1
#define CENTER_length 2
#define CENTER_pad    3

BUILTIN(CENTER) {
  RexxString  *string;                 /* target centered string            */
  RexxInteger *length;                 /* target string length              */
  RexxString  *pad;                    /* optional pad character            */

  fixArgs(CENTER);                    /* check on required number of args  */
                                       /* force first argument to a string  */
  string = requiredString(CENTER, string);
                                       /* this is a required length         */
  length = requiredInteger(CENTER, length);
  pad = optionalString(CENTER, pad);  /* the pad character must be one too */
  checkPadArgument(CHAR_CENTER, IntegerThree, pad);
  return string->center(length, pad);  /* do the center function            */
}

#define CENTRE_MIN 2
#define CENTRE_MAX 3
#define CENTRE_string 1
#define CENTRE_length 2
#define CENTRE_pad    3

BUILTIN(CENTRE) {
  RexxString  *string;                 /* target string                     */
  RexxInteger *length;                 /* target string length              */
  RexxString  *pad;                    /* optional pad character            */

  fixArgs(CENTRE);                    /* check on required number of args  */
                                       /* force first argument to a string  */
  string = requiredString(CENTRE, string);
                                       /* this is a required length         */
  length = requiredInteger(CENTRE, length);
  pad = optionalString(CENTRE, pad);  /* the pad character must be one too */
  checkPadArgument(CHAR_CENTRE, IntegerThree, pad);
  return string->center(length, pad);  /* do the center function            */
}

#define DELSTR_MIN 2
#define DELSTR_MAX 3
#define DELSTR_string 1
#define DELSTR_n      2
#define DELSTR_length 3

BUILTIN(DELSTR) {
  RexxString  *string;                 /* target string                     */
  RexxInteger *n;                      /* start position                    */
  RexxInteger *length;                 /* target string length              */

  fixArgs(DELSTR);                    /* check on required number of args  */
                                       /* must have the first argument      */
  string = requiredString(DELSTR, string);
  n = requiredInteger(DELSTR, n);     /* need a delete position            */
                                       /* length is optional                */
  length = optionalInteger(DELSTR, length);
  return string->delstr(n, length);    /* do the delstr function            */
}

#define DELWORD_MIN 2
#define DELWORD_MAX 3
#define DELWORD_string 1
#define DELWORD_n      2
#define DELWORD_length 3

BUILTIN(DELWORD) {
  RexxString  *string;                 /* target string                     */
  RexxInteger *n;                      /* start position                    */
  RexxInteger *length;                 /* target string length              */

  fixArgs(DELWORD);                   /* check on required number of args  */
                                       /* must have the first argument      */
  string = requiredString(DELWORD, string);
  n = requiredInteger(DELWORD, n);    /* need a delete position            */
                                       /* length is optional                */
  length = optionalInteger(DELWORD, length);
  return string->delWord(n, length);   /* do the delword function           */
}

#define INSERT_MIN 2
#define INSERT_MAX 5
#define INSERT_new    1
#define INSERT_target 2
#define INSERT_n      3
#define INSERT_length 4
#define INSERT_pad    5

BUILTIN(INSERT) {
  RexxString  *newString;              /* new string to insert              */
  RexxString  *target;                 /* target string                     */
  RexxInteger *n;                      /* start position                    */
  RexxInteger *length;                 /* target string length              */
  RexxString  *pad;                    /* optional pad character            */

  fixArgs(INSERT);                    /* check on require number of args   */
                                       /* get string for new                */
  newString = requiredString(INSERT, new);
                                       /* get string for target             */
  target = requiredString(INSERT, target);
  n = optionalInteger(INSERT, n);     /* insert position is optional       */
                                       /* length is optional                */
  length = optionalInteger(INSERT, length);
  pad = optionalString(INSERT, pad);  /* get string for pad                */
                                       /* go perform the insert function    */
  checkPadArgument(CHAR_INSERT, IntegerFour, pad);
  return target->insert(newString, n, length, pad);
}

#define LEFT_MIN 2
#define LEFT_MAX 3
#define LEFT_string 1
#define LEFT_length 2
#define LEFT_pad    3

BUILTIN(LEFT) {
  RexxString  *string;                 /* target string                     */
  RexxInteger *length;                 /* target string length              */
  RexxString  *pad;                    /* optional pad character            */

  fixArgs(LEFT);                      /* check on required number of args  */
                                       /* must have the first argument      */
  string = requiredString(LEFT, string);
                                       /* length is optional                */
  length = optionalInteger(LEFT, length);
  pad = optionalString(LEFT, pad);    /* pad must be a string also         */
  checkPadArgument(CHAR_LEFT, IntegerThree, pad);
  return string->left(length, pad);    /* do the substr function            */
}

#define OVERLAY_MIN 2
#define OVERLAY_MAX 5
#define OVERLAY_new    1
#define OVERLAY_target 2
#define OVERLAY_n      3
#define OVERLAY_length 4
#define OVERLAY_pad    5

BUILTIN(OVERLAY) {
  RexxString  *newString;              /* new string to overlay             */
  RexxString  *target;                 /* target string                     */
  RexxInteger *n;                      /* start position                    */
  RexxInteger *length;                 /* target string length              */
  RexxString  *pad;                    /* optional pad character            */

  fixArgs(OVERLAY);                   /* check on require number of args   */
                                       /* get string for new                */
  newString = requiredString(OVERLAY, new);
                                       /* get string for target             */
  target = requiredString(OVERLAY, target);
  n = optionalInteger(OVERLAY, n);    /* overlay position is optional      */
                                       /* length is optional                */
  length = optionalInteger(OVERLAY, length);
  pad = optionalString(OVERLAY, pad); /* get string for pad                */
                                       /* go perform the overlay function   */
  checkPadArgument(CHAR_OVERLAY, IntegerFive, pad);
  return target->overlay(newString, n, length, pad);
}

#define POS_MIN 2
#define POS_MAX 3
#define POS_needle   1
#define POS_haystack 2
#define POS_start    3

BUILTIN(POS) {
  RexxString  *needle;                 /* search needle                     */
  RexxString  *haystack;               /* string to search in               */
  RexxInteger *start;                  /* the start position                */

  fixArgs(POS);                       /* check on require number of args   */
                                       /* get string for new                */
  needle = requiredString(POS, needle);
                                       /* get string for target             */
  haystack = requiredString(POS, haystack);
  start = optionalInteger(POS, start);/* start position is optional        */
                                       /* go perform the pos function       */
  return haystack->posRexx(needle, start);
}

#define LASTPOS_MIN 2
#define LASTPOS_MAX 3
#define LASTPOS_needle   1
#define LASTPOS_haystack 2
#define LASTPOS_start    3

BUILTIN(LASTPOS) {
  RexxString  *needle;                 /* search needle                     */
  RexxString  *haystack;               /* string to search in               */
  RexxInteger *start;                  /* the start position                */

  fixArgs(LASTPOS);                   /* check on require number of args   */
                                       /* get string for new                */
  needle = requiredString(LASTPOS, needle);
                                       /* get string for target             */
  haystack = requiredString(LASTPOS, haystack);
                                       /* start position is optional        */
  start = optionalInteger(LASTPOS, start);
                                       /* go perform the lastpos function   */
  return haystack->lastPosRexx(needle, start);
}

#define REVERSE_MIN 1
#define REVERSE_MAX 1
#define REVERSE_string 1

BUILTIN(REVERSE) {
  RexxString  *string;                 /* target string                     */

  fixArgs(REVERSE);                   /* check on require number of args   */
                                       /* get string for string             */
  string = requiredString(REVERSE, string);
  return string->reverse();            /* go perform the reverse function   */
}

#define RIGHT_MIN 2
#define RIGHT_MAX 3
#define RIGHT_string 1
#define RIGHT_length 2
#define RIGHT_pad    3

BUILTIN(RIGHT) {
  RexxString  *string;                 /* target string                     */
  RexxInteger *length;                 /* target string length              */
  RexxString  *pad;                    /* optional pad character            */

  fixArgs(RIGHT);                     /* check on required number of args  */
                                       /* must have the first argument      */
  string = requiredString(RIGHT, string);
                                       /* length is optional                */
  length = optionalInteger(RIGHT, length);
  pad = optionalString(RIGHT, pad);   /* pad must be a string also         */
  checkPadArgument(CHAR_RIGHT, IntegerThree, pad);
  return string->right(length, pad);   /* do the substr function            */
}

#define STRIP_MIN 1
#define STRIP_MAX 3
#define STRIP_string 1
#define STRIP_option 2
#define STRIP_char   3

BUILTIN(STRIP) {
  RexxString  *string;                 /* target string                     */
  RexxString  *option;                 /* function option                   */
  RexxString  *character;              /* character to strip                */

  fixArgs(STRIP);                     /* check on required number of args  */
                                       /* must have the first argument      */
  string = requiredString(STRIP, string);
                                       /* option must be a string too       */
  option = optionalString(STRIP, option);
                                       /* as is char as well                */
  character = optionalString(STRIP, char);
                                       /* do the strip function             */
  return string->strip(option, character);
}

#define SPACE_MIN 1
#define SPACE_MAX 3
#define SPACE_string 1
#define SPACE_n      2
#define SPACE_pad    3


BUILTIN(SPACE) {
  RexxString  *string;                 /* target string                     */
  RexxInteger *n;                      /* inter-word spaces                 */
  RexxString  *pad;                    /* optional pad character            */

  fixArgs(SPACE);                     /* check on required number of args  */
                                       /* must have the first argument      */
  string = requiredString(SPACE, string);
  n = optionalInteger(SPACE, n);      /* spacing is an optional integer    */
  pad = optionalString(SPACE, pad);   /* pad must be a string also         */
  checkPadArgument(CHAR_SPACE, IntegerThree, pad);
  return string->space(n, pad);        /* do the space function             */
}

#define SUBSTR_MIN 2
#define SUBSTR_MAX 4
#define SUBSTR_string 1
#define SUBSTR_n      2
#define SUBSTR_length 3
#define SUBSTR_pad    4


BUILTIN(SUBSTR) {
  RexxString  *string;                 /* target string                     */
  RexxInteger *n;                      /* start position                    */
  RexxInteger *length;                 /* target string length              */
  RexxString  *pad;                    /* optional pad character            */

  fixArgs(SUBSTR);                    /* check on required number of args  */
                                       /* must have the first argument      */
  string = requiredString(SUBSTR, string);
  n = requiredInteger(SUBSTR, n);     /* position is required              */
                                       /* length is optional                */
  length = optionalInteger(SUBSTR, length);
  pad = optionalString(SUBSTR, pad);  /* pad must be a string also         */
                                       /* do the substr function            */
  checkPadArgument(CHAR_SUBSTR, IntegerFour, pad);
  return string->substr(n, length, pad);
}

#define LOWER_MIN 2
#define LOWER_MAX 3
#define LOWER_string 1
#define LOWER_n      2
#define LOWER_length 3


BUILTIN(LOWER) {
  RexxString  *string;                 /* target string                     */
  RexxInteger *n;                      /* start position                    */
  RexxInteger *length;                 /* target string length              */

  fixArgs(LOWER);                      /* check on required number of args  */
                                       /* must have the first argument      */
  string = requiredString(LOWER, string);
  n = optionalInteger(LOWER, n);       /* position is optional              */
                                       /* length is optional                */
  length = optionalInteger(LOWER, length);
                                       /* do the LOWER function            */
  return string->lowerRexx(n, length);
}


#define UPPER_MIN 2
#define UPPER_MAX 3
#define UPPER_string 1
#define UPPER_n      2
#define UPPER_length 3


BUILTIN(UPPER) {
  RexxString  *string;                 /* target string                     */
  RexxInteger *n;                      /* start position                    */
  RexxInteger *length;                 /* target string length              */

  fixArgs(UPPER);                      /* check on required number of args  */
                                       /* must have the first argument      */
  string = requiredString(UPPER, string);
  n = optionalInteger(UPPER, n);       /* position is optional              */
                                       /* length is optional                */
  length = optionalInteger(UPPER, length);
                                       /* do the UPPER function            */
  return string->upperRexx(n, length);
}


#define SUBWORD_MIN 2
#define SUBWORD_MAX 3
#define SUBWORD_string 1
#define SUBWORD_n      2
#define SUBWORD_length 3

BUILTIN(SUBWORD) {
  RexxString  *string;                 /* target string                     */
  RexxInteger *n;                      /* required start                    */
  RexxInteger *length;                 /* target string length              */

  fixArgs(SUBWORD);                   /* check on required number of args  */
                                       /* must have the first argument      */
  string = requiredString(SUBWORD, string);
  n = requiredInteger(SUBWORD, n);    /* position is required              */
                                       /* length is optional                */
  length = optionalInteger(SUBWORD, length);
  return string->subWord(n, length);   /* do the subword function           */
}

#define WORD_MIN 2
#define WORD_MAX 2
#define WORD_string 1
#define WORD_n      2

BUILTIN(WORD) {
  RexxString  *string;                 /* target string                     */
  RexxInteger *n;                      /* start position                    */

  fixArgs(WORD);                      /* check on required number of args  */
                                       /* must have the first argument      */
  string = requiredString(WORD, string);
  n = requiredInteger(WORD, n);       /* position is required              */
  return string->word(n);              /* do the word function              */
}

#define WORDINDEX_MIN 2
#define WORDINDEX_MAX 2
#define WORDINDEX_string 1
#define WORDINDEX_n      2

BUILTIN(WORDINDEX) {
  RexxString  *string;                 /* target string                     */
  RexxInteger *n;                      /* word number                       */

  fixArgs(WORDINDEX);                 /* check on required number of args  */
                                       /* must have the first argument      */
  string = requiredString(WORDINDEX, string);
  n = requiredInteger(WORDINDEX, n);  /* position is required              */
  return string->wordIndex(n);         /* do the wordindex function         */
}

#define WORDLENGTH_MIN 2
#define WORDLENGTH_MAX 2
#define WORDLENGTH_string 1
#define WORDLENGTH_n      2

BUILTIN(WORDLENGTH) {
  RexxString  *string;                 /* target string                     */
  RexxInteger *n;                      /* start position                    */

  fixArgs(WORDLENGTH);                /* check on required number of args  */
                                       /* must have the first argument      */
  string = requiredString(WORDLENGTH, string);
  n = requiredInteger(WORDLENGTH, n); /* position is required              */
  return string->wordLength(n);        /* do the wordlength function        */
}

#define COPIES_MIN 2
#define COPIES_MAX 2
#define COPIES_string 1
#define COPIES_n      2

BUILTIN(COPIES) {
  RexxString  *string;                 /* target string                     */
  RexxInteger *n;                      /* number of copies                  */

  fixArgs(COPIES);                    /* check on required number of args  */
                                       /* must have the first argument      */
  string = requiredString(COPIES, string);
  n = requiredInteger(COPIES, n);     /* position is required              */
  return string->copies(n);            /* do the copies function            */
}

#define WORDPOS_MIN 2
#define WORDPOS_MAX 3
#define WORDPOS_phrase 1
#define WORDPOS_string 2
#define WORDPOS_start  3

BUILTIN(WORDPOS) {
  RexxString  *string;                 /* target string                     */
  RexxString  *phrase;                 /* search phrase                     */
  RexxInteger *start;                  /* start position                    */

  fixArgs(WORDPOS);                   /* check on required number of args  */
                                       /* must have a phrase string         */
  phrase = requiredString(WORDPOS, phrase);
                                       /* must have the string argument     */
  string = requiredString(WORDPOS, string);
                                       /* start position is optional        */
  start = optionalInteger(WORDPOS, start);
                                       /* do the wordpos function           */
  return string->wordPos(phrase, start);
}

#define WORDS_MIN 1
#define WORDS_MAX 1
#define WORDS_string 1

BUILTIN(WORDS) {
  RexxString  *string;                 /* target string                     */

  fixArgs(WORDS);                     /* check on required number of args  */
                                       /* must have the string argument     */
  string = requiredString(WORDS, string);
  return string->words();              /* do the words function             */
}

#define ABBREV_MIN 2
#define ABBREV_MAX 3
#define ABBREV_information 1
#define ABBREV_info        2
#define ABBREV_length      3

BUILTIN(ABBREV) {
  RexxString  *information;            /* information to check              */
  RexxString  *info;                   /* target abbreviation               */
  RexxInteger *length;                 /* target string length              */

  fixArgs(ABBREV);                    /* check on required number of args  */
                                       /* information must be a string arg  */
  information = requiredString(ABBREV, information);
  info = requiredString(ABBREV, info);/* info must also be a string        */
                                       /* length is optional                */
  length = optionalInteger(ABBREV, length);
                                       /* check on the abbreviation         */
  return information->abbrev(info, length);
}

#define BITAND_MIN 1
#define BITAND_MAX 3
#define BITAND_string1 1
#define BITAND_string2 2
#define BITAND_pad     3

BUILTIN(BITAND) {
  RexxString  *string1;                /* first string                      */
  RexxString  *string2;                /* second string                     */
  RexxString  *pad;                    /* optional pad character            */

  fixArgs(BITAND);                    /* check on required number of args  */
                                       /* must have the first string        */
  string1 = requiredString(BITAND, string1);
                                       /* second string is optional         */
  string2 = optionalString(BITAND, string2);
  pad = optionalString(BITAND, pad);  /* pad is optional also              */
  checkPadArgument(CHAR_BITAND, IntegerThree, pad);
  return string1->bitAnd(string2, pad);/* do the bitand function            */
}

#define BITOR_MIN 1
#define BITOR_MAX 3
#define BITOR_string1 1
#define BITOR_string2 2
#define BITOR_pad     3

BUILTIN(BITOR) {
  RexxString  *string1;                /* first string                      */
  RexxString  *string2;                /* second string                     */
  RexxString  *pad;                    /* optional pad character            */

  fixArgs(BITOR);                     /* check on required number of args  */
                                       /* must have the first string        */
  string1 = requiredString(BITOR, string1);
                                       /* second string is optional         */
  string2 = optionalString(BITOR, string2);
  pad = optionalString(BITOR, pad);   /* pad is optional also              */
  checkPadArgument(CHAR_BITOR, IntegerThree, pad);
  return string1->bitOr(string2, pad); /* do the bitor function             */
}

#define BITXOR_MIN 1
#define BITXOR_MAX 3
#define BITXOR_string1 1
#define BITXOR_string2 2
#define BITXOR_pad     3

BUILTIN(BITXOR) {
  RexxString  *string1;                /* first string                      */
  RexxString  *string2;                /* second string                     */
  RexxString  *pad;                    /* optional pad character            */

  fixArgs(BITXOR);                    /* check on required number of args  */
                                       /* must have the first string        */
  string1 = requiredString(BITXOR, string1);
                                       /* second string is optional         */
  string2 = optionalString(BITXOR, string2);
  pad = optionalString(BITXOR, pad);  /* pad is optional also              */
  checkPadArgument(CHAR_BITXOR, IntegerThree, pad);
  return string1->bitXor(string2, pad);/* do the bitxor function            */
}

#define B2X_MIN 1
#define B2X_MAX 1
#define B2X_string  1

BUILTIN(B2X) {
  RexxString  *string;                 /* target string                     */

  fixArgs(B2X);                       /* check on required number of args  */
                                       /* must have the first string        */
  string = requiredString(B2X, string);
  return string->b2x();                /* do the b2x function               */
}

#define X2B_MIN 1
#define X2B_MAX 1
#define X2B_string  1

BUILTIN(X2B) {
  RexxString  *string;                 /* target string                     */

  fixArgs(X2B);                       /* check on required number of args  */
                                       /* must have the first string        */
  string = requiredString(X2B, string);
  return string->x2b();                /* do the x2b function               */
}

#define C2X_MIN 1
#define C2X_MAX 1
#define C2X_string  1

BUILTIN(C2X) {
  RexxString  *string;                 /* target string                     */

  fixArgs(C2X);                       /* check on required number of args  */
                                       /* must have the first string        */
  string = requiredString(C2X, string);
  return string->c2x();                /* do the c2x function               */
}

#define X2C_MIN 1
#define X2C_MAX 1
#define X2C_string  1

BUILTIN(X2C) {
  RexxString  *string;                 /* target string                     */

  fixArgs(X2C);                       /* check on required number of args  */
                                       /* must have the first string        */
  string = requiredString(X2C, string);
  return string->x2c();                /* do the x2c function               */
}

#define C2D_MIN 1
#define C2D_MAX 2
#define C2D_string  1
#define C2D_n       2

BUILTIN(C2D) {
  RexxString  *string;                 /* target string                     */
  RexxInteger *n;                      /* start position                    */

  fixArgs(C2D);                       /* check on required number of args  */
                                       /* must have the first string        */
  string = requiredString(C2D, string);
  n = optionalInteger(C2D, n);        /* length is optional                */
  return string->c2d(n);               /* do the c2d function               */
}

#define TRUNC_MIN 1
#define TRUNC_MAX 2
#define TRUNC_number  1
#define TRUNC_n       2

BUILTIN(TRUNC) {
  RexxString  *number;                 /* number to truncate                */
  RexxInteger *n;                      /* digits to use                     */

  fixArgs(TRUNC);                     /* check on required number of args  */
                                       /* must have the first string        */
  number = requiredString(TRUNC, number);
  n = optionalInteger(TRUNC, n);      /* length is optional                */
  return number->trunc(n);             /* do the trunc function             */
}

#define X2D_MIN 1
#define X2D_MAX 2
#define X2D_string  1
#define X2D_n       2

BUILTIN(X2D) {
  RexxString  *string;                 /* target string                     */
  RexxInteger *n;                      /* start position                    */

  fixArgs(X2D);                       /* check on required number of args  */
                                       /* must have the first string        */
  string = requiredString(X2D, string);
  n = optionalInteger(X2D, n);        /* length is optional                */
  return string->x2d(n);               /* do the x2d function               */
}

#define D2X_MIN 1
#define D2X_MAX 2
#define D2X_string  1
#define D2X_n       2

BUILTIN(D2X) {
  RexxString  *string;                 /* target string                     */
  RexxInteger *n;                      /* conversion length                 */

  fixArgs(D2X);                       /* check on required number of args  */
                                       /* must have the first string        */
  string = requiredString(D2X, string);
  n = optionalInteger(D2X, n);        /* length is optional                */
  return string->d2x(n);               /* do the x2d function               */
}

#define D2C_MIN 1
#define D2C_MAX 2
#define D2C_string  1
#define D2C_n       2

BUILTIN(D2C) {
  RexxString  *string;                 /* target string                     */
  RexxInteger *n;                      /* start position                    */

  fixArgs(D2C);                       /* check on required number of args  */
                                       /* must have the first string        */
  string = requiredString(D2C, string);
  n = optionalInteger(D2C, n);        /* length is optional                */
  return string->d2c(n);               /* do the x2d function               */
}

#define COMPARE_MIN 2
#define COMPARE_MAX 3
#define COMPARE_string1 1
#define COMPARE_string2 2
#define COMPARE_pad     3

BUILTIN(COMPARE) {
  RexxString  *string1;                /* first comparison string           */
  RexxString  *string2;                /* second comparison string          */
  RexxString  *pad;                    /* optional pad character            */

  fixArgs(COMPARE);                   /* check on required number of args  */
                                       /* must have the first string        */
  string1 = requiredString(COMPARE, string1);
                                       /* and the second string also        */
  string2 = requiredString(COMPARE, string2);
  pad = optionalString(COMPARE, pad); /* padding is optional               */
                                       /* do the comparison                 */
  checkPadArgument(CHAR_COMPARE, IntegerThree, pad);
  return string1->compare(string2, pad);
}

#define LENGTH_MIN 1
#define LENGTH_MAX 1
#define LENGTH_string  1

BUILTIN(LENGTH) {
  RexxString  *target;                 /* target of the operation           */

  fixArgs(LENGTH);                     /* check on required number of args  */
                                       /* must have a string                */
  target = requiredString(LENGTH, string);
  return target->lengthRexx();         /* get the length                    */
}

#define TRANSLATE_MIN 1
#define TRANSLATE_MAX 4
#define TRANSLATE_string  1
#define TRANSLATE_tableo  2
#define TRANSLATE_tablei  3
#define TRANSLATE_pad     4

BUILTIN(TRANSLATE) {
  RexxString  *string;                 /* target string                     */
  RexxString  *tableo;                 /* output table                      */
  RexxString  *tablei;                 /* input table                       */
  RexxString  *pad;                    /* optional pad character            */

  fixArgs(TRANSLATE);                 /* check on required number of args  */
                                       /* must have a string                */
  string = requiredString(TRANSLATE, string);
                                       /* output table is optional          */
  tableo = optionalString(TRANSLATE, tableo);
                                       /* input table is optional           */
  tablei = optionalString(TRANSLATE, tablei);
                                       /* pad is also optional              */
  pad = optionalString(TRANSLATE, pad);
                                       /* perform the translate             */
  checkPadArgument(CHAR_TRANSLATE, IntegerFour, pad);
  return string->translate(tableo, tablei, pad);
}

#define VERIFY_MIN 2
#define VERIFY_MAX 4
#define VERIFY_string    1
#define VERIFY_reference 2
#define VERIFY_option    3
#define VERIFY_start     4

BUILTIN(VERIFY) {
  RexxString  *string;                 /* target string                     */
  RexxString  *reference;              /* reference characters              */
  RexxString  *option;                 /* function option                   */
  RexxInteger *start;                  /* start position                    */

  fixArgs(VERIFY);                    /* check on required number of args  */
                                       /* must have a string                */
  string = requiredString(VERIFY, string);
                                       /* reference is also required        */
  reference = requiredString(VERIFY, reference);
                                       /* the options are optional          */
  option = optionalString(VERIFY, option);
                                       /* start is optional                 */
  start = optionalInteger(VERIFY, start);
                                       /* do the verify function            */
  return string->verify(reference, option, start);
}

#define DATATYPE_MIN 1
#define DATATYPE_MAX 2
#define DATATYPE_string    1
#define DATATYPE_type      2

BUILTIN(DATATYPE) {
  RexxString  *string;                 /* target string                     */
  RexxString  *type;                   /* type to check against             */

  fixArgs(DATATYPE);                  /* check on required number of args  */
                                       /* must have a string                */
  string = requiredString(DATATYPE, string);
                                       /* type must also be a string        */
  type = optionalString(DATATYPE, type);
  return string->dataType(type);       /* call the datatype method          */
}

#define ADDRESS_MIN 0
#define ADDRESS_MAX 0

BUILTIN(ADDRESS) {
  checkArgs(ADDRESS);                 /* check on required number of args  */
  return context->getAddress();        /* return the current address setting*/
}

#define DIGITS_MIN 0
#define DIGITS_MAX 0

BUILTIN(DIGITS) {
  stringsize_t tempDigits;

  checkArgs(DIGITS);                  /* check on required number of args  */
  tempDigits = context->digits();      /* get the number of digits          */
  return new_integer(tempDigits);      /* return as an option               */
}

#define FUZZ_MIN 0
#define FUZZ_MAX 0

BUILTIN(FUZZ) {
  stringsize_t  tempFuzz;

  checkArgs(FUZZ);                    /* check on required number of args  */
  tempFuzz = context->fuzz();          /* get the fuzz value                */
  return new_integer(tempFuzz);        /* return as an integer object       */
}

#define FORM_MIN 0
#define FORM_MAX 0

BUILTIN(FORM) {
  checkArgs(FORM);                    /* check on required number of args  */
                                       /* return the current form setting   */
  return context->form() == Numerics::FORM_SCIENTIFIC ? OREF_SCIENTIFIC : OREF_ENGINEERING;
}

#define USERID_MIN 0
#define USERID_MAX 0

BUILTIN(USERID) {
  checkArgs(USERID);
  return SysInterpreter::getUserid();
}

#define ERRORTEXT_MIN 1
#define ERRORTEXT_MAX 1
#define ERRORTEXT_n   1

BUILTIN(ERRORTEXT) {
  wholenumber_t  error_number;         /* requested error number            */
  RexxString *result;                  /* function result                   */

  checkArgs(ERRORTEXT);               /* check on required number of args  */
                                       /* get the error number              */
  error_number = (requiredInteger(ERRORTEXT, n))->wholeNumber();
                                       /* outside allowed range?            */
  if (error_number < 0 || error_number > 99)
                                       /* this is an error                  */
    reportException(Error_Incorrect_call_range, new_string(CHAR_ERRORTEXT), IntegerOne, new_integer(error_number));
                                       /* retrieve the major error message  */
  result = SysInterpreter::messageText(error_number * 1000);
  if (result == OREF_NULL)             /* not found?                        */
    result = OREF_NULLSTRING;          /* this is a null string result      */
  return result;                       /* finished                          */
}

#define ARG_MIN 0
#undef ARG_MAX                      /* In AIX already defined            */
#define ARG_MAX 2
#define ARG_n      1
#define ARG_option 2

BUILTIN(ARG) {
  RexxString  *option;                 /* function option                   */
  RexxInteger *n;                      /* arg position count                */
  RexxObject  *result;                 /* function result                   */
  RexxObject **arglist;                /* activation argument list          */
  stringsize_t position;               /* position argument                 */
  size_t size;                         /* array size                        */

  fixArgs(ARG);                       /* expand arguments to full value    */
  n = optionalInteger(ARG, n);        /* get the position info             */
                                       /* get the option string             */
  option = optionalString(ARG, option);
  /* get the argument array            */
  arglist = context->getMethodArgumentList();
  size = context->getMethodArgumentCount();
                                       /* have an option but no position?   */
  if (n == OREF_NULL) {                /* no position specified?            */
    if (option != OREF_NULL)           /* have an option with no position   */
                                       /* raise an error                    */
      reportException(Error_Incorrect_call_noarg, CHAR_ARG, IntegerOne);
    /* return the count as an object */
    result = new_integer(size);
  }
  else if (option == OREF_NULL) {      /* just looking for a specific arg?  */
    position = n->wholeNumber();               /* get the integer value             */
                                       /* must be a positive integer        */
    positiveInteger(position, ARG, IntegerOne);
                                       /* bigger than argument list size?   */
    if (size < position)
      result = OREF_NULLSTRING;        /* just return a null string         */
    else {
      result = arglist[position - 1];  /* get actual value from arglist     */
      if (result == OREF_NULL)         /* argument wasn't there?            */
        result = OREF_NULLSTRING;      /* this too is a null string         */
    }
  }
                                       /* have a null string?               */
  else if (option->getLength() == 0)
                                       /* this is an error                  */
    reportException(Error_Incorrect_call_list, new_string(CHAR_ARG), IntegerTwo, new_string("AENO"), option);
  else {                               /* need to process an option         */
    position = n->wholeNumber();               /* get the integer value             */
                                       /* must be a positive integer        */
    positiveInteger(position, ARG, IntegerOne);

    switch (option->getChar(0)) {      /* process the option character      */

      case 'A':                        /* return argument array             */
      case 'a':                        /* return argument array             */
        if (position == 1) {           /* want it all?                      */
            /* create an array result for the return */
            result = new (size, arglist) RexxArray;
        }
        else if (position > size)      /* beyond bounds of argument list?   */
                                       /* this is a zero size array         */
            result = TheNullArray->copy();
        else {                         /* need to extract a sub array       */
            result = new (size - position + 1, &arglist[position - 1]) RexxArray;
        }
        break;

      case 'E':                        /* argument 'E'xist?                 */
      case 'e':                        /* argument 'E'xist?                 */
        if (position > size)           /* too big for argument list?        */
          result = TheFalseObject;     /* can't be true                     */
                                       /* have a real argument?             */
        else if (arglist[position - 1] == OREF_NULL)
          result = TheFalseObject;     /* nope, this is false also          */
        else
          result = TheTrueObject;      /* have a real argument              */
        break;

      case 'O':                        /* argument 'O'mitted?               */
      case 'o':                        /* argument 'O'mitted?               */
        if (position > size)           /* too big for argument list?        */
          result = TheTrueObject;      /* must be omitted                   */
                                       /* have a real argument?             */
        else if (arglist[position - 1] == OREF_NULL)
          result = TheTrueObject;      /* this is omitted also              */
        else
          result = TheFalseObject;     /* have a real argument              */
        break;

      case 'N':                        /* 'N'ormal processing?              */
      case 'n':                        /* 'N'ormal processing?              */
        if (position > size)           /* bigger than argument list size?   */
          result = OREF_NULLSTRING;    /* just return a null string         */
        else {                         /* get actual value from arglist     */
          result = arglist[position - 1];
          if (result == OREF_NULL)     /* argument wasn't there?            */
            result = OREF_NULLSTRING;  /* this too is a null string         */
        }
        break;

      default:                         /* unknown option                    */
                                       /* this is an error                  */
        reportException(Error_Incorrect_call_list, new_string(CHAR_ARG), IntegerTwo, new_string("AENO"), option);
        break;
    }
  }
  return result;                       /* all finished                      */
}

 static char *daynames[] = {           /* static data for date/time         */
    "Sunday",                          /* formatting                        */
    "Monday",
    "Tuesday",
    "Wednesday",
    "Thursday",
    "Friday",
    "Saturday"
 };

 static char *monthnames[] = {
    "January",
    "February",
    "March",
    "April",
    "May",
    "June",
    "July",
    "August",
    "September",
    "October",
    "November",
    "December"
 };

#define MONTHS  12                     /* months in a year                  */

 static wholenumber_t monthstarts[] = {
     0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365
 };

 static wholenumber_t leapmonthstarts[] = {
     0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366
 };

 static wholenumber_t monthdays[] = {
    31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
 };

#define nextDigit(digit, target) target = target * 10 + (digit - '0')
#define IsDigit(digit) (digit >= '0' && digit <= '9')
#define MONTH_SIZE  2                  /* size of a month date field        */
#define DAY_SIZE    2                  /* size of a day date field          */
#define SHORT_YEAR  2                  /* size of a 2 digit year field      */
#define LONG_YEAR   4                  /* size of a 4 digit year field      */
#define CHAR_MONTH  3                  /* size of the character month field */
#define HOURS_SIZE  2                  /* size of an hours field            */
#define MINUTES_SIZE 2                 /* size of a minutes field           */
#define SECONDS_SIZE 2                 /* size of a seconds field           */
#define MICRO_SIZE   6                 /* size of micro seconds field       */

#define PAST_THRESHOLD   50            /* past threshold for 2 digit years  */
#define FUTURE_THRESHOLD 49            /* future threshold for 2 digit years*/
#define POSTMERIDIAN     "pm"          /* "pm" spec of civil time           */
#define ANTEMERIDIAN     "am"          /* "am" spec of civil time           */
                                       /* leap year calculation             */
#define LeapYear(year) ((!(year % LEAP_CYCLE)) && ((year % CENTURY) || (!(year % OLYMPIAD))))

#define JANUARY     1                  /* positions of the months           */
#define FEBRUARY    2
#define MARCH       3
#define APRIL       4
#define MAY         5
#define JUNE        6
#define JULY        7
#define AUGUST      8
#define SEPTEMBER   9
#define OCTOBER     10
#define NOVEMBER    11
#define DECEMBER    12

#define LEAPMONTH       29             /* days in a leap year February      */
#define MAXCIVILHOURS   12             /* maximum hours in a civil time     */
#define MAXHOURS        23             /* maximum hours in 24 hour clock    */
#define MAXSECONDS      60             /* maximum seconds in time           */
#define MAXMINUTES      60             /* maximum minutes in time           */
#define HOURS_IN_DAY    24             /* hours in a day                    */
#define MINUTES_IN_HOUR 60             /* minutes in an hour                */
#define SECONDS_IN_MINUTE 60           /* second in a minute                */
                                       /* seconds in an hour                */
#define SECONDS_IN_HOUR (SECONDS_IN_MINUTE * MINUTES_IN_HOUR)
                                       /* seconds in a complete day         */
#define SECONDS_IN_DAY  (SECONDS_IN_HOUR * HOURS_IN_DAY)
#define MINUTES_IN_DAY  (MINUTES_IN_HOUR * HOURS_IN_DAY)
#define MICROSECONDS    1000000        /* microseconds in a second          */

                                       /* days in a 400 year olympiad       */
#define BASE_DAYS(year) (((year) * 365) + ((year) / 4) - ((year) / 100) + ((year) / 400))
                                       /* days in a 400 year olympiad       */
#define OLYMPIAD_DAYS BASE_DAYS(400)   /* days in a 400 year Olympiad       */
#define CENTURY_DAYS  BASE_DAYS(100)   /* days in a 100 year century        */
#define LEAP_DAYS     BASE_DAYS(4)     /* days in a 4 year leap cycle       */
#define YEAR_DAYS     365              /* days in a normal year             */
#define LEAP_CYCLE    4                /* years in a leap cycle             */
#define CENTURY       100              /* years in a century                */
#define OLYMPIAD      400              /* years in an Olympiad cycle        */

stringsize_t getNumber(                /* get a number from a date spec     */
     stringchar_t *         input,     /* input string                      */
     stringsize_t  length,             /* length of the numeric field       */
     bool         *invalid )           /* output location                   */
/******************************************************************************/
/* Function:  Validate and convert a number of the specified length into the  */
/*            target variable.  Returns true for invalid numbers.             */
/******************************************************************************/
{
  stringsize_t value;                  /* returned value                    */

  value = 0;                           /* clear this out                    */
  *invalid = false;                    /* this is valid so far              */
  while (length--) {                   /* do for specified length           */
    if (IsDigit(*input))               /* real digit?                       */
      nextDigit(*input, value);        /* add in this digit                 */
    else {                             /* not good digit                    */
      *invalid = true;                 /* flag it                           */
      break;                           /* get out of here                   */
    }
    input++;                           /* step the input pointer            */
  }
  return value;                        /* return the validation flag        */
}


bool  validateDateFormat(              /* validate the date format          */
     stringchar_t *date,               /* input date                        */
     stringchar_t *format,             /* expected format                   */
     REXXDATETIME *output,             /* returned date information         */
     REXXDATETIME *current )           /* current time stamp                */
/******************************************************************************/
/* Function:  Validate and reformat an input date according to the specified  */
/*            format.  Format specifiers are:                                 */
/*                                                                            */
/*            ' '   A blank is expected \                                     */
/*            '/'   A slash is expected |  with feature 476 these separator   */
/*            '.'   A period is expectd |  characters can be any non-alphanum */
/*            ':'   A colon is expected /                                     */
/*            'm'   Start of a month specification                            */
/*            'd'   Start of a day specification                              */
/*            'y'   Start of a 2-digit year spec                              */
/*            'Y'   Start of a 4-digit year spec                              */
/*            'M'   Start of a "named" 3 character month                      */
/*            'h'   Start of a 12-hour hour field                             */
/*            'H'   Start of a 24-hour hour field                             */
/*            'i'   Start of a mInutes field                                  */
/*            's'   Start of a seconds field                                  */
/*            'u'   Start of a microseconds field                             */
/*            'C'   Start of a Civil time meridian designation                */
/*            'c'   Start of a Civil time hour (no leading blanks)            */
/*                                                                            */
/******************************************************************************/
{
    stringchar_t *     inputscan;        /* input string scanning pointer     */
    stringchar_t *     formatscan;       /* format scanning pointer           */
    bool      invalid;                   /* invalid format found              */

    output->day = 1;                     /* default day to 1                  */
    output->month = 1;                   /* default month is january          */
    output->year = 1;                    /* default year is antiquity         */
    inputscan = date;                    /* point to the date start           */
    formatscan = format;                 /* point to the format               */
    invalid = false;                     /* assume this will be valid         */

    if (strlen((char *)date) > strlen((char *)format))   /* not the right length?             */
        invalid = true;                    /* this is bad                       */
    else
    {                               /* scan through this                 */
                                    /* while still good and not done     */
        while (!invalid && *formatscan != '\0')
        {

            switch (*formatscan)
            {           /* process each format piece         */
                case 'm':                      /* month spec                        */
                    /* test and convert                  */
                    output->month = (int16_t) getNumber(inputscan, MONTH_SIZE, &invalid);
                    if (output->month > MONTHS)  /* too big?                          */
                        invalid = true;            /* flag this                         */
                    inputscan += MONTH_SIZE;     /* step both pointers                */
                    formatscan += MONTH_SIZE;
                    break;

                case 'd':                      /* fixed length day spec             */
                    /* test and convert                  */
                    output->day = (int16_t) getNumber(inputscan, DAY_SIZE, &invalid);
                    inputscan += DAY_SIZE;       /* step both pointers                */
                    formatscan += DAY_SIZE;
                    break;

                case 'D':                      /* variable length day spec          */
                    output->day = 0;             /* no days yet                       */
                    if (IsDigit(*inputscan))
                    {   /* real digit?                       */
                        /* pick up the digit                 */
                        nextDigit(*inputscan, output->day);
                        inputscan++;               /* step the pointer                  */
                        if (IsDigit(*inputscan))
                        { /* real digit?                       */
                          /* pick up the digit                 */
                            nextDigit(*inputscan, output->day);
                            inputscan++;             /* step the pointer                  */
                        }
                    }
                    else                         /* not good digit                    */
                        invalid = true;            /* flag it                           */
                    formatscan += DAY_SIZE;      /* skip both format specs            */
                    break;

                case 'h':                      /* 12 hour hours field               */
                    /* test and convert                  */
                    output->hours = (int16_t) getNumber(inputscan, HOURS_SIZE, &invalid);
                    /* too big?                          */
                    if (output->hours > MAXCIVILHOURS)
                        invalid = true;            /* flag this                         */
                    inputscan += HOURS_SIZE;     /* step both pointers                */
                    formatscan += HOURS_SIZE;
                    break;

                case 'H':                      /* 24 hour hours field               */
                    /* test and convert                  */
                    output->hours = (int16_t) getNumber(inputscan, HOURS_SIZE, &invalid);
                    /* too big?                          */
                    if (output->hours > MAXHOURS)
                        invalid = true;            /* flag this                         */
                    inputscan += HOURS_SIZE;     /* step both pointers                */
                    formatscan += HOURS_SIZE;
                    break;

                case 'i':                      /* Minutes time field                */
                    /* test and convert                  */
                    output->minutes = (int16_t) getNumber(inputscan, MINUTES_SIZE, &invalid);
                    /* too big?                          */
                    if (output->minutes >= MAXMINUTES)
                        invalid = true;            /* flag this                         */
                    inputscan += MINUTES_SIZE;   /* step both pointers                */
                    formatscan += MINUTES_SIZE;
                    break;

                case 's':                      /* Seconds time field                */
                    /* test and convert                  */
                    output->seconds = (int16_t) getNumber(inputscan, SECONDS_SIZE, &invalid);
                    /* too big?                          */
                    if (output->seconds >= MAXSECONDS)
                        invalid = true;            /* flag this                         */
                    inputscan += SECONDS_SIZE;   /* step both pointers                */
                    formatscan += SECONDS_SIZE;
                    break;

                case 'u':                      /* micro seconds time field          */
                    /* test and convert                  */
                    output->microseconds = (uint32_t)getNumber(inputscan, MICRO_SIZE, &invalid);
                    inputscan += MICRO_SIZE;     /* step both pointers                */
                    formatscan += MICRO_SIZE;
                    break;

                case 'y':                      /* two digit year spec               */
                    /* test and convert                  */
                    output->year = (int16_t) getNumber(inputscan, SHORT_YEAR, &invalid);
                    /* add in the current century        */
                    output->year += (current->year / 100) * 100;
                    /* converted year in the past?       */
                    if (output->year < current->year)
                    {
                        /* by more than 50 years?            */
                        if ((current->year - output->year) > PAST_THRESHOLD)
                            output->year += CENTURY; /* move it up a century              */
                    }
                    else
                    {                       /* may be in the future              */
                                            /* by more than 49 years?            */
                        if ((output->year - current->year ) > FUTURE_THRESHOLD)
                            output->year -= CENTURY; /* move it back a century            */

                    }
                    inputscan += SHORT_YEAR;     /* step both pointers                */
                    formatscan += SHORT_YEAR;
                    break;

                case 'Y':                      /* four digit year spec              */
                    /* test and convert                  */
                    output->year = (int16_t) getNumber(inputscan, LONG_YEAR, &invalid);
                    inputscan += LONG_YEAR;      /* step both pointers                */
                    formatscan += LONG_YEAR;
                    break;

                case 'M':                      /* Three character "Mmm" form        */
                    {
                        invalid = true;              /* default to invalid                */
                                                     /* scan the months table             */
                        for (uint16_t month = 0; month < MONTHS; month++)
                        {
                            /* have a match?                     */
                            if (!memcmp(monthnames[month], inputscan, CHAR_MONTH))
                            {
                                invalid = false;         /* we're valid again                 */
                                output->month = month + 1;  /* set the month                     */
                                break;                   /* get out of here                   */
                            }
                        }
                        inputscan += CHAR_MONTH;     /* step over the date                */
                        formatscan += CHAR_MONTH;    /* step over the date                */
                        break;
                    }


                case 'C':                      /* civil time spec                   */
                    /* "am" time                         */
                    if (!memcmp(inputscan, ANTEMERIDIAN, strlen(ANTEMERIDIAN)))
                    {
                        if (output->hours == 12)   /* 12 something in the morning?      */
                            output->hours = 0;       /* convert to 24 hour time           */
                    }
                    else if (!memcmp(inputscan, POSTMERIDIAN, strlen(POSTMERIDIAN)))
                    {
                        if (output->hours == 12)   /* 12 something in the afternoon?    */
                            output->hours = 0;       /* convert to 24 hour time           */
                        output->hours += 12;       /* now make into the post meridian   */
                    }
                    else
                        invalid = true;            /* not proper civil time             */
                                                   /* step over the designator          */
                    inputscan += strlen(ANTEMERIDIAN);
                    formatscan += strlen(ANTEMERIDIAN);
                    break;

                case 'c':                      /* civil time hours spec             */
                    output->hours = 0;           /* no hours yet                      */
                    if (IsDigit(*inputscan))
                    {   /* real digit?                       */
                        /* pick up the digit                 */
                        nextDigit(*inputscan, output->hours);
                        inputscan++;               /* step the pointer                  */
                        if (IsDigit(*inputscan))
                        { /* real digit?                       */
                          /* pick up the digit                 */
                            nextDigit(*inputscan, output->hours);
                            inputscan++;             /* step the pointer                  */
                        }
                    }
                    else                         /* not good digit                    */
                        invalid = true;            /* flag it                           */
                    if (output->hours > MAXHOURS)/* too big?                          */
                        invalid = true;            /* flag this                         */
                    formatscan += HOURS_SIZE;    /* just step the format pointer      */
                    break;
                    /* initial cases moved               */
                default:                       /* separators to skip                */
                    /* not the same given?               */
                    if (*inputscan++ != *formatscan++)
                        invalid = true;            /* this is bad                       */
            }
        }
    }
    /* end up with zero for any dates?   */
    if (output->day == 0 || output->month == 0 || output->year == 0)
        invalid = true;                    /* this is invalid                   */
                                           /* have a special leap year check?   */
    if (output->month == FEBRUARY && LeapYear(output->year))
    {
        if (output->day > LEAPMONTH)       /* too many days?                    */
            invalid = true;                  /* this is bad also                  */
    }
    /* too many days for the month       */
    else if (output->day > monthdays[output->month - 1])
        invalid = true;                    /* this is bad also                  */
                                           /* validate the time portion         */
    return invalid;                      /* return validation state           */
}

#define DATE_MIN 0
#define DATE_MAX 5
#define DATE_option  1
#define DATE_indate  2
#define DATE_option2 3
#define DATE_osep    4
#define DATE_isep    5

BUILTIN(DATE) {
  stringchar_t style;                  /* style of DATE output              */
  stringchar_t style2;                 /* style of DATE input               */
  RexxString  *option;                 /* function option                   */
  RexxString  *option2;                /* option string                     */
  RexxString  *osep;                   /* option string 'osep'              */
  RexxString  *isep;                   /* option string 'isep'              */
  RexxString  *indate;                 /* input date                        */
  RexxString  *month_name;             /* local month name                  */
  REXXDATETIME timestamp;              /* working time stamp                */
  REXXDATETIME current;                /* current activation timestamp      */
  wholenumber_t day;                   /* current day                       */
  wholenumber_t month;                 /* current month                     */
  wholenumber_t weekday;               /* current weekday                   */
  wholenumber_t year;                  /* current year                      */
  wholenumber_t basedate;              /* calculated basedate               */
  wholenumber_t basedays;              /* converted base days               */
  wholenumber_t yearday;               /* converted day in year             */
  stringchar_t  work[80];              /* temporary work                    */
  stringchar_t  formstr[20];           /* temporary format string           */
  stringchar_t * format;               /* format for conversion             */
  wholenumber_t *monthtable;           /* pointer to month days             */
  int   i;                             /* loop counter                      */
  bool invalid;                        /* invalid input date format         */

  fixArgs(DATE);                      /* expand arguments to full value    */
                                       /* get the option string             */
  option = optionalString(DATE, option);
                                       /* the input date                    */
  indate = optionalString(DATE, indate);
                                       /* input date format                 */
  option2 = optionalString(DATE, option2);
                                       /* output separator                  */
  osep = optionalString(DATE, osep);
                                       /* input separator                   */
  isep = optionalString(DATE, isep);

  context->getTime(&current);          /* get the current activation time   */

  if (option == OREF_NULL)             /* just using default format?        */
    style = 'N';                       /* use the 'Normal form              */
  else if (option->getLength() == 0)        /* have a null string?               */
                                       /* this is an error                  */
    reportException(Error_Incorrect_call_list, new_string(CHAR_DATE), IntegerOne, new_string("BDELMNOSUW"), option);
  else                                 /* need to process an option         */
                                       /* option is first character         */
    style = toupper(option->getChar(0));
                                       /* opt2 or isep w/o date?            */
  if (indate == OREF_NULL && (option2 != OREF_NULL || isep != OREF_NULL))
                                       /* this is an error                  */
    reportException(Error_Incorrect_call_noarg, CHAR_DATE, IntegerTwo);

  if (option2 == OREF_NULL)            /* just using default format?        */
    style2 = 'N';                      /* use the 'Normal form              */
  else if (option2->getLength() == 0)       /* have a null string?               */
                                       /* this is an error                  */
    reportException(Error_Incorrect_call_list, new_string(CHAR_DATE), IntegerThree, new_string("BDENOSU"), option2);
  else                                 /* need to process an option         */
                                       /* option is first character         */
    style2 = toupper(option2->getChar(0));

                                       /* begin addition                    */
  if (osep != OREF_NULL && strchr("BDMWL", style) != NULL)
    reportException(Error_Incorrect_call_format_incomp_sep, new_string(CHAR_DATE), IntegerOne, new_string(style), IntegerFour);
                                       /* end addition                      */

  if (indate != OREF_NULL) {           /* given a time stamp?               */
    invalid = false;                   /* assume have a good stamp          */

                                       /* begin addition                    */
    if (isep != OREF_NULL && strchr("BDMWL", style2) != NULL)
      reportException(Error_Incorrect_call_format_incomp_sep, new_string(CHAR_DATE), IntegerThree, new_string(style2), IntegerFive);
                                       /* end addition                      */
                                       /* clear the time stamp              */
    memset(&timestamp, '\0', sizeof(REXXDATETIME));
    switch (style2) {                  /* convert to usable form per option2*/

      case 'N':                        /* 'N'ormal: default style           */
        if (isep == OREF_NULL)         /* begin additions                   */
          format = (stringchar_t *)"DD MMM YYYY";  /* default: dd Mmm yyyy              */
        else if (isep->getLength() == 0)
          format = (stringchar_t *)"DDMMMYYYY";        /* without any separator             */
        else if (isep->getLength() > 1 || strchr((char *)ALPHANUM, isep->getChar(0)) != NULL)
          reportException(Error_Incorrect_call_parm_wrong_sep, new_string(CHAR_DATE), IntegerFive, new_string(isep->getStringData(), isep->getLength()));
        else if (isep->getChar(0) == ' ')
          format = (stringchar_t *)"DD MMM YYYY";      /* with spaces                       */
        else {                         /* with user defined separator       */
          strcpy((char *)formstr, "DD MMM YYYY");
          formstr[2] = isep->getChar(0);
          formstr[6] = isep->getChar(0);
          format = formstr;
        }                              /* end additions                     */
        break;

      case 'B':                        /* 'B'asedate                        */
        format = NULL;                 /* don't use verification routine    */
                                       /*convert the value                  */
                                       /* bad value?                        */
        if (!indate->numberValue(&basedays, Numerics::DEFAULT_DIGITS) || basedays < 0)
          invalid = true;              /* have a bad value                  */
        else {                         /* calculate a timestamp             */
          basedays++;                  /* add one for today                 */
                                       /* clear the working copy            */
          memset(&timestamp, '\0', sizeof(REXXDATETIME));
                                       /* get to start of current 400 years */
          timestamp.year = (uint16_t)((basedays / OLYMPIAD_DAYS) * OLYMPIAD);
                                       /* adjust this downward              */
          basedays -= BASE_DAYS(timestamp.year);
          if (basedays == 0)           /* end on a boundary?                */
            basedays = YEAR_DAYS + 1;  /* last day of a leap year           */
          else {
                                       /* get to start of current centry    */
            timestamp.year += (uint16_t)((basedays / CENTURY_DAYS) * CENTURY);
                                       /* get the remainder portion         */
            basedays = basedays % CENTURY_DAYS;
            if (basedays == 0)         /* end on a century boundary?        */
              basedays = YEAR_DAYS;    /* normal year count - non leap year */
            else {
                                       /* now get to start of leap cycle    */
              timestamp.year += (uint16_t)((basedays / LEAP_DAYS) * LEAP_CYCLE);
                                       /* strip out that part too           */
              basedays = basedays % LEAP_DAYS;
              if (basedays == 0)       /* end on a leap cycle?              */
                                       /* last day of a leap year           */
                basedays = YEAR_DAYS + 1;
              else {
                                       /* get the last few years            */
                timestamp.year += (uint16_t)((basedays / YEAR_DAYS));
                                       /* down to the actual year day       */
                basedays = basedays % YEAR_DAYS;
                if (basedays == 0)     /* end on a year boundary?           */
                  basedays = YEAR_DAYS;/* end of the year                   */
                else
                timestamp.year ++;     /* need to step to the next year     */
              }
            }
          }
          if (LeapYear(timestamp.year))/* in a leap year                    */
                                       /* use the leap table                */
            monthtable = leapmonthstarts;
          else
            monthtable = monthstarts;  /* use the standard table            */
                                       /* find the relevant month           */
          for (i = 0; ; i++) {
                                       /* in the proper month yet?          */
            if (monthtable[i] >= basedays) {
              timestamp.month = i;     /* this is the proper month          */
                                       /* finally get the days              */
              timestamp.day = (uint16_t)(basedays - monthtable[i - 1]);
              break;                   /* finished                          */
            }
          }
        }
        break;

      case 'D':                        /* 'D'ay of year                     */
        format = NULL;                 /* don't use verification routine    */
                                       /*convert the value                  */
                                       /* bad value?                        */
        if (!indate->numberValue(&yearday, Numerics::DEFAULT_DIGITS) || yearday < 0 || yearday > YEAR_DAYS + 1 ||
            (yearday > YEAR_DAYS && !LeapYear(current.year)))
          invalid = true;              /* have a bad value                  */
        else {                         /* calculate a timestamp             */
                                       /* clear the working copy            */
          memset(&timestamp, '\0', sizeof(REXXDATETIME));

          timestamp.year = current.year; /* we always use the current year  */

          if (LeapYear(timestamp.year))/* in a leap year                    */
                                       /* use the leap table                */
            monthtable = leapmonthstarts;
          else
            monthtable = monthstarts;  /* use the standard table            */
                                       /* find the relevant month           */
          for (i = 0; ; i++) {
                                       /* in the proper month yet?          */
            if (monthtable[i] >= yearday) {
              timestamp.month = i;     /* this is the proper month          */
                                       /* finally get the days              */
              timestamp.day = (uint16_t)(yearday - monthtable[i - 1]);
              break;                   /* finished                          */
            }
          }
        }
        break;

      case 'E':                        /* 'E'uropean format: days-month-year*/
        if (isep == OREF_NULL)         /* begin additions                   */
          format = (stringchar_t *)"dd/mm/yy";         /* default: with slashes             */
        else if (isep->getLength() == 0)
          format = (stringchar_t *)"ddmmyy";           /* without any separator             */
        else if (isep->getLength() > 1 || strchr((char *)ALPHANUM, isep->getChar(0)) != NULL)
          reportException(Error_Incorrect_call_parm_wrong_sep, new_string(CHAR_DATE), IntegerFive, new_string(isep->getStringData(), isep->getLength()));
        else if (isep->getChar(0) == '/')
          format = (stringchar_t *)"dd/mm/yy";         /* with slashes                      */
        else {                         /* with user defined separator       */
          strcpy((char *)formstr, "dd/mm/yy");
          formstr[2] = isep->getChar(0);
          formstr[5] = isep->getChar(0);
          format = formstr;
        }
        break;

      case 'O':                        /* 'O'rdered format: year-month-day  */
        if (isep == OREF_NULL)
          format = (stringchar_t *)"yy/mm/dd";         /* default: with slashes             */
        else if (isep->getLength() == 0)
          format = (stringchar_t *)"yymmdd";           /* without any separator             */
        else if (isep->getLength() > 1 || strchr((char *)ALPHANUM, isep->getChar(0)) != NULL)
          reportException(Error_Incorrect_call_parm_wrong_sep, new_string(CHAR_DATE), IntegerFive, new_string(isep->getStringData(), isep->getLength()));
        else if (isep->getChar(0) == '/')
          format = (stringchar_t *)"yy/mm/dd";         /* with slashes                      */
        else {                         /* with user defined separator       */
          strcpy((char *)formstr, "yy/mm/dd");
          formstr[2] = isep->getChar(0);
          formstr[5] = isep->getChar(0);
          format = formstr;
        }
        break;

      case 'S':                        /* 'S'tandard format (ISO date)      */
        if (isep == OREF_NULL || isep != OREF_NULL && isep->getLength() == 0 )
          format = (stringchar_t *)"YYYYmmdd";         /* default: compact format           */
        else if (isep->getLength() > 1 || strchr((char *)ALPHANUM, isep->getChar(0)) != NULL)
          reportException(Error_Incorrect_call_parm_wrong_sep, new_string(CHAR_DATE), IntegerFive, new_string(isep->getStringData(), isep->getLength()));
        else if (isep->getChar(0) == ' ')
          format = (stringchar_t *)"YYYY mm dd";       /* spread format                     */
        else {                         /* spread format with separator      */
          strcpy((char *)formstr, "YYYY mm dd");
          formstr[4] = isep->getChar(0);
          formstr[7] = isep->getChar(0);
          format = formstr;
        }
        break;

      case 'U':                        /* 'U'SA format: month-day-year      */
        if (isep == OREF_NULL)
          format = (stringchar_t *)"mm/dd/yy";         /* default: with slashes             */
        else if (isep->getLength() == 0)
          format = (stringchar_t *)"mmddyy";           /* without any separator             */
        else if (isep->getLength() > 1 || strchr((char *)ALPHANUM, isep->getChar(0)) != NULL)
          reportException(Error_Incorrect_call_parm_wrong_sep, new_string(CHAR_DATE), IntegerFive, new_string(isep->getStringData(), isep->getLength()));
        else if (isep->getChar(0) == '/')
          format = (stringchar_t *)"mm/dd/yy";         /* with slashes                      */
        else {                         /* with user defined separator       */
          strcpy((char *)formstr, "mm/dd/yy");
          formstr[2] = isep->getChar(0);
          formstr[5] = isep->getChar(0);
          format = formstr;
        }
        break;

      default:
//      work[0] = style2;              /* copy over the character           */
        reportException(Error_Incorrect_call_list, new_string(CHAR_DATE), IntegerThree, new_string("BDENOSU"), new_string(style2));
        break;
    }
    if (format != NULL)                /* using common formatting?          */
                                       /* yes, call the verify routine      */
      invalid = validateDateFormat(indate->getStringData(), format, &timestamp, &current);
    if (invalid) {                     /* not convert cleanly?              */
      if (isep != OREF_NULL)
        reportException(Error_Incorrect_call_format_incomp_sep, new_string(CHAR_DATE), IntegerTwo, indate, IntegerFive);
      else
        reportException(Error_Incorrect_call_format_invalid, new_string(CHAR_DATE), indate, new_string(style2));
    }
                                       /* now calculate the yearday         */
    timestamp.yearday = (uint32_t)(monthstarts[timestamp.month - 1] + timestamp.day);
                                       /* in a leap year?                   */
    if (timestamp.month > 2 && (!(timestamp.year % 4)) && ((timestamp.year % 100) || (! (timestamp.year % 400))))
      timestamp.yearday++;             /* adjust for leap year       */
    year = timestamp.year - 1;         /* get zero based year               */
                                       /* get the base number of days       */
    basedate = (year * 365) + (year / 4) - (year / 100) + (year / 400);
    basedate += timestamp.yearday;     /* add in days for this year         */
    timestamp.weekday = (uint16_t)(basedate % 7);  /* calculate the week day            */
  }
  else
    timestamp = current;               /* just copy the current time stamp  */

  day = timestamp.day;                 /* get various date parts            */
  month = timestamp.month;
  year = timestamp.year;
                                       /* 0 = sunday                        */
  weekday = timestamp.weekday;

  switch (style) {                     /* process the various styles        */

    case 'B':                          /* 'B'asedate                        */
      year--;                          /* decrement the year                */
      basedate = (year * 365) + (year / 4) - (year / 100) + (year / 400);
                                       /* add in days for this year         */
      basedate += timestamp.yearday - 1;
      sprintf((char *)work, "%lu", basedate);  /* format this into the buffer       */
      break;

    case 'D':                          /* 'D'ays                            */
                                       /* format directly into the buffer   */
      sprintf((char *)work, "%u", timestamp.yearday);
      break;

    case 'E':                          /* 'E'uropean                        */
      if (osep == OREF_NULL)
        format = (stringchar_t *)"%02d/%02d/%02d";     /* default format                    */
      else if (osep->getLength() == 0 )
        format = (stringchar_t *)"%02d%02d%02d";       /* compact format                    */
      else if (osep->getLength() > 1 || strchr((char *)ALPHANUM, osep->getChar(0)) != NULL)
        reportException(Error_Incorrect_call_parm_wrong_sep, new_string(CHAR_DATE), IntegerFour, new_string(osep->getStringData(), osep->getLength()));
      else if (osep->getChar(0) == '/')
        format = (stringchar_t *)"%02d/%02d/%02d";     /* with slashes                      */
      else {                           /* with user defined separator       */
        strcpy((char *)formstr, "%02d/%02d/%02d");
        formstr[4] = osep->getChar(0);
        formstr[9] = osep->getChar(0);
        format = formstr;
      }
      sprintf((char *)work, (char *)format, day, month, year % 100);
      break;

    case 'L':                          /* 'L'ocal                           */
                                       /* get the month name                */
      month_name = SysInterpreter::messageText(Message_Translations_January + month - 1);
                                       /* format as a date                  */
      sprintf((char *)work, "%u %s %4.4u", day, month_name->getStringData() , year);
      break;

    case 'M':                          /* 'M'onth                           */
                                       /* copy the name into the buffer     */
      strcpy((char *)work, monthnames[month - 1]);
      break;

    case 'N':                          /* 'N'ormal -- default format        */
      if (osep == OREF_NULL)
        format = (stringchar_t *)"%u %3.3s %4.4u";     /* default format                    */
      else if (osep->getLength() == 0 )
        format = (stringchar_t *)"%u%3.3s%4.4u";       /* compact format                    */
      else if (osep->getLength() > 1 || strchr((char *)ALPHANUM, osep->getChar(0)) != NULL)
        reportException(Error_Incorrect_call_parm_wrong_sep, new_string(CHAR_DATE), IntegerFour, new_string(osep->getStringData(), osep->getLength()));
      else if (osep->getChar(0) == ' ')
        format = (stringchar_t *)"%u %3.3s %4.4u";     /* with blanks                       */
      else {                           /* with user defined separator       */
        strcpy((char *)formstr, "%u %3.3s %4.4u");
        formstr[2] = osep->getChar(0);
        formstr[8] = osep->getChar(0);
        format = formstr;
      }
      sprintf((char *)work, (char *)format, day, monthnames[month-1] , year);
      break;

    case 'O':                          /* 'O'rdered                         */
      if (osep == OREF_NULL)
        format = (stringchar_t *)"%02d/%02d/%02d";     /* default format                    */
      else if (osep->getLength() == 0 )
        format = (stringchar_t *)"%02d%02d%02d";       /* default: compact format           */
      else if (osep->getLength() > 1 || strchr((char *)ALPHANUM, osep->getChar(0)) != NULL)
        reportException(Error_Incorrect_call_parm_wrong_sep, new_string(CHAR_DATE), IntegerFour, new_string(osep->getStringData(), osep->getLength()));
      else if (osep->getChar(0) == '/')
        format = (stringchar_t *)"%02d/%02d/%02d";     /* with slashes                      */
      else {                           /* with user defined separator       */
        strcpy((char *)formstr, "%02d/%02d/%02d");
        formstr[4] = osep->getChar(0);
        formstr[9] = osep->getChar(0);
        format = formstr;
      }
      sprintf((char *)work, (char *)format, year % 100, month, day);
      break;

    case 'S':                          /* 'S'tandard format (ISO dates)     */
      if (osep == OREF_NULL || osep != OREF_NULL && osep->getLength() == 0 )
        format = (stringchar_t *)"%04d%02d%02d";       /* default: compact format           */
      else if (osep->getLength() > 1 || strchr((char *)ALPHANUM, osep->getChar(0)) != NULL)
        reportException(Error_Incorrect_call_parm_wrong_sep, new_string(CHAR_DATE), IntegerFour, new_string(osep->getStringData(), osep->getLength()));
      else if (osep->getChar(0) == ' ')
        format = (stringchar_t *)"%04d %02d %02d";     /* spread format                     */
      else {                           /* spread format with separator      */
        strcpy((char *)formstr, "%04d %02d %02d");
        formstr[4] = osep->getChar(0);
        formstr[9] = osep->getChar(0);
        format = formstr;
      }
      sprintf((char *)work, (char *)format, year, month, day);
      break;

    case 'U':                          /* 'U'SA                             */
      if (osep == OREF_NULL)
        format = (stringchar_t *)"%02d/%02d/%02d";     /* default format                    */
      else if (osep->getLength() == 0 )
        format = (stringchar_t *)"%02d%02d%02d";       /* default: compact format           */
      else if (osep->getLength() > 1 || strchr((char *)ALPHANUM, osep->getChar(0)) != NULL)
        reportException(Error_Incorrect_call_parm_wrong_sep, new_string(CHAR_DATE), IntegerFour, new_string(osep->getStringData(), osep->getLength()));
      else if (osep->getChar(0) == '/')
        format = (stringchar_t *)"%02d/%02d/%02d";     /* with slashes                      */
      else {                           /* with user defined separator       */
        strcpy((char *)formstr, "%02d/%02d/%02d");
        formstr[4] = osep->getChar(0);
        formstr[9] = osep->getChar(0);
        format = formstr;
      }
      sprintf((char *)work, (char *)format, month, day, year % 100);
      break;

    case 'W':                          /* 'W'eekday                         */
      strcpy((char *)work, daynames[weekday]); /* copy over the text name           */
      break;

    default:                           /* unrecognized                      */
      work[0] = style;                 /* copy over the character           */
      reportException(Error_Incorrect_call_list, new_string(CHAR_DATE), IntegerOne, new_string("BDELMNOSUW"), new_string(work, 1));
      break;
  }
                                       /* now create a string object        */
  return new_string(work);
}

#define TIME_LOWER -1                  /* could not subtract times          */
#define TIME_EQUAL  0                  /* times were exactly equal          */
#define TIME_HIGHER 1                  /* times were subtractable           */


int  subtractTimes(                    /* subtract two time stamps          */
     REXXDATETIME *end,                /* time to subtract from             */
     REXXDATETIME *start,              /* time to subtract out              */
     wholenumber_t*seconds,            /* difference in seconds             */
     wholenumber_t*microseconds)       /* difference in microseconds        */
{
  wholenumber_t startdate;             /* starting date in days             */
  wholenumber_t starttime;             /* starting time in seconds          */
  wholenumber_t enddate;               /* ending time in days               */
  wholenumber_t endtime;               /* ending time in seconds            */
  int       result;                    /* subtraction result flag           */

  result = TIME_LOWER;                 /* assume this is bad                */
                                       /* get the starting date             */
  startdate = start->yearday + BASE_DAYS(start->year) - 1;
                                       /* and the ending date               */
  enddate = end->yearday + BASE_DAYS(end->year) - 1;
  starttime = (((start->hours * MINUTES_IN_HOUR) + start->minutes) * SECONDS_IN_MINUTE) + start->seconds;
  endtime = ((end->hours * MINUTES_IN_HOUR + end->minutes) * SECONDS_IN_MINUTE) + end->seconds;
  *seconds = 0;                        /* set an initial value              */
  *microseconds = 0;                   /* for both values                   */
  if (enddate < startdate)             /* ending date problem?              */
    return TIME_LOWER;                 /* got a problem here                */
                                       /* adjust the ending time by diff.   */
  endtime += (enddate - startdate) * SECONDS_IN_DAY;
  if (endtime == starttime && end->microseconds == start->microseconds)
    result = TIME_EQUAL;               /* have time equality                */
  else if (endtime == starttime) {     /* equal except microseconds?        */
                                       /* not a bad result?                 */
    if (end->microseconds > start->microseconds) {
      result = TIME_HIGHER;            /* have a good time value            */
                                       /* do the subtraction                */
      *microseconds = end->microseconds - start->microseconds;
    }
  }
  else if (endtime > starttime) {      /* differ by more than microseconds  */
    result = TIME_HIGHER;              /* have a good time value            */
                                       /* subracting going to carry?        */
    if (start->microseconds > end->microseconds) {
                                       /* borrow from the timestamp         */
      *microseconds = (end->microseconds + MICROSECONDS) - start->microseconds;
      endtime--;                       /* reduce ending time by one         */
    }
    else                               /* do the subtraction                */
      *microseconds = end->microseconds - start->microseconds;
    *seconds = endtime - starttime;    /* calculate the seconds portion     */
  }
  return result;                       /* return the subtraction result     */
}



#define TIME_MIN 0
#define TIME_MAX 3
#define TIME_option  1
#define TIME_intime  2
#define TIME_option2 3

BUILTIN(TIME) {
  int   style;                         /* style of TIME output              */
  int   style2;                        /* style of TIME input               */
  RexxString  *option;                 /* function option                   */
  RexxString  *option2;                /* option string                     */
  RexxString  *intime;                 /* input date                        */
  REXXDATETIME timestamp;              /* working time stamp                */
  REXXDATETIME current;                /* current activation timestamp      */
  stringchar_t  work[30];              /* temporary work                    */
  stringchar_t * format;               /* format for conversion             */
  wholenumber_t i;                     /* loop counter (and temporary)      */
  wholenumber_t hours;                 /* working hours                     */
  wholenumber_t minutes;               /* working minutes                   */
  wholenumber_t seconds;               /* working seconds                   */
  wholenumber_t microseconds;          /* working microseconds              */
  stringchar_t * ampm;                 /* am or pm pointer                  */
  int   threshold;                     /* time comparison threshold         */
  bool invalid;                        /* input time validation flag        */

  fixArgs(TIME);                      /* expand arguments to full value    */
                                       /* get the option string             */
  option = optionalString(TIME, option);
                                       /* the input date                    */
  intime = optionalString(TIME, intime);
                                       /* input date format                 */
  option2 = optionalString(TIME, option2);
  context->getTime(&current);          /* get the current activation time   */
  if (option == OREF_NULL)             /* just using default format?        */
    style = 'N';                       /* use the 'Normal form              */
  else if (option->getLength() == 0)        /* have a null string?               */
                                       /* this is an error                  */
    reportException(Error_Incorrect_call_list, new_string(CHAR_TIME), IntegerOne, new_string("CEHLMNRS"), option);
  else                                 /* need to process an option         */
                                       /* option is first character         */
    style = toupper(option->getChar(0));
                                       /* second option and no date?        */
  if (intime == OREF_NULL && option2 != OREF_NULL)
                                       /* this is an error                  */
    reportException(Error_Incorrect_call_noarg, new_string(CHAR_TIME), IntegerTwo);

  if (option2 == OREF_NULL)            /* just using default format?        */
    style2 = 'N';                      /* use the 'Normal form              */
  else if (option2->getLength() == 0)       /* have a null string?               */
                                       /* this is an error                  */
    reportException(Error_Incorrect_call_list, new_string(CHAR_TIME), IntegerThree, new_string("CHLMNS"), option2);
  else                                 /* need to process an option         */
                                       /* option is first character         */
    style2 = toupper(option2->getChar(0));

  if (intime != OREF_NULL) {           /* given a time stamp?               */
    if (style == 'R' || style == 'E')  /* either of the elapsed time options*/
                                       /* this is an error                  */
      reportException(Error_Incorrect_call_invalid_conversion, CHAR_TIME, new_string((stringchar_t)style));
    invalid = false;                   /* assume have a good stamp          */
                                       /* clear the time stamp              */
    memset(&timestamp, '\0', sizeof(REXXDATETIME));
    switch (style2) {                  /* convert to usable form per option2*/

      case 'N':                        /* default style                     */
        format = (stringchar_t *)"HH:ii:ss";           /* 01:23:45 format (24 hour)         */
        break;

      case 'C':                        /* 'C'ivil time                      */
        format = (stringchar_t *)"cc:iiCC";            /* 1:23pm format (12-hour, no zero)  */
        break;

      case 'L':                        /* 'L'ong format                     */
        format = (stringchar_t *)"HH:ii:ss.uuuuuu";    /* full 24-hour, plus fractional     */
        break;

      case 'H':                        /* 'H'ours format                    */
        format = NULL;                 /* don't use verification routine    */
                                       /* bad value?                        */
        if (!intime->numberValue(&i, Numerics::DEFAULT_DIGITS) || i < 0 || i >= HOURS_IN_DAY)
          invalid = true;              /* have a bad value                  */
        else
          timestamp.hours = (uint16_t)i;/* set the hours time (other parts 0)*/
        break;

      case 'S':                        /* 'S'econds format                  */
        format = NULL;                 /* don't use verification routine    */
                                       /* bad value?                        */
        if (!intime->numberValue(&i, Numerics::DEFAULT_DIGITS) || i < 0 || i >= SECONDS_IN_DAY)
          invalid = true;              /* have a bad value                  */
        else {                         /* break down into complete time     */
                                       /* get the hours                     */
          timestamp.hours = (uint16_t)(i / SECONDS_IN_HOUR);
          i = i % SECONDS_IN_HOUR;     /* subtract out the hours portion    */
                                       /* pull out the minutes portion      */
          timestamp.minutes = (uint16_t)(i / SECONDS_IN_MINUTE);
                                       /* again reduce to the remainder     */
          timestamp.seconds = (uint16_t)(i % SECONDS_IN_MINUTE);
        }
        break;

      case 'M':                        /* 'M'inutes format                  */
        format = NULL;                 /* don't use verification routine    */
                                       /* bad value?                        */
        if (!intime->numberValue(&i, Numerics::DEFAULT_DIGITS) || i < 0 || i >= MINUTES_IN_DAY)
          invalid = true;              /* have a bad value                  */
        else {                         /* break down into complete time     */
                                       /* get the hours                     */
          timestamp.hours = (uint16_t)(i / MINUTES_IN_HOUR);
                                       /* subtract out the hours portion    */
          timestamp.minutes = (uint16_t)(i % MINUTES_IN_HOUR);
        }
        break;

      default:
        work[0] = style2;              /* copy over the character           */
        reportException(Error_Incorrect_call_list, new_string(CHAR_TIME), IntegerThree, new_string("CHLMNS"), new_string((stringchar_t *)work, 1));
        break;
    }
    if (format != NULL)                /* using common formatting?          */
                                       /* yes, call the verify routine      */
      invalid = validateDateFormat(intime->getStringData(), format, &timestamp, &current);
    if (invalid)                       /* not convert cleanly?              */
      reportException(Error_Incorrect_call_format_invalid, new_string(CHAR_TIME), intime, new_string((stringchar_t)style2) );
  }
  else
    timestamp = current;               /* just copy the current time stamp  */
  hours = timestamp.hours;             /* copy parts of the timestamp       */
  minutes = timestamp.minutes;
  seconds = timestamp.seconds;
  microseconds = timestamp.microseconds;

  switch (style) {                     /* process the styles                */

     case 'E':                         /* 'E'lapsed time                    */
     case 'R':                         /* 'R'eset elapsed time              */

                                       /* get the current elapsed time      */
       context->getElapsed(&current);
                                       /* subtract the timestamps           */
       threshold = subtractTimes(&timestamp, &current, &seconds, &microseconds);
       if (threshold == TIME_LOWER) {  /* have a clock reset situation?     */
         strcpy((char *)work, "0");    /* just return zero                  */
         context->resetElapsed();      /* reset the clock for next time     */
       }                               /* times equal?                      */
       else if (threshold == TIME_EQUAL)
         strcpy((char *)work, "0");    /* just return zero                  */
       else
                                       /* format the result                 */
         sprintf((char *)work, "%lu.%06lu", seconds, microseconds);
       if (style == 'R')               /* is this a reset call?             */
         context->resetElapsed();      /* reset the clock for next time     */
       break;

     case 'C':                         /* 'C'ivil time                      */
                                       /* get the am or pm designation      */
       ampm = (hours >= 12) ? (stringchar_t *)POSTMERIDIAN : (stringchar_t *)ANTEMERIDIAN;
       if (hours == 0)                 /* early morning time stamp?         */
         hours = 12;                   /* make sure this displays as 12     */
       else if (hours > 12)            /* afternoon or evening?             */
         hours = hours - 12;           /* reduce to civil form              */
                                       /* format the time stamp             */
       sprintf((char *)work,"%u:%2.2u%s",hours, minutes, ampm);
       break;

     case 'H':                         /* 'Hours'                           */
       sprintf((char *)work,"%u",hours);       /* just format the hours             */
       break;

     case 'L':                         /* 'L'ong format                     */
                                       /* format the timestamp immediately  */
       sprintf((char *)work,"%2.2u:%2.2u:%2.2u.%6.6lu", hours, minutes, seconds, microseconds);
       break;

     case 'M':                         /* 'M'inutes format                  */
                                       /* format total number of minutes    */
       sprintf((char *)work,"%u", hours * MINUTES_IN_HOUR + minutes);
       break;

     case 'N':                         /* 'N'ormal format...the default     */
                                       /* just the simple hh:mm:ss form     */
       sprintf((char *)work, "%2.2u:%2.2u:%2.2u", hours, minutes, seconds);
       break;

     case 'S':                         /* 'S'econds format...total seconds  */
                                       /* calculate total seconds           */
       sprintf((char *)work,"%u", (hours * MINUTES_IN_HOUR + minutes) * SECONDS_IN_MINUTE + seconds);
       break;

     default:                          /* unknown format                    */
       work[0] = style;                /* copy over the character           */
       reportException(Error_Incorrect_call_list, new_string(CHAR_TIME), IntegerOne, new_string("CEHLMNRS"), new_string((stringchar_t *)work, 1));
       break;
  }
                                       /* now create a string object        */
  return new_string(work, strlen((char *)work));
}


#define DEFAULT_MIN 0                  /* default random minimum value      */
#define DEFAULT_MAX 999                /* default random maximum value      */

#define RANDOM_MIN 0
#define RANDOM_MAX 3
#define RANDOM_minimum 1
#define RANDOM_maximum 2
#define RANDOM_seed    3

BUILTIN(RANDOM) {
  RexxInteger *minimum;                /* RANDOM minimum value              */
  RexxInteger *maximum;                /* RANDOM maximum value              */
  RexxInteger *seed;                   /* RANDOM seed value                 */

  fixArgs(RANDOM);                     /* expand arguments to full value    */

  if (argcount == 1)
  {
                                       /* get the maximum value             */
      maximum = optionalInteger(RANDOM, maximum);
      return context->random(DEFAULT_MIN, maximum->wholeNumber(), OREF_NULL);
  }
  else
  {
                                       /* get the minimum value             */
      minimum = optionalInteger(RANDOM, minimum);
                                       /* get the maximum value             */
      maximum = optionalInteger(RANDOM, maximum);

      wholenumber_t minValue = DEFAULT_MIN;
      wholenumber_t maxValue = DEFAULT_MAX;

      if (minimum != OREF_NULL)
      {
          minValue = minimum->wholeNumber();
      }
      if (maximum != OREF_NULL)
      {
          maxValue = maximum->wholeNumber();
      }
                                       /* get the seed value                */
      seed    = optionalInteger(RANDOM, seed);

      return context->random(minValue, maxValue, seed);
  }
}


#define XRANGE_MIN 0
#define XRANGE_MAX 2
#define XRANGE_start   1
#define XRANGE_end     2

BUILTIN(XRANGE) {
  RexxString *start;                   /* starting position                 */
  RexxString *end;                     /* ending position                   */
  RexxString *result;                  /* XRANGE result                     */
  stringchar_t  startchar;             /* starting character                */
  stringchar_t  endchar;               /* ending character                  */
  stringsize_t  length;                /* length of result                  */
  stringsize_t  i;                     /* loop counter                      */

  fixArgs(XRANGE);                    /* expand arguments to full value    */
  startchar = 0;                       /* set default start position        */
  endchar = 0xff;                      /* set default end position          */

                                       /* get the starting string           */
  start = optionalString(XRANGE, start);
  end = optionalString(XRANGE, end);  /* get the ending string             */

  if (start != OREF_NULL) {            /* have a start position             */
    if (start->getLength() != 1)            /* not a single character?           */
                                       /* have an error                     */
      reportException(Error_Incorrect_call_pad, new_string(CHAR_XRANGE), IntegerOne, start);
    startchar = start->getChar(0);     /* get the new start position        */
  }
  if (end != OREF_NULL) {              /* have an end position              */
    if (end->getLength() != 1)              /* not a single character?           */
                                       /* have an error                     */
      reportException(Error_Incorrect_call_pad, new_string(CHAR_XRANGE), IntegerTwo, end);
    endchar = end->getChar(0);         /* get the new end position          */
  }
                                       /* calculate result size             */
  length = ((endchar < startchar) ? (256 - startchar) + endchar : (endchar - startchar)) + 1;
  result = raw_string(length);         /* get a result string               */
  for (i = 0; i < length; i++)         /* loop through result length        */
    result->putChar(i, startchar++);   /* inserting each character          */
  return result;                       /* finished                          */
}

#define SYMBOL_MIN 1
#define SYMBOL_MAX 1
#define SYMBOL_name    1

BUILTIN(SYMBOL) {
  RexxVariableBase *variable;          /* specified variable name           */
  RexxString *result;                  /* function result                   */

  fixArgs(SYMBOL);                    /* expand arguments to full value    */
                                       /* get the variable name             */
  result = requiredString(SYMBOL, name);
                                       /* get a variable retriever          */
  variable = context->getVariableRetriever(result);
  if (variable == OREF_NULL)           /* invalid variable name?            */
                                       /* return the 'BAD' result           */
    result = OREF_BAD;
  else if (isString((RexxString *)variable))    /* directly returned a string?       */
                                       /* this is a literal value           */
    result = OREF_LIT;
  else {                               /* need to perform lookup            */
                                       /* see if variable has a value       */
    if (!variable->exists(context))
                                       /* this is a literal value           */
      result = OREF_LIT;
    else
                                       /* this is a variable value          */
      result = OREF_VAR;
  }
  return result;                       /* return the indicator              */
}

#define VAR_MIN 1
#define VAR_MAX 1
#define VAR_name    1

BUILTIN(VAR) {
  RexxString       *variable;          /* specified variable name           */
  RexxVariableBase *retriever;         /* variable retriever                */
  RexxObject       *result;            /* function result                   */

  fixArgs(VAR);                       /* expand arguments to full value    */
                                       /* get the variable name             */
  variable = requiredString(VAR, name);
                                       /* get a variable retriever          */
  retriever = context->getVariableRetriever(variable);
  if (retriever == OREF_NULL)          /* invalid variable name?            */
    result = TheFalseObject;           /* return the 'BAD' result           */
  else if (isString((RexxObject *)retriever))   /* directly returned a string?       */
    result = TheFalseObject;           /* this doesn't exist either         */
  else {                               /* need to perform lookup            */
                                       /* get the variable value            */
    result = retriever->exists(context) ? TheTrueObject : TheFalseObject;
  }
  return result;                       /* return the indicator              */
}

#define VALUE_MIN 1
#define VALUE_MAX 3
#define VALUE_name     1
#define VALUE_newValue 2
#define VALUE_selector 3

BUILTIN(VALUE) {
  RexxString  *variable;               /* specified variable name           */
  RexxVariableBase *retriever;         /* variable retriever                */
  RexxObject  *newvalue;               /* new variable value                */
  RexxObject  *result;                 /* function result                   */
  RexxString  *selector;               /* variable pool selector            */

  fixArgs(VALUE);                     /* expand arguments to full value    */
                                       /* get the variable name             */
  variable = requiredString(VALUE, name);
                                       /* get the new value                 */
  newvalue = optionalArgument(VALUE, newValue);
                                       /* and the selector                  */
  selector = optionalString(VALUE, selector);
  if (selector == OREF_NULL) {         /* have a selector?                  */
                                       /* get a variable retriever          */
    retriever = context->getVariableRetriever(variable);
    if ((retriever == OREF_NULL) ||
        ((newvalue != OREF_NULL) && (variable->isSymbol() == STRING_NUMERIC)))  /* invalid variable name? */
      reportException(Error_Incorrect_call_symbol, new_string(CHAR_VALUE), IntegerOne, variable);
    else {                             /* need to perform lookup            */
                                       /* get the variable value            */
      result = retriever->getValue(context);
      if (newvalue != OREF_NULL)       /* have a new value to assign?       */
                                       /* do the assignment                 */
        retriever->assign(context, stack, newvalue);
    }
  }
  else if (selector->getLength() == 0) {    /* null string selector?             */
                                       /* get the existing value            */
    result = RexxInterpreter::getEnvironment(variable);
    if (result == OREF_NULL)           /* not in the environment?           */
                                       /* turn into ".VARIABLE" as value    */
      result = ((RexxString *)OREF_PERIOD)->concat(variable->upper());
    if (newvalue != OREF_NULL)         /* have a new value?                 */
                                       /* do the set also                   */
      RexxInterpreter::setEnvironment(variable, newvalue);
  }
  else {                               /* external value function           */
                                       /* need to go external on this       */
    result = SysInterpreter::valueFunction(variable, newvalue, selector);
  }
  return result;                       /* return the indicator              */
}

#define ABS_MIN 1
#define ABS_MAX 1
#define ABS_n   1

BUILTIN(ABS) {
    fixArgs(ABS);                       /* check on required number of args  */
    /* get the argument in question      */
    RexxObject *argument = getArg(ABS, n);
    if (isInteger(argument)) {      /* integer object already?           */
        /* we can process this without conversion */
        return ((RexxInteger *)argument)->abs();
    }
    else if (isOfClass(NumberString, argument)) { /* how about already numeric?        */
        /* we can process this without conversion */
        return ((RexxNumberString *)argument)->abs();
    }
    /* force to a string object          */
    RexxString *n = requiredString(ABS, n);
    return n->abs();                   /* invoke the string ABS function    */
}

#define SIGN_MIN 1
#define SIGN_MAX 1
#define SIGN_n   1

BUILTIN(SIGN) {
    fixArgs(SIGN);                       /* check on required number of args  */
    /* get the argument in question      */
    RexxObject *argument = getArg(SIGN, n);
    if (isInteger(argument)) {       /* integer object already?           */
        /* we can process this without conversion */
        return ((RexxInteger *)argument)->sign();
    }
    else if (isOfClass(NumberString, argument)) { /* how about already numeric?        */
        /* we can process this without conversion */
        return ((RexxNumberString *)argument)->Sign();
    }
    /* force to a string object          */
    RexxString *n = requiredString(SIGN, n);
    return n->sign();                     /* invoke the string SIGN function    */
}

#define FORMAT_MIN 1
#define FORMAT_MAX 5
#define FORMAT_number 1
#define FORMAT_before 2
#define FORMAT_after  3
#define FORMAT_expp   4
#define FORMAT_expt   5

BUILTIN(FORMAT) {
  RexxString  *number;                 /* number to format                  */
  RexxInteger *before;                 /* before part                       */
  RexxInteger *after;                  /* after digits                      */
  RexxInteger *expp;                   /* expp value                        */
  RexxInteger *expt;                   /* expt value                        */

  fixArgs(FORMAT);                    /* check on required number of args  */
                                       /* force to a string object          */
  number = requiredString(FORMAT, number);
                                       /* before value is optional          */
  before = optionalInteger(FORMAT, before);
                                       /* after value is optional           */
  after = optionalInteger(FORMAT, after);
                                       /* expp value is optional            */
  expp = optionalInteger(FORMAT, expp);
                                       /* expt value is optional            */
  expt = optionalInteger(FORMAT, expt);
                                       /* invoke the string FORMAT function   */
  return number->format(before, after, expp, expt);
}

#define MAX_MIN 1
#define MAX_MAX argcount
#define MAX_target 1

BUILTIN(MAX) {
  checkArgs(MAX);                     /* check on required args            */
  /* get the argument in question      */
  RexxObject *argument = getArg(MAX, target);
  if (isOfClass(NumberString, argument)) { /* how about already numeric?        */
      /* we can process this without conversion */
      return ((RexxNumberString *)argument)->Max(stack->arguments(argcount - 1), argcount - 1);
  }
                                       /* get the target string             */
  RexxString *target = requiredString(MAX, target);
  /* go perform the MIN function       */
  return target->Max(stack->arguments(argcount - 1), argcount - 1);
}

#define MIN_MIN 1
#define MIN_MAX argcount
#define MIN_target 1

BUILTIN(MIN) {
  checkArgs(MIN);                     /* check on required args            */
  /* get the argument in question      */
  RexxObject *argument = getArg(MIN, target);
  if (isOfClass(NumberString, argument)) { /* how about already numeric?        */
      /* we can process this without conversion */
      return ((RexxNumberString *)argument)->Min(stack->arguments(argcount - 1), argcount - 1);
  }
                                       /* get the target string             */
  RexxString *target = requiredString(MIN, target);
  /* go perform the MIN function       */
  return target->Min(stack->arguments(argcount - 1), argcount - 1);
}

#define SOURCELINE_MIN 0
#define SOURCELINE_MAX 1
#define SOURCELINE_n   1

BUILTIN(SOURCELINE) {
  stringsize_t line_number;            /* requested error number            */
  RexxObject  *result;                 /* function result                   */
  RexxSource  *source;                 /* current program source            */
  stringsize_t size;                   /* size of source program            */

  fixArgs(SOURCELINE);                /* check on required number of args  */
  source = context->getSource();       /* get current source object         */
  size = source->sourceSize();         /* get the program size              */
  if (argcount == 1) {                 /* asking for a specific line?       */
                                       /* get the line number               */
    line_number = requiredInteger(SOURCELINE, n)->stringSize();
                                       /* must be a positive integer        */
    positiveInteger(line_number, SOURCELINE, IntegerOne);
    if (line_number > size)            /* larger than program source?       */
                                       /* this is an error too?             */
      reportException(Error_Incorrect_call_sourceline, (wholenumber_t)line_number, (wholenumber_t)size);
                                       /* get the specific line             */
    result = (RexxObject *)source->get(line_number);
  }
  else
                                       /* just return the source size       */
    result = (RexxObject *)new_integer(size);
  return result;                       /* finished                          */
}

#define TRACE_MIN 0
#define TRACE_MAX 1
#define TRACE_setting 1

BUILTIN(TRACE) {
  RexxString  *result;                 /* returned result                   */
  RexxString  *setting;                /* new trace setting                 */
  int    newsetting;                   /* new trace setting                 */
  int    debug;                        /* new debug setting                 */

  fixArgs(TRACE);                     /* check required arguments          */
                                       /* get the trace setting             */
  setting = optionalString(TRACE, setting);
  result = context->traceSetting();    /* get the existing trace setting    */
  if (setting != OREF_NULL) {          /* have a new setting?               */
    context->source->parseTraceSetting(setting, &newsetting, &debug);
                                       /* now change the setting            */
    context->setTrace(newsetting, debug);
  }
  return result;                       /* return old trace setting          */
}

RexxObject *resolve_stream(            /* resolve a stream name             */
  RexxString          *name,           /* name of the stream                */
  RexxActivation      *context,        /* current activation context        */
  RexxExpressionStack *stack,          /* current expression stack          */
  bool                 input,          /* input or an output stream         */
  RexxString          **fullName,      /* qualified name of stream          */
  bool                *added)          /* not found -> true                 */
/******************************************************************************/
/* Function:  Convert a stream name into a stream object                      */
/******************************************************************************/
{
  RexxObject    *stream;               /* associated stream                 */
  RexxDirectory *streamTable;          /* current set of open streams       */
  RexxObject    *streamClass;          /* current stream class              */
  RexxString    *qualifiedName;        /* qualified file name               */
  RexxDirectory *securityArgs;         /* security check arguments          */

  if (added) *added = false;           /* when caller requires stream table entry then initialize */
  streamTable = context->getStreams(); /* get the current stream set        */
  if (fullName)                        /* fullName requested?               */
    *fullName = name;                  /* initialize to name                */
  /* if length of name is 0, then it's the same as omitted */
  if (name == OREF_NULL || name->getLength() == 0) { /* no name?                 */
    if (input) {                       /* input operation?                  */
                                       /* get the default output stream     */
      stream = context->getFromLocalEnvironment(OREF_INPUT);
    }
    else {
                                       /* get the default output stream     */
      stream = context->getFromLocalEnvironment(OREF_OUTPUT);
    }
  }
                                       /* standard input stream?            */
  else if (name->strICompare(CHAR_STDIN) || name->strICompare(CHAR_CSTDIN))
                                       /* get the default output stream     */
    stream = context->getFromLocalEnvironment(OREF_INPUT);
                                       /* standard output stream?           */
  else if (name->strICompare(CHAR_STDOUT) || name->strICompare(CHAR_CSTDOUT))
                                       /* get the default output stream     */
    stream = context->getFromLocalEnvironment(OREF_OUTPUT);
                                       /* standard error stream?            */
  else if (name->strICompare(CHAR_STDERR) || name->strICompare(CHAR_CSTDERR))
                                       /* get the default output stream     */
    stream = context->getFromLocalEnvironment(OREF_ERRORNAME);
  else {
                                       /* go get the qualified name         */
    qualifiedName = (RexxString *)SysInterpreter::qualifyFileSystemName(name);
    if (fullName)                      /* fullName requested?               */
      *fullName = qualifiedName;       /* provide qualified name            */
    stack->push(qualifiedName);        /* Protect from GC.                  */
    /* Note: stream name is pushed to the stack to be protected from GC;    */
    /* e.g. it is used by the caller to remove stream from stream table.    */
    /* The stack will be reset after the function was executed and the      */
    /* protection is released                                               */
                                       /* see if we've already opened this  */
    stream = streamTable->at(qualifiedName);
    if (stream == OREF_NULL) {         /* not open                          */
                                       /* need to secure this?              */
      if (context->hasSecurityManager()) {
        securityArgs = new_directory();/* get the information directory     */
                                       /* put the name in the directory     */
        securityArgs->put(qualifiedName, OREF_NAME);
        if (context->callSecurityManager(OREF_STREAM, securityArgs)) {
          stream = securityArgs->fastAt(OREF_STREAM);
          if (stream == OREF_NULL)     /* in an expression and need a result*/
                                       /* need to raise an exception        */
            reportException(Error_No_result_object_message, OREF_STREAM);
                                       /* add to the streams table          */
          streamTable->put(stream, qualifiedName);
          return stream;               /* return the stream object          */
        }
      }
                                       /* get the stream class              */
      streamClass = RexxInterpreter::getEnvironment(OREF_STREAM);
                                       /* create a new stream object        */
      stream = streamClass->sendMessage(OREF_NEW, name);

      if (added) {                     /* open the stream?   begin          */
                                       /* add to the streams table          */
        streamTable->put(stream, qualifiedName);
        *added = true;                 /* mark it as added to stream table  */
      }
    }
  }

  return stream;                       /* return the stream object          */
}

bool check_queue(                      /* check to see if stream is to queue*/
  RexxString *name)                    /* namve of the stream               */
/******************************************************************************/
/* Function:  Check to see if a stream name is a queue                        */
/******************************************************************************/
{
  if (name != OREF_NULL)               /* non-default name?                 */
    return name->strICompare((stringchar_t *)"QUEUE:");/* compare against the queue         */
  else return false;                   /* not the queue                     */
}

#define LINEIN_MIN 0
#define LINEIN_MAX 3
#define LINEIN_name   1
#define LINEIN_line   2
#define LINEIN_count  3

BUILTIN(LINEIN) {
  RexxObject   *stream;                /* target stream                     */
  RexxInteger  *line;                  /* target line                       */
  RexxInteger  *count;                 /* count of lines                    */
  RexxString   *name;                  /* stream name                       */
  RexxString   *result;                /* linein result                     */
  bool          added;                 /* add to stream table               */

  fixArgs(LINEIN);                    /* check required arguments          */

  name = optionalString(LINEIN, name);/* get the string name               */
                                       /* get the line position             */
  line = optionalInteger(LINEIN, line);
                                       /* and the optional count of lines   */
  count = optionalInteger(LINEIN, count);
  if (check_queue(name)) {             /* is this "QUEUE:"                  */
                                       /* if exit declines call             */
    if (context->activity->callQueuePullExit(context, &result)) {
                                       /* get the default output stream     */
        stream = context->getFromLocalEnvironment(OREF_REXXQUEUE);
                                       /* pull from the queue               */
        result = (RexxString *)stream->sendMessage(OREF_LINEIN);
    }
  }
  else {
                                       /* get a stream for this name        */
    stream = resolve_stream(name, context, stack, true, NULL, &added);
    switch (argcount) {                /* process according to argcount     */
      case 0:                          /* no name                           */
      case 1:                          /* name only                         */
        result = (RexxString *)stream->sendMessage(OREF_LINEIN);
        break;
      case 2:                          /* name and start                    */
        result = (RexxString *)stream->sendMessage(OREF_LINEIN, line);
        break;
      case 3:                          /* name, start and count             */
        result = (RexxString *)stream->sendMessage(OREF_LINEIN, line, count);
        break;
    }
  }
  return result;                       /* all finished                      */
}

#define CHARIN_MIN 0
#define CHARIN_MAX 3
#define CHARIN_name   1
#define CHARIN_start  2
#define CHARIN_count  3

BUILTIN(CHARIN) {
  RexxObject   *stream;                /* target stream                     */
  RexxInteger  *position;              /* target position                   */
  RexxInteger  *count;                 /* count of lines                    */
  RexxString   *name;                  /* stream name                       */
  RexxString   *result;                /* linein result                     */
  bool          added;                 /* add to stream table               */

  fixArgs(CHARIN);                    /* check required arguments          */
                                       /* get the string name               */
  name = optionalString(CHARIN, name);
                                       /* get the line position             */
  position = optionalInteger(CHARIN, start);
                                       /* and the optional count of chars   */
  count = optionalInteger(CHARIN, count);
  if (check_queue(name))               /* is this "QUEUE:"                  */
                                       /* this isn't allowed                */
    reportException(Error_Incorrect_call_queue_no_char, OREF_CHARIN);

                                       /* get a stream for this name        */
  stream = resolve_stream(name, context, stack, true, NULL, &added);
  switch (argcount) {                  /* process according to argcount     */
    case 0:                            /* no name                           */
    case 1:                            /* name only                         */
      result = (RexxString *)stream->sendMessage(OREF_CHARIN);
      break;
    case 2:                            /* name and string                   */
      result = (RexxString *)stream->sendMessage(OREF_CHARIN, position);
      break;
    case 3:                            /* name, string and line             */
      result = (RexxString *)stream->sendMessage(OREF_CHARIN, position, count);
      break;
  }
  return result;                       /* return final result               */
}

#define LINEOUT_MIN 0
#define LINEOUT_MAX 3
#define LINEOUT_name   1
#define LINEOUT_string 2
#define LINEOUT_line   3

BUILTIN(LINEOUT) {
  RexxObject   *stream;                /* target stream                     */
  RexxInteger  *line;                  /* target position                   */
  RexxString   *name;                  /* stream name                       */
  RexxString   *result;                /* linein result                     */
  RexxString   *string;                /* target string                     */
  RexxString   *fullName;              /* fully qual'd stream name          */
  bool          added;                 /* add to stream table               */

  fixArgs(LINEOUT);                   /* check required arguments          */
                                       /* get the string name               */
  name = optionalString(LINEOUT, name);
                                       /* get the output string             */
  string = optionalString(LINEOUT, string);
                                       /* get the line position             */
  line = optionalInteger(LINEOUT, line);
  if (check_queue(name)) {             /* is this "QUEUE:"                  */
                                       /* if exit declines call             */
    if (context->activity->callQueuePushExit(context, string, QUEUE_FIFO)) {
      if (string != OREF_NULL) {       /* have an actual string to write?   */
                                       /* get the default output stream     */
        stream = context->getFromLocalEnvironment(OREF_REXXQUEUE);
                                       /* push onto the queue               */
        result = (RexxString *)stream->sendMessage(OREF_QUEUENAME, string);
      }
      else
                                       /* always a zero residual            */
        result = (RexxString *)IntegerZero;
    }
  }
  else {
                                       /* get a stream for this name        */
    stream = resolve_stream(name, context, stack, false, &fullName, &added);
    switch (argcount) {                /* process according to argcount     */
      case 0:                          /* no name                           */
      case 1:                          /* name only                         */
        result = (RexxString *)stream->sendMessage(OREF_LINEOUT);
        break;
      case 2:                          /* name and string                   */
        result = (RexxString *)stream->sendMessage(OREF_LINEOUT, string);
        break;
      case 3:                          /* name, string and line             */
        result = (RexxString *)stream->sendMessage(OREF_LINEOUT, string, line);
        break;
    }
  }
  return result;                       /* all finished                      */
}

#define CHAROUT_MIN 0
#define CHAROUT_MAX 3
#define CHAROUT_name   1
#define CHAROUT_string 2
#define CHAROUT_start  3

BUILTIN(CHAROUT) {
  RexxObject   *stream;                /* target stream                     */
  RexxInteger  *position;              /* target position                   */
  RexxString   *name;                  /* stream name                       */
  RexxString   *result;                /* linein result                     */
  RexxString   *string;                /* target string                     */
  RexxString   *fullName;              /* fully qual'd stream name          */
  bool          added;                 /* add to stream table               */

  fixArgs(CHAROUT);                   /* check required arguments          */
                                       /* get the string name               */
  name = optionalString(CHAROUT, name);
                                       /* get the output string             */
  string = optionalString(CHAROUT, string);
                                       /* get the line position             */
  position = optionalInteger(CHAROUT, start);
  if (check_queue(name))               /* is this "QUEUE:"                  */
                                       /* this isn't allowed                */
    reportException(Error_Incorrect_call_queue_no_char, OREF_CHAROUT);
                                       /* get a stream for this name        */
  stream = resolve_stream(name, context, stack, false, &fullName, &added);
  switch (argcount) {                  /* process according to argcount     */
    case 0:                            /* no name                           */
    case 1:                            /* name only                         */
      result = (RexxString *)stream->sendMessage(OREF_CHAROUT);
      break;
    case 2:                            /* name and string                   */
      result = (RexxString *)stream->sendMessage(OREF_CHAROUT, string);
      break;
    case 3:                            /* name, string and line             */
      result = (RexxString *)stream->sendMessage(OREF_CHAROUT, string, position);
      break;
  }
  return result;                       /* all finished                      */
}

#define LINES_MIN 0
#define LINES_MAX    2
#define LINES_name   1
#define LINES_option 2

BUILTIN(LINES) {
  RexxObject   *stream;                /* target stream                     */
  RexxString   *name;                  /* stream name                       */
  RexxString   *option;                /* option: "N" or "C"                */
  RexxInteger  *result;                /* linein result                     */
  bool          added;                 /* add to stream table               */
  RexxString    *quick;

  fixArgs(LINES);                     /* check required arguments          */

  name = optionalString(LINES, name); /* get the string name               */
  option = optionalString(LINES, option);
  if (check_queue(name)) {             /* is this "QUEUE:"                  */
                                       /* get the default output stream     */
    stream = context->getFromLocalEnvironment(OREF_REXXQUEUE);
                                       /* return count on the queue         */
    result = (RexxInteger *)stream->sendMessage(OREF_QUERY);
  }
  else
  {
                                       /* get a stream for this name        */
    stream = resolve_stream(name, context, stack, true, NULL, &added);

    if (option != OREF_NULL)
      switch (option->getChar(0)) {      /* process the option character      */
        case 'C':
        case 'c':
          quick = new_string("C");
          break;
        case 'N':
        case 'n':
          quick = new_string("N");
          break;
        case '\0':                        /* argument 'O'mitted?               */
          quick = new_string("N");
          break;
        default:                         /* unknown option                    */
                                         /* this is an error                  */
          reportException(Error_Incorrect_call_list, new_string(CHAR_ARG), IntegerTwo, new_string("NC"), option);
          break;
    }
    else
      quick = new_string("N");

    /* use modified LINES method with quick flag       */
    result = (RexxInteger *)stream->sendMessage(OREF_LINES, quick);
  }
                                       /* for compatibility this needs      */
                                       /* to only return 0 or 1             */
  if (quick->getChar(0) == 'N')
    return (result != IntegerZero) ? IntegerOne : IntegerZero;
  else
    return result;
}

#define CHARS_MIN 0
#define CHARS_MAX 1
#define CHARS_name   1

BUILTIN(CHARS) {
  RexxObject   *stream;                /* target stream                     */
  RexxString   *name;                  /* stream name                       */
  bool          added;                 /* add to stream table               */

  fixArgs(CHARS);                     /* check required arguments          */

  name = optionalString(CHARS, name); /* get the string name               */
  if (check_queue(name))               /* is this "QUEUE:"                  */
                                       /* this isn't allowed                */
    reportException(Error_Incorrect_call_queue_no_char, OREF_CHARS);
                                       /* get a stream for this name        */
  stream = resolve_stream(name, context, stack, true, NULL, &added);
  return stream->sendMessage(OREF_CHARS);
}

#define STREAM_MIN 1
#undef STREAM_MAX                      /* already defined in AIX            */
#define STREAM_MAX 3
#define STREAM_name      1
#define STREAM_operation 2
#define STREAM_command   3

#define STREAM_STATUS      'S'
#define STREAM_DESCRIPTION 'D'
#define STREAM_COMMAND     'C'

BUILTIN(STREAM)
{
    RexxObject   *stream;                /* target stream                     */
    RexxString   *name;                  /* stream name                       */
    RexxString   *action;                /* stream action                     */
    RexxString   *command;               /* stream command                    */
    RexxString   *command_upper;         /* stream command upper case         */
    RexxObject   *result;                /* function result                   */
    char          action_char;           /* reduced action character          */
    RexxString   *fullName;              /* fully qual'd stream name          */
    bool          added;
    bool          fOpen = false;         /* open flag                         */
    bool          fClose = false;        /* close flag                        */

    fixArgs(STREAM);                    /* check required arguments          */
    /* get the string name               */
    name = requiredString(STREAM, name);
    if (name->getLength() == 0)               /* check name validity               */
        /* raise an error                    */
        reportException(Error_Incorrect_call_stream_name, OREF_STREAM, name);
    /* get any operation                 */
    action = optionalString(STREAM, operation);
    /* get any command                   */
    command = optionalString(STREAM, command);
    /* get a stream for this name        */
    /* we need to now, whether the stream already was in the stream table (added = false) */
//stream = resolve_stream(name, context, stack, true, &added);

    if (action == OREF_NULL)
    {           /* no action given?                  */
        action_char = STREAM_STATUS;       /* this is a status attempt          */
    }
    else
    {
        if (action->getLength() == 0)
        {         /* get a null string?                */
            /* this is an error                  */
            reportException(Error_Incorrect_call_list, OREF_STREAM, IntegerTwo, new_string("SDC"), action);
        }
        else
        {
            /* get the option character          */
            action_char = toupper(action->getChar(0));
        }
    }
    switch (action_char)
    {               /* process the options               */
        case STREAM_STATUS:                /* stream(name, s)                   */
            if (argcount > 2)
            {              /* given a third argument?           */
                           /* raise an error                    */
                reportException(Error_Incorrect_call_maxarg, OREF_STREAM, IntegerTwo);
            }
            stream = resolve_stream(name, context, stack, true, NULL, NULL);
            /* get the stream state              */
            result = stream->sendMessage(OREF_STATE);
            break;

        case STREAM_DESCRIPTION:           /* stream(name, d)                   */
            if (argcount > 2)
            {              /* given a third argument?           */
                           /* raise an error                    */
                reportException(Error_Incorrect_call_maxarg, OREF_STREAM, IntegerTwo);
            }
            stream = resolve_stream(name, context, stack, true, NULL, NULL);
            /* get the stream description        */
            result = stream->sendMessage(OREF_DESCRIPTION);
            break;

        case STREAM_COMMAND:               /* stream(name, c, command)          */
            {
                if (argcount < 3)
                {              /* given a third argument?           */
                               /* raise an error                    */
                    reportException(Error_Incorrect_call_minarg, OREF_STREAM, IntegerThree);
                }
                /* get the stream description        */
                ProtectedObject p1(command);

                /* I have to check the command twice because in the RexxMethods (i.g. query_exists)
                   I don't have access to the activation and thus not to the streamtable.
                   It's also not possible to pass context as the second argument because
                   stream is a RexxMethod and USE ARG RexxActivation is not possible */
                command_upper = command->upper();

                ProtectedObject p2(command_upper);


                fOpen = fClose = false;    /* this whole part was reworked and moved up here */
                if (command_upper->wordPos(new_string("OPEN"), OREF_NULL)->wholeNumber() > 0)
                {
                    fOpen = true;
                    stream = resolve_stream(name, context, stack, true, &fullName, &added);
                }
                else if (command_upper->wordPos(new_string("CLOSE"), OREF_NULL)->wholeNumber() > 0)
                {
                    fClose = true;
                    stream = resolve_stream(name, context, stack, true, &fullName, &added);
                }
                else
                    stream = resolve_stream(name, context, stack, true, NULL, NULL);

                result = stream->sendMessage(OREF_COMMAND, command);

                /* this repairs the removed code below */
                if (fClose)
                {
                    context->getStreams()->remove(fullName);
                }
                else if (added && fOpen)
                {
                    /* if open failed, remove the stream object from stream table again */
                    if (memcmp("READY:", ((RexxString*) result)->getStringData(), 6) != 0)
                    {
                        context->getStreams()->remove(fullName);
                    }
                }
                break;

            }
        default:
            /* this is an error                  */
            reportException(Error_Incorrect_call_list, OREF_STREAM, IntegerTwo, new_string("SDC"), action);
            break;
    }
    return result;                       /* return the function result        */
}

#define QUEUED_MIN 0
#define QUEUED_MAX 0

BUILTIN(QUEUED) {
  RexxObject   *queue;                 /* current queue object              */
  RexxInteger  *queuesize;             /* returned queue size from sys exit */

  checkArgs(QUEUED);                  /* check on required number of args  */
                                       /* get the default output stream     */
  if (context->activity->callQueueSizeExit(context, &queuesize)) {
    queue = context->getFromLocalEnvironment(OREF_REXXQUEUE);
                                       /* return count on the queue         */
    return queue->sendMessage(OREF_QUERY);
  }
  else
    return queuesize;                  /* return count from system exit     */
}

#define CONDITION_MIN 0
#define CONDITION_MAX 1
#define CONDITION_option 1

BUILTIN(CONDITION) {
  stringchar_t   style;                /* style of condition output         */
  RexxString    *option;               /* function option                   */
  RexxDirectory *conditionobj;         /* current trapped condition object  */
  RexxObject    *result;               /* returned result                   */

  fixArgs(CONDITION);                 /* expand arguments to full value    */
                                       /* get the option string             */
  option = optionalString(CONDITION, option);
  if (option == OREF_NULL)             /* just using default format?        */
    style = 'I';                       /* use the 'Normal form              */
  else if (option->getLength() == 0)        /* have a null string?               */
                                       /* this is an error                  */
    reportException(Error_Incorrect_call_list, new_string(CHAR_CONDITION), IntegerOne, new_string("ACDIOS"), option);
  else                                 /* need to process an option         */
                                       /* option is first character         */
    style = toupper(option->getChar(0));
                                       /* get current trapped condition     */
  conditionobj = context->getConditionObj();

  result = OREF_NULLSTRING;            /* use a nullstring safety net       */
  switch (style) {                     /* process various CONDITION objects */

    case 'A':                          /* 'A'dditional                      */
      if (conditionobj != OREF_NULL) { /* have a condition object?          */
                                       /* retrieve the additional info      */
        result = conditionobj->at(OREF_ADDITIONAL);
        if (result == OREF_NULL)       /* not there?                        */
          result = TheNilObject;       /* return .nil                       */
        else
          result = result->copy();     /* copy the result info              */
      }
      else
        result = TheNilObject;         /* return .nil if not there          */
      break;

    case 'I':                          /* 'I'nstruction                     */
      if (conditionobj != OREF_NULL)   /* have a condition object?          */
                                       /* retrieve the instruction info     */
        result = conditionobj->at(OREF_INSTRUCTION);
      break;

    case 'D':                          /* 'D'escription                     */
      if (conditionobj != OREF_NULL) { /* have a condition object?          */
                                       /* retrieve the description info     */
        result = conditionobj->at(OREF_DESCRIPTION);
        if (result == OREF_NULL)       /* not found?                        */
          result = OREF_NULLSTRING;    /* return a null string if nothing   */
      }
      break;

    case 'C':                          /* 'C'ondition name                  */
      if (conditionobj != OREF_NULL)   /* have a condition object?          */
                                       /* retrieve the condition name       */
        result = conditionobj->at(OREF_CONDITION);
      break;

    case 'O':                          /* 'C'ondition name                  */
      if (conditionobj != OREF_NULL)   /* have a condition object?          */
        result = conditionobj->copy(); /* just return a copy of this        */
      else
        result = TheNilObject;         /* return the NIL object             */
      break;

    case 'S':                          /* 'S'tate                           */
      if (conditionobj != OREF_NULL)   /* have a condition object?          */
                                       /* get the current trap state        */
        result = context->trapState((RexxString *)conditionobj->at(OREF_CONDITION));
      break;

    default:                           /* unknown option                    */
                                       /* report an error                   */
      reportException(Error_Incorrect_call_list, new_string(CHAR_CONDITION), IntegerOne, new_string("ACDIOS"), option);
      break;
  }
  return result;                       /* return requested value            */
}

#define CHANGESTR_MIN 3
#define CHANGESTR_MAX 3
#define CHANGESTR_needle     1
#define CHANGESTR_haystack   2
#define CHANGESTR_newneedle  3

BUILTIN(CHANGESTR) {
  RexxString *needle;                  /* needle to change                  */
  RexxString *haystack;                /* target haystack                   */
  RexxString *newneedle;               /* new, replacement string           */

  fixArgs(CHANGESTR);                 /* check on require number of args   */
                                       /* get string for new                */
  needle = requiredString(CHANGESTR, needle);
                                       /* get string for target             */
  haystack = requiredString(CHANGESTR, haystack);
                                       /* get string to change to           */
  newneedle = requiredString(CHANGESTR, newneedle);
                                       /* go perform the pos function       */
  return haystack->changeStr(needle, newneedle);
}

#define COUNTSTR_MIN 2
#define COUNTSTR_MAX 2
#define COUNTSTR_needle     1
#define COUNTSTR_haystack   2

BUILTIN(COUNTSTR) {
  RexxString *needle;                  /* needle to change                  */
  RexxString *haystack;                /* target haystack                   */
  stringsize_t  count;                 /* returned count                    */

  fixArgs(COUNTSTR);                  /* check on require number of args   */
                                       /* get string for new                */
  needle = requiredString(COUNTSTR, needle);
                                       /* get string for target             */
  haystack = requiredString(COUNTSTR, haystack);
  count = haystack->countStr(needle);  /* go perform the countstr function  */
  return new_integer(count);           /* return the new count              */
}

#define RXFUNCADD_MIN 2
#define RXFUNCADD_MAX 3
#define RXFUNCADD_name   1
#define RXFUNCADD_module 2
#define RXFUNCADD_proc   3

BUILTIN(RXFUNCADD)
{
  RexxString  *name;                   /* registered function name          */
  RexxString  *module;                 /* source library module             */
  RexxString  *proc;                   /* library procedure name            */

  fixArgs(RXFUNCADD);                 /* check on required number of args  */

  // we require a name and module, but the
  // procedure is optional.  If not specified, we
  // use the function name directly.
  name = requiredString(RXFUNCADD, name);
  module = requiredString(RXFUNCADD, module);
  proc = optionalString(RXFUNCADD, proc);

  if (proc == OREF_NULL)
  {
      proc = name;
  }

  // hand this off to the package manager.
  return ThePackageManager->addRegisteredFunction(name, module, proc);
}

#define RXFUNCDROP_MIN 1
#define RXFUNCDROP_MAX 1
#define RXFUNCDROP_name   1

BUILTIN(RXFUNCDROP)
{
  RexxString  *name;                   /* registered function name          */

  fixArgs(RXFUNCDROP);                 /* check on required number of args  */

  // only a name is required.
  name = requiredString(RXFUNCDROP, name);

  // hand this off to the package manager.
  return ThePackageManager->dropRegisteredFunction(name);
}

#define RXFUNCQUERY_MIN 1
#define RXFUNCQUERY_MAX 1
#define RXFUNCQUERY_name   1

BUILTIN(RXFUNCQUERY)
{
  RexxString  *name;                   /* registered function name          */

  fixArgs(RXFUNCQUERY);                 /* check on required number of args  */

  // only a name is required.
  name = requiredString(RXFUNCQUERY, name);

  // hand this off to the package manager.
  return ThePackageManager->dropRegisteredFunction(name);
}


#define QUEUEEXIT_MIN 1
#define QUEUEEXIT_MAX 1
#define QUEUEEXIT_name   1


// This somewhat funny function is implemented as a builtin because it
// requires quite a bit of internal access.
BUILTIN(QUEUEEXIT)
{
  RexxString  *name;                   /* registered function name          */

  fixArgs(QUEUEEXIT);                   /* check on required number of args  */

  // only a name is required.
  name = requiredString(QUEUEEXIT, name);
                                       /* call the exit                     */
  context->activity->callQueueNameExit(context, &name);
  // make sure we have real object to return
  if (name == OREF_NULL)
  {
      name = OREF_NULLSTRING;
  }
  return name;
}

#define SYSSETLOCAL_MIN 0
#define SYSSETLOCAL_MAX 0

BUILTIN(SYSSETLOCAL)
{
  checkArgs(SYSSETLOCAL);              /* check on required number of args  */
  // the external environment implements this
  return SysInterpreter::pushEnvironment(context);
}

#define SYSENDLOCAL_MIN 0
#define SYSENDLOCAL_MAX 0

BUILTIN(SYSENDLOCAL)
{
  checkArgs(SYSENDLOCAL);              /* check on required number of args  */
  // the external environment implements this
  return SysInterpreter::popEnvironment(context);
}

                                       /* the following builtin function    */
                                       /* table must maintain the same order*/
                                       /* as the builtin function codes used*/
                                       /* in the token class builtin        */
                                       /* builtin function lookup           */
BuiltinFunction *BuiltinFunctions::builtinFunctions[] = {
  NULL,                                /* NULL first entry as dummy         */
  &builtin_function_ABBREV           ,
  &builtin_function_ABS              ,
  &builtin_function_ADDRESS          ,
  &builtin_function_ARG              ,
  &builtin_function_B2X              ,
  &builtin_function_BITAND           ,
  &builtin_function_BITOR            ,
  &builtin_function_BITXOR           ,
  &builtin_function_C2D              ,
  &builtin_function_C2X              ,
  &builtin_function_CENTER           ,
  &builtin_function_CENTRE           ,
  &builtin_function_CHANGESTR        ,
  &builtin_function_CHARIN           ,
  &builtin_function_CHAROUT          ,
  &builtin_function_CHARS            ,
  &builtin_function_COMPARE          ,
  &builtin_function_CONDITION        ,
  &builtin_function_COPIES           ,
  &builtin_function_COUNTSTR         ,
  &builtin_function_D2C              ,
  &builtin_function_D2X              ,
  &builtin_function_DATATYPE         ,
  &builtin_function_DATE             ,
  &builtin_function_DELSTR           ,
  &builtin_function_DELWORD          ,
  &builtin_function_DIGITS           ,
  &builtin_function_ERRORTEXT        ,
  &builtin_function_FORM             ,
  &builtin_function_FORMAT           ,
  &builtin_function_FUZZ             ,
  &builtin_function_INSERT           ,
  &builtin_function_LASTPOS          ,
  &builtin_function_LEFT             ,
  &builtin_function_LENGTH           ,
  &builtin_function_LINEIN           ,
  &builtin_function_LINEOUT          ,
  &builtin_function_LINES            ,
  &builtin_function_LOWER            ,
  &builtin_function_MAX              ,
  &builtin_function_MIN              ,
  &builtin_function_OVERLAY          ,
  &builtin_function_POS              ,
  &builtin_function_QUEUED           ,
  &builtin_function_RANDOM           ,
  &builtin_function_REVERSE          ,
  &builtin_function_RIGHT            ,
  &builtin_function_RXFUNCADD        ,
  &builtin_function_RXFUNCDROP       ,
  &builtin_function_RXFUNCQUERY      ,
  &builtin_function_SIGN             ,
  &builtin_function_SOURCELINE       ,
  &builtin_function_SPACE            ,
  &builtin_function_STREAM           ,
  &builtin_function_STRIP            ,
  &builtin_function_SUBSTR           ,
  &builtin_function_SUBWORD          ,
  &builtin_function_SYMBOL           ,
  &builtin_function_SYSENDLOCAL      ,
  &builtin_function_SYSSETLOCAL      ,
  &builtin_function_TIME             ,
  &builtin_function_TRACE            ,
  &builtin_function_TRANSLATE        ,
  &builtin_function_TRUNC            ,
  &builtin_function_UPPER            ,
  &builtin_function_USERID           ,
  &builtin_function_VALUE            ,
  &builtin_function_VAR              ,
  &builtin_function_VERIFY           ,
  &builtin_function_WORD             ,
  &builtin_function_WORDINDEX        ,
  &builtin_function_WORDLENGTH       ,
  &builtin_function_WORDPOS          ,
  &builtin_function_WORDS            ,
  &builtin_function_X2B              ,
  &builtin_function_X2C              ,
  &builtin_function_X2D              ,
  &builtin_function_XRANGE           ,
  &builtin_function_QUEUEEXIT        ,
};

