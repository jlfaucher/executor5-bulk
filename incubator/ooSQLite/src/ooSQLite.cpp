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
 * ooSQLite.cpp
 *
 * ...
 */

#include "sqlite3.hpp"

#ifdef _WIN32
    #include "platform\windows\winOS.hpp"
#else
    #include "platform/unix/unixOS.hpp"
#endif

#include <stdio.h>
#include <string.h>

#include "oorexxapi.h"
#include "ooSQLite.hpp"
#include "APICommon.hpp"


// Initialized in ooSQLiteLoad().
RexxObjectPtr       TheTrueObj        = NULLOBJECT;
RexxObjectPtr       TheFalseObj       = NULLOBJECT;
RexxObjectPtr       TheNilObj         = NULLOBJECT;
RexxObjectPtr       TheZeroObj        = NULLOBJECT;
RexxObjectPtr       TheOneObj         = NULLOBJECT;
RexxObjectPtr       TheTwoObj         = NULLOBJECT;
RexxObjectPtr       TheNegativeOneObj = NULLOBJECT;

// Initialized in the class init methods.
RexxClassObject     TheOOSQLiteDBClass    = NULLOBJECT;
RexxClassObject     TheOOSQLiteStmtClass  = NULLOBJECT;
RexxClassObject     TheOOSQLiteMutexClass = NULLOBJECT;

// The class object is initialized in the class init method.  The hidden helper
// object is only initialized when / if needed.
RexxClassObject     TheRoutineHelperClass = NULLOBJECT;
RexxObjectPtr       TheHiddenHelper       = NULLOBJECT;


/**
 * RexxPackageLoader function.
 *
 * The package loader function is called when the library package is first
 * loaded.  This makes it the ideal place for any initialization that must be
 * done prior to the Rexx program start up.  We call sqlite3_initialize() and
 * set up a few global values.
 *
 * @param c  Thread context pointer passed from the intepreter when this package
 *           is loaded.
 *
 * @return Nothing is returned
 */
void RexxEntry ooSQLiteLoad(RexxThreadContext *c)
{
    int rc = sqlite3_initialize();

#ifdef OOSQLDBG
    printf("\nooSQLiteLoad() entered sqlite3_initialize() rc=%d\n\n", rc);
#endif

    TheTrueObj    = c->True();
    TheFalseObj   = c->False();
    TheNilObj     = c->Nil();
    TheZeroObj    = TheFalseObj;
    TheOneObj     = TheTrueObj;

    TheNegativeOneObj = c->WholeNumber(-1);
    c->RequestGlobalReference(TheNegativeOneObj);

    TheTwoObj = c->WholeNumber(2);
    c->RequestGlobalReference(TheTwoObj);

#ifndef _WIN32
    crit_sec = sqlite3_mutex_alloc(SQLITE_MUTEX_FAST);
    if ( crit_sec == NULL )
    {
        executionErrorException(c, "Could not allocate required mutex, abort on package load");
    }
#endif

}

/**
 * RexxPackageUnloader function.
 *
 * The package unloader function is called when the library package is unloaded
 * by the interpreter. The unloading process happens when the last interpreter
 * instance is destroyed during the last cleanup stages.
 *
 * We just shut down the sqlite3 library, basically undo sqlite3_initialize().
 * sqlit3_shutdown() is a noop if the library is already shut down, and always
 * returns SQLITE_OK.
 *
 * @param c  Thread context pointer passed from the intepreter when this package
 *           is unloaded.
 *
 * @return Nothing is returned
 */
void RexxEntry ooSQLiteUnload(RexxThreadContext *c)
{
#ifdef OOSQLDBG
    printf("\nooSQLiteUnload() entered\n\n");
#endif

#ifndef _WIN32
    sqlite3_mutex_free(crit_sec);
#endif

    sqlite3_shutdown();
}


/**
 * This is not used anywhere, but it seems like it would be useful.
 *
 * @param c         Thread context we are operating in.
 * @param db        Database connection.
 * @param function  Name of SQLite API.
 */
void sqliteErrorException(RexxThreadContext *c, sqlite3 *db, const char *function)
{
    char buffer[256];
    snprintf(buffer, sizeof(buffer), "SQLite API error:  API(%s) rc(%d) msg(%s)",
             function, sqlite3_errcode(db), sqlite3_errmsg(db) ? sqlite3_errmsg(db) : "none");
    systemServiceException(c, buffer);
}

/**
 * Instantiates a new 2-dimensional array.  This is not used anywhere, seemed
 * like it would be useful, but not so sure about that now.
 *
 * @param c
 * @param size
 *
 * @return RexxArrayObject
 */
RexxArrayObject newTwoDimensionArray(RexxThreadContext *c, size_t size)
{
    RexxClassObject arrayCls = c->FindClass("ARRAY");
    RexxArrayObject result   = NULLOBJECT;

    if ( arrayCls != NULLOBJECT )
    {
        if ( size < 1 )
        {
            size = 5;
        }
        RexxObjectPtr s = c->WholeNumber(size);

        RexxArrayObject args = c->ArrayOfTwo(s, s);

        result = (RexxArrayObject)c->SendMessage(arrayCls, "NEW", args);
    }
    return result;
}


/**
 * If the value in the database for the requested field is NULL, the
 * sqlite3_column_text() returns a null pointer
 *
 * We can't pass a null pointer ito the native API, so if null, we use the
 * string "NULL" instead.
 *
 * @param stmt
 * @param col
 *
 * @return a non-null string.
 */
inline CSTRING safeColumnText(sqlite3_stmt *stmt, int col)
{
    CSTRING data = (CSTRING)sqlite3_column_text(stmt, col);
    return (data == NULL ? "NULL" : data);
}

/**
 * Many of the returns from the SQLite functions are a const char pointer, or
 * NULL.  This function ensures that NULL is not passed into the native API and
 * uses the string "NULL" if str is null.
 *
 * @param c    Thread context we are operating in.
 * @param str  String to convert to a Rexx string.
 *
 * @return "str" converted to a Rexx string, using the word "NULL" if str is
 *         null.
 */
inline RexxStringObject safeRexxString(RexxThreadContext *c, CSTRING str)
{
    return c->String(str == NULL ? "NULL" : str);
}

/**
 * As noted above, many of the returns from the SQLite functions are a const
 * char pointer, or NULL.  This function ensures that NULL is not passed into
 * the native API and uses the normal Rexx empty string.  Hence the 'Rx' at the
 * end of the function name.
 *
 * @param c    Thread context we are operating in.
 * @param str  String to convert to a Rexx string.
 *
 * @return The converted Rexx string for str, or the empty string if str is
 *         null.
 */
inline RexxStringObject safeRexxStringRx(RexxThreadContext *c, CSTRING str)
{
    return (str == NULL ? c->NullString() : c->String(str));
}

/**
 * There are cases where the return to Rexx can be a number, or an error code.
 * The user can not distinguish if the number is an error or not.
 *
 * For now, this error string is returned in some cases.  Note - this does make
 * a quick and easy return during developement, but for release, having a
 * message is better.
 *
 * Need to revisit this thinking.
 *
 * @param c            Thread context we are operating in.
 * @param rc           SQLITE or OOSQLITE error code.
 * @param isSQLiteErr  True if this is an error code returned from the SQLite
 *                     API, false if it is our own ooRexx extension error.
 */
static inline RexxStringObject ooSQLiteErr(RexxThreadContext *c, wholenumber_t rc, bool isSQLiteErr)
{
    char buf[64];

    snprintf(buf, sizeof(buf), "%s Error: %d", isSQLiteErr ? "SQLite" : "ooSQLite", rc);
    return c->String(buf);
}

/**
 * There are cases where the return to Rexx can be a number, or an error code.
 * The user can not distinguish if the number is an error or not.
 *
 * For now, this error string is returned.
 *
 * Need to revisit this thinking.
 *
 * @param c            Thread context we are operating in.
 * @param rc           SQLITE or OOSQLITE error code.
 * @param msg          Message to incorporate into the returned string.
 * @param isSQLiteErr  True if this is an error code returned from the SQLite
 *                     API, false if it is our own ooRexx extension error.
 */
static inline RexxStringObject ooSQLiteErr(RexxThreadContext *c, wholenumber_t rc, CSTRING msg, bool isSQLiteErr)
{
    char buf[512];

    snprintf(buf, sizeof(buf), "%s Error: rc = %d, %s",
             isSQLiteErr ? "SQLite" : "ooSQLite", rc, msg);
    return c->String(buf);
}


/**
 * Returns a result set in the form of an array of 'records.'
 *
 * Each record is an array of the column valuess for that record, except for the
 * first array in the records.  The first array object in the records array
 * contains the column names, making the first data record the object at index
 * 2.
 *
 * @param c     Thread context we are operating in.
 * @param stmt  A prepared sqlite3 statemnt.
 *
 * @return An array of the records produced by the statement.
 *
 * @assumes  The specified statement is valid, that the return from
 *           sqlite3_prepare_v2() has already been checked.  Also assumes that
 *           sqlite3_step() has not been called yet.
 *
 * @note  If there are no rows returned by stepping the statement, then an empty
 *        array is returned.
 */
RexxArrayObject getRecords(RexxThreadContext *c, sqlite3_stmt *stmt)
{
    RexxArrayObject a = c->NewArray(10);  // An array of records
    RexxArrayObject r;                    // A single record
    int i = 1, j = 0;

    while ( sqlite3_step(stmt) == SQLITE_ROW )
    {
        CSTRING data;
        int     count;

        if ( i == 1 )
        {
            count = sqlite3_column_count(stmt);

            r = c->NewArray(count);
            for ( j = 0; j < count; j++)
            {
                data = sqlite3_column_name(stmt, j);
                c->ArrayAppendString(r, data, strlen(data));
            }
            c->ArrayPut(a, r, i++);


            r = c->NewArray(count);
            for ( j = 0; j < count; j++)
            {
                data = safeColumnText(stmt, j);
                c->ArrayAppendString(r, data, strlen(data));
            }
        }
        else
        {
            r = c->NewArray(count);
            for ( int j = 0; j < count; j++)
            {
                data = safeColumnText(stmt, j);
                c->ArrayAppendString(r, data, strlen(data));
            }
        }
        c->ArrayPut(a, r, i++);
    }

    return a;
}


/**
 * Generic function to return either the compact or full version for the
 * ooSQLite extension.
 *
 * This is not part of the SQLite library, it is an ooSQLite only interface.
 * Used by both the object orientated (.ooSQLite) and classic Rexx interfaces.
 *
 * @param c     Thread context we are operating in.
 * @param full  If true, the full version is returned, otherwise the one line
 *              version.
 *
 * @return A formatted string specifying the ooSQLite version.
 */
RexxStringObject genGetVersion(RexxThreadContext *c, logical_t full, logical_t minimal)
{
    char buf[512];

    size_t bits = 32;

#ifdef __REXX64__
    bits = 64;
#endif

    if ( full )
    {
        snprintf(buf, sizeof(buf), "ooSQLite: ooSQLite Version %d.%d.%d.%d (%d bit)\n",
                 OOSQLITE_VER_MAJOR, OOSQLITE_VER_MINOR, OOSQLITE_VER_LEVEL, OOSQLITE_VER_BUILD, bits);

        char buf1[256];

        snprintf(buf1, sizeof(buf1), "          Built %s %s\n          Copyright (c) RexxLA %s.\n"
                                     "          All Rights Reserved.\n\n",
                 __DATE__, __TIME__, OOSQLITE_COPYRIGHT_YEAR);
        strcat(buf, buf1);

        size_t rx = c->InterpreterVersion();

        snprintf(buf1, sizeof(buf1), "Rexx:     Open Object Rexx Version %d.%d.%d\n\n",
                 (rx >> 16) & 0xff, (rx >> 8) & 0xff, rx & 0x0000ff);
        strcat(buf, buf1);

        snprintf(buf1, sizeof(buf1), "SQLite:   SQLite Library Version %s\n          %.19s\n",
                 sqlite3_libversion(), sqlite3_sourceid());
        strcat(buf, buf1);
    }
    else
    {
        if ( minimal )
        {
            snprintf(buf, sizeof(buf), "%d.%d.%d.%d\n",
                     OOSQLITE_VER_MAJOR, OOSQLITE_VER_MINOR, OOSQLITE_VER_LEVEL, OOSQLITE_VER_BUILD);
        }
        else
        {
            snprintf(buf, sizeof(buf), "ooSQLite Version %d.%d.%d.%d (%d bit)\n",
                     OOSQLITE_VER_MAJOR, OOSQLITE_VER_MINOR, OOSQLITE_VER_LEVEL, OOSQLITE_VER_BUILD, bits);
        }
    }

    return c->String(buf);
}


/**
 * Generic function to do a bitwise or (|) together a series of numbers.
 * Intended to allow the ooSQLite programmer to combine SQLite bit flags, but it
 * makes no check that the numbers provided are actually SQLite defines.
 *
 * This is not part of the SQLite library, it is an ooSQLite only interface.
 * Used by both the object orientated (.ooSQL) and classic Rexx interfaces.
 *
 * @param c
 * @param args
 *
 * @return wholenumber_t
 */
wholenumber_t genMerge(RexxThreadContext *c, RexxArrayObject args)
{
    wholenumber_t result = 0;

    size_t count = c->ArraySize(args);
    for ( size_t i = 1; i <= count; i++ )
    {
        RexxObjectPtr v = c->ArrayAt(args, i);
        if ( v != NULLOBJECT )
        {
            wholenumber_t val;
            if ( ! c->WholeNumber(v, &val) )
            {
                invalidTypeException(c, i, "whole number");
                return 0;
            }
            result |= val;
        }
        else
        {
            missingArgException(c, i);
            return 0;
        }
    }

    return result;
}


int genDbStatus(RexxThreadContext *c, sqlite3 *db, int param, RexxObjectPtr _result, logical_t reset, size_t pos)
{
    if ( ! c->IsDirectory(_result) )
    {
        wrongClassException(c, pos, "Directory", _result);
        return SQLITE_MISUSE;
    }

    RexxDirectoryObject result = (RexxDirectoryObject)_result;

    int cur;
    int hiwtr;

    int rc = sqlite3_db_status(db, param, &cur, &hiwtr, (int)reset);

    if ( rc == SQLITE_OK )
    {
        c->DirectoryPut(result, c->StringSize(cur), "CURRENT");
        c->DirectoryPut(result, c->StringSize(hiwtr), "HIGHWATER");
    }

    return rc;
}

int genStatus(RexxThreadContext *c, int param, RexxObjectPtr _result, logical_t reset)
{
    if ( ! c->IsDirectory(_result) )
    {
        wrongClassException(c, 2, "Directory", _result);
        return SQLITE_MISUSE;
    }

    RexxDirectoryObject result = (RexxDirectoryObject)_result;

    int cur    = -1;
    int hiwtr  = -1;

    int rc = sqlite3_status(param, &cur, &hiwtr, (int)reset);

    if ( rc == SQLITE_OK )
    {
        c->DirectoryPut(result, c->WholeNumber(cur), "CURRENT");
        c->DirectoryPut(result, c->WholeNumber(hiwtr), "HIGHWATER");
    }

    return rc;
}

int genTableColumnMetadata(RexxThreadContext *c, sqlite3 *db, CSTRING dbName, CSTRING tableName,
                           CSTRING colName, RexxObjectPtr _results, size_t pos)
{
    if ( ! c->IsDirectory(_results) )
    {
        wrongClassException(c, pos, "Directory", _results);
        return SQLITE_MISUSE;
    }

    RexxDirectoryObject results = (RexxDirectoryObject)_results;

    char const *dataType;
    char const *collSeq;
    int         notNull;
    int         primaryKey;
    int         autoInc;

    int rc = sqlite3_table_column_metadata(
      db,           /* Connection handle */
      dbName,       /* Database name or NULL */
      tableName,    /* Table name */
      colName,      /* Column name */
      &dataType,    /* OUTPUT: Declared data type */
      &collSeq,     /* OUTPUT: Collation sequence name */
      &notNull,     /* OUTPUT: True if NOT NULL constraint exists */
      &primaryKey,  /* OUTPUT: True if column part of PK */
      &autoInc      /* OUTPUT: True if column is auto-increment */
    );

    if ( rc == SQLITE_OK )
    {
        c->DirectoryPut(results, c->String(dataType),     "DATATYPE");
        c->DirectoryPut(results, c->String(collSeq),      "COLLATIONSEQUENCE");
        c->DirectoryPut(results, c->Logical(notNull),     "NOTNULL");
        c->DirectoryPut(results, c->Logical(primaryKey),  "PRIMARYKEY");
        c->DirectoryPut(results, c->Logical(autoInc),     "AUTOINCREMENT");
    }

    return rc;
}


/**
 * Checks that the reply from a Rexx callback is valid.
 *
 * Note that whether to clear the condition when a condition was raised in the
 * Rexx callback may be hard to judge.  In the simple test case programs, not
 * clearing the condition causes the error message to be printed twice.  But,
 * I'm not sure that will be the case is more complex applications.  May have to
 * revisit this.  TODO REVISIT.  Clearing the condition here, causes the
 * condition to NOT be raised when we get back to Rexx.  Surely that is not
 * good.
 *
 * @param c
 * @param reply
 * @param rc
 * @param mthName
 * @param rtnName
 * @param isMethod
 *
 * @return bool
 */
static bool replyIsGood(RexxThreadContext *c, RexxObjectPtr reply, wholenumber_t *rc, CSTRING mthName,
                        CSTRING rtnName, bool isMethod)
{
    bool result = true;

    if ( checkForCondition(c, false) )
    {
        result = false;
    }
    else if ( reply == NULLOBJECT )
    {
        result = false;

        if ( isMethod )
        {
            noMsgReturnException(c, mthName);
        }
        else
        {
            noRoutineReturnException(c, rtnName);
        }

        // This is used to print out the condition we just raised.
        checkForCondition(c, false);
    }
    else if ( ! c->WholeNumber(reply, rc) )
    {
        result = false;

        invalidReturnWholeNumberException(c, isMethod ? mthName : rtnName, reply, isMethod);
    }

    return result;
}

/**
 * The call back function for sqlite3_set_authorizer()
 *
 * @param data
 * @param op
 * @param str1
 * @param str2
 * @param str3
 * @param str4
 *
 * @return int  Must be one on SQLITE_OK, SQLITE_IGNORE, or SQLITE_DENY
 *
 * @notes  The SQLite doc does not say what the const char *args are.  It just
 *         says: "The third through sixth parameters to the callback are
 *         zero-terminated strings that contain additional details about the
 *         action to be authorized."  Need to ask on the user list what the args
 *         are.
 */
static int authorizerCallback(void *data, int op, const char *str1, const char *str2, const char *str3, const char *str4)
{
    pCGenericCallback  d = (pCGenericCallback)data;
    RexxThreadContext *c = d->callbackContext;

    RexxArrayObject args;
    RexxObjectPtr   reply = NULLOBJECT;
    wholenumber_t   rc    = 0;

    bool isMethod = (d->callbackObj == NULLOBJECT) ? false : true;

    args = c->ArrayOfFour(c->WholeNumber(op), c->String(str1), c->String(str2), c->String(str3));

    c->ArrayPut(args, c->String(str4), 5);

    if ( d->userData == NULL )
    {
       c->ArrayPut(args, TheNilObj, 6);
    }
    else
    {
        c->ArrayPut(args, d->userData, 6);
    }

    if ( isMethod )
    {
        reply = c->SendMessage(d->callbackObj, d->callbackMethod, args);
    }
    else
    {
        reply = c->CallRoutine(d->callbackRtn, args);
    }

    // If rc turns out to not be one of the 3 acceptable returns, then SQLite
    // will deal with it, so we don't check what the actual value of rc is.
    // But, if there is a bad reply we return SQLITE_DENY
    if ( ! replyIsGood(c, reply, &rc, d->callbackMethod, d->routineName, isMethod) )
    {
        rc = SQLITE_DENY;
    }

    return (int)rc;
}

/**
 * The call back function for sqlite3_busy_handler()
 *
 *
 * @param data
 *
 * @return int
 */
static int busyCallBack(void *data, int countInvoked)
{
    pCGenericCallback  d = (pCGenericCallback)data;
    RexxThreadContext *c = d->callbackContext;

    RexxArrayObject args;
    RexxObjectPtr   reply = NULLOBJECT;
    RexxObjectPtr   count = c->WholeNumber(countInvoked);
    wholenumber_t   rc    = 0;

    bool isMethod = (d->callbackObj == NULLOBJECT) ? false : true;

    if ( d->userData == NULL )
    {
       args = c->ArrayOfTwo(count, TheNilObj);
    }
    else
    {
        args = c->ArrayOfTwo(count, d->userData);
    }

    if ( isMethod )
    {
        reply = c->SendMessage(d->callbackObj, d->callbackMethod, args);
    }
    else
    {
        reply = c->CallRoutine(d->callbackRtn, args);
    }

    if ( ! replyIsGood(c, reply, &rc, d->callbackMethod, d->routineName, isMethod) )
    {
        rc = 0;
    }

    return (int)rc;
}


/**
 * The call back function for sqlite3_commit_hook()
 *
 * @param data
 *
 * @return int
 *
 * @notes  If this callback returns 0 then the commit is allowed to procede
 *         normally.  If non-zero is returned the commit is turned into a
 *         rollback.  So, the Rexx callback should return the same.  If the Rexx
 *         return is an error then we return non-zero to cause a rollback.
 */
static int commitHookCallback(void *data)
{
    pCGenericCallback  d = (pCGenericCallback)data;
    RexxThreadContext *c = d->callbackContext;

    RexxArrayObject args;
    RexxObjectPtr   reply = NULLOBJECT;
    wholenumber_t   rc    = 0;

    bool isMethod = (d->callbackObj == NULLOBJECT) ? false : true;

    if ( d->userData == NULL )
    {
        args = c->ArrayOfOne(TheNilObj);
    }
    else
    {
        args = c->ArrayOfOne(d->userData);
    }

    if ( isMethod )
    {
        reply = c->SendMessage(d->callbackObj, d->callbackMethod, args);
    }
    else
    {
        reply = c->CallRoutine(d->callbackRtn, args);
    }

    if ( ! replyIsGood(c, reply, &rc, d->callbackMethod, d->routineName, isMethod) )
    {
        rc = 1;
    }

    return (int)rc;
}

/**
 * The call back function for sqlite3_profile()
 *
 * @param data  A poitner to our callback struct
 *
 * @param statement  The original SQL statement
 *
 * @param nanosecs   The wall clock time it took the statement to run.
 *
 * @return void
 *
 * @notes  There is no return from the callback to SQLite.  But, we still
 *         require a return from the Rexx callback.  That return is just
 *         ignored.  The Rexx programmer should just return 0.
 */
static void profileHookCallback(void *data, const char *statement, sqlite3_uint64 nanosecs)
{
    pCGenericCallback  d = (pCGenericCallback)data;
    RexxThreadContext *c = d->callbackContext;

    RexxArrayObject args;
    RexxObjectPtr   reply = NULLOBJECT;
    wholenumber_t   rc    = 0;

    bool isMethod = (d->callbackObj == NULLOBJECT) ? false : true;

    if ( d->userData == NULL )
    {
        args = c->ArrayOfThree(c->String(statement), c->UnsignedInt64(nanosecs), TheNilObj);
    }
    else
    {
        args = c->ArrayOfThree(c->String(statement), c->UnsignedInt64(nanosecs), d->userData);
    }

    if ( isMethod )
    {
        reply = c->SendMessage(d->callbackObj, d->callbackMethod, args);
    }
    else
    {
        reply = c->CallRoutine(d->callbackRtn, args);
    }

    // We just check this, which may raise an exception, but don't do anything
    // else.
    replyIsGood(c, reply, &rc, d->callbackMethod, d->routineName, isMethod);
}


/**
 * The call back function for sqlite3_progressHandler()
 *
 * @param data  A poitner to our callback struct
 *
 * @return Non-zero to interrupt the operation in progress, or 0 to continue.
 */
static int progressCallback(void *data)
{
    pCGenericCallback  d = (pCGenericCallback)data;
    RexxThreadContext *c = d->callbackContext;

    RexxArrayObject args;
    RexxObjectPtr   reply = NULLOBJECT;
    wholenumber_t   rc    = 0;

    bool isMethod = (d->callbackObj == NULLOBJECT) ? false : true;

    if ( d->userData == NULL )
    {
        args = c->ArrayOfOne(TheNilObj);
    }
    else
    {
        args = c->ArrayOfOne(d->userData);
    }

    if ( isMethod )
    {
        reply = c->SendMessage(d->callbackObj, d->callbackMethod, args);
    }
    else
    {
        reply = c->CallRoutine(d->callbackRtn, args);
    }

    // If the return from Rexx is not good, we return 1 to interrupt the
    // operation.
    if ( ! replyIsGood(c, reply, &rc, d->callbackMethod, d->routineName, isMethod) )
    {
        rc = 1;
    }

    return (int)rc;
}


/**
 * The call back function for sqlite3_rollback_hook()
 *
 * @param data
 *
 * @return void
 *
 * @notes  There is no return from the callback to SQLite.  But, we still
 *         require a return from the Rexx callback.  That return is just
 *         ignored.  The Rexx programmer should just return 0.
 */
static void rollbackHookCallback(void *data)
{
    pCGenericCallback  d = (pCGenericCallback)data;
    RexxThreadContext *c = d->callbackContext;

    RexxArrayObject args;
    RexxObjectPtr   reply = NULLOBJECT;
    wholenumber_t   rc    = 0;

    bool isMethod = (d->callbackObj == NULLOBJECT) ? false : true;

    if ( d->userData == NULL )
    {
        args = c->ArrayOfOne(TheNilObj);
    }
    else
    {
        args = c->ArrayOfOne(d->userData);
    }

    if ( isMethod )
    {
        reply = c->SendMessage(d->callbackObj, d->callbackMethod, args);
    }
    else
    {
        reply = c->CallRoutine(d->callbackRtn, args);
    }

    // We just check this, which may raise an exception, but don't do anything
    // else.
    replyIsGood(c, reply, &rc, d->callbackMethod, d->routineName, isMethod);
}


/**
 * The call back function for sqlite3_trace()
 *
 * @param data  A poitner to our callback struct
 *
 * @param statement  A UTF-8 rendering of the SQL statement text or a UTF-8 SQL
 *                   comment that identifies the trigger if the callback is
 *                   invoked by a trigger
 *
 * @return void
 *
 * @notes  There is no return from the callback to SQLite.  But, we still
 *         require a return from the Rexx callback.  That return is just
 *         ignored.  The Rexx programmer should just return 0.
 */
static void traceHookCallback(void *data, const char *statement)
{
    pCGenericCallback  d = (pCGenericCallback)data;
    RexxThreadContext *c = d->callbackContext;

    RexxArrayObject args;
    RexxObjectPtr   reply = NULLOBJECT;
    wholenumber_t   rc    = 0;

    bool isMethod = (d->callbackObj == NULLOBJECT) ? false : true;

    if ( d->userData == NULL )
    {
        args = c->ArrayOfTwo(c->String(statement), TheNilObj);
    }
    else
    {
        args = c->ArrayOfTwo(c->String(statement), d->userData);
    }

    if ( isMethod )
    {
        reply = c->SendMessage(d->callbackObj, d->callbackMethod, args);
    }
    else
    {
        reply = c->CallRoutine(d->callbackRtn, args);
    }

    // We just check this, which may raise an exception, but don't do anything
    // else.
    replyIsGood(c, reply, &rc, d->callbackMethod, d->routineName, isMethod);
}


/**
 * The call back function for sqlite3_update_hook()
 *
 * @param data
 * @param op
 * @param dbName
 * @param tableName
 * @param rowID
 *
 * @return void
 *
 * @notes  There is no return from the callback to SQLite.  But, we still
 *         require a return from the Rexx callback.  That return is just
 *         ignored.  The Rexx programmer should just return 0.
 */
static void updateHookCallback(void *data, int op, const char *dbName, const char *tableName, sqlite3_int64 rowID)
{
    pCGenericCallback  d = (pCGenericCallback)data;
    RexxThreadContext *c = d->callbackContext;

    RexxArrayObject args;
    RexxObjectPtr   reply = NULLOBJECT;
    wholenumber_t   rc    = 0;

    bool isMethod = (d->callbackObj == NULLOBJECT) ? false : true;

    args = c->ArrayOfFour(c->WholeNumber(op), c->String(dbName), c->String(tableName), c->Int64(rowID));

    if ( d->userData == NULL )
    {
       c->ArrayPut(args, TheNilObj, 5);
    }
    else
    {
        c->ArrayPut(args, d->userData, 5);
    }

    if ( isMethod )
    {
        reply = c->SendMessage(d->callbackObj, d->callbackMethod, args);
    }
    else
    {
        reply = c->CallRoutine(d->callbackRtn, args);
    }

    // We just check this, which may raise an exception if the Rexx method or
    // routine has problems, but don't do anything else.
    replyIsGood(c, reply, &rc, d->callbackMethod, d->routineName, isMethod);
}

/**
 * The call back function for sqlite3_exec()
 *
 *
 * @param data
 * @param ncols
 * @param values
 * @param headers
 *
 * @return int
 */
static int execCallBack(void *data, int ncols, char **values, char **headers)
{
    pCGenericCallback  d = (pCGenericCallback)data;
    RexxThreadContext *c = d->callbackContext;

    RexxArrayObject record = c->NewArray(5);
    RexxArrayObject header = c->NewArray(5);

    for ( int i = 0; i < ncols; i++ )
    {
        if ( values[i] == NULL )
        {
            c->ArrayAppendString(record, "NULL", 4);
        }
        else
        {
            c->ArrayAppendString(record, values[i], strlen(values[i]));
        }
    }

    wholenumber_t rc = SQLITE_OK;

    if ( d->useArray )
    {
        if ( d->count == 0 )
        {
            for ( int i = 0; i < ncols; i++ )
            {
                c->ArrayAppendString(header, headers[i], strlen(headers[i]));
            }

            c->ArrayPut(d->records, header, 1);
            d->count = 1;
        }

        d->count++;
        c->ArrayPut(d->records, record, d->count);
    }
    else
    {
        RexxObjectPtr   reply    = NULLOBJECT;
        RexxArrayObject rows     = c->NewArray(2);
        bool            isMethod = (d->callbackObj == NULLOBJECT) ? false : true;

        for ( int i = 0; i < ncols; i++ )
        {
            c->ArrayAppendString(header, headers[i], strlen(headers[i]));
        }

        c->ArrayPut(rows, header, 1);
        c->ArrayPut(rows, record, 2);

        d->count++;

        RexxArrayObject args;
        RexxObjectPtr   count = c->WholeNumber(d->count);

        if ( d->userData == NULL )
        {
            args = c->ArrayOfTwo(rows, count);
        }
        else
        {
            args = c->ArrayOfThree(rows, count, d->userData);
        }

        if ( isMethod )
        {
            reply = c->SendMessage(d->callbackObj, d->callbackMethod, args);
        }
        else
        {
            reply = c->CallRoutine(d->callbackRtn, args);
        }

        if ( ! replyIsGood(c, reply, &rc, d->callbackMethod, d->routineName, isMethod) )
        {
            rc = SQLITE_ABORT;
        }
    }

    return (int)rc;
}


