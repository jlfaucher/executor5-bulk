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

::options trace c

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

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: DOMStringList                                                       */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class "DOMStringList"
-- initialize a string list from an array of strings
::method init
  expose array
  -- default is to create an empty list
  use strict arg array = (.array~new)

-- retrieve an item from the list using a 0-based index
::method item
  expose array
  use strict arg index
  if index > 0 then return array[index]
  -- defined as returning .nil for anything out of range
  return .nil

-- get the count of items in the array
::attribute length get
  expose array
  return array~items

-- returns true if this list contains the given string
::method contains
  expose array
  use strict arg target
  return array~hasItem(target)

-- some compatibility methods to make nodelist feel like a Rexx collection
::method makearray
  expose array
  return array~copy    -- return a copy of our array so this stays immutable

::method "[]"
  forward message("ITEM")
::method items
  forward message("LENGTH")

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: NameList                                                            */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class "NameList"
-- initialize an empty NameList
::method init
  expose names namespaces


-- retrieve the indexed name item from the list (0-based index)
::method getName
  expose names
  use strict arg index
  if index > 0 then return names[index]
  -- defined as returning .nil for anything out of range
  return .nil


-- retrieve the indexed name item from the list (0-based index)
::method getNamespaceURI
  expose namespaces
  use strict arg index
  if index > 0 then return namespace[index]
  -- defined as returning .nil for anything out of range
  return .nil

-- get the count of items in the list
::attribute length get
  expose names
  return names~items

-- returns true if this list contains the given string
::method contains
  expose names
  use strict arg target
  return names~hasItem(target)

-- returns true if this list contains the given string
::method containsNS
  expose names namespaces
  use strict arg namespaceURI, name

  loop i = 1 to names~items
      if names[i] == name & namespaces[i] == namespaceURI then return .true
  end
  return .false

-- internal method for adding items to this list
::method addName
  expose names namespaces
  use strict arg namespaceURI, name

  names~append(name)
  namespaces~append(namespaceURI)

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Node                                                                */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


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

-- DocumentPosition indicators
::constant DOCUMENT_POSITION_DISCONNECTED 1
::constant DOCUMENT_POSITION_PRECEDING 2
::constant DOCUMENT_POSITION_FOLLOWING 4
::constant DOCUMENT_POSITION_CONTAINS 8
::constant DOCUMENT_POSITION_IS_CONTAINED 16
::constant DOCUMENT_POSITION_IMPLEMENTATION_SPECIFIC 32

::method nodeName abstract
::method nodeValue abstract
::method "nodeValue=" abstract
::method appendNode abstract
::method cloneNode abstract
::method attributes abstract
::method childNodes abstract
::method firstChild abstract
::method lastChild abstract
::method localName abstract
::method namespaceUri abstract
::method nextSibling abstract
::method nodeType abstract
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
::method lookupPrefix abstract
::method lookupNamespaceURI abstract
::method textContent abstract
::method isDefaultNamespace

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Attr                                                                */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

-- an abstract attribute interface definition
::class "Attr" mixinclass Node public
::method name abstract
::method ownerElement abstract
::method specified abstract
::method value abstract
::method "value=" abstract
::method isId abstract

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: CharacterData                                                       */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

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

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Text                                                                */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

-- abstract text definition
::class "Text" mixinclass CharacterData public
::method splitText abstract
::method wholeText abstract
::method replaceWholeText abstract

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: CDATASection                                                        */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

-- abstract CDATA section definition
::class "CDATASection" mixinclass Text public
-- no additional methods defined on this interface

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Comment                                                             */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

-- abstract Comment definition
::class "Comment" mixinclass CharacterData public
-- no additional methods defined on this interface

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Document                                                            */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

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
::method adoptNode abstract
::method "encoding=" abstract

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: DocumentFragment                                                    */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

-- abstract documentfragment definition
::class "DocumentFragment" mixinclass Node public
-- no additional methods defined on this interface

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: DocumentType                                                        */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

-- abstract documenttype definition
::class "DocumentType" mixinclass Node public
::method entities abstract
::method internalSubset abstract
::method name abstract
::method notations abstract
::method publicID abstract
::method systemID abstract

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: DOMImplementation                                                   */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

-- abstract DOMImplementation definiton
::class "DOMImplementation" mixinclass Object public
::method createDocument abstract
::method createDocumentType abstract
::method hasFeature abstract

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Element                                                             */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

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
::method setIdAttribute abstract
::method setIdAttributeNS abstract
::method setIdAttributeNode abstract

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Entity                                                              */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

-- abstract Entity definition
::class "Entity" mixinclass Node public
::method xmlEncoding abstract
::method notationName abstract
::method systemId abstract
::method publicId abstract
::method xmlVersion abstract
::method inpugEncoding

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: EntityReference                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

-- abstract entityreference definition
::class "EntityReference" mixinclass Node public
-- no additional methods defined on this interface

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: NamedNodeMap                                                        */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

-- abstract NamedNodeMap definition
::class "NamedNodeMap" mixinclass Object public
::method length abstract
::method getNamedItem abstract
::method getNamedItemNS abstract
::method item abstract
::method removeNamedItem abstract
::method removeNamedItemNS abstract
::method setNamedItem abstract
::method setNamedItemNS abstract
::method publicID abstract
::method systemID abstract
::method internalSubset abstract

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Notation                                                            */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

-- abstract notation definition
::class "Notation" mixinclass Node public
::method publicID abstract
::method systemID abstract

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: ProcessingInstruction                                               */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

-- abstract processinginstruction definition
::class "ProcessingInstruction" mixinclass Node public
::method data abstract
::method target abstract
::method "data=" abstract

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: UserDataHandler                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

-- interface class for implementing a user data handler
::class "UserDataHandler" public mixinclass object
::constant NODE_CLONED 1
::constant NODE_IMPORTED 2
::constant NODE_DELETED 3
::constant NODE_RENAMED 4
::constant NODE_ADOPTED 5

-- called as handle(key, data, srcNode, destNode)
::method handle

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: DOMEventListener                                                    */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

-- interface class for implementing a DOM event handler
::class "DOMEventListener" public mixinclass object
-- called as handleEvent(event)
::method handleEvent

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: DOMEventTarget                                                      */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

-- interface for the DOM objects that allow attaching of event listeners
::class "DOMEventTarget" public mixinclass object
::method addEventListener abstract
::method removeEventListener abstract
::method dispatchEvent abstract

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: DocumentEvent                                                       */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

-- interface for the document objects that support event creation
::class "DocumentEvent" public mixinclass object
::method createEvent abstract

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: DocumentTraversal                                                   */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

-- interface for supporting document traversal methods
::class "DocumentTraversal" mixinclass object
::method createNodeIterator abstract
::method createTreeWalker abstract

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: ElementTraversal                                                    */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

-- interface for supporting element traversal methods
::class "ElementTraversal" mixinclass object
::method firstElementChild abstract
::method lastElementChild abstract
::method previousElementSibling abstract
::method nextElementSibling abstract
::method childElementCount abstract

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: NodeIterator                                                        */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
::class "NodeIterator" public mixinclass object

::method root abstract
::method whatToShow abstract
::method filter abstract
::method expandEntityReferences abstract

::method nextNode abstract
::method previousNode abstract
::method detach abstract


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: TreeWalker                                                          */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class "TreeWalker" public mixinclass Object
::method root abstract
::method whatToShow abstract
::method filter abstract
::method expandEntityReferences abstract
::method currentNode abstract
::method "CURRENTNODE=" abstract

::method parentNode
::method firstChild
::method lastChild
::method previousSibling abstract
::method nextSibling abstract
::method previousNode abstract
::method nextNode abstract


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

-- helper method to test a particular flag setting against
-- a mask of what to show.  Return .true if this is in the mask,
-- .false otherwise
::method isShowing class
  use strict arg whatToShow, test

  -- this is easy
  if whatToShow == .NodeFilter~SHOW_ALL then
      return .true
  -- convert both values to a binary string, then pad out to 11 digits for the
  -- mapping
  flagValues = whatToShow~d2x~x2b~right(12, 0)
  testFlags = test~d2x~x2b~right(12, 0)

  -- by bitanding these together, if there are position matches,
  -- we'll end up with 1s in the matching locations, allowing us to
  -- return .true
  return flagValues~bitand(testFlags) \= 0

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: DocumentRange                                                       */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class "DocumentRange" public mixinclass Object
::method createRange abstract

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Range                                                               */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class "Range" public mixinclass Object
::constant START_TO_START 0
::constant START_TO_END   1
::constant END_TO_END     2
::constant END_TO_START   3

::method startContainer abstract
::method startOffset abstract
::method endContainer abstract
::method endOffset abstract
::method collapsed abstract
::method commonAncestorContainer abstract

::method setStart abstract
::method setEnd abstract
::method setStartBefore abstract
::method setStartAfter abstract
::method setEndBefore abstract
::method setEndAfter abstract
::method collapse abstract
::method selectNode abstract
::method selectNodeContents abstract
::method compareBoundaryPoints abstract
::method deleteContents abstract
::method extractContents abstract
::method cloneContents abstract
::method insertNode abstract
::method surroundContents abstract
::method cloneRange abstract
::method toString abstract
::method detach abstract


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

  index = nodes~index(target)

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

  index = nodes~index(target)
  -- not found or the first one, return nothing
  if index == .nil | index = 1 then return .nil
  return nodes[index]

-- get the item followinging a given one
::method next
  expose nodes
  use strict arg target

  index = nodes~index(target)
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
  if index < currentSize then return self~nodes[index + 1]
  else do
      -- we need to continue the traversal from the last node added
      if currentSize = 0 then thisNode = rootNode
      else thisNode = self~nodes[currentSize]
      -- keep adding up to the one we're looking for

      loop while index > currentSize
          thisNode = self~nextMatchingElementAfter(thisNode)
          if thisNode \= .nil then do
              self~nodes~append(thisNode)
              currentSize += currentSize
          end
          else leave  -- no more nodes available
      end
      -- this is either the one we want or .nil
      return thisNode
  end

-- get the length of the nodelist
::attribute length GET
  expose nodes
  use strict arg
  -- this forces everything to be initialized
  self~item(999999999)
  return self~nodes~items

-- loop over the entire list
::method makearray
  expose nodes
  use strict arg
  -- this forces everything to be initialized
  self~item(999999999)
  return self~nodes~copy

-- locate the next matching element after a starting node
::method nextMatchingElementAfter private
  expose rootNode tagName nsName
  use arg current

  loop while current \= .nil
      -- go down to the first child if it has one
      if current~hasChildNodes then current = current~firstChild
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
              if next \= .nil then leave
          end
          -- this is either a good node, or .nil
          current = next
      end
     -- we have a candidate node, now make sure it matches what we're looking for
     if current \= rootNode, current \= .nil, current~nodeType == .Node~ELEMENT_NODE then do
         -- no namespace checking?  We'll take any element node if the name is
         -- "*" or it matches directly
         if nsName == .nil  then
             if tagName == "*" | current~tagName == tagName then return current
         -- namespace qualified (which might also be "*")
         else do
             -- wildcard match on the tagname, so just check the namespace name
             if tagName == "*" then do
                 -- wildcards on both, this is easy
                 if nsName == "*" then return current
                 else do
                     -- null string is a non-specific namepace request, which
                     -- matches only if the element does not have a namespace
                     if nsName == "" & current~namespaceURI == .nil then return current
                     -- requires an exact namespace match
                     else if nsName == current~namespaceURI then return current
                 end
             end
             -- non-wild card on the name
             else do
                 -- if we have a name match, then see if the namespace
                 -- name also matches
                 if current~localName == tagName then do
                     -- ok, the local name matches, the ns rules are the
                     -- same as above
                     if nsName == "*" then return current
                     else do
                         -- null string is a non-specific namepace request, which
                         -- matches only if the element does not have a namespace
                         if nsName == "" & current~namespaceURI == .nil then return current
                         -- requires an exact namespace match
                         else if nsName == current~namespaceURI then return current
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
/* Class: NamedNodeMapImpl                                                    */
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

-- find the index position for a given attribute
::method findItemPoint private
  expose attributes
  use arg item

  if attributes == .nil then return .nil

  return attributes~index(item)

-- find the index position for a given attribute name
::method findNamePoint private
  expose attributes
  use arg name

  if attributes == .nil then return .nil

  loop i = 1 to attributes~items
      attr = attributes[i]
      if attr~nodeName == name then return i
  end

  return .nil

 -- locate the index of a given namespace/localname pair
::method findNamePointNS private
  expose attributes
  use arg namespace, name

  if attributes == .nil then return .nil

  loop i = 1 to attributes~items
      attr = attributes[i]
      if namespace == .nil then do
          if attr~namespaceURI == .nil & name == attr~localName then
              return i
          if attr~localName == .nil & name == attr~nodeName then
              return i
      end
      else if attr~namespaceURI == namespace & attr~localName == name then
          return i
  end

  return .nil

-- determine the relative ordering of two contained nodes.  If a
-- precedes b, return true, otherwise return false
::method precedes
  expose attributes
  use strict arg a, b

  if attributes \= .nil then do
      loop node over attributes
          if node == a then return .true
          if node == b then return .false
      end
  end
  return .false

-- compare two NamedNodeMaps for equality.  To be equal, each named item in
-- each collection must be equal to the equivalent item in the other map.
-- order is not important
::method isEqual
  expose attributes
  use strict arg other

  -- these can't be equal if they contain a different number of items
  if self~length \= other~length then return .false

  -- these are independent of order, so we need to look up by name
  -- and compare the items.  Any mismatch is a failure
  loop attr over attributes
      otherAttr = other~getNamedItemNS(attr~namespaceURI, attr~name)
      if otherAttr == .nil then return .false
      if \attr~isEqualNode(otherAttr) then return .false
  end
  -- passed all of the tests...yay!
  return .true

::attribute ownerNode
::attribute hasDefaults
::attribute changed
::attribute readonly

-- get a named item
::method getNamedItem
  expose attributes
  use strict arg name

  if attributes == .nil then return .nil

  loop attribute over attributes
      if attribute~name == name then
          return attribute
  end
  return .nil

-- get a named item by namespace
::method getNamedItemNS
  expose attributes
  use strict arg namespace, name

  if attributes == .nil then return .nil

  loop attribute over attributes
      if attribute~localname == name & attribute~namespaceURI == namespace then
          return attribute
  end
  return .nil

-- set a named item
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
      if attributes == .nil then attributes = .queue~new
      attributes~append(node)
  end

  return previous


-- set a named item
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
          if attributes == .nil then attributes = .queue~new
          attributes~append(node)
      end
  end

  return previous


-- remove a named item
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

-- remove a named item
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

-- perform a deep copy of this map object
::method cloneMap
  expose attributes
  use strict arg owner
  newMap = .NamedNodeMap(owner)
  newMap~cloneContent(attributes)

-- initialize the content from another map
::method cloneContent
  expose attributes
  use arg source
  if source \= .nil then do
      -- we use a queue because it can be addressed by relative position
      attributes = .queue~new
      do attr over source
          attributes~append(attr~cloneNode(.true))
      end
  end

-- return the named item
::method item
  expose attributes
  use strict arg position

  if attributes == .nil then return .nil
  else return attributes[position + 1]

-- return the number of named items in the list
::method length
  expose attributes
  use strict arg
  if attributes \== .nil then return attributes~items
  else return 0

::attribute attributes PRIVATE

-- some convienence methods for Rexx collection appearances
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

-- set a named item
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
      previous~specified = .true
  end
  else do
      if attributes = .nil then do
          attributes = .queue~new
          self~attributes = attributes
      end
      attributes~append(attribute)
  end

  self~ownerNode~ownerDocument~setAttrNode(attribute, previous)
  return previous

-- add an attribute to the list.  This also has effect of adding it to the
-- owning element
::method setNamedItemNS
  use strict arg attribute
  -- replacing an attribute with itself does nothing
  if attribute~isOwned then return attribute

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
      previous~specified = .true
  end
  else do
      if attributes = .nil then do
          attributes = .queue~new
          self~attributes = attributes
      end
      attributes~append(attribute)
  end

  self~ownerNode~ownerDocument~setAttrNode(attribute, previous)
  return previous

-- remove an item from the attribute map
::method removeNamedItem
  use strict arg name

  index = self~findNamePoint(name)
  if index == .nil then return .nil

  return self~remove(self~attributes[index], index, .true)

-- remove an item from the attribute map
::method removeNamedItemNS
  use strict arg namespaceURI, name

  index = self~findNamePointNS(namespaceURI, name)
  if index == .nil then return .nil

  return self~remove(self~attributes[index], index, .true)

-- remove an item node
::method removeItem
  use strict arg attr, addDefault

  index = self~findItemPoint(attr)
  if index == .nil then return .nil

  return self~remove(attr, index, addDefault)

-- internal removal method used by the different update methods
::method remove private
  use strict arg attr, index, addDefault = .false

  ownerDocument = self~ownerNode~ownerDocument
  name = attr~nodeName
  -- if this is an id attribute, make sure the document knows
  -- this is going away
  if attr~isId then self~ownerDocument~removeIdentifier(attr~value)

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
              if newAttr~localName \== .nil then
                  newAttr~namespaceURI = attr~namespaceURI
              newAttr~ownerNode = self~ownerNode
              newAttr~isOwned = .true
              -- mark this as a default value
              newAttr~specified = .false
              attributes[index] = newAttr
              setdefault = .true
              -- if this is the id attribute, make sure the document knows
              -- about this mapping
              if attr~isId then
                  ownerDocument~putIdentifier(newAttr~nodeValue, ownerNode)
          end
      end
  end
  -- if we didn't end up setting a default, remove the node
  if \setDefault then attributes~remove(index)
  -- detach from usage
  attr~ownerNode = .nil
  attr~isOwned = .false
  attr~specified = .true
  attr~isId = .false

  -- notify the document
  ownerDocument~removedAttrNode(attr, self~ownerNode, name)
  return attr

-- clone the content of an attribue map
::method cloneContent
  use strict arg source
  srcnodes = source~attributes
  if srcnodes \= .nil then do
     self~attributes = .Queue~new
     loop node over srcnodes
         newNode = node~cloneNode(true)
         newNode~ownerNode = self~ownerNode
         newNode~isOwned = .true
         self~attributes~append(newNode)
     end
  end

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: NodeImpl                                                            */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::CLASS "NodeImpl" public inherit Node DOMEventTarget

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
  expose ownerNode nodeId userData
  use strict arg ownerNode = .nil
  nodeId = self~class~getId
  userData = .nil        -- no userdata
  self~isowned = .false  -- unowned until we are added as a child
  self~readonly = .false -- newly created nodes can be altered
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
  .DomErrors~raiseError(.DomException~NAMESPACE_ERR)
-- for node types that don't support a real localname,
-- this is the same as nodeName
::attribute localName GET
  use strict arg
  return self~nodeName

-- base URI is not supported yet
::attribute baseURI GET
  use strict arg
  return .nil

-- private attributes used for the implementation
::attribute isowned
::attribute readonly

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
  .DomErrors~raiseError(.DomException~Hierarchy_request_err)

::method replaceChild
  use strict arg newChild, oldChild
  -- not implemented in the base node
  .DomErrors~raiseError(.DomException~Hierarchy_request_err)

