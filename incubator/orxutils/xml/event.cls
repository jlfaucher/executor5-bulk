/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Document                                                            */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
::class DOMEvent
::method init
  expose initialized target eventPhase currentTarget timeStamp stopPropagation preventDefault
  initialized = .false
  currentTarget = .nil
  eventPhase = .nil
  timeStamp = .DateTime~new
  stopPropagation = .false
  preventDevault = .false

::method initEvent
  expose type bubbles cancelable initialized target eventPhase currentTarget timeStamp stopPropagation preventDefault
  use strict arg type, bubbles, cancelable
  initialized = .true

::attribute bubbles GET
::attribute cancelable GET
::attribute currentTarget GET
::attribute eventPhase GET
::attribute target GET
::attribute type GET
::attribute timeStamp GET

::method stopPropagation
  expose stopPropagation
  use strict arg
  stopPropagation = .true

::method preventDefault
  expose preventDefault
  use strict arg
  preventDefault = .true

::class MutationEvent subclass DOMEvent
::constant DOM_SUBTREE_MODIFIED "DOMSubtreeModified"
::constant DOM_NODE_INSERTED "DOMNodeInserted"
::constant DOM_NODE_REMOVED "DOMNodeRemoved"
::constant DOM_NODE_REMOVED_FROM_DOCUMENT "DOMNodeRemovedFromDocument"
::constant DOM_NODE_INSERTED_INTO_DOCUMENT "DOMNodeInsertedIntoDocument"
::constant DOM_ATTR_MODIFIED "DOMAttrModified"
::constant DOM_CHARACTER_DATA_MODIFIED "DOMCharacterDataModified"


::method init
  expose relatedNode prevValue newValue attrName
  self~init:super
  relatedNode = .nil
  prevValue = .nil
  newValue = .nil
  attrName = .nil

::attribute attrName GET
::attribute attrChange GET
::attribute newValue GET
::attribute prevValue GET
::attribute relatedNode GET

::method initMutationEvent
  expose relatedNode prevValue newValue attrName attrChange
  use strict arg type, bubble, cancelable, relatedNode, prevValue, newValue, attrName, attrChange
  self~initEvent(type, canBubble, cancelable)
