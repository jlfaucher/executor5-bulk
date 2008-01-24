/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2007-2008 Rexx Language Association. All rights reserved.    */
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


/* include the linux headers and our local stuff */
#include "rexxgtk.h"


/*----------------------------------------------------------------------------*/
/* Private variables                                                          */
/*----------------------------------------------------------------------------*/

typedef struct _constants {
    const gchar * name;
    const gint value;
} CONSTANTS;

CONSTANTS constResponse[] = {
    "GTK_RESPONSE_NONE"  , GTK_RESPONSE_NONE,
    "GTK_RESPONSE_REJECT", GTK_RESPONSE_REJECT,
    "GTK_RESPONSE_ACCEPT", GTK_RESPONSE_ACCEPT,
    "GTK_RESPONSE_OK",     GTK_RESPONSE_OK,
    "GTK_RESPONSE_CANCEL", GTK_RESPONSE_CANCEL,
    "GTK_RESPONSE_CLOSE",  GTK_RESPONSE_CLOSE,
    "GTK_RESPONSE_YES",    GTK_RESPONSE_YES,
    "GTK_RESPONSE_NO",     GTK_RESPONSE_NO,
    "GTK_RESPONSE_APPLY",  GTK_RESPONSE_APPLY,
    "GTK_RESPONSE_HELP",   GTK_RESPONSE_HELP,
    NULL, 0,
};







/*----------------------------------------------------------------------------*/
/* Local Definitions                                                          */
/*----------------------------------------------------------------------------*/


/*============================================================================*/
/* Private Functions                                                          */
/*============================================================================*/


/*============================================================================*/
/* Public Functions                                                           */
/*============================================================================*/


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxConstantString2Value                            */
/* Description: Return the numeric value for a constant                       */
/* Rexx Args:   Constant group string                                         */
/*              Constant string                                               */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxConstantString2Value(const char * Name,
                                    const size_t Argc, const RXSTRING Argv[],
                                    const char * Queuename, PRXSTRING Retstr)
{
    CONSTANTS *constants;
    gint value = 0, i = 0;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    // Get the correct structure address
    if (strcmp(Argv[0].strptr, "GTK_RESPONSE_TYPE") == 0)
        constants = constResponse;
//    else if (strcmp(Argv[0].strptr, "GTK_JUSTIFY_RIGHT") == 0)
//        jtype = GTK_JUSTIFY_RIGHT;
    else
        return RXFUNC_BADCALL;

    // Get the value
    while (constants[i].name != NULL) {
        if (strcmp(constants[i].name, Argv[1].strptr) == 0) {
            value = constants[i].value;
            break;
        }
        i++;
    }

    /* Set up the Rexx return code */
    g_snprintf(Retstr->strptr, RXAUTOBUFLEN, "%d", value);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxValue2ConstantString                            */
/* Description: Return constant string from the numeric value                 */
/* Rexx Args:   Constant group string                                         */
/*              Numeric value                                                 */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxValue2ConstantString(const char * Name,
                                    const size_t Argc, const RXSTRING Argv[],
                                    const char * Queuename, PRXSTRING Retstr)
{
    CONSTANTS * constants;
    gint value, i = 0;
    const gchar *str = "\0";

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    // Get the correct structure address
    if (strcmp(Argv[0].strptr, "GTK_RESPONSE_TYPE") == 0)
        constants = constResponse;
//    else if (strcmp(Argv[0].strptr, "GTK_JUSTIFY_RIGHT") == 0)
//        jtype = GTK_JUSTIFY_RIGHT;
    else
        return RXFUNC_BADCALL;
    sscanf(Argv[1].strptr, "%d", &value);

    // Get the value
    while (constants[i].name != NULL) {
        if (constants[i].value == value) {
            str = constants[i].name;
            break;
        }
        i++;
    }

    /* Set up the Rexx return code */
    g_snprintf(Retstr->strptr, RXAUTOBUFLEN, "%s", str);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}

