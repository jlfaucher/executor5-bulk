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
/* Class: DOMString                                                           */
/*        This class is here as a placeholder only as we do not support       */
/*        unicode string, only ASCII strings. We can modify this class in     */
/*        the future if we ever support unicode strings.                      */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class DOMString subclass string public


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: DOMString                                                           */
/*        Private methods                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: DOMString                                                           */
/*        Public methods                                                      */
/*----------------------------------------------------------------------------*/
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
if arg() > 0 then raise syntax 93.902 array (0)
self~ver = .directory~new
self~ver['1.0'] = .directory~new
return


/*----------------------------------------------------------------------------*/
/* Method: hasFeature                                                         */
/* Description: return boolean indicator for a specified feature.             */
/*----------------------------------------------------------------------------*/

::method hasFeature
if arg() < 2 then raise syntax 93.901 array (2)
if arg() > 2 then raise syntax 93.902 array (2)
use arg feature, version
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

::class NodeList subclass list public


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
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
return self[arg(1)]


/*----------------------------------------------------------------------------*/
/* Method: length                                                             */
/* Description: get the number of items in the list.                          */
/*----------------------------------------------------------------------------*/

::method item
if arg() > 0 then raise syntax 93.902 array (0)
return self~items


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: NamedNodeList                                                       */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class NamedNodeList subclass directory public


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: NamedNodeList                                                       */
/*        Private methods                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: NamedNodeList                                                       */
/*        Public methods                                                      */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* Method: getNamedItem                                                       */
/* Description: get a named item.                                             */
/*----------------------------------------------------------------------------*/

::method getNamedItem
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
use arg name
return self[name]


/*----------------------------------------------------------------------------*/
/* Method: setNamedItem                                                       */
/* Description: set a named item.                                             */
/*----------------------------------------------------------------------------*/

::method setNamedItem
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
use arg node
temp = self[node~nodeName] -- are we replacing an existing node?
self[node~nodeName] = node
return temp -- return either the replaced node or .nil


/*----------------------------------------------------------------------------*/
/* Method: removeNamedItem                                                    */
/* Description: remove a named item.                                          */
/*----------------------------------------------------------------------------*/

::method removeNamedItem
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
use arg name
return self~remove(name)


/*----------------------------------------------------------------------------*/
/* Method: item                                                               */
/* Description: return the named item.                                        */
/*----------------------------------------------------------------------------*/

::method item
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
use arg name
return self[name]


/*----------------------------------------------------------------------------*/
/* Method: length                                                             */
/* Description: return the number of named items.                             */
/*----------------------------------------------------------------------------*/

::method length
if arg() > 0 then raise syntax 93.902 array (0)
return self~items


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Node                                                                */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class Node public


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Node                                                                */
/*        Private methods                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::method ctr      class private
::method id       private
::method children private
::method attr     private


/*----------------------------------------------------------------------------*/
/* Method: nodeType=                                                          */
/* Description: set type of node.                                             */
/*----------------------------------------------------------------------------*/

::method 'nodeType=' private
expose nodeType
use arg nodeType
return


/*----------------------------------------------------------------------------*/
/* Method: nodeName=                                                          */
/* Description: set node name.                                                */
/*----------------------------------------------------------------------------*/

::method 'nodeName=' private
expose nodeName
use arg nodeName
return


/*----------------------------------------------------------------------------*/
/* Method: parentNode                                                         */
/* Description: set the parent node of this node.                             */
/*----------------------------------------------------------------------------*/

::method 'parentNode=' private
expose parentNode
use arg parentNode
return


/*----------------------------------------------------------------------------*/
/* Method: ownerDocument                                                      */
/* Description: set the owner document node of this node.                     */
/*----------------------------------------------------------------------------*/

::method 'ownerDocument=' private
expose ownerDocument
use arg ownseDocument
return


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: Node                                                                */
/*        Public methods                                                      */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::method nodeValue


/*----------------------------------------------------------------------------*/
/* Method: init                                                               */
/* Description: instance initialization.                                      */
/*----------------------------------------------------------------------------*/

::method init
if .node~ctr = 'CTR' then .node~ctr = 0
.node~ctr = .node~ctr + 1
self~id = .node~ctr
self~nodeType = ''
self~nodeName = ''
self~nodeValue = .nil
self~parentNode = .nil
self~children = .NodeList~new
self~attr = .nil
self~ownerDcoument = .nil
return


/*----------------------------------------------------------------------------*/
/* Method: =                                                                  */
/* Description: test for equal.                                               */
/*----------------------------------------------------------------------------*/

::method '='
use arg testnode
if testnode~id = self~id then return .true
return .false


/*----------------------------------------------------------------------------*/
/* Method: nodeType                                                           */
/* Description: get type of node.                                             */
/*----------------------------------------------------------------------------*/

