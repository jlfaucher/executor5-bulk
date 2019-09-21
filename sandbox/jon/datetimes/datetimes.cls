/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2005-2009 Rexx Language Association. All rights reserved.    */
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


/* datetimes - usefull extensions to datetime                                */

/*
Element Civil Description
d         x   The one- or two-digit day (civil format)
dd            The two-digit day. Single-digit day values are preceded by a zero.
ddd       C   The three-character weekday abbreviation.
dddd      C   The full weekday name.
D     new x   The day followed by suffix - ie 9th
h         x   The one- or two-digit hour in 12-hour (civil) format.
hh            The two-digit hour in 12-hour format. Single-digit values are preceded by a zero.
H         x   The one- or two-digit hour in 24-hour (civil) format.
HH            The two-digit hour in 24-hour format. Single-digit values are preceded by a zero.
m         x   The one- or two-digit minute. (civil)
mm            The two-digit minute. Single-digit values are preceded by a zero.
M         x   The one- or two-digit month number. (civil)
MM            The two-digit month number. Single-digit values are preceded by a zero.
MMM       C   The three-character month abbreviation.
MMMM      C   The full month name.
q     new     The quarter number 1 - 4
s     new x   The one- or two-digit seconds (civil)
ss    new     The two-digit seconds
t         C   The one-letter AM/PM abbreviation (that is, AM is displayed as A.)
tt        C   The two-letter AM/PM abbreviation (that is, AM is displayed as AM.)
yy            The last two digits of the year (that is, 2019 would be displayed as 19.)
yyyy          The full year (that is, 2019 would be displayed as 2019.)
f...  new     Fraction of a second.  Precision by number of 'f's up to 6

      * Not present in DTP definition - added for this class
      x - a civil format element
      C - a character format element

      civil format elements (x) must be separated by a delimiter
      or character format element (C)

*/


/* ========================================================================= */
::class datetimes subclass datetime                                   public
/* ========================================================================= */

/* ========================================================================= */
/*                               class methods                               */
/* ========================================================================= */

::attribute formatChars          class                               private
::attribute dayNames             class                               private
::attribute weekStart      get   class
::attribute weekStart      set   class
expose weekStart
use arg Weekday

    select case Weekday~upper
       when 'SUN' then weekstart = 'Sun'
       when 'MON' then weekstart = 'Mon'
       otherwise RAISE syntax 88.916 array ("1", "Sun or Mon", period)
    end /* select */

/* ------------------------------------------------------------------------- */
::method init                    class
/* ------------------------------------------------------------------------- */
expose formatChars civilElements charElements monthAbbrevs monthNames         -
       dayNames weekStart

   formatChars   = 'd','D','f','h','H','m','M','q','s','t','y'
   civilElements = 'd','D','h','H','m','M','s'
   charElements  = 'ddd','dddd','MMM','MMMM','t','tt'
   monthAbbrevs  = 'JAN', 'FEB', 'MAR', 'APR', 'MAY', 'JUN', 'JUL', 'AUG', 'SEP', 'OCT', 'NOV', 'DEC'
   monthNames    = 'January', 'February', 'March', 'April', 'May', 'June' -
                 , 'July', 'August', 'September', 'October', 'November', 'December'
   dayNames      = 'MON','TUE','WED','THU','FRI','SAT','SUN'
   weekstart     = 'Mon'

/* ------------------------------------------------------------------------- */
::method from                    class
/* ------------------------------------------------------------------------- */
expose out h12? pm? formatChars format
use strict arg data, format, esc = '', offset=(time('O')/60000000)

   out  = .mutableBuffer~new(self~new~date~isoDate)
   h12? = .false
   pm?  = .false

