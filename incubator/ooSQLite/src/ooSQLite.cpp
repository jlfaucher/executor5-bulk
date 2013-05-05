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
 * ooSQLite.cpp
 *
 * ...
 */

#include "sqlite3.h"
#include "ooSQLite.hpp"

#include <stdio.h>
#include <string.h>

#include "APICommon.hpp"
#include "SysLibrary.hpp"

#include <new>
using namespace std;


// Initialized in ooSQLiteLoad().
RexxObjectPtr       TheTrueObj        = NULLOBJECT;
RexxObjectPtr       TheFalseObj       = NULLOBJECT;
RexxObjectPtr       TheNilObj         = NULLOBJECT;
RexxObjectPtr       TheZeroObj        = NULLOBJECT;
RexxObjectPtr       TheOneObj         = NULLOBJECT;
RexxObjectPtr       TheTwoObj         = NULLOBJECT;
RexxObjectPtr       TheNegativeOneObj = NULLOBJECT;

// Initialized in the class init methods.
RexxClassObject     TheOOSQLiteClass            = NULLOBJECT;
RexxClassObject     TheOOSQLiteConnectionClass  = NULLOBJECT;
RexxClassObject     TheOOSQLiteStmtClass        = NULLOBJECT;
RexxClassObject     TheOOSQLiteMutexClass       = NULLOBJECT;

// The RoutineHelper class object is initialized in the class init method.  The
// hidden helper object is only initialized when / if needed.
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
 * @param c  Thread context pointer passed from the interpreter when this package
 *           is loaded.
 *
 * @return Nothing is returned
 */
