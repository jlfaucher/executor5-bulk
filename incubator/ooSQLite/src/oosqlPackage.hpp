/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2013-2013 Rexx Language Association. All rights reserved.    */
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

/** oosqlPackage.hpp
 *
 * Defines to help build and allow loading of an ooSQLite Package containing
 * external SQLite collations, functions, and modules.  This is similar to the
 * use of sqlite3_load_extension() with some ooSQLite specific enhancements.
 *
 * The implementation itself is modeled on the ooRexx package loading mechanism.
 */

#ifndef oosqlPackage_Included
#define oosqlPackage_Included

#ifdef __cplusplus
#define BEGIN_EXTERN_C() extern "C" {
#define END_EXTERN_C() }
#else
#define BEGIN_EXTERN_C()
#define END_EXTERN_C()
#endif

#ifndef NO_SQLITEEXT_H
#include <sqlite3ext.h>

const sqlite3_api_routines *sqlite3_api = 0;

#endif

typedef const sqlite3_api_routines *SqlApiVector;

BEGIN_EXTERN_C()

#define OOSQLITEENTRY APIENTRY
#define ooSQLiteEntry OOSQLITEENTRY

// Type def for the SQLite collation and collation needed callbacks.
typedef int  (*fnXCompare)(void*, int, const void*, int, const void*);
typedef void (*fnXCollNeeded)(void*, sqlite3*, int, const char*);

// Type def for the SQLite function callbacks
typedef void (*fnXFunc)(sqlite3_context*, int, sqlite3_value**);
typedef void (*fnXStep)(sqlite3_context*, int, sqlite3_value**);
typedef void (*fnXFinal)(sqlite3_context*);

// For create module, a struct is passed in, there are no callback functions,
// except for those in the struct.

// Type def for the common destroy callback.
typedef void (*fnXDestroy)(void*);


// Many of the SQLite callbacks can take a void pointer to user defined data.
// Rather than pass in pUserData in the entry table, a function pointer can be
// passed in the table.  If not null, this function will be invoked to retrieve
// the user data pointer.
typedef void * (*fnXGetUserData)(void);


/* Not sure that we will ever need the reserved fields. */
typedef struct _SQLiteCollationEntry
{
    void           *reserved1;        // reserved for future use
    const char     *name;             // name of the collation
    fnXCompare      entryCompare;     // resolved collation function entry point
    fnXDestroy      entryDestroy;     // resolved destroy function entry point
    // If not null will be invoked before sqlite3_create_collation to retrieve user data point instead of using pUserData.
    fnXGetUserData  entryGetUserData;
    void           *pUserData;        // User (client)) data that can be passed to the compare function
    fnXCollNeeded   entryCollNeeded;  // Collation needed callback, not registered during automatic registration.
    int             reserved2;        // reserved for future use
} SQLiteCollationEntry;
typedef SQLiteCollationEntry *pSQLiteCollationEntry;

#define OOSQL_COLLATION_ENTRY(n, eC, eD, eGUD, uD, eCN) { 0, #n, eC, eD, eGUD, uD, eCN, 0 }

#define OOSQL_COLLATION(n, eC, eD, eGUD, uD, eCN) OOSQL_COLLATION_ENTRY(n, eC, eD, eGUD, uD, eCN)
#define OOSQL_LAST_COLLATION()  { NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0 }

typedef struct _SQLiteFunctionEntry
{
    void           *reserved1;        // reserved for future use
    const char     *name;             // name of the function
    fnXFunc         entryFunc;        // resolved func function entry point
    fnXStep         entryStep;        // resolved step function entry point
    fnXFinal        entryFinal;       // resolved fianl function entry point
    fnXDestroy      entryDestroy;     // resolved destroy function entry point
    fnXGetUserData  entryGetUserData; // If not null will be invoked before sqlite3_create_collation to retrieve user data point
                                      // instead of using pUserData.
    void           *pUserData;        // User (client)) data that can be accessed through sqlite3_user_data()
    int            countArgs;         // Count of args passed to the callback(s)
    int            reserved2;         // reserved for future use
} SQLiteFunctionEntry;
typedef SQLiteFunctionEntry *pSQLiteFunctionEntry;

