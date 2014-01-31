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


::class 'CONVERSIONtester' public

-- These are the real external functions we are testing
::method TestObjectToWholeNumber CLASS EXTERNAL "LIBRARY orxmethod TestObjectToWholeNumber"
::method TestObjectToStringSize CLASS EXTERNAL "LIBRARY orxmethod TestObjectToStringSize"
::method TestObjectToInt64 CLASS EXTERNAL "LIBRARY orxmethod TestObjectToInt64"
::method TestObjectToUnsignedInt64 CLASS EXTERNAL "LIBRARY orxmethod TestObjectToUnsignedInt64"
::method TestObjectToInt32 CLASS EXTERNAL "LIBRARY orxmethod TestObjectToInt32"
::method TestObjectToUnsignedInt32 CLASS EXTERNAL "LIBRARY orxmethod TestObjectToUnsignedInt32"
::method TestObjectToIntptr CLASS EXTERNAL "LIBRARY orxmethod TestObjectToIntptr"
::method TestObjectToUintptr CLASS EXTERNAL "LIBRARY orxmethod TestObjectToUintptr"
::method TestObjectToLogical CLASS EXTERNAL "LIBRARY orxmethod TestObjectToLogical"
::method TestObjectToDouble CLASS EXTERNAL "LIBRARY orxmethod TestObjectToDouble"

::method TestWholeNumberToObject CLASS EXTERNAL   "LIBRARY orxmethod TestWholeNumberToObject"
::method TestStringSizeToObject CLASS EXTERNAL    "LIBRARY orxmethod TestStringSizeToObject"
::method TestInt64ToObject CLASS EXTERNAL         "LIBRARY orxmethod TestInt64ToObject"
::method TestUnsignedInt64ToObject CLASS EXTERNAL "LIBRARY orxmethod TestUnsignedInt64ToObject"
::method TestInt32ToObject CLASS EXTERNAL         "LIBRARY orxmethod TestInt32ToObject"
::method TestUnsignedInt32ToObject CLASS EXTERNAL "LIBRARY orxmethod TestUnsignedInt32ToObject"
::method TestIntptrToObject CLASS EXTERNAL        "LIBRARY orxmethod TestIntptrToObject"
::method TestUintptrToObject CLASS EXTERNAL       "LIBRARY orxmethod TestUintptrToObject"
::method TestLogicalToObject CLASS EXTERNAL       "LIBRARY orxmethod TestLogicalToObject"
::method TestDoubleToObject CLASS EXTERNAL        "LIBRARY orxmethod TestDoubleToObject"
::method TestObjectToValue  CLASS EXTERNAL        "LIBRARY orxmethod TestObjectToValue"
::method TestStringGet  CLASS EXTERNAL            "LIBRARY orxmethod TestStringGet"
::method TestStringLength  CLASS EXTERNAL         "LIBRARY orxmethod TestStringLength"
::method TestStringUpper  CLASS EXTERNAL          "LIBRARY orxmethod TestStringUpper"
::method TestStringLower  CLASS EXTERNAL          "LIBRARY orxmethod TestStringLower"
::method TestIsString  CLASS EXTERNAL             "LIBRARY orxmethod TestIsString"

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


