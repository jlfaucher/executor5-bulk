/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Description: An implementation of the Document Object Model in ooRexx.     */
/*                                                                            */
/* Copyright (c) 2012 Rexx Language Association. All rights reserved.         */
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


-- now that everything has been started up, poke the token class to create its
-- initial tables
.XPathToken~setup

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
::constant ANY_NODE 0
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
::method createProcessingInstruction abstract
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
/* Class: QName -- a qualified XML name                                       */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
::class "QName" public
::method init
  expose localName prefix namespaceURI

  localName = ""
  prefix = ""
  namespaceURI = ""

::attribute prefix
::attribute localName
::attribute namespaceURI

-- get the fully constructed node name
::attribute nodeName get
  expose prefix localName
  return prefix":"localName

-- test for a match between a node and this qualified name
::method matchesNode
  expose localName namespaceURI
  use strict arg node

  return localName == node~localName & namespaceURI == node~namespaceURI

-- handy string method
::method string
  return self~nodeName


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
  else if nodes~isA(.array) then
      -- make a copy of this
      nodes = nodes~copy
  -- must be a nodelist...get the items array from the list
  else nodes = nodes~makearray

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

-- access to the set of nodes
::attribute nodes

-- methods to support xpath/nodeset operations

-- append an item to the list
::method append
  expose nodes
  use strict arg item
  nodes~append(item)

-- merge two nodelists by adding all of the elements of the
-- argument list to the target list.
::method union
  use strict arg other
  loop node over other
      self~append(node)
  end
  return self   -- always return self as a value

-- return the index location of a given item.  Returns -1
-- if the item is not found
::method indexOf
  expose nodes
  use strict arg target

  index = nodes~itemIndex(target)

  if index == .nil then return -1
  -- need to convert this to zero-based for
  -- nodelists
  return index - 1

-- get the first item in the node list
::method first
  expose nodes
  return nodes[1]

-- get the last item in the node list
::method last
  expose nodes

  if nodes~isEmpty then return .nil
  return nodes[nodes~last]

-- get the item previous to a given one
::method previous
  expose nodes
  use strict arg target

  index = nodes~itemIndex(target)
  -- not found or the first one, return nothing
  if index == .nil | index = 1 then return .nil
  return nodes[index]

-- get the item followinging a given one
::method next
  expose nodes
  use strict arg target

  index = nodes~itemIndex(target)
  -- not found, return nothing
  if index == .nil then return .nil
  return nodes[index]

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: DeepNodeList                                                        */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class "DeepNodeList" subclass NodeListImpl public
::method init
  expose rootNode tagName changes nsName
  use strict arg rootNode, tagName, nsName = .nil

  self~init:super
  changes = 0

-- lazy search for the matching nodes.  This only fills in the cache
-- as far as the last valid requested position
::method item
  expose rootNode changes
  use strict arg index

  -- if the tree has changed, we need to restart this
  if rootNode~changes \= changes then do
      self~nodes = .array~new
      changes = rootNode~changes
  end

  currentSize = self~nodes~items
  if index < currentSize then do
      return self~nodes[index + 1]
  end
  else do
      -- we need to continue the traversal from the last node added
      if currentSize == 0 then do
          thisNode = rootNode
      end
      else do
          thisNode = self~nodes[currentSize]
      end
      -- keep adding up to the one we're looking for

      do while index > currentSize
          thisNode = self~nextMatchingElementAfter(thisNode)
          if thisNode \= .nil then do
              self~nodes~append(thisNode)
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
  return self~nodes~items

::method makearray
  expose nodes
  use strict arg
  -- this forces everything to be initialized
  self~item(999999999)
  return self~nodes~copy

::method nextMatchingElementAfter private
  expose rootNode tagName nsName
  use arg current

  do while current \= .nil
      -- go down to the first child if it has one
      if current~hasChildNodes then do
          current = current~firstChild
      end
      -- if not the root, then check for a sibling
      else if current \= rootNode, current~nextSibling \= .nil then do
          next = current~nextSibling
          current = next
      end
      else do
          next = .nil
          -- now we step "up and to the right" for as many attempts as needed
          -- or utnil we pop back to the root node
          loop while current \= rootNode
              current = current~parentNode
              next = current~nextSibling
              -- we have a next, so quit
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
/* Class: NodeSet                                                             */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

-- a nodeset used in an xpath expression
::class "NodeSet" public subclass NodeListImpl
::method init
  expose owner parent
  use strict arg owner, parent, nodes
  self~init:super(nodes)


-- apply a selection criteria to a nodeset, returning
-- a new nodeset with the selected nodes.
::method selectNodeSet
  expose owner
  use strict arg step, context

  -- get a result nodeset for the base
  nodeSet = .nodeset~new(owner, context)

  -- loop over all of the nodes in this set and apply
  -- the selection rules to each one, merging the result
  -- into the set
  loop node over self
      nodeSet~union(step~evaluateNodeSet(node, self))
  end
  return nodeSet

-- retrieve a named set of nodes from the set using a qualified name.
::method getNamedItems
  expose owner
  use strict arg name

  -- get a result nodeset for the base
  nodeSet = .nodeset~new(owner)

  -- wild card on the namespace?  This is a complete wildcard request
  if name~namespaceURI == '*' then do
      nodeSet~union(self)
  end
  -- wild card request with an explicit namespace?, return
  -- just URI matches
  else if name~localName == "*" then do
      loop node over self
         if name~namespaceURI == node~namespaceURI then nodeSet~append(node)
      end
  end
  -- no wild cards, so we're looking for explicit matches
  else do
      loop node over self
          if name~matchesNode(node) then nodeSet~appendNode
      end
  end

  return nodeSet

-- extract the nodes of the indicated type
::method getTypedItems
  expose owner
  use strict arg type

  -- get a result nodeset for the base
  nodeSet = .nodeset~new(owner)

  loop node over self
      if type == 0 | node~nodeType == type then do
          nodeSet~append(node)
      end
  end

-- methods for axis manipulations on nodesets

-- return the combined ancestor axis of the contents
::method ancestorAxis
  expose owner

  -- get a result nodeset for the base
  nodeSet = .nodeset~new(owner)

  loop node over self
      nodeSet~union(node~ancestorAxis)
  end
  return nodeSet

-- return the combined ancestor-or-self axis of the contents
::method ancestorOrSelfAxis
  expose owner

  -- get a result nodeset for the base
  nodeSet = .nodeset~new(owner)

  loop node over self
      nodeSet~union(node~ancestorOrSelfAxis)
  end
  return nodeSet

-- return the combined attribute axis of the contents
::method attributeAxis
  expose owner

  -- get a result nodeset for the base
  nodeSet = .nodeset~new(owner)

  loop node over self
      nodeSet~union(node~attributeAxis)
  end
  return nodeSet

-- return the combined child axis of the contents
::method childAxis
  expose owner

  -- get a result nodeset for the base
  nodeSet = .nodeset~new(owner)

  loop node over self
      nodeSet~union(node~childAxis)
  end
  return nodeSet

-- return the combined descendant axis of the contents
::method descendantAxis
  expose owner

  -- get a result nodeset for the base
  nodeSet = .nodeset~new(owner)

  loop node over self
      nodeSet~union(node~descendantAxis)
  end
  return nodeSet

-- return the combined reversed descendant axis of the contents
::method reversedDescendantAxis
  expose owner

  -- get a result nodeset for the base
  nodeSet = .nodeset~new(owner)

  loop node over self
      nodeSet~union(node~reversedDescendantAxis)
  end
  return nodeSet

-- return the combined descendant-or-self axis of the contents
::method descendantOrSelfAxis
  expose owner

  -- get a result nodeset for the base
  nodeSet = .nodeset~new(owner)

  loop node over self
      nodeSet~union(node~descendantOrSelfAxis)
  end
  return nodeSet

-- return the combined following axis of the contents
::method followingAxis
  expose owner

  -- get a result nodeset for the base
  nodeSet = .nodeset~new(owner)

  loop node over self
      nodeSet~union(node~followingAxis)
  end
  return nodeSet

-- return the combined following-sibling axis of the contents
::method followingSiblingAxis
  expose owner

  -- get a result nodeset for the base
  nodeSet = .nodeset~new(owner)

  loop node over self
      nodeSet~union(node~followingSiblingAxis)
  end
  return nodeSet


-- return the combined namespace axis of the contents
::method namespaceAxis
  expose owner

  -- get a result nodeset for the base
  nodeSet = .nodeset~new(owner)

  loop node over self
      nodeSet~union(node~namespaceAxis)
  end
  return nodeSet

-- return the combined parent axis of the contents
::method parentAxis
  expose owner

  -- get a result nodeset for the base
  nodeSet = .nodeset~new(owner)

  loop node over self
      nodeSet~union(node~parentAxis)
  end
  return nodeSet

-- return the combined preceding axis of the contents
::method precedingAxis
  expose owner

  -- get a result nodeset for the base
  nodeSet = .nodeset~new(owner)

  loop node over self
      nodeSet~union(node~precedingAxis)
  end
  return nodeSet

-- return the combined preceding-sibling axis of the contents
::method precedingSiblineAxis
  expose owner

  -- get a result nodeset for the base
  nodeSet = .nodeset~new(owner)

  loop node over self
      nodeSet~union(node~precedingSiblingAxis)
  end
  return nodeSet

-- return the combined self axis of the contents
::method selfAxis
  expose owner

  -- get a result nodeset for the base
  nodeSet = .nodeset~new(owner)

  loop node over self
      nodeSet~union(node~selfAxis)
  end
  return nodeSet

-- these are self containing
::attribute containingNodeSet get
  return self

-- filter this node set based on an expression criteria, returning
-- a new node set
::method filter
  expose owner
  use strict arg context, predicate

  -- get a result nodeset for the base
  nodeSet = .nodeset~new(owner)

  loop node over self
      -- evaluate the predicate expression against this node and container
      -- context.
      if  predicate.evaluatePredicate(node, self, s) then
          nodeSet~append(node)
  end

  return nodeSet

-- extract the nodes by qualified name, returning a new nodeset
::method getElementsByQualifiedName
  expose owner
  use strict arg qname

  -- get a result nodeset for the base
  nodeSet = .nodeset~new(owner)

  loop node over self
      -- evaluate the predicate expression against this node and container
      -- context.
      if  qname~matchesNode(node) then
          nodeSet~append(node)
  end

  return nodeSet

