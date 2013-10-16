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
   Exercise 09: The Person ListView				  v01-00 09Oct13

   Contains: classes "PersonListView" and "HRSplv".

   Pre-requisite: ListView.rex, PersonList.ico

   Description: Provides a list of Persons and supports viewing any given
                Person via a double-click on that Person's item in the list.
                This is an "Intermediate" component - it is invoked by OrderMgmt,
                and invokes PersonView.

   v01-00 09Oct12: First Version.

   Outstanding Problems: None reported.
*******************************************************************************/


::REQUIRES "ooDialog.cls"
::REQUIRES "extras\person\personview.rex"
::REQUIRES "Support\View.rex"
::REQUIRES "Support\Component.rex"
::REQUIRES "Support\ListView.rex"

/*//////////////////////////////////////////////////////////////////////////////
  ==============================================================================
  PersonListView						  v01-00 09Oct13
  ----------------
  The view of a list of Persons.
  Changes:
    v01-01: First version

  = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */


::CLASS PersonListView SUBCLASS ListView PUBLIC --INHERIT View Component
  /*----------------------------------------------------------------------------
    Class Methods
    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

--  ::METHOD newInstance CLASS PUBLIC
--    self~newInstance:super(.HRSplv)
    
  /*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
    getDlgInfo (class method) - invoked by the ListView superclass to get
                                the text to disply on the list view, and also
                                the columns from the data file to display.    
  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  ::METHOD getDlgInfo CLASS
    dlgInfo = .Array~new
    -- Text to appear on the List View:
    dlgInfo[1] = .HRSplv		
    -- Columns (Fields) to show in List View:
    --dlgInfo[2] = "1-Number-60,2-Family Name-80,5-Job Description-100"	
    dlgInfo[2] = "2-Family Name-80,5-Job Description-100,1-Number-60-Key"
    return dlgInfo

  /*----------------------------------------------------------------------------
    Instance Methods
    - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

--  ::METHOD initDialog
--    self~initDialog:super
    
/*============================================================================*/



/*//////////////////////////////////////////////////////////////////////////////
  ==============================================================================
  HRSplv (Human-Readable Strings for PersonListView)		  v01-00 09Oct13
  ---
  The HRpclv class provides constant character strings for user-visible text 
  (excluding column-headings) used by the PersonListView class.
  = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = = */

::CLASS HRSplv PRIVATE		-- Human-Readable Strings
  ::CONSTANT dlgTitle     "Person List"
  ::CONSTANT menu1        "Actions"
  ::CONSTANT menu11       "New person"
  ::CONSTANT menu2        "Help"
  ::CONSTANT menu21       "About"
  ::CONSTANT noMenu       "This menu item is not yet implemented."
  ::CONSTANT showButton   "Show Person"
  ::CONSTANT cancelButton "Cancel"

/*============================================================================*/
