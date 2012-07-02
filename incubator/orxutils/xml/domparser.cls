/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Description: DOM Parser for XML                                            */
/*                                                                            */
/* Copyright (c) 2012 Rexx Language Association. All rights reserved.         */
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
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: XMLPARSER                                                           */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


::class "DomParser" public inherit xmlcontenthandler
::method init
  expose parser
  -- create a parser instance with this parser as the content handler
  parser = .xmlparser~new(self)

-- the document currently being constructed
::attribute doc


/*----------------------------------------------------------------------------*/
/* Method: parse_array                                                        */
/* Description: parse the specified array of XML code.                        */
/*----------------------------------------------------------------------------*/

::method parse_array
  expose doc parser

  -- have the parser process this
  forward to(parser) continue
  -- return the doc instance.  If this failed, then the
  -- error can be retrieved after the fact
  return doc


/*----------------------------------------------------------------------------*/
/* Method: parse_file                                                         */
/* Description: parse the specified file of XML code.                         */
/*----------------------------------------------------------------------------*/

::method parse_file
  expose doc parser

  -- have the parser process this
  forward to(parser) continue
  -- return the doc instance.  If this failed, then the
  -- error can be retrieved after the fact
  return doc

/*----------------------------------------------------------------------------*/
/* Method: parse_stream                                                       */
/* Description: parse the specified stream of XML code                        */
/*              The stream should be opened, and will not be closed at the end*/
/*----------------------------------------------------------------------------*/

::method parse_stream
  expose doc parser

  -- have the parser process this
  forward to(parser) continue
  -- return the doc instance.  If this failed, then the
  -- error can be retrieved after the fact
  return doc


/*----------------------------------------------------------------------------*/
/* Method: start_document                                                     */
/* Description: called before the start of document processing                */
/* Arguments:   none                                                          */
/*----------------------------------------------------------------------------*/

::method start_document
  expose doc dom xmlerror currentElement rootElement
  use strict arg

  -- clear parsing state
  xmlerror = .nil
  currentElement = .nil
  rootElement = .nil

  -- we're live...get the DOM and create a document instance
  dom = .ooRexxDOM~implementation
  doc = dom~createDocument

/*----------------------------------------------------------------------------*/
/* Method: end_document                                                       */
/* Description: called after all parsing has completed                        */
/* Arguments:   none                                                          */
/*----------------------------------------------------------------------------*/

::method end_document
/* this method is designed to be overridden by a subclass */
  use strict arg

/*----------------------------------------------------------------------------*/
/* Method: start_element                                                      */
/* Description: called when a start element tag has been encountered.         */
/* Arguments:   an xmlchunk instance.                                         */
/*----------------------------------------------------------------------------*/

::method start_element
  expose currentElement rootElement doc

  use strict arg chunk

  -- create a new context for the new element, inheriting the namespace
  -- definitions from the parents.
  newElement = .ElementContext~new(self, chunk, currentElement)

  -- set the root element if this is the first one
  if rootElement == .nil then do
     rootElement = newElement
     -- add this as the root element of the document
     doc~appendChild(newElement~element)
  end
  -- this is the top of the stack
  currentElement = newElement

/*----------------------------------------------------------------------------*/
/* Method: end_element                                                        */
/* Description: called when an end element tag has been encountered.          */
/* Arguments:   an xmlchunk instance.                                         */
/*----------------------------------------------------------------------------*/

::method end_element
  expose currentElement
  use strict arg chunk

  -- this must be an exact match
  if chunk~tag \== currentElement~element~tagName then
     self~create_error("End tag" chunk~tag "does not match current element" element~tagName)
  -- pop this off the stack and make it the new current
  currentElement = currentElement~parent

/*----------------------------------------------------------------------------*/
/* Method: text                                                               */
/* Description: called when character data has been encountered.              */
/* Arguments:   an xmlchunk instance.                                         */
/*----------------------------------------------------------------------------*/

::method text
  expose doc currentElement
  use strict arg chunk

  -- an element may have multiple text nodes, so we need to
  -- explicitly create the node and append it as a child to the current element
  text = doc~createTextNode(chunk~text)
  currentElement~element~appendChild(text)

/*----------------------------------------------------------------------------*/
/* Method: called when a comment tag has been encountered                     */
/* Arguments:   an xmlchunk instance.                                         */
/*----------------------------------------------------------------------------*/

::method comment
  expose doc currentElement
  use strict arg chunk

  -- comments can appear anywhere...even before the first element
  -- if we have a current element being processed, then add this to that
  if currentElement \= .nil then do
     -- just create the node and add to the current element
     currentElement~element~appendChild(doc~createComment(chunk~text))
  end
  else do
     -- add the comment to the document as a child
     doc~appendChild(doc~createComment(chunk~text))
  end

/*----------------------------------------------------------------------------*/
/* Method: called when a CDATA tag has been encountered                       */
/* Arguments:   an xmlchunk instance.                                         */
/*----------------------------------------------------------------------------*/

::method cdata
  expose doc currentElement
  use strict arg chunk

  -- just create the node and add to the current element
  currentElement~element~appendChild(doc~createCDataSection(chunk~text))

