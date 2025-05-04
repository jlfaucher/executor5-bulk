/**
 *  genTimeOptions.rex  This is a throw away program that was used to generate
 *  test case methods for the TIME BIF.  It tested every possible character
 *  as the first option to TIME().  The program generated a text file that was
 *  then pasted into the TIME.testGroup file.
 *
 *  It is here more as an example for other test writers of possible solutions
 *  than that it has any lasting value.
 */
 validopts='CEFHLMNRST'

 fsObj = .stream~new('hexMethods.txt')
 fsObj~open("WRITE REPLACE")
 do i = 0 to 255
   if verify(i~d2c~translate, validopts) = 0 then do
     if i > 96 then line = "::method test_validOpt_littleChar_" || i~d2c
     else line = "::method test_validOpt_bigChar_" || i~d2c
     fsObj~lineout(line)
     fsObj~lineout("   a = d2c(" || i || ")")
     fsObj~lineout("   y = time(a); z = time(a)")
     fsObj~lineout("   self~assertEquals(y, z)")
   end
   else do
     fsObj~lineout("::method test_invalidOpt_hex_" || i~d2x)
     fsObj~lineout("   a = d2c(" || i || ")")
     fsObj~lineout("   self~expectSyntax(40.904)")
     fsObj~lineout("   z = time(a)")
   end
   fsObj~lineout("")
 end

 fsObj~close

/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2007-2008 Rexx Language Association. All rights reserved.    */
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
