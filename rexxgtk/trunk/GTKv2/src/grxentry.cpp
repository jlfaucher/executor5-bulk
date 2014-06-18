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
                          gchar * arg1,
                          gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(window), "OORXOBJECT");
    RexxThreadContext *context;

    cblock->instance->AttachThread(&context);
    RexxObjectPtr rxarg1 = context->NewStringFromAsciiz(arg1);;
    context->SendMessage1(rxobj, ((cbcb *)data)->signal_name, rxarg1);
    context->DetachThread();
    return;
}

static void signal_func_1a(GtkWidget *window,
                           GtkMenu * menu,
                           gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(window), "OORXOBJECT");
    RexxThreadContext *context;

    cblock->instance->AttachThread(&context);
    RexxObjectPtr rxwidget = (RexxObjectPtr)g_object_get_data(G_OBJECT(menu), "OORXOBJECT");
    context->SendMessage1(rxobj, ((cbcb *)data)->signal_name, rxwidget);
    context->DetachThread();
    return;
}

static void signal_func_1b(GtkWidget *window,
                           GtkScrollType arg1,
                           gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(window), "OORXOBJECT");
    RexxThreadContext *context;

    cblock->instance->AttachThread(&context);
    RexxObjectPtr scrolltype = context->WholeNumberToObject((wholenumber_t)arg1);;
    context->SendMessage1(rxobj, ((cbcb *)data)->signal_name, scrolltype);
    context->DetachThread();
    return;
}

static void signal_func_2(GtkWidget *window,
                          GtkDeleteType type,
                          gint arg2,
                          gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(window), "OORXOBJECT");
    RexxThreadContext *context;

    cblock->instance->AttachThread(&context);
    RexxObjectPtr rxtype = context->WholeNumberToObject((wholenumber_t)type);;
    RexxObjectPtr rxarg2 = context->WholeNumberToObject((wholenumber_t)arg2);;
    context->SendMessage2(rxobj, ((cbcb *)data)->signal_name, rxtype, rxarg2);
    context->DetachThread();
    return;
}

static void signal_func_3(GtkWidget *window,
                          GtkMovementStep step,
                          gint arg2,
                          gboolean arg3,
                          gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(window), "OORXOBJECT");
    RexxThreadContext *context;
    RexxArrayObject arr = context->NewArray(1);

    cblock->instance->AttachThread(&context);
    RexxObjectPtr rxstep = context->WholeNumberToObject((wholenumber_t)step);;
    context->ArrayPut(arr, rxstep, 1);
    RexxObjectPtr rxarg2 = context->WholeNumberToObject((wholenumber_t)arg2);;
    context->ArrayPut(arr, rxarg2, 2);
    RexxObjectPtr rxarg3 = context->WholeNumberToObject((wholenumber_t)arg3);;
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
 * Create an entry box.
 *
 * @return        Zero.
 **/
RexxMethod1(int,                       // Return type
            GrxEntryNew,               // Object_method name
            OSELF, self)               // Self
{
    GtkWidget *myWidget = gtk_entry_new();

    // Save ourself
    context->SendMessage1(self, "set_cself", context->NewPointer(myWidget));
    g_object_set_data(G_OBJECT(myWidget), "OORXOBJECT", self);

    return 0;
}

/**
 * Method:  set/get_max_length
 *
 * Set/get the maximum length of text in the entry box.
 *
 * @param maxlen  The max length.
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxEntryGetSetMaxLength,   // Object_method name
            OPTIONAL_int, maxlen,      // Max text length
            CSELF, self)               // GTK self
{
    if (argumentExists(1)) {
        gtk_entry_set_max_length(GTK_ENTRY(self), maxlen);
    }
    else {
        return gtk_entry_get_max_length(GTK_ENTRY(self));
    }

    return 0;
}

/**
 * Method:  set/get_text
 *
 * Set/get the text in the entry box.
 *
 * @param text    The text
 *
 * @return        Zero.
 **/
RexxMethod2(RexxObjectPtr,             // Return type
            GrxEntryGetSetText,        // Object_method name
            OPTIONAL_CSTRING, text,    // The text to set
            CSELF, self)               // GTK self
{
    RexxObjectPtr retc;

    if (argumentExists(1)) {
        gtk_entry_set_text(GTK_ENTRY(self), text);
        retc = (RexxObjectPtr) context->Nil();
    }
    else{
        retc = (RexxObjectPtr)context->NewStringFromAsciiz(gtk_entry_get_text(GTK_ENTRY(self)));
    }

    return retc;
}

/**
 * Method:  set_width_chars
 *
 * Set the width (in characters) of the entry box.
 *
 * @param width   The width
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxEntrySetWidthChars,     // Object_method name
            int, width,                // The width
            CSELF, self)               // GTK self
{
    gtk_entry_set_width_chars(GTK_ENTRY(self), width);

    return 0;
}

/**
 * Method:  set/get_visibility
 *
 * Set/get the visibility of the text in the entry box.
 *
 * @param flag    The visibility boolean
 *
 * @return        Zero.
 **/
RexxMethod2(logical_t,                 // Return type
            GrxEntryGetSetVisibility,  // Object_method name
            OPTIONAL_logical_t, flag,  // The flag
            CSELF, self)               // GTK self
{
    if (argumentExists(1)) {
        gtk_entry_set_visibility(GTK_ENTRY(self), flag);
    }
    else {
        return gtk_entry_get_visibility(GTK_ENTRY(self));
    }

    return 0;
}

/**
 * Method:  set/get_invisibile_char
 *
 * Set/get the invisible character.
 *
 * @param ichar   The invisible character
 *
 * @return        Zero.
 **/
