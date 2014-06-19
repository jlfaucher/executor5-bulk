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
                          gint arg1,
                          gpointer data)
{
    cbcb *cblock = (cbcb *)data;
    RexxObjectPtr rxobj = (RexxObjectPtr)g_object_get_data(G_OBJECT(window), "OORXOBJECT");
    RexxThreadContext *context;

    cblock->instance->AttachThread(&context);
    RexxObjectPtr rxarg1 = context->WholeNumberToObject((wholenumber_t)arg1);;
    context->SendMessage1(rxobj, ((cbcb *)data)->signal_name, rxarg1);
    context->DetachThread();
    return;
}


/*============================================================================*/
/* Public Functions                                                           */
/*============================================================================*/

/**
 * Method:  init
 *
 * Create a dialog with buttons.
 *
 * @param title   The dialog title
 *
 * @param rxparent The parent window
 *
 * @param flags   The flags
 *
 * @param btext1  The id or text (repeating)
 *
 * @param bid1    The response id (repeating)
 *
 * @return        Zero.
 **/
RexxMethod5(int,                       // Return type
            GrxDialogNewWithButtons,   // Object_method name
            CSTRING, title,            // Dialog title
            RexxObjectPtr, parent,     // Parent window
            int, flags,                // Dialog flags
            ARGLIST, args,             // Array of buttons and responses
            OSELF, self)               // Self
{
    GtkWindow *myParent = NULL;
    GtkWidget *myWidget, *vbox;
    const gchar *bid, *rids;
    int rid;
    size_t members = context->ArraySize(args);

    if (parent != context->Nil()) {
        if (!context->IsOfType(parent, "GtkWindow")) {
            context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                     context->WholeNumberToObject(2),
                                     context->NewStringFromAsciiz("GtkWindow"));
            return 0;
        }
        myParent = (GtkWindow *)context->ObjectToCSelf(parent);
    }
    myWidget = gtk_dialog_new_with_buttons(title, myParent,
                                           (GtkDialogFlags)flags, NULL);
    for (int i = 5; i <= members; i += 2) {
        bid = context->ObjectToStringValue(context->ArrayAt(args, i - 1));
        context->ObjectToInt32(context->ArrayAt(args, i), &rid);
        gtk_dialog_add_button(GTK_DIALOG(myWidget), bid, rid);
    }

    // Save ourself
    context->SendMessage1(self, "set_cself", context->NewPointer(myWidget));
    g_object_set_data(G_OBJECT(myWidget), "OORXOBJECT", self);

    // create the vbox object
#if GTK_CHECK_VERSION(2,14,0)
	vbox = gtk_dialog_get_content_area(GTK_DIALOG(myWidget));
#else
    vbox = GTK_DIALOG(myWidget)->vbox;
#endif // GTK_CHECK_VERSION
    context->SendMessage1(self, "VBOX=", context->NewPointer(vbox));

    return 0;
}

/**
 * Method:  add_button
 *
 * Add buttons to a dialog.
 *
 * @param btext1  The button text
 *
 * @param bid1    The response id
 *
 * @return        Zero.
 **/
RexxMethod3(int,                       // Return type
            GrxDialogAddButton,        // Object_method name
            CSTRING, btext,            // Button text
            int, bid,                  // Response id
            CSELF, self)               // GTK self
{
    gtk_dialog_add_button(GTK_DIALOG(self), btext, bid);

    return 0;
}

/**
 * Method:  set_has_separator
 *
 * Add/remove the button separator.
 *
 * @param flag    The separator flag.
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxDialogSetHasSeparator,  // Object_method name
            logical_t, flag,           // Separator boolean
            CSELF, self)               // GTK self
{
#if GTK_CHECK_VERSION(3,0,0)
	return -1;
#else
    gtk_dialog_set_has_separator(GTK_DIALOG(self), flag);
    return 0;
#endif // GTK_CHECK_VERSION
}

/**
 * Method:  set_default_response
 *
 * Set the default response id.
 *
 * @param rid     Response id
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxDialogSetDefaultResponse, // Object_method name
            int, rid,                  // Response id
            CSELF, self)               // GTK self
{
    gtk_dialog_set_default_response(GTK_DIALOG(self), rid);

    return 0;
}

/**
 * Method:  dialog_run
 *
 * Run a dialog.
 *
 * @return        Response id
 **/
