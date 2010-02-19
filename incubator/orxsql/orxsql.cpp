/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2010-2010 Rexx Language Association. All rights reserved.    */
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
/* Authors;                                                                   */
/*       W. David Ashley <dashley@us.ibm.com>                                 */
/*                                                                            */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*  Note:                                                                     */
/*                                                                            */
/*  The code in this file conforms to the ISO/IEC 9075-3: "SQL Call-Level     */
/*  Interface", the ODBC V3.5 updates and the ooRexx 4.0 API. Any database or */
/*  driver that supplies a CLI that conforms to this specification should be  */
/*  able to utilize this source code.                                         */
/*                                                                            */
/*----------------------------------------------------------------------------*/


/* include the specific driver header files. These might need to be changed   */
/* if the CLI being used does not support them. Windows & Linux support       */
/* these names.                                                               */
#include <sql.h>
#include <sqlext.h>

/* include the standard C headers */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <dlfcn.h>
#include <oorexxapi.h>


/*----------------------------------------------------------------------------*/
/* Global variables                                                           */
/*----------------------------------------------------------------------------*/

static SQLRETURN SQL_API (*instSQLAllocHandle)(SQLSMALLINT HandleType,
                                               SQLHANDLE InputHandle,
                                               SQLHANDLE *OutputHandle) = NULL;
static SQLRETURN SQL_API (*instSQLSetEnvAttr)(SQLHENV EnvironmentHandle,
                                              SQLINTEGER Attribute, SQLPOINTER Value,
                                              SQLINTEGER StringLength) = NULL;
static SQLRETURN SQL_API (*instSQLGetEnvAttr)(SQLHENV EnvironmentHandle,
                                              SQLINTEGER Attribute, SQLPOINTER Value,
                                              SQLINTEGER BufferLength,
                                              SQLINTEGER *StringLength) = NULL;
static SQLRETURN SQL_API (*instSQLConnect)(SQLHDBC ConnectionHandle,
                                           SQLCHAR *ServerName, SQLSMALLINT NameLength1,
                                           SQLCHAR *UserName, SQLSMALLINT NameLength2,
                                           SQLCHAR *Authentication,
                                           SQLSMALLINT NameLength3) = NULL;
static SQLRETURN SQL_API (*instSQLDisconnect)(SQLHDBC ConnectionHandle) = NULL;
static SQLRETURN SQL_API (*instSQLSetConnectAttr)(SQLHDBC ConnectionHandle,
                                                  SQLINTEGER Attribute, SQLPOINTER Value,
                                                  SQLINTEGER StringLength) = NULL;
static SQLRETURN SQL_API (*instSQLGetConnectAttr)(SQLHDBC ConnectionHandle,
                                                  SQLINTEGER Attribute, SQLPOINTER Value,
                                                  SQLINTEGER BufferLength,
                                                  SQLINTEGER *StringLength) = NULL;
static SQLRETURN SQL_API (*instSQLEndTran)(SQLSMALLINT HandleType, SQLHANDLE Handle,
                                           SQLSMALLINT CompletionType) = NULL;
static SQLRETURN SQL_API (*instSQLFreeHandle)(SQLSMALLINT HandleType,
                                              SQLHANDLE Handle) = NULL;
static SQLRETURN SQL_API (*instSQLGetInfo)(SQLHDBC ConnectionHandle, SQLUSMALLINT InfoType,
                                           SQLPOINTER InfoValue, SQLSMALLINT BufferLength,
                                           SQLSMALLINT *StringLength) = NULL;
static SQLRETURN SQL_API (*instSQLExecute)(SQLHSTMT StatementHandle) = NULL;
static SQLRETURN SQL_API (*instSQLExecDirect)(SQLHSTMT StatementHandle,
                                              SQLCHAR *StatementText,
                                              SQLINTEGER TextLength) = NULL;
static SQLRETURN SQL_API (*instSQLBindParam)(SQLHSTMT StatementHandle,
                                             SQLUSMALLINT ParameterNumber, SQLSMALLINT ValueType,
                                             SQLSMALLINT ParameterType, SQLULEN LengthPrecision,
                                             SQLSMALLINT ParameterScale, SQLPOINTER ParameterValue,
                                             SQLLEN *StrLen_or_Ind) = NULL;
static SQLRETURN SQL_API (*instSQLPrepare)(SQLHSTMT StatementHandle,
                                           SQLCHAR *StatementText,
                                           SQLINTEGER TextLength) = NULL;
static SQLRETURN SQL_API (*instSQLError)(SQLHENV EnvironmentHandle,
                                         SQLHDBC ConnectionHandle, SQLHSTMT StatementHandle,
                                         SQLCHAR *Sqlstate, SQLINTEGER *NativeError,
                                         SQLCHAR *MessageText, SQLSMALLINT BufferLength,
                                         SQLSMALLINT *TextLength) = NULL;
static SQLRETURN SQL_API (*instSQLFetch)(SQLHSTMT StatementHandle) = NULL;
static SQLRETURN SQL_API (*instSQLNumResultCols)(SQLHSTMT StatementHandle,
                                                 SQLSMALLINT *ColumnCount) = NULL;
static SQLRETURN SQL_API (*instSQLGetData)(SQLHSTMT StatementHandle,
                                           SQLUSMALLINT ColumnNumber, SQLSMALLINT TargetType,
                                           SQLPOINTER TargetValue, SQLLEN BufferLength,
                                           SQLLEN *StrLen_or_Ind) = NULL;
static SQLRETURN SQL_API (*instSQLDescribeCol)(SQLHSTMT StatementHandle,
                                               SQLUSMALLINT ColumnNumber, SQLCHAR *ColumnName,
                                               SQLSMALLINT BufferLength, SQLSMALLINT *NameLength,
                                               SQLSMALLINT *DataType, SQLULEN *ColumnSize,
                                               SQLSMALLINT *DecimalDigits, SQLSMALLINT *Nullable) = NULL;


/*----------------------------------------------------------------------------*/
/* Local Definitions/variables                                                */
/*----------------------------------------------------------------------------*/

#define VERSTRING(major,minor,rel) #major "." #minor "." #rel

#define SETDIAG(retc, henv, hdbc, hstmt) \
    if (retc != SQL_SUCCESS) { \
        SQLCHAR sqlstate[SQL_SQLSTATE_SIZE + 1]; \
        SQLINTEGER sqlcode; \
        SQLSMALLINT msglen; \
        SQLCHAR msgbuffer[SQL_MAX_MESSAGE_LENGTH + 1]; \
        char msgbuf[SQL_MAX_MESSAGE_LENGTH + 30]; \
                                                  \
        context->SetObjectVariable("SQLDIAG", context->NewArray(0)); \
        RexxArrayObject arr = (RexxArrayObject)context->GetObjectVariable("SQLDIAG"); \
        snprintf(msgbuf, sizeof(msgbuf), "ooRexx Method: %s, SQL Return Code %d.", \
                 context->GetMessageName(), retc); \
        context->ArrayAppendString(arr, msgbuf, strlen(msgbuf)); \
        while (instSQLError(henv, hdbc, hstmt, sqlstate, &sqlcode, msgbuffer, \
                            sizeof(msgbuffer), &msglen) == SQL_SUCCESS) { \
            snprintf(msgbuf, sizeof(msgbuf), "SQLCODE: %d, SQLSTATE: %s", \
                     sqlcode, (char *)sqlstate); \
            context->ArrayAppendString(arr, msgbuf, strlen(msgbuf)); \
            snprintf(msgbuf, sizeof(msgbuf), "Message: %s", (char *)msgbuffer); \
            context->ArrayAppendString(arr, msgbuf, strlen(msgbuf)); \
        } \
    } \
    else { \
        context->SetObjectVariable("SQLDIAG", context->NewArray(0)); \
    }

