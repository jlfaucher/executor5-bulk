/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: ProcessingInstruction                                               */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
::class "ProcessingInstruction" subclass CharacterData PUBLIC
::method init
  expose target
  use strict arg ownerDocument, target, data
  self~init:super(ownerDocument, data)

::attribute nodeType GET
  use strict arg
  return .Node~PROCESSING_INSTRUCTION_NODE

::attribute nodeName GET
  expose target
  use strict arg
  return target

::attribute target
::attribute data GET
  forward message("NODEVALUE")
::attribute data SET
  forward message("NODEVALUE=")
::attribute baseURI GET
  use strict arg
  return self~ownerNode~baseURI
