/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Description: An implementation of the Document Object Model in ooRexx.     */
/*                                                                            */
/* Copyright (c) 2006 Rexx Language Association. All rights reserved.         */
/*                                                                            */
/* This program and the accompanying materials are made available under       */
/* the terms of the Common Public License v1.0 which accompanies this         */
/* distribution. A copy is also available at the following address:           */
/* http://www.ibm.com/developerworks/oss/CPLv1.0.htm                          */
/*                                                                            */
/* Redistribution and use in source and binary forms, with or                 */
/* without modification, are permitted provided that the following            */
/* conditions are met:                                                        */
/*                                                                            */
/* Redistributions of source code must retain the above copyright             */
/* notice, this list of conditions and the following disclaimer.              */
/* Redistributions in binary form must reproduce the above copyright          */
/* notice, this list of conditions and the following disclaimer in            */
/* the documentation and/or other materials provided with the distribution.   */
/*                                                                            */
/* Neither the name of Rexx Language Association nor the names                */
/* of its contributors may be used to endorse or promote products             */
/* derived from this software without specific prior written permission.      */
/*                                                                            */
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS        */
/* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT          */
/* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS          */
/* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT   */
/* OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,      */
/* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED   */
/* TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,        */
/* OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY     */
/* OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING    */
/* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS         */
/* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.               */
/*                                                                            */
/* Author: W. David Ashley                                                    */
/*                                                                            */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* NOTE!!!!!!!!!!!                                                            */
/* This class file does not yet work! It is under development!                */
/* It is VERY incomplete!                                                     */
/*                                                                            */
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: DOMImplementation                                                   */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class DOMImplementation public


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: DOMImplementation                                                   */
/*        Private methods                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::method ver     private


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: DOMImplementation                                                   */
/*        Public methods                                                      */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Method: init                                                               */
/* Description: instance initialization.                                      */
/*----------------------------------------------------------------------------*/

::method init
use strict arg
self~ver = .directory~new
self~ver['1.0'] = .directory~new
return


/*----------------------------------------------------------------------------*/
/* Method: hasFeature                                                         */
/* Description: return boolean indicator for a specified feature.             */
/*----------------------------------------------------------------------------*/

::method hasFeature
use strict arg feature, version = (.nil)
if version = .nil then do featureset over self~ver
   if featureset[feature] <> .nil then return .true
      end
   end
else do
   featureset = self~feature[version]
   if self~featureset[feature] <> .nil then return .true
   end
return .false


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: NodeList                                                            */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class 'NodeList' public
::method init
  expose nodes
  -- we're a read-only list...the owning node returns the list, we work off
  -- of a snapshot copy
  use strict arg nodes = .nil
  -- this is an empty node list if no list given
  if nodes == .nil then do
      nodes = .array~new(0)
  end
  else do
      -- make a copy of this
      nodes = nodes~copy
  end

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: NodeList                                                            */
/*        Private methods                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: NodeList                                                            */
/*        Public methods                                                      */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Method: item                                                               */
/* Description: get a list item.                                              */
/*----------------------------------------------------------------------------*/

::method item
  expose nodes
  use strict arg index
  -- nodelists start indexing at 0.  For xpath expresion compatibility, we should
  -- maintain that.
  return nodes[index + 1]   -- returns .nil for out of bounds

/*----------------------------------------------------------------------------*/
/* Method: length                                                             */
/* Description: get the number of items in the list.                          */
/*----------------------------------------------------------------------------*/
::method length
  expose nodes
  use strict arg
  return nodes~items

::method makearray
  expose nodes
  use strict arg
  return nodes~copy   -- make sure this is a copy

::method "[]"
  return message("ITEM")






























/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: DocumentFragment                                                    */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class DocumentFragment subclass Node public


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: DocumentFragment                                                    */
/*        Private methods                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: DocumentFragment                                                    */
/*        Public methods                                                      */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Method: init                                                               */
/* Description: instance initialization.                                      */
/*----------------------------------------------------------------------------*/

