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
    RexxObjectPtr rxarg1 = context->NumberToObject((wholenumber_t)arg1);;
    context->SendMessage1(rxobj, ((cbcb *)data)->signal_name, rxarg1);
    context->DetachThread();
    return;
}


/*============================================================================*/
/* Public Functions                                                           */
/*============================================================================*/

/*
 * Method:  init
 *
 * Create a dialog.
 *
 * @return        Zero.
 */
RexxMethod1(int,                       // Return type
            GrxDialogNew,              // Object_method name
            OSELF, self)               // Self
{
    GtkWidget *myWidget;

    myWidget = gtk_dialog_new();
    context->SetObjectVariable("!POINTER", context->NewPointer(myWidget));
    g_object_set_data(G_OBJECT(myWidget), "OORXOBJECT", self);

    return 0;
}

/*
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
 * @param btext1  The button text
 *
 * @param bid1    The response id 
 *
 * @param btext2  The button text
 *
 * @param bid2    The response id 
 *
 * @param btext3  The button text
 *
 * @param bid3    The response id 
 *
 * @return        Zero.
 */
RexxMethod9(int,                       // Return type
            GrxDialogNewWithButtons,   // Object_method name
            CSTRING, title,            // Dialog title
            RexxObjectPtr, parent,     // Parent window
            int, flags,                // Dialog flags
            CSTRING, btext1,           // Button text 
            int, bid1,                 // Response id 
            OPTIONAL_CSTRING, btext2,  // Button text 
            OPTIONAL_int, bid2,        // Response id 
            OPTIONAL_CSTRING, btext3,  // Button text 
            OPTIONAL_int, bid3)        // Response id 
{
    RexxPointerObject parentptr = (RexxPointerObject)context->SendMessage0(parent, "POINTER");
    GtkWindow *myParent = (GtkWindow *)context->PointerValue(parentptr);
    GtkWidget *myWidget, *vbox;

    if (btext3 != NULL) {
        myWidget = gtk_dialog_new_with_buttons(title, myParent, (GtkDialogFlags)flags,
                                               btext1, bid1, btext2, bid2,
                                               btext3, bid3, NULL);
    }
    else if (btext2 != NULL) {
        myWidget = gtk_dialog_new_with_buttons(title, myParent, (GtkDialogFlags)flags,
                                               btext1, bid1, btext2, bid2,
                                               NULL);
    }
    else {
        myWidget = gtk_dialog_new_with_buttons(title, myParent, (GtkDialogFlags)flags,
                                               btext1, bid1, NULL);
    }
    context->SetObjectVariable("!POINTER", context->NewPointer(myWidget));
    g_object_set_data(G_OBJECT(myWidget), "OORXOBJECT", context->GetSelf());
    vbox = GTK_DIALOG(myWidget)->vbox;
    RexxObjectPtr rxvbox = (RexxObjectPtr)g_object_get_data(G_OBJECT(vbox), "OORXOBJECT");
    if (rxvbox == NULL) {
        // no Rexx Object found so create it
        RexxClassObject cobj = context->FindClass("GtkVBox");
        rxvbox = context->SendMessage1(cobj, "NEW", context->NewPointer(vbox));
    }

    return 0;
}

/*
 * Method:  add_button
 *
 * Add buttons to a dialog.
 *
 * @param btext1  The button text
 *
 * @param bid1    The response id 
 *
 * @return        Zero.
 */
RexxMethod2(int,                       // Return type
            GrxDialogAddButton,        // Object_method name
            CSTRING, btext,            // Button text 
            int, bid)                  // Response id 
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkDialog *myWidget = (GtkDialog *)context->PointerValue(rxptr);

    gtk_dialog_add_button(myWidget, btext, bid);

    return 0;
}

/*
 * Method:  set_has_separator
 *
 * Add/remove the button separator.
 *
 * @param flag    The separator flag.
 *
 * @return        Zero.
 */
RexxMethod1(int,                       // Return type
            GrxDialogSetHasSeparator,  // Object_method name
            logical_t, flag)           // Separator boolean
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkDialog *myWidget = (GtkDialog *)context->PointerValue(rxptr);

    gtk_dialog_set_has_separator(myWidget, flag);

    return 0;
}

/*
 * Method:  set_default_response
 *
 * Set the default response id.    
 *
 * @param rid     Response id
 *
 * @return        Zero.
 */
RexxMethod1(int,                       // Return type
            GrxDialogSetDefaultResponse, // Object_method name
            int, rid)                  // Response id
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkDialog *myWidget = (GtkDialog *)context->PointerValue(rxptr);

    gtk_dialog_set_default_response(myWidget, rid);

    return 0;
}

