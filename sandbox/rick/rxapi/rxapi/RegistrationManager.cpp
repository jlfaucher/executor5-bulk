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

#include "RegistrationManager.hpp"
#include "ServiceMessage.hpp"
#include "ServiceException.hpp"


RegistrationData::RegistrationData(char *n, char *m, ServiceRegistrationData *regData)
{
    name = dupString(n);
    moduleName = dupString(m);
    procedureName = dupString(regData->procedureName);
    dropAuthority = regData->dropAuthority;
    userData[0] = regData->userData[0];
    userData[1] = regData->userData[1];
}

RegistrationData::RegistrationData(char *n, SessionID s, ServiceRegistrationData *regData)
{
    name = dupString(n);
    owner = s;
    dropAuthority = regData->dropAuthority;
    userData[0] = regData->userData[0];
    userData[1] = regData->userData[1];
    entryPoint = regData->entryPoint;
}

RegistrationData::~RegistrationData()
{
    delete [] name;
    delete [] moduleName;
    delete [] procedureName;

    SessionCookie *cookie = references;
    while (cookie != NULL)
    {
        SessionCookie *localnext = cookie->next;
        delete cookie;
        cookie = localnext;
    }
}


void RegistrationData::getRegistrationData(ServiceRegistrationData &regData)
{
    if (moduleName != NULL)
    {
        strcpy(regData.moduleName, moduleName);
    }

    if (procedureName != NULL)
    {
        strcpy(regData.procedureName, procedureName);
    }
    regData.userData[0] = userData[0];
    regData.userData[1] = userData[1];
    regData.entryPoint = entryPoint;
    regData.dropAuthority = dropAuthority;
}

void RegistrationData::addSessionReference(SessionID s)
{
    SessionCookie *cookie = findSessionReference(s);
    // already there?  just add a reference.
    if (cookie != NULL)
    {
        cookie->addReference();
    }
    else
    {
        cookie = new SessionCookie(s);
        cookie->next = references;
        references = cookie;
    }
}

void RegistrationData::removeSessionReference(SessionID s)
{
    SessionCookie *cookie = findSessionReference(s);
    // already there?  just add a reference.
    if (cookie != NULL)
    {
        if (cookie->removeReference() == 0)
        {
            removeSessionReference(cookie);
        }
    }
}

SessionCookie *RegistrationData::findSessionReference(SessionID s)
{
    SessionCookie *cookie = references;
    while (cookie != NULL)
    {
        if (cookie->session == s)
        {
            return cookie;
        }
    }
    return NULL;
}

void RegistrationData::removeSessionReference(SessionCookie *s)
{
    if (s == references)
    {
        references = s->next;
    }
    else
    {
        SessionCookie * current = references;
        while (current != NULL)
        {
            if (current->next == s)
            {
                current->next = s->next;
                break;
            }
        }
    }
    delete s;
}



// Add a library registration item to the table.
// Message arguments have the following meanings:
//
// parameter1 -- registration type
// nameArg    -- name of the registered object
void RegistrationTable::registerLibraryCallback(ServiceMessage &message)
{
    ServiceRegistrationData *regData = (ServiceRegistrationData *)message.getMessageData();
    // get the argument names
    char *name = message.nameArg;
    char *module = regData->moduleName;

    RegistrationData *callback = locate(name, module);
    // update the reference counts to make sure drops don't
    // clear things out for other processes.
    if (callback != NULL)
    {
        callback->addSessionReference(message.session);
        message.setResult(REGISTRATION_COMPLETED);
    }
    else
    {
        callback = new RegistrationData(name, module, regData);

        // add to the chain
        callback->next = firstLibrary;
        firstLibrary = callback;

        if (locate(name, message.session) != NULL)
        {
            message.setResult(DUPLICATE_REGISTRATION);
        }
        else
        {
            message.setResult(REGISTRATION_COMPLETED);
        }
    }
    // make sure the data message buffer is not passed back.
    message.freeMessageData();
}

