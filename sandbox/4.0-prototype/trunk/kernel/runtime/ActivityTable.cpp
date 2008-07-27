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
/******************************************************************************/
/* REXX Kernel                                          ActivityTable.cpp     */
/*                                                                            */
/* Activity Storage Table for High Thread IDs                                 */
/*                                                                            */
/******************************************************************************/

#include "RexxCore.h"
#include "ActivityTable.hpp"
#include "SynchronizedBlock.hpp"
#include "Interpreter.hpp"

#define INITIALSIZE 512

ActivityTable::ActivityTable()
{
    for (int i = 0; i < ACTIVITY_TABLE_BUCKETS; i++)
    {
        bucket[i] = OREF_NULL;
    }
}


void ActivityTable::add(RexxActivity *newActivity)
{
    // protect this on a local thread basis
    CriticalBlock block;

    size_t bucketId = newActivity->_thread.hash() / ACTIVITY_TABLE_BUCKETS;
    newActivity->nextProcessActivity = bucket[bucketId];
    bucket[bucketId] = newActivity;
}


void ActivityTable::remove(RexxActivity *activity)
{
    // protect this on a local thread basis
    CriticalBlock block;

    size_t bucketId = activity->_thread.hash() / ACTIVITY_TABLE_BUCKETS;
    RexxActivity *current = bucket[bucketId];
    RexxActivity *previous = OREF_NULL;

    while (current != OREF_NULL)
    {
        // if we found a match
        if (current == activity)
        {
            if (previous == OREF_NULL)
            {
                bucket[bucketId] = activity->nextProcessActivity;
            }
            else
            {
                previous->nextProcessActivity = activity->nextProcessActivity;
            }
            return;
        }

    }
}


RexxActivity *ActivityTable::getActivityForThread(RexxThread &tid)
{
    CriticalBlock block;

    size_t bucketId = tid.hash() / ACTIVITY_TABLE_BUCKETS;
    RexxActivity *current = bucket[bucketId];
    RexxActivity *previous = OREF_NULL;

    while (current != OREF_NULL)
    {
        // if we found a match
        if (current->_thread.equals(tid))
        {
            return current;
        }
    }
    return OREF_NULL;
}

