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
 *  loadLibrary.rex
 *
 *  Demonstrates how to load and use an ooSQLite library file.
 *
 *  The loadLibrary() method loads a shared library containing, presumably, user
 *  defined SQLite collations, functions, or modules.
 *
 */

  os = getOSName()

  if os == 'WINDOWS' then do
    libFile = 'user.extensions\examplelibrary.dll'
  end
  else do
    libFile = 'user.extensions/libexamplelibrary.so'
  end

  -- Set the result set format to an array of arrays:
  .ooSQLite~recordFormat = .ooSQLite~OO_ARRAY_OF_ARRAYS

	dbName = 'ooFoods.rdbx'
  dbConn = .ooSQLiteConnection~new(dbName, .ooSQLite~OPEN_READWRITE)

  success = .ooSQLExtensions~loadLibrary(libFile, .array~of('ebcdic', 'reverse', 'halfFunc'))
  if \ success then do
    say 'Failed to load library'
    say '  Error code:   ' .ooSQLExtensions~lastErrCode
    say '  Error message:' .ooSQLExtensions~lastErrMsg

    return .ooSQLExtensions~lastErrCode
  end

  lib = .ooSQLExtensions~getLibrary('examplelibrary')
  if lib == .nil then do
    say 'Failed to retrieve library from extensions manager'
    say '  Error code:   ' .ooSQLExtensions~lastErrCode
    say '  Error message:' .ooSQLExtensions~lastErrMsg

    return .ooSQLExtensions~lastErrCode
  end

  hReverse = lib~getHandle('reverse')
  hEbcdic = lib~getHandle('ebcdic')

  ret = dbConn~createCollation('REVERSE', hReverse)
  ret = dbConn~createCollation('EBCDIC', hEbcdic)

  sql = "SELECT * FROM foods where name like 'J%' ORDER BY name COLLATE REVERSE;"
  resultSet = dbConn~exec(sql, .true)

  say 'SQL:             ' sql
  say 'Result Set:      ' resultSet
  say 'Result Set Class:' resultSet~class
  say
  say 'Hit enter to continue'
  pull
  z = printResultSet(resultSet)

  sql = "SELECT * FROM foods where name like 'J%' ORDER BY name COLLATE EBCDIC;"
  resultSet = dbConn~exec(sql, .true)

  say 'SQL:             ' sql
  say 'Result Set:      ' resultSet
  say 'Result Set Class:' resultSet~class
  say
  say 'Hit enter to continue'
  pull
  z = printResultSet(resultSet)

  sql = "SELECT * FROM foods ORDER BY name COLLATE REVERSE;"
  resultSet = dbConn~exec(sql, .true)

  say 'SQL:             ' sql
  say 'Result Set:      ' resultSet
  say 'Result Set Class:' resultSet~class
  say
  say 'Hit enter to continue'
  pull
  z = printResultSet(resultSet)

  -- Register a user defined function.  SQLite allows the creation of user
  -- defined SQL functions and aggregates.  They are both created / registered
  -- the same way.  There are 3 callbacks involve here: the function callback,
  -- the step callback, and the final callback.
  --
  -- To register a function you must provide a valid function handle.  To
  -- register an aggregate you must provide valid step and final handles.  In
  -- the createFunction() method you can provide a zero pointer for the unneeded
  -- handles.  The second argument in createFunction() is required.  The
  -- ooSQLite nees this argument to determine the proper action.  So, to
  -- register an aggregate you must use a zero pointer for the function callback
  -- handle.  To register a function, you can either omit the step and final
  -- callback handles, or provide zero pointers.


  hHalf = lib~getHandle('halfFunc')
  if hHalf == .nil then do
    say 'Failed to get halfFunc function handle for the library "exampleLibrary"'
    say '  Error code:   ' lib~lastErrCode
    say '  Error message:' lib~lastErrMsg

    return .ooSQLExtensions~lastErrCode
  end

  -- Create the function. We need the SQL name for the function, the handle to
  -- the function, and the number of arguments to the function

  -- We show 2 ways to create the function here.  One using the 4th and 5th
  -- arguments, the other way omits the argumens.
  --
  -- Note that when you register a function or collation with the same name as
  -- an existing collation or function, the second simply replaces the first.
  -- So we can register the half function here twice and have the same effect.
  -- Normally it would not make sense to do this in a regular program.  This is
  -- just to illustrate how to use the createFunction
  dbConn~createFunction('half', hHalf, 1, .ooSQLite~zeroPointer, .ooSQLite~zeroPointer)

  -- and again, omit arg 4 and 5 has the same effect as using zero pointers.
  dbConn~createFunction('half', hHalf, 1)

  sql = "SELECT half(11);"
  resultSet = dbConn~exec(sql, .true)

  say 'SQL:             ' sql
  say 'Result Set:      ' resultSet
  say 'Result Set Class:' resultSet~class
  say
  say 'Hit enter to continue'
  pull
  z = printResultSet(resultSet)

  -- Register a user defined aggregate.  For an aggregate we need the step and
  -- the final function handles.  And we must provide a zero handle for the
  -- function handle.
  hStrAggFunc = .ooSQLite~zeroPointer
  hStrAggStep = lib~getHandle('strAggStep')
  if hStrAggStep == .nil then do
    say 'Failed to get strAppStep function handle for the library "exampleLibrary"'
    say '  Error code:   ' lib~lastErrCode
    say '  Error message:' lib~lastErrMsg

    return .ooSQLExtensions~lastErrCode
  end

  hStrAggFinal = lib~getHandle('strAggFinalize')
  if hStrAggStep == .nil then do
    say 'Failed to get strAggFinalize function handle for the library "exampleLibrary"'
    say '  Error code:   ' lib~lastErrCode
    say '  Error message:' lib~lastErrMsg

    return .ooSQLExtensions~lastErrCode
  end

  -- Create the aggregate. We need the SQL name for the aggregate, the handle to
  -- the function, which must be a zero pointer, the number of arguments to the
  -- aggregate, and the step and final handles.
  dbConn~createFunction('strAggregate', hStrAggFunc, 1, hStrAggStep, hStrAggFinal)

  sql = "SELECT season, strAggregate(name) from episodes group by season;"
  resultSet = dbConn~exec(sql, .true)

  say 'SQL:             ' sql
  say 'Result Set:      ' resultSet
  say 'Result Set Class:' resultSet~class
  say
  say 'Hit enter to continue'
  pull
  z = printStrAgg(resultSet)

  ret = dbConn~close

  return ret

