/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2008 Rexx Language Association. All rights reserved.         */
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

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\
  File:    billingDlg.rex                           Author:      Mark Miesfeld
                                                    Creation date:  05/02/2008

  Project: ooDialog Tutorial                        Last update:    05/19/2008
  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Category:
    ooDialog samples

  Syntax:
    billingDlg <account name>

  Purpose:
    Provide an ooDialog sample that uses combo boxes.

    This dialog is part of a fictious accounting program.  The dialog collects
    the billing address for an account.  The collected address is passed back
    from the dialog in a table object.  The table object could then be passed on
    to another part of the accounting program where it would be recorded,
    perhaps in a database, or maybe a flat file.

    A few things are done differently than they would be in a real program to
    faciliate running this from a command prompt as a stand alone example.
    Those places should be relatively easy to spot.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
use arg accountName

  -- In a real program, this would probably be called as a function and the
  -- account name passed in as the argument.  Here it is just faked by using a
  -- command line argument.

  if arg(1, 'O') then do
    say "You must supply an account name for this program"
    say "Syntax:"
    say "  rexx billingDlg.rex <accountName>"
    say
    say "For example:"
    say "  rexx billingDlg.rex Home Depot"
    return 99
  end

  -- Instantiate the dialog, check the init code, and return if some error
  -- happened.
  --
  -- The dialog is created from the template in the resource script,
  -- billingDlg.rc.  Symbolic IDs are used for the resource IDs throughout this
  -- example.  The IDs are defined in billingDlg.h. The symbolic ID,
  -- IDD_DLG_COMBOBOX is the ID of the dialog in the resource script.

  dlg = .BillingDialog~new("billingDlg.rc", IDD_DLG_COMBOBOX, , "billingDlg.h")
  if dlg~initCode <> 0 then do
    say "Error starting dialog.  initCode:" dlg~initCode
    return dlg~initCode
  end

  -- Pass the account name on to the dialog.
  dlg~setAccount(accountName)

  -- The execute method is what actually creates the underlying Windows dialog
  -- and runs it until the user closes the dialog.  Note that a custom icon is
  -- used for the dialog.  The second parameter to the execute method is the
  -- resource ID of this icon.  The icon is defined in the resource script.
  --
  -- When the execute method returns, the underlying dialog has been closed.
  -- The method will return 1 if the user pushes the Okay button, and 2 if the
  -- user cancels the dialog.  The billing address data is only valid if
  -- the user pushes Okay.
  ret = dlg~execute("NORMAL", IDI_DLG_ICON)

  -- In a real program, the data object would be returned to the caller, maybe
  -- passing back .nil if the dialog was canceled.  Here the results are just
  -- displayed to the console.
  if ret == 1 then do
    -- Get the billing address from the dialog.  It is returned in the form of a
    -- table object.
    data = dlg~getBillingAddress
    say data['name']
    say data['address1']
    if data['address2'] \== .nil then say data['address2']
    say data['city'] data['state'] data['zip']
  end
  else do
    say "The user canceled"
  end

  -- The deInstall method unregisters all the ooDialog external functions.
  -- Normally that would only be done when the program is entirely finished.
  -- Since this is just an example and it is now finished, the deInstall method
  -- is invoked.  This is not really needed.
  dlg~deInstall

return 0
-- End of entry point.

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\
  Directives, Classes, or Routines.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::requires "oodwin32.cls"

::class 'BillingDialog' public subclass RcDialog inherit AdvancedControls MessageExtensions

/** init()
 * The initialization for this dialog.  The superclass init method must be
 * called.  Then the county / city / zip code data table is filled in.
 */
::method init
  expose countyData accountName
  forward class (super) continue

  self~initCode = result
  if self~initCode <> 0 then return self~initCode

  countyData = .table~new
  self~fillCountyData(countyData)


/** setAccount()
 * Sets the name of the account and erases the billing address fields in the
 * address structure (sets them to .nil.)
 */
::method setAccount
  expose accountName billingAddress
  use strict arg accountName

  billingAddress = .table~new
  self~nullify


/** getBillingAddress()
 * Returns the billing address data to the programmer.  This data is only valid
 * if the user clicked Okay.
 */