/*
 * Method:  dialog_run
 *
 * Run a dialog.
 *
 * @return        Response id
 */
RexxMethod0(int,                       // Return type
            GrxDialogRun)              // Object_method name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkDialog *myWidget = (GtkDialog *)context->PointerValue(rxptr);
    gint response;

    response = gtk_dialog_run(myWidget);

    return response;
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
RexxMethod1(RexxObjectPtr,             // Return type
            GrxDialogSignalConnect,    // Object_method name
            CSTRING, name)             // Signal name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);
    cbcb *cblock;

    if (strcmp(name, "close") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_close";
        g_signal_connect(G_OBJECT(myWidget), "close",
                         G_CALLBACK(signal_func_0), cblock);
        return context->True();
    }
    else if (strcmp(name, "response") == 0) {
        cblock = (cbcb *)malloc(sizeof(cbcb));
        cblock->instance = context->threadContext->instance;
        cblock->signal_name = "signal_response";
        g_signal_connect(G_OBJECT(myWidget), "response",
                         G_CALLBACK(signal_func_1), cblock);
        return context->True();
    }
    RexxObjectPtr parent = context->GetSuper();
    return context->SendMessage0(parent, name);
}

/*
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
 */
RexxMethod5(int,                       // Return type
            GrxMessageDialogNew,       // Object_method name
            RexxObjectPtr, parent,     // Parent window
            int, flags,                // Dialog flags
            int, type,                 // Message type
            int, bset,                 // Button set
            CSTRING, text)             // Message text
{
    RexxPointerObject parentptr = (RexxPointerObject)context->SendMessage0(parent, "POINTER");
    GtkWindow *myParent = (GtkWindow *)context->PointerValue(parentptr);
    GtkWidget *myWidget;


    myWidget = gtk_message_dialog_new(myParent, (GtkDialogFlags)flags,
                                      (GtkMessageType)type,
                                      (GtkButtonsType)bset, text);

    context->SetObjectVariable("!POINTER", context->NewPointer(myWidget));
    g_object_set_data(G_OBJECT(myWidget), "OORXOBJECT", context->GetSelf());

    return 0;
}

/*
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
 * @param bset    The button set  
 *
 * @param text    The message text
 *
 * @return        Zero.
 */
RexxMethod9(int,                       // Return type
            GrxFileChooserDialogNew,   // Object_method name
            CSTRING, title,            // Title
            RexxObjectPtr, parent,     // Parent window
            int, action,               // Dialog action type
            CSTRING, sid1,             // Stock button id
            int, rid1,                 // Response idt
            OPTIONAL_CSTRING, sid2,    // Stock button id
            OPTIONAL_int, rid2,        // Response idt
            OPTIONAL_CSTRING, sid3,    // Stock button id
            OPTIONAL_int, rid3)        // Response idt
{
    RexxPointerObject parentptr = (RexxPointerObject)context->SendMessage0(parent, "POINTER");
    GtkWindow *myParent = (GtkWindow *)context->PointerValue(parentptr);
    GtkWidget *myWidget;
    gint resptype;

    if (sid3 != NULL) {
        myWidget = gtk_file_chooser_dialog_new(title, myParent,
                                               (GtkFileChooserAction)action,
                                               sid1, rid1, sid2, rid2, sid3, rid3,
                                               NULL);
    }
    else if (sid2 != NULL) {
        myWidget = gtk_file_chooser_dialog_new(title, myParent,
                                               (GtkFileChooserAction)action,
                                               sid1, rid1, sid2, rid2, NULL);
    }
    else {
        myWidget = gtk_file_chooser_dialog_new(title, myParent,
                                               (GtkFileChooserAction)action,
                                               sid1, rid1, NULL);
    }
    context->SetObjectVariable("!POINTER", context->NewPointer(myWidget));
    g_object_set_data(G_OBJECT(myWidget), "OORXOBJECT", context->GetSelf());

    return 0;
}

/*
 * Method:  init
 *
 * @param title   The dialog title
 *
 * Create a font selection dialog.
 *
 * @return        Zero.
 */
RexxMethod1(int,                       // Return type
            GrxFontSelectionDialogNew, // Object_method name
            CSTRING, title)            // Dialog title
{
    GtkWidget *myWidget;

    myWidget = gtk_font_selection_dialog_new(title);
    context->SetObjectVariable("!POINTER", context->NewPointer(myWidget));
    g_object_set_data(G_OBJECT(myWidget), "OORXOBJECT", context->GetSelf());

    return 0;
}

