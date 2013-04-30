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
 *  collationCustom.rex
 *
 *  This example demonstrates the createCollation() and collationNeeded()
 *  methods of the ooSQLiteConnection class.  Both of these methods register
 *  callback methods in a Rexx object.
 */

  -- Set the result set format to an array of arrays:
  .ooSQLite~recordFormat = .ooSQLite~OO_ARRAY_OF_ARRAYS

	dbName = 'ooFoods.rdbx'

  dbConn = .ooSQLiteConnection~new(dbName, .ooSQLite~OPEN_READWRITE)

  -- Instantiate our Rexx object that contains our callback methods.
  collater = .Collater~new

  -- In this first example we created a custom collation using a callback method
  -- in a Rexx object.
  --
  -- 1st arg name of COLLATE function, 2nd arg Rexx object with method
  -- 3rd arg collation method, 4th arg any Rexx object
  dbConn~createCollation('ReverseSort', collater, 'reverseCollate', collater)

  sql = "SELECT * FROM foods ORDER BY name COLLATE REVERSESORT;"
  resultSet = dbConn~exec(sql, .true)

  z = printResultSet(resultSet)
  say

  -- When the database connection is closed, the custom collation is unregisterd
  ret = dbConn~close

  -- In this second example, we do not create a custom collation.  Instead we
  -- register a collation needed callback.  When SQLite encounters an undefined
  -- collation it invokes the callback.  Within the callback, the programmer
  -- should create the needed collation.

  -- We use a different name for the database connection to emphasis that this
  -- is a completely different connection than the dbConn connection above.
  connection = .ooSQLiteConnection~new(dbName, .ooSQLite~OPEN_READWRITE)

  connection~collationNeeded(collater, collationNeeded, collater)

  -- With this connection we are going to use 2 different custom collations.
  -- Just to show how the collation need call back works.
  sql = "SELECT * FROM foods where name like 'J%' ORDER BY name COLLATE REVERSESORT;"
  resultSet = connection~exec(sql, .true)

  z = printResultSet(resultSet)
  say

  -- Use a second collation here.
  sql = "SELECT * FROM foods ORDER BY name COLLATE CASEDREVERSESORT;"
  resultSet = connection~exec(sql, .true)

  z = printResultSet(resultSet)
  say

  -- Always close any open database connections.
  ret = connection~close

  return ret

::requires 'ooSQLite.cls'

-- The Collater class is used to define our callback methods.
::class 'Collater'

-- Our registered callback method.  SQLite invokes this method when it
-- encounters and undefined collation.
::method collationNeeded
  use arg dbConn, collationName, userData

  say 'collationNeeded() collation name:' collationName
  say

  -- We check the collation name to determine which collation to create.
  if collationName~caseLessCompare('ReverseSort') == 0 then do
      -- 1st arg name of COLLATE function, 2nd arg Rexx object with method
      -- 3rd arg collation method, 4th arg any Rexx object
      dbConn~createCollation('ReverseSort', self, 'reverseCollate', self)
  end

  if collationName~caseLessCompare('CasedReverseSort') == 0 then do
      -- 1st arg name of COLLATE function, 2nd arg Rexx object with method
      -- 3rd arg collation method, 4th arg any Rexx object
      dbConn~createCollation('CasedReverseSort', self, 'reverseCollateWithCase', self)
  end

  -- Must return a result
  return .ooSQLiteConstants~OK

-- Our first registered collation method.  SQLite invokes this method to compare
-- 2  strings.  We must return 0 if the strings are equal, a negative number if the
-- first string is less than the second, and a positive number if the first
-- string is greater than the second.
--
-- We just do a caseless reverse sort by negating the return from
-- caselessCompareTo()
::method reverseCollate
  use arg str1, str2, userData

  return - str1~caselessCompareTo(str2);


-- Our second registere collation method. Here we also do a reverse sort, but
-- we use case sensitive sort.
::method reverseCollateWithCase
  use arg str1, str2, userData

  return - str1~CompareTo(str2);


-- Common utility routine used to print a result set that is an array of arrays.
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
