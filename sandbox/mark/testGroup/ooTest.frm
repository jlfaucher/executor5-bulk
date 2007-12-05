#!/usr/bin/rexx
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2007-2008 Rexx Language Association. All rights reserved.    */
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
/*----------------------------------------------------------------------------*/

/*
   name:             ooTest.frm
   authors:          Mark Miesfeld
   date:             2007-12-05
   version:          0.1.0

   purpose:          An extension to the ooRexxUnit framework providing function
                     and features specific to testing the ooRexx package.

   category:         Framework
*/

-- End of entry point.

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\
  Directives, Classes, or Routines.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::requires "OOREXXUNIT.CLS"

/* class: TestGroup- - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\

    Test Groups are containers of Tests from a single file.  Upon request they
    can provide a Test Suite consisting of all, or part, of the contained Tests.

    When a Test, in the form of the Test class object is added to a Test Group,
    the Test Group handles some of the rote chores used in configuring the Test.

\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::class "TestGroup" public

  -- pathName  The fully qualified path name of the file this test group
  -- represents.
  ::attribute pathName get
  ::attribute pathName set private

  -- testInfo  A directory object that is used to set the TestCaseInfo class
  -- attribute of each Test class this test group contains.
  ::attribute testInfo get
  ::attribute testInfo set private

  -- tests  A table of Test classes indexed by their category.
  ::attribute tests private

  -- hasTests  True if this group has any executable tests, otherwise false.
  ::attribute hasTests get
  ::attribute hasTests set private


  ::method init
    use strict arg fileSpec, src = (.array~new)

    fObj = .stream~new(fileSpec)
    self~pathName = fObj~query("EXISTS")
    if self~pathName == "" then
      raise syntax 88.917 array ("1 'fileSpec'", "must be an existing file path name")

    if \ src~isInstanceOf(.array) then
      raise syntax 88.914 array ("2 'src'", "Array")

    self~tests = .table~new
    self~hasTests = .false

    self~createMetaData(src)

  -- End init( )

  /** addTest()
   *
   * Adds a test class to this group.
   *
   */
  ::method addTest
    use arg strict test

    if \ isSubClassOf(test, "TestCase") then
      raise syntax 88.914 array ("1 'test'", "TestCase")

    if isSubClassOf(test, "TestSuite") then
      raise syntax 88.917 array ("1 'test'", "can not be a TestSuite subclass")

  -- End addTest()

  /** createMetaData()
   *
   * Creates a directory object with the metadata from the header of the test
   * group file.  The directory object is used to set the TestInfo class
   * attribute of any Test classes this group contains.
   *
   */
  ::method createMetaData private
    use arg src

    data = .directory~new
    data~setentry("test_Case-source", self~pathName);

    keyWord=""
    tOut=xrange("A","Z")||xrange("a","z")
    tIn =xrange("A","Z")||xrange("a","z")||xrange()

    do line over src
      line = line~strip
      ch = line~left(2)
      if ch == "--" | ch == "/*" | ch == "*/" then iterate

      -- If not a keyword line, add the line if needed and iterate.
      if line~pos(":") == 0 then do
        if keyWord \== "" then data~entry(keyWord)~queue(line)
        iterate
      end

      -- It is a keyword line, process it.
      parse var line name ":" rest
      keyword = name~translate(tOut, tIn)~space(0)

      -- If the key word has changed then add a new entry.
      if \ data~hasEntry(keyword) then
        data~setentry(keyword, .queue~new)

      data~entry(keyword)~queue(rest~strip)
    end

    self~cleanUpMetaData(data)
    self~testInfo = data
  -- End createMetaData()

  /** cleanUpMetaData()
   *
   * Cleans up the the metadata directory object.  If an entry is a queue, blank
   * lines at the end of the queue are stripped out.  If the queue has less than
   * 2 items, the queue is eliminated and the directory item is replaced by a
   * string.
   *
   */
  ::method cleanUpMetaData private
    use arg data

    keywords = data~allIndexes
    do keyword over keywords
      if data~entry(keyword)~isA(.queue) then do
        -- First strip out any items at the end of the queue that are blank
        -- lines.
        q = data~entry(keyword)
        i = q~last
        do while i <> .nil
          if q~at(i) \== "" then leave
          q~remove(i)
          i = q~last
        end

        -- Now, if the queue has only 1 line left in it (or none,) set the data
        -- item to a string consisting of just this line.  (Discard the queue.)
        if q~items < 2 then do
          item = q~pull
          if item == .nil then item = ""
          data~setEntry(keyword, item)
        end
      end
    end

  -- End cleanUpMetaData()

-- End of class: TestGroup

/** printTestInfo()
 *
 * Debugging method to print out the information in the TestInfo class attribute
 * of a TestCase class.  Curious as to how useful this information is.
 *
 */
::routine printTestInfo public
  use arg metaData

  itr = metaData~supplier
  do while itr~available
    keyword = itr~index || ":"
    data = itr~item

    if \ data~isA(.queue) then do
      say keyword~left(18) data
    end
    else do
      say
      say keyword
      do line over data
        say line
      end
      say
    end
    itr~next
  end
return 0
-- End printTestInfo()
