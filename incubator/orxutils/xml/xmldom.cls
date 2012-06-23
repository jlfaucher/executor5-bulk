/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Description: An implementation of the Document Object Model in ooRexx.     */
/*                                                                            */
/* Copyright (c) 2006 Rexx Language Association. All rights reserved.         */
/*                                                                            */
/* This program and the accompanying materials are made available under       */
/* the terms of the Common Public License v1.0 which accompanies this         */
/* distribution. A copy is also available at the following address:           */
/* http://www.ibm.com/developerworks/oss/CPLv1.0.htm                          */
/*                                                                            */
/* Redistribution and use in source and binary forms, with or                 */
/* without modification, are permitted provided that the following            */
/* conditions are met:                                                        */
/*                                                                            */
/* Redistributions of source code must retain the above copyright             */
/* notice, this list of conditions and the following disclaimer.              */
/* Redistributions in binary form must reproduce the above copyright          */
/* notice, this list of conditions and the following disclaimer in            */
/* the documentation and/or other materials provided with the distribution.   */
/*                                                                            */
/* Neither the name of Rexx Language Association nor the names                */
/* of its contributors may be used to endorse or promote products             */
/* derived from this software without specific prior written permission.      */
/*                                                                            */
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS        */
/* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT          */
/* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS          */
/* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT   */
/* OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,      */
/* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED   */
/* TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,        */
/* OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY     */
/* OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING    */
/* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS         */
/* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.               */
/*                                                                            */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* NOTE!!!!!!!!!!!                                                            */
/* This class file does not yet work! It is under development!                */
/* It is VERY incomplete!                                                     */
/*                                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Section:  Abstract DOM interface class definitions                         */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: NodeList -- Abstract NodeList mixin                                 */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class "NodeList" public mixinclass Object

/*----------------------------------------------------------------------------*/
/* Method: item                                                               */
/* Description: get a list item.                                              */
/*----------------------------------------------------------------------------*/

::method item abstract

/*----------------------------------------------------------------------------*/
/* Method: length                                                             */
/* Description: get the number of items in the list.                          */
/*----------------------------------------------------------------------------*/
::method length abstract


-- some compatibility methods to make nodelist feel like a Rexx collection
::method makearray abstract
::method "[]"
  forward message("ITEM")
::method items
  forward message("LENGTH")

-- DOM Node interface class
::class "Node" public mixinclass Object
-- various type definitions
::constant ELEMENT_NODE 1
::constant ATTRIBUTE_NODE 2
::constant TEXT_NODE 3
::constant CDATA_SECTION_NODE 4
::constant ENTITY_REFERENCE_NODE 5
::constant ENTITY_NODE 6
::constant PROCESSING_INSTRUCTION_NODE 7
::constant COMMENT_NODE 8
::constant DOCUMENT_NODE 9
::constant DOCUMENT_TYPE_NODE 10
::constant DOCUMENT_FRAGMENT_NODE 11
::constant NOTATION_NODE 12

::method appendNode abstract
::method cloneNode abstract
::method attributes abstract
::method childNodes abstract
::method firstChild abstract
::method lastChild abstract
::method localName abstract
::method namespaceUri abstract
::method nextSibling abstract
::method nodeName abstract
::method nodeType abstract
::method nodeValue abstract
::method "nodeValue=" abstract
::method ownerDocument abstract
::method parentNode abstract
::method prefix abstract
::method "prefix=" abstract
::method previousSibling abstract
::method hasAttributes abstract
::method hasChildNodes abstract
::method insertBefore abstract
::method isSupported abstract
::method normalize abstract
::method removeChild abstract
::method replaceChild abstract

-- an abstract attribute interface definition
::class "Attr" mixinclass Node public
::method name abstract
::method ownerElement abstract
::method isSpecified abstract
::method value abstract
::method "value=" abstract

-- an abstract characterdata interface definition
::class "CharacterData" mixinclass Node public
::method appendData abstract
::method deleteData abstract
::method data abstract
::method "data=" abstract
::method length abstract
::method insertData abstract
::method replaceData abstract
::method substringData abstract

-- abstract text definition
::class "Text" mixinclass CharacterData public
::method splitText

-- abstract CDATA section definition
::class "CDATASection" mixinclass Text public
-- no additional methods defined on this interface

-- abstract Comment definition
::class "Comment" mixinclass CharacterData public
-- no additional methods defined on this interface

-- abstract document interface
::class "Document" mixinclass Node public
::method createAttribute abstract
::method createAttributeNS abstract
::method createCDATASection abstract
::method createComment abstract
::method createDocumentFragment abstract
::method createElement abstract
::method createElementNS abstract
::method createEntityReference abstract
::method createProcessinginstruction abstract
::method createTextNode abstract
::method doctype abstract
::method documentElement abstract
::method getElementByID abstract
::method getElementsByTagName abstract
::method getElementsByTagNameNS abstract
::method implementation abstract
::method version abstract
::method importNode abstract
::method "encoding=" abstract

-- abstract documentfragment definition
::class "DocumentFragment" mixinclass Node public
-- no additional methods defined on this interface

-- abstract documenttype definition
::class "DocumentType" mixinclass Node public
::method entities abstract
::method internalSubset abstract
::method name abstract
::method notations abstract
::method publicID abstract
::method systemID abstract

-- abstract DOMImplementation definiton
::class "DOMImplementation" mixinclass Object public
::method createDocument abstract
::method createDocumentType abstract
::method hasFeature abstract

-- abstract Element definition
::class "Element" mixinclass Node public
::method getAttribute abstract
::method getAttributeNode abstract
::method getAttributeNS abstract
::method getAttributeNodeNS abstract
::method getElementsByTagName abstract
::method getElementsByTagNameNS abstract
::method tagName abstract
::method "tagName=" abstract
::method hasAttribute abstract
::method hasAttributeNS abstract
::method removeAttribute abstract
::method removeAttributeNode abstract
::method removeAttributeNS abstract
::method setAttribute abstract
::method setAttributeNode abstract
::method setAttributeNodeNS abstract
::method setAttributeNS abstract

-- abstract Entity definition
::class "Entity" mixinclass Node public
::method encoding abstract
::method notationName abstract
::method systemId abstract
::method version abstract

-- abstract entityreference definition
::class "EntityReference" mixinclass Node public
-- no additional methods defined on this interface

-- abstract NameNodeMap definition
::class "NamedNodeMap" mixinclass Object public
::method length abstract
::method getNamedItem abstract
::method getNamedItemNS abstract
::method item abstract
::method removeNamedItem abstract
::method removeNamedItemNS abstract
::method setNamedItem abstract
::method setNamedItemNS abstract

-- abstract notation definition
::class "Notation" mixinclass Node public
::method publicID abstract
::method systemID abstract

-- abstract processinginstruction definition
::class "ProcessingInstruction" mixinclass Node public
::method data abstract
::method target abstract
::method "data=" abstract

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Section:  Concrete implementation of the DOM classes                       */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: NodeListImpl -- concrete NodeList class                             */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class "NodeListImpl" public inherit NodeList
::method init
  expose nodes
  -- we're a read-only list...the owning node returns the list, we work off
  -- of a snapshot copy
  use strict arg nodes = .nil
  -- this is an empty node list if no list given
  if nodes == .nil then do
      nodes = .array~new
  end
  else do
      -- make a copy of this
      nodes = nodes~copy
  end

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: NodeList                                                            */
/*        Private methods                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: NodeList                                                            */
/*        Public methods                                                      */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Method: item                                                               */
/* Description: get a list item.                                              */
/*----------------------------------------------------------------------------*/

::method item
  expose nodes
  use strict arg index
  -- nodelists start indexing at 0.  For xpath expresion compatibility, we should
  -- maintain that.
  return nodes[index + 1]   -- returns .nil for out of bounds

/*----------------------------------------------------------------------------*/
/* Method: length                                                             */
/* Description: get the number of items in the list.                          */
/*----------------------------------------------------------------------------*/
::method length
  expose nodes
  use strict arg
  return nodes~items

-- some compatibility methods to make nodelist feel like a Rexx collection
::method makearray
  expose nodes
  use strict arg
  return nodes~copy   -- make sure this is a copy

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: DeepNodeList                                                        */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class "DeepNodeList" subclass NodeListImpl public
::method init
  expose rootNode tagName changes nodes nsName

  use strict arg rootNode, tagName, nsName = .nil
  nodes = .Array~new
  changes = 0

-- lazy search for the matching nodes.  This only fills in the cache
-- as far as the last valid requested position
::method item
  expose rootNode changes nodes
  use strict arg index
  -- if the tree has changed, we need to restart this
  if rootNode~changes \= changes then do
      nodes = .array~new
      changes = rootNode~changes
  end

  currentSize = nodes~items
  if index < currentSize then do
      return nodes[index + 1]
  end
  else do
      -- we need to continue the traversal from the last node added
      if currentSize == 0 then do
          thisNode = rootNode
      end
      else do
          thisNode = nodes[currentSize]
      end
      -- keep adding up to the one we're looking for

      do while index > currentSize
          thisNode = self~nexMatchingElementAfter(thisNode)
          if thisNode \= .nil then do
              nodes~append(thisNode)
              currentSize += currentSize
          end
          else do
              -- no more nodes available
              leave
          end
      end
      -- this is either the one we want or .nil
      return thisNode
  end

::attribute length GET
  expose nodes
  use strict arg
  -- this forces everything to be initialized
  self~item(999999999)
  return nodes~items

::method makearray
  expose nodes
  use strict arg
  -- this forces everything to be initialized
  self~item(999999999)
  return nodes~copy

::method nextMatchingElementAfter private
  expose rootNode tagName nsName
  use arg current

  do while current \= nil
      -- go down to the first child if it has one
      if current~hasChildNodes then do
          current = current~firstChild
      end
      -- no children and we're at the rootnode, so we
      -- don't go up or to any siblings
      else if current == rootNode then do
          return .nil
      end
      -- now try for a sibling of our current position
      next = current~nextSibling
      -- if we found one, then this is our next candidate
      if next \= .nil  then do
          current = next
      end
      else do
          next = .nil
          -- now we step "up and to the right" for as many attempts as needed
          -- or utnil we pop back to the root node
          do while current \= rootNode
              current = current~parentNode
              next = current~nextSibling
              if next \= .nil then do
                  leave
              end
          end
          -- this is either a good node, or .nil
          current = next
      end
     -- we have a candidate node, now make sure it matches what we're looking for
     if current \= rootNode, current \= .nil, current~nodeType == .Node~ELEMENT_NODE then do
         -- no namespace checking?  We'll take any element node if the name is
         -- "*" or it matches directly
         if nsName == .nil  then do
             if tagName == "*" | current~tagName == tagName then do
                 return current
             end
         end
         -- namespace qualified (which might also be "*")
         else do
             -- wildcard match on the tagname, so just check the namespace name
             if tagName == "*" then do
                 -- wildcards on both, this is easy
                 if nsName == "*" then do
                     return current
                 end
                 else do
                     -- null string is a non-specific namepace request, which
                     -- matches only if the element does not have a namespace
                     if nsName == "" & current~namespaceURI == .nil then do
                         return current
                     end
                     -- requires an exact namespace match
                     else if nsName == current~namespaceURI then do
                         return current
                     end
                 end
             end
             -- non-wild card on the name
             else do
                 -- if we have a name match, then see if the namespace
                 -- name also matches
                 if current~localName == tagName then do
                     -- ok, the local name matches, the ns rules are the
                     -- same as above
                     if nsName == "*" then do
                         return current
                     end
                     else do
                         -- null string is a non-specific namepace request, which
                         -- matches only if the element does not have a namespace
                         if nsName == "" & current~namespaceURI == .nil then do
                             return current
                         end
                         -- requires an exact namespace match
                         else if nsName == current~namespaceURI then do
                             return current
                         end
                     end
                 end
             end
         end
     end
     -- if we get here, we had a candidate, but it was not a match
     -- just continue the tree walk
  end
  -- nothing found
  return .nil


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: NamedNodeMap                                                        */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class "NamedNodeMapImpl" public
::method init
  expose ownerNode attributes hasDefaults changed readonly
  use strict arg ownerNode
  attributes = .nil
  hasDefaults = .false
  changed = .false
  readonly = .false


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: NamedNodeMap                                                        */
/*        Private methods                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
::method findItemPoint private
  expose attributes
  use arg item

  if attributes == .nil then do
      return .nil
  end

  s = attributes~supplier
  do while s~available
      attr = s~item
      if attr == item then do
          return s~index
      end
  end

  return .nil


::method findNamePoint private
  expose attributes
  use arg name

  if attributes == .nil then do
      return .nil
  end

  s = attributes~supplier
  do while s~available
      attr = s~item
      if attr~nodeName == name then do
          return s~index
      end
  end

  return .nil

 -- locate the index of a given namespace/localname pair
::method findNamePointNS private
  expose attributes
  use arg namespace, name

  if attributes == .nil then do
      return .nil
  end

  s = attributes~supplier
  do while s~available
      attr = s~item
      if namespace == .nil then do
          if attr~namespaceURI == .nil & name == attr~localName then do
              return s~index
          end

          if attr~localName == .nil & name == attr~nodeName then do
              return s~index
          end
      end
      else do
          if attr~namespaceURI == namespace & attr~localName == name then do
              return s~index
          end
      end
      s~next
  end

  return .nil

::attribute ownerNode
::attribute hasDefaults
::attribute changed
::attribute readonly


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: NamedNodeMap                                                        */
/*        Public methods                                                      */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Method: getNamedItem                                                       */
/* Description: get a named item.                                             */
/*----------------------------------------------------------------------------*/

::method getNamedItem
  expose attributes
  use strict arg name

  if attributes == .nil then do
      return .nil
  end

  do attribute over attributes
      if attribute~name == name then do
          return attribute
      end
  end
  return .nil


/*----------------------------------------------------------------------------*/
/* Method: getNamedItemNS                                                     */
/* Description: get a named item by namespace                                 */
/*----------------------------------------------------------------------------*/

::method getNamedItemNS
  expose attributes
  use strict arg namespace, name

  if attributes == .nil then do
      return .nil
  end

  do attribute over attributes
      if attribute~localname == name & attribute~namespaceURI == namespace then do
          return attribute
      end
  end
  return .nil


/*----------------------------------------------------------------------------*/
/* Method: setNamedItem                                                       */
/* Description: set a named item.                                             */
/*----------------------------------------------------------------------------*/

::method setNamedItem
  expose attributes
  use strict arg node

  index = self~findNamePoint(node~nodeName)
  previous = .nil

  if index \= .nil then do
      previous = attributes[index]
      attributes[index] = node
  end
  else do
      if attributes == .nil then do
          attributes = .list~new
      end
      attributes~append(node)
  end

  return previous


/*----------------------------------------------------------------------------*/
/* Method: setNamedItemNS                                                     */
/* Description: set a named item.                                             */
/*----------------------------------------------------------------------------*/

::method setNamedItemNS
  expose attributes
  use strict arg node

  index = self~findNamePointNS(node~namespaceURI, node~localName)
  previous = .nil

  if index \= .nil then do
      previous = attributes[index]
      attributes[index] = node
  end
  else do
      -- we try again just based on the nodeName
      index = self~findNamePoint(node~nodeName)
      if index > .nil then do
          previous = attributes[index]
          attributes[index] = node
      end
      else do
          if attributes == .nil then do
              attributes = .list~new
          end
          attributes~append(node)
      end
  end

  return previous


/*----------------------------------------------------------------------------*/
/* Method: removeNamedItem                                                    */
/* Description: remove a named item.                                          */
/*----------------------------------------------------------------------------*/

::method removeNamedItem
  expose attributes
  use strict arg name

  index = self~findNamePoint(name)
  previous = .nil

  if index \= .nil then do
      previous = attributes[index]
      attributes~remove(index)
  end
  return previous


