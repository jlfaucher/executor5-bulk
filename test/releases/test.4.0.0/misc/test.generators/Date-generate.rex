/*
use arg file
if file="" then file="ooRexx.Base.Date.testUnit"

call sysfiledelete file

g = .generator~new(.stream~new(file)~~open("output replace")
*/

/*
   name:             ooRexx.Base.Datee-generate.rex
   author:           Rick McGuire
   date:             2007-09-22
   purpose:          generate testUnit that tests the date()-BIF
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
   d~testUnit_name         ="ooRexx.Base.Date.testUnit"  -- determines filename and testUnit class name
   d~testUnit_languageLevel="6.02"
   d~testUnit_author       ="Rick McGuire"
   d~testUnit_version      ="1.0.0"
   d~testUnit_purpose      ="Test the Date() BIF"
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

g~generateTest("standard", "20070922", "S")
g~generateTest("standard_dash", "2007-09-22", "S", "-")
g~generateTest("standard_null", "20070922", "S", "")
g~generateTest("normal", "22 Sep 2007", "N")
g~generateTest("normal_dash", "22-Sep-2007", "N", "-")
g~generateTest("normal_null", "22Sep2007", "N", "")
g~generateTest("base", "732940", "B")
g~generateTest("european", "22/09/07", "E")
g~generateTest("european_dash", "22-09-07", "E", "-")
g~generateTest("european_null", "220907", "E", "")
g~generateTest("ordered", "07/09/22", "O")
g~generateTest("ordered_dash", "07-09-22", "O", "-")
g~generateTest("ordered_null", "070922", "O", "")
g~generateTest("usa", "09/22/07", "U")
g~generateTest("usa_dash", "09-22-07", "U", "-")
g~generateTest("usa_null", "092207", "U", "")

-- now test some fringe dates

g~generateTest("first_day", "00010101", "S")
g~generateTest("lastday", "99991231", "S")
g~generateTest("early_leap_year", "20040228", "S")
g~generateTest("late_leap_year", "20040301", "S")
g~generateTest("end_leap_year", "20041231", "S")
g~generateTest("early_non_leap_year", "20070228", "S")
g~generateTest("late_non_leap_year", "20070301", "S")
g~generateTest("end_non_leap_year", "20071231", "S")
g~generateTest("leap_day", "20040229", "S")
g~generateTest("century", "19000301", "S")
g~generateTest("olympiad", "20000229", "S")


::class generator
::method init
  expose out
  use arg out

::method lineout
  expose out
  use arg line
  out~lineout(line)

::method assert
  use arg label, test, value

  self~lineout('self~assertSame("'label'", 'test', "'value'")')


::method generateTest
use arg label, date, format, sep

self~lineout("")
self~lineout("::METHOD 'test_"label"'")    -- the test set

self~lineout("date = '"date"'")              -- easier to generate the test with a variable
self~lineout("format = '"format"'")          -- easier to generate the test with a variable

l='/ subtest #'
-- if no input separator, use the default
if arg() == 3 then do
    self~assert("base             " l  1, "date('B', date, format)"      , date('B', date, format))
    self~assert("days             " l  2, "date('D', date, format)"      , date('D', date, format))
    self~assert("european         " l  3, "date('E', date, format)"      , date('E', date, format))
    self~assert("european_no_sep  " l  4, "date('E', date, format, '')"  , date('E', date, format, ""))
    self~assert("european_dash_sep" l  5, "date('E', date, format, '-')" , date('E', date, format, "-"))
    self~assert("language         " l  6, "date('L', date, format)"      , date('L', date, format))
    self~assert("month            " l  7, "date('M', date, format)"      , date('M', date, format))
    self~assert("normal           " l  8, "date('N', date, format)"      , date('N', date, format))
    self~assert("normal_no_sep    " l  9, "date('N', date, format, '')"  , date('N', date, format, ""))
    self~assert("normal_dash_sep  " l 10, "date('N', date, format,'-')"  , date('N', date, format, "-"))
    self~assert("ordered          " l 11, "date('O', date, format)"      , date('O', date, format))
    self~assert("ordered_no_sep   " l 12, "date('O', date, format, '')"  , date('O', date, format, ""))
    self~assert("ordered_dash_sep " l 13, "date('O', date, format, '-')" , date('O', date, format, "-"))
    self~assert("standard         " l 14, "date('S', date, format)"      , date('S', date, format))
    self~assert("stardard_no_sep  " l 15, "date('S', date, format, '')"  , date('S', date, format, ""))
    self~assert("standard_dash_sep" l 16, "date('S', date, format, '-')" , date('S', date, format, "-"))
    self~assert("usa              " l 17, "date('U', date, format)"      , date('U', date, format))
    self~assert("usa_no_sep       " l 18, "date('U', date, format, '')"  , date('U', date, format, ""))
    self~assert("usa_dash_sep     " l 19, "date('U', date, format, '-')" , date('U', date, format, "-"))
    self~assert("weekday          " l 20, "date('W', date, format)"      , date('W', date, format))
end
-- explicit input format
else do
    self~lineout("sep = '"sep"'")

    self~assert("base             " l  1, "date('B', date, format,, sep)"     , date('B', date, format,, sep))
    self~assert("days             " l  2, "date('D', date, format,, sep)"     , date('D', date, format,, sep))
    self~assert("european         " l  3, "date('E', date, format,, sep)"     , date('E', date, format,, sep))
    self~assert("european_no_sep  " l  4, "date('E', date, format, '', sep)"  , date('E', date, format, "", sep))
    self~assert("european_dash_sep" l  5, "date('E', date, format, '-', sep)" , date('E', date, format, "-", sep))
    self~assert("language         " l  6, "date('L', date, format,, sep)"     , date('L', date, format,, sep))
    self~assert("month            " l  7, "date('M', date, format,, sep)"     , date('M', date, format,, sep))
    self~assert("normal           " l  8, "date('N', date, format,, sep)"     , date('N', date, format,, sep))
    self~assert("normal_no_sep    " l  9, "date('N', date, format, '', sep)"  , date('N', date, format, "", sep))
    self~assert("normal_dash_sep  " l 10, "date('N', date, format,'-', sep)"  , date('N', date, format, "-", sep))
    self~assert("ordered          " l 11, "date('O', date, format,, sep)"     , date('O', date, format,, sep))
    self~assert("ordered_no_sep   " l 12, "date('O', date, format, '', sep)"  , date('O', date, format, "", sep))
    self~assert("ordered_dash_sep " l 13, "date('O', date, format, '-', sep)" , date('O', date, format, "-", sep))
    self~assert("standard         " l 14, "date('S', date, format,, sep)"     , date('S', date, format,, sep))
    self~assert("stardard_no_sep  " l 15, "date('S', date, format, '', sep)"  , date('S', date, format, "", sep))
    self~assert("standard_dash_sep" l 16, "date('S', date, format, '-', sep)" , date('S', date, format, "-", sep))
    self~assert("usa              " l 17, "date('U', date, format,, sep)"     , date('U', date, format,, sep))
    self~assert("usa_no_sep       " l 18, "date('U', date, format, '', sep)"  , date('U', date, format, "", sep))
    self~assert("usa_dash_sep     " l 19, "date('U', date, format, '-', sep)" , date('U', date, format, "-", sep))
    self~assert("weekday          " l 20, "date('W', date, format,, sep)"     , date('W', date, format,, sep))
end





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