RexxMethod1(int,                       // Return type
            GrxDialogRun,              // Object_method name
            CSELF, self)               // GTK self
{
    return gtk_dialog_run(GTK_DIALOG(self));
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
            GrxDialogSignalConnect,    // Object_method name
            CSTRING, name,             // Signal name
            SUPER, super,              // The superclass override
            CSELF, self)               // GTK self
{
    cbcb *cblock;

    if (strcmp(name, "close") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_close";
        g_signal_connect(G_OBJECT(self), "close",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "response") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_response";
        g_signal_connect(G_OBJECT(self), "response",
                         G_CALLBACK(signal_func_1), cblock);
        return context->True();
    }
    return context->ForwardMessage(NULLOBJECT, NULL, super, NULLOBJECT);
}

/**
 * Method:  init
 *
 * Create a message dialog.
 *
 * @param rxparent The parent window
 *
 * @param flags   The flags
 *
 * @param type    The message type
 *
 * @param bset    The button set
 *
 * @param text    The message text
 *
 * @return        Zero.
 **/
RexxMethod6(int,                       // Return type
            GrxMessageDialogNew,       // Object_method name
            RexxObjectPtr, parent,     // Parent window
            int, flags,                // Dialog flags
            int, type,                 // Message type
            int, bset,                 // Button set
            CSTRING, text,             // Message text
            OSELF, self)               // Self
{
    GtkWindow *myParent = NULL;
    GtkWidget *myWidget;

    if (parent != context->Nil()) {
        if (!context->IsOfType(parent, "GtkWindow")) {
            context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                     context->WholeNumberToObject(1),
                                     context->NewStringFromAsciiz("GtkWindow"));
            return 0;
        }
        myParent = (GtkWindow *)context->ObjectToCSelf(parent);
    }
    myWidget = gtk_message_dialog_new(myParent, (GtkDialogFlags)flags,
                                      (GtkMessageType)type,
                                      (GtkButtonsType)bset, text);

    // Save ourself
    context->SendMessage1(self, "set_cself", context->NewPointer(myWidget));
    g_object_set_data(G_OBJECT(myWidget), "OORXOBJECT", self);

    return 0;
}

/**
 * Method:  init
 *
 * Create a file chooser dialog.
 *
 * @param title   The title
 *
 * @param rxparent The parent window
 *
 * @param type    The message type
 *
 * @param bset    The button id or text (repeating)
 *
 * @param rid     The response id (repeating)
 *
 * @return        Zero.
 **/
RexxMethod5(int,                       // Return type
            GrxFileChooserDialogNew,   // Object_method name
            CSTRING, title,            // Title
            RexxObjectPtr, parent,     // Parent window
            int, action,               // Dialog action type
            ARGLIST, args,             // Array of buttons and responses
            OSELF, self)               // Self
{
    GtkWindow *myParent = NULL;
    GtkWidget *myWidget;
    const gchar *bid;
    int rid;
    size_t members = context->ArraySize(args);

    if (parent != context->Nil()) {
        if (!context->IsOfType(parent, "GtkWindow")) {
            context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                     context->WholeNumberToObject(2),
                                     context->NewStringFromAsciiz("GtkWindow"));
            return 0;
        }
        myParent = (GtkWindow *)context->ObjectToCSelf(parent);
    }
    myWidget = gtk_file_chooser_dialog_new(title, myParent,
                                           (GtkFileChooserAction)action, NULL);
    for (int i = 5; i <= members; i += 2) {
        if (!context->IsOfType(context->ArrayAt(args, i - 1), "String")) {
            context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                     context->WholeNumberToObject(i - 1),
                                     context->NewStringFromAsciiz("String"));
            return 0;
        }
        bid = context->ObjectToStringValue(context->ArrayAt(args, i - 1));
        context->ObjectToInt32(context->ArrayAt(args, i), &rid);
        gtk_dialog_add_button(GTK_DIALOG(myWidget), bid, rid);
    }

    // Save ourself
    context->SendMessage1(self, "set_cself", context->NewPointer(myWidget));
    g_object_set_data(G_OBJECT(myWidget), "OORXOBJECT", self);

    return 0;
}

