/*
   name:             ooRexx.Base.Time-generate.rex
   author:           Rick McGuire
   date:             2007-09-22
   purpose:          generate testUnit that tests the time()-BIF
   language level:   6.02
   changed:          2007-09-23, Rony G. Flatscher: added testUnit template header handling
   license:          CPL 1.0 (see license text at the bottom of this program)
   remarks:          ---
*/
parse arg file       -- using PARSE would set 'file' to empty string "", wheras using
                     -- USE ARG would set 'file' to 'FILE'

if file="" then   -- no file given, use the template file and "testUnit_name" as filename
do
      -- up to 10 categories allowed
   arrCategory=.array~of("ooRexx", "base", "BIF")
      -- set up infos
   d=.directory~new     -- contains needles and replacement values
   d~testUnit_name         ="ooRexx.Base.Time.testUnit"  -- determines filename and testUnit class name
   d~testUnit_languageLevel="6.02"
   d~testUnit_author       ="Rick McGuire"
   d~testUnit_version      ="1.0.0"
   d~testUnit_purpose      ="Test the Time() BIF"
   d~testUnit_remark       ="---"
   d~testUnit_needs        ="---"
   d~testUnit_link         ="---"

      -- get and set run-date/time information
   parse value date("S",,,"-") date("S")~left(4) with d~testUnit_date d~testUnit_year

      -- now create header
   templateFileName="testUnit-Header.template"
   templ=.stream~new("testUnit-Header.template")~~open("read")
   headerData=templ~charin(1, templ~chars)
   templ~close

      -- replace placeholders
   do idx over d
      headerData=headerData~caselessChangeStr("%"idx"%", d[idx])
   end
      -- create category information
   tmpStr=""
   i=0
   do cat over arrCategory
      i=i+1
      if i>arrCategory~items then leave
      tmpStr=tmpStr || .endOfLine "  category"i":       " || arrCategory[i]
   end

   headerData=headerData~caselessChangeStr("%category_section%",tmpStr)

   fStream=.stream~new(d~testUnit_name)~~open("write replace")
   fStream~~say(headerData)~~say
end
else  -- use given fileName
do
   fStream=.stream~new(file)~~open("write replace")
end

/* ================================================================== */
/* ------------------------ generator part -------------------------- */
g = .generator~new(fStream)

-- first, different variations of formats and separators

g~generateTest("civil_morning"      , "4:47am"         , "C")
g~generateTest("civil_afternoon"    , "4:47pm"         , "C")
g~generateTest("hours_morning"      , "4"              , "H")
g~generateTest("hours_afternoon"    , "16"             , "H")
g~generateTest("long_morning"       , "06:47:49.384783", "L")
g~generateTest("long_afternoon"     , "16:47:49.387902", "L")
g~generateTest("minutes_morning"    , "250"            , "M")
g~generateTest("minutes_afternoon"  , "1012"           , "M")
g~generateTest("normal_morning"     , "06:47:49"       , "N")
g~generateTest("normal_afternoon"   , "16:47:49"       , "N")
g~generateTest("seconds_morning"    , "1517"           , "S")
g~generateTest("seconds_afternoon"  , "60801"          , "S")

-- now test some fringe times

g~generateTest("start_day"          , "00:00:00.000000", "L")
g~generateTest("past_start_day"     , "00:00:00.000001", "L")
g~generateTest("ten_morning"        , "10:00:00.000000", "L")
g~generateTest("almost_noon"        , "11:59:59.999999", "L")
g~generateTest("noon"               , "12:00:00.000000", "L")
g~generateTest("past_noon"          , "12:00:00.000001", "L")
g~generateTest("ten_night"          , "22:00:00.000000", "L")
g~generateTest("end_day"            , "23:59:59.999999", "L")

fStream~close


::class generator
::method init
  expose out
  use arg out

::method lineout
  expose out
  parse arg line
  out~lineout(line)

::method assert
  parse arg label, test, value

  self~lineout('self~assertSame("'label'", 'test', "'value'")')
  -- self~lineout('self~assertSame('pp(label)', 'test',' pp(value)')')
  return

  pp:procedure
    parse arg a
    return '"'a'"'~left(15)


::method generateTest
  parse arg label, time, format

  self~lineout("")
  self~lineout("::METHOD 'test_"label"'")    -- the test set

  self~lineout("time = '"time"'")            -- easier to generate the test with a variable
  self~lineout("format = '"format"'")        -- easier to generate the test with a variable

  l='/ subtest #'

-- if no input separator, use the default
  self~assert("civil" l 1, "time('C', time, format)", time('C', time, format))
  self~assert("hours" l 2, "time('H', time, format)", time('H', time, format))
  self~assert("long " l 3, "time('L', time, format)", time('L', time, format))
  self~assert("long " l 4, "time('M', time, format)", time('M', time, format))
  self~assert("long " l 5, "time('N', time, format)", time('N', time, format))
  self~assert("long " l 6, "time('S', time, format)", time('S', time, format))



/*----------------------------------------------------------------------------*/
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

