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
/*********************************************************************/
#include <rexx.h>
#include <oorexx.h>
#include "RexxLegacyAPI.hpp"
#include "InternalAPI.hpp"


/*********************************************************************/
/*                                                                   */
/*  Function:        RexxCreateQueue()                               */
/*                                                                   */
/*  Description:     API Entry to create a queue.                    */
/*                                                                   */
/*  Function:        Create a new (empty) queue.                     */
/*                                                                   */
/*  Notes:           Queues are assigned an internal name            */
/*                   derived from the session that created them      */
/*                   and the serial number of the queue.  When a     */
/*                   queue is deleted, its serial number becomes     */
/*                   reuseable.                                      */
/*                                                                   */
/*                   The queue header blocks are stored in order     */
/*                   of their serial number.  The first queue        */
/*                   created has serial number 1.  Serial number 0   */
/*                   is reserved for the session queue.              */
/*                                                                   */
/*                   Before a queue is created, the chain of queue   */
/*                   headers is searched for gaps.  If we find a     */
/*                   gap in the sequence, we use the number that     */
/*                   belongs to the gap.  This is somewhat           */
/*                   inefficient, but has the advantages of          */
/*                   simplicity, reliability and understandability.  */
/*                                                                   */
/*                   The user can pass a NULL for the requested queue*/
/*                   name.  In that case, we assign an arbitrary     */
/*                   name.                                           */
/*                                                                   */
/*                   If the requested name already exists, we assign */
/*                   an arbitrary name to the new queue.  This name  */
/*                   is passed back to the caller, and the duplicate */
/*                   flag is set.                                    */
/*                                                                   */
/*                   Queue names must obey the rules for REXX        */
/*                   symbols.  Lower case characters in queue names  */
/*                   are translated to upper case.                   */
/*                                                                   */
/*  Input:           Buffer for the name of the created queue.       */
/*                   Size of this buffer.                            */
/*                   Requested queue name.  May be NULL.             */
/*                                                                   */
/*  Output:          Internal name for the queue.  Duplicate         */
/*                   indicator.  Status of create.                   */
/*                                                                   */
/*  Effects:         New queue created.                              */
/*                                                                   */
/*********************************************************************/
APIRET  APIENTRY RexxCreateQueue(
  const char *     name,                  /* Internal name (returned).  */
  unsigned long   size,                   /* Length of name buffer.     */
  char *     usrrequest,                  /* Desired name.              */
  unsigned long *  pdup)                  /* Duplicate name flag.       */
{
  RexxBoolean dup;                     // temporary for value marshalling

  unsigned long rc = ooRexxCreateQueue(const_cast<RexxStringPointer>(name), size, usrrequest, &dup);
  *pdup = (unsigned long)dup;
  return rc;
}


/*********************************************************************/
/*                                                                   */
/*  Function:         RexxDeleteQueue()                              */
/*                                                                   */
/*  Description:      Delete a queue.                                */
/*                                                                   */
/*  Function:         Delete all entries in a queue, then delete     */
/*                    the queue header.                              */
/*                                                                   */
/*  Notes:            Must tell the queue data manager to            */
/*                    delete the queue entries.                      */
/*                                                                   */
/*  Input:            external queue name.                           */
/*                                                                   */
/*  Effects:          Queue and all its entries deleted.             */
/*                                                                   */
/*********************************************************************/
APIRET APIENTRY RexxDeleteQueue(
  const char * name)                            /* name of queue to delete    */
{
  return ooRexxDeleteQueue(const_cast<RexxStringPointer>(name));
}


/*********************************************************************/
/*                                                                   */
/*  Function:         RexxQueryQueue()                               */
/*                                                                   */
/*  Description:      Return size of a named queue.                  */
/*                                                                   */
/*  Function:         Return the count of elements in a named queue. */
/*                                                                   */
/*  Input:            external queue name                            */
/*                                                                   */
/*  Effects:          Count of queue elements.                       */
/*                                                                   */
/*********************************************************************/
APIRET APIENTRY RexxQueryQueue(
  const char *    name,                        /* Queue to query.             */
  unsigned long * count)                       /* Length of queue (returned)  */
{
    RexxUnsignedNumber qCount;

    unsigned long rc = ooRexxQueryQueue(const_cast<RexxStringPointer>(name), &qCount);
    *count = (unsigned long)qCount;
    return rc;
}