#define ooSQLite_Methods_Section  1


/**
 * Retrieves the CSelf pointer for a ooSQLiteDB object when the database object
 * is not the direct object the method was invoked on.  This performs a scoped
 * CSelf lookup.
 *
 * @param c    The method context we are operating in.
 * @param db   The database object whose CSelf pointer is needed.
 *
 * @return A pointer to the CSelf of the database object.
 *
 * @assumes  The caller has ensured db is in fact an ooSQLiteDB Rexx database
 *           object.
 */
inline pCooSQLiteDB dbToCSelf(RexxThreadContext *c, RexxObjectPtr db)
{
    return (pCooSQLiteDB)c->ObjectToCSelf(db, TheOOSQLiteDBClass);
}
inline pCooSQLiteDB dbToCSelf(RexxMethodContext *c, RexxObjectPtr db)
{
    return dbToCSelf(c->threadContext, db);
}


/**
 * Retrieves the CSelf pointer for a ooSQLiteStmt object when the statemnt
 * object is not the direct object the method was invoked on.  This performs a
 * scoped CSelf lookup.
 *
 * @param c    The method / thread context we are operating in.
 * @param stmt The statement object whose CSelf pointer is needed.
 *
 * @return A pointer to the CSelf of the statement object.
 *
 * @assumes  The caller has ensured stmt is in fact an ooSQLiteStmt Rexx
 *           statement object.
 */
inline pCooSQLiteStmt stmtToCSelf(RexxThreadContext *c, RexxObjectPtr stmt)
{
    return (pCooSQLiteStmt)c->ObjectToCSelf(stmt, TheOOSQLiteStmtClass);
}
inline pCooSQLiteStmt stmtToCSelf(RexxMethodContext *c, RexxObjectPtr stmt)
{
    return stmtToCSelf(c->threadContext, stmt);
}


void noOpenDBException(RexxThreadContext *context, pCooSQLiteDB pCdb)
{
    char buffer[256];
    if ( pCdb->fileName != NULL )
    {
        snprintf(buffer, sizeof(buffer), "The %s data base is not open", pCdb->fileName);
    }
    else
    {
        snprintf(buffer, sizeof(buffer), "There is no open data base");
    }
    executionErrorException(context, buffer);
}


void invalidStmtException(RexxThreadContext *context, pCooSQLiteStmt pCstmt)
{
    char buffer[256];
    if ( pCstmt->finalized && ! pCstmt->initializationErr )
    {
        snprintf(buffer, sizeof(buffer), "The ooSQLite statement is not valid, it has been finalized");
    }
    else
    {
        snprintf(buffer, sizeof(buffer), "The ooSQLite statement is not valid, it was not prepared without error");
    }
    executionErrorException(context, buffer);
}


void invalidMutexException(RexxThreadContext *context, pCooSQLiteMutex pCmtx)
{
    char buffer[256];
    if ( pCmtx->closed && ! pCmtx->initializationErr )
    {
        snprintf(buffer, sizeof(buffer), "The ooSQLite mutex is not valid, it has been closed");
    }
    else
    {
        snprintf(buffer, sizeof(buffer), "The ooSQLite mutex is not valid, it was not opened without error");
    }
    executionErrorException(context, buffer);
}


/**
 * Produces
 *
 * For now, this error string is returned.
 *
 * Need to revisit this thinking.
 *
 * @param c            Method context we are operating in.
 * @param pCdb         Pointer to the CSelf for an ooSQLiteDB object.
 * @param rc           SQLITE error code.
 * @param isSQLiteErr  True if this is an error code returned from the SQLite
 *                     API, false if it is our own ooRexx extension error.
 */
static RexxStringObject ooSQLiteErr(RexxMethodContext *c, pCooSQLiteDB pCdb, int rc, CSTRING msg, bool isSQLiteErr)
{
    pCdb->lastErrMsg  = ooSQLiteErr(c->threadContext, rc, msg, isSQLiteErr);
    pCdb->lastErrCode = rc;

    c->SetObjectVariable("rxLastErrMsg", pCdb->lastErrMsg);

    return pCdb->lastErrMsg;
}

static inline pCooSQLiteDB requiredDBCSelf(RexxMethodContext *c, void *pCSelf)
{
    if ( pCSelf == NULL )
    {
        baseClassIntializationException(c, "ooSQLiteDB");
    }
    return (pCooSQLiteDB)pCSelf;
}

static inline pCooSQLiteDB requiredDB(RexxMethodContext *c, void *pCSelf)
{
    pCooSQLiteDB pCdb = requiredDBCSelf(c, pCSelf);
    if ( pCdb != NULL )
    {
        if ( pCdb->db == NULL )
        {
            noOpenDBException(c->threadContext, pCdb);
            pCdb = NULL;
        }
    }
    return pCdb;
}

static inline pCooSQLiteMutex requiredMtxCSelf(RexxMethodContext *c, void *pCSelf)
{
    if ( pCSelf == NULL )
    {
        baseClassIntializationException(c, "ooSQLiteMutex");
    }
    return (pCooSQLiteMutex)pCSelf;
}

static inline pCooSQLiteMutex requiredMtx(RexxMethodContext *c, void *pCSelf)
{
    pCooSQLiteMutex pCmtx = requiredMtxCSelf(c, pCSelf);
    if ( pCmtx != NULL )
    {
        if ( pCmtx->mutex == NULL )
        {
            invalidMutexException(c->threadContext, pCmtx);
            pCmtx = NULL;
        }
    }
    return pCmtx;
}

static inline pCooSQLiteStmt requiredStmtCSelf(RexxMethodContext *c, void *pCSelf)
{
    if ( pCSelf == NULL )
    {
        baseClassIntializationException(c, "ooSQLiteStmt");
    }
    return (pCooSQLiteStmt)pCSelf;
}


static inline pCooSQLiteStmt requiredStmt(RexxMethodContext *c, void *pCSelf)
{
    pCooSQLiteStmt pCstmt = requiredStmtCSelf(c, pCSelf);
    if ( pCstmt != NULL )
    {
        if ( pCstmt->stmt == NULL )
        {
            invalidStmtException(c->threadContext, pCstmt);
            pCstmt = NULL;
        }
    }
    return pCstmt;
}

static inline CSTRING getCallbackVarName(CallbackType cb)
{
    switch ( cb )
    {
        case authorizer :
            return "__rxAuthorizer";
        case busyHandler :
            return "__rxBusyHandler";
        case commitHook :
            return "__rxCommitHook";
        case profileHook :
            return "__rxProfileHook";
        case progressHandler :
            return "__rxProgressHandler";
        case rollbackHook :
            return "__rxRollbrackHook";
        case traceHook :
            return "__rxTraceHook";
        case updateHook :
            return "__rxUpdateHook";
        case execCallbackHook :
        default :
            // Can't happen ...
            return "";
    }
}

inline static RexxBufferObject getCallbackVar(RexxMethodContext *c, CallbackType cb)
{
    return (RexxBufferObject)c->GetObjectVariable(getCallbackVarName(cb));
}

inline static void setCallbackVar(RexxMethodContext *c, RexxBufferObject buf, CallbackType cb)
{
    c->SetObjectVariable(getCallbackVarName(cb), buf);
}

/**
 * Cleans up the Rexx side of one of the callback functions in SQLite.
 *
 * The callback data struct is set in a Rexx buffer object so that the
 * interpreter will handle freeing the memory for the stuct.  To prevent garbage
 * collection, the Rexx buffer is set in a object variable.
 *
 * 1.) When (if) the callback is removed the Rexx buffer object should be
 * removed from the object variable so it will be garbage collected, if needed.
 *
 * 2.) The callback data struct may have a global reference for the userData,
 * which should be released so it will be garbage collected, if needed.
 *
 * @param c   Rexx method context we are operating in.  Assumed to be, and
 *            should be, an ooSQLiteDB method.
 *
 * @param cb  Identifies the callback type so that the object variable can be
 *            retrieved.
 */
static void cleanupCallback(RexxMethodContext *c, CallbackType cb)
{
    RexxBufferObject cbcBuffer = getCallbackVar(c, cb);

    if ( cbcBuffer != NULLOBJECT )
    {
        CSTRING varName = getCallbackVarName(cb);

        pCGenericCallback cbc = (pCGenericCallback)c->BufferData(cbcBuffer);

        if ( cbc->userData != NULL )
        {
            c->ReleaseGlobalReference(cbc->userData);
        }
        c->DropObjectVariable(varName);
    }
}


/**
 * Cleans up all the Rexx callback stuff for a database connection.  Called when
 * the database connection is closed, or at uninit()
 *
 * @param c Rexx method context we are operating in.  Assumed to be, and should
 *          be, an ooSQLiteDB method.
 */
static void cleanupCallbacks(RexxMethodContext *c)
{
    cleanupCallback(c, authorizer);
    cleanupCallback(c, busyHandler);
    cleanupCallback(c, commitHook);
    cleanupCallback(c, profileHook);
    cleanupCallback(c, progressHandler);
    cleanupCallback(c, rollbackHook);
    cleanupCallback(c, traceHook);
    cleanupCallback(c, updateHook);
}


/**
 * The generic callback buffer may contain a user data object.  If it does, that
 * object has a global reference that should be released.  The release is done
 * here.
 *
 * @param c
 * @param buf
 */
static void cleanupThisCallback(RexxThreadContext *c, RexxBufferObject buf)
{
    if ( buf != NULLOBJECT && buf != TheNilObj )
    {
        pCGenericCallback cbc = (pCGenericCallback)c->BufferData(buf);

        if ( cbc->userData != NULL )
        {
            c->ReleaseGlobalReference(cbc->userData);
        }
    }
}


/**
 *  Methods for the .ooSQL class.
 */
#define OOSQL_CLASS    "ooSQL"

/** ooSQL::merge()  [Class method]
 *
 *
 */
RexxMethod1(wholenumber_t, oosqlC_merge_cls, ARGLIST, args)
{
    return genMerge(context->threadContext, args);
}


/**
 *  Methods for the .ooSQLite class.
 */
#define OOSQLITE_CLASS    "ooSQLite"

/** ooSQLite::version  [class method]
 *
 *  @param type  [optional]  The style of the version output returned.  Keywords
 *               are, but only 1st letter is required:
 *
 *                 Compact
 *                 Full
 *                 OneLine
 *
 *                 LibVersion
 *                 Number (for libversion number)
 *                 SourceID
 *
 *               The defualt is OneLine
 *
 */
RexxMethod1(RexxObjectPtr, oosql_version_cls, OPTIONAL_CSTRING, type)
{
    if ( argumentExists(1) )
    {
        switch ( toupper(*type) )
        {

            case 'O' :
                return genGetVersion(context->threadContext, FALSE, FALSE);
                break;

            case 'L' :
                return context->String(sqlite3_libversion());
                break;

            case 'N' :
                return context->Int32(sqlite3_libversion_number());
                break;

            case 'S' :
                return context->String(sqlite3_sourceid());
                break;

            case 'F' :
                return genGetVersion(context->threadContext, TRUE, FALSE);
                break;

            case 'C' :
                return genGetVersion(context->threadContext, FALSE, TRUE);
                break;

            default :
                wrongArgOptionException(context->threadContext, 1, VALID_VERSION_TYPES, type);
                return TheZeroObj;
                break;
        }
    }

    return genGetVersion(context->threadContext, FALSE, FALSE);
}


/** ooSQLite::compileOptionGet  [class method]
 *
 *
 *  @note  Returns "NULL" for the the null pointer return from sqlite.
 */
RexxMethod1(RexxObjectPtr, oosql_compileOptionGet_cls, int, index)
{
    return safeRexxString(context->threadContext, sqlite3_compileoption_get(index));
}


/** ooSQLite::compileOptionUsed  [class method]
 *
 *
 */
RexxMethod1(logical_t, oosql_compileOptionUsed_cls, CSTRING, name)
{
    return sqlite3_compileoption_used(name);
}


/** ooSQLite::complete()  [class method]
 *
 *
 */
RexxMethod1(logical_t, oosql_complete_cls, CSTRING, sql)
{
    return sqlite3_complete(sql);
}

/** ooSQLite::libVersion()  [class method]
 *
 *
 */
RexxMethod0(RexxObjectPtr, oosql_libVersion_cls)
{
    return context->String(sqlite3_libversion());
}

/** ooSQLite::libVersionNumber()  [class method]
 *
 *
 */
RexxMethod0(RexxObjectPtr, oosql_libVersionNumber_cls)
{
    return context->Int32(sqlite3_libversion_number());
}

/** ooSQLite::memoryHighWater()
 *
 *
 */
RexxMethod1(int64_t, oosql_memoryHighWater_cls, logical_t, reset)
{
    return sqlite3_memory_highwater((int)reset);
}

/** ooSQLite::memoryUsed()
 *
 *
 */
RexxMethod0(int64_t, oosql_memoryUsed_cls)
{
    return sqlite3_memory_used();
}

/** ooSQLite::releaseMemory()
 *
 *
 */
RexxMethod1(int, oosql_releaseMemory_cls, int, nBytes)
{
    return sqlite3_release_memory(nBytes);
}

/** ooSQLite::softHeapLimit64()
 *
 *  Imposes a limit on the heap size, or queries the current size.
 *
 *  @param n  A 64-bit signed whole number.  If n is positive the soft heap
 *            limit is set to its value.  If n is zero then the soft heap limit
 *            is disabled.  If n is negative then no change is made to the
 *            current limit.
 *
 *  @return  Returns the soft heap limit before the method was invoked, or a
 *           negative number on error.
 *
 *  @note  Use a negative n to query the current size.
 *
 */
RexxMethod1(int64_t, oosql_softHeapLimit64_cls, int64_t, n)
{
    return sqlite3_soft_heap_limit64(n);
}

/** ooSQLite::sourceID()
 *
 *
 */
RexxMethod0(RexxObjectPtr, oosql_sourceID_cls)
{
    return context->String(sqlite3_sourceid());
}

/** ooSQLite::sqlite3Version()
 *
 *
 */
RexxMethod0(RexxStringObject, oosql_sqlite3Version_cls)
{
    return context->String(sqlite3_version);
}

/** ooSQLite::status()
 *
 *
 */
RexxMethod3(int, oosql_status_cls, int, param, RexxObjectPtr, _result, OPTIONAL_logical_t, reset)
{
    return genStatus(context->threadContext, param, _result, reset);
}

/** ooSQLite::threadSafe()
 *
 *
 */
RexxMethod0(logical_t, oosql_threadSafe_cls)
{
    return sqlite3_threadsafe();
}


/**
 * Transitory method used to test various things as they come up.
 *
 */
RexxMethod1(POINTER, oosql_test_cls, POINTER, db)
{
    sqlite3_mutex *mtx = sqlite3_db_mutex((sqlite3 *)db);
    printf("db            mutex=%p\n", mtx);

    mtx = sqlite3_mutex_alloc(SQLITE_MUTEX_STATIC_MASTER);
    printf("static master mutex=%p\n", mtx);

    mtx = sqlite3_mutex_alloc(SQLITE_MUTEX_STATIC_MEM);
    printf("static mem    mutex=%p\n", mtx);

    mtx = sqlite3_mutex_alloc(SQLITE_MUTEX_STATIC_MEM2);
    printf("static mem2   mutex=%p\n", mtx);

    mtx = sqlite3_mutex_alloc(SQLITE_MUTEX_STATIC_LRU);
    printf("static lru    mutex=%p\n", mtx);

    mtx = sqlite3_mutex_alloc(SQLITE_MUTEX_STATIC_LRU2);
    printf("static lru2   mutex=%p\n", mtx);

    mtx = sqlite3_mutex_alloc(SQLITE_MUTEX_STATIC_PRNG);
    printf("static prng   mutex=%p\n", mtx);

    mtx = sqlite3_mutex_alloc(SQLITE_MUTEX_STATIC_PMEM);
    printf("static pmem   mutex=%p\n", mtx);

    mtx = sqlite3_db_mutex((sqlite3 *)db);
    printf("db            mutex=%p\n", mtx);

    return db;
}


/**
 *  Methods for the .ooSQLiteDB class.
 */
#define OOSQLITEDB_CLASS    "ooSQLiteDB"
#define STMTSET_ATTRIBUTE   "ooSQLiteDBStmtBag"


/**
 * This function ensures that any unfinalized prepared statements for the
 * specified database connection are finalized.
 *
 * Before a SQLite database connection can be closed, all prepared statements
 * for that specific connection must be finalized.
 *
 * ooSQLite keeps track of all prepared statements, that have not been
 * finalized, for any open database connection.  During the uninit() method for
 * an ooSQLiteDB object, if the connection has not been closed, it is closed.
 * At this time, if there are any unfinalized prepared statements, they are
 * finalized so that the database connection will close.
 *
 * @param c
 * @param pCdb
 */
static void ensureFinalized(RexxMethodContext *c, pCooSQLiteDB pCdb)
{
    CRITICAL_SECTION_ENTER

    RexxArrayObject stmts = (RexxArrayObject)c->SendMessage0(pCdb->stmtBag, "ALLITEMS");
    size_t          count = c->ArrayItems(stmts);

    for ( size_t i = 1; i <= count; i++ )
    {
        RexxObjectPtr  stmt   = c->ArrayAt(stmts, i);
        pCooSQLiteStmt pCstmt = stmtToCSelf(c, stmt);

        sqlite3_finalize(pCstmt->stmt);

        pCstmt->stmt      = NULL;
        pCstmt->db        = NULL;
        pCstmt->tail      = NULL;
        pCstmt->finalized = true;

        c->SendMessage1(pCdb->stmtBag, "REMOVE", stmt);
    }

    CRITICAL_SECTION_LEAVE
}


/**
 * Find the ooSQLiteStmt object for the specified prepared statement and return
 * it.  Returns .nil if not found.
 *
 * @param c
 * @param pCdb
 * @param stmt
 *
 * @return RexxObjectPtr
 */
static RexxObjectPtr findStatement(RexxMethodContext *c, pCooSQLiteDB pCdb, sqlite3_stmt *stmt)
{
    CRITICAL_SECTION_ENTER

    RexxObjectPtr   result = TheNilObj;
    RexxArrayObject stmts  = (RexxArrayObject)c->SendMessage0(pCdb->stmtBag, "ALLITEMS");
    size_t          count  = c->ArrayItems(stmts);

    for ( size_t i = 1; i <= count; i++ )
    {
        RexxObjectPtr  _stmt   = c->ArrayAt(stmts, i);
        pCooSQLiteStmt pCstmt = stmtToCSelf(c, _stmt);

        if ( pCstmt->stmt == stmt )
        {
            result = _stmt;
            break;
        }
    }

    CRITICAL_SECTION_LEAVE

    return result;
}


/**
 * Saves the database file name in an object variable of an ooSQLiteDB object
 * when the object is instantiated.
 *
 * @param c
 * @param pCdb
 * @param file
 */
static void setDBInitStatusFile(RexxMethodContext *c, pCooSQLiteDB pCdb, CSTRING file)
{
    RexxStringObject rxName = c->String(file);
    c->SetObjectVariable("rxFileName", rxName);

    pCdb->rxFileName = rxName;
    pCdb->fileName   = file;
}

/**
 * Provides the final initialization of an ooSQLiteDB object during
 * instantiation.  This initialization is dependent on whether the underlying
 * SQLite database connection was opened with or without error.
 *
 * @param c
 * @param pCdb
 * @param db
 * @param rc
 * @param self
 *
 * @return bool
 */
static bool setDBInitStatus(RexxMethodContext *c, pCooSQLiteDB pCdb, sqlite3 *db, int rc, RexxObjectPtr self)
{
    bool result = true;
    if ( rc )
    {
        pCdb->lastErrCode = sqlite3_errcode(db);
        pCdb->lastErrMsg  = c->String(sqlite3_errmsg(db));
        pCdb->initCode    = rc;

        sqlite3_close(pCdb->db);
        pCdb->db = NULL;

        result = false;
    }
    else
    {
        pCdb->lastErrMsg  = c->String("No error");
        pCdb->rexxSelf    = self;

        RexxObjectPtr set = rxNewSet(c);
        pCdb->stmtBag = set;
        c->SendMessage1(self, "OOSQLITEDBSTMTBAG=", set);

        // Here we enable things that are enabled / disabled on a per database
        // connection basis that are set by default for ooSQLite.
        sqlite3_extended_result_codes(pCdb->db, 1);
    }

    c->SetObjectVariable("rxLastErrMsg", pCdb->lastErrMsg);

    return result;
}


static CSTRING defaultCallbackMethod(CallbackType cbt)
{
    if ( cbt == authorizer )
    {
        return "AUTHORIZERCALLBACK";   // authorizerCallback()
    }
    if ( cbt == busyHandler )
    {
        return "BUSYCALLBACK";         // busyCallback()
    }
    else if ( cbt == commitHook )
    {
        return "COMMITHOOKCALLBACK";   // commitHookCallback()
    }
    else if ( cbt == profileHook )
    {
        return "PROFILECALLBACK";      // profileCallback()
    }
    else if ( cbt == progressHandler )
    {
        return "PROGRESSCALLBACK";     // progressCallback()
    }
    else if ( cbt == rollbackHook )
    {
        return "ROLLBACKHOOKCALLBACK"; // rollbackHookCallback()
    }
    else if ( cbt == traceHook )
    {
        return "TRACECALLBACK";        // traceCallback()
    }
    else
    {
        return "UPDATEHOOKCALLBACK";   // updateHookCallback()
    }
}


static RexxObjectPtr removeCallback(RexxMethodContext *c, sqlite3 *db, CallbackType cbt)
{
    RexxObjectPtr     result = TheNilObj;
    pCGenericCallback cbc    = NULL;
    int               rc     = SQLITE_OK;

    if ( cbt == authorizer )
    {
        rc = sqlite3_set_authorizer(db, NULL, NULL);
        result = c->WholeNumber(rc);
    }
    else if ( cbt == busyHandler )
    {
        rc = sqlite3_busy_handler(db, NULL, NULL);
        result = c->WholeNumber(rc);
    }
    else if ( cbt == commitHook )
    {
        cbc = (pCGenericCallback)sqlite3_commit_hook(db, NULL, NULL);
    }
    else if ( cbt == profileHook )
    {
        cbc = (pCGenericCallback)sqlite3_profile(db, NULL, NULL);
    }
    else if ( cbt == progressHandler )
    {
        sqlite3_progress_handler(db, 0, NULL, NULL);
        result = c->WholeNumber(SQLITE_OK);
    }
    else if ( cbt == rollbackHook )
    {
        cbc = (pCGenericCallback)sqlite3_rollback_hook(db, NULL, NULL);
    }
    else if ( cbt == traceHook )
    {
        cbc = (pCGenericCallback)sqlite3_trace(db, NULL, NULL);
    }
    else
    {
        cbc = (pCGenericCallback)sqlite3_update_hook(db, NULL, NULL);
    }

    if ( cbc != NULL )
    {
        if ( cbc->userData != NULL )
        {
            result = cbc->userData;
        }
    }

    cleanupCallback(c, cbt);

    return result;
}


static RexxObjectPtr installCallback(RexxMethodContext *c, sqlite3 *db, pCGenericCallback cbc,
                                     CallbackType cbt, RexxBufferObject oldCallback, int instructions)
{
    RexxObjectPtr     result = TheNilObj;
    pCGenericCallback oldCbc = NULL;
    int               rc     = SQLITE_OK;

    if ( cbt == authorizer )
    {
        rc = sqlite3_set_authorizer(db, authorizerCallback, (void *)cbc);
        result = c->WholeNumber(rc);
    }
    else if ( cbt == busyHandler )
    {
        rc = sqlite3_busy_handler(db, busyCallBack, (void *)cbc);
        result = c->WholeNumber(rc);
    }
    else if ( cbt == commitHook )
    {
        oldCbc = (pCGenericCallback)sqlite3_commit_hook(db, commitHookCallback, (void *)cbc);
    }
    else if ( cbt == profileHook )
    {
        oldCbc = (pCGenericCallback)sqlite3_profile(db, profileHookCallback, (void *)cbc);
    }
    else if ( cbt == progressHandler )
    {
        sqlite3_progress_handler(db, instructions, progressCallback, (void *)cbc);
        result = c->WholeNumber(SQLITE_OK);
    }
    else if ( cbt == rollbackHook )
    {
        oldCbc = (pCGenericCallback)sqlite3_rollback_hook(db, rollbackHookCallback, (void *)cbc);
    }
    else if ( cbt == traceHook )
    {
        oldCbc = (pCGenericCallback)sqlite3_trace(db, traceHookCallback, (void *)cbc);
    }
    else
    {
        oldCbc = (pCGenericCallback)sqlite3_update_hook(db, updateHookCallback, (void *)cbc);
    }

    if ( oldCbc != NULL )
    {
        if ( oldCbc->userData != NULL )
        {
            result = oldCbc->userData;
        }
    }

    cleanupThisCallback(c->threadContext, oldCallback);

    return result;
}


/**
 * Does the basic setup, which is the same, for one of the 3 SQLite provided
 * callback registrations: sqlite3_commit_hook(), sqlite3_rollback_hook(), or
 * sqlite3_update_hook().
 *
 * @param c
 * @param db
 * @param callbackObj
 * @param mthName
 * @param userData
 * @param cbt        Identifies which callback registration this is for.
 *
 * @return RexxObjectPtr
 */
static RexxObjectPtr doCallbackSetup(RexxMethodContext *c, sqlite3 *db, RexxObjectPtr callbackObj,
                                     CSTRING mthName, RexxObjectPtr userData, CallbackType cbt, int instructions)
{
    if ( callbackObj == TheNilObj )
    {
        return removeCallback(c, db, cbt);
    }

    RexxBufferObject oldCallback = getCallbackVar(c, cbt);
    RexxBufferObject cbcBuffer   = c->NewBuffer(sizeof(CGenericCallback));
    if ( cbcBuffer == NULLOBJECT )
    {
        outOfMemoryException(c->threadContext);
        return c->WholeNumber(SQLITE_MISUSE);
    }

    setCallbackVar(c, cbcBuffer, cbt);

    pCGenericCallback cbc = (pCGenericCallback)c->BufferData(cbcBuffer);
    memset(cbc, 0, sizeof(CGenericCallback));

    cbc->callbackObj     = callbackObj;
    cbc->callbackContext = c->threadContext;

    if ( mthName != NULL )
    {
        cbc->callbackMethod = mthName;
    }
    else
    {
        cbc->callbackMethod = defaultCallbackMethod(cbt);
    }

    if ( userData != NULLOBJECT )
    {
        cbc->userData = c->RequestGlobalReference(userData);
    }

    return installCallback(c, db, cbc, cbt, oldCallback, instructions);
}

/**
 * Returns the proper PragmaType enum for the specified name.
 *
 * @param c
 * @param name
 *
 * @return PragmaType
 *
 * @note  A few of the pragmas can have a database qualifier, i.e.
 *        main.wal_checkpoint.  We check for the '.' and always compare just
 *        the pragma name part of the input string.  According to the SQLite
 *        docs, pragmas can be added, changed, dropped from version to version,
 *        so future versions could have more pragmas that use a qualifier.
 */
PragmaType getPragmaType(RexxThreadContext *c, CSTRING name)
{
    const char *pName = strchr(name, '.');

    if ( pName == NULL )
    {
        pName = name;
    }
    else if ( strlen(pName) > 1 )
    {
        ++pName;
    }

    if ( strcmp(pName, "auto_vacuum")               == 0 ) return autoVacuum;
    if ( strcmp(pName, "automatic_index")           == 0 ) return automaticIndex;
    if ( strcmp(pName, "cache_size")                == 0 ) return cacheSize;
    if ( strcmp(pName, "case_sensitive_like")       == 0 ) return caseSensitiveLike;
    if ( strcmp(pName, "checkpoint_fullfsync")      == 0 ) return checkpointFullfsync;
    if ( strcmp(pName, "collation_list")            == 0 ) return collationList;
    if ( strcmp(pName, "compile_options")           == 0 ) return compileOptions;
    if ( strcmp(pName, "database_list")             == 0 ) return databaseList;
    if ( strcmp(pName, "encoding")                  == 0 ) return encoding;
    if ( strcmp(pName, "foreign_key_list")          == 0 ) return foreignKeyList;
    if ( strcmp(pName, "foreign_keys")              == 0 ) return foreignKeys;
    if ( strcmp(pName, "freelist_count")            == 0 ) return freelistCount;
    if ( strcmp(pName, "fullfsync")                 == 0 ) return fullfsync;
    if ( strcmp(pName, "ignore_check_constraints")  == 0 ) return ignoreCheckConstraints;
    if ( strcmp(pName, "incremental_vacuum")        == 0 ) return incrementalVacuum;
    if ( strcmp(pName, "index_info")                == 0 ) return indexInfo;
    if ( strcmp(pName, "index_list")                == 0 ) return indexList;
    if ( strcmp(pName, "integrity_check")           == 0 ) return integrityCheck;
    if ( strcmp(pName, "journal_mode")              == 0 ) return journalMode;
    if ( strcmp(pName, "journal_size_limit")        == 0 ) return journalSizeLimit;
    if ( strcmp(pName, "legacy_file_format")        == 0 ) return legacyFileFormat;
    if ( strcmp(pName, "locking_mode")              == 0 ) return lockingMode;
    if ( strcmp(pName, "max_page_count")            == 0 ) return maxPageCount;
    if ( strcmp(pName, "page_count")                == 0 ) return pageCount;
    if ( strcmp(pName, "page_size")                 == 0 ) return pageSize;
    if ( strcmp(pName, "quick_check")               == 0 ) return quickCheck;
    if ( strcmp(pName, "read_uncommitted")          == 0 ) return readUncommitted;
    if ( strcmp(pName, "recursive_triggers")        == 0 ) return recursiveTriggers;
    if ( strcmp(pName, "reverse_unordered_selects") == 0 ) return reverseUnorderedSelects;
    if ( strcmp(pName, "schema_version")            == 0 ) return schemaVersion;
    if ( strcmp(pName, "secure_delete")             == 0 ) return secureDelete;
    if ( strcmp(pName, "shrink_memory")             == 0 ) return shrinkMemory;
    if ( strcmp(pName, "synchronous")               == 0 ) return synchronous;
    if ( strcmp(pName, "table_info")                == 0 ) return tableInfo;
    if ( strcmp(pName, "temp_store")                == 0 ) return tempStore;
    if ( strcmp(pName, "user_version")              == 0 ) return userVersion;
    if ( strcmp(pName, "wal_autocheckpoint")        == 0 ) return walAutocheckpoint;
    if ( strcmp(pName, "wal_checkpoint")            == 0 ) return walCheckpoint;
    if ( strcmp(pName, "writable_schema")           == 0 ) return writableSchema;

  userDefinedMsgException(c, 1, "is not a recognized pragma");
  return unknownPragma;
}

