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

static void signal_func_1(GtkWidget *window,
                          gint arg1,
                          gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(window), "OORXOBJECT");
    RexxThreadContext *context;

    cblock->instance->AttachThread(&context);
    RexxObjectPtr arg1type = context->NumberToObject((wholenumber_t)arg1);;
    context->SendMessage1(rxobj, ((cbcb *)data)->signal_name, arg1type);
    context->DetachThread();
    return;
}

static void signal_func_1a(GtkWidget *window,
                           gboolean arg1,
                           gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(window), "OORXOBJECT");
    RexxThreadContext *context;

    cblock->instance->AttachThread(&context);
    RexxObjectPtr arg1type = context->NumberToObject((wholenumber_t)arg1);;
    context->SendMessage1(rxobj, ((cbcb *)data)->signal_name, arg1type);
    context->DetachThread();
    return;
}

static void signal_func_2(GtkWidget *window,
                          GtkWidget * widget,
                          guint arg2,
                          gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(window), "OORXOBJECT");
    RexxObjectPtr rxwidget = (RexxObjectPtr)g_object_get_data(G_OBJECT(widget), "OORXOBJECT");
    RexxThreadContext *context;

    cblock->instance->AttachThread(&context);
    RexxObjectPtr arg2type = context->NumberToObject((wholenumber_t)arg2);;
    context->SendMessage2(rxobj, ((cbcb *)data)->signal_name, rxwidget, arg2type);
    context->DetachThread();
    return;
}

static void signal_func_2a(GtkWidget *window,
                           gint arg1,
                           gboolean arg2,
                           gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(window), "OORXOBJECT");
    RexxThreadContext *context;

    cblock->instance->AttachThread(&context);
    RexxObjectPtr arg1type = context->NumberToObject((wholenumber_t)arg1);;
    RexxObjectPtr arg2type = context->NumberToObject((wholenumber_t)arg2);;
    context->SendMessage2(rxobj, ((cbcb *)data)->signal_name, arg1type, arg2type);
    context->DetachThread();
    return;
}

static void signal_func_3(GtkWidget *window,
                          GtkWidget * widget,
                          gint arg2,
                          gint arg3,
                          gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(window), "OORXOBJECT");
    RexxThreadContext *context;
    RexxArrayObject arr;

    cblock->instance->AttachThread(&context);
    arr = context->NewArray(1);
    RexxObjectPtr rxwidget = (RexxObjectPtr)g_object_get_data(G_OBJECT(widget), "OORXOBJECT");
    context->ArrayPut(arr, rxwidget, 1);
    RexxObjectPtr rxarg2 = context->NumberToObject((wholenumber_t)arg2);;
    context->ArrayPut(arr, rxarg2, 2);
    RexxObjectPtr rxarg3 = context->NumberToObject((wholenumber_t)arg3);;
    context->ArrayPut(arr, rxarg3, 3);
    context->SendMessage(rxobj, ((cbcb *)data)->signal_name, arr);
    context->DetachThread();
    return;
}


/*============================================================================*/
/* Public Functions                                                           */
/*============================================================================*/

/**
 * Method:  init
 *
 * Create a notebook.
 *
 * @return        Zero.
 **/
RexxMethod1(int,                       // Return type
            GrxNotebookNew,            // Object_method name
            OSELF, self)               // Self
{
    GtkWidget *myWidget;

    myWidget = gtk_notebook_new();
    context->SendMessage1(self, "POINTER=", context->NewPointer(myWidget));
    g_object_set_data(G_OBJECT(myWidget), "OORXOBJECT", self);

    return 0;
}

/**
 * Method:  append_page
 *
 * Append a page to the notebook.
 *
 * @param page    The container page to append
 *
 * @param label   The notebook tab label
 *
 * @return        Zero.
 **/
RexxMethod3(int,                       // Return type
            GrxNotebookAppendPage,     // Object_method name
            RexxObjectPtr, rxcontainer, // The container
            RexxObjectPtr, rxlabel,    // The label
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkNotebook *myWidget = (GtkNotebook *)context->PointerValue(rxptr);
    RexxPointerObject containerptr = (RexxPointerObject)context->SendMessage0(rxcontainer, "POINTER");
    GtkWidget *myContainer = (GtkWidget *)context->PointerValue(containerptr);
    RexxPointerObject labelptr = (RexxPointerObject)context->SendMessage0(rxlabel, "POINTER");
    GtkWidget *myLabel = (GtkWidget *)context->PointerValue(labelptr);

    gtk_notebook_append_page(myWidget, myContainer, myLabel);

    return 0;
}

