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
/* Author: W. David Ashley                                                    */
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
/* Class: DOMString                                                           */
/*        This class is here as a placeholder only as we do not support       */
/*        unicode string, only ASCII strings. We can modify this class in     */
/*        the future if we ever support unicode strings.                      */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class DOMString subclass string public


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: DOMString                                                           */
/*        Private methods                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: DOMString                                                           */
/*        Public methods                                                      */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: DOMImplementation                                                   */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class DOMImplementation public


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: DOMImplementation                                                   */
/*        Private methods                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::method ver     private


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: DOMImplementation                                                   */
/*        Public methods                                                      */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Method: init                                                               */
/* Description: instance initialization.                                      */
/*----------------------------------------------------------------------------*/

::method init
use strict arg
self~ver = .directory~new
self~ver['1.0'] = .directory~new
return


/*----------------------------------------------------------------------------*/
/* Method: hasFeature                                                         */
/* Description: return boolean indicator for a specified feature.             */
/*----------------------------------------------------------------------------*/

::method hasFeature
use strict arg feature, version = (.nil)
if version = .nil then do featureset over self~ver
   if featureset[feature] <> .nil then return .true
      end
   end
else do
   featureset = self~feature[version]
   if self~featureset[feature] <> .nil then return .true
   end
return .false


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: NodeList                                                            */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class 'NodeList' public
::method init
  expose nodes
  -- we're a read-only list...the owning node returns the list, we work off
  -- of a snapshot copy
  use strict arg nodes
  nodes = nodes~copy

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

::method makearray
  expose nodes
  use strict arg
  return nodes~copy   -- make sure this is a copy

::method "[]"
  return message("ITEM")


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: NodeList                                                            */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


::class DeepNodeList subclass NodeList
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
      // we need to continue the traversal from the last node added
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
              // no more nodes available
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
  item(999999999)
  return nodes~items

::attribute makearray
  expose nodes
  use strict arg
  -- this forces everything to be initialized
  item(999999999)
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
      else if current == rootNode do
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
                     else if nsName == current~namespaceURI
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
                         else if nsName == current~namespaceURI
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

::class 'NamedNodeMap' public
::method init
  expose ownerNode attributes
  use strict arg ownerNode
  attributes = .nil


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: NamedNodeMap                                                        */
/*        Private methods                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

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


::method findNamePointNS private
  expose attributes
  use arg name, namespace

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
  end

  return .nil


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
  use strict arg name, namespace

  if attributes == .nil then do
      return .nil
  end

  do attribute over attributes
      if attribute~name == name & attribute~namespaceURI == namespace then do
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

  index = self~findNamePointNS(node~localName, node~namespaceURI)
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

  index = self~findNamePointNS(name, namespace)
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
          attributes~append(attr~cloneNode(.true)
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

::method makearray
  expose attributes
  use strict arg
  return attributes~makearray -- make sure this is a copy

::method "[]"
  forward message("ITEM")

::attribute attributes PRIVATE



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

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: ChildNode base type for nodes that can be children of other nodes   */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class "ChildNode" subclass Node
::method init
  expose previousSibling nextSibling
  forward class(super) continue

  previousSibling = .nil
  nextSibling = .nil


::method cloneNode
  use strict arg deep = .false

  newNode = self~init:super(deep)
  // detach the new instance from the context
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

::class "ParentNode" extend ChildNode
::method init
  expose firstChild lastChild ownerDocument childNodes
  forward class(super) continue

  -- set this explicitly
  use strict arg ownerDocument
  self~clearChildNodes

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

::method hasChildren
  expose firstChild
  use strict arg
  return firstChild \= .nil

::attribute firstChild GET
::attribute lastChild GET

