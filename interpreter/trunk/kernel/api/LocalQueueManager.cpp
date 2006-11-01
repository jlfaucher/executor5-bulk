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

#include "LocalQueueManager.hpp"
#include "Encodings.hpp"
#include "LocalAPIManager.hpp"
#include "SysLocalAPIManager.hpp"
#include "oorexx.h"
#include "ClientMessage.hpp"
#include "SysUtil.hpp"
#include <stdio.h>
#include <ctype.h>


void LocalQueueManager::validateQueueName(char *username)
{
   char       *valptr;                 /* Used to validate name.     */
   char        ch;
   size_t      namelen;                /* Length of the user's name. */

   if (username == NULL)               /* NULL is OK.                */
   {
       return;
   }
   // "SESSION" is a reserved name, reject this in this context
   if (SysUtil::stricmp(username, "SESSION") == 0)
   {
       throw new ServiceException(INVALID_QUEUE_NAME, username);
   }

   namelen = strlen(username);
   if (namelen > 0 && namelen < MAX_QUEUE_NAME_LENGTH)
   {
       valptr = username;                /* point to name              */
       while((ch = *(valptr++))) {       /* While have not reached end */
           ch = toupper(ch);               /* convert to upper case      */
           if (!isalpha(ch) && !isdigit(ch) && ch != ch_PERIOD &&
               ch != ch_QUESTION_MARK && ch != ch_EXCLAMATION && ch != ch_UNDERSCORE)
           {
               throw new ServiceException(INVALID_QUEUE_NAME, username);
           }
       }
   }
   else
   {
       throw new ServiceException(INVALID_QUEUE_NAME, username);
   }
}

void LocalQueueManager::initializeLocal(LocalAPIManager *a)
{
    localManager = a;
    LocalAPISubsystem::initializeLocal(a);
    // find the session queue
    sessionQueue = initializeSessionQueue(a->getSession());
}

void LocalQueueManager::terminateProcess()
{
    // if we have a session queue
    if (sessionQueue != 0)
    {
        deleteSessionQueue();    // try to delete this
    }
                                 // continue the shutdown
    LocalAPISubsystem::terminateProcess();
}


QueueHandle LocalQueueManager::initializeSessionQueue(SessionID session)
{
    // first check to see if we have an env variable set...if we do we
    // inherit from our parent session
    QueueHandle mysessionQueue;
    // we could be inheriting the session queue from a caller process...check first.
    if (SysLocalAPIManager::getActiveSessionQueue(mysessionQueue))
    {
        nestSessionQueue(mysessionQueue);      // make sure we update the nest count
    }
    else
    {
        // create a new session queue
        mysessionQueue = createSessionQueue(session);
        SysLocalAPIManager::setActiveSessionQueue(mysessionQueue);
    }
    return mysessionQueue;
}

QueueHandle LocalQueueManager::createSessionQueue(SessionID session)
{
    ClientMessage message(QueueManager, CREATE_SESSION_QUEUE, session);
    message.send();
    // the handle is returned in the first parameter
    return (QueueHandle)message.parameter1;
}

bool LocalQueueManager::createNamedQueue(char *name, size_t size, char *createdName)
{
    validateQueueName(name);            // make sure this is a valid name

    ClientMessage message(QueueManager, CREATE_NAMED_QUEUE, name);

    strcpy(message.nameArg, name);
    message.send();
    strncpy(createdName, message.nameArg, size);
    // return the dup name indicator
    return message.result == DUPLICATE_QUEUE_NAME;
}

void LocalQueueManager::deleteSessionQueue()
{
    ClientMessage message(QueueManager, DELETE_SESSION_QUEUE, sessionQueue);
    message.send();
}

void LocalQueueManager::deleteNamedQueue(char *name)
{
    validateQueueName(name);            // make sure this is a valid name

    ClientMessage message(QueueManager, DELETE_NAMED_QUEUE, name);
    message.send();
}

size_t LocalQueueManager::getSessionQueueCount()
{
    ClientMessage message(QueueManager, GET_SESSION_QUEUE_COUNT, sessionQueue);

    message.send();
    // the handle is returned in the first parameter
    return (size_t)message.parameter1;
}

size_t LocalQueueManager::getQueueCount(char *name)
{
    validateQueueName(name);            // make sure this is a valid name

    ClientMessage message(QueueManager, GET_NAMED_QUEUE_COUNT, name);

    message.send();
    // the handle is returned in the first parameter
    return (size_t)message.parameter1;
}

void LocalQueueManager::clearSessionQueue()
{
    ClientMessage message(QueueManager, CLEAR_SESSION_QUEUE, sessionQueue);

    message.send();
}

void LocalQueueManager::clearNamedQueue(char *name)
{
    validateQueueName(name);            // make sure this is a valid name

    ClientMessage message(QueueManager, CLEAR_NAMED_QUEUE, name);

    message.send();
}


void LocalQueueManager::addToNamedQueue(char *name, ManagedRxstring &data, uintptr_t lifoFifo)
{
    ClientMessage message(QueueManager, ADD_TO_NAMED_QUEUE, name);
                                           // set the additional arguments
    message.parameter1 = data.strlength;
    message.parameter2 = lifoFifo;     // make sure we have the add order

    // attach the queue item to the message.
    message.setMessageData(data.strptr, data.strlength);
    message.send();
}


void LocalQueueManager::addToSessionQueue(ManagedRxstring &data, uintptr_t lifoFifo)
{
    ClientMessage message(QueueManager, ADD_TO_SESSION_QUEUE);

                                       // set the additional arguments
    message.parameter1 = data.strlength;
    message.parameter2 = lifoFifo;     // make sure we have the add order
    message.parameter3 = sessionQueue; // set the session handle next
    // attach the queue item to the message.
    message.setMessageData(data.strptr, data.strlength);

    message.send();
}

void LocalQueueManager::pullFromQueue(char *name, RxString &data, uintptr_t waitFlag, RexxTimeStamp *timeStamp)
{
    ClientMessage message(QueueManager, PULL_FROM_NAMED_QUEUE);
    // set up for either name or session queue read
    if (name != NULL)
    {
        strcpy(message.nameArg, name);
    }
    else
    {
        message.operation = PULL_FROM_SESSION_QUEUE;
        message.parameter3 = sessionQueue;
    }
    message.parameter1 = waitFlag != 0 ? QUEUE_WAIT_FOR_DATA : QUEUE_NO_WAIT;
    message.send();
    MakeRxString(data, message.getMessageData(), message.getMessageDataLength());
}

void LocalQueueManager::nestSessionQueue(QueueHandle q)
{
    ClientMessage message(QueueManager, NEST_SESSION_QUEUE, sessionQueue);
    message.send();
}


RexxReturnCode LocalQueueManager::processServiceException(ServiceException *e)
{
    switch (e->getErrorCode())
    {
        case INVALID_QUEUE_NAME:
            return RXQUEUE_BADQNAME;

        case BAD_FIFO_LIFO:
            return RXQUEUE_PRIORITY;

        case BAD_WAIT_FLAG:
            return RXQUEUE_BADWAITFLAG;

        case QUEUE_DOES_NOT_EXIST:
            return RXQUEUE_NOTREG;

        case QUEUE_IN_USE:
            return RXQUEUE_ACCESS;

        default:
            return RXQUEUE_MEMFAIL;
    }
}
