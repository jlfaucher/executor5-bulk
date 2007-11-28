/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2005-2008 Rexx Language Association. All rights reserved.    */
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
/* Author: Moritz Hoffmann <spambasket@antiguru.de>                           */
/*----------------------------------------------------------------------------*/
/*
This is an Enumeration class. It offers all methods enumeration elements need
to work correctly. Defines enumerations do not need to implement any code
for managing the enumeration.
Enumerations add methods for ordering and comparing to classes and objects.
These are helpful to have less strict equality operators and to iterate over
an enumeration as well as ordering the items.

Written by Moritz Hoffmann 2007

Terminology
An enumeration type is a subclass of the enumeration class. It can not be
instantiated, meaning that it needs sub classes that are the enumeration
items.
An enumeration item is a value a enumeration type can acquire.

ENUMERATION TYPES
  Enumeration types are direct sub classes of the .Enumeration class. They
  act as a container for their items. The items are the characteristic for the
  type.
  Instance methods defined on the type will be common to all items.

ENUMERATION ITEMS
  Enumeration items are the values a enumeration type can aquire. Just like
  any other class they can have custom methods only valid for them.
  Items are allowed to have their own constructor. Especially when using
  items with attributes it may be handy to accept parameters as initial
  values.
  Still there are items created internally, thus the constructor using no
  argmuments must exist.

*/

/*
The Enumeration class. This class defines both class and instance methods for
working with enumerations as well as registering them at application
initialization.
*/
::CLASS Enumeration PUBLIC

/*
The values this enumeration can have.
@type Directory
*/
::ATTRIBUTE Values CLASS

/*
The enumeration type this item belongs to.
@type .Enumeration sub class
*/
::ATTRIBUTE EnumerationClass CLASS

/*
The order of the enumeration items
@type Array
*/
::ATTRIBUTE Order CLASS

/*
This class has been initilized already, don't try to repreat the custom
initialization. This is required for sub classed items to register correctly.
@type boolean
*/
::ATTRIBUTE IsInited CLASS PRIVATE
/*
Class initialization. Initializes the values directory and fills it with
content.
*/
::METHOD Init CLASS
  if self~IsInited = .true then self~init:super
  --TODO: replace this by better code to check if self is the Enumeration class
  if .Enumeration~class = .String then return

  -- a direct subclass, initialize the values directory
  if self~superClass = .Enumeration then do
    self~Values = .Directory~new
    self~Order = .Array~new
  end
  -- a subclass of an enumeration
  else if self~isSubclassOf(.Enumeration) then do
    -- find the enumeration type
    class = self
    do until class = .Enumeration
      enum = class
      class = class~superClass
    end
    -- ok, it is now stored in variable enum
    -- add the class to the values directory
    values = enum~values
    -- TODO: how to get the class name? (The ... class)
    Values[self~defaultname~word(2)] = self
    -- save the order
    self~orderID = values~items
    enum~order[values~items] = self
    -- save the enumeration type within the item
    self~EnumerationClass = enum
  end
  -- forward the invocation
  self~isInited = .true
  self~init:super

/*
Overwrite the new method to avoid instances of the enumeration class and its
direct subclasses.
*/
::METHOD new CLASS
  -- don't permit to create instances of the Enumeration class and its
  -- direct sub classes
  if self = .Enumeration | self~superClass = .Enumeration then
    raise syntax 97.001 array (self,"new")
  else forward class (super) 

/*
This method returns the enumeration entry with the given name.
@param name
  The name for wich the enumeration is requested.
@returns
  An enumeration item if there is one with that name or NIL.
*/
::METHOD valueOf CLASS
  arg name
  if name = "" then raise syntax 93.911 array ("name")
  if self~values~hasIndex(name) then
    return self~values[name]~new
  else
    return .nil

/*
The unknown method tries to resolve enumeration items. Requests not mateched
are forwarded.
*/
::METHOD Unknown CLASS
  use arg name, args
  arg name
  if self~values~hasIndex(name) then
    return .message~new(self~values[name],"new","A",args)~send
-- FIXME: comparison operators are never ever forwarded like this.
-- this was intended for lines like .MyEnum~Item = .nil ouside a comparison
--  else if name~right(1) = "=" & args~size = 1 then
--    return args[1]~class = self~values[name~left(name~length-1)]
  else raise syntax 97.001 array (self,name)