/**
 * Processes a pragma that triggers an action.
 *
 * integrityCheck -> Returns a result set array that has a single column.
 *
 * quickCheck     -> Returns a result set array that has a single column.
 *
 * shrinkMemory   -> There is no return from sqlite for this pragma so the
 *                   return is the Ok return code
 *
 * walCheckpoint  -> Returns a result set array that has a single row.
 *
 * @param c
 * @param pCdb
 * @param name
 * @param pragma
 *
 * @return A Rexx object as described above on success, or an error return code
 *         with message.
 */
RexxObjectPtr pragmaTrigger(RexxMethodContext *c, pCooSQLiteDB pCdb, CSTRING name, RexxObjectPtr value, PragmaType pragma)
{
    char     buf[256];
    sqlite3 *db = pCdb->db;

    if ( value == NULLOBJECT )
    {
        snprintf(buf, sizeof(buf), "PRAGMA %s;", name);
    }
    else
    {
        snprintf(buf, sizeof(buf), "PRAGMA %s(%s);", name, c->ObjectToStringValue(value));
    }

    RexxObjectPtr result;
    sqlite3_stmt *stmt;

    int rc = sqlite3_prepare_v2(db, buf, (int)strlen(buf) + 1, &stmt, NULL);

    if ( rc == SQLITE_OK )
    {
        if ( pragma == shrinkMemory )
        {
            rc = sqlite3_step(stmt);
            if ( rc == SQLITE_DONE )
            {
                result = c->WholeNumber(SQLITE_OK);
            }
            else
            {
                result = ooSQLiteErr(c, pCdb, rc, sqlite3_errmsg(db), true);
            }

        }
        else
        {
            result = getRecords(c->threadContext, stmt);
        }
    }
    else
    {
        result = ooSQLiteErr(c, pCdb, rc, sqlite3_errmsg(db), true);
    }

    sqlite3_finalize(stmt);

    return result;
}

/**
 * Processes a query only pragma that returns an array of records.
 *
 * @param c
 * @param pCdb
 * @param name
 * @param value
 * @param pragma
 *
 * @return An array of records.
 *
 * @note  If there are no rows returned by the pragma statement, then an empty
 *        array is returned.
 */
RexxObjectPtr pragmaList(RexxMethodContext *c, pCooSQLiteDB pCdb, CSTRING name, RexxObjectPtr value, PragmaType pragma)
{
    char     buf[256];
    sqlite3 *db = pCdb->db;

    if ( pragma == collationList || pragma == databaseList || pragma == compileOptions )
    {
        snprintf(buf, sizeof(buf), "PRAGMA %s;", name);
    }
    else
    {
        if ( value == NULLOBJECT )
        {
            return missingArgException(c->threadContext, 2);
        }
        snprintf(buf, sizeof(buf), "PRAGMA %s(%s);", name, c->ObjectToStringValue(value));
    }

    RexxObjectPtr result;
    sqlite3_stmt *stmt;

    int rc = sqlite3_prepare_v2(db, buf, (int)strlen(buf) + 1, &stmt, NULL);
    if ( rc == SQLITE_OK )
    {
        result = getRecords(c->threadContext, stmt);
    }
    else
    {
        result = ooSQLiteErr(c, pCdb, rc, sqlite3_errmsg(db), true);
    }

    sqlite3_finalize(stmt);

    return result;
}

/**
 * Processes a pragma that is expected to return a single value.
 *
 * @param c
 * @param pCdb
 * @param name
 * @param pragma
 *
 * @return RexxObjectPtr
 */
RexxObjectPtr pragmaGet(RexxMethodContext *c, pCooSQLiteDB pCdb, CSTRING name, PragmaType pragma)
{
    char buf[128];
    snprintf(buf, sizeof(buf), "PRAGMA %s;", name);

    sqlite3_stmt *stmt;
    sqlite3      *db = pCdb->db;

    int rc = sqlite3_prepare_v2(db, buf, (int)strlen(buf) + 1, &stmt, NULL);

    if ( rc != SQLITE_OK )
    {
        return ooSQLiteErr(c, pCdb, rc, sqlite3_errmsg(db), true);
    }

    RexxObjectPtr result;

    rc = sqlite3_step(stmt);
    printf("pragma=%s rc=%d col count=%d\n", name, rc, sqlite3_column_count(stmt));
    if ( rc == SQLITE_ROW && sqlite3_column_count(stmt) == 1 )
    {
        result = c->String(safeColumnText(stmt, 0));
    }
    else
    {
        snprintf(buf, sizeof(buf), "Unexpected result, no data returned from pragma %s", name);
        result = ooSQLiteErr(c, pCdb, OO_UNEXPECTED_RESULT, buf, false);
    }

    sqlite3_finalize(stmt);

    return result;
}


/**
 * Processes a pragma that is setting a value
 *
 * @param c
 * @param pCdb
 * @param name
 * @param value
 * @param pragma
 *
 * @return RexxObjectPtr
 *
 * @note  We are only called internally, so we know for sure that 'value' is not
 *        the NULLOBJECT.
 */
RexxObjectPtr pragmaSet(RexxMethodContext *c, pCooSQLiteDB pCdb, CSTRING name, RexxObjectPtr value, PragmaType pragma)
{
    char     buf[256];
    sqlite3 *db = pCdb->db;

    snprintf(buf, sizeof(buf), "PRAGMA %s(%s);", name, c->ObjectToStringValue(value));

    sqlite3_stmt *stmt;

    int rc = sqlite3_prepare_v2(db, buf, (int)strlen(buf) + 1, &stmt, NULL);
    if ( rc != SQLITE_OK )
    {
        return ooSQLiteErr(c, pCdb, rc, sqlite3_errmsg(db), true);
    }

    RexxObjectPtr result = TheZeroObj;

    rc = sqlite3_step(stmt);
    printf("pragma=%s rc=%d col count=%d\n", name, rc, sqlite3_column_count(stmt));
    if ( rc != SQLITE_DONE )
    {
        if ( rc == SQLITE_ROW )
        {
            // This is unexpected to me, but maybe it is okay?
            snprintf(buf, sizeof(buf), "Unexpected result, data returned from pragma %s", name);
            result = ooSQLiteErr(c, pCdb, OO_UNEXPECTED_RESULT, buf, false);
        }
        else
        {
            return ooSQLiteErr(c, pCdb, rc, sqlite3_errmsg(db), true);
        }
    }

    sqlite3_finalize(stmt);

    return result;

}

/** ooSQLiteDB:Class::init()
 */
RexxMethod1(RexxObjectPtr, oosqldb_init_cls, OSELF, self)
{
    if ( isOfClassType(context, self, OOSQLITEDB_CLASS) )
    {
        TheOOSQLiteDBClass = (RexxClassObject)self;
        context->RequestGlobalReference(TheOOSQLiteDBClass);
    }
    return NULLOBJECT;
}


/** ooSQLiteDB::closed  [attribute get]
 */
RexxMethod1(logical_t, oosqldb_getClosed_atr, CSELF, pCSelf)
{
    pCooSQLiteDB pCdb = requiredDBCSelf(context, pCSelf);
    if ( pCdb == NULL )
    {
        return 1;
    }
    return pCdb->closed;
}

/** ooSQLiteDB::fileName  [attribute get]
 */
RexxMethod1(RexxStringObject, oosqldb_getFileName_atr, CSELF, pCSelf)
{
    pCooSQLiteDB pCdb = requiredDBCSelf(context, pCSelf);
    if ( pCdb == NULL )
    {
        return NULLOBJECT;
    }
    return pCdb->rxFileName;
}

/** ooSQLiteDB::initCode  [attribute get]
 */
RexxMethod1(int, oosqldb_getInitCode_atr, CSELF, pCSelf)
{
    pCooSQLiteDB pCdb = requiredDBCSelf(context, pCSelf);
    if ( pCdb == NULL )
    {
        return -1;
    }
    return pCdb->initCode;
}

/** ooSQLiteDB::lastErrCode  [attribute get]
 */
RexxMethod1(int, oosqldb_getLastErrCode_atr, CSELF, pCSelf)
{
    pCooSQLiteDB pCdb = requiredDBCSelf(context, pCSelf);
    if ( pCdb == NULL )
    {
        return -1;
    }
    return pCdb->lastErrCode;
}

/** ooSQLiteDB::lastErrMsg  [attribute get]
 */
RexxMethod1(RexxStringObject, oosqldb_getLastErrMsg_atr, CSELF, pCSelf)
{
    pCooSQLiteDB pCdb = requiredDBCSelf(context, pCSelf);
    if ( pCdb == NULL )
    {
        return NULLOBJECT;
    }
    return pCdb->lastErrMsg;
}

/** ooSQLiteDB::init()
 *
 *  @param file     [required]  Name of the database for this sqlite database
 *                              connection.
 *
 *  @param flags    [optional]  Flags for opening the database.  Use the .ooSQL
 *                              constants.
 *
 *  @param vfsName  [optional]  The name of the sqlite3_vfs object that defines
 *                              the operating system interface that the new
 *                              database connection should use.  We don't check
 *                              this yet, so the sqlitee default is always used.
 *
 *
 */
RexxMethod4(RexxObjectPtr, oosqldb_init, CSTRING, file, OPTIONAL_int32_t, _flags, OPTIONAL_CSTRING, vfsName, OSELF, self)
{

    // Get a buffer for the ooSQLiteDB CSelf.
    RexxBufferObject cselfBuffer = context->NewBuffer(sizeof(CooSQLiteDB));
    if ( cselfBuffer == NULLOBJECT )
    {
        outOfMemoryException(context->threadContext);
        return NULLOBJECT;
    }

    context->SetObjectVariable("CSELF", cselfBuffer);

    pCooSQLiteDB pCdb = (pCooSQLiteDB)context->BufferData(cselfBuffer);
    memset(pCdb, 0, sizeof(CooSQLiteDB));

    int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    if ( argumentExists(2) )
    {
        flags = _flags;
    }

    setDBInitStatusFile(context, pCdb, file);

    int rc = sqlite3_open_v2(file, &pCdb->db, flags, NULL);

    setDBInitStatus(context, pCdb, pCdb->db, rc, self);

    return NULLOBJECT;
}

/** ooSQLiteDB::uninit()
 *
 *  At this point, the only purpose of uninit() is to ensure that an open
 *  database connection is closed.  There is no allocated memory in the
 *  CooSQLiteDB struct that needs to be freed, which could change in the future.
 *
 *  If we attempt to close the connection and get the busy return, we call
 *  interrupt().  The SQLite doc makes this sound a little dangerous if the
 *  database should close on another thread before the call to interrupt
 *  returns, something to keep in mind.
 */
RexxMethod1(RexxObjectPtr, oosqldb_uninit, CSELF, pCSelf)
{
    if ( pCSelf != NULLOBJECT )
    {
        pCooSQLiteDB pCdb = (pCooSQLiteDB)pCSelf;

#ifdef OOSQLDBG
        printf("ooSQLiteDB::uninit() database connection=%p closed=%d\n", pCdb->db, pCdb->closed);
#endif
        if ( ! pCdb->closed && pCdb->db != NULL )
        {
            cleanupCallbacks(context);

            ensureFinalized(context, pCdb);

            int rc = sqlite3_close(pCdb->db);

            if ( rc == SQLITE_OK )
            {
#ifdef OOSQLDBG
                printf("ooSQLiteDB::uninit() did close database first try\n");
#endif

                pCdb->db = NULL;
                pCdb->closed = true;
                return TheZeroObj;
            }

            // Not sure what to do if rc != SQLITE_BUSY ...
            if ( rc == SQLITE_BUSY )
            {
                sqlite3_interrupt(pCdb->db);

                size_t i = 0;
                do
                {
                    sqlite3_sleep(100);
                    rc = sqlite3_close(pCdb->db);
                } while ( rc == SQLITE_BUSY && ++i <= 10);
            }

            if ( rc == SQLITE_OK )
            {
                pCdb->db = NULL;
                pCdb->closed = true;
            }
            else
            {
                printf("ooSQLiteDB::uninit() database connection not closed successfully. rc=%d\n", rc);
            }
        }
    }

    return TheZeroObj;
}


/** ooSQLiteDB::busyHandler()
 *
 *  Installs a user defined busy handler.
 *
 *  @param  callbackObj  [required]  An instantiated class object with a method
 *                       that may be invoked whenever an attempt is made to open
 *                       a database table that another thread or process has
 *                       locked.
 *
 *                       However, this argument can also be .nil to indicate
 *                       that any installed busy handler is to be removed.  When
 *                       no busy handler is installed then SQLITE_BUSY or
 *                       SQLITE_IOERR_BLOCKED is returned immediately upon
 *                       encountering the lock
 *
 *  @param  mthName      [optional]  The method name that will be invoked during
 *                       a call back.  By default, the method invoked will be
 *                       busyCallBack().  However, the user can specify an
 *                       alternative method if desired.  This argument is
 *                       ignored when the callbackObj argument is .nil.
 *
 *  @param userData      [optional] This can be any Rexx object the user
 *                       desires. The object will be sent as the second argument
 *                       to the busy callback method when it is invoked.  This
 *                       argument is ignored when  the callbackObj argument is
 *                       .nil.
 *
 *  @return  A sqlite result code.
 *
 *  @notes  By default, there is no busy handler installed.
 *
 *          There can only be one busy handler installed.  Setting a new busy
 *          handler automatically clears any previously installed handler.  Note
 *          that invoking ooSQLiteDB::busyTimeOut() can also set or clear the
 *          busy handler.
 *
 *          The busy handler should not take any actions which modify the
 *          database connection that invoked the busy handler. Any such actions
 *          result in undefined behavior.
 *
 *          A busy handler must not close the database connection or prepared
 *          statement that invoked the busy handler.
 */
RexxMethod4(RexxObjectPtr, oosqldb_busyHandler, RexxObjectPtr, callbackObj, OPTIONAL_CSTRING, mthName,
            OPTIONAL_RexxObjectPtr, userData, CSELF, pCSelf)
{
    pCooSQLiteDB pCdb = requiredDB(context, pCSelf);
    if ( pCdb == NULL )
    {
        return context->WholeNumber(SQLITE_MISUSE);
    }

    return doCallbackSetup(context, pCdb->db, callbackObj, mthName, userData, busyHandler, 0);
}

/** ooSQLiteDB::busyTimeOut()
 *
 *  @remarks sqlite3_busy_timeout() installs a SQLite timeout handler.  This has
 *           the side effect of removing any existing handler, which could have
 *           been a user registered handler.  So, we need to clean up a possible
 *           user registered handler.
 */
RexxMethod2(int, oosqldb_busyTimeOut, int, ms, CSELF, pCSelf)
{
    pCooSQLiteDB pCdb = requiredDB(context, pCSelf);
    if ( pCdb == NULL )
    {
        return SQLITE_MISUSE;
    }

    int rc = sqlite3_busy_timeout(pCdb->db, ms);
    cleanupCallback(context, busyHandler);

    return rc;
}


/** ooSQLiteDB::changes()
 *
 *
 */
RexxMethod1(int, oosqldb_changes, CSELF, pCSelf)
{
    pCooSQLiteDB pCdb = requiredDB(context, pCSelf);
    if ( pCdb == NULL )
    {
        return SQLITE_MISUSE;
    }

    return sqlite3_changes(pCdb->db);
}


/** ooSQLiteDB::close()
 *
 *  Closes the database connection for this ooSQLiteDB object.
 *
 *
 *
 *  @remarks  The SQLite doc says that the sqlite3 pointer can be null or a
 *            pointer that has not been previously closed.  Calling close with a
 *            NULL is a harmless no-op.  We want to maintain that behaviour, so
 *            we don't call it an error if the user calls close when the
 *            connection has already been closed.
 */
RexxMethod1(int, oosqldb_close, CSELF, pCSelf)
{
    int rc = SQLITE_MISUSE;

    pCooSQLiteDB pCdb = requiredDBCSelf(context, pCSelf);
    if ( pCdb != NULL )
    {
        cleanupCallbacks(context);

        rc = sqlite3_close(pCdb->db);

        if ( rc == SQLITE_OK )
        {
            pCdb->db     = NULL;
            pCdb->closed = true;
        }
        else if ( rc == SQLITE_BUSY )
        {
            printf("Database busy. Sleep here or let user deal with this??\n");
        }
    }

    return rc;
}


/** ooSQLiteDB::commitHook()
 *
 *  Registers a callback method to be invoked whenever a transaction is
 *  committed.
 *
 *  @param  callbackObj  [required]  An instantiated class object with a method
 *                       that is invoked whenever a transaction is committed.
 *
 *                       However, this argument can also be .nil to indicate
 *                       that an already registered callback is to be removed.
 *
 *  @param  mthName      [optional]  The method name that will be invoked during
 *                       the callback.  By default, the method invoked will be
 *                       commitHookCallback().  However, the user can specify an
 *                       alternative method if desired.  This argument is
 *                       ignored when the callbackObj argument is .nil.
 *
 *  @param userData      [optional] This can be any Rexx object the user
 *                       desires. The object will be sent as the first and only
 *                       argument to the callback method when it is invoked.
 *                       This argument is ignored when  the callbackObj argument
 *                       is .nil.
 *
 *  @return  The userData argument to a previous invocation of the commitHook()
 *           method on the same database connection, or .nil if there is not
 *           one.
 *
 *  @notes  When the commit hook callback method returns zero, the COMMIT
 *          operation is allowed to continue normally. If the commit hook
 *          callback returns non-zero, then the COMMIT is converted into a
 *          ROLLBACK.  The Rexx method *must* return a whole number.
 */
RexxMethod4(RexxObjectPtr, oosqldb_commitHook, RexxObjectPtr, callbackObj, OPTIONAL_CSTRING, mthName,
            OPTIONAL_RexxObjectPtr, userData, CSELF, pCSelf)
{
    pCooSQLiteDB pCdb = requiredDB(context, pCSelf);
    if ( pCdb == NULL )
    {
        return context->WholeNumber(SQLITE_MISUSE);
    }

    return doCallbackSetup(context, pCdb->db, callbackObj, mthName, userData, commitHook, 0);
}


/** ooSQLiteDB::dbFileName()
 *
 *
 */
RexxMethod2(RexxObjectPtr, oosqldb_dbFileName, CSTRING, name, CSELF, pCSelf)
{
    pCooSQLiteDB pCdb = requiredDB(context, pCSelf);
    if ( pCdb == NULL )
    {
        return context->UnsignedInt32(SQLITE_MISUSE);
    }

    return safeRexxStringRx(context->threadContext, sqlite3_db_filename(pCdb->db, name));
}


/** ooSQLiteDB::dbMutex()
 *
 *
 *
 */
RexxMethod1(RexxObjectPtr, oosqldb_dbMutex, CSELF, pCSelf)
{
    pCooSQLiteDB pCdb = requiredDB(context, pCSelf);
    if ( pCdb == NULL )
    {
        return context->WholeNumber(SQLITE_MISUSE);
    }

    sqlite3_mutex *mtx = sqlite3_db_mutex(pCdb->db);

    // mtx could be null, but we just let the ooSQLiteMutex class take care of it.
    RexxArrayObject args = context->ArrayOfThree(context->WholeNumber(SQLITE_MUTEX_RECURSIVE_DB),
                                                 context->NewPointer((POINTER)MutexMagic),
                                                 context->NewPointer((POINTER)mtx));

    return context->SendMessage(TheOOSQLiteMutexClass, "NEW", args);
}


/** ooSQLiteDB::dbReadOnly()
 *
 *
 */
RexxMethod2(int, oosqldb_dbReadOnly, CSTRING, name, CSELF, pCSelf)
{
    pCooSQLiteDB pCdb = requiredDB(context, pCSelf);
    if ( pCdb == NULL )
    {
        return SQLITE_MISUSE;
    }

    return sqlite3_db_readonly(pCdb->db, name);
}


/** ooSQLiteDB::dbReleaseMemory()
 *
 *
 *
 */
RexxMethod1(int, oosqldb_dbReleaseMemory, CSELF, pCSelf)
{
    pCooSQLiteDB pCdb = requiredDB(context, pCSelf);
    if ( pCdb == NULL )
    {
        return SQLITE_MISUSE;
    }

    return sqlite3_db_release_memory(pCdb->db);
}


/** ooSQLiteDB::dbStatus()
 *
 *
 *
 */
RexxMethod4(int, oosqldb_dbStatus, int, param, RexxObjectPtr, _result, OPTIONAL_logical_t, reset, CSELF, pCSelf)
{
    pCooSQLiteDB pCdb = requiredDB(context, pCSelf);
    if ( pCdb == NULL )
    {
        return SQLITE_MISUSE;
    }

    return genDbStatus(context->threadContext, pCdb->db, param, _result, reset, 2);
}


/** ooSQLiteDB::errCode()
 *  ooSQLiteDB::extendedErrCode()
 *
 *
 */
RexxMethod1(int, oosqldb_errCode, CSELF, pCSelf)
{
    pCooSQLiteDB pCdb = requiredDB(context, pCSelf);
    if ( pCdb == NULL )
    {
        return SQLITE_MISUSE;
    }

    return sqlite3_extended_errcode(pCdb->db);
}


/** ooSQLiteDB::errMsg()
 *
 *
 */
RexxMethod1(RexxObjectPtr, oosqldb_errMsg, CSELF, pCSelf)
{
    pCooSQLiteDB pCdb = requiredDB(context, pCSelf);
    if ( pCdb == NULL )
    {
        return context->UnsignedInt32(SQLITE_MISUSE);
    }

    return context->String(sqlite3_errmsg(pCdb->db));
}


/** ooSQLiteDB::exec()
 *
 *  Executes the specified SQL statement(s).
 *
 *  @param  sql          A string containing the SQL statement or statements to
 *                       execute. More than 1 statement can be executed by
 *                       including semi-colons in the string.  Each SQL
 *                       statement is ended with a semi-colon.
 *
 *  @param  doCallback   [optional]  Whether to use the call back feature of
 *                       sqlite3_exec(). If false, then the remaining arguments
 *                       are ignored, and the return from exec() is a return
 *                       code.
 *
 *                       If true *and* the callBackObj arg is omitted then a
 *                       result set is returned, which could be empty if the sql
 *                       does not produce a result set.
 *
 *                       Otherwise, if callBackObj is not omitted, then the call
 *                       back method of that object is invoked for each result
 *                       row coming out of the evaluated SQL statements.  In
 *                       this case the return is a return code
 *
 *  @param  callbackObj  [optional]  An instantiated class object with a method
 *                       that is invoked for each result row produced by the
 *                       evaluated SQL statements.  This argument is ignored if
 *                       doCallback is omitted or false.
 *
 *  @param  mthName      [optional]  The method name that will be invoked during
 *                       a call back.  By default, the method invoked will be
 *                       execCallBack().  However, the user can specify an
 *                       alternative method if desired.  This argument is
 *                       ignored unless doCallBack is true and callbackObj is
 *                       not omitted.
 *
 *  @param userData      [optional] This can be any Rexx object the user
 *                       desires. The object will be sent as the third argument
 *                       to the callback method when it is invoked.  This
 *                       argument is ignored when doCallBack is false.
 *
 *  @return  A result code, one of the .ooSQL constants, or a result set as
 *           described above.
 */
RexxMethod6(RexxObjectPtr, oosqldb_exec, CSTRING, sql, OPTIONAL_logical_t, doCallback,
            OPTIONAL_RexxObjectPtr, callbackObj, OPTIONAL_CSTRING, mthName, OPTIONAL_RexxObjectPtr, userData,
            CSELF, pCSelf)
{
    int rc = SQLITE_ERROR;

    pCooSQLiteDB pCdb = requiredDB(context, pCSelf);
    if ( pCdb == NULL )
    {
        return context->WholeNumber(rc);
    }

    char             *errMsg = NULL;
    CGenericCallback  cbc    = {0};

    if ( doCallback )
    {
        cbc.callbackContext = context->threadContext;

        if ( argumentExists(3) )
        {
            cbc.callbackObj = callbackObj;

            if ( argumentExists(4) )
            {
                cbc.callbackMethod = mthName;
            }
            else
            {
                cbc.callbackMethod = "EXECCALLBACK"; // execCallback()
            }

            if ( argumentExists(5) )
            {
                cbc.userData = userData;
            }
        }
        else
        {
            cbc.useArray = true;
            cbc.records  = context->NewArray(10);
        }

        rc = sqlite3_exec(pCdb->db, sql, execCallBack, (void *)&cbc, &errMsg);
    }
    else
    {
        rc = sqlite3_exec(pCdb->db, sql, NULL, NULL, &errMsg);
    }

    if ( rc != SQLITE_OK )
    {
        if ( errMsg != NULL )
        {
            printf("SQL error: %s\n", errMsg);
            ooSQLiteErr(context, pCdb, rc, errMsg, true);
            sqlite3_free(errMsg);
        }
        else
        {
            ooSQLiteErr(context, pCdb, rc, "error code, but sqlite3 did not set an error message", true);
        }
    }

    if ( cbc.useArray )
    {
        return cbc.records;
    }

    return context->WholeNumber(rc);
}


/** ooSQLiteDB::extendedResultCodes()
 *
 *  This is a noop, for the ooSQLite object orientated interface.  Extended
 *  result codes are always turned on in the init() method for each database
 *  connection.
 */
RexxMethod0(int, oosqldb_extendedResultCodes)
{
    return SQLITE_OK;
}


/** ooSQLiteDB::getAutoCommit()
 *
 *
 */
RexxMethod1(logical_t, oosqldb_getAutoCommit, CSELF, pCSelf)
{
    pCooSQLiteDB pCdb = requiredDB(context, pCSelf);
    if ( pCdb == NULL )
    {
        return SQLITE_MISUSE;
    }

    return sqlite3_get_autocommit(pCdb->db);
}


/** ooSQLiteDB::interrupt()
 *
 *
 */
RexxMethod1(int, oosqldb_interrupt, CSELF, pCSelf)
{
    pCooSQLiteDB pCdb = requiredDB(context, pCSelf);
    if ( pCdb == NULL )
    {
        return SQLITE_MISUSE;
    }

    sqlite3_interrupt(pCdb->db);

    return SQLITE_OK;
}


/** ooSQLiteDB::lastInsertRowID()
 *
 *
 */
RexxMethod1(int64_t, oosqldb_lastInsertRowID, CSELF, pCSelf)
{
    pCooSQLiteDB pCdb = requiredDB(context, pCSelf);
    if ( pCdb == NULL )
    {
        return SQLITE_MISUSE;
    }

    return sqlite3_last_insert_rowid(pCdb->db);
}


/** ooSQLiteDB::limit()
 *
 *
 */
RexxMethod3(int, oosqldb_limit, int, id, int, value, CSELF, pCSelf)
{
    pCooSQLiteDB pCdb = requiredDB(context, pCSelf);
    if ( pCdb == NULL )
    {
        return SQLITE_MISUSE;
    }

    return sqlite3_limit(pCdb->db, id, value);
}


/** ooSQLiteDB::nextStmt()
 *
 *
 */
RexxMethod2(RexxObjectPtr, oosqldb_nextStmt, OPTIONAL_RexxObjectPtr, _stmt, CSELF, pCSelf)
{
    pCooSQLiteDB pCdb = requiredDB(context, pCSelf);
    if ( pCdb == NULL )
    {
        return context->WholeNumber(SQLITE_MISUSE);
    }

    sqlite3_stmt *stmt = NULL;

    if ( argumentExists(1) )
    {
        if ( _stmt == TheNilObj )
        {
            ; // Do nothing, this is okay and *stmt is already set to null.
        }
        else if ( ! context->IsOfType(_stmt, "OOSQLITESTMT") )
        {
            return wrongArgValueException(context->threadContext, 1, "an ooSQLiteStmt object or the nil object ", _stmt);
        }
        else
        {
            // If the statement is finalized, that's okay, stmt will be null and
            // that is an acceptable argument to sqlite3_next_stmt.
            pCooSQLiteStmt pCstmt = stmtToCSelf(context, _stmt);
            stmt = pCstmt->stmt;
        }
    }

    sqlite3_stmt *nextStmt = sqlite3_next_stmt(pCdb->db, stmt);

    RexxObjectPtr result = TheNilObj;
    if ( nextStmt != NULL )
    {
        result = findStatement(context, pCdb, nextStmt);
        if ( result == TheNilObj )
        {
            // This should never happen.  SQLite thinks there is a prepared
            // statement, but we don't.
            printf("Internal error.  Cound not find ooSQLiteStmt object for %p statement\n", stmt);
        }
    }

    return result;
}


/** ooSQLiteDB::pragma()
 *
 *  Executes a pragma statement.  A 'PRAGMA' statement is a SQLite specific SQL
 *  extension, probably unknown to any other database engine.
 *
 *  The PRAGMA statement can be used to set / configure database values, to
 *  trigger an action, and to query most of the configurable database values.
 *
 *  @param  name  The name of the pragma.  The user needs to supply the correct
 *                name as documented in the SQLite documentation.
 */