// Add an exe registration item to the table.
// Message arguments have the following meanings:
//
// parameter1 -- registration type
// parameter2 -- drop authority flag
// nameArg    -- The registration name
void RegistrationTable::registerCallback(ServiceMessage &message)
{
    ServiceRegistrationData *regData = (ServiceRegistrationData *)message.getMessageData();
    // get the argument name
    char *name = message.nameArg;

    // now locate an exe registration.
    RegistrationData *callback = locate(name, message.session);
    // update the reference counts to make sure drops don't
    // clear things out for other processes.
    if (callback != NULL)
    {
        message.setResult(DUPLICATE_REGISTRATION);
    }
    else
    {
        callback = new RegistrationData(name, message.session, regData);

        // add to the chain
        callback->next = firstEntryPoint;
        firstEntryPoint = callback;

        // see if this is duplicated in library form
        if (locate(firstLibrary, name) != NULL)
        {
            message.setResult(DUPLICATE_REGISTRATION);
        }
        else
        {
            message.setResult(REGISTRATION_COMPLETED);
        }
    }
    // make sure the data message buffer is not passed back.
    message.freeMessageData();
}

// General query by name only.  Can return either form of registraction.
// Message arguments have the following meanings:
//
// parameter1 -- registration type
// nameArg    -- The registration name
void RegistrationTable::queryCallback(ServiceMessage &message)
{
    ServiceRegistrationData *regData = (ServiceRegistrationData *)message.allocateMessageData(sizeof(ServiceRegistrationData));
    // get the argument name (local copy only)
    char *name = message.nameArg;

    // now check the exe version first.
    RegistrationData *callback = locate(name, message.session);
    // not found?  try a library version
    if (callback == NULL || callback->owner != message.session)
    {
        callback = locate(firstLibrary, name);
    }
    // copy the data into the buffer if we found one
    if (callback != NULL)
    {
        // copy the registration information
        callback->getRegistrationData(*regData);
        message.setResult(CALLBACK_EXISTS);
    }
    else
    {
        message.setExceptionInfo(CALLBACK_NOT_FOUND, message.nameArg);
    }
}


// General query by name and qualified module name.  Can return only the EXE version
// Message arguments have the following meanings:
//
// parameter1 -- registration type
// nameArg    -- The registration name
void RegistrationTable::queryLibraryCallback(ServiceMessage &message)
{
    // we're sent an extra registration block here with input data.  We can just
    // reuse this buffer to send the information back.
    ServiceRegistrationData *regData = (ServiceRegistrationData *)message.getMessageData();
    // get the argument name (local copy only)
    char *name = message.nameArg;
    char *module = regData->moduleName;

    // now check a library version first.
    RegistrationData *callback = locate(name, module);
    // copy the data if we found this
    if (callback != NULL)
    {
        // copy the registration information
        callback->getRegistrationData(*regData);
        message.setResult(CALLBACK_EXISTS);
    }
    else
    {
        message.setExceptionInfo(CALLBACK_NOT_FOUND, message.nameArg);
    }
}


// Update a library-based callback after a successful load event.
// Message arguments have the following meanings:
//
// parameter1 -- registration type
// nameArg    -- The registration name
void RegistrationTable::updateCallback(ServiceMessage &message)
{
    ServiceRegistrationData *regData = (ServiceRegistrationData *)message.getMessageData();
    // get the argument name (local copy only)
    char *name = message.nameArg;
    char *module = regData->moduleName;

    // now check a library version first.
    RegistrationData *callback = locate(name, module);
    // copy the data if we found this
    if (callback != NULL)
    {
        // we're only updating the entry point data
        callback->entryPoint = regData->entryPoint;
        message.setResult(CALLBACK_EXISTS);
    }
    else
    {
        message.setExceptionInfo(CALLBACK_NOT_FOUND, message.nameArg);
    }
    // make sure the data message buffer is not passed back.
    message.freeMessageData();
}


