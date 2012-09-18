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

::requires 'oosqlite' LIBRARY


/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\

                  Object Orientated Section

    The ooRexx classes in this section provide the object orientated interface
    to the SQLite library.


\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

/** class: ooSQLite
 * ...
 *
 */
::class 'ooSQLite' public inherit ooSQLiteConstants

::attribute recordFormat get class external "LIBRARY oosqlite oosql_getRecordFormat_atr_cls"
::attribute recordFormat set class external "LIBRARY oosqlite oosql_setRecordFormat_atr_cls"

::method init class external "LIBRARY oosqlite oosql_init_cls"                                -- Do not document, internal use only
::method compileOptionGet class external "LIBRARY oosqlite oosql_compileOptionGet_cls"
::method compileOptionUsed class external "LIBRARY oosqlite oosql_compileOptionUsed_cls"
::method complete class external "LIBRARY oosqlite oosql_complete_cls"
::method enquote class external "LIBRARY oosqlite oosql_enquote_cls"
::method libVersion class external "LIBRARY oosqlite oosql_libVersion_cls"
::method libVersionNumber class external "LIBRARY oosqlite oosql_libVersionNumber_cls"
::method memoryHighWater class external "LIBRARY oosqlite oosql_memoryHighWater_cls"
::method memoryUsed class external "LIBRARY oosqlite oosql_memoryUsed_cls"
::method releaseMemory class external "LIBRARY oosqlite oosql_releaseMemory_cls"
::method softHeapLimit64 class external "LIBRARY oosqlite oosql_softHeapLimit64_cls"
::method sourceID class external "LIBRARY oosqlite oosql_sourceID_cls"
::method sqlite3Version class external "LIBRARY oosqlite oosql_sqlite3Version_cls"
::method status class external "LIBRARY oosqlite oosql_status_cls"
::method threadSafe class external "LIBRARY oosqlite oosql_threadSafe_cls"
::method version class external "LIBRARY oosqlite oosql_version_cls"

::method test class external "LIBRARY oosqlite oosql_test_cls"                                -- Do not document, internal use only


/** class: ooSQLiteConnection
 * ...
 *
 */
::class 'ooSQLiteConnection' public inherit ooSQLiteConstants

::method init class external "LIBRARY oosqlite oosqlconn_init_cls"

::attribute backupDestination get external "LIBRARY oosqlite oosqlconn_getBackupDestination_atr"
::attribute closed get external "LIBRARY oosqlite oosqlconn_getClosed_atr"
::attribute fileName get external "LIBRARY oosqlite oosqlconn_getFileName_atr"
::attribute initCode get external "LIBRARY oosqlite oosqlconn_getInitCode_atr"
::attribute lastErrCode get external "LIBRARY oosqlite oosqlconn_getLastErrCode_atr"
::attribute lastErrMsg get external "LIBRARY oosqlite oosqlconn_getLastErrMsg_atr"
::attribute recordFormat get external "LIBRARY oosqlite oosqlconn_getRecordFormat_atr"
::attribute recordFormat set external "LIBRARY oosqlite oosqlconn_setRecordFormat_atr"

::method init external "LIBRARY oosqlite oosqlconn_init"
::method uninit external "LIBRARY oosqlite oosqlconn_uninit"

::method busyHandler external "LIBRARY oosqlite oosqlconn_busyHandler"
::method busyTimeOut external "LIBRARY oosqlite oosqlconn_busyTimeOut"
::method changes external "LIBRARY oosqlite oosqlconn_changes"
::method close external "LIBRARY oosqlite oosqlconn_close"
::method commitHook external "LIBRARY oosqlite oosqlconn_commitHook"
::method dbFileName external "LIBRARY oosqlite oosqlconn_dbFileName"
::method dbMutex external "LIBRARY oosqlite oosqlconn_dbMutex"
::method dbReadOnly external "LIBRARY oosqlite oosqlconn_dbReadOnly"
::method dbReleaseMemory external "LIBRARY oosqlite oosqlconn_dbReleaseMemory"
::method dbStatus external "LIBRARY oosqlite oosqlconn_dbStatus"
::method errCode external "LIBRARY oosqlite oosqlconn_errCode"
::method errMsg external "LIBRARY oosqlite oosqlconn_errMsg"
::method exec external "LIBRARY oosqlite oosqlconn_exec"
::method extendedErrCode external "LIBRARY oosqlite oosqlconn_errCode"
::method extendedResultCodes external "LIBRARY oosqlite oosqlconn_extendedResultCodes"
::method getAutocommit external "LIBRARY oosqlite oosqlconn_getAutocommit"
::method interrupt external "LIBRARY oosqlite oosqlconn_interrupt"
::method lastInsertRowID external "LIBRARY oosqlite oosqlconn_lastInsertRowID"
::method limit external "LIBRARY oosqlite oosqlconn_limit"
::method nextStmt external "LIBRARY oosqlite oosqlconn_nextStmt"
::method profile external "LIBRARY oosqlite oosqlconn_profile"
::method progressHandler external "LIBRARY oosqlite oosqlconn_progressHandler"
::method pragma external "LIBRARY oosqlite oosqlconn_pragma"
::method rollbackHook external "LIBRARY oosqlite oosqlconn_rollbackHook"
::method setAuthorizer external "LIBRARY oosqlite oosqlconn_setAuthorizer"
::method tableColumnMetadata external "LIBRARY oosqlite oosqlconn_tableColumnMetadata"
::method trace external "LIBRARY oosqlite oosqlconn_trace"
::method updateHook external "LIBRARY oosqlite oosqlconn_updateHook"