RexxMethod3(RexxObjectPtr, oosqldb_pragma, RexxStringObject, _name, OPTIONAL_RexxObjectPtr, value, CSELF, pCSelf)
{
    RexxObjectPtr result = ooSQLiteErr(context->threadContext, SQLITE_MISUSE, true);
    wholenumber_t rc     = SQLITE_ERROR;

    pCooSQLiteDB pCdb = requiredDB(context, pCSelf);
    if ( pCdb == NULL )
    {
        return result;
    }

    CSTRING name = context->StringData(context->StringLower(_name));

    PragmaType pragma = getPragmaType(context->threadContext, name);
    if ( pragma == unknownPragma )
    {
        return result;
    }

    switch ( pragma )
    {
        case compileOptions :
        case collationList  :
        case databaseList   :
        case foreignKeyList :
        case indexInfo      :
        case indexList      :
        case tableInfo      :
            return pragmaList(context, pCdb, name, value, pragma);

        case integrityCheck :
        case quickCheck     :
        case shrinkMemory   :
        case walCheckpoint  :
            return pragmaTrigger(context, pCdb, name, value, pragma);

        case caseSensitiveLike :
            if ( argumentOmitted(2) )
            {
                return missingArgException(context->threadContext, 2);
            }
            return pragmaSet(context, pCdb, name, value, pragma);

        case incrementalVacuum :
            if (argumentOmitted(2) )
            {
                // SQLite allows the argument to be omitted entirely.  0
                // produces the same effect in SQLite as omitting the argumen.
                value = TheZeroObj;
            }
            return pragmaSet(context, pCdb, name, value, pragma);

        default :
            break;

    }

    if ( argumentExists(2) )
    {
        return pragmaSet(context, pCdb, name, value, pragma);
    }
    else
    {
        return pragmaGet(context, pCdb, name, pragma);
    }
}


/** ooSQLiteDB::profile()
 *
 *  Registers an user callback method used for profiling
 *
 *  @param  callbackObj  [required]  An instantiated class object with a method
 *                       that is invoked as each SQL statement finishes.
 *
 *                       This argument can also be .nil to indicate that any
 *                       installed callback is to be removed.
 *
 *  @param  mthName      [optional]  The method name that will be invoked during
 *                       a call back.  By default, the method invoked will be
 *                       profileCallback().  However, the user can specify an
 *                       alternative method if desired.  This argument is
 *                       ignored when the callbackObj argument is .nil.
 *
 *  @param userData      [optional] This can be any Rexx object the user
 *                       desires. The object will be sent as the third argument
 *                       to the profile callback method when it is invoked.
 *                       This argument is ignored when  the callbackObj argument
 *                       is .nil.
 *
 *  @return  A sqlite result code.
 *
 *  @notes  By default, there is no profile callback installed.
 *
 *          There can only be one profile callback per database connection.
 *          Setting a new profile callback automatically clears any previously
 *          installed callback.
 *
 *          The first argument sent to the callback is the original statement
 *          text.  The second argument is an estimate of wall-clock time of how
 *          long that statement took to run. The profile callback time is in
 *          units of nanoseconds, however the current implementation is only
 *          capable of millisecond resolution so the six least significant
 *          digits in the time are meaningless.
 *
 *          The third arugment is the userData argument to this method, if not
 *          omitted.  If userData is omitted then the third argument is .nil.
 *
 *          Future versions of SQLite might provide greater resolution on the
 *          profiler callback. The sqlite3_profile() function is considered
 *          experimental and is subject to change in future versions of SQLite.
 */
RexxMethod4(RexxObjectPtr, oosqldb_profile, RexxObjectPtr, callbackObj, OPTIONAL_CSTRING, mthName,
            OPTIONAL_RexxObjectPtr, userData, CSELF, pCSelf)
{
    pCooSQLiteDB pCdb = requiredDB(context, pCSelf);
    if ( pCdb == NULL )
    {
        return context->WholeNumber(SQLITE_MISUSE);
    }

    return doCallbackSetup(context, pCdb->db, callbackObj, mthName, userData, profileHook, 0);
}


/** ooSQLiteDB::progressHandler()
 *
 *  Registers an user callback method that is invoked periodically during long
 *  running calls to ooSQLiteDB::exec(), and ooSQLiteStmt::step() for this
 *  database connection. An example use for this interface is to keep a GUI
 *  updated during a large query.
 *
 *  @param  callbackObj  [required]  An instantiated class object with a method
 *                       that is invoked periodically as described above.
 *
 *                       This argument can also be .nil to indicate that any
 *                       installed callback is to be removed.
 *
 *  @param  instructions [required]  the number of virtual machine instructions
 *                       that are evaluated between successive invocations of
 *                       the callback.  If this argument is less than 1, it also
 *                       has the effect of removing any installed callback.
 *
 *  @param  mthName      [optional]  The method name that will be invoked during
 *                       a callback.  By default, the method invoked will be
 *                       progressCallback().  However, the user can specify an
 *                       alternative method if desired.  This argument is
 *                       ignored when the callbackObj argument is .nil.
 *
 *  @param userData      [optional] This can be any Rexx object the user
 *                       desires. The object will be sent as the first and only
 *                       argument to the progress handler callback method when
 *                       it is invoked. This argument is ignored when  the
 *                       callbackObj argument is .nil.  If this argument is
 *                       omitted and the callbackObj argument is not omitted,
 *                       then the .nil object is sent as the only argument to
 *                       the progress handler callback method.
 *
 *  @return  A sqlite result code.
 *
 *  @notes  By default, there is no progress handler callback installed.
 *
 *          There can only be one progress callback per database connection.
 *          Setting a new progress callback automatically clears any previously
 *          installed callback.
 *
 *          The only argument sent to the callback is the userData argument, if
 *          supplied here, or .nil if userData is omitted.
 *
 *          If the callback method returns non-zero, then the operation is
 *          interrupted.  This could be used to implement a Cancel button in a
 *          GUI application.
 */
RexxMethod5(RexxObjectPtr, oosqldb_progressHandler, RexxObjectPtr, callbackObj, int, instructions,
            OPTIONAL_CSTRING, mthName, OPTIONAL_RexxObjectPtr, userData, CSELF, pCSelf)
{
    pCooSQLiteDB pCdb = requiredDB(context, pCSelf);
    if ( pCdb == NULL )
    {
        return context->WholeNumber(SQLITE_MISUSE);
    }

    if ( instructions < 1 )
    {
        callbackObj = TheNilObj;
    }

    return doCallbackSetup(context, pCdb->db, callbackObj, mthName, userData, progressHandler, instructions);
}


/** ooSQLiteDB::rollbackHook()
 *
 *  Registers a callback method to be invoked whenever a transaction is rolled
 *  back.
 *
 *  @param  callbackObj  [required]  An instantiated class object with a method
 *                       that is invoked whenever a transaction is rolled back.
 *
 *                       However, this argument can also be .nil to indicate
 *                       that an already registered callback is to be removed.
 *
 *  @param  mthName      [optional]  The method name that will be invoked during
 *                       the callback.  By default, the method invoked will be
 *                       rollbackHookCallback().  However, the user can specify
 *                       an alternative method if desired.  This argument is
 *                       ignored when the callbackObj argument is .nil.
 *
 *  @param userData      [optional] This can be any Rexx object the user
 *                       desires. The object will be sent as the first and only
 *                       argument to the callback method when it is invoked.
 *                       This argument is ignored when  the callbackObj argument
 *                       is .nil.
 *
 *  @return  The userData argument to a previous invocation of the
 *           rollBackHook() method on the same database connection, or .nil if
 *           there is not one.
 *
 *  @note    The Rexx callback method *must* return a whole number, but that
 *           return is ignored by SQLite.
 */
RexxMethod4(RexxObjectPtr, oosqldb_rollbackHook, RexxObjectPtr, callbackObj, OPTIONAL_CSTRING, mthName,
            OPTIONAL_RexxObjectPtr, userData, CSELF, pCSelf)
{
    pCooSQLiteDB pCdb = requiredDB(context, pCSelf);
    if ( pCdb == NULL )
    {
        return context->WholeNumber(SQLITE_MISUSE);
    }

    return doCallbackSetup(context, pCdb->db, callbackObj, mthName, userData, rollbackHook, 0);
}


/** ooSQLiteDB::setAuthorizer()
 *
 *  Registers an user callback method that is invoked as SQL statements are
 *  being compiled by ooSQLStmt::Prepare().  At various points during the
 *  compilation process, as logic is being created to perform various actions,
 *  the authorizer callback is invoked to see if those actions are allowed.
 *
 *  @param  callbackObj  [required]  An instantiated class object with a method
 *                       that is invoked periodically as described above.
 *
 *                       This argument can also be .nil to indicate that any
 *                       installed callback is to be removed.
 *
 *  @param  mthName      [optional]  The method name that will be invoked during
 *                       a callback.  By default, the method invoked will be
 *                       authorizerCallback().  However, the user can specify an
 *                       alternative method if desired.  This argument is
 *                       ignored when the callbackObj argument is .nil.
 *
 *  @param userData      [optional] This can be any Rexx object the user
 *                       desires. The object will be sent as the last argument
 *                       to the authorizer callback method when it is invoked.
 *                       This argument is ignored when  the callbackObj argument
 *                       is .nil.  If this argument is omitted and the
 *                       callbackObj argument is not omitted, then the .nil
 *                       object is sent as the last argument to the callback
 *                       method.
 *
 *  @return  A sqlite result code.  Although at this time, SQLite always returns
 *           SQLITE_OK.
 *
 *  @notes  By default, there is no authorizer callback installed.
 *
 *          There can only be one authorizer callback per database connection.
 *          Setting a new authorizer callback automatically clears any
 *          previously installed callback.
 *
 *          The arguments sent to the callback are:
 *
 *          1.) A whole number authorizer action code.
 *
 *          2.) String #1.
 *
 *          3.) String #2.
 *
 *          4.) String #3.
 *
 *          5.) String #4.
 *
 *          6.) The userData arg if supplied, otherwise the .nil object if
 *          userData is omitted.
 *
 *          The SQLite docs do not explain what strings 1 through 4 are, other
 *          than to say the arguments: "are zero-terminated strings that contain
 *          additional details about the action to be authorized."
 *
 *          The callback method must return one of 3 codes in response:
 *          SQLITE_OK allows the action.  SQLITE_IGNORE disallows the specific
 *          action but allows the SQL statement to continue to be compiled.
 *          SQLITE_DENY causes the entire SQL statement to be rejected with an
 *          error.
 *
 *          Any other return causes the ooSQLStmt::prepare() call that triggered
 *          the authorizer to fail with an error message.
 */
RexxMethod4(RexxObjectPtr, oosqldb_setAuthorizer, RexxObjectPtr, callbackObj,
            OPTIONAL_CSTRING, mthName, OPTIONAL_RexxObjectPtr, userData, CSELF, pCSelf)
{
    pCooSQLiteDB pCdb = requiredDB(context, pCSelf);
    if ( pCdb == NULL )
    {
        return context->WholeNumber(SQLITE_MISUSE);
    }

    return doCallbackSetup(context, pCdb->db, callbackObj, mthName, userData, authorizer, 0);
}


/** ooSQLiteDB::tableColumnMetadata()
 *
 *
 */
RexxMethod5(int, oosqldb_tableColumnMetadata, CSTRING, dbName, CSTRING, tableName, CSTRING, colName,
            RexxObjectPtr, results, CSELF, pCSelf)
{
    pCooSQLiteDB pCdb = requiredDB(context, pCSelf);
    if ( pCdb == NULL )
    {
        return SQLITE_MISUSE;
    }

    return genTableColumnMetadata(context->threadContext, pCdb->db, dbName, tableName, colName, results, 4);
}


/** ooSQLiteDB::totalChanges()
 *
 *
 */
RexxMethod1(int, oosqldb_totalChanges, CSELF, pCSelf)
{
    pCooSQLiteDB pCdb = requiredDB(context, pCSelf);
    if ( pCdb == NULL )
    {
        return SQLITE_MISUSE;
    }

    return sqlite3_total_changes(pCdb->db);
}


/** ooSQLiteDB::trace()
 *
 *  Registers an user callback method used for tracing.
 *
 *  @param  callbackObj  [required]  An instantiated class object with a method
 *                       that is invoked at various times when an SQL statement
 *                       is being run by ooSQLiteStmt::step(). The callback is
 *                       invoked with a UTF-8 rendering of the SQL statement
 *                       text as the statement first begins executing.
 *                       Additional callbacks might occur as each triggered
 *                       subprogram is entered.
 *
 *                       This argument can also be .nil to indicate that any
 *                       installed callback is to be removed.
 *
 *  @param  mthName      [optional]  The method name that will be invoked during
 *                       a call back.  By default, the method invoked will be
 *                       traceCallback().  However, the user can specify an
 *                       alternative method if desired.  This argument is
 *                       ignored when the callbackObj argument is .nil.
 *
 *  @param userData      [optional] This can be any Rexx object the user
 *                       desires. The object will be sent as the second argument
 *                       to the trace callback method when it is invoked.
 *                       This argument is ignored when  the callbackObj argument
 *                       is .nil.
 *
 *  @return  A sqlite result code.
 *
 *  @notes  By default, there is no trace callback installed.
 *
 *          There can only be one trace callback per database connection.
 *          Setting a new profile callback automatically clears any previously
 *          installed callback.
 *
 *          The first argument sent to the callback is the original SQL
 *          statement text, or a comment identifying a trigger if the callback
 *          is invoked as the result of a trigger.
 *
 *          The second arugment is the userData argument to this method, if not
 *          omitted.  If userData is omitted then the third argument is .nil.
 */
RexxMethod4(RexxObjectPtr, oosqldb_trace, RexxObjectPtr, callbackObj, OPTIONAL_CSTRING, mthName,
            OPTIONAL_RexxObjectPtr, userData, CSELF, pCSelf)
{
    pCooSQLiteDB pCdb = requiredDB(context, pCSelf);
    if ( pCdb == NULL )
    {
        return context->WholeNumber(SQLITE_MISUSE);
    }

    return doCallbackSetup(context, pCdb->db, callbackObj, mthName, userData, traceHook, 0);
}


/** ooSQLiteDB::updateHook()
 *
 *  Registers a callback method to be invoked whenever a row is updated,
 *  inserted or deleted.
 *
 *  @param  callbackObj  [required]  An instantiated class object with a method
 *                       that is invoked whenever a row is updated, inserted or
 *                       deleted.
 *
 *                       However, this argument can also be .nil to indicate
 *                       that an already registered callback is to be removed.
 *
 *  @param  mthName      [optional]  The method name that will be invoked during
 *                       the callback.  By default, the method invoked will be
 *                       updateHookCallback().  However, the user can specify an
 *                       alternative method if desired.  This argument is
 *                       ignored when the callbackObj argument is .nil.
 *
 *  @param userData      [optional] This can be any Rexx object the user
 *                       desires. The object will be sent as the fifth argument
 *                       to the callback method when it is invoked. This
 *                       argument is ignored when  the callbackObj argument is
 *                       .nil.
 *
 *  @return  The userData argument to a previous invocation of the updateHook()
 *           method on this database connection object, or .nil if there is not
 *           a previous invocation.
 *
 *  @note    The Rexx callback method *must* return a whole number, but that
 *           return is ignored by SQLite.
 *
 *           The first 4 arguments to the callback method, in order, are:
 *
 *           1.) An integer constant, one of INSERT, DELETE, or UPDATE,
 *           depending on what the operation was.
 *
 *           2.) The database name containing the affected row.
 *
 *           3.) The table name containing the affected row.
 *
 *           4.) The affected row's rowID. In the case of an update, this is the
 *           rowID after the update takes place.
 */
RexxMethod4(RexxObjectPtr, oosqldb_updateHook, RexxObjectPtr, callbackObj, OPTIONAL_CSTRING, mthName,
            OPTIONAL_RexxObjectPtr, userData, CSELF, pCSelf)
{
    pCooSQLiteDB pCdb = requiredDB(context, pCSelf);
    if ( pCdb == NULL )
    {
        return context->WholeNumber(SQLITE_MISUSE);
    }

    return doCallbackSetup(context, pCdb->db, callbackObj, mthName, userData, updateHook, 0);
}


/**
 * Internal use only method.
 *
 */
RexxMethod2(RexxObjectPtr, oosqldb_putStmt, RexxObjectPtr, stmt, CSELF, pCSelf)
{
    pCooSQLiteDB pCdb = requiredDBCSelf(context, pCSelf);
    if ( pCdb != NULL )
    {
        context->SendMessage1(pCdb->stmtBag, "PUT", stmt);
    }

    return TheNilObj;
}

/**
 * Internal use only method.
 *
 */
RexxMethod2(RexxObjectPtr, oosqldb_delStmt, RexxObjectPtr, stmt, CSELF, pCSelf)
{
    pCooSQLiteDB pCdb = requiredDBCSelf(context, pCSelf);
    if ( pCdb != NULLOBJECT )
    {
        return context->SendMessage1(pCdb->stmtBag, "REMOVE", stmt);
    }

    return TheNilObj;
}

/**
 *  Methods for the .ooSQLiteStmt class.
 */
#define OOSQLITESTMT_CLASS    "ooSQLiteStmt"


/** ooSQLiteStmt:Class::init()
 */
RexxMethod1(RexxObjectPtr, oosqlstmt_init_cls, OSELF, self)
{
    if ( isOfClassType(context, self, OOSQLITESTMT_CLASS) )
    {
        TheOOSQLiteStmtClass = (RexxClassObject)self;
        context->RequestGlobalReference(TheOOSQLiteStmtClass);
    }
    return NULLOBJECT;
}


/** ooSQLiteStmt::errMsg  [attribute get]
 *
 *  Note that errMsg, finalized and initCode attributes have to be accessible
 *  even if stmt is null.  So we just check for a NULL pCSelf, a null stmt is
 *  okay.
 */
RexxMethod1(RexxStringObject, oosqlstmt_getErrMsg_atr, CSELF, pCSelf)
{
    pCooSQLiteStmt pCstmt = requiredStmtCSelf(context, pCSelf);
    if ( pCstmt == NULL )
    {
        return NULLOBJECT;
    }
    return pCstmt->errMsg;
}

/** ooSQLiteStmt::finalized  [attribute get]
 *
 *  Note that errMsg, finalized, and initCode attributes have to be accessible
 *  even if stmt is null.  So we just check for a NULL pCSelf, a null stmt is
 *  okay.
 */
RexxMethod1(logical_t, oosqlstmt_getFinalized_atr, CSELF, pCSelf)
{
    pCooSQLiteStmt pCstmt = requiredStmtCSelf(context, pCSelf);
    if ( pCstmt == NULL )
    {
        return 1;
    }
    return pCstmt->finalized;
}

/** ooSQLiteStmt::initCode  [attribute get]
 *
 *  Note that errMsg, finalized and initCode attributes have to be accessible
 *  even if stmt is null.  So we just check for a NULL pCSelf, a null stmt is
 *  okay.
 */
RexxMethod1(int, oosqlstmt_getInitCode_atr, CSELF, pCSelf)
{
    pCooSQLiteStmt pCstmt = requiredStmtCSelf(context, pCSelf);
    if ( pCstmt == NULL )
    {
        return -1;
    }
    return pCstmt->initCode;
}

/** ooSQLiteStmt::init()
 *
 *  @param db  [required]  The ooSQLiteDB object the statement is for.
 *
 *  @param sql [required]  The SQL statement(s) used by this ooSQLiteStmt.
 *
 *
 */
RexxMethod3(RexxObjectPtr, oosqlstmt_init, RexxObjectPtr, db, CSTRING, sql, OSELF, self)
{
    const char    *msg    = "no error";
    const char    *tail   = NULL;
    pCooSQLiteDB  pCdb    = NULL;
    pCooSQLiteStmt pCstmt = NULL;
    int            rc     = SQLITE_MISUSE;

    // Get a buffer for the ooSQLiteStmt CSelf.
    RexxBufferObject cselfBuffer = context->NewBuffer(sizeof(CooSQLiteStmt));
    if ( cselfBuffer == NULLOBJECT )
    {
        goto done_out;
    }

    context->SetObjectVariable("CSELF", cselfBuffer);

    pCstmt = (pCooSQLiteStmt)context->BufferData(cselfBuffer);
    memset(pCstmt, 0, sizeof(CooSQLiteStmt));

    if ( ! context->IsOfType(db, OOSQLITEDB_CLASS) )
    {
        wrongClassException(context->threadContext, 1, OOSQLITEDB_CLASS);
        goto done_out;
    }

    pCdb = dbToCSelf(context, db);

    if ( pCdb == NULL )
    {
        baseClassIntializationException(context, OOSQLITEDB_CLASS);
        goto done_out;
    }

    if ( pCdb->db == NULL )
    {
        noOpenDBException(context->threadContext, pCdb);
        goto done_out;
    }

    rc = sqlite3_prepare_v2(pCdb->db, sql, (int)strlen(sql) + 1, &pCstmt->stmt, &tail);

    if ( rc == SQLITE_OK )
    {
        pCstmt->db       = db;
        pCstmt->pCdb     = pCdb;
        pCstmt->rexxSelf = self;
        pCstmt->tail     = safeRexxStringRx(context->threadContext, tail);

        context->SetObjectVariable("__rxTail", pCstmt->tail);

        context->SendMessage1(db, "PUTSTMT", self);
    }
    else
    {
        msg = sqlite3_errmsg(pCdb->db);
        if ( msg == NULL )
        {
            msg = "not available";
        }

        // pCstmt->stmt, pCstmt->db, etc., are all still null.  But, we need
        // finalized and initalizationerr to be true.
        pCstmt->finalized = true;
        pCstmt->initializationErr = true;
    }

    pCstmt->initCode = rc;
    pCstmt->errMsg   = context->String(msg);

    context->SetObjectVariable("__rxErrMsg", pCstmt->errMsg);


done_out:
    return NULLOBJECT;
}


RexxMethod1(RexxObjectPtr, oosqlstmt_uninit, CSELF, pCSelf)
{
    if ( pCSelf != NULLOBJECT )
    {
        pCooSQLiteStmt pCstmt = (pCooSQLiteStmt)pCSelf;

#ifdef OOSQLDBG
        printf("ooSQLiteStmt::uninit() prepared statement=%p finalized=%d\n", pCstmt->stmt, pCstmt->finalized);
#endif

        CRITICAL_SECTION_ENTER

        if ( pCstmt->stmt != NULL && pCstmt->db != NULL )
        {
            context->SendMessage1(pCstmt->db, "DELSTMT", pCstmt->rexxSelf);

            sqlite3_finalize(pCstmt->stmt);

            pCstmt->stmt      = NULL;
            pCstmt->db        = NULL;
            pCstmt->tail      = NULL;
            pCstmt->finalized = true;
        }

        CRITICAL_SECTION_LEAVE
    }

    return TheZeroObj;
}


/** ooSQLiteStmt::bindBlob()
 *
 *
 */
RexxMethod3(int, oosqlstmt_bindBlob, int32_t, index, RexxObjectPtr, value, CSELF, pCSelf)
{
    pCooSQLiteStmt pCstmt = requiredStmt(context, pCSelf);
    if ( pCstmt == NULL )
    {
        return SQLITE_MISUSE;
    }

    POINTER data;
    size_t  length;

    if ( context->IsBuffer(value) )
    {
        data   = context->BufferData((RexxBufferObject)value);
        length = context->BufferLength((RexxBufferObject)value);
    }
    else if ( context->IsString(value) )
    {
        data   = (POINTER)context->StringData((RexxStringObject)value);
        length = context->StringLength((RexxStringObject)value);
    }
    else if ( context->IsOfType(value, "MUTABLEBUFFER") )
    {
        RexxStringObject s = (RexxStringObject)context->SendMessage0(value, "STRING");
        data   = (POINTER)context->StringData(s);
        length = context->StringLength(s);
    }
    else
    {
        char buffer[256];
        RexxObjectPtr id = context->SendMessage0(value, "DEFAULTNAME");

        snprintf(buffer, sizeof(buffer),
                 "The bindBlob method only supports Buffer, String, or MutableBuffer objects; found %s",
                 context->ObjectToStringValue(id));
        userDefinedMsgException(context, buffer);
        return SQLITE_MISUSE;
    }

    return sqlite3_bind_blob(pCstmt->stmt, index, data, (int)length, SQLITE_TRANSIENT);
}


/** ooSQLiteStmt::bindDouble()
 *
 *
 */
RexxMethod3(int, oosqlstmt_bindDouble, int32_t, index, double, value, CSELF, pCSelf)
{
    pCooSQLiteStmt pCstmt = requiredStmt(context, pCSelf);
    if ( pCstmt == NULL )
    {
        return SQLITE_MISUSE;
    }

    return sqlite3_bind_double(pCstmt->stmt, index, value);
}


/** ooSQLiteStmt::bindInt()
 *
 *
 */
RexxMethod3(int, oosqlstmt_bindInt, int32_t, index, int, value, CSELF, pCSelf)
{
    pCooSQLiteStmt pCstmt = requiredStmt(context, pCSelf);
    if ( pCstmt == NULL )
    {
        return SQLITE_MISUSE;
    }

    return sqlite3_bind_int(pCstmt->stmt, index, value);
}


/** ooSQLiteStmt::bindInt64()
 *
 *
 */
RexxMethod3(int, oosqlstmt_bindInt64, int32_t, index, int64_t, value, CSELF, pCSelf)
{
    pCooSQLiteStmt pCstmt = requiredStmt(context, pCSelf);
    if ( pCstmt == NULL )
    {
        return SQLITE_MISUSE;
    }

    return sqlite3_bind_int64(pCstmt->stmt, index, value);
}


/** ooSQLiteStmt::bindNull()
 *
 *
 */
RexxMethod2(int, oosqlstmt_bindNull, int32_t, index, CSELF, pCSelf)
{
    pCooSQLiteStmt pCstmt = requiredStmt(context, pCSelf);
    if ( pCstmt == NULL )
    {
        return SQLITE_MISUSE;
    }

    return sqlite3_bind_null(pCstmt->stmt, index);
}


/** ooSQLiteStmt::bindParameterCount()
 *
 *
 */
RexxMethod1(int, oosqlstmt_bindParameterCount, CSELF, pCSelf)
{
    pCooSQLiteStmt pCstmt = requiredStmt(context, pCSelf);
    if ( pCstmt == NULL )
    {
        return SQLITE_MISUSE;
    }

    return sqlite3_bind_parameter_count(pCstmt->stmt);
}


/** ooSQLiteStmt::bindParameterIndex()
 *
 *
 */
RexxMethod2(int, oosqlstmt_bindParameterIndex, CSTRING, name, CSELF, pCSelf)
{
    pCooSQLiteStmt pCstmt = requiredStmt(context, pCSelf);
    if ( pCstmt == NULL )
    {
        return SQLITE_MISUSE;
    }

    return sqlite3_bind_parameter_index(pCstmt->stmt, name);
}


/** ooSQLiteStmt::bindParameterName()
 *
 *
 */
RexxMethod2(RexxObjectPtr, oosqlstmt_bindParameterName, int, index, CSELF, pCSelf)
{
    pCooSQLiteStmt pCstmt = requiredStmt(context, pCSelf);
    if ( pCstmt == NULL )
    {
        return context->WholeNumber(SQLITE_MISUSE);
    }

    return safeRexxStringRx(context->threadContext, sqlite3_bind_parameter_name(pCstmt->stmt, index));
}


/** ooSQLiteStmt::bindText()
 *
 *
 */
RexxMethod3(int, oosqlstmt_bindText, int32_t, index, CSTRING, text, CSELF, pCSelf)
{
    pCooSQLiteStmt pCstmt = requiredStmt(context, pCSelf);
    if ( pCstmt == NULL )
    {
        return SQLITE_MISUSE;
    }

    return sqlite3_bind_text(pCstmt->stmt, index, text, -1, SQLITE_TRANSIENT);
}


/** ooSQLiteStmt::bindValue()
 *
 *  Not implemented.  A pointer to a sqlite3_value (sqlite3_value *) is not
 *  fully understood at this point.
 *
 *  SQLite allows the implementation of application-defined SQL functions and
 *  aggregates.  A possible future enhancement of ooSQLite would be to allow
 *  application-defined SQL functions and aggregates to be written in Rexx.
 *  This method is a place-holder for that enhancement.
 *
 *  See also ooSQLiteStmt::columnValue()
 *
 */
RexxMethod3(int, oosqlstmt_bindValue, int32_t, index, RexxObjectPtr, value, CSELF, pCSelf)
{
    pCooSQLiteStmt pCstmt = requiredStmt(context, pCSelf);
    if ( pCstmt != NULL )
    {
        return SQLITE_MISUSE;
    }

    context->RaiseException0(Rexx_Error_Unsupported_method);
    return SQLITE_MISUSE;
}


/** ooSQLiteStmt::bindZeroBlob()
 *
 *
 */
RexxMethod3(int, oosqlstmt_bindZeroBlob, int32_t, index, int, length, CSELF, pCSelf)
{
    pCooSQLiteStmt pCstmt = requiredStmt(context, pCSelf);
    if ( pCstmt == NULL )
    {
        return SQLITE_MISUSE;
    }

    return sqlite3_bind_zeroblob(pCstmt->stmt, index, length);
}


/** ooSQLiteStmt::clearBindings()
 *
 *
 */
RexxMethod1(int, oosqlstmt_clearBindings, CSELF, pCSelf)
{
    pCooSQLiteStmt pCstmt = requiredStmt(context, pCSelf);
    if ( pCstmt == NULL )
    {
        return SQLITE_MISUSE;
    }

    return sqlite3_clear_bindings(pCstmt->stmt);
}


/** ooSQLiteStmt::columnBlob()
 *
 *
 *  @param col  [required]  The column number in the result row to get the BLOB
 *                          from.  Although SQLite uses 0-based column numbers,
 *                          the Rexx programmer should use 1-based numbers as is
 *                          normal for Rexx.
 */
RexxMethod2(RexxObjectPtr, oosqlstmt_columnBlob, int32_t, col, CSELF, pCSelf)
{
    pCooSQLiteStmt pCstmt = requiredStmt(context, pCSelf);
    if ( pCstmt == NULL )
    {
        return context->WholeNumber(SQLITE_MISUSE);
    }

    col--;
    const void *b = sqlite3_column_blob(pCstmt->stmt, col);
    size_t      s = sqlite3_column_bytes(pCstmt->stmt, col);

    RexxBufferObject result = context->NewBuffer(s);
    if ( result == NULLOBJECT )
    {
        outOfMemoryException(context->threadContext);
        return NULLOBJECT;
    }

    POINTER data = context->BufferData(result);
    memcpy(data, b, s);

    return result;
}


