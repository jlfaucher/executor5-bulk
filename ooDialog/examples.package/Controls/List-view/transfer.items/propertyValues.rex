/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2012-2013 Rexx Language Association. All rights reserved.    */
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
 * propteryValues.rex
 *
 * A simple dialog that displays various real estate properites, presumably
 * homes for sale in La Jolla CA.
 *
 * When a column header is clicked on, each selected item is displayed in a
 * second dialog, in a list view, with totals for all the columns.  The columns
 * are re-ordered so that the column clicked on is first.  (Actually second, the
 * 'Property (MLS #)' column is always first.)
 *
 * In addition, the items in the second dialog are sorted, in ascending order,
 * on the column that was clicked.  This demonstrates the sortItems() and
 * setDatItem() methods of a list view.
 */

  -- Set up symbolic IDs for the resource IDs of the controls used in this,
  -- simple, program.
  symbols = .table~new
  symbols[IDC_LV_REALESTATE] = 150
  symbols[IDC_LV_TOTALS]     = 200

  -- Have this program only use the global .constDir, and load in our symbolic
  -- IDs.
  .application~useGlobalConstDir('O', symbols)

  -- Instantiate our main dialog object and run it.
  dlg = .PropertyDialog~new
  dlg~execute("SHOWTOP", IDI_DLG_OOREXX)

return 0
-- End of entry point.

::requires "ooDialog.cls"

/* class: PropertyDialog- - - - - - - - - - - - - - - - - - - - - - - - - - - *\

  The main dialog class for our program.  We subclass a UserDialog, which means
  we will define the dialog template for the underlying Windows dialog manually
  in our code.  That is, the ooDialog framework will create an in-memory dialog
  template based on methods of the UserDialog we invoke in our init() and
  defineDialog() methods.

\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::class 'PropertyDialog' subclass UserDialog

::method init
  expose properties

  -- The super class *must* be initialized before any methods of this class are
  -- invoked.
  forward class (super) continue

  -- Start the in-memory dialog template for this dialog.
  self~create(400, 300, 325, 200, "La Jolla CA Homes for Sale")

  -- Connect the column click event of the list view control with a method in
  -- this Rexx dialog.
  self~connectListNotify(IDC_LV_REALESTATE, 'COLUMNCLICK', onColClick)

  -- Get an array that contains the information for each item we will add to our
  -- list view.
  properties = self~generateListings


/** defineDialog()
 *
 *  Standard defineDialog() method for an UserDialog.  We add 2 push buttons and
 *  a list view control to the in-memory dialog template.
 */
::method defineDialog

  self~createListView(IDC_LV_REALESTATE, 10, 10, 305, 155, "REPORT SHOWSELALWAYS")

  self~createPushButton(IDOK, 210, 175, 50, 15, "DEFAULT", "Ok")
  self~createPushButton(IDCANCEL, 265, 175, 50, 15, , "Cancel")


/** initDialog()
 *
 * The initDialog() method is invoked by the ooDialog framework automatically at
 * the proper time.  When initDialog() is invoked the underlying dialog and
 * dialog controls have been created by the operarting system.
 *
 * The method should be used for configuring any of the controls, or the dialog
 * itself, where the configuration requires that the underlying windows exist.
 *
 * For this dialog, the method is used to set up the list view control.
 */
::method initDialog
  expose lv properties

  -- Instatiate a list view object and add some extended styles to the
  -- underlying Windows list view.
  lv = self~newListView(IDC_LV_REALESTATE)
  lv~addExtendedStyle("FULLROWSELECT GRIDLINES")

  -- Insert columns into the list view.  Each column is one piece of information
  -- for a single real estate property.
  lv~insertColumn(0, "Property (MLS #)", 50)
  lv~insertColumn(1, "Sq. ft.", 30, 'RIGHT')
  lv~insertColumn(2, "Acreage", 30, 'RIGHT')
  lv~insertColumn(3, "# Baths", 25, 'RIGHT')
  lv~insertColumn(4, "# Bedrooms", 35, 'RIGHT')
  lv~insertColumn(5, "Price", 55, 'RIGHT')

  -- Add each item to the list view.
  do d over properties
    lv~addRow( , , d~property, d~sqft, d~acreage~format( , 2), d~baths, d~bedrooms, d~price)
  end


/** onColClick()
 *
 * The event handler for the column click event of the list view control.  It is
 * invoked each time the user clicks on a column header in the list view.  The
 * event handler receives two arguments: the control ID of the list view control
 * and the zero-based column number of which header was clicked on.
 *
 * Here we simply invoke the showDisplayDialog() method, where all the work is
 * done.
 */
::method onColClick unguarded
  expose lv
  use arg id, colNo

  -- See if any item is selected, if not just return
  if lv~selectedItems < 1 then return 0

  self~showDisplayDialog(colNo)

  return 0


