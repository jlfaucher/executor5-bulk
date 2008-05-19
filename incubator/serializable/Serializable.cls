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
-- Version: 1.9
/** <h1>Serializable Package</h1>
* This pacakge offers the classes needed to serialize rexx objects.
*/
if .environment~hasIndex("SERIALIZABLE") then return
.environment~Serializable = .Serializable
.environment~SerializeFunctions = .SerializeFunctions


/* Register default handlers */
.SerializeFunctions~setHandler(.SerializeMethod~new)
.SerializeFunctions~setHandler(.SerializeMutableBuffer~new)
.SerializeFunctions~setHandler(.SerializeNil~new)
.SerializeFunctions~setHandler(.SerializeRelation~new)
.SerializeFunctions~setHandler(.SerializeTable~new)
.SerializeFunctions~setHandler(.SerializeStem~new)
.SerializeFunctions~setHandler(.SerializeDirectory~new)
.SerializeFunctions~setHandler(.SerializeArray~new)
.SerializeFunctions~setHandler(.SerializeBag~new)
.SerializeFunctions~setHandler(.SerializeList~new)
.SerializeFunctions~setHandler(.SerializeQueue~new)
.SerializeFunctions~setHandler(.SerializeCircularQueue~new)

/** This class is the one indicating that an object can be serialized. A class
* has to inherit from Serializable if it needs serializing.
* 
* <code>::CLASS SomeClass MIXINCLASS Serializable</code>
*
* On Top if this is implements the default behaviour for the Read- and
* WriteObject methods. It assumes there is an public attribute called
* PersistentData that stores the relevant data. In case this method is not
* implemented an error is raised.
*/
::CLASS Serializable MIXINCLASS Object

/** This method is called for each sub class the rexx interpreter finds.
* It registers the the class with the serialization handlers to allow serialization
* and deserialization.
*/
::METHOD Init CLASS
/* Register sub classes */
-- at this point .Serializable is not yet created, thus we use the string
  if self~id \= "SERIALIZABLE" then
    .SerializeFunctions~serializableClasses[self~id] = self
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

/** This class offers the base functionality to serialize and deserialize data.
* It manages data handlers for managing a specific data type and offers a public
* function to serialize data and deserialize it.
* Assume variable A holds an object of a class inheriting from Serializable.
* In this case the following command will return the serialized object:
* <code>buffer = .SerializeFunctions~Serialize(A)</code>
*
* Deserializing works the same way (B will be like A~copy with references copied):
*  <code>B = .SerializableFunctions~DeSerialize(buffer)</code>
*
* The serializing and deserializing process is iterative. Thus there can be as
* many objects nested as desired - as long as the memory is not full. Recursive
* data structure should cause no problem.
******************************************************************************/
::CLASS SerializeFunctions PUBLIC
::ATTRIBUTE serializableClasses GET CLASS

::METHOD Init CLASS
/** initailize containers for handler classes
*/
  expose name type ClassType serializableClasses
  -- These are containers for data handlers
  name = .Directory~new
  ClassType = .Table~new
  serializableClasses = .Directory~new
  forward class (super)

::METHOD SetHandler CLASS
/** Used by SerializeHandler: registers a new handler
*/
  expose name ClassType
  use strict arg Handler
  if \ Handler~isInstanceOf(.SerializeHandler) then
    raise syntax 93
  if Handler~isInstanceOf(.CheckType) then
    ClassType[Handler~checktype] = Handler
  name[Handler~name] = Handler