/*
This method does common checks on an "other" item. It is used by comparison
methods to validate the argument.
*/
::METHOD validateOther PRIVATE
  use strict arg other
  -- the order is required as it would cause the = message to be send to
  -- self otherwise. Thus .nil needs to be first here.
  if .nil = other then raise syntax 93.911 array ("other")
  -- Is the other object really an instance of an Enumeration item?
  if \ other~isInstanceOf(self~class~EnumerationClass) then
    raise syntax 93.948 array ("other",self~class~EnumerationClass)

/*
Compare two enumeration items.
@param other
  The object to compare against.
@returns
  1 if the classes are equal of if both objects are equal, 0 otherwise.
*/
::METHOD equal PRIVATE
  use strict arg other
  -- other must not be nul
  if .nil = other then return .false
  -- must be of same enumeration type
  if \ other~isInstanceOf(self~class~EnumerationClass) then
    return .false 
  -- try relaxed equality check
  if other~isInstanceOf(.Enumeration) then
    return self~class = other~class
  -- fall back to default
  else return other = self

/*
This method compares self to another item. It inverts the result of equal.
@param other
  The other item to compare against.
@returns
  0 if they are equal, 1 otherwise.
*/
::METHOD unequal PRIVATE
  use strict arg other
  -- invert result of equality operator
  return \(self~equal(other))

/*
Equality opertors.
*/
::METHOD "="
  forward message "equal"
/*
TODO
This still allows object reference comparison. Do we need to overwrite it?
::METHOD "=="
  forward message "equal"
*/

/*
Inequality operators.
*/
::METHOD "\="
  forward message "unequal"
/*
TODO see ::METHOD "=="
::METHOD "\=="
  forward message "unequal"
*/
::METHOD "<>"
  forward message "unequal"
::METHOD "><"
  forward message "unequal"

/*
Relation comparisons. Enumeration items support these as they are ordered.
*/
::METHOD ">"
  forward message "validateOther" continue
  use strict arg other
  return self~compareTo(other) > 0
::METHOD "<"
  forward message "validateOther" continue
  use strict arg other
  return self~compareTo(other) < 0
::METHOD ">="
  forward message "validateOther" continue
  use strict arg other
  return self~compareTo(other) >= 0
::METHOD "<="
  forward message "validateOther" continue
  use strict arg other
  return self~compareTo(other) <= 0

/*
This method is only important for enumerations where each item has attributes.
It has to make sure that items are not equal by their class but by their
content. That means that every enumeration type that has items with attributes
needs to implement this method.
The default implementation is intended to be replaced by custom code comparing
relevant item attributes.
@param other
  The other enumeration element to compare against
@returns
  1 if they are strictly equal (same content and same item), 0 otherwise.
*/
::METHOD strictlyEquals
  use strict arg other
  -- make sure the overloaded = operator is bypassed
  return self~"=":super(other)

/*
Compares two enumeration items and returns their order. This can be used to
sort enumeration items.
@param other
  The other item to copare against. Must have the same super class as the
  receiver object.
@returns
  The order as number.
    order = 0: receiver and other are of same class.
    order < 0: other follows receiver.
    order > 0: reciever follows other.
*/
::METHOD compareTo
  forward message "validateOther" continue
  use strict arg other
  return self~class~orderID - other~class~orderID

/*
The order of the enumeration item. Only valid on enumeration items.
@type Number String
*/
::ATTRIBUTE orderID CLASS

-- just a wrapper to shorten calling code.
::METHOD orderID
  return self~class~orderID

/*
This method gets adjacent items. It accepts an offset.
@param offset
  The offsetto use for getting another enumeration item.
@returns
  The enumeration item or NIL if it does not exist.
*/
::METHOD getAdjacent
  use strict arg adjacent
  order = self~class~EnumerationClass~order
  index = self~class~orderID + adjacent
  -- make sure that order~hasIndex does not throw a syntax error
  if index < 1 | index > 99999999 then return .nil
  if order~hasIndex(index) then
    return order[index]~new
  else
    return .nil

/*
Get the enumeration item following this item. Shortcut to getAdjacent(1).
*/
::METHOD next
  return self~getAdjacent(1)

/*
Get the enumeration item preceding this item. Shortcut to getAdjacent(-1).
*/
::METHOD previous
  return self~getAdjacent(-1)
