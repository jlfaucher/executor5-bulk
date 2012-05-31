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
 *  insertIntoDatabas.rex
 *
 * Uses the phone book database created in the createDatabase.rex example.
 * Please execute createDatabase.rex first, then execute this example.  The
 * example will not work correctly if createDatabase.rex is not executed prior
 * to executing this program.
 *
 * This example inserts data into the various tables in the phoneBook database.
 * The various task involved in inserting the data are carried out by methods of
 * the DatabaseAssistant class.  The DatabaseAssistant class is not a part of
 * the ooSQLite extension.
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

  -- DatabaseAssistant is not a part of ooSQLite.  It is a private classe
  -- defined in this example.  It is used to make the code more modular
  dbHelper = .DatabaseAssistant~new

  db = .ooSQLiteDB~new('phoneBook.rdbx')
  if db~initCode <> 0 then return dbHelper~openDbErr(db)

  -- We set a busy handler of 3 seconds, which is more than enough for this
  -- simple program.  If we get busy timeouts with this value, it would indicate
  -- that there was an error in this program.
  db~busyTimeout(3000)

  -- This checks that the database is empty of records.
  if \ dbHelper~okayToContinue(db) then do
    db~close
    return 99
  end

  if dbHelper~addAddressTypeRecords(db) \== .ooSQL~ok then return 99

  if dbHelper~addContacts(db) \== .ooSQL~ok then return 99

  ret = db~close
  if ret == .ooSQL~busy then do
    say 'Database busy return from close.'
  end

  return 0

::requires 'ooSQLite.cls'


::class 'DatabaseAssistant'

::method addContacts
  use strict arg db

  contacts = self~getContacts

  do c over contacts
    sql = "INSERT INTO contacts (fName, mName, lName, nickname, title)"   || -
          " VALUES('"c[1]"', '"c[2]"', '"c[3]"', '"c[4]"', '"c[5]"');"

    stmt = .ooSQLiteStmt~new(db, sql)
    if stmt~initCode <> 0 then return dbHelp~stmtError(stmt, db, 'PREPARE', 'contacts')

    ret = stmt~step
    if ret <> .ooSQL~done then return dbHelper~stmtError(stmt, db, 'INSERT', 'contacts')

    stmt~finalize
  end

  return .ooSQL~ok


::method getContacts private

  contacts = .array~new(10)

  contacts[ 1] = .array~of('Tom',    'Sawyer', '',         '',           ''    )
  contacts[ 2] = .array~of('Howard', 'Martin', 'Pat',      'Howie',      'Mr.' )
  contacts[ 3] = .array~of('Mary',   'Lyle',   'Beth',     'Mary Beth',  'Mrs.')
  contacts[ 4] = .array~of('Frank',  'Lyle',   'Richard',  '',           'Mr.' )

  return contacts


::method addAddressTypeRecords
  use strict arg db

  vals = .array~of('Home', 'Mobile', 'Work', 'Business', 'Office', 'Home e-mail',         -
                   'Alternate e-mail', 'Work e-mail', 'Business e-mail', 'Office e-mail', -
                   'Business website', 'Personal website')

  sql = "INSERT INTO addr_type (type) VALUES(?1);"

  stmt = .ooSQLiteStmt~new(db, sql)
  if stmt~initCode <> 0 then return dbHelp~stmtError(stmt, db, 'PREPARE', 'addr_type')

  do i = 1 to vals~items
    ret = stmt~bindText(1, vals[i])
    if ret <> .ooSQL~ok then return dbHelper~bindError(db, stmt, ret, vals[i])

    ret = stmt~step
    if ret <> .ooSQL~done then return dbHelper~stmtError(stmt, db, 'INSERT', 'addr_type')

    ret = stmt~reset
    if ret <> .ooSQL~ok then return dbHelper~resetError(db, stmt, ret, vals[i])
  end

  ret = stmt~finalize
  if ret == .ooSQL~busy then do
    say 'Database busy after last statement finalize.' ret
    db~close
  end

  return ret



::method okayToContinue
  use strict arg db

  sql = 'select * from addr_type;'

  stmt = .ooSQLiteStmt~new(db, sql)
  if stmt~initCode <> 0 then do
    stmt~finalize
    return .false
  end

  -- If there are no records then the return should be .ooSQL~done
  ret = stmt~step
  stmt~finalize

  if ret <> .ooSQL~done then do
    say 'This example program requires the database to be created, but'
    say 'empty of records.'
    say
    say 'Please execute "createDatabase.rex" first to create the empty'
    say 'database, then run this example again.'
    say
    return .false
  end

  return .true


::method bindError
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
    say '  Error:' stmt~initCode '('stmt~errMsg')'
  end
  else if id == 'TABLE' then do
    say "Unexpected return from creating" tableName "table:"
    say "  database error:" db~lastErrCode "("db~lastErrMsg")"
  end
  else if id == 'PREPARE' then do
    say 'ooSQLiteStmt initialization error (insert into' tablenName 'table):'
    say '  Error:' stmt~initCode '('stmt~errMsg')'
  end
  else if id == 'INSERT' then do
    say 'Unexpected return from inserting record into' tablenName 'table):'
    say '  Error:' stmt~initCode '('stmt~errMsg')'
  end

  say "Aborting example program ..."

  stmt~finalize
  db~close
  return 99

::method openDbErr
  use strict arg db

  errRC  = db~lastErrCode
  errMsg = db~lastErrMsg

  say 'ooSQLiteDB initialization error:' db~initCode
  say '  Error code:' errRC '('errMsg')'

  db~close
  return 99

