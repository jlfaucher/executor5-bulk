/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Entity -- an entity value                                           */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class "Entity" subclass ParentNode PUBLIC
::method init
  expose name publicId systemId XmlEncoding inputEncoding XMLversion notationName baseURI
  use strict arg ownerDoc, name
  self~init:super(ownerDoc)
  publicId = .nil
  systemId = .nil
  XmlEncoding = .nil
  inputEncoding = .nil
  XmlVersion = .nil
  notationName = .nil
  baseURI = ,nil

::attribute nodeType GET
  use strict arg
  return .Node~ENTITY_NODE

::attribute nodeName GET
  expose name
  use strict arg
  return name

::attribute publicId
::attribute systemId
::attribute XmlVersion
::attribute XmlEncoding
::attribute inputEncoding
::attribute notationName
::attribute baseURI
