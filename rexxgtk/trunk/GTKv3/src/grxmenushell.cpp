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

static void signal_func_0(GtkWidget *window,
                          gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(window), "OORXOBJECT");
    RexxThreadContext *context;

    cblock->instance->AttachThread(&context);
    context->SendMessage0(rxobj, ((cbcb *)data)->signal_name);
    context->DetachThread();
    return;
}

static void signal_func_1(GtkWidget *window,
                          gboolean arg1,
                          gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(window), "OORXOBJECT");
    RexxThreadContext *context;

    cblock->instance->AttachThread(&context);
    RexxObjectPtr arg1type = context->WholeNumberToObject((wholenumber_t)arg1);;
    context->SendMessage1(rxobj, ((cbcb *)data)->signal_name, arg1type);
    context->DetachThread();
    return;
}

static void signal_func_1a(GtkWidget *window,
                           GtkDirectionType arg1,
                           gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(window), "OORXOBJECT");
    RexxThreadContext *context;

    cblock->instance->AttachThread(&context);
    RexxObjectPtr arg1type = context->WholeNumberToObject((wholenumber_t)arg1);;
    context->SendMessage1(rxobj, ((cbcb *)data)->signal_name, arg1type);
    context->DetachThread();
    return;
}

static gboolean signal_func_1b(GtkWidget *window,
                           gint arg1,
                           gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(window), "OORXOBJECT");
    RexxThreadContext *context;
    RexxObjectPtr retc;
    gboolean rc;

    cblock->instance->AttachThread(&context);
    RexxObjectPtr arg1type = context->WholeNumberToObject((wholenumber_t)arg1);;
    retc = context->SendMessage1(rxobj, ((cbcb *)data)->signal_name, arg1type);
    context->ObjectToInt32(retc, &rc);
    context->DetachThread();
    return rc;
}


/*============================================================================*/
/* Public Functions                                                           */
/*============================================================================*/

/**
 * Method:  append
 *
 * Append a widget to a menu.
 *
 * @param child   The child widget.
 *
 * @return        Zero
 **/
RexxMethod2(int,                       // Return type
            GrxMenuShellAppend,        // Object_method name
            RexxObjectPtr, child,      // The child widget
            CSELF, self)               // GTK self
{
    if (!context->IsOfType(child, "GtkWidget")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(1),
                                 context->NewStringFromAsciiz("GtkWidget"));
        return 0;
    }
    GtkWidget *childWidget = (GtkWidget *)context->ObjectToCSelf(child);

    gtk_menu_shell_append(GTK_MENU_SHELL(self), childWidget);

    return 0;
}

/**
 * Method:  prepend
 *
 * Prepend a widget to a menu.
 *
 * @param child   The child widget.
 *
 * @return        Zero
 **/
RexxMethod2(int,                       // Return type
            GrxMenuShellPrepend,       // Object_method name
            RexxObjectPtr, child,      // The child widget
            CSELF, self)               // GTK self
{
    if (!context->IsOfType(child, "GtkWidget")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(1),
                                 context->NewStringFromAsciiz("GtkWidget"));
        return 0;
    }
    GtkWidget *childWidget = (GtkWidget *)context->ObjectToCSelf(child);

    gtk_menu_shell_prepend(GTK_MENU_SHELL(self), childWidget);

    return 0;
}

/**
 * Method:  insert
 *
 * Insert a widget to a menu.
 *
 * @param child   The child widget.
 *
 * @param pos     Position
 *
 * @return        Zero
 **/
RexxMethod3(int,                       // Return type
            GrxMenuShellInsert,        // Object_method name
            RexxObjectPtr, child,      // The child widget
            int, pos,                  // Position
            CSELF, self)               // GTK self
{
    if (!context->IsOfType(child, "GtkWidget")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(1),
                                 context->NewStringFromAsciiz("GtkWidget"));
        return 0;
    }
    GtkWidget *childWidget = (GtkWidget *)context->ObjectToCSelf(child);

    gtk_menu_shell_insert(GTK_MENU_SHELL(self), childWidget, pos);

    return 0;
}

/**
 * Method:  deactivate
 *
 * Deactivate the menu.
 *
 * @return        Zero
 **/
RexxMethod1(int,                       // Return type
            GrxMenuShellDeactivate,    // Object_method name
            CSELF, self)               // GTK self
{
    gtk_menu_shell_deactivate(GTK_MENU_SHELL(self));

    return 0;
}

/**
 * Method:  select_item
 *
 * Select a menu item.
 *
 * @param child   The child menu item
 *
 * @return        Zero
 **/
