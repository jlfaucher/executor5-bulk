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
