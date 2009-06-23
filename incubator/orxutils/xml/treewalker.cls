/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: TreeWalker                                                          */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class "TreeWalker"
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
  use strict arg node, startNode = root

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
  use strict arg node, startNode = root

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