-- break the format string & the data into inter-delimiter fragments
   fragment_start = 1
   stop           = format~length + 1

   do i = 1 to stop
      if i >= fragment_start  -- skip past escaped delimiters
      then do
         char     = format~substr(i,1)
         escaped? = (esc == char)
         if escaped? | \formatChars~hasItem(char) | i = stop
         then do
            format_fragment = format~substr(fragment_start, i - fragment_start)
            delimiter       = format~substr(i + escaped?,1)
            parse var data data_fragment (delimiter) data

            self~parseFormatFragment(data_fragment,format_fragment)

            fragment_start = i + 1 + escaped?
         end /* DO */
      end /* DO */
   end /* DO */

-- adjust for 12hr clock afternoons
   if h12? = .true & pm? = .true then out~overlay(out~substr(12,2) + 12, 12, 2)

RETURN self~fromIsoDate(out~string, offset)  -- pass to .datetime constructor

/* ------------------------------------------------------------------------- */
::method parseFormatFragment     class                               private
/* ------------------------------------------------------------------------- */
expose data_fragment format_fragment civil?
use arg data_fragment, format_fragment

-- say '{'format_fragment'}{'data_fragment'}'

   civil? = .false -- enforce rule that civil elements must be separated by delimiter or char data

-- each fragment consists of one or more elements
   do while format_fragment \= ''
      break = format_fragment~verify(format_fragment~left(1))           -- pos of 1st char different from 1st char
      if break = 0 then break = format_fragment~length + 1              -- last element
      parse var format_fragment format_element =(break) format_fragment -- break element off front of fragment
      self~parseFormatElement(format_element)                           -- parse the element
   end /* DO */


   if data_fragment \= ''
   then RAISE SYNTAX 88.900 array ('Invalid Format string or data: Format exhausted before data ['data_fragment']')

RETURN 0

/* ------------------------------------------------------------------------- */
::method parseFormatElement      class                               private
/* ------------------------------------------------------------------------- */
expose data_fragment format_fragment out civilElements monthAbbrevs monthNames -
       charElements pm? h12? civil? format
use arg format_element

   -- if this is a civil element (1 or 2 chars) prepend 0 if necesary
   if civilElements~hasItem(format_element)
   then do
--say '['format_element']['data_fragment']'
      if civil? = .true
      then RAISE SYNTAX 88.900 array ('Invalid Format string: Civil format elements ('civilElements~makestring('l',', ')') must be separated from each other by at least a delimiter or character element. Found ['format']')
      else civil? = .true

      firstChar = data_fragment~verify('1234567890')
      ff        = format_element||format_fragment
      if firstChar = 0
      then if ff~length = data_fragment~length
           then data_fragment = '0'||data_fragment
           else nop
      else do
      -- some later elements in the data may cause characters.
      -- We should have one more char of data then format chars before those elements
         do char_element over 'ddd', 'MMM', 't' until j = firstchar
            j = ff~pos(char_element)
            if j = firstChar then data_fragment = '0'||data_fragment
         end /* DO */
      end /* DO */
   end /* DO */
   else if charElements~hasItem(format_element)  -- char elements count as implicit delimiter for numerics
        then civil? = .false

-- set 12hr clock indicator if this is an Hour element
   select
      when format_element~abbrev('H') then h12? = .false
      when format_element~abbrev('h') then h12? = .true
      otherwise nop
   end /* select */


