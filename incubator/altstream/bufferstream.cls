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



