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
   
   Product List View						  v04-00 15Oct13
   -----------------
   
   Contains: classes "ProductListView" and "HRSplv" (for human-readable strings).

   Pre-requisite: ListView.rex

   Description: Provides a list of Products and supports viewing any given
                Product via a double-click on that Product's item in the list.
   Changes:
     v01-00 06Jun12: First version
     v02-00 21Aug12: Modified for Exercise07 to use the Model-View Framework.
            11Jan13: Commented-out 'say' instructions.
            01Apr13: After ooDialog 4.2.2, Support folder moved to exercise
                     folder, so change to ::Requires needed. 
     v03-00 20May13: Now inherits directly from RcDialog plus the View &
                     Component mixins.
     v04-00 15Oct13: Uses the LisView superclass introduced in Exercise 9. 

   Outstanding Problems: None reported.
*******************************************************************************/

::REQUIRES "ooDialog.cls"
::REQUIRES "Product\ProductView.rex"
::REQUIRES "Support\View.rex"
::REQUIRES "Support\Component.rex"
::REQUIRES "Support\ListView.rex"

/*//////////////////////////////////////////////////////////////////////////////
  ==============================================================================
  ProductListView						  v04-00 15oct13
  -------------
  The view of a list of products.
  Changes:
    v01-00 06Jun12: First version
    v02-00 21Aug12: Modified to use the Model-View Framework.
    		    Comment "Ex07" shows where changes from Ex06 have been made.
           09Jan13: 'Say' instructions commented out or removed.
                    Standalone operation removed (not now needed).
    v03-00 24May13: ProducView now inherits directly from ResDialog plus the
                    View & Compoment mixins.                   
    v04-00 15Oct13: Most code provided by the ListView superclass introduced
                    in Exercise 9.
  = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */


::CLASS ProductListView SUBCLASS ListView PUBLIC

  /*----------------------------------------------------------------------------
    Class Methods
    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  ::METHOD getDlgInfo CLASS PRIVATE
    dlgInfo = .Array~new
    -- Text to appear on the List View:    
    dlgInfo[1] = .HRSplv
    -- Columns (Fields) to show in List View:    
    --dlgInfo[2] = "1-Number-60-key, 2-Name-130, 3-Price-50-f, 4-UOM-40"
    dlgInfo[2] = "1-Number-60-key, 2-Name-130, 3-Price-50-fr, 4-UOM-40-r"
    dlgInfo[3] = "225-273"	-- width-height of dialog
    return dlgInfo

  ::METHOD formatPrice PRIVATE
    use arg price
    return (price/100)~format(,2)
    
  
  /*----------------------------------------------------------------------------
    Instance Methods
    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

/*============================================================================*/



/*//////////////////////////////////////////////////////////////////////////////
  ==============================================================================
  HRSplv (Human-Readable Strings for ProductListView)		  v02-00 15Oct13
  ---
  The HRSplv class provides constant character strings for dialog title, menus,
  buttons, and user-visible messages issued by the ProductListView class.
  = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */

::CLASS HRSplv PRIVATE		-- Human-Readable Strings
  ::CONSTANT dlgTitle	  "Product List"
  ::CONSTANT menu1        "Actions"
  ::CONSTANT menu11       "New Product..."
  ::CONSTANT menu2        "Help"
  ::CONSTANT menu21       "About..."
  ::CONSTANT noMenu       "This menu item is not yet implemented."
  ::CONSTANT showButton   "Show Product"
  ::CONSTANT cancelButton "Cancel"  

/*============================================================================*/

