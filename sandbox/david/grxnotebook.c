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
/* Global variables                                                           */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Local Definitions                                                          */
/*----------------------------------------------------------------------------*/


/*============================================================================*/
/* Private Functions                                                          */
/*============================================================================*/

static void signal_func_2(GtkWidget *window,
                          gint arg1,
                          gpointer data)
{
    char buffer[256];
    RXSTRING entry;

    // set up the queue entry data
#ifdef WIN32
    sprintf(buffer, "%p %s %d", window, data, arg1);
#else
    snprintf(buffer, sizeof(buffer), "%p %s %d", window, data, arg1);
#endif
    entry.strptr = buffer;
    entry.strlength = strlen(buffer);

    // insert the signal event here
    RexxAddQueue(GrxGetRexxQueueName(), &entry, RXQUEUE_FIFO);

    return;
}

static void signal_func_2a(GtkWidget *window,
                           gboolean arg1,
                           gpointer data)
{
    char buffer[256];
    RXSTRING entry;

    // set up the queue entry data
#ifdef WIN32
    sprintf(buffer, "%p %s %d", window, data, arg1);
#else
    snprintf(buffer, sizeof(buffer), "%p %s %d", window, data, arg1);
#endif
    entry.strptr = buffer;
    entry.strlength = strlen(buffer);

    // insert the signal event here
    RexxAddQueue(GrxGetRexxQueueName(), &entry, RXQUEUE_FIFO);

    return;
}

static void signal_func_3(GtkWidget *window,
                          GtkWidget * widget,
                          guint arg2,
                          gpointer data)
{
    char buffer[256];
    RXSTRING entry;

    // set up the queue entry data
#ifdef WIN32
    sprintf(buffer, "%p %s %p %u", window, data, widget, arg2);
#else
    snprintf(buffer, sizeof(buffer), "%p %s %p %u", window, data, widget, arg2);
#endif
    entry.strptr = buffer;
    entry.strlength = strlen(buffer);

    // insert the signal event here
    RexxAddQueue(GrxGetRexxQueueName(), &entry, RXQUEUE_FIFO);

    return;
}

static void signal_func_3a(GtkWidget *window,
                           gint arg1,
                           gboolean arg2,
                           gpointer data)
{
    char buffer[256];
    RXSTRING entry;

    // set up the queue entry data
#ifdef WIN32
    sprintf(buffer, "%p %s %d %d", window, data, arg1, arg2);
#else
    snprintf(buffer, sizeof(buffer), "%p %s %d %d", window, data, arg1, arg2);
#endif
    entry.strptr = buffer;
    entry.strlength = strlen(buffer);

    // insert the signal event here
    RexxAddQueue(GrxGetRexxQueueName(), &entry, RXQUEUE_FIFO);

    return;
}

static void signal_func_4(GtkWidget *window,
                          GtkWidget * widget,
                          gint arg2,
                          gint arg3,
                          gpointer data)
{
    char buffer[256];
    RXSTRING entry;

    // set up the queue entry data
#ifdef WIN32
    sprintf(buffer, "%p %s %p %d %d", window, data, widget, arg2, arg3);
#else
    snprintf(buffer, sizeof(buffer), "%p %s %p %d %d", window, data, widget, arg2, arg3);
#endif
    entry.strptr = buffer;
    entry.strlength = strlen(buffer);

    // insert the signal event here
    RexxAddQueue(GrxGetRexxQueueName(), &entry, RXQUEUE_FIFO);

    return;
}