::method removeChild
  use strict arg oldChild
  -- not implemented in the base node
  .DomErrors~raiseError(.DomException~Not_found_err)

::method appendChild
  use strict arg newChild
  return self~insertBefore(newChild, .nil)

::method compareDocumentPosition
  use strict arg other

  -- if comparing a node against itself, then this is always equal
  if self == other then return 0

  -- this must be from the same dom type
  if other \== .nil, \other~isa(.NodeImpl) then
     .DomErrors~raiseError(.DomException~Not_supported_err)

  -- get the owners of the two nodes
  if self~nodeType == .Node~DOCUMENT_NODE then
      thisOwner = self
  else thisOwner = self~ownerDocument
  if other~nodeType == .Node~DOCUMENT_NODE then
      otherOwner = other
  else otherOwner = other~ownerDocument

  -- just punt and return a disconnected indicator if these have
  -- different owners
  if thisOwner \= otherOwner then
      return .Node~DOCUMENT_POSITION_DISCONNECTED

  -- Find the ancestor of each node, and the distance each node is from
  -- its ancestor.
  -- During this traversal, look for ancestor/descendent relationships
  -- between the 2 nodes in question.
  -- We do this now, so that we get this info correct for attribute nodes
  -- and their children.

  thisAncestor = self
  otherAncestor = other

  thisDepth = 0
  otherDepth = 0

  node = self
  loop while node \= .nil
      thisDepth += 1
      -- we're a descendant of other...this is an easy test.  Other precedes us
      -- and also contains us
      if node == other then
          return .Node~DOCUMENT_POSITION_CONTAINS + .Node~DOCUMENT_POSITION_PRECEDING

      thisAncestor = node
      node = node~parentNode
  end
  -- we've reached the top without finding other.  Now repeat for other to see
  -- if we can find ourself.
  node = other
  loop while node \= .nil
      otherDepth += 1
      -- we're a descendant of other...this is an easy test.  Other follows us
      -- and also contains us
      if node == self then
          return .Node~DOCUMENT_POSITION_CONTAINS + .Node~DOCUMENT_POSITION_FOLLOWING

      otherAncestor = node
      node = node~parentNode
  end

  thisAncestorType = thisAncestor~nodeType
  otherAncestorType = otherAncestor~nodeType
  thisNode = self
  otherNode = other

  -- there's special casing for ENTITY, NOTATION, DOCTYPE and ATTRIBUTES, so
  -- we need to pay attention to the types.  This occurs because the document
  -- type is sort of a side chain to the document.

  -- a child of a notation or entity...we need to step up a level and see
  -- if we're decended from the other ancestor.
  if thisAncestorType == .Node~NOTATION_NODE | thisAncestorType == .Node~ENTITY_NODE then do
      container = thisOwner~doctype
      if container = otherAncestor then
          return .Node~DOCUMENT_POSITION_CONTAINS + .Node~DOCUMENT_POSITION_PRECEDING
      -- now look for the reverse relationship
      if otherAncestorType == .Node~NOTATION_NODE | otherAncestorType == .Node~ENTITY_NODE then do
          -- if the nodes are of different types, the order depends on the
          -- type number
          if thisAncestorType \= otherAncestorType then do
              if thisAncestorType > otherAncestorType then
                  return .Node~DOCUMENT_POSITION_PRECEDING
              else return .Node~DOCUMENT_POSITION_FOLLOWING
          end
          else do
              -- this is either part of the doctype notations or entities,
              -- so query the appropriate list
              if thisAncestorType == .Node~NOTATION_NODE then do
                  if container~notations~precedes(otherAncestor, thisAncestor) then
                      return .Node~DOCUMENT_POSITION_PRECEDING
                  else
                      return .Node~DOCUMENT_POSITION_FOLLOWING
              end
              else do
                  if container~entities~precedes(otherAncestor, thisAncestor) then
                      return .Node~DOCUMENT_POSITION_PRECEDING
                  else
                      return .Node~DOCUMENT_POSITION_FOLLOWING
              end
          end
      end
  end
  -- owned by a document type
  else if thisAncestorType == .Node~DOCUMENT_TYPE_NODE then do
      if otherNode == thisOwner then
          return .Node~DOCUMENT_POSITION_CONTAINS + .Node~DOCUMENT_POSITION_PRECEDING
      else if thisOwner \== .nil & thisOwner == otherOwner then
          return .Node~DOCUMENT_POSITION_FOLLOWING
  end
  -- an attribute
  else if thisAncestorType == .Node~ATTRIBUTE_NODE then do
      thisNode = thisAncestor~ownerElement
      if otherAncestorType == .Node~ATTRIBUTE_NODE then do
          otherNode = otherAncestor~ownerElement
          -- if these attributes are owned by the same element,
          -- life is easy...just check the container
          if otherNode == thisNode then do
              if thisNode~attributes~precedes(other, this) then
                  return .Node~DOCUMENT_POSITION_PRECEDING
              else return .Node~DOCUMENT_POSITION_FOLLOWING
          end

          -- owned by different elements...see if one element is an ancestor of
          -- the other
          thisDepth = 0
          node = thisNode
          loop while node \= .nil
              thisDepth += 1
              -- the other node is an ancestor of the owning element, which
              -- means it precedes this one
              if node == otherNode then
                  return .Node~DOCUMENT_POSITION_CONTAINS + .Node~DOCUMENT_POSITION_PRECEDING
              thisAncestor = node
              node = node~parentNode
          end
      end
  end
  -- still unresolved...we need to repeat most of the above from the
  -- perspective of the other node now
  if otherAncestorType == .Node~NOTATION_NODE | otherAncestorType == .Node~ENTITY_NODE then do
      container = thisOwner~doctype
      if container = this then
          return .Node~DOCUMENT_POSITION_CONTAINS + .Node~DOCUMENT_POSITION_FOLLOWING
      otherNode = thisOwner
      otherAncestor = thisOwner
  end
  -- owned by a document type
  else if otherAncestorType == .Node~DOCUMENT_TYPE_NODE then do
      if thisNode == otherOwner then
          return .Node~DOCUMENT_POSITION_CONTAINS + .Node~DOCUMENT_POSITION_FOLLOWING
      else if otherOwner \== .nil & thisOwner == otherOwner then
          return .Node~DOCUMENT_POSITION_PRECEDING
  end
  -- an attribute.  We already know they are not both attributes
  else if otherAncestorType == .Node~ATTRIBUTE_NODE then do
      otherDepth = 0
      otherNode = otherAncestor~ownerElement
      node = otherNode
      loop while node \= .nil
          otherDepth += 1
          -- the other node is an descendent of the reference node's element, which
          -- means it follows this one
          if node == otherNode then
              return .Node~DOCUMENT_POSITION_CONTAINS + .Node~DOCUMENT_POSITION_FOLLOWING
          otherAncestor = node
          node = node~parentNode
      end
  end

  -- thisAncestor and otherAncestor must be the same at this point,
  -- otherwise, the original nodes are disconnected
  if thisAncestor \= otherAncestor then do
      return .Node~DOCUMENT_POSITION_DISCONNECTED
  end

  -- Go up the parent chain of the deeper node, until we find a node
  -- with the same depth as the shallower node
  if thisDepth > otherDepth then do
      loop for thisDepth - otherDepth
          thisNode = thisNode~parentNode
      end
      -- Check if the node we have reached is in fact "otherNode". This can
      -- happen in the case of attributes.  In this case, otherNode
      -- "precedes" this.
      if thisNode == otherNode then
          return .Node~DOCUMENT_POSITION_PRECEDING;
  end
  -- moving up the way
  else do
      loop for otherDepth - thisDepth
          otherNode = otherNode~parentNode
      end
      -- Check if the node we have reached is in fact "thisNode". This can
      -- happen in the case of attributes.  In this case, otherNode
      -- "follow" this.
      if thisNode == otherNode then
          return .Node~DOCUMENT_POSITION_PRECEDING;
  end
  -- We now have nodes at the same depth in the tree.  Find a common
  -- ancestor.
  thisNodeParent = thisNode~parentNode
  otherNodeParent = otherNode~parentNode
  loop while thisNodeParent \= otherNodeParent
      thisNode = thisNodeParent
      otherNode = otherNodeParent
      thisNodeParent = thisNodeParent~parentNode
      otherNodeParent = otherNodeParent~parentNode
  end

  -- At this point, thisNode and otherNode are direct children of
  -- the common ancestor.
  -- See whether thisNode or otherNode is the leftmost

  current = thisNodeParent~firstChild
  loop while current \= .nil
      if current == otherNode then
          return .Node~DOCUMENT_POSITION_PRECEDING
      else if current == thisNode then
          return .Node~DOCUMENT_POSITION_FOLLOWING
      current = current~nextSibling
  end

  -- should never get here...throw up our hands and
  -- say they are disconnected.
  return .Node~DOCUMENT_POSITION_DISCONNECTED

-- get the default text content for a node.  This is
-- the same as the nodevalue for many node types
::attribute textContent GET
  forward message("NODEVALUE")

-- default set text content operation.  For most nodes,
-- this sets the nodevalue.
::attribute textContent SET
  forward message("NODEVALUE=")

-- verify that two nodes are in fact the same node.
-- since we use direct references rather than some sort of
-- proxy, this is a very easy determination.
::method isSameNode
  use strict arg other
  return self~identityHash = other~identityHash

-- resolve the prefix that a node is using for a given namespace URI
::method lookupPrefix
  use strict arg uri

  -- this is the default behavior used for many nodes.  Other nodes
  -- will override this to provide different behaviors.

  -- see if we have an element ancestor.
  -- if we have one, ask for this from the element
  ancestor = self~getElementAncestor
  if ancestor \== .nil then return ancestor~lookupPrefix(uri)
  return .nil

-- resolve the namespace uri that a node is using for a given prefix
::method lookupNamespaceURI
  use strict arg prefix

  -- this is the default behavior used for many nodes.  Other nodes
  -- will override this to provide different behaviors.

  -- see if we have an element ancestor.
  -- if we have one, ask for this from the element
  ancestor = self~getElementAncestor
  if ancestor \== .nil then return ancestor~lookupNamespaceURI(prefix)
  return .nil

-- test if the current node is using a given namespace as the default
::method isDefaultNamespace
  use strict arg uri
  -- see if we have an element ancestor.
  -- if we have one, ask for this from the element
  ancestor = self~getElementAncestor
  if ancestor \== .nil then return ancestor~isDefaultNamespace(uri)
  return .false

-- this is the default behavior used for many nodes.  Other nodes
-- will override this to provide different behaviors.

-- find the first ancestor of this node that is an
-- element node
::method getElementAncestor
  parent = self~parentNode
  loop while parent \== .nil
      if parent~nodeType == .Node~ELEMENT_NODE then return parent
      parent = parent~parentNode
  end
  return .nil

::method isEqualNode
  use strict arg other
  -- a node is always equal to itself
  if self == other then return .true

  -- the spec defines the following properties as required to be equal
  -- in order to pass
  if self~nodetype \= other~nodetype then return .false
  if self~nodename \= other~nodename then return .false

  if self~localname \= other~localname then return .false

  if self~namespaceURI \= other~namespaceURI then return .false
  if self~prefix \= other~prefix then return .false
  if self~nodevalue \= other~nodevalue then return .false

  -- the attributes, must be equivalent.  For attributes, they
  -- must all exist on both nodes and have the same values.  Order
  -- is not important.
  thisAttr = self~attributes
  otherAttr = other~attributes

  -- if we have attributes, compare the sets
  if thisAttr \== .nil then do
      if otherAttr == .nil then return .false
      if \thisAttr~isEqual(otherAttr) then return .false
  end
  else if otherAttr \== .nil then return .false

  thisChildren = self~children
  otherChildren = other~children

  if thisChildren~length \= otherChildren~length then return .false
  -- the children must be equal at every position...order is dependent here.
  loop i = 0 for this~children
      if \thisChildren~item(i)~isEqualNode(otherChildren~item(i)) then return .false
  end

::method getFeature
  use strict arg feature, version
  return .nil

-- associate an object to a key on this node.
::method setUserData
  use strict arg key, data, handler
  return self~ownerDocument~setUserData(self, key, data, handler)

-- retrieve userdata associated with this node for a given key
::method getUserData
  use strict arg key
  return self~ownerDocument~setUserData(self, key)

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

-- methods for the DOMEventTracker interface

-- add a listener to this node
::method addEventListener
  use strict arg type, listener, useCapture

  self~ownerDocument~addEventListener(self, type, listener, useCapture)

-- remove a listener attached to this node
::method removeEventListener
  use strict arg type, listener, useCapture

  self~ownerDocument~removeEventListener(self, type, listener, useCapture)

-- dispatch an event on this node context
::method dispatchEvent
  use strict arg event

  self~ownerDocument~dispatchEvent(self, event)


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Node                                                                */
/*        Private methods                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

-- retrieve the nodes complete ancestor chain
::attribute ancestors get
  use strict arg

  -- NB:  We include the target node itself in this list...if you wish to exclude that,
  -- start the request one level up
  ancestors = .array~new
  node = self
  loop while node \= .nil
      ancestors~append(node)
      node = node~parentNode
  end

  return ancestors

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

-- default textContent behavior for most node types that
-- allow children.  This concatenates all child text nodes into a
-- single string
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

-- default text content method for nodes that allow children.  This
-- will remove ALL child nodes and replace them with a single text node
-- with the new content
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

-- resolve the prefix that a node is using for a given namespace URI
::method lookupPrefix
  use strict arg uri

  -- always returns .nil
  return .nil

-- resolve the namespace that a node is using for a given prefix
::method lookupNamespaceURI
  use strict arg uri

  -- always returns .nil
  return .nil

-- perform a default namespace test for a node
::method isDefaultNamespace
  use strict arg uri

  -- always returns .false for this type
  return .false

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: DocumentImpl                                                        */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
::class "DocumentImpl" subclass ParentNode public inherit Document DocumentTraversal DocumentEvent DocumentRange
::method init
  expose iterators ranges eventListeners doctype haveMutationEventListeners savedEventContext
  use arg doctype = .nil

  self~init:super
  iterators = .nil
  ranges = .nil
  eventListeners = .nil
  haveMutationEventListeners = .false
  savedEventContext = .nil

-- clone a document node
::method cloneNode
  use strict arg deep = .false

  -- create an instance of the same class (we might be subclassed)
  -- and go through the cloning logic
  newDoc = self~class~new
  self~cloneDocument(newDoc, deep)
  return newDoc

-- retrieve the DOM implementation this is associated with.  This
-- always returns the ooRexxDOM
::method implementation
  -- this is a singleton
  return .ooRexxDOM~implementation

-- create a NodeIterator instance for the DocumentTraversal interface
::method createNodeIterator
  expose iterators
  use strict arg root, whatToShow, filter, entityReferenceExpansion = .true

  iterator = .NodeIteratorImpl~new(self, root, whatToShow, filter, entityReferenceExpansion)

  -- we keep track of the iterators so we can dispatch update events to them
  if iterators = .nil  then iterators = .list~new

  iterators~append(iterator)
  return iterator

-- create a tree walker for moving through the nodes
::method createTreeWalker
  use strict arg root, whatToShow, filter, entityReferenceExpansion = .true
  return .TreeWalkerImpl~new(root, whatToShow, filter, entityReferenceExpansion)

-- remove an iterator from tracking as a result of a detach call
::method removeNodeIterator
  expose iterators
  use strict arg nodeIterator

  if nodeIterator == .nil | iterators == .nil then return

  iterators~removeItem(iterator)


-- create a range instance associated with this document
::method createRange
  expose ranges
  use strict arg

  -- if this is the first range, then create a list to track these
  if ranges == .nil then ranges = .list~new

  -- create an instance and add it to the tracking list
  range = .RangeImpl~new(self)
  ranges~append(range)
  return range

-- remove a range from the document
::method removeRange
  expose ranges
  use strict arg range

  if range == .nil | ranges == .nil then return
  ranges~removeItem(range)

-- handle a replacedText event.  This will broadcast to all of the ranges
::method replacedText
  expose ranges
  use strict arg node

  if ranges \= .nil then do
      loop range over ranges
          range~receiveReplacedText(node)
      end
  end


-- handle a deletedText event.  This will broadcast to all of the ranges
::method deletedText
  expose ranges
  use strict arg node, offset, count

  if ranges \= .nil then do
      loop range over ranges
          range~receiveDeletedText(node, offset, count)
      end
  end

-- handle a insertedText event.  This will broadcast to all of the ranges
::method insertedText
  expose ranges
  use strict arg node, offset, count

  if ranges \= .nil then do
      loop range over ranges
          range~receiveInsertedText(node, offset, count)
      end
  end

::method splitData
  expose ranges
  use strict arg node, newNode, offset

  if ranges \= .nil then do
      loop range over ranges
          range~receiveSplitData(node, newNode, offset)
      end
  end

-- create an event object.  Part of the DocumentEvent support
::method createEvent
  use strict arg type
  if type~caselessEquals("Event") then return .DOMEvent~new
  else if type~caselessEquals("MutationEvent") then return .DOMMutationEvent~new
  else .DomErrors~raiseError(.DomException~NOT_SUPPORTED_ERR)

-- flag to indicate if there are any mutation event listeners in use.
-- provides an optimization for the normal case of no listeners.
::attribute haveMutationEventListeners

-- set a set of event listeners for a given node
::method setEventListeners
  expose eventListeners
  use strict arg node, listeners

  -- if this is the first listener request, create the
  -- listener table
  if eventListeners == .nil then do
      eventListeners = .identitytable~new
  end

  -- no listeners provided?  Just remove the listeners for this node
  if listeners == .nil then do
      eventListeners~remove(node)
      -- if there are no more listeners, turn off even checking
      if eventListeners~isEmpty then
          self~haveMutationEventListeners = .false
  end
  else do
      -- add this to the listener table and make sure the
      -- event flag is turned on
      eventListeners[node] = listeners
      self~haveMutationEventListeners = .true
  end

-- retrieve the event listeners for a target node
::method getEventListeners
  expose eventListeners
  use strict arg node

  if eventListeners == .nil then return .nil
  return eventListeners[node]

-- EventTarget support

-- add an event listener for a node
::method addEventListener
  use strict arg node, eventName, listener, useCapture

  -- we need a good type and listener for this to work
  if type == .nil | type == "" | listener == .nil then return
  -- remove any existing event listener for this node and type
  self~removeEventListener(node, eventName, listener, useCapture)
  -- get the listener list for this node, creating one if needed
  nodeListeners = self~getEventListeners(node)

  if nodeListeners == .nil then do
      nodeListeners = .list~new
      self~setEventListeners(node, nodeListeners)
  end

  nodeListeners~append(.EventListenerDescriptor~new(eventName, listener, useCapture))
  -- look up the capture record for this eventName and record the fact
  capture = self~lookupCapture(eventName)
  capture~addListener(useCapture)

-- remove an event listener from the tables
::method removeEventListener
  use strict arg node, type, listener, useCapture

  if type == .nil | type == "" | listener == .nil then return

  nodeListeners = self~getEventListeners(node)
  if nodeListeners == .nil then return

  do le over nodeListeners
      if le~equals(type, listener, useCapture) then do
          nodeListeners~removeItem(le)
          if nodeListeners~isEmpty then
              self~setEventListeners(node, .nil)
          -- remove this from the capture tables as well
          capture = self~lookupCapture(eventName)
          capture~removeListener(useCapture)
          return
      end
  end

