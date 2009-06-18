/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: DeepNodeList                                                        */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


::class DeepNodeList subclass NodeList
::method init
  expose rootNode tagName changes nodes nsName

  use strict arg rootNode, tagName, nsName = .nil
  nodes = .Array~new
  changes = 0

-- lazy search for the matching nodes.  This only fills in the cache
-- as far as the last valid requested position
::method item
  expose rootNode changes nodes
  use strict arg index
  -- if the tree has changed, we need to restart this
  if rootNode~changes \= changes then do
      nodes = .array~new
      changes = rootNode~changes
  end

  currentSize = nodes~items
  if index < currentSize then do
      return nodes[index + 1]
  end
  else do
      // we need to continue the traversal from the last node added
      if currentSize == 0 then do
          thisNode = rootNode
      end
      else do
          thisNode = nodes[currentSize]
      end
      -- keep adding up to the one we're looking for

      do while index > currentSize
          thisNode = self~nexMatchingElementAfter(thisNode)
          if thisNode \= .nil then do
              nodes~append(thisNode)
              currentSize += currentSize
          end
          else do
              // no more nodes available
              leave
          end
      end
      -- this is either the one we want or .nil
      return thisNode
  end

::attribute length GET
  expose nodes
  use strict arg
  -- this forces everything to be initialized
  item(999999999)
  return nodes~items

::attribute makearray
  expose nodes
  use strict arg
  -- this forces everything to be initialized
  item(999999999)
  return nodes~copy

::method nextMatchingElementAfter private
  expose rootNode tagName nsName
  use arg current

  do while current \= nil
      -- go down to the first child if it has one
      if current~hasChildNodes then do
          current = current~firstChild
      end
      -- no children and we're at the rootnode, so we
      -- don't go up or to any siblings
      else if current == rootNode do
          return .nil
      end
      -- now try for a sibling of our current position
      next = current~nextSibling
      -- if we found one, then this is our next candidate
      if next \= .nil  then do
          current = next
      end
      else do
          next = .nil
          -- now we step "up and to the right" for as many attempts as needed
          -- or utnil we pop back to the root node
          do while current \= rootNode
              current = current~parentNode
              next = current~nextSibling
              if next \= .nil then do
                  leave
              end
          end
          -- this is either a good node, or .nil
          current = next
      end
     -- we have a candidate node, now make sure it matches what we're looking for
     if current \= rootNode, current \= .nil, current~nodeType == .Node~ELEMENT_NODE then do
         -- no namespace checking?  We'll take any element node if the name is
         -- "*" or it matches directly
         if nsName == .nil  then do
             if tagName == "*" | current~tagName == tagName then do
                 return current
             end
         end
         -- namespace qualified (which might also be "*")
         else do
             -- wildcard match on the tagname, so just check the namespace name
             if tagName == "*" then do
                 -- wildcards on both, this is easy
                 if nsName == "*" then do
                     return current
                 end
                 else do
                     -- null string is a non-specific namepace request, which
                     -- matches only if the element does not have a namespace
                     if nsName == "" & current~namespaceURI == .nil then do
                         return current
                     end
                     -- requires an exact namespace match
                     else if nsName == current~namespaceURI
                         return current
                     end
                 end
             end
             -- non-wild card on the name
             else do
                 -- if we have a name match, then see if the namespace
                 -- name also matches
                 if current~localName == tagName then do
                     -- ok, the local name matches, the ns rules are the
                     -- same as above
                     if nsName == "*" then do
                         return current
                     end
                     else do
                         -- null string is a non-specific namepace request, which
                         -- matches only if the element does not have a namespace
                         if nsName == "" & current~namespaceURI == .nil then do
                             return current
                         end
                         -- requires an exact namespace match
                         else if nsName == current~namespaceURI
                             return current
                         end
                     end
                 end
             end
         end
     end
     -- if we get here, we had a candidate, but it was not a match
     -- just continue the tree walk
  end
  -- nothing found
  return .nil
