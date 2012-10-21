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
 *  pullColumnData.rex
 *
 * This program shows how to get the data for a specific column name in a SELECT
 * query.  It uses the 'classic Rexx' interface to SQLite.
 *
 * Once you have the handle to a prepared statementm you can get the column
 * index using the name of the column.  You can then use that in any other
 * columnXXXX method that takes a column number.
 *
 * Columns are 1-indexed in ooSQLite as would be expected.  Note that when
 * reading the SQLite documentation, columns are actually 0-indexed in SQLite.
 *
 * The second argument in ooSQLOpen() is the *name* of the variable that will be
 * set to the handle of the database connection.  Likewise the third argument in
 * oosqlPrepare() is the *name* of the variable that will be set to the handle
 * of the prepared statement.  Note here that you can explicitly name the
 * variable by setting it to some value beforehand, as in: dbConn = ''
 *
 * But that is not necessary.  You can simply name it in the argument list as is
 * done in the call to oosqlPrepare().
 *
 * Lastly, no return codes are checked here because this simple program won't
 * fail.  But, wait.  What if someone moves the database file?  What if someone
 * overwrites the file with an non-database file?  What if someone manipulates
 * the database file and removes the foods table, or removes the name column?
 *
 * The fact is that there are a lot of ways this program could fail, we just
 * don't expect it to.
 */

  dbConn = ''
  ret = oosqlOpen('ooFoods.rdbx', 'dbConn')

  ret = oosqlPrepare(dbConn, "SELECT * FROM foods", 'stmt')
  index = oosqlColumnIndex(stmt, 'name')

  do while oosqlStep(stmt) == .ooSQLite~ROW
    say oosqlColumnText(stmt, index)
  end
  say

::requires 'ooSQLite.cls'

