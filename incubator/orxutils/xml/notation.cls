/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Notation                                                            */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
::class "Notation" subclass Node PUBLIC
::method init
  expose name publicId systemId baseURI
  use strict arg ownerDoc, name
  self~init:super(ownerDoc)

  publicId = .nil
  systemId = .nil
  baseURI = .nil

::attribute nodeType GET
  use strict arg
  return .Node~NOTATION_NODE

::attribute nodeName GET
  expose name
  use strict arg
  return name

::attribute publicId
::attribute systemId
::attribute baseURI
