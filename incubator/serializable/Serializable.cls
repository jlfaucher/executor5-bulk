/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 1995, 2004 IBM Corporation. All rights reserved.             */
/* Copyright (c) 2005-2006 Rexx Language Association. All rights reserved.    */
/*                                                                            */
/* This program and the accompanying materials are made available under       */
/* the terms of the Common Public License v1.0 which accompanies this         */
/* distribution. A copy is also available at the following address:           */
/* http://www.oorexx.org/license.html                          */
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

/******************************************************************************
	The Serializable Class
This class is the one indicating that an object can be serialized. A class
has to inherit from Serializable if it needs serializing.
	::CLASS SomeClass MIXINCLASS Serializable

On Top if this is implements the default behaviour for the Read- and
WriteObject methods. It assumes there is an public attribute called
PersistentData that stores the relevant data. In case this method is not
implemented an error is raised.
******************************************************************************/
::CLASS Serializable PUBLIC MIXINCLASS Object

::METHOD Init CLASS
/* Register sub classes */
-- at this point .Serializable is not yet created, thus we use the string
	if self~id \= "SERIALIZABLE" then
		.SerializeSerializable~new(self)
	forward class (super)

-- Initalization for sub classes, tries to set PersistentData and raises an
-- error if it fails
::METHOD Init
	use arg data
	signal on nomethod
	self~PersistentData = data
	return self

	nomethod: raise syntax 97.001 array(self, "PersistentData")

-- Method to store object persistently. Has to return its data. Default
-- implementation tries to access PersistentData.
::METHOD WriteObject
	signal on nomethod
	return self~PersistentData

	nomethod: raise syntax 97.001 array(self, "PersistentData")

-- Method to recreate an object. Creates an instance of self and forwards the
-- data to New.
::METHOD ReadObject CLASS
	use arg Data
	signal on syntax
	Instance =  self~New(Data)
	return Instance

	syntax: raise propagate


/******************************************************************************
	The SerializeFunctions class
This class offers the base functionality to serialize and deserialize data.
It manages data handlers for managing a specific data type and offers a public
function to serialize data and deserialize it.
Assume variable a holds an object whichs class inherits from Serializable.
In this case the following command will return the serialized object:
	lines = .SerializeFunctions~Serialize(a)

Deserializing works the same way (b will be like a~copy):
	b = .SerializableFunctions~DeSerialize(lines)

The serializing and deserializing process is iterative. Thus there can be as
many objects nested as desired - as long as the memory is not full.
******************************************************************************/
::CLASS SerializeFunctions PUBLIC

::METHOD Init CLASS
/* initailize containers for handler classes */
	expose name type classtype
	-- These are containers for data handlers
	name = .directory~new
	classtype = .table~new
	forward class (super)

::METHOD SetHandler CLASS
/* Used by SerializeHandler: registers a new handler */
	expose name classtype
	use arg Handler
	say "SetHandler:" Handler
	if Handler~new~IsInstanceOf(.SerializeHandler) then
		classtype[Handler~checktype] = Handler
	name[Handler~name] = Handler

-- Serialize a given object
/*
>>-Serialize(data)------------------------------------------------------------><
*/
::METHOD Serialize CLASS
	expose name classtype
	use arg data
	signal on syntax
	-- create a new serializer instance, just for data encapsulation
	serializer = self~new(name,classtype)
	-- return the serialized data
	return serializer~ToSerializableDataCtrl(data)

	syntax: raise propagate

-- Method to deserialize stored objects
/*
                          +-,-1-,-linearray~size--+
>>-DeSerialize(linearray--+-----------------------+--)------------------------><
                          +-,-start--+------------+
			             +-,-end------+
*/
::METHOD DeSerialize CLASS
	expose name classtype
	-- called with an array of lines
	use arg linearray
	signal on syntax
	-- Is parameter start set?
	if arg() >= 2 then do
		start = arg(2)
		-- It must be an integer within the boundaries
		if  (datatype(start) \= "NUM") then
			-- Incorrect type
			raise syntax 93.905 array ("start", arg(2))
		else if linearray~size < start then
			-- Incorrect array position
			raise syntax 93.919 array (start)
	end
	else
		start = 1
	-- Is parameter last set?
	if arg() = 3 then do
		last = arg(3)
		if  (datatype(last) \= "NUM") then
			-- Incorrect type
			raise syntax 93.905 array ("last", arg(3))
		else if linearray~size < last then
			-- Incorrect array position
			raise syntax 93.919 array (last)
		else if last < start then
			-- Application arror
			raise syntax 93.964 array ("Last is smaller than start")
	end
	else last = linearray~size

	-- Instace self
	deserializer = self~new(name,classtype)
	return deserializer~FromSerializedDataCtrl(linearray,start,last)
	syntax: raise propagate