-- 2019-05-19T16:17:32.982000
-- 00000000011111111112222222
-- 12345678901234567890123456

  select case format_element
     when 'd'    then call inject 2,  9, 2
     when 'dd'   then call inject 2,  9, 2
     when 'h'    then call inject 2, 12, 2          -- 12hr
     when 'hh'   then call inject 2, 12, 2          -- 12hr
     when 'H'    then call inject 2, 12, 2          -- 24hr
     when 'HH'   then call inject 2, 12, 2          -- 24hr
     when 'm'    then call inject 2, 15, 2
     when 'mm'   then call inject 2, 15, 2
     when 'M'    then call inject 2,  6, 2
     when 'MM'   then call inject 2,  6, 2
     when 's'    then call inject 2, 18, 2
     when 'ss'   then call inject 2, 18, 2
     when 'yy'   then call inject 2,  1, 4, .datetime~new~standardDate~left(2)||data_fragment~left(2)
     when 'yyyy' then call inject 4,  1, 4
     when 'MMM'  then call inject 3,  6, 2, monthAbbrevs~index(data_fragment~left(3)~upper)~right(2,0)
     when 'ddd'  then call inject 3,  9, 0          -- this element is ignored
     when 'dddd' then parse caseless var data_fragment . 'DAY' data_fragment  -- this element is ignored
     when 'D'    then call inject dataFragment~strip('l',0)~verify('1234567890') + 1,  9, 2, data_fragment~strip('l',0)~left(dataFragment~verify('1234567890') - 1)
     when 'q'    then call inject 1,  0, 0          -- this element is ignored
     when 'MMMM' then do
                    do monthName over monthNames
                       if data_fragment~caselessAbbrev(monthName)
                       then do
                          call inject monthName~length, 6, 2, monthnames~index(monthName)~right(2,0)
                          LEAVE monthName
                       end /* DO */
                    end /* DO */
                 end /* DO */
     when 't'    then do
                      pm? = data_fragment~caselessAbbrev('p')
                      call inject 1,  0, 0
                 end /* DO */
     when 'tt'   then do
                      pm? = data_fragment~caselessAbbrev('p')
                      call inject 2,  0, 0
                 end /* DO */
     otherwise
        if data_element~verify('f') = 0
        then call inject data_element~length, 21, data_element~length
  end /* select */

RETURN 0

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
inject:
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
use arg how_much, targetPos, targetLength

   if how_much = 0 then RETURN 0

   if data_fragment~length < how_much
   then RAISE SYNTAX 88.900 array ('Invalid Format string or data: Data exhausted before parsing ['format_element']')

   parse var data_fragment datum =(how_much + 1) data_fragment

   if targetLength = 0 then RETURN 0

   if arg(4,'e') then datum = arg(4)

   out~overlay(datum, targetPos, targetLength)

RETURN 0

/* ------------------------------------------------------------------------- */
::method fromEuropean4Date class
/* ------------------------------------------------------------------------- */
/* european4dates are like dd/mm/yyyy                                        */
use arg euro4Date, separator = '/', offset=(time('O')/60000000)
RETURN self~fromStandardDate('7890645312'~translate(euro4Date,'1234567890'),separator, offset)

/* ------------------------------------------------------------------------- */
::method fromDateTime      class
/* ------------------------------------------------------------------------- */
use arg datetime
RETURN self~fromUTCIsoDate(dateTime~UTCIsoDate)

/* ========================================================================= */
/*                             Instance Methods                              */
/* ========================================================================= */
::attribute weekStart      get
::attribute weekStart      set
expose weekStart
use arg Weekday

    select case Weekday~upper
       when 'SUN' then weekstart = 'Sun'
       when 'MON' then weekstart = 'Mon'
       otherwise RAISE syntax 88.916 array ("1", "Sun or Mon", period)
    end /* select */

/* ------------------------------------------------------------------------- */
::method init
/* ------------------------------------------------------------------------- */
expose weekstart

   weekstart = self~class~weekstart  -- default can be recalibrated in class

   FORWARD class (super)

