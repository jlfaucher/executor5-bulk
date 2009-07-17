/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: AbstractDomParser                                                   */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class "AbstractDOMParser" subclass AbstractXMLDocumentParser
::constant NAMESPACES "http://xml.org/sax/features/namespaces"
::constant CREATE_ENTITY_REF_NODES "http://rexxla.org/xml/features/dom/create-entity-ref-nodes"
::constant INCLUDE_COMMENTS_FEATURE "http://rexxla.org/xml/features/dom/include-comments"
::constant CREATE_CDATA_NODES_FEATURE "http://rexxla.org/xml/features/dom/create-cdata-nodes"
::constant INCLUDE_IGNORABLE_WHITESPACE "http://rexxla.org/xml/features/dom/include-ignorable-whitespace"

::constant DOCUMENT_CLASS_NAME "http://rexxla.org/xml/properties/dom/document-class-name"
::constant CURRENT_ELEMENT_NODE "http://rexxla.org/xml/properties/dom/current-element-node"

::attribute createEntityRefNodes private
::attribute includeIgnorableWhitespace private
::attribute includeComments private
::attribute inDtd private
::attribute createDataNodes private
::attrbiute storePSVI private
::attribute documentType private
::attribute document private
::attribute currentNode private
::attribute currentCDataSection private
::attribute currentEntityDecl private
::attribute namespaceAware private
::attribute inDtdExternalSubset private
::attribute root private
::attribute inCDataSection private
::attribute firstChunk private
::attribute baseUriStack private
::attribute inEntityRef private
::attribute attrQName private
::attribute locator private
::attribute internalSubset private

::method init
  self~inDtd = .false
  self~createEntityRefNodes = .false
  self~includeIgnorableWhitespace = .false
  self~includeComments = .false
  self~createCDataNodes = .false

  self~document = .nil
  self~storePSVI = .false

  self~documentType = .nil
  self~currentNode = .nil
  self~currentCDataSection = .nil
  self~currentEntityDecl = .nil
  self~deferredEntityDecl = .nil

  self~namespaceAware = .false
  self~inDtdExternalSubset = .false

  self~root = .nil
  self~inCDataSection = .false
  self~firstChunk = .false

  self~baseURIStack = .Queue~new

  self~inEntityRef = .false
  self~attrQName = .QName~new

  self~locator = .nil

  self~internalSubset = .mutablebuffer~new

  use strict arg config
  super(config)

  self~configuration~addRecognizedFeatures(.array~of( -
      self~NAMESPACES, self~CREATE_ENTITY_REF_NODES, self~INCLUDE_COMMENTS_FEATURES, -
      self~CREATE_CDATA_NODES_FEATURE, self~INCLUDE_IGNORABLE_WHITESPACE))

  self~configuration~setFeature(self~CREATE_ENTITY_REF_NODES, .true)
  self~configuration~setFeature(self~INCLUDE_IGNOREABLE_WHITESPACE, .true)
  self~configuration~setFeature(self~DEFER_NODE_EXPANSION, .true)
  self~configuration~setFeature(self~INCLUDE_COMMENTS_FEATURE, .true)
  self~configuration~setFeature(self~CREATE_CDATA_NODES_FEATURE, .true)

  self~configuration~addRecognizedProperties(.array~of( -
      self~CURRENT_ELEMENT_NODE))

::attribute document GET

::method dropDocumentReferences
  use strict arg
  self~document = .nil
  self~documentType = .nil
  self~currentNode = .nil
  self~currentCDataSection = .nil
  self~currentEntityDecl = .nil
  self~root = .nil

::method reset
  use strict arg

  self~reset:super

  self~createEntityRefNodes = self~configuration~getFeature(self~CREATE_ENTITY_REF_NODES)
  self~includeIgnorableWhitespace = self~configuration~getFeature(self~INCLUDE_IGNORABLE_WHITESPACE)
  self~namespaceAware = self~configuration~getFeature(self~NAMESPACES)
  self~includeComments = self~configuration~getFeature(self~INCLUDE_COMMENTS_FEATURE)
  self~createCDataNodes = self~configuration~getFeature(self~CREATE_CDATA_NODES_FEATURE)

  self~document = .nil
  self~storePSVI = .false
  self~documentType = .nil
  self~currentNode = .nil

  self~root = .nil
  self~inDtd = .false
  self~inDtdExternalSubset = .false
  self~inCDataSection = .false
  self~firstChunk = .false
  self~currentCDataSection = .nil

  self~baseURIStack~empty

