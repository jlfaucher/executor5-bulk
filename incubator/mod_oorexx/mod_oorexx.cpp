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
/* Data declarations.                                                         */
/*                                                                            */
/*----------------------------------------------------------------------------*/

const char *version = MOD_OOREXX_VERSION;
const char *compDate = __DATE__;
const char *compTime = __TIME__;

static const char *lf = "\n";


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Function:    CreateTempFile                                                */
/*                                                                            */
/* Description: Create a temporary file.                                      */
/*                                                                            */
/*----------------------------------------------------------------------------*/

char * CreateTempFile(request_rec *r, const char *fntemplate)
{
    char * TempName;
    int i, j;
    unsigned int fno;
    char *rbuf;
    char templatechar = '?';
    int len;
    apr_file_t *fh;

    /* get a random number */
    srand( (unsigned)time(NULL));
    fno = (unsigned int)rand();

    /* copy file name to our buffer */
    TempName = apr_pstrdup(r->pool, fntemplate);

    /* substitute pattern in file name and try to open the file */
    do {
        rbuf = apr_psprintf(r->pool, "%05lu", (unsigned long)fno);
        len = strlen(rbuf);
        for (i = 0, j = len - 1; i < strlen(fntemplate) && j > (len - 6); i++) {
            if (fntemplate[i] == templatechar) {
                TempName[i] = rbuf[j--];
            }
        }
        fno++;
    } while (apr_file_open(&fh, TempName, APR_WRITE | APR_CREATE | APR_EXCL,
             APR_OS_DEFAULT, r->pool) != APR_SUCCESS);

    /* now close the opened file so it can be reopened by Rexx */
    apr_file_close(fh);

    return(TempName);
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Function:    SetooRexxVar                                                    */
/*                                                                            */
/* Description: Set a Rexx variable.                                          */
/*                                                                            */
/*----------------------------------------------------------------------------*/

int SetooRexxVar(char *varname, char *value)
{
    SHVBLOCK   block;
    int rc;

    block.shvcode = RXSHV_SET;
    block.shvret = (unsigned char) 0;
    block.shvnext = NULL;

    MAKERXSTRING(block.shvname, varname, strlen(varname));
    block.shvnamelen = strlen(varname);

    /* The possibility exists that we will be passed a NULL ptr for the value */
    /* of the Rexx variable so take care of that possibility when setting up  */
    /* the shvvalue and shvvaluelen variables.                                */
    if (value == NULL) {
        MAKERXSTRING(block.shvvalue, (char *)"\0", 0);
    }
    else {
        MAKERXSTRING(block.shvvalue, value, strlen(value));
    }
    block.shvvaluelen = strlen(value);

    rc = RexxVariablePool(&block);
    return rc;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Function:    ooRexx_IO_Exit                                                */
/*                                                                            */
/* Description: I/O exit for the Rexx procedure.                              */
/*                                                                            */
/*----------------------------------------------------------------------------*/

int REXXENTRY ooRexx_IO_Exit(RexxExitContext *context, int ExitNumber, int Subfunction,
                             PEXIT ParmBlock)
{
    RXSIOSAY_PARM *sparm;
    int32_t type;

    /* Get the request_rec pointer */
    RexxDirectoryObject dir = (RexxDirectoryObject)context->GetLocalEnvironment();
    RexxPointerObject rxr = (RexxPointerObject)context->DirectoryAt(dir, REQUEST_REC_PTR);
    request_rec *r = (request_rec *)context->PointerValue(rxr);
    RexxObjectPtr reqtype = context->DirectoryAt(dir, REQUEST_HANDLER_TYPE);
    context->ObjectToInt32(reqtype, &type);

    /* Perform the exit function */
    switch (Subfunction) {
    case RXSIOSAY:
        if (type) {
            sparm = (RXSIOSAY_PARM *)ParmBlock;
            modoorexx_debug(r->server, sparm->rxsio_string.strptr);
            ap_rputs(sparm->rxsio_string.strptr, r);
            ap_rputs(lf, r);
        }
        return RXEXIT_HANDLED;
    case RXSIOTRC:
        sparm = (RXSIOSAY_PARM *)ParmBlock;
        ap_log_rerror(APLOG_MARK, APLOG_NOERRNO | APLOG_ERR, 0, r,
                  (const char *)sparm->rxsio_string.strptr);
        return RXEXIT_HANDLED;
    case RXSIOTRD:
    case RXSIODTR:
        /* Do not allow any input or output */
        return (RXEXIT_HANDLED);
    default:
        break;
    }

    return RXEXIT_RAISE_ERROR;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Function:    ooRexx_INI_Exit                                               */
/*                                                                            */
/* Description: INI exit for the Rexx procedure.                              */
/*                                                                            */
/* Notes: Do NOT try to read the POST name/value pairs from the browser       */
/*        client in this exit. Really bad things will happen.                 */
/*                                                                            */
/*----------------------------------------------------------------------------*/

int REXXENTRY ooRexx_INI_Exit(RexxExitContext *context, int ExitNumber, int Subfunction,
                              PEXIT ParmBlock)
{
    request_rec *subr;
    char *varname;
    rexx_config *c;
    oorexx_var_rec *sr;

    /* Get the request_rec pointer */
    RexxDirectoryObject dir = (RexxDirectoryObject)context->GetLocalEnvironment();
    RexxPointerObject rxr = (RexxPointerObject)context->DirectoryAt(dir, REQUEST_REC_PTR);
    request_rec *r = (request_rec *)context->PointerValue(rxr);

    /* Get the Rexx config info */
    c = (rexx_config *)ap_get_module_config(r->per_dir_config,
                                            &oorexx_module);

    /* Perform the exit function */
    switch (Subfunction) {
    case RXINIEXT:
//      modoorexx_debug(r->server, "Entering Rexx_INI_Exit routine.");

        /* Set our standard CGI Rexx variables */
        SetooRexxVar((char *)"WWWAUTH_TYPE", r->ap_auth_type);
        SetooRexxVar((char *)"WWWCONTENT_LENGTH",
                     (char *)apr_table_get(r->headers_in, "Content-Length"));
        SetooRexxVar((char *)"WWWCONTENT_TYPE",
                     (char *)apr_table_get(r->headers_in, "Content-Type"));
        SetooRexxVar((char *)"WWWGATEWAY_INTERFACE", (char *)version);
        SetooRexxVar((char *)"WWWHTTP_USER_ACCEPT",
                     (char *)apr_table_get(r->headers_in, "Accept"));
        SetooRexxVar((char *)"WWWHTTP_USER_AGENT",
                     (char *)apr_table_get(r->headers_in, "User-Agent"));
        SetooRexxVar((char *)"WWWPATH_INFO",(char *)r->path_info);

        subr = ap_sub_req_lookup_uri(r->path_info, r, NULL);
        SetooRexxVar((char *)"WWWPATH_TRANSLATED", subr->filename);
        ap_destroy_sub_req(subr);

        SetooRexxVar((char *)"WWWQUERY_STRING", (char *)r->args);
        SetooRexxVar((char *)"WWWREMOTE_ADDR", r->connection->remote_ip);
        SetooRexxVar((char *)"WWWREMOTE_HOST",
                     (char *)ap_get_remote_host(r->connection,
                                                r->per_dir_config,
                                                REMOTE_HOST, NULL));
        SetooRexxVar((char *)"WWWREMOTE_IDENT", (char *)ap_get_remote_logname(r));
        SetooRexxVar((char *)"WWWREMOTE_USER", r->user);
        SetooRexxVar((char *)"WWWREQUEST_METHOD", (char *)r->method);
        SetooRexxVar((char *)"WWWSCRIPT_NAME", (char *)r->uri);
        SetooRexxVar((char *)"WWWSERVER_NAME", (char *)ap_get_server_name(r));

        varname = apr_psprintf(r->pool, "%d", ap_get_server_port(r));
        SetooRexxVar((char *)"WWWSERVER_PORT", varname);

        SetooRexxVar((char *)"WWWSERVER_PROTOCOL", (char *)r->protocol);
        SetooRexxVar((char *)"WWWSERVER_SOFTWARE", (char *)ap_get_server_version());

        /* Define some other useful variables to the Rexx program. */
        SetooRexxVar((char *)"WWWDEFAULT_TYPE", (char *)ap_default_type(r));
        if (r->main == NULL) {
            SetooRexxVar((char *)"WWWIS_MAIN_REQUEST", (char *)"1");
        }
        else {
            SetooRexxVar((char *)"WWWIS_MAIN_REQUEST", (char *)"0");
        }
        SetooRexxVar((char *)"WWWSERVER_ROOT",
                   ap_server_root_relative (r->pool, ""));
        SetooRexxVar((char *)"WWWUNPARSEDURI", (char *) r->unparsed_uri);
        SetooRexxVar((char *)"WWWURI", r->uri);
        SetooRexxVar((char *)"WWWFILENAME", r->filename);
        SetooRexxVar((char *)"WWWPOST_STRING", NULL);

        /* Define the Rexx variables defined in the Apache httpd.conf file.   */
        sr = c->rexxvars;
        while (sr != NULL) {
            SetooRexxVar(sr->oorexx_var_name, sr->oorexx_var_value);
            sr = sr->next;
        }

        /* Define the Rexx RSP compiler information variables. */
        SetooRexxVar((char *)"WWWRSPCOMPILER", c->rspcompiler);
        SetooRexxVar((char *)"WWWFNAMETEMPLATE", c->fnametemplate);

//      modoorexx_debug(r->server, "Exiting Rexx_INI_Exit routine.");

        return RXEXIT_HANDLED;
    default:
        break;
    }

    return RXEXIT_RAISE_ERROR;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Function:    get_oorexx_err_str                                            */
/*                                                                            */
/* Description: Return the Rexx error string.                                 */
/*                                                                            */
/*----------------------------------------------------------------------------*/

const char *get_oorexx_err_str(request_rec *r, int rxrc)
{
    const char *errstr;

    switch (rxrc) {
    case -3:
        errstr = "Error 3  Failure during initialization. "
         "Program may be missing or unreadable.";
        break;
    case -4:
        errstr = "Error 4  Program interrupted.";
        break;
    case -5:
        errstr = "Error 5  System resources exhausted.";
        break;
    case -6:
        errstr = "Error 6  Unmatched \"/*\" or quote.";
        break;
    case -7:
        errstr = "Error 7  WHEN or OTHERWISE expected.";
        break;
    case -8:
        errstr = "Error 8  Unexpected THEN or ELSE.";
        break;
    case -9:
        errstr = "Error 9  Unexpected WHEN or OTHERWISE.";
        break;
    case -10:
        errstr = "Error 10  Uexpected or unmatched END.";
        break;
    case -11:
        errstr = "Error 11  Control Stack full.";
        break;
    case -13:
        errstr = "Error 13  Invalid character in program.";
        break;
    case -14:
        errstr = "Error 14  Incomplete DO/SELECT/IF.";
        break;
    case -15:
        errstr = "Error 15  Invalid hexadecimal or binary string.";
        break;
    case -16:
        errstr = "Error 16  Label not found.";
        break;
    case -17:
        errstr = "Error 17  Unexpected PROCEDURE.";
        break;
    case -18:
        errstr = "Error 18  THEN expected.";
        break;
    case -19:
        errstr = "Error 19  String or symbol expected.";
        break;
    case -20:
        errstr = "Error 20  Symbol expected.";
        break;
    case -21:
        errstr = "Error 21  Invalid data on end of clause.";
        break;
    case -22:
        errstr = "Error 22  Invalid character string.";
        break;
    case -23:
        errstr = "Error 23  Invalid data string.";
        break;
    case -24:
        errstr = "Error 24  Invalid TRACE request.";
        break;
    case -25:
        errstr = "Error 25  Invalid subkeyword found.";
        break;
    case -26:
        errstr = "Error 26  Invalid whole number.";
        break;
    case -27:
        errstr = "Error 27  Invalid DO syntax.";
        break;
    case -28:
        errstr = "Error 28  Invalid LEAVE or ITERATE.";
        break;
    case -29:
        errstr = "Error 29  Environment name too long.";
        break;
    case -30:
        errstr = "Error 30  Name or string too long.";
        break;
    case -31:
        errstr = "Error 31  Name starts with number or \".\".";
        break;
    case -33:
        errstr = "Error 33  Invalid expression result.";
        break;
    case -34:
        errstr = "Error 34  Logical value not 0 or 1.";
        break;
    case -35:
        errstr = "Error 35  Invalid expression.";
        break;
    case -36:
        errstr = "Error 36  Unmatched \"(\" or \"[\" in expression.";
        break;
    case -37:
        errstr = "Error 37  Unexpected \",\", \")\", or \"]\".";
        break;
    case -38:
        errstr = "Error 38  Invalid template or pattern.";
        break;
    case -40:
        errstr = "Error 40  Incorrect call to routine.";
        break;
    case -41:
        errstr = "Error 41  Bad arithmetic conversion.";
        break;
    case -42:
        errstr = "Error 42  Arithmetic overflow/underflow.";
        break;
    case -43:
        errstr = "Error 43  Routine not found.";
        break;
    case -44:
        errstr = "Error 44  Function or message did not return data.";
        break;
    case -45:
        errstr = "Error 45  No data specified on function RETURN.";
        break;
    case -46:
        errstr = "Error 46  Invalid variable reference.";
        break;
    case -47:
        errstr = "Error 47  Unexpected lable.";
        break;
    case -48:
        errstr = "Error 48  Failure in system service.";
        break;
    case -49:
        errstr = "Error 49  Interpretation error.";
        break;
    case -90:
        errstr = "Error 90  External name not found.";
        break;
    case -91:
        errstr = "Error 91  No result object.";
        break;
    case -93:
        errstr = "Error 93  Incorrect call to method.";
        break;
    case -97:
        errstr = "Error 97  Object method not found.";
        break;
    case -98:
        errstr = "Error 98  Execution error.";
        break;
    case -99:
        errstr = "Error 99  Translation error.";
        break;
    default:
        errstr = apr_psprintf(r->pool,
                         "Unknown error %d returned from the Rexx Interpreter.",
                         (int)rxrc);
        break;
    }

    return errstr;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Function:    oorexxstart_error_processor                                     */
/*                                                                            */
/* Description: Output a page to the browser showing the error code and       */
/*              message from the Rexx Interpreter.                            */
/*                                                                            */
/*----------------------------------------------------------------------------*/

void oorexxstart_error_processor(request_rec *r, char *rxprocpath,
                                 int rxrc)
{
    char *outstr;
    const char *errstr;

    errstr = get_oorexx_err_str(r, rxrc);
    if (rxrc < 0) {
        outstr = apr_psprintf(r->pool,
                 "<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML 2.0//EN\">\n"
                 "<html><head>\n"
                 "<title>Internal Server Error</title>\n"
                 "</head></body>\n"
                 "<h1>Internal Server Error</h1>\n"
                 "<h2>Rexx Interpreter Error</h2>\n"
                 "<p>Rexx procedure filename: %s\n"
                 "<br />%s</p>\n"
                 "<hr />\n"
                 "<address>%s at %s Port %d</address>\n"
                 "</body></html>\n",
                rxprocpath, errstr, ap_get_server_version(),
                ap_get_server_name(r), ap_get_server_port(r));
    }
    else {
        outstr = apr_psprintf(r->pool,
                 "<!DOCTYPE HTML PUBLIC \"-//IETF//DTD HTML 2.0//EN\">\n"
                 "<html><head>\n"
                 "<title>Internal Server Error</title>\n"
                 "</head></body>\n"
                 "<h1>Internal Server Error</h1>\n"
                 "<h2>System Error Returned from the Rexx Interpreter</h2>\n"
                 "<p>Rexx procedure filename: %s\n"
                 "<br />%s</p>\n"
                 "<hr />\n"
                 "<address>%s at %s Port %d</address>\n"
                "</body></html>\n",
                rxprocpath, errstr, ap_get_server_version(),
                ap_get_server_name(r), ap_get_server_port(r));
    }
    ap_custom_response(r,HTTP_INTERNAL_SERVER_ERROR, outstr);
    modoorexx_error(r->server, rxprocpath);
    modoorexx_error(r->server, errstr);

    return;
}
