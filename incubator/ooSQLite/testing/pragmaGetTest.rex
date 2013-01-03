#!/usr/bin/rexx
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2012-2013 Rexx Language Association. All rights reserved.    */
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
 *  pragmaGetTest.rex
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
 *  This test issues pragmas that return a single value.
 */

  dbFile = 'ooFoods.rdbx'

  db = .ooSQLiteConnection~new(dbFile, .ooSQLite~OPEN_READWRITE)
  if db~initCode <> 0 then do
    say 'ooSQLiteConnection initialization err:' db~initCode
    say '  Error code:' db~lastErrCode '('db~lastErrMsg')'
    if db~initCode == db~CANTOPEN then do
      say '  Database file name:' db~fileName '(Is this the correct database?)'
    end
    db~close
    return 99
  end


  names = getPragmaNames()

  do n over names
    ret = db~pragma(n)
    say 'Pragma:' n':' ret
    say; say; say
  end


  db~close

::requires 'ooSQLite.cls'

/*  Originally this test used all possible pragmas, but wasn't practical to use.
    The complete list is left here as a reference.
::routine getPragmaNames

  n = .array~of( -
    "auto_vacuum",  -
    "automatic_index",  -
    "cache_size",  -
    "case_sensitive_like",  -
    "checkpoint_fullfsync",  -
    "collation_list",  -
    "compile_options",  -
    "database_list",  -
    "encoding",  -
    "foreign_key_list",  -
    "foreign_keys",  -
    "freelist_count",  -
    "fullfsync",  -
    "ignore_check_constraints",  -
    "incremental_vacuum",  -
    "index_info",  -
    "index_list",  -
    "integrity_check",  -
    "journal_mode",  -                  -- can have .
    "journal_size_limit",  -
    "legacy_file_format",  -
    "locking_mode",  -                  -- can have .
    "max_page_count",  -
    "page_count",  -
    "page_size",  -
    "quick_check",  -
    "read_uncommitted",  -
    "recursive_triggers",  -
    "reverse_unordered_selects",  -
    "schema_version",  -
    "secure_delete",  -                 -- can have .
    "shrink_memory",  -
    "synchronous",  -
    "table_info",  -
    "temp_store",  -
    "user_version",  -
    "wal_autocheckpoint",  -
    "main.wal_checkpoint",  -           -- must have .
    "writable_schema")

    return n

*/

::routine getPragmaNames
  -- This list is the pragmas that return a single value.
  n = .array~of( -
    "auto_vacuum",  -
    "automatic_index",  -
    "cache_size",  -
    "checkpoint_fullfsync",  -
    "encoding",  -
    "foreign_keys",  -
    "freelist_count",  -
    "fullfsync",  -
    "ignore_check_constraints",  -
    "journal_mode",  -
    "journal_size_limit",  -
    "legacy_file_format",  -
    "locking_mode",  -
    "max_page_count",  -
    "page_count",  -
    "page_size",  -
    "read_uncommitted",  -
    "recursive_triggers",  -
    "reverse_unordered_selects",  -
    "schema_version",  -
    "secure_delete",  -
    "synchronous",  -
    "temp_store",  -
    "user_version",  -
    "wal_autocheckpoint",  -
    "writable_schema")

    --"incremental_vacuum",  -            -- See if doc is wrong, set only ??

    return n
