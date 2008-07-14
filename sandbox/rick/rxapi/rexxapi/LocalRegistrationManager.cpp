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


#include "LocalRegistrationManager.hpp"
#include "LocalAPIManager.hpp"
#include "SysLibrary.hpp"
#include "ClientMessage.hpp"


ServiceReturn LocalRegistrationManager::registerCallback(RegistrationType type, char *name, char *module,
    char *proc, uint32_t *userData, void *userPointer, size_t drop, int legacyStyle)
{
    // first parameter for these calls is ALWAYS the type
    ClientMessage message(RegistrationManager, REGISTER_LIBRARY, type);

    // the environment name always goes into name argument buffer
    strcpy(message.nameArg, name);

    ServiceRegistrationData regData;
    message.setMessageData((char *)&regData, sizeof(ServiceRegistrationData));

    strcpy(regData.moduleName, module);
    strcpy(regData.procedureName, proc);
    regData.dropAuthority = drop == 0 ? DROP_ANY : OWNER_ONLY;
    regData.userPointer = userPointer;
    regData.legacyStyle = legacyStyle;
    // we have two bits of user data to copy
    if (userData != NULL)
    {
        regData.userData[0] = userData[0];
        regData.userData[1] = userData[1];
    }
    else
    {
        regData.userData[0] = 0;
        regData.userData[1] = 0;
    }

    message.send();
    return message.result;
}

ServiceReturn LocalRegistrationManager::registerCallback(RegistrationType type, char *name, RexxCallback entryPoint,
    uint32_t *userData, void *userPointer, int legacyStyle)
{
    // first parameter for these calls is ALWAYS the type
    ClientMessage message(RegistrationManager, REGISTER_ENTRYPOINT, type);

    // the environment name always goes into name argument buffer
    strcpy(message.nameArg, name);
    ServiceRegistrationData regData;
    message.setMessageData((char *)&regData, sizeof(ServiceRegistrationData));

    regData.userPointer = userPointer;
    regData.entryPoint = (uintptr_t)entryPoint;
    regData.dropAuthority = OWNER_ONLY;
    regData.legacyStyle = legacyStyle;
    // we have two bits of user data to copy
    if (userData != NULL)
    {
        regData.userData[0] = userData[0];
        regData.userData[1] = userData[1];
    }
    else
    {
        regData.userData[0] = 0;
        regData.userData[1] = 0;
    }
    message.send();
    return message.result;
}


ServiceReturn LocalRegistrationManager::dropCallback(RegistrationType type, char *name, char *module)
{
    // first parameter for these calls is ALWAYS the type
    ClientMessage message(RegistrationManager, REGISTER_DROP, type);

    // the environment name always goes into name argument buffer
    strcpy(message.nameArg, name);
    ServiceRegistrationData regData;

    // requesting by explicit library name
    if (module != NULL)
    {
        // we need to change the operation and copy one additional parameter
        message.operation = REGISTER_DROP_LIBRARY;
        strcpy(regData.moduleName, module);
        message.setMessageData((char *)&regData, sizeof(ServiceRegistrationData));
    }

    message.send();
    return message.result;
}

//TODO:  Double check the clearing of passed buffers on returns to avoid memory leaks.
ServiceReturn LocalRegistrationManager::queryCallback(RegistrationType type, char *name)
{
    // first parameter for these calls is ALWAYS the type
    ClientMessage message(RegistrationManager, REGISTER_QUERY, type);

    // the environment name always goes into name argument buffer
    strcpy(message.nameArg, name);

    message.send();
    return message.result;
}

ServiceReturn LocalRegistrationManager::queryCallback(RegistrationType type, char *name, char *module,
    uint32_t *userData, void **userPointer)
{
    // first parameter for these calls is ALWAYS the type
    ClientMessage message(RegistrationManager, REGISTER_QUERY, type);
    ServiceRegistrationData regData;

    // the environment name always goes into name argument buffer
    strcpy(message.nameArg, name);

    // requesting by explicit library name
    if (module != NULL)
    {
        // we need to change the operation and copy one additional parameter
        message.operation = REGISTER_QUERY_LIBRARY;
        strcpy(regData.moduleName, module);
        message.setMessageData((char *)&regData, sizeof(ServiceRegistrationData));
    }

    message.send();
    // if this was there, copy the user information back
    if (message.result == CALLBACK_EXISTS)
    {
        ServiceRegistrationData *retData = (ServiceRegistrationData *)message.getMessageData();

        if (userData != NULL)
        {
            userData[0] = retData->userData[0];
            userData[1] = retData->userData[1];
        }
        if (userPointer != NULL)
        {
            *userPointer = retData->userPointer;
        }
        // make sure we free the returned value.
        ooRexxFreeMemory(retData);
    }

    return message.result;
}

void LocalRegistrationManager::resolveCallback(RegistrationType type, char *name, char *module,
    RexxCallback &entryPoint, int &legacyStyle)
{
    entryPoint = NULL;                 // assume failure

    // first parameter for these calls is ALWAYS the type
    ClientMessage message(RegistrationManager, REGISTER_LOAD_LIBRARY, type);
    ServiceRegistrationData regData;
    message.setMessageData((char *)&regData, sizeof(ServiceRegistrationData));

    // the environment name always goes into name argument buffer
    strcpy(message.nameArg, name);
    // module is optional, so make sure we have something to copy.
    if (module == NULL)
    {
        strcpy(module,"");
    }
    strcpy(regData.moduleName, module);

    message.send();

    // if this was there, now try to load the module, if necessary.
    if (message.result == CALLBACK_EXISTS)
    {
        ServiceRegistrationData *retData = (ServiceRegistrationData *)message.getMessageData();
        if (strlen(retData->moduleName) != 0)
        {
            entryPoint = NULL;
            SysLibrary lib;
            if (lib.load(retData->moduleName))
            {
                entryPoint = (RexxCallback)lib.getProcedure(retData->procedureName);
            }
        }
        else
        {
            entryPoint = (RexxCallback)retData->entryPoint;
        }
        legacyStyle = retData->legacyStyle;
        // make sure we free the returned value.
        ooRexxFreeMemory(retData);
    }
}


RexxReturnCode LocalRegistrationManager::processServiceException(ServiceException *e)
{
    switch (e->getErrorCode())
    {
        case CALLBACK_NOT_FOUND:
            return RXSUBCOM_NOTREG;

        case DROP_NOT_AUTHORIZED:
            return RXSUBCOM_NOCANDROP;

        default:
            return RXAPI_MEMFAIL;
    }
}