/** showDisplayDialog()
 *
 * This method instantiates and runs the secondary dialog in our program.  The
 * dialog is used to show the items in the main dialog selected by the user.
 */
::method showDisplayDialog unguarded private
  expose lv
  use strict arg colNo

  -- Reply so that the event handler will return.
  reply 0

  -- Get the index of the first selected list view item.
  index = lv~selected

  -- Now we will create an array to hold the information of all the selected
  -- items.
  properties = .array~new

  do while index <> -1
    d = .directory~new

    d~property = lv~itemText(index, 0)
    d~sqft     = lv~itemText(index, 1)
    d~acreage  = lv~itemText(index, 2)
    d~baths    = lv~itemText(index, 3)
    d~bedrooms = lv~itemText(index, 4)
    d~price    = lv~itemText(index, 5)

    -- Add the directory object to the array ...
    properties~append(d)

    -- ... and get the next selected item.
    index = lv~nextSelected(index)
  end

  -- Get our current position on the screen.  The secondary dialog uses this
  -- position to place itself on the screen.
  pos = self~getRealPos

  -- Instantiate the secondary dialog, ...
  displayDlg = .DisplayDialog~new

  -- ... pass in the information it needs, ...
  displayDlg~setProperties(properties, colNo, pos)

  -- ... and run it.
  displayDlg~execute('SHOWTOP')


/** generatListings()
 *
 * This method creates an array of the data used for a single list view item.
 * Each item in the array is a .Directory object.  Each directory object has
 * indexes containing the text for each column in the list view.
 *
 * Here the data is just manually added to each directory object.  The method
 * could be altered to obtain the data from some other source, say a database,
 * or a flat file, etc..
 */
::method generateListings private

  items = .array~new

  d = .directory~new
  d~property = 120000110
  d~sqft     = '9,142'
  d~acreage  = 1.9
  d~baths    = 9
  d~bedrooms = 4
  d~price    = '$28,500,000'
  items[1] = d

  d = .directory~new
  d~property = 110006452
  d~sqft     = '4,900'
  d~acreage  = .98
  d~baths    = 5
  d~bedrooms = 5
  d~price    = '$14,500,000'
  items[2] = d

  d = .directory~new
  d~property = 110041133
  d~sqft     = '4,162'
  d~acreage  = .67
  d~baths    = 5
  d~bedrooms = 6
  d~price    = '$12,500,000'
  items[3] = d

  d = .directory~new
  d~property = 100062115
  d~sqft     = '5,534'
  d~acreage  = 1.56
  d~baths    = 6
  d~bedrooms = 4
  d~price    = '$6,975,000'
  items[4] = d

  d = .directory~new
  d~property = 110055464
  d~sqft     = '2,609'
  d~acreage  = .74
  d~baths    = 3
  d~bedrooms = 3
  d~price    = '$725,000'
  items[5] = d


  d = .directory~new
  d~property = 110039301
  d~sqft     = '1,078'
  d~acreage  = .3
  d~baths    = 1
  d~bedrooms = 1
  d~price    = '$549,000'
  items[6] = d

  d = .directory~new
  d~property = 110004449
  d~sqft     = '1,467'
  d~acreage  = .4
  d~baths    = 3
  d~bedrooms = 3
  d~price    = '$419,000'
  items[7] = d

  return items


/* class: DisplayDialog- - - - - - - - - - - - - - - - - - - - - - - - - - - -*\

  The DisplayDialog, an UserDialog subclass.  The dialog displays the items
  selected by the user in the main dialog.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::class 'DisplayDialog' subclass UserDialog

::method init

  -- Initialize the super class.
  forward class (super) continue

  -- Start our in-memory dialog template.
  self~create(30, 30, 325, 200, "Selected La Jolla CA Properties")


/** setProperties()
 *
 * This method is used by the main dialog to pass in the information needed
 * by this dialog.  It must be invoked before the main dialog starts executing
 * this dialog.
 */
::method setProperties
  expose items colNo parentPos
  use strict arg items, colNo, parentPos


/** defineDialog()
 *
 *  Standard defineDialog() method for an UserDialog.  We add a push button and
 *  a list view control to the dialog.
 */
::method defineDialog

  self~createListView(IDC_LV_TOTALS, 10, 10, 305, 155, "REPORT SHOWSELALWAYS")

  self~createPushButton(IDOK, 265, 175, 50, 15, "DEFAULT", "Close")


/** initDialog()
 *
 * The initDialog() method is invoked by the ooDialog framework automatically at
 * the proper time.  When initDialog() is invoked the underlying dialog and
 * dialog controls have been created by the operarting system.
 *
 * The method should be used for configuring any of the controls, or the dialog
 * itself, where the configuration requires that the underlying windows exist.
 *
 * For this dialog, the method is mostly used to set up the list view control,
 * and then at the end of the method to position the dialog.
 */