::method    putStmt external "LIBRARY oosqlite oosqlconn_putStmt"                    -- Internal use only
::method    delStmt external "LIBRARY oosqlite oosqlconn_delStmt"                    -- Internal use only
::attribute ooSQLiteConnectionStmtBag private                                        -- Internal use only


/** class: ooSQLiteStmt
 * ...
 *
 */
::class 'ooSQLiteStmt' public inherit ooSQLiteConstants

::method init class external "LIBRARY oosqlite oosqlstmt_init_cls"                   -- Internal use only

::attribute finalized get external "LIBRARY oosqlite oosqlstmt_getFinalized_atr"
::attribute initCode get external "LIBRARY oosqlite oosqlstmt_getInitCode_atr"
::attribute lastErrCode get external "LIBRARY oosqlite oosqlstmt_getLastErrCode_atr"
::attribute lastErrMsg get external "LIBRARY oosqlite oosqlstmt_getLastErrMsg_atr"
::attribute recordFormat get external "LIBRARY oosqlite oosqlconn_getRecordFormat_atr"
::attribute recordFormat set external "LIBRARY oosqlite oosqlconn_setRecordFormat_atr"

::method init external "LIBRARY oosqlite oosqlstmt_init"
::method uninit external "LIBRARY oosqlite oosqlstmt_uninit"

::method bindBlob external "LIBRARY oosqlite oosqlstmt_BindBlob"
::method bindDouble external "LIBRARY oosqlite oosqlstmt_BindDouble"
::method bindInt external "LIBRARY oosqlite oosqlstmt_BindInt"
::method bindInt64 external "LIBRARY oosqlite oosqlstmt_BindInt64"
::method bindNull external "LIBRARY oosqlite oosqlstmt_BindNull"
::method bindParameterCount external "LIBRARY oosqlite oosqlstmt_bindParameterCount"
::method bindParameterIndex external "LIBRARY oosqlite oosqlstmt_bindParameterIndex"
::method bindParameterName external "LIBRARY oosqlite oosqlstmt_bindParameterName"
::method bindText external "LIBRARY oosqlite oosqlstmt_BindText"
::method bindValue external "LIBRARY oosqlite oosqlstmt_BindValue"
::method bindZeroBlob external "LIBRARY oosqlite oosqlstmt_BindZeroBlob"
::method clearBindings external "LIBRARY oosqlite oosqlstmt_clearBindings"
::method columnBlob external "LIBRARY oosqlite oosqlstmt_columnBlob"
::method columnBytes external "LIBRARY oosqlite oosqlstmt_columnBytes"
::method columnCount external "LIBRARY oosqlite oosqlstmt_columnCount"
::method columnDataBaseName external "LIBRARY oosqlite oosqlstmt_columnDatabaseName"
::method columnDeclType external "LIBRARY oosqlite oosqlstmt_columnDeclType"
::method columnDouble external "LIBRARY oosqlite oosqlstmt_columnDouble"
::method columnIndex external "LIBRARY oosqlite oosqlstmt_columnIndex"
::method columnInt external "LIBRARY oosqlite oosqlstmt_columnInt"
::method columnInt64 external "LIBRARY oosqlite oosqlstmt_columnInt64"
::method columnName external "LIBRARY oosqlite oosqlstmt_columnName"
::method columnOriginName external "LIBRARY oosqlite oosqlstmt_columnOriginName"
::method columnTableName external "LIBRARY oosqlite oosqlstmt_columnTableName"
::method columnText external "LIBRARY oosqlite oosqlstmt_columnText"
::method columnType external "LIBRARY oosqlite oosqlstmt_columnType"
::method columnValue external "LIBRARY oosqlite oosqlstmt_columnValue"
::method dataCount external "LIBRARY oosqlite oosqlstmt_dataCount"
::method dbHandle external "LIBRARY oosqlite oosqlstmt_dbHandle"
::method finalize external "LIBRARY oosqlite oosqlstmt_finalize"
::method reset external "LIBRARY oosqlite oosqlstmt_reset"
::method step external "LIBRARY oosqlite oosqlstmt_step"
::method stmtBusy external "LIBRARY oosqlite oosqlstmt_stmtBusy"
::method stmtReadonly external "LIBRARY oosqlite oosqlstmt_stmtReadonly"
::method stmtStatus external "LIBRARY oosqlite oosqlstmt_stmtStatus"
::method value external "LIBRARY oosqlite oosqlstmt_value"


/** class: ooSQLiteBackup
 * ...
 *
 */
::class 'ooSQLiteBackup' public inherit ooSQLiteConstants

