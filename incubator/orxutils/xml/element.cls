/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Element                                                             */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class "Element" subclass ParentNode public
::method init
  expose nodeName attributes namespaceURI localName type
  attributes = .nil
  namespaceURI = .nil
  localName = .nil
  type = .nil

  if arg() == 2 then do
      use strict arg ownerDoc, nodeName
      self~init:super(ownerDoc)
  end
  else if arg() == 3 then do
      use strict arg ownerDoc, namespaceURI, nodeName
      self~init:super(ownerDoc)
      self~setName(nodeName)
  end
  else do
      use strict arg ownerDoc, namespaceURI, nodeName, localName
      self~init:super(ownerDoc)
  end

::method setName private
  expose namespaceURI localName
  use arg qname
  -- null string is the same as not there
  if namspaceURI == "" then do
      namespaceURI = .nil
  end

  colon1 = qname~pos(":")
  colon2 = qname~lastPos(":")
  -- no prefix
  if colon1 == 0 then do
      -- local name and qualified name are the same
      localName = qname
  end
  else do
      parse var qname localName (colon1) (colon2 + 1) localName
  end

-- support for the Document renameNode method
::method rename
  expose nodeName namespaceURI
  use strict arg uri, name = .nil
  if name == .nil then do
      nodeName = uri
  end
  else do
      nodeName = name
      namespaceURI = uri
      self~setName(name)
  end

-- override for default method
::attribute namespaceURI GET
-- get the prefix from the node name
::attribute prefix GET
  expose nodeName
  index = nodeName~pos(":")
  if index > 0 then do
      return nodeName~substr(1, index - 1)
  end
  else do
      return .nil
  end

::attribute prefix SET
  expose nodeName localName
  use strict arg prefix

  -- we're either adding or replacing the prefix
  if prefix \= "" then do
      nodeName = prefix":"localName
  end
  -- or removing it entirely
  else do
      nodeName = localName
  end

::attribute localName GET

::attribute nodeType GET
  return .node~ELEMENT_NODE

::attribute nodeName GET

::attribute attributes GET
  expose attributes
  use strict arg

  if attributes == .nil then do
      attributes = .AttributeMap~new(this, .nil)
  end

  return attributes

::attribute attributes SET private

::method cloneNode
  expose attributes
  use strict arg deep = .false
  newNode = self~cloneNode(deep)

  -- the attributes are always copied regardless of the deep flag
  if attributes \= .nil then do
      newNode~attributes = attributes~cloneMap(newNode)
  end

  return newNode

::attribute baseURI GET
  use strict arg
  return .nil

::attribute ownerDocument SET private
  expose attributes
  use strict arg doc

  forward class(super) continue
  -- also set this for all of the attributes
  if attributes \= .nil  then do
      attributes~ownderDocument = doc
  end


::method getAttribute
  expose attributes

  use strict arg name
  if attributes == .nil then do
      return ""
  end

  attr = attributes~getNamedItem(name)
  if attr == .nil then do
      return ""
  end
  else do
      return attr~value
  end


::method getAttributeNode
  expose attributes

  use strict arg name
  if attributes == .nil then do
      return .nil
  end

  return attributes~getNamedItem(name)

::method getElementsByTagName
  use strict arg tagname
  // this version does a lazy search
  return .DeepNodeList~new(this, tagname)

::remove attributeNode
  expose attributes

  use strict arg attr

  if attributes \= .nil then do
      attributes~removeItem(attr)
  end

-- this is the same as the nodename for an element
::attribute tagname GET
  expose nodeName
  return nodeName

::method removeAttribute
  expose attributes
  ust strict arg name

  if attrbutes \= .nil  then do
      attributes~removeNamedItem(name)
  end

::method removeAttributeNode
  expose attributes
  ust strict arg oldAttr

  if attrbutes \= .nil  then do
      attributes~removItem(oldAttr, .true)
  end

