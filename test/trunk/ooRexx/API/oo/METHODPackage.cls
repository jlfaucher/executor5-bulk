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


::class 'METHODtester' public

-- These are the real external functions we are testing
::method TestZeroIntArgs EXTERNAL "LIBRARY orxmethod TestZeroIntArgs"
::method TestOneIntArg EXTERNAL "LIBRARY orxmethod TestOneIntArg"
::method TestTwoIntArgs EXTERNAL "LIBRARY orxmethod TestTwoIntArgs"
::method TestThreeIntArgs EXTERNAL "LIBRARY orxmethod TestThreeIntArgs"
::method TestFourIntArgs EXTERNAL "LIBRARY orxmethod TestFourIntArgs"
::method TestFiveIntArgs EXTERNAL "LIBRARY orxmethod TestFiveIntArgs"
::method TestSixIntArgs EXTERNAL "LIBRARY orxmethod TestSixIntArgs"
::method TestSevenIntArgs EXTERNAL "LIBRARY orxmethod TestSevenIntArgs"
::method TestEightIntArgs EXTERNAL "LIBRARY orxmethod TestEightIntArgs"
::method TestNineIntArgs EXTERNAL "LIBRARY orxmethod TestNineIntArgs"
::method TestTenIntArgs EXTERNAL "LIBRARY orxmethod TestTenIntArgs"
::method TestIntArg EXTERNAL "LIBRARY orxmethod TestIntArg"
::method TestInt32Arg EXTERNAL "LIBRARY orxmethod TestInt32Arg"
::method TestUint32Arg EXTERNAL "LIBRARY orxmethod TestUint32Arg"
::method TestInt8Arg EXTERNAL "LIBRARY orxmethod TestInt8Arg"
::method TestUint8Arg EXTERNAL "LIBRARY orxmethod TestUint8Arg"
::method TestInt16Arg EXTERNAL "LIBRARY orxmethod TestInt16Arg"
::method TestUint16Arg EXTERNAL "LIBRARY orxmethod TestUint16Arg"
::method TestInt64Arg EXTERNAL "LIBRARY orxmethod TestInt64Arg"
::method TestUint64Arg EXTERNAL "LIBRARY orxmethod TestUint64Arg"
::method TestIntPtrArg EXTERNAL "LIBRARY orxmethod TestIntPtrArg"
::method TestUintPtrArg EXTERNAL "LIBRARY orxmethod TestUintPtrArg"
::method TestWholeNumberArg EXTERNAL "LIBRARY orxmethod TestWholeNumberArg"
::method TestPositiveWholeNumberArg EXTERNAL "LIBRARY orxmethod TestPositiveWholeNumberArg"
::method TestStringSizeArg EXTERNAL "LIBRARY orxmethod TestStringSizeArg"
::method TestSizeArg EXTERNAL "LIBRARY orxmethod TestSizeArg"
::method TestSSizeArg EXTERNAL "LIBRARY orxmethod TestSSizeArg"
::method TestLogicalArg EXTERNAL "LIBRARY orxmethod TestLogicalArg"
::method TestFloatArg EXTERNAL "LIBRARY orxmethod TestFloatArg"
::method TestDoubleArg EXTERNAL "LIBRARY orxmethod TestDoubleArg"
::method TestCstringArg EXTERNAL "LIBRARY orxmethod TestCstringArg"
::method TestPointerValue EXTERNAL "LIBRARY orxmethod TestPointerValue"
::method TestPointerArg EXTERNAL "LIBRARY orxmethod TestPointerArg"
::method TestNullPointerValue EXTERNAL "LIBRARY orxmethod TestNullPointerValue"
::method TestPointerStringValue EXTERNAL "LIBRARY orxmethod TestPointerStringValue"
::method TestPointerStringArg EXTERNAL "LIBRARY orxmethod TestPointerStringArg"
::method TestNullPointerStringValue EXTERNAL "LIBRARY orxmethod TestNullPointerStringValue"
::method TestStemArg EXTERNAL "LIBRARY orxmethod TestStemArg"
::method TestObjectArg EXTERNAL "LIBRARY orxmethod TestObjectArg"
::method TestStringArg EXTERNAL "LIBRARY orxmethod TestStringArg"
::method TestArrayArg EXTERNAL "LIBRARY orxmethod TestArrayArg"
::method TestClassArg EXTERNAL "LIBRARY orxmethod TestClassArg"
::method TestOptionalIntArg EXTERNAL "LIBRARY orxmethod TestOptionalIntArg"
::method TestOptionalInt32Arg EXTERNAL "LIBRARY orxmethod TestOptionalInt32Arg"
::method TestOptionalUint32Arg EXTERNAL "LIBRARY orxmethod TestOptionalUint32Arg"
::method TestOptionalInt8Arg EXTERNAL "LIBRARY orxmethod TestOptionalInt8Arg"
::method TestOptionalUint8Arg EXTERNAL "LIBRARY orxmethod TestOptionalUint8Arg"
::method TestOptionalInt16Arg EXTERNAL "LIBRARY orxmethod TestOptionalInt16Arg"
::method TestOptionalUint16Arg EXTERNAL "LIBRARY orxmethod TestOptionalUint16Arg"
::method TestOptionalInt64Arg EXTERNAL "LIBRARY orxmethod TestOptionalInt64Arg"
::method TestOptionalUint64Arg EXTERNAL "LIBRARY orxmethod TestOptionalUint64Arg"
::method TestOptionalIntPtrArg EXTERNAL "LIBRARY orxmethod TestOptionalIntPtrArg"
::method TestOptionalUintPtrArg EXTERNAL "LIBRARY orxmethod TestOptionalUintPtrArg"
::method TestOptionalWholeNumberArg EXTERNAL "LIBRARY orxmethod TestOptionalWholeNumberArg"
::method TestOptionalPositiveWholeNumberArg EXTERNAL "LIBRARY orxmethod TestOptionalPositiveWholeNumberArg"
::method TestOptionalStringSizeArg EXTERNAL "LIBRARY orxmethod TestOptionalStringSizeArg"
::method TestOptionalSizeArg EXTERNAL "LIBRARY orxmethod TestOptionalSizeArg"
::method TestOptionalSSizeArg EXTERNAL "LIBRARY orxmethod TestOptionalSSizeArg"
::method TestOptionalLogicalArg EXTERNAL "LIBRARY orxmethod TestOptionalLogicalArg"
::method TestOptionalFloatArg EXTERNAL "LIBRARY orxmethod TestOptionalFloatArg"
::method TestOptionalDoubleArg EXTERNAL "LIBRARY orxmethod TestOptionalDoubleArg"
::method TestOptionalCstringArg EXTERNAL "LIBRARY orxmethod TestOptionalCstringArg"
::method TestOptionalPointerArg EXTERNAL "LIBRARY orxmethod TestOptionalPointerArg"
::method TestOptionalPointerStringArg EXTERNAL "LIBRARY orxmethod TestOptionalPointerStringArg"
::method TestOptionalStemArg EXTERNAL "LIBRARY orxmethod TestOptionalStemArg"
::method TestOptionalObjectArg EXTERNAL "LIBRARY orxmethod TestOptionalObjectArg"
::method TestOptionalStringArg EXTERNAL "LIBRARY orxmethod TestOptionalStringArg"
::method TestOptionalArrayArg EXTERNAL "LIBRARY orxmethod TestOptionalArrayArg"
::method TestOptionalClassArg EXTERNAL "LIBRARY orxmethod TestOptionalClassArg"
::method TestSuperArg EXTERNAL "LIBRARY orxmethod TestSuperArg"
::method TestScopeArg EXTERNAL "LIBRARY orxmethod TestScopeArg"
::method TestOSelfArg EXTERNAL "LIBRARY orxmethod TestOSelfArg"
::method TestArglistArg EXTERNAL "LIBRARY orxmethod TestArglistArg"
::method TestNameArg EXTERNAL "LIBRARY orxmethod TestNameArg"
::method TestNameArg2 EXTERNAL "LIBRARY orxmethod TestNameArg"        -- extra definition for name testing
::method TestGetSuper EXTERNAL "LIBRARY orxmethod TestGetSuper"
::method TestGetScope EXTERNAL "LIBRARY orxmethod TestGetScope"
::method TestGetSelf EXTERNAL "LIBRARY orxmethod TestGetSelf"
::method TestGetMessageName EXTERNAL "LIBRARY orxmethod TestGetMessageName"
::method TestGetMessageName2 EXTERNAL "LIBRARY orxmethod TestGetMessageName"    -- extra definition for name testing
::method TestGetArguments EXTERNAL "LIBRARY orxmethod TestGetArguments"
::method TestGetArgument EXTERNAL "LIBRARY orxmethod TestGetArgument"
::method TestGetMethod EXTERNAL "LIBRARY orxmethod TestGetMethod"
::method TestForwardMessage EXTERNAL "LIBRARY orxmethod TestForwardMessage"
::method TestGetObjectVariable EXTERNAL "LIBRARY orxmethod TestGetObjectVariable"
::method TestSetObjectVariable EXTERNAL "LIBRARY orxmethod TestSetObjectVariable"
::method TestDropObjectVariable EXTERNAL "LIBRARY orxmethod TestDropObjectVariable"
::method TestFindContextClass EXTERNAL "LIBRARY orxmethod TestFindContextClass"
::method TestNewStem EXTERNAL "LIBRARY orxmethod TestNewStem"
::method TestSetStemElement EXTERNAL "LIBRARY orxmethod TestSetStemElement"
::method TestGetStemElement EXTERNAL "LIBRARY orxmethod TestGetStemElement"
::method TestDropStemElement EXTERNAL "LIBRARY orxmethod TestDropStemElement"
::method TestSetStemArrayElement EXTERNAL "LIBRARY orxmethod TestSetStemArrayElement"
::method TestGetStemArrayElement EXTERNAL "LIBRARY orxmethod TestGetStemArrayElement"
::method TestDropStemArrayElement EXTERNAL "LIBRARY orxmethod TestDropStemArrayElement"
::method TestGetAllStemElements EXTERNAL "LIBRARY orxmethod TestGetAllStemElements"
::method TestGetStemValue EXTERNAL "LIBRARY orxmethod TestGetStemValue"
::method TestIsStem EXTERNAL "LIBRARY orxmethod TestIsStem"
::method TestSupplierItem EXTERNAL "LIBRARY orxmethod TestSupplierItem"
::method TestSupplierIndex EXTERNAL "LIBRARY orxmethod TestSupplierIndex"
::method TestSupplierAvailable EXTERNAL "LIBRARY orxmethod TestSupplierAvailable"
::method TestSupplierNext EXTERNAL "LIBRARY orxmethod TestSupplierNext"
::method TestNewSupplier EXTERNAL "LIBRARY orxmethod TestNewSupplier"
::method TestArrayAt EXTERNAL "LIBRARY orxmethod TestArrayAt"
::method TestArrayPut EXTERNAL "LIBRARY orxmethod TestArrayPut"
::method TestArrayAppend EXTERNAL "LIBRARY orxmethod TestArrayAppend"
::method TestArrayAppendString EXTERNAL "LIBRARY orxmethod TestArrayAppendString"
::method TestArraySize EXTERNAL "LIBRARY orxmethod TestArraySize"
::method TestArrayItems EXTERNAL "LIBRARY orxmethod TestArrayItems"
::method TestArrayDimension EXTERNAL "LIBRARY orxmethod TestArrayDimension"
::method TestNewArray EXTERNAL "LIBRARY orxmethod TestNewArray"
::method TestArrayOfOne EXTERNAL "LIBRARY orxmethod TestArrayOfOne"
::method TestArrayOfTwo EXTERNAL "LIBRARY orxmethod TestArrayOfTwo"
::method TestArrayOfThree EXTERNAL "LIBRARY orxmethod TestArrayOfThree"
::method TestArrayOfFour EXTERNAL "LIBRARY orxmethod TestArrayOfFour"
::method TestArrayOfOneAlt EXTERNAL "LIBRARY orxmethod TestArrayOfOneAlt"
::method TestArrayOfTwoAlt EXTERNAL "LIBRARY orxmethod TestArrayOfTwoAlt"
::method TestArrayOfThreeAlt EXTERNAL "LIBRARY orxmethod TestArrayOfThreeAlt"
::method TestArrayOfFourAlt EXTERNAL "LIBRARY orxmethod TestArrayOfFourAlt"
::method TestIsArray EXTERNAL "LIBRARY orxmethod TestIsArray"
::method TestDirectoryPut EXTERNAL "LIBRARY orxmethod TestDirectoryPut"
::method TestDirectoryAt EXTERNAL "LIBRARY orxmethod TestDirectoryAt"
::method TestDirectoryRemove EXTERNAL "LIBRARY orxmethod TestDirectoryRemove"
::method TestNewDirectory EXTERNAL "LIBRARY orxmethod TestNewDirectory"
::method TestIsDirectory EXTERNAL "LIBRARY orxmethod TestIsDirectory"
::method TestStringTablePut EXTERNAL "LIBRARY orxmethod TestStringTablePut"
::method TestStringTableAt EXTERNAL "LIBRARY orxmethod TestStringTableAt"
::method TestStringTableRemove EXTERNAL "LIBRARY orxmethod TestStringTableRemove"
::method TestNewStringTable EXTERNAL "LIBRARY orxmethod TestNewStringTable"
::method TestIsStringTable EXTERNAL "LIBRARY orxmethod TestIsStringTable"
::method TestSendMessage EXTERNAL "LIBRARY orxmethod TestSendMessage"
::method TestSendMessage0 EXTERNAL "LIBRARY orxmethod TestSendMessage0"
::method TestSendMessage1 EXTERNAL "LIBRARY orxmethod TestSendMessage1"
::method TestSendMessage2 EXTERNAL "LIBRARY orxmethod TestSendMessage2"
::method TestIsInstanceOf EXTERNAL "LIBRARY orxmethod TestIsInstanceOf"
::method TestHasMethod EXTERNAL "LIBRARY orxmethod TestHasMethod"
::method TestLoadPackage EXTERNAL "LIBRARY orxmethod TestLoadPackage"
::method TestLoadPackageFromData EXTERNAL "LIBRARY orxmethod TestLoadPackageFromData"
::method TestFindClass EXTERNAL "LIBRARY orxmethod TestFindClass"
::method TestFindPackageClass EXTERNAL "LIBRARY orxmethod TestFindPackageClass"
::method TestGetPackageRoutines EXTERNAL "LIBRARY orxmethod TestGetPackageRoutines"
::method TestGetPackagePublicRoutines EXTERNAL "LIBRARY orxmethod TestGetPackagePublicRoutines"
::method TestGetPackageClasses EXTERNAL "LIBRARY orxmethod TestGetPackageClasses"
::method TestGetPackagePublicClasses EXTERNAL "LIBRARY orxmethod TestGetPackagePublicClasses"
::method TestGetPackageMethods EXTERNAL "LIBRARY orxmethod TestGetPackageMethods"
::method TestCallRoutine EXTERNAL "LIBRARY orxmethod TestCallRoutine"
::method TestCallProgram EXTERNAL "LIBRARY orxmethod TestCallProgram"
::method TestNewRoutine EXTERNAL "LIBRARY orxmethod TestNewRoutine"
::method TestNewMethod EXTERNAL "LIBRARY orxmethod TestNewMethod"
::method TestIsMethod EXTERNAL "LIBRARY orxmethod TestIsMethod"
::method TestIsRoutine EXTERNAL "LIBRARY orxmethod TestIsRoutine"
::method TestGetMethodPackage EXTERNAL "LIBRARY orxmethod TestGetMethodPackage"
::method TestGetRoutinePackage EXTERNAL "LIBRARY orxmethod TestGetRoutinePackage"
::method TestObjectToString EXTERNAL "LIBRARY orxmethod TestObjectToString"
::method TestStringGet EXTERNAL "LIBRARY orxmethod TestStringGet"
::method TestStringData EXTERNAL "LIBRARY orxmethod TestStringData"
::method TestStringLength EXTERNAL "LIBRARY orxmethod TestStringLength"
::method TestStringUpper EXTERNAL "LIBRARY orxmethod TestStringUpper"
::method TestStringLower EXTERNAL "LIBRARY orxmethod TestStringLower"
::method TestIsString EXTERNAL "LIBRARY orxmethod TestIsString"
::method TestRaiseException0 EXTERNAL "LIBRARY orxmethod TestRaiseException0"
::method TestRaiseException1 EXTERNAL "LIBRARY orxmethod TestRaiseException1"
::method TestRaiseException2 EXTERNAL "LIBRARY orxmethod TestRaiseException2"
::method TestRaiseException EXTERNAL "LIBRARY orxmethod TestRaiseException"
::method TestRaiseCondition EXTERNAL "LIBRARY orxmethod TestRaiseCondition"
::method TestInterpreterVersion EXTERNAL "LIBRARY orxmethod TestInterpreterVersion"
::method TestLanguageLevel EXTERNAL "LIBRARY orxmethod TestLanguageLevel"
::method TestNewStringFromAsciiz EXTERNAL "LIBRARY orxmethod TestNewStringFromAsciiz"
::method TestNewStringFromAsciizAlt EXTERNAL "LIBRARY orxmethod TestNewStringFromAsciizAlt"
::method TestNewString EXTERNAL "LIBRARY orxmethod TestNewString"
::method TestNewStringAlt EXTERNAL "LIBRARY orxmethod TestNewStringAlt"
::method TestObjectToWholeNumber EXTERNAL "LIBRARY orxmethod TestObjectToWholeNumber"
::method TestObjectToWholeNumberAlt EXTERNAL "LIBRARY orxmethod TestObjectToWholeNumberAlt"
::method TestWholeNumberToObject EXTERNAL "LIBRARY orxmethod TestWholeNumberToObject"
::method TestWholeNumberToObjectAlt EXTERNAL "LIBRARY orxmethod TestWholeNumberToObjectAlt"
::method TestStringSizeToObject EXTERNAL "LIBRARY orxmethod TestStringSizeToObject"
::method TestStringSizeToObjectAlt EXTERNAL "LIBRARY orxmethod TestStringSizeToObjectAlt"
::method TestObjectToStringSize EXTERNAL "LIBRARY orxmethod TestObjectToStringSize"
::method TestObjectToStringSizeAlt EXTERNAL "LIBRARY orxmethod TestObjectToStringSizeAlt"
::method TestObjectToInt64 EXTERNAL "LIBRARY orxmethod TestObjectToInt64"
::method TestObjectToInt64Alt EXTERNAL "LIBRARY orxmethod TestObjectToInt64Alt"
::method TestObjectToUnsignedInt64 EXTERNAL "LIBRARY orxmethod TestObjectToUnsignedInt64"
::method TestObjectToUnsignedInt64Alt EXTERNAL "LIBRARY orxmethod TestObjectToUnsignedInt64Alt"
::method TestInt64ToObject EXTERNAL "LIBRARY orxmethod TestInt64ToObject"
::method TestInt64ToObjectAlt EXTERNAL "LIBRARY orxmethod TestInt64ToObjectAlt"
::method TestUnsignedInt64ToObject EXTERNAL "LIBRARY orxmethod TestUnsignedInt64ToObject"
::method TestUnsignedInt64ToObjectAlt EXTERNAL "LIBRARY orxmethod TestUnsignedInt64ToObjectAlt"
::method TestObjectToInt32 EXTERNAL "LIBRARY orxmethod TestObjectToInt32"
::method TestObjectToInt32Alt EXTERNAL "LIBRARY orxmethod TestObjectToInt32Alt"
::method TestObjectToUnsignedInt32 EXTERNAL "LIBRARY orxmethod TestObjectToUnsignedInt32"
::method TestObjectToUnsignedInt32Alt EXTERNAL "LIBRARY orxmethod TestObjectToUnsignedInt32Alt"
::method TestInt32ToObject EXTERNAL "LIBRARY orxmethod TestInt32ToObject"
::method TestInt32ToObjectAlt EXTERNAL "LIBRARY orxmethod TestInt32ToObjectAlt"
::method TestUnsignedInt32ToObject EXTERNAL "LIBRARY orxmethod TestUnsignedInt32ToObject"
::method TestUnsignedInt32ToObjectAlt EXTERNAL "LIBRARY orxmethod TestUnsignedInt32ToObjectAlt"
::method TestObjectToIntptr EXTERNAL "LIBRARY orxmethod TestObjectToIntptr"
::method TestObjectToIntptrAlt EXTERNAL "LIBRARY orxmethod TestObjectToIntptrAlt"
::method TestObjectToUintptr EXTERNAL "LIBRARY orxmethod TestObjectToUintptr"
::method TestObjectToUintptrAlt EXTERNAL "LIBRARY orxmethod TestObjectToUintptrAlt"
::method TestIntptrToObject EXTERNAL "LIBRARY orxmethod TestIntptrToObject"
::method TestIntptrToObjectAlt EXTERNAL "LIBRARY orxmethod TestIntptrToObjectAlt"
::method TestUintptrToObject EXTERNAL "LIBRARY orxmethod TestUintptrToObject"
::method TestUintptrToObjectAlt EXTERNAL "LIBRARY orxmethod TestUintptrToObjectAlt"
::method TestObjectToLogical EXTERNAL "LIBRARY orxmethod TestObjectToLogical"
::method TestObjectToLogicalAlt EXTERNAL "LIBRARY orxmethod TestObjectToLogicalAlt"
::method TestLogicalToObject EXTERNAL "LIBRARY orxmethod TestLogicalToObject"
::method TestLogicalToObjectAlt EXTERNAL "LIBRARY orxmethod TestLogicalToObjectAlt"
::method TestObjectToDouble EXTERNAL "LIBRARY orxmethod TestObjectToDouble"
::method TestObjectToDoubleAlt EXTERNAL "LIBRARY orxmethod TestObjectToDoubleAlt"
::method TestDoubleToObject EXTERNAL "LIBRARY orxmethod TestDoubleToObject"
::method TestDoubleToObjectAlt EXTERNAL "LIBRARY orxmethod TestDoubleToObjectAlt"
::method TestCStringToObject EXTERNAL "LIBRARY orxmethod TestCStringToObject"
::method TestObjectToCString EXTERNAL "LIBRARY orxmethod TestObjectToCString"
::method TestNewMutableBuffer EXTERNAL "LIBRARY orxmethod TestNewMutableBuffer"
::method TestIsMutableBuffer EXTERNAL "LIBRARY orxmethod TestIsMutableBuffer"
::method TestMutableBufferLength EXTERNAL "LIBRARY orxmethod TestMutableBufferLength"
::method TestSetMutableBufferLength EXTERNAL "LIBRARY orxmethod TestSetMutableBufferLength"
::method TestMutableBufferCapacity EXTERNAL "LIBRARY orxmethod TestMutableBufferCapacity"
::method TestSetMutableBufferCapacity EXTERNAL "LIBRARY orxmethod TestSetMutableBufferCapacity"
::method TestSetMutableBufferValue EXTERNAL "LIBRARY orxmethod TestSetMutableBufferValue"
::method TestGetMutableBufferValue EXTERNAL "LIBRARY orxmethod TestGetMutableBufferValue"