-- Serialize a given object
/**
* <code>&gt;&gt;-Serialize(data)------------------------------------------------------------&gt;&lt;</code>
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
/** <pre>
*                           +-,-1------+<br/>
* >>-DeSerialize(Buffer-----+----------+-)--------------------------------------><<br/>
*                           +-,-Offset-+</pre>                                 
*/
::METHOD DeSerialize CLASS
  expose Name ClassType
  -- called with a string or mutable buffer.
  use arg Buffer
  -- Is parameter Offset set?
  if arg() >= 2 then do
    Offset = arg(2)
    -- It must be an integer within the boundaries
    if (datatype(Offset) \= "NUM") then
      -- Incorrect type
      raise syntax 93.905 array ("Offset", arg(2))
  end
  else
    Offset = 1
    
  -- Instace self
  Deserializer = self~new(Name,ClassType,Buffer)
  return Deserializer~FromSerializedDataCtrl(Offset)
  syntax: raise propagate
/**************************** INSTANCE METHODS ********************************/

::METHOD Init
  -- save the handlers and the string buffer
  expose Name ClassType Buffer
  use arg Name, ClassType, Buffer

/** Control the deserialization. This method works on the current serializer
*  instance.
*  @param offset The offset specifies the position the serialized data starts
*    in the buffer.
*/
::METHOD FromSerializedDataCtrl
  expose ObjectIndex Buffer SerialObjects
  parse arg Offset
  -- split the line
  limit = Buffer~pos(";",Offset)
  if limit < 2 then
    raise syntax 93.900 array ("Unable to locate data end marker")
  SerialObjects = Buffer~substr(Offset,limit-1)~makeArray(".")
  -- initialize the object index
  ObjectIndex = .array~new(SerialObjects~items)
  do i = SerialObjects~items to 1 by -1
    self~FromSerializedData(i)
  end
  -- the top index stores the first object referencing all other objects
  head = ObjectIndex[1]
  ObjectIndex = .nil
  SerialObject = .nil
  return head

/**
*  Do the actual deserialization on the specified position.
*  @param pos The entry to restore.
*/
::METHOD FromSerializedData
  expose Name ObjectIndex CurrentPos SerialObjects
  use arg pos
  -- check if there is something in the cache for the index
  if ObjectIndex~HasIndex(pos) then
    return ObjectIndex[pos]
  -- store the current position for FromSerializedDataPut
  CurrentPos = pos
  parse value SerialObjects[pos] with type arguments
  if Type = "S" then
    Object = Arguments~decodeBase64
  else if Type = "OBJECT" then do
    parse var arguments className reference
    data = self~fromSerializedData(reference)
    class = self~class~serializableClasses[self~fromSerializedData(className)]
    Object = Class~readObject(data)
  end
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
  return Object

-- early put method to allow recursive datastructure recreation
::METHOD FromSerializedDataPut
  expose CurrentPos ObjectIndex
  use arg Object
  ObjectIndex[CurrentPos] = Object

::METHOD ToSerializableDataCtrl
  expose ObjectList ClassType ObjectIndex Buffer StringIndex stat.
  use arg Object
  -- initalize some containers
  -- store the object -> id relation
  ObjectIndex = .table~new
  -- store the string -> id relation
  StringIndex = .table~new
  -- store id -> object relation
  ObjectList = .array~new
/*  
  do i = 1 to 2
    do j = 1 to 2 
      stat.i.j = 0
    end
  end
*/
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
      Buffer~append(Handler~name)
      Buffer~append(" ")
      Handler~Serialize(self, Buffer, Object)
    end
    else if object~isinstanceOf(.Serializable) then do
      Buffer~append("OBJECT ")
      Buffer~append(self~toSerializableData(Object~class~id))
      Buffer~append(" ")
      Buffer~append(self~ToSerializableData(Object~WriteObject))
    end
    -- the nil handler can not match by class
    else if Object = .nil then
      Buffer~append("NIL")
    -- no handler found and object is not .nil, so raise an error
    else raise syntax 93.900 array ("Unable to serialize object at position" position":" Object)
    -- go to the next position
    position += 1
  end
  -- append object structure end marker
  Buffer~append(";")
  -- return the array of lines
/*
Say "strings:" StringIndex~items
Say "other objects:" ObjectIndex~items
Say "cache: String: total:" stat.1.1 "unique:" stat.1.2
Say "       Object: total: "stat.2.1 "unique:" stat.2.2
*/
  return buffer

