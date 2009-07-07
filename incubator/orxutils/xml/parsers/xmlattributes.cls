/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: XMLAttributes                                                       */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
::class "XMLAttributes"
::method init
  expose nameSpaces attributes

  nameSpaces = .false
  attributes = .table~new

::attribute nameSpaces
::method addAttribute
  expose attributes
  use strict arg qname, type, value

  attribute = .XMLAttribute~new(qname, type, value, nonNormalizedValue, .false)
  attributes[qname] = attribute

::method removeAllAttributes
  expose attributes
  use strict arg
  attributes~empty

::method removeAttribute
  expose attributes
  use strict arg attribute
  attributes~remove(attribute~name)


::class "XMLAttribute"
::method init
  expose name type value nonNormalizedValue specified schemaId augmentations
  use strict arg name, type, value, nonNormalizedValue, specified
  -- QNames are mutable, so make a copy of the name
  name = name~copy

  schemaId = .false
  augmentations = .directory~new

::attribute name
::attribute type
::attribute value
::attribute nonNormalizedValue
::attribute schemaID
::attribute specified
::attribute augmentations




