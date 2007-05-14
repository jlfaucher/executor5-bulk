/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Description: Methods which can be attached to the builtin collection       */
/*              classes to add a makexhtml method.                            */
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
/*         Rony G. Flatscher                                                  */
/*                                                                            */
/*----------------------------------------------------------------------------*/


::routine xhtml.methods public
return .methods                -- return floating methods collection


/*----------------------------------------------------------------------------*/
/* Routine: td                                                                */
/*----------------------------------------------------------------------------*/

/** Replaces the XML metacharacters &quot;&amp;&quot;, &quot;&lt;&quot; and
*  &quot;&gt;&quot; with their respective entity representations
*  &quot;&amp;amp;&quot;, &quot;&amp;lt;&quot;, and &quot;&amp;gt;&quot; and
*  returns the edited argument value.
*
*  @param  str    string to edit
*  @return edited the edited string as a 'td' element
*/
::routine td
parse arg str

return "<td>"str~changestr("&","&amp;")~changestr("<","&lt;")~changestr(">","&gt;")"</td>"



/*----------------------------------------------------------------------------*/
/* Method: makexhtml                                                          */
/*----------------------------------------------------------------------------*/

/** Creates an XHTML table containing the contents of the collection class.
*  The table will contain all the items in the collection and optionally their
*  index value.
*
*  @param  border   The XHTML table boder size.
*  @param  prtindex Either .true or .false indicating whether or not to display
*                   the item's index value.
*  @return          XHTML table as a single string
*/
::method makexhtml public
-- set defaults
border = 1
prtindex = .true
-- check arguments
if arg() > 2 then raise syntax 93.902 array (2)
if arg() = 1 then do
   use arg border
   end
else if arg() = 2 then do
   use arg border, prtindex
   end
if datatype(border, 'W') = .false then raise syntax 93.905 array (border)
if border < 0 then raise syntax 93.907 array (border)
prtindex = (prtindex <> .false)

-- create the xhtml table
nl = '0D0A'x

tab1 = "    "
tab2 = tab1~copies(2)
tab3 = tab1~copies(3)

xhtml = "<!-- table for collection of type '"self~class~id"' -->" nl
xhtml = xhtml || tab1 || '<table border="' || border || '">' || nl

w=length(self~items)    -- get maximum number of digits for right-adjusting
s=self~supplier         -- get a supplier object
do i = 1 while s~available
   hint = "#" i~right(w)      -- determine row #
   xhtml = xhtml || tab2 || '<tr>' || nl
   if prtindex = .true then ,
    xhtml = xhtml || tab3 || "<!-- index" hint "-->" td(s~index) || nl

   xhtml = xhtml || tab3 || "<!-- item " hint "-->" td(s~item) || nl
   xhtml = xhtml || tab2 || '</tr>' || nl || nl
   s~next         -- get next index/item pair
   end
xhtml = xhtml || tab1 || '</table>' || nl
return xhtml

