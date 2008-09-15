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