::method init
if arg() > 0 then raise syntax 93.902 array (0)
self~nodeType = 11
return


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Document                                                            */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class 'Document' subclass Node public


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Document                                                            */
/*        Private methods                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Method: doctype=                                                           */
/* Description: set the document type.                                        */
/*----------------------------------------------------------------------------*/

::attribute doctype GET

/*----------------------------------------------------------------------------*/
/* Method: implementation                                                     */
/* Description: set the document implementation.                              */
/*----------------------------------------------------------------------------*/

::attribute implementation GET

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Document                                                            */
/*        Public methods                                                      */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Method: init                                                               */
/* Description: instance initialization.                                      */
/*----------------------------------------------------------------------------*/

::method init
  expose doctype implementation
  self~nodeType = 9
  doctype = .nil
  implementation = DOMImplementation~new


/*----------------------------------------------------------------------------*/
/* Method: documentElement                                                    */
/* Description: return the root element of the document.                      */
/*----------------------------------------------------------------------------*/

::method documentElement
return self~children[0]


/*----------------------------------------------------------------------------*/
/* Method: createElement                                                      */
/* Description: create an element node.                                      */
/*----------------------------------------------------------------------------*/

::method createElement
  use strict arg tagname
  return .Element~new(tagname)

/*----------------------------------------------------------------------------*/
/* Method: createElementNS                                                    */
/* Description: create an element node.                                      */
/*----------------------------------------------------------------------------*/

::method createElementNS
  use strict arg uri, qname
  -- TODO finish this
  return .Element~new(tagname)


/*----------------------------------------------------------------------------*/
/* Method: createDocumentFragment                                             */
/* Description: create a document fragment node.                              */
/*----------------------------------------------------------------------------*/

::method createDocumentFragment
  use strict arg
  return .DocumentFragment~new


/*----------------------------------------------------------------------------*/
/* Method: createTextNode                                                     */
/* Description: create a text node.                                           */
/*----------------------------------------------------------------------------*/

::method createTextNode
  use strict arg data
  return .Text~new(data)


/*----------------------------------------------------------------------------*/
/* Method: createComment                                                      */
/* Description: create a comment node.                                        */
/*----------------------------------------------------------------------------*/

::method createComment
  use strict arg data
  return .Comment~new(data)


/*----------------------------------------------------------------------------*/
/* Method: createCDATASect                                                    */
/* Description: create a CDATA section node.                                  */
/*----------------------------------------------------------------------------*/

::method createCDATASect
  use strict arg data
  return .CDATASection~new(data)


/*----------------------------------------------------------------------------*/
/* Method: createProcessingInstruction                                        */
/* Description: create a processing instruction node.                         */
/*----------------------------------------------------------------------------*/

::method createProcessingInstruction
  use strict arg target, data
  return .ProcessingInstruction~new(target, data)


/*----------------------------------------------------------------------------*/
/* Method: createAttribute                                                    */
/* Description: create an attribute node.                                     */
/*----------------------------------------------------------------------------*/

::method createAttribute
  use strict arg name
  return .Attr~new(name)


/*----------------------------------------------------------------------------*/
/* Method: createAttributeNS                                                  */
/* Description: create an attribute node.                                     */
/*----------------------------------------------------------------------------*/

::method createAttributeNS
  use strict arg uri, qname
  -- TODO This needs finishing
  return .Attr~new(name)


/*----------------------------------------------------------------------------*/
/* Method: createEntityReference                                              */
/* Description: create an entity reference node.                              */
/*----------------------------------------------------------------------------*/

::method createEntityReference
  use strict arg name
  return .EntityReference~new(name)


/*----------------------------------------------------------------------------*/
/* Method: getElementsByName                                                  */
/* Description: create a list of all element with the specified name.         */
/*----------------------------------------------------------------------------*/

::method getElementsByName
  use arg tagname
  temp = .NodeList~new
  -- TODO: lots to do here, see the spec
  return temp


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Attr                                                                */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class Attr subclass Node public


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Attr                                                                */
/*        Private methods                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Method: specified=                                                         */
/* Description: return the specified indicator.                               */
/*----------------------------------------------------------------------------*/

