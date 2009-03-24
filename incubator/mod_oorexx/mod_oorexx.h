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

#ifndef MOD_OOREXX_H
#define MOD_OOREXX_H

/* define this to get Win32 support */
#if defined(WIN32)
    #ifndef _WIN32_WINNT
        #define _WIN32_WINNT 0x0400
    #endif
#endif

#ifdef HAVE_CONFIG_H
    #include "config.h"
#endif

#include <oorexxapi.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#if defined(WIN32)
    #include <io.h>
#endif

#include <httpd.h>
#include <http_config.h>
#include <http_core.h>
#include <http_log.h>
#include <http_main.h>
#include <http_request.h>
#include <http_protocol.h>
#include <util_script.h>
#include <ap_compat.h>
#include <apr_strings.h>


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Global Definitions                                                         */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

/* Mod_ooRexx version string */
#define MOD_OOREXX_VERSION "Mod_ooRexx/2.2.0"

/* Debug and error log macros */
#define modoorexx_debug(srvr, msg) ap_log_error(APLOG_MARK, \
    APLOG_NOERRNO | APLOG_DEBUG, 0, srvr, msg)
#define modoorexx_error(srvr, msg) ap_log_error(APLOG_MARK, \
    APLOG_NOERRNO | APLOG_ERR, 0, srvr, msg)

/* Name of the variable in the .local environment which holds the request_rec pointer */
#define REQUEST_REC_PTR "request_rec"
/* Name of the variable in the .local environment which holds the handler type */
#define REQUEST_HANDLER_TYPE "request_handler_type"

/* Struct for holding our Rexx instances for each server child */
typedef struct _ooRexxSrvrConfig {
    RexxInstance      *contentInst;
    RexxThreadContext *contentThrdInst;
} ooRexxSrvrConfig;

/* Our ooRexx package entry */
extern RexxPackageEntry mod_oorexx_package_entry;



/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Data declarations.                                                         */
/*                                                                            */
/*----------------------------------------------------------------------------*/

extern const command_rec oorexx_cmds[];
extern const char *version;
extern const char *compDate;
extern const char *compTime;
extern module MODULE_VAR_EXPORT oorexx_module;

/* Linked list struct for the RexxSetVar directive */
typedef struct _oorexx_var_rec {
    struct _oorexx_var_rec *next;
    char *oorexx_var_name;
    char *oorexx_var_value;
} oorexx_var_rec;

/* Configuration directive structure */
typedef struct _oorexx_config {
    oorexx_var_rec *rexxvars;
    char *translate;
    char *authenticate;
    char *authorize;
    char *access;
    char *mime_type;
    char *fixup;
    char *logging;
    char *header_parser;
    char *post_read;
    char *content;
    char *rspcompiler;                 /* path to the RSP compiler            */
    char *fnametemplate;               /* RSP temp file template              */
} rexx_config;

/* default RSP compiler and file name template */
#define RSP_COMPILER "rspcomp.rex"
#define RSP_FNAMETEMPLATE "./execrsp?????.rex"


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Utility function declarations.                                             */
/*                                                                            */
/*----------------------------------------------------------------------------*/

int SetooRexxVar(char *name, char *value);
void *create_oorexx_dir_config(apr_pool_t *p, char *path);
const char *get_oorexx_err_str(request_rec *r, int rxrc);
void oorexxstart_error_processor(request_rec * r, char *rxprocpath, int rxrc);
char * CreateTempFile(request_rec *r, const char *fntemplate);
int REXXENTRY ooRexx_INI_Exit(RexxExitContext *context, int ExitNumber, int Subfunction,
                              PEXIT ParmBlock);
int REXXENTRY ooRexx_IO_Exit(RexxExitContext *context, int ExitNumber, int Subfunction,
                             PEXIT ParmBlock);
int REXXENTRY ooRexx_IO_Exit_2(RexxExitContext *context, int ExitNumber, int Subfunction,
                               PEXIT ParmBlock);


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Handler and module init function declarations.                             */
/*                                                                            */
/*----------------------------------------------------------------------------*/

int oorexx_post_config_handler(apr_pool_t *p, apr_pool_t *plog,
                               apr_pool_t *ptemp, server_rec *s);
static int oorexx_handler(request_rec *r);
static int oorexx_rsphandler(request_rec *r);


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Rexx exit function declarations                                            */
/*                                                                            */
/*----------------------------------------------------------------------------*/

RexxExitHandler Rexx_INI_Exit;
RexxExitHandler Rexx_IO_Exit;


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Misc Rexx function declarations                                            */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/* The following registers all of the Rexx external functions */
void reg_oorexx_ext_funcs(void);

#ifdef __cplusplus
}
#endif

#endif  /* #ifndef MOD_OOREXX_H */

