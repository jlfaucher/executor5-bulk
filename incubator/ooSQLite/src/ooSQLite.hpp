
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2012-2023 Rexx Language Association. All rights reserved.    */
/*                                                                            */
/* This program and the accompanying materials are made available under       */
/* the terms of the Common Public License v1.0 which accompanies this         */
/* distribution. A copy is also available at the following address:           */
/* https://www.oorexx.org/license.html                                        */
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


#define NO_SQLITEEXT_H
#include "oosqlPackage.hpp"
#undef  NO_SQLITEEXT_H

#ifdef _WIN32
    #define NEED_DLL_MAIN
    #include "winOS.hpp"
    #undef  NEED_DLL_MAIN
#else
    #define NEED_CRIT_SECT
    #include "unixOS.hpp"
    #undef  NEED_CRIT_SECT
#endif

#include "oorexxapi.h"
#include "ooSqlSysLibrary.hpp"

// The range of errors needs to be contiguous and not include any SQLite error
// rc.  The next SQLite error rc is 1034.  Unfortunately, there is no guarentee
// that newer versions of SQLite3 won't intrude into this range, so it should be
// checked each time a new version of SQLite 3 is introduced.
#define OO_ERR_FIRST                     1000
#define OO_INTERNAL_ERR                  1000
#define OO_WRONG_ARG_TYPE                1001
#define OO_UNEXPECTED_RESULT             1002
#define OO_BACKUP_IN_PROGRESS            1003
#define OO_BACKUP_DB_ERRSTATE            1004
#define OO_NO_CSELF                      1005
#define OO_PACKAGE_NOT_VALID             1006
#define OO_NO_SUCH_PACKAGE               1007
#define OO_NO_SUCH_LIBRARY               1008
#define OO_PACKAGE_NOT_AUTOMATIC         1009
#define OO_COLLATION_NOT_AUTOMATIC       1010
#define OO_FUNCTION_NOT_AUTOMATIC        1011
#define OO_ERR_LAST                      1011

#define OO_INTERNAL_ERR_STR              "an unexpected ooSQLite internal error occurred"
#define OO_WRONG_ARG_TYPE_STR            "an argument to a ooSQLite method or fucntion is the wrong type"
#define OO_UNEXPECTED_RESULT_STR         "a SQLite API returned a result that is not believed possible"
#define OO_BACKUP_IN_PROGRESS_STR        "ooSQLite method or function can not be invoked when backup is in progress"
#define OO_BACKUP_DB_ERRSTATE_STR        "backup not possible, source or destination database is in error state"
#define OO_NO_CSELF_STR                  "internal error, failed to get %s C Self"
#define OO_PACKAGE_NOT_VALID_STR         "the current version of ooSQLite or SQLite does not meet the package requirements"
#define OO_NO_SUCH_PACKAGE_STR           "package %s is not loaded"
#define OO_NO_SUCH_LIBRARY_STR           "library %s is not loaded"
#define OO_PACKAGE_NOT_AUTOMATIC_STR     "package %s is not an automatically registered package"
#define OO_COLLATION_NOT_AUTOMATIC_STR   "collations %s is not an automatically registered collation"
#define OO_FUNCTION_NOT_AUTOMATIC_STR    "function %s is not an automatically registered function"

// Use SQLITE_MISUSE for these:
#define PACKAGE_NO_COLLATIONS_FMT        "this package (%s) contains no collations"
#define PACKAGE_NO_COLLATIONNEEDED_FMT   "this package (%s) contains no collation needed callback"
#define PACKAGE_NO_FUNCTIONS_FMT         "this package (%s) contains no functions"

#define VALID_VERSION_TYPES "[O]neLine [F]ull [C]ompact [L]ibVersion [N]umber [S]ourceID"
#define RECORD_FORMATS_LIST "OO_ARRAY_OF_ARRAYS, OO_ARRAY_OF_DIRECTORIES, OO_STEM_OF_STEMS, or OO_CLASSIC_STEM"

