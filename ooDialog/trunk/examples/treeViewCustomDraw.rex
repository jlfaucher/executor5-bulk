/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 1995, 2004 IBM Corporation. All rights reserved.             */
/* Copyright (c) 2005-2012 Rexx Language Association. All rights reserved.    */
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
 *
 */
    -- Use the global .constDir for symbolic IDs and turn automatic data
    -- detection off.
    .application~setDefaults('O', 'rc\treeViewCustomDraw.h', .false)

    dlg = .InventoryDlg~new("rc\treeViewCustomDraw.rc", IDD_TREE_DLG)
    if dlg~initCode = 0 then do
        ret = dlg~execute("SHOWTOP")
    end

return 0

::requires "ooDialog.cls"  -- Require the ooDialog framework.

::class 'TreeViewConstants' mixinclass Object
::constant BMP_FILE  "bmp\treeViewCustomDraw.bmp"  -- Icons for selected/not-selected items.
::constant TREE_FILE "treeViewCustomDraw.inp"      -- Input file with the items to build the tree.
::constant ITEM_FILE "treeViewCustomDrawi.inp"     -- Input file with dynamically added items.

::constant UNSELECTED_FOLDER  0         -- Index for the icon of an unselected folder item
::constant SELECTED_FOLDER    1         -- Index for the icon of a selected folder item
::constant UNSELECTED_LEAF    2         -- Index for the icon of an unselected leaf item
::constant SELECTED_LEAF      3         -- Index for the icon of a selected leaf item


::class 'InventoryDlg' subclass RcDialog inherit CustomDraw TreeViewConstants

::method isLeafItem class
  use strict arg treeView, item

  info. = treeView~itemInfo(item)

return info.!Image == self~UNSELECTED_LEAF

::method init
    expose bkClr oddLevelClr evenLevelClr selectedClr leafClr
    use arg rcFile, idDlg

    self~init:super(rcFile, idDlg)
    if self~initCode <> 0 then return self~initCode

    if self~checkForRequiredFiles == .false then do
      self~initCode = 17
      return self~initCode
    end

    -- Connect dialog control events to methods in the Rexx dialog.

    --self~connectTreeViewEvent("IDC_TREE", "SelChanged", "onSelChanged_IDC_TREE")
    self~connectTreeViewEvent("IDC_TREE", "Expanding", "onExpanding_IDC_TREE")
    --self~connectTreeViewEvent("IDC_TREE", "Expanded", "onExpanded_IDC_TREE")
    self~connectTreeViewEvent("IDC_TREE", "DefaultEdit")
    self~connectTreeViewEvent("IDC_TREE","BEGINDRAG", "DefTreeDragHandler")
    --self~connectTreeViewEvent("IDC_TREE","Delete","OnDelete_IDC_TREE")
    self~connectTreeViewEvent("IDC_TREE","KeyDown","OnKeyDown_IDC_TREE")
    self~connectButtonEvent("IDC_PB_NEW", "CLICKED", "IDC_PB_NEW")
    self~connectButtonEvent("IDC_PB_DELETE", "CLICKED", "IDC_PB_DELETE")
    self~connectButtonEvent(IDC_PB_EXP_ALL, "CLICKED", "IDC_PB_EXP_ALL")
    self~connectButtonEvent(IDC_PB_COL_ALL, "CLICKED", "IDC_PB_COL_ALL")
    self~connectButtonEvent(IDC_PB_INFO, "CLICKED", "IDC_PB_INFO")

    bkClr        =  self~RGB(250, 250, 250)
    oddLevelClr  = self~RGB(82, 61, 0)
    evenLevelClr = self~RGB(0, 20, 82)
    selectedClr  = self~RGB(82, 0, 20)
    leafClr      = self~RGB(0, 82, 20)

    self~customDraw
    self~customDrawControl(IDC_TREE, 'TreeView')

return 0

/** checkForRequiredFiles()
 *
 * This method is used to check that the bitmap and input files are available.
 * It returns .true if they are found and .false if they are not found.
 */
::method checkForRequiredFiles private

    haveError = .false
    file = ''

    if stream(self~BMP_FILE, "C", "QUERY EXISTS") = "" then do
      haveError = .true
      file = self~BMP_FILE
    end
    else if stream(self~TREE_FILE, "C", "QUERY EXISTS") = "" then do
      haveError = .true
      file = self~TREE_FILE
    end
    else if stream(self~ITEM_FILE, "C", "QUERY EXISTS") = "" then do
        haveError = .true
        file = self~ITEM_FILE
    end

    if haveError then do
        msg   = "The required data file" file "does"  || .endOfLine || -
                "not exist.  Without that file, this progam"   || .endOfLine || -
                "will not run."                                || .endOfLine~copies(2) || -
                "The program will abort."
        title = 'File Error'

        ret = MessageDialog(msg, 0, title, 'OK', 'ERROR')
    end

