#!/usr/bin/rexx
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2012-2023 Rexx Language Association. All rights reserved.    */
/*                                                                            */
/* This program and the accompanying materials are made available under       */
/* the terms of the Common Public License v1.0 which accompanies this         */
/* distribution. A copy is also available at the following address:           */
/* httpa://www.oorexx.org/license.html                                        */
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
 *  insertIntoDatabas.rex
 *
 * Uses the phone book database created in the createDatabase.rex example.
 * Please execute createDatabase.rex first, then execute this example.  The
 * example will not work correctly if createDatabase.rex is not executed prior
 * to executing this program.
 *
 * This example inserts data into the various tables in the phoneBook database.
 * The various task involved in inserting the data are carried out by methods of
 * the ProgramAssistant class.  The ProgramAssistant class is not a part of
 * the ooSQLite extension.
 *
 * Note that the author has little to no previous experience with SQL and
 * relational databases.  This example is not intended to show the "best" way
 * to use SQL or to deal with a relational database in general.  It is meant to
 * help someone get started using ooSQLite.
 *
 * A note on SQL Injection Flaws
 *
 * SQL Injection flaws are introduced when software developers create dynamic
 * database queries that include user supplied input.
 *
 * In this program dynamic SQL INSERT statements are constructed, however, the
 * input to the construction does not come from user input.  The input comes
 * from within this program.  The input should be trustworthy in this case.  Do
 * not blindly copy code from this example without giving some thought to SQL
 * Injection flaws.
 */

  dbFile = .File~new('phoneBook.rdbx')
  if \ dbFile~exists then do
    say 'This example program requires that the phoneBook.rdbx database'
    say 'exists, and it can not be found.'
    say
    say 'First execute the "createDatabase.rex" example program.  This'
    say 'will create the required database.  Then try this example.'
    say
    return 99
  end

  -- ProgramAssistant is not a part of ooSQLite.  It is a private class defined
  -- in this example.  It is used to make the code more modular.  The
  -- ProgramAssistant has 6 main methods. 1 method to check that the database is
  -- empty of records and 1 method to insert the records into each of the 5
  -- tables of the database.
  helper = .ProgramAssistant~new

  db = .ooSQLiteConnection~new('phoneBook.rdbx')
  if db~initCode <> 0 then return helper~openDbErr(db)

  -- We set a busy handler of 3 seconds, which is more than enough for this
  -- simple program.  If we get busy timeouts with this value, it would indicate
  -- that there was an error in this program.
  db~busyTimeout(3000)

  -- This checks that the database is empty of records.
  if \ helper~okayToContinue(db) then do
    db~close
    return 99
  end

  if helper~addContacts(db) \== db~OK then return 99

  if helper~addAddressTypes(db) \== db~OK then return 99

  if helper~addStreetAddresses(db) \== db~OK then return 99

  if helper~addPhones(db) \== db~OK then return 99

  if helper~addInternet(db) \== db~OK then return 99

  ret = db~close
  if ret == db~BUSY then do
    say 'Database busy return from close.'
  end
  say

  say 'The phoneBook database is now populated with records.'
  say

  return 0

::requires 'ooSQLite.cls'


::class 'ProgramAssistant'

/* Simple method to print an error message if there is an error trying to open
 * the database.
 */
::method openDbErr
  use strict arg db

  errRC  = db~lastErrCode
  errMsg = db~lastErrMsg

  say 'ooSQLiteConnection initialization error:' db~initCode
  say '  Error code:' errRC '('errMsg')'

  db~close
  return 99


/* Checks that the database is empty, that there are no records in it yet.
 * Inserting records when they already exist causes an error. To run this
 * program properly, createDatabase.rex needs to be run first.
 */
::method okayToContinue
  use strict arg db

  sql = 'select * from addr_type;'

  stmt = .ooSQLiteStmt~new(db, sql)
  if stmt~initCode <> 0 then do
    stmt~finalize
    return .false
  end

  -- If there are no records then the return should be db~DONE
  ret = stmt~step
  stmt~finalize

  if ret <> db~DONE then do
    say 'This example program requires the database to be created, but'
    say 'empty of records.'
    say
    say 'Please execute "createDatabase.rex" first to create the empty'
    say 'database, then run this example again.'
    say
    return .false
  end

  return .true


/* Adds the initial records to the contacts table */
::method addContacts
  use strict arg db

  contacts = self~getContacts

  do c over contacts
    sql = "INSERT INTO contacts (fName, lName, mName, nickname, title)"   || -
          " VALUES('"c[1]"', '"c[2]"', '"c[3]"', '"c[4]"', '"c[5]"');"

    stmt = .ooSQLiteStmt~new(db, sql)
    if stmt~initCode <> 0 then return self~stmtError(stmt, db, 'PREPARE', 'contacts')

    ret = stmt~step
    if ret <> db~DONE then return self~stmtError(stmt, db, 'INSERT', 'contacts')

    stmt~finalize
  end

  say 'Added contact records'

  return db~OK


