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

::method buildTextContent private
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