/*----------------------------------------------------------------------------*/
/* Method: removeNamedItemNS                                                  */
/* Description: remove a named item.                                          */
/*----------------------------------------------------------------------------*/

::method removeNamedItemNS
  expose attributes
  use strict arg namespace, name

  index = self~findNamePointNS(namespace, name)
  previous = .nil

  if index \= .nil then do
      previous = attributes[index]
      attributes~remove(index)
  end
  return previous

/*----------------------------------------------------------------------------*/
/* Method: cloneMap                                                           */
/* Description: perform a deep copy of this map object                        */
/*----------------------------------------------------------------------------*/
::method cloneMap
  expose attributes
  use strict arg owner
  newMap = .NamedNodeMap(owner)
  newMap~cloneContent(attributes)

/*----------------------------------------------------------------------------*/
/* Method: cloneContent                                                       */
/* Description: initialize the content from another map                       */
/*----------------------------------------------------------------------------*/
::method cloneContent
  expose attributes
  use arg source
  if source \= .nil then do
      attributes = .list~new
      do attr over source
          attributes~append(attr~cloneNode(.true))
      end
  end


/*----------------------------------------------------------------------------*/
/* Method: item                                                               */
/* Description: return the named item.                                        */
/*----------------------------------------------------------------------------*/

::method item
  expose attributes
  use strict arg position
  if attributes == .nil then do
      return .nil
  end
  else do
      return attributes~makearray[position + 1]
  end


/*----------------------------------------------------------------------------*/
/* Method: length                                                             */
/* Description: return the number of named items.                             */
/*----------------------------------------------------------------------------*/

::method length
  expose attributes
  use strict arg
  if attributes \== .nil then do
      return attributes~items
  end
  else do
      return 0;
  end

::attribute attributes PRIVATE

-- some convience methods for Rexx collection appearances
::method makearray
  expose attributes
  use strict arg
  return attributes~makearray -- make sure this is a copy

::method "[]"
  forward message("ITEM")

::method items
  forward message("LENGTH")


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: AttributeMap -- an implementation of NamedNodeMap that can deal     */
/* with member ownership issues                                               */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class "AttributeMap" subclass NamedNodeMapImpl public
::method init
  expose hasDefaults
  use strict arg ownerNode, defaults = .nil

  self~init:super(ownerNode)
  hasDefaults = .false
  -- if we have a defaults set, clone it and if we really added
  -- something, marks us as having defaults
  if defaults \= .nil then do
      self~cloneContent(defaults)
      if self~attributes \= .nil  then do
          hasDefaults = .true
      end
  end

::method setNamedItem
  use strict arg attribute
  -- replacing an attribute with itself does nothing
  if attribute~isOwned then do
      return attribute
  end

  attribute~ownerNode = self~ownerNode
  attribute~isOwned = .true

  index = self~findNamePoint(attribute~nodeName, 0)
  attributes = self~attributes
  previous = .nil
  if index \= .nil then do
      previous = attributes[i]
      attributes[i] = attribute
      previous~ownerNode = self~ownerNode~ownerDocument
      previous~isOwned = .false
      previous~isSpecified = .true
  end
  else do
      if attributes = .nil then do
          attributes = .list~new
          self~attributes = attributes
      end
      attributes~append(attribute)
  end

  self~ownerNode~ownerDocument~setAttrNode(attribute, previous)
  return previous

::method setNamedItemNS
  use strict arg attribute
  -- replacing an attribute with itself does nothing
  if attribute~isOwned then do
      return attribute
  end

  attribute~ownerNode = self~ownerNode
  attribute~isOwned = .true

  index = self~findNamePointNS(attribute~namespaceURI, attribute~localName)
  attributes = self~attributes
  previous = .nil
  if index \= .nil then do
      previous = attributes[index]
      attributes[index] = attribute
      previous~ownerNode = self~ownerNode~ownerDocument
      previous~isOwned = .false
      previous~isSpecified = .true
  end
  else do
      if attributes = .nil then do
          attributes = .list~new
          self~attributes = attributes
      end
      attributes~append(attribute)
  end

  self~ownerNode~ownerDocument~setAttrNode(attribute, previous)
  return previous

::method removeNamedItem
  use strict arg name

  index = self~findNamePoint(name)
  if index == .nil then do
      return .nil
  end

  return self~remove(self~attributes[index], index, .true)

::method removeNamedItemNS
  use strict arg namespaceURI, name

  index = self~findNamePointNS(namespaceURI, name)
  if index == .nil then do
      return .nil
  end

  return self~remove(self~attributes[index], index, .true)

::method removeItem
  use strict arg attr, addDefault

  index = self~findItemPoint(attr)
  if index == .nil then do
      return .nil
  end

  return self~remove(attr, index, addDefault)

::method remove private
  use strict arg attr, index, addDefault = .false

  ownerDocument = self~ownerNode~ownerDocument
  name = attr~nodeName
  if attr~isId then do
      self~ownerDocument~removeIdentifier(attr~value)
  end

  attributes = self~attributes

  setdefault = .false
  -- do we have default attributes that we might need to revert to?
  if self~hasDefaults & addDefault then do
      defaults = ownerNode~defaultAttributes
      if defaults \= .nil then do
          defaultAttr = defaults~getNamedItem(name)
          if defaultAttr \= .nil then do
              newAttr = defaultAttr~cloneNode(.true)
              -- the namespace uri comes from the deleted node,
              -- not the default source value if the default
              -- does not have a local name
              if newAttr~localName \== .nil then do
                  newAttr~namespaceURI = attr~namespaceURI
              end
              newAttr~ownerNode = self~ownerNode
              newAttr~isOwned = .true
              -- mark this as a default value
              newAttr~isSpecified = .false
              attributes[index] = newAttr
              setdefault = .true
              -- if this is the id attribute, make sure the document knows
              -- about this mapping
              if attr~isId then do
                  ownerDocument~putIdentifier(newAttr~nodeValue, ownerNode)
              end
          end
      end
  end
  -- if we didn't end up setting a default, remove the node
  if \setDefault then do
      attributes~remove(index)
  end
  -- detach from usage
  attr~ownerNode = .nil
  attr~isOwned = .false
  attr~isSpecified = .true
  attr~idAttribute = .false

  -- notify the document
  ownerDocument~removedAttrNode(attr, self~ownerNode, name)
  return attr

::method cloneContent
  use strict arg source
  srcnodes = source~attributes
  if srcnodes \= .nil then do
     self~attributes = .List~new
     do node over srcnodes
         newNode = node~cloneNode(true)
         newNode~ownerNode = self~ownerNode
         newNode~isOwned = .true
         self~attributes~append(newNode)
     end
  end

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Node                                                                */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::CLASS "NodeImpl" public inherit Node

-- initialize a counter to give new nodes a unique id
::method init class
  expose ctr
  ctr = 0

-- get the next id for new nodes
::method getId class private
  expose ctr
  ctr += 1
  return ctr


/*----------------------------------------------------------------------------*/
/* Method: init                                                               */
/* Description: instance initialization.                                      */
/*----------------------------------------------------------------------------*/

::method init
  expose ownerNode childNodes nodeId
  use strict arg ownerNode = .nil
  nodeId = self~class~getId
  childNodes = .NodeListImpl~new
  self~isowned = .false  -- unowned until we are added as a child
  self~readonly = .false -- newly created nodes can be altered
  self~isFirstChild = .false -- not the first child until added to something.
  self~isOwned = .false      -- not owned until attached
  self~isIgnorableWhitespace = .false  -- only text nodes have this

::attribute isIgnorableWhitespace

::attribute nodeName GET
-- superclasses override
::attribute nodeValue GET
  return .nil
::attribute nodeValue SET
  return -- default behavior is to do nothing...superclasses override this
::attribute nodeType GET
-- superclasses override
::attribute parentNode GET
  return .nil
::attribute childNodes GET
  use strict arg
  -- the nodes implement the NodeList methods directly, so
  -- all we need to do is return ourself
  return self
::attribute firstChild GET
  return .nil
-- superclasses override
::attribute lastChild GET
  return .nil
-- superclasses override
::attribute previousSibling GET
-- superclasses override
::attribute nextSibling GET
-- superclasses override
::attribute attributes GET
  return .nil

::method hasAttributes
  use strict arg
  return .false

::attribute ownerNode

::attribute ownerDocument GET
  expose ownerDocument ownerNode

  -- if we're a child node, then ask our owner for the information
  if self~isowned then do
      return ownerNode~ownerDocument
  end
  else do
      -- return the owner directly
      return ownerNode
  end

::attribute ownerDocument SET PRIVATE
  expose ownerNode

  use strict arg doc
  if \self~isowned then do
      ownerNode = doc
  end

::attribute namespaceURI GET
  use strict arg
  return .nil
::attribute prefix GET
  use strict arg
  return .nil
::attribute prefix SET
  -- this is an error by default
  .DomErrors~raiseError(.DomErrors~NAMESPACE_ERR)
::attribute localName GET
  use strict arg
  return .nil
::attribute baseURI GET
  use strict arg
  return .nil

-- private attributes used for the implementation
::attribute isowned
::attribute readonly
::attribute isFirstChild

-- default implementations designed to be overridden
::method hasChildNodes
  use strict arg
  return .false;

-- base cloning method
::method cloneNode
  use strict arg deep = .false
  -- make a copy of ourselves
  newNode = self~copy
  -- neither owned or readonly
  newNode~isowned = .false
  newNode~readOnly = .false

  return newNode

::method isSupported
  use strict arg feature, version
  return .false

::method insertBefore
  use strict arg newChild, refChild
  -- not implemented in the base node
  .DomErrors~raiseError(.DomErrors~Hierarchy_request_err)

::method replaceChild
  use strict arg newChild, oldChild
  -- not implemented in the base node
  .DomErrors~raiseError(.DomErrors~Hierarchy_request_err)

::method removeChild
  use strict arg oldChild
  -- not implemented in the base node
  .DomErrors~raiseError(.DomErrors~Not_found_err)

::method appendChild
  use strict arg newChild
  return self~insertBefore(newChild, .nil)

::method compareDocumentPosition
  use strict arg other
  raise syntax 93.963 -- not supported

::attribute textContent GET
  forward message("NODEVALUE")

::attribute textContent SET
  forward message("NODEVALUE=")

::method isSameNode
  use strict arg other
  return self~identityHash = other~identityHash

::method lookupPrefix
  use strict arg uri
  raise syntax 93.963 -- not supported

::method isDefaultNamespace
  use strict arg uri
  raise syntax 93.963 -- not supported

::method lookupNamespaceURI
  use strict arg prefix
  raise syntax 93.963 -- not supported

::method isEqualNode
  use strict arg other

  if self == other then do
      return .true
  end

  if self~nodetype \= other~nodetype then do
      return .false
  end

  if self~nodename \= other~nodename then do
      return .false
  end

  if self~localname \= other~localname then do
      return .false
  end

  if self~namespaceURI \= other~namespaceURI then do
      return .false
  end

  if self~prefix \= other~prefix then do
      return .false
  end

  if self~nodevalue \= other~nodevalue then do
      return .false
  end

::method getFeature
  use strict arg feature, version
  return .nil

::method setUserData
  use strict arg key, date, handler
  raise syntax 93.963 -- not supported

::method getUserData
  use strict arg key
  raise syntax 93.963 -- not supported

-- handle mutation events
::method changed
  self~ownerdocument~changed

::method changes
  return self~ownerdocument~changes


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
  -- always returns .nil
  return .nil

/*----------------------------------------------------------------------------*/
/* Method: length                                                             */
/* Description: get the number of items in the list.                          */
/*----------------------------------------------------------------------------*/
::method length
  use strict arg
  -- always returns 0
  return 0

-- some useful compatibility items
::method makearray
  use strict arg
  -- just an empty array here
  return .array~new(0)

::method "[]"
  forward message("ITEM")

::method items
  forward message("LENGTH")

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: ChildNode base type for nodes that can be children of other nodes   */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class "ChildNode" subclass NodeImpl public
::method init
  expose previousSibling nextSibling
  forward class(super) continue

  previousSibling = .nil
  nextSibling = .nil


::method cloneNode
  use strict arg deep = .false

  newNode = self~init:super(deep)
  -- detach the new instance from the context
  newNode~previousSibling = .nil
  newNode~nextSibling = .nil
  newNode~isFirstChild = .false

  return newNode

::attribute parentNode GET
  if self~isOwned then do
      return self~ownerNode
  end
  else do
      return .nil
  end

::attribute nextSibling GET
::attribute nextSibling SET PRIVATE

::attribute previousSibling GET
::attribute previousSibling SET PRIVATE


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: ParentNode base type for nodes that can be parents of other nodes   */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class "ParentNode" subclass ChildNode public
::method init
  expose firstChild lastChild ownerDocument childNodes
  forward class(super) continue

  -- set this explicitly
  use strict arg ownerDocument = .nil
  self~clearChildren

::method clearChildren private
  expose firstChild lastChild childNodes
  firstChild = .nil
  lastChild = .nil
  childNodes = 0

::method cloneNode
  expose firstChild
  use strict arg deep = .false

  newNode = self~cloneNode:super(deep)
  newNode~clearChildNodes
  -- if this is a deep copy, then we need to clone all of the children too
  if deep then do
      child = firstChild
      do while child \= .nil
          newNode~appendNode(child~cloneNode(.true))
          child = child.nextSibling
      end
  end
  return newNode

::attribute ownerDocument GET
::attribute ownerDocument SET private
  expose ownerDocument firstChild
  use strict arg doc
  -- we need to set this at the higher levels too
  self~"OWNERDOCUMENT":super(doc)
  -- set this in each of the children too
  child = firstChild
  do while child \= .nil
      child~ownerDocument = doc
      child = child.nextSibling
  end

-- returns true if the node currently has child nodes
::method hasChildNodes
  expose firstChild
  use strict arg
  return firstChild \= .nil

::attribute firstChild GET
::attribute lastChild GET

::method insertBefore
  expose firstChild lastChild ownerDocument childNodes
  use arg newChild, refChild = .nil, replace = .false

  -- this case is really a no-op, but we need to go through the steps
  -- in case we need to signal events.
  if newChild == refChild then do
      refChild = refChild~nextSibling
      self~removeChild(newChild)
      self~insertBefore(newChild, refChild)
      return newChild
  end

  -- inform the owner that this is happening
  ownerDocument~insertingNode(self, replace)

  -- make sure we've detached this node from any previous
  -- parent node.
  oldParent = newChild~parentNode
  if oldParent \= .nil then do
      oldParent~removeChild(newChild)
  end

  -- set the new owner
  newChild~ownerNode = self
  newChild~isOwned = .true

  -- Now detach before and after

  -- first added child is easy
  if firstChild == .nil then do
      firstChild = newChild
      lastChild = newChild
  end
  -- .nil is an append
  else if refChild == .nil then do
      lastChild~nextSibling = newChild
      newChild~previousSibling = lastChild
      newChild~nextSibling = .nil
      lastChild = newChild
  end
  -- normal insertion
  else do
      -- insertion at the beginning, need to adjust
      if refChild == firstChild then do
          newChild~nextSibling = firstChild
          newChild~previousSibling = .nil
          firstChild~previousSibling = newChild
          firstChild = newChild
      end
      -- insertion in the middle
      else do
          previousNode = refChild~previousSibling
          previousNode~nextSibling = newChild
          newChild~previousSibling = previousNode
          newChild~nextSibling = refChild
          refChild~previousSibling = newChild
      end
  end

  -- bump the count of nodes
  childNodes += 1

  -- broadcast a change event
  self~changed
  -- inform the document of this update
  ownerDocument~insertedNode(self, newChild, replace)
  return newChild

