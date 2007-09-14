/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 1995, 2004 IBM Corporation. All rights reserved.             */
/* Copyright (c) 2005-2007 Rexx Language Association. All rights reserved.    */
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
/****************************************************************************/
/* Name: ftypeRex.rex                                                       */
/* Type: Open Object REXX Script                                            */
/*                                                                          */
/* Description:                                                             */
/* This script reads the registry entries for the ftype REXXScript OPEN     */
/* setting and allows the user to change it between rexx.exe, rexxhide.exe, */
/* rexxpaws.exe, and notepad.                                               */
/*                                                                          */
/* It uses the WindowsRegistry Class, a custom dialog that subclasses the   */
/* UserDialog Class, and some of the message box functions supplied by      */
/* ooDialog.                                                                */
/*                                                                          */
/* Returns:                                                                 */
/*    0 The ftype setting was changed.                                      */
/*    1 The user canceled or new value equals old value.                    */
/*   -1 Some error accessing the registry.                                  */
/*   -2 Other unexpected error.                                             */
/*                                                                          */
/****************************************************************************/

/* See if the user specifies the console version. */
parse arg Interface

  /* The user can pick between a console or ooDialog version.  The default is
   * the ooDialog version.
   *
   * If the ftype setting is currently rexxhide, the console version will not
   * work if this program is started through the ftype association.  I.e., by
   * typing ftypeRex on the command line or double clicking on the program in
   * Windows Explorer.  (Because there will be no console to display the
   * output.)  Of course the user could successfully start the console version
   * by typing 'rexx ftypeRex c' at the command prompt but we will ignore that
   * possibility and force the ooDialog interface when the ftype setting is
   * rexxhide.
   */

  /* If the given parm starts with "c", use the console interface.  TRANSLATE
   * changes the leftmost character to UPPER CASE.
   */
  if Interface~LEFT(1)~TRANSLATE = "C" then Interface = "CONSOLE"
  else Interface = "OODIALOG"

  /* Set up a new line sequence.  This is used to break long message strings
   * into separate lines.
   */
  eol = '0d0a'x

  /* Create a new registry object. */
  r = .WindowsRegistry~new

  /* Leave if the init of the WindowsRegistry object failed. */
  if r~InitCode \= 0 then do
    call ErrorDialog 'Error opening the registry. Program aborted.'
    return -1
  end

  /* Change the current key from the default of HKEY_LOCAL_MACHINE to
   * HKEY_CLASSES_ROOT.
   *
   * Note that for the 7 major key handles, you can use a string keyword in
   * place of the normal numeric handle value:
   *
   *  "MACHINE"        => HKEY_LOCAL_MACHINE
   *  "CLASSES"        => HKEY_CLASSES_ROOT
   *  "CURRENT_USER"   => HKEY_CURRENT_USER
   *  "USERS"          => HKEY_USERS
   *  "PERFORMANCE"    => HKEY_PERFORMANCE_DATA
   *  "CURRENT_CONFIG" => HKEY_CURRENT_CONFIG
   *  "DYN_DATA"       => HKEY_DYN_DATA
   *
   * The following could be coded: r~Current_Key = "CLASSES" but it is probably
   * clearer to explicitly code:   r~Current_Key = r~Classes_Root
   */
  r~Current_Key = r~Classes_Root

  /* Open the Key with Options QUERY and WRITE, exit on error. */
  if r~open(,"REXXScript\Shell\Open\Command","QUERY WRITE") == 0 then do
    msg = "Error opening the REXXScript\Shell key in the" || eol ||            -
          "registry. Program aborted."
    call ErrorDialog msg
    return -1
  end

  /* The query method retrieves information about a given key in a compound
   * variable.  q.values will hold the number of value entries. The number of
   * value entries should be exactly equal to 1.  When working with the Windows
   * registry, it only makes sense to take extra care.  If q.values is not as
   * expected, then abort.
   */
  q. = r~Query
  if q.values \== 1 then do
    r~close(r~Current_Key)
    msg = "Unexpected error with the REXXScript\Shell key in" || eol ||        -
          "the registry.  Expected only 1 value entry, but"   || eol ||        -
          "found" q.values " Program aborted."
    call ErrorDialog msg
    return -1
  end

  /* ListValues retrieves all value entries of a given key into a compound
   * variable
   */
  if r~ListValues(,vals.) = 0 then do
    /* There are 3 possible values for each entry:
     *   vals.i.name  the Name of the value
     *   vals.i.data  the data of the entry
     *   vals.i.type  the type : NORMAL for alphabetic values,
     *                           EXPAND for expandable strings such as a path,
     *                           NONE for no specified type,
     *                           MULTI for multiple strings,
     *                           NUMBER for a 4-byte value, and
     *                           BINARY for any data format.
     */
    /* verify, which is the current entry, TRANSLATE it to UPPER CASE */
    if vals.1.data~TRANSLATE~LASTPOS('REXX.EXE') = 0 then do
      if vals.1.data~TRANSLATE~LASTPOS('REXXHIDE.EXE') = 0 then do
        if vals.1.data~TRANSLATE~LASTPOS('REXXPAWS.EXE') = 0 then do
          if vals.1.data~TRANSLATE~LASTPOS('NOTEPAD.EXE') = 0 then do
            msg = "ftypeRex is not prepared to work with this"       || eol || -
                  "setting:"                                  || eol || eol || -
                  vals.1.data
            call ErrorDialog msg
            return -2
          end
          else do /* remember the current entry */
            existing = 4  -- notepad.exe
          end
        end
        else do /* remember the current entry */
          existing = 3  -- rexxpaws.exe
        end
      end
      else do /* remember the current entry */
        existing = 2  -- rexxhide.exe
      end
    end
    else do /* remember the current entry */
      existing = 1  -- rexx.exe
    end
  end
  else do
    r~close(r~Current_Key)
    call ErrorDialog 'Error reading the registry. Program aborted.'
    return -1
  end

  /* Ask the user what to do. */
  if Interface = "CONSOLE" then do
    /* Test.  If *.rex files are currently associated with rexxhide, the console
     * interface won't work if the user just types ftypeRex on the command line.
     * (Of course 'rexx ftypeRex.rex' will still work.)  In this case, show the
     * user a message box and force the ooDialog interface.
     */
    if existing = 2 then do
      r~close(r~Current_Key)
      msg = "ftype is curently set to rexxhide.exe. You must" || eol ||        -
            "use the OODIALOG interface."
      call ErrorDialog msg
      return -2
    end
    /* The next lines use a plain text interface.  Show an explanation message,
     * the curent setting for ftype, and the text menu.
     */
    say 'The ftype setting assoicates *.rex files with an executable'
    say 'and a default action.  For example, start a *.rex file with'
    say 'rexx.exe'
    say
    say 'Current ftype Setting is : ' || vals.1.data
    say
    say 'Please enter a number + <Enter> to set ftype to :'
    say '1 start a *.rex file with rexx.exe'
    say '2 start a *.rex file with rexxhide.exe'
    say '3 start a *.rex file with rexxpaws.exe'
    say '4 open a *.rex file with notepad.exe'
    say 'Any other entry to leave without changes'
    say
    /* Get selection */
    pull answer
  end
  else do
    /* The next lines use a custom dialog as the interface to the user.  The
     * definition for this dialog class is in the PickOneDlg.cls file.
     */

    /* Create an array of the labels for the radio buttons in the dialog.  With
     * radio buttons, the user can only select 1 radio button.  This makes radio
     * buttons a good choice when there are a number of mutually exclusive
     * options.
     */
    rbLabels = .array~new
    rbLabels[1] = "Start a *.rex file with rexx.exe"
    rbLabels[2] = "Start a *.rex file with rexxhide.exe"
    rbLabels[3] = "Start a *.rex file with rexxpaws.exe"
    rbLabels[4] = "Open a *.rex file with notepad.exe"

    /* This message is displayed at the top of the dialog to help the user
     * understand what to do.
     */
    msg = 'The ftype setting assoicates *.rex files with an executable and a ' -
          'default action.  For example, start a *.rex file with rexx.exe.  '  -
          'The current ftype Setting is : '                                    -
          || eol || eol || vals.1.data || eol || eol ||                        -
          'Select a radio button below and press OK to set ftype.  Use Cancel' -
          ' to leave the setting as is.'

    /* The title for the dialog. */
    title = "ftype setting for REXXScript"

    /* The PickOneDlg class will calculate the proper size for the dialog based
     * on the radio button labels.  However, since this dialog is using a very
     * long message, the resulting dialog is rather out of proportion.  Too tall
     * and skinny.  The optional minimum width argument allows the programmer to
     * adjust the size of the dialog by specifying a minimum width.
     */
    minimum = 200

    /* Create the dialog and show it.  The excute method returns the index of
     * the selected radio button, or -1 if the user cancels.
     */
    answer = .PickOneDlg~new(msg, title, rbLabels, existing, minimum)~execute
  end
  /* end of user interaction */

  /* Get the complete path to the possible executables.  rexx, rexxhide, and
   * rexxpaws are all in the same directory.  pathToExec() is a helper routine
   * towards the bottom of this file.
   */
  pathToRexx = pathToExec("ooRexx")
  pathToNotepad = pathToExec("notepad")

  /* If we do not have the paths, then quit. */
  if pathToRexx == "" | pathToNotepad == "" then do
    r~close(r~Current_Key)
    msg = "Error detecting the path to the ooRexx executables or to" || eol || -
          "Notepad.  Detected paths:"                         || eol || eol || -
          "  ooRexx: " pathToRexx                                    || eol || -
          "  Notepad:" pathToNotepad
    call ErrorDialog msg
    return -2
  end

  /* Verify what to do.  If the setting is the same as what it was, do nothing.
   * There is no sense writing to the registry if it is not needed.
   */
  if answer == existing then do
    r~close(r~Current_Key)
    return 1
  end

  select
    when answer = '1' then do
      newval = '"' || pathToRexx || 'rexx.exe" "%1" %*'
    end

    when answer = '2' then do
      newval = '"' || pathToRexx || 'rexxhide.exe" "%1" %*'
    end

    when answer = '3' then do
      newval = '"' || pathToRexx || 'rexxpaws.exe" "%1" %*'
    end

    when answer = '4' then do
      newval = '"' || pathToNotePad || 'notepad.exe" "%1" %*'
    end
    otherwise return 1
  end

  /* Set the new value in the registry.  The setValue method sets the value for
   * a named data item of a given key.  If name is blank or omitted, the default
   * data item value is set
   */
  r~setValue(r~Current_Key, "", newval, NORMAL)

  /* The flush method forces the system to write the cache buffer of a given key
   * to disk.  If key_handle is omitted, CURRENT_KEY is flushed.
   */
  r~flush(r~Current_Key)

  /* The close method closes a previously opened key specified by its handle.
   * Since it can take several seconds before all data is written to disk, FLUSH
   * was used above to flush the cache for this key to disk.  If key_handle is
   * omitted, CURRENT_KEY is closed.
   */
  r~close(r~Current_Key)

