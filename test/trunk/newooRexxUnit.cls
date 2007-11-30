/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2005-2008 Rexx Language Association. All rights reserved.    */
/*                                                                            */
/* This program and the accompanying materials are made available under       */
/* the terms of the Common Public License v1.0 which accompanies this         */
/* distribution. A copy is also available at the following address:           */
/* http://www.opensource.org/licenses/cpl1.0.php                              */
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


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: ooRexxUnit                                                          */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class 'ooRexxUnit' public


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: ooRexxUnit                                                          */
/*        Class methods                                                      */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::attribute successes  class        -- the number of successful tests
::attribute failures   class        -- the number of failed tests
::attribute warnings   class        -- the number of warnings issued
::attribute assertions class        -- the number of assertions

::method init class
expose successes failures warnings assertions
-- initialize the statistics counters
successes = 0
failures = 0
warnings = 0
assertions = 0
return


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: ooRexxUnit                                                          */
/*        Private methods                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


::attribute assertions set private

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: ooRexxUnit                                                          */
/*        Public methods                                                      */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Method: init                                                               */
/* Description: instance initialization                                       */
/*----------------------------------------------------------------------------*/

::method init
return


/*----------------------------------------------------------------------------*/
/* Method: assertEquals                                                       */
/* Description: determine if two expressions are equal                        */
/*----------------------------------------------------------------------------*/

::method assertEquals
use strict arg value1, value2, msg = ''
.ooRexxUnit~assertions += 1
comp = (value1 = value2)
if \comp then do
   -- values are not equal so see if they are collections
   comp = isCollEqual(value1, value2)
   if comp then do
      .ooRexxUnit~successes += 1
      return  -- assertion holds
      end
   tmp = "@assertFailure assertEquals: expected="formatObjectInfo(value1) || ,
    ", actual="formatObjectInfo(value2)"."||"09"x
   self~fail(tmp || msg)
   end
.ooRexxUnit~successes += 1
return   -- assertion holds


/*----------------------------------------------------------------------------*/
/* Method: assertFalse                                                        */
/* Description: determine if the expression is false                          */
/*----------------------------------------------------------------------------*/

::method assertFalse
use arg value1, msg = ''
.ooRexxUnit~assertions += 1
if value1 = .false then do
   .ooRexxUnit~successes += 1
   return   -- assertion holds
   end
tmp = "@assertFailure assertFalse: expected=[0], actual="ppp(value1)"."||"09"x
self~fail(sTmp || msg)


/*----------------------------------------------------------------------------*/
/* Method: assertNotEquals                                                    */
/* Description: determine if two expressions are not equal                    */
/*----------------------------------------------------------------------------*/

::method assertNotEquals
use strict arg value1, value2, msg = ''
.ooRexxUnit~assertions += 1
comp = (value1 = value2)
if \comp then do
   -- values are not equal so see if they are collections
   comp = isCollEqual(value1, value2)
   if \comp then do
      .ooRexxUnit~successes += 1
      return  -- assertion holds
      end
  end
tmp = "@assertFailure assertNotEquals: expected=" || ,
 formatObjectInfo(value1, "\= ")", actual=" || ,
 formatObjectInfo(value2)"."||"09"x
self~fail(tmp || msg)


/*----------------------------------------------------------------------------*/
/* Method: assertNotNull                                                      */
/* Description: determine if an expressions is not .nil                       */
/*----------------------------------------------------------------------------*/

::method assertNotNull
use arg value1, msg = ''
.ooRexxUnit~assertions += 1
if .nil <> value1 then do
   .ooRexxUnit~successes += 1
   return   -- assertion holds
   end
tmp= "@assertFailure assertNotNull: expected=[\= [.nil]], actual=[.nil]."||"09"x
self~fail(tmp || msg)


/*----------------------------------------------------------------------------*/
/* Method: assertNotSame                                                      */
/* Description: determine if two expressions are strictly not the same        */
/*----------------------------------------------------------------------------*/

::method assertNotSame
use strict arg value1, value2, msg = ''
.ooRexxUnit~assertions += 1
if (value1 == value2) = .false then do
   .ooRexxUnit~successes += 1
    return   -- assertion holds
    end
tmp = "@assertFailure assertNotSame: expected=" || ,
 formatObjectInfo(value1, "\== ")", actual="formatObjectInfo(value2)"."||"09"x
self~fail(tmp || msg)


/*----------------------------------------------------------------------------*/
/* Method: assertNull                                                         */
/* Description: determine if an expressions is .nil                           */
/*----------------------------------------------------------------------------*/

::method assertNull
use arg value1, msg = ''
.ooRexxUnit~assertions += 1
if .nil = value1 then do
   .ooRexxUnit~successes += 1
    return   -- assertion holds
    end
tmp = "@assertFailure assertNull: expected=[.nil], actual="ppp(value1)"."||"09"x
self~fail(tmp || msg)


/*----------------------------------------------------------------------------*/
/* Method: assertSame                                                         */
/* Description: determine if two expressions are strictly the same            */
/*----------------------------------------------------------------------------*/

::method assertSame
use strict arg value1, value2, msg = ''
.ooRexxUnit~assertions += 1
if (value1 == value2) = .true then do
   .ooRexxUnit~successes += 1
    return   -- assertion holds
    end
tmp = "@assertFailure assertSame: expected="formatObjectInfo(value1) || ,
 ", actual="formatObjectInfo(value2)"."||"09"x
self~fail(tmp || msg)


/*----------------------------------------------------------------------------*/
/* Method: assertTrue                                                         */
/* Description: determine if anwo expressions is true                         */
/*----------------------------------------------------------------------------*/

::method assertTrue
use arg value1, msg = ''
.ooRexxUnit~assertions += 1
if value1 = .true then do
   .ooRexxUnit~successes += 1
    return   -- assertion holds
    end
tmp = "@assertFailure assertTrue: expected=[1], actual="ppp(vlaue1)"."||"09"x
self~fail(tmp || msg)


/*----------------------------------------------------------------------------*/
/* Method: fail                                                               */
/* Description: raise an error so the message is displayed                    */
/*----------------------------------------------------------------------------*/

::method fail
use strict arg msg = ''
.ooRexxUnit~failures += 1
-- need to raise a syntax error, because USER exception needs to be propageted

-- use application definable syntax error (since ooRexx 3.1), supply description for
-- test methods to be able to find out that the ooRexxUnit framework raised it
RAISE syntax 93.964 array (msg) description ("ooRexxUnit.cls - source of syntax exception 'FAIL' method invocation in class 'ASSERT'.")








