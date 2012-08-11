::requires "xmldom.cls"

-- record a location within an XML entity.
::class "XmlEntityLocation"
::method init
  expose publicID systemID baseSystemID expandedSystemId lineNumber columnNumber characterOffset
  publicID = .nil
  systemID = .nil
  baseSystemId = .nil
  expandedSystemId = .nil
  lineNumber = 1
  columnNumber = 1
  characterOffset = 1

-- accessors for the stat data
::attribute publicId          -- the public identifier
::attribute systemId          -- the system identifier
::attribute baseSystemId      -- base URI of the entire document
::attribute expandedSystemId  -- expanded fully resolved identifier for the entity
::attribute lineNumber        -- the entity linenumber position
::attribute columnNumber      -- the entity columnNumber position
::attribute characterOffset   -- the entity characterOffset


-- in the XML specification, a document is composed of multiple units called "entities".
-- The main document is called the "document entity"  Contained within the document entity,
-- there will be references to other enties in the form of "&name." (or "%name." for
-- parameter entities).  When an entity reference is encountered, the parsing context
-- switches to the entity's value as if the reference had been directly replaced by
-- the text value of that entity.  Thus, entity references may contain document markup themselves,
-- including references to other entities.  However, it's not exactly that simple.  There are
-- a number of rules that must be respected:
--
-- 1) The entity boundary acts as a separator during parsing.  For example, name token
-- parsing would be terminated at an entity reference (and conversely, for scanning of
-- tokens inside of an entity value, the scanning is terminated at the end of the entity
-- text.
-- 2) Constructs contained within entity text must be complete.  For example, element start
-- and end tags must part of the same entity.
-- 3) The allowed content of the entity depends on the source of the entity.  Internally
-- defined entities operate with one set of rules, externally defined ones have a different
-- rule set.
-- 4) External parsed entities may begin with a text declaration that defines the encoding and
-- xml version that is processed immediately and does not appear as part of the replacement
-- text.
--
-- These rules introduce a number of parsing complications to the scanning/parsing process,
-- so care needs to be taken to recognize when the boundaries are crossed.
-- A good explanation of this physical structure can be found here:
--   http://www.xml.com/axml/target.html#NT-TextDecl

::class "XmlEntityReader"
-- the amount of buffer space we read ahead
::constant BUFFER_SIZE 2048
::constant DOCUMENT_ENTITY 0
::constant INTERNAL_ENTITY 1
::constant EXTERNAL_ENTITY 2
::constant INTERNAL_PARAMETER_ENTITY 3
::constant EXTERNAL_PARAMETER_ENTITY 4
::constant CHARACTER_REFERENCE_ENTITY 5

-- initialize an entity instance
::method init
  expose type stream buffer
  use strict arg type, location, stream = .nil, buffer = .nil

-- the version of the XML we're scanning.  This comes from the <?xml...> declaration.
::attribute xmlVersion
-- ditto for the document encoding.
::attribute encoding
-- forwarders to retrieve information from the location
::attribute publicId get
  expose location
  forward to(location)
::attribute systemId get
  expose location
  forward to(location)
::attribute baseSystemId get
  expose location
  forward to(location)
::attribute expandedSystemId get
  expose location
  forward to(location)
::attribute lineNumber get
  expose location
  forward to(location)
::attribute columnNumber get
  expose location
  forward to(location)
::attribute characterOffset get
  expose location
  forward to(location)

-- check if we need to read more data into the buffer, or
-- potentially, pop the top entity off of the stack
-- and return to a previous context
::method checkBuffer
  expose currentPosition bufferEnd
  -- there are many, many situations where crossing boundaries are
  -- not permitted, enough that it is dangerous to default this.
  -- not defaulting this will help catch errors.
  use strict arg canPopEntity

  if currentPosition > bufferEnd then do
      -- try to load.  This may switch the current
      -- entity, in which case, we need to retry with the
      -- new current to catch the edge cases
      previous = self~loadBuffer(canPopEntity)
      if previous \== .nil then previous~checkBuffer(canPopEntity)
  end

-- load another buffer of data from the stream
::method loadBuffer
  expose parser stream currentPosition bufferEnd baseOffset buffer physicalEof
  -- there are many, many situations where crossing boundaries are
  -- not permitted, enough that it is dangerous to default this.
  -- not defaulting this will help catch errors.
  use strict arg canPopEntity

  -- is this an internal item?
  if stream == .nil then do
      -- if we can't pop the entity, then this is essentially
      -- the EOF for this bit
      if \canPopEntity then return .nil
      -- pop the previous entity.  This is used by the
      -- caller to recursively reinvoke the same call if needed
      return parser~popEntity
  end

  -- increment the basestream offset by the current offset inside the
  -- buffer.  The positions are relative to the buffer, but we also
  -- want to keep track of the actual stream position.
  baseOffset += currentPosition
  buffer = reader~charin(, self~BUFFER_SIZE)

  -- get the read length
  bufferEnd = buffer~length
  currentPosition = 1
  -- if nothing read, we've hit the real eof
  if bufferEnd == 0 then do
      -- if we can't pop the entity, then this is essentially
      -- the EOF for this bit
      if \canPopEntity then return .nil
      -- pop the previous entity.  This is used by the
      -- caller to recursively reinvoke the same call if needed
      return parser~popEntity
  end
  return .nil

-- potentially concatenate another buffer of data from the stream to our current buffer
::method checkExtendBuffer
  expose parser reader currentPosition bufferEnd baseOffset buffer
  -- there are many, many situations where crossing boundaries are
  -- not permitted, enough that it is dangerous to default this.
  -- not defaulting this will help catch errors.
  use strict arg extension, canPopEntity

  -- if the extension amount fits in the current buffer, this is easy.  This
  -- is the normal situation
  if currentPosition + extension <= bufferEnd then return

  -- this might be an internal entity...see if we can pop this off.
  if reader == .nil then do
      -- if we're parsing something, we normally can't switch
      if \canPopEntity then return
      -- we never allow checks to cross an entity border.  We can
      -- only switch if the current position is at the boundary
      -- position
      if currentPosition <= bufferEnd then return
      -- pop the entity and have the previous version to
      -- perform the same check
      previous = parser~popEntity
      if previous \== .nil then previous~checkExtendBuffer(extension, .true)
  end
  else do
      -- concatenate
      extension = reader~charin(, self~BUFFER_SIZE)
      -- we might hit the eof with this read
      if extension == "" then do
          -- we've hit the end...now do the same
          -- entity switch checks.
          -- if we're parsing something, we normally can't switch
          if \canPopEntity then return
          -- we never allow checks to cross an entity border.  We can
          -- only switch if the current position is at the boundary
          -- position
          if currentPosition <= bufferEnd then return
          -- pop the entity and have the previous version to
          -- perform the same check
          previous = parser~popEntity
          if previous \== .nil then previous~checkExtendBuffer(extension, .true)
      end
      else do
        -- add this to the buffer
        buffer = buffer||extension
        -- update the length
        bufferEnd = buffer~length
      end
  end

-- peek at the next character...returns "" if we've hit the end of the entity
-- and we're not able to push back
::method peekCharacter
  expose buffer currentPosition isCharacterReference
  -- there are many, many situations where crossing boundaries are
  -- not permitted, enough that it is dangerous to default this.
  -- not defaulting this will help catch errors.
  use strict arg canPopEntity

  -- check to see if we need to read more data
  self~checkBuffer(canPopEntity)

  -- return the current position.  Note, if we've hit the
  -- eof mark, then this will return ""
  ch = buffer~charAt(currentPosition)
  -- character references are one-character long entities that can contain
  -- any content and are not interpreted as character data no subject to
  -- normalization.  Don't change these.
  if \isCharacterReference then do
      -- we normalize line-ends to be just the single character form
      -- This is a little dicey, since a lot of the scanning loops depend
      -- on using peek to see if they've hit a delimiter and manually
      -- advance the character pointer.  We might need to step ahead
      -- a little and pretend we've kept the position the same.
      if ch == '0d'x then do
          -- we're going to return a newline
          ch = '0a'x
          -- it is most helpful if we can extend the buffer to hold at least
          -- an extra character.  This must remain within the same entity
          self~checkExtendBuffer(1, .false)
          -- now look at the next character
          ch2 = buffer~charAt(currentPosition + 1)

          -- if the next character is the LF, then advance
          -- over the LF.  The LF will be returned if there is a
          -- subsequent scanCharacter call
          if ch2 == '0a'x then currentPosition += 1
      end
  end

  return ch

-- read the next character in the stream.   This normalizes line-ends, and
-- also keeps track of line and column positions for error tracking purposes
::method scanCharacter
  expose buffer currentPosition lineNumber columnNumber isCharacterReference
  -- there are many, many situations where crossing boundaries are
  -- not permitted, enough that it is dangerous to default this.
  -- not defaulting this will help catch errors.
  use strict arg canPopEntity

  -- check to see if we need to read more data
  self~checkBuffer(canPopEntity)

  -- return the current position.  Note, if we've hit the
  -- end of the entity, then this will return ""
  ch = buffer~charAt(currentPosition)
  -- step the next character
  currentPosition += 1
     -- advance the column position
  columnNumber += 1
  -- if this is a character reference, then no normalization is done.  The
  -- character can be any data and will be returned unchanged
  if \isCharacterReference then do
      -- we normalize line-ends to be just the single character form
      -- since we're reading, we need to peek at the next character to
      -- determine if we're scanning one character or two
      --
      if ch == '0d'x then do
          ch = '0a'x
          -- if the next character is the LF, then advance
          -- over the LF.  Otherwise, treat the CR as a stanalone
          -- newline
          if self~peekCharacter(canPopEntity) == '0a'x then
              currentPosition += 1
      end
      -- if this is the end-of-line, reset the logical line positions
      if ch == '0a'x then do
          lineNumber += 1
          columnNumber = 1
      end
  end
  -- and return the character
  return ch

-- scan a span of characters of a given type, copying
-- them to the buffer.  The return value is count of
-- characters scanned.  NOTE:  This does not handle linends, so
-- this should only be called for non-whitespace characters
::method scanCharacters
  expose buffer currentPosition columnNumber bufferEnd
  -- there are many, many situations where crossing boundaries are
  -- not permitted, enough that it is dangerous to default this.
  -- not defaulting this will help catch errors.
  use strict arg target, characterSet, canPopEntity

  -- check to see if we need to read more data.  This is
  -- the only place where we allow an entity switch to occur
  self~checkBuffer(canPopEntity)

  spanStart = currentPosition
  spanEnd = buffer~verify(characterSet,, spanStart)

  -- already at a non-matching character, nothing to copy
  if spanEnd = currentPosition then return 0

  -- matches all the way to the end of the buffer.  If this is
  -- the end of the entity, we're done.  However, we could just be
  -- at a buffer boundary, which means we might need to read more.
  loop forever
      spanEnd = buffer~verify(characterSet,, currentPosition)
      -- good all the way to the end of the current buffer?
      if spanEnd == 0 then do
          length = bufferEnd - spanStart + 1
          target~append(buffer~substr(spanStart))
          currentPosition += length
          columnNumber += length
          -- get another buffer, but don't cross entity
          -- boundaries this time
          self~checkBuffer(.false)
          -- ok, nothing left to scan, but we have at least
          -- something to return
          if currentPosition > bufferEnd then return target~length
          spanStart = currentPosition
      end
      -- found a terminating character, so copy this over and quit
      else do
          length = spanEnd - spanStart
          target~append(buffer~substr(spanStart, length))
          currentPosition += length
          columnNumber += length
          -- we're good
          return target~length
      end
  end

-- scan a particular token type (generally, a name).  The token
-- must begin with one character of a given type and have zero or
-- more characters of a second type.  The token may not cross an entity
-- boundary.
::method scanToken
  expose buffer currentPosition columnNumber bufferEnd
  -- there are many, many situations where crossing boundaries are
  -- not permitted, enough that it is dangerous to default this.
  -- not defaulting this will help catch errors.
  use strict arg target, startSet, characterSet, canPopEntity

  -- check to see if we need to read more data.  This is
  -- the only place where we allow an entity switch to occur
  self~checkBuffer(canPopEntity)

  spanStart = currentPosition
  -- if we miss on the first character, no point in continuing
  if \buffer~matchChar(startSet) then return .false
  currentPosition += 1
  -- now scan for the rest of the token using the secondary
  -- character set
  loop forever
      spanEnd = buffer~verify(characterSet,, currentPosition)
      -- good all the way to the end of the current buffer?
      if spanEnd == 0 then do
          length = bufferEnd - spanStart + 1
          target~append(buffer~substr(spanStart))
          currentPosition += length
          columnNumber += length
          -- get another buffer, but don't cross entity
          -- boundaries this time
          self~checkBuffer(.false)
          -- ok, nothing left to scan, but we have at least
          -- something to return
          if currentPosition > bufferEnd then return .true
          spanStart = currentPosition
      end
      -- found a terminating character, so copy this over and quit
      else do
          length = spanEnd - spanStart
          target~append(buffer~substr(spanStart, length))
          currentPosition += length
          columnNumber += length
          -- we're good
          return .true
      end
  end