/** ooSQLiteStmt::columnBytes()
 *
 *
 *  @param col  [required]  The column number in the result row to get the
 *                          number of bytes containted in the value for that
 *                          column. Although SQLite uses 0-based column
 *                          numbers, the Rexx programmer should use 1-based
 *                          numbers as is normal for Rexx.
 */
RexxMethod2(int, oosqlstmt_columnBytes, int32_t, col, CSELF, pCSelf)
{
    pCooSQLiteStmt pCstmt = requiredStmt(context, pCSelf);
    if ( pCstmt == NULL )
    {
        return SQLITE_MISUSE;
    }

    col--;
    return sqlite3_column_bytes(pCstmt->stmt, col);
}


/** ooSQLiteStmt::columnCount()
 *
 *
 */
RexxMethod1(int, oosqlstmt_columnCount, CSELF, pCSelf)
{
    pCooSQLiteStmt pCstmt = requiredStmt(context, pCSelf);
    if ( pCstmt == NULL )
    {
        return SQLITE_MISUSE;
    }

    return sqlite3_column_count(pCstmt->stmt);
}


/** ooSQLiteStmt::columnDataBaseName()
 *
 *
 *  @param col  [required]  A column number in the result row.  SQLite requires
 *                          this.  Although SQLite uses 0-based column numbers,
 *                          the Rexx programmer should use 1-based numbers as is
 *                          normal for Rexx.
 *
 */
RexxMethod2(RexxObjectPtr, oosqlstmt_columnDatabaseName, int32_t, col, CSELF, pCSelf)
{
    pCooSQLiteStmt pCstmt = requiredStmt(context, pCSelf);
    if ( pCstmt == NULL )
    {
        return context->WholeNumber(SQLITE_MISUSE);
    }

    col--;
    CSTRING name = sqlite3_column_database_name(pCstmt->stmt, col);
    return safeRexxString(context->threadContext, name);
}


/** ooSQLiteStmt::columnDeclType()
 *
 *  Retrives the declared datatype of a query result.
 *
 *  @param col  [required]  The column number in the result row to get the
 *                          declared type for. Although SQLite uses 0-based
 *                          column numbers, the Rexx programmer should use
 *                          1-based numbers as is normal for Rexx.
 *
 *  @return
 *
 *  @note  The empty string is returned when sqlite returns a NULL pointer.
 */
RexxMethod2(RexxObjectPtr, oosqlstmt_columnDeclType, int32_t, col, CSELF, pCSelf)
{
    pCooSQLiteStmt pCstmt = requiredStmt(context, pCSelf);
    if ( pCstmt == NULL )
    {
        return context->WholeNumber(SQLITE_MISUSE);
    }

    col--;
    CSTRING name = sqlite3_column_decltype(pCstmt->stmt, col);
    return safeRexxStringRx(context->threadContext, name);
}


/** ooSQLiteStmt::columnDouble()
 *
 *
 *  @param col  [required]  The column number in the result row to get the
 *                          double value from. Although SQLite uses 0-based
 *                          column numbers, the Rexx programmer should use
 *                          1-based numbers as is normal for Rexx.
 */
RexxMethod2(double, oosqlstmt_columnDouble, int32_t, col, CSELF, pCSelf)
{
    pCooSQLiteStmt pCstmt = requiredStmt(context, pCSelf);
    if ( pCstmt == NULL )
    {
        return SQLITE_MISUSE;
    }

    col--;
    return sqlite3_column_double(pCstmt->stmt, col);
}


/** ooSQLiteStmt::columnInt()
 *
 *
 *  @param col  [required]  The column number in the result row to get the
 *                          integer value from. Although SQLite uses 0-based
 *                          column numbers, the Rexx programmer should use
 *                          1-based numbers as is normal for Rexx.
 */
RexxMethod2(intptr_t, oosqlstmt_columnInt, int32_t, col, CSELF, pCSelf)
{
    pCooSQLiteStmt pCstmt = requiredStmt(context, pCSelf);
    if ( pCstmt == NULL )
    {
        return SQLITE_MISUSE;
    }

    col--;
    return sqlite3_column_int(pCstmt->stmt, col);
}


/** ooSQLiteStmt::columnInt64()
 *
 *
 *  @param col  [required]  The column number in the result row to get the
 *                          64-bit integer value from. Although SQLite uses
 *                          0-based column numbers, the Rexx programmer should
 *                          use 1-based numbers as is normal for Rexx.
 */
RexxMethod2(int64_t, oosqlstmt_columnInt64, int32_t, col, CSELF, pCSelf)
{
    pCooSQLiteStmt pCstmt = requiredStmt(context, pCSelf);
    if ( pCstmt == NULL )
    {
        return SQLITE_MISUSE;
    }

    col--;
    return sqlite3_column_int64(pCstmt->stmt, col);
}


/** ooSQLiteStmt::columnName()
 *
 *  Returns the name assigned to a particular column in the result set of a
 *  SELECT statement.
 *
 *  @param col  [required]  The column number in the result row whose origin
 *                          (column) name is desired.  Although SQLite uses
 *                          0-based column numbers, the Rexx programmer should
 *                          use 1-based numbers as is normal for Rexx.
 *
 */
RexxMethod2(RexxObjectPtr, oosqlstmt_columnName, int32_t, col, CSELF, pCSelf)
{
    pCooSQLiteStmt pCstmt = requiredStmt(context, pCSelf);
    if ( pCstmt == NULL )
    {
        return context->WholeNumber(SQLITE_MISUSE);
    }

    col--;
    CSTRING name = sqlite3_column_name(pCstmt->stmt, col);
    return safeRexxString(context->threadContext, name);
}


/** ooSQLiteStmt::columnOriginName()
 *
 *  Returns the original un-aliased name of the column in the result row
 *  specified.
 *
 *  @param col  [required]  The column number in the result row whose origin
 *                          (column) name is desired.  Although SQLite uses
 *                          0-based column numbers, the Rexx programmer should
 *                          use 1-based numbers as is normal for Rexx.
 *
 */
RexxMethod2(RexxObjectPtr, oosqlstmt_columnOriginName, int32_t, col, CSELF, pCSelf)
{
    pCooSQLiteStmt pCstmt = requiredStmt(context, pCSelf);
    if ( pCstmt == NULL )
    {
        return context->WholeNumber(SQLITE_MISUSE);
    }

    col--;
    CSTRING name = sqlite3_column_origin_name(pCstmt->stmt, col);
    return safeRexxString(context->threadContext, name);
}


/** ooSQLiteStmt::columnTableName()
 *
 *
 *  @param col  [required]  A column number in the result row.  SQLite requires
 *                          this.  Although SQLite uses 0-based column numbers,
 *                          the Rexx programmer should use 1-based numbers as is
 *                          normal for Rexx.
 *
 */
RexxMethod2(RexxObjectPtr, oosqlstmt_columnTableName, int32_t, col, CSELF, pCSelf)
{
    pCooSQLiteStmt pCstmt = requiredStmt(context, pCSelf);
    if ( pCstmt == NULL )
    {
        return context->WholeNumber(SQLITE_MISUSE);
    }

    col--;
    CSTRING name = sqlite3_column_table_name(pCstmt->stmt, col);
    return safeRexxString(context->threadContext, name);
}


/** ooSQLiteStmt::columnText()
 *
 *
 *  @param col  [required]  The column in the result row to get the text for.
 *                          Although SQLite uses 0-based column numbers, the
 *                          Rexx programmer should use 1-based numbers as is
 *                          normal for Rexx.
 *
 *
 */
RexxMethod2(RexxObjectPtr, oosqlstmt_columnText, int32_t, col, CSELF, pCSelf)
{
    pCooSQLiteStmt pCstmt = requiredStmt(context, pCSelf);
    if ( pCstmt == NULL )
    {
        return context->WholeNumber(SQLITE_MISUSE);
    }
    col--;
    return context->String(safeColumnText(pCstmt->stmt, col));
}


/** ooSQLiteStmt::columnType()
 *
 *
 *  @param col  [required]  The column in the result row to get the type of.
 *                          Although SQLite uses 0-based column numbers, the
 *                          Rexx programmer should use 1-based numbers as is
 *                          normal for Rexx.
 *
 *
 */
RexxMethod2(int, oosqlstmt_columnType, int32_t, col, CSELF, pCSelf)
{
    pCooSQLiteStmt pCstmt = requiredStmt(context, pCSelf);
    if ( pCstmt == NULL )
    {
        return SQLITE_MISUSE;
    }
    col--;
    return sqlite3_column_type(pCstmt->stmt, col);
}


/** ooSQLiteStmt::columnValue()
 *
 *  Not implemented.  A pointer to a sqlite3_value (sqlite3_value *) is not
 *  fully understood at this point.  From the SQLite docs:
 *
 *  The object returned by sqlite3_column_value() is an unprotected
 *  sqlite3_value object. An unprotected sqlite3_value object may only be used
 *  with sqlite3_bind_value() and sqlite3_result_value(). If the unprotected
 *  sqlite3_value object returned by sqlite3_column_value() is used in any other
 *  way, including calls to routines like sqlite3_value_int(),
 *  sqlite3_value_text(), or sqlite3_value_bytes(), then the behavior is
 *  undefined.
 *
 *  SQLite allows the implementation of application-defined SQL functions and
 *  aggregates.  A possible future enhancement of ooSQLite would be to allow
 *  application-defined SQL functions and aggregates to be written in Rexx.
 *  This method is a place-holder for that enhancement.
 *
 *  See also ooSQLiteStmt::bindValue()
 *
 */
RexxMethod2(int, oosqlstmt_columnValue, int32_t, index, CSELF, pCSelf)
{
    pCooSQLiteStmt pCstmt = requiredStmt(context, pCSelf);
    if ( pCstmt != NULL )
    {
        return SQLITE_MISUSE;
    }

    context->RaiseException0(Rexx_Error_Unsupported_method);
    return SQLITE_MISUSE;
}


/** ooSQLiteStmt::dataCount()
 *
 *
 */
RexxMethod1(int, oosqlstmt_dataCount, CSELF, pCSelf)
{
    pCooSQLiteStmt pCstmt = requiredStmt(context, pCSelf);
    if ( pCstmt == NULL )
    {
        return SQLITE_MISUSE;
    }

    return sqlite3_data_count(pCstmt->stmt);
}


/** ooSQLiteStmt::dbHandle()
 *
 *
 */
RexxMethod1(RexxObjectPtr, oosqlstmt_dbHandle, CSELF, pCSelf)
{
    pCooSQLiteStmt pCstmt = requiredStmt(context, pCSelf);
    if ( pCstmt == NULL )
    {
        return context->WholeNumber(SQLITE_MISUSE);
    }

    RexxObjectPtr result = pCstmt->db;

    // We should get the same handle as we already know. What to do if we don't?
    sqlite3 *handle = sqlite3_db_handle(pCstmt->stmt);

    if ( handle != pCstmt->pCdb->db )
    {
        printf("Internal consistency check fails.\nSQLite db handle (%p) does not match known db handle (%p)\n",
               handle, pCstmt->pCdb->db);
        result = TheNilObj;
    }

    return result;
}


/** ooSQLiteStmt::finalize()
 *
 *
 *  @note  The SQLite docs say that finializing a null stmt is a harmless no-op.
 *
 *         Also, if I'm interpreting the sqlite docs correctly, although an
 *         error code might be returned, the prepared statement is always
 *         deleted. The, possible, error code is the return from a previous
 *         error.  Such as an error from a previously executed step.
 *
 *         Nevertheless, if this prepared statement has been finalized, we don't
 *         want to do it again.  For one thing we no longer have a db to send a
 *         message to.
 */
RexxMethod1(int, oosqlstmt_finalize, CSELF, pCSelf)
{
    pCooSQLiteStmt pCstmt = requiredStmtCSelf(context, pCSelf);
    if ( pCstmt == NULL )
    {
        return SQLITE_MISUSE;
    }

    int rc = SQLITE_OK;

    if ( ! pCstmt->finalized )
    {
        if ( pCstmt->db != NULLOBJECT && pCstmt->rexxSelf != NULLOBJECT )
        {
            context->SendMessage1(pCstmt->db, "DELSTMT", pCstmt->rexxSelf);
        }

        rc = sqlite3_finalize(pCstmt->stmt);

        pCstmt->stmt      = NULL;
        pCstmt->db        = NULL;
        pCstmt->tail      = NULL;
        pCstmt->finalized = true;
    }

    return rc;
}


/** ooSQLiteStmt::reset()
 *
 *
 */
RexxMethod1(int, oosqlstmt_reset, CSELF, pCSelf)
{
    pCooSQLiteStmt pCstmt = requiredStmt(context, pCSelf);
    if ( pCstmt == NULL )
    {
        return SQLITE_MISUSE;
    }

    return sqlite3_reset(pCstmt->stmt);
}


/** ooSQLiteStmt::sql()
 *
 *
 */
RexxMethod1(RexxObjectPtr, oosqlstmt_sql, CSELF, pCSelf)
{
    pCooSQLiteStmt pCstmt = requiredStmt(context, pCSelf);
    if ( pCstmt == NULL )
    {
        return context->WholeNumber(SQLITE_MISUSE);
    }

    return safeRexxStringRx(context->threadContext, sqlite3_sql(pCstmt->stmt));
}


/** ooSQLiteStmt::step()
 *
 *
 */
RexxMethod1(int, oosqlstmt_step, CSELF, pCSelf)
{
    pCooSQLiteStmt pCstmt = requiredStmt(context, pCSelf);
    if ( pCstmt == NULL )
    {
        return SQLITE_MISUSE;
    }

    return sqlite3_step(pCstmt->stmt);
}


/** ooSQLiteStmt::stmtBusy()
 *
 *  The stmtBusy() method returns true if this prepared statement has been
 *  stepped at least once using step() but has not run to completion and/or has
 *  not been reset using reset().
 *
 *  @return True or false.
 *
 *  @note  We allow pCstmt->stmt to be null, i.e. the statement has already been
 *         finalize, for this method because SQLite returns false if stmt is
 *         null.
 */
RexxMethod1(logical_t, oosqlstmt_stmtBusy, CSELF, pCSelf)
{
    pCooSQLiteStmt pCstmt = requiredStmtCSelf(context, pCSelf);
    if ( pCstmt == NULL )
    {
        return SQLITE_MISUSE;
    }

    return sqlite3_stmt_busy(pCstmt->stmt);
}


/** ooSQLiteStmt::stmtReadonly()
 *
 *
 *
 */
RexxMethod1(logical_t, oosqlstmt_stmtReadonly, CSELF, pCSelf)
{
    pCooSQLiteStmt pCstmt = requiredStmt(context, pCSelf);
    if ( pCstmt == NULL )
    {
        return FALSE;
    }

    return sqlite3_stmt_readonly(pCstmt->stmt);
}


/** ooSQLiteStmt::stmtStatus()
 *
 *
 *
 */
RexxMethod3(int, oosqlstmt_stmtStatus, int, param, OPTIONAL_logical_t, reset, CSELF, pCSelf)
{
    pCooSQLiteStmt pCstmt = requiredStmt(context, pCSelf);
    if ( pCstmt == NULL )
    {
        return SQLITE_MISUSE;
    }

    return sqlite3_stmt_status(pCstmt->stmt, param, (int)reset);
}


/** ooSQLiteStmt::value()
 *
 *  A debugging method.  Returns the string value of the stmt pointer.  Not sure
 *  whether to document this.
 *
 *
 */
RexxMethod1(RexxObjectPtr, oosqlstmt_value, CSELF, pCSelf)
{
    pCooSQLiteStmt pCstmt = requiredStmt(context, pCSelf);
    if ( pCstmt == NULL )
    {
        return context->WholeNumber(SQLITE_MISUSE);
    }

    char buf[64];
    snprintf(buf, sizeof(buf), "0x%p", pCstmt->stmt);

    return context->String(buf);
}



/**
 *  Methods for the .ooSQLiteMutex class.
 */
#define OOSQLITEMUTEX_CLASS    "ooSQLiteMutex"


/** ooSQLiteMutex:Class::init()
 */
RexxMethod1(RexxObjectPtr, oosqlmtx_init_cls, OSELF, self)
{
    if ( isOfClassType(context, self, OOSQLITEMUTEX_CLASS) )
    {
        TheOOSQLiteMutexClass = (RexxClassObject)self;
        context->RequestGlobalReference(TheOOSQLiteMutexClass);
    }
    return NULLOBJECT;
}


/** ooSQLiteMutex::closed  [attribute get]
 */
RexxMethod1(logical_t, oosqlmtx_getClosed_atr, CSELF, pCSelf)
{
    pCooSQLiteMutex pCmtx = requiredMtxCSelf(context, pCSelf);
    if ( pCmtx == NULL )
    {
        return 1;
    }
    return pCmtx->closed;
}

/** ooSQLiteMutex::isNull  [attribute get]
 */
RexxMethod1(logical_t, oosqlmtx_getIsNull_atr, CSELF, pCSelf)
{
    pCooSQLiteMutex pCmtx = requiredMtxCSelf(context, pCSelf);
    if ( pCmtx == NULL )
    {
        return 1;
    }
    return pCmtx->mutex == NULL;
}

/** ooSQLiteMutex::init()
 *
 *  @param type  [required]  The SQLite muxtex type.  Can only be:
 *               SQLITE_MUTEX_RECURSIVE or SQLITE_MUTEX_FAST.
 *
 *
 *  @remarks  The magic and mtx arguments are to allow a special case, internal
 *            use only, creation of an ooSQLiteMutext.  If either argument
 *            exists, they must both exist, magic must be correct, mtx can be
 *            null though.
 *
 *            We should probably test all of that, but really it seems
 *            impossible for the Rexx programmer to accidently get magic right,
 *            so that is all that is tested.
 */
RexxMethod4(RexxObjectPtr, oosqlmtx_init, int, type, OPTIONAL_POINTER, magic, OPTIONAL_POINTER, mtx, OSELF, self)
{
    bool isDbMutex = false;

    if ( argumentExists(2) || argumentExists(3) )
    {
        if ( argumentOmitted(1) || magic != (POINTER)MutexMagic )
        {
            return tooManyArgsException(context->threadContext, 1);
        }
        isDbMutex = true;
    }

    if ( ! isDbMutex )
    {
        // We assume if we called this internally we passed the right type.
        if ( ! (type == SQLITE_MUTEX_FAST || type == SQLITE_MUTEX_RECURSIVE)  )
        {
            return wrongArgValueException(context->threadContext, 1, "MUTEX_RECURSIVE or MUTEX_FAST",
                                          context->WholeNumber(type));
        }
    }

    // Get a buffer for the ooSQLiteMutex CSelf.
    RexxBufferObject cselfBuffer = context->NewBuffer(sizeof(CooSQLiteMutex));
    if ( cselfBuffer == NULLOBJECT )
    {
        outOfMemoryException(context->threadContext);
        return NULLOBJECT;
    }

    context->SetObjectVariable("CSELF", cselfBuffer);

    pCooSQLiteMutex pCmtx = (pCooSQLiteMutex)context->BufferData(cselfBuffer);
    memset(pCmtx, 0, sizeof(CooSQLiteMutex));

    if ( isDbMutex )
    {
        // mtx can be null, that's fine, but then we need to set the state
        // variables.
        pCmtx->mutex    = (sqlite3_mutex *)mtx;
        pCmtx->type     = SQLITE_MUTEX_RECURSIVE_DB;
        pCmtx->rexxSelf = self;

        if ( mtx == NULL )
        {
            pCmtx->initializationErr = true;
            pCmtx->closed            = true;
        }
    }
    else
    {
        pCmtx->mutex = sqlite3_mutex_alloc(type);

        if ( pCmtx->mutex != NULL )
        {
            pCmtx->type     = type;
            pCmtx->rexxSelf = self;
        }
        else
        {
            pCmtx->closed = true;
            pCmtx->initializationErr = true;
        }
    }

    return NULLOBJECT;
}

RexxMethod1(RexxObjectPtr, oosqlmtx_uninit, CSELF, pCSelf)
{
    if ( pCSelf != NULLOBJECT )
    {
        pCooSQLiteMutex pCmtx = (pCooSQLiteMutex)pCSelf;

#ifdef OOSQLDBG
        printf("ooSQLiteMutex::uninit() mutex=%p closed=%d\n", pCmtx->mutex, pCmtx->closed);
#endif

        if ( pCmtx->mutex != NULL && pCmtx->type != SQLITE_MUTEX_RECURSIVE_DB )
        {
            sqlite3_mutex_free(pCmtx->mutex);

            pCmtx->mutex  = NULL;
            pCmtx->closed = true;
        }
    }

    return TheZeroObj;
}


/** ooSQLiteMutex::enter()
 *
 *  Enters this mutex.  Will block if the mutex is held by another thread.
 *
 *  @returns  Always returns OK, has no meaning.
 */
RexxMethod1(int, oosqlmtx_enter, CSELF, pCSelf)
{
    pCooSQLiteMutex pCmtx = requiredMtxCSelf(context, pCSelf);
    if ( pCmtx == NULL )
    {
        return 1;
    }

    sqlite3_mutex_enter(pCmtx->mutex);

    return SQLITE_OK;
}


/** ooSQLiteMutex::free()
 *
 *  Frees this mutex.  The mutex must not be in use
 *
 *  @returns  Always returns OK, has no meaning.
 */
RexxMethod1(int, oosqlmtx_free, CSELF, pCSelf)
{
    pCooSQLiteMutex pCmtx = requiredMtxCSelf(context, pCSelf);
    if ( pCmtx == NULL )
    {
        return 0;
    }

    if ( pCmtx->type != SQLITE_MUTEX_RECURSIVE_DB )
    {
        sqlite3_mutex_free(pCmtx->mutex);
    }

    pCmtx->closed   = true;
    pCmtx->mutex    = NULL;
    pCmtx->rexxSelf = NULLOBJECT;

    return SQLITE_OK;
}


/** ooSQLiteMutex::leave()
 *
 *  Leaves this mutex.
 *
 *  @returns  Always returns OK, has no meaning.
 */
RexxMethod1(int, oosqlmtx_leave, CSELF, pCSelf)
{
    pCooSQLiteMutex pCmtx = requiredMtxCSelf(context, pCSelf);
    if ( pCmtx == NULL )
    {
        return 1;
    }

    sqlite3_mutex_leave(pCmtx->mutex);

    return SQLITE_OK;
}


/** ooSQLiteMutex::try()
 *
 *  Trys to enter this mutex.  Will return immediatley if the mutex is held by
 *  another thread.
 *
 *  @returns  Always returns OK, has no meaning.
 */
RexxMethod1(int, oosqlmtx_try, CSELF, pCSelf)
{
    pCooSQLiteMutex pCmtx = requiredMtxCSelf(context, pCSelf);
    if ( pCmtx == NULL )
    {
        return 1;
    }

    return sqlite3_mutex_try(pCmtx->mutex);
}



#define ooSQLite_Routines_Section


static inline sqlite3 *routineDB(RexxCallContext *c, void *_db)
{
    sqlite3 *db = (sqlite3 *)_db;
    if ( db == NULL )
    {
        nullObjectException(c->threadContext, "database connection", 1);
    }
    return db;
}

static inline sqlite3_stmt *routineStmt(RexxCallContext *c, void *_stmt)
{
    sqlite3_stmt *stmt = (sqlite3_stmt *)_stmt;
    if ( stmt == NULL )
    {
        nullObjectException(c->threadContext, "prepared statment", 1);
    }
    return stmt;
}

static inline sqlite3_mutex *routineMutex(RexxCallContext *c, void *_mtx)
{
    sqlite3_mutex *mtx = (sqlite3_mutex *)_mtx;
    if ( mtx == NULL )
    {
        nullObjectException(c->threadContext, "mutex", 1);
    }
    return mtx;
}

/**
 * Instantiates the hidden help object.  The instantiation will put the DB
 * callback buffer as a side effect.
 *
 * @assumes The caller has checked that TheHiddenHelper is null.
 *
 * @param c
 * @param db
 * @param buf
 * @param cbt
 *
 * @return RexxObjectPtr
 */
static RexxObjectPtr lazyInstantiateHelper(RexxCallContext *c, sqlite3 *db, RexxBufferObject buf, CallbackType cbt)
{
    RexxArrayObject args = c->ArrayOfThree(c->NewPointer(db), buf, c->String(getCallbackVarName(cbt)));

    RexxObjectPtr h = c->SendMessage(TheRoutineHelperClass, "NEW", args);
    if ( h == NULLOBJECT )
    {
        severeErrorException(c->threadContext, "Failed to instantiate TheHiddenHelper object");
        return NULLOBJECT;
    }

    TheHiddenHelper = c->RequestGlobalReference(h);

    return TheNilObj;
}

static RexxBufferObject helperPutCallback(RexxCallContext *c, sqlite3 *db, RexxBufferObject buf, CallbackType cbt)
{
    if ( TheHiddenHelper == NULLOBJECT )
    {
        return (RexxBufferObject)lazyInstantiateHelper(c, db, buf, cbt);
    }

    RexxArrayObject args = c->ArrayOfThree(c->NewPointer(db), buf, c->String(getCallbackVarName(cbt)));

    return (RexxBufferObject)c->SendMessage(TheHiddenHelper, "PUTDBCALLBACK", args);
}

static void helperDelCallback(RexxCallContext *c, sqlite3 *db, CallbackType cbt)
{
    if ( TheHiddenHelper != NULLOBJECT )
    {
        c->SendMessage2(TheHiddenHelper, "DELDBCALLBACK",
                        c->NewPointer(db),
                        c->String(getCallbackVarName(cbt)));
    }
}

static void helperCleanCallbacks(RexxCallContext *c, sqlite3 *db)
{
    if ( TheHiddenHelper != NULLOBJECT )
    {
        c->SendMessage1(TheHiddenHelper, "CLEANUPDBCALLBACKS", c->NewPointer(db));
    }
}

/**
 * Given a call context, attempts to find the *public* routine object with the
 * specified name.
 *
 * @param c        Caller context we are operating in.
 * @param rtnName  Name of the public routine to search for.
 *
 * @return The Rexx routine object if found, otherwise the null object.
 */
static RexxRoutineObject getCallerRoutine(RexxCallContext *c, CSTRING rtnName)
{
    RexxRoutineObject rtn = NULLOBJECT;

    RexxObjectPtr rxContext = c->GetCallerContext();
    RexxPackageObject pkg = (RexxPackageObject)c->SendMessage0(rxContext, "PACKAGE");

    if ( pkg != NULLOBJECT )
    {
        RexxDirectoryObject d = c->GetPackagePublicRoutines(pkg);
        rtn = (RexxRoutineObject)c->DirectoryAt(d, rtnName);
    }

    return rtn;
}

static RexxObjectPtr removeCallbackRtn(RexxCallContext *c, sqlite3 *db, CallbackType cbt)
{
    RexxObjectPtr     result = TheNilObj;
    pCGenericCallback cbc    =  NULL;
    int               rc     = SQLITE_OK;

    if ( cbt == authorizer )
    {
        rc = sqlite3_set_authorizer(db, NULL, NULL);
        result = c->WholeNumber(rc);
    }
    else if ( cbt == busyHandler )
    {
        rc = sqlite3_busy_handler(db, NULL, NULL);
        result = c->WholeNumber(rc);
    }
    else if ( cbt == commitHook )
    {
        cbc = (pCGenericCallback)sqlite3_commit_hook(db, NULL, NULL);
    }
    else if ( cbt == profileHook )
    {
        cbc = (pCGenericCallback)sqlite3_profile(db, NULL, NULL);
    }
    else if ( cbt == progressHandler )
    {
        sqlite3_progress_handler(db, 0, NULL, NULL);
        result = c->WholeNumber(SQLITE_OK);
    }
    else if ( cbt == rollbackHook )
    {
        cbc = (pCGenericCallback)sqlite3_rollback_hook(db, NULL, NULL);
    }
    else if ( cbt == traceHook )
    {
        cbc = (pCGenericCallback)sqlite3_trace(db, NULL, NULL);
    }
    else
    {
        cbc = (pCGenericCallback)sqlite3_update_hook(db, NULL, NULL);
    }

    if ( cbc != NULL )
    {
        if ( cbc->userData != NULL )
        {
            result = cbc->userData;
        }

        helperDelCallback(c, db, cbt);
    }

    return result;
}

static RexxObjectPtr installCallbackRtn(RexxCallContext *c, sqlite3 *db, RexxBufferObject cbcBuffer,
                                        pCGenericCallback cbc, CallbackType cbt, int instructions)
{
    pCGenericCallback oldCbc = NULL;
    RexxObjectPtr     result = TheNilObj;
    int               rc     = SQLITE_OK;

    if ( cbt == authorizer )
    {
        rc = sqlite3_set_authorizer(db, authorizerCallback, (void *)cbc);
        result = c->WholeNumber(rc);
    }
    else if ( cbt == busyHandler )
    {
        rc = sqlite3_busy_handler(db, busyCallBack, (void *)cbc );
        result = c->WholeNumber(rc);
    }
    else if ( cbt == commitHook )
    {
        oldCbc = (pCGenericCallback)sqlite3_commit_hook(db, commitHookCallback, (void *)cbc);
    }
    else if ( cbt == profileHook )
    {
        oldCbc = (pCGenericCallback)sqlite3_profile(db, profileHookCallback, (void *)cbc);
    }
    else if ( cbt == progressHandler )
    {
        sqlite3_progress_handler(db, instructions, progressCallback, (void *)cbc);
        result = c->WholeNumber(SQLITE_OK);
    }
    else if ( cbt == rollbackHook )
    {
        oldCbc = (pCGenericCallback)sqlite3_rollback_hook(db, rollbackHookCallback, (void *)cbc);
    }
    else if ( cbt == traceHook )
    {
        oldCbc = (pCGenericCallback)sqlite3_profile(db, profileHookCallback, (void *)cbc);
    }
    else
    {
        oldCbc = (pCGenericCallback)sqlite3_update_hook(db, updateHookCallback, (void *)cbc);
    }

    if ( oldCbc != NULL )
    {
        if ( oldCbc->userData != NULL )
        {
            result = oldCbc->userData;
        }
    }

    RexxBufferObject oldBuffer = helperPutCallback(c, db, cbcBuffer, cbt);
    cleanupThisCallback(c->threadContext, oldBuffer);

    return result;
}

