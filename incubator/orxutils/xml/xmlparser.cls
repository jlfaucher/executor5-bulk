/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Description: Very simple class to parse XML.                               */
/*                                                                            */
/* Copyright (c) 2006-2009 Rexx Language Association. All rights reserved.    */
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
/* Contributors: Ruurd Idenburg                                               */
/*                                                                            */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Notes:                                                                     */
/*                                                                            */
/* The xmlparser class is a very simple parser for XML files. It is not an    */
/* official 100% compatible XML parser because it has a lot of limitations,   */
/* most of which you could probably care less about.                          */
/*                                                                            */
/* 1. The parser only understands ASCII, which is a valid subset of UTF-8.    */
/*    It does not understand any other encoding except 8-bit ASCII.           */
/* 2. It does not test that the document is well-formed. It assumes that the  */
/*    document is a well-formed XML document.                                 */
/* 3. The parser does not know how to handle XML processing instructions. It  */
/*    passes those instructions through the passthrough method intact so      */
/*    that the user can try to make sense of them.                            */
/*                                                                            */
/* To use the xmlparser you need to be aware of the following.                */
/*                                                                            */
/* 1. The parser requires ooRexx version 3.2.0 or later.                      */
/* 2. The parser uses a SAX-like interface, but methods of the class are used */
/*    instead of a call-back mechanism. The default methods perform no        */
/*    actions. The user will need to subclass the xmlparser class and         */
/*    override the call-back methods in order to insert their own actions     */
/*    for each XML chunk type.                                                */
/* 3. The call-back methods use the xmlchunk class to pass data to the        */
/*    methods. This is a very simple class and is used as a container for     */
/*    specific types of XML chunks.                                           */
/* 4. Text chunks (CDATA) are passed through the text method intact. If the   */
/*    parser encounters multiple lines of text it invokes the text method     */
/*    for each line individually. It also does not collapse white space chars.*/
/* 5. XML tags are collapsed. This means that if a tag crosses a line         */
/*    boundary then the lines are collapsed together. This is important       */
/*    for processing instruction tags, comment tags and other special tags.   */
/*                                                                            */
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: XMLCHAR                                                             */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

-- a class for identifying valid xml character values
::class xmlchar public mixinclass object

