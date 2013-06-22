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
 *  builtinExtensions.rex
 *
 *  Demonstrates how to use the ooSQLite builtin extensions.
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
 *  The registerBuiltin() method of the ooSQLiteExtensions class is used to
 *  register builtin extensions with a database connection.  The method will
 *  register a single available extension, some of the available extensions, or
 *  all of the available extensions.
 *
 *  SQLite also provides a mechanism that will automatically register a builtin
 *  extension every time a database connection is opened.  The autoBuiltin()
 *  method of the ooSQLExtensions class is used to make any, or all of the
 *  available builtin extensions automatically registered when any database
 *  connection is opened.  The resetAutoBuiltin method of the ooSQLExtensions
 *  class removes all automatic extensions.
 *
 *  The list of currently available builtin extensions is:
 *
 *    ieee754
 *    nextChar
 *    regExp
 *    rot13
 *    spellFix
 *    wholeNumber
 *
 *  This example uses iee754, regExp, rot13, and wholenumber to demonstrate the
 *  methods related to using the builtin extensions.
 */

  .ooSQLite~recordFormat = .ooSQLite~OO_ARRAY_OF_ARRAYS
	dbName                 = 'ooFoods.rdbx'

  -- Set all the available builtin extensions to automatically be registered
  -- with any database connection when it is opened:
  ret = .ooSQLExtensions~autoBuiltin
  if ret <> .ooSQLExtensions~ok then do
    say 'Error registering builtin extensions:' .ooSQLExtensions~lastErrMsg
    return .ooSQLExtensions~lastErrCode
  end

  -- Open a database connection and execute some SQL that uses four of the
  -- available extensions
  dbConn = .ooSQLiteConnection~new(dbName, .ooSQLite~OPEN_READWRITE)

  say "No executed SQL should have an empty result set:"
  say '------------------------------------------------'
  say
  ret = execShortList(dbConn)
  say

  ret = dbConn~close

  -- This will reset the automatic extension list back to none.
  .ooSQLExtensions~resetAutoBuiltin

  dbConn2 = .ooSQLiteConnection~new(dbName, .ooSQLite~OPEN_READWRITE)

  say "All executed SQL should have an empty result set:"
  say '-------------------------------------------------'
  say
  ret = execShortList(dbConn2)

  ret = dbConn2~close

  -- This sets only the ieee754, nextChar, and rot13 extensions to be
  -- automatically registered.  Our execShortList() routine does not use
  -- nextChar
  ret = .ooSQLExtensions~autoBuiltin(.array~of(ieee754, nextChar, rot13))
  if ret <> .ooSQLExtensions~ok then do
    say 'Error registering builtin extensions:' .ooSQLExtensions~lastErrMsg
    return .ooSQLExtensions~lastErrCode
  end

  say "Only SQL using ieee754 and rot13 should produce a retul;t set:"
  say '--------------------------------------------------------------'
  say
  dbConn3 = .ooSQLiteConnection~new(dbName, .ooSQLite~OPEN_READWRITE)
  ret = execShortList(dbConn3)

  ret = dbConn3~close

  -- Reset the automatically registered extensions list
  .ooSQLExtensions~resetAutoBuiltin

  dbConn4 = .ooSQLiteConnection~new(dbName, .ooSQLite~OPEN_READWRITE)

  -- Register builtin extensions individually with opened database connections:
  .ooSQLExtensions~registerBuiltin(dbConn4, 'ieee754')

  say "Registering ieee754 only:"
  say '-------------------------'
  say
  sql = "SELECT ieee754(2.0);"
  resultSet = dbConn4~exec(sql, .true)

  say 'SQL:             ' sql
  say
  say 'Hit enter to continue'
  pull
  z = printResultSet(resultSet)
  say

  ret = dbConn4~close

  dbConn5 = .ooSQLiteConnection~new(dbName, .ooSQLite~OPEN_READWRITE)

  .ooSQLExtensions~registerBuiltin(dbConn5, .array~of('ieee754', 'rot13'))

  say "Registering ieee754 and rot13 only:"
  say '-----------------------------------'
  say
  sql = "SELECT ieee754(2.0);"
  resultSet = dbConn5~exec(sql, .true)

  say 'SQL:             ' sql
  say
  say 'Hit enter to continue'
  pull
  z = printResultSet(resultSet)
  say

  sql = "SELECT rot13('Hello');"
  resultSet = dbConn5~exec(sql, .true)

  say 'SQL:             ' sql
  say
  say 'Hit enter to continue'
  pull
  z = printResultSet(resultSet)
  say

  sql = "SELECT rot13('Goodbye');"
  resultSet = dbConn5~exec(sql, .true)

  say 'SQL:             ' sql
  say
  say 'Hit enter to continue'
  pull
  z = printResultSet(resultSet)
  say

  ret = dbConn5~close

  -- All the builtin extensions can be registered with a databaseconnections
  -- at one time by omitting the argument.

  dbConn6 = .ooSQLiteConnection~new(dbName, .ooSQLite~OPEN_READWRITE)

  .ooSQLExtensions~registerBuiltin(dbConn6)

  say "No executed SQL should have an empty result set:"
  say '------------------------------------------------'
  say
  ret = execShortList(dbConn6)
  say

  ret = dbConn6~close
  return ret

::requires 'ooSQLite.cls'
::requires 'utilities.frm'


-- This routine will execute some SQL using several of the avaiable builtin
-- extensions.  If any extension is not registered with the database connection
-- the SQL will produce an empty result set
::routine execShortList
  use arg dbConn

  sql = "SELECT ieee754(2.0);"
  resultSet = dbConn~exec(sql, .true)

  say 'SQL:             ' sql
  say
  say 'Hit enter to continue'
  pull
  z = printResultSet(resultSet)
  say

  sql = "SELECT ieee754(45.25);"
  resultSet = dbConn~exec(sql, .true)

  say 'SQL:             ' sql
  say
  say 'Hit enter to continue'
  pull
  z = printResultSet(resultSet)
  say

  sql = "SELECT rot13('Hello');"
  resultSet = dbConn~exec(sql, .true)

  say 'SQL:             ' sql
  say
  say 'Hit enter to continue'
  pull
  z = printResultSet(resultSet)
  say

  sql = "SELECT rot13('Goodbye');"
  resultSet = dbConn~exec(sql, .true)

  say 'SQL:             ' sql
  say
  say 'Hit enter to continue'
  pull
  z = printResultSet(resultSet)
  say

  sql = "SELECT regexp('o?', 'Goodbye');"
  resultSet = dbConn~exec(sql, .true)

  say 'SQL:             ' sql
  say
  say 'Hit enter to continue'
  pull
  z = printResultSet(resultSet)
  say

  sql = "SELECT * FROM foods where name regexp 'o{2,2}' ORDER BY name;"
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
  say
  say 'Hit enter to continue'
  pull
  z = printResultSet(resultSet)

  return 0
