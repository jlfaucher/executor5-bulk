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
 *  collationNeeded.rex
 *
 *  Demonstrates the collation needed callback usage.
 *
 *  Rather than register each user defined collation individually, SQLite allows
 *  the registration of a 'collation needed' callback.  Each time SQLite
 *  encounters an unknown collation, if there is a registered collation needed
 *  callback, SQLite inovkes that callback.  Within the callback, the programmer
 *  would register the needed collation.
 *
 *  This example concentrates on collation needed callbacks implemented in a
 *  shared library (.ooSQLLibrary) or in a ooSQLite package (.ooSQLPackage).
 *
 *  The collationCustom.rex example shows how to implement a collation needed
 *  callback in Rexx.
 *
 *  Both the shared library and the package have 4 collations defined.  The 4
 *  collations are all basically the same, just with different names to give
 *  this example something to work with.
 *
 *  The execSQL() routine executes some SQL that uses all 4 collations.  This
 *  routine is used to show that all 4 collations are working.
 */

  os = getOSName()

  if os == 'WINDOWS' then do
    libFile    = 'user.extensions\testlibrary.dll'
    packageFile = 'user.extensions\testpackage.dll'
  end
  else do
    libFile = 'user.extensions/libtestlibrary.so'
    packageFile = 'user.extensions/libtestpackage.so'
  end

  -- Load the package file.
  success = .ooSQLExtensions~loadPackage(packageFile)
  if \ success then do
    say 'Failed to load package'
    say '  Error code:   ' .ooSQLExtensions~lastErrCode
    say '  Error message:' .ooSQLExtensions~lastErrMsg

    return .ooSQLExtensions~lastErrCode
  end

  -- Load the library file and resolve the collationNeeded callback at the
  -- same time.
  success = .ooSQLExtensions~loadLibrary(libFile, 'collationNeeded')
  if \ success then do
    say 'Failed to load library'
    say '  Error code:   ' .ooSQLExtensions~lastErrCode
    say '  Error message:' .ooSQLExtensions~lastErrMsg

    return .ooSQLExtensions~lastErrCode
  end

  .ooSQLite~recordFormat = .ooSQLite~OO_ARRAY_OF_ARRAYS

	dbName = 'ooFoods.rdbx'
  dbConn = .ooSQLiteConnection~new(dbName, .ooSQLite~OPEN_READWRITE)

  -- For a collation needed callback defined in a library, we need to get our
  -- loaded library and then get a handle for the callback from the library
  -- object.
  lib = .ooSQLExtensions~getLibrary('testlibrary')
  if lib == .nil then do
    say 'Failed to retrieve library from extensions manager'
    say '  Error code:   ' .ooSQLExtensions~lastErrCode
    say '  Error message:' .ooSQLExtensions~lastErrMsg

    return .ooSQLExtensions~lastErrCode
  end

  hCollationNeeded = lib~getHandle('collationNeeded')

  -- Register the callback with the database connection using the handle to the
  -- collation needed callback in the library.
  ret = dbConn~collationNeeded(hCollationNeeded)

  -- Execute the SQL that uses the collations.
  r = execSQL(dbConn, 'lib')

  ret = dbConn~close

  -- Open a new database connection.  This new connection will not have any
  -- of the user defined collations registered, or the collation needed callback
  -- registered.  The user defined collations and collation needed callback are
  -- only registered per database connection.
  dbConn2 = .ooSQLiteConnection~new(dbName, .ooSQLite~OPEN_READWRITE)

  -- Get our already loaded package from the .ooSQLExtensions class object.
  -- The .ooSQLExtensions class object is essentially a manager for all the
  -- builtin user defined extensions and all the user defined extensions that
  -- are implemented in external shared libraries or packages.
  package = .ooSQLExtensions~getPackage('testPackage')

  -- Get the collation needed callback from the package
  collNeededEntry = package~getCollationNeeded

  -- Register the callback wit the database connection
  ret = dbConn2~collationNeeded(collNeededEntry)

  -- Execute the SQL that uses the collations.
  r = execSQL(dbConn2, 'package')

  ret = dbConn2~close

  -- Open a new database connection.
  dbConn3 = .ooSQLiteConnection~new(dbName, .ooSQLite~OPEN_READWRITE)

  -- Register the collation needed callback.  Since there can only be one
  -- collation needed callback per package, we only need to pass in the package.
  -- The collationNeeded method knows how to extract the proper callback from
  -- the package.
  ret = dbConn3~collationNeeded(package)

  r = execSQL(dbConn3, 'package')

  ret = dbConn3~close

  -- With a ooSQLPackage, we can also make a collation needed callback an
  -- automatically registered collation needed callback.  An automatically
  -- registered collation needed callback wil be registered with every database
  -- connection that is opened.
  if \ .ooSQLExtensions~makeAutoCollationNeeded('testPackage') then do
    say 'Failed to make collation needed callback automatic.'
    say '  Error code:   ' .ooSQLExtensions~lastErrCode
    say '  Error message:' .ooSQLExtensions~lastErrMsg

    return .ooSQLExtensions~lastErrCode
  end

  say 'Made the collation needed callback an automatically registered callback.'

  dbConn4 = .ooSQLiteConnection~new(dbName, .ooSQLite~OPEN_READWRITE)

  r = execSQL(dbConn4, 'package')

  ret = dbConn4~close

  -- We can also 'un-make' a collation needed callback automatically registered
  -- by setting the second optional argument to .false:
  if \ .ooSQLExtensions~makeAutoCollationNeeded('testPackage', .false) then do
    say 'Failed to un-make collation needed callback automatic.'
    say '  Error code:   ' .ooSQLExtensions~lastErrCode
    say '  Error message:' .ooSQLExtensions~lastErrMsg

    return .ooSQLExtensions~lastErrCode
  end

  say 'Made the collation needed callback not an automatically registered callback.'

  dbConn5 = .ooSQLiteConnection~new(dbName, .ooSQLite~OPEN_READWRITE)

  -- Now execute the SQL again, but since there is no collation needed callback
  -- and the collations are not registered with this connection, all the SQL
  -- will an empty result set returned.
  say 'Expect EMPTY result sets for all SQL:'
  say
  r = execSQL(dbConn5, 'package')

  ret = dbConn5~close
  return ret

