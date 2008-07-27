/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 1995, 2004 IBM Corporation. All rights reserved.             */
/* Copyright (c) 2005-2006 Rexx Language Association. All rights reserved.    */
/*                                                                            */
/* This program and the accompanying materials are made available under       */
/* the terms of the Common Public License v1.0 which accompanies this         */
/* distribution. A copy is also available at the following address:           */
/* http://www.ibm.com/developerworks/oss/CPLv1.0.htm                          */
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
/******************************************************************************/
/*                                                                            */
/*       Windows Dialog Interface for Object REXX                             */
/*                                                                            */
/* Windows 32-bit Control Support and Dialog Control Classes                  */
/*                                                                            */
/******************************************************************************/

::class AdvancedControls Mixinclass Object public


::method GetControl unguarded private
   use arg id, category, type
   if Arg(1,'o') = 1 then return .Nil
   select
       when type = "ST" then class = .StaticControl
       when type = "EL" then class = .EditControl
       when type = "BUT" then class = .ButtonControl
       when type = "RB" then class = .RadioButton
       when type = "CH" then class = .CheckBox
       when type = "LB" then class = .ListBox
       when type = "CB" then class = .ComboBox
       when type = "SB" then class = .ScrollBar
       when type = "TC" then class = .TreeControl
       when type = "LC" then class = .ListControl
       when type = "PB" then class = .ProgressBar
       when type = "SC" then class = .SliderControl
       when type = "TAB" then class = .TabControl
       otherwise return .Nil
   end
   ctrlobj = .Nil
   if Arg(2,'o') = 1 then do
       /* check if dialog is subclass of CategoryDialog */
       if self~HasMethod("!MarkAsCatDlg!") = 1 then
           ctrlobj = class~new(self, id, self~catalog['category'])  /* suppose active page is ment */
       else ctrlobj = class~new(self, id)
   end
   else ctrlobj = class~new(self, id, category)
   if ctrlobj~hwnd = 0 then return .Nil; else return ctrlobj

::method GetStaticControl unguarded
   arr = Arg(1,"A")
   arr[3] = "ST"
   forward message "GetControl" Arguments (arr)

::method GetEditControl unguarded
   arr = Arg(1,"A")
   arr[3] = "EL"
   forward message "GetControl" Arguments (arr)

::method GetButtonControl unguarded
   arr = Arg(1,"A")
   arr[3] = "BUT"
   forward message "GetControl" Arguments (arr)

::method GetRadioControl unguarded
   arr = Arg(1,"A")
   arr[3] = "RB"
   forward message "GetControl" Arguments (arr)

::method GetCheckControl unguarded
   arr = Arg(1,"A")
   arr[3] = "CH"
   forward message "GetControl" Arguments (arr)

::method GetListBox unguarded
   arr = Arg(1,"A")
   arr[3] = "LB"
   forward message "GetControl" Arguments (arr)

::method GetComboBox unguarded
   arr = Arg(1,"A")
   arr[3] = "CB"
   forward message "GetControl" Arguments (arr)

::method GetScrollBar unguarded
   arr = Arg(1,"A")
   arr[3] = "SB"
   forward message "GetControl" Arguments (arr)

::method GetTreeControl unguarded
   arr = Arg(1,"A")
   arr[3] = "TC"
   forward message "GetControl" Arguments (arr)

::method GetListControl unguarded
   arr = Arg(1,"A")
   arr[3] = "LC"
   forward message "GetControl" Arguments (arr)

::method GetProgressBar unguarded
   arr = Arg(1,"A")
   arr[3] = "PB"
   forward message "GetControl" Arguments (arr)

::method GetSliderControl unguarded
   arr = Arg(1,"A")
   arr[3] = "SC"
   forward message "GetControl" Arguments (arr)

::method GetTabControl unguarded
   arr = Arg(1,"A")
   arr[3] = "TAB"
   forward message "GetControl" Arguments (arr)


::method ConnectTreeControl unguarded
   use arg id, attributeName
   forward message "AddAttribute" continue
   if result = -1 then return -1
   return DataTable(self~Adm,"ADD",result,6);         /* new id in result  6 == Tree Type */

::method ConnectListControl unguarded
   use arg id, attributeName
   forward message "AddAttribute" continue
   if result = -1 then return -1
   return DataTable(self~Adm,"ADD",result,7);         /* new id in result  7 == List Type */

::method ConnectSliderControl unguarded
   use arg id, attributeName
   forward message "AddAttribute" continue
   if result = -1 then return -1
   return DataTable(self~Adm,"ADD",result,8);         /* new id in result  8 == Track Bar Type */

::method ConnectTabControl unguarded
   use arg id, attributeName
   forward message "AddAttribute" continue
   if result = -1 then return -1
   return DataTable(self~Adm,"ADD",result,9);         /* new id in result  9 == Tab Type */


::method InternGetNewCtrlStyle
   use arg style, kind
   ret = ""
   if style~wordpos("NOT WS_VISIBLE") > 0 then ret = "HIDDEN"
   else if style~wordpos("WS_VISIBLE") > 0 then ret = "VISIBLE"
   if style~wordpos("WS_DISABLED") > 0 then ret = ret || " DISABLED"
   if kind = "TREE" then do
      if style~wordpos("TVS_DISABLEDRAGDROP") > 0 then ret = ret || " NODRAG"
      if style~wordpos("TVS_EDITLABELS") > 0 then ret = ret || " EDIT"
      if style~wordpos("TVS_HASBUTTONS") > 0 then ret = ret || " BUTTONS"
      if style~wordpos("TVS_HASLINES") > 0 then ret = ret || " LINES"
      if style~wordpos("TVS_LINESATROOT") > 0 then ret = ret || " ATROOT"
      if style~wordpos("TVS_SHOWSELALWAYS") > 0 then ret = ret || " SHOWSELALWAYS"
      if style~wordpos("WS_BORDER") = 0 then ret = ret || " NOBORDER"
   end
   else if kind = "LIST" then do
      if style~wordpos("LVS_EDITLABELS") > 0 then ret = ret || " EDIT"
      if style~wordpos("LVS_SHOWSELALWAYS") > 0 then ret = ret || " SHOWSELALWAYS"
      if style~wordpos("LVS_ALIGNLEFT") > 0 then ret = ret || " ALIGNLEFT"
      if style~wordpos("LVS_ALIGNTOP") > 0 then ret = ret || " ALIGNTOP"
      if style~wordpos("LVS_AUTOARRANGE") > 0 then ret = ret || " AUTOARRANGE"
      if style~wordpos("LVS_ICON") > 0 then ret = ret || " ICON"
      if style~wordpos("LVS_SMALLICON") > 0 then ret = ret || " SMALLICON"
      if style~wordpos("LVS_LIST") > 0 then ret = ret || " LIST"
      if style~wordpos("LVS_REPORT") > 0 then ret = ret || " REPORT"
      if style~wordpos("LVS_NOCOLUMNHEADER") > 0 then ret = ret || " NOHEADER"
      if style~wordpos("LVS_NOLABELWRAP") > 0 then ret = ret || " NOWRAP"
      if style~wordpos("LVS_NOSCROLL") > 0 then ret = ret || " NOSCROLL"
      if style~wordpos("LVS_NOSORTHEADER") > 0 then ret = ret || " NOSORTHEADER"
      if style~wordpos("LVS_SHAREIMAGELISTS") > 0 then ret = ret || " SHAREIMAGES"
      if style~wordpos("LVS_SINGLESEL") > 0 then ret = ret || " SINGLESEL"
      if style~wordpos("LVS_SORTASCENDING") > 0 then ret = ret || " ASCENDING"
      if style~wordpos("LVS_SORTDESCENDING") > 0 then ret = ret || " DESCENDING"
      if style~wordpos("WS_BORDER") = 0 then ret = ret || " NOBORDER"
   end
   else if kind = "SLIDER" then do
       if style~wordpos("TBS_AUTOTICKS") > 0 then ret = ret || " AUTOTICKS"
       if style~wordpos("TBS_NOTICKS") > 0 then ret = ret || " NOTICKS"
       if style~wordpos("TBS_VERT") > 0 then ret = ret || " VERTICAL"
       if style~wordpos("TBS_HORZ") > 0 then ret = ret || " HORIZONTAL"
       if style~wordpos("TBS_TOP") > 0 then ret = ret || " TOP"
       if style~wordpos("TBS_BOTTOM") > 0 then ret = ret || " BOTTOM"
       if style~wordpos("TBS_LEFT") > 0 then ret = ret || " LEFT"
       if style~wordpos("TBS_RIGHT") > 0 then ret = ret || " RIGHT"
       if style~wordpos("TBS_BOTH") > 0 then ret = ret || " BOTH"
       if style~wordpos("TBS_ENABLESELRANGE") > 0 then ret = ret || " ENABLESELRANGE"
       if style~wordpos("WS_BORDER") > 0 then ret = ret || " BORDER"
   end
   else if kind = "TAB" then do
        if style~wordpos("WS_GROUP") > 0 then ret = ret || " GROUP"
        if style~wordpos("TCS_BUTTONS") > 0 then ret = ret || " BUTTONS"
        if style~wordpos("TCS_FIXEDWIDTH") > 0 then ret = ret || " FIXED"
        if style~wordpos("TCS_FOCUSNEVER") > 0 then ret = ret || " FOCUSNEVER"
        if style~wordpos("TCS_FOCUSONBUTTONDOWN") > 0 then ret = ret || " FOCUSONDOWN"
        if style~wordpos("TCS_FORCEICONLEFT") > 0 then ret = ret || " ICONLEFT"
        if style~wordpos("TCS_FORCELABELLEFT") > 0 then ret = ret || " LABELLEFT"
        if style~wordpos("TCS_MULTILINE") > 0 then ret = ret || " MULTILINE"
        if style~wordpos("TCS_RIGHTJUSTIFY") > 0 then ret = ret || " ALIGNRIGHT"
        if style~wordpos("WS_BORDER") > 0 then ret = ret || " BORDER"
   end
   else if kind = "PROGRESS" then do
        if style~wordpos("PBS_VERTICAL") > 0 then ret = ret || " VERTICAL"
        if style~wordpos("PBS_SMOOTH") > 0 then ret = ret || " SMOOTH"
        if style~wordpos("WS_BORDER") > 0 then ret = ret || " BORDER"
   end
   if style~wordpos("WS_VSCROLL") > 0 then ret = ret || " VSCROLL"
   if style~wordpos("WS_HSCROLL") > 0 then ret = ret || " HSCROLL"
   if style~wordpos("WS_TABSTOP") = 0 then ret = ret || " NOTAB"
   return ret


