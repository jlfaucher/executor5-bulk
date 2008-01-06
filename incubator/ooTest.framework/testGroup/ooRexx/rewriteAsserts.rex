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
   name:             rewriteAsserts.rex
   authors:          Mark Miesfeld
   date:             2008-01-05
   version:          0.1.0

   purpose:          A brute force parser that rewrites the assertXXX() methods.
                     This is really a throw away program, it is just being
                     retained in case it can be tweaked and used in the future
                     for some pupose.
*/
cmdLine = arg(1)

  .local~eol = .endOfLine

  -- If command line is exactly "FINALIZE" then all the *.testGroupX files will
  -- be copied over to the matching *.testGroup file and the testGoupX file
  -- will be deleted.

  if cmdLine == "FINALIZE" then do
    z = doFinalize()
    return
  end

  .local~logger = .stream~new("AssertRewrite.log")
  .logger~open("WRITE REPLACE")
  .logger~lineout(.eol)
  .logger~lineout('Swap args in all assert methods.')
  .logger~lineout(.eol)

  files = findFiles('*.testGroup')

  do file over files
    .logger~lineout('Working on:' file)
    if \ writeNew(file) then do
      .logger~lineout("  Some error happened.  Aborting on this file.")
      .logger~lineout("")
      -- don't leave, process all files
    end
    .logger~lineout("")
  end

  .logger~close

return 0

::routine doFinalize

  .local~logger = .stream~new("Finalize.log")
  .logger~open("WRITE REPLACE")
  .logger~lineout('Make all *.testGroupX files the real *.testGroup file.')
  .logger~lineout(.eol)

  parse upper source os .
  if os~abbrev("WIN") then os = "WINDOWS"

  select
    when os == "WINDOWS" then do
      copyCmd = 'copy /Y /V'
      delCmd = 'del /f'
    end
    when os == "LINUX" then do
      copyCmd = 'mv'
      delCmd = 'rm'
    end
    otherwise raise syntax 93.964 array ("Application can only run Windows or Linux")
  end

  files = findFiles("*.testGroupX")
  do file over files
    tGroup = file~strip('T', "X")
    .logger~lineout("Overwriting" filespec('N', tGroup) "with" filespec('N', file))
    copyCmd file tGroup
    delCmd file
  end
  .logger~close

return 0

::routine writeNew
  use arg file

  newFile = file || 'X'

  oldObj = .stream~new(file)
  lines = oldObj~~open("READ")~arrayin
  oldObj~close

  newObj = .stream~new(newFile)
  newObj~open("WRITE REPLACE")
  if newObj~state \== "READY" then do
    .logger~lineout('Error opening new file:' newFile)
    .logger~lineout('Skipping this file.')
    .logger~lineout("")
    return .true   -- Probably should quit, but will keep trying.
  end

  okay = doOut(newObj, lines)
  return okay