::method removeChild
  expose firstChild lastChild ownerDocument childNodes
  use strict arg oldChild, replace = .false

  ownerDocument~removingNode(self, oldChild, replace)

  -- removing the first child
  if oldChild == firstChild then do
      firstChild = firstChild~nextSibling
      if firstChild \= .nil then do
          firstChild~previousSibling = .nil
      end
      -- if this was the only child, then clear out everything
      if lastChild == oldChild then do
          lastChild = .nil
      end
  end
  else do
      previous = oldChild~previousSibling
      next = oldChild~nextSibling
      previous~nextSibling = next
      -- this could be the last child, so we might have to update that
      if next == .nil then do
          lastChild = previous
      end
      else do
          -- close up the chaing
          next~previousSibling = previous
      end
  end

  childNodes -= 1

  oldChild~ownerNode = ownerDocument
  oldChild~isOwned = .false
  oldChild~nextSibling = .nil
  oldChild~previousSibling = .nil

  -- note the change update
  self~changed

  ownerDocument~removedNode(self, replace)
  return oldChild

::method replaceChild
  expose ownerDocument
  use strict arg newChild, oldChild

  self~insertBefore(newChild, oldChild, .true)
  if newChild \== oldChild then do
      self~removeChild(oldChild, .true)
  end

  ownerDocument~replacedNode(self)
  return oldChild

::attribute textContent GET
  use strict arg

  child = self~firstChild
  if child \== .nil then do
      next = child~nextSibling
      if next == .nil then do
          if self~hasTextContent(child) then do
              return child~textContent
          end
          else do
              return ""
          end
      end
      else do
          buffer = .mutablebuffer~new
          self~buildTextContent(buffer)
          return buffer~string
      end
  end
  return ""

-- private method to build up text content from multiple nodes
::method buildTextContent private
  use arg buffer
  child = self~firstChild
  do while child \= .nil
      if self~hasTextContent(child) then do
          content = child~nodeValue
          if nodeValue \= .nil then do
              buffer~append(content)
          end
      end
      child = child~nextSibling
  end

::method hasTextContent private
  use arg child
  type = child~nodeType
  if type \= .Node~COMMENT_NODE & -
      type \= .Node~PROCESSING_INSTRUCTION_NODE & -
      type \= .Node~TEXT_NODE then do
      return .false
  end

  return \child~isIgnorableWhitespace

::attribute textContent SET
  use strict arg text
  child = self~firstChild
  do while child \= .nil
      self~removeChild(child)
      child = self~firstChild
  end

  -- create a text node and append
  if text \= .nil, text \== "" then do
      self~appendChild(self~ownerDocument~createTextNode(text))
  end

-- overrides for the NodeList methods
::attribute length GET
  expose childNodes
  use strict arg
  return childNodes

::attribute item GET
  expose firstChild childNodes
  use strict arg index

  if index < 0 || index >= childNodes then do
      return .nil
  end

  child = firstChild
  loop index
      child = child~nextSibling
  end

  return child

::method makearray
  expose firstChild childNodes
  use strict arg

  array = .array~new(childNodes)
  child = firstChild
  do i = 1 while child \= .nil
      array[i] = child
      child = child~nextSibling
  end

  return array


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: DocumentFragment                                                    */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
::class "DocumentFragmentImpl" public subclass ParentNode

::attribute nodeType GET
  return .Node~DOCUMENT_FRAGMENT_NODE

::attribute nodeName GET
  return "#document-fragment"

::method normalize
  if self~isNormalized then do
      return
  end

  kid = self~firstChild

  do while kid \= .nil
      next = kid~nextSibling

      if kid~nodeType == .Node~TEXT_NODE then do
          if next \= .nil & next~nodeType == .Node~TEXT_NODE then do
              kid~appendData(next~nodeValue)
              self~removeChild(next)
              -- back up, we might have more nodes to collapse
              next = kid
          end
          else do
              if kid~nodeValue == .nil | kid~nodeValue == "" then do
                  self~removeChild(kid)
              end
          end
      end
      kid~normalize()
      kid = next
  end

  self~isNormalized = .true

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Document                                                            */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
::class "DocumentImpl" subclass ParentNode public
::method init
  expose iterators ranges eventListeners grammarAccess doctype hasMutationEvents
  use arg doctype = .nil, grammarAccess = .false

  self~init:super
  iterators = .nil
  ranges = .nil
  eventListeners = .nil
  hasMutationEvents = .false

::method cloneNode
  use strict arg deep = .false

  newDoc = self~class~new
  self~cloneDocument(newDoc, deep)
  return newDoc

::method implementation
  -- this is a singleton
  return .ooRexxDOM~implementation

::method createNodeIterator
  expose iterators
  use strict arg root, whatToShow, filter, entityReferenceExpansion = .true

  iterator = .NodeIterator~new(self, root, whatToShow, filter, entityReferenceExpansion)

  if iterators = .nil  then do
      iterators = .list~new
  end

  iterators~append(iterator)
  return iterator

::method createTreeWalker
  use strict arg root, whatToShow, filter, entityReferenceExpansion = .true
  return .TreeWalker~new(root, whatToShow, filter, entityReferenceExpansion)

::method removeNodeIterator
  expose iterators
  use strict arg nodeIterator

  if nodeIterator == .nil | iterators == .nil then do
      return
  end

  iterators~remove(iterator)

::method createRange
  expose ranges
  if ranges == .nil then do
      ranges = .list~new
  end

  range = .range~new(self)
  ranges~append(range)
  return range

::method removeRange
  expose ranges
  use strict arg range
  if range == .nil | ranges == .nil then do
      return
  end
  ranges~removeItem(range)

::method replacedText
  expose ranges
  use strict arg node

  if ranges \= .nil then do
      do range over ranges
          range~receiveReplacedText(node)
      end
  end

::method deletedText
  expose ranges
  use strict arg node, offset, count

  if ranges \= .nil then do
      do range over ranges
          range~receiveDeletedText(node, offset, count)
      end
  end

::method insertedText
  expose ranges
  use strict arg node, offset, count

  if ranges \= .nil then do
      do range over ranges
          range~receiveInsertedText(node, offset, count)
      end
  end

::method splitData
  expose ranges
  use strict arg node, newNode, offset

  if ranges \= .nil then do
      do range over ranges
          range~receiveSplitData(node, newNode, offset)
      end
  end


::method createEvent
  use strict arg type
  if type~caselessEquals("Events") then do
      return .DOMEvent~new
  end
  else if type~caselessEquals("MutationEvent") then do
      return .MutationEvent~new
  end
  else do
      .DomErrors~raiseError(.DomErrors~NOT_SUPPORTED_ERR)
  end

::attribute hasMutationEvents

::method setEventListeners
  expose eventListeners
  use strict arg node, listeners

  if eventListeners == .nil then do
      eventListeners = new table
  end

  if listeners == .nil then do
      eventListeners~remove(node)
      if eventListeners~isEmpty then do
          self~hasMutationEvents = .false
      end
  end
  else do
      eventListeners[node] = listeners
      self~hasMutationEvents = .true
  end

::method getEventListeners
  expose eventListeners
  use strict arg node
  if eventListeners == .nil then do
      return .nil
  end
  return eventListeners[node]

-- EventTarget support

::method addEventListener
  use strict arg node, type, listener, useCapture

  if type == .nil | type == "" | listener == .nil then do
      return
  end

  self~removeEventListener(node, type, listener, useCapture)

  nodeListeners = self~getEventListeners(node)

  if nodeListeners == .nil then do
      nodeListeners = .List~new
      self~setEventListeners(node, nodeListeners)
  end

  nodeListeners~append(.EventListener~new(type, listener, useCapture))

  capture = self~lookupCapture(type)
  capture~addListener(useCapture)

::method removeEventListener
  use strict arg node, type, listener, useCapture

  if type == .nil | type == "" | listener == .nil then do
      return
  end
  nodeListeners = self~getEventListeners(node)
  if nodeListeners == .nil then do
      return
  end

  do le over nodeListeners
      if le~equals(type, listener, useCapture) then do
          nodeListeners~removeItem(le)
          if nodeListeners~isEmpty then do
              self~eventListeners(node, .nil)
          end
          return
      end
  end

::method copyEventListeners
  use strict arg source, target

  nodeListeners = self~getEventListeners(source)
  if nodeListeners == .nil then do
      return
  end

  self~setEventListeners(target, nodeListeners~copy)




/**
 * Introduced in DOM Level 2. <p>
 * Distribution engine for DOM Level 2 Events.
 * <p>
 * Event propagation runs as follows:
 * <ol>
 * <li>Event is dispatched to a particular target node, which invokes
 *   this code. Note that the event's stopPropagation flag is
 *   cleared when dispatch begins; thereafter, if it has
 *   been set before processing of a node commences, we instead
 *   immediately advance to the DEFAULT phase.
 * <li>The node's ancestors are established as destinations for events.
 *   For capture and bubble purposes, node ancestry is determined at
 *   the time dispatch starts. If an event handler alters the document
 *   tree, that does not change which nodes will be informed of the event.
 * <li>CAPTURING_PHASE: Ancestors are scanned, root to target, for
 *   Capturing listeners. If found, they are invoked (see below).
 * <li>AT_TARGET:
 *   Event is dispatched to NON-CAPTURING listeners on the
 *   target node. Note that capturing listeners on this node are _not_
 *   invoked.
 * <li>BUBBLING_PHASE: Ancestors are scanned, target to root, for
 *   non-capturing listeners.
 * <li>Default processing: Some DOMs have default behaviors bound to
 *   specific nodes. If this DOM does, and if the event's preventDefault
 *   flag has not been set, we now return to the target node and process
 *   its default handler for this event, if any.
 * </ol>
 * <p>
 * Note that registration of handlers during processing of an event does
 * not take effect during this phase of this event; they will not be called
 * until the next time this node is visited by dispatchEvent. On the other
 * hand, removals take effect immediately.
 * <p>
 * If an event handler itself causes events to be dispatched, they are
 * processed synchronously, before processing resumes
 * on the event which triggered them. Please be aware that this may
 * result in events arriving at listeners "out of order" relative
 * to the actual sequence of requests.
 * <p>
 * Note that our implementation resets the event's stop/prevent flags
 * when dispatch begins.
 * I believe the DOM's intent is that event objects be redispatchable,
 * though it isn't stated in those terms.
 * @param node node to dispatch to
 * @param event the event object to be dispatched to
 *              registered EventListeners
 * @return true if the event's <code>preventDefault()</code>
 *              method was invoked by an EventListener; otherwise false.
*/

::method dispatchEvent private
  use strict arg node, event

  -- don't do anything if there are no listeners
  capture = lookupCapture(event~type)
  if capture~total == 0 then do
      return event~preventDefault
  end

  -- initialize the events dispatch status
  event~target = node
  event~stopPropagation = .false
  event~preventDefault = .false

  -- capture the event parentage chain
  parents = .array~new
  previous = node
  parent = previous~parentNode
  loop while parent \= .nil
      parents~append(parent)
      previous = parent
      parent = parent~parentNode
  end

  -- capturing phase
  if capture~captures > 0 then do
      event~eventPhase = .Event~CAPTURING_PHASE
      do parentNode over parents
          if event~stopPropagation then do
              leave
          end

          event~currentTarget = parentNode
          listeners = self~getEventListeners(parentNode)
          if nodeListeners \= .nil then do
              -- NB:  Do over takes a snapshot copy, so
              -- this is thread safe
              do listener over nodeListeners
                  if listener~matches(event~type, .true) then do
                      listener~invoke(event)
                  end
              end
          end
      end
  end

  -- both AT_TARGET and BUGGLE use non-capturing listeners
  if capture~bubbles > 0 then do
      event~eventPhase = .Event~AT_TARGET
      event~currentTarget = node
      nodeListeners = self~getEventListeners(node)
      if \event~stopPropagation & nodeListeners \= .nil then do
          -- NB:  Do over takes a snapshot copy, so
          -- this is thread safe
          do listener over nodeListeners
              if listener~matches(event~type, .false) then do
                  listener~invoke(event)
              end
          end
      end

      -- BUBBLING_PHASE: Ancestors are scanned, target to root, for
      -- non-capturing listeners. If the event's preventBubbling flag
      -- has been set before processing of a node commences, we
      -- instead immediately advance to the default phase.
      -- Note that not all events bubble.
      if event~bubbles then do
          event~eventPhase = .Event~BUBBLING_PHASE
          do parent over parents
              if event~stopPropagation then do
                  leave
              end

              event~currentTarget = parent
              nodeListeners = self~getEventListeners(node)
              if nodeListeners \= .nil then do
                  do listener over nodeListener
                      if listener~matches(event~type, .false) then do
                          listener~invoke(event)
                      end
                  end
              end
          end
      end
  end

  return event~preventDefault

::method lookupCapture PRIVATE
  expose captures
  use strict arg type
  if caputures == .nil then do
      captures = .directory~new
  end

  capture = captures[type]
  if capture == .nil then do
      capture = .EventTracer~new
      captures[type] = capture
  end

  return capture

::method dispatchEventToSubtree private
  use strict arg node, event
  if node~nodeType == .Node~ELEMENT_NODE then do
      do attr over node~attributes
          self~dispatchinggEventToSubtree(attr, event)
      end
  end
  self~dispatchingEventToSubtree(node~firstChild(, event))

::method dispatchingEventToSubtree
  use strict arg node, event

  if node == .nil then do
      return
  end

  node~dispatchEvent(event)
  if node~nodeType == .Node~ELEMENT_NODE then do
      do attr over node~attributes
          self~dispatchingEventToSubtree(attr, event)
      end
  end

  self~dispatchingEventToSubtree(node~firstChild, event)
  self~dispatchingEventToSubtree(node~nextSibling, event)

::method dispatchAggregateEvents
  use strict arg node, enclosingAttr = .nil, oldvalue = .nil, change = 0

  owner = .nil
  if enclosingAttr \= .nil then do
      capture = self~lookupCapture(.MutationEvent~DOM_ATTR_MODIFIED)
      owner = enclosingAttr~ownerElement
      if capture~total then do
          if owner \= .nil then do
              me = .MutationEvent~new
              me.initMutationEvent(.MutationEvent~DOM_ATTR_MODIFIED, .true. -
                  .false, enclosingAttr, oldvalue, enclosingAttr~nodeValue, -
                  enclosing~nodeName, change)
              owner~dispatchEvent(me)
          end
      end
  end

  capture = self~lookupCapture(.MutationEvent~DOM_SUBTREE_MODIFIED)
  if capture~total > 0 then do
      me = .MutationEvent~new
      me.initMutationEvent(.MutationEvent~DOM_SUBTREE_MODIFIED, .true. -
          .false, .nil, .nil, .nil, .nil, 0)
      owner~dispatchEvent(me)

      if enclosingAttr \= .nil then do
          self~dispatchEvent(enclosingAttr, me)
          if owner \= .nil then do
              self~dispatchEvent(owner, me)
          end
      end
      else do
          self~dispatchEvent(node, me)
      end
  end

::method saveEnclosingAttr
  expose savedEnclosingAttr
  use strict arg node

  savedEnclosingAttr = .nil

  capture = self~captureLookup(.MutationEvent~DOM_ATTR_MODIFIED)
  if capture~total > 0 then do
      eventAncestor = .nil
      do forever
          if eventAncestor == .nil then do
              return
          end
          type = eventAncestor~nodeType
          if type == .Node~ATTRIBUTE_NODe then do
              retval = .EnclosingAttr(eventAncestor, eventAncestor~nodeValue)
              savedEnclosingAttr = retval
              return
          end
          else if type == .Node~ENTITY_REFERENCE_NODE then do
              eventAncestor = eventAncestor~parentNode
          end
          else if type == .Node~TEXT_NODE then do
              eventAncestor = eventAncestor~parentNode
          end
          else do
              return
          end
      end
  end

::method modifyingCharacterData
  expose hasMutationEvents

  use strict arg node, replace

  if hasMutationEvents then do
      if \replace then do
          self~saveEnclosingAttr(node)
      end
  end