::method AddTreeControl
   use arg id, attname, x, y, cx, cy, opts
   symbid = id
   id = self~ItemAdd(id)
   if id < 0 then return id
   if Arg(2,'o') = 1 then attname = "DATA"id
   opts = self~GetDefaultOpts(opts)
   if opts = "ALL" then opts = "LINES VSCROLL HSCROLL EDIT BUTTONS ATROOT SHOWSELALWAYS"
   self~AktPtr = AddTreeControl(self~AktPtr, id, x, y, cx, cy, opts)
   if self~AutoDetect = 1 then do
       self~ConnectTreeControl(symbid, attname)
   end


::method AddListControl
   use arg id, attname, x, y, cx, cy, opts
   symbid = id
   id = self~ItemAdd(id)
   if id < 0 then return id
   if Arg(2,'o') = 1 then attname = "DATA"id
   opts = self~GetDefaultOpts(opts)
   self~AktPtr = AddListControl(self~AktPtr, id, x, y, cx, cy, opts)
   if self~AutoDetect = 1 then do
       self~ConnectListControl(symbid, attname)
   end

::method AddProgressBar
   use arg id, x, y, cx, cy, opts
   symbid = id
   id = self~ItemAdd(id)
   if id < 0 then return id
   opts = self~GetDefaultOpts(opts)
   self~AktPtr = AddProgressControl(self~AktPtr, id, x, y, cx, cy, opts)


::method AddSliderControl
   use arg id, attname, x, y, cx, cy, opts
   symbid = id
   id = self~ItemAdd(id)
   if id < 0 then return id
   if Arg(2,'o') = 1 then attname = "DATA"id
   opts = self~GetDefaultOpts(opts)
   self~AktPtr = AddSliderControl(self~AktPtr, id, x, y, cx, cy, opts)
   if self~AutoDetect = 1 then do
       self~ConnectSliderControl(symbid, attname)
   end


::method AddTabControl
   use arg id, attname, x, y, cx, cy, opts
   symbid = id
   id = self~ItemAdd(id)
   if id < 0 then return id
   if Arg(2,'o') = 1 then attname = "DATA"id
   opts = self~GetDefaultOpts(opts)
   self~AktPtr = AddTabControl(self~AktPtr, id, x, y, cx, cy, opts)
   if opts~wordpos("CAT") = 0 & self~AutoDetect = 1 then do
       self~ConnectTabControl(symbid, attname)
   end



/***************************************************************************************************************/
/*                                       Control Classes                                                       */
/***************************************************************************************************************/

/* Base class for all other controls */
::class DialogControl public inherit WindowBase WindowExtensions

::method oDlg attribute
::method hDlg attribute
::method Id attribute

::method new class
   use arg pdialog, id, category
   Forward Class(Super) continue  /* this also calls Init */
   obj = result

   if obj \= .Nil then do
      obj~Hwnd = 0
      obj~Id = 0
      if Arg() = 0 then do
         obj~oDlg = .Nil
         return obj
      end
      else do
         parse value GetDialogFactor() with x y
         obj~FactorX=x
         obj~FactorY=Y
      end

      if self~Id = -1 then return .Nil
      if Arg(3,'o') = 1 | category = 0 then do
          obj~hDlg = pDialog~DlgHandle
          obj~AssignWindow(pDialog~GetItem(id))
      end
      else do
          obj~hDlg = pDialog~catalog['handles'][category]
          obj~AssignWindow(pDialog~GetItem(id, obj~hDlg))
      end
   end
   return obj


::method Init
   use arg pdialog, id, category
   self~init:super
   if Arg() = 0 then return
   self~oDlg = pDialog
   if id~DataType("N") = 0 then id = pDialog~ResolveSymbolicId(id)
   self~Id = id

::method AssignWindow
   use arg hwnd, parent, category
   self~Hwnd = hwnd
   if hwnd = 0 then return 0
   parse value Wnd_Desktop("RECT", hwnd) with x y cx cy
   self~SizeX = (cx - x) % self~FactorX
   self~SizeY = (cy - y) % self~FactorY
   if self~Id = 0 then self~Id = self~GetID    /* the id is important for most methods */
   if Arg(2,'e') = 1 then do
       self~oDlg = parent
       if Arg(3,'o') = 1 then self~hDlg = self~oDlg~DlgHandle
       else self~hDlg = self~oDlg~catalog['handles'][category]
   end
   return hwnd


::method ProcessMessage
   parse arg msg,wp,lp
   return SendWinMsg("DLG",self~hDlg,self~Id,msg,wp,lp)

::method Message2Parent unguarded private /* This is the replacement for BoxMessage */
   use arg msg, wP, lP
   return SendWinMsg("PTR", Self~hDlg, Self~Id, msg, wP, lP)

::method SetColor
   use arg colorbk, colorfg
   if Arg(2,'o') = 1 then
       return SetBackground(self~oDlg~Adm, "COL", self~id, colorbk, -1)  /* creates an entry in the dialogs color table */
   else
       return SetBackground(self~oDlg~Adm, "COL", self~id, colorbk, colorfg)  /* set both, background and foreground color */

::method AssignFocus
   return SendWinMsg("ANY",self~hDlg, "0x0028", self~hwnd, 1)

::method GetFocus unguarded
   return Wnd_Desktop("GETFOC", self~hDlg)

::method SetFocus unguarded
   use arg hwnd
   return Wnd_Desktop("SETFOC", self~hDlg, hwnd)

::method Clear
   parse value WindowRect("GET", self~Hwnd) with r.1 r.2 r.3 r.4
   return WindowRect("CLR", self~oDlg~Adm, self~Hwnd,r.1,r.2,r.3,r.4)

::method ClearRect
   use arg left, top, right, bottom
   return WindowRect("CLR", self~oDlg~Adm, self~hwnd,left,top,right,bottom)

::method Value unguarded
    return self~oDlg~InternalGetItemData(self~Id, self~hDlg)

::method "Value=" unguarded
    use arg data
    self~oDlg~InternalSetItemData(self~Id, data, self~hDlg)

::method CaptureMouse
   return Wnd_Desktop("CAP", self~hDlg, self~hwnd)

::method GetMouseCapture
   return Wnd_Desktop("CAP", self~hDlg, "G")

::method ReleaseMouseCapture
   return Wnd_Desktop("CAP", self~hDlg, "R")

::method IsMouseButtonDown
   forward to (self~oDlg)

   /* Redraws the given rectangle */
::method RedrawRect unguarded
   use arg left, top, right, bottom, erasebkg
   if Arg(5, 'o') = 1 then erasebkg = 0
   return WindowRect("RDW", self~hwnd,left,top,right,bottom, erasebkg)

/****************************************************** Tree Control Class *********************************************************/


::class TreeControl subclass DialogControl public

::method RootArray attribute private

::method Init
   expose first, last, sort, root
   forward class (super) continue
   self~RootArray = .array~new(4)

   root = 2**16
   first = root - 1
   last = root - 2
   sort = root - 3