RexxMethod2(int,                       // Return type
            GrxMenuShellSelectItem,    // Object_method name
            RexxObjectPtr, child,      // The child widget
            CSELF, self)               // GTK self
{
    if (!context->IsOfType(child, "GtkWidget")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(1),
                                 context->NewStringFromAsciiz("GtkWidget"));
        return 0;
    }
    GtkWidget *childWidget = (GtkWidget *)context->ObjectToCSelf(child);

    gtk_menu_shell_select_item(GTK_MENU_SHELL(self), childWidget);

    return 0;
}

/**
 * Method:  select_first
 *
 * Select a menu item first.
 *
 * @param ss      Search sensitive flag
 *
 * @return        Zero
 **/
RexxMethod2(int,                       // Return type
            GrxMenuShellSelectFirst,   // Object_method name
            logical_t, ss,             // The search sensitive flag
            CSELF, self)               // GTK self
{
    gtk_menu_shell_select_first(GTK_MENU_SHELL(self), ss);

    return 0;
}

/**
 * Method:  deselect
 *
 * Deselect the menu.
 *
 * @return        Zero
 **/
RexxMethod1(int,                       // Return type
            GrxMenuShellDeselect,      // Object_method name
            CSELF, self)               // GTK self
{
    gtk_menu_shell_deselect(GTK_MENU_SHELL(self));

    return 0;
}

/**
 * Method:  activate_item
 *
 * Activate a menu item.
 *
 * @param child   The child widget.
 *
 * @param force   Force deactivate flag
 *
 * @return        Zero
 **/
RexxMethod3(int,                       // Return type
            GrxMenuShellActivateItem,  // Object_method name
            RexxObjectPtr, child,      // The child widget
            logical_t, force,          // Force deactivate flag
            CSELF, self)               // GTK self
{
    if (!context->IsOfType(child, "GtkWidget")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(1),
                                 context->NewStringFromAsciiz("GtkWidget"));
        return 0;
    }
    GtkWidget *childWidget = (GtkWidget *)context->ObjectToCSelf(child);

    gtk_menu_shell_activate_item(GTK_MENU_SHELL(self), childWidget, force);

    return 0;
}

/**
 * Method:  cancel
 *
 * Cancel the menu selection.
 *
 * @return        Zero
 **/
RexxMethod1(int,                       // Return type
            GrxMenuShellCancel,        // Object_method name
            CSELF, self)               // GTK self
{
    gtk_menu_shell_cancel(GTK_MENU_SHELL(self));

    return 0;
}

/**
 * Method:  set_take_focus
 *
 * Set or take the focus..
 *
 * @param take    Take focus flag
 *
 * @return        Zero
 **/
RexxMethod2(int,                       // Return type
            GrxMenuShellSetTakeFocus,  // Object_method name
            logical_t, focus,          // The focus flag
            CSELF, self)               // GTK self
{
    gtk_menu_shell_set_take_focus(GTK_MENU_SHELL(self), focus);

    return 0;
}

/**
 * Method:  get_take_focus
 *
 * Get the focus..
 *
 * @return        Focus boolean
 **/
RexxMethod1(logical_t,                 // Return type
            GrxMenuShellGetTakeFocus,  // Object_method name
            CSELF, self)               // GTK self
{
    return gtk_menu_shell_get_take_focus(GTK_MENU_SHELL(self));
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
            GrxMenuShellSignalConnect, // Object_method name
            CSTRING, name,             // Signal name
            SUPER, super,              // The superclass override
            CSELF, self)               // GTK self
{
    cbcb *cblock;

    if (strcmp(name, "activate_current") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_activate_current";
        g_signal_connect(G_OBJECT(self), "activate-current",
                         G_CALLBACK(signal_func_1), cblock);
        return context->True();
    }
    else if (strcmp(name, "cancel") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_cancel";
        g_signal_connect(G_OBJECT(self), "cancel",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "cycle_focus") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_cycle_focus";
        g_signal_connect(G_OBJECT(self), "cycle-focus",
                         G_CALLBACK(signal_func_1a), cblock);
        return context->True();
    }
    else if (strcmp(name, "deactivate") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_deactivate";
        g_signal_connect(G_OBJECT(self), "deactivate",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "move_current") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_move_current";
        g_signal_connect(G_OBJECT(self), "move-current",
                         G_CALLBACK(signal_func_1a), cblock);
        return context->True();
    }
    else if (strcmp(name, "move_selected") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_move_selected";
        g_signal_connect(G_OBJECT(self), "move-selected",
                         G_CALLBACK(signal_func_1), cblock);
        return context->True();
    }
    else if (strcmp(name, "selection_done") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_selection_done";
        g_signal_connect(G_OBJECT(self), "selection-done",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    return context->ForwardMessage(NULLOBJECT, NULL, super, NULLOBJECT);
}