-- copy event listeners to another node
::method copyEventListeners
  use strict arg sourceNode, targetNode

  nodeListeners = self~getEventListeners(sourceNode)
  if nodeListeners == .nil then return
  -- just copy the event listener table over to the new
  -- node
  self~setEventListeners(targetNode, nodeListeners~copy)

-- resolve the prefix that a node is using for a given namespace URI
::method lookupPrefix
  use strict arg uri

  -- ask the root element for this (if we have one)
  root = self~documentElement
  if root \= .nil then return root~lookupPrefix(uri)
  return .nil

-- resolve the namespace that a node is using for a given prefix
::method lookupNamespaceURI
  use strict arg prefix

  -- ask the root element for this (if we have one)
  root = self~documentElement
  if root \= .nil then return root~lookupNamespaceURI(prefix)
  return .nil

-- test if the current node is using a given namespace as the default
::method isDefaultNamespace
  use strict arg uri

  -- ask the root element for this (if we have one)
  root = self~documentElement
  if root \= .nil then return root~isDefaultNamespace(uri)
  return .false


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
 */

::method dispatchEvent private
  use strict arg node, event

  -- check the capture descriptors.  If there are no active listeners
  -- for this event type, then we don't dispatch this
  capture = self~lookupCapture(event~type)
  if capture~total == 0 then return event~defaultPrevented

  -- initialize the events dispatch status
  event~target = node
  event~propagationStopped = .false
  event~defaultPrevented = .false

  -- capture the event parentage chain
  parents = .array~new
  previous = node
  parent = previous~parentNode
  loop while parent \== .nil
      parents~append(parent)
      previous = parent
      parent = parent~parentNode
  end

  -- capturing phase
  if capture~captures > 0 then do
      event~eventPhase = .DOMEvent~CAPTURING_PHASE
      -- we need to send this up the parent chain.  At any
      -- point, a listener may turn on the propagation flag,
      -- so we stop doing this at that poing
      loop parentNode over parents
          if event~propagationStopped then leave

          event~currentTarget = parentNode
          -- for each ancestor node, dispatch a call if there
          -- is a listener defined for this event type on the node
          listeners = self~getEventListeners(parentNode)
          if nodeListeners \== .nil then do
              -- NB:  Do over takes a snapshot copy, so
              -- this is thread safe
              loop listener over nodeListeners
                  if listener~matches(event~type, .true) then do
                      listener~invoke(event)
                      -- if the immediate stop has been turned on, we're done dispatching
                      if event~stoppedImmediatePropagation then return event~defaultPrevented
                  end
              end
          end
      end
  end

  -- both AT_TARGET and BUBBLE use non-capturing listeners
  if capture~bubbles > 0 then do
      event~eventPhase = .DOMEvent~AT_TARGET
      event~currentTarget = node
      nodeListeners = self~getEventListeners(node)
      if \event~propagationStopped & nodeListeners \= .nil then do
          -- NB:  Do over takes a snapshot copy, so
          -- this is thread safe
          loop listener over nodeListeners
              if listener~matches(event~type, .false) then do
                  listener~invoke(event)
                  -- if the immediate stop has been turned on, we're done dispatching
                  if event~stoppedImmediatePropagation then return event~defaultPrevented
              end
          end
      end

      -- BUBBLING_PHASE: Ancestors are scanned, target to root, for
      -- non-capturing listeners. If the event's preventBubbling flag
      -- has been set before processing of a node commences, we
      -- instead immediately advance to the default phase.
      -- Note that not all events bubble.
      if event~bubbles then do
          event~eventPhase = .DOMEvent~BUBBLING_PHASE
          do parent over parents
              if event~propagationStopped then leave

              event~currentTarget = parent
              nodeListeners = self~getEventListeners(parent)
              if nodeListeners \= .nil then do
                  do listener over nodeListener
                      if listener~matches(event~type, .false) then do
                          listener~invoke(event)
                          -- if the immediate stop has been turned on, we're done dispatching
                          if event~stoppedImmediatePropagation then return event~defaultPrevented
                      end
                  end
              end
          end
      end
  end

  return event~defaultPevented

-- look up the capture information for a given event type
::method lookupCapture private
  expose captures
  use strict arg type
  if captures == .nil then captures = .directory~new

  capture = captures[type]
  -- create a tracker if this is the first request for this type
  if capture == .nil then do
      capture = .EventTracker~new
      captures[type] = capture
  end

  return capture

-- dispatch an event to an entire subtree
::method dispatchEventToSubtree private
  use strict arg node, event

  -- if this is an element type, we need to dispatch
  -- the attribute nodes too
  if node~nodeType == .Node~ELEMENT_NODE then do
      loop attr over node~attributes
          self~dispatchingEventToSubtree(attr, event)
      end
  end
  self~dispatchingEventToSubtree(node~firstChild, event)

-- handle dispatch of an event to a subtree.  This will
-- recursively hit the entire subtree
::method dispatchingEventToSubtree
  use strict arg node, event

  if node == .nil then return

  -- dispatch the event to this node
  node~dispatchEvent(event)
  -- if this is an element, dispatch on the attributes as well
  if node~nodeType == .Node~ELEMENT_NODE then do
      loop attr over node~attributes
          self~dispatchingEventToSubtree(attr, event)
      end
  end

  -- hit the first child, which will also propagate to the siblings
  self~dispatchingEventToSubtree(node~firstChild, event)
  -- send down the sibling chain as well
  self~dispatchingEventToSubtree(node~nextSibling, event)

