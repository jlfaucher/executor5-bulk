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
    RexxInstance      *instance = ((cbcb *)data)->instance;
    RexxThreadContext *context;

    instance->AttachThread(&context);
    context->SendMessage0(GrxDBFindObject(widget), ((cbcb *)data)->signal_name);
    context->DetachThread();
    return;
}


/*============================================================================*/
/* Public Functions                                                           */
/*============================================================================*/

/*
 * Method:  init
 *
 * Create a push button.
 *
 * @return        Zero.
 */
RexxMethod0(int,                       // Return type
            GrxButtonNew)              // Object_method name
{
    GtkWidget *myWidget;

    myWidget = gtk_button_new();
    context->SetObjectVariable("!POINTER", context->NewPointer(myWidget));
    // add the widget to the db
    GrxDBAdd(context->GetSelf(), myWidget);

    return 0;
}

/*
 * Method:  init
 *
 * Create a push button from stock.
 *
 * @param type    The button type.
 *
 * @return        Zero.
 */
RexxMethod1(int,                       // Return type
            GrxButtonNewFromStock,     // Object_method name
            CSTRING, type)             // Button type
{
    GtkWidget *myWidget;

    myWidget = gtk_button_new_from_stock(type);
    context->SetObjectVariable("!POINTER", context->NewPointer(myWidget));
    // add the widget to the db
    GrxDBAdd(context->GetSelf(), myWidget);

    return 0;
}

/*
 * Method:  set_label
 *
 * Set the button label.
 *
 * @param type    The label text.
 *
 * @return        Zero.
 */
RexxMethod1(int,                       // Return type
            GrxButtonSetLable,         // Object_method name
            CSTRING, text)             // Button type
{
    RexxObjectPtr rxptr = context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);

    gtk_button_set_label(GTK_BUTTON(myWidget), text);

    return 0;
}

/*
 * Method:  get_label
 *
 * Get the button label.
 *
 * @return        Label text.
 */
RexxMethod0(RexxObjectPtr,             // Return type
            GrxButtonGetLabel)         // Object_method name
{
    RexxObjectPtr rxptr = context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);

    return context->NewString(gtk_button_get_label(GTK_BUTTON(myWidget)));
}

/*
 * Method:  set_relief
 *
 * Set the button relief.
 *
 * @param relief  The button relief style.
 *
 * @return        Zero.
 */
RexxMethod1(int,                       // Return type
            GrxButtonSetRelief,       // Object_method name
            int, relief)               // Button relief style
{
    RexxObjectPtr rxptr = context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);

    gtk_button_set_relief(GTK_BUTTON(myWidget), relief);

    return 0;
}

/*
 * Method:  use_underline
 *
 * Set the button underline flag.
 *
 * @param flag    The button underline flag.
 *
 * @return        Zero.
 */
RexxMethod1(int,                       // Return type
            GrxButtonSetUnderline,     // Object_method name
            bool, flag)                // Button underline flag
{
    RexxObjectPtr rxptr = context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);

    gtk_button_set_use_underline(GTK_BUTTON(myWidget), flag);

    return 0;
}

/*
 * Method:  init
 *
 * Create a toggle button.
 *
 * @return        Zero.
 */                      
RexxMethod0(int,                       // Return type
            GrxToggleButtonNew)        // Object_method name
{
    GtkWidget *myWidget;

    myWidget = gtk_toggle_button_new();
    context->SetObjectVariable("!POINTER", context->NewPointer(myWidget));
    // add the widget to the db
    GrxDBAdd(context->GetSelf(), myWidget);

    return 0;
}

/*
 * Method:  get_mode
 *
 * Get the display mode of the button.
 *
 * @return        Mode.
 */
RexxMethod0(bool,                      // Return type
            GrxToggleButtonGetMode)    // Object_method name
{
    RexxObjectPtr rxptr = context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);

    return gtk_toggle_button_get_mode(GTK_TOGGLE_BUTTON(myWidget));
}

/*
 * Method:  set_mode
 *
 * Set the display mode of the button.
 *
 * @param mode    The button mode.
 *
 * @return        Zero.
 */
RexxMethod1(int,                       // Return type
            GrxToggleButtonSetMode,    // Object_method name
            bool, mode)                // Button mode flag
{
    RexxObjectPtr rxptr = context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);

    gtk_toggle_button_set_mode(GTK_TOGGLE_BUTTON(myWidget), mode);

    return 0;
}

/*
 * Method:  get_active
 *
 * Get the active state.
 *
 * @return        Boolean.
 */
RexxMethod0(bool,                      // Return type
            GrxToggleButtonGetActive)  // Object_method name
{
    RexxObjectPtr rxptr = context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);

    return gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(myWidget));
}

