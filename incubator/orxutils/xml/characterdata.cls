/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: CharacterData -- base class for all nodes that carry character data */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class "CharacterData" subclass ChildNode
::method init
  expose data
  use strict arg ownerDocument, data
  self~init:super(ownerDocument)

::attribute childNodes GET
  use strict arg
  -- always returns an empty node list
  return .NodeList~new

::attribute nodeValue GET
  expose data
  use strict arg
  return data

::attribute nodeValue SET
  expose data
  use strict arg value, replace = .false

  oldValue = data
  self~ownerDocument~modifyingCharacterData(self, replace)

  data = value

  self~ownerDocument~modifiedCharacterData(self, oldValue, value, replace)

::attribute data GET
::attribute length GET
  expose data
  use strict arg
  return data~length

::method appendData
  expose data
  use strict arg newData
  if newData == .nil then do
      return
  end

  self~setNodeValue(data||newData)

::method deleteData
  expose data
  use strict arg offset, count, replace = .false

  tailLength = max(data~length - count - offset, 0)
  if offset >= data~length  then do
      newData = data
  end
  else do
      newData = data~delstr(offset + 1, count)
  end
  -- set the node value to the adjusted version
  self~nodeValue = newData

  self~ownerDocument~deletedText(self, offset, count)

::method insertData
  expose data
  use strict arg offset, newData, replace = .false

  if offset >= data~length then do
      newValue = data
  end
  else do
      newValue = date~insert(newData, offset +1)
  end

  self~ownerDocument~deletedText(self, offset, data~length)

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

::method setData
  forward message("NODEVALUE=")

::method substringData
  expose data
  use strict arg offset, count

  return data~substring(offset + 1, count)