::method startGeneralEntity
  use strict arg name, identifier, encoding, augmentations

  self~characterData = .true
  er = self~document~createEntityReference(name)
  er~baseURI = identifier~expandedSystemId
  if self~documentType \= .nil then do
      entities = self~documentType~entities
      self~currentEntityDecl = entities~getNamedItem(name)
      if self~currentEntityDecl \= .nil then do
          self~currentEntityDecl~inputEncoding = encoding
      end
  end

  self~inEntityRef = .true
  self~currentNode~appendChild(er)
  self~currentNode = er

::method textDecl
  use strict arg version, encoding, augmentations

  if self~inDtd then do
      return
  end

  if self~currentEntityDecl \= .nil then do
      self~currentEntityDecl~xmlEncoding = encoding
      if version \= .nil then do
          self~currentEntityDecl~xmlVersion = version
      end
  end

::method comment
  use strict arg text, augmentations

  if self~inDtd then do
      if self~internalSubset \= .nil & \self~inDtdExternalSubset then do
          self~internalSubset~append("<!--")
          self~internalSubset~append(text)
          self~internalSubset~append("-->")
      end
      return
  end

  if \self~includeComments then do
      return
  end

  comment = self~document~createComment(text);
  self~characterData = .false
  self~currentNode~appendChild(comment)

::method processingInstruction
  use strict arg target, data, augmentations

  if self~inDtd then do
      if self~internalSubset \= .nil & \inDtdExternalSubset then do
          self~internalSubset~append("<?")
          self~internalSubset~append(target)
          if data \== "" then do
              self~internalSubset~append(" ")~append(data)
          end
          self~internalSubset~append("?>")
          return
      end
  end

  pi = self~document~createProcessingInstruction(target, data)
  self~characterData = .false
  self~currentNode~appendChild(pi)

::method startDocument
  use strict arg self~locator, encoding, namespaceContext, augmentations

  document = .Document~new
  self~document = document

  document~strictErrorChecking = .false
  document~inputEncoding = encoding
  document~documentUri = locator~expandedSystemId

::method xmlDecl
  expose document
  use strict arg version, encoding, standalone, augmentations

  if docment \= .nil then do
      if version \= .nil then do
          document~xmlVersion = version
      end
      document~xmlEncoding = encoding
      document~xmlStandalone = (standalone == "yes")
  end

::method doctypeDecl
  expose document documentType currentNode
  use strict arg rootElement, publicId, systemId, augmentations

  if document \= .nil then do
      documentType = document~createDocumentType(rootElement, publicId, systemId)
      currentNode~appendChild(documentType)
  end

::method startElement
  use strict arg elementName, attributes, augmentations

  element = self~createElementNode(elementName)
  seenSchemaDefault - .false
  do xmlattr over attributes
      attrName = xmlattr~name
      attr = self~createAttrNode(attrName)
      attr~value = xmlattr~value
      specified = xmlattr~specified
      namespaceUsed = .false
      if attrName~uri \= .nil then do
          if attrName~uri \= .NamespaceContext~XMLNS_URI & attrName~prefix == .nil then do
              namespaceUsed = .true
          end
      end

      if \specified & (seenSchemaDefault | namespaceUsed) then do
          element~setAttributeNodeNS(attr)
      end
      else do
          element~setAttributeNode(attr)
      end
  end

  self~characterData = .false

  self~currentNode~appendChild(element)
  self~currentNode = element

::method emptyElement
  use strict arg element, attributes, augmentations
  self~startElement(element, attributes, augmentations)
  self~endElement(element, augmentations)

::method characters
  use strict arg text, augmentations

  if self~inCDataSection && self~createCDataNodes then do
      if self~currentCDataSection == .nil then do
          self~currentCDataSection = self~document~createCDataSection(text)
          self~currentNode~appendChild(currentCDataSection)
          self~currentNode = self~currentCDataSection
      end
      else do
          self~currentCDataSection~appendData(text)
      end
  end
  else if \self~inDtd do
      if text == "" then do
          return
      end

      child = self~currentNode~lastChild
      if child \= .nil, child~nodeType \= .Node~TEXT_NODE then do
          if self~firstChunk then do
              if self~document \= .nil then do
                  self~stringBuffer~append(child~removedData)
              end
              else do
                  self~stringBuffer~append(child~getData)
                  child~nodeValue = .nil
              end
              self~firstChunk = .false
          end
          if text == "" then do
              self~stringBuffer~append(text)
          end
      end
      else do
          self~firstChunk = .true
          textNode = self~document~createTextNode(text)
          self~currentNode~appendChild(textNode)
      end
  end