::method setAttribute
  expose attributes

  use strict arg name, value

  attr = self~getAttributeName(name)
  if newAttr == .nil then do
      newAttr = getOwnerDocument().createAttribute(name)

      if attributes =- .nil  then do
          attributes = .AttributeMap~new(this)
      end

      newAttr~nodeValue = value
      attributes~setNamedItem(newAttr)
  end
  else do
      newAttr~nodeValue = value
  end

::method setAttributeNode
  expose attributes

  use strict arg newAttr

  if attributes == .nil then do
      attributes = .AttributeMap~new(this)
  end

  return attributes~setNamedItem(newAttr)


::method getAttributeNS
  expose attributes
  use strict arg namespaceURI, localName

  if attributes == .nil then do
      return ""
  end

  attr = attributes~getNamedItemNS(namespaceURI, localName)
  if attr == .nil then do
      return ""
  end
  else do
      return attr~value
  end


::method setAttributeNS
  espose attributes
  use strict arg namespaceURI, qualifiedName, value

  index = qualifiedName~pos(":")
  if pos > 0 then do
      parse var qualifiedName prefix ":" localName
  end
  else do
      prefix = .nil
      localName = qualifiedName
  end

  newAttr = getAttributeNodeNS(namespaceURI, localName)
  if newAttr == .nil then do
      newAttr = self~getOwnerDocument ~createAttributeNS(namespaceURI, qualifiedName)

      if attributes == .nil then do
          attributes = .AttributeMap~new(this)
      end

      newAttr~nodeValue = value
      attributes~setNameItemNS(newAttr)
  end
  else do
      newAttr~name = qualifiedName
      newAttr~nodeValue = value
  end

::method removeAttributeNS
  expose attributes
  use strict arg namespaceURI, localName

  if attributes == .nil then do
      return
  end

  attributes~removeNamedItemNS(namespaceURI, localName)

::method getAttributeNodeNS
  expose attributes
  use strict arg namespaceURI, localName

  if attributes == .nil then do
      return
  end

  return attributes~getNamedItemNS(namespaceURI, localName)

::method setAttributeNodeNS
  expose attributes
  use strict arg newAttr

  if attributes == .nil then do
      return
  end

  return attributes~setNamedItemNS(newAttr)

::method hasAttributes
  expose attributes

  if attributes == .nil then do
      return .false
  end

  return attributes~length > 0

::method hasAttribute
  use strict arg name
  return self~getAttributeNode(name) \= .nil

::method hasAttributeNS
  use strict arg namespaceURI, localName

  return self~getAttributeNodeNS(namespaceURI, localName) \= .nil

::method getElementsByTagNameNS
  use strict arg namespaceURI, localName

  return .DeepNodeList~new(this, localName, namespaceURI)

::method setIdAttributeNode
  use strict arg at, makeId

  at~isIdAttribute = makeId
  if makeId then do
      self~ownerDocument~putIdentifier(at~value, this)
  end
  else do
      self~ownerDocument~removeIdentifier(at~value)
  end

::method setIdAttribute
  use strict arg name, makeId

  at = self~getAttributeNode(name)
  if at == .nil then do
      return
  end

  at~isIdAttribute makeId
  if makeId then do
      self~ownerDocument~putIdentifier(at~value, this)
  end
  else do
      self~ownerDocument~removeIdentifier(at~value)
  end

::method setIdAttributeNS
  use strict arg namespaceURI, localname, makeId

  at = self~getAttributeNodeNS(namspaceURI, localname)
  if at == .nil then do
      return
  end

  at~isIdAttribute makeId
  if makeId then do
      self~ownerDocument~putIdentifier(at~value, this)
  end
  else do
      self~ownerDocument~removeIdentifier(at~value)
  end

::attribute typeName GET
  expose type
  use strict arg
  if type \= .nil then do
      return type~typeName
  end
  else do
      return .nil
  end

::attribute typeNamespace GET
  expose type
  use strict arg
  if type \= .nil then do
      return type~namespace
  end
  else do
      return .nil
  end

