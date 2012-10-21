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
 *  createDatabase.rex
 *
 * Creates an example database to store contact information.  This database is
 * then used in several of the other example programs.
 *
 * The example will not work correctly if the database file it is about to
 * create already exists.  So we first check if the file exists and try to
 * back it up by renaming it.  If this fails, we just proceed.  The example will
 * then return an error, which is part of the example.  In that case, delete the
 * database file manually to see the example work as designed.
 *
 * Note that the author has little to no previous experience with SQL and
 * relational databases.  This example is not intended to show the "best" way
 * to use SQL or to deal with a relational database in general.  It is meant to
 * help someone get started using ooSQLite.
 *
 * In the create table sql strings, .endOfLine is not needed for the SQL, but
 * rather it is used to format the output when the .schema command is used with
 * command line shell: ooSQLite3
 */

  dbFile = .File~new('phoneBook.rdbx')
  if dbFile~exists then do
    bakFile = .File~new('phoneBook-bak.rdbx')
    if \ dbFile~renameTo(bakFile) then do
      bakFile~delete
      success = dbFile~renameTo(bakFile)
      if \ success then say 'Backing up existing database file did not work'
    end
  end

  db = .ooSQLiteConnection~new('phoneBook.rdbx')

  if db~initCode <> 0 then return .ErrorPrint~openDb(db)

  say 'Created database: phoneBook'

  -- Create table contacts
  sql = "CREATE TABLE contacts ("                           || .endOfLine || -
        "  id INTEGER PRIMARY KEY,"                         || .endOfLine || -
        "  lName TEXT NOT NULL,"                            || .endOfLine || -
        "  fName TEXT NOT NULL DEFAULT '',"                 || .endOfLine || -
        "  mName TEXT NOT NULL DEFAULT '',"                 || .endOfLine || -
        "  nickName TEXT NOT NULL DEFAULT '',"              || .endOfLine || -
        "  title TEXT NOT NULL DEFAULT 'Ms.'"               || .endOfLine || -
        ");"

  stmt = .ooSQLiteStmt~new(db, sql)
  if stmt~initCode <> 0 then return .ErrorPrint~stmtError(stmt, db, 'INIT', 'contacts')

  ret = stmt~step
  if ret <> stmt~DONE then return .ErrorPrint~stmtError(stmt, db, 'TABLE', 'contacts')

  stmt~finalize

  say 'Added TABLE: contacts    to database: phoneBook'

  -- Create table address types
  sql = "CREATE TABLE addr_type ("                          || .endOfLine || -
        "  id INTEGER PRIMARY KEY,"                         || .endOfLine || -
        "  type TEXT NOT NULL DEFAULT 'Home'"               || .endOfLine || -
        ");"

  stmt = .ooSQLiteStmt~new(db, sql)
  if stmt~initCode <> 0 then return .ErrorPrint~stmtError(stmt, db, 'INIT', 'addr_type')

  ret = stmt~step
  if ret <> stmt~DONE then return .ErrorPrint~stmtError(stmt, db, 'TABLE', 'addr_type')

  stmt~finalize

  say 'Added TABLE: addr_type   to database: phoneBook'

  -- Create table street addresses
  sql = "CREATE TABLE street_addr ("                        || .endOfLine || -
        "  street_id INTEGER PRIMARY KEY,"                  || .endOfLine || -
        "  street1 TEXT NOT NULL,"                          || .endOfLine || -
        "  street2 TEXT NOT NULL DEFAULT '',"               || .endOfLine || -
        "  city TEXT NOT NULL DEFAULT '',"                  || .endOfLine || -
        "  state TEXT NOT NULL DEFAULT '',"                 || .endOfLine || -
        "  zip TEXT NOT NULL DEFAULT '',"                   || .endOfLine || -
        "  contact_id INTEGER NOT NULL,"                    || .endOfLine || -
        "  type_id INTEGER NOT NULL,"                       || .endOfLine || -
        "  FOREIGN KEY(contact_id) REFERENCES contacts(id)" || .endOfLine || -
        "  ON UPDATE CASCADE ON DELETE RESTRICT,"           || .endOfLine || -
        "  FOREIGN KEY(type_id) REFERENCES addr_type(id)"   || .endOfLine || -
        "  ON UPDATE CASCADE ON DELETE RESTRICT"            || .endOfLine || -
        ");"

  stmt = .ooSQLiteStmt~new(db, sql)
  if stmt~initCode <> 0 then return .ErrorPrint~stmtError(stmt, db, 'INIT', 'street_addr')

  ret = stmt~step
  if ret <> stmt~DONE then return .ErrorPrint~stmtError(stmt, db, 'TABLE', 'street_addr')

  stmt~finalize

  say 'Added TABLE: street_addr to database: phoneBook'

  -- Create table phone addresses
  sql = "CREATE TABLE phone_addr ("                         || .endOfLine || -
        "  phone_id INTEGER PRIMARY KEY,"                   || .endOfLine || -
        "  number TEXT NOT NULL,"                           || .endOfLine || -
        "  area_code TEXT NOT NULL DEFAULT '',"             || .endOfLine || -
        "  extension TEXT NOT NULL DEFAULT '',"             || .endOfLine || -
        "  contact_id INTEGER NOT NULL,"                    || .endOfLine || -
        "  type_id INTEGER NOT NULL,"                       || .endOfLine || -
        "  FOREIGN KEY(contact_id) REFERENCES contacts(id)" || .endOfLine || -
        "  ON UPDATE CASCADE ON DELETE RESTRICT,"           || .endOfLine || -
        "  FOREIGN KEY(type_id) REFERENCES addr_type(id)"   || .endOfLine || -
        "  ON UPDATE CASCADE ON DELETE RESTRICT"            || .endOfLine || -
        ");"

  stmt = .ooSQLiteStmt~new(db, sql)
  if stmt~initCode <> 0 then return .ErrorPrint~stmtError(stmt, db, 'INIT', 'phone_addr')

  ret = stmt~step
  if ret <> stmt~DONE then return .ErrorPrint~stmtError(stmt, db, 'TABLE', 'phone_addr')

  stmt~finalize

  say 'Added TABLE: phone_addr  to database: phoneBook'

  -- Create table internet addresses
  sql = "CREATE TABLE inet_addr ("                          || .endOfLine || -
        "  inet_id INTEGER PRIMARY KEY,"                    || .endOfLine || -
        "  inet_addr TEXT NOT NULL,"                        || .endOfLine || -
        "  contact_id INTEGER NOT NULL,"                    || .endOfLine || -
        "  type_id INTEGER NOT NULL,"                       || .endOfLine || -
        "  FOREIGN KEY(contact_id) REFERENCES contacts(id)" || .endOfLine || -
        "  ON UPDATE CASCADE ON DELETE RESTRICT,"           || .endOfLine || -
        "  FOREIGN KEY(type_id) REFERENCES addr_type(id)"   || .endOfLine || -
        "  ON UPDATE CASCADE ON DELETE RESTRICT"            || .endOfLine || -
        ");"

  stmt = .ooSQLiteStmt~new(db, sql)
  if stmt~initCode <> 0 then return .ErrorPrint~stmtError(stmt, db, 'INIT', 'inet_addr')

  ret = stmt~step
  if ret <> stmt~DONE then return .ErrorPrint~stmtError(stmt, db, 'TABLE', 'inet_addr')

  stmt~finalize

  say 'Added TABLE: inet_addr   to database: phoneBook'

  db~close

  say 'Finished.'
  say

  return 0

::requires 'ooSQLite.cls'


::class 'ErrorPrint'

::method stmtError class
  use strict arg stmt, db, id, tableName = ''

  if id == 'INIT' then do
    say 'ooSQLiteStmt initialization error (create' tableName 'table):'
    say '  Error:' stmt~initCode '('stmt~lastErrMsg')'
  end
  else if id == 'TABLE' then do
    say "Unexpected return from creating" tableName "table:"
    say "  database error:" db~lastErrCode "("db~lastErrMsg")"
  end

  say "Aborting example program ..."
  say

  stmt~finalize
  db~close
  return 99

::method openDb class
  use strict arg db

  errRC  = db~lastErrCode
  errMsg = db~lastErrMsg

  say 'ooSQLiteConnection initialization error:' db~initCode
  say '  Error code:' errRC '('errMsg')'
  say

  db~close
  return 99

