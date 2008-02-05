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
                          gpointer data)
{
    char buffer[256];
    RXSTRING entry;

    // set up the queue entry data
    g_snprintf(buffer, sizeof(buffer), "%p %s", window, data);
    entry.strptr = buffer;
    entry.strlength = strlen(buffer);

    // insert the signal event here
    RexxAddQueue(GrxGetRexxQueueName(), &entry, RXQUEUE_FIFO);

    return;
}


static void signal_func_2(GtkWidget *window,
                          gint arg1,
                          gpointer data)
{
    char buffer[256];
    RXSTRING entry;

    // set up the queue entry data
    g_snprintf(buffer, sizeof(buffer), "%p %s %d", window, data, arg1);
    entry.strptr = buffer;
    entry.strlength = strlen(buffer);

    // insert the signal event here
    RexxAddQueue(GrxGetRexxQueueName(), &entry, RXQUEUE_FIFO);

    return;
}


static gint str2resp(const char * str)
{
    gint resptype;

    if (!strcmp(str, "GTK_RESPONSE_NONE"))
        return GTK_RESPONSE_NONE;
    else if (!strcmp(str, "GTK_RESPONSE_REJECT"))
        return GTK_RESPONSE_REJECT;
    else if (!strcmp(str, "GTK_RESPONSE_ACCEPT"))
        return GTK_RESPONSE_ACCEPT;
    else if (!strcmp(str, "GTK_RESPONSE_DELETE_EVENT"))
        return GTK_RESPONSE_DELETE_EVENT;
    else if (!strcmp(str, "GTK_RESPONSE_OK"))
        return GTK_RESPONSE_OK;
    else if (!strcmp(str, "GTK_RESPONSE_CANCEL"))
        return GTK_RESPONSE_CANCEL;
    else if (!strcmp(str, "GTK_RESPONSE_CLOSE"))
        return GTK_RESPONSE_CLOSE;
    else if (!strcmp(str, "GTK_RESPONSE_YES"))
        return GTK_RESPONSE_YES;
    else if (!strcmp(str, "GTK_RESPONSE_NO"))
        return GTK_RESPONSE_NO;
    else if (!strcmp(str, "GTK_RESPONSE_APPLY"))
        return GTK_RESPONSE_APPLY;
    else if (!strcmp(str, "GTK_RESPONSE_HELP"))
        return GTK_RESPONSE_HELP;
    else
        sscanf(str, "%d", &resptype);
    return resptype;
}