::method Insert
   expose first, last, sort, root
   use arg parent, after, text, image, selImage, opts, children
   if Arg(1,'o') = 1 then parent = root
   else if parent == "ROOT" then parent = root
   if Arg(2,'o') = 1 then after = "LAST"
   else if after = "LAST" then after = last
   else if after = "FIRST" then after = first
   else if after = "SORT" then after = sort
   if Arg(4,'o') = 1 then image = -1
   if Arg(5,'o') = 1 then selImage = image
   if Arg(6,'o') = 1 then opts = ""
   if Arg(7,'o') = 1 then children = 0
   return InsertTreeCtrl(self~Hwnd, parent, after, text, image, opts~translate, children, selImage)

::method Add
   do i = 1 to Arg()
      if Arg(i,'E') = 1 then do
          if Arg(i+1,'e') = 1 then image = Arg(i+1); else image = -1
          if Arg(i+2,'e') = 1 then selImage = Arg(i+2); else selImage = image
          if Arg(i+3,'e') = 1 then opts = Arg(i+3); else opts = ""
          if Arg(i+4,'e') = 1 then children = Arg(i+4); else children = 0  /* for dynamic children */
          if i=1 then do
              self~RootArray[i] = self~Insert("ROOT",,Arg(i),image,selimage,opts~translate, children)
              return self~RootArray[i]
          end
          else if self~RootArray~HasIndex(i-1)=1 then do
              if i=1 then parent = "ROOT"; else parent = self~RootArray[i-1]
              self~RootArray[i] = self~Insert(parent,,Arg(i),image,selimage,opts~translate, children)
              return self~RootArray[i]
          end
          else return 0
      end
   end


::method Modify
   use arg hItem, text, image, selImage, state, children
   if Arg(1,'o') = 1 then hItem = self~Selected
   if hitem = 0 then return -1
   if Arg(2,'o') = 1 then text = ""
   if Arg(3,'o') = 1 then image = -1
   if Arg(4,'o') = 1 then selImage = -1
   if Arg(5,'o') = 1 then state = ""
   if Arg(6,'o') = 1 then children = -1
   return SetTreeItem(self~Hwnd, hItem, text, image, state~translate, children, selImage)

::method ItemInfo
   use arg hItem
   if Arg(1,'o') = 1 | hItem = 0 then return -1
   ret = GetTreeItem(self~Hwnd, hItem, InternalTVItemInfo.)
   return InternalTVItemInfo.

::method MoveItem
   use arg hItem, hNewParent, redraw, extended
   if Arg(1,'o') = 1 | hItem = 0 then return 0
   if Arg(2,'o') = 1 | hNewParent = 0 then return 0
   if Arg(3,'o') = 1 then redraw = 1
   if Arg(4,'o') = 1 then extended = ""; else extended = extended~translate
   if hItem = hNewParent | hNewParent = self~Parent(hItem) | self~IsAncestor(hItem, hNewParent) = 1 then return 0
   iinfo. = self~ItemInfo(hItem)
   newRoot = self~Insert(hNewParent,,iinfo.!Text, iinfo.!Image, iinfo.!SelectedImage, iinfo.!State, iinfo.!Children)
   if iinfo.!Children = 1 then do
       child = self~Child(hItem)
       if child \= 0 then self~MoveItem(child, newRoot, 0, "SIBLINGS")
   end
   if extended = "SIBLINGS" then do
       sibling = self~next(hItem)
       do while sibling \= 0
           self~MoveItem(sibling, hNewParent, 0, "NODELETE")
           oldsib = sibling
           sibling = self~next(sibling)
           self~Delete(oldsib)
       end
   end
   if extended \= "NODELETE" then self~Delete(hItem)
   if redraw = 1 | redraw~left(1)~translate = "Y" then self~Update
   return newRoot  /* return new handle */


::method IsAncestor
   use arg hParent, hItem
   if Arg(1,'o') = 1 | hParent = 0 then return 0
   if Arg(2,'o') = 1 | hItem = 0 then return 0
   if hItem = hParent then return 0
   act = self~Parent(hItem)
   do while act \= 0 & act \= hParent
       act = self~Parent(act)
   end
   if act = hParent then return 1
   else return 0


::method Items
   return GetTreeItemCount(self~Hwnd)

::method VisibleItems
   return GetTreeItemVisibleCount(self~Hwnd)

::method Root
   return GetTreeItemHandle(self~Hwnd, 0 ,"ROOT")

::method Parent
   use arg hitem
   if Arg(1,'o') = 1 | hItem = 0 then return -1
   return GetTreeItemHandle(self~Hwnd, hitem, "PARENT")

::method Child
   use arg hitem
   if Arg(1,'o') = 1 | hItem = 0 then return -1
   return GetTreeItemHandle(self~Hwnd, hitem, "CHILD")

::method Selected
   return GetTreeItemHandle(self~Hwnd, 0, "CARET")

::method DropHighlighted
   return GetTreeItemHandle(self~Hwnd, 0, "DROP")

::method FirstVisible
   return GetTreeItemHandle(self~Hwnd, 0, "FIRSTVISIBLE")

::method Next
   use arg hitem
   if Arg(1,'o') = 1 | hItem = 0 then return -1
   return GetTreeItemHandle(self~Hwnd, hitem, "NEXT")

::method NextVisible
   use arg hitem
   if Arg(1,'o') = 1 | hItem = 0 then return -1
   return GetTreeItemHandle(self~Hwnd, hitem, "NEXTVISIBLE")

::method Previous
   use arg hitem
   if Arg(1,'o') = 1 | hItem = 0 then return -1
   return GetTreeItemHandle(self~Hwnd, hitem, "PREVIOUS")

::method PreviousVisible
   use arg hitem
   if Arg(1,'o') = 1 | hItem = 0 then return -1
   return GetTreeItemHandle(self~Hwnd, hitem, "PREVIOUSVISIBLE")

::method Delete
   use arg item
   if Arg(1,'o') = 1 then return -1
   if item \= 0 & item~Datatype("N") = 1 then return DeleteTreeItem(self~hwnd, item)
   else return -1

::method DeleteAll
   expose root
   return DeleteAllTreeItems(self~Hwnd)

::method Collapse
   use arg hitem
   if Arg(1,'o') = 1 | hItem = 0 then return -1
   return ExpandTreeItem(self~Hwnd, hitem, 2)

::method CollapseAndReset
   use arg hitem
   if Arg(1,'o') = 1 | hItem = 0 then return -1
   return ExpandTreeItem(self~Hwnd, hitem, 3)

::method Expand
   use arg hitem
   if Arg(1,'o') = 1 | hItem = 0 then return -1
   return ExpandTreeItem(self~Hwnd, hitem, 0)

::method Toggle
   use arg hitem
   if Arg(1,'o') = 1 | hItem = 0 then return -1
   return ExpandTreeItem(self~Hwnd, hitem, 1)

::method EnsureVisible
   use arg hitem
   if Arg(1,'o') = 1 | hItem = 0 then return -1
   return EnsureTreeItemVisible(self~Hwnd, hitem)

::method Indent
      return GetTreeItemIndent(self~Hwnd)

::method "Indent="
   use arg ind
   if Arg(1,'o') = 1 then return -1
   ret = SetTreeItemIndent(self~Hwnd, ind)

::method Edit
   use arg hitem
   if Arg(1,'o') = 1 | hItem = 0 then return -1
   return EditTreeItemLabel(self~Hwnd, hitem)

::method EndEdit
   use arg cancel
   if Arg(1,'o') = 1 then cancel = 0
   return EndTreeItemEdit(self~Hwnd, cancel)

::method SubclassEdit
   return SubclassTreeEditor(self~Hwnd)

::method RestoreEditClass
   return RestoreTreeEditor("RESUB_EDIT", self~Hwnd)

::method Select
   use arg hitem
   if Arg(1,'o') = 1 | hItem = 0 then return -1
   return SelectTreeItem(self~Hwnd, hitem, 0)    -- 0 is the caret

::method MakeFirstVisible
   use arg hitem
   if Arg(1,'o') = 1 | hItem = 0 then return -1
   return SelectTreeItem(self~Hwnd, hitem, 1)    -- 1 is first visible

::method DropHighlight
   use arg hitem
   if Arg(1,'o') = 1 then hItem = 0
   return SelectTreeItem(self~Hwnd, hitem, 2)    -- 2 is drop item


::method SortChildren
   use arg hitem
   if Arg(1,'o') = 1 | hItem = 0 then return -1
   return SortTreeItemChildren(self~Hwnd, hitem)


::method SetImages
   use arg bmp, cx, cy
   if Arg(1,'o') = 1 then return -1

   -- bitmap handles are pointers, so likely to be big...bump the digits level to check
   numeric digits digits() + 2
   if \datatype(bmp, 'Whole') then do
       bmp = self~LoadBitmap(bmp)
   end

   if Arg(2,'o') = 1 then cx = 0
   if Arg(3,'o') = 1 then cy = 0
   return SetTreeImages(self~Hwnd, bmp, cx, cy)


