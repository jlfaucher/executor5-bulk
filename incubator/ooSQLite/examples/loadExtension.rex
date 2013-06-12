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
 *  SQLite has the ability to load extensions at run-time.  An extension can
 *  include new application-defined SQL functions, collating sequences, virtual
 *  tables, and VFSes.  The extensions are, usually, written in a C / C++.
 *
 *  The extensions for this example are implemented in simpleExtension.c which
 *  is in the user.extensions subdirectory.  On Windows simpleExtension.c is
 *  compiled into a DLL, on Linux it is compiled into a .so file.
 *
 *  The extension file implements 1 function, 1 aggregate, and 2 collations.
 *
 *  When the extension file is loaded through loadExtension(), SQLite
 *  automatically registers the extensions in the extension library.  For this
 *  example, the registered names are as follows:
 *
 *   Type           Registered Name
 *   =======================================
 *   function        ->   half
 *   collation       ->   reverse
 *   collation       ->   ebcdic
 *   aggregate       ->   strAggregate
 *
 *  In SQLite the loading of extensions is disabled by default and currently
 *  ooSQLite is built to conform to that.  So, to use an extension file, first
 *  extension loading must be enabled and then the extension library must be
 *  loaded.
 *
 */

  os = getOSName()

  if os == 'WINDOWS' then do
    extensionFile = 'user.extensions\simpleExtension.dll'
  end
  else do
    extensionFile = 'user.extensions/libsimpleextension.so'
  end

	dbName = 'ooFoods.rdbx'

  -- Set the result set format to an array of arrays:
  .ooSQLite~recordFormat = .ooSQLite~OO_ARRAY_OF_ARRAYS

  dbConn = .ooSQLiteConnection~new(dbName, .ooSQLite~OPEN_READWRITE)

  -- 1.) Enable extension loading:
  dbConn~enableLoadExtension(.true)

  -- 2.) Load the extension:
  ret = dbConn~loadExtension(extensionFile)
  if ret <> dbConn~ok then do
    say 'Error loading extension:' dbConn~lastErrMsg
    return dbConn~lastErrCode
  end

  sql = "SELECT * FROM foods where name like 'J%' ORDER BY name COLLATE REVERSE;"
  resultSet = dbConn~exec(sql, .true)

  say 'Use the reverse collation:'
  say 'SQL:             ' sql
  say 'Result Set:      ' resultSet
  say 'Result Set Class:' resultSet~class
  say
  say 'Hit enter to continue'
  pull
  z = printResultSet(resultSet)

  sql = "SELECT * FROM foods where name like 'J%' ORDER BY name COLLATE EBCDIC;"
  resultSet = dbConn~exec(sql, .true)

  say 'Use the EBCDIC collation:'
  say 'SQL:             ' sql
  say 'Result Set:      ' resultSet
  say 'Result Set Class:' resultSet~class
  say
  say 'Hit enter to continue'
  pull
  z = printResultSet(resultSet)

  sql = "SELECT half(11);"
  resultSet = dbConn~exec(sql, .true)

  say 'Use the half functions:'
  say 'SQL:             ' sql
  say 'Result Set:      ' resultSet
  say 'Result Set Class:' resultSet~class
  say
  say 'Hit enter to continue'
  pull
  z = printResultSet(resultSet)

  sql = "SELECT * FROM foods ORDER BY name COLLATE REVERSE;"
  resultSet = dbConn~exec(sql, .true)

  say 'Use the reverse collation:'
  say 'SQL:             ' sql
  say 'Result Set:      ' resultSet
  say 'Result Set Class:' resultSet~class
  say
  say 'Hit enter to continue'
  pull
  z = printResultSet(resultSet)

  sql = "SELECT season, strAggregate(name) from episodes group by season;"
  resultSet = dbConn~exec(sql, .true)

  say 'Use the strAggregate aggregate:'
  say 'SQL:             ' sql
  say 'Result Set:      ' resultSet
  say 'Result Set Class:' resultSet~class
  say
  say 'Hit enter to continue'
  pull
  z = printStrAgg(resultSet)
  say

  ret = dbConn~close
  return ret

::requires 'ooSQLite.cls'
::requires 'utilities.frm'

