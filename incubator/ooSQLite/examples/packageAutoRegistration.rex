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
 *  packageAutoRegistration.rex
 *
 *  The .ooSQLExtensions class can be used to load an ooSQLite package file,
 *  which creates an .ooSQLPackage object.
 *
 *  .ooSQLPackage objects can be marked to be automatically registered for
 *  every database connection when it is opened.  In addtion, individual
 *  collations, functions, and aggregates in the package can also be marked to
 *  be automatically registered for every database connection.
 *
 *  This program provides an example of how the above works.  The following
 *  lists the 5 packages used by this example and the names of the collations
 *  and functions in each package.  In reality, all 5 packages are copies of
 *  each other, with only the package, function, and collation names changed.
 *
 *  autoPackage1.dll
 *    collations: reverse    ebcdic
 *    functions:  half       strAggregate
 *
 *  autoPackage2.dll
 *    collations: reversing  ascii2ebcdic
 *    functions:  halfOf     stringifyAggregate
 *
 *  autoPackage3.dll
 *    collations: reversible ebcdicSort
 *    functions:  toHalf     stringAgg
 *
 *  autoPackage4.dll
 *    collations: reverseTo  sortEbcdic
 *    functions:  aHalf      strAgg
 *
 *  autoPackage5.dll
 *    collations: reverseTo1 sortEbcdic1
 *    functions:  aHalf1     strAgg1
 *
 */

  .ooSQLite~recordFormat = .ooSQLite~OO_ARRAY_OF_ARRAYS
	dbName = 'ooFoods.rdbx'

  os = getOSName()

  if os == 'WINDOWS' then do
    packageFile1 = 'user.extensions\autoPackages\autoPackage1.dll'
    packageFile2 = 'user.extensions\autoPackages\autoPackage2.dll'
    packageFile3 = 'user.extensions\autoPackages\autoPackage3.dll'
    packageFile4 = 'user.extensions\autoPackages\autoPackage4.dll'
    packageFile5 = 'user.extensions\autoPackages\autoPackage5.dll'
  end
  else do
    packageFile1 = 'user.extensions/autoPackages/libautopackage1.so'
    packageFile2 = 'user.extensions/autoPackages/libautopackage2.so'
    packageFile3 = 'user.extensions/autoPackages/libautopackage3.so'
    packageFile4 = 'user.extensions/autoPackages/libautopackage4.so'
    packageFile5 = 'user.extensions/autoPackages/libautopackage5.so'
  end

  packages = .Array~of(packageFile1, packageFile2, packageFile3, packageFile4, packageFile5)
  errCount = 0
  do package over packages
    success = .ooSQLExtensions~loadPackage(package, , .true)
    if \ success then do
      say 'Failed to load package'
      say '  Error code:   ' .ooSQLExtensions~lastErrCode
      say '  Error message:' .ooSQLExtensions~lastErrMsg
      errCount += 1
    end
  end

  if errCount > 0 then exit 99

  -- The run test routine will execute SQL that uses all the functions,
  -- aggregates, and collations in the 5 pacages.
  ret = runTest()

  -- Unmark two of the loaded packages so that they will no longer be
  -- automatically registered with a new database connection.
  if \ .ooSQLExtensions~makeAutoPackage('autoPackage3', .false) then do
      say 'Failed to un-mark auto package'
      say '  Error code:   ' .ooSQLExtensions~lastErrCode
      say '  Error message:' .ooSQLExtensions~lastErrMsg
  end
  else do
    say 'autoPackage3 is not auto any more'
  end

  if \ .ooSQLExtensions~makeAutoPackage('autoPackage5', .false) then do
      say 'Failed to un-mark auto package'
      say '  Error code:   ' .ooSQLExtensions~lastErrCode
      say '  Error message:' .ooSQLExtensions~lastErrMsg
  end
  else do
    say 'autoPackage5 is not auto any more'
  end
  say
  say 'Hit enter to continue'
  pull

  -- This time when the SQL is executed some will produce no results because
  -- of the functions, collations, and aggregates that are not registered.
  ret = runTest()

  -- Now, for the purpose of demonstration, we go through the two packages that
  -- were unmarked and mark the individual collations and functions as
  -- automatically regestered.
  -- 
  -- For package 3
  if \ .ooSQLExtensions~makeAutoCollation('autoPackage3', 'reversible') then do
      say 'Failed to make auto collation: reversible'
      say '  Error code:   ' .ooSQLExtensions~lastErrCode
      say '  Error message:' .ooSQLExtensions~lastErrMsg
  end
  else do
    say 'reversible is now an auto collation'
  end

  if \ .ooSQLExtensions~makeAutoCollation('autoPackage3', 'ebcdicSort') then do
      say 'Failed to make auto collation: ebcdicSort'
      say '  Error code:   ' .ooSQLExtensions~lastErrCode
      say '  Error message:' .ooSQLExtensions~lastErrMsg
  end
  else do
    say 'ebcdicSort is now an auto collation'
  end

  if \ .ooSQLExtensions~makeAutoFunction('autoPackage3', 'toHalf') then do
      say 'Failed to make auto fucntion: toHalf'
      say '  Error code:   ' .ooSQLExtensions~lastErrCode
      say '  Error message:' .ooSQLExtensions~lastErrMsg
  end
  else do
    say 'toHalf is now an auto function'
  end

  if \ .ooSQLExtensions~makeAutoFunction('autoPackage3', 'stringAgg') then do
      say 'Failed to make auto function: stringAgg'
      say '  Error code:   ' .ooSQLExtensions~lastErrCode
      say '  Error message:' .ooSQLExtensions~lastErrMsg
  end
  else do
    say 'stringAgg is now an auto function'
  end

  -- For package 5
  if \ .ooSQLExtensions~makeAutoCollation('autoPackage5', 'reverseTo1') then do
      say 'Failed to make auto collation: reverseTo1'
      say '  Error code:   ' .ooSQLExtensions~lastErrCode
      say '  Error message:' .ooSQLExtensions~lastErrMsg
  end
  else do
    say 'reverseTo1 is now an auto collation'
  end

  if \ .ooSQLExtensions~makeAutoCollation('autoPackage5', 'sortEbcdic1') then do
      say 'Failed to make auto collation: sortEbcdic1'
      say '  Error code:   ' .ooSQLExtensions~lastErrCode
      say '  Error message:' .ooSQLExtensions~lastErrMsg
  end
  else do
    say 'sortEbcdic1 is now an auto collation'
  end

  if \ .ooSQLExtensions~makeAutoFunction('autoPackage5', 'aHalf1') then do
      say 'Failed to make auto collation: aHalf1'
      say '  Error code:   ' .ooSQLExtensions~lastErrCode
      say '  Error message:' .ooSQLExtensions~lastErrMsg
  end
  else do
    say 'aHalf1 is now an auto function'
  end

  if \ .ooSQLExtensions~makeAutoFunction('autoPackage5', 'strAgg1') then do
      say 'Failed to make auto function: strAgg1'
      say '  Error code:   ' .ooSQLExtensions~lastErrCode
      say '  Error message:' .ooSQLExtensions~lastErrMsg
  end
  else do
    say 'strAgg1 is now an auto function'
  end
  say
  say 'Hit enter to continue'
  pull

  ret = runTest()
                  
  -- Now unmark some of the auto collations and functions
  if \ .ooSQLExtensions~makeAutoFunction('autoPackage5', 'aHalf1', .false) then do
      say 'Failed to un-mark auto collation: aHalf1'
      say '  Error code:   ' .ooSQLExtensions~lastErrCode
      say '  Error message:' .ooSQLExtensions~lastErrMsg
  end
  else do
    say 'aHalf1 is no longer an auto function'
  end

  if \ .ooSQLExtensions~makeAutoCollation('autoPackage3', 'ebcdicSort', .false) then do
      say 'Failed to un-mark auto collation: ebcdicSort'
      say '  Error code:   ' .ooSQLExtensions~lastErrCode
      say '  Error message:' .ooSQLExtensions~lastErrMsg
  end
  else do
    say 'ebcdicSort is no longer an auto collation'
  end

  if \ .ooSQLExtensions~makeAutoFunction('autoPackage5', 'strAgg1', .false) then do
      say 'Failed to un-mark auto function: strAgg1'
      say '  Error code:   ' .ooSQLExtensions~lastErrCode
      say '  Error message:' .ooSQLExtensions~lastErrMsg
  end
  else do
    say 'strAgg1 is no longer an auto function'
  end
  say
  say 'Hit enter to continue'
  pull

  ret = runTest()
                             
  -- Un-mark all the auto collations and functions we first registered.  Some of
  -- these were un-marked above.  So, the second attempt to un-mark it will 
  -- produce an error.
  if \ .ooSQLExtensions~makeAutoCollation('autoPackage3', 'reversible', .false) then do
      say 'Failed to un-mark auto collation: reversible'
      say '  Error code:   ' .ooSQLExtensions~lastErrCode
      say '  Error message:' .ooSQLExtensions~lastErrMsg
  end
  else do
    say 'reversible is no longer an auto collation'
  end

  if \ .ooSQLExtensions~makeAutoCollation('autoPackage3', 'ebcdicSort', .false) then do
      say 'Failed to un-mark auto collation: ebcdicSort'
      say '  Error code:   ' .ooSQLExtensions~lastErrCode
      say '  Error message:' .ooSQLExtensions~lastErrMsg
  end
  else do
    say 'ebcdicSort is no longer an auto collation'
  end

  if \ .ooSQLExtensions~makeAutoFunction('autoPackage3', 'toHalf', .false) then do
      say 'Failed to un-mark auto fucntion: toHalf'
      say '  Error code:   ' .ooSQLExtensions~lastErrCode
      say '  Error message:' .ooSQLExtensions~lastErrMsg
  end
  else do
    say 'toHalf is no longer an auto function'
  end

  if \ .ooSQLExtensions~makeAutoFunction('autoPackage3', 'stringAgg', .false) then do
      say 'Failed to un-mark auto function: stringAgg'
      say '  Error code:   ' .ooSQLExtensions~lastErrCode
      say '  Error message:' .ooSQLExtensions~lastErrMsg
  end
  else do
    say 'stringAgg is no longer an auto function'
  end

  -- For package 5
  if \ .ooSQLExtensions~makeAutoCollation('autoPackage5', 'reverseTo1', .false) then do
      say 'Failed to un-mark auto collation: reverseTo1'
      say '  Error code:   ' .ooSQLExtensions~lastErrCode
      say '  Error message:' .ooSQLExtensions~lastErrMsg
  end
  else do
    say 'reverseTo1 is no longer an auto collation'
  end

  if \ .ooSQLExtensions~makeAutoCollation('autoPackage5', 'sortEbcdic1', .false) then do
      say 'Failed to un-mark auto collation: sortEbcdic1'
      say '  Error code:   ' .ooSQLExtensions~lastErrCode
      say '  Error message:' .ooSQLExtensions~lastErrMsg
  end
  else do
    say 'sortEbcdic1 is no longer an auto collation'
  end

  if \ .ooSQLExtensions~makeAutoFunction('autoPackage5', 'aHalf1', .false) then do
      say 'Failed to un-mark auto collation: aHalf1'
      say '  Error code:   ' .ooSQLExtensions~lastErrCode
      say '  Error message:' .ooSQLExtensions~lastErrMsg
  end
  else do
    say 'aHalf1 is no longer an auto function'
  end

  if \ .ooSQLExtensions~makeAutoFunction('autoPackage5', 'strAgg1', .false) then do
      say 'Failed to un-mark auto function: strAgg1'
      say '  Error code:   ' .ooSQLExtensions~lastErrCode
      say '  Error message:' .ooSQLExtensions~lastErrMsg
  end
  else do
    say 'strAgg1 is no longer an auto function'
  end
  say
  say 'Hit enter to continue'
  pull

  ret = runTest()
                  
                            

  dbConn = .ooSQLiteConnection~new(dbName, .ooSQLite~OPEN_READWRITE)

  sql = "SELECT * FROM foods where name like 'J%' ORDER BY name COLLATE EBCDIC;"
  resultSet = dbConn~exec(sql, .true)

  say 'SQL:             ' sql
  say 'Result Set:      ' resultSet
  say 'Result Set Class:' resultSet~class
  say
  say 'Hit enter to continue'
  pull
  z = printResultSet(resultSet)

  sql = "SELECT * FROM foods ORDER BY name COLLATE REVERSE;"
  resultSet = dbConn~exec(sql, .true)

  say 'SQL:             ' sql
  say 'Result Set:      ' resultSet
  say 'Result Set Class:' resultSet~class
  say
  say 'Hit enter to continue'
  pull
  z = printResultSet(resultSet)

  sql = "SELECT half(11);"
  resultSet = dbConn~exec(sql, .true)

  say 'SQL:             ' sql
  say 'Result Set:      ' resultSet
  say 'Result Set Class:' resultSet~class
  say
  say 'Hit enter to continue'
  pull
  z = printResultSet(resultSet)

  ret = dbConn~close

  -- Open a new database connection.
  dbConn2 = .ooSQLiteConnection~new(dbName, .ooSQLite~OPEN_READWRITE)

  -- This will work because every time a database connection is opened, the
  -- automatic extensions are registered on that connection.
  sql = "SELECT half(11);"
  resultSet = dbConn2~exec(sql, .true)

  say 'SQL:             ' sql
  say 'Result Set:      ' resultSet
  say 'Result Set Class:' resultSet~class
  say
  say 'Hit enter to continue'
  pull
  z = printResultSet(resultSet)

  dbConn2~close

  -- Another example with a new database connection.
  dbConn3 = .ooSQLiteConnection~new(dbName, .ooSQLite~OPEN_READWRITE)

  sql = "SELECT * FROM foods where name like 'J%' ORDER BY name COLLATE REVERSE;"
  resultSet = dbConn3~exec(sql, .true)

  say 'SQL:             ' sql
  say 'Result Set:      ' resultSet
  say 'Result Set Class:' resultSet~class
  say
  say 'Hit enter to continue'
  pull
  z = printResultSet(resultSet)

  dbConn3~close

  -- And again ...
  dbConn4 = .ooSQLiteConnection~new(dbName, .ooSQLite~OPEN_READWRITE)

  sql = "SELECT half(11);"
  resultSet = dbConn4~exec(sql, .true)

  say 'SQL:             ' sql
  say 'Result Set:      ' resultSet
  say 'Result Set Class:' resultSet~class
  say
  say 'Hit enter to continue'
  pull
  z = printResultSet(resultSet)


  sql = "SELECT season, strAggregate(name) from episodes group by season;"
  resultSet = dbConn4~exec(sql, .true)

  say 'SQL:             ' sql
  say 'Result Set:      ' resultSet
  say 'Result Set Class:' resultSet~class
  say
  say 'Hit enter to continue'
  pull
  z = printStrAgg(resultSet)
  say

  ret = dbConn4~close

  return ret


