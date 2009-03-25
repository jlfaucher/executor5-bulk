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
/* Function:    oorexx_child_init                                             */
/*                                                                            */
/* Description: This handler is invoked when a new server is created.         */
/*                                                                            */
/*----------------------------------------------------------------------------*/

static void oorexx_child_init(apr_pool_t *pchild, server_rec *s)
{
    ooRexxSrvrConfig *cfg;
    RexxOption options[3];
    RexxContextExit exits[3];
    RexxLibraryPackage package;

    ap_add_version_component(pchild, MOD_OOREXX_VERSION);

    cfg = (ooRexxSrvrConfig *) apr_pcalloc(pchild, sizeof(ooRexxSrvrConfig));
    ap_set_module_config(s->module_config, &oorexx_module, cfg);

//  modoorexx_debug(s, "Entering rexx_child_init routine.");

    /* set up our package */
    package.registeredName = "mod_oorexx";
    package.table = &mod_oorexx_package_entry;

    /* set up our ooRexx content instance exits */
    exits[0].handler = ooRexx_INI_Exit;
    exits[0].sysexit_code = RXINI;
    exits[1].handler = ooRexx_IO_Exit;
    exits[1].sysexit_code = RXSIO;
    exits[2].sysexit_code = 0;

    /* set up our ooRexx instance options */
    options[0].optionName = DIRECT_EXITS;
    options[0].option = (void *)exits;
    options[1].optionName = REGISTER_LIBRARY;
    options[1].option = (void *)&package;
    options[2].optionName = NULL;

    /* set up our ooRexx content handler instance */
    RexxCreateInterpreter(&cfg->contentInst, &cfg->contentThrdInst, options);

//  modoorexx_debug(s, "Exiting rexx_child_init routine.");

    return;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Function:    oorexx_handler                                                */
/*                                                                            */
/* Description: Main content handler. This function calls Rexx to handle the  */
/*              HTTP request.                                                 */
/*                                                                            */
/*----------------------------------------------------------------------------*/

static int oorexx_handler(request_rec *r)
{
    char *rxprocpath;
    rexx_config *c;
    ooRexxSrvrConfig *cfg;
    int32_t rc;

    /* test for our MIME types */
    if(strcmp(r->handler, "application/x-httpd-rexx-script")
        && strcmp(r->handler, "rexx_handler")) {
        return DECLINED;
    }

    modoorexx_debug(r->server, "Entering rexx_handler routine.");

    /* If we're only supposed to send header information (HEAD request), */
    /* then don't bother Rexx with the request.                          */
    if (r->header_only) {
        /* Set the content-type */
        r->content_type = "text/html";
        ap_send_http_header(r);
        modoorexx_debug(r->server, "Exiting rexx_handler routine.");
        return OK;
    }

    /* If the ooRexx script file does not exist then return an error. */
    if(r->finfo.filetype == 0) {
        return HTTP_NOT_FOUND;
    }

    /* Are we overridding the proc name through a config directive? */
    c = (rexx_config *)ap_get_module_config(r->per_dir_config, &oorexx_module);
    if (c->content != NULL) {
        rxprocpath = c->content;
    }
    else {
        rxprocpath = r->filename;
    }

    /* Get the Rexx instance */
    cfg = (ooRexxSrvrConfig *)ap_get_module_config(r->server->module_config, &oorexx_module);

    /* Set the request_rec pointer and the type for the ooRexx exits */
    RexxDirectoryObject dir = (RexxDirectoryObject)cfg->contentThrdInst->GetLocalEnvironment();
    cfg->contentThrdInst->DirectoryPut(dir, cfg->contentThrdInst->NewPointer(r),
                                       REQUEST_REC_PTR);
    cfg->contentThrdInst->DirectoryPut(dir, cfg->contentThrdInst->String("1"),
                                       REQUEST_HANDLER_TYPE);

    // create an Array object to hold the program arguments
    RexxArrayObject args = cfg->contentThrdInst->NewArray(1);
    // add the argument to the array.
    cfg->contentThrdInst->ArrayPut(args, cfg->contentThrdInst->NewPointer(r), 1);
    // call our program, using the provided arguments.
    modoorexx_debug(r->server, "Calling Rexx.");
    RexxObjectPtr result = cfg->contentThrdInst->CallProgram(rxprocpath, args);
    cfg->contentThrdInst->ObjectToInt32(result, &rc);
    // if an error occurred, get the decoded exception information
    if (cfg->contentThrdInst->CheckCondition()) {
        RexxCondition condition;
        // retrieve the error information and get it into a decoded form
        RexxDirectoryObject cond = cfg->contentThrdInst->GetConditionInfo();
        cfg->contentThrdInst->DecodeConditionInfo(cond, &condition);
        modoorexx_debug(r->server, "Exiting rexx_rsphandler routine.");
        // display the errors
        oorexxstart_error_processor(r, rxprocpath, rc);
        rc = HTTP_INTERNAL_SERVER_ERROR;
    }

    modoorexx_debug(r->server, "Exiting rexx_handler routine.");

    return rc;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Function:    oorexx_rsphandler                                             */
/*                                                                            */
/* Description: Handler for Rexx RSP files. This fuction calls Rexx to handle */
/*              an RSP request.                                               */
/*                                                                            */
/*----------------------------------------------------------------------------*/

static int oorexx_rsphandler(request_rec *r)
{
    rexx_config *c;
    ooRexxSrvrConfig *cfg;
    char * TempName;
    char *rxarg;
    int32_t rc;

    /* test for our MIME type */
    if(strcmp(r->handler, "application/x-httpd-rexx-rsp")) {
        return DECLINED;
    }

    modoorexx_debug(r->server, "Entering rexx_rsphandler routine.");
    /* If we're only supposed to send header information (HEAD request), */
    /* then don't bother Rexx with the request.                          */
    if (r->header_only) {
        /* Set the content-type */
        r->content_type = "text/html";
        ap_send_http_header(r);
//      modoorexx_debug(r->server, "Exiting rexx_rsphandler routine.");
        return OK;
    }

    /* If the rsp file does not exist then return an error. */
    if(r->finfo.filetype == 0) {
//      modoorexx_debug(r->server, "Exiting rexx_rsphandler routine.");
        return HTTP_NOT_FOUND;
    }

    /* Get a temporary file to put the Rexx program into. */
    c = (rexx_config *)ap_get_module_config(r->per_dir_config,
                                                &oorexx_module);
    TempName = CreateTempFile(r, c->fnametemplate);

    /* Get the Rexx instance */
    cfg = (ooRexxSrvrConfig *)ap_get_module_config(r->server->module_config, &oorexx_module);

    /* Set the request_rec pointer and the type for the ooRexx exits */
    RexxDirectoryObject dir = (RexxDirectoryObject)cfg->contentThrdInst->GetLocalEnvironment();
    cfg->contentThrdInst->DirectoryPut(dir, cfg->contentThrdInst->NewPointer(r),
                                       REQUEST_REC_PTR);
    cfg->contentThrdInst->DirectoryPut(dir, cfg->contentThrdInst->String("0"),
                                       REQUEST_HANDLER_TYPE);

    /* Compile the rsp file */

    // create an Array object to hold the program arguments
    RexxArrayObject args = cfg->contentThrdInst->NewArray(1);
    // add the argument to the array.
    rxarg = (char *)apr_pcalloc(r->pool, strlen(TempName) + strlen(r->filename) + 6);
    sprintf(rxarg, "\"%s\" \"%s\"", r->filename, TempName);
    cfg->contentThrdInst->ArrayPut(args, cfg->contentThrdInst->String(rxarg), 1);
    // call our program, using the provided arguments.
    modoorexx_debug(r->server, "Compiling rsp file.");
    RexxObjectPtr result = cfg->contentThrdInst->CallProgram(c->rspcompiler, args);
    modoorexx_debug(r->server, "Finished compiling rsp file.");
    cfg->contentThrdInst->ObjectToInt32(result, &rc);
    // if an error occurred, get the decoded exception information
    if (cfg->contentThrdInst->CheckCondition()) {
        RexxCondition condition;
        // retrieve the error information and get it into a decoded form
        RexxDirectoryObject cond = cfg->contentThrdInst->GetConditionInfo();
        cfg->contentThrdInst->DecodeConditionInfo(cond, &condition);
//      modoorexx_debug(r->server, "Exiting rexx_rsphandler routine.");
        // display the errors
        oorexxstart_error_processor(r, c->rspcompiler, rc);
        return HTTP_INTERNAL_SERVER_ERROR;
    }

    /* Execute the compiled rsp file */

    /* Set the type for the ooRexx exits */
    cfg->contentThrdInst->DirectoryPut(dir, cfg->contentThrdInst->String("1"),
                                       REQUEST_HANDLER_TYPE);

    // add the argument to the array.
    cfg->contentThrdInst->ArrayPut(args, cfg->contentThrdInst->NewPointer(r), 1);
    // call our program, using the provided arguments.
    modoorexx_debug(r->server, "Executing rsp file.");
    result = cfg->contentThrdInst->CallProgram(TempName, args);
    cfg->contentThrdInst->ObjectToInt32(result, &rc);
    // clean up the temporary file
    remove(TempName);
    // if an error occurred, get the decoded exception information
    if (cfg->contentThrdInst->CheckCondition()) {
        RexxCondition condition;
        // retrieve the error information and get it into a decoded form
        RexxDirectoryObject cond = cfg->contentThrdInst->GetConditionInfo();
        cfg->contentThrdInst->DecodeConditionInfo(cond, &condition);
//      modoorexx_debug(r->server, "Exiting rexx_rsphandler routine.");
        // display the errors
        oorexxstart_error_processor(r, c->rspcompiler, rc);
        return HTTP_INTERNAL_SERVER_ERROR;
    }

//  modoorexx_debug(r->server, "Exiting rexx_rsphandler routine.");

    return rc;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Function:    oorexx_translation_handler                                    */
/*                                                                            */
/* Description: This handler is invoked during a request. It can translate    */
/*              the URI in the request.                                       */
/*                                                                            */
/*----------------------------------------------------------------------------*/

int oorexx_translation_handler(request_rec *r)
{
    rexx_config *c;
    int rc = DECLINED;
    ooRexxSrvrConfig *cfg;

    /* if this is a subrequest then don't perform any function */
    if (r->main != NULL) {
        return rc;
    }

    /* Check if we need to deal with this phase */
    c = (rexx_config *)ap_get_module_config(r->per_dir_config, &oorexx_module);
    if (c->translate == NULL) {
        return rc;
    }

    /* Get the Rexx instance */
    cfg = (ooRexxSrvrConfig *)ap_get_module_config(r->server->module_config, &oorexx_module);

    /* Set the request_rec pointer and the type for the ooRexx exits */
    RexxDirectoryObject dir = (RexxDirectoryObject)cfg->contentThrdInst->GetLocalEnvironment();
    cfg->contentThrdInst->DirectoryPut(dir, cfg->contentThrdInst->NewPointer(r),
                                       REQUEST_REC_PTR);
    cfg->contentThrdInst->DirectoryPut(dir, cfg->contentThrdInst->String("0"),
                                       REQUEST_HANDLER_TYPE);

//  modoorexx_debug(r->server, "Entering rexx_translation_handler routine.");

    // create an Array object to hold the program arguments
    RexxArrayObject args = cfg->contentThrdInst->NewArray(1);
    // add the argument to the array. Note that ArrayPut() requires an
    // index that is origin-1, unlike C arrays which are origin-0.
    cfg->contentThrdInst->ArrayPut(args, cfg->contentThrdInst->NewPointer(r), 1);
    // call our program, using the provided arguments.
    RexxObjectPtr result = cfg->contentThrdInst->CallProgram(c->translate, args);
    cfg->contentThrdInst->ObjectToInt32(result, &rc);
    // if an error occurred, get the decoded exception information
    if (cfg->contentThrdInst->CheckCondition()) {
        RexxCondition condition;
        // retrieve the error information and get it into a decoded form
        RexxDirectoryObject cond = cfg->contentThrdInst->GetConditionInfo();
        cfg->contentThrdInst->DecodeConditionInfo(cond, &condition);
        // display the errors
        oorexxstart_error_processor(r, c->translate, rc);
        return HTTP_INTERNAL_SERVER_ERROR;
    }

//  modoorexx_debug(r->server, "Exiting rexx_translation_handler routine.");

    return rc;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Function:    oorexx_authentication_handler                                 */
/*                                                                            */
/* Description: This handler is invoked during a request. Authentication of   */
/*              the request is done here.                                     */
/*                                                                            */
/*----------------------------------------------------------------------------*/

int oorexx_authentication_handler(request_rec *r)
{
    rexx_config *c;
    int rc = DECLINED;
    ooRexxSrvrConfig *cfg;

    /* if this is a subrequest then don't perform any function */
    if (r->main != NULL) {
        return DECLINED;
    }

    /* Check if we need to deal with this phase */
    c = (rexx_config *)ap_get_module_config(r->per_dir_config, &oorexx_module);
    if (c->authenticate == NULL) {
        return rc;
    }

    /* Get the Rexx instance */
    cfg = (ooRexxSrvrConfig *)ap_get_module_config(r->server->module_config, &oorexx_module);

    /* Set the request_rec pointer and the type for the ooRexx exits */
    RexxDirectoryObject dir = (RexxDirectoryObject)cfg->contentThrdInst->GetLocalEnvironment();
    cfg->contentThrdInst->DirectoryPut(dir, cfg->contentThrdInst->NewPointer(r),
                                       REQUEST_REC_PTR);
    cfg->contentThrdInst->DirectoryPut(dir, cfg->contentThrdInst->String("0"),
                                       REQUEST_HANDLER_TYPE);

//  modoorexx_debug(r->server, "Entering rexx_authentication_handler routine.");

    // create an Array object to hold the program arguments
    RexxArrayObject args = cfg->contentThrdInst->NewArray(1);
    // add the argument to the array. Note that ArrayPut() requires an
    // index that is origin-1, unlike C arrays which are origin-0.
    cfg->contentThrdInst->ArrayPut(args, cfg->contentThrdInst->NewPointer(r), 1);
    // call our program, using the provided arguments.
    RexxObjectPtr result = cfg->contentThrdInst->CallProgram(c->authenticate, args);
    cfg->contentThrdInst->ObjectToInt32(result, &rc);
    // if an error occurred, get the decoded exception information
    if (cfg->contentThrdInst->CheckCondition()) {
        RexxCondition condition;
        // retrieve the error information and get it into a decoded form
        RexxDirectoryObject cond = cfg->contentThrdInst->GetConditionInfo();
        cfg->contentThrdInst->DecodeConditionInfo(cond, &condition);
        // display the errors
        oorexxstart_error_processor(r, c->authenticate, rc);
        return HTTP_INTERNAL_SERVER_ERROR;
    }

//  modoorexx_debug(r->server, "Exiting rexx_authentication_handler routine.");

    return rc;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Function:    oorexx_authorization_handler                                  */
/*                                                                            */
/* Description: This handler is invoked during a request. Authorization  of   */
/*              the request is done here.                                     */
/*                                                                            */
/*----------------------------------------------------------------------------*/

int oorexx_authorization_handler(request_rec *r)
{
    rexx_config *c;
    int rc = DECLINED;
    ooRexxSrvrConfig *cfg;

    /* if this is a subrequest then don't perform any function */
    if (r->main != NULL) {
        return DECLINED;
    }

    /* Check if we need to deal with this phase */
    c = (rexx_config *)ap_get_module_config(r->per_dir_config, &oorexx_module);
    if (c->authorize == NULL) {
        return rc;
    }

    /* Get the Rexx instance */
    cfg = (ooRexxSrvrConfig *)ap_get_module_config(r->server->module_config, &oorexx_module);

    /* Set the request_rec pointer and the type for the ooRexx exits */
    RexxDirectoryObject dir = (RexxDirectoryObject)cfg->contentThrdInst->GetLocalEnvironment();
    cfg->contentThrdInst->DirectoryPut(dir, cfg->contentThrdInst->NewPointer(r),
                                       REQUEST_REC_PTR);
    cfg->contentThrdInst->DirectoryPut(dir, cfg->contentThrdInst->String("0"),
                                       REQUEST_HANDLER_TYPE);

//  modoorexx_debug(r->server, "Entering rexx_authorization_handler routine.");

    // create an Array object to hold the program arguments
    RexxArrayObject args = cfg->contentThrdInst->NewArray(1);
    // add the argument to the array. Note that ArrayPut() requires an
    // index that is origin-1, unlike C arrays which are origin-0.
    cfg->contentThrdInst->ArrayPut(args, cfg->contentThrdInst->NewPointer(r), 1);
    // call our program, using the provided arguments.
    RexxObjectPtr result = cfg->contentThrdInst->CallProgram(c->authorize, args);
    cfg->contentThrdInst->ObjectToInt32(result, &rc);
    // if an error occurred, get the decoded exception information
    if (cfg->contentThrdInst->CheckCondition()) {
        RexxCondition condition;
        // retrieve the error information and get it into a decoded form
        RexxDirectoryObject cond = cfg->contentThrdInst->GetConditionInfo();
        cfg->contentThrdInst->DecodeConditionInfo(cond, &condition);
        // display the errors
        oorexxstart_error_processor(r, c->authorize, rc);
        return HTTP_INTERNAL_SERVER_ERROR;
    }

//  modoorexx_debug(r->server, "Exiting rexx_authorization_handler routine.");

    return rc;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Function:    oorexx_access_handler                                         */
/*                                                                            */
/* Description: This handler is invoked during a request. Access control of   */
/*              the request is done here.                                     */
/*                                                                            */
/*----------------------------------------------------------------------------*/

int oorexx_access_handler(request_rec *r)
{
    rexx_config *c;
    int rc = DECLINED;
    ooRexxSrvrConfig *cfg;

    /* if this is a subrequest then don't perform any function */
    if (r->main != NULL) {
        return DECLINED;
    }

    /* Check if we need to deal with this phase */
    c = (rexx_config *)ap_get_module_config(r->per_dir_config, &oorexx_module);
    if (c->access == NULL) {
        return rc;
    }

    /* Get the Rexx instance */
    cfg = (ooRexxSrvrConfig *)ap_get_module_config(r->server->module_config, &oorexx_module);

    /* Set the request_rec pointer and the type for the ooRexx exits */
    RexxDirectoryObject dir = (RexxDirectoryObject)cfg->contentThrdInst->GetLocalEnvironment();
    cfg->contentThrdInst->DirectoryPut(dir, cfg->contentThrdInst->NewPointer(r),
                                       REQUEST_REC_PTR);
    cfg->contentThrdInst->DirectoryPut(dir, cfg->contentThrdInst->String("0"),
                                       REQUEST_HANDLER_TYPE);

//  modoorexx_debug(r->server, "Entering rexx_access_handler routine.");

    // create an Array object to hold the program arguments
    RexxArrayObject args = cfg->contentThrdInst->NewArray(1);
    // add the argument to the array. Note that ArrayPut() requires an
    // index that is origin-1, unlike C arrays which are origin-0.
    cfg->contentThrdInst->ArrayPut(args, cfg->contentThrdInst->NewPointer(r), 1);
    // call our program, using the provided arguments.
    RexxObjectPtr result = cfg->contentThrdInst->CallProgram(c->access, args);
    cfg->contentThrdInst->ObjectToInt32(result, &rc);
    // if an error occurred, get the decoded exception information
    if (cfg->contentThrdInst->CheckCondition()) {
        RexxCondition condition;
        // retrieve the error information and get it into a decoded form
        RexxDirectoryObject cond = cfg->contentThrdInst->GetConditionInfo();
        cfg->contentThrdInst->DecodeConditionInfo(cond, &condition);
        // display the errors
        oorexxstart_error_processor(r, c->access, rc);
        return HTTP_INTERNAL_SERVER_ERROR;
    }

//  modoorexx_debug(r->server, "Exiting rexx_access_handler routine.");

    return rc;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Function:    oorexx_mime_type_handler                                      */
/*                                                                            */
/* Description: This handler is invoked during a request. Mime checking of    */
/*              the request is done here.                                     */
/*                                                                            */
/*----------------------------------------------------------------------------*/

int oorexx_mime_type_handler(request_rec *r)
{
    rexx_config *c;
    int rc = DECLINED;
    ooRexxSrvrConfig *cfg;

    /* if this is a subrequest then don't perform any function */
    if (r->main != NULL) {
        return DECLINED;
    }

    /* Check if we need to deal with this phase */
    c = (rexx_config *)ap_get_module_config(r->per_dir_config, &oorexx_module);
    if (c->mime_type == NULL) {
        return rc;
    }

    /* Get the Rexx instance */
    cfg = (ooRexxSrvrConfig *)ap_get_module_config(r->server->module_config, &oorexx_module);

    /* Set the request_rec pointer and the type for the ooRexx exits */
    RexxDirectoryObject dir = (RexxDirectoryObject)cfg->contentThrdInst->GetLocalEnvironment();
    cfg->contentThrdInst->DirectoryPut(dir, cfg->contentThrdInst->NewPointer(r),
                                       REQUEST_REC_PTR);
    cfg->contentThrdInst->DirectoryPut(dir, cfg->contentThrdInst->String("0"),
                                       REQUEST_HANDLER_TYPE);

//  modoorexx_debug(r->server, "Entering rexx_mime_type_handler routine.");

    // create an Array object to hold the program arguments
    RexxArrayObject args = cfg->contentThrdInst->NewArray(1);
    // add the argument to the array. Note that ArrayPut() requires an
    // index that is origin-1, unlike C arrays which are origin-0.
    cfg->contentThrdInst->ArrayPut(args, cfg->contentThrdInst->NewPointer(r), 1);
    // call our program, using the provided arguments.
    RexxObjectPtr result = cfg->contentThrdInst->CallProgram(c->mime_type, args);
    cfg->contentThrdInst->ObjectToInt32(result, &rc);
    // if an error occurred, get the decoded exception information
    if (cfg->contentThrdInst->CheckCondition()) {
        RexxCondition condition;
        // retrieve the error information and get it into a decoded form
        RexxDirectoryObject cond = cfg->contentThrdInst->GetConditionInfo();
        cfg->contentThrdInst->DecodeConditionInfo(cond, &condition);
        // display the errors
        oorexxstart_error_processor(r, c->mime_type, rc);
        return HTTP_INTERNAL_SERVER_ERROR;
    }

//  modoorexx_debug(r->server, "Exiting rexx_mime_type_handler routine.");

    return rc;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Function:    oorexx_fixup_handler                                          */
/*                                                                            */
/* Description: This handler is invoked during a request. Fix up of           */
/*              the request is done here.                                     */
/*                                                                            */
/*----------------------------------------------------------------------------*/

int oorexx_fixup_handler(request_rec *r)
{
    rexx_config *c;
    int rc = DECLINED;
    ooRexxSrvrConfig *cfg;

    /* if this is a subrequest then don't perform any function */
    if (r->main != NULL) {
        return DECLINED;
    }

    /* Check if we need to deal with this phase */
    c = (rexx_config *)ap_get_module_config(r->per_dir_config, &oorexx_module);
    if (c->fixup == NULL) {
        return rc;
    }

    /* Get the Rexx instance */
    cfg = (ooRexxSrvrConfig *)ap_get_module_config(r->server->module_config, &oorexx_module);

    /* Set the request_rec pointer and the type for the ooRexx exits */
    RexxDirectoryObject dir = (RexxDirectoryObject)cfg->contentThrdInst->GetLocalEnvironment();
    cfg->contentThrdInst->DirectoryPut(dir, cfg->contentThrdInst->NewPointer(r),
                                       REQUEST_REC_PTR);
    cfg->contentThrdInst->DirectoryPut(dir, cfg->contentThrdInst->String("0"),
                                       REQUEST_HANDLER_TYPE);

//  modoorexx_debug(r->server, "Entering rexx_fixup_handler routine.");

    // create an Array object to hold the program arguments
    RexxArrayObject args = cfg->contentThrdInst->NewArray(1);
    // add the argument to the array. Note that ArrayPut() requires an
    // index that is origin-1, unlike C arrays which are origin-0.
    cfg->contentThrdInst->ArrayPut(args, cfg->contentThrdInst->NewPointer(r), 1);
    // call our program, using the provided arguments.
    RexxObjectPtr result = cfg->contentThrdInst->CallProgram(c->fixup, args);
    cfg->contentThrdInst->ObjectToInt32(result, &rc);
    // if an error occurred, get the decoded exception information
    if (cfg->contentThrdInst->CheckCondition()) {
        RexxCondition condition;
        // retrieve the error information and get it into a decoded form
        RexxDirectoryObject cond = cfg->contentThrdInst->GetConditionInfo();
        cfg->contentThrdInst->DecodeConditionInfo(cond, &condition);
        // display the errors
        oorexxstart_error_processor(r, c->fixup, rc);
        return HTTP_INTERNAL_SERVER_ERROR;
    }

//  modoorexx_debug(r->server, "Exiting rexx_fixup_handler routine.");

    return rc;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Function:    oorexx_logging_handler                                        */
/*                                                                            */
/* Description: This handler is invoked during a request. Logging of          */
/*              the request is done here.                                     */
/*                                                                            */
/*----------------------------------------------------------------------------*/

int oorexx_logging_handler(request_rec *r)
{
    rexx_config *c;
    int rc = DECLINED;
    ooRexxSrvrConfig *cfg;

    /* if this is a subrequest then don't perform any function */
    if (r->main != NULL) {
        return DECLINED;
    }

    /* Check if we need to deal with this phase */
    c = (rexx_config *)ap_get_module_config(r->per_dir_config,
                                            &oorexx_module);
    if (c->logging == NULL) {
        return rc;
    }

    /* Get the Rexx instance */
    cfg = (ooRexxSrvrConfig *)ap_get_module_config(r->server->module_config, &oorexx_module);

    /* Set the request_rec pointer and the type for the ooRexx exits */
    RexxDirectoryObject dir = (RexxDirectoryObject)cfg->contentThrdInst->GetLocalEnvironment();
    cfg->contentThrdInst->DirectoryPut(dir, cfg->contentThrdInst->NewPointer(r),
                                       REQUEST_REC_PTR);
    cfg->contentThrdInst->DirectoryPut(dir, cfg->contentThrdInst->String("0"),
                                       REQUEST_HANDLER_TYPE);

//  modoorexx_debug(r->server, "Entering rexx_logging_handler routine.");

    // create an Array object to hold the program arguments
    RexxArrayObject args = cfg->contentThrdInst->NewArray(1);
    // add the argument to the array. Note that ArrayPut() requires an
    // index that is origin-1, unlike C arrays which are origin-0.
    cfg->contentThrdInst->ArrayPut(args, cfg->contentThrdInst->NewPointer(r), 1);
    // call our program, using the provided arguments.
    RexxObjectPtr result = cfg->contentThrdInst->CallProgram(c->logging, args);
    cfg->contentThrdInst->ObjectToInt32(result, &rc);
    // if an error occurred, get the decoded exception information
    if (cfg->contentThrdInst->CheckCondition()) {
        RexxCondition condition;
        // retrieve the error information and get it into a decoded form
        RexxDirectoryObject cond = cfg->contentThrdInst->GetConditionInfo();
        cfg->contentThrdInst->DecodeConditionInfo(cond, &condition);
        // display the errors
        oorexxstart_error_processor(r, c->logging, rc);
        return HTTP_INTERNAL_SERVER_ERROR;
    }

//  modoorexx_debug(r->server, "Exiting rexx_logging_handler routine.");

    return rc;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Function:    oorexx_header_parser_handler                                  */
/*                                                                            */
/* Description: This handler is invoked during a request. Header parsing of   */
/*              the request is done here.                                     */
/*                                                                            */
/*----------------------------------------------------------------------------*/

int oorexx_header_parser_handler(request_rec *r)
{
    rexx_config *c;
    int rc = DECLINED;
    ooRexxSrvrConfig *cfg;

    /* if this is a subrequest then don't perform any function */
    if (r->main != NULL) {
        return DECLINED;
    }

    /* Check if we need to deal with this phase */
    c = (rexx_config *)ap_get_module_config(r->per_dir_config,
                                            &oorexx_module);
    if (c->header_parser == NULL) {
        return rc;
    }

    /* Get the Rexx instance */
    cfg = (ooRexxSrvrConfig *)ap_get_module_config(r->server->module_config, &oorexx_module);

    /* Set the request_rec pointer and the type for the ooRexx exits */
    RexxDirectoryObject dir = (RexxDirectoryObject)cfg->contentThrdInst->GetLocalEnvironment();
    cfg->contentThrdInst->DirectoryPut(dir, cfg->contentThrdInst->NewPointer(r),
                                       REQUEST_REC_PTR);
    cfg->contentThrdInst->DirectoryPut(dir, cfg->contentThrdInst->String("0"),
                                       REQUEST_HANDLER_TYPE);

//  modoorexx_debug(r->server, "Entering rexx_header_parser_handler routine.");

    // create an Array object to hold the program arguments
    RexxArrayObject args = cfg->contentThrdInst->NewArray(1);
    // add the argument to the array. Note that ArrayPut() requires an
    // index that is origin-1, unlike C arrays which are origin-0.
    cfg->contentThrdInst->ArrayPut(args, cfg->contentThrdInst->NewPointer(r), 1);
    // call our program, using the provided arguments.
    RexxObjectPtr result = cfg->contentThrdInst->CallProgram(c->header_parser, args);
    cfg->contentThrdInst->ObjectToInt32(result, &rc);
    // if an error occurred, get the decoded exception information
    if (cfg->contentThrdInst->CheckCondition()) {
        RexxCondition condition;
        // retrieve the error information and get it into a decoded form
        RexxDirectoryObject cond = cfg->contentThrdInst->GetConditionInfo();
        cfg->contentThrdInst->DecodeConditionInfo(cond, &condition);
        // display the errors
        oorexxstart_error_processor(r, c->header_parser, rc);
        return HTTP_INTERNAL_SERVER_ERROR;
    }

//  modoorexx_debug(r->server, "Exiting rexx_header_parser_handler routine.");

    cfg->contentThrdInst->ObjectToInt32(result, &rc);
    return rc;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Function:    oorexx_post_request_handler                                   */
/*                                                                            */
/* Description: This handler is invoked during a request. Post processing of  */
/*              the request is done here.                                     */
/*                                                                            */
/*----------------------------------------------------------------------------*/

int oorexx_post_request_handler(request_rec *r)
{
    rexx_config *c;
    int rc = DECLINED;
    ooRexxSrvrConfig *cfg;

    /* if this is a subrequest then don't perform any function */
    if (r->main != NULL) {
        return DECLINED;
    }

    /* Check if we need to deal with this phase */
    c = (rexx_config *)ap_get_module_config(r->per_dir_config,
                                            &oorexx_module);
    if (c->post_read == NULL) {
        return DECLINED;
    }

    /* Get the Rexx instance */
    cfg = (ooRexxSrvrConfig *)ap_get_module_config(r->server->module_config, &oorexx_module);

    /* Set the request_rec pointer and the type for the ooRexx exits */
    RexxDirectoryObject dir = (RexxDirectoryObject)cfg->contentThrdInst->GetLocalEnvironment();
    cfg->contentThrdInst->DirectoryPut(dir, cfg->contentThrdInst->NewPointer(r),
                                       REQUEST_REC_PTR);
    cfg->contentThrdInst->DirectoryPut(dir, cfg->contentThrdInst->String("0"),
                                       REQUEST_HANDLER_TYPE);

//  modoorexx_debug(r->server, "Entering rexx_post_request_handler routine.");

    // create an Array object to hold the program arguments
    RexxArrayObject args = cfg->contentThrdInst->NewArray(1);
    // add the argument to the array. Note that ArrayPut() requires an
    // index that is origin-1, unlike C arrays which are origin-0.
    cfg->contentThrdInst->ArrayPut(args, cfg->contentThrdInst->NewPointer(r), 1);
    // call our program, using the provided arguments.
    RexxObjectPtr result = cfg->contentThrdInst->CallProgram(c->post_read, args);
    cfg->contentThrdInst->ObjectToInt32(result, &rc);
    // if an error occurred, get the decoded exception information
    if (cfg->contentThrdInst->CheckCondition()) {
        RexxCondition condition;
        // retrieve the error information and get it into a decoded form
        RexxDirectoryObject cond = cfg->contentThrdInst->GetConditionInfo();
        cfg->contentThrdInst->DecodeConditionInfo(cond, &condition);
        // display the errors
        oorexxstart_error_processor(r, c->post_read, rc);
        return HTTP_INTERNAL_SERVER_ERROR;
    }

//  modoorexx_debug(r->server, "Exiting rexx_post_request_handler routine.");

    return rc;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Function:    oorexx_register_hooks                                         */
/*                                                                            */
/* Description: This handler is invoked to register all the standard handler  */
/*              hooks.                                                        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

static void oorexx_register_hooks(apr_pool_t *p)
{

    ap_hook_child_init(oorexx_child_init, NULL, NULL, APR_HOOK_MIDDLE);
    ap_hook_translate_name(oorexx_translation_handler, NULL, NULL,
                           APR_HOOK_MIDDLE);
    ap_hook_auth_checker(oorexx_authentication_handler, NULL, NULL,
                         APR_HOOK_MIDDLE);
    ap_hook_check_user_id(oorexx_authorization_handler, NULL, NULL,
                          APR_HOOK_MIDDLE);
    ap_hook_access_checker(oorexx_access_handler, NULL, NULL, APR_HOOK_MIDDLE);
    ap_hook_type_checker(oorexx_mime_type_handler, NULL, NULL, APR_HOOK_MIDDLE);
    ap_hook_fixups(oorexx_fixup_handler, NULL, NULL, APR_HOOK_MIDDLE);
    ap_hook_log_transaction(oorexx_logging_handler, NULL, NULL, APR_HOOK_MIDDLE);
    ap_hook_header_parser(oorexx_header_parser_handler, NULL, NULL,
                          APR_HOOK_MIDDLE);
    ap_hook_post_read_request(oorexx_post_request_handler, NULL, NULL,
                              APR_HOOK_MIDDLE);
    ap_hook_handler(oorexx_handler, NULL, NULL, APR_HOOK_MIDDLE);
    ap_hook_handler(oorexx_rsphandler, NULL, NULL, APR_HOOK_MIDDLE);
};


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* The list of callback routines and data structures that provide the hooks   */
/* into our module from the other parts of the Apache server.                 */
/*                                                                            */
/*----------------------------------------------------------------------------*/

module AP_MODULE_DECLARE_DATA oorexx_module =
{
    STANDARD20_MODULE_STUFF,
    create_oorexx_dir_config,          /* per-directory config creator        */
    NULL,                              /* dir config merger                   */
    NULL,                              /* server config creator               */
    NULL,                              /* server config merger                */
    oorexx_cmds,                       /* command table                       */
    oorexx_register_hooks              /* register hooks                      */
};