-- determine the index position of a given node within the nodeset.
-- returns -1 for not found
::method positionOf
  use strict arg target

  loop i = 0 to self~length
      if self~item(i) then return i
  end
  return -1  -- not found

-- test if the node contains a give target node
::method contains
  use strict arg target
  return self~positionOf(target) \= -1


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
-- for node types that don't support a real localname,
-- this is the same as nodeName
::attribute localName GET
  use strict arg
  return self~nodeName
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

-- return a change count for the node...this just returns
-- the document change count
::method changes
  return self~ownerdocument~changes

-- select a set of nodes using an xpath expression
::method selectNodeSet
  use strict arg path

  -- any errors are just treated as selection failures and return an
  -- empty node set
  signal on syntax

  -- parse the expression and execute using this node as the starting context
  parser = .XpathParser~new(self~document, path)
  expr = parse~parse
  return expr~evaluate(self, .nil)

  syntax:
  -- just return an empty node set


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Node                                                                */
/*        Private methods                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::attribute ctr class private
::attribute id       private

-- retrieve the ancestor axis
::attribute ancestorAxis get
  parentNode = self~parentNode
  -- if not at the top of the tree, have our parent handle this
  if parentNode~nodeType \= self~DOCUMENT_NODE then
      return parentNode~ancestorOrSelfAxis
  -- at the top, so there are no ancestors.  Just return an empty
  -- nodeset
  else return .NodeSet~new(self~document, parentNode)

-- retrieve the ancestor-or-self axis
::attribute ancestorOrSelfAxis get
  -- merge the self list with the ancestor
  return self~selfAxis~union(self~ancestorAxis)

-- retrieve the attribute axis
::attribute attributeAxis get
  -- node set of attributes

  --TODO:  Make sure NodeImpl has an attributes method and attributes
  -- returns the appropriate thing
  return .nodeset~new(self~document, self~parentNode, self~attributes)

-- retrieve the child axis
::attribute childAxis get
  return .nodeset~new(self~document, self~parentNode, self~childNodes)

-- retrieve the descendant axis
::attribute descendantAxis get
  nodeSet = .nodeset~new(self~document, self~parentNode)
  -- merge in all of the descendants
  loop child over self~childNodes
      nodeSet~union(child~descendantOrSelfAxis)
  end
  return nodeSet

-- retrieve the reversed descendant axis.  This
-- builds the list in reverse descenant order
::attribute reversedDescendantAxis get
  nodeSet = .nodeset~new(self~document, self~parentNode)

  children = self~childNodes
  -- merge in all of the descendants in reverse order
  loop i = children~length to 1 by -1
      nodeSet~union(children[i]~reversedDescendantOrSelfAxis)
  end
  return nodeSet

-- descendant axis + self
::attribute descendantOrSelfAxis get
  return self~selfAxis~union(self~descendantAxis)

-- and the reversed version of the descendant + self axis
::attribute reversedDescendantOrSelfAxis get
  return self~selfAxis~union(self~reversedDescendantAxis)

-- The following axis...this is all of the following siblings +
-- their descendants
::attribute followingAxis get
  nodeSet = .nodeSet~new(self~document, self~parentNode)

  nextSibling = self~nextSibling
  -- if we have a sibling, then get its descendant axis and
  -- also get its following axis.
  if nextSibling \== .nil then do
      nodeSet~append(nextSibling)
      nodeSet~union(nextSibling~descendantAxis)
      nodeSet~union(nextSibling~followingAxis)
  end

-- following sibling axis is like the following axis, but omits the
-- children of the siblings
::attribute followingSiblingAxis get
  nodeSet = .nodeSet~new(self~document, self~parentNode)

  nextSibling = self~nextSibling
  -- if we have a sibling, then get its descendant axis and
  -- also get its following axis.
  if nextSibling \== .nil then do
      nodeSet~append(nextSibling)
      nodeSet~union(nextSibling~followingSiblingAxis)
  end

-- the nodeset containing just the parent (if it exists)
::attribute parentAxis get
  nodeSet = .nodeSet~new(self~document, self~parentNode)

  if self~parentNode \= .nil then nodeSet~append(self~parentNode)
  return nodeSet

-- The preceding axis...this is all of the preceding siblings +
-- their descendants
::attribute precedingAxis get
  nodeSet = .nodeSet~new(self~document, self~parentNode)

  previousSibling = self~previousSibling
  -- if we have a sibling, then get its descendant axis and
  -- also get its preceding axis.
  if previousSibling \== .nil then do
      nodeSet~append(previousSibling)
      nodeSet~union(previousSibling~descendantAxis)
      nodeSet~union(previousSibling~precedingAxis)
  end

-- preceding sibling axis is like the preceding axis, but omits the
-- children of the siblings
::attribute precedingSiblingAxis get
  nodeSet = .nodeSet~new(self~document, self~parentNode)

  nextSibling = self~nextSibling
  -- if we have a sibling, then get its descendant axis and
  -- also get its preceding axis.
  if nextSibling \== .nil then do
      nodeSet~append(previousSibling)
      nodeSet~union(previousSibling~precedingSiblingAxis)
  end

-- the nodeset containing just this node
::attribute selfAxis get
  nodeSet = .nodeSet~new(self~document, self~parentNode)
  nodeSet~append(self)
  return nodeSet

-- the nodeset containing the tree root
::attribute rootAxis get
  nodeSet = .nodeSet~new(self~document, self~parentNode)
  nodeSet~append(self~document~documentElement)
  return nodeSet

-- methods of NodeList that the node implements directly.  superclasses will
-- implement this more fully

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

::attribute ownerDocument SET private
  expose firstChild

  use strict arg doc
  -- we need to set this at the higher levels too
  forward class(super) continue
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
  expose firstChild lastChild childNodes
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
  self~ownerDocument~insertingNode(self, replace)

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
  self~ownerDocument~insertedNode(self, newChild, replace)
  return newChild

::method removeChild
  expose firstChild lastChild childNodes
  use strict arg oldChild, replace = .false

  self~ownerDocument~removingNode(self, oldChild, replace)

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

  self~ownerDocument~removedNode(self, replace)
  return oldChild

::method replaceChild
  use strict arg newChild, oldChild

  self~insertBefore(newChild, oldChild, .true)
  if newChild \== oldChild then do
      self~removeChild(oldChild, .true)
  end

  self~ownerDocument~replacedNode(self)
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
  forward message("NODEVALUE")
::attribute data SET
  forward message("NODEVALUE=")
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

  self~nodeValue = data||newData

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
  data = newData

  self~ownerDocument~deletedText(self, offset, count)

::method insertData
  expose data
  use strict arg offset, newData, replace = .false

  -- NB:  In this case, we don't add one to the offset because
  -- the Rexx insert function inserts after the given offset, not
  -- before.  This actually works to our advantage.
  newValue = data~insert(newData, offset)
  data = newValue

  self~ownerDocument~insertedText(self, offset, newData~length)

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

::method substringData
  expose data
  use strict arg offset, count

  return data~substr(offset + 1, count)


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Text -- a DOM comment node                                          */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class "CommentImpl" public subclass CharacterDataImpl inherit Comment
::attribute nodeType GET
  use strict arg
  return .Node~COMMENT_NODE

::attribute nodeName GET
  use strict arg
  return "#comment"


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
  use strict arg offset

  newText = self~ownerDocument~createTextNode(self~data~substr(offset + 1))
  self~nodeValue = self~data~substr(1, offset)

  -- now insert the new text node
  parent = self~parentNode
  if parent \= .nil then do
      parent~insertBefore(newText, self~nextSibling)
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
  -- we are our own owning document for purposes of child appends
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
::attribute changes GET

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


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Section:  XPath parser and evaluation engine                               */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


::class "XPath"
::constant INVALID_OPERATOR_ERROR "Invalid XPath operator found"
::constant INVALID_EXPRESSION_ERROR "Invalid XPath expression."
::constant INVALID_VARIABLE_REFERENCE_ERROR "Variable references not supported"
::constant MISSING_QUOTE_ERROR "Missing closing literal string quote"
::constant MISSING_PAREN_ERROR "Missing closing parentheses"
::constant INVALID_AXIS_NAME_ERROR "Invalid XPath axis name"
::constant INVALID_TYPE_TEST_ERROR "Invalid node type."
::constant INVALID_NAME_TEST_ERROR "Invalid node name type."
::constant MISSING_ARGUMENT_ERROR "Missing function argument."
::constant INCORRECT_FUNCTION_ARGUMENTS_ERROR "Incorrect function arguments."
::constant NODESET_ERROR "Expression did not evaluate to a node set."
::constant UNKNOWN_FUNCTION_ERROR "Unknown function name in expression."

-- raise an error for an xpath problem
::method xpathError class
  use strict arg reason
  raise syntax 93.900 array(reason)

-- a queue of tokens
::class "TokenQueue"
::method init
  expose queue currentToken
  queue = .queue~new
  -- this is used for reading the tokens
  currentToken = 1

-- add a token to the queue
::method addToken
  expose queue
  use strict arg token
  queue~queue(token)

-- reset the token position to the beginning
::method rewind
  expose currentToken
  currentToken = 1

-- test if there are more tokens
::method hasMore
  expose queue currentToken
  return currentToken <= queue~size

-- get the next token, stepping the position
::method nextToken
  expose queue currentToken
  item = queue[currentToken]
  currentToken += 1
  return item

-- back up a token in the queue
::method previousToken
  expose currentToken

  if currentToken > 1 then currentToken += 1

-- peek at the next (current) token
::method peekToken
  expose queue currentToken
  return queue[currentToken]

-- remove the first token from the queue
::method pop
  expose queue
  queue~pop


-- a generic token value.  Most of the work here is done via class methods.
-- the token itself is very general and very simple
::class "XPathToken"