::attribute finished get external "LIBRARY oosqlite oosqlbu_getFinished_atr"
::attribute initCode get external "LIBRARY oosqlite oosqlbu_getInitCode_atr"
::attribute lastErrCode get external "LIBRARY oosqlite oosqlbu_getLastErrCode_atr"
::attribute lastErrMsg get external "LIBRARY oosqlite oosqlbu_getLastErrMsg_atr"
::attribute pageCount get unguarded external "LIBRARY oosqlite oosqlbu_getPageCount_atr"
::attribute remaining get unguarded external "LIBRARY oosqlite oosqlbu_getRemaining_atr"
::attribute saveDestConn get unguarded external "LIBRARY oosqlite oosqlbu_getSaveDestConn_atr"
::attribute saveDestConn set unguarded external "LIBRARY oosqlite oosqlbu_setSaveDestConn_atr"

::method init external "LIBRARY oosqlite oosqlbu_init"
::method uninit external "LIBRARY oosqlite oosqlbu_uninit"

::method finish external "LIBRARY oosqlite oosqlbu_finish"
::method getDestConn external "LIBRARY oosqlite oosqlbu_getDestConn"
::method step external "LIBRARY oosqlite oosqlbu_step"


/** class: ooSQLiteMutex
 * ...
 *
 */
::class 'ooSQLiteMutex' public inherit ooSQLiteConstants

::attribute closed get unguarded external "LIBRARY oosqlite oosqlmtx_getClosed_atr"
::attribute isNull get unguarded external "LIBRARY oosqlite oosqlmtx_getIsNull_atr"

::method init external "LIBRARY oosqlite oosqlmtx_init"
::method uninit external "LIBRARY oosqlite oosqlmtx_uninit"

::method enter unguarded external "LIBRARY oosqlite oosqlmtx_enter"
::method free unguarded external "LIBRARY oosqlite oosqlmtx_free"
::method leave unguarded external "LIBRARY oosqlite oosqlmtx_leave"
::method try unguarded external "LIBRARY oosqlite oosqlmtx_try"



/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\

                  Functional Section

    The routines in this section provide a classic Rexx interface to the
    SQLite library.  The routines are a mostly 1 to 1 interface to the
    SQLite C API.

\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

::routine ooSQLiteEnquote public external "LIBRARY oosqlite ooSQLiteEnquote_rtn"
::routine ooSQLiteMerge public external "LIBRARY oosqlite ooSQLiteMerge_rtn"
::routine ooSQLiteVersion public external "LIBRARY oosqlite ooSQLiteVersion_rtn"