-- This method is called by handlers to register objects. It returns the
-- object's id. The object is stored but not yet serialized.

/**
* Strings are stored in the variable StringIndex, other objects in ObjectIndex.
* The reason for this is that there are situations where an object gets returnd
* where a string is needed, expecially when they both have the same string value.
* To avoid this strings have their own table, whereas other objects are stored
* via their identity hash value in a directory.
*/
::METHOD ToSerializableData
  expose ObjectList ObjectIndex StringIndex stat.
  use arg Object
  if Object~class = .String then do
    -- work on strings
--    stat.1.1 += 1
    -- this cache check will avoid serializing the same string again
    if StringIndex~hasIndex(Object) then
      return StringIndex[Object]
--    stat.1.2 += 1
    -- the ID will be the next ObjectList entry
    CurrPos = ObjectList~size + 1
    -- Store the ID -> string and string -> ID relation
    ObjectList[CurrPos] = Object
    StringIndex[Object] = CurrPos
    return Currpos
  end
  else do
    -- work on other objects
--    stat.2.1 += 1
    idhash = Object~identityHash
    -- this cache check will avoid serializing the same object again
    if ObjectIndex~HasIndex(idhash) then
      return ObjectIndex[idhash]
--    stat.2.2 += 1
    -- the ID will be the next ObjectList entry
    CurrPos = ObjectList~size + 1
    -- Store the ID -> object and object -> ID relation
    ObjectList[CurrPos] = Object
    ObjectIndex[idhash] = CurrPos
    return CurrPos
  end

/** Signature of a data handler:
* <ul>
* <li>It must directly inherit from SerializeHandler</li>
* <li>It must define <ul>
*  <li>Name: return the identifier used to store the data, must be unique</li>
*  <li>CheckType: class -- The class this handler can serialize
*    (some built-in handlers do not need this)</li>
*  <li>Serialize(Caller, Buffer, Object) -- appends data to the buffer</li>
*  <li>DeSerialize(Caller,Arguments): Object -- returns deserialized data</li></ul>
* </li>
* <li>Calls to Init MUST be forwarded, otherwise a handler will not register.</li>
* </ul>
*/
::CLASS SerializeHandler PUBLIC -- SUBCLASS Class
/** This method returns the name of the serialized object. The name is used to
* differenciate between different kind of objects on the stream. Note that not
* two equal names are allowed. Names may also contain no spaces.
*/
::METHOD Name ABSTRACT
/** This method serializes an object. It accepts the caller, the buffer, and
* the object to deserialize. The buffer stores the serial data itself. To
* serialize other objects and to get their serial ID use
* <code>Caller~ToSerializableData(object~someData)</code>. This method
* returns the id that can be used to deserialize the object again.
*/
::METHOD Serialize ABSTRACT
/** The deserialization method does the opposit to the serialization method:
* It uses a buffer to recreate an object. The object ID that the buffer
* stores can be turned into
* an object by calling the method <code>Caller~FromSerializedData(ID)</code>.
* The method has to return the new object.
*/
::METHOD DeSerialize ABSTRACT

/** Classes implementing the CheckType method can be matched
* by class directly.*/
::CLASS CheckType MIXINCLASS Object
/** This method retuns the class the handler can serialize.
*/
::METHOD CheckType ABSTRACT

/******************************************************************************
  Here follow the handler classes
******************************************************************************/
::CLASS SerializeMethod SUBCLASS SerializeHandler INHERIT CheckType
::METHOD Name
  return "METHOD"
::METHOD CheckType
  return .Method
::METHOD Serialize
  use strict arg Caller, Buffer, Object
  -- TODO: How to preserve the method's name?
  if Object~source~size = 0 then
    raise syntax 93.900 array ("Unable to serialize method without source")
  Buffer~append(Object~isGuarded*4+Object~isPrivate*2+Object~isProtected)
  Buffer~append(" ")
  Buffer~append(Caller~ToSerializableData(Object~source))