::method specified
expose specified
use arg specified
return


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Attr                                                                */
/*        Public methods                                                      */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Method: init                                                               */
/* Description: instance initialization.                                      */
/*----------------------------------------------------------------------------*/

::method init
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
use arg name
self~nodeName = name
self~nodeType = 2
self~nodeValue = ''
self~specified = .false
return


/*----------------------------------------------------------------------------*/
/* Method: name                                                               */
/* Description: return the node name.                                         */
/*----------------------------------------------------------------------------*/

::method name
if arg() > 0 then raise syntax 93.902 array (0)
return self~nodeName


/*----------------------------------------------------------------------------*/
/* Method: specified                                                          */
/* Description: return the specified indicator.                               */
/*----------------------------------------------------------------------------*/

::method specified
expose specified
if arg() > 0 then raise syntax 93.902 array (0)
return specified


/*----------------------------------------------------------------------------*/
/* Method: value                                                              */
/* Description: return the node value.                                        */
/*----------------------------------------------------------------------------*/

::method value
if arg() > 0 then raise syntax 93.902 array (0)
return self~nodeValue


/*----------------------------------------------------------------------------*/
/* Method: value=                                                             */
/* Description: set the node value.                                           */
/*----------------------------------------------------------------------------*/

::method 'value='
use arg newval
self~nodeValue = newval
self~specified = .true
return


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Element                                                             */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class 'Element' subclass Node public


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Element                                                             */
/*        Private methods                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Element                                                             */
/*        Public methods                                                      */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Method: init                                                               */
/* Description: instance initialization.                                      */
/*----------------------------------------------------------------------------*/

::method init
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
use arg tagname
self~nodeName = tagname
self~nodeType = 1
self~nodeValue = .nil
self~attr = .NamedNodeList~new
return


/*----------------------------------------------------------------------------*/
/* Method: tagname                                                            */
/* Description: return the node name.                                         */
/*----------------------------------------------------------------------------*/

::method tagname
if arg() > 0 then raise syntax 93.902 array (0)
return self~nodeName


/*----------------------------------------------------------------------------*/
/* Method: getAttribute                                                       */
/* Description: return the specified attribute value.                         */
/*----------------------------------------------------------------------------*/

::method getAttribute
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
use arg name
return self~attributes[name]~nodeValue


/*----------------------------------------------------------------------------*/
/* Method: setAttribute                                                       */
/* Description: set a specified attribute.                                    */
/*----------------------------------------------------------------------------*/

::method setAttribute
if arg() < 2 then raise syntax 93.901 array (2)
if arg() > 2 then raise syntax 93.902 array (2)
use arg name, value
self~attributes[name]~nodeValue = value
return


/*----------------------------------------------------------------------------*/
/* Method: removeAttribute                                                    */
/* Description: remove an attribute value.                                    */
/*----------------------------------------------------------------------------*/

::method removeAttribute
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
use arg name
x = self~attributes[name]~nodeValue = ''
return


/*----------------------------------------------------------------------------*/
/* Method: getAttributeNode                                                   */
/* Description: get an attribute node.                                        */
/*----------------------------------------------------------------------------*/

::method getAttributeNode
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
use arg name
return self~attribute[name]


/*----------------------------------------------------------------------------*/
/* Method: setAttributeNode                                                   */
/* Description: set an attribute node.                                        */
/*----------------------------------------------------------------------------*/

::method setAttributeNode
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
use arg newAttr
self~attributes[newAttr~nodeName] = newAttr
return newAttr


/*----------------------------------------------------------------------------*/
/* Method: removeAttributeNode                                                */
/* Description: set an attribute node.                                        */
/*----------------------------------------------------------------------------*/

::method removeAttributeNode
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
use arg oldAttr
self~attributes~remove(oldAttr~nodeName)
return oldAttr


