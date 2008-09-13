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


-- These are the real external functions we are testing
::routine TestZeroIntArgs EXTERNAL "LIBRARY orxfunction TestZeroIntArgs"
::routine TestOneIntArg EXTERNAL "LIBRARY orxfunction TestOneIntArg"
::routine TestTwoIntArgs EXTERNAL "LIBRARY orxfunction TestTwoIntArgs"
::routine TestThreeIntArgs EXTERNAL "LIBRARY orxfunction TestThreeIntArgs"
::routine TestFourIntArgs EXTERNAL "LIBRARY orxfunction TestFourIntArgs"
::routine TestFiveIntArgs EXTERNAL "LIBRARY orxfunction TestFiveIntArgs"
::routine TestSixIntArgs EXTERNAL "LIBRARY orxfunction TestSixIntArgs"
::routine TestSevenIntArgs EXTERNAL "LIBRARY orxfunction TestSevenIntArgs"
::routine TestEightIntArgs EXTERNAL "LIBRARY orxfunction TestEightIntArgs"
::routine TestNineIntArgs EXTERNAL "LIBRARY orxfunction TestNineIntArgs"
::routine TestTenIntArgs EXTERNAL "LIBRARY orxfunction TestTenIntArgs"
::routine TestIntArg EXTERNAL "LIBRARY orxfunction TestIntArg"
::routine TestInt32Arg EXTERNAL "LIBRARY orxfunction TestInt32Arg"
::routine TestUint32Arg EXTERNAL "LIBRARY orxfunction TestUint32Arg"
::routine TestInt8Arg EXTERNAL "LIBRARY orxfunction TestInt8Arg"
::routine TestUint8Arg EXTERNAL "LIBRARY orxfunction TestUint8Arg"
::routine TestInt16Arg EXTERNAL "LIBRARY orxfunction TestInt16Arg"
::routine TestUint16Arg EXTERNAL "LIBRARY orxfunction TestUint16Arg"
::routine TestInt64Arg EXTERNAL "LIBRARY orxfunction TestInt64Arg"
::routine TestUint64Arg EXTERNAL "LIBRARY orxfunction TestUint64Arg"
::routine TestIntPtrArg EXTERNAL "LIBRARY orxfunction TestIntPtrArg"
::routine TestUintPtrArg EXTERNAL "LIBRARY orxfunction TestUintPtrArg"
::routine TestWholeNumberArg EXTERNAL "LIBRARY orxfunction TestWholeNumberArg"
::routine TestStringSizeArg EXTERNAL "LIBRARY orxfunction TestStringSizeArg"
::routine TestSizeArg EXTERNAL "LIBRARY orxfunction TestSizeArg"
::routine TestSSizeArg EXTERNAL "LIBRARY orxfunction TestSSizeArg"
::routine TestLogicalArg EXTERNAL "LIBRARY orxfunction TestLogicalArg"
::routine TestFloatArg EXTERNAL "LIBRARY orxfunction TestFloatArg"
::routine TestDoubleArg EXTERNAL "LIBRARY orxfunction TestDoubleArg"
::routine TestCstringArg EXTERNAL "LIBRARY orxfunction TestCstringArg"
::routine TestPointerValue EXTERNAL "LIBRARY orxfunction TestPointerValue"
::routine TestPointerArg EXTERNAL "LIBRARY orxfunction TestPointerArg"
::routine TestNullPointerValue EXTERNAL "LIBRARY orxfunction TestNullPointerValue"
::routine TestPointerStringValue EXTERNAL "LIBRARY orxfunction TestPointerStringValue"
::routine TestPointerStringArg EXTERNAL "LIBRARY orxfunction TestPointerStringArg"
::routine TestNullPointerStringValue EXTERNAL "LIBRARY orxfunction TestNullPointerStringValue"
::routine TestStemArg EXTERNAL "LIBRARY orxfunction TestStemArg"
::routine TestObjectToValue EXTERNAL "LIBRARY orxfunction TestObjectToValue"

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