return 0
-- End of entry point.

::requires "winsystm.cls" -- required for the registry class
::requires "PickOneDlg.cls" -- required for the dialog class

/**
 * pathToExec(exe)   Helper routine.
 *
 * Return the fully qualified path to the directory the argument executable is
 * in.  Return the empty string if the executable is not found.
 */
::routine pathToExec
  use arg exe

  select
    when exe~translate == "OOREXX" then do
      -- The ooRexx install sets the REXX_HOME environment variable.  Look for
      -- it first.
      path = value("REXX_HOME", , "ENVIRONMENT")

      if path == "" then do
        -- If REXX_HOME is not set, try searching for rexx.exe in the path.  If
        -- still not found, give up.
        path = SysSearchPath('PATH', 'rexx.exe')
        path = path~left(path~lastpos('\'))
      end
    end

    when exe~translate == "NOTEPAD" then do
      -- Notepad is always in the path on Windows, assume that is true for the
      -- current system.
      path = SysSearchPath('PATH', 'notepad.exe')
      path = path~left(path~lastpos('\'))
    end

    otherwise do
      -- A future enhancement would be to allow the user to associate any
      -- program by entering its name.  The following code would be used to
      -- search for the program.  (The program would need to be in the path
      -- for this to work.)
      if exe~right(4)~translate <> ".EXE" then exe = exe || ".exe"
      path = SysSearchPath('PATH', exe)
      path = path~left(path~lastpos('\'))
    end
  end
  -- End select

  if path <> "" & path~right(1) <> '\' then path = path || '\'

return path