/*********************************************************************/
/*                                                                   */
/*  Function:         RexxAddQueue()                                 */
/*                                                                   */
/*  Description:      Add entry to a queue.                          */
/*                                                                   */
/*  Function:         Allocate memory for queue entry and control    */
/*                    block.  Move data into entry & set up          */
/*                    control info.  Add entry to queue chain.       */
/*                                                                   */
/*  Input:            external queue name, entry data, data size,    */
/*                    LIFO/FIFO flag.                                */
/*                                                                   */
/*  Effects:          Memory allocated for entry.  Entry added to    */
/*                    queue.                                         */
/*                                                                   */
/*********************************************************************/
APIRET APIENTRY RexxAddQueue(
  const char *       name,
  PRXSTRING data,
  unsigned long     flag)
{
  return ooRexxAddQueue(const_cast<RexxStringPointer>(name), (RxString *)data, flag);
}

/*********************************************************************/
/*                                                                   */
/*  Function Name:  RexxRegisterSubcomDll                            */
/*                                                                   */
/*  Description:    Registration function for the subcommand         */
/*                  interface.  All programs wishing to act as       */
/*                  subcommand environments for REXX must first      */
/*                  register through this function.                  */
/*                                                                   */
/*                  The function adds new registration blocks to the */
/*                  subcommand registration table. Uses functions in */
/*                  REXXAPI for memory allocation functions.         */
/*                                                                   */
/*  Entry Point:    RexxRegisterSubcomDll                            */
/*                                                                   */
/*  Parameter(s):   EnvName   - name of the registered subcommand    */
/*                              handler                              */
/*                                                                   */
/*                  ModuleName - name of the DLL module containing   */
/*                               the subcommand handler              */
/*                                                                   */
/*                  EntryPoint - name of the DLL routine for the     */
/*                               handler                             */
/*                                                                   */
/*                  UserArea   - Area for any user data              */
/*                                                                   */
/*                  DropAuth   - Drop authority flag                 */
/*                                                                   */
/*  Return Value:   Valid RXSUBCOM return codes                      */
/*                                                                   */
/*********************************************************************/

APIRET APIENTRY RexxRegisterSubcomDll(
  const char *   EnvName,                       /* Subcom name                */
  const char *   ModuleName,                    /* Name of DLL                */
  const char *   EntryPoint,                    /* DLL routine name           */
  char *         UserArea,                      /* User data                  */
  unsigned long  DropAuth )                     /* Drop Authority             */
{
    return _ooRexxRegisterLibrarySubcom(const_cast<RexxStringPointer>(EnvName), const_cast<RexxStringPointer>(ModuleName), const_cast<RexxStringPointer>(EntryPoint), NULL, (uint32_t *)UserArea, DropAuth, SUBCOM_CLASSIC_STYLE);
}

/*********************************************************************/
/*                                                                   */
/*  Function Name:  RexxRegisterSubcomExe                            */
/*                                                                   */
/*  Description:    Registration function for the subcommand         */
/*                  interface.  All programs wishing to act as       */
/*                  subcommand environments for REXX must first      */
/*                  register through this function.                  */
/*                                                                   */
/*                  The function adds new registration blocks to the */
/*                  subcommand registration table. Uses functions in */
/*                  REXXAPI for memory allocation functions.         */
/*                                                                   */
/*  Entry Point:    RexxRegisterSubcomDll                            */
/*                                                                   */
/*  Parameter(s):   EnvName   - name of the registered subcommand    */
/*                              handler                              */
/*                                                                   */
/*                  EntryPoint - address of the subcommand handler   */
/*                                                                   */
/*                  UserArea   - Area for any user data              */
/*                                                                   */
/*  Return Value:   Valid RXSUBCOM return codes                      */
/*                                                                   */
/*********************************************************************/

APIRET APIENTRY RexxRegisterSubcomExe(
  const char *   EnvName,                       /* Subcom name                */
  PFN   EntryPoint,                    /* DLL routine name           */
  char * UserArea )                    /* User data                  */
{
    return _ooRexxRegisterSubcom(const_cast<RexxStringPointer>(EnvName), (RexxCallback)EntryPoint, NULL, (uint32_t *)UserArea, SUBCOM_CLASSIC_STYLE);
}

