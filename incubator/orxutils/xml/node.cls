/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Node                                                                */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::CLASS 'Node' public
::CONSTANT ELEMENT_NODE 1
::CONSTANT ATTRIBUTE_NODE 2
::CONSTANT TEXT_NODE 3
::CONSTANT CDATA_SECTION_NODE 4
::CONSTANT ENTITY_REFERENCE_NODE 5
::CONSTANT ENTITY_NODE 6
::CONSTANT PROCESSING_INSTRUCTION_NODE 7
::CONSTANT COMMENT_NODE 8
::CONSTANT DOCUMENT_NODE 9
::CONSTANT DOCUMENT_TYPE_NODE 10
::CONSTANT DOCUMENT_FRAGMENT_NODE 11
::CONSTANT NOTATION_NODE 12

::METHOD init class
  expose ctr
  ctr = 0

::METHOD getId class private
  expose ctr
  ctr += 1
  return ctr


/*----------------------------------------------------------------------------*/
/* Method: init                                                               */
/* Description: instance initialization.                                      */
/*----------------------------------------------------------------------------*/

::METHOD init
  expose ownerNode childNodes nodeId
  use strict arg ownerNode
  nodeId = self~class~getId
  childNodes = .NodeList~new
  self~owned = .false   -- unowned until we are added as a child
  self~readonly = .false -- newly created nodes can be altered
  self~firstChild = .false -- not attached yet, so can't be first

::ATTRIBUTE nodeName GET
-- superclasses overrid
::ATTRIBUTE nodeValue GET
  return .nil
::ATTRIBUTE nodeValue SET
  return -- default behavior is to do nothing...superclasses override this
::ATTRIBUTE nodeType GET
-- superclasses override
::ATTRIBUTE parentNode GET
  return .nil
::ATTRIBUTE childNodes GET
  use strict arg
  -- the nodes implement the NodeList methods directly, so
  -- all we need to do is return ourself
  return self
-- superclasses override
::ATTRIBUTE firstChild GET
  return .nil
-- superclasses override
::ATTRIBUTE lastChild GET
  return .nil
-- superclasses override
::ATTRIBUTE previousSibling GET
  return .nil
-- superclasses override
::ATTRIBUTE nextSibling GET
  return .nil
-- superclasses override
::ATTRIBUTE attributes GET
  return .nil

::METHOD hasAttributes
  use strict arg
  return .false

::ATTRIBUTE ownerDocument GET
  expose ownerDocument ownerNode

  -- if we're a child node, then ask our owner for the information
  if self~owned then do
      return ownerNode~ownerDocument
  end
  else do
      -- return the owner directly
      return ownerNode
  end

::ATTRIBUTE ownerDocument SET PRIVATE
  expose ownerNode

  use strict arg doc
  if \self~owned then do
      ownerNode = doc
  end

::ATTRIBUTE namespaceURI GET
  use strict arg
  return .nil
::ATTRIBUTE prefix GET
  use strict arg
  return .nil
::ATTRIBUTE prefix SET
  -- this is an error by default
  .DomErrors~raiseError(.DomErrors~NAMESPACE_ERR)
::ATTRIBUTE localName GET
  use strict arg
  return .nil
::ATTRIBUTE baseURI GET
  use strict arg
  return .nil

-- private attributes used for the implementation
::ATTRIBUTE owned PRIVATE
::ATTRIBUTE readonly PRIVATE
::ATTRIBUTE isFirstChild PRIVATE

-- default implementations designed to be overridden
::METHOD hasChildNodes
  use strict arg
  return .false;

-- base cloning method
::METHOD cloneNode
  use strict arg deep = .false
  // make a copy of ourselves
  newNode = self~copy
  // neither owned or readonly
  newNode~owned = .false
  newNode~readOnly = .false

  return newNode

::METHOD isSupported
  use strict arg feature, version
  return .false

::ATTRIBUTE nodeValue SET
  use strict arg value
  raise syntax 93.963 -- not supported

::ATTRIBUTE prefix SET
  use strict arg value
  raise syntax 93.963 -- not supported

::METHOD insertBefore
  use strict arg newChild, refChild
  -- not implemented in the base node
  .DomErrors~raiseError(.DomErrors~Hierarchy_request_err)

::METHOD replaceChild
  use strict arg newChild, oldChild
  -- not implemented in the base node
  .DomErrors~raiseError(.DomErrors~Hierarchy_request_err)

::METHOD removeChild
  use strict arg oldChild
  -- not implemented in the base node
  .DomErrors~raiseError(.DomErrors~Not_found_err)

::METHOD appendChild
  use strict arg newChild
  return self~insertBefore(newChild, .nil)

::METHOD compareDocumentPosition
  use strict arg other
  raise syntax 93.963 -- not supported

::ATTRIBUTE textContent GET
  forward message("NODEVALUE")

::ATTRIBUTE textContent SET
  forward message("NODEVALUE=")

::METHOD isSameNode
  use strict arg other
  return self~identityHash = other~identityHash

::METHOD lookupPrefix
  use strict arg uri
  raise syntax 93.963 -- not supported

::METHOD isDefaultNamespace
  use strict arg uri
  raise syntax 93.963 -- not supported

::METHOD lookupNamespaceURI
  use strict arg prefix
  raise syntax 93.963 -- not supported

::METHOD isEqualNode
  use strict arg other

  if self == other then do
      return .true
  end

  if self~nodetype != other~nodetype then do
      return .false
  end

  if self~nodename != other~nodename then do
      return .false
  end

  if self~localname != other~localname then do
      return .false
  end

  if self~namespaceURI != other~namespaceURI then do
      return .false
  end

  if self~prefix != other~prefix then do
      return .false
  end

  if self~nodevalue != other~nodevalue then do
      return .false
  end

::METHOD getFeature
  use strict arg feature, version
  return .nil

::METHOD setUserData
  use strict arg key, date, handler
  raise syntax 93.963 -- not supported

::METHOD getUserData
  use strict arg key
  raise syntax 93.963 -- not supported


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Node                                                                */
/*        Private methods                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::attribute ctr class private
::attribute id       private

-- methods of NodeList that the node implements directly.  superclasses will
-- implement this more full

/*----------------------------------------------------------------------------*/
/* Method: item                                                               */
/* Description: get a list item.                                              */
/*----------------------------------------------------------------------------*/

::method item
  use strict arg index
  // always returns .nil
  return .nil

/*----------------------------------------------------------------------------*/
/* Method: length                                                             */
/* Description: get the number of items in the list.                          */
/*----------------------------------------------------------------------------*/
::method length
  use strict arg
  -- always returns 0

::method makearray
  use strict arg
  // just an empty array
  return .array~new(0)

::method "[]"
  return message("ITEM")

