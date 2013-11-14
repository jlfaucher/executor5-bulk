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
/* ooDialog User Guide - Exercise09

   Customer List View						  v04-00 14Nov13
   ------------------
   
   Contains: classes "CustomerListView" and "HRSclv".

   Pre-requisite: CustomerListView.rc, CustomerListView.h, CustList.ico,

   Description: Provides a list of Customers and supports viewing any given
                Customer via a double-click on that Customer's item in the list.

   v01-00 06Jun12: First Version.
   v02-00 08Jan13: Removed stand-alone startup code (not needed after Ex06)
                   Commented out say's.
   v03-00 24May13: Updated to use View and Component mixins.
   v04-00 14Nov13: Uses the LisView superclass introduced in Exercise 9. 

   Outstanding Problems: None reported.
*******************************************************************************/


--.Application~addToConstDir("Customer\CustomerListView.h")

::REQUIRES "ooDialog.cls"
::REQUIRES "customer\customerview.rex"
::REQUIRES "Support\View.rex"
::REQUIRES "Support\Component.rex"
::REQUIRES "Support\ListView.rex"

/*//////////////////////////////////////////////////////////////////////////////
  ==============================================================================
  CustomerListView						  v04-00 14Oct13
  ----------------
  The view of a list of Customers.
  Changes:
    v01-01: First version
    v01-02: Corrected for standalone invocation.
    v01-03 28Jan12: Changed name of HRS class to HRSplv.
    v01-04 11Feb12: moved .application~setDefaults() to app startup file.
                    changed to .application~addToConstDir() here.
    v01-05 19Feb12: Moved .Application~addToConstDir statement from newInstance
                    method to top of file - just before ::requires statement(s).
    v01-06 29Mar12: Very minor mods - all just minor clean-ups. All comments removed
    v02-00 17Aug12: Exercise07 - modified to use the MVF.
           08Jan13: Removed stand-alone startup (not now needed).
    v03-00 24May13: Inherits directly from RcDialog plus the View & Component mixins
    v04-00 14Oct13: Most code provided by the ListView superclass introduced
                    in Exercise 9.
 

  = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */


::CLASS CustomerListView SUBCLASS ListView PUBLIC
  /*----------------------------------------------------------------------------
    Class Methods
    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  ::METHOD getDlgConfig CLASS PRIVATE
    dlgConfig = .Directory~new
    -- Text to appear on the List View:    
    dlgConfig[text] = .HRSclv	-- Text strings visible to the user
    				-- Columns (Fields) to show in List View:    
    dlgConfig[lvColumns] = "Number-60-1-k,Name-170-2,Zip-80-5"
    dlgConfig[dlgSize]   = "225-273"	-- width-height of dialog
    dlgConfig[lvSize]    = "190-215"
    dlgConfig[dlgIcon]   = "Customer\bmp\custlist.ico"
    return dlgConfig

  /*----------------------------------------------------------------------------
    Instance Methods
    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

/*============================================================================*/



/*//////////////////////////////////////////////////////////////////////////////
  ==============================================================================
  HRSclv (Human-Readable Strings for CustomerListView)		  v02-00 14Nov13
  ---
  The HRSclv class provides constant character strings for dialog title, menus,
  buttons, and user-visible messages issued by the CustomerListView class.
  
  Changes:
  v01-00 07Jun12: First Version
  v02-00 14Nov13: Second version (used from Exercise 9)  
  = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */

::CLASS HRSclv PRIVATE		-- Human-Readable Strings
  ::CONSTANT dlgTitle	  "Customer List"
  ::CONSTANT menu1        "Actions"
  ::CONSTANT menu11       "New Customer"
  ::CONSTANT menu2        "Help"
  ::CONSTANT menu21       "About"
  ::CONSTANT noMenu       "This menu item is not yet implemented."
  ::CONSTANT showButton   "Show Customer"
  ::CONSTANT cancelButton "Cancel"  

/*============================================================================*/

