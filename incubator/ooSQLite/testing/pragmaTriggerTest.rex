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
 *  pragmaTriggerTest.rex
 *
 *  One of the non-standard SQL features of SQLitet is the PRAGMA statement.
 *
 *  "The pragma command is specific to SQLite and is very unlikely to be
 *   compatible with any other SQL database engine."
 *
 *  PRAGMA statements are executed like any other SQL.  Some pragmas return
 *  tables, some do not.  Unknown pragmas are ignored by SQLite.  Some pragmas
 *  set values, some return a single value.
 *
 *  Because of these differences, ooSQLite wraps the execution of a pragma in
 *  the pragma() method. You would not have to use the pragma() method, you
 *  could instantiate an ooSQLiteStmt and execute the SQL like any other SQL.
 *
 *  This test issues some pragmas that trigger an action, and mostly, return a
 *  table.  In addition it tests changing the the format of a returned result
 *  set.
 */

  dbFile = 'ooFoods.rdbx'

  -- When opening the connection to the database here, we change the default
  -- record format to an array of arrays:

  db = .ooSQLiteConnection~new(dbFile, .ooSQLite~OPEN_READWRITE, .ooSQLite~OO_ARRAY_OF_ARRAYS)
  if db~initCode <> 0 then do
    say 'ooSQLiteConnection initialization err:' db~initCode
    say '  Error code:' db~lastErrCode '('db~lastErrMsg')'
    if db~initCode == db~CANTOPEN then do
      say '  Database file name:' db~fileName '(Is this the correct database?)'
    end
    say;say;say
    db~close
    return 99
  end

  names = getPragmaNames()

  -- Here we invoke the pragma method for each pragma listed in names using the
  -- 3 different formats for returning a result set:

  do i = 1 to 3
    if i == 1 then do
      say 'Result set format is an array of arrays.'
      say
    end
    else if i == 2 then do
      say 'Result set format is an array of directories (default.)'
      say
      db~recordFormat = db~OO_ARRAY_OF_DIRECTORIES
    end
    else if i == 3 then do
      say 'Result set format is a stem of stems.'
      say
      db~recordFormat = db~OO_STEM_OF_STEMS
    end

    do n over names
      ret = db~pragma(n)

      if n == 'shrink_memory' then do
        say 'Pragma:' n 'Result:' ret
        say; say; say
      end
      else do
        select
          when db~recordFormat == db~OO_ARRAY_OF_ARRAYS then do
            z = printResultSetA(n, ret)
          end
          when db~recordFormat == db~OO_ARRAY_OF_DIRECTORIES then do
            z = printResultSetD(n, ret)
          end
          when db~recordFormat == db~OO_STEM_OF_STEMS then do
            z = printResultSetS(n, ret)
          end
        end
        -- End select
      end

    end
  end


  db~close

::requires 'ooSQLite.cls'

::routine getPragmaNames

  n = .array~of( -
    "integrity_check",  -
    "quick_check",  -
    "shrink_memory",  -
    "main.wal_checkpoint",  -                -- must have .
    )

    return n


::routine printResultSetD
  use arg n, rs

  say 'Pragma:' n
  if rs~items == 0 then do
    say 'Result: No data'
    return 0
  end

  say 'Result:'

  indexes = rs[1]~allIndexes
  line    = ''
  do index over indexes
    line ||= index~left(25)
  end
  say line
  say '='~copies(80)

  do rec over rs
    line    = ''
    do index over indexes
      line ||= rec[index]~left(25)
    end
    say line
  end
  say; say; say

  return 0


::routine printResultSetA
  use arg n, ret

  say 'Pragma:' n
  if ret~items == 0 then do
    say 'Result: No data'
    return 0
  end

  say 'Result:'

  colCount = ret[1]~items
  rowCount = ret~items

  line = ''
  record = ret[1]
  do j = 1 to colCount
    line ||= record[j]~left(12)
  end

  say line
  say '='~copies(80)

  do i = 2 to rowCount
    line = ''
    record = ret[i]
    do j = 1 to colCount
      line ||= record[j]~left(12)
    end

    say line
  end
  say; say; say

  return 0


::routine printResultSetS
  use arg n, rs.

  say 'Pragma:' n
  if rs.0 == 0 then do
    say 'Result: No data'
    say
    return 0
  end

  say 'Result:'

  -- We know now there is at least one record at rs.1, which is a stem.  We
  -- get the stem and then get its indexes
  st = rs.1
  indexes = st~allIndexes
  line    = ''
  do index over indexes
    line ||= index~left(25)
  end
  say line
  say '='~copies(80)

  do i = 1 to rs.0
    rec. = rs.i
    line    = ''
    do index over indexes
      line ||= rec.index~left(25)
    end
    say line
  end
  say; say; say

  return 0

