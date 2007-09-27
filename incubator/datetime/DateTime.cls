::CLASS 'DateTime' public inherit Comparable

-- return a DateTime object for today's date, and a time
-- of 00:00:0).000000
::METHOD today class
  use strict arg
  -- just grab the date in standard format and convert
  return self~fromStandardDate(date('s'))

-- Create a DateTime object from a Normal date string.  An optional
-- separator character can be specified.
::METHOD fromNormalDate class
  use strict arg date, sep = (" ")
  return self~new(date('F', date, 'N', , sep))


-- Create a DateTime object from a European date string.  An optional
-- separator character can be specified.
::METHOD fromEuropeanDate class
  use strict arg date, sep = ("/")
  return self~new(date('F', date, 'E', , sep))


-- Create a DateTime object from an Ordered date string.  An optional
-- separator character can be specified.
::METHOD fromOrderedDate class
  use strict arg date, sep = ("/")
  return self~new(date('F', date, 'E', , sep))


-- Create a DateTime object from a Standard date string.  An optional
-- separator character can be specified.
::METHOD fromStandardDate class
  use strict arg date, sep = ("")
  return self~new(date('F', date, 'S', , sep))


-- Create a DateTime object from a Usa date string.  An optional
-- separator character can be specified.
::METHOD fromUsaDate class
  use strict arg date, sep = ("/")
  return self~new(date('F', date, 'U', , sep))


-- Create a DateTime object from a Normal time string.
::METHOD fromNormalTime class
  use strict arg time
  return self~new(time('F', time, 'N'))


-- Create a DateTime object from a Civil time string.
::METHOD fromCivilTime class
  use strict arg time
  return self~new(time('F', time, 'C'))


-- Create a DateTime object from a Iso date/time string.
::METHOD fromIsoDate class
  use strict arg time
  parse var time with date 'T' time
  numeric digits 18
  return self~new(date('F', date, 'S',,"-") + time("F", time, "L"))


-- Create a DateTime object from Unix Ticks value
::METHOD fromTicks class
  use strict arg date
  return self~new(date('F', date, 'T'))


-- Create a DateTime object from a basedate value
::METHOD fromBaseDate class
  use strict arg date
  return self~new(date('F', date, 'B'))


-- initialize a DateTime instance.  There are multiple forms of this:
--
-- init(fulltime)
--      input is a full time number such as returned from date('f')
-- init(year, month, day)
--      initialize from a year/month/day value.  The time will be set to
--      00:00:00.000000 of the current date
-- init(year, month, day, hours, minutes, seconds, microseconds)
--      initialize from a fully specified time specification.  The microseconds
--      are optional and default to zero.
::METHOD init
  expose timestamp

  numeric digits 18    -- need higher digits for time calculations

  select
      when arg() == 0 then do
          timestamp = date('Full')    -- no arguments, just use the current time
      end
      when arg() == 1 then do    -- just create from a microseconds value
          use strict arg timestamp
          -- seems silly, but this validates the time
          timestamp = date('f', timestamp, 'f')
      end
      when arg() <= 3 then do
          -- year month day to microseconds value
          use strict arg year, month, day

          -- format into a convertable date
          timestamp = self~convertDate(year, month, day)
      end
      -- only other format is everything, and only the microseconds are optional.
      otherwise
          -- starting with year, month, day, hour, minutes, seconds
          use strict arg year, month, day, hour, minutes, seconds, usecs = 0

          timestamp = self~convertDate(year, month, day) + self~convertTime(hours, minutes, seconds, usecs)
  end

-- private utility method to get a full date value from year/month/day.
::METHOD convertDate private
  use arg year, month, day

  date = year~right(4, '0')month~right(2, '0')day~right(2, '0')
  return date('F', date, 'S')

-- private utility method to get a full date value from an hour/minute/second/microseconds set.
::METHOD convertTime private
  use arg hours, minutes, seconds, usecs = 0

  time = hours~right(2, '0')':'minutes~right(2, '0')':'seconds~right(2, '0')'.'usecs~right(6, '0')
  return time('f', time, 'l')