::method nodeType
expose nodeType
if arg() > 0 then raise syntax 93.902 array (0)
return nodeType


/*----------------------------------------------------------------------------*/
/* Method: nodeName                                                           */
/* Description: get node name.                                                */
/*----------------------------------------------------------------------------*/

::method nodeName
expose nodeNaem
if arg() > 0 then raise syntax 93.902 array (0)
return nodeName


/*----------------------------------------------------------------------------*/
/* Method: parentNode                                                         */
/* Description: get parent node of this node.                                 */
/*----------------------------------------------------------------------------*/

::method parentNode
expose parentNode
if arg() > 0 then raise syntax 93.902 array (0)
return parentNode


/*----------------------------------------------------------------------------*/
/* Method: childNode                                                          */
/* Description: get list od child nodes.                                      */
/*----------------------------------------------------------------------------*/

::method childNodes
expose children
if arg() > 0 then raise syntax 93.902 array (0)
return self~children


/*----------------------------------------------------------------------------*/
/* Method: firstChild                                                         */
/* Description: get the first child node.                                     */
/*----------------------------------------------------------------------------*/

::method firstChild
expose children
if arg() > 0 then raise syntax 93.902 array (0)
if children~items = 0 then return .nil
return children[1]


/*----------------------------------------------------------------------------*/
/* Method: lastChild                                                          */
/* Description: get the last child node.                                      */
/*----------------------------------------------------------------------------*/

::method lastChild
expose children
if arg() > 0 then raise syntax 93.902 array (0)
if children~items = 0 then return .nil
return children[children~items]


/*----------------------------------------------------------------------------*/
/* Method: previousSibling                                                    */
/* Description: get our previous sibling.                                     */
/*----------------------------------------------------------------------------*/

::method previousSibling
if arg() > 0 then raise syntax 93.902 array (0)
if self~parentNode = .nil then return .nil
siblings = self~parentNode~childNodes
do i = 1 to siblings~items
   if siblings[1] = self then do
      if i = 1 then return .nil
      else return siblings[i - 1]
      end
   end
end
return .nil


/*----------------------------------------------------------------------------*/
/* Method: nextSibling                                                        */
/* Description: get our next sibling.                                         */
/*----------------------------------------------------------------------------*/

::method nextSibling
if arg() > 0 then raise syntax 93.902 array (0)
if self~parentNode = .nil then return .nil
siblings = self~parentNode~childNodes
do i = 1 to siblings~items
   if siblings[1] = self then do
      if i = siblings~items then return .nil
      else return siblings[i + 1]
      end
   end
end
return .nil


/*----------------------------------------------------------------------------*/
/* Method: attributes                                                         */
/* Description: get our list of attributes.                                   */
/*----------------------------------------------------------------------------*/

::method attributes
if arg() > 0 then raise syntax 93.902 array (0)
return self~attr


/*----------------------------------------------------------------------------*/
/* Method: ownerDocument                                                      */
/* Description: get the owner document node of this node.                     */
/*----------------------------------------------------------------------------*/

::method ownerDocument
expose ownerDocument
if arg() > 0 then raise syntax 93.902 array (0)
return ownerDocument


/*----------------------------------------------------------------------------*/
/* Method: insertBefore                                                       */
/* Description: insert a node in the child list before the specified child.   */
/*----------------------------------------------------------------------------*/

::method insertBefore
expose children
if arg() < 2 then raise syntax 93.901 array (2)
if arg() > 2 then raise syntax 93.902 array (2)
use arg newChild, refChild
if refChild = .nil then do
   children[children~items + 1] = newChild
   return newChild
   end
-- find the reference node
do i = 1 to children~items
   if children[i] = refChild then leave
   end
xx = children~inseret(newChild, i - 1)
newChild~parentNode = self
return newChild


/*----------------------------------------------------------------------------*/
/* Method: replaceChild                                                       */
/* Description: replace a node in the child list.                             */
/*----------------------------------------------------------------------------*/

::method replaceChild
expose children
if arg() < 2 then raise syntax 93.901 array (2)
if arg() > 2 then raise syntax 93.902 array (2)
use arg newChild, oldChild
if oldChild = .nil then return .nil
-- find the reference node
do i = 1 to children~items
   if children[i] = refChild then leave
   end
temp = children[i]
children[i] = newChild
newChild~parentNode = self
temp~parentNode = .nil
return temp


/*----------------------------------------------------------------------------*/
/* Method: removeChild                                                        */
/* Description: removes a node in the child list.                             */
/*----------------------------------------------------------------------------*/

::method removeChild
expose children
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
use arg oldChild
if oldChild = .nil then return .nil
-- find the reference node
do i = 1 to children~items
   if children[i] = refChild then leave
   end
