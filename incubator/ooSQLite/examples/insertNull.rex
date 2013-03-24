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
 *  insertNull.rex
 *
 * This example shows how to insert records into a table where the value for a
 * column in the table can be / is NULL.
 *
 * The approach taken here is to use a parameterized SQL statement.  There is
 * also the insertNullTwo.rex program that shows a second approach.  Both
 * approaches are useful for preventing SQL Injection flaws in an application.
 *
 */

  -- Open the database.  We don't check for error here, an error is unlikely.
  db = .ooSQLiteConnection~new('nullTest.rdbx')

  -- Drop the table we are going to use so that this example can be run multiple
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

  -- Now create our table:
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

  -- Get the values for our data.  Here the values come from our program here,
  -- so the source should be trustworthy.  But, it the values instead came from
  -- user entered data, you would definitely need to protect from SQL injection:
  values = getRecordValues()

  -- Create a parameterized SQL statement
  sql = "INSERT INTO test_table (fName, lName, phone) VALUES(?1, ?2, ?3);"

  stmt = .ooSQLiteStmt~new(db, sql)
  if stmt~initCode \== db~OK then do
    say 'Error 5:' stmt~lastErrCode stmt~lastErrMsg
    db~close
    return 99
  end

  -- Now insert how ever many rows we have. Count is used here to generate the
  -- Error message if we have an error so that we have a clue as to where the
  -- error occurred:
  count = 5
  do r over values
    count += 1

    do i = 1 to r~items
      if r[i] == .nil then ret = stmt~bindNull(i)
      else ret = stmt~bindText(i, r[i])
    end

    ret = stmt~step
    if ret \== stmt~DONE then do
      say 'Error' count':' db~errCode db~errMsg
      stmt~finalize
      db~close
      return 99
    end

    stmt~reset
  end
  stmt~finalize

  -- Print out the data in the table. printTableWithNulls() is a .rex program
  -- in this directory.  Here we are calling it as a function.
  ret = 'printTableWithNulls'(db, "test_table")

  db~close
  return 0

::requires 'ooSQLite.cls'

/* Just generate some random data for the test_table table. */
::routine getRecordValues

  vals = .array~new

  vals~append(.array~of('Sam',     .nil,        '234-5566'))
  vals~append(.array~of(.nil,      'Franks',    .nil))
  vals~append(.array~of('Howard',  'Johnson',   '674-2920'))
  vals~append(.array~of('Larry',   .nil,        .nil))
  vals~append(.array~of('Susan',   'McColl',    '556-1297'))
  vals~append(.array~of(.nil,      .nil,        '858-444-1388'))
  vals~append(.array~of('Mary',    'Hisante',   .nil))
  vals~append(.array~of(.nil,      'Edwards',   '234-5566'))
  vals~append(.array~of('Alice',   'Tolkas',    '443-564-9904'))
  vals~append(.array~of('June',    'Cleaver',   '944-7623'))
  vals~append(.array~of('Ward',    'Cleaver',   '944-7623'))
  vals~append(.array~of('Tobey',   .nil,        .nil))

return vals
