/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Document                                                            */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
::class "Document" subclass Document
::method init
  expose iterators ranges eventListeners
  use arg doctype = .nil, grammarAccess = .false

  self~init:super(doctype, grammarAccess)
  iterators = .nil
  ranges = .nil
  eventListeners = .nil

::method cloneNode
  use strict arg deep = .false

  newDoc = .Document~new
  self~cloneDocument(newDoc, deep)
  return newDoc

::method implementation
  -- this is a singleton
  return .DOMImplementation~implementation

::method createNodeIterator
  expose iterators
  use strict arg root, whatToShow, filter, entityReferenceExpansion = .true

  iterator = .NodeIterator~new(self, root, whatToShow, filter, entityReferenceExpansion)

  if iterators = .nil  then do
      iterators = .list~new
  end

  iterators~append(iterator)
  return iterator

::method createTreeWalker
  use strict arg root, whatToShow, filter, entityReferenceExpansion = .true
  return .TreeWalker~new(root, whatToShow, filter, entityReferenceExpansion)

::method removeNodeIterator
  expose iterators
  use strict arg nodeIterator

  if nodeIterator == .nil | iterators == .nil then do
      return
  end

  iterators~remove(iterator)

::method createRange
  expose ranges
  if ranges == .nil then do
      ranges = .list~new
  end

  range = .range~new(self)
  ranges~append(range)
  return range

::method removeRange
  expose ranges
  use strict arg range
  if range == .nil | ranges == .nil then do
      return
  end
  ranges~removeItem(range)

::method replacedText
  expose ranges
  use strict arg node

  if ranges \= .nil then do
      do range over ranges
          range~receiveReplacedText(node)
      end
  end

::method deletedText
  expose ranges
  use strict arg node, offset, count

  if ranges \= .nil then do
      do range over ranges
          range~receiveDeletedText(node, offset, count)
      end
  end

::method insertedText
  expose ranges
  use strict arg node, offset, count

  if ranges \= .nil then do
      do range over ranges
          range~receiveInsertedText(node, offset, count)
      end
  end

::method splitData
  expose ranges
  use strict arg node, newNode, offset

  if ranges \= .nil then do
      do range over ranges
          range~receiveSplitData(node, newNode, offset)
      end
  end


::method createEvent
  use strict arg type
  if type~caselessEquals("Events") then do
      return .DOMEvent~new
  end
  else if type~caselessEquals("MutationEvent") then do
      return .MutationEvent~new
  end
  else do
      .DomErrors~raiseError(.DomErrors~NOT_SUPPORTED_ERR)
  end

::attribute mutationEvents

::method setEventListeners
  expose eventListeners
  use strict arg node, listeners

  if eventListeners == .nil then do
      eventListeners = new table
  end

  if listeners == .nil then do
      eventListeners~remove(node)
      if eventListeners~isEmpty then do
          self~mutationEvents = .false
      end
  end
  else do
      eventListeners[node] = listeners
      self~mutationEvents = .true
  end

::method getEventListeners
  expose eventListeners
  use strict arg node
  if eventListeners == .nil then do
      return .nil
  end
  return eventListeners[node]

-- EventTarget support

::method addEventListener
  use strict arg node, type, listener, useCapture

  if type == .nil | type == "" | listener == .nil then do
      return
  end

  self~removeEventListener(node, type, listener, useCapture)

  nodeListeners = self~getEventListeners(node)

  if nodeListeners == .nil then do
      nodeListeners = .List~new
      self~setEventListeners(node, nodeListeners)
  end

  nodeListeners~append(.EventListener~new(type, listener, useCapture))

  capture = self~lookupCapture(type)
  capture~addListener(useCapture)

::method removeEventListener
  use strict arg node, type, listener, useCapture

  if type == .nil | type == "" | listener == .nil then do
      return
  end
  nodeListeners = self~getEventListeners(node)
  if nodeListeners == .nil then do
      return
  end

  do le over nodeListeners
      if le~equals(type, listener, useCapture) then do
          nodeListeners~removeItem(le)
          if nodeListeners~isEmpty then do
              self~eventListeners(node, .nil)
          end
          return
      end
  end

