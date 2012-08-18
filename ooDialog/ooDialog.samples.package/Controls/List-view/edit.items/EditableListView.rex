/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2011-2011 Rexx Language Association. All rights reserved.    */
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
/*----------------------------------------------------------------------------*/

/**
 * This example shows a simple list view with the "edit labels" style.  This
 * style allows the user to edit the label of each item in the list in place.
 *
 * When the user clicks on a list item to select it and then clicks one more
 * time, the label of the item becomes editable.  The user can cancel the change
 * by using the escape key.  The OS sends several event notifications to the
 * parent dialog so that the programmer can handle the label editing.
 *
 * This would involve determining when the user had finished the editing,
 * whether the user canceled the edit, if the edited text was acceptable, and
 * finally with updating the list item with the new text.
 *
 * For simple purposes, the ooDialog framework will handle the details of label
 * editing for the programmer.  This is done by connecting a list view event
 * notification using the special keyword: DEFAULTEDIT.  With this keyword, the
 * ooDialog framework actually connect several event notifications and provides
 * its own event handlers to take care of the editing.
 *
 * The event connection is done like this:
 *
 *   self~connectListViewEvent(resourceID, "DEFAULTEDIT")
 *
 * This example also shows how to add drag and drop to the items in the list
 * view.  The use can drag the items to position them anywhere in the list view.
 * The method used here is also to allow the ooDialog framework to handle the
 * details in the drag and drop operation.
 *
 * The event connection for drag and drop is done like this:
 *
 *   self~connectListViewEvent(resourceID, "BEGINDRAG", "defListDragHandler")
 */

  -- We want to use symbolic IDs, even though there are only a few resource IDs.
  .application~useGlobalConstDir("O", 'EditableListView.h')


  dlg = .EditableListView~new

  if dlg~initCode = 0 then do
    dlg~create(30, 30, 325, 190, "List View with Item Editing")

    dlg~execute("SHOWTOP")
  end

::requires "ooDialog.cls"

::class 'EditableListView' subclass UserDialog

/** defineDialog()
 *
 *  Standard defineDialog() method for an UserDialog.  We add a push button and
 *  a list view controls to the dialog.  We also place our event connections
 *  here.
 */
::method defineDialog

  self~createListView(IDC_LISTVIEW, 10, 10, 305, 145, "ICON SINGLESEL EDIT")

  self~createPushButton(IDC_PB_CLOSE, 280, 165, 35, 15, "DEFAULT", "Close", close)

  self~connectListViewEvent(IDC_LISTVIEW, "defaultEdit")
  self~connectListViewEvent(IDC_LISTVIEW, "BEGINDRAG", "defListDragHandler")

/** initDialog()
 *
 *  Standard initDialog() method.  We create and assing a image list to the list
 *  view control and populate the list view with four itmes.
 */
::method initDialog
  expose list

  -- Get and save a reference to the list view control.
  list = self~newListView(IDC_LISTVIEW)

  -- Create and assign an image list to the list view.
  self~createImageList

  -- Add the items to the list view.  The last argument is the index of the icon
  -- in the image list to associate with the item.  There are only 2 images, a
  -- male figure with an index of 0 and a female figure with an index of 1.
  list~insert( , , "Tom Smith", 0)
  list~insert( , , "Henry Kogler", 0)
  list~insert( , , "Sue Grafton", 1)
  list~insert( , , "Mary Simms", 1)

/** close()
 *
 *  The close() method is the event handler for the 'Close' push button.  It
 *  simply puts up a message box that lists the labe for each item in the list
 *  view.  The user can verify that the label was indeed changed if she edited
 *  it.  Then we end the dialog with Ok.
 *
 *  Note that there is no event handler for the 'Cancel' event.  We just let the
 *  default handler in the super-class deal with cancel.  The dialog just ends.
 */
::method close
  expose list

  msg = "The text, (the labels,) of the list view" || .endOfLine            || -
        "items:"                                   || .endOfLine~copies(2)  || -
        " " list~itemText(0)                       || .endOfLine            || -
        " " list~itemText(1)                       || .endOfLine            || -
        " " list~itemText(2)                       || .endOfLine            || -
        " " list~itemText(3)

  z = MessageDialog(msg, self~hwnd, "List View Item Listing")

  return self~ok:super


/** createImageList()
 *
 *  Creates an image list and assigns it to the list view control.
 *
 *  Each item in a list view can have an image index associated with it.  The
 *  list view matches the image index of each item with an image index in the
 *  image list.
 *
 *  If it finds a matching index in the image list, it displays that image along
 *  with the text of item.  If it can not match the index, no image is
 *  displayed.
 */
::method createImageList private
  expose list

  -- Create an image object from a file.
  normalIcons = .Image~getImage("EditableListView.bmp")

  -- Create an image list whose image sizes are 32 x 32.  The image list uses
  -- uses 4-bit color (ILC_COLOR4.)  We only have 2 images in the image list, a
  -- male and female icon.  And we do not need to add any images after we create
  -- it.
  imageList = .ImageList~create(.Size~new(32, 32), .Image~toID(ILC_COLOR4), 2, 0)

  -- Now, if there were no errors creating the image or the image list, add the
  -- bitmap image to the image list, and then set the image list in the list
  -- view.
  if \normalIcons~isNull,  \imageList~isNull then do
      imageList~add(normalIcons)

      list~setImageList(imageList, .Image~toID(LVSIL_NORMAL))

      -- The image list creates a copy of the bitmap, so we can release the
      -- bitmap image now.
      normalIcons~release
  end
  else do
    -- Let the user know about the error.
    msg = "Error.  The icon images were not" || .endOfLine || -
          "added to the list view control."  || .endOfLine

    if .SystemErrorCode <> 0 then do
      msg ||= .endOfLine || -
          "Operating system error code:" .SystemErrorcode  || .endOfLine~copies(2) || -
          "Explanation:"                                   || .endOfLine           || -
          SysGetErrorText(.SystemErrorCode)
    end

    j = MessageDialog(msg, self~hwnd, "Errors Detected", "OK", "WARNING")
  end


