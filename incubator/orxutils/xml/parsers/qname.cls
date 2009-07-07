/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: QName                                                               */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
::class "QName"
::method init
  if args() == 0 then do
      self~clear
  end
  else if args() == 1 then do
      use strict arg qname
      self~setValues(qname)
  end
  else if args > 1 then do do
      use strict arg prefix, localpart, rawname, uri
      self~setValues(prefix, localpart, rawname, uri)
  end

::method setValues
  expose prefix localpart rawname uri
  if args() <= 1 then do
      use strict arg qname
      prefix = qname~prefix
      localpart = qname~localpart
      rawname = qname~rawname
      uri = qname~uri
  end
  else if args > 1 then do do
      use strict arg prefix, localpart, rawname, uri
  end

::method clear
  expose prefix localpart rawname uri
  prefix = .nil
  localpart = .nil
  rawname = .nil
  uri = .nil

::method hashCode
  expose prefix localpart rawname uri

  if uri \= .nil then do
      return uri~hashCode~bitxor(localPart~hashCode)
  end
  else do
      return rawname~hashCode
  end

::method equals
  expose prefix localpart rawname uri
  use strict arg object
  if object~isA(.QName) then do
      return uri == qname~uri & localpart == qname~localpart & rawname == qname~rawname
  end

  return .false

::method "=="
  forward message("EQUALS")

::method "="
  forward message("EQUALS")

::method "\=="
  forward message("EQUALS")
  return \result

::method "\="
  forward message("EQUALS")
  return \result

::method string
  expose prefix localpart rawname uri
  buffer = .mutablebuffer~new
  comma = .false

  if prefix \= .nil then do
      buffer~append('prefix="')~append(prefix)~append('"')
      command = .true
  end

  if localpart \= .nil then do
      if comma then do
          buffer~append(",")
      end
      buffer~append('localpart="')~append(localpart)~append('"')
      command = .true
  end

  if rawname \= .nil then do
      if comma then do
          buffer~append(",")
      end
      buffer~append('rawname="')~append(rawname)~append('"')
      command = .true
  end

  if uri \= .nil then do
      if comma then do
          buffer~append(",")
      end
      buffer~append('uri="')~append(uri)~append('"')
      command = .true
  end




