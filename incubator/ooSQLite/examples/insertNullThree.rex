#!/usr/bin/rexx
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2012-2012 Rexx Language Association. All rights reserved.    */
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
 *  insertNullThree.rex
 *
 * This example shows another alternative method to insert records into a table
 * where the value for a column in the table can be / is NULL.
 *
 * This method uses the .ooSQLite~enquote method to single quote the value,
 * escape any single quotes within the value, and use NULL, no single quotes
 * for an values that are empty or .nil
 *
 */

  -- Open the database.  We don't check for error here, an error is unlikely.
  db = .ooSQLiteConnection~new('nullTest.rdbx')

  -- Drop the table we are going to use so we can run this example multiple
  -- times without error.
  sql = "DROP TABLE IF EXISTS main.test_table"

  stmt = .ooSQLiteStmt~new(db, sql)
  if stmt~initCode \== db~OK then do
    say 'Error 1:' stmt~lastErrCode stmt~lastErrMsg
    db~close
    return 99
  end

  ret = stmt~step
  stmt~finalize
  if ret \== stmt~DONE then do
    say 'Error 2:' db~errCode db~errMsg
    db~close
    return 99
  end

  -- Now create out table:
  sql = "CREATE TABLE test_table ("               || .endOfLine || -
        "  id INTEGER PRIMARY KEY,"               || .endOfLine || -
        "  fName TEXT,"                           || .endOfLine || -
        "  lName TEXT,"                           || .endOfLine || -
        "  phone TEXT,"                           || .endOfLine || -
        "  type TEXT NOT NULL DEFAULT 'Home'"     || .endOfLine || -
        ");"

  stmt = .ooSQLiteStmt~new(db, sql)
  if stmt~initCode \== db~OK then do
    say 'Error 3:' stmt~lastErrCode stmt~lastErrMsg
    db~close
    return 99
  end

  ret = stmt~step
  stmt~finalize
  if ret \== stmt~DONE then do
    say 'Error 4:' db~errCode db~errMsg
    db~close
    return 99
  end

  -- Get the values of our data:
  values = getRecordValues()

  -- Now insert how ever many rows we have:
  count = 4
  do r over values
    count += 1

    -- Each r is an arry of 3 elements. Any missing indexes, or .nil objects at
    -- any index will be treated as SQL NULL.  All other items quoted with sigle
    -- quotes, plus any embedded single quotes are escaped. Commas are added
    -- between each individual value, so the returned string is correct to
    -- insert into VALUES()

    valStr = .ooSQLite~enquote(r)
    sql = "INSERT INTO test_table (fName, lName, phone) VALUES("valStr");"

    stmt = .ooSQLiteStmt~new(db, sql)
    if stmt~initCode \== db~OK then do
      say 'Error' count':' stmt~lastErrCode stmt~lastErrMsg
      db~close
      return 99
    end

    ret = stmt~step
    if ret \== stmt~DONE then do
      say 'Error' count':' db~errCode db~errMsg
      stmt~finalize
      db~close
      return 99
    end

    stmt~finalize
  end

  -- Print out the data in the table. printTableWithNulls() is a .rex program
  -- in this directory.  Here we are calling it as a function.
  ret = 'printTableWithNulls'(db, "test_table")

  db~close
  return 0

::requires 'ooSQLite.cls'


/* Returns the correct SQL INSERT string to insert the row represented by the array "v" */
::routine getSQL
  use arg v  -- An array of values

  sql = "INSERT INTO test_table (fName, lName, phone) VALUES("escape(v[1])", "escape(v[2])", "escape(v[3])");"

return sql


-- Just generate some random data for the test_table table.  Either missing
-- indexes in the array, or the .nil object will be treated as SQL NULLs
::routine getRecordValues

  vals = .array~new

  vals~append(.array~of('Sam',     .nil,        '234-5566'))
  vals~append(.array~of(.nil,      'Franks',    .nil))
  vals~append(.array~of('Howard',  'Johnson',   '674-2920'))
  vals~append(.array~of('Larry',   .nil,        .nil))
  vals~append(.array~of('Susan',   'McColl',    '556-1297'))
  vals~append(.array~of(.nil,      ,            '858-444-1388'))
  vals~append(.array~of('Mary',    'Hisante',   .nil))
  vals~append(.array~of(.nil,      "Edward's",   '234-5566'))
  vals~append(.array~of('Alice',   'Tolkas',    '443-564-9904'))
  vals~append(.array~of('June',    'Cleaver',   '944-7623'))
  vals~append(.array~of('Ward',    'Cleaver',   '944-7623'))
  vals~append(.array~of('Tobey',   ,            .nil))

return vals