/*============================================================================*/
/* Public Functions                                                           */
/*============================================================================*/


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxDialogNew                                       */
/* Description: Create a new dialog                                           */
/* Rexx Args:   None                                                          */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxDialogNew(const char * Name,
                             const size_t Argc, const RXSTRING Argv[],
                             const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;

    /* Check for valid arguments */
    if (GrxCheckArgs(0, Argc, Argv)) {
        return RXFUNC_BADCALL;
    }

    myWidget = gtk_dialog_new();

    /* Set up the REXX return code */
    g_snprintf(Retstr->strptr, RXAUTOBUFLEN, "%p %p", myWidget, GTK_DIALOG(myWidget)->vbox);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxDialogNewWithButtons                            */
/* Description: Create a new dialog with buttons                              */
/* Rexx Args:   Title                                                         */
/*              Parent window                                                 */
/*              Flags                                                         */
/*              Button text                                                   */
/*              Response id                                                   */
/*              ...                                                           */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxDialogNewWithButtons(const char * Name,
                             const size_t Argc, const RXSTRING Argv[],
                             const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget, *myParent;
    GtkDialogFlags flags;
    gint resptype;
    size_t i = 5;

    /* Check for valid arguments */
    if (Argc < 5) {
        return RXFUNC_BADCALL;
    }
    if (GrxCheckArgs(5, 5, Argv)) {
        return RXFUNC_BADCALL;
    }

    /* Initialize function parameters */
    sscanf(Argv[1].strptr, "%p", &myParent);
    if (!strcmp(Argv[2].strptr, "GTK_DIALOG_MODAL"))
        flags = GTK_DIALOG_MODAL;
    else if (!strcmp(Argv[2].strptr, "GTK_DIALOG_DESTROY_WITH_PARENT"))
        flags = GTK_DIALOG_DESTROY_WITH_PARENT;
    else if (!strcmp(Argv[2].strptr, "GTK_DIALOG_NO_SEPARATOR"))
        flags = GTK_DIALOG_NO_SEPARATOR;
    else
        sscanf(Argv[2].strptr, "%d", &flags);
    resptype = str2resp(Argv[4].strptr);

    myWidget = gtk_dialog_new_with_buttons(Argv[0].strptr,
                                           GTK_WINDOW(myParent),
                                           flags,
                                           Argv[3].strptr,
                                           resptype,
                                           NULL);

    // add the rest of the buttons
    while (i + 1 < Argc) {
        if (!RXVALIDSTRING(Argv[i]) || !RXVALIDSTRING(Argv[i + 1]))
            return RXFUNC_BADCALL;
        resptype = str2resp(Argv[i + 1].strptr);
        gtk_dialog_add_button(GTK_DIALOG(myWidget), Argv[i].strptr, resptype);
        i += 2;
    }

    /* Set up the REXX return code */
    g_snprintf(Retstr->strptr, RXAUTOBUFLEN, "%p %p", myWidget, GTK_DIALOG(myWidget)->vbox);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxDialogAddButton                                 */
/* Description: Add a button to a dialog                                      */
/* Rexx Args:   Pointer to the dialog                                         */
/*              Button text                                                   */
/*              Response id                                                   */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxDialogAddButton(const char * Name,
                             const size_t Argc, const RXSTRING Argv[],
                             const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    gint resptype;

    /* Check for valid arguments */
    if (GrxCheckArgs(3, Argc, Argv))
        return RXFUNC_BADCALL;

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);
    resptype = str2resp(Argv[2].strptr);

    // add the button
    gtk_dialog_add_button(GTK_DIALOG(myWidget), Argv[1].strptr, resptype);

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxDialogSetHasSeparator                           */
/* Description: Toggle the seperator                                          */
/* Rexx Args:   Pointer to the dialog                                         */
/*              Flag                                                          */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxDialogSetHasSeparator(const char * Name,
                             const size_t Argc, const RXSTRING Argv[],
                             const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    gboolean flag;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv))
        return RXFUNC_BADCALL;

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);
    sscanf(Argv[1].strptr, "%d", &flag);

    // add the button
    gtk_dialog_set_has_separator(GTK_DIALOG(myWidget), flag);

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxDialogSetDefaultResponse                        */
/* Description: Toggle the seperator                                          */
/* Rexx Args:   Pointer to the dialog                                         */
/*              Response id                                                   */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxDialogSetDefaultResponse(const char * Name,
                             const size_t Argc, const RXSTRING Argv[],
                             const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    gint resptype;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv))
        return RXFUNC_BADCALL;

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);
    resptype = str2resp(Argv[1].strptr);

    // add the button
    gtk_dialog_set_default_response(GTK_DIALOG(myWidget), resptype);

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxDialogRun                                       */
/* Description: Run the dialog                                                */
/* Rexx Args:   Pointer to the dialog                                         */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxDialogRun(const char * Name,
                             const size_t Argc, const RXSTRING Argv[],
                             const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    gint response, i = 0;
    GrxConstants *resp = GrxResponseType;

    /* Check for valid arguments */
    if (GrxCheckArgs(1, Argc, Argv))
        return RXFUNC_BADCALL;

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);

    // run the dialog
    response = gtk_dialog_run(GTK_DIALOG(myWidget));

    // convert the response
    while (resp[i].name != NULL) {
        if (resp[i].value == response) {
            break;
        }
        i++;
    }

    /* Set up the Rexx return code */
    if (resp == NULL)
        g_snprintf(Retstr->strptr, RXAUTOBUFLEN, "%d", response);
    else
        g_snprintf(Retstr->strptr, RXAUTOBUFLEN, "%s", resp[i].name);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxDialogConnectSignal                             */