/* Adds the initial records to the addr_type table */
::method addAddressTypes
  use strict arg db

  vals = .array~of('Home', 'Mobile', 'Work', 'Business', 'Office', 'Home e-mail',         -
                   'Alternate e-mail', 'Work e-mail', 'Business e-mail', 'Office e-mail', -
                   'Business website', 'Personal website')

  sql = "INSERT INTO addr_type (type) VALUES(?1);"

  stmt = .ooSQLiteStmt~new(db, sql)
  if stmt~initCode <> 0 then return self~stmtError(stmt, db, 'PREPARE', 'addr_type')

  do i = 1 to vals~items
    ret = stmt~bindText(1, vals[i])
    if ret <> db~OK then return self~bindError(db, stmt, ret, vals[i])

    ret = stmt~step
    if ret <> db~DONE then return self~stmtError(stmt, db, 'INSERT', 'addr_type')

    ret = stmt~reset
    if ret <> db~OK then return self~resetError(db, stmt, ret, vals[i])
  end

  ret = stmt~finalize
  if ret == db~BUSY then do
    say 'Database busy after last statement finalize.' ret
    db~close
  end

  say 'Added address type records'

  return ret


/* Adds the initial records to the street_addr table */
::method addStreetAddresses
  use strict arg db

  sql = "INSERT INTO street_addr (street1, street2, city, state, zip, zip_ext, country, contact_id, type_id)" -
        "VALUES(?1, ?2, ?3, ?4, ?5, ?6, ?7, ?8, ?9);"

  stmt = .ooSQLiteStmt~new(db, sql)
  if stmt~initCode <> 0 then return self~stmtError(stmt, db, 'PREPARE', 'street_addr')

  -- Tom Sawyer
  vals = .Array~of('61 Mud St', '', 'St. Petersburg', 'Missouri', '63366', '', 'United States', 1, 1)
  ret = self~addRecord(db, stmt, vals, 9, 'street_adr')
  if ret == 99 then return ret

  vals = .Array~of('raft', '', 'Mississippi River', 'Missouri', '62264', '4598', 'United States', 1, 5)
  ret = self~addRecord(db, stmt, vals, 9, 'street_adr')
  if ret == 99 then return ret

  -- Howard Martin
  vals = .Array~of('505 Dellwood Ave', 'Apt. #23', 'Boulder', 'Colorado', '80301', '', 'United States', 2, 1)
  ret = self~addRecord(db, stmt, vals, 9, 'street_adr')
  if ret == 99 then return ret

  vals = .Array~of('928 Perl St', '', 'Boulder', 'Colorado', '80302', '2448', 'United States', 2, 3)
  ret = self~addRecord(db, stmt, vals, 9, 'street_adr')
  if ret == 99 then return ret

  -- Mary Lyle
  vals = .Array~of('4325 NE Knott St', '', 'Portland', 'Oregon', '97266', '', 'United States', 3, 1)
  ret = self~addRecord(db, stmt, vals, 9, 'street_adr')
  if ret == 99 then return ret

  vals = .Array~of('5555 N Channel Ave', '#71', 'Portland', 'Oregon', '97603', '2278', 'United States', 3, 4)
  ret = self~addRecord(db, stmt, vals, 9, 'street_adr')
  if ret == 99 then return ret

  -- Frank Lyle
  vals = .Array~of('7633 SE Garrett Dr', '', 'Portland', 'Oregon', '97222', '', 'United States', 4, 1)
  ret = self~addRecord(db, stmt, vals, 9, 'street_adr')
  if ret == 99 then return ret

  vals = .Array~of('3900 Northeast 158th Ave', '', 'Portland', 'Oregon', '97603', '7774', 'United States', 4, 3)
  ret = self~addRecord(db, stmt, vals, 9, 'street_adr')
  if ret == 99 then return ret

  stmt~finalize

  say 'Added address records'

  return 0


/* Adds the initial records to the phone_addr table */
::method addPhones
  use strict arg db

  sql = "INSERT INTO phone_addr (number, area_code, extension, contact_id, type_id)" -
        "VALUES(?1, ?2, ?3, ?4, ?5);"

  stmt = .ooSQLiteStmt~new(db, sql)
  if stmt~initCode <> 0 then return self~stmtError(stmt, db, 'PREPARE', 'phone_addr')

  -- Howard Martin
  vals = .Array~of('543-3892', '303', '', 2, 1)
  ret = self~addRecord(db, stmt, vals, 5, 'phone_adr')
  if ret == 99 then return ret

  vals = .Array~of('667-8392', '970', '', 2, 2)
  ret = self~addRecord(db, stmt, vals, 5, 'phone_adr')
  if ret == 99 then return ret

  vals = .Array~of('935-6911', '720', '', 2, 3)
  ret = self~addRecord(db, stmt, vals, 5, 'phone_adr')
  if ret == 99 then return ret

  -- Mary Lyle
  vals = .Array~of('713-2398', '503', '', 3, 1)
  ret = self~addRecord(db, stmt, vals, 5, 'phone_adr')
  if ret == 99 then return ret

  vals = .Array~of('771-6651', '503', '55', 3, 4)
  ret = self~addRecord(db, stmt, vals, 5, 'phone_adr')
  if ret == 99 then return ret

  vals = .Array~of('833-0523', '360', '', 3, 3)
  ret = self~addRecord(db, stmt, vals, 5, 'phone_adr')
  if ret == 99 then return ret

  -- Frank Lyle
  vals = .Array~of('713-2398', '503', '', 4, 1)
  ret = self~addRecord(db, stmt, vals, 5, 'phone_adr')
  if ret == 99 then return ret

  vals = .Array~of('887-4106', '360', '', 4, 2)
  ret = self~addRecord(db, stmt, vals, 5, 'phone_adr')
  if ret == 99 then return ret

  stmt~finalize

  say 'Added phone records'

  return 0