-- complete set of valid characters in 8-bit ascii
::constant valid '090A0D202122232425262728292A2B2C2D2E2F303132333435363738393A3B3C3D3E3F404142434445464748494A4B4C4D4E4F505152535455565758595A5B5C5D5E5F606162636465666768696A6B6C6D6E6F707172737475767778797A7B7C7D7E7F808182838485868788898A8B8C8D8E8F909192939495969798999A9B9C9D9E9FA0A1A2A3A4A5A6A7A8A9AAABACADAEAFB0B1B2B3B4B5B6B7B8B9BABBBCBDBEBFC0C1C2C3C4C5C6C7C8C9CACBCCCDCECFD0D1D2D3D4D5D6D7D8D9DADBDCDDDEDFE0E1E2E3E4E5E6E7E8E9EAEBECEDEEEFF0F1F2F3F4F5F6F7F8F9FAFBFCFDFEFF'x
-- the characters considered space characters
::constant space '090A0D20'x
-- characters valid as the first characters of an XML name
::constant namestart '3A4142434445464748494A4B4C4D4E4F505152535455565758595A5F6162636465666768696A6B6C6D6E6F707172737475767778797AC0C1C2C3C4C5C6C7C8C9CACBCCCDCECFD0D1D2D3D4D5D6D8D9DADBDCDDDEDFE0E1E2E3E4E5E6E7E8E9EAEBECEDEEEFF0F1F2F3F4F5F6F8F9FAFBFCFDFEFF'x
-- characters valid at any position in an XML name
::constant name '2D2E303132333435363738393A4142434445464748494A4B4C4D4E4F505152535455565758595A5F6162636465666768696A6B6C6D6E6F707172737475767778797AB7C0C1C2C3C4C5C6C7C8C9CACBCCCDCECFD0D1D2D3D4D5D6D8D9DADBDCDDDEDFE0E1E2E3E4E5E6E7E8E9EAEBECEDEEEFF0F1F2F3F4F5F6F8F9FAFBFCFDFEFF'x
-- characters valid in a pubid
::constant pubid '0A0D20212324252728292A2B2C2D2E2F303132333435363738393A3B3D3F404142434445464748494A4B4C4D4E4F505152535455565758595A5F6162636465666768696A6B6C6D6E6F707172737475767778797A'x
-- characters valid anywhere in content
::constant content '092021222324252728292A2B2C2D2E2F303132333435363738393A3B3D3E3F404142434445464748494A4B4C4D4E4F505152535455565758595A5B5C5E5F606162636465666768696A6B6C6D6E6F707172737475767778797A7B7C7D7E7F808182838485868788898A8B8C8D8E8F909192939495969798999A9B9C9D9E9FA0A1A2A3A4A5A6A7A8A9AAABACADAEAFB0B1B2B3B4B5B6B7B8B9BABBBCBDBEBFC0C1C2C3C4C5C6C7C8C9CACBCCCDCECFD0D1D2D3D4D5D6D7D8D9DADBDCDDDEDFE0E1E2E3E4E5E6E7E8E9EAEBECEDEEEFF0F1F2F3F4F5F6F7F8F9FAFBFCFDFEFF'x
-- characters valid as first character of an ncname
::constant ncnamestart '4142434445464748494A4B4C4D4E4F505152535455565758595A5F6162636465666768696A6B6C6D6E6F707172737475767778797AC0C1C2C3C4C5C6C7C8C9CACBCCCDCECFD0D1D2D3D4D5D6D8D9DADBDCDDDEDFE0E1E2E3E4E5E6E7E8E9EAEBECEDEEEFF0F1F2F3F4F5F6F8F9FAFBFCFDFEFF'x
-- characters valid anywhere in an ncname
::constant ncname '4142434445464748494A4B4C4D4E4F505152535455565758595A5F6162636465666768696A6B6C6D6E6F707172737475767778797AC0C1C2C3C4C5C6C7C8C9CACBCCCDCECFD0D1D2D3D4D5D6D8D9DADBDCDDDEDFE0E1E2E3E4E5E6E7E8E9EAEBECEDEEEFF0F1F2F3F4F5F6F8F9FAFBFCFDFEFF'x
-- special markup characters
::constant markup "<&%"

-- test if a character is valid in xml encodings
::method isValid class
  use strict arg c
  return c~matchChar(1, self~valid)

-- test if a character is valid in xml content
::method isContent class
  use strict arg c
  return c~matchChar(1, self~content)

-- test if a character is a markup character
::method isMarkup class
  use strict arg c
  return c~matchChar(1, self~markup)

-- test if a character is a space character
::method isSpace class
  use strict arg c
  return c~matchChar(1, self~space)

-- test if a character is valid as the start of a name
::method isNameStart class
  use strict arg c
  return c~matchChar(1, self~namestart)

-- test if a character is valid in a name
::method isName class
  use strict arg c
  return c~matchChar(1, self~name)

-- test if a character is valid as the start of an ncname
::method isNCNameStart class
  use strict arg c
  return c~matchChar(1, self~ncnamestart)

-- test if a character is valid in an ncname
::method isNCName class
  use strict arg c
  return c~matchChar(1, self~ncname)

-- test if a character is valid in a pubid
::method isPubID class
  use strict arg c
  return c~matchChar(1, self~pubid)

-- test if a string is a valid XML name
::method isValidName class
  use strict arg name
  if name = '' then return .false

  if \name~matchchar(1, self~namestart) then return .false

  return name~verify(self~name,,2) \= 0

-- test if a string is a valid XML ncname
::method isValidNCName class
  use strict arg name
  if name = '' then return .false

  if \name~matchchar(1, self~ncnamestart) then return .false

  return name~verify(self~ncname,,2) \= 0

-- test if a string is a valid XML nmtoken
::method isValidNMToken class
  use strict arg name
  if name = '' then return .false

  return name~verify(self~name) \= 0

