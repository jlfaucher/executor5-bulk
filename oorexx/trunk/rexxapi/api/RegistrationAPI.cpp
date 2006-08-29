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
#include "LocalRegistrationManager.hpp"
#include "RexxAPI.h"
#include "LocalAPIContext.hpp"
#include "RexxLegacyAPI.hpp"
#include "InternalAPI.hpp"
#include "ClientMessage.hpp"
#include "SysLocalAPIManager.hpp"


/*********************************************************************/
/*                                                                   */
/*  Function Name:  ooRexxRegisterLibrarySubcom                      */
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
/*  Parameter(s):   EnvName   - name of the registered subcommand    */
/*                              handler                              */
/*                                                                   */
/*                  ModuleName - name of the module containing       */
/*                               the subcommand handler              */
/*                                                                   */
/*                  EntryPoint - name of the routine for the         */
/*                               handler                             */
/*                                                                   */
/*                  UserArea   - Area for any user data              */
/*                                                                   */
/*                  DropAuth   - Drop authority flag                 */
/*                                                                   */
/*  Return Value:   Valid RXSUBCOM return codes                      */
/*                                                                   */
/*********************************************************************/

RexxReturnCode RexxEntry ooRexxRegisterLibrarySubcom(
  RexxStringPointer  envName,                    // Subcom name
  RexxStringPointer  moduleName,                 // Name of DLL
  RexxStringPointer  procedureName,              // DLL routine name
  RexxUserData       userArea,                   // User data
  RexxUnsignedNumber dropAuthority,              // Drop Authority
  RexxNumber         style)                      // call style
{
    return _ooRexxRegisterLibrarySubcom(envName, moduleName, procedureName, userArea, NULL, dropAuthority == RXSUBCOM_NONDROP, style);
}

RexxReturnCode RexxEntry _ooRexxRegisterLibrarySubcom(
  RexxStringPointer  envName,                    // Subcom name
  RexxStringPointer  moduleName,                 // Name of library
  RexxStringPointer  procedureName,              // library routine name
  RexxUserData       userArea,                   // User data
  uint32_t          *oldUserArea,                // legacy user data
  RexxUnsignedNumber dropAuthority,              // Drop Authority
  RexxNumber         style)                      // the call style
{
    ENTER_REXX_API(RegistrationManager)
    {
        //TODO:  add style validity checking
        lam->registrationManager.registerCallback(SubcomAPI,
            envName, moduleName, procedureName, oldUserArea, userArea, dropAuthority == RXSUBCOM_NONDROP, style);
    }
    EXIT_REXX_API();
}

/*********************************************************************/
/*                                                                   */
/*  Function Name:  RexxRegisterSubcom                               */
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

RexxReturnCode RexxEntry ooRexxRegisterSubcom(
  RexxStringPointer envName,                  // Subcom name
  RexxCallback      entryPoint,               // callback address
  RexxUserData      userArea,                 // User data
  RexxNumber        style)                    // style of the callback
{
    return _ooRexxRegisterSubcom(envName, entryPoint, userArea, NULL, style);
}