::method TestAllocateObjectMemory   EXTERNAL "LIBRARY orxmethod"
::method TestFreeObjectMemory       EXTERNAL "LIBRARY orxmethod"
::method TestReallocateObjectMemory EXTERNAL "LIBRARY orxmethod"
::method TestSetObjectMemory        EXTERNAL "LIBRARY orxmethod"
::method TestGetObjectMemory        EXTERNAL "LIBRARY orxmethod"

::method TestRaiseExceptionX
  signal on syntax
  forward message(TestRaiseException) continue
  return .nil

  syntax:
     return condition('o')

::method TestRaiseException0X
  signal on syntax
  forward message(TestRaiseException0) continue
  return .nil

  syntax:
     return condition('o')

::method TestRaiseException1X
  signal on syntax
  forward message(TestRaiseException1) continue
  return .nil

  syntax:
     return condition('o')

::method TestRaiseException2X
  signal on syntax
  forward message(TestRaiseException2) continue
  return .nil

  syntax:
     return condition('o')

::class VariableTester PUBLIC
::method init
  expose var1 stem1. stem2.1
  var1 = "Hello"
  stem1.1 = "World"
  stem2.1 = "Again"

::method TestGet01
  use arg testgroup
  testGroup~assertSame("Hello", self~TestGetObjectVariable('VAR1'))
  testGroup~assertSame("Hello", self~TestGetObjectVariable('var1'))

  a = self~TestGetObjectVariable('stem1.')
  testGroup~assertTrue(a~isA(.Stem))
  testGroup~assertSame("World", a[1])

  a = self~TestGetObjectVariable('STEM1.')
  testGroup~assertTrue(a~isA(.Stem))
  testGroup~assertSame("World", a[1])

  a = self~TestGetObjectVariable('STEM2.')
  testGroup~assertTrue(a~isA(.Stem))
  testGroup~assertSame("Again", a[1])