/**
 * Method:  prepend_page
 *
 * Append a page to the notebook.
 *
 * @param page    The container page to prepend
 *
 * @param label   The notebook tab label
 *
 * @return        Zero.
 **/
RexxMethod3(int,                       // Return type
            GrxNotebookPrependPage,    // Object_method name
            RexxObjectPtr, rxcontainer, // The container
            RexxObjectPtr, rxlabel,    // The label
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkNotebook *myWidget = (GtkNotebook *)context->PointerValue(rxptr);
    RexxPointerObject containerptr = (RexxPointerObject)context->SendMessage0(rxcontainer, "POINTER");
    GtkWidget *myContainer = (GtkWidget *)context->PointerValue(containerptr);
    RexxPointerObject labelptr = (RexxPointerObject)context->SendMessage0(rxlabel, "POINTER");
    GtkWidget *myLabel = (GtkWidget *)context->PointerValue(labelptr);

    gtk_notebook_prepend_page(myWidget, myContainer, myLabel);

    return 0;
}

/**
 * Method:  insert_page
 *
 * Insert a page to the notebook.
 *
 * @param page    The container page to insert
 *
 * @param label   The notebook tab label
 *
 * @param pos     The position of the page
 *
 * @return        Zero.
 **/
RexxMethod4(int,                       // Return type
            GrxNotebookInsertPage,     // Object_method name
            RexxObjectPtr, rxcontainer, // The container
            RexxObjectPtr, rxlabel,    // The label
            int, pos,                  // The position
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkNotebook *myWidget = (GtkNotebook *)context->PointerValue(rxptr);
    RexxPointerObject containerptr = (RexxPointerObject)context->SendMessage0(rxcontainer, "POINTER");
    GtkWidget *myContainer = (GtkWidget *)context->PointerValue(containerptr);
    RexxPointerObject labelptr = (RexxPointerObject)context->SendMessage0(rxlabel, "POINTER");
    GtkWidget *myLabel = (GtkWidget *)context->PointerValue(labelptr);

    gtk_notebook_insert_page(myWidget, myContainer, myLabel, pos);

    return 0;
}

/**
 * Method:  append_page_menu
 *
 * Append a page with a popup menu to the notebook.
 *
 * @param page    The container page to insert
 *
 * @param label   The notebook tab label
 *
 * @param menu    The menu
 *
 * @return        Zero.
 **/
RexxMethod4(int,                       // Return type
            GrxNotebookAppendPageMenu, // Object_method name
            RexxObjectPtr, rxcontainer, // The container
            RexxObjectPtr, rxlabel,    // The label
            RexxObjectPtr, rxmenu,     // The menu
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkNotebook *myWidget = (GtkNotebook *)context->PointerValue(rxptr);
    RexxPointerObject containerptr = (RexxPointerObject)context->SendMessage0(rxcontainer, "POINTER");
    GtkWidget *myContainer = (GtkWidget *)context->PointerValue(containerptr);
    RexxPointerObject labelptr = (RexxPointerObject)context->SendMessage0(rxlabel, "POINTER");
    GtkWidget *myLabel = (GtkWidget *)context->PointerValue(labelptr);
    RexxPointerObject menuptr = (RexxPointerObject)context->SendMessage0(rxmenu, "POINTER");
    GtkWidget *myMenu = (GtkWidget *)context->PointerValue(menuptr);

    gtk_notebook_append_page_menu(myWidget, myContainer, myLabel, myMenu);

    return 0;
}

/**
 * Method:  prepend_page_menu
 *
 * Prepend a page with a popup menu to the notebook.
 *
 * @param page    The container page to insert
 *
 * @param label   The notebook tab label
 *
 * @param menu    The menu
 *
 * @return        Zero.
 **/
RexxMethod4(int,                       // Return type
            GrxNotebookPrependPageMenu, // Object_method name
            RexxObjectPtr, rxcontainer, // The container
            RexxObjectPtr, rxlabel,    // The label
            RexxObjectPtr, rxmenu,     // The menu
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkNotebook *myWidget = (GtkNotebook *)context->PointerValue(rxptr);
    RexxPointerObject containerptr = (RexxPointerObject)context->SendMessage0(rxcontainer, "POINTER");
    GtkWidget *myContainer = (GtkWidget *)context->PointerValue(containerptr);
    RexxPointerObject labelptr = (RexxPointerObject)context->SendMessage0(rxlabel, "POINTER");
    GtkWidget *myLabel = (GtkWidget *)context->PointerValue(labelptr);
    RexxPointerObject menuptr = (RexxPointerObject)context->SendMessage0(rxmenu, "POINTER");
    GtkWidget *myMenu = (GtkWidget *)context->PointerValue(menuptr);

    gtk_notebook_prepend_page_menu(myWidget, myContainer, myLabel, myMenu);

    return 0;
}