/* Description: Connect a signal function to the Widget                       */
/* Rexx Args:   Pointer to the widget                                         */
/*              Signal name                                                   */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxDialogConnectSignal(const char * Name,
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
        if (strcmp(Argv[1].strptr, "close") == 0) {
            g_signal_connect(G_OBJECT(myWidget), "close",
                             G_CALLBACK(signal_func_1), "signal_close");
        }
        else if (strcmp(Argv[1].strptr, "response") == 0) {
            g_signal_connect(G_OBJECT(myWidget), "response",
                             G_CALLBACK(signal_func_2), "signal_response");
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


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxMessageDialogNew                                */
/* Description: Create a message dialog                                       */
/* Rexx Args:   Parent window pointer                                         */
/*              Flags                                                         */
/*              Message type                                                  */
/*              Button set                                                    */
/*              Message text                                                  */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxMessageDialogNew(const char * Name,
                                    const size_t Argc, const RXSTRING Argv[],
                                    const char * Queuename, PRXSTRING Retstr)
{
    GtkWindow *parentWindow;
    GtkWidget *myWidget;
    gint flags;
    GtkMessageType type;
    GtkButtonsType btype;

    /* Check for valid arguments */
    if (GrxCheckArgs(5, Argc, Argv))
        return RXFUNC_BADCALL;

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &parentWindow);
    if (!strcmp(Argv[1].strptr, "GTK_DIALOG_MODAL"))
        flags = GTK_DIALOG_MODAL;
    else if (!strcmp(Argv[1].strptr, "GTK_DIALOG_DESTROY_WITH_PARENT"))
        flags = GTK_DIALOG_DESTROY_WITH_PARENT;
    else if (!strcmp(Argv[1].strptr, "GTK_DIALOG_NO_SEPARATOR"))
        flags = GTK_DIALOG_NO_SEPARATOR;
    else
        sscanf(Argv[1].strptr, "%d", &flags);
    if (!strcmp(Argv[2].strptr, "GTK_MESSAGE_INFO"))
        type = GTK_MESSAGE_INFO;
    else if (!strcmp(Argv[2].strptr, "GTK_MESSAGE_WARNING"))
        type = GTK_MESSAGE_WARNING;
    else if (!strcmp(Argv[2].strptr, "GTK_MESSAGE_QUESTION"))
        type = GTK_MESSAGE_QUESTION;
    else if (!strcmp(Argv[2].strptr, "GTK_MESSAGE_ERROR"))
        type = GTK_MESSAGE_ERROR;
    else if (!strcmp(Argv[2].strptr, "GTK_MESSAGE_OTHER"))
        type = GTK_MESSAGE_OTHER;
    else
        sscanf(Argv[2].strptr, "%d", &type);
    if (!strcmp(Argv[3].strptr, "GTK_BUTTONS_NONE"))
        btype = GTK_BUTTONS_NONE;
    else if (!strcmp(Argv[3].strptr, "GTK_BUTTONS_OK"))
        btype = GTK_BUTTONS_OK;
    else if (!strcmp(Argv[3].strptr, "GTK_BUTTONS_CLOSE"))
        btype = GTK_BUTTONS_CLOSE;
    else if (!strcmp(Argv[3].strptr, "GTK_BUTTONS_CANCEL"))
        btype = GTK_BUTTONS_CANCEL;
    else if (!strcmp(Argv[3].strptr, "GTK_BUTTONS_YES_NO"))
        btype = GTK_BUTTONS_YES_NO;
    else if (!strcmp(Argv[3].strptr, "GTK_BUTTONS_OK_CANCEL"))
        btype = GTK_BUTTONS_OK_CANCEL;
    else
        sscanf(Argv[3].strptr, "%d", &btype);

    myWidget = gtk_message_dialog_new(parentWindow, flags,
                                      type, btype, Argv[4].strptr);

    /* Set up the REXX return code */
    g_snprintf(Retstr->strptr, RXAUTOBUFLEN, "%p %p", myWidget, GTK_DIALOG(myWidget)->vbox);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxFileChooserDialogNew                            */
/* Description: Create a file chooser dialog                                  */
/* Rexx Args:   Title                                                         */
/*          :   Parent window pointer                                         */
/*              Message type                                                  */
/*              Button set                                                    */
/*              Message text                                                  */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxFileChooserDialogNew(const char * Name,
                                        const size_t Argc, const RXSTRING Argv[],
                                        const char * Queuename, PRXSTRING Retstr)
{
    GtkWindow *parentWindow;
    GtkWidget *myWidget;
    GtkFileChooserAction action;
    gint resptype;

    /* Check for valid arguments */
    if (GrxCheckArgs(5, Argc, Argv))
        return RXFUNC_BADCALL;

    /* Initialize function parameters */
    sscanf(Argv[1].strptr, "%p", &parentWindow);
    if (!strcmp(Argv[2].strptr, "GTK_FILE_CHOOSER_ACTION_OPEN"))
        action = GTK_FILE_CHOOSER_ACTION_OPEN;
    else if (!strcmp(Argv[2].strptr, "GTK_FILE_CHOOSER_ACTION_SAVE"))
        action = GTK_FILE_CHOOSER_ACTION_SAVE;
    else if (!strcmp(Argv[2].strptr, "GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER"))
        action = GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER;
    else if (!strcmp(Argv[2].strptr, "GTK_FILE_CHOOSER_ACTION_CREATE_FOLDER"))
        action = GTK_FILE_CHOOSER_ACTION_CREATE_FOLDER;
    else
        sscanf(Argv[2].strptr, "%d", &action);
    resptype = str2resp(Argv[4].strptr);

    myWidget = gtk_file_chooser_dialog_new(Argv[0].strptr, parentWindow,
                                           action, Argv[3].strptr,
                                           resptype, NULL);

    /* Set up the REXX return code */
    g_snprintf(Retstr->strptr, RXAUTOBUFLEN, "%p", myWidget);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxFontSelectionDialogNew                          */
/* Description: Create a font selection dialog                                */
/* Rexx Args:   None                                                          */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxFontSelectionDialogNew(const char * Name,
                                          const size_t Argc, const RXSTRING Argv[],
                                          const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;

    /* Check for valid arguments */
    if (GrxCheckArgs(1, Argc, Argv))
        return RXFUNC_BADCALL;

    myWidget = gtk_font_selection_dialog_new(Argv[0].strptr);

    /* Set up the REXX return code */
    g_snprintf(Retstr->strptr, RXAUTOBUFLEN, "%p", myWidget);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxFontSelectionDialogSetFontName                  */
/* Description: Set the dialog font name                                      */
/* Rexx Args:   Pointer to the dialog                                         */
/*              Font name                                                     */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxFontSelectionDialogSetFontName(const char * Name,
                                                  const size_t Argc, const RXSTRING Argv[],
                                                  const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv))
        return RXFUNC_BADCALL;

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);

    gtk_font_selection_dialog_set_font_name(GTK_FONT_SELECTION_DIALOG(myWidget),
                                            Argv[1].strptr);

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxFontSelectionDialogGetFontName                  */
/* Description: Get the dialog font name                                      */
/* Rexx Args:   Pointer to the dialog                                         */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxFontSelectionDialogGetFontName(const char * Name,
                                                  const size_t Argc, const RXSTRING Argv[],
                                                  const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    gchar * font;

    /* Check for valid arguments */
    if (GrxCheckArgs(1, Argc, Argv))
        return RXFUNC_BADCALL;

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);

    font = gtk_font_selection_dialog_get_font_name(GTK_FONT_SELECTION_DIALOG(myWidget));

    /* Set up the REXX return code */
    g_snprintf(Retstr->strptr, RXAUTOBUFLEN, "%s", font);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxFontSelectionDialogSetPreviewText               */
/* Description: Set the dialog preview text                                   */
/* Rexx Args:   Pointer to the dialog                                         */
/*              Font name                                                     */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxFontSelectionDialogSetPreviewText(const char * Name,
                                                     const size_t Argc, const RXSTRING Argv[],
                                                     const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv))
        return RXFUNC_BADCALL;

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);

    gtk_font_selection_dialog_set_preview_text(GTK_FONT_SELECTION_DIALOG(myWidget),
                                               Argv[1].strptr);

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxAboutDialogNew                                  */
/* Description: Create an about dialog                                        */
/* Rexx Args:   None                                                          */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxAboutDialogNew(const char * Name,
                                  const size_t Argc, const RXSTRING Argv[],
                                  const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;

    /* Check for valid arguments */
    if (GrxCheckArgs(0, Argc, Argv))
        return RXFUNC_BADCALL;

    myWidget = gtk_about_dialog_new();

    /* Set up the REXX return code */
    g_snprintf(Retstr->strptr, RXAUTOBUFLEN, "%p", myWidget);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxAboutDialogSetProgramName                       */
/* Description: Set the program name                                          */
/* Rexx Args:   Pointer to the dialog                                         */
/*              Program name                                                  */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxAboutDialogSetProgramName(const char * Name,
                                             const size_t Argc, const RXSTRING Argv[],
                                             const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv))
        return RXFUNC_BADCALL;

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);

