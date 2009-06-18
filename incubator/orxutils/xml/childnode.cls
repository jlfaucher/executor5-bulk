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