/*----------------------------------------------------------------------------*/
/* Method: getElementsByTagName                                               */
/* Description: get a list of elements by tag name.                           */
/*----------------------------------------------------------------------------*/

::method getElementsByTagName
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
use arg name
return self~getElementsByName(name)


/*----------------------------------------------------------------------------*/
/* Method: normalize                                                          */
/* Description: normalize the subtree.                                        */
/*----------------------------------------------------------------------------*/

::method normalize
if arg() > 0 then raise syntax 93.902 array (0)
-- TODO: a lot of work to do here, see the spec
return


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Text                                                                */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class Text subclass Node public


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Text                                                                */
/*        Private methods                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Text                                                                */
/*        Public methods                                                      */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Method: init                                                               */
/* Description: instance initialization.                                      */
/*----------------------------------------------------------------------------*/

::method init
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
use arg data
self~nodeName = '#text'
self~nodeType = 3
self~nodeValue = data
return


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Comment                                                             */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class Comment subclass Node public


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Comment                                                             */
/*        Private methods                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Comment                                                             */
/*        Public methods                                                      */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Method: init                                                               */
/* Description: instance initialization.                                      */
/*----------------------------------------------------------------------------*/

::method init
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
use arg data
self~nodeName = '#comment'
self~nodeType = 8
self~nodeValue = data
return


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: CDATASection                                                        */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class CDATASection subclass Node public


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: CDATASection                                                        */
/*        Private methods                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: CDATASection                                                        */
/*        Public methods                                                      */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Method: init                                                               */
/* Description: instance initialization.                                      */
/*----------------------------------------------------------------------------*/

::method init
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
use arg data
self~nodeName = '#cdata-section'
self~nodeType = 4
self~nodeValue = data
return


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: ProcessingInstruction                                               */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class ProcessingInstruction subclass Node public


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: ProcessingInstruction                                               */
/*        Private methods                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: ProcessingInstruction                                               */
/*        Public methods                                                      */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Method: init                                                               */
/* Description: instance initialization.                                      */
/*----------------------------------------------------------------------------*/

::method init
if arg() < 2 then raise syntax 93.901 array (2)
if arg() > 2 then raise syntax 93.902 array (2)
use arg target, data
self~nodeName = target
self~nodeType = 7
self~nodeValue = data
return


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: EntityReference                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class EntityReference subclass Node public


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: EntityReference                                                     */
/*        Private methods                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: EntityReference                                                     */
/*        Public methods                                                      */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Method: init                                                               */
/* Description: instance initialization.                                      */
/*----------------------------------------------------------------------------*/

::method init
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
use arg name
self~nodeName = name
self~nodeType = 5
self~nodeValue = .nil
return


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: CharacterData                                                       */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class CharacterData subclass Node public


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: CharacterData                                                       */
/*        Private methods                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: CharacterData                                                       */
/*        Public methods                                                      */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Method: init                                                               */
/* Description: instance initialization.                                      */
/*----------------------------------------------------------------------------*/

::method init
if arg() > 0 then raise syntax 93.902 array (0)
return


/*----------------------------------------------------------------------------*/
/* Method: data                                                               */
/* Description: return the node value.                                        */
/*----------------------------------------------------------------------------*/

::method data
if arg() > 0 then raise syntax 93.902 array (0)
return self~nodeValue


/*----------------------------------------------------------------------------*/
/* Method: data=                                                              */
/* Description: set the node value.                                           */
/*----------------------------------------------------------------------------*/

::method 'data='
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
use arg data
self~nodeValue = data
return


/*----------------------------------------------------------------------------*/
/* Method: length                                                             */
/* Description: return the node value length.                                 */
/*----------------------------------------------------------------------------*/

::method length
if arg() > 0 then raise syntax 93.902 array (0)
return self~nodeValue~length


/*----------------------------------------------------------------------------*/
/* Method: substringData                                                      */
/* Description: return the node value substr.                                 */
/*----------------------------------------------------------------------------*/

