/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: DocumentFragment                                                    */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
::class 'DocumentFragment' public subclass ParentNode

::attribute nodeType GET
  return .Node~DOCUMENT_FRAGMENT_NODE

::attribute nodeName GET
  return "#document-fragment"

::method normalize
  if self~isNormalized then do
      return
  end

  kid = self~firstChild

  do while kid \= .nil
      next = kid~nextSibling

      if kid~nodeType == .Node~TEXT_NODE then do
          if next \= .nil & next~nodeType == .Node~TEXT_NODE then do
              kid~appendData(next~nodeValue)
              self~removeChild(next)
              -- back up, we might have more nodes to collapse
              next = kid
          end
          else do
              if kid~nodeValue == .nil | kid~nodeValue == "" then do
                  self~removeChild(kid)
              end
          end
      end
      kid~normalize()
      kid = next
  end

  self~isNormalized = .true
