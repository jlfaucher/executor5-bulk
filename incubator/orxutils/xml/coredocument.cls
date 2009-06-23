/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: CoreDocument                                                        */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
::class "CoreDocument" subclass ParentNode
::method init
  expose docType docElement encoding actualEncoding version standalone documentURI -
         userdata identifiers domNormalizer configuration xpathEvaluator changes   -
         allowGrammerAccess errorChecking documentNumber nodeCounter nodeTable
  use strict arg docType = .nil, grammarAccess = .false
  self~init:super(.nil)
  self~ownerDocument = self

  docElement = .nil
  encoding = .nil
  actualEncoding = .nil
  version = .nil
  standalone = .false
  documentURI = .false
  userData = .nil
  identifiers = .nil
  domNormalizer = .nil
  configuration = .nil
  xpathEvaluator = .nil
  changes = 0
  errorChecking = .true
  documentNumber = 0
  nodeCounter = 0
  nodeTable = .nil

  -- if we have a document type, this gets appended as our first child
  if docType \= .nil then do
      docType~ownerDocument = self
      appendChild(docType)
  end

::attribute ownerDocument GET
  use strict arg
  -- documents don't have an owner
  return .nil

::attribute nodeType GET
  use strict arg
  return .Node~DOCUMENT_NODE

::attirbute nodeName GET
  use strict arg
  return "#document"

::method cloneNode
  use strict arg deep = .false
  newDoc = .CoreDocument~new
  self~cloneDocument(newDoc, deep)
  return newDoc

::method cloneDocument private
  expose identifers, firstChild
  use arg newDoc, deep

  if deep then do
      reversedIdentifers = .nil
      if identifiers \= .nil then do
          reversedIdentifiers = .directory~new
          -- create the table using the inverse look up logic
          sup = identifiers~supplier
          do while sup~available
              reversedIdentifiers[sup~item] = sup~index
          end
      end

      -- now copy each of the children into the new document
      child = firstChild
      do while child \= .nil
          newDoc~appendChild(newDoc~importDocNode(child, .true, .true, reversedIdentifers))
      end
  end

  newDoc~allowGrammarAccess = self~allowGrammarAccess
  newDoc~errorChecking = self~errorChecking

::method insertBefore
  expose docElement docType
  use strict arg newChild, refChild

  type = newChild~nodeType
  -- if this is a DocumentType node, then make ourselves the owner
  if newChild~ownerDocument == .nil & newChild~isA(.DocumentType) then do
      newChild~ownerDocument = self
  end
  -- do a normal insert
  self~insertBefore:super(newChild, refChild)
  -- we only have two types of children, so cache each of
  -- type types
  if type == .Node~ELEMENT_NODE then do
      docElement = newChild
  end
  else if type == .Node~DOCUMENT_TYPE_NODE then do
      docType = newChild
  end
  return newChild

::method removeChild
  expose docElement docType
  use strict arg oldChild

  self~removeChild:super(oldChild)

  type = oldChild~ELEMENT_NODE

  if type = .Node~ELEMENT_TYPE then do
      docElement = .nil
  end
  else if type = .Node~DOCUMENT_TYPE_NODE do
      docType = .nil
  end
  return oldChild

::method replaceChild
  expose docElement docType
  use strict arg newChild, oldChild

  -- if this is a DocumentType node, then make ourselves the owner
  if newChild~ownerDocument == .nil & newChild~isA(.DocumentType) then do
      newChild~ownerDocument = self
  end

  self~replaceChild:super(newChild, oldChild)

  type = oldChild~ELEMENT_NODE

  if type = .Node~ELEMENT_TYPE then do
      docElement = .newChild
  end
  else if type = .Node~DOCUMENT_TYPE_NODE do
      docType = .newChild
  end
  return oldChild

::attribute textContent GET
  use strict arg
  return .nil

::attribute textContent SET
  use strict arg value
  -- this is a NOP