::method modifiedCharacterData
  expose hasMutationEvents savedEnclosingAttr

  use strict arg node, oldValue, value, replace

  if hasMutationEvents then do
      self~mutationEventsModifiedCharacterData(node, oldValue, value, replace)
  end

::method mutationEventsModifiedCharacterData
  use strict arg node, oldValue, value, replace

  if \replace then do
      capture = self~lookupCapture(.MutationEvent.DOM_CHARACTER_DATA_MODIFIED)
      if capture~total > 0 then do
          me = .MutationEvent~new
          me~initMutationEvent(.MutationEvent~DOM_CHARACTER_DATA_MODIFIED, -
              .true, .false, .nil, oldValue, value, .nil, 0)
          self~dispatchEvent(node, me)
      end

      self~dispatchAggregateEvents(node, savedEnclosingAttr~node, oldValue)
  end

::method replaceCharacterData
  use strict arg node, oldvalue, value
  self~modifiedCharacterData(node, oldvalue, value, .false)

::method insertingNode
  expose hasMutationEvents

  use strict arg node, replace
  if hasMutationEvents then do
      if \replace then do
          self~saveEnclosingAttr(node)
      end
  end

::method insertedNode
  expose hasMutationEvents ranges
  use strict arg node, newInternal, replace

  if hasMutationEvents then do
      self~mutationEventsInsertedNode(node, newInternal, replace)
  end

  if ranges \= .nil then do
      self~notifyRangesInsertedNode(newInternal)
  end

::method mutationEventsInsertedNode private
  expose savedEnclosingAttr
  use strict arg node, newInternal, replace

  capture = self~lookupCapture(.MutationEvent~DOM_NODE_INSERTED)
  if capture~total > 0 then do
      me = .MutationEvent~new
      me~initMutationEvent(.MutationEvent~DOM_NODE_INSERTED, .true. .false, -
          node, .nil, .nil, .nil, 0)
      self~dispatchEvent(newInternal, me)
  end

  capture = self~lookupCapture(.MutationEvent~DOM_NODE_INSERTED_INTO_DOCUMENT)
  if capture~total > 0 then do
      eventAncestor = .node
      if savedEnclosingAttr \= .nil then do
          eventAncestor = savedEnclosingAttr~node~ownerElement
      end
      if eventAncestor \= .nil then do
          parent = eventAncestor
          do while parent \= .nil
              if parent~nodeType == .Node~ATTRIBUTE_NODE then do
                  parent = parent~ownerDocument
              end
              else do
                  parent = parent~parentNode
              end
          end
          if eventAncestor~nodeType == .Node~DOCUMENT_NODE then do
              me = .MutationEvent~new
              me~initMutationEvent(.MutationEvent~DOM_NODE_INSERTED_INTO_DOCUMENT, -
                  .false, .false, .nil, .nil, .nil, .nil, 0)
              self~dispatchEventToSubtree(newInternal, me)
          end
      end
  end

  if \replace then do
      self~dispatchAggregateEvents(node, savedEnclosingAttr~node, saveEnclosingAttr~oldValue)
  end

::method notifyRangesInsertNode
  expose ranges
  use strict arg newInternal

  do range over ranges
      range~insertedNodeFromDOM(newInternal)
  end

::method removingNode
  expose iterators ranges hasMutationEvents
  use strict arg node, oldChild, replace

  if iterators \= .nil then do
      self~notifyIteratorsRemovingNode(oldChild)
  end

  if ranges \= .nil then do
      self~notifyRangesRemovingNode(oldChild)
  end

  if hasMutationEvents then do
      self~mutationEventsRemovingNode(node, oldChild, replace)
  end

::method notityIteratorsRemovingNode
  expose iterators
  use strict arg oldChild

  do iterator over iterators
      iterator~removeNode(oldChild)
  end

::method notifyRangesRemovingNode
  expose ranges
  use strict arg oldChild

  do range over ranges
      range~removeNode(oldChild)
  end

::method mutationEventsRemovingNode
  expose savedEnclosingAttr
  use strict arg node, oldChild, replace

  if \replace then do
      self~saveEnclosingAttr(node)
  end

  capture = self~lookupCapture(.MutationEvent~DOM_NODE_REMOVED)
  if capture~total > 0 then do
      me = .MutationEvent~new
      me~initMutationEvent(.MutationEvent~DOM_NODE_REMOVED, .true, .false,
          node, .nil, .nil, .nil, 0)
      self~dispatchEvent(oldChild, me)
  end

  capture~captureLookup(.MutationEvent~DOM_NODE_REMOVED_FROM_DOCUMENT)
  if capture~total then do
      eventAncestor = self
      if savedEnclosingAttr \= .nil then do
          eventAncestor = savedEnclosingAttr~node~ownerElement
      end
      if eventAncestor \= .nil then do
          parent = eventAncestor~parentNode
          do while parent \= .nil
              eventAncestor = parent
              parent = parent~parentNode
          end
      end
      if eventAncestor~nodeType == .Node~DOCUMENT_NODE then do
          me = new .MutationEvent~new
          me~initMutationEvent(.MutationEvent~DOM_NODE_REMOVED_FROM_DOCUMENT,
              .false, .false, .nil, .nil, .nil, .nil, 0)
          self~dispatchEventToSubtree(oldChild, me)
      end
  end

::method removedNode
  expose hasMutationEvents savedEnclosingAttr
  use strict arg node, replace

  if hasMutationEvents then do
      if \replace then do
          self~dispatchAggregateEvents(node, savedEnclosingAttr~node, savedEnclosingAttr~oldValue)
      end
  end

::method replacingNode
  expose hasMutationEvents
  use strict arg node

  if hasMutationEvents then do
      self~saveEnclosingAttr(node)
  end

::method replacingData
  expose hasMutationEvents
  use strict arg node
  if hasMutationEvents then do
      self~saveEnclosingAttr(node)
  end

::method replacedNode
  expose hasMutationEvents savedEnclosingAttr
  use strict arg node

  if hasMutationEvents then do
      self~dispatchAggregateEvents(node, savedEnclosingAttr~node, saveEnclosingAttr~oldValue)
  end

::method modifiedAttrValue
  expose hasMutationEvents
  use strict arg attr, oldValue

  if hasMutationEvents then do
      self~dispatchAggregateEvents(attr, attr, oldvalue, .MutationEvent~MODIFICATION)
  end

::method setAttrNode
  expose hasMutationEvents
  use strict arg attr, previous

  if hasMutationEvents then do
      if previous == .nil then do
          self~dispatchAggregateEvents(attr~ownerNode, attr, .nil, .MutationEvent~ADDITION)
      end
      else do
          self~dispatchAggregateEvents(attr~ownerNode, attr, previous~nodeValue, .MutationEvent~MODIFICATION)
      end
  end

::method removeAttrNode
  expose hasMutationEvents
  use strict arg attr, oldOwner, name

  if hasMutationEvents then do
      self~mutationEventRemovedAttrNode(attr, oldOwner, name)
  end

::method mutationEventsRemovedAttrNode
  use strict arg attr, oldOwner, name

  capture = self~lookupCapture(.MutationEvent~DOM_ATTR_MODIFIED)
  if capture~total > 0 then do
      me = .MutationEvent~new
      me~initMutationEvent(.MutationEvent~DOM_ATTR_MODIFIED, .true, .false, attr,
          attr~nodeValue, .nil, name, .MutationEvent~REMOVAL)
      self~dispatchEvent(oldOwner, me)
  end

  self~dispatchAggregateEvents(oldOwner)

::method renamedAttrNode
::method renamedElement

::class "EventListenerImpl" public
::method init
  expose type useCapture listener
  use strict arg type, listener, useCapture

::method equals
  expose type useCapture listener
  use strict arg eventType, eventlistener, phase

  return phase == useCapture & listener == eventListener & eventType == type

::method handlesEvent
  expose type useCapture

  use strict arg eventType, phase

  return phase == useCapture & type == eventType

::method invoke
  expose listener
  use strict arg event

  signal on syntax
  listener~handleEvent(event)
  return

syntax:
  return   -- all errors are just ignored


::class "EventTrackerImpl" public
::method init
  expose captures total bubble total

::attribute captures GET
::attribute total    GET
::attribute bubbles  GET

::method addListener
  expose captures total bubbles
  use arg useCapture

  total += 1

  if useCapture then do
      captures += 1
  end
  else do
      bubbles += 1
  end

::method removeListener
  expose captures total bubbles
  use arg useCapture

  total -= 1

  if useCapture then do
      captures -= 1
  end
  else do
      bubbles -= 1
  end

::class "EnclosingAttr" public
::method init
  expose node oldValue
  use strict arg node, oldValue

::attribute node GET
::attribute oldValue GET

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Attr                                                                */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class "AttrImpl" public subclass NodeImpl inherit Attr
::method init
  expose value nodeName textNode namespaceURI localName type idAttribute prefix
  value = .nil
  type = .nil
  textNode = .nil
  namespaceURI = .nil
  localName = .nil
  prefix = .nil
  idAttribute = .false
  use strict arg ownerDoc, nodeName, namespaceURI = .nil, explicitLocalName = .nil
  self~init:super(ownerDoc)
  self~setName(nodeName)
  if explicitLocalName \= .nil then do
      localName = explicitLocalName
  end
  self~isSpecified = .false
  self~hasStringValue = .true

::method setName private
  expose namespaceURI localName prefix
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
      parse var qname prefix =(colon1) =(colon2 + 1) localName
  end

-- support for the Document renameNode method
::method rename
  use strict arg uri, name = .nil
  if name == .nil then do
      self~nodeName = uri
  end
  else do
      self~nodeName = name
      self~namespaceURI = uri
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

::method isId
  expose idAttribute
  return idAttribute

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

::attribute nodeName

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

::attribute schemaTypeInfo GET
  use strict arg
  -- we provide all of our own type information directly
  return this


-- name is the same as nodename
::attribute name GET
  forward message("NODENAME")
::attribute name SET
  forward message("NODENAME=")

::attribute value SET
  expose value textNode
  use strict arg newValue

  oldvalue = ""

  -- we might need to process an old value if it's a serious of text nodes
  if value \= .nil then do
      if self~ownerDocument~hasMutationEvents then do
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
                  self~removeChild(textNode, .true)
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
  if self~ownerDocument~hasMutationEvents then do
      self~insertBefore(self~ownerDocument~createTextNode(newValue), .nil, .true)
      self~hasStringValue = .false
      self~ownerDocument~modifiedAttrValue(self, oldvalue)
  end
  else do
      value = newValue
      self~changed
  end

  if self~isId & self~ownerElement \= .nil then do
      self~ownerDocument~putIdentifier(newvalue, ownerElement)
  end

::attribute value GET
  expose value
  use strict arg
  -- no set value is a null string
  if value == .nil then do
      return ""
  end
  -- if already a string, then return it directly
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

-- some attribute properties
::attribute hasStringValue
::attribute isSpecified

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

  self~ownerDocument~insertingNode(self, replace)
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

  self~ownerDocument~insertedNode(self, newNode, replace)

  return newChild

::method removeChild
  expose value
  use strict arg oldChild, replace = .false

  if value~isA(.string) then do
      return .nil
  end

  self~ownerDocument~removingNode(self, oldNode, replace)

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

  ownerDocument~removedNode(self, replace)

  return oldNode

::method replaceChild
  use strict arg newChild, oldChild

  self~makeChildNode
  self~ownerDocument~replacingNode(self)
  self~insertBefore(newChild, oldChild, .true)
  if newChild \= oldChild then do
      self~removeChild(oldChild, .true)
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


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Element                                                             */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class "ElementImpl" subclass ParentNode public inherit Element NodeList
::method init
  expose nodeName attributes namespaceURI localName type
  attributes = .nil
  namespaceURI = .nil
  localName = .nil
  type = .nil

  if arg() == 2 then do
      use strict arg ownerDoc, nodeName
      self~init:super(ownerDoc)
      -- these are the same if not provided
      localName = nodeName
  end
  else if arg() == 3 then do
      use strict arg ownerDoc, namespaceURI, nodeName
      self~init:super(ownerDoc)
      -- decode the name to see if there is a prefix
      self~setName(nodeName)
  end
  else do
      use strict arg ownerDoc, namespaceURI, nodeName, temp
      self~init:super(ownerDoc)
      -- decode the name to see if there is a prefix
      self~setName(nodeName)
      -- the provided version overrides anything decoded from the qname
      localName = temp

  end

-- decode a qualified name into its elements
::method setName private
  expose namespaceURI localName
  use arg qname
  -- null string is the same as not there
  if namespaceURI == "" then do
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
      parse var qname prefix =(colon1) =(colon2 + 1) localName
      -- TODO:  validate the name information with the ownerdoc
  end

-- support for the Document renameNode method
::method rename
  expose nodeName namespaceURI
  use strict arg uri, name = .nil
  if name == .nil then do
      nodeName = uri
      self~setName(nodeName)
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
      attributes = .AttributeMap~new(self, .nil)
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
      attributes~ownerDocument = doc
  end

-- retrieve the value of a named attribute
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

-- get the node associated with a specific named attribute
::method getAttributeNode
  expose attributes

  use strict arg name
  if attributes == .nil then do
      return .nil
  end

  return attributes~getNamedItem(name)

::method getElementsByTagName
  use strict arg tagname
  -- this version does a lazy search
  return .DeepNodeList~new(self, tagname)

-- this is the same as the nodename for an element
::attribute tagname GET
  expose nodeName
  return nodeName

::method removeAttribute
  expose attributes
  use strict arg name

  if attributes \= .nil  then do
      return attributes~removeNamedItem(name)
  end
  return .nil

::method removeAttributeNode
  expose attributes
  use strict arg oldAttr

  if attrbutes \= .nil  then do
      return attributes~removeItem(oldAttr, .true)
  end
  return .nil

::method setAttribute
  use strict arg name, value

  newAttr = self~getAttributeNode(name)
  if newAttr == .nil then do
      -- get the attribute map.  This also creates it if
      -- we don't have one yet
      attributes = self~attributes
      newAttr = self~ownerDocument~createAttribute(name)

      newAttr~value = value
      attributes~setNamedItem(newAttr)
  end
  else do
      newAttr~value = value
  end
  return newAttr

::method setAttributeNode
  use strict arg newAttr

  -- get the attribute map.  This also creates it if
  -- we don't have one yet
  attributes = self~attributes

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


  -- set an attribute using a namespace URI and a qualified name
::method setAttributeNS
  expose attributes
  use strict arg namespaceURI, qualifiedName, value

  index = qualifiedName~pos(":")
  if pos > 0 then do
      parse var qualifiedName prefix ":" localName
  end
  else do
      prefix = .nil
      localName = qualifiedName
  end

  newAttr = self~getAttributeNodeNS(namespaceURI, localName)
  if newAttr == .nil then do
      newAttr = self~ownerDocument~createAttributeNS(namespaceURI, qualifiedName)
      -- get the attribute map.  This also creates it if
      -- we don't have one yet
      attributes = self~attributes

      newAttr~nodeValue = value
      attributes~setNamedItemNS(newAttr)
  end
  else do
      newAttr~name = qualifiedName
      newAttr~nodeValue = value
  end
  return newAttr

::method removeAttributeNS
  expose attributes
  use strict arg namespaceURI, localName

  if attributes == .nil then do
      return .nil
  end

  return attributes~removeNamedItemNS(namespaceURI, localName)

::method getAttributeNodeNS
  expose attributes
  use strict arg namespaceURI, localName

  if attributes == .nil then do
      return .nil
  end

  return attributes~getNamedItemNS(namespaceURI, localName)

::method setAttributeNodeNS
  use strict arg newAttr

  -- get the attribute map.  This also creates it if
  -- we don't have one yet
  attributes = self~attributes

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

  return .DeepNodeList~new(self, localName, namespaceURI)