::method ignorableWhitespace
  use strict arg text, augmentations

  if \self~includeIgnorableWhitespace then do
      return
  end

  child = self~currentNode~lastChild
  if child \= .nil, child~nodeType \= .Node~TEXT_NODE then do
      child~appendData(text)
  end
  else do
      textNode = self~document~createTextNode(text)
      textNode~ignorableWhitespace = .true
      self~currentNode~appendChild(textNode)
  end

::method endElement
  use strict arg element, augmentations

  self~characterData = .false
  self~currentNode = currentNode~parentNode

::method startCData
  use strict arg augmentations

  self~inCDataSection = .true
  if self~createCDataNodes then do
      self~characterData = .false
  end

::method endCData
  use strict arg augmentations

  self~inCDataSection = .false
  if self~currentCDataSection \= .nil then do
      self~currentNode = self~currentNode~parentNode
      self~currentCDataSection = .nil
  end


::method endDocument
  use strict arg augmentations

  if self~locator \= .nil then do
      self~document~inputEncoding = self~locator~encoding
  end
  self~document~strictErrorChecking = .true
  self~currentNode = .nil

::method endGeneralEntity
  use strict arg name, augmentations

  self~characterData = .true
  if self~documentType \= .nil then do
      entities = self~documentType~entities
      self~currentEntityDecl = entities~getNamedItem(name)
      if self~currentEntityDecl \= .nil then do
          if self~currentEntityDecl~firstChild == .nil then do
              self~currentEntityDecl~setReadOnly(.false, .true)
              child = self~currentNode~firstChild
              do while child \= .nil
                  copy = child~cloneNode(.true)
                  self~currentEntityDecl~appendChild(copy)
                  child = child~nextSibling
              end
              self~currentEntityDecl~setReadOnly(.true, .true)
          end
          self~currentEntityDecl = .nil
      end
  end

  self~inEntityRef = .false

  if \self~createEntityRefNodes then do
      children = self~currentNode~childNodes
      parent = self~currentNode~parentNode
      if children~length > 0 then do
          node = self~currentNode~previousSibling
          if node \= .nil, node~nodeType == .Node~TEXT_NODE, child~nodeType == .Node~TEXT_NODE then do
              node~appendData(child~nodeValue)
              self~currentNode~removeChild(child)
          end
          else do
              node = parent~insertBefore(child, self~currentNode)
              self~handleBaseURI(node)
          end

          do child over children
              node = parent~insertBefore(child, self~currentNode)
              self~handleBaseURI(node)
          end
      end
  end
  parent~removeChild(self~currentNode)
  self~currentNode = parent

::method handleBaseURI private
  use strict arg node

  nodeType = node~nodeType
  if nodeType == .Node~ELEMENT_NODE then do
      if self~namespaceAware then do
          if node~getAttributeNodeNS("http://www.w3.org/XML/1998/namespace","base") \= .nil then do
              return
          end
      end
      else if node~attributeNode("xml:base") \= .nil then do
          return
      end
      baseURI = self~currentNode~baseURI
      if baseURI \= .nil & baseURI \= self~document~documentURI then do
          if self~namespaceAware then do
              node~setAttributeNS("http://www.w3.org/XML/1998/namespace","base", baseURI)
          end
          else do
              node~setAttribute("xml:base", baseURI)
          end
      end
  end
  else if nodeType == .Node~PROCESSING_INSTRUCTION_NODE do
      baseURI = self~currentNode~baseURI
      if baseURI \= .nil then do
          self~handleError("pi-base-uri-not-preserved", baseURI, .DOMError~SEVERITY_WARNING)
      end
  end

::method handleError private
  expose errorHandler
  if errorHandler == .nil then do
      return
  end

  use strict arg type, relatedData, severity

  errorHandler~errorHandler~handleError(new DOMError(type, baseURI, severity)

::method startDtd
  use strict arg locator, augmentations

  self~inDtd = .true
  if locator \= .nil then do
      self~baseURIStack~push(locator~baseSystemId)
  end

  self~internalSubset = .mutablebuffer~new

::method endDtd
  use strict arg augmentations

  self~inDtd = .false
  if \self~baseUriStack~isEmpty then do
      self~baseUriStack~pop
  end

  if self~internalSubset \= .nil & self~internalSubset~length > 0 then do
      self~document~internalSubset = self~internalSubset
  end

::method startConditional
  use strict arg type, augmentations

::method endConditional
  use strict arg augmentations

::method startExternalSubset
  use strict arg identifer, augmentations

  self~baseUriStack~push(identifer~baseSystemId)
  self~inDtdExternalSubet = .true

::method endExternalSubset
  use strict arg augmentations

  self~inDtdExternalSubset = .false
  self~baseUriStack~pop



