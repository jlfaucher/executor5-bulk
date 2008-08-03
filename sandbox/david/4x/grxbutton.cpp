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
    GtkWidget *myWidget;

    myWidget = gtk_button_new();
    context->SendMessage1(self, "POINTER=", context->NewPointer(myWidget));
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
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkButton *myWidget = (GtkButton *)context->PointerValue(rxptr);

    gtk_button_set_use_stock(myWidget, flag);

    return 0;
}

/**
 * Method:  set_label
 *
 * Set the button label.
 *
 * @param type    The label text.
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxButtonSetLabel,         // Object_method name
            CSTRING, text,             // Button type
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);

    gtk_button_set_label(GTK_BUTTON(myWidget), text);

    return 0;
}

/**
 * Method:  get_label
 *
 * Get the button label.
 *
 * @return        Label text.
 **/
RexxMethod1(RexxObjectPtr,             // Return type
            GrxButtonGetLabel,         // Object_method name
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);

    return context->NewStringFromAsciiz(gtk_button_get_label(GTK_BUTTON(myWidget)));
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
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);

    gtk_button_set_relief(GTK_BUTTON(myWidget), (GtkReliefStyle)relief);

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
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);

    gtk_button_set_use_underline(GTK_BUTTON(myWidget), flag);

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
    GtkWidget *myWidget;

    myWidget = gtk_toggle_button_new();
    context->SendMessage1(self, "POINTER=", context->NewPointer(myWidget));
    g_object_set_data(G_OBJECT(myWidget), "OORXOBJECT", self);

    return 0;
}

/**
 * Method:  get_mode
 *
 * Get the display mode of the button.
 *
 * @return        Mode.
 **/
RexxMethod1(logical_t,                 // Return type
            GrxToggleButtonGetMode,    // Object_method name
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);

    return gtk_toggle_button_get_mode(GTK_TOGGLE_BUTTON(myWidget));
}

/**
 * Method:  set_mode
 *
 * Set the display mode of the button.
 *
 * @param mode    The button mode.
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxToggleButtonSetMode,    // Object_method name
            logical_t, mode,           // Button mode flag
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);

    gtk_toggle_button_set_mode(GTK_TOGGLE_BUTTON(myWidget), mode);

    return 0;
}

/**
 * Method:  get_active
 *
 * Get the active state.
 *
 * @return        Boolean.
 **/
RexxMethod1(logical_t,                 // Return type
            GrxToggleButtonGetActive,  // Object_method name
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);

    return gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(myWidget));
}

/**
 * Method:  set_active
 *
 * Set the active state of the button.
 *
 * @param mode    The button state.
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxToggleButtonSetActive,  // Object_method name
            logical_t, state,          // Button state flag
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);

    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(myWidget), state);

    return 0;
}

/**
 * Method:  get_inconsistent
 *
 * Get the inconsistent state.
 *
 * @return        Boolean.
 **/
RexxMethod1(logical_t,                 // Return type
            GrxToggleButtonGetInconsistent, // Object_method name
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);

    return gtk_toggle_button_get_inconsistent(GTK_TOGGLE_BUTTON(myWidget));
}

/**
 * Method:  set_inconsistent
 *
 * Set the inconsistent state of the button.
 *
 * @param mode    The button state.
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxToggleButtonSetInconsistent, // Object_method name
            logical_t, state,         // Button state flag
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);

    gtk_toggle_button_set_inconsistent(GTK_TOGGLE_BUTTON(myWidget), state);

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
            OSELF, self,               // Self
            OPTIONAL_CSTRING, text)    // Button text
{
    GtkWidget *myWidget;

    if (text != NULL) {
       myWidget = gtk_check_button_new_with_label(text);
    }
    else {
        myWidget = gtk_check_button_new();
    }
    context->SendMessage1(self, "POINTER=", context->NewPointer(myWidget));
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
            OSELF, self,               // Self
            RexxObjectPtr, rxlistptr,  // GList object
            OPTIONAL_CSTRING, text)    // Button text
{
    RexxPointerObject listptr = (RexxPointerObject)context->SendMessage0(rxlistptr, "POINTER");
    GSList *head = NULL;
    GtkWidget *myWidget;

    if (listptr != NULL) {
        GSList *head = (GSList *)context->PointerValue(listptr);
    }
    myWidget = gtk_radio_button_new(head);
    if (text != NULL) {
        gtk_button_set_label(GTK_BUTTON(myWidget), text);
    }
    context->SendMessage1(self, "POINTER=", context->NewPointer(myWidget));
    g_object_set_data(G_OBJECT(myWidget), "OORXOBJECT", self);

    return 0;
}

/**
 * Method:  get_group
 *
 * Get the radio button group.
 *
 * @return        GList group.
 **/
RexxMethod1(RexxObjectPtr,             // Return type
            GrxRadioButtonGetGroup,    // Object_method name
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GSList *myWidget = (GSList *)context->PointerValue(rxptr);
    GSList *head = NULL;

    head = gtk_radio_button_get_group(GTK_RADIO_BUTTON(myWidget));
    if (head == NULL) {
        return context->Nil();
    }
    return (RexxObjectPtr)g_object_get_data(G_OBJECT(myWidget), "OORXOBJECT");
}

