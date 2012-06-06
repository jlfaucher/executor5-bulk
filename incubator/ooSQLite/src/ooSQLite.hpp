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

#ifndef ooSQLite_Included
#define ooSQLite_Included


#define OO_INTERNAL_ERR                  1000
#define OO_WRONG_ARG_TYPE                1001
#define OO_UNEXPECTED_RESULT             1002

#define VALID_VERSION_TYPES "[O]neLine [F]ull [C]ompact [L]ibVersion [N]umber [S]ourceID"


// Enum for the pragma commands in SQLite3.
typedef enum
{
    autoVacuum,
    automaticIndex,
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


/* Struct for the ooSQLiteConnection object CSelf. */
typedef struct _oosqlConnCSelf {
    sqlite3            *db;               // The actual database connection.
    const char         *fileName;         // The database file name.
    RexxObjectPtr       rexxSelf;         // The Rexx ooSQLiteConnection object.
    RexxObjectPtr       stmtBag;          // The bag holding prepared statments (which is really a set.)
    RexxStringObject    rxFileName;       // The Rexx string object database file name.
    RexxStringObject    lastErrMsg;       // sqlite3_errmsg()
    int                 lastErrCode;      // sqlite3_errcode()

    // Set during .ooSQLiteConnection~new(), never changed.  If not 0, database
    // operations will fail.
    int                 initCode;
    bool                closed;           // The database was closed.
} CooSQLiteConn;
typedef CooSQLiteConn *pCooSQLiteConn;

/* Struct for the ooSQLiteStmt object CSelf. */
typedef struct _oosqlstmtCSelf {
    sqlite3_stmt       *stmt;              // The actual statement.
    RexxObjectPtr       db;                // The Rexx database connection used with the statement.
    pCooSQLiteConn      pConn;             // The CSelf database connection used with the statement.
    RexxObjectPtr       rexxSelf;          // The Rexx ooSQLiteStmt object.
    RexxStringObject    tail;              // The tail pointer returned from SQLite.
    RexxStringObject    errMsg;            // Set during .ooSQLiteStmt~new(), never changed. Uses sqlite3_errmsg() for the msg
    int                 initCode;          // Set during .ooSQLiteStmt~new(), never changed
    bool                initializationErr; // sqlite3_prepare_v2 failed.
    bool                finalized;         // finalize() was called, or not initalized ok.
} CooSQLiteStmt;
typedef CooSQLiteStmt *pCooSQLiteStmt;

/* Struct for the ooSQLiteMutex object CSelf. */
typedef struct _oosqlmutexCSelf {
    sqlite3_mutex      *mutex;             // The SQLite mutex handle
    RexxObjectPtr       rexxSelf;          // The Rexx ooSQLiteConnection object.
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
    RexxThreadContext   *callbackContext;
    CSTRING              callbackMethod;
    CSTRING              routineName;     // The name of the Rexx routine, needed for exception messages.
    RexxArrayObject      records;         // records, count, and useArray are for execCallBack() only.
    uint32_t             count;
    bool                 useArray;
} CGenericCallback;
typedef CGenericCallback *pCGenericCallback;


#endif