/* ------------------------------------------------------------------------- */
::method To
/* ------------------------------------------------------------------------- */
use strict arg format, esc = ''

   out = .mutableBuffer~new

   i = 1
   do until i > format~length
      char = format~substr(i,1)
      select
         when char == esc   -- following character is a delimiter
            then do
               out~append(format~substr(i+1,1))
               i += 2
            end /* DO */
         when \self~class~formatChars~hasItem(char)
            then do -- this char is a delimiter
               out~append(char)
               i += 1
            end /* DO */
         otherwise
         -- this element is the one or more repeated identical characters
            break = format~substr(i)~verify(char)
            if break = 0 -- this is the final element in the format string
            then element = format~substr(i)
            else element = format~substr(i,break - 1)
            select case element
               when 'd'    then out~append(self~day)
               when 'dd'   then out~append(self~day~right(2,0))
               when 'ddd'  then out~append(self~dayname~left(3))
               when 'dddd' then out~append(self~dayname)
               when 'D'    then out~append(self~day||self~suffix(self~day))
               when 'h'    then out~append(self~getPrefix(self~civilTime,':'))
               when 'hh'   then out~append(self~getPrefix(self~civilTime,':')~right(2,0))
               when 'H'    then out~append(self~normalTime~left(1)~strip('l',0)||self~normalTime~substr(2,1))
               when 'HH'   then out~append(self~normalTime~left(2))
               when 'm'    then out~append(self~normalTime~substr(4,1)~strip('l',0)||self~normalTime~substr(5,1))
               when 'mm'   then out~append(self~normalTime~substr(4,2))
               when 'M'    then out~append(self~month)
               when 'MM'   then out~append(self~month~right(2,0))
               when 'MMM'  then out~append(self~monthName~left(3))
               when 'MMMM' then out~append(self~monthName)
               when 'q'    then out~append(self~Quarter)
               when 's'    then out~append(self~normalTime~substr(7,1)~strip('l',0)||self~normalTime~substr(8,1))
               when 'ss'   then out~append(self~normalTime~substr(7,2))
               when 't'    then out~append(self~civilTime~right(2)~left(1))
               when 'tt'   then out~append(self~civilTime~right(2))
               when 'yy'   then out~append(self~year~right(2))
               when 'yyyy' then out~append(self~year)
               otherwise
                  if element~verify('f') = 0
                  then out~append(self~isoDate~substr(21,element~length))
            end /* select */
            i += element~length
      end /* select */
   end /* DO */

RETURN out~string

/* ------------------------------------------------------------------------- */
::method getPrefix                                                   private
/* ------------------------------------------------------------------------- */
use arg text, delim

   parse var text prefix (delim) .

RETURN prefix

/* ------------------------------------------------------------------------- */
::method suffix                                                      public
/* ------------------------------------------------------------------------- */
use arg dayNo

  select
     when dayNo > 3 & dayno < 21  then RETURN 'th'
     when dayNo~right(1) = 1      then RETURN 'st'
     when dayNo~right(1) = 2      then RETURN 'nd'
     when dayNo~right(1) = 3      then RETURN 'rd'
     otherwise                         RETURN 'th'
  end /* select */

RETURN 0
/* ------------------------------------------------------------------------- */


/* ------------------------------------------------------------------------- */
::method european4Date
/* ------------------------------------------------------------------------- */
/* european4Date are like dd/mm/yyyy                                         */
use arg separator = '/'
return '9086751234'~translate(self~standardDate(separator),'1234567890')

/* ------------------------------------------------------------------------- */
::method hhmmTime
/* ------------------------------------------------------------------------- */
/* hhmm times are like hhmm                                                  */
use arg separator = ''
return self~normalTime~left(5)~changeStr(':',separator)

/* ------------------------------------------------------------------------- */
::method mySQLDate
/* ------------------------------------------------------------------------- */
return self~isodate~ChangeStr('T',' ')~left(19)

