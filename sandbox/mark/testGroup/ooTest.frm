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

  -- currentTypes  A set of the categories of tests this group currently contains.
  ::attribute currentTypes get
  ::attribute currentTypes set private

  -- machineOS  The operating system this group is executing on.
  ::attribute machineOS private

  -- knownOSes  A set containing all the possible OSes that ooRexx will run on.
  -- This set is intended to be immutable.  If / when ooRexx is compilable on
  -- addtional operating systems, the set will need to be updated.
  ::attribute knownOSes get
  ::attribute knownOSes set private

  -- allowedOSes  A set containing the OSes the tests in this group can execute
  -- on.  By default the set contains all known OSes.
  ::attribute allowedOSes get
  ::attribute allowedOSes set private

  -- hasTests  True if this group has any executable tests, otherwise false.
  ::attribute hasTests get
  ::attribute hasTests set private


  /** init()
   *
   * Initializes a new test group instance.
   *
   * @param fileSpec  REQUIRED
   *   The path name of the file this test group represents.  The file must
   *   exist.  Relative path names are acceptable, if they will resolve from the
   *   current working directory.  This is UNLIKELY to be the case in an
   *   automated test run, so the fully qualified path name is usually needed.
   *
   * @param data      OPTIONAL
   *   An array of lines containing the metadata from the file header for this
   *   test group.  This is only useful if the file header is in the format
   *   specified in the ooTest Framework reference.
   */
  ::method init
    use strict arg fileSpec, data = (.array~new)

    fObj = .stream~new(fileSpec)
    self~pathName = fObj~query("EXISTS")
    if self~pathName == "" then
      raise syntax 88.917 array ("1 'fileSpec'", "must be an existing file path name")

    if \ data~isInstanceOf(.array) then
      raise syntax 88.914 array ("2 'data'", "Array")

    self~tests = .table~new
    self~hasTests = .false
    self~currentTypes = .set~new

    -- All possible OS words are put into the allowed OSes set, although it is
    -- doubtful that ooRexx is compiled on the last 3.
    self~knownOSes = .set~of('WINDOWS', 'LINUX', 'SUNOS', 'AIX', 'MACOSX', 'CYGNUS', 'FREEBSD', 'NETBSD')
    self~allowedOSes = self~knownOSes~copy
    self~machineOS = .ooRexxUnit.OSName

    self~createMetaData(data)

  -- End init( )

  /** restrictOS()
   *
   * Alerts this test group that the tests it contains are operating system
   * specific.  By default tests in a test group are expected to execute on the
   * set of all OSes that ooRexx runs on.  However, test groups can be
   * restricted to only produce test suites for a subset of those OSes.
   *
   * @param acceptable
   *   A set of OS words that this test group should be restricted to.  This can
   *   be either a string of blank separated OS words, or a set of the OS words.
   *   Case is not significant.  The set must be a subset of the known OS words.
   *
   */
  ::method restrictOS
    use strict arg acceptable

    s = .set~new
    select
      when acceptable~isA(.string) then do os over acceptable~space(1)~makearray(" ")
        s~put(os~translate)
      end

      when acceptable~isA(.set) then do os over = acceptable
        s~put(os~translate)
      end

      otherwise raise syntax 88.914 array ("1 'acceptable'", "'String' or 'Set'")
    end
    -- End select

    if \ s~subset(self~knownOSes) then
      raise syntax 88.914 array ("1 'acceptable'", "is not a subset of the known operating systems")

    self~allowedOSes = s
    if \ self~allowedOSes~hasIndex(self~machineOS) then self~hasTests = .false

  -- End restrictOS()

  /** add()
   *
   * Adds a test class object to this group.  The class object has to be a
   * direct subclass of ooTestCase.
   *
   */
  ::method add
    expose tests
    use strict arg test

    if \ isSubClassOf(test, "ooTestCase") then
      raise syntax 88.917 array ("1 'test'", "must be a subclass of the ooTestCase class. Found:" test)

    if tests~hasIndex(test~ooTestType) then
      raise syntax 88.917 array ("1 'test'", "this test group already contains a test of type" test~ooTestType)

    tests[test~ooTestType] = test
    self~hasTests = .true

  -- End add()

  ::method suite
    expose tests
    use strict arg type, testSuite = (.TestSuite~new)

    do t over type
      testClass = tests[t~translate]
      if testClass <> .nil then testSuite~addTest(.TestSuite~new(testClass))
    end

  return testSuite

  /** createMetaData()
   *
   * Creates a directory object with the metadata from the header of the test
   * group file.  The directory object is used to set the TestInfo class
   * attribute of any Test classes this group contains.
   *
   */
  ::method createMetaData private
    use arg data

    data = .directory~new
    data~setentry("test_Case-source", self~pathName);

    keyWord=""
    tOut=xrange("A","Z")||xrange("a","z")
    tIn =xrange("A","Z")||xrange("a","z")||xrange()

    do line over data
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

/* class: ooTestCase - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\

    ooTest Cases are used to test the ooRexx interpreter package.

\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::class 'ooTestCase' public subclass TestCase

  -- ooTestType  The type of test cases contained in this test case class.  The
  -- default type is "UNIT" and other typical types are "STRESS", "SAMPLE",
  -- "GUI_SAMPLE", "DOC_EXAMPLE", etc..
  ::attribute ooTestType get class
  ::attribute ooTestType set class private

  ::method init class
    forward class (super) continue
    self~ooTestType = "UNIT"

  -- End init( )

-- End of class: ooTestCase

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