return \haveError


/** initDialog()
 */
::method initDialog
    expose tv

    tv = self~newTreeView("IDC_TREE")

    -- Set the image list for the tree-vies
    image = .Image~getImage(self~BMP_FILE)
    imageList = .ImageList~create(.Size~new(16, 12), .Image~toID(ILC_COLOR8), 5, 2)
    if \image~isNull,  \imageList~isNull then do
         imageList~add(image)
         tv~setImageList(imageList, .Image~toID(TVSIL_NORMAL))
         image~release
    end

    -- Read the file containig the tree input data and build the tree.
    do while lines(self~TREE_FILE)
        line = linein(self~TREE_FILE)
        command = "tv~Add("||line||")"
        interpret command
    end

    -- Select the item with the text of Computers.
    hItem = tv~find('Computers')
    if hItem \== 0 then tv~select(hItem)

return 0


/*- - - - - - - - - - Event handler(s) - - - - - - - - - - - - - - - - - - - -*/

::method onCustomDraw unguarded
  expose tv bkClr oddLevelClr evenLevelClr selectedClr leafClr
  use arg tvcds

  --say 'onCustomDraw()'
  --say '  item:     ' tvcds~item
  --say '  level:    ' tvcds~level
  --say '  drawStage:' self~drawStage2string(tvcds~drawStage)

  if tvcds~drawStage == self~CDDS_ITEMPREPAINT then do
      tvcds~reply = self~CDRF_NEWFONT

      selected = tv~selected
      isLeaf   = .InventoryDlg~isLeafItem(tv, tvcds~item)

      if selected == tvcds~item then do
        tvcds~clrText   = selectedClr
        tvcds~clrTextBk = bkClr
      end
      else if isLeaf then do
        tvcds~clrText   = leafClr
        tvcds~clrTextBk = bkClr
      end
      else if tvcds~level // 2 == 1 then do
        tvcds~clrText   = oddLevelClr
        tvcds~clrTextBk = bkClr
      end
      else do
        tvcds~clrText   = evenLevelClr
        tvcds~clrTextBk = bkClr
      end

      return .true
  end

  return .false




/* Method onExpanding_IDC_TREE handles notification 'Expanding' for item IDC_TREE */
::method onExpanding_IDC_TREE
  expose ITEM_FILE
  use arg tree, item, what
  curTree = self~newTreeView(tree)
  itemInfo. = curTree~itemInfo(item)

  /* if the special item is selected, load the child items dynamically from a file */
  /* and delete all children of the item if it will be collapsed                   */
  if itemInfo.!TEXT = "Special Offers" then do
    if what = "COLLAPSED" & curTree~child(item) = 0 then do
      do while lines(self~ITEM_FILE)
        line = linein(self~ITEM_FILE)
        command = "curTree~insert(item,,"||line||")"
        interpret command
      end
      curTree~expand(item)
    end
    else
      if what = "EXPANDED" & curTree~child(item) \= 0 then do
        curTree~collapseAndReset(item)
      end
  end


/* Method onKeyDown_IDC_TREE handles notification 'KeyDown' for item IDC_TREE */
::method onKeyDown_IDC_TREE
  use arg treeId, key
  curTree = self~newTreeView(treeId)
  /* if DELETE key is pressed, delete the selected item */
  if key == .VK~DELETE then
    curTree~delete(curTree~Selected)
  else
    /* if INSERT key is pressed, simulate pressing the New button */
    if key == .VK~INSERT then
      self~IDC_PB_NEW

/* Method IDC_PB_NEW is connected to item IDC_PB_NEW */
::method IDC_PB_NEW
  -- When the new button is pressed, display a dialog that gets the name of the new item
  -- and inserts it into the tree.
  dlg = .NewTreeItemDlg~new("rc\treeViewCustomDraw.rc",  IDD_ADD_TREE_ITEM, self~newTreeView("IDC_TREE"))
  dlg~execute


/* Method IDC_PB_DELETE is connected to item IDC_PB_DELETE */
::method IDC_PB_DELETE
  /*delete the selected item */
  curTree = self~newTreeView("IDC_TREE")
  curTree~delete(curTree~selected)