/**
 * Method:  insert_page_menu
 *
 * Insert a page with a popup menu to the notebook.
 *
 * @param page    The container page to insert
 *
 * @param label   The notebook tab label
 *
 * @param menu    The menu
 *
 * @param pos     The position
 *
 * @return        Zero.
 **/
RexxMethod5(int,                       // Return type
            GrxNotebookInsertPageMenu, // Object_method name
            RexxObjectPtr, rxcontainer, // The container
            RexxObjectPtr, rxlabel,    // The label
            RexxObjectPtr, rxmenu,     // The menu
            int, pos,                  // The position
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkNotebook *myWidget = (GtkNotebook *)context->PointerValue(rxptr);
    RexxPointerObject containerptr = (RexxPointerObject)context->SendMessage0(rxcontainer, "POINTER");
    GtkWidget *myContainer = (GtkWidget *)context->PointerValue(containerptr);
    RexxPointerObject labelptr = (RexxPointerObject)context->SendMessage0(rxlabel, "POINTER");
    GtkWidget *myLabel = (GtkWidget *)context->PointerValue(labelptr);
    RexxPointerObject menuptr = (RexxPointerObject)context->SendMessage0(rxmenu, "POINTER");
    GtkWidget *myMenu = (GtkWidget *)context->PointerValue(menuptr);

    gtk_notebook_insert_page_menu(myWidget, myContainer, myLabel, myMenu, pos);

    return 0;
}

/**
 * Method:  remove_page
 *
 * Remove a page from the notebook.
 *
 * @param pos     The position
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxNotebookRemovePage,     // Object_method name
            int, pos,                  // The position
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkNotebook *myWidget = (GtkNotebook *)context->PointerValue(rxptr);

    gtk_notebook_remove_page(myWidget, pos);

    return 0;
}

/**
 * Method:  set_show_border
 *
 * Toggle the display of the border.
 *
 * @param toggle  The border display boolean
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxNotebookSetShowBorder,  // Object_method name
            logical_t, toggle,         // The border display boolean
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkNotebook *myWidget = (GtkNotebook *)context->PointerValue(rxptr);

    gtk_notebook_set_show_border(myWidget, toggle);

    return 0;
}

/**
 * Method:  set_show_tabs
 *
 * Toggle the display of the tabs.
 *
 * @param toggle  The tab display boolean
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxNotebookSetShowTabs,    // Object_method name
            logical_t, toggle,         // The tabs display boolean
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkNotebook *myWidget = (GtkNotebook *)context->PointerValue(rxptr);

    gtk_notebook_set_show_tabs(myWidget, toggle);

    return 0;
}

/**
 * Method:  set_tab_pos
 *
 * Toggle the display position of the tabs.
 *
 * @param pos     The tab display position
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxNotebookSetTabPos,      // Object_method name
            int, pos,                  // The tab position
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkNotebook *myWidget = (GtkNotebook *)context->PointerValue(rxptr);

    gtk_notebook_set_tab_pos(myWidget, (GtkPositionType)pos);

    return 0;
}

/**
 * Method:  set_homegeneous_tabs
 *
 * Toggle the display position of homogemeous tabs.
 *
 * @param toggle  The homogeneous boolean
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxNotebookSetHomogeneousTabs, // Object_method name
            logical_t, toggle,         // The homogeneous boolean
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkNotebook *myWidget = (GtkNotebook *)context->PointerValue(rxptr);

    gtk_notebook_set_homogeneous_tabs(myWidget, toggle);

    return 0;
}

/**
 * Method:  set_tab_border
 *
 * Set the width of tab borders.
 *
 * @param widthe  The tab border width
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxNotebookSetTabBorder,   // Object_method name
            int, width,                // The tab border width
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkNotebook *myWidget = (GtkNotebook *)context->PointerValue(rxptr);

    gtk_notebook_set_tab_border(myWidget, width);

    return 0;
}

/**
 * Method:  set_scrollable
 *
 * Set tabs as scrollable.
 *
 * @param toggle  The scrollable boolean
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxNotebookSetScrollable,  // Object_method name
            logical_t, toggle,         // The scrollable boolean
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkNotebook *myWidget = (GtkNotebook *)context->PointerValue(rxptr);

    gtk_notebook_set_scrollable(myWidget, toggle);

    return 0;
}

/**
 * Method:  popup_enable
 *
 * Enable popups.
 *
 * @return        Zero.
 **/
