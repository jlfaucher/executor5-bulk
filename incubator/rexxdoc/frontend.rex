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
parser~parse

out = .stdout
-- write to a file
out = .stream~new("doc.html")~~command("open replace")
-- write some header
out~say('<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">')
out~say('<html xmlns="http://www.w3.org/1999/xhtml"><head><title>')
-- the title is the root file name
out~say(parser~tree~name)
-- link the style sheet
out~say('</title><link rel="stylesheet" type="text/css" href="style.css" title="Style"></head><body>')
-- print the tree information
printFile(parser~tree,out)

-- print the required files
out~say('<h2>Required Files</h2>')
do r over parser~tree~requires
  out~say('<strong>'r~getFile~name'</strong><br>')
  -- print their tree...
  printFile(r~getFile,out)
end
-- add some common ending
out~say('</body></html>')


::REQUIRES "rexxdoc.cls"

/**
* This routine converts a given rxdoc object to an HTML string.
* Line breakes are replaced by <br>
*/
::ROUTINE doc2string
  use arg doc
  if doc~class \= .string then do
    accu = ""
    do d over doc
      if d~strip = "" then accu||='<br>'
      else accu||=" "d
    end
    return accu'<br>'
  end
  else return doc'<br>'

/**
* Print a while file.
*/
::ROUTINE printFile
  use arg tree, out
  -- the file initialization code
  out~say(doc2string(tree~doc))
  -- this could be moved to some shared routine
  out~say('<pre class="rxsource">')
  do s over tree~source~source
    if s = "" then iterate
    out~say(s)
  end
  out~say("</pre>")

  -- get a list of classes
  classes = tree~classes

  -- print the link section
  out~say('<h1 class="classes">Classes</h1>')
  do o over classes~makeArray~sort
    out~say('<a href="#'o~name'" class="clsi" id="clsi_'o~name'">'o~name||"</a><br>")
  end
  -- print details
  do o over classes~makeArray~sort
    out~say('<div id="cls_'o~name'"><a name="'o~name'"/><h2>'o~name"</h2>")
    out~say(doc2string(o~doc))
    out~say('<div class="methods" id="mth_'o~name'"><h3>Methods</h3>')
    methods = classes[o]~methods
    out~say('<table class="table_methods"><tr><td>name</td><td>Description</td></tr>')
    -- print the methods
    do p over methods~makeArray~sort
      out~say('<tr class="method" id="mth_'o~name'_'p~name'">')
      out~say('  <td>'p~name"</td><td>")
      out~say(doc2string(p~doc))
      -- print the source
      source = p~source~source
      if source \= .nil , source~items > 0 then do
        out~say('  <span class="span_source">Show source')
        out~say('<div class="rxsource_div">')
        out~say('Source for <strong>'o~name||'~'||p~name||'</strong>')
        out~say('<pre class="rxsource">')
        do s over source 
          if s = "" then iterate
          out~say(s)
        end
        out~say('</pre></div></span>')
      end
      out~say('  </td></tr>')
    end
    out~say('</table>')
    -- methods, class
    -- TODO: add support for floating methods, routines...
    out~say('</div></div>')
  end
  return ''
