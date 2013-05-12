/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2013-2013 Rexx Language Association. All rights reserved.    */
/*                                                                            */
/* This program and the accompanying materials are made available under       */
/* the terms of the Common Public License v1.0 which accompanies this         */
/* distribution. A copy is also available at the following address:           */
/* http://www.oorexx.org/license.html                                         */
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

/**
 * exampleLibrary.cpp
 *
 * Example of how to write a shared library file for ooSQLite that contains user
 * define collations, functions, or modules.
 *
 * This file does not contain any user defined modules, but if you know how to
 * write a module, you could use this example as a template to include it.
 *
 * The shared library can be loaded through the loadLibrary() class method of
 * the ooSQLExtension class.  You then use the ooSQLLibrary class to gain access
 * to the function pointers in the library.
 *
 * Note these points:
 *
 *
 *
 *
 *
 *
 */

// You must include this header if you use this example as a template for your
// own library.  Otherwise you could deconstruct the macros and build the
// library any way you choose.
#include "oosqlPackage.hpp"

#ifdef _WIN32
    #include <windows.h>
#endif

#include <stdio.h> // printf() if needed

/**
 * ASCII to EBCDIC tranlation table.  Not sure what version of EBCDIC, values
 * taken directly from an ooRexx contributor.
 */
static unsigned char asciiToEbcdic[ 256 ] =
{
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
    0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
    0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
    0x40, 0x5A, 0x7F, 0x7B, 0x5B, 0x6C, 0x50, 0x7D, 0x4D,
    0x5D, 0x5C, 0x4E, 0x6B, 0x60, 0x4B, 0x61,
    0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8,
    0xF9, 0x7A, 0x5E, 0x4C, 0x7E, 0x6E, 0x6F,
    0x7C, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8,
    0xC9, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6,
    0xD7, 0xD8, 0xD9, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7,
    0xE8, 0xE9, 0xAD, 0xE0, 0xBD, 0x5F, 0x6D,
    0x79, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88,
    0x89, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96,
    0x97, 0x98, 0x99, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7,
    0xA8, 0xA9, 0xC0, 0x4F, 0xD0, 0xA1, 0x4B,
    0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B,
    0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B,
    0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B,
    0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B,
    0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B,
    0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B,
    0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B,
    0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B,
    0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B,
    0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B,
    0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B,
    0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B,
    0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B,
    0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B,
    0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B,
    0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B, 0x4B
};

/**
 * EBCDIC to ASCII translation table.  This is an exact inverse of the above.
 * The table is not used here in this code.  It is present simply to be
 * complete, the 2 tables could be used in some other application that translate
 * from and to ASCII.
 */
static unsigned char ebcdicToAscii[256] =
{
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
    0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
    0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
    0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
    0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
    0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
    0x2E, 0x2E, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38,
    0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x2E, 0x3F,
    0x20, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E,
    0x2E, 0x2E, 0x2E, 0x3C, 0x28, 0x2B, 0x7C,
    0x26, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E,
    0x2E, 0x21, 0x24, 0x2A, 0x29, 0x3B, 0x5E,
    0x2D, 0x2F, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E,
    0x2E, 0x7C, 0x2C, 0x25, 0x5F, 0x3E, 0x3F,
    0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E,
    0x60, 0x3A, 0x23, 0x40, 0x27, 0x3D, 0x22,
    0x2E, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68,
    0x69, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E,
    0x2E, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70, 0x71,
    0x72, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E,
    0x2E, 0x7E, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79,
    0x7A, 0x2E, 0x2E, 0x2E, 0x5B, 0x2E, 0x2E,
    0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E,
    0x2E, 0x2E, 0x2E, 0x2E, 0x5D, 0x2E, 0x2E,
    0x7B, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
    0x49, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E,
    0x7D, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50, 0x51,
    0x52, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E,
    0x5C, 0x2E, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59,
    0x5A, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E,
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38,
    0x39, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E, 0x2E
};


/**
 * The half() SQL function returns half of its input value.  This is an example
 * from the SQLite website.
 */
void halfFunc(sqlite3_context *context, int argc, sqlite3_value **argv)
{
  sqlite3_result_double(context, 0.5*sqlite3_value_double(argv[0]));
}

/**
 * An example collation, essentially a reverse sort.
 *
 * @param data
 * @param len1
 * @param str1
 * @param len2
 * @param str2
 *
 * @return int
 */