-- strip all XML white space charactes from a string.  The source
-- string can be either a string or mutablebuffer.
::method stripWhiteSpace
  use strict arg source

  -- find the first non-white space character
  firstNonWhite = source~verify(self~space)
  if firstNonWhite == 0 then
      -- NB:  We could just return "", but doing it this way
      -- will also work with mutablebuffers.
      return source~delstr(1)

  -- if there are leading white space characters, delete them now.
  -- again, doing this in two steps will work appropriately with
  -- mutablebuffers too.
  if firstNonWhite > 1 then do
      source = source~delstr(1, firstNonWhite - 1)
  end

  loop i = source~length by -1
      -- find a non-whitespace char?, then delete
      -- tail now.  Note that since we have detected
      -- a non-whitespace char already, we will always
      -- terminate
      if \source~matchChar(i, self~space) then do
          source = source~delstr(i + 1)
          -- done
          return source
      end
  end


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: XMLPARSER                                                           */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class xmlparser subclass object public


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: XMLPARSER                                                           */
/*        Private methods                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::constant parserver '0.5'             -- the version of this parser
::constant eol '0a'x                   -- newline character used for XML
::constant tab '09'x
::attribute src         private -- the array of xml lines to be parsed
::attribute lineidx     private -- the index into the array of xml lines
::attribute charidx     private -- the index into a xml line
::attribute errortxt    private -- error text
::attribute eof         private -- done parsing?
::attribute xmlerror    private -- the last error



/*----------------------------------------------------------------------------*/
/* Method: setErrorPoint                                                      */
/* Description: record current position for error reporting                   */
/*----------------------------------------------------------------------------*/
::method setErrorPoint
  expose errlineidx errcharidx lineidx charidx
  errlineidx = lineidx
  errcharidx = charidx

/*----------------------------------------------------------------------------*/
/* Method: create_error                                                       */
/* Description: creates an xmlerror instance.                                 */
/*----------------------------------------------------------------------------*/

::method create_error private
  expose errlineidx errcharidx xmlerror eof
  use strict arg msg
  xmlerror = .xmlerror~new
  xmlerror~text = 'Error on line' errlineidx', column' errcharidx':' msg
  xmlerror~filename = self~xmlfile
  xmlerror~line = errline
  xmlerror~charpos = errpos
  -- turn on the eof indicator so nothing else gets processed
  eof = .true
  self~error(xmlerror)   -- pass this to the processing routine


/*----------------------------------------------------------------------------*/
/* Method: xlatetext                                                          */
/* Description: translate & attributes to their normal characters.            */
/*----------------------------------------------------------------------------*/

::method xlatetext private
use strict arg text

-- NOTE:  This works fine for both strings and mutablebuffer.  Care should be
-- taken to ensure it remains that way.
text = text~changestr('&apos;', "'")
text = text~changestr('&quot;', '"')
text = text~changestr('&gt;', '>')
text = text~changestr('&lt;', '<')
text = text~changestr('&amp;', '&') -- always do this one last!
return text


/*----------------------------------------------------------------------------*/
/* Method: current char                                                       */
/* Description: return the current character.                                 */
/*----------------------------------------------------------------------------*/

::method currentchar private
  expose src lineidx charidx
  use strict arg
  if lineidx > src~items then return .nil
  -- report eol as the currennt char if the index is zero
  if charidx == 0 then do
      return self~eof
  end
  return src[lineidx]~subChar(charidx)

/*----------------------------------------------------------------------------*/
/* Method: checkeol                                                           */
/* Description: handle end of line situations                                 */
/*----------------------------------------------------------------------------*/

::method checkeol private
  expose src lineidx charidx eof
  if charidx > src[lineidx]~length then do
     lineidx += 1
     charidx = 0    -- this indicates we just crossed a boundary
  end
  if lineidx > src~items then do
     eof = .true
  end

/*----------------------------------------------------------------------------*/
/* Method: getchar                                                            */
/* Description: get a single character from the xml document.                 */
/*----------------------------------------------------------------------------*/

::method getchar private
  expose src lineidx charidx eof
  use strict arg eol = .false

  -- we set to zero if the last character crossed a line boundary
  -- we might want this, we might not want this...
  if charidx == 0 then do
     -- advance to the first real character
     charidx = 1
     -- if newlines are requested, then return it now
     if eol then do
        return self~eol
     end
  end

  character = src[lineidx]~subchar(charidx)
  charidx += 1
  self~checkeol
  return character