::requires 'ooSQLite.cls'

::routine printResultSet
  use arg rs

  colCount = rs[1]~items
  rowCount = rs~items

  line = ''
  headers = rs[1]
  do j = 1 to colCount
    line ||= headers[j]~left(25)
  end

  say line
  say '='~copies(80)

  do i = 2 to rowCount
    line = ''
    record = rs[i]
    do j = 1 to colCount
      line ||= record[j]~left(25)
    end

    say line
  end
  say

  return 0

-- strAggregate specific utility routine used to print the strAggregate result
-- set.
::routine printStrAgg
  use arg rs

  colCount = 2
  rowCount = rs~items

  headers = rs[1]
  line = headers[1]~left(9) || headers[2]

  say line
  say '='~copies(80)

  do i = 2 to rowCount
    record = rs[i]
    if record[1] == .nil then line = 'NULL'~left(9) || record[2]
    else line = record[1]~left(9) || record[2]

    if line~length > 80 then do
      say line~left(80)

      line = ' '~copies(9) || line~substr(81)
      do while line~length > 80
        say line~left(80)
        line = ' '~copies(9) || line~substr(81)
      end
    end
    else do
      say line
    end
    say
  end
  say

  return 0


::routine getOSName

  parse upper source os .
  if os~abbrev("WIN") then os = "WINDOWS"
  return os