/**************************** INSTANCE METHODS ********************************/
::METHOD Init
	-- save the handlers
	expose name classtype
	use arg name, classtype

::METHOD FromSerializedDataCtrl
	expose objindex data
	use arg linearray, start, last
	objindex = .array~new
	data = linearray
	-- convert the data to objects, starting at the last serialized object
	-- avoids recursion as later queries will hit the cache
	do i = last to start by -1
		self~FromSerializedData(i)
	end
	return objindex[start]

::METHOD FromSerializedData
	expose name objindex data
	use arg pos
	-- check if there is somethig in the cache for the index
	if objindex~hasindex(pos) then
		return objindex[pos]
	-- split the current string into type and arguments
	parse value data[pos] with type arguments
	-- there must be a class associated with type
	class = name[type]
	if class = .nil then
		raise syntax 93.914 array -
("object","a class handled by a .SerialzeHandler class",type)
	else
		-- use the class found to deserialize the data
		objindex[pos] = class~DeSerialize(self,arguments,data)
	-- free some memory by resetting the array
	data~remove(pos)
	-- this does not return anything, data is stored in cache objindex
	return objindex[pos]

::METHOD ToSerializableDataCtrl
	expose ObjectList classtype objindex
	use arg Object
	-- initalize some containers
	-- store the object -> id relation
	objindex = .table~new
	-- store id -> object relation
	ObjectList = .array~new
	-- store lines of string
	rindex = .array~new
	-- the first item to be serialized is no. 1
	position = 1
	ObjectList[1] = Object
	-- work until all objects have been serialized
	do until position > Objectlist~size
		-- get the current object
		Object = ObjectList[position]
		-- check the way to serialize it
		if classtype~hasindex(Object~class) then do
			-- it can be matched by class, get the handler
			handler = classtype[Object~class]
		end
		-- use the nil handler for unknown objects, default exit
		else Handler = .SerializeNil
		-- handler must be set at this point, so serialize the data
		rindex[position] = handler~name handler~Serialize(self,Object)
		-- go to the next position
		position = position + 1
	end
	-- return the array of lines
	return rindex

-- This method is called by handlers to register objects. It returns the
-- object's id.
::METHOD ToSerializableData
	expose ObjectList ObjIndex
	use arg Object
	-- this cache check will avoid serializing the same object again
	if ObjIndex~HasIndex(Object) then do
		return ObjIndex[Object]
	end
	-- the ID will be the next ObjectList entry
	CurrPos = ObjectList~size + 1
	-- Store the ID -> object and object -> ID relation
	ObjectList[CurrPos] = Object
	ObjIndex[Object] = CurrPos
	return CurrPos

/******************************************************************************
	The SerializeHandler Class

Signature of a data handler:
- It must directly inherit from SerializeHandler
- It must define
	- Name: return the identifier used to store the data
	- CheckType: class -- The class this handler can serialize
	- Serialize(Caller,Object): string
	- DeSerialize(Caller,Arguments): object -- return an instance
- Calls to Init MUST be forwarded, otherwise a handler will not register.
******************************************************************************/

::CLASS SerializeHandler PUBLIC -- SUBCLASS Class
::METHOD Init CLASS
	-- check if the class implements the required methods
	methods = .array~of("NAME","CHECKTYPE","SERIALIZE","DESERIALIZE")
	correct = .true
	do i = 1 to methods~size while correct
		correct = correct & self~hasmethod(methods[i])
	end
	-- if yes then register the class
	if correct then
		.SerializeFunctions~SetHandler(self)
        forward class (super)

/******************************************************************************
	Here follow the handler classes
******************************************************************************/
-- Special handler for serializing .Serializable subclasses. Object is created
-- by .Serializable class method Init.
::CLASS SerializeSerializable SUBCLASS SerializeHandler
::METHOD Init CLASS
-- The following two methods are needed to simulate class behaviour
-- .SerializeFunctions~SetHandler expects a class as arguments, not an object
::METHOD new
	return self
::METHOD Superclasses
	return self~class~superclasses
::METHOD Init
	expose myclass
	use arg myclass
	.SerializeFunctions~SetHandler(self)
::METHOD CheckType
	expose myclass
	return myclass
::METHOD Name
	expose myclass
	return myclass~id
::METHOD Serialize
	expose myclass
	use arg Caller,Object
	return Caller~ToSerializableData(Object~WriteObject)
::METHOD DeSerialize
	expose myclass
	use arg Caller,arguments
	parse var arguments ptr
		return myclass~ReadObject(Caller~FromSerializedData(ptr))