/*----------------------------------------------------------------------------*/
/* Method: checkchar                                                          */
/* Description: check ahead to see if a given string are the next characters  */
/* up in the sequence.  If there is a match, then the read pointer is         */
/* advanced past the matching characters, otherwise the position remains      */
/* unchanged.  Matches will not cross line boundaries                         */
/*----------------------------------------------------------------------------*/

::method checkChar private
  expose charidx lineidx src
  use strict arg string
  -- if we are at a linened boundary, this is always false
  if charidx == 0 then do
     return .false
  end
  -- check for a match
  if src[lineidx]~match(charidx, string) then do
     -- advance the position if it matches
     charidx += string~length
     self~checkeol
     return .true
  end
  return .false


/*----------------------------------------------------------------------------*/
/* Method: getchunk                                                           */
/* Description: returns a chunk of the xml document.                          */
/*----------------------------------------------------------------------------*/

::method getchunk private
  expose src lineidx charidx errortxt eof
  use strict arg
  -- remember the position for error reporting
  self~setErrorPoint
  -- something other than a tag?
  if \self~checkChar('<') then do
     -- handle the text part of an element
     return self~parseText
  end

  -- closing element tag?
  if self~checkChar("/") then do
     return self~parseEndElement
  end
  -- is this CDATA?
  else if self~checkchar("![CDATA[") then do
     return self~parseCData
  end
  -- is this a comment?
  else if self~checkchar("!--") then do
     return self~parseComment
  end
  -- is this a !DOCTYPE declaration?
  else if self~checkChar('!DOCTYPE') then do
     return self~parseDocType
  end
  -- a processing instruction like <?xml >?
  else if self~checkChar('?') then do
     return self~parseProcessingInstruction
  end
  -- some sort of processing instruction.  Of particular importance are CDATA tags
  else do
     -- handle an element tag
     return self~parseElement
  end


/*----------------------------------------------------------------------------*/
/* Method: scanToTagEnd                                                       */
/* Description: scan to the end of an element tag, returning all of the       */
/*              characters between                                            */
/* Arguments:   none                                                          */
/*----------------------------------------------------------------------------*/
::method scanToTagEnd
  expose eof
  element = .mutablebuffer~new
  -- scan until we find the closing > of the element.  This
  -- may span lines...if we find a line break, then replace it
  -- with a blank
  do while \eof
     char = self~getChar(.true)
     -- if we've hit a line break, replace with a space instead
     if char == self~eol then char = ' '
     element~append(char)
     -- we're done once we hit the end
     if self~checkChar('>') then leave
  end
  return element

/*----------------------------------------------------------------------------*/
/* Method: scanToTagEndNested                                                 */
/* Description: scan to the end of an element tag, returning all of the       */
/*              characters between.  This allows for nested elements          */
/* Arguments:   none                                                          */
/*----------------------------------------------------------------------------*/
::method scanToTagEndNested
  expose eof
  instruction = .mutablebuffer~new
  -- scan until we find the closing > of the element.  This
  -- may span lines...if we find a line break, then replace it
  -- with a blank
  nestlevel = 0
  do while \eof
     -- It is possible for tags to be contained within other tags in XML
     -- processing tags. The next two IF statements take care of that nesting
     -- possibility. It will be up to the user to parse out the contained
     -- tags.  This also includes lineends unmodified
     if self~currentchar() = '<' then nestlevel += 1
     if self~currentchar() = '>' then do
        -- if we're still nested, just reduce the nesting level
        if nestlevel > 0 then nestlevel -= 1
        -- done parsing this
        else do
           self~getChar
           leave
        end
     end
     instruction~append(self~getchar(.true))
  end

  return instruction


/*----------------------------------------------------------------------------*/
/* Method: parseText                                                          */
/* Description: parse out element text, maintaining line breaks               */
/* Arguments:   none                                                          */
/*----------------------------------------------------------------------------*/
::method parseText
  expose eof
   chunk = .xmlchunk~new
   -- we found some text data
   text = .mutablebuffer~new
   do while eof = .false, self~currentchar() <> '<'
      -- Do NOT collapse the white space and newlines out of the chunk!
      -- We leave that task up to the client of this class.
      -- Instead, we return a chunk with .endofline stuck in between
      -- at the linebreaks.
      text~append(self~getchar(.true))
   end
   chunk~text = self~xlatetext(text)~string~strip
   -- if this chunk if just whitespace characters (including lineneds), just
   -- ignore it.
   if chunk~text~verify(' '||self~tab||self~eol) \= 0 then
      self~text(chunk) -- call the public override method
   return chunk


