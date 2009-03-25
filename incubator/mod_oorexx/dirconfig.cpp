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


/**
 * Function:  create_oorexx_dir_config
 *
 * Creates the variables used by the directive processor 
 * functions.
 *
 * @param p       The pool.
 *
 * @param path    The path.
 *
 * @return        Void
 */
void *create_oorexx_dir_config(apr_pool_t *p, char *path)
{
    rexx_config *c;

    /* Allocate memory for the config record */
    c = (rexx_config *)apr_pcalloc(p, sizeof(rexx_config));

    /* Initialize the data fields */
    c->rexxvars = NULL;
    c->translate = NULL;
    c->authenticate = NULL;
    c->authorize = NULL;
    c->access = NULL;
    c->mime_type = NULL;
    c->fixup = NULL;
    c->logging = NULL;
    c->header_parser = NULL;
    c->post_read = NULL;
    c->content = NULL;
    c->rspcompiler = (char *)RSP_COMPILER;
    c->fnametemplate = (char *)RSP_FNAMETEMPLATE;

    /* Successful processing */
    return (void *)c;
}


/**
 * Function:  oorexx_set_var
 *
 * Process a directive which sets the value of a Rexx variable
 * that can be used in any handler.
 *
 * @param parms   The command parameters.
 *
 * @param mconfig The module config ptr.
 *
 * @param varname The Rexx variable name.
 *
 * @param value   The Rexx variable value.
 *
 * @return        Void
 */
static const char *oorexx_set_var(cmd_parms *parms, void *mconfig,
                                  const char *varname, const char *value)
{
    rexx_config *c;
    oorexx_var_rec *er;
    oorexx_var_rec *nr;

    /* Convert pointers */
    c = (rexx_config *) mconfig;

    /* Find the place in the linked list to put our RexxSetVar information    */
    /* and allocate a new struct to hold the directive                        */
    er = c->rexxvars;
    if (er == NULL) {
        nr = (oorexx_var_rec *)apr_pcalloc(parms->pool, sizeof(oorexx_var_rec));
        c->rexxvars = nr;
        }
    else {
        while (er->next != NULL)
           er = er->next;
        nr = (oorexx_var_rec *)apr_pcalloc(parms->pool, sizeof(oorexx_var_rec));
        er->next = nr;
        }

    /* Now that we have a new record we can initialize it */
    nr->next = NULL;
    nr->oorexx_var_name = apr_pstrdup(parms->pool, varname);
    nr->oorexx_var_value = apr_pstrdup(parms->pool, value);

    /* Successful processing */
    return NULL;
}


/**
 * Function:  oorexx_post_read
 *
 * Process a directive which sets the Post Request Handler.
 *
 * @param parms   The command parameters.
 *
 * @param mconfig The module config ptr.
 *
 * @param value   The handler filename.
 *
 * @return        Void
 */
static const char *oorexx_post_read(cmd_parms *parms, void *mconfig,
                                    const char *value)
{
    rexx_config  *c;

    /* Convert pointers */
    c = (rexx_config *)mconfig;

    /* allocate a new struct to hold the directive */
    c->post_read = apr_pstrdup(parms->pool, value);

    /* Successful processing */
    return NULL;
}


/**
 * Function:  oorexx_trans
 *
 * Process a directive which sets the Translation Handler.
 *
 * @param parms   The command parameters.
 *
 * @param mconfig The module config ptr.
 *
 * @param value   The handler filename.
 *
 * @return        Void
 */
static const char *oorexx_trans(cmd_parms *parms, void  *mconfig,
                                const char *value)
{
    rexx_config  *c;

    /* Convert pointers */
    c = (rexx_config *)mconfig;

    /* allocate a new struct to hold the directive */
    c->translate = apr_pstrdup(parms->pool, value);

    /* Successful processing */
    return NULL;
}


/**
 * Function:  oorexx_header_parser
 *
 * Process a directive which sets the Header Parser Handler.
 *
 * @param parms   The command parameters.
 *
 * @param mconfig The module config ptr.
 *
 * @param value   The handler filename.
 *
 * @return        Void
 */