typedef struct _dbself {
#if defined WIN32 || defined WIN64
    HMODULE handle;
#else
    void *handle;
#endif
    SQLHENV henv;
    SQLHDBC hdbc;
} dbself;

typedef struct _stmtself {
    dbself *pdbself;
    SQLHSTMT hstmt;
} stmtself;


/*============================================================================*/
/* Private Functions                                                          */
/*============================================================================*/


/*============================================================================*/
/* Public Functions                                                           */
/*============================================================================*/


/*============================================================================*/
/* Private Methods                                                            */
/*============================================================================*/


/*============================================================================*/
/* Public Methods                                                             */
/*============================================================================*/

/**
 * Method:  OrxDB_isbitset
 *
 * Return true if a bit is set.
 *
 * @param field   Field value to test.
 *
 * @param bit     The bit to test.
 *
 * @return        Zero.
 **/
RexxMethod2(logical_t,                 // Return type
            OrxDB_isbitset,            // Object_method name
            uint32_t, field,
            uint32_t, bit)
{

    return (field & bit);
}

/**
 * Method:  OrxDB_version
 *
 * Return the OrxSQL version string.
 *
 * @return        Zero.
 **/
RexxMethod0(RexxObjectPtr,             // Return type
            OrxDB_version)             // Object_method name
{
    
    return (RexxObjectPtr)context->NewStringFromAsciiz(VERSTRING(VMAJOR, VMINOR, VREL));
}

/**
 * Method:  OrxDB_dbinit
 *
 * Init the database class.
 *
 * @return        Zero.
 **/
RexxMethod1(int,                       // Return type
            OrxDB_dbinit,              // Object_method name
            CSTRING, libname)          // CLI library name
{
    dbself *pself;

    // allocate memory for our self data
    pself = (dbself *)malloc(sizeof(dbself));
    memset(pself, 0, sizeof(dbself));
    context->SetObjectVariable("CSELF", context->NewPointer(pself));
    
    // allocate the sql environment if not already allocated
    if (pself->henv == SQL_NULL_HANDLE) {
        // if necessary, load the library
#if defined WIN32 || defined WIN64
        pself->handle = LoadLibrary(libname);
#else
        pself->handle = dlopen(libname, RTLD_LAZY);
#endif
        if (!pself->handle) {
            context->RaiseException1(Rexx_Error_System_resources_user_defined,
                                     (RexxObjectPtr)context->String("Could not load CLI library."));
            return 0;
        }
        // if necessary, get the symbol(s)
        if (instSQLAllocHandle == SQL_NULL_HANDLE) {
#if defined WIN32 || defined WIN64
            instSQLAllocHandle = GetProcAddress(pself->handle, "SQLAllocHandle");
#else
            *(void**)(&instSQLAllocHandle) = dlsym(pself->handle, "SQLAllocHandle");
#endif
            if (!instSQLAllocHandle) {
                context->RaiseException1(Rexx_Error_System_resources_user_defined,
                                         (RexxObjectPtr)context->String("Could not load symbol SQLAllocHandle from CLI library."));
                return 0;  
            }
        }
        if (instSQLError == SQL_NULL_HANDLE) {
#if defined WIN32 || defined WIN64
            instSQLError = GetProcAddress(pself->handle, "SQLError");
#else
            *(void**)(&instSQLError) = dlsym(pself->handle, "SQLError");
#endif
            if (!instSQLError) {
                context->RaiseException1(Rexx_Error_System_resources_user_defined,
                                         (RexxObjectPtr)context->String("Could not load symbol SQLError from CLI library."));
                return 0;  
            }
        }
        SQLRETURN retc = (*instSQLAllocHandle)(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &pself->henv);
        context->SetObjectVariable("SQLRETURN", context->Int32((int32_t) retc));
        SETDIAG(retc, pself->henv, pself->hdbc, SQL_NULL_HANDLE);
    }

    return 0;
}

/**
 * Method:  OrxDB_set_env_attr
 *
 * Set an attribute.
 *
 * @param attr    Attribute name.
 *
 * @param val     Attribute value.
 *
 * @return        Return code.
 **/
RexxMethod3(int,                       // Return type
            OrxDB_set_env_attr,        // Object_method name
            int, attr,
            RexxObjectPtr, val,
            CSELF, cself)
{
    dbself *pself = (dbself *)cself;
    SQLRETURN retc;
    int32_t itmp;
    uint32_t utmp;

    // if necessary, get the symbol(s)
    if (instSQLSetEnvAttr == SQL_NULL_HANDLE) {
#if defined WIN32 || defined WIN64
        instSQLSetEnvAttr = GetProcAddress(pself->handle, "SQLSetEnvAttr");
#else
        *(void**)(&instSQLSetEnvAttr) = dlsym(pself->handle, "SQLSetEnvAttr");
#endif
        if (!instSQLSetEnvAttr) {
            context->RaiseException1(Rexx_Error_System_resources_user_defined,
                                     (RexxObjectPtr)context->String("Could not load symbol SQLSetEnvAttr from CLI library."));
            return 0;  
        }
    }
    //
    switch (attr) {
    case SQL_ATTR_OUTPUT_NTS:
#if defined WIN32 || defined WIN64
    case SQL_ATTR_ODBC_VERSION:
#endif
        context->ObjectToInt32(val, &itmp);
        retc = (*instSQLSetEnvAttr)(pself->henv, (SQLINTEGER)attr, (SQLPOINTER)&itmp, 0);
        break;
#if defined WIN32 || defined WIN64
    case SQL_ATTR_CONNECTION_POOLING:
    case SQL_ATTR_CP_MATCH:
        context->ObjectToUnsignedInt32(val, &utmp);
        retc = (*instSQLSetEnvAttr)(pself->henv, (SQLINTEGER)attr, (SQLPOINTER)&utmp, 0);
        break;
#endif
    default:
        context->RaiseException1(Rexx_Error_Invalid_argument_general,
                                 (RexxObjectPtr)context->String("1 unsupported."));
        return 0;  
    }
    context->SetObjectVariable("SQLRETURN", context->Int32((int32_t) retc));
    SETDIAG(retc, pself->henv, pself->hdbc, SQL_NULL_HANDLE);
    
    return 0;
}

/**
 * Method:  OrxDB_get_env_attr
 *
 * Get an attribute.
 *
 * @param attr    Attribute name.
 *
 * @return        Attribute value.
 **/
