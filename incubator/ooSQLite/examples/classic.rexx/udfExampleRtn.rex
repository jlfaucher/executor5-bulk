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
 *  udfExampleRtn.rex
 *
 *  This example demonstrates the oosqlCreateFunction() routine of the classic
 *  Rexx interface.  The oosqlCreateFunction() routine is used to add SQL
 *  functions or aggregates, or to redefine the behavior of existing SQL
 *  functions or aggregates.
 *
 *  The functions or aggregates can be implemented externally in C / C++ code,
 *  or they can be implemented in Rexx.  This example shows the implementaion of
 *  some user defined functions in Rexx.
 */

	dbName = 'ooFoods.rdbx'
  dbConn = ''

  ret = oosqlOpen(dbName, 'dbConn', .ooSQLite~OPEN_READWRITE)

  -- Create an aggregate
  ret = oosqlCreateFunction(dbConn, 'strAggregate', 1,  , 'strAggStep', 'strAggFinal')

  sql = "SELECT season, strAggregate(name) from episodes group by season;"
  resultSet = oosqlExec(dbConn, sql, .true, .ooSQLite~OO_ARRAY_OF_ARRAYS)

  z = printStrAgg(resultSet)
  say

  ret = oosqlCreateFunction(dbConn, 'helloWorld', 1, 'helloWorld')
  ret = oosqlCreateFunction(dbConn, 'half', 1, 'half')
  ret = oosqlCreateFunction(dbConn, 'echo', 1, 'echo')

  sql = "SELECT helloWorld('Rick');"
  resultSet = oosqlExec(dbConn, sql, .true, .ooSQLite~OO_ARRAY_OF_ARRAYS)

  z = printResultSet(resultSet)
  say

  sql = "SELECT helloWorld('ooRexx');"
  resultSet = oosqlExec(dbConn, sql, .true, .ooSQLite~OO_ARRAY_OF_ARRAYS)

  z = printResultSet(resultSet)
  say

  sql = "SELECT half(11);"
  resultSet = oosqlExec(dbConn, sql, .true, .ooSQLite~OO_ARRAY_OF_ARRAYS)

  z = printResultSet(resultSet)
  say

  sql = "SELECT half(197);"
  resultSet = oosqlExec(dbConn, sql, .true, .ooSQLite~OO_ARRAY_OF_ARRAYS)

  z = printResultSet(resultSet)
  say

  sql = "SELECT half(114599);"
  resultSet = oosqlExec(dbConn, sql, .true, .ooSQLite~OO_ARRAY_OF_ARRAYS)

  z = printResultSet(resultSet)
  say

  sql = "SELECT echo(56.78) as reply, typeof(echo(56.78)) as type;"
  resultSet = oosqlExec(dbConn, sql, .true, .ooSQLite~OO_ARRAY_OF_ARRAYS)

  z = printResultSet(resultSet)
  say

  sql = "SELECT echo(X'ab24') as reply, typeof(echo(X'ab24')) as type;"
  resultSet = oosqlExec(dbConn, sql, .true, .ooSQLite~OO_ARRAY_OF_ARRAYS)

  z = printResultSet(resultSet)
  say

  sql = "SELECT echo(NULL) as reply, typeof(echo(NULL)) as type;"
  resultSet = oosqlExec(dbConn, sql, .true, .ooSQLite~OO_ARRAY_OF_ARRAYS)

  z = printResultSet(resultSet)
  say

  -- When the database connection is closed, the user defined functions are
  -- unregisterd.
  ret = oosqlClose(dbConn)

  return ret

::requires 'ooSQLite.cls'
::requires '..\utilities.frm'

::routine strAggStep public
  use arg db, cntx, aggregater, userData, txtToAggregate

  str = oosqlValueText(txtToAggregate)

  if aggregater == .nil then do
    aggStem.count = 1
    aggStem.completeStr = str
    return aggStem.
  end

  aggregater[count] += 1
  aggregater[completeStr] ||= ' 'str
  return .ooSQLite~OK


::routine strAggFinal public
  use arg dbConn, cntx, aggregater, userData

  ret = oosqlResultText(cntx, aggregater[completeStr])

  -- Must return a result code. You do not return an .ooSQLResult.
  return .ooSQLite~OK


-- Our registered helloWorld() callback method.  SQLite invokes this method when
-- it encounters a SQL function named helloWord
::routine helloWorld public
  use arg dbConn, cntx, userData, echoValue

  name = oosqlValueText(echoValue)
  txt  = 'Hello' name'|'

  z = oosqlResultText(cntx, txt)

  -- Must return a result code. You do not return an .ooSQLResult.
  return .ooSQLite~OK


-- Our registered half() callback method.  SQLite invokes this method when it
-- encounters a SQL function named half
::routine half public
  use arg dbConn, cntx, userData, decimalNum

  d = oosqlValueDouble(decimalNum)
  d = d / 2

  z = oosqlResultDouble(cntx, d)

  -- Must return a result code. You do not return an .ooSQLResult.
  return .ooSQLite~OK


-- Our registered echo() callback method.  SQLite invokes this method when it
-- encounters a SQL function named ehco.  All this does is set the result of the
-- function to the value it is invoked with.
::routine echo public
  use arg dbConn, cntx, userData, echoVal

  z = oosqlResultValue(cntx, echoVal)

  -- Must return a result code. You do not return an .ooSQLResult.
  return .ooSQLite~OK