::method RemoveImages
   return UnsetTreeImages(self~Hwnd)

::method HitTest
   use arg x, y
   return TestTreeItemHit(self~Hwnd, x, y)


/****************************************************** List Control Class *********************************************************/

::class ListControl subclass DialogControl public

::method LastItem attribute

::method Init
   forward class (super) continue
   self~LastItem = 0

::method ReplaceStyle
   use arg oldstyle, newstyle
   if Arg(1,'o') = 1 | Arg(2,'o') = 1 then return 0
   ret = RemoveListStyle(self~Hwnd, oldstyle)
   return AddListStyle(self~Hwnd, newstyle)

::method AddStyle
   use arg style
   if Arg(1,'o') = 1 then return 0
   return AddListStyle(self~Hwnd, style)

::method RemoveStyle
   use arg style
   if Arg(1,'o') = 1 then return 0
   ret = RemoveListStyle(self~Hwnd, style)

::method InsertColumn
   use arg nr, text, width, fmt
   if Arg(1,'o') = 1 then nr = 0
   if Arg(3,'o') = 1 then width = -1; else width = width*self~FactorX
   if Arg(4,'o') = 1 then fmt = "LEFT"
   return InsertListColumn(self~Hwnd, nr, text, width, wordpos(fmt~translate, "LEFT RIGHT CENTER"))


::method DeleteColumn
   use arg nr
   if Arg(1,'o') = 1 then return -1
   return DeleteListColumn(self~Hwnd, nr)


::method ModifyColumn
   use arg nr, text, width, fmt
   if Arg(1,'o') = 1 then return -1
   if Arg(2,'o') = 1 then text = ""
   if Arg(3,'o') = 1 then width = -1; else width = width*self~FactorX
   if Arg(4,'o') = 1 then fmt = ""
   return ModifyListColumn(self~Hwnd, nr, text, width, wordpos(fmt~translate, "LEFT RIGHT CENTER"))


::method ColumnInfo
   use arg nr
   if Arg(1,'o') = 1 then return -1
   ret = GetListColumnInfo(self~Hwnd, nr, InternalLVColInfo.)
   if InternalLVColInfo.!Width~Datatype('N') = 1 then InternalLVColInfo.!Width = InternalLVColInfo.!Width / self~FactorX
   return InternalLVColInfo.


::method ColumnWidth
   use arg nr
   if Arg(1,'o') = 1 then return -1
   return GetListColumnWidth(self~Hwnd, nr) / self~FactorX

::method SetColumnWidth
   use arg nr, width
   auto = .false
   if Arg(1,'o') = 1 then return -1
   if Arg(2,'o') = 1 then width = "AUTO"
   if width~datatype("N") = 1 then do
       width = width * self~FactorX
       option = ""
   end
   else do
        style = width~translate
        width = 0;
   end
   return SetListColumnWidth(self~Hwnd, nr, width, style)


::method StringWidth
   use arg text
   if Arg(1,'o') = 1 then return -1
   return GetListItemStringWidth(self~Hwnd, text) / self~FactorX


::method Insert
   use arg nr, subnr, text, image
   if Arg(2,'o') = 1 then subnr = 0
   if Arg(1,'o') = 1 then do
        if subnr = 0 then nr = self~LastItem + 1
        else nr = self~LastItem
   end
   if Arg(4,'o') = 1 then image = -1
   if subnr = 0 then do
       item = InsertListItem(self~Hwnd, nr, text, image)
       if item \= -1 then self~LastItem = item
   end
   else item = SetListItem(self~Hwnd, nr, subnr, text, image)
   return item


::method Modify
   use arg nr, subnr, text, image
   if Arg(1,'o') = 1 then nr = self~Selected
   if Arg(2,'o') = 1 then subnr = 0
   if Arg(4,'o') = 1 then image = -1
   return SetListItem(self~Hwnd, nr, subnr, text, image)

::method SetItemText
   use arg item, subitem, text
   if Arg(1,'o') = 1 then return -1
   if Arg(2,'o') = 1 then subitem = 0
   return SetListItemText(self~Hwnd, item, subitem, text)

::method SetItemState
   use arg item, state
   if Arg(1,'o') = 1 then return -1
   return SetListItemState(self~Hwnd, item, state)


::method Add
   do i = 1 to Arg()
      if Arg(i,'E') = 1 then do
          if Arg(i+1,'e') = 1 then image = Arg(i+1); else image = -1
          if i = 1 then do
              item = self~Insert(self~LastItem+1,i-1,Arg(i),image)
              if item \= -1 then self~LastItem = item
          end
          else do
             item = self~Insert(self~LastItem,i-1,Arg(i),image)
          end
          return item
      end
   end
   return -1


::method AddRow
   use arg nr, image, text
   if Arg(1,'o') = 1 then nr = self~LastItem+1
   if Arg(2,'o') = 1 then image = -1
   if Arg(3,'o') = 1 then text = ""
   item = self~Insert(nr,0,text,image)
   if item \= -1 then do
       self~LastItem = item
       do i = 4 to Arg()
           if Arg(i,'E') = 1 then self~Insert(item,i-3,Arg(i),-1)
       end
   end
   return item


::method Delete
   use arg item
   if Arg(1,'o') = 1 then return -1
   return DeleteListItem(self~hwnd, item)

::method DeleteAll
   return DeleteAllListItems(self~Hwnd)

::method Items
   return GetListItemCount(self~Hwnd)

::method Last
   return self~Items - 1

::method Prepare4nItems
   use arg citems
   if Arg(1,'o') = 1 then return -1
   return SetListItemCount(self~Hwnd, citems)


::method SelectedItems
   return GetListSelectedCount(self~Hwnd)

::method ItemInfo
   use arg item, subitem
   if Arg(1,'o') = 1 then return -1
   if Arg(2,'o') = 1 then subitem = 0
   ret = GetListItem(self~Hwnd, item, subitem, InternalLVItemInfo.)
   return InternalLVItemInfo.

::method ItemText
   use arg item, subitem
   if Arg(1,'o') = 1 then return -1
   if Arg(2,'o') = 1 then subitem = 0
   return GetListItemText(self~Hwnd, item, subitem)

::method ItemState
   use arg item
   if Arg(1,'o') = 1 then return -1
   return GetListItemState(self~Hwnd, item)

::method Select
   use arg item
   if Arg(1,'o') = 1 then return -1
   return SetListItemState(self~Hwnd, item, "SELECTED")

::method Deselect
   use arg item
   if Arg(1,'o') = 1 then return -1
   return SetListItemState(self~Hwnd, item, "NOTSELECTED")

::method Selected
   return GetNextListItem(self~Hwnd, -1, "SELECTED")

::method LastSelected
   if self~ItemState(self~Last)~wordpos("SELECTED") > 0 then  return self~Last
   return self~PreviousSelected(self~Last)

::method Focused
   return GetNextListItem(self~Hwnd, -1, "FOCUSED")

::method Focus
   use arg item
   if Arg(1,'o') = 1 then return -1
   return SetListItemState(self~Hwnd, item, "FOCUSED")

::method DropHighlighted
   return GetNextListItem(self~Hwnd, -1, "DROP")

::method FirstVisible
   return GetNextListItem(self~Hwnd, 0, "FIRSTVISIBLE")

::method NextSelected
   use arg startItem
   if Arg(1,'o') = 1 then startItem = -1
   return GetNextListItem(self~Hwnd, startItem, "BELOW TORIGHT SELECTED")

::method PreviousSelected
   use arg startItem
   if Arg(1,'o') = 1 then startItem = -1
   return GetNextListItem(self~Hwnd, startItem, "ABOVE TOLEFT SELECTED")

::method Next
   use arg startItem
   -- The Windows API appears to have a bug when the list contains a single item, insisting on returning
   -- 0.  This, rather unfortunately, can cause some infinite loops because iterating code is looking for
   -- a -1 value to mark the iteration end.
   if self~LastItem < 2 then return -1
   if Arg(1,'o') = 1 then startItem = -1
   return GetNextListItem(self~Hwnd, startItem, "BELOW TORIGHT")

::method Previous
   use arg startItem
   -- The Windows API appears to have a bug when the list contains a single item, insisting on returning
   -- 0.  This, rather unfortunately, can cause some infinite loops because iterating code is looking for
   -- a -1 value to mark the iteration end.
   if self~LastItem < 2 then return -1
   if Arg(1,'o') = 1 then startItem = -1
   return GetNextListItem(self~Hwnd, startItem, "ABOVE TOLEFT")