/*********************************************************************/
/*                                                                   */
/*  Function Name:   RexxDeregisterSubcom                            */
/*                                                                   */
/*  Description:     drops a block from the subcommand registration  */
/*                   table. Uses functions in rexxapi to free memory.*/
/*                                                                   */
/*  Entry Point:     RexxDeregisterSubcom                            */
/*                                                                   */
/*  Parameter(s):    name     -  The environment name to be dropped. */
/*                   dllname  -  The associated dllname to be        */
/*                               dropped, if appropiate.             */
/*                                                                   */
/*  Return Value:    Valid RXSUBCOM return codes                     */
/*                                                                   */
/*********************************************************************/

APIRET APIENTRY RexxDeregisterSubcom(
  const char * name,                            /* Environment Name           */
  const char * dllname )                        /* Associated Name (of DLL)   */
{
    return ooRexxDeregisterSubcom(const_cast<RexxStringPointer>(name), const_cast<RexxStringPointer>(dllname));
}

/*********************************************************************/
/*                                                                   */
/*  Function Name:   RexxQuerySubcom                                 */
/*                                                                   */
/*  Description:     Querys the subcommand registration table.       */
/*                   Allows user to tell if a subcommand is          */
/*                   registered.                                     */
/*                                                                   */
/*  Entry Point:     RexxQuerySubcom                                 */
/*                                                                   */
/*  Parameter(s):    name     -  The environment name to be dropped. */
/*                   dll      -  The associated dllname to be        */
/*                               dropped, if appropiate.             */
/*                   exist    -  Existence flag.                     */
/*                   userword -  8 bytes of user data.               */
/*                                                                   */
/*  Return Value:    RXSUBCOM_OK - Subcommand is registered          */
/*                   RXSUBCOM_NOTREG - Subcommand not registered     */
/*                   RXSUBCOM_BADTYPE - Internal error.  Should not  */
/*                                      occur.                       */
/*                                                                   */
/*********************************************************************/

APIRET APIENTRY RexxQuerySubcom(
  const char *     name,                        /* Environment Name           */
  const char *     dll,                         /* Associated Name (of DLL)   */
  unsigned short * exist,                       /* existence information      */
  char *  userword )                   /* data from registration     */
{
    RexxUnsignedNumber lexist;

    APIRET rc = _ooRexxQuerySubcom(const_cast<RexxStringPointer>(name), const_cast<RexxStringPointer>(dll), &lexist, NULL, (uint32_t *)userword);
    *exist = lexist;
    return rc;
}

/*********************************************************************/
/*                                                                   */
/*  Function Name:  RexxRegisterExitDll                              */
/*                                                                   */
/*  Description:    Registration function for the exit               */
/*                  interface.  All programs wishing to act as       */
/*                  exit handlers for REXX must first                */
/*                  register through this function.                  */
/*                                                                   */
/*                  The function adds new registration blocks to the */
/*                  exit registration table. Uses functions in       */
/*                  REXXAPI for memory allocation functions.         */
/*                                                                   */
/*  Entry Point:    RexxRegisterExitDll                              */
/*                                                                   */
/*  Parameter(s):   EnvName   - name of the registered exit          */
/*                              handler                              */
/*                                                                   */
/*                  ModuleName - name of the DLL module containing   */
/*                               the exit handler                    */
/*                                                                   */
/*                  EntryPoint - name of the DLL routine for the     */
/*                               handler                             */
/*                                                                   */
/*                  UserArea   - Area for any user data              */
/*                                                                   */
/*                  DropAuth   - Drop authority flag                 */
/*                                                                   */
/*  Return Value:   Valid RXSUBCOM return codes                      */
/*                                                                   */
/*********************************************************************/

APIRET APIENTRY RexxRegisterExitDll(
  const char *   EnvName,                       /* Exit name                  */
  const char *   ModuleName,                    /* Name of DLL                */
  const char *   EntryPoint,                    /* DLL routine name           */
  char * UserArea,                     /* User data                  */
  unsigned long DropAuth )                     /* Drop Authority             */
{
    return _ooRexxRegisterLibraryExit(const_cast<RexxStringPointer>(EnvName), const_cast<RexxStringPointer>(ModuleName), const_cast<RexxStringPointer>(EntryPoint), NULL, (uint32_t *)UserArea, DropAuth, EXIT_CLASSIC_STYLE);
}