/*
 * Method:  set_active
 *
 * Set the active state of the button.
 *
 * @param mode    The button state.
 *
 * @return        Zero.
 */
RexxMethod1(int,                       // Return type
            GrxToggleButtonSetActive,  // Object_method name
            bool, state)               // Button state flag
{
    RexxObjectPtr rxptr = context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);

    gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(myWidget), state);

    return 0;
}

/*
 * Method:  get_inconsistent
 *
 * Get the inconsistent state.
 *
 * @return        Boolean.
 */
RexxMethod0(bool,                      // Return type
            GrxToggleButtonGetInconsistent) // Object_method name
{
    RexxObjectPtr rxptr = context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);

    return gtk_toggle_button_get_inconsistent(GTK_TOGGLE_BUTTON(myWidget));
}

/*
 * Method:  set_inconsistent
 *
 * Set the inconsistent state of the button.
 *
 * @param mode    The button state.
 *
 * @return        Zero.
 */
RexxMethod1(int,                       // Return type
            GrxToggleButtonSetInconsistent, // Object_method name
            bool, state)               // Button state flag
{
    RexxObjectPtr rxptr = context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);

    gtk_toggle_button_set_inconsistent(GTK_TOGGLE_BUTTON(myWidget), state);

    return 0;
}

/*
 * Method:  init
 *
 * Create a check button.
 *
 * @param mode    The label for the button.
 *
 * @return        Zero.
 */
RexxMethod1(int,                       // Return type
            GrxCheckButtonNew,         // Object_method name
            OPTIONAL_CSTRING, text)    // Button text
{
    GtkWidget *myWidget;

    if (text <> NULL) {
       myWidget = gtk_check_button_new_with_label(text);
    }
    else {
        myWidget = gtk_check_button_new();
    }
    context->SetObjectVariable("!POINTER", context->NewPointer(myWidget));
    // add the widget to the db
    GrxDBAdd(context->GetSelf(), myWidget);

    return 0;
}

/*
 * Method:  init
 *
 * Create a radio button.
 *
 * @param group   The group for the button.
 *
 * @param label   The label for the button (optional).
 *
 * @return        Zero.
 */
RexxMethod2(int,                       // Return type
            GrxRadioButtonNew,         // Object_method name
            RexxObjectPtr, rxlistptr,  // GList object
            OPTIONAL_CSTRING, text)    // Button text
{
    RexxObjectPtr listtptr = context->functions->GetObjectVariable(rxlistptr, "!POINTER");
    GList *head = (GList *)context->PointerValue(listptr);
    GtkWidget *myWidget;

    myWidget = gtk_radio_button_new(head);
    if (ltext != NULL) {
        gtk_button_set_label(GTK_BUTTON(myWidget), text);
    }
    context->SetObjectVariable("!POINTER", context->NewPointer(myWidget));
    // add the widget to the db
    GrxDBAdd(context->GetSelf(), myWidget);

    return 0;
}

/*
 * Method:  init
 *
 * Create a radio button from a pointer.
 *
 * @param group   The widget pointer.
 *
 * @return        Zero.
 */
RexxMethod1(int,                       // Return type
            GrxRadioButtonNewFromWidget, // Object_method name
            RexxObjectPtr, rxwidgetptr) // Widget pointer
{
    GtkWidget *srcWidget = (GtkWidget *)context->PointerValue(rxwidgetptr);
    GtkWidget *myWidget;

    myWidget = gtk_radio_button_new_from_widget(GTK_RADIO_BUTTON(srcWidget));
    context->SetObjectVariable("!POINTER", context->NewPointer(myWidget));
    // add the widget to the db
    GrxDBAdd(context->GetSelf(), myWidget);

    return 0;
}

/*
 * Method:  get_group
 *
 * Get the radio button group.
 *
 * @return        GList group.
 */
RexxMethod0(RexxObjectPtr,             // Return type
            GrxRadioButtonGetGroup)    // Object_method name
{
    RexxObjectPtr rxptr = context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);
    GSList *head = NULL;

    head = gtk_radio_button_get_group(GTK_RADIO_BUTTON(myWidget));
    return GrxDBFindObject(head);
}

/*
 * Method:  set_group
 *
 * Set the radio button group.
 *
 * @param group   The group pointer.
 *
 * @return        Zero.
 */
RexxMethod1(int,                       // Return type
            GrxRadioButtonSetGroup,    // Object_method name
            RexxObjectPtr, rxlisttptr) // GList pointer
{
    RexxObjectPtr rxptr = context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);
    RexxObjectPtr listtptr = context->functions->GetObjectVariable(rxlistptr, "!POINTER");
    GList *head = (GList *)context->PointerValue(listptr);

    gtk_radio_button_set_group(GTK_RADIO_BUTTON(myWidget), head);

    return 0;
}

