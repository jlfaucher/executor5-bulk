#!/usr/bin/rexx
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2012-2013 Rexx Language Association. All rights reserved.    */
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
 *  pragmaSpecialTest.rex
 *
 *  One of the non-standard SQL features of SQLitet is the PRAGMA statement.
 *
 *  "The pragma command is specific to SQLite and is very unlikely to be
 *   compatible with any other SQL database engine."
 *
 *  PRAGMA statements are executed like any other SQL.  Some pragmas return
 *  tables, some do not.  Unknown pragmas are ignored by SQLite.  Some pragmas
 *  set values, some return a single value.
 *
 *  Because of these differences, ooSQLite wraps the execution of a pragma in
 *  the pragma() method. You would not have to use the pragma() method, you
 *  could instantiate an ooSQLiteStmt and execute the SQL like any other SQL.
 *
 *  This test issues pragmas that return a list of values.
 */

  dbFile = 'ooFoods.rdbx'

  db = .ooSQLiteConnection~new(dbFile, .ooSQLite~OPEN_READWRITE, .ooSQLite~OO_ARRAY_OF_ARRAYS)
  if db~initCode <> 0 then do
    say 'ooSQLiteConnection initialization err:' db~initCode
    say '  Error code:' db~lastErrCode '('db~lastErrMsg')'
    if db~initCode == 14 then do
      say '  Database file name:' db~fileName '(Is this the correct database?)'
    end
    db~close
    return 99
  end

  ret = db~pragma('compile_options')
  z = printResult('compile_options', ret)

  ret = db~pragma('collation_list')
  z = printResult('collation_list', ret)

  ret = db~pragma('database_list')
  z = printResult('database_list', ret)

  -- There are no foreign keys in ooFoods.rdbx
  ret = db~pragma('foreign_key_list', 'foods')
  z = printResult('foreign_key_list table: foods', ret)

  -- We need an index for index_info and index_list, but ooFoods.rdbx does not
  -- have any indexes.  Unlike foreign key above, we create a temp index and
  -- then delete it when we are done with it.

  sql = 'CREATE INDEX temp_idx ON foods (name, type_id)'
  stmt = .ooSQLiteStmt~new(db, sql)
  ret = stmt~step
  stmt~finalize

  ret = db~pragma('index_info', 'temp_idx')
  z = printResult('index_info index: temp_idx', ret)

  ret = db~pragma('index_list', 'foods')
  z = printResult('index_list table: foods', ret)

  sql = 'DROP INDEX temp_idx'
  stmt = .ooSQLiteStmt~new(db, sql)
  ret = stmt~step
  stmt~finalize

  ret = db~pragma('table_info', 'foods')
  z = printResult('table_info table: foods', ret)


  db~close

::requires 'ooSQLite.cls'

::routine printResult
  use arg n, ret

  say 'Pragma:' n
  if ret~items == 0 then do
    say 'Result: No data'
    say; say; say
    return 0
  end

  say 'Result:'

  -- We do some special formatting of the table output below because we know
  -- what is needed to display all the data for some of the pragmas.

  colCount = ret[1]~items
  rowCount = ret~items

  line = ''
  record = ret[1]

  if n == 'database_list' then do
    line = record[1]~left(7) record[2]~left(7) record[3]
  end
  else do j = 1 to colCount
    line ||= record[j]~left(14)
  end

  say line
  say '='~copies(80)

  do i = 2 to rowCount
    line = ''
    record = ret[i]

    width = 14
    if n == 'compile_options' then width = 65

    if n == 'database_list' then do
      line = record[1]~left(7) record[2]~left(7) record[3]
    end
    else do j = 1 to colCount
      col = record[j]
      if col == .nil then col = 'NULL'
      line ||= col~left(width)
    end

    say line
  end
  say; say; say

  return 0
