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
/* REXX Kernel                                                  ArgumentUtilities.hpp       */
/*                                                                            */
/* REXX String method utility definitions                                     */
/*                                                                            */
/******************************************************************************/

#ifndef ArgumentUtilities_Included
#define ArgumentUtilities_Included

inline RexxString *stringArgument(RexxObject *s, size_t p) { return REQUIRED_STRING(s, p); }
inline RexxString * optionalStringArgument(RexxObject *o, RexxString *d, size_t p) { return (o == OREF_NULL ? d : stringArgument(o, p)); }
stringsize_t lengthArgument(RexxObject *, size_t);
inline stringsize_t optionalLengthArgument(RexxObject *o, stringsize_t d, size_t p) { return (o == OREF_NULL ? d : lengthArgument(o, p)); }
stringsize_t positionArgument(RexxObject *, size_t);
inline stringsize_t optionalPositionArgument(RexxObject *o, stringsize_t d, size_t p) { return (o == OREF_NULL ? d : positionArgument(o, p)); }
stringchar_t getPadCharacter(RexxObject *, size_t);
inline stringchar_t padArgument(RexxObject *o, stringchar_t d, size_t p) { return (o == OREF_NULL ? d : getPadCharacter(o, p)); }
stringchar_t getOptionCharacter(RexxObject *, size_t);
inline stringchar_t optionArgument(RexxObject *o, stringchar_t d, size_t p) { return (o == OREF_NULL ? d : getOptionCharacter(o, p)); }
inline stringsize_t optionalNonNegative(RexxObject *o, stringsize_t d, size_t p) { return (o == OREF_NULL ? d : o->requiredNonNegative(p)); }
inline stringsize_t optionalPositive(RexxObject *o, stringsize_t d, size_t p) { return (o == OREF_NULL ? d : o->requiredPositive(p)); }

RexxString *packHex(stringchar_t * String, stringsize_t StringLength);

#define ch_SPACE ' '
#define ch_NULL  '\0'
#define OVERFLOWSPACE 2                /* space for numeric buffer overflow */

                                       /* Strip function options     */
#define  STRIP_BOTH                'B'
#define  STRIP_LEADING             'L'
#define  STRIP_TRAILING            'T'
                                       /* Datatype function options  */
#define  DATATYPE_ALPHANUMERIC     'A'
#define  DATATYPE_BINARY           'B'
#define  DATATYPE_LOWERCASE        'L'
#define  DATATYPE_MIXEDCASE        'M'
#define  DATATYPE_NUMBER           'N'
#define  DATATYPE_SYMBOL           'S'
#define  DATATYPE_VARIABLE         'V'
#define  DATATYPE_UPPERCASE        'U'
#define  DATATYPE_WHOLE_NUMBER     'W'
#define  DATATYPE_HEX              'X'
#define  DATATYPE_9DIGITS          '9'
#define  DATATYPE_LOGICAL          'O' // lOgical.
                                       /* Verify function options    */
#define  VERIFY_MATCH              'M'
#define  VERIFY_NOMATCH            'N'

                                       /* character validation sets for the */
                                       /* datatype function                 */
static stringchar_t *HEX_CHAR_STR = (stringchar_t *)"0123456789ABCDEFabcdef";
static stringchar_t *HEX_DIGITS = (stringchar_t *)"0123456789ABCDEF";
static stringchar_t *ALPHANUM =
   (stringchar_t *)"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
static stringchar_t *BINARI = (stringchar_t *)"01";
static stringchar_t *LOWER_ALPHA = (stringchar_t *)"abcdefghijklmnopqrstuvwxyz";
static stringchar_t *MIXED_ALPHA = (stringchar_t *)
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
static stringchar_t *UPPER_ALPHA = (stringchar_t *)"ABCDEFGHIJKLMNOPQRSTUVWXYZ";

/*********************************************************************/
/*                                                                   */
/*      Name:                   IntToHexdigit                        */
/*                                                                   */
/*      Descriptive name:       convert int to hexadecimal digit     */
/*                                                                   */
/*      Returns:                A hexadecimal digit representing n.  */
/*                                                                   */
/*********************************************************************/

                                       /* convert the number                */
inline stringchar_t intToHexDigit(int n) { return HEX_DIGITS[n]; }

#endif