::METHOD DeSerialize
  use strict arg Caller,arguments
  parse var arguments mode arguments
  m =  .Method~new('', Caller~FromSerializedData(arguments))
  if mode~bitAnd(4) = 4 then m~setGuarded
  if mode~bitAnd(2) = 2 then m~setPrivate
  if mode~bitAnd(1) = 1 then m~setProtected
  return m

::CLASS SerializeMutableBuffer SUBCLASS SerializeHandler INHERIT CheckType
::METHOD Name
  return "MUTABLEBUFFER"
::METHOD CheckType
  return .MutableBuffer
::METHOD Serialize
  use strict arg Caller, Buffer, Object
  Buffer~append(Caller~ToSerializableData(Object~string))
::METHOD DeSerialize
  use strict arg Caller, pos
  return .MutableBuffer~new(Caller~FromSerializedData(pos))

::CLASS SerializeNil  SUBCLASS SerializeHandler
::METHOD Name
  return "NIL"
::METHOD Serialize
::METHOD DeSerialize
        return .nil


::CLASS SerializeCollection SUBCLASS SerializeHandler
::CLASS SerializeMap SUBCLASS SerializeCollection
::CLASS SerializeOrdered SUBCLASS SerializeCollection
::CLASS SerializeSet SUBCLASS SerializeCollection

::CLASS SerializeRelation SUBCLASS SerializeMap INHERIT CheckType
::METHOD Name
  return "RELATION"
::METHOD CheckType
  return .Relation
::METHOD Serialize
  use strict arg Caller, mb, Object
  s = Object~supplier
  relation_index = .Table~new
  do while s~available
    relation_index[s~index] = .true
    s~next
  end
  do index over relation_index
    if relation_index[index] = .true then do
      relation_index[index] = Caller~ToSerializableData(index)
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
::METHOD DeSerialize
  use strict arg Caller, arguments
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
      ret[current_index] = Caller~FromSerializedData(ptr)
    end
  end
  return ret

::CLASS SerializeTable SUBCLASS SerializeMap INHERIT CheckType
::METHOD Name
  return "TABLE"
::METHOD CheckType
  return .Table
::METHOD Serialize
  use strict arg Caller, mb, Object
  do index over Object
    mb~append(Caller~ToSerializableData(index) || ":")
    mb~append(Caller~ToSerializableData(Object[index]) || " ")
  end
::METHOD DeSerialize
  use strict arg Caller, arguments
  ret = .Table~new
  Caller~FromSerializedDataPut(ret)
  do word over arguments~makeArray(" ")
    parse var word index ":" ptr
    ret~put(Caller~FromSerializedData(ptr), -
      Caller~FromSerializedData(index))
  end
  return ret

::CLASS SerializeStem SUBCLASS SerializeMap INHERIT CheckType
::METHOD Name
  return "STEM"
::METHOD CheckType
  return .Stem
::METHOD Serialize
  use strict arg Caller,mb,Object
  -- indicate when there is no default value.
  if Object[] = "" then
    mb~append("-")
  else
    mb~append(Caller~ToSerializableData(Object[]))
  do index over Object
    mb~append(" " || Caller~ToSerializableData(index) || ":" -
      || Caller~ToSerializableData(Object[index]))
  end
::METHOD DeSerialize
  use strict arg Caller, arguments
  ret = .Stem~new
  Caller~FromSerializedDataPut(ret)
  content = arguments~makeArray(" ")
  if content[1] \= "-" then
    ret[] = Caller~FromSerializedData(content[1])
  content~remove(1)
  do word over content
    parse var word index ":" ptr .
    ret[Caller~FromSerializedData(index)] = -
      Caller~FromSerializedData(ptr)
  end
  return ret

::CLASS SerializeDirectory SUBCLASS SerializeMap INHERIT CheckType
::METHOD Name
  return "DIRECTORY"
::METHOD CheckType
  return .Directory