::method isDerivedFrom
  expose type
  use strict arg typeNamespace, typeName, derivationMethod
  if type \= .nil then do
      return type~isDOMDerivedFrom(typeNamespace, typeName, derivationMethod)
  end
  else do
      return .false
  end

-- set/retrieve type directly
::attribute type

::attribute schemaTypeInfo GET
  use strict arg
  return self

-- ElementTraversal methods

::attribute childElementCount GET
  use strict arg
  count = 0
  child = self~firstElementChild
  do while child \= .nil
      count += 1
      child = child~nextElementSibling
  end

  return count

::attribute firstElementChild GET
  use strict arg
  node = self~firstChild

  do while node \= .nil
      if node~nodeType == .Node~ELEMENT_NODE then do
          return node
      end
      else if node~nodeType == .Node~ENTITY_REFERENCE_NODE then do
          e = self~getFirstChildElement(node)
          if e \= .nil  then do
              return e
          end
      end
      node = node~nextSibling
  end
  return .nil

::attribute lastElementChild GET
  use strict arg
  node = self~lastChild

  do while node \= .nil
      if node~nodeType == .Node~ELEMENT_NODE then do
          return node
      end
      else if node~nodeType == .Node~ENTITY_REFERENCE_NODE then do
          e = self~getFirstChildElement(node)
          if e \= .nil  then do
              return e
          end
      end
      node = node~previousSibling
  end
  return .nil


::attribute nextElementSibling GET
  use strict arg
  node = self~nextLogicalSibling(self)

  do while node \= .nil
      if node~nodeType == .Node~ELEMENT_NODE then do
          return node
      end
      else if node~nodeType == .Node~ENTITY_REFERENCE_NODE then do
          e = self~getFirstChildElement(node)
          if e \= .nil  then do
              return e
          end
      end
      node = node~nextLogicalSibling(node)
  end


::attribute previousElementSibling GET
  use strict arg
  node = self~previousLogicalSibling(self)

  do while node \= .nil
      if node~nodeType == .Node~ELEMENT_NODE then do
          return node
      end
      else if node~nodeType == .Node~ENTITY_REFERENCE_NODE then do
          e = self~getFirstChildElement(node)
          if e \= .nil  then do
              return e
          end
      end
      node = node~previousLogicalSibling(node)
  end

::method getFirstElementChild private
  use strict arg node

  top = node

  do while node \= .nil
      if node~nodeType == .Node~ELEMENT_NODE then do
          return node
      end
      next = node~firstChild
      do while next \= .nil
          if node == top then do
              leave;
          end
          next = node~nextSibling
          if next \= .nil then do
              node = node~parentNode
              if node == .nil | node == top then do
                  return .nil
              end
          end
      end
      node = next
  end
  return .nil

::method getLastElementChild private
  use strict arg node

  top = node

  do while node \= .nil
      if node~nodeType == .Node~ELEMENT_NODE then do
          return node
      end
      next = node~lastChild
      do while next \= .nil
          if node == top then do
              leave;
          end
          next = node~previousSibling
          if next \= .nil then do
              node = node~parentNode
              if node == .nil | node == top then do
                  return .nil
              end
          end
      end
      node = next
  end
  return .nil

::method getNextLogicalSibling private
  use arg node

  next = node~nextSibling

  if next == .nil then do
      parent = node~parentNode
      do while parent \= .nil, parent~nodeType == .Node~ENTITY_REFERENCE_NODE
          next = parent~nextSibling
          if next \= .nil then do
              leave
          end
          parent = parent~parentNode
      end
  end

  return next

::method getPreviousLogicalSibling private
  use arg node

  previous = node~previousSibling

  if previous == .nil then do
      parent = node~parentNode
      do while parent \= .nil, parent~nodeType == .Node~ENTITY_REFERENCE_NODE
          previous = parent~previousSibling
          if previous \= .nil then do
              leave
          end
          parent = parent~parentNode
      end
  end

  return previous
