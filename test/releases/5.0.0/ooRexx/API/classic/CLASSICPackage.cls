/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2008-2021 Rexx Language Association. All rights reserved.    */
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


::class TestQueueFuncs public

::attribute retc get
::attribute flag get -- this attribute contains method specific data
::attribute timestamp get -- RxxPullFromQueue timestamp info

::method TestCreateQueue PUBLIC EXTERNAL "LIBRARY orxclassic TestCreateQueue"
::method TestOpenQueue PUBLIC EXTERNAL "LIBRARY orxclassic TestOpenQueue"
::method TestQueueExists PUBLIC EXTERNAL "LIBRARY orxclassic TestQueueExists"
::method TestDeleteQueue PUBLIC EXTERNAL "LIBRARY orxclassic TestDeleteQueue"
::method TestQueryQueue PUBLIC EXTERNAL "LIBRARY orxclassic TestQueryQueue"
::method TestAddQueue PUBLIC EXTERNAL "LIBRARY orxclassic TestAddQueue"
::method TestPullFromQueue PUBLIC EXTERNAL "LIBRARY orxclassic TestPullFromQueue"
::method TestClearQueue PUBLIC EXTERNAL "LIBRARY orxclassic TestClearQueue"


::class TestMemoryFuncs public

::method TestAllocateFreeMemory PUBLIC EXTERNAL "LIBRARY orxclassic TestAllocateFreeMemory"


::class TestVariablePool public

::method TestVariablePool PUBLIC EXTERNAL "LIBRARY orxclassic TestMVariablePool"


::class ShvBlock public

::attribute shvname
::attribute shvvalue
::attribute shvnamelen
::attribute shvvaluelen
::attribute shvcode
::attribute shvret


::class TestMacroSpaceFuncs public

::method TestAddMacro PUBLIC EXTERNAL "LIBRARY orxclassic TestAddMacro"
::method TestDropMacro PUBLIC EXTERNAL "LIBRARY orxclassic TestDropMacro"
::method TestSaveMacroSpace PUBLIC EXTERNAL "LIBRARY orxclassic TestSaveMacroSpace"
::method TestLoadMacroSpace PUBLIC EXTERNAL "LIBRARY orxclassic TestLoadMacroSpace"
::method TestQueryMacro PUBLIC EXTERNAL "LIBRARY orxclassic TestQueryMacro"
::method TestReorderMacro PUBLIC EXTERNAL "LIBRARY orxclassic TestReorderMacro"
::method TestClearMacroSpace PUBLIC EXTERNAL "LIBRARY orxclassic TestClearMacroSpace"

::class TestFunctionRegistration public
::method TestRegisterFunctionExe PUBLIC EXTERNAL "LIBRARY orxclassic TestRegisterFunctionExe"
::method TestDeregisterFunction PUBLIC EXTERNAL "LIBRARY orxclassic TestDeregisterFunction"
::method TestQueryFunction PUBLIC EXTERNAL "LIBRARY orxclassic TestQueryFunction"