//    gtk_about_dialog_set_program_name(GTK_ABOUT_DIALOG(myWidget), Argv[1].strptr);
    gtk_about_dialog_set_name(GTK_ABOUT_DIALOG(myWidget), Argv[1].strptr);

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxAboutDialogSetVersion                           */
/* Description: Set the version                                               */
/* Rexx Args:   Pointer to the dialog                                         */
/*              Version                                                       */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxAboutDialogSetVersion(const char * Name,
                                         const size_t Argc, const RXSTRING Argv[],
                                         const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv))
        return RXFUNC_BADCALL;

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);

    gtk_about_dialog_set_version(GTK_ABOUT_DIALOG(myWidget), Argv[1].strptr);

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxAboutDialogSetCopyright                         */
/* Description: Set the copyright notice                                      */
/* Rexx Args:   Pointer to the dialog                                         */
/*              Copyright                                                     */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxAboutDialogSetCopyright(const char * Name,
                                           const size_t Argc, const RXSTRING Argv[],
                                           const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv))
        return RXFUNC_BADCALL;

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);

    gtk_about_dialog_set_copyright(GTK_ABOUT_DIALOG(myWidget), Argv[1].strptr);

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxAboutDialogSetComments                          */
/* Description: Set the comments                                              */
/* Rexx Args:   Pointer to the dialog                                         */
/*              Comment                                                       */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxAboutDialogSetComments(const char * Name,
                                          const size_t Argc, const RXSTRING Argv[],
                                          const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv))
        return RXFUNC_BADCALL;

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);

    gtk_about_dialog_set_comments(GTK_ABOUT_DIALOG(myWidget), Argv[1].strptr);

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxAboutDialogSetLicense                           */
/* Description: Set the license                                               */
/* Rexx Args:   Pointer to the dialog                                         */
/*              License                                                       */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxAboutDialogSetLicense(const char * Name,
                                         const size_t Argc, const RXSTRING Argv[],
                                         const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv))
        return RXFUNC_BADCALL;

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);

    gtk_about_dialog_set_license(GTK_ABOUT_DIALOG(myWidget), Argv[1].strptr);

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxAboutDialogSetWrapLicense                       */
/* Description: Set the wrap license flag                                     */
/* Rexx Args:   Pointer to the dialog                                         */
/*              Flag                                                          */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxAboutDialogSetWrapLicense(const char * Name,
                                             const size_t Argc, const RXSTRING Argv[],
                                             const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    gboolean flag;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv))
        return RXFUNC_BADCALL;

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);
    sscanf(Argv[1].strptr, "%d", &flag);

    gtk_about_dialog_set_wrap_license(GTK_ABOUT_DIALOG(myWidget), flag);

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxAboutDialogSetWebsite                           */
/* Description: Set the website                                               */
/* Rexx Args:   Pointer to the dialog                                         */
/*              Website                                                       */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxAboutDialogSetWebsite(const char * Name,
                                         const size_t Argc, const RXSTRING Argv[],
                                         const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv))
        return RXFUNC_BADCALL;

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);

    gtk_about_dialog_set_website(GTK_ABOUT_DIALOG(myWidget), Argv[1].strptr);

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxAboutDialogSetWebsiteLabel                      */
/* Description: Set the website label                                         */
/* Rexx Args:   Pointer to the dialog                                         */
/*              Website                                                       */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxAboutDialogSetWebsiteLabel(const char * Name,
                                              const size_t Argc, const RXSTRING Argv[],
                                              const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv))
        return RXFUNC_BADCALL;

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);

    gtk_about_dialog_set_website_label(GTK_ABOUT_DIALOG(myWidget), Argv[1].strptr);

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxAboutDialogSetAuthors                           */
/* Description: Set the authors                                               */
/* Rexx Args:   Pointer to the dialog                                         */
/*              Author (could repeat)                                         */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxAboutDialogSetAuthors(const char * Name,
                                         const size_t Argc, const RXSTRING Argv[],
                                         const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    const gchar **authors;
    size_t i;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, 2, Argv))
        return RXFUNC_BADCALL;

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);
    authors = (const gchar **)g_malloc(sizeof(void *) * Argc);
    for (i = 0; i < Argc - 1; i++) {
        authors[i] = Argv[i + 1].strptr;
    }
    authors[i] = NULL;

    gtk_about_dialog_set_authors(GTK_ABOUT_DIALOG(myWidget), authors);

    g_free(authors);

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxAboutDialogSetArtists                           */
/* Description: Set the artists                                               */
/* Rexx Args:   Pointer to the dialog                                         */
/*              Author (could repeat)                                         */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxAboutDialogSetArtists(const char * Name,
                                         const size_t Argc, const RXSTRING Argv[],
                                         const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    const gchar **authors;
    size_t i;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, 2, Argv))
        return RXFUNC_BADCALL;

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);
    authors = (const gchar **)g_malloc(sizeof(void *) * Argc);
    for (i = 0; i < Argc - 1; i++) {
        authors[i] = Argv[i + 1].strptr;
    }
    authors[i] = NULL;

    gtk_about_dialog_set_artists(GTK_ABOUT_DIALOG(myWidget), authors);

    g_free(authors);

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxAboutDialogSetDocumenters                       */
/* Description: Set the documenters                                           */
/* Rexx Args:   Pointer to the dialog                                         */
/*              Author (could repeat)                                         */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxAboutDialogSetDocumenters(const char * Name,
                                             const size_t Argc, const RXSTRING Argv[],
                                             const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    const gchar **authors;
    size_t i;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, 2, Argv))
        return RXFUNC_BADCALL;

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);
    authors = (const gchar **)g_malloc(sizeof(void *) * Argc);
    for (i = 0; i < Argc - 1; i++) {
        authors[i] = Argv[i + 1].strptr;
    }
    authors[i] = NULL;

    gtk_about_dialog_set_documenters(GTK_ABOUT_DIALOG(myWidget), authors);

    g_free(authors);

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxAboutDialogSetLogo                              */
/* Description: Set the logo                                                  */
/* Rexx Args:   Pointer to the dialog                                         */
/*              Logo (filename)                                               */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxAboutDialogSetLogo(const char * Name,
                                      const size_t Argc, const RXSTRING Argv[],
                                      const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget;
    GdkPixbuf *logo;
    GError *error = NULL;              // we will just ignore this error

    /* Check for valid arguments */
    if (GrxCheckArgs(2, Argc, Argv))
        return RXFUNC_BADCALL;

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);
    logo = gdk_pixbuf_new_from_file(Argv[1].strptr, &error);

    gtk_about_dialog_set_logo(GTK_ABOUT_DIALOG(myWidget), logo);

    /* Set up the REXX return code */
    *(Retstr->strptr) = '0';
    Retstr->strlength = 1;

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxColorSelectionDialogNew                         */
/* Description: Create a color selection dialog                               */
/* Rexx Args:   Title                                                         */
/*          :   Initial color                                                 */
/*          :   Initial alpha (optional)                                      */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxColorSelectionDialogNew(const char * Name,
                                           const size_t Argc, const RXSTRING Argv[],
                                           const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget, *colorsel;
    GdkColor color;
    guint alpha = 65535;

    /* Check for valid arguments */
    if (GrxCheckArgs(2, 2, Argv))
        return RXFUNC_BADCALL;

    /* Initialize function parameters */
    if (Argc == 3) {
        sscanf(Argv[2].strptr, "%u", &alpha);
    }

    myWidget = gtk_color_selection_dialog_new(Argv[0].strptr);
    colorsel = GTK_COLOR_SELECTION_DIALOG(myWidget)->colorsel;
    gtk_color_selection_set_has_opacity_control(GTK_COLOR_SELECTION(colorsel),
                                                TRUE);
    gtk_color_selection_set_current_color(GTK_COLOR_SELECTION(colorsel), &color);
    gtk_color_selection_set_current_alpha(GTK_COLOR_SELECTION(colorsel), alpha);

    /* Set up the REXX return code */
    g_snprintf(Retstr->strptr, RXAUTOBUFLEN, "%p", myWidget);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}