-- perform a sort comparison of two timespans
::METHOD compareTo
  expose timestamp

  use strict arg other

  if \other~isa(.datetime) then do
      raise syntax 93.948 array(1, "TimeSpan")
  end

  othertime = other~fullDate

  -- just pick up the sign value of the subtraction
  return (timestamp - othertime)~sign

-- return the year value of the datetime.
::METHOD year
  expose timestamp
  return date('Standard', timestamp, 'Full')~substr(1, 4)

-- return the month value of the datetime
::METHOD month
  expose timestamp
  -- NB make sure the leading zeros are stripped
  return date('Standard', timestamp, 'Full')~substr(5, 2) + 0

-- return the day value of the datetime
::METHOD day
  expose timestamp
  -- NB make sure the leading zeros are stripped
  return date('Standard', timestamp, 'Full')~substr(7, 2) + 0

-- return the hours value of the datetime
::METHOD hours
  expose timestamp
  return time('Hours', timestamp, 'Full')

-- return the minutes value of the datetime
::METHOD minutes
  expose timestamp
  return time('Minutes', timestamp, 'Full') // 60

-- return the seconds value of the datetime
::METHOD seconds
  expose timestamp
  return time('Seconds', timestamp, 'Full') // 60

-- return the microseconds value of the datetime
::METHOD microseconds
  expose timestamp

  parse value time('Long', timestamp, 'Full')  with '.' microseconds
  return microseconds + 0

-- return the number of minutes since 00:00:00.000000
::METHOD dayMinutes
  expose timestamp
  return time('Minutes', timestamp, 'Full')

-- return the number of seconds since 00:00:00.000000
::METHOD daySeconds
  expose timestamp
  return time('Seconds', timestamp, 'Full')

-- return the number of microseconds since 00:00:00.000000
::METHOD dayMicroseconds
  expose timestamp
  numeric digits 18

  return self~daySeconds * 1000000 + self~microseconds

-- hashcode override for MapCollection interaction.
::METHOD hashCode
  expose timestamp
  return timestamp~hashcode

-- compare two DateTime instances
::METHOD "=="
  use strict arg other

  if \other~isa(self~class) then do
      return .false
  end

  return self~compareTo(other) == 0


-- compare two DateTime instances
::METHOD "\=="
  use strict arg other

  if \other~isa(self~class) then do
      return .true
  end

  return self~compareTo(other) \== 0

::METHOD "="
  -- this is equivalent of "=="
  forward message("==")

::METHOD "\="
  -- this is equivalent of "\=="
  forward message("\==")

::METHOD "<>"
  -- this is equivalent of "\=="
  forward message("\==")

::METHOD "><"
  -- this is equivalent of "\=="
  forward message("\==")

::METHOD "<"
  use strict arg other
  return self~compareTo(other) < 0

::METHOD "<="
  use strict arg other
  return self~compareTo(other) <= 0

::METHOD "\>"
  use strict arg other
  return self~compareTo(other) <= 0

::METHOD "<<"
  use strict arg other
  return self~compareTo(other) < 0

::METHOD "<<="
  use strict arg other
  return self~compareTo(other) <= 0

::METHOD "\>>"
  use strict arg other
  return self~compareTo(other) <= 0

::METHOD ">"
  use strict arg other
  return self~compareTo(other) > 0

::METHOD ">="
  use strict arg other
  return self~compareTo(other) >= 0

::METHOD "\<"
  use strict arg other
  return self~compareTo(other) >= 0

::METHOD ">>"
  use strict arg other
  return self~compareTo(other) > 0

::METHOD ">>="
  use strict arg other
  return self~compareTo(other) >= 0

::METHOD "\<<"
  use strict arg other
  return self~compareTo(other) >= 0


