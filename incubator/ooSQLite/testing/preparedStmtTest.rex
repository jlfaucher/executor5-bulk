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
 *  preparedStmtTest.rex
 *
 *  Opens the ooFoods database, creates a prepared statement that queries the
 *  database and steps through the result set printing the rows returned.
 *
 *  This program does the error checking that is missing in some of the other
 *  test programs.
 */

	dbName = 'ooFoods.rdbx'

  dbConn = .ooSQLiteConnection~new(dbName, .ooSQLite~OPEN_READWRITE)

  if dbConn~initCode <> 0 then do
    errRC  = dbConn~lastErrCode
    errMsg = dbConn~lastErrMsg

    say 'ooSQLiteConnection initialization error:' dbConn~initCode
    say '  Error code:' errRC '('errMsg')'
    if errRC == dbConn~CANTOPEN then do
      say '  Database file name:' dbName '(Is this the correct database?)'
    end

    dbConn~close
    return 99
  end

  stmt = .ooSQLiteStmt~new(dbConn, 'SELECT * FROM foods ORDER BY name;')
  if stmt~initCode <> 0 then do
    say 'ooSQLiteStmt initialization error:' stmt~initCode
    say '  Error code:' stmt~initCode '('stmt~errMsg')'

    stmt~finialize
    dbConn~close
    return 99
  end

  stepRC = stmt~step

  if stepRC == stmt~ROW then do
    colCount = stmt~columnCount

    header = ''
    do i = 1 to colCount
      header ||= stmt~columnName(i)~left(20)
    end
    say header
    say '='~copies(80)

    do while stepRC == stmt~ROW
      row = ''
      do i = 1 to colCount
        row ||= stmt~columnText(i)~left(20)
      end
      say row

      stepRC = stmt~step
    end
  end
  else do
    say 'Unexpected error running query "SELECT * FROM foods ORDER BY name;"'
    say 'Error code:' stepRC
  end
  say

  stmt~finalize

  ret = dbConn~close

  return ret

::requires 'ooSQLite.cls'
