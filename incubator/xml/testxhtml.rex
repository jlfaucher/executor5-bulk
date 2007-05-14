/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Description: Methods which can be attached to the builtin collection       */
/*              classes to add a makexhtml method.                            */
/*                                                                            */
/* Copyright (c) 2006 Rexx Language Association. All rights reserved.         */
/*                                                                            */
/* This program and the accompanying materials are made available under       */
/* the terms of the Common Public License v1.0 which accompanies this         */
/* distribution. A copy is also available at the following address:           */
/* http://www.ibm.com/developerworks/oss/CPLv1.0.htm                          */
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
/* Author: Rony G. Flatscher                                                  */
/*                                                                            */
/*----------------------------------------------------------------------------*/


/*
Test conversions of collection objects to xhtml tables.

*/


nl="0d0a"x     -- define CR-LF
call "xhtml.cls"        -- get floating methods from "xhtml.cls"
m=xhtml.methods()       -- get directory of floating methods of "xhtml.cls"

-- /*  more than two dimensions in array would break appropriate xhtml-method
a1=.array~new
a1 ~~put("one", 1,2,3) ~~put("two", 2,1,3) ~~put("three", 3,2,1)
-- */
-- a1=.array~of("one", "two", "three")

-- b1=.bag~of("one", "two", "three", "four", "three", "two")
b1=.bag~of("one<one", "two>two", "three&three", "four", "three&three", "two>two")

d1=.directory~new ~~setentry("one", "one") ~~setentry("two", "two") ~~setentry("three", "three")

l1=.list~of("one", "two", "three")

q1=.queue~new ~~queue("one") ~~queue("two") ~~queue("three")

r1=.relation~new ~~put("one", "one" ) ~~put("two", "one" ) ~~put("two", "one" )
r1               ~~put("one", "two" ) ~~put("two", "two" ) ~~put("two"  , "two" )
r1               ~~put("one", "four") ~~put("one", "four")

s1=.set~of("one", "two", "three", "four", "three", "two")


t1=.table   ~new ~~put("one", "one" ) ~~put("two", "one" ) ~~put("two", "one" )
t1               ~~put("one", "two" ) ~~put("two", "two" ) ~~put("two"  , "two" )
t1               ~~put("one", "four") ~~put("one", "four")


coll=.list~of(a1, b1, d1, l1, q1, r1, s1, t1)

-- define classes with user-supplied index
useIndex=.set~of(.bag, .directory, .relation, .set, .table)

do el over coll
   cl=el~class          -- get class object
   id=cl~id             -- get ID (class name)

   -- create an enhanced instance of the same class
   tmpEl=el~class~enhanced(.directory~new~~setentry("makexhtml", m~entry("makexhtml")))
   -- add elements to newly created instance
   s=el~supplier        -- get a supplier object
   say "class:" id

   do while s~available

      if .array=cl then                   -- an array ?
      INTERPRET "tmpEl["s~index"]="'"'s~item'"'

      else if .list=cl then               -- a list
      tmpEl~insert(s~item)

      else if .queue=cl then              -- a list
      tmpEl~queue(s~item)

      else if useIndex~hasIndex(cl) then  -- use index part of supplier
      tmpEl~put(s~item,s~index)        -- add item to element

      else              -- only use item part of supplier
      tmpEl~put(s~item)

      s~next            -- next element
      end

   say "<!--" center(" rendering object of type" id "", 50, "*") "-->"
   say
   say tmpEl~makexhtml     -- get xhtml-rendering of
   say
   end
return


::requires "makexhtml.cls"  -- get floating method from "xhtml.cls"

