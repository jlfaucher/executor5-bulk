#!/usr/bin/rexx
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2008 Rexx Language Association. All rights reserved.         */
/*                                                                            */
/* This program and the accompanying materials are made available under       */
/* the terms of the Common Public License v1.0 which accompanies this         */
/* distribution. A copy is also available at the following address:           */
/* http://www.oorexx.org/license.html                                         */
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
/**
* This is a sample front end for the rexxdoc pacakge. It is rather simple and
* only prints a very basic list of classes and their methods. It includes the
* source code of the parsed class. It creates a single file called doc.html.
* Please stay away from using this in a production environment, this is
* experimental software.
* Usage: rexx frontend.rex rexxdoc.cls
*/

use strict arg file
parser = .FileParser~new(file)
parser~parse(.stream~new(file)~arrayIn)


options = .Directory~new
options~include.source = .false
options~hide.private = .true

out = .stdout
-- write to a file
out = .stream~new("doc.html")~~command("open replace")
-- write some header
out~say('<?xml version="1.0" encoding="utf-8" ?><!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">')
out~say('<html xmlns="http://www.w3.org/1999/xhtml"><head><title>')
-- the title is the root file name
out~say(parser~tree~name)
-- link the style sheet
out~say('</title><link rel="stylesheet" type="text/css" href="style.css" title="Style"/></head><body>')
-- print the tree information
printFile(parser~tree,out,options)

-- print the required files
out~say('<h2>Required Files</h2>')
do r over parser~tree~requires
  out~say('<strong>'r~getFile~name'</strong><br/>')
  -- print their tree...
  printFile(r~getFile,out)
end
-- add some common ending
out~say('</body></html>')


::REQUIRES "rexxdoc.cls"

::ROUTINE getMethodDetails
  use strict arg method, mb = (.mutableBuffer~new)
  if method~isInstanceOf(.SourceMethod) , ,
    method~isAttribute then mb~append("Attribute ")
  if method~isUnguarded then mb~append("Unguarded ")
  if method~isClassMethod then mb~append("Class ")
  if method~isPrivate then mb~append("Private ")
  if method~isProtected then mb~append("Protected ")
  if method~isInstanceOf(.SourceMethod) , ,
    method~isAbstract then mb~append("Abstract ")
  
  if mb~length = 0 then mb~append("(default)")
  return mb~string

/**
* This routine converts a given rxdoc object to an HTML string.
* Line breakes are replaced by <br/>
*/
::ROUTINE doc2string
  use strict arg doc, accu = (.MutableBuffer~new)
  if doc~class \= .string then do
    lines = doc~makearray
    empty = .true
    do i = 1 to lines~items
      d = lines[i]
      stripped = d~strip
      if empty , stripped = '' then iterate
      empty = .false
      if stripped = "" then accu~append('<br/>')
      else if i > 1 then accu~append(" "d)
        else accu~append(d)
    end
    if accu~length > 0 then accu~append('<br/>')
  end
  else do
    accu~append(doc'<br/>')
  end
  return accu~string

::ROUTINE replaceSpecials
  use arg string
  string = string~changestr("&","&amp;")
  string = string~changestr("<","&lt;")
  string = string~changestr(">","&gt;")
  return string

::ROUTINE printCode
  use strict arg source, mb = (.MutableBUffer~new)
  mb~append('<pre class="rxsource">'||.EndOfLine)
  do s over source
    if s = "" then iterate
    mb~append(replaceSpecials(s))
    mb~append(.EndOfLine)
  end
  mb~append("</pre>")
  return mb~string

::ROUTINE getSubClasses
  use arg tree, class
  subclasses = .array~new
  do o over tree~classes
    if o~subclass = class then subclasses[subClasses~items+1] = o
  end
  return subClasses~sort


::ROUTINE printClassHierarchy
  use arg tree, mb = (.MutableBuffer~new('<div class="hierarchy">')), known = (.Set~new), indent = 0
  classes = tree~classes~makearray~sort
  do cls over classes
    if cls~subclass~class = .string then do
      known~put(cls)
      mb~append('&nbsp;'~copies(indent)||'<a href="#cls_'cls~name'" class="clsh" id="clsh_'cls~name'">'cls~name||"</a><br/>")
      call printClassHierarchyInt tree, cls, mb, known, indent + 4 
    end
  end
  do cls over classes~difference(known)
    .stderr~say('Class' cls~name 'not included in hierarchy!')
  end
  mb~append('</div>')
  return mb~string

::ROUTINE printClassHierarchyInt
  use arg tree, object, mb, known, indent
  do cls over getSubClasses(tree, object)
    known~put(cls)
    mb~append('&nbsp;'~copies(indent)||'&rarr;'||'<a href="#cls_'cls~name'" class="clsh" id="clsh_'cls~name'">'cls~name||"</a><br/>")
    call printClassHierarchyInt tree, cls, mb, known, indent + 4 
  end