RexxMethod1(int,                       // Return type
            GrxNotebookPopupEnable,    // Object_method name
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkNotebook *myWidget = (GtkNotebook *)context->PointerValue(rxptr);

    gtk_notebook_popup_enable(myWidget);

    return 0;
}

/**
 * Method:  popup_disable
 *
 * Disable popups.
 *
 * @return        Zero.
 **/
RexxMethod1(int,                       // Return type
            GrxNotebookPopupDisable,   // Object_method name
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkNotebook *myWidget = (GtkNotebook *)context->PointerValue(rxptr);

    gtk_notebook_popup_disable(myWidget);

    return 0;
}

/**
 * Method:  get_current_page
 *
 * Disable popups.
 *
 * @return        Page number
 **/
RexxMethod1(int,                       // Return type
            GrxNotebookGetCurrentPage, // Object_method name
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkNotebook *myWidget = (GtkNotebook *)context->PointerValue(rxptr);

    return gtk_notebook_get_current_page(myWidget);
}

/**
 * Method:  set_current_page
 *
 * Set the current page.
 *
 * @param page    The page number
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxNotebookSetCurrentPage, // Object_method name
            int, page,                 // The page number
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkNotebook *myWidget = (GtkNotebook *)context->PointerValue(rxptr);

    gtk_notebook_set_current_page(myWidget, page);

    return 0;
}

/**
 * Method:  signal_connect
 *
 * Connect a signal to an ooRexx method.
 *
 * @param name    The signal name
 *
 * @return        Zero
 **/
RexxMethod3(RexxObjectPtr,             // Return type
            GrxNotebookSignalConnect,  // Object_method name
            CSTRING, name,             // Signal name
            ARGLIST, args,             // The whole argument list as an array
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);
    cbcb *cblock;

    if (strcmp(name, "change_current_page") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_change_current_page";
        g_signal_connect(G_OBJECT(myWidget), "change-current-page",
                         G_CALLBACK(signal_func_1), cblock);
        return context->True();
    }
    else if (strcmp(name, "create_window") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_create_window";
        g_signal_connect(G_OBJECT(myWidget), "create-window",
                         G_CALLBACK(signal_func_3), cblock);
        return context->True();
    }
    else if (strcmp(name, "focus_tab") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_focus_tab";
        g_signal_connect(G_OBJECT(myWidget), "focus-tab",
                         G_CALLBACK(signal_func_1), cblock);
        return context->True();
    }
    else if (strcmp(name, "move_focus_out") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_move_focus_out";
        g_signal_connect(G_OBJECT(myWidget), "move-focus-out",
                         G_CALLBACK(signal_func_1), cblock);
        return context->True();
    }
    else if (strcmp(name, "page_added") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_page_added";
        g_signal_connect(G_OBJECT(myWidget), "page-added",
                         G_CALLBACK(signal_func_2), cblock);
        return context->True();
    }
    else if (strcmp(name, "page_removed") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_page_removed";
        g_signal_connect(G_OBJECT(myWidget), "page-removed",
                         G_CALLBACK(signal_func_2), cblock);
        return context->True();
    }
    else if (strcmp(name, "page_reordered") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_page_reordered";
        g_signal_connect(G_OBJECT(myWidget), "page-reordered",
                         G_CALLBACK(signal_func_2), cblock);
        return context->True();
    }
    else if (strcmp(name, "reorder_tab") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_reorder_tab";
        g_signal_connect(G_OBJECT(myWidget), "reorder-tab",
                         G_CALLBACK(signal_func_2a), cblock);
        return context->True();
    }
    else if (strcmp(name, "select_page") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_select_page";
        g_signal_connect(G_OBJECT(myWidget), "select-page",
                         G_CALLBACK(signal_func_1a), cblock);
        return context->True();
    }
    else if (strcmp(name, "switch_page") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_switch_page";
        g_signal_connect(G_OBJECT(myWidget), "switch-page",
                         G_CALLBACK(signal_func_2), cblock);
        return context->True();
    }
    return context->SendSuperMessage("signal_connect", args);
}