/*
 * Method:  set_font_name
 *
 * Set the font in the dialog.
 *
 * @param name    The font name
 *
 * @return        Zero.
 */
RexxMethod1(int,                       // Return type
            GrxFontSelectionDialogSetFontName, // Object_method name
            CSTRING, name)             // Font name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);

    gtk_font_selection_dialog_set_font_name(GTK_FONT_SELECTION_DIALOG(myWidget),
                                            name);

    return 0;
}

/*
 * Method:  get_font_name
 *
 * Get the selected font name in the dialog.
 *
 * @return        Font name
 */
RexxMethod0(CSTRING,                   // Return type
            GrxFontSelectionDialogGetFontName) // Object_method name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);


    return gtk_font_selection_dialog_get_font_name(GTK_FONT_SELECTION_DIALOG(myWidget));
}

/*
 * Method:  set_preview_text
 *
 * Set the preview text in the dialog.
 *
 * @param text    The preview text
 *
 * @return        Zero.
 */
RexxMethod1(int,                       // Return type
            GrxFontSelectionDialogSetPreviewText, // Object_method name
            CSTRING, text)             // Preview text
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);


    gtk_font_selection_dialog_set_preview_text(GTK_FONT_SELECTION_DIALOG(myWidget),
                                               text);

    return 0;
}

/*
 * Method:  init
 *
 * Create an about dialog.
 *
 * @return        Zero.
 */
RexxMethod0(int,                       // Return type
            GrxAboutDialogNew)         // Object_method name
{
    GtkWidget *myWidget;

    myWidget = gtk_about_dialog_new();
    context->SetObjectVariable("!POINTER", context->NewPointer(myWidget));
    g_object_set_data(G_OBJECT(myWidget), "OORXOBJECT", context->GetSelf());

    return 0;
}

/*
 * Method:  set_name
 *
 * Set the about program name.
 *
 * @param text    The program name
 *
 * @return        Zero.
 */
RexxMethod1(int,                       // Return type
            GrxAboutDialogSetName,     // Object_method name
            CSTRING, name)             // Program name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);

    gtk_about_dialog_set_name(GTK_ABOUT_DIALOG(myWidget), name);

    return 0;
}

/*
 * Method:  set_version
 *
 * Set the about program version.
 *
 * @param text    The program version
 *
 * @return        Zero.
 */
RexxMethod1(int,                       // Return type
            GrxAboutDialogSetVersion,  // Object_method name
            CSTRING, version)          // Program version
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);

    gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(myWidget), version);

    return 0;
}

/*
 * Method:  set_copyright
 *
 * Set the about program copyright notice.
 *
 * @param text    The copyright notice
 *
 * @return        Zero.
 */
RexxMethod1(int,                       // Return type
            GrxAboutDialogSetCopyright, // Object_method name
            CSTRING, copyright)        // Program copyright text
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);

    gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(myWidget), copyright);

    return 0;
}

/*
 * Method:  set_comments
 *
 * Set the about program comments.
 *
 * @param text    The comments
 *
 * @return        Zero.
 */
RexxMethod1(int,                       // Return type
            GrxAboutDialogSetComments, // Object_method name
            CSTRING, comments)         // Program comments
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);

    gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(myWidget), comments);

    return 0;
}

/*
 * Method:  set_license
 *
 * Set the about program comments.
 *
 * @param text    The comments
 *
 * @return        Zero.
 */
RexxMethod1(int,                       // Return type
            GrxAboutDialogSetLicense,  // Object_method name
            CSTRING, license)          // Program license
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);

    gtk_about_dialog_set_license(GTK_ABOUT_DIALOG(myWidget), license);

    return 0;
}

/*
 * Method:  set_wrap_license
 *
 * Set the boolean license wrap flag.
 *
 * @param flag    The wrap flag
 *
 * @return        Zero.
 */
RexxMethod1(int,                       // Return type
            GrxAboutDialogSetWrapLicense, // Object_method name
            logical_t, flag)           // Wrap boolean
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);

    gtk_about_dialog_set_wrap_license(GTK_ABOUT_DIALOG(myWidget), flag);

    return 0;
}

/*
 * Method:  set_website
 *
 * Set the about program website.
 *
 * @param text    The website URL
 *
 * @return        Zero.
 */
RexxMethod1(int,                       // Return type
            GrxAboutDialogSetWebsite,  // Object_method name
            CSTRING, website)          // Program website
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);

    gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(myWidget), website);

    return 0;
}

/*
 * Method:  set_website_label
 *
 * Set the about program website label.
 *
 * @param text    The website label
 *
 * @return        Zero.
 */