::method getBillingAddress
  expose billingAddress

  if \ billingAddress~isA(.table) then return .nil
  else return billingAddress


/** initAutoDetection()
 * An ooDialog supplied dialog method intended to be overridden by the
 * programmer.
 *
 * In this case it is used to prevent ooDialog from setting values for the
 * controls when the dialog is first displayed.
 *
 * This dialog manages the controls itself.
 */
::method initAutoDetection
  self~noAutoDetection


/** initDialog()
 * An ooDialog supplied dialog method intended to be overridden by the
 * programmer.
 *
 * This method is always invoked by ooDialog.  It is called immediately after
 * the underlying Windows dialog has been created.  Before the underlying dialog
 * has been created, the programmer can not interact with the dialog controls.
 * So, for instance, a combo box can not be filled with its values.
 *
 * Therefore, initDialog() is the proper place for the programmer to do
 * initialization of the dialog controls, all those things that require
 * interacting with the actual control.
 */
::method initDialog
  expose accountName countyData zipData

  -- The account name does not change during the display of the dialog, so a
  -- static control is used for that information. Here that static control is
  -- set with the current account name.
  self~getStaticControl(IDC_STATIC_CUSTOMER)~setTitle(accountName)

  -- Start with the San Diego county radio button checked.  For each county, the
  -- cities combo box is filled with the cities in that county. Each time a
  -- different city is selected, the zip code combo box is filled with the valid
  -- zip codes for that city.
  --
  -- All the city and zip code data is stored in a table.  The data for each
  -- county is stored as an item in the table using the resource ID of the radio
  -- button for the county as its index.
  --
  -- Here: 1.) The radio button control with symbolic resource ID of
  -- IDC_RB_SANDIEGO is set to checked.  2.) The city/zip code data for San
  -- Diego county is fetched from the data table using the resource ID as the
  -- index.  3.) A private method is called that handles filling the combo
  -- boxes.
  self~getRadioControl(IDC_RB_SANDIEGO)~check
  zipData = countyData[self~constDir[IDC_RB_SANDIEGO]]
  self~setCities(zipData['cities'])

  -- The fictious business using this fictious accounting program only operates
  -- in Southern California, so the state is unchanging.  A combo box is used
  -- anyway, in case the business decides to expand to Arizona.  Here, the combo
  -- box is filled with the single item, CA, and it is set as selected.
  combo = self~getComboBox(IDC_COMBO_STATE)
  combo~add("CA")
  combo~selectIndex(1)

  -- This type of GUI programming is often called 'event-driven programming.'
  -- As the user interacts with the dialog events are generated representing the
  -- actions the user has taken.  Clicking a button, scrolling a scroll bar,
  -- selecting an item in a list view, etc..  When a certain event happens, the
  -- program can be notified of the event and then take action.
  --
  -- There are a large number of possible events.  Generally, a program is
  -- interested in only a few of them, the ones needed to drive the actions of
  -- the program.
  --
  -- In ooDialog, the programmer connects the events she is interested in with a
  -- specific method in her dialog object.  When the event occurrs, the method
  -- in the dialog object is invoked.  Events are tied to specific controls
  -- using the resource ID of the control
  --
  -- This program has two main events that drive it.  When the user selects a
  -- different county, the city and zip code combo boxes need to be filled with
  -- the correct information.  When the user selects a different city, the zip
  -- code combo box needs to be filled with the correct zip codes.

  -- For the radio buttons, connect each button clicked event to the same
  -- method.  When a radio button is clicked, it becomes the selected radio
  -- button in the group.
  self~connectButton(IDC_RB_SANDIEGO, onClick)
  self~connectButton(IDC_RB_ORANGE,   onClick)
  self~connectButton(IDC_RB_IMPERIAL, onClick)

  -- For combo boxes, there is an event generated each time the selected item in
  -- the combo box changes.  Connect that event for the city combo box only.
  self~connectComboBoxNotify(IDC_COMBO_CITY, "SELCHANGE", onCityChange)


/** onClick()
 * This method is invoked every time one of the county radio buttons is clicked.
 * 2 arguments are sent to this method.  The first will always be the resource
 * ID of the button clicked.  The second will always be the window handle of the
 * button.
 *
 * Since clicking on a radio button changes it to the selected button, the zip
 * code data for the button is retrieved from the data table and the city / zip
 * code combo boxes are populated.  The window handle is not needed so that
 * argument is simply ignored.
 */