::method setIdAttributeNode
  use strict arg at, makeId

  at~idAttribute = makeId
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

  at~idAttribute = makeId
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

  at~idAttribute = makeId
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
  return .nil


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
  return .nil

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

::method nextLogicalSibling private
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

::method previousLogicalSibling private
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



/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: CharacterData -- base class for all nodes that carry character data */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class "CharacterDataImpl" public subclass ChildNode inherit CharacterData
::method init
  expose data
  use strict arg ownerDocument, data
  self~init:super(ownerDocument)

::attribute childNodes GET
  use strict arg
  -- always returns an empty node list
  return .NodeListImpl~new

::attribute nodeValue GET
  expose data
  use strict arg
  return data

::attribute nodeValue SET
  expose data
  use strict arg value, replace = .false

  oldValue = data
  self~ownerDocument~modifyingCharacterData(self, replace)

  data = value

  self~ownerDocument~modifiedCharacterData(self, oldValue, value, replace)

::attribute data GET
::attribute length GET
  expose data
  use strict arg
  return data~length

::method appendData
  expose data
  use strict arg newData
  if newData == .nil then do
      return
  end

  self~setNodeValue(data||newData)

::method deleteData
  expose data
  use strict arg offset, count, replace = .false

  tailLength = max(data~length - count - offset, 0)
  if offset >= data~length  then do
      newData = data
  end
  else do
      newData = data~delstr(offset + 1, count)
  end
  -- set the node value to the adjusted version
  self~nodeValue = newData

  self~ownerDocument~deletedText(self, offset, count)

::method insertData
  expose data
  use strict arg offset, newData, replace = .false

  if offset >= data~length then do
      newValue = data
  end
  else do
      newValue = date~insert(newData, offset +1)
  end

  self~ownerDocument~deletedText(self, offset, data~length)

::method replaceData
  expose data
  use strict arg offset, count, newData

  oldvalue = data

  self~ownerDocument~replacingData(self)

  -- this needs to be done as multiple operations to get all of the
  -- events broadcast
  self~deleteData(offset, count, .true)
  self~insertData(offset, newData, .true)

  self~ownerDocument~replacedCharacterData(self, oldvalue, data)

::method setData
  forward message("NODEVALUE=")

::method substringData
  expose data
  use strict arg offset, count

  return data~substring(offset + 1, count)


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Text -- a DOM text node                                             */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class "TextImpl" public subclass CharacterDataImpl inherit Text

::attribute nodeType GET
  use strict arg
  return .Node~TEXT_NODE

::attribute nodeName GET
  use strict arg
  return "#text"

::method getWholeText
  use strict arg

  buffer = .mutablebuffer~new
  data = self~data
  if data \= .nil then do
      buffer~append(data)
  end

  self~getWholeTextBackward(self~previousSibling, buffer, self~parentNode)
  self~getWholeTextForward(self~nextSibling, buffer, self~parentNode)

  return buffer~string

::method getWholeTextForward
  use arg node, buffer, parent

  processingEntity = .false
  if parent \= .nil then do
      processingEntity = parent~nodeType == .Node~ENTITY_REFERENCE_NODE
  end

  do while node \= .nil
      type= node~nodeType
      -- unwind the entity references as a unit
      if type == .Node~ENTITY_REFERENCE_NODE then do
          if self~getWholeTextForward(node~firstChild, buffer, node) then do
              return .true
          end
      end
      -- a text node, just add the text content to the buffer
      else if type == .Node~TEXT_NODE | type = .Node~CDATA_SECTION_NODE then do
          node~buildTextContent(buffer)
      end
      else do
          -- we're finished if we found a non-text node
          return .true
      end
      node = node~nextSibling
  end

  -- we've run through everything, but if we started with the child of an
  -- entity reference, we need to check the siblings for additional logical
  -- text nodes
  if processingEntity then do
      self~getWholeTextForward(parent~nextSibling, buffer, parent~parentNode)
      return .true
  end

  return .false

::method getWholeTextBackward
  use strict arg node, buffer, parent

  processingEntity = .false
  if parent \= .nil then do
      processingEntity = parent~nodeType == .Node~ENTITY_REFERENCE_NODE
  end

  do while node \= .nil
      type= node~nodeType
      -- unwind the entity references as a unit
      if type == .Node~ENTITY_REFERENCE_NODE then do
          if self~getWholeTextBackward(node~lastChild, buffer, node) then do
              return .true
          end
      end
      -- a text node, just add the text content to the buffer
      else if type == .Node~TEXT_NODE | type = .Node~CDATA_SECTION_NODE then do
          node~insertTextContent(buffer)
      end
      else do
          -- we're finished if we found a non-text node
          return .true
      end
      node = node~previousSibling
  end

  -- we've run through everything, but if we started with the child of an
  -- entity reference, we need to check the siblings for additional logical
  -- text nodes
  if processingEntity then do
      self~getWholeTextBackward(parent~previousSibling, buffer, parent~parentNode)
      return .true
  end

  return .false

::method insertTextContent private
  use arg buffer
  content = self~nodeValue
  if content \= .nil then do
      buffer~insert(1, content)
  end

::method replaceWholeText
  use strict arg content

  parent = self~parentNode
  -- no content or a null string content, we just remove everything
  if content == .nil | content == "" then do
      -- just remove ourselves from the parent
      if parent \= .nil then do
          parent~removeChild(self)
      end
      return .nil
  end

  self~data = content
  currentNode = self

  previous = currentNode~previousSibling
  do while previous \= .nil
      -- remove any logically adjacent text or entity reference nodes
      nodetype = previous~nodeType
      if nodeType = .Node~TEXT_NODE | -
          nodeType = .Node~CDATA_SECTION_NODE | -
          nodeType = .Node~ENTITY_REFERENCE_NODE
          then do
          -- remove the node from the parent
          parent~removeChild(previous)
      end
      else do
          -- non-text node found, time to quit
          leave
      end
      -- we keep pulling the previous sibling of the current
      -- until we find a non-text node
      previous = currentNode~previousSibling
  end

  -- now remove the trailing siblings
  next = currentNode~nextSibling

  do while next \= .nil
      -- remove any logically adjacent text or entity reference nodes
      nodetype = next~nodeType
      if nodeType = .Node~TEXT_NODE | -
          nodeType = .Node~CDATA_SECTION_NODE | -
          nodeType = .Node~ENTITY_REFERENCE_NODE
          then do
          -- remove the node from the parent
          parent~removeChild(next)
      end
      else do
          -- non-text node found, time to quit
          leave
      end
      -- we keep pulling the previous sibling of the current
      -- until we find a non-text node
      next = currentNode~nextSibling
  end

  -- this is now the only text node
  return currentNode

::method splitText
  expose data
  use strict arg offset

  newText = self~ownerDocument~createTextNode(data~substr(offset + 1))
  self~nodeValue = data~substr(1, offset)

  -- now insert the new text node
  parent = self~parentNode
  if parentNode \= .nil then do
      parentNode~insertBefore(newText, self~nextSibling)
  end

  return newText


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: CDATASection                                                        */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class "CDATASectionImpl" public subclass TextImpl inherit CDATASection
::attribute nodeType GET
  use strict arg
  return .Node~CDATA_SECTION_NODE

::attribute nodeName GET
  use strict arg
  return "#cdata-section"


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: DocumentType                                                        */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class "DocumentTypeImpl" public subclass NodeImpl inherit DocumentType


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: DocumentType                                                        */
/*        Private methods                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: DocumentType                                                        */
/*        Public methods                                                      */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Method: init                                                               */
/* Description: instance initialization.                                      */
/*----------------------------------------------------------------------------*/

::method init
  use strict arg name
  self~nodeName = name
  self~nodeType = 10
  self~nodeValue = .nil
  self~entities = .NamedNodeMapImpl~new
  self~notations = .NamedNodeMapImpl~new

/*----------------------------------------------------------------------------*/
/* Method: name                                                               */
/* Description: return the node name.                                         */
/*----------------------------------------------------------------------------*/

::method name
  use strict arg
  return self~nodeName


/*----------------------------------------------------------------------------*/
/* Method: entities                                                           */
/* Description: return the list of entities.                                  */
/*----------------------------------------------------------------------------*/

::method entities
  expose entities
  use strict arg
  return entities


/*----------------------------------------------------------------------------*/
/* Method: notations                                                          */
/* Description: return the list of notations                                  */
/*----------------------------------------------------------------------------*/

::method notations
  expose notations
  use strict arg
  return notations


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Notation                                                            */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
::class "NotationImpl" public subclass NodeImpl inherit Notation
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


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Entity -- an entity value                                           */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class "EntityImpl" public subclass ParentNode inherit Entity
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
  baseURI = .nil

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


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: EntityReference                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class "EntityReferenceImpl" public subclass ParentNode inherit EntityReference
::method init
  expose name baseURI
  use strict arg ownerDoc, name
  self~init:super(ownerDoc)
  baseURI = .nil

::attribute nodeType GET
  use strict arg
  return .Node~ENTITY_NODE

::attribute nodeName GET
  expose name
  use strict arg
  return name

::attribute entityRefValue GET
  use strict arg

  value = ""
  firstChild = self~firstChild
  if firstChild == .nil then do
      return .nil
  end
  if firstChild~nodeType == .Node~ENTITY_REFERENCE_NODE then do
      value = firstChild~entityRefValue
  end
  else if firstChild~nodeType == .Node~TEXT_NODE then do
      value = firstChild~nodeValue
  end
  else do
      return .nil
  end

  if firstChild~nextSibling == .nil then do
      return value
  end
  else do
      buffer = .mutablebuffer~new(value)
      next = firstChild~nextSibling
      do while next \= .nil
          if next~nodeType == .Node~ENTITY_REFERENCE_NODE then do
              value = next~entityRefValue
          end
          else if next~nodeType == .Node~TEXT_NODE then do
              value = next~nodeValue
          end
          else do
              return .nil
          end
          buffer~append(value)
          next = next~nextSibling
      end
      return buffer~string
  end

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: ProcessingInstruction                                               */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
::class "ProcessingInstructionImpl" public subclass CharacterDataImpl inherit ProcessingInstruction
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


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class:  ooRexxDOM                                                          */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class "ooRexxDOM" public inherit DOMImplementation
::method init class
  expose singleton
  singleton = .nil

::attribute implementation GET class
  expose singleton
  use strict arg
  if singleton == .nil then do
      singleton = self~new
  end

  return singleton

::method hasFeature
  use strict arg feature, version = .nil

  anyVersion = version == .nil | version == ""
  feature = feature~upper

  select
      when feature = "CORE" then do
          return anyVersion | version == 1.0 | verison == 2.0 | version == 3.0
      end
      when feature = "XML" then do
          return anyVersion | version == 1.0 | verison == 2.0 | version == 3.0
      end
      when feature = "XMLVERSION" then do
          return anyVersion | version == 1.0 | verison == 1.1
      end
      when feature = "LS" then do
          return anyVersion | version == 3.0
      end
      when feature = "XPATH" then do
          return anyVersion | version == 3.0
      end
      otherwise  do
          return .false
      end
  end

::method createDocumentType
  use strict arg qualifiedName, publicID, systemID
  self~checkQName(qualifiedName)
  return .DocumentType(.nil, qualifiedName, publicID, systemID)

::method checkQName
  use strict arg qname
  index = qname~pos(":")
  lastIndex = qname~lastPos(":")
  length = qname~length

  if index = 1 | index = length | lastIndex \= index then do
      .DomErrors(.DomErrors~NAMESPACE_ERR)
  end

  start = 1
  if index > 1 then do
      if \.XMLChar~isNCNameStart(gname~subChar(start)) then do
          .DomErrors~raiseError(.DomErrors~INVALID_CHARACTER_ERR)
      end

      do i = 2 to index
          if \.XMLChar~isNCName(gname~subChar(i)) then do
              .DomErrors~raiseError(.DomErrors~INVALID_CHARACTER_ERR)
          end
      end
      start = index + 1
  end

  if \.XMLChar~isNCNameStart(gname~subChar(start)) then do
      .DomErrors~raiseError(.DomErrors~INVALID_CHARACTER_ERR)
  end
  do i = start + 1 to length
      if \.XMLChar~isNCName(gname~subChar(i)) then do
          .DomErrors~raiseError(.DomErrors~INVALID_CHARACTER_ERR)
      end
  end

-- create a document node
::method createDocument
  use strict arg namespaceURI = .nil, qualifiedName = .nil, doctype = .nil

  if doctype \= .nil, doctype~ownerDocument \= .nil then do
      .DomErrors~raiseError(.DomErrors~WRONG_DOCUMENT_ERR)
  end

  doc = .CoreDocument~new(doctype)

  if qualifiedName \= .nil | namespaceURI \= .nil then do
      element = doc~createElementNS(namespaceURI, qualifiedName)
      doc~appendChild(element)
  end

  return doc

::method getFeature
  singleton = self~class~DOMImplementation
  if singleton~hasFeature(feature, version) then do
      if feature~upper = "XPATH" then do
          return .XPathEvaluator~new
      end
      else do
          return singleton
      end
  end

  return .nil


::class "DOMErrors" public
::constant HIERARCHY_REQUEST_ERR  "An attempt was made to insert a node where it is not permitted."
::constant INDEX_SIZE_ERR         "The index or size is negative, or greater than the allowed value."
::constant INUSE_ATTRIBUTE_ERR    "An attempt is made to add an attribute that is already in use elsewhere."
::constant INVALID_ACCESS_ERR     "A parameter or an operation is not supported by the underlying object."
::constant INVALID_CHARACTER_ERR  "An invalid or illegal XML character is specified."
::constant INVALID_MODIFICATION_ERR "An attempt is made to modify the type of the underlying object."
::constant INVALID_STATE_ERR        "An attempt is made to use an object that is not, or is no longer, usable."
::constant NAMESPACE_ERR            "An attempt is made to create or change an object in a way which is incorrect with regard to namespaces."
::constant NOT_FOUND_ERR            "An attempt is made to reference a node in a context where it does not exist."
::constant NOT_SUPPORTED_ERR        "The implementation does not support the requested type of object or operation."
::constant NO_DATA_ALLOWED_ERR      "Data is specified for a node which does not support data."
::constant NO_MODIFICATION_ALLOWED_ERR "An attempt is made to modify an object where modifications are not allowed."
::constant SYNTAX_ERR                  "An invalid or illegal string is specified."
::constant VALIDATION_ERR              "A call to a method such as insertBefore or removeChild would make the Node invalid with respect to document grammar."
::constant WRONG_DOCUMENT_ERR          "A node is used in a different document than the one that created it."
::constant TYPE_MISMATCH_ERR           "The value type for this parameter name is incompatible with the expected value type."

::constant BAD_BOUNDARYPOINTS_ERR      "The boundary-points of a Range do not meet specific requirements."
::constant INVALID_NODE_TYPE_ERR       "The container of a boundary-point of a Range is being set to either a node of an invalid type or a node with an ancestor of an invalid type."

::method raiseError class
  use arg message

  raise syntax 98.900 array(message)    -- just raise this as a user execution error


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: DOMEvent                                                            */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
::class "DOMEvent" public
::method init
  expose initialized target eventPhase currentTarget timeStamp stopPropagation preventDefault
  initialized = .false
  currentTarget = .nil
  eventPhase = .nil
  timeStamp = .DateTime~new
  stopPropagation = .false
  preventDevault = .false

::method initEvent
  expose type bubbles cancelable initialized target eventPhase currentTarget timeStamp stopPropagation preventDefault
  use strict arg type, bubbles, cancelable
  initialized = .true

::attribute bubbles GET
::attribute cancelable GET
::attribute currentTarget GET
::attribute eventPhase GET
::attribute target GET
::attribute type GET
::attribute timeStamp GET

::method stopPropagation
  expose stopPropagation
  use strict arg
  stopPropagation = .true

::method preventDefault
  expose preventDefault
  use strict arg
  preventDefault = .true

