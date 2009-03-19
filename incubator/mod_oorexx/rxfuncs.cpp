/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (C) W. David Ashley 2004-2008. All Rights Reserved.              */
/* Copyright (c) 2009-2009 Rexx Language Association. All rights reserved.    */
/*                                                                            */
/* This program and the accompanying materials are made available under       */
/* the terms of the Common Public License v1.0 which accompanies this         */
/* distribution. A copy is also available at the following address:           */
/* http://www.ibm.com/developerworks/oss/CPLv1.0.htm                          */
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
/* Author: W. David Ashley                                                    */
/*                                                                            */
/*----------------------------------------------------------------------------*/


#include "mod_oorexx.h"


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* The following functions are general and implement high-level Apache API    */
/* functionality or specific mod_rexx functionality.                          */
/*                                                                            */
/*----------------------------------------------------------------------------*/


/**
 * Function:  WWWGetVersion
 *
 * Return the Mod_ooRexx version string.
 *
 * @return        Version string
 */
RexxRoutine0(RexxObjectPtr,            // Return type
             WWWGetVersion)            // Routine name
{
    char verstr[50];

    /* Set the return code */
    strcpy(verstr, version);
    strcat(verstr, " ");
    strcat(verstr, compDate);
    strcat(verstr, " ");
    strcat(verstr, compTime);

    return context->String(verstr);
}


/**
 * Function:  WWWLogError
 *
 * Logs a error level message.
 *
 * @param rr      The request record pointer.
 *
 * @param msg     The message to be logged.
 *
 * @return        Zero
 */
RexxRoutine2(int,                      // Return type
             WWWLogError,              // Routine name
             POINTER, rr,              // Request record pointer
             CSTRING, msg)             // Error message
{
    request_rec *r = (request_rec *)rr;

    /* Log the message */
    ap_log_rerror(APLOG_MARK, APLOG_NOERRNO | APLOG_ERR, 0, r, (const char *)msg);

    return 0;
}


/**
 * Function:  WWWLogWarning
 *
 * Logs a warning level message.
 *
 * @param rr      The request record pointer.
 *
 * @param msg     The message to be logged.
 *
 * @return        Zero
 */
RexxRoutine2(int,                      // Return type
             WWWLogWarning,            // Routine name
             POINTER, rr,              // Request record pointer
             CSTRING, msg)             // Error message
{
    request_rec *r = (request_rec *)rr;

    /* Log the message */
    ap_log_rerror(APLOG_MARK, APLOG_NOERRNO | APLOG_WARNING, 0, r, (const char *)msg);

    return 0;
}


/**
 * Function:  WWWLogInfo
 *
 * Logs a informational level message.
 *
 * @param rr      The request record pointer.
 *
 * @param msg     The message to be logged.
 *
 * @return        Zero
 */
RexxRoutine2(int,                      // Return type
             WWWLogInfo,               // Routine name
             POINTER, rr,              // Request record pointer
             CSTRING, msg)             // Error message
{
    request_rec *r = (request_rec *)rr;

    /* Log the message */
    ap_log_rerror(APLOG_MARK, APLOG_NOERRNO | APLOG_INFO, 0, r, (const char *)msg);

    return 0;
}


/**
 * Function:  WWWLogReason
 *
 * Logs a reason level message.
 *
 * @param rr      The request record pointer.
 *
 * @param msg     The message to be logged.
 *
 * @param msg     The message to be logged.
 *
 * @return        Zero
 */
RexxRoutine3(int,                      // Return type
             WWWLogReason,             // Routine name
             POINTER, rr,              // Request record pointer
             CSTRING, obj,             // Failed object
             CSTRING, rmsg)            // Reason message
{
    request_rec *r = (request_rec *)rr;

    /* Log the reason message */
    ap_log_rerror(APLOG_MARK, APLOG_NOERRNO, 0, r,
                  "Access to %s failed for %s. reason: %s", obj,
                  ap_get_remote_host(r->connection, r->per_dir_config,
                                      REMOTE_NAME, NULL),
                  rmsg);

    return 0;
}


/**
 * Function:  WWWSendHTTPHeader
 *
 * Sets the content type for the returned page and senf the HTTP 
 * header. 
 *
 * @param rr      The request record pointer.
 *
 * @param type    The mime type.
 *
 * @return        Zero
 */
RexxRoutine2(int,                      // Return type
             WWWSendHTTPHeader,        // Routine name
             POINTER, rr,              // Request record pointer
             CSTRING, type)            // Mime type string
{
    request_rec *r = (request_rec *)rr;

    /* Set the content-type and send the header */
    r->content_type = apr_pstrdup(r->pool, type);
    ap_send_http_header(r);

    return 0;
}


/**
 * Function:  WWWAddCookie
 *
 * Sets a cookie in the header of the returned page.
 *
 * @param rr      The request record pointer.
 *
 * @param cookie  The cookie text.
 *
 * @return        Zero
 */
RexxRoutine2(int,                      // Return type
             WWWAddCookie,             // Routine name
             POINTER, rr,              // Request record pointer
             CSTRING, cookie)          // Cookie text
{
    request_rec *r = (request_rec *)rr;

    /* Set the cookie */
    apr_table_add(r->headers_out, "Set-Cookie", cookie);

    return 0;
   }


