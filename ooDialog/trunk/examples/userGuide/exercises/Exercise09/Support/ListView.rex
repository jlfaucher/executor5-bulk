/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2011-2014 Rexx Language Association. All rights reserved.    */
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
/* ooDialog User Guide
   Exercise 09: The ListView superclass				  v01-00 09Oct13

   Contains: the superclass "ListView".

   Description: Provides a list superclass for lists of Customers, Products, etc.
                This makes the constructioon of list dialogs very easy.
                Supports two ways of surfacing item components such as Customer
                and Product: first by double-click on a l;ist item, second by
                selecting a list item and pressing the "show" button.

                Format of list is done via the variable dlgInfo (an array) which
                is provided by the sublcass in its required getDlgInfo class
                method. The format of dlgInfo is:
                dlgInfo[1]: a class that provides the set of text constants
                  to display, including dialog title, menu text, button text,
                  and text in a warning message. This not only provides for
                  customisation but also for NLS.
                dlgInfo[2]: A string that specifies the columns in the list view.
                  The string format for a single column is:
                    n-columnTitle-columnWidth
                  Multiple columns are comma-separated. For example:
                    "2-Family Name-80,5-Job Description-100,1-Number-60"
                  This specifies three columns, the left-most specifying the first
                  column in the list view. Each column specification defines
                  (a) the field in the data record (b), the column title, and
                  (c) the width of the column in pixels. Thus the first spec
                  ("2-Family Name-80") specifies that the first column contains
                  the second field from the data record, the column heading is
                  "Family Name", and the width of the column in 80 pixels.
   v01-00 09Oct13: First Version.

   Outstanding Problems: None reported.
*******************************************************************************/


::REQUIRES "ooDialog.cls"
::REQUIRES "Support\View.rex"
::REQUIRES "Support\Component.rex"

::CLASS ListView SUBCLASS UserDialog PUBLIC INHERIT View Component

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  ::METHOD newInstance CLASS PUBLIC
    use arg idListModel, rootDlg			-- Defined by MVF
    dlgConfig = self~getDlgConfig	 -- Dialog info/config is provided by the subclass.
    dlg = self~new
    if dlg~initCode = 0 then do
      -- Add a symbolic resource IDs:
      dlg~constdir[IDC_LISTVIEW] = 200
      dlg~constdir[IDC_SHOWITEM] = 201
      --say ".ListView-newInstance-01: HRS class =" dlgText
      --say ".ListView-newInstance-02: Dialog Title =" dlgText~dlgTitle
      --dlg~create(100, 100, 225, 273, "Test ListView UserDlg 2", "VISIBLE")
      --say ".ListView-iewInstance-01: HRS~noMenu =" dlgtext~noMenu
      dlg~activate( idListModel, rootDlg, dlgConfig)
      --dlg~execute("SHOWTOP")
    end
    return dlg						-- Required by MVF

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  ::METHOD init
    forward class (super) continue
    self~initView
    --say "ListView-init-01."

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  ::METHOD activate UNGUARDED
    expose rootDlg dlgConfig modelData
    use arg idListModel, rootDlg, dlgConfig
    forward class (super) continue		-- super invokes defineDialog
    modelData = RESULT

    columnSpec = dlgConfig[lvColumns]	-- a comma-speparated spec of col no & width.
    dlgSize = dlgConfig[dlgSize]
    parse var dlgSize dlgWidth "-" dlgHeight
    self~addIconResource(199, dlgConfig[dlgIcon])
    self~create(100, 100, dlgWidth, dlgHeight, dlgConfig[text]~dlgTitle, "VISIBLE MINIMIZEBOX")
    self~popupAsChild(rootDlg, "SHOWTOP",,199)

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  ::METHOD defineDialog
    expose dlgConfig
    dlgText = dlgConfig[text]
    lvSize = dlgConfig[lvSize]
    parse var lvSize lvW "-" lvH
    -- say "ListView-defineDialog-01: lvWdth lvHeight =" lvW lvH
    self~createListView(IDC_LISTVIEW, 15,10,lvW,lvH, "REPORT SHOWSELALWAYS")
    self~createPushButton(IDCANCEL, 150, 235, 50, 14, "", dlgText~cancelButton)
    self~createPushButton(IDC_SHOWITEM, 89, 235, 54, 14, "DEFAULT DISABLED", -
                          dlgText~showButton, showItem)

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  ::METHOD initDialog
    expose lv dlgConfig keyColSpec keyColNum
    --say "ListView-initDialog-01."
    dlgText = dlgConfig[text]
    menu = .BinaryMenuBar~new(.nil, 901, , self)

    subMenu = .PopupMenu~new(902)
    menu~insertPopup(902, 902, subMenu, dlgText~menu1)
    subMenu~insertItem(903, 903, dlgText~menu11)

    subMenu = .PopupMenu~new(912)
    menu~insertPopup(912, 912, subMenu, dlgtext~menu2)
    subMenu~insertitem(913, 913, dlgText~menu21)

    -- Note: menu texts may be translated, so cannot use 'connectAllCommandEvents'
    --       since this uses the text as the method name.
    menu~connectCommandEvent(903,actionMenu11)
    menu~connectCommandEvent(913,actionMenu21)

    -- Get a reference to the ListView:
    lv = self~newListView(IDC_LISTVIEW)
    lv~addExtendedStyle("FULLROWSELECT GRIDLINES")

    -- Format the ListView:
    -- 1. Make an array out of the string columns specs:
    arrColSpecs = dlgConfig[lvColumns]~makeArray(",")
    --say "ListView-initDialog-01: arColSpecs:" arrColSpecs[1] arrColSpecs[2] arrColSpecs[3]
    -- 2. Make an array for any format flags (either "f" or "r" or "fr" -
    --    f = invoke self~format(n) where n is the column name)
    --    r = right-adjust data in the column
    -- 3. Note that a key column cannot (in this version) be formatted
    --    using format flags.
    -- Parse the column definitions and issue the 'insertColumnsPX's; also
    --  make an array of the data fields to be shown:
    fileColNumbers = .array~new	-- an array of the file column numbers in the spec.
    arrColCallBacks = .array~new
    keyColNum = 1		-- if no key column defined, then default is 1.
    do i = 1 to arrColSpecs~items	-- do for each column spec
      colSpec = arrColSpecs[i]
      parse var colSpec colHeader "-" pixels "-" colNum "-" flags
      --say "ListView-initDialog-02:"
      fileColNumbers[i] = colNum
      if flags~caselessPos("R") >0 then colAlign = "R"; else colAlign = "L"
      lv~insertColumnPX(i-1,colHeader,pixels,colAlign)
      if flags~caselessPos("K") then keyColNum = i
      if flags~caselessPos("F") then arrColCallBacks[i] = .true
    end