::class "MutationEvent" subclass DOMEvent public
::constant DOM_SUBTREE_MODIFIED "DOMSubtreeModified"
::constant DOM_NODE_INSERTED "DOMNodeInserted"
::constant DOM_NODE_REMOVED "DOMNodeRemoved"
::constant DOM_NODE_REMOVED_FROM_DOCUMENT "DOMNodeRemovedFromDocument"
::constant DOM_NODE_INSERTED_INTO_DOCUMENT "DOMNodeInsertedIntoDocument"
::constant DOM_ATTR_MODIFIED "DOMAttrModified"
::constant DOM_CHARACTER_DATA_MODIFIED "DOMCharacterDataModified"


::method init
  expose relatedNode prevValue newValue attrName
  self~init:super
  relatedNode = .nil
  prevValue = .nil
  newValue = .nil
  attrName = .nil

::attribute attrName GET
::attribute attrChange GET
::attribute newValue GET
::attribute prevValue GET
::attribute relatedNode GET

::method initMutationEvent
  expose relatedNode prevValue newValue attrName attrChange
  use strict arg type, bubble, cancelable, relatedNode, prevValue, newValue, attrName, attrChange
  self~initEvent(type, canBubble, cancelable)


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: NodeFilter                                                          */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
::class "NodeFilter" mixinclass Object public
-- constants returned by acceptNode
::constant FILTER_ACCEPT 1
::constant FILTER_REJECT 2
::constant FILTER_SKIP   3

-- whatToShow values
::constant SHOW_ALL                     -1
::constant SHOW_ELEMENT                  1
::constant SHOW_ATTRIBUTE                2
::constant SHOW_TEXT                     4
::constant SHOW_CDATA_SECTION            8
::constant SHOW_ENTITY_REFERENCE        16
::constant SHOW_ENTITY                  32
::constant SHOW_PROCESSING_INSTRUCTION  64
::constant SHOW_COMMENT                128
::constant SHOW_DOCUMENT               256
::constant SHOW_DOCUMENT_TYPE          512
::constant SHOW_DOCUMENT_FRAGMENT     1024
::constant SHOW_NOTATION              2048

-- use to filter the individual nodes
::method acceptNode
  use strict arg node
  -- default implementation accepts everything
  return self~FILTER_ACCEPT

::method isShowing
  use strict arg whatToShow, test

  if whatToShow == .NodeFilter~SHOW_ALL then do
      flagValues = "11111111111"    -- we'll set all flags to true
  end
  else do
      -- convert this to a binary string, then pad out to 11 digits for the
      -- mapping
      flagValues = whatToShow~d2x~x2b~right(11, 0)
  end

  whatToShowFlags = .directory~new
  parse var flagValues whatToShowFlags[.Node~ELEMENT] +1 -
                       whatToShowFlags[.Node~ATTRIBUTE] +1 -
                       whatToShowFlags[.Node~TEXT] +1 -
                       whatToShowFlags[.Node~CDATA_SECTION] +1 -
                       whatToShowFlags[.Node~ENTITY_REFERENCE] +1 -
                       whatToShowFlags[.Node~ENTITY] +1 -
                       whatToShowFlags[.Node~PROCESSING_INSTRUCTION] +1 -
                       whatToShowFlags[.Node~COMMENT] +1 -
                       whatToShowFlags[.Node~DOCUMENT] +1 -
                       whatToShowFlags[.Node~DOCUMENT_TYPE] +1 -
                       whatToShowFlags[.Node~DOCUMENT_FRAGMENT] +1 -
                       whatToShowFlags[.Node~NOTATION] +1

 return whatToShowFlags[test]


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: NodeIterator                                                        */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
::class "NodeIterator" public
::method init
  expose document root currentNode whatToShowFlags whatToShow nodeFilter entityReferenceExpansion forward
  use strict arg document, root, whatToShow = (.NodeFilter~SHOW_ALL), nodeFilter = .nil, entityReferenceExpansion = .false

  if whatToShow == .NodeFilter~SHOW_ALL then do
      flagValues = "11111111111"    -- we'll set all flags to true
  end
  else do
      -- convert this to a binary string, then pad out to 11 digits for the
      -- mapping
      flagValues = whatToShow~d2x~x2b~right(11, 0)
  end

  whatToShowFlags = .directory~new
  parse var flagValues whatToShowFlags[.Node~ELEMENT] +1 -
                       whatToShowFlags[.Node~ATTRIBUTE] +1 -
                       whatToShowFlags[.Node~TEXT] +1 -
                       whatToShowFlags[.Node~CDATA_SECTION] +1 -
                       whatToShowFlags[.Node~ENTITY_REFERENCE] +1 -
                       whatToShowFlags[.Node~ENTITY] +1 -
                       whatToShowFlags[.Node~PROCESSING_INSTRUCTION] +1 -
                       whatToShowFlags[.Node~COMMENT] +1 -
                       whatToShowFlags[.Node~DOCUMENT] +1 -
                       whatToShowFlags[.Node~DOCUMENT_TYPE] +1 -
                       whatToShowFlags[.Node~DOCUMENT_FRAGMENT] +1 -
                       whatToShowFlags[.Node~NOTATION] +1

  forward = .true      -- we start iterating in a forward direction

::attribute root GET
::attribute whatToShow GET
::attribute filter GET
::attribute expandEntityReferences GET

::method nextNode
  expose root currentNode forward
  use strict arg

  if root == .nil then do
      return .nil
  end

  nextNode = currentNode

  do forever
      -- if we're backing up, repeat the current node
      if \forward & nextNode \= .nil then do
          nextNode = currentNode
      end
      else do
          if \entityReferenceExpansion, nextNode \= .nil, nextNode~nodeType == .Node~ENTITY_REFERENCE_NODE then do
              nextNode = self~nextNode(nextNode, .false)
          end
          else do
              nextNode = self~nextNode(nextNode, .true)
          end
      end
      forward = .true
      -- did not find a next node, return .nil
      if nextNode = .nil then do
          return .nil
      end

      -- now try the filters
      if self~acceptNode(nextNode) then do
      -- if the filter says this one is ok, then set the iteration
      -- position and return
          currentNode = nextNode
          return currentNode
      end
      -- keep looping until we find something good
  end

  return .nil   -- no acceptable nodes found

::method previousNode
  expose root currentNode forward
  use strict arg

  if root == .nil | currentNode = .nil then do
      return .nil
  end

  previousNode = currentNode

  do forever
      if forward & previousNode \= .nil then do
          previousNode = currentNode
      end
      else do
          previousNode = self~previousNode(previousNode)
      end

      -- this is going backwards
      forward = .false

      if previousNode = .nil then do
          return .nil
      end

      if self~acceptNode(previousNode) then do
          currentNode = previousNode
          return currentNode
      end
  end

  return .nil

::method acceptNode private
  expose nodeFilter whatToShowFlags
  use arg node

  if nodeFilter == .nil then do
      return whatToShowFlags[node~nodeType]
  end
  else do
      return whatToShowFlags[node~nodeType] & nodeFilter~acceptNode(node) == .NodeFilter~FILTER_ACCEPT
  end

::method locateNextNode private
  expose root
  use strict arg node, visitChildren

  if node == .nil then do
      return .nil
  end

  if visitChildren then do
      if node~hasChildNodes then do
          return node~firstChild
      end
  end

  -- back to the root?  We're done
  if node == root then do
      return .nil
  end

  -- use the next sibling if it exists
  result = node~nextSibling
  if result \= .nil then do
      return result
  end

  -- go up to the parent
  parent = node~parentNode
  do while parent \= .nil & parent \= root
      result = parent~nextSibling
      if result \= .nil then do
          return result
      end
      parent = parent~parentNode
  end
  -- end of the nodes, return .nil
  return .nil

::method locatePreviousNode private
  expose root entityReferenceExpansion
  use strict arg node

  -- back to the root?  We're done
  if node == root then do
      return .nil
  end

  -- use the next sibling if it exists
  result = node~previousSibling
  if result == .nil then do
      -- if no previous sibling, then step up to the parent
      return node~parentNode
  end

  -- if the sibling has children, drill down to the last last child
  if result~hasChildNodes & \(\entityReferenceExpansion & result~nodeType == .Node~ENTITY_REFERENCE_NODE) then do
      do while result~hasChildNodes
          result = result~lastChild
      end
  end

  return result

::method removeNode
  expose forward currentNode
  use strict arg node
  if node = .nil then do
      return
  end

  delete = self~matchNodeOrParent(node)

  if forward then do
      currentNode = self~locatePreviousNode(deleted)
  end
  else do
      next = self~locateNextNode(deleted, .false)
      if next \= .nil then do
          currentNode = next
      end
      else do
          currentNode = self~locatedPreviousNode(deleted)
          forward = .true
      end
  end

::method matchNodeOrParent private
  expose currentNode root
  use strict arg node

  if currentNode == .nil then do
      return .nil
  end

  target = currentNode
  do while target \= root
      if node == target then do
          return target
      end
      target = target~parentNode
  end

  return .nil

::method detach
  expose root currentNode document

  root = .nil
  currentNode = .nil
  document~removeNodeIterator(self)
  document = .nil

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Range                                                               */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
::class "Range" public
::constant START_TO_START 0
::constant START_TO_END   1
::constant END_TO_END     2
::constant END_TO_START   3
::constant EXTRACT_CONTENTS 1
::constant CLONE_CONTENTS   2
::constant DELETE_CONTENTS  3

::method init
  expose document startContainer endContainer startOffset endOffset -
     insertNode deleteNode splitNode insertedFromRange
  use strict arg document
  startContainer = document
  endContainer = document
  startOffset = 0
  endOffset = 0
  insertNode = .nil
  deleteNode = .nil
  splitNode = .nil
  insertedFromRange = .false

::attribute startContainer GET
::attribute startOffset GET
::attribute endContainer GET
::attribute endOffset GET
::attribute collapsed GET
  expose startContainer endContainer startOffset endOffset
  use strict arg
  return startContainer == endContainer & startOffset = endOffset

::attribute commonAncestorContainer GET
  expose startContainer
  startV = .array~new
  node = startContainer
  do while node \= .nil
      startV~append(node)
      node = node~parentNode
  end

  endV = .array~new

  node = endContainer
  do while node \= .nil
      endV~append(node)
      node = node~parentNode
  end

  -- this will give all of the common elements,
  -- retaining the order.  The last one is the
  -- element we want
  common = startV~intersection(endV)
  return common[common~last]

::method setStart
  expose startContainer startOffset endContainer endOffset
  use strict arg refNode, offset

  self~checkIndex(refNode, offset)
  startContainer = refNode
  startOffset = offset

  if self~commonAncestorContainer == .nil | -
      (startContainer == endContainer & endOffset < startOffset) then do
      self~collapse(.true)
  end

::method setEnd
  expose startContainer startOffset endContainer endOffset
  use strict arg refNode, offset

  self~checkIndex(refNode, offset)
  endContainer = refNode
  endOffset = offset

  if self~commonAncestorContainer == .nil | -
      (startContainer == endContainer & endOffset < startOffset) then do
      self~collapse(.false)
  end


::method setStartBefore
  expose startContainer startOffset endContainer endOffset
  use strict arg refNode

  startContainer = refNode~parentNode
  i = 0
  node = refNode
  do while node \= .nil
      i += 1
      node = node~previousSibling
  end

  startOffset = i - 1

  -- now collapse this, if necessary
  if self~commonAncestorContainer == .nil | -
      (startContainer == endContainer & endOffset < startOffset) then do
      self~collapse(.true)
  end


::method setStartAfter
  expose startContainer startOffset endContainer endOffset
  use strict arg refNode

  startContainer = refNode~parentNode
  i = 0
  node = refNode
  do while node \= .nil
      i += 1
      node = node~previousSibling
  end

  startOffset = i

  -- now collapse this, if necessary
  if self~commonAncestorContainer == .nil | -
      (startContainer == endContainer & endOffset < startOffset) then do
      self~collapse(.true)
  end


::method setEndAfter
  expose startContainer startOffset endContainer endOffset
  use strict arg refNode

  endContainer = refNode~parentNode
  i = 0
  node = refNode
  do while node \= .nil
      i += 1
      node = node~previousSibling
  end

  endOffset = i

  -- now collapse this, if necessary
  if self~commonAncestorContainer == .nil | -
      (startContainer == endContainer & endOffset < startOffset) then do
      self~collapse(.false)
  end


::method collapse
  expose startContainer startOffset endContainer endOffset
  use strict arg toStart

  if toStart then do
      endContainer = startContainer
      endOffset = startOffset
  end
  else do
      startContainer = endContainer
      startOffset = endOffset
  end

::method selectNode
  expose startContainer startOffset endContainer endOffset
  use strict arg refnode

  parent = refNode~parentNode
  if parent == .nil then do
      startContainer = parent
      endContainer = parent
      i = 0
      node = refNode
      do while node \= .nil
          node = previousSibling
          i += 1
      end
      startOffet = i - 1
      endOffset = i
  end

::method selectNodeContents
  expose startContainer startOffset endContainer endOffset
  use strict arg refnode

  startContainer = refNode
  startOffset = 0
  endContainer = refNode
  endOffset = 0
  first = refNode~firstChild
  do while first \= .nil
      endOffset += 1
      first = first~nextSibling
  end

::method compareBoundaryPoints
  expose startContainer startOffset endContainer endOffset
  use strict arg how, sourceRange

  select
      when how == .Range~START_TO_START then do
          endPointA = sourceRange~startContainer
          endPointB = startContainer
          offsetA = sourceRange~startOffset
          offsetB = startOffset
      end
      when how == .Range~START_TO_END then do
          endPointA = sourceRange~startContainer
          endPointB = endContainer
          offsetA = sourceRange~startOffset
          offsetB = endOffset
      end
      when how == .Range~END_TO_START then do
          endPointA = sourceRange~endContainer
          endPointB = startContainer
          offsetA = sourceRange~endOffset
          offsetB = startOffset
      end
      when how == .Range~END_TO_END then do
          endPointA = sourceRange~endContainer
          endPointB = endContainer
          offsetA = sourceRange~endOffset
          offsetB = endOffset
      end
  end

  -- The DOM Spec outlines four cases that need to be tested
  -- to compare two range boundary points:
  --   case 1: same container
  --   case 2: Child C of container A is ancestor of B
  --   case 3: Child C of container B is ancestor of A
  --   case 4: preorder traversal of context tree.

  -- case 1: same container
  if endPointA == endPointB then do
      -- returns 0, 1, or -1
      return (offsetB - offsetA)~sign
  end

  --   case 2: Child C of container A is ancestor of B
  current = endPointB
  parent = current~parentNode
  do while parent \= .nil
      if parent == endPointA then do
          if offsetA <= self~indexOf(current, endPointA) then do
              return 1
          end
          else do
              return -1
          end
      end
      current = parent
      parent = parent~parentNode
  end

  --   case 3: Child C of container B is ancestor of A
  current = endPointA
  parent = current~parentNode
  do while parent \= .nil
      if parent == endPointB then do
          if self~indexOf(current, endPointB) < offsetB then do
              return 1
          end
          else do
              return -1
          end
      end
      current = parent
      parent = parent~parentNode
  end

  --   case 4: preorder traversal of context tree.
  depthDiff = 0
  node = endPointA
  do while node \= .nil
      depthDiff += 1
      node = node~parentNode
  end

  node = endPointB
  do while node \= .nil
      depthDiff -= 1
      node = node~parentNode
  end

  do while depthDiff > 0
      endPointA = endPointA~parentNode
      depthDiff -= 1
  end

  do while depthDiff < 0
      endPointB = endPointB~parentNode
      depthDiff += 1
  end

  parentA = endPointA~parentNode
  parentB = endPointB~parentNode
  do while parentA \= parentB
      endPointA = parentA
      endPointB = parentB
      parentA = parentA~parentNode
      parentB = parentB~parentNode
  end

  node = endPointA~nextSibling
  do while node \= .nil
      if node == endPointB then do
          return 1
      end
      node = node~nextSibling
  end

  return -1