::routine oosqlBackupFinish public external "LIBRARY oosqlite oosqlBackupFinish_rtn"
::routine oosqlBackupInit public external "LIBRARY oosqlite oosqlBackupInit_rtn"
::routine oosqlBackupPageCount public external "LIBRARY oosqlite oosqlBackupRemaining_rtn"
::routine oosqlBackupRemaining public external "LIBRARY oosqlite oosqlBackupPageCount_rtn"
::routine oosqlBackupStep public external "LIBRARY oosqlite oosqlBackupStep_rtn"
::routine oosqlBindBlob public external "LIBRARY oosqlite oosqlBindBlob_rtn"
::routine oosqlBindDouble public external "LIBRARY oosqlite oosqlBindDouble_rtn"
::routine oosqlBindInt public external "LIBRARY oosqlite oosqlBindInt_rtn"
::routine oosqlBindInt64 public external "LIBRARY oosqlite oosqlBindInt64_rtn"
::routine oosqlBindNull public external "LIBRARY oosqlite oosqlBindNull_rtn"
::routine oosqlBindParameterCount public external "LIBRARY oosqlite oosqlBindParameterCount_rtn"
::routine oosqlBindParameterIndex public external "LIBRARY oosqlite oosqlBindParameterIndex_rtn"
::routine oosqlBindParameterName public external "LIBRARY oosqlite oosqlBindParameterName_rtn"
::routine oosqlBindText public external "LIBRARY oosqlite oosqlBindText_rtn"
::routine oosqlBindValue public external "LIBRARY oosqlite oosqlBindValue_rtn"
::routine oosqlBindZeroBlob public external "LIBRARY oosqlite oosqlBindZeroBlob_rtn"
::routine oosqlBusyHandler public external "LIBRARY oosqlite oosqlBusyHandler_rtn"
::routine oosqlBusyTimeOut public external "LIBRARY oosqlite oosqlBusyTimeOut_rtn"
::routine oosqlChanges public external "LIBRARY oosqlite oosqlChanges_rtn"
::routine oosqlClearBindings public external "LIBRARY oosqlite oosqlClearBindings_rtn"
::routine oosqlClose public external "LIBRARY oosqlite oosqlClose_rtn"
::routine oosqlColumnBlob public external "LIBRARY oosqlite oosqlColumnBlob_rtn"
::routine oosqlColumnBytes public external "LIBRARY oosqlite oosqlColumnBytes_rtn"
::routine oosqlColumnCount public external "LIBRARY oosqlite oosqlColumnCount_rtn"
::routine oosqlColumnDatabaseName public external "LIBRARY oosqlite oosqlColumnDatabaseName_rtn"
::routine oosqlColumnDeclType public external "LIBRARY oosqlite oosqlColumnDeclType_rtn"
::routine oosqlColumnDouble public external "LIBRARY oosqlite oosqlColumnDouble_rtn"
::routine oosqlColumnIndex public external "LIBRARY oosqlite oosqlColumnIndex_rtn"
::routine oosqlColumnInt public external "LIBRARY oosqlite oosqlColumnInt_rtn"
::routine oosqlColumnInt64 public external "LIBRARY oosqlite oosqlColumnInt64_rtn"
::routine oosqlColumnName public external "LIBRARY oosqlite oosqlColumnName_rtn"
::routine oosqlColumnOriginName public external "LIBRARY oosqlite oosqlColumnOriginName_rtn"
::routine oosqlColumnTableName public external "LIBRARY oosqlite oosqlColumnTableName_rtn"
::routine oosqlColumnText public external "LIBRARY oosqlite oosqlColumnText_rtn"
::routine oosqlColumnType public external "LIBRARY oosqlite oosqlColumnType_rtn"
::routine oosqlColumnValue public external "LIBRARY oosqlite oosqlColumnValue_rtn"
::routine oosqlCommitHook public external "LIBRARY oosqlite oosqlCommitHook_rtn"
::routine oosqlCompileOptionGet public external "LIBRARY oosqlite oosqlCompileOptionGet_rtn"
::routine oosqlCompileOptionUsed public external "LIBRARY oosqlite oosqlCompileOptionUsed_rtn"
::routine oosqlComplete public external "LIBRARY oosqlite oosqlComplete_rtn"
::routine oosqlDataCount public external "LIBRARY oosqlite oosqlDataCount_rtn"
::routine oosqlDbFileName public external "LIBRARY oosqlite oosqlDbFileName_rtn"
::routine oosqlDbHandle public external "LIBRARY oosqlite oosqlDbHandle_rtn"
::routine oosqlDbMutex public external "LIBRARY oosqlite oosqlDbMutex_rtn"
::routine oosqlDbReadOnly public external "LIBRARY oosqlite oosqlDbReadOnly_rtn"
::routine oosqlDbReleaseMemory public external "LIBRARY oosqlite oosqlDbReleaseMemory_rtn"
::routine oosqlDbStatus public external "LIBRARY oosqlite oosqlDbStatus_rtn"
::routine oosqlErrCode public external "LIBRARY oosqlite oosqlErrCode_rtn"
::routine oosqlErrMsg public external "LIBRARY oosqlite oosqlErrMsg_rtn"
::routine oosqlExec public external "LIBRARY oosqlite oosqlExec_rtn"
::routine oosqlExtendedErrCode public external "LIBRARY oosqlite oosqlErrCode_rtn"
::routine oosqlExtendedResultCodes public external "LIBRARY oosqlite oosqlExtendedResultCodes_rtn"
::routine oosqlFinalize public external "LIBRARY oosqlite oosqlFinalize_rtn"
::routine oosqlGetAutocommit public external "LIBRARY oosqlite oosqlGetAutocommit_rtn"
::routine oosqlInterrupt public external "LIBRARY oosqlite oosqlInterrupt_rtn"
::routine oosqlIsHandleNull public external "LIBRARY oosqlite oosqlIsHandleNull_rtn"
::routine oosqlLastInsertRowID public external "LIBRARY oosqlite oosqlLastInsertRowID_rtn"
::routine oosqlLibVersion public external "LIBRARY oosqlite oosqlLibVersion_rtn"
::routine oosqlLibVersionNumber public external "LIBRARY oosqlite oosqlLibVersion_rtn"
::routine oosqlLimit public external "LIBRARY oosqlite oosqlLimit_rtn"
::routine oosqlMemoryHighWater public external "LIBRARY oosqlite oosqlMemoryHighWater_rtn"
::routine oosqlMemoryUsed public external "LIBRARY oosqlite oosqlMemoryUsed_rtn"
::routine oosqlMutexAlloc public external "LIBRARY oosqlite oosqlMutexAlloc_rtn"
::routine oosqlMutexEnter public external "LIBRARY oosqlite oosqlMutexEnter_rtn"
::routine oosqlMutexFree public external "LIBRARY oosqlite oosqlMutexFree_rtn"
::routine oosqlMutexLeave public external "LIBRARY oosqlite oosqlMutexLeave_rtn"
::routine oosqlMutexTry public external "LIBRARY oosqlite oosqlMutexTry_rtn"
::routine oosqlNextStmt public external "LIBRARY oosqlite oosqlNextStmt_rtn"
::routine oosqlOpen public external "LIBRARY oosqlite oosqlOpen_rtn"
::routine oosqlPrepare public external "LIBRARY oosqlite oosqlPrepare_rtn"
::routine oosqlProfile public external "LIBRARY oosqlite oosqlProfile_rtn"
::routine oosqlProgressHandler public external "LIBRARY oosqlite oosqlProgressHandler_rtn"
::routine oosqlReleaseMemory public external "LIBRARY oosqlite oosqlReleaseMemory_rtn"
::routine oosqlReset public external "LIBRARY oosqlite oosqlReset_rtn"
::routine oosqlRollbackHook public external "LIBRARY oosqlite oosqlRollbackHook_rtn"
::routine oosqlSetAuthorizer public external "LIBRARY oosqlite oosqlSetAuthorizer_rtn"
::routine oosqlSoftHeapLimit64 public external "LIBRARY oosqlite oosqlSoftHeapLimit64_rtn"
::routine oosqlSourceID public external "LIBRARY oosqlite oosqlLibVersion_rtn"
::routine oosqlSql public external "LIBRARY oosqlite oosqlSql_rtn"
::routine oosqlStatus public external "LIBRARY oosqlite oosqlStatus_rtn"
::routine oosqlStep public external "LIBRARY oosqlite oosqlStep_rtn"
::routine oosqlStmtBusy public external "LIBRARY oosqlite oosqlStmtBusy_rtn"
::routine oosqlStmtReadonly public external "LIBRARY oosqlite oosqlStmtReadonly_rtn"
::routine oosqlStmtStatus public external "LIBRARY oosqlite oosqlStmtStatus_rtn"
::routine oosqlTableColumnMetadata public external "LIBRARY oosqlite oosqlTableColumnMetadata_rtn"
::routine oosqlThreadSafe public external "LIBRARY oosqlite oosqlThreadSafe_rtn"
::routine oosqlTotalChanges public external "LIBRARY oosqlite oosqlTotalChanges_rtn"
::routine oosqlTrace public external "LIBRARY oosqlite oosqlTrace_rtn"
::routine oosqlUpdateHook public external "LIBRARY oosqlite oosqlUpdateHook_rtn"
::routine oosqlVersion public external "LIBRARY oosqlite oosqlVersion_rtn"

