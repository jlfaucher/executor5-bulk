/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2011-2013 Rexx Language Association. All rights reserved.    */
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

   Support - DragMgr					 	 v01-00  03Jun13
   ---------------------
   A singleton component that manages direct manipulation.

   Notes: A 'source' view is a dialog that's "picked up" by pressing and holding
          button 1 (usually the left button) of the mouse.
          A 'target' view is a dialog that's dropped onto by releasing 
          button 1 over the target dialog. 

   Interface DragMgr {
     setTarget
     setSource
     removeDlg
     queryTables
     pickup 	  captures mouse - i.e. sets the mouse capture to the window
           	  of this mouse instance. 
     moving	  If over a target (as supered by a dlg instance):
                  - check if this is a valid target AND is topmost window
		  - If over a valid drop area send dmQueryDrop(sourceDlg,mousePos)
		    to target.
		  - If response is .true then show dropOKCursor
		    else show noDropCursor (system-provided).
     drop	  - If cursor is no-drop then no-op.
		    Else send dmDrop(sourceDlg) to drop target. 
     }

  Changes:
    v01-00 03Jun13: First version.

  = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */

::REQUIRES ooDialog.cls

/*//////////////////////////////////////////////////////////////////////////////
  ============================================================================*/
::CLASS DragMgr PUBLIC

  ::ATTRIBUTE targetDialogs PRIVATE  -- dlg - hwnd, dropArea
  ::ATTRIBUTE sourceDialogs PRIVATE  -- dlg - mouse, dropOkCursor, pickupArea
  ::ATTRIBUTE dragInProgress
  --::ATTRIBUTE draggingSourceDlg PRIVATE
  
    
  /*----------------------------------------------------------------------------
    init - initialises the Drag Manager
    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  ::METHOD init
    expose icons dragging cursorIsNoDrop noDropCursor
    .local~my.DragMgr = self
    say "DragMgr-init-01: .local~my.DragMgr =" .local~my.DragMgr 
    self~dragInProgress = .false
    self~targetDialogs = .Table~new	-- Index = dlg id
    					-- Item = an Array: hwnd, dropArea,
    self~sourceDialogs = .Table~new	-- Index = dlg id,
    					-- Item = an Array: mouse, cursor, pickupArea
    noDropCursor = .Mouse~loadCursor("NO")
    if noDropCursor == 0 then do
      say "DragManager-init-02:" .HRSdm~badNoDropCursor .SystemErrorCode
    end

    cursorIsNoDrop = .false
    dragging       = .false

    return self

    
  /*----------------------------------------------------------------------------
    setTarget - Adds a target view component to the Targets table
    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  ::METHOD setTarget PUBLIC
    --expose targetDialogs
    use strict arg dlg, hwnd, dropArea
    items = .Array~new
    items[1] = hwnd;  items[2] = dropArea
    self~targetDialogs[dlg] = items

    
  /*----------------------------------------------------------------------------
    setSource - Adds a source view component to the Targets table
    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  ::METHOD setSource PUBLIC
    --expose sourceDialogs dropOkCursor
    use strict arg dlg, mouse, cursorFile, pickupArea
    -- cursorFile is relative path and filename e.g. "bmp\customer.cur".
    --say "DragManager-setSource: sourceArea =" pickupArea

    dropOkCursor = .Mouse~loadCursorFromFile(cursorFile)
    if dropOkCursor == 0 then do
      say "DragManager-init:" .HRSdm~badOkCursor .SystemErrorCode
    end

    items    = .Array~new
    items[1] = mouse;  items[2] = dropOkCursor;  items[3] = pickupArea
    self~sourceDialogs[dlg] = items

    
  /*----------------------------------------------------------------------------
    RemoveDlg - Removes a dialog from the Targets table when the dialog closes.
    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  ::METHOD removeDlg PUBLIC
    use strict arg dlg
    -- Check in SourceDlgs:
    self~sourceDialogs~remove(dlg)
    self~targetDialogs~remove(dlg)

    
  /*----------------------------------------------------------------------------
    pickup - Handles drag initiation when the user "picks up" a dialog.
    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  ::METHOD pickup PUBLIC
    expose cursorIsNoDrop dragging dropOkCursor mouse noDropCursor oldCursor overTarget
    use strict arg sourceDlg, keyState, mousePos

    say "------------------------------------------------------------"
    arrItems = self~sourceDialogs[sourceDlg]  	-- mouse,srcCursor,pickupArea
    mouse = arrItems[1]
    dropOkCursor = arrItems[2]
    dragging = .false

    if keyState \== 'lButton' | \ mousePos~inRect(arrItems[3]) then return .false

    if mouse~dragDetect(mousePos) then do
      --say "DragManager-pickup-01: dropOkCursor =" dropOkCursor
      if dropOkCursor == 0 then do
        say "DragManager-pickup-02:" .HRSdm~badOkCursor .SystemErrorCode
      end
      say 'DragManager-pickup-03: Detected dragging.'
      mouse~capture()
      oldCursor = mouse~setCursor(noDropCursor)
      cursorIsNoDrop = .true
      dragging       = .true
      validTarget = .false
      overTarget = .false
    end
--say "DragManager-pickup-04: dragging =" dragging
    return

    
  /*----------------------------------------------------------------------------
    moving - Handles mouse movements - detects when mouse over a dilaog, and if
             it's a 'target' dialog, asks it whether it will accept a drop.
    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  ::METHOD moving PUBLIC
    expose cursorIsNoDrop dragging dropOkCursor mouse noDropCursor overTarget dropTarget
    use arg sourceDlg, keyState, mousePos
    -- overTarget - true when the cursor is over a potential target dialog,
    --   regardless of whether the target refuses a drop or not.
    -- sourceDlg is the dialog that has captured the mouse.
    -- Note: new APIs introduced in ooDialog Build 7449

    if \dragging then return
    --say "Flags:" cursorIsNoDrop dragging overTarget
    -- Find out if mouse is over a target dialog:
    targetDlg = 0
    do i over self~targetDialogs		-- items: 1 = hwnd, 2 = droparea
      targetHwnd = self~targetDialogs[i][1]
      droparea   = self~targetDialogs[i][2]

      -- Prevent showing drop when over a target that is under another window:
      screenPt = mousePos~copy()			-- Point in source dialog (i.e. top left of dlg is 0,0)
      --say "DragManager-moving-01: screenPt =" screenPt
      sourceDlg~client2screen(screenPt)			-- screenPt is set to screen position (i.e. top left of screen is 0,0)
      --say "DragManager-moving-02: screenPt =" screenPt
      screenHwnd = .DlgUtil~windowFromPoint(screenPt)	-- Get hwd of topmost window that mouse is over
      --say "DragManager-moving-03: screenHwnd =" screenHwnd

      pDlg = mousePos~copy
      sourceDlg~mapWindowPoints(targetHwnd, pDlg)	-- MousePos relative to targetHwnd
      --say "DragManager-moving-04: pDlg =" pDlg
      childHwnd = i~childWindowFromPoint(pDlg)		-- Get the hwnd visible under mouse pointer
      --say "DragManager-moving-05: childHwnd / pDlg =" childHwnd "/" pDlg

      if screenHwnd == childHwnd then do
      	p1 = mousePos~copy()
      	sourceDlg~mapWindowPoints(targetHwnd,p1)
      	--say "DragManager-moving-06: p1 =" p1
        if p1~inRect(droparea) then do
          targetDlg = i
        -- next two statements for debug only
          --tgtNumber = i~queryNumber
          --say "DragManager-moving-07: Target found - hwnd =" targetHwnd tgtNumber
          leave					-- Stop looping - target found!
        end
      end
    end

    if targetDlg \= 0 then do			-- If we're over a target
      if targetDlg = sourceDlg then return	-- If target is also the source
      	      					--   then it's not a target (in this version!).
      if \overTarget then do			-- if first time over target
        r = targetDlg~dmQueryDrop(sourceDlg,mousePos)
        say "DragManager-moving-07: first time - queryDrop returned" r
        if r = .true then do			-- if target accepts a drop
          validTarget = .true
          dropTarget = targetDlg		-- for drop method ...
        end
        else do					-- if target refuses drop
          validTarget = .false
          overTarget = .true
          dropTarget = .false
          return
        end
        -- It's a valid target!
        overTarget = .true
        mouse~setCursor(dropOkCursor)
        cursorIsNoDrop = .false
      end
    end
    else do					-- Not over a target
      if overTarget then do			-- Mouse has just come off target
        validTarget = .false
        overTarget = .false
        mouse~setCursor(noDropCursor)
        cursorIsNoDrop = .true
        dropTargetDlg = 0
      end
    end



  /*----------------------------------------------------------------------------
    drop - Handles things when the user drops onto a target.
    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  ::METHOD drop PUBLIC
    expose cursorIsNoDrop dragging mouse oldCursor dropTarget
    use strict arg sourceDlg, keyState, mousePos

    say "DragManager-drop-01: the mouse is at ("mousePos~x", "mousePos~y")"
    say "DragManager-drop-02: cursorIsNoDrop =" cursorIsNoDrop

    if dragging then do
      okayToDrop = (cursorIsNoDrop \== .true)	-- if cind = .false then okToDrop is true;
              					-- if cind = .true then okToDrop is false
      dragging = .false
      cursorIsNoDrop = .false
      mouse~releaseCapture()
      mouse~setCursor(oldCursor)
      -- Jiggle the mouse so it is redrawn immediately (OS: seems to make no diff.)
      p = mouse~getCursorPos; p~incr; mouse~setCursorPos(p)

      if okayToDrop then do
      	say "DragManager-drop-03: sourceDlg =" sourceDlg
        dropTarget~dmDrop(sourceDlg)
        say "DragManager-drop-04: Drop Happened OK!!"
      end
      else say "DragManager-drop-05: Drop Did Not Occur."
    end

    return

    
  /*----------------------------------------------------------------------------
    queryTables - A debug mehod that lists source and target dialogs 
                  on the console.
    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  ::METHOD queryTables PUBLIC		-- Debug method
    say; say "======================================="
    say "DragManager Tables"
    say "-------------------------------------"
    say "Sources:"
    --say "asking dlg =" dlg
    do i over self~SourceDialogs
      say "i =" i
      items = self~SourceDialogs[i]
      say "mouse  = " items[1]
      say "cursor = " items[2]
      say "area   = " items[3]
    end
    say "-------------------------------------"
    say "Targets:"
    arr = self~targetDialogs[dlg]
    do i over self~targetDialogs
      say "i =" i
      items = self~TargetDialogs[i]
      say "hwnd     = " items[1]
      say "dropArea = " items[2]
    end
    say "======================================="; say

/*============================================================================*/


/*==============================================================================
  Human-Readable Strings (HRSdm)				  v00-01 23Jan12
  --------
   The HRSdm class provides constant character strings for user-visible messages.
  = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */

::CLASS HRSdm PRIVATE		-- Human-Readable Strings
  ::CONSTANT badNoDropCursor  "Error loading NoDrop Cursor, sys error code:"
  ::CONSTANT badOkCursor  "Error loading DropOK Cursor, sys error code:"