::method onClick
  expose countyData zipData
  use arg buttonID

  -- The button resource ID is used as the index in the data table to fetch the
  -- correct data for the selected button.  Every time a radio button is
  -- clicked, the zipData instance variable is set to the correct data for that
  -- button.  Then the private method is invoked that populates the combo boxes.
  zipData = countyData[buttonID]
  self~setCities(zipData['cities'])


/** onCityChange()
 * This method is invoked every time the selected item in the cities combo box
 * changes.  The arguments sent to this method are not needed and are simply
 * ignored.
 *
 * The selected city is fetched from the combo box and the zip code combo box is
 * populated with the correct zip codes for that city.
 */
::method onCityChange
  expose zipData

  -- Create an object that represents the underlying combo box.  The method can
  -- return .nil if the object can not be created.  The reality is that .nil is
  -- only returned when the programmer has an error, and once the program is
  -- working, the check for .nil can be discarded.  .nil is returned if the
  -- resource ID is incorrect, if the underlying dialog control has not been
  -- created, or if the underlying control has been destroyed.  Using symbolic
  -- IDs mostly eliminates the first error and the control can not generate a
  -- clicked event if it has not yet been created or has already been destroyed.
  combo = self~getComboBox(IDC_COMBO_CITY)
  if combo == .nil then return

  -- Combo boxes can have no items selected, which is why the check for the
  -- empty string is done.  However, in this program that can never happen and
  -- the check for the empty string is not really needed.
  city = combo~selected
  if selected == "" then return

  -- Invoke the private method to populate the zip code combo box with the
  -- correct zip codes for the selected city.
  self~setZipCodes(zipData[city]);

/** setCities()
 * Populates the cities combo box with the names of the cities specified.
 *
 * The cities argument is an array of city names.
 */
::method setCities private
  expose zipData
  use strict arg cities

  -- Create an object that represents the underlying combo box.  See the notes
  -- in the onCityChange() method.
  combo = self~getComboBox(IDC_COMBO_CITY)
  if combo == .nil then return .false

  -- First, delete all the existing items in the combo box.
  combo~deleteAll

  -- Second, add each city name in the array of city names to the combo box.
  do city over cities
    combo~add(city)
  end

  -- Third, set the selected item in the combo box by position.  This will be
  -- the first item in the combo box.
  combo~selectIndex(1)

  -- Each time the selected city in the cities combo box changes, the zip code
  -- combo box needs to be popluated with the correct data for that city.  In
  -- the zip code data table each item is an array of zip codes and the index
  -- for the item is the name of the city that the zip codes belong to.
  --
  -- Here the private method that populates the zip code combo box is invoked
  -- with an array containing the zip codes.  The selected city is at index 1
  -- and the name of the city is retrieved from the combo box.  This city name
  -- is then used as the index in the zip code data table.
  self~setZipCodes(zipData[combo~getText(1)])

  return .true

/** setZipCodes()
 * Populates the zip code combo box with the zip codes specified.
 *
 * The codes argument is an array of zip codes.
 */
::method setZipCodes private
  use strict arg codes

  -- Create an object that represents the underlying combo box.  See the notes
  -- in the onCityChange() method.
  combo = self~getComboBox(IDC_COMBO_ZIP)
  if combo == .Nil then return .false

  -- Remove all the existing items in the combo box.
  combo~deleteAll

  -- Now add all the new zip codes to the combo box.  Keep track of the lowest
  -- zip code that is added.
  lowest = 99999
  do zipCode over codes
    combo~add(zipCode)
    if zipCode < lowest then do
      lowest = zipCode
    end
  end

  -- Finally, select the item in the combo box that is the lowest zip code.  The
  -- item is selected by its text, rather than by position.
  combo~select(lowest)

  return .true

/** validate()
 * An ooDialog supplied dialog method intended to be overridden by the
 * programmer.
 *
 * This method is invoked automatically from the ooDialog supplied OK method,
 * which under normal circumstances is invoked when the user pushes the OK
 * button.  If validate() returns 1, the dialog is destroyed; if validate()
 * returns 0, the dialog continues to execute.  The intent is that the
 * programmer can override this method and use it to validate the data the user
 * entered in the dialog.
 */