static const char *oorexx_header_parser(cmd_parms *parms, void *mconfig,
                                        const char *value)
{
    rexx_config *c;

    /* Convert pointers */
    c = (rexx_config *)mconfig;

    /* allocate a new struct to hold the directive */
    c->header_parser = apr_pstrdup(parms->pool, value);

    /* Successful processing */
    return NULL;
}


/**
 * Function:  oorexx_access
 *
 * Process a directive which sets the Access Handler.
 *
 * @param parms   The command parameters.
 *
 * @param mconfig The module config ptr.
 *
 * @param value   The handler filename.
 *
 * @return        Void
 */
static const char *oorexx_access(cmd_parms *parms, void *mconfig,
                                 const char *value)
{
    rexx_config *c;

    /* Convert pointers */
    c = (rexx_config *)mconfig;

    /* allocate a new struct to hold the directive */
    c->access = apr_pstrdup(parms->pool, value);

    /* Successful processing */
    return NULL;
}


/**
 * Function:  oorexx_authen
 *
 * Process a directive which sets the Authentication Handler.
 *
 * @param parms   The command parameters.
 *
 * @param mconfig The module config ptr.
 *
 * @param value   The handler filename.
 *
 * @return        Void
 */
static const char *oorexx_authen(cmd_parms *parms, void *mconfig,
                                 const char *value)
{
    rexx_config *c;

    /* Convert pointers */
    c = (rexx_config *)mconfig;

    /* allocate a new struct to hold the directive */
    c->authenticate = apr_pstrdup(parms->pool, value);

    /* Successful processing */
    return NULL;
}


/**
 * Function:  oorexx_authz
 *
 * Process a directive which sets the Authorization Handler.
 *
 * @param parms   The command parameters.
 *
 * @param mconfig The module config ptr.
 *
 * @param value   The handler filename.
 *
 * @return        Void
 */
static const char *oorexx_authz(cmd_parms *parms, void *mconfig,
                                const char *value)
{
    rexx_config *c;

    /* Convert pointers */
    c = (rexx_config *)mconfig;

    /* allocate a new struct to hold the directive */
    c->authorize = apr_pstrdup(parms->pool, value);

    /* Successful processing */
    return NULL;
}


/**
 * Function:  oorexx_type
 *
 * Process a directive which sets the Type Checking Handler.
 *
 * @param parms   The command parameters.
 *
 * @param mconfig The module config ptr.
 *
 * @param value   The handler filename.
 *
 * @return        Void
 */
static const char *oorexx_type(cmd_parms *parms, void *mconfig,
                               const char *value)
{
    rexx_config *c;

    /* Convert pointers */
    c = (rexx_config *)mconfig;

    /* allocate a new struct to hold the directive */
    c->mime_type = apr_pstrdup(parms->pool, value);

    /* Successful processing */
    return NULL;
}


/**
 * Function:  oorexx_fixup
 *
 * Process a directive which sets the Fixup Handler.
 *
 * @param parms   The command parameters.
 *
 * @param mconfig The module config ptr.
 *
 * @param value   The handler filename.
 *
 * @return        Void
 */
static const char *oorexx_fixup(cmd_parms *parms, void *mconfig,
                                const char *value)
{
    rexx_config *c;

    /* Convert pointers */
    c = (rexx_config *)mconfig;

    /* allocate a new struct to hold the directive */
    c->fixup = apr_pstrdup(parms->pool, value);

    /* Successful processing */
    return NULL;
}


/**
 * Function:  oorexx_log
 *
 * Process a directive which sets the Logging Handler.
 *
 * @param parms   The command parameters.
 *
 * @param mconfig The module config ptr.
 *
 * @param value   The handler filename.
 *
 * @return        Void
 */
static const char *oorexx_log(cmd_parms *parms, void *mconfig,
                              const char *value)
{
    rexx_config *c;

    /* Convert pointers */
    c = (rexx_config *) mconfig;

    /* allocate a new struct to hold the directive */
    c->logging = apr_pstrdup (parms->pool, value);

    /* Successful processing */
    return NULL;
}