::routine oosqlTest public external "LIBRARY oosqlite oosqlTest_rtn"            -- Internal use only


/**
    The ooSQLiteConstants class contains all Constant values defined by the
    SQLite C API interface, except for a few marked as not to be used by
    applications.

    Each constant is named the same as the SQLITE_x constant, minus the SQLITE_
    part and is intended to be used as:  .ooSQLiteConstants~x  That is:

    .ooSQLiteConstants~OK     ->  is the value of SQLITE_OK
    .ooSQLiteConstants~ERROR  ->  is the value of SQLITE_ERROR

    Since some of these constants are flags, where several flags can be | (or'd)
    together, the merge() method is provided to do that.

    The constant values should be used with either the object orientated
    interface or the classic Rexx interface.
 */
::class 'ooSQLiteConstants' public mixinclass Object

-- TODO we need a constant value to string method, something like const2string(.ooSQLiteConstants~INTERNAL) -> "INTERNAL"
::method merge class external "LIBRARY oosqlite oosqlC_merge_cls"

/* Compile-Time Library Version Numbers */
-- These constants should be updated by the ooSQLite extension developers when
-- migrating to a newer version of SQLite.  This would allow users of the
-- extension to check that ooSQLite.cls matches the oosqlite library.
-- (oosqlite.dll / oosqlite.so depending on platform.)
--
-- Copy the values directly from the top of sqlit3.hpp.
::constant VERSION        "3.7.14"
::constant VERSION_NUMBER 3007014
::constant SOURCE_ID      "2012-09-03 15:42:36 c0d89d4a9752922f9e367362366efde4f1b06f2a"


-- OO_ prefix indicates constant is not from the SQLite library.

-- Result codes
::constant OO_INTERNAL_ERR                  1000
::constant OO_WRONG_ARG_TYPE                1001
::constant OO_UNEXPECTED_RESULT             1002
::constant OO_BACKUP_IN_PROGRESS            1003
::constant OO_BACKUP_DB_ERRSTATE            1004

-- Specifies how a result set is returned to Rexx
::constant OO_ARRAY_OF_ARRAYS               1
::constant OO_ARRAY_OF_DIRECTORIES          2
::constant OO_STEM_OF_STEMS                 3


-- Beginning of SQLite constants

::constant OK           0   /* Successful result */
/* beginning-of-error-codes */
::constant ERROR        1   /* SQL error or missing database */
::constant INTERNAL     2   /* Internal logic error in SQLite */
::constant PERM         3   /* Access permission denied */
::constant ABORT        4   /* Callback routine requested an abort */
::constant BUSY         5   /* The database file is locked */
::constant LOCKED       6   /* A table in the database is locked */
::constant NOMEM        7   /* A malloc() failed */
::constant READONLY     8   /* Attempt to write a readonly database */
::constant INTERRUPT    9   /* Operation terminated by sqlite3_interrupt()*/
::constant IOERR       10   /* Some kind of disk I/O error occurred */
::constant CORRUPT     11   /* The database disk image is malformed */
::constant NOTFOUND    12   /* Unknown opcode in sqlite3_file_control() */
::constant FULL        13   /* Insertion failed because database is full */
::constant CANTOPEN    14   /* Unable to open the database file */
::constant PROTOCOL    15   /* Database lock protocol error */
::constant EMPTY       16   /* Database is empty */
::constant SCHEMA      17   /* The database schema changed */
::constant TOOBIG      18   /* String or BLOB exceeds size limit */
::constant CONSTRAINT  19   /* Abort due to constraint violation */
::constant MISMATCH    20   /* Data type mismatch */
::constant MISUSE      21   /* Library used incorrectly */
::constant NOLFS       22   /* Uses OS features not supported on host */
::constant AUTH        23   /* Authorization denied */
::constant FORMAT      24   /* Auxiliary database format error */
::constant RANGE       25   /* 2nd parameter to sqlite3_bind out of range */
::constant NOTADB      26   /* File opened that is not a database file */
::constant ROW         100  /* sqlite3_step() has another row ready */
::constant DONE        101  /* sqlite3_step() has finished executing */
/* end-of-error-codes */