/**
* Print a whole file.
*/
::ROUTINE printFile
  use arg tree, out, options = (.Directory~new)
  -- the file initialization code
  out~say(doc2string(tree~doc))
  if options~include.source = .true then
    out~say(printCode(tree~source~source))
  -- get a list of classes
  classes = tree~classes

  -- print the link section
  out~say('<h1 class="classes">Classes</h1>')
--  do o over classes~makeArray~sort
--    out~say('<a href="#cls_'o~name'" class="clsi" id="clsi_'o~name'">'o~name||"</a><br/>")
--  end

  -- print the class tree

  out~say(printClassHierarchy(tree))

  -- print details
  do o over classes~makeArray~sort
    out~say('<div id="cls_'o~name'"><h2>'o~name"</h2>")
    out~say(doc2string(o~doc))
    subclass = o~subclass
    if subclass~class = .string then
      out~say('Super class: '||subclass)
    else
      out~say('Super class: <a href="#cls_'subclass~name'">'subclass~name'</a>')
    metaclass = o~metaclass
    if metaclass \= .nil then do
      out~say('<br/>')
      if metaclass~class = .string then
        out~say('Meta class: '||metaclass)
      else
        out~say('Meta class: <a href="#cls_'metaclass~name'">'metaclass~name'</a>')
    end
    inherits = o~inherit
    if inherits~items > 0 then do
      out~say('<br/>')
      do inherit over inherits
        if inherit~class = .string then
          out~say('Inherit class: '||inherit)
        else
          out~say('Inherit class: <a href="#cls_'inherit~name'">'inherit~name'</a>')
      end
    end
    subClasses = getSubClasses(tree,o)
    if subClasses~items > 0 then do
      subClasses = subClasses~sort
      num = subClasses~items
      out~say('Direct sub classes: ')
      do i = 1 to num
        cls = subClasses[i]
        out~charout('<a href="#cls_'cls~name'">'cls~name'</a>')
          if i < num then out~say(', ')
      end
    end
    -- print the constants
    constants = classes[o]~constants
    if constants~items > 0 then do
      out~say('<div class="constants" id="const_'o~name'"><h3>Constants</h3>')
      out~say('<table class="srctable table_constants"><thead><tr><td>Name</td><td>Value</td><td>Description</td></tr></thead><tfoot><tr><td colspan="3"></td></tr></tfoot><tbody>')
      do constant over constants
        out~say('<tr class="constant" id="const_'o~name'_'constant~name'"><td>'constant~name'</td><td>'constant~value'</td><td>')
        out~say(doc2string(constant~doc))
        out~say('Modifiers: ')
        out~say(getMethodDetails(constant))
        out~say('</td></tr>')
      end
      out~say('</tbody></table></div>')
    end
    -- print the attributes
    attributes = classes[o]~attributes
    if attributes~items > 0 then do
      out~say('<div class="attributes" id="attr_'o~name'"><h3>Attributes</h3>')
      out~say('<table class="srctable table_attributes"><thead><tr><td>Name</td><td>Access</td><td>Description</td></tr></thead><tfoot><tr><td colspan="3"></td></tr></tfoot><tbody>')
      do attribute over attributes
        out~say('<tr class="attribute" id="attr_'o~name'_'attribute~name'"><td>'attribute~name'</td><td>'attribute~getAccess'</td><td>')
        out~say(doc2string(attribute~doc))
        out~say('Modifiers: ')
        out~say(getMethodDetails(attribute))
        out~say('</td></tr>')
      end
      out~say('</tbody></table></div>')
    end
    -- print the methods
    methods = classes[o]~methods~makeArray~sort
    if methods~items > 0 then do
      out~say('<div class="methods" id="mth_'o~name'"><h3>Methods</h3>')
      out~say('<table class="srctable table_methods"><thead><tr><td>Name</td><td>Description</td></tr></thead><tfoot><tr><td colspan="2"></td></tr></tfoot>')
      out~say('<tbody>')
      do p over methods
        if options~hide.private = .true , p~isPrivate then iterate
        out~say('<tr class="method" id="mth_'o~name'_'p~name'">')
        out~say('  <td>'p~name)
        if options~include.source = .true then do
          -- print the source
          source = p~source~source
          if source \= .nil , source~items > 0 & \p~isAbstract then do
            out~say('<div class="span_source">Show source')
            out~say('<div class="rxsource_div">')
            out~say('Source for <strong>'o~name||'~'||p~name||'</strong>')
            out~say(printCode(source))
            out~say('</div></div>')
          end
        end
        out~say("</td><td>")
        out~say(doc2string(p~doc))
        out~say('Modifiers: ')
        out~say(getMethodDetails(p))
        out~say('  </td></tr>')
      end
      out~say('</tbody>')
      out~say('</table></div>')
    end
    -- methods, class
    -- TODO: add support for floating methods, routines...
    out~say('</div>')
  end
  return ''
