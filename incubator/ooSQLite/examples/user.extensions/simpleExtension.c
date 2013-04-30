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
 * simpleExtension.c
 *
 * Example of how to write a loadable extension file for SQLite.  This file is
 * built as a shared library, which can be loaded through the loadExtension()
 * method of the ooSQLiteConnection class.
 *
 * Note these points:
 *
 *   Include sqlite3ext.h instead of sqlite3.h.
 *
 *   Use the SQLITE_EXTENSION_INIT1 and SQLITE_EXTENSION_INIT2
 *
 *   Name the entry function sqlite3_extension_init to have it invoked
 *   automatically.  Otherwise, you need to specify the entry point name in the
 *   loadExtension() call.
 */


/* Normally use sqlite3ext.h, the ooSQLite source uses sqlite3ext.hpp */
#include <sqlite3ext.hpp>

SQLITE_EXTENSION_INIT1

/**
 * The half() SQL function returns half of its input value.  This is an example
 * from the SQLite website.
 */
static void halfFunc(sqlite3_context *context, int argc, sqlite3_value **argv)
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
static int reverseCollation(void* data, int len1, const void* str1, int len2, const void* str2)
{
    return - sqlite3_strnicmp((char *)str1, (char *)str2, len1 < len2 ? len1 : len2);
}

/**
 * SQLite invokes this routine once when it loads the extension.  Create new
 * functions, collating sequences, and virtual table modules here.  This is
 * usually the only exported symbol in the shared library.
 *
 * Here we create 1 function and 1 collation.
 *
 * @param db
 * @param pzErrMsg
 * @param pApi
 *
 * @return int
 */
int sqlite3_extension_init(sqlite3 *db, char **pzErrMsg, const sqlite3_api_routines *pApi)
{
  SQLITE_EXTENSION_INIT2(pApi)

  sqlite3_create_function(db, "half", 1, SQLITE_ANY, 0, halfFunc, 0, 0);
  sqlite3_create_collation_v2(db, "reverseSort", SQLITE_UTF8, NULL, reverseCollation, NULL);

  return 0;
}