::CLASS SerializeMethod SUBCLASS SerializeHandler
::METHOD Name CLASS
	return "METHOD"
::METHOD CheckType CLASS
	return .Method
::METHOD Serialize CLASS
        use arg Caller,Object
	-- How to preserve the method's name?
	return Caller~ToSerializableData(Object~source)
::METHOD DeSerialize CLASS
	use arg Caller,arguments
	return .Method~new('',Caller~FromSerializedData(arguments))

::CLASS SerializeMutableBuffer SUBCLASS SerializeHandler
::METHOD Name CLASS
        return "MUTABLEBUFFER"
::METHOD CheckType CLASS
        return .MutableBuffer
::METHOD Serialize CLASS
	use arg Caller,Object
	return Caller~ToSerializableData(Object~string)
::METHOD DeSerialize CLASS
	use arg Caller, pos
        return .MutableBuffer~new(Caller~FromSerializedData(pos))

::CLASS SerializeString SUBCLASS SerializeHandler
::METHOD Name CLASS
        return "STRING"
::METHOD CheckType CLASS
        return .string
::METHOD Serialize CLASS
	return arg(2)~encodeBase64
::METHOD DeSerialize CLASS
   return arg(2)~decodeBase64

::CLASS SerializeNil  SUBCLASS SerializeHandler
::METHOD Name CLASS
        return "NIL"
-- implement CheckType just to fit specification (objects can not inherit from .nil)
::METHOD CheckType CLASS
        return .nil
::METHOD Serialize CLASS
	return ""
::METHOD DeSerialize CLASS
        return .nil

::CLASS SerializeRelation SUBCLASS SerializeHandler
::METHOD Name CLASS
        return "RELATION"
::METHOD CheckType CLASS
        return .Relation
::METHOD Serialize CLASS
        use arg Caller,Object
	mb = .MutableBuffer~new
	s = Object~supplier
	relation_index = .Table~new
	do while s~available
		relation_index[s~index] = .true
		s~next
	end
	do index over relation_index
		if relation_index[index] = .true then do
			relation_index[index] =  -
				Caller~ToSerializableData(index)
			mb~append(" "||relation_index[index]||":")
			i = 0
			do item over Object~allat(index)
				if i \= 0 then
					mb~append(",")
				mb~append(Caller~ToSerializableData(item))
				i = 1
			end
		end
	end
        return mb~string
::METHOD DeSerialize CLASS
        use arg Caller,arguments
	ret = .Relation~new
	start = 0
	al = arguments~length
	do while al > start
		parse var arguments +(start) index ":" dataptr .
		start = start + index~length + dataptr~length + 2
		current_index = Caller~FromSerializedData(index)
		dstart = 0
		dl = dataptr~length
		do while dl > dstart
			parse var dataptr +(dstart) ptr ","
			dstart = dstart + ptr~length + 1
			ret[current_index] = -
				Caller~FromSerializedData(ptr)
		end
	end
        return ret

::CLASS SerializeTable SUBCLASS SerializeHandler
::METHOD Name CLASS
	return "TABLE"
::METHOD CheckType CLASS
	return .Table
::METHOD Serialize CLASS
        use arg Caller,Object
	mb = .MutableBuffer~new(" ",Object~items)
	do index over Object
		mb~append(Caller~ToSerializableData(index)||":")
		mb~append(Caller~ToSerializableData(Object[index])||" ")
	end
	return mb~string
::METHOD DeSerialize CLASS
        use arg Caller,arguments
	ret = .Table~new
	start = 0
	al = arguments~length
	do while al > start
		parse var arguments +(start) index ":" ptr .
		start = start + index~length + ptr~length + 2
		ret~put(Caller~FromSerializedData(ptr), -
			Caller~FromSerializedData(index))
	end
	return ret

::CLASS SerializeStem SUBCLASS SerializeHandler
::METHOD Name CLASS
	return "STEM"
::METHOD CheckType CLASS
	return .Stem
::METHOD Serialize CLASS
        use arg Caller,Object
	mb = .MutableBuffer~new
	if Object[] = "" then
		mb~append("-")
	else
		mb~append(Caller~ToSerializableData(Object[]))
	do index over Object
		mb~append(" "||Caller~ToSerializableData(index)||":"-
			||Caller~ToSerializableData(Object[index]))
	end
	return mb~string
::METHOD DeSerialize CLASS
        use arg Caller,arguments
	ret = .Stem~new
	parse var arguments default .
	if default \= '-' then
		ret[] = Caller~FromSerializedData(default)
	start = default~length+1
	al = arguments~length
	do while al > start
		parse var arguments +(start) index ":" ptr .
		start = start + index~length + ptr~length + 2
		ret[Caller~FromSerializedData(index)] = -
			Caller~FromSerializedData(ptr)
	end
	return ret

