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

#include <oorexxapi.h>


RexxMethod0(int,                       // Return type
            TestZeroIntArgs)            // Object_method name
{
    return 0;
}

RexxMethod1(int,                       // Return type
            TestOneIntArg,              // Object_method name
            int, arg1)                 // Argument
{
    return arg1;
}

RexxMethod2(int,                       // Return type
            TestTwoIntArgs,             // Object_method name
            int, arg1,                 // Argument
            int, arg2)                 // Argument
{
    return arg1 + arg2;
}

RexxMethod3(int,                       // Return type
            TestThreeIntArgs,           // Object_method name
            int, arg1,                 // Argument
            int, arg2,                 // Argument
            int, arg3)                 // Argument
{
    return arg1 + arg2 + arg3;
}

RexxMethod4(int,                       // Return type
            TestFourIntArgs,            // Object_method name
            int, arg1,                 // Argument
            int, arg2,                 // Argument
            int, arg3,                 // Argument
            int, arg4)                 // Argument
{
    return arg1 + arg2 + arg3 + arg4;
}

RexxMethod5(int,                       // Return type
            TestFiveIntArgs,            // Object_method name
            int, arg1,                 // Argument
            int, arg2,                 // Argument
            int, arg3,                 // Argument
            int, arg4,                 // Argument
            int, arg5)                 // Argument
{
    return arg1 + arg2 + arg3 + arg4 + arg5;
}

RexxMethod6(int,                       // Return type
            TestSixIntArgs,             // Object_method name
            int, arg1,                 // Argument
            int, arg2,                 // Argument
            int, arg3,                 // Argument
            int, arg4,                 // Argument
            int, arg5,                 // Argument
            int, arg6)                 // Argument
{
    return arg1 + arg2 + arg3 + arg4 + arg5 + arg6;
}

RexxMethod7(int,                       // Return type
            TestSevenIntArgs,           // Object_method name
            int, arg1,                 // Argument
            int, arg2,                 // Argument
            int, arg3,                 // Argument
            int, arg4,                 // Argument
            int, arg5,                 // Argument
            int, arg6,                 // Argument
            int, arg7)                 // Argument
{
    return arg1 + arg2 + arg3 + arg4 + arg5 + arg6 + arg7;
}

RexxMethod8(int,                       // Return type
            TestEightIntArgs,           // Object_method name
            int, arg1,                 // Argument
            int, arg2,                 // Argument
            int, arg3,                 // Argument
            int, arg4,                 // Argument
            int, arg5,                 // Argument
            int, arg6,                 // Argument
            int, arg7,                 // Argument
            int, arg8)                 // Argument
{
    return arg1 + arg2 + arg3 + arg4 + arg5 + arg6 + arg7 + arg8;
}

RexxMethod9(int,                       // Return type
            TestNineIntArgs,            // Object_method name
            int, arg1,                 // Argument
            int, arg2,                 // Argument
            int, arg3,                 // Argument
            int, arg4,                 // Argument
            int, arg5,                 // Argument
            int, arg6,                 // Argument
            int, arg7,                 // Argument
            int, arg8,                 // Argument
            int, arg9)                 // Argument
{
    return arg1 + arg2 + arg3 + arg4 + arg5 + arg6 + arg7 + arg8 + arg9;
}

RexxMethod10(int,                       // Return type
            TestTenIntArgs,             // Object_method name
            int, arg1,                 // Argument
            int, arg2,                 // Argument
            int, arg3,                 // Argument
            int, arg4,                 // Argument
            int, arg5,                 // Argument
            int, arg6,                 // Argument
            int, arg7,                 // Argument
            int, arg8,                 // Argument
            int, arg9,                 // Argument
            int, arg10)                // Argument
{
    return arg1 + arg2 + arg3 + arg4 + arg5 + arg6 + arg7 + arg8 + arg9 + arg10;
}

RexxMethod1(int,                       // Return type
            TestIntArg,                 // Object_method name
            int, arg1)                 // Argument
{
    return arg1;
}

RexxMethod1(int32_t,                   // Return type
            TestInt32Arg,               // Function routine name
            int32_t, arg1)             // Argument
{
    return arg1;
}

RexxMethod1(uint32_t,                  // Return type
            TestUint32Arg,              // Function routine name
            uint32_t, arg1)            // Argument
{
    return arg1;
}

RexxMethod1(int8_t,                    // Return type
            TestInt8Arg,                // Function routine name
            int8_t, arg1)              // Argument
{
    return arg1;
}

RexxMethod1(uint8_t,                   // Return type
            TestUint8Arg,               // Function routine name
            uint8_t, arg1)             // Argument
{
    return arg1;
}

RexxMethod1(int16_t,                   // Return type
            TestInt16Arg,               // Function routine name
            int16_t, arg1)             // Argument
{
    return arg1;
}

