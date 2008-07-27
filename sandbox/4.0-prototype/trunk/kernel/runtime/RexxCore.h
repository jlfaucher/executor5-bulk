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
/* REXX Kernel                                                  RexxCore.h      */
/*                                                                            */
/* Global Declarations                                                        */
/******************************************************************************/

/******************************************************************************/
/* Globally required include files                                            */
/******************************************************************************/
#ifndef RexxCore_Included
#define RexxCore_Included

/* ANSI C definitions */
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include <new>

#define OREF_NULL NULL                 /* definition of a NULL REXX object  */

#include "PlatformTypes.h"             /* get the base types                */

/******************************************************************************/
/* Portable type definitions                                                  */
/******************************************************************************/
typedef size_t stringsize_t;         // size used for string sizes and positions
typedef size_t arraysize_t;          // size used for array sizes and positions
typedef unsigned char stringchar_t;  // string character data
typedef ssize_t wholenumber_t;       // Rexx whole number
typedef ssize_t HashCode;            // a hash code value

#ifndef _REXXDATETIME
#define _REXXDATETIME
typedef struct _REXXDATETIME {         /* REXX time stamp format            */
  uint16_t       hours;                /* hour of the day (24-hour)         */
  uint16_t       minutes;              /* minute of the hour                */
  uint16_t       seconds;              /* second of the minute              */
  uint16_t       hundredths;           /* hundredths of a second            */
  uint16_t       day;                  /* day of the month                  */
  uint16_t       month;                /* month of the year                 */
  uint16_t       year;                 /* current year                      */
  uint16_t       weekday;              /* day of the week                   */
  uint32_t       microseconds;         /* microseconds                      */
  uint32_t       yearday;              /* day number within the year        */
  bool           valid;                /* valid time stamp marker           */
} REXXDATETIME;
#endif

// this pulls in any platform specific defintions related to the
// kernel->platform interface
#include "PlatformDefinitions.h"       /* include platform definitions first*/
#include "RexxErrorCodes.h"            /* error code definitions            */
#include "RexxConstants.hpp"

/******************************************************************************/
/* Kernel Internal Limits                                                     */
/******************************************************************************/

// move into parser routines
const int MAX_ERROR_NUMBER = 99999;        /* maximum error code number         */
const int MAX_SYMBOL_LENGTH = 250;         /* length of a symbol name           */
const int MAX_STRING_LENGTH = 250;         /* length of a literal string        */
const int MAX_HEX_LENGTH    = 250;         /* packed length of hex literal      */
const int MAX_BIN_LENGTH    = 250;         /* packed length of binary literal   */

/******************************************************************************/
/* Defines for argument error reporting                                       */
/******************************************************************************/

const stringsize_t ARG_ONE    = 1;
const stringsize_t ARG_TWO    = 2;
const stringsize_t ARG_THREE  = 3;
const stringsize_t ARG_FOUR   = 4;
const stringsize_t ARG_FIVE   = 5;
const stringsize_t ARG_SIX    = 6;
const stringsize_t ARG_SEVEN  = 7;
const stringsize_t ARG_EIGHT  = 8;
const stringsize_t ARG_NINE   = 9;
const stringsize_t ARG_TEN    = 10;

/******************************************************************************/
/* Constants used for trace prefixes                                          */
/******************************************************************************/

enum TracePrefixes {
    TRACE_PREFIX_CLAUSE   ,
    TRACE_PREFIX_ERROR    ,
    TRACE_PREFIX_RESULT   ,
    TRACE_PREFIX_DUMMY    ,
    TRACE_PREFIX_VARIABLE ,
    TRACE_PREFIX_LITERAL  ,
    TRACE_PREFIX_FUNCTION ,
    TRACE_PREFIX_PREFIX   ,
    TRACE_PREFIX_OPERATOR ,
    TRACE_PREFIX_COMPOUND ,
    TRACE_PREFIX_MESSAGE
};

#define MAX_TRACEBACK_LIST 80      /* 40 messages are displayed */
#define MAX_TRACEBACK_INDENT 20    /* 10 messages are indented */

/******************************************************************************/
/* Constants used for setting trace                                           */
/******************************************************************************/

