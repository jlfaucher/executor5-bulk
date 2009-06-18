/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Text -- a DOM text node                                             */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class "Text" subclass CharacterData

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
      // unwind the entity references as a unit
      if type == .Node~ENTITY_REFERENCE_NODE then do
          if self~getWholeTextForward(node~firstChild, buffer, node) then do
              return .true
          end
      end
      -- a text node, just add the text content to the buffer
      else if type == .Node~TEXT_NODE | type = .Node~CDATA_SECTION_NODE do
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
      // unwind the entity references as a unit
      if type == .Node~ENTITY_REFERENCE_NODE then do
          if self~getWholeTextBackward(node~lastChild, buffer, node) then do
              return .true
          end
      end
      -- a text node, just add the text content to the buffer
      else if type == .Node~TEXT_NODE | type = .Node~CDATA_SECTION_NODE do
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
          nodeType = .Node~ENTITY_REFERENCE_NODE | -
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
          nodeType = .Node~ENTITY_REFERENCE_NODE | -
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
