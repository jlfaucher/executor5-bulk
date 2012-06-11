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
 *  Tests the classic Rexx interface to ooSQLite.
 *
 *  Opens the ooFoods database, creates a prepared statement that queries the
 *  database and steps through the result set printing the rows returned.
 *
 *  This program does the error checking that is missing in some of the other
 *  test programs.
 */

  dbName = 'ooFoods.rdbx'

  dbConn = oosqlOpen(dbName, .ooSQLite~OPEN_READWRITE)

  if oosqlErrCode(dbConn) <> 0 then do
    errRC   = oosqlErrCode(dbConn)
    errMsg  = oosqlErrMsg(dbConn)

    say 'Open database error'
    say '  Error code:' errRC '('errMsg')'
    if errRC == .ooSQLite~CANTOPEN then do
      say '  Database file name:' dbName '(Is this the correct database?)'
    end

    oosqlClose(dbConn)
    return 99
  end

  -- The third argument to oosqlPrepare() can be a stem.  If not omitted, the
  -- stem will have a tail added to the stem that is set to the unused portion
  -- of the sql string.  Note that SQLite calls this string the tail, and the
  -- ooSQLite implementation has choose to keep the SQLite name, even though it
  -- may sound a bit confusing with the tail of a stem.  The tail name will be:
  -- OOSQLITE_SQLTAIL

  -- Note the nonsense at the end of the first SQL statement.  This string will
  -- be returned in the stem.  Further note that the stem is not set if there is
  -- an error return from oosqlPrepare().  (You can generate an error by adding
  -- a syntax error to the SELECT statement.)
  stmt = oosqlPrepare(dbConn, 'SELECT * FROM foods ORDER BY name; INSERT nonsense INTO foodes', unused.)
  if stmt~isNull then do
    say 'Prepared statment initialization error:'
    say '  Error code:' oosqlErrCode(dbConn) '('oosqlErrMsg(dbConn)')'

    str = unused.oosqlite_sqlTail; say 'unused SQL:' str 'unused SQL length:' length(str)

    ret = oosqlFinalize(stmt); say 'Error finalize ret:' ret
    ret = oosqlClose(dbConn) ; say 'Error close    ret:' ret
    return 99
  end
  else do
    say 'Unused portion of sql:'
    str = unused.oosqlite_sqlTail
    say '  string: ' str
    say '  length: ' length(str)
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
