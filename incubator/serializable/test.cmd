#!/opt/ooRexx/bin/rexx
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

say "Creating a new .test object"
a = .test~new
say "Filling it with data"
a~FillData
Say "Showing the data"
a~ShowData
Say "Serializing the object"
starttime = time('r')
t = .SErializeFunctions~Serialize(a)
Say "Here is the serialized data: Time needed for serializing:" time('r')

do i = 1 to t~size
--    call lineout "dmp.txt", i~right(4)||" "||t[i]
    say i~right(4)||" "||t[i]
end
Say "Deserializing the data and storing the class in b"
b = .SerializeFunctions~Deserialize(t)
say "Time needed for deserializing:" time('e')
Say "And that's the data b stores:"
b~ShowData

::REQUIRES "Serializable.cls"
::CLASS test MIXINCLASS Serializable
::METHOD PersistentData ATTRIBUTE
::METHOD FillData
    stem. = "I'm a stem variable"
    stem.1 = "first level entry"
    stem.1.1 = "sub entry"
    d = .directory~new
    d~method = .method~new("my method","Say 'hello'")
    d~mb = .mutablebuffer~new("This is some string")
    d~stem = stem.
    string = "Test"
    d~array = .array~of(.bag~of(string,string),.list~of(string,string))
    d~object = .test2~new
    self~PersistentData = d
::METHOD Showdata
    self~ShowDataRecursor(self~PersistentData,0)

::METHOD ShowDataRecursor
    use arg object,level
    objectclass = object~class
    indent = " "~copies((level+1)*3)
    select
	when objectclass = .array then do
	    say indent".Array"
	    do i = 1 to object~size
		if object[i] = .nil then iterate
--		call charout ,i~left(2)
		self~ShowDataRecursor(object[i],level+1)
	    end
	end
	when objectclass = .bag then do
	    say indent".Bag"
	    c = object~makearray
	    do i = 1 to c~size
		self~ShowDataRecursor(c[i],level+1)
	    end
	end
	when objectclass = .directory then do
	    say indent".Directory"
	    c = object~makearray
	    do i = 1 to c~size
		say indent||c[i]":"
		self~ShowDataRecursor(object[c[i]],level+1)
	    end
	end
	when objectclass = .list then do
	    say indent".List"
	    c = object~makearray
	    do i = 1 to c~size
		self~ShowDataRecursor(c[i],level+1)
	    end
	end
	when objectclass = .queue then do
	    say indent".Queue"
	    c = object~makearray
	    do i = 1 to c~size
		self~ShowDataRecursor(c[i],level+1)
	    end
	end
	when objectclass = .circularqueue then do
	    say indent".CircularQueue, " object~size "entries"
	    c = object~makearray
	    do i = 1 to c~size
		self~ShowDataRecursor(c[i],level+1)
	    end
	end
	when objectclass = .relation then do
	    say indent".Relation"
	    do i over object
		str = indent || i~string || " "
		do j over object~allat(i)
		    str = str||","||j~string
		end
		say str
	    end
	end
	when objectclass = .stem then do
	    say indent".Stem Default value:" object[]
	    do i over object
		say indent||i
		self~ShowDataRecursor(object[i],level+1)
	    end
	end
	when objectclass = .string then say indent".String:" object
	when objectclass = .mutablebuffer then say indent".MutableBuffer:("object~GetBufferSize")" object~string
	when object = .nil then say indent".Nil"
	when object~IsInstanceOf(.Serializable) then do
	    say indent||objectclass~string
	    if object~HasMethod("PersistentData") then
		if object~PersistentData != "PERSISTENTDATA" then
		    self~ShowDataRecursor(object~PersistentData,level+1)
	    end
	otherwise
	    say indent||objectclass~string
    end

::CLASS Test2 MIXINCLASS Serializable
::METHOD ReadObject CLASS
    use arg data
    return self~new(data)
::METHOD WriteObject
    return 'a'
::METHOD Init
    use arg data
