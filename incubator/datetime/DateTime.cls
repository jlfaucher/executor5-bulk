::CLASS 'DateTime' public inherit Comparable

::METHOD today class
  use strict arg
  -- just grab the date in standard format and convert
  return self~fromStandardDate(date('s'))

::METHOD fromNormalDate class
  use strict arg date, sep = (" ")
  return self~new(date('F', date, 'N', , sep))

::METHOD fromEuropeanDate class
  use strict arg date, sep = ("/")
  return self~new(date('F', date, 'E', , sep))

::METHOD fromOrderedDate class
  use strict arg date, sep = ("/")
  return self~new(date('F', date, 'E', , sep))

::METHOD fromStandardDate class
  use strict arg date, sep = ("")
  return self~new(date('F', date, 'S', , sep))

::METHOD fromUsaDate class
  use strict arg date, sep = ("/")
  return self~new(date('F', date, 'U', , sep))

::METHOD fromNormalTime class
  use strict arg time
  return self~new(time('F', time, 'N'))

::METHOD fromCivilTime class
  use strict arg time
  return self~new(time('F', time, 'C'))

::METHOD fromIsoDate class
  use strict arg time
  parse var time with date 'T' time
  numeric digits 18
  return self~new(date('F', date, 'S',,"-") + time("F", time, "L"))

::METHOD fromTicks class
  use strict arg date
  return self~new(date('F', date, 'T'))

::METHOD fromBaseDate class
  use strict arg date
  return self~new(date('F', date, 'B'))



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

::METHOD convertDate private
  use arg year, month, day

  date = year~right(4, '0')month~right(2, '0')day~right(2, '0')
  return date('F', date, 'S')

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

::METHOD year
  expose timestamp
  return date('Standard', timestamp, 'Full')~substr(1, 4)

::METHOD month
  expose timestamp
  -- NB make sure the leading zeros are stripped
  return date('Standard', timestamp, 'Full')~substr(5, 2) + 0

::METHOD day
  expose timestamp
  -- NB make sure the leading zeros are stripped
  return date('Standard', timestamp, 'Full')~substr(7, 2) + 0

::METHOD hours
  expose timestamp
  return time('Hours', timestamp, 'Full')

::METHOD minutes
  expose timestamp
  return time('Minutes', timestamp, 'Full') // 60

::METHOD seconds
  expose timestamp
  return time('Seconds', timestamp, 'Full') // 60

::METHOD microseconds
  expose timestamp

  parse value time('Long', timestamp, 'Full')  with '.' microseconds
  return microseconds + 0

::METHOD dayMinutes
  expose timestamp
  return time('Minutes', timestamp, 'Full')

::METHOD daySeconds
  expose timestamp
  return time('Seconds', timestamp, 'Full')

::METHOD dayMicroseconds
  expose timestamp
  numeric digits 18

  return self~daySeconds * 1000000 + self~microseconds

::METHOD hashCode
  expose timestamp
  return timestamp~hashcode

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


::METHOD addDays
  expose timestamp
  use strict arg days

  numeric digits 18
  -- return a new instance of DateTime
  return self~class~new(timestamp + days * (24 * 3600 * 1000000))


::METHOD addHours
  expose timestamp
  use strict arg hours

  numeric digits 18

  -- return a new instance of DateTime
  return self~class~new(timestamp + hours * (3600 * 1000000))


::METHOD addMinutes
  expose timestamp
  use strict arg minutes

  numeric digits 18

  -- return a new instance of DateTime
  return self~class~new(timestamp + minutes * (60 * 1000000))


::METHOD addSeconds
  expose timestamp
  use strict arg seconds

  numeric digits 18

  -- return a new instance of DateTime
  return self~class~new(timestamp + seconds * 1000000)


::METHOD addMicroseconds
  expose timestamp
  use strict arg usecs

  numeric digits 18

  -- return a new instance of DateTime
  return self~class~new(timestamp + usecs)


::METHOD isoDate
  expose timestamp
  use strict arg
  return date("S",timestamp,"F","-")"T"time("L", timestamp, "F")

::METHOD baseDate
  expose timestamp
  use strict arg
  return date("B", timestamp, "F")

::METHOD yearDay
  expose timestamp
  use strict arg
  return date("D", timestamp, "F");

::METHOD europeanDate
  expose timestamp
  use strict arg sep = ("/")
  return date("E", timestamp, "F", sep);

::METHOD languageDate
  expose timestamp
  use strict arg
  return date("L", timestamp, "F");

::METHOD monthName
  expose timestamp
  use strict arg
  return date("M", timestamp, "F");

::METHOD dayName
  expose timestamp
  use strict arg
  return date("W", timestamp, "F");

::METHOD normalDate
  expose timestamp
  use strict arg sep = (" ")
  return date("N", timestamp, "F", sep);

::METHOD orderedDate
  expose timestamp
  use strict arg sep = ("/")
  return date("O", timestamp, "F", sep);

::METHOD standardDate
  expose timestamp
  use strict arg sep = ("")
  return date("S", timestamp, "F", sep);

::METHOD usaDate
  expose timestamp
  use strict arg sep = ("/")
  return date("U", timestamp, "F", sep);

::METHOD weekDay
  expose timestamp
  use strict arg
  return date("B", timestamp, "F") // 7

::METHOD civilTime
  expose timestamp
  use strict arg
  return time("C", timestamp, "F")