::method initDialog
  expose items colNo parentPos parentHwnd

  list = self~newListView(IDC_LV_TOTALS)
  list~addExtendedStyle("FULLROWSELECT GRIDLINES")

  -- Add our columns to the list view.
  list~insertColumn(0, "Property (MLS #)", 50)
  list~insertColumn(1, "Sq. ft.", 30, 'RIGHT')
  list~insertColumn(2, "Acreage", 30, 'RIGHT')
  list~insertColumn(3, "# Baths", 25, 'RIGHT')
  list~insertColumn(4, "# Bedrooms", 35, 'RIGHT')
  list~insertColumn(5, "Price", 55, 'RIGHT')

  -- Set up counters to use to total up the values of each row.
  sqft     = 0
  acreage  = 0
  baths    = 0
  bedrooms = 0
  price    = 0

  -- Add a list view item for each of the real estate properties passed to us
  -- from the parent dialog.
  do d over items
    index = list~addRow( , , d~property, d~sqft, d~acreage~format( , 2), d~baths, d~bedrooms, d~price)

    -- Each item in a list view can have a value associated with it through the
    -- setItemData() method.  The value can be any Rexx object.  The value can
    -- be retrieved through the getItemData() method.  In addition, during a
    -- sort operation, the operating system passes this value to the Rexx sort
    -- method.  In this program, that is the reason for assigning a value to
    -- each list view item, we use it to sort with.
    list~setItemData(index, d)

    -- Total up the columns
    sqft     += d~sqft~changestr(',', '')
    acreage  += d~acreage
    baths    += d~baths
    bedrooms += d~bedrooms
    price    += d~price~changestr(',', '')~changestr('$', '')
  end

  -- Sort the items.  The list view control will sort the items for us.  We need
  -- to provide a 'call back' method.  The comments for the onSort() method
  -- explain how this works.
  list~sortItems('onSort')

  -- Add commas and dollar signs where needed for the totals
  sqft  = self~beautify(sqft)
  price = '$' || self~beautify(price)

  -- Add a blank row and then a 'totals' row.
  list~addRow( , , '', '', '', '', '', '')
  list~addRow( , , 'Totals:', sqft, acreage~format( , 2), baths, bedrooms, price)

  -- Decide if and how to re-order the columns.  The first column always stays
  -- as the property identifier, the MLS number.  Then the second column is set
  -- as the column the user clicked on, if it is a column other than the first
  -- column.
  if colNo > 1 then do
    colOrder = .array~of(0, 1, 2, 3, 4, 5)
    newOrder = .array~of(0, colNo)

    do i = 1 to 5
      if i == colNo then iterate
      newOrder~append(i)
    end

    list~setColumnOrder(newOrder)
  end

  -- Place this dialog over the parent, but slightly offset.
  parentPos~incr(20, 30)
  self~moveTo(parentPos, 'SHOWWINDOW')

  -- Make sure this dialog is not partially off the screen.
  self~ensureVisible


/** onSort()
 *
 * This is the sorting 'call back' method specified in the sortItems() method
 * invoked in initDialog().  The method recieves two arguments.  Each argument
 * is the data value associated with a list item through the setItemData()
 * method.
 *
 * The sorting call back method must return less than 0 if the first item should
 * come before the second item, greater than 0 if the first item should come
 * after the second item and 0 if the two items are equivalent in order.
 *
 * The value we associated with each list item was the .directory object that
 * contained the text for each column.  The exposed 'colNo' variable is the
 * column we should sort on.  So, this method simply looks at the text for the
 * column we are sorting on, turns the text into a number, and returns -1, 0, or
 * 1 as appropriate.  The actual sorting is done by the list view control
 * itself.
 */
::method onSort unguarded
  expose colNo
  use arg item1, item2

  select
    when colNo == 0 then do
      data1 = item1~property
      data2 = item2~property
    end
    when colNo == 1 then do
      data1 = item1~sqft~changestr(',', '')
      data2 = item2~sqft~changestr(',', '')
    end
    when colNo == 2 then do
      data1 = item1~acreage
      data2 = item2~acreage
    end
    when colNo == 3 then do
      data1 = item1~baths
      data2 = item2~baths
    end
    when colNo == 4 then do
      data1 = item1~bedrooms
      data2 = item2~bedrooms
    end
    when colNo == 5 then do
      data1 = item1~price~changestr(',', '')~changestr('$', '')
      data2 = item2~price~changestr(',', '')~changestr('$', '')
    end
  end
  -- End select

  -- All columns contain numbers, so we need to do a numeric comparsion rather
  -- than a string comparison.
  if data1 < data2 then return -1
  else if data1 > data2 then return 1
  else return 0


/** beautify()
 *
 * Simple helper method to add commas to a number.
 */
::method beautify private
  use strict arg num

  str = ''
  do while num~length > 3
    str = ',' || num~right(3) || str
    num = num~substr(1, num~length - 3)
  end

  -- Pickup any leading characters left in num
  str = num || str

  return str