/*********************************************************************/
/*                                                                   */
/*  Function Name:  RexxRegisterExitExe                              */
/*                                                                   */
/*  Description:    Registration function for the exit               */
/*                  interface.  All programs wishing to act as       */
/*                  exit handlers for REXX must first                */
/*                  register through this function.                  */
/*                                                                   */
/*                  The function adds new registration blocks to the */
/*                  subcommand registration table. Uses functions in */
/*                  REXXAPI for memory allocation functions.         */
/*                                                                   */
/*  Entry Point:    RexxRegisterExitDll                              */
/*                                                                   */
/*  Parameter(s):   EnvName   - name of the registered exit          */
/*                              handler                              */
/*                                                                   */
/*                  EntryPoint - address of the exit handler         */
/*                                                                   */
/*                  UserArea   - Area for any user data              */
/*                                                                   */
/*  Return Value:   Valid RXSUBCOM return codes                      */
/*                                                                   */
/*********************************************************************/

APIRET APIENTRY RexxRegisterExitExe(
  const char *   EnvName,                       /* exit name                  */
  PFN   EntryPoint,                    /* DLL routine name           */
  char * UserArea )                    /* User data                  */
{
    return _ooRexxRegisterExit(const_cast<RexxStringPointer>(EnvName), (RexxCallback)EntryPoint, NULL, (uint32_t *)UserArea, EXIT_CLASSIC_STYLE);
}

/*********************************************************************/
/*                                                                   */
/*  Function Name:   RexxDeregisterExit                              */
/*                                                                   */
/*  Description:     drops a block from the exit registration        */
/*                   table. Uses functions in rexxapi to free memory.*/
/*                                                                   */
/*  Entry Point:     RexxDeregisterExit                              */
/*                                                                   */
/*  Parameter(s):    name     -  The environment name to be dropped. */
/*                   dllname  -  The associated dllname to be        */
/*                               dropped, if appropiate.             */
/*                                                                   */
/*  Return Value:    Valid RXSUBCOM return codes                     */
/*                                                                   */
/*********************************************************************/

APIRET APIENTRY RexxDeregisterExit(
  const char * name,                            /* Environment Name           */
  const char * dllname )                        /* Associated Name (of DLL)   */
{
    return ooRexxDeregisterExit(const_cast<RexxStringPointer>(name), const_cast<RexxStringPointer>(dllname));
}

/*********************************************************************/
/*                                                                   */
/*  Function Name:   RexxQueryExit                                   */
/*                                                                   */
/*  Description:     Querys the exit registration table.             */
/*                   Allows user to tell if an exit is               */
/*                   registered.                                     */
/*                                                                   */
/*  Entry Point:     RexxQueryExit                                   */
/*                                                                   */
/*  Parameter(s):    name     -  The environment name to be dropped. */
/*                   dll      -  The associated dllname to be        */
/*                               dropped, if appropiate.             */
/*                   exist    -  Existence flag.                     */
/*                   userword -  8 bytes of user data.               */
/*                                                                   */
/*  Return Value:    RXSUBCOM_OK - Exit is registered                */
/*                   RXSUBCOM_NOTREG - Exit not registered           */
/*                   RXSUBCOM_BADTYPE - Internal error.  Should not  */
/*                                      occur.                       */
/*                                                                   */
/*********************************************************************/

APIRET APIENTRY RexxQueryExit(
  const char * name,                            /* Environment Name           */
  const char * dll,                             /* Associated Name (of DLL)   */
  unsigned short * exist,                       /* existence information      */
  char *  userword )                   /* data from registration     */
{
    RexxUnsignedNumber lexist;

    APIRET rc = _ooRexxQueryExit(const_cast<RexxStringPointer>(name), const_cast<RexxStringPointer>(dll), &lexist, NULL, (uint32_t *)userword);
    *exist = lexist;
    return rc;
}


/*********************************************************************/
/*                                                                   */
/*  Function Name:  RexxRegisterFunctionDll                          */
/*                                                                  */
/*  Description:    Registration function for the external function  */
/*                  interface.  All programs wishing to act as       */
/*                  external functions for REXX must first           */
/*                  register through this function.                  */
/*                                                                   */
/*                  The function adds new registration blocks to the */
/*                  function registration table. Uses functions in   */
/*                  REXXAPI for memory allocation functions.         */
/*                                                                   */
/*  Entry Point:    RexxRegisterFunctionDll                          */
/*                                                                   */
/*  Parameter(s):   EnvName   - name of the registered function      */
/*                              handler                              */
/*                                                                   */
/*                  ModuleName - name of the DLL module containing   */
/*                               the function                        */
/*                                                                   */
/*                  EntryPoint - name of the DLL routine for the     */
/*                               function                            */
/*                                                                   */
/*  Return Value:   Valid RXSUBCOM return codes                      */
/*                                                                   */
/*********************************************************************/