/* Adds the initial records to the inet_addr table */
::method addInternet
  use strict arg db

  sql = "INSERT INTO inet_addr (inet_addr, contact_id, type_id)" -
        "VALUES(?1, ?2, ?3);"

  stmt = .ooSQLiteStmt~new(db, sql)
  if stmt~initCode <> 0 then return self~stmtError(stmt, db, 'PREPARE', 'inet_addr')

  -- Howard Martin
  vals = .Array~of('hmartin@san.rr.com', 2, 6)
  ret = self~addRecord(db, stmt, vals, 3, 'inet_adr')
  if ret == 99 then return ret

  vals = .Array~of('hmarabcd@manta.com', 2, 3)
  ret = self~addRecord(db, stmt, vals, 3, 'inet_adr')
  if ret == 99 then return ret

  vals = .Array~of('http://www.manta.com/', 2, 11)
  ret = self~addRecord(db, stmt, vals, 3, 'inet_adr')
  if ret == 99 then return ret

  vals = .Array~of('http://www.howard.martin.name/', 2, 12)
  ret = self~addRecord(db, stmt, vals, 3, 'inet_adr')
  if ret == 99 then return ret

  -- Mary Lyle
  vals = .Array~of('marylyle@gmail.com', 3, 6)
  ret = self~addRecord(db, stmt, vals, 3, 'inet_adr')
  if ret == 99 then return ret

  -- Frank Lyle
  vals = .Array~of('franklyle@gmail.com', 4, 6)
  ret = self~addRecord(db, stmt, vals, 3, 'inet_adr')
  if ret == 99 then return ret

  stmt~finalize

  say 'Added Internet records'

  return 0


::method addRecord private
  use strict arg db, stmt, vals, count, tableName

  do i = 1 to count
    ret = stmt~bindText(i, vals[i])
    if ret <> db~OK then return self~bindError(db, stmt, ret, vals[i])
  end

  ret = stmt~step
  if ret <> db~DONE then return self~stmtError(stmt, db, 'INSERT', tableName)

  ret = stmt~reset
  if ret <> db~OK then return self~resetError(db, stmt, ret, vals[i])

  return 0

::method getContacts private

  contacts = .array~new(10)

  contacts[ 1] = .array~of('Tom',    'Sawyer', '',         '',           ''    )
  contacts[ 2] = .array~of('Howard', 'Martin', 'Pat',      'Howie',      'Mr.' )
  contacts[ 3] = .array~of('Mary',   'Lyle',   'Beth',     'Mary Beth',  'Mrs.')
  contacts[ 4] = .array~of('Frank',  'Lyle',   'Richard',  '',           'Mr.' )

  return contacts



::method bindError private
  use strict arg db, stmt, ret, val

  say 'Error binding "'val'".  RC:' ret
  say "  database error:" db~lastErrCode "("db~lastErrMsg")"
  stmt~finalize
  db~close

  return 99

::method resetError
  use strict arg db, stmt, ret, val

  say 'Error reseting statement after binding "'val'".  RC:' ret
  say "  database error:" db~lastErrCode "("db~lastErrMsg")"
  stmt~finalize
  db~close

  return 99


::method stmtError
  use strict arg stmt, db, id, tableName = ''

  if id == 'INIT' then do
    say 'ooSQLiteStmt initialization error (create' tablenName 'table):'
    say '  Error:' stmt~initCode '('stmt~lastErrMsg')'
  end
  else if id == 'TABLE' then do
    say "Unexpected return from creating" tableName "table:"
    say "  database error:" db~lastErrCode "("db~lastErrMsg")"
  end
  else if id == 'PREPARE' then do
    say 'ooSQLiteStmt initialization error (insert into' tablenName 'table):'
    say '  Error:' stmt~initCode '('stmt~lastErrMsg')'
  end
  else if id == 'INSERT' then do
    say 'Unexpected return from inserting record into' tablenName 'table):'
    say '  Error:' stmt~initCode '('stmt~lastErrMsg')'
  end

  say "Aborting example program ..."

  stmt~finalize
  db~close
  return 99

