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
 *  printTableWtihNulls.rex
 *
 * This program is called from some of the other examples to print out the
 * values of a table where the table can contain SQL NULL values.
 *
 * It is an error to run this from the command line.
 *
 * For example, the insertNull.rex and insertNullTwo.rex examples call this
 * program as a function.  To see the output from this example, execute one of
 * those 2 examples.
 *
 */
 use strict arg db, table

  if \ db~isA(.ooSQLiteConnection) then do
    say "Incorrect usage of 'printTableWithNulls'."
    say "  This function can not be run from the command line."
    say "  This function is called correctly from some of the example programs."
    return .false
  end

  -- Print out the data in the table. We use a generic algorithm here where the
  -- number of the columns is unknown to begin with.  This will sometimes
  -- truncate the data in a column if it is too long.

  sql = "SELECT * FROM" table";"
  stmt = .ooSQLiteStmt~new(db, sql)
  if stmt~initCode \== stmt~OK then do
    say 'Error in SQL:' sql
    say '  rc:' stmt~lastErrCode 'msg:' stmt~lastErrMsg
    say
    return .false
  end

  n = stmt~columnCount

  header = ''
  do i = 1 to n
    header ||= stmt~columnName(i)~left(17)
  end
  say header
  say '='~copies(80)

  -- In this approach a NULL in a column will be printed as NULL
  stmt~null = 'NULL'
  do while stmt~step == stmt~ROW
    row = ''
    do i = 1 to n
      row ||= stmt~columnText(i)~left(17)
    end
    say row
  end
  say

  -- Print out the data in the table again.
  stmt~reset

  header = ''
  do i = 1 to n
    header ||= stmt~columnName(i)~left(17)
  end
  say header
  say '='~copies(80)

  -- In this approach NULLs in the table are returned as the .nil object.  This
  -- is actually the default, but since we set the null attribute for the stmt
  -- above to a non-default, we need to reset the attribute here.
  stmt~null = .nil
  do while stmt~step == stmt~ROW
    row = ''

    do i = 1 to n
      f = stmt~columnText(i)
      row ||= left(f, 17)
    end

    say row
  end
  say

  stmt~finalize

  return .true

::requires 'ooSQLite.cls'
