#!/usr/bin/rexx
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2013-2023 Rexx Language Association. All rights reserved.    */
/*                                                                            */
/* This program and the accompanying materials are made available under       */
/* the terms of the Common Public License v1.0 which accompanies this         */
/* distribution. A copy is also available at the following address:           */
/* https://www.oorexx.org/license.html                                         */
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
 *  builtinExtensions.rex
 *
 *  Demonstrates how to use the ooSQLite builtin extensions.
 *
 *  The builtin extensions consist of some extensions provided by SQLite in C
 *  source code.  These extensions are compiled into the ooSQLite shared library
 *  and can be used by registering any of them with a database connection, or
 *  by making any of them an automatic extension.
 *
 *  When a builtin extension is registered with a database connection, then that
 *  extension can be used for the life of the database connection.  When the
 *  database connection is closed, the builtin extension is no longer usable.
 *
 *  The registerBuiltin() method of the ooSQLiteExtensions class is used to
 *  register builtin extensions with a database connection.  The method will
 *  register a single available extension, some of the available extensions, or
 *  all of the available extensions.
 *
 *  SQLite also provides a mechanism that will automatically register a builtin
 *  extension every time a database connection is opened.  The autoBuiltin()
 *  method of the ooSQLExtensions class is used to make any, or all of the
 *  available builtin extensions automatically registered when any database
 *  connection is opened.  The resetAutoBuiltin method of the ooSQLExtensions
 *  class removes all automatic extensions.
 *
 *  The list of currently available builtin extensions is:
 *
 *    base64
 *    ieee754
 *    csv
 *    nextChar
 *    percentile
 *    regExp
 *    rot13
 *    spellFix
 *    toType
 *    wholeNumber

 *  This example demonstrate some methods related to using the builtin
 *  extensions.
 */

say "version     " .ooSQLite~version
say "libVersion  " .ooSQLite~libVersion
say "version('s')" .ooSQLite~version("s")

say
say "builtin extensions:"

mem = .ooSQLiteConnection~new(":memory:")
food = .ooSQLiteConnection~new('ooFoods.rdbx', .ooSQLite~OPEN_READONLY)
do extension over .ooSQLExtensions~listBuiltins
  say extension~name
  say extension~description
  -- register this extension with both our database connections
  -- (we could have used .ooSQLExtensions~autoBuiltin to register all of them)
  .ooSQLExtensions~registerBuiltin(mem, extension~name)
  .ooSQLExtensions~registerBuiltin(food, extension~name)
  select case extension~name
    when "base64" then call exec mem, -
      "SELECT base64('SGVsbG8gd29ybGQh');"
    when "ieee754" then call exec mem, -
      "SELECT ieee754(45.25);" --2, ieee754(45.25);"
    when "csv" then call exec mem, -
      'CREATE VIRTUAL TABLE orders USING csv(filename = "test.csv", header = 1);' .endofline || -
      'SELECT "order date",total FROM orders;'
    when "regExp" then call exec food, -
      "SELECT name FROM foods WHERE name regexp 'Th.+r';"
    when "rot13" then call exec food, -
      "SELECT name, rot13(name) FROM foods WHERE name like 'Flou%'"
    when "spellFix" then call exec mem, -
      "CREATE VIRTUAL TABLE words USING spellfix1;" .endofline || -
      "INSERT INTO words (word) VALUES ('adder'), ('alter'), ('odd'), ('other');" .endofline || -
      "SELECT word FROM words WHERE word MATCH 'odder';"
    when "toType" then call exec mem, -
      "SELECT tointeger('0123'), toreal('.0123e4');"
    when "wholeNumber" then call exec mem, -
      "CREATE VIRTUAL TABLE nums USING wholenumber; SELECT value FROM nums WHERE value < 5;"
    otherwise say "---- no test SQL for" extension~name "----"; say
  end
end

::routine exec
  use strict arg db, sql
  first = .true

  say sql
  do row over db~exec(sql, .true, .ooSQLite~OO_ARRAY_OF_ARRAYS)
    if first then -- skip result table header
      first = .false
    else
      say row~toString(, ", ")
  end
  if first, db~lastErrCode \ = 0 then
    say "**" "error code" db~lastErrCode db~lastErrMsg
  say


::requires "ooSQLite"
