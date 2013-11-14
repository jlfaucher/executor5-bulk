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
   Exercise 09: The Order ListView 				  v04-00 23Oct13

   Contains: class "OrderListView", "HRSolv"

   Pre-requisite files: None.

   Description: Provides a list of Orders and supports viewing any given
                Order via a double-click on that Order's item in the list.

   Changes:
   v01-00 07Jun12: First Version (Ex06).
   v02-00 25Aug12: Updated for Ex07 using the MVF.
          08Jan13: Removed stand-alone startup (not now needed).
          11Jan13: Commented-out 'say' instructions.
          01Apr13: After ooDialog 4.2.2, Support folder moved to exercise
                   folder, so change to ::Requires needed. 
   v03-00 24May13: Exercice08 Update to use View & Component mixin.
   v04-00 23Oct13: Uses new ListView superclass. 
   
   Outstanding Problems: None reported.

*******************************************************************************/


::REQUIRES "ooDialog.cls"
::REQUIRES "Order\OrderView.rex"
--::REQUIRES "Support\View.rex"
--::REQUIRES "Support\Component.rex"
::REQUIRES "Support\ListView.rex"


/*//////////////////////////////////////////////////////////////////////////////
  ==============================================================================
  OrderListView						  	  v04-00 14Nov13
  -------------
  The view of a list of products.
  Changes:
    v01-00 07Jun12: First version
    v02-00 25Aug12: Updated for Ex07 using the MVF.
    v03-00 24May13: Update for Ex08 to use View & Component mixin.
    v04-00 14Nov13: Uses ListView superclass.

  = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */

::CLASS OrderListView SUBCLASS ListView PUBLIC 
  /*----------------------------------------------------------------------------
    Class Methods
    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

    
  /*----------------------------------------------------------------------------
    Instance Methods
    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  ::METHOD getDlgConfig CLASS PRIVATE
    dlgConfig = .Directory~new
    -- Text to appear on the List View:    
    dlgConfig[text] = .HRSolv	-- The class object containing text strings 
    				-- visible to the user.
    dlgConfig[lvColumns] ="OrderNo-60-1-k,CustNo-65-2,CustName-140-6,Date-60-3-fr" 
    dlgConfig[dlgSize] = "255-273"	-- width-height of Dialog
    dlgConfig[lvSize] = "225-225"	-- width-height of ListView control
    dlgConfig[dlgIcon] = "OrderCustomer\bmp\orderlist.ico"
    return dlgConfig

    
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  ::METHOD lvFieldFormat PRIVATE
    use strict arg fieldNumber, fieldValue
    if fieldNumber = 3 then do			-- Field 3 is a date yyyymmdd
      year  = fieldValue~left(2)
      month = fieldValue~substr(3,2)
      day   = fieldValue~right(2)
      return month||"/"||day||"/"||year
    end
    else return .false
    	    
/*============================================================================*/


/*//////////////////////////////////////////////////////////////////////////////
  ==============================================================================
  HRSolv (Human-Readable Strings for OrderListView)		  v02-00 24Oct13
  ---
  The HRSolv class provides constant character strings for user-visible text.

  Changes:
   v01-00 07Jun12: First Version
   v02-00 24Oct13: Second version (used from Exercise 9)
  = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */


::CLASS HRSolv PRIVATE		-- Human-Readable Strings
  ::CONSTANT dlgTitle     "Order List"
  ::CONSTANT menu1        "Actions"
  ::CONSTANT menu11       "New Order..."
  ::CONSTANT menu2        "Help"
  ::CONSTANT menu21       "About..."
  ::CONSTANT noMenu       "This menu item is not yet implemented."
  ::CONSTANT showButton   "Show Order"
  ::CONSTANT cancelButton "Cancel"
  
/*============================================================================*/


