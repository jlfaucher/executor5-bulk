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