::METHOD Serialize
  use strict arg Caller, mb, Object
  index = Object~makearray
  do i = 1 to index~size
    mb~append(Caller~ToSerializableData(index[i]) || ":")
    mb~append(Caller~ToSerializableData(Object[index[i]])||" ")
  end
::METHOD DeSerialize
  use strict arg Caller, arguments
  ret = .Directory~new
  Caller~FromSerializedDataPut(ret)
  do word over arguments~makeArray(" ")
    parse var word index ":" ptr
    ret~put(Caller~FromSerializedData(ptr), -
    Caller~FromSerializedData(index))
  end
  return ret

::CLASS SerializeArray SUBCLASS SerializeOrdered INHERIT CheckType
::METHOD Name
  return "ARRAY"
::METHOD CheckType
  return .Array
::METHOD Serialize
  use strict arg Caller, mb, Object
  indexes = Object~allIndexes
  last = indexes~items
  do i = 1 to last
    ci = indexes[i]
    mb~append(ci)
    mb~append(":")
    mb~append(Caller~ToSerializableData(Object[ci]))
    if i \= last then
      mb~append(" ")
  end
::METHOD DeSerialize
  use strict arg Caller, arguments
  ret = .Array~new()
  Caller~FromSerializedDataPut(ret)
  do word over arguments~makeArray(" ")
    parse var word index ":" ptr
    ret[index] = caller~fromSerializedData(ptr)
  end
  return ret

::CLASS SerializeBag SUBCLASS SerializeSet INHERIT CheckType
::METHOD Name
  return "BAG"
::METHOD CheckType
  return .Bag
::METHOD Serialize
  use strict arg Caller, mb, Object
  do item over Object
    mb~append(Caller~ToSerializableData(item) || " ")
  end
::METHOD DeSerialize
  use strict arg Caller, arguments
  ret = .Bag~new
  Caller~FromSerializedDataPut(ret)
  do ptr over arguments~makeArray(" ")
    ret~put(caller~fromSerializedData(ptr))
  end
  return ret

::CLASS SerializeList SUBCLASS SerializeOrdered INHERIT CheckType
::METHOD Name
  return "LIST"
::METHOD CheckType
  return .List
::METHOD Serialize
  use strict arg Caller, mb, Object
  do item over Object
    mb~append(Caller~ToSerializableData(item) || " ")
  end
::METHOD DeSerialize
  use strict arg Caller, arguments
  ret = .List~new
  Caller~FromSerializedDataPut(ret)
  do ptr over arguments~makeArray(" ")
    ret~insert(caller~fromSerializedData(ptr))
  end
  return ret

::CLASS SerializeQueue SUBCLASS SerializeOrdered INHERIT CheckType
::METHOD Name
  return "QUEUE"
::METHOD CheckType
  return .Queue
::METHOD Serialize
  use strict arg Caller, mb, Object
  do item over Object
    mb~append(Caller~ToSerializableData(item) || " ")
  end
::METHOD DeSerialize
  use strict arg Caller, arguments
  ret = .Queue~new
  Caller~FromSerializedDataPut(ret)
  do ptr over arguments~makeArray(" ")
    ret~queue(caller~fromSerializedData(ptr))
  end
  return ret

::CLASS SerializeCircularQueue SUBCLASS SerializeOrdered INHERIT CheckType
::METHOD Name
  return "CIRCULARQUEUE"
::METHOD CheckType
  return .CircularQueue
::METHOD Serialize
  use strict arg Caller, mb, Object
  mb~append(Object~size" ")
  do item over Object
    mb~append(Caller~ToSerializableData(item) || " ")
  end
::METHOD DeSerialize
  use strict arg Caller,arguments
  contentptr = arguments~makeArray(" ")
  ret = .CircularQueue~new(contentptr[1])
  -- chop off the queue size
  contentptr~remove(1)
  Caller~FromSerializedDataPut(ret)
  do ptr over contentptr
    ret~queue(caller~fromSerializedData(ptr))
  end
  return ret