APIRET APIENTRY RexxRegisterFunctionDll(
  const char *   EnvName,                       /* Subcom name                */
  const char *   ModuleName,                    /* Name of DLL                */
  const char *   EntryPoint )                   /* DLL routine name           */
{
    return ooRexxRegisterLibraryFunction(const_cast<RexxStringPointer>(EnvName), const_cast<RexxStringPointer>(ModuleName), const_cast<RexxStringPointer>(EntryPoint), FUNCTION_CLASSIC_STYLE);
}

/*********************************************************************/
/*                                                                   */
/*  Function Name:  RexxRegisterFunctionExe                          */
/*                                                                   */
/*  Description:    Registration function for the external function  */
/*                  interface.  All programs wishing to act as       */
/*                  external functions for REXX must first           */
/*                  register through this function.                  */
/*                                                                   */
/*                  The function adds new registration blocks to the */
/*                  function registration table.  Uses functions in  */
/*                  REXXAPI for memory allocation functions.         */
/*                                                                   */
/*  Entry Point:    RexxRegisterFunctionDll                          */
/*                                                                   */
/*  Parameter(s):   EnvName   - name of the registered function      */
/*                                                                   */
/*                  EntryPoint - address of the function             */
/*                                                                   */
/*  Return Value:   Valid RXSUBCOM return codes                      */
/*                                                                   */
/*********************************************************************/

APIRET APIENTRY RexxRegisterFunctionExe(
  const char *   EnvName,                       /* Subcom name                */
  PFN   EntryPoint )                   /* DLL routine name           */
{
    return ooRexxRegisterFunction(const_cast<RexxStringPointer>(EnvName), (RexxCallback)EntryPoint, FUNCTION_CLASSIC_STYLE);
}

/*********************************************************************/
/*                                                                   */
/*  Function Name:   RexxDeregisterFunction                          */
/*                                                                   */
/*  Description:     drops a block from the function registration    */
/*                   table. Uses functions in rexxapi to free memory.*/
/*                                                                   */
/*  Entry Point:     RexxDeregisterFunction                          */
/*                                                                   */
/*  Parameter(s):    name     -  The function name to be dropped.    */
/*                                                                   */
/*  Return Value:    Valid RXSUBCOM return codes                     */
/*                                                                   */
/*********************************************************************/

APIRET APIENTRY RexxDeregisterFunction(
  const char * name )                           /* Environment Name           */
{
    return ooRexxDeregisterFunction(const_cast<RexxStringPointer>(name));
}

/*********************************************************************/
/*                                                                   */
/*  Function Name:   RexxQueryFunction                               */
/*                                                                   */
/*  Description:     Querys the function registration table.         */
/*                   Allows user to tell if a function is            */
/*                   registered.                                     */
/*                                                                   */
/*  Entry Point:     RexxQueryFunction                               */
/*                                                                   */
/*  Parameter(s):    name     -  The function name to be queried.    */
/*                                                                   */
/*  Return Value:    RXSUBCOM_OK - Function is registered            */
/*                   RXSUBCOM_NOTREG - Function not registered       */
/*                                                                   */
/*********************************************************************/

APIRET APIENTRY RexxQueryFunction(
  const char * name )                           /* Environment Name           */
{
    return ooRexxQueryFunction(const_cast<RexxStringPointer>(name));
}

/*********************************************************************/
/*                                                                   */
/*  Function Name:      RexxAddMacro                                 */
/*                                                                   */
/*  Description:        change or insert a macro's pcode and literal */
/*                      images in the workspace                      */
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

APIRET APIENTRY RexxAddMacro(
  const char *    n,                            /* name of macro function     */
  const char *    s,                            /* name of file               */
  unsigned long  pos )                         /* search order pos request   */
{
    return ooRexxAddMacro(const_cast<RexxStringPointer>(n), const_cast<RexxStringPointer>(s), pos);
}


