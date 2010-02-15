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


/*----------------------------------------------------------------------------*/
/* Local Definitions/variables                                                */
/*----------------------------------------------------------------------------*/

#define VERSTRING(major,minor,rel) #major "." #minor "." #rel

typedef struct _dbself {
#if defined WIN32 || defined WIN64
    HMODULE handle;
#else
    void *handle;
#endif
    SQLHENV henv;
    SQLHDBC hdbc;
    SQLRETURN SQL_API (*instSQLAllocHandle)(SQLSMALLINT HandleType,
                                            SQLHANDLE InputHandle,
                                            SQLHANDLE *OutputHandle);
    SQLRETURN SQL_API (*instSQLSetEnvAttr)(SQLHENV EnvironmentHandle,
                                           SQLINTEGER Attribute, SQLPOINTER Value,
                                           SQLINTEGER StringLength);
    SQLRETURN SQL_API (*instSQLGetEnvAttr)(SQLHENV EnvironmentHandle,
                                           SQLINTEGER Attribute, SQLPOINTER Value,
                                           SQLINTEGER BufferLength,
                                           SQLINTEGER *StringLength);
    SQLRETURN SQL_API (*instSQLConnect)(SQLHDBC ConnectionHandle,
                                        SQLCHAR *ServerName, SQLSMALLINT NameLength1,
                                        SQLCHAR *UserName, SQLSMALLINT NameLength2,
                                        SQLCHAR *Authentication, SQLSMALLINT NameLength3);
    SQLRETURN SQL_API (*instSQLDisconnect)(SQLHDBC ConnectionHandle);
    SQLRETURN SQL_API (*instSQLSetConnectAttr)(SQLHDBC ConnectionHandle,
                                               SQLINTEGER Attribute, SQLPOINTER Value,
                                               SQLINTEGER StringLength);
    SQLRETURN SQL_API (*instSQLGetConnectAttr)(SQLHDBC ConnectionHandle,
                                               SQLINTEGER Attribute, SQLPOINTER Value,
                                               SQLINTEGER BufferLength,
                                               SQLINTEGER *StringLength);
    SQLRETURN SQL_API (*instSQLEndTran)(SQLSMALLINT HandleType, SQLHANDLE Handle,
                                        SQLSMALLINT CompletionType);
} dbself;

