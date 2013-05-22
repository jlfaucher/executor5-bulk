#!/usr/bin/rexx
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
 *  loadBuiltinExtensions.rex
 *
 *  Demonstrates how to load the builtin SQLite extensions.
 *
 *  The extensions are implemented in simpleExtension.c which is in the
 *  user.extensions subdirectory.  On Windows simpleExtension.c is compiled into
 *  a DLL, on Linux it is compiled into a .so file.
 *
 *  One of the extensions is a collation, REVERSE.  After loading the
 *  extension file, we can use the REVERSE collation here.
 *
 */

	dbName = 'ooFoods.rdbx'

  -- Set the result set format to an array of arrays:
  .ooSQLite~recordFormat = .ooSQLite~OO_ARRAY_OF_ARRAYS

  dbConn = .ooSQLiteConnection~new(dbName, .ooSQLite~OPEN_READWRITE)

  ret = dbConn~registerBuiltinExtension
  if ret <> dbConn~ok then do
    say 'Error registering builtin extensions:' dbConn~lastErrMsg
    return dbConn~lastErrCode
  end

  sql = "SELECT ieee754(2.0);"
  resultSet = dbConn~exec(sql, .true)

  z = printResultSet(resultSet)
  say

  sql = "SELECT ieee754(45.25);"
  resultSet = dbConn~exec(sql, .true)

  z = printResultSet(resultSet)
  say

  sql = "SELECT rot13('Hello');"
  resultSet = dbConn~exec(sql, .true)

  z = printResultSet(resultSet)
  say

  sql = "SELECT rot13('Goodbye');"
  resultSet = dbConn~exec(sql, .true)

  z = printResultSet(resultSet)
  say

  sql = "SELECT regexp('o?', 'Goodbye');"
  resultSet = dbConn~exec(sql, .true)

  z = printResultSet(resultSet)
  say

  sql = "SELECT * FROM foods where name regexp 'o+' ORDER BY name;"
  resultSet = dbConn~exec(sql, .true)

  say 'SQL:             ' sql
  say
  say 'Hit enter to continue'
  pull
  z = printResultSet(resultSet)

  sql = "SELECT * FROM foods where name regexp 'Th.+r' ORDER BY name;"
  resultSet = dbConn~exec(sql, .true)

  say 'SQL:             ' sql
  say
  say 'Hit enter to continue'
  pull
  z = printResultSet(resultSet)

  sql1 = "CREATE VIRTUAL TABLE nums USING wholenumber;"
  sql2 = "SELECT value FROM nums WHERE value>10 AND value< 22;"
  dbConn~exec(sql1)
  resultSet = dbConn~exec(sql2, .true)

  say 'SQL1:            ' sql1
  say 'SQL2:            ' sql2
  say 'Result Set:      ' resultSet
  say 'Result Set Class:' resultSet~class
  say
  say 'Hit enter to continue'
  pull
  z = printResultSet(resultSet)


  ret = dbConn~close
  return ret

::requires 'ooSQLite.cls'
::requires 'utilities.frm'


