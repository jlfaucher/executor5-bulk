/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Attr                                                                */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class "Attr" subclass Node
::init
  expose value name textNode namespaceURI localName type
  value = .nil
  type = .nil
  textNode = .nil
  namespaceURI = .nil
  localName = .nil
  if arg() == 2 then do
      use strict arg ownerDocument, name
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
  self~init:super(ownerDocument)
  self~isSpecified = .false
  self~hasStringValue = .true

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

::attribute ownerDocument SET private
  expose value
  use strict arg doc
  forward class(super) continue
  if \self~hasStringValue then do
      child = value
      do while child \= .nil
          child~ownerDocument = doc
          child = child~nextSibling
      end
  end

::attribute idAttribute SET
  use strict arg value
  self~isIdAttribute = value

::method isId
  use strict arg
  return self~isIdAttribute

::method cloneNode
  expose value
  use strict arg deep = .false

  newNode = self~cloneNode:super(deep)

  if \self~hasStringValue then do
      close~value = .nil
      child = value
      do while child \= .nil
          newNode~appendChild(child~cloneNode(.true))
          child = child~nextSibling
      end
  end

  newNode~isSpecified = .true
  return newNode

::attribute nodeType GET
  use strict arg
  return .Node~ATTRIBUTE_NODE

::attribute nodeName GET
  expose name
  use strict arg
  return name

::attribute nodeValue SET
  forward message("VALUE=")
::attribute nodeValue GET
  forward message("VALUE")


::attribute typeName GET
  expose type
  use strict arg
  if type \= .nil then do
      if type~isA(.String) then do
          return type
      end
      else do
          return type~typeName
      end
  end
  else do
      return .nil
  end

::attribute typeNamespace GET
  expose type
  use strict arg
  if type \= .nil then do
      if \type~isA(.String) then do
          return type~namespace
      end
      else do
          return "http://www.w3.org/TR/REC-xml";
      end
  end
  return .nil

::method isDerivedFrom
  expose type
  use strict arg typeNamespace, typeName, derivationMethod
  if type \= .nil, \type~isA(.String) then do
      return type~isDOMDerivedFrom(typeNamespace, typeName, derivationMethod)
  end
  return .false

-- set/retrieve type directly
::attribute type
::attribute typeNamespace GET
  expose typeName
  use strict arg
  if typeName \= .nil then do
      return "http://www.w3.org/TR/REC-xml"
  end
  else do
      return .nil
  end

::attribute schemaTypeInfo
  use strict arg
  -- we provide all of our own type information directly
  return this

::attribute name GET

::attribute value SET
  expose value textNode
  use strict arg newValue

  self~ownerElement
  oldvalue = ""

  -- we might need to process an old value if it's a serious of text nodes
  if value \= .nil then do
      if self~ownerDocument~mutationEvents then do
          if self~hasMutationEvents then do
              -- if we have a string value and we need to
              -- broadcast mutation events, then we need to
              -- convert this into a text node for the event
              if self~hasStringValue then do
                  oldValue = value
                  if textNode == .nil then do
                      textNode = self~ownerDocument~createTextNode(value)
                  end
                  else do
                      textNode~data = value
                  end
                  -- set the value to be a text node rather than a string
                  value = textNode
                  textNode~ownerNode = this
                  textNode~isOwned = .true
                  self~removeChild(textNode, true)
              end
              else do
                  oldValue = self~value
                  do while value \= .nil
                      self~removeChild(value, .true)
                  end
              end
          end
      end
      else do
          if self~hasStringValue then do
              oldValue = value
          end
          else do
              -- discard any text children
              oldValue = self~value
              firstChild = value
              firstChild ownerNode = self~ownerDocument
          end
          -- remove the existing value
          value = .nil
      end
  end

  self~isSpecified = .true
  if self~ownerDocument~mutationEvents then do
      self~insertBefore(self~ownerDocument~createTextNode(newValue), .nil, .true)
      self~hasStringValue = .false
      self~ownerDocument~modifiedAttrValue(this, oldvalue)
  end
  else do
      value = newValue
      self~changed()
  end

  if self~isIdAttribute & self~ownderElement \= .nil then do
      self~ownerDocument~putIdentifier(newvalue, ownerElement)
  end

::attribute value GET
  expose value
  use strict arg
  -- no set value is a null string
  if value == .nil then do
      return ""
  end
  -- if already a string, then set it directly
  if value~isA(.String) then do
      return value
  end

  -- this is a potential chain of child nodes
  firstChild = value

  if firstChild~nodeType == .Node~ENTITY_REFERENCE_NODE then do
      data = firstChild~entryRefValue
  end
  else do
      data = firstChild~nodeValue
  end
  -- no data at this point, return a null string
  if data == .nil then do
      return ""
  end

  node = firstChild~nextSibling
  -- if the only child node, we're done
  if node == .nil then do
      return data
  end

  buffer = .mutableBuffer~new(data)

  do while node \= .nil
      if firstChild~nodeType == .Node~ENTITY_REFERENCE_NODE then do
          data = firstChild~entryRefValue
          if data \= .nil then do
              buffer~append(data)
          end
      end
      else do
          buffer~append(firstChild~nodeValue)
      end
      node = node~nextSibling
  end

  return buffer~string