-- perform subtraction between two datetime objects or a datetime
-- object and a timespan object.  The original object is left unchanged
-- and new instance is returned.
--
-- Prefix minus has no meaning, and will result in an error.
::METHOD "-"
  expose timestamp

  numeric digits 18     -- need this to do math on the value

  -- could be a prefix "-", but we'll treat it as dyadic

  -- actual subtraction op
  use strict arg other

  -- "-" is not commutative with .DateTime.  We don't support
  -- subtracting a time from a TimeStamp.  The reverse operation,
  -- however, makes sense.
  if other~isa(.timespan) then do
      -- subtract the timespan from this class
      return self~class~new(timestamp - other~totalMicroseconds)
  end
  -- subtracting two dates
  else if other~isa(.datetime) then do
      -- perform the math using the big numbers...the result may be negative
      return .timespan~new(timestamp - other~fullDate)
  end

  raise syntax 93.948 array(1, "DateTime or TimeSpan")


-- add a datetime instance to a timespan instance, returning a new datetime.
::METHOD "+"
  expose timestamp

  numeric digits 18     -- need this to do math on the value

  -- could be a prefix "+"
  if arg() == 0 then do
      -- this is pretty meaningless, but less so than
      -- prefix -.  We'll just make this a noop and return a copy of ourselves
      return self~class~new(timestamp)
  end

  -- ok, actually addition.  We require a timespan.  Trying to add
  -- 2 dates is pretty meaningless
  use strict arg other

  -- "+" can do the addition to either a timespan or a datetime.
  -- When addition occurs between two timespaces, the result is a timespan.
  -- When adding to a datatime, the result is a datetime.
  if other~isa(.timespan) then do
      -- combine the two intervals
      return self~class~new(timestamp + other~totalMicroseconds)
  end

  raise syntax 93.948 array(1, "TimeSpan")


-- Add a number of years to a datetime object, returning a new object
-- instance.  Leap years are accounted for, and if the resulting date
-- will be February 29th in a non-leap year, the day is rolled back to the
-- 28th.
::METHOD addYears
  expose timestamp
  use strict arg years

  -- This is easier to deal with if we split the date and time into
  -- two components first
  basedate = self~date
  dayoffset = self~timeofday

  year = basedate~year
  month = basedate~month
  day = basedate~day

  year += years

  -- if we're on Feb 29th, then we have a leap day problem to deal with
  -- we need to check to see if the resulting year will be a leap year
  -- or not.  If it is, we're fine, otherwise we have to step back to the
  -- 28th.
  if month = 2 & day = 29 then do
      -- test for the leap year on the new target
      if year // 4 == 0 & ((year // 100 \= 0) | (year // 400 == 0))  then nop
      else do
          day -= 1
      end
  end

  -- now adjust that component by the corresponding years
  newDate = self~class~new(year, month, day)

  -- and finally recombine with the time to get the adjusted timestamp
  return newDate + dayoffset


-- Add a number of days to a datetime, returning a new datetime instance.
::METHOD addDays
  expose timestamp
  use strict arg days

  numeric digits 18
  -- return a new instance of DateTime
  return self~class~new(trunc(timestamp + days * (24 * 3600 * 1000000)))


-- Add a number of hours to a datetime, returning a new datetime instance.
::METHOD addHours
  expose timestamp
  use strict arg hours

  numeric digits 18

  -- return a new instance of DateTime.
  -- NB, we trunk the value, as it's possible to add non-whole numbers of
  -- these units.
  return self~class~new(trunc(timestamp + (hours * (3600 * 1000000))))


-- Add a number of minutes to a datetime, returning a new datetime instance.
::METHOD addMinutes
  expose timestamp
  use strict arg minutes

  numeric digits 18

  -- return a new instance of DateTime
  -- NB, we trunk the value, as it's possible to add non-whole numbers of
  -- these units.
  return self~class~new(trunc(timestamp + (minutes * (60 * 1000000))))


-- Add a number of seconds to a datetime, returning a new datetime instance.
::METHOD addSeconds
  expose timestamp
  use strict arg seconds

  numeric digits 18

  -- return a new instance of DateTime
  -- NB, we trunk the value, as it's possible to add non-whole numbers of
  -- these units.
  return self~class~new(trunc(timestamp + (seconds * 1000000)))


-- Add a number of microseconds to a datetime, returning a new datetime instance.
::METHOD addMicroseconds
  expose timestamp
  use strict arg usecs

  numeric digits 18

  -- return a new instance of DateTime
  -- NB, we trunk the value, as it's possible to add non-whole numbers of
  -- these units.
  return self~class~new(trunc(timestamp + usecs))