/**
 * Method:  init
 *
 * Create a font selection dialog.
 *
 * @param title   The dialog title
 *
 * Create a font selection dialog.
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxFontSelectionDialogNew, // Object_method name
            CSTRING, title,            // Dialog title
            OSELF, self)               // Self
{
    GtkWidget *myWidget = gtk_font_selection_dialog_new(title);

    // Save ourself
    context->SendMessage1(self, "set_cself", context->NewPointer(myWidget));
    g_object_set_data(G_OBJECT(myWidget), "OORXOBJECT", self);

    return 0;
}

/**
 * Method:  set/get_font_name
 *
 * Setget the font in the dialog.
 *
 * @param name    The font name
 *
 * @return        Zero.
 **/
RexxMethod2(RexxObjectPtr,             // Return type
            GrxFontSelectionDialogGetSetFontName, // Object_method name
            OPTIONAL_CSTRING, name,    // Font name
            CSELF, self)               // GTK self
{
    RexxObjectPtr retc;

    if (argumentExists(1)) {
        gtk_font_selection_dialog_set_font_name(GTK_FONT_SELECTION_DIALOG(self),
                                                name);
        retc = (RexxObjectPtr)context->Nil();
    }
    else {
        retc = (RexxObjectPtr)context->NewStringFromAsciiz(gtk_font_selection_dialog_get_font_name(GTK_FONT_SELECTION_DIALOG(self)));
    }

    return retc;
}

/**
 * Method:  set_preview_text
 *
 * Set the preview text in the dialog.
 *
 * @param text    The preview text
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxFontSelectionDialogSetPreviewText, // Object_method name
            CSTRING, text,             // Preview text
            CSELF, self)               // GTK self
{
    gtk_font_selection_dialog_set_preview_text(GTK_FONT_SELECTION_DIALOG(self),
                                               text);

    return 0;
}

/**
 * Method:  init
 *
 * Create an about dialog.
 *
 * @return        Zero.
 **/
RexxMethod1(int,                       // Return type
            GrxAboutDialogNew,         // Object_method name
            OSELF, self)               // Self
{
    GtkWidget *myWidget = gtk_about_dialog_new();
    context->SendMessage1(self, "set_cself", context->NewPointer(myWidget));
    g_object_set_data(G_OBJECT(myWidget), "OORXOBJECT", self);

    return 0;
}