/*============================================================================*/
/* Public Functions                                                           */
/*============================================================================*/


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxNotebookNew                                     */
/* Description: Create a notebook.                                            */
/* Rexx Args:   None                                                          */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxNotebookNew(const char * Name,
                               const size_t Argc, const RXSTRING Argv[],
                               const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;

    /* Check for valid arguments */
    if (GrxCheckArgs(0, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    myWidget = gtk_notebook_new();

    /* Set up the REXX return code */
    sprintf(Retstr->strptr, "%p", myWidget);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxNotebookAppendPage                              */
/* Description: Append a page to the notebook.                                */
/* Rexx Args:   Pointer to the widget                                         */
/*              Container                                                     */
/*              Tab label                                                     */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxNotebookAppendPage(const char * Name,
                                      const size_t Argc, const RXSTRING Argv[],
                                      const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myNotebook;
    GtkWidget *myContainer;
    GtkWidget *myLabel;

    /* Check for valid arguments */
    if (GrxCheckArgs(3, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myNotebook);
    sscanf(Argv[1].strptr, "%p", &myContainer);
    sscanf(Argv[2].strptr, "%p", &myLabel);

    if (GTK_IS_NOTEBOOK(GTK_OBJECT(myNotebook))) {
        gtk_notebook_append_page(GTK_NOTEBOOK(myNotebook), myContainer,
                                 myLabel);
    }

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxNotebookPrependPage                             */
/* Description: Prepend a page to the notebook.                               */
/* Rexx Args:   Pointer to the widget                                         */
/*              Container                                                     */
/*              Tab label                                                     */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxNotebookPrependPage(const char * Name,
                                       const size_t Argc, const RXSTRING Argv[],
                                       const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myNotebook;
    GtkWidget *myContainer;
    GtkWidget *myLabel;

    /* Check for valid arguments */
    if (GrxCheckArgs(3, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myNotebook);
    sscanf(Argv[1].strptr, "%p", &myContainer);
    sscanf(Argv[2].strptr, "%p", &myLabel);

    if (GTK_IS_NOTEBOOK(GTK_OBJECT(myNotebook))) {
        gtk_notebook_prepend_page(GTK_NOTEBOOK(myNotebook), myContainer,
                                  myLabel);
    }

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxNotebookInsertPage                              */
/* Description: Insert a page to the notebook.                                */
/* Rexx Args:   Pointer to the widget                                         */
/*              Container                                                     */
/*              Tab label                                                     */
/*              Position                                                      */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxNotebookInsertPage(const char * Name,
                                      const size_t Argc, const RXSTRING Argv[],
                                      const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myNotebook;
    GtkWidget *myContainer;
    GtkWidget *myLabel;
    gint       pos;

    /* Check for valid arguments */
    if (GrxCheckArgs(4, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myNotebook);
    sscanf(Argv[1].strptr, "%p", &myContainer);
    sscanf(Argv[2].strptr, "%p", &myLabel);
    sscanf(Argv[3].strptr, "%d", &pos);

    if (GTK_IS_NOTEBOOK(GTK_OBJECT(myNotebook))) {
        gtk_notebook_insert_page(GTK_NOTEBOOK(myNotebook), myContainer,
                                 myLabel, pos);
    }

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxNotebookAppendPageMenu                          */
/* Description: Append a page to the notebook with a popup menu.              */
/* Rexx Args:   Pointer to the widget                                         */
/*              Container                                                     */
/*              Tab label                                                     */
/*              Menu label                                                    */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxNotebookAppendPageMenu(const char * Name,
                                          const size_t Argc, const RXSTRING Argv[],
                                          const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myNotebook;
    GtkWidget *myContainer;
    GtkWidget *myLabel;
    GtkWidget *myMenu;

    /* Check for valid arguments */
    if (GrxCheckArgs(4, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myNotebook);
    sscanf(Argv[1].strptr, "%p", &myContainer);
    sscanf(Argv[2].strptr, "%p", &myLabel);
    sscanf(Argv[3].strptr, "%p", &myMenu);

    if (GTK_IS_NOTEBOOK(GTK_OBJECT(myNotebook))) {
        gtk_notebook_append_page_menu(GTK_NOTEBOOK(myNotebook), myContainer,
                                      myLabel, myMenu);
    }

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxNotebookPrependPageMenu                         */
/* Description: Prepend a page to the notebook with a popup menu.             */
/* Rexx Args:   Pointer to the widget                                         */
/*              Container                                                     */
/*              Tab label                                                     */
/*              Menu label                                                    */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxNotebookPrependPageMenu(const char * Name,
                                           const size_t Argc, const RXSTRING Argv[],
                                           const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myNotebook;
    GtkWidget *myContainer;
    GtkWidget *myLabel;
    GtkWidget *myMenu;

    /* Check for valid arguments */
    if (GrxCheckArgs(4, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myNotebook);
    sscanf(Argv[1].strptr, "%p", &myContainer);
    sscanf(Argv[2].strptr, "%p", &myLabel);
    sscanf(Argv[3].strptr, "%p", &myMenu);

    if (GTK_IS_NOTEBOOK(GTK_OBJECT(myNotebook))) {
        gtk_notebook_prepend_page_menu(GTK_NOTEBOOK(myNotebook), myContainer,
                                       myLabel, myMenu);
    }

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxNotebookInsertPageMenu                          */
/* Description: Insert a page to the notebook with a popup menu               */
/* Rexx Args:   Pointer to the widget                                         */
/*              Container                                                     */
/*              Tab label                                                     */
/*              Menu label                                                    */
/*              Position                                                      */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxNotebookInsertPageMenu(const char * Name,
                                          const size_t Argc, const RXSTRING Argv[],
                                          const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myNotebook;
    GtkWidget *myContainer;
    GtkWidget *myLabel;
    GtkWidget *myMenu;
    gint       pos;

    /* Check for valid arguments */
    if (GrxCheckArgs(4, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myNotebook);
    sscanf(Argv[1].strptr, "%p", &myContainer);
    sscanf(Argv[2].strptr, "%p", &myLabel);
    sscanf(Argv[2].strptr, "%p", &myMenu);
    sscanf(Argv[3].strptr, "%d", &pos);

    if (GTK_IS_NOTEBOOK(GTK_OBJECT(myNotebook)) && 
     GTK_IS_WIDGET(GTK_OBJECT(myContainer))) {
        gtk_notebook_insert_page_menu(GTK_NOTEBOOK(myNotebook), myContainer,
                                      myLabel, myMenu, pos);
    }

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxNotebookRemovePage                              */
/* Description: Remove a page from the notebook.                              */
/* Rexx Args:   Pointer to the widget                                         */
/*              Position                                                      */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxNotebookRemovePage(const char * Name,
                                      const size_t Argc, const RXSTRING Argv[],
                                      const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myNotebook;
    gint       pos;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myNotebook);
    sscanf(Argv[1].strptr, "%d", &pos);

    if (GTK_IS_NOTEBOOK(GTK_OBJECT(myNotebook))) {
        gtk_notebook_remove_page(GTK_NOTEBOOK(myNotebook), pos);
    }

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxNotebookSetShowBorder                           */
/* Description: Toggle the display of a border.                               */
/* Rexx Args:   Pointer to the widget                                         */
/*              Boolean                                                       */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxNotebookSetShowBorder(const char * Name,
                                         const size_t Argc, const RXSTRING Argv[],
                                         const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myNotebook;
    gboolean   toggle;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myNotebook);
    sscanf(Argv[1].strptr, "%d", &toggle);

    if (GTK_IS_NOTEBOOK(GTK_OBJECT(myNotebook))) {
        gtk_notebook_set_show_border(GTK_NOTEBOOK(myNotebook), toggle);
    }

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxNotebookSetShowTabs                             */
/* Description: Toggle the display of the tabs.                               */
/* Rexx Args:   Pointer to the widget                                         */
/*              Boolean                                                       */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxNotebookSetShowTabs(const char * Name,
                                       const size_t Argc, const RXSTRING Argv[],
                                       const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myNotebook;
    gboolean   toggle;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myNotebook);
    sscanf(Argv[1].strptr, "%d", &toggle);

    if (GTK_IS_NOTEBOOK(GTK_OBJECT(myNotebook))) {
        gtk_notebook_set_show_tabs(GTK_NOTEBOOK(myNotebook), toggle);
    }

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxNotebookSetTabPos                               */
/* Description: Set the display position of the tabs.                         */
/* Rexx Args:   Pointer to the widget                                         */
/*              Position                                                      */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxNotebookSetTabPos(const char * Name,
                                     const size_t Argc, const RXSTRING Argv[],
                                     const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myNotebook;
    GtkPositionType pos;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myNotebook);
    if (!strcmp(Argv[1].strptr, "GTK_POS_LEFT")) {
        pos = GTK_POS_LEFT;
    }
    else if (!strcmp(Argv[1].strptr, "GTK_POS_RIGHT")) {
        pos = GTK_POS_RIGHT;
    }
    else if (!strcmp(Argv[1].strptr, "GTK_POS_TOP")) {
        pos = GTK_POS_TOP;
    }
    else if (!strcmp(Argv[1].strptr, "GTK_POS_BOTTOM")) {
        pos = GTK_POS_BOTTOM;
    }
    else {
        sscanf(Argv[1].strptr, "%d", &pos);
    }

    if (GTK_IS_NOTEBOOK(GTK_OBJECT(myNotebook))) {
        gtk_notebook_set_tab_pos(GTK_NOTEBOOK(myNotebook), pos);
    }

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxNotebookSetHomogeneousTabs                      */
/* Description: Toggle the display of same-sized tabs.                        */
/* Rexx Args:   Pointer to the widget                                         */
/*              Boolean                                                       */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxNotebookSetHomogeneousTabs(const char * Name,
                                              const size_t Argc, const RXSTRING Argv[],
                                              const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myNotebook;
    gboolean   toggle;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myNotebook);
    sscanf(Argv[1].strptr, "%d", &toggle);

    if (GTK_IS_NOTEBOOK(GTK_OBJECT(myNotebook))) {
        gtk_notebook_set_homogeneous_tabs(GTK_NOTEBOOK(myNotebook), toggle);
    }

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxNotebookSetTabBorder                            */
/* Description: Set the border width of tabs.                                 */
/* Rexx Args:   Pointer to the widget                                         */
/*              Width                                                         */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxNotebookSetTabBorder(const char * Name,
                                        const size_t Argc, const RXSTRING Argv[],
                                        const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myNotebook;
    guint      width;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myNotebook);
    sscanf(Argv[1].strptr, "%d", &width);

    if (GTK_IS_NOTEBOOK(GTK_OBJECT(myNotebook))) {
        gtk_notebook_set_tab_border(GTK_NOTEBOOK(myNotebook), width);
    }

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxNotebookSetScrollable                           */
/* Description: Toggle the ability to scroll the tabs.                        */
/* Rexx Args:   Pointer to the widget                                         */
/*              Boolean                                                       */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxNotebookSetScrollable(const char * Name,
                                         const size_t Argc, const RXSTRING Argv[],
                                         const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myNotebook;
    gboolean   toggle;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myNotebook);
    sscanf(Argv[1].strptr, "%d", &toggle);

    if (GTK_IS_NOTEBOOK(GTK_OBJECT(myNotebook))) {
        gtk_notebook_set_scrollable(GTK_NOTEBOOK(myNotebook), toggle);
    }

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxNotebookPopupEnable                             */
/* Description: Enable popup menus.                                           */
/* Rexx Args:   Pointer to the widget                                         */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxNotebookPopupEnable(const char * Name,
                                       const size_t Argc, const RXSTRING Argv[],
                                       const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myNotebook;

    /* Check for valid arguments */
    if (GrxCheckArgs(1, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myNotebook);

    if (GTK_IS_NOTEBOOK(GTK_OBJECT(myNotebook))) {
        gtk_notebook_popup_enable(GTK_NOTEBOOK(myNotebook));
    }

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxNotebookPopupDisable                            */
/* Description: Enable popup menus.                                           */
/* Rexx Args:   Pointer to the widget                                         */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxNotebookPopupDisable(const char * Name,
                                        const size_t Argc, const RXSTRING Argv[],
                                        const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myNotebook;

    /* Check for valid arguments */
    if (GrxCheckArgs(1, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myNotebook);

    if (GTK_IS_NOTEBOOK(GTK_OBJECT(myNotebook))) {
        gtk_notebook_popup_disable(GTK_NOTEBOOK(myNotebook));
    }

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxNotebookGetCurrentPage                          */
/* Description: Get the current page number                                   */
/* Rexx Args:   Pointer to the widget                                         */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxNotebookGetCurrentPage(const char * Name,
                                          const size_t Argc, const RXSTRING Argv[],
                                          const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myNotebook;
    gint page;

    /* Check for valid arguments */
    if (GrxCheckArgs(1, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myNotebook);

    if (GTK_IS_NOTEBOOK(GTK_OBJECT(myNotebook))) {
        page = gtk_notebook_get_current_page(GTK_NOTEBOOK(myNotebook));
    }

    /* Set up the REXX return code */
    sprintf(Retstr->strptr, "%d", page);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxNotebookSetCurrentPage                          */
/* Description: Set the current page number                                   */
/* Rexx Args:   Pointer to the widget                                         */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxNotebookSetCurrentPage(const char * Name,
                                          const size_t Argc, const RXSTRING Argv[],
                                          const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myNotebook;
    gint page;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myNotebook);
    sscanf(Argv[1].strptr, "%d", &page);

    if (GTK_IS_NOTEBOOK(GTK_OBJECT(myNotebook))) {
        gtk_notebook_set_current_page(GTK_NOTEBOOK(myNotebook), page);
    }

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxNotebookConnectSignal                           */
/* Description: Connect a signal function to the Widget                       */
/* Rexx Args:   Pointer to the widget                                         */
/*              Signal name                                                   */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxNotebookConnectSignal(const char * Name,
                                         const size_t Argc, const RXSTRING Argv[],
                                         const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);

    if (GTK_IS_WIDGET(GTK_OBJECT(myWidget))) {
        if (strcmp(Argv[1].strptr, "change_current_page") == 0) {
            g_signal_connect(G_OBJECT(myWidget), "change-current-page",
                             G_CALLBACK(signal_func_2), "signal_change_current_page");
        }
        else if (strcmp(Argv[1].strptr, "create_window") == 0) {
            g_signal_connect(G_OBJECT(myWidget), "create-window",
                             G_CALLBACK(signal_func_4), "signal_create_window");
        }
        else if (strcmp(Argv[1].strptr, "focus_tab") == 0) {
            g_signal_connect(G_OBJECT(myWidget), "focus-tab",
                             G_CALLBACK(signal_func_2), "signal_focus_tab");
        }
        else if (strcmp(Argv[1].strptr, "move_focus_out") == 0) {
            g_signal_connect(G_OBJECT(myWidget), "move-focus-out",
                             G_CALLBACK(signal_func_2), "signal_move_focus_out");
        }
        else if (strcmp(Argv[1].strptr, "page_added") == 0) {
            g_signal_connect(G_OBJECT(myWidget), "page-added",
                             G_CALLBACK(signal_func_3), "signal_page_added");
        }
        else if (strcmp(Argv[1].strptr, "page_removed") == 0) {
            g_signal_connect(G_OBJECT(myWidget), "page-removed",
                             G_CALLBACK(signal_func_3), "signal_page_removed");
        }
        else if (strcmp(Argv[1].strptr, "page_reordered") == 0) {
            g_signal_connect(G_OBJECT(myWidget), "page-reordered",
                             G_CALLBACK(signal_func_3), "signal_page_reordered");
        }
        else if (strcmp(Argv[1].strptr, "reorder_tab") == 0) {
            g_signal_connect(G_OBJECT(myWidget), "reorder-tab",
                             G_CALLBACK(signal_func_3a), "signal_reorder_tab");
        }
        else if (strcmp(Argv[1].strptr, "select_page") == 0) {
            g_signal_connect(G_OBJECT(myWidget), "select-page",
                             G_CALLBACK(signal_func_2a), "signal_select_page");
        }
        else if (strcmp(Argv[1].strptr, "switch_page") == 0) {
            g_signal_connect(G_OBJECT(myWidget), "switch-page",
                             G_CALLBACK(signal_func_3), "signal_switch_page");
        }
        else {
            return RXFUNC_BADCALL;
        }
    }
    else {
        return RXFUNC_BADCALL;
    }

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}