RexxMethod2(RexxObjectPtr,             // Return type
            OrxDB_get_env_attr,        // Object_method name
            int, attr,
            CSELF, cself)
{
    dbself *pself = (dbself *)cself;
    SQLRETURN retc;
    int32_t itmp;
    uint32_t utmp;

    // if necessary, get the symbol(s)
    if (instSQLGetEnvAttr == SQL_NULL_HANDLE) {
#if defined WIN32 || defined WIN64
        instSQLGetEnvAttr = GetProcAddress(pself->handle, "SQLGetEnvAttr");
#else
        *(void**)(&instSQLGetEnvAttr) = dlsym(pself->handle, "SQLGetEnvAttr");
#endif
        if (!instSQLGetEnvAttr) {
            context->RaiseException1(Rexx_Error_System_resources_user_defined,
                                     (RexxObjectPtr)context->String("Could not load symbol SQLGetEnvAttr from CLI library."));
            return 0;  
        }
    }
    //
    switch (attr) {
    case SQL_ATTR_OUTPUT_NTS:
#if defined WIN32 || defined WIN64
    case SQL_ATTR_ODBC_VERSION:
#endif
        retc = (*instSQLGetEnvAttr)(pself->henv, (SQLINTEGER)attr, (SQLPOINTER)&itmp, 0, 0);
        context->SetObjectVariable("SQLRETURN", context->Int32((int32_t) retc));
        SETDIAG(retc, pself->henv, pself->hdbc, SQL_NULL_HANDLE);
        return context->Int32(itmp);
#if defined WIN32 || defined WIN64
    case SQL_ATTR_CONNECTION_POOLING:
    case SQL_ATTR_CP_MATCH:
        retc = (*instSQLGetEnvAttr)(pself->henv, (SQLINTEGER)attr, (SQLPOINTER)&utmp, 0, 0);
        context->SetObjectVariable("SQLRETURN", context->Int32((int32_t) retc));
        SETDIAG(retc, pself->henv, pself->hdbc, SQL_NULL_HANDLE);
        return context->UnsignedInt32(utmp);
#endif
    default:
        context->RaiseException1(Rexx_Error_Invalid_argument_general,
                                 (RexxObjectPtr)context->String("1 unsupported."));
        return 0;  
    }

    return (RexxObjectPtr)context->Nil();
}

/**
 * Method:  OrxDB_connect
 *
 * Connect to the database.
 *
 * @param connx   Connection string.
 *
 * @return        Boolean return code.
 **/
RexxMethod4(int,                       // Return type
            OrxDB_connect,             // Object_method name
            CSTRING, dbname,
            OPTIONAL_CSTRING, user,
            OPTIONAL_CSTRING, pw,
            CSELF, cself)
{
    dbself *pself = (dbself *)cself;
    SQLRETURN retc;
    
    if (pself->hdbc == SQL_NULL_HANDLE) {
        // if necessary, get the symbol(s)
        if (instSQLConnect == SQL_NULL_HANDLE) {
#if defined WIN32 || defined WIN64
            instSQLConnect = GetProcAddress(pself->handle, "SQLConnect");
#else
            *(void**)(&instSQLConnect) = dlsym(pself->handle, "SQLConnect");
#endif
            if (!instSQLConnect) {
                context->RaiseException1(Rexx_Error_System_resources_user_defined,
                                         (RexxObjectPtr)context->String("Could not load symbol SQLConnect from CLI library."));
                return 0;  
            }
        }
        // allocate a connection handle
        retc = (*instSQLAllocHandle)(SQL_HANDLE_DBC, pself->henv, &pself->hdbc);
    }
    // connect to the database
    retc = (*instSQLConnect)(pself->hdbc, (SQLCHAR *)dbname, strlen(dbname),
                            (SQLCHAR *)user, strlen(user), (SQLCHAR *)pw, strlen(pw));
    context->SetObjectVariable("SQLRETURN", context->Int32((int32_t) retc));
    SETDIAG(retc, pself->henv, pself->hdbc, SQL_NULL_HANDLE);
    return 0;
}

/**
 * Method:  OrxDB_close
 *
 * Close the database connection.
 *
 * @return        Return code.
 **/
RexxMethod1(int,                       // Return type
            OrxDB_close,               // Object_method name
            CSELF, cself)
{
    dbself *pself = (dbself *)cself;
    
    // if necessary, get the symbol(s)
    if (instSQLDisconnect == SQL_NULL_HANDLE) {
#if defined WIN32 || defined WIN64
        instSQLDisconnect = GetProcAddress(pself->handle, "SQLDisconnect");
#else
        *(void**)(&instSQLDisconnect) = dlsym(pself->handle, "SQLDisconnect");
#endif
        if (!instSQLDisconnect) {
            context->RaiseException1(Rexx_Error_System_resources_user_defined,
                                     (RexxObjectPtr)context->String("Could not load symbol SQLDisconnect from CLI library."));
            return 0;  
        }
    }
    // disconnect from the database
    SQLRETURN retc = (*instSQLDisconnect)(pself->hdbc);
    context->SetObjectVariable("SQLRETURN", context->Int32((int32_t) retc));
    SETDIAG(retc, pself->henv, pself->hdbc, SQL_NULL_HANDLE);
    // do not free the hdbc or the henv handle!
    return 0;
}

/**
 * Method:  OrxDB_set_connx_attr
 *
 * Set an attribute.
 *
 * @param attr    Attribute name.
 *
 * @param val     Attribute value.
 *
 * @return        Return code.
 **/
RexxMethod3(int,                       // Return type
            OrxDB_set_connx_attr,      // Object_method name
            int, attr,
            RexxObjectPtr, val,
            CSELF, cself)
{
    dbself *pself = (dbself *)cself;
    SQLRETURN retc;
    const char *ctmp;
    uint32_t utmp;

    // if necessary, get the symbol(s)
    if (instSQLSetConnectAttr == SQL_NULL_HANDLE) {
#if defined WIN32 || defined WIN64
        instSQLSetConnectAttr = GetProcAddress(pself->handle, "SQLSetConnectAttr");
#else
        *(void**)(&instSQLSetConnectAttr) = dlsym(pself->handle, "SQLSetConnectAttr");
#endif
        if (!instSQLSetConnectAttr) {
            context->RaiseException1(Rexx_Error_System_resources_user_defined,
                                     (RexxObjectPtr)context->String("Could not load symbol SQLSetConnectAttr from CLI library."));
            return 0;  
        }
    }
    //
    switch (attr) {
    case SQL_ATTR_CURRENT_CATALOG:
    case SQL_ATTR_TRACEFILE:
    case SQL_ATTR_TRANSLATE_LIB:
        ctmp = context->CString(val);
        retc = (*instSQLSetConnectAttr)(pself->hdbc, (SQLINTEGER)attr, (SQLPOINTER)ctmp, strlen(ctmp));
        break;
    case SQL_ATTR_ACCESS_MODE:
#if defined WIN32 || defined WIN64
    case SQL_ATTR_ASYNC_DBC_FUNCTIONS_ENABLE:
#endif
    case SQL_ATTR_ASYNC_ENABLE:
    case SQL_ATTR_AUTO_IPD:
    case SQL_ATTR_AUTOCOMMIT:
    case SQL_ATTR_CONNECTION_DEAD:
    case SQL_ATTR_CONNECTION_TIMEOUT:
    case SQL_ATTR_ENLIST_IN_DTC:
    case SQL_ATTR_LOGIN_TIMEOUT:
    case SQL_ATTR_METADATA_ID:
    case SQL_ATTR_ODBC_CURSORS:
    case SQL_ATTR_PACKET_SIZE:
    case SQL_ATTR_TRACE:
    case SQL_ATTR_TRANSLATE_OPTION:
    case SQL_ATTR_TXN_ISOLATION:
        context->ObjectToUnsignedInt32(val, &utmp);
        retc = (*instSQLSetConnectAttr)(pself->hdbc, (SQLINTEGER)attr, (SQLPOINTER)&utmp, 0);
        break;
    default:
        context->RaiseException1(Rexx_Error_Invalid_argument_general,
                                 (RexxObjectPtr)context->String("1 unsupported."));
        return 0;  
    }
    context->SetObjectVariable("SQLRETURN", context->Int32((int32_t) retc));
    SETDIAG(retc, pself->henv, pself->hdbc, SQL_NULL_HANDLE);
    
    return 0;
}