-- Rewrites the file.  Returns false on error.  Assumes fsObj is open and closes
-- fsObj before returning.
::routine doOut
  use arg fsObj, lines

  okay = .true

  do i = 1 to lines~items
    select
      when lines[i]~caselessPos("self~assertEquals(") <> 0 then do
        h = .AssertHolder~new(lines, lines[i], i)
        --say 'line:' lines[i]
        line = maybeJoinLines(h)
        if line~dataType('O') then do
          okay = .false
          leave
        end
        --say 'line-' line
        if i <> h~i then do
          .logger~lineout('  Joined line' i 'with the next' (h~i - i) 'line(s)')
          i = h~i
        end

        parenStart = line~caselessPos("self~assertEquals(") + 17
        parenEnd   = findParenEnd(line, parenStart)
        if parenEnd == 0 then do
          okay = logBadParen(i)
          leave
        end

        line = swapArgs(2, line, parenStart, parenEnd, i)
      end

      when lines[i]~caselessPos("self~assertNotEquals(") <> 0 then do
        h = .AssertHolder~new(lines, lines[i], i)
        -- say 'line:' lines[i]
        line = maybeJoinLines(h)
        -- say 'line:' line
        if line~dataType('O') then do
          okay = .false
          leave
        end
        -- say 'line-' line
        if i <> h~i then do
          .logger~lineout('  Joined line' i 'with the next' (h~i - i) 'line(s)')
          i = h~i
        end

        parenStart = line~caselessPos("self~assertNotEquals(") + 20
        -- say 'parenStart:' parenStart
        parenEnd   = findParenEnd(line, parenStart)
        if parenEnd == 0 then do
          okay = logBadParen(i)
          leave
        end

        line = swapArgs(2, line, parenStart, parenEnd, i)
      end

      when lines[i]~caselessPos("self~assertNull(") <> 0 then do
        h = .AssertHolder~new(lines, lines[i], i)
        -- say 'line:' lines[i]
        line = maybeJoinLines(h)
        if line~dataType('O') then do
          okay = .false
          leave
        end
        -- say 'line-' line
        if i <> h~i then do
          .logger~lineout('  Joined line' i 'with the next' (h~i - i) 'line(s)')
          i = h~i
        end

        parenStart = line~caselessPos("self~assertNull(") + 15
        parenEnd   = findParenEnd(line, parenStart)
        if parenEnd == 0 then do
          okay = logBadParen(i)
          leave
        end
        -- say 'line-' line
        line = swapArgs(1, line, parenStart, parenEnd, i)
      end

      when lines[i]~caselessPos("self~assertNotNull(") <> 0 then do
        h = .AssertHolder~new(lines, lines[i], i)
        -- say 'line:' lines[i]
        line = maybeJoinLines(h)
        if line~dataType('O') then do
          okay = .false
          leave
        end
        -- say 'line-' line
        if i <> h~i then do
          .logger~lineout('  Joined line' i 'with the next' (h~i - i) 'line(s)')
          i = h~i
        end

        parenStart = line~caselessPos("self~assertNotNull(") + 18
        parenEnd   = findParenEnd(line, parenStart)
        if parenEnd == 0 then do
          okay = logBadParen(i)
          leave
        end

        line = swapArgs(1, line, parenStart, parenEnd, i)
      end

      when lines[i]~caselessPos("self~assertSame(") <> 0 then do
        h = .AssertHolder~new(lines, lines[i], i)
        -- say 'line:' lines[i]
        line = maybeJoinLines(h)
        if line~dataType('O') then do
          okay = .false
          leave
        end
        -- say 'line-' line
        if i <> h~i then do
          .logger~lineout('  Joined line' i 'with the next' (h~i - i) 'line(s)')
          i = h~i
        end

        parenStart = line~caselessPos("self~assertSame(") + 15
        parenEnd   = findParenEnd(line, parenStart)
        if parenEnd == 0 then do
          okay = logBadParen(i)
          leave
        end

        line = swapArgs(2, line, parenStart, parenEnd, i)
      end

      when lines[i]~caselessPos("self~assertNotSame(") <> 0 then do
        h = .AssertHolder~new(lines, lines[i], i)
        -- say 'line:' lines[i]
        line = maybeJoinLines(h)
        if line~dataType('O') then do
          okay = .false
          leave
        end
        -- say 'line-' line
        if i <> h~i then do
          .logger~lineout('  Joined line' i 'with the next' (h~i - i) 'line(s)')
          i = h~i
        end

        parenStart = line~caselessPos("self~assertNotSame(") + 18
        parenEnd   = findParenEnd(line, parenStart)
        if parenEnd == 0 then do
          okay = logBadParen(i)
          leave
        end

        line = swapArgs(2, line, parenStart, parenEnd, i)
      end

      when lines[i]~caselessPos("self~assertTrue(") <> 0 then do
        h = .AssertHolder~new(lines, lines[i], i)
        -- say 'line:' lines[i]
        line = maybeJoinLines(h)
        if line~dataType('O') then do
          okay = .false
          leave
        end
        -- say 'line-' line
        if i <> h~i then do
          .logger~lineout('  Joined line' i 'with the next' (h~i - i) 'line(s)')
          i = h~i
        end

        parenStart = line~caselessPos("self~assertTrue(") + 15
        parenEnd   = findParenEnd(line, parenStart)
        if parenEnd == 0 then do
          okay = logBadParen(i)
          leave
        end

        line = swapArgs(1, line, parenStart, parenEnd, i)
      end

      when lines[i]~caselessPos("self~assertFalse(") <> 0 then do
        h = .AssertHolder~new(lines, lines[i], i)
        -- say 'line:' lines[i]
        line = maybeJoinLines(h)
        if line~dataType('O') then do
          okay = .false
          leave
        end
        -- say 'line-' line
        if i <> h~i then do
          .logger~lineout('  Joined line' i 'with the next' (h~i - i) 'line(s)')
          i = h~i
        end

        parenStart = line~caselessPos("self~assertFalse(") + 16
        parenEnd   = findParenEnd(line, parenStart)
        if parenEnd == 0 then do
          okay = logBadParen(i)
          leave
        end

        line = swapArgs(1, line, parenStart, parenEnd, i)
      end

      otherwise
        line = lines[i]

    end
    -- End select

    if line~datatype('O') then do
      okay = .false
      leave
    end

    fsObj~lineout(line)
  end

  if \ okay then .logger~lineout("  Error Error")

  fsObj~close