::requires 'ooSQLite.cls'
::requires 'utilities.frm'


::routine runTest

	dbName = 'ooFoods.rdbx'
  dbConn = .ooSQLiteConnection~new(dbName, .ooSQLite~OPEN_READWRITE)

  collationSQLs = .Array~of( -
      "SELECT * FROM foods where name like 'J%' ORDER BY name COLLATE REVERSE;"     , -
      "SELECT * FROM foods where name like 'J%' ORDER BY name COLLATE EBCDIC;"      , -
      "SELECT * FROM foods where name like 'J%' ORDER BY name COLLATE REVERSING;"   , -
      "SELECT * FROM foods where name like 'J%' ORDER BY name COLLATE ASCII2EBCDIC;", -
      "SELECT * FROM foods where name like 'J%' ORDER BY name COLLATE REVERSIBLE;"  , -
      "SELECT * FROM foods where name like 'J%' ORDER BY name COLLATE EBCDICSORT;"  , -
      "SELECT * FROM foods where name like 'J%' ORDER BY name COLLATE REVERSETO;"   , -
      "SELECT * FROM foods where name like 'J%' ORDER BY name COLLATE SORTEBCDIC;"  , -
      "SELECT * FROM foods where name like 'J%' ORDER BY name COLLATE REVERSETO1;"    , -
      "SELECT * FROM foods where name like 'J%' ORDER BY name COLLATE SORTEBCDIC1;" , -
      )

  halfSQLs = .Array~of( -
      "SELECT half(11);"  ,  -
      "SELECT halfOf(19);",  -
      "SELECT toHalf(21);",  -
      "SELECT aHalf(31);" ,  -
      "SELECT aHalf1(33);",  -
      )

  aggSQLs = .Array~of( -
      "SELECT season, strAggregate(name) from episodes group by season;"      ,  -
      "SELECT season, stringifyAggregate(name) from episodes group by season;",  -
      "SELECT season, stringAgg(name) from episodes group by season;"         ,  -
      "SELECT season, strAgg(name) from episodes group by season;"            ,  -
      "SELECT season, strAgg1(name) from episodes group by season;"           ,  -
      )

  do sql over collationSQLs
    resultSet = dbConn~exec(sql, .true)

    say 'SQL:             ' sql
    say 'Result Set:      ' resultSet
    say 'Result Set Class:' resultSet~class
    say
    say 'Hit enter to continue'
    pull
    z = printResultSet(resultSet)
  end

  do sql over halfSQLs
    resultSet = dbConn~exec(sql, .true)

    say 'SQL:             ' sql
    say 'Result Set:      ' resultSet
    say 'Result Set Class:' resultSet~class
    say
    say 'Hit enter to continue'
    pull
    z = printResultSet(resultSet)
  end

  do sql over aggSQLs
    resultSet = dbConn~exec(sql, .true)

    say 'SQL:             ' sql
    say 'Result Set:      ' resultSet
    say 'Result Set Class:' resultSet~class
    say
    say 'Hit enter to continue'
    pull
    z = printStrAgg(resultSet)
    say
  end

  dbConn~close

  return 0