/* Method IDC_PB_EXP_ALL is connected to item IDC_PB_EXP_ALL */
::method IDC_PB_EXP_ALL
  /*expand the selected item and all its childs */
  curTree = self~newTreeView("IDC_TREE")
  if curTree~selected = 0 then
    call infoDialog "No item selected !"
  else do
    curTree~expand(curTree~Selected)
    nextItem = curTree~child(curTree~Selected)
    do while nextItem \= 0
      curTree~expand(nextItem)
      nextItem = curTree~next(nextItem)
    end
  end


/* Method IDC_PB_COL_ALL is connected to item IDC_PB_COL_ALL */
::method IDC_PB_COL_ALL
  /*collapse the selected item and all its childs */
  curTree = self~newTreeView("IDC_TREE")
  if curTree~selected = 0 then
    call infoDialog "No item selected !"
  else do
    nextItem = curTree~child(curTree~Selected)
    do while nextItem \= 0
      curTree~collapse(nextItem)
      nextItem = curTree~next(nextItem)
    end
    curTree~collapse(curTree~selected)
  end

/* Method IDC_PB_INFO is connected to item IDC_PB_COL_ALL */
::method IDC_PB_INFO
  /* Display the attributes of the selected item */
  use arg tree
  curTree = self~newTreeView("IDC_TREE")
  itemInfo. = curTree~itemInfo(curTree~selected)

  if itemInfo.!Children = 0 then children = "no"
  else children = ""

  call InfoDialog 'The selected item "'itemInfo.!Text'" has' children 'children. The index for the icon is "'itemInfo.!Image'"',
                   ', the index for the selected icon is "'itemInfo.!SelectedImage'". The states are "'itemInfo.!State'".'

/** help()
 */
::method help
  call infoDialog "No help available."


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - *\

 NewTreeItemDlg - Class
   This class allows the user to add a new item to the tree-view

\*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
::class 'NewTreeItemDlg' subclass RcDialog inherit TreeViewConstants

::method init
  expose treeControl
  use arg scriptFile, dlgID, treeControl

  -- Initialize the super class.
  self~init:super(scriptFile, dlgID)

::method initDialog
    expose treeControl editControl childRB folderChk selected

    -- Save a reference to the current selected item
    selected = treeControl~selected

    editControl = self~newEdit(IDC_EDIT_NAME)
    childRB     = self~newRadioButton(IDC_RB_CHILD)
    siblingRB   = self~newRadioButton(IDC_RB_SIBLING)
    folderChk   = self~newCheckBox(IDC_CHK_FOLDER)

    -- If the selected is the root of the tree, a new item has to be inserted as
    -- a child.  So disable the radio buttons that allow the user to choose to
    -- insert as a child or sibling.  And, pre-check the add as a folder check
    -- box.
    if selected == treeControl~root then do
        childRB~~check~disable
        siblingRB~disable
        folderChk~check
    end
    else if .InventoryDlg~isLeafItem(treeControl, selected) then do
        siblingRB~~check~disable
        childRB~disable
    end
    else do
        siblingRB~check
    end

    -- Set a visual cue for the edit control.  This will only show when the edit
    -- control has no text in it, and does not have the focus.
    editControl~setCue("Enter name of new item")

return 0

::method ok
    expose treeControl editControl childRB folderChk selected

    -- Make sure the user has given the item a name.
    text = editControl~getText~strip
    if text == "" then do
        msg   = "You must enter the name of the new item"
        title = "Inventory - Add Item"

        ret = MessageDialog(msg, self~hwnd, title, 'WARNING', 'OKCANCEL')
        if ret == self~IDCANCEL then return self~cancel:super

        editControl~assignFocus
        return 0
    end

    -- See if the user wants to add this as a folder item, or a regular item.
    -- This will determine the image IDs we use when we insert the item
    addAsFolder = folderChk~checked

    -- Now insert the item either as a child or a sibling depending on what the
    -- user requested.
    if childRB~checked then do
        if addAsFolder then newItem = treeControl~insert(selected, , text, self~UNSELECTED_FOLDER, self~SELECTED_FOLDER)
        else newItem = treeControl~insert(selected, , text, self~UNSELECTED_LEAF, self~SELECTED_LEAF)
        treeControl~expand(treeControl~parent(newItem))
    end
    else do
        if addAsFolder then treeControl~insert(treeControl~Parent(selected), , text, self~UNSELECTED_FOLDER, self~SELECTED_FOLDER)
        else treeControl~insert(treeControl~Parent(selected), , text, self~UNSELECTED_LEAF, self~SELECTED_LEAF)
    end

    -- Finally, quit by invoking the super class ok() method.
return self~ok:super