// Drop a callback by name only.
// Message arguments have the following meanings:
//
// parameter1 -- registration type
// nameArg    -- The registration name
void RegistrationTable::dropLibraryCallback(ServiceMessage &message)
{
    // get the argument name (local copy only)
    char *name = message.nameArg;

    // now check a library version first.
    RegistrationData *callback = locate(name);
    if (callback != NULL)
    {
        // an attempt to drop by somebody other than the owner?
        if (callback->dropAuthority == OWNER_ONLY && callback->owner != message.session)
        {
            message.setExceptionInfo(DROP_NOT_AUTHORIZED, message.nameArg);
        }
        else
        {

            // remove this session reference.
            callback->removeSessionReference(message.session);
            // still referenced by other processes?
            if (callback->hasReferences())
            {
                message.setExceptionInfo(DROP_NOT_AUTHORIZED, message.nameArg);
            }
            else
            {
                remove(&firstLibrary, callback);
                delete callback;
                message.setResult(CALLBACK_DROPPED);
            }
        }
    }
    else
    {
        message.setExceptionInfo(CALLBACK_NOT_FOUND, message.nameArg);
    }
}


// Drop a callback by qualified name.  Can only drop a library registration.
// Message arguments have the following meanings:
//
// parameter1 -- registration type
// nameArg    -- The registration name
void RegistrationTable::dropCallback(ServiceMessage &message)
{
    // get the argument name (local copy only)
    char *name = message.nameArg;

    // now check the exe version first.
    RegistrationData *callback = locate(name, message.session);
    // not found?  try a Library version
    if (callback == NULL)
    {
        callback = locate(firstLibrary, name);
    }
    // copy the data into the buffer if we found one
    if (callback != NULL)
    {
        // an attempt to drop by somebody other than the owner?
        if (callback->dropAuthority == OWNER_ONLY && callback->owner != message.session)
        {
            message.setExceptionInfo(DROP_NOT_AUTHORIZED, message.nameArg);
        }
        else
        {

            // remove this session reference.
            callback->removeSessionReference(message.session);
            // still referenced by other processes?
            if (callback->hasReferences())
            {
                message.setExceptionInfo(DROP_NOT_AUTHORIZED, message.nameArg);
            }
            else
            {
                if (callback->isEntryPoint())
                {
                    remove(&firstEntryPoint, callback);
                }
                else
                {
                    remove(&firstLibrary, callback);
                }
                delete callback;
                message.setResult(CALLBACK_DROPPED);
            }
        }
    }
    else
    {
        message.setExceptionInfo(CALLBACK_NOT_FOUND, message.nameArg);
    }
}

// search for a name-only registration
RegistrationData *RegistrationTable::locate(RegistrationData *anchor, char *name)
{
    RegistrationData *current = anchor;
    RegistrationData *previous = NULL;

    while (current != NULL)              /* while more queues          */
    {
        // find the one we want?
        if (current->matches(name))
        {
            return current;
        }
        previous = current;                /* remember this block        */
        current = current->next;           /* step to the next block     */
    }
    return NULL;
}

// search for a name-only registration
void RegistrationTable::remove(RegistrationData **anchor, RegistrationData *block)
{
    RegistrationData *current = *anchor;
    RegistrationData *previous = NULL;

    while (current != NULL)              /* while more queues          */
    {
        // find the one we want?
        if (current == block)
        {
            // is this the first one?
            if (previous == NULL)
            {
                // update the anchor position
                *anchor = current->next;
            }
            else
            {
                // close the chain and get out of here
                previous->next = current->next;
            }
            return;
        }
        previous = current;                /* remember this block        */
        current = current->next;           /* step to the next block     */
    }
}