#define NO_LOCAL_ENVIRONMENT_MSG  "the .local environment was not found"

// Buffer size must be plus one -> 4096
#define MAX_ENQUOTE_STRING  4095

#define MAX_AUTO_PACKAGES         10
#define MAX_AUTO_FUNCTIONS        10
#define MAX_AUTO_COLLATIONS       10


// Function prototypes for the builtin extensions
BEGIN_EXTERN_C()

extern int sqlite3_base_init(sqlite3 *db, char **pzErrMsg, const sqlite3_api_routines *pApi);
extern int sqlite3_csv_init(sqlite3 *db, char **pzErrMsg, const sqlite3_api_routines *pApi);
extern int sqlite3_ieee_init(sqlite3 *db, char **pzErrMsg, const sqlite3_api_routines *pApi);
extern int sqlite3_nextchar_init(sqlite3 *db, char **pzErrMsg, const sqlite3_api_routines *pApi);
extern int sqlite3_percentile_init(sqlite3 *db, char **pzErrMsg, const sqlite3_api_routines *pApi);
extern int sqlite3_regexp_init(sqlite3 *db, char **pzErrMsg, const sqlite3_api_routines *pApi);
extern int sqlite3_rot_init(sqlite3 *db, char **pzErrMsg, const sqlite3_api_routines *pApi);
extern int sqlite3_spellfix_init(sqlite3 *db, char **pzErrMsg, const sqlite3_api_routines *pApi);
extern int sqlite3_totype_init(sqlite3 *db, char **pzErrMsg, const sqlite3_api_routines *pApi);
extern int sqlite3_wholenumber_init(sqlite3 *db, char **pzErrMsg, const sqlite3_api_routines *pApi);

END_EXTERN_C()

const fnXExtensionInit builtins[] =
{
    sqlite3_base_init,
    sqlite3_csv_init,
    sqlite3_ieee_init,
    sqlite3_nextchar_init,
    sqlite3_percentile_init,
    sqlite3_regexp_init,
    sqlite3_rot_init,
    sqlite3_spellfix_init,
    sqlite3_totype_init,
    sqlite3_wholenumber_init
};

const char *builtinNames[] =
{
    "base64",
    "csv",
    "ieee754",
    "nextChar",
    "percentile",
    "regExp",
    "rot13",
    "spellFix",
    "toType",
    "wholeNumber"
};

const char *builtinDescription[] =
{
    "Implements a base64() SQL function to convert between a blob and Base64 text.",
    "Implements a virtual table for reading CSV files.",
    "Implements SQL functions for IEEE 754 binary64 floating-point numbers.",
    "Implements the next_char(A, T, F, W, C) SQL function.",
    "Implements the percentile(Y, P) SQL function.",
    "Implements a POSIX extended regular expression matcher for UTF-8.",
    "Implements a rot13() SQL function and a rot13 collating sequence.",
    "Implements the spellfix1 virtual table that can search a vocabulary for close matches.",
    "Implements tointeger() and toreal() SQL functions.",
    "Implements the wholenumber virtual table that returns whole numbers between 1 and 4294967295."
};

#define BUILTIN_ROW_FMT  "%-11s %s" LINE_END
#define BUILTIN_DASHED_LINE \
    "---------------------------------------------------------------------------------------------------------"
#define BUILTINS_COUNT sizeof(builtins) / sizeof(fnXExtensionInit)

#define BUILTIN_NAMES                 "base64, csv, ieee754, nextChar, percentile, regExp, rot13, spellFix, toType, or wholeNumber"
#define BUILTIN_LOAD_ERR_FMT          "Error loading builtin extension %s"
#define BUILTIN_AUTO_ERR_FMT          "Failed to make builtin extension %s automatic"
#define BUILTIN_CANCEL_AUTO_ERR_FMT   "Failed to cancel builtin extension %s as automatic"
#define BUILTIN_NAME_ERR_FMT          "Argument %d, keyword must be exactly one of %s; found \"%s\""