-- set up all of the initial token types
::method setup class
  expose simpleOperators namedOpertors nodeTypes axisTypes
  simpleOperators = .directory~new
  namedOperators = .directory~new
  nodeTypes = .directory~new
  axisTypes = .directory~new

  -- this is a dummy marker token used as the first one
  self~dummy = .XPathToken~new('')
  -- special terminator token function argument lists
  self~function_arg = .XPathToken~new(',)')

  -- create the various constant tokens
  self~open_paren = .XPathToken~new('(')
  self~close_paren = .XPathToken~new(')', .true)
  self~open_bracket = .XPathToken~new('[')
  self~close_bracket = .XPathToken~new(']', .true)
  self~period = .XPathToken~new('.', .true)
  self~double_period = .XPathToken~new('..', .true)
  self~at_sign = .XPathToken~new('@')
  self~comma = .XPathToken~new(',')
  self~double_colon = .XPathToken~new('::')
  self~slash = .XPathToken~new('/')
  self~double_slash = .XPathToken~new('//')
  self~union = .XPathOperatorToken~new('|', 0)
  self~addition = .XPathOperatorToken~new('+', 6)
  self~subtraction = .XPathOperatorToken~new('-', 6)
  self~equal = .XPathOperatorToken~new('=', 3)
  self~not_equal = .XPathOperatorToken~new('!=', 3)
  self~less_than = .XPathOperatorToken~new('<', 4)
  self~less_than_equal = .XPathOperatorToken~new('<=', 4)
  self~greater_than = .XPathOperatorToken~new('>', 4)
  self~greater_than_equal = .XPathOperatorToken~new('>=', 4)
  self~multiply = .XPathOperatorToken~new('*', 5)
  self~wildcard = .XPathToken~new('*', .true)

  -- this is a subset of the operator tokens.  The ones
  -- in this list can be resolved immediately just off of a
  -- single character.  Others will need some additional processing
  simpleOperators["open_paren"] =          self~open_paren
  simpleOperators["close_paren"] =         self~close_paren
  simpleOperators["open_bracket"] =        self~open_bracket
  simpleOperators["close_bracket"] =       self~close_bracket
  simpleOperators["at_sign"] =             self~at_sign
  simpleOperators["comma"] =               self~comma
  simpleOperators["union"] =               self~union
  simpleOperators["addition"] =            self~addition
  simpleOperators["subtraction"] =         self~subtraction
  simpleOperators["equal"] =               self~equal

  self~and = .XPathOperatorToken~new('and', 2)
  self~or = .XPathOperatorToken~new('or', 1)
  self~mod = .XPathOperatorToken~new('mod', 5)
  self~div = .XPathOperatorToken~new('div', 5)

  -- the named operators
  namedoperators["and"] = self~and
  namedoperators["or"] = self~or
  namedoperators["mod"] = self~mod
  namedoperators["div"] = self~div

  self~comment = .XPathToken~new("NODETYPE", , 'comment')
  self~text = .XPathToken~new("NODETYPE", , 'text')
  self~processing_instruction = .XPathToken~new("NODETYPE", , 'processing_instruction')
  self~node = .XPathToken~new("NODETYPE", , 'node')
  self~element = .XPathToken~new("NODETYPE", , 'element')

  -- quick resolution table for the node types
  nodetypes["comment"] =                  self~comment
  nodetypes["text"] =                     self~text
  nodetypes["processing_instruction"] =   self~processing_instruction
  nodetypes["node"] =                     self~node
  nodetypes["element"] =                  self~element

  self~ancestor = .XPathToken~new('ancestor')
  self~ancestor_or_self = .XPathToken~new('ancestor_or_self')
  self~attribute = .XPathToken~new('attribute')
  self~child = .XPathToken~new('child')
  self~descendant = .XPathToken~new('descendant')
  self~descendant_or_self = .XPathToken~new('descendant_or_self')
  self~following = .XPathToken~new('following')
  self~following_sibling = .XPathToken~new('following_sibling')
  self~namespace = .XPathToken~new('namespace')
  self~parent = .XPathToken~new('parent')
  self~preceding = .XPathToken~new('preceding')
  self~preceding_sibling = .XPathToken~new('preceding_sibling')
  self~self = .XPathToken~new('self')

  -- build the axis table for quick parsing
  axistypes["ancestor"] = self~ancestor
  axistypes["ancestor_or_self"] = self~ancestor_or_self
  axistypes["attribute"] = self~attribute
  axistypes["child"] = self~child
  axistypes["desendant"] = self~descendant
  axistypes["desendant_or_self"] = self~descendant_or_self
  axistypes["following"] = self~following
  axistypes["following_sibling"] = self~following_sibling
  axistypes["namespace"] = self~namespace
  axistypes["parent"] = self~parent
  axistypes["preceding"] = self~preceding
  axistypes["preceding_sibling"] = self~preceding_sibling
  axistypes["self"] = self~self


-- singletons for the different imutable operator types
::attribute dummy class
::attribute function_arg class
::attribute open_paren class
::attribute close_paren class
::attribute open_bracket class
::attribute close_bracket class
::attribute period class
::attribute double_period class
::attribute at_sign class
::attribute comma class
::attribute double_colon class
::attribute slash class
::attribute double_slash class
::attribute union class
::attribute addition class
::attribute subtraction class
::attribute equal class
::attribute not_equal class
::attribute less_than class
::attribute less_than_equal class
::attribute greater_than class
::attribute greater_than_equal class
::attribute multiply class
::attribute wildcard class
::attribute and class
::attribute or class
::attribute mod class
::attribute div class
::attribute node class
::attribute text class
::attribute element class
::attribute processing_instruction class
::attribute comment class
::attribute ancestor class
::attribute ancestor_or_self class
::attribute attribute class
::attribute child class
::attribute descendant class
::attribute descendant_or_self class
::attribute following class
::attribute following_sibling class
::attribute namespace class
::attribute parent class
::attribute preceding class
::attribute preceding_sibling class
::attribute preceding_self class
::attribute self class

-- create a literal string token
::method literalToken class
  use strict arg value
  return .XPathToken~new("LITERAL", .true, value)

-- create a function token
::method functionToken class
  use strict arg prefix, name
  return .XPathToken~new("FUNCTION", .false, name, prefix)

-- create a namespace token
::method nameToken class
  use strict arg name
  return .XPathToken~new("NCNAME", .true, name)

-- create a qualified name token
::method qualifiedNameToken class
  use strict arg prefix, name
  return .XPathToken~new("QNAME", .true, name, prefix)

-- create a number token
::method numberToken class
  use strict arg value
  return .XPathToken~new("NUMBER", .true, value)

-- create a variable reference token
::method variableReferenceToken class
  use strict arg prefix, name
  return .XPathToken~new("VARIABLE", .true, name, prefix)

  -- resolve an operator token based on the character.
  -- returns .nil if the operator is not simply resolvable
::method resolveSimpleOperator class
  expose simpleOperators
  use strict arg op
  return simpleOperators[op]

  -- resolve an operator token based on the operator name.
  -- returns .nil if the operator is not simply resolvable
::method resolveNamedOperator class
  expose namedOperators
  use strict arg op
  return namedOperators[op]

  -- resolve a node type token based on the name.
  -- returns .nil if the operator is not simply resolvable
::method resolveNodeType class
  expose nodeTypes
  use strict arg op
  return nodeTypes[op]


  -- resolve a axis type token based on the name.
  -- returns .nil if the operator is not simply resolvable
::method resolveAxis class
  expose axisTypes
  use strict arg op
  return axisTypes[op]

::method init
  expose isMultTarget name prefix
  use strict arg type, isMultTarget = .false, name = .nil, prefix = .nil

-- the identifier of the token.  For operators, this will be
-- the operator type itself.  All operator tokens are singletons.
-- This can also be STRING, NAME, QNAME, or NUMBER
::attribute type GET
-- indicates whether this token can be used as a multiply target
::attribute isMultTarget GET
::attribute prefix GET
::attribute name GET

-- special test for operator tokens -- default is always false
::attribute isOperator GET

-- special subclass for the operators
::class "XPathOperatorToken" subclass XPathToken
::method init
  expose precedence
  use strict arg type, precedence
  -- operators use default values for everything else
  self~init:super(type)

-- Override of the operator test
::attribute isOperator GET
  return .true
-- operator precedence value
::attribute precedence GET

-- a class to scan an xpath expression and break it up into its tokens.
::class "XPathScanner"
::method init class
  expose nonascii namecharacters
  nonascii = xrange('80'x, 'ff'x)
  namecharacters = 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_' || nonascii

