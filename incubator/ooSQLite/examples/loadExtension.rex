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
 *  loadExtension.rex
 *
 *  Demonstrates how to load an SQLite extensions file.
 *
 *  The extensions are implemented in simpleExtension.c which is in the
 *  user.extensions subdirectory.  On Windows simpleExtension.c is compiled into
 *  a DLL, on Linux it is compiled into a .so file.
 *
 *  One of the extensions is a collation, REVERSESORT.  After loading the
 *  extension file, we can use the REVERSESORT collation here.
 *
 */

  os = getOSName()

  if os == 'WINDOWS' then do
    extensionFile = 'user.extensions\simpleExtension.dll'
  end
  else do
    extensionFile = 'user.extensions/libsimpleExtension.so'
  end

	dbName = 'ooFoods.rdbx'

  -- Set the result set format to an array of arrays:
  .ooSQLite~recordFormat = .ooSQLite~OO_ARRAY_OF_ARRAYS

  dbConn = .ooSQLiteConnection~new(dbName, .ooSQLite~OPEN_READWRITE)

  dbConn~enableLoadExtension(.true)

  ret = dbConn~loadExtension(extensionFile)
  if ret <> dbConn~ok then do
    say 'Error loading extension:' dbConn~lastErrMsg
    return dbConn~lastErrCode
  end

  sql = "SELECT * FROM foods where name like 'J%' ORDER BY name COLLATE REVERSESORT;"
  resultSet = dbConn~exec(sql, .true)

  say 'SQL:             ' sql
  say 'Result Set:      ' resultSet
  say 'Result Set Class:' resultSet~class
  say
  say 'Hit enter to continue'
  pull
  z = printResultSet(resultSet)

  sql = "SELECT * FROM foods ORDER BY name COLLATE REVERSESORT;"
  resultSet = dbConn~exec(sql, .true)

  say 'SQL:             ' sql
  say 'Result Set:      ' resultSet
  say 'Result Set Class:' resultSet~class
  say
  say 'Hit enter to continue'
  pull
  z = printResultSet(resultSet)
  ret = dbConn~close

  return ret

::requires 'ooSQLite.cls'

::class 'Collater'

::method reverseCollate
  use arg str1, str2, userData

  return - str1~caselessCompareTo(str2);

::routine printResultSet
  use arg rs

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
  say

  return 0

::routine getOSName

  parse upper source os .
  if os~abbrev("WIN") then os = "WINDOWS"
  return os