::method deleteContents
  use strict arg
  self~traverseContents(self~DELETE_CONTENTS)

::method extractContents
  use strict arg
  return self~traverseContents(self~EXTRACT_CONTENTS)

::method cloneContents
  use strict arg
  return self~traverseContents(self~CLONE_CONTENTS)

::method insertNode
  expose startContainer startOffset endContainer endOffset insertedFromRange
  use strict arg newNode
  if newNode == .nil then do
      return
  end

  type = newNode~nodeType
  currentChildren = 0
  insertedFromRange = .true

  if startContainer == .Node~TEXT_NODE then do
      parent = startContainer~parentNode
      currentChildren = parent~childNodes~length
      cloneCurrent = startContainer~cloneNode(.false)
      cloneCurrent~nodeValue = cloneCurrent~nodeValue~substr(startOffset + 1)
      startContainer~nodeValue = startContainer~nodeValue(1, startOffset)
      next = startContainer~nextSibling
      if next \= .nil then do
          if parent \= .nil then do
              parent~insertBefore(newNode, next)
              paretn~insertBefore(cloneNode, next)
          end
      end
      else do
          if parent \= .nil then do
              parent~appendChild(newNode)
              parent~appendChild(cloneCurrent)
          end
      end
      -- update the ranges
      if endContainer == startContainer then do
          endContainer = cloneCurrent
          endOffset -= startOffset
      end
      else if endContainer == parent then do
          endOffset += parent~childNodes~length - currentChildren
      end

      self~signalSplitdata(startContainer, cloneCurrent, startOffset)
  end
  else do
      if endContainer == startContainer then do
          currentChildren = endContainer~childNodes~length
      end
      current = startContainer~firstChild
      do i = 1 to startOffset while current \= .nil
          current = current~nextSibling
      end
      if current \= .nil then do
          startContainer~insertBefore(newNode, current)
      end
      else do
          startContainer~appendChild(newNode)
      end
      if endContainer = startContainer & endOffset \= 0 then do
          endOffset += endContainer~childNodes~length - currentNodes
      end
  end
  insertedFromRange = .false

::method surroundContents
  expose startContainer startOffset endContainer endOffset
  use strict arg newParent

  if newParent == .nil then do
      return
  end

  type = newParent~nodeType
  realStart = startContainer
  realEnd = endContainer

  if startContainer~nodeType == .Node~TEXT_NODE then do
      realStart = startContainer~parentNode
  end

  if endContainer~nodeType == .Node~TEXT_NODE then do
      realEnd = endContainer~parentNode
  end

  if realStart \= realEnd then do
      DomErrors~raiseError(.DomErrors~BAD_BOUNDARY_POINTS_ERR)
  end

  frag = self~extractContents()
  self~insertNode(newParent)
  newParent~appendChild(frag)
  self~selectNode(newParent)

::method cloneRange
  expose document startContainer startOffset endContainer endOffset

  range = document~createRange
  range~setStart(startContainer, startOffset)
  range~setEnd(endContainer, endOffset)
  return range

::method string
  expose startContainer startOffset endContainer endOffset

  node = startContainer
  stopNode = endContainer

  buffer = .mutablebuffer~new
  if startContainer~nodeType == .Node~TEXT_NODE | startContainer~nodeType == .Node~CDATA_SECTION_NODE then do
      if startContainer == endContainter then do
          return startContainer~nodeValue~substr(startOffset + 1, endOffset - startOffset)
      end
      buffer~append(startContainer~nodeValue~substr(startOffset + 1))
  end
  else do
      node = node~firstChild
      if startOffset > 0 then do
          counter = 0
          do while counter < startOffset, node \= .nil
              node = node~nextSibling
              counter += 1
          end
      end
      if nod == .nil then do
          node = self~nextNode(startContainer, .false)
      end
  end

  if endContainer~nodeType \= .Node~TEXT_NODE & endContainer~nodeType \= .Node~CDATA_SECTION_NODE then do
      counter = endOffset
      stopNode = endContainer~firstChild
      do while counter > 0, stopNode \= .nil
          counter -= 1
          stopNode = stopNode~nextSibling
      end

      if stopNode == .nil then do
          stopNode = self~nextNode(endContainer, .false)
      end
  end

  do while node \= stopNode, node \= .nil
      if node~nodeType == .Node~TEXT_NODE | node~nodeType == .Node~CDATA_SECTION_NODE then do
          buffer~append(node~nodeValue)
      end
      node = self~nextNode(node, .true)
  end

  if endContainer~nodeType == .Node~TEXT_NODE | endContainer~nodeType == .Node~CDATA_SECTION_NODE then do
      buffer~append(endContainer~nodeValue~substr(1, endOffset))
  end

  return buffer~string

::method detach
  expose document
  use strict arg
  document~removeRange(self)
  document = .nil

::method signalSplitData
  expose splitNode document
  use strict arg node, newNode, offset

  splitNode = node
  document~splitData(node, newNode, offset)
  splitNode = .nil

::method receiveSplitData
  expose startContainer startOffset endContainer endOffset splitNode
  use strict arg node, newNode, offset

  if node == .nil | newNode == .nil | splitNode == node then do
      return
  end

  if node == startContainer & startContainer~nodeType == .Node~TEXT_NODE then do
      if startOffset > offset then do
          startOffset = startOffset - offset
          startContainer = newNode
      end
  end

  if node == endContainer & endContainer~nodeType == .Node~TEXT_NODE then do
      if endtOffset > offset then do
          endOffset = endOffset - offset
          endContainer = newNode
      end
  end

::method deleteData
  expose deleteNode
  use strict arg node, offset, count

  deleteNode = node
  node~deleteData(offset, count)
  deleteNode = .nil

::method receiveDeletedText
  expose startContainer startOffset endContainer endOffset deleteNode
  use strict arg node, offset, count

  if node == .nil | deleteNode == node then do
      return
  end

  if node == startContainer then do
      if startOffset > offset + count then do
          startOffset = offset + (startOffset - (offset + count))
      end
      else if startOffset > offset then do
          startOffset = offset
      end
  end
  if node == endContainer then do
      if endOffset > offset + count then do
          endOffset = offset + (endOffset - (offset + count))
      end
      else if endOffset > offset then do
          endOffset = offset
      end
  end

::method insertData
  expose insertNode
  use strict arg node, index, insert

  insertNode = node
  node~insertData(index, insert)
  insertNode = .nil

::method receiveInsertedText
  expose startContainer startOffset endContainer endOffset insertNode
  use strict arg node, index, len

  if node == .nil | deleteNode == node then do
      return
  end

  if node == startContainer then do
      if index < startContainer then do
          startOffset += len
      end
  end
  if node == endContainer then do
      if index < endOffset then do
          endOffset += len
      end
  end

::method receiveReplacedText
  expose startContainer startOffset endContainer endOffset
  use strict arg node

  if node == .nil then do
      return
  end

  if node == startContainer then do
      startOffset = 0
  end

  if node == endContainer then do
      endOffset = 0
  end

::method insertedNodeFromDOM
  expose startContainer startOffset endContainer endOffset insertNode insertedFromRange
  use strict arg node

  if node == .nil | insertNode == node | insertedFromRange then do
      return
  end

  parent = node~parentNode

  if parent == startContainer then do
      index = self~indexOf(node, startContainer)
      if index < startOffset then do
          startOffset += 1
      end
  end

  if parent == endContainer then do
      index = self~indexOf(node, endContainer)
      if index < endOffset then do
          endOffset += 1
      end
  end


::method removeChild private
  expose removeChild
  use strict arg parent, child
  removeChild = child
  old = parent~removeChild(child)
  removeChild = .nil
  return old


::method removeNode
  expose startContainer startOffset endContainer endOffset removeChild
  use strict arg node

  if node == .nil | removeChild == node then do
      return
  end

  parent = node~parentNode
  if parent == startContainer then do
      index = self~indexOf(node, startContainer)
      if index < startOffset then do
          startOffset -= 1
      end
  end

  if parent == endContainer then do
      index = self~indexOf(node, endContainer)
      if index < endOffset then do
          endOffset--
      end
  end

  if parent \= startContainer | parent \= endContainer then do
      if self~isAncestorOf(node, startContainer) then do
          startContainer = parent
          startOffset = self~indexOf(node, parent)
      end
      if self~isAncestorOf(node, endContainer) then do
          endContainer = parent
          endOffset = self~indexOf(node, parent)
      end
  end


-- utility functions
::method traverseContents private
  expose startContainer startOffset endContainer endOffset
  use strict arg now

  if startContainer == .nil | endContainer = .nil then do
      return .nil
  end

  -- Case 1:  same container
  if startContainer == endContainer then do
      return self~traverseSameContainer(how)
  end

  -- Case 2:  Child C if start container is ancestor of end container.
  -- this can be quickly tested by walking the parent chain of the end
  -- container
  endContainerDepth = 0
  node = endContainer
  parent = node~parentNode
  do while parent \= .nil
      if p == startContainer then do
          return self~traverseCommonStartContainer(node, how)
      end
      node = parent
      parent = p~parentNode
      endContainerDepth += 1
  end

  -- case 3: Child C of container B is ancestor of A
  -- This can be quickly tested by walking the parent chain of A
  endContainerDepth = 0
  node = startContainer
  parent = node~parentNode
  do while parent \= .nil
      if p == endContainer then do
          return self~traverseCommonEndContainer(node, how)
      end
      node = parent
      parent = p~parentNode
      endContainerDepth += 1
  end

  -- case 4: There is a common ancestor container.  Find the
  -- ancestor siblings that are children of that container.

  depthDiff = startContainerDept - endContainerDepth
  startNode = startContainer

  do while depthDiff > 0
      startNode = startNode~parentNode
      depthDiff -= 1
  end

  endNode = endContainer

  do while depthDiff < 0
      endNode = endNode~parentNode
      depthDiff += 1
  end

  sp = startNode~parentNode
  ep = endNode~parentNode

  do while sp \= ep
      startNode = sp
      endNode = sp

      sp = sp~parentNode
      ep = ep~parentNode
  end
  return self~traverseCommonAncestors(startNode, endNode, how)

::method traverseSameContainer private
  expose document startContainer startOffset endContainer endOffset
  use strict arg how

  fragment = .nil

  if how == self~DELETE_CONTENTS then do
      fragment = document~createDocumentFragment
  end

  nodeType = startContainer~nodeType
  if nodeType == .Node~TEXT_NODe | nodeType == .Node~CDATA_SECTION_NODE | nodeType == .Node~COMMENT_NODE | nodeType == .Node~PROCESSING_INSTRUCTION_NODE then do
      test = startContainer~nodeValue
      sub = s~substr(startOffset + 1, endOffset - startOffset)

      if how \= self~CLONE_CONTENTS then do
          startContainer~deleteData(startOffset, endOffset - startOffset)
          self~collapse(.true)
      end

      if how == self~DELETE_CONTENTS then do
          return .nil
      end
      if nodeType == .Node~TEXT_NODE then do
          fragment~appendChild(document~createTestNode(sub))
      end
      else if nodeType == .Node~CDATA_SECTION_NODE then do
          fragment~appendChild(document~createCDATASection(sub))
      end
      else if nodeType == .Node~COMMENT_NODE then do
          fragment~appendChild(document~createComment(sub))
      end
      else do  -- .Node~PROCESSING_INSTRUCTION_NODE
          fragment~appendChild(document~createProcessingInstruction(startContainer~nodeName, sub))
      end
      return fragment
  end

  -- copy nodes between start/end offsets
  node = self~getSelectedNode(startContainer, startOffset)
  count = endOffset - startOffset

  do count
      sibling = node~nextSibling
      xferNode = self~traverseFullySelected(node, how)
      if fragment \= .nil then do
          fragment~appendChild(xferNode)
      end
      node = sibling
  end

  if now \= self~CLONE_CONTENTS then do
      self~collapse(.true)
  end

  return fragment

::method traverseCommonStartContainer private
  expose document startContainer startOffset endContainer endOffset
  use strict arg endAncestor, how

  fragment = .nil

  if how == self~DELETE_CONTENTS then do
      fragment = document~createDocumentFragment
  end

  node = self~traverseRightBoundary(endAncestor, how)
  if fragment \= .nil then do
      fragment~appendChild(node)
  end

  endIndex = self~indexOf(endAncestor, startContainer)
  count = endIndex - startOffset

  if count <= 0 then do
      if how \= self~CLONE_CONTENTS then do
          self~setEndBefore(endAncestor)
          self~collapse(.false)
      end
      return fragment
  end

  node = endAncestor~previousSibling
  do count
      sibling = node~previousSibling
      xferNode = self~traverseFullySelected(node, how)
      if fragment \= .nil then do
          fragment~insertBefore(xferNode, fragment~firstChild)
      end
      node = sibling
  end

  if how \= self~CLONE_CONTENTS then do
      self~setEndBefore(endAncestor)
      self~collapse(.false)
  end
  return fragment

::method traverseCommonEndContainer private
  expose document startContainer endContainer
  use strict arg startAncestor, how

  fragment = .nil

  if how == self~DELETE_CONTENTS then do
      fragment = document~createDocumentFragment
  end

  node = self~traverseLeftBoundary(endAncestor, how)
  if fragment \= .nil then do
      fragment~appendChild(node)
  end

  startIndex = self~indexOf(startAncestor, endContainer) + 1
  count = endOffset - startIndex
  node = startAncestor~nextSibling

  do count
      sibling = node~nextSibling
      xferNode = self~traverseFullySelected(node, how)
      if fragment \= .nil then do
          fragment~appendChild(xferNode)
      end
      node = sibling
  end

  if how \= self~CLONE_CONTENTS then do
      self~setStartAfter(startAncestor)
      self~collapse(.true)
  end
  return fragment

::method traverseCommonAncestors private
  expose document startContainer endContainer
  use strict arg startAncestor, endAncestor, how

  fragment = .nil

  if how == self~DELETE_CONTENTS then do
      fragment = document~createDocumentFragment
  end

  node = self~traverseLeftBoundary(endAncestor, how)
  if fragment \= .nil then do
      fragment~appendChild(node)
  end

  commonParent = startAncestor~parentNode
  startOffset = self~indexOf(startAncestor, commonParent) + 1
  endOffset = self~indexOf(endAncestor, commonParent)

  count = endOffset - startOffset
  sibling = startAncestor~nextSibling

  do count
      nextSibling = sibling~nextSibling
      node = self~traverseFullySelected(sibling, now)
      if fragment \= .nil then do
          fragment~appendChild(node)
      end
      sibling = nextSibling
  end

  node = self~traverseRightBoundary(endAncestor, how)
  if fragment \= .nil then do
      fragment~appendChild(node)
  end

  if how \= self~CLONE_CONTENTS then do
      self~setStartAfter(startAncestor)
      self~collapse(.true)
  end
  return fragment

::method traverseRightBoundary private
  expose document startContainer startOffset endContainer endOffset
  use strict arg root, how

  next = self~getSelectedNode(endContainer, endOffset - 1)
  isFullySelected = next \= endContainer

  if next == root then do
      return self~traverseNode(next, isFullySelected, .false, now)
  end

  parent = next~parentNode
  clonedParent = self~traverseNode(parent, .false, .false, how)

  do while parent \= .nil
      do while next \= .nil
          prevSibling = next~previousSibling
          clonedChild = self~traverseNode(next, isFullySelected, .false, how)
          if how \= self~DELETE_CONTENTS then do
              clonedParent~insertBefore(clonedChild, clonedParent~firstChild)
          end
          isFullySelected = .true
          next = prevSibling
      end
      if parent == root then do
          return clonedParent
      end

      next = parent~previousSibling
      parent = parent~parentNode
      node clonedGrandParent = self~traverseNode(parent, .false, .false, how)
      if how \= self~DELETE_CONTENTS then do
          clonedGrandParent~appendChild(clonedParent)
      end
      clonedParent = clonedGrandParent
  end

  return .nil