/*----------------------------------------------------------------------------*/
/* Rexx External Function: GrxColorSelectionDialogGetColor                    */
/* Description: Get the selected color                                        */
/* Rexx Args:   Dialog widget                                                 */
/*----------------------------------------------------------------------------*/

APIRET APIENTRY GrxColorSelectionDialogGetColor(const char * Name,
                                                const size_t Argc, const RXSTRING Argv[],
                                                const char * Queuename, PRXSTRING Retstr)
{
    GtkWidget *myWidget, *colorsel;
    GdkColor color;
    guint alpha;

    /* Check for valid arguments */
    if (GrxCheckArgs(1, Argc, Argv))
        return RXFUNC_BADCALL;

    /* Initialize function parameters */
    sscanf(Argv[0].strptr, "%p", &myWidget);

    colorsel = GTK_COLOR_SELECTION_DIALOG(myWidget)->colorsel;
    gtk_color_selection_get_current_color(GTK_COLOR_SELECTION(colorsel), &color);
    alpha = gtk_color_selection_get_current_alpha(GTK_COLOR_SELECTION(colorsel));

    /* Set up the REXX return code */
    g_snprintf(Retstr->strptr, RXAUTOBUFLEN, "#%04X%04X%04X%04X",
               color.red, color.green, color.blue, alpha);
    Retstr->strlength = strlen(Retstr->strptr);

    return RXFUNC_OK;
}

