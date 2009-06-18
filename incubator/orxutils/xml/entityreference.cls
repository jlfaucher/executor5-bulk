::class "EntityReference" subclass ParentNode
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

::attribute entityRefValue
  use strict arg

  value = ""
  firstChild = self~firstChild
  if firstChild == .nil then do
      return .nil
  end
  if firstChild~nodeType == .Node~ENTITY_REFERENCE_NODE then do
      value = firstChild~entityRefValue
  end
  else if firstChild~nodeType == .Node~TEXT_NODE do
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
          else if next~nodeType == .Node~TEXT_NODE do
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
