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

  .ooSQLite~recordFormat = .ooSQLite~OO_ARRAY_OF_ARRAYS
  dbName = 'ooFoods.rdbx'

  dbConn = .ooSQLiteConnection~new(dbName, .ooSQLite~OPEN_READWRITE)

  if dbConn~initCode <> 0 then do
    errRC  = dbConn~lastErrCode
    errMsg = dbConn~lastErrMsg

    say 'ooSQLiteConnection initialization error:' dbConn~initCode
    say '  Error code:' errRC '('errMsg')'
    if errRC == .ooSQLite~CANTOPEN then do
      say '  Database file name:' dbName '(Is this the correct database?)'
    end

    dbConn~close
    return 99
  end

  ret = dbConn~exec('SELECT * FROM foods ORDER BY name;', .true)
  if \ ret~isA(.array) then do
    say ret
    ret = 99
  end
  else do
    z = printResultSet(ret)
    ret = 0
  end

  say; say 'Start of status test.  Print SQLite status parameters:'; say

  a = .array~of(                                    -
                .ooSQLite~STATUS_MEMORY_USED          ,-
                .ooSQLite~STATUS_PAGECACHE_USED       ,-
                .ooSQLite~STATUS_PAGECACHE_OVERFLOW   ,-
                .ooSQLite~STATUS_SCRATCH_USED         ,-
                .ooSQLite~STATUS_SCRATCH_OVERFLOW     ,-
                .ooSQLite~STATUS_MALLOC_SIZE          ,-
                .ooSQLite~STATUS_PARSER_STACK         ,-
                .ooSQLite~STATUS_PAGECACHE_SIZE       ,-
                .ooSQLite~STATUS_SCRATCH_SIZE         ,-
                .ooSQLite~STATUS_MALLOC_COUNT         -
                )

  n = .array~of(                                    -
                'STATUS_MEMORY_USED       '   ,-
                'STATUS_PAGECACHE_USED    '   ,-
                'STATUS_PAGECACHE_OVERFLOW'   ,-
                'STATUS_SCRATCH_USED      '   ,-
                'STATUS_SCRATCH_OVERFLOW  '   ,-
                'STATUS_MALLOC_SIZE       '   ,-
                'STATUS_PARSER_STACK      '   ,-
                'STATUS_PAGECACHE_SIZE    '   ,-
                'STATUS_SCRATCH_SIZE      '   ,-
                'STATUS_MALLOC_COUNT      '   -
                )

  values = .directory~new
  do i = 1 to a~items
    .ooSQLite~status(a[i], values)
    say n[i]': current:' values~current~left(10) 'high water:' values~highWater
  end


  say; say 'Print SQLite status parameters and reset high water:'; say

  do i = 1 to a~items
    .ooSQLite~status(a[i], values, .true)
    say n[i]': current:' values~current~left(10) 'high water:' values~highWater
  end

  say; say 'Print SQLite status parameters, again, after reset high water:'; say

  do i = 1 to a~items
    .ooSQLite~status(a[i], values)
    say n[i]': current:' values~current~left(10) 'high water:' values~highWater
  end


  say; say 'Print SQLiteDB status parameters:'; say

  a = .array~of(                                    -
                .ooSQLite~DBSTATUS_LOOKASIDE_USED     ,-
                .ooSQLite~DBSTATUS_CACHE_USED         ,-
                .ooSQLite~DBSTATUS_SCHEMA_USED        ,-
                .ooSQLite~DBSTATUS_STMT_USED          ,-
                .ooSQLite~DBSTATUS_LOOKASIDE_HIT      ,-
                .ooSQLite~DBSTATUS_LOOKASIDE_MISS_SIZE,-
                .ooSQLite~DBSTATUS_LOOKASIDE_MISS_FULL,-
                .ooSQLite~DBSTATUS_CACHE_HIT          ,-
                .ooSQLite~DBSTATUS_CACHE_MISS          -
                )

  n = .array~of(                                  -
                'DBSTATUS_LOOKASIDE_USED     '   ,-
                'DBSTATUS_CACHE_USED         '   ,-
                'DBSTATUS_SCHEMA_USED        '   ,-
                'DBSTATUS_STMT_USED          '   ,-
                'DBSTATUS_LOOKASIDE_HIT      '   ,-
                'DBSTATUS_LOOKASIDE_MISS_SIZE'   ,-
                'DBSTATUS_LOOKASIDE_MISS_FULL'   ,-
                'DBSTATUS_CACHE_HIT          '   ,-
                'DBSTATUS_CACHE_MISS         '    -
                )

  do i = 1 to a~items
    dbConn~dbStatus(a[i], values)
    say n[i]': current:' values~current~left(10) 'high water:' values~highWater
  end
  say


  say; say 'Print SQLiteDB status parameters and reset high water:'; say

  do i = 1 to a~items
    dbConn~dbStatus(a[i], values, .true)
    say n[i]': current:' values~current~left(10) 'high water:' values~highWater
  end
  say

  say; say 'Print SQLiteDB status parameters again, after reset high water:'; say

  do i = 1 to a~items
    dbConn~dbStatus(a[i], values, .true)
    say n[i]': current:' values~current~left(10) 'high water:' values~highWater
  end
  say

  ret = dbConn~close

  return ret

::requires 'ooSQLite.cls'


::routine printResultSet
  use arg rs

  say 'Result:'

  colCount = rs[1]~items
  rowCount = rs~items

  line = ''
  headers = rs[1]
  do j = 1 to colCount
    line ||= headers[j]~left(25)
  end

  say line
  say '='~copies(80)

  do i = 2 to rowCount
    line = ''
    record = rs[i]
    do j = 1 to colCount
      line ||= record[j]~left(25)
    end

    say line
  end

  return 0
