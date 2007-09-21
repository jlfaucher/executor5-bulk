/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2007 Rexx Language Association. All rights reserved.         */
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
	Instance = self~New(Data)
	return Instance

	syntax: raise propagate

/******************************************************************************
	The SerializeFunctions class
This class offers the base functionality to serialize and deserialize data.
It manages data handlers for managing a specific data type and offers a public
function to serialize data and deserialize it.
Assume variable a holds an object whichs class inherits from Serializable.
In this case the following command will return the serialized object:
	buffer = .SerializeFunctions~Serialize(a)

Deserializing works the same way (b will be like a~copy with references copied):
	b = .SerializableFunctions~DeSerialize(buffer)

The serializing and deserializing process is iterative. Thus there can be as
many objects nested as desired - as long as the memory is not full. Recursive
data structure should cause no problem.
******************************************************************************/
::CLASS SerializeFunctions PUBLIC

::METHOD Init CLASS
/* initailize containers for handler classes */
	expose name type ClassType
	-- These are containers for data handlers
	name = .directory~new
	ClassType = .table~new
	forward class (super)

::METHOD SetHandler CLASS
/* Used by SerializeHandler: registers a new handler */
	expose name ClassType
	use arg Handler
	-- !! Clean up, that's bad code
	if \ Handler~new~IsInstanceOf(.SerializeHandler) then
		raise syntax 93
	if Handler~hasMethod("CHECKTYPE") then
		ClassType[Handler~checktype] = Handler
	name[Handler~name] = Handler

-- Serialize a given object
/*
>>-Serialize(data)------------------------------------------------------------><
*/
::METHOD Serialize CLASS
	expose Name ClassType
	use arg Data, Buffer
	if symbol("BUFFER") = "LIT" then
		Buffer = .MutableBuffer~new
	signal on syntax
	-- create a new serializer instance, just for data encapsulation
	Serializer = self~new(Name, ClassType, Buffer)
	-- return the serialized data
	return Serializer~ToSerializableDataCtrl(Data)

	syntax: raise propagate

-- Method to deserialize stored objects
/*
                          +-,-0------+             
>>-DeSerialize(Buffer-----+----------+-)--------------------------------------><
                          +-,-Offset-+             
			                           
*/
::METHOD DeSerialize CLASS
	expose Name ClassType
	-- called with an array of lines
	use arg Buffer
	-- signal on syntax
	-- Is parameter Offset set?
	if arg() >= 2 then do
		Offset = arg(2)
		-- It must be an integer within the boundaries
		if  (datatype(Offset) \= "NUM") then
			-- Incorrect type
			raise syntax 93.905 array ("Offset", arg(2))
	end
	else
		Offset = 0
		
	-- Instace self
	Deserializer = self~new(Name,ClassType,Buffer)
	return Deserializer~FromSerializedDataCtrl(Offset)
	syntax: raise propagate
/**************************** INSTANCE METHODS ********************************/
::METHOD Init
	-- save the handlers and the string buffer
	expose Name ClassType Buffer
	use arg Name, ClassType, Buffer

::METHOD FromSerializedDataCtrl
	expose ObjectIndex Buffer Boundaries
	parse arg Offset
	Boundaries = .array~new
	-- get the upper boundary
	limit = Buffer~pos(";", Offset + 1)
	ObjEnd = Offset
	count = 1
	-- iterate as long as ObjEnd is found in front of the ;
	do until (ObjEnd >= limit) | (ObjEnd = 0)
		-- store the beginning of the object
		Boundaries~put(ObjEnd,count)
		count += 1
		-- find next start
		ObjEnd = Buffer~pos(".", ObjEnd+1)
	end
	-- put the end of the last object on the array
	Boundaries~put(limit, count)
	if count = 2 then raise syntax 93.900 array ("No data found!") 
	ObjectIndex = .array~new(Boundaries~items-1)
	-- convert the data to objects, starting at the last serialized object
	-- avoids recursion as later queries will hit the cache
	do i = Boundaries~items - 1 to 1 by -1
		self~FromSerializedData(i)
	end
	-- the top index stores the first object referencing all other objects
	Buffer = .nil
	Boundaries = .nil
	return ObjectIndex[1]

::METHOD FromSerializedData
	expose Name ObjectIndex Buffer CurrentPos Boundaries ExpectElement ExpectPos
	use arg pos
	-- check if there is somethig in the cache for the index
	if ObjectIndex~HasIndex(pos) then
		return ObjectIndex[pos]
	-- store the current position for FromSerializedDataPut
	CurrentPos = pos
	-- check if the poition cache is valid
	if ExpectPos \= pos then
		ExpectElement = Boundaries[pos + 1]
	-- the position cache is valid for the next element
	ExpectPos = pos - 1
	-- extract the current serialized object from the buffer
	ObjStart = Boundaries[pos] + 1
	SpacePos = Buffer~pos(" ", ObjStart)
	-- split the current string into type and arguments
	Type = Buffer~SubStr(ObjStart, SpacePos - ObjStart )
	Arguments = Buffer~SubStr(SpacePos + 1, ExpectElement - SpacePos - 1)
	if Type = "S" then
		Object = Arguments~decodeBase64
	else do
		-- there must be a class associated with type
		Handler = Name[Type]
		if Handler = .nil then raise syntax 93.914 array -