/**
 * Function:  oorexx_content
 *
 * Process a directive which sets the Content Handler.
 *
 * @param parms   The command parameters.
 *
 * @param mconfig The module config ptr.
 *
 * @param value   The handler filename.
 *
 * @return        Void
 */
static const char *oorexx_content(cmd_parms *parms, void *mconfig,
                                  const char *value)
{
    rexx_config *c;

    /* Convert pointers */
    c = (rexx_config *)mconfig;

    /* allocate a new struct to hold the directive */
    c->content = apr_pstrdup(parms->pool, value);

    /* Successful processing */
    return NULL;
}


/**
 * Function:  oorexx_rsp_compiler
 *
 * Process a directive which sets the RSP Compiler handler.
 *
 * @param parms   The command parameters.
 *
 * @param mconfig The module config ptr.
 *
 * @param value   The handler filename.
 *
 * @return        Void
 */
static const char *oorexx_rsp_compiler(cmd_parms *parms, void *mconfig,
                                       const char *value)
{
    rexx_config *c;

    /* Convert pointers */
    c = (rexx_config *)mconfig;

    /* allocate a new struct to hold the directive */
    c->rspcompiler = apr_pstrdup(parms->pool, value);

    /* Successful processing */
    return NULL;
}


/**
 * Function:  oorexx_fnametemplate
 *
 * Process a directive which sets the SysTempFileName template.
 *
 * @param parms   The command parameters.
 *
 * @param mconfig The module config ptr.
 *
 * @param value   The handler filename.
 *
 * @return        Void
 */
static const char *oorexx_fnametemplate(cmd_parms *parms, void *mconfig,
                                        const char *value)
{
    rexx_config *c;

    /* Convert pointers */
    c = (rexx_config *)mconfig;

    /* allocate a new struct to hold the directive */
    c->fnametemplate = apr_pstrdup(parms->pool, value);

    /* Successful processing */
    return NULL;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* The list of Apache configuration directives available for Mod_Rexx.        */
/*                                                                            */
/*----------------------------------------------------------------------------*/

const command_rec oorexx_cmds[] =
{
    {"RexxSetVar", (cmd_func) oorexx_set_var, NULL, OR_ALL, TAKE2,
     "A Rexx variable name followed by the value."},
    {"RexxPostReadRequestHandler", (cmd_func) oorexx_post_read, NULL, OR_ALL, TAKE1,
     "The name of the Post Read Request Handler."},
    {"RexxTransHandler", (cmd_func) oorexx_trans, NULL, OR_ALL, TAKE1,
     "The name of the Translation Handler."},
    {"RexxHeaderParserHandler", (cmd_func) oorexx_header_parser, NULL, OR_ALL, TAKE1,
     "The name of the Header Parser Handler."},
    {"RexxAccessHandler", (cmd_func) oorexx_access, NULL, OR_ALL, TAKE1,
     "The name of the Access Handler."},
    {"RexxAuthenHandler", (cmd_func) oorexx_authen, NULL, OR_ALL, TAKE1,
     "The name of the Authentication Handler."},
    {"RexxAuthzHandler", (cmd_func) oorexx_authz, NULL, OR_ALL, TAKE1,
     "The name of the Authorization Handler."},
    {"RexxTypeHandler", (cmd_func) oorexx_type, NULL, OR_ALL, TAKE1,
     "The name of the Type Checking Handler."},
    {"RexxFixupHandler", (cmd_func) oorexx_fixup, NULL, OR_ALL, TAKE1,
     "The name of the Fixup Handler."},
    {"RexxLogHandler", (cmd_func) oorexx_log, NULL, OR_ALL, TAKE1,
     "The name of the Logging Handler."},
    {"RexxHandler", (cmd_func) oorexx_content, NULL, OR_ALL, TAKE1,
     "The name of the Content Handler."},
    {"RexxRspCompiler", (cmd_func) oorexx_rsp_compiler, NULL, OR_ALL, TAKE1,
     "The name of the Rexx script which compiles RSP files."},
    {"RexxTempFileNameTemplate", (cmd_func) oorexx_fnametemplate, NULL, OR_ALL, TAKE1,
     "The template to be passed to the SysTempFileName external function."},
    {NULL}
};

