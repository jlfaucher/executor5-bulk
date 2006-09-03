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
/****************************************************************************/
/* Name: EditRex.rex                                                        */
/* Type: Object Rexx Script                                                 */
/*                                                                          */
/* Description:                                                             */
/* This script reads the registry entries for the Ftype REXXScript EDIT     */
/* type and allows to change it between Notepad and any given Editor.       */
/*                                                                          */
/* It uses the WindowsRegistry Class.                                       */
/*                                                                          */
/****************************************************************************/

/* create a new registry object */
r = .WindowsRegistry~new

/* leave if init failed */
if r~InitCode \= 0 then exit

/* Set from default HKEY_LOCAL_MACHINE to CLASSES_ROOT*/
r~Current_Key = CLASSES_ROOT

/* Open the Key with Options QUERY and WRITE */
if r~open(,"REXXScript\Shell\Edit\Command","QUERY WRITE") \= 0 then do

  /* Retrieves information about a given key in a compound variable */
  /* q.values holds the number of value entries. */
  q. = r~Query

  /* Retrieves all value entries of a given key into a compound variable */
  if r~ListValues(,vals.) = 0 then do
    /* There are 3 possible values for each entry:
       vals.i.name  the Name of the value
       vals.i.data  the data of the entry
       vals.i.type  the type : NORMAL for alphabetic values,
                               EXPAND for expandable strings such as a path,
                               NONE for no specified type,
                               MULTI for multiple strings,
                               NUMBER for a 4-byte value, and
                               BINARY for any data format.
    */
    /* get the current program only. Do not show the "%1" parameter */
    program = vals.1.data~word(1)
  end
  else do
    say 'Error reading the registry.'
    say 'Program aborted.'
    exit
  end


  /* Ask user what to do */
  /* Show the curent content */
  say 'Current Edit Setting is : ' || program
  say 'Please enter a number + <Enter> to set it to:'
  say '1 to EDIT it with NOTEPAD.EXE'
  say '2 to ENTER an OTHER program'
  say 'Any other entry to leave without changes'
  say
  /* Get selection */
  pull answer

  /* verify what to do */
  select
    when answer = '1'  then do
      newval = 'notepad.exe "%1"'
    end

    when answer = '2' then do
       say
       say 'Please enter Path and name of the wanted editor '
       pull program
       if program~LENGTH < 3 then exit
       newval = program ' %1'
    end
    otherwise exit
  end

  /* Set the new value */
  /* Sets a named value of a given key */
  /* If name is blank or omitted, the default value is set */
  r~SetValue(r~Current_Key,"",newval,NORMAL)

  /* Forces the system to write the cache buffer of a given key to disk */
  /* If key_handle is omitted, CURRENT_KEY is flushed */
  r~flush(r~Current_Key)

  /* Closes a previously opened key specified by its handle */
  /* Since it can take several seconds before all data is written to disk,*/
  /* I've used FLUSH before, to empty the cache */
  /* If key_handle is omitted, CURRENT_KEY is closed */
  r~close(r~Current_Key)

  say 'Changed to : ' newval
end


::requires "winsystm.cls" -- required for the registry class