::method validate
  expose billingAddress

  -- Check that the user has entered all valid data.  In this simple dialog,
  -- the only thing to check is that an address1 has been supplied.  Other
  -- dialogs could have more elaborate validation.

  -- Create an object that represents the underlying edit control for address
  -- line 1 and fetch the text from the control.  See the notes in
  -- onCityChange() concerning the possibility of getEditControl() returning
  -- .nil
  text = self~getEditControl(IDC_EDIT_ADDR1)~getText~strip

  -- Check that the user entered the address.  If the edit control is blank,
  -- put up a message box telling the user what they need to do, and return 0.
  if text == "" then do
    j = infoDialog("The address line 1 field must be filled in.")

    -- Returning 0 causes the dialog to keep executing.
    return 0
  end

  -- That is all the validation that is done for this dialog.  The use of combo
  -- boxes has ensured that city, state, and zip code are already valid.  If the
  -- optionaly address line 2 is blank, that index in the billing address table
  -- is left .nil.
  billingAddress['address1'] = text

  text = self~GetEditControl(IDC_EDIT_ADDR2)~getText~strip
  if text \== "" then
    billingAddress['address2'] = text

  billingAddress['city']  = self~getComboBox(IDC_COMBO_CITY)~selected
  billingAddress['state'] = self~getComboBox(IDC_COMBO_STATE)~selected
  billingAddress['zip']   = self~getComboBox(IDC_COMBO_ZIP)~selected

  -- Returning 1 closes the dialog.
  return 1

/** nullify()
 * Sets all the fields, except account name, in the billing address to nil.
 */
::method nullify private
  expose accountName billingAddress

  billingAddress['name']     = accountName
  billingAddress['address1'] = .nil
  billingAddress['address2'] = .nil
  billingAddress['city']     = .nil
  billingAddress['state']    = .nil
  billingAddress['zip']      = .nil


/* The methods below are used to construct a table of tables containing the
 * county, city, and zip code data used in this example.  In a real world
 * application, this data might be read from a file, or maybe gotten from a
 * database.
 */
::method fillCountyData private
  use arg tbl

  id = self~constDir[IDC_RB_SANDIEGO]
  tbl[id] = self~doSanDiego

  id = self~constDir[IDC_RB_ORANGE]
  tbl[id] = self~doOrange

  id = self~constDir[IDC_RB_IMPERIAL]
  tbl[id] = self~doImperial

::method doSanDiego private

  t = .table~new
  t['cities'] = .array~of("La Jolla", "Escondido", "La Mesa", "Chula Vista")
  t["La Jolla"] = .array~of(92037, 92038, 92039, 92092, 92093)
  t["Chula Vista"] = .array~of(91909, 91910, 91911, 91912, 91913, 91914, 91915, 91921)
  t["La Mesa"] = .array~of(91941, 91942, 91943, 91944)
  t["Escondido"] = .array~of(92025, 92026, 92027, 92029, 92030, 92033, 92046)

  return t

::method doOrange private

  t = .table~new
  t['cities'] = .array~of("Hunington Beach", "Anaheim", "Newport Beach", "Costa Mesa")
  t["Newport Beach"] = .array~of(92658, 92659, 92660, 92661, 92662, 92663 )
  t["Hunington Beach"] = .array~of(92605, 92615, 92646, 92647, 92648, 92649)
  t["Anaheim"] = .array~of(92801, 92802, 92803, 92804, 92805, 92806, 92807, 92808, 92809, -
                                92812, 92814, 92815, 92816, 92817, 92825, 92850, 92899 )
  t["Costa Mesa"] = .array~of(92626, 92627, 92628)

  return t

::method doImperial private

  t = .table~new
  t['cities'] = .array~of("El Centro", "Calexico")
  t["El Centro"] = .array~of(92243, 92244)
  t["Calexico"] = .array~of(92231, 92232 )

  return t

/* - - - End Of File: billingDlg.rex- - - - - - - - - - - - - - - - - - - - - */
