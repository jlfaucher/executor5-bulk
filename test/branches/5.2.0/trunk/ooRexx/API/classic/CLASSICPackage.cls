/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2008-2024 Rexx Language Association. All rights reserved.    */
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


::class Shv subclass shvblock public

/*
#define RXSHV_SET          0x00       /* Set var from given value    */
#define RXSHV_FETCH        0x01       /* Copy value of var to buffer */
#define RXSHV_DROPV        0x02       /* Drop variable               */
#define RXSHV_SYSET        0x03       /* Symbolic name Set variable  */
#define RXSHV_SYFET        0x04       /* Symbolic name Fetch variable*/
#define RXSHV_SYDRO        0x05       /* Symbolic name Drop variable */
#define RXSHV_NEXTV        0x06       /* Fetch "next" variable       */
#define RXSHV_PRIV         0x07       /* Fetch private information   */
*/
::constant code (.StringTable~of(-
  ('S', 0), ('F', 1), ('D', 2), -
  ('s', 3), ('f', 4), ('d', 5), -
  ('N', 6), ('P', 7)))

::constant OK    ('00'~x2d) -- RXSHV_OK    0x00 /* Execution was OK            */
::constant NEWV  ('01'~x2d) -- RXSHV_NEWV  0x01 /* Variable did not exist      */
::constant LVAR  ('02'~x2d) -- RXSHV_LVAR  0x02 /* Last var trans via SHVNEXTV */
::constant TRUNC ('04'~x2d) -- RXSHV_TRUNC 0x04 /* Truncation occurred-Fetch   */
::constant BADN  ('08'~x2d) -- RXSHV_BADN  0x08 /* Invalid variable name       */
::constant MEMFL ('10'~x2d) -- RXSHV_MEMFL 0x10 /* Out of memory failure       */
::constant BADF  ('80'~x2d) -- RXSHV_BADF  0x80 /* Invalid funct code (shvcode)*/

::method init
  -- we must have at least a code and a name
  use strict arg code, name, value = "", valuelen = 0
  c = self~code[code]
  self~shvcode = (c == .nil)~?(42, c) -- 42 is illegal
  self~shvname = name;
  self~shvvalue = value;
  self~shvvaluelen = valuelen;
  self~shvnamelen = 0;

::method makeArray
  return .Array~of(self)


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
