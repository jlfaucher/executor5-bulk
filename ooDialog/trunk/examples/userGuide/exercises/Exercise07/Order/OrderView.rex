/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2011-2012 Rexx Language Association. All rights reserved.    */
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
   Exercise 07: The OrderView class				  v01-01 25Aug12

   OrderFormView.rex

   Contains: class "OrderView".
   Pre-requisite files: OrderView.rc, OrderView.h.

   Description: A sample Order View component - part of the sample
        	Order Management application.
        	This is a "leaf" component - invoked by OrderListView.

   Outstanding Problems: None reported.

   Changes:
     v01-00 07Jun12: First Version (Exercise06)
     v01-01 25Aug12: Ex07 - changed to use the MVF.

------------------------------------------------------------------------------*/


.Application~addToConstDir("Order\OrderView.h")


::REQUIRES "ooDialog.cls"
::REQUIRES "Order\OrderModelsData.rex"
::REQUIRES "..\Support\RcView.rex"


/*==============================================================================
  OrderView							  v01-01 25Aug12
  -------------
  The "view" (or "gui") part of the Order component - part of the sample
  Order Management application.

  Changes:
    v01-00 07Jun12: First Version.
    v01-01 25Aug12: Ex07 - changed to use the MVF.
  = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */

::CLASS OrderView SUBCLASS RcView PUBLIC				-- Ex07

  ::METHOD newInstance CLASS PUBLIC
    use strict arg idModel, rootDlg					-- Ex07
    say ".OrderView-newInstance: rootDlg =" rootDlg
    dlg = self~new("Order\OrderView.rc", "IDD_ORDER_DIALOG")
    dlg~activate(idModel, rootDlg)					-- Ex07
    return dlg								-- Ex07

  /*----------------------------------------------------------------------------
    Dialog Setup Methods
    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  ::METHOD init
    -- creates the dialog instance but does not make it visible.
    expose menuBar
    say "OrderView-init-01"

    forward class (super) continue

    if \ self~createMenuBar then do		-- if there was a problem
      self~initCode = 1
      return
    end


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  ::METHOD createMenuBar
    -- Creates the menu bar on the dialog.
    expose menuBar
    say "OrderView-createMenuBar-01"
    menuBar = .ScriptMenuBar~new("Order\OrderView.rc", IDR_ORDER_MENU, , , .true)

    return .true


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  ::METHOD activate UNGUARDED
    expose rootDlg orderData
    use strict arg idModelInstance, rootDlg				-- Ex07
    forward class (super) continue		-- Ex07 - MVF: required to get Model's data
    orderData = RESULT				-- Ex07 - MVF: model's data returned by super
    say "OrderView-activate-00: orderData = " orderData

    -- Shows the Dialog - i.e. makes it visible to the user.
    say "OrderView-activate-01: OrderNumber = " orderData["OrderNo"] orderData~orderNo
    -- OrderNo SO-3456
    -- CustNo BA0314
    -- Disc 0
    -- Cmtd Y
    -- Date 120527
    if rootDlg = "SA" then self~execute("SHOWTOP","IDI_ORDER_DLGICON")
    else self~popUpAsChild(rootDlg,"SHOWTOP",,"IDI_ORDER_DLGICON")
    return self								-- Ex07


  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  ::METHOD initDialog
    -- Called by ooDialog after SHOWTOP.
    expose menuBar orderControls orderData				-- Ex07
    say "OrderView-initDialog-01"

    menuBar~attachTo(self)

    orderControls = .Directory~new
    orderControls[ecOrderNameAddr] = self~newEdit(          "IDC_ORDER_NAMEADDR")
    orderControls[dtOrderDate]     = self~newDateTimePicker("IDC_ORDER_DATE"    )
    orderControls[ecOrderNo]       = self~newEdit(          "IDC_ORDER_ORDNO"   )
    orderControls[ecCustNo]        = self~newEdit(          "IDC_ORDER_CUSTNO"  )
    orderControls[ecOrderItems]    = self~newListView(      "IDC_ORDER_ITEMS"   )
    orderControls[ecOrder]         = self~newListView(      "IDC_ORDER_ITEMS"   )
    orderControls[stNetCost]       = self~newStatic(        "IDC_ST_NET"        )
    orderControls[stDiscountPC]    = self~newStatic(        "IDC_ST_DISCOUNT_PC")
    orderControls[stDiscount]      = self~newStatic(        "IDC_ST_DISCOUNT"   )
    orderControls[stTaxPC]         = self~newStatic(        "IDC_ST_TAX_PC"     )
    orderControls[stTax]           = self~newStatic(        "IDC_ST_TAX"        )
    orderControls[stTotal]         = self~newStatic(        "IDC_ST_TOTAL"      )


    self~showData


  /*----------------------------------------------------------------------------
    Event-Handler Methods - Menu Events
    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  ::METHOD anAction UNGUARDED
    self~noMenuFunction(.HRSov~anAction)

  /*- - Help - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  ::METHOD about UNGUARDED
    self~noMenuFunction(.HRSov~HelpAbout)

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  ::METHOD noMenuFunction UNGUARDED
    use arg title
    ret = MessageDialog(.HRSov~NoMenu, self~hwnd, title, 'WARNING')


  /*----------------------------------------------------------------------------
    Application Methods
    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  ::METHOD showData
    expose orderControls orderData
    say "--------------------"
    say "OrderView-showData-00: contents of orderData:"
    do i over orderdata
      say i orderData[i]
    end
    --say "orderData['CustNo']:" orderData['CustNo']  -- orderData~CustNo, orderData~'CustNo' - neither work.
    --say "--------------------"

    orderControls[ecOrderNo]~setText(orderData["OrderNo"])
    --orderControls[] = self~newEdit("IDC_ORDER_NAMEADDR")
    --orderControls[ecOrderItems]    = self~newListView("IDC_ORDER_ITEMS")
    --orderControls[ecOrderNo]       = self~newListView("IDC_ORDER_ORDNO")

  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
    -- "Cancel" - This method over-rides the default Windows action of
    -- 'cancel window' for an Escape key.
  ::METHOD cancel
    response = askDialog(.HRSov~QExit, "N")
    if response = 1 then forward class (super)
    return

/*============================================================================*/


/*//////////////////////////////////////////////////////////////////////////////
  ==============================================================================
  Human-Readable Strings (HRSov)				  v01-00 07Jun12
  --------
   The HRSofv class provides constant character strings for user-visible messages.
  = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */

::CLASS HRSov PRIVATE	   -- Human-Readable Strings
  ::CONSTANT anAction    "An Action"
  ::CONSTANT NoMenu      "This menu item is not implemented."
  ::CONSTANT QExit       "Are you sure you want to cancel this Order View?"
  ::CONSTANT HelpAbout   "About Sales Order"

/*============================================================================*/