/*
 * Method:  init
 *
 * Create a color button.
 *
 * @param colorstr Color string
 *
 * @return        Zero.
 */
RexxMethod1(int,                       // Return type
            GrxColorButtonNew,         // Object_method name
            OPTIONAL_CSTRING, colorstr)// Color string
{
    GtkWidget *myWidget;
    GdkColor color;

    if (colorstr = NULL) {
        myWidget = gtk_color_button_new();
    }
    else {
        gdk_color_parse(Argv[0].strptr, &color);
        myWidget = gtk_color_button_new_with_color(&color);
    }
    context->SetObjectVariable("!POINTER", context->NewPointer(myWidget));
    // add the widget to the db
    GrxDBAdd(context->GetSelf(), myWidget);

    return 0;
}

/*
 * Method:  set_title
 *
 * Set the color button title.
 *
 * @param title   Color string
 *
 * @return        Zero.
 */
RexxMethod1(int,                       // Return type
            GrxColorButtonSetTitle,    // Object_method name
            CSTRING, title)            // Color string
{
    RexxObjectPtr rxptr = context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);

    gtk_color_button_set_title(GTK_COLOR_BUTTON(myWidget), title);

    return 0;
}

/*
 * Method:  get_color
 *
 * Get the color button color.
 *
 * @param title   Color string
 *
 * @return        Color string
 */
RexxMethod0(RexxObjectPtr,             // Return type
            GrxColorButtonGetColor)    // Object_method name
{
    RexxObjectPtr rxptr = context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);
    GdkColor color;
    char colorstr[64];

    gtk_color_button_get_color(GTK_COLOR_BUTTON(myWidget), &color);

    /* Set up the REXX return code */
    g_snprintf(colorstr, sizeof(colorstr), "#%04X%04X%04X", color.red, color.green, color.blue);
    return context->NewString(colorstr);
}

/*
 * Method:  Set_color
 *
 * Set the color button color.
 *
 * @param title   Color string
 *
 * @return        Zero.
 */
RexxMethod1(int,                       // Return type
            GrxColorButtonSetColor,    // Object_method name
            CSTRING, colorstr)         // Color string
{
    RexxObjectPtr rxptr = context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);
    GdkColor color;

    gdk_color_parse(colorstr, &color);
    gtk_color_button_set_color(GTK_COLOR_BUTTON(myWidget), &color);

    return 0;
}

/*
 * Method:  init
 *
 * Create a file chooser button.
 *
 * @param title   Button title
 *
 * @param action  Button action.
 *
 * @return        Zero.
 */
RexxMethod2(int,                       // Return type
            GrxFileChooserButtonNew,   // Object_method name
            CSTRING, title,            // Title string
            int, action)               // Title action
{
    GtkWidget *myWidget;

    myWidget = gtk_file_chooser_button_new(title, action);
    context->SetObjectVariable("!POINTER", context->NewPointer(myWidget));
    // add the widget to the db
    GrxDBAdd(context->GetSelf(), myWidget);

    return 0;
}

/*
 * Method:  init
 *
 * Create a font button.
 *
 * @return        Zero.
 */
RexxMethod0(int,                       // Return type
            GrxFontButton)             // Object_method name
{
    GtkWidget *myWidget;

    myWidget = gtk_font_button_new();
    context->SetObjectVariable("!POINTER", context->NewPointer(myWidget));
    // add the widget to the db
    GrxDBAdd(context->GetSelf(), myWidget);

    return 0;
}

/*
 * Method:  set_title
 *
 * Set the font button title.
 *
 * @param title   Button title
 *
 * @return        Zero.
 */
RexxMethod1(int,                       // Return type
            GrxFontButtonSetTitle,     // Object_method name
            CSTRING, title)            // Title string
{
    RexxObjectPtr rxptr = context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);

    gtk_font_button_set_title(GTK_FONT_BUTTON(myWidget), title);

    return 0;
}

/*
 * Method:  get_font_name
 *
 * Get the font chosen by the user.
 *
 * @return        Font string
 */
RexxMethod0(int,                       // Return type
            GrxFontButtonGetFontName)  // Object_method name
{
    RexxObjectPtr rxptr = context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);
    const gchar * font;

    font = gtk_font_button_get_font_name(GTK_FONT_BUTTON(myWidget));
    return context->NewString(font);
}


/**
 * Method:  signal_connect
 *
 * Connect a signal to an ooRexx method.
 *
 * @param name    The signal name
 *
 * @return        Zero
 */