-- scan a span of content characters, copying
-- them to the buffer.  The return value is the character that terminated
-- the scan, or "" if this was terminated by an EOF
-- NOTE:  This does handle linends, performing
-- linend normalization and keeping track of line and column positions
::method scanContentCharacters
  expose buffer currentPosition lineNumber columnNumber bufferEnd isCharacterReference
  use strict arg target

  -- check to see if we need to read more data.  We handle
  -- entity switches anywhere here.
  self~checkBuffer(.true)

  -- this is what is valid in the content portion
  characterSet = .XmlChar~content
  spanStart = currentPosition
  -- we keep matching all the way until we find non-content
  -- and non-newline characters.  We need to normalize the
  -- newlines and also keep updating the
  loop forever
      spanEnd = buffer~verify(characterSet,, spanStart)
      -- did we match all the way to the end of the buffer?  Add
      -- this section to the return buffer, try to load more, including
      -- entity switches, and keep looping if there is more data
      if spanEnd == 0 then do
          length = bufferEnd - spanStart + 1
          target~append(buffer~substr(spanStart, length))
          currentPosition = bufferEnd + 1
          columnNumber += length
          -- get more or pop the entity, as appropriate
          self~checkBuffer(.true)
          -- if we have nothing left, then there is nothing left
          if currentPosition > bufferEnd then return ""
      end
      else do
          -- append this portion
          length = spanEnd - spanStart
          target~append(buffer~substr(spanStart, length))
          -- adjust the position
          currentPosition = spanEnd
          columnNumber += length
          -- now we have to see if this might be a newline situation,
          -- these are a bit of a pain, since we need to update line/column positions

          -- we know we have at least one character here.  We might need to expand
          -- of this is a CR character, so we can inspect the next one
          ch = buffer~charAt(currentPosition)
          -- step the next character
          currentPosition += 1
          -- if this is a character reference entity, it will be just one character
          -- long and should just be copied over as content.
          if \isCharacterReference then do
              -- we normalize line-ends to be just the single character form
              -- since we're reading, we need to peek at the next character to
              -- determine if we're scanning one character or two
              if ch == '0d'x then do
                  -- step the next character and add to the buffer
                  currentPosition += 1
                  target~append('0a'x)
                  -- if the next character is the LF, then advance
                  -- over the LF.  Otherwise, treat the CR as a stanalone
                  -- newline
                  if self~peekCharacter(.true) == '0a'x then
                      currentPosition += 1
                  -- adjust the line positions
                  lineNumber += 1
                  columnNumber = 1
              end
              -- a newline character
              else if ch == '0a'x then do
                  -- step the next character and add to the buffer
                  currentPosition += 1
                  target~append('0a'x)
                  -- adjust the line positions
                  lineNumber += 1
                  columnNumber = 1
              end
              else return   -- we're done scanning
          end
          else do
              -- This is a character reference.  Just consume the character and
              -- continue scanning.  We'll pick up again at the previous entity level.
              currentPosition += 1
              target~append(ch)
              -- adjust the column position
              columnNumber = 1
          end
      end
      -- get more or pop the entity, as appropriate
      self~checkBuffer(.true)
      -- if we have nothing left, then there is nothing left
      if currentPosition > bufferEnd then return
      -- look for more characters
      spanStart = currentPosition
  end

-- scan a span of literal characters, copying
-- them to the buffer.  The return value is the character that terminated
-- the scan (hopefully the closing quote).  This will NOT cross an entity
-- boundary, so scanning a quoted string may require multiple calls to scan
-- the entire literal.  A return of "" will indicate we encountered an entity
-- boundary.  NOTE:  This does handle linends, performing
-- linend normalization and keeping track of line and column positions
::method scanLiteral
  expose buffer currentPosition lineNumber columnNumber bufferEnd isCharacterReference
  use strict arg target, quote, characterSet = (.XmlChar~literalcontent)

  -- check to see if we need to read more data.  No entity switches are allowed here.
  self~checkBuffer(.false)

  spanStart = currentPosition
  -- we keep matching all the way until we find non-content
  -- characters.  We need to normalize the  newlines and also keep updating the
  -- position indicators
  loop forever
      spanEnd = buffer~verify(characterSet,, spanStart)
      -- did we match all the way to the end of the buffer?  Add
      -- this section to the return buffer, try to load more, including
      -- entity switches, and keep looping if there is more data
      if spanEnd == 0 then do
          length = bufferEnd - spanStart + 1
          target~append(buffer~substr(spanStart, length))
          currentPosition = bufferEnd + 1
          columnNumber += length
          -- get more, but only within this entity
          self~checkBuffer(.false)
          -- if we have nothing left, then there is nothing left
          if currentPosition > bufferEnd then return ""
      end
      else do
          -- append this portion
          length = spanEnd - spanStart
          target~append(buffer~substr(spanStart, length))
          -- adjust the position
          currentPosition = spanEnd
          columnNumber += length
          -- now we need to see why we terminated.  If this is a literal character
          -- reference, we don't interpret this as content, but append without
          -- interpretation.
          -- If not a literal, then we need to check for a closing quote, newline
          -- characters, or potential entity references.
          if isCharacterReference then do
              target~append(ch)
          end

          -- we know we have at least one character here.  We might need to expand
          -- of this is a CR character, so we can inspect the next one
          ch = buffer~charAt(currentPosition)
          -- a literal value is always taken directly
          if isCharacterReference then do
              target~append(ch)
              -- step the next character
              currentPosition += 1
              columnPosition += 1
              -- get more, but only within this entity
              self~checkBuffer(.false)
              -- if we have nothing left, then there is nothing left
              if currentPosition > bufferEnd then return ""
          end
          else do
              -- is this our target quote?
              if ch == quote then do
                  -- consume this from the buffer and return the terminator
                  currentPosition += 1
                  columnPosition += 1
                  return ch
              end
              -- we normalize line-ends to be just the single character form
              -- since we're reading, we need to peek at the next character to
              -- determine if we're scanning one character or two
              else if ch == '0d'x then do
                  -- step the next character and add to the buffer
                  currentPosition += 1
                  target~append('0a'x)
                  -- get more, but only within this entity
                  self~checkBuffer(.false)
                  -- if the next character is the LF, then advance
                  -- over the LF.  Otherwise, treat the CR as a stanalone
                  -- newline
                  if buffer~subChar(currentPosition) == '0a'x then
                      currentPosition += 1
                  -- adjust the line positions
                  lineNumber += 1
                  columnNumber = 1
              end
              -- a newline character
              else if ch == '0a'x then do
                  -- step the next character and add to the buffer
                  currentPosition += 1
                  target~append('0a'x)
                  -- adjust the line positions
                  lineNumber += 1
                  columnNumber = 1
              end
              else return ch  -- we're done scanning
          end
      end
      -- get more or pop the entity, as appropriate
      self~checkBuffer(.true)
      -- if we have nothing left, then there is nothing left
      if currentPosition > bufferEnd then return ""
      -- look for more characters
      spanStart = currentPosition
  end

-- scan a span of pseudo literal characters, copying
-- them to the buffer.  The return value is the character that terminated
-- the scan (hopefully the closing quote).  This will NOT cross an entity
-- boundary, but will consume characters that would normally terminate
-- a normal attribute literal.
-- A return of "" will indicate we encountered an entity
-- boundary.  NOTE:  This does handle linends, performing
-- linend normalization and keeping track of line and column positions
::method scanPseudoLiteral
  expose buffer currentPosition lineNumber columnNumber bufferEnd isCharacterReference
  use strict arg target, quote, characterSet = (.XmlChar~literalcontent)

  -- check to see if we need to read more data.  No entity switches are allowed here.
  self~checkBuffer(.false)

  spanStart = currentPosition
  -- we keep matching all the way until we find non-content
  -- characters.  We need to normalize the  newlines and also keep updating the
  -- position indicators
  loop forever
      spanEnd = buffer~verify(characterSet,, spanStart)
      -- did we match all the way to the end of the buffer?  Add
      -- this section to the return buffer, try to load more, including
      -- entity switches, and keep looping if there is more data
      if spanEnd == 0 then do
          length = bufferEnd - spanStart + 1
          target~append(buffer~substr(spanStart, length))
          currentPosition = bufferEnd + 1
          columnNumber += length
          -- get more, but only within this entity
          self~checkBuffer(.false)
          -- if we have nothing left, then there is nothing left
          if currentPosition > bufferEnd then return ""
      end
      else do
          -- append this portion
          length = spanEnd - spanStart
          target~append(buffer~substr(spanStart, length))
           -- adjust the position
          currentPosition = spanEnd
          columnNumber += length

          -- NOTE:  We will never be in a character reference, so all of the
          -- data is what it is

          -- we know we have at least one character here.  We might need to expand
          -- of this is a CR character, so we can inspect the next one
          ch = buffer~charAt(currentPosition)
          -- is this our target quote?
          if ch == quote then do
              -- consume this from the buffer and return the terminator
              currentPosition += 1
              columnPosition += 1
              return ch
          end
          -- we normalize line-ends to be just the single character form
          -- since we're reading, we need to peek at the next character to
          -- determine if we're scanning one character or two
          else if ch == '0d'x then do
              -- step the next character and add to the buffer
              currentPosition += 1
              target~append('0a'x)
              self~checkBuffer(.false)
              -- if the next character is the LF, then advance
              -- over the LF.  Otherwise, treat the CR as a stanalone
              -- newline
              if buffer~subChar(currentPosition) == '0a'x then currentPosition += 1
              -- adjust the line positions
              lineNumber += 1
              columnNumber = 1
          end
          -- a newline character
          else if ch == '0a'x then do
              -- step the next character and add to the buffer
              currentPosition += 1
              target~append('0a'x)
              -- adjust the line positions
              lineNumber += 1
              columnNumber = 1
          end
          -- just append this to the buffer and continue
          else do
              currentPosition += 1
              target~append(ch)
              columnNumber += 1
          end
      end
      -- get more or pop the entity, as appropriate
      self~checkBuffer(.true)
      -- if we have nothing left, then there is nothing left
      if currentPosition > bufferEnd then return ""
      -- look for more characters
      spanStart = currentPosition
  end

-- test if we're at a delimiter boundary.  The check string should be everything
-- but the first character of the delimiter.  Returns true if this is a match AND
-- the read position will be stepped over the matching delimiter string
::method checkDelimiter
  expose buffer currentPosition isCharacterReference
  -- there are many, many situations where crossing boundaries are
  -- not permitted, enough that it is dangerous to default this.
  -- not defaulting this will help catch errors.
  use strict arg target, canPopEntity

  -- character references cannot be seen as markup, so always fail these.
  if isCharacterReference then return .false

  -- ensure we won't hit a buffer boundary
  self~checkExtendBuffer(target~length, canPopEntity)

  if buffer~match(currentPosition, target) then do
      -- update the position and return true
      currentPosition += target~length
      return .true
  end

  return .false

-- test if we're at the start of an element.  This looks for a '<' followed by a
-- valid namestart character.  is a look-a-head operation that does not advance the
-- scan pointer.  This will pop the current entity, if necessary.
::method checkElementStart
  expose buffer currentPosition

  -- ensure we won't hit a buffer boundary.  We need to
  -- see two characters ahead.  Since we're checking for the
  -- start of an element, we allow this to switch entities.
  self~checkExtendBuffer(2, .true)

  if buffer~match(currentPosition, '<'), .XmlChar~isNameStart(buffer~subchar(currentPosition + 1)) then
      return .true
  return .false


-- skip any whitespace characters in the stream.  This will always cross entity
-- boundaries
::method skipWhiteSpace
  expose currentPosition buffer lineNumber columnNumber
  -- there are many, many situations where crossing boundaries are
  -- not permitted, enough that it is dangerous to default this.
  -- not defaulting this will help catch errors.
  use strict arg canPopEntity

  skipped = .false   -- assume we don't find any

  -- blanks and tabs are more common than newlines, so we can use this to
  -- quickly skip spans of these.
  characterSet = '0920'x
  -- we keep matching all the way until we find non-space characters
  -- this is a bit of a pain, because we also need to keep track of
  -- line number positions when we skip over linends.  This requires
  -- doing a more manual scan and processing each of the whitespace
  -- candidates
  loop forever
      -- check to see if we need to read more data.  Usually, entity
      -- switches are fine, but inside of element tags, for example,
      -- they are not permitted.
      self~checkBuffer(canPopEntity)
      -- if we have nothing left, then there is nothing left
      if currentPosition > bufferEnd then return skipped
      -- skip over blanks and tabs
      scanEnd = buffer~verify(characterSet,, currentPosition)
      -- if no hits, we can just skip the rest of the buffer.
      -- We don't have to normalize anything (yay!)
      if scanEnd = 0 then do
          skipped = .true
          columnNumber += bufferEnd - currentPosition + 1
          currentPosition = bufferEnd + 1
      end
      -- we found a non target character.  Handle newlines, but if
      -- this is not one of those, we're done
      -- we skipped over and then see what triggered the stop
      else do
          -- do we have anything to skip?
          if scanEnd \= currentPosition then do
              skipped = .true
              length = scanEnd - currentPosition
              columnNumber += length
              currentPosition += length
          end
          -- now try to normalize the linends
          ch = buffer~subChar(currentPosition)
          -- special processing may be required for CR
          if ch == '0d'x then do
              skipped = .true
              currentPosition += 1
              -- if this is the first part of a CRLF sequence, just
              -- skip over the second character
              if buffer~subChar(currentPosition) == '0a'x then currentPosition += 1
              -- treat this as a new line and update positions
              lineNumber += 1
              columnNumber = 1
          end
          -- line feed, this is a line break
          else if ch == '0a'x then do
              skipped = .true
              currentPosition += 1
              lineNumber += 1
              columnNumber = 1
          end
          else return skipped
      end
  end