void RexxEntry ooSQLiteLoad(RexxThreadContext *c)
{
    int rc = sqlite3_initialize();

#if OOSQLDBG == 1
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
#if OOSQLDBG == 1
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
static void sqliteErrorException(RexxThreadContext *c, sqlite3 *db, const char *function)
{
    char buffer[256];
    snprintf(buffer, sizeof(buffer), "SQLite API error:  API(%s) rc(%d) msg(%s)",
             function, sqlite3_errcode(db), sqlite3_errmsg(db) ? sqlite3_errmsg(db) : "none");
    systemServiceException(c, buffer);
}

/**
 * If the value in the database for the requested field is NULL, the
 * sqlite3_column_text() returns a null pointer
 *
 * We can't pass a null pointer ito the native API, so if null, we use the
 * passed in nulObj instead.  'nullObj' allows the user to set the
 * representation they want for SQL NULL.  The default is .nil.
 *
 * @param stmt
 * @param col
 * @param nullObj
 *
 * @return a Rexx object representing the value of the column text, which could
 *         be SQL NULL.
 */
static inline RexxObjectPtr safeColumnText(RexxThreadContext *c, sqlite3_stmt *stmt, int col, RexxObjectPtr nullObj)
{
    CSTRING data = (CSTRING)sqlite3_column_text(stmt, col);
    return (data == NULL ? nullObj : c->String(data));
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
 *
 * @note  This function is used for values returned from SQLite, *other*, than
 *        database column values.  Many of the SQLite functions that return
 *        strings can return null, as can the sqlite3_column_text() function.
 *        Use this function for those other function.  Use the safeColumnText()
 *        function for sqlite3_column_text().
 */
static inline RexxStringObject safeRexxString(RexxThreadContext *c, CSTRING str)
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
static inline RexxStringObject safeRexxStringRx(RexxThreadContext *c, CSTRING str)
{
    return (str == NULL ? c->NullString() : c->String(str));
}

/**
 * Returns the sqlite3_errmsg string from the specified database as a Rexx
 * string.
 *
 * @param c   Thread context we are operating in.
 * @param db  sqlite3 database connection pointer.
 *
 * @return The err msg as a Rexx String object.
 *
 * @remarks  Originally I thought it was possible for sqlite3_errmsg() to return
 *           null.  But it looks like that can not happen.  So the check for
 *           null is dropped.
 */
static inline RexxStringObject dbErrStringRx(RexxThreadContext *c, sqlite3 *db)
{
    return c->String(sqlite3_errmsg(db));
}
static inline RexxStringObject dbErrStringRx(RexxMethodContext *c, sqlite3 *db)
{
    return dbErrStringRx(c->threadContext, db);
}

/**
 * Returns the sqlite3_errcode from the specified database.
 *
 * @param db  sqlite3 database connection pointer.
 *
 * @return The err code.
 *
 * @remarks The only real purpose of this inline is to provide a matching
 *          function to dbErrStringRx(), making it easier to remember.
 */
static inline int dbErrCode(sqlite3 *db)
{
    return sqlite3_errcode(db);
}

/**
 * Checks if the Rexx object is the number SQLITE_OK, which is 0.
 *
 * @param c
 * @param _rc
 *
 * @return True if _rc is the nubmer 0, otherwise false.
 */
static inline bool objIsSqliteOK(RexxThreadContext *c, RexxObjectPtr _rc)
{
    int rc = SQLITE_MISUSE;
    if ( c->Int32(_rc, &rc) )
    {
        return (rc == SQLITE_OK) ? true : false;
    }
    return false;
}
static inline bool objIsSqliteOK(RexxMethodContext *c, RexxObjectPtr _rc)
{
    return objIsSqliteOK(c->threadContext, _rc);
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
 * Same as above but includes an error message along with the error code.
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
 * Similar to the other ooSQLiteErr() functions above, but the primary purpose
 * is to set the database connection last error message and last error code.
 *
 * Note that this is the ooSQLiteConnection last error *attributes* which are
 * separate from the SQLite last error functions.  The attributes can be set to
 * the same values as the SQLite APIs produce, but need not be.
 *
 * @param c            Method context we are operating in.
 * @param pConn        Pointer to the CSelf for an ooSQLiteConnection object.
 * @param rc           SQLITE error code.
 * @param isSQLiteErr  True if this is an error code returned from the SQLite
 *                     API, false if it is our own ooRexx extension error.
 *
 * @return  For convenience the lastErrMsg attribute value is returned.
 */
static RexxStringObject ooSQLiteErr(RexxMethodContext *c, pCooSQLiteConn pConn, int rc, CSTRING msg, bool isSQLiteErr)
{
    pConn->lastErrMsg  = ooSQLiteErr(c->threadContext, rc, msg, isSQLiteErr);
    pConn->lastErrCode = rc;

    c->SetObjectVariable("rxLastErrMsg", pConn->lastErrMsg);

    return pConn->lastErrMsg;
}

/**
 * The reverse of the ooSQLiteErr() function above.  This is used to set the
 * last error to no error.
 *
 * @param c            Method context we are operating in.
 * @param pConn        Pointer to the CSelf for an ooSQLiteConnection object.
 * @return  For convenience the lastErrMsg attribute value is returned.
 */
static RexxStringObject ooSQLiteNoErr(RexxMethodContext *c, pCooSQLiteConn pConn)
{
    pConn->lastErrMsg  = c->String("No error");
    pConn->lastErrCode = SQLITE_OK;

    c->SetObjectVariable("rxLastErrMsg", pConn->lastErrMsg);

    return pConn->lastErrMsg;
}

/**
 * Returns an upper-cased copy of the string.
 *
 * @param str   The string to copy and upper case.
 *
 * @return      A pointer to a new string, or null on a memory allocation
 *              failure.
 *
 * The caller is responsible for freeing the returned string.  Memory is
 * malloc'd using the SQLite malloc so the the SQLite free must be used to
 * release the memory.
 */
static char *strdupupr(const char *str)
{
    char *retStr = NULL;
    if ( str )
    {
        size_t l = strlen(str);
        l++;

        retStr = (char *) sqlite3_malloc((int)l);
        if ( retStr )
        {
            char *p;
            for ( p = retStr; *str; ++str )
            {
                if ( ('a' <= *str) && (*str <= 'z') )
                {
                    *p++ = *str - ('a' - 'A');
                }
                else
                {
                    *p++ = *str;
                }
            }
            *p = '\0';
        }
    }
    return retStr;
}

/**
 * Uppercase a memory location.  Taken from rexxutil.cpp.
 *
 * @param location  Memory location
 * @param length    Length to uppercase.
 */
static void  strupper(char *location, size_t length)
{
    // Loop through entire string:
  for (; length--; location++)
  {
      // Uppercase in place.
      *location = toupper(*location);
  }
}

/**
 * Convert any Rexx object to its string value. If the specified object is the
 * NULLOBJECT or the .nil object then a null pointer is returned.
 *
 * @param c
 * @param o
 *
 * @return CSTRING
 */
static inline CSTRING obj2str(RexxThreadContext *c, RexxObjectPtr o)
{
    return ( o == NULLOBJECT || o == TheNilObj ) ? NULL : c->ObjectToStringValue(o);
}

/**
 * Convenience function to return sqlite3_mprintf() result as a Rexx string.
 *
 * sqlite3_mprintf() can return null if the database engine can not malloc
 * memory.  So, if s is null, an out of memory exception is appropriate.
 *
 * @param c
 * @param s
 *
 * @return RexxObjectPtr
 */
static RexxObjectPtr enquoteResult(RexxThreadContext *c, char *s)
{
    if ( s == NULL )
    {
        outOfMemoryException(c);
        return c->NullString();
    }

    RexxObjectPtr result = c->String(s);
    sqlite3_free(s);

    return result;
}

static inline RexxObjectPtr enquote1(RexxThreadContext *c, RexxArrayObject v)
{
    char *s = sqlite3_mprintf("%Q", obj2str(c, c->ArrayAt(v, 1)));
    return enquoteResult(c, s);
}

static inline RexxObjectPtr enquote2(RexxThreadContext *c, RexxArrayObject v)
{
    char *s = sqlite3_mprintf("%Q, %Q",
                              obj2str(c, c->ArrayAt(v, 1)),
                              obj2str(c, c->ArrayAt(v, 2)));

    return enquoteResult(c, s);
}

static inline RexxObjectPtr enquote3(RexxThreadContext *c, RexxArrayObject v)
{
    char *s = sqlite3_mprintf("%Q, %Q, %Q",
                              obj2str(c, c->ArrayAt(v, 1)),
                              obj2str(c, c->ArrayAt(v, 2)),
                              obj2str(c, c->ArrayAt(v, 3)));

    return enquoteResult(c, s);
}

static inline RexxObjectPtr enquote4(RexxThreadContext *c, RexxArrayObject v)
{
    char *s = sqlite3_mprintf("%Q, %Q, %Q, %Q",
                              obj2str(c, c->ArrayAt(v, 1)),
                              obj2str(c, c->ArrayAt(v, 2)),
                              obj2str(c, c->ArrayAt(v, 3)),
                              obj2str(c, c->ArrayAt(v, 4)));

    return enquoteResult(c, s);
}

static inline RexxObjectPtr enquote5(RexxThreadContext *c, RexxArrayObject v)
{
    char *s = sqlite3_mprintf("%Q, %Q, %Q, %Q, %Q",
                              obj2str(c, c->ArrayAt(v, 1)),
                              obj2str(c, c->ArrayAt(v, 2)),
                              obj2str(c, c->ArrayAt(v, 3)),
                              obj2str(c, c->ArrayAt(v, 4)),
                              obj2str(c, c->ArrayAt(v, 5)));

    return enquoteResult(c, s);
}

static inline RexxObjectPtr enquote6(RexxThreadContext *c, RexxArrayObject v)
{
    char *s = sqlite3_mprintf("%Q, %Q, %Q, %Q, %Q, %Q",
                              obj2str(c, c->ArrayAt(v, 1)),
                              obj2str(c, c->ArrayAt(v, 2)),
                              obj2str(c, c->ArrayAt(v, 3)),
                              obj2str(c, c->ArrayAt(v, 4)),
                              obj2str(c, c->ArrayAt(v, 5)),
                              obj2str(c, c->ArrayAt(v, 6)));

    return enquoteResult(c, s);
}

/**
 * Uses sqlite3_mprintf on an number of items in the supplied array.
 *
 * The returned string has an upper bound on its length, currently 4095.  A Rexx
 * string is always returned, produced from a zero terminated string.
 *
 * @author Administrator (6/23/2012)
 *
 * @param c
 * @param v
 * @param size
 *
 * @return RexxObjectPtr
 *
 * @assumes  That the caller has checked the size of the array and it is at
 *           least 7 in size.
 */
static RexxObjectPtr enquoteN(RexxThreadContext *c, RexxArrayObject v, size_t size)
{
    char *s = sqlite3_mprintf("%Q, %Q, %Q, %Q, %Q, %Q",
                              obj2str(c, c->ArrayAt(v, 1)),
                              obj2str(c, c->ArrayAt(v, 2)),
                              obj2str(c, c->ArrayAt(v, 3)),
                              obj2str(c, c->ArrayAt(v, 4)),
                              obj2str(c, c->ArrayAt(v, 5)),
                              obj2str(c, c->ArrayAt(v, 6)));

    if ( s == NULL )
    {
        outOfMemoryException(c);
        return c->NullString();
    }

    char  buf[MAX_ENQUOTE_STRING + 1];
    char *lead = s;

    for ( size_t i = 7; i <= size; i++)
    {
        lead = sqlite3_snprintf(sizeof(buf), buf, "%s, %Q", lead, obj2str(c, c->ArrayAt(v, i)));

        if ( i < size && strlen(lead) == MAX_ENQUOTE_STRING )
        {
            stringTooLongException(c, "enquote", true, MAX_ENQUOTE_STRING);
            break;
        }
    }

    sqlite3_free(s);
    return c->String(buf);
}

/**
 * Returns an array of pointer to strings consisting of the column names for the
 * specified prepared statement, upper-cased.
 *
 * @param c       Thread context we are operating in.  Used for out-of-memory
 *                errors.
 *
 * @param stmt    The prepared statement in use.
 *
 * @param _count  [IN/OUT] The column count is returned here.
 *
 * @return An array of the column names, all upper-cased on success.  Null on
 *         error.  On error an exception has been raised, with one exception see
 *         the remarks.
 *
 * @remarks  Assumes the statement is valid.  The caller is repsonisible for
 *           freeing memory in the returned array, which inlcudes freeing the
 *           strings in the array.
 *
 *           Memory is allocated using sqlite3_malloc() so the returned array
 *           must be freed with sqlite3_free().  Note that it is safe to pass a
 *           null pointer to sqlite3_free().
 *
 *           NOTE: It is always possible that the statment has SQL errors or is
 *           one that returns no data, in which case there are no columns.  We
 *           do not want to raise an exception for this, but we still return
 *           null. However, for this case we set _count to -1.  In the current
 *           code, this function is only called when preparing a result set and
 *           all cases simply return an empty result set.  So, simply not
 *           raising an exception for a column count of 0 is sufficient.
 */
static char **getHeadersUpper(RexxThreadContext *c, sqlite3_stmt *stmt, int *_count)
{
    int    count = sqlite3_column_count(stmt);
    int    i     = 0;

    if ( count < 1 )
    {
        *_count = -1;
        return NULL;
    }

    char **headers = (char **)sqlite3_malloc(count * sizeof(char *));
    if ( headers == NULL )
    {
        outOfMemoryException(c);
        return NULL;
    }

    for ( i = 0; i < count; i++ )
    {
        CSTRING index = sqlite3_column_name(stmt, i);
        if ( index == NULL )
        {
            goto err_out;
        }

        headers[i] = strdupupr(index);
        if ( headers[i] == NULL )
        {
            goto err_out;
        }
    }

    *_count = count;
    return headers;

err_out:
    outOfMemoryException(c);

    for ( i = 0; i < count; i++ )
    {
        sqlite3_free(headers[i]);
    }
    sqlite3_free(headers);
    return NULL;
}

/**
 * Convenience function to free the memory allocated during getHeadersUpper().
 * Ensures the memory is freed correctly.
 *
 * @param headers  An array of strings, presumably the array returned from
 *                 getHeadersUpper().
 *
 * @param count    The count of strings in headers.
 *
 * @note  Note that is safe to pass a null pointer to sqlite3_free().
 */
static void freeHeadersUpper(char **headers, int count)
{
    if ( headers != NULL )
    {
        for ( int i = 0; i < count; i++ )
        {
            sqlite3_free(headers[i]);
        }
        sqlite3_free(headers);
    }
}


/**
 * Returns a result set in the "array of arrays" format
 *
 * @param c        Thread context we are operating in.
 * @param stmt     A prepared sqlite3 statemnt.
 * @param nullObj  How SQL NULL should be represented.
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
static RexxArrayObject getRecordsArray(RexxThreadContext *c, sqlite3_stmt *stmt, RexxObjectPtr nullObj)
{
    RexxArrayObject a = c->NewArray(10);  // An array of records
    RexxArrayObject r;                    // A single record
    int i = 1, j = 0;

    while ( sqlite3_step(stmt) == SQLITE_ROW )
    {
        RexxObjectPtr colValue;
        CSTRING       colName;
        int           count;

        if ( i == 1 )
        {
            count = sqlite3_column_count(stmt);

            r = c->NewArray(count);
            for ( j = 0; j < count; j++)
            {
                colName = sqlite3_column_name(stmt, j);
                c->ArrayAppendString(r, colName, strlen(colName));
            }
            c->ArrayPut(a, r, i++);


            r = c->NewArray(count);
            for ( j = 0; j < count; j++)
            {
                colValue = safeColumnText(c, stmt, j, nullObj);
                c->ArrayAppend(r, colValue);
            }
        }
        else
        {
            r = c->NewArray(count);
            for ( int j = 0; j < count; j++)
            {
                colValue = safeColumnText(c, stmt, j, nullObj);
                c->ArrayAppend(r, colValue);
            }
        }
        c->ArrayPut(a, r, i++);
    }

    return a;
}


/**
 * Returns a result set in the "array of directories" format
 *
 * @param c     Thread context we are operating in.
 * @param stmt  A prepared sqlite3 statemnt.
 *
 * @return An array of the records produced by the statement.  Each record is a
 *         directory object.
 *
 * @assumes  The specified statement is valid, that the return from
 *           sqlite3_prepare_v2() has already been checked.  Also assumes that
 *           sqlite3_step() has not been called yet.
 *
 * @note  If there are no rows returned by stepping the statement, then an empty
 *        array is returned.
 */
static RexxArrayObject getRecordsDirectory(RexxThreadContext *c, sqlite3_stmt *stmt)
{
    RexxDirectoryObject record;                      // A single record
    RexxArrayObject     records = c->NewArray(100);  // An array of records
    size_t              item = 1;                    // Current record index

    int    count;
    char **headers = getHeadersUpper(c, stmt, &count);
    if ( headers == NULL )
    {
        return records;
    }

    while ( sqlite3_step(stmt) == SQLITE_ROW )
    {
        record = c->NewDirectory();

        for ( int i = 0; i < count; i++ )
        {
            CSTRING       data  = (CSTRING)sqlite3_column_text(stmt, i);
            RexxObjectPtr value = (data == NULL) ? TheNilObj : c->String(data);

            c->DirectoryPut(record, value, headers[i]);
        }

        c->ArrayPut(records, record, item++);
    }

    freeHeadersUpper(headers, count);

    return records;
}


/**
 * Returns a result set in the "stem of stems" format
 *
 * @param c     Thread context we are operating in.
 * @param stmt  A prepared sqlite3 statemnt.
 *
 * @return A stem containing the records produced by the statement.  Each record
 *         is a stem object.
 *
 * @assumes  The specified statement is valid, that the return from
 *           sqlite3_prepare_v2() has already been checked.  Also assumes that
 *           sqlite3_step() has not been called yet.
 *
 * @note  If there are no rows returned by stepping the statement, then a stem
 *        with stem.0 equal to zero is returned.
 */
static RexxStemObject getRecordsStem(RexxThreadContext *c, sqlite3_stmt *stmt)
{
    RexxStemObject record;                           // A single record.
    RexxStemObject records = c->NewStem("records");  // A stem containing records.
    c->SetStemArrayElement(records, 0, TheZeroObj);

    int    count;
    char **headers = getHeadersUpper(c, stmt, &count);
    if ( headers == NULL )
    {
        return records;
    }

    size_t item = 1;                                 // Current record index.

    while ( sqlite3_step(stmt) == SQLITE_ROW )
    {
        record = c->NewStem("record");

        for ( int i = 0; i < count; i++ )
        {
            CSTRING       data  = (CSTRING)sqlite3_column_text(stmt, i);
            RexxObjectPtr value = (data == NULL) ? TheNilObj : c->String(data);

            c->SetStemElement(record, headers[i], value);
        }

        c->SetStemArrayElement(records, 0, c->StringSize(item));  // Update number of records in the stem.
        c->SetStemArrayElement(records, item++, record);          // Add the record.
    }

    freeHeadersUpper(headers, count);
    return records;
}


/**
 * Returns a result set in the "classic stem" format
 *
 * @param c     Thread context we are operating in.
 * @param stmt  A prepared sqlite3 statemnt.
 *
 * @return A stem containing the records produced by the statement.  Each record
 *         is a stem object.
 *
 * @assumes  The specified statement is valid, that the return from
 *           sqlite3_prepare_v2() has already been checked.  Also assumes that
 *           sqlite3_step() has not been called yet.
 *
 * @note  If there are no rows returned by stepping the statement, then a stem
 *        with stem.0 equal to zero is returned.
 */
static RexxStemObject getRecordsClassicStem(RexxThreadContext *c, sqlite3_stmt *stmt)
{
    RexxStemObject records = c->NewStem("records");  // A stem containing records.
    c->SetStemArrayElement(records, 0, TheZeroObj);

    int    count;
    char **headers = getHeadersUpper(c, stmt, &count);
    if ( headers == NULL )
    {
        return records;
    }

    size_t item = 1;                                 // Current record index.

    while ( sqlite3_step(stmt) == SQLITE_ROW )
    {
        char buf[1024] = {0};

        for ( int i = 0; i < count; i++ )
        {
            snprintf(buf, sizeof(buf) - 1, "%d.%s", i + 1, headers[i]);

            CSTRING       data  = (CSTRING)sqlite3_column_text(stmt, i);
            RexxObjectPtr value = (data == NULL) ? TheNilObj : c->String(data);

            c->SetStemElement(records, buf, value);
        }

        c->SetStemArrayElement(records, 0, c->StringSize(item));  // Update number of records in the stem.
    }

    freeHeadersUpper(headers, count);
    return records;
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
    char   *encrypt = (char *)"";

#ifdef __REXX64__
    bits = 64;
#endif

#ifdef SQLITE_HAS_CODEC
    encrypt = "(Encryption enabled)";
#endif

    if ( full )
    {
        if ( strlen(encrypt) > 0 )
        {
            snprintf(buf, sizeof(buf), "ooSQLite: ooSQLite Version %d.%d.%d.%d (%d bit) %s\n",
                     OOSQLITE_VER_MAJOR, OOSQLITE_VER_MINOR, OOSQLITE_VER_LEVEL, OOSQLITE_VER_BUILD, bits, encrypt);
        }
        else
        {
            snprintf(buf, sizeof(buf), "ooSQLite: ooSQLite Version %d.%d.%d.%d (%d bit)\n",
                     OOSQLITE_VER_MAJOR, OOSQLITE_VER_MINOR, OOSQLITE_VER_LEVEL, OOSQLITE_VER_BUILD, bits);
        }

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
            if ( strlen(encrypt) > 0 )
            {
                snprintf(buf, sizeof(buf), "ooSQLite Version %d.%d.%d.%d (%d bit) %s\n",
                         OOSQLITE_VER_MAJOR, OOSQLITE_VER_MINOR, OOSQLITE_VER_LEVEL, OOSQLITE_VER_BUILD, bits, encrypt);
            }
            else
            {
                snprintf(buf, sizeof(buf), "ooSQLite Version %d.%d.%d.%d (%d bit)\n",
                         OOSQLITE_VER_MAJOR, OOSQLITE_VER_MINOR, OOSQLITE_VER_LEVEL, OOSQLITE_VER_BUILD, bits);
            }
        }
    }

    return c->String(buf);
}


/**
 * Generic function to do a bitwise or (|) of a series of numbers. Intended to
 * allow the ooSQLite programmer to combine SQLite bit flags, but it makes no
 * check that the numbers provided are actually SQLite defines.
 *
 * This is not part of the SQLite library, it is an ooSQLite only interface.
 * Used by both the object orientated (.ooSQLiteConstants) and classic Rexx
 * interfaces.
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

/**
 * A generic function return the English language description for a result code.
 *
 * We add in our own ooSQLite codes to the SQLite sqlite3_errstr() function
 *
 * @param c
 * @param resultCode
 *
 * @return RexxObjectPtr
 */
RexxObjectPtr genErrStr(RexxThreadContext *c, int resultCode)
{
    CSTRING str = "unknown error";

    if ( resultCode >= OO_ERR_FIRST && resultCode <= OO_ERR_LAST )
    {
        switch ( resultCode )
        {
            case OO_BACKUP_DB_ERRSTATE :
                str = OO_BACKUP_DB_ERRSTATE_STR;
                break;

            case OO_BACKUP_IN_PROGRESS :
                str = OO_BACKUP_IN_PROGRESS_STR;
                break;

            case OO_INTERNAL_ERR :
                str = OO_INTERNAL_ERR_STR;
                break;

            case OO_UNEXPECTED_RESULT :
                str = OO_UNEXPECTED_RESULT_STR;
                break;

            case OO_WRONG_ARG_TYPE :
                str = OO_WRONG_ARG_TYPE_STR;
                break;

            default :
                break;
        }
    }
    else
    {
        str = sqlite3_errstr(resultCode);
    }

    return c->String(str);
}

int genStatus(RexxThreadContext *c, int param, RexxObjectPtr _result, logical_t reset, size_t pos, bool isMethod)
{
    if ( isMethod )
    {
        if ( ! c->IsDirectory(_result) )
        {
            wrongClassException(c, pos, "Directory", _result);
            return SQLITE_MISUSE;
        }
    }
    else
    {
        if (  ! c->IsStem(_result) )
        {
            wrongClassException(c, pos, "Stem", _result);
            return SQLITE_MISUSE;
        }
    }

    int cur;
    int hiwtr;

    int rc = sqlite3_status(param, &cur, &hiwtr, (int)reset);

    if ( rc == SQLITE_OK )
    {
        if ( isMethod )
        {
            RexxDirectoryObject result = (RexxDirectoryObject)_result;

            c->DirectoryPut(result, c->StringSize(cur), "CURRENT");
            c->DirectoryPut(result, c->StringSize(hiwtr), "HIGHWATER");
        }
        else
        {
            RexxStemObject result = (RexxStemObject)_result;

            c->SetStemElement(result, "CURRENT",   c->StringSize(cur));
            c->SetStemElement(result, "HIGHWATER", c->StringSize(hiwtr));
        }
    }

    return rc;
}

/**
 * Generic implementation of sqlite3_db_status for the OO and classic
 * interfaces.
 *
 * @param c
 * @param db
 * @param param
 * @param _result
 * @param reset
 * @param pos
 *
 * @return int
 *
 * @remarks  We cheat here in determining if we are called by a routine or a
 *           method.  We know that the arg pos is 2 for a method and 3 for a
 *           routine.
 */
int genDbStatus(RexxThreadContext *c, sqlite3 *db, int param, RexxObjectPtr _result, logical_t reset, size_t pos)
{
    if ( pos == 2 )
    {
        if ( ! c->IsDirectory(_result) )
        {
            wrongClassException(c, pos, "Directory", _result);
            return SQLITE_MISUSE;
        }
    }
    else
    {
        if ( ! c->IsStem(_result) )
        {
            wrongClassException(c, pos, "Stem", _result);
            return SQLITE_MISUSE;
        }
    }

    int cur;
    int hiwtr;

    int rc = sqlite3_db_status(db, param, &cur, &hiwtr, (int)reset);

    if ( rc == SQLITE_OK )
    {
        if ( pos == 2 )
        {
            RexxDirectoryObject result = (RexxDirectoryObject)_result;

            c->DirectoryPut(result, c->StringSize(cur), "CURRENT");
            c->DirectoryPut(result, c->StringSize(hiwtr), "HIGHWATER");
        }
        else
        {
            RexxStemObject result = (RexxStemObject)_result;

            c->SetStemElement(result, "CURRENT",   c->StringSize(cur));
            c->SetStemElement(result, "HIGHWATER", c->StringSize(hiwtr));
        }
    }

    return rc;
}

/**
 * Generic implemenation of tableColumnMetaData used for either the object
 * orientated interface, or the classic Rexx interface.
 *
 * @param c
 * @param db
 * @param dbName
 * @param tableName
 * @param colName
 * @param _results
 * @param pos
 *
 * @return int
 *
 * @note  dbName can be NULL, this is acceptable to SQLite.
 *
 * @remarks  We cheat here in the test for classic Rexx or not.  We know that
 *           the 'results' object is at arg position 3 if called from a method
 *           and at arg position 4 if called from a routine.
 */
int genTableColumnMetadata(RexxThreadContext *c, sqlite3 *db, CSTRING dbName, CSTRING tableName,
                           CSTRING colName, RexxObjectPtr _results, size_t pos)
{
    if ( pos == 3 && ! c->IsDirectory(_results) )
    {
        wrongClassException(c, pos, "Directory", _results);
        return SQLITE_MISUSE;
    }
    else if ( pos == 4 && ! c->IsStem(_results) )
    {
        wrongClassException(c, pos, "Stem", _results);
        return SQLITE_MISUSE;
    }

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
        if ( pos == 3 )
        {
            RexxDirectoryObject results = (RexxDirectoryObject)_results;

            c->DirectoryPut(results, c->String(dataType),     "DATATYPE");
            c->DirectoryPut(results, c->String(collSeq),      "COLLATIONSEQUENCE");
            c->DirectoryPut(results, c->Logical(notNull),     "NOTNULL");
            c->DirectoryPut(results, c->Logical(primaryKey),  "PRIMARYKEY");
            c->DirectoryPut(results, c->Logical(autoInc),     "AUTOINCREMENT");
        }
        else
        {
            RexxStemObject results = (RexxStemObject)_results;

            c->SetStemElement(results, "DATATYPE",          c->String(dataType));
            c->SetStemElement(results, "COLLATIONSEQUENCE", c->String(collSeq));
            c->SetStemElement(results, "NOTNULL",           c->Logical(notNull));
            c->SetStemElement(results, "PRIMARYKEY",        c->Logical(primaryKey));
            c->SetStemElement(results, "AUTOINCREMENT",     c->Logical(autoInc));
        }
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

    if ( d->initialThreadID != oosqlGetCurrentThreadId() )
    {
        if ( ! d->interpreter->AttachThread(&c) )
        {
            // There is nothing to be done about this, need to just quit.
            return SQLITE_OK;
        }
    }

    RexxObjectPtr rxOp   = c->WholeNumber(op);
    RexxObjectPtr rxStr1 = c->String(str1);
    RexxObjectPtr rxStr2 = c->String(str2);
    RexxObjectPtr rxStr3 = c->String(str3);
    RexxObjectPtr rxStr4 = c->String(str4);

    args = c->ArrayOfFour(rxOp, rxStr1, rxStr2, rxStr3);

    c->ArrayPut(args, rxStr4, 5);

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

    c->ReleaseLocalReference(rxOp);
    c->ReleaseLocalReference(rxStr1);
    c->ReleaseLocalReference(rxStr2);
    c->ReleaseLocalReference(rxStr3);
    c->ReleaseLocalReference(rxStr4);
    c->ReleaseLocalReference(reply);
    c->ReleaseLocalReference(args);

    if ( c != d->callbackContext )
    {
        c->DetachThread();
    }
    return (int)rc;
}

/**
 * The call back function for sqlite3_busy_handler()
 *
 * @param data
 *
 * @return If the busy callback returns 0, then no additional attempts are made
 *         to access the database and SQLITE_BUSY or SQLITE_IOERR_BLOCKED is
 *         returned. If the callback returns non-zero, then another attempt is
 *         made to open the database for reading and the cycle repeats
 */
static int busyCallBack(void *data, int countInvoked)
{
    pCGenericCallback  d = (pCGenericCallback)data;
    RexxThreadContext *c = d->callbackContext;

    if ( d->initialThreadID != oosqlGetCurrentThreadId() )
    {
        if ( ! d->interpreter->AttachThread(&c) )
        {
            // There is nothing to be done about this, need to just quit.
            return SQLITE_OK;
        }
    }

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

    c->ReleaseLocalReference(count);
    c->ReleaseLocalReference(reply);
    c->ReleaseLocalReference(args);

    if ( c != d->callbackContext )
    {
        c->DetachThread();
    }
    return (int)rc;
}


static int collationCallback(void* data, int len1, const void* str1, int len2, const void* str2)
{
    pCGenericCallback  d = (pCGenericCallback)data;
    RexxThreadContext *c = d->callbackContext;

    if ( d->initialThreadID != oosqlGetCurrentThreadId() )
    {
        if ( ! d->interpreter->AttachThread(&c) )
        {
            // There is nothing to be done about this, we should just use a
            // default compare, maybe stricmp.  Or we should maybe kill
            // everything??

            return sqlite3_strnicmp((char *)str1, (char *)str2, len1 < len2 ? len1 : len2);
        }
    }

    RexxArrayObject   args;
    RexxObjectPtr     reply = NULLOBJECT;
    RexxStringObject  rxStr1 = c->String((char *)str1, len1);
    RexxStringObject  rxStr2 = c->String((char *)str2, len2);
    wholenumber_t     cmp    = 0;

    bool isMethod = (d->callbackObj == NULLOBJECT) ? false : true;

    if ( d->userData == NULL )
    {
       args = c->ArrayOfThree(rxStr1, rxStr2, TheNilObj);
    }
    else
    {
        args = c->ArrayOfThree(rxStr1, rxStr2, d->userData);
    }

    if ( isMethod )
    {
        reply = c->SendMessage(d->callbackObj, d->callbackMethod, args);
    }
    else
    {
        reply = c->CallRoutine(d->callbackRtn, args);
    }

    if ( ! replyIsGood(c, reply, &cmp, d->callbackMethod, d->routineName, isMethod) )
    {
        cmp = 0;
    }

    c->ReleaseLocalReference(rxStr1);
    c->ReleaseLocalReference(rxStr2);
    c->ReleaseLocalReference(reply);
    c->ReleaseLocalReference(args);

    if ( c != d->callbackContext )
    {
        c->DetachThread();
    }
    return (int)cmp;
}

/**
 * The call back function for sqlite3_collation_needed()
 *
 * @param data
 *
 * @return void
 *
 * @notes  The Rexx callback method or routine must return a whole number, but
 *         the actual value is ignored.  Within the callback
 */
static void collationNeededCallback(void *data, sqlite3* db, int eTextRep, const char *name)
{
    pCGenericCallback  d = (pCGenericCallback)data;
    RexxThreadContext *c = d->callbackContext;

    if ( d->initialThreadID != oosqlGetCurrentThreadId() )
    {
        if ( ! d->interpreter->AttachThread(&c) )
        {
            // There is nothing to be done about this, need to just quit.  But,
            // I doubt it ever happens.
            return;
        }
    }

    RexxArrayObject   args;
    RexxStringObject  collationName = c->String(name);
    RexxObjectPtr     reply = NULLOBJECT;
    wholenumber_t     rc    = 0;

    bool isMethod = (d->callbackObj == NULLOBJECT) ? false : true;

    if ( d->userData == NULL )
    {
        args = c->ArrayOfThree(d->rexxDB, collationName, TheNilObj);
    }
    else
    {
        args = c->ArrayOfThree(d->rexxDB, collationName, d->userData);
    }

    if ( isMethod )
    {
        reply = c->SendMessage(d->callbackObj, d->callbackMethod, args);
    }
    else
    {
        reply = c->CallRoutine(d->callbackRtn, args);
    }

    // We just check for errors, we do not actually use rc.
    replyIsGood(c, reply, &rc, d->callbackMethod, d->routineName, isMethod);

    c->ReleaseLocalReference(reply);
    c->ReleaseLocalReference(collationName);
    c->ReleaseLocalReference(args);

    if ( c != d->callbackContext )
    {
        c->DetachThread();
    }
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

    bool isMethod = (d->callbackObj == NULLOBJECT) ? false : true;

    if ( d->initialThreadID != oosqlGetCurrentThreadId() )
    {
        if ( ! d->interpreter->AttachThread(&c) )
        {
            // There is nothing to be done about this, need to just quit.
            return SQLITE_OK;
        }
    }

    RexxArrayObject args;
    RexxObjectPtr   reply = NULLOBJECT;
    wholenumber_t   rc    = 0;

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

    c->ReleaseLocalReference(reply);
    c->ReleaseLocalReference(args);

    if ( c != d->callbackContext )
    {
        c->DetachThread();
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

    if ( d->initialThreadID != oosqlGetCurrentThreadId() )
    {
        if ( ! d->interpreter->AttachThread(&c) )
        {
            // There is nothing to be done about this, need to just quit.
            return;
        }
    }

    RexxObjectPtr rxStmt = c->String(statement);
    RexxObjectPtr rxNano = c->UnsignedInt64(nanosecs);

    if ( d->userData == NULL )
    {
        args = c->ArrayOfThree(rxStmt, rxNano, TheNilObj);
    }
    else
    {
        args = c->ArrayOfThree(rxStmt, rxNano, d->userData);
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

    c->ReleaseLocalReference(rxStmt);
    c->ReleaseLocalReference(rxNano);
    c->ReleaseLocalReference(reply);
    c->ReleaseLocalReference(args);

    if ( c != d->callbackContext )
    {
        c->DetachThread();
    }
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

    if ( d->initialThreadID != oosqlGetCurrentThreadId() )
    {
        if ( ! d->interpreter->AttachThread(&c) )
        {
            // There is nothing to be done about this, need to just quit.
            return SQLITE_OK;
        }
    }

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

    c->ReleaseLocalReference(reply);
    c->ReleaseLocalReference(args);

    if ( c != d->callbackContext )
    {
        c->DetachThread();
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

    if ( d->initialThreadID != oosqlGetCurrentThreadId() )
    {
        if ( ! d->interpreter->AttachThread(&c) )
        {
            // There is nothing to be done about this, need to just quit.
            return;
        }
    }

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

    c->ReleaseLocalReference(reply);
    c->ReleaseLocalReference(args);

    if ( c != d->callbackContext )
    {
        c->DetachThread();
    }
}


/**
 * The call back function for sqlite3_trace()
 *
 * @param data  A pointer to our callback struct
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

    if ( d->initialThreadID != oosqlGetCurrentThreadId() )
    {
        if ( ! d->interpreter->AttachThread(&c) )
        {
            // There is nothing to be done about this, need to just quit.
            return;
        }
    }

    RexxObjectPtr rxStmt = c->String(statement);

    if ( d->userData == NULL )
    {
        args = c->ArrayOfTwo(rxStmt, TheNilObj);
    }
    else
    {
        args = c->ArrayOfTwo(rxStmt, d->userData);
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

    c->ReleaseLocalReference(rxStmt);
    c->ReleaseLocalReference(reply);
    c->ReleaseLocalReference(args);

    if ( c != d->callbackContext )
    {
        c->DetachThread();
    }
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

    if ( d->initialThreadID != oosqlGetCurrentThreadId() )
    {
        if ( ! d->interpreter->AttachThread(&c) )
        {
            // There is nothing to be done about this, need to just quit.
            return;
        }
    }

    RexxObjectPtr rxOp        = c->WholeNumber(op);
    RexxObjectPtr rxDbName    = c->String(dbName);
    RexxObjectPtr rxTableName = c->String(tableName);
    RexxObjectPtr rxRowID     = c->Int64(rowID);

    args = c->ArrayOfFour(rxOp, rxDbName, rxTableName, rxRowID);

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

    c->ReleaseLocalReference(rxOp);
    c->ReleaseLocalReference(rxDbName);
    c->ReleaseLocalReference(rxTableName);
    c->ReleaseLocalReference(rxRowID);
    c->ReleaseLocalReference(reply);
    c->ReleaseLocalReference(args);

    if ( c != d->callbackContext )
    {
        c->DetachThread();
    }
}


/**
 * A generic call back into Rexx used to send a record from sqlite3_exec() to
 * the user.  This call turns out to be the same whether the format of single
 * record is a stem or a diretory.
 *
 * @param d
 * @param record
 *
 * @return wholenumber_t
 */
wholenumber_t genExecCall2Rexx(pCGenericCallback d, RexxObjectPtr record)
{
    wholenumber_t      rc = SQLITE_OK;
    RexxThreadContext *c = d->callbackContext;

    d->count++;

    RexxArrayObject args;
    RexxObjectPtr   reply    = NULLOBJECT;
    bool            isMethod = (d->callbackObj == NULLOBJECT) ? false : true;
    RexxObjectPtr   count    = c->WholeNumber(d->count);

    if ( d->userData == NULL )
    {
        args = c->ArrayOfTwo(record, count);
    }
    else
    {
        args = c->ArrayOfThree(record, count, d->userData);
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

    c->ReleaseLocalReference(reply);
    c->ReleaseLocalReference(args);
    c->ReleaseLocalReference(count);

    return rc;
}

/**
 * A generic function to produce the index names for each column in a result
 * set.  This is used by execCallBackDirectory() and execCallBackStem().
 *
 * @param d
 * @param ncols
 * @param headers
 *
 * @return bool
 */
static bool genCreateIndexes(pCGenericCallback d, int ncols, char **headers)
{
    RexxThreadContext *c = d->callbackContext;

    d->indexes = (char **)sqlite3_malloc(ncols * sizeof(char **));
    if ( d->indexes == NULL )
    {
        outOfMemoryException(c);
        return false;
    }

    for ( int i = 0; i < ncols; i++ )
    {
        char *index = strdupupr(headers[i]);
        if ( index == NULL )
        {
            outOfMemoryException(c);
            return false;
        }
        d->indexes[i] = index;
        d->countIndexes++;
    }
    return true;
}


/**
 * The call back function for sqlite3_exec() when the format of a record is an
 * array.
 *
 *
 * @param data
 * @param ncols
 * @param values
 * @param headers
 *
 * @return int
 *
 * @notes  This callback is always executing under the same thread as the
 *         callbackContext thread.
 */
static int execCallBackArray(void *data, int ncols, char **values, char **headers)
{
    pCGenericCallback  d = (pCGenericCallback)data;
    RexxThreadContext *c = d->callbackContext;

    RexxArrayObject record = c->NewArray(ncols);

    for ( int i = 0; i < ncols; i++ )
    {
        if ( values[i] == NULL )
        {
            c->ArrayPut(record, d->nullObj, i + 1);
        }
        else
        {
            c->ArrayAppendString(record, values[i], strlen(values[i]));
        }
    }

    wholenumber_t rc = SQLITE_OK;

    if ( d->createRS )
    {
        if ( d->count == 0 )
        {
            RexxArrayObject header = c->NewArray(ncols);

            for ( int i = 0; i < ncols; i++ )
            {
                c->ArrayAppendString(header, headers[i], strlen(headers[i]));
            }

            c->ArrayPut(d->rsArray, header, 1);
            d->count = 1;

            c->ReleaseLocalReference(header);
        }

        d->count++;
        c->ArrayPut(d->rsArray, record, d->count);
    }
    else
    {
        RexxObjectPtr   reply    = NULLOBJECT;
        RexxArrayObject rows     = c->NewArray(2);
        RexxArrayObject header   = c->NewArray(ncols);
        bool            isMethod = (d->callbackObj == NULLOBJECT) ? false : true;

        for ( int i = 0; i < ncols; i++ )
        {
            if ( values[i] == NULL )
            {
                c->ArrayPut(record, d->nullObj, i + 1);
            }
            else
            {
                c->ArrayAppendString(record, values[i], strlen(values[i]));
            }
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

        c->ReleaseLocalReference(reply);
        c->ReleaseLocalReference(rows);
        c->ReleaseLocalReference(header);
        c->ReleaseLocalReference(args);
        c->ReleaseLocalReference(count);
    }

    c->ReleaseLocalReference(record);
    return (int)rc;
}


/**
 * The call back function for sqlite3_exec() when the format of a record is a
 * directory.
 *
 *
 * @param data
 * @param ncols
 * @param values
 * @param headers
 *
 * @return int
 *
 * @note  This callback is always running on the same therea as the
 *        callbackContext thread
 */
static int execCallBackDirectory(void *data, int ncols, char **values, char **headers)
{
    pCGenericCallback  d = (pCGenericCallback)data;
    RexxThreadContext *c = d->callbackContext;

    RexxDirectoryObject record = c->NewDirectory();

    if ( d->indexes == NULL )
    {
        if ( ! genCreateIndexes(d, ncols, headers) )
        {
            return SQLITE_ERROR;
        }
    }

    for ( int i = 0; i < ncols; i++ )
    {
        CSTRING idx = d->indexes[i];

        RexxObjectPtr value;
        if ( values[i] == NULL )
        {
            value = d->nullObj;
        }
        else
        {
            value = c->String(values[i]);
        }

        c->DirectoryPut(record, value, idx);

        if ( value != TheNilObj )
        {
            c->ReleaseLocalReference(value);
        }
    }

    wholenumber_t rc = SQLITE_OK;

    if ( d->createRS )
    {
        d->count++;
        c->ArrayPut(d->rsArray, record, d->count);
    }
    else
    {
        rc = genExecCall2Rexx(d, record);
    }

    c->ReleaseLocalReference(record);
    return (int)rc;
}

/**
 * The call back function for sqlite3_exec() when the format of a record is a
 * stem of stems.
 *
 *
 * @param data
 * @param ncols
 * @param values
 * @param headers
 *
 * @return int
 *
 * @note  This callback is always running on the same thread as the
 *        callbackContext thread.
 */
static int execCallBackStem(void *data, int ncols, char **values, char **headers)
{
    pCGenericCallback  d = (pCGenericCallback)data;
    RexxThreadContext *c = d->callbackContext;

    RexxStemObject record = c->NewStem("record");

    if ( d->indexes == NULL )
    {
        if ( ! genCreateIndexes(d, ncols, headers) )
        {
            return SQLITE_ERROR;
        }
    }

    for ( int i = 0; i < ncols; i++ )
    {
        CSTRING idx = d->indexes[i];

        RexxObjectPtr value;
        if ( values[i] == NULL )
        {
            value = d->nullObj;
        }
        else
        {
            value = c->String(values[i]);
        }

        c->SetStemElement(record, idx, value);

        if ( value != TheNilObj )
        {
            c->ReleaseLocalReference(value);
        }
    }

    wholenumber_t rc = SQLITE_OK;

    if ( d->createRS )
    {
        d->count++;
        RexxObjectPtr num = c->UnsignedInt32(d->count);

        c->SetStemArrayElement(d->rsStem, d->count, record);
        c->SetStemArrayElement(d->rsStem, 0, num);

        c->ReleaseLocalReference(num);
    }
    else
    {
        rc = genExecCall2Rexx(d, record);
    }

    c->ReleaseLocalReference(record);
    return (int)rc;
}


/**
 * The call back function for sqlite3_exec() when the format of a record is a
 * classic stem.
 *
 *
 * @param data
 * @param ncols
 * @param values
 * @param headers
 *
 * @return int
 *
 * @note  This callback is always running on the same thread as the
 *        callbackContext thread.
 */
static int execCallBackClassicStem(void *data, int ncols, char **values, char **headers)
{
    pCGenericCallback  d = (pCGenericCallback)data;
    RexxThreadContext *c = d->callbackContext;

    if ( d->indexes == NULL )
    {
        if ( ! genCreateIndexes(d, ncols, headers) )
        {
            return SQLITE_ERROR;
        }
    }

    wholenumber_t rc = SQLITE_OK;

    if ( d->createRS )
    {
        d->count++;

        char buf[1024] = {0};
        for ( int i = 0; i < ncols; i++ )
        {
            CSTRING idx = d->indexes[i];

            snprintf(buf, sizeof(buf) - 1, "%d.%s", d->count, idx);

            RexxObjectPtr value;
            if ( values[i] == NULL )
            {
                value = d->nullObj;
            }
            else
            {
                value = c->String(values[i]);
            }

            c->SetStemElement(d->rsStem, buf, value);

            if ( value != TheNilObj )
            {
                c->ReleaseLocalReference(value);
            }
        }

        RexxObjectPtr num = c->UnsignedInt32(d->count);
        c->SetStemArrayElement(d->rsStem, 0, num);

        c->ReleaseLocalReference(num);
    }
    else
    {
        RexxStemObject record = c->NewStem("record");

        for ( int i = 0; i < ncols; i++ )
        {
            CSTRING idx = d->indexes[i];

            RexxObjectPtr value;
            if ( values[i] == NULL )
            {
                value = TheNilObj;
            }
            else
            {
                value = c->String(values[i]);
            }

            c->SetStemElement(record, idx, value);

            if ( value != TheNilObj )
            {
                c->ReleaseLocalReference(value);
            }
        }

        rc = genExecCall2Rexx(d, record);

        c->ReleaseLocalReference(record);
    }

    return (int)rc;
}

#define ooSQLite_Methods_Section  1


/**
 * Retrieves the CSelf pointer for a ooSQLiteConnection object when the
 * connection object is not the direct object the method was invoked on.  This
 * performs a scoped CSelf lookup.
 *
 * @param c    The method context we are operating in.
 * @param conn The database connection object whose CSelf pointer is needed.
 *
 * @return A pointer to the CSelf of the connection object.
 *
 * @assumes  The caller has ensured conn is in fact an ooSQLiteConnection Rexx
 *           connection object.
 */
inline pCooSQLiteConn dbToCSelf(RexxThreadContext *c, RexxObjectPtr conn)
{
    return (pCooSQLiteConn)c->ObjectToCSelf(conn, TheOOSQLiteConnectionClass);
}
inline pCooSQLiteConn dbToCSelf(RexxMethodContext *c, RexxObjectPtr conn)
{
    return dbToCSelf(c->threadContext, conn);
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


void noOpenDBException(RexxThreadContext *context, pCooSQLiteConn pConn)
{
    char buffer[256];
    if ( pConn->fileName != NULL )
    {
        snprintf(buffer, sizeof(buffer), "The %s data base is not open", pConn->fileName);
    }
    else
    {
        snprintf(buffer, sizeof(buffer), "There is no open data base");
    }
    executionErrorException(context, buffer);
}


void dbNotOpenException(RexxMethodContext *c, pCooSQLiteConn pConn, size_t pos)
{
    char buffer[256];
    if ( pConn->fileName != NULL )
    {
        snprintf(buffer, sizeof(buffer), "Argument %d, the %s data base connection, is not open", pos, pConn->fileName);
    }
    else
    {
        snprintf(buffer, sizeof(buffer), "Argument %d, the data base connection, is not open", pos);
    }
    executionErrorException(c->threadContext, buffer);
}


void invalidDbStateException(RexxMethodContext *c, pCooSQLiteConn pConn, size_t pos)
{
    char buffer[256];
    if ( pConn->fileName != NULL )
    {
        if ( pos == 0 )
        {
            snprintf(buffer, sizeof(buffer), "The %s data base is a backup destination target", pConn->fileName);
        }
        else
        {
            snprintf(buffer, sizeof(buffer), "Argument %d, the %s data base connection, is a backup destination target",
                     pos, pConn->fileName);
        }
    }
    else
    {
        if ( pos == 0 )
        {
            snprintf(buffer, sizeof(buffer), "The data base is a backup destination target");
        }
        else
        {
            snprintf(buffer, sizeof(buffer), "Argument %d, the data base connection, is a backup destination target", pos);
        }
    }
    executionErrorException(c->threadContext, buffer);
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


void invalidBuException(RexxThreadContext *context, pCooSQLiteBackup pCbu)
{
    char buffer[256];
    if ( pCbu->finished && ! pCbu->initializationErr )
    {
        snprintf(buffer, sizeof(buffer), "The ooSQLite backup object is not valid, it has been finished");
    }
    else
    {
        snprintf(buffer, sizeof(buffer), "The ooSQLite backup object is not valid, it was not opened without error");
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


static inline pCooSQLiteConn requiredDBCSelf(RexxMethodContext *c, void *pCSelf)
{
    if ( pCSelf == NULL )
    {
        baseClassInitializationException(c, "ooSQLiteConnection");
    }
    return (pCooSQLiteConn)pCSelf;
}

/**
 * Checks both that the database connection CSelf is not null and that the
 * connection is open and usable.  Usable means that the database is not the
 * target of a back up.
 *
 * @param c
 * @param pCSelf
 *
 * @return pCooSQLiteConn
 */
static inline pCooSQLiteConn requiredDB(RexxMethodContext *c, void *pCSelf)
{
    pCooSQLiteConn pConn = requiredDBCSelf(c, pCSelf);
    if ( pConn != NULL )
    {
        if ( pConn->db == NULL )
        {
            noOpenDBException(c->threadContext, pConn);
            pConn = NULL;
        }
        else if ( pConn->isDestinationBU )
        {
            invalidDbStateException(c, pConn, 0);
        }
    }
    return pConn;
}

static inline pCooSQLiteMutex requiredMtxCSelf(RexxMethodContext *c, void *pCSelf)
{
    if ( pCSelf == NULL )
    {
        baseClassInitializationException(c, "ooSQLiteMutex");
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
        baseClassInitializationException(c, "ooSQLiteStmt");
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

static inline pCooSQLiteBackup requiredBuCSelf(RexxMethodContext *c, void *pCSelf)
{
    if ( pCSelf == NULL )
    {
        baseClassInitializationException(c, "ooSQLiteBackup");
    }
    return (pCooSQLiteBackup)pCSelf;
}


/**
 * Returns the backup object's CSelf or null.  If the backup has been finished
 * or the backup ojbect has not been properly instantiated then a syntax
 * condition is raised.
 *
 * @param c
 * @param pCSelf
 *
 * @return pCooSQLiteBackup
 *
 * @note  We don't check pCbu->finished here because at finish pCbu->backup is
 *        set to null.
 */
static inline pCooSQLiteBackup requiredBackup(RexxMethodContext *c, void *pCSelf)
{
    pCooSQLiteBackup pCbu = requiredBuCSelf(c, pCSelf);
    if ( pCbu != NULL )
    {
        if ( pCbu->backup == NULL )
        {
            invalidBuException(c->threadContext, pCbu);
            pCbu = NULL;
        }
    }
    return pCbu;
}

/**
 * Returns the CSelf struct from an ooSQLiteConnection object.  The database
 * connection must be open and usable or a condition is raised.  Usable means
 * that the database can not be the destination target of a running backup
 * operation.
 *
 * @param c
 * @param dbConn
 * @param pos
 *
 * @return pCooSQLiteConn
 */
static pCooSQLiteConn requiredOpenConnection(RexxMethodContext *c, RexxObjectPtr dbConn, size_t pos)
{
    pCooSQLiteConn pConn = dbToCSelf(c, dbConn);

    if ( pConn == NULL )
    {
        baseClassInitializationException(c, "ooSQLiteConnection");
        return NULL;
    }

    if ( pConn->db == NULL )
    {
        dbNotOpenException(c, pConn, pos);
        return NULL;
    }

    if ( pConn->isDestinationBU )
    {
        invalidDbStateException(c, pConn, pos);
        return NULL;
    }
    return pConn;
}

static pCooSQLiteConn requiredDBConnectionArg(RexxMethodContext *c, RexxObjectPtr dbConn, size_t pos)
{
    if ( ! c->IsOfType(dbConn, "ooSQLiteConnection") )
    {
        wrongClassException(c->threadContext, pos, "ooSQLiteConnection", dbConn);
        return NULL;
    }
    return requiredOpenConnection(c, dbConn, pos);
}

/**
 * Each different callback type has an unique string name for the object
 * variable used to store the callback's Rexx buffer object
 *
 * This function returns the unique name.
 *
 * @param cb
 *
 * @return CSTRING
 */
static inline CSTRING getCallbackVarName(CallbackType cb)
{
    switch ( cb )
    {
        case authorizer :
            return "__rxAuthorizer";
        case busyHandler :
            return "__rxBusyHandler";
        case collation :
            return "__rxCollation";
        case collationNeeded :
            return "__rxCollationNeeded";
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

/**
 * Each Rexx callback registered for a database connection allocates a generic
 * callback struct as a RexxBufferObject.  The Rexx buffer object is stored as
 * an object variable of the Rexx database connection.
 *
 * This function retrieves the Rexx buffer object.
 *
 * @param c
 * @param cb
 * @param zName
 *
 * @return RexxBufferObject
 *
 * @remarks  This is somewhat complicated for a collation callback because any
 *           single database connection can have more than one collation
 *           function.
 */
static RexxBufferObject getCallbackVar(RexxMethodContext *c, CallbackType cb, CSTRING zName)
{
    CSTRING varName = getCallbackVarName(cb);

    if ( cb == collation )
    {
        RexxObjectPtr table = c->GetObjectVariable(varName);
        if ( table != NULLOBJECT )
        {
            RexxStringObject index = c->String(zName);
            return (RexxBufferObject)c->SendMessage1(table, "REMOVE", c->StringUpper(index));
        }
        return NULLOBJECT;
    }
    return (RexxBufferObject)c->GetObjectVariable(varName);
}

/**
 * Each Rexx callback registered for a database connection allocates a generic
 * callback struct as a RexxBufferObject.  The Rexx buffer object is stored as
 * an object variable of the Rexx database connection.
 *
 * This function sets the object variable with the Rexx buffer object.
 *
 * @param c
 * @param buf
 * @param cb
 * @param zName
 *
 * @remarks  This is somewhat complicated for a collation callback because any
 *           single database connection can have more than one collation
 *           function.
 */
static void setCallbackVar(RexxMethodContext *c, RexxBufferObject buf, CallbackType cb, CSTRING zName)
{
    CSTRING varName = getCallbackVarName(cb);

    if ( cb == collation )
    {
        RexxObjectPtr table = c->GetObjectVariable(varName);
        if ( table == NULLOBJECT )
        {
            table = rxNewBuiltinObject(c, "TABLE");
            if ( table != NULLOBJECT )
            {
                c->SetObjectVariable(varName, table);
            }
        }

        if ( table != NULLOBJECT )
        {
            RexxStringObject index = c->String(zName);
            c->SendMessage2(table, "PUT", buf, index);
        }

        return;
    }

    c->SetObjectVariable(varName, buf);
}

/**
 * For each Rexx callback registered for a database connection, a generic
 * callback struct is allocated as a RexxBufferObject and stored as an object
 * variable of the Rexx database connection object.  When the callback is
 * removed, the RexxBufferObject needs to be cleaned up.
 *
 * This is somewhat complicated for a collation callback, because any single
 * database connection can have multiple collations.
 *
 * This function cleans up all the collations for the database connection.  The
 * Rexx buffer objects are stored in an object variable that is a table.  We
 * retrieve all the items in the table, clean them up and remove the table
 * itself.
 *
 * @param c
 * @param varName
 */
static void cleanupAllCollationBuffers(RexxMethodContext *c, CSTRING varName)
{
    RexxObjectPtr table = c->GetObjectVariable(varName);

    if ( table != NULLOBJECT )
    {
        RexxArrayObject items = (RexxArrayObject)c->SendMessage0(table, "ALLITEMS");

        if ( items != NULLOBJECT )
        {
            size_t count = c->ArrayItems(items);
            for ( size_t i = 1; i <= count; i++ )
            {
                RexxBufferObject cbcBuffer = (RexxBufferObject)c->ArrayAt(items, i);

                if ( cbcBuffer != NULLOBJECT )
                {
                    pCGenericCallback cbc = (pCGenericCallback)c->BufferData(cbcBuffer);

                    c->ReleaseGlobalReference(cbc->callbackObj);

                    if ( cbc->userData != NULL )
                    {
                        c->ReleaseGlobalReference(cbc->userData);
                    }
                }
            }
        }
        c->SendMessage0(table, "EMPTY");
        c->DropObjectVariable(varName);
    }
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
 *            should be, an ooSQLiteConnection method.
 *
 * @param cb  Identifies the callback type so that the object variable can be
 *            retrieved.
 *
 * @param zName  For a collation callback only, this identifies the collation.
 *               Any single database connection can have multiple collations.
 *               If this is a collation callback, and zName is null, it means
 *               clean up all of the collations for the connection.
 */
static void cleanupCallback(RexxMethodContext *c, CallbackType cb, CSTRING zName)
{
    CSTRING varName = getCallbackVarName(cb);

    if ( cb == collation && zName == NULL )
    {
        cleanupAllCollationBuffers(c, varName);
        return;
    }

    RexxBufferObject cbcBuffer = getCallbackVar(c, cb, zName);

    if ( cbcBuffer != NULLOBJECT )
    {
        pCGenericCallback cbc = (pCGenericCallback)c->BufferData(cbcBuffer);

        c->ReleaseGlobalReference(cbc->callbackObj);

        if ( cbc->userData != NULL )
        {
            c->ReleaseGlobalReference(cbc->userData);
        }
        if ( cb != collation )
        {
            c->DropObjectVariable(varName);
        }
    }
}


/**
 * Cleans up all the Rexx callback stuff for a database connection.  Called when
 * the database connection is closed, or at uninit()
 *
 * @param c Rexx method context we are operating in.  Assumed to be, and should
 *          be, an ooSQLiteConnection method.
 */
static void cleanupCallbacks(RexxMethodContext *c)
{
    cleanupCallback(c, authorizer, NULL);
    cleanupCallback(c, busyHandler, NULL);
    cleanupCallback(c, collation, NULL);
    cleanupCallback(c, commitHook, NULL);
    cleanupCallback(c, profileHook, NULL);
    cleanupCallback(c, progressHandler, NULL);
    cleanupCallback(c, rollbackHook, NULL);
    cleanupCallback(c, traceHook, NULL);
    cleanupCallback(c, updateHook, NULL);
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

        c->ReleaseGlobalReference(cbc->callbackObj);

        if ( cbc->userData != NULL )
        {
            c->ReleaseGlobalReference(cbc->userData);
        }
    }
}


/**
 *  Methods for the .ooSQLiteConstants class.
 */
#define OOSQLCONSTANTS_CLASS    "ooSQLiteConstants"

/** ooSQLiteConstants::merge()  [Class method]
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


/** ooSQLite::init()
 */
RexxMethod1(RexxObjectPtr, oosql_init_cls, OSELF, self)
{
    if ( isOfClassType(context, self, OOSQLITE_CLASS) )
    {
        TheOOSQLiteClass = (RexxClassObject)self;
        context->RequestGlobalReference(TheOOSQLiteClass);

        // Get a buffer for the CSelf.
        RexxBufferObject cselfBuffer = context->NewBuffer(sizeof(CooSQLiteClass));
        if ( cselfBuffer == NULLOBJECT )
        {
            outOfMemoryException(context->threadContext);
            return NULLOBJECT;
        }

        context->SetObjectVariable("CSELF", cselfBuffer);

        pCooSQLiteClass pCoosql = (pCooSQLiteClass)context->BufferData(cselfBuffer);
        memset(pCoosql, 0, sizeof(CooSQLiteClass));

        pCoosql->format  = anArrayOfDirectories;
        pCoosql->nullObj = TheNilObj;
    }
    return NULLOBJECT;
}


/** ooSQLite::null  [attribute get]
 */
RexxMethod1(RexxObjectPtr, oosql_getNull_atr_cls, CSELF, pCSelf)
{
    return ((pCooSQLiteClass)pCSelf)->nullObj;
}
/** ooSQLite::null  [attribute get]
 *
 *  @note  The default value to use for SQL NULL is set in this, the ooSQLite
 *         class. When a new connection object is created, the default value of
 *         NULL is set for that object using the default value.
 *
 *         If the NULL value is the nil object, we do not need to worry about
 *         garbage collection.  If it is a Rexx string value, we do.  When the
 *         NULL attribute is set, we release the global reference on the old
 *         value if it was a Rexx string object.  For the connection object, we
 *         do not want to release the global reference on the global default
 *         value, so for that object we create a new Rexx string  for the
 *         object.  This is why we save the CSTRING value of the global NULL
 *         value.
 */
RexxMethod2(RexxObjectPtr, oosql_setNull_atr_cls, RexxObjectPtr, nullObj, CSELF, pCSelf)
{
    pCooSQLiteClass pCoosql = (pCooSQLiteClass)pCSelf;
    RexxObjectPtr   oldNull = pCoosql->nullObj;

    if ( nullObj == TheNilObj )
    {
        pCoosql->nullObj = nullObj;
        pCoosql->nullStr = NULL;
    }
    else
    {
        pCoosql->nullStr = context->ObjectToStringValue(nullObj);
        pCoosql->nullObj = context->String(pCoosql->nullStr);

        context->RequestGlobalReference(pCoosql->nullObj);
    }

    if ( oldNull != TheNilObj )
    {
        context->ReleaseGlobalReference(oldNull);
    }

    return NULLOBJECT;
}


/** ooSQLite::recordFormat  [attribute get]
 */
RexxMethod1(uint32_t, oosql_getRecordFormat_atr_cls, CSELF, pCSelf)
{
    return ((pCooSQLiteClass)pCSelf)->format;
}
/** ooSQLite::recordFormat  [attribute get]
 */
RexxMethod2(RexxObjectPtr, oosql_setRecordFormat_atr_cls, uint32_t, format, CSELF, pCSelf)
{
    if ( format < anArrayOfArrays || format > aClassicStem )
    {
        wrongArgValueException(context->threadContext, 1, RECORD_FORMATS_LIST, context->UnsignedInt32(format));
    }
    else
    {
        ((pCooSQLiteClass)pCSelf)->format = (ResultSetType)format;
    }
    return NULLOBJECT;
}


/** ooSQLite::compileOptionGet  [class method]
 *
 *  sqlite3_compileoption_get() uses 0 through N - 1  to get the Nth compile
 *  option.  We adjust for using 1 through N in Rexx.
 *
 *  @note  Returns "NULL" for the the null pointer return from sqlite.
 */
RexxMethod1(RexxObjectPtr, oosql_compileOptionGet_cls, int, index)
{
    index--;
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

/** ooSQLite::encryptionAvailable()  [class method]
 *
 *  Returns true if the Boton crypto library is built in, otherwise false.
 *
 */
RexxMethod0(logical_t, oosql_encryptionAvailable_cls)
{
#ifdef SQLITE_HAS_CODEC
    return 1;
#else
    return 0;
#endif
}

/** ooSQLite::enquote  [class method]
 *
 *  @param values  [optional]
 *
 */
RexxMethod1(RexxObjectPtr, oosql_enquote_cls, OPTIONAL_RexxObjectPtr, values)
{
    if ( argumentOmitted(1) )
    {
        return context->String("NULL");
    }
    else if ( ! context->IsArray(values) )
    {
        char *s = sqlite3_mprintf("%Q", obj2str(context->threadContext, values));
        return enquoteResult(context->threadContext, s);
    }
    else
    {
        RexxArrayObject a = (RexxArrayObject)values;
        size_t          s = context->ArraySize(a);

        switch ( s )
        {
            case 0 :
                return context->String("NULL");
            case 1 :
                return enquote1(context->threadContext, a);
            case 2 :
                return enquote2(context->threadContext, a);
            case 3 :
                return enquote3(context->threadContext, a);
            case 4 :
                return enquote4(context->threadContext, a);
            case 5 :
                return enquote5(context->threadContext, a);
            case 6 :
                return enquote6(context->threadContext, a);
            default :
                return enquoteN(context->threadContext, a, s);

        }
    }
}


/** ooSQLite::errStr()  [class method]
 *
 *
 */
RexxMethod1(RexxObjectPtr, oosql_errStr_cls, int, errCode)
{
    return genErrStr(context->threadContext, errCode);
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
RexxMethod1(int64_t, oosql_memoryHighWater_cls, OPTIONAL_logical_t, reset)
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
    return genStatus(context->threadContext, param, _result, reset, 2, true);
}

/** ooSQLite::threadSafe()
 *
 *
 */
RexxMethod0(int, oosql_threadSafe_cls)
{
    return sqlite3_threadsafe();
}


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

/**
 * Internal use only.
 *
 * Transient method used to test various things as they come up.
 *
 */
RexxMethod1(int, oosql_test_cls, ARGLIST, args)
{
    RexxMethodContext *c = context;

    size_t size = c->ArraySize(args);

    printf("CONSTRAINT_CHECK     =%d\n", SQLITE_CONSTRAINT_CHECK     );
    printf("CONSTRAINT_COMMITHOOK=%d\n", SQLITE_CONSTRAINT_COMMITHOOK);
    printf("CONSTRAINT_FOREIGNKEY=%d\n", SQLITE_CONSTRAINT_FOREIGNKEY);
    printf("CONSTRAINT_FUNCTION  =%d\n", SQLITE_CONSTRAINT_FUNCTION  );
    printf("CONSTRAINT_NOTNULL   =%d\n", SQLITE_CONSTRAINT_NOTNULL   );
    printf("CONSTRAINT_PRIMARYKEY=%d\n", SQLITE_CONSTRAINT_PRIMARYKEY);
    printf("CONSTRAINT_TRIGGER   =%d\n", SQLITE_CONSTRAINT_TRIGGER   );
    printf("CONSTRAINT_UNIQUE    =%d\n", SQLITE_CONSTRAINT_UNIQUE    );
    printf("CONSTRAINT_VTAB      =%d\n", SQLITE_CONSTRAINT_VTAB      );
    printf("READONLY_ROLLBACK    =%d\n", SQLITE_READONLY_ROLLBACK    );

    return 0;
}


/**
 *  Methods for the .ooSQLiteConnection class.
 */
#define OOSQLITECONNECTION_CLASS    "ooSQLiteConnection"
#define STMTSET_ATTRIBUTE           "ooSQLiteConnectionStmtBag"


/**
 * This function ensures that any unfinalized prepared statements for the
 * specified database connection are finalized.
 *
 * Before a SQLite database connection can be closed, all prepared statements
 * for that specific connection must be finalized.
 *
 * ooSQLite keeps track of all prepared statements, that have not been
 * finalized, for any open database connection.  During the uninit() method for
 * an ooSQLiteConnection object, if the connection has not been closed, it is closed.
 * At this time, if there are any unfinalized prepared statements, they are
 * finalized so that the database connection will close.
 *
 * @param c
 * @param pConn
 */
static void ensureFinalized(RexxMethodContext *c, pCooSQLiteConn pConn)
{
    CRITICAL_SECTION_ENTER

    RexxArrayObject stmts = (RexxArrayObject)c->SendMessage0(pConn->stmtBag, "ALLITEMS");
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

        c->SendMessage1(pConn->stmtBag, "REMOVE", stmt);
    }

    CRITICAL_SECTION_LEAVE
}


/**
 * Find the ooSQLiteStmt object for the specified prepared statement and return
 * it.  Returns .nil if not found.
 *
 * @param c
 * @param pConn
 * @param stmt
 *
 * @return RexxObjectPtr
 */
static RexxObjectPtr findStatement(RexxMethodContext *c, pCooSQLiteConn pConn, sqlite3_stmt *stmt)
{
    CRITICAL_SECTION_ENTER

    RexxObjectPtr   result = TheNilObj;
    RexxArrayObject stmts  = (RexxArrayObject)c->SendMessage0(pConn->stmtBag, "ALLITEMS");
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
    else if ( cbt == collation )
    {
        return "COLLATIONCALLBACK";   // collationCallback()
    }
    else if ( cbt == collationNeeded )
    {
        return "COLLATIONNEEDEDCALLBACK";   // collationNeededCallback()
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


static RexxObjectPtr removeCallback(RexxMethodContext *c, sqlite3 *db, CallbackType cbt, CSTRING zName)
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
    else if ( cbt == collation )
    {
        rc = sqlite3_create_collation_v2(db, zName, SQLITE_UTF8, NULL, NULL, NULL);
        result = c->WholeNumber(rc);
    }
    else if ( cbt == collationNeeded )
    {
        // Note that docs do not actually say this removes the callback.
        rc = sqlite3_collation_needed(db, NULL, NULL);
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

    cleanupCallback(c, cbt, zName);

    return result;
}


static RexxObjectPtr installCallback(RexxMethodContext *c, sqlite3 *db, pCGenericCallback cbc, CallbackType cbt,
                                     RexxBufferObject oldCallback, int instructions, CSTRING zName)
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
    else if ( cbt == collation )
    {
        rc = sqlite3_create_collation_v2(db, zName, SQLITE_UTF8, (void *)cbc, collationCallback, NULL);
        result = c->WholeNumber(rc);
    }
    else if ( cbt == collationNeeded )
    {
        rc = sqlite3_collation_needed(db, (void *)cbc, collationNeededCallback);
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
 * Does the setup, which is basically the same, for most of the SQLite provided
 * callback registrations: sqlite3_commit_hook(), sqlite3_rollback_hook(),
 * sqlite3_update_hook(), etc., etc..
 *
 * @param c
 * @param db
 * @param callbackObj
 * @param mthName
 * @param userData
 * @param cbt        Identifies which callback registration this is for.
 *
 * @param instructions Only used for the progressHandler callback.  Value is
 *                     ignored for all other callback types
 *
 * @param zName        Only used for the create collation callback.  Value is
 *                     ignored for all other callback types
 *
 * @return RexxObjectPtr
 *
 * @notes  We request a global reference for both the user data *and* the call
 *         back oject.  The Rexx programmer may not keep a reference to either
 *         of them and we don't want them garbage collected out from under us
 *         during a callback.
 */
static RexxObjectPtr doCallbackSetup(RexxMethodContext *c, pCooSQLiteConn pConn, RexxObjectPtr callbackObj, CSTRING mthName,
                                     RexxObjectPtr userData, CallbackType cbt, int instructions, CSTRING zName)
{
    sqlite3 *db = pConn->db;

    if ( callbackObj == TheNilObj )
    {
        return removeCallback(c, db, cbt, zName);
    }

    RexxBufferObject oldCallback = getCallbackVar(c, cbt, zName);
    RexxBufferObject cbcBuffer   = c->NewBuffer(sizeof(CGenericCallback));
    if ( cbcBuffer == NULLOBJECT )
    {
        outOfMemoryException(c->threadContext);
        return c->WholeNumber(SQLITE_MISUSE);
    }

    setCallbackVar(c, cbcBuffer, cbt, zName);

    pCGenericCallback cbc = (pCGenericCallback)c->BufferData(cbcBuffer);
    memset(cbc, 0, sizeof(CGenericCallback));

    cbc->callbackObj     = c->RequestGlobalReference(callbackObj);
    cbc->callbackContext = c->threadContext;
    cbc->rexxDB          = pConn->rexxSelf;
    cbc->interpreter     = c->threadContext->instance;
    cbc->initialThreadID = oosqlGetCurrentThreadId();

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

    return installCallback(c, db, cbc, cbt, oldCallback, instructions, zName);
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
    if ( strcmp(pName, "busy_timeout")              == 0 ) return busyTimeout;
    if ( strcmp(pName, "cache_size")                == 0 ) return cacheSize;
    if ( strcmp(pName, "case_sensitive_like")       == 0 ) return caseSensitiveLike;
    if ( strcmp(pName, "checkpoint_fullfsync")      == 0 ) return checkpointFullfsync;
    if ( strcmp(pName, "collation_list")            == 0 ) return collationList;
    if ( strcmp(pName, "compile_options")           == 0 ) return compileOptions;
    if ( strcmp(pName, "database_list")             == 0 ) return databaseList;
    if ( strcmp(pName, "encoding")                  == 0 ) return encoding;
    if ( strcmp(pName, "foreign_key_check")         == 0 ) return foreignKeyCheck;
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
 * @param pConn
 * @param name
 * @param pragma
 *
 * @return A Rexx object as described above on success, or an error return code
 *         with message.
 */
RexxObjectPtr pragmaTrigger(RexxMethodContext *c, pCooSQLiteConn pConn, CSTRING name, RexxObjectPtr value, PragmaType pragma)
{
    char     buf[256];
    sqlite3 *db = pConn->db;

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
        ooSQLiteNoErr(c, pConn);

        if ( pragma == shrinkMemory )
        {
            rc = sqlite3_step(stmt);
            if ( rc == SQLITE_DONE )
            {
                result = c->WholeNumber(SQLITE_OK);
            }
            else
            {
                result = ooSQLiteErr(c, pConn, rc, sqlite3_errmsg(db), true);
            }
        }
        else
        {
            switch ( pConn->format )
            {
                case anArrayOfArrays :
                    result = getRecordsArray(c->threadContext, stmt, pConn->nullObj);
                    break;
                case anArrayOfDirectories :
                    result = getRecordsDirectory(c->threadContext, stmt);
                    break;
                case aClassicStem :
                    result = getRecordsClassicStem(c->threadContext, stmt);
                    break;
                default :
                    result = getRecordsStem(c->threadContext, stmt);
                    break;
            }
        }
    }
    else
    {
        // This sets the last error attributes.
        result = ooSQLiteErr(c, pConn, rc, sqlite3_errmsg(db), true);

        if ( pragma != shrinkMemory )
        {
            // But, if not shrinkMemory we want to return an empty result set:
            if ( pConn->format == aStemOfStems || pConn->format == aClassicStem )
            {
                RexxStemObject r = c->NewStem("records");
                c->SetStemArrayElement(r, 0, TheZeroObj);

                result = r;
            }
            else
            {
                result = c->NewArray(0);
            }
        }
    }

    sqlite3_finalize(stmt);

    return result;
}

/**
 * Processes a query only pragma that returns the records in the default result
 * set format for the database connection..
 *
 * @param c
 * @param pConn
 * @param name
 * @param value
 * @param pragma
 *
 * @return A result set containing the records returned by the pragma statement.
 *
 * @note  If there are no rows returned by the pragma statement, then an empty
 *        result set is returned.
 */
RexxObjectPtr pragmaList(RexxMethodContext *c, pCooSQLiteConn pConn, CSTRING name, RexxObjectPtr value, PragmaType pragma)
{
    char     buf[256];
    sqlite3 *db = pConn->db;

    if ( pragma == collationList || pragma == databaseList || pragma == compileOptions )
    {
        snprintf(buf, sizeof(buf), "PRAGMA %s;", name);
    }
    else if ( pragma == foreignKeyCheck )
    {
        if ( value == NULLOBJECT )
        {
            snprintf(buf, sizeof(buf), "PRAGMA %s;", name);
        }
        else
        {
            snprintf(buf, sizeof(buf), "PRAGMA %s(%s);", name, c->ObjectToStringValue(value));
        }
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
        ooSQLiteNoErr(c, pConn);

        switch ( pConn->format )
        {
            case anArrayOfArrays :
                result = getRecordsArray(c->threadContext, stmt, pConn->nullObj);
                break;
            case anArrayOfDirectories :
                result = getRecordsDirectory(c->threadContext, stmt);
                break;
            case aClassicStem :
                result = getRecordsClassicStem(c->threadContext, stmt);
                break;
            default :
                result = getRecordsStem(c->threadContext, stmt);
                break;
        }
    }
    else
    {
        // Set the last error attributes of the connections.
        ooSQLiteErr(c, pConn, rc, sqlite3_errmsg(db), true);

        // Return an empty result set.
        if ( pConn->format == aStemOfStems || pConn->format == aClassicStem )
        {
            RexxStemObject r = c->NewStem("records");
            c->SetStemArrayElement(r, 0, TheZeroObj);

            result = r;
        }
        else
        {
            result = c->NewArray(0);
        }
    }

    sqlite3_finalize(stmt);

    return result;
}

/**
 * Processes a pragma that is expected to return a single value.
 *
 * @param c
 * @param pConn
 * @param name
 * @param pragma
 *
 * @return RexxObjectPtr
 */
RexxObjectPtr pragmaGet(RexxMethodContext *c, pCooSQLiteConn pConn, CSTRING name, PragmaType pragma)
{
    char buf[128];
    snprintf(buf, sizeof(buf), "PRAGMA %s;", name);

    sqlite3_stmt *stmt;
    sqlite3      *db = pConn->db;

    int rc = sqlite3_prepare_v2(db, buf, (int)strlen(buf) + 1, &stmt, NULL);

    if ( rc != SQLITE_OK )
    {
        return ooSQLiteErr(c, pConn, rc, sqlite3_errmsg(db), true);
    }

    RexxObjectPtr result;

    rc = sqlite3_step(stmt);
    if ( rc == SQLITE_ROW && sqlite3_column_count(stmt) == 1 )
    {
        result = safeColumnText(c->threadContext,stmt , 0, pConn->nullObj);
    }
    else
    {
        snprintf(buf, sizeof(buf), "Unexpected result, no data returned from pragma %s", name);
        result = ooSQLiteErr(c, pConn, OO_UNEXPECTED_RESULT, buf, false);
    }

    sqlite3_finalize(stmt);

    return result;
}

/**
 * Processes a pragma that is setting a value
 *
 * @param c
 * @param pConn
 * @param name
 * @param value
 * @param pragma
 *
 * @return RexxObjectPtr
 *
 * @note  We are only called internally, so we know for sure that 'value' is not
 *        the NULLOBJECT.
 */
RexxObjectPtr pragmaSet(RexxMethodContext *c, pCooSQLiteConn pConn, CSTRING name, RexxObjectPtr value, PragmaType pragma)
{
    char     buf[256];
    sqlite3 *db = pConn->db;

    snprintf(buf, sizeof(buf), "PRAGMA %s(%s);", name, c->ObjectToStringValue(value));

    sqlite3_stmt *stmt;

    int rc = sqlite3_prepare_v2(db, buf, (int)strlen(buf) + 1, &stmt, NULL);
    if ( rc != SQLITE_OK )
    {
        return ooSQLiteErr(c, pConn, rc, sqlite3_errmsg(db), true);
    }

    RexxObjectPtr result = TheZeroObj;

    rc = sqlite3_step(stmt);

    if ( rc != SQLITE_DONE )
    {
        if ( rc == SQLITE_ROW )
        {
            // This is unexpected to me, but maybe it is okay?
            snprintf(buf, sizeof(buf), "Unexpected result, data returned from pragma %s", name);
            result = ooSQLiteErr(c, pConn, OO_UNEXPECTED_RESULT, buf, false);
        }
        else
        {
            return ooSQLiteErr(c, pConn, rc, sqlite3_errmsg(db), true);
        }
    }

    sqlite3_finalize(stmt);

    return result;

}


/**
 * Saves the database file name in an object variable of an ooSQLiteConnection object
 * when the object is instantiated.
 *
 * @param c
 * @param pConn
 * @param file
 */
static void setDBInitStatusFile(RexxMethodContext *c, pCooSQLiteConn pConn, CSTRING file)
{
    RexxStringObject rxName = c->String(file);
    c->SetObjectVariable("rxFileName", rxName);

    pConn->rxFileName = rxName;
    pConn->fileName   = file;
}

/**
 * Provides the final initialization of an ooSQLiteConnection object during
 * instantiation.  This initialization is dependent on whether the underlying
 * SQLite database connection was opened with or without error.
 *
 * @param c
 * @param pConn
 * @param db
 * @param rc
 * @param self
 *
 * @return bool
 */
static bool setDBInitStatus(RexxMethodContext *c, pCooSQLiteConn pConn, sqlite3 *db, int rc, RexxObjectPtr self)
{
    bool result = true;
    if ( rc )
    {
        pConn->lastErrCode = sqlite3_errcode(db);
        pConn->lastErrMsg  = c->String(sqlite3_errmsg(db));
        pConn->initCode    = rc;

        sqlite3_close(pConn->db);
        pConn->db = NULL;

        result = false;
    }
    else
    {
        pConn->lastErrMsg  = c->String("No error");
        pConn->rexxSelf    = self;

        RexxObjectPtr set = rxNewSet(c);
        pConn->stmtBag = set;
        c->SendMessage1(self, "OOSQLITECONNECTIONSTMTBAG=", set);

        // Here we enable things that are enabled / disabled on a per database
        // connection basis that are set by default for ooSQLite.
        sqlite3_extended_result_codes(pConn->db, 1);
    }

    c->SetObjectVariable("rxLastErrMsg", pConn->lastErrMsg);

    return result;
}


/** ooSQLiteConnection:Class::init()
 */
RexxMethod1(RexxObjectPtr, oosqlconn_init_cls, OSELF, self)
{
    if ( isOfClassType(context, self, OOSQLITECONNECTION_CLASS) )
    {
        TheOOSQLiteConnectionClass = (RexxClassObject)self;
        context->RequestGlobalReference(TheOOSQLiteConnectionClass);
    }
    return NULLOBJECT;
}


/** ooSQLiteConnection::backupDestination  [attribute get]
 */
RexxMethod1(logical_t, oosqlconn_getBackupDestination_atr, CSELF, pCSelf)
{
    pCooSQLiteConn pConn = requiredDBCSelf(context, pCSelf);
    if ( pConn == NULL )
    {
        return 1;
    }
    return pConn->isDestinationBU;
}

/** ooSQLiteConnection::closed  [attribute get]
 */
RexxMethod1(logical_t, oosqlconn_getClosed_atr, CSELF, pCSelf)
{
    pCooSQLiteConn pConn = requiredDBCSelf(context, pCSelf);
    if ( pConn == NULL )
    {
        return 1;
    }
    return pConn->closed;
}

/** ooSQLiteConnection::fileName  [attribute get]
 */
RexxMethod1(RexxStringObject, oosqlconn_getFileName_atr, CSELF, pCSelf)
{
    pCooSQLiteConn pConn = requiredDBCSelf(context, pCSelf);
    if ( pConn == NULL )
    {
        return NULLOBJECT;
    }
    return pConn->rxFileName;
}

/** ooSQLiteConnection::initCode  [attribute get]
 */
RexxMethod1(int, oosqlconn_getInitCode_atr, CSELF, pCSelf)
{
    pCooSQLiteConn pConn = requiredDBCSelf(context, pCSelf);
    if ( pConn == NULL )
    {
        return -1;
    }
    return pConn->initCode;
}

/** ooSQLiteConnection::lastErrCode  [attribute get]
 */
RexxMethod1(int, oosqlconn_getLastErrCode_atr, CSELF, pCSelf)
{
    pCooSQLiteConn pConn = requiredDBCSelf(context, pCSelf);
    if ( pConn == NULL )
    {
        return -1;
    }
    return pConn->lastErrCode;
}

/** ooSQLiteConnection::lastErrMsg  [attribute get]
 */
RexxMethod1(RexxStringObject, oosqlconn_getLastErrMsg_atr, CSELF, pCSelf)
{
    pCooSQLiteConn pConn = requiredDBCSelf(context, pCSelf);
    if ( pConn == NULL )
    {
        return NULLOBJECT;
    }
    return pConn->lastErrMsg;
}

/** ooSQLiteConnection::recordFormat  [attribute get]
 */
RexxMethod1(uint32_t, oosqlconn_getRecordFormat_atr, CSELF, pCSelf)
{
    pCooSQLiteConn pConn = requiredDBCSelf(context, pCSelf);
    if ( pConn == NULL )
    {
        return 0;
    }
    return pConn->format;
}
/** ooSQLite::recordFormat  [attribute set]
 */
RexxMethod2(RexxObjectPtr, oosqlconn_setRecordFormat_atr, uint32_t, format, CSELF, pCSelf)
{
    pCooSQLiteConn pConn = requiredDBCSelf(context, pCSelf);
    if ( pConn == NULL )
    {
        return NULLOBJECT;
    }
    else if ( format < anArrayOfArrays || format > aClassicStem )
    {
        wrongArgValueException(context->threadContext, 1, RECORD_FORMATS_LIST, context->UnsignedInt32(format));
    }
    else
    {
        pConn->format = (ResultSetType)format;
    }
    return NULLOBJECT;
}


/** ooSQLiteConnection::null  [attribute get]
 */
RexxMethod1(RexxObjectPtr, oosqlconn_getNull_atr, CSELF, pCSelf)
{
    pCooSQLiteConn pConn = requiredDBCSelf(context, pCSelf);
    if ( pConn == NULL )
    {
        return TheZeroObj;
    }
    return pConn->nullObj;
}
/** ooSQLiteConnection::null  [attribute set]
 *
 *  @note  The default value to use for SQL NULL, for this connection, is first
 *         set in the init() method.  That used the global default value from
 *         the ooSQLite class.  If the global default value is a Rexx string, we
 *         create a new Rexx string object and request a global reference.
 *
 *         If the user changes the attribute we release the global reference for
 *         the existing, (if it is not the nil object,) and create a new Rexx
 *         string if needed.
 *
 *         This same pattern is followed for a new statement object, which is
 *         why we save the CSTRING value of the attribute here.
 */
RexxMethod2(RexxObjectPtr, oosqlconn_setNull_atr, RexxObjectPtr, nullObj, CSELF, pCSelf)
{
    pCooSQLiteConn pConn = requiredDBCSelf(context, pCSelf);
    if ( pConn == NULL )
    {
        return NULLOBJECT;
    }
    RexxObjectPtr  oldNull = pConn->nullObj;

    if ( nullObj == TheNilObj )
    {
        pConn->nullObj = nullObj;
        pConn->nullStr = NULL;
    }
    else
    {
        pConn->nullStr = context->ObjectToStringValue(nullObj);
        pConn->nullObj = context->String(pConn->nullStr);

        context->RequestGlobalReference(pConn->nullObj);
    }

    if ( oldNull != TheNilObj )
    {
        context->ReleaseGlobalReference(oldNull);
    }

    return NULLOBJECT;
}


/** ooSQLiteConnection::init()
 *
 *  @param file     [required]   Name of the database for this sqlite database
 *                               connection.
 *
 *  @param flags    [optional]   Flags for opening the database.  Use the
 *                               .ooSQLiteConstants constants.
 *
 *  @param defFormat [optional]  An OO constant defining the default format for
 *                               a record set returned.  The default if this
 *                               argument is omitted is the process default
 *                               format, (.ooSQLite~recordFormat.)
 *
 *  @param vfsName  [optional]   The name of the sqlite3_vfs object that defines
 *                               the operating system interface that the new
 *                               database connection should use.  We don't check
 *                               this yet. so the sqlitee default is always
 *                               used.
 */
RexxMethod5(RexxObjectPtr, oosqlconn_init, CSTRING, file, OPTIONAL_int32_t, _flags, OPTIONAL_uint32_t, defFormat,
            OPTIONAL_CSTRING, vfsName, OSELF, self)
{

    // Get a buffer for the ooSQLiteConnection CSelf.
    RexxBufferObject cselfBuffer = context->NewBuffer(sizeof(CooSQLiteConn));
    if ( cselfBuffer == NULLOBJECT )
    {
        outOfMemoryException(context->threadContext);
        return NULLOBJECT;
    }

    context->SetObjectVariable("CSELF", cselfBuffer);

    pCooSQLiteConn pConn = (pCooSQLiteConn)context->BufferData(cselfBuffer);
    memset(pConn, 0, sizeof(CooSQLiteConn));

    int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    if ( argumentExists(2) )
    {
        flags = _flags;
    }

    setDBInitStatusFile(context, pConn, file);

    int rc = sqlite3_open_v2(file, &pConn->db, flags, NULL);

    bool success = setDBInitStatus(context, pConn, pConn->db, rc, self);

    if ( success )
    {
        pCooSQLiteClass pCsc = (pCooSQLiteClass)context->ObjectToCSelf(TheOOSQLiteClass);

        if ( argumentExists(3) )
        {
            if ( defFormat < anArrayOfArrays || defFormat > aClassicStem )
            {
                wrongArgValueException(context->threadContext, 3, RECORD_FORMATS_LIST,
                                       context->UnsignedInt32(defFormat));
            }
            else
            {
                pConn->format = (ResultSetType)defFormat;
            }
        }
        else
        {
            pConn->format = pCsc->format;
        }

        if ( pCsc->nullObj == TheNilObj )
        {
            pConn->nullObj = pCsc->nullObj;
        }
        else
        {
            pConn->nullStr = pCsc->nullStr;
            pConn->nullObj = context->String(pConn->nullStr);

            context->RequestGlobalReference(pConn->nullObj);
        }
    }

    return NULLOBJECT;
}

/** ooSQLiteConnection::uninit()
 *
 *  At this point, the only purpose of uninit() is to ensure that an open
 *  database connection is closed.  There is no allocated memory in the
 *  CooSQLiteConn struct that needs to be freed, which could change in the future.
 *
 *  If we attempt to close the connection and get the busy return, we call
 *  interrupt().  The SQLite doc makes this sound a little dangerous if the
 *  database should close on another thread before the call to interrupt
 *  returns, something to keep in mind.
 */
RexxMethod1(RexxObjectPtr, oosqlconn_uninit, CSELF, pCSelf)
{
    if ( pCSelf != NULLOBJECT )
    {
        pCooSQLiteConn pConn = (pCooSQLiteConn)pCSelf;

#if OOSQLDBG == 1
        printf("ooSQLiteConnection::uninit() database connection=%p closed=%d\n", pConn->db, pConn->closed);
#endif
        if ( ! pConn->closed && pConn->db != NULL )
        {
            cleanupCallbacks(context);

            ensureFinalized(context, pConn);

            if ( pConn->nullObj != TheNilObj )
            {
                context->ReleaseGlobalReference(pConn->nullObj);
            }

            int rc = sqlite3_close(pConn->db);

            if ( rc == SQLITE_OK )
            {
#if OOSQLDBG == 1
                printf("ooSQLiteConnection::uninit() did close database first try\n");
#endif

                pConn->db = NULL;
                pConn->closed = true;
                return TheZeroObj;
            }

            // Not sure what to do if rc != SQLITE_BUSY ...
            if ( rc == SQLITE_BUSY )
            {
                sqlite3_interrupt(pConn->db);

                size_t i = 0;
                do
                {
                    sqlite3_sleep(100);
                    rc = sqlite3_close(pConn->db);
                } while ( rc == SQLITE_BUSY && ++i <= 10);
            }

            if ( rc == SQLITE_OK )
            {
                pConn->db = NULL;
                pConn->closed = true;
            }
#if OOSQLDBG == 1
            else
            {
                printf("ooSQLiteConnection::uninit() database connection not closed successfully. rc=%d\n", rc);
            }
#endif
        }
    }

    return TheZeroObj;
}


/** ooSQLiteConnection::busyHandler()
 *
 *  Installs a user defined busy handler.
 *
 *  @param  callbackObj  [required]  An instantiated object with a method that
 *                       may be invoked whenever an attempt is made to open a
 *                       database table that another thread or process has
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
 *          that invoking ooSQLiteConnection::busyTimeOut() can also set or
 *          clear the busy handler.
 *
 *          The busy handler should not take any actions which modify the
 *          database connection that invoked the busy handler. Any such actions
 *          result in undefined behavior.
 *
 *          A busy handler must not close the database connection or prepared
 *          statement that invoked the busy handler.
 */
RexxMethod4(RexxObjectPtr, oosqlconn_busyHandler, RexxObjectPtr, callbackObj, OPTIONAL_CSTRING, mthName,
            OPTIONAL_RexxObjectPtr, userData, CSELF, pCSelf)
{
    pCooSQLiteConn pConn = requiredDB(context, pCSelf);
    if ( pConn == NULL )
    {
        return context->WholeNumber(SQLITE_MISUSE);
    }

    RexxObjectPtr rc = doCallbackSetup(context, pConn, callbackObj, mthName, userData, busyHandler, 0, NULL);

    if ( objIsSqliteOK(context, rc) )
    {
        pConn->hasBusyHandler = (callbackObj == TheNilObj) ? false : true;
    }

    return rc;
}

/** ooSQLiteConnection::busyTimeOut()
 *
 *  @remarks sqlite3_busy_timeout() installs a SQLite timeout handler.  This has
 *           the side effect of removing any existing handler, which could have
 *           been a user registered handler.  So, we need to clean up a possible
 *           user registered handler.
 *
 *           It is safe to call cleanupCallback() even if there has not been a
 *           user registered handler.  We will attempt to get the generic
 *           callback buffer from this ooSQLiteConnection object's context
 *           variable and get back NULLOBJECT.  There is a check for NULLOBJECT
 *           that will prevent us from crashing.
 */
RexxMethod2(int, oosqlconn_busyTimeOut, int, ms, CSELF, pCSelf)
{
    pCooSQLiteConn pConn = requiredDB(context, pCSelf);
    if ( pConn == NULL )
    {
        return SQLITE_MISUSE;
    }

    int rc = sqlite3_busy_timeout(pConn->db, ms);
    cleanupCallback(context, busyHandler, NULL);

    pConn->hasBusyHandler = (ms > 0) ? true : false;

    return rc;
}


/** ooSQLiteConnection::changes()
 *
 *
 */
RexxMethod1(int, oosqlconn_changes, CSELF, pCSelf)
{
    pCooSQLiteConn pConn = requiredDB(context, pCSelf);
    if ( pConn == NULL )
    {
        return SQLITE_MISUSE;
    }

    return sqlite3_changes(pConn->db);
}


/** ooSQLiteConnection::close()
 *
 *  Closes the database connection for this ooSQLiteConnection object.
 *
 *
 *  @remarks  The SQLite doc says that the sqlite3 pointer can be null or a
 *            pointer that has not been previously closed.  Calling close with a
 *            NULL is a harmless no-op.  We want to maintain that behaviour, so
 *            we don't call it an error if the user calls close when the
 *            connection has already been closed.
 */
RexxMethod1(int, oosqlconn_close, CSELF, pCSelf)
{
    int rc = SQLITE_MISUSE;

    pCooSQLiteConn pConn = requiredDBCSelf(context, pCSelf);
    if ( pConn != NULL )
    {
        if ( pConn->isDestinationBU )
        {
            invalidDbStateException(context, pConn, 0);
            return rc;
        }

        cleanupCallbacks(context);

        rc = sqlite3_close(pConn->db);

        if ( rc == SQLITE_OK )
        {
            pConn->db     = NULL;
            pConn->closed = true;
        }
#if OOSQLDBG == 1
        else if ( rc == SQLITE_BUSY )
        {
            printf("Database busy. Sleep here or let user deal with this??\n");
        }
#endif
    }

    return rc;
}


/** ooSQLiteConnection::collationNeeded()
 *
 *  Registers a callback method to be invoked whenever whenever an undefined
 *  collation sequence is required.
 *
 *  @param  callbackObj  [required]  An instantiated class object with a method
 *                       that is invoked whenever an undefined collation is
 *                       needed.
 *
 *                       However, this argument can also be .nil to indicate
 *                       that an already registered callback is to be removed.
 *
 *  @param  mthName      [optional]  The method name that will be invoked during
 *                       the callback.  By default, the method invoked will be
 *                       collationNeededCallback().  However, the user can
 *                       specify an alternative method if desired.  This
 *                       argument is ignored when the callbackObj argument is
 *                       .nil.
 *
 *  @param userData      [optional] This can be any Rexx object the user
 *                       desires. The object will be sent as the secon argument
 *                       to the callback method when it is invoked. This
 *                       argument is ignored when  the callbackObj argument is
 *                       .nil.
 *
 *  @return  An ooSQLite result code.
 *
 *  @notes  When the callback is invoked, the method should register the desired
 *          collation using createCollation().
 *
 *          The callback method is sent 3 arguments, the Rexx database
 *          connection object, the name of the collation, and the userData
 *          object, or .nil if no userData object was specified.
 */
RexxMethod4(RexxObjectPtr, oosqlconn_collationNeeded, RexxObjectPtr, callbackObj, OPTIONAL_CSTRING, mthName,
            OPTIONAL_RexxObjectPtr, userData, CSELF, pCSelf)
{
    pCooSQLiteConn pConn = requiredDB(context, pCSelf);
    if ( pConn == NULL )
    {
        return context->WholeNumber(SQLITE_MISUSE);
    }

    return doCallbackSetup(context, pConn, callbackObj, mthName, userData, collationNeeded, 0, NULL);
}


/** ooSQLiteConnection::commitHook()
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
RexxMethod4(RexxObjectPtr, oosqlconn_commitHook, RexxObjectPtr, callbackObj, OPTIONAL_CSTRING, mthName,
            OPTIONAL_RexxObjectPtr, userData, CSELF, pCSelf)
{
    pCooSQLiteConn pConn = requiredDB(context, pCSelf);
    if ( pConn == NULL )
    {
        return context->WholeNumber(SQLITE_MISUSE);
    }

    return doCallbackSetup(context, pConn, callbackObj, mthName, userData, commitHook, 0, NULL);
}


/** ooSQLiteConnection::createCollation()
 *
 *  Adds, removes, or modifies a collation associated with this database
 *  connection. A collation defines how two strings are compared.  SQLite uses
 *  the collation whenever it needs to compare two strings in the database.
 *
 *  @param  collationName [required] The collation name.  This is the name that
 *                        identifies the collation to SQLite.
 *
 *  @param  callbackObj   [required] Identifies where 'callBackName' comes from.
 *
 *                        If this is an instantiated Rexx object, then
 *                        'callBackName' is a method within the class of the
 *                        object.
 *
 *                        If this is a .Pointer object then it identifies an
 *                        already loaded library of external functions
 *
 *                        If this is a .Directory object then the NAME index of
 *                        the directory is the string name of an external
 *                        library that should be loaded, if it is not already
 *                        loaded.  The HANDLE index is used to return the
 *                        .Pointer object when the library is loaded
 *                        successfully.  If this method fails then the value for
 *                        the HANDLE index is undefined.
 *
 *  @param  callBackName  [optional]  Identifies the callback method or external
 *                        function that is registered with SQLite for the
 *                        collation.
 *
 *                        If 'callBackObj' is a Rexx object, then 'callBackName'
 *                        is name of the method to be invoked for the
 *                        coallation.  In this case only, if the argument is
 *                        omitted the default name for the method is
 *                        collationCallBack().
 *
 *                        If 'callBackObj' is a .Directory object or a .Pointer
 *                        object then a similar pattern is used as is used for
 *                        the 'callBackObj'.
 *
 *                        If it is a .Pointer object then it identifies and
 *                        already loaded external *function*.  Note that a
 *                        loaded library and a loaded function are two different
 *                        things.
 *
 *                        If 'callBackName' is a directory object, then the NAME
 *                        index identifies an external function to be loaded
 *                        from the external library identified by 'callBackObj'.
 *                        On success the HANDLE index is used to return the
 *                        .Pointer object for the external function.  If this
 *                        method fails, the value for the HANDLE index is
 *                        undefined.
 *
 *
 *  @param userData       [optional] This can be any Rexx object the user
 *                        desires.
 *
 *                        If the 'callBackObj' is a Rexx object, then this
 *                        object will be sent as the first argument to the
 *                        callback method.
 *
 *                        If the 'callBackObj is an external library, then this
 *                        is a little more complicated and is described in the
 *                        section on how to write an external collation
 *                        function.
 *
 *                        If, the string value of this object is exactly
 *                        "REMOVE", case does not matter, then the collation
 *                        callback identified by 'callBackObj' and
 *                        'callBackName' is removed.  Please note that "REMOVE
 *                        ", " REMOVE", and "REMOVE" are all not exactly the
 *                        same.
 *
 *  @return  ??
 *
 *  @notes
 */
RexxMethod5(RexxObjectPtr, oosqlconn_createCollation, CSTRING, collationName, RexxObjectPtr, callbackObj,
            OPTIONAL_RexxObjectPtr, callbackName, OPTIONAL_RexxObjectPtr, userData, CSELF, pCSelf)
{
    RexxObjectPtr result = context->WholeNumber(SQLITE_MISUSE);

    pCooSQLiteConn pConn = requiredDB(context, pCSelf);
    if ( pConn == NULL )
    {
        return result;
    }
    RexxMethodContext *c = context;

    bool isExternal      = true;
    bool removeCollation = false;

    if ( c->IsOfType(callbackObj, "POINTER") )
    {
        c->RaiseException0(Rexx_Error_Unsupported_method);
        return result;
    }
    else if ( c->IsDirectory(callbackObj) )
    {
        c->RaiseException0(Rexx_Error_Unsupported_method);
        return result;
    }
    else
    {
        isExternal = false;
    }

    // Determine if we are removing this collation.
    if ( argumentExists(4) )
    {
        CSTRING str = c->ObjectToStringValue(userData);
        if ( sqlite3_stricmp(str, "REMOVE") == 0 )
        {
            removeCollation = true;
        }
    }

    if ( isExternal )
    {
        if ( argumentOmitted(3) )
        {
            c->RaiseException1(Rexx_Error_Incorrect_method_noarg, c->WholeNumber(3));
            return result;
        }

        if ( c->IsOfType(callbackName, "POINTER") )
        {
            c->RaiseException0(Rexx_Error_Unsupported_method);
            return result;
        }
        else if ( c->IsDirectory(callbackName) )
        {
            c->RaiseException0(Rexx_Error_Unsupported_method);
            return result;
        }
        else
        {
            wrongArgValueException(c->threadContext, 3, "Pointer or Directory", callbackName);
            return result;
        }

        // Here do set up for external collation function
        // sqlite3_create_collation.  Until implemented we just return misuse
        return result;
    }

    // Okay we are installing a Rexx callback.  If we are removing, we do not
    // care what the method name is, otherwise we need to get it from the
    // object, if arg 3 exists.
    CSTRING mthName = NULL;

    if ( removeCollation )
    {
        // Will cause doCallbackSetup() to remove the collation.
        callbackObj = TheNilObj;
    }
    if ( argumentExists(3) )
    {
        mthName = c->ObjectToStringValue(callbackName);
    }

    result = doCallbackSetup(context, pConn, callbackObj, mthName, userData, collation, 0, collationName);

    return result;
}


/** ooSQLiteConnection::dbFileName()
 *
 *
 */
RexxMethod2(RexxObjectPtr, oosqlconn_dbFileName, CSTRING, name, CSELF, pCSelf)
{
    pCooSQLiteConn pConn = requiredDB(context, pCSelf);
    if ( pConn == NULL )
    {
        return context->WholeNumber(SQLITE_MISUSE);
    }

    return safeRexxStringRx(context->threadContext, sqlite3_db_filename(pConn->db, name));
}


/** ooSQLiteConnection::dbMutex()
 *
 *
 *
 */
RexxMethod1(RexxObjectPtr, oosqlconn_dbMutex, CSELF, pCSelf)
{
    pCooSQLiteConn pConn = requiredDB(context, pCSelf);
    if ( pConn == NULL )
    {
        return context->WholeNumber(SQLITE_MISUSE);
    }

    sqlite3_mutex *mtx = sqlite3_db_mutex(pConn->db);

    // mtx could be null, but we just let the ooSQLiteMutex class take care of it.
    RexxArrayObject args = context->ArrayOfThree(context->WholeNumber(SQLITE_MUTEX_RECURSIVE_DB),
                                                 context->NewPointer((POINTER)MutexMagic),
                                                 context->NewPointer((POINTER)mtx));

    return context->SendMessage(TheOOSQLiteMutexClass, "NEW", args);
}


/** ooSQLiteConnection::dbReadOnly()
 *
 *
 */
RexxMethod2(int, oosqlconn_dbReadOnly, CSTRING, name, CSELF, pCSelf)
{
    pCooSQLiteConn pConn = requiredDB(context, pCSelf);
    if ( pConn == NULL )
    {
        return SQLITE_MISUSE;
    }

    return sqlite3_db_readonly(pConn->db, name);
}


/** ooSQLiteConnection::dbReleaseMemory()
 *
 *
 *
 */
RexxMethod1(int, oosqlconn_dbReleaseMemory, CSELF, pCSelf)
{
    pCooSQLiteConn pConn = requiredDB(context, pCSelf);
    if ( pConn == NULL )
    {
        return SQLITE_MISUSE;
    }

    return sqlite3_db_release_memory(pConn->db);
}


/** ooSQLiteConnection::dbStatus()
 *
 *
 *
 */
RexxMethod4(int, oosqlconn_dbStatus, int, param, RexxObjectPtr, _result, OPTIONAL_logical_t, reset, CSELF, pCSelf)
{
    pCooSQLiteConn pConn = requiredDB(context, pCSelf);
    if ( pConn == NULL )
    {
        return SQLITE_MISUSE;
    }

    return genDbStatus(context->threadContext, pConn->db, param, _result, reset, 2);
}


/** ooSQLiteConnection::enableLoadExtension()
 *
 *
 */
RexxMethod2(int, oosqlconn_enableLoadExtension, OPTIONAL_logical_t, on, CSELF, pCSelf)
{
    pCooSQLiteConn pConn = requiredDB(context, pCSelf);
    if ( pConn == NULL )
    {
        return SQLITE_MISUSE;
    }

    return sqlite3_enable_load_extension(pConn->db, on ? 1 : 0);
}

/** ooSQLiteConnection::errCode()
 *  ooSQLiteConnection::extendedErrCode()
 *
 *
 */
RexxMethod1(int, oosqlconn_errCode, CSELF, pCSelf)
{
    pCooSQLiteConn pConn = requiredDB(context, pCSelf);
    if ( pConn == NULL )
    {
        return SQLITE_MISUSE;
    }

    return sqlite3_extended_errcode(pConn->db);
}


/** ooSQLiteConnection::errMsg()
 *
 *
 */
RexxMethod1(RexxObjectPtr, oosqlconn_errMsg, CSELF, pCSelf)
{
    pCooSQLiteConn pConn = requiredDB(context, pCSelf);
    if ( pConn == NULL )
    {
        return context->WholeNumber(SQLITE_MISUSE);
    }

    return context->String(sqlite3_errmsg(pConn->db));
}


/** ooSQLiteConnection::exec()
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
 *                       code. The defualt if this argument is omitted is false.
 *
 *                       If true *and* the callBackObj arg is omitted then an
 *                       internal callback of the ooSQLite framework is used and
 *                       a result set is returned.  The result set could be
 *                       empty if the sql does not produce a result set.  The
 *                       format for the returned result set will the format
 *                       specified by the recordFomat attribute of this database
 *                       connection. However, the default format can be
 *                       overridden for this invocation of exec() through the
 *                       optional format argument.
 *
 *                       Otherwise, if callBackObj is not omitted, then the call
 *                       back method of that object is invoked for each result
 *                       row coming out of the evaluated SQL statements.  In
 *                       this case the return is a return code
 *
 *  @param  format       [optional]  Specifies the format of the returned result
 *                       set or single record.  If this argument is omitted, the
 *                       format specified by the recordFomat attribute of this
 *                       database connection is used.  This argument is ignored
 *                       if doCallback is false.
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
 *  @return  A result code, or a result set as described above.
 *
 *  @note  If the user requests a result set be returned, then we always return
 *         the result set, even if there was an error.  On error, the
 *         lastErrCode and lastErrMsg get set.  If there was no error, they are
 *         cleared.  This way the user can determine if there was an error, but
 *         still get a partial result set (if a partial result set was created.)
 *
 *         In addition, if the user specifies a callback object they could end
 *         the exec processing once they got all the data they needed.
 *
 *         The exec() callback is always running on this thread, so the thread
 *         context for this method is always valid.  Contrast this with some of
 *         the other callbacks like trace(), commitHook(), etc., where the
 *         callback could end up running under a different thread.  We do not
 *         need to worry about the thread context for the exec() method.
 */
RexxMethod7(RexxObjectPtr, oosqlconn_exec, CSTRING, sql, OPTIONAL_logical_t, doCallback,
            OPTIONAL_uint32_t, format, OPTIONAL_RexxObjectPtr, callbackObj, OPTIONAL_CSTRING, mthName,
            OPTIONAL_RexxObjectPtr, userData, CSELF, pCSelf)
{
    int rc = SQLITE_ERROR;

    pCooSQLiteConn pConn = requiredDB(context, pCSelf);
    if ( pConn == NULL )
    {
        return context->WholeNumber(rc);
    }

    char             *errMsg = NULL;
    CGenericCallback  cbc    = {0};

    if ( doCallback )
    {
        if ( argumentExists(3) )
        {
            if ( format < anArrayOfArrays || format > aClassicStem )
            {
                wrongArgValueException(context->threadContext, 3, RECORD_FORMATS_LIST,
                                       context->UnsignedInt32(format));
                return NULLOBJECT;
            }
            else
            {
                cbc.format = (ResultSetType)format;
            }
        }
        else
        {
            cbc.format = pConn->format;
        }

        cbc.callbackContext = context->threadContext;
        cbc.nullObj         = pConn->nullObj;

        if ( argumentExists(4) )
        {
            cbc.callbackObj = callbackObj;

            if ( argumentExists(5) )
            {
                cbc.callbackMethod = mthName;
            }
            else
            {
                cbc.callbackMethod = "EXECCALLBACK"; // execCallback()
            }

            if ( argumentExists(6) )
            {
                cbc.userData = userData;
            }
        }
        else
        {
            cbc.createRS = true;

            if ( cbc.format == aStemOfStems || cbc.format == aClassicStem )
            {
                RexxStemObject st = context->NewStem("records");
                context->SetStemArrayElement(st, 0, TheZeroObj);  // No records to start with.

                cbc.rsStem = st;
            }
            else
            {
                cbc.rsArray = context->NewArray(100);  // What is a good initial size?
            }
        }

        switch ( cbc.format )
        {
            case anArrayOfArrays :
                rc = sqlite3_exec(pConn->db, sql, execCallBackArray, (void *)&cbc, &errMsg);
                break;

            case anArrayOfDirectories :
                rc = sqlite3_exec(pConn->db, sql, execCallBackDirectory, (void *)&cbc, &errMsg);
                break;

            case aClassicStem :
                rc = sqlite3_exec(pConn->db, sql, execCallBackClassicStem, (void *)&cbc, &errMsg);
                break;

            default :
                rc = sqlite3_exec(pConn->db, sql, execCallBackStem, (void *)&cbc, &errMsg);
                break;
        }

        if ( cbc.indexes != NULL )
        {
            for ( uint32_t i = 0; i < cbc.countIndexes; i++ )
            {
                sqlite3_free(cbc.indexes[i]);
            }
            sqlite3_free(cbc.indexes);
        }
    }
    else
    {
        rc = sqlite3_exec(pConn->db, sql, NULL, NULL, &errMsg);
    }

    if ( rc != SQLITE_OK )
    {
        if ( errMsg != NULL )
        {
            ooSQLiteErr(context, pConn, rc, errMsg, true);
            sqlite3_free(errMsg);
        }
        else
        {
            ooSQLiteErr(context, pConn, rc, "error code, but sqlite3 did not set an error message", true);
        }
    }
    else
    {
        ooSQLiteNoErr(context, pConn);
    }

    if ( cbc.createRS )
    {
        if ( cbc.format == aStemOfStems || cbc.format == aClassicStem )
        {
            return cbc.rsStem;
        }
        else
        {
            return cbc.rsArray;
        }
    }

    return context->WholeNumber(rc);
}


/** ooSQLiteConnection::extendedResultCodes()
 *
 *  This is a noop, for the ooSQLite object orientated interface.  Extended
 *  result codes are always turned on in the init() method for each database
 *  connection.
 */
RexxMethod1(int, oosqlconn_extendedResultCodes, logical_t, onOff)
{
    return SQLITE_OK;
}


/** ooSQLiteConnection::getAutoCommit()
 *
 *
 */
RexxMethod1(logical_t, oosqlconn_getAutoCommit, CSELF, pCSelf)
{
    pCooSQLiteConn pConn = requiredDB(context, pCSelf);
    if ( pConn == NULL )
    {
        return SQLITE_MISUSE;
    }

    return sqlite3_get_autocommit(pConn->db);
}


/** ooSQLiteConnection::interrupt()
 *
 *
 */
RexxMethod1(int, oosqlconn_interrupt, CSELF, pCSelf)
{
    pCooSQLiteConn pConn = requiredDB(context, pCSelf);
    if ( pConn == NULL )
    {
        return SQLITE_MISUSE;
    }

    sqlite3_interrupt(pConn->db);

    return SQLITE_OK;
}


#ifdef SQLITE_HAS_CODEC
/** ooSQLiteConnection::key()
 *
 *  @param key      [required]   The encryption key to use.
 *
 *  @return  Returns one of the SQLite status code constants.
 */
RexxMethod2(int, oosqlconn_key, CSTRING, key, CSELF, pCSelf)
{
    pCooSQLiteConn pConn = requiredDB(context, pCSelf);
    if ( pConn == NULL )
    {
        return SQLITE_ERROR;
    }

    return sqlite3_key(pConn->db, key, (int)strlen(key));
}

#else

/** ooSQLiteConnection::key()
 *
 *  @param key      [required]   The encryption key to use.
 *
 *  @return  Returns one of the SQLite status code constants.
 */
RexxMethod2(int, oosqlconn_key, CSTRING, key, CSELF, pCSelf)
{
    pCooSQLiteConn pConn = requiredDB(context, pCSelf);
    if ( pConn == NULL )
    {
        return SQLITE_ERROR;
    }

    // Think we should raise an exception here.
    return SQLITE_ERROR;
}
#endif


/** ooSQLiteConnection::lastInsertRowID()
 *
 *
 */
RexxMethod1(int64_t, oosqlconn_lastInsertRowID, CSELF, pCSelf)
{
    pCooSQLiteConn pConn = requiredDB(context, pCSelf);
    if ( pConn == NULL )
    {
        return SQLITE_MISUSE;
    }

    return sqlite3_last_insert_rowid(pConn->db);
}


/** ooSQLiteConnection::limit()
 *
 *
 */
RexxMethod3(int, oosqlconn_limit, int, id, int, value, CSELF, pCSelf)
{
    pCooSQLiteConn pConn = requiredDB(context, pCSelf);
    if ( pConn == NULL )
    {
        return SQLITE_MISUSE;
    }

    return sqlite3_limit(pConn->db, id, value);
}


/** ooSQLiteConnection::loadExtension()
 *
 *
 *  @notes  If an error happens loading the extension, the lastErrMsg and
 *          lastErrCode attributes are set.
 */
RexxMethod3(int, oosqlconn_loadExtension, CSTRING, library, OPTIONAL_CSTRING, entryPoint, CSELF, pCSelf)
{
    pCooSQLiteConn pConn = requiredDB(context, pCSelf);
    if ( pConn == NULL )
    {
        return SQLITE_MISUSE;
    }

    char *errMsg = NULL;

    int rc = sqlite3_load_extension(pConn->db, library, entryPoint, &errMsg);
    if ( rc != SQLITE_OK && errMsg != NULL )
    {
        ooSQLiteErr(context, pConn, rc, errMsg, true);
        sqlite3_free(errMsg);
    }

    return rc;
}


/** ooSQLiteConnection::nextStmt()
 *
 *
 */
RexxMethod2(RexxObjectPtr, oosqlconn_nextStmt, OPTIONAL_RexxObjectPtr, _stmt, CSELF, pCSelf)
{
    pCooSQLiteConn pConn = requiredDB(context, pCSelf);
    if ( pConn == NULL )
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

    sqlite3_stmt *nextStmt = sqlite3_next_stmt(pConn->db, stmt);

    RexxObjectPtr result = TheNilObj;
    if ( nextStmt != NULL )
    {
        result = findStatement(context, pConn, nextStmt);
        if ( result == TheNilObj )
        {
            // This should never happen.  SQLite thinks there is a prepared
            // statement, but we don't.
            printf("Internal error.  Cound not find ooSQLiteStmt object for %p statement\n", stmt);
        }
    }

    return result;
}


/** ooSQLiteConnection::pragma()
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
RexxMethod3(RexxObjectPtr, oosqlconn_pragma, RexxStringObject, _name, OPTIONAL_RexxObjectPtr, value, CSELF, pCSelf)
{
    RexxObjectPtr result = ooSQLiteErr(context->threadContext, SQLITE_MISUSE, true);
    wholenumber_t rc     = SQLITE_ERROR;

    pCooSQLiteConn pConn = requiredDB(context, pCSelf);
    if ( pConn == NULL )
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
        case compileOptions  :
        case collationList   :
        case databaseList    :
        case foreignKeyCheck :
        case foreignKeyList  :
        case indexInfo       :
        case indexList       :
        case tableInfo       :
            return pragmaList(context, pConn, name, value, pragma);

        case integrityCheck :
        case quickCheck     :
        case shrinkMemory   :
        case walCheckpoint  :
            return pragmaTrigger(context, pConn, name, value, pragma);

        case caseSensitiveLike :
            if ( argumentOmitted(2) )
            {
                return missingArgException(context->threadContext, 2);
            }
            return pragmaSet(context, pConn, name, value, pragma);

        case incrementalVacuum :
            if (argumentOmitted(2) )
            {
                // SQLite allows the argument to be omitted entirely.  0
                // produces the same effect in SQLite as omitting the argument.
                value = TheZeroObj;
            }
            return pragmaSet(context, pConn, name, value, pragma);

        case busyTimeout :
            if ( argumentExists(2) )
            {
                return pragmaList(context, pConn, name, value, pragma);
            }
            else
            {
                return pragmaGet(context, pConn, name, pragma);
            }

        default :
            break;

    }

    if ( argumentExists(2) )
    {
        return pragmaSet(context, pConn, name, value, pragma);
    }
    else
    {
        return pragmaGet(context, pConn, name, pragma);
    }
}


/** ooSQLiteConnection::profile()
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
RexxMethod4(RexxObjectPtr, oosqlconn_profile, RexxObjectPtr, callbackObj, OPTIONAL_CSTRING, mthName,
            OPTIONAL_RexxObjectPtr, userData, CSELF, pCSelf)
{
    pCooSQLiteConn pConn = requiredDB(context, pCSelf);
    if ( pConn == NULL )
    {
        return context->WholeNumber(SQLITE_MISUSE);
    }

    return doCallbackSetup(context, pConn, callbackObj, mthName, userData, profileHook, 0, NULL);
}


/** ooSQLiteConnection::progressHandler()
 *
 *  Registers an user callback method that is invoked periodically during long
 *  running calls to ooSQLiteConnection::exec(), and ooSQLiteStmt::step() for this
 *  database connection. An example use for this interface is to keep a GUI
 *  updated during a large query.
 *
 *  @param  callbackObj  [required]  An instantiated object with a method that
 *                       is invoked periodically as described above.
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
RexxMethod5(RexxObjectPtr, oosqlconn_progressHandler, RexxObjectPtr, callbackObj, int, instructions,
            OPTIONAL_CSTRING, mthName, OPTIONAL_RexxObjectPtr, userData, CSELF, pCSelf)
{
    pCooSQLiteConn pConn = requiredDB(context, pCSelf);
    if ( pConn == NULL )
    {
        return context->WholeNumber(SQLITE_MISUSE);
    }

    if ( instructions < 1 )
    {
        callbackObj = TheNilObj;
    }

    return doCallbackSetup(context, pConn, callbackObj, mthName, userData, progressHandler, instructions, NULL);
}


#ifdef SQLITE_HAS_CODEC
/** ooSQLiteConnection::rekey()
 *
 *  @param key      [required]   The new encryption key to use.
 *
 *  @return  Returns one of the SQLite status code constants.
 */
RexxMethod2(int, oosqlconn_rekey, CSTRING, key, CSELF, pCSelf)
{
    pCooSQLiteConn pConn = requiredDB(context, pCSelf);
    if ( pConn == NULL )
    {
        return SQLITE_ERROR;
    }

    return sqlite3_rekey(pConn->db, key, (int)strlen(key));
}

#else

/** ooSQLiteConnection::rekey()
 *
 *  @param key      [required]   The new encryption key to use.
 *
 *  @return  Returns one of the SQLite status code constants.
 */
RexxMethod2(int, oosqlconn_rekey, CSTRING, key, CSELF, pCSelf)
{
    pCooSQLiteConn pConn = requiredDB(context, pCSelf);
    if ( pConn == NULL )
    {
        return SQLITE_ERROR;
    }

    return SQLITE_ERROR;
}
#endif


/** ooSQLiteConnection::rollbackHook()
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
RexxMethod4(RexxObjectPtr, oosqlconn_rollbackHook, RexxObjectPtr, callbackObj, OPTIONAL_CSTRING, mthName,
            OPTIONAL_RexxObjectPtr, userData, CSELF, pCSelf)
{
    pCooSQLiteConn pConn = requiredDB(context, pCSelf);
    if ( pConn == NULL )
    {
        return context->WholeNumber(SQLITE_MISUSE);
    }

    return doCallbackSetup(context, pConn, callbackObj, mthName, userData, rollbackHook, 0, NULL);
}


/** ooSQLiteConnection::setAuthorizer()
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
RexxMethod4(RexxObjectPtr, oosqlconn_setAuthorizer, RexxObjectPtr, callbackObj,
            OPTIONAL_CSTRING, mthName, OPTIONAL_RexxObjectPtr, userData, CSELF, pCSelf)
{
    pCooSQLiteConn pConn = requiredDB(context, pCSelf);
    if ( pConn == NULL )
    {
        return context->WholeNumber(SQLITE_MISUSE);
    }

    return doCallbackSetup(context, pConn, callbackObj, mthName, userData, authorizer, 0, NULL);
}


/** ooSQLiteConnection::tableColumnMetadata()
 *
 *
 *  @note The parameter order here is slightly switched from
 *        sqlite3_table_column_metadata() to put the opitonal dbName after
 *        results, so that optional arguments are placed at the end.
 */
RexxMethod5(int, oosqlconn_tableColumnMetadata, CSTRING, tableName, CSTRING, colName,
            RexxObjectPtr, results, OPTIONAL_CSTRING, dbName, CSELF, pCSelf)
{
    pCooSQLiteConn pConn = requiredDB(context, pCSelf);
    if ( pConn == NULL )
    {
        return SQLITE_MISUSE;
    }

    if ( argumentOmitted(4) || strlen(dbName) < 1 )
    {
        dbName = NULL;
    }

    return genTableColumnMetadata(context->threadContext, pConn->db, dbName, tableName, colName, results, 3);
}


/** ooSQLiteConnection::totalChanges()
 *
 *
 */
RexxMethod1(int, oosqlconn_totalChanges, CSELF, pCSelf)
{
    pCooSQLiteConn pConn = requiredDB(context, pCSelf);
    if ( pConn == NULL )
    {
        return SQLITE_MISUSE;
    }

    return sqlite3_total_changes(pConn->db);
}


/** ooSQLiteConnection::trace()
 *
 *  Registers an user callback method used for tracing.
 *
 *  @param  callbackObj  [required]  An instantiated object with a method that
 *                       is invoked at various times when an SQL statement is
 *                       being run by ooSQLiteStmt::step(). The callback is
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
 *          Setting a new trace callback automatically clears any previously
 *          installed callback.
 *
 *          The first argument sent to the callback is the original SQL
 *          statement text, or a comment identifying a trigger if the callback
 *          is invoked as the result of a trigger.
 *
 *          The second arugment is the userData argument to this method, if not
 *          omitted.  If userData is omitted then the third argument is .nil.
 */
RexxMethod4(RexxObjectPtr, oosqlconn_trace, RexxObjectPtr, callbackObj, OPTIONAL_CSTRING, mthName,
            OPTIONAL_RexxObjectPtr, userData, CSELF, pCSelf)
{
    pCooSQLiteConn pConn = requiredDB(context, pCSelf);
    if ( pConn == NULL )
    {
        return context->WholeNumber(SQLITE_MISUSE);
    }

    return doCallbackSetup(context, pConn, callbackObj, mthName, userData, traceHook, 0, NULL);
}


/** ooSQLiteConnection::updateHook()
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
RexxMethod4(RexxObjectPtr, oosqlconn_updateHook, RexxObjectPtr, callbackObj, OPTIONAL_CSTRING, mthName,
            OPTIONAL_RexxObjectPtr, userData, CSELF, pCSelf)
{
    pCooSQLiteConn pConn = requiredDB(context, pCSelf);
    if ( pConn == NULL )
    {
        return context->WholeNumber(SQLITE_MISUSE);
    }

    return doCallbackSetup(context, pConn, callbackObj, mthName, userData, updateHook, 0, NULL);
}


/**
 * Internal use only method.
 *
 */
RexxMethod2(RexxObjectPtr, oosqlconn_putStmt, RexxObjectPtr, stmt, CSELF, pCSelf)
{
    pCooSQLiteConn pConn = requiredDBCSelf(context, pCSelf);
    if ( pConn != NULL )
    {
        context->SendMessage1(pConn->stmtBag, "PUT", stmt);
    }

    return TheNilObj;
}

/**
 * Internal use only method.
 *
 */
RexxMethod2(RexxObjectPtr, oosqlconn_delStmt, RexxObjectPtr, stmt, CSELF, pCSelf)
{
    pCooSQLiteConn pConn = requiredDBCSelf(context, pCSelf);
    if ( pConn != NULLOBJECT )
    {
        return context->SendMessage1(pConn->stmtBag, "REMOVE", stmt);
    }

    return TheNilObj;
}

/**
 * Internal use only, transient testing function.
 *
 */
RexxMethod2(int, oosqlconn_test, POINTER, functPointer, CSELF, pCSelf)
{
    int rc = SQLITE_MISUSE;

    fnXCompare f = (fnXCompare)functPointer;

    pCooSQLiteConn pConn = requiredDBCSelf(context, pCSelf);
    if ( pConn != NULLOBJECT )
    {
        rc = sqlite3_create_collation_v2(pConn->db, "REVERSESORT", SQLITE_UTF8, NULL,
                                         f, NULL);
    }

    return rc;
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

/** ooSQLiteStmt::lastErrCode  [attribute get]
 *
 *  Note that lastErrMsg, lastErrCode, finalized and initCode attributes have to
 *  be accessible even if stmt is null.  So we just check for a NULL pCSelf, a
 *  null stmt is okay.
 */
RexxMethod1(int, oosqlstmt_getLastErrCode_atr, CSELF, pCSelf)
{
    pCooSQLiteStmt pCstmt = requiredStmtCSelf(context, pCSelf);
    if ( pCstmt == NULL )
    {
        return SQLITE_MISUSE;
    }
    return pCstmt->lastErrCode;
}

/** ooSQLiteStmt::lastErrMsg  [attribute get]
 *
 *  Note that lastErrMsg, lastErrCode, finalized and initCode attributes have to
 *  be accessible even if stmt is null.  So we just check for a NULL pCSelf, a
 *  null stmt is okay.
 */
RexxMethod1(RexxStringObject, oosqlstmt_getLastErrMsg_atr, CSELF, pCSelf)
{
    pCooSQLiteStmt pCstmt = requiredStmtCSelf(context, pCSelf);
    if ( pCstmt == NULL )
    {
        return NULLOBJECT;
    }
    return pCstmt->lastErrMsg;
}

/** ooSQLiteStmt::null  [attribute get]
 */
RexxMethod1(RexxObjectPtr, oosqlstmt_getNull_atr, CSELF, pCSelf)
{
    pCooSQLiteStmt pCstmt = requiredStmtCSelf(context, pCSelf);
    if ( pCstmt == NULL )
    {
        return NULLOBJECT;
    }
    return pCstmt->nullObj;
}
/** ooSQLiteStmt::null  [attribute set]
 */
RexxMethod2(RexxObjectPtr, oosqlstmt_setNull_atr, RexxObjectPtr, nullObj, CSELF, pCSelf)
{
    pCooSQLiteStmt pCstmt = requiredStmtCSelf(context, pCSelf);
    if ( pCstmt == NULL )
    {
        return NULLOBJECT;
    }
    RexxObjectPtr  oldNull = pCstmt->nullObj;

    if ( nullObj == TheNilObj )
    {
        pCstmt->nullObj = nullObj;
    }
    else
    {
        pCstmt->nullObj = context->String(context->ObjectToStringValue(nullObj));
        context->RequestGlobalReference(pCstmt->nullObj);
    }

    if ( oldNull != TheNilObj )
    {
        context->ReleaseGlobalReference(oldNull);
    }

    return NULLOBJECT;
}


/** ooSQLiteStmt::recordFormat  [attribute get]
 */
RexxMethod1(uint32_t, oosqlstmt_getRecordFormat_atr, CSELF, pCSelf)
{
    pCooSQLiteStmt pCstmt = requiredStmtCSelf(context, pCSelf);
    if ( pCstmt == NULL )
    {
        return 0;
    }
    return pCstmt->format;
}
/** ooSQLiteStmt::recordFormat  [attribute get]
 */
RexxMethod2(RexxObjectPtr, oosqlstmt_setRecordFormat_atr, uint32_t, format, CSELF, pCSelf)
{
    pCooSQLiteStmt pCstmt = requiredStmtCSelf(context, pCSelf);
    if ( pCstmt == NULL )
    {
        return NULLOBJECT;
    }
    else if ( format < anArrayOfArrays || format > aClassicStem )
    {
        wrongArgValueException(context->threadContext, 1, RECORD_FORMATS_LIST, context->UnsignedInt32(format));
    }
    else
    {
        pCstmt->format = (ResultSetType)format;
    }
    return NULLOBJECT;
}


/** ooSQLiteStmt::init()
 *
 *  @param db        [required]  The ooSQLiteConnection object the statement is
 *                               for.
 *
 *  @param sql       [required]  The SQL statement(s) used by this ooSQLiteStmt.
 *
 *  @param defFormat [optional]  An OO constant defining the default format for
 *                               a record set returned.  The default if this
 *                               argument is omitted is the default format of
 *                               the database connection.
 */
RexxMethod4(RexxObjectPtr, oosqlstmt_init, RexxObjectPtr, db, CSTRING, sql, OPTIONAL_uint32_t, defFormat, OSELF, self)
{
    // Get a buffer for the ooSQLiteStmt CSelf.
    RexxBufferObject cselfBuffer = context->NewBuffer(sizeof(CooSQLiteStmt));
    if ( cselfBuffer == NULLOBJECT )
    {
        return NULLOBJECT;
    }

    context->SetObjectVariable("CSELF", cselfBuffer);

    pCooSQLiteStmt pCstmt = (pCooSQLiteStmt)context->BufferData(cselfBuffer);
    memset(pCstmt, 0, sizeof(CooSQLiteStmt));

    pCooSQLiteConn pConn = requiredDBConnectionArg(context, db, 1);
    if ( pConn == NULL )
    {
        return NULLOBJECT;
    }

    if ( argumentExists(3) )
    {
        if ( defFormat < anArrayOfArrays || defFormat > aClassicStem )
        {
            wrongArgValueException(context->threadContext, 3, RECORD_FORMATS_LIST,
                                   context->UnsignedInt32(defFormat));
            return NULLOBJECT;
        }
        else
        {
            pCstmt->format = (ResultSetType)defFormat;
        }
    }
    else
    {
        pCstmt->format = pConn->format;
    }

    CSTRING msg  = "not an error";
    CSTRING tail = NULL;

    int rc = sqlite3_prepare_v2(pConn->db, sql, (int)strlen(sql) + 1, &pCstmt->stmt, &tail);

    if ( rc == SQLITE_OK )
    {
        pCstmt->db       = db;
        pCstmt->pConn    = pConn;
        pCstmt->rexxSelf = self;
        pCstmt->tail     = safeRexxStringRx(context->threadContext, tail);

        if ( pConn->nullObj == TheNilObj )
        {
            pCstmt->nullObj = TheNilObj;
        }
        else
        {
            pCstmt->nullObj = context->String(pConn->nullStr);
            context->RequestGlobalReference(pCstmt->nullObj);
        }

        context->SetObjectVariable("__rxTail", pCstmt->tail);

        context->SendMessage1(db, "PUTSTMT", self);
    }
    else
    {
        msg = sqlite3_errmsg(pConn->db);
        if ( msg == NULL )
        {
            msg = "not available";
        }

        // pCstmt->stmt, pCstmt->db, etc., are all still null.  But, we need
        // finalized and initalizationerr to be true.
        pCstmt->finalized = true;
        pCstmt->initializationErr = true;
    }

    pCstmt->initCode    = rc;
    pCstmt->lastErrCode = rc;
    pCstmt->lastErrMsg  = context->String(msg);

    context->SetObjectVariable("__rxErrMsg", pCstmt->lastErrMsg);

    return NULLOBJECT;
}


RexxMethod1(RexxObjectPtr, oosqlstmt_uninit, CSELF, pCSelf)
{
    if ( pCSelf != NULLOBJECT )
    {
        pCooSQLiteStmt pCstmt = (pCooSQLiteStmt)pCSelf;

#if OOSQLDBG == 1
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

            if ( pCstmt->nullObj != TheNilObj )
            {
                context->ReleaseGlobalReference(pCstmt->nullObj);
            }
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


/** ooSQLiteStmt::columnIndex()
 *
 *  Returns the index of the column with the specified column name in the result
 *  set of a SELECT statement.
 *
 *  @param colName  [required]  The name of a column that is a column in a
 *                          select statement.
 *
 *  @return  Returns the 1 based column index, or 0 if there is no column with
 *           the specified name.
 *
 *  @note    The name comparison is case insensitive.  I think this is correct,
 *           because as far as I can tell SQLite does not allow column names
 *           that differ only in case.  Not sure if this is SQLite specific or
 *           not.
 *
 *  @note    This is not method that maps to a SQLite API.  It is an ooSQLite
 *           enhancement.
 *
 *  @note    sqlite3_free() is a harmless nop for a null pointer.
 */
RexxMethod2(int, oosqlstmt_columnIndex, CSTRING, _colName, CSELF, pCSelf)
{
    pCooSQLiteStmt pCstmt = requiredStmt(context, pCSelf);
    if ( pCstmt == NULL )
    {
        return 0;
    }

    char *colName     = NULL;
    char *currentName = NULL;
    int   result      = 0;
    int   count       = 0;

    colName = strdupupr(_colName);
    if ( colName == NULL )
    {
        goto err_out;
    }

    count = sqlite3_column_count(pCstmt->stmt);
    if ( count == 0 )
    {
        // Not an error, statement may not be a SELECT.
        goto done_out;
    }

    for ( int i = 0; i < count; i++)
    {
        CSTRING _currentName = sqlite3_column_name(pCstmt->stmt, i);
        if ( _currentName == NULL )
        {
            goto err_out;
        }

        char *currentName = strdupupr(_currentName);
        if ( currentName == NULL )
        {
            goto err_out;
        }

        if ( strcmp(colName, currentName) == 0 )
        {
            result = i + 1;
            goto done_out;
        }

        sqlite3_free(currentName);
    }

    goto done_out;

err_out:
    outOfMemoryException(context->threadContext);

done_out:
    sqlite3_free(colName);
    sqlite3_free(currentName);
    return result;
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
    return safeColumnText(context->threadContext, pCstmt->stmt, col, pCstmt->nullObj);
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

    if ( handle != pCstmt->pConn->db )
    {
        printf("Internal consistency check fails.\nSQLite db handle (%p) does not match known db handle (%p)\n",
               handle, pCstmt->pConn->db);
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
 *  Methods for the .ooSQLiteBackup class.
 */
#define OOSQLITEBACKUP_CLASS    "ooSQLiteBackup"


/**
 * Checks if the destination database is in memory, and if so sets the page size
 * of the in memory database to the same page size as the destination database.
 *
 * This is a special purpose function called to protect against one of the
 * possible reasons for a backup to fail, backing up to an in memory database
 * with the wrong page size.
 *
 * When called for that purpose, the in memory database will be empty and its
 * page size can be changed to match the source database.
 *
 * @param dbDst
 * @param dbSrc
 * @param dstName
 *
 * @return bool
 *
 * @remarks  Rather than get the page size from both databases, we just get the
 *           destination database page size and unconditionally set the in
 *           memory database page size to that value.  Setting an in memory,
 *           empty, database page size should be very quick and error proof.
 *
 *           This should not fail, unless the engine can not get a lock on the
 *           source database to query its page size.  The Rexx user has to be
 *           the responsible party for the source database and should have set a
 *           busy handler.  If there is not a busy hancler, we'll try a couple
 *           of times if we get busy and then give up.
 *
 *           If this does fail, i.e., false is returned, then the error should
 *           be with the source database and dbSrc can be used to get the error
 *           message and code.
 */
bool buCheckPageSize(pCooSQLiteConn dstConn, pCooSQLiteConn srcConn, CSTRING dstName)
{
    if ( strcmp(dstName, ":memory:") )
    {
        return true;
    }

    sqlite3_stmt *stmt;
    int           srcSize = 0;

    int rc = sqlite3_prepare_v2(srcConn->db, "PRAGMA page_size;", -1, &stmt, NULL);
    if ( rc != SQLITE_OK )
    {
        return false;
    }

    rc = sqlite3_step(stmt);
    if ( rc == SQLITE_ROW )
    {
        srcSize = sqlite3_column_bytes(stmt, 0);
    }
    else if ( rc == SQLITE_BUSY && ! srcConn->hasBusyHandler)
    {
        for ( int i = 0; i < 4; i++ )
        {
            sqlite3_reset(stmt);
            sqlite3_sleep(250);

            rc = sqlite3_step(stmt);
            if ( rc == SQLITE_ROW )
            {
                srcSize = sqlite3_column_bytes(stmt, 0);
                break;
            }
            else if ( rc != SQLITE_BUSY )
            {
                break;
            }
        }
    }

    sqlite3_finalize(stmt);
    if ( srcSize == 0 )
    {
        return false;
    }

    char buf[256];
    snprintf(buf, sizeof(buf), "PRAGMA page_size = %d;", srcSize);

    // We're going to just assume this works.  The only reason it wouldn't, as
    // far as I can tell would be that the SQL was malformed.  Since it is hard
    // coded, once it is debugged, it should always be correct.
    sqlite3_prepare_v2(dstConn->db, buf, -1, &stmt, NULL);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return true;
}

/**
 * Takes care of the internal details of doing a finish.  This is used for both
 * the finish() method and when finish is done automatically for the user from
 * the step() method.
 *
 * @param c
 * @param pCbu
 *
 * @return int
 *
 * @remarks  Note that a global reference is held on the Rexx self of the
 *           destination database connection.  We can't release that reference
 *           here if saveDest is true.  Instead it must be released in uninit,
 *           or when the user requests it.
 *
 *           On the other hand, until the global reference is released we can be
 *           sure the destionation object, (and its CSelf,) is valid.
 */
int buFinish(RexxMethodContext *c, pCooSQLiteBackup pCbu)
{
    int rc = sqlite3_backup_finish(pCbu->backup);

    pCbu->backup      = NULL;
    pCbu->finished    = true;
    pCbu->lastErrMsg  = dbErrStringRx(c, pCbu->dstCSelf->db);
    pCbu->lastErrCode = dbErrCode(pCbu->dstCSelf->db);

    c->SetObjectVariable("__rxLastErrMsg", pCbu->lastErrMsg);

    CRITICAL_SECTION_ENTER

    pCbu->dstCSelf->isDestinationBU = false;

    CRITICAL_SECTION_LEAVE

    if ( pCbu->dstDbWasName && ! pCbu->saveDest )
    {
        c->SendMessage0(pCbu->dstRexxSelf, "CLOSE");

        c->ReleaseGlobalReference(pCbu->dstRexxSelf);
        pCbu->dstRexxSelf = NULLOBJECT;
    }

    c->ReleaseGlobalReference(pCbu->srcRexxSelf);

    return rc;
}


/**
 * Helper function for ooSQLiteBackup::init().  Takes care of the details of
 * getting the database connection CSelf for either the detination or source
 * databases for the backup.
 *
 * @param context
 * @param pCbu
 * @param rxDB
 * @param dbName
 * @param pos
 *
 * @return pCooSQLiteConn
 *
 * @remarks  We derive which database is called for, destionation or source,
 *           from the 'pos' argument.  The source database argument is the first
 *           arg.
 */
static pCooSQLiteConn buGetCSelfDB(RexxMethodContext *context, pCooSQLiteBackup pCbu, RexxObjectPtr rxDB,
                                   CSTRING *dbName, size_t pos)
{
    pCooSQLiteConn pConn = requiredDBConnectionArg(context, rxDB, pos);
    if ( pConn == NULL )
    {
        return NULL;
    }

    CSTRING errMsg;
    if ( pos == 1 )
    {
        errMsg = "The source database for the backup is in an error state";
        if ( argumentOmitted(4) )
        {
            *dbName = "main";
        }
    }
    else
    {
        errMsg = "The destination database for the backup is in an error state";
        if ( argumentOmitted(5) )
        {
            *dbName = "main";
        }
    }

    int dbRC = sqlite3_errcode(pConn->db);
    if ( dbRC != 0 )
    {
        pCbu->initializationErr = true;
        pCbu->finished          = true;
        pCbu->initCode          = OO_BACKUP_DB_ERRSTATE;
        pCbu->lastErrCode       = dbRC;
        pCbu->lastErrMsg        = context->String(errMsg);

        context->SetObjectVariable("__rxLastErrMsg", pCbu->lastErrMsg);
        return NULL;
    }

    return pConn;
}


/**
 * Sets the common failure state variables in the ooSQLiteBackup CSelf when
 * ooSQLiteBackup::init() fails.
 *
 * @param c
 * @param pCbu
 * @param msg
 * @param rc
 */
void buSetInitErr(RexxMethodContext *c, pCooSQLiteBackup pCbu, RexxStringObject msg, int rc)
{
    pCbu->initializationErr = true;
    pCbu->lastErrMsg        = msg;
    pCbu->lastErrCode       = rc;
    pCbu->initCode          = rc;
    pCbu->finished          = true;

    c->SetObjectVariable("__rxLastErrMsg", pCbu->lastErrMsg);
}


/** ooSQLiteBackup::finished  [attribute get]
 *
 *  Note that errMsg, finished, and initCode attributes have to be accessible
 *  even if the backup struct is null.  So we just check for a NULL pCSelf, a
 *  null backup pointer is okay.
 */
RexxMethod1(logical_t, oosqlbu_getFinished_atr, CSELF, pCSelf)
{
    pCooSQLiteBackup pCbu = requiredBuCSelf(context, pCSelf);
    if ( pCbu == NULL )
    {
        return 1;
    }
    return pCbu->finished;
}

/** ooSQLiteBackup::initCode  [attribute get]
 *
 *  Note that errMsg, finished, and initCode attributes have to be accessible
 *  even if the backup struct is null.  So we just check for a NULL pCSelf, a
 *  null backup pointer is okay.
 */
RexxMethod1(int, oosqlbu_getInitCode_atr, CSELF, pCSelf)
{
    pCooSQLiteBackup pCbu = requiredBuCSelf(context, pCSelf);
    if ( pCbu == NULL )
    {
        return -1;
    }
    return pCbu->initCode;
}

/** ooSQLiteBackup::lastErrCode  [attribute get]
 *
 *  Note that lastErrMsg, lastErrCode, finished, and initCode attributes have to
 *  be accessible even if the backup struct is null.  So we just check for a
 *  NULL pCSelf, a null backup pointer is okay.
 */
RexxMethod1(int, oosqlbu_getLastErrCode_atr, CSELF, pCSelf)
{
    pCooSQLiteBackup pCbu = requiredBuCSelf(context, pCSelf);
    if ( pCbu == NULL )
    {
        return SQLITE_MISUSE;
    }
    return pCbu->lastErrCode;
}

/** ooSQLiteBackup::lastErrMsg  [attribute get]
 *
 *  Note that lastErrMsg, lastErrCode, finished, and initCode attributes have to
 *  be accessible even if the backup struct is null.  So we just check for a
 *  NULL pCSelf, a null backup pointer is okay.
 */
RexxMethod1(RexxStringObject, oosqlbu_getLastErrMsg_atr, CSELF, pCSelf)
{
    pCooSQLiteBackup pCbu = requiredBuCSelf(context, pCSelf);
    if ( pCbu == NULL )
    {
        return NULLOBJECT;
    }
    return pCbu->lastErrMsg;
}

/** ooSQLiteBackup::pageCount  [attribute get]
 *
 *  The attribute can not be accessed after finish() has been invoked.
 */
RexxMethod1(int, oosqlbu_getPageCount_atr, CSELF, pCSelf)
{
    pCooSQLiteBackup pCbu = requiredBackup(context, pCSelf);
    if ( pCbu == NULL )
    {
        return SQLITE_MISUSE;
    }
    return sqlite3_backup_pagecount(pCbu->backup);
}

/** ooSQLiteBackup::remaining  [attribute get]
 *
 *  The attribute can not be accessed after finish() has been invoked.
 */
RexxMethod1(int, oosqlbu_getRemaining_atr, CSELF, pCSelf)
{
    pCooSQLiteBackup pCbu = requiredBackup(context, pCSelf);
    if ( pCbu == NULL )
    {
        return SQLITE_MISUSE;
    }
    return sqlite3_backup_remaining(pCbu->backup);
}

/** ooSQLiteBackup::saveDestConn  [attribute get]
 *
 */
RexxMethod1(logical_t, oosqlbu_getSaveDestConn_atr, CSELF, pCSelf)
{
    pCooSQLiteBackup pCbu = requiredBuCSelf(context, pCSelf);
    if ( pCbu == NULL )
    {
        return 0;
    }
    return pCbu->saveDest;
}
/** ooSQLiteBackup::saveDestConn  [attribute set]
 *
 *  We don't require that the backup be not finished here, although changing the
 *  value of the attribute will have no effect if the backup is finished.
 */
RexxMethod2(RexxObjectPtr, oosqlbu_setSaveDestConn_atr, logical_t, save, CSELF, pCSelf)
{
    pCooSQLiteBackup pCbu = requiredBuCSelf(context, pCSelf);
    if ( pCbu == NULL )
    {
        return NULLOBJECT;
    }
    pCbu->saveDest = save ? true : false;
    return NULLOBJECT;
}

/** ooSQLiteBackup::init()
 *
 *  @param srcDB   [required]  The source SQLite database connection object.
 *                             Must be opened already.
 *
 *  @param dstDB   [required]  Specifies the database to back up to.  This can
 *                             either be an .ooSQLiteConnection object, in which
 *                             case it must already be opened.  Or it can be the
 *                             database file name, in which case the
 *                             ooSQliteConnection object is instantiated.
 *
 *                             Normally, when this argument is a file name, the
 *                             connection object is closed during the finish()
 *                             method.  This behavior can be changed either by
 *                             setting the saveDestConn argument to true, or by
 *                             setting the saveDestConn attribute to true at any
 *                             time prior to invoking finish.  Both the argument
 *                             and the attribute default to false.
 *
 *  @param save    [optional]  If true, set the saveDestConn attributre to true.
 *                             Defaults to false.
 *
 *  @param srcName [optional]  The source database name.  This is not the
 *                             database file name, but rather the "main",
 *                             "temp", etc., name.  Defaults to "main"
 *
 *  @param dstName [optional]  The destinationdatabase name.  This is not the
 *                             database file name, but rather the "main",
 *                             "temp", etc., name.  Defaults to "main."  If the
 *                             destination DB is a file name this argument is
 *                             ignored because a new database connection is
 *                             created and this has to be "main".
 *
 *  @notes  The save argument is only checked when dstDB is a file name,
 *          otherwise it is completely ignored.
 *
 *          Both the source and destination databases can not be in an error
 *          state.  If either database has its last error code set,
 *          ooSQLiteBackup will not initialize the backup.  initCode will be set
 *          to OO_BACKUP_DB_ERRSTATE, lastErrMsg will specify which database is
 *          in the error state, and lastErrCode will be the database errCode.
 *
 *  @remarks  The SQLite doc says:
 *
 *  The application must guarantee that the destination database connection is
 *  not passed to any other API (by any thread) after sqlite3_backup_init() is
 *  called and before the corresponding call to sqlite3_backup_finish(). SQLite
 *  does not currently check to see if the application incorrectly accesses the
 *  destination database connection and so no error code is reported, but the
 *  operations may malfunction nevertheless. Use of the destination database
 *  connection while a backup is in progress might also also cause a mutex
 *  deadlock.
 *
 *  So - we set a flag in the CSelf of the destination database.  If this flag
 *  is set, invocation of any method of that database connection will abort with
 *  an error, OO_BACKUP_IN_PROGRESS.
 */
RexxMethod6(RexxObjectPtr, oosqlbu_init, RexxObjectPtr, srcDB, RexxObjectPtr, dstDB,
            OPTIONAL_logical_t, save, OPTIONAL_CSTRING, srcName, OPTIONAL_CSTRING, dstName, OSELF, self)
{
    // Get a buffer for the ooSQLiteBackup CSelf.
    RexxBufferObject cselfBuffer = context->NewBuffer(sizeof(CooSQLiteBackup));
    if ( cselfBuffer == NULLOBJECT )
    {
        outOfMemoryException(context->threadContext);
        return NULLOBJECT;
    }
    RexxMethodContext *c = context;
    context->SetObjectVariable("CSELF", cselfBuffer);

    pCooSQLiteBackup pCbu = (pCooSQLiteBackup)context->BufferData(cselfBuffer);
    memset(pCbu, 0, sizeof(CooSQLiteBackup));

    pCooSQLiteConn pConnSrc = buGetCSelfDB(context, pCbu, srcDB, &srcName, 1);
    if ( pConnSrc == NULL )
    {
        return NULLOBJECT;
    }
    pCooSQLiteConn pConnDst = NULL;

    if ( context->IsOfType(dstDB, OOSQLITECONNECTION_CLASS) )
    {
        pConnDst = buGetCSelfDB(context, pCbu, dstDB, &dstName, 5);
        if ( pConnDst == NULL )
        {
            return NULLOBJECT;
        }
    }
    else
    {
        CSTRING dstFileName = c->ObjectToStringValue(dstDB);
        pCbu->dstDbWasName  = true;
        dstName             = "main";

        RexxObjectPtr dstTemp = c->SendMessage1(TheOOSQLiteConnectionClass, "NEW", dstDB);

        if ( c->CheckCondition() )
        {
            // The only condition this could be is out of memory.  The user
            // should not trap this, nothing will work  ... so we don't set up
            // the backup CSelf with the error stuff.
            return NULLOBJECT;
        }

        if ( dstTemp == NULLOBJECT )
        {
            // I don't think this is possible, but we'll set up the error stuff.
            buSetInitErr(context, pCbu,
                         context->String("Message \"new\" did not return a result for the destionation database"),
                         OO_UNEXPECTED_RESULT);

            return NULLOBJECT;
        }

        pConnDst = dbToCSelf(context, dstTemp);

        // Highly unlikely the database was not opened, but we need to check ...
        if ( pConnDst->db == NULL )
        {
            buSetInitErr(context, pCbu, pConnDst->lastErrMsg, pConnDst->lastErrCode);
            return NULLOBJECT;
        }

        // We want a busy timout on the database connection we just instantiated.
        sqlite3_busy_timeout(pConnDst->db, 3000);
        pConnDst->hasBusyHandler = true;

        if ( ! buCheckPageSize(pConnDst, pConnSrc, dstFileName) )
        {
            buSetInitErr(context, pCbu, dbErrStringRx(context, pConnSrc->db), dbErrCode(pConnSrc->db));
            return NULLOBJECT;
        }

        pCbu->saveDest = save ? true : false;  // Will be false if omitted anyway.
    }

    CRITICAL_SECTION_ENTER

    pConnDst->isDestinationBU = true;

    CRITICAL_SECTION_LEAVE

    pCbu->backup = sqlite3_backup_init(pConnDst->db, dstName, pConnSrc->db, srcName);
    if ( pCbu->backup == NULL )
    {
        buSetInitErr(context, pCbu, dbErrStringRx(context, pConnDst->db), dbErrCode(pConnDst->db));

        CRITICAL_SECTION_ENTER

        pConnDst->isDestinationBU = false;

        CRITICAL_SECTION_LEAVE

        if ( pCbu->dstDbWasName )
        {
            context->SendMessage0(pConnDst->rexxSelf, "CLOSE");
        }
    }
    else
    {
        pCbu->dstCSelf = pConnDst;
        pCbu->srcCSelf = pConnSrc;

        pCbu->srcRexxSelf = c->RequestGlobalReference(pConnSrc->rexxSelf);
        pCbu->dstRexxSelf = c->RequestGlobalReference(pConnDst->rexxSelf);

        pCbu->lastErrMsg = context->String("not an error");
        context->SetObjectVariable("__rxErrMsg", pCbu->lastErrMsg);
    }

    return NULLOBJECT;
}


RexxMethod1(RexxObjectPtr, oosqlbu_uninit, CSELF, pCSelf)
{
    if ( pCSelf != NULLOBJECT )
    {
        pCooSQLiteBackup pCbu = (pCooSQLiteBackup)pCSelf;

#if OOSQLDBG == 1
        printf("ooSQLiteBackup::uninit() backup=%p finished=%d dstRexxSelf=%p\n",
               pCbu->backup, pCbu->finished, pCbu->dstRexxSelf);
#endif

        if ( pCbu->backup != NULL )
        {
            sqlite3_backup_finish(pCbu->backup);

            if ( ! pCbu->finished )
            {
                context->ReleaseGlobalReference(pCbu->dstRexxSelf);
                context->ReleaseGlobalReference(pCbu->srcRexxSelf);
            }

            pCbu->backup   = NULL;
            pCbu->finished = true;
        }
        else if ( pCbu->dstRexxSelf != NULLOBJECT )
        {
            context->ReleaseGlobalReference(pCbu->dstRexxSelf);
            pCbu->dstRexxSelf = NULLOBJECT;
        }
    }

    return TheZeroObj;
}


/** ooSQLiteBackup::finish()
 *
 */
RexxMethod1(int, oosqlbu_finish, CSELF, pCSelf)
{
    pCooSQLiteBackup pCbu = requiredBackup(context, pCSelf);
    if ( pCbu == NULL )
    {
        return SQLITE_MISUSE;
    }

    return buFinish(context, pCbu);
}


/** ooSQLiteBackup::getDestConn
 *
 *  Returns the saved destination database connection object, iff the
 *  destination database was specified as a file name *and* the user set the
 *  saveDestConn attribute to true, *and* the backup has been finished.
 *  Otherwise returns .nil.
 */
RexxMethod1(RexxObjectPtr, oosqlbu_getDestConn, CSELF, pCSelf)
{
    pCooSQLiteBackup pCbu = requiredBuCSelf(context, pCSelf);
    if ( pCbu == NULL )
    {
        return NULLOBJECT;
    }

    if ( pCbu->dstDbWasName && pCbu->finished && pCbu->saveDest )
    {
        return pCbu->dstRexxSelf;
    }
    return TheNilObj;
}

/** ooSQLiteBackup::step()
 *
 *  @param  pages  [optional]  The number of pages to copy.  If omitted, 5 pages
 *                             is used.
 *
 *  @return  The SQLITE result code.
 *
 *  @remarks  When SQLITE_DONE is returned, or an unrecoverable error code, then
 *            step() automatically invokes the finish() method for the user.
 *
 *            If OK, BUSY, or LOCKED is returned the user can retry the step. A
 *            syntax condition is rasied if the user invokes finish() when
 *            finish has already been invoked.  Because of this, the user only
 *            needs to / should only invoke finish() when she wants to abandon
 *            the backup.  If needed, the user can check the finished attribute
 *            to determine if the backup is still viable.
 *
 *            As far as the internal code here goes, it turns out that we do the
 *            same thing if the return from step is DONE, or some other fatal
 *            error.  So we don't check what the return code is.
 */
RexxMethod2(int, oosqlbu_step, OPTIONAL_int, pages, CSELF, pCSelf)
{
    pCooSQLiteBackup pCbu = requiredBackup(context, pCSelf);
    if ( pCbu == NULL )
    {
        return SQLITE_MISUSE;
    }

    int rc = sqlite3_backup_step(pCbu->backup, argumentOmitted(1) ? 5 : pages);

    if ( rc == SQLITE_OK || rc == SQLITE_BUSY || rc == SQLITE_LOCKED  )
    {
        pCbu->lastErrMsg  = dbErrStringRx(context, pCbu->dstCSelf->db);
        pCbu->lastErrCode = dbErrCode(pCbu->dstCSelf->db);

        context->SetObjectVariable("__rxLastErrMsg", pCbu->lastErrMsg);

        return rc;
    }
    else
    {
        buFinish(context, pCbu);
    }

    return rc;
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
 *  @param type  [optional]  The SQLite muxtex type.  Can only be:
 *               SQLITE_MUTEX_RECURSIVE or SQLITE_MUTEX_FAST, defaults to
 *               SQLITE_MUTEX_FAST.
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
RexxMethod4(RexxObjectPtr, oosqlmtx_init, OPTIONAL_int, type, OPTIONAL_POINTER, magic, OPTIONAL_POINTER, mtx, OSELF, self)
{
    bool isDbMutex = false;

    if ( argumentExists(2) || argumentExists(3) )
    {
        if ( argumentOmitted(3) || magic != (POINTER)MutexMagic )
        {
            return tooManyArgsException(context->threadContext, 1);
        }
        isDbMutex = true;
    }

    if ( ! isDbMutex )
    {
        // We assume if we called this internally we passed the right type.

        if ( argumentOmitted(1) )
        {
            type = SQLITE_MUTEX_FAST;
        }
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

#if OOSQLDBG == 1
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


/**
 *  Methods for the .ooSQLExtensions class.
 *
 *  The ooSQLExtensions class can load packages and load libraries.  It acts
 *  as a container to hold them, retrieve them, and unload / release them.
 */
#define OOSQLEXTENSIONS_CLASS     "ooSQLExtensions"
#define LAST_ERR_MSG_ATTRIBUTE    "extensionsLastErrMsgAttribute"
#define LAST_ERR_CODE_ATTRIBUTE   "extensionsLastErrCodeAttribute"

/**
 * Sets the last error attributes of the object whose method context we are
 * operating in.
 *
 * @param c
 * @param lastErrMsg   Error message.
 * @param lastErrCode  Error code.
 *
 * @note We use this as a generic function for the ooSQLExtensions,
 *       ooSQLPackage, and ooSQLLibrary classes.
 */
void extensionsSetLastErr(RexxMethodContext *c, RexxStringObject lastErrMsg, RexxObjectPtr lastErrCode)
{
    c->SetObjectVariable(LAST_ERR_MSG_ATTRIBUTE, lastErrMsg);
    c->SetObjectVariable(LAST_ERR_CODE_ATTRIBUTE, lastErrCode);
}

/**
 * Sets the last error attributes of the ooSQLExensions object back to the no
 * error state.
 *
 * @param c
 */
void resetExtensionsLastErr(RexxMethodContext *c, pCooSQLExtensions pcext)
{
    pcext->lastErrCode = TheZeroObj;
    pcext->lastErrMsg  = c->NullString();
    extensionsSetLastErr(c, c->NullString(), TheZeroObj);
}

/**
 * Sets the last error attributes of the ooSQLPackage object back to the no
 * error state.
 *
 * @param c
 */
void resetPackageLastErr(RexxMethodContext *c, pCooSQLPackage pcp)
{
    pcp->lastErrCode = TheZeroObj;
    pcp->lastErrMsg  = c->NullString();
    extensionsSetLastErr(c, c->NullString(), TheZeroObj);
}

/**
 * Sets the last error attributes of the ooSQLLibrary object back to the no
 * error state.
 *
 * @param c
 */
void resetLibraryLastErr(RexxMethodContext *c, pCooSQLLibrary pcl)
{
    pcl->lastErrCode = TheZeroObj;
    pcl->lastErrMsg  = c->NullString();
    extensionsSetLastErr(c, c->NullString(), TheZeroObj);
}

/**
 * Format a last error message and then set the last error message and last
 * error code attributes.
 *
 *
 * @param c
 * @param code
 * @param fmt
 * @param insert
 *
 * @note  We expect fmt to have exactly 1 %s in it.
 */
void extensionsFormatLastErr(RexxMethodContext *c, uint32_t code, char *fmt, char *insert)
{
    char buf[512];

    snprintf(buf, 512, fmt, insert);
    extensionsSetLastErr(c, c->String(buf), c->UnsignedInt32(code));
}

bool validPackageVersion(RexxMethodContext *c, pCooSQLPackage pcp)
{
    ooSQLitePackageEntry *pa = pcp->packageEntry;

    if ( OOSQLITE_CURRENT_VERSION < pa->reqOOSQLVersion || sqlite3_libversion_number() < pa->reqSQLiteVersion )
    {
        char buf[512];

        if ( OOSQLITE_CURRENT_VERSION < pa->reqOOSQLVersion )
        {
            snprintf(buf, 512, "Package: %s requires ooSQLite version: %d current version: %d",
                     pa->packageName, pa->reqOOSQLVersion, OOSQLITE_CURRENT_VERSION);
        }
        else
        {
            snprintf(buf, 512, "Package: %s requires SQLite version: %d current version: %d",
                     pa->packageName, pa->reqSQLiteVersion, sqlite3_libversion_number());
        }
        userDefinedMsgException(c->threadContext, buf);
        return false;
    }
    return true;
}

bool registerCollation(RexxMethodContext *c, pSQLiteCollationEntry e, pCooSQLiteConn pConn)
{
    if ( e->entryCompare == NULL )
    {
        char buf[256];

        snprintf(buf, 256, "the compare function for the collation: %s can not be null", e->name);
        userDefinedMsgException(c->threadContext, buf);
        return false;
    }

    void *userData = NULL;
    if ( e->entryGetUserData != NULL )
    {
        userData = e->entryGetUserData();
    }
    else if ( e->pUserData != NULL )
    {
        userData = e->pUserData;
    }

    int ret = sqlite3_create_collation_v2(pConn->db, e->name, SQLITE_UTF8, userData, e->entryCompare, e->entryDestroy);

    return ret == SQLITE_OK;
}

bool registerFunction(RexxMethodContext *c, pSQLiteFunctionEntry e, pCooSQLiteConn pConn)
{
    if ( e->entryFunc == NULL )
    {
        char buf[256];

        snprintf(buf, 256, "the function entry for the user defined function: %s can not be null", e->name);
        userDefinedMsgException(c->threadContext, buf);
        return false;
    }
    void *userData = NULL;
    if ( e->entryGetUserData != NULL )
    {
        userData = e->entryGetUserData();
    }
    else if ( e->pUserData != NULL )
    {
        userData = e->pUserData;
    }

    int ret = sqlite3_create_function_v2(pConn->db, e->name, e->countArgs, SQLITE_UTF8, userData, e->entryFunc,
                                         e->entryStep, e->entryFinal, e->entryDestroy);

    return ret == SQLITE_OK;
}

bool registerModule(RexxMethodContext *c, pSQLiteModuleEntry e, pCooSQLiteConn pConn)
{
    if ( e->moduleTab == NULL )
    {
        char buf[256];

        snprintf(buf, 256, "the module table for the user defined module: %s can not be null", e->name);
        userDefinedMsgException(c->threadContext, buf);
        return false;
    }
    void *userData = NULL;
    if ( e->entryGetUserData != NULL )
    {
        userData = e->entryGetUserData();
    }
    else if ( e->pUserData != NULL )
    {
        userData = e->pUserData;
    }

    int ret = sqlite3_create_module_v2(pConn->db, e->name, e->moduleTab, userData, e->entryDestroy);

    return ret == SQLITE_OK;
}

bool packageRegister(RexxMethodContext *c, pCooSQLPackage pcp, pCooSQLiteConn pConn)
{
    ooSQLitePackageEntry *pa = pcp->packageEntry;

    if ( pa->collations != NULL )
    {
        pSQLiteCollationEntry e = pa->collations;
        while ( e->name != NULL )
        {
            if ( ! registerCollation(c, e, pConn) )
            {
                return false;
            }
            e++;
        }
    }

    if ( pa->functions != NULL )
    {
        pSQLiteFunctionEntry e = pa->functions;
        while ( e->name != NULL )
        {
            if ( ! registerFunction(c, e, pConn) )
            {
                return false;
            }
            e++;
        }
    }

    if ( pa->modules != NULL )
    {
        pSQLiteModuleEntry e = pa->modules;
        while ( e->name != NULL )
        {
            if ( ! registerModule(c, e, pConn) )
            {
                return false;
            }
            e++;
        }
    }

    return true;
}

/** ooSQLExtensions::init()
 */
RexxMethod1(RexxObjectPtr, oosqlext_init_cls, OSELF, self)
{
    if ( isOfClassType(context, self, OOSQLEXTENSIONS_CLASS) )
    {
        RexxBufferObject cselfBuffer = context->NewBuffer(sizeof(CooSQLExtensions));
        if ( cselfBuffer == NULLOBJECT )
        {
            outOfMemoryException(context->threadContext);
            return NULLOBJECT;
        }

        context->SetObjectVariable("CSELF", cselfBuffer);

        pCooSQLExtensions pcext = (pCooSQLExtensions)context->BufferData(cselfBuffer);
        memset(pcext, 0, sizeof(CooSQLExtensions));

        pcext->libraryTable = rxNewBuiltinObject(context->threadContext, "TABLE");
        pcext->packageTable = rxNewBuiltinObject(context->threadContext, "TABLE");
        resetExtensionsLastErr(context, pcext);
    }
    return NULLOBJECT;
}


/** ooSQLExtensions::lastErrCode  [attribute get]
 */
RexxMethod1(RexxObjectPtr, oosqlext_getLastErrCode_atr, CSELF, pCSelf)
{
    pCooSQLExtensions pcext = (pCooSQLExtensions)pCSelf;
    return pcext->lastErrCode;
}

/** ooSQLExtensions::lastErrMsg  [attribute get]
 */
RexxMethod1(RexxStringObject, oosqlext_getLastErrMsg_atr, CSELF, pCSelf)
{
    pCooSQLExtensions pcext = (pCooSQLExtensions)pCSelf;
    return pcext->lastErrMsg;
}

/** ooSQLite::loadLibrary()  [class method]
 *
 *  Loads a shared library and optionally the extension functions in the
 *  library.
 *
 */
RexxMethod3(RexxObjectPtr, oosqlext_loadLibrary_cls, RexxObjectPtr, libName, OPTIONAL_RexxObjectPtr, procedures, CSELF, pCSelf)
{
    RexxMethodContext *c = context;
    pCooSQLExtensions pcext  = (pCooSQLExtensions)pCSelf;
    RexxObjectPtr     result = TheNilObj;

    RexxClassObject libCls = rxGetContextClass(context, "ooSQLLibrary");
    if ( libCls == NULLOBJECT )
    {
        goto done_out;
    }

    RexxObjectPtr library = c->SendMessage1(libCls, "NEW", libName);
    if ( library == NULLOBJECT )
    {
        goto done_out;
    }

    pCooSQLLibrary pcl = (pCooSQLLibrary)context->ObjectToCSelf(library);

    printf("oosqlext_loadLibrary_cls library CSelf=%p\n", pcl);
    if ( pcl == NULL )
    {
        printf("oosqlext_loadPackage_cls failed to get ooSQLLibrary CSelf\n");
        goto done_out;
    }

    if ( ! pcl->valid )
    {
        printf("oosqlext_loadPackage_cls ooSQLLibrary is not valid errCode=%d msg=%s\n",
               pcl->lib->getLastErrCode(), pcl->lib->getLastErrMsg());
        goto done_out;
    }


    CSTRING       procName      = NULL;

    if ( argumentExists(2) )
    {
        // check if it is an array.

        procName  = c->ObjectToStringValue(procedures);

        void *procedure = pcl->lib->getProcedure(procName);
        printf("procName=%s pointer=%p\n", procName, procedure);

        if ( procedure != NULL )
        {
            return c->NewPointer(procedure);
        }
    }

done_out:
    return result;
}

/** ooSQLite::loadPackage()  [class method]
 *
 *  Loads an ooSQLite package library and optionally regsiters the collations,
 *  functions, modules in the package.
 *
 *  ooSQLPackage:
 *
 *
 */
RexxMethod3(RexxObjectPtr, oosqlext_loadPackage_cls, CSTRING, libName, OPTIONAL_RexxObjectPtr, dbConn, CSELF, pCSelf)
{
    RexxMethodContext *c = context;
    RexxObjectPtr     result = TheFalseObj;
    pCooSQLExtensions pcext  = (pCooSQLExtensions)pCSelf;

    RexxClassObject packageCls = rxGetContextClass(context, "ooSQLPackage");
    if ( packageCls == NULLOBJECT )
    {
        goto done_out;
    }

    RexxObjectPtr package = context->SendMessage1(packageCls, "NEW", context->String(libName));
    if ( package == NULLOBJECT )
    {
        goto done_out;
    }

    pCooSQLPackage pcp = (pCooSQLPackage)c->ObjectToCSelf(package);
    if ( pcp == NULL )
    {
        extensionsFormatLastErr(context, OO_NO_CSELF, OO_NO_CSELF_STR, "ooSQLPackage");
        goto done_out;
    }

    if ( ! pcp->valid )
    {
        pcext->lastErrMsg  = pcp->lastErrMsg;
        pcext->lastErrCode = pcp->lastErrCode;

        extensionsSetLastErr(context, pcp->lastErrMsg, pcp->lastErrCode);
        goto done_out;
    }

    if ( ! validPackageVersion(context, pcp) )
    {
        goto done_out;
    }

    c->SendMessage2(pcext->packageTable, "PUT", package, c->String(pcp->packageEntry->packageName));
    result = TheTrueObj;

    // Check if we have a database connection to immediately register the functions
    if ( argumentExists(2) )
    {
        if ( ! c->IsOfType(dbConn, "ooSQliteConnection") )
        {
            wrongArgValueException(context->threadContext, 2, "ooSQliteConnection object", dbConn);
            goto done_out;
        }
        pCooSQLiteConn pConn = dbToCSelf(context,  dbConn);

        packageRegister(context, pcp, pConn);
    }

done_out:
    return result;
}


/**
 *  Methods for the .ooSQLPackage class.
 *
 *  An ooSQLite package is very similar to ooRexx Package.  an ooSQLitePackage
 *  package contains user defined collations, functions, and modules.  The
 *  external package file is built as a shared library containing collations,
 *  functionts and modules defined in C / C++.
 *
 *  An ooSQLite package object holds all the information neeeded to create
 *  collation, create function, and create module.  If passed a database
 *  connection it can automatically register all the functions in the package.
 *
 *  The object can return a collation, function, or module as a RexxBufferObject
 *  (or as Collation, Function, or Module class objects?)
 */
#define OOSQLPACKAGE_CLASS    "ooSQLPackage"
#define SYSLIB_ATTRIBUTE      "ooSQLPackageSysLibraryAttribute"


/** ooSQLPackage::init()
 */
RexxMethod2(RexxObjectPtr, oosqlpack_init, CSTRING, libName, OSELF, self)
{
    RexxMethodContext *c = context;

    RexxBufferObject cselfBuffer = context->NewBuffer(sizeof(CooSQLPackage));
    if ( cselfBuffer == NULLOBJECT )
    {
        outOfMemoryException(context->threadContext);
        return NULLOBJECT;
    }

    context->SetObjectVariable("CSELF", cselfBuffer);

    pCooSQLPackage pcp = (pCooSQLPackage)context->BufferData(cselfBuffer);
    memset(pcp, 0, sizeof(CooSQLPackage));

    RexxBufferObject libBuffer = context->NewBuffer(sizeof(SysLibrary));

    context->SetObjectVariable(SYSLIB_ATTRIBUTE, libBuffer);

    void *sysLibBuf = context->BufferData(libBuffer);
    SysLibrary *lib = new (sysLibBuf) SysLibrary();

    pcp->sqliteAPIs  = oosqlite3_get_routines();
    pcp->rexxSelf    = self;
    pcp->lib         = lib;
    resetPackageLastErr(context, pcp);

    if ( ! lib->load(libName) )
    {
        pcp->lastErrMsg  = context->String(lib->getLastErrMsg());
        pcp->lastErrCode = c->UnsignedInt32(lib->getLastErrCode());

        lib->resetLastErr();
        extensionsSetLastErr(context, pcp->lastErrMsg, pcp->lastErrCode);
        return NULLOBJECT;
    }

    OOSQLITE_LOADER loader = (OOSQLITE_LOADER)lib->getProcedure("ooSQLiteGetPackage");
    if ( loader == NULL )
    {
        pcp->lastErrMsg  = context->String(lib->getLastErrMsg());
        pcp->lastErrCode = c->UnsignedInt32(lib->getLastErrCode());

        lib->resetLastErr();
        extensionsSetLastErr(context, pcp->lastErrMsg, pcp->lastErrCode);
        return NULLOBJECT;
    }

    pcp->packageEntry = (*loader)(pcp->sqliteAPIs);
    pcp->valid        = true;

    return NULLOBJECT;
}


/** ooSQLPackage::lastErrCode  [attribute get]
 */
RexxMethod1(RexxObjectPtr, oosqlpack_getLastErrCode_atr, CSELF, pCSelf)
{
    pCooSQLPackage pcp = (pCooSQLPackage)pCSelf;
    return pcp->lastErrCode;
}

/** ooSQLPackage::lastErrMsg  [attribute get]
 */
RexxMethod1(RexxStringObject, oosqlpack_getLastErrMsg_atr, CSELF, pCSelf)
{
    pCooSQLPackage pcp = (pCooSQLPackage)pCSelf;
    return pcp->lastErrMsg;
}

/**
 *  Methods for the .ooSQLLibrary class.
 *
 */
#define OOSQLIBRARY_CLASS               "ooSQLLibrary"
#define SYSLIB_OOSQLLIBRARY_ATTRIBUTE   "ooSQLLibrarySysLibraryAttribute"


/** ooSQLLibrary::init()
 */
RexxMethod2(RexxObjectPtr, oosqllib_init, CSTRING, libName, OSELF, self)
{
    RexxMethodContext *c = context;

    RexxBufferObject cselfBuffer = context->NewBuffer(sizeof(CooSQLLibrary));
    if ( cselfBuffer == NULLOBJECT )
    {
        outOfMemoryException(context->threadContext);
        return NULLOBJECT;
    }

    context->SetObjectVariable("CSELF", cselfBuffer);

    pCooSQLLibrary pcl = (pCooSQLLibrary)context->BufferData(cselfBuffer);
    memset(pcl, 0, sizeof(CooSQLLibrary));

    RexxBufferObject libBuffer = context->NewBuffer(sizeof(SysLibrary));

    context->SetObjectVariable(SYSLIB_OOSQLLIBRARY_ATTRIBUTE, libBuffer);

    void *sysLibBuf = context->BufferData(libBuffer);
    SysLibrary *lib = new (sysLibBuf) SysLibrary();

    pcl->sqliteAPIs  = oosqlite3_get_routines();
    pcl->rexxSelf    = self;
    pcl->lib         = lib;
    resetLibraryLastErr(context, pcl);

    if ( ! lib->load(libName) )
    {
        pcl->lastErrMsg  = context->String(lib->getLastErrMsg());
        pcl->lastErrCode = c->UnsignedInt32(lib->getLastErrCode());

        lib->resetLastErr();
        extensionsSetLastErr(context, pcl->lastErrMsg, pcl->lastErrCode);
        return NULLOBJECT;
    }

    OOSQLITE_API_SETTER setter = (OOSQLITE_API_SETTER)lib->getProcedure("ooSQLiteSetApi");
    if ( setter == NULL )
    {
        pcl->lastErrMsg  = context->String(lib->getLastErrMsg());
        pcl->lastErrCode = c->UnsignedInt32(lib->getLastErrCode());

        lib->resetLastErr();
        extensionsSetLastErr(context, pcl->lastErrMsg, pcl->lastErrCode);
        return NULLOBJECT;
    }

    pcl->functionTable = rxNewBuiltinObject(context->threadContext, "TABLE");
    pcl->valid         = true;

    (*setter)(pcl->sqliteAPIs);

    return NULLOBJECT;
}


/** ooSQLLibrary::lastErrCode  [attribute get]
 */
RexxMethod1(RexxObjectPtr, oosqllib_getLastErrCode_atr, CSELF, pCSelf)
{
    pCooSQLLibrary pcl = (pCooSQLLibrary)pCSelf;
    return pcl->lastErrCode;
}

/** ooSQLExtensions::lastErrMsg  [attribute get]
 */
RexxMethod1(RexxStringObject, oosqllib_getLastErrMsg_atr, CSELF, pCSelf)
{
    pCooSQLLibrary pcl = (pCooSQLLibrary)pCSelf;
    return pcl->lastErrMsg;
}


#define ooSQLite_Routines_Section


static inline sqlite3_backup *routineBu(RexxCallContext *c, void *_bu, size_t pos)
{
    sqlite3_backup *bu = (sqlite3_backup *)_bu;
    if ( bu == NULL )
    {
        nullObjectException(c->threadContext, "backup handle", pos);
    }
    return bu;
}

static inline sqlite3 *routineDB(RexxCallContext *c, void *_db, size_t pos)
{
    sqlite3 *db = (sqlite3 *)_db;
    if ( db == NULL )
    {
        nullObjectException(c->threadContext, "database connection", pos);
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
    cbc->interpreter     = c->threadContext->instance;
    cbc->initialThreadID = oosqlGetCurrentThreadId();

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



/** ooSQLiteEncryptionAvailable()
 *
 *  Returns true if the Boton crypto library is built in, otherwise false.
 *
 */
RexxRoutine0(logical_t, ooSQLiteEncryptionAvailable_rtn)
{
#ifdef SQLITE_HAS_CODEC
    return 1;
#else
    return 0;
#endif
}

/** ooSQLiteMerge()
 *
 *  Performs a bitwise or ( a | b ) on the arguments provided.
 *
 *  @args  Each argument is or'd together.  Any number of arguments can be
 *         supplied.  If no arguments are supplied, 0 is returned.  If 1
 *         argument is provided, then that argument is returned.
 *
 *         Each argument must be a whole number and arguments can not be omitted
 *         within the series of arguments.  That is, if argument N exists, then
 *         arguments 1 through (N - 1) arguments must all exist.
 *
 *  @return  The value of a bitwise or of every argument.
 *
 */
RexxRoutine1(wholenumber_t, ooSQLiteMerge_rtn, ARGLIST, args)
{
    return genMerge(context->threadContext, args);
}


/** ooSQLiteEnquote()
 *
 *  @param values  [optional]
 *
 *  @remarks We need a thread context for some of the helper functions called
 *           here, so we just convert context to that and use it through out.
 */
RexxRoutine1(RexxObjectPtr, ooSQLiteEnquote_rtn, OPTIONAL_RexxObjectPtr, values)
{
    RexxThreadContext *c = context->threadContext;

    if ( argumentOmitted(1) )
    {
        return c->String("NULL");
    }
    else if ( ! c->IsStem(values) )
    {
        char *s = sqlite3_mprintf("%Q", obj2str(c, values));
        return enquoteResult(c, s);
    }
    else
    {
        RexxStemObject stem   = (RexxStemObject)values;
        RexxObjectPtr  _count = c->GetStemArrayElement(stem, 0);

        size_t count;
        if ( _count == NULLOBJECT || ! c->ObjectToStringSize(_count, &count) )
        {
            invalidTypeException(context->threadContext, 1, "stem in 'array' format with the count of tails at tail \"0\"");
            return NULLOBJECT;
        }

        char *s = sqlite3_mprintf("%Q", obj2str(c, c->GetStemArrayElement(stem, 1)));

        if ( s == NULL )
        {
            outOfMemoryException(c);
            return NULLOBJECT;
        }

        char  buf[MAX_ENQUOTE_STRING + 1];
        char *lead = s;

        for ( size_t i = 2; i <= count; i++)
        {
            lead = sqlite3_snprintf(sizeof(buf), buf, "%s, %Q", lead, obj2str(c, c->GetStemArrayElement(stem, i)));

            if ( i < count && strlen(lead) == MAX_ENQUOTE_STRING )
            {
                stringTooLongException(c, "ooSQLiteEnquote", false, MAX_ENQUOTE_STRING);
                break;
            }
        }

        sqlite3_free(s);
        return c->String(buf);
    }
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


/** oosqlBackupFinish()
 *
 *
 */
RexxRoutine1(int, oosqlBackupFinish_rtn, POINTER, _bu)
{
    sqlite3_backup *bu = routineBu(context, _bu, 1);
    if ( bu == NULL )
    {
        return SQLITE_MISUSE;
    }

    return sqlite3_backup_finish(bu);
}

/** oosqlBackupInit()
 *
 *  @param dstConn  [required]  An open database connection.  This database is
 *                  used as the destination of the backup.
 *
 *  @param srcConn  [required]  An open database connection.  This database is
 *                  used as the source of the backup.
 *
 *  @param dstName  [optional]  The name of the destination backup.  This is not
 *                  the file name but rather the 'main', 'temp', or attached as
 *                  name.  If omitted, 'main' is used.
 *
 *  @param srcName  [optional]  The name of the source backup.  This is not the
 *                  file name but rather the 'main', 'temp', or attached as
 *                  name.  If omitted, 'main' is used.
 *
 *  @return  On success returns a backup handle that is used in the other online
 *           backup functions.  The handle is an opaque datatype.  On error,
 *           this handle is null.  To test if the handle is null use the
 *           oosqlIsHandleNull() function.
 *
 *  @note  The parameter order is different from the order in
 *         sqlite3_backup_init() so that the optional parameters are last
 *
 */
RexxRoutine4(POINTER, oosqlBackupInit_rtn, POINTER, dstConn, POINTER, srcConn, OPTIONAL_CSTRING, dstName,
             OPTIONAL_CSTRING, srcName)
{
    sqlite3 *dst = routineDB(context, dstConn, 1);
    sqlite3 *src = routineDB(context, srcConn, 3);

    if ( dst == NULL || src == NULL )
    {
        return NULL;
    }

    if ( argumentOmitted(3) )
    {
        dstName = "main";
    }
    if ( argumentOmitted(4) )
    {
        srcName = "main";
    }

    return sqlite3_backup_init(dst, dstName, src, srcName);
}

/** oosqlBackupPageCount()
 *
 *
 */
RexxRoutine1(int, oosqlBackupPageCount_rtn, POINTER, _bu)
{
    sqlite3_backup *bu = routineBu(context, _bu, 1);
    if ( bu == NULL )
    {
        return SQLITE_MISUSE;
    }

    return sqlite3_backup_pagecount(bu);
}

/** oosqlBackupRemaining()
 *
 *
 */
RexxRoutine1(int, oosqlBackupRemaining_rtn, POINTER, _bu)
{
    sqlite3_backup *bu = routineBu(context, _bu, 1);
    if ( bu == NULL )
    {
        return SQLITE_MISUSE;
    }

    return sqlite3_backup_remaining(bu);
}

/** oosqlBackupStep()
 *
 *
 */
RexxRoutine2(int, oosqlBackupStep_rtn, POINTER, _bu, OPTIONAL_int, pageCount)
{
    sqlite3_backup *bu = routineBu(context, _bu, 1);
    if ( bu == NULL )
    {
        return SQLITE_MISUSE;
    }

    return sqlite3_backup_step(bu, argumentOmitted(2) ? 5 : pageCount);
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
    sqlite3 *db = routineDB(context, _db, 1);
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
    sqlite3 *db = routineDB(context, _db, 1);
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
    sqlite3 *db = routineDB(context, _db, 1);
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
    sqlite3 *db = routineDB(context, _db, 1);
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

/** oosqlColumnIndex()
 *
 *  Returns the index of the column with the specified column name in the result
 *  set of a SELECT statement.
 *
 *  @param stmt     [required]  Handle to the prepared statement whose result
 *                              set will contain the specified column.
 *
 *  @param colName  [required]  The name of a column that is a column in a
 *                              SELECT statement.
 *
 *  @return  Returns the 1 based column index, or 0 if there is no column with
 *           the specified name.
 *
 *  @note    The name comparison is case insensitive.  I think this is correct,
 *           because as far as I can tell SQLite does not allow column names
 *           that differ only in case.  Not sure if this is SQLite specific or
 *           not.
 *
 *  @note    This is not method that maps to a SQLite API.  It is an ooSQLite
 *           enhancement.
 *
 *  @note    sqlite3_free() is a harmless nop for a null pointer.
 */
RexxRoutine2(int, oosqlColumnIndex_rtn, POINTER, _stmt, CSTRING, _colName)
{
    sqlite3_stmt *stmt = routineStmt(context, _stmt);
    if ( stmt == NULL )
    {
        return SQLITE_MISUSE;
    }

    char *colName     = NULL;
    char *currentName = NULL;
    int   result      = 0;
    int   count       = 0;

    colName = strdupupr(_colName);
    if ( colName == NULL )
    {
        goto err_out;
    }

    count = sqlite3_column_count(stmt);
    if ( count == 0 )
    {
        // Not an error, statement may not be a SELECT.
        goto done_out;
    }

    for ( int i = 0; i < count; i++)
    {
        CSTRING _currentName = sqlite3_column_name(stmt, i);
        if ( _currentName == NULL )
        {
            goto err_out;
        }

        char *currentName = strdupupr(_currentName);
        if ( currentName == NULL )
        {
            goto err_out;
        }

        if ( strcmp(colName, currentName) == 0 )
        {
            result = i + 1;
            goto done_out;
        }

        sqlite3_free(currentName);
    }

    goto done_out;

err_out:
    outOfMemoryException(context->threadContext);

done_out:
    sqlite3_free(colName);
    sqlite3_free(currentName);
    return result;
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
 *  @param nullObj [optional]  If the column value is SQL NULL, by default the
 *                             .nil object is returned.  This argument allows
 *                             the user to specify an alternative value to be
 *                             returned for SQL NULL.
 *
 */
RexxRoutine3(RexxObjectPtr, oosqlColumnText_rtn, POINTER, _stmt, int32_t, col, OPTIONAL_RexxObjectPtr, nullObj)
{
    sqlite3_stmt *stmt = routineStmt(context, _stmt);
    if ( stmt == NULL )
    {
        return context->WholeNumber(SQLITE_MISUSE);
    }

    if ( argumentOmitted(3) )
    {
        nullObj = TheNilObj;
    }

    col--;
    return safeColumnText(context->threadContext, stmt, col, nullObj);
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
    sqlite3 *db = routineDB(context, _db, 1);
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
    sqlite3 *db = routineDB(context, _db, 1);
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
    sqlite3 *db = routineDB(context, _db, 1);
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
    sqlite3 *db = routineDB(context, _db, 1);
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
    sqlite3 *db = routineDB(context, _db, 1);
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
    sqlite3 *db = routineDB(context, _db, 1);
    if ( db == NULL )
    {
        return SQLITE_MISUSE;
    }

    return genDbStatus(context->threadContext, db, param, _result, reset, 3);
}

/** oosqlErrCode()
/** oosqlExtendedErrCode()
 *
 */
RexxRoutine1(int, oosqlErrCode_rtn, POINTER, _db)
{
    sqlite3 *db = routineDB(context, _db, 1);
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
    sqlite3 *db = routineDB(context, _db, 1);
    if ( db == NULL )
    {
        return context->UnsignedInt32(SQLITE_MISUSE);
    }

    return context->String(sqlite3_errmsg(db));
}

/** oosqlErrStr()
 *
 */
RexxRoutine1(RexxObjectPtr, oosqlErrStr_rtn, int, resultCode)
{
    return genErrStr(context->threadContext, resultCode);
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
 *  @param  format       [optional]  By default for the 'classic Rexx'
 *                       interface, a result set is returned as a stem of stems.
 *                       However, for programmers that are trying to ease their
 *                       way into the use of objects, this argument can be used
 *                       to change the result set returned to be an array of
 *                       arrays or an array of directories. This argument is
 *                       ignored if doCallback is false.
 *
 *  @param  rtnName      The routine name in the Rexx progarm that will be
 *                       called each time the SQL produces a row.  This argument
 *                       is ignored unless doCallBack is not omitted and is
 *                       true.
 *
 *  @return  A result code, one of the .ooSQLiteConstants constants, or a result set as
 *           described above.
 *
 *  @note The callback routine runs in this same thread, so we do not have to
 *        worry about garbage collection of the objects or an incorrect thread
 *        context.
 */
RexxRoutine5(RexxObjectPtr, oosqlExec_rtn, POINTER, _db, CSTRING, sql, OPTIONAL_logical_t, doCallback,
             OPTIONAL_uint32_t, format, OPTIONAL_CSTRING, rtnName)
{
    sqlite3 *db = routineDB(context, _db, 1);
    if ( db == NULL )
    {
        return context->UnsignedInt32(SQLITE_MISUSE);
    }

    CGenericCallback  cbc    = {0};
    int               rc     = SQLITE_ERROR;
    char             *errMsg = NULL;

    if ( doCallback )
    {
        if ( argumentExists(4) )
        {
            if ( format < anArrayOfArrays || format > aClassicStem )
            {
                wrongArgValueException(context->threadContext, 3, RECORD_FORMATS_LIST,
                                       context->UnsignedInt32(format));
                return NULLOBJECT;
            }
            else
            {
                cbc.format = (ResultSetType)format;
            }
        }
        else
        {
            cbc.format = aStemOfStems;
        }

        cbc.callbackContext = context->threadContext;

        if ( argumentExists(5) )
        {
            cbc.callbackRtn = getCallerRoutine(context, rtnName);
            if ( cbc.callbackRtn == NULLOBJECT )
            {
                return noSuchRoutineException(context->threadContext, rtnName, 4);
            }
        }
        else
        {
            cbc.createRS = true;

            if ( cbc.format == aStemOfStems || cbc.format == aClassicStem )
            {
                RexxStemObject st = context->NewStem("records");
                context->SetStemArrayElement(st, 0, TheZeroObj);  // No records to start with.

                cbc.rsStem = st;
            }
            else
            {
                cbc.rsArray = context->NewArray(100);  // What is a good initial size?
            }
        }

        switch ( cbc.format )
        {
            case anArrayOfArrays :
                rc = sqlite3_exec(db, sql, execCallBackArray, (void *)&cbc, &errMsg);
                break;

            case anArrayOfDirectories :
                rc = sqlite3_exec(db, sql, execCallBackDirectory, (void *)&cbc, &errMsg);
                break;

            case aClassicStem :
                rc = sqlite3_exec(db, sql, execCallBackClassicStem, (void *)&cbc, &errMsg);
                break;

            default :
                rc = sqlite3_exec(db, sql, execCallBackStem, (void *)&cbc, &errMsg);
                break;
        }
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

    if ( cbc.createRS )
    {
        if ( cbc.format == aStemOfStems || cbc.format == aClassicStem )
        {
            return cbc.rsStem;
        }
        else
        {
            return cbc.rsArray;
        }
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
RexxRoutine2(int, oosqlExtendedResultCodes_rtn, POINTER, _db, logical_t, onOff)
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
    sqlite3 *db = routineDB(context, _db, 1);
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
    sqlite3 *db = routineDB(context, _db, 1);
    if ( db == NULL )
    {
        return SQLITE_MISUSE;
    }

    sqlite3_interrupt(db);

    return SQLITE_OK;
}

/** oosqlIsHandleNull()
 *
 *  A convenience function that the classic Rexx programmer can use as an
 *  alternative to handle~isNull.
 *
 *  @param  [required]  The handle to test.
 *
 *  @returns  True if the specified handle is null, otherwise false.
 *
 */
RexxRoutine1(logical_t, oosqlIsHandleNull_rtn, POINTER, handle)
{
    if ( handle == NULL )
    {
        return 1;
    }
    return 0;
}

#ifdef SQLITE_HAS_CODEC
/** oosqlKey()
 *
 *  Provides an encryption key for use to access a database.
 *
 *  @param db     [required] The database connection the statement is for.
 *
 *  @param key    [required] The encryption key to use.
 *
 *  @return  Returns one of the SQLite status code constants.
 */
RexxRoutine2(int, oosqlKey_rtn, POINTER, _db, CSTRING, key)
{
    sqlite3 *db = routineDB(context, _db, 1);
    if ( db == NULL )
    {
        return SQLITE_MISUSE;
    }

    return sqlite3_key(db, key, (int)strlen(key));
}

#else

RexxRoutine2(int, oosqlKey_rtn, POINTER, _db, CSTRING, key)
{
    sqlite3 *db = routineDB(context, _db, 1);
    if ( db == NULL )
    {
        return SQLITE_MISUSE;
    }

    return SQLITE_MISUSE;
}
#endif

/** oosqlLastInsertRowID()
 *
 *
 */
RexxRoutine1(int64_t, oosqlLastInsertRowID_rtn, POINTER, _db)
{
    sqlite3 *db = routineDB(context, _db, 1);
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
    sqlite3 *db = routineDB(context, _db, 1);
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
RexxRoutine1(int64_t, oosqlMemoryHighWater_rtn, OPTIONAL_logical_t, reset)
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
 *  @return  On success returns a handle to the mutex that can be used as an
 *           argument in any other function that requires a mutex handle. The
 *           handle is an opaque datatype.
 *
 *           On error, this handle is null. To test if the handle is null use
 *           the oosqlIsHandleNull() function.  Do not use a null handle in any
 *           other function.
 */
RexxRoutine1(POINTER, oosqlMutexAlloc_rtn, int, type)
{
    if ( ! (type == SQLITE_MUTEX_FAST || type == SQLITE_MUTEX_RECURSIVE)  )
    {
        wrongArgValueException(context->threadContext, 1, "MUTEX_RECURSIVE or MUTEX_FAST",
                               context->WholeNumber(type));
        return NULL;
    }

    return sqlite3_mutex_alloc(type);
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
 */
RexxRoutine2(RexxObjectPtr, oosqlNextStmt_rtn, POINTER, _db, OPTIONAL_RexxObjectPtr, _stmt)
{
    sqlite3 *db = routineDB(context, _db, 1);
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

/** oosqlOpen()
 *
 *  Open a database connection.
 *
 *  @param file    [required] The file name of the database to open.
 *
 *  @param dbConn  [required] The *name* of a variable in your Rexx program
 *                            that will be set to the handle to the database
 *                            connection.  Note that this is the string name of
 *                            the variable, not the variable itself. The
 *                            variable can or can not already exist in your
 *                            program.
 *
 *  @param flags   [optional] Flags that control how the database is opened.
 *
 *  @param vfsName [optional] The name of the alternative operating system
 *                            interface to use.  This option is ignored by
 *                            ooSQLite at this time.
 *
 *  @return  On success returns OK, otherwise returns an error code.
 *
 *           On error, this handle could be null. To test if the handle is null
 *           the oosqlIsHandleNull() function can be used.  However testing the
 *           return code is sufficient.  If the return code is NOMEM, the handle
 *           is always null, otherwise, it is never null. Do not use a null
 *           handle in any other function.
 *
 *  @note    The connection is (almost) always returned, even on error.  The
 *           programmer should immediately use oosqlErrCode() to check for
 *           error.  If oosqlErrCode() does not return .ooSQLiteConstants~OK
 *           then the database was not opened properly and the connection should
 *           not be used and must be closed.
 *
 *           oosqlClose() must always be called to properly close the database,
 *           even if it was opened with an error.
 *
 *           The only exception to this is if the returned handle is null, in
 *           which case no connection was created.  A null handle should never
 *           be used in any ooSQLite function, with this exception.  A null
 *           handle passed to oosqlClose() is a harmles nop.
 */
RexxRoutine4(int, oosqlOpen_rtn, CSTRING, file, CSTRING, dbConn, OPTIONAL_int32_t, _flags, OPTIONAL_CSTRING, vfsName)
{
    sqlite3 *db;

    int flags = SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE;
    if ( argumentExists(3) )
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

    context->SetContextVariable(dbConn, context->NewPointer(db));

    return rc;
}

/** oosqlPrepare()
 *
 *  Prepares a SQL statement to be executed by SQLite and returns a handle to
 *  that prepared statement.
 *
 *  To execute a SQL statement, SQLite first compiles the statement into a
 *  byte-code program.  This can be thought of as preparing, or initializing
 *  the statement.
 *
 *
 *  @param db     [required] The database connection the statement is for.
 *
 *  @param sql    [required] The SQL statement(s) used for this prepared
 *                           statement.
 *
 *  @param _stmt  [required]  The *name* of a variable in your Rexx program that
 *                            will be set to the handle to the prepared
 *                            statement.  Note that this is the string name of
 *                            the variable, not the variable itself.  The
 *                            variable can or can not already exist in your
 *                            program.
 *
 *  @param _tail  [optional] The *name* of a variable in your Rexx program that
 *                           will be set to the 'tail' of the SQL statement.
 *
 *                           SQLite only compiles the first statement in sql.
 *                           The 'tail' is the portion, if any, of the sql
 *                           string past the first ';', or the empty string.
 *
 *                           When the optional tail argument is used, the
 *                           variable of that name is set to the unused portion
 *                           of the sql string.  If there is no unused portion
 *                           the variable is set to the empty string.  When this
 *                           argument is omitted, the tail, if any, is ignored.
 *
 *  @return  Returns one of the SQLite status code constants.  On success this
 *           will be OK and the _stmt variable in the Rexx program will be set
 *           to the handle to the prepared statement.
 *
 *           On error, an error status code is returned, and the handle to the
 *           prepared statement will be a null handle.
 *
 *           Note that the _stmt variable is always set to a handle.  To test if
 *           the handle is null use the oosqlIsHandleNull() function.  However,
 *           checking the return code is sufficient.  If the return code is not
 *           OK, then the handle will be null.  Do not use a null handle in any
 *           other function, except it is a harmless nop to use a null handle in
 *           oosqlFinalize().
 *
 *  @remarks  Testing has shown that if ppTail is not null when passed into
 *            sqlite3_prepare_v2() it is always set to a zero-terminated string.
 *            I.e. even when there is no 'tail', it is set to an empty string.
 */
RexxRoutine4(int, oosqlPrepare_rtn, POINTER, _db, CSTRING, sql, CSTRING, _stmt, OPTIONAL_CSTRING, _tail)
{
    sqlite3_stmt *stmt   = NULL;
    const char   *tail   = NULL;
    const char  **ppTail = NULL;

    sqlite3 *db = routineDB(context, _db, 1);
    if ( db == NULL )
    {
        return SQLITE_MISUSE;
    }

    if ( argumentExists(4) )
    {
        ppTail = &tail;
    }

    int rc = sqlite3_prepare_v2(db, sql, (int)strlen(sql) + 1, &stmt, ppTail);

    if ( rc == SQLITE_OK && argumentExists(4) && tail != NULL )
    {
        context->SetContextVariable(_tail, context->String(tail));
    }

    context->SetContextVariable(_stmt, context->NewPointer(stmt));

    return rc;
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
    sqlite3 *db = routineDB(context, _db, 1);
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
    sqlite3 *db = routineDB(context, _db, 1);
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


#ifdef SQLITE_HAS_CODEC
/** oosqlRekey()
 *
 *  Prevides a new encryption key for use to access a database.
 *
 *  @param db     [required] The database connection the statement is for.
 *
 *  @param key    [required] The new encryption key to use.
 *
 *  @return  Returns one of the SQLite status code constants.
 */
RexxRoutine2(int, oosqlRekey_rtn, POINTER, _db, CSTRING, key)
{
    sqlite3 *db = routineDB(context, _db, 1);
    if ( db == NULL )
    {
        return SQLITE_MISUSE;
    }

    return sqlite3_rekey(db, key, (int)strlen(key));
}

#else

/** oosqlRekey()
 *
 *  Prevides a new encryption key for use to access a database.
 *
 *  @param db     [required] The database connection the statement is for.
 *
 *  @param key    [required] The new encryption key to use.
 *
 *  @return  Returns one of the SQLite status code constants.
 */
RexxRoutine2(int, oosqlRekey_rtn, POINTER, _db, CSTRING, key)
{
    sqlite3 *db = routineDB(context, _db, 1);
    if ( db == NULL )
    {
        return SQLITE_MISUSE;
    }

    return SQLITE_MISUSE;
}
#endif

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
    sqlite3 *db = routineDB(context, _db, 1);
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
    sqlite3 *db = routineDB(context, _db, 1);
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
 *  Retrieves runtime status information about the performance of SQLite, and
 *  optionally resets various highwater marks.
 */
RexxRoutine3(int, oosqlStatus_rtn, int, param, RexxObjectPtr, result, OPTIONAL_logical_t, reset)
{
    return genStatus(context->threadContext, param, result, reset, 2, false);
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
 *  @param  results  A Stem object in which the metadata is returned.
 *
 *  @return  OK on success, othewise an error code.
 *
 *  @note The parameter order here is slightly switched from
 *        sqlite3_table_column_metadata() to put the opitonal dbName after
 *        results, so that optional arguments are placed at the end.
 */
RexxRoutine5(int, oosqlTableColumnMetadata_rtn, POINTER, _db, CSTRING, tableName, CSTRING, colName, RexxObjectPtr, results,
             OPTIONAL_CSTRING, dbName)
{
    sqlite3 *db = routineDB(context, _db, 1);
    if ( db == NULL )
    {
        return SQLITE_MISUSE;
    }

    if ( argumentOmitted(5) || strlen(dbName) < 1 )
    {
        dbName = NULL;
    }

    return genTableColumnMetadata(context->threadContext, db, dbName, tableName, colName, results, 4);
}

/** oosqlThreadSafe()
 *
 *
 */
RexxRoutine0(int, oosqlThreadSafe_rtn)
{
    return sqlite3_threadsafe();
}

/** oosqlTotalChanges()
 *
 *
 */
RexxRoutine1(int, oosqlTotalChanges_rtn, POINTER, _db)
{
    sqlite3 *db = routineDB(context, _db, 1);
    if ( db == NULL )
    {
        return SQLITE_MISUSE;
    }

    return sqlite3_total_changes(db);
}

/** ooSQLiteConnection::trace()
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
    sqlite3 *db = routineDB(context, _db, 1);
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
    sqlite3 *db = routineDB(context, _db, 1);
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
RexxRoutine1(int, oosqlTest_rtn, CSTRING, varName)
{
    context->SetContextVariable(varName, context->NewPointer(NULL));

    return SQLITE_OK;
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

#if OOSQLDBG == 1
    if ( cbc == NULL )
    {
        printf("db_cb_releaseBuffer cbc=%p, is NULL\n", cbc);
    }
    else
    {
        printf("db_cb_releaseBuffer routine name=%s\n", cbc->routineName);
    }
#endif

    if ( cbc != NULL )
    {
        if ( cbc->userData != NULLOBJECT )
        {
            context->ReleaseGlobalReference(cbc->userData);
        }
    }

    return 0;
}




REXX_TYPED_ROUTINE_PROTOTYPE(ooSQLiteEnquote_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(ooSQLiteMerge_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(ooSQLiteVersion_rtn);

REXX_TYPED_ROUTINE_PROTOTYPE(oosqlBackupFinish_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlBackupInit_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlBackupPageCount_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlBackupRemaining_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlBackupStep_rtn);
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
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlColumnIndex_rtn);
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
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlErrStr_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlExec_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlExtendedResultCodes_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlFinalize_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlGetAutocommit_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlInterrupt_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlIsHandleNull_rtn);
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlKey_rtn);
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
REXX_TYPED_ROUTINE_PROTOTYPE(oosqlRekey_rtn);
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
    REXX_TYPED_ROUTINE(ooSQLiteEncryptionAvailable_rtn, ooSQLiteEncryptionAvailable_rtn),
    REXX_TYPED_ROUTINE(ooSQLiteEnquote_rtn,             ooSQLiteEnquote_rtn),
    REXX_TYPED_ROUTINE(ooSQLiteMerge_rtn,               ooSQLiteMerge_rtn),
    REXX_TYPED_ROUTINE(ooSQLiteVersion_rtn,             ooSQLiteVersion_rtn),

    REXX_TYPED_ROUTINE(oosqlBackupFinish_rtn,         oosqlBackupFinish_rtn),
    REXX_TYPED_ROUTINE(oosqlBackupInit_rtn,           oosqlBackupInit_rtn),
    REXX_TYPED_ROUTINE(oosqlBackupPageCount_rtn,      oosqlBackupRemaining_rtn),
    REXX_TYPED_ROUTINE(oosqlBackupRemaining_rtn,      oosqlBackupPageCount_rtn),
    REXX_TYPED_ROUTINE(oosqlBackupStep_rtn,           oosqlBackupStep_rtn),
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
    REXX_TYPED_ROUTINE(oosqlColumnIndex_rtn,          oosqlColumnIndex_rtn),
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
    REXX_TYPED_ROUTINE(oosqlErrStr_rtn,	              oosqlErrStr_rtn),
    REXX_TYPED_ROUTINE(oosqlExec_rtn,                 oosqlExec_rtn),
    REXX_TYPED_ROUTINE(oosqlExtendedResultCodes_rtn,  oosqlExtendedResultCodes_rtn),
    REXX_TYPED_ROUTINE(oosqlFinalize_rtn,             oosqlFinalize_rtn),
    REXX_TYPED_ROUTINE(oosqlGetAutocommit_rtn,        oosqlGetAutocommit_rtn),
    REXX_TYPED_ROUTINE(oosqlInterrupt_rtn,            oosqlInterrupt_rtn),
    REXX_TYPED_ROUTINE(oosqlIsHandleNull_rtn,         oosqlIsHandleNull_rtn),
    REXX_TYPED_ROUTINE(oosqlKey_rtn,                  oosqlKey_rtn),
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
    REXX_TYPED_ROUTINE(oosqlRekey_rtn,                oosqlRekey_rtn),
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



// .ooSQLiteConstants
REXX_METHOD_PROTOTYPE(oosqlC_merge_cls);

// .ooSQLite
REXX_METHOD_PROTOTYPE(oosql_init_cls);
REXX_METHOD_PROTOTYPE(oosql_getNull_atr_cls);
REXX_METHOD_PROTOTYPE(oosql_setNull_atr_cls);
REXX_METHOD_PROTOTYPE(oosql_getRecordFormat_atr_cls);
REXX_METHOD_PROTOTYPE(oosql_setRecordFormat_atr_cls);
REXX_METHOD_PROTOTYPE(oosql_compileOptionGet_cls);
REXX_METHOD_PROTOTYPE(oosql_compileOptionUsed_cls);
REXX_METHOD_PROTOTYPE(oosql_complete_cls);
REXX_METHOD_PROTOTYPE(oosql_encryptionAvailable_cls);
REXX_METHOD_PROTOTYPE(oosql_enquote_cls);
REXX_METHOD_PROTOTYPE(oosql_errStr_cls);
REXX_METHOD_PROTOTYPE(oosql_libVersion_cls);
REXX_METHOD_PROTOTYPE(oosql_libVersionNumber_cls);
REXX_METHOD_PROTOTYPE(oosql_memoryUsed_cls);
REXX_METHOD_PROTOTYPE(oosql_releaseMemory_cls);
REXX_METHOD_PROTOTYPE(oosql_softHeapLimit64_cls);
REXX_METHOD_PROTOTYPE(oosql_sourceID_cls);
REXX_METHOD_PROTOTYPE(oosql_sqlite3Version_cls);
REXX_METHOD_PROTOTYPE(oosql_status_cls);
REXX_METHOD_PROTOTYPE(oosql_threadSafe_cls);
REXX_METHOD_PROTOTYPE(oosql_version_cls);

REXX_METHOD_PROTOTYPE(oosql_test_cls);

// .ooSQLiteConnection
REXX_METHOD_PROTOTYPE(oosqlconn_init_cls);

REXX_METHOD_PROTOTYPE(oosqlconn_getBackupDestination_atr);
REXX_METHOD_PROTOTYPE(oosqlconn_getClosed_atr);
REXX_METHOD_PROTOTYPE(oosqlconn_getFileName_atr);
REXX_METHOD_PROTOTYPE(oosqlconn_getInitCode_atr);
REXX_METHOD_PROTOTYPE(oosqlconn_getLastErrCode_atr);
REXX_METHOD_PROTOTYPE(oosqlconn_getLastErrMsg_atr);
REXX_METHOD_PROTOTYPE(oosqlconn_getNull_atr);
REXX_METHOD_PROTOTYPE(oosqlconn_setNull_atr);
REXX_METHOD_PROTOTYPE(oosqlconn_getRecordFormat_atr);
REXX_METHOD_PROTOTYPE(oosqlconn_setRecordFormat_atr);

REXX_METHOD_PROTOTYPE(oosqlconn_init);
REXX_METHOD_PROTOTYPE(oosqlconn_uninit);

REXX_METHOD_PROTOTYPE(oosqlconn_busyHandler);
REXX_METHOD_PROTOTYPE(oosqlconn_busyTimeOut);
REXX_METHOD_PROTOTYPE(oosqlconn_changes);
REXX_METHOD_PROTOTYPE(oosqlconn_close);
REXX_METHOD_PROTOTYPE(oosqlconn_collationNeeded);
REXX_METHOD_PROTOTYPE(oosqlconn_commitHook);
REXX_METHOD_PROTOTYPE(oosqlconn_createCollation);
REXX_METHOD_PROTOTYPE(oosqlconn_dbFileName);
REXX_METHOD_PROTOTYPE(oosqlconn_dbMutex);
REXX_METHOD_PROTOTYPE(oosqlconn_dbReadOnly);
REXX_METHOD_PROTOTYPE(oosqlconn_dbReleaseMemory);
REXX_METHOD_PROTOTYPE(oosqlconn_dbStatus);
REXX_METHOD_PROTOTYPE(oosqlconn_enableLoadExtension);
REXX_METHOD_PROTOTYPE(oosqlconn_errCode);
REXX_METHOD_PROTOTYPE(oosqlconn_errMsg);
REXX_METHOD_PROTOTYPE(oosqlconn_exec);
REXX_METHOD_PROTOTYPE(oosqlconn_extendedResultCodes);
REXX_METHOD_PROTOTYPE(oosqlconn_getAutoCommit);
REXX_METHOD_PROTOTYPE(oosqlconn_interrupt);
REXX_METHOD_PROTOTYPE(oosqlconn_key);
REXX_METHOD_PROTOTYPE(oosqlconn_lastInsertRowID);
REXX_METHOD_PROTOTYPE(oosqlconn_limit);
REXX_METHOD_PROTOTYPE(oosqlconn_loadExtension);
REXX_METHOD_PROTOTYPE(oosqlconn_nextStmt);
REXX_METHOD_PROTOTYPE(oosqlconn_pragma);
REXX_METHOD_PROTOTYPE(oosqlconn_profile);
REXX_METHOD_PROTOTYPE(oosqlconn_progressHandler);
REXX_METHOD_PROTOTYPE(oosqlconn_rekey);
REXX_METHOD_PROTOTYPE(oosqlconn_rollbackHook);
REXX_METHOD_PROTOTYPE(oosqlconn_setAuthorizer);
REXX_METHOD_PROTOTYPE(oosqlconn_tableColumnMetadata);
REXX_METHOD_PROTOTYPE(oosqlconn_totalChanges);
REXX_METHOD_PROTOTYPE(oosqlconn_trace);
REXX_METHOD_PROTOTYPE(oosqlconn_updateHook);

REXX_METHOD_PROTOTYPE(oosqlconn_putStmt);
REXX_METHOD_PROTOTYPE(oosqlconn_delStmt);
REXX_METHOD_PROTOTYPE(oosqlconn_test);

// .ooSQLiteStmt
REXX_METHOD_PROTOTYPE(oosqlstmt_init_cls);

REXX_METHOD_PROTOTYPE(oosqlstmt_getFinalize_atr);
REXX_METHOD_PROTOTYPE(oosqlstmt_getInitCode_atr);
REXX_METHOD_PROTOTYPE(oosqlstmt_getLastErrCode_atr);
REXX_METHOD_PROTOTYPE(oosqlstmt_getLastErrMsg_atr);
REXX_METHOD_PROTOTYPE(oosqlstmt_getNull_atr);
REXX_METHOD_PROTOTYPE(oosqlstmt_setNull_atr);
REXX_METHOD_PROTOTYPE(oosqlstmt_getRecordFormat_atr);
REXX_METHOD_PROTOTYPE(oosqlstmt_setRecordFormat_atr);

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
REXX_METHOD_PROTOTYPE(oosqlstmt_columnIndex);
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

// .ooSQLiteBackup
REXX_METHOD_PROTOTYPE(oosqlbu_getFinished_atr);
REXX_METHOD_PROTOTYPE(oosqlbu_getInitCode_atr);
REXX_METHOD_PROTOTYPE(oosqlbu_getLastErrCode_atr);
REXX_METHOD_PROTOTYPE(oosqlbu_getLastErrMsg_atr);
REXX_METHOD_PROTOTYPE(oosqlbu_getPageCount_atr);
REXX_METHOD_PROTOTYPE(oosqlbu_getRemaining_atr);
REXX_METHOD_PROTOTYPE(oosqlbu_getSaveDestConn_atr);
REXX_METHOD_PROTOTYPE(oosqlbu_setSaveDestConn_atr);

REXX_METHOD_PROTOTYPE(oosqlbu_init);
REXX_METHOD_PROTOTYPE(oosqlbu_uninit);

REXX_METHOD_PROTOTYPE(oosqlbu_finish);
REXX_METHOD_PROTOTYPE(oosqlbu_getDestConn);
REXX_METHOD_PROTOTYPE(oosqlbu_step);

// .ooSQLiteMutex
REXX_METHOD_PROTOTYPE(oosqlmtx_init_cls);

REXX_METHOD_PROTOTYPE(oosqlmtx_getClosed_atr);
REXX_METHOD_PROTOTYPE(oosqlmtx_getIsNull_atr);

REXX_METHOD_PROTOTYPE(oosqlmtx_init);
REXX_METHOD_PROTOTYPE(oosqlmtx_uninit);

REXX_METHOD_PROTOTYPE(oosqlmtx_enter);
REXX_METHOD_PROTOTYPE(oosqlmtx_free);
REXX_METHOD_PROTOTYPE(oosqlmtx_leave);
REXX_METHOD_PROTOTYPE(oosqlmtx_try);

// .ooSQLExtensions
REXX_METHOD_PROTOTYPE(oosqlext_init_cls);

REXX_METHOD_PROTOTYPE(oosqlext_getLastErrCode_atr);
REXX_METHOD_PROTOTYPE(oosqlext_getLastErrMsg_atr);

REXX_METHOD_PROTOTYPE(oosqlext_loadLibrary_cls);
REXX_METHOD_PROTOTYPE(oosqlext_loadPackage_cls);

// .ooSQLPackage
REXX_METHOD_PROTOTYPE(oosqlpack_init);

REXX_METHOD_PROTOTYPE(oosqlpack_getLastErrCode_atr);
REXX_METHOD_PROTOTYPE(oosqlpack_getLastErrMsg_atr);

// .ooSQLLibrary
REXX_METHOD_PROTOTYPE(oosqllib_init);

REXX_METHOD_PROTOTYPE(oosqllib_getLastErrCode_atr);
REXX_METHOD_PROTOTYPE(oosqllib_getLastErrMsg_atr);

// __rtn_helper_class
REXX_METHOD_PROTOTYPE(hlpr_init_cls);

// __rtn_db_callback_class
REXX_METHOD_PROTOTYPE(db_cb_releaseBuffer);



RexxMethodEntry ooSQLite_methods[] = {
    // .ooSQLiteConstants
    REXX_METHOD(oosqlC_merge_cls,      oosqlC_merge_cls),

    // .ooSQLite
    REXX_METHOD(oosql_init_cls,                       oosql_init_cls),
    REXX_METHOD(oosql_getNull_atr_cls,                oosql_getNull_atr_cls),
    REXX_METHOD(oosql_setNull_atr_cls,                oosql_setNull_atr_cls),
    REXX_METHOD(oosql_getRecordFormat_atr_cls,        oosql_getRecordFormat_atr_cls),
    REXX_METHOD(oosql_setRecordFormat_atr_cls,        oosql_setRecordFormat_atr_cls),
    REXX_METHOD(oosql_compileOptionGet_cls,           oosql_compileOptionGet_cls),
    REXX_METHOD(oosql_compileOptionUsed_cls,          oosql_compileOptionUsed_cls),
    REXX_METHOD(oosql_complete_cls,                   oosql_complete_cls),
    REXX_METHOD(oosql_encryptionAvailable_cls,        oosql_encryptionAvailable_cls),
    REXX_METHOD(oosql_enquote_cls,                    oosql_enquote_cls),
    REXX_METHOD(oosql_errStr_cls,                     oosql_errStr_cls),
    REXX_METHOD(oosql_libVersion_cls,                 oosql_libVersion_cls),
    REXX_METHOD(oosql_libVersionNumber_cls,           oosql_libVersionNumber_cls),
    REXX_METHOD(oosql_memoryUsed_cls,                 oosql_memoryUsed_cls),
    REXX_METHOD(oosql_memoryHighWater_cls,            oosql_memoryHighWater_cls),
    REXX_METHOD(oosql_releaseMemory_cls,              oosql_releaseMemory_cls),
    REXX_METHOD(oosql_softHeapLimit64_cls,            oosql_softHeapLimit64_cls),
    REXX_METHOD(oosql_sourceID_cls,                   oosql_sourceID_cls),
    REXX_METHOD(oosql_sqlite3Version_cls,             oosql_sqlite3Version_cls),
    REXX_METHOD(oosql_status_cls,                     oosql_status_cls),
    REXX_METHOD(oosql_threadSafe_cls,                 oosql_threadSafe_cls),
    REXX_METHOD(oosql_version_cls,                    oosql_version_cls),

    REXX_METHOD(oosql_test_cls,                       oosql_test_cls),

    // .ooSQLiteConnection
    REXX_METHOD(oosqlconn_init_cls,                   oosqlconn_init_cls),

    REXX_METHOD(oosqlconn_getBackupDestination_atr,   oosqlconn_getBackupDestination_atr),
    REXX_METHOD(oosqlconn_getClosed_atr,              oosqlconn_getClosed_atr),
    REXX_METHOD(oosqlconn_getFileName_atr,            oosqlconn_getFileName_atr),
    REXX_METHOD(oosqlconn_getInitCode_atr,            oosqlconn_getInitCode_atr),
    REXX_METHOD(oosqlconn_getLastErrCode_atr,         oosqlconn_getLastErrCode_atr),
    REXX_METHOD(oosqlconn_getLastErrMsg_atr,          oosqlconn_getLastErrMsg_atr),
    REXX_METHOD(oosqlconn_getNull_atr,                oosqlconn_getNull_atr),
    REXX_METHOD(oosqlconn_setNull_atr,                oosqlconn_setNull_atr),
    REXX_METHOD(oosqlconn_getRecordFormat_atr,        oosqlconn_getRecordFormat_atr),
    REXX_METHOD(oosqlconn_setRecordFormat_atr,        oosqlconn_setRecordFormat_atr),

    REXX_METHOD(oosqlconn_init,                       oosqlconn_init),
    REXX_METHOD(oosqlconn_uninit,                     oosqlconn_uninit),

    REXX_METHOD(oosqlconn_busyHandler,                oosqlconn_busyHandler),
    REXX_METHOD(oosqlconn_busyTimeOut,                oosqlconn_busyTimeOut),
    REXX_METHOD(oosqlconn_changes,                    oosqlconn_changes),
    REXX_METHOD(oosqlconn_close,                      oosqlconn_close),
    REXX_METHOD(oosqlconn_collationNeeded,            oosqlconn_collationNeeded),
    REXX_METHOD(oosqlconn_commitHook,                 oosqlconn_commitHook),
    REXX_METHOD(oosqlconn_createCollation,            oosqlconn_createCollation),
    REXX_METHOD(oosqlconn_dbFileName,                 oosqlconn_dbFileName),
    REXX_METHOD(oosqlconn_dbMutex,                    oosqlconn_dbMutex),
    REXX_METHOD(oosqlconn_dbReadOnly,                 oosqlconn_dbReadOnly),
    REXX_METHOD(oosqlconn_dbReleaseMemory,            oosqlconn_dbReleaseMemory),
    REXX_METHOD(oosqlconn_dbStatus,                   oosqlconn_dbStatus),
    REXX_METHOD(oosqlconn_enableLoadExtension,        oosqlconn_enableLoadExtension),
    REXX_METHOD(oosqlconn_errCode,                    oosqlconn_errCode),
    REXX_METHOD(oosqlconn_errMsg,                     oosqlconn_errMsg),
    REXX_METHOD(oosqlconn_exec,                       oosqlconn_exec),
    REXX_METHOD(oosqlconn_extendedResultCodes,        oosqlconn_extendedResultCodes),
    REXX_METHOD(oosqlconn_getAutoCommit,              oosqlconn_getAutoCommit),
    REXX_METHOD(oosqlconn_interrupt,                  oosqlconn_interrupt),
    REXX_METHOD(oosqlconn_key,                        oosqlconn_key),
    REXX_METHOD(oosqlconn_lastInsertRowID,            oosqlconn_lastInsertRowID),
    REXX_METHOD(oosqlconn_limit,                      oosqlconn_limit),
    REXX_METHOD(oosqlconn_loadExtension,              oosqlconn_loadExtension),
    REXX_METHOD(oosqlconn_nextStmt,                   oosqlconn_nextStmt),
    REXX_METHOD(oosqlconn_pragma,                     oosqlconn_pragma),
    REXX_METHOD(oosqlconn_profile,                    oosqlconn_profile),
    REXX_METHOD(oosqlconn_progressHandler,            oosqlconn_progressHandler),
    REXX_METHOD(oosqlconn_rekey,                      oosqlconn_rekey),
    REXX_METHOD(oosqlconn_rollbackHook,               oosqlconn_rollbackHook),
    REXX_METHOD(oosqlconn_setAuthorizer,              oosqlconn_setAuthorizer),
    REXX_METHOD(oosqlconn_tableColumnMetadata,        oosqlconn_tableColumnMetadata),
    REXX_METHOD(oosqlconn_totalChanges,               oosqlconn_totalChanges),
    REXX_METHOD(oosqlconn_trace,                      oosqlconn_trace),
    REXX_METHOD(oosqlconn_updateHook,                 oosqlconn_updateHook),

    REXX_METHOD(oosqlconn_putStmt,                    oosqlconn_putStmt),
    REXX_METHOD(oosqlconn_delStmt,                    oosqlconn_delStmt),
    REXX_METHOD(oosqlconn_test,                       oosqlconn_test),

    // .ooSQLiteStmt
    REXX_METHOD(oosqlstmt_init_cls,                   oosqlstmt_init_cls),

    REXX_METHOD(oosqlstmt_getFinalized_atr,           oosqlstmt_getFinalized_atr),
    REXX_METHOD(oosqlstmt_getInitCode_atr,            oosqlstmt_getInitCode_atr),
    REXX_METHOD(oosqlstmt_getLastErrCode_atr,         oosqlstmt_getLastErrCode_atr),
    REXX_METHOD(oosqlstmt_getLastErrMsg_atr,          oosqlstmt_getLastErrMsg_atr),
    REXX_METHOD(oosqlstmt_getNull_atr,                oosqlstmt_getNull_atr),
    REXX_METHOD(oosqlstmt_setNull_atr,                oosqlstmt_setNull_atr),
    REXX_METHOD(oosqlstmt_getRecordFormat_atr,        oosqlconn_getRecordFormat_atr),
    REXX_METHOD(oosqlstmt_setRecordFormat_atr,        oosqlconn_setRecordFormat_atr),

    REXX_METHOD(oosqlstmt_init,                       oosqlstmt_init),
    REXX_METHOD(oosqlstmt_uninit,                     oosqlstmt_uninit),

    REXX_METHOD(oosqlstmt_bindBlob,                   oosqlstmt_bindBlob),
    REXX_METHOD(oosqlstmt_bindDouble,                 oosqlstmt_bindDouble),
    REXX_METHOD(oosqlstmt_bindInt,                    oosqlstmt_bindInt),
    REXX_METHOD(oosqlstmt_bindInt64,                  oosqlstmt_bindInt64),
    REXX_METHOD(oosqlstmt_bindNull,                   oosqlstmt_bindNull),
    REXX_METHOD(oosqlstmt_bindParameterCount,         oosqlstmt_bindParameterCount),
    REXX_METHOD(oosqlstmt_bindParameterIndex,         oosqlstmt_bindParameterIndex),
    REXX_METHOD(oosqlstmt_bindParameterName,          oosqlstmt_bindParameterName),
    REXX_METHOD(oosqlstmt_bindText,                   oosqlstmt_bindText),
    REXX_METHOD(oosqlstmt_bindValue,                  oosqlstmt_bindValue),
    REXX_METHOD(oosqlstmt_bindZeroBlob,               oosqlstmt_bindZeroBlob),
    REXX_METHOD(oosqlstmt_clearBindings,              oosqlstmt_clearBindings),
    REXX_METHOD(oosqlstmt_columnBlob,                 oosqlstmt_columnBlob),
    REXX_METHOD(oosqlstmt_columnBytes,                oosqlstmt_columnBytes),
    REXX_METHOD(oosqlstmt_columnCount,                oosqlstmt_columnCount),
    REXX_METHOD(oosqlstmt_columnDatabaseName,         oosqlstmt_columnDatabaseName),
    REXX_METHOD(oosqlstmt_columnDeclType,             oosqlstmt_columnDeclType),
    REXX_METHOD(oosqlstmt_columnDouble,               oosqlstmt_columnDouble),
    REXX_METHOD(oosqlstmt_columnIndex,                oosqlstmt_columnIndex),
    REXX_METHOD(oosqlstmt_columnInt,                  oosqlstmt_columnInt),
    REXX_METHOD(oosqlstmt_columnInt64,                oosqlstmt_columnInt64),
    REXX_METHOD(oosqlstmt_columnName,                 oosqlstmt_columnName),
    REXX_METHOD(oosqlstmt_columnOriginName,           oosqlstmt_columnOriginName),
    REXX_METHOD(oosqlstmt_columnTableName,            oosqlstmt_columnTableName),
    REXX_METHOD(oosqlstmt_columnText,                 oosqlstmt_columnText),
    REXX_METHOD(oosqlstmt_columnType,                 oosqlstmt_columnType),
    REXX_METHOD(oosqlstmt_columnValue,                oosqlstmt_columnValue),
    REXX_METHOD(oosqlstmt_dataCount,                  oosqlstmt_dataCount),
    REXX_METHOD(oosqlstmt_dbHandle,                   oosqlstmt_dbHandle),
    REXX_METHOD(oosqlstmt_finalize,                   oosqlstmt_finalize),
    REXX_METHOD(oosqlstmt_reset,                      oosqlstmt_reset),
    REXX_METHOD(oosqlstmt_sql,                        oosqlstmt_sql),
    REXX_METHOD(oosqlstmt_step,                       oosqlstmt_step),
    REXX_METHOD(oosqlstmt_stmtBusy,                   oosqlstmt_stmtBusy),
    REXX_METHOD(oosqlstmt_stmtReadonly,               oosqlstmt_stmtReadonly),
    REXX_METHOD(oosqlstmt_stmtStatus,                 oosqlstmt_stmtStatus),
    REXX_METHOD(oosqlstmt_value,                      oosqlstmt_value),

    // .ooSQLiteBackup
    REXX_METHOD(oosqlbu_getFinished_atr,              oosqlbu_getFinished_atr),
    REXX_METHOD(oosqlbu_getInitCode_atr,              oosqlbu_getInitCode_atr),
    REXX_METHOD(oosqlbu_getLastErrCode_atr,           oosqlbu_getLastErrCode_atr),
    REXX_METHOD(oosqlbu_getLastErrMsg_atr,            oosqlbu_getLastErrMsg_atr),
    REXX_METHOD(oosqlbu_getPageCount_atr,             oosqlbu_getPageCount_atr),
    REXX_METHOD(oosqlbu_getRemaining_atr,             oosqlbu_getRemaining_atr),
    REXX_METHOD(oosqlbu_getSaveDestConn_atr,          oosqlbu_getSaveDestConn_atr),
    REXX_METHOD(oosqlbu_setSaveDestConn_atr,          oosqlbu_setSaveDestConn_atr),

    REXX_METHOD(oosqlbu_init,                         oosqlbu_init),
    REXX_METHOD(oosqlbu_uninit,                       oosqlbu_uninit),

    REXX_METHOD(oosqlbu_finish,                       oosqlbu_finish),
    REXX_METHOD(oosqlbu_getDestConn,                  oosqlbu_getDestConn),
    REXX_METHOD(oosqlbu_step,                         oosqlbu_step),

    // .ooSQLiteMutex
    REXX_METHOD(oosqlmtx_init_cls,                    oosqlmtx_init_cls),

    REXX_METHOD(oosqlmtx_getClosed_atr,               oosqlmtx_getClosed_atr),
    REXX_METHOD(oosqlmtx_getIsNull_atr,               oosqlmtx_getIsNull_atr),

    REXX_METHOD(oosqlmtx_init,                        oosqlmtx_init),
    REXX_METHOD(oosqlmtx_uninit,                      oosqlmtx_uninit),

    REXX_METHOD(oosqlmtx_enter,                       oosqlmtx_enter),
    REXX_METHOD(oosqlmtx_free,                        oosqlmtx_free),
    REXX_METHOD(oosqlmtx_leave,                       oosqlmtx_leave),
    REXX_METHOD(oosqlmtx_try,                         oosqlmtx_try),

    // .ooSQLExtensions
    REXX_METHOD(oosqlext_init_cls,                    oosqlext_init_cls),

    REXX_METHOD(oosqlext_getLastErrCode_atr,          oosqlext_getLastErrCode_atr),
    REXX_METHOD(oosqlext_getLastErrMsg_atr,           oosqlext_getLastErrMsg_atr),

    REXX_METHOD(oosqlext_loadLibrary_cls,             oosqlext_loadLibrary_cls),
    REXX_METHOD(oosqlext_loadPackage_cls,             oosqlext_loadPackage_cls),

    // .ooSQLPackage
    REXX_METHOD(oosqlpack_init,                       oosqlpack_init),

    REXX_METHOD(oosqlpack_getLastErrCode_atr,         oosqlpack_getLastErrCode_atr),
    REXX_METHOD(oosqlpack_getLastErrMsg_atr,          oosqlpack_getLastErrMsg_atr),

    // .ooSQLLibrary
    REXX_METHOD(oosqllib_init,                        oosqllib_init),

    REXX_METHOD(oosqllib_getLastErrCode_atr,          oosqllib_getLastErrCode_atr),
    REXX_METHOD(oosqllib_getLastErrMsg_atr,           oosqllib_getLastErrMsg_atr),

    // __rtn_helper_class
    REXX_METHOD(hlpr_init_cls,                        hlpr_init_cls),

    // __rtn_db_callback_class
    REXX_METHOD(db_cb_releaseBuffer,                  db_cb_releaseBuffer),

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

OOREXX_GET_PACKAGE(ooSQLite);
