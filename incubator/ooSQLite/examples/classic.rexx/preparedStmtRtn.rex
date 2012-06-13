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
 *  preparedStmtTestRtn.rex
 *
 *  This example program demonstrates the classic Rexx interface to ooSQLite.
 *
 *  It opens the ooFoods database, creates a prepared statement that queries the
 *  database and steps through the result set printing the rows returned.
 *
 *  This example also demonstrates more of the error checking that is missing in
 *  some of the simple example programs.
 */

  dbName = 'ooFoods.rdbx'

  -- dbConn will be set to the handle to the database connection.  We do not
  -- need to explicitly set the variable first.  We could just name the variable
  -- in the oosqlOpen() function and the result would be the same.  But, for an
  -- example program we do this to better show what is happening.

  dbConn = ''
  ret = oosqlOpen(dbName, 'dbConn', .ooSQLite~OPEN_READWRITE)

  if ret <> .ooSQLite~OK then do
    -- dbConn could be a null handle, if and only if the return code is NOMEM.
    -- In this simple program, I would say that is impossible.  But, you can not
    -- pass in a null handle to any ooSQLite function.  So, here we show how to
    -- properly deal with this.

    if ret == .ooSQLite~NOMEM then do
      errRC  = .ooSQLite~NOMEM
      errMsg = 'memory allocation failed'
    end
    else do
      errRC  = oosqlErrCode(dbConn)
      errMsg = oosqlErrMsg(dbConn)
    end

    say 'Open database error'
    say '  Error code:' errRC '('errMsg')'
    if errRC == .ooSQLite~CANTOPEN then do
      say '  Database file name:' dbName '(Is this the correct database?)'
    end

    -- The statement that you can not pass in a null handle to any ooSQLite
    -- function is not 100% accurate.  Using null handles for oosqlClose() and
    -- oosqlFinalize() are harmles nops.  So, we don't check here.  If dbConn is
    -- a null handle, it doesn't need to be closed.  In all other cases it does.

    ret = oosqlClose(dbConn)
    return 99
  end

  -- The third argument to oosqlPrepare(), below, is the name of the variable
  -- that the handle to the prepared statement is set to.  Similar to
  -- oosqlOpen() explained above.  Here we do not explicity set the stmt
  -- variable, we just name it in the call to oosqlPrepare()
  --
  -- The fourth argument is the name of a variable whose value will be set to
  -- the 'tail' of the SQL statement.  SQLite only prepares the first statement
  -- in the SQL, that is up to and including the first semi-colon ';'.  SQLite
  -- calls the unused portion of the SQL string the tail and will return the
  -- tail if asked.  The fourth argument is opitonal.  If not omitted, the named
  -- varible will have its value set to the unused portion of the SQL string.
  -- This value will be the empty string if there is no tail.  Again, this
  -- argument is optional.
  --
  -- Note the nonsense at the end of the first SQL statement.  This string will
  -- be returned by setting the value of the variable unusedTail to the string
  -- following the first semi-colon.  Further note that the stmt variable is
  -- always set, but the unusedTail variable is never set on error.  If the
  -- return code from oosqlPrepare() is an error code, stmt will always be a
  -- null handle and unusedTail will never be set.

  sqlStatement = 'SELECT * FROM foods ORDER BY name; INSERT nonsense INTO foodes'

  ret = oosqlPrepare(dbConn, sqlStatement, 'stmt', 'unusedTail')

  if ret <> .ooSQLite~OK then do
    say 'Prepared statment initialization error:'
    say '  Error code:' oosqlErrCode(dbConn) '('oosqlErrMsg(dbConn)')'
    say '  Is stmt a null handle?' oosqlIsHandleNull(stmt)

    ret = oosqlFinalize(stmt); say 'Error finalize ret:' ret
    ret = oosqlClose(dbConn) ; say 'Error close    ret:' ret
    return 99
  end
  else do
    say 'Unused portion of sql:'
    str = unusedTail
    say '  string: ' str
    say '  length: ' length(str)
    say
  end

  stepRC = oosqlStep(stmt)

  if stepRC == .ooSQLite~ROW then do
    colCount = oosqlColumnCount(stmt)

    header = ''
    do i = 1 to colCount
      header ||= oosqlColumnName(stmt, i)~left(20)
    end
    say header
    say '='~copies(80)

    do while stepRC == .ooSQLite~ROW
      row = ''
      do i = 1 to colCount
        row ||= oosqlColumnText(stmt, i)~left(20)
      end
      say row

      stepRC = oosqlStep(stmt)
    end
  end
  else do
    say 'Unexpected error running query "SELECT * FROM foods ORDER BY name;"'
    say 'Error code:' stepRC
  end
  say

  ret = oosqlFinalize(stmt); say 'No err finalize ret:' ret
  ret = oosqlClose(dbConn) ; say 'No err close    ret:' ret
  return ret

::requires 'ooSQLite.cls'
