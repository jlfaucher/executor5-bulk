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

static void signal_func_0(GtkWidget *widget,
                          gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(widget), "OORXOBJECT");
    RexxThreadContext *context;

    cblock->instance->AttachThread(&context);
    context->SendMessage0(rxobj, ((cbcb *)data)->signal_name);
    context->DetachThread();
    return;
}


/*============================================================================*/
/* Public Functions                                                           */
/*============================================================================*/

/**
 * Method:  init
 *
 * Create a push button.
 *
 * @return        Zero.
 **/
RexxMethod1(int,                       // Return type
            GrxButtonNew,              // Object_method name
            OSELF, self)               // Self
{
    GtkWidget *myWidget = gtk_button_new();

    // Save ourself
    context->SendMessage1(self, "set_cself", context->NewPointer(myWidget));
    g_object_set_data(G_OBJECT(myWidget), "OORXOBJECT", self);

    return 0;
}

/**
 * Method:  set_use_stock
 *
 * Indicate the label is a stock item.
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxButtonSetUseStock,      // Object_method name
            logical_t, flag,           // Label is stock item flag
            CSELF, self)               // GTK self
{
    gtk_button_set_use_stock(GTK_BUTTON(self), flag);

    return 0;
}

/**
 * Method:  set/get_label
 *
 * Set/get the button label.
 *
 * @param type    The label text.
 *
 * @return        Zero.
 **/
RexxMethod2(RexxObjectPtr,             // Return type
            GrxButtonGetSetLabel,      // Object_method name
            OPTIONAL_CSTRING, text,    // Button type
            CSELF, self)               // GTK self
{
    RexxObjectPtr retc;

    if (argumentExists(1)) {
        gtk_button_set_label(GTK_BUTTON(self), text);
        retc = (RexxObjectPtr)context->Nil();
    }
    else {
        retc = (RexxObjectPtr) context->NewStringFromAsciiz(gtk_button_get_label(GTK_BUTTON(self)));
    }

    return retc;
}

/**
 * Method:  set_relief
 *
 * Set the button relief.
 *
 * @param relief  The button relief style.
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxButtonSetRelief,       // Object_method name
            int, relief,               // Button relief style
            CSELF, self)               // GTK self
{
    gtk_button_set_relief(GTK_BUTTON(self), (GtkReliefStyle)relief);

    return 0;
}

/**
 * Method:  use_underline
 *
 * Set the button underline flag.
 *
 * @param flag    The button underline flag.
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxButtonUseUnderline,     // Object_method name
            logical_t, flag,           // Button underline flag
            CSELF, self)               // GTK self
{
    gtk_button_set_use_underline(GTK_BUTTON(self), flag);

    return 0;
}

/**
 * Method:  init
 *
 * Create a toggle button.
 *
 * @return        Zero.
 **/
RexxMethod1(int,                       // Return type
            GrxToggleButtonNew,        // Object_method name
            OSELF, self)               // Self
{
    GtkWidget *myWidget = gtk_toggle_button_new();

    // Save ourself
    context->SendMessage1(self, "set_cself", context->NewPointer(myWidget));
    g_object_set_data(G_OBJECT(myWidget), "OORXOBJECT", self);

    return 0;
}

/**
 * Method:  get/set_mode
 *
 * Get/set the display mode of the button.
 *
 * @return        Mode.
 **/
RexxMethod2(logical_t,                 // Return type
            GrxToggleButtonGetSetMode, // Object_method name
            OPTIONAL_logical_t, mode,  // Button mode flag
            CSELF, self)               // GTK self
{
    if (argumentExists(2)) {
        gtk_toggle_button_set_mode(GTK_TOGGLE_BUTTON(self), mode);
    }
    else {
        return gtk_toggle_button_get_mode(GTK_TOGGLE_BUTTON(self));
    }
    return 0;
}

/**
 * Method:  set/get_active
 *
 * Set/get the active state of the button.
 *
 * @param mode    The button state.
 *
 * @return        Zero.
 **/
RexxMethod2(logical_t,                 // Return type
            GrxToggleButtonGetSetActive, // Object_method name
            OPTIONAL_logical_t, state, // Button state flag
            CSELF, self)               // GTK self
{
    if (argumentExists(1)) {
        gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(self), state);
    }
    else {
        return gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(self));
    }

    return 0;
}

/**
 * Method:  set/get_inconsistent
 *
 * Set/get the inconsistent state of the button.
 *
 * @param mode    The button state.
 *
 * @return        Zero.
 **/
RexxMethod2(logical_t,                 // Return type
            GrxToggleButtonGetSetInconsistent, // Object_method name
            OPTIONAL_logical_t, state, // Button state flag
            CSELF, self)               // GTK self
{
    if (argumentExists(1)) {
        gtk_toggle_button_set_inconsistent(GTK_TOGGLE_BUTTON(self), state);
    }
    else {
        return gtk_toggle_button_get_inconsistent(GTK_TOGGLE_BUTTON(self));
    }

    return 0;
}