/**
 * Does the basic setup, which is the same, for one of the 3 SQLite provided
 * callback registrations: sqlite3_commit_hook(), sqlite3_rollback_hook(), or
 * sqlite3_update_hook().
 *
 * This function handles this for the classic Rexx interface to SQLite.  It is
 * similar to the doCallbackSetup(), which is for the object orientated SQLite
 * object orientated interface.
 *
 * @param c
 * @param db
 * @param userData
 * @param cbt        Identifies which callback registration this is for.
 * @param int
 *
 * @return RexxObjectPtr
 */
static RexxObjectPtr doCallbackSetupRtn(RexxCallContext *c, sqlite3 *db, CSTRING rtnName, RexxObjectPtr userData,
                                        CallbackType cbt, int instructions)
{

    if ( strlen(rtnName) == 0 )
    {
        return removeCallbackRtn(c, db, cbt);
    }

    RexxBufferObject cbcBuffer = c->NewBuffer(sizeof(CGenericCallback));

    if ( cbcBuffer == NULLOBJECT )
    {
        outOfMemoryException(c->threadContext);
        return c->WholeNumber(SQLITE_NOMEM);
    }

    pCGenericCallback cbc = (pCGenericCallback)c->BufferData(cbcBuffer);
    memset(cbc, 0, sizeof(CGenericCallback));

    cbc->callbackContext = c->threadContext;
    cbc->routineName     = rtnName;
    cbc->callbackRtn     = getCallerRoutine(c, rtnName);

    if ( cbc->callbackRtn == NULLOBJECT )
    {
        noSuchRoutineException(c->threadContext, rtnName, 2);
        return c->WholeNumber(SQLITE_MISUSE);
    }

    if ( userData != NULLOBJECT )
    {
        cbc->userData = c->RequestGlobalReference(userData);
    }

    return installCallbackRtn(c, db, cbcBuffer, cbc, cbt, instructions);
}



/** ooSQLiteVersion()
 *
 *  Returns the ooSQLite version string
 *
 *  @param type  Specifies the version type: Full, OneLine, or Compact.  Only
 *               the first letter is checked, F O C, and case is not
 *               significant.  The default is OneLine
 *
 *  @return  A string, either the full version string, or the one line version
 *           string.
 */
RexxRoutine1(RexxObjectPtr, ooSQLiteVersion_rtn, OPTIONAL_CSTRING, type)
{
    if ( argumentExists(1) )
    {
        switch ( toupper(*type) )
        {
            case 'O' :
                return genGetVersion(context->threadContext, FALSE, FALSE);
                break;

            case 'L' :
                return context->String(sqlite3_libversion());
                break;

            case 'N' :
                return context->Int32(sqlite3_libversion_number());
                break;

            case 'S' :
                return context->String(sqlite3_sourceid());
                break;

            case 'F' :
                return genGetVersion(context->threadContext, TRUE, FALSE);
                break;

            case 'C' :
                return genGetVersion(context->threadContext, FALSE, TRUE);
                break;

            default :
                wrongArgOptionException(context->threadContext, 1, VALID_VERSION_TYPES, type);
                return TheZeroObj;
                break;
        }
    }

    return genGetVersion(context->threadContext, FALSE, FALSE);
}


/** ooSQLiteMerge()
 *
 *
 */
RexxRoutine1(wholenumber_t, ooSQLiteMerge_rtn, ARGLIST, args)
{
    return genMerge(context->threadContext, args);
}


/** oosqlBindBlob()
 *
 *
 */
RexxRoutine3(int, oosqlBindBlob_rtn, POINTER, _stmt, int32_t, index, RexxObjectPtr, value)
{
    sqlite3_stmt *stmt = routineStmt(context, _stmt);
    if ( stmt == NULL )
    {
        return SQLITE_MISUSE;
    }

    POINTER data;
    size_t  length;

    if ( context->IsBuffer(value) )
    {
        data   = context->BufferData((RexxBufferObject)value);
        length = context->BufferLength((RexxBufferObject)value);
    }
    else if ( context->IsString(value) )
    {
        data   = (POINTER)context->StringData((RexxStringObject)value);
        length = context->StringLength((RexxStringObject)value);
    }
    else if ( context->IsOfType(value, "MUTABLEBUFFER") )
    {
        RexxStringObject s = (RexxStringObject)context->SendMessage0(value, "STRING");
        data   = (POINTER)context->StringData(s);
        length = context->StringLength(s);
    }
    else
    {
        char buffer[256];
        RexxObjectPtr id = context->SendMessage0(value, "DEFAULTNAME");

        snprintf(buffer, sizeof(buffer),
                 "The oosqlBindBlob routine only supports Buffer, String, or MutableBuffer objects; found %s",
                 context->ObjectToStringValue(id));
        userDefinedMsgException(context->threadContext, buffer);
        return SQLITE_MISUSE;
    }

    return sqlite3_bind_blob(stmt, index, data, (int)length, SQLITE_TRANSIENT);
}

/** oosqlBindDouble()
 *
 *
 */
RexxRoutine3(int, oosqlBindDouble_rtn, POINTER, _stmt, int32_t, index, double, value)
{
    sqlite3_stmt *stmt = routineStmt(context, _stmt);
    if ( stmt == NULL )
    {
        return SQLITE_MISUSE;
    }

    return sqlite3_bind_double(stmt, index, value);
}

/** oosqlBindInt()
 *
 *
 */
RexxRoutine3(int, oosqlBindInt_rtn, POINTER, _stmt, int32_t, index, int, value)
{
    sqlite3_stmt *stmt = routineStmt(context, _stmt);
    if ( stmt == NULL )
    {
        return SQLITE_MISUSE;
    }

    return sqlite3_bind_int(stmt, index, value);
}

/** oosqlBindInt64()
 *
 *
 */
RexxRoutine3(int, oosqlBindInt64_rtn, POINTER, _stmt, int32_t, index, int64_t, value)
{
    sqlite3_stmt *stmt = routineStmt(context, _stmt);
    if ( stmt == NULL )
    {
        return SQLITE_MISUSE;
    }

    return sqlite3_bind_int64(stmt, index, value);
}

/** oosqlBindNull()
 *
 *
 */
RexxRoutine2(int, oosqlBindNull_rtn, POINTER, _stmt, int32_t, index)
{
    sqlite3_stmt *stmt = routineStmt(context, _stmt);
    if ( stmt == NULL )
    {
        return SQLITE_MISUSE;
    }

    return sqlite3_bind_null(stmt, index);
}

/** oosqlBindParameterCount()
 *
 *
 */
RexxRoutine1(int, oosqlBindParameterCount_rtn, POINTER, _stmt)
{
    sqlite3_stmt *stmt = routineStmt(context, _stmt);
    if ( stmt == NULL )
    {
        return SQLITE_MISUSE;
    }

    return sqlite3_bind_parameter_count(stmt);
}

/** oosqlBindParameterIndex()
 *
 *
 */
RexxRoutine2(int, oosqlBindParameterIndex_rtn, POINTER, _stmt, CSTRING, name)
{
    sqlite3_stmt *stmt = routineStmt(context, _stmt);
    if ( stmt == NULL )
    {
        return SQLITE_MISUSE;
    }

    return sqlite3_bind_parameter_index(stmt, name);
}

/** oosqlBindParameterName()
 *
 *
 */
RexxRoutine2(RexxObjectPtr, oosqlBindParameterName_rtn, POINTER, _stmt, int, index)
{
    sqlite3_stmt *stmt = routineStmt(context, _stmt);
    if ( stmt == NULL )
    {
        return context->WholeNumber(SQLITE_MISUSE);
    }

    return safeRexxStringRx(context->threadContext, sqlite3_bind_parameter_name(stmt, index));
}

/** oosqlBindText()
 *
 *
 */
RexxRoutine3(int, oosqlBindText_rtn, POINTER, _stmt, int32_t, index, CSTRING, text)
{
    sqlite3_stmt *stmt = routineStmt(context, _stmt);
    if ( stmt == NULL )
    {
        return SQLITE_MISUSE;
    }

    return sqlite3_bind_text(stmt, index, text, -1, SQLITE_TRANSIENT);
}

/** oosqlBindValue()
 *
 *  Not implemented.  A pointer to a sqlite3_value (sqlite3_value *) is not
 *  fully understood at this point.
 *
 *  SQLite allows the implementation of application-defined SQL functions and
 *  aggregates.  A possible future enhancement of ooSQLite would be to allow
 *  application-defined SQL functions and aggregates to be written in Rexx.
 *  This method is a place-holder for that enhancement.
 *
 *  See also oosqlColumnValue()
 *
 *
 */
RexxRoutine3(int, oosqlBindValue_rtn, POINTER, _stmt, int32_t, index, RexxObjectPtr, value)
{
    sqlite3_stmt *stmt = routineStmt(context, _stmt);
    if ( stmt == NULL )
    {
        return SQLITE_MISUSE;
    }

    unsupportedRoutineException(context, "oosqlColumnValue");
    return SQLITE_MISUSE;
}

/** oosqlBindZeroBlob()
 *
 *
 */
RexxRoutine3(int, oosqlBindZeroBlob_rtn, POINTER, _stmt, int32_t, index, int, length)
{
    sqlite3_stmt *stmt = routineStmt(context, _stmt);
    if ( stmt == NULL )
    {
        return SQLITE_MISUSE;
    }

    return sqlite3_bind_zeroblob(stmt, index, length);
}

/** oosqlBusyHandler()
 *
 *  Installs a user defined busy handler.
 *
 *  @param  db       [required]  The open database connection that the busy
 *                   handler is registered (installed) for.
 *
 *  @param  rntName  [required]  The name of the routine that will be invoked
 *                   during a call back.  If this argument is the empty string,
 *                   then any existing registered callback is removed.
 *
 *  @param userData  [optional] This can be any Rexx object the user desires.
 *                   The object will be sent as the second argument to the busy
 *                   callback routine when it is invoked.  If this argument is
 *                   omitted, then only one argument is sent to the callback
 *                   routine.
 *
 *  @return  A sqlite result code.
 *
 *  @notes  By default, there is no busy handler installed.
 *
 *          There can only be one busy handler installed.  Setting a new busy
 *          handler automatically clears any previously installed handler.  Note
 *          that calling oosqlBusyTimeOut() can also set or clear the busy
 *          handler.
 *
 *          The busy handler should not take any actions which modify the
 *          database connection that invoked the busy handler. Any such actions
 *          result in undefined behavior.
 *
 *          A busy handler must not close the database connection or prepared
 *          statement that invoked the busy handler.
 */
RexxRoutine3(RexxObjectPtr, oosqlBusyHandler_rtn, POINTER, _db, CSTRING, rtnName, OPTIONAL_RexxObjectPtr, userData)
{
    sqlite3 *db = routineDB(context, _db);
    if ( db == NULL )
    {
        return context->WholeNumber(SQLITE_MISUSE);
    }

    return doCallbackSetupRtn(context, db, rtnName, userData, busyHandler, 0);
}

/** oosqlBusyTimeOut()
 *
 *
 */
RexxRoutine2(int, oosqlBusyTimeOut_rtn, POINTER, _db, int, ms)
{
    sqlite3 *db = routineDB(context, _db);
    if ( db == NULL )
    {
        return SQLITE_MISUSE;
    }

    int rc = sqlite3_busy_timeout(db, ms);
    helperDelCallback(context, db, busyHandler);

    return rc;
}

/** oosqlChanges()
 *
 *
 */
RexxRoutine1(int, oosqlChanges_rtn, POINTER, _db)
{
    sqlite3 *db = routineDB(context, _db);
    if ( db == NULL )
    {
        return SQLITE_MISUSE;
    }

    return sqlite3_changes(db);
}

/** oosqlClearBindings()
 *
 */
RexxRoutine1(int, oosqlClearBindings_rtn, POINTER, _stmt)
{
    sqlite3_stmt *stmt = routineStmt(context, _stmt);
    if ( stmt == NULL )
    {
        return SQLITE_MISUSE;
    }

    return sqlite3_clear_bindings(stmt);
}

/** oosqlClose()
 *
 */
RexxRoutine1(int, oosqlClose_rtn, POINTER, _db)
{
    sqlite3 *db = routineDB(context, _db);
    if ( db == NULL )
    {
        return SQLITE_MISUSE;
    }

    int rc = sqlite3_close(db);
    helperCleanCallbacks(context, db);

    return rc;
}

/** oodqlColumnBlob()
 *
 *
 *  @param col  [required]  The column number in the result row to get the BLOB
 *                          from.  Although SQLite uses 0-based column numbers,
 *                          the Rexx programmer should use 1-based numbers as is
 *                          normal for Rexx.
 */
RexxRoutine2(RexxObjectPtr, oosqlColumnBlob_rtn, POINTER, _stmt, int32_t, col)
{
    sqlite3_stmt *stmt = routineStmt(context, _stmt);
    if ( stmt == NULL )
    {
        return context->WholeNumber(SQLITE_MISUSE);
    }

    col--;
    const void *b = sqlite3_column_blob(stmt, col);
    size_t      s = sqlite3_column_bytes(stmt, col);

    RexxBufferObject result = context->NewBuffer(s);
    if ( result == NULLOBJECT )
    {
        outOfMemoryException(context->threadContext);
        return NULLOBJECT;
    }

    POINTER data = context->BufferData(result);
    memcpy(data, b, s);

    return result;
}

/** oosqlColumnBytes()
 *
 *
 *  @param col  [required]  The column number in the result row to get the
 *                          number of bytes containted in the value for that
 *                          column. Although SQLite uses 0-based column
 *                          numbers, the Rexx programmer should use 1-based
 *                          numbers as is normal for Rexx.
 */
RexxRoutine2(int, oosqlColumnBytes_rtn, POINTER, _stmt, int32_t, col)
{
    sqlite3_stmt *stmt = routineStmt(context, _stmt);
    if ( stmt == NULL )
    {
        return SQLITE_MISUSE;
    }

    col--;
    return sqlite3_column_bytes(stmt, col);
}

/** oosqlColumnCount()
 *
 *
 */
RexxRoutine1(int, oosqlColumnCount_rtn, POINTER, _stmt)
{
    sqlite3_stmt *stmt = routineStmt(context, _stmt);
    if ( stmt == NULL )
    {
        return SQLITE_MISUSE;
    }

    return sqlite3_column_count(stmt);
}

/** oosqlColumnDataBaseName()
 *
 *
 *  @param col  [required]  A column number in the result row.  SQLite requires
 *                          this.  Although SQLite uses 0-based column numbers,
 *                          the Rexx programmer should use 1-based numbers as is
 *                          normal for Rexx.
 *
 */
RexxRoutine2(RexxObjectPtr, oosqlColumnDatabaseName_rtn, POINTER, _stmt, int32_t, col)
{
    sqlite3_stmt *stmt = routineStmt(context, _stmt);
    if ( stmt == NULL )
    {
        return context->WholeNumber(SQLITE_MISUSE);
    }

    col--;
    CSTRING name = sqlite3_column_database_name(stmt, col);
    return safeRexxString(context->threadContext, name);
}

/** oosqlColumnDeclType()
 *
 *  Retrives the declared datatype of a query result.
 *
 *  @param col  [required]  The column number in the result row to get the
 *                          declared type for. Although SQLite uses 0-based
 *                          column numbers, the Rexx programmer should use
 *                          1-based numbers as is normal for Rexx.
 *
 *  @return
 *
 *  @note  The empty string is returned when sqlite returns a NULL pointer.
 */
RexxRoutine2(RexxObjectPtr, oosqlColumnDeclType_rtn, POINTER, _stmt, int32_t, col)
{
    sqlite3_stmt *stmt = routineStmt(context, _stmt);
    if ( stmt == NULL )
    {
        return context->WholeNumber(SQLITE_MISUSE);
    }

    col--;
    CSTRING name = sqlite3_column_decltype(stmt, col);
    return safeRexxStringRx(context->threadContext, name);
}

/** oosqlColumnDouble()
 *
 *
 *  @param col  [required]  The column number in the result row to get the
 *                          double value from. Although SQLite uses 0-based
 *                          column numbers, the Rexx programmer should use
 *                          1-based numbers as is normal for Rexx.
 */
RexxRoutine2(double, oosqlColumnDouble_rtn, POINTER, _stmt, int32_t, col)
{
    sqlite3_stmt *stmt = routineStmt(context, _stmt);
    if ( stmt == NULL )
    {
        return SQLITE_MISUSE;
    }

    col--;
    return sqlite3_column_double(stmt, col);
}

/** oosqlColumnInt()
 *
 *
 *  @param col  [required]  The column number in the result row to get the
 *                          integer value from. Although SQLite uses 0-based
 *                          column numbers, the Rexx programmer should use
 *                          1-based numbers as is normal for Rexx.
 */
RexxRoutine2(intptr_t, oosqlColumnInt_rtn, POINTER, _stmt, int32_t, col)
{
    sqlite3_stmt *stmt = routineStmt(context, _stmt);
    if ( stmt == NULL )
    {
        return SQLITE_MISUSE;
    }

    col--;
    return sqlite3_column_int(stmt, col);
}

/** oosqlColumnInt64()
 *
 *
 *  @param col  [required]  The column number in the result row to get the
 *                          64-bit integer value from. Although SQLite uses
 *                          0-based column numbers, the Rexx programmer should
 *                          use 1-based numbers as is normal for Rexx.
 */
RexxRoutine2(int64_t, oosqlColumnInt64_rtn, POINTER, _stmt, int32_t, col)
{
    sqlite3_stmt *stmt = routineStmt(context, _stmt);
    if ( stmt == NULL )
    {
        return SQLITE_MISUSE;
    }

    col--;
    return sqlite3_column_int64(stmt, col);
}

/** oosqlColumnName()
 *
 *  Returns the name assigned to a particular column in the result set of a
 *  SELECT statement.
 *
 *  @param stmt [required]  A prepared statment to get the column name from.
 *                          This must not be a null statement.
 *
 *  @param col  [required]  The column number in the result row whose origin
 *                          (column) name is desired.  Although SQLite uses
 *                          0-based column numbers, the Rexx programmer should
 *                          use 1-based numbers as is normal for Rexx.
 *
 */
RexxRoutine2(RexxObjectPtr, oosqlColumnName_rtn, POINTER, _stmt, int32_t, col)
{
    sqlite3_stmt *stmt = routineStmt(context, _stmt);
    if ( stmt == NULL )
    {
        return context->WholeNumber(SQLITE_MISUSE);
    }

    col--;
    CSTRING name = sqlite3_column_name(stmt, col);
    return safeRexxString(context->threadContext, name);
}

/** oosqlColumnOriginName()
 *
 *  Returns the original un-aliased name of the column in the result row
 *  specified.
 *
 *  @param col  [required]  The column number in the result row whose origin
 *                          (column) name is desired.  Although SQLite uses
 *                          0-based column numbers, the Rexx programmer should
 *                          use 1-based numbers as is normal for Rexx.
 *
 */
RexxRoutine2(RexxObjectPtr, oosqlColumnOriginName_rtn, POINTER, _stmt, int32_t, col)
{
    sqlite3_stmt *stmt = routineStmt(context, _stmt);
    if ( stmt == NULL )
    {
        return context->WholeNumber(SQLITE_MISUSE);
    }

    col--;
    CSTRING name = sqlite3_column_origin_name(stmt, col);
    return safeRexxString(context->threadContext, name);
}


/** oosqlColumnTableName()
 *
 *
 *  @param col  [required]  A column number in the result row.  SQLite requires
 *                          this.  Although SQLite uses 0-based column numbers,
 *                          the Rexx programmer should use 1-based numbers as is
 *                          normal for Rexx.
 *
 */
RexxRoutine2(RexxObjectPtr, oosqlColumnTableName_rtn, POINTER, _stmt, int32_t, col)
{
    sqlite3_stmt *stmt = routineStmt(context, _stmt);
    if ( stmt == NULL )
    {
        return context->WholeNumber(SQLITE_MISUSE);
    }

    col--;
    CSTRING name = sqlite3_column_table_name(stmt, col);
    return safeRexxString(context->threadContext, name);
}

/** oosqlColumnText()
 *
 *  @param stmt [required]  A prepared statment to get the column text from.
 *                          This must not be a null statement.
 *
 *  @param col  [required]  The column in the result row to get the text for.
 *                          Although SQLite uses 0-based column numbers, the
 *                          Rexx programmer should use 1-based numbers as is
 *                          normal for Rexx.
 *
 */
RexxRoutine2(RexxObjectPtr, oosqlColumnText_rtn, POINTER, _stmt, int32_t, col)
{
    sqlite3_stmt *stmt = routineStmt(context, _stmt);
    if ( stmt == NULL )
    {
        return context->WholeNumber(SQLITE_MISUSE);
    }

    col--;
    return context->String(safeColumnText(stmt, col));
}

/** oosqlColumnType()
 *
 *
 *  @param col  [required]  The column in the result row to get the type of.
 *                          Although SQLite uses 0-based column numbers, the
 *                          Rexx programmer should use 1-based numbers as is
 *                          normal for Rexx.
 */
RexxRoutine2(int, oosqlColumnType_rtn, POINTER, _stmt, int32_t, col)
{
    sqlite3_stmt *stmt = routineStmt(context, _stmt);
    if ( stmt == NULL )
    {
        return SQLITE_MISUSE;
    }

    col--;
    return sqlite3_column_type(stmt, col);
}

/** oosqlColumnValue()
 *
 *  Not implemented.  A pointer to a sqlite3_value (sqlite3_value *) is not
 *  fully understood at this point.  From the SQLite docs:
 *
 *  The object returned by sqlite3_column_value() is an unprotected
 *  sqlite3_value object. An unprotected sqlite3_value object may only be used
 *  with sqlite3_bind_value() and sqlite3_result_value(). If the unprotected
 *  sqlite3_value object returned by sqlite3_column_value() is used in any other
 *  way, including calls to routines like sqlite3_value_int(),
 *  sqlite3_value_text(), or sqlite3_value_bytes(), then the behavior is
 *  undefined.
 *
 *  SQLite allows the implementation of application-defined SQL functions and
 *  aggregates.  A possible future enhancement of ooSQLite would be to allow
 *  application-defined SQL functions and aggregates to be written in Rexx.
 *  This method is a place-holder for that enhancement.
 *
 *  See also oosqlBindValue()
 *
 */
RexxRoutine2(int, oosqlColumnValue_rtn, POINTER, _stmt, int32_t, index)
{
    sqlite3_stmt *stmt = routineStmt(context, _stmt);
    if ( stmt == NULL )
    {
        return SQLITE_MISUSE;
    }

    unsupportedRoutineException(context, "oosqlColumnValue");
    return SQLITE_MISUSE;
}

/** oosqlCommitHook()
 *
 *  Registers a callback routine to be called whenever a transaction is
 *  committed.
 *
 *  @param  db       [required]  The open database connection that the commit
 *                   hook is registered (installed) for.
 *
 *  @param  rntName  [required]  The name of the routine that will be invoked
 *                   during a call back.  If this argument is the empty string,
 *                   then any existing registered callback is removed.
 *
 *  @param userData  [optional] This can be any Rexx object the user desires.
 *                   The object will be sent as the second argument to the
 *                   commit hook callback routine when it is called.  If this
 *                   argument is omitted, then only one argument is sent to the
 *                   callback routine.
 *
 *  @return  The userData argument to a previous call of the oosqlCommitHook()
 *           routine on the same database connection, or .nil if there is not
 *           one.
 *
 *  @notes  When the commit hook callback routine returns zero, the COMMIT
 *          operation is allowed to continue normally. If the commit hook
 *          callback returns non-zero, then the COMMIT is converted into a
 *          ROLLBACK.  The Rexx routine *must* return a whole number.
 */
RexxRoutine3(RexxObjectPtr, oosqlCommitHook_rtn, POINTER, _db, CSTRING, rtnName, OPTIONAL_RexxObjectPtr, userData)
{
    sqlite3 *db = routineDB(context, _db);
    if ( db == NULL )
    {
        return context->WholeNumber(SQLITE_MISUSE);
    }

    return doCallbackSetupRtn(context, db, rtnName, userData, commitHook, 0);
}

/** oosqlCompileOptionGet()
 *
 *
 *  @note  Returns "NULL" for the the null pointer return from sqlite.
 */
RexxRoutine1(RexxObjectPtr, oosqlCompileOptionGet_rtn, int, index)
{
    return safeRexxString(context->threadContext, sqlite3_compileoption_get(index));
}

/** oosqlCompileOptionUsed()
 *
 *
 */
RexxRoutine1(logical_t, oosqlCompileOptionUsed_rtn, CSTRING, name)
{
    return sqlite3_compileoption_used(name);
}

/** oosqlComplete()
 *
 */
RexxRoutine1(logical_t, oosqlComplete_rtn, CSTRING, sql)
{
    return sqlite3_complete(sql);
}

/** oosqlDataCount()
 *
 *
 */
RexxRoutine1(int, oosqlDataCount_rtn, POINTER, _stmt)
{
    sqlite3_stmt *stmt = routineStmt(context, _stmt);
    if ( stmt == NULL )
    {
        return SQLITE_MISUSE;
    }

    return sqlite3_data_count(stmt);
}

/** oosqlDbFileName()
 *
 *
 */
RexxRoutine2(RexxObjectPtr, oosqlDbFileName_rtn, POINTER, _db, CSTRING, name)
{
    sqlite3 *db = routineDB(context, _db);
    if ( db == NULL )
    {
        return context->UnsignedInt32(SQLITE_MISUSE);
    }

    return safeRexxStringRx(context->threadContext, sqlite3_db_filename(db, name));
}

/** oosqlDbHandle()
 *
 *
 */
RexxRoutine1(POINTER, oosqlDbHandle_rtn, POINTER, _stmt)
{
    sqlite3_stmt *stmt = routineStmt(context, _stmt);
    if ( stmt == NULL )
    {
        return context->WholeNumber(SQLITE_MISUSE);
    }

    return sqlite3_db_handle(stmt);
}

/** oosqlDbMutex()
 *
 *
 */
RexxRoutine1(POINTER, oosqlDbMutex_rtn, POINTER, _db)
{
    sqlite3 *db = routineDB(context, _db);
    if ( db == NULL )
    {
        return context->UnsignedInt32(SQLITE_MISUSE);
    }

    return sqlite3_db_mutex(db);
}


/** oosqlDbReadOnly()
 *
 *
 */
RexxRoutine2(int, oosqlDbReadOnly_rtn, POINTER, _db, CSTRING, name)
{
    sqlite3 *db = routineDB(context, _db);
    if ( db == NULL )
    {
        return SQLITE_MISUSE;
    }

    return sqlite3_db_readonly(db, name);
}

/** oodqlDbReleaseMemory()
 *
 *
 */
RexxRoutine1(int, oosqlDbReleaseMemory_rtn, POINTER, _db)
{
    sqlite3 *db = routineDB(context, _db);
    if ( db == NULL )
    {
        return SQLITE_MISUSE;
    }

    return sqlite3_db_release_memory(db);
}

/** oosqlDbStatus()
 *
 */
RexxRoutine4(int, oosqlDbStatus_rtn, POINTER, _db, int, param, RexxObjectPtr, _result, OPTIONAL_logical_t, reset)
{
    sqlite3 *db = routineDB(context, _db);
    if ( db == NULL )
    {
        return SQLITE_MISUSE;
    }

    return genDbStatus(context->threadContext, db, param, _result, reset, 2);
}

/** oosqlErrCode()
/** oosqlExtendedErrCode()
 *
 */
RexxRoutine1(int, oosqlErrCode_rtn, POINTER, _db)
{
    sqlite3 *db = routineDB(context, _db);
    if ( db == NULL )
    {
        return SQLITE_MISUSE;
    }

    return sqlite3_extended_errcode(db);
}

/** oosqlErrMsg()
 *
 */
RexxRoutine1(RexxObjectPtr, oosqlErrMsg_rtn, POINTER, _db)
{
    sqlite3 *db = routineDB(context, _db);
    if ( db == NULL )
    {
        return context->UnsignedInt32(SQLITE_MISUSE);
    }

    return context->String(sqlite3_errmsg(db));
}

/** oosqlExec()
 *
 *  Interface to the sqlite3_exec() function, which executes a SQL statement or
 *  statements in one step.
 *
 *  @param  db           The opened database connection.
 *
 *  @param  sql          A string containing the SQL statement or statements to
 *                       execute. More than 1 statement can be executed by
 *                       including semi-colons in the string.  Each SQL
 *                       statement is ended with a semi-colon.
 *
 *  @param  doCallback   Whether to use the call back feature of sqlite3_exec().
 *                       If false, then the 3rd argument is ignored, and the
 *                       return from exec() is a return code.
 *
 *                       If true and the rtnName arg is omitted then a result
 *                       set is returned containing all the rows produced by
 *                       executing the SQL.  This could be an empty array if the
 *                       SQL does not produce a result set.
 *
 *                       Otherwise, if rtnName is not omitted, then that
 *                       function is called one time for each result row coming
 *                       out of the evaluated SQL statements.  This exactly
 *                       mimics how sqlite3_exec() works.
 *
 *  @param  mthName      The routine name in the Rexx progarm that will be
 *                       called each time the SQL produces a row.  This argument
 *                       is ignored unless doCallBack is not omitted and is
 *                       true.
 *
 *  @return  A result code, one of the .ooSQL constants, or a result set as
 *           described above.
 */