RexxReturnCode RexxEntry _ooRexxRegisterSubcom(
  RexxStringPointer envName,                  // Subcom name
  RexxCallback      entryPoint,               // callback address
  RexxUserData      userArea,                 // User data
  uint32_t          *oldUserArea,             // legacy user data
  RexxNumber        style)                    // legacy call style
{
    ENTER_REXX_API(RegistrationManager)
    {
        lam->registrationManager.registerCallback(SubcomAPI,
            envName, entryPoint, oldUserArea, userArea, style);
    }
    EXIT_REXX_API();
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

RexxReturnCode RexxEntry ooRexxDeregisterSubcom(
  RexxStringPointer name,                            /* Environment Name           */
  RexxStringPointer moduleName )                     /* Associated library name    */
{
    ENTER_REXX_API(RegistrationManager)
    {
        lam->registrationManager.dropCallback(SubcomAPI, name, moduleName);
    }
    EXIT_REXX_API();
}

/*********************************************************************/
/*                                                                   */
/*  Function Name:   RexxQuerySubcom                                 */
/*                                                                   */
/*  Description:     Queries the subcommand registration table.      */
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

RexxReturnCode RexxEntry ooRexxQuerySubcom(
  RexxStringPointer   name,            /* Environment Name           */
  RexxStringPointer   module,          /* Associated library name    */
  RexxUnsignedNumber *flags,           /* existence information      */
  RexxUserData       *userWord)        /* data from registration     */
{
    return _ooRexxQuerySubcom(name, module, flags, userWord, NULL);
}


RexxReturnCode RexxEntry _ooRexxQuerySubcom(
  RexxStringPointer   name,            // Environment Name
  RexxStringPointer   module,          // Associated Name (of DLL)
  RexxUnsignedNumber *flags,           // existence information
  RexxUserData       *userWord,        // data from registration
  uint32_t           *oldUserArea)     // legacy user data
{
    ENTER_REXX_API(RegistrationManager)
    {
        if (lam->registrationManager.queryCallback(SubcomAPI, name, module, oldUserArea, userWord) == CALLBACK_EXISTS)
        {
            return RXSUBCOM_OK;
        }
        else
        {
            return RXSUBCOM_NOTREG;
        }
    }
    EXIT_REXX_API();
}


/*********************************************************************/
/*                                                                   */
/*  Function Name:   ooRexxResolveSubcom                             */
/*                                                                   */
/*  Description:     Query and return information about a resolved   */
/*                   Subcom handler                                  */
/*                                                                   */
/*  Entry Point:     ooRexxResolveSubcom                             */
/*                                                                   */
/*  Parameter(s):    name       -  Name of the desired system exit   */
/*                   entryPoint -  Pointer to the resolved handler   */
/*                   style      -  The subcom call style info        */
/*                                                                   */
/*  Return Value:    Return code from subcommand handler processing  */
/*                                                                   */
/*********************************************************************/

RexxReturnCode RexxEntry ooRexxResolveSubcom(
  RexxStringPointer name,              // Exit name.
  RexxCallback     *entryPoint,        // the entry point of the exit
  RexxNumber       *legacyStyle)       // the type of call
{
    ENTER_REXX_API(RegistrationManager)
    {
        int legacy;
        lam->registrationManager.resolveCallback(SubcomAPI, name, NULL, *entryPoint, legacy);
        *legacyStyle = (RexxNumber)legacy;
    }
    EXIT_REXX_API();
}


/*********************************************************************/
/*                                                                   */
/*  Function Name:  RexxRegisterLibraryExit                          */
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
/*  Entry Point:    RexxRegisterLibraryExit                          */
/*                                                                   */
/*  Return Value:   Valid RXSUBCOM return codes                      */
/*                                                                   */
/*********************************************************************/

RexxReturnCode RexxEntry ooRexxRegisterLibraryExit(
  RexxStringPointer  envName,                  // Exit name
  RexxStringPointer  moduleName,               // Name of DLL
  RexxStringPointer  procedureName,            // DLL routine name
  RexxUserData       userArea,                 // User data
  RexxUnsignedNumber dropAuthority,            // Drop Authority
  RexxNumber         style)                    // the exit call style
{
    return _ooRexxRegisterLibraryExit(envName, moduleName, procedureName, userArea, NULL, dropAuthority, style);
}

RexxReturnCode RexxEntry _ooRexxRegisterLibraryExit(
  RexxStringPointer  envName,                  /* Exit name                  */
  RexxStringPointer  moduleName,               /* Name of DLL                */
  RexxStringPointer  procedureName,            /* DLL routine name           */
  RexxUserData       userArea,                 /* User data                  */
  uint32_t          *oldUserArea,              // legacy user data
  RexxUnsignedNumber dropAuthority,            /* Drop Authority             */
  RexxNumber         legacyStyle)              // old-style calls
{
    ENTER_REXX_API(RegistrationManager)
    {
        lam->registrationManager.registerCallback(ExitAPI,
            envName, moduleName, procedureName, oldUserArea, userArea, dropAuthority == RXSUBCOM_NONDROP, legacyStyle);
    }
    EXIT_REXX_API();
}

/*********************************************************************/
/*                                                                   */
/*  Function Name:  RexxRegisterExit                                 */
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
/*  Return Value:   Valid RXSUBCOM return codes                      */
/*                                                                   */
/*********************************************************************/

RexxReturnCode RexxEntry ooRexxRegisterExit(
  RexxStringPointer envName,           /* exit name                  */
  RexxCallback  entryPoint,            /* Entry point address        */
  RexxUserData *userArea,              /* User data                  */
  RexxNumber    style)                 // the exit call style
{
    return _ooRexxRegisterExit(envName, entryPoint, userArea, NULL, style);
}


RexxReturnCode RexxEntry _ooRexxRegisterExit(
  RexxStringPointer envName,           /* exit name                  */
  RexxCallback  entryPoint,            /* Entry point address        */
  RexxUserData *userArea,              /* User data                  */
  uint32_t     *oldUserArea,           // legacy user data
  RexxNumber legacyStyle)         // indicates the call style of the exit
{
    ENTER_REXX_API(RegistrationManager)
    {
        lam->registrationManager.registerCallback(ExitAPI,
            envName, entryPoint, oldUserArea, userArea, legacyStyle);
    }
    EXIT_REXX_API();
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

RexxReturnCode RexxEntry ooRexxDeregisterExit(
  RexxStringPointer name,                          /* Environment Name           */
  RexxStringPointer moduleName)                    /* Associated library name    */
{
    ENTER_REXX_API(RegistrationManager)
    {
        lam->registrationManager.dropCallback(ExitAPI, name, moduleName);
    }
    EXIT_REXX_API();
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

RexxReturnCode RexxEntry ooRexxQueryExit(
  RexxStringPointer name,              /* Environment Name           */
  RexxStringPointer module,            /* Associated Name (of DLL)   */
  RexxUnsignedNumber *exist,           /* existence information      */
  RexxUserData       *userword)        /* data from registration     */
{
    return _ooRexxQueryExit(name, module, exist, userword, NULL);
}

RexxReturnCode RexxEntry _ooRexxQueryExit(
  RexxStringPointer name,              /* Environment Name           */
  RexxStringPointer module,            /* Associated Name (of DLL)   */
  RexxUnsignedNumber *exist,           /* existence information      */
  RexxUserData       *userword,        /* data from registration     */
  uint32_t     *oldUserArea)           // legacy user data
{
    ENTER_REXX_API(RegistrationManager)
    {
        if (lam->registrationManager.queryCallback(ExitAPI, name, module, oldUserArea, userword) == CALLBACK_EXISTS)
        {
            *exist = 1;
            return RXSUBCOM_OK;
        }
        else
        {
            *exist = 0;
            return RXSUBCOM_NOTREG;
        }
    }
    EXIT_REXX_API();
}


/*********************************************************************/
/*                                                                   */
/*  Function Name:   ooRexxResolveExit                               */
/*                                                                   */
/*  Description:     Resolves a system exit entrypoint address       */
/*                                                                   */
/*  Entry Point:     ooRexxResolveExit                               */
/*                                                                   */
/*  Parameter(s):    name       -  Name of the desired system exit   */
/*                   entrypoint -  returned entry point address      */
/*                   legacyStyle - style of the call                 */
/*                                                                   */
/*  Return Value:    Return code from exit if the exit ran           */
/*                   -1 otherwise                                    */
/*                                                                   */
/*********************************************************************/

RexxReturnCode RexxEntry ooRexxResolveExit(
  RexxStringPointer name,              // Exit name.
  RexxCallback     *entryPoint,        // the entry point of the exit
  RexxNumber       *legacyStyle)       // the type of call
{
    ENTER_REXX_API(RegistrationManager)
    {
        int legacy;
        lam->registrationManager.resolveCallback(ExitAPI, name, NULL, *entryPoint, legacy);
        *legacyStyle = (RexxNumber)legacy;
    }
    EXIT_REXX_API();
}

/*********************************************************************/
/*                                                                   */
/*  Function Name:  RexxRegisterLibraryFunction                      */
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
/*  Return Value:   Valid RXSUBCOM return codes                      */
/*                                                                   */
/*********************************************************************/

RexxReturnCode RexxEntry ooRexxRegisterLibraryFunction(
  RexxStringPointer name,                 // Subcom name
  RexxStringPointer moduleName,           // Name of library
  RexxStringPointer procedureName,        // library routine name
  RexxNumber    style)                    // the function call style
{
    ENTER_REXX_API(RegistrationManager)
    {
        lam->registrationManager.registerCallback(FunctionAPI,
            name, moduleName, procedureName, NULL, NULL, 0, style);
    }
    EXIT_REXX_API();
}

/*********************************************************************/
/*                                                                   */
/*  Function Name:  RexxRegisterFunction                             */
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
/*  Return Value:   Valid RXSUBCOM return codes                      */
/*                                                                   */
/*********************************************************************/

RexxReturnCode RexxEntry ooRexxRegisterFunction(
  RexxStringPointer name,                 // Function name
  RexxCallback  entryPoint,               // Entry point address
  RexxNumber    style)                    // the function call style
{
    ENTER_REXX_API(RegistrationManager)
    {
        lam->registrationManager.registerCallback(FunctionAPI,
            name, entryPoint, NULL, NULL, style);
    }
    EXIT_REXX_API();
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

RexxReturnCode RexxEntry ooRexxDeregisterFunction(
  RexxStringPointer name)                   /* Function Name           */
{
    ENTER_REXX_API(RegistrationManager)
    {
        lam->registrationManager.dropCallback(FunctionAPI, name, NULL);
    }
    EXIT_REXX_API();
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

RexxReturnCode RexxEntry ooRexxQueryFunction(
  RexxStringPointer name)                 /* Function Name           */
{
    ENTER_REXX_API(RegistrationManager)
    {
        if (lam->registrationManager.queryCallback(FunctionAPI, name, NULL, NULL, NULL) == CALLBACK_EXISTS)
        {
            return RXSUBCOM_OK;
        }
        else
        {
            return RXSUBCOM_NOTREG;
        }
    }
    EXIT_REXX_API();
}


RexxReturnCode RexxEntry ooRexxResolveFunction(
  RexxStringPointer name,              // Function name
  RexxStringPointer library,           // Qualifying library name (optional)
  RexxCallback     *entryPoint,        // the entry point of the exit
  RexxNumber       *legacyStyle)       // the type of call
{
    ENTER_REXX_API(RegistrationManager)
    {
        int legacy;
        lam->registrationManager.resolveCallback(FunctionAPI, name, library, *entryPoint, legacy);
        *legacyStyle = (RexxNumber)legacy;
    }
    EXIT_REXX_API();
}


/* try to shutdown the RXAPI.EXE */
/* request from toronto */

RexxReturnCode RexxEntry ooRexxShutDownAPI(void)
{
    ENTER_REXX_API(APIManager)
    {
        lam->shutdown();
    }
    EXIT_REXX_API();
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
/*  Parameter(s):    Specifies the number of bytes to allocate       */
/*                                                                   */
/*  Return Value:    Pointer to the allocated memory                 */
/*                   NULL if the function fails                      */
/*                                                                   */
/*********************************************************************/
void * RexxEntry ooRexxAllocateMemory(RexxStringLength size)
{
    return SysLocalAPIManager::allocateMemory((size_t)size);
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
/*                   RexxAllocateMemory                              */
/*                                                                   */
/*  Return Value:    always returns 0                                */
/*                                                                   */
/*********************************************************************/
RexxReturnCode RexxEntry ooRexxFreeMemory(void *pMem)
{
    SysLocalAPIManager::releaseMemory(pMem);
    return 0;
}