::constant whitespace '090A0D20'x
::constant other      '2325263F5C5E607B7D7E7F'x
::constant quote      '''"'
::constant digit      '0123456789'
::constant letter     'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ'
-- all characters above '80'x
::attribute nonascii GET
-- all characters valid at the start of an xpath name.  This is letters,
-- underscore, and the nonascii characters
::attribute namecharacters GET

-- raise an error as the result of a scanning error
::method error private
  use strict arg currentOffset, msg
  raise syntax 93.900 array('Error at offset' currentOffset':' msg, currentOffset)

-- scan an expression string, breaking this up into a list of tokens.
::method scanExpression
  use strict arg data

  currentOffset = 1
  endOffset = data~length

  -- create a queue for the tokens and make the first token a dummy marker
  tokens = .tokenqueue~new
  tokens~addqueue(.XPathToken~dummy)

  loop name scan forever
      -- skip over any blanks
      currentOffset = data~verify(self~whitespace, currentOffset)
      -- hit the end of what we're parsing?  Done
      if currentOffset > endOffset then leave
      -- skip over any blanks
      currentOffset = data~verify(self~whitespace, currentOffset)
      -- all whitespace left, we're done
      if currentOffset = 0 then leave scan

      ch = data~subchar(currentOffset)
      -- some single character tokens can be resolved and handled
      -- quickly if this is one of these, we can go around
      token = .XPathToken~resolveSimpleOperator(ch)
      if token \== .nil then do
          tokens~addToken(token)
          currentOffset += 1
          iterate scan
      end

      -- something requiring a little more processing
      select
          -- potentially "::"
          when ch == ':' then do
             currentOffset += 1
             ch = data~subchar(currentOffset)
             -- this also catches eof-of-line because subchar will return
             -- "" for out-of-range.
             if ch \== ':' then self~error(currentOffset - 1, "Invalid token ':'")
             tokens~addToken(.XPathToken~double_colon)
             currentOffset += 1
          end
          -- can be either '/' or '//'
          when ch == '/' then do
             currentOffset += 1
             -- single colon at the end is not valid
             if currentOffset > endOffset then do
                tokens~addToken(.XPathToken~slash)
                leave scan
             end
             -- grab the next character and see if this is a
             -- double or a single
             ch = data~subchar(currentOffset)
             if ch == '/' then do
                tokens~addToken(.XPathToken~doubleslash)
                currentOffset += 1
             end
             else do
                tokens~addToken(.XPathToken~slash)
             end
          end
          -- non-equality operator...potentially
          when ch == '!' then do
             currentOffset += 1
             -- single bang at the end is not valid
             ch = data~subchar(currentOffset)
             -- only '=' is valid after a '!'
             if ch \== '=' then self~error(currentOffset - 1, "Invalid token '!'")
             tokens~addToken(.XPathToken~not_equal)
             currentOffset += 1
          end
          -- the relative operators can be either "<" or "<="
          when ch == '<' then do
             currentOffset += 1
             -- Just a less than
             if currentOffset > endOffset then do
                tokens~addToken(.XPathToken~less_than)
                currentOffset += 1
             end
             else do
                 ch = data~subchar(currentOffset)
                 -- have the "="?
                 if ch == '=' then do
                     tokens~addToken(.XPathToken~less_than_equal)
                     currentOffset += 1
                 end
                 else do
                     -- just the single form
                     tokens~addToken(.XPathToken~less_than)
                 end
             end
          end
          -- and the greater than forms
          when ch == '>' then do
             currentOffset += 1
             -- Just a less than
             if currentOffset > endOffset then do
                tokens~addToken(.XPathToken~greater_than)
                currentOffset += 1
             end
             else do
                 ch = data~subchar(currentOffset)
                 -- have the "="?
                 if ch == '=' then do
                     tokens~addToken(.XPathToken~greater_than_equal)
                     currentOffset += 1
                 end
                 else do
                     -- just the single form
                     tokens~addToken(.XPathToken~greater_than)
                 end
             end
          end
          -- have a quoted string.  This is defined by the spec as
          --
          -- [29] Literal ::= '"' [^"]* '"' | "'" [^']* "'"
          --
          -- either-or on the delimiters and no escaping
          when ch~matchChar(1, self~quote) then do
              currentOffset += 1
              -- find the closing point
              endquote = data~pos(ch, currentOffset)
              if endquote = 0 then self~error(currentOffset - 1, "Missing close quote on string")
              value = data~substr(currentOffset, endQuote - (currentOffset))
              -- add a literal token
              tokens~addToken(.XPathToken~literalToken(value))
              currentOffset = endquote + 1
          end
          -- this is potentially a number.  The xpath rule for numbers is:
          -- [30] Number ::= Digits ('.' Digits?)? | '.' Digits
          when ch == '.' then do
              -- are we at the end?  This can't be a number so add a
              -- period token and get out
              if currentOffset = endOffset then do
                 tokens~addToken(.XPathToken~period)
                 currentOffset += 1
                 leave scan
              end
              -- get the next character
              ch = data~subchar(currentOffset + 1)
              -- another period is a double period token
              if ch == '.' then do
                 tokens~addToken(.XPathToken~double_period)
                 currentOffset += 2
              end
              -- period followed by '/' or '|' scans as a period token
              else if ch == '/' then do
                 tokens~addToken(.XPathToken~period)
                 currentOffset += 1
              end
              else if ch == '|' then do
                 tokens~addToken(.XPathToken~period)
                 currentOffset += 1
              end
              -- have a digit...bingo, this is a number
              else if ch~matchChar(1, self~digits) then do
                  -- scan off a number
                  currentOffset = self~scanNumber(tokens, data, currentOffset, endOffset);
              end
              -- a period followed by whitespace is only legal if this is the last token
              -- OR the next token is a '|'.  This scans a single period in that case.
              else if ch~matchChar(1, self~whitespace) then do
                  nonblank = data~verify(self~whitespace, currentOffset + 1)
                  if nonBlank = 0 then do
                     tokens~addToken(.XPathToken~period)
                     currentOffset = endOffset + 1
                  end
                  else if data~subchar(nonBlank) == '|' then do
                     tokens~addToken(.XPathToken~period)
                     currentOffset = nonBlank
                  end
                  else do
                      -- this is an error
                      self~xpathError("Invalid XPath expression")
                  end
              end
          end
          -- a number, as defined by the xpath spec
          --
          -- [30] Number ::= Digits ('.' Digits?)? | '.' Digits
          -- [31] Digits ::= [0-9]+
          --
          when ch~matchChar(1, self~digit) then do
              -- scan off a number
              currentOffset = self~scanNumber(tokens, data, endOffset, currentOffset);
          end
          -- a * can be either a wildcard selector on a name or the multiply
          -- operation, depending on what the previous token was
          --
          -- [37] NameTest ::= '*' | NCName ':' '*' | QName
          -- [34] MultiplyOperator ::= '*'
          --
          --
          -- 3.7 Lexical Structure
          --
          --  If there is a preceding token and the preceding token is not one of @, ::, (, [, , or
          --  an Operator, then a * must be recognized as a MultiplyOperator.
          --
          -- Otherwise, the token must not be recognized as a MultiplyOperator.
          --
          when ch == '*' then do
              if tokens~lastToken~isMultTarget then do
                 tokens~addToken(.XPathToken~multiply)
              end
              else do
                 tokens~addToken(.XPathToken~wildcard)
              end
              currentOffset += 1
          end
          -- a name of some sort.  These are complicated, because the meaning depends
          -- both on the preceeding token AND the following tokens.  The spec definitions are:
          --
          -- 3.7 Lexical Structure
          --
          --  If there is a preceding token and the preceding token is not one of @, ::, (, [, , or
          --  an Operator, then an NCName must be recognized as an OperatorName.
          --
          --  If the character following an NCName (possibly after intervening ExprWhitespace) is (,
          --  then the token must be recognized as a NodeType or a FunctionName.
          --
          --  If the two characters following an NCName (possibly after intervening ExprWhitespace)
          --  are ::, then the token must be recognized as an AxisName.
          --
          --  Otherwise, the token must not be recognized as an OperatorName, a NodeType, a
          --  FunctionName, or an AxisName.
          --
          -- [33] OperatorName ::= 'and' | 'or' | 'mod' | 'div'
          -- [38] NodeType ::= 'comment' | 'text' | 'processing-instruction' | 'node'
          -- [35] FunctionName ::= QName - NodeType
          -- [6] AxisName ::= (see above)
          --
          -- [37] NameTest ::= '*' | NCName ':' '*' | QName
          -- [5] NCName ::= (Letter | '_') (NCNameChar)*
          -- [?] NCNameChar ::= Letter | Digit | '.' | '-' | '_'  (ascii subset of 'NCNameChar')
          -- [?] QName ::= (NCName ':')? NCName
          -- [?] Letter ::= [A-Za-z]                              (ascii subset of 'Letter')
          -- [?] Digit ::= [0-9]                                  (ascii subset of 'Digit')
          --
          when ch~matchChar(1, self~nameCharacters) then do
              nameOffset = currentOffset
              -- scan off the name
              currentOffset = self~NCName(data, currentOffset, endOffset)
              -- NB:  This will return a null string if we're past the end
              -- of the string.  This is a convenient eol marker.
              ch = data~subChar(currentOffset)
              -- extract the name
              name = data~substr(nameOffset, currentOffset - nameOffset)
              -- no prefix yet
              prefix = ""
              -- potential qualified name form?  We need to look
              -- ahead a little, then check out the previous token to
              -- understand how this needs to be treated.
              if ch == ':' then do
                  -- this is just a standard name...so far.
                  nametype = "NAME"
                  currentOffset += 1
                  -- invalid name if at the end
                  if currentOffset > endOffset then self~error(currentOffset - 1, "Missing local name part for qualified name")
                  ch = data~subchar(currentOffset)
                  -- wildcarded qualified name?
                  if ch == '*' then do
                      currentOffset += 1
                      ch = data~subchar(currentOffset)
                      -- this is a more complicated name type
                      nametype = "NCNAME"
                  end
                  -- double colon form is an axis name
                  else if ch == ':' then do
                      currentOffset += 1
                      ch = data~subchar(currentOffset)
                      -- the axis forms require special attention later
                      nametype = "AXIS"
                  end
                  -- this is a qualified name form
                  else do
                     -- the previously parsed name is now the prefix
                     prefix = name
                     nameOffset = currentOffset
                     -- scan off the trailing name
                     currentOffset = self~scanNCName(data, currentOffset, endOffset)
                     -- this is an error if not followed by a name
                     if nameOffset = currentOffset then self~error(currentOffset - 1, "Missing local name part for qualified name")
                     -- get the following character (note that ch will be '' if
                     -- there isn't anything left
                     ch = data~subchar(currentOffset)
                     -- get the name part
                     name = data~substr(nameOffset, currentOffset - nameOffset)
                  end
              end
              -- whitespace is allowed here, so skip over it
              currentOffset = self~skipWhiteSpace(data, currentOffset, endOffset)
              ch = data~charAt(currentOffset)
              --
              --  If there is a preceding token and the preceding token is not one of @, ::, (, [, , or
              --  an Operator, then an NCName must be recognized as an OperatorName.
              --
              if tokens~lastToken~isMultTarget then do
                  -- resolve to a the appropriate operator
                  token = .XPathToken~resolveNamedOperator(name)
                  if token == .nil then self~error(currentOffset - 1, "invalid named operator" name)
                  tokens~addToken(token)
                  -- ok, this was an operator, but it might have been disguised as a
                  -- complex name...diagnose that
                  if nametype \= "NAME" then self~error(currentOffset - 1, "operator name used as a qualified name")
              end
              -- some sort of valid name expected here...but this might also be a
              -- nodetype or function name if followed by a '('
              else do
                 -- if we have a simple name followed by a paren, this is special
                 if ch == '(' & nametype == "NAME" then do
                     nodetype = .XPathToken~resolveNodeType(name)
                     if nodetype \= .nil then do
                         tokens~addToken(nodetype)
                     end
                     else do
                         tokens~addToken(.XPathToken~functionToken(prefix, name))
                     end
                     -- add the open paren that indicates the special nature
                     tokens~addToken(.XPathToken~openParen)
                     currentOffset += 1
                 end
                 -- ok, now check for axis names...We might already have detected this, but there could have
                 -- been intervening whitespace before the double colon
                 else if nametype == "AXIS" | (ch == ':' & data.subchar(currentOffset + 1) == ':') then do
                     axis = .XPathToken~resolveAxis(name)
                     if axis == .nil then self~error(currentOffset - 1, "Invalid axis name" name)
                     tokens~addToken(axis)
                     -- add the double colon marker
                     tokens~addToken(.XPathToken~double_colon)
                     -- if we detected this here, then step over the double colon
                     if nametype \= "AXIS" then currentOffset += 1
                 end
                 else do
                     if nametype == "NCNAME" then do
                        -- get a namespace indicator
                        tokens~addToken(.XPathToken~nameToken(name))
                     end
                     -- just a qualified name (finally!)
                     else do
                        -- get a qualified name indicator
                        tokens~addToken(.XPathToken~qualifiedNameToken(prefix, name))
                     end
                 end
              end
          end
          -- an external variable reference
          --
          -- [36] VariableReference ::= '$' QName
          --
          when ch == '$' then do
             currentOffset += 1
             -- invalid name if at the end
             if currentOffset > endOffset then self~error(currentOffset - 1, "Missing variable reference name")
             nameOffset = currentOffset
             currentOffset = self~scanNCName(data, currentOffset, endOffset)
             if nameOffset = currentOffset then self~error(currentOffset - 1, "Missing variable reference name")
             ch = data~subchar(currentOffset)
             name = data~substr(nameOffset, currentOffset - nameOffset)
             prefix = ""
             -- if this has the form of a qualified name, then the name we just scanned off is the
             -- prefix
             if ch == ':' then do
                 prefix = name
                 currentOffset += 1
                 -- invalid name if at the end
                 if currentOffset > endOffset then self~error(currentOffset - 1, "Invalid qualified name")
                 nameOffset = currentOffset
                 currentOffset = self~scanNCName(data, currentOffset, endOffset)
                 if nameOffset = currentOffset then self~error(currentOffset - 1, "Invalid qualified name")
                 name = data~substr(nameOffset, currentOffset - nameOffset)
             end
             tokens~addToken(.XPathToken~variableReferenceToken(prefix, name))
          end
          -- some sort of invalid token type
          otherwise do
              self~error(currentOffset - 1, "Invalid character in expression" ch)
          end
      end
  end
  -- everything is scanned, and we have a queue of tokens to process now
  -- we need to pull the dummy token from the front
  tokens~pull
  return tokens

-- scan an NCNAME from the expression.  A NCNAME is defined as:
--
--
-- [5] NCName ::= (Letter | '_') (NCNameChar)*
-- [6] NCNameChar ::= Letter | Digit | '.' | '-' | '_' | CombiningChar | Extender
--
::method scanNCName
  use strict arg data, currentOffset, endOffset
  ch = data~subchar(currentOffset)
  -- use the XMLCHAR class to check this...
  -- if not a valid first character, we're done
  if \XMLChar~isNameStart(ch) then do
      return currentOffset
  end

  currentOffset = data~verify(xmlchars~name, currentOffset + 1)
  if currentOffset = 0 then currentOffset = endOffset + 1
  return currentOffset

-- scan off a number value, defined as
--
--
-- [30] Number ::= Digits ('.' Digits?)? | '.' Digits
-- [31] Digits ::= [0-9]+
--
::method scanNumber
  use arg tokens, data, currentOffset, endOffset

  start = currentOffset
  -- we know we at least have a valid first character, which might be a period
  -- scan over any leading digits
  currentOffset = data~verify(self~digits, currentOffset)
  -- are we at a period now?
  if data~match(currentOffset, '.') then do
      period = currentOffset
      currentOffset = data~verify(self~digit, currentOffset)
      if currentOffset = 0 then currentOffset = period + 1
  end

  number = data~substr(start, currentOffset - start)
  tokens~addToken(.XPathToken~numbertoken(number))
  return currentOffset;

-- test for a node match
::class "NodeTest"
::method init
  expose nameTest typeTest
  use strict arg nameTest = .nil, typeTest = (.Node~ANY_NODE)

-- apply the match logic
::method apply
  expose nameTest typeTest
  use strict arg nodeset

  if nameTest \= .nil then return nodeset~getNamedItems(nameTest)
  else nodeset~getTypedItems(typeTest)

-- base class for an xpath expression term
::class XpathExpr
-- evaluate the expression
::method evaluate
  use strict arg context, container
  return .nil

::method evaluateBoolean
  use strict arg context, container

  value = self~evaluate(context, container)

  if \value~datatype('o') then XPath~error(.Xpath~BOOLEAN_ERROR)
  return value

::method evaluateString
  use strict arg context, container

  value = self~evaluate(context, container)

  if \value~isA(.string) then XPath~error(.Xpath~STRING_VALUE_ERROR)
  return value

::method evaluateNumber
  use strict arg context, container

  value = self~evaluate(context, container)

  if \value~datatype('Number') then XPath~error(.Xpath~NUMBER_VALUE_ERROR)
  return value

::method evaluateNodeSet
  use strict arg context, container

  value = self~evaluate(context, container)

  if \value~isA(.NodeSet) then XPath~error(.Xpath~NODESET_VALUE_ERROR)
  return value

::method evaluatePredicate
  use strict arg context, container, proximityPosition

  value = self~evaluate(context, container)
  if value~isA(.string), value~datatype('Number') then
      return result = proximityPosition
  -- xpath has funny rules for boolean true/false, so this requires
  -- some inspection
  return self~convertBoolean(result)

-- Apply the xpath rules for boolean true/false to an expression result
::method convertBoolean
  use strict arg value

  -- special rules for string values
  if value~isA(.string), value~datatype('Number') then do
      -- if this is a numberic value, then non-zero is true, zero is false
      if value~datatype('Number') then
         -- non-exact comparison here...
         return value \= 0
      -- the strings true and false are special
      if value == "true" then return .true
      if vallue == "false" then return .false
      -- if this is a string value, then a null string is false, any other value
      -- is true
      return value \== ""
  end
  -- nodesets convert to boolean based on an empty test
  else if value~isa(.NodeSet) then return value~length > 0
  -- other than the 4 basic types is ambiguously defined by the standard,
  -- so we'll just assume the answer is false for now.
  return .false

-- convert a value to a numeric value.  Things don't really map that
-- well for Rexx values, so for strings that aren't numeric, we'll
-- just return the final option of 0.
::method convertNumber
  use strict arg value
  if value~isa(.String) then do
      -- if numeric, then return it directly
      if value~datatype('Number') then return value
      -- the string true converts to the numeric version
      if value == "true" then return .true
      -- the default "not good value" result (which also handles
      -- "false"
      return 0
  end
  else if value~isa(.nodeset) then do
      -- nodesets have a string value...convert this and then
      -- try to convert to numeric form
      return self~convertNumber(self~convertString(value))
  end
  return 0   -- unknown type...this is zero

-- convert an object value to a string value.
::method convertString
  use strict arg value

  -- already a string, then return directly.  Note that this also
  -- handles the number category.  Strictly speaking, we should also
  -- be returning "true" and "false", but this would mess up some
  -- very common numeric values.
  if value~isa(.string) then return value
  -- if this is a node set, return the string value of the first
  -- node.  If the set is empty, return a null string
  if value~isa(.nodeset) then do
      if value~length = 0 then return ""
      else return value[0]~nodeValue
  end
  -- last option...just return the string form of the object
  return value~string

-- an axis evaluation step in an expression
::class "XpathStep" subclass xpathexpr
::method init
  expose axis nodeTest predicates
  use strict arg axis, nodeTest = (.NodeTest~new(.Node~ANY_NODE)), predicates = (.array~new)

::method evaluate
  expose axis nodeTest
  use strict arg context, container

  -- use the axis name to request our initial nodeset value
  nodeSet = context~send(axis)
  -- apply any type filtering
  nodeSet = nodeTest~apply(nodeSet)
  -- now filter by predicates
  nodeSet = self~filterByPredicates(context, nodeSet)
  return nodeSet

-- apply and predicate filtering to this step
::method filterByPredicates
  expose predicates
  -- apply each predicate in turn
  loop predicate over predicates
      nodeSet = nodeSet~filter(context, predicate)
  end

  return nodeSet

-- a dyadic expression term
::class "XPathDyadicOperator" subclass XPathExpr
::method init
  expose handler left right
  -- the handler is the string name of the method to invoke
  use strict arg handler, left, right

::method evaluate
  expose handler
  -- just pass this along to the appropriate handler method using the
  -- same arguments
  forward message(handler)

-- the operator methods
::method operatorPlus
  expose left right

  return leftTerm~evaluateNumber(context) + rightTerm~evaluateNumber(context)

::method operatorMinus
  expose left right

  return leftTerm~evaluateNumber(context) - rightTerm~evaluateNumber(context)

::method operatorMultiply
  expose left right

  return leftTerm~evaluateNumber(context) * rightTerm~evaluateNumber(context)

::method operatorDivide
  expose left right

  return leftTerm~evaluateNumber(context) / rightTerm~evaluateNumber(context)

::method operatorMod
  expose left right

  return leftTerm~evaluateNumber(context) // rightTerm~evaluateNumber(context)

::method operatorAnd
  expose left right

  return leftTerm~evaluateBoolean(context) & rightTerm~evaluateBoolean(context)

::method operatorOr
  expose left right

  return leftTerm~evaluateBoolean(context) | rightTerm~evaluateBoolean(context)

-- comparison methods
::method compareEqual
  use strict arg left, right
  return left = right

::method compareNotEqual
  use strict arg left, right
  return left \= right

::method compareLessThan
  use strict arg left, right
  return left < right

::method compareLessThanEqual
  use strict arg left, right
  return left <= right

::method compareGreaterThan
  use strict arg left, right
  return left > right

::method compareGreaterThanEqual
  use strict arg left, right
  return left >= right

-- returns true if the string value of any two members of the nodesets are true
-- using the comparison method
::method compareNodeSets
  use strict arg comparison, left, right

  loop leftNode over left
      loop rightNode over right
          -- do the comparison method, and if true, return
          if self~send(comparison, leftNode~nodeValue, rightNode~nodeValue) then
             return .true
      end
  end

  -- nothing compared true, so false
  return .false

::method compareNodeSetToValue
  use strict arg comparison, left, right, converter

  loop leftNode over left
      if self~send(comparison, self~send(converter, left~nodeValue), right) then return .true
  end

  return .false

-- the equality operator.  This is complicated by the different pairing
-- types that can occur between left and right
::method operatorEqual
  expose left right
  use strict arg context, container

  leftValue = left~evaluate(context)
  rightValue = right~evaluate(context)

  -- handle booleans first, since the same comparison rules
  -- apply to NodeSet values also.  Boolean in this context
  -- is the string "true" or "false"
  if leftVal == "true" | leftValue == "false" then
      return self~convertBoolean(leftVal) == self~convertBoolean(rightVal)
  else if rightVal == "true" | rightValue == "false" then
      return self~convertBoolean(leftVal) == self~convertBoolean(rightVal)

  -- nodeset comparisons
  if leftVal~isA(.nodeset) then do
      if rightVal~isA(.nodeset) then
          return self~compareNodeSets("COMPAREEQUAL", leftVal, rightVal)
      -- doing numeric comparisons?
      if rightVal~isa(.string), rightVal~datatype('Number') then
          return self~compareNodeSetToValue("COMPAREEQUAL", leftVal, rightVal, "CONVERTNUMBER")
      -- string compare
      return self~compareNodeSetToValue("COMPAREEQUAL", leftVal, rightVal, "CONVERTSTRING")
  end
  else if rightVal~isa(.nodeset) then do
      -- doing numeric comparisons?
      if leftVal~isa(.string), leftVal~datatype('Number') then
          return self~compareNodeSetToValue("COMPAREEQUAL", rightVal, leftVal, "CONVERTNUMBER")
      -- string compare
      return self~compareNodeSetToValue("COMPAREEQUAL", rightVal, leftVal, "CONVERTSTRING")
  end
  else if leftVal~isa(.string), leftVal~datatype('Number') then
      return leftVal = self~convertNumber(rightVal)
  else if rightVal~isa(.string), rightVal~datatype('Number') then
      return self~convertNumber(leftVal) = rightVal
  else return leftVal == rightVal

-- the inequality operator.  This is complicated by the different pairing
-- types that can occur between left and right
::method operatorNotEqual
  expose left right
  use strict arg context, container

  leftValue = left~evaluate(context)
  rightValue = right~evaluate(context)

  -- handle booleans first, since the same comparison rules
  -- apply to NodeSet values also.  Boolean in this context
  -- is the string "true" or "false"
  if leftVal == "true" | leftValue == "false" then
      return self~convertBoolean(leftVal) \= self~convertBoolean(rightVal)
  else if rightVal == "true" | rightValue == "false" then
      return self~convertBoolean(leftVal) \= self~convertBoolean(rightVal)

  -- nodeset comparisons
  if leftVal~isA(.nodeset) then do
      if rightVal~isA(.nodeset) then
          return self~compareNodeSets("COMPARENOTEQUAL", leftVal, rightVal)
      -- doing numeric comparisons?
      if rightVal~isa(.string), rightVal~datatype('Number') then
          return self~compareNodeSetToValue("COMPARENOTEQUAL", leftVal, rightVal, "CONVERTNUMBER")
      -- string compare
      return self~compareNodeSetToValue("COMPARENOTEQUAL", leftVal, rightVal, "CONVERTSTRING")
  end
  else if rightVal~isa(.nodeset) then do
      -- doing numeric comparisons?
      if leftVal~isa(.string), leftVal~datatype('Number') then
          return self~compareNodeSetToValue("COMPARENOTEQUAL", rightVal, leftVal, "CONVERTNUMBER")
      -- string compare
      return self~compareNodeSetToValue("COMPARENOTEQUAL", rightVal, leftVal, "CONVERTSTRING")
  end
  else if leftVal~isa(.string), leftVal~datatype('Number') then
      return leftVal \= self~convertNumber(rightVal)
  else if rightVal~isa(.string), rightVal~datatype('Number') then
      return self~convertNumber(leftVal) \= rightVal
  else return leftVal \== rightVal

-- the less than operator.  This is complicated by the different pairing
-- types that can occur between left and right
::method operatorLessThan
  expose left right
  use strict arg context, container

  leftValue = left~evaluate(context)
  rightValue = right~evaluate(context)

  -- handle booleans first, since the same comparison rules
  -- apply to NodeSet values also.  Boolean in this context
  -- is the string "true" or "false"
  if leftVal == "true" | leftValue == "false" then
      return self~convertBoolean(leftVal) < self~convertBoolean(rightVal)
  else if rightVal == "true" | rightValue == "false" then
      return self~convertBoolean(leftVal) < self~convertBoolean(rightVal)

  -- nodeset comparisons
  if leftVal~isA(.nodeset) then do
      if rightVal~isA(.nodeset) then
          return self~compareNodeSets("COMPARELESSTHAN", leftVal, rightVal)
      -- doing numeric comparisons?
      if rightVal~isa(.string), rightVal~datatype('Number') then
          return self~compareNodeSetToValue("COMPARELESSTHAN", leftVal, rightVal, "CONVERTNUMBER")
      -- string compare
      return self~compareNodeSetToValue("COMPARELESSTHAN", leftVal, rightVal, "CONVERTSTRING")
  end
  else if rightVal~isa(.nodeset) then do
      -- NB:  Since we're reversing the order of the arguments here, the
      -- comparison function used is the inverse of the actual operation.
      -- In other words, we're using the equivalency of "left < right" and
      -- "right >= left".
      -- doing numeric comparisons?
      if leftVal~isa(.string), leftVal~datatype('Number') then
          return self~compareNodeSetToValue("COMPAREGREATERTHANEQUAL", rightVal, leftVal, "CONVERTNUMBER")
      -- string compare
      return self~compareNodeSetToValue("COMPAREGREATERTHANEQUAL", rightVal, leftVal, "CONVERTSTRING")
  end
  else if leftVal~isa(.string), leftVal~datatype('Number') then
      return leftVal < self~convertNumber(rightVal)
  else if rightVal~isa(.string), rightVal~datatype('Number') then
      return self~convertNumber(leftVal) < rightVal
  else return leftVal << rightVal

-- the less than equal operator.  This is complicated by the different pairing
-- types that can occur between left and right
::method operatorLessThanEqual
  expose left right
  use strict arg context, container

  leftValue = left~evaluate(context)
  rightValue = right~evaluate(context)

  -- handle booleans first, since the same comparison rules
  -- apply to NodeSet values also.  Boolean in this context
  -- is the string "true" or "false"
  if leftVal == "true" | leftValue == "false" then
      return self~convertBoolean(leftVal) <= self~convertBoolean(rightVal)
  else if rightVal == "true" | rightValue == "false" then
      return self~convertBoolean(leftVal) <= self~convertBoolean(rightVal)

  -- nodeset comparisons
  if leftVal~isA(.nodeset) then do
      if rightVal~isA(.nodeset) then
          return self~compareNodeSets("COMPARELESSTHANEQUAL", leftVal, rightVal)
      -- doing numeric comparisons?
      if rightVal~isa(.string), rightVal~datatype('Number') then
          return self~compareNodeSetToValue("COMPARELESSTHANEQUAL", leftVal, rightVal, "CONVERTNUMBER")
      -- string compare
      return self~compareNodeSetToValue("COMPARELESSTHANEQUAL", leftVal, rightVal, "CONVERTSTRING")
  end
  else if rightVal~isa(.nodeset) then do
      -- NB:  Since we're reversing the order of the arguments here, the
      -- comparison function used is the inverse of the actual operation.
      -- In other words, we're using the equivalency of "left < right" and
      -- "right >= left".
      -- doing numeric comparisons?
      if leftVal~isa(.string), leftVal~datatype('Number') then
          return self~compareNodeSetToValue("COMPAREGREATERTHAN", rightVal, leftVal, "CONVERTNUMBER")
      -- string compare
      return self~compareNodeSetToValue("COMPAREGREATERTHAN", rightVal, leftVal, "CONVERTSTRING")
  end
  else if leftVal~isa(.string), leftVal~datatype('Number') then
      return leftVal <= self~convertNumber(rightVal)
  else if rightVal~isa(.string), rightVal~datatype('Number') then
      return self~convertNumber(leftVal) <= rightVal
  else return leftVal <<= rightVal

-- the greater than operator.  This is complicated by the different pairing
-- types that can occur between left and right
::method operatorGreaterThan
  expose left right
  use strict arg context, container

  leftValue = left~evaluate(context)
  rightValue = right~evaluate(context)

  -- handle booleans first, since the same comparison rules
  -- apply to NodeSet values also.  Boolean in this context
  -- is the string "true" or "false"
  if leftVal == "true" | leftValue == "false" then
      return self~convertBoolean(leftVal) > self~convertBoolean(rightVal)
  else if rightVal == "true" | rightValue == "false" then
      return self~convertBoolean(leftVal) > self~convertBoolean(rightVal)

  -- nodeset comparisons
  if leftVal~isA(.nodeset) then do
      if rightVal~isA(.nodeset) then
          return self~compareNodeSets("COMPAREGREATERTHAN", leftVal, rightVal)
      -- doing numeric comparisons?
      if rightVal~isa(.string), rightVal~datatype('Number') then
          return self~compareNodeSetToValue("COMPAREGREATERTHAN", leftVal, rightVal, "CONVERTNUMBER")
      -- string compare
      return self~compareNodeSetToValue("COMPAREGREATERTHAN", leftVal, rightVal, "CONVERTSTRING")
  end
  else if rightVal~isa(.nodeset) then do
      -- NB:  Since we're reversing the order of the arguments here, the
      -- comparison function used is the inverse of the actual operation.
      -- In other words, we're using the equivalency of "left < right" and
      -- "right >= left".
      -- doing numeric comparisons?
      if leftVal~isa(.string), leftVal~datatype('Number') then
          return self~compareNodeSetToValue("COMPARELESSTHANEQUAL", rightVal, leftVal, "CONVERTNUMBER")
      -- string compare
      return self~compareNodeSetToValue("COMPARELESSTHANEQUAL", rightVal, leftVal, "CONVERTSTRING")
  end
  else if leftVal~isa(.string), leftVal~datatype('Number') then
      return leftVal > self~convertNumber(rightVal)
  else if rightVal~isa(.string), rightVal~datatype('Number') then
      return self~convertNumber(leftVal) > rightVal
  else return leftVal >> rightVal

-- the greater than equal operator.  This is complicated by the different pairing
-- types that can occur between left and right
::method operatorGreaterThanEqual
  expose left right
  use strict arg context, container

  leftValue = left~evaluate(context)
  rightValue = right~evaluate(context)

  -- handle booleans first, since the same comparison rules
  -- apply to NodeSet values also.  Boolean in this context
  -- is the string "true" or "false"
  if leftVal == "true" | leftValue == "false" then
      return self~convertBoolean(leftVal) >= self~convertBoolean(rightVal)
  else if rightVal == "true" | rightValue == "false" then
      return self~convertBoolean(leftVal) >= self~convertBoolean(rightVal)

  -- nodeset comparisons
  if leftVal~isA(.nodeset) then do
      if rightVal~isA(.nodeset) then
          return self~compareNodeSets("COMPAREGREATERTHANEQUAL", leftVal, rightVal)
      -- doing numeric comparisons?
      if rightVal~isa(.string), rightVal~datatype('Number') then
          return self~compareNodeSetToValue("COMPAREGREATERTHANEQUAL", leftVal, rightVal, "CONVERTNUMBER")
      -- string compare
      return self~compareNodeSetToValue("COMPAREGREATERTHANEQUAL", leftVal, rightVal, "CONVERTSTRING")
  end
  else if rightVal~isa(.nodeset) then do
      -- NB:  Since we're reversing the order of the arguments here, the
      -- comparison function used is the inverse of the actual operation.
      -- In other words, we're using the equivalency of "left < right" and
      -- "right >= left".
      -- doing numeric comparisons?
      if leftVal~isa(.string), leftVal~datatype('Number') then
          return self~compareNodeSetToValue("COMPARELESSTHAN", rightVal, leftVal, "CONVERTNUMBER")
      -- string compare
      return self~compareNodeSetToValue("COMPARELESSTHAN", rightVal, leftVal, "CONVERTSTRING")
  end
  else if leftVal~isa(.string), leftVal~datatype('Number') then
      return leftVal >= self~convertNumber(rightVal)
  else if rightVal~isa(.string), rightVal~datatype('Number') then
      return self~convertNumber(leftVal) >= rightVal
  else return leftVal >>= rightVal

::method union
  expose left right
  use strict arg context, container

  leftVal = left~evaluateNodeSet(context, container)
  rightVal = right~evaluateNodeSet(context, container)

  union = .nodeset~new(context~document, context)
  union~union(leftVal)
  union~union(rightVal)

  return union

-- a unary operator expression term
::class "XPathUnaryOperator" subclass XPathExpr
::method init
  expose term
  use strict arg term

::method evaluate
  expose term
  use strict arg context, container

  -- the only unary operator is negation, so
  -- this is fairly simple.  Just negate the numeric
  -- value
  return - self~evaluateNumber(term)

-- a literal value as an expression term
::class "XPathLiteral" subclass XPathExpr
::method init
  expose value
  use strict arg value

::method evaluate
  expose value
  use strict arg context, container

  -- just return this without evaluation
  return value

::class "XPathFunctionCall" subclass XPathExpr
::method init
  expose name arguments
  use strict arg name, arguments

-- evaluate the named function
::method evaluate
  expose name
  use strict arg context, container

  -- forward to the appropriate function implementation
  return self~send(name"Function", context, container)

-- check the number of arguments being passed
::method checkArgs
  expose arguments
  use strict arg count

  if count \= arguments~items then
      self~xpathError(XPath~INCORRECT_FUNCTION_ARGUMENTS_ERROR);

-- check the minimum number of arguments
::method checkMinArgs
  expose arguments
  use strict arg count

  if count > arguments~items then
      self~xpathError(XPath~INCORRECT_FUNCTION_ARGUMENTS_ERROR);

-- check the arguments fall in a given range
::method checkMinMaxArgs
  expose arguments
use strict arg min, max

  if arguments~items < min | arguments~items > max then
      self~xpathError(XPath~INCORRECT_FUNCTION_ARGUMENTS_ERROR);

-- start of the actual function implementation methods
::method notFunction
  expose arguments
  use strict arg context, container

  self~checkArgs(1)
  return \arguments[1]~evaluateBoolean(context, container)

::method lastFunction
  expose arguments
  use strict arg context, container

  self~checkArgs(1)
  return container~last

::method positionFunction
  expose arguments
  use strict arg context, container

  self~checkArgs(0)
  return container~positionOf(context)

::method localNameFunction
  expose arguments
  use strict arg context, container

  self~checkMinMaxArgs(0, 1)
  -- no node set to check on, just return the localname of the
  -- context node
  if arguments~isEmpty then return context~localName

  -- get the target nodeset
  nodeSet = arguments[0]~evaluateNodeSet(context, container)
  -- no elements, no local name
  if nodeSet~length == 0 then return ""

  -- use the first item
  return nodeSet~first~localName

::method namespaceUriFunction
  expose arguments
  use strict arg context, container

  self~checkMinMaxArgs(0, 1)
  -- no node set to check on, just return the namespaceuri of the
  -- context node
  if arguments~isEmpty then return context~namespaceURI

  -- get the target nodeset
  nodeSet = arguments[1]~evaluateNodeSet(context, container)
  -- no elements, no local name
  if nodeSet~length == 0 then return ""

  -- use the first item
  return nodeSet~first~namespaceURI

::method nameFunction
  expose arguments
  use strict arg context, container

  self~checkMinMaxArgs(0, 1)
  -- no node set to check on, just return the node name of the
  -- context node
  if arguments~isEmpty then return context~nodeName

  -- get the target nodeset
  nodeSet = arguments[1]~evaluateNodeSet(context, container)
  -- no elements, no node name
  if nodeSet~length == 0 then return ""

  -- use the first item
  return nodeSet~first~nodeName

::method stringFunction
  expose arguments
  use strict arg context, container

  self~checkMinMaxArgs(0, 1)
  -- no node set to check on, just return the string value of the
  -- context node
  if arguments~isEmpty then return context~nodeValue

  -- get the target nodeset
  return arguments[0]~evaluateString(context, container)

::method concatFunction
  expose arguments
  use strict arg context, container

  -- variable number of arguments, but at least 2 are required
  self~checkMinArgs(2)

  base = .mutablebuffer~new(arguments[1]~evaluateString(context, container))
  loop i = 2 to arguments~length
      base~append(arguments[i]~evaluateString(context, container))
  end

  return base~string

::method startsWithFunction
  expose arguments
  use strict arg context, container

  self~checkArgs(2)

  haystack = arguments[1]~evaluateString(context, container)
  needle = arguments[2]~evaluateString(context, container)

  return haystack~match(1, needle)

::method containsFunction
  expose arguments
  use strict arg context, container

  self~checkArgs(2)

  haystack = arguments[1]~evaluateString(context, container)
  needle = arguments[2]~evaluateString(context, container)

  return haystack~pos(needle) \= 0

::method substringBeforeFunction
  expose arguments
  use strict arg context, container

  self~checkArgs(2)

  haystack = arguments[1]~evaluateString(context, container)
  needle = arguments[2]~evaluateString(context, container)

  index = haystack~pos(needle)
  if index = 0 then return ""
  return haystack~substr(1, index - 1)

::method substringAfterFunction
  expose arguments
  use strict arg context, container

  self~checkArgs(2)

  haystack = arguments[1]~evaluateString(context, container)
  needle = arguments[2]~evaluateString(context, container)

  index = haystack~pos(needle)
  if index = 0 then return ""
  return haystack~substr(index + needle~length)

::method substringFunction
  expose arguments
  use strict arg context, container

  self~checkMinMaxArgs(2, 3)

  haystack = arguments[1]~evaluateString(context, container)
  start = arguments[2]~evaluateNumber(context, container)

  if arguments~length = 2 then
      return haystack~substr(start)

  length = arguments[3]~evaluateNumber(context, container)
  return haystack~substr(start, length)

::method stringLengthFunction
  expose arguments
  use strict arg context, container

  self~checkMinMaxArgs(0, 1)

  if arguments~length = 0 then return context~nodeValue~length
  return arguments[1]~evaluateString(context, container)~length

::method normalizeSpaceFunction
  expose arguments
  use strict arg context, container

  self~checkMinMaxArgs(0, 1)

  if arguments~length = 0 then return context~nodeValue~space
  return arguments[1]~evaluateString(context, container)~space

::method booleanFunction
  expose arguments
  use strict arg context, container

  self~checkArgs(1)

  return arguments[1]~evaluateBoolean(context, container)

::method trueFunction
  use strict arg context, container

  self~checkArgs(0)
  return .true

::method falseFunction
  use strict arg context, container

  self~checkArgs(0)
  return .false

::method numberFunction
  expose arguments
  use strict arg context, container

  self~checkMinMaxArgs(0, 1)

  if arguments~length = 0 then self~convertNumber(context~nodeValue)
  return arguments[1]~evaluateNumber(context, container)

::method sumFunction
  expose arguments
  use strict arg context, container

  self~checkArgs(0)

  nodeSet = arguments[1]~evaluateNodeSet(context, container)
  accum = 0

  loop node over nodeset
      accum += node~nodeValue
  end

  return accum

::method floorFunction
  expose arguments
  use strict arg context, container

  return self~floor(arguments[1]~evaluateNumber(context, container))

::method ceilingFunction
  expose arguments
  use strict arg context, container

  return self~ceiling(arguments[1]~evaluateNumber(context, container))

::method roundFunction
  expose arguments
  use strict arg context, container

  return self~round(arguments[1]~evaluateNumber(context, container))

::method floor private
  use strict arg value

  -- use the ceiling/floor equivalancy to handle negative numbers
  if value < 0 then return -self~ceiling(-value)
  return value~trunc

::method ceiling private
  use strict arg value

  -- use the ceiling/floor equivalancy to handle negative numbers
  if value < 0 then return -self~floor(-value)
  if value~datatype('Whole') then return value~trunc  -- trunc will normalize the value
  else return (value + 1)~trunc  -- will step up and truncate down

::method round private
  use strict arg value
  return self~floor(value + .5)

-- a multi-step path to a given location
::class "LocationPath"
::method init
  expose steps
  use strict arg steps

::method evaluate
  expose steps
  use strict arg context, container

  -- evaluate the first step...all the rest are evaluated in the context of
  -- the returned nodeset

  nodeSet = steps[1]~evaluate(context, container)

  loop i = 2 to steps~length
      nodeset = nodeset~selectNodeSet(step, context)
  end

  return nodeSet

-- the expression parser, which builds a location path that can be used to
-- evaluate the xpath expression
::class "XPathParser"
::method init
  expose termStack opStack document expr
  use strict arg document, expr

  -- create stacks for building up the semantic tree
  termStack = .queue~new
  opStack = .queue~new

::method parse
  expose termStack opStack document expr tokenQueue
 use strict arg

  -- scan off all of the expression tokens
  scanner = .XPathScanner~new
  tokenQueue = scanner~scanExpression(expr)
  -- parse the tokens into a location path
  return self~parseLocationPath

-- Parse the main location path
::method parseLocationPath
  expose tokenQueue

  steps = .array~new
  token = tokenQueue~nextToken


  -- if the path begins with a slash, then we need to start from
  -- the root axis
  if token == .XPathToken~slash then
      steps~append(.xpathstep~new("rootAxis"))
  -- probably the axis, so push it back on for processing.
  else tokenqueue~previousToken

  loop forever
      -- get the next step in the chain
      step = self~parseLocationStep
      steps~append(step)
      token = tokenQueue~nextToken
      -- now check the next token to decide what to do
      if token == .XPathToken~double_slash then do
          -- a double slash is a axis shorthand.  This is used
          -- by the next step parsing, so push it back
      end
      -- a slash is a true step delimiter, just keep parsing
      else if token == .XPathToken~slash then do
          iterate
      end
      -- something else, this is probably the end
      else do
          -- if we have a real token, push it back
          if token \= .nil then tokenQueue~previousToken
          -- and return the working location path
          return .XPathLocationPath~new(steps)
      end
  end

-- parse an individual step in an xpath expression
::method parseLocationStep
  expose tokenQueue

  token = tokenQueue~nextToken
  -- a period is just the self axis, we can return this quickly
  if token == .XPathToken~period then return .XPathStep~new("selfAxis")
  -- double period is the parent axis
  else if token == .XPathToken~double_period then return .XPathStep~new("parentAxis")
  -- back up...need to parse some other axis
  tokenQueue~previousToken
  -- get the axis
  axis = self~parseAxis
  -- any node test
  test = self~parseNodeTest
  -- selection predicates
  predicates = self~parsePredicates
  -- and combine this into a step
  return .XPathStep~new(axis, test, predicates)

-- parse an axis definition
::method parseAxis
  expose tokenQueue

  token = tokenQueue~nextToken
  -- the @ sign is short hand for the attribute axis
  if token == .Token~at_sign then return "attributeAxis"
  -- double slash is also short hand
  else if token == .Token~double_slash then return "descendantOrSelfAxis"
  -- ok, potentially an axis specifier (the tokenization process handles
  -- the double colons following
  if token~type == "AXIS" then do
      -- these have already been validated, so we can get the name from
      -- the token
      return token~name||"Axis"
  end
  -- no explicit axis, so we default this to the child axis.  Put the
  -- token back before we return
  tokenQueue~previousToken
  return "ChildAxis"

-- parse the nodetest portion of an xpath expression
::method parseNodeTest
  expose tokenQueue

  token = tokenQueue~nextToken
  -- a wild card, this is easy
  if token == .XPathToken~wildcard then do
      -- looking for element nodes, matching anything
      return .NodeTest~new(Node~ELEMENT_NODE, .QName~new("*", "*"))
  end
  -- some sort of node type test
  else if token~type == "NODETYPE" then do
      if token~value = "element" then return .NodeTest~new(.Node~ELEMENT_NODE)
      if token~value = "text" then return .NodeTest~new(.Node~TEXT_NODE)
      if token~value = "comment" then return .NodeTest~new(.Node~COMMENT_NODE)
      if token~value = "node" then return .NodeTest~new(.Node~ANY_NODE)
      if token~value = "processing-instruction" then return .NodeTest~new(.Node~PROCESSING_INSTRUCTION_NODE)
  end
  -- have a qualified name...all of the pieces are here already
  else if token~type == "QNAME" then do
      return .NodeTest~new(Node~ANY_NODE, .QName~new(token~prefix, token~name))
  end
  -- simple name token?
  else if token~type == "NCNAME" then
      return .NodeTest~new(Node~ANY_NODE, .QName~new("*", token~name))
  -- this token is something else...push it back and just return an ANY_NODE selector
  tokenqueue~previousToken
  return .NodeTest~new(Node~ANY_NODE);

-- parse out a predicate
::method parsePredicate
  expose tokenQueue

  token = tokenQueue~nextToken
  -- this is only a predicate if it is a bracket
  if token == .XPathToken~open_bracket then do
      return self~parseExpression(.XPathToken~close_bracket)
  end

  return .nil   -- no expression to return

-- parse an expression upto the indicated terminator token
::method parseExpression
  use strict arg terminator

  -- prepare for parsing a new expression
  self~newExpression
  -- parse off the expression
  expr = self~parseSubexpression(terminator)
  -- clean up the expression environment
  self~endExpression

  return expr

-- parse function arguments
::method parseFunctionArguments
  expose tokenQueue

  arguments = .array~new

  loop forever
      arg = self~parseSubexpression(.XPathToken~function_argument)
      if arg \= .nil then arguments~append(arg)
      else do
          -- need to figure out why we didn't get anything
          terminator = tokenQueue~nextToken
          if terminator == .XPathToken~comma then
              self~xpathError(.XPath~MISSING_ARGUMENT_ERROR)
          -- this was the real end of the arguments
          else if terminator == .XPathToken~close_paren then leave
          else self~xpathError(.XPath~MISSING_PAREN_ERROR)
      end
  end

  return arguments

-- parse out a function call
::method parseFunctionCall
  expose tokenQueue
  use strict arg functionName

  -- function calls are always followed by a paren, so skip over it
  tokenQueue~nextToken

  return .XPathFunctionCall~new(functionName, self~parseFunctionArguments)

-- parse an expression subterm. This will be an individual item, a parentheitcal
-- expression or potentially a prefix operator.
::method parseSubTerm
  expose tokenQueue
  use strict arg terminator

  token = tokenQueue~nextToken
  -- if this is an expected terminator, we're finished.
  if self~isTerminator(token, terminator) then return .nil

  select
      -- potentially a prefix operation
      when token = .XPathToken~subtraction then do
          -- get the term this applies to
          term = self~parseSubTerm(terminator)
          if term == .nil then self~xpathError(XPath~INVALID_EXPRESSION_ERROR)
          -- the term is the unary operator
          return .XPathUnaryOperator~new(token, term)
      end
      -- literals are used directly
      when token~type == "LITERAL" then return .XPathLiteral~new(token)
      -- a parenthetical expression
      when token == .XPathToken~open_paren then do
          -- parse the sub expression
          term = self~parseSubexpression(.XPathToken~close_paren)
          if term == .nil then self~xpathError(XPath~INVALID_EXPRESSION_ERROR)
          -- this term can be used directly
          return term
      end
      -- variable references are not supported.
      when token~type == "VARIABLE" then self~xpathError(XPath~INVALID_VARIABLE_REFERENCE_ERROR)
      -- go parse the function call
      when token~type == "FUNCTION" then return self~parseFunctionCall(token~name)
      -- variable references are not supported.
      when token~type == "VARIABLE" then self~xpathError(XPath~INVALID_VARIABLE_REFERENCE_ERROR)
      -- various location names as a substep term.  This needs to be parsed recursively
      when token~type == "AXIS" | token~type == "NCNAME" | -
           token == .XPathToken~at_sign | token == .XPathToken~period | -
           token == .XPathToken~double_period | token == .XPathToken~slash | -
           token == .XPathToken~double_slash then do
          -- push the token back
          tokenQueue~previousToken
          -- parse the location term
          return self~parseLocation
      end
      -- probably a dyadic operator in an invalide location.  Invalid at this spot
      otherwise self~xpathError(XPath~INVALID_EXPRESSION_ERROR)
  end

-- check for an expression terminator token
::method isTerminator
  use strict arg token, terminator

  -- end of the queue is always a terminator
  if token == .nil then return .true
  -- exact match?  Also true
  if token == terminator then return .true
  -- function argument lists have multiple terminators
  if terminator == .XPathToken~function_args, token == .XPathToken~comma | token == .XPathToken~close_paren then
      return .true
  -- not a terminator
  return .false

-- parse a subexpression, such as a function argument
::method parseSubexpression
  expose tokenQueue
  use strict arg terminator

  -- scan off the first term. If we don't get anything, we've hit
  -- a terminator token and we're done
  leftTerm = self~parseSubTerm(terminator)
  if leftTerm == .nil then return .nil
  -- make a mark on the operator stack so we know how far we can pop back.
  self~newOperatorStack

  -- now handle the following part, looping until we hit the
  -- expected termination point
  token = tokenQueue~nextToken

  loop while \self~isTerminator(token, terminator)
      -- is this a dyadic operator?  Need to handle
      -- a more complex expression, applying precedence if necessary
      if token~isOperator then do
          loop forever
              topOp = self~topOperator
              -- we use the dummy token to mark the substack on the
              -- operator stack
              if topOp == .XPathToken~dummy then leave
              -- if the current is higher precedence than the operator at the top
              -- of the stack, we process it first.
              if token~precedence > topOp~precedence then leave
              right = self~popTerm
              left = self~popTerm
              -- replace the top of the stack with the operator term
              self~pushTerm(.DyadicOperator~new(self~popOperator, left, right))
              -- current operator goes to the top of the stack
          end
          self~pushOperator(token)
          -- this must be followed by a valid subterm, so parse it off
          -- and push it on to the term stack
          right = self~parseSubTerm(terminator)
          if right == .nil then self~xpathError(XPath~INVALID_EXPRESSION_ERROR)
          self~pushTerm(right)
      end
      -- something we don't recognize
      else self~xpathError(XPath~INVALID_EXPRESSION_ERROR)
  end

-- parse out all of the predicate modifiers for an xpath expresion
::method parsePredicates

  predicates = .array~new
  loop forever
      predicate = self~parsePredicate
      if predicate == .nil then leave
      predicates~append(predicate)
  end
  -- don't return anything if nothing was there
  if predicates~isEmpty then return .nil
  return predicates

-- raise an XPATH error
::method xpathError
  use strict arg reason
  .XPath~xpathError(reason)