::METHOD normalTime
  expose timestamp
  use strict arg
  return time("N", timestamp, "F")

::METHOD longTime
  expose timestamp
  use strict arg
  return time("L", timestamp, "F")

::METHOD fullDate
  expose timestamp
  return timestamp

::METHOD ticks
  expose timestamp
  use strict arg
  return time("T", timestamp, "F")

::METHOD isLeapYear
  use strict arg
  year = self~year

  return year // 4 == 0 & ((year // 100 \= 0) | (year // 400 == 0))

::METHOD daysInMonth
  use strict arg
  month = self~month

  if self~isLeapYear then do
      return "31 29 31 30 31 30 31 31 30 31 30 31"~word(month)
  end
  else do
      return "31 28 31 30 31 30 31 31 30 31 30 31"~word(month)
  end

::METHOD timeOfDay
  use strict arg
  return .timespan~new(time('F', self~LongTime, 'L'))

::METHOD date
  use strict arg
  return self~class~new(date('F', self~standardDate, 'S'))


::METHOD string      -- return a formatted string value
  return self~isoDate


::CLASS 'TimeSpan' public inherit Comparable
::METHOD fromDays class
  use strict arg days
  usec = days * (24 * 3600 * 1000000)
  return self~new(usec)


::METHOD fromHours class
  use strict arg hours
  usec = hours * (3600 * 1000000)
  return self~new(usec)


::METHOD fromMinutes class
  use strict arg minutes
  usec = minutes * (60 * 1000000)
  return self~new(usec)


::METHOD fromSeconds class
  use strict arg seconds
  usec = seconds * (000000)
  return self~new(usec)


::METHOD fromMicorseconds class
  use strict arg usec
  return usec


::METHOD init
  expose timestamp

  numeric digits 20    -- need higher digits for time calculations

  select
      when arg() == 0 then do
          raise syntax 93.901 array(1)
      end
      when arg() == 1 then do    -- just create from a microseconds value
          use strict arg timestamp
      end
      when arg() == 2 then do
          raise syntax 93.901 array(3)
      end
      when arg() == 3 then do
          -- convert hours, minutes, seconds to microseconds value
          use strict arg hours, minutes, seconds
          timestamp = ((hours * 3600) + (minutes * 60) + seconds) * 1000000
      end
      when arg() == 4 then do
          -- starting with days, hours, minutes, and seconds
          use strict arg days, hours, minutes, seconds
          timestamp = ((days * 24 * 3600) + (hours * 3600) + (minutes * 60) + seconds) * 1000000
      end
      when arg() == 5 then do
          -- starting with days, hours, minutes, seconds, and microseconds
          use strict arg days, hours, minutes, seconds, microseconds
          timestamp = (((days * 24 * 3600) + (hours * 3600) + (minutes * 60) + seconds) * 1000000) + microseconds
      end
      otherwise  do
          raise syntax 93.902 array(5)
      end
  end


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

::METHOD days
  expose timestamp
  numeric digits 18
  return timestamp % (24 * 3600 * 1000000)


::METHOD hours
  expose timestamp

  numeric digits 18
  -- subtract out the higher components
  remainder = timestamp // (24 * 3600 * 1000000)
  -- and return the number full units remaining
  return remainder % (3600 * 1000000)


::METHOD minutes
  expose timestamp

  numeric digits 18
  -- subtract out the higher components
  remainder = timestamp // (3600 * 1000000)
  -- and return the number full units remaining
  return remainder % (60 * 1000000)


::METHOD seconds
  expose timestamp

  numeric digits 18
  -- subtract out the higher components
  remainder = timestamp // (60 * 1000000)
  -- and return the number full units remaining
  return remainder % (1000000)


::METHOD microseconds
  expose timestamp

  numeric digits 18
  -- subtract out the higher components
  remainder = timestamp // 1000000
  -- and return the number full units remaining
  return remainder


::METHOD totalDays
  expose timestamp
  numeric digits 18
  return timestamp / (24 * 3600 * 1000000)


::METHOD totalHours
  expose timestamp

  numeric digits 18
  return timestamp / (3600 * 1000000)


::METHOD totalMinutes
  expose timestamp

  numeric digits 18
  return timestamp / (60 * 1000000)


::METHOD totalSeconds
  expose timestamp

  numeric digits 18
  return timestamp / (1000000)


::METHOD totalMicroseconds
  expose timestamp
  return timestamp

::METHOD hashCode
  expose timestamp
  return timestamp~hashcode

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


::METHOD "%"
  expose timestamp

  numeric digits 18     -- need this to do math on the value

  -- we just get a divisor and use it.  If it's bad, it's bad
  use strict arg divisor

  -- do the division and create a new interval
  return self~class~new(timestamp % divisor)

::METHOD "/"
  -- dividing a timespan is inherently an integer operation.  Handle as
  -- integer division.
  forward message("%")


::METHOD "//"
  expose timestamp

  numeric digits 18     -- need this to do math on the value

  -- we just get a divisor and use it.  If it's bad, it's bad
  use strict arg divisor


  -- do the remainder and create a new interval
  return self~class~new(timestamp // divisor)


::METHOD "*"
  expose timestamp

  numeric digits 18     -- need this to do math on the value

  -- we just get a multiplier and use it.  If it's bad, it's bad
  use strict arg multiplier

  -- do the multiply and create a new interval
  return self~class~new(timestamp * multiplier)


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