RexxMethod2(RexxObjectPtr,             // Return type
            GrxEntryGetSetInvisibleChar, // Object_method name
            OPTIONAL_CSTRING, ichar,   // The character
            CSELF, self)               // GTK self
{
    RexxObjectPtr retc;

    if (argumentExists(1)) {
        gtk_entry_set_invisible_char(GTK_ENTRY(self), *ichar);
        retc = (RexxObjectPtr)context->Nil();
    }
    else {
        char ichar2[2] = {' ', '\0'};

        ichar2[0] = gtk_entry_get_invisible_char(GTK_ENTRY(self));
        retc = (RexxObjectPtr)context->NewStringFromAsciiz(ichar2);
    }

    return retc;
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
            GrxEntrySignalConnect,     // Object_method name
            CSTRING, name,             // Signal name
            SUPER, super,              // The superclass override
            CSELF, self)               // GTK self
{
    cbcb *cblock;

    if (strcmp(name, "activate") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_activate";
        g_signal_connect(G_OBJECT(self), "activate",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "backspace") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_backspace";
        g_signal_connect(G_OBJECT(self), "backspace",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "changed") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_changed";
        g_signal_connect(G_OBJECT(self), "changed",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "copy_clipboard") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_copy_clipboard";
        g_signal_connect(G_OBJECT(self), "copy-clipboard",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "cut_clipboard") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_cut_clipboard";
        g_signal_connect(G_OBJECT(self), "cut-clipboard",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "delete_from_cursor") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_delete_from_cursor";
        g_signal_connect(G_OBJECT(self), "delete-from-cursor",
                         G_CALLBACK(signal_func_2), cblock);
        return context->True();
    }
    else if (strcmp(name, "insert_at_cursor") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_insert_at_cursor";
        g_signal_connect(G_OBJECT(self), "insert-at-cursor",
                         G_CALLBACK(signal_func_1), cblock);
        return context->True();
    }
    else if (strcmp(name, "move_cursor") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_move_cursor";
        g_signal_connect(G_OBJECT(self), "move-cursor",
                         G_CALLBACK(signal_func_3), cblock);
        return context->True();
    }
    else if (strcmp(name, "paste_clipboard") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_paste_clipboard";
        g_signal_connect(G_OBJECT(self), "paste-clipboard",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "populate_popup") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_populate_popup";
        g_signal_connect(G_OBJECT(self), "populate-popup",
                         G_CALLBACK(signal_func_1a), cblock);
        return context->True();
    }
    else if (strcmp(name, "toggle_overwrite") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_toggle_overwrite";
        g_signal_connect(G_OBJECT(self), "toggle-overwrite",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    return context->ForwardMessage(NULLOBJECT, NULL, super, NULLOBJECT);
}

/**
 * Method:  init
 *
 * Create a new spin button.
 *
 * @param rxadj   The adjustment object
 *
 * @param crate   The climb rate
 *
 * @param digits  The digits
 *
 * @return        Zero.
 **/
RexxMethod4(int,                       // Return type
            GrxSpinButtonNew,          // Object_method name
            RexxObjectPtr, rxadj,      // Adjustment object
            double, crate,             // Climb rate
            uint32_t, digits,          // Digits
            OSELF, self)               // Self
{
    if (!context->IsOfType(rxadj, "GtkAdjustment")) {
        context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                 context->WholeNumberToObject(1),
                                 context->NewStringFromAsciiz("GtkAdjustment"));
        return 0;
    }
    GtkAdjustment *adj = (GtkAdjustment *)context->ObjectToCSelf(rxadj);
    GtkWidget *myWidget = gtk_spin_button_new(adj, crate, digits);

    // Save ourself
    context->SendMessage1(self, "set_cself", context->NewPointer(myWidget));
    g_object_set_data(G_OBJECT(myWidget), "OORXOBJECT", self);

    return 0;
}

/**
 * Method:  init
 *
 * Create a new spin button.
 *
 * @param lower   The lower limit
 *
 * @param upper   The upper limit
 *
 * @param step    The step increment
 *
 * @return        Zero.
 **/
RexxMethod4(int,                       // Return type
            GrxSpinButtonNewWithRange, // Object_method name
            double, lower,             // Lower limit
            double, upper,             // Upper limit
            double, step,              // Step
            OSELF, self)               // Self
{
    GtkWidget *myWidget = gtk_spin_button_new_with_range(lower, upper, step);

    // Save ourself
    context->SendMessage1(self, "set_cself", context->NewPointer(myWidget));
    g_object_set_data(G_OBJECT(myWidget), "OORXOBJECT", self);

    return 0;
}

/**
 * Method:  set_digits
 *
 * Set the digits.
 *
 * @param digits  The number of digits
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxSpinButtonSetDigits,    // Object_method name
            uint32_t, digits,          // The digits
            CSELF, self)               // GTK self
{
    gtk_spin_button_set_digits(GTK_SPIN_BUTTON(self), digits);

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
            GrxSpinButtonSignalConnect, // Object_method name
            CSTRING, name,             // Signal name
            SUPER, super,              // The superclass override
            OSELF, self)               // Self
{
    cbcb *cblock;

    if (strcmp(name, "change_value") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_change_value";
        g_signal_connect(G_OBJECT(self), "change-value",
                         G_CALLBACK(signal_func_1b), cblock);
        return context->True();
    }
    else if (strcmp(name, "value_changed") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_changed";
        g_signal_connect(G_OBJECT(self), "value-changed",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "wrapped") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_wrapped";
        g_signal_connect(G_OBJECT(self), "wrapped",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    return context->ForwardMessage(NULLOBJECT, NULL, super, NULLOBJECT);
}