::method copyEventListeners
  use strict arg source, target

  nodeListeners = self~getEventListeners(source)
  if nodeListeners == .nil then do
      return
  end

  self~setEventListeners(target, nodeListeners~copy)




/**
 * Introduced in DOM Level 2. <p>
 * Distribution engine for DOM Level 2 Events.
 * <p>
 * Event propagation runs as follows:
 * <ol>
 * <li>Event is dispatched to a particular target node, which invokes
 *   this code. Note that the event's stopPropagation flag is
 *   cleared when dispatch begins; thereafter, if it has
 *   been set before processing of a node commences, we instead
 *   immediately advance to the DEFAULT phase.
 * <li>The node's ancestors are established as destinations for events.
 *   For capture and bubble purposes, node ancestry is determined at
 *   the time dispatch starts. If an event handler alters the document
 *   tree, that does not change which nodes will be informed of the event.
 * <li>CAPTURING_PHASE: Ancestors are scanned, root to target, for
 *   Capturing listeners. If found, they are invoked (see below).
 * <li>AT_TARGET:
 *   Event is dispatched to NON-CAPTURING listeners on the
 *   target node. Note that capturing listeners on this node are _not_
 *   invoked.
 * <li>BUBBLING_PHASE: Ancestors are scanned, target to root, for
 *   non-capturing listeners.
 * <li>Default processing: Some DOMs have default behaviors bound to
 *   specific nodes. If this DOM does, and if the event's preventDefault
 *   flag has not been set, we now return to the target node and process
 *   its default handler for this event, if any.
 * </ol>
 * <p>
 * Note that registration of handlers during processing of an event does
 * not take effect during this phase of this event; they will not be called
 * until the next time this node is visited by dispatchEvent. On the other
 * hand, removals take effect immediately.
 * <p>
 * If an event handler itself causes events to be dispatched, they are
 * processed synchronously, before processing resumes
 * on the event which triggered them. Please be aware that this may
 * result in events arriving at listeners "out of order" relative
 * to the actual sequence of requests.
 * <p>
 * Note that our implementation resets the event's stop/prevent flags
 * when dispatch begins.
 * I believe the DOM's intent is that event objects be redispatchable,
 * though it isn't stated in those terms.
 * @param node node to dispatch to
 * @param event the event object to be dispatched to
 *              registered EventListeners
 * @return true if the event's <code>preventDefault()</code>
 *              method was invoked by an EventListener; otherwise false.
*/

::method dispatchEvent private
  use strict arg node, event

  -- don't do anything if there are no listeners
  capture = lookupCapture(event~type)
  if capture~total == 0 then do
      return event~preventDefault
  end

  -- initialize the events dispatch status
  event~target = node
  event~stopPropagation = .false
  event~preventDefault = .false

  -- capture the event parentage chain
  parents = .array~new
  previous = node
  parent = previous~parentNode
  while parent \= .nil
      parents~append(parent)
      previous = parent
      parent = parent~parentNode
  end

  -- capturing phase
  if capture~captures > 0 then do
      event~eventPhase = .Event~CAPTURING_PHASE
      do parentNode over parents
          if event~stopPropagation then do
              leave
          end

          event~currentTarget = parentNode
          listeners = self~getEventListeners(parentNode)
          if nodeListeners \= .nil then do
              -- NB:  Do over takes a snapshot copy, so
              -- this is thread safe
              do listener over nodeListeners
                  if listener~matches(event~type, .true) then do
                      listener~invoke(event)
                  end
              end
          end
      end
  end

  -- both AT_TARGET and BUGGLE use non-capturing listeners
  if capture~bubbles > 0 then do
      event~eventPhase = .Event~AT_TARGET
      event~currentTarget = node
      nodeListeners = self~getEventListeners(node)
      if \event~stopPropagation & nodeListeners \= .nil then do
          -- NB:  Do over takes a snapshot copy, so
          -- this is thread safe
          do listener over nodeListeners
              if listener~matches(event~type, .false) then do
                  listener~invoke(event)
              end
          end
      end

      -- BUBBLING_PHASE: Ancestors are scanned, target to root, for
      -- non-capturing listeners. If the event's preventBubbling flag
      -- has been set before processing of a node commences, we
      -- instead immediately advance to the default phase.
      -- Note that not all events bubble.
      if event~bubbles then do
          event~eventPhase = .Event~BUBBLING_PHASE
          do parent over parents
              if event~stopPropagation then do
                  leave
              end

              event~currentTarget = parent
              nodeListeners = self~getEventListeners(node)
              if nodeListeners \= .nil then do
                  do listener over nodeListener
                      if listener~matches(event~type, .false) then do
                          listener~invoke(event)
                      end
                  end
              end
          end
      end
  end

  return event~preventDefault