::method getFeature
  expose xpathEvaluator
  use strict arg feature, version = .nil

  anyVersion = version == .nil | version == ""

  if feature~caselessEquals("+XPath") & (anyVersion | version == "3.0") then do
      if xpathEvaluator == .nil then do
          xpathEvaluator = .XPathEvaluator~new(self)
      end
      return xpathEvaluator
  end

  return self~getFeature:super(feature, version)

-- Document factory methods

::method createAttribute
  use strict arg name
  return .Attr~new(self, name)

::method createAttributeNS
  if arg() == 2 then do
      use strict arg namespaceURI, qualifiedName
      return .Attr~new(self, namespaceURI, qualifiedName)
  end
  else do
      use strict arg namespaceURI, qualifiedName, localName
      return .Attr~new(self, namespaceURI, qualifiedName, localName)
  end

::method createCDATASection
  use strict arg data
  return .CDATASection~new(self, data)

::method createComment
  use strict arg data
  return .Comment~new(self, data)

::method createDocumentFragment
  use strict arg
  return .DocumentFragment~new(self)

::method createElement
  use strict arg tagname
  return .Element~new(self, tagname)

::method createElementNS
  if arg() == 2 then do
      use strict arg namespaceURI, qualifiedName
      return .Element~new(self, namespaceURI, qualifiedName)
  end
  else do
      use strict arg namespaceURI, qualifiedName, localName
      return .Element~new(self, namespaceURI, qualifiedName, localName)
  end

::method createEntityReference
  use strict arg name
  return .EntityReference~new(self, name)

::method createProcessingInstruction
  use strict arg target, data
  return .ProcessingInstruction~new(self, target, data)

::method createTextNode
  use strict arg data
  return .Text~new(self, data)

::attribute docType GET
::attribute documentElement GET
  expose docElement
  use strict arg
  return docElement

::method getElementsByTagName
  use strict arg tagName
  return .DeepNodeList~new(self, tagname)

::method getElementsByTagNameNS
  use strict arg namespaceURI, localName
  return .DeepNodeList~new(self, namespaceURI, localName)

::method getImplementation
  use strict arg

  return .DomImplementation~getDOMImplementation

::attribute errorChecking
::attribute strictErrorChecking

::attribute inputEncoding
::attribute xmlEncoding

::attribute documentURI

::method createDocumentType
  use strict arg qualifiedName, publicID, systemID

  return .DocumentType~new(self, qualifiedName, publicID, systemID)

::method createEntity
  use strict arg name
  return .Entity~new(self, name)

::method createNotation
  use strict arg name
  return .Notation~new(self, name)

::method importNode
  use strict arg source, deep = .false
  return self~importDocNode(source, deep, .false, .nil)

