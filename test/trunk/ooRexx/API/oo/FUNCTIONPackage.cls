/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2008-2019 Rexx Language Association. All rights reserved.    */
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


-- These are the real external functions we are testing
::routine TestZeroIntArgs PUBLIC EXTERNAL "LIBRARY orxfunction TestZeroIntArgs"
::routine TestOneIntArg PUBLIC EXTERNAL "LIBRARY orxfunction TestOneIntArg"
::routine TestTwoIntArgs PUBLIC EXTERNAL "LIBRARY orxfunction TestTwoIntArgs"
::routine TestThreeIntArgs PUBLIC EXTERNAL "LIBRARY orxfunction TestThreeIntArgs"
::routine TestFourIntArgs PUBLIC EXTERNAL "LIBRARY orxfunction TestFourIntArgs"
::routine TestFiveIntArgs PUBLIC EXTERNAL "LIBRARY orxfunction TestFiveIntArgs"
::routine TestSixIntArgs PUBLIC EXTERNAL "LIBRARY orxfunction TestSixIntArgs"
::routine TestSevenIntArgs PUBLIC EXTERNAL "LIBRARY orxfunction TestSevenIntArgs"
::routine TestEightIntArgs PUBLIC EXTERNAL "LIBRARY orxfunction TestEightIntArgs"
::routine TestNineIntArgs PUBLIC EXTERNAL "LIBRARY orxfunction TestNineIntArgs"
::routine TestTenIntArgs PUBLIC EXTERNAL "LIBRARY orxfunction TestTenIntArgs"
::routine TestIntArg PUBLIC EXTERNAL "LIBRARY orxfunction TestIntArg"
::routine TestInt32Arg PUBLIC EXTERNAL "LIBRARY orxfunction TestInt32Arg"
::routine TestUint32Arg PUBLIC EXTERNAL "LIBRARY orxfunction TestUint32Arg"
::routine TestInt8Arg PUBLIC EXTERNAL "LIBRARY orxfunction TestInt8Arg"
::routine TestUint8Arg PUBLIC EXTERNAL "LIBRARY orxfunction TestUint8Arg"
::routine TestInt16Arg PUBLIC EXTERNAL "LIBRARY orxfunction TestInt16Arg"
::routine TestUint16Arg PUBLIC EXTERNAL "LIBRARY orxfunction TestUint16Arg"
::routine TestInt64Arg PUBLIC EXTERNAL "LIBRARY orxfunction TestInt64Arg"
::routine TestUint64Arg PUBLIC EXTERNAL "LIBRARY orxfunction TestUint64Arg"
::routine TestIntPtrArg PUBLIC EXTERNAL "LIBRARY orxfunction TestIntPtrArg"
::routine TestUintPtrArg PUBLIC EXTERNAL "LIBRARY orxfunction TestUintPtrArg"
::routine TestWholeNumberArg PUBLIC EXTERNAL "LIBRARY orxfunction TestWholeNumberArg"
::routine TestPositiveWholeNumberArg PUBLIC EXTERNAL "LIBRARY orxfunction TestPositiveWholeNumberArg"
::routine TestNonnegativeWholeNumberArg PUBLIC EXTERNAL "LIBRARY orxfunction TestNonnegativeWholeNumberArg"
::routine TestStringSizeArg PUBLIC EXTERNAL "LIBRARY orxfunction TestStringSizeArg"
::routine TestSizeArg PUBLIC EXTERNAL "LIBRARY orxfunction TestSizeArg"
::routine TestSSizeArg PUBLIC EXTERNAL "LIBRARY orxfunction TestSSizeArg"
::routine TestLogicalArg PUBLIC EXTERNAL "LIBRARY orxfunction TestLogicalArg"
::routine TestFloatArg PUBLIC EXTERNAL "LIBRARY orxfunction TestFloatArg"
::routine TestDoubleArg PUBLIC EXTERNAL "LIBRARY orxfunction TestDoubleArg"
::routine TestCstringArg PUBLIC EXTERNAL "LIBRARY orxfunction TestCstringArg"
::routine TestOptionalCstringArg PUBLIC EXTERNAL "LIBRARY orxfunction"
::routine TestStringArg          PUBLIC EXTERNAL "LIBRARY orxfunction"
::routine TestOptionalStringArg  PUBLIC EXTERNAL "LIBRARY orxfunction"
::routine TestPointerValue PUBLIC EXTERNAL "LIBRARY orxfunction TestPointerValue"
::routine TestPointerArg PUBLIC EXTERNAL "LIBRARY orxfunction TestPointerArg"
::routine TestNullPointerValue PUBLIC EXTERNAL "LIBRARY orxfunction TestNullPointerValue"
::routine TestPointerStringValue PUBLIC EXTERNAL "LIBRARY orxfunction TestPointerStringValue"
::routine TestPointerStringArg PUBLIC EXTERNAL "LIBRARY orxfunction TestPointerStringArg"
::routine TestNullPointerStringValue PUBLIC EXTERNAL "LIBRARY orxfunction TestNullPointerStringValue"
::routine TestStemArg PUBLIC EXTERNAL "LIBRARY orxfunction TestStemArg"
::routine TestObjectToValue PUBLIC EXTERNAL "LIBRARY orxfunction TestObjectToValue"
::routine TestArglistArg PUBLIC EXTERNAL "LIBRARY orxfunction TestArglistArg"
::routine TestNameArg PUBLIC EXTERNAL "LIBRARY orxfunction TestNameArg"
::routine TestNameArg2 PUBLIC EXTERNAL "LIBRARY orxfunction TestNameArg"        -- extra definition for name testing
::routine TestGetRoutineName PUBLIC EXTERNAL "LIBRARY orxfunction TestGetRoutineName"
::routine TestGetRoutineName2 PUBLIC EXTERNAL "LIBRARY orxfunction TestGetRoutineName"    -- extra definition for name testing
::routine TestGetArguments PUBLIC EXTERNAL "LIBRARY orxfunction TestGetArguments"
::routine TestGetArgument PUBLIC EXTERNAL "LIBRARY orxfunction TestGetArgument"
::routine TestGetRoutine PUBLIC EXTERNAL "LIBRARY orxfunction TestGetRoutine"
::routine TestGetRoutine2 PUBLIC EXTERNAL "LIBRARY orxfunction TestGetRoutine"      -- extra definition for routine testing
::routine TestGetContextVariable PUBLIC EXTERNAL "LIBRARY orxfunction TestGetContextVariable"
::routine TestSetContextVariable PUBLIC EXTERNAL "LIBRARY orxfunction TestSetContextVariable"
::routine TestDropContextVariable PUBLIC EXTERNAL "LIBRARY orxfunction TestDropContextVariable"
::routine TestGetAllContextVariables PUBLIC EXTERNAL "LIBRARY orxfunction TestGetAllContextVariables"
::routine TestGetContextDigits PUBLIC EXTERNAL "LIBRARY orxfunction TestGetContextDigits"
::routine TestGetContextFuzz PUBLIC EXTERNAL "LIBRARY orxfunction TestGetContextFuzz"
::routine TestGetContextForm PUBLIC EXTERNAL "LIBRARY orxfunction TestGetContextForm"
::routine TestResolveStemVariable PUBLIC EXTERNAL "LIBRARY orxfunction TestResolveStemVariable"
::routine TestFindContextClass PUBLIC EXTERNAL "LIBRARY orxfunction TestFindContextClass"
::routine TestAddCommandEnvironment PUBLIC EXTERNAL "LIBRARY orxfunction"

::class TypeConstants PUBLIC
::constant ObjectType 11
::constant IntType    12
::constant WholeNumberType  13
::constant DoubleType  14
::constant CStringType 15
::constant PointerType 16
::constant StringObjectType 17
::constant StringSizeType 18
::constant FloatType 19
::constant Int8Type  20
::constant Int16Type 21
::constant Int32Type 22
::constant Int64Type 23
::constant Uint8Type 24
::constant Uint16Type 25
::constant Uint32Type 26
::constant Uint64Type 27
::constant IntptrType 28
::constant UintptrType 29
::constant LogicalType 30
::constant StemType    32
::constant SizeType    33
::constant SSizeType   34
::constant PointerStringype   35