/*********************************************************************/
/*                                                                   */
/*  Function Name:      RexxDropMacro                                */
/*                                                                   */
/*  Description:        remove a macro's pcode and literal images    */
/*                                                                   */
/*  Entry Point:        RexxDropMacro                                */
/*                                                                   */
/*  Input:              n - macro name asciiz string                 */
/*                                                                   */
/*  Output:             return code                                  */
/*                                                                   */
/*********************************************************************/

APIRET APIENTRY RexxDropMacro(
  const char * n )                              /* name of macro to delete    */
{
    return ooRexxDropMacro(const_cast<RexxStringPointer>(n));
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

APIRET APIENTRY RexxClearMacroSpace(void)
{
    return ooRexxClearMacroSpace();
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
APIRET APIENTRY RexxSaveMacroSpace(
  unsigned long    ac,                         /* count of arguments         */
  const char *     *av,                         /* argument list              */
  const char *      fnam )                      /* file name                  */
{
    return ooRexxSaveMacroSpace(ac, const_cast<RexxStringPointer *>(av), const_cast<RexxStringPointer>(fnam));
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
APIRET APIENTRY RexxLoadMacroSpace(
  unsigned long    ac,                         /* argument count             */
  const char *     *av,                         /* list of argument strings   */
  const char *      fnam )                      /* file name to load functs   */
{
    return ooRexxLoadMacroSpace(ac, const_cast<RexxStringPointer *>(av), const_cast<RexxStringPointer>(fnam));
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

APIRET APIENTRY RexxQueryMacro(
  const char *     name,                        /* name to search for         */
  unsigned short * pos )                        /* pointer for return of pos  */
{
    RexxUnsignedNumber lpos;
    APIRET rc = ooRexxQueryMacro(const_cast<RexxStringPointer>(name), &lpos);
    *pos = lpos;
    return rc;
}


/*********************************************************************/
/*                                                                   */
/*  Function Name:      RexxReorderMacro                             */
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

APIRET APIENTRY RexxReorderMacro(
  const char *    name,                         /* name of function to change */
  unsigned long  pos )                         /* new position for function  */
{
    return ooRexxReorderMacro(const_cast<RexxStringPointer>(name), pos);
}


/*********************************************************************/
/*                                                                   */
/*  Function Name:      RexxShutdownAPI                              */
/*                                                                   */
/*  Description:        try to shutdown the interpreter.             */
/*                                                                   */
/*  Entry Point:        RexxShutdownAPI                              */
/*                                                                   */
/*  Output:             return code                                  */
/*                                                                   */
/*********************************************************************/

unsigned long APIENTRY RexxShutDownAPI(void)
{
    return ooRexxShutDownAPI();
}


/*********************************************************************/
/*                                                                   */
/*  Function Name:   RexxAllocateMemory                              */
/*                                                                   */
/*  Description:     Operating system independant function to        */
/*                   allocate memory. The function is a wrapper      */
/*                   for appropriate operating system memory         */
/*                   allocation function.                            */
/*                                                                   */
/*                                                                   */
/*  Entry Point:     RexxAllocateMemory                              */
/*                                                                   */
/*  Parameter(s):    Specifies the number of bytes to allocate(unsigned long)*/
/*                                                                   */
/*  Return Value:    Pointer to the allocated memory (void *)         */
/*                   NULL if the function fails                      */
/*                                                                   */
/*********************************************************************/
void * APIENTRY RexxAllocateMemory(unsigned long size)
{
    return ooRexxAllocateMemory(size);
}

/*********************************************************************/
/*                                                                   */
/*  Function Name:   RexxFreeMemory                                  */
/*                                                                   */
/*  Description:     Operating system independant function to        */
/*                   free memory. The function is a wrapper          */
/*                   for appropriate operating system memory         */
/*                   allocation function.                            */
/*                                                                   */
/*                                                                   */
/*  Entry Point:     RexxFreeMemory                                  */
/*                                                                   */
/*  Parameter(s):    Pointer to the memory allocated with            */
/*                   RexxAllocateMemory (void *)                      */
/*                                                                   */
/*  Return Value:    always returns 0 (unsigned long)                        */
/*                                                                   */
/*********************************************************************/
APIRET APIENTRY RexxFreeMemory(void * pMem)
{
    return ooRexxFreeMemory(pMem);
}