::method NextLeft
   use arg startItem
   -- The Windows API appears to have a bug when the list contains a single item, insisting on returning
   -- 0.  This, rather unfortunately, can cause some infinite loops because iterating code is looking for
   -- a -1 value to mark the iteration end.
   if self~LastItem < 2 then return -1
   if Arg(1,'o') = 1 then startItem = -1
   return GetNextListItem(self~Hwnd, startItem, "TOLEFT")

::method NextRight
   use arg startItem
   -- The Windows API appears to have a bug when the list contains a single item, insisting on returning
   -- 0.  This, rather unfortunately, can cause some infinite loops because iterating code is looking for
   -- a -1 value to mark the iteration end.
   if self~LastItem < 2 then return -1
   if Arg(1,'o') = 1 then startItem = -1
   return GetNextListItem(self~Hwnd, startItem, "TORIGHT")

::method SmallSpacing
   return GetListSpacing(self~Hwnd, .true)

::method Spacing
   return GetListSpacing(self~Hwnd, .false)

::method RedrawItems
   use arg first, last
   if Arg(1,'o') = 1 then first = 0
   if Arg(2,'o') = 1 then last = self~items
   return RedrawListItems(self~Hwnd, first, last)

::method UpdateItem
   use arg item
   if Arg(1,'o') = 1 then return -1
   return UpdateListItem(self~Hwnd, item)

::method Update
   return WndShow_Pos("S",self~Hwnd, "UPDATE")

::method EnsureVisible
   use arg item, partial
   if Arg(1,'o') = 1 then return -1
   if Arg(2,'o') = 1 then partial = 0
   return EnsureListItemVisible(self~Hwnd, item, partial)

::method SetSmallImages
   use arg bmp, cx, cy
   if Arg(1,'o') = 1 then return -1
   if Arg(2,'o') = 1 then cx = 0
   if Arg(3,'o') = 1 then cy = 0

   -- bitmap handles are pointers, so likely to be big...bump the digits level to check
   numeric digits digits() + 2
   if \datatype(bmp, 'Whole') then do
       bmp = self~LoadBitmap(bmp)
   end

   return SetListImages(self~Hwnd, bmp, cx, cy, .true)

::method SetImages
   use arg bmp, cx, cy
   if Arg(1,'o') = 1 then return -1
   if Arg(2,'o') = 1 then cx = 0
   if Arg(3,'o') = 1 then cy = 0

   -- bitmap handles are pointers, so likely to be big...bump the digits level to check
   numeric digits digits() + 2
   if \datatype(bmp, 'Whole') then do
       bmp = self~LoadBitmap(bmp)
   end
   return SetListImages(self~Hwnd, bmp, cx, cy, .false)

::method RemoveSmallImages
   return UnsetListImages(self~Hwnd, .true)

::method RemoveImages
   return UnsetListImages(self~Hwnd, .false)

::method Find
   use arg txt, startItem, wrap
   if Arg(2,'o') = 1 then startItem = -1
   if Arg(3,'o') = 1 then wrap = 0
   if wrap = 1 | wrap~translate = "J" then opt = "WRAP"; else opt = ""
   return FindListItem(self~Hwnd, startItem, opt, txt)

::method FindPartial
   use arg txt, startItem, wrap
   if Arg(2,'o') = 1 then startItem = -1
   if Arg(3,'o') = 1 then wrap = 0
   if wrap = 1 | wrap~translate = "J" then opt = "PARTIAL WRAP"; else opt = "PARTIAL"
   return FindListItem(self~Hwnd, startItem, opt, txt)

::method FindNearestXY
   use arg x,y, direction
   if Arg(1,'o') = 1 | Arg(2,'o') = 1 then return -1
   if Arg(3,'o') = 1 then direction = "DOWN"
   return FindNearestListItem(self~Hwnd, x, y, direction~translate)

::method Arrange
   return ArrangeList(self~Hwnd, 4)

::method SnapToGrid
   return ArrangeList(self~Hwnd, 3)

::method AlignLeft
   return ArrangeList(self~Hwnd, 1)

::method AlignTop
   return ArrangeList(self~Hwnd, 0)

::Method ItemPos
   use arg item
   if Arg(1,'o') = 1 then return -1
   return GetItemPosition(self~Hwnd, item)

::Method SetItemPos
   use arg item, x, y
   if Arg(1,'o') = 1 then return -1
   if Arg(2,'o') = 1 then x = 0
   if Arg(3,'o') = 1 then y = 0
   return SetItemPosition(self~Hwnd, item, x, y)

::method Edit
   use arg hitem
   if Arg(1,'o') = 1 then return -1
   return EditListItem(self~Hwnd, hitem)

::method EndEdit
   return EndListItemEdit(self~Hwnd)

::method SubclassEdit
   return SubclassListEditor(self~Hwnd)

::method RestoreEditClass
   return RestorListEditor(self~Hwnd)

::method ItemsPerPage
   return GetListPerPageCount(self~Hwnd)

::method Scroll
   use arg x, y
   if Arg(1,'o') = 1 then x = 0
   if Arg(2,'o') = 1 then y = 0
   return ScollList(self~Hwnd, x, y)

::Method BkColor
   return GetListColor(self~Hwnd, 0)