// search for a library-type registration
RegistrationData *RegistrationTable::locate(char *name, char *module)
{
    RegistrationData *current = firstLibrary;
    RegistrationData *previous = NULL;

    while (current != NULL)              /* while more queues          */
    {
        // find the one we want?
        if (current->matches(name, module))
        {
            // move this to the front so we find it quickly
            reorderBlocks(firstLibrary, current, previous);
            return current;
        }
        previous = current;                /* remember this block        */
        current = current->next;           /* step to the next block     */
    }
    return NULL;
}

// search for a library-type registration
RegistrationData *RegistrationTable::locate(char *name)
{
    RegistrationData *callback = locate(firstLibrary, name);
    if (callback == NULL)
    {
        callback = locate(firstEntryPoint, name);
    }
    return callback;
}

// search for a local type registration
RegistrationData *RegistrationTable::locate(char *name, SessionID session)
{
    RegistrationData *current = firstEntryPoint;
    RegistrationData *previous = NULL;

    while (current != NULL)              /* while more queues          */
    {
        // find the one we want?
        if (current->matches(name, session))
        {
            // move this to the front so we find it quickly
            reorderBlocks(firstEntryPoint, current, previous);
            return current;
        }
        previous = current;                /* remember this block        */
        current = current->next;           /* step to the next block     */
    }
    return NULL;
}

void RegistrationTable::reorderBlocks(RegistrationData *& anchor, RegistrationData *current, RegistrationData *previous)
{
    if (previous != NULL)            // if we have a predecessor
    {
        // rearrange to get "most recently used" behavior
        previous->next = current->next;
        current->next = anchor;
        anchor = current;
    }
}


// It will remove all the registration entries for a specific process
void RegistrationTable::freeProcessEntries(SessionID session)
{
    RegistrationData *current = firstEntryPoint;
    RegistrationData *previous = NULL;

    while (current != NULL)
    {
        if (current->owner == session)
        {
            if (previous == NULL)
            {
                firstEntryPoint = current->next;
                delete current;
                current = firstEntryPoint;
            }
            else
            {
                previous->next = current->next;
                delete current;
                current = current->next;
            }
        }
        else
        {
            previous = current;
            current = current->next;
        }
    }
}



// It will remove all the registration entries for a specific process
void ServerRegistrationManager::freeProcessRegistrations(SessionID session)
{
    // delete all of the entries associated with this process.
    functions.freeProcessEntries(session);
    exits.freeProcessEntries(session);
    commandHandlers.freeProcessEntries(session);
}


void ServerRegistrationManager::dispatch(ServiceMessage &message)
{
    RegistrationTable *table = NULL;
    switch ((RegistrationType)message.parameter1)
    {
        case FunctionAPI:
            table = &functions;
            break;
        case ExitAPI:
            table = &exits;
            break;
        case SubcomAPI:
            table = &commandHandlers;
            break;
        default:
            message.setExceptionInfo(SERVER_FAILURE, "Invalid registration type");
            return;
    }


    switch (message.operation)
    {
        // registration manager operations
        case REGISTER_LIBRARY:
            table->registerLibraryCallback(message);
            break;
        case REGISTER_ENTRYPOINT:
            table->registerCallback(message);
            break;
        case REGISTER_DROP:
            table->dropCallback(message);
            break;
        case REGISTER_DROP_LIBRARY:
            table->dropLibraryCallback(message);
            break;
        case REGISTER_QUERY:
            table->queryCallback(message);
            break;
        case REGISTER_QUERY_LIBRARY:
            table->queryLibraryCallback(message);
            break;
        case REGISTER_LOAD_LIBRARY:
            table->queryLibraryCallback(message);
            break;
        case UPDATE_CALLBACK:
            table->updateCallback(message);
            break;

        default:
            message.setExceptionInfo(SERVER_FAILURE, "Invalid registration manager operation");
            break;
    }
}

void ServerRegistrationManager::cleanupProcessResources(SessionID session)
{
    // just free up any registrations associated with this
    // session.
    freeProcessRegistrations(session);
}