-- Format the datetime into ISO format, yyyy-mm-ddT00:00:00.000000
::METHOD isoDate
  expose timestamp
  use strict arg
  return date("S",timestamp,"F","-")"T"time("L", timestamp, "F")

-- Return the basedate for this datetime instance.
::METHOD baseDate
  expose timestamp
  use strict arg
  return date("B", timestamp, "F")

-- Return the day number within the year.
::METHOD yearDay
  expose timestamp
  use strict arg
  return date("D", timestamp, "F");

-- Format the datetime into European format, dd/mm/yy.  An optional
-- separator may be specified.
::METHOD europeanDate
  expose timestamp
  use strict arg sep = ("/")
  return date("E", timestamp, "F", sep);

-- Format the datetime into the Language format.
::METHOD languageDate
  expose timestamp
  use strict arg
  return date("L", timestamp, "F");

-- Return the name of the current month, in English
::METHOD monthName
  expose timestamp
  use strict arg
  return date("M", timestamp, "F");

-- Return the day of the week, in English
::METHOD dayName
  expose timestamp
  use strict arg
  return date("W", timestamp, "F");


-- Format the datetime into European format, 1 Sep 2007.  An optional
-- separator may be specified.
::METHOD normalDate
  expose timestamp
  use strict arg sep = (" ")
  return date("N", timestamp, "F", sep);


-- Format the datetime into Ordered format, yy/mm/dd.  An optional
-- separator may be specified.
::METHOD orderedDate
  expose timestamp
  use strict arg sep = ("/")
  return date("O", timestamp, "F", sep);


-- Format the datetime into Standard format, yyyymmdd.  An optional
-- separator may be specified.
::METHOD standardDate
  expose timestamp
  use strict arg sep = ("")
  return date("S", timestamp, "F", sep);

-- Format the datetime into USA format, mm/dd/yy.  An optional
-- separator may be specified.
::METHOD usaDate
  expose timestamp
  use strict arg sep = ("/")
  return date("U", timestamp, "F", sep);

-- Return the day of the week as an integer.  0 = Monday,
-- 1 - Tuesday, etc.
::METHOD weekDay
  expose timestamp
  use strict arg
  return date("B", timestamp, "F") // 7

-- Format the datetime into Civil format, hh:mmam/pm.
::METHOD civilTime
  expose timestamp
  use strict arg
  return time("C", timestamp, "F")


-- Format the datetime into Normal time format, hh:mm:ss
::METHOD normalTime
  expose timestamp
  use strict arg
  return time("N", timestamp, "F")


-- Format the datetime into Long time format, hh:mm:ss.uuuuuu
::METHOD longTime
  expose timestamp
  use strict arg
  return time("L", timestamp, "F")

-- Return the full date timestamp value for this date.
::METHOD fullDate
  expose timestamp
  return timestamp

-- Return the Unix Ticks value for this timestamp.
::METHOD ticks
  expose timestamp
  use strict arg
  return time("T", timestamp, "F")

-- Returns true if the datetime object year is a leap year.
::METHOD isLeapYear
  use strict arg
  year = self~year

  return year // 4 == 0 & ((year // 100 \= 0) | (year // 400 == 0))

-- Returns the number of days in the current month.  This accounts
-- for leap years.
::METHOD daysInMonth
  use strict arg
  month = self~month

  if self~isLeapYear then do
      return "31 29 31 30 31 30 31 31 30 31 30 31"~word(month)
  end
  else do
      return "31 28 31 30 31 30 31 31 30 31 30 31"~word(month)
  end

-- Returns the time of day as a TimeSpan object with the interval
-- from 00:00:00.000000 of the datetime day.
::METHOD timeOfDay
  use strict arg
  return .timespan~new(time('F', self~LongTime, 'L'))

-- Returns a datetime object with a time of day value set to
-- 00:00:00.000000
::METHOD date
  use strict arg
  return self~class~new(date('F', self~standardDate, 'S'))

