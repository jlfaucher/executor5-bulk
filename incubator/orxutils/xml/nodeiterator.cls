/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: NodeIterator                                                        */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
::class "NodeIterator"
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
  use strictArg

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
  use strictArg

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

