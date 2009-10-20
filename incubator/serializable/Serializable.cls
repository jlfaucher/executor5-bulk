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
::CLASS Serializable MIXINCLASS Object PUBLIC

-- Method to store object persistently. Has to return its data. Default
-- implementation tries to access PersistentData.
::METHOD WriteObject ABSTRACT

-- Method to recreate an object. Creates an instance of self and forwards the
-- data to New.
::METHOD ReadObject ABSTRACT

/** This class offers the base functionality to serialize and deserialize data.
* It manages data handlers for managing a specific data type and offers a public
* function to serialize data and deserialize it.
* Assume variable A holds an object of a class inheriting from Serializable.
* In this case the following command will return the serialized object:
* <code>buffer = .SerializeFunctions~Serialize(A)</code>
*
* Deserializing works the same way (B will be like A~copy with references copied):
*    <code>B = .SerializableFunctions~DeSerialize(buffer)</code>
*
* The serializing and deserializing process is iterative. Thus there can be as
* many objects nested as desired - as long as the memory is not full. Recursive
* data structure should cause no problem.
******************************************************************************/
::CLASS SerializeFunctions PUBLIC

-- Serialize a given object
/**
* <code>&gt;&gt;-Serialize(data)------------------------------------------------------------&gt;&lt;</code>
*/
::METHOD Serialize CLASS
    use strict arg data, buffer=(.MutableBuffer~new)
    signal on syntax
    -- create a new serializer instance, just for data encapsulation
    serializer = self~new(buffer)
    -- return the serialized data
    return serializer~toSerializableDataCtrl(data)

    syntax: raise propagate

-- Method to deserialize stored objects
/** <pre>
*                           +-,-1------+<br/>
* >>-DeSerialize(Buffer-----+----------+-)--------------------------------------><<br/>
*                           +-,-Offset-+</pre> 
*/
::METHOD DeSerialize CLASS
    -- called with a string or mutable buffer.
    use strict arg Buffer, package=(.Context~package)
    deserializer = self~new(buffer, package)
    return deserializer~fromSerializedDataCtrl(1)
    syntax: raise propagate

/**************************** INSTANCE METHODS ********************************/

::METHOD Init
    -- save the handlers and the string buffer
    expose buffer package
    use arg buffer, package=(.Context~package)

/** Control the deserialization. This method works on the current serializer
*    instance.
*    @param offset The offset specifies the position the serialized data starts
*        in the buffer.
*/
::METHOD FromSerializedDataCtrl
    expose objectIndex buffer serialObjects
    parse arg Offset
    -- split the line
    limit = buffer~pos(";", offset)
    if limit < 2 then
        raise syntax 93.900 array ("Unable to locate data end marker")
    serialObjects = buffer~substr(offset, limit-1)~makeArray(".")
    -- initialize the object index
    objectIndex = .array~new(serialObjects~items)
    do i = serialObjects~items to 1 by -1
        self~fromSerializedData(i)
    end
    -- the top index stores the first object referencing all other objects
    head = objectIndex[1]
    objectIndex = .nil
    serialObject = .nil
    return head

/**
*    Do the actual deserialization on the specified position.
*    @param pos The entry to restore.
*/
::METHOD FromSerializedData
    expose objectIndex currentPos serialObjects references current package
    use arg pos
    -- check if there is something in the cache for the index
    if ObjectIndex~HasIndex(pos) then
        return ObjectIndex[pos]
    -- store the current position for FromSerializedDataPut
    CurrentPos = pos
    references = serialObjects[pos]~makeArray(" ")
    if references[1] = "S" then do
        object = references[2]~decodeBase64
        objectIndex[pos] = object
    end
    else if references[1] = "OBJECT" then do
        class = package~findClass(self~fromSerializedData(references[2]))
        current = 3
        object = class~new
        objectIndex[pos] = object
        object~readObject(self)
    end
    return object

::METHOD ToSerializableDataCtrl
    expose objectList objectIndex buffer stringIndex stat.
    use strict arg object
    -- initalize some containers
    -- store the object -> id relation
    objectIndex = .IdentityTable~new
    -- store the string -> id relation
    stringIndex = .Table~new
    -- store id -> object relation
    objectList = .Array~new
/*    
    do i = 1 to 2
        do j = 1 to 2 
            stat.i.j = 0
        end
    end
*/
    -- the first item to be serialized is no. 1
    position = 1
    objectList[1] = object
    -- work until all objects have been serialized
    do until position > objectList~size
        -- get the current object
        object = objectList[position]
        -- only append dot after the first element
        if position > 1 then buffer~append(".")
        -- check the way to serialize it
        -- match string diretly, not via handler
        if object~class = .String then
            buffer~append("S ")
            buffer~append(object~encodeBase64)
        else if object~isInstanceOf(.Serializable) then do
                buffer~append("OBJECT ")
                buffer~append(self~toSerializableData(object~class~id))
                -- Ask the object to store its data. Provide self as callback.
                object~writeObject(self)
        end
        -- the nil handler can not match by class
        else if object = .nil then
            buffer~append("NIL")
        -- no handler found and object is not .nil, so raise an error
        else raise syntax 93.900 array ("Unable to serialize object at position" position":" object)
        -- go to the next position
        position += 1
    end
    -- append object structure end marker
    buffer~append(";")
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
    expose objectList objectIndex stringIndex stat.
    use strict arg object
    if object~class = .String then do
        -- work on strings
--        stat.1.1 += 1
        -- this cache check will avoid serializing the same string again
        if stringIndex~hasIndex(object) then
            return stringIndex[object]
--        stat.1.2 += 1
        -- the ID will be the next ObjectList entry
        currPos = objectList~size + 1
        -- Store the ID -> string and string -> ID relation
        objectList[currPos] = object
        stringIndex[object] = currPos
        return currpos
    end
    else do
        -- work on other objects
--        stat.2.1 += 1
        -- this cache check will avoid serializing the same object again
        if objectIndex~HasIndex(object) then
            return objectIndex[object]
--        stat.2.2 += 1
        -- the ID will be the next ObjectList entry
        currPos = objectList~size + 1
        -- Store the ID -> object and object -> ID relation
        objectList[currPos] = object
        objectIndex[object] = currPos
        return CurrPos
    end

::METHOD writeObject
    expose buffer
    use strict arg object
    pos = self~toSerializableData(object)
    buffer~append(" ")
    buffer~append(pos)

::METHOD readObject
    expose references current
    if current > references~items then
        raise syntax 93
    object = self~fromSerializedData(references[current])
    current = current + 1
    return object

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
    m =    .Method~new('', Caller~FromSerializedData(arguments))
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

::CLASS SerializeNil    SUBCLASS SerializeHandler
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