RexxMethod1(uint16_t,                  // Return type
            TestUint16Arg,              // Function routine name
            uint16_t, arg1)            // Argument
{
    return arg1;
}

RexxMethod1(int64_t,                   // Return type
            TestInt64Arg,               // Function routine name
            int64_t, arg1)             // Argument
{
    return arg1;
}

RexxMethod1(uint64_t,                  // Return type
            TestUint64Arg,              // Function routine name
            uint64_t, arg1)            // Argument
{
    return arg1;
}

RexxMethod1(intptr_t,                  // Return type
            TestIntPtrArg,               // Function routine name
            intptr_t, arg1)            // Argument
{
    return arg1;
}

RexxMethod1(uintptr_t,                 // Return type
            TestUintPtrArg,              // Function routine name
            uintptr_t, arg1)           // Argument
{
    return arg1;
}

RexxMethod1(wholenumber_t,             // Return type
            TestWholeNumberArg,         // Function routine name
            wholenumber_t, arg1)       // Argument
{
    return arg1;
}

RexxMethod1(stringsize_t,              // Return type
            TestStringSizeArg,          // Function routine name
            stringsize_t, arg1)        // Argument
{
    return arg1;
}

RexxMethod1(logical_t,                 // Return type
            TestLogicalArg,             // Function routine name
            logical_t, arg1)           // Argument
{
    return arg1;
}

RexxMethod1(float,                     // Return type
            TestFloatArg,               // Function routine name
            float, arg1)               // Argument
{
    return arg1;
}

RexxMethod1(double,                    // Return type
            TestDoubleArg,              // Function routine name
            double, arg1)              // Argument
{
    return arg1;
}

RexxMethod1(CSTRING,                   // Return type
            TestCstringArg,             // Function routine name
            CSTRING, arg1)             // Argument
{
    return arg1;
}

RexxMethod0(POINTER,                   // Return type
           TestPointerValue)            // Function routine name
{
    return (void *)TestPointerValue;
}

RexxMethod1(logical_t,                 // Return type
           TestPointerArg,              // Function routine name
           POINTER, arg1)              // Argument
{
    if (arg1 == TestPointerValue) {
        return 1;
    }
    return 0;
}

RexxMethod0(POINTER,                   // Return type
           TestNullPointerValue)        // Function routine name
{
    return NULL;
}


RexxMethodEntry orxtest_methods[] = {
    REXX_METHOD(TestZeroIntArgs,       TestZeroIntArgs),
    REXX_METHOD(TestOneIntArg,         TestOneIntArg),
    REXX_METHOD(TestTwoIntArgs,        TestTwoIntArgs),
    REXX_METHOD(TestThreeIntArgs,      TestThreeIntArgs),
    REXX_METHOD(TestFourIntArgs,       TestFourIntArgs),
    REXX_METHOD(TestFiveIntArgs,       TestFiveIntArgs),
    REXX_METHOD(TestSixIntArgs,        TestSixIntArgs),
    REXX_METHOD(TestSevenIntArgs,      TestSevenIntArgs),
    REXX_METHOD(TestEightIntArgs,      TestEightIntArgs),
    REXX_METHOD(TestNineIntArgs,       TestNineIntArgs),
    REXX_METHOD(TestTenIntArgs,        TestTenIntArgs),
    REXX_METHOD(TestIntArg,            TestIntArg),
    REXX_METHOD(TestInt32Arg,          TestInt32Arg),
    REXX_METHOD(TestUint32Arg,         TestUint32Arg),
    REXX_METHOD(TestInt8Arg,           TestInt8Arg),
    REXX_METHOD(TestUint8Arg,          TestUint8Arg),
    REXX_METHOD(TestInt16Arg,          TestInt16Arg),
    REXX_METHOD(TestUint16Arg,         TestUint16Arg),
    REXX_METHOD(TestInt64Arg,          TestInt64Arg),
    REXX_METHOD(TestUint64Arg,         TestUint64Arg),
    REXX_METHOD(TestIntPtrArg,         TestIntPtrArg),
    REXX_METHOD(TestUintPtrArg,        TestUintPtrArg),
    REXX_METHOD(TestWholeNumberArg,    TestWholeNumberArg),
    REXX_METHOD(TestStringSizeArg,     TestStringSizeArg),
    REXX_METHOD(TestLogicalArg,        TestLogicalArg),
    REXX_METHOD(TestFloatArg,          TestFloatArg),
    REXX_METHOD(TestDoubleArg,         TestDoubleArg),
    REXX_METHOD(TestCstringArg,        TestCstringArg),
    REXX_METHOD(TestPointerValue,      TestPointerValue),
    REXX_METHOD(TestPointerArg,        TestPointerArg),
    REXX_METHOD(TestNullPointerValue,  TestNullPointerValue),
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