/*----------------------------------------------------------------------------*/
/* Method: parseCData                                                         */
/* Description: parse out CDATA test, maintaining line breaks                 */
/* Arguments:   none                                                          */
/*----------------------------------------------------------------------------*/
::method parseCData
  expose eof
  chunk = .xmlchunk~new
  -- we found some text data
  data = .mutablebuffer~new
  do while eof = .false
     char = self~getchar(.true)
     -- potential ending marker?
     if char == ']' then do
        -- test for the rest of the marker
        if self~checkChar(']>') then leave
     end

     -- Do NOT collapse the white space and newlines out of the chunk!
     -- We leave that task up to the client of this class.
     -- Instead, we return a chunk with .eol stuck in between
     -- at the linebreaks.
     data~append(char)
  end

  -- cdata information does not recognize entities.
  chunk~text = data~string
  self~cdata(chunk) -- call the public override method
  return chunk

/*----------------------------------------------------------------------------*/
/* Method: parseComment                                                       */
/* Description: parse out Comment data, maintaining line breaks               */
/* Arguments:   none                                                          */
/*----------------------------------------------------------------------------*/
::method parseComment
  expose eof
   chunk = .xmlchunk~new
   -- we found some text data
   data = .mutablebuffer~new
   do while eof = .false
      char = self~getchar(.true)
      -- potential ending marker?
      if char == '-' then do
         -- test for the rest of the marker
         if self~checkChar('->') then leave
      end

      -- Do NOT collapse the white space and newlines out of the chunk!
      -- We leave that task up to the client of this class.
      -- Instead, we return a chunk with .eol stuck in between
      -- at the linebreaks.
      data~append(char)
   end

   -- comment information does not recognize entities.
   chunk~text = data~string
   self~comment(chunk) -- call the public override method
   return chunk

/*----------------------------------------------------------------------------*/
/* Method: parseEndElement                                                    */
/* Description: parse out a closing element tag, calling the handlers         */
/* Arguments:   none                                                          */
/*----------------------------------------------------------------------------*/
::method parseEndElement
  element = self~scanToTagEnd
  chunk = .xmlchunk~new
  chunk~tag = element
  self~end_element(chunk) -- call the public override method
  return chunk


/*----------------------------------------------------------------------------*/
/* Method: parseAttributes                                                    */
/* Description: parse attribute values from a string                          */
/* Arguments:   the attribute string                                          */
/*----------------------------------------------------------------------------*/
::method parseAttributes
  use arg element

  -- Note:  this is the "=" form, which will ignore the whitespace
  if element = '' then return .nil
  attr = .directory~new
  loop while element \= ''
      -- attribute values must be quoted strings, but either single or
      -- double quotes can be used...
      assignment = element~pos('=')
      -- hmmm, no assignment found, this is invalid
      if assignment = 0 then do
         -- do not allow attributes without values!
         errortxt = 'Error line' errlineidx 'column' errcharidx || ,
                    ': Invalid tag attribute' element'.'
         -- create the error message and handle termination
         self~create_error('Invalid tag attribute' element'.')
         return .nil
      end
      -- pull off the attribute name and see what sort of
      -- value we have
      parse var element attrname =(assignment) +1 element
      if element~match(1, "'") then do
         parse var element "'" value "'" element
      end
      else if element~match(1, '"') then do
         parse var element '"' value '"' element
      end
      else do
        -- do not allow attributes without values!
        self~create_error('Invalid attribute value' element'.') -- call the public override method
        return .nil
      end
      -- these have the entities translated out
      attr[attrname] = self~xlatetext(value)
  end
  return attr


/*----------------------------------------------------------------------------*/
/* Method: parseElement                                                       */
/* Description: parse out an element tag, calling the handlers                */
/* Arguments:   none                                                          */
/*----------------------------------------------------------------------------*/