/*    lv~insertColumnPX(0,"Number",60,"LEFT")
    lv~insertColumnPX(1,"Name",150,"LEFT")
    lv~insertColumnPX(2,"Zip",60,"LEFT") */
    self~connectListViewEvent("IDC_LISTVIEW","CLICK",itemSelected) -- Single click
    self~connectListViewEvent("IDC_LISTVIEW","ACTIVATE",openItem)  -- Double-click

    self~loadList(fileColNumbers,arrColCallBacks)

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  ::METHOD loadList PRIVATE
    expose lv modelData
    use strict arg fileColNumbers, arrColCallBacks
    -- fileColNumbers is an array of the file columns to be selected and shown
    -- on the listview. E.g. .array~of(3,1,5) states that fields (i.e. columns)
    -- to be selected from the file are (in this order) 3, 1, and 5.)
    --say "ListView-loadList-00: modelData =" modelData "columnNumbers =" fileColNumbers
    rows = modelData[count]
    colsRequired = fileColNumbers~items
    --say "ListView-loadlist-01: fileColNumbers =" fileColNumbers~toString(,",")
    arrData = modelData[records]

    arrRow = .array~new	     	     -- The array to be used in addRowFromArray().
    do i=1 to rows			-- iterate over data rows
      do j = 1 to colsRequired		   -- iterate over the cols required
      	col = fileColNumbers[j]
      	--say "ListView-loadList-02: col =" col
     	arrRow[j] = arrData[i,col]
     	if arrColCallBacks[j] = .true then do
     	  --say "ListView-loadList-03: modelData[i,col] =" arrData[i,col]
     	  -- col = 2 -- debug stmt - wrong column to check if subclass returns .false.
     	  formattedField= self~lvFieldFormat(col, arrData[i,col])
     	  if formattedField \= .false then arrRow[j] = formattedField
     	  else say "ListView-loadList-01: Error in specifying correct field for subclass method 'lvFieldFormat'."
        end
      end
      lv~addRowFromArray(arrRow, , 'null')
    end

   --lv~addRow( , , arrData[i,1], arrData[i,2], arrData[i,3])
   -- lvCustomers~setColumnWidth(1) -- set width of 2nd column to longest text entry.

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  ::METHOD showItem UNGUARDED		-- Surfaces the item dialog
    expose lv rootDlg keyColNum
    --say "ListView-showItem-00: column that's the key is:" keyColNum
    item = lv~selected
    if item = -1 then do
      say "ListView-showItem-01: Nothing selected."
      return
    end
    info = .Directory~new
    --if lv~getItemInfo(item, info) then do
    if lv~getItemInfo(item, info, keyColNum-1) then do
      -- say "ListView-showItem-02: info~text =" info~text
      objectMgr = .local~my.ObjectMgr
      parse var self . className	-- Q: Who am I? A: an xxxxListView
      parse var className classRoot "LISTVIEW"
      --say "ListView-showItem-03: Class Name Root =" classRoot
      modelClassName = classRoot||"Model"
      objectMgr~showModel(modelClassName, info~text, rootDlg)
      self~disableControl("IDC_SHOWITEM")
      --self~focusControl("IDC_LISTVIEW")
    end


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  ::METHOD openItem UNGUARDED
    --say "openItem-01."
    self~showItem


  /*----------------------------------------------------------------------------
    Event-Handler Methods - Menu Events
    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  ::METHOD actionMenu11 UNGUARDED
    expose dlgText
    say "ListView-actionMenu11-01: dlgText =" dlgText
    self~noMenuFunction(dlgText~menu11)

  /*- - Help - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  ::METHOD actionMenu21 UNGUARDED
    expose dlgText
    say "dlgText =" dlgText
    self~noMenuFunction(dlgText~menu21)

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  ::METHOD noMenuFunction UNGUARDED
   expose dlgText
    use arg title
    ret = MessageDialog(dlgtext~noMenu, self~hwnd, title, 'WARNING')


  /*----------------------------------------------------------------------------
    Event Handling Methods - List Items
    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  ::METHOD itemSelected UNGUARDED
    use arg id, itemIndex, columnIndex, keyState
    --say "ListView-itemSelected-01; item index =" itemIndex
    if itemIndex > -1 then do
      self~enableControl("IDC_SHOWITEM")
      --self~focusControl( "IDC_SHOWITEM") -- This changes the select color of the selected item!!!!
    end
    else self~disableControl("IDC_SHOWITEM")

/*============================================================================*/