-- dispatch an aggregate event
::method dispatchAggregateEvents
  use strict arg node, enclosingAttr = .nil, oldvalue = .nil, change = 0

  owner = .nil
  if enclosingAttr \== .nil then do
      capture = self~lookupCapture(.DOMMutationEvent~DOM_ATTR_MODIFIED)
      attr = enclosingAttr[1]
      if capture~total > 0 then do
          owner = attr~ownerElement
          if owner \== .nil then do
              me = .DOMMutationEvent~new
              me.initMutationEvent(.DOMMutationEvent~DOM_ATTR_MODIFIED, .true. -
                  .false, attr, enclosingAttr[2], attr~nodeValue, -
                  attr~nodeName, change)
              owner~dispatchEvent(me)
          end
      end
  end

  capture = self~lookupCapture(.DOMMutationEvent~DOM_SUBTREE_MODIFIED)
  if capture~total > 0 then do
      me = .DOMMutationEvent~new
      me.initMutationEvent(.DOMMutationEvent~DOM_SUBTREE_MODIFIED, .true. -
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

-- For some modification events, there is an event that says "we're modifying",
-- Followed by an event that says "We modified".  These are dispatched at the same
-- time following the actual modification.  However, the first event requires some of
-- the pre-event state.  This caches the pre-event state needed for the first event
-- so that it can be retrieved to dispatch the event.  The information is just
-- cached in a simple array item.
::method saveEventContext
  expose savedEventContext
  use strict arg node

  -- clear any previous state...we might not actually be saving anything here
  savedEventContext = .nil

  -- First make sure we've got listeners for this type of event at all
  capture = self~captureLookup(.DOMMutationEvent~DOM_ATTR_MODIFIED)
  if capture~total > 0 then do
      eventAncestor = node
      -- search for a target event ancestor.  We're looking for an
      -- attribute node, but since the source event might be one of the
      -- children of the attribute, we need to find the dispatch target.
      loop forever
          -- no attribute ancestor, done
          if eventAncestor == .nil then return
          type = eventAncestor~nodeType
          -- found an attribute in the ancestry, this will be our target.  Save
          -- the current attribute node value (which may be a composite) to
          -- broadcast in the event.
          if type == .Node~ATTRIBUTE_NODE then do
              savedEventContext = .array~of(eventAncestor, eventAncestor~nodeValue)
              return
          end
          -- an entity reference...move up the chain
          else if type == .Node~ENTITY_REFERENCE_NODE then eventAncestor = eventAncestor~parentNode
          -- text nodes also go up
          else if type == .Node~TEXT_NODE then eventAncestor = eventAncestor~parentNode
          -- starting point was not an ancestor of an attribute...done
          else return
      end
  end

-- We're modifying character data on a node
-- this does not dispatch an event now...this will be done as part of an
-- aggregate later
::method modifyingCharacterData
  expose haveMutationEventListeners
  use strict arg node, replace

  if haveMutationEventListeners then do
      -- if we're not replacing, check if this belongs to an attribute
      -- and save that context for event dispatch later
      if \replace then self~saveEventContext(node)
  end

-- character data has been modified.
::method modifiedCharacterData
  expose haveMutationEventListeners
  use strict arg node, oldValue, value, replace

  -- if we have listeners, dispatch the event
  if haveMutationEventListeners then
      self~mutationEventsModifiedCharacterData(node, oldValue, value, replace)

-- this dispatches multiple events for modified character data,
-- using potential cached state
::method mutationEventsModifiedCharacterData
  expose savedEventContext
  use strict arg node, oldValue, value, replace

  if \replace then do
      capture = self~lookupCapture(.DOMMutationEvent.DOM_CHARACTER_DATA_MODIFIED)
      if capture~total > 0 then do
          me = .DOMMutationEvent~new
          me~initMutationEvent(.DOMMutationEvent~DOM_CHARACTER_DATA_MODIFIED, -
              .true, .false, .nil, oldValue, value, .nil, 0)
          self~dispatchEvent(node, me)
      end
      -- now dispatch the aggregate event on the enclosing attribute
      self~dispatchAggregateEvents(node, savedEventContext[1], oldValue)
  end

-- character data replacement...handle as a modification
::method replacedCharacterData
  use strict arg node, oldvalue, value
  self~modifiedCharacterData(node, oldvalue, value, .false)

-- inserting a new node
::method insertingNode
  expose haveMutationEventListeners
  use strict arg node, replace

  -- no event dispatched at this time, but save the
  -- context for later
  if haveMutationEventListeners & \replace then self~saveEventContext(node)

-- inserting a node
::method insertedNode
  expose haveMutationEventListeners ranges
  use strict arg node, newInternal, replace

  -- if we have listeners, dispatch the event
  if haveMutationEventListeners then
      self~mutationEventsInsertedNode(node, newInternal, replace)

  -- handle the range-related updates
  if ranges \= .nil then
      self~notifyRangesInsertedNode(newInternal)

-- process a node insertion event
::method mutationEventsInsertedNode private
  expose savedEventContext
  use strict arg node, newInternal, replace

  capture = self~lookupCapture(.DOMMutationEvent~DOM_NODE_INSERTED)
  if capture~total > 0 then do
      me = .DOMMutationEvent~new
      me~initMutationEvent(.DOMMutationEvent~DOM_NODE_INSERTED, .true. .false, -
          node, .nil, .nil, .nil, 0)
      self~dispatchEvent(newInternal, me)
  end

  -- now see if anybody is watching at the document level
  capture = self~lookupCapture(.DOMMutationEvent~DOM_NODE_INSERTED_INTO_DOCUMENT)
  if capture~total > 0 then do
      eventAncestor = node
      -- if we have an active saved context, then the ancestor is the
      -- element that owns the attribute of the saved context.
      if savedEventContext \== .nil then
          eventAncestor = savedEventContext[1]~ownerElement
      if eventAncestor \== .nil then do
          parent = eventAncestor
          loop while parent \== .nil
              -- save the last valid parent
              eventAncestor = parent
              -- if we've walked back up to an attribute, the parent
              -- is the owning element
              if parent~nodeType == .Node~ATTRIBUTE_NODE then
                  parent = parent~ownerElement
              -- get the parent
              else parent = parent~parentNode
          end
          -- did we eventually arrive at a document node?  dispatch this
          if eventAncestor~nodeType == .Node~DOCUMENT_NODE then do
              me = .DOMMutationEvent~new
              me~initMutationEvent(.DOMMutationEvent~DOM_NODE_INSERTED_INTO_DOCUMENT, -
                  .false, .false, .nil, .nil, .nil, .nil, 0)
              self~dispatchEventToSubtree(newInternal, me)
          end
      end
  end

  -- also transmit DOMAttrModified and DOMSubtreeModified
  -- (Common to most kinds of mutation)
  if \replace then
      self~dispatchAggregateEvents(node, savedEventContext[1], savedEventContext[2])

-- notify any ranges of a node insertion
::method notifyRangesInsertNode
  expose ranges
  use strict arg newInternal

  do range over ranges
      range~insertedNodeFromDOM(newInternal)
  end

-- handle the pre-removal event processing.  This may involve
-- notifying iterators and ranges of the event, allowing them
-- to update their internal state before things change
::method removingNode
  expose iterators ranges haveMutationEventListeners
  use strict arg node, oldChild, replace

  -- have iterators?  broadcast this to them
  if iterators \= .nil then
      self~notifyIteratorsRemovingNode(oldChild)

  -- ditto for the ranges
  if ranges \= .nil then
      self~notifyRangesRemovingNode(oldChild)

  -- if there are mutation event listeners, let them know too
  if haveMutationEventListeners then
      self~mutationEventsRemovingNode(node, oldChild, replace)

-- notify any attached iterators that we're moving a node
::method notityIteratorsRemovingNode
  expose iterators
  use strict arg oldChild

  -- just tap each one on the shoulder
  do iterator over iterators
      iterator~removeNode(oldChild)
  end

-- notify the rances of a removal
::method notifyRangesRemovingNode
  expose ranges
  use strict arg oldChild

  do range over ranges
      range~removeNode(oldChild)
  end

-- handle a node removal mutation event
::method mutationEventsRemovingNode
  expose savedEventContext
  use strict arg node, oldChild, replace

  -- if not a replacement operation, save the
  -- context for post-update event processing
  if \replace then
      self~saveEventContext(node)

  -- do we have capture listeners for this event type?
  capture = self~lookupCapture(.DOMMutationEvent~DOM_NODE_REMOVED)
  if capture~total > 0 then do
      me = .DOMMutationEvent~new
      me~initMutationEvent(.DOMMutationEvent~DOM_NODE_REMOVED, .true, .false,
          node, .nil, .nil, .nil, 0)
      self~dispatchEvent(oldChild, me)
  end

  -- If within Document, the child's subtree is informed that it's
  -- being removed
  capture~captureLookup(.DOMMutationEvent~DOM_NODE_REMOVED_FROM_DOCUMENT)
  if capture~total > 0 then do
      eventAncestor = self
      if savedEventContext \== .nil then
          eventAncestor = savedEventContext[1]~ownerElement
      -- find the last non-null ancestor
      if eventAncestor \= .nil then do
          parent = eventAncestor~parentNode
          do while parent \= .nil
              eventAncestor = parent
              parent = parent~parentNode
          end
      end
      -- this must stil be part of a document for this to go out
      if eventAncestor~nodeType == .Node~DOCUMENT_NODE then do
          me = new .DOMMutationEvent~new
          me~initMutationEvent(.DOMMutationEvent~DOM_NODE_REMOVED_FROM_DOCUMENT,
              .false, .false, .nil, .nil, .nil, .nil, 0)
          -- and dispatch to the removed subtree
          self~dispatchEventToSubtree(oldChild, me)
      end
  end

-- handle node removal notification.  This is the post-update
-- event broadcast
::method removedNode
  expose haveMutationEventListeners savedEventContext
  use strict arg node, replace

  if haveMutationEventListeners then do
      if \replace then do
          self~dispatchAggregateEvents(node, savedEventContext~node, savedEventContext~oldValue)
      end
  end

-- broadcast intention of node replacement to the appropriate listeners
::method replacingNode
  expose haveMutationEventListeners
  use strict arg node

  -- if we have events to broadcast later, save the context of the
  -- replacement
  if haveMutationEventListeners then
      self~saveEventContext(node)

-- broadcast an impending data replacement
::method replacingData
  expose haveMutationEventListeners
  use strict arg node

  -- if we have events to broadcast later, save the context of the
  -- replacement
  if haveMutationEventListeners then
      self~saveEventContext(node)

-- broadcast a node replacement
::method replacedNode
  expose haveMutationEventListeners savedEventContext
  use strict arg node

  -- we've replaced a node...broadcast the update to the node listener and listeners on
  -- the effected subtree
  if haveMutationEventListeners then
      self~dispatchAggregateEvents(node, savedEventContext[1], savedEventContext[2])

-- handle attribute value notification
::method modifiedAttrValue
  expose haveMutationEventListeners
  use strict arg attr, oldValue

  -- broadcast to all effected nodes
  if haveMutationEventListeners then
      self~dispatchAggregateEvents(attr, attr, oldvalue, .DOMMutationEvent~MODIFICATION)

-- handle an attribute set event
::method setAttrNode
  expose haveMutationEventListeners
  use strict arg attr, previous

  if haveMutationEventListeners then do
      -- new node event
      if previous == .nil then
          self~dispatchAggregateEvents(attr~ownerNode, attr, .nil, .DOMMutationEvent~ADDITION)
      -- replacing an existing node, this is a modification
      else self~dispatchAggregateEvents(attr~ownerNode, attr, previous~nodeValue, .DOMMutationEvent~MODIFICATION)
  end

-- handle notification of attribute node removal
::method removedAttrNode
  expose haveMutationEventListeners
  use strict arg attr, oldOwner, name

  if haveMutationEventListeners then
      self~mutationEventRemovedAttrNode(attr, oldOwner, name)

-- real processing of attribute node removal
::method mutationEventRemovedAttrNode
  use strict arg attr, oldAttr, name

  capture = self~lookupCapture(.DOMMutationEvent~DOM_ATTR_MODIFIED)
  if capture~total > 0 then do
      me = .DOMMutationEvent~new
      me~initMutationEvent(.DOMMutationEvent~DOM_ATTR_MODIFIED, .true, .false, attr,
          attr~nodeValue, .nil, name, .DOMMutationEvent~MODIFICATION)
      self~dispatchEvent(oldAttr, me)
  end

  -- we might need to dispatch events to the subtree as well
  self~dispatchAggregateEvents(oldOwner)

-- handle notification of attribute node rename
::method renamedAttrNode
  use strict arg attr, oldName

  if haveMutationEventListeners then
      self~mutationEventRenamedAttrNode(newAttr, attr, oldName)

-- real processing of attribute node rename
::method mutationEventRenameAttrNode
  use strict arg attr, oldName

  capture = self~lookupCapture(.DOMMutationEvent~DOM_ATTR_MODIFIED)
  if capture~total > 0 then do
      me = .DOMMutationEvent~new
      me~initMutationEvent(.DOMMutationEvent~DOM_ATTR_MODIFIED, .true, .false, attr,
          attr~nodeName, .nil, oldName, .DOMMutationEvent~MODIFICATION)
      self~dispatchEvent(attr, me)
  end

-- handle notification of element renames
::method renamedElement
  -- old node and new node should be the same
  use strict arg element, oldName

  if haveMutationEventListeners then
      self~mutationEventRenamedElementNode(element, oldName)

-- real processing of element node rename
::method mutationEventRenameElementNode
  use strict arg element, oldName

  capture = self~lookupCapture(.DOMMutationEvent~DOM_ELEMENT_MODIFIED)
  if capture~total > 0 then do
      me = .DOMMutationEvent~new
      me~initMutationEvent(.DOMMutationEvent~DOM_ELEMENT_MODIFIED, .true, .false, element,
         element~nodeName, .nil, oldName, .DOMMutationEvent~MODIFICATION)
      self~dispatchEvent(element, me)
  end

-- internal class for tracking event listeners
::class "EventListenerDescriptor"
::method init
  expose type useCapture listener
  use strict arg type, listener, useCapture

-- test if an event listener matches a given search criteria
::method equals
  expose type useCapture listener
  use strict arg eventType, eventlistener, phase

  return phase == useCapture & listener == eventListener & eventType == type

-- tests if a handler should handle a given event phase
::method handlesEvent
  expose type useCapture

  use strict arg eventType, phase

  return phase == useCapture & type == eventType

-- invoke an event handler with syntax error handling
::method invoke
  expose listener
  use strict arg event

  signal on syntax
  listener~handleEvent(event)
  return

syntax:
  return   -- all errors are just ignored


-- internal class for tracking named event trackers
::class "EventTracker"
::method init
  expose captures total bubble total

-- the number of capture phase listeners we have for this event
::attribute captures GET
-- the total number of listeners we have for this event type
::attribute total    GET
-- the total number of bubble phase listeners we have for this event type
::attribute bubbles  GET

-- add another listener for a given event type
::method addListener
  expose captures total bubbles
  use arg useCapture

  total += 1

  if useCapture then captures += 1
  else bubbles += 1

-- remove a listener from the tracking for this event
::method removeListener
  expose captures total bubbles
  use arg useCapture

  total -= 1

  if useCapture then captures -= 1
  else bubbles -= 1

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: AttrImpl                                                            */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class "AttrImpl" public subclass ParentNode inherit Attr
::method init
  expose nodeName textNode namespaceURI localName type isId prefix
  type = .nil
  namespaceURI = .nil
  localName = .nil
  prefix = .nil
  isId = .false
  use strict arg ownerDoc, nodeName, namespaceURI = .nil, explicitLocalName = .nil
  self~init:super(ownerDoc)
  self~setName(nodeName)
  if explicitLocalName \= .nil then
      localName = explicitLocalName
  self~specified = .false

-- process a name change for this attribute
::method setName private
  expose namespaceURI localName prefix
  use arg qname
  -- null string is the same as not there
  if namspaceURI == "" then
      namespaceURI = .nil

  colon1 = qname~pos(":")
  colon2 = qname~lastPos(":")
  -- no prefix
  if colon1 == 0 then
      -- local name and qualified name are the same
      localName = qname
  else parse var qname prefix =(colon1) =(colon2 + 1) localName

-- support for the Document renameNode method
::method rename
  use strict arg uri, name = .nil

  if name == .nil then
      self~nodeName = uri
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
  if index > 0 then
      return nodeName~substr(1, index - 1)
  else return .nil

::attribute prefix SET
  expose nodeName localName
  use strict arg prefix

  -- we're either adding or replacing the prefix
  if prefix \= "" then
      nodeName = prefix":"localName
  -- or removing it entirely
  else nodeName = localName

-- the localname part of the qualified name
::attribute localName GET

-- resolve the prefix that a node is using for a given namespace URI
::method lookupPrefix
  use strict arg uri

  -- if this node is owned and the owner is an attribute, then
  -- pass along the lookup
  ownerNode = self~ownerNode
  if ownerNode \== .nil, ownerNode~nodeType == .Node~ELEMENT_NODE then
      return ownerNode~lookupPrefix(uri)

  return .nil

-- resolve the namespace that a node is using for a given prefix
::method lookupNamespaceURI
  use strict arg prefix

  -- if this node is owned and the owner is an attribute, then
  -- pass along the lookup
  ownerNode = self~ownerNode
  if ownerNode \== .nil, ownerNode~nodeType == .Node~ELEMENT_NODE then
      return ownerNode~lookupNamespaceURI(prefix)

  return .nil

-- test if the current node is using a given namespace as the default
::method isDefaultNamespace
  use strict arg uri

  -- if this node is owned and the owner is an attribute, then
  -- pass along the lookup
  ownerNode = self~ownerNode
  if ownerNode \== .nil, ownerNode~nodeType == .Node~ELEMENT_NODE then
      return ownerNode~isDefaultNamespace(uri)
  return .false

-- indicates that this attribute has been marked as an id attribute
::attribute isId

-- clone an attribute node
::method cloneNode
  expose value
  use strict arg deep = .false

  -- do the node cloning first
  newNode = self~cloneNode:super(deep)
  -- flip the specified flag
  newNode~specified = .true
  return newNode

-- override to return the attribute node type
::attribute nodeType GET
  use strict arg
  return .Node~ATTRIBUTE_NODE

-- node name attribute
::attribute nodeName

-- remap nodevalue setter/getter to the value method
::attribute nodeValue SET
  forward message("VALUE=")
::attribute nodeValue GET
  forward message("VALUE")

-- get the type name...this is the type information
-- from a schema, which is not yet implemented
::attribute typeName GET
  expose type
  use strict arg
  if type \= .nil then do
      if type~isA(.String) then return type
      else return type~typeName
  end
  else return .nil

-- return the type definition namespace.  Not yet really
-- implemented since the parser doesn't have type support.
::attribute typeNamespace GET
  expose type
  use strict arg
  if type \= .nil then do
      if \type~isA(.String) then return type~namespace
      else return "http://www.w3.org/TR/REC-xml";
  end
  return .nil

-- determine if an attribute is derived from a given type
::method isDerivedFrom
  expose type
  use strict arg typeNamespace, typeName, derivationMethod
  if type \= .nil, \type~isA(.String) then
      return type~isDOMDerivedFrom(typeNamespace, typeName, derivationMethod)
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

-- set the attribute value
::attribute value SET
  use strict arg newValue
  -- we might need to send mutation events, so get the old value
  if self~ownerDocument~haveMutationEventListeners then
      -- get the old value (normalized)
      oldValue = self~textcontent:super

  -- this gets handled by normal text content removal/addition
  forward message("TEXTCONTENT=") class(super) continue

  self~specified = .true
  if self~ownerDocument~haveMutationEventListeners then
      self~ownerDocument~modifiedAttrValue(self, oldvalue)
  else self~changed

  if self~isId & self~ownerElement \== .nil then
      self~ownerDocument~putIdentifier(newvalue, ownerElement)


-- set the value of the attribute.  This adds a text node to the attribute
-- node
::attribute value GET
  use strict arg
  -- this is the same as text content with the addition of the
  -- buildTextContent override
  forward message("TEXTCONTENT") class(super)

-- override to the Node buildTextContent method to include
-- entity reference nodes in the mix
::method buildTextContent private
  use arg buffer
  child = self~firstChild
  loop while child \= .nil
      if child~nodeType == .Node~ENTITY_REFERENCE_NODE then
          data = child~entryRefValue
      else if self~hasTextContent(child) then
          data = child~nodeValue
      if data \== .nil then
          buffer~append(data)
      child = child~nextSibling
  end

-- overrides to make sure this passes through the value attributes
::attribute textContent GET
  forward message("VALUE")
::attribute textContent SET
  forward message("VALUE=")

-- get the element parent for this attribute
::attribute element GET
  use strict arg

  if self~isOwned then
      return self~ownerNode
  else return .nil

-- same as element
::attribute ownerElement GET
  forward message("ELEMENT")

-- some attribute properties
::attribute specified

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Element                                                             */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class "ElementImpl" subclass ParentNode public inherit Element NodeList ElementTraversal
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
  if namespaceURI == "" then namespaceURI = .nil

  colon1 = qname~pos(":")
  colon2 = qname~lastPos(":")
  -- no prefix
  if colon1 == 0 then
      -- local name and qualified name are the same
      localName = qname
  else parse var qname prefix =(colon1) =(colon2 + 1) localName


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
  if index > 0 then return nodeName~substr(1, index - 1)
  else return .nil

-- set the prefix for the element name
::attribute prefix SET
  expose nodeName localName
  use strict arg prefix

  -- we're either adding or replacing the prefix
  if prefix \= "" then
      nodeName = prefix":"localName
  -- or removing it entirely
  else nodeName = localName

-- retrieve the localName
::attribute localName GET

-- standard nodetype override
::attribute nodeType GET
  use strict arg
  return .node~ELEMENT_NODE

-- the node name attribue
::attribute nodeName GET

-- get the argument map holding the attributes
::attribute attributes GET
  expose attributes
  use strict arg

  if attributes == .nil then attributes = .AttributeMap~new(self, .nil)
  return attributes

-- set a new attribute set
::attribute attributes SET private

-- clone an element node
::method cloneNode
  expose attributes
  use strict arg deep = .false
  newNode = self~cloneNode(deep)

  -- the attributes are always copied regardless of the deep flag
  if attributes \= .nil then
      newNode~attributes = attributes~cloneMap(newNode)
  return newNode

-- get the base URI for this element...not supported yet
::attribute baseURI GET
  use strict arg
  return .nil

-- set the element owner document
::attribute ownerDocument SET private
  expose attributes
  use strict arg doc

  forward class(super) continue
  -- also set this for all of the attributes
  if attributes \= .nil  then attributes~ownerDocument = doc

-- retrieve the value of a named attribute
::method getAttribute
  expose attributes

  use strict arg name
  if attributes == .nil then return ""

  attr = attributes~getNamedItem(name)
  if attr == .nil then return ""
  else return attr~value

-- get the node associated with a specific named attribute
::method getAttributeNode
  expose attributes

  use strict arg name
  if attributes == .nil then return .nil
  return attributes~getNamedItem(name)

-- return a list of all elements by tag name
::method getElementsByTagName
  use strict arg tagname
  -- this version does a lazy search
  return .DeepNodeList~new(self, tagname)

-- this is the same as the nodename for an element
::attribute tagname GET
  expose nodeName
  return nodeName

-- remove an attribute from the element
::method removeAttribute
  expose attributes
  use strict arg name

  if attributes \= .nil  then
      return attributes~removeNamedItem(name)
  return .nil

-- remove an attribute node from an element
::method removeAttributeNode
  expose attributes
  use strict arg oldAttr

  if attrbutes \= .nil then return attributes~removeItem(oldAttr, .true)
  return .nil

-- set a new attribute value on an element
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
  else newAttr~value = value
  return newAttr

-- set an attribute on an element by node
::method setAttributeNode
  use strict arg newAttr

  -- get the attribute map.  This also creates it if
  -- we don't have one yet
  attributes = self~attributes
  return attributes~setNamedItem(newAttr)

-- set an attribute by localname/namespace pair
::method getAttributeNS
  expose attributes
  use strict arg namespaceURI, localName

  if attributes == .nil then return ""

  attr = attributes~getNamedItemNS(namespaceURI, localName)
  if attr == .nil then return ""
  else return attr~value

  -- set an attribute using a namespace URI and a qualified name
::method setAttributeNS
  expose attributes
  use strict arg namespaceURI, qualifiedName, value

  index = qualifiedName~pos(":")
  if pos > 0 then parse var qualifiedName prefix ":" localName
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

-- remove an attribute by specified localname/namespace pair
::method removeAttributeNS
  expose attributes
  use strict arg namespaceURI, localName

  if attributes == .nil then return .nil
  return attributes~removeNamedItemNS(namespaceURI, localName)


-- retrieve an attribute node by specified localname/namespace pair
::method getAttributeNodeNS
  expose attributes
  use strict arg namespaceURI, localName

  if attributes == .nil then return .nil
  return attributes~getNamedItemNS(namespaceURI, localName)

-- set an attribute node by specified localname/namespace pair
::method setAttributeNodeNS
  use strict arg newAttr

  -- get the attribute map.  This also creates it if
  -- we don't have one yet
  attributes = self~attributes
  return attributes~setNamedItemNS(newAttr)

-- test if an element has any attributes
::method hasAttributes
  expose attributes

  if attributes == .nil then return .false
  return attributes~length > 0

-- test if an element has a given attribute
::method hasAttribute
  use strict arg name
  return self~getAttributeNode(name) \= .nil

-- test if an element has a given attribute using a namespace qualified name
::method hasAttributeNS
  use strict arg namespaceURI, localName
  return self~getAttributeNodeNS(namespaceURI, localName) \= .nil

-- get all elements using a namespace qualified name
::method getElementsByTagNameNS
  use strict arg namespaceURI, localName

  return .DeepNodeList~new(self, localName, namespaceURI)

-- set a node on the element as an id attribute
::method setIdAttributeNode
  use strict arg attr, makeId

  self~makeAttributeId(attr, makeId)

-- set a named attribute as an id value for the element
::method setIdAttribute
  use strict arg name, makeId

  attr = self~getAttributeNode(name)
  if attr == .nil then return

  self~makeAttributeId(attr, makeId)

-- set an attribute to be an id attribute by namespace/localname
::method setIdAttributeNS
  use strict arg namespaceURI, localname, makeId

  attr = self~getAttributeNodeNS(namspaceURI, localname)
  if attr == .nil then return
  self~makeAttributeId(attr, makeId)

-- private method shared between the different setIdAttributeXXX methods
::method makeAttributeId private
  use strict arg attr, makeId

  attr~isID = makeId
  if makeId then self~ownerDocument~putIdentifier(attr~value, self)
  else self~ownerDocument~removeIdentifier(attr~value)

-- retrieve any schema typeName set on this element
::attribute typeName GET
  expose type
  use strict arg
  if type \= .nil then return type~typeName
  else return .nil

-- retrieve the type namespace
::attribute typeNamespace GET
  expose type
  use strict arg
  if type \= .nil then return type~namespace
  else return .nil

-- test if a node is derived from a given type
::method isDerivedFrom
  expose type
  use strict arg typeNamespace, typeName, derivationMethod
  if type \= .nil then
      return type~isDOMDerivedFrom(typeNamespace, typeName, derivationMethod)
  else return .false

-- set/retrieve type directly
::attribute type

-- retrieve the schema information
::attribute schemaTypeInfo GET
  use strict arg
  return self

-- ElementTraversal methods

-- return the count of child elements
::attribute childElementCount GET
  use strict arg
  count = 0
  child = self~firstElementChild
  loop while child \= .nil
      count += 1
      child = child~nextElementSibling
  end

  return count

-- get the first element child of this node (skipping over non-element children)
::attribute firstElementChild GET
  use strict arg
  node = self~firstChild

  loop while node \= .nil
      if node~nodeType == .Node~ELEMENT_NODE then return node
      else if node~nodeType == .Node~ENTITY_REFERENCE_NODE then do
          e = self~getFirstElementChild(node)
          if e \= .nil  then return e
      end
      node = node~nextSibling
  end
  return .nil

-- get the last elemet child of this element
::attribute lastElementChild GET
  use strict arg
  node = self~lastChild

  loop while node \= .nil
      if node~nodeType == .Node~ELEMENT_NODE then return node
      else if node~nodeType == .Node~ENTITY_REFERENCE_NODE then do
          e = self~getLastChildElement(node)
          if e \= .nil  then return e
      end
      node = node~previousSibling
  end
  return .nil


-- get the next element sibling of this node
::attribute nextElementSibling GET
  use strict arg
  node = self~nextLogicalSibling(self)

  loop while node \= .nil
      if node~nodeType == .Node~ELEMENT_NODE then return node
      else if node~nodeType == .Node~ENTITY_REFERENCE_NODE then do
          e = self~getFirstElementChild(node)
          if e \= .nil  then return e
      end
      node = node~nextLogicalSibling(node)
  end
  return .nil


-- get the previous element sibling
::attribute previousElementSibling GET
  use strict arg
  node = self~previousLogicalSibling(self)

  loop while node \= .nil
      if node~nodeType == .Node~ELEMENT_NODE then return node
      else if node~nodeType == .Node~ENTITY_REFERENCE_NODE then do
          e = self~getLastElementChild(node)
          if e \= .nil  then return e
      end
      node = node~previousLogicalSibling(node)
  end
  return .nil

-- get the first element child of this node
::method getFirstElementChild private
  use strict arg node

  top = node

  loop while node \= .nil
      if node~nodeType == .Node~ELEMENT_NODE then return node
      next = node~firstChild
      loop while next \= .nil
          if node == top then leave
          next = node~nextSibling
          if next \= .nil then do
              node = node~parentNode
              if node == .nil | node == top then return .nil
          end
      end
      node = next
  end
  return .nil

-- get the next logical sibling of this node
::method nextLogicalSibling private
  use arg node

  next = node~nextSibling

  if next == .nil then do
      parent = node~parentNode
      loop while parent \= .nil, parent~nodeType == .Node~ENTITY_REFERENCE_NODE
          next = parent~nextSibling
          if next \= .nil then leave
          parent = parent~parentNode
      end
  end

  return next

-- get the previous logical sibling
::method previousLogicalSibling private
  use arg node

  previous = node~previousSibling

  if previous == .nil then do
      parent = node~parentNode
      do while parent \= .nil, parent~nodeType == .Node~ENTITY_REFERENCE_NODE
          previous = parent~previousSibling
          if previous \= .nil then leave
          parent = parent~parentNode
      end
  end

  return previous

-- Returns the first element node found from a
-- non-recursive reverse order traversal of the given node.
::method getLastElementChild private
  use strict arg node

  top = node
  loop while node \= .nil
      if node~nodeType == .Node~ELEMENT_NODE then return node
      next = node~lastChild
      loop while next \= .nil
          if node == top then leave
          next = node~previousSibling
          if next \= .nil then do
              node = node~parentNode
              if node == .nil | node == top then return .nil
          end
      end
      node = next
  end
  return .nil

-- resolve the prefix that a node is using for a given namespace URI
::method lookupPrefix
  use strict arg uri

  -- pass this along to the specialized lookup method
  return self~lookupNamespacePrefix(uri, self)

-- resolve the prefix that a node is using for a given namespace URI
::method lookupNamespacePrefix private
  use strict arg uri, originalElement

  namespace = self~namespaceURI
  prefix = self~prefix

  -- the dom spec algorithm says that if the element has a namespace URI and
  -- prefix, then perform the prefix lookup and see if that prefix actually
  -- matches the uri.  If it does, that gets returned.
  if namespace == uri then do
      if prefix \== .nil then do
          if namespace = originalElement~lookupNamespaceURI(prefix) then
              return prefix
      end
  end

  -- ok, no luck above, now check the attributes for the
  -- appropriate definitions

  if self~hasAttributes then do
      loop attribute over self~attributes
          prefix = attribute~prefix
          value = attribute~nodeValue
          namespace = attribute~namespaceURI
          if namespace == "http://www.w3.org/2000/xmlns/" then do
              if attribute~nodeName == "xmlns" || (prefix == "xmlns" & value == uri) then do
                  localName = attribute~localName
                  if uri == originalElement~lookupNamespaceUri(localName) then return localName
              end
          end
      end
  end

  -- last chance...pass this up the element chain and ask again, using
  -- the same original element
  ancestor = self~getElementAncestor
  if ancestor \= .nil then return ancestor~lookupNamespacePrefix(uri, originalElement)
  -- unresolved
  return .nil

-- look up the namespace associate with a prefix
::method lookupNamespace
  use strict arg specifiedPrefix

  namespace = self~namespaceURI
  prefix = self~prefix
  if namespace \== .nil then do
      -- if the prefixes match, then return the namespace.  Note that
      -- this works even if both prefixes are .nil
      if prefix == specifiedPrefix then return namespace
  end

  -- ok, no luck above, now check the attributes for the
  -- appropriate definitions

  if self~hasAttributes then do
      loop attribute over self~attributes
          prefix = attribute~prefix
          value = attribute~nodeValue
          namespace = attribute~namespaceURI
          if namespace == "http://www.w3.org/2000/xmlns/" then do
              -- if asking for the default namespace and this is the xmlns
              -- attribute, then return the value, if it has one
              if specifiedPrefix == .nil & attribute~nodeName = "xmlns" then do
                  if value \= "" then return value
                  else return .nil
              end
              else if prefix == "xmlns" & attribute~localname == specifiedPrefix then do
                  if value \= "" then return value
                  else return .nil
              end
          end
      end
  end

  -- last chance...pass this up the element chain and ask again, using
  -- the same original element
  ancestor = self~getElementAncestor
  if ancestor \= .nil then return ancestor~lookupNamespaceURI(uri)
  -- unresolved
  return .nil

-- test if the current node is using a given namespace as the default
::method isDefaultNamespace
  use strict arg uri

  namespace = self~namespaceURI
  prefix = self~prefix

  -- if there's no prefix in use, any namespace for this element
  -- will be from a default, so just compare.  This also handles
  -- the situation where any of these are .nil
  if prefix == .nil | prefix == "" then do
      return namespace == uri
  end

  -- if the node has attributes, then we need to look for an xmlns
  -- attribute and compare that
  if self~hasAttributes then do
      attr = self~getAttributeNodeNS("http://www.w3.org/2000/xmlns/", "xmlns")
      if attr \== .nil then return uri == attr~nodeValue
  end

  return .false

  -- if this node is owned and the owner is an attribute, then
  -- pass along the lookup
  ownerNode = self~ownerNode
  if ownerNode \== .nil, ownerNode~nodeType == .Node~ELEMENT_NODE then
      return ownerNode~isDefaultNamespace(uri)
  return .false


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

-- character data nodes do not have children
::attribute childNodes GET
  use strict arg
  -- always returns an empty node list
  return .NodeListImpl~new

-- retrieve the nodevalue, which is the same as the data
::attribute nodeValue GET
  expose data
  use strict arg
  return data

-- set the nodeValue
::attribute nodeValue SET
  expose data
  use strict arg value, replace = .false

  oldValue = data
  -- announce that we're modifying
  self~ownerDocument~modifyingCharacterData(self, replace)

  data = value
  -- and say we're done
  self~ownerDocument~modifiedCharacterData(self, oldValue, value, replace)

-- data is mapped to the node vale
::attribute data GET
  forward message("NODEVALUE")
::attribute data SET
  forward message("NODEVALUE=")

-- retrieve the data length
::attribute length GET
  expose data
  use strict arg
  return data~length

-- append a string to the node data
::method appendData
  expose data
  use strict arg newData

  if newData == .nil then return

  -- set the node value, which also handles events
  self~nodeValue = data||newData

-- delete data from the node
::method deleteData
  expose data
  use strict arg offset, count, replace = .false

  tailLength = max(data~length - count - offset, 0)
  if offset >= data~length  then newData = data
  else newData = data~delstr(offset + 1, count)
  -- set the node value to the adjusted version
  -- we do this directly since we don't want to
  -- raise set events
  data = newData

  -- indicate we deleted
  self~ownerDocument~deletedText(self, offset, count)

-- insert data into the node
::method insertData
  expose data
  use strict arg offset, newData, replace = .false

  -- NB:  In this case, we don't add one to the offset because
  -- the Rexx insert function inserts after the given offset, not
  -- before.  This actually works to our advantage.
  newValue = data~insert(newData, offset)
  -- set directly to avoid set events
  data = newValue
  -- send the insertion event
  self~ownerDocument~insertedText(self, offset, newData~length)

-- replace data in the node
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

-- extract a substring from the node
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

-- return the string created by merging the values of all logically adjacent
-- text nodes.  This includes both Text and CData nodes.
::method wholeText
  use strict arg

  -- the parent node contains all of the siblings, so everything is done
  -- relative to that
  parent = self~parentNode
  -- no parent means no siblings, so our value is all we have
  if parent == .nil then return self~textContent

  buffer = .mutableBuffer~new
  loop node over parent~children
      if node~nodeType == .Node~TEXT_NODE | node~nodeType == .Node~CDATA_NODE then
          buffer~append(node~nodeValue)
      else if node~nodeType == .Node~ENTITY_REFERENCE_NODE then
          buffer~append(node~entityRefValue)
  end

  return buffer~string

-- replace all the text of a text node
::method replaceWholeText
  use strict arg content

  parent = self~parentNode
  -- no content or a null string content, we just remove everything
  if content == .nil | content == "" then do
      -- just remove ourselves from the parent
      if parent \= .nil then parent~removeChild(self)
      return .nil
  end

  self~nodeValue = content

  -- no parent, there's nothing to replace
  if parent == .nil then return self

  loop node over parent
      -- leave our own node in the child list...just delete the siblings
      if node == self then iterate
      -- remove any logically adjacent text or entity reference nodes
      nodetype = node~nodeType
      if nodeType = .Node~TEXT_NODE | nodeType = .Node~CDATA_SECTION_NODE
          -- remove the node from the parent
          then parent~removeChild(node)
  end

  -- this is now the only text node
  return self

-- split the text at the indicated position, creating a new node with
-- the split off text and updating the value of the target node
::method splitText
  use strict arg offset

  newText = self~ownerDocument~createTextNode(self~data~substr(offset + 1))
  self~nodeValue = self~data~substr(1, offset)

  -- now insert the new text node
  parent = self~parentNode
  if parent \= .nil then parent~insertBefore(newText, self~nextSibling)

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
/* Method: init                                                               */
/* Description: instance initialization.                                      */
/*----------------------------------------------------------------------------*/

::method init
  expose publicID systemID
  use strict arg self~nodename, publicID, systemID
  self~nodeType = .Node~DOCUMENT_TYPE_NODE
  self~nodeValue = .nil
  self~entities = .NamedNodeMapImpl~new
  self~notations = .NamedNodeMapImpl~new
  self~publicID = .nil
  self~systemID = .nil
  self~internalSubset = .nil

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

::attribute entities get

/*----------------------------------------------------------------------------*/
/* Method: notations                                                          */
/* Description: return the list of notations                                  */
/*----------------------------------------------------------------------------*/

::attribute notations get

-- override for text content.  For this type, it always
-- returns .nil
::attribute textContent GET
  use strict arg
  return .nil

-- override for setting text content.  For this type, it has no effect.
::attribute textContent SET
  use strict arg value
  -- this is a NOP for nodes where this has no meaning.

-- override for the default node isEqual method.  DocumentType nodes have
-- different equality rules
::method isEqualNode
  use strict arg other

  if other~nodeType \== self~nodeType then return .false
  if self~publicId \= other~publicId then return .false
  if self~systemId \= other~systemId then return .false
  if self~internalSubset \= other~internalSubset then return .false

  -- the notations and entities must be equal
  if \self~notations~isEqual(other~notations) then return .false
  return self~entities~isEqual(other~entities)

-- general attributes
::attribute internalSubset
::attribute publicID
::attribute systemID


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

-- resolve the prefix that a node is using for a given namespace URI
::method lookupPrefix
  use strict arg uri

  -- always returns .nil
  return .nil

-- resolve the namespace that a node is using for a given prefix
::method lookupNamespaceURI
  use strict arg prefix

  -- always returns .nil
  return .nil

-- perform a default namespace test for a node
::method isDefaultNamespace
  use strict arg uri

  -- always returns .false for this type
  return .false

-- override for text content.  For this type, it always
-- returns .nil
::attribute textContent GET
  use strict arg
  return .nil

-- override for setting text content.  For this type, it has no effect.
::attribute textContent SET
  use strict arg value
  -- this is a NOP for nodes where this has no meaning.


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Entity -- an entity value                                           */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class "EntityImpl" public subclass ParentNode inherit Entity
::method init
  expose name publicId systemId xmlEncoding inputEncoding xmlVersion notationName
  use strict arg ownerDoc, name
  self~init:super(ownerDoc)
  publicId = .nil
  systemId = .nil
  xmlEncoding = .nil
  inputEncoding = .nil
  xmlVersion = .nil
  notationName = .nil

::attribute nodeType GET
  use strict arg
  return .Node~ENTITY_NODE

::attribute nodeName GET
  expose name
  use strict arg
  return name

::attribute publicId
::attribute systemId
::attribute xmlVersion
::attribute xmlEncoding
::attribute inputEncoding
::attribute notationName

-- resolve the prefix that a node is using for a given namespace URI
::method lookupPrefix
  use strict arg uri

  -- always returns .nil
  return .nil

-- resolve the namespace that a node is using for a given prefix
::method lookupNamespaceURI
  use strict arg prefix

  -- always returns .nil
  return .nil

-- perform a default namespace test for a node
::method isDefaultNamespace
  use strict arg uri

  -- always returns .false for this type
  return .false

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

-- override for the nodetype
::attribute nodeType GET
  use strict arg
  return .Node~ENTITY_REFERENCE_NODE

-- entity references are created with a name
::attribute nodeName GET
  expose name
  use strict arg
  return name

-- retrieve the value of the entity reference
::attribute entityRefValue GET
  use strict arg

  firstChild = self~firstChild
  -- no value if there are no children
  if firstChild == .nil then return ""

  -- an entity reference may contain elements too...we only care about
  -- the real text node children.  We can optimize this slightly for
  -- the common case of just a single child node
  if firstChild~nextSibling == .nil then do
      -- get the value
      if firstChild~nodeType == .Node~ENTITY_REFERENCE_NODE then
          return firstChild~entityRefValue
      else if firstChild~nodeType == .Node~TEXT_NODE then
          return firstChild~nodeValue
      else return .nil
  end
  buffer = .mutablebuffer~new(value)
  appended = .false
  loop node over self
      -- only entity references, text, and cdata nodes are of interest here
      if node~nodeType == .Node~ENTITY_REFERENCE_NODE then do
          buffer~append(next~entityRefValue)
          appended = .true
      end
      else if next~nodeType == .Node~TEXT_NODE | next~nodeType == .Node~CDATA_SECTION_NODE then do
          buffer~append(next~nodeValue)
          appended = .true
      end
  end

  -- if we actually found text nodes of some sort, return the value
  return buffer~string

-- test if modification or replacement of an entity reference is allowed.
-- for this to be true, the children can only be text, cdata, or entity reference
-- nodes
::method isModificationAllowed
  use strict arg

  loop node over self
      nodeType = node~nodeType
      if nodeType == .Node~TEXT_NODE | nodType = .Node~CDATA_SECTION_NODE | .Node~ENTITY_REFERENCE_NODE then iterate
      return .false
  end

  return .true

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

-- create a document type value
::method createDocumentType
  use strict arg qualifiedName, publicID, systemID
  self~checkQName(qualifiedName)
  return .DocumentTypeImpl(qualifiedName, publicID, systemID)

-- validate a QName
::method checkQName
  use strict arg qname

  if \.XMLChar~isValidQName(qname) then
      .DomErrors~raiseError(.DomException~INVALID_CHARACTER_ERR)

-- create a document node
::method createDocument
  use strict arg namespaceURI = .nil, qualifiedName = .nil, doctype = .nil

  if doctype \= .nil, doctype~ownerDocument \= .nil then
      .DomErrors~raiseError(.DomException~WRONG_DOCUMENT_ERR)

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

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class:  DOMException                                                       */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class "DOMException" public
::constant INDEX_SIZE_ERR 1
::constant DOMSTRING_SIZE_ERR 2
::constant HIERARCHY_REQUEST_ERR 3
::constant WRONG_DOCUMENT_ERR 4
::constant INVALID_CHARACTER_ERR 5
::constant NO_DATA_ALLOWED_ERR 6
::constant NO_MODIFICATION_ALLOWED_ERR 7
::constant NOT_FOUND_ERR 8
::constant NOT_SUPPORTED_ERR 9
::constant INUSE_ATTRIBUTE_ERR 10
::constant INVALID_STATE_ERR 11
::constant SYNTAX_ERR 12
::constant INVALID_MODIFICATION_ERR 13
::constant NAMESPACE_ERR 14
::constant INVALID_ACCESS_ERR 15
::constant VALIDATION_ERR 16
::constant TYPE_MISMATCH_ERR 17
::constant BAD_BOUNDARYPOINTS_ERR 18
::constant INVALID_NODE_TYPE_ERR 19

-- instance item for DOM errors.  This is attached to the condition information
::method init
  expose code message
  use strict arg code, message

::method string
  expose code message
  return "DOM Error" code":" message

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class:  DOMErrors                                                          */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


-- central class for raising DOMException events.
::class "DOMErrors" public subclass DomException
::method init class
  expose messageTable
  messageTable = .array~new
  messageTable[.DomException~HIERARCHY_REQUEST_ERR] =  "An attempt was made to insert a node where it is not permitted."
  messageTable[.DomException~INDEX_SIZE_ERR] =         "The index or size is negative, or greater than the allowed value."
  messageTable[.DomException~INUSE_ATTRIBUTE_ERR] =    "An attempt is made to add an attribute that is already in use elsewhere."
  messageTable[.DomException~INVALID_ACCESS_ERR] =     "A parameter or an operation is not supported by the underlying object."
  messageTable[.DomException~INVALID_CHARACTER_ERR] =  "An invalid or illegal XML character is specified."
  messageTable[.DomException~INVALID_MODIFICATION_ERR] = "An attempt is made to modify the type of the underlying object."
  messageTable[.DomException~INVALID_STATE_ERR] =        "An attempt is made to use an object that is not, or is no longer, usable."
  messageTable[.DomException~NAMESPACE_ERR] =            "An attempt is made to create or change an object in a way which is incorrect with regard to namespaces."
  messageTable[.DomException~NOT_FOUND_ERR] =            "An attempt is made to reference a node in a context where it does not exist."
  messageTable[.DomException~NOT_SUPPORTED_ERR] =        "The implementation does not support the requested type of object or operation."
  messageTable[.DomException~NO_DATA_ALLOWED_ERR] =      "Data is specified for a node which does not support data."
  messageTable[.DomException~NO_MODIFICATION_ALLOWED_ERR] = "An attempt is made to modify an object where modifications are not allowed."
  messageTable[.DomException~SYNTAX_ERR] =                  "An invalid or illegal string is specified."
  messageTable[.DomException~VALIDATION_ERR] =              "A call to a method such as insertBefore or removeChild would make the Node invalid with respect to document grammar."
  messageTable[.DomException~WRONG_DOCUMENT_ERR] =          "A node is used in a different document than the one that created it."
  messageTable[.DomException~TYPE_MISMATCH_ERR] =           "The value type for this parameter name is incompatible with the expected value type."
  messageTable[.DomException~BAD_BOUNDARYPOINTS_ERR] =      "The boundary-points of a Range do not meet specific requirements."
  messageTable[.DomException~INVALID_NODE_TYPE_ERR] =       "The container of a boundary-point of a Range is being set to either a node of an invalid type or a node with an ancestor of an invalid type."

-- raise an error using the DOMException error numbers
::method raiseError class
  expose messageTable
  use arg code

  raise syntax 98.900 array(.DOMException~new(code, messageTable[code]))    -- just raise this as a user execution error


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: DOMEvent                                                            */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
::class "DOMEvent" public
::constant NONE 0
::constant CAPTURING_PHASE 1
::constant AT_TARGET 2
::constant BUBBLING_PHASE 3

::method init
  expose type initialized cancelable target eventPhase currentTarget timeStamp propagationStopped defaultPrevented isTrusted immediatePropagationStopped
  type = .nil
  initialized = .false
  currentTarget = .nil
  eventPhase = .nil
  timeStamp = .DateTime~new
  propagationStopped = .false
  defaultPrevented = .false
  cancelable = .false
  isTrusted = .false  -- spec requires this initially be false
  immediatePropagationStopped = .false

::method initEvent
  expose type bubbles cancelable initialized
  use strict arg type, bubbles, cancelable
  initialized = .true

::attribute bubbles GET
::attribute cancelable GET
::attribute currentTarget GET
::attribute eventPhase GET
::attribute target GET
::attribute type GET
::attribute timeStamp GET
::attribute defaultPrevented
::attribute propagationStopped
::attribute isTrusted GET

::method preventDefault
  expose defaultPrevented
  use strict arg
  defaultPrevented = .true

::method stopPropagation
  expose propagationStopped
  use strict arg
  propagationStopped = .true

::method stopImmediatePropagation
  expose immediatePropagationStopped
  use strict arg
  immediatePropagationStopped = .true

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class:  DOMMutationEvent                                                   */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class "DOMMutationEvent" subclass DOMEvent public
::constant MODIFICATION 1
::constant ADDITION 2
::constant REMOVAL 3

::constant DOM_SUBTREE_MODIFIED "DOMSubtreeModified"
::constant DOM_NODE_INSERTED "DOMNodeInserted"
::constant DOM_NODE_REMOVED "DOMNodeRemoved"
::constant DOM_NODE_REMOVED_FROM_DOCUMENT "DOMNodeRemovedFromDocument"
::constant DOM_NODE_INSERTED_INTO_DOCUMENT "DOMNodeInsertedIntoDocument"
::constant DOM_ATTR_MODIFIED "DOMAttrModified"
::constant DOM_CHARACTER_DATA_MODIFIED "DOMCharacterDataModified"
::constant DOM_ELEMENT_MODIFIED "DOMElementModified"


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
/* Class: NodeIteratorImpl                                                    */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
::class "NodeIteratorImpl" public inherit NodeIterator
::method init
  expose document root currentNode whatToShowFlags whatToShow nodeFilter entityReferenceExpansion forward
  use strict arg document, root, whatToShow = (.NodeFilter~SHOW_ALL), nodeFilter = .nil, entityReferenceExpansion = .false

  -- convert the whatToShow flags into a string of binary digits that can be
  -- easily tested
  if whatToShow == .NodeFilter~SHOW_ALL then
      whatToShowFlags = "111111111111"    -- we'll set all flags to true
      -- convert this to a binary string, then pad out to 11 digits for the
      -- mapping
  else whatToShowFlags = whatToShow~d2x~x2b~right(12, 0)
  forward = .true      -- we start iterating in a forward direction

-- attributes defined by the interface
::attribute root GET
::attribute whatToShow GET
::attribute filter GET
::attribute expandEntityReferences GET

-- retrieve the next node in the iteration sequence.  Returns .nil at
-- the end
::method nextNode
  expose root currentNode forward
  use strict arg

  -- a null root is unusual, but not illegal
  if root == .nil then return .nil
  nextNode = currentNode

  loop forever
      -- if we're backing up, repeat the current node
      if \forward & nextNode \== .nil then nextNode = currentNode
      else do
          if \entityReferenceExpansion, nextNode \= .nil, nextNode~nodeType == .Node~ENTITY_REFERENCE_NODE then
              nextNode = self~locateNextNode(nextNode, .false)
          else nextNode = self~locateNextNode(nextNode, .true)
      end
      forward = .true
      -- did not find a next node, return .nil
      if nextNode = .nil then return .nil

      -- now try the filters
      if self~acceptNode(nextNode) then do
      -- if the filter says this one is ok, then set the iteration
      -- position and return
          currentNode = nextNode
          return currentNode
      end
      -- keep looping until we find something good
  end

-- back up to the previous node
::method previousNode
  expose root currentNode forward
  use strict arg

  -- nothing to work from...we're done
  if root == .nil | currentNode = .nil then return .nil

  previousNode = currentNode

  loop forever
      if forward & previousNode \= .nil then previousNode = currentNode
      else previousNode = self~previousNode(previousNode)

      -- this is going backwards
      forward = .false

      if previousNode = .nil then return .nil

      if self~acceptNode(previousNode) then do
          currentNode = previousNode
          return currentNode
      end
  end

-- a test to determine what should be accepted
::method acceptNode private
  expose nodeFilter whatToShowFlags
  use arg node
  nodeFlag = (2**(node~nodetype - 1))~d2x~x2b~right(12, 0)

  -- test the flags first
  if whatToShowFlags~bitxor(nodeflag) = 0 then return .false
  -- then filter
  if nodeFilter \= .nil then
      return nodeFilter~acceptNode(node) == .NodeFilter~FILTER_ACCEPT
  -- true based on the flags
  return .true

-- internal method to locate the next node
::method locateNextNode private
  expose root
  use strict arg node, visitChildren

  -- might have already reached the end
  if node == .nil then return .nil

  if visitChildren then do
      if node~hasChildNodes then return node~firstChild
  end

  -- back to the root?  We're done
  if node == root then return .nil

  -- use the next sibling if it exists
  result = node~nextSibling
  if result \= .nil then return result

  -- go up to the parent
  parent = node~parentNode
  loop while parent \== .nil & parent \== root
      result = parent~nextSibling
      if result \= .nil then return result
      parent = parent~parentNode
  end

  -- end of the nodes, return .nil
  return .nil

-- internal node to locate a previous node
::method locatePreviousNode private
  expose root entityReferenceExpansion
  use strict arg node

  -- back to the root?  We're done
  if node == root then return .nil

  -- use the next sibling if it exists
  result = node~previousSibling
  if result == .nil then
      -- if no previous sibling, then step up to the parent
      return node~parentNode

  -- if the sibling has children, drill down to the last last child
  if result~hasChildNodes & \(\entityReferenceExpansion & result~nodeType == .Node~ENTITY_REFERENCE_NODE) then do
      loop while result~hasChildNodes
          result = result~lastChild
      end
  end

  return result

-- remove a node from the iteration
::method removeNode
  expose forward currentNode
  use strict arg node

  if node = .nil then return

  deleted = self~matchNodeOrParent(node)

  if forward then currentNode = self~locatePreviousNode(deleted)
  else do
      next = self~locateNextNode(deleted, .false)
      if next \= .nil then currentNode = next
      else do
          currentNode = self~locatePreviousNode(deleted)
          forward = .true
      end
  end

::method matchNodeOrParent private
  expose currentNode root
  use strict arg node

  if currentNode == .nil then return .nil

  target = currentNode
  loop while target \= root
      if node == target then return target
      target = target~parentNode
  end

  return .nil

-- detach this iterator from the document
::method detach
  expose root currentNode document

  root = .nil
  currentNode = .nil
  document~removeNodeIterator(self)
  document = .nil

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: RangeImpl                                                           */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class "RangeImpl" public inherit Range
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

-- test if a range has been collapsed.  A collapsed range
-- starts and ends in the same container and has the same offset
::attribute collapsed GET
  expose startContainer endContainer startOffset endOffset
  use strict arg
  return startContainer == endContainer & startOffset = endOffset

-- locate the common ancestor of both the start and end containers
::attribute commonAncestorContainer GET
  expose startContainer

  -- build a chain of the start containers ancestor chain
  startAncestors = startContainer~ancestors
  endAncestors = endContainer~ancestors

  -- this will give all of the common elements,
  -- retaining the order.  The last one is the
  -- element we want
  common = startAncestors~intersection(endV)
  -- not really good if they have no common ancestor
  if common~isEmpty then return .nil
  -- the last item is the one we want
  return common[common~last]

-- set the range start as a node and offset
::method setStart
  expose startContainer startOffset
  use strict arg refNode, offset

  -- validate this is a valid starting point and set the start item
  self~checkIndex(refNode, offset)
  startContainer = refNode
  startOffset = offset

  self~checkCollapse    -- this new range may require collapsing

-- set the end position
::method setEnd
  expose endContainer endOffset
  use strict arg refNode, offset

  -- validate this is a valid ending point and set the end items
  self~checkIndex(refNode, offset)
  endContainer = refNode
  endOffset = offset

  self~checkCollapse    -- this new range may require collapsing

-- set the start position as being before a target node
::method setStartBefore
  expose startContainer startOffset
  use strict arg refNode

  startContainer = refNode~parentNode
  -- the starting offset is one less than the node offset
  startOffset = self~nodeOffset(startContainer) - 1

  self~checkCollapse    -- this new range may require collapsing

-- set the start position to immediately after a given node
::method setStartAfter
  expose startContainer startOffset endContainer endOffset
  use strict arg refNode

  startContainer = refNode~parentNode
  -- the starting offset is the node offset
  startOffset = self~nodeOffset(startContainer)

  self~checkCollapse    -- this new range may require collapsing

-- set the end position after a given node
::method setEndAfter
  expose endContainer endOffset
  use strict arg refNode

  -- the starting offset is the node offset
  startOffset = self~nodeOffset(startContainer)

  self~checkCollapse    -- this new range may require collapsing

-- collapse a range
::method collapse
  expose startContainer startOffset endContainer endOffset
  use strict arg toStart

  -- collapsing to the start?  The end becomes same as the start
  if toStart then do
      endContainer = startContainer
      endOffset = startOffset
  end
  -- collapsing to the end...the start becomes the end
  else do
      startContainer = endContainer
      startOffset = endOffset
  end

-- select a node.  This node is both the start and end of the range.
::method selectNode
  expose startContainer startOffset endContainer endOffset
  use strict arg refnode

  parent = refNode~parentNode
  if parent == .nil then do
      startContainer = parent
      endContainer = parent
      ennOffset = self~nodeOffset(refnode)
      startOffset = endOffset - 1
  end

-- select the contents of a node.  The start and end containers are
-- the selected nodes, and the offsets cover all of the children
::method selectNodeContents
  expose startContainer startOffset endContainer endOffset
  use strict arg refnode

  startContainer = refNode
  startOffset = 0
  endContainer = refNode
  endOffset = self~nodeOffset(refNode~lastChild)

-- compare some boundary points between this range and
-- another range
::method compareBoundaryPoints
  expose startContainer startOffset endContainer endOffset
  use strict arg how, sourceRange

  select
      -- comparing the start points
      when how == .Range~START_TO_START then do
          endPointA = sourceRange~startContainer
          endPointB = startContainer
          offsetA = sourceRange~startOffset
          offsetB = startOffset
      end
      -- comparing the other start to our end
      when how == .Range~START_TO_END then do
          endPointA = sourceRange~startContainer
          endPointB = endContainer
          offsetA = sourceRange~startOffset
          offsetB = endOffset
      end
      -- comparing the other end to our start
      when how == .Range~END_TO_START then do
          endPointA = sourceRange~endContainer
          endPointB = startContainer
          offsetA = sourceRange~endOffset
          offsetB = startOffset
      end
      -- comparing the two end points
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
  loop while parent \= .nil
      if parent == endPointA then do
          if offsetA <= self~indexOf(current, endPointA) then return 1
          else return -1
      end
      current = parent
      parent = parent~parentNode
  end

  --   case 3: Child C of container B is ancestor of A
  current = endPointA
  parent = current~parentNode
  loop while parent \= .nil
      if parent == endPointB then do
          if self~indexOf(current, endPointB) < offsetB then return 1
          else return -1
      end
      current = parent
      parent = parent~parentNode
  end

  --   case 4: preorder traversal of context tree.
  depthA = self~depthOf(endPointA)
  depthB = self~depthOf(endPointB)

  depthDiff = depthA - depthB

  -- if A is deeper, back up
  loop while depthDiff > 0
      endPointA = endPointA~parentNode
      depthDiff -= 1
  end

  -- or possible B is deeper
  loop while depthDiff < 0
      endPointB = endPointB~parentNode
      depthDiff += 1
  end

  -- we should be at equal depths now, so keep going up
  -- until these merge
  parentA = endPointA~parentNode
  parentB = endPointB~parentNode
  loop while parentA \= parentB
      endPointA = parentA
      endPointB = parentB
      parentA = parentA~parentNode
      parentB = parentB~parentNode
  end

  -- now see if B follows A.
  node = endPointA~nextSibling
  loop while node \= .nil
      if node == endPointB then return 1
      node = node~nextSibling
  end
  -- B must precede A
  return -1

-- delete all of the range contents.
::method deleteContents
  use strict arg
  self~traverseContents(self~DELETE_CONTENTS)

-- extract the contents and return
::method extractContents
  use strict arg
  return self~traverseContents(self~EXTRACT_CONTENTS)

-- clone the contents in the range
::method cloneContents
  use strict arg
  return self~traverseContents(self~CLONE_CONTENTS)

-- insert a node into the range
::method insertNode
  expose startContainer startOffset endContainer endOffset insertedFromRange
  use strict arg newNode

  if newNode == .nil then return

  type = newNode~nodeType
  currentChildren = 0
  -- when we do inserts, our listeners will get called back to inform us of this.
  -- this flag lets the listener know that we're the source of this callback.
  insertedFromRange = .true

  -- if the start container is a text node. then the range
  -- refers to the text within the node. This is a string insertion
  if startContainer == .Node~TEXT_NODE then do
      parent = startContainer~parentNode
      currentChildren = parent~childNodes~length
      -- this will split the text into two nodes, making the
      -- second node the next sibling of this one.  We insert the
      -- new node between these two.
      splitNode = startContainer~splitText(startOffset)
      -- insert this between the container and the split node
      parent~insertBefore(newNode, splitNode)
      -- update the ranges.  If the range was inside the
      -- same text node, then the end container is the node
      -- that was split off and the offset is adjusted for the
      -- amount that stayed with the original node
      if endContainer == startContainer then do
          endContainer = splitNode
          endOffset -= startOffset
      end
      -- if the end was the parent node, then adjust by the number of
      -- children added.
      else if endContainer == parent then
          endOffset += parent~childNodes~length - currentChildren
      -- broadcast a data split
      self~signalSplitdata(startContainer, cloneCurrent, startOffset)
  end
  else do
      -- not a text node, so we're inserting between nodes
      -- in the same container, so the offsets refer to the container
      children.  Remember how many there are
      if endContainer == startContainer then
          currentChildren = endContainer~childNodes~length
      current = startContainer~firstChild
      -- skip forward to the target starting offset
      loop for startOffset while current \= .nil
          current = current~nextSibling
      end

      -- the offset might be at the end, so we may have to append
      if current \= .nil then
          startContainer~insertBefore(newNode, current)
      else
          startContainer~appendChild(newNode)
      -- if start and end are the same, then adjust the end offset by the
      -- amount inserted.
      if endContainer = startContainer & endOffset \== 0 then
          endOffset += endContainer~childNodes~length - currentChildren
  end
  -- as you were...
  insertedFromRange = .false

-- surround the contents of a range with a node.  This basically
-- makes the entire range a child of the provided node
::method surroundContents
  expose startContainer startOffset endContainer endOffset
  use strict arg newParent

  if newParent == .nil then return

  type = newParent~nodeType
  realStart = startContainer
  realEnd = endContainer

  -- text nodes for start and end change things because the
  -- offsets refer to positions inside the text content
  if startContainer~nodeType == .Node~TEXT_NODE then
      realStart = startContainer~parentNode

  if endContainer~nodeType == .Node~TEXT_NODE then
      realEnd = endContainer~parentNode

  -- get the contents of the range as a document fragment
  frag = self~extractContents()
  -- insert the new node into the range
  self~insertNode(newParent)
  -- append the node to the parent
  newParent~appendChild(frag)
  -- the range now is the new parent
  self~selectNode(newParent)

-- clone the range, returning a new range covering the same positions
::method cloneRange
  expose document startContainer startOffset endContainer endOffset

  -- get a new range from the document and set the same start/end values
  range = document~createRange
  range~setStart(startContainer, startOffset)
  range~setEnd(endContainer, endOffset)
  return range

-- return the string value of the range. This is created by extracting the TEXT and CDATA
-- and concatenating them together into a single string
::method string
  expose startContainer startOffset endContainer endOffset

  node = startContainer
  stopNode = endContainer

  buffer = .mutablebuffer~new

  -- if the start is a text node or cdata node, then the start offset is inside the character data
  if startContainer~nodeType == .Node~TEXT_NODE | startContainer~nodeType == .Node~CDATA_SECTION_NODE then do
      -- if the start and end are the same, we can just return the substring directly
      if startContainer == endContainer then
          return startContainer~nodeValue~substr(startOffset + 1, endOffset - startOffset)
      -- append the covered section
      buffer~append(startContaine~substringData(startOffset))
  end
  else do
      -- step forward to the target offset
      node = node~firstChild
      if startOffset > 0 then do
          loop for startOffset while node \= .nil
              node = node~nextSibling
          end
      end
      -- if we reached the end, then we need to step to the next logical node
      if node == .nil then
          node = self~nextNode(startContainer, .false)
  end
  -- if the end it not a text type, then we need to do the same thing
  if endContainer~nodeType \= .Node~TEXT_NODE & endContainer~nodeType \= .Node~CDATA_SECTION_NODE then do
      stopNode = endContainer~firstChild
      loop for endOffset while stopNode \= .nil
          stopNode = stopNode~nextSibling
      end

      if stopNode == .nil then
          stopNode = self~nextNode(endContainer, .false)
  end

  -- step throug everything appending the contents of the text nodes
  loop while node \= stopNode, node \= .nil
      if node~nodeType == .Node~TEXT_NODE | node~nodeType == .Node~CDATA_SECTION_NODE then
          buffer~append(node~nodeValue)
      node = self~nextNode(node, .true)
  end

  -- and finally, check for an end target of a text node and append just the subpiece of it
  if endContainer~nodeType == .Node~TEXT_NODE | endContainer~nodeType == .Node~CDATA_SECTION_NODE then
      buffer~append(endContainer~substringData(1, endOffset))

  return buffer~string

-- detach a range from a document
::method detach
  expose document
  use strict arg
  document~removeRange(self)
  document = .nil

-- broadcast a data splitting event to any other ranges
::method signalSplitData
  expose splitNode document
  use strict arg node, newNode, offset

  splitNode = node
  document~splitData(node, newNode, offset)
  splitNode = .nil

-- receive a split data notification from the document
::method receiveSplitData
  expose startContainer startOffset endContainer endOffset splitNode
  use strict arg node, newNode, offset

  -- this could be one from us
  if node == .nil | newNode == .nil | splitNode == node then return

  -- a split to a text node that is our start container?  If the split offset
  -- is before our start, we need to adjust the offset
  if node == startContainer & startContainer~nodeType == .Node~TEXT_NODE then do
      if startOffset > offset then do
          startOffset = startOffset - offset
          startContainer = newNode
      end
  end

  -- a similar check for the end
  if node == endContainer & endContainer~nodeType == .Node~TEXT_NODE then do
      if endtOffset > offset then do
          endOffset = endOffset - offset
          endContainer = newNode
      end
  end

-- broadcast a data deletiong event
::method deleteData
  expose deleteNode
  use strict arg node, offset, count

  deleteNode = node
  node~deleteData(offset, count)
  deleteNode = .nil

-- and handle a text deletion event
::method receiveDeletedText
  expose startContainer startOffset endContainer endOffset deleteNode
  use strict arg node, offset, count

  -- nothing to do if this came from us
  if node == .nil | deleteNode == node then return

  -- if deleted from our start container, we need to adjust our starting offset
  if node == startContainer then do
      if startOffset > offset + count then
          startOffset = offset + (startOffset - (offset + count))
      else if startOffset > offset then startOffset = offset
  end
  if node == endContainer then do
      if endOffset > offset + count then
          endOffset = offset + (endOffset - (offset + count))
      else if endOffset > offset then endOffset = offset
  end

-- broadcast a data insertion
::method insertData
  expose insertNode
  use strict arg node, index, insert

  insertNode = node
  node~insertData(index, insert)
  insertNode = .nil

-- handle a data insertion notification from the document
::method receiveInsertedText
  expose startContainer startOffset endContainer endOffset insertNode
  use strict arg node, index, len

  -- ignore if we triggered this
  if node == .nil | deleteNode == node then return

  -- adjust the start or end offset if the insertion occurred
  -- in our end points
  if node == startContainer then
      if index < startContainer then startOffset += len

  if node == endContainer then
      if index < endOffset then endOffset += len

-- handle a text replacement event
::method receiveReplacedText
  expose startContainer startOffset endContainer endOffset
  use strict arg node

  if node == .nil then return

  -- if the replacement occurred in our nodes, then the offsets
  -- go to zero.
  if node == startContainer then startOffset = 0
  if node == endContainer then endOffset = 0

-- an insertion coming from the DOM ... we might have been the ones to trigger this.
::method insertedNodeFromDOM
  expose startContainer startOffset endContainer endOffset insertNode insertedFromRange
  use strict arg node

  -- this was our update, ignore it
  if node == .nil | insertNode == node | insertedFromRange then return

  parent = node~parentNode

  -- is this node one of our children?  Find out its index and see
  -- if this requires a range adjustment
  if parent == startContainer then do
      index = self~indexOf(node, startContainer)
      if index < startOffset then startOffset += 1
  end

  -- ditto for the end container
  if parent == endContainer then do
      index = self~indexOf(node, endContainer)
      if index < endOffset then endOffset += 1
  end


-- handle a child removal for this range.
::method removeChild private
  expose removeChild
  use strict arg parent, child
  removeChild = child
  old = parent~removeChild(child)
  removeChild = .nil
  return old

-- handle a node removal for the range
::method removeNode
  expose startContainer startOffset endContainer endOffset removeChild
  use strict arg node

  -- This is a removal we already know about, so ignore
  if node == .nil | removeChild == node then return

  -- this may change the offsets, so check this out
  parent = node~parentNode
  if parent == startContainer then do
      index = self~indexOf(node, startContainer)
      if index < startOffset then startOffset -= 1
  end

  if parent == endContainer then do
      index = self~indexOf(node, endContainer)
      if index < endOffset then endOffset -= 1
  end

  -- there may be ancestor issues involved here
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

-- traverse the range contents, applying the appropriate operation
::method traverseContents private
  expose startContainer startOffset endContainer endOffset
  use strict arg now

  -- not valid bounds always returns null
  if startContainer == .nil | endContainer = .nil then return .nil

  -- Case 1:  same container
  if startContainer == endContainer then return self~traverseSameContainer(how)

  -- Case 2:  Child C of start container is ancestor of end container.
  -- this can be quickly tested by walking the parent chain of the end
  -- container
  endContainerDepth = 0
  node = endContainer
  parent = node~parentNode
  loop while parent \= .nil
      if parent == startContainer then
          return self~traverseCommonStartContainer(node, how)
      node = parent
      parent = parent~parentNode
      endContainerDepth += 1
  end

  -- case 3: Child C of container B is ancestor of A
  -- This can be quickly tested by walking the parent chain of A
  endContainerDepth = 0
  node = startContainer
  parent = node~parentNode
  loop while parent \= .nil
      if parent == endContainer then
          return self~traverseCommonEndContainer(node, how)
      node = parent
      parent = parent~parentNode
      endContainerDepth += 1
  end

  -- case 4: There is a common ancestor container.  Find the
  -- ancestor siblings that are children of that container.

  depthDiff = startContainerDept - endContainerDepth
  startNode = startContainer

  -- adjust these to be at the same depth
  loop while depthDiff > 0
      startNode = startNode~parentNode
      depthDiff -= 1
  end

  endNode = endContainer

  loop while depthDiff < 0
      endNode = endNode~parentNode
      depthDiff += 1
  end

  sp = startNode~parentNode
  ep = endNode~parentNode

  loop while sp \= ep
      startNode = sp
      endNode = sp

      sp = sp~parentNode
      ep = ep~parentNode
  end

  return self~traverseCommonAncestors(startNode, endNode, how)

-- perform a traversal on the same container level
::method traverseSameContainer private
  expose document startContainer startOffset endContainer endOffset
  use strict arg how

  fragment = .nil

  -- if we're deleting the contents, then we need to create a document fragment
  -- to receive the deleted nodes
  if how \= self~DELETE_CONTENTS then
      fragment = document~createDocumentFragment
  nodeType = startContainer~nodeType
  -- if the starting node is any of the text type nodes, then we need to split the node
  -- and take the trailing section
  if nodeType == .Node~TEXT_NODe | nodeType == .Node~CDATA_SECTION_NODE | nodeType == .Node~COMMENT_NODE | nodeType == .Node~PROCESSING_INSTRUCTION_NODE then do
      -- get the portion of the text from the start offset
      test = startContainer~nodeValue
      sub = s~substr(startOffset + 1, endOffset - startOffset)

      -- if not cloning, then we need to delete the data and
      -- collapse everything
      if how \= self~CLONE_CONTENTS then do
          startContainer~deleteData(startOffset, endOffset - startOffset)
          self~collapse(.true)
      end

      -- no document fragment to return for a deletion
      if how == self~DELETE_CONTENTS then return .nil

      -- create the appropriate node type and attach to the fragment
      if nodeType == .Node~TEXT_NODE then fragment~appendChild(document~createTextNode(sub))
      else if nodeType == .Node~CDATA_SECTION_NODE then fragment~appendChild(document~createCDATASection(sub))
      else if nodeType == .Node~COMMENT_NODE then fragment~appendChild(document~createComment(sub))
      else fragment~appendChild(document~createProcessingInstruction(startContainer~nodeName, sub))

      return fragment
  end

  -- copy nodes between start/end offsets
  node = self~getSelectedNode(startContainer, startOffset)
  count = endOffset - startOffset

  loop count
      sibling = node~nextSibling
      xferNode = self~traverseFullySelected(node, how)
      -- if we're accumulating, add to the fragment
      if fragment \= .nil then fragment~appendChild(xferNode)
      node = sibling
  end

  -- if not just copying, collapse the existing range
  if how \= self~CLONE_CONTENTS then
      self~collapse(.true)
  -- return the fragment with the extracted nodes
  return fragment


-- traverse over a range using a common start container
::method traverseCommonStartContainer private
  expose document startContainer startOffset endContainer endOffset
  use strict arg endAncestor, how

  fragment = .nil
  -- if not deleting, create a document fragment to accumulate
  if how \= self~DELETE_CONTENTS then
      fragment = document~createDocumentFragment

  -- traverse the boundary at the endAncestor side
  node = self~traverseRightBoundary(endAncestor, how)
  if fragment \= .nil then fragment~appendChild(node)

  endIndex = self~indexOf(endAncestor, startContainer)
  count = endIndex - startOffset

  if count <= 0 then do
      -- if not cloning, we're removing these nodes, so collapse things
      if how \= self~CLONE_CONTENTS then do
          self~setEndBefore(endAncestor)
          self~collapse(.false)
      end
      return fragment
  end

  -- run through the sibling list getting the rest of the children
  node = endAncestor~previousSibling
  loop count
      sibling = node~previousSibling
      xferNode = self~traverseFullySelected(node, how)
      if fragment \= .nil then
          fragment~insertBefore(xferNode, fragment~firstChild)
      node = sibling
  end
  -- again, collapse if this was not a cloning operation
  if how \= self~CLONE_CONTENTS then do
      self~setEndBefore(endAncestor)
      self~collapse(.false)
  end
  return fragment

-- traverse where there is a common end container
::method traverseCommonEndContainer private
  expose document startContainer endContainer
  use strict arg startAncestor, how

  -- get an accumulator fragment if needed
  fragment = .nil
  if how == self~DELETE_CONTENTS then
      fragment = document~createDocumentFragment

  -- do the left boundary of the range and add if needed
  node = self~traverseLeftBoundary(endAncestor, how)
  if fragment \= .nil then fragment~appendChild(node)

  -- get the rest of the siblings
  startIndex = self~indexOf(startAncestor, endContainer) + 1
  count = endOffset - startIndex
  node = startAncestor~nextSibling

  loop count
      sibling = node~nextSibling
      xferNode = self~traverseFullySelected(node, how)
      if fragment \= .nil then fragment~appendChild(xferNode)
      node = sibling
  end

  -- collapse if needed
  if how \= self~CLONE_CONTENTS then do
      self~setStartAfter(startAncestor)
      self~collapse(.true)
  end
  return fragment

-- traverse a range where there are common ancestors
::method traverseCommonAncestors private
  expose document startContainer endContainer
  use strict arg startAncestor, endAncestor, how

  fragment = .nil
  if how == self~DELETE_CONTENTS then
      fragment = document~createDocumentFragment

  node = self~traverseLeftBoundary(endAncestor, how)
  if fragment \= .nil then fragment~appendChild(node)

  commonParent = startAncestor~parentNode
  startOffset = self~indexOf(startAncestor, commonParent) + 1
  endOffset = self~indexOf(endAncestor, commonParent)

  count = endOffset - startOffset
  sibling = startAncestor~nextSibling

  loop count
      nextSibling = sibling~nextSibling
      node = self~traverseFullySelected(sibling, now)
      if fragment \= .nil then fragment~appendChild(node)
      sibling = nextSibling
  end

  node = self~traverseRightBoundary(endAncestor, how)
  if fragment \= .nil then fragment~appendChild(node)

  if how \= self~CLONE_CONTENTS then do
      self~setStartAfter(startAncestor)
      self~collapse(.true)
  end
  return fragment

-- traverse the right boundary of the range
::method traverseRightBoundary private
  expose document startContainer startOffset endContainer endOffset
  use strict arg root, how

  next = self~getSelectedNode(endContainer, endOffset - 1)
  isFullySelected = next \= endContainer

  if next == root then
      return self~traverseNode(next, isFullySelected, .false, now)

  parent = next~parentNode
  clonedParent = self~traverseNode(parent, .false, .false, how)

  loop while parent \= .nil
      loop while next \= .nil
          prevSibling = next~previousSibling
          clonedChild = self~traverseNode(next, isFullySelected, .false, how)
          if how \= self~DELETE_CONTENTS then
              clonedParent~insertBefore(clonedChild, clonedParent~firstChild)
          isFullySelected = .true
          next = prevSibling
      end
      if parent == root then return clonedParent

      next = parent~previousSibling
      parent = parent~parentNode
      node clonedGrandParent = self~traverseNode(parent, .false, .false, how)
      if how \= self~DELETE_CONTENTS then
          clonedGrandParent~appendChild(clonedParent)
      clonedParent = clonedGrandParent
  end

  return .nil

-- traverse a single node
::method traverseNode private
  use strict arg node, isFullySelected, isLeft, how

  if isFullySelected then return self~traverseFullySelected(node, how)

  nodeType = node~nodeType
  -- character data nodes get special handling
  if nodeType == .Node~TEXT_NODE | nodeType == .Node~CDATA_SECTION_NODE | -
      nodeType == .Node~COMMENT_NODE | nodeType == .Node~PROCESSING_INSTRUCTION_NODE then
      return self~traverseCharacterDataNode(node, isLeft, how)
  -- handle the partial selection
  return self~traversePartiallySelected(node, how)

-- traverse a single node that is fully selected within the range
::method traverseFullySelected private
  use strict arg node, how

  select
      -- if cloning, do a deep copy of the node
      when how == self~CLONE_CONTENTS then
          return node~cloneNode(.true)
      -- if we're extracting, just return the node as is
      when how == self~EXTRACT_CONTENTS then
          return node
      -- if deleting, remove the node and return .nil
      when how == self~DELETE_CONTENTS then do
          node~parentNode~removeChild(node)
          return .nil
      end
  end

-- handling a partially selected node
::method traversePartiallySelected
  use strict arg node, how

  select
      -- if cloning, this is just a shallow copy
      when how == self~CLONE_CONTENTS then
          return node~cloneNode(.false)
      -- extraction is a shallow copy
      when how == self~EXTRACT_CONTENTS then
          return node~cloneNode(.false)
      -- deleting returns .nil
      when how == self~DELETE_CONTENTS then
          return .nil
  end

-- traverse a character node
::method traverseCharacterDataNode private
  use strict arg node, isLeft, how

  textValue = node~nodeValue
  -- extract the section depending on direction (e.g., whether this
  -- is a start or end node)
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

  -- if not cloning, then update the existing node
  if how \= self~CLONE_CONTENTS then node~nodeValue = oldNodeValue
  -- if deleting, we're done
  if how == self~DELETE_CONTENTS then return .nil
  -- clone the node and return the extracted bit
  newNode = node~cloneNode(.false)
  newNode~nodeValue = newNodeValue
  return newNode

-- check an index value for validity
::method checkIndex private
  use strict arg refNode, offset

  if offset < 0 then .DOMErrors~raiseError(.DomException~INDEX_SIZE_ERR)

  type= refNode~nodeType

  -- if on a text node, this must lie within the length of the character data
  if nodeType == .Node~TEXT_NODE | nodeType == .Node~CDATA_SECTION_NODE | -
      nodeType == .Node~COMMENT_NODE | nodeType == .Node~PROCESSING_INSTRUCTION_NODE then do
      if offset > refNode~nodeValue~length then
          .DOMErrors~raiseError(.DomException~INDEX_SIZE_ERR)
  end
  -- must lie with the range of children for the node
  else if offset > refNode~childNodes~length then
          .DOMErrors~raiseError(.DomException~INDEX_SIZE_ERR)

-- check the start and end positions and determine if the range
-- needs to be collapsed
::method checkCollapse private

  -- if there is no common ancestor or the end precedes the start, then
  -- this is a collapsed container
  if self~commonAncestorContainer == .nil | -
      (startContainer == endContainer & endOffset < startOffset) then self~collapse(.true)

-- determine the relative offset of a node
::method nodeOffset private
  use strict arg node
  -- count the offset of the reference node relative to its siblings
  loop i = 0 while node \= .nil
      node = node~previousSibling
  end
  -- the offset is one less than that
  return i - 1

-- get the root container of the node
::method getRootContainer private
  use strict arg node

  loop while node \= .nil
      node = node~parentNode
  end

  return node

-- test if a node is a legal container for a range
::method isLegalContainer private
  use strict arg node

  if node == .nil then return .false

  -- check the entire ancestor chain for a non-allowed node type
  loop while node \= .nil
      nodeType = node~nodeType
      -- these are invalid node types
      if nodeType == .Node~ENTITY_NODE | nodeType == .Node~NOTATION_NODE | nodeType == .Node~DOCUMENT_TYPE_NODE then
          return .false
      node = node~parentNode
  end

  return .true

-- check if a node has a legal root container
::method hasLegalRootContainer private
  use strict arg node

  if node == .nil then return .false

  rootContainer = self~getRootContainer(node)
  nodeType = rootContainer~nodeType
  -- attributes, documents, and document fragments are the only valid roots.
  if nodeType == .Node~ATTRIBUTE_NODE | nodeType == .Node~DOCUMENT_NODE | nodeType == .Node~DOCUMENT_FRAGMENT_NODE then
      return .true
  return .false

-- check if we have a legal contained node for a start or end point
::method isLegalContainedNode private
  use strict arg node

  if node == .nil then return .false

  -- candidate nodes must be legal document tree nodes
  if nodeType == .Node~ATTRIBUTE_NODE | nodeType == .Node~DOCUMENT_NODE | nodeType == .Node~DOCUMENT_FRAGMENT_NODE | -
          nodeType == .Node~ENTITY_NODE | nodeType == .Node~NOTATION_NODE then
      return .false

  return .true

-- traverse to the next logical node
::method nextNode private
  expose document
  use strict arg node, visitChildren

  if node == .nil then return .nil

  -- if children are an option, then go to the first child if there is one
  if visitChildren then do
      next = node~firstChild
      if next \= .nil then return result
  end


  -- no children or skipping intentionally
  -- try for a sibling
  next = node~nextSibling
  if next \== .nil then return result

  -- go up the parent hierarchy looking for a sibling to a direct
  -- ancestor node
  parent = node~parentNode
  loop while parent \= .nil, parent \= document
      result = parent~nextSibling
      if next \== .nil then return result
      else parent = parent~parentNode
  end
  -- nothing found
  return .nil

-- test if a node is an ancestor of another node
::method isAncestorOf private
  use strict arg a, b

  node = b
  loop while node \= .nil
      if node == a then return .true
      node = node~parentNode
  end

  return .false

-- determine the tree depth of a node
::method depthOf private
  use strict arg node

  loop depthDiff = 0 while node \= .nil
      node = node~parentNode
  end

  return depthDiff

-- find the index offset of a child node
::method indexOf private
  use strict arg child, parent

  if child~parentNode \== parent then return -1

  node = parent~firstChild
  loop i = 0 while node \= child
      node = node~nextSibling
  end

  return i

-- get a selected node
::method getSelectedNode private
  use strict arg container, offset

  -- if this is a text node, just return it directly.  The offset
  -- is within the text of the node
  if container~nodeType == .Node~TEXT_NODE then return container

  -- a negative offset means return the container
  if offset < 0 then return container

  -- skip ahead to the indicated child
  child = container~firstChild
  do offset while child \= .nil
      child = child~nextSibling
  end

  if child \== .nil then return child
  -- can't get the child, return the container
  return container


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: TreeWalkerImpl                                                      */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class "TreeWalkerImpl" inherit TreeWalker
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
  -- convert the whatToShow flags into a string of binary digits that can be
  -- easily tested
  if whatToShow == .NodeFilter~SHOW_ALL then
      whatToShowFlags = "111111111111"    -- we'll set all flags to true
      -- convert this to a binary string, then pad out to 11 digits for the
      -- mapping
  else whatToShowFlags = whatToShow~d2x~x2b~right(12, 0)

-- retrieve the parent node of the
::attribute parentNode GET
  expose currentNode
  if currentNode = .nil then return .nil
  -- if we have a parent, then that becomes the
  -- new current node
  node = currentNode~parentNode
  if node \= .nil then currentNode = node
  return node

-- retrieve the last child of the current node position
::method lastChild
  expose currentNode
  use strict arg

  if currentNode == .nil then return .nil
  -- skip forward to the last child
  node = self~getLastChild(currentNode)
  if node \= .nil then currentNode = node
  return node

-- retrieve the previous sibling of the current node position
::method previousSibling
  expose currentNode
  use strict arg

  -- locate the previous sibling
  node = self~getPreviousSibling(currentNode)
  if node \= .nil then currentNode = node
  return node

-- get the next sibling of the current node
::method nextSibling
  expose currentNode
  use strict arg

  node = self~getNextSibling(currentNode)
  if node \= .nil then currentNode = node
  return node

-- retrieve the previous node in the tree walking order
::method previousNode
  expose currentNode
  use strict arg

  if currentNode == .nil then return .nil

  -- get the previous sibling.  If we don't have one,
  -- then it's time to step up to the parent level
  node = self~getPreviousSibling(currentNode)
  if node == .nil then do
      node = self~getParentNode(currentNode)
      if node \= .nil then currentNode = node
      return node
  end

  -- in traversal order, we would be coming up from the depths,
  -- so first we go the last child of that sibling, and keep
  -- drilling down to the last child of each one in order
  lastChild = self~getLastChild(node)
  previous = lastChild

  loop while lastChild \= .nil
      previous = lastChild
      lastChild = self~getLastChild(previous)
  end

  lastChild = previous

  if lastChild \= . nil then do
      currentNode = lastChild
      return lastChild
  end

  -- didn't find a last child, so we're returning the sibling,
  -- which we know not to be .nil at this point
  currentNode = node
  return currentNode

-- get the next node in the sequence
::method nextNode
  expose currentNode
  use strict arg

  if currentNode == .nil then return .nil

  -- first choice is a child of this node
  node = self~getFirstChild(currentNode)

  if node \= .nil then do
      currentNode = node
      return node
  end
  -- ok, step to the next sibling node
  node = self~getNextSibling(currentNode)

  if node \= .nil then do
      currentNode = node
      return node
  end

  -- ok, up to the parent
  parent = self~getParentNode(currentNode)
  do while parent \= .nil
      node = self~getNextSibling(parent)
      if node \= .nil then do
          currentNode = node
          return node
      end
      -- no siblings, try from its parent
      parent = self~getParentNode(parent)
  end
  -- reached the root...we're finished
  return .nil

-- special method to retrieve a parent node.  This also applies
-- filtering logic to the process
::method getParentNode private
  expose currentNode
  if node == .nil | node == root then return .nil

  newNode = node~parentNode
  if newNode == .nil then return .nil

  if self~acceptNode(newNode) == .NodeFilter~FILTER_ACCEPT then
      return newNode

  -- if skipped or rejected, recursively try for another parent
  return self~getParentNode(newNode)

-- retrieve the next sibling node for the traversal, applying filtering logic
-- to the retrieval
::method getNextSibling private
  expose root
  use strict arg node, startNode = (root)

  if node == .nil | node == startNode then return .nil

  newNode = node~nextSibling
  if newNode == .nil then do
      newNode = node~parentNode
      if newNode = .nil | newNode == startNode then return .nil
      -- if filtered out, then try this recursively
      if self~acceptNode(newNode) == .NodeFilter~FILTER_SKIP then
          return self~getNextSibling(newNode, startNode)
      return .nil
  end

  -- filter the node here
  accept = self~acceptNode(newNode)
  -- accepted, return this
  if accept == .NodeFilter~FILTER_ACCEPT then return newNode
  -- told to skip...try for a child first, and if there
  -- are no children, try for another sibling
  else if accept == .NodeFilter~FILTER_SKIP then do
      child = self~getFirstChild(newNode)
      if child \== .nil then
          return child
      return self~getNextSibling(newNode, startNode)
  end
  -- rejection basically means pretend this doesn't exist
  else return self~getNextSibling(newNode, startNode)

-- retrieve the previous sibling, applying filtering logic
::method getPreviousSibling private
  expose root
  use strict arg node, startNode = (root)

  if node == .nil | node == startNode then return .nil

  newNode = node~previousSibling
  if newNode == .nil then do
      newNode = node~parentNode
      if newNode == .nil | newNode == startNode then return .nil

      if self~acceptNode(newNode) == .NodeFilter~FILTER_SKIP then
          return self~getPreviousSibling(newNode, startNode)
      return .nil
  end

  accept = self~acceptNode(newNode)
  if accept == .NodeFilter~FILTER_ACCEPT then return newNode
  else if accept == .NodeFilter~FILTER_SKIP then do
      child = self~getLastChild(newNode)
      if child == .nil then
          return self~getPreviousSibling(newNode, startNode)
      return child
  end
  else return self~getPreviousSibling(newNode, startNode)

-- get the first child of a node, applying filtering logic
::method getFirstChild private
  expose entityReferenceExpansion
  use strict arg node

  -- if we're on an entity reference and expansion is disabled, there are no children
  if \entityReferenceExpansion & node~nodeType == .Node~ENTITY_REFERENCE_NODE then
      return .nil

  newNode = node~firstChild
  if newNode == .nil then return .nil

  accept = self~acceptNode(newNode)
  if accept == .NodeFilter~FILTER_ACCEPT then return newNode
  -- if skipping, then we drill down to that child node
  else if accept == .NodeFilter~FILTER_SKIP & newNode~hasChildNodes then do
      child = self~getFirstChild(newNode)
      if child == .nil then return self~getNextSibling(newNode, node)
  end
  else return self~getNextSibling(newNode, node)

-- get the last child of a node, applying filtering rules
::method getLastChild private
  expose entityReferenceExpansion
  use strict arg node

  if \entityReferenceExpansion & node~nodeType == .Node~ENTITY_REFERENCE_NODE then
      return .nil

  newNode = nofr~lastChild
  if newNode == .nil then return .nil

  accept = self~acceptNode(newNode)
  if accept == .NodeFilter~FILTER_ACCEPT then return newNode
  else if accept == .NodeFilter~FILTER_SKIP & newNode~hasChildNodes then do
      child = self~getLastChild(newNode)
      if child == .nil then return self~getPreviousSibling(newNode, node)
  end
  else return self~getPreviousSibling(newNode, node)

-- do filtering logic on a node
::method acceptNode private
  expose nodeFilter whatToShowFlags
  use strict arg node

  nodeFlag = (2**(node~nodetype - 1))~d2x~x2b~right(12, 0)
  -- test the flags first...if bad, this is a skip
  if whatToShowFlags~bitxor(nodeflag) \= 0 then
      return .NodeFilter~FILTER_SKIP

  if nodeFilter \== .nil then
      return nodeFilter~accept(node)

  -- passed the flag selection and no filter, this is good
  return .NodeFilter~FILTER_ACCEPT

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: CoreDocument                                                        */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
::class "CoreDocument" subclass DocumentImpl public
::method init
  expose docType docElement version standalone documentURI -
         userdata identifiers changes   -
         documentNumber nodeCounter nodeTable
  use strict arg docType = .nil
  self~init:super(.nil)
  -- we are our own owning document for purposes of child appends
  self~ownerDocument = self

  docElement = .nil
  version = .nil
  standalone = .false
  documentURI = .false
  userData = .nil
  identifiers = .nil
  changes = 0
  documentNumber = 0
  nodeCounter = 0
  nodeTable = .nil

  -- if we have a document type, this gets appended as our first child
  if docType \= .nil then do
      docType~ownerDocument = self
      appendChild(docType)
  end

-- get the node type
::attribute nodeType GET
  use strict arg
  return .Node~DOCUMENT_NODE

-- the node name is a constant for documents
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

-- insert a node into the document
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
  -- we have two special types of children, so cache each
  -- type
  if type == .Node~ELEMENT_NODE then do
      docElement = newChild
  end
  else if type == .Node~DOCUMENT_TYPE_NODE then do
      docType = newChild
  end
  return newChild

-- override for a remove child
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

-- document override for text content.  For this type, it always
-- returns .nil
::attribute textContent GET
  use strict arg
  return .nil

-- override for setting text content.  For this type, it has no effect.
::attribute textContent SET
  use strict arg value
  -- this is a NOP for nodes where this has no meaning.

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
                  if attr~specified | cloningDoc then do
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
          -- we'll do a deep copy
          deep = .true
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
         .DomErrors~raiseError(.DomException~NOT_SUPPORTED_ERR)
      end
      otherwise  do
         .DomErrors~raiseError(.DomException~NOT_SUPPORTED_ERR)
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

-- adopt a node from another document.  This does not copy the
-- tree, but rather removes from its existing owner and
-- inserts into the new document.
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
          .DomErrors~raiseError(.DomException~NO_MODIFICATION_ALLOWED_ERR)
      end
      when type == .Node~DOCUMENT_NODE | type == .Node~DOCUMENT_TYPE_NODE then do
          .DomErrors~raiseError(.DomException~NOT_SUPPORTED_ERR)
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

-- rename a target node using the new namespaceURI and qualified name
::method renameNode
  use strict arg node, namespaceURI, name

  newNode = node
  if node~nodeType == .Node~ELEMENT_NODE then do
      oldName = node~nodeName
      node~rename(namespaceURI, name)
      self~callUserDataHandlers(node, .nil, .UserDataHandler~NODE_RENAMED)
      self~renamedElement(node, oldName)
  end
  else if node~nodeType == .Node~ATTRIBUTE_NODE then do
      oldName = node~nodeName
      -- detach the attr from its owning element
      owner = node~ownerElement
      if owner \= .nil then
          owner~removeAttributeNode(node)
      -- rename and reattach, if we have an owner
      node~rename(namespaceURI, name)
      if owner \= .nil then
          owner~setAttributeNodeNS(node)
      self~callUserDataHandlers(node, .nil, UserDataHandler~NODE_RENAMED)
      -- fire AttributeNameChanged event
      self~renamedAttrNode(node, nodeName)
  end
  else
      .DomErrors~raiseError(.DomException~NOT_SUPPORTED_ERR)

-- clear our identifiers table
::method clearIdentifiers private
  expose identifers
  if identifers \= .nil then
      identifiers~empty

-- retrieve an element by an id attribute
::method getElementById
  use strict arg id

  return self~getIdentifer(id)

-- retrieve an element node by its identifier
::method getIdentifier
  expose identifers
  use strict arg id

  if identifers == .nil then return .nil

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

-- add an element as being identified using an ID attribute name
::method putIdentifier
  expose identifiers
  use strict arg name, element

  -- if no element given, this is a deletion for the name
  if element == .nil then
      self~removeIdentifier(name)
  -- add this to the global identifiers table
  else do
      if identifiers == .nil then
          identifiers = .directory~new
      identifers[name] = element
  end

-- remove a name from the document identifier table
::method removeIdentifier
  expose identifiers
  use strict arg name

  -- no identifiers?  removal is easy
  if identifiers == .nil then
      return
  -- remove from the table...we don't really care if
  -- there's anything there or not.
  identifers~remove(name)

-- retrieve the identifiers name set as an array
::method identifiers
  if identifiers == .nil  then
      identifiers = .directory~new

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

-- call user data handlers when a node is deleted
::method callUserDataHandlers
  expose userData
  use strict arg node, copyNode, operation
  -- if no userdata, then this is easy
  if userData == .nil then return

  -- get the userdata for this node
  userData = node~userDataRecord
  if userData == .nil then return
  if userData~isempty then return

  loop key over userData
      dataRecord = userData[key]
      if dataRecord~handler \== .nil then
          dataRecord~handler~handle(operation, key, dataRecord~data, node, copyNode)
  end


-- associate an object to a key on this node
::method setUserData
  expose userdata
  use strict arg node, key, data, handler

  -- .nil for data means delete this key
  if data == .nil then do
      -- can only delete if we have data
      if userdata \== .nil then do
          -- this is keyed to the node instance
          nodeData = userData[node]
          if nodeData \== .nil then do
              oldData = nodeData~remove(key)
              if oldData \== .nil then return oldData~data
          end
      end
      return .nil
  end
  else do
      if userData == .nil then do
          userData = .IdentityTable~new
          nodeData = .Directory~new
          userData[node] = nodeData
      end
      else do
          nodeData = userData[node]
          if nodeData = .nil then do
              nodeData = .Directory~new
              userData[node] = nodeData
          end
      end
      -- just use a directory object for this
      userRecord = .directory~new
      userRecord~data = data
      userRecord~handler = handler
      oldData = nodeData[key]
      nodeData[key] = userRecord
      if oldData \== .nil then return oldData~data
  end
  return .nil

-- retrieve the object associated to a key on this node
::method getUserData
  expose userData
  use strict arg node, key

  -- no user data registered, so nothing to return
  if userData == .nil then return .nil
  nodeData = userData[node]
  if nodeData == .nil then return .nil

  userRecord = nodeData[key]
  if userRecord \== .nil then return userRecord~data
  return .nil

-- retrieve the userdata table for a given node
::method getUserDataTable
  expose userData

  if userData == .nil then return .nil
  return userData[null]

-- remove a nodes user data table
::method removeUserDataTable
  expose userdata
  use strict arg node

  if userData == .nil then return .nil
  return userData~remove(node)

-- set a user data table for a node.
::method setUserDataTable
  expose userData
  use strict arg node
  if userData == .nil then userData = .IdentityTable~new
  if data \== .nil then userData[node] = data

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
  return currentToken <= queue~items

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
  if \.XMLChar~isNameStart(ch) then do
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

  if \value~datatype('o') then .XPath~error(.Xpath~BOOLEAN_ERROR)
  return value

::method evaluateString
  use strict arg context, container

  value = self~evaluate(context, container)

  if \value~isA(.string) then .XPath~error(.Xpath~STRING_VALUE_ERROR)
  return value

::method evaluateNumber
  use strict arg context, container

  value = self~evaluate(context, container)

  if \value~datatype('Number') then .XPath~error(.Xpath~NUMBER_VALUE_ERROR)
  return value

::method evaluateNodeSet
  use strict arg context, container

  value = self~evaluate(context, container)

  if \value~isA(.NodeSet) then .XPath~error(.Xpath~NODESET_VALUE_ERROR)
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
      self~xpathError(.XPath~INCORRECT_FUNCTION_ARGUMENTS_ERROR);

-- check the minimum number of arguments
::method checkMinArgs
  expose arguments
  use strict arg count

  if count > arguments~items then
      self~xpathError(.XPath~INCORRECT_FUNCTION_ARGUMENTS_ERROR);

-- check the arguments fall in a given range
::method checkMinMaxArgs
  expose arguments
use strict arg min, max

  if arguments~items < min | arguments~items > max then
      self~xpathError(.XPath~INCORRECT_FUNCTION_ARGUMENTS_ERROR);

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
          if term == .nil then self~xpathError(.XPath~INVALID_EXPRESSION_ERROR)
          -- the term is the unary operator
          return .XPathUnaryOperator~new(token, term)
      end
      -- literals are used directly
      when token~type == "LITERAL" then return .XPathLiteral~new(token)
      -- a parenthetical expression
      when token == .XPathToken~open_paren then do
          -- parse the sub expression
          term = self~parseSubexpression(.XPathToken~close_paren)
          if term == .nil then self~xpathError(.XPath~INVALID_EXPRESSION_ERROR)
          -- this term can be used directly
          return term
      end
      -- variable references are not supported.
      when token~type == "VARIABLE" then self~xpathError(.XPath~INVALID_VARIABLE_REFERENCE_ERROR)
      -- go parse the function call
      when token~type == "FUNCTION" then return self~parseFunctionCall(token~name)
      -- variable references are not supported.
      when token~type == "VARIABLE" then self~xpathError(.XPath~INVALID_VARIABLE_REFERENCE_ERROR)
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
      otherwise self~xpathError(.XPath~INVALID_EXPRESSION_ERROR)
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
          if right == .nil then self~xpathError(.XPath~INVALID_EXPRESSION_ERROR)
          self~pushTerm(right)
      end
      -- something we don't recognize
      else self~xpathError(.XPath~INVALID_EXPRESSION_ERROR)
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

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: XMLCHAR                                                             */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

-- a class for identifying valid xml character values
::class xmlchar public mixinclass object

-- complete set of valid characters in 8-bit ascii
::constant valid '090A0D202122232425262728292A2B2C2D2E2F303132333435363738393A3B3C3D3E3F404142434445464748494A4B4C4D4E4F505152535455565758595A5B5C5D5E5F606162636465666768696A6B6C6D6E6F707172737475767778797A7B7C7D7E7F808182838485868788898A8B8C8D8E8F909192939495969798999A9B9C9D9E9FA0A1A2A3A4A5A6A7A8A9AAABACADAEAFB0B1B2B3B4B5B6B7B8B9BABBBCBDBEBFC0C1C2C3C4C5C6C7C8C9CACBCCCDCECFD0D1D2D3D4D5D6D7D8D9DADBDCDDDEDFE0E1E2E3E4E5E6E7E8E9EAEBECEDEEEFF0F1F2F3F4F5F6F7F8F9FAFBFCFDFEFF'x
-- the characters considered space characters
::constant space '090A0D20'x
-- characters valid as the first characters of an XML name
::constant namestart '3A4142434445464748494A4B4C4D4E4F505152535455565758595A5F6162636465666768696A6B6C6D6E6F707172737475767778797AC0C1C2C3C4C5C6C7C8C9CACBCCCDCECFD0D1D2D3D4D5D6D8D9DADBDCDDDEDFE0E1E2E3E4E5E6E7E8E9EAEBECEDEEEFF0F1F2F3F4F5F6F8F9FAFBFCFDFEFF'x
-- characters valid at any position in an XML name
::constant name '2D2E303132333435363738393A4142434445464748494A4B4C4D4E4F505152535455565758595A5F6162636465666768696A6B6C6D6E6F707172737475767778797AB7C0C1C2C3C4C5C6C7C8C9CACBCCCDCECFD0D1D2D3D4D5D6D8D9DADBDCDDDEDFE0E1E2E3E4E5E6E7E8E9EAEBECEDEEEFF0F1F2F3F4F5F6F8F9FAFBFCFDFEFF'x
-- characters valid in a pubid
::constant pubid '0A0D20212324252728292A2B2C2D2E2F303132333435363738393A3B3D3F404142434445464748494A4B4C4D4E4F505152535455565758595A5F6162636465666768696A6B6C6D6E6F707172737475767778797A'x
-- characters valid anywhere in content
::constant content '092021222324252728292A2B2C2D2E2F303132333435363738393A3B3D3E3F404142434445464748494A4B4C4D4E4F505152535455565758595A5B5C5E5F606162636465666768696A6B6C6D6E6F707172737475767778797A7B7C7D7E7F808182838485868788898A8B8C8D8E8F909192939495969798999A9B9C9D9E9FA0A1A2A3A4A5A6A7A8A9AAABACADAEAFB0B1B2B3B4B5B6B7B8B9BABBBCBDBEBFC0C1C2C3C4C5C6C7C8C9CACBCCCDCECFD0D1D2D3D4D5D6D7D8D9DADBDCDDDEDFE0E1E2E3E4E5E6E7E8E9EAEBECEDEEEFF0F1F2F3F4F5F6F7F8F9FAFBFCFDFEFF'x
-- characters valid as first character of an ncname
::constant ncnamestart '4142434445464748494A4B4C4D4E4F505152535455565758595A5F6162636465666768696A6B6C6D6E6F707172737475767778797AC0C1C2C3C4C5C6C7C8C9CACBCCCDCECFD0D1D2D3D4D5D6D8D9DADBDCDDDEDFE0E1E2E3E4E5E6E7E8E9EAEBECEDEEEFF0F1F2F3F4F5F6F8F9FAFBFCFDFEFF'x
-- characters valid anywhere in an ncname
::constant ncname '4142434445464748494A4B4C4D4E4F505152535455565758595A5F6162636465666768696A6B6C6D6E6F707172737475767778797AC0C1C2C3C4C5C6C7C8C9CACBCCCDCECFD0D1D2D3D4D5D6D8D9DADBDCDDDEDFE0E1E2E3E4E5E6E7E8E9EAEBECEDEEEFF0F1F2F3F4F5F6F8F9FAFBFCFDFEFF'x
-- special markup characters
::constant markup "<&%"

-- test if a character is valid in xml encodings
::method isValid class
  use strict arg c
  return c~matchChar(1, self~valid)

-- test if a character is valid in xml content
::method isContent class
  use strict arg c
  return c~matchChar(1, self~content)

-- test if a character is a markup character
::method isMarkup class
  use strict arg c
  return c~matchChar(1, self~markup)

-- test if a character is a space character
::method isSpace class
  use strict arg c
  return c~matchChar(1, self~space)

-- test if a character is valid as the start of a name
::method isNameStart class
  use strict arg c
  return c~matchChar(1, self~namestart)

-- test if a character is valid in a name
::method isName class
  use strict arg c
  return c~matchChar(1, self~name)

-- test if a character is valid as the start of an ncname
::method isNCNameStart class
  use strict arg c
  return c~matchChar(1, self~ncnamestart)

-- test if a character is valid in an ncname
::method isNCName class
  use strict arg c
  return c~matchChar(1, self~ncname)

-- test if a character is valid in a pubid
::method isPubID class
  use strict arg c
  return c~matchChar(1, self~pubid)

-- test if a string is a valid XML name
::method isValidName class
  use strict arg name
  if name = '' then return .false

  if \name~matchchar(1, self~namestart) then return .false

  return name~verify(self~name,,2) \= 0

-- test if a string is a valid XML ncname
::method isValidNCName class
  use strict arg name
  if name = '' then return .false

  if \name~matchchar(1, self~ncnamestart) then return .false

  return name~verify(self~ncname,,2) \= 0

-- test if a string is a valid XML nmtoken
::method isValidNMToken class
  use strict arg name
  if name = '' then return .false

  return name~verify(self~name) \= 0

-- test if a string is a valid XML qname
::method isValidQName class
  use strict arg name
  if name = '' then return .false

  parse var name prefix ':' localName
  return self~isValidNCName(prefix) & self~isValidNCName(localName)

-- strip all XML white space characters from a string.  The source
-- string can be either a string or mutablebuffer.
::method stripWhiteSpace class
  use strict arg source

  -- find the first non-white space character
  firstNonWhite = source~verify(self~space)
  if firstNonWhite == 0 then
      -- NB:  We could just return "", but doing it this way
      -- will also work with mutablebuffers.
      return source~delstr(1)

  -- if there are leading white space characters, delete them now.
  -- again, doing this in two steps will work appropriately with
  -- mutablebuffers too.
  if firstNonWhite > 1 then do
      source = source~delstr(1, firstNonWhite - 1)
  end

  loop i = source~length by -1
      -- find a non-whitespace char?, then delete
      -- tail now.  Note that since we have detected
      -- a non-whitespace char already, we will always
      -- terminate
      if \source~matchChar(i, self~space) then do
          source = source~delstr(i + 1)
          -- done
          return source
      end
  end