/**
 * Method:  OrxDB_get_connx_attr
 *
 * Get an attribute.
 *
 * @param attr    Attribute name.
 *
 * @return        Attribute value.
 **/
RexxMethod2(RexxObjectPtr,             // Return type
            OrxDB_get_connx_attr,      // Object_method name
            int, attr,
            CSELF, cself)
{
    dbself *pself = (dbself *)cself;
    SQLRETURN retc;
    char *ctmp;
    uint32_t utmp;
    int32_t itmp = 1;

    // if necessary, get the symbol(s)
    if (instSQLGetConnectAttr == SQL_NULL_HANDLE) {
#if defined WIN32 || defined WIN64
        instSQLGetConnectAttr = GetProcAddress(pself->handle, "SQLGetConnectAttr");
#else
        *(void**)(&instSQLGetConnectAttr) = dlsym(pself->handle, "SQLGetConnectAttr");
#endif
        if (!instSQLGetConnectAttr) {
            context->RaiseException1(Rexx_Error_System_resources_user_defined,
                                     (RexxObjectPtr)context->String("Could not load symbol SQLGetConnectAttr from CLI library."));
            return 0;  
        }
    }
    //
    switch (attr) {
    case SQL_ATTR_CURRENT_CATALOG:
    case SQL_ATTR_TRACEFILE:
    case SQL_ATTR_TRANSLATE_LIB:
        retc = (*instSQLGetConnectAttr)(pself->henv, (SQLINTEGER)attr, (SQLPOINTER)ctmp, 0, &itmp);
        context->SetObjectVariable("SQLRETURN", context->Int32((int32_t) retc));
        SETDIAG(retc, pself->henv, pself->hdbc, SQL_NULL_HANDLE);
        return context->String(ctmp, (size_t)itmp);
    case SQL_ATTR_ACCESS_MODE:
#if defined WIN32 || defined WIN64
    case SQL_ATTR_ASYNC_DBC_FUNCTIONS_ENABLE:
#endif
    case SQL_ATTR_ASYNC_ENABLE:
    case SQL_ATTR_AUTO_IPD:
    case SQL_ATTR_AUTOCOMMIT:
    case SQL_ATTR_CONNECTION_DEAD:
    case SQL_ATTR_CONNECTION_TIMEOUT:
    case SQL_ATTR_ENLIST_IN_DTC:
    case SQL_ATTR_LOGIN_TIMEOUT:
    case SQL_ATTR_METADATA_ID:
    case SQL_ATTR_ODBC_CURSORS:
    case SQL_ATTR_PACKET_SIZE:
    case SQL_ATTR_TRACE:
    case SQL_ATTR_TRANSLATE_OPTION:
    case SQL_ATTR_TXN_ISOLATION:
        retc = (*instSQLGetConnectAttr)(pself->henv, (SQLINTEGER)attr, (SQLPOINTER)&utmp, 0, 0);
        context->SetObjectVariable("SQLRETURN", context->Int32((int32_t) retc));
        SETDIAG(retc, pself->henv, pself->hdbc, SQL_NULL_HANDLE);
        return context->UnsignedInt32(utmp);
    default:
        context->RaiseException1(Rexx_Error_Invalid_argument_general,
                                 (RexxObjectPtr)context->String("1 unsupported."));
        return 0;  
    }

    return (RexxObjectPtr)context->Nil();
}

/**
 * Method:  OrxDB_commit
 *
 * Commit a database transaction.
 *
 * @return        Return code.
 **/
RexxMethod1(int,                       // Return type
            OrxDB_commit,              // Object_method name
            CSELF, cself)
{
    dbself *pself = (dbself *)cself;

    // if necessary, get the symbol(s)
    if (instSQLEndTran == SQL_NULL_HANDLE) {
#if defined WIN32 || defined WIN64
        instSQLEndTran = GetProcAddress(pself->handle, "SQLEndTran");
#else
        *(void**)(&instSQLEndTran) = dlsym(pself->handle, "SQLEndTran");
#endif
        if (!instSQLEndTran) {
            context->RaiseException1(Rexx_Error_System_resources_user_defined,
                                     (RexxObjectPtr)context->String("Could not load symbol SQLEndTran from CLI library."));
            return 0;  
        }
    }
    // commit the transaction
    SQLRETURN retc = (*instSQLEndTran)(SQL_HANDLE_ENV, pself->henv, SQL_COMMIT);
    context->SetObjectVariable("SQLRETURN", context->Int32((int32_t) retc));
    SETDIAG(retc, pself->henv, pself->hdbc, SQL_NULL_HANDLE);
    
    return 0;
}

/**
 * Method:  OrxDB_rollback
 *
 * Roll back a database transaction.
 *
 * @return        Return code.
 **/
RexxMethod1(int,                       // Return type
            OrxDB_rollback,            // Object_method name
            CSELF, cself)
{
    dbself *pself = (dbself *)cself;

    // if necessary, get the symbol(s)
    if (instSQLEndTran == SQL_NULL_HANDLE) {
#if defined WIN32 || defined WIN64
        instSQLEndTran = GetProcAddress(pself->handle, "SQLEndTran");
#else
        *(void**)(&instSQLEndTran) = dlsym(pself->handle, "SQLEndTran");
#endif
        if (!instSQLEndTran) {
            context->RaiseException1(Rexx_Error_System_resources_user_defined,
                                     (RexxObjectPtr)context->String("Could not load symbol SQLEndTran from CLI library."));
            return 0;  
        }
    }
    // rollback the transaction
    SQLRETURN retc = (*instSQLEndTran)(SQL_HANDLE_ENV, pself->henv, SQL_ROLLBACK);
    context->SetObjectVariable("SQLRETURN", context->Int32((int32_t) retc));
    SETDIAG(retc, pself->henv, pself->hdbc, SQL_NULL_HANDLE);

    return 0;
}

/**
 * Method:  OrxDB_check_connx
 *
 * Is the database connection live?
 *
 * @return        Boolean return code.
 **/
RexxMethod1(logical_t,                 // Return type
            OrxDB_check_connx,         // Object_method name
            CSELF, cself)
{
    dbself *pself = (dbself *)cself;
    
    if (pself->hdbc == SQL_NULL_HANDLE) {
        return 0;
    }
    return 1;
}

/**
 * Method:  OrxDB_destroy
 *
 * Destroy a database instance.
 *
 * @return        Return code.
 **/
RexxMethod1(int,                       // Return type
            OrxDB_destroy,             // Object_method name
            CSELF, cself)
{
    dbself *pself = (dbself *)cself;
    
    // if necessary, get the symbol(s)
    if (instSQLFreeHandle == SQL_NULL_HANDLE) {
#if defined WIN32 || defined WIN64
        instSQLFreeHandle = GetProcAddress(pself->handle, "SQLFreeHandle");
#else
        *(void**)(&instSQLFreeHandle) = dlsym(pself->handle, "SQLFreeHandle");
#endif
        if (!instSQLFreeHandle) {
            context->RaiseException1(Rexx_Error_System_resources_user_defined,
                                     (RexxObjectPtr)context->String("Could not load symbol SQLFreeHandle from CLI library."));
            return 0;  
        }
    }
    if (pself->hdbc != NULL) {
        SQLRETURN retc = (*instSQLFreeHandle)(SQL_HANDLE_DBC, &pself->hdbc);
        SETDIAG(retc, pself->henv, pself->hdbc, SQL_NULL_HANDLE);
    }
    if (pself->henv != NULL) {
        SQLRETURN retc = (*instSQLFreeHandle)(SQL_HANDLE_ENV, &pself->henv);
        SETDIAG(retc, pself->henv, pself->hdbc, SQL_NULL_HANDLE);
    }
    if (pself != NULL) {
        free(pself);
    }
    return 0;
}