/**
 * Method:  set_name
 *
 * Set the about program name.
 *
 * @param text    The program name
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxAboutDialogSetName,     // Object_method name
            CSTRING, name,             // Program name
            CSELF, self)               // GTK self
{
#if GTK_CHECK_VERSION(2,12,0)
    gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(self), name);
#else
    gtk_about_dialog_set_name(GTK_ABOUT_DIALOG(self), name);
#endif // GTK_CHECK_VERSION

    return 0;
}

/**
 * Method:  set_version
 *
 * Set the about program version.
 *
 * @param text    The program version
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxAboutDialogSetVersion,  // Object_method name
            CSTRING, version,          // Program version
            CSELF, self)               // GTK self
{
    gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(self), version);

    return 0;
}

/**
 * Method:  set_copyright
 *
 * Set the about program copyright notice.
 *
 * @param text    The copyright notice
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxAboutDialogSetCopyright, // Object_method name
            CSTRING, copyright,        // Program copyright text
            CSELF, self)               // GTK self
{
    gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(self), copyright);

    return 0;
}

/**
 * Method:  set_comments
 *
 * Set the about program comments.
 *
 * @param text    The comments
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxAboutDialogSetComments, // Object_method name
            CSTRING, comments,         // Program comments
            CSELF, self)               // GTK self
{
    gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(self), comments);

    return 0;
}

/**
 * Method:  set_license
 *
 * Set the about program comments.
 *
 * @param text    The comments
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxAboutDialogSetLicense,  // Object_method name
            CSTRING, license,          // Program license
            CSELF, self)               // GTK self
{
    gtk_about_dialog_set_license(GTK_ABOUT_DIALOG(self), license);

    return 0;
}

/**
 * Method:  set_wrap_license
 *
 * Set the boolean license wrap flag.
 *
 * @param flag    The wrap flag
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxAboutDialogSetWrapLicense, // Object_method name
            logical_t, flag,           // Wrap boolean
            CSELF, self)               // GTK self
{
    gtk_about_dialog_set_wrap_license(GTK_ABOUT_DIALOG(self), flag);

    return 0;
}

/**
 * Method:  set_website
 *
 * Set the about program website.
 *
 * @param text    The website URL
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxAboutDialogSetWebsite,  // Object_method name
            CSTRING, website,          // Program website
            CSELF, self)               // GTK self
{
    gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(self), website);

    return 0;
}

/**
 * Method:  set_website_label
 *
 * Set the about program website label.
 *
 * @param text    The website label
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxAboutDialogSetWebsiteLabel, // Object_method name
            CSTRING, label,            // Program website label
            CSELF, self)               // GTK self
{
    gtk_about_dialog_set_website_label(GTK_ABOUT_DIALOG(self), label);

    return 0;
}

/**
 * Method:  set_authors
 *
 * Set the about program authors.
 *
 * @param args    The array of authors
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxAboutDialogSetAuthors,  // Object_method name
            ARGLIST, args,             // Array of authors
            CSELF, self)               // GTK self
{
    size_t members = context->ArraySize(args);

    if (members) {
        const gchar **names = (const char **)malloc(sizeof(const char *) * (members +1));
        const gchar **allnames = names;
        if (!names) {
            context->RaiseException0(Rexx_Error_System_resources);
            return 0;
        }

        for (int i = 1; i <= members; i++, names++) {
            if (!context->IsOfType(context->ArrayAt(args, i), "String")) {
                context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                         context->WholeNumberToObject(i),
                                         context->NewStringFromAsciiz("String"));
                return 0;
            }
            *names = context->ObjectToStringValue(context->ArrayAt(args, i));
        }
        *names = NULL;
        gtk_about_dialog_set_authors(GTK_ABOUT_DIALOG(self), allnames);
    }

    return 0;
}

/**
 * Method:  set_artists
 *
 * Set the about program artists.
 *
 * @param args    The array of artists
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxAboutDialogSetArtists,  // Object_method name
            ARGLIST, args,             // Array of artists
            CSELF, self)               // GTK self
{
    size_t members = context->ArraySize(args);

    if (members) {
        const gchar **names = (const char **)malloc(sizeof(const char *) * (members +1));
        const gchar **allnames = names;
        if (!names) {
            context->RaiseException0(Rexx_Error_System_resources);
            return 0;
        }

        for (int i = 1; i <= members; i++, names++) {
            if (!context->IsOfType(context->ArrayAt(args, i), "String")) {
                context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                         context->WholeNumberToObject(i),
                                         context->NewStringFromAsciiz("String"));
                return 0;
            }
            *names = context->ObjectToStringValue(context->ArrayAt(args, i));
        }
        *names = NULL;
        gtk_about_dialog_set_artists(GTK_ABOUT_DIALOG(self), allnames);
    }

    return 0;
}

/**
 * Method:  set_documentors
 *
 * Set the about program documentors.
 *
 * Set the about program artists.
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxAboutDialogSetDocumentors, // Object_method name
            ARGLIST, args,             // Array of authors
            CSELF, self)               // GTK self
{
    size_t members = context->ArraySize(args);

    if (members) {
        const gchar **names = (const char **)malloc(sizeof(const char *) * members);
        if (!names) {
            context->RaiseException0(Rexx_Error_System_resources);
            return 0;
        }

        for (int i = 0; i < members; i++, names++) {
            if (!context->IsOfType(context->ArrayAt(args, i), "String")) {
                context->RaiseException2(Rexx_Error_Incorrect_method_noclass,
                                         context->WholeNumberToObject(i),
                                         context->NewStringFromAsciiz("String"));
                return 0;
            }
            *names = context->ObjectToStringValue(context->ArrayAt(args, i + 1));
        }
        *(++names) = NULL;
        gtk_about_dialog_set_documenters(GTK_ABOUT_DIALOG(self), names);
    }

    return 0;
}

/**
 * Method:  set_logo
 *
 * Set the about program logo.
 *
 * @param logo    The logo filename
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxAboutDialogSetLogo,     // Object_method name
            CSTRING, logofile,         // Program logo filename
            CSELF, self)               // GTK self
{
    GdkPixbuf *logo;

    logo = gdk_pixbuf_new_from_file(logofile, NULL);
    gtk_about_dialog_set_logo(GTK_ABOUT_DIALOG(self), logo);

    return 0;
}

/**
 * Method:  init
 *
 * Create a color selection dialog
 *
 * @param title   The dialog title
 *
 * @return        Zero.
 **/