::requires 'ooSQLite.cls'
::requires 'utilities.frm'

::routine execSQL
  use strict arg db, type

  -- The collation names registered in the libray are slightly different than
  -- in the package, for demonstration.  But, it means we need to do a little
  -- extra here.
  if type == 'lib' then do
    names = .array~of('reverse', 'ebcdic', 'reverseTwo', 'halfEbcdic')
  end
  else do
    names = .array~of('reverse', 'ebcdic', 'reverseOrder', 'fullEbcdic')
  end

  sql = "SELECT * FROM foods where name like 'J%' ORDER BY name COLLATE "names[1]";"
  resultSet = db~exec(sql, .true)

  say 'SQL: ' sql
  say
  z = printResultSet(resultSet)

  sql = "SELECT * FROM foods where name like 'J%' ORDER BY name COLLATE "names[2]";"
  resultSet = db~exec(sql, .true)

  say 'SQL: ' sql
  say
  z = printResultSet(resultSet)

  sql = "SELECT * FROM foods where name like 'J%' ORDER BY name COLLATE "names[3]";"
  resultSet = db~exec(sql, .true)

  say 'SQL: ' sql
  say
  z = printResultSet(resultSet)

  sql = "SELECT * FROM foods where name like 'J%' ORDER BY name COLLATE "names[4]";"
  resultSet = db~exec(sql, .true)

  say 'SQL: ' sql
  say
  z = printResultSet(resultSet)

  say 'Hit enter to continue'
  pull
  say

  return z