return okay

::routine logBadParen
  use arg i
  .logger~lineout("  Could not find closing paren!!  Working on line:" i)
  .logger~lineout("  #####################################")
return .false

-- Find the closing paren.  Assumes continued lines have been joined into 1
-- line.  If 0 is returned an error has happened.
::routine findParenEnd
  use arg line, startPos

  -- say 'line starting' line
  -- say 'at startPos' startPos

  lb = .MutableBuffer~new(line)
  sQuote = .queue~new -- single quote
  dQuote = .queue~new -- double quote
  paren  = .queue~new -- parenthesis
                      -- going to ignore embedded comments /* */
  endP = 0
  do i = (startPos) to line~length
    char = lb~subChar(i)
    select
      when char == '(' then do
        if notInQuotes(sQuote, dQuote) then paren~push('(')
      end

      when char == ')' then do
        if notInQuotes(sQuote, dQuote) then paren~pull
        if queuesEmpty(sQuote, dQuote, paren) then do
          -- This is it.
          endP = i
          leave
        end
      end

      when char == '"' then do
        if dQuote~isEmpty then dQuote~push('"'); else dQuote~pull
      end

      when char == "'" then do
        if sQuote~isEmpty then sQuote~push("'"); else sQuote~pull
      end
      otherwise NOP
    end
    -- End select
  end

  return endP

::routine notInQuotes
  use arg q1, q2
  return (q1~isEmpty & q2~isEmpty)

::routine queuesEmpty
  do i = 1 to arg()
    if \ arg(i)~isEmpty then return .false
  end
  return .true

