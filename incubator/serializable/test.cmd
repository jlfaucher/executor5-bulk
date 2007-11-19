#!/opt/ooRexx/bin/rexx
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
-- This sections defines the configurations
--debug = 0 -- true: run a test on all available types, false: performance test
p.debug_elements = 1e5+0 -- number of elements in performance test
p.iterations =  8 -- sample number, minimum is 2
-- End of configuration

call RxFuncAdd "MathLoadFuncs","rxmath","MathLoadFuncs"
call MathLoadFuncs


parse arg argvalue
if argvalue = "" then do
  -- simple arguments parsing
  call help
  exit
end
else do
  start = 0
  len = argvalue~length
  do while start < len
    parse upper var argvalue +(start) op value .
    SELECT
      when op = "-M" then do
        if value~abbrev("TES") then debug = .true
        else
          if value~abbrev("PER") then
            debug = .false
          else do
call help( "Incorrect mode:" value)
            exit 1
          end
        start += 4 + value~length
      end
      when op = "-I" & debug \= .true then do
        if value~datatype("W") then
          if value > 1 then do
            p.iterations = value
            marker = .true
          end
        if marker \= .true then do
call help("Incorrect number of interations :" value-
  "(must be a whole number > 1)")
          exit 1
        end
        drop marker
        start += 4 + value~length
      end
      when op = "-V" & debug \= .true then do
        start += 3
        p.verbose = .true
      end
      when (op = "-Q" | op = "--CSV") & debug \= .true then do
        start += op~length
        p.verbose = .true
        p.quiet = .true
      end
      when op = "-N" & debug \= .true then do
        if value~datatype("W") then
        if value > 0 then do
          p.debug_elements = value
          marker = .true
        end
        if marker \= .true then do
call help("Incorrect element number:" value)
          exit 1
        end
        drop maker
        start += 4 + value~length
      end
      otherwise do
        call help("Unknown parameter:" op)
        exit 1
      end
    END
  end
end



if debug = .true then call testcase
else if debug = .false then call performance
else call help("Wrong arguments or arguments missing!")
return

testcase: procedure expose p.
say "Creating a new .test object"
a = .test~new
a~debug = .true
say "Filling it with data"
a~FillData
Say "Showing the data"
a~ShowData
Say "Serializing the object"
starttime = time('r')
t = .SerializeFunctions~Serialize(a)
Say "Serialized data: Time needed for serializing:" time('r')
Say t~string
say t~Substr(100,3)
Say "Deserializing the data and storing the class in b"
b = .SerializeFunctions~Deserialize(t)
say "Time needed for deserializing:" time('e')
Say "And that's the data b stores:"
b~ShowData
return

performance: procedure expose p.
times_s = .array~new
times_d = .array~new
a = .test~new
a~debug = .false
a~elements = p.debug_elements
a~FillData
do i = 1 to p.iterations
  drop t b
  call time('R')
  t = .SerializeFunctions~Serialize(a)
  times_s[i] = time('R')
  a = .SerializeFunctions~Deserialize(t)
  times_d[i] = time('R')
  if p.quiet \= .true then
    call charout , "."
end
if p.quiet \= .true then do
  say
  data = stdev(times_s)
  Say "Normal distribution for serializing: S~N("data[2]","data[3]")"
  data = stdev(times_d)
  Say "Normal distribution for de-serializing: D~N("data[2]","data[3]")"
end
else do
  say '"Time serializing","Time de-serializing"'
  do i = 1 to p.iterations
    say times_s[i]","times_d[i]
  end
end
return

stdev: procedure expose p.
use arg value
total = 0
do i = 1 to value~size
  total = total + value[i]
  if p.verbose = .true then
    call lineout ,value[i]
end
average = total/value~size
sum = 0
do i = 1 to value~size
  sum = sum + abs(value[i]-average)**2
end
variance = sum / (value~size-1)
variance = RxCalcSqrt(variance)
return .array~of(total,average,variance)

help: procedure expose p.
if arg(1,'O') then do
  parse source . . app
  -- add some voodoo to handle path correctly
  app = app~right(app~length - app~lastpos("/")+1)~strip
  app = "..."app~right(app~length - app~lastpos("\")+1)~strip
  lines = .array~of( -
    "Serializable class test application",-
    "Copyright (c) 2007 Rexx Language Association. All rights reserved.",-
    "Moritz Hoffmann 2007, CPL V 1.0",-
    "Usage:",-
    app " -m test",-
    app " -m performance [-i "p.iterations"] [-n "p.debug_elements"] [-v] [--csv]",-
    ,-
    "-m  Defines the test to run. TEST is a general purpose test that serializes",-
    "  all serializable objects. PERFORMANCE runs a performance test.",-
    "The following options are only relevant for the performance test:",-
    "-i  Number of iterations for collectiong data.",-
    "-n  Number of objects.",-
    "-v  Enables verbose mode.",-
    "--csv  Prints raw data as comma seperated values."-
  )
  say lines~makestring
end
if \arg(1,"o") then say arg(1)
else do
  lines = .array~of(-
    "Example:",-
    app "-m test",-
    app "-m perf -i 64 -n 1e4",-
    app "-m perf -v",-
    app "-m perf -i 64 -n 1000 --csv > data.csv"-
  )
  say lines~makestring
end
return

::REQUIRES "Serializable.cls"
::CLASS test MIXINCLASS Serializable
::METHOD Debug ATTRIBUTE
::METHOD Elements ATTRIBUTE
::METHOD PersistentData ATTRIBUTE
::METHOD FillData
  expose debug
  if debug then do
    stem. = "I'm a stem variable"
    stem.1 = "first level entry"
    stem.1.1 = "sub entry"
    d = .directory~new
    d~method = .method~new("my method","Say 'hello'")
    d~mb = .mutablebuffer~new("This is some string")
    d~stem = stem.
    string = "Test, occurring several times"
    d~array = .array~of(.bag~of(string,string),.list~of(string,string))
    d~object = .test2~new
    d~nil = .nil
    r = .relation~new
    r[string] = 1
    r[string] = 42
    r[42] = string
    d~relation = r
    t = .table~new
    t[string] = stem.
    d~table = t
    q = .queue~new
    q~queue(string)
    d~queue = q
    d~cq = .circularqueue~of(r,string)
    a = .array~new
    a[1] = a
    d~array = a
  end
  else do
    d = .array~new
    offset = self~elements
    do i = 1 to self~Elements
      d[i] = i + offset
    end
  end
  self~PersistentData = d

::METHOD Showdata
  self~ShowDataRecursor(self~PersistentData,0)

::METHOD ShowDataRecursor
  use arg object,level
  objectclass = object~class
  indent = " "~copies(level*3)
  if level > 10 then do
    say indent"..."
    return
  end
  select
    when objectclass = .array then do
      say indent".Array"
      do i = 1 to object~size
        if object[i] = .nil then iterate
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
        str = indent || i~string || ": "
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
    when object~isInstanceOf(.Method) then do
      mod = " "
      if Object~isGuarded then mod ||="Guarded "
      if Object~isPrivate then mod ||="Private "
      if Object~isProtected then mod ||="Protected"
      say indent||objectclass~string||mod
        self~ShowDataRecursor(object~source,level+1)

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