int reverse(void* data, int len1, const void* str1, int len2, const void* str2)
{
    return - sqlite3_strnicmp((char *)str1, (char *)str2, len1 < len2 ? len1 : len2);
}

/**
 * An example collation, it allows ascii data to be ordered as if it were
 * ebcdic.
 *
 * @param data
 * @param len1
 * @param str1
 * @param len2
 * @param str2
 *
 * @return int
 */
int ebcdic(void* data, int len1, const void* str1, int len2, const void* str2)
{
    register unsigned char *a = (unsigned char *)str1;
    register unsigned char *b = (unsigned char *)str2;

    int min = len1 < len2 ? len1 : len2;
    int i;

    if ( min == 0 )
    {
        // I don't think this is possible, but just to make sure
        return len1 - len2;
    }
    for ( i = 0; i < min, asciiToEbcdic[*a] == asciiToEbcdic[*b]; i++ )
    {
        a++;
        b++;
    }

    return (i == min) ? len1 - len2 : asciiToEbcdic[*a] - asciiToEbcdic[*b];
}


/* struct for use in the strAggregate aggregate function below. */
typedef struct _StrAggCntx
{
    size_t   count;
    char    *result;
} StrAggCntx;
typedef StrAggCntx *pStrAggCntx;

/**
 * An example of a user defined aggregate.  This example is a simple rewrite of
 * an example in "Definitive Guide to SQLite", Second Edition, by Grant Allen
 * and Mike Owens, published by Apress.
 *
 * The aggregate expects 1 text argument and concatenates each string to produce
 * its result.
 *
 * An example usage:
 *
 * SELECT season, strAggregate(name) from episodes group by season;
 *
 * This will concatenate the name of each episode into a single string listing
 * each episode name, grouped by season.
 *
 * Each aggregate can, and probably needs to, allocate its individual block to
 * keep track of what it is aggragating.  This then is specific to the unique
 * instance of the aggregate.  Allocationg the block is done through
 * sqlite3_aggregate_context().
 *
 * At the first invocation, this SQLite function will allocated a zero filled
 * block of the size requested.  On each subsequent invocation, this function
 * simple returns the already allocated block.
 *
 * After that, is is simply a matter of concatenating the new string to the
 * accumulated result string.
 *
 * @param sqlCntx
 * @param countArgs
 * @param values
 */
void strAggStep(sqlite3_context* sqlCntx, int countArgs, sqlite3_value** values)
{
  pStrAggCntx psac = (pStrAggCntx)sqlite3_aggregate_context(sqlCntx, sizeof(StrAggCntx));
  if ( psac == NULL )
  {
      sqlite3_result_error_nomem(sqlCntx);
      return;
  }

  static const char comma[] = ", ";

  const  char *text = (const char *)sqlite3_value_text(values[0]);
  size_t len       = strlen(text);

  if ( psac->result == NULL )
  {
    psac->result = (char *)sqlite3_malloc((int)len + 1);
    if ( psac->result == NULL )
    {
        sqlite3_result_error_nomem(sqlCntx);
        return;
    }

    memcpy(psac->result, text, len + 1);
    psac->count = len;
  }
  else
  {
    const size_t commaLen = strlen(comma);
    psac->result = (char *)sqlite3_realloc(psac->result, (int)(psac->count + len + commaLen + 1));
    memcpy(psac->result +  psac->count, comma, commaLen);
    psac->count += commaLen;
    memcpy(psac->result + psac->count, text, len + 1);
    psac->count += len;
  }
}

/**
 * Implementation of the xFinal function for the strAggregate user define
 * aggregate.
 *
 * Here we simple set the result of the aggregate to the text value in our
 * accumulator.
 *
 * @param sqlCntx
 */
void strAggFinalize(sqlite3_context* sqlCntx)
{
    pStrAggCntx psac = (pStrAggCntx) sqlite3_aggregate_context(sqlCntx, sizeof(StrAggCntx));
    if ( psac == NULL || psac->result == NULL )
    {
        sqlite3_result_error_nomem(sqlCntx);
        return;
    }

    sqlite3_result_text(sqlCntx, psac->result, (int)psac->count, sqlite3_free);
}




// This macro expands to a function that gives access to the SQLite APIs in this
// library.  Without it, your functions would not be able to access any SQLite
// APIs.
//
// On Windows, you *must* declare an exportable function: ooSQLiteSetApi.
// Personally, the author thinks the easist way to do this is through a .def
// file as done in this example.  There are other ways to do it in Windows and
// you are free to choose any method you prefer.
OOSQLITE_SET_API();