typedef struct _stmtself {
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
        if (pself->instSQLAllocHandle == SQL_NULL_HANDLE) {
#if defined WIN32 || defined WIN64
            pself->instSQLAllocHandle = GetProcAddress(pself->handle, "SQLAllocHandle");
#else
            *(void**)(&pself->instSQLAllocHandle) = dlsym(pself->handle, "SQLAllocHandle");
#endif
            if (!pself->instSQLAllocHandle) {
                context->RaiseException1(Rexx_Error_System_resources_user_defined,
                                         (RexxObjectPtr)context->String("Could not load symbol SQLAllocHandle from CLI library."));
                return 0;  
            }
        }
        SQLRETURN retc = (*pself->instSQLAllocHandle)(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &pself->henv);
        context->SetObjectVariable("SQLRETURN", context->Int32((int32_t) retc));
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
    if (pself->instSQLSetEnvAttr == SQL_NULL_HANDLE) {
#if defined WIN32 || defined WIN64
        pself->instSQLSetEnvAttr = GetProcAddress(pself->handle, "SQLSetEnvAttr");
#else
        *(void**)(&pself->instSQLSetEnvAttr) = dlsym(pself->handle, "SQLSetEnvAttr");
#endif
        if (!pself->instSQLSetEnvAttr) {
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
        retc = (*pself->instSQLSetEnvAttr)(pself->henv, (SQLINTEGER)attr, (SQLPOINTER)&itmp, 0);
        break;
#if defined WIN32 || defined WIN64
    case SQL_ATTR_CONNECTION_POOLING:
    case SQL_ATTR_CP_MATCH:
        context->ObjectToUnsignedInt32(val, &utmp);
        retc = (*pself->instSQLSetEnvAttr)(pself->henv, (SQLINTEGER)attr, (SQLPOINTER)&utmp, 0);
        break;
#endif
    default:
        context->RaiseException1(Rexx_Error_Invalid_argument_general,
                                 (RexxObjectPtr)context->String("1 invalid."));
        return 0;  
    }
    context->SetObjectVariable("SQLRETURN", context->Int32((int32_t) retc));
    
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
    if (pself->instSQLGetEnvAttr == SQL_NULL_HANDLE) {
#if defined WIN32 || defined WIN64
        pself->instSQLGetEnvAttr = GetProcAddress(pself->handle, "SQLGetEnvAttr");
#else
        *(void**)(&pself->instSQLGetEnvAttr) = dlsym(pself->handle, "SQLGetEnvAttr");
#endif
        if (!pself->instSQLGetEnvAttr) {
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
        retc = (*pself->instSQLGetEnvAttr)(pself->henv, (SQLINTEGER)attr, (SQLPOINTER)&itmp, 0, 0);
        context->SetObjectVariable("SQLRETURN", context->Int32((int32_t) retc));
        return context->Int32(itmp);
        break;
#if defined WIN32 || defined WIN64
    case SQL_ATTR_CONNECTION_POOLING:
    case SQL_ATTR_CP_MATCH:
        retc = (*pself->instSQLGetEnvAttr)(pself->henv, (SQLINTEGER)attr, (SQLPOINTER)&utmp, 0, 0);
        context->SetObjectVariable("SQLRETURN", context->Int32((int32_t) retc));
        return context->UnsignedInt32(utmp);
        break;
#endif
    default:
        context->RaiseException1(Rexx_Error_Invalid_argument_general,
                                 (RexxObjectPtr)context->String("1 invalid."));
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
        if (pself->instSQLConnect == SQL_NULL_HANDLE) {
#if defined WIN32 || defined WIN64
            pself->instSQLConnect = GetProcAddress(pself->handle, "SQLConnect");
#else
            *(void**)(&pself->instSQLConnect) = dlsym(pself->handle, "SQLConnect");
#endif
            if (!pself->instSQLConnect) {
                context->RaiseException1(Rexx_Error_System_resources_user_defined,
                                         (RexxObjectPtr)context->String("Could not load symbol SQLConnect from CLI library."));
                return 0;  
            }
        }
        // allocate a connection handle
        retc = (*pself->instSQLAllocHandle)(SQL_HANDLE_DBC, pself->henv, &pself->hdbc);
    }
    // connect to the database
    retc = (*pself->instSQLConnect)(pself->hdbc, (SQLCHAR *)dbname, strlen(dbname),
                                    (SQLCHAR *)user, strlen(user), (SQLCHAR *)pw, strlen(pw));
    context->SetObjectVariable("SQLRETURN", context->Int32((int32_t) retc));
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
    if (pself->instSQLDisconnect == SQL_NULL_HANDLE) {
#if defined WIN32 || defined WIN64
        pself->instSQLDisconnect = GetProcAddress(pself->handle, "SQLDisconnect");
#else
        *(void**)(&pself->instSQLDisconnect) = dlsym(pself->handle, "SQLDisconnect");
#endif
        if (!pself->instSQLDisconnect) {
            context->RaiseException1(Rexx_Error_System_resources_user_defined,
                                     (RexxObjectPtr)context->String("Could not load symbol SQLDisconnect from CLI library."));
            return 0;  
        }
    }
    // disconnect from the database
    SQLRETURN retc = (*pself->instSQLDisconnect)(pself->hdbc);
    context->SetObjectVariable("SQLRETURN", context->Int32((int32_t) retc));
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
    if (pself->instSQLSetConnectAttr == SQL_NULL_HANDLE) {
#if defined WIN32 || defined WIN64
        pself->instSQLSetConnectAttr = GetProcAddress(pself->handle, "SQLSetConnectAttr");
#else
        *(void**)(&pself->instSQLSetConnectAttr) = dlsym(pself->handle, "SQLSetConnectAttr");
#endif
        if (!pself->instSQLSetConnectAttr) {
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
        retc = (*pself->instSQLSetConnectAttr)(pself->hdbc, (SQLINTEGER)attr, (SQLPOINTER)ctmp, strlen(ctmp));
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
        retc = (*pself->instSQLSetConnectAttr)(pself->hdbc, (SQLINTEGER)attr, (SQLPOINTER)&utmp, 0);
        break;
    default:
        context->RaiseException1(Rexx_Error_Invalid_argument_general,
                                 (RexxObjectPtr)context->String("1 invalid."));
        return 0;  
    }
    context->SetObjectVariable("SQLRETURN", context->Int32((int32_t) retc));
    
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

    // if necessary, get the symbol(s)
    if (pself->instSQLGetConnectAttr == SQL_NULL_HANDLE) {
#if defined WIN32 || defined WIN64
        pself->instSQLGetConnectAttr = GetProcAddress(pself->handle, "SQLGetConnectAttr");
#else
        *(void**)(&pself->instSQLGetConnectAttr) = dlsym(pself->handle, "SQLGetConnectAttr");
#endif
        if (!pself->instSQLGetConnectAttr) {
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
        retc = (*pself->instSQLGetConnectAttr)(pself->henv, (SQLINTEGER)attr, (SQLPOINTER)ctmp, 0, 0);
        context->SetObjectVariable("SQLRETURN", context->Int32((int32_t) retc));
        return context->String(ctmp);
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
        retc = (*pself->instSQLGetConnectAttr)(pself->henv, (SQLINTEGER)attr, (SQLPOINTER)&utmp, 0, 0);
        context->SetObjectVariable("SQLRETURN", context->Int32((int32_t) retc));
        return context->UnsignedInt32(utmp);
        break;
    default:
        context->RaiseException1(Rexx_Error_Invalid_argument_general,
                                 (RexxObjectPtr)context->String("1 invalid."));
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
    if (pself->instSQLEndTran == SQL_NULL_HANDLE) {
#if defined WIN32 || defined WIN64
        pself->instSQLEndTran = GetProcAddress(pself->handle, "SQLEndTran");
#else
        *(void**)(&pself->instSQLEndTran) = dlsym(pself->handle, "SQLEndTran");
#endif
        if (!pself->instSQLEndTran) {
            context->RaiseException1(Rexx_Error_System_resources_user_defined,
                                     (RexxObjectPtr)context->String("Could not load symbol SQLEndTran from CLI library."));
            return 0;  
        }
    }
    // commit the transaction
    SQLRETURN retc = (*pself->instSQLEndTran)(SQL_HANDLE_ENV, pself->henv, SQL_COMMIT);
    context->SetObjectVariable("SQLRETURN", context->Int32((int32_t) retc));
    
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
    if (pself->instSQLEndTran == SQL_NULL_HANDLE) {
#if defined WIN32 || defined WIN64
        pself->instSQLEndTran = GetProcAddress(pself->handle, "SQLEndTran");
#else
        *(void**)(&pself->instSQLEndTran) = dlsym(pself->handle, "SQLEndTran");
#endif
        if (!pself->instSQLEndTran) {
            context->RaiseException1(Rexx_Error_System_resources_user_defined,
                                     (RexxObjectPtr)context->String("Could not load symbol SQLEndTran from CLI library."));
            return 0;  
        }
    }
    // rollback the transaction
    SQLRETURN retc = (*pself->instSQLEndTran)(SQL_HANDLE_ENV, pself->henv, SQL_ROLLBACK);
    context->SetObjectVariable("SQLRETURN", context->Int32((int32_t) retc));

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
//
// /**
//  * Method:  OrxDB_prepare
//  *
//  * Prepare an SQL statement for execution.
//  *
//  * @param sql     SQL statement to be prepared.
//  *
//  * @param options Options string.
//  *
//  * @return        Instance of the Statement class.
//  **/
// RexxMethod3(RexxObjectPtr,             // Return type
//             OrxDB_prepare,             // Object_method name
//             CSTRING, sql,
//             CSTRING, options,
//             CSELF, cself)
// {
//     dbself *pself = (dbself *)cself;
//
//     return context->Nil();
// }
//
// /**
//  * Method:  OrxDB_dbexecute
//  *
//  * Execute a raw SQL statemwnt.
//  *
//  * @param sql     SQL statement to be executed.
//  *
//  * @return        Return code.
//  **/
// RexxMethod2(int,                       // Return type
//             OrxDB_dbexecute,           // Object_method name
//             CSTRING, sql,
//             CSELF, cself)
// {
//     dbself *pself = (dbself *)cself;
//     
//     return 0;
// }
//
// /**
//  * Method:  OrxDB_begin
//  *
//  * Begin a database transaction.
//  *
//  * @return        Return code.
//  **/
// RexxMethod1(int,                       // Return type
//             OrxDB_begin,               // Object_method name
//             CSELF, cself)
// {
//     dbself *pself = (dbself *)cself;
//     
//     return 0;
// }
//
// /**
//  * Method:  OrxDB_get_connx_attr
//  *
//  * Get an attribute.
//  *
//  * @param attr    Attribute name.
//  *
//  * @return        Attribute value.
//  **/
// RexxMethod2(RexxObjectPtr,             // Return type
//             OrxDB_get_connx_attr,      // Object_method name
//             int, attr,
//             CSELF, cself)
// {
//     dbself *pself = (dbself *)cself;
//
//     return (RexxObjectPtr)context->String("");
// }
//
// /**
//  * Method:  OrxDB_set_connx_attr
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
//             OrxDB_set_connx_attr,      // Object_method name
//             CSTRING, attr,
//             RexxObjectPtr, val,
//             CSELF, cself)
// {
//     dbself *pself = (dbself *)cself;
//     
//     return 0;
// }
//
// /*----------------------------------------------------------------------------*/
// /* Statement class methods                                                    */
// /*----------------------------------------------------------------------------*/
//
// /**
//  * Method:  OrxDB_stmtinit
//  *
//  * Init the statement class.
//  *
//  * @return        Zero.
//  **/
// RexxMethod0(int,                       // Return type
//             OrxDB_stmtinit)            // Object_method name
// {
//     stmtself *pstmtself;
//     
//     return 0;
// }
//
// /**
//  * Method:  OrxDB_destroy
//  *
//  * Destroy a statement instance.
//  *
//  * @return        Return code.
//  **/
// RexxMethod1(int,                       // Return type
//             OrxDB_destroy,             // Object_method name
//             CSELF, cself)
// {
//     
//     if (cself != NULL) {
//         free(cself);
//     }
//     return 0;
// }
//
// /**
//  * Method:  OrxDB_stmtexecute
//  *
//  * Execute a prepared SQL statemwnt.
//  *
//  * @return        Return code.
//  **/
// RexxMethod1(int,                       // Return type
//             OrxDB_stmtexecute,         // Object_method name
//             CSELF, cself)
// {
//     stmtself *pstmtself = (stmtself *)cself;
//     
//     return 0;
// }
//
// /**
//  * Method:  OrxDB_fetch
//  *
//  * Fetch a row.
//  *
//  * @return        Return code.
//  **/
// RexxMethod1(int,                       // Return type
//             OrxDB_fetch,               // Object_method name
//             CSELF, cself)
// {
//     stmtself *pstmtself = (stmtself *)cself;
//     
//     return 0;
// }
//
// /**
//  * Method:  OrxDB_describe_col
//  *
//  * Describe a result column.
//  *
//  * @param colnum  Column number.
//  *
//  * @return        Return code.
//  **/
// RexxMethod2(int,                       // Return type
//             OrxDB_describe_col,        // Object_method name
//             int, colnum,
//             CSELF, cself)
// {
//     stmtself *pstmtself = (stmtself *)cself;
//     
//     return 0;
// }
//
// /**
//  * Method:  OrxDB_get_col_data
//  *
//  * Return result column data.
//  *
//  * @param colnum  Column number.
//  *
//  * @return        Rexx string.
//  **/
// RexxMethod2(RexxObjectPtr,             // Return type
//             OrxDB_get_col_data,        // Object_method name
//             int, colnum,
//             CSELF, cself)
// {
//     stmtself *pstmtself = (stmtself *)cself;
//     
//     return context->Nil();
// }
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