-- Returns a string form for this date time.  The string value
-- is the ISO date.
::METHOD string      -- return a formatted string value
  return self~isoDate


-- TimeSpan represents an interval of time.  Timespans are created by subtracting
-- two DateTime objects.  They also can added to DateTime objects.
::CLASS 'TimeSpan' public inherit Comparable

-- Create a timespan object representing a number of days
::METHOD fromDays class
  use strict arg days
  return self~new(days, 0, 0, 0, 0)

-- Create a timespan object representing a number of hours
::METHOD fromHours class
  use strict arg hours
  return self~new(0, hours, 0, 0, 0)

-- Create a timespan object representing a number of minutes
::METHOD fromMinutes class
  use strict arg minutes
  return self~new(0, 0, minutes, 0, 0)

-- Create a timespan object representing a number of seconds
::METHOD fromSeconds class
  use strict arg seconds
  return self~new(0, 0, 0, seconds, 0)

-- Create a timespan object representing a number of microseconds
::METHOD fromMicroseconds class
  use strict arg usec
  return self~new(usec)

-- Initialize a timespan object
-- initialize a TimeSpan instance.  There are multiple forms of this:
--
-- init(microseconds)
--      input is a full time number of microseconds from time('f')
-- init(hours, minutes, seconds)
--      initialize from an hours, minutes, seconds combo.
-- init(days, hours, minutes, seconds, microseconds)
--      initialize from a fully specified time specification.  The microseconds
--      are optional and default to zero.
::METHOD init
  expose timestamp

  numeric digits 20    -- need higher digits for time calculations

  select
      when arg() <= 1 then do    -- just create from a microseconds value
          use strict arg timestamp
      end
      when arg() <= 3 then do
          -- convert hours, minutes, seconds to microseconds value
          use strict arg hours, minutes, seconds
          timestamp = ((hours * 3600) + (minutes * 60) + seconds) * 1000000
      end
      otherwise
          -- starting with days, hours, minutes, seconds, and microseconds
          use strict arg days, hours, minutes, seconds, microseconds = 0
          timestamp = (((days * 24 * 3600) + (hours * 3600) + (minutes * 60) + seconds) * 1000000) + microseconds
  end
  -- this will validate the timestamp value
  timestamp = date('f', abs(timestamp), 'f') * sign(timestamp)


-- perform a sort comparison of two timespans
::METHOD compareTo
  expose timestamp

  use strict arg other

  if \other~isa(.timespan) then do
      raise syntax 93.948 array(1, "TimeSpan")
  end

  othertime = other~totalMicroseconds

  -- now produce a compare result based on the relative values.
  -- NOTE:  the timestamp values can be negative, so just subtraction won't work for
  -- a relative size test.
  if timestamp < othertime then do
      return -1
  end
  else if timestamp > othertime then do
      return 1
  end

  return 0


-- return a timespan instance as a positive value
::METHOD duration
  expose timestamp
  return self~class~new(abs(timestamp))

-- return the number of whole days in the timespan
::METHOD days
  expose timestamp
  numeric digits 18
  return timestamp % (24 * 3600 * 1000000)


-- return the hours component of the timespan
::METHOD hours
  expose timestamp

  numeric digits 18
  -- subtract out the higher components
  remainder = timestamp // (24 * 3600 * 1000000)
  -- and return the number full units remaining
  return remainder % (3600 * 1000000)


-- return the minutes component of the timespan
::METHOD minutes
  expose timestamp

  numeric digits 18
  -- subtract out the higher components
  remainder = timestamp // (3600 * 1000000)
  -- and return the number full units remaining
  return remainder % (60 * 1000000)


-- return the seconds component of the timespan
::METHOD seconds
  expose timestamp

  numeric digits 18
  -- subtract out the higher components
  remainder = timestamp // (60 * 1000000)
  -- and return the number full units remaining
  return remainder % (1000000)

-- return the microseconds component of the timespan
::METHOD microseconds
  expose timestamp

  numeric digits 18
  -- subtract out the higher components
  remainder = timestamp // 1000000
  -- and return the number full units remaining
  return remainder