::method importDocNode private
  expose identifiers
  use strict arg source, deep, cloningDoc, reversedIdentifiers

  newNode = .nil
  type = source~nodeType
  select
      when type == .Node~ELEMENT_NODE then do
          if source~localName == .nil then do
              newElement = self~createElement(source~nodeName)
          end
          else do
              newElement = self~createElementNS(source~namespaceURI, source~nodeName)
          end
          -- we need to copy the attributes for the element here...other
          -- children are handled below
          sourceAttrs = source~attributes
          if sourceAttrs \= .nil then do
              do attr over sourceAttrs
                  if attr~isSpecified | cloningDoc then do
                      -- if we're just importing, ignore the default attributes.
                      newAttr = self~importNode(attr, .true, cloningDoc, reversedIdentifiers)
                      if attr~localName == .nil then do
                          newElement~setAttributeNode(newAttr)
                      end
                      else do
                          newElement~setAttributeNodeNS(newAttr)
                      end
                  end
              end
          end
          -- have a reversed identifer table?  We need to check if
          -- the element has an identifier and fix this up
          if reversedIdentifiers \= .nil then do
              elementId = reversedIdentifers[source]
              if elementId \= .nil then do
                  if identifiers == .nil then do
                      identifers = .table~new
                  end
                  identifiers[elementId] = newElement
              end
          end
          newNode = newElement
      end
      when type == .Node~ATTRIBUTE_NODE then do
          if source~localName == .nil then do
              newNode = self~createAttribute(source~nodeName)
          end
          else do
              newNode = self~createAttributeNS(source~namespaceURI, source~nodeName)
          end
          -- we'll do a deep copy, unless this can be avoided in the simple
          -- cases
          deep = .true
          -- if we have a string value, we can just copy that and
          -- avoid doing the deep copy
          if attr~hasStringValue then do
              newNode~value = attr~value
              deep = .false
          end
      end
      when type == .Node~TEXT_NODE then do
          newNode = self~createTextNode(source~nodeValue)
      end
      when type == .Node~CDATA_SECTION_NODE then do
          newNode = self~createCDATASection(source~nodeValue)
      end
      when type == .Node~ENTITY_REFERENCE_NODE then do
          newNode = self~createEntityReference(source~nodeName)
          -- createEntityReference copies the subtree, so
          -- disable the deep copy operation
          deep = .false
      end
      when type == .Node~ENTITY_NODE then do
          newNode = self~createEntity(source~nodeName)
          newNode~publicId = source~publicId
          newNode~systemId = source~systemId
          newNode~notationName = source~notationName
          -- the children need to be copied also...to do this,
          -- we need to make the entity writeable
          newNode = readOnly = .false
      end
      when type == .Node~PROCESSING_INSTRUCTION_NODE then do
          newNode = self~createProcessingInstruction(source~nodeName, source~nodeValue)
      end
      when type == .Node~COMMENT_NODE then do
          newNode = self~createComment(source~nodeValue)
      end
      when type == .Node~DOCUMENT_TYPE_NODE then do
          newNode = self~createDocumentType(source~nodeName, source~publicId, source~systemId)
          newNode~internalSubset = source~internalSubset
          sourceMap = source~entities
          newMap = newNode~entities
          -- copy all of the entities, if thee are any
          if sourceMap \= .nil then do
              do item over sourceMap
                  newMap~setNamedItem(self~importNode(item, .true, .true, reversedIdentifers)
              end
          end
          sourceMap = source~notations
          newMap = newNode~notations
          -- copy all of the notations, if thee are any
          if sourceMap \= .nil then do
              do item over sourceMap
                  newMap~setNamedItem(self~importNode(item, .true, .true, reversedIdentifers)
              end
          end
      end
      when type == .Node~DOCUMENT_FRAGMENT_NODE then do
          newNode = self~createDocumentFragment
      end
      when type == .Node~NOTATION_NODE then do
          newNode = self~createNotation(source~nodeName)
          newNode~publicId = source~publicId
          newNode~systemId = source~systemId
      end
      when type == .Node~DOCUMENT_NODE then do
         .DomErrors~raiseError(.DomErrors~NOT_SUPPORTED_ERR)
      end
      otherwise  do
         .DomErrors~raiseError(.DomErrors~NOT_SUPPORTED_ERR)
      end
  end

  -- do we need to copy the child nodes too?
  if deep then do
      child = source~firstChild
      do while child \= .nil
          newNode~appendChild(self~importNode(child, .true, cloningDoc, reversedIdentifiers)
      end
  end

  if newNode~nodeType == .Node~ENTITY_NODE then do
      newNode~readOnly = .true
  end
  return newNode

::method adoptNode
  expose docType
  use strict arg source

  if source == .nil then do
      return .nil
  end

  type = source~nodeType

  select
      when type == .Node~ATTRIBUTE_NODE then do
          -- detach from the owner if this is owned
          if source~ownerDocument \= .nil then do
              source~ownerElement~removeAttributeNode(source)
          end
          -- this is now specified, since it's no longer
          -- derived from a default associated with an element
          source~specified = .true
          -- change the owner
          source~ownerDocument = self
      end
      when type == .Node~ENTITY_NODE | type == .Node~NOTATION_NODE then do
          .DomErrors~raiseError(.DomErrors~NO_MODIFICATION_ALLOWED_ERR)
      end
      when type == .Node~DOCUMENT_NODE | type == .Node~DOCUMENT_TYPE_NODE then do
          .DomErrors~raiseError(.DomErrors~NOT_SUPPORTED_ERR)
      end
      when type == .Node~ENTITY_REFERENCE_NODE then do
          parent = source~parentNode
          if parent \= .nil then do
              parent~removeChild(source)
          end
          -- remove the replacement value
          child = source~firstChild
          do while child \= .nil
              source~removeChild(child)
              child = source~firstChild
          end

          source~ownerDocument = self
          if docType \= .nil then do
              entities = docType~entities
              entityNode = entities~getNamedItem(source~nodeName)
              if entityNode \= .nil then do
                  child = entityNode~firstChild
                  do while child \= .nil
                      newChild = child~cloneNode(.true)
                      source~appendChild(newChild)
                      child = child~nextSibling
                  end
              end
          end
      end
      when type == .Node~ELEMENT_NODE then do
          parent = source~parentNode
          if parent \= .nil then do
              parent~removeChild(source)
          end
          source~ownerDocument = self
      end
      otherwise  do
          parent = source~parentNode
          if parent \= .nil then do
              parent~removeChild(source)
          end
          source~ownerDocument = self
      end
  end
  -- return the adopted node
  return source

::method getElementById
  use strict arg id

  return self~getIdentifer(id)

::method getIdentifier
  expose identifers
  use strict arg id
  if identifers == .nil then do
      return .nil
  end

  element = identifers[id]
  if element \= .nil then do
      parent = element~parentNode
      do while parent \= .nil
          if parent == self then do
              return element
          end
          parent = parent~parentNode
      end
  end

  return .nil

::method clearIdentifiers private
  expose identifers
  if identifers \= .nil then do
      identifiers~empty
  end

::method putIdentifier
  expose identifiers
  use strict arg name, element

  if element == .nil then do
      self~removeIdentifier(name)
  end
  else do
      if identifiers == .nil then do
          identifiers = .directory~new
      end
      identifers[name] = element
  end

::method removeIdentifier
  expose identifiers
  use strict arg name
  if identifiers == .nil then do
      return
  end

  identifers~remove(name)

::method identifiers
  if identifiers == .nil  then do
      identifiers = .directory~new
  end

  return identifers~allIndexes

::method copy
  use strict arg
  newDoc = self~copy:super()

  newDoc~docType = .nil
  newDoc~docElement = .nil
  return newDoc

::method changed
  expose changes
  changes += 1

::method addEventListener
  use strict arg node, type, listener, useCapture
  -- this is a nop

::method removeEventListener
  use strict arg node, type, listener, useCapture

::method copyEventListeners
  use strict arg source, target

::method dispatchEvent
  use strict arg node, event

::method replacedText
  use strict arg node

::method deletedText
  use strict arg node, offset, count

::method insertedText
  use strict arg node, offset, count

::method modifyingCharacterData
  use strict arg node, replace

::method modifiedCharacterData
  use strict arg node, oldValue, value, replace

::method insertingNode
  use strict arg node, replace

::method insertedNode
  use strict arg node, newInternal, replace

::method removingNode
  use strict arg node, oldChild, replace

::method removedNode
  use strict arg node, replace

::method replacingNode
  use strict arg node

::method replacedNode
  use strict arg node

::method replacingData
  use strict arg node

::method replacedCharacterData
  use strict arg node, oldValue, value

::method modifiedAttrValue
  use strict arg attr, oldValud

::method setAttrNode
  use strict arg attr, previous

::method removedAttrNode
  use strict arg attr, oldOwner, name

::method renamedAttrNode
  use strict arg oldAttr, newAttr

::method renamedElement
  use strict arg oldElement, newElement