// Enum for the pragma commands in SQLite3.
typedef enum
{
    applicationID,
    autoVacuum,
    automaticIndex,
    busyTimeout,
    cacheSize,
    cacheSpill,
    caseSensitiveLike,
    checkpointFullfsync,
    collationList,
    compileOptions,
    databaseList,
    deferForeignKeys,
    encoding,
    foreignKeyCheck,
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
    mmapSize,
    pageCount,
    pageSize,
    queryOnly,
    quickCheck,
    readUncommitted,
    recursiveTriggers,
    reverseUnorderedSelects,
    schemaVersion,
    secureDelete,
    shrinkMemory,
    softHeapLimit,
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
    collation,
    collationNeeded,
    commitHook,
    function,
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


/* Struct for the ooSQLiteConnection object CSelf. */
typedef struct _oosqlConnCSelf {
    sqlite3            *db;                 // The actual database connection.
    const char         *fileName;           // The database file name.
    RexxObjectPtr       rexxSelf;           // The Rexx ooSQLiteConnection object.
    RexxObjectPtr       nullObj;            // Default representation of SQL NULL for this connection
    CSTRING             nullStr;            // If nullObj is a Rexx string object, the string value of the object
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
    RexxObjectPtr       nullObj;           // Default representation of SQL NULL for this statement
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
    bool                saveDest;          // Indicates that if the destination connection was opened, it should not be closed.
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

/* Struct for the ooSQLPackage object CSelf. */
typedef struct _oosqlPackageCSelf {
    ooSQLitePackageEntry *packageEntry;     // Pointer to the package entry table in the external package.
    ooSqlSysLibrary      *lib;              // ooSqlSysLibrary object used to load the package.
    SqlApiVector          sqliteAPIs;       // Pointer to the SQLite API vector.
    RexxObjectPtr         rexxSelf;
    RexxObjectPtr         collationTable;
    RexxObjectPtr         functionTable;
    RexxObjectPtr         moduleTable;
    RexxStringObject      lastErrMsg;
    RexxObjectPtr         lastErrCode;
    bool                  valid;            // False if we failed to load the package table, this object is not usable
} CooSQLPackage;
typedef CooSQLPackage *pCooSQLPackage;

/* Struct for the ooSQLLibrary object CSelf. */
typedef struct _oosqlLibraryCSelf {
    char                  baseName[MAX_LIBRARY_NAME_LENGTH + 1];
    RexxObjectPtr         functionTable;    // A Rexx table object used to hold the resolved functions in the library
    ooSqlSysLibrary      *lib;              // ooSqlSysLibrary object used to load the package.
    SqlApiVector          sqliteAPIs;       // Pointer to the SQLite API vector.
    RexxObjectPtr         rexxSelf;
    RexxStringObject      lastErrMsg;
    RexxObjectPtr         lastErrCode;
    bool                  valid;            // False if some error happened during init(), this object is not usable
} CooSQLLibrary;
typedef CooSQLLibrary *pCooSQLLibrary;

/* Struct for the ooSQLite class object CSelf. */
typedef struct _oosqlclassCSelf {
    pSQLiteCollationNeededEntry autoCollationNeeded;  // Only one at any time.
    pCooSQLPackage        *autoPackages;
    pSQLiteFunctionEntry  *autoFunctions;
    pSQLiteCollationEntry *autoCollations;
    RexxObjectPtr         nullObj;            // Default representation of SQL NULL
    CSTRING               nullStr;            // If nullObj is a Rexx string object, the string value of the object
    ResultSetType         format;             // The default format of a result set for the current process.
    size_t                countPackages;
    size_t                countFunctions;
    size_t                countCollations;
} CooSQLiteClass;
typedef CooSQLiteClass *pCooSQLiteClass;

/* Struct for the ooSQLExtensions class object CSelf. */
typedef struct _oosqlExtensionsCSelf {
    RexxObjectPtr      libraryTable;  // A Rexx table object used to hold loaded library objects
    RexxObjectPtr      packageTable;  // A Rexx table object used to hold loaded package objects
    RexxObjectPtr      rexxSelf;
    RexxStringObject   lastErrMsg;
    RexxObjectPtr      lastErrCode;
    pCooSQLiteClass    pCsc;          // Pointer to the .ooSQLite CSelf struct, uses a lazy init.
} CooSQLExtensions;
typedef CooSQLExtensions *pCooSQLExtensions;

/** Generic struct used for implementing most of the sqlite3 call back functions
 *  in Rexx.
 */
typedef struct _genericCallback {
    RexxRoutineObject    callbackRtn;     // Rexx routine to call.
    RexxObjectPtr        callbackObj;     // Rexx object to invoke the callback() method on.
    RexxObjectPtr        userData;        // A Rexx object that the user wants sent to its Rexx callback.
    RexxObjectPtr        rexxDB;          // The Rexx database connection object, RexxSelf.
    sqlite3             *rtnDB;           // The database connection, used for routine callbacks, like Rexx database for method callbacks
    RexxObjectPtr        nullObj;         // The Rexx object that represents SQL NULL for the database connection
    RexxInstance        *interpreter;
    RexxThreadContext   *callbackContext;
    char               **indexes;
    CSTRING              callbackMethod;
    CSTRING              routineName;     // The name of the Rexx routine, needed for exception messages.
    CSTRING              sqlFuncName;     // The name of the function used by SQLite, the name the collations is registered with
    RexxArrayObject      rsArray;         // When creating a result set and format is arrayOfArrays or arrayOfDirectories
    RexxStemObject       rsStem;          // When creating a result set and format is stemOfStems
    ResultSetType        format;          // Format of a record, array, stem, or directory.
    uint32_t             count;           // This is the record count, updated as each record is produced.
    uint32_t             countIndexes;    // This is the count of allocated strings in the indexes array.
    thread_id_t          initialThreadID;
    bool                 createRS;        // Determines if we are creating a record set or invoking a Rexx callback.
} CGenericCallback;
typedef CGenericCallback *pCGenericCallback;

/**
 *  Non-generic struct used to immplement user defined SQL functions or
 *  aggregates in Rexx.
 */
typedef struct _functionCallback {
    RexxObjectPtr        callbackObj;     // Rexx object to invoke the callback() methods on.
    RexxRoutineObject    funcRtn;         // Rexx routine to call for xFunction.
    RexxRoutineObject    stepRtn;         // Rexx routine to call for xStep.
    RexxRoutineObject    finalRtn;        // Rexx routine to call for xFinal.
    RexxObjectPtr        userData;        // A Rexx object that the user wants sent to its Rexx callback.
    RexxObjectPtr        rexxDB;          // The Rexx database connection object, RexxSelf.
    sqlite3             *rtnDB;           // The database connection, used for routine callbacks, like Rexx database for method callbacks
    RexxInstance        *interpreter;
    RexxThreadContext   *callbackContext;
    CSTRING              funcMethod;
    CSTRING              stepMethod;
    CSTRING              finalMethod;
    CSTRING              sqlFuncName;     // The name of the function used by SQLite, the name the function is registered with
    CSTRING              funcName;        // The name of the Rexx routine, needed for exception messages.
    CSTRING              stepName;        // The name of the Rexx routine, needed for exception messages.
    CSTRING              finalName;       // The name of the Rexx routine, needed for exception messages.
    thread_id_t          initialThreadID;
} CFunctionCallback;
typedef CFunctionCallback *pCFunctionCallback;

typedef struct _AggregateCallback {
    pCFunctionCallback   pcfcb;           // Pointer to our callback information block
    RexxObjectPtr        aggregateObj;    // Sent to Rexx implementation to use as its context
} CAggregateCallback;
typedef CAggregateCallback *pCAggregateCallback;


#endif