::method parseElement
  element = self~scanToTagEnd
  chunk = .xmlchunk~new
  -- assume this is not a complete tag element
  endTag = .false
  -- if the close is here, then truncate and remember to send
  -- the end element notification
  if element~match(element~length, '/') then do
     element~setBufferSize(element~length - 1)
     endTag = .true
  end
  -- convert to a string and strip now
  element = element~string~strip

  -- ok, now process the attributes, if any

  -- strip off the tag and parse off each of the attributes
  parse var element chunk~tag element

  -- now parse out the attributes
  chunk~attr = self~parseAttributes(element)

  self~start_element(chunk) -- call the public override method
  -- self terminating element?
  if endTag then do
     endchunk = .xmlchunk~new
     endchunk~tag = chunk~tag
     self~end_element(endchunk) -- call the public override method
  end

  return chunk


/*----------------------------------------------------------------------------*/
/* Method: parseProcessingInstruction                                         */
/* Description: parse out a processing instruction tag                        */
/* Arguments:   none                                                          */
/*----------------------------------------------------------------------------*/
::method parseProcessingInstruction
  expose eof

  chunk = .xmlchunk~new
  instruction = self~scanToTagEnd
  if eof then do
     self~create_error('EOF within an XML tag.') -- call the public override method
     return .nil
  end
  -- instruction is now a string value
  instruction = instruction~string~strip

  -- strip off the target name and parse off each of the attributes
  parse var instruction chunk~tag chunk~text '?'
  self~processingInstruction(chunk) -- call the public override method
  return chunk


/*----------------------------------------------------------------------------*/
/* Method: parseDoctype                                                       */
/* Description: parse out a complex doctype tag.  No attempt is made to       */
/*              interpret the information, including the nested parts         */
/* Arguments:   none                                                          */
/*----------------------------------------------------------------------------*/
::method parseDoctype
  expose eof

  chunk = .xmlchunk~new
  chunk~text = self~scanToTagEndNested~string~strip
  if eof then do
     self~create_error('EOF within a DOCTYPE tag.') -- call the public override method
     return .nil
  end
  chunk~tag = ''
  self~doctypeDecl(chunk) -- call the public override method
  return chunk


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: XMLPARSER                                                           */
/*        Public methods                                                      */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::attribute xmlfile private -- the XML file name, if known

/*----------------------------------------------------------------------------*/
/* Method: start_document                                                     */
/* Description: called before the start of document processing                */
/* Arguments:   none                                                          */
/*----------------------------------------------------------------------------*/

::method start_document
/* this method is designed to be overridden by a subclass */
  use strict arg

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
/* this method is designed to be overridden by a subclass */
  use strict arg chunk

/*----------------------------------------------------------------------------*/
/* Method: end_element                                                        */
/* Description: called when an end element tag has been encountered.          */
/* Arguments:   an xmlchunk instance.                                         */
/*----------------------------------------------------------------------------*/

::method end_element
/* this method is designed to be overridden by a subclass */
  use strict arg chunk

/*----------------------------------------------------------------------------*/
/* Method: text                                                               */
/* Description: called when character data has been encountered.              */
/* Arguments:   an xmlchunk instance.                                         */
/*----------------------------------------------------------------------------*/

::method text
/* this method is designed to be overridden by a subclass */
  use strict arg chunk

/*----------------------------------------------------------------------------*/
/* Method: called when a comment tag has been encountered                     */
/* Arguments:   an xmlchunk instance.                                         */
/*----------------------------------------------------------------------------*/

::method comment
/* this method is designed to be overridden by a subclass */
  use strict arg chunk

/*----------------------------------------------------------------------------*/
/* Method: called when a CDATA tag has been encountered                       */
/* Arguments:   an xmlchunk instance.                                         */
/*----------------------------------------------------------------------------*/

::method cdata
/* this method is designed to be overridden by a subclass */
  use strict arg chunk

/*----------------------------------------------------------------------------*/
/* Method: called when a processing instruction has been encountered          */
/* Arguments:   an xmlchunk instance.                                         */
/*----------------------------------------------------------------------------*/

::method processingInstruction
/* this method is designed to be overridden by a subclass */
  use strict arg chunk