/**
 * Method:  OrxDB_get_info
 *
 * Get information.
 *
 * @param attr    Attribute name.
 *
 * @return        Attribute value.
 **/
RexxMethod2(RexxObjectPtr,             // Return type
            OrxDB_get_info,            // Object_method name
            int, attr,
            CSELF, cself)
{
    dbself *pself = (dbself *)cself;
    SQLRETURN retc;
    char ctmp[1024];
    SQLSMALLINT buflen = 1;
    SQLUSMALLINT u16tmp;
    SQLUINTEGER u32tmp;

    // if necessary, get the symbol(s)
    if (instSQLGetInfo == SQL_NULL_HANDLE) {
#if defined WIN32 || defined WIN64
        instSQLGetInfo = GetProcAddress(pself->handle, "SQLGetInfo");
#else
        *(void**)(&instSQLGetInfo) = dlsym(pself->handle, "SQLGetInfo");
#endif
        *(void**)(&instSQLGetInfo) = dlsym(pself->handle, "SQLGetInfo");
        if (!instSQLGetInfo) {
            context->RaiseException1(Rexx_Error_System_resources_user_defined,
                                     (RexxObjectPtr)context->String("Could not load symbol SQLGetInfo from CLI library."));
            return 0;  
        }
    }
    //
    switch (attr) {
    case SQL_ACCESSIBLE_PROCEDURES:
    case SQL_ACCESSIBLE_TABLES:
    case SQL_CATALOG_NAME:
    case SQL_CATALOG_NAME_SEPARATOR:
    case SQL_CATALOG_TERM:
    case SQL_COLLATION_SEQ:
    case SQL_COLUMN_ALIAS:
    case SQL_DATA_SOURCE_NAME:
    case SQL_DATA_SOURCE_READ_ONLY:
    case SQL_DATABASE_NAME:
    case SQL_DBMS_NAME:
    case SQL_DBMS_VER:
    case SQL_DESCRIBE_PARAMETER:
    case SQL_DM_VER:
    case SQL_DRIVER_NAME:
    case SQL_DRIVER_ODBC_VER:
    case SQL_DRIVER_VER:
    case SQL_EXPRESSIONS_IN_ORDERBY:
    case SQL_IDENTIFIER_QUOTE_CHAR:
    case SQL_INTEGRITY:
    case SQL_KEYWORDS:
    case SQL_LIKE_ESCAPE_CLAUSE:
    case SQL_MAX_ROW_SIZE_INCLUDES_LONG:
    case SQL_MULT_RESULT_SETS:
    case SQL_MULTIPLE_ACTIVE_TXN:
    case SQL_NEED_LONG_DATA_LEN:
    case SQL_ODBC_VER:
    case SQL_ORDER_BY_COLUMNS_IN_SELECT:
    case SQL_PROCEDURE_TERM:
    case SQL_PROCEDURES:
    case SQL_ROW_UPDATES:
    case SQL_SCHEMA_TERM:
    case SQL_SEARCH_PATTERN_ESCAPE:
    case SQL_SERVER_NAME:
    case SQL_SPECIAL_CHARACTERS:
    case SQL_TABLE_TERM:
    case SQL_USER_NAME:
    case SQL_XOPEN_CLI_YEAR:
        retc = (*instSQLGetInfo)(pself->hdbc, (SQLUSMALLINT)attr, (SQLPOINTER)ctmp, sizeof(ctmp), &buflen);
        context->SetObjectVariable("SQLRETURN", context->Int32((int32_t) retc));
        SETDIAG(retc, pself->henv, pself->hdbc, SQL_NULL_HANDLE);
        return context->String(ctmp, (size_t)buflen);
    case SQL_ACTIVE_ENVIRONMENTS:
    case SQL_CATALOG_LOCATION:
    case SQL_CONCAT_NULL_BEHAVIOR:
    case SQL_CORRELATION_NAME:
    case SQL_CURSOR_COMMIT_BEHAVIOR:
    case SQL_CURSOR_ROLLBACK_BEHAVIOR:
    case SQL_FILE_USAGE:
    case SQL_GROUP_BY:
    case SQL_IDENTIFIER_CASE:
    case SQL_MAX_CATALOG_NAME_LEN:
    case SQL_MAX_COLUMN_NAME_LEN:
    case SQL_MAX_COLUMNS_IN_GROUP_BY:
    case SQL_MAX_COLUMNS_IN_INDEX:
    case SQL_MAX_COLUMNS_IN_ORDER_BY:
    case SQL_MAX_COLUMNS_IN_SELECT:
    case SQL_MAX_CONCURRENT_ACTIVITIES:
    case SQL_MAX_CURSOR_NAME_LEN:
    case SQL_MAX_DRIVER_CONNECTIONS:
    case SQL_MAX_IDENTIFIER_LEN:
    case SQL_MAX_PROCEDURE_NAME_LEN:
    case SQL_MAX_SCHEMA_NAME_LEN:
    case SQL_MAX_TABLE_NAME_LEN:
    case SQL_MAX_TABLES_IN_SELECT:
    case SQL_MAX_USER_NAME_LEN:
    case SQL_NON_NULLABLE_COLUMNS:
    case SQL_NULL_COLLATION:
    case SQL_QUOTED_IDENTIFIER_CASE:
    case SQL_TXN_CAPABLE:
        retc = (*instSQLGetInfo)(pself->hdbc, (SQLINTEGER)attr, (SQLPOINTER)&u16tmp, 0, NULL);
        context->SetObjectVariable("SQLRETURN", context->Int32((int32_t) retc));
        SETDIAG(retc, pself->henv, pself->hdbc, SQL_NULL_HANDLE);
        return context->UnsignedInt32((uint32_t)u16tmp);
#if defined WIN32 || defined WIN64
    case SQL_ASYNC_DBC_FUNCTIONS:
    case SQL_DRIVER_HDBCSQL_DRIVER_HENV:
#endif
    case SQL_AGGREGATE_FUNCTIONS:
    case SQL_ALTER_DOMAIN:
    case SQL_ALTER_TABLE:
    case SQL_ASYNC_MODE:
    case SQL_BATCH_ROW_COUNT:
    case SQL_BATCH_SUPPORT:
    case SQL_BOOKMARK_PERSISTENCE:
    case SQL_CATALOG_USAGE:
    case SQL_CONVERT_BIGINT:
    case SQL_CONVERT_BINARY:
    case SQL_CONVERT_BIT:
    case SQL_CONVERT_CHAR:
    case SQL_CONVERT_DATE:
    case SQL_CONVERT_DECIMAL:
    case SQL_CONVERT_DOUBLE:
    case SQL_CONVERT_FLOAT:
    case SQL_CONVERT_INTEGER:
    case SQL_CONVERT_LONGVARCHAR:
    case SQL_CONVERT_NUMERIC:
    case SQL_CONVERT_REAL:
    case SQL_CONVERT_SMALLINT:
    case SQL_CONVERT_TIME:
    case SQL_CONVERT_TIMESTAMP:
    case SQL_CONVERT_TINYINT:
    case SQL_CONVERT_VARBINARY:
    case SQL_CONVERT_VARCHAR:
    case SQL_CONVERT_LONGVARBINARY:
    case SQL_CONVERT_GUID:
    case SQL_CONVERT_FUNCTIONS:
    case SQL_CREATE_ASSERTION:
    case SQL_CREATE_CHARACTER_SET:
    case SQL_CREATE_COLLATION:
    case SQL_CREATE_DOMAIN:
    case SQL_CREATE_SCHEMA:
    case SQL_CREATE_TABLE:
    case SQL_CREATE_TRANSLATION:
    case SQL_CREATE_VIEW:
    case SQL_CURSOR_SENSITIVITY:
    case SQL_DATETIME_LITERALS:
    case SQL_DDL_INDEX:
    case SQL_DEFAULT_TXN_ISOLATION:
    case SQL_DRIVER_HDESC:
    case SQL_DRIVER_HLIB:
    case SQL_DRIVER_HSTMT:
    case SQL_DROP_ASSERTION:
    case SQL_DROP_CHARACTER_SET:
    case SQL_DROP_COLLATION:
    case SQL_DROP_DOMAIN:
    case SQL_DROP_SCHEMA:
    case SQL_DROP_TABLE:
    case SQL_DROP_TRANSLATION:
    case SQL_DROP_VIEW:
    case SQL_DYNAMIC_CURSOR_ATTRIBUTES1:
    case SQL_DYNAMIC_CURSOR_ATTRIBUTES2:
    case SQL_FORWARD_ONLY_CURSOR_ATTRIBUTES1:
    case SQL_FORWARD_ONLY_CURSOR_ATTRIBUTES2:
    case SQL_GETDATA_EXTENSIONS:
    case SQL_INDEX_KEYWORDS:
    case SQL_INFO_SCHEMA_VIEWS:
    case SQL_INSERT_STATEMENT:
    case SQL_KEYSET_CURSOR_ATTRIBUTES1:
    case SQL_KEYSET_CURSOR_ATTRIBUTES2:
    case SQL_MAX_ASYNC_CONCURRENT_STATEMENTS:
    case SQL_MAX_BINARY_LITERAL_LEN:
    case SQL_MAX_CHAR_LITERAL_LEN:
    case SQL_MAX_INDEX_SIZE:
    case SQL_MAX_ROW_SIZE:
    case SQL_MAX_STATEMENT_LEN:
    case SQL_NUMERIC_FUNCTIONS:
    case SQL_ODBC_INTERFACE_CONFORMANCE:
    case SQL_OJ_CAPABILITIES:
    case SQL_PARAM_ARRAY_ROW_COUNTS:
    case SQL_PARAM_ARRAY_SELECTS:
    case SQL_POS_OPERATIONS:
    case SQL_SCHEMA_USAGE:
    case SQL_SCROLL_OPTIONS:
    case SQL_SQL_CONFORMANCE:
    case SQL_SQL92_DATETIME_FUNCTIONS:
    case SQL_SQL92_FOREIGN_KEY_DELETE_RULE:
    case SQL_SQL92_FOREIGN_KEY_UPDATE_RULE:
    case SQL_SQL92_GRANT:
    case SQL_SQL92_NUMERIC_VALUE_FUNCTIONS:
    case SQL_SQL92_PREDICATES:
    case SQL_SQL92_RELATIONAL_JOIN_OPERATORS:
    case SQL_SQL92_REVOKE:
    case SQL_SQL92_ROW_VALUE_CONSTRUCTOR:
    case SQL_SQL92_STRING_FUNCTIONS:
    case SQL_SQL92_VALUE_EXPRESSIONS:
    case SQL_STANDARD_CLI_CONFORMANCE:
    case SQL_STATIC_CURSOR_ATTRIBUTES1:
    case SQL_STATIC_CURSOR_ATTRIBUTES2:
    case SQL_STRING_FUNCTIONS:
    case SQL_SUBQUERIES:
    case SQL_SYSTEM_FUNCTIONS:
    case SQL_TIMEDATE_ADD_INTERVALS:
    case SQL_TIMEDATE_DIFF_INTERVALS:
    case SQL_TIMEDATE_FUNCTIONS:
    case SQL_TXN_ISOLATION_OPTION:
    case SQL_UNION:
        retc = (*instSQLGetInfo)(pself->hdbc, (SQLINTEGER)attr, (SQLPOINTER)&u32tmp, 0, NULL);
        context->SetObjectVariable("SQLRETURN", context->Int32((int32_t) retc));
        SETDIAG(retc, pself->henv, pself->hdbc, SQL_NULL_HANDLE);
        return context->UnsignedInt32(u32tmp);
    default:
        context->RaiseException1(Rexx_Error_Invalid_argument_general,
                                 (RexxObjectPtr)context->String("1 unsupported."));
        return 0;  
    }

    return (RexxObjectPtr)context->Nil();
}