::CLASS SerializeDirectory SUBCLASS SerializeHandler
::METHOD Name CLASS
        return "DIRECTORY"
::METHOD CheckType CLASS
        return .Directory
::METHOD Serialize CLASS
        use arg Caller,Object
        mb = .MutableBuffer~new
	index = Object~makearray
	do i = 1 to index~size
		mb~append(Caller~ToSerializableData(index[i])||":")
		mb~append(Caller~ToSerializableData( -
			Object[index[i]])||" ")
        end
        return mb~string
::METHOD DeSerialize CLASS
        use arg Caller,arguments
	ret = .Directory~new
        start = 0
        al = arguments~length
        do while al > start
                parse var arguments +(start) index ":" ptr .
                start = start + index~length + ptr~length + 2
		ret~put(Caller~FromSerializedData(ptr), -
			Caller~FromSerializedData(index))
        end
        return ret

::CLASS SerializeArray SUBCLASS SerializeHandler
::METHOD Name CLASS
        return "ARRAY"
::METHOD CheckType CLASS
        return .Array
::METHOD Serialize CLASS
        use arg Caller,Object
	mb = .MutableBuffer~new
        do i = 1 to Object~size
		if .nil = Object[i] then iterate
		mb~append(i)
		mb~append(":")
		mb~append(Caller~ToSerializableData(Object[i]))
		mb~append(" ")
        end
        return mb~string
::METHOD DeSerialize CLASS
        use arg Caller,arguments
        ret = .Array~new()
        start = 0
        al = arguments~length
        do while al > start
                parse var arguments +(start) index ":" ptr .
                start = start + index~length + ptr~length + 2
                ret[index] = Caller~FromSerializedData(ptr)
        end
        return ret

::CLASS SerializeBag SUBCLASS SerializeHandler
::METHOD Name  CLASS
        return "BAG"
::METHOD CheckType CLASS
        return .Bag
::METHOD Serialize CLASS
        use arg Caller,Object
	mb = .MutableBuffer~new('',Object~items)
	do item over Object
		mb~append(Caller~ToSerializableData(item)||" ")
	end
	return mb~string
::METHOD DeSerialize CLASS
        use arg Caller,arguments
	ret = .Bag~new
	start = 0
	al = arguments~length
	do while al > start
		parse var arguments +(start) ptr .
		start = start + ptr~length + 1
		ret~Put(Caller~FromSerializedData(ptr))
	end
	return ret

::CLASS SerializeList SUBCLASS SerializeHandler
::METHOD Name CLASS
        return "LIST"
::METHOD CheckType CLASS
        return .List
::METHOD Serialize CLASS
        use arg Caller,Object
	mb = .MutableBuffer~new('',Object~items)
	do item over Object
		mb~append(Caller~ToSerializableData(item)||" ")
	end
	return mb~string
::METHOD DeSerialize CLASS
        use arg Caller,arguments
	ret = .List~new
	start = 0
	al = arguments~length
	do while al > start
		parse var arguments +(start) ptr .
		start = start + ptr~length + 1
		ret~Insert(Caller~FromSerializedData(ptr))
	end
	return ret

::CLASS SerializeQueue SUBCLASS SerializeHandler
::METHOD Name CLASS
        return "QUEUE"
::METHOD CheckType CLASS
        return .Queue
::METHOD Serialize CLASS
        use arg Caller,Object
	mb = .MutableBuffer~new('',Object~items)
	do item over Object
		mb~append(Caller~ToSerializableData(item)||" ")
	end
	return mb~string
::METHOD DeSerialize CLASS
        use arg Caller,arguments
	ret = .Queue~new
	start = 0
	al = arguments~length
	do while al > start
		parse var arguments +(start) ptr .
		start = start + ptr~length + 1
		ret~queue(Caller~FromSerializedData(ptr))
	end
	return ret

::CLASS SerializeCircularQueue SUBCLASS SerializeHandler
::METHOD Name CLASS
        return "CIRCULARQUEUE"
::METHOD CheckType CLASS
        return .CircularQueue
::METHOD Serialize CLASS
        use arg Caller,Object
	mb = .MutableBuffer~new('',Object~items)
	do item over Object
		mb~append(Caller~ToSerializableData(item)||" ")
	end
	return mb~string
::METHOD DeSerialize CLASS
        use arg Caller,arguments
	ret = .CircularQueue~new(arguments~words)
	start = 0
	al = arguments~length
	do while al > start
		parse var arguments +(start) ptr .
		start = start + ptr~length + 1
		ret~queue(Caller~FromSerializedData(ptr))
	end