/* beginning-of-extended-error-codes */
::constant IOERR_READ               266
::constant IOERR_SHORT_READ         522
::constant IOERR_WRITE              778
::constant IOERR_FSYNC             1034
::constant IOERR_DIR_FSYNC         1290
::constant IOERR_TRUNCATE          1546
::constant IOERR_FSTAT             1802
::constant IOERR_UNLOCK            2058
::constant IOERR_RDLOCK            2314
::constant IOERR_DELETE            2570
::constant IOERR_BLOCKED           2826
::constant IOERR_NOMEM             3082
::constant IOERR_ACCESS            3338
::constant IOERR_CHECKRESERVEDLOCK 3594
::constant IOERR_LOCK              3850
::constant IOERR_CLOSE             4106
::constant IOERR_DIR_CLOSE         4362
::constant IOERR_SHMOPEN           4618
::constant IOERR_SHMSIZE           4874
::constant IOERR_SHMLOCK           5130
::constant IOERR_SHMMAP            5386
::constant IOERR_SEEK              5642
::constant LOCKED_SHAREDCACHE       262
::constant BUSY_RECOVERY            261
::constant CANTOPEN_NOTEMPDIR       270
::constant CORRUPT_VTAB             267
::constant READONLY_RECOVERY        264
::constant READONLY_CANTLOCK        520
::constant ABORT_ROLLBACK           516
/* end-of-extended-error-codes */

/* Flags For File Open Operations */
::constant OPEN_READONLY         1      /* 0x00000001  Ok for sqlite3_open_v2() */
::constant OPEN_READWRITE        2      /* 0x00000002  Ok for sqlite3_open_v2() */
::constant OPEN_CREATE           4      /* 0x00000004  Ok for sqlite3_open_v2() */
::constant OPEN_DELETEONCLOSE    8      /* 0x00000008  VFS only */
::constant OPEN_EXCLUSIVE        16     /* 0x00000010  VFS only */
::constant OPEN_AUTOPROXY        32     /* 0x00000020  VFS only */
::constant OPEN_URI              64     /* 0x00000040  Ok for sqlite3_open_v2() */
::constant OPEN_MAIN_DB          256    /* 0x00000100  VFS only */
::constant OPEN_TEMP_DB          512    /* 0x00000200  VFS only */
::constant OPEN_TRANSIENT_DB     1024   /* 0x00000400  VFS only */
::constant OPEN_MAIN_JOURNAL     2048   /* 0x00000800  VFS only */
::constant OPEN_TEMP_JOURNAL     4096   /* 0x00001000  VFS only */
::constant OPEN_SUBJOURNAL       8192   /* 0x00002000  VFS only */
::constant OPEN_MASTER_JOURNAL   16384  /* 0x00004000  VFS only */
::constant OPEN_NOMUTEX          32768  /* 0x00008000  Ok for sqlite3_open_v2() */
::constant OPEN_FULLMUTEX        65536  /* 0x00010000  Ok for sqlite3_open_v2() */
::constant OPEN_SHAREDCACHE      131072 /* 0x00020000  Ok for sqlite3_open_v2() */
::constant OPEN_PRIVATECACHE     262144 /* 0x00040000  Ok for sqlite3_open_v2() */
::constant OPEN_WAL              524288 /* 0x00080000  VFS only */


/***Authorizer Action Codes********2nd****** 3rd ************ 4th ***********/
::constant CREATE_INDEX          1   /* Index Name      Table Name      */
::constant CREATE_TABLE          2   /* Table Name      NULL            */
::constant CREATE_TEMP_INDEX     3   /* Index Name      Table Name      */
::constant CREATE_TEMP_TABLE     4   /* Table Name      NULL            */
::constant CREATE_TEMP_TRIGGER   5   /* Trigger Name    Table Name      */
::constant CREATE_TEMP_VIEW      6   /* View Name       NULL            */
::constant CREATE_TRIGGER        7   /* Trigger Name    Table Name      */
::constant CREATE_VIEW           8   /* View Name       NULL            */
::constant DELETE                9   /* Table Name      NULL            */
::constant DROP_INDEX           10   /* Index Name      Table Name      */
::constant DROP_TABLE           11   /* Table Name      NULL            */
::constant DROP_TEMP_INDEX      12   /* Index Name      Table Name      */
::constant DROP_TEMP_TABLE      13   /* Table Name      NULL            */
::constant DROP_TEMP_TRIGGER    14   /* Trigger Name    Table Name      */
::constant DROP_TEMP_VIEW       15   /* View Name       NULL            */
::constant DROP_TRIGGER         16   /* Trigger Name    Table Name      */
::constant DROP_VIEW            17   /* View Name       NULL            */
::constant INSERT               18   /* Table Name      NULL            */
::constant PRAGMA               19   /* Pragma Name     1st arg or NULL */
::constant READ                 20   /* Table Name      Column Name     */
::constant SELECT               21   /* NULL            NULL            */
::constant TRANSACTION          22   /* Operation       NULL            */
::constant UPDATE               23   /* Table Name      Column Name     */
::constant ATTACH               24   /* Filename        NULL            */
::constant DETACH               25   /* Database Name   NULL            */
::constant ALTER_TABLE          26   /* Database Name   Table Name      */
::constant REINDEX              27   /* Index Name      NULL            */
::constant ANALYZE              28   /* Table Name      NULL            */
::constant CREATE_VTABLE        29   /* Table Name      Module Name     */
::constant DROP_VTABLE          30   /* Table Name      Module Name     */
::constant FUNCTION             31   /* NULL            Function Name   */
::constant SAVEPOINT            32   /* Operation       Savepoint Name  */
--::constant COPY                  0   /* No longer used */