/*----------------------------------------------------------------------------*/
/* Statement class methods                                                    */
/*----------------------------------------------------------------------------*/

/**
 * Method:  OrxDB_stmtinit
 *
 * Init the statement class.
 *
 * @param db      Database connection object.
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            OrxDB_stmtinit,            // Object_method name
            RexxObjectPtr, db,
            OPTIONAL_CSTRING, stmt)
{
    stmtself *pself;

    // allocate memory for our self data
    pself = (stmtself *)malloc(sizeof(stmtself));
    memset(pself, 0, sizeof(stmtself));
    context->SetObjectVariable("CSELF", context->NewPointer(pself));

    // get the database connction pointer
    pself->pdbself = (dbself *)context->ObjectToCSelf(db);
    
    // allocate a statement handle
    SQLRETURN retc = (*instSQLAllocHandle)(SQL_HANDLE_STMT, pself->pdbself->hdbc,
                                           &pself->hstmt);
    context->SetObjectVariable("SQLRETURN", context->Int32((int32_t) retc));
    SETDIAG(retc, pself->pdbself->henv, pself->pdbself->hdbc, pself->hstmt);

    // save SQL statement
    if (stmt != NULL) {
        context->SetObjectVariable("stmt", context->String(stmt));
    }
    else {
        context->SetObjectVariable("stmt", context->String(""));
    }
    
    return 0;
}

/**
 * Method:  OrxDB_stmtdestroy
 *
 * Destroy a statement instance.
 *
 * @return        Return code.
 **/
RexxMethod1(int,                       // Return type
            OrxDB_stmtdestroy,         // Object_method name
            CSELF, cself)
{
    stmtself *pself = (stmtself *)cself;
    
    // if necessary, get the symbol(s)
    if (instSQLFreeHandle == SQL_NULL_HANDLE) {
#if defined WIN32 || defined WIN64
        instSQLFreeHandle = GetProcAddress(pself->pdbself->handle, "SQLFreeHandle");
#else
        *(void**)(&instSQLFreeHandle) = dlsym(pself->pdbself->handle, "SQLFreeHandle");
#endif
        if (!instSQLFreeHandle) {
            context->RaiseException1(Rexx_Error_System_resources_user_defined,
                                     (RexxObjectPtr)context->String("Could not load symbol SQLFreeHandle from CLI library."));
            return 0;  
        }
    }
    if (pself->hstmt != NULL) {
        SQLRETURN retc = (*instSQLFreeHandle)(SQL_HANDLE_STMT, &pself->hstmt);
        SETDIAG(retc, pself->pdbself->henv, pself->pdbself->hdbc, pself->hstmt);
    }
    if (pself != NULL) {
        free(pself);
    }
    return 0;
}