("object","a class handled by a .SerialzeHandler class",type)
		else
			-- use the class found to deserialize the data
			Object = Handler~DeSerialize(self, arguments)
	end
	ObjectIndex[pos] = Object
	ExpectElement = ObjStart - 1
	return Object

-- early put method to allow recursive datastructure recreation
::METHOD FromSerializedDataPut
	expose CurrentPos ObjectIndex
	use arg Object
	ObjectIndex[CurrentPos] = Object

::METHOD ToSerializableDataCtrl
	expose ObjectList ClassType ObjectIndex Buffer StringIndex --stat.
	use arg Object
	-- initalize some containers
	-- store the object -> id relation
	ObjectIndex = .directory~new
	-- store the string -> id relation
	StringIndex = .table~new
	-- store id -> object relation
	ObjectList = .array~new
	
--	do i = 1 to 2
--		do j = 1 to 2 
--			stat.i.j = 0
--		end
--	end

	-- the first item to be serialized is no. 1
	position = 1
	ObjectList[1] = Object
	-- work until all objects have been serialized
	do until position > ObjectList~size
		-- get the current object
		Object = ObjectList[position]
		-- only append dot after the first element
		if position > 1 then Buffer~append(".")
		-- check the way to serialize it
		-- match string diretly, not via handler
		if Object~class = .String then
			Buffer~append("S "Object~encodeBase64)
		else if ClassType~HasIndex(Object~class) then do
			-- it can be matched by class, get the handler
			Handler = ClassType[Object~class]
			Buffer~append(handler~name)
			Buffer~append(" ")
			Handler~Serialize(self, Buffer, Object)
		end
		-- the nil handler can not match by class
		else if Object == .nil then
			Buffer~append("NIL ")
		-- no handler found and object is not .nil, so raise an error
		else raise syntax 93
		-- go to the next position
		position += 1
	end
	-- append object structure end marker
	Buffer~append(";")
	-- return the array of lines
/*
Say "strings:" StringIndex~items
Say "other objects:" ObjectIndex~items
Say "cache: String:"stat.1.1"/"stat.1.2
Say "       Object:"stat.2.1"/"stat.1.2
*/
	return buffer

-- This method is called by handlers to register objects. It returns the
-- object's id. The object is stored but not yet serialized.

/*
Strings are stored in the variable StringIndex, other objects in ObjectIndex.
The reason for this is that there are situations where an object gets returnd
where a string is needed, expecially when they both have the same string value.
To avoid this stings have their own table, whereas other objects are stored
via their identity hash value in a directory.
*/
::METHOD ToSerializableData
	expose ObjectList ObjectIndex StringIndex --stat.
	use arg Object
	if Object~class = .String then do
		-- work on strings
--		stat.1.1 += 1
		-- this cache check will avoid serializing the same string again
		if StringIndex~hasIndex(Object) then
			return StringIndex[Object]
--		stat.1.2 += 1
		-- the ID will be the next ObjectList entry
		CurrPos = ObjectList~size + 1
		-- Store the ID -> string and string -> ID relation
		ObjectList[CurrPos] = Object
		StringIndex[Object] = CurrPos
		return Currpos
	end
	else do
		-- work on other objects
--		stat.2.1 += 1
		idhash = Object~identityHash~d2c
		-- this cache check will avoid serializing the same object again
		if ObjectIndex~HasIndex(idhash) then
			return ObjectIndex[idhash]
----		stat.2.2 += 1
		-- the ID will be the next ObjectList entry
		CurrPos = ObjectList~size + 1
		-- Store the ID -> object and object -> ID relation
		ObjectList[CurrPos] = Object
		ObjectIndex[idhash] = CurrPos
		return CurrPos
	end

/******************************************************************************
	The SerializeHandler Class

Signature of a data handler:
- It must directly inherit from SerializeHandler
- It must define
	- Name: return the identifier used to store the data, must be unique
	- CheckType: class -- The class this handler can serialize
		(some built-in handlers do not need this)
	- Serialize(Caller, Buffer, Object) -- appends data to the buffer
	- DeSerialize(Caller,Arguments): Object -- returns deserialized data
- Calls to Init MUST be forwarded, otherwise a handler will not register.
******************************************************************************/

::CLASS SerializeHandler PUBLIC -- SUBCLASS Class
::METHOD Init CLASS
	-- check if the class implements the required methods
	methods = .array~of("NAME","SERIALIZE","DESERIALIZE")
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
	use arg Caller, Buffer, Object
	Buffer~append(Caller~ToSerializableData(Object~WriteObject))
::METHOD DeSerialize
	expose myclass
	use arg Caller, arguments
	parse var arguments ptr
	return myclass~ReadObject(Caller~FromSerializedData(ptr))

::CLASS SerializeMethod SUBCLASS SerializeHandler
::METHOD Name CLASS
	return "METHOD"
::METHOD CheckType CLASS
	return .Method
