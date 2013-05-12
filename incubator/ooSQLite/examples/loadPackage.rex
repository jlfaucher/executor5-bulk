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
 *  loadPackage.rex
 *
 *  Demonstrates how to load and use an ooSQLite package file.
 *
 *  Loading an ooSQLite package file is very similar to using the loadExtension
 *  method to load a SQLite extension library.  If the loadPackage() method is
 *  passed a valid database connection, all the collations, functions, and
 *  aggragetes are automatically registered for the database connection.
 *
 *  Once loaded, the register() method can be used to automatically register
 *  everything in the pacakge for any database connection passed to the register
 *  method.
 *
 *  Also, once loaded, individual collations and functions can be retrieved from
 *  the package object and used individually in the createCollation() or
 *  createFunction() methods.
 *
 */

  os = getOSName()

  if os == 'WINDOWS' then do
    packageFile = 'user.extensions\examplePackage.dll'
  end
  else do
    packageFile = 'user.extensions/libexamplePackage.so'
  end

  -- Set the result set format to an array of arrays:
  .ooSQLite~recordFormat = .ooSQLite~OO_ARRAY_OF_ARRAYS

	dbName = 'ooFoods.rdbx'
  dbConn = .ooSQLiteConnection~new(dbName, .ooSQLite~OPEN_READWRITE)

  -- Load the package and automatically register everything in the package for
  -- out open database connection.
  success = .ooSQLExtensions~loadPackage(packageFile, dbConn)
  if \ success then do
    say 'Failed to load package'
    say '  Error code:   ' .ooSQLExtensions~lastErrCode
    say '  Error message:' .ooSQLExtensions~lastErrMsg

    return .ooSQLExtensions~lastErrCode
  end

  sql = "SELECT season, strAggregate(name) from episodes group by season;"
  resultSet = dbConn~exec(sql, .true)

  say 'SQL:             ' sql
  say 'Result Set:      ' resultSet
  say 'Result Set Class:' resultSet~class
  say
  say 'Hit enter to continue'
  pull
  z = printStrAgg(resultSet)
  say

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

  sql = "SELECT half(11);"
  resultSet = dbConn~exec(sql, .true)

  say 'SQL:             ' sql
  say 'Result Set:      ' resultSet
  say 'Result Set Class:' resultSet~class
  say
  say 'Hit enter to continue'
  pull
  z = printResultSet(resultSet)

  ret = dbConn~close

  -- Open a new database connection.  This new connection will not have any
  -- user defined collations, functions, or aggregates.  These user defined
  -- things can only be registered per database connection.
  dbConn2 = .ooSQLiteConnection~new(dbName, .ooSQLite~OPEN_READWRITE)

  -- Get our already loaded package from the .ooSQLExtensions class object.
  -- This object is essentially a manager for all external extensions.
  package = .ooSQLExtensions~getPackage('examplePackage')

  -- Now have the package register everything in it automatically with the
  -- second database connection.
  package~register(dbConn2)

  sql = "SELECT half(11);"
  resultSet = dbConn2~exec(sql, .true)

  say 'SQL:             ' sql
  say 'Result Set:      ' resultSet
  say 'Result Set Class:' resultSet~class
  say
  say 'Hit enter to continue'
  pull
  z = printResultSet(resultSet)

  dbConn2~close

  -- Another example with a new database connection.
  dbConn3 = .ooSQLiteConnection~new(dbName, .ooSQLite~OPEN_READWRITE)

  -- Get the package and rather than register everything in the package,
  -- register a single collation from the package.
  package = .ooSQLExtensions~getPackage('examplePackage')

  collation = package~getCollation('reverse')
  dbConn3~createCollation('reverse', collation)

  sql = "SELECT * FROM foods where name like 'J%' ORDER BY name COLLATE REVERSE;"
  resultSet = dbConn3~exec(sql, .true)

  say 'SQL:             ' sql
  say 'Result Set:      ' resultSet
  say 'Result Set Class:' resultSet~class
  say
  say 'Hit enter to continue'
  pull
  z = printResultSet(resultSet)

  dbConn3~close

  -- Similar example to the last one.  This time only register a function and
  -- an aggregate with a new database connection.
  dbConn4 = .ooSQLiteConnection~new(dbName, .ooSQLite~OPEN_READWRITE)

  -- Get the package and register a single function and a single aggregate from
  -- the package.
  package = .ooSQLExtensions~getPackage('examplePackage')

  function  = package~getFunction('half')
  aggregate = package~getFunction('strAggregate')

  dbConn4~createFunction('half', function)
  dbConn4~createFunction('strAggregate', aggregate)

  sql = "SELECT half(11);"
  resultSet = dbConn4~exec(sql, .true)

  say 'SQL:             ' sql
  say 'Result Set:      ' resultSet
  say 'Result Set Class:' resultSet~class
  say
  say 'Hit enter to continue'
  pull
  z = printResultSet(resultSet)


  sql = "SELECT season, strAggregate(name) from episodes group by season;"
  resultSet = dbConn4~exec(sql, .true)

  say 'SQL:             ' sql
  say 'Result Set:      ' resultSet
  say 'Result Set Class:' resultSet~class
  say
  say 'Hit enter to continue'
  pull
  z = printStrAgg(resultSet)
  say

  ret = dbConn4~close

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