/**
 * Method:  OrxDB_execute
 *
 * Execute a prepared SQL statemwnt.
 *
 * @return        Return code.
 **/
RexxMethod1(int,                       // Return type
            OrxDB_execute,             // Object_method name
            CSELF, cself)
{
    stmtself *pself = (stmtself *)cself;
    
    // if necessary, get the symbol(s)
    if (instSQLExecute == SQL_NULL_HANDLE) {
#if defined WIN32 || defined WIN64
        instSQLExecute = GetProcAddress(pself->pdbself->handle, "SQLExecute");
#else
        *(void**)(&instSQLExecute) = dlsym(pself->pdbself->handle, "SQLExecute");
#endif
        if (!instSQLExecute) {
            context->RaiseException1(Rexx_Error_System_resources_user_defined,
                                     (RexxObjectPtr)context->String("Could not load symbol SQLExecute from CLI library."));
            return 0;  
        }
    }

    // execute the SQL statement
    SQLRETURN retc = (*instSQLExecute)(pself->hstmt);
    context->SetObjectVariable("SQLRETURN", context->Int32((int32_t) retc));
    SETDIAG(retc, pself->pdbself->henv, pself->pdbself->hdbc, pself->hstmt);
    
    return 0;
}

/**
 * Method:  OrxDB_execdirect
 *
 * Execute a preparable SQL statemwnt.
 *
 * @return        Return code.
 **/
RexxMethod2(int,                       // Return type
            OrxDB_execdirect,          // Object_method name
            OPTIONAL_CSTRING, stmt,
            CSELF, cself)
{
    stmtself *pself = (stmtself *)cself;
    
    // if necessary, get the symbol(s)
    if (instSQLExecDirect == SQL_NULL_HANDLE) {
#if defined WIN32 || defined WIN64
        instSQLExecDirect = GetProcAddress(pself->pdbself->handle, "SQLExecDirect");
#else
        *(void**)(&instSQLExecDirect) = dlsym(pself->pdbself->handle, "SQLExecDirect");
#endif
        if (!instSQLExecDirect) {
            context->RaiseException1(Rexx_Error_System_resources_user_defined,
                                     (RexxObjectPtr)context->String("Could not load symbol SQLExecDirect from CLI library."));
            return 0;  
        }
    }

    // if no statement passed then use the stmt attribute variable
    if (stmt == NULL) {
        stmt = context->CString(context->GetObjectVariable("stmt"));
    }

    // execute the SQL statement
    SQLRETURN retc = (*instSQLExecDirect)(pself->hstmt, (SQLCHAR *)stmt, strlen(stmt));
    context->SetObjectVariable("SQLRETURN", context->Int32((int32_t) retc));
    SETDIAG(retc, pself->pdbself->henv, pself->pdbself->hdbc, pself->hstmt);
    
    return 0;
}

/**
 * Method:  OrxDB_bind_parms
 *
 * Bind all parameters of an SQL statement
 *
 * Note: The API used here may need to be changed for Windows to
 * SQLBindParameter. The SQLBindParam API is not documented on
 * the MS web pages.
 *
 * @param parms   Argument array
 *
 * @return        Return code.
 **/
RexxMethod2(int,                       // Return type
            OrxDB_bind_parms,          // Object_method name
            RexxArrayObject, parms,
            CSELF, cself)
{
    stmtself *pself = (stmtself *)cself;
    SQLRETURN retc;
    
    // if necessary, get the symbol(s)
    if (instSQLBindParam == SQL_NULL_HANDLE) {
#if defined WIN32 || defined WIN64
        instSQLBindParam = GetProcAddress(pself->pdbself->handle, "SQLBindParam");
#else
        *(void**)(&instSQLBindParam) = dlsym(pself->pdbself->handle, "SQLBindParam");
#endif
        if (!instSQLBindParam) {
            context->RaiseException1(Rexx_Error_System_resources_user_defined,
                                     (RexxObjectPtr)context->String("Could not load symbol SQLBindParam from CLI library."));
            return 0;  
        }
    }

    // bind all parameters
    size_t items = context->ArrayItems(parms);
    for (int n = 1; n <= items; n++) {
        SQLPOINTER p = (SQLPOINTER)context->CString((RexxStringObject)context->ArrayAt(parms, n));
        retc = (*instSQLBindParam)(pself->hstmt, (SQLUSMALLINT)n, SQL_C_CHAR, SQL_CHAR,
                                                   0, 0, p, (SQLLEN *)SQL_NTS);

    }
    context->SetObjectVariable("SQLRETURN", context->Int32((int32_t) retc));
    SETDIAG(retc, pself->pdbself->henv, pself->pdbself->hdbc, pself->hstmt);
    
    return 0;
}

/**
 * Method:  OrxDB_prepare
 *
 * Prepare an SQL statemwnt.
 *
 * @return        Return code.
 **/
RexxMethod2(int,                       // Return type
            OrxDB_prepare,             // Object_method name
            OPTIONAL_CSTRING, stmt,
            CSELF, cself)
{
    stmtself *pself = (stmtself *)cself;
    
    // if necessary, get the symbol(s)
    if (instSQLPrepare == SQL_NULL_HANDLE) {
#if defined WIN32 || defined WIN64
        instSQLPrepare = GetProcAddress(pself->pdbself->handle, "SQLPrepare");
#else
        *(void**)(&instSQLPrepare) = dlsym(pself->pdbself->handle, "SQLPrepare");
#endif
        if (!instSQLPrepare) {
            context->RaiseException1(Rexx_Error_System_resources_user_defined,
                                     (RexxObjectPtr)context->String("Could not load symbol SQLPrepare from CLI library."));
            return 0;  
        }
    }

    // if no statement passed then use the stmt attribute variable
    if (stmt == NULL) {
        stmt = context->CString(context->GetObjectVariable("stmt"));
    }

    // execute the SQL statement
    SQLRETURN retc = (*instSQLPrepare)(pself->hstmt, (SQLCHAR *)stmt, strlen(stmt));
    context->SetObjectVariable("SQLRETURN", context->Int32((int32_t) retc));
    SETDIAG(retc, pself->pdbself->henv, pself->pdbself->hdbc, pself->hstmt);
    
    return 0;
}

/**
 * Method:  OrxDB_fetch
 *
 * Fetch a row.
 *
 * @return        Return code.
 **/
RexxMethod1(int,                       // Return type
            OrxDB_fetch,               // Object_method name
            CSELF, cself)
{
    stmtself *pself = (stmtself *)cself;
    
    // if necessary, get the symbol(s)
    if (instSQLFetch == SQL_NULL_HANDLE) {
#if defined WIN32 || defined WIN64
        instSQLFetch = GetProcAddress(pself->pdbself->handle, "SQLFetch");
#else
        *(void**)(&instSQLFetch) = dlsym(pself->pdbself->handle, "SQLFetch");
#endif
        if (!instSQLFetch) {
            context->RaiseException1(Rexx_Error_System_resources_user_defined,
                                     (RexxObjectPtr)context->String("Could not load symbol SQLFetch from CLI library."));
            return 0;  
        }
    }

    // execute the SQL statement
    SQLRETURN retc = (*instSQLFetch)(pself->hstmt);
    context->SetObjectVariable("SQLRETURN", context->Int32((int32_t) retc));
    SETDIAG(retc, pself->pdbself->henv, pself->pdbself->hdbc, pself->hstmt);
    
    return 0;
}

