::class OutputStream public
::method charout abstract
::method lineout abstract

::method open
::method close

::method arrayOut
  use strict arg lines

  do line over lines
      self~lineout(line)
  end

::method linein
  raise ....  -- not supported

::method lines
  raise ....  -- not supported

::method charin
  raise ....  -- not supported

::method chars
  raise ....  -- not supported

::method position
  raise ....  -- not supported


::class InputStream public
::method charout
  raise ....  -- not supported
::method lineout
  raise ....  -- not supported

::method open
::method close

::method linein abstract
::method lines  abstract
::method charin abstract
::method chars  abstract

::method position
  raise ....  -- not supported


::class InputOutputStream public
::method charout abstract
::method lineout abstract

::method open
::method close

::method linein abstract
::method lines  abstract
::method charin abstract
::method chars  abstract

::method position
  raise ....  -- not supported


::class BufferOutputStream public subclass OutputStream
::init
  expose buffer size position linend
  use strict arg size = 4096
  buffer = .mutablebuffer~new(size)
  position = 1

  parse source platform .

::method charout
  expose buffer position

  use strict arg string, start = position

  buffer~overlay(string, start)
  position = start + string~length

  return 0

::method lineout
  use string arg line

  self~charout(line)
  self~charout(.lineend)
  return 0

::method buffer
  expose buffer
  return buffer

::method stringValue
  expose buffer
  return buffer~string


::class ArrayOutputStream public subclass OutputStream
::method init
  expose buffer, array

  buffer = .mutablebuffer~new
  array = .array~new

::method lineout
  use string arg line

  self~charout(line)
  self~charout(.lineend)
  return 0

::method charout
  expose buffer array
  use strict arg string

  buffer~append(charout)
  lineend = buffer~pos(.lineend)

  do while lineend \= 0
      array~append(buffer~substr(1, lineend - 1))
      buffer~delete(1, lineend + .lineend~length - 1)
      lineend = buffer~pos(.lineend)
  end

  return 0

::method flush private
  expose buffer, array
  if buffer~length > 0 then do
      array~append(buffer~string)
      buffer~setBufferLength(0)
  end

::method getArray
  expose array
  self~flush
  return array



