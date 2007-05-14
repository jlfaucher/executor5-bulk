/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Description: Methods and routines for supplying basic Is* methods.         */
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
/*         Updated by W. David Ashley                                         */
/*                                                                            */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* This file consists of four public methods that can be attached to other    */
/* classes. These methods return information about the type of class, its     */
/* super class or other info.                                                 */
/*                                                                            */
/* IsClassObject - returns .true if the class is a class object or if it is   */
/*    decended from one.                                                      */
/*                                                                            */
/* IsDecendedFrom - returns .true if the class is decended from the           */
/*    specified super class.                                                  */
/*                                                                            */
/* IsA - returns .true if the class is a child of the specified super class.  */
/*                                                                            */
/* IsInstanceOf - returns .true if the class is of the specified class.       */
/*                                                                            */
/* IsA2 - returns .true if the class is of the specified class or is decended */
/*     from the specified class.                                              */
/*                                                                            */
/*----------------------------------------------------------------------------*/


::method IsClassObject
return IsClassObject(self)


::method IsDescendedFrom
use arg SuperClass
return IsDescendedFrom(self, SuperClass)


::method IsA
use arg SuperClass
return IsA(self, SuperClass)


::method IsInstanceOf
use arg Class
return IsInstanceOf(self, Class)


::method IsA2
use arg SuperClass
return IsA2(self, SuperClass)


/* return the method-directory  */
::routine get_methods_FROM_is_util public
return .methods


/* determine whether an object is a class object */
::routine IsClassObject public
use arg instance
return DetIsClassObject(instance~class)

/* determine class-object property */
DetIsClassObject: procedure
use arg class
/* recursively check    */
if class = .object then return .false
if class = class ~ metaclass then return .true
return DetIsClassObject(class~SUPERCLASSES[1])


/* test whether first argument is either of the same class or a subclass      */
/* of the second argument's class                                             */
::routine IsDescendedFrom public
use arg ClassO1, ClassO2
if \IsClassObject(ClassO1) then do   /* 1st argument not a class object !  */
   signal on syntax
   raise syntax 93.914 array ("# 1", "the class objects", ClassO1 ~ string)
   end
if \IsClassObject(ClassO2) then do   /* 2nd argument not a class object !  */
   signal on syntax
   raise syntax 93.914 array ("# 2", "the class objects", ClassO2 ~ string)
   end
return DetIsDescendedFrom(ClassO1, ClassO2)

syntax:
   raise propagate              /* show caller's error position */

DetIsDescendedFrom: procedure
use arg ClassO1, ClassO2
if ClassO1 = ClassO2 then return .true
/* get superclasses     */
SCArray = ClassO1~SUPERCLASSES
do tmpClass over SCArray             /* test immediate superclasses  */
   if tmpClass = ClassO2 then return .true
   end
/* not found, maybe one of the superClasses preceding the immediate ones ? */
do tmpClass over SCArray             /* test immediate superclasses  */
   if DetIsDescendedFrom(tmpClass, ClassO2) then return .true
   end
return .false



/* test whether first argument is either of the same class or a subclass      */
/* of the second argument's class                                             */
::routine IsA public
use arg Object1, ClassObject2
if \IsClassObject(ClassObject2) then do
   signal on syntax
   raise syntax 93.914 array("# 2", "[the class objects]", ClassObject2~string)
   end
return DetIsA(Object1~class, ClassObject2)

syntax:
   raise propagate              /* show caller's error position */

DetIsA: procedure
use arg ClassObject1, SuperClass
if ClassObject1 = SuperClass then return .true
/* get superclasses      */
SCArray = ClassObject1 ~ SUPERCLASSES
do tmpClass over SCArray             /* test immediate superclasses  */
   if tmpClass = SuperClass then return .true
   end
/* not found, maybe one of the superclasses preceding the immediate ones ? */
do tmpClass over SCArray             /* test immediate superclasses  */
   if DetIsA(tmpClass, SuperClass) then return .true
   end
   return .false


/* test whether first argument is an instance of second argument which is     */
/* a class object                                                             */
::routine IsInstanceOf public
use arg Object, Class
return (Object~class = Class)


/* test whether first argument is either of the same class or a subclass      */
/* of the second argument                                                     */
::routine IsA2 public
use arg ClassObject1, ClassObject2
if \IsClassObject(ClassObject1) then ClassObject1 = ClassObject1~class
if \IsClassObject(ClassObject2) then ClassObject2 = ClassObject2~class
return DetIsA2(ClassObject1, ClassObject2)


DetIsA2: procedure
use arg ClassObject1, SuperClass
if ClassObject1 = SuperClass then return .true
/* get superclasses      */
SCArray = ClassObject1 ~ SUPERCLASSES
do tmpClass over SCArray             /* test immediate superclasses  */
   if tmpClass = SuperClass then return .true
   end
/* not found, maybe one of the superclasses preceding the immediate ones ? */
do tmpClass over SCArray             /* test immediate superclasses  */
   if DetIsA2(tmpClass, SuperClass) then return .true
   end
   return .false

