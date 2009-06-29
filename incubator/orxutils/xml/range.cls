/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Range                                                               */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
::class "Range"
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

::attribute commonAncestorContainer
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


::method setStartAfter
  expose startContainer startOffset endContainer endOffset
  use strict arg refNode

  endContainer = refNode~parentNode
  i = 0
  node = refNode
  do while node \= .nil
      i += 1
      node = node~previousSibling
  end

  endOffset = i - 1

  -- now collapse this, if necessary
  if self~commonAncestorContainer == .nil | -
      (startContainer == endContainer & endOffset < startOffset) then do
      self~collapse(.false)
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

::insertNode
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
      else if endContainer == parent do
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

  if realStart != realEnd then do
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

::detach
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
  result parent~removeChild(child)
  removeChild = .nil
  return result


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
      endContainerDepth++
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
      endContainerDepth++
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
          return true
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