/*----------------------------------------------------------------------------*/
/* Method: called when a CDATA tag has been encountered                       */
/* Arguments:   an xmlchunk instance.                                         */
/*----------------------------------------------------------------------------*/

::method doctypeDecl
/* this method is designed to be overridden by a subclass */
  use strict arg chunk

/*----------------------------------------------------------------------------*/
/* Method: error                                                              */
/* Description: called on an error.                                           */
/* Arguments:   an xmlerror instance.                                         */
/*----------------------------------------------------------------------------*/

::method error
/* this method is designed to be overridden by a subclass */
  use strict arg xmlerror

/*----------------------------------------------------------------------------*/
/* Method: getversion                                                         */
/* Description: return the version of this class.                             */
/*----------------------------------------------------------------------------*/

::method version
  return self~parserver


/*----------------------------------------------------------------------------*/
/* Method: parse_array                                                        */
/* Description: parse the specified array of XML code.                        */
/*----------------------------------------------------------------------------*/

::method parse_array
  expose src lineidx charidx eof xmlerror
  use strict arg src
  eof = .false
  /* parse the xml array */
  lineidx = 1
  charidx = 1
  xmlerror = .nil
  -- set the initial error point
  self~setErrorPoint
  /* make sure this is an xml document */
  if src[1]~pos('<?xml') <> 1 then do
     self~create_error('Invalid XML document.')
     return xmlerror~text
  end
  self~start_document
  do while \eof
     self~getchunk
  end
  -- we send the end_document even if there is an error
  self~end_document
  -- if we had an error, return the message as a return value
  if xmlerror \= .nil then do
     return xmlerror~text
  end
  return ''


/*----------------------------------------------------------------------------*/
/* Method: parse_file                                                         */
/* Description: parse the specified file of XML code.                         */
/*----------------------------------------------------------------------------*/

::method parse_file
  expose xmlfile
  use strict arg xmlfile
  tfile = .stream~new(xmlfile)
  errortxt = tfile~open('read')
  if errortxt <> 'READY:' then do
     tfile~close()
     return errortxt
     end
  lines = tfile~arrayin()
  tfile~close()

  return self~parse_array(lines)


/*----------------------------------------------------------------------------*/
/* Method: parse_stream                                                       */
/* Description: parse the specified stream of XML code                        */
/*              The stream should be opened, and will not be closed at the end*/
/*----------------------------------------------------------------------------*/

::method parse_stream
  use strict arg tfile
  lines = tfile~arrayin()

  return self~parse_array(lines)


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: XMLCHUNK                                                            */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class xmlchunk subclass object public


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: XMLCHUNK                                                            */
/*        Private methods                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: XMLCHUNK                                                            */
/*        Public methods                                                      */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::attribute text                 -- the text
::attribute tag                  -- the xml tag name
::attribute attr                 -- the tag attributes

/*----------------------------------------------------------------------------*/
/* Method: init                                                               */
/* Description: instance initialization                                       */
/*----------------------------------------------------------------------------*/

::method init
use strict arg
  self~text = .nil  -- For the start_element and passthrough methods this contains
                    -- the entire text string enclosed within the '<' and '>'
                    -- brackets. For the text method it contains a single line
                    -- of CDATA text.
  self~tag = .nil   -- For  the start_element and end_element methods this is
                    -- the XML element (tag) name. For the end_element method the
                    -- leading '/' character is not a part of this string.
  self~attr = .nil  -- For the start_element method this is an ooRexx directory
                    -- class instance. Each attribute and value is contained in
                    -- the ooRexx directory instance.

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: XMLERROR                                                            */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::class xmlerror subclass object public


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: XMLERROR                                                            */
/*        Private methods                                                     */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: XMLERROR                                                            */
/*        Public methods                                                      */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

::attribute text                -- the error message text, if any
::attribute filename            -- the xml file name, if known
::attribute line                -- the error line number
::attribute charpos             -- the error character position


/*----------------------------------------------------------------------------*/
/* Method: init                                                               */
/* Description: instance initialization                                       */
/*----------------------------------------------------------------------------*/

::method init
  use strict arg
  self~text = ''
  self~filename = ''
  self~line = 0
  self~charpos = 0