/**
 * Method:  set_group
 *
 * Set the radio button group.
 *
 * @param group   The group pointer.
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxRadioButtonSetGroup,    // Object_method name
            RexxObjectPtr, rxlistptr,  // GList pointer
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);
    RexxPointerObject listptr = (RexxPointerObject)context->SendMessage0(rxlistptr, "POINTER");
    GSList *head = (GSList *)context->PointerValue(listptr);

    gtk_radio_button_set_group(GTK_RADIO_BUTTON(myWidget), head);

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
            OSELF, self,               // Self
            OPTIONAL_CSTRING, colorstr)// Color string
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
    context->SendMessage1(self, "POINTER=", context->NewPointer(myWidget));
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
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);

    gtk_color_button_set_title(GTK_COLOR_BUTTON(myWidget), title);

    return 0;
}

/**
 * Method:  get_color
 *
 * Get the color button color.
 *
 * @param title   Color string
 *
 * @return        Color string
 **/
RexxMethod1(RexxObjectPtr,             // Return type
            GrxColorButtonGetColor,    // Object_method name
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);
    GdkColor color;
    char colorstr[64];

    gtk_color_button_get_color(GTK_COLOR_BUTTON(myWidget), &color);

    /* Set up the REXX return code */
    g_snprintf(colorstr, sizeof(colorstr), "#%04X%04X%04X", color.red, color.green, color.blue);
    return context->NewStringFromAsciiz(colorstr);
}

/**
 * Method:  Set_color
 *
 * Set the color button color.
 *
 * @param title   Color string
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxColorButtonSetColor,    // Object_method name
            CSTRING, colorstr,         // Color string
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);
    GdkColor color;

    gdk_color_parse(colorstr, &color);
    gtk_color_button_set_color(GTK_COLOR_BUTTON(myWidget), &color);

    return 0;
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
            OSELF, self,               // Self
            CSTRING, title,            // Title string
            int, action)               // Title action
{
    GtkWidget *myWidget;

    myWidget = gtk_file_chooser_button_new(title, (GtkFileChooserAction)action);
    context->SendMessage1(self, "POINTER=", context->NewPointer(myWidget));
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
    GtkWidget *myWidget;

    myWidget = gtk_font_button_new();
    context->SendMessage1(self, "POINTER=", context->NewPointer(myWidget));
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
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);

    gtk_font_button_set_title(GTK_FONT_BUTTON(myWidget), title);

    return 0;
}

/**
 * Method:  get_font_name
 *
 * Get the font chosen by the user.
 *
 * @return        Font string
 **/
RexxMethod1(CSTRING,                   // Return type
            GrxFontButtonGetFontName,  // Object_method name
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);
    const gchar * font;

    return gtk_font_button_get_font_name(GTK_FONT_BUTTON(myWidget));
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
            ARGLIST, args,             // The whole argument list as an array
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);
    cbcb *cblock;

    if (strcmp(name, "pressed") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_pressed";
        g_signal_connect(G_OBJECT(myWidget), "pressed",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "released") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_released";
        g_signal_connect(G_OBJECT(myWidget), "released",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "activate") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_activate";
        g_signal_connect(G_OBJECT(myWidget), "activate",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "clicked") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_clicked";
        g_signal_connect(G_OBJECT(myWidget), "clicked",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "enter") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_enter";
        g_signal_connect(G_OBJECT(myWidget), "enter",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "leave") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_leave";
        g_signal_connect(G_OBJECT(myWidget), "leave",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    return context->SendSuperMessage("signal_connect", args);
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
            ARGLIST, args,             // The whole argument list as an array
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);
    cbcb *cblock;

    if (strcmp(name, "toggled") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_toggled";
        g_signal_connect(G_OBJECT(myWidget), "toggled",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    return context->SendSuperMessage("signal_connect", args);
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
            ARGLIST, args,             // The whole argument list as an array
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);
    cbcb *cblock;

    if (strcmp(name, "group_changed") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_group_changed";
        g_signal_connect(G_OBJECT(myWidget), "group-changed",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    return context->SendSuperMessage("signal_connect", args);
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
            ARGLIST, args,             // The whole argument list as an array
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);
    cbcb *cblock;

    if (strcmp(name, "color_set") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_color_set";
        g_signal_connect(G_OBJECT(myWidget), "color-set",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    return context->SendSuperMessage("signal_connect", args);
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
            ARGLIST, args,             // The whole argument list as an array
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);
    cbcb *cblock;

    if (strcmp(name, "file_set") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_file_set";
        g_signal_connect(G_OBJECT(myWidget), "file-set",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    return context->SendSuperMessage("signal_connect", args);
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
            ARGLIST, args,             // The whole argument list as an array
            OSELF, self)               // Self
{
    RexxPointerObject rxptr = (RexxPointerObject)context->SendMessage0(self, "POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);
    cbcb *cblock;

    if (strcmp(name, "font_set") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_font_set";
        g_signal_connect(G_OBJECT(myWidget), "font-set",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    return context->SendSuperMessage("signal_connect", args);
}

