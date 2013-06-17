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
 *  udfExample.rex
 *
 *  This example demonstrates the createFunction() method of the
 *  ooSQLiteConnection class.  The createFunction() method is used to add SQL
 *  functions or aggregates, or to redefine the behavior of existing SQL
 *  functions or aggregates.
 *
 *  The functions or aggregates can be implemented externally in C / C++ code,
 *  or they can be implemented in Rexx.  This example shows the implementaion of
 *  some user defined functions in Rexx.
 */

  -- Set the result set format to an array of arrays:
  .ooSQLite~recordFormat = .ooSQLite~OO_ARRAY_OF_ARRAYS

	dbName = 'ooFoods.rdbx'

  dbConn = .ooSQLiteConnection~new(dbName, .ooSQLite~OPEN_READWRITE)

  -- Instantiate our Rexx object that contains our callback methods.
  callBackObj = .FunctionImpl~new

  -- Create an aggregate
  dbConn~createFunction('strAggregate', callBackObj, 1, , 'strAggStep', 'strAggFinal')

  sql = "SELECT season, strAggregate(name) from episodes group by season;"
  resultSet = dbConn~exec(sql, .true)

  z = printStrAgg(resultSet)
  say

  dbConn~createFunction('helloWorld', callBackObj, 1, 'helloWorld')
  dbConn~createFunction('half', callBackObj, 1, 'half')
  dbConn~createFunction('echo', callBackObj, 1, 'echo')

  sql = "SELECT helloWorld('Rick');"
  resultSet = dbConn~exec(sql, .true)

  z = printResultSet(resultSet)
  say

  sql = "SELECT helloWorld('ooRexx');"
  resultSet = dbConn~exec(sql, .true)

  z = printResultSet(resultSet)
  say

  sql = "SELECT half(11);"
  resultSet = dbConn~exec(sql, .true)

  z = printResultSet(resultSet)
  say

  sql = "SELECT half(197);"
  resultSet = dbConn~exec(sql, .true)

  z = printResultSet(resultSet)
  say

  sql = "SELECT half(114599);"
  resultSet = dbConn~exec(sql, .true)

  z = printResultSet(resultSet)
  say

  sql = "SELECT echo(56.78) as reply, typeof(echo(56.78)) as type;"
  resultSet = dbConn~exec(sql, .true)

  z = printResultSet(resultSet)
  say

  sql = "SELECT echo(X'ab24') as reply, typeof(echo(X'ab24')) as type;"
  resultSet = dbConn~exec(sql, .true)

  z = printResultSet(resultSet)
  say

  sql = "SELECT echo(NULL) as reply, typeof(echo(NULL)) as type;"
  resultSet = dbConn~exec(sql, .true)

  z = printResultSet(resultSet)
  say

  -- When the database connection is closed, the user defined functions are
  -- unregisterd.
  ret = dbConn~close

  return ret

::requires 'ooSQLite.cls'
::requires 'utilities.frm'

::class 'strCollecter'
::attribute str
::method init
  use arg firstStr
  self~str = firstStr

::method addStr
  use arg str
  self~str ||= ',' str



-- The FunctionImpl class is used to define the callback methods for our
-- functions.
::class 'FunctionImpl' inherit ooSQLiteConstants

::method strAggStep
  use arg dbConn, cntx, values, collecterObj, userData

  str = .ooSQLValue~text(values[1])

  if collecterObj == .nil then do
    collObj = .strCollecter~new(str)
    return collObj
  end

  collecterObj~addStr(str)
  return self~OK


::method strAggFinal
  use arg dbConn, cntx, collecterObj, userData

  .ooSQLResult~text(cntx, collecterObj~str)

  -- Must return a result code. You do not return an .ooSQLResult.
  return self~OK


-- Our registered helloWorld() callback method.  SQLite invokes this method when
-- it encounters a SQL function named helloWord
::method helloWorld
  use arg dbConn, cntx, values, userData

  name = .ooSQLValue~text(values[1])
  txt  = 'Hello' name'|'

  .ooSQLResult~text(cntx, txt)

  -- Must return a result code. You do not return an .ooSQLResult.
  return self~OK


-- Our registered half() callback method.  SQLite invokes this method when it
-- encounters a SQL function named half
::method half
  use arg dbConn, cntx, values, userData

  d = .ooSQLValue~double(values[1])
  d = d / 2

  .ooSQLResult~double(cntx, d)

  -- Must return a result code. You do not return an .ooSQLResult.
  return self~OK


-- Our registered echo() callback method.  SQLite invokes this method when it
-- encounters a SQL function named ehco
::method echo
  use arg dbConn, cntx, values, userData

  .ooSQLResult~value(cntx, values[1])

  -- Must return a result code. You do not return an .ooSQLResult.
  return self~OK


