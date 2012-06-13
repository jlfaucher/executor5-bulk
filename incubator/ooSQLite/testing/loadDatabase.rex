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
 *  loadDatabase.rex
 *
 * This example loads the ooFoodsrdbx database from disk into an in memory
 * database using the ooSQLiteBackup object.  It does a few things to check that
 * the in memory database is okay and then writes out the in memory database to
 * a new database on disk using the ooSQLiteBackup object.
 */


  srcConn = .ooSQLiteConnection~new("ooFoods.rdbx")

  -- 1st arg to new has to be an open database connection, and it is the source
  -- database for the backup.  The second arg can be an open database connection
  -- or a database file name.  If the second arg is not an ooSQLiteConnection
  -- object it is assumed to be a database name and the backup ojbect opens the
  -- connection for you.  This second database connection will be the
  -- destination for the backup.
  --
  -- Normally, if the backup object opens the destination database, it closes it
  -- automatically.  Normally if you need the destination connection, you would
  -- just open the connection yourself and pass it into new().  Which could have
  -- been done here.
  --
  -- But, opening the destionation database is a convenience. If the destination
  -- is an in memory database, and it is closed you have lost the backup.  So,
  -- the 3rd argument is whether to SAVE the destination or not.  If true, the
  -- open connection can be retrieved using the getDestConn() method.  That is
  -- what we do here.
  --
  -- The source database connection and the destionation database connection
  -- should have a busy handler installed.  By default there is no busy handler
  -- whe a connection is opened.  When the destination database connection is
  -- opened automatically from a database name, the backup object automatically
  -- adds a busy timeout handler of 3 seconds.  But, the backup object never
  -- changes anything in a user supplied connection, (i.e., the srcConn in this
  -- example.)  In this simple program we probably would never need a busy
  -- handler because there is no concurrent access going on.  But, it is
  -- probably a good practice to have one.

  srcConn~busyTimeout(3000)  -- 3 seconds.

  bu = .ooSQLiteBackup~new(srcConn, ":memory:", .true)
  if bu~initCode <> bu~OK then do
    say 'Error opening backup object:' bu~lastErrCode bu~lastErrMsg
    srcConn~close
    return 99
  end
  else do
    say 'No error opening backup object:' bu~lastErrCode bu~lastErrMsg
  end

  -- The arg to step is the number of pages to copy.  If it is negative all
  -- remaining pages are copied.  When the return from step is DONE, or a fatal
  -- error occurs, finish() is automatically invoked.  The return could also be
  -- BUSY or LOCKED.  These are not fatal errors and step() can be retried.  But
  -- for this example, BUSY or LOCKED is almost impossible so we don't check for
  -- that.

  ret = bu~step(-1)
  if ret == bu~DONE then do
    say 'Back up finished with no error:'  bu~lastErrCode bu~lastErrMsg
  end
  else do
    if \ bu~finished then bu~finish
    say "Error backing up:"  bu~lastErrCode bu~lastErrMsg
    srcConn~close
    return 99
  end
  say

  srcConn~close

  -- The destination connection can only be retrieved when these 3 conditions
  -- are meet:  1.) The second arg to ooSQLiteBackup::new() was a database name,
  -- not a connection object.  2.)  The saveDestConn attribute is true.  3.)
  -- The backup has had finish() called.  If all 3 conditions are not true, .nil
  -- is returned.  The 3 conditions are true here, so we can get the in memory
  -- database connection.

  memConn = bu~getDestConn

  -- Do a query to show we have good results for the database in memory.  The
  -- query is put in a function below because we use it several times.

  if queryDB(memConn) == 99 then return 99

  -- We can also use a pragma to test the integrity of the in memory database.

  memConn~recordFormat = memConn~OO_ARRAY_OF_ARRAYS
  rs = memConn~pragma('integrity_check')
  say rs[1]~at(1)
  say '='~copies(80)
  say rs[2]~at(1)
  say

  if rs[2]~at(1) \== 'ok' then do
    say 'Backup to in memory database was corrupted, quitting'
    memConn~close  -- No real need, this will just disapear when the interpreter closes.
    return 99
  end


  -- Now write the in memory database to disk using a different database name.
  bu = .ooSQLiteBackup~new(memConn, 'ooFoodsCopy.rdbx')
  if bu~initCode <> bu~OK then do
    say 'Error opening second backup object:' bu~lastErrCode bu~lastErrMsg
    memConn~close  -- No real need, this will just disapear when the interpreter closes.
    return 99
  end
  else do
    say 'No error opening second backup object:' bu~lastErrCode bu~lastErrMsg
  end

  do while .true
    ret = bu~step(2)

    select
      when ret == bu~DONE then do
        say 'Second back up finished with no error:'  bu~lastErrCode bu~lastErrMsg
        leave
      end
      when ret == bu~OK then do
        say 'Copied 2 pages from the in memory database to the disk database.'
      end
      when ret == bu~BUSY then do
        say 'Database engine could not get required file system lock.'
      end
      when ret == bu~LOCKED then do
        say 'Source database connection is being used to write to the source database'
      end
      otherwise do
        say 'Fatal error during back up:'  bu~lastErrCode bu~lastErrMsg
        leave
      end
    end
    -- End select

    j = SysSleep(.5)
  end

  -- Okay, the back up either finished or a fatal error happened.  In either
  -- case, finish() has already been called on the backup object.
  -- ooFoodsCopy.rdbx was closed automatically.

  say 'back up finished lastErrCode:' bu~lastErrCode
  say

  memConn~close

  -- If there was no error, connect to the copied database and run the same
  -- query as before.  You should see the same output as when the query was run
  -- on the in memory database.

  if bu~lastErrCode == bu~OK then do
    diskConn = .ooSQLiteConnection~new('ooFoodsCopy.rdbx')

    -- Test the integrity of the backed up copy on disk
    diskConn~recordFormat = diskConn~OO_ARRAY_OF_ARRAYS
    rs = diskConn~pragma('integrity_check')
    say rs[1]~at(1)
    say '='~copies(80)
    say rs[2]~at(1)
    say

    if rs[2]~at(1) \== 'ok' then do
      say 'Backup from in memory database to disk was corrupted, quitting'
      diskConn~close
      return 99
    end

    if queryDB(diskConn) == 99 then return 99
  end

  -- Quit
  diskConn~close
  return 0

::requires 'ooSQLite.cls'

::routine queryDB
  use strict arg dbConn

  sql = "SELECT * FROM foods WHERE name LIKE '%Bobka%';"
  stmt = .ooSQLiteStmt~new(dbConn, sql)
  if stmt~initCode <> 0 then do
    say 'ooSQLiteStmt initialization error:' stmt~initCode
    say '  Error code:' stmt~initCode '('stmt~lastErrMsg')'

    stmt~finalize
    memConn~close
    return 99
  end

  say stmt~columnName(1)~left(25) || stmt~columnName(2)~left(25) || stmt~columnName(3)~left(25)
  say '='~copies(80)

  do while stmt~step == stmt~ROW
    say stmt~columnText(1)~left(25) || stmt~columnText(2)~left(25) || stmt~columnText(3)~left(25)
  end
  say

  stmt~finalize

  return stmt~OK

