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
 *  listBuiltins.rex
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

  -- Here we invoke the listBuiltins() method for each of the 4 different
  -- formats for returning a result set.  The listBuiltins() method returns the
  -- list of builtin extensions is the form of a result set.  This is intended
  -- to make it similar to a pragma command.

  do i = 1 to 4
    if i == 1 then do
      .ooSQLite~recordFormat = .ooSQLite~OO_ARRAY_OF_ARRAYS
      say 'Result set format is an array of arrays.'
      say

      rs = .ooSQLExtensions~listBuiltins
      z  = printResultSetA(rs)
    end
    else if i == 2 then do
      .ooSQLite~recordFormat = .ooSQLite~OO_ARRAY_OF_DIRECTORIES
      say 'Result set format is an array of directories (default.)'
      say

      rs = .ooSQLExtensions~listBuiltins
      z  = printResultSetD(rs)
    end
    else if i == 3 then do
      .ooSQLite~recordFormat = .ooSQLite~OO_STEM_OF_STEMS
      say 'Result set format is a stem of stems.'
      say

      rs = .ooSQLExtensions~listBuiltins
      z  = printResultSetS(rs)
    end
    else if i == 4 then do
      .ooSQLite~recordFormat = .ooSQLite~OO_CLASSIC_STEM
      say 'Result set format is a classic stem.'
      say

      rs = .ooSQLExtensions~listBuiltins
      z  = printResultSetCS(rs)
    end
  end


::requires 'ooSQLite.cls'

::routine printResultSetD
  use arg rs

  say 'Result:'

  -- We cheat with the indexes.  I know that the indexes are NAME and
  -- DESCRIPTION, so I could just use those.  I go ahead and get the indexes as
  -- if I didn't already know them.  But, the because I want the NAME column
  -- first, I reverse sort the indexes.
  indexes = rs[1]~allIndexes
  indexes~sortWith(.DescendingComparator~new)

  line = indexes[1]~left(11) indexes[2]
  say line
  say '='~copies(80)

  do rec over rs
    line = rec[indexes[1]]~left(11) rec[indexes[2]]
    say line
  end
  say; say; say

  return 0


::routine printResultSetA
  use arg rs

  say 'Result:'

  rowCount = rs~items

  record = rs[1]
  line = record[1]~left(11) record[2]

  say line
  say '='~copies(80)

  do i = 2 to rowCount
    record = rs[i]
    line = record[1]~left(11) record[2]

    say line
  end
  say; say; say

  return 0


::routine printResultSetS
  use arg rs.

  say 'Result:'

  -- We know now there is at least one record at rs.1, which is a stem.  We
  -- get the stem and then get its indexes
  st = rs.1
  indexes = st~allIndexes

  -- We cheat with the indexes.  I know that the indexes are NAME and
  -- DESCRIPTION, so I could just use those.  I go ahead and get the indexes as
  -- if I didn't already know them.  But, the because I want the NAME column
  -- first, I reverse sort the indexes.
  indexes~sortWith(.DescendingComparator~new)

  line = indexes[1]~left(11) indexes[2]
  say line
  say '='~copies(80)

  do i = 1 to rs.0
    rec. = rs.i
    idx1 = indexes[1]
    idx2 = indexes[2]
    line = rec.idx1~left(11) rec.idx2
    say line
  end
  say; say; say

  return 0


::routine printResultSetCS
  use arg rs.

  say 'Result:'

  -- We know now there is at least one record at rs.1.
  indexSet = getIndexes(rs.)

  indexes = indexSet~allIndexes
  indexes~sortWith(.DescendingComparator~new)

  line = indexes[1]~left(11) indexes[2]
  say line
  say '='~copies(80)

  do i = 1 to rs.0
    idx1 = i || '.' || indexes[1]
    idx2 = i || '.' || indexes[2]
    line = rs.idx1~left(11) rs.idx2
    say line
  end
  say; say; say

  return 0

-- There is no good way to get the column names from the 'classic Rexx'
-- format.  Normally in a program the column names would be known.  But for
-- a generic routine to print a result set in the classic Rexx format, the
-- column names aren't known.  Here we just brute force it, look at every index,
-- if the index starts with '1.' then consider everything after the '1.' the
-- column name, collect them in a .Set.
--
-- This assumes there is a record 1.
::routine getIndexes
  use strict arg rs.

  indexes = .set~new

  idxs = rs.~allIndexes
  do i over idxs
    if i~left(2) == '1.' then indexes~put(i~substr(3))
  end

  return indexes
