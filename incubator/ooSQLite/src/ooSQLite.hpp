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

#ifndef ooSQLite_Included
#define ooSQLite_Included

// The range of errors needs to be contiguous and not include any SQLite error
// rc.  The next SQLite error rc is 1034.
#define OO_ERR_FIRST                     1000
#define OO_INTERNAL_ERR                  1000
#define OO_WRONG_ARG_TYPE                1001
#define OO_UNEXPECTED_RESULT             1002
#define OO_BACKUP_IN_PROGRESS            1003
#define OO_BACKUP_DB_ERRSTATE            1004
#define OO_ERR_LAST                      1004

#define OO_INTERNAL_ERR_STR              "an unexpected ooSQLite internal error occurred"
#define OO_WRONG_ARG_TYPE_STR            "an argument to a ooSQLite method or fucntion is the wrong type"
#define OO_UNEXPECTED_RESULT_STR         "a SQLite API returned a result that is not believed possible"
#define OO_BACKUP_IN_PROGRESS_STR        "ooSQLite method or function can not be invoked when backup is in progress"
#define OO_BACKUP_DB_ERRSTATE_STR        "backup not possible, source or destination database is in error state"

#define VALID_VERSION_TYPES "[O]neLine [F]ull [C]ompact [L]ibVersion [N]umber [S]ourceID"
#define RECORD_FORMATS_LIST "OO_ARRAY_OF_ARRAYS, OO_ARRAY_OF_DIRECTORIES, OO_STEM_OF_STEMS, or OO_CLASSIC_STEM"

// Buffer size must be plus one -> 4096
#define MAX_ENQUOTE_STRING  4095

// Enum for the pragma commands in SQLite3.
typedef enum
{
    autoVacuum,
    automaticIndex,
    busyTimeout,
    cacheSize,
    caseSensitiveLike,
    checkpointFullfsync,
    collationList,
    compileOptions,
    databaseList,
    encoding,
    foreignKeyList,
    foreignKeys,
    freelistCount,
    fullfsync,
    ignoreCheckConstraints,
    incrementalVacuum,
    indexInfo,
    indexList,
    integrityCheck,
    journalMode,
    journalSizeLimit,
    legacyFileFormat,
    lockingMode,
    maxPageCount,
    pageCount,
    pageSize,
    quickCheck,
    readUncommitted,
    recursiveTriggers,
    reverseUnorderedSelects,
    schemaVersion,
    secureDelete,
    shrinkMemory,
    synchronous,
    tableInfo,
    tempStore,
    userVersion,
    walAutocheckpoint,
    walCheckpoint,
    writableSchema,
    unknownPragma
} PragmaType;


// Enum for the different callback types in SQLite.
typedef enum
{
    authorizer,
    execCallbackHook,
    busyHandler,
    commitHook,
    profileHook,
    progressHandler,
    rollbackHook,
    traceHook,
    updateHook
} CallbackType;


// Enum for the different result set formats in SQLite.
typedef enum
{
    anArrayOfArrays      = 1,
    anArrayOfDirectories = 2,
    aStemOfStems         = 3,
    aClassicStem         = 4
} ResultSetType;


/* Struct for the ooSQLite class object CSelf. */
typedef struct _oosqlclassCSelf {
    ResultSetType      format;             // The default format of a result set for the current process.
} CooSQLiteClass;
typedef CooSQLiteClass *pCooSQLiteClass;

/* Struct for the ooSQLiteConnection object CSelf. */
typedef struct _oosqlConnCSelf {
    sqlite3            *db;                 // The actual database connection.
    const char         *fileName;           // The database file name.
    RexxObjectPtr       rexxSelf;           // The Rexx ooSQLiteConnection object.
    RexxObjectPtr       stmtBag;            // The bag holding prepared statments (which is really a set.)
    RexxStringObject    rxFileName;         // The Rexx string object database file name.
    RexxStringObject    lastErrMsg;         // sqlite3_errmsg()
    ResultSetType       format;             // The default format of a result set for this database.
    int                 lastErrCode;        // sqlite3_errcode()

    // Set during .ooSQLiteConnection~new(), never changed.  If not 0, database
    // operations will fail.
    int                 initCode;
    bool                closed;             // The database was closed.
    bool                isDestinationBU;    // The database is the destination of a backup, in progress.
    bool                hasBusyHandler;     // The database has a busy handler installed.
} CooSQLiteConn;
typedef CooSQLiteConn *pCooSQLiteConn;