::Method "BkColor="
   use arg color
   call SetListColor("COLOR", self~Hwnd, 0, color

::Method TextColor
   return GetListColor(self~Hwnd, 1)

::Method "TextColor="
   use arg color
   call SetListColor("COLOR", self~Hwnd, 1, color

::Method TextBkColor
   return GetListColor(self~Hwnd, 2)

::Method "TextBkColor="
   use arg color
   call SetListColor("COLOR", self~Hwnd, 2, color




/****************************************************** Progress Bar Class *********************************************************/


::class ProgressBar subclass DialogControl public

::method Step
   if Arg(1,'e') then
       return SetProgressDialogDeltaPos(self~hwnd, arg(1))
   else
       return StepProgressDialog(self~hwnd)

::method SetPos
   use arg newpos
   if Arg(1,'o') = 1 then return -1
   return SetProgressDialogPosition(self~hwnd, newpos)

::method SetStep
   use arg newstep
   if Arg(1,'o') = 1 then return -1
   return SetProgressDialogStep(self~hwnd, newstep)

::method SetRange
   use arg min, max
   if Arg(1,'o') = 1 then min = 0
   if Arg(2,'o') = 1 then max = 100
   if max < min then return 0
   prevrange =  SetProgressDialogRange(self~hwnd, min, max)
   if prevrange \= 0 then do
      pmin = BinaryAnd(prevrange, "0x0000FFFF")
      pmax = BinaryAnd(prevrange, "0xFFFF0000") % "10000"~x2d
      return pmin || " " || pmax
   end
   else return 0






/****************************************************** Track Bar Class *********************************************************/


::class SliderControl subclass DialogControl public

::method "Pos="
   forward message (SetPos)

::method Pos
   return GetSliderPosition(self~hwnd)

::method SetPos
   use arg p, redraw
   if Arg(1,'o') = 1 then return -1
   if Arg(2,'o') = 1 then redraw = 0
   call SetSliderPosition(self~hwnd, p, redraw)

::method InitRange
   use arg min, max, redraw
   if Arg(1,'o') = 1 then min = 0
   if Arg(2,'o') = 1 then max = 100
   if Arg(3,'o') = 1 then redraw = 0
   if max < min then return -1
   return SetSliderRange(self~hwnd, min, max, redraw)

::method SetMin
   use arg min, redraw
   if Arg(1,'o') = 1 then return -1
   if Arg(2,'o') = 1 then redraw = 1
   return SetSliderMin(self~hwnd, min, redraw)

::method SetMax
   use arg max, redraw
   if Arg(1,'o') = 1 then return -1
   if Arg(2,'o') = 1 then redraw = 1
   return SetSliderMax(self~hwnd, min, redraw)

::method Range
   return GetSliderRange(self~hwnd)

::method ClearTicks
   use arg redraw
   if Arg(1,'o') = 1 then redraw = 1
   return ClearSliderTicks(self~hwnd, redraw)

::method CountTicks
   return GetSliderTickCount(self~hwnd)

::method GetTick
   use arg tic
   if Arg(1,'o') = 1 then return -1
   return GetSliderTick(self~hwnd, tic)

::method SetTickAt
   use arg pos
   if Arg(1,'o') = 1 then return -1
   return SetSliderTick(self~hwnd, pos)

::method SetTickFrequency
   use arg freq
   if Arg(1,'o') = 1 then return -1
   return SetSliderTickFrequencey(self~hwnd, freq)

::method GetLineStep
   return GetSliderLineStep(self~hwnd)

::method GetPageStep
   return GetSliderPageStep(self~hwnd)

::method SetLineStep
   use arg step
   if Arg(1,'o') = 1 then return -1
   return SetSliderLineStep(self~hwnd, step)

::method SetPageStep
   use arg step
   if Arg(1,'o') = 1 then return -1
   return SetSliderPageStep(self~hwnd, step)

::method InitSelRange
   use arg min, max, redraw
   if Arg(1,'o') = 1 then min = 0
   if Arg(2,'o') = 1 then parse value self~Range with . max
   if Arg(3,'o') = 1 then redraw = 0
   if max < min then return -1
   return SetSliderSelRange(self~hwnd, min, max, redraw)

::method SetSelStart
   use arg min, redraw
   if Arg(1,'o') = 1 then return -1
   if Arg(2,'o') = 1 then redraw = 1
   return SetSliderSelStart(self~hwnd, min, redraw)

::method SetSelEnd
   use arg max, redraw
   if Arg(1,'o') = 1 then return -1
   if Arg(2,'o') = 1 then redraw = 1
   return SetSliderSelEnd(self~hwnd, max, redraw)

::method ClearSelRange
   use arg redraw
   if Arg(1,'o') = 1 then redraw = 1
   return ClearSliderSelRange(self~hwnd, redraw)

::method SelRange
   return GetSliderSelRange(self~hwnd)



/****************************************************** Tab Control Class *********************************************************/


::class TabControl subclass DialogControl public

::method LastItem attribute

::method Init
   forward class (super) continue
   self~LastItem = -1

::method Insert
   use arg nr, text, image, lparam
   if Arg(1,'o') = 1 then nr = self~LastItem + 1
   if Arg(2,'o') = 1 then text = ""
   if Arg(3,'o') = 1 then image = -1
   if Arg(4,'o') = 1 then lparam = 0
   item = InsertTabItem(self~Hwnd, nr, text, image, lparam)
   if item \= -1 then self~LastItem = item
   return item


::method Modify
   use arg nr, text, image, lparam
   if Arg(1,'o') = 1 then return -1
   if Arg(2,'o') = 1 then text = ""
   if Arg(3,'o') = 1 then image = -1
   if Arg(4,'o') = 1 then lparam = 0;
   else return ModifyTabItem(self~Hwnd, nr, text, image, lparam)


/* adds text1, text2, text3.... */
::method AddSequence
   nr = self~LastItem
   do i= 1 to Arg()
       nr = nr + 1
       nr = self~Insert(nr,Arg(i))
       if nr = -1 then return -1    /* error occurred */
       self~LastItem = nr
   end
   return nr

/* adds text1, image1, lparam1, text2, image2, lparam2, text3.... */
::method AddFullSeq
   args = Arg()
   nr = self~LastItem
   i = 1
   do while i <= args
       nr = nr + 1
       if i+2 <= args & Arg(i+2,'e') = 1 then do
           if Arg(i+1,'e') = 1 then nr = self~Insert(nr,Arg(i), Arg(i+1), Arg(i+2))
           else nr = self~Insert(nr,Arg(i),, Arg(i+2))
       end
       else if i+1 <= args & Arg(i+1,'e') = 1 then nr = self~Insert(nr,Arg(i), Arg(i+1))
       else nr = self~Insert(nr,Arg(i))
       if nr = -1 then return -1   /* error occurred */
       self~LastItem = nr
       i = i+3
   end
   return nr

::method Items
   return GetTabCount(self~Hwnd)

::method Rows
   return GetTabRowCount(self~Hwnd)

::method ItemInfo
   use arg hItem
   if Arg(1,'o') = 1 then return -1
   ret = GetTabItem(self~Hwnd, hItem, InternalTCItemInfo.)
   return InternalTCItemInfo.

::method Delete
   use arg item
   if Arg(1,'o') = 1 then return -1
   return DeleteTabItem(self~Hwnd, item)

::method DeleteAll
   return DeleteAllTabs(self~Hwnd)

::method Last
   return self~Items - 1

::method Selected
   return GetSelectedTabText(self~Hwnd)

::method SelectedIndex
   return GetSelectedTabIndex(self~Hwnd)

::method Select
   use arg text
   return SetSelectedTabByText(self~Hwnd, text)

::method SelectIndex
   use arg item
   return SetSelectedTab(self~Hwnd, item)

::method Focus
   use arg item
   return SetTabFocus(self~Hwnd, item)

::method Focused
   return GetTabFocus(self~Hwnd)

::method SetImages
   use arg bmp, cx, cy
   if Arg(1,'o') = 1 then return -1
   if Arg(2,'o') = 1 then cx = 0
   if Arg(3,'o') = 1 then cy = 0

   -- bitmap handles are pointers, so likely to be big...bump the digits level to check
   numeric digits digits() + 2
   if \datatype(bmp, 'Whole') then do
       bmp = self~LoadBitmap(bmp)
   end

   return SetTabImages(self~Hwnd, bmp, cx, cy)

::method RemoveImages
   return UnsetTabImages(self~Hwnd)

::method SetPadding
   parse arg cx, cy
   return SetTabPadding(self~Hwnd, cx, cy)

::method SetSize
   parse arg cx, cy
   return SetTabSize(self~Hwnd, cx, cy)

::method PosRectangle
   use arg item
   if Arg(1,'o') = 1 then return ""
   return GetTabItemRectangle(self~Hwnd, item)

::method AdjustToRectangle
   use arg left, top, right, bottom
   return AdjustTab(self~Hwnd, .true1, left, top, right, bottom)

::method RequiredWindowSize
   use arg left, top, right, bottom
   return AdjustTab(self~Hwnd, .false, left, top, right, bottom)


/**********                                    Old controls                                         ****************************/

::class StaticControl subclass DialogControl public

/****************************************************** Edit Control Class *********************************************************/

::class EditControl subclass DialogControl public

::Method Selected
    selndx = self~Message2Parent(0x00B0, 0, 0)
    end = BinaryAnd(selndx, 0xFFFF0000) % X2D("10000")
    start = BinaryAnd(selndx, 0x0000FFFF)
    return start+1" "end+1

::Method Select
    use arg start, end
    if Arg(1,'o') = 1 then start = 1
    if Arg(2,'o') = 1 then end = 0
    self~Message2Parent(0x00B1, start-1, end-1)

::Method ScrollCommand
    use arg kind, reps
    if Arg(1,'o') = 1 then kind = "UP"; else kind = kind~translate
    if Arg(2,'o') = 1 then reps = 1
    select
        when kind = "UP" | kind = "LEFT" then sb = 0
        when kind = "DOWN" | kind = "RIGHT" then sb = 1
        when kind = "PAGEUP" | kind = "PAGELEFT" then sb = 2
        when kind = "PAGEDOWN" | kind = "PAGERIGHT" then sb = 3
    end
    do i = 1 to reps
        self~Message2Parent(0x00B5, sb, 0)
    end

::Method LineScroll
    use arg cx, cy
    return \self~Message2Parent(0x00B6, cx, cy)

::Method EnsureCaretVisibility
    return \self~Message2Parent(0x00B7, 0, 0)

::Method IsModified
    return self~Message2Parent(0x00B8, 0, 0)

::Method SetModified
    use arg bool
    if Arg(1,'o') = 1 then bool = 1
    self~Message2Parent(0x00B9, bool, 0)

::Method Lines
    return self~Message2Parent(0x00BA, 0, 0)

::Method LineIndex
    use arg line
    if Arg(1,'o') = 1 then line = 0
    return self~Message2Parent(0x00BB, line-1, 0) +1

::Method LineLength
    use arg line
    if Arg(1,'o') = 1 then line = -1; else line = line-1
    ndx = self~Message2Parent(0x00BB, line, 0)
    return self~Message2Parent(0x00C1, ndx, 0)

::Method ReplaceSelText
    use arg text
    if Arg(1,'o') = 1 then return -1
    self~Message2Parent(0x00C2, 1, "T" || text)

::Method SetLimit
    use arg limit
    self~Message2Parent(0x00C5, limit, 0)

::Method LineFromIndex
    use arg ndx
    if Arg(1,'o') = 1 then ndx = -1; else ndx = ndx -1
    return self~Message2Parent(0x00C9, ndx, 0) +1

::Method "PasswordChar="
    use arg char
    self~Message2Parent(0x00CC, char~c2d, 0)

::Method PasswordChar
    char = self~Message2Parent(0x00D2, 0, 0)
    if char \= 0 then return char~d2c
    else return ""

::Method FirstVisibleLine
    return self~Message2Parent(0x00CE, 0, 0)

::Method SetReadOnly
    use arg bool
    if Arg(1,'o') = 1 then bool = 1
    return \self~Message2Parent(0x00CF, bool, 0)

::Method SetMargins
    use arg left, right
    flag = 0
    if Arg(1,'o') = 1 & Arg(2,'o') = 1 then flag = "0xFFFF" /* no arguments = auto margins */
    else do
        if Arg(1,'e') = 1 then flag = flag + 1
        if Arg(2,'e') = 1 then flag = flag + 2
    end
    self~Message2Parent(0x00D3, flag, right*X2D("10000") + left)

::Method Margins
    marg = self~Message2Parent(0x00D4, 0, 0)
    right = BinaryAnd(marg, 0xFFFF0000) % X2D("10000")
    left = BinaryAnd(marg, 0x0000FFFF)
    return left" "right

::method GetLine
    use arg line, maxlen
    if Arg(1,'o') = 1 then line = 0
    if Arg(2,'o') = 1 then maxlen = 255
    return self~Message2Parent(0x00C4, line-1, "G" || maxlen)

::method "Tab="
    use arg dlgunits
    self~Message2Parent(0x00CB, 1, "L" || dlgunits)


/****************************************************** ButtonControl Classes *********************************************************/

::class ButtonControl subclass DialogControl public

::Method State
    ch = self~Message2Parent(0x00F2, 0, 0)
    st = ""
    if BinaryAnd(ch,1) \= 0  then st = "CHECKED"; else st = "UNCHECKED"
    if BinaryAnd(ch,2) \= 0 then st = st || " INDETERMINATE"
    if BinaryAnd(ch,4) \= 0 then st = st || " PUSHED"
    if BinaryAnd(ch,8) \= 0 then st = st || " FOCUS"
    return st

::Method "State="
    use arg sttext
    sttext = sttext~Translate
    st = 0
    if sttext~wordpos("CHECKED")>0 then st = 1
    if sttext~wordpos("INDETERMINATE")>0 then st = st + 2
    if sttext~wordpos("PUSHED")>0 then st = st + 4
    if sttext~wordpos("FOCUS")>0 then st = st + 8
    self~Message2Parent(0x00F3, st, 0)

::Method GetImage
    return self~Message2Parent(0x00F6, 0, 0)

::Method SetImage
    use arg img
    return self~Message2Parent(0x00F7, 0, img)

::Method "Style="
    use arg sttext
    st = 0
    sttext = sttext~Translate
    if sttext~wordpos("DEFPUSHBUTTON")>0 then st = 1
    if sttext~wordpos("CHECKBOX")>0 then st = 2
    if sttext~wordpos("AUTOCHECKBOX")>0 then st = 3
    if sttext~wordpos("RADIOBUTTON")>0 then st = 4
    if sttext~wordpos("3STATE")>0 then st = 5
    if sttext~wordpos("AUTO3STATE")>0 then st = 6
    if sttext~wordpos("GROUPBOX")>0 then st = 7
    if sttext~wordpos("AUTORADIOBUTTON")>0 then st = 9
    if sttext~wordpos("OWNERDRAW")>0 then st = st + "B"~X2D
    if sttext~wordpos("LEFTTEXT")>0 then st = st + "20"~X2D
    if sttext~wordpos("BITMAP")>0 then st = st + "80"~X2D
    if sttext~wordpos("LEFT")>0 then st = st + "100"~X2D
    if sttext~wordpos("RIGHT")>0 then st = st + "200"~X2D
    if sttext~wordpos("CENTER")>0 then st = st + "300"~X2D
    if sttext~wordpos("TOP")>0 then st = st + "400"~X2D
    if sttext~wordpos("BOTTOM")>0 then st = st + "800"~X2D
    if sttext~wordpos("VCENTER")>0 then st = st + "C00"~X2D
    if sttext~wordpos("PUSHLIKE")>0 then st = st + "1000"~X2D
    if sttext~wordpos("MULTILINE")>0 then st = st + "2000"~X2D
    if sttext~wordpos("NOTIFY")>0 then st = st + "4000"~X2D
    if sttext~wordpos("FLAT")>0 then st = st + "8000"~X2D
    self~Message2Parent(0x00F4, st, 1)


::method ChangeBitmap unguarded
   arga = Arg(1,"A")
   newarg = .array~new(arga~Items+1)
   newarg[1] = self~ID
   do i = 1 to arga~Items; if arga~hasindex(i) = 1 then newarg[i+1] = arga[i]; end
   forward to (self~oDlg) message "ChangeBitmapButton" Arguments (newarg)

::method DisplaceBitmap unguarded
   use arg x, y
   return self~oDlg~DisplaceBitmap(self~ID, x, y)

::method GetBmpDisplacement unguarded
   return self~oDlg~GetBmpDisplacement(self~ID)

::method ScrollText unguarded
   arga = Arg(1,"A")
   newarg = .array~new(arga~Items+1)
   newarg[1] = self~hwnd
   do i = 1 to arga~Items; if arga~hasindex(i) = 1 then newarg[i+1] = arga[i]; end
   forward to (self~oDlg) Arguments (newarg)

   /* This method moves the rectangle within a button and redraws the uncovered area */
::method Scroll unguarded
   use arg xPos, yPos, left, top, right, bottom
   return ScrollTheWindow(self~oDlg~Adm, self~hwnd, xPos, yPos, left, top, right, bottom)

::method GetBitmapSizeX unguarded
   return self~oDlg~GetBitmapSizeX(self~ID)

::method GetBitmapSizeY unguarded
   return self~oDlg~GetBitmapSizeY(self~ID)

   /* This method will draw the bitmap of a button */
   /* Use this method to move a bitmap or a part of it... */

::method DrawBitmap unguarded
   arga = Arg(1,"A")
   newarg = .array~new(arga~Items+2)
   newarg[1] = self~hwnd
   newarg[2] = self~id
   do i = 1 to arga~Items; if arga~hasindex(i) = 1 then newarg[i+2] = arga[i]; end
   forward to (self~oDlg) Arguments (newarg)


   /* This method will draw the bitmap step by step */

::method DimBitmap unguarded
   arga = Arg(1,"A")
   newarg = .array~new(arga~Items+1)
   newarg[1] = self~id
   do i = 1 to arga~Items; if arga~hasindex(i) = 1 then newarg[i+1] = arga[i]; end
   forward to (self~oDlg) Arguments (newarg)


::method ScrollBitmapFromTo unguarded
   arga = Arg(1,"A")
   newarg = .array~new(arga~Items+1)
   newarg[1] = self~id
   do i = 1 to arga~Items; if arga~hasindex(i) = 1 then newarg[i+1] = arga[i]; end
   forward to (self~oDlg) Arguments (newarg)


::class RadioButton subclass ButtonControl public

::Method IsChecked
    ch = self~Message2Parent(0x00F0, 0, 0)
    if ch = 0 then return "UNCHECKED"
    else if ch = 1 then return "CHECKED"
    else if ch = 2 then return "INDETERMINATE"
    else return "UNKNOWN"

::Method Check
    return self~Message2Parent(0x00F1, 1, 0)

::Method Uncheck
    return self~Message2Parent(0x00F1, 0, 0)

::Method Indeterminate
    return self~Message2Parent(0x00F1, 2, 0)

::class CheckBox subclass RadioButton public


/****************************************************** List Box Class *********************************************************/

::class ListBox subclass DialogControl public
::method Init
expose attributes

Forward Class(Super) continue

attributes = .directory~new
attributes~READWRITE = 0
attributes~READONLY   = 1
attributes~HIDDEN     = 2
attributes~SYSTEM     = 4
attributes~DIRECTORY  = 16
attributes~ARCHIVE    = 32
attributes~EXCLUSIVE  = 32768

::method TranslateAttributes class
expose attributes
use arg list


value = 0
do i = 1 to words(list)
   value = value + attributes[word(list)]
end

if value \= 0 then value = value + attributes~EXCLUSIVE

return value

::method Add
   use arg data
   if Arg(1,'o') = 1 then return -1
   return self~Message2Parent(0x00000180, 0, "T" || data) +1

::method Insert
   use arg index, data
   if Arg(2,'o') = 1 then return -1
   if Arg(1,"o") = 1 then index = self~SelectedIndex
   return self~Message2Parent(0x00000181, index-1, "T" || data) + 1

::method Delete
   use arg index
   if Arg(1,"o") = 1 then index = self~SelectedIndex
   return self~Message2Parent(0x00000182, index-1, 0)

::method DeleteAll
   return self~Message2Parent(0x00000184, 0, 0)

::method Find
   use arg dataString, ndx, how
   if Arg(1,'o') = 1 then return -1
   if Arg(2,'o') = 1 then ndx = 0
   if Arg(3,'e') = 1 & (how = 1 | how~left(1)~translate = "E") then
       index = self~Message2Parent(0x000001A2, ndx-1, "T" || dataString)  /* find exactly */
   else
       index = self~Message2Parent(0x0000018F, ndx-1, "T" || dataString)  /* find prefix */
   if index >= 0 then return index + 1; else return 0

::method SelectedIndex
   return self~Message2Parent(0x00000188, 0, 0) + 1

::method Selected
   return self~GetText(self~SelectedIndex)

::method SelectIndex
   use arg ndx
   if arg(1,'o') = 1 then ndx = 0
   return self~Message2Parent(0x00000186, ndx-1, 0)

::method DeSelectIndex
   use arg ndx
   if arg(1,'o') = 1 then ndx = 0
   return self~Message2Parent(0x00000186, ndx-1, "D")

::method Select
   use arg text
   if Arg(1,'o') = 1 then return -1
   index = self~Message2Parent(0x0000018F, 0, "T" || text)
   if index < 0 then return 0
   return self~Message2Parent(0x00000186, index, 0)

::method SelectRange
   use arg fromNdx, toNdx
   if Arg(1,'o') = 1 then fromNdx = 1
   if Arg(2,'o') = 1 then toNdx = self~Items

   lparam = (toNdx-1) * "10000"~x2d + (fromNdx-1)
   return self~Message2Parent(0x0000019B, 1, lparam)


::method DeselectRange
   use arg fromNdx, toNdx
   if Arg(1,'o') = 1 then fromNdx = 1
   if Arg(2,'o') = 1 then toNdx = self~Items

   lparam = (toNdx-1) * "10000"~x2d + (fromNdx-1)
   return self~Message2Parent(0x0000019B, 0, lparam)


::method Items
   return self~Message2Parent(0x0000018B, 0, 0)

::method SelectedItems
   return self~Message2Parent(0x00000190, 0, 0)

::method SelectedIndexes
   return self~oDlg~GetMultiList(self~ID)

::method MakeFirstVisible
   use arg ndx
   if arg(1,'o') = 1 then ndx = 1
   return self~Message2Parent(0x00000197, ndx-1, 0)

::method GetFirstVisible
   return self~Message2Parent(0x0000018E, 0, 0) + 1

::method GetText
   use arg ndx
   len = SendWinMsg("DLG", Self~hDlg, self~id, 0x0000018A, ndx-1,0)
   if len <= 0 then return ""
   else return SendWinMsg("PTR", Self~hDlg, self~id, 0x00000189, ndx-1,"G" || len+1)

::method Modify
   use arg index, dataString
   if Arg(1,"o") = 1 then index = self~SelectedIndex
   if index <= 0 then return -1
   self~Delete(index)
   return self~Insert(index, dataString)

::method SetTabulators
   call SetLBTabStops, self~hDlg, self~id, arg(1, 'A')

::method AddDirectory
   use arg drvPath, fileAttributes
   if Arg(2,"o") = 1 then fileAttributes = "READWRITE"
   opts = self~translateAttributes(fileAttributes)
   if opts \= 0 then opts = opts + EXCLUSIVE
   return self~Message2Parent(0x0000018D, opts, "T" || drvpath) + 1

::method SetWidth
   use arg dlgunits
   if Arg(1,'o') = 1 | dlgunits~datatype("N") = 0 then return -1
   self~Message2Parent(0x00000194, dlgunits*self~FactorX, 0)

::method Width
   return self~Message2Parent(0x00000193, 0, 0)/self~FactorX

::method "ItemHeight="
   use arg dlgunits
   if Arg(1,'o') = 1 | dlgunits~datatype("N") = 0 then return -1
   self~Message2Parent(0x000001A0, 0, dlgunits*self~FactorY)

::method ItemHeight
   return self~Message2Parent(0x000001A1, 0, 0)/self~FactorY

::method "ColumnWidth="
   use arg dlgunits
   if Arg(1,'o') = 1 | dlgunits~datatype("N") = 0 then return -1
   self~Message2Parent(0x00000195, dlgunits*self~FactorX, 0)



/****************************************************** Combo Box Class *********************************************************/


::class ComboBox subclass DialogControl public

::method Init
expose attributes
Forward Class(Super) continue

attributes = .directory~new
attributes~READWRITE = 0
attributes~READONLY   = 1
attributes~HIDDEN     = 2
attributes~SYSTEM     = 4
attributes~DIRECTORY  = 16
attributes~ARCHIVE    = 32
attributes~EXCLUSIVE  = 32768

::method TranslateAttributes class
expose attributes
use arg list

value = 0
do i = 1 to words(list)
   value = value + attributes[word(list)]
end

if value \= 0 then value = value + attributes~EXCLUSIVE

return value

::method Add
   use arg data
   if Arg(1,'o') = 1 then return -1
   return self~Message2Parent(0x00000143, 0, "T" || data) + 1

::method Insert
   use arg index, data
   if Arg(2,'o') = 1 then return -1
   if Arg(1,"o") = 1 then index = self~SelectedIndex
   return self~Message2Parent(0x0000014A, index-1, "T" || data) + 1

::method Delete
   use arg index
   if Arg(1,"o") = 1 then index = self~SelectedIndex
   return self~Message2Parent(0x00000144, index-1, 0)

::method DeleteAll
   return self~Message2Parent(0x0000014B, 0, 0)

::method Find
   use arg dataString, ndx, how
   if Arg(1,'o') = 1 then return -1
   if Arg(2,'o') = 1 then ndx = 0
   if Arg(3,'e') = 1 & (how = 1 | how~left(1)~translate = "E") then
       index = self~Message2Parent(0x00000158, ndx-1, "T" || dataString)  /* find exactly */
   else
       index = self~Message2Parent(0x0000014C, ndx-1, "T" || dataString)    /* find prefix */
   if index >= 0 then return index + 1; else return 0


::method SelectedIndex
   return self~Message2Parent(0x00000147, 0, 0) + 1

::method Selected
   return self~GetText(self~SelectedIndex)

::method SelectIndex
   use arg ndx
   if arg(1,'o') = 1 then ndx = 0
   return self~Message2Parent(0x0000014E, ndx-1, 0)

::method Select
   use arg text
   if Arg(1,'o') = 1 then return -1
   index = self~Message2Parent(0x0000014C, 0, "T" || text)
   if index < 0 then return 0
   return self~Message2Parent(0x0000014E, index, 0)

::method Items
   return self~Message2Parent(0x00000146, 0, 0)

::method GetText
   use arg ndx
   len = SendWinMsg("DLG", Self~hDlg, self~id, 0x00000149, ndx-1,0)
   if len <= 0 then return ""
   else return SendWinMsg("PTR", Self~hDlg, self~id, 0x00000148, ndx-1,"G" || len+1)

::method Modify
   use arg index, dataString
   if Arg(1,"o") = 1 then index = self~SelectedIndex
   if index <= 0 then return -1
   self~Delete(index)
   return self~Insert(index, dataString)

::method OpenDropDown
   self~Message2Parent(0x0000014F, 1,0)

::method CloseDropDown
   self~Message2Parent(0x0000014F, 0, 0)

::method IsDropDownOpen
   return self~Message2Parent(0x00000157, 0, 0)

::method EditSelection
   use arg startndx, endndx
   if Arg(1,'o') = 1 then startNdx = 0
   if Arg(2,'o') = 1 then endNdx = 0
   lparam = (endndx-1) * "10000"~x2d + (startndx -1)
   return (self~Message2Parent(0x00000142, 0, lparam)\=1)



::method AddDirectory
   use arg drvPath, fileAttributes
   if Arg(2,"o") = 1 then fileAttributes = "READWRITE"
   opts = translateAttributes(fileAttributes)
   return self~Message2Parent(0x00000145, opts, "T" || drvpath) + 1



/****************************************************** ScrollBar Class *********************************************************/


::class ScrollBar subclass DialogControl public

::method SetRange
   use arg min, max, redraw
   if Arg(3, "o") = 1 then redraw = 1
   return HandleScrollBar("SR",self~hwnd,min,max, redraw)

::method Range
   return HandleScrollBar("GR",self~hwnd)

::method SetPos
   use arg pos, redraw
   if Arg(2, "o") = 1 then redraw = 1
   return HandleScrollBar("SP",self~hwnd,pos, redraw)

::method Position
   return HandleScrollBar("GP",self~hwnd)

::method DeterminePosition
   use arg posdata, single, page
   if Arg(2,"o") = 1 then single = 1
   if Arg(3,"o") = 1 then page = 10
   code = BinaryAnd(posdata, 0x0000FFFF)
   parse value self~Range with rmin rmax
   pos = self~Position
   select
      /* Line up */
      when code = 0 then pos = max(rmin,pos - single)
      /* Line down */
      when code = 1 then pos = min(rmax,pos + single)
      /* page up */
      when code = 2 then pos = max(rmin,pos - page)
      /* page down */
      when code = 3 then pos = min(rmax,pos + page)
      /* track position */
      when code = 4 then pos = BinaryAnd(posdata, 0xFFFF0000) % X2D("10000")
      /* tracking */
      when code = 5 then pos = BinaryAnd(posdata, 0xFFFF0000) % X2D("10000")
      /* top */
      when code = 6 then pos = rmin
      /* bottom */
      when code = 7 then pos = rmax
      otherwise nop;
   end
   self~SetPos(pos)
   return pos
