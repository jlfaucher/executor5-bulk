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
 *  registerBuiltinExtensions.rex
 *
 *  Demonstrates how to register the builtin SQLite extensions.
 *
 *  The builtin extensions consist of some extensions provided by SQLite in C
 *  source code.  These extensions are compiled into the ooSQLite shared library
 *  and can be used by registering any of them with a database connection, or
 *  by making any of them an automatic extension.
 *
 *  When a builtin extension is registered with a database connection, then that
 *  extension can be used for the life of the database extension.  When the
 *  database connection is closed, the builtin extension is no longer usable.
 *
 *  The registerBuiltinExtension() method of the ooSQLiteConnection class is
 *  used to register builtin extensions with a database connection.  The method
 *  will register a single available extension, some of the available
 *  extensions, or all of the available extensions.
 *
 *  This example is similar to the builtinExtensions.rex example, except that
 *  example shows some other methods related to the builtin extensions.
 */

	dbName = 'ooFoods.rdbx'

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

  say 'SQL: ' sql
  say
  say 'Hit enter to continue'
  pull
  z = printResultSet(resultSet)

  sql = "SELECT * FROM foods where name regexp 'Th.+r' ORDER BY name;"
  resultSet = dbConn~exec(sql, .true)

  say 'SQL: ' sql
  say
  say 'Hit enter to continue'
  pull
  z = printResultSet(resultSet)

  sql1 = "CREATE VIRTUAL TABLE nums USING wholenumber;"
  sql2 = "SELECT value FROM nums WHERE value>10 AND value< 22;"
  dbConn~exec(sql1)
  resultSet = dbConn~exec(sql2, .true)

  say 'SQL1: ' sql1
  say 'SQL2: ' sql2
  say
  say 'Hit enter to continue'
  pull
  z = printResultSet(resultSet)


  ret = dbConn~close
  return ret

::requires 'ooSQLite.cls'
::requires 'utilities.frm'