::method traverseNode private
  use strict arg node, isFullySelected, isLeft, how

  if isFullySelected then do
      return self~traverseFullySelected(node, how)
  end

  nodeType = node~nodeType
  if nodeType == .Node~TEXT_NODE | nodeType == .Node~CDATA_SECTION_NODE | -
      nodeType == .Node~COMMENT_NODE | nodeType == .Node~PROCESSING_INSTRUCTION_NODE then do
      return self~traverseCharacterDataNode(node, isLeft, how)
  end

  return self~traversePartiallySelected(node, how)

::method traverseFullySelected private
  use strict arg node, how

  select
      when how == self~CLONE_CONTENTS then do
          return node~cloneNode(.true)
      end
      when how == self~EXTRACT_CONTENTS then do
          return node
      end
      when how == self~DELETE_CONTENTS then do
          node~parentNode~removeChild(node)
          return .nil
      end
  end

::method traversePartiallySelected
  use strict arg node, how

  select
      when how == self~CLONE_CONTENTS then do
          return node~cloneNode(.false)
      end
      when how == self~EXTRACT_CONTENTS then do
          return node~cloneNode(.false)
      end
      when how == self~DELETE_CONTENTS then do
          return .nil
      end
  end

::method traverseCharacterDataNode private
  use strict arg node, isLeft, how

  textValue = node~nodeValue
  if isLeft then do
      offset = self~startOffset
      newNodeValue = textValue~substr(offset + 1)
      oldNodeValue = textValue~substr(1, offset)
  end
  else do
      offset = self~endOffset
      newNodeValue = textValue~substr(1, offset)
      oldNodeValue = textValue~substr(offset + 1)
  end

  if how \= self~CLONE_CONTENTS then do
      node~nodeValue = oldNodeValue
  end
  if how == self~DELETE_CONTENTS then do
      return .nil
  end

  newNode = node~cloneNode(.false)
  newNode~nodeValue = newNodeValue
  return newNode

::method checkIndex private
  use strict arg refNode, offset

  if offset < 0 then do
      .DOMErrors~raiseError(.DOMErrors~INDEX_SIZE_ERR)
  end

  type= refNode~nodeType

  if nodeType == .Node~TEXT_NODE | nodeType == .Node~CDATA_SECTION_NODE | -
      nodeType == .Node~COMMENT_NODE | nodeType == .Node~PROCESSING_INSTRUCTION_NODE then do
      if offset > refNode~nodeValue~length then do
          .DOMErrors~raiseError(.DOMErrors~INDEX_SIZE_ERR)
      end
  end
  else do
      if offset > refNode~childNodes~length then do
          .DOMErrors~raiseError(.DOMErrors~INDEX_SIZE_ERR)
      end
  end

::method getRootContainer private
  use strict arg node

  do while node \= .nil
      node = node~parentNode
  end

  return node

::method isLegalContainer private
  use strict arg node
  if node == .nil then do
      return .false
  end

  do while node \= .nil
      nodeType = node~nodeType
      if nodeType == .Node~ENTITY_NODE | nodeType == .Node~NOTATION_NODE | nodeType == .Node~DOCUMENT_TYPE_NODE then do
          return .false
      end
      node = node~parentNode
  end

  return .true

::method hasLegalRootContainer private
  use strict arg node
  if node == .nil then do
      return .false
  end

  rootContainer = self~getRootContainer(node)
  nodeType = node~nodeType
  if nodeType == .Node~ATTRIBUTE_NODE | nodeType == .Node~DOCUMENT_NODE | nodeType == .Node~DOCUMENT_FRAGMENT_NODE then do
      return .true
  end
  return .false

::method isLegalContainedNode private
  use strict arg node
  if node == .nil then do
      return .false
  end

  if nodeType == .Node~ATTRIBUTE_NODE | nodeType == .Node~DOCUMENT_NODE | nodeType == .Node~DOCUMENT_FRAGMENT_NODE | -
          nodeType == .Node~ENTITY_NODE | nodeType == .Node~NOTATION_NODE then do
      return .false
  end

  return .true

::method nextNode private
  expose document
  use strict arg node, visitChildren
  if node == .nil then do
      return .nil
  end

  if visitChildren then do
      result = node~firstChild
      if result \= .nil then do
          return result
      end
  end

  result = node~nextSibling
  if result \= .nil then do
      return result
  end

  parent = node~parentNode
  do while parent \= .nil, parent \= document
      result = parent~nextSibling
      if result \= .nil then do
          return result
      end
      else do
          parent = parent~parentNode
      end
  end

  return .nil

::method isAncestorOf private
  use strict arg a, b
  node = b
  do while node \= .nil
      if node == a then do
          return .true
      end
      node = node~parentNode
  end

  return .false

::method indexOf private
  use strict arg child, parent
  if child~parentNode \= parent then do
      return -1
  end

  node = parent~firstChild
  do i = 0 while node \= child
      node = node~nextSibling
  end
  return i


::method getSelectedNode private
  use strict arg container, offset

  if container~nodeType == .Node~TEXT_NODE then do
      return container
  end

  if offset < 0 then do
      return container
  end

  child = container~firstChild

  do i = 1 to offset while child \= .nil
      child = child~nextSibling
  end

  if child \= null then do
      return child
  end

  return container


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: TreeWalker                                                          */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class "TreeWalker" public
::method init
  expose currentNode root whatToShow nodeFilter entityReferenceExpansion whatToShowFlags
  use strict arg root, self~whatToShow, nodeFilter, entityReferenceExpansion

  currentNode = root

::attribute root GET
::attribute filter GET
::attribute expandEntityReferences GET
::attribute currentNode
::attribute whatToShow GET
::attribute whatToShow SET
  expose whatToShow whatToShowFlags
  use strict arg whatToShow

  if whatToShow == .NodeFilter~SHOW_ALL then do
      flagValues = "11111111111"    -- we'll set all flags to true
  end
  else do
      -- convert this to a binary string, then pad out to 11 digits for the
      -- mapping
      flagValues = whatToShow~d2x~x2b~right(11, 0)
  end

  whatToShowFlags = .directory~new
  parse var flagValues whatToShowFlags[.Node~ELEMENT] +1 -
                       whatToShowFlags[.Node~ATTRIBUTE] +1 -
                       whatToShowFlags[.Node~TEXT] +1 -
                       whatToShowFlags[.Node~CDATA_SECTION] +1 -
                       whatToShowFlags[.Node~ENTITY_REFERENCE] +1 -
                       whatToShowFlags[.Node~ENTITY] +1 -
                       whatToShowFlags[.Node~PROCESSING_INSTRUCTION] +1 -
                       whatToShowFlags[.Node~COMMENT] +1 -
                       whatToShowFlags[.Node~DOCUMENT] +1 -
                       whatToShowFlags[.Node~DOCUMENT_TYPE] +1 -
                       whatToShowFlags[.Node~DOCUMENT_FRAGMENT] +1 -
                       whatToShowFlags[.Node~NOTATION] +1

::attribute parentNode GET
  expose currentNode
  if currentNode = .nil then do
      return .nil
  end

::method lastChild
  expose currentNode
  use strict arg

  if currentNode == .nil then do
      return .nil
  end

  node = self~getLastChild(currentNode)
  if node \= .nil then do
      currentNode = node
  end

  return node

::method previousSibling
  expose currentNode
  use strict arg

  node = self~getPreviousSibline(currentNode)
  if node \= .nil then do
      currentNode = node
  end

  return node

::method nextSibling
  expose currentNode
  use strict arg

  node = self~getNextSibling(currentNode)
  if node \= .nil then do
      currentNode = node
  end

  return node

::method previousNode
  expose currentNode
  use strict arg

  if currentNode == .nil then do
      return .nil
  end

  node = self~getPreviousSibling(currentNode)
  if node == .nil then do
      node = self~getParentNode(currentNode)
      if result \= .nil then do
          currentNode = node
          return node
      end
      return .nil
  end

  lastChild = self~getLastChild(node)
  previos = lastChild

  do while lastChild \= .nil
      previous = lastChild
      lastChild = self~getLastChild(previous)
  end

  lastChild = previous

  if lastChild \= . nil then do
      currentNode = lastChild
      return lastChild
  end

  if node \= .nil then do
      currentNode = node
      return currentNode
  end

  return .nil

::method nextNode
  expose currentNode
  use strict arg

  if currentNode == .nil then do
      return .nil
  end

  node = self~getFirstChild(currentNode)

  if node \= .nil then do
      currentNode = node
      return node
  end

  node = self~getNextSibling(currentNode)

  if node \= .nil then do
      currentNode = node
      return node
  end

  parent = self~getParentNode(currentNode)
  do while parent \= .nil
      node = self~getNextSibling(parent)
      if node \= .nil then do
          currentNode = node
          return node
      end

      parent = self~getParentNode(parent)
  end

  return .nil

::method getParentNode private
  expose currentNode
  if node == .nil | node == root then do
      return .nil
  end

  newNode = node~parentNode
  if newNode == .nil then do
      return .nil
  end

  if self~acceptNode(newNode) == .NodeFilter~FILTER_ACCEPT then do
      return newNode
  end

  -- if skipped or rejected, try for another parent
  return self~getParentNode(newNode)

::method getNextSibling private
  expose root
  use strict arg node, startNode = (root)

  if node == .nil | node == startNode then do
      return .nil
  end

  newNode = node~nextSibling
  if newNode == .nil then do
      newNode = node~parentNode
      if newNode = .nil | newNode == startNode then do
          return .nil
      end

      if self~acceptNode(newNode) == .NodeFilter~FILTER_SKIP then do
          return self~getNextSibling(newNode, startNode)
      end

      return .nil
  end

  accept = self~acceptNode(newNode)
  if accept == .NodeFilter~FILTER_ACCEPT then do
      return newNode
  end
  else if accept == .NodeFilter~FILTER_SKIP then do
      child = self~getFirstChild(newNode)
      if child == .nil then do
          return self~getNextSibling(newNode, startNode)
      end
      return child
  end
  else do
      return self~getNextSibling(newNode, startNode)
  end

::method getPreviousSibling private
  expose root
  use strict arg node, startNode = (root)

  if node == .nil | node == startNode then do
      return .nil
  end

  newNode = node~previousSibling
  if newNode == .nil then do
      newNode = node~parentNode
      if newNode == .nil | newNode == startNode then do
          return .nil
      end

      if self~acceptNode(newNode) == .NodeFilter~FILTER_SKIP then do
          return self~getPreviousSibling(newNode, startNode)
      end
      return .nil
  end

  accept = self~acceptNode(newNode)
  if accept == .NodeFilter~FILTER_ACCEPT then do
      return newNode
  end
  else if accept == .NodeFilter~FILTER_SKIP then do
      child = self~getLastChild(newNode)
      if child == .nil then do
          return self~getPreviousSibling(newNode, startNode)
      end
      return child
  end
  else do
      return self~getPreviousSibling(newNode, startNode)
  end

::method getFirstChild private
  expose entityReferenceExpansion
  use strict arg node

  if entityReferenceExpansion & node~nodeType == .Node~ENTITY_REFERENCE_NODE then do
      return .nil
  end

  newNode = node~firstChild
  if newNode == .nil then do
      return .nil
  end

  accept = self~acceptNode(newNode)
  if accept == .NodeFilter~FILTER_ACCEPT then do
      return newNode
  end
  else if accept == .NodeFilter~FILTER_SKIP & newNode~hasChildNodes then do
      child = self~getFirstChild(newNode)
      if child == .nil then do
          return self~getNextSibling(newNode, node)
      end
  end
  else do
      return self~getNextSibling(newNode, node)
  end

::method getLastChild private
  expose entityReferenceExpansion
  use strict arg node

  if entityReferenceExpansion & node~nodeType == .Node~ENTITY_REFERENCE_NODE then do
      return .nil
  end

  newNode = self~lastChild
  if newNode == .nil then do
      return .nil
  end

  accept = self~acceptNode(newNode)
  if accept == .NodeFilter~FILTER_ACCEPT then do
      return newNode
  end
  else if accept == .NodeFilter~FILTER_SKIP & newNode~hasChildNodes then do
      child = self~getLastChild(newNode)
      if child == .nil then do
          return self~getPreviousSibling(newNode, node)
      end
  end
  else do
      return self~getPreviousSibling(newNode, node)
  end

::method acceptNode private
  expose nodeFilter whatToShowFlags
  use strict arg node

  if nodeFilter == .nil then do
      if whatToShowFlags[node~nodeType] then do
          return .NodeFilter~FILTER_ACCEPT
      end
      else do
          return .NodeFilter~FILTER_SKIP
      end
  end
  else do
      if whatToShowFlags[node~nodeType] then do
          return nodeFilter~accept(node)
      end
      else do
          return .NodeFilter~FILTER_SKIP
      end
  end

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: CoreDocument                                                        */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
::class "CoreDocument" subclass DocumentImpl public
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

::attribute nodeName GET
  use strict arg
  return "#document"

::method cloneNode
  use strict arg deep = .false
  newDoc = .CoreDocument~new
  self~cloneDocument(newDoc, deep)
  return newDoc

::method cloneDocument private
  expose identifers firstChild
  use arg newDoc, deep

  if deep then do
      reversedIdentifers = .nil
      if identifiers \= .nil then do
          reversedIdentifiers = .directory~new
          -- create the table using the inverse look up logic
          sup = identifiers~supplier
          do while sup~available
              reversedIdentifiers[sup~item] = sup~index
              sup~next
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
  else if type = .Node~DOCUMENT_TYPE_NODE then do
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
  else if type = .Node~DOCUMENT_TYPE_NODE then do
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
  return .AttrImpl~new(self, name)

::method createAttributeNS
  if arg() == 2 then do
      use strict arg namespaceURI, qualifiedName
      return .AttrImpl~new(self, qualifiedName, namespaceURI)
  end
  else do
      use strict arg namespaceURI, qualifiedName, localName
      return .AttrImpl~new(self, qualifiedName, namespaceURI, localName)
  end

::method createCDATASection
  use strict arg data
  return .CDATASectionImpl~new(self, data)

::method createComment
  use strict arg data
  return .CommentImpl~new(self, data)

::method createDocumentFragment
  use strict arg
  return .DocumentFragmentImpl~new(self)

::method createElement
  use strict arg tagname
  return .ElementImpl~new(self, tagname)

::method createElementNS
  if arg() == 2 then do
      use strict arg namespaceURI, qualifiedName
      return .ElementImpl~new(self, namespaceURI, qualifiedName)
  end
  else do
      use strict arg namespaceURI, qualifiedName, localName
      return .ElementImpl~new(self, namespaceURI, qualifiedName, localName)
  end

::method createEntityReference
  use strict arg name
  return .EntityReferenceImpl~new(self, name)

::method createProcessingInstruction
  use strict arg target, data
  return .ProcessingInstructionImpl~new(self, target, data)

::method createTextNode
  use strict arg data
  return .TextImpl~new(self, data)

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

  return .DocumentTypeImpl~new(self, qualifiedName, publicID, systemID)

::method createEntity
  use strict arg name
  return .EntityImpl~new(self, name)

::method createNotation
  use strict arg name
  return .NotationImpl~new(self, name)

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
                  newMap~setNamedItem(self~importNode(item, .true, .true, reversedIdentifers))
              end
          end
          sourceMap = source~notations
          newMap = newNode~notations
          -- copy all of the notations, if thee are any
          if sourceMap \= .nil then do
              do item over sourceMap
                  newMap~setNamedItem(self~importNode(item, .true, .true, reversedIdentifers))
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
          newNode~appendChild(self~importNode(child, .true, cloningDoc, reversedIdentifiers))
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
          source~isspecified = .true
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
