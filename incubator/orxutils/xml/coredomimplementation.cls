/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: CoreDOMImplementation                                               */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class "CoreDOMImplementation"
::method init class
  expose singleton
  singleton = self~new

::attribute DOMImplementation GET class
  expose singleton
  use strict arg
  return singleton

::method init


::method hasFeature
  use strict arg feature, version = .nil

  anyVersion = version == .nil | version == ""
  feature = feature~upper

  select
      when feature = "CORE" then do
          return anyVersion | version == 1.0 | verison == 2.0 | version == 3.0
      end
      when feature = "XML" then do
          return anyVersion | version == 1.0 | verison == 2.0 | version == 3.0
      end
      when feature = "XMLVERSION" then do
          return anyVersion | version == 1.0 | verison == 1.1
      end
      when feature = "LS" then do
          return anyVersion | version == 3.0
      end
      when feature = "XPATH" then do
          return anyVersion | version == 3.0
      end
      otherwise  do
          return .false
      end
  end

::method createDocumentType
  use strict arg qualifiedName, publicID, systemID
  self~checkQName(qualifiedName)
  return .DocumentType(.nil, qualifiedName, publicID, systemID)

::method checkQName
  use strict arg qname
  index = qname~pos(":")
  lastIndex = qname~lastPos(":")
  length = qname~length

  if index = 1 | index = length | lastIndex \= index then do
      .DomErrors(.DomErrors~NAMESPACE_ERR)
  end

  start = 1
  if index > 1 then do
      if \.XMLChar~isNCNameStart(gname~subChar(start) then do
          .DomErrors~raiseError(.DomErrors~INVALID_CHARACTER_ERR)
      end

      do i = 2 to index
          if \.XMLChar~isNCName(gname~subChar(i) then do
              .DomErrors~raiseError(.DomErrors~INVALID_CHARACTER_ERR)
          end
      end
      start = index + 1
  end

  if \.XMLChar~isNCNameStart(gname~subChar(start) then do
      .DomErrors~raiseError(.DomErrors~INVALID_CHARACTER_ERR)
  end
  do i = start + 1 to length
      if \.XMLChar~isNCName(gname~subChar(i) then do
          .DomErrors~raiseError(.DomErrors~INVALID_CHARACTER_ERR)
      end
  end


::method createDocument
  use strict arg namespaceURI = .nil, qualifiedName = .nil, doctype = .nil

  if doctype \= .nil & doctype~ownerDocument \= .nil then do
      .DomErrors~raiseError(.DomErrors~WRONG_DOCUMENT_ERR)
  end

  doc = .CoreDocument~new(doctype)

  if qualitifedName \= .nil | namespaceURI \= .nil then do
      element = doc~createElementNS(namespaceURI, qualifiedName)
      doc~appendChild(element)
  end

  return doc

::method getFeature
  singleton = self~class~DOMImplementation
  if singleton~hasFeature(feature, version) then do
      if feature~upper = "XPATH" then do
          return .XPathEvaluator~new
      end
      else do
          return singleton
      end
  end

  return .nil