RexxRoutine4(RexxObjectPtr, oosqlExec_rtn, POINTER, _db, CSTRING, sql, OPTIONAL_logical_t, doCallback,
             OPTIONAL_CSTRING, rtnName)
{
    sqlite3 *db = routineDB(context, _db);
    if ( db == NULL )
    {
        return context->UnsignedInt32(SQLITE_MISUSE);
    }

    CGenericCallback  cbc    = {0};
    int               rc     = SQLITE_ERROR;
    char             *errMsg = NULL;

    if ( doCallback )
    {
        cbc.callbackContext = context->threadContext;

        if ( argumentExists(4) )
        {
            cbc.callbackRtn = getCallerRoutine(context, rtnName);
            if ( cbc.callbackRtn == NULLOBJECT )
            {
                return noSuchRoutineException(context->threadContext, rtnName, 4);
            }
        }
        else
        {
            cbc.useArray = true;
            cbc.records  = context->NewArray(10);
        }

        rc = sqlite3_exec(db, sql, execCallBack, (void *)&cbc, &errMsg);
    }
    else
    {
        rc = sqlite3_exec(db, sql, NULL, NULL, &errMsg);
    }

    RexxObjectPtr result = context->WholeNumber(rc);

    if ( rc != SQLITE_OK )
    {
        if ( errMsg != NULL )
        {
            result = ooSQLiteErr(context->threadContext, rc, errMsg, true);
            sqlite3_free(errMsg);
        }
        else
        {
            result = ooSQLiteErr(context->threadContext, rc, "no error message available from SQLite", true);
        }
        return result;
    }

    if ( cbc.useArray )
    {
        return cbc.records;
    }

    return context->WholeNumber(rc);
}

/** oosqlExtendedResultCodes()
 *
 *  This is a noop, for the ooSQLite classic Rexx interface.  Extended result
 *  codes are always turned on in the oosqlOpen() routine for each database
 *  connection.
 *
 *  @param  _db
 *  @param  onOff
 *
 *  The _db and onOff parameters are only present to make the routine look the
 *  same as sqlite3_extended_result_codes()
 */
RexxRoutine2(int, oosqlExtendedResultCodes_rtn, POINTER, _db, int, onOff)
{
    return SQLITE_OK;
}

/** oosqlFinalize()
 *
 *
 *  @note  The SQLite docs say that finializing a null stmt is a harmless no-op.
 *
 *         Also, if I'm interpreting the sqlite docs correctly, although an
 *         error code might be returned, the prepared statement is always
 *         deleted. The, possible, error code is the return from a previous
 *         error.  Such as an error from a previously executed step.
 */
RexxRoutine1(int, oosqlFinalize_rtn, POINTER, stmt)
{
    return sqlite3_finalize((sqlite3_stmt *)stmt);
}

/** oosqlGetAutocommit()
 *
 *
 */
RexxRoutine1(logical_t, oosqlGetAutocommit_rtn, POINTER, _db)
{
    sqlite3 *db = routineDB(context, _db);
    if ( db == NULL )
    {
        return 0;
    }

    return sqlite3_get_autocommit(db);
}

/** oosqlInterrupt()
 *
 *
 */
RexxRoutine1(int, oosqlInterrupt_rtn, POINTER, _db)
{
    sqlite3 *db = routineDB(context, _db);
    if ( db == NULL )
    {
        return SQLITE_MISUSE;
    }

    sqlite3_interrupt(db);

    return SQLITE_OK;
}

/** oosqlLastInsertRowID()
 *
 *
 */
RexxRoutine1(int64_t, oosqlLastInsertRowID_rtn, POINTER, _db)
{
    sqlite3 *db = routineDB(context, _db);
    if ( db == NULL )
    {
        return SQLITE_MISUSE;
    }

    return sqlite3_last_insert_rowid(db);
}


/** oosqlLibVersion()
 *  oosqlLibVersionNumber()
 *  oosqlSourceID()
 *
 */
RexxRoutine1(RexxObjectPtr, oosqlLibVersion_rtn, NAME, name)
{
    const char *v;

    size_t len = strlen(name);

    if ( len == 15 )
    {
        v = sqlite3_libversion();
    }
    else if ( len == 21)
    {
        return context->Int32(sqlite3_libversion_number());
    }
    else
    {
        v = sqlite3_sourceid();
    }

    return context->String(v);
}

/** oosqlLimit()
 *
 *
 */
RexxRoutine3(int, oosqlLimit_rtn, POINTER, _db, int, id, int, value)
{
    sqlite3 *db = routineDB(context, _db);
    if ( db == NULL )
    {
        return SQLITE_MISUSE;
    }

    return sqlite3_limit(db, id, value);
}

/** oosqlMemoryHighWater()
 *
 *
 */
RexxRoutine1(int64_t, oosqlMemoryHighWater_rtn, logical_t, reset)
{
    return sqlite3_memory_highwater((int)reset);
}

/** oosqlMemoryUsed()
 *
 *
 */
RexxRoutine0(int64_t, oosqlMemoryUsed_rtn)
{
    return sqlite3_memory_used();
}

/** oosqlMutexAlloc()
 *
 *  @param type  [required]  The SQLite muxtex type.  Can only be:
 *               SQLITE_MUTEX_RECURSIVE or SQLITE_MUTEX_FAST.
 *
 *
 */
RexxRoutine1(POINTER, oosqlMutexAlloc_rtn, int, type)
{
    if ( ! (type == SQLITE_MUTEX_FAST || type == SQLITE_MUTEX_RECURSIVE)  )
    {
        wrongArgValueException(context->threadContext, 1, "MUTEX_RECURSIVE or MUTEX_FAST",
                               context->WholeNumber(type));
        return NULL;
    }

    return sqlite3_mutex_alloc(type);;
}

/** oosqlMutexEnter()
 *
 *  Enters this mutex.  Will block if the mutex is held by another thread.
 *
 *  @returns  Always returns OK, has no meaning.
 */
RexxRoutine1(int, oosqlMutexEnter_rtn, POINTER, _mtx)
{
    sqlite3_mutex *mtx = routineMutex(context, _mtx);
    if ( mtx == NULL )
    {
        return SQLITE_MISUSE;
    }

    sqlite3_mutex_enter(mtx);

    return SQLITE_OK;
}


/** oosqlMutexFree()
 *
 *  Frees this mutex.  The mutex must not be in use.
 *
 *  @returns  Always returns OK, has no meaning.
 */
RexxRoutine1(int, oosqlMutexFree_rtn, POINTER, _mtx)
{
    sqlite3_mutex *mtx = routineMutex(context, _mtx);
    if ( mtx == NULL )
    {
        return SQLITE_MISUSE;
    }

    sqlite3_mutex_free(mtx);

    return SQLITE_OK;
}


/** oosqlMutexLeave()
 *
 *  Leaves this mutex.
 *
 *  @returns  Always returns OK, has no meaning.
 */
RexxRoutine1(int, oosqlMutexLeave_rtn, POINTER, _mtx)
{
    sqlite3_mutex *mtx = routineMutex(context, _mtx);
    if ( mtx == NULL )
    {
        return SQLITE_MISUSE;
    }

    sqlite3_mutex_leave(mtx);

    return SQLITE_OK;
}


/** ooSQLiteMutex::try
 *
 *  Trys to enter this mutex.  Will return immediatley if the mutex is held by
 *  another thread.
 *
 *  @returns  BUSY if the mutex is held by another thread, otherwise returns OK.
 */
RexxRoutine1(int, oosqlMutexTry_rtn, POINTER, _mtx)
{
    sqlite3_mutex *mtx = routineMutex(context, _mtx);
    if ( mtx == NULL )
    {
        return SQLITE_MISUSE;
    }

    return sqlite3_mutex_try(mtx);
}


/** oosqlOpen()
 *
 *  Open a database connection.
 *
 *  @param file     The file name of the database to open.
 *
 *  @param flags    Flags that control how the database is opened..
 *
 *  @param vfsName  The name of the alternative operating system interface to
 *                  use.  This option is ignored by ooSQLite at this time.
 *
 *  @return  The connection to the database.  The connection is an opaque handle
 *           returned to the ooSQLite programmer as a Rexx Pointer object.
 *
 *  @note    The connection is (almost) always returned, even on error.  The
 *           programmer should immediately use oosqlErrCode() to check for
 *           error.  If oosqlErrCode() does not return .ooSQL~OK then the
 *           database was not opened properly and the connection should not be
 *           used and must be closed.
 *
 *           oosqlClose() must always be called to properly close the database,
 *           even if it was opened with an error.
 *
 *           The only exception to this is if the error code is .ooSQL~NOMEM, in
 *           which case no connection was created.  The returned Pointer is null
 *           and should not be passed to oosqlClose().
 */
RexxRoutine3(POINTER, oosqlOpen_rtn, CSTRING, file, OPTIONAL_int32_t, _flags, OPTIONAL_CSTRING, vfsName)
{
    sqlite3 *db;

    int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    if ( argumentExists(2) )
    {
        flags = _flags;
    }

    int rc = sqlite3_open_v2(file, &db, flags, NULL);

    if ( rc == SQLITE_OK )
    {
        // Here we enable things that are enabled / disabled on a per database
        // connection basis that are set by default for ooSQLite.
        sqlite3_extended_result_codes(db, 1);
    }

    return db;
}

/** oosqlNextStmt()
 *
 *  Finds the next prepared statement associated with the specified database
 *  connection.
 *
 *  @param  db    [required] An open database connection.
 *  @param  stmt  [optinal]  The statment to start the search with. If omitted,
 *                or .nil then the returned statement will be the first prepared
 *                statement, if there is one.
 *
 *  @return Returns the next prepared statement after stmt associated with the
 *          database connection db. If stmt is .nil, or omitted, then returns
 *          the first prepared statement associated with the database
 *          connection.  If no prepared statement satisfies the conditions,
 *          returns .nil.
 *
 */
RexxRoutine2(RexxObjectPtr, oosqlNextStmt_rtn, POINTER, _db, OPTIONAL_RexxObjectPtr, _stmt)
{
    sqlite3 *db = routineDB(context, _db);
    if ( db == NULL )
    {
        return NULLOBJECT;
    }

    sqlite3_stmt *stmt = NULL;

    if ( argumentExists(2) )
    {
        if ( _stmt == TheNilObj )
        {
            ; // Do nothing, this is okay and *stmt is already set to null.
        }
        else if ( ! context->IsPointer(_stmt) )
        {
            return wrongArgValueException(context->threadContext, 2, "a prepared statement or nil", _stmt);
        }
        else
        {
            stmt = (sqlite3_stmt *)context->PointerValue((RexxPointerObject)_stmt);
        }
    }

    sqlite3_stmt *nextStmt = sqlite3_next_stmt(db, stmt);

    RexxObjectPtr result = TheNilObj;
    if ( nextStmt != NULL )
    {
        result = context->NewPointer(nextStmt);
    }

    return result;
}

/** oosqlPrepare()
 *
 *  @param db  [required]  The database connection the statement is for.
 *
 *  @param sql [required]  The SQL statement(s) used for this prepared
 *                         statement.
 *
 *
 *  @remarks  Testing has shown that if ppTail is not null when passed into
 *            sqlite3_prepare_v2() it is always set to a zero-terminated string.
 *            Even when there is not 'tail', i.e. it is set to an empty string.
 */
RexxRoutine3(POINTER, oosqlPrepare_rtn, POINTER, _db, CSTRING, sql, OPTIONAL_RexxObjectPtr, _tail)
{
    sqlite3_stmt *stmt   = NULL;
    const char   *tail   = NULL;
    const char  **ppTail = NULL;

    sqlite3 *db = routineDB(context, _db);
    if ( db == NULL )
    {
        goto done_out;
    }

    if ( argumentExists(3) )
    {
        if ( ! context->IsOfType(_tail, "MutableBuffer") )
        {
            wrongClassException(context->threadContext, 1, OOSQLITEDB_CLASS);
            goto done_out;
        }

        ppTail = &tail;
        context->SendMessage1(_tail, "SETBUFFERSIZE", TheZeroObj);
    }

    if ( sqlite3_prepare_v2(db, sql, (int)strlen(sql) + 1, &stmt, ppTail) == SQLITE_OK )
    {
        if ( tail != NULL )
        {
            context->SendMessage1(_tail, "APPEND", context->String(tail));
        }
    }

done_out:
    return stmt;
}

/** oosqlProfile()
 *
 *  Registers an user callback routine used for profiling
 *
 *  @param  db       [required]  The open database connection that the profile
 *                   callback is registered (installed) for.
 *
 *  @param  rntName  [required]  The name of the routine that will be invoked
 *                   during a call back.  If this argument is the empty string,
 *                   then any existing registered callback is removed.
 *
 *  @param userData  [optional] This can be any Rexx object the user desires.
 *                   The object will be sent as the third argument to the
 *                   profile callback routine when it is called.  If this
 *                   argument is omitted then the third argume to the profile
 *                   callback will be the .nil object.
 *
 *  @return  The previous userDate argument to the oosqlProfile() routine on
 *           this database connection, if there was one.  If there was not a
 *           previous call then the .nil object is returned.
 *
 *  @notes  By default, there is no profile callback installed.
 *
 *          There can only be one profile callback per database connection.
 *          Setting a new profile callback automatically clears any previously
 *          installed callback.
 *
 *          The first argument sent to the callback is the original statement
 *          text.  The second argument is an estimate of wall-clock time of how
 *          long that statement took to run. The profile callback time is in
 *          units of nanoseconds, however the current implementation is only
 *          capable of millisecond resolution so the six least significant
 *          digits in the time are meaningless.
 *
 *          The third arugment is the userData argument to this routine, if not
 *          omitted.  If userData is omitted then the third argument is .nil.
 *
 *          Future versions of SQLite might provide greater resolution on the
 *          profiler callback. The sqlite3_profile() function is considered
 *          experimental and is subject to change in future versions of SQLite.
 */
RexxRoutine3(RexxObjectPtr, oosqlProfile_rtn, POINTER, _db, CSTRING, rtnName, OPTIONAL_RexxObjectPtr, userData)
{
    sqlite3 *db = routineDB(context, _db);
    if ( db == NULL )
    {
        return context->WholeNumber(SQLITE_MISUSE);
    }

    return doCallbackSetupRtn(context, db, rtnName, userData, profileHook, 0);
}

/** oosqlProgressHandler()
 *
 *  Registers an user callback routine that is called periodically during long
 *  running calls to oosqlExec(), and oosqlStep() for this database connection.
 *  An example use for this interface is to keep a GUI updated during a large
 *  query.
 *
 *  @param  db           [required]  The open database connection that the
 *                       progress callback is registered (installed) for.
 *
 *  @param  rntName      [required]  The name of the routine that will be
 *                       invoked during a call back.  If this argument is the
 *                       empty string, then any existing registered callback is
 *                       removed.
 *
 *  @param  instructions [required]  the number of virtual machine instructions
 *                       that are evaluated between successive calls of the
 *                       callback.  If this argument is less than 1, it also has
 *                       the effect of removing any installed callback.
 *
 *  @param userData      [optional] This can be any Rexx object the user
 *                       desires. The object will be sent as the first and only
 *                       argument to the progress handler callback routine when
 *                       it is invoked. This argument is ignored when  the
 *                       rtnName argument is the empty string.  If this argument
 *                       is omitted and the rtnName argument is not the empty
 *                       string, then the .nil object is sent as the only
 *                       argument to the progress handler callback routine.
 *
 *  @return  A sqlite result code.
 *
 *  @notes  By default, there is no progress handler callback installed.
 *
 *          There can only be one progress callback per database connection.
 *          Setting a new progress callback automatically clears any previously
 *          installed callback.
 *
 *          The only argument sent to the callback is the userData argument, if
 *          supplied here, or .nil if userData is omitted.
 *
 *          If the callback routine returns non-zero, then the operation is
 *          interrupted.  This could be used to implement a Cancel button in a
 *          GUI application.
 */
RexxRoutine4(RexxObjectPtr, oosqlProgressHandler_rtn, POINTER, _db, CSTRING, rtnName, int, instructions,
             OPTIONAL_RexxObjectPtr, userData)
{
    sqlite3 *db = routineDB(context, _db);
    if ( db == NULL )
    {
        return context->WholeNumber(SQLITE_MISUSE);
    }

    if ( instructions < 1 )
    {
        rtnName = "";
    }

    return doCallbackSetupRtn(context, db, rtnName, userData, progressHandler, instructions);
}

/** oosqlReleaseMemory()
 *
 *
 */
RexxRoutine1(int, oosqlReleaseMemory_rtn, int, nBytes)
{
    return sqlite3_release_memory(nBytes);
}

/** oosqlReset()
 *
 *
 */
RexxRoutine1(int, oosqlReset_rtn, POINTER, _stmt)
{
    sqlite3_stmt *stmt = routineStmt(context, _stmt);
    if ( stmt == NULL )
    {
        return SQLITE_MISUSE;
    }

    return sqlite3_reset(stmt);
}

/** oosqlRollbackHook()
 *
 *  Registers a callback routine to be called whenever a transaction is rolled
 *  back.
 *
 *  @param  db       [required]  The open database connection that the rollback
 *                   hook is registered (installed) for.
 *
 *  @param  rntName  [required]  The name of the routine that will be invoked
 *                   during a call back.  If this argument is the empty string,
 *                   then any existing registered callback is removed.
 *
 *  @param userData  [optional] This can be any Rexx object the user desires.
 *                   The object will be sent as the first argument and only
 *                   arguemnt to the rollback hook callback routine when it is
 *                   calle. If this argument is omitted, then no argument is
 *                   sent to the callback routine.
 *
 *  @return  The userData argument to a previous call of the oosqlRollBackHook()
 *           routine on the same database connection, or .nil if there is not
 *           one.
 *
 *  @note    The Rexx callback routine *must* return a whole number, but that
 *           return is ignored by SQLite.
 */
RexxRoutine3(RexxObjectPtr, oosqlRollbackHook_rtn, POINTER, _db, CSTRING, rtnName, OPTIONAL_RexxObjectPtr, userData)
{
    sqlite3 *db = routineDB(context, _db);
    if ( db == NULL )
    {
        return context->WholeNumber(SQLITE_MISUSE);
    }

    return doCallbackSetupRtn(context, db, rtnName, userData, rollbackHook, 0);
}

/** oosqlSetAuthorizer()
 *
 *  Registers an user callback routine that is invoked as SQL statements are
 *  being compiled by oosqlPrepare().  At various points during the compilation
 *  process, as logic is being created to perform various actions, the
 *  authorizer callback is called to see if those actions are allowed.
 *
 *  @param  db       [required]  The open database connection that the
 *                   authorizer callback is registered (installed) for.
 *
 *  @param  rntName  [required]  The name of the routine that will be invoked
 *                   during a callback.  If this argument is the empty string,
 *                   then any existing registered callback is removed.
 *
 *  @param userData  [optional] This can be any Rexx object the user desires.
 *                   The object will be sent as the last argument to the
 *                   authorizer callback routine when it is called. This
 *                   argument is ignored when the rtnName argument is the empty
 *                   string. If this argument is omitted and the rtnName
 *                   argument is not the empty string, then the .nil object is
 *                   sent as the last argument to the callback routine.
 *
 *  @return  A sqlite result code.  Although at this time, SQLite always returns
 *           SQLITE_OK.
 *
 *  @notes  By default, there is no authorizer callback installed.
 *
 *          There can only be one authorizer callback per database connection.
 *          Setting a new authorizer callback automatically clears any
 *          previously installed callback.
 *
 *          The arguments sent to the callback are:
 *
 *          1.) A whole number authorizer action code.
 *
 *          2.) String #1.
 *
 *          3.) String #2.
 *
 *          4.) String #3.
 *
 *          5.) String #4.
 *
 *          6.) The userData arg if supplied, otherwise the .nil object if
 *          userData is omitted.
 *
 *          The SQLite docs do not explain what strings 1 through 4 are, other
 *          than to say the arguments: "are zero-terminated strings that contain
 *          additional details about the action to be authorized."
 *
 *          The callback routine must return one of 3 codes in response:
 *          SQLITE_OK allows the action.  SQLITE_IGNORE disallows the specific
 *          action but allows the SQL statement to continue to be compiled.
 *          SQLITE_DENY causes the entire SQL statement to be rejected with an
 *          error.
 *
 *          Any other return causes the oosqlPrepare() call that triggered the
 *          authorizer to fail with an error message.
 */
RexxRoutine3(RexxObjectPtr, oosqlSetAuthorizer_rtn, POINTER, _db, CSTRING, rtnName, OPTIONAL_RexxObjectPtr, userData)
{
    sqlite3 *db = routineDB(context, _db);
    if ( db == NULL )
    {
        return context->WholeNumber(SQLITE_MISUSE);
    }

    return doCallbackSetupRtn(context, db, rtnName, userData, authorizer, 0);
}

/** oosqlSoftHeapLimit64()
 *
 *  Imposes a limit on the heap size, or queries the current size.
 *
 *  @param n  A 64-bit signed whole number.  If n is positive the soft heap
 *            limit is set to its value.  If n is zero then the soft heap limit
 *            is disabled.  If n is negative then no change is made to the
 *            current limit.
 *
 *  @return  Returns the soft heap limit before the method was invoked, or a
 *           negative number on error.
 *
 *  @note  Use a negative n to query the current size.
 *
 */
RexxRoutine1(int64_t, oosqlSoftHeapLimit64_rtn, int64_t, n)
{
    return sqlite3_soft_heap_limit64(n);
}

/** oosqlSql()
 *
 */
RexxRoutine1(RexxObjectPtr, oosqlSql_rtn, POINTER, _stmt)
{
    sqlite3_stmt *stmt = routineStmt(context, _stmt);
    if ( stmt == NULL )
    {
        return context->WholeNumber(SQLITE_MISUSE);
    }

    return safeRexxStringRx(context->threadContext, sqlite3_sql(stmt));
}

/** oosqlStatus()
 *
 */
RexxRoutine3(int, oosqlStatus_rtn, int, param, RexxObjectPtr, _result, OPTIONAL_logical_t, reset)
{
    return genStatus(context->threadContext, param, _result, reset);
}

/** oosqlStep()
 *
 */
RexxRoutine1(int, oosqlStep_rtn, POINTER, _stmt)
{
    sqlite3_stmt *stmt = routineStmt(context, _stmt);
    if ( stmt == NULL )
    {
        return SQLITE_MISUSE;
    }

    return sqlite3_step(stmt);
}

/** oosqlStmtBusy()
 *
 *  The oosqlStmtBusy(stmt) routine returns true if the prepared statement has
 *  been stepped at least once using oosqlStep() but has not run to completion
 *  and/or has not been reset using oosqlReset().
 *
 *  @param stmt  The statment to check.
 *
 *  @return True or false.
 *
 *  @note  We allow stmt to be null, i.e. the statement has already been
 *         finalized, for this method because SQLite returns false if stmt is
 *         null.
 */
RexxRoutine1(logical_t, oosqlStmtBusy_rtn, POINTER, _stmt)
{
    sqlite3_stmt *stmt = (sqlite3_stmt *)_stmt;
    return sqlite3_stmt_busy(stmt);
}

/** ooSQLiteStmt::stmtReadonly()
 *
 *
 *
 */
RexxRoutine1(logical_t, oosqlStmtReadonly_rtn, POINTER, _stmt)
{
    sqlite3_stmt *stmt = routineStmt(context, _stmt);
    if ( stmt == NULL )
    {
        return SQLITE_MISUSE;
    }

    return sqlite3_stmt_readonly(stmt);
}

/** oosqlStmtStatus()
 *
 *
 */
RexxRoutine3(int, oosqlStmtStatus_rtn, POINTER, _stmt, int, param, OPTIONAL_logical_t, reset)
{
    sqlite3_stmt *stmt = routineStmt(context, _stmt);
    if ( stmt == NULL )
    {
        return SQLITE_MISUSE;
    }

    return sqlite3_stmt_status(stmt, param, (int)reset);
}

/** oosqlTableColumnMetadata()
 *
 *
 */
RexxRoutine5(int, oosqlTableColumnMetadata_rtn, POINTER, _db, CSTRING, dbName, CSTRING, tableName,
             CSTRING, colName, RexxObjectPtr, results)
{
    sqlite3 *db = routineDB(context, _db);
    if ( db == NULL )
    {
        return SQLITE_MISUSE;
    }

    return genTableColumnMetadata(context->threadContext, db, dbName, tableName, colName, results, 5);
}

/** oosqlThreadSafe()
 *
 *
 */
RexxRoutine0(logical_t, oosqlThreadSafe_rtn)
{
    return sqlite3_threadsafe();
}

/** oosqlTotalChanges()
 *
 *
 */
RexxRoutine1(int, oosqlTotalChanges_rtn, POINTER, _db)
{
    sqlite3 *db = routineDB(context, _db);
    if ( db == NULL )
    {
        return SQLITE_MISUSE;
    }

    return sqlite3_total_changes(db);
}

/** ooSQLiteDB::trace()
 *
 *  Registers an user callback routine used for tracing.
 *
 *  @param  db       [required]  The open database connection that the profile
 *                   callback is registered (installed) for.
 *
 *  @param  rntName  [required]  The name of the routine that will be invoked
 *                   during a call back.  If this argument is the empty string,
 *                   then any existing registered callback is removed.
 *
 *  @param userData  [optional] This can be any Rexx object the user desires.
 *                   The object will be sent as the second argument to the trace
 *                   callback routine when it is called. If this argument is
 *                   omitted then the second argument set to the callback will
 *                   be the .nil object.
 *
 *  @return  The userData argument to the previous call to the oosqlTrace()
 *           routine, if there was a previous call.  Otherwise the .nil object
 *           is returned.
 *
 *  @notes  By default, there is no trace callback installed.
 *
 *          There can only be one trace callback per database connection.
 *          Setting a new profile callback automatically clears any previously
 *          installed callback.
 *
 *          The first argument sent to the callback is the original SQL
 *          statement text, or a comment identifying a trigger if the callback
 *          is invoked as the result of a trigger.
 *
 *          The second arugment is the userData argument to this method, if not
 *          omitted.  If userData is omitted then the second argument is .nil.
 */
RexxRoutine3(RexxObjectPtr, oosqlTrace_rtn, POINTER, _db, CSTRING, rtnName, OPTIONAL_RexxObjectPtr, userData)
{
    sqlite3 *db = routineDB(context, _db);
    if ( db == NULL )
    {
        return context->WholeNumber(SQLITE_MISUSE);
    }

    return doCallbackSetupRtn(context, db, rtnName, userData, traceHook, 0);
}

/** oosqlUpdateHook()
 *
 *  Registers a callback routine to be invoked whenever a row is updated,
 *  inserted or deleted.
 *
 *  @param  db       [required]  The open database connection that the update
 *                   hook is registered (installed) for.
 *
 *  @param  rntName  [required]  The name of the routine that will be invoked
 *                   during a call back.  If this argument is the empty string,
 *                   then any existing registered callback is removed.
 *
 *  @param userData  [optional] This can be any Rexx object the user desires.
 *                   The object will be sent as the fifth argument to the
 *                   callback routine when it is called.  If this argument is
 *                   omitted, then only 4 arguments are sent to the callback
 *                   routine.
 *
 *  @return  The userData argument to a previous call of the oosqlUpdateHook()
 *           routine on this database connection object, or .nil if there is not
 *           a previous invocation.
 *
 *  @note    The Rexx callback method *must* return a whole number, but that
 *           return is ignored by SQLite.
 *
 *           The first 4 arguments to the callback method, in order, are:
 *
 *           1.) An integer constant, one of INSERT, DELETE, or UPDATE,
 *           depending on what the operation was.
 *
 *           2.) The database name containing the affected row.
 *
 *           3.) The table name containing the affected row.
 *
 *           4.) The affected row's rowID. In the case of an update, this is the
 *           rowID after the update takes place.
 *
 *           5.) The fifth argument will be the userData argument to
 *           oosqlUpdateHook(), if it was not omitted.  If it was omitted, then
 *           only 4 arugments are sent to oodqlUpdateHook().
 */
RexxRoutine3(RexxObjectPtr, oosqlUpdateHook_rtn, POINTER, _db, CSTRING, rtnName, OPTIONAL_RexxObjectPtr, userData)
{
    sqlite3 *db = routineDB(context, _db);
    if ( db == NULL )
    {
        return context->WholeNumber(SQLITE_MISUSE);
    }

    return doCallbackSetupRtn(context, db, rtnName, userData, updateHook, 0);
}

/** oosqlVersion()
 *
 *
 */
RexxRoutine0(RexxStringObject, oosqlVersion_rtn)
{
    return context->String(sqlite3_version);
}




/** oosqlTest()
 *
 *  Private routine to use for quick tests.  Code varies as to what it does.
 */
RexxRoutine1(RexxObjectPtr, oosqlTest_rtn, CSTRING, callBack)
{
    RexxObjectPtr result = TheZeroObj;

    RexxRoutineObject rtn = getCallerRoutine(context, callBack);
    if ( rtn != NULLOBJECT )
    {
        RexxArrayObject args = context->ArrayOfOne(context->String("Got me"));
        result = context->CallRoutine(rtn, args);
    }

    return result;
}


#define __rtn_helper_class "__rtn_helper_class"


/** __rtn_helper_class:Class::init()
 */
RexxMethod1(RexxObjectPtr, hlpr_init_cls, OSELF, self)
{
    if ( isOfClassType(context, self, __rtn_helper_class) )
    {
        TheRoutineHelperClass = (RexxClassObject)self;
        context->RequestGlobalReference(TheRoutineHelperClass);
    }
    return NULLOBJECT;
}



#define __rtn_db_callback_class "__rtn_db_callback_class"


/** Either a bug or the Rexx docs are incorrect.  Using a POINTER arg is
 *  documented as accepting a Buffer object.  But if a RexxBufferObject is
 *  passed then we get an Argument 1 must be of the Pointer class error.
 */
RexxMethod1(int, db_cb_releaseBuffer, RexxObjectPtr, buffer)
{
    pCGenericCallback cbc = (pCGenericCallback)context->BufferData((RexxBufferObject)buffer);
    printf("db_cb_releaseBuffer routine name=%s\n", cbc->routineName);

    if ( cbc->userData != NULLOBJECT )
    {
        context->ReleaseGlobalReference(cbc->userData);
    }

    return 0;
}