::routine swapArgs
  use arg max, line, startP, endP, i

  parse var line pre =(startP + 1) fix =(endP) post

  /* For whatever obtuse reason, some of the asserts look like this:

       self~assertSame("subTest6",  0    , ABS(' + 0 '),  )
                       "subTest1",  12.3 , ABS('12.3')
  */
  if fix~strip('T')~right(1) == ',' then do
    .logger~lineout('  Odd assert:' fix)
    fix = fix~strip('T')~strip('T', ',')
    .logger~lineout('  Fix is now:' fix)
  end

  commas = findCommas(fix)

  --    self~assertSame("subTest1", centre('',0),'')

  --    self~assertTrue("Call beep loop ran 8 times", i == (8 + 1))

  select
    when max == 2 & commas~items == 2 then do
      c1 = commas~pull; c2 = commas~pull
      parse var fix msg =(c1) expected =(c2) actual
      msg = msg~strip('B')
      expected = expected~strip('L', ",")~strip('B')
      actual = actual~strip('L', ",")~strip('B')

      -- Remove simple subTestXXX messages
      if msg~caselessPos("subtest") <> 0, msg~words() < 3 then
        line = pre || expected || "," actual || post
      else
        line = pre || expected || "," actual || "," msg || post
    end

    when max == 2 & commas~items == 1 then do
      line = pre || fix || post
    end

    when max == 1 & commas~items == 1 then do
      c1 = commas~pull
      parse var fix msg =(c1) actual
      msg = msg~strip('B')
      actual = actual~strip('L', ",")~strip('B')

      -- Remove simple subTestXXX messages
      if msg~caselessPos("subtest") <> 0, msg~words() < 3 then
        line = pre || actual || post
      else
        line = pre || actual || "," msg || post
    end

    when max == 1 & commas~items == 0 then do
      line = pre || fix || post
    end

    otherwise do
      .logger~lineout("  Error parsing commas.  Max and comma count make no sense!!!")
      .logger~lineout("  Working on line" i "Max:" max "Comma count:" commas~items)
      .logger~lineout("  ##################################")
      line = .false
    end
  end
  -- End select

return line

::routine findCommas
  use arg fix

  -- say 'fix:' fix

  mb = .MutableBuffer~new(fix)
  commaPos = .queue~new

  sQuote = .queue~new -- single quote
  dQuote = .queue~new -- double quote
  paren  = .queue~new -- parenthesis
  brack  = .queue~new -- bracket
                      -- going to ignore embedded comments /* */

  do i = 1 to fix~length
    char = mb~subChar(i)
    -- say 'char:' char
    select
      when char == '(' then do
        if notInQuotes(sQuote, dQuote) then paren~push('(')
      end

      when char == ')' then do
        if notInQuotes(sQuote, dQuote) then paren~pull
      end

      when char == '[' then do
        if notInQuotes(sQuote, dQuote) then brack~push('[')
      end

      when char == ']' then do
        if notInQuotes(sQuote, dQuote) then brack~pull
      end

      when char == '"' then do
        if dQuote~isEmpty then dQuote~push('"'); else dQuote~pull
      end

      when char == "'" then do
        if sQuote~isEmpty then sQuote~push("'"); else sQuote~pull
      end

      when char == "," then do
        if queuesEmpty(sQuote, dQuote, paren, brack) then do
          -- Found one
          commaPos~queue(i)
        end
      end

      otherwise NOP
    end
    -- End select
  end

return commaPos

-- Brute force, not elegance
::routine maybeJoinLines
  use arg data
  -- say 'data~line:' data~line
  -- Return on the easy case, which is usally the case.
  if \ isContinuation(data~line) then return data~line

  j = data~i
  l = data~line
  a = data~lines
  do while isContinuation(l)
    l = l~strip('T')
    if l~right(1) == ',' then l = l~strip('T', ','); else l = l~strip('T', '-')

    j += 1
    if j > a~items then leave
    l = l a[j]
  end

  if j > a~items then do
    .logger~lineout("Error trying to joing continuation lines!!")
    .logger~lineout("  Started with line" data~i "and reached the end")
    .logger~lineout("  of the source line array without a good completion")
    .logger~lineout("  ########################################")
    return .false
  end

  data~i = j
  data~line = l

return l

::routine isContinuation
  use arg line
  char = line~strip('T')~right(1)

  select
    when char == ','  then return .true
    -- For whatever obtuse reason, some lines end in: --
    when char == '-' then do
      if line~strip('T')~right(2) == "--" then return .false; else return .true
    end
    otherwise return .false
  end
  -- End select


::routine findFiles
  use arg fileSpec

  .logger~lineout('Find files filespec:' fileSpec)

  f = .array~new
  j = SysFileTree(fileSpec, files., "FOS")
  do i = 1 to files.0
    f~append(files.i)
  end
  .logger~lineout("  Found" f~items 'files')
  .logger~lineout("")

return f


::class 'AssertHolder'
::attribute lines
::attribute line
::attribute i
::method init
  expose lines line i
  use arg lines, line, i