::METHOD Serialize CLASS
	use arg Caller, Buffer, Object
	-- !! How to preserve the method's name?
	Buffer~append(Caller~ToSerializableData(Object~source))
::METHOD DeSerialize CLASS
	use arg Caller,arguments
	return .Method~new('', Caller~FromSerializedData(arguments))

::CLASS SerializeMutableBuffer SUBCLASS SerializeHandler
::METHOD Name CLASS
	return "MUTABLEBUFFER"
::METHOD CheckType CLASS
	return .MutableBuffer
::METHOD Serialize CLASS
	use arg Caller, Buffer, Object
	Buffer~append(Caller~ToSerializableData(Object~string))
::METHOD DeSerialize CLASS
	use arg Caller, pos
	return .MutableBuffer~new(Caller~FromSerializedData(pos))

::CLASS SerializeNil  SUBCLASS SerializeHandler
::METHOD Name CLASS
        return "NIL"
::METHOD Serialize CLASS
::METHOD DeSerialize CLASS
        return .nil

::CLASS SerializeRelation SUBCLASS SerializeHandler
::METHOD Name CLASS
        return "RELATION"
::METHOD CheckType CLASS
        return .Relation
::METHOD Serialize CLASS
        use arg Caller, mb, Object
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
			mb~append(relation_index[index] || ":")
			first = .true
			do item over Object~allat(index)
				if first \= .true then
					mb~append(",")
				mb~append(Caller~ToSerializableData(item))
				first = .false
			end
			mb~append(" ")
		end
	end
::METHOD DeSerialize CLASS
        use arg Caller, arguments
	ret = .Relation~new
	Caller~FromSerializedDataPut(ret)
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
        use arg Caller, mb, Object
	do index over Object
		mb~append(Caller~ToSerializableData(index) || ":")
		mb~append(Caller~ToSerializableData(Object[index]) || " ")
	end
::METHOD DeSerialize CLASS
        use arg Caller, arguments
	ret = .Table~new
	Caller~FromSerializedDataPut(ret)
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
        use arg Caller,mb,Object
	-- indicate when there is no default value.
	if Object[] = "" then
		mb~append("-")
	else
		mb~append(Caller~ToSerializableData(Object[]))
	do index over Object
		mb~append(" " || Caller~ToSerializableData(index) || ":" -
			|| Caller~ToSerializableData(Object[index]))
	end
::METHOD DeSerialize CLASS
        use arg Caller, arguments
	ret = .Stem~new
	Caller~FromSerializedDataPut(ret)
	parse var arguments default .
	if default \= "-" then
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
        use arg Caller, mb, Object
	index = Object~makearray
	do i = 1 to index~size
		mb~append(Caller~ToSerializableData(index[i]) || ":")
		mb~append(Caller~ToSerializableData( -
			Object[index[i]]) || " ")
        end
::METHOD DeSerialize CLASS
        use arg Caller, arguments
	ret = .Directory~new
	Caller~FromSerializedDataPut(ret)
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
        use arg Caller, mb, Object
        do i = 1 to Object~size
		mb~append(i)
		mb~append(":")
		mb~append(Caller~ToSerializableData(Object[i]))
		mb~append(" ")
        end
::METHOD DeSerialize CLASS
        use arg Caller, arguments
        ret = .Array~new()
	Caller~FromSerializedDataPut(ret)
        start = 0
        al = arguments~length
        do while al > start
                parse var arguments +(start) index ":" ptr .
                start = arguments~pos(" ",start+1)
--                start = start + index~length + ptr~length + 2
                ret[index] = Caller~FromSerializedData(ptr)
        end
        return ret

::CLASS SerializeBag SUBCLASS SerializeHandler
::METHOD Name  CLASS
        return "BAG"
::METHOD CheckType CLASS
        return .Bag
::METHOD Serialize CLASS
        use arg Caller, mb, Object
	do item over Object
		mb~append(Caller~ToSerializableData(item) || " ")
	end
::METHOD DeSerialize CLASS
        use arg Caller, arguments
	ret = .Bag~new
	Caller~FromSerializedDataPut(ret)
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
        use arg Caller, mb, Object
	do item over Object
		mb~append(Caller~ToSerializableData(item) || " ")
	end
::METHOD DeSerialize CLASS
        use arg Caller, arguments
	ret = .List~new
	Caller~FromSerializedDataPut(ret)
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
        use arg Caller, mb, Object
	do item over Object
		mb~append(Caller~ToSerializableData(item) || " ")
	end
::METHOD DeSerialize CLASS
        use arg Caller, arguments
	ret = .Queue~new
	Caller~FromSerializedDataPut(ret)
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
        use arg Caller, mb, Object
	do item over Object
		mb~append(Caller~ToSerializableData(item) || " ")
	end
::METHOD DeSerialize CLASS
        use arg Caller,arguments
	ret = .CircularQueue~new(arguments~words)
	Caller~FromSerializedDataPut(ret)
	start = 0
	al = arguments~length
	do while al > start
		parse var arguments +(start) ptr .
		start = start + ptr~length + 1
		ret~queue(Caller~FromSerializedData(ptr))
	end
	return ret