/**
 * Method:  init
 *
 * Create a check button.
 *
 * @param mode    The label for the button.
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxCheckButtonNew,         // Object_method name
            OPTIONAL_CSTRING, text,    // Button text
            OSELF, self)               // Self
{
    GtkWidget *myWidget;

    if (argumentExists(1)) {
       myWidget = gtk_check_button_new_with_label(text);
    }
    else {
        myWidget = gtk_check_button_new();
    }

    // Save ourself
    context->SendMessage1(self, "set_cself", context->NewPointer(myWidget));
    g_object_set_data(G_OBJECT(myWidget), "OORXOBJECT", self);

    return 0;
}

/**
 * Method:  init
 *
 * Create a radio button.
 *
 * @param group   The group for the button.
 *
 * @param label   The label for the button (optional).
 *
 * @return        Zero.
 **/
RexxMethod3(int,                       // Return type
            GrxRadioButtonNew,         // Object_method name
            RexxObjectPtr, rxwidget,   // Group widget
            OPTIONAL_CSTRING, text,    // Button text
            OSELF, self)               // Self
{
    GSList *head = NULL;
    GtkWidget *myWidget;

    if (rxwidget == context->Nil()) {
        myWidget = gtk_radio_button_new(NULL);
    }
    else {
        if (!context->IsOfType(rxwidget, "GtkWidget")) {
            context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                     context->WholeNumberToObject(1),
                                     context->NewStringFromAsciiz("GtkWidget"));
            return 0;
        }
        GtkWidget *cpWidget = (GtkWidget *)context->ObjectToCSelf(rxwidget);
        if (cpWidget == NULL) {
            myWidget = gtk_radio_button_new(NULL);
        }
        else {
            myWidget = gtk_radio_button_new_from_widget(GTK_RADIO_BUTTON(cpWidget));
        }
    }
    if (argumentExists(2)) {
        gtk_button_set_label(GTK_BUTTON(myWidget), text);
    }

    // Save ourself
    context->SendMessage1(self, "set_cself", context->NewPointer(myWidget));
    g_object_set_data(G_OBJECT(myWidget), "OORXOBJECT", self);

    return 0;
}

/**
 * Method:  init
 *
 * Create a color button.
 *
 * @param colorstr Color string
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxColorButtonNew,         // Object_method name
            OPTIONAL_CSTRING, colorstr,// Color string
            OSELF, self)               // Self
{
    GtkWidget *myWidget;
    GdkColor color;

    if (colorstr = NULL) {
        myWidget = gtk_color_button_new();
    }
    else {
        gdk_color_parse(colorstr, &color);
        myWidget = gtk_color_button_new_with_color(&color);
    }

    // Save ourself
    context->SendMessage1(self, "set_cself", context->NewPointer(myWidget));
    g_object_set_data(G_OBJECT(myWidget), "OORXOBJECT", self);

    return 0;
}

/**
 * Method:  set_title
 *
 * Set the color button title.
 *
 * @param title   Color string
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxColorButtonSetTitle,    // Object_method name
            CSTRING, title,            // Color string
            CSELF, self)               // GTK self
{
    gtk_color_button_set_title(GTK_COLOR_BUTTON(self), title);

    return 0;
}

/**
 * Method:  Set/get_color
 *
 * Set/get the color button color.
 *
 * @param title   Color string
 *
 * @return        Zero.
 **/
RexxMethod2(RexxObjectPtr,             // Return type
            GrxColorButtonGetSetColor, // Object_method name
            OPTIONAL_CSTRING, colorstr,// Color string
            CSELF, self)               // GTK self
{
    RexxObjectPtr retc;
    GdkColor color;

    if (argumentExists(1)) {
        gdk_color_parse(colorstr, &color);
        gtk_color_button_set_color(GTK_COLOR_BUTTON(self), &color);
        retc = (RexxObjectPtr)context->Nil();
    }
    else {
        char colorstr[64];

        gtk_color_button_get_color(GTK_COLOR_BUTTON(self), &color);
        g_snprintf(colorstr, sizeof(colorstr), "#%04X%04X%04X", color.red, color.green, color.blue);
        retc = (RexxObjectPtr)context->NewStringFromAsciiz(colorstr);
    }

    return retc;
}

/**
 * Method:  init
 *
 * Create a file chooser button.
 *
 * @param title   Button title
 *
 * @param action  Button action.
 *
 * @return        Zero.
 **/
