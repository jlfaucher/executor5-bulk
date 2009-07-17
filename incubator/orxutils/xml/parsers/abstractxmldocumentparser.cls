/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: AbstractXMLDocumentParser                                           */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
::class "AbstractXMLDocumentParser" subclass XMLParser

::method init
  expose inDtd documentSource dtdSource dtdContentModelSource
  use strict arg config
  self~init:super(config)

  config~documentHandler = self
  config~dtdHandler = self
  config~dtdContentModelHandler = self

::method startDocument
  use strict arg locator, encoding, namespaceContext, augmentations

::method xmlDecl
  use strict arg version, encoding, standalone, augmentations

::method doctypeDecl
  use strict arg rootElement, publicId, systemId, augmentations

::method startElement
  use strict arg element, attributes augmentations

::method emptyElement
  use strict arg element, attributes, augmentations

::method characters
  use strict arg text, augmentations

::method ignorableWhiteSpace
  use strict arg text, augmentations

::method endElement
  use strict arg qname, augmentations

::method startCDATA
  use strict arg augmentations

::method endCDATA
  use strict arg augmentations

::method endDocument
  use strict arg augmentations

::method startGeneralEntity
  use strict arg name, identifier, encoding, augmentations

::method textDecl
  use strict arg version, encoding, augmentations

::method endGeneralEntity
  use strict arg name, augmentations

::method comment
  use strict arg text, augmentations

::method processingInstruction
  use strict arg target, data, augmentations

::attribute documentSource

::method startDTD
  expose inDtd
  use strict arg locator, augmentations

  inDtd = .true

::method startExternalSubset
  use strict arg identifier, augmentations

::method endExternalSubset
  use strict arg augmentations

::method startParameterEntity
  use strict arg name, identifer, encoding, augmentations

::method endParameterEntity
  use strict arg name, augmentations

::method ignoredCharacters
  use strict arg text, augmentations

::method elementDecl
  use strict arg name, contentModel, augmentations

::method startAttlist
  use strict arg elementName, augmentations

::method attributeDecl
  use strict arg elementName, attributeName, type, enumeration, defaultType, defaultValue

::method endAttlist
  use strict arg augmentations

::method internalEntityDecl
  use strict arg name, text, nonNormalizedText, augmentations

::method externalEntityDecl
  use strict arg name, identifier, augmentations

::method unparsedEntityDecl
  use strict arg name, identifier, notation, augmentations

::method notationDecl
  use strict arg name, identifier, augmentations

::method startConditional
  use strict arg type, augmentations

::method endConditional
  use strict arg augmentations

::method endDtd
  use strict arg augmentations

::attribute dtdSource

::method startContentModel
  use strict arg elementName, augmentations

::method any
  use strict arg augmentations

::method empty
  use strict arg empty

::method startGroup
  use strict arg augmentations

::method pcdata
  use strict arg augmentations

::method element
  use strict arg elementName, augmentations

::method separator
  use strict arg separator, augmentations

::method occurrence
  use strict arg occurrence, augmentations

::method endGroup
  use strict arg augmentations

::method endContentModel
  use strict arg augmentations

::attribute dtdContentModelSource

::method reset private
  expose
  self~reset:super
  inDtd = .false