::attribute element GET
  use strict arg

  if self~isOwned then do
      return self~ownerNode
  end
  else do
      return .nil
  end

::attribute ownerElement GET
  forward message("ELEMENT")

::attribute specified GET
  forward name("ISSPECIFIED")
::attribute specified SET
  forward name("ISSPECIFIED=")

::method hasChildNodes
  expose value
  use strict arg
  return value \= .nil

::method childNodes
  -- we handle the node list methods directly
  use strict arg
  return self

::method firstChild
  expose value
  use strict arg
  self~makeChildNode
  return value

::method lastChild
  expose value
  use strict arg

  self~makeChildNode

  child = value
  previous = .nil

  do while child \= .nil
      previous = child
      child = child~nextSibling
  end

  return previous

::method insertBefore
  expose value
  use strict arg newChild, refChild, replace = .false

  -- if this is a fragment, transfer each of the
  -- children from the fragment to ourselves
  if newChild~nodeType == .Node~DOCUMENT_FRAGMENT_MODE then do
      do while newChild~hasChildNodes
          self~insertBefore(newChild~firstChild, refChild)
      end
      return newChild
  end
  -- already here?  This is a little silly, but the event model
  -- requires us to remove it and the reinsert
  if newChild == refChild then do
      refChild = refChild~nextSibling
      self~removeChild(newChild)
      self~insertBefore(newChild, refChild)
      return newChild
  end
  -- ensure if we just have a string value that it is converted into a text
  -- node
  self~makeChildNode

  self~ownerDocument~insertingNode(this, replace)
  -- detach from any existing parent node
  oldParent = newChild~parentNode

  if oldParent \= .nil then do
      oldParent~removeChild(newNode)
  end

  newNode~ownerNode = this
  newNode~isOwned = .true

  firstChild = value
  -- no existing value, this case is easy
  if firstChild == .nil then do
      value = newChild
  end
  else do
      -- append operation?
      if refNode == .nil then do
          lastChild = self~lastChild
          lastChild~nextSibling = newNode
          newNode~previousSibling = lastChild
      end
      -- normal insertion
      else do
          if refChild == firstChild then do
              newNode~nextSibling = firstChild
              firstChild~previousSibling = newNode
              newNode~previousSibling = .nil
              value = newNode
          end
          else do
              previous = refNode~previousSibling
              newNode~nextSibling = refNode
              previous~nextSibling = newNode
              refNode~previousSibling = newNode
              newNode~previousSibling = previous
          end
      end
  end

  -- record the change
  self~changed

  self~ownerDocument~insertedNode(this, newNode, replace)

  return newChild

::method removeChild
  expose value
  use strict arg oldChild, replace = .false

  if value~isA(.string) then do
      return .nil
  end

  self~ownerDocument~removingNode(this, oldNode, replace)

  -- removing the first?
  if oldNode == value then do
      value = oldNode~nextSibling
      if value \= .nil then do
          value~previousSibling = .nil
      end
  end
  else do
      previous = oldNode~previousSibling
      next = oldNode~nextSibling
      previous~nextSibling = next
      if next \= .nil then do
          next~previousSibling = previous
      end
  end

  oldNode~ownerNode = self~ownerDocument
  oldNode~isOwned = .false
  oldNode~nextSibling = .nil
  oldNode~previousSibling = .nil

  self~changed

  ownerDocument~removedNode(this, replace)

  return oldNode

::method replaceChild
  use strict arg newChild, oldChild

  self~makeChildNode
  self~ownerDocument~replacingNode(self)
  self~insertBefore(newChild, oldChild, .true)
  if newChild \= oldChild then do
      self~removeChild(oldChild, true)
  end

  self~ownerDocument~repacingNode(self)
  return oldChild

-- NodeList methods
::attribute length GET
  expose value
  use strict arg

  if value~isA(.string) then do
      return 1
  end

  child = value
  length = 0
  do while child \= .nil
      length += 1
      child = child~nextSibling
  end

  return length

::method item
  expose value
  use strict arg index

  if value~isA(.String) then do
      if index \= 0 then do
          return .nil
      end
      self~makeChildNode
      return value
  end
  else do
      if index < 0 then do
          return .nil
      end
      node = value
      do i = 0 to index while node \= .nil
          node = node~nextSibling
      end
      return node
  end

::method isDerivedFrom
  use strict arg typeNamespace, typeName, derivationMethod
  return .false