::method lookupCapture PRIVATE
  expose captures
  use strict arg type
  if caputures == .nil then do
      captures = .directory~new
  end

  capture = captures[type]
  if capture == .nil then do
      capture = .EventTracer~new
      captures[type] = capture
  end

  return capture

::method dispatchEventToSubtree private
  use strict arg node, event
  if node~nodeType == .Node~ELEMENT_NODE then do
      do attr over node~attributes
          self~dispatchinggEventToSubtree(attr, event)
      end
  end
  self~dispatchingEventToSubtree(node~firstChild(, event)

::method dispatchingEventToSubtree
  use strict arg node, event

  if node == .nil then do
      return
  end

  node~dispatchEvent(event)
  if node~nodeType == .Node~ELEMENT_NODE then do
      do attr over node~attributes
          self~dispatchingEventToSubtree(attr, event)
      end
  end

  self~dispatchingEventToSubtree(node~firstChild, event)
  self~dispatchingEventToSubtree(node~nextSibling, event)

::method dispatchAggregateEvents
  use strict arg node, enclosingAttr = .nil, oldvalue = .nil, change = 0

  owner = .nil
  if enclosingAttr \= .nil then do
      capture = self~lookupCapture(.MutationEvent~DOM_ATTR_MODIFIED)
      owner = enclosingAttr~ownerElement
      if capture~total then do
          if owner \= .nil then do
              me = .MutationEvent~new
              me.initMutationEvent(.MutationEvent~DOM_ATTR_MODIFIED, .true. -
                  .false, enclosingAttr, oldvalue, enclosingAttr~nodeValue, -
                  enclosing~nodeName, change)
              owner~dispatchEvent(me)
          end
      end
  end

  capture = self~lookupCapture(.MutationEvent~DOM_SUBTREE_MODIFIED)
  if capture~total > 0 then do
      me = .MutationEvent~new
      me.initMutationEvent(.MutationEvent~DOM_SUBTREE_MODIFIED, .true. -
          .false, .nil, .nil, .nil, .nil, 0)
      owner~dispatchEvent(me)

      if enclosingAttr \= .nil then do
          self~dispatchEvent(enclosingAttr, me)
          if owner \= .nil then do
              self~dispatchEvent(owner, me)
          end
      end
      else do
          self~dispatchEvent(node, me)
      end
  end

::method saveEnclosingAttr
  expose savedEnclosingAttr
  use strict arg node

  savedEnclosingAttr = .nil

  capture = self~captureLookup(.MutationEvent~DOM_ATTR_MODIFIED)
  if capture~total > 0 then do
      eventAncestor = .nil
      do forever
          if eventAncestor == .nil then do
              return
          end
          type = eventAncestor~nodeType
          if type == .Node~ATTRIBUTE_NODe then do
              retval = .EnclosingAttr(eventAncestor, eventAncestor~nodeValue)
              savedEnclosingAttr = retval
              return
          end
          else if type == .Node~ENTITY_REFERENCE_NODE then do
              eventAncestor = eventAncestor~parentNode
          end
          else if type == .Node~TEXT_NODE then do
              eventAncestor = eventAncestor~parentNode
          end
          else do
              return
          end
      end
  end

::method modifyingCharacterData
  expose mutationEvents

  use strict arg node, replace

  if mutationEvents then do
      if \replace then do
          self~saveEnclosingAttr(node)
      end
  end

::method modifiedCharacterData
  expose mutationEvents savedEnclosingAttr

  use strict arg node, oldValue, value, replace

  if mutationEvents then do
      self~mutationEventsModifiedCharacterData(node, oldValue, value, replace)
  end

::method mutationEventsModifiedCharacterData
  use strict arg node, oldValue, value, replace

  if \replace then do
      capture = self~lookupCapture(.MutationEvent.DOM_CHARACTER_DATA_MODIFIED)
      if capture~total > 0 then do
          me = .MutationEvent~new
          me~initMutationEvent(.MutationEvent~DOM_CHARACTER_DATA_MODIFIED, -
              .true, .false, .nil, oldValue, value, .nil, 0)
          self~dispatchEvent(node, me)
      end

      self~dispatchAggregateEvents(node, savedEnclosingAttr~node, oldValue)
  end

::method replaceCharacterData
  use strict arg node, oldvalue, value
  self~modifiedCharacterData(node, oldvalue, value, .false)

::method insertingNode
  expose mutationEvents

  use strict arg node, replace
  if mutationEvents then do
      if \replace then do
          self~saveEnclosingAttr(node)
      end
  end

::method insertedNode
  expose mutationEvents ranges
  use strict arg node, newInternal, replace

  if mutationEvents then do
      self~mutationEventsInsertedNode(node, newInternal, replace)
  end

  if ranges \= .nil then do
      self~notifyRangesInsertedNode(newInternal)
  end

::method mutationEventsInsertedNode private
  expose savedEnclosingAttr
  use strict arg node, newInternal, replace

  capture = self~lookupCapture(.MutationEvent~DOM_NODE_INSERTED)
  if capture~total > 0 then do
      me = .MutationEvent~new
      me~initMutationEvent(.MutationEvent~DOM_NODE_INSERTED, .true. .false, -
          node, .nil, .nil, .nil, 0)
      self~dispatchEvent(newInternal, me)
  end

  capture = self~lookupCapture(.MutationEvent~DOM_NODE_INSERTED_INTO_DOCUMENT)
  if capture~total > 0 then do
      eventAncestor = .node
      if savedEnclosingAttr \= .nil then do
          eventAncestor = savedEnclosingAttr~node~ownerElement
      end
      if eventAncestor \= .nil then do
          parent = eventAncestor
          do while parent \= .nil
              if parent~nodeType == .Node~ATTRIBUTE_NODE then do
                  parent = parent~ownerDocument
              end
              else do
                  parent = parent~parentNode
              end
          end
          if eventAncestor~nodeType == .Node~DOCUMENT_NODE then do
              me = .MutationEvent~new
              me~initMutationEvent(.MutationEvent~DOM_NODE_INSERTED_INTO_DOCUMENT, -
                  .false, .false, .nil, .nil, .nil, .nil, 0)
              self~dispatchEventToSubtree(newInternal, me)
          end
      end
  end

  if \replace then do
      self~dispatchAggregateEvents(node, savedEnclosingAttr~node, saveEnclosingAttr~oldValue)
  end

::method notifyRangesInsertNode
  expose ranges
  use strict arg newInternal

  do range over ranges
      range~insertedNodeFromDOM(newInternal)
  end

::method removingNode
  expose iterators ranges mutationEvents
  use strict arg node, oldChild, replace

  if iterators \= .nil then do
      self~notifyIteratorsRemovingNode(oldChild)
  end

  if ranges \= .nil then do
      self~notifyRangesRemovingNode(oldChild)
  end

  if mutationEvents then do
      self~mutationEventsRemovingNode(node, oldChild, replace)
  end

::method notityIteratorsRemovingNode
  expose iterators
  use strict arg oldChild

  do iterator over iterators
      iterator~removeNode(oldChild)
  end

::method notifyRangesRemovingNode
  expose ranges
  use strict arg oldChild

  do range over ranges
      range~removeNode(oldChild)
  end

::method mutationEventsRemovingNode
  expose savedEnclosingAttr
  use strict arg node, oldChild, replace

  if \replace then do
      self~saveEnclosingAttr(node)
  end

  capture = self~lookupCapture(.MutationEvent~DOM_NODE_REMOVED)
  if capture~total > 0 then do
      me = .MutationEvent~new
      me~initMutationEvent(.MutationEvent~DOM_NODE_REMOVED, .true, .false,
          node, .nil, .nil, .nil, 0)
      self~dispatchEvent(oldChild, me)
  end

  capture~captureLookup(.MutationEvent~DOM_NODE_REMOVED_FROM_DOCUMENT)
  if capture~total then do
      eventAncestor = self
      if savedEnclosingAttr \= .nil then do
          eventAncestor = savedEnclosingAttr~node~ownerElement
      end
      if eventAncestor \= .nil then do
          parent = eventAncestor~parentNode
          do while parent \= .nil
              eventAncestor = parent
              parent = parent~parentNode
          end
      end
      if eventAncestor~nodeType == .Node~DOCUMENT_NODE then do
          me = new .MutationEvent~new
          me~initMutationEvent(.MutationEvent~DOM_NODE_REMOVED_FROM_DOCUMENT,
              .false, .false, .nil, .nil, .nil, .nil, 0)
          self~dispatchEventToSubtree(oldChild, me)
      end
  end

::method removedNode
  expose mutationEvents savedEnclosingAttr
  use strict arg node, replace

  if mutationEvents then do
      if \replace then do
          self~dispatchAggregateEvents(node, savedEnclosingAttr~node, savedEnclosingAttr~oldValue)
      end
  end

::method replacingNode
  expose mutationEvents
  use strict arg node

  if mutationEvents then do
      self~saveEnclosingAttr(node)
  end

::method replacingData
  expose mutationEvents
  use strict arg node
  if mutationEvents then do
      self~saveEnclosingAttr(node)
  end

::method replacedNode
  expose mutationEvents savedEnclosingAttr
  use strict arg node

  if mutationEvents then do
      self~dispatchAggregateEvents(node, savedEnclosingAttr~node, saveEnclosingAttr~oldValue)
  end

::method modifiedAttrValue
  expose mutationEvents
  use strict arg attr, oldValue

  if mutationEvents then do
      self~dispatchAggregateEvents(attr, attr, oldvalue, .MutationEvent~MODIFICATION)
  end

::method setAttrNode
  expose mutationEvents
  use strict arg attr, previous

  if mutationEvents then do
      if previous == .nil then do
          self~dispatchAggregateEvents(attr~ownerNode, attr, .nil, .MutationEvent~ADDITION)
      end
      else do
          self~dispatchAggregateEvents(attr~ownerNode, attr, previous~nodeValue, .MutationEvent~MODIFICATION)
      end
  end

::method removeAttrNode
  expose mutationEvents
  use strict arg attr, oldOwner, name

  if mutationEvents then do
      self~mutationEventRemovedAttrNode(attr, oldOwner, name)
  end

::method mutationEventsRemovedAttrNode
  use strict arg attr, oldOwner, name

  capture = self~lookupCapture(.MutationEvent~DOM_ATTR_MODIFIED)
  if capture~total > 0 then do
      me = .MutationEvent~new
      me~initMutationEvent(.MutationEvent~DOM_ATTR_MODIFIED, .true, .false, attr,
          attr~nodeValue, .nil, name, .MutationEvent~REMOVAL)
      self~dispatchEvent(oldOwner, me)
  end

  self~dispatchAggregateEvents(oldOwner)

::method renamedAttrNode
::method renamedElement

::class EventListener
::method init
  expose type useCapture listener
  use string arg type, listener, useCapture

::method equals
  expose type useCapture listener
  use strict arg eventType, eventlistener, phase

  return phase == useCapture & listener == eventListener & eventType == type

::method handlesEvent
  expose type, useCapture

  use strict arg eventType, phase

  return phase == useCapture & type == eventType

::method invoke
  expose listener
  use strict arg event

  signal on syntax
  listener~handleEvent(event)
  return

syntax:
  return   -- all errors are just ignored


::class EventTracker
::method init
  expose captures total bubble total

::attribute captures GET
::attribute total    GET
::attribute bubbles  GET

::method addListener
  expose captures total bubbles
  use arg useCapture

  total += 1

  if useCapture then do
      captures += 1
  end
  else do
      bubbles += 1
  end

::method removeListener
  expose captures total bubbles
  use arg useCapture

  total -= 1

  if useCapture then do
      captures -= 1
  end
  else do
      bubbles -= 1
  end

::class enclosingAttr
::method init
  expose node oldValue
  use strict arg node, oldValue

::attribute node GET
::attribute oldValue GET