::method TestGet02
  self~TestGetObjectVariable("FOOBAR");

::method TestGet03
  self~TestGetObjectVariable("STEM1.1");

::method TestSet01
  expose setVar1 setStem.
  use arg testgroup

  drop setVar1
  drop setStem.


  self~TestSetObjectVariable('setVar1', "abc")
  testGroup~assertSame("abc", setVar1)

  self~TestSetObjectVariable('setVar1', "def")
  testGroup~assertSame("def", setVar1)

  self~TestSetObjectVariable('setStem.', .stem~new("FOOBAR."))
  testGroup~assertSame("FOOBAR.", setStem.[])
  testGroup~assertSame("FOOBAR.1", setStem.1)

  self~TestSetObjectVariable('setStem.', "Fred")
  testGroup~assertSame("Fred", setStem.[])
  testGroup~assertSame("Fred", setStem.1)

  -- The following does not set...for object variables, only
  -- simple vars and stems are supported
  setStem.1 = "Mike"
  self~TestSetObjectVariable("STEM1.1", "George");

  testGroup~assertSame("Mike", setStem.1)

::method TestDrop01
  expose setVar1 setStem.
  use arg testgroup

  setVar1 = "Mike"
  self~TestDropObjectVariable("setVar1")
  testGroup~assertFalse(var("SETVAR1"), "Simple var 1")

  setVar1 = "Mike"
  self~TestDropObjectVariable("SETVAR1")
  testGroup~assertFalse(var("SETVAR1"), "Simple var 2")

  setStem.1 = "Mike"
  self~TestDropObjectVariable("setStem.")
  testGroup~assertSame("SETSTEM.1", setStem.1, "Stem var 1")
  setStem.1 = "Mike"
  self~TestDropObjectVariable("SETSTEM.")
  testGroup~assertSame("SETSTEM.1", setStem.1, "Stem var 1")

::method TestGetObjectVariable EXTERNAL "LIBRARY orxmethod TestGetObjectVariable"
::method TestSetObjectVariable EXTERNAL "LIBRARY orxmethod TestSetObjectVariable"
::method TestDropObjectVariable EXTERNAL "LIBRARY orxmethod TestDropObjectVariable"

::class BufferTester PUBLIC
::method Init EXTERNAL "LIBRARY orxmethod TestBufferInit"
::method GetBuffer  EXTERNAL "LIBRARY orxmethod TestGetBuffer"
::method CSELF      EXTERNAL "LIBRARY orxmethod TestBufferCSelf"
::method OBJECTTOCSELF  EXTERNAL "LIBRARY orxmethod TestBufferObjectToCSelf"
::method IsBuffer EXTERNAL "LIBRARY orxmethod TestIsBuffer"