RexxMethod1(int,                       // Return type
            GrxAboutDialogSetWebsiteLabel, // Object_method name
            CSTRING, label)            // Program website label
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);

    gtk_about_dialog_set_website_label(GTK_ABOUT_DIALOG(myWidget), label);

    return 0;
}

/*
 * Method:  set_authors
 *
 * Set the about program authors.
 *
 * @param author1 The author
 *
 * @param author2 The author
 *
 * @param author3 The author
 *
 * @param author4 The author
 *
 * @param author5 The author
 *
 * @param author6 The author
 *
 * @param author7 The author
 *
 * @param author8 The author
 *
 * @param author9 The author
 *
 * @return        Zero.
 */
RexxMethod9(int,                       // Return type
            GrxAboutDialogSetAuthors,  // Object_method name
            CSTRING, author1,          // Program author
            OPTIONAL_CSTRING, author2, // Program author
            OPTIONAL_CSTRING, author3, // Program author
            OPTIONAL_CSTRING, author4, // Program author
            OPTIONAL_CSTRING, author5, // Program author
            OPTIONAL_CSTRING, author6, // Program author
            OPTIONAL_CSTRING, author7, // Program author
            OPTIONAL_CSTRING, author8, // Program author
            OPTIONAL_CSTRING, author9) // Program author
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);
    const gchar *authors[10];

    authors[9] = NULL;
    if (author9 != NULL) {
        authors[8] = author9;
    }
    else {
        authors[8] = NULL;
    }
    if (author8 != NULL) {
        authors[7] = author8;
    }
    else {
        authors[7] = NULL;
    }
    if (author7 != NULL) {
        authors[6] = author7;
    }
    else {
        authors[6] = NULL;
    }
    if (author6 != NULL) {
        authors[5] = author6;
    }
    else {
        authors[5] = NULL;
    }
    if (author5 != NULL) {
        authors[4] = author5;
    }
    else {
        authors[4] = NULL;
    }
    if (author4 != NULL) {
        authors[3] = author4;
    }
    else {
        authors[3] = NULL;
    }
    if (author3 != NULL) {
        authors[2] = author3;
    }
    else {
        authors[2] = NULL;
    }
    if (author2 != NULL) {
        authors[1] = author2;
    }
    else {
        authors[1] = NULL;
    }
    authors[0] = author1;
    gtk_about_dialog_set_authors(GTK_ABOUT_DIALOG(myWidget), authors);

    return 0;
}

/*
 * Method:  set_artists
 *
 * Set the about program artists.
 *
 * @param artist1 The artist
 *
 * @param artist2 The artist
 *
 * @param artist3 The artist
 *
 * @param artist4 The artist
 *
 * @param artist5 The artist
 *
 * @param artist6 The artist
 *
 * @param artist7 The artist
 *
 * @param artist8 The artist
 *
 * @param artist9 The artist
 *
 * @return        Zero.
 */
RexxMethod9(int,                       // Return type
            GrxAboutDialogSetArtists,  // Object_method name
            CSTRING, artist1,          // Program artist
            OPTIONAL_CSTRING, artist2, // Program artist
            OPTIONAL_CSTRING, artist3, // Program artist
            OPTIONAL_CSTRING, artist4, // Program artist
            OPTIONAL_CSTRING, artist5, // Program artist
            OPTIONAL_CSTRING, artist6, // Program artist
            OPTIONAL_CSTRING, artist7, // Program artist
            OPTIONAL_CSTRING, artist8, // Program artist
            OPTIONAL_CSTRING, artist9) // Program artist
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);
    const gchar *artists[10];

    artists[9] = NULL;
    if (artist9 != NULL) {
        artists[8] = artist9;
    }
    else {
        artists[8] = NULL;
    }
    if (artist8 != NULL) {
        artists[7] = artist8;
    }
    else {
        artists[7] = NULL;
    }
    if (artist7 != NULL) {
        artists[6] = artist7;
    }
    else {
        artists[6] = NULL;
    }
    if (artist6 != NULL) {
        artists[5] = artist6;
    }
    else {
        artists[5] = NULL;
    }
    if (artist5 != NULL) {
        artists[4] = artist5;
    }
    else {
        artists[4] = NULL;
    }
    if (artist4 != NULL) {
        artists[3] = artist4;
    }
    else {
        artists[3] = NULL;
    }
    if (artist3 != NULL) {
        artists[2] = artist3;
    }
    else {
        artists[2] = NULL;
    }
    if (artist2 != NULL) {
        artists[1] = artist2;
    }
    else {
        artists[1] = NULL;
    }
    artists[0] = artist1;
    gtk_about_dialog_set_artists(GTK_ABOUT_DIALOG(myWidget), artists);

    return 0;
}