::method substringData
if arg() < 2 then raise syntax 93.901 array (2)
if arg() > 2 then raise syntax 93.902 array (2)
use arg offset, count
return self~nodeValue~substr(offset, count)


/*----------------------------------------------------------------------------*/
/* Method: appendData                                                         */
/* Description: append data to the end of the node value.                     */
/*----------------------------------------------------------------------------*/

::method appendData
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
use arg data
self~nodeValue = self~nodeValue || data
return


/*----------------------------------------------------------------------------*/
/* Method: insertData                                                         */
/* Description: insert data into the node value.                              */
/*----------------------------------------------------------------------------*/

::method insertData
if arg() < 2 then raise syntax 93.901 array (2)
if arg() > 2 then raise syntax 93.902 array (2)
use arg offset, data
self~nodeValue~insert(data, offset - 1)
return


/*----------------------------------------------------------------------------*/
/* Method: deleteData                                                         */
/* Description: delete data from the node value.                              */
/*----------------------------------------------------------------------------*/

::method deleteData
if arg() < 2 then raise syntax 93.901 array (2)
if arg() > 2 then raise syntax 93.902 array (2)
use arg offset, count
x = self~nodeValue~delstr(offset, count)
return


/*----------------------------------------------------------------------------*/
/* Method: replaceData                                                        */
/* Description: replace data in the node value.                               */
/*----------------------------------------------------------------------------*/

::method replaceData
if arg() < 3 then raise syntax 93.901 array (3)
if arg() > 3 then raise syntax 93.902 array (3)
use arg offset, count, data
x = self~nodeValue~delstr(offset, count)
self~nodeValue~insert(data, offset - 1)
return


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Text                                                                */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class Text subclass CharacterData public


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Text                                                                */
/*        Private methods                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Text                                                                */
/*        Public methods                                                      */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Method: init                                                               */
/* Description: instance initialization.                                      */
/*----------------------------------------------------------------------------*/

::method init
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
use arg data
self~nodeName = '#text'
self~nodeType = 3
self~nodeValue = data
return


/*----------------------------------------------------------------------------*/
/* Method: splitText                                                          */
/* Description: split the node into two parts.                                */
/*----------------------------------------------------------------------------*/