/**
 * Function:  WWWSetHeaderValue
 *
 * Replaces a value in the header of the returned page.
 *
 * @param rr      The request record pointer.
 *
 * @param key     Key value.
 *
 * @param cookie  Cookie value.
 *
 * @return        Zero
 */
RexxRoutine3(int,                      // Return type
             WWWSetHeaderValue,        // Routine name
             POINTER, rr,              // Request record pointer
             CSTRING, key,             // Key text
             CSTRING, cookie)          // Cookie text
{
    request_rec *r = (request_rec *)rr;

    /* Set the cookie */
    apr_table_set(r->headers_out, key, cookie);

    return 0;
}


/**
 * Function:  translate
 *
 * Translates characters in a string.
 *
 * @param source  The source string.
 *
 * @param intable The input translate table.
 *
 * @param outtable The output translate table.
 *
 * @return        The translated string.
 */
static char *translate(char *source, const char *intable, const char *outtable)
{
    int i = 0, j;

    /* Look for bad input argument data */
    if (source == NULL || intable == NULL || outtable == NULL) {
        return (source);
    }
    if (strlen(source) == 0 || strlen(intable) == 0 ||
        strlen(outtable) == 0) {
        return source;
    }

    /* Perform character translation(s) */
    for (i = 0; i < strlen(source); i++) {
        for (j = 0; j < strlen(intable); j++) {
            if (*(intable + j) == *(source + i)) {
                *(source + i) = *(outtable + j);
                break;
            }
        }
    }

    return source;
}


/**
 * Function:  read_post_vars
 *
 * Read the POST request variables from the client.
 *
 * @param rr      The request record pointer.
 *
 * @param rbuf    The target buffer for the POST variables.
 *
 * @return        Client read return code.
 */
static int read_post_vars(request_rec *r, const char **rbuf)
{
    int rc;

    /* See if we need to chunk the request */
    if ((rc = ap_setup_client_block(r, REQUEST_CHUNKED_ERROR)) != OK) {
        return rc;
    }

    /* Now do the body of the request */
    if (ap_should_client_block (r)) {
        char argsbuffer[HUGE_STRING_LEN];
        int rsize, len_read, rpos = 0;
        long length = r->remaining;

        *rbuf = (char *)apr_pcalloc(r->pool, length + 1);

        while ((len_read =
               ap_get_client_block(r, argsbuffer, sizeof(argsbuffer))) > 0) {
            if ((rpos + len_read) > length) {
                rsize = length - rpos;
            }
            else {
                rsize = len_read;
            }
            memcpy((char *)*rbuf + rpos, argsbuffer, rsize);
            rpos += rsize;
        }
    }

    return rc;
}


/**
 * Function:  parse_vars
 *
 * Parses the buffer for name=value pairs and then assigns them 
 * to a Rexx stem variabe.
 *
 * @param rr      The request record pointer.
 *
 * @param data    The data to be parsed.
 *
 * @return        Void
 */
static void parse_vars(request_rec *r, char *data)
{
    int num = 0;
    char *val;
    char *name;
    char varname[512];

    if (data != NULL) {
        while (*data &&
               (val = ap_getword (r->pool, (const char **) &data, '&'))) {
            name = ap_getword(r->pool, (const char **)&val, '=');
            ap_unescape_url(translate(name, "+", " "));
            ap_unescape_url(translate(val, "+", " "));

            /* We now have a name=value pair so create a Rexx variable */
            num++;
            sprintf(varname, "WWWARGS.%d.!NAME", num);
            SetooRexxVar(varname, name);
            sprintf(varname, "WWWARGS.%d.!VALUE", num);
            SetooRexxVar(varname, val);
            sprintf(varname, "WWWARGS.%s", name);
            SetooRexxVar(varname, val);
        }
    }

    /* Now set the correct number of arguments */
    sprintf(varname, "%d", num);
    SetooRexxVar((char *)"WWWARGS.0", varname);
}


/**
 * Function:  WWWGetArgs
 *
 * Returns either the GET or POST name/value pairs.
 * to a Rexx stem variabe.
 *
 * @param rr      The request record pointer.
 *
 * @return        Void
 */
RexxRoutine1(int,                      // Return type
             WWWGetArgs,               // Routine name
             POINTER, rr)              // Request record pointer
{
    request_rec *r = (request_rec *)rr;
    const char *type;
    char *data = NULL;
    int rc;

    /* What kind of request is this? */
    switch (r->method_number) {
    case (M_POST):
        /* Test content type */
        type = apr_table_get(r->headers_in, "Content-Type");
        /* There are tools which can send a POST request without a content-type */
        if (type != NULL) {
            if (strcasecmp(type, "application/x-www-form-urlencoded") != 0) {
                break;
            }
        }

        /* Read the request body and point to POST arguments*/
        if ((rc = read_post_vars(r, (const char **)&data)) != OK) {
            break;
        }

        /* Set Rexx stem array */
        parse_vars(r, data);

        /* Set Rexx Post data variable */
        SetooRexxVar((char *)"WWWPOST_STRING", data);
        break;
    case (M_GET):
        /* point to GET arguments */
        data = r->args;

        /* Set Rexx stem array */
        parse_vars(r, data);
        break;
    default:
        break;
    }

    return 0;
}


