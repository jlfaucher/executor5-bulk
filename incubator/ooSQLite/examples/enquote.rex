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
 *  enquote.rex
 *
 * The enquoute method, or the ooSQLiteEnquote() function, are useful for
 * constructing SQL statements.
 *
 * Both the method and the function enclose the value passed to them in single
 * quotes and escape any single quote within the value passed to them.  The .nil
 * object is treated as NULL.
 *
 * To help with INSERT statements, an array of values is accepted.  Any index in
 * the array that is empty, is treated as NULL, along with the .nil object.
 *
 * The ooSQLiteEnquote() function takes a stem instead of an array and uses the
 * standard stem.0 = N stem.1 through stem.N are the values.
 *
 * Some examples below, execute the program to see the output.
 */

  val1 = "cherries"
  str  = .ooSQLite~enquote(val1)

  say 'val1:' val1
  say 'str: ' str
  say

  val2 = "It's a happy day!";
  str  = .ooSQLite~enquote(val2)

  say 'val2:' val2
  say 'str: ' str
  say

  val3 = .nil
  str  = .ooSQLite~enquote(val3)

  say 'val3:' val3
  say 'str: ' str
  say

  a = .array~of(val1, val2, val3)
  str = "INSERT INTO my_table (col1, col2, col3) VALUES(".ooSQLite~enquote(a)");"

  say 'items in array:' a[1] a[2] a[3]
  say 'str:           ' str
  say

  -- An example for multiple inserts:
  rows = .array~new
  rows[1] = .array~of('Tom'       , 'Hanks'    ,           , 'male'  )
  rows[2] = .array~of('Mike'      ,            , '555-9988', .nil    )
  rows[3] = .array~of(            , 'Hammersen', '334-8897', 'female')
  rows[4] = .array~of("Mike's Son", .nil       ,           , 'male'  )
  rows[5] = .array~of('Tiger'     , 'Son'      , '92037 '  , 'n/a'   )
  rows[6] = .array~of('Mary'      ,            ,           , .nil    )
  rows[7] = .array~of('Susan'     ,'Maloney'   , '342-8721', 'female')

  do a over rows
    str = "INSERT INTO her_table (col1, col2, col3, col4) VALUES(".ooSQLite~enquote(a)");"

    say 'Insert statement:'
    say str
    say
  end
  say; say; say

  -- Note this similar example. When using an array, the *size* of the array
  -- must match the number of columns.  Look at the output for Mary:
  say 'Watch the *Mary* row:'
  rows = .array~new
  rows[1] = .array~of('Tom'       , 'Hanks'    ,           , 'male'  )
  rows[2] = .array~of('Mike'      ,            , '555-9988', .nil    )
  rows[3] = .array~of(            , 'Hammersen', '334-8897', 'female')
  rows[4] = .array~of("Mike's Son", .nil       ,           , 'male'  )
  rows[5] = .array~of('Tiger'     , 'Son'      , '92037 '  , 'n/a'   )
  rows[6] = .array~of('Mary'      ,            ,           ,         )
  rows[7] = .array~of('Susan'     ,'Maloney'   , '342-8721', 'female')

  do a over rows
    str = "INSERT INTO her_table (col1, col2, col3, col4) VALUES(".ooSQLite~enquote(a)");"

    say 'Insert statement:'
    say str
    say
  end
  say; say; say

  -- There are 2 ways to fix the Mary row.  Either use .nil for the last empty
  -- column, or create a correctly sized array:

  say 'The *Mary* row fixed:'
  rows[6] = .array~of('Mary'      ,            ,           , .nil    )
  str = "INSERT INTO her_table (col1, col2, col3, col4) VALUES(".ooSQLite~enquote(rows[6])");"

  say 'Insert statement:'
  say str
  say; say

  say 'The *Mary* row fixed:'
  b = .array~new(4)
  b[1] = 'Mary'

  str = "INSERT INTO her_table (col1, col2, col3, col4) VALUES(".ooSQLite~enquote(b)");"

  say 'Insert statement:'
  say str
  say; say; say

  -- Finally, the same basic thing with the ooSQLiteEnquote() function:
  say "Using the ooSQLiteEnquote() function with stems:"

  r1.0 = 4; r1.1 = 'Tom'       ; r1.2 = 'Hanks'                       ; r1.4 = 'male'
  r2.0 = 4; r2.1 = 'Mike'                          ; r2.3 = '555-9988'; r2.4 = .nil
  r3.0 = 4                     ; r3.2 = 'Hammersen'; r3.3 = '334-8897'; r3.4 = 'female'
  r4.0 = 4; r4.1 = "Mike's Son"; r4.2 = .nil                          ; r4.4 = 'male'
  r5.0 = 4; r5.1 = 'Tiger'     ; r5.2 = 'Son'      ; r5.3 = '92037 '  ; r5.4 = 'n/a'
  r6.0 = 4; r6.1 = 'Mary'
  r7.0 = 4; r7.1 = 'Susan'     ; r7.2 = 'Maloney'  ; r7.3 = '342-8721'; r7.4 = 'female'

  str = "INSERT INTO her_table (col1, col2, col3, col4) VALUES("ooSQLiteEnquote(r1.)");"
  say 'Insert statement:'
  say str
  say

  str = "INSERT INTO her_table (col1, col2, col3, col4) VALUES("ooSQLiteEnquote(r2.)");"
  say 'Insert statement:'
  say str
  say

  str = "INSERT INTO her_table (col1, col2, col3, col4) VALUES("ooSQLiteEnquote(r3.)");"
  say 'Insert statement:'
  say str
  say

  str = "INSERT INTO her_table (col1, col2, col3, col4) VALUES("ooSQLiteEnquote(r4.)");"
  say 'Insert statement:'
  say str
  say

  str = "INSERT INTO her_table (col1, col2, col3, col4) VALUES("ooSQLiteEnquote(r5.)");"
  say 'Insert statement:'
  say str
  say

  str = "INSERT INTO her_table (col1, col2, col3, col4) VALUES("ooSQLiteEnquote(r6.)");"
  say 'Insert statement:'
  say str
  say

  str = "INSERT INTO her_table (col1, col2, col3, col4) VALUES("ooSQLiteEnquote(r7.)");"
  say 'Insert statement:'
  say str
  say



::requires 'ooSQLite.cls'