#define OOSQL_FUNCTION_ENTRY(n, eF, eS, eFi, eD, eGUD, uD, cA) { 0, #n, eF, eS, eFi, eD, eGUD, uD, cA, 0 }

#define OOSQL_FUNCTION(n, eF, eS, eFi, eD, eGUD, uD, cA) OOSQL_FUNCTION_ENTRY(n, eF, eS, eFi, eD, eGUD, uD, cA)
#define OOSQL_LAST_FUNCTION()  { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 0, 0 }

typedef struct _SQLiteModuleEntry
{
    void                 *reserved1;        // reserved for future use
    const char           *name;             // name of the module
    const sqlite3_module *moduleTab;        // The SQLite3 defined struct holding the callbacks
    fnXDestroy            entryDestroy;     // resolved destroy function entry point
    fnXGetUserData        entryGetUserData; // If not null will be invoked before sqlite3_create_collation to retrieve user data
                                            // pointer instead of using pUserData.
    void                 *pUserData;        // User (client)) data for xCreate/xConnect
    int                   reserved2;        // reserved for future use
} SQLiteModuleEntry;
typedef SQLiteModuleEntry *pSQLiteModuleEntry;

#define OOSQL_MODULE_ENTRY(n, mT, uD) { 0, #n, eC, eD, uD, 0 }

#define OOSQL_MODULE(n, mT, uD) OOSQL_COLLATION_ENTRY(n, mT, uD)
#define OOSQL_LAST_MODULE()  { NULL, NULL, NULL, NULL, 0 }

// Package API number is essentially a sortable date, implying later numbers
// are greate than earlier numbers.
#define OOSQL_PACKAGE_API_NO 20130501

// The ooSQLite version number uses 2 digits for major, minor, and level.  The
// svn build number is not used.
#define OOSQLITE_1_0_0           0x00010000
#define OOSQLITE_CURRENT_VERSION OOSQLITE_1_0_0

/**
 * A note on SQLite version numbers.  We use the same format that SQLite defines
 * for itself:
 *
 * "An integer with the value (X*1000000 + Y*1000 + Z) where X, Y, and Z are the
 * same numbers used in SQLITE_VERSION."  Such as 3.7.17.
 *
 * Set reqSQLiteVersion field in the ooSQLitePackageEntry table to the minimum
 * required SQLite version numbe in this format.
 *
 * This is the same value as returned by sqlite3_libversion_number().
 */

#define OOSQL_STANDARD_PACKAGE_HEADER sizeof(ooSQLitePackageEntry), OOSQL_PACKAGE_API_NO,

typedef struct _ooSQLitePackageEntry
{
    int                          size;             // size of the structure...helps compatibility
    int                          apiVersion;       // ooSQLite API version this was compiled with
    int                          reqOOSQLVersion;  // minimum required ooSQLite version (0 means any)
    int                          reqSQLiteVersion; // minimum required SQLite version (0 means any)
    const char                  *packageName;      // package identifier
    const char                  *packageVersion;   // package version #
    pSQLiteCollationEntry        collations;       // collations contained in this package
    pSQLiteFunctionEntry         functions;        // functions contained in this package
    pSQLiteModuleEntry           modules;          // modules contained in this package
} ooSQLitePackageEntry;

#define OOSQLITE_GET_PACKAGE(name) \
    BEGIN_EXTERN_C()\
    ooSQLitePackageEntry *ooSQLiteEntry ooSQLiteGetPackage(const sqlite3_api_routines *pAPI)\
    {\
        sqlite3_api = pAPI;\
        return &name##_package_entry;\
    }\
    END_EXTERN_C()

#define OOSQLITE_SET_API() \
    BEGIN_EXTERN_C()\
    void ooSQLiteEntry ooSQLiteSetApi(const sqlite3_api_routines *pAPI)\
    {\
        sqlite3_api = pAPI;\
    }\
    END_EXTERN_C()


END_EXTERN_C()

typedef ooSQLitePackageEntry * (*OOSQLITE_LOADER)(const sqlite3_api_routines *);
typedef void (*OOSQLITE_API_SETTER)(const sqlite3_api_routines *);

#endif