REXX_TYPED_ROUTINE_PROTOTYPE(ooSQLiteVersion_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(ooSQLiteMerge_rtn);

REXX_TYPED_ROUTINE_PROTOTYPE(oosqlBindBlob_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlBindDouble_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlBindInt_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlBindInt64_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlBindNull_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlBindParameterCount_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlBindParameterIndex_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlBindParameterName_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlBindText_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlBindValue_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlBindZeroBlob_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlBusyHandler_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlBusyTimeOut_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlChanges_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlClearBindings_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlClose_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlColumnBlob_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlColumnBytes_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlColumnCount_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlColumnDatabaseName_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlColumnDeclType_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlColumnDouble_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlColumnInt_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlColumnInt64_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlColumnName_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlColumnOriginName_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlColumnTableName_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlColumnText_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlColumnType_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlColumnValue_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlCommitHook_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlCompileOptionGet_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlCompileOptionUsed_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlComplete_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlDataCount_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlDbFileName_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlDbHandle_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlDbMutex_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlDbReadOnly_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlDbReleaseMemory_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlDbStatus_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlErrCode_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlErrMsg_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlExec_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlExtendedResultCodes_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlFinalize_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlGetAutocommit_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlInterrupt_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlLastInsertRowID_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlLibVersion_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlLimit_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlMemoryHighWater_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlMemoryUsed_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlMutexAlloc_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlMutexEnter_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlMutexFree_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlMutexLeave_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlMutexTry_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlNextStmt_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlOpen_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlPrepare_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlProfile_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlProgressHandler_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlReleaseMemory_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlReset_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlRollbackHook_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlSetAuthorizer_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlSoftHeapLimit64_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlSql_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlStatus_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlStmtBusy_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlStmtBusy_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlStmtReadonly_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlTableColumnMetadata_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlThreadSafe_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlTrace_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlUpdateHook_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlVersion_rtn);

REXX_TYPED_ROUTINE_PROTOTYPE(oosqlTest_rtn);

RexxRoutineEntry ooSQLite_functions[] =
{
    REXX_TYPED_ROUTINE(ooSQLiteVersion_rtn,           ooSQLiteVersion_rtn),
    REXX_TYPED_ROUTINE(ooSQLiteMerge_rtn,             ooSQLiteMerge_rtn),

    REXX_TYPED_ROUTINE(oosqlBindBlob_rtn,             oosqlBindBlob_rtn),
    REXX_TYPED_ROUTINE(oosqlBindDouble_rtn,           oosqlBindDouble_rtn),
    REXX_TYPED_ROUTINE(oosqlBindInt_rtn,              oosqlBindInt_rtn),
    REXX_TYPED_ROUTINE(oosqlBindInt64_rtn,            oosqlBindInt64_rtn),
    REXX_TYPED_ROUTINE(oosqlBindNull_rtn,             oosqlBindNull_rtn),
    REXX_TYPED_ROUTINE(oosqlBindParameterCount_rtn,   oosqlBindParameterCount_rtn),
    REXX_TYPED_ROUTINE(oosqlBindParameterIndex_rtn,   oosqlBindParameterIndex_rtn),
    REXX_TYPED_ROUTINE(oosqlBindParameterName_rtn,    oosqlBindParameterName_rtn),
    REXX_TYPED_ROUTINE(oosqlBindText_rtn,             oosqlBindText_rtn),
    REXX_TYPED_ROUTINE(oosqlBindValue_rtn,            oosqlBindValue_rtn),
    REXX_TYPED_ROUTINE(oosqlBindZeroBlob_rtn,         oosqlBindZeroBlob_rtn),
    REXX_TYPED_ROUTINE(oosqlBusyHandler_rtn,          oosqlBusyHandler_rtn),
    REXX_TYPED_ROUTINE(oosqlBusyTimeOut_rtn,          oosqlBusyTimeOut_rtn),
    REXX_TYPED_ROUTINE(oosqlChanges_rtn,              oosqlChanges_rtn),
    REXX_TYPED_ROUTINE(oosqlClearBindings_rtn,        oosqlClearBindings_rtn),
    REXX_TYPED_ROUTINE(oosqlClose_rtn,                oosqlClose_rtn),
    REXX_TYPED_ROUTINE(oosqlColumnBlob_rtn,           oosqlColumnBlob_rtn),
    REXX_TYPED_ROUTINE(oosqlColumnBytes_rtn,          oosqlColumnBytes_rtn),
    REXX_TYPED_ROUTINE(oosqlColumnCount_rtn,          oosqlColumnCount_rtn),
    REXX_TYPED_ROUTINE(oosqlColumnDatabaseName_rtn,   oosqlColumnDatabaseName_rtn),
    REXX_TYPED_ROUTINE(oosqlColumnDeclType_rtn,       oosqlColumnDeclType_rtn),
    REXX_TYPED_ROUTINE(oosqlColumnDouble_rtn,         oosqlColumnDouble_rtn),
    REXX_TYPED_ROUTINE(oosqlColumnInt_rtn,            oosqlColumnInt_rtn),
    REXX_TYPED_ROUTINE(oosqlColumnInt64_rtn,          oosqlColumnInt64_rtn),
    REXX_TYPED_ROUTINE(oosqlColumnName_rtn,           oosqlColumnName_rtn),
    REXX_TYPED_ROUTINE(oosqlColumnOriginName_rtn,     oosqlColumnOriginName_rtn),
    REXX_TYPED_ROUTINE(oosqlColumnTableName_rtn,      oosqlColumnTableName_rtn),
    REXX_TYPED_ROUTINE(oosqlColumnText_rtn,           oosqlColumnText_rtn),
    REXX_TYPED_ROUTINE(oosqlColumnType_rtn,           oosqlColumnType_rtn),
    REXX_TYPED_ROUTINE(oosqlColumnValue_rtn,          oosqlColumnValue_rtn),
    REXX_TYPED_ROUTINE(oosqlCommitHook_rtn,           oosqlCommitHook_rtn),
    REXX_TYPED_ROUTINE(oosqlCompileOptionGet_rtn,     oosqlCompileOptionGet_rtn),
    REXX_TYPED_ROUTINE(oosqlCompileOptionUsed_rtn,    oosqlCompileOptionUsed_rtn),
    REXX_TYPED_ROUTINE(oosqlComplete_rtn,             oosqlComplete_rtn),
    REXX_TYPED_ROUTINE(oosqlDataCount_rtn,            oosqlDataCount_rtn),
    REXX_TYPED_ROUTINE(oosqlDbFileName_rtn,           oosqlDbFileName_rtn),
    REXX_TYPED_ROUTINE(oosqlDbHandle_rtn,             oosqlDbHandle_rtn),
    REXX_TYPED_ROUTINE(oosqlDbMutex_rtn,           	  oosqlDbMutex_rtn),
    REXX_TYPED_ROUTINE(oosqlDbReadOnly_rtn,           oosqlDbReadOnly_rtn),
    REXX_TYPED_ROUTINE(oosqlDbReleaseMemory_rtn,      oosqlDbReleaseMemory_rtn),
    REXX_TYPED_ROUTINE(oosqlDbStatus_rtn,             oosqlDbStatus_rtn),
    REXX_TYPED_ROUTINE(oosqlErrCode_rtn,              oosqlErrCode_rtn),
    REXX_TYPED_ROUTINE(oosqlErrMsg_rtn,               oosqlErrMsg_rtn),
    REXX_TYPED_ROUTINE(oosqlExec_rtn,                 oosqlExec_rtn),
    REXX_TYPED_ROUTINE(oosqlExtendedResultCodes_rtn,  oosqlExtendedResultCodes_rtn),
    REXX_TYPED_ROUTINE(oosqlFinalize_rtn,             oosqlFinalize_rtn),
    REXX_TYPED_ROUTINE(oosqlGetAutocommit_rtn,        oosqlGetAutocommit_rtn),
    REXX_TYPED_ROUTINE(oosqlInterrupt_rtn,            oosqlInterrupt_rtn),
    REXX_TYPED_ROUTINE(oosqlLastInsertRowID_rtn,      oosqlLastInsertRowID_rtn),
    REXX_TYPED_ROUTINE(oosqlLibVersion_rtn,           oosqlLibVersion_rtn),
    REXX_TYPED_ROUTINE(oosqlLimit_rtn,                oosqlLimit_rtn),
    REXX_TYPED_ROUTINE(oosqlMemoryHighWater_rtn,      oosqlMemoryHighWater_rtn),
    REXX_TYPED_ROUTINE(oosqlMemoryUsed_rtn,           oosqlMemoryUsed_rtn),
    REXX_TYPED_ROUTINE(oosqlMutexAlloc_rtn,           oosqlMutexAlloc_rtn),
    REXX_TYPED_ROUTINE(oosqlMutexEnter_rtn,           oosqlMutexEnter_rtn),
    REXX_TYPED_ROUTINE(oosqlMutexFree_rtn,            oosqlMutexFree_rtn),
    REXX_TYPED_ROUTINE(oosqlMutexLeave_rtn,           oosqlMutexLeave_rtn),
    REXX_TYPED_ROUTINE(oosqlMutexTry_rtn,             oosqlMutexTry_rtn),
    REXX_TYPED_ROUTINE(oosqlNextStmt_rtn,             oosqlNextStmt_rtn),
    REXX_TYPED_ROUTINE(oosqlOpen_rtn,                 oosqlOpen_rtn),
    REXX_TYPED_ROUTINE(oosqlPrepare_rtn,              oosqlPrepare_rtn),
    REXX_TYPED_ROUTINE(oosqlProfile_rtn,              oosqlProfile_rtn),
    REXX_TYPED_ROUTINE(oosqlProgressHandler_rtn,      oosqlProgressHandler_rtn),
    REXX_TYPED_ROUTINE(oosqlReleaseMemory_rtn,        oosqlReleaseMemory_rtn),
    REXX_TYPED_ROUTINE(oosqlReset_rtn,                oosqlReset_rtn),
    REXX_TYPED_ROUTINE(oosqlRollbackHook_rtn,         oosqlRollbackHook_rtn),
    REXX_TYPED_ROUTINE(oosqlSetAuthorizer_rtn,        oosqlSetAuthorizer_rtn),
    REXX_TYPED_ROUTINE(oosqlSoftHeapLimit64_rtn,      oosqlSoftHeapLimit64_rtn),
    REXX_TYPED_ROUTINE(oosqlSql_rtn,                  oosqlSql_rtn),
    REXX_TYPED_ROUTINE(oosqlStatus_rtn,               oosqlStatus_rtn),
    REXX_TYPED_ROUTINE(oosqlStep_rtn,                 oosqlStep_rtn),
    REXX_TYPED_ROUTINE(oosqlStmtBusy_rtn,             oosqlStmtBusy_rtn),
    REXX_TYPED_ROUTINE(oosqlStmtReadonly_rtn,         oosqlStmtReadonly_rtn),
    REXX_TYPED_ROUTINE(oosqlStmtStatus_rtn,           oosqlStmtStatus_rtn),
    REXX_TYPED_ROUTINE(oosqlTableColumnMetadata_rtn,  oosqlTableColumnMetadata_rtn),
    REXX_TYPED_ROUTINE(oosqlThreadSafe_rtn,           oosqlThreadSafe_rtn),
    REXX_TYPED_ROUTINE(oosqlTotalChanges_rtn,         oosqlTotalChanges_rtn),
    REXX_TYPED_ROUTINE(oosqlTrace_rtn,                oosqlTrace_rtn),
    REXX_TYPED_ROUTINE(oosqlUpdateHook_rtn,           oosqlUpdateHook_rtn),
    REXX_TYPED_ROUTINE(oosqlVersion_rtn,              oosqlVersion_rtn),

    REXX_TYPED_ROUTINE(oosqlTest_rtn,                 oosqlTest_rtn),

    REXX_LAST_ROUTINE()
};



// .ooSQL
REXX_METHOD_PROTOTYPE(oosqlC_merge_cls);

// .ooSQLite
REXX_METHOD_PROTOTYPE(oosql_compileOptionGet_cls);
REXX_METHOD_PROTOTYPE(oosql_compileOptionUsed_cls);
REXX_METHOD_PROTOTYPE(oosql_complete_cls);
REXX_METHOD_PROTOTYPE(oosql_libVersion_cls);
REXX_METHOD_PROTOTYPE(oosql_libVersionNumber_cls);
REXX_METHOD_PROTOTYPE(oosql_memoryHighWater_cls);
REXX_METHOD_PROTOTYPE(oosql_memoryUsed_cls);
REXX_METHOD_PROTOTYPE(oosql_releaseMemory_cls);
REXX_METHOD_PROTOTYPE(oosql_softHeapLimit64_cls);
REXX_METHOD_PROTOTYPE(oosql_sourceID_cls);
REXX_METHOD_PROTOTYPE(oosql_sqlite3Version_cls);
REXX_METHOD_PROTOTYPE(oosql_status_cls);
REXX_METHOD_PROTOTYPE(oosql_threadSafe_cls);
REXX_METHOD_PROTOTYPE(oosql_version_cls);

REXX_METHOD_PROTOTYPE(oosql_test_cls);

// .ooSQLiteDB
REXX_METHOD_PROTOTYPE(oosqldb_init_cls);

REXX_METHOD_PROTOTYPE(oosqldb_getClosed_atr);
REXX_METHOD_PROTOTYPE(oosqldb_getFileName_atr);
REXX_METHOD_PROTOTYPE(oosqldb_getInitCode_atr);
REXX_METHOD_PROTOTYPE(oosqldb_getLastErrCode_atr);
REXX_METHOD_PROTOTYPE(oosqldb_getLastErrMsg_atr);

REXX_METHOD_PROTOTYPE(oosqldb_init);
REXX_METHOD_PROTOTYPE(oosqldb_uninit);

REXX_METHOD_PROTOTYPE(oosqldb_busyHandler);
REXX_METHOD_PROTOTYPE(oosqldb_busyTimeOut);
REXX_METHOD_PROTOTYPE(oosqldb_changes);
REXX_METHOD_PROTOTYPE(oosqldb_close);
REXX_METHOD_PROTOTYPE(oosqldb_commitHook);
REXX_METHOD_PROTOTYPE(oosqldb_dbFileName);
REXX_METHOD_PROTOTYPE(oosqldb_dbMutex);
REXX_METHOD_PROTOTYPE(oosqldb_dbReadOnly);
REXX_METHOD_PROTOTYPE(oosqldb_dbReleaseMemory);
REXX_METHOD_PROTOTYPE(oosqldb_dbStatus);
REXX_METHOD_PROTOTYPE(oosqldb_errCode);
REXX_METHOD_PROTOTYPE(oosqldb_errMsg);
REXX_METHOD_PROTOTYPE(oosqldb_exec);
REXX_METHOD_PROTOTYPE(oosqldb_extendedResultCodes);
REXX_METHOD_PROTOTYPE(oosqldb_getAutoCommit);
REXX_METHOD_PROTOTYPE(oosqldb_interrupt);
REXX_METHOD_PROTOTYPE(oosqldb_lastInsertRowID);
REXX_METHOD_PROTOTYPE(oosqldb_limit);
REXX_METHOD_PROTOTYPE(oosqldb_nextStmt);
REXX_METHOD_PROTOTYPE(oosqldb_pragma);
REXX_METHOD_PROTOTYPE(oosqldb_profile);
REXX_METHOD_PROTOTYPE(oosqldb_progressHandler);
REXX_METHOD_PROTOTYPE(oosqldb_rollbackHook);
REXX_METHOD_PROTOTYPE(oosqldb_setAuthorizer);
REXX_METHOD_PROTOTYPE(oosqldb_tableColumnMetadata);
REXX_METHOD_PROTOTYPE(oosqldb_totalChanges);
REXX_METHOD_PROTOTYPE(oosqldb_trace);
REXX_METHOD_PROTOTYPE(oosqldb_updateHook);

REXX_METHOD_PROTOTYPE(oosqldb_putStmt);
REXX_METHOD_PROTOTYPE(oosqldb_delStmt);

// .ooSQLiteStmt
REXX_METHOD_PROTOTYPE(oosqlstmt_init_cls);

REXX_METHOD_PROTOTYPE(oosqlstmt_getErrMsg_atr);
REXX_METHOD_PROTOTYPE(oosqlstmt_getErrMsg_atr);
REXX_METHOD_PROTOTYPE(oosqlstmt_getInitCode_atr);
REXX_METHOD_PROTOTYPE(oosqlstmt_getFinalize_atr);

REXX_METHOD_PROTOTYPE(oosqlstmt_init);
REXX_METHOD_PROTOTYPE(oosqlstmt_uninit);

REXX_METHOD_PROTOTYPE(oosqlstmt_bindBlob);
REXX_METHOD_PROTOTYPE(oosqlstmt_bindDouble);
REXX_METHOD_PROTOTYPE(oosqlstmt_bindInt);
REXX_METHOD_PROTOTYPE(oosqlstmt_bindInt64);
REXX_METHOD_PROTOTYPE(oosqlstmt_bindNull);
REXX_METHOD_PROTOTYPE(oosqlstmt_bindParameterCount);
REXX_METHOD_PROTOTYPE(oosqlstmt_bindParameterIndex);
REXX_METHOD_PROTOTYPE(oosqlstmt_bindParameterName);
REXX_METHOD_PROTOTYPE(oosqlstmt_bindText);
REXX_METHOD_PROTOTYPE(oosqlstmt_bindValue);
REXX_METHOD_PROTOTYPE(oosqlstmt_bindZeroBlob);
REXX_METHOD_PROTOTYPE(oosqlstmt_clearBindings);
REXX_METHOD_PROTOTYPE(oosqlstmt_columnBlob);
REXX_METHOD_PROTOTYPE(oosqlstmt_columnCount);
REXX_METHOD_PROTOTYPE(oosqlstmt_columnDatabaseName);
REXX_METHOD_PROTOTYPE(oosqlstmt_columnDeclType);
REXX_METHOD_PROTOTYPE(oosqlstmt_columnDouble);
REXX_METHOD_PROTOTYPE(oosqlstmt_columnInt);
REXX_METHOD_PROTOTYPE(oosqlstmt_columnInt64);
REXX_METHOD_PROTOTYPE(oosqlstmt_columnName);
REXX_METHOD_PROTOTYPE(oosqlstmt_columnOriginName);
REXX_METHOD_PROTOTYPE(oosqlstmt_columnTableName);
REXX_METHOD_PROTOTYPE(oosqlstmt_columnText);
REXX_METHOD_PROTOTYPE(oosqlstmt_columnType);
REXX_METHOD_PROTOTYPE(oosqlstmt_columnValue);
REXX_METHOD_PROTOTYPE(oosqlstmt_dataCount);
REXX_METHOD_PROTOTYPE(oosqlstmt_dbHandle);
REXX_METHOD_PROTOTYPE(oosqlstmt_finalize);
REXX_METHOD_PROTOTYPE(oosqlstmt_reset);
REXX_METHOD_PROTOTYPE(oosqlstmt_sql);
REXX_METHOD_PROTOTYPE(oosqlstmt_step);
REXX_METHOD_PROTOTYPE(oosqlstmt_stmtBusy);
REXX_METHOD_PROTOTYPE(oosqlstmt_stmtReadonly);
REXX_METHOD_PROTOTYPE(oosqlstmt_stmtStatus);
REXX_METHOD_PROTOTYPE(oosqlstmt_value);

// .ooSQLiteMutex
REXX_METHOD_PROTOTYPE(oosqlmtx_getClosed_atr);
REXX_METHOD_PROTOTYPE(oosqlmtx_getIsNull_atr);

REXX_METHOD_PROTOTYPE(oosqlmtx_init);
REXX_METHOD_PROTOTYPE(oosqlmtx_uninit);

REXX_METHOD_PROTOTYPE(oosqlmtx_enter);
REXX_METHOD_PROTOTYPE(oosqlmtx_free);
REXX_METHOD_PROTOTYPE(oosqlmtx_leave);
REXX_METHOD_PROTOTYPE(oosqlmtx_try);

// __rtn_helper_class
REXX_METHOD_PROTOTYPE(hlpr_init_cls);

// __rtn_db_callback_class
REXX_METHOD_PROTOTYPE(db_cb_releaseBuffer);



RexxMethodEntry ooSQLite_methods[] = {
    // .ooSQL
    REXX_METHOD(oosqlC_merge_cls,      oosqlC_merge_cls),

    // .ooSQLite
    REXX_METHOD(oosql_compileOptionGet_cls,    oosql_compileOptionGet_cls),
    REXX_METHOD(oosql_compileOptionUsed_cls,   oosql_compileOptionUsed_cls),
    REXX_METHOD(oosql_complete_cls,            oosql_complete_cls),
    REXX_METHOD(oosql_libVersion_cls,          oosql_libVersion_cls),
    REXX_METHOD(oosql_libVersionNumber_cls,    oosql_libVersionNumber_cls),
    REXX_METHOD(oosql_memoryUsed_cls,          oosql_memoryUsed_cls),
    REXX_METHOD(oosql_memoryHighWater_cls,     oosql_memoryHighWater_cls),
    REXX_METHOD(oosql_releaseMemory_cls,       oosql_releaseMemory_cls),
    REXX_METHOD(oosql_softHeapLimit64_cls,     oosql_softHeapLimit64_cls),
    REXX_METHOD(oosql_sourceID_cls,            oosql_sourceID_cls),
    REXX_METHOD(oosql_sqlite3Version_cls,      oosql_sqlite3Version_cls),
    REXX_METHOD(oosql_status_cls,              oosql_status_cls),
    REXX_METHOD(oosql_threadSafe_cls,          oosql_threadSafe_cls),
    REXX_METHOD(oosql_version_cls,             oosql_version_cls),

    REXX_METHOD(oosql_test_cls,                oosql_test_cls),

    // .ooSQLiteDB
    REXX_METHOD(oosqldb_init_cls,              oosqldb_init_cls),

    REXX_METHOD(oosqldb_getClosed_atr,         oosqldb_getClosed_atr),
    REXX_METHOD(oosqldb_getFileName_atr,       oosqldb_getFileName_atr),
    REXX_METHOD(oosqldb_getInitCode_atr,       oosqldb_getInitCode_atr),
    REXX_METHOD(oosqldb_getLastErrCode_atr,    oosqldb_getLastErrCode_atr),
    REXX_METHOD(oosqldb_getLastErrMsg_atr,     oosqldb_getLastErrMsg_atr),

    REXX_METHOD(oosqldb_init,                  oosqldb_init),
    REXX_METHOD(oosqldb_uninit,                oosqldb_uninit),

    REXX_METHOD(oosqldb_busyHandler,           oosqldb_busyHandler),
    REXX_METHOD(oosqldb_busyTimeOut,           oosqldb_busyTimeOut),
    REXX_METHOD(oosqldb_changes,               oosqldb_changes),
    REXX_METHOD(oosqldb_close,                 oosqldb_close),
    REXX_METHOD(oosqldb_commitHook,            oosqldb_commitHook),
    REXX_METHOD(oosqldb_dbFileName,            oosqldb_dbFileName),
    REXX_METHOD(oosqldb_dbMutex,               oosqldb_dbMutex),
    REXX_METHOD(oosqldb_dbReadOnly,            oosqldb_dbReadOnly),
    REXX_METHOD(oosqldb_dbReleaseMemory,       oosqldb_dbReleaseMemory),
    REXX_METHOD(oosqldb_dbStatus,              oosqldb_dbStatus),
    REXX_METHOD(oosqldb_errCode,               oosqldb_errCode),
    REXX_METHOD(oosqldb_errMsg,                oosqldb_errMsg),
    REXX_METHOD(oosqldb_exec,                  oosqldb_exec),
    REXX_METHOD(oosqldb_extendedResultCodes,   oosqldb_extendedResultCodes),
    REXX_METHOD(oosqldb_getAutoCommit,         oosqldb_getAutoCommit),
    REXX_METHOD(oosqldb_interrupt,             oosqldb_interrupt),
    REXX_METHOD(oosqldb_lastInsertRowID,       oosqldb_lastInsertRowID),
    REXX_METHOD(oosqldb_limit,                 oosqldb_limit),
    REXX_METHOD(oosqldb_nextStmt,              oosqldb_nextStmt),
    REXX_METHOD(oosqldb_pragma,                oosqldb_pragma),
    REXX_METHOD(oosqldb_profile,               oosqldb_profile),
    REXX_METHOD(oosqldb_progressHandler,       oosqldb_progressHandler),
    REXX_METHOD(oosqldb_rollbackHook,          oosqldb_rollbackHook),
    REXX_METHOD(oosqldb_setAuthorizer,         oosqldb_setAuthorizer),
    REXX_METHOD(oosqldb_tableColumnMetadata,   oosqldb_tableColumnMetadata),
    REXX_METHOD(oosqldb_totalChanges,          oosqldb_totalChanges),
    REXX_METHOD(oosqldb_trace,                 oosqldb_trace),
    REXX_METHOD(oosqldb_updateHook,            oosqldb_updateHook),

    REXX_METHOD(oosqldb_putStmt,               oosqldb_putStmt),
    REXX_METHOD(oosqldb_delStmt,               oosqldb_delStmt),

    // .ooSQLiteStmt
    REXX_METHOD(oosqlstmt_init_cls,            oosqlstmt_init_cls),

    REXX_METHOD(oosqlstmt_getErrMsg_atr,       oosqlstmt_getErrMsg_atr),
    REXX_METHOD(oosqlstmt_getInitCode_atr,     oosqlstmt_getInitCode_atr),
    REXX_METHOD(oosqlstmt_getFinalized_atr,    oosqlstmt_getFinalized_atr),

    REXX_METHOD(oosqlstmt_init,                oosqlstmt_init),
    REXX_METHOD(oosqlstmt_uninit,              oosqlstmt_uninit),

    REXX_METHOD(oosqlstmt_bindBlob,            oosqlstmt_bindBlob),
    REXX_METHOD(oosqlstmt_bindDouble,          oosqlstmt_bindDouble),
    REXX_METHOD(oosqlstmt_bindInt,             oosqlstmt_bindInt),
    REXX_METHOD(oosqlstmt_bindInt64,           oosqlstmt_bindInt64),
    REXX_METHOD(oosqlstmt_bindNull,            oosqlstmt_bindNull),
    REXX_METHOD(oosqlstmt_bindParameterCount,  oosqlstmt_bindParameterCount),
    REXX_METHOD(oosqlstmt_bindParameterIndex,  oosqlstmt_bindParameterIndex),
    REXX_METHOD(oosqlstmt_bindParameterName,   oosqlstmt_bindParameterName),
    REXX_METHOD(oosqlstmt_bindText,            oosqlstmt_bindText),
    REXX_METHOD(oosqlstmt_bindValue,           oosqlstmt_bindValue),
    REXX_METHOD(oosqlstmt_bindZeroBlob,        oosqlstmt_bindZeroBlob),
    REXX_METHOD(oosqlstmt_clearBindings,       oosqlstmt_clearBindings),
    REXX_METHOD(oosqlstmt_columnBlob,          oosqlstmt_columnBlob),
    REXX_METHOD(oosqlstmt_columnBytes,         oosqlstmt_columnBytes),
    REXX_METHOD(oosqlstmt_columnCount,         oosqlstmt_columnCount),
    REXX_METHOD(oosqlstmt_columnDatabaseName,  oosqlstmt_columnDatabaseName),
    REXX_METHOD(oosqlstmt_columnDeclType,      oosqlstmt_columnDeclType),
    REXX_METHOD(oosqlstmt_columnDouble,        oosqlstmt_columnDouble),
    REXX_METHOD(oosqlstmt_columnInt,           oosqlstmt_columnInt),
    REXX_METHOD(oosqlstmt_columnInt64,         oosqlstmt_columnInt64),
    REXX_METHOD(oosqlstmt_columnName,          oosqlstmt_columnName),
    REXX_METHOD(oosqlstmt_columnOriginName,    oosqlstmt_columnOriginName),
    REXX_METHOD(oosqlstmt_columnTableName,     oosqlstmt_columnTableName),
    REXX_METHOD(oosqlstmt_columnText,          oosqlstmt_columnText),
    REXX_METHOD(oosqlstmt_columnType,          oosqlstmt_columnType),
    REXX_METHOD(oosqlstmt_columnValue,         oosqlstmt_columnValue),
    REXX_METHOD(oosqlstmt_dataCount,           oosqlstmt_dataCount),
    REXX_METHOD(oosqlstmt_dbHandle,            oosqlstmt_dbHandle),
    REXX_METHOD(oosqlstmt_finalize,            oosqlstmt_finalize),
    REXX_METHOD(oosqlstmt_reset,               oosqlstmt_reset),
    REXX_METHOD(oosqlstmt_sql,                 oosqlstmt_sql),
    REXX_METHOD(oosqlstmt_step,                oosqlstmt_step),
    REXX_METHOD(oosqlstmt_stmtBusy,            oosqlstmt_stmtBusy),
    REXX_METHOD(oosqlstmt_stmtReadonly,        oosqlstmt_stmtReadonly),
    REXX_METHOD(oosqlstmt_stmtStatus,          oosqlstmt_stmtStatus),
    REXX_METHOD(oosqlstmt_value,               oosqlstmt_value),

    // .ooSQLiteMutex
    REXX_METHOD(oosqlmtx_getClosed_atr,        oosqlmtx_getClosed_atr),
    REXX_METHOD(oosqlmtx_getIsNull_atr,        oosqlmtx_getIsNull_atr),

    REXX_METHOD(oosqlmtx_init,                 oosqlmtx_init),
    REXX_METHOD(oosqlmtx_uninit,               oosqlmtx_uninit),

    REXX_METHOD(oosqlmtx_enter,                oosqlmtx_enter),
    REXX_METHOD(oosqlmtx_free,                 oosqlmtx_free),
    REXX_METHOD(oosqlmtx_leave,                oosqlmtx_leave),
    REXX_METHOD(oosqlmtx_try,                  oosqlmtx_try),

    // __rtn_helper_class
    REXX_METHOD(hlpr_init_cls,                hlpr_init_cls),

    // __rtn_db_callback_class
    REXX_METHOD(db_cb_releaseBuffer,          db_cb_releaseBuffer),

    REXX_LAST_METHOD()
};

RexxPackageEntry ooSQLite_package_entry =
{
    STANDARD_PACKAGE_HEADER
    REXX_INTERPRETER_4_1_0,              // needs at least the 4.1.0 interpreter
    "ooSQLite",                          // name of the package
    "1.0.0",                             // package information
    ooSQLiteLoad,                        // package load function
    ooSQLiteUnload,                      // package unload function
    ooSQLite_functions,                  // the exported functions
    ooSQLite_methods                     // the exported methods
};

// The package loading stub.  On Windows, the compiler optimizations for a
// release build remove the stub.  So, we use DllExport to generate an export
// directive in the object file for RexxGetPackage() to prevent that.
// (DllExport is an empty macro on unixes.)
BEGIN_EXTERN_C()
    DllExport RexxPackageEntry *RexxEntry RexxGetPackage(void) { return &ooSQLite_package_entry; }
END_EXTERN_C()