/* Authorizer Return Codes */
::constant DENY   1   /* Abort the SQL statement with an error */
::constant IGNORE 2   /* Don't allow access, but don't generate an error */


/* xAccess VFS method flags */
::constant ACCESS_EXISTS    0
::constant ACCESS_READWRITE 1   /* Used by PRAGMA temp_store_directory */
::constant ACCESS_READ      2   /* Unused */


/* Checkpoint operation parameters */
::constant CHECKPOINT_PASSIVE 0
::constant CHECKPOINT_FULL    1
::constant CHECKPOINT_RESTART 2


/* Configuration Options */
::constant CONFIG_SINGLETHREAD  1  /* nil */
::constant CONFIG_MULTITHREAD   2  /* nil */
::constant CONFIG_SERIALIZED    3  /* nil */
::constant CONFIG_MALLOC        4  /* sqlite3_mem_methods* */
::constant CONFIG_GETMALLOC     5  /* sqlite3_mem_methods* */
::constant CONFIG_SCRATCH       6  /* void*, int sz, int N */
::constant CONFIG_PAGECACHE     7  /* void*, int sz, int N */
::constant CONFIG_HEAP          8  /* void*, int nByte, int min */
::constant CONFIG_MEMSTATUS     9  /* boolean */
::constant CONFIG_MUTEX        10  /* sqlite3_mutex_methods* */
::constant CONFIG_GETMUTEX     11  /* sqlite3_mutex_methods* */
-- ::constant CONFIG_CHUNKALLOC 12 which is now unused. */
::constant CONFIG_LOOKASIDE    13  /* int int */
::constant CONFIG_PCACHE       14  /* no-op */
::constant CONFIG_GETPCACHE    15  /* no-op */
::constant CONFIG_LOG          16  /* xFunc, void* */
::constant CONFIG_URI          17  /* int */
-- ::constant CONFIG_PCACHE2      18  /* sqlite3_pcache_methods2*  obsolete */
-- ::constant CONFIG_GETPCACHE2   19  /* sqlite3_pcache_methods2*  obsolete */


/* Database Connection Configuration Options */
::constant DBCONFIG_LOOKASIDE       1001  /* void* int int */
::constant DBCONFIG_ENABLE_FKEY     1002  /* int int* */
::constant DBCONFIG_ENABLE_TRIGGER  1003  /* int int* */


/* Status Parameters for database connections */
::constant DBSTATUS_LOOKASIDE_USED       0
::constant DBSTATUS_CACHE_USED           1
::constant DBSTATUS_SCHEMA_USED          2
::constant DBSTATUS_STMT_USED            3
::constant DBSTATUS_LOOKASIDE_HIT        4
::constant DBSTATUS_LOOKASIDE_MISS_SIZE  5
::constant DBSTATUS_LOOKASIDE_MISS_FULL  6
::constant DBSTATUS_CACHE_HIT            7
::constant DBSTATUS_CACHE_MISS           8
::constant DBSTATUS_CACHE_WRITE          9
::constant DBSTATUS_MAX                  9   /* Largest defined DBSTATUS */


/* Standard File Control Opcodes */
::constant FCNTL_LOCKSTATE               1
::constant GET_LOCKPROXYFILE             2
::constant SET_LOCKPROXYFILE             3
::constant LAST_ERRNO                    4
::constant FCNTL_SIZE_HINT               5
::constant FCNTL_CHUNK_SIZE              6
::constant FCNTL_FILE_POINTER            7
::constant FCNTL_SYNC_OMITTED            8
::constant FCNTL_WIN32_AV_RETRY          9
::constant FCNTL_PERSIST_WAL            10
::constant FCNTL_OVERWRITE              11
::constant FCNTL_VFSNAME                12
::constant FCNTL_POWERSAFE_OVERWRITE    13
::constant FCNTL_PRAGMA                 14


/* Fundamental Datatypes */
::constant INTEGER  1
::constant FLOAT    2
::constant BLOB     4
::constant NULL     5
::constant TEXT     3


/* Device Characteristics */
::constant IOCAP_ATOMIC                 1
::constant IOCAP_ATOMIC512              2
::constant IOCAP_ATOMIC1K               4
::constant IOCAP_ATOMIC2K               8
::constant IOCAP_ATOMIC4K               16
::constant IOCAP_ATOMIC8K               32
::constant IOCAP_ATOMIC16K              64
::constant IOCAP_ATOMIC32K              128
::constant IOCAP_ATOMIC64K              256
::constant IOCAP_SAFE_APPEND            512
::constant IOCAP_SEQUENTIAL             1024
::constant IOCAP_UNDELETABLE_WHEN_OPEN  2048
::constant IOCAP_POWERSAFE_OVERWRITE    4096


