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
 *  execTestNoExec.rex
 *
 *  The exec() method of the database connection object is really a convenience
 *  wrapper for using a prepared statement (an .ooSQLiteStmt object.)
 *
 *  Stepping through a prepared statement gives the programmer much more
 *  control, at the price of a little more coding.  This example performs the
 *  same task as the execTest?.rex programs, but does it without using the
 *  exec() method.
 *
 *  Note that for a simple task like the one used in the execTest?.rex programs,
 *  using a prepared statement has less coding than using the exec() method.
 */

	dbName = 'ooFoods.rdbx'

  dbConn = .ooSQLiteConnection~new(dbName, .ooSQLite~OPEN_READWRITE)

  sql = 'SELECT * FROM foods ORDER BY name;'
  stmt = .ooSQLiteStmt~new(dbConn, sql)

  say stmt~columnName(1)~left(25) || stmt~columnName(2)~left(25) || stmt~columnName(3)~left(25)
  say '='~copies(80)

  do while stmt~step == stmt~ROW
    say stmt~columnText(1)~left(25) || stmt~columnText(2)~left(25) || stmt~columnText(3)~left(25)
  end
  say

  say 'Result Set:       Not used'
  say 'Result Set Class: N/A'
  say

  stmt~finalize
  ret = dbConn~close

  return ret

::requires 'ooSQLite.cls'