/**
 * Method:  OrxDB_num_result_cols
 *
 * Return the number of result columns.
 *
 * @return        Number of cols.
 **/
RexxMethod1(int,                       // Return type
            OrxDB_num_result_cols,     // Object_method name
            CSELF, cself)
{
    stmtself *pself = (stmtself *)cself;
    SQLSMALLINT n;
    
    // if necessary, get the symbol(s)
    if (instSQLNumResultCols == SQL_NULL_HANDLE) {
#if defined WIN32 || defined WIN64
        instSQLNumResultCols = GetProcAddress(pself->pdbself->handle, "SQLNumResultCols");
#else
        *(void**)(&instSQLNumResultCols) = dlsym(pself->pdbself->handle, "SQLNumResultCols");
#endif
        if (!instSQLNumResultCols) {
            context->RaiseException1(Rexx_Error_System_resources_user_defined,
                                     (RexxObjectPtr)context->String("Could not load symbol SQLNumResultCols from CLI library."));
            return 0;  
        }
    }

    // execute the SQL statement
    SQLRETURN retc = (*instSQLNumResultCols)(pself->hstmt, &n);
    context->SetObjectVariable("SQLRETURN", context->Int32((int32_t) retc));
    SETDIAG(retc, pself->pdbself->henv, pself->pdbself->hdbc, pself->hstmt);
    
    return (int32_t)n;
}

/**
 * Method:  OrxDB_get_data
 *
 * Return result column data.
 *
 * @param colnum  Column number.
 *
 * @return        Rexx string.
 **/
RexxMethod2(RexxObjectPtr,             // Return type
            OrxDB_get_data,            // Object_method name
            int, column,
            CSELF, cself)
{
    stmtself *pself = (stmtself *)cself;
    SQLCHAR *buf;
    SQLULEN buflen;
    
    // if necessary, get the symbol(s)
    if (instSQLGetData == SQL_NULL_HANDLE) {
#if defined WIN32 || defined WIN64
        instSQLGetData = GetProcAddress(pself->pdbself->handle, "SQLGetData");
#else
        *(void**)(&instSQLGetData) = dlsym(pself->pdbself->handle, "SQLGetData");
#endif
        if (!instSQLGetData) {
            context->RaiseException1(Rexx_Error_System_resources_user_defined,
                                     (RexxObjectPtr)context->String("Could not load symbol SQLGetData from CLI library."));
            return 0;  
        }
    }
    if (instSQLDescribeCol == SQL_NULL_HANDLE) {
#if defined WIN32 || defined WIN64
        instSQLDescribeCol = GetProcAddress(pself->pdbself->handle, "SQLDescribeCol");
#else
        *(void**)(&instSQLDescribeCol) = dlsym(pself->pdbself->handle, "SQLDescribeCol");
#endif
        if (!instSQLDescribeCol) {
            context->RaiseException1(Rexx_Error_System_resources_user_defined,
                                     (RexxObjectPtr)context->String("Could not load symbol SQLDescribeCol from CLI library."));
            return 0;  
        }
    }

    // get the size of the buffer needed and allocate the buffer
    SQLRETURN retc = (*instSQLDescribeCol)(pself->hstmt, column, NULL, 0, NULL,
                                           NULL, &buflen, NULL, NULL);
    buflen++;  // add one byte for the null terminator
    buf = (SQLCHAR *)alloca((size_t)buflen);
    
    // execute the SQL statement
    retc = (*instSQLGetData)(pself->hstmt, column, SQL_C_CHAR, buf, buflen,
                             (SQLLEN *)&buflen);
    context->SetObjectVariable("SQLRETURN", context->Int32((int32_t) retc));
    SETDIAG(retc, pself->pdbself->henv, pself->pdbself->hdbc, pself->hstmt);

    return (RexxObjectPtr)context->String((char *)buf, (size_t)buflen);
}
//
// /**
//  * Method:  OrxDB_get_stmt_attr
//  *
//  * Get an attribute.
//  *
//  * @param attr    Attribute name.
//  *
//  * @return        Attribute value.
//  **/
// RexxMethod2(RexxObjectPtr,             // Return type
//             OrxDB_get_stmt_attr,       // Object_method name
//             CSTRING, attr,
//             CSELF, cself)
// {
//     stmtself *pstmtself = (stmtself *)cself;
//
//     return context->Nil();
// }
//
// /**
//  * Method:  OrxDB_set_stmt_attr
//  *
//  * Set an attribute.
//  *
//  * @param attr    Attribute name.
//  *
//  * @param val     Attribute value.
//  *
//  * @return        Return code.
//  **/
// RexxMethod3(int,                       // Return type
//             OrxDB_set_stmt_attr,       // Object_method name
//             CSTRING, attr,
//             CSTRING, val,
//             CSELF, cself)
// {
//     stmtself *pstmtself = (stmtself *)cself;
//     
//     return 0;
// }

// build the actual function entry list
RexxRoutineEntry orxsql_routines[] = {
    REXX_LAST_ROUTINE()
};


// build the actual method entry list
RexxMethodEntry orxsql_methods[] = {
    REXX_METHOD(OrxDB_isbitset, OrxDB_isbitset),
    REXX_METHOD(OrxDB_version, OrxDB_version),
    REXX_METHOD(OrxDB_dbinit, OrxDB_dbinit),
    REXX_METHOD(OrxDB_set_env_attr, OrxDB_set_env_attr),
    REXX_METHOD(OrxDB_get_env_attr, OrxDB_get_env_attr),
    REXX_METHOD(OrxDB_connect, OrxDB_connect),
    REXX_METHOD(OrxDB_close, OrxDB_close),
    REXX_METHOD(OrxDB_set_connx_attr, OrxDB_set_connx_attr),
    REXX_METHOD(OrxDB_get_connx_attr, OrxDB_get_connx_attr),
    REXX_METHOD(OrxDB_commit, OrxDB_commit),
    REXX_METHOD(OrxDB_rollback, OrxDB_rollback),
    REXX_METHOD(OrxDB_check_connx, OrxDB_check_connx),
    REXX_METHOD(OrxDB_destroy, OrxDB_destroy),
    REXX_METHOD(OrxDB_get_info, OrxDB_get_info),
    REXX_METHOD(OrxDB_stmtinit, OrxDB_stmtinit),
    REXX_METHOD(OrxDB_execute, OrxDB_execute),
    REXX_METHOD(OrxDB_execdirect, OrxDB_execdirect),
    REXX_METHOD(OrxDB_bind_parms, OrxDB_bind_parms),
    REXX_METHOD(OrxDB_prepare, OrxDB_prepare),
    REXX_METHOD(OrxDB_fetch, OrxDB_fetch),
    REXX_METHOD(OrxDB_num_result_cols, OrxDB_num_result_cols),
    REXX_METHOD(OrxDB_get_data, OrxDB_get_data),
    REXX_METHOD(OrxDB_stmtdestroy, OrxDB_stmtdestroy),


    REXX_LAST_METHOD()
};


RexxPackageEntry orxsql_package_entry = {
    STANDARD_PACKAGE_HEADER
    REXX_INTERPRETER_4_0_0,              // anything after 4.0.0 will work
    "OrxSQL",                            // name of the package
    VERSTRING(VMAJOR,VMINOR,VREL),       // package information
    NULL,                                // no load functions
    NULL,                                // no unload functions
    orxsql_routines,                     // the exported routines
    orxsql_methods                       // the exported methods
};

// package loading stub.
OOREXX_GET_PACKAGE(orxsql);

