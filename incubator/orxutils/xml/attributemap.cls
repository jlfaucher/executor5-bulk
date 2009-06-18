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