-- return the timespan as a number of days (including fractional days)
::METHOD totalDays
  expose timestamp
  numeric digits 18
  return timestamp / (24 * 3600 * 1000000)


-- return the timespan as a number of hours (including fractional hours)
::METHOD totalHours
  expose timestamp

  numeric digits 18
  return timestamp / (3600 * 1000000)


-- return the timespan as a number of minutes (including fractional minutes)
::METHOD totalMinutes
  expose timestamp

  numeric digits 18
  return timestamp / (60 * 1000000)


-- return the timespan as a number of seconds (including fractional seconds)
::METHOD totalSeconds
  expose timestamp

  numeric digits 18
  return timestamp / (1000000)


-- return the timespan as a number of microseconds
::METHOD totalMicroseconds
  expose timestamp
  return timestamp

-- hashcode override for MapCollection interaction.
::METHOD hashCode
  expose timestamp
  return timestamp~hashcode

-- comparison methods for TimeSpan instances
::METHOD "=="
  use strict arg other

  if \other~isa(self~class) then do
      return .false
  end

  return self~compareTo(other) == 0


::METHOD "\=="
  use strict arg other

  if \other~isa(self~class) then do
      return .true
  end

  return self~compareTo(other) \== 0

::METHOD "="
  -- this is equivalent of "=="
  forward message("==")

::METHOD "\="
  -- this is equivalent of "\=="
  forward message("\==")

::METHOD "<>"
  -- this is equivalent of "\=="
  forward message("\==")

::METHOD "><"
  -- this is equivalent of "\=="
  forward message("\==")

::METHOD "<"
  use strict arg other
  return self~compareTo(other) < 0

::METHOD "<="
  use strict arg other
  return self~compareTo(other) <= 0

::METHOD "\>"
  use strict arg other
  return self~compareTo(other) <= 0

::METHOD "<<"
  use strict arg other
  return self~compareTo(other) < 0

::METHOD "<<="
  use strict arg other
  return self~compareTo(other) <= 0

::METHOD "\>>"
  use strict arg other
  return self~compareTo(other) <= 0

::METHOD ">"
  use strict arg other
  return self~compareTo(other) > 0

::METHOD ">="
  use strict arg other
  return self~compareTo(other) >= 0

::METHOD "\<"
  use strict arg other
  return self~compareTo(other) >= 0

::METHOD ">>"
  use strict arg other
  return self~compareTo(other) > 0

::METHOD ">>="
  use strict arg other
  return self~compareTo(other) >= 0

::METHOD "\<<"
  use strict arg other
  return self~compareTo(other) >= 0


-- perform timespan subtraction.  If this is a prefix "-",
-- then the timespan is just negated.  Otherwise, the other
-- argument must be a timespan.  The return result is a new
-- timespan representing the difference between the two values.
::METHOD "-"
  expose timestamp

  numeric digits 18     -- need this to do math on the value

  -- could be a prefix "-"
  if arg() == 0 then do
      return self~class~new(-timestamp)
  end

  -- actual subtraction op
  use strict arg other

  -- "-" is not commutative with .DateTime.  We don't support
  -- subtracting a time from a TimeStamp.  The reverse operation,
  -- however, makes sense.
  if !other~isa(.timespan) then do
      raise syntax 93.948 array(1, "TimeSpan")
  end

  -- combine the two intervals
  return self~class~new(timestamp - other~totalMicroseconds)


-- perform timespan addition.  If this is the prefix "+" operator,
-- a new timespan instance with the same value is returned.  Otherwise,
-- if the argument is a TimeSpan object, the two timespans are added together,
-- returning a new TimeSpan.  If the argument object is a DateTime, the TimeSpan
-- is added to the DateTime and an new, adjusted DateTime object is returned.
::METHOD "+"
  expose timestamp

  numeric digits 18     -- need this to do math on the value

  -- could be a prefix "+"
  if arg() == 0 then do
      return self~class~new(timestamp)
  end

  -- ok, actually addition.  We require
  use strict arg other

  -- "+" can do the addition to either a timespan or a datetime.
  -- When addition occurs between two timespaces, the result is a timespan.
  -- When adding to a datatime, the result is a datetime.
  if other~isa(.timespan) then do
      -- combine the two intervals
      return self~class~new(timestamp + other~totalMicroseconds)
  end

  if other~isa(.datetime) then do
      -- combine the interval with the timestamp
      return other~class~new(timestamp + other~fullDate)
  end

  raise syntax 93.948 array(1, "TimeSpan or DateTime")