temp = children~remove(i)
temp~parentNode = .nil
return temp


/*----------------------------------------------------------------------------*/
/* Method: appendChild                                                        */
/* Description: appends a node in the child list.                             */
/*----------------------------------------------------------------------------*/

::method appendChild
expose children
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
use arg newChild
if newChild = .nil then return .nil
children[children~items + 1] = newChild
newChild~parentNode = self
return newChild


/*----------------------------------------------------------------------------*/
/* Method: hasChildNodes                                                      */
/* Description: returns .true if this nodes has child nodes.                  */
/*----------------------------------------------------------------------------*/

::method hasChildNodes
expose children
if arg() > 0 then raise syntax 93.902 array (0)
if children~items > 0 then return .true
return .false


/*----------------------------------------------------------------------------*/
/* Method: cloneNode                                                          */
/* Description: returns a copy of this node.                                  */
/*----------------------------------------------------------------------------*/

::method cloneNode
if arg() > 1 then raise syntax 93.902 array (1)
if arg() = 1 then use arg deep
else deep = .false
temp = self~copy
temp~parentNode = .nil
if deep = .true then do
   -- TODO: got a lot of work to do here, see spec
   end
return temp


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

::class Document subclass Node public


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

::method 'doctype='
expose doctype
use arg doctype
return


/*----------------------------------------------------------------------------*/
/* Method: implementation                                                     */
/* Description: set the document implementation.                              */
/*----------------------------------------------------------------------------*/

::method 'implementation='
expose implementation
use arg implementation
return


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
self~nodeType = 9
self~doctype = .nil
self~implementation = DOMImplementation~new
return


/*----------------------------------------------------------------------------*/
/* Method: doctype                                                            */
/* Description: return the document type.                                     */
/*----------------------------------------------------------------------------*/

::method doctype
expose doctype
return doctype


/*----------------------------------------------------------------------------*/
/* Method: implementation                                                     */
/* Description: return the document implementation.                           */
/*----------------------------------------------------------------------------*/

::method implementation
expose implementation
return implementation


/*----------------------------------------------------------------------------*/
/* Method: documentElement                                                    */
/* Description: return the root element of the document.                      */
/*----------------------------------------------------------------------------*/

::method documentElement
return self~children[1]


/*----------------------------------------------------------------------------*/
/* Method: createElement                                                      */
/* Description: create an element node.                                      */
/*----------------------------------------------------------------------------*/

::method createElement
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
use arg tagname
return .Element~new(tagname)


/*----------------------------------------------------------------------------*/
/* Method: createDocumentFragment                                             */
/* Description: create a document fragment node.                              */
/*----------------------------------------------------------------------------*/

::method createDocumentFragment
if arg() > 0 then raise syntax 93.902 array (0)
return .DocumentFragment~new


/*----------------------------------------------------------------------------*/
/* Method: createTextNode                                                     */
/* Description: create a text node.                                           */
/*----------------------------------------------------------------------------*/

::method createTextNode
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
use arg data
return .Text~new(data)


/*----------------------------------------------------------------------------*/
/* Method: createComment                                                      */
/* Description: create a comment node.                                        */
/*----------------------------------------------------------------------------*/

::method createComment
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
use arg data
return .Comment~new(data)


/*----------------------------------------------------------------------------*/
/* Method: createCDATASect                                                    */
/* Description: create a CDATA section node.                                  */
/*----------------------------------------------------------------------------*/

::method createCDATASect
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
use arg data
return .CDATASection~new(data)


/*----------------------------------------------------------------------------*/
/* Method: createProcessingInstruction                                        */
/* Description: create a processing instruction node.                         */
/*----------------------------------------------------------------------------*/

::method createProcessingInstruction
if arg() < 2 then raise syntax 93.901 array (2)
if arg() > 2 then raise syntax 93.902 array (2)
use arg target, data
return .ProcessingInstruction~new(target, data)


/*----------------------------------------------------------------------------*/
/* Method: createAttribute                                                    */
/* Description: create an attribute node.                                     */
/*----------------------------------------------------------------------------*/

::method createAttribute
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
use arg name
return .Attr~new(name)


/*----------------------------------------------------------------------------*/
/* Method: createEntityReference                                              */
/* Description: create an entity reference node.                              */
/*----------------------------------------------------------------------------*/

::method createEntityReference
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
use arg name
return .EntityReference~new(name)


/*----------------------------------------------------------------------------*/
/* Method: getElementsByName                                                  */
/* Description: create a list of all element with the specified name.         */
/*----------------------------------------------------------------------------*/

::method getElementsByName
if arg() < 1 then raise syntax 93.901 array (1)
if arg() > 1 then raise syntax 93.902 array (1)
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

::class Element subclass Node public


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

