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

#include "oorexx.h"
#include "LocalAPIManager.hpp"
#include "LocalMacroSpaceManager.hpp"
#include "RexxAPI.h"
#include "LocalAPIContext.hpp"
#include "InternalAPI.hpp"

/*********************************************************************/
/*                                                                   */
/*  Function Name:      RexxAddMacro                                 */
/*                                                                   */
/*  Description:        change or insert a macro's compiled image    */
/*                      image in the macrospace.                     */
/*                                                                   */
/*  Entry Point:        RexxAddMacro                                 */
/*                                                                   */
/*  Inputs:             n   - macro name asciiz string               */
/*                      s   - filename asciiz string                 */
/*                      pos - search order position                  */
/*                                                                   */
/*  Output:             return code                                  */
/*                                                                   */
/*********************************************************************/

RexxReturnCode RexxEntry ooRexxAddMacro(
  RexxStringPointer name,           /* name of macro function     */
  RexxStringPointer file,           /* name of file               */
  RexxUnsignedNumber pos )          /* search order pos request   */
{
    ENTER_REXX_API(MacroSpaceManager)
    {
        lam->macroSpaceManager.addMacroFromFile(name, file, pos);
    }
    EXIT_REXX_API();
}


/*********************************************************************/
/*                                                                   */
/*  Function Name:      RexxDropMacro                                */
/*                                                                   */
/*  Description:        remove a macro's compiled image.             */
/*                                                                   */
/*  Entry Point:        RexxDropMacro                                */
/*                                                                   */
/*  Input:              n - macro name asciiz string                 */
/*                                                                   */
/*  Output:             return code                                  */
/*                                                                   */
/*********************************************************************/

RexxReturnCode RexxEntry ooRexxDropMacro(
  RexxStringPointer name)                          /* name of macro to delete    */
{
    ENTER_REXX_API(MacroSpaceManager)
    {
        lam->macroSpaceManager.removeMacro(name);
    }
    EXIT_REXX_API();
}


/*********************************************************************/
/*                                                                   */
/*  Function Name:      RexxClearMacroSpace                          */
/*                                                                   */
/*  Description:        erase all entries in the macro chain         */
/*                                                                   */
/*  Entry Point:        RexxCleanMacroSpace                          */
/*                                                                   */
/*  Output:             return code                                  */
/*                                                                   */
/*********************************************************************/

RexxReturnCode RexxEntry ooRexxClearMacroSpace(void)
{
    ENTER_REXX_API(MacroSpaceManager)
    {
        lam->macroSpaceManager.clearMacroSpace();
    }
    EXIT_REXX_API();
}


/*********************************************************************/
/*                                                                   */
/*  Function Name:      RexxSaveMacroSpace                           */
/*                                                                   */
/*  Description:        saves all entries in the macro chain         */
/*                                                                   */
/*  Notes:              File Format:                                 */
/*                         - REXXSAA interpreter version string      */
/*                         - Macro Space file signature flag         */
/*                         - MACRO structures for functions          */
/*                         - RXSTRING structures for images          */
/*                                                                   */
/*  Entry Point:        RexxSaveMacroSpace                           */
/*                                                                   */
/*  Input:              ac   - number of requested functions (0=all) */
/*                      av   - list of function names to save        */
/*                      fnam - name of file to save macros in        */
/*                                                                   */
/*  Output:             return code                                  */
/*                                                                   */
/*********************************************************************/
RexxReturnCode RexxEntry ooRexxSaveMacroSpace(
  RexxUnsignedNumber count,                     /* count of arguments         */
  RexxStringPointer *names,                     /* argument list              */
  RexxStringPointer targetFile)                 /* file name                  */
{
    ENTER_REXX_API(MacroSpaceManager)
    {
        // save an explicit list of macros
        if (names != NULL)
        {
            lam->macroSpaceManager.saveMacroSpace(targetFile, names, count);
        }
        else
        {
            // saving everything
            lam->macroSpaceManager.saveMacroSpace(targetFile);
        }
    }
    EXIT_REXX_API();
}


