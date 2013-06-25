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
 *  execCS.rex
 *
 *  Uses the exec() method of the database connection object to query the foods
 *  table in the ooFoods.rdbx database.  Then print out the results.
 *
 *  Here the result set format is specified to be a 'classic Rexx' stem.
 *
 */

  -- Set the result set format to a stem of stems, set the representation of
  -- SQL null for this program to be '(NULL)' because we are only going to be
  -- printing the values.  Then we do not have to check for .nil
  .ooSQLite~recordFormat = .ooSQLite~OO_CLASSIC_STEM
  .ooSQLite~null         = '(NULL)'

	dbName = 'ooFoods.rdbx'

  dbConn = .ooSQLiteConnection~new(dbName, .ooSQLite~OPEN_READWRITE)

  sql = 'SELECT * FROM foods ORDER BY name;'
  resultSet = dbConn~exec(sql, .true)

  z = printResultSet(resultSet)

  say 'Result Set:      ' resultSet
  say 'Result Set Class:' resultSet~class
  say

  ret = dbConn~close

  return ret

::requires 'ooSQLite.cls'


::routine printResultSet
  use arg rs.

  say 'Count records:' rs.0

  say 'id'~left(25) || 'type_id'~left(25) || 'name'~left(25)
  say '='~copies(80)

  do i = 1 to rs.0
    say rs.i.id~left(25) || rs.i.type_id~left(25)  || rs.i.name~left(25)
  end
  say

  return 0