RexxMethod1(int,                       // Return type
            GrxButtonSignalConnect,    // Object_method name
            CSTRING, name)             // Signal name
{
    RexxObjectPtr rxptr = context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);
    cbcb *cblock;

    if (strcmp(Argv[1].strptr, "pressed") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->instance;
        cblock->signal_name = "signal_pressed";
        g_signal_connect(G_OBJECT(myWidget), "pressed",
                         G_CALLBACK(signal_func_0), cblock);
    }
    else if (strcmp(Argv[1].strptr, "released") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->instance;
        cblock->signal_name = "signal_released";
        g_signal_connect(G_OBJECT(myWidget), "released",
                         G_CALLBACK(signal_func_0), cblock);
    }
    else if (strcmp(Argv[1].strptr, "clicked") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->instance;
        cblock->signal_name = "signal_clicked";
        g_signal_connect(G_OBJECT(myWidget), "clicked",
                         G_CALLBACK(signal_func_0), cblock);
    }
    else if (strcmp(Argv[1].strptr, "enter") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->instance;
        cblock->signal_name = "signal_enter";
        g_signal_connect(G_OBJECT(myWidget), "enter",
                         G_CALLBACK(signal_func_0), cblock);
    }
    else if (strcmp(Argv[1].strptr, "leave") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->instance;
        cblock->signal_name = "signal_leave";
        g_signal_connect(G_OBJECT(myWidget), "leave",
                         G_CALLBACK(signal_func_0), cblock);
    }
    else {
        RexxObjectPtr parent = context->GetSuper();
        context->SendMessage0(parent, name);
    }

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
 */
RexxMethod1(int,                       // Return type
            GrxToggleButtonSignalConnect, // Object_method name
            CSTRING, name)             // Signal name
{
    RexxObjectPtr rxptr = context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);
    cbcb *cblock;

    if (strcmp(Argv[1].strptr, "toggled") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->instance;
        cblock->signal_name = "signal_toggked";
        g_signal_connect(G_OBJECT(myWidget), "toggled",
                         G_CALLBACK(signal_func_0), cblock);
    }
    else {
        RexxObjectPtr parent = context->GetSuper();
        context->SendMessage0(parent, name);
    }

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
 */
RexxMethod1(int,                       // Return type
            GrxRadioButtonSignalConnect, // Object_method name
            CSTRING, name)             // Signal name
{
    RexxObjectPtr rxptr = context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);
    cbcb *cblock;

    if (strcmp(Argv[1].strptr, "group_changed") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->instance;
        cblock->signal_name = "signal_group_changed";
        g_signal_connect(G_OBJECT(myWidget), "group-changed",
                         G_CALLBACK(signal_func_0), cblock);
    }
    else {
        RexxObjectPtr parent = context->GetSuper();
        context->SendMessage0(parent, name);
    }

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
 */
RexxMethod1(int,                       // Return type
            GrxColorButtonSignalConnect, // Object_method name
            CSTRING, name)             // Signal name
{
    RexxObjectPtr rxptr = context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);
    cbcb *cblock;

    if (strcmp(Argv[1].strptr, "color_set") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->instance;
        cblock->signal_name = "signal_color_set";
        g_signal_connect(G_OBJECT(myWidget), "color-set",
                         G_CALLBACK(signal_func_0), cblock");
    }
    else {
        RexxObjectPtr parent = context->GetSuper();
        context->SendMessage0(parent, name);
    }

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
 */
RexxMethod1(int,                       // Return type
            GrxFileChooserButtonSignalConnect, // Object_method name
            CSTRING, name)             // Signal name
{
    RexxObjectPtr rxptr = context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);
    cbcb *cblock;

    if (strcmp(Argv[1].strptr, "file_set") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->instance;
        cblock->signal_name = "signal_file_set";
        g_signal_connect(G_OBJECT(myWidget), "file-set",
                         G_CALLBACK(signal_func_0), cblock);
    }
    else {
        RexxObjectPtr parent = context->GetSuper();
        context->SendMessage0(parent, name);
    }

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
 */
RexxMethod1(int,                       // Return type
            GrxFontButtonSignalConnect, // Object_method name
            CSTRING, name)             // Signal name
{
    RexxObjectPtr rxptr = context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);
    cbcb *cblock;

    if (strcmp(Argv[1].strptr, "font_set") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->instance;
        cblock->signal_name = "signal_font_set";
        g_signal_connect(G_OBJECT(myWidget), "font-set",
                         G_CALLBACK(signal_func_0), cblock);
    }
    else {
        RexxObjectPtr parent = context->GetSuper();
        context->SendMessage0(parent, name);
    }

    return 0;
}