::method splitText
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
use arg offset
siblings = self~parentNode~childNodes
newnode = .Text~new(self~nodeValue~substr(offset + 1)
self~nodeValue = self~nodeValue~substr(1, offset)
-- find ourselves in the siblings
do i = 1 to siblings~items
   if self = siblings[i] then leave
   end
siblings~insert(newnode, i)
return newNode


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: DocumentType                                                        */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class DocumentType subclass Node public


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: DocumentType                                                        */
/*        Private methods                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: DocumentType                                                        */
/*        Public methods                                                      */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Method: init                                                               */
/* Description: instance initialization.                                      */
/*----------------------------------------------------------------------------*/

::method init
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
use arg name
self~nodeName = name
self~nodeType = 10
self~nodeValue = .nil
self~entities = .NamedNodeMap~new
self~notations = .NamedNodeMap~new
return


/*----------------------------------------------------------------------------*/
/* Method: name                                                               */
/* Description: return the node name.                                         */
/*----------------------------------------------------------------------------*/

::method name
if arg() > 0 then raise syntax 93.902 array (0)
return self~nodeName


/*----------------------------------------------------------------------------*/
/* Method: entities                                                           */
/* Description: return the list of entities.                                  */
/*----------------------------------------------------------------------------*/

::method entities
expose entities
if arg() > 0 then raise syntax 93.902 array (0)
return entities


/*----------------------------------------------------------------------------*/
/* Method: notations                                                          */
/* Description: return the list of notations                                  */
/*----------------------------------------------------------------------------*/

::method notations
expose notations
if arg() > 0 then raise syntax 93.902 array (0)
return notations


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Notation                                                            */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class Notation subclass Node public


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Notation                                                            */
/*        Private methods                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Notation                                                            */
/*        Public methods                                                      */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Method: init                                                               */
/* Description: instance initialization.                                      */
/*----------------------------------------------------------------------------*/

::method init
expose publicId systemId
if arg() > 2 then raise syntax 93.902 array (2)
if arg() = 1 then do
   use arg publicId
   systemId = .nil
   end
else use arg publicId, systemId
self~nodeType = 12
self~nodeValue = .nil
return


/*----------------------------------------------------------------------------*/
/* Method: publicId                                                           */
/* Description: return the publicId.                                          */
/*----------------------------------------------------------------------------*/

::method publicId
expose publicId
if arg() > 0 then raise syntax 93.902 array (0)
return publicId


/*----------------------------------------------------------------------------*/
/* Method: systemId                                                           */
/* Description: return the systemId.                                          */
/*----------------------------------------------------------------------------*/

::method systemId
expose systemId
if arg() > 0 then raise syntax 93.902 array (0)
return systemId


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Entity                                                              */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class Entity subclass Node public


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Entity                                                              */
/*        Private methods                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Entity                                                              */
/*        Public methods                                                      */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Method: init                                                               */
/* Description: instance initialization.                                      */
/*----------------------------------------------------------------------------*/

::method init
expose NodeName, publicId systemId
if arg() > 3 then raise syntax 93.902 array (3)
if arg() = 1 then do
   use arg nodeName
   publicId = .nil
   systemId = .nil
   end
if arg() = 2 then do
   use arg nodeName, publicId
   systemId = .nil
   end
else use arg nodeName, publicId, systemId
self~nodeType = 6
self~nodeValue = .nil
return


/*----------------------------------------------------------------------------*/
/* Method: publicId                                                           */
/* Description: return the publicId.                                          */
/*----------------------------------------------------------------------------*/

::method publicId
expose publicId
if arg() > 0 then raise syntax 93.902 array (0)
return publicId


/*----------------------------------------------------------------------------*/
/* Method: systemId                                                           */
/* Description: return the systemId.                                          */
/*----------------------------------------------------------------------------*/

::method systemId
expose systemId
if arg() > 0 then raise syntax 93.902 array (0)
return systemId


/*----------------------------------------------------------------------------*/
/* Method: notationName                                                       */
/* Description: return the notation name.                                     */
/*----------------------------------------------------------------------------*/

::method notationName
if arg() > 0 then raise syntax 93.902 array (0)
return self~nodeName


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: EntityReference                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class EntityReference subclass Node public


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: EntityReference                                                     */
/*        Private methods                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: EntityReference                                                     */
/*        Public methods                                                      */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Method: init                                                               */
/* Description: instance initialization.                                      */
/*----------------------------------------------------------------------------*/

::method init
expose nodeName
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
use arg nodeName
self~nodeType = 5
self~nodeValue = .nil
return


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: ProcessingInstruction                                               */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class ProcessingInstruction subclass Node public


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: ProcessingInstruction                                               */
/*        Private methods                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: ProcessingInstruction                                               */
/*        Public methods                                                      */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Method: init                                                               */
/* Description: instance initialization.                                      */
/*----------------------------------------------------------------------------*/

::method init
expose nodeName nodeValue
if arg() < 2 then raise syntax 93.901 array (2)
if arg() > 2 then raise syntax 93.902 array (2)
use arg nodeName, nodeValue
self~nodeType = 7
return


/*----------------------------------------------------------------------------*/
/* Method: target                                                             */
/* Description: return the node name.                                         */
/*----------------------------------------------------------------------------*/

::method target
if arg() > 0 then raise syntax 93.902 array (0)
return self~nodeName


/*----------------------------------------------------------------------------*/
/* Method: data                                                               */
/* Description: return the node value.                                        */
/*----------------------------------------------------------------------------*/

::method data
if arg() > 0 then raise syntax 93.902 array (0)
return self~nodeValue


/*----------------------------------------------------------------------------*/
/* Method: data=                                                              */
/* Description: set the node value.                                           */
/*----------------------------------------------------------------------------*/

::method 'data='
expose nodeValue
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
use arg nodeValue
return