/**
 * Function:  WWWGetCookies
 *
 * Returns cookies.
 *
 * @param rr      The request record pointer.
 *
 * @return        Void
 */
RexxRoutine1(int,                      // Return type
             WWWGetCookies,            // Routine name
             POINTER, rr)              // Request record pointer
{
    request_rec *r = (request_rec *)rr;
    const char *data;
    char *val;
    char *name;
    char *pair;
    int num = 0;
    char varname [512];

    /* Initialize data */
    data = apr_table_get(r->headers_in, "Cookie");
    if (!data) {
        SetooRexxVar((char *)"WWWCOOKIES.0", (char *)"0");
        return RXFUNC_OK;
    }

    /* Get the cookies out of the buffer */
    while (*data &&
           (pair = ap_getword(r->pool, (const char **)&data, ';'))) {
        if (*data == ' ') {
            ++data;
        }
        name = ap_getword(r->pool, (const char **)&pair, '=');
        while (*pair &&
               (val = ap_getword(r->pool, (const char **)&pair, '&'))) {
            ap_unescape_url(translate(val, "+", " "));

            /* We now have a name=value pair so create a Rexx variable */
            sprintf(varname, "WWWCOOKIES.%d.!NAME", ++num);
            SetooRexxVar(varname, name);
            sprintf(varname, "WWWCOOKIES.%d.!VALUE", num);
            SetooRexxVar(varname, val);
            sprintf(varname, "WWWCOOKIES.%s", name);
            SetooRexxVar (varname, val);
        }
    }

    /* Now set the correct number of arguments */
    sprintf(varname, "%d", num);
    SetooRexxVar((char *)"WWWCOOKIES.0", varname);

    return 0;
}


/**
 * Function:  WWWSub_Req_Lookup_URI
 *
 * Returns a subrequest handle.
 *
 * @param rr      The request record pointer.
 *
 * @param uri     The URI
 *
 * @return        Void
 */
RexxRoutine2(POINTER,                  // Return type
             WWWSub_Req_Lookup_URI,    // Routine name
             POINTER, rr,              // Request record pointer
             CSTRING, uri)             // Subrequest uri
{
    request_rec *r = (request_rec *)rr;
    request_rec *subreq;

    /* Do the subrequest */
    subreq = ap_sub_req_lookup_uri(uri, r, NULL);

    return subreq;
}


/**
 * Function:  WWWSub_Req_Lookup_File
 *
 * Returns a subrequest handle.
 *
 * @param rr      The request record pointer.
 *
 * @param uri     The URI
 *
 * @return        Void
 */
RexxRoutine2(POINTER,                  // Return type
             WWWSub_Req_Lookup_File,   // Routine name
             POINTER, rr,              // Request record pointer
             CSTRING, uri)             // File path
{
    request_rec *r = (request_rec *)rr;
    request_rec *subreq;

    /* Do the subrequest */
    subreq = ap_sub_req_lookup_file(uri, r, NULL);

    return subreq;
}


/**
 * Function:  WWWRun_Sub_Req
 *
 * Run the content handler for a subrequest.
 *
 * @param rr      The request record pointer.
 *
 * @return        Void
 */
RexxRoutine1(int,                      // Return type
             WWWRun_Sub_Req,           // Routine name
             POINTER, rr)              // Request record pointer
{
    request_rec *r = (request_rec *)rr;

    /* Do the subrequest */
    return ap_run_sub_req(r);
}


/**
 * Function:  WWWInternal_Redirect
 *
 * Create a request from the URI argument and run it.
 *
 * @param rr      The request record pointer.
 *
 * @param uri     The request URI.
 *
 * @return        Void
 */
RexxRoutine2(int,                      // Return type
             WWWInternal_Redirect,     // Routine name
             POINTER, rr,              // Request record pointer
             CSTRING, uri)             // Subrequest uri
{
    request_rec *r = (request_rec *)rr;

    /* Do the redirect */
    ap_internal_redirect(uri, r);

    return 0;
}


/**
 * Function:  WWWEscape_Path
 *
 * Converts a filesystem pathname to a properly escaped URI.
 *
 * @param rr      The request record pointer.
 *
 * @param path    The pathname.
 *
 * @return        Void
 */
RexxRoutine3(RexxObjectPtr,            // Return type
             WWWEscape_Path,           // Routine name
             POINTER, rr,              // Request record pointer
             CSTRING, path,            // Path to convert
             int, partial)             // Partial flag
{
    request_rec *r = (request_rec *)rr;
    char *escurl;

    /* Perform the function */
    escurl = ap_os_escape_path(r->pool, path, partial);

    return context->String(escurl);
}


/**
 * Function:  WWWConstruct_URL
 *
 * Constructs a fully qualified url from the specified path.
 *
 * @param rr      The request record pointer.
 *
 * @param path    The path.
 *
 * @return        Void
 */
RexxRoutine2(RexxObjectPtr,            // Return type
             WWWConstruct_URL,         // Routine name
             POINTER, rr,              // Request record pointer
             CSTRING, path)            // Path to convert
{
    request_rec *r = (request_rec *)rr;
    char *url;

    /* Perform the function */
    url = ap_construct_url(r->pool, path, r);

    return context->String(url);
}


