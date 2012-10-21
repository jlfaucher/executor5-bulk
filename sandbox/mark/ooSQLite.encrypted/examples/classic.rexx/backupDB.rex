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
 *  backupDB.rex
 *
 * This program shows how to use the online backup feature of SQLite with the
 * classic Rexx interface to SQLite.
 *
 * We are pretending here that the source database is in use with possibly a lot
 * of heavy activity, reads and writes.
 *
 * When a write to the source database happens, the SQLite database engine
 * internally restarts the backup.  This ensure that if the backup finishes with
 * DONE the destination database contains a consistent and up-to-date snapshot
 * of the original.
 *
 * But, if the database is big and if the backup has to be restared internally,
 * it is possible that the backup will never finish.  That's what we are
 * pretending here.  The idea for an application would be, that if the backup
 * did not finish within a time limit, the database is in a period of heavier
 * than usual activity.  So the application would reschedule the backup for some
 * other time.
 *
 * In truth here, there is no other activity and the database is not big.  We
 * just make the limit too small.
 *
 * To focus on the functions used to do the actual backup, the error handling /
 * checking is very minimal.
 *
 * backupWithTimeLimit() is a local function.  All functions beginning with
 * 'oosql' are functions from the ooSQLite package.
 */

  ret1 = oosqlOpen('ooFoods.rdbx', 'srcDB')
  ret2 = oosqlOpen('ooFoodsBackup.rdbx', 'destDB')

  if ret1 \== .ooSQLite~OK | ret2 \== .ooSQLite~OK then do
    say "Error opening one or boths database connections, aborting"
    r = oosqlClose(srcDB)
    r = oosqlClose(destDB)
    return 99.
  end

  buHandle = oosqlBackupInit(destDB, srcDB)
  if oosqlIsHandleNull(buHandle) then do
    say 'Error initializing backup. Error code:' oosqlErrCode(destDB) oosqlErrMsg(destDB)
    r = oosqlClose(srcDB)
    r = oosqlClose(destDB)
    return 99.
  end

  -- Time limit in seconds. You can raise or lower the limit to see the backup
  -- finish or be abandoned.  This limit on my system only gets 96% complete,
  -- usually.  Sometimes it finishes:

  timeLimit = 21

  ret = backupWithTimeLimit(buHandle, timeLimit)
  if ret \== .ooSQLite~DONE & ret \== .ooSQLite~BUSY then do
    say 'Backup error:' oosqlErrCode(destDB) oosqlErrMsg(destDB)
  end

  r = oosqlClose(srcDB)
  r = oosqlClose(destDB)

  return ret

::requires 'ooSQLite.cls'

-- This function will return DONE if completed and BUSY if abandoned.  Any other
-- return would be a fatal error.
::routine backupWithTimeLimit
  use strict arg buHandle, limit

  count = 0
  do while .true
    ret = oosqlBackupStep(buHandle, 2)
    if ret == .ooSQLite~DONE then do
      say 'Backup finished with no error.'
      outcome = .ooSQLite~DONE
      leave
    end

    if ret <> .ooSQLite~OK, ret <> .ooSQLite~BUSY, ret <> .ooSQLite~LOCKED then do
      say 'Fatal error during back up.'
      outcome = ret
      leave
    end

    -- This section shows how to calculate the percentage complete:

    remain = oosqlBackupRemaining(buHandle)
    pages  = oosqlBackupPageCount(buHandle)
    percentComplete = 100 * (pages - remain) / pages
    say "Backup" percentComplete "percent complete..."

    -- Here we check if the backup is taking too long.  This is only approximate
    -- of course, good enough for an example

    if count * 2 > limit then do
      say
      say "Backup has not completed within the time limit, going to abandon the operation."
      say
      outcome = .ooSQLite~BUSY
      leave
    end

    j = SysSleep(.5)
    count += 1

  end

  ret = oosqlBackupFinish(buHandle)

  return outcome

