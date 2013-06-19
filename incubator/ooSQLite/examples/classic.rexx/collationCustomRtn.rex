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
 *  collationCustomRtn.rex
 *
 * This example shows how to create user defined collations implemented in Rexx
 * code.  It also shows how to use the collation needed callback with the
 * callback implmented in Rexx code.
 *
 */

	dbName = 'ooFoods.rdbx'
  dbConn = ''

  ret = oosqlOpen(dbName, 'dbConn', .ooSQLite~OPEN_READWRITE)

  -- In this first example we created a custom collation using a public callback
  -- routine in this program.
  --
  -- 1st arg is the db connection, 2nd arg is the name of the collation routine,
  -- 3rd arg is the collation name, 4th arg any Rexx object, but we do not
  -- use it.
  ret = oosqlCreateCollation(dbConn, 'REVERSECOLLATE', 'ReverseSort')

  sql       = "SELECT * FROM foods ORDER BY name COLLATE REVERSESORT;"
  resultSet = oosqlExec(dbConn, sql, .true, .ooSQLite~OO_ARRAY_OF_ARRAYS)

  z = printResultSet(resultSet)
  say

  -- When the database connection is closed, the custom collation is unregisterd
  ret = oosqlClose(dbConn)

  -- In this second example, we do not create a custom collation.  Instead we
  -- register a collation needed callback.  When SQLite encounters an undefined
  -- collation it invokes the callback.  Within the callback, the programmer
  -- should create the needed collation.

  -- We use a different name for the database connection to emphasis that this
  -- is a completely different connection than the dbConn connection above.
  connection = ''
  ret = oosqlOpen(dbName, 'connection', .ooSQLite~OPEN_READWRITE)

  ret = oosqlCollationNeeded(connection, 'COLLATIONNEEDED')

  -- With this connection we are going to use 2 different custom collations.
  -- Just to show how the collation needed call back works.
  sql = "SELECT * FROM foods where name like 'J%' ORDER BY name COLLATE REVERSESORT;"
  resultSet = oosqlExec(connection, sql, .true, .ooSQLite~OO_ARRAY_OF_ARRAYS)

  z = printResultSet(resultSet)
  say

  -- Use a second collation here.
  sql = "SELECT * FROM foods ORDER BY name COLLATE CASEDREVERSESORT;"
  resultSet = oosqlExec(connection, sql, .true, .ooSQLite~OO_ARRAY_OF_ARRAYS)

  z = printResultSet(resultSet)
  say

  -- Always close any open database connections.
  ret = oosqlClose(connection)

  return ret

::requires 'ooSQLite.cls'
::requires '..\utilities.frm'

-- Our registered collation needed callback routine.  SQLite invokes this
-- routine when it encounters an undefined collation.
::routine collationNeeded public
  use arg db, collationName, userData

  say 'collationNeeded() collation name:' collationName
  say

  -- We check the collation name to determine which collation to create.
  if collationName~caseLessCompare('ReverseSort') == 0 then do
      -- 1st arg database connection that the COLLATE function will be
      -- registered with, 2nd arg is the name of the collation routine,
      -- 3rd arg is the collation name as known to SQLite, 4th arg any Rexx
      -- object, but we do not use it.
      ret = oosqlCreateCollation(db, 'REVERSECOLLATE', 'ReverseSort')
  end

  if collationName~caseLessCompare('CasedReverseSort') == 0 then do
      ret = oosqlCreateCollation(db, 'REVERSECOLLATEWITHCASE', 'CasedReverseSort')
  end

  -- Must return a result
  return .ooSQLiteConstants~OK

-- Our first registered collation routine.  SQLite invokes this method to
-- compare 2  strings.  We must return 0 if the strings are equal, a negative
-- number if the first string is less than the second, and a positive number if
-- the first string is greater than the second.
--
-- We just do a caseless reverse sort by negating the return from
-- caselessCompareTo()
::routine reverseCollate public
  use arg str1, str2, userData

  return - str1~caselessCompareTo(str2);


-- Our second registered collation routine. Here we also do a reverse sort, but
-- we use case sensitive sort.
::routine reverseCollateWithCase public
  use arg str1, str2, userData

  return - str1~CompareTo(str2);