/*----------------------------------------------------------------------------*/
/* Method: called when a processing instruction has been encountered          */
/* Arguments:   an xmlchunk instance.                                         */
/*----------------------------------------------------------------------------*/

::method processingInstruction
  expose doc currentElement
  use strict arg chunk

  -- if we have a current element being processed, then add this to that
  if currentElement \= .nil then do
     -- just create the node and add to the current element
     currentElement~element~appendChild(doc~createProcessingInstruction(chunk~tag, chunk~text))
  end
  else do
     -- add the processing instruction to the document as a child
     doc~appendChild(doc~createProcessingInstruction(chunk~tag, chunk~text))
  end

/*----------------------------------------------------------------------------*/
/* Method: called when a <!DOCTYPE tag has been encountered                   */
/* Arguments:   an xmlchunk instance.                                         */
/*----------------------------------------------------------------------------*/

::method doctypeDecl
  use strict arg chunk

  -- not handled currently

/*----------------------------------------------------------------------------*/
/* Method: error                                                              */
/* Description: called on an error.                                           */
/* Arguments:   an xmlerror instance.                                         */
/*----------------------------------------------------------------------------*/

::method error
  expose xmlerror doc
  use strict arg xmlerror
  -- no document to return if there's an error
  doc = .nil


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: ElementContext                                                      */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

-- class to manage the parsing context of an element under construction.
::class elementContext public
::constant xmlNamespace "http://www.w3.org/XML/1998/namespace"
::constant xmlnsNamespace "http://www.w3.org/2000/xmlns/"
-- the default W3C-defined reserved prefix mappings
::attribute defaultPrefixMap class
-- the parent node context to this node
::attribute parent

::method init class
  expose defaultPrefixMap

  defaultPrefixMap = .directory~new
  defaultPrefixMap['xml'] = self~xmlNamespace
  defaultPrefixMap['xmlns'] = self~xmlnsNamespace

::method init
  expose parser element parent
  use strict arg parser, chunk, parent

  -- step one, figure out all of the namespaces associated with this
  -- element.  This is merged from the parent and extracted from the
  -- attributes.
  self~resolveNameSpaces(chunk, parent)
  -- resolve the namespace for this based on whether the tag name has a
  -- prefix
  namespace = self~resolvePrefix(chunk~tag)
  -- create the element
  element = parser~doc~createElementNS(namespace, chunk~tag)

  -- now process all of the attributes
  loop attr over chunk~attr
      -- special meaning for this one
      if attr == "xmlns" then namespace = self~xmlnsNamespace
      -- otherwise go through the normal resolution process
      else namespace = self~resolvePrefix(attr)
      -- add the attribute to the the element
      element~setAttributeNS(namespace, attr, chunk~attr[attr])
  end

-- the element being constructed
::attribute element
-- URI of any namespace default applied to attributes and children
::attribute defaultNamespace
-- map of prefixes known in this element context
::attribute prefixMap

::method resolveNameSpaces
  expose prefixMap defaultNamespace parser
  use strict arg chunk, parent

  -- if we have a parent node, then start out by inheriting the
  -- xmlns declarations from it.
  if parent \= .nil then do
      defaultNamespace = parent~defaultNamespace
      prefixMap = parent~prefixMap~copy
  end
  else do
      -- no default (yet) and start with the default prefix map
      defaultNamespace = .nil
      prefixMap = .ElementContext~defaultPrefixMap~copy
  end

  -- now look for xmlns attributes for this node
  loop attrName over chunk~attr
      -- default namespace has been specified for this element.  This
      -- will apply to any untagged attributes and children of this node.
      if attrName == "xmlns" then do
         uri = chunk~attr[attrName]
         -- verify we're not attempting to use the standard namespaces illegally.
         if uri == self~xmlNamespace then parser~create_error("URI" self~xmlNamespace "may not be used as the default element namespace.")
         if uri == self~xmlnsNamespace then parser~create_error("URI" self~xmlnsNamespace "may not be used as the default element namespace.")
         defaultNamespace = uri
      end
      -- tagged as an xmlns: prefix definition
      else if attrName~match(1, "xmlns:") then do
          -- add this value to our prefix map
          parse var attrName ":" prefix
          if prefix = "xml" | prefix = "xmlns" then parser~create_error("Prefix" prefix "may not be redefined in a document.")
          if \.xmlchar~isNCName(prefix) then parser~create_error("Invalid prefix name" prefix".")
          prefixMap[prefix] = value
      end
  end

-- resolve a prefix to its URI equivalent for the current element context.
::method resolvePrefix
  expose parser defaultNamespace prefixMap
  use arg tagName

  -- no prefix on the tag name, so use the default
  if tagName~pos(":") == 0 then return defaultNamespace

  parse var tagname prefix ':' localName

  -- this needs to be in the map, otherwise it is an error
  namespace = prefixMap[prefix]
  if namespace = .nil then parser~createError("Unknown namespace prefix:" prefix)
  if \.xmlchar~isNCName(localName) then parser~create_error("Invalid XML localName" localName".")

  return namespace

-- add a child node to this element
::method addChildNode
  expose element
  use strict arg node
  element~appendChild(node)

::requires "xmldom.cls"
::requires "xmlparser.cls"