RexxMethod2(int,                       // Return type
            GrxColorSelectionDialogNew, // Object_method name
            CSTRING, title,            // Dialog title
            OSELF, self)               // Self
{
    GtkWidget *myWidget = gtk_color_selection_dialog_new(title);
#if GTK_CHECK_VERSION(2,14,0)
    GtkWidget *colorsel = gtk_color_selection_dialog_get_color_selection(GTK_COLOR_SELECTION_DIALOG(myWidget));
#else
    GtkWidget *colorsel = GTK_COLOR_SELECTION_DIALOG(myWidget)->colorsel;
#endif // GTK_CHECK_VERSION
    gtk_color_selection_set_has_opacity_control(GTK_COLOR_SELECTION(colorsel), TRUE);

    // Save ourself
    context->SendMessage1(self, "set_cself", context->NewPointer(myWidget));
    g_object_set_data(G_OBJECT(myWidget), "OORXOBJECT", self);

    return 0;
}

/**
 * Method:  get_color
 *
 * Get the selected color.
 *
 * @return        Color
 **/
RexxMethod1(RexxObjectPtr,             // Return type
            GrxColorSelectionDialogGetColor, // Object_method name
            CSELF, self)               // GTK self
{
    GtkWidget *myWidget = (GtkWidget *)self;
    GtkWidget *colorsel;
    GdkColor color;
    guint alpha;
    char colorstr[256];

#if GTK_CHECK_VERSION(2,14,0)
    colorsel = gtk_color_selection_dialog_get_color_selection(GTK_COLOR_SELECTION_DIALOG(self));
#else
    colorsel = GTK_COLOR_SELECTION_DIALOG(self)->colorsel;
#endif // GTK_CHECK_VERSION
    gtk_color_selection_get_current_color(GTK_COLOR_SELECTION(colorsel), &color);
    alpha = gtk_color_selection_get_current_alpha(GTK_COLOR_SELECTION(colorsel));

    /* Set up the REXX return code */
    g_snprintf(colorstr, sizeof(colorstr), "#%04X%04X%04X%04X",
               color.red, color.green, color.blue, alpha);
    return context->NewStringFromAsciiz(colorstr);
}