const stringchar_t TRACE_ALL           = 'A';
const stringchar_t TRACE_COMMANDS      = 'C';
const stringchar_t TRACE_LABELS        = 'L';
const stringchar_t TRACE_NORMAL        = 'N';
const stringchar_t TRACE_FAILURES      = 'F';
const stringchar_t TRACE_ERRORS        = 'E';
const stringchar_t TRACE_RESULTS       = 'R';
const stringchar_t TRACE_INTERMEDIATES = 'I';
const stringchar_t TRACE_OFF           = 'O';
const stringchar_t TRACE_IGNORE        = '0';

/******************************************************************************/
/* Constants used for setting trace interactive debug                         */
/******************************************************************************/
const int DEBUG_IGNORE      =  0x00;
const int DEBUG_ON          =  0x01;
const int DEBUG_OFF         =  0x02;
const int DEBUG_TOGGLE      =  0x04;

/******************************************************************************/
/* Random number generation constants                                         */
/******************************************************************************/

const int32_t RANDOM_FACTOR = 1664525L;   /* random multiplication factor      */
                                       /* randomize a seed number           */
inline int32_t RANDOMIZE(int32_t seed) { return (seed * RANDOM_FACTOR + 1); }

/* Object Reference Assignment */
#ifndef CHECKOREFS
#define OrefSet(o,r,v) (OldSpace(o) ? RexxMemory::memoryObject.setOref((RexxObject **)&(r),(RexxObject *)v) : (RexxObject *)(r=v))
#else
#define OrefSet(o,r,v) RexxMemory::memoryObject.checkSetOref((RexxObject *)o, (RexxObject **)&(r), (RexxObject *)v, __FILE__, __LINE__)
#endif


/******************************************************************************/
/* Define the name of the image file that is saved and restored.              */
/******************************************************************************/

#define BASEIMAGE   "rexx.img"

/******************************************************************************/
/* Define the name of the program called to load create the saved             */
/* image file.                                                                */
/******************************************************************************/

#define CORECLASSES "CoreClasses.orx"


typedef struct locationinfo {          /* token/clause location information */
  stringsize_t line;                   /* file line location                */
  stringsize_t offset;                 /* token location within the line    */
  stringsize_t endline;                /* ending line location              */
  stringsize_t endoffset;              /* ending offset location (+1)       */
} LocationInfo;

class RexxActivation;
class RexxExpressionStack;
                                       /* used ofor special constructor   */
typedef enum {RESTOREIMAGE, MOBILEUNFLATTEN, METHODUNFLATTEN} RESTORETYPE;

/******************************************************************************/
/* Global Objects - General                                                   */
/******************************************************************************/
// Class object instances
#define TheActivityManager (RexxActivityManager::singleInstance)
#define TheArrayClass     (RexxArray::classInstance)
#define TheClassClass     (RexxClass::classInstance)
#define TheDirectoryClass (RexxDirectory::classInstance)
#define TheIntegerClass   (RexxIntegerClass::classInstance)
#define TheListClass      (RexxList::classInstance)
#define TheMessageClass   (RexxMessage::classInstance)
#define TheMethodClass    (RexxMethod::classInstance)
#define ThePackageManager (PackageManager::singleInstance)
#define TheNumberStringClass (RexxNumberString::classInstance)
#define TheObjectClass    (RexxObject::classInstance)
#define TheQueueClass     (RexxQueue::classInstance)
#define TheStemClass      (RexxStem::classInstance)
#define TheStringClass    (RexxString::classInstance)
#define TheMutableBufferClass (RexxMutableBuffer::classInstance)
#define TheSupplierClass  (RexxSupplier::classInstance)
#define TheTableClass     (RexxTable::classInstance)
#define TheRelationClass  (RexxRelation::classInstance)
#define ThePointerClass   (RexxPointer::classInstance)

