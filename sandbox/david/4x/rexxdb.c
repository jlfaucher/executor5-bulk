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


/*----------------------------------------------------------------------------*/
/* Definitions                                                                */
/*----------------------------------------------------------------------------*/

#include "rexxgtk.h"


/*----------------------------------------------------------------------------*/
/* Global variables                                                           */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Local variables and definitions                                            */
/*----------------------------------------------------------------------------*/

static GList *rxgtkdb = NULL;

typedef struct _dbentry {
    RexxObjectPtr object;
    GtkWidget     *widget;
} dbentry;


/*----------------------------------------------------------------------------*/
/* Functions                                                                  */
/*----------------------------------------------------------------------------*/

static int custom_compare(
    gconstpointer a,
    gconstpointer b) {

    dbentry *aa = a;
    dbentry *bb = b;
    if (aa->widget != bb->widget) {
        return -1;
    }
    return 0;
}


int GrxDBAdd(
    const RexxObjectPtr rxobj,       // the object to be added
    const GtkWidget *widget) {       // the widget pointer

    dbentry *newentry = (dbentry *)malloc(sizeof(dbentry));
    newentry->object = rxobj;
    newentry->widget = widget;
    rxgtkdb = g_list_prepend(rxgtkdb, newentry);
}


int GrxDBRemove(
    const GtkWidget *widget) {       // the widget pointer

    dbentry searchentry;
    searchentry.widget = widget;
    dbentry *entry = g_list_find_custom(rxgtkdb, &searchentry, custom_compare);
    if (dbentry != NULL) {
        rxgtkdb = g_list_remove(rxgtkdb, dbentry);
        free(dbentry);
        return 0;
    }
    return 1;
}


RexxObjectPtr GrxDBFindObject(
    const GtkWidget *widget) {       // the widget pointer

    dbentry searchentry;
    searchentry.widget = widget;
    dbentry *entry = g_list_find_custom(rxgtkdb, &searchentry, custom_compare);
    if (dbentry != NULL) {
        return dbentry->rxobj;
    }
    return NULL;
}