RexxMethod3(int,                       // Return type
            GrxFileChooserButtonNew,   // Object_method name
            CSTRING, title,            // Title string
            int, action,               // Title action
            OSELF, self)               // Self
{
    GtkWidget *myWidget = gtk_file_chooser_button_new(title, (GtkFileChooserAction)action);

    // Save ourself
    context->SendMessage1(self, "set_cself", context->NewPointer(myWidget));
    g_object_set_data(G_OBJECT(myWidget), "OORXOBJECT", self);

    return 0;
}

/**
 * Method:  init
 *
 * Create a font button.
 *
 * @return        Zero.
 **/
RexxMethod1(int,                       // Return type
            GrxFontButtonNew,          // Object_method name
            OSELF, self)               // Self
{
    GtkWidget *myWidget = gtk_font_button_new();

    // Save ourself
    context->SendMessage1(self, "set_cself", context->NewPointer(myWidget));
    g_object_set_data(G_OBJECT(myWidget), "OORXOBJECT", self);

    return 0;
}

/**
 * Method:  set_title
 *
 * Set the font button title.
 *
 * @param title   Button title
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxFontButtonSetTitle,     // Object_method name
            CSTRING, title,            // Title string
            CSELF, self)               // GTK self
{
    gtk_font_button_set_title(GTK_FONT_BUTTON(self), title);

    return 0;
}

/**
 * Method:  get_font_name
 *
 * Get the font chosen by the user.
 *
 * @return        Font string
 **/
RexxMethod1(RexxObjectPtr,             // Return type
            GrxFontButtonGetFontName,  // Object_method name
            CSELF, self)               // GTK self
{
    const gchar * font = gtk_font_button_get_font_name(GTK_FONT_BUTTON(self));
    if (font == NULL) {
        return (RexxObjectPtr)context->NewStringFromAsciiz("\0");
    }
    return (RexxObjectPtr)context->NewStringFromAsciiz(font);
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
            GrxButtonSignalConnect,    // Object_method name
            CSTRING, name,             // Signal name
            SUPER, super,              // The superclass override
            CSELF, self)               // GTK self
{
    cbcb *cblock;

    if (strcmp(name, "pressed") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_pressed";
        g_signal_connect(G_OBJECT(self), "pressed",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "released") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_released";
        g_signal_connect(G_OBJECT(self), "released",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "activate") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_activate";
        g_signal_connect(G_OBJECT(self), "activate",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "clicked") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_clicked";
        g_signal_connect(G_OBJECT(self), "clicked",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "enter") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_enter";
        g_signal_connect(G_OBJECT(self), "enter",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "leave") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_leave";
        g_signal_connect(G_OBJECT(self), "leave",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    return context->ForwardMessage(NULLOBJECT, NULL, super, NULLOBJECT);
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
            GrxToggleButtonSignalConnect, // Object_method name
            CSTRING, name,             // Signal name
            SUPER, super,              // The superclass override
            CSELF, self)               // GTK self
{
    cbcb *cblock;

    if (strcmp(name, "toggled") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_toggled";
        g_signal_connect(G_OBJECT(self), "toggled",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    return context->ForwardMessage(NULLOBJECT, NULL, super, NULLOBJECT);
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
            GrxRadioButtonSignalConnect, // Object_method name
            CSTRING, name,             // Signal name
            SUPER, super,              // The superclass override
            CSELF, self)               // GTK self
{
    cbcb *cblock;

    if (strcmp(name, "group_changed") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_group_changed";
        g_signal_connect(G_OBJECT(self), "group-changed",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    return context->ForwardMessage(NULLOBJECT, NULL, super, NULLOBJECT);
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
            GrxColorButtonSignalConnect, // Object_method name
            CSTRING, name,             // Signal name
            SUPER, super,              // The superclass override
            CSELF, self)               // GTK self
{
    cbcb *cblock;

    if (strcmp(name, "color_set") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_color_set";
        g_signal_connect(G_OBJECT(self), "color-set",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    return context->ForwardMessage(NULLOBJECT, NULL, super, NULLOBJECT);
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
            GrxFileChooserButtonSignalConnect, // Object_method name
            CSTRING, name,             // Signal name
            SUPER, super,              // The superclass override
            CSELF, self)               // GTK self
{
    cbcb *cblock;

    if (strcmp(name, "file_set") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_file_set";
        g_signal_connect(G_OBJECT(self), "file-set",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    return context->ForwardMessage(NULLOBJECT, NULL, super, NULLOBJECT);
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
            GrxFontButtonSignalConnect, // Object_method name
            CSTRING, name,             // Signal name
            SUPER, super,              // The superclass override
            CSELF, self)               // GTK self
{
    cbcb *cblock;

    if (strcmp(name, "font_set") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_font_set";
        g_signal_connect(G_OBJECT(self), "font-set",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    return context->ForwardMessage(NULLOBJECT, NULL, super, NULLOBJECT);
}