// globally defined objects
#define TheNilObject      (RexxObject::nilObject)
#define TheNullPointer    (RexxPointer::nullPointer)
#define TheNullArray      (RexxArray::nullArray)
#define TheFalseObject    (RexxIntegerClass::falseObject)
#define TheTrueObject     (RexxIntegerClass::trueObject)
#define IntegerZero       (RexxIntegerClass::zero)
#define IntegerOne        (RexxIntegerClass::one)
#define IntegerTwo        (RexxIntegerClass::two)
#define IntegerThree      (RexxIntegerClass::three)
#define IntegerFour       (RexxIntegerClass::four)
#define IntegerFive       (RexxIntegerClass::five)
#define IntegerSix        (RexxIntegerClass::six)
#define IntegerSeven      (RexxIntegerClass::seven)
#define IntegerEight      (RexxIntegerClass::eight)
#define IntegerNine       (RexxIntegerClass::nine)

#define TheEnvironment    (RexxInterpreter::theEnvironment)
#define TheKernel         (RexxInterpreter::theKernel)

/******************************************************************************/
/* Primitive Object Types                                                     */
/******************************************************************************/

#include "ClassTypeCodes.h"

/******************************************************************************/
/* Shortcut names to the behaviours                                           */
/******************************************************************************/

#include "PrimitiveBehaviourNames.h"

/******************************************************************************/
/* Utility Macros                                                             */
/******************************************************************************/

#define RXROUNDUP(n,to)  ((((n)+(to-1))/(to))*to)
#define rounddown(n,to)  (((n)/(to))*to)

                                       /* verify argument presence          */
void missingArgument(size_t position);
#define requiredArg(arg, position) { if (arg == OREF_NULL) missingArgument(ARG_##position); }

#include "GlobalNames.hpp"
#include "ObjectClass.hpp"               /* get real definition of Object     */
#include "TableClass.hpp"
#include "StackClass.hpp"
#include "RexxMemory.hpp"                /* memory next, to get OrefSet       */
#include "RexxBehaviour.hpp"             /* now behaviours and                */
#include "ClassClass.hpp"                /* classes, which everything needs   */
#include "RexxEnvelope.hpp"              /* envelope is needed for flattens   */
#include "RexxActivity.hpp"              /* activity is needed for errors     */
#include "NumberStringClass.hpp"         /* added to make 'number_digits()'   */

/******************************************************************************/
/* Utility Functions                                                          */
/******************************************************************************/

                                       /* do a case insensitive compare     */
int  caselessCompare(const char *, const char *, size_t);
char *mempbrk(const char *, const char *, size_t);     /* search for characters             */



/******************************************************************************/
/* Return codes                                                               */
/******************************************************************************/

const int RC_OK         = 0;
const int RC_LOGIC_ERROR  = 2;

const int POSITIVE    = 1;             /* integer must be positive          */
const int NONNEGATIVE = 2;             /* integer must be non-negative      */
const int WHOLE       = 3;             /* integer must be whole             */


/* The next macro is specifically for REQUESTing a STRING, since there are    */
/* four primitive classes that are equivalents for strings.  It will trap on  */
/* OREF_NULL. */
inline RexxString *REQUEST_STRING(RexxObject *object)
{
  return isString(object) ? (RexxString *)object : (object)->requestString();
}

/* The next routine is specifically for REQUESTing a STRING needed as a method*/
/* argument.  This raises an error if the object cannot be converted to a     */
/* string value.                                                              */
inline RexxString * REQUIRED_STRING(RexxObject *object, size_t position)
{
  if (object == OREF_NULL)             /* missing argument?                 */
    missingArgument(position);         /* raise an error                    */
                                       /* force to a string value           */
  return object->requiredString(position);
}

/* The next routine is specifically for REQUESTing an ARRAY needed as a method*/
/* argument.  This raises an error if the object cannot be converted to a     */
/* single dimensional array item                                              */
inline RexxArray * REQUIRED_ARRAY(RexxObject *object, size_t position)
{
  if (object == OREF_NULL)             /* missing argument?                 */
  {
      missingArgument(position);         /* raise an error                    */
  }
                                       /* force to array form               */
  RexxArray *array = object->requestArray();
                                       /* not an array?                     */
  if (array == TheNilObject || array->getDimension() != 1)
  {
                                       /* raise an error                    */
      reportException(Error_Execution_noarray, object);
  }
  return array;
}

#endif
