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
/* THIS SOFTWARE IS PROVIDED BY THE COPYright HOLDERS AND CONTRIBUTORS        */
/* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT          */
/* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS          */
/* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYright   */
/* OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,      */
/* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED   */
/* TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,        */
/* OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY     */
/* OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING    */
/* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS         */
/* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.               */
/*                                                                            */
/*----------------------------------------------------------------------------*/

#include <rexx.h>
#include <oorexxapi.h>
#include <string.h>

RexxMethod1(RexxObjectPtr,              // Return type
            TestCreateQueue,            // Method name
            OPTIONAL_CSTRING, qname)    // Queue name
{
    char newQueueName[MAX_QUEUE_NAME_LENGTH];
    size_t flag;

    RexxReturnCode rc = RexxCreateQueue(newQueueName, sizeof(newQueueName),
                                        qname, &flag);
    context->SetObjectVariable("RETC", context->Int32ToObject(rc));
    context->SetObjectVariable("FLAG", context->Int32ToObject(flag));
    return context->NewStringFromAsciiz(newQueueName);
}

RexxMethod1(int,                        // Return type
            TestOpenQueue,              // Method name
            CSTRING, qname)             // Queue name
{
    size_t flag;

    RexxReturnCode rc = RexxOpenQueue(qname, &flag);
    context->SetObjectVariable("RETC", context->Int32ToObject(rc));
    context->SetObjectVariable("FLAG", context->Int32ToObject(flag));
    return rc;
}

RexxMethod1(int,                        // Return type
            TestQueueExists,            // Method name
            CSTRING, qname)             // Queue name
{
    RexxReturnCode rc = RexxQueueExists(qname);
    context->SetObjectVariable("RETC", context->Int32ToObject(rc));
    return rc;
}

RexxMethod1(int,                        // Return type
            TestDeleteQueue,            // Method name
            CSTRING, qname)             // Queue name
{
    RexxReturnCode rc = RexxDeleteQueue(qname);
    context->SetObjectVariable("RETC", context->Int32ToObject(rc));
    return rc;
}

RexxMethod1(int,                        // Return type
            TestQueryQueue,             // Method name
            CSTRING, qname)             // Queue name
{
    size_t count;

    RexxReturnCode rc = RexxQueryQueue(qname, &count);
    context->SetObjectVariable("RETC", context->Int32ToObject(rc));
    context->SetObjectVariable("FLAG", context->Int32ToObject(count));
    return rc;
}

RexxMethod3(int,                        // Return type
            TestAddQueue,               // Method name
            CSTRING, qname,             // Queue name
            CSTRING, data,              // Queue data to add
            int, type)                  // Queue FIFO/LIFO flag
{
    CONSTRXSTRING rxdata;

    rxdata.strptr = data;
    rxdata.strlength = strlen(data);
    RexxReturnCode rc = RexxAddQueue(qname, &rxdata, type);
    context->SetObjectVariable("RETC", context->Int32ToObject(rc));
    return rc;
}

RexxMethod1(int,                        // Return type
            TestPullFromQueue,          // Method name
            CSTRING, qname)             // Queue name
{
    RXSTRING data;
    REXXDATETIME timestamp;

    RexxReturnCode rc = RexxPullFromQueue(qname, &data, &timestamp, 0);
    context->SetObjectVariable("RETC", context->Int32ToObject(rc));
    context->SetObjectVariable("FLAG", context->NewStringFromAsciiz(data.strptr));
    return rc;
}

RexxMethod1(int,                        // Return type
            TestClearQueue,             // Method name
            CSTRING, qname)             // Queue name
{
    RexxReturnCode rc = RexxClearQueue(qname);
    context->SetObjectVariable("RETC", context->Int32ToObject(rc));
    return rc;
}

RexxMethod1(int,                        // Return type
            TestAllocateFreeMemory,     // Method name
            int, size)                  // Size of memory block
{
    void *block = RexxAllocateMemory(size);
    if (block == NULL) {
        return 1;
    }
    return RexxFreeMemory(block);
}


RexxMethodEntry orxtest_methods[] = {
    REXX_METHOD(TestCreateQueue,        TestCreateQueue),
    REXX_METHOD(TestOpenQueue,          TestOpenQueue),
    REXX_METHOD(TestQueueExists,        TestQueueExists),
    REXX_METHOD(TestDeleteQueue,        TestDeleteQueue),
    REXX_METHOD(TestQueryQueue,         TestQueryQueue),
    REXX_METHOD(TestAddQueue,           TestAddQueue),
    REXX_METHOD(TestPullFromQueue,      TestPullFromQueue),
    REXX_METHOD(TestClearQueue,         TestClearQueue),
    REXX_METHOD(TestAllocateFreeMemory, TestAllocateFreeMemory),
    REXX_LAST_METHOD()
};


RexxPackageEntry UnitTest_package_entry = {
    STANDARD_PACKAGE_HEADER
    REXX_INTERPRETER_4_0_0,              // anything after 4.0.0 will work
    "UnitTest",                          // name of the package
    "1.0.0",                             // package information
    NULL,                                // no load/unload functions
    NULL,
    NULL,                                // the exported routines
    orxtest_methods                      // the exported methods
};

// package loading stub.
OOREXX_GET_PACKAGE(UnitTest);