/* Struct for the ooSQLiteStmt object CSelf. */
typedef struct _oosqlstmtCSelf {
    sqlite3_stmt       *stmt;              // The actual statement.
    RexxObjectPtr       db;                // The Rexx database connection used with the statement.
    pCooSQLiteConn      pConn;             // The CSelf database connection used with the statement.
    RexxObjectPtr       rexxSelf;          // The Rexx ooSQLiteStmt object.
    RexxStringObject    tail;              // The tail pointer returned from SQLite.
    RexxStringObject    lastErrMsg;        // Set during .ooSQLiteStmt~new(), maybe never changed. Uses sqlite3_errmsg() for the msg
    ResultSetType       format;            // The default format of a result set for this statment.
    int                 lastErrCode;       // Set during .ooSQLiteStmt~new(), maybe never changed
    int                 initCode;          // Set during .ooSQLiteStmt~new(), never changed
    bool                initializationErr; // sqlite3_prepare_v2 failed.
    bool                finalized;         // finalize() was called, or not initalized ok.
} CooSQLiteStmt;
typedef CooSQLiteStmt *pCooSQLiteStmt;

/* Struct for the ooSQLiteBackup object CSelf. */
typedef struct _oosqlbuCSelf {
    sqlite3_backup     *backup;            // The SQLite backup handle
    RexxObjectPtr       rexxSelf;          // The Rexx ooSQLiteBackup object.
    pCooSQLiteConn      dstCSelf;          // The destination CSelf of the database connection.
    RexxObjectPtr       dstRexxSelf;       // The destination Rexx database connection.
    pCooSQLiteConn      srcCSelf;          // The source CSelf of the database connection.
    RexxObjectPtr       srcRexxSelf;       // The source Rexx database connection.
    RexxStringObject    lastErrMsg;        // Set during .ooSQLiteBackup~new() and updated after every step(), and finish()
    int                 lastErrCode;       // Set during .ooSQLiteBackup~new() and updated after every step(), and finish()
    int                 initCode;          // Set during .ooSQLiteBackup~new() and never changed.
    bool                initializationErr; // sqlite3_backup_init failed.
    bool                finished;          // Finish has been called on the back up object.
    bool                dstDbWasName;      // In new() the destination DB was a file name, not a connection object
    bool                saveDest;          // Indicates that if the destionation connection was opened, it should not be closed.
} CooSQLiteBackup;
typedef CooSQLiteBackup *pCooSQLiteBackup;

/* Struct for the ooSQLiteMutex object CSelf. */
typedef struct _oosqlmutexCSelf {
    sqlite3_mutex      *mutex;             // The SQLite mutex handle
    RexxObjectPtr       rexxSelf;          // The Rexx ooSQLiteMutex object.
    int                 type;              // The SQLite mutex type.
                                           // Should we reference count if the type is recursive?
    bool                initializationErr; // sqlite3_mutex_alloc failed.
    bool                closed;            // This mutex has been closed (free() was called on it.)
} CooSQLiteMutex;
typedef CooSQLiteMutex *pCooSQLiteMutex;

#define MutexMagic                  0xFD74CA32
#define SQLITE_MUTEX_RECURSIVE_DB   999    // Our own mutex define to indicate the mutex is the sqlite3_db_mutex()

/* Generic struct passed to several sqlite3 call back functions */
typedef struct _genericCallback {
    RexxRoutineObject    callbackRtn;     // Rexx routine to call.
    RexxObjectPtr        callbackObj;     // Rexx object to invoke the callback() method on.
    RexxObjectPtr        userData;        // A Rexx object that the user wants sent to its Rexx callback.
    RexxInstance        *interpreter;
    RexxThreadContext   *callbackContext;
    char               **indexes;
    CSTRING              callbackMethod;
    CSTRING              routineName;     // The name of the Rexx routine, needed for exception messages.
    RexxArrayObject      rsArray;         // When creating a result set and format is arrayOfArrays or arrayOfDirectories
    RexxStemObject       rsStem;          // When creating a result set and format is stemOfStems
    ResultSetType        format;          // Format of a record, array, stem, or directory.
    uint32_t             count;           // This is the record count, updated as each record is produced.
    uint32_t             countIndexes;    // This is the count of allocated strings in the indexes array.
    thread_id_t          initialThreadID;
    bool                 createRS;        // Determines if we are creating a record set or invoking a Rexx callback.
} CGenericCallback;
typedef CGenericCallback *pCGenericCallback;


#endif