-- scan data until we hit a particular delimiter.
::method scanData
  expose currentPosition buffer lineNumber columnNumber
  use arg delimiter, target

  -- We scan for newlines and the first character of the delimiter.
  -- we need to search for the newlines because we have to normalize
  characterSet = .XmlChar~newline||delimiter~subchar(1)
  -- we keep matching all the way until we find non-space characters
  -- this is a bit of a pain, because we also need to keep track of
  -- line number positions when we skip over linends.  This requires
  -- doing a more manual scan and processing each of the whitespace
  -- candidates
  loop forever
      -- check to see if we need to read more data.  We handle
      -- entity switches anywhere here.
      self~checkBuffer(.true)
      -- if we have nothing left, then there is nothing left
      if currentPosition > bufferEnd then return .false
      -- scan for any of the target characters.
      scanEnd = buffer~verify(characterSet, 'M', currentPosition)
      -- if not found, then the rest of the buffer is just
      -- copied over.  We don't have to normalize anything (yay!)
      if scanEnd = 0 then do
          target~append(buffer~substr(currentPosition))
          columnNumber += bufferEnd - currentPosition + 1
          currentPosition = bufferEnd + 1
      end
      -- we found one of the target characters. Copy over the part
      -- we skipped over and then see what triggered the stop
      else do
          -- do we have anything to copy?
          if scanEnd \= currentPosition then do
              length = scanEnd - currentPosition
              target~append(buffer~substr(currentPosition, length))
              columnNumber += length
              currentPosition += length
          end
          -- is this our delimiter?  we're done
          if buffer~match(i, delimiter) then return .true
          -- now try to normalize the linends
          ch = buffer~subChar(currentPosition)
          -- special processing may be required for CR
          if ch == '0d'x then do
               -- convert this.
               ch = '0a'x
               -- if this is the first part of a CRLF sequence, just
               -- skip over the second character
               if buffer~subChar(currentPosition + 1) == '0a'x then currentPosition += 1
               -- treat this as a new line and update positions
               lineNumber += 1
               columnNumber = 1
          end
          -- line feed, this is a line break
          else if ch == '0a'x then do
              lineNumber += 1
              columnNumber = 1
          end
          -- add this to the buffer.  Note that this could be the first
          -- character of our delimiter, but we failed to match, so just copy it over
          target~append(ch)
      end
  end

-- a mixin class for handling reading different XML elements from
-- an input stream
::class "XMLTokenScanner" mixinclass Object
-- initialize the scanner element
::method initScanner

-- accessor method for the current entity
::attribute currentEntity get

-- peek at the next character...returns "" if we've hit the end of the stream
::method peekCharacter
  expose currentEntity
  -- this is all handled by the current entity
  forward to(currentEntity)

-- read the next character in the stream.   This normalizes line-ends, and
-- also keeps track of line and column positions for error tracking purposes
::method scanCharacter
  expose currentEntity
  -- this is all handled by the current entity
  forward to(currentEntity)

-- we expect an NMToken at the current position, so scan it off
::method scanNMToken
  expose tokenBuffer currentEntity

  -- clear out our accumulator buffer
  tokenBuffer~setBufferSize(0)

  -- allow an entity switch at the boundary
  currentEntity~scanCharacters(tokenBuffer, .XmlChar~name, .true)
  -- retrieve the string form
  return tokenBuffer~string

-- we expect a NameToken at the current position, so scan it off
::method scanName
  expose tokenBuffer currentEntity
  -- there are many, many situations where crossing boundaries are
  -- not permitted, enough that it is dangerous to default this.
  -- not defaulting this will help catch errors.
  use strict arg canPopEntity

  -- clear out our accumulator buffer
  tokenBuffer~setBufferSize(0)

  -- This will scan everything using both character sets and ensure it is
  -- not split across an entity boundary.
  currentEntity~scanToken(tokenBuffer, .XmlChar~nameStart, .XmlChar~name, canPopEntity)
  -- retrieve the string form
  return tokenBuffer~string

-- we expect either a Name token or an entity ref at the current position
::method scanNameTokenOrParameterEntityRef
  expose tokenBuffer currentEntity

  -- not at the parameter entity marker, this must be a name
  if \currentEntity~checkDelimiter('%', .true) then return self~scanName(.true)

  -- clear out our accumulator buffer
  tokenBuffer~setBufferSize(0)
  tokenBuffer~append('%')

  -- This will scan everything using both character sets and ensure it is
  -- not split across an entity boundary.
  currentEntity~scanToken(tokenBuffer, .XmlChar~nameStart, .XmlChar~name, .false)
  -- now make sure this is terminated correctly (again, without crossing a boundary)
  if \currentEntity~checkDelimiter(';', .false) then self~reportFatalError("Missing ';' in parameter entityName" tokenBuffer~string)
  -- add that to the scanned name
  tokenBuffer~append(';')
  -- retrieve the string form
  return tokenBuffer~string

-- we expect a NCNameToken at the current position, so scan it off
::method scanNCName
  expose tokenBuffer currentPosition
  use strict arg canPopEntity

  -- clear out our accumulator buffer
  tokenBuffer~setBufferSize(0)

  -- This will scan everything using both character sets and ensure it is
  -- not split across an entity boundary.
  currentEntity~scanToken(tokenBuffer, .XmlChar~NCNameStart, .XmlChar~ncname, canPopEntity)

  -- retrieve the string form
  return tokenBuffer~string

-- we expect a QName at the current position, so scan it off
::method scanQName
  expose tokenBuffer currentPosition
  use strict arg canPopEntity

  -- clear out our accumulator buffer
  tokenBuffer~setBufferSize(0)

  prefix = ""
  localName = ""
  -- This will scan everything using both character sets and ensure it is
  -- not split across an entity boundary.
  currentEntity~scanToken(tokenBuffer, .XmlChar~NCNameStart, .XmlChar~name, canPopEntity)

  colon = tokenBuffer~pos(':')
  if colon = 0 then localName = tokenBuffer~string
  else do
      prefix = tokenBuffer~substr(1, colon - 1)
      localName = tokenBuffer~substr(colon + 1)
      -- both parts must be valid ncnames (which also eliminates "")
      if \.XmlChar~isNCName(prefix) | \.XmlChar~isNCName(localName) then return .nil
  end
  -- return as a qualified name
  return .qname~new(localName, prefix)

-- scan content characters between element tags
::method scanContent
  expose tokenBuffer currentEntity documentHandler

  -- we might be reading this in multiple chunks if
  -- we're dealing with nested entities.  Keep handling
  -- these until we hit a character that causes us to switch
  -- context
  loop forever
      -- clear out our accumulator buffer
      tokenBuffer~setBufferSize(0)

      -- scan off as much as we can, including new line characters,
      -- performing line-end normalization
      ch = currentEntity~scanContentCharacters(tokenBuffer, .true)
      -- if we have data, send to the document handler for processing
      if tokenBuffer~length > 0 then documentHandler~characters(tokenBuffer~string)
      -- hit a terminating character, so return that indicator
      if ch \== '' then return ch
      -- pop off the entity we just hit the end of
      previous = self~popEntity
      -- hmmm, hit the actual EOF...looks like something is missing
      if previous == .nil then return ""
  end