/*
 * Method:  set_documentors
 *
 * Set the about program artists.
 *
 * @param author1 The author
 *
 * @param author2 The author
 *
 * @param author3 The author
 *
 * @param author4 The author
 *
 * @param author5 The author
 *
 * @param author6 The author
 *
 * @param author7 The author
 *
 * @param author8 The author
 *
 * @param author9 The author
 *
 * @return        Zero.
 */
RexxMethod9(int,                       // Return type
            GrxAboutDialogSetDocumentors, // Object_method name
            CSTRING, author1,          // Program artist
            OPTIONAL_CSTRING, author2, // Program author
            OPTIONAL_CSTRING, author3, // Program author
            OPTIONAL_CSTRING, author4, // Program author
            OPTIONAL_CSTRING, author5, // Program author
            OPTIONAL_CSTRING, author6, // Program author
            OPTIONAL_CSTRING, author7, // Program author
            OPTIONAL_CSTRING, author8, // Program author
            OPTIONAL_CSTRING, author9) // Program author
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);
    const gchar *authors[10];

    authors[9] = NULL;
    if (author9 != NULL) {
        authors[8] = author9;
    }
    else {
        authors[8] = NULL;
    }
    if (author8 != NULL) {
        authors[7] = author8;
    }
    else {
        authors[7] = NULL;
    }
    if (author7 != NULL) {
        authors[6] = author7;
    }
    else {
        authors[6] = NULL;
    }
    if (author6 != NULL) {
        authors[5] = author6;
    }
    else {
        authors[5] = NULL;
    }
    if (author5 != NULL) {
        authors[4] = author5;
    }
    else {
        authors[4] = NULL;
    }
    if (author4 != NULL) {
        authors[3] = author4;
    }
    else {
        authors[3] = NULL;
    }
    if (author3 != NULL) {
        authors[2] = author3;
    }
    else {
        authors[2] = NULL;
    }
    if (author2 != NULL) {
        authors[1] = author2;
    }
    else {
        authors[1] = NULL;
    }
    authors[0] = author1;
    gtk_about_dialog_set_documenters(GTK_ABOUT_DIALOG(myWidget), authors);

    return 0;
}

/*
 * Method:  set_logo
 *
 * Set the about program logo.
 *
 * @param logo    The logo filename
 *
 * @return        Zero.
 */
RexxMethod1(int,                       // Return type
            GrxAboutDialogSetLogo,     // Object_method name
            CSTRING, logofile)         // Program logo filename
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);
    GdkPixbuf *logo;

    logo = gdk_pixbuf_new_from_file(logofile, NULL);
    gtk_about_dialog_set_logo(GTK_ABOUT_DIALOG(myWidget), logo);

    return 0;
}

/*
 * Method:  init
 *
 * Create a color selection dialog
 *
 * @param title   The dialog title
 *
 * @return        Zero.
 */
RexxMethod1(int,                       // Return type
            GrxColorSelectionDialogNew, // Object_method name
            CSTRING, title)            // Dialog title
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);
    GtkWidget *colorsel;

    myWidget = gtk_color_selection_dialog_new(title);
    colorsel = GTK_COLOR_SELECTION_DIALOG(myWidget)->colorsel;
    gtk_color_selection_set_has_opacity_control(GTK_COLOR_SELECTION(colorsel), TRUE);
    context->SetObjectVariable("!POINTER", context->NewPointer(myWidget));
    g_object_set_data(G_OBJECT(myWidget), "OORXOBJECT", context->GetSelf());

    return 0;
}

/*
 * Method:  get_color
 *
 * Get the selected color.
 *
 * @return        Color
 */
RexxMethod0(RexxObjectPtr,             // Return type
            GrxColorSelectionDialogGetColor) // Object_method name
{
    RexxPointerObject rxptr = (RexxPointerObject)context->GetObjectVariable("!POINTER");
    GtkWidget *myWidget = (GtkWidget *)context->PointerValue(rxptr);
    GtkWidget *colorsel;
    GdkColor color;
    guint alpha;
    char colorstr[256];

    colorsel = GTK_COLOR_SELECTION_DIALOG(myWidget)->colorsel;
    gtk_color_selection_get_current_color(GTK_COLOR_SELECTION(colorsel), &color);
    alpha = gtk_color_selection_get_current_alpha(GTK_COLOR_SELECTION(colorsel));

    /* Set up the REXX return code */
    g_snprintf(colorstr, sizeof(colorstr), "#%04X%04X%04X%04X",
               color.red, color.green, color.blue, alpha);
    return context->NewStringFromAsciiz(colorstr);
}