::method insertBefore
  expose firstChild lastChild ownerDocument childNodes
  use arg newChild, refChild, replace = .false

  -- this case is really a no-op, but we need to go through the steps
  -- in case we need to signal events.
  if newChild == refChild then do
      refChild = refChild~nextSibling
      self~removeChild(newChild)
      self~insertBefore(newChild, refChild)
      return newChild
  end

  -- inform the owner that this is happening
  ownerDocment~insertingNode(this, replace)

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
          newChild~firstSibling = firstChild
          newChild~previousSibling = .nil
          firstChild~previousSibling = newChild
          firstChild = newChild
      end
      -- insertion in the middle
      else do
          previousNode = refNode~previousSibling
          previousNode~nextSibling = newChild
          newChild~previousSibling = previousNode
          newChild~nextSibling = refNode
          refNode~previousSibling = newChild
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
  use strict arg oldChild replace = .false

  ownerDocument~removingNode(self, oldChild, replace)

  -- removing the first child
  if oldChild == firstChild then do
      firstChild = firstChild~nextSibling
      firstChild~previousSibling = .nil
      -- if this was the only child, then clear out everything
      if lastChild == oldChild then do
          lastChild = .nil
      end
  end
  else do
      previous = oldChild~previousSibling
      next = oldChild~nextSibling
      previous~nextSibling = next
      // this could be the last child, so we might have to update that
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

  ownerDocument~removeNode(self, replace)
  return oldChild

::method replaceChild
  expose ownerDocument
  use strict arg newChild, oldChild

  self~insertBefore(newChild, oldChild, .true)
  if newChild !== oldChild then do
      removeChild(oldChild, true)
  end

  ownerDocument.replacedNode(this)
  return oldChild

::method textContent
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
          buildTextContent(buffer)
          return buffer~string
      end
  end
  return ""

::method buildTextContext private
  use arg buffer
  child = self~firstChild
  do while child \= .nil
      if self~hasTextContent(child) then do
          child~buildTextContext(buffer)
      end
      child = child~nextSibling
  end

::method hasTextContext private
  use arg child
  type = child~nodeType
  if type \= .Node~COMMENT_NODE && -
      child \= .Node~PROCESSING_INSTRUCTION_NODE && -
      child \= .Node~TEXT_NODE  -then do
      return .false
  end

  return \child~isIgnorableWhilespace