-- scan a required quoted string.  This also resolves and handles
-- any embedded entities
::method scanQuotedString
  expose tokenBuffer currentEntity
  -- clear out our accumulator buffer
  tokenBuffer~setBufferSize(0)

  startEntity = currentEntity   -- save this in case there are switches
  -- get the first item, with no entity switching.
  quote = currentEntity~scanCharacter(.false)
  if quote \== "'" & quote \== '"' then self~fatalError(self~OPEN_QUOTE_EXPECTED_ERR)
  -- perform the scan.  We're only looking for the closing quote here
  ch = currentEntity~scanLiteral(quote, tokenBuffer)
  -- if this was terminated by the same quote we started with, then we're done. The
  -- scanning will not cross entity boundaries, so this was contained within the entity,
  -- which is common (and what we needed)
  if ch == quote then do
      -- normalize the whitespace
      tokenBuffer~translate('   ', '090a0d'x)
      return tokenBuffer~string
  end

  -- we terminated with something other than our target quote.  Some of these
  -- reasons will require some additional scanning, other's are errors.  We may
  -- require multiple passes to scan the entire value if there are entities
  -- in the value.
  loop forever
      -- encountered an entity reference in the literal.  We need to make that
      -- the active entity, then scan it.
      if ch == '&' then do
          -- skip over it
          currentEntity~scanCharacter(.false)
          -- is this a character reference?
          if currentEntity~checkDelimiter('#', .false) then do
              -- scan the character reference...any bad values are
              -- raised as errors.  We can add the returned value
              -- directly to the literal
              tokenBuffer~append(currentEntity~scanCharacterReferenceValue)
          end
          -- some sort of named entity.  This will either be a predefined one that
          -- can be processed here, or a named one that will become the
          -- current parsing context.
          else do
              -- scan off the name
              entityName = entityScanner~scanName(.false)
              if entityName == "" then self~reportFatalError("Invalid entity name")
              if \currentEntity~checkDelimiter(';', .false) then self~reportFatalError("Missing ';' on entity name" entityName)
              -- now check the predefined entities
              if entityName == "amp" then tokenBuffer~append('&')
              else if entityName == "apos" then tokenBuffer~append("'")
              else if entityName == "lt" then tokenBuffer~append('<')
              else if entityName == "gt" then tokenBuffer~append('>')
              else if entityName == "quot" then tokenBuffer~append('"')
              else do
                  -- make this entity our active one and continue parsing
                  self~pushEntity(entityName)
              end
          end
      end
      else if ch == '<' then self~reportFatalError("Less than ('<') is not valid in an attribute value")
      -- We have a quote character.  We've already checked for the terminating quote,
      -- so this will be the other version.  Just add it on to the value
      else if ch == '"' | ch == "'" then do
          currentEntity~scanCharacter(.false)
          tokenBuffer~append(ch)
      end
      else if ch == "" then do
          -- if we've hit the end of the starting entity and not found
          -- a closing quote, this is an error
          if currentEntity == startEntity then self~reportFatalError("Missing attribute value closing quote")
          -- used up an entity, so back up
          self~popEntity
      end
      else self~reportFatalError("Invalid character in attribute value: '"ch"' ('"ch~c2x"'x)")
      -- scan some more data and see
      newData = tokenBuffer~length + 1
      ch = currentEntity~scanLiteral(quote, tokenBuffer)
      -- normalize the whitespace (but only the new data so we don't change values from char refs)
      tokenBuffer~translate('   ', '090a0d'x,, newData)
      -- if this is our quote, we need to make sure it came from the same entity
      -- as the starting quote
      if ch == quote then do
          if currentEntity \= startEntity then self~reportFatalError("Invalid quote in entity value")
          leave   -- we have the final scanned value
      end
  end

  return tokenBuffer~string

-- scan literal string characters, terminating with the indicated quote type
--
-- [12] PubidLiteral ::= '"' PubidChar* '"' | "'" (PubidChar - "'")* "'"
-- [13] PubidChar::= #x20 | #xD | #xA | [a-zA-Z0-9] | [-'()+,./:=?;!*#@$_%]
--
-- The returned string is normalized according to the following rule,
-- from http://www.w3.org/TR/REC-xml#dt-pubid:
--
-- Before a match is attempted, all strings of white space in the public
-- identifier must be normalized to single space characters (#x20), and
-- leading and trailing white space must be removed.
::method scanPubIdLiteral
  expose tokenBuffer currentEntity

  -- clear out our accumulator buffer
  tokenBuffer~setBufferSize(0)

  -- get the first item, with no entity switching.
  quote = currentEntity~scanCharacter(.false)
  if quote \== "'" & quote \== '"' then self~fatalError("Quote expected for public id value")
  -- perform the scan.  We're only looking for the closing quote here
  ch = currentEntity~scanLiteral(quote, tokenBuffer, .XmlChar~pubid)
  -- if this was terminated by the same quote we started with, then we're done.  We
  -- don't support entities here and also don't cross bounaries, so we have a few error
  -- checks and we're done
  if ch == quote then return tokenBuffer~string
  -- hit the end of the entity, missing quote error
  if ch == "" then self~fatalError("Missing closing quote for public id value")
  -- an invalid character
  else self~fatalError("Invalid character '"ch"' ('"ch~c2x"'x) in public id value")

-- test if we're at a delimiter boundary.  The check string should be everything
-- but the first character of the delimiter.  Returns true if this is a match AND
-- the read position will be stepped over the matching delimiter string
::method checkDelimiter
  expose currentEntity
  forward to(currentEntity)

-- test if we're at the start of an element.  This looks for a '<' followed by a
-- valid namestart character.  is a look-a-head operation that does not advance the
-- scan pointer.
::method checkElementStart
  expose currentEntity
  forward to(currentEntity)

-- skip any whitespace characters in the stream
::method skipWhiteSpace
  expose currentEntity
  forward to(currentEntity)

-- scan until a delimiter is found
::method scanData
  expose currentEntity
  forward to(currentEntity)

-- we have some required whitespace...raise an error if not found
::method requiredWhiteSpace
  -- we only see required whitespace in situations where we're contrained to
  -- same entity parsing situations, so don't allow switching
  if \self~skipWhiteSpace(.false) then do
      use strict arg error
      self~reportFatalError(error)
  end

-- skip over white space and potentially handle paremeter entities
-- while scanning DTD statements.  Parameter entities can appear basically
-- at any boundary where a space is required, so in addition to skipping
-- spaces, we need to detect and handle PE references in the stream.  This can
-- go recursively as well.
::method skipDeclSep
  expose currentEntity peRefsPermitted
  -- most of the time, recognition of PE refs depends on whether this is an
  -- internal subset or not.  In a few locations, the recognition is explicit,
  -- so we have an override means
  use strict arg checkPeRefs = (peRefsPermitted)

  -- we can be switching back and forth constantly in DTD contexts, so if
  -- scanning blanks, keep popping entity contexts until we find a real non-blank
  hadSpace = currentEntity~skipWhiteSpace(.true)
  -- PE refs are only valid in external subsets, so if processing an internal
  -- entity, we don't even look for PERefs
  if checkPeRefs then do
      hadSpace = .true   -- PErefs act as separators, so pretend this was a space
      -- handle PErefs.  Note that each PE ref could start
      -- with a PEref (or even whitespace), so we keep doing this
      -- until we find something real to parse
      loop while currentEntity~checkDelimiter('%', .true)
          -- scan the name, which must be part of the same entity as the
          -- triggering '%'
          name = currentEntity~scanName(.false)
          if name == "" then self~reportFatalError("Missing parameter entity name")
          if \currentEntity~checkDelimiter(';', .false) then self~reportFatalError("Unterminated parameter entity name: '"name"'")
          -- handle the context switch
          self~pushParameterEntity(name)
          -- handle any whitespace in this entity
          currentEntity~skipWhiteSpace(.true)
      end
  end
  return hadSpace

-- we require either whitespace or a PE ref...raise an error if not found
::method requiredDeclSep
  -- we only see required whitespace in situations where we're contrained to
  -- same entity parsing situations, so don't allow switching
  if \self~skipDeclSep then do
      use strict arg error
      self~reportFatalError(error)
  end


-- This is the part of the parser that's document structure aware
::class "XmlDomParser" inherit XMLTokenScanner

-- scan an XML declaration at the start of a document
-- [23] XMLDecl ::= '<?xml' VersionInfo EncodingDecl? SDDecl? S? '?>'
-- [24] VersionInfo ::= S 'version' Eq (' VersionNum ' | " VersionNum ")
-- [80] EncodingDecl ::= S 'encoding' Eq ('"' EncName '"' |  "'" EncName "'" )
-- [81] EncName ::= [A-Za-z] ([A-Za-z0-9._] | '-')*
-- [32] SDDecl ::= S 'standalone' Eq (("'" ('yes' | 'no') "'")
--                 | ('"' ('yes' | 'no') '"'))
::method scanXMLDecl
  expose documentHandler

  currentEntity = self~currentEntity

  -- just return using a directory for simplicity
  do label attributes
      -- skip any whitespace before the attributes...the space is
      -- a required element, so remember if we saw this
      self~requiredWhiteSpace("A space is required before attribute names")

      -- scan off an attribute value
      attr = self~scanPseudoAttribute
      -- the first attribute MUST be the version
      if attr[1] \= "version" then self~fatalError("Unrecognized xml declaration attribute" attr[1])
      -- validate the version information
      self~validateVersion(attr[2])
      currentEntity~version = attr[2]

      hadSpace = self~skipWhiteSpace(.false)
      -- if we're at a question mark, this is the end
      if self~peekCharacter(.false) == '?' then leave attributes
      -- should be an attribute separated by a space here
      if \hadSpace then self~fatalError("A space is required before attribute names")
      -- scan off an attribute value
      attr = self~scanPseudoAttribute
      -- this one can be an encoding or the standalone attribute.  If it is the encoding,
      -- then it can be followed by standalone.  If standalone, that must be the last one.
      if attr[1] == "encoding" then do
          currentEntity~encoding = attr[2]
          -- now check for standalone
          hadSpace = self~skipWhiteSpace(.false)
          -- if we're at a question mark, this is the end
          if self~peekCharacter(.false) == '?' then leave attributes
          -- should be an attribute separated by a space here
          if \hadSpace then self~fatalError("A space is required before attribute names")
          -- scan off an attribute value
          attr = self~scanPseudoAttribute
          if attr[1] \= "standalone" then self~fatalError("Unrecognized xml declaration attribute" attr[1])
          if attr[2] \== "yes" & attr[2] \= "no" then self~fatalError("Invalid standalone value '"attr[2]"'")
          if attr[2] == "yes" then currentEntity~standalone = .true
          else currentEntity~standalone = .false
      end
      else if attr[1] == "standalone" then do
          if attr[2] \== "yes" & attr[2] \= "no" then self~fatalError("Invalid standalone value '"attr[2]"'")
          if attr[2] == "yes" then currentEntity~standalone = .true
          else currentEntity~standalone = .false
      end
      else self~fatalError("Unrecognized xml declaration attribute" attr[1])
  end

  -- we should be at the delimiter next
  self~skipWhiteSpace(.false)
  -- if we're at a question mark, this is the end
  if self~checkDelimiter('?>', .false) then self~fatalError("Missing ?> terminator for xml declaration")
  -- notify the document handler of what we have
  documentHandler~xmlDecl(currentEntity~version, currentEntity~encoding, currentEntity~standalone)

-- scan a text declaration at the start of an external entity
-- [77] TextDecl ::= '<?xml' VersionInfo? EncodingDecl S? '?>'
-- [24] VersionInfo ::= S 'version' Eq (' VersionNum ' | " VersionNum ")
-- [80] EncodingDecl ::= S 'encoding' Eq ('"' EncName '"' |  "'" EncName "'" )
-- [81] EncName ::= [A-Za-z] ([A-Za-z0-9._] | '-')*
::method scanTextDecl
  expose documentHandler

  currentEntity = self~currentEntity

  do label attributes
      -- skip any whitespace before the attributes...the space is
      -- a required element, so remember if we saw this
      self~requiredWhiteSpace("A space is required before attribute names")

      -- scan off an attribute value
      attr = self~scanPseudoAttribute
      -- the first attribute MUST be the version
      if attr[1] \= "version" then self~fatalError("Unrecognized xml declaration attribute" attr[1])
      -- validate the version information
      self~validateVersion(attr[2])
      currentEntity~version = attr[2]

      hadSpace = self~skipWhiteSpace(.false)
      -- if we're at a question mark, this is the end
      if self~peekCharacter(.false) == '?' then leave attributes
      -- should be an attribute separated by a space here
      if \hadSpace then self~fatalError("A space is required before attribute names")
      -- scan off an attribute value
      attr = self~scanPseudoAttribute
      -- this one can be an encoding or the standalone attribute.  If it is the encoding,
      -- then it can be followed by standalone.  If standalone, that must be the last one.
      if attr[1] == "encoding" then do
          currentEntity~encoding = attr[2]
          self~skipWhiteSpace(.false)
      end
      else self~fatalError("Unrecognized xml declaration attribute" attr[1])
  end

  -- we should be at the delimiter next
  self~skipWhiteSpace(.false)
  -- if we're at a question mark, this is the end
  if self~checkDelimiter('?>', .false) then self~fatalError("Missing ?> terminator for xml declaration")
  -- notify the document handler of what we have
  documentHandler~textDecl(currentEntity~encoding, currentEntity~standalone)


-- scan a pseudo attribute name value pair for an xml/text decl.  This
-- is like an attribute, but there are simpler rules for scanning the
-- attribute quoted string value
::method scanPseudoAttribute
  expose tokenBuffer currentEntity

  name = self~scanName(.false)   -- get the name part, must be in same entity
  if name == "" then self~fatalError(self~ATTRIBUTE_NAME_EXPECTED_ERR)
  self~skipWhiteSpace(.false)
  if \self~checkDelimiter("=", .false) then self~fatalError(self~ATTRIBUTE_EQUAL_EXPECTED_ERR)
  self~skipWhiteSpace(.false)

  -- clear out our accumulator buffer
  tokenBuffer~setBufferSize(0)

  -- get the first item, with no entity switching.
  quote = currentEntity~scanCharacter(.false)
  if quote \== "'" & quote \== '"' then self~fatalError(self~OPEN_QUOTE_EXPECTED_ERR)
  -- perform the scan.  We're only looking for the closing quote here
  ch = currentEntity~scanPseudoLiteral(quote, tokenBuffer)
  -- if this was terminated by the same quote we started with, then we're done. The
  -- scanning will not cross entity boundaries, so this was contained within the entity,
  -- which is common (and what we needed)
  if ch \= quote then self~fatalError(self~CLOSE_QUOTE_EXPECTED_ERR)
  -- normalize the whitespace
  tokenBuffer~translate('   ', '090a0d'x)
  value = tokenBuffer~string
  -- return a composite value
  return .array~of(name, value)


-- scan a single attribute name value pair
::method scanAttribute
  name = self~scanName(.false)   -- get the name part
  if name == "" then self~fatalError(self~ATTRIBUTE_NAME_EXPECTED_ERR)
  self~skipWhiteSpace(.false)
  if \self~checkDelimiter("=", .false) then self~fatalError(self~ATTRIBUTE_EQUAL_EXPECTED_ERR)
  self~skipWhiteSpace(.false)
  value = self~scanQuotedString
  -- return a composite value
  return .array~of(name, value)

-- scan off an element attribute,adding it to the set of attributes
::method scanElementAttribute
  use strict arg attributes

  -- scan off an individual attribute
  attr = self~scanAttribute
  attributes[attr[1]] = attr[2]

-- scan off a processing instruction
::method scanProcessingInstruction
  expose documentHandler

  target = self~scanNCName(.false)  -- this cannot be a qname, so use the ncname subset
  if target == "" then self~fatalError("Missing required processing instruction target")

  if target~lower == "xml" then self~fatalError("xml is a reserved processing instruction target")

  -- there should be a space before the data...but that is optional.  We might be
  -- at the closing delimiter
  if \self~skipWhiteSpace(.false) then do
      --  we're done if we hit a delimiter, but
      -- a space is required before the data
      if \self~checkDelimiter("?>", .false) then self~fatalError(self~SPACE_REQUIRED_BEORE_ERR)
      documentHandler~processingInstruction(target, "")
      return
  end
  -- scan until we hit the delimiter
  data = self~scanData("?>")
  documentHandler~processingInstruction(target, data)

-- scan off a comment
::method scanComment
  expose documentHandler

  -- scan the data looking for "--".  This sequence is
  -- invalid in comments, so if the next character is not
  -- the final ">", this is an error
  data = self~scanData("--")
  if \self~checkDelimiter('>', .false) then self~reportFatalError("Character sequence '--' is not allowed in XML comments")
  documentHandler~comment(data)

-- scan an external id value
::method scanExternalId
  info = .directory~new
  if self~checkDelimiter("PUBLIC", .false) then do
      self~requiredWhiteSpace("Space required before PUBLIC id value")
      info~publicid = self~scanPublicId
      self~requiredWhiteSpace("Space required before external system id value")
      info~systemId = self~scanQuotedString
  end
  else if self~checkDelimiter("SYSTEM", .false) then do
      self~requiredWhiteSpace("Space required before external system id value")
      info~systemId = self~scanQuotedString
  end
  -- return the composite value
  return info

-- scan a starting element
::method scanStartElement
  expose documentHandler elementQueue currentEntity

  attributes = .table~new
  -- scan off the qualified name
  qname = self~scanQName(.false)
  -- not an element
  emptyElement = .false

  loop forever
      hadSpace = self~skipWhiteSpace(.false)
      -- end of the tag, stop scanning
      if self~checkDelimiter('>', .false) then leave
      -- end marker
      else if self~checkDelimiter('/', .false) then do
          if \self~checkDelimiter('>', .false) then self~reportFatalError("Missing '>' terminator in element" qname)
          emptyElement = .true   -- this is an empty one
          leave
      end
      -- this must be a namestart character for an attribute name
      -- and it must have been preceded by a space
      if \hadSpace then self~reportFatalError("Missing space before element attribute")
      -- scanElementAttribute handles all other validity checks
      self~scanElementAttribute(attributes)
  end

  -- send the appropriate event type
  if emptyElement then documentHandler~emptyElement(qname, attributes)
  else do
      documentHandler~startElement(qname, attributes)
      -- push the queue name to the front of the element queue.  We also
      -- add the entity so we can check that the end tag is defined at the same
      -- entity level
      elementQueue~push(.array~of(qname, currentEntity))
  end

-- scan an element end tag
::method scanEndElement
  expose documentHandler elementQueue currentEntity

  endTagEntity = currentEntity
  -- scan off a qname for this tag
  name = self~scanQName(.false)

  self~skipWhiteSpace(.false)
  if \self~checkDelimiter('>', .false) then self~reportFatalError("Missing terminator on element end tag" name)

  -- get the current element context
  lastElement = elementQueue~pull
  if lastElement == .nil then self~reportFatalError("Unexpected element end tag" name)

  if lastElement[1] \== name then self~reportFatalError("Element end tag" name "does not match last element" lastElement[1])
  -- these must be defined in the same entity
  if endTagEntity \== lastElement[2] then self~reportFatalError("Start and end tags for element" name "are not in the same document entity")

  -- tell the document handler
  documentHandler~endElement(name)

-- scan a CData element
::method scanCDataSection
  expose documentHandler

  documentHandler~startCData

  data = self~scanCData
  documentHandler~characters(data)

  documentHandler~endCData

-- scan a character reference encountered in content
::method scanCharacterReference
  expose documentHandler

  -- decode this value
  value = self~scanCharacterReferenceValue
  -- indicate we're working on an entity, passing the literal value
  -- pulled from the xml.
  documentHandler~startGeneralEntity(value)

  -- send the character as part of the character stream
  documentHandler~characters(value)
  -- and mark the end of the entity
  documentHandler~endGeneralEntity(value)

-- scan a named entity reference
::method scanEntityReference
  name = self~scanEntityReferenceName

  -- built-in entities are special
  if name == "amp" then self~handleBuiltinEntity('&', "amp")
  else if name == "lt" then self~handleBuiltinEntity('<', "lt")
  else if name == "gt" then self~handleBuiltinEntity('>', "gt")
  else if name == "quot" then self~handleBuiltinEntity('"', "quot")
  else if name == "apos" then self~handleBuiltinEntity("'", "apos")
  else do
      -- validate this entity as being known
      -- TODO:  Write a mixin that handles entities also
      self~handleEntity(name)
  end

-- scan an entity reference name value, without processing
-- anything about the name
::method scanEntityReferenceName
  expose currentEntity

  -- don't allow an entity boundary switch here.  This must be part of
  -- the same entity as the "&"
  name = currentEntity~scanName(.false)
  if name == "" then self~reportFatalError("Missing entity name")
  -- again, this must be part of the same entity.
  if \currentEntity~checkDelimiter(';', .false) then self~reportFatalError("Missing ';' terminator for entity name" name)
  return name


-- scan some sort of coded character reference value in a string.  We're handed
-- a mutable buffer and an offset.  On completion, the reference is replaced
-- with the appropriate value and appended to the second buffer.  The return
-- value is the offset of the first
-- position after the replacement.

-- NOTE:  we're already past the "&#" marking the start of the entity and
-- ready to translate the value to convert
::method scanCharacterReferenceValue
  expose currentEntity
  use strict arg

  -- get the first char type
  ch = currentEntity~scanCharacter(.false)
  -- in hex
  if ch == 'x' then do
      hexChars = ""
      loop forever
          ch = currentEntity~scanCharacter(.false)
          if ch~matchChar(1, .XmlChar~HEXDIGIT) then hexChars = hexChars||ch
          else if ch == ";" then leave
          else self~reportFatalError("Invalid hex character '"ch"' in character reference value")
      end
      if hexChars == "" then self~reportFatalError("Missing character value in decimal character reference")
      value = hexChars~x2c
      if value~length > 1 then self~reportFatalError("Invalid character value in hex character reference: '"hexChars"'")
  end
  -- encoded in decimal
  else if ch == 'd' then do
      numberChars = ""
      loop forever
          ch = currentEntity~scanCharacter(.false)
          if ch~matchChar(1, .XmlChar~DIGIT) then numberChars = numberChars||ch
          else if ch == ";" then leave
          else self~reportFatalError("Invalid decimal character '"ch"' in character reference value")
      end
      if numberChars == "" then self~reportFatalError("Missing character value in decimal character reference")
      value = numberChars~d2c
      if value~length > 1 then self~reportFatalError("Invalid character value in decimal character reference: '"numberChars"'")
  end
  else self~reportFatalError("Invalid character reference type: '"ch"'")

  return value

-- process a reference to a built-in entity
::method handleBuiltinEntity
  expose documentHandler
  use strict arg ch, name

  documentHandler~startGeneralEntity(name)
  documentHandler~characters(ch)
  documentHandler~endGeneralEntity(name)

-- parse the initial XML declaration in a document
::method parseXmlDecl
  -- have a potential '<?xml' declaration?
  if check~delimiter('<?', .true) then do
      name = self~scanNCName(.false)
      -- xml is a special name, that requires special handling
      if name == "xml" then self~scanXmlDecl
      -- could be a processing instruction, so handle it here
      -- before starting on the prolog
      else self~scanProcessingInstruction(name)
  end

-- parse the document prolog between the XML decl and the first element tag
::method parseProlog
  expose currentEntity
  -- loop until we find the start of a real element
  seenDoctype = .false

  loop forever
      -- A comment.  These are ok in the prolog
      if self~checkDelimiter('<!--', .true) then self~scanComment
      else if self~checkDelimiter('<!', .true) then do
          -- this must be '<!DOCTYPE'
          name = self~scanName(.false)
          if name \== "DOCTYPE" then self~reportFatalError("Unrecognized markup: '!"name"'")
          if seenDoctype then self~reportFatalError("Only one !DOCTYPE declaration permitted in a document")
          seenDoctype = .false
          -- scan the doctype instruction itself
          self~scanDoctypeDecl
          -- TODO:  Lots of processing needed for external DTDs.
          self~scanDoctypeDecls
      end
      -- we've already seen the xml decl, so this must be a processing instruction
      else if currentEntity~checkDelimiter('<?', .true) then
          self~scanProcessingInstruction
      -- a reference just sitting out there?
      else if currentEntity~checkDelimiter('&', .true) then self~reportFatalError("Content not valid in document prolog")
      -- this is potentially the root element.  We've already checked other forms
      -- of markup.  If the next character is a namestart, this is an element and we
      -- exit to start the main processing loop
      else if self~checkElementStart then return
      -- hit an eof before the root element
      else if currentEntity~peekCharacter(.false) == '' then self~reportFatalError("No root element in document")
      else self~reportFatalError("Content not valid in document prolog")
  end

-- parse the main body of the document once the first element tag is encountered
::method parseDocumentContent
  expose elementStack currentEntity


  loop forever
      -- A comment.  These are ok anywhere
      if currentEntity~checkDelimiter('<!--', .true) then self~scanComment
      else if currentEntity~checkDelimiter('<!', .true) then do
          -- this must be '<!DOCTYPE'
          name = self~scanName(.false)
          if name == "DOCTYPE" then self~reportFatalError("!DOCTYPE declaration not permitted after root element")
          else
      end
      -- we've already seen the xml decl, so this must be a processing instruction
      else if currentEntity~checkDelimiter('<?', .true) then
          self~scanProcessingInstruction
      -- handle the different reference types
      else if currentEntity~checkDelimiter('&', .true) then do
          if currentEntity~checkDelimiter('#', .false) then self~scanCharacterReference
          else self~scanEntityReference
      end
      -- this is potentially an element.  If it is an element form, then
      -- handle this.  If this is not an empty element, this will call
      -- us recursively to handle its contents.
      else if currentEntity~checkElementStart then self~scanStartElement
      -- could (should) be the end to our current context.  If this is
      -- the main processing loop, we don't have an active element yet, so this
      -- could be an error
      else if currentEntity~checkDelimiter('</', .true) then self~scanEndElement
      -- might actually be an eof...quit the processing loop now
      else if self~peekCharacter(.true) == '' then do
          -- this is most likely true, since we terminate the loop as soon as
          -- the stack is empty
          if \elementStack~isEmpty then self~reportFatalError("Unterminated element in document:" elementStack~peek~element~nodeName)
          leave
      end
      -- scan any other content
      else do
          -- if this is the first main document call, we can't have content yet
          if \elementStack~isEmpty then self~reportFatalError("Content is not valid before root element")
          -- processing an active element, so this is good
          self~scanContent
      end
      -- we only enter here because we've found the root element tag.  If
      -- we no longer have any elements active, this should be the end of the
      -- document
      if elementStack~isEmpty then leave
  end

-- ensure there's nothing after the close tag for the root element
::method parseTrailing
  self~skipWhiteSpace(.true)
  if self~peekCharacter(.true) \== '' then self~reportFatalError("Illegal content after root element")

-- scan the doctype declaration element itself
::method scanDocTypeDecl
  expose currentEntity documentHandler

  if \currentEntity~skipWhiteSpace(.false) then self~reportFatalError("Space required before !DOCTYPE root element name")
  -- root name is required
  rootElementName = self~scanName(.false)
  if rootElementName == "" then self~reportFatalError("Missing !DOCTYPE root element name")
  -- get the id information
  systemId = .nil
  publicId = .nil
  if currentEntity~skipWhiteSpace(.false) then do
      ids = self~scanExternalId
      systemId = ids~systemId
      publicId = ids~publicId
  end

  -- allow the doctype handler to process this part
  documentHandler~doctypeDecl(rootName, publicId, systemId)

  currentEntity~skipWhiteSpace(.false)
  -- do we have an internal subset?  Then go secan it
  if currentEntity~checkDelimiter('[') then do
      self~scanDoctypeDecls
  end
  else do
      if \currenEntity~checkDelimiter('>') then self~reportFatalError("Unterminated !DOCTYPE declaration")
  end


-- scan the various Decl types for a DocType
::method scanDocTypeDecls
  expose conditionalDepth includeDepth documentHandler currentEntity
  use strict arg scanningInclude = .false

  loop forever
      -- skip over whitespace and process any PErefs
      self~skipDeclSep
      -- some sort of markup?
      if currentEntity~checkDelimiter('<', .true) then do
          -- looking at a processing instruction?
          if currentEntity~checkDelimiter('?', .false) then self~scanProcessingInstruction
          -- a lot of different DTD markup begin with '<!'
          else if currentEntity~checkDelimiter('!', .false) then do
              -- comments are "<!--"
              if currentEntity~checkDelimiter('--', .false) then self~scanDtdComment
              -- <!ELEMENT declaration
              else if currentEntity~checkDelimiter("ELEMENT", .false) then self~scanElementDecl
              -- <!ATTLIST declaration
              else if currentEntity~checkDelimiter("ATTLIST", .false) then self~scanAttListDecl
              -- <!ENTITY declaration
              else if currentEntityt~checkDelimiter("ENTITY", .false) then self~scanEntityDecl
              -- <!NOTATION declaration
              else if currentEntity~checkDelimiter("NOTATION", .false) then self~scanNotationDecl
              -- <![ conditonal.  Only valid if not an internal subset.  Note:  conditional sections CAN nest.
              else if \self~scanningInternalSubset, currentEntity~checkDelimiter("[", .true) then self~scanConditionalSection
              else self~reportFatalError("Unrecognized DTD markup")
          end
          else self~reportFatalError("Unrecognized DTD markup")
      end
      -- have we been called recursively to process the statements inside of an
      -- include section?  Then check for terminator.  The .true return indicates we found what we're looking for
      else if scanningInclude, currentEntity~checkDelimiter(']]>', .true) then return .true
      -- if we're handling an internal subset, we've hit the end of what we're parsing
      else if self~scanningInternalSubset, currentEntity~checkDelimiter(.true) == ']' then return .true
      -- end of data?  Terminate the loop
      else if currentEntity~peekCharacter(.true) == "" then return .false
      else self~reportFatalError("Unrecognized DTD markup")
  end

-- scan a comment in a DTD definition
::method scanDtdComment
  expose dtdHandler

  -- just scan for the closing delimiter
  data = self~scanData("-->")
  -- this is handled by the dtdHandler
  dtdHandler~dtdComment(data)

-- scan an element declaration in a DTD
::method scanElementDecl
  expose dtdHandler currentEntity

  self~requiredDeclSep("Space required before !ELEMENT type")
  typename = self~scanName(.true)
  contentModel = .nil -- no extra information available by default
  if typename == '' then self~reportFatalError("Missing !ELEMENT type name")
  self~requiredDeclSep("Space required before !ELEMENT content specification")
  -- indicate we're processing a new element
  dtdHandler~startContentModel(name)
  if currentEntity~checkDelimiter("EMPTY", .false) then do
      contentType = "EMPTY"
      dtdHandler~empty
  end
  if currentEntity~checkDelimiter("ANY", .false) then do
      contentType = "ANY"
      dtdHandler~any
  end
  else do
      if \currentEntity~checkDelimiter('(', .false) then self~reportFatalError("Invalid !ELEMENT type")
      dtdHandler~startGroup
      self~skipDeclSep
      if currentEntity~checkDelimiter('#PCDATA', .false) then do
          contentType = "MIXED"
          contentModel = self~scanMixedContentModel
      end
      else do
          contentType = "CHILDREN"
          contentModel = self~scanChildTypes
      end
  end

  dtdHandler~endContentModel
  self~skipDeclSep
  if \currentEntity~checkDelimiter('>', .false) then self~reportFatalError("Unterminated !ELEMENT specification")
  dtdHandler~elementDecl(name, contentType, contentModel)

-- scan a mixed content model, defined as
--  [51]    Mixed    ::=    '(' S? '#PCDATA' (S? '|' S? Name)* S? ')*'
--                        | '(' S? '#PCDATA' S? ')'
::method scanMixedContentModel
  expose dtdHandler currentEntity

  dtdHandler~startGroup
  dtdHandler~pcData

  self~skipDeclSep
  seq = .XMLTypeChoice~new

  loop while currentEntity~checkDelimiter('|', .false)
      dtdHandler~separator('|')
      self~skipDeclSep
      -- the child name can be a directly specified name or a Parameter Entity
      -- reference
      childName = self~scanNameOrParameterEntityRef
      if childName == "" then self~reportFatalError("Missing !ELEMENT type name")
      seq~append(childName)
      dtdHandler~elementType(childName)
      self~skipDeclSep
  end

  -- the parens can be in a different entity, so allow this to pop
  if currentEntity~checkDelimiter(')*', .true) then do
      dtdHandler~endGroup
      dtdHandler~occurrence(.XMLContentModelHandler~ZERO_OR_MORE)
  end
  else if childName \= .nil then self~reportFatalError("Unterminated !ELEMENT mixed content specification")
  else if currentEntity~checkDelimiter(')', .true) then dtdHandler~endGroup
  else self~reportFatalError("Missing closing parentheses in !ELEMENT mixed content specification")
  return seq

-- scan child types in an element decl.  These can be nested lists of child types
--   [47]    children  ::=    (choice | seq) ('?' | '*' | '+')?
--   [48]    cp        ::=    (Name | choice | seq) ('?' | '*' | '+')?
--   [49]    choice    ::=    '(' S? cp ( S? '|' S? cp )+ S? ')'
--   [50]    seq       ::=    '(' S? cp ( S? ',' S? cp )* S? ')'
::method scanChildTypes
  expose dtdHandler currentEntity

  -- since there are nested groups, we need to keep track of
  -- what sort of choice operators we're dealing with
  operatorStack = .queue~new
  currentGroup = .nil  -- this is the current group we add elements to
  firstChild = .nil

  groupDepth = 0   -- keep track of the groupings
  loop forever
      self~skipDeclSep
      currentEntity~skipWhiteSpace(.false)
      -- start of a new group (or potentially, the first one)
      if currentEntity~checkDelimiter('(', .false) then do
          -- start a new group
          dtdHandler~startGroup
          groupDepth += 1
          -- reset the operator to none until we find a new one
          operatorStack~push(currentGroup)
          currentGroup = .nil
      end
      else do
          childname = self~scanNameOrParameterEntityRef
          if childname == "" then self~reportFatalError("Missing !ELEMENT child element type")
          dtdHandler~element(childname)
          -- check for modifiers on the child type
          if currentEntity~checkDelimiter('?', .false) then do
              dtdHandler~occurrence(.XMLContentModelHandler~ZERO_OR_ONE)
              child = .ZeroOrOne~new(childName)
          end
          else if currentEntity~checkDelimiter('*', .false) then do
              dtdHandler~occurrence(.XMLContentModelHandler~ZERO_OR_MORE)
              child = .ZeroOrMore~new(childName)
          end
          else if currentEntity~checkDelimiter('+', .false) then do
              dtdHandler~occurrence(.XMLContentModelHandler~ONE_OR_MORE)
              child = .OneOrMore~new(childName)
          end
          else child = .SingleChild~new(childName)
          -- if we don't know what sort of group this is yet, hold on to the child element
          -- until we see the next bit
          if currentGroup == .nil then firstChild = child
          else currentGroup~append(child)
      end
      -- we're in a group, now continue with the choice or sequence
      loop forever
          self~skipDeclSep
          -- processing a sequence?  We only allow this if we're not currently
          -- handling a choice expression
          if \currentGroup~isA(.TypeChoice), currentEntity~checkDelimiter(',', .true) then do
              -- this might be the first type indicator, so we can finally create the
              -- group instance and add the first type
              if currentGroup == .nil then do
                  currentGroup = .TypeSequence~new
                  currentGroup~append(firstChild)
                  firstChild = .nil
              end
              dtdHandler~separator(.XMLContentModelHandler~SEQUENCE)
              leave   -- done processing
          end
          -- the opposite situation
          else if \currentGroup~isA(.TypeSequence), currentEntity~checkDelimiter('|', .true) then do
              -- this might be the first type indicator, so we can finally create the
              -- group instance and add the first type
              if currentGroup == .nil then do
                  currentGroup = .TypeChoice~new
                  currentGroup~append(firstChild)
                  firstChild = .nil
              end
              dtdHandler~separator(.XMLContentModelHandler~CHOICE)
              leave   -- done processing
          end
          -- the close of a group
          else if currentEntity~checkDelimiter(')', .true) then do
              groupDepth -= 1
              dtdHandler~endGroup
              -- we've hit the end, now look to see if there
              -- is a repetition modifier (NOTE, these must be in the same
              -- entity as the terminator
              if currentEntity~checkDelimiter('?', .false) then do
                  currentGroup~repetitor = .XMLContentModelHandler~ZERO_OR_ONE
                  dtdHandler~occurrence(.XMLContentModelHandler~ZERO_OR_ONE)
              end
              else if currentEntity~checkDelimiter('+', .false) then do
                  currentGroup~repetitor = .XMLContentModelHandler~ONE_OR_NORE
                  dtdHandler~occurrence(.XMLContentModelHandler~ONE_OR_MORE)
              end
              else if currentEntity~checkDelimiter('*', .false) then do
                  currentGroup~repetitor = .XMLContentModelHandler~ZERO_OR_NORE
                  dtdHandler~occurrence(.XMLContentModelHandler~ZERO_OR_MORE)
              end
              -- we've unwound all of the groups, we're done parsing this
              if groupDepth = 0 then return currentGroup
              -- back up to the previous group
              currentGroup = operatorStack~pull
          end
          -- something invalid here
          else self~reportFatalError("Invalid marking up in element content type")
      end
  end

-- scan an ATTLIST declaration
--  [52]  AttlistDecl    ::=   '<!ATTLIST' S Name AttDef* S? '>'
--  [53]  AttDef         ::=   S Name S AttType S DefaultDecl
::method scanAttListDecl
  expose dtdHandler currentEntity

  self~requiredDeclSep("Space required before !ATTLIST element name")
  elementName = self~scanName(.false)
  if elementName == "" then self~reportFatalError("Missing !ATTLIST element name")
  dtdHandler~startAttList(elementName)

  -- white space is optional if this is the end already.  This essentially declares
  -- that the named element doesn't have any attributes
  if \self~skipDeclSep then do
      -- is this the end?
      if currentEntity~checkDelimiter('>', .false) then do
          -- tell the handler
          dtdHandler~endAttList
          return -- we're done
      end
      self~reportFatalError("Space required before !ATTLIST attribute name")
  end

  -- now loop through the attribute definitions until we hit the markup end
  loop while \self~checkDelimiter('>', .true)
      attributeName = self~scanName(.true)
      if attributeName == "" then self~reportFatalError("Missing !ATTLIST attribute name")
      self~requiredDeclSep("Space required before !ATTLIST attribute type")
      attributeType = self~scanAttributeType(elementName, attributeName)
      self~requiredDeclSep("Space required before !ATTLIST attribute type")
      defaultType = self~scanAttributeDefault(elementName, attributeName, attributeType)

      dtdHandler~attributeDecl(elementName, attributeName, .nil, attributeType, defaultType, .nil, .nil)
      currentEntity~skipWhiteSpace(.true)
  end

  dtdHandler~endAttList

-- scan the attribute type from an attlist decl
--  [54]  AttType        ::=   StringType | TokenizedType | EnumeratedType
--  [55]  StringType     ::=   'CDATA'
--  [56]  TokenizedType  ::=   'ID'
--                           | 'IDREF'
--                           | 'IDREFS'
--                           | 'ENTITY'
--                           | 'ENTITIES'
--                           | 'NMTOKEN'
--                           | 'NMTOKENS'
--  [57]  EnumeratedType ::=    NotationType | Enumeration
--  [58]  NotationType ::= 'NOTATION' S '(' S? Name (S? '|' S? Name)* S? ')'
--  [59]  Enumeration    ::=    '(' S? Nmtoken (S? '|' S? Nmtoken)* S? ')'
::method scanAttributeType
  expose currentEntity

  typeInfo = .directory~new  -- we'll return all of the information using a directory
  -- NOTE:  These don't use a delimiter, so for this to work, when there are
  -- tokens that are substrings of longer tokens, the longest string needs to be
  -- checked first.  For example, IDREFS must be checked before IDREF and IDREF
  -- before ID
  if currentEntity~checkDelimiter('CDATA', .false) then typeInfo~type = 'CDATA'
  else if currentEntity~checkDelimiter('IDREFS', .false) then typeInfo~type = 'IDREFS'
  else if currentEntity~checkDelimiter('IDREF', .false) then typeInfo~type = 'IDREF'
  else if currentEntity~checkDelimiter('ID', .false) then typeInfo~type = 'ID'
  else if currentEntity~checkDelimiter('ENTITY', .false) then typeInfo~type = 'ENTITY'
  else if currentEntity~checkDelimiter('ENTITIES', .false) then typeInfo~type = 'ENTITIES'
  else if currentEntity~checkDelimiter('NMTOKENS', .false) then typeInfo~type = 'NMTOKENS'
  else if currentEntity~checkDelimiter('NMTOKEN', .false) then typeInfo~type = 'NMTOKEN'
  -- notation is a more complex item
  else if currentEntityself~checkDelimiter('NOTATION', .false) then do
      typeInfo~type = 'NOTATION'
      self~requiredDeclSep("Space required after NOTATION attribute type")
      if \currentEntity~checkDelimiter('(', .false) then self~reportFatalError("Missing open parenthesis for NOTATION attribute list")
      -- loop until we've used all of the choice values
      typeInfo~values = .array~new
      loop until \currentEntity~checkDelimiter('|', .false)
          self~skipDeclSep
          attributeName = self~scanName(.false)
          if attributeName == "" then self~reportFatalError("Missing NOTATION attribute name")
          -- add this to the notation list
          typeInfo~values~append(attributeName)
          self~skipDeclSep
      end
      if \currentEntity~checkDelimiter(')', .false) then self~reportFatalError("Missing closing parenthese for NOTATION attribute list")
  end
  -- an enumeration of values
  else do
      typeInfo~type = 'ENUMERATIONS'
      typeInfo~values = .array~new
      if \currentEntity~checkDelimiter('(', .false) then self~reportFatalError("Missing opening parentheses for attribute enumeration type")
      loop until \currentEntity~checkDelimiter('|', .false)
          self~skipDeclSep
          type = self~scanNmToken(.false)
          if type == "" then self~reportFatalError("Missing attribute enumeration type name")
          -- add this to the notation list
          typeInfo~values~append(type)
          self~skipDeclSep
      end
      if \currentEntity~checkDelimiter(')', .false) then self~reportFatalError("Missing closing parentheses on attribute enumeration list")
  end
  return typeInfo

-- scan an attribute default value
--  [60] DefaultDecl ::= '#REQUIRED' | '#IMPLIED' | (('#FIXED' S)? AttValue)
::method scanAttributeDefault
  expose currentEntity

  defaultInfo = .directory~new

  if currentEntity~checkDelimiter("#REQUIRED", .false) then defaultInfo~defaultType = "#REQUIRED"
  else if currentEntity~checkDelimiter("#IMPLIED", .false) then defaultInfo~defaultType = "#IMPLIED"
  else do
      if currentEntity~checkDelimiter("#FIXED", .false) then do
          defaultInfo~defaultType = "#FIXED"
          self~requiredDeclSep("Missing space after #FIXED default attribute value")
      end
      defaultInfo~defaultValue = self~scanQuotedString
  end
  return defaultinfo

-- scan an entity declaration
--  [70]    EntityDecl  ::=    GEDecl | PEDecl
--  [71]    GEDecl      ::=    '<!ENTITY' S Name S EntityDef S? '>'
--  [72]    PEDecl      ::=    '<!ENTITY' S '%' S Name S PEDef S? '>'
--  [73]    EntityDef   ::=    EntityValue | (ExternalID NDataDecl?)
--  [74]    PEDef       ::=    EntityValue | ExternalID
--  [75]    ExternalID  ::=    'SYSTEM' S SystemLiteral
--                           | 'PUBLIC' S PubidLiteral S SystemLiteral
--  [76]    NDataDecl   ::=    S 'NDATA' S Name
::method scanEntityDecl
  expose dtdHandler currentEntity

  entityType = "&"  -- default to a normal entity
  self~requiredDeclSep("Space required before !ENTITY entity name")
  -- is this a paramenter entity?
  peRef = .false
  if currentEntity~checkDelimiter('%', .false) then do
      self~requiredDeclSep("Space required before !ENTITY parameter entity name")
      peRef = .true   -- remember this is a PE ref
  end
  -- get the name, and aug
  name = self~scanNCName(.false)
  if name == "" then self~reportFatalError("Missing !ENTITY name")
  if peRef then name = '%'name
  self~requiredDeclSep("Space required after !ENTITY name")
  externalId = self~scanExternalId
  notation = .nil
  hadSpace = self~skipDeclSep
  if currentEntity~checkDelimiter('NDATA', .false) then do
      -- space is required if NDATA was there
      if \hadSpace then self~reportFatalError("Space required before !ENTITY NDATA definition")
      self~requiredDeclSep("Space required before !ENTITY NDATA notation name")
      notation = self~scanName(.false)
      if notation == "" then self~reportFatalError("Missing !ENTITY NDATA notation name")
  end

  -- if no system id, we expect to find an internal value
  if externalId~systemId == .nil then entityValue = self~scanEntityValue

  self~skipDeclSep
  if \self~checkDelimiter('>', .false) then self~reportFatalError("Unterminated !ENTITY NDATA notation name")

  if externalId~systemId \== .nil then do
      baseSystemId = self~baseSystemId
      if notation \== .nil then do
          self~addUnparsedEntity(name, externalID~publicId, externalID~systemId, baseSystemId, notation)
          dtdHandler~unparsedEntityDecl(name, externalID~publicId, externalID~systemId, baseSystemId, notation)
      end
      else do
          self~addExternalEntity(name, externalID~publicId, externalID~systemId, baseSystemId)
          dtdHandler~externalEntityDecl(name, externalID~publicId, externalID~systemId, baseSystemId)
      end
  end
  else do
      self~addInternalEntity(name, entityValue~normalized)
      dtdHandler~internalEntityDecl(name, entityValue~normalized, entityValue~unnormalized)
  end

-- parse out an entity value, returning both the raw and normalized versions
::method scanEntityValue
  expose currentEntity tokenBuffer

  tokenBuffer~setBufferSize(0)
  normalizedValue = .mutableBuffer~new   -- the normalized version
  unnormaliziedValue = .mutableBuffer~new  -- raw version of the literal without refs handled
  quote = currentEntity~scanCharacter(.false)
  if quote \== '"' & quote \== "'" then self~reportFatalError("Missing quote in !ENTITY value")

  buffer = .mutableBuffer~new
  -- this literal might have PE reference values contained within it.  We
  -- might need to consume this in chunks
  ch = self~scanLiteral(quote, buffer)
  -- loop until we hit the quote terminator
  loop while ch \== quote
      -- transfer everything from our current buffer to the accumulators
      current = buffer~string
      normalizedValue~append(current)
      unnormalizedValue~append(current)
      buffer~setBufferSize(0)

      -- normal entity reference
      if ch == '&' then do
          -- a character reference
          if self~checkDelimiter('#', .false) then do
              -- scan the character reference, getting both the raw
              -- version and the translated value
              value = self~scanCharacterReferenceValue
              normalizedValue~append(append)
              unnormalizedValue~append(append)
          end
          -- named entity reference.  Just get the name and append to both
          -- values.  The references get resolved when the value is used and scanned
          else do
              name = self~scanEntityReferenceName
              normalizedValue~append('&'refs[1]';')
              unnormalizedValue~append('&'refs[2]';')
          end
      end
      -- a parameter entity reference
      else if ch == '%' then do
          -- this one is only added to the unnormalized version.  It is null for now
          name = self~scanEntityReferenceName
          unnormalizedValue~append('%'refs[2]';')
          -- TODO:  I think there is additional handling required here.
      end
      -- try for the next segment.  We might find the quote this time
      ch = self~scanLiteral(quote, buffer)
  end

  value = .directory~new
  value~normalized = normalizedValue~string
  value~unnormalized = unnomalizedValue~string
  return value

-- scan a notation declaration
--  [82] NotationDecl ::= '<!NOTATION' S Name S (ExternalID|PublicID) S? '>'
--  [83]  PublicID    ::= 'PUBLIC' S PubidLiteral
::method scanNotationDecl
  expose dtdHandler currentEntity
  self~requiredDeclSep("Space required before !NOTATION name")
  name = currentEntity~scanName(.false)
  if name == "" then self~reportFatalError("Missing !NOTATION name")
  self~requiredDeclSep("Space required after !NOTATION name")
  info = self~scanExternalId
  baseSystemId = self~baseSystemId

  if info~systemId == .nil & info~publicId == .nil then self~reportFatalError("Missing !NOTATION public or system id")
  self~skipDeclSep
  if \self~checkDelimiter('>', .false) then self~reportFatalError("Unterminated !NOTATION declaration")
  dtdHandler~notationDecl(name, info~systemId, info~publicId, baseSystemId)

-- scan a conditional section
::method scanConditionalSection
  expose dtdHandler currentEntity tokenBuffer

  -- save this depth.  While it is normal for the INCLUDE/IGNORE of the
  -- conditional section to be defined in a PE Ref, both sides of the
  -- keyword MUST be part of the same entity.
  startEntity = currentEntity
  -- NOTE:  this is one place where looking for a PEref is critical, as
  -- the INCLUDE/IGNORE keyword is typically defined in a PE ref.
  self~skipDeclSep
  -- is this section to be included?
  if currentEntity~checkDelimiter('INCLUDE', .false) then do
      -- skip any white space...and also give this a chance
      -- to switch back to the initiating entity
      self~skipDeclSep
      if currentEntity \== startEntity then self~reportFatalError("Invalid parameter entity reference in conditional section")
      -- must have the delimiter marker here
      if \currentEntity~checkDelimiter('[', .false) then self~reportFatalError("Unrecognized DTD markup")
      -- inform the DTD handler we're starting this...the DTD handler will drive the rest of the processing
      dtdHandler~startConditional("INCLUDE")
      -- now recursively scan the section for additional statements
      -- the argument causes this to recognize the conditional terminator
      if \self~scanDocTypeDecls(.true) then self~reportFatalError("Unterminated conditional INCLUDE section")
      if startEntity \== currentEntity then self~reportFatalError("Parameter entity within a conditional section cannot contain a ]]> terminator")
      -- give the termination indicator
      dtdHandler~endConditional("INCLUDE")
  end
  -- section to be ignored
  else if currentEntity~checkDelimiter('IGNORE', .false) then do
      -- skip any white space...and also give this a chance
      -- to switch back to the initiating entity
      self~skipDeclSep
      if currentEntity \== startEntity then self~reportFatalError("Invalid parameter entity reference in conditional section")
      -- must have the delimiter marker here
      if \currentEntity~checkDelimiter('[', .false) then self~reportFatalError("Unrecognized DTD markup")
      -- inform the DTD handler we're starting this...the DTD handler will drive the rest of the processing
      dtdHandler~startConditional("IGNORE")
      -- now perform scanning on the ignore buffer.  We can't just scan for the terminator, but
      -- must also check for disallowed combinations inside the data.

      -- conditional sections can nest, so we need to at least recognize the start of
      -- the nesting so we can keep track of the terminators as well
      nestLevel = 0
      tokenBuffer~setBufferSize(0)
      loop forever
          -- is this a nested conditional section?
          if currentEntity~checkDelimiter('<![') then do
              nestLevel += 1
              tokenBuffer~append('<![')
          end
          else if currentEntity~checkDelimiter(']]>') then do
              tokenBuffer~append(']]>')
              -- reduce the nesting level
              nestLevel -= 1
              -- if we've unwound to zero, then it's time to stop
              if nestLevel = 0 then do
                  -- check that the start and end are at the same entity level
                  if startEntity \== currentEntity then self~reportFatalError("Parameter entity within a conditional section cannot contain a ]]> terminator")
                  -- poke the dtd handler with this stuff
                  dtdHandler~ignoredCharacters(tokenBuffer~string)
                  dtdHandler~endConditional
              end
          end
          else do
              -- get the next character
              ch = self~scanCharacter(.true)
              if ch == '' then self~reportFatalError("Unterminated conditional section")
              tokenBuffer~append(ch)
          end
      end
  end
  else self~reportFatalError("Unrecognized DTD markeup")



-- drive the parsing of the document
::method parseDocument
  -- handle the initial xml decl parsing
  self~parseXmlDecl
  -- handle any comments/processing instructions in the prolog
  -- before the root element
  self~parseProlog
  -- if we return here, then we've recognized the start of the root
  -- element.  We're positioned at the first character, all ready
  self~parseDocumentBody
  -- now verify there is nothing but whitespace following the close of the
  -- root element
  self~parseTrailing

-- this is the object that receives the various parsing events and constructs the DOM tree
::class "DomDocumentHandler"
::method init
  expose document
  document = .nil

-- the document instance that owns this tree
::attribute document get

-- process a comment event
::method comment
  expose document currentNode
  use strict arg text

  -- TODO:  Add filtration
  comment = document~createComment(text)
  currentNode~appendChild(comment)

-- process a processing instruction event
::method processingInstruction
  expose document currentNode
  use strict arg target, data

  -- TODO:  Add filtration
  pi = document~createProcessingInstruction(target, data)
  currentNode~appendChild(pi)

-- handle the beginning of a document
::method startDocument
  expose document
  use strict arg documentURI, encoding

  document = .ooRexxDom~createDocument
  document~documentURI = documentURI
  document~inputEncoding = encoding

-- process the XML declaraton
::method xmlDecl
  expose document
  use strict arg version, encoding, standalone

  if version \== .nil then document~version = version
  document~xmlEncoding = encoding
  document~standalone = standalone

-- handle a doctype declaration
::method doctypeDecl
  expose document currentNode
  use strict arg rootElement, publicID, systemID

  documentType = document~createDocumentType(rootElement, publidID, systemID)
  -- attach to the document node
  currentNode~appendChild(documentType)

-- handle an element start event
::method startElement
  expose document currentNode currentElement
  use strict arg qname, attributes

  -- create a new context for the new element, inheriting the namespace
  -- definitions from the parents.
  newElement = .ElementContext~new(document, qname, attributes, currentElement)

  -- set the root element if this is the first one
  if currentElement == .nil then do
     -- add this as the root element of the document
     document~appendChild(newElement~element)
  end
  -- this is the top of the stack
  currentElement = newElement
  currentNode = currentElement~element

-- handle an empty element.  This is essentially a start followed immediately
-- by an end
::method emptyElement
  use strict arg qname, attributes
  self~startElement(qname, attributes)
  self~endElement(qname)

-- handle an element end
::method endElement
  expose document currentNode currentElement
  use strict arg qname, attributes

  -- the main parser has already validated a name match,
  -- so we just have finalization activities to handle here.

  -- pop this off the stack and make it the new current
  currentElement = currentElement~parent
  currentNode = currentNode~parentNode

-- characters have been encountered.  We will have already
-- received a start for the appropriate type, so we'll just
-- be appending the characters to the node
::method characters
  expose cdataNode currentNode document
  use strict arg text

  -- if we have a current cdataNode that we're accumulating, then
  -- append the data to that
  if cdataNode \= .nil then cdataNode~appendData(text)
  -- handle as a text node
  else do
    -- if the last child of the current element is a text node,
    -- append the data to that node.  Otherwise, create a new text
    -- node and add it as the new last child
    lastChild = currentNode~lastChild
    if lastChild \== .nil, lastChild~nodeType == .Node~TEXT_NODE then
        node~appendData(text)
    else do
        textNode = document~createTextNode(text)
        currentNode~appendChild(textNode)
    end
  end

-- process the start of a CData node
::method startCData
  expose cdataNode document currentNode
  use strict arg
  -- create an empty cdata node.  The data will be filled in via character
  -- events
  cdataNode = document~createCDataSection('')
  -- add this as a child now
  currentNode~appendChild(cdataNode)


-- process the end of a CData node
::method endCData
  expose cdataNode currentNode

  -- TODO:  Currently a NOP, but filtering needs to be done here.
  cdataNode = .nil

-- finalize the document
::method endDocument
  expose document currentNode currentElement

  -- nothing much to do here.  The main parser has handled
  -- element completion issues.
  currentNode = .nil
  currentElement = .nil

-- we have a reference to an entity
::method startGeneralEntity
  expose document currentNode entityRef
  use strict arg name

  entityRef = document~createEntityReference(name)
  currentNode~appendChild(entityRef)
  -- additional bits get appended to this entity
  currentNode = entityRef

-- complete entity processing
::method endGeneralEntity
  expose document currentNode entityRef
  use strict arg name

  -- if we have a document type on this document, try to resolve the
  -- entity reference
  docType = document~documentType
  if docType \== .nil then do
      entities = docType~entities
      entity = entities~getNamedItem(name)
      -- we have an entity in the document type.  We need to copy the child
      -- nodes of our current node onto the entiry reference
      if entity \== .nil, entity~firstChild \== .nil then do
          loop child over currentNode
              entity~appendChild(child~cloneNode(.true))
          end
      end
  end
  entityRef = .nil
  currentNode = currentNode~parentNode

-- start processing of a DTD.  This initializes important state data
::method startDTD
  expose locations
  use strict arg location
  -- add this to the top of the location stack
  locations~push(location)

-- end parsing of a DTD
::method endDTD
  expose locations

  locations~pull   -- remove the top-most item
  -- just a NOP for the moment

-- begin processing of a parameterEntity
::method startParameterEntity
  expose entities
  use strict arg name
  entities~push(name)

-- end processing of an external entity
::method endParameterEntity
  expose entities

  entities~pull

-- begin handling of an external subset referenced from the DOCTYPE
::method startExternalSubset
  expose inExternalSubset locations
  use strict arg location

  locations~push(location)   -- keep track of this location
  inExternalSubset = .true

-- mark the end of external subset processing
::method endExternalSubset
  expose inExternalSubset locations

  inExternalSubset = .false
  locations~pop

-- add an element declaration to our tables
::method elementDecl
  expose elementDecls elementContentModels
  use strict arg name, contentType

  -- see if we have a definition for this element name yet
  elementDecl = elementDecls[name]
  if elementDecl \== .nil then do
      -- it's possible we have had a forward reference to an element by an attribute
      -- declaration.  If this declaration has type information, then this is a duplicate
      if elementDecl \== .nil then self~reportFatalError("Duplicate !ELEMENT definition:" name)
      -- complete the declaraton
      elementDecl~contentType = contentType
      elementDecl~contentModel = elementContentModels[name]
  end
  -- need to create a new one for our tables
  else do
      -- create an element declaration and hook it up with any accumulated content model
      -- information we may have for this element
      elementDecl = .ElementDecl~new(name, contentType, elementContentModels[name])
      elementDecls[name] = elementDecl
  end

-- process an attribute declaration
::method  attributeDecl
  expose elementDecls
  use strict arg elementName, attributeName, type, enumeration, defaultType, defaultValue, unnormalizedDefault

  -- see if we have a matching element already defined
  elementDecl = elementDecls[elementName]
  -- this is a forward reference to an undeclared element decl, so
  -- create a placeholder
  if elementDecl == .nil then do
      elementDecl = .ElementDecl~new(elementName)
      elementDecls[elementName] = elementDecl
  end

  -- the spec says only the first definition is binding and all additional ones
  -- are just ignored.
  if elementDecl~attributes[attributeName] \== .nil then return

  -- fill in the attribute decl values and attach to the element declaration
  attributeDecl = .AttributeDecl~new(attributeName, type)
  attributeDecl~enumeration = enumeration
  attributeDecl~defaultType = defaultValue
  attributeDecl~unnormalizedDefault = unnormalizedDefault

  elementDecl~attributes[attributeName] = attributeDecl

-- handle an internal entity declaration
::method internalEntityDecl
  expose entityDecls parameterEntityDecls document documentType
  use strict arg entityName, isParameter, text, normalizedText

  -- normal entities and parameter entities have different name spaces
  if isParameter then table = parameterEntityDecls
  else table = entityDecls

  -- duplicate entity declarations are ignored, with the first
  -- one always remaining bound to the name
  if table~hasIndex(entityName) then return

  entityDecl = .InternalEntityDecl~new(entityName, text, normalizedText)
  table[entityName] = entityDecl

  -- Now add an entity node to the document type, but only general entities.
  if \isParameter then do
      entities = documentType~entities
      entity = document~createEntity(entityName)
      entities~setNamedItem(entity)
  end

-- handle an external entity declaration
::method externalEntityDecl
  expose entityDecls parameterEntityDecls document documentType
  use strict arg entityName, location

  -- normal entities and parameter entities have different name spaces
  if isParameter then table = parameterEntityDecls
  else table = entityDecls

  -- duplicate entity declarations are ignored, with the first
  -- one always remaining bound to the name
  if table~hasIndex(entityName) then return

  entityDecl = .ExternalEntityDecl~new(entityName, location)
  table[entityName] = entityDecl

  -- Now add an entity node to the document type, but only general entities.
  if \isParameter then do
      entities = documentType~entities
      entity = document~createEntity(entityName)
      entity~publicId = location~publicId
      entity~systemId = location~systemId
      entity~baseUri = location~baseUri
      entities~setNamedItem(entity)
  end

-- handle an unparsed entity declaration
::method unparsedEntityDecl
  expose entityDecls parameterEntityDecls
  use strict arg entityName, location, notation

  -- normal entities and parameter entities have different name spaces
  if isParameter then table = parameterEntityDecls
  else table = entityDecls

  -- duplicate entity declarations are ignored, with the first
  -- one always remaining bound to the name
  if table~hasIndex(entityName) then return

  entityDecl = .ExternalEntityDecl~new(entityName, location, notation)
  table[entityName] = entityDecl

  -- Now add an entity node to the document type, but only general entities.
  if \isParameter then do
      entities = documentType~entities
      entity = document~createEntity(entityName)
      entity~publicId = location~publicId
      entity~systemId = location~systemId
      entity~baseUri = location~baseUri
      entity~notation = notation
      entities~setNamedItem(entity)
  end

-- handle a notation declaration
::method notationDecl
  expose notationDecls
  use strict arg notationName, location

  -- duplicate notation declarations are ignored, with the first
  -- one always remaining bound to the name
  if notationDecls~hasIndex(notationName) then return

  notationDecl = .NotationDecl~new(notationName, location)
  notationDecls[notationName] = notationDecl

  -- Now add an entity node to the document type, but only general entities.
  if \isParameter then do
      notatons = documentType~notations
      notation = document~createNotation(notationName)
      notation~publicId = location~publicId
      notation~systemId = location~systemId
      notation~baseUri = location~baseUri
      notation~notation = notation
      notations~setNamedItem(notation)
  end

-- start a group within a content model declaration
::method startGroup
  expose groupStack currentGroup

  -- push the current grouping, and make a new current group
  groupStack~push(currentGroup)
  currentGroup = .ContentModelGroup~new

-- signaled if a #PCDATA indicator is encountered in a content model
-- definition.  This is called after the startGroup for the group to which
-- it belongs, so this will modify the state of the current group
::method pcdata
  expose currentGroup
  currentGroup~mixed = .true

-- add an element name to an existing group
::method element
  expose currentGroup
  use strict arg elementName

  currentGroup~elements~append(elementName)

-- handle a group separator.  This will determine whether a group
-- is a sequence or a choice
::method separator
  expose currentGroup
  use strict arg separator

  -- the separator argument doubles as the type value
  currentGroup~type = separator

-- add an occurrence modifier to a current group
::method occurrence
  expose currentGroup
  use strict arg currentGroup~occurrence

-- process the end of a group element
::method endGroup
  expose groupStack currentGroup

  -- get the previous group
  oldGroup = groupStack~pull
  -- if there is a previous group, then make it current and add the
  -- just terminated group to that one as a child
  if oldGroup \== .nil then do
      oldGroup~elements~append(currentGroup)
      currentGroup = oldGroup
  end
  -- if this is the top group, then we leave currentGroup active

-- handle the start of an attlist declaration
::method startAttlist
  use strict arg elementName

  -- this is a nop for this handler, but other handlers might decide to
  -- do something.

-- handle the end of an attlist declaration
::method endAttlist
  use strict arg elementName

  -- this is a nop for this handler, but other handlers might decide to
  -- do something.

-- handle the start of a conditional section
::method startConditional
  use strict arg type

  -- this is a nop for this handler, but other handlers might decide to
  -- do something.

-- handle the end of a conditional section
::method endConditional
  use strict arg type

  -- this is a nop for this handler, but other handlers might decide to
  -- do something.



-- handle the ignored characters of a conditional section
::method ignoredCharacters
  use strict arg ignoredCharacters

  -- this is a nop for this handler, but other handlers might decide to
  -- do something.





/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: ContentModelGroup                                                   */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

-- a definition grouping within a content model
::class "ContentModelGroup"
::method init
  expose elements mixed type
  elements = .array~new
  mixed = .false
  type = .nil   -- unknown type so far

::attribute elements
::attribute mixed
::attribute type
::attribute occurrence

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: ElementDecl                                                         */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

-- a holder for an element declaration
::class "ElementDecl"
::method init
  expose name contentType contentModel
  use strict arg name, contentType = .nil, contentModel = .nil
  -- NB: the name is a QName, so use a table rather than a directory.
  attributes = .table~new

::attribute name
::attribute contentType
::attribute contentModel
::attribute attributes
::attribute enumeration


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: AttributeDecl                                                       */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

-- a holder for an attribute declaration
::class "AttributeDecl"
::method init
  expose name type
  use strict arg name, type

::attribute name
::attribute type
::attribute defaultType
::attribute enumeration
::attribute defaultValue
::attribute unnormalizedDefault


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: InternalEntityDecl                                                  */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

-- a holder for an internal entity declaration
::class "InternalEntityDecl"
::method init
  expose name text normalizedText
  use strict arg name, text, normalizedText

::attribute name
::attribute text
::attribute normalizedText
::method isInternal
  return .true
::method isExternal
  return .false


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: ExternalEntityDecl                                                  */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

-- a holder for an external entity declaration
::class "ExternalEntityDecl"
::method init
  expose name location
  use strict arg name, location, notation = .nil

::attribute name
::attribute location
::attribute notation
::method isUnparsed
  expose notation
  return notation \== .nil
::method isInternal
  return .false
::method isExternal
  return .true


/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/* Class: NotationDecl                                                        */
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

-- a holder for a notation declaration
::class "NotationDecl"
::method init
  expose name location
  use strict arg name, location

::attribute name
::attribute location


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
  expose document element parent
  use strict arg document, qname, attributes, parent

  -- step one, figure out all of the namespaces associated with this
  -- element.  This is merged from the parent and extracted from the
  -- attributes.
  self~resolveNameSpaces(attributes, parent)
  -- resolve the namespace for this based on whether the tag name has a
  -- prefix
  namespace = self~resolvePrefix(qname)
  -- create the element
  element = document~createElementNS(namespace, qname~nodeName)

  -- now process all of the attributes
  loop attr over attributes
      -- special meaning for this one
      if attr~nodeName == "xmlns" then namespace = self~xmlnsNamespace
      -- otherwise go through the normal resolution process
      else namespace = self~resolvePrefix(attr)
      -- add the attribute to the the element
      element~setAttributeNS(namespace, attr~nodeName, attributes[attr])
  end

-- the element being constructed
::attribute element
-- URI of any namespace default applied to attributes and children
::attribute defaultNamespace
-- map of prefixes known in this element context
::attribute prefixMap

-- create a namespace map
::method resolveNameSpaces
  expose prefixMap defaultNamespace document
  use strict arg attributes, parent

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
  loop attrName over attributes
      -- default namespace has been specified for this element.  This
      -- will apply to any untagged attributes and children of this node.
      if attrName~nodeName == "xmlns" then do
         uri = attributes[attrName]
         -- verify we're not attempting to use the standard namespaces illegally.
         -- NOTE:  when we create actual attribute values, this will result in a DOM exception
         if uri == self~xmlNamespace then iterate
         if uri == self~xmlnsNamespace then iterate
         defaultNamespace = uri
      end
      -- tagged as an xmlns: prefix definition
      else if attrName~prefix == "xmlns" then do
          -- add this value to our prefix map
          if attrName~localName = "xml" | attrName~localName = "xmlns" then iterate
          if \.xmlchar~isNCName(prefix) then iterate
          prefixMap[attrName~localName] = attributes[attrName]
      end
  end

-- resolve a prefix to its URI equivalent for the current element context.
::method resolvePrefix
  expose document defaultNamespace prefixMap
  use arg qname

  -- no prefix on the tag name, so use the default
  if qname~prefix == "" then return defaultNamespace

  -- this needs to be in the map, otherwise it is an error
  namespace = prefixMap[qname~prefix]
  if namespace = .nil then self~reportError(.DomException~NAMESPACE_ERR)
  return namespace

-- add a child node to this element
::method addChildNode
  expose element
  use strict arg node
  element~appendChild(node)