/* ------------------------------------------------------------------------- */
::method startOf
/* ------------------------------------------------------------------------- */
expose weekstart
use arg period, offset = 0, timeoption = 'S'

   select
      when 'MInute'~caselessAbbrev(period,3)    then RETURN self~addMinutes(offset)~addSeconds(0 - self~seconds)~addMicroseconds(0 - self~microseconds)
      when 'Hour'~caselessAbbrev(period,1)      then RETURN self~addHours(offset)~AddMinutes(0 - self~minutes)~addSeconds(0 - self~seconds)~addMicroseconds(0 - self~microseconds)
      when 'Day'~caselessAbbrev(period,1)       then RETURN self~timeOption(self~addDays(offset),'S')
      when 'Week'~caselessAbbrev(period,1)      then RETURN self~timeOption(self~addDays(0 - (self~weekday + (6 * (weekstart = 'Mon'))) // 7)~addWeeks(offset), timeOption)
      when 'MOnth'~caselessAbbrev(period,2)
         then do
            dt = self~addDays((-1 * self~day) + 1)

            do i = offset to 0 by -1 while i > 0
               dt = dt~addDays(dt~daysInMonth)
            end /* DO */

            do i = offset to 0 by 1 while i < 0
               dt = dt~addDays(-1 * dt~addDays(-1)~daysInMonth)
            end /* DO */

            RETURN self~timeOption(dt,timeOption)
         end /* DO */
      when 'Quarter'~caselessAbbrev(period,1)   then RETURN self~timeOption(self~startOf('Month',((0 - (self~Month + 2)) // 3) + (offset * 3), 'R'), timeOption)
      when 'Year'~caselessabbrev(period,1)      then RETURN self~timeOption(self~addDays((-1 * self~yearDay) + 1)~addYears(offset), timeOption)
      when 'Decade'~caselessAbbrev(period,2)    then RETURN self~timeOption(.datetimes~new(addPeriod(self~year~left(3),offset),1,1),timeOption)
      when 'Century'~caselessAbbrev(period,1)   then RETURN self~timeOption(.datetimes~new(addPeriod(self~year~left(2),offSet),1,1),timeOption)
      when 'Millenium'~caselessAbbrev(period,3) then RETURN self~timeOption(.datetimes~new(addPeriod(self~year~left(1),offset),1,1),timeOption)
      otherwise RAISE syntax 88.916 array ("1 period", "Sun or MInute, Hour, Day, Week, MOnth, Quarter or Year", period)
   end /* select */

RETURN -1  -- < we never get here

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
addPeriod:
/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
use arg num, operand
numLength = num~length

   num += operand

   if num~length > numLength,
   |  num <= 0
   then RAISE syntax 88.900 array ('Datetimes offset' operand 'causes date overflow for StartOf' period)
RETURN num~right(numlength,0)~left(4,0)

/* ------------------------------------------------------------------------- */
::method endOf
/* ------------------------------------------------------------------------- */
use arg period, offset = 0, TimeOption = 'E'

   select
      when 'MINute'~caselessAbbrev(period,3)
         then RETURN self~startOf('MInute',offset)~addMinutes(1)~addMicroseconds(-1)
      when 'Hour'~caselessAbbrev(period,2)
         then RETURN self~startOf('Hour',offset)~addHours(1)~addMicroseconds(-1)
      when 'Day'~caselessAbbrev(period,2)
         then RETURN self~timeOption(self~date~addDays(offset),'E')
      when 'Week'~caselessAbbrev(period,1)
         then RETURN self~timeOption(self~startOf('Week',offset,'R')~addDays(6),timeOption)
      when 'MOnth'~caselessAbbrev(period,2)
         then RETURN self~timeOption(self~startof('Month',offset + 1, 'R')~addDays(-1),timeOption)
      when 'Quarter'~caselessAbbrev(period,1)
         then RETURN self~timeOption(self~startOf('Quarter',offset + 1, 'R')~addDays(-1), timeOption)
      when 'Year'~caselessAbbrev(period,1)
         then RETURN self~timeOption(self~startOf('Year',offset + 1, 'R')~addDays(-1), timeOption)
      when 'DEcade'~caselessAbbrev(period,2)
         then RETURN self~timeOption(self~startOf('DEcade',offset + 1, 'R') ~addDays(-1), timeOption)
      when 'Century'~caselessAbbrev(period,1)
         then RETURN self~timeOption(self~startOf('Century',offset + 1, 'R') ~addDays(-1), timeOption)
      when 'MILlenium'~caselessAbbrev(period,3)
         then RETURN self~timeOption(self~startOf('MILlenium',offset + 1, 'R') ~addDays(-1), timeOption)
      otherwise RAISE syntax 88.916 array ("1 'period'", "MInute, Hour, Day, Week, MOnth, Quarter, Year, DEcade, Century or MILlenium", period)
   end /* select */

/* ------------------------------------------------------------------------- */
::method Quarter
/* ------------------------------------------------------------------------- */

   RETURN ((self~month - 1) % 3) + 1

/* ------------------------------------------------------------------------- */
::method TimeOption                                                  Private
/* ------------------------------------------------------------------------- */
Use arg dt, opt = ''

   select
      When 'Respect'~caselessAbbrev(opt,1) then Return dt
      when 'Start'~caselessabbrev(opt,1)   then Return dt~date
      when 'End'~caselessabbrev(opt,1)     then Return dt~date~addDays(1)~addMicroseconds(-1)
      otherwise                                 Return dt
   end /* select */

/* ------------------------------------------------------------------------- */
::method toLocalTimeEU
/* ------------------------------------------------------------------------- */
/* performs a UTC toLocalTime taking into account changes of clock in EU     */
RETURN self~toLocalTime:super~addHours(self~EUSummerTime? - .datetimes~new~EUSummerTime?)

/* ------------------------------------------------------------------------- */
::method EUSummertime?
/* ------------------------------------------------------------------------- */
/* EUsummertime: 01:00 last Sunday of March to 01:00 last Sunday of October  */

   dt = self~nthWeekdayofMonth(-1,'Sun',,'start')~addHours(1)

   select
      when self~month < 3 | self~month > 10 then RETURN .false
      when self~month > 3 & self~month < 10 then RETURN .true
      when self~month = 3
         then   if self < dt
                then RETURN .false
                else RETURN .true
      otherwise if self < dt
                then RETURN .true
                else RETURN .false
   end /* select */

/* ------------------------------------------------------------------------- */
::method USSummertime?
/* ------------------------------------------------------------------------- */
/* USsummertime: 01:00 2nd Sunday of March to 02:00 1st Sunday of November   */

   dt = self~nthWeekdayofMonth(-1,'Sun',,'start')~addHours(1)

   select
      when self~month < 3 | self~month > 11 then RETURN .false
      when self~month > 3 & self~month < 11 then RETURN .true
      when self~month = 3
         then   if self < self~nthWeekdayofMonth(2,'Sun',,'start')~addHours(2)
                then RETURN .false
                else RETURN .true
      otherwise if self < self~nthWeekdayofMonth(1,'Sun',,'start')~addHours(2)
                then RETURN .true
                else RETURN .false
   end /* select */



/* ------------------------------------------------------------------------- */
::method nthWeekdayOfMonth
/* ------------------------------------------------------------------------- */
/* if n is negative date returned is nth weekday before end of month         */
use arg n = 1, weekday = 0, offset = 0, timeOption = '?'

   if \weekday~datatype('w'), self~class~dayNames~hasItem(weekday~upper~left(3))
   then weekday = self~class~dayNames~index(weekday~upper~left(3))
   if \weekday~datatype('w')                                              --=>.
   then RAISE syntax 88.917 array ('weekday','Weekday must be integer 1..7 or string starting with dayname from [mon,tue,wed,thu,fri,sat,sun] found ['weekday']')

   select
      when n > 0
         then do
            if timeoption = '?' then timeOption = 's'
            dt = self~startof('Month',offset,timeoption)
            origMonth = dt~month
            diff = weekday - dt~weekday
            if diff < 0 then diff += 7
            dt = dt~addDays(diff + ((n - 1) * 7))
         end /* DO */
      when n < 0
         then do
            if timeoption = '?' then timeOption = 'e'
            dt = self~Endof('Month',offset,timeoption)
            origMonth = dt~month
            diff = weekday - dt~weekday
            if diff > 0 then diff -= 7
            dt = dt~addDays(diff - ((n + 1) * 7))
         end /* DO */
      otherwise  RAISE syntax 88.917 array ('n', 'May not be 0')          --=>.
   end /* select */

   if dt~month \= origMonth
   then RAISE syntax 88.917 array ('n','Result out of scope of month')    --=>.

RETURN self~timeOption(dt,timeOption)

/* ========================================================================= */