-- perform integer division on a timespan.  This divides the interval
-- and returns a new TimeSpan instance.
::METHOD "%"
  expose timestamp

  numeric digits 18     -- need this to do math on the value

  -- we just get a divisor and use it.  If it's bad, it's bad
  use strict arg divisor

  -- do the division and create a new interval
  return self~class~new(timestamp % divisor)

-- perform division on a timespan.  This divides the interval
-- and returns a new TimeSpan instance.  This is the same as integer division.
::METHOD "/"
  -- dividing a timespan is inherently an integer operation.  Handle as
  -- integer division.
  forward message("%")


-- perform remainder division on a timespan.  This divides the interval
-- and returns a new TimeSpan instance with the remainder.
::METHOD "//"
  expose timestamp

  numeric digits 18     -- need this to do math on the value

  -- we just get a divisor and use it.  If it's bad, it's bad
  use strict arg divisor


  -- do the remainder and create a new interval
  return self~class~new(timestamp // divisor)


-- perform multiplication on a timespan.  This multiplies the interval
-- and returns a new TimeSpan instance.
::METHOD "*"
  expose timestamp

  numeric digits 18     -- need this to do math on the value

  -- we just get a multiplier and use it.  If it's bad, it's bad
  use strict arg multiplier

  -- do the multiply and create a new interval
  return self~class~new(trunc(timestamp * multiplier))


-- Add a number of days to a timespan, returning a new timespan instance.
::METHOD addDays
  expose timestamp
  use strict arg days

  numeric digits 18
  -- return a new instance of TimeSpan
  return self~class~new(trunc(timestamp + (days * (24 * 3600 * 1000000))))


-- Add a number of hours to a timespan, returning a new timespan instance.
::METHOD addHours
  expose timestamp
  use strict arg hours

  numeric digits 18

  -- return a new instance of TimeSpan
  return self~class~new(trunc(timestamp + (hours * (3600 * 1000000))))


-- Add a number of minutes to a timespan, returning a new timespan instance.
::METHOD addMinutes
  expose timestamp
  use strict arg minutes

  numeric digits 18

  -- return a new instance of TimeSpan
  return self~class~new(trunc(timestamp + (minutes * (60 * 1000000))))


-- Add a number of seconds to a timespan, returning a new timespan instance.
::METHOD addSeconds
  expose timestamp
  use strict arg seconds

  numeric digits 18

  -- return a new instance of TimeSpan
  return self~class~new(trunc(timestamp + (seconds * 1000000)))


-- Add a number of microseconds to a timespan, returning a new timespan instance.
::METHOD addMicroseconds
  expose timestamp
  use strict arg usecs

  numeric digits 18

  -- return a new instance of TimeSpan
  return self~class~new(trunc(timestamp + usecs))


-- format the TimeSpan into string form.  The format is
-- (-)(ddddddd.)hh:mm:ss.uuuuuu  The portions in parentheses
-- are optional.
::METHOD string      -- return a formatted string value
  expose timestamp
  use strict arg

  days = abs(self~days)
  hours = abs(self~hours)
  minutes = abs(self~minutes)
  seconds = abs(self~seconds)
  microseconds = abs(self~microseconds)

  sign = ""

  if timestamp < 0 then do
      sign = "-"
  end

  -- only include the days portion if it's non-zero.
  if days > 0 then do
      return sign||days"."hours~right(2, '0')":"minutes~right(2, '0')":"seconds~right(2, '0')"."microseconds~right(6, '0')
  end
  else do
      return sign||hours~right(2, '0')":"minutes~right(2, '0')":"seconds~right(2, '0')"."microseconds~right(6, '0')
  end