/*********************************************************************/
/*                                                                   */
/*  Function Name:      RexxLoadMacroSpace                           */
/*                                                                   */
/*  Description:        loads entries from a file into macro space   */
/*                                                                   */
/*  Entry Point:        RexxLoadMacroSpace                           */
/*                                                                   */
/*  Input:              ac   - number of requested functions (0=all) */
/*                      av   - list of function names to load        */
/*                      fnam - name of file to load macros from      */
/*                                                                   */
/*  Output:             return code                                  */
/*                                                                   */
/*********************************************************************/
RexxReturnCode RexxEntry ooRexxLoadMacroSpace(
  RexxUnsignedNumber   count,                      // argument count
  RexxStringPointer   *names,                      // list of argument strings
  RexxStringPointer    macroFile)                  // file name to load functs
{
    ENTER_REXX_API(MacroSpaceManager)
    {
        // load an explicit list of macros
        if (names != NULL)
        {
            lam->macroSpaceManager.loadMacroSpace(macroFile, names, count);
        }
        else
        {
            // loading everything
            lam->macroSpaceManager.loadMacroSpace(macroFile);
        }
    }
    EXIT_REXX_API();
}

/*********************************************************************/
/*                                                                   */
/*  Function Name:      RexxQueryMacro                               */
/*                                                                   */
/*  Description:        search for a function in the workspace       */
/*                                                                   */
/*  Entry Point:        RexxQueryMacro                               */
/*                                                                   */
/*  Input:              name - name of function to look for          */
/*                      pos  - pointer to storage for return of pos  */
/*                                                                   */
/*  Output:             return code                                  */
/*                                                                   */
/*********************************************************************/

RexxReturnCode RexxEntry ooRexxQueryMacro(
  RexxStringPointer name,                /* name to search for         */
  RexxUnsignedNumber *pos)               /* pointer for return of pos  */
{
    ENTER_REXX_API(MacroSpaceManager)
    {
        lam->macroSpaceManager.queryMacro(name, pos);
    }
    EXIT_REXX_API();
}


/*********************************************************************/
/*                                                                   */
/*  Function Name:      RxRecorderMacro                              */
/*                                                                   */
/*  Description:        change a functions search order position     */
/*                                                                   */
/*  Entry Point:        RexxReorderMacro                             */
/*                                                                   */
/*  Input:              name - name of function to change order for  */
/*                      pos  - new search order position             */
/*                                                                   */
/*  Output:             return code                                  */
/*                                                                   */
/*********************************************************************/

RexxReturnCode RexxEntry ooRexxReorderMacro(
  RexxStringPointer  name,              /* name of function to change */
  RexxUnsignedNumber pos )              /* new position for function  */
{
    ENTER_REXX_API(MacroSpaceManager)
    {
        lam->macroSpaceManager.reorderMacro(name, pos);
    }
    EXIT_REXX_API();
}


/*********************************************************************/
/*                                                                   */
/*  Function Name:      RexxExecuteMacroFunction                     */
/*                                                                   */
/*  Description:        find a macro's pcode and literal images in   */
/*                      the workspace                                */
/*                                                                   */
/*  Entry Point:        RexxExecuteMacroFunction                     */
/*                                                                   */
/*  Inputs:             name - macro name asciiz string              */
/*                      p    - pointer for return of pcode+lits buf  */
/*                                                                   */
/*  Output:             return code                                  */
/*                                                                   */
/*********************************************************************/

RexxReturnCode RexxEntry ooRexxResolveMacroFunction(
  RexxStringPointer  name,             /* name of func to find       */
  RxString *p)                         /* storage for image return   */
{
    ENTER_REXX_API(MacroSpaceManager)
    {
        lam->macroSpaceManager.getMacro(name, p);
    }
    EXIT_REXX_API();
}