::attribute textContent SET
  use strict arg text
  child = self~firstChild
  do while child \= .nil
      self~removeChild(child)
  end

  -- create a text node and append
  if text \= .nil, text \== "" then do
      self~appendChild(self~ownerDocument~createTextNode(text)
  end

-- overrides for the NodeList methods
::attribute length GET
  expose childNodes
  use strict arg
  return childNodes

::attribute item
  expose firstNode childNodes
  use strict arg index

  if index < 0 || index >= childNodes then do
      return .nil
  end
  child = firstChild
  do while index > 0
      child = child~nextSibling
      index -= 1
  end

  return child

::method makearray
  expose firstNode childNodes
  use strict arg

  result = .array~new(childNodes)
  child = firstChild
  do while child \= .nil
      result~append(child)
      child = child~nextSibling
  end

  return result

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: AttributeMap -- an implementation of NamedNodeMap that can deal     */
/* with member ownership issues                                               */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class "AttributeMap" subclass NamedNodeMap
::method init
  expose hasDefaults
  use strict arg ownerNode, defaults = . nil

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
  -- replaceing an attribute with itself does nothing
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
  -- replaceing an attribute with itself does nothing
  if attribute~isOwned then do
      return attribute
  end

  attribute~ownerNode = self~ownerNode
  attribute~isOwned = .true

  index = self~findNamePointNS(attribute~namespaceURI, attribute~nodeName)
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

::method removeNamedItem
  use strict arg name

  index = self~findNamePoint(name)
  if index == .nil then do
      return .nil
  end

  return self~remove(self~attributes[index], index, true)

::method removeNamedItemNS
  use strict arg namespaceURI, name

  index = self~findNamePointNS(namspaceURI, name)
  if index == .nil then do
      return .nil
  end

  return self~remove(self~attributes[index], index, true)

::method remove private
  use strict arg attr, index, addDefault = .false

  ownerDocument = self~ownerNode~ownderDocument
  name = attr~nodeName
  if attr~isIdAttribue then do
      ownerDocument~removeIdentifier(attr~value)
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
              newAttr~ownerNode = ownerNode
              newAttr~isOwned = true
              -- mark this as a default value
              newAttr~isSpecified = false
              attributes[index] = newAttr
              setdefault = .true
              -- if this is the id attribute, make sure the document knows
              -- about this mapping
              if attr~isIdAttribute then do
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
  attr~isIdAttribute = false

  -- notify the document
  ownerDocument~removedAttrNode(attr, ownerNode, name)
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
/* Class: Element                                                             */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class "Element" subclass Node public
::method init
  expose nodeName attributes
  use strict arg ownerDoc, nodeName
  self~init:super(ownerDoc)
  attributes = .nil

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
  use strict arg
  return .nil

::attribute typeNamespace GET
  use strict arg
  return .nil

::method isDerivedFrom
  use strict arg nampeNamespace, typeName, deivationMethod

  return .false

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



/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: DocumentFragment                                                    */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class DocumentFragment subclass Node public


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: DocumentFragment                                                    */
/*        Private methods                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: DocumentFragment                                                    */
/*        Public methods                                                      */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Method: init                                                               */
/* Description: instance initialization.                                      */
/*----------------------------------------------------------------------------*/

::method init
if arg() > 0 then raise syntax 93.902 array (0)
self~nodeType = 11
return


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Document                                                            */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class 'Document' subclass Node public


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Document                                                            */
/*        Private methods                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Method: doctype=                                                           */
/* Description: set the document type.                                        */
/*----------------------------------------------------------------------------*/

::attribute doctype GET

/*----------------------------------------------------------------------------*/
/* Method: implementation                                                     */
/* Description: set the document implementation.                              */
/*----------------------------------------------------------------------------*/

::attribute implementation GET

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Document                                                            */
/*        Public methods                                                      */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Method: init                                                               */
/* Description: instance initialization.                                      */
/*----------------------------------------------------------------------------*/

::method init
  expose doctype implementation
  self~nodeType = 9
  doctype = .nil
  implementation = DOMImplementation~new


/*----------------------------------------------------------------------------*/
/* Method: documentElement                                                    */
/* Description: return the root element of the document.                      */
/*----------------------------------------------------------------------------*/

::method documentElement
return self~children[0]


/*----------------------------------------------------------------------------*/
/* Method: createElement                                                      */
/* Description: create an element node.                                      */
/*----------------------------------------------------------------------------*/

::method createElement
  use strict arg tagname
  return .Element~new(tagname)

/*----------------------------------------------------------------------------*/
/* Method: createElementNS                                                    */
/* Description: create an element node.                                      */
/*----------------------------------------------------------------------------*/

::method createElementNS
  use strict arg uri, qname
  -- TODO finish this
  return .Element~new(tagname)


/*----------------------------------------------------------------------------*/
/* Method: createDocumentFragment                                             */
/* Description: create a document fragment node.                              */
/*----------------------------------------------------------------------------*/

::method createDocumentFragment
  use strict arg
  return .DocumentFragment~new


/*----------------------------------------------------------------------------*/
/* Method: createTextNode                                                     */
/* Description: create a text node.                                           */
/*----------------------------------------------------------------------------*/

::method createTextNode
  use strict arg data
  return .Text~new(data)


/*----------------------------------------------------------------------------*/
/* Method: createComment                                                      */
/* Description: create a comment node.                                        */
/*----------------------------------------------------------------------------*/

::method createComment
  use strict arg data
  return .Comment~new(data)


/*----------------------------------------------------------------------------*/
/* Method: createCDATASect                                                    */
/* Description: create a CDATA section node.                                  */
/*----------------------------------------------------------------------------*/

::method createCDATASect
  use strict arg data
  return .CDATASection~new(data)


/*----------------------------------------------------------------------------*/
/* Method: createProcessingInstruction                                        */
/* Description: create a processing instruction node.                         */
/*----------------------------------------------------------------------------*/

::method createProcessingInstruction
  use strict arg target, data
  return .ProcessingInstruction~new(target, data)


/*----------------------------------------------------------------------------*/
/* Method: createAttribute                                                    */
/* Description: create an attribute node.                                     */
/*----------------------------------------------------------------------------*/

::method createAttribute
  use strict arg name
  return .Attr~new(name)


/*----------------------------------------------------------------------------*/
/* Method: createAttributeNS                                                  */
/* Description: create an attribute node.                                     */
/*----------------------------------------------------------------------------*/

::method createAttributeNS
  use strict arg uri, qname
  -- TODO This needs finishing
  return .Attr~new(name)


/*----------------------------------------------------------------------------*/
/* Method: createEntityReference                                              */
/* Description: create an entity reference node.                              */
/*----------------------------------------------------------------------------*/

::method createEntityReference
  use strict arg name
  return .EntityReference~new(name)


/*----------------------------------------------------------------------------*/
/* Method: getElementsByName                                                  */
/* Description: create a list of all element with the specified name.         */
/*----------------------------------------------------------------------------*/

::method getElementsByName
  use arg tagname
  temp = .NodeList~new
  -- TODO: lots to do here, see the spec
  return temp


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Attr                                                                */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class Attr subclass Node public


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Attr                                                                */
/*        Private methods                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Method: specified=                                                         */
/* Description: return the specified indicator.                               */
/*----------------------------------------------------------------------------*/

::method specified
expose specified
use arg specified
return


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Attr                                                                */
/*        Public methods                                                      */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Method: init                                                               */
/* Description: instance initialization.                                      */
/*----------------------------------------------------------------------------*/

::method init
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
use arg name
self~nodeName = name
self~nodeType = 2
self~nodeValue = ''
self~specified = .false
return


/*----------------------------------------------------------------------------*/
/* Method: name                                                               */
/* Description: return the node name.                                         */
/*----------------------------------------------------------------------------*/

::method name
if arg() > 0 then raise syntax 93.902 array (0)
return self~nodeName


/*----------------------------------------------------------------------------*/
/* Method: specified                                                          */
/* Description: return the specified indicator.                               */
/*----------------------------------------------------------------------------*/

::method specified
expose specified
if arg() > 0 then raise syntax 93.902 array (0)
return specified


/*----------------------------------------------------------------------------*/
/* Method: value                                                              */
/* Description: return the node value.                                        */
/*----------------------------------------------------------------------------*/

::method value
if arg() > 0 then raise syntax 93.902 array (0)
return self~nodeValue


/*----------------------------------------------------------------------------*/
/* Method: value=                                                             */
/* Description: set the node value.                                           */
/*----------------------------------------------------------------------------*/

::method 'value='
use arg newval
self~nodeValue = newval
self~specified = .true
return


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Element                                                             */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class 'Element' subclass Node public


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Element                                                             */
/*        Private methods                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Element                                                             */
/*        Public methods                                                      */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Method: init                                                               */
/* Description: instance initialization.                                      */
/*----------------------------------------------------------------------------*/

::method init
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
use arg tagname
self~nodeName = tagname
self~nodeType = 1
self~nodeValue = .nil
self~attr = .NamedNodeList~new
return


/*----------------------------------------------------------------------------*/
/* Method: tagname                                                            */
/* Description: return the node name.                                         */
/*----------------------------------------------------------------------------*/

::method tagname
if arg() > 0 then raise syntax 93.902 array (0)
return self~nodeName


/*----------------------------------------------------------------------------*/
/* Method: getAttribute                                                       */
/* Description: return the specified attribute value.                         */
/*----------------------------------------------------------------------------*/

::method getAttribute
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
use arg name
return self~attributes[name]~nodeValue


/*----------------------------------------------------------------------------*/
/* Method: setAttribute                                                       */
/* Description: set a specified attribute.                                    */
/*----------------------------------------------------------------------------*/

::method setAttribute
if arg() < 2 then raise syntax 93.901 array (2)
if arg() > 2 then raise syntax 93.902 array (2)
use arg name, value
self~attributes[name]~nodeValue = value
return


/*----------------------------------------------------------------------------*/
/* Method: removeAttribute                                                    */
/* Description: remove an attribute value.                                    */
/*----------------------------------------------------------------------------*/

::method removeAttribute
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
use arg name
x = self~attributes[name]~nodeValue = ''
return


/*----------------------------------------------------------------------------*/
/* Method: getAttributeNode                                                   */
/* Description: get an attribute node.                                        */
/*----------------------------------------------------------------------------*/

::method getAttributeNode
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
use arg name
return self~attribute[name]


/*----------------------------------------------------------------------------*/
/* Method: setAttributeNode                                                   */
/* Description: set an attribute node.                                        */
/*----------------------------------------------------------------------------*/

::method setAttributeNode
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
use arg newAttr
self~attributes[newAttr~nodeName] = newAttr
return newAttr


/*----------------------------------------------------------------------------*/
/* Method: removeAttributeNode                                                */
/* Description: set an attribute node.                                        */
/*----------------------------------------------------------------------------*/

::method removeAttributeNode
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
use arg oldAttr
self~attributes~remove(oldAttr~nodeName)
return oldAttr


/*----------------------------------------------------------------------------*/
/* Method: getElementsByTagName                                               */
/* Description: get a list of elements by tag name.                           */
/*----------------------------------------------------------------------------*/

::method getElementsByTagName
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
use arg name
return self~getElementsByName(name)


/*----------------------------------------------------------------------------*/
/* Method: normalize                                                          */
/* Description: normalize the subtree.                                        */
/*----------------------------------------------------------------------------*/

::method normalize
if arg() > 0 then raise syntax 93.902 array (0)
-- TODO: a lot of work to do here, see the spec
return


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Text                                                                */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class Text subclass Node public


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Text                                                                */
/*        Private methods                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Text                                                                */
/*        Public methods                                                      */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Method: init                                                               */
/* Description: instance initialization.                                      */
/*----------------------------------------------------------------------------*/

::method init
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
use arg data
self~nodeName = '#text'
self~nodeType = 3
self~nodeValue = data
return


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Comment                                                             */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class Comment subclass Node public


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Comment                                                             */
/*        Private methods                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Comment                                                             */
/*        Public methods                                                      */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Method: init                                                               */
/* Description: instance initialization.                                      */
/*----------------------------------------------------------------------------*/

::method init
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
use arg data
self~nodeName = '#comment'
self~nodeType = 8
self~nodeValue = data
return


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: CDATASection                                                        */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class CDATASection subclass Node public


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: CDATASection                                                        */
/*        Private methods                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: CDATASection                                                        */
/*        Public methods                                                      */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Method: init                                                               */
/* Description: instance initialization.                                      */
/*----------------------------------------------------------------------------*/

::method init
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
use arg data
self~nodeName = '#cdata-section'
self~nodeType = 4
self~nodeValue = data
return


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: ProcessingInstruction                                               */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class ProcessingInstruction subclass Node public


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: ProcessingInstruction                                               */
/*        Private methods                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: ProcessingInstruction                                               */
/*        Public methods                                                      */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Method: init                                                               */
/* Description: instance initialization.                                      */
/*----------------------------------------------------------------------------*/

::method init
if arg() < 2 then raise syntax 93.901 array (2)
if arg() > 2 then raise syntax 93.902 array (2)
use arg target, data
self~nodeName = target
self~nodeType = 7
self~nodeValue = data
return


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: EntityReference                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class EntityReference subclass Node public


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: EntityReference                                                     */
/*        Private methods                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: EntityReference                                                     */
/*        Public methods                                                      */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Method: init                                                               */
/* Description: instance initialization.                                      */
/*----------------------------------------------------------------------------*/

::method init
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
use arg name
self~nodeName = name
self~nodeType = 5
self~nodeValue = .nil
return


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: CharacterData                                                       */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class CharacterData subclass Node public


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: CharacterData                                                       */
/*        Private methods                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: CharacterData                                                       */
/*        Public methods                                                      */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Method: init                                                               */
/* Description: instance initialization.                                      */
/*----------------------------------------------------------------------------*/

::method init
if arg() > 0 then raise syntax 93.902 array (0)
return


/*----------------------------------------------------------------------------*/
/* Method: data                                                               */
/* Description: return the node value.                                        */
/*----------------------------------------------------------------------------*/

::method data
if arg() > 0 then raise syntax 93.902 array (0)
return self~nodeValue


/*----------------------------------------------------------------------------*/
/* Method: data=                                                              */
/* Description: set the node value.                                           */
/*----------------------------------------------------------------------------*/

::method 'data='
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
use arg data
self~nodeValue = data
return


/*----------------------------------------------------------------------------*/
/* Method: length                                                             */
/* Description: return the node value length.                                 */
/*----------------------------------------------------------------------------*/

::method length
if arg() > 0 then raise syntax 93.902 array (0)
return self~nodeValue~length


/*----------------------------------------------------------------------------*/
/* Method: substringData                                                      */
/* Description: return the node value substr.                                 */
/*----------------------------------------------------------------------------*/

::method substringData
if arg() < 2 then raise syntax 93.901 array (2)
if arg() > 2 then raise syntax 93.902 array (2)
use arg offset, count
return self~nodeValue~substr(offset, count)


/*----------------------------------------------------------------------------*/
/* Method: appendData                                                         */
/* Description: append data to the end of the node value.                     */
/*----------------------------------------------------------------------------*/

::method appendData
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
use arg data
self~nodeValue = self~nodeValue || data
return


/*----------------------------------------------------------------------------*/
/* Method: insertData                                                         */
/* Description: insert data into the node value.                              */
/*----------------------------------------------------------------------------*/

::method insertData
if arg() < 2 then raise syntax 93.901 array (2)
if arg() > 2 then raise syntax 93.902 array (2)
use arg offset, data
self~nodeValue~insert(data, offset - 1)
return


/*----------------------------------------------------------------------------*/
/* Method: deleteData                                                         */
/* Description: delete data from the node value.                              */
/*----------------------------------------------------------------------------*/

::method deleteData
if arg() < 2 then raise syntax 93.901 array (2)
if arg() > 2 then raise syntax 93.902 array (2)
use arg offset, count
x = self~nodeValue~delstr(offset, count)
return


/*----------------------------------------------------------------------------*/
/* Method: replaceData                                                        */
/* Description: replace data in the node value.                               */
/*----------------------------------------------------------------------------*/

::method replaceData
if arg() < 3 then raise syntax 93.901 array (3)
if arg() > 3 then raise syntax 93.902 array (3)
use arg offset, count, data
x = self~nodeValue~delstr(offset, count)
self~nodeValue~insert(data, offset - 1)
return


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Text                                                                */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class Text subclass CharacterData public


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Text                                                                */
/*        Private methods                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Text                                                                */
/*        Public methods                                                      */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Method: init                                                               */
/* Description: instance initialization.                                      */
/*----------------------------------------------------------------------------*/

::method init
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
use arg data
self~nodeName = '#text'
self~nodeType = 3
self~nodeValue = data
return


/*----------------------------------------------------------------------------*/
/* Method: splitText                                                          */
/* Description: split the node into two parts.                                */
/*----------------------------------------------------------------------------*/

::method splitText
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
use arg offset
siblings = self~parentNode~childNodes
newnode = .Text~new(self~nodeValue~substr(offset + 1)
self~nodeValue = self~nodeValue~substr(1, offset)
-- find ourselves in the siblings
do i = 1 to siblings~items
   if self = siblings[i] then leave
   end
siblings~insert(newnode, i)
return newNode


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: DocumentType                                                        */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class DocumentType subclass Node public


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
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
use arg name
self~nodeName = name
self~nodeType = 10
self~nodeValue = .nil
self~entities = .NamedNodeMap~new
self~notations = .NamedNodeMap~new
return


/*----------------------------------------------------------------------------*/
/* Method: name                                                               */
/* Description: return the node name.                                         */
/*----------------------------------------------------------------------------*/

::method name
if arg() > 0 then raise syntax 93.902 array (0)
return self~nodeName


/*----------------------------------------------------------------------------*/
/* Method: entities                                                           */
/* Description: return the list of entities.                                  */
/*----------------------------------------------------------------------------*/

::method entities
expose entities
if arg() > 0 then raise syntax 93.902 array (0)
return entities


/*----------------------------------------------------------------------------*/
/* Method: notations                                                          */
/* Description: return the list of notations                                  */
/*----------------------------------------------------------------------------*/

::method notations
expose notations
if arg() > 0 then raise syntax 93.902 array (0)
return notations


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Notation                                                            */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class Notation subclass Node public


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Notation                                                            */
/*        Private methods                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Notation                                                            */
/*        Public methods                                                      */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Method: init                                                               */
/* Description: instance initialization.                                      */
/*----------------------------------------------------------------------------*/

::method init
expose publicId systemId
if arg() > 2 then raise syntax 93.902 array (2)
if arg() = 1 then do
   use arg publicId
   systemId = .nil
   end
else use arg publicId, systemId
self~nodeType = 12
self~nodeValue = .nil
return


/*----------------------------------------------------------------------------*/
/* Method: publicId                                                           */
/* Description: return the publicId.                                          */
/*----------------------------------------------------------------------------*/

::method publicId
expose publicId
if arg() > 0 then raise syntax 93.902 array (0)
return publicId


/*----------------------------------------------------------------------------*/
/* Method: systemId                                                           */
/* Description: return the systemId.                                          */
/*----------------------------------------------------------------------------*/

::method systemId
expose systemId
if arg() > 0 then raise syntax 93.902 array (0)
return systemId


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Entity                                                              */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class Entity subclass Node public


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Entity                                                              */
/*        Private methods                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Entity                                                              */
/*        Public methods                                                      */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Method: init                                                               */
/* Description: instance initialization.                                      */
/*----------------------------------------------------------------------------*/

::method init
expose NodeName, publicId systemId
if arg() > 3 then raise syntax 93.902 array (3)
if arg() = 1 then do
   use arg nodeName
   publicId = .nil
   systemId = .nil
   end
if arg() = 2 then do
   use arg nodeName, publicId
   systemId = .nil
   end
else use arg nodeName, publicId, systemId
self~nodeType = 6
self~nodeValue = .nil
return


/*----------------------------------------------------------------------------*/
/* Method: publicId                                                           */
/* Description: return the publicId.                                          */
/*----------------------------------------------------------------------------*/

::method publicId
expose publicId
if arg() > 0 then raise syntax 93.902 array (0)
return publicId


/*----------------------------------------------------------------------------*/
/* Method: systemId                                                           */
/* Description: return the systemId.                                          */
/*----------------------------------------------------------------------------*/

::method systemId
expose systemId
if arg() > 0 then raise syntax 93.902 array (0)
return systemId


/*----------------------------------------------------------------------------*/
/* Method: notationName                                                       */
/* Description: return the notation name.                                     */
/*----------------------------------------------------------------------------*/

::method notationName
if arg() > 0 then raise syntax 93.902 array (0)
return self~nodeName


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: EntityReference                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class EntityReference subclass Node public


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: EntityReference                                                     */
/*        Private methods                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: EntityReference                                                     */
/*        Public methods                                                      */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Method: init                                                               */
/* Description: instance initialization.                                      */
/*----------------------------------------------------------------------------*/

::method init
expose nodeName
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
use arg nodeName
self~nodeType = 5
self~nodeValue = .nil
return


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: ProcessingInstruction                                               */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class ProcessingInstruction subclass Node public


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: ProcessingInstruction                                               */
/*        Private methods                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: ProcessingInstruction                                               */
/*        Public methods                                                      */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Method: init                                                               */
/* Description: instance initialization.                                      */
/*----------------------------------------------------------------------------*/

::method init
expose nodeName nodeValue
if arg() < 2 then raise syntax 93.901 array (2)
if arg() > 2 then raise syntax 93.902 array (2)
use arg nodeName, nodeValue
self~nodeType = 7
return


/*----------------------------------------------------------------------------*/
/* Method: target                                                             */
/* Description: return the node name.                                         */
/*----------------------------------------------------------------------------*/

::method target
if arg() > 0 then raise syntax 93.902 array (0)
return self~nodeName


/*----------------------------------------------------------------------------*/
/* Method: data                                                               */
/* Description: return the node value.                                        */
/*----------------------------------------------------------------------------*/

::method data
if arg() > 0 then raise syntax 93.902 array (0)
return self~nodeValue


/*----------------------------------------------------------------------------*/
/* Method: data=                                                              */
/* Description: set the node value.                                           */
/*----------------------------------------------------------------------------*/

::method 'data='
expose nodeValue
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
use arg nodeValue
return


::class DOMErrors
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