/* Run-Time Limit Categories */
::constant LIMIT_LENGTH                    0
::constant LIMIT_SQL_LENGTH                1
::constant LIMIT_COLUMN                    2
::constant LIMIT_EXPR_DEPTH                3
::constant LIMIT_COMPOUND_SELECT           4
::constant LIMIT_VDBE_OP                   5
::constant LIMIT_FUNCTION_ARG              6
::constant LIMIT_ATTACHED                  7
::constant LIMIT_LIKE_PATTERN_LENGTH       8
::constant LIMIT_VARIABLE_NUMBER           9
::constant LIMIT_TRIGGER_DEPTH            10


/* File Locking Levels */
::constant LOCK_NONE          0
::constant LOCK_SHARED        1
::constant LOCK_RESERVED      2
::constant LOCK_PENDING       3
::constant LOCK_EXCLUSIVE     4


/* Mutex Types */
::constant MUTEX_FAST             0
::constant MUTEX_RECURSIVE        1
::constant MUTEX_STATIC_MASTER    2
::constant MUTEX_STATIC_MEM       3  /* sqlite3_malloc() */
-- ::constant MUTEX_STATIC_MEM2      4  /* NOT USED */
::constant MUTEX_STATIC_OPEN      4  /* sqlite3BtreeOpen() */
::constant MUTEX_STATIC_PRNG      5  /* sqlite3_random() */
::constant MUTEX_STATIC_LRU       6  /* lru page list */
-- ::constant MUTEX_STATIC_LRU2      7  /* NOT USED */
::constant MUTEX_STATIC_PMEM      7  /* sqlite3PageMalloc() */


/* Flags for the xShmLock VFS method */
::constant SHM_UNLOCK       1
::constant SHM_LOCK         2
::constant SHM_SHARED       4
::constant SHM_EXCLUSIVE    8


/* Constants Defining Special Destructor Behavior */
::constant STATIC       0
::constant TRANSIENT   -1


/* Status Parameters */
::constant STATUS_MEMORY_USED          0
::constant STATUS_PAGECACHE_USED       1
::constant STATUS_PAGECACHE_OVERFLOW   2
::constant STATUS_SCRATCH_USED         3
::constant STATUS_SCRATCH_OVERFLOW     4
::constant STATUS_MALLOC_SIZE          5
::constant STATUS_PARSER_STACK         6
::constant STATUS_PAGECACHE_SIZE       7
::constant STATUS_SCRATCH_SIZE         8
::constant STATUS_MALLOC_COUNT         9


/* Status Parameters for prepared statements */
::constant STMTSTATUS_FULLSCAN_STEP     1
::constant STMTSTATUS_SORT              2
::constant STMTSTATUS_AUTOINDEX         3


/* Synchronization Type Flags */
::constant SYNC_NORMAL                   2
::constant SYNC_FULL                     3
::constant SYNC_DATAONLY                16


/* Text Encodings */
::constant UTF8           1
::constant UTF16LE        2
::constant UTF16BE        3
::constant UTF16          4    /* Use native byte order */
::constant ANY            5    /* sqlite3_create_function only */
::constant UTF16_ALIGNED  8    /* sqlite3_create_collation only */


/* Virtual Table Configuration Options */
::constant VTAB_CONSTRAINT_SUPPORT 1


/** class: __rtn_helper_class
 *
 * This class is for internal use only.  This class may be changed or removed
 * from ooSQLite at any time.  Do not use this class in any ooRexx program.
 *
 */
::class '__rtn_helper_class'

::method init class external "LIBRARY oosqlite hlpr_init_cls"

::attribute dbCallbacks

::method init
  use arg db, buffer, callbackID

  self~dbCallbacks = .table~new

  dbObj = .__rtn_db_callback_class~new(buffer, callbackID)
  self~dbCallbacks[db~string] = dbObj

::method putDbCallback
  use arg db, buffer, callbackID

  dbObj = self~dbCallbacks[db~string]

  if dbObj == .nil then do
    dbObj = .__rtn_db_callback_class~new(buffer, callbackID)
    self~dbCallbacks[db~string] = dbObj
    return .nil
  end

  return dbObj~putCallback(buffer, callbackID)

::method delDbCallback
  use arg db, callbackID

  dbObj = self~dbCallbacks[db~string]
  if dbObj == .nil then return

  dbObj~delCallback(callbackID)

::method cleanUpDbCallbacks
  use arg db

  dbObj = self~dbCallbacks~remove(db~string)
  if dbObj == .nil then return

  dbObj~releaseAll


/** class: __rtn_db_callback_class
 *
 * This class is for internal use only.  This class may be changed or removed
 * from ooSQLite at any time.  Do not use this class in any ooRexx program.
 *
 */
::class '__rtn_db_callback_class'

::attribute callbackBuffers

::method init
  use arg buffer, callbackID

  self~callbackBuffers = .table~new
  self~callbackBuffers[callbackID] = buffer

::method putCallback
  use arg buffer, callbackID

  oldBuffer = self~callbackBuffers[callbackID]
  self~callbackBuffers[callbackID] = buffer

  return oldBuffer

::method delCallback
  use arg callbackID

  buf = self~callbackBuffers~remove(callbackID)
  if buf \== .nil then self~releaseBuffer(buf)

::method releaseAll

  indexes = self~callbackBuffers~allIndexes

  do i over indexes
    buf = self~callbackBuffers~remove(i)
    self~releaseBuffer(buf)
  end

::method releaseBuffer private external "LIBRARY oosqlite db_cb_releaseBuffer"