/**
 * Function:  WWWHTTP_Time
 *
 * Returns the time formatted per RFC 822 and 1123.
 *
 * @param rr      The request record pointer.
 *
 * @return        Void
 */
RexxRoutine1(RexxObjectPtr,            // Return type
             WWWHTTP_Time,             // Routine name
             POINTER, rr)              // Request record pointer
{
    request_rec *r = (request_rec *)rr;
    char *time_buf;

    /* Perform the function */
    time_buf = ap_ht_time(r->pool, apr_time_now(), "%a %d %b %Y %T %Z", 0);

    return context->String(time_buf);
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* The following functions support the Rexx Request_rec class declared in the */
/* file Apache.cls. This file can be included into an ooRexx program via      */
/* the ::REQUIRES directive.                                                  */
/*                                                                            */
/*----------------------------------------------------------------------------*/


/**
 * Method:  init
 *
 * Create a request_rec.
 *
 * @param rr      The request record pointer.
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            WWWNewReqRec,              // Object_method name
            OSELF, self,               // Self
            POINTER, rr)               // Server record pointer
{

    // Save ourself
    context->SetObjectVariable("CSELF", context->NewPointer(rr));

    return 0;
}


/**
 * Function:  WWWReqRecConnection
 *
 * Returns the connection record pointer.
 *
 * @return        Pointer
 */
RexxMethod1(POINTER,                   // Return type
            WWWReqRecConnection,       // Routine name
            CSELF, self)               // self
{
    request_rec *r = (request_rec *)self;

    return r->connection;
}


/**
 * Function:  WWWReqRecServer
 *
 * Returns the server record pointer.
 *
 * @return        Pointer
 */
RexxMethod1(POINTER,                   // Return type
            WWWReqRecServer,           // Routine name
            CSELF, self)               // self
{
    request_rec *r = (request_rec *)self;

    return r->server;
}


/**
 * Function:  WWWReqRecNext
 *
 * Returns the next record pointer.
 *
 * @return        Pointer
 */
RexxMethod1(POINTER,                   // Return type
            WWWReqRecNext,             // Routine name
            CSELF, self)               // self
{
    request_rec *r = (request_rec *)self;

    return r->next;
}


/**
 * Function:  WWWReqRecPrev
 *
 * Returns the previous request record pointer.
 *
 * @return        Pointer
 */
RexxMethod1(POINTER,                   // Return type
            WWWReqRecPrev,             // Routine name
            CSELF, self)               // self
{
    request_rec *r = (request_rec *)self;

    return r->prev;
}


/**
 * Function:  WWWReqRecMain
 *
 * Returns the main request record pointer.
 *
 * @return        Pointer
 */
RexxMethod1(POINTER,                   // Return type
            WWWReqRecMain,             // Routine name
            CSELF, self)               // self
{
    request_rec *r = (request_rec *)self;

    return r->main;
}


/**
 * Function:  WWWReqRecIsMain
 *
 * Returns true if this is the main request.
 *
 * @return        True or False
 */
RexxMethod1(logical_t,                 // Return type
            WWWReqRecIsMain,           // Routine name
            CSELF, self)               // self
{
    request_rec *r = (request_rec *)self;

    if (r->main == NULL) {
        return TRUE;
    }

    return FALSE;
}


/**
 * Function:  WWWReqRecThe_Request
 *
 * Give direct access to the the_Request field in the request.
 *
 * @return        Pointer
 */
RexxMethod1(POINTER,                   // Return type
            WWWReqRecThe_Request,      // Routine name
            CSELF, self)               // self
{
    request_rec *r = (request_rec *)self;

    /* return the existing value */
    return r->the_request;
}


/**
 * Function:  WWWReqRecProxyreq
 *
 * Returns true if this is a proxy request.
 *
 * @return        True or False
 */
RexxMethod1(logical_t,                 // Return type
            WWWReqRecProxyreq,         // Routine name
            CSELF, self)               // self
{
    request_rec *r = (request_rec *)self;

    return r->proxyreq;
}


/**
 * Function:  WWWReqRecHeader_only
 *
 * Returns the header_only field.
 *
 * @return        int
 */
RexxMethod1(int,                       // Return type
            WWWReqRecHeader_only,      // Routine name
            CSELF, self)               // self
{
    request_rec *r = (request_rec *)self;

    return r->header_only;
}


/**
 * Function:  WWWReqRecProtocol
 *
 * Returns the protocol field.
 *
 * @return        String
 */
RexxMethod1(RexxObjectPtr,             // Return type
            WWWReqRecProtocol,         // Routine name
            CSELF, self)               // self
{
    request_rec *r = (request_rec *)self;

    return context->String(r->protocol);
}


/**
 * Function:  WWWReqRecStatus_line
 *
 * Returns/sets the status_line field.
 *
 * @param sl      The new status_line.
 *
 * @return        String
 */
RexxMethod2(RexxObjectPtr,             // Return type
            WWWReqRecStatus_line,      // Routine name
            CSELF, self,               // self
            OPTIONAL_CSTRING, sl)      // New status line
{
    request_rec *r = (request_rec *)self;

    if (sl != NULL) {
        r->status_line = apr_pstrdup(r->pool, sl);
        return context->String("");
    }
    if (r->status_line == NULL) {
        return context->String("");
    }
    return context->String(r->status_line);
}


/**
 * Function:  WWWReqRecStatus
 *
 * Returns/sets the status field.
 *
 * @param st      The new status
 *
 * @return        String
 */
RexxMethod2(int,                       // Return type
            WWWReqRecStatus,           // Routine name
            CSELF, self,               // self
            OPTIONAL_CSTRING, st)      // New status
{
    request_rec *r = (request_rec *)self;

    if (st != NULL) {
        r->status = atoi(st);
        return 0;
    }
    return r->status;
}


/**
 * Function:  WWWReqRecMethod
 *
 * Returns/sets the method field.
 *
 * @param m       The new method
 *
 * @return        String
 */
RexxMethod2(RexxObjectPtr,             // Return type
            WWWReqRecMethod,           // Routine name
            CSELF, self,               // self
            OPTIONAL_CSTRING, m)       // New method
{
    request_rec *r = (request_rec *)self;

    /* return the existing value */
    if (m != NULL) {
        r->method = apr_pstrdup(r->pool, m);
    }
    if (r->method == NULL) {
        return context->String("");
    }
    return context->String(r->method);
}


/**
 * Function:  WWWReqRecMethod_number
 *
 * Returns/sets the method_number field.
 *
 * @param m       The new method
 *
 * @return        String
 */
RexxMethod2(int,                       // Return type
            WWWReqRecMethod_number,    // Routine name
            CSELF, self,               // self
            OPTIONAL_CSTRING, m)       // New method number
{
    request_rec *r = (request_rec *)self;

    /* return the existing value */
    if (m != NULL) {
        r->method_number = atoi(m);
    }
    return r->method_number;
}


/**
 * Function:  WWWReqRecAllowed
 *
 * Returns/sets the allowed field.
 *
 * @param a       The new allowed
 *
 * @return        String
 */
RexxMethod2(int,                       // Return type
            WWWReqRecAllowed,          // Routine name
            CSELF, self,               // self
            OPTIONAL_CSTRING, a)       // New allowed number
{
    request_rec *r = (request_rec *)self;

    if (a != NULL) {
        r->allowed = atoi(a);
    }
    return r->allowed;
}


/**
 * Function:  WWWReqRecBytes_sent
 *
 * Returns the bytes_sent field.
 *
 * @return        String
 */
RexxMethod1(int,                       // Return type
            WWWReqRecBytes_sent,       // Routine name
            CSELF, self)               // self
{
    request_rec *r = (request_rec *)self;

    return r->bytes_sent;
}


/**
 * Function:  WWWReqRecHeader_in
 *
 * Returns/sets the headers_in field.
 *
 * @param key     The lookup key value
 *
 * @param val     The value for the lookup key
 *
 * @return        String
 */
RexxMethod3(RexxObjectPtr,             // Return type
            WWWReqRecHeader_in,        // Routine name
            CSELF, self,               // self
            CSTRING, key,              // Lookup key
            OPTIONAL_CSTRING, val)     // New value for the key
{
    request_rec *r = (request_rec *)self;

    if (val != NULL) {
        apr_table_set(r->headers_in, key, val);
        return context->String("");
    }
    return context->String((char *)apr_table_get(r->headers_in, key));
}


/**
 * Function:  WWWReqRecHeader_out
 *
 * Sets the headers_out field.
 *
 * @param key     The lookup key value
 *
 * @param val     The value for the lookup key
 *
 * @return        Zero
 */
RexxMethod3(int,                       // Return type
            WWWReqRecHeader_out,       // Routine name
            CSELF, self,               // self
            CSTRING, key,              // Lookup key
            CSTRING, val)              // New value for the key
{
    request_rec *r = (request_rec *)self;

    apr_table_set(r->headers_out, key, val);

    return 0;
}


/**
 * Function:  WWWReqRecErr_header_out
 *
 * Returns/sets the err_header_out field.
 *
 * @param key     The lookup key value
 *
 * @param val     The value for the lookup key
 *
 * @return        String
 */
RexxMethod3(RexxObjectPtr,             // Return type
            WWWReqRecErr_header_out,   // Routine name
            CSELF, self,               // self
            CSTRING, key,              // Lookup key
            OPTIONAL_CSTRING, val)     // New value for the key
{
    request_rec *r = (request_rec *)self;

    if (val != NULL) {
        apr_table_set(r->err_headers_out, key, val);
        return context->String("");
    }
    return context->String((char *)apr_table_get(r->err_headers_out, key));
}


/**
 * Function:  WWWReqRecSubprocess_env
 *
 * Returns/sets the subprocess_env field.
 *
 * @param key     The lookup key value
 *
 * @param val     The value for the lookup key
 *
 * @return        String
 */
RexxMethod3(RexxObjectPtr,             // Return type
            WWWReqRecSubprocess_env,   // Routine name
            CSELF, self,               // self
            CSTRING, key,              // Lookup key
            OPTIONAL_CSTRING, val)     // New value for the key
{
    request_rec *r = (request_rec *)self;

    if (val != NULL) {
        apr_table_set(r->subprocess_env, key, val);
        return context->String("");
    }
    return context->String((char *)apr_table_get(r->subprocess_env, key));
}


/**
 * Function:  WWWReqRecNotes
 *
 * Returns/sets the subprocess_env field.
 *
 * @param key     The lookup key value
 *
 * @param val     The value for the lookup key
 *
 * @return        String
 */
RexxMethod3(RexxObjectPtr,             // Return type
            WWWReqRecNotes,            // Routine name
            CSELF, self,               // self
            CSTRING, key,              // Lookup key
            OPTIONAL_CSTRING, val)     // New value for the key
{
    request_rec *r = (request_rec *)self;

    if (val != NULL) {
        apr_table_set(r->notes, key, val);
        return context->String("");
    }
    return context->String((char *)apr_table_get(r->notes, key));
}


/**
 * Function:  WWWReqRecContent_type
 *
 * Returns/sets the content_type field.
 *
 * @param type    The new type
 *
 * @return        String
 */
RexxMethod2(RexxObjectPtr,             // Return type
            WWWReqRecContent_type,     // Routine name
            CSELF, self,               // self
            OPTIONAL_CSTRING, type)    // New type
{
    request_rec *r = (request_rec *)self;

    if (type != NULL) {
        r->content_type = apr_pstrdup(r->pool, type);
        return context->String("");
    }
    return context->String(r->content_type);
}


/**
 * Function:  WWWReqRecContent_encoding
 *
 * Returns/sets the content_encoding field.
 *
 * @param type    The new type
 *
 * @return        String
 */
RexxMethod2(RexxObjectPtr,             // Return type
            WWWReqRecContent_encoding, // Routine name
            CSELF, self,               // self
            OPTIONAL_CSTRING, type)    // New type
{
    request_rec *r = (request_rec *)self;

    if (type != NULL) {
        r->content_encoding = apr_pstrdup(r->pool, type);
        return context->String("");
    }
    return context->String(r->content_encoding);
}


/**
 * Function:  WWWReqRecHandler
 *
 * Returns/sets the handler field.
 *
 * @param type    The new type
 *
 * @return        String
 */
RexxMethod2(RexxObjectPtr,             // Return type
            WWWReqRecHandler,          // Routine name
            CSELF, self,               // self
            OPTIONAL_CSTRING, type)    // New type
{
    request_rec *r = (request_rec *)self;

    if (type != NULL) {
        r->handler = apr_pstrdup(r->pool, type);
        return context->String("");
    }
    return context->String(r->handler);
}


/**
 * Function:  WWWReqRecContent_languages
 *
 * Returns/sets the content_languages field.
 *
 * @param lang    The new language
 *
 * @return        String
 */
RexxMethod2(RexxObjectPtr,             // Return type
            WWWReqRecContent_languages, // Routine name
            CSELF, self,               // self
            OPTIONAL_CSTRING, lang)    // New language
{
    request_rec *r = (request_rec *)self;
    char **element;
    char *list;

    if (lang != NULL) {
        /* check to see if we need to create a new array */
        if (r->content_languages == NULL) {
            r->content_languages = apr_array_make(r->pool, 0, sizeof(char *));
        }
        /* add the new array element */
        element = (char **)apr_array_push(r->content_languages);
        *element = apr_pstrdup(r->pool, lang);
        return context->String("");
    }
    /* return the existing values */
    if (r->content_languages == NULL) {
        return context->String("");
    }
    list = apr_array_pstrcat(r->pool, r->content_languages, ',');
    return context->String(list);
}


/**
 * Function:  WWWReqRecNo_cache
 *
 * Returns/sets the no_cache field.
 *
 * @param lang    The new no_cache
 *
 * @return        Zero
 */
RexxMethod2(int,                       // Return type
            WWWReqRecNo_cache,         // Routine name
            CSELF, self,               // self
            OPTIONAL_CSTRING, val)     // New no_cache value
{
    request_rec *r = (request_rec *)self;

    /* Set/return the existing value */
    if (val != NULL) {
        r->no_cache = atoi(val);
        return 0;
    }
    return r->no_cache;
}


/**
 * Function:  WWWReqRecUri
 *
 * Returns/sets the uri field.
 *
 * @param uri     The new uri
 *
 * @return        String
 */
RexxMethod2(RexxObjectPtr,             // Return type
            WWWReqRecUri,              // Routine name
            CSELF, self,               // self
            OPTIONAL_CSTRING, uri)     // New uri
{
    request_rec *r = (request_rec *)self;

    if (uri != NULL) {
        r->uri = apr_pstrdup(r->pool, uri);
        return context->String("");
    }
    return context->String(r->uri);
}


/**
 * Function:  WWWReqRecFilename
 *
 * Returns/sets the filename field.
 *
 * @param fn      The new fn
 *
 * @return        String
 */
RexxMethod2(RexxObjectPtr,             // Return type
            WWWReqRecFilename,         // Routine name
            CSELF, self,               // self
            OPTIONAL_CSTRING, fn)      // New filename
{
    request_rec *r = (request_rec *)self;

    if (fn != NULL) {
        r->filename = apr_pstrdup(r->pool, fn);
        return context->String("");
    }
    return context->String(r->filename);
}


/**
 * Function:  WWWReqRecPath_info
 *
 * Returns/sets the path_info field.
 *
 * @param pi      The new pi
 *
 * @return        String
 */
RexxMethod2(RexxObjectPtr,             // Return type
            WWWReqRecPath_info,        // Routine name
            CSELF, self,               // self
            OPTIONAL_CSTRING, pi)      // New path_info
{
    request_rec *r = (request_rec *)self;

    if (pi != NULL) {
        r->path_info = apr_pstrdup(r->pool, pi);
        return context->String("");
    }
    return context->String(r->path_info);
}


/**
 * Function:  WWWReqRecArgs
 *
 * Returns the args field.
 *
 * @return        String
 */
RexxMethod1(RexxObjectPtr,             // Return type
            WWWReqRecArgs,             // Routine name
            CSELF, self)               // self
{
    request_rec *r = (request_rec *)self;

    return context->String(r->args);
}


/**
 * Function:  WWWReqRecFinfo_stmode
 *
 * Returns the finfo_stmode field.
 *
 * @return        String
 */
RexxMethod1(RexxObjectPtr,             // Return type
            WWWReqRecFinfo_stmode,     // Routine name
            CSELF, self)               // self
{
    request_rec *r = (request_rec *)self;
    char mode[50];

    sprintf(mode, "%X", r->finfo.filetype);
    return context->String(mode);
}


/**
 * Function:  WWWReqRecUser
 *
 * Returns the user field.
 *
 * @return        String
 */
RexxMethod1(RexxObjectPtr,             // Return type
            WWWReqRecUser,             // Routine name
            CSELF, self)               // self
{
    request_rec *r = (request_rec *)self;

    return context->String(r->user);
}


/**
 * Function:  WWWReqRecAuth_type
 *
 * Returns the auth_type field.
 *
 * @return        String
 */
RexxMethod1(RexxObjectPtr,             // Return type
            WWWReqRecAuth_type,        // Routine name
            CSELF, self)               // self
{
    request_rec *r = (request_rec *)self;

    return context->String(r->ap_auth_type);
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* The following functions support the Rexx Server_rec class declared in the  */
/* file Apache.cls. This file can be included into an ooRexx program via      */
/* the ::REQUIRES directive.                                                  */
/*                                                                            */
/*----------------------------------------------------------------------------*/


/**
 * Method:  init
 *
 * Create a server_rec.
 *
 * @param sr      The server record pointer.
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            WWWNewSrvRec,             // Object_method name
            OSELF, self,               // Self
            POINTER, sr)               // Server record pointer
{

    // Save ourself
    context->SetObjectVariable("CSELF", context->NewPointer(sr));

    return 0;
}


/**
 * Function:  WWWSrvRecServer_admin
 *
 * Returns the server_admin field.
 *
 * @return        String
 */
RexxMethod1(RexxObjectPtr,             // Return type
            WWWSrvRecServer_admin,     // Routine name
            CSELF, self)               // self
{
    server_rec *s = (server_rec *)self;

    return context->String(s->server_admin);
}


/**
 * Function:  WWWSrvRecServer_hostname
 *
 * Returns the server_hostname field.
 *
 * @return        String
 */
RexxMethod1(RexxObjectPtr,             // Return type
            WWWSrvRecServer_hostname,  // Routine name
            CSELF, self)               // self
{
    server_rec *s = (server_rec *)self;

    return context->String(s->server_hostname);
}


/**
 * Function:  WWWSrvRecPort
 *
 * Returns the port field.
 *
 * @return        Port number
 */
RexxMethod1(int,                       // Return type
            WWWSrvRecPort,             // Routine name
            CSELF, self)               // self
{
    server_rec *s = (server_rec *)self;

    return s->port;
}


/**
 * Function:  WWWSrvRecIs_virtual
 *
 * Returns the is_virtual field.
 *
 * @return        0 or 1
 */
RexxMethod1(int,                       // Return type
            WWWSrvRecIs_virtual,       // Routine name
            CSELF, self)               // self
{
    server_rec *s = (server_rec *)self;

    return s->is_virtual;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* The following functions support the Rexx Connection_rec class declared in  */
/* the file Apache.cls. This file can be included into an ooRexx program      */
/* via the ::REQUIRES directive.                                              */
/*                                                                            */
/*----------------------------------------------------------------------------*/


/**
 * Method:  init
 *
 * Create a connection_rec.
 *
 * @param cr      The connection record pointer.
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            WWWNewCnxRec,              // Object_method name
            OSELF, self,               // Self
            POINTER, cr)               // Connection record pointer
{

    // Save ourself
    context->SetObjectVariable("CSELF", context->NewPointer(cr));

    return 0;
}


/**
 * Function:  WWWCnxRecAborted
 *
 * Returns the aborted field.
 *
 * @return        0 or 1
 */
RexxMethod1(int,                       // Return type
            WWWCnxRecAborted,          // Routine name
            CSELF, self)               // self
{
    conn_rec *c = (conn_rec *)self;

    return c->aborted;
}






// build the actual entry list
RexxMethodEntry mod_oorexx_methods[] = {
    REXX_METHOD(WWWReqRecConnection, WWWReqRecConnection),
    REXX_METHOD(WWWReqRecServer, WWWReqRecServer),
    REXX_METHOD(WWWReqRecNext, WWWReqRecNext),
    REXX_METHOD(WWWReqRecPrev, WWWReqRecPrev),
    REXX_METHOD(WWWReqRecMain, WWWReqRecMain),
    REXX_METHOD(WWWReqRecIsMain, WWWReqRecIsMain),
    REXX_METHOD(WWWReqRecThe_Request, WWWReqRecThe_Request),
    REXX_METHOD(WWWReqRecProxyreq, WWWReqRecProxyreq),
    REXX_METHOD(WWWReqRecHeader_only, WWWReqRecHeader_only),
    REXX_METHOD(WWWReqRecProtocol, WWWReqRecProtocol),
    REXX_METHOD(WWWReqRecStatus_line, WWWReqRecStatus_line),
    REXX_METHOD(WWWReqRecStatus, WWWReqRecStatus),
    REXX_METHOD(WWWReqRecMethod, WWWReqRecMethod),
    REXX_METHOD(WWWReqRecMethod_number, WWWReqRecMethod_number),
    REXX_METHOD(WWWReqRecAllowed, WWWReqRecAllowed),
    REXX_METHOD(WWWReqRecBytes_sent, WWWReqRecBytes_sent),
    REXX_METHOD(WWWReqRecHeader_in, WWWReqRecHeader_in),
    REXX_METHOD(WWWReqRecHeader_out, WWWReqRecHeader_out),
    REXX_METHOD(WWWReqRecSubprocess_env, WWWReqRecSubprocess_env),
    REXX_METHOD(WWWReqRecNotes, WWWReqRecNotes),
    REXX_METHOD(WWWReqRecContent_type, WWWReqRecContent_type),
    REXX_METHOD(WWWReqRecContent_encoding, WWWReqRecContent_encoding),
    REXX_METHOD(WWWReqRecHandler, WWWReqRecHandler),
    REXX_METHOD(WWWReqRecContent_languages, WWWReqRecContent_languages),
    REXX_METHOD(WWWReqRecNo_cache, WWWReqRecNo_cache),
    REXX_METHOD(WWWReqRecUri, WWWReqRecUri),
    REXX_METHOD(WWWReqRecFilename, WWWReqRecFilename),
    REXX_METHOD(WWWReqRecPath_info, WWWReqRecPath_info),
    REXX_METHOD(WWWReqRecArgs, WWWReqRecArgs),
    REXX_METHOD(WWWReqRecFinfo_stmode, WWWReqRecFinfo_stmode),
    REXX_METHOD(WWWReqRecUser, WWWReqRecUser),
    REXX_METHOD(WWWReqRecAuth_type, WWWReqRecAuth_type),
    REXX_METHOD(WWWSrvRecServer_admin, WWWSrvRecServer_admin),
    REXX_METHOD(WWWSrvRecServer_hostname, WWWSrvRecServer_hostname),
    REXX_METHOD(WWWSrvRecPort, WWWSrvRecPort),
    REXX_METHOD(WWWSrvRecIs_virtual, WWWSrvRecIs_virtual),
    REXX_METHOD(WWWCnxRecAborted, WWWCnxRecAborted),
    REXX_LAST_METHOD()
};


// build the actual entry list
RexxRoutineEntry mod_oorexx_routines[] = {
    REXX_TYPED_ROUTINE(WWWGetVersion, WWWGetVersion),
    REXX_TYPED_ROUTINE(WWWLogError, WWWLogError),
    REXX_TYPED_ROUTINE(WWWLogWarning, WWWLogWarning),
    REXX_TYPED_ROUTINE(WWWLogInfo, WWWLogInfo),
    REXX_TYPED_ROUTINE(WWWLogReason, WWWLogReason),
    REXX_TYPED_ROUTINE(WWWSendHTTPHeader, WWWSendHTTPHeader),
    REXX_TYPED_ROUTINE(WWWAddCookie, WWWAddCookie),
    REXX_TYPED_ROUTINE(WWWSetHeaderValue, WWWSetHeaderValue),
    REXX_TYPED_ROUTINE(WWWGetArgs, WWWGetArgs),
    REXX_TYPED_ROUTINE(WWWGetCookies, WWWGetCookies),
    REXX_TYPED_ROUTINE(WWWSub_Req_Lookup_URI, WWWSub_Req_Lookup_URI),
    REXX_TYPED_ROUTINE(WWWSub_Req_Lookup_File, WWWSub_Req_Lookup_File),
    REXX_TYPED_ROUTINE(WWWRun_Sub_Req, WWWRun_Sub_Req),
    REXX_TYPED_ROUTINE(WWWInternal_Redirect, WWWInternal_Redirect),
    REXX_TYPED_ROUTINE(WWWEscape_Path, WWWEscape_Path),
    REXX_TYPED_ROUTINE(WWWConstruct_URL, WWWConstruct_URL),
    REXX_TYPED_ROUTINE(WWWHTTP_Time, WWWHTTP_Time),
    REXX_LAST_ROUTINE()
};


RexxPackageEntry mod_oorexx_package_entry = {
    STANDARD_PACKAGE_HEADER
    REXX_INTERPRETER_4_0_0,              // anything after 4.0.0 will work
    "Mod_ooRexx",                        // name of the package
    "2.2.0",                             // package information
    NULL,                                // no load/unload functions
    NULL,
    mod_oorexx_routines,                 // the exported routines
    mod_oorexx_methods                   // the exported methods
};

// package loading stub.
OOREXX_GET_PACKAGE(mod_oorexx);

