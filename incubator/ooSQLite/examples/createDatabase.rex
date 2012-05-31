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

  db = .ooSQLiteDB~new('phoneBook.rdbx')

  if db~initCode <> 0 then return .ErrorHandler~openDb(db)

  sql = "CREATE TABLE contacts ("                   || .endOfLine || -
        "  id integer primary key,"                 || .endOfLine || -
        "  fName text not null default '',"         || .endOfLine || -
        "  lName text not null default '',"         || .endOfLine || -
        "  mName text not null default '',"         || .endOfLine || -
        "  nickName text not null default '',"      || .endOfLine || -
        "  title text not null default 'Ms.'"       || .endOfLine || -
        ");"

  stmt = .ooSQLiteStmt~new(db, sql)
  if stmt~initCode <> 0 then return .ErrorHandler~stmtError(stmt, db, 'INIT', 'contacts')

  ret = stmt~step
  if ret <> .ooSQL~done then return .ErrorHandler~stmtError(stmt, db, 'TABLE', 'contacts')

  stmt~finalize

  sql = "CREATE TABLE addresses ("              || .endOfLine || -
        "  addr_id integer primary key,"        || .endOfLine || -
        "  street1 text not null default '',"   || .endOfLine || -
        "  street2 text not null default '',"   || .endOfLine || -
        "  city text not null default '',"      || .endOfLine || -
        "  state text not null default '',"     || .endOfLine || -
        "  zip text not null default '',"       || .endOfLine || -
        "  contact_id integer not null,"        || .endOfLine || -
        "  type_id integer not null"            || .endOfLine || -
        ");"

  stmt = .ooSQLiteStmt~new(db, sql)
  if stmt~initCode <> 0 then return .ErrorHandler~stmtError(stmt, db, 'INIT', 'addresses')

  ret = stmt~step
  if ret <> .ooSQL~done then return .ErrorHandler~stmtError(stmt, db, 'TABLE', 'addresses')

  stmt~finalize

  sql = "CREATE TABLE phone_num ("                    || .endOfLine || -
        "  phone_num_id integer primary key,"         || .endOfLine || -
        "  area_code text not null default '',"       || .endOfLine || -
        "  number text not null default '',"          || .endOfLine || -
        "  extension text not null default '',"       || .endOfLine || -
        "  contact_id integer not null,"              || .endOfLine || -
        "  type_id integer not null"                  || .endOfLine || -
        ");"

  stmt = .ooSQLiteStmt~new(db, sql)
  if stmt~initCode <> 0 then return .ErrorHandler~stmtError(stmt, db, 'INIT', 'phone_num')

  ret = stmt~step
  if ret <> .ooSQL~done then return .ErrorHandler~stmtError(stmt, db, 'TABLE', 'phone_num')

  stmt~finalize

  sql = "CREATE TABLE inet_addr ("                    || .endOfLine || -
        "  inet_addr_id integer primary key,"         || .endOfLine || -
        "  inet_addr text not null default '',"       || .endOfLine || -
        "  contact_id integer not null,"              || .endOfLine || -
        "  type_id integer not null"                  || .endOfLine || -
        ");"

  stmt = .ooSQLiteStmt~new(db, sql)
  if stmt~initCode <> 0 then return .ErrorHandler~stmtError(stmt, db, 'INIT', 'inet_addr')

  ret = stmt~step
  if ret <> .ooSQL~done then return .ErrorHandler~stmtError(stmt, db, 'TABLE', 'inet_addr')

  stmt~finalize

  sql = "CREATE TABLE addr_type ("              || .endOfLine || -
        "  addr_type_id integer primary key,"   || .endOfLine || -
        "  type text not null default 'Home'"   || .endOfLine || -
        ");"

  stmt = .ooSQLiteStmt~new(db, sql)
  if stmt~initCode <> 0 then return .ErrorHandler~stmtError(stmt, db, 'INIT', 'addr_type')

  ret = stmt~step
  if ret <> .ooSQL~done then return .ErrorHandler~stmtError(stmt, db, 'TABLE', 'addr_type')

  stmt~finalize

  db~close

  return 0

::requires 'ooSQLite.cls'


::class ErrorHandler

::method stmtError class
  use strict arg stmt, db, id, tableName = ''

  if id == 'INIT' then do
    say 'ooSQLiteStmt initialization error (create' tableName 'table):'
    say '  Error:' stmt~initCode '('stmt~errMsg')'
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

  say 'ooSQLiteDB initialization error:' db~initCode
  say '  Error code:' errRC '('errMsg')'
  say

  db~close
  return 99

