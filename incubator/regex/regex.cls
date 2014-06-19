/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2014 Rexx Language Association. All rights reserved.         */
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
-- ::options trace commands

-- a compiled pattern that can perform regex operations
::class "Pattern" public


-- compile a string regex expression into a pattern
::method compile class
  -- the compiler is configurable by overriding this in a superclass
  -- or providing a different compiler on the invocation.
  -- the default is standard (or close to standard) regex expressions.
  use strict arg pattern, compiler = (.RegexCompiler~new)
  return compiler~compile(pattern)


-- initialize a pattern instance.  Arguments are the string pattern
-- we were compiled from, the root of the evaluation tree, and the
-- list of groups used for back references.
::method init
  expose pattern root groups
  use strict arg pattern, root, groups


-- Patterns are immutable, be we do provide read-only access to the elements
::attribute pattern GET
::attribute groups GET
::attribute root GET


-- retrieve the number of capturing groups
-- contained within this pattern.
::attribute groupCount GET
  expose groups
  return groups~items


-- display a pattern instance as its source string pattern
::method string
  expose pattern
  return pattern


-- simple one time match operation on a regular expression and
-- a region within a string.  Returns .true if this is an exact
-- match for the entire region
::method matches
  use strict arg text, start = 1, length = (text~length)
  -- create a matching context for this
  context = .MatchContext~new(text, start, length)
  -- and just perform a match on this
  return context~matches(self)


-- simple one time match operation on a regular expression and
-- a region within a string.  Returns a MatchResult item if this
-- matches at the starting position (but not necessarily the entire
-- string)
::method match
  use strict arg text, start = 1
  -- create a matching context for this
  context = .MatchContext~new(text)
  -- and just perform a match on this
  return context~match(self, start)


-- simple one time match operation on a regular expression and
-- a region within a string.  Returns .true if the region starts
-- with the given pattern
::method startsWith
  use strict arg text, start = 1
  -- create a matching context for this
  context = .MatchContext~new(text)
  -- and just perform a match on this
  return context~startsWith(self, start)


-- search a region of a string for a regex value.  Only the
-- section between the start position and the given length
-- are searched.  The return value is a MatchResult object with the
-- full details of the match.
::method find
  use strict arg text, start = 1, length = (text~length)
  -- create a matching context for this
  context = .MatchContext~new(text, start, length)
  -- and just perform a search and return the match position
  -- value
  return context~find(self, start)


-- split a string into an array of segments using a regular expression
-- as the match position
::method split
  use strict arg text, limit = (-1)

  list = .array~new   -- our set of match results
  indexPosition = 1   -- split always starts at the beginning

  -- now loop until we no longer match
  result = self~find(text, indexPosition)
  -- non-limited is more typical and simpler
  if limit < 0 then do
      -- now loop until we no longer match
      result = self~find(text, indexPosition)
      do while result~matched
          list~append(result~prefix)
          -- advance to the next position.  Note that
          -- this works correctly even if the match length
          -- is a null string
          indexPosition = result~nextMatch
      end
  end
  else do
      count = 0
      do while count < limit
          -- now loop until we no longer match
          result = self~find(text, indexPosition)
          if \result~matched then do
              leave
          end
          list~append(result~prefix)
          -- advance to the next position.  Note that
          -- this works correctly even if the match length
          -- is a null string
          indexPosition = result~nextMatch
          count += 1
      end
  end

  -- append any tail piece, if there is one
  if indexPosition <= text~length then do
      list~append(text~substr(indexPosition))
  end
  return list


-- return a set of match metrics for this pattern instance
::method matchMetrics
  expose root
  use strict arg
  metrics = .MatchMetrics~new
  -- forward this to the matching tree
  root~calculateMatchMetrics(metrics)
  return metrics


-- non-public class used by both the RegexCompiler and RegexParser to
-- validate compile options
::class "RegexOptionsValidator"

-- the following CONSTANTs define the valid options; split into two parts for
-- readability and source line length
--  NB DO NOT change the order without modifying the code in validateOptions()
::constant validOptions1 "MULTILINE SINGLELINE INTERNETLINES UNIXLINES"
::constant validOptions2 "CASELESS RESPECTCASE DOTALL DOTRESTRICTED"


-- validate any compiler options.  These can be done on behalf of the compiler at
-- creation time, or passed to the compiler at compile time to override the defaults
::method validateOptions class
  use strict arg target, options

  if options == .nil then do
      return
  end

  options = options~upper
  -- combine the two lists of options
  validOptions = self~validOptions1 self~validOptions2

  loop while options \== ""
      parse var options option options

      select
          -- option = MULTILINE
          when validOptions~word(1)~abbrev(option, 1) then do
              target~multiLineMode = .true
          end
          -- option = SINGLELINE
          when validOptions~word(2)~abbrev(option, 1) then do
              target~multiLineMode = .false
          end
          -- option = INTERNETLINES
          when validOptions~word(3)~abbrev(option, 1) then do
              target~unixLinesMode = .false
          end
          -- option = UNIXLINES
          when validOptions~word(4)~abbrev(option, 1) then do
              target~unixLinesMode = .true
          end
          -- option = CASELESS
          when validOptions~word(5)~abbrev(option, 1) then do
              target~caselessMode = .true
          end
          -- option = RESPECTCASE
          when validOptions~word(6)~abbrev(option, 1) then do
              target~caselessMode = .false
          end
          -- option = DOTALL
          when validOptions~word(7)~abbrev(option, 4) then do
              target~dotAllMode = .true
          end
          -- option = DOTRESTRICTED
          when validOptions~word(8)~abbrev(option, 4) then do
              target~dotAllMode = .false
          end
          otherwise  do
              raise syntax 93.915 array(validOptions, option)
          end
      end
  end


-- The default Regex compiler...other compilers are possible, as
-- long as they produce Pattern instances.
::class "RegexCompiler" public

-- various sets of characters used for parsing.
::constant METACHARACTERS "([{\^$|]})?*+."
::constant CLASSMETACHARACTERS "[]"
::constant CLOSURECHARACTERS "*+?{"
::constant WORDCHARACTERS 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_'
::constant DIGITCHARACTERS '0123456789'
::constant LOWERCASECHARACTERS "abcdefghijklmnopqrstuvwxyz"
::constant UPPERCASECHARACTERS "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
::constant WHITESPACECHARACTERS '20090a0b0c0d'x
::constant CRLF                 '0d0a'x

-- options constants for the different compiler modes
::constant multiline "MULTILINE"
::constant singleline "SINGLELINE"
::constant internetlines "INTERNETLINES"
::constant unixlines "UNIXLINES"
::constant caseless  "CASELESS"
::constant respectcase "RESPECTCASE"
::constant dotall "DOTALL"
::constant dotrestricted "DOTRESTRICTED"

-- various option flags
::attribute unixLinesMode
::attribute caselessMode
::attribute multiLineMode
::attribute dotAllMode


-- initialize class-level information.  This uses activate
-- rather than init to ensure that all other classes in the package
-- are available when run.
::method activate class

  -- initialize the default pattern library
  self~initializePatternLibrary


-- access the default pattern library
::attribute defaultPatternLibrary class GET


-- initialize the default pattern library on first use
::method initializePatternLibrary class private
  expose defaultPatternLibrary

  defaultPatternLibrary = .PatternLibrary~new
  -- a version for brevity during the setup phase
  pl = defaultPatternLibrary

  pl~addFamily("LOWER", self~lowerCaseCharacters)
  pl~addFamily("UPPER", self~upperCaseCharacters)
  pl~addFamily("ASCII", xrange('00'x, '7F'x))
  pl~addFamily("ALPHA", self~lowerCaseCharacters||self~upperCaseCharacters)
  pl~addFamily("DIGIT", self~digitCharacters)
  pl~addFamily("ALNUM", self~lowerCaseCharacters||self~upperCaseCharacters||self~digitCharacters)
  pl~addFamily("PUNCT", "!""#$%&'()*+,-./:;<=>?@[\]^_`{|}~")
  pl~addFamily("GRAPH", pl~getFamily('ALNUM')||pl~getFamily('PUNCT'))
  pl~addFamily("PRINT", pl~getFamily('GRAPH'))
  pl~addFamily("BLANK", " " || .String~tab)
  pl~addFamily("CNTRL",  xrange("00"x, "1F"x) || "7F"x)
  pl~addFamily("XDIGIT", "0123456789abcdefABCDEF")
  pl~addFamily("ODIGIT", "01234567")
  pl~addFamily("SPACE", self~whiteSpaceCharacters)
  pl~addFamily("WORD", self~wordCharacters)

  pl~addFamily("REXXSYMBOL", pl~getFamily('ALNUM') || ".!?_" )
  pl~addFamily("REXXVARIABLESTART", pl~getFamily("ALPHA") || "!?_")
  pl~addFamily("REXXOPERATOR", "+-\/%*|&=<>" || 'aaac'x)
  pl~addFamily("REXXSPECIAL", ",;:~()[]")

  pl~addFamily("EMAILUSERNAME", pl~getFamily("ALNUM") || ".!#$%&'*+/=?^_`{|}~-")
  pl~addFamily("URLUSERNAME", pl~getFamily("ALNUM") || "-._~%!$&'()*+,;=")
  pl~addFamily("URLVALID]", pl~getFamily("ALNUM") || "-._~%!$&'()*+,;=:@/?")

  pl~addPattern('TOPLEVELDOMAIN', "(?:\p{Alpha}{2}|com|org|net|edu|gov|mil|biz|info|mobi|name|aero|asia|jobs|museum|travel)\b")
  pl~addPattern('EMAILUSERNAME', "(?:\p{EmailUserName}+(?:\.\p{EmailUserName}+)*)")
  pl~addPattern('INTERNETDOMAIN', "(?:\p{Alnum}(?:[\p{Alnum}-]*\p{Alnum})?\.)+\m{TopLevelDomain}")
  pl~addPattern('EMAIL', "(?<username>\m{EmailUserName})@(?<domain>\m{InternetDomain})")
  pl~addPattern('IPV4DIGITS', "(25[0-5]|2[0-4][0-9]|1[0-9][0-9]|[1-9]?[0-9])")
  pl~addPattern('IPV4ADDRESS', "\b\m{IPV4Digits}\.\m{IPV4Digits}\.\m{IPV4Digits}\.\m{IPV4Digits}\b")
  pl~addPattern('STANDARDIPV6ADDRESS', "(?<![:.\w])(?:\p{Alnum}{1,4}:){7}\p{Alnum}{1,4}(?![:.\w])")
  pl~addPattern('URLPROTOCOL', "\p{Alpha}[\p{Alnum}+\-.]*:")
  pl~addPattern('URLUSER', "\p{URLUSERNAME}+@")
  pl~addPattern('IPV6HOST', "\[[\p{XDigit}:.]+\]")
  pl~addPattern('IPV6FUTUREHOST', "\[v[\p{XDigit}][\p{XDigit}\-._~%!$&'()*+,;=:]+\]")
  pl~addPattern('URLPORT', ":[0-9]+")
  pl~addPattern('URLPATH', "(/[\p{Alnum}\-._~%!$&'()*+,;=:@]+)*/")
  pl~addPattern('URLQUERY', "\?[\p{URLValid}]*")
  pl~addPattern('URLFRAGMENT', "\#[p{URLValid}]*")
  pl~addPattern('URL', "(?<protocol>\m{URLProtocol})(?<user>\m{URLUser})?(?<host>\m{InternetDomain}|\m{IPv6Host}|\m{IPv6FutureHost})(?<port>\m{URLPort})(?<path>\m{URLPath})(?<query>\m{URLQuery})?(?<fragment>\m{URLFragment})?")


-- initialize a regex compiler instance
::method init
  expose defaultLibrary patternLibrary addedLibraries
  use strict arg options = .nil

  -- default all of these settings before processing any explicit
  -- options that have been provided.
  self~multiLineMode = .false
  self~unixLinesMode = .false
  self~caselessMode = .false
  self~dotAllMode = .false

  -- validate the compile options before starting
  .RegexOptionsValidator~validateOptions(self, options)

  -- initialize the libraries
  defaultLibrary = self~class~defaultPatternLibrary
  -- this is the library for patterns added directly to this compiler instance
  -- we only create this the first time the library is requested
  patternLibrary = .nil
  -- these are additional libraries that can be attached to the compiler
  addedLibraries = .array~new

-- compile a regex expression using the posix-like
-- regular expression syntax
::method compile
  use strict arg pattern, options = "", compileLibrary  = .nil

  -- create a parser instance and convert the string pattern
  -- into an executable pattern object.
  parser = .RegexParser~new(self, pattern, options, compileLibrary)
  return parser~parse

-- retrieve the compiler's pattern library instance.  This is lazily created on
-- first request
::attribute patternLibrary GET
  expose patternLibrary

  -- create this if this is the first request
  if patternLibrary == .nil then do
      patternLibrary = .PatternLibrary~new
  end

  return patternLibrary


-- retrieve a named class family from the libraries this compile is working with.
-- Search order is 1) libraries added to the
-- compiler, 2) the default system library
::method getFamily
  expose defaultLibrary patternLibrary addedLibraries
  use strict arg name
  name = name~upper -- make sure we always use an uppercase name

  -- if we've had any patterns added, check that first
  if patternLibrary \= .nil then do
      family = patternLibrary~getFamily(name)
      return family
  end

  loop library over addedLibraries
     family = library~getFamily(name)
     if family \= nil then return family
  end

  return defaultLibrary~getFamily(name)


-- retrieve a named pattern from the libraries this compile is working with.
-- Search order is 1) libraries added to the
-- compiler, 2) the default system library
::method getPattern
  expose patternLibrary defaultLibrary addedLibraries
  use strict arg name
  name = name~upper -- make sure we always use an uppercase name

  -- if we've had any patterns added, check that first
  if patternLibrary \= .nil then do
      pattern = patternLibrary~getPattern(name)
      return pattern
  end

  loop library over addedLibraries
     pattern = library~getPattern(name)
     if pattern \= nil then return pattern
  end

  return defaultLibrary~getPattern(name)


-- add a named character family to this compiler instance.  This will
-- override any other libraries this compiler searches (except for any
-- library provided on the compile method)
::method addFamily
  use strict arg name, characters

  -- request the pattern library from this instance (which may
  -- create it for the first time) and add the family
  self~patternLibrary~addFamily(name, characters)


-- add a named pattern to this compiler instance.  This will
-- override any other libraries this compiler searches (except for any
-- library provided on the compile method)
::method addPattern
  use strict arg name, pattern

  -- request the pattern library from this instance (which may
  -- create it for the first time) and add the family
  self~patternLibrary~addPattern(name, pattern)


-- add a library to the compiler instance
::method addPatternLibrary
  expose addedLibraries
  use strict arg library

  -- if not already a pattern library, assume this
  -- is a source that a pattern library can be loaded from
  if \library~isA(.PatternLibrary) then do
      library = .PatternLibrary~new(library)
  end



-- internal class to manage the actual compiling process.  Since the compiler
-- instance can be recursively invoked to handle named patterns, we need to
-- have a context class to handle the details of the compilation using resources
-- from the compiler instance.
::class "RegexParser"
::method init
  expose compiler pattern compileLibrary current length groups groupCount
  use strict arg compiler, pattern, options = .nil, compileLibrary = .nil

  -- default the options settings before processing any compile options
  self~multiLineMode = compiler~multiLineMode
  self~unixLinesMode = compiler~unixLinesMode
  self~caselessMode  = compiler~caselessMode
  self~dotAllMode    = compiler~dotAllMode

  -- validate the compile options before starting
  .RegexOptionsValidator~validateOptions(self, options)

  current = 1     -- always start at the beginning
  length = pattern~length
  groups = .nil   -- no group information until we hit the first capturing group
  groupCount = 0  -- This is for the group numbering
  -- create the group reference item for the main matching context
  self~createGroupReference(0)


-- parse the pattern and return an executable Pattern instance
::method parse
  expose pattern groups

  -- this is the last node that will get control only if all other
  -- elements match cleanly.  The expression parser hooks this up to the
  -- end of the expression graph.
  lastNode = .TerminatorNode~new

  rootNode = self~parseExpression(lastNode)
  -- and return a usable pattern for this
  return .Pattern~new(pattern, rootNode, groups)


-- retrieve a named class family from the libraries this compile is working with.
-- Search order is 1)  library specified on the compile, 2) libraries added to the
-- compiler, 3) the default system library
::method getFamily
  expose compileLibrary compiler
  use strict arg name
  name = name~upper -- make sure we always use an uppercase name

  if compileLibrary \= .nil then do
     family = compileLibrary~getFamily(name)
     if family \= nil then return family
  end

  -- the compiler handles steps 2&3
  return compiler~getFamily(name)


-- retrieve a named pattern from the libraries this compile is working with.
-- Search order is 1)  library specified on the compile, 2) libraries added to the
-- compiler, 3) the default system library
::method getPattern
  expose compileLibrary compiler
  use strict arg name
  name = name~upper -- make sure we always use an uppercase name

  if compileLibrary \= .nil then do
     pattern = compileLibrary~getPattern(name)
     if pattern \= nil then return pattern
  end

  -- the compiler handles steps 2&3
  return compiler~getPattern(name)


-- we occasionally need to retrieve the current position
::attribute current GET


-- extract a substring from the parsing context
::method extract
  expose pattern
  use strict arg start, end

  return pattern~substr(start, end - start)

-- extract the next character and step the parsing position
-- Returns .nil if past the end of the pattern
::method next
  expose pattern current length

  -- .nil triggers the end of the parsing
  if current > length then do
      return .nil
  end

  -- grab the character at the current position and
  -- step to the next slot
  ch = pattern~subchar(current)
  current = current + 1
  return ch


-- peek at the current parsing position without stepping
-- the cursor.  Returns .nil if past the end of the pattern.
::method peek
  expose pattern current length

  if current > length then do
      return .nil
  end

  ch = pattern~subchar(current)
  return ch


-- step ahead one character and then peek at the next character.
-- this combines two operations that are frequently needed in
-- succession.
::method stepAndPeek
  expose pattern current length

  -- step first, unconditionally (we're usually at a good character
  -- position when we issue this, but since the EOS test if any
  -- pointer greater than the length, this is always safe).
  current = current + 1

  -- .nil triggers the end of the parsing
  if current > length then do
      return .nil
  end

  -- grab the character at the current position and
  -- step to the next slot
  ch = pattern~subchar(current)
  return ch


-- peek at the next character at a given offset from the current parsing position without
-- moving the cursor.  The offset can be positive or negative.  Returns .nil
-- if the resulting position is beyond the pattern bounds
::method peekOffset
  expose pattern current length
  use strict arg offset

  position = current + offset

  if position > length | position < 1 then do
      return .nil
  end

  ch = pattern~subchar(position)
  return ch


-- process reading of a single character, taking escaping
-- into account
::method singleChar

  ch = self~next
  -- have an escape character?
  if ch == '\' then do
      return self~parseEscapedCharacters
  end

  return ch   -- return whatever this is, including .nil


-- move the cursor to a previous position, ensuring that we
-- don't move past the beginning
::method previous
  expose current

  current = max(current - 1, 1)


-- test if a character is a metacharacter that needs to be skipped
::method isMetaCharacter
  use strict arg ch

  return .RegexCompiler~METACHARACTERS~contains(ch)


-- test if a character is a metacharacter in the context of parsing
-- a class range
::method isClassMetaCharacter
  use strict arg ch

  return .RegexCompiler~CLASSMETACHARACTERS~contains(ch)


-- test if a character is a closure character
-- a class range
::method isClosureCharacter
  use strict arg ch

  return .RegexCompiler~CLOSURECHARACTERS~contains(ch)


-- test if a character is a digit character
::method isDigit
  use strict arg ch

  return .RegexCompiler~DIGITCHARACTERS~contains(ch)


-- extract a string from a pattern that is delimited
-- by a pair of markers
::method extractDelimited
  expose pattern current length
  use strict arg start, end

  -- In theory, we should be at the start delimiter now.  If
  -- not there, this is an error
  if \pattern~match(current, start) then do
      raise syntax 13.900 array("Delimiter character" start "expected for delimited string")
  end

  -- step over the delimiter
  startPos = current + start~length
  -- and look for the closing delimiter (no escaping here, so we just search)
  endPos = pattern~pos(end, startPos)
  if endPos == 0 then do
      raise syntax 6.900 array("Missing closing delimeter" end)
  end

  current = endPos + end~length  -- position past the read position
  -- and return the string between the markers
  return pattern~substr(startPos, endPos - startPos)


-- extract a numeric value from the stream.  Terminates
-- on the first non-numeric character or the EOS.
::method parseNumber
  number = ""

  do forever
     ch = self~next
     if ch == .nil then do
         leave
     end
     if \self~isDigit(ch) then do
         -- non-numeric character, so backup and quit the loop
         self~previous
         leave
     end
     -- add to the accumulator
     number = number || ch
  end

  -- nothing valid found, return .nil as the failure value
  if number == '' then do
      return .nil
  end
  return number


-- Parse out an expression tree and compile into a directed graph of
-- match nodes.  This version handles alternative forms.  If not
-- part of an alternative sequence, it just returns the base sub expression.
::method parseExpression
  expose pattern matchNodes current length
  use strict arg terminator   -- this is the end node that gets plugged into the end of the expression.

  alternative = .nil   -- if we have alternatives in this expression, we group them all
  do forever
      -- parse a sequence and chain up with the terminator
      node = self~parseSequence(terminator)
      -- if there is no node here, this could be a "dangling"
      -- "|" on the end of an alternative.  If this is the case,
      -- (i.e., we've been accumulating alternatives), then add
      -- a special everything matcher to the end and finish up
      if node == .nil then do
          if alternative \= .nil then do
              -- add a node that will match everything
              alternative~addAlternative(.EverythingNode~new)
              return alternative
          end
          -- We expected something here and got nothing
          raise syntax 93.900 array("Invalid regular expression sequence")
      end
      -- if we've had at least one choice, then this just gets added to the
      -- list
      if alternative \== .nil then do
          alternative~addAlternative(node)
      end

      -- look ahead to see if this is again part of an alternation
      ch = self~peek

      if ch \== '|' then do
          -- not an alternative....see what we need to return
          if alternative == .nil then do
              return node
          end
          else do
              return alternative
          end
      end
      else do
          self~next -- step over over the | operator
          -- if we've only processed the first node, create an alternative and
          -- add the node.
          if alternative == .nil then do
              alternative = .AlternativeNode~new
              -- this now assumes the terminator
              alternative~next = terminator
              -- add the node to the alternative
              alternative~addAlternative(node)
          end
      end
  end


-- parse out a subexpression.  This also handles any of the modifiers
-- that might be associated with a single subexpression
::method parseSequence
  use strict arg terminator

  firstNode = .nil
  lastNode = .nil

  do forever
      -- just take a peek at the next character here so we can decide
      -- which option to take.
      ch = self~peek

      select
          -- this is an end of string, done parsing
          when .nil == ch then do
              leave
          end
          -- either of these is a terminator for the sequence.
          -- if this is "|", the caller will accumulate the alternatives.
          -- if this is ")", then this is the close of a group
          when ch == '|' | ch == ')' then do
              -- allow the caller to handle
              leave;
          end
          -- a class of characters
          when ch == '[' then do
              self~next
              node = self~parseClass
          end
          -- start of a group.  We'll process and then recurse
          when ch == '(' then do
              self~next
              -- parse out the group expression, then chain the
              -- entire section into our graph.  All subsequent
              -- bits follow the group sequence.
              groupInfo = self~parseGroup
              -- This could be a standalone flag group or a comment.  In that case,
              -- the compiler mode flags have been processed, but there
              -- is no node to handle.  Just skip over this and keep parsing
              if groupInfo == .nil then do
                  iterate
              end
              if firstNode == .nil then do
                  firstNode = groupInfo~firstNode
              end
              else do
                  lastNode~next = groupInfo~firstNode
              end
              lastNode = groupInfo~lastNode
              iterate     -- closure has already been handled
          end
          -- a start anchor
          when ch == '^' then do
              self~next
              node = self~parseStartAnchor
          end
          -- end anchor variants
          when ch == '$' then do
              self~next
              node = self~parseEndAnchor
          end
          -- match any character
          when ch == '.' then do
              self~next
              node = self~parseDot
          end
          -- various escape characters
          when ch == '\' then do
              self~next
              node = self~parseEscapes
          end
          -- these are not expected here
          when ch == '?' | ch == '*' | ch == '+' then do
              raise syntax 13.900 array ("Unexpected modifier character '"||ch||"'" )
          end
          otherwise do
              -- an atom string that is taken as-is
              node = self~parseAtom
          end
      end

      -- now figure out any closures to the section we just parsed.
      node = self~parseClosure(node)
      -- now process the chaining
      if firstNode == .nil then do
          firstNode = node
          lastNode = node
      end
      else do
          lastNode~next = node
          lastNode = node
      end
  end
  -- it's possible there is nothing to return.  In that case, just
  -- return .nil to indicate we have nothing here
  if firstNode == .nil then do
      return .nil
  end
  else do
      -- put the terminator as the next element of the last node of
      -- the chain.
      lastNode~next = terminator
  end
  return firstNode


-- create an appropriate start anchor (^) based on the
-- current mode flags
::method parseStartAnchor
  -- if operating in single line mode, this is just
  -- a text beginning
  if \self~multiLineMode then do
      return .BeginTextNode~new
  end
  else if self~unixLinesMode then do
      return .UnixMultilineCaretNode~new
  end
  else do
      return .InternetMultilineCaretNode~new
  end

-- Handle adding a handler for a $ anchor to the tree.  The operation
-- depends on the multiLine and unixLines flags.  If not in multiLine,
-- this will only match on the end of text OR if positioned on the appropriate
-- linend marker that is at the end of the text.  If in multiline mode,
-- then this will also recognize interior linend sequences.
::method parseEndAnchor
  -- an end anchor, which has 4 possibilities
  -- in multiline mode?  interior linends are recognized
  if self~multiLineMode then do
      -- return the version appropriate to the linend style
      if self~unixLinesMode then do
          return .UnixMultiLineEndNode~new
      end
      else do
          return .InternetMultiLineEndNode~new
      end
  end
  else do
      -- return the version appropriate to the linend style
      if self~unixLinesMode then do
          return .UnixLineEndNode~new
      end
      else do
          return .InternetLineEndNode~new
      end
  end


  -- a match anything character.  The meaning of
  -- "match anything" differs depending on options
::method parseDot
  if self~dotAllMode then do
      return .AllDotNode~new
  end
  else if self~unixLinesMode then do
      return .UnixDotNode~new
  end
  else do
      return .InternetDotNode~new
  end


-- parse the full range of escapes, including the operations.  This
-- returns a node to handle the escaped character
::method parseEscapes
  -- There are overlaps between the excaped operations and the escaped
  -- characters (\b is both backspace and word boundary, depending on context)
  -- Allow this override to occur by processing the operations first.  In
  -- contexts where word boundaries don't apply, this step is skipped.

  -- there are more complex operations that are escaped
  node = self~parseEscapedOperations
  -- if a recognized operation, return that
  if node \= .nil then do
      return node
  end
  -- now check for escaped characters that are really class nodes
  char = self~parseEscapedCharacters
  if char == .nil then do
      raise syntax 13.900 array("Invalid character after \ escape character")
  end
  -- this just parsed a single character, turn it into a
  -- node
  return .StringNode~new(char)


-- create a new group item for the pattern's list of numbered capture groups
::method newGroup
  expose groupCount
  groupCount += 1  -- each group gets allocated when first encountered
  -- we treat all of these as named groups, so just
  -- allocate one with the counter name
  self~createGroupReference(groupCount)
  return groupCount


-- Create the group reference item for a numbered or named group.
-- A named group can be used in multiple places
::method getGroupReference
  expose groups
  use strict arg id

  -- prevent numeric names from overlapping with the numbered groups
  if id~datatype('Whole') then do
      raise syntax 93.900 array("Numeric names cannot be use for named groups; found" id);
  end
  return self~createGroupReference(id~upper)


-- create a group reference item in our set of groups
::method createGroupReference private
  expose groups
  use strict arg id
  if groups == .nil then do
      groups = .directory~new
  end

  -- right now, this is just stored in the group registry and
  -- returned to the creating context.
  groupInfo = .GroupReference~new(id)
  groups[id] = groupInfo
  return groupInfo


-- test if a given named group has been encountered yet
-- (used for resolving back references)
::method haveGroup
  expose groups
  use strict arg id

  return groups~hasIndex(id~upper)


-- parse group information
::method parseGroup

  -- group specifications can change the mode settings, but the
  -- scope of the change is limited to the containing group.
  -- We need to restore these once we're finished parsing the group
  savedFlags = self~saveFlags
  -- most groups require closure, but not all
  closure = .true
  -- we return a directory of information about this group
  groupInfo = .directory~new
  ch = self~peek
  -- do we have a group qualifier.  These are all non-capturing groups
  if ch == '?' then do
      -- step over the peeked character and get the next one.
      ch = self~stepAndPeek
      select
          -- (?: is a non-capturing group.  Just like a normal group,
          -- but without the capture part
          when ch == ':' then do
              self~next -- step over the peeked character
              -- non capture group
              group = .GroupEnvelope~new
              -- parse the encapsulated group expression.  This gets
              -- slotted between the group element and the terminator
              group~next = self~parseExpression(group~terminator)

              -- to wire in to the caller, we need to provide a node that
              -- is plugged into the previous node, and a node that will be
              -- the new tail end of the chain.
              groupInfo~firstNode = group
              groupInfo~lastNode = group~terminator
          end
          -- (?> is an atomic capturing group.  Once it matches,
          -- it does not give up whatever it matched.
          when ch == '>' then do
              self~next -- step over the peeked character
              -- an atomic group.  Matching is managed differently
              group = .GroupEnvelope~new
              -- parse the encapsulated group expression.  This gets
              -- slotted between the group element and the terminator
              group~next = self~parseExpression(group~terminator)

              -- wrap this with an encapsulating atomic node.
              wrapper = .AtomicGroupNode~new(group)
              -- wrappered nodes fill both roles
              groupInfo~firstNode = wrapper
              groupInfo~lastNode = wrapper
          end
          -- positive zero-length lookahead.  This checks that the given
          -- pattern is at the current position, but its match is always
          -- zero-length, so it does not step the position nor does its
          -- match position count toward the bounds of the final match
          when ch == '=' then do
              self~next -- step over the peeked character
              -- parse the encapsulated group expression, but terminate
              -- with a branch terminator node.
              node = self~parseExpression(.nil)
              -- this is a special wrapper
              wrapper = .PositiveLookAheadNode~new(node)
              -- wrappered nodes fill both roles
              groupInfo~firstNode = wrapper
              groupInfo~lastNode = wrapper
              -- zero length matches do not have closures
              closure = .false
          end
          -- this is a lookbehind.  It checks the data before the current
          -- position
          when ch == '!' then do
              self~next -- step over the peeked character
              -- parse the encapsulated group expression, but terminate
              -- with a branch terminator node.
              node = self~parseExpression(.nil)
              -- this is a special wrapper
              wrapper = .NegativeLookAheadNode~new(node)
              -- wrappered nodes fill both roles
              groupInfo~firstNode = wrapper
              groupInfo~lastNode = wrapper
              -- zero length matches do not have closures
              closure = .false
          end
          -- Three possibilities here:
          -- 1)  (?<=... a "zero-width positive lookbehind"
          -- 2)  (?<!... a "zero-width negative lookbehind"
          -- 3)  (?<xxxxx>... a named capture group
          when ch == '<' then do
              self~next -- step over the peeked character
              -- a look behind.  These are a pain.  We can only
              -- support this if the pattern has a deterministic
              -- max and min.  Before we can decide, we need to
              -- extract the group expression and then calculate
              -- the metrics for the entire expression tree.

              -- this is the '=' or '!' qualifier
              ch = self~peek

              if ch == '=' | ch == '!' then do
                  self~next -- step over the modifier
                  -- parse the encapsulated group expression, but terminate
                  -- with a normal terminator node.  This will perform a
                  -- end of range check on the matches to ensure the
                  -- tests will butt up against the current position.
                  node = self~parseExpression(.TerminatorNode~new)

                  -- gather some metrics.  If there is a deterministic maximum,
                  -- we can optimize the back search by using it.  Otherwise,
                  -- we just go back to the beginning
                  metrics = self~getPatternMetrics(node)

                  -- create the matcher type based on the modifier
                  if ch == '=' then do
                      wrapper = .PositiveLookBehindNode~new(node, metrics)
                  end
                  else if ch == '!' then do
                      wrapper = .NegativeLookBehindNode~new(node, metrics)
                  end
                  -- wrappered nodes fill both roles
                  groupInfo~firstNode = wrapper
                  groupInfo~lastNode = wrapper
                  -- zero length matches do not have closures
                  closure = .false
              end
              -- this is a named capture group
              else do
                  -- back up for the delimiter parsing
                  self~previous
                  -- and parse out the delimited name
                  referenceName = self~extractDelimited('<', '>')
                  -- apply Rexx symbol rules to the name and make uppercase
                  referenceName = referenceName~upper

                  -- check that this is unique
                  if self~haveGroup(referenceName) then do
                      raise syntax 93.900 array("Duplicate named group in pattern:" referenceName)
                  end

                  -- add this to our reference table
                  self~getGroupReference(referenceName)
                  -- a named capturing group
                  group = .CapturingGroupEnvelope~new(referenceName)
                  -- parse the encapsulated group expression.  This gets
                  -- slotted between the group element and the terminator
                  group~next = self~parseExpression(group~terminator)

                  -- to wire in to the caller, we need to provide a node that
                  -- is plugged into the previous node, and a node that will be
                  -- the new tail end of the chain.
                  groupInfo~firstNode = group
                  groupInfo~lastNode = group~terminator
                  -- we need the group id to process the closure
                  groupInfo~id = referenceName
              end
          end
          -- a conditional node.  These are a bit more complex
          when ch == '(' then do
              node = self~parseConditional
              -- add this to the returned group information.  This still
              -- needs to have closures processed
              groupInfo~firstNode = node
              groupInfo~lastNode = node
          end
          -- documentation in a regex...can you imagine that!
          when ch == '#' then do
              self~previous  -- step back to the '(' that started the group
              self~previous
              -- just swallow the comment group and continue
              self~extractDelimited('(', ')')
              return .nil
          end
          -- a boundary defined by a character class
          when ch == 'b' then do
              -- step over the current and peek at the next one.
              ch = self~stepAndPeek
              -- the default type is simple class boundary
              boundaryType = .ClassBoundaryNode
              select
                  -- Not on a class boundary
                  when ch == '^' then do
                      boundaryType = .NotClassBoundaryNode
                      self~next
                  end
                  -- beginning of the sequence boundary
                  when ch == '<' then do
                      boundaryType = .BeginClassBoundaryNode
                      self~next
                  end
                  when ch == '>' then do
                      boundaryType = .EndClassBoundaryNode
                      self~next
                  end
                  otherwise do
                      -- the default type is simple class boundary
                      boundaryType = .ClassBoundaryNode
                  end
              end
              ch = self~next
              if ch \= '[' then do
                  raise syntax 93.900 array("Missing class definition for (?b) boundary match")
              end
              -- parse out the class definition
              node = self~parseClass
              -- create the boundary checker
              wrapper = boundaryType~new(node)
              -- wrappered nodes fill both roles
              groupInfo~firstNode = wrapper
              groupInfo~lastNode = wrapper
              -- zero length matches do not have closures
              closure = .false
          end
          otherwise do
              -- if .true, this is a closed modifier.  A
              -- closed modifier will last for the lifetime
              -- of its enclosing group.  This set of parens
              -- is ignored for the purposes of restoring the
              -- flag settings
              if self~parseFlag then do
                  return .nil
              end
              -- non capture group after the flag value
              group = .GroupEnvelope~new
              -- parse the encapsulated group expression.  This gets
              -- slotted between the group element and the terminator
              group~next = self~parseExpression(group~terminator)

              -- to wire in to the caller, we need to provide a node that
              -- is plugged into the previous node, and a node that will be
              -- the new tail end of the chain.
              groupInfo~firstNode = group
              groupInfo~lastNode = group~terminator
          end
      end
  end
  else do
      -- we're looking at the start of the sequence inside because we peeked
      -- at the character
      -- this is a capturing group
      id = self~newGroup
      group = .CapturingGroupEnvelope~new(id)
      -- parse the encapsulated group expression.  This gets
      -- slotted between the group element and the terminator
      group~next = self~parseExpression(group~terminator)

      -- to wire in to the caller, we need to provide a node that
      -- is plugged into the previous node, and a node that will be
      -- the new tail end of the chain.
      groupInfo~firstNode = group
      groupInfo~lastNode = group~terminator
      groupInfo~id = id
  end
  -- we should be positioned at the closing paren now.  Make sure
  -- it is there
  ch = self~next
  if ch \== ')' then do
      raise syntax 93.900 array("Missing closing ')' for a group")
  end
  -- restore the flags now to the original state
  self~restoreFlags(savedFlags)
  -- we need to process this differently, since there are different
  -- repetitors needed to handle updating group information
  if closure then do
      self~parseGroupClosure(groupInfo)
  end
  -- return the new chain information.
  return groupInfo


-- parse a conditional node in the form "(?(cond)then|else)
::method parseConditional
  -- we're positioned at the the ( that starts the conditional now.
  -- step over that and examine the different options
  ch = self~stepAndPeek
  select
      -- this is a lookahead or lookbehind construct
      when ch == '?' then do
          -- step ahead one character and see which
          -- flavor this is
          ch = self~stepAndPeek
          select
              -- forward lookahead test
              when ch == '=' then do
                  self~next -- step over the peeked character
                  -- parse the encapsulated group expression, but terminate
                  -- with a branch terminator node.
                  node = self~parseExpression(.nil)
                  -- this is a special wrapper
                  condition = .PositiveLookAheadNode~new(node)
              end
              -- this is a negative lookahead.
              when ch == '!' then do
                  self~next -- step over the peeked character
                  -- parse the encapsulated group expression, but terminate
                  -- with a branch terminator node.
                  node = self~parseExpression(.nil)
                  -- this is a special wrapper
                  condition = .NegativeLookAheadNode~new(node)
              end
              -- Three possibilities here:
              -- 1)  (?<=... a "zero-width positive lookbehind"
              -- 2)  (?<!... a "zero-width negative lookbehind"
              when ch == '<' then do
                  self~next -- step over the peeked character
                  -- a look behind.  These are a pain.  We can only
                  -- support this if the pattern has a deterministic
                  -- max and min.  Before we can decide, we need to
                  -- extract the group expression and then calculate
                  -- the metrics for the entire expression tree.

                  -- this is the "=" or "!" qualifier
                  ch = self~peek

                  if ch == '=' | ch == '!' then do
                      self~next -- step over the modifier
                      -- parse the encapsulated group expression, but terminate
                      -- with a normal terminator node.  This will perform a
                      -- and of range check on the matches to ensure the
                      -- tests will butt up against the current position.
                      node = self~parseExpression(.nil)

                      -- gather some metrics.  If there is a deterministic maximum,
                      -- we can optimize the back search by using it.  Otherwise,
                      -- we just go back to the beginning
                      metrics = self~getPatternMetrics(node)

                      -- create the matcher type based on the modifier
                      if ch == '=' then do
                          condition = .PositiveLookBehindNode~new(node, metrics)
                      end
                      else if ch == '!' then do
                          condition = .NegativeLookBehindNode~new(node, metrics)
                      end
                  end
                  else do
                      raise syntax 93.900 array("Unknown conditional type '"ch"'")
                  end
              end
              otherwise  do
                  raise syntax 93.900 array("Unknown conditional type '"ch"'")
              end
          end
          -- we should be positioned at the closing paren now.  Make sure
          -- it is there
          ch = self~next
          if ch \== ')' then do
              raise syntax 93.900 array("Missing closing ')' for a group")
          end
      end
      -- a back reference test.  This is of the form (?(name)...
      -- we're looking at the first character, so back up and
      -- extract the name using the '(' and ')' delimiters
      otherwise  do
          -- potentially a compound back reference name?  This requires
          -- more complicated parsing
          if ch == '<' then do
              -- parse out the group name (which might include nested group
              -- references)
              path = self~parseGroupName(path)
              -- we can validate the first, the rest are very dynamic
              if \self~haveGroup(path[1]) then do
                  raise syntax 93.900 array("Unrecognized group back reference:" path[1])
              end
              -- the more typical case of a single level conditional.  This just
              -- checks on group participation
              if path~items == 1 then do
                  -- a named back reference to previous node
                  condition = .BackReferenceTestNode~new(path[1])
              end
              else do
                  -- a more complex look up
                  condition = .ResultBackReferenceTestNode~new(path)
              end
          end
          else do
              self~previous
              referenceName = self~extractDelimited('(', ')')
              referenceName = referenceName~upper -- all names are stored in uppercase
              -- a named back reference to previous node
              condition = .BackReferenceTestNode~new(referenceName)
          end
      end
  end

  -- ok, we have the conditional part, now parse out the then and optional
  -- else sections

  -- NOTE:  we need to parse this as a sequence, not as an expression
  -- because the | is an expression end delimiter
  thenNode = self~parseSequence(.nil)
  ch = self~peek
  -- have the | part?
  if ch == '|' then do
      self~next -- step over the |
      -- now parse out the else section.  NOTE: parse sequence
      -- needs to be used because we don't accept alternatives
      -- unless in a group
      elseNode = self~parseSequence(.nil)
      node = .IfThenElseNode~new(condition, thenNode, elseNode)
  end
  else do
      -- simple then node
      node = .IfThenNode~new(condition, thenNode)
  end
  return node


-- calculate the match metrics for a given pattern.
::method getPatternMetrics
  use strict arg head

  metrics = .MatchMetrics~new
  -- forward this to the matching tree
  head~calculateMatchMetrics(metrics)
  return metrics


-- parse options flags in the expressions
::method parseFlag
  ch = self~next
  setting = .true

  -- multiple flags can appear in a group
  do forever
      select
          -- we're using unix linends
          when ch == 'd' then do
              self~unixLinesMode = setting
              setting = .true  -- reset for an additional mode flag
          end
          -- case insensitive matching
          when ch == 'i' then do
              self~caselessMode = setting
              setting = .true  -- reset for an additional mode flag
          end
          -- linends are recognized at other than the end of the data
          when ch == 'm' then do
              self~multiLineMode = setting
              setting = .true  -- reset for an additional mode flag
          end
          -- "single line" mode.
          when ch == 's' then do
              self~dotAllMode = setting
              setting = .true  -- reset for an additional mode flag
          end
          -- turning a flag off
          when ch == '-' then do
              -- if we're already negated, this is an error.
              if \setting then do
                  raise syntax 93.915 array("dims", ch)
                  -- this is a mode negation
              end
              setting = .false
          end
          -- this is the (?f:X) form.  The caller needs to
          -- process the rest of the group.  Return .false to
          -- indicate there's more to do.
          when ch == ':' then do
              return .false
          end
          -- if the flag terminator is the closing paren, then
          -- this group is a closed flag group.  The return value
          -- tells the caller there's nothing left in the group to
          -- process
          when ch == ')' then do
              return .true
          end
          otherwise do
              raise syntax 93.915 array("dims-", ch)
          end
      end
      ch = self~next  -- step to the next character
  end


-- various option flags
::attribute unixLinesMode
::attribute caselessMode
::attribute multiLineMode
::attribute dotAllMode


-- take a snapshot of our option flags for saving
-- and restoring around groups that alter the flags
-- just in the local context.  Returns a directory
-- with the flag settings
::method saveFlags
  flags = .directory~new
  flags~unixLinesMode = self~unixLinesMode
  flags~caselessMode = self~caselessMode
  flags~multiLineMode = self~multiLineMode
  flags~dotAllMode = self~dotAllMode
  return flags

-- restore the flag settings from a previous
-- saveFlags collection
::method restoreFlags
  use strict arg flags

  self~unixLinesMode = flags~unixLinesMode
  self~caselessMode = flags~caselessMode
  self~multiLineMode = flags~multiLineMode
  self~dotAllMode = flags~dotAllMode


-- process the closures for a group node.  This handles all of the
-- repetition count details
::method parseGroupClosure
  use strict arg groupInfo
  -- we're parsing any closure items that may follow the group itself.
  -- If there are closure items to process, this will end up adjusting the
  -- firstNode/lastNode information in groupInfo

  -- this will be the piece that's consumed by a potential closure item.
  target = groupInfo~firstNode

  ch = self~peek

  select
      when ch == '?' then do
          -- a question repetitor
          ch = self~stepAndPeek

          if ch == '?' then do
              self~next
              node = .ReluctantGroupQuestionNode~new(groupInfo~id, target)
          end
          else if ch == '+' then do
              self~next
              node = .PossessiveGroupQuestionNode~new(groupInfo~id, target)
          end
          else do
              node = .GreedyGroupQuestionNode~new(groupInfo~id, target)
          end
      end
      when ch == '*' then do
          -- zero or more occurrences
          ch = self~stepAndPeek

          if ch == '?' then do
              self~next
              node = .ReluctantGroupRepetitionNode~new(groupInfo~id, target, 0, 999999999)
          end
          else if ch == '+' then do
              self~next
              node = .PossessiveGroupRepetitionNode~new(groupInfo~id, target, 0, 999999999)
          end
          else do
              node = .GreedyGroupRepetitionNode~new(groupInfo~id, target, 0, 999999999)
          end
      end
      when ch == '+' then do
          -- one or more occurrences
          ch = self~stepAndPeek

          if ch == '?' then do
              self~next
              node = .ReluctantGroupRepetitionNode~new(groupInfo~id, target, 1, 999999999)
          end
          else if ch == '+' then do
              self~next
              node = .PossessiveGroupRepetitionNode~new(groupInfo~id, target, 1, 999999999)
          end
          else do
              node = .GreedyGroupRepetitionNode~new(groupInfo~id, target, 1, 999999999)
          end
      end
      when ch == '{' then do
          self~next
          min = self~parseNumber
          max = min
          ch = self~next
          if ch == ',' then do
              max = self~parseNumber
              ch = self~next
          end
          if ch \= '}' then do
              raise syntax 36.900 array("Missing closing '}' for repetition count")
          end
          --
          if .nil == min then do
              raise syntax 93.900 array("Invalid repetition count minimum")
          end
          -- we use the default digits maximum for max if not specified
          if max == .nil then do
              max = 999999999
          end

          if max < min then do
              raise syntax 93.900 array("Repetition maximum:" max", is less than minimum:" min)
          end

          ch = self~peek

          if ch == '?' then do
              self~next
              node = .ReluctantGroupRepetitionNode~new(groupInfo~id, target, min, max)
          end
          else if ch == '+' then do
              self~next
              node = .PossessiveGroupRepetitionNode~new(groupInfo~id, target, min, max)
          end
          else do
              node = .GreedyGroupRepetitionNode~new(groupInfo~id, target, min, max)
          end
      end
      otherwise do
          -- no additional closure
          return
      end
  end

  -- stick branch terminator after the end of this.  The
  -- question node will handle the repetitions.
  groupInfo~lastNode~next = .nil
  -- this is atomic, and handles both roles
  groupInfo~firstNode = node
  groupInfo~lastNode = node
  return


-- process the closure for a node.  Because a sequence might have
-- modifiers, we might need to wrap the node sequence in
-- an enclosing node that handles the modifiers
::method parseClosure
  use strict arg target
  ch = self~next

  select
      -- A question modifier, which itself might have a
      -- modifier to control the operating mode
      when ch == '?' then do
          -- a question repetitor
          ch = self~peek
          -- ?? operates in reluctant mode
          if ch == '?' then do
              self~next
              return .ReluctantQuestionNode~new(target)
          end
          -- possesive question mode
          else if ch == '+' then do
              self~next
              return .PossessiveQuestionNode~new(target)
          end
          else do
              -- no modifier is greedy
              return .GreedyQuestionNode~new(target)
          end
      end
      -- zero or more occurrences.  This also operates with mode
      -- modifiers
      when ch == '*' then do
          ch = self~peek

          if ch == '?' then do
              self~next
              return .ReluctantRepetitionNode~new(target, 0, 999999999)
          end
          else if ch == '+' then do
              self~next
              return .PossessiveRepetitionNode~new(target, 0, 999999999)
          end
          else do
              return .GreedyRepetitionNode~new(target, 0, 999999999)
          end
      end
      -- one or more occurrences.  Also with different modes of
      -- operation
      when ch == '+' then do
          ch = self~peek

          if ch == '?' then do
              self~next
              return .ReluctantRepetitionNode~new(target, 1, 999999999)
          end
          else if ch == '+' then do
              self~next
              return .PossessiveRepetitionNode~new(target, 1, 999999999)
          end
          else do
              return .GreedyRepetitionNode~new(target, 1, 999999999)
          end
      end
      -- {min[,max]} range
      when ch == '{' then do
          min = self~parseNumber
          max = min
          ch = self~next
          if ch == ',' then do
              max = self~parseNumber
              ch = self~next
          end
          -- must be a closing '}' on this
          if ch \= '}' then do
              raise syntax 36.900 array("Missing closing '}' for repetition count")
          end
          --
          if .nil == min then do
              raise syntax 93.900 array("Invalid repetition count minimum")
          end
          -- we use the default digits maximum for max if not specified
          if max == .nil then do
              max = 999999999
          end

          if max < min then do
              raise syntax 93.900 array("Repetition maximum:" max", is less than minimum:" min)
          end

          ch = self~peek

          if ch == '?' then do
              self~next
              return .ReluctantRepetitionNode~new(target, min, max)
          end
          else if ch == '+' then do
              self~next
              return .PossessiveRepetitionNode~new(target, min, max)
          end
          else do
              return .GreedyRepetitionNode~new(target, min, max)
          end
      end
      otherwise do
          -- no additional closure. make sure we put the character
          -- we just borrowed back...but only if it was a real character!
          if ch \= .nil then do
              self~previous
          end
          return target
      end
  end


-- parse a [abc] class modifier
::method parseClass
  -- parse the section of the class, which may
  -- included nested pieces
  node = self~parseClassSection

  ch = self~next
  if ch \= ']' then do
      raise syntax 36.900 array("Missing closing ']' for character class")
  end

  return node


-- parse out the characters between a class section.
::method parseClassSection
  expose current length pattern
  previousNode = .nil
  negated = .false
  firstchar = .true

  -- we look until the end of input or until we hit our
  -- closing delimiter
  do forever
      ch = self~peek -- just taking a peek
      select
          -- our caller checks for the closing piece, so
          -- just return what we have and allow them to issue
          -- the error message
          when ch == .nil then do
              return previousNode
          end
          -- negation of the class
          when ch == '^' then do
              if firstChar then do
                  self~next
                  negated = .true
                  iterate    -- go around and check the next character
              end
          end
          -- this is likely the close of our section, so
          -- finish up and let the caller determine if this is good
          when ch == ']' then do
              -- we've hit the end of this class spec, time to return.
              return previousNode
          end
          -- nested section within a class.  These are OR'd together
          when ch == '[' then do
              self~next  -- step over the delimiter
              subNode = self~parseClass

              -- if we already have a node within the class, we combine this
              -- using a logical op
              if .nil == previousNode then do
                  previousNode = subNode
              end
              else do
                  previousNode = .ClassOrNode~new(previousNode, subNode)
              end
          end
          when ch == '&' then do
              -- step over the first & and peek at the next char
              ch = self~stepAndPeek
              if ch == '&' then do     -- this is a logical "&&"
                  if prev == .nil then do
                      raise syntax 35.900 array("Missing left term for '&&' operator")
                  end
                  self~next   -- step over the peeked character

                  rightHand = .nil

                  do forever
                      ch = self~peek  -- peek so we don't ruin undoing a .nil read
                      select
                          when ch == '[' then do
                              self~next   -- step over the peeked character
                              -- embedded class...recursively parse, and chain up, if necessary
                              node = self~parseClass
                              if rightHand == .nil then do
                                  rightHand = node
                              end
                              else do
                                  -- chain up the logical operation
                                  rightHand = .ClassAndNode~new(rightHand, node);
                              end
                          end
                          when ch == ']' | ch == '&' then do
                              -- either the complete end, or a section end.
                              leave
                          end
                          otherwise do
                              -- parse out the next section
                              node = self~parseClassSection
                              if rightHand == .nil then do
                                  rightHand = node
                              end
                              else do
                                  -- chain up the logical operation
                                  rightHand = .ClassAndNode~new(rightHand, node);
                              end
                          end
                      end
                  end
                  -- must have both a left term and a right term here
                  if rightHand == .nil then do
                      raise syntax 35.900 array("Missing right term for '&&' operator")
                  end
                  -- This might be the entire term, or needs to be AND'd with
                  -- the working chain
                  if previousNode == .nil then do
                      previousNode = rightHand
                  end
                  else do
                      previousNode = .ClassAndNode~new(previousNode, rightHand)
                  end
              end
              else do
                  -- unread the character...the "&" is just a literal char
                  self~previous
              end
          end
          -- possibly a predefined class or named class family.  This is
          -- handled here
          when ch == '\' then do
              self~next   -- step over the peeked character
              -- see if this is one of the special classes
              subNode = self~parseEscapedClasses
              -- if we have something special here, add this to the
              -- group chain
              if subNode \= .nil then do
                  -- sort of a pain, but if this is a quoted string in the
                  -- class, we need to convert that into a class node
                  if subNode~isa(.StringNode) then do
                      -- create the appropriate class node
                      subNode = self~classNode(subNode~matchString, negated)
                  end
                  -- if we already have a node within the class, we combine this
                  -- using a logical op
                  if .nil == previousNode then do
                      previousNode = subNode
                  end
                  else do
                      previousNode = .ClassOrNode~new(previousNode, subNode)
                  end
              end
              else do
                  self~previous  -- handled in the block parsing
              end
          end
          otherwise do
              nop  -- nothing extra to do here
          end
      end

      -- processed all of the special chars, we should be looking at class characters.
      -- process this into a node.
      node = self~parseClassRange(negated)
      -- if this is a negation, then this is an AND operation
      if negated then do
          if previousNode \= .nil then do
              previousNode = .ClassAndNode~new(previousNode, node)
          end
          else do
              previousNode = node
          end
      end
      else do
          -- not negated, so this just adds to the list
          if previousNode \= .nil then do
              previousNode = .ClassOrNode~new(previousNode, node)
          end
          else do
              previousNode = node
          end
      end
      negated = .false
  end


-- parse a range of characters for a class.  This should either
-- be a set of specific characters (including escaped
-- characters, or a range in the form "x-z".  The range will be terminated
-- by any of the meta characters.  We may also encounter a named family
-- class, in which case we'll return just that piece.  Range negations have
-- already been processed by our caller
::method parseClassRange
  use strict arg negated    -- determines the type of node we return
  firstchar = .true
  characters = ""

  do forever
      ch = self~peek   -- just peek at the next character
      -- we can terminate on the end of data or the closing bracket
      if ch == .nil then do
          leave
      end
      -- is this any of e or a nested range?
      -- stop here
      else if self~isClassMetaCharacter(ch) then do
          -- quit now...we only peeked at this character,
          -- so it's still there
          leave
      end
      -- an escape character...this might be a real escaped character or
      -- a more complex operation.  If it's a character, accept it now, otherwise
      -- stop parsing here and allow the higher level to handle the escaped
      -- operation
      else if ch == '\' then do
          self~next -- step over the escape
          ch = self~parseEscapedCharacters
          -- if this doesn't parse into a character, then step back and
          -- finish up here
          if ch == .nil then do
              self~previous
              leave
          end
          -- we have the character for adding to this range
      end
      -- if the current position is the sequence '&&', this is
      -- an interesection.  If it is a single '&', we use the character
      -- as is
      else if ch == '&' then do
          if self~peekOffset(1) == '&' then do
              -- still positioned at the first '&'.  The
              -- intersection will be processed at a higher level
              leave
          end
          self~next -- step over the character
      end
      else do
          -- Simple character added to the range
          ch = self~next
      end

      -- this could be followed by a '-' character
      if self~peek == '-' then do
          -- skip over that and get the next character
          self~next
          endRange = self~next
          -- missing closing class piece?  Return now and allow caller
          -- to handle
          if endRange == .nil then do
              leave
          end
          -- a '-' at the very end of a range is interpreted as a
          -- '-' character rather than the range operator.  Also,
          -- in a '--' sequence, the second '-' is just a character.
          if \self~isClassMetaCharacter(endRange) then do
              -- if the endRange character is an escape, then we
              -- need to process both characters
              if endRange == '\' then do
                  self~previous
                  endRange = self~singleChar
                  -- if not valid, we have a problem of some sort
                  if endRange == .nil then do
                      leave
                  end
              end
              -- replace the character with the end range
              ch = xrange(ch, endRange)
          end
          else do
              self~previous -- back up and process the closing on the next loop
          end
      end
      -- add this to the accumulator
      characters = characters || ch
  end
  -- return the node based on the class modifier
  return self~classNode(characters, negated)


-- parse a named class family and turn it into an equivalent family class
::method parseNamedClassFamily
  use strict arg negated

  -- pull out the name and create a family
  familyName = self~extractDelimited('{', '}')
  return self~createNamedClassFamily(familyName, negated)


-- create a named class family
::method createNamedClassFamily
  use strict arg familyName, negated

  familyChars = self~getFamily(familyName)
  if familyChars \= .nil then do
     -- now return the appropriate node type for the match
     -- NOTE:  Not sure I know how the caseless flag is supposed to be
     -- applied here.  For example, with the Java implementation,
     -- (?i)/p{Lower} does not match A.  The other sources of information
     -- don't really touch on it.  I'm inclined to have it work with case
     -- insensitive matches, but this is not a strong stance.
     return self~classNode(familyChars, negated)
  end
  -- One convention is the append Is to the name.  We
  -- just drop that part and handle it like a general situation.
  if familyName~match(1, "Is") then do
     subFamilyName = familyName~substr(3)
     familyChars = self~getFamily(subFamilyName)
     if familyChars \= .nil then do
        return self~classNode(familyChars, negated)
     end
  end

  -- did not find this, raise an error
  raise syntax 93.900 array("Unknown named class family '"familyName"'")


-- parse out a section of as-is characters that conforms to an atom
-- specification
::method parseAtom
  characters = ""
  do forever
      ch = self~next
      select
          when ch = .nil then do
              leave
          end
          when ch == '\' then do
              ch = self~parseEscapedCharacters
              -- this is either an invalid escape char, or an operation
              -- we push the char back on, and stop processing.
              if ch == .nil then do
                  self~previous
                  leave
              end
          end
          -- if this is a non-escaped special char,
          -- backup and finish up this section
          when self~isMetaCharacter(ch) then do
              self~previous
              leave
          end

          otherwise do
              -- We have a good character, but it might be followed
              -- by a modifier.  If it is, push it back and terminate
              -- the parsing here.
              test = self~peek
              if test \= .nil, self~isClosureCharacter(test) > 0  then do
                  -- if there are more than one character here,
                  -- back up and return what we have.
                  if characters \== '' then do
                      self~previous
                  end
                  else do
                      -- otherwise, this single character is the entire atom
                      -- the closure characters will apply to it
                      characters = ch
                  end
                  leave
              end

              characters = characters || ch
          end
      end
  end

  return self~stringNode(characters)


-- create a node for matching a character string
::method stringNode
  use strict arg string

  if self~caselessMode then do
      return .CaselessStringNode~new(string)
  end
  else do
      return .StringNode~new(string)
  end


-- create a node for matching a a range of characters.  Which
-- node gets created depends on whether this is a negation operation
-- and whether caseless matches are called for.
::method classNode
  use strict arg characters, negated
  -- if this is a ^ form, then we need the appropriate reverse node
  if negated then do
      if self~caselessMode then do
          return .CaselessNotClassNode~new(characters)
      end
      else do
          return .NotClassNode~new(characters)
      end
  end
  -- normal matching mode, which also has two forms
  else do
      if self~caselessMode then do
          return .CaselessClassNode~new(characters)
      end
      else do
          return .ClassNode~new(characters)
      end
  end


-- parse out an escaped character
::method parseEscapedCharacters private
  ch = self~next

  select
      when ch == '0' then do
          -- octal character value
          firstDigit = self~readOctal
          if .nil \= firstDigit then do
              secondDigit = self~readOctal
              if .nil \= secondDigit then do
                  thirdDigit = self~readOctal
                  if .nil \= thirdDigit then do
                      if firstDigit <= 3 then do
                          return d2c((firstDigit * 64) + (secondDigit * 8) + thirdDigit)
                      end
                      -- not a value 3 digit octal, so only use 2
                      self~previous
                  end
                  return d2c((firstDigit * 8) + secondDigit)
              end
              return d2c(firstDigit)
          end

          raise syntax 93.900 array("Missing octal character")
      end
      -- alert (bell)
      when ch == 'a' then do
          return '07'x
      end
      -- backspace (only valid in a class...this is a word boundary elsewhere)
      when ch == 'b' then do
          return '08'x
      end
      -- control character corresponding to "x".  The
      -- allowed control charaters are the values a-z or A-Z.  The
      -- resulting character will be the value '01'x through '1a'x (decimal 26)
      -- Either upper or lowercase letters are permitted
      when ch == 'c' then do

          control = self~next
          if .nil == control then do
              raise syntax 93.900 array("Character expected after \c escape")
          end
          -- check and convert the lower case characters
          ctrl = .RegexCompiler~LOWERCASECHARACTERS~pos(control)
          if ctrl \= 0 then do
              return d2c(ctrl)
          end
          -- now uppercase versions
          ctrl = .RegexCompiler~UPPERCASECHARACTERS~pos(control)
          if ctrl \= 0 then do
              return d2c(ctrl)
          end
          raise syntax 93.900 array("Invalid \c control character name")
      end
      when ch == 'e' then do
          return '1b'x  -- escape char
      end
      when ch == 'f' then do
          return '0c'x  -- form feed
      end
      when ch == 'n' then do
          return .String~nl  -- new line
      end
      when ch == 'r' then do
          return .String~cr  -- carriage return
      end
      when ch == 't' then do
          return .String~tab  -- horizontal tab
      end
      when ch == 'v' then do
          return '0b'x  -- vertical tab
      end
      when ch == 'x' then do -- hex encoded character
          -- hex value
          return x2c(self~readHex || self~readHex)
      end
      otherwise do
          -- if not one of the reserved characters at this point, we return the escaped char
          if \'0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ<>'~contains(ch) then do
              return ch
          end

          -- this is either an operation or invalid.  We leave that for another phase
          self~previous
          return .nil
      end
  end


-- special escaped operations, including the special character classes
::method parseEscapedOperations private
  -- try for one of the classes first
  class = self~parseEscapedClasses
  -- done
  if class \== .nil then do
      return class
  end
  -- The next category of escape are the different anchor/group indicators.
  -- These are only allowed at a top-level context
  ch = self~next

  select
      when .nil == ch then do
          raise syntax 93.900 array("Missing character after escape")
      end
      -- numbered back reference
      when '123456789'~contains(ch) then do
          -- a back reference
          if \self~haveGroup(ch) then do
              raise syntax 93.900 array("Unrecognized group back reference" ch)
          end
          if self~caselessMode then do
              return .CaselessGroupBackReferenceNode~new(ch)
          end
          else do
              return .GroupBackReferenceNode~new(ch)
          end
      end
      -- named back reference
      when ch == 'k' then do
          -- a \k back reference can be complicated
          return self~parseNamedGroup
      end
      -- reference to a registered pattern
      when ch == 'm' then do
          -- get the pattern name
          patternName = self~extractDelimited('{', '}')
          patternName = patternName~upper
          -- a reference to a registered pattern impliclitly creates
          -- a named group that can be used to retrieve the information.  However,
          -- since it is possible to use a named pattern in more than one position,
          -- the group can be explicitly named by specifying {group:pattern}.
          if patternName~contains(":") then do
             parse var patternName groupName ":" patternName
          end
          else do
             groupName = patternName
          end

          -- see if we can resolve the pattern.
          pattern = self~getPattern(patternName)

          if pattern == .nil then do
              raise syntax 93.900 array("Unrecognized pattern reference:" patternName)
          end

          -- check that this is unique
          if self~haveGroup(groupName) then do
              raise syntax 93.900 array("Duplicate named group in pattern:" groupName)
          end

          -- add this to our reference table
          self~getGroupReference(groupName)

          -- this handles the indirection
          return .PatternNode~new(groupName, patternName, pattern)
      end
      -- start of text anchor
      when ch == 'A' then do
          return .BeginTextNode~new
      end
      -- NOT on a word boundary anchor
      when ch == 'B' then do
          return .NotClassBoundaryNode~new(self~createNamedClassFamily("WORD", .false))
      end
      -- ON a word boundary anchor
      when ch == 'b' then do
          return .ClassBoundaryNode~new(self~createNamedClassFamily("WORD", .false))
      end
      -- Beginning word boundary anchor
      when ch == '<' then do
          return .BeginClassBoundaryNode~new(self~createNamedClassFamily("WORD", .false))
      end
      -- End of word boundary anchor
      when ch == '>' then do
          return .EndClassBoundaryNode~new(self~createNamedClassFamily("WORD", .false))
      end
      -- Last match.  We do iteration a little differently, so this
      -- really matches the start of the text region we're looking at.
      when ch == 'G' then do
          return .LastMatchNode~new
      end
      -- this is like $, but it always is an end of input or linend
      -- match, rather than multiline
      when ch == 'Z' then do
          if self~unixLinesMode then do
              return .UnixLineEndNode~new
          end
          else do
              return .InternetLineEndNode~new
          end
      end
      -- always match the end of the text
      when ch == 'z' then do
          return .TextEndNode~new
      end
      otherwise  do
          self~previous -- revert the read character
          return .nil -- return the failure indicator
      end
  end


-- parse a group back reference.  The back reference is in the
-- form <group{result{group...}}> for as many levels as are needed.
-- Thus <group> will return the match value for named group "group".
-- <group{result}> will return the named result "result" within group
-- "group", while <group{result{subgroup}}> will return the value of
-- group "subgroup" within named result "result" of group "group"
::method parseNamedGroup
  -- parse the potentially lengthy group name
  path = self~parseGroupName

  -- the first level is all we can verify exists at this point
  if \self~haveGroup(path[1]) then do
      raise syntax 93.900 array("Unrecognized group back reference:" path[1])
  end

  -- just a simple group reference?
  if path~items == 1 then do
      if self~caselessMode then do
          return .CaselessGroupBackReferenceNode~new(path[1])
      end
      else do
          return .GroupBackReferenceNode~new(path[1])
      end
  end
  -- more complex path resolution required
  else do
      if self~caselessMode then do
          return .CaselessResultBackReferenceNode~new(path)
      end
      else do
          return .ResultBackReferenceNode~new(path)
      end
  end


-- parse the group portion of a resolution path.  This will be
-- of the form <name{groupname/groupname/groupname/...}>
::method parseGroupName
  -- this is the path accumulator array
  path = .array~new
  self~next -- step over the '<' delimiter that started this
  referenceName = ""  -- the accumulated name
  loop forever
      ch = self~next -- get the next character
      if ch == .nil then do
              raise syntax 93.900 array("Missing closing '>' on group name")
      end
      -- end of the group name portion?
      else if ch == '>' then do
          -- an empty reference
          if referenceName == "" then do
              referenceName = "0"
          end
          -- apply Rexx symbol rules to the name and make uppercase
          referenceName = referenceName~upper
          path~append(referenceName)
          return path
      end
      -- is this a fuller path specification?
      else if ch == '/' then do
          -- we allow a default here...going right to the
          -- result means use the 0 group.
          if referenceName == "" then do
              referenceName = "0"
          end
          -- apply Rexx symbol rules to the name and make uppercase
          referenceName = referenceName~upper
          path~append(referenceName)
          -- get the result portion
          self~parseSubgroupName(path)
          -- perform the delimiter check
          ch = self~next
          if ch \== '>' then do
              raise syntax 93.900 array("Missing closing '>' on group name")
          end
          return path
      end
      else do
          referenceName = referenceName || ch
      end
  end


-- parse additions pieces of a resolution path after the first
-- element.  These will be the names of nested named groups separated
-- by '/'.   Note:  After the  first level, all additional references
-- must be to named groups.
::method parseSubgroupName
  -- this is the path accumulator array
  use strict arg path

  -- we should be positioned on the first character of the next named group
  -- now.
  referenceName = ""  -- the accumulated name

  loop forever
      ch = self~next -- get the next character
      if ch == .nil then do
         raise syntax 93.900 array("Missing closing '>' in group path specification")
      end
      -- end of the group name portion?
      else if ch == '>' then do
          -- we must have something here
          if referenceName == "" then do
             raise syntax 93.900 array("Missing group name in path specification")
          end
          -- apply Rexx symbol rules to the name and make uppercase
          referenceName = referenceName~upper
          path~append(referenceName)
          -- need to back up on the terminator
          self~previous
          return
      end
      -- Next step in the path
      else if ch == '/' then do
          -- we must have something here
          if referenceName == "" then do
             raise syntax 93.900 array("Missing group name in path specification")
          end
          -- apply Rexx symbol rules to the name and make uppercase
          referenceName = referenceName~upper
          path~append(referenceName)
          -- clear the name for the next path through the loop
          referenceName = ""
          -- step forward to the first name characters and head around
      end
      -- just add this character to the name
      else do
          referenceName = referenceName || ch
      end
  end


-- special escaped character classes, including the named class families
::method parseEscapedClasses
  ch = self~next

  select
      when .nil == ch then do
          raise syntax 93.900 array("Missing character after escape")
      end
      -- NOT a digit
      when ch == 'D' then do
          -- no need to handle caseless matches here
          return self~createNamedClassFamily("Digit", .true)
      end
      -- Is a digit
      when ch == 'd' then do
          -- no need to handle caseless matches here
          return self~createNamedClassFamily("Digit", .false)
      end
      -- quoted literal..return the entire group
      when ch == 'Q' then do
          return self~parseLiteral
      end
      -- NOT whitespace
      when ch == 'S' then do
          -- no need to handle caseless matches here
          return self~createNamedClassFamily("Space", .true)
      end
      -- is whitespace
      when ch == 's' then do
          -- no need to handle caseless matches here
          return self~createNamedClassFamily("Space", .false)
      end
      -- NOT a word character
      when ch == 'W' then do
          -- This includes all of the upper and lower case letters, so no
          -- caseless considerations to worry about
          return self~createNamedClassFamily("Word", .true)
      end
      -- is a word character
      when ch == 'w' then do
          -- This includes all of the upper and lower case letters, so no
          -- caseless considerations to worry about
          return self~createNamedClassFamily("Word", .false)
      end
      -- named class family
      when ch == 'p' then do
          -- parse out the named property
          return self~parseNamedClassFamily(.false)
      end
      -- negation of the a named class family
      when ch == 'P' then do
          -- parse out the named property
          return self~parseNamedClassFamily(.true)
      end
      otherwise  do
          -- back off the character..might be some other type of operation
          self~previous
          return .nil   -- return a failure indicator
      end
  end


-- parse out a literal defined by using the \Q and \E delimiters
::method parseLiteral private
  current = self~current
  endPosition = -1

  do forever
      ch = self~next
      if .nil == ch then do
          endPosition = self~current
          leave
      end
      if ch == '\' then do
          modifier = self~peek
          if .nil == modifier then do
              raise syntax 93.900 array("Missing closing \E following \Q")
          end
          if modifier == 'E' then do
              endPosition = self~current - 1
              self~next  -- step over the closing bit
              leave
          end
      end
  end

  return self~stringNode(self~extract(current, endPosition))


-- read and validate a octal value in an escaped value, returning the
-- eqivalent character value.  Since octal escapes are variable length,
-- we just return .nil rather than raise a syntax error.
::method readOctal
  ch = self~next
  if ch == .nil then do
      return .nil
  end

  if ch < '0' | ch > '7' then do
      self~previous
      return .nil
  end
  return ch


-- read and validate a hex encoded character in an escaped value, returning
-- the character equivalent
::method readHex
  ch = self~next

  if .nil \= ch then do
      if \'1234567890abcdefABCDEF'~contains(ch) then do
          raise syntax 93.900 array("Invalid hex digit '"ch"'")
      end
      return ch
  end
  -- we can directly raise a syntax error for reading hex
  raise syntax 93.900 array("Hex digit expected")

-- Start of nodes that implement the various matching algorithms



-- base class for all matching nodes
::class "MatchNode"
::method init
  expose next
  next = .nil


-- this is the next element in the matching node chain.
::attribute next


-- invoke the next handler in the chain
::method matchNext
  expose next
  use strict arg context, position, target

  -- if there is no node attached, assume it matches
  if next == .nil then do
      -- set the end match position
      context~matchEnd = position
      -- this is always a successful match.  This really just
      -- records the last position of this branch segment.
      return .true
  end
  -- just send this along
  return next~match(context, position, target)


-- calculate the length of this match, if possible.  The
-- default implementation assumes the length of this node is zero and
-- just passes the request down the chain
::method calculateMatchMetrics
  expose next
  use strict arg metrics
  if next \== .nil then do
      next~calculateMatchMetrics(metrics)
  end


-- calculate the length of the submetrics for a given child node.  This
-- creates a completely new set of metrics rather than add to the existing
-- ones.
::method calculateChildMatchMetrics
  use strict arg child
  -- get our repetition node values as if it was the only thing of interest
  submetrics = .MatchMetrics~new

  child~calculateMatchMetrics(submetrics)
  return submetrics


-- add a .TerminatorNode terminator to a node that will
-- be used as part of a composite
::method terminate
  expose next

  -- if we don't have a successor yet or our successor
  -- is a normal terminator, then set our successor to be
  -- the branch version.  The latter case generally happens
  -- when a parsed sequence is wrapped in an alteration
  if next == .nil | next~isa(.TerminatorNode) then do
      -- matchNext handles the .nil like it was a terminator
      -- terminate is only called for side branches, so we can
      -- eliminate that extra node
      self~next = .nil
  end
  else do
      -- we have a child node, make sure that child has a
      -- terminator
      self~next~terminate
  end



-- A node that will match anything.  This is typically
-- used when an alternative is expressed as "this|that|".
-- The trailing "|" indicates that we always have a match,
-- even if it is zero-length
::class "EveryThingNode" subclass MatchNode
::method match
  use strict arg context, position, target
  -- we always match, so just send along to the successor
  return self~matchNext(context, position, target)



-- A node that enforces anchoring at the beginning of the text
::class "BeginTextNode" subclass MatchNode
::method match
  use strict arg context, position, target

  -- if not at the beginning, this fails
  if \context~atStart(position) then do
      return .false
  end
  -- forward and complete
  return self~matchNext(context, position, target)



-- A node that enforces anchoring at the end of the text
::class "EndTextNode" subclass MatchNode
::method match
  use strict arg context, position, target
  -- must be at the end to match
  if \context~checkEnd(position) then do
      return .false
  end
  -- forward and complete
  return self~matchNext(context, position, target)



-- A node that enforces a match at the beginning of a boundary defined
-- by a class of characters
::class "ClassBoundaryNode" subclass MatchNode
::method init
  expose boundaryClass
  use strict arg boundaryClass

  self~init:super

  -- make sure we have an explicit terminator set
  boundaryClass~terminate


::method match
  use strict arg context, position, target

  -- At the very end (e.g., past the end really), the last character
  -- must be of the boundary character.  Note that we need to check for a
  -- null string condition too
  if context~checkEnd(position) then do
      -- also at the beginning means this is a
      -- null string.  No characters to check means not a
      -- word boundary
      if context~atStart(position) then do
          return .false
      end
      -- not a word character in the previous position?  This fails
      if \self~checkBoundary(context, position - 1, target) then do
          return .false
      end
      -- good so far, pass this along
      return self~matchNext(context, position, target)
  end

  -- if at the start of the range, the character must be of the class character
  -- NB, the at the end check above handled the null string case, so we
  -- can check the first character unconditionally
  if context~atStart(position) then do
      -- not a class character in the current position?  This fails
      if \self~checkBoundary(context, position, target) then do
          return .false
      end
      -- good so far, pass this along
      return self~matchNext(context, position, target)
  end
  -- exclusive OR situation.  Only one of these should be a class character to
  -- qualify as a boundary
  if self~checkBoundary(context, position, target) && self~checkBoundary(context, position - 1, target) then do
      return self~matchNext(context, position, target)
  end

  return .false


-- simple method to perform the boundary test.  A negative test
-- will override this
::method checkBoundary
  expose boundaryClass
  use strict arg context, position, target

  return boundaryClass~match(context, position, target)



-- the inverse of a  class boundary match.  Matches only if the current
-- position is NOT on a class boundary
::class "NotClassBoundaryNode" subclass ClassBoundaryNode
::method match
  use strict arg context, position, target

  -- At the very end (e.g., past the end really), the last character
  -- must not be a boundary character.  Note that we need to check for a
  -- null string condition too
  if context~checkEnd(position) then do
      -- also at the beginning means this is a
      -- null string.  No characters to check means not a
      -- word boundary
      if context~atStart(position) then do
          -- a null string is not a word boundary, so this passes
          return self~matchNext(context, position, target)
      end
      -- a boundary character in the previous position?  This fails
      if self~checkBoundary(context, position - 1, target) then do
          return .false
      end
      -- good so far, pass this along
      return self~matchNext(context, position, target)
  end

  -- if at the start of the range, the character must be a boundary character
  -- NB, the at the end check above handled the null string case, so we
  -- can check the first character unconditionally
  if context~atStart(position) then do
      -- a boundary character in the current position?  This fails
      if self~checkBoundary(context, position, target) then do
          return .false
      end
      -- good so far, pass this along
      return self~matchNext(context, position, target)
  end
  -- exclusive OR situation.  If one is a boundary and the other not, this is a boundary
  -- and thus not a match.
  if self~checkBoundary(context, position, target) && self~checkBoundary(context, position - 1, target) then do
      return .false
  end
  -- forward along
  return self~matchNext(context, position, target)



-- Test if on the beginning boundary of a character class.   To qualify,
-- the current position must be a class character and the previous
-- character must NOT be a class character or be the beginning of
-- the range
::class "BeginClassBoundaryNode" subclass ClassBoundaryNode
::method match
  use strict arg context, position, target

  -- At the very end (e.g., past the end really), by definition this
  -- cannot be a beginning word boundary.
  if context~checkEnd(position) then do
      return .false
  end

  -- if the current character is not a class character, this fails too
  if \self~checkBoundary(context, position, target) then do
      return .false
  end
  -- if the position is the start of the range,
  -- this passes
  if context~atStart(position) then do
      return self~matchNext(context, position, target)
  end
  -- the previous character must NOT be a class character to pass
  if \self~checkBoundary(context, position - 1, target) then do
      return self~matchNext(context, position, target)
  end

  return .false



-- Test if on the ending boundary of a character class.   To qualify,
-- the current position must be the end of the range or,
-- NOT be a class character and the previous character must
-- be a class character.
::class "EndClassBoundaryNode" subclass ClassBoundaryNode
::method match
  use strict arg context, position, target

  -- At the very beginning?  by definition this
  -- cannot be a ending word boundary.
  if context~atStart(position) then do
      return .false
  end

  -- if the previous character is not a class character, this fails too
  if \self~checkBoundary(context, position - 1, target) then do
      return .false
  end

  -- if the position is the end, this passes
  if context~checkEnd(position) then do
      return self~matchNext(context, position, target)
  end

  -- the current character MUST NOT be a word character to pass
  if \self~checkBoundary(context, position, target) then do
      return self~matchNext(context, position, target)
  end

  return .false



-- retrieves the last match information.  Since a match context
-- only implements a single match, this is the same as the starting
-- position of the context.
::class "LastMatchNode" subclass MatchNode
::method match
  use strict arg context, position, target

  -- this must be the start of the match region
  if position \= context~regionStart then do
      return .false
  end

  return self~matchNext(context, position, target)



-- Generic "dot".  This matches everything but the end of
-- of data
::class "AllDotNode" subclass MatchNode
::method match
  use strict arg context, position, target

  if context~checkEnd(position) then do
      return .false
  end

  -- always true at this node, so just return the successor result
  -- starting from the next character
  return self~matchNext(context, position + 1, target)


::method calculateMatchMetrics
  use strict arg metrics
  -- a dot is always a single character, so easy to manage
  metrics~addLength(1)
  self~calculateMatchMetrics:super(metrics)



-- like all dot node, but doesn't match lineends (CRLF sequences,
-- in this case)
::class "InternetDotNode" subclass AllDotNode
::method match
  use strict arg context, position, target

  if context~checkEnd(position) then do
      return .false
  end

  -- if positioned on ANY linend char, this is a failure
  if target~matchChar(position, .RegexCompiler~CRLF) then do
      return .false
  end

  -- always true at this point, so just return the successor result.
  return self~matchNext(context, position + 1, target)



-- like all dot node, but doesn't match lineends (Unix mode, so
-- this is just linefeeds)
::class "UnixDotNode" subclass AllDotNode
::method match
  use strict arg context, position, target

  if context~checkEnd(position) then do
      return .false
  end

  -- if positioned on a linefeed char, this is a failure
  if target~match(position, .String~nl) then do
      return .false
  end

  -- always true at this point, so just return the successor result.
  return self~matchNext(context, position + 1, target)



-- match a string of characters
::class "StringNode" subclass matchNode
::method init
  expose matchString
  use strict arg matchString
  self~init:super


::attribute matchString   -- useful for debugging


-- base method for performing string matches.  Other subclasses
-- can override this to perform the match differently
::method isMatch
  use strict arg target, position, matchString

  -- this requires an exact match
  return target~match(position, matchString)


::method match
  expose matchString
  use strict arg context, position, target

  -- must have at least the same length as the string available
  if context~checkEnd(position + matchString~length - 1) then do
      return .false
  end
  -- if no match, fail
  if \self~isMatch(target, position, matchString) then do
      return .false
  end
  -- continue checking after this position
  return self~matchNext(context, position + matchString~length, target)


::method calculateMatchMetrics
  expose matchString
  use strict arg metrics
  -- always the length of this string
  metrics~addLength(matchString~length)
  self~calculateMatchMetrics:super(metrics)



-- a caseless match for a string
::class "CaselessStringNode" subclass StringNode
-- override of superclass method to perform a caseless match
-- can override this to perform the match differently
::method isMatch
  use strict arg target, position, matchString

  -- this requires an exact match
  return target~caselessMatch(position, matchString)



-- match for a defined set of characters
::class "ClassNode" subclass MatchNode
::method init
  expose characters
  use strict arg characters
  self~init:super


-- the characters we check against
::attribute characters GET


::method match
  expose characters
  use strict arg context, position, target

  if context~checkEnd(position) then do
      return .false
  end
  -- must match at least one of these
  if \self~characterMatch(position, target, characters) then do
      return .false
  end

  return self~matchNext(context, position + 1, target)


-- apply the appropriate character match operation for a given subclass
::method characterMatch
  use strict arg position, target, characters
  return target~matchChar(position, characters)


::method calculateMatchMetrics
  use strict arg metrics
  -- also just a single character match
  metrics~addLength(1)
  self~calculateMatchMetrics:super(metrics)



-- an inversion of the class node
::class "NotClassNode" subclass ClassNode
-- apply the appropriate character match operation for a given subclass
::method characterMatch
  use strict arg position, target, characters
  -- this must NOT match any of the charactets
  return \target~matchChar(position, characters)



-- caseless match for a defined set of characters
::class "CaselessClassNode" subclass ClassNode
-- apply the appropriate character match operation for a given subclass
::method characterMatch
  use strict arg position, target, characters
  -- this must match any of the characters
  return target~caselessMatchChar(position, characters)



-- caseless match for a defined set of characters
::class "CaselessNotClassNode" subclass ClassNode
-- apply the appropriate character match operation for a given subclass
::method characterMatch
  use strict arg position, target, characters
  -- this must NOT match any of the characters
  return \target~caselessMatchChar(position, characters)



-- the intersection of multiple class node definitions
::class "ClassIntersectionNode" subclass MatchNode
::method init
  expose subExpressions
  use strict arg subExpressions

  self~init:super


::method match
  expose subExpressions
  use strict arg context, position, target

  -- perform a match operation on each in turn
  do test over subExpressions
      if \test~match(context, position, target) then do
          return .false
      end
  end

  -- the intersection worked, forward on
  return self~matchNext(context, position + 1, target)


::method calculateMatchMetrics
  use strict arg metrics
  -- also just a single character match
  metrics~addLength(1)
  self~calculateMatchMetrics:super(metrics)



-- match on a series of OR expressions
::class "AlternativeNode" subclass MatchNode
::method init
  expose alternatives

  -- these are accumulated during parsing
  alternatives = .array~new
  self~init:super


::method match
  expose alternatives
  use strict arg context, position, target

  -- return on the first match
  do alternative over alternatives
      -- we try each alternative, then the subsequent
      -- nodes.  We only return true IFF we get a match
      -- in both places.  This really is a branch point, not
      -- a "select the one that matches" situation
      if alternative~match(context, position, target)  then do
          -- once we get a match, send this along to the next
          if self~matchNext(context, context~matchEnd, target) then do
              return .true
          end
      end
  end

  -- none of the branches matched all the way out
  return .false


-- calculate match metrics for an alternative node
::method calculateMatchMetrics
  expose alternatives
  use strict arg metrics

  -- create a metrics item for the merge
  accumulator = .MatchMetrics~new
  -- set a large minimum for the start
  accumulator~minLength = 999999999

  -- we need to analyze all of the alternative paths.
  do alternative over alternatives
      -- get our repetition node values as if it was the only thing of interest
      submetrics = self~calculateChildMatchMetrics(alternative)
      -- merge these into our accumulator
      accumulator~mergeMetrics(submetrics)
  end
  -- add these into full set
  metrics~addMetrics(accumulator)
  -- send this along
  self~calculateMatchMetrics:super(metrics)


-- add an expression to our list of alternatives
::method addAlternative
  expose alternatives
  use strict arg newChoice

  newChoice~terminate   -- make sure this has a terminator
  alternatives~append(newChoice)



-- check for a lineend in non-unix mode.  This matches the END of input or
-- on a '\r\n' sequence, but only if the lineend sequence is at the very end
::class "InternetLineEndNode" subclass MatchNode
::method match
  use strict arg context, position, target

  -- at the end, this is automatically a match.
  if context~checkEnd(position) then do
      -- we still need to forward this along, even if we've hit the
      -- end position to ensure the terminators get poked.
      return self~matchNext(context, position, target)
  end

  -- the end position will be before the \r\n sequence, if it matches
  endPosition = context~endPosition - 2

  -- Current position too early to be a terminal linend?
  -- this is definitely not a match.
  if position \= endPosition then do
      return .false
  end

  -- must be a \r\n sequence here, or this is not a match
  if \target~match(position, .RegexCompiler~CRLF) then do
      return .false
  end

  -- we still need to forward this along, even if we've hit the
  -- end position to ensure the terminators get poked.  NOTE:  We do
  -- NOT step over the linend here.  The following characters still
  -- exist, but are not skipped over
  return self~matchNext(context, position, target)



-- check for a lineend in non-unix mode.  This matches on a '\n' character
-- or the END of input, but only if the lineend character is at the very end
::class "UnixLineEndNode" subclass MatchNode
::method match
  use strict arg context, position, target

  -- at the end is automatically a match
  if context~checkEnd(position) then do
      -- we still need to forward this along, even if we've hit the
      -- end position to ensure the terminators get poked.
      return self~matchNext(context, position, target)
  end

  -- the ending position is one back, since we only look for a single linefeed.
  endPosition = context~endPosition - 1

  -- Current position too early to be a terminal linend?
  -- this is definitely not a match
  if position < endPosition then do
      return .false
  end

  -- must be a \n character here, or this is not a match
  if \target~match(position, .String~nl) then do
      return .false
  end

  -- we still need to forward this along, even if we've hit the
  -- end position to ensure the terminators get poked.  NOTE:  We do
  -- NOT step over the linend here.  The following characters still
  -- exist, but are not skipped over
  return self~matchNext(context, position, target)



-- check for a lineend in non-unix, multiline mode.  This matches on a '\r\n' sequence
-- OR the end of the input
::class "InternetMultiLineEndNode" subclass MatchNode
::method match
  use strict arg context, position, target

  -- end match, this is true
  if context~checkEnd(position) then do
      -- we still need to forward this along, even if we've hit the
      -- end position to ensure the terminators get poked.
      return self~matchNext(context, position, target)
  end

  endPosition = context~endPosition - 2

  -- not enough room for a linend?
  if position > endPosition then do
      return .false
  end

  -- if the current position does not match a linend sequence, this fails
  if \target~match(position, .RegexCompiler~CRLF) then do
      return .false
  end
  -- we still need to forward this along, even if we've hit the
  -- end position to ensure the terminators get poked.  NOTE:  We do
  -- NOT step over the linend here.  The following characters still
  -- exist, but are not skipped over
  return self~matchNext(context, position, target)



-- check for a lineend in unix mode.  This matches on a '\n' sequence
-- but NOT the end of the input
::class "UnixMultiLineEndNode" subclass MatchNode
::method match
  use strict arg context, position, target

  -- end match, this is true
  if context~checkEnd(position) then do
      -- we still need to forward this along, even if we've hit the
      -- end position to ensure the terminators get poked.
      return self~matchNext(context, position, target)
  end

  endPosition = context~endPosition - 1

  -- if the current position does not match a linend sequence, this fails
  if \target~match(position, .String~nl) then do
      return .false
  end

  -- we still need to forward this along, even if we've hit the
  -- end position to ensure the terminators get poked.  NOTE:  We do
  -- NOT step over the linend here.  The following characters still
  -- exist, but are not skipped over
  return self~matchNext(context, position, target)



-- check for being at the end of the text string.  Does not recognize linend sequences.
::class "TextEndNode" subclass MatchNode
::method match
  use strict arg context, position, target

  if context~checkEnd(position) then do
      return self~matchNext(context, position, target)
  end

  return .false



-- Base class for the different Question nodes.  The init and
-- calculateMatchMetrics are the same, but the matching logic is different
::class "QuestionNode" subclass MatchNode
::method init
  expose optional
  use strict arg optional

  self~init:super
  optional~terminate  -- these needs termination


-- set if this is an optional value
::attribute optional GET


::method calculateMatchMetrics
  expose optional
  use strict arg metrics

  -- our following match pattern is optional, so we ignore any
  -- additions it makes to the metrics before sending it down the chain.
  minLength = metrics~minLength
  optional~calculateMatchMetrics(metrics)
  metrics~minLength = minLength
  -- this is no longer deterministic because it depends on context
  metrics~deterministic = .false
  self~calculateMatchMetrics:super(metrics)



-- a question node that implements greedy matching semantics.  It will
-- match itself first, and back off if it does not find a match.
::class "GreedyQuestionNode" subclass QuestionNode
::method match
  use strict arg context, position, target

  -- if we match the optional part, and if we match the following part,
  -- this is gold.
  if self~optional~match(context, position, target) then do
      if self~matchNext(context, context~matchEnd, target) then do
          return .true
      end
  end

  -- try again, but without the optional section.
  return self~matchNext(context, position, target)



-- reluctant question node.  If the following part matches,
-- it will not perform its own match.
::class "ReluctantQuestionNode" subclass QuestionNode
::method match
  use strict arg context, position, target

  -- first try for a match on the trailing part.  If that matches, we ignore
  -- the optional section
  if self~matchNext(context, position, target) then do
      return .true
  end

  -- try again, but without the optional section.
  if \self~optional~match(context, position, target) then do
      return .false
  end

  -- now match the following part
  return self~matchNext(context, context~matchPosition, target)



-- possessive question node.  If it has a match itself, it will
-- not back up...EVER!
::class "PossessiveQuestionNode" subclass QuestionNode
::method match
  use strict arg context, position, target

  -- try for the optional match...no backtracking if it is a match.
  if self~optional~match(context, position, target) then do
      position = context~matchEnd
  end

  -- either match from the original position, or after the matched position
  return self~matchNext(context, position, target)



-- a mixin used for the different varieties of group nodes to implement
-- common methods
::class "GroupNodeMixin" mixinclass MatchNode
-- set the ID attribute
::method setGroupID
  expose id
  use strict arg id


  -- get the group reference for a question node.  This will also
  -- set this as the current group in the group context
::method getGroupReference
  expose id
  use strict arg context


  -- if this is a capturing group, return the
  -- saved item.  Otherwise, return a dummy to handle
  -- the save/restore logic
  if id \= .nil then do
      group = context~getGroupReference(id)
  end
  else do
      group = .GroupReference~new("")
  end

  -- make this the current group
  context~enterGroup(group)
  return group


-- invoke the next handler in the chain.  This is an
-- override that will pop the group context before
-- making a call out to a successor node.
::method matchNext
  use strict arg context, position, target

  context~exitGroup   -- we're done
  return self~matchNext:super(context, position, target)


-- invoke the next handler in the chain.  This will pop and
-- push the group context around the call out
::method wrappedMatchNext
  use strict arg context, position, target

  info = context~exitGroup   -- we're leaving this context
  ret = self~matchNext:super(context, position, target)
  context~enterGroup(info)  -- reset this as the current group
  return ret



-- Base class for the different Group Question nodes.  The init and
-- calculateMatchMetrics are the same, but the matching logic is different
::class "GroupQuestionNode" subclass QuestionNode inherit GroupNodeMixin
::method init
  expose id
  use strict arg id, optional

  -- initialize the main super class
  self~init:super(optional)

  -- and also the mixin
  self~setGroupId(id) -- the mixing manages the id portion


::method calculateMatchMetrics
  use strict arg metrics

  -- our following match pattern is optional, so we ignore any
  -- additions it makes to the metrics before sending it down the chain.
  minLength = metrics~minLength
  self~optional~calculateMatchMetrics(metrics)
  metrics~minLength = minLength
  -- also no longer deterministic
  metrics~deterministic = .false

  self~calculateMatchMetrics:super(metrics)



-- A node that handles the Greedy ? qualifier following a
-- group.  This implements the ? matching symantics and
-- updates the group position accordingly
::class "GreedyGroupQuestionNode" subclass GroupQuestionNode
::method match
  use strict arg context, position, target

  -- save the group context
  info = self~getGroupReference(context)

  -- if we match the optional part, and if we match the following part,
  -- this is gold.
  if self~optional~match(context, position, target) then do
      -- update the group capture information
      info~setMatch(context, position, context~matchEnd)

      -- handle as a group match
      if self~wrappedMatchNext(context, context~matchEnd, target) then do
          context~exitGroup -- we're done
          return .true
      end
  end
  else do
      info~participated = .false  -- We match if not there, but do not "participate"
  end

  -- we always match, even if it is nothing
  info~setMatch(context, position, position)

  -- try again, but without the optional section.
  return self~matchNext(context, position, target)



-- A node that handles the Reluctant ? qualifier following a
-- group.  This implements the ? matching symantics and
-- updates the group position accordingly
::class "ReluctantGroupQuestionNode" subclass QuestionNode
::method match
  use strict arg context, position, target

  info = self~getGroupReference(context)
  -- we always match, even if it is nothing
  info~setMatch(context, position, position)

  -- first try for a match on the trailing part.  If that matches, we ignore
  -- the optional section
  if self~wrappedMatchNext(context, position, target) then do
      info~participated = .false  -- We match if skipped, but do not "participate"
      context~exitGroup -- we're done
      return .true
  end

  -- try again, but with our optional section.
  if \self~optional~match(context, position, target) then do
      info~participated = .false  -- We match if skipped, but do not "participate"
      context~exitGroup -- we're done
      return .false
  end

  -- update the group capture information
  info~setMatch(context, position, context~matchEnd)
  -- now try the final part
  return self~matchNext(context, context~matchPosition, target)



-- A node that handles the Possessive ? qualifier following a
-- group.  This implements the ? matching symantics and
-- updates the group position accordingly
::class "PossessiveGroupQuestionNode" subclass GroupQuestionNode
::method match
  use strict arg context, position, target

  info = self~getGroupReference(context)
  -- we always match, even if it is nothing
  info~setMatch(context, position, position)

  -- try for the optional match...no backtracking if it is a match.
  if self~optional~match(context, position, target) then do
      -- update the group capture information
      info~setMatch(context, position, context~matchEnd)
      position = context~matchEnd
  end
  else do
      info~participated = .false  -- We match if skipped, but do not "participate"
  end

  return self~matchNext(context, position, target)



-- match a repetition of nodes
::class "RepetitionNode" subclass MatchNode
::method init
  expose repNode min max
  use strict arg repNode, min, max
  self~init:super
  repNode~terminate   -- we need to ensure the chain ends in a terminator

-- common initial piece for all repetition nodes.  The greedy/possessive/reluctant
-- parts are implemented in the recursiveMatch method that each subclass
-- implements
::method match
  use strict arg context, position, target
  current = position
  repNode = self~repNode

  -- if we have a minimum specified, we must get at least that, else we fail
  do i = 1 to self~min
      if \repNode~match(context, current, target) then do
          return .false
      end
      current = context~matchEnd
  end

  return self~recursiveMatch(context, current, target, repNode, self~min, self~max)


::attribute repNode GET
::attribute min GET
::attribute max GET


::method calculateMatchMetrics
  expose repNode min max
  use strict arg metrics
  -- get our repetition node values as if it was the only thing of interest
  submetrics = self~calculateChildMatchMetrics(repNode)

  metrics~addMin(min * submetrics~minLength)
  metrics~addMax(max * submetrics~maxLength)

  -- if the max and min are the same, then this is potentially deterministic
  if max == min then do
      metrics~deterministic = metrics~deterministic & submetrics~deterministic
  end
  else do
      -- no go on the length predictions
      metrics~deterministic = .false
  end

  -- send this along
  self~calculateMatchMetrics:super(metrics)



-- a Greedy repetition of a pattern.  This will match as
-- much as possible, but will back off in an attempt to match
-- any of the following pieces
::class "GreedyRepetitionNode" subclass RepetitionNode
-- implement greedy repetition matching logic
::method recursiveMatch
  use strict arg context, position, target, repNode, matches, max

  -- we hit the max count, now try to match the trailing bit
  if matches >= max then do
      return self~matchNext(context, position, target)
  end

  -- preserve matches, since that determines how much we can
  -- back up
  counter = matches

  -- loop until we don't get a match on our search node
  loop while repNode~match(context, position, target)
      matchLength = context~matchEnd - position
      -- if this is a zero length match, there's no point
      -- in continuing
      if matchLength == 0 then do
          leave
      end
      -- count this occurrence and move up
      counter += 1
      position = context~matchEnd
      -- now we need to consume as many as possible, up to
      -- the maximum
      loop while counter < max
          -- test again
          if \repNode~match(context, position, target) then do
              -- we've eaten our fill, now see what happens
              -- after this
              leave
          end
          -- we matched, but if this match was a different
          -- length from our working length, we need to recurse
          -- to handle backing up
          if position + matchLength \= context~matchEnd then do
              if self~recursiveMatch(context, context~matchEnd, counter + 1, max) then do
                  return .true
              end
              -- go handle backing up from here.  The backup position is
              -- actually the previous match
              leave
          end
          position = context~matchEnd
          counter += 1
      end
      -- we can back up for the number of matches we've had
      -- at this recursion level
      loop while counter >= matches
          -- if we hit a spot where our successor can match, we're
          -- done
          if self~matchNext(context, position, target) then do
              return .true
          end
          -- step back the fixed length and decrement our counter
          position -= matchLength
          counter -= 1
      end
      return .false   -- backed off as far as we can, but can't fit in the rest
  end

  -- we've matched as much as we can, now check the rest
  return self~matchNext(context, position, target)


-- a node that implements reluctant repetition matches
::class "ReluctantRepetitionNode" subclass RepetitionNode
-- the initial part to match the minimum is performed
-- in the base class.  The reluctant part of the algorithm is
-- done here
::method recursiveMatch
  use strict arg context, position, target, repNode, matches, max

  loop forever
      -- check the successor without consuming any more of the
      -- string.  If it matches, we're done
      if self~matchNext(context, position, target) then do
          return .true
      end
      -- if we've hit the limit and there's no successor match,
      -- this fails
      if matches >= max then do
          return .false
      end
      -- (reluctantly), we try to eat one leetle mint...
      if \repNode~match(context, position, target) then do
          -- we've eaten our fill, this is a failure
          return .false
      end
      -- if we hit a zero-length match, there's no moving forward
      -- from here.  This is also a failure
      if position == context~matchEnd then do
          return .false
      end
      -- step forward, and try again
      position = context~matchEnd
      matches += 1
  end



-- possessive repetition nodes...this sucks up all it can and will
-- never give anything back
::class "PossessiveRepetitionNode" subclass RepetitionNode
-- the initial part to match the minimum is performed
-- in the base class.  The reluctant part of the algorithm is
-- done here
::method recursiveMatch
  use strict arg context, position, target, repNode, matches, max
  do i = self~min + 1 to self~max
      -- match failure means we've got all we can get
      if \repNode~match(context, position, target) then do
          leave
      end
      -- a zero-length match also terminates matching
      if position == context~matchEnd then do
          leave
      end
      -- step over this
      position = context~matchEnd
  end
  -- and try the next part
  return self~matchNext(context, position, target)



-- Special logic for handling group repetitions
::class "GroupRepetitionNode" subclass MatchNode  inherit GroupNodeMixin
::method init
  expose repNode min max
  use strict arg id, repNode, min, max

  self~init:super
  -- and also the mixin
  self~setGroupId(id) -- the mixing manages the id portion
  repNode~terminate   -- we need to ensure the chain ends in a terminator


-- common initial piece for all repetition nodes.  The greedy/possessive/reluctant
-- parts are implemented in the recursiveMatch method that each subclass
-- implements
::method match
  expose id
  use strict arg context, position, target

  repNode = self~repNode
  info = self~getGroupReference(context)
  -- save the original state in case we need to back up
  saved = info~saveMatch

  -- if we have a minimum specified, we must get at least that, else we fail
  do i = 1 to self~min
      if \repNode~match(context, position, target) then do
          -- restore the group state, we failed
          info~restoreMatch(saved)
          info~participated = .false  -- We don't participate here either
          context~exitGroup  -- pop the group context
          return .false
      end
      -- update the capture info to the current bit
      info~setMatch(context, position, context~matchEnd)
      position = context~matchEnd
  end
  -- The subclasses handle the match rules after the minimum
  return self~recursiveMatch(context, info, position, target, repNode, self~min, self~max)


::attribute repNode GET
::attribute min GET
::attribute max GET


::method calculateMatchMetrics
  expose repNode min max
  use strict arg metrics
  -- get our repetition node values as if it was the only thing of interest
  submetrics = self~calculateChildMatchMetrics(repNode)

  metrics~addMin(min * submetrics~minLength)
  metrics~addMax(max * submetrics~maxLength)

  -- if the max and min are the same, then this is potentially deterministic
  -- if the children are deterministic.
  if max == min then do
      metrics~deterministic = metrics~deterministic & submetrics~deterministic
  end
  else do
      -- no go on the length predictions
      metrics~deterministic = .false
  end

  -- send this along
  self~calculateMatchMetrics:super(metrics)



-- a Greedy repetition of a pattern.  This will match as
-- much as possible, but will back off in an attempt to match
-- any of the following pieces
::class "GreedyGroupRepetitionNode" subclass GroupRepetitionNode
-- implement greedy repetition matching logic
::method recursiveMatch
  use strict arg context, info, position, target, repNode, matches, max
  -- save the original state in case we need to back up
  saved = info~saveMatch

  -- preserve matches, since that determines how much we can
  -- back up
  counter = matches

  do label match
      -- if reached the limit already, then quit
      if counter >= max then do
          leave match
      end
      -- if the first match fails, then quit immediately too
      if \repNode~match(context, position, target) then do
          leave match
      end
      matchLength = context~matchEnd - position
      -- There are forms of groups that back up, so the length
      -- can be negative.
      if matchLength <= 0 then do
          -- the match positions are reversed.
          info~setMatch(context, context~matchEnd, position)
          -- and this is our new position point
          position = context~matchEnd
          leave match -- no more matching here
      end
      loop forever
          info~setMatch(context, position, position + matchLength)
          -- step the match position to the end of the last
          -- successful match
          position = context~matchEnd

          -- increment the counter now, and check for the max
          counter += 1
          if counter >= max then do
              leave
          end
          -- no match, time to quit
          if \repNode~match(context, position, target) then do
              leave
          end
          -- if the length of the match has changed, then recurse
          -- for the backups
          if position + matchLength \= context~matchEnd then do
              if self~recursiveMatch(context, info, context~matchEnd, counter + 1, max) then do
                  return .true
              end
              -- go handle backing up from here.  The backup position is
              -- actually the previous match
              leave
          end
      end
      -- we can back up for the number of matches we've had
      -- at this recursion level
      loop while counter >= matches
          -- if we hit a spot where our successor can match, we're
          -- done
          if self~wrappedMatchNext(context, position, target) then do
              -- if our match count was zero, then we don't participate
              if counter == 0 then do
                  info~participated = .false
              end
              context~exitGroup
              return .true
          end
          -- step back the fixed length and decrement our counter
          position -= matchLength
          -- update the group match
          info~setMatch(context, position, position + matchLength)
          counter -= 1
      end
  end
  info~restoreMatch(saved)    -- restore the match position
  -- if our match count was zero, then we don't participate
  if counter == 0 then do
      info~participated = .false
  end
  -- we've matched as much as we can, now check the rest
  return self~matchNext(context, position, target)



-- a node that implements reluctant repetition matches
::class "ReluctantGroupRepetitionNode" subclass GroupRepetitionNode
-- the initial part to match the minimum is performed
-- in the base class.  The reluctant part of the algorithm is
-- done here
::method recursiveMatch
  use strict arg context, info, position, target, repNode, matches, max

  loop forever
      -- check the successor without consuming any more of the
      -- string.  If it matches, we're done
      if self~wrappedMatchNext(context, position, target) then do
          -- if our match count was zero, then we don't participate
          if matches == 0 then do
              info~participated = .false
          end
          context~exitGroup
          return .true
      end
      -- if we've hit the limit and there's no successor match,
      -- this fails
      if matches >= max then do
          context~exitGroup
          return .false
      end
      -- (reluctanly), we try to eat one leetle mint...
      if \repNode~match(context, position, target) then do
          -- we've eaten our fill, this is a failure
          context~exitGroup
          return .false
      end
      -- if we hit a zero-length match, there's no moving forward
      -- from here.  This is also a failure
      if position == context~matchEnd then do
          context~exitGroup
          return .false
      end
      -- update the group information
      info~setMatch(context, position, context~matchEnd)
      -- step forward, and try again
      position = context~matchEnd
      matches += 1
  end



-- possessive repetition nodes...this sucks up all it can and will
-- never give anything back
::class "PossessiveGroupRepetitionNode" subclass GroupRepetitionNode
-- the initial part to match the minimum is performed
-- in the base class.  The reluctant part of the algorithm is
-- done here
::method recursiveMatch
  use strict arg context, info, position, target, repNode, matches, max
  do i = matches + 1 to self~max
      -- match failure means we've got all we can get
      if \repNode~match(context, position, target) then do
          leave
      end
      -- update the group information
      info~setMatch(context, position, context~matchEnd)
      -- a zero-length match also terminates matching
      if position == context~matchEnd then do
          leave
      end
      -- step over this
      position = context~matchEnd
  end
  -- if our match count was zero, then we don't participate
  if i == 0 then do
      info~participated = .false
  end
  -- and try the next part
  return self~matchNext(context, position, target)



-- base class for all nodes that match back references
-- a back reference match
::class "BackReferenceNode" subclass MatchNode
-- main matching method for a back reference.
::method match
  use strict arg context, position, target

  -- get the match text defined for this type of back reference
  -- if nothing is there, then this match fails
  matchValue = self~matchText(context)
  if matchValue == .nil then do
      return .false
  end

  if context~checkEnd(position + matchValue~length - 1) then do
      return .false
  end

  if \self~submatch(context, position, target, matchValue) then do
      return .false
  end

  -- we match the back value, now continue matching from the end of
  -- this point.
  return self~matchNext(context, position + matchValue~length, target)

::method calculateMatchMetrics
  use strict arg metrics

  -- the maximum can no longer be trusted
  metrics~validMaximum = .false
  self~calculateMatchMetrics:super(metrics)



-- matches the current position for the same string value as
-- a back reference match
::class "GroupBackReferenceNode" subclass BackReferenceNode
::method init
  expose ref
  use strict arg ref
  self~init:super


-- resolve the matching text for this back reference.
-- returns .nil if this cannot be located
::method matchText
  expose ref
  use strict arg context
  -- retrieve the match result for our target back ref
  refGroup = context~getBackReferenceGroup(ref)

  -- if the back reference did not match, then this is a
  -- failure here too
  if \refGroup~matched then do
      return .nil
  end
  -- return the group's match text
  return refGroup~matchText


-- simple method for performing the implementation specific
-- matching for a back reference.  Intended to be overridden
::method submatch
  use strict arg context, position, target, matchValue
  return target~match(position, matchValue)



-- same as a back reference, but matching is done caselessly
::class "CaselessGroupBackReferenceNode" subclass GroupBackReferenceNode
-- simple method for performing the implementation specific
-- matching for a back reference.  Intended to be overridden
::method submatch
  use strict arg context, position, target, matchValue
  return target~caselessMatch(position, matchValue)



-- a back reference node to handle more complex group resolution
::class "ResultBackReferenceNode" subclass BackReferenceNode
::method init
  expose resultPath
  -- the result path
  use strict arg resultPath
  self~init:super


-- run the resolution path to locate the target result or group.
-- returns .nil if there is a failure at any point in the matching
-- process
::method matchText
  expose resultPath
  use strict arg context

  -- the first two matches here are a little different, since
  -- there will a guarantee of 2 names and the first one comes
  -- from the current matching context.  After that, the
  -- match results are used for retrieval

  -- retrieve the match result for our target back ref
  refGroup = context~resolveBackReferencePath(resultPath)

  -- we're after the result text now
  return refGroup~matchText


-- simple method for performing the implementation specific
-- matching for a back reference.  Intended to be overridden
::method submatch
  use strict arg context, position, target, matchValue
  return target~match(position, matchValue)



-- same as a result back reference, but matching is done caselessly
::class "CaselessResultBackReferenceNode" subclass ResultBackReferenceNode
-- simple method for performing the implementation specific
-- matching for a back reference.  Intended to be overridden
::method submatch
  use strict arg context, position, target, matchValue
  return target~caselessMatch(position, matchValue)



-- a normal group node with implicit numbering
::class "GroupNode" subclass MatchNode
::method init
  expose id groupMatch
  use strict arg id, groupMatch
  self~init:super


::method match
  expose id groupMatch
  use strict arg context, position, target

  info = context~getGroupReference(id)

  -- if the pattern inside the group matches, then record
  -- the match position inside group matches, record this
  -- in the corresponding group information and continue
  if groupMatch~match(context, position, target) then do
      info~setMatch(context, position, context~matchEnd)
      return self~matchNext(context, context~matchEnd, target)
  end
  else do
      -- mark the group as non-matching
      info~clearMatch
      return .false
  end



-- A back reference node used in conditionals to test if a
-- group node had participated in a match operation
::class "BackReferenceTestNode" subclass MatchNode
::method init
  expose ref
  use strict arg ref
  self~init:super


-- perform the back reference test.  A match here
-- is just an indicator that the group had participated
-- in a prior match.  This does not forward to a successor
-- node
::method match
  expose ref
  use strict arg context, position, target
  -- retrieve the match result for our target back ref
  refGroup = context~getBackReferenceGroup(ref)
  -- and just return the matched indictor
  return refGroup~participated


::method terminate
  -- optimization override.  This node does not require
  -- a terminator, so turn this into a NOP when called



-- A back reference node used in conditionals to test if a
-- group result node had participated in a match operation
::class "ResultBackReferenceTestNode" subclass MatchNode
::method init
  expose resultPath
  -- the result path
  use strict arg resultPath
  self~init:super


-- run the resolution path to locate the target result or group.
-- returns .false if there is a failure at any point in the matching
-- process
::method match
  expose resultPath
  use strict arg context, position, target
  -- the first two matches here are a little different, since
  -- there will a guarantee of 2 names and the first one comes
  -- from the current matching context.  After that, the
  -- match results are used for retrieval

  -- retrieve the match result for our target back ref
  refGroup = context~getBackReferenceGroup(resultPath[1])
  result = refGroup~result(resultPath[2])
  -- this might not be here, so return .nil for any failure
  if result == .nil then do
      return .false
  end

  count = resultPath~items
  -- resolution here is done in pairs, though there could be an odd
  -- number
  loop i = 3 by 2 while i < count
      group = result~group(resultPath[i])
      if group == .nil then do
          return .false
      end
      -- if this is the last item, the group value is the
      -- desired text
      if i == count then do
          return group~participated
      end

      -- step to the next result
      result = group~result(resultPath[i + 1])
      -- and fail it it is not there
      if result == .nil then do
          return .false
      end
  end
  -- we're after the result match now
  return result~matched


::method terminate
  -- optimization override.  This node does not require
  -- a terminator, so turn this into a NOP when called



-- support for a (?(cond)then) node
::class "IfThenNode" subclass MatchNode
::method init
  expose condition thenNode
  use strict arg condition, thenNode
  self~init:super
  thenNode~terminate   -- make sure the then node is terminated
  condition~terminate  -- and the condition also


::method match
  expose condition thenNode
  use strict arg context, position, target

  -- if the condition succeeds, then attempt the then portion
  if condition~match(context, position, target) then do
      -- the condition is true, so attempt the conditional then part
      -- using the original match position.  The conditional never
      -- changes the position
      if \thenNode~match(context, position, target) then do
          return .false
      end
      -- this succeed, so update the match position before
      -- forwarding
      position = context~matchEnd
  end
  -- either the condition was false or the then matched, so
  -- forward along
  return self~matchNext(context, position, target)


-- calculate match metrics for a conditional node
::method calculateMatchMetrics
  expose thenNode
  use strict arg metrics

  -- calculate metrics for the then branch
  submetrics = self~calculateChildMatchMetrics(thenNode)
  -- since we don't necessary perform the then match,
  -- our minimum successful match needs to be zero
  submetrics~minLength = 0
  -- we are not deterministic
  submetrics~deterministic = .false
  -- and merge into the full set
  metrics~addMetrics(submetrics)
  -- send this along
  self~calculateMatchMetrics:super(metrics)



-- support for a (?(cond)then|) node
::class "IfThenElseNode" subclass MatchNode
::method init
  expose condition thenNode elseNode
  use strict arg condition, thenNode, elseNode
  self~init:super
  thenNode~terminate   -- make sure both nodes are terminated
  elseNode~terminate
  condition~terminate  -- and the condition also


::method match
  expose condition thenNode elseNode
  use strict arg context, position, target

  -- if the condition succeeds, then attempt the then portion
  if condition~match(context, position, target) then do
      -- the condition is true, so attempt the conditional then part
      -- using the original match position.  The conditional never
      -- changes the position
      if \thenNode~match(context, position, target) then do
          return .false
      end
      -- this succeeded, so update the match position before
      -- forwarding
      position = context~matchEnd
  end
  -- if the condition match is false, then take the else branch
  else do
      -- the condition is true, so attempt the conditional then part
      -- using the original match position.  The conditional never
      -- changes the position
      if \elseNode~match(context, position, target) then do
          return .false
      end
      -- this succeeded, so update the match position before
      -- forwarding
      position = context~matchEnd
  end
  -- we've had a match on either the then or else branch, so
  -- forward along
  return self~matchNext(context, position, target)


-- calculate match metrics for a conditional node
::method calculateMatchMetrics
  expose thenNode
  use strict arg metrics

  -- calculate metrics for the then branch
  submetrics = self~calculateChildMatchMetrics(thenNode)
  elseMetrics = self~calculateChildMatchMetrics(elseNode)
  -- get a merged set from the two branches
  submetrics~mergeMetrics(elseMetrics)
  submetrics~minLength = 0
  -- we are not deterministic
  submetrics~deterministic = .false
  -- and merge into the full set
  metrics~addMetrics(submetrics)
  -- send this along
  self~calculateMatchMetrics:super(metrics)



-- a group node with an explicit name
::class "NamedGroupNode" subclass MatchNode
::method init
  expose id groupMatch
  use strict arg id, groupMatch
  self~init:super


::method match
  expose id groupMatch
  use strict arg context, position, target

  -- get the group information from the context so we can
  -- store match information back into it.
  info = context~namedGroupInfo(id)

  -- now have the group try to match.
  if groupMatch~match(context, position, target) then do
      -- ok, we have a match now store that match information back into
      -- the context information, and continue on from the end of the
      -- matching point.
      info~setMatch(context, position, context~matchEnd)
      return self~matchNext(context, context~matchEnd, target)
  end
  else do
      -- make sure the match information is cleared.
      info~clearMatch
      return .false
  end



-- base class for logical operations of class patterns
::class "ClassLogicalNode" subclass MatchNode
::method init
  expose leftSide rightSide
  use strict arg leftSide, rightSide

  self~init:super
  -- ensure these side branches have terminator elements
  leftSide~terminate
  rightSide~terminate


::method calculateMatchMetrics
  expose leftSide rightSide
  use strict arg metrics

  leftMetrics = self~calculateChildMatchMetrics(leftSide)
  rightMetrics = self~calculateChildMatchMetrics(rightSide)
  -- merge this set of metrics into a combined result
  leftMetrics~mergeMetrics(rightMetrics)
  -- and add to the full metric set
  metrics~addMetrics(leftMetrics)
  self~calculateMatchMetrics:super(metrics)


::attribute leftSide
::attribute rightSide



-- class logical OR node
::class "ClassOrNode" subclass ClassLogicalNode
::method match
  use strict arg context, position, target

  -- NOTE:  We do not check if we're at the end before doing
  -- these tests.  One of these patterns might be an end anchor,
  -- which would return .true.  We need to let each of the possibilities
  -- decide whether past the end is bad or not.

  -- this is an OR with logical shortcutting, so if the left expression
  -- is true, we don't check the right side, but unconditionally take the
  -- left path.
  if self~leftSide~match(context, position, target) then do
     return self~matchNext(context, context~matchEnd, target)
  end
  else if self~rightSide~match(context, position, target) then do
     return self~matchNext(context, context~matchEnd, target)
  end

  return .false



-- class logical AND
::class "ClassAndNode" subclass ClassLogicalNode
::method match
  use strict arg context, position, target


  -- NOTE:  We do not check if we're at the end before doing
  -- these tests.  One of these patterns might be an end anchor,
  -- which would return .true.  We need to let each of the possibilities
  -- decide whether past the end is bad or not.

  -- we need both the left and right sides of the expression to
  -- be true before passing on the expression
  if self~leftSide~match(context, position, target) then do

     -- TODO:  What position should this be continuing from?  This sort of
     -- assumes from the last match position from the second test.  Is that right?
     if self~rightSide~match(context, position, target) then do
         return self~matchNext(context, context~matchEnd, target)
     end
  end

  return .false



-- base wrapper class for group operations.  This holds all of
-- the pieces involved with a group and manages the wrappering
-- and management of the group
::class "GroupEnvelope" subclass MatchNode
::method init
  expose terminator
  -- The terminator handles end of group processing.
  terminator = .GroupTerminator~new(self)
  self~init:super


::method match
  use strict arg context, position, target

  -- our successor is our real node, but if it
  -- matches, it will go through the GroupTerminator
  -- node which will poke us to update the group state

  -- however, since this is a non-capturing group, we don't
  -- actually update anything
  return self~matchNext(context, position, target)


::attribute terminator GET


-- method for the group terminator to make a callback to indicate
-- a successful match.  This is ignored for non-capturing nodes
::method setGroupEnd
  -- nop in the base class


-- The terminator updates the match information before
-- calling its successor node in case there are back references.
-- If the successor does not match, it might need to rollback
-- the match information to a previous value.  This will update the
-- information, but also return the previous information for saving
::method updateAndSaveMatch
  -- nop in the base class, but we need a return value
  return .true


-- this is the reverse of the previous.  Restores the
-- start and end information using a previous saved state
::method restorePreviousMatch
-- nop in the base class


-- base class for capturing group interactions
::class "CapturingGroupEnvelope" subclass GroupEnvelope
::method init
  expose id
  -- capturing groups all have an id.  This is either an
  -- already allocated numeric or a string name
  use strict arg id
  self~init:super


::method match
  use strict arg context, position, target
  -- our successor is our real node, but if it
  -- matches, it will go through the GroupTerminator
  -- node which will poke us to update the group state

  --save the current postion so we can retrieve it when
  -- poked by the terminator
  context~setLocal(self, position)
  -- perform the match
  ret =  self~matchNext(context, position, target)
  -- delete the local reference
  context~removeLocal(self)
  return ret


-- return the group reference information for a capturing group
::method getGroupReference
  expose id
  use strict arg context

  return context~getGroupReference(id)

-- method for the group terminator to make a callback to indicate
-- a successful match.  This uses the saved local information for
-- the position
::method setGroupEnd
  use strict arg context, target, end
  -- get the group information
  info = getGroupReference(context)
  -- and update with the current match information
  info~setMatch(context, context~getLocal(self), end)


-- The terminator updates the match information before
-- calling its successor node in case there are back references.
-- If the successor does not match, it might need to rollback
-- the match information to a previous value.  This will update the
-- information, but also return the previous information for saving
::method updateAndSaveMatch
  use strict arg context, target, end
  -- get the group information
  info = self~getGroupReference(context)
  save = .directory~new
  save~start = info~start
  save~end = info~end
  -- and update with the current match information
  info~setMatch(context, context~getLocal(self), end)
  return save


-- this is the reverse of the previous.  Restores the
-- start and end information using a previous saved state
::method restorePreviousMatch
  use strict arg context, save

  -- get the group information
  info = self~getGroupReference(context)
  info~start = save['START']
  info~end = save~end



-- special terminator for the end of a grouping
::class "GroupTerminator" subclass TerminatorNode
::method init
  expose group
  use strict arg group
  self~init:super


::method match
  expose group
  use strict arg context, position, target

  -- this will update the group information and return the
  -- old information to us in case we need to revert
  save = group~updateAndSaveMatch(context, target, position)
  if self~matchNext(context, position, target) then do
      -- good match, everything is ok
      return .true
  end
  -- roll the info change back to the prior value
  -- we did not get a complete and proper match
  group~restorePreviousMatch(context, save)
  return .false



-- wrapper around a group to give atomic nature.
::class "AtomicGroupNode" subclass MatchNode
::method init
  expose group
  use strict arg group
  self~init:super
  -- The first match that comes out of the group will forward to
  -- the terminator, which will tell it everything is good.  At
  -- that point, we unconditionally send this down the chain
  group~terminate


::method match
  expose group
  use strict arg context, position, target

  -- do the group match and accounting, then forward along.
  if group~match(context, position, target) then do
      return self~matchNext(context, context~matchEnd, target)
  end

  return .false



-- wrapper around a group to perform a positive lookahead.
-- Positive lookahead means this is true if the group
-- matches at the current position.
-- This only checks for a match, but does not consume
-- any text.  If this matches, the same position
-- information is passed along to the successor node
::class "PositiveLookaheadNode" subclass MatchNode
::method init
  expose group
  use strict arg group
  self~init:super


::method match
  expose group
  use strict arg context, position, target

  -- do the group match and accounting, then forward along.
  -- NOTE:  both of these match the same position
  if group~match(context, position, target) then do
      return self~matchNext(context, position, target)
  end

  return .false



-- wrapper around a group to perform a negative lookahead.
-- Negative lookahead means this is true if the group
-- does NOT match at the current position.
-- This only checks for a match, but does not consume
-- any text.  If this matches, the same position
-- information is passed along to the successor node
::class "NegativeLookaheadNode" subclass MatchNode
::method init
  expose group
  use strict arg group
  self~init:super

::method match
  expose group
  use strict arg context, position, target

  -- do the group match and accounting, then forward along.
  -- NOTE:  both of these match the same position
  if \group~match(context, position, target) then do
      return self~matchNext(context, position, target)
  end

  return .false



-- wrapper around a group to perform a positive lookbehind.
-- Positive lookbehind means this is true if the group
-- matches at prior position.
-- This only checks for a match, but does not consume
-- any text.  If this matches, the same position
-- information is passed along to the successor node
::class "PositiveLookBehindNode" subclass MatchNode
::method init
  expose group min max
  use strict arg group, metrics
  self~init:super

  min = metrics~minLength
  -- if there is a deterministic maximum, then
  -- use it to optimize the search.  Otherwise, set the
  -- max to zero, which will force things to check everywhere.
  if metrics~validMaximum then do
      max = metrics~maxLength
  end
  else do
      max = 0
  end


::attribute group


::method match
  expose group min max
  use strict arg context, position, target

  -- this is how much we back up for the first test
  backPosition = position - min
  -- if not enough room to test this, we fail
  if \context~checkStart(backPosition) then do
      return .false
  end
  -- no set maximum?
  if max == 0 | max > position - context~startPosition then do
      -- test all possible positions
      startRange = context~startPosition
  end
  else do
      -- we can cap this using the maximum
      startRange = position - max
  end
  -- create a subcontext that restricts the possible match range
  subContext = context~createSubContext(startRange, position)
  -- mark this as requiring a complete match from the position to
  -- the end.  This will ensure this really is butted up against
  -- the prior position
  subContext~matchEndRequired = .true

  return self~backMatch(context, position, target, subContext, backPosition)


-- backward matching method designed to be overridden by a subclass.
-- this version performs the positive match
::method backMatch
  use strict arg context, position, target, subContext, backPosition

  -- we need to test the shortest matches first
  do i = backPosition by -1 while subContext~checkStart(i)
      -- if we have a back match, then time to forward to the next bit
      if self~group~match(subContext, i, target) then do
          -- got a match, forward on using the original context
          return self~matchNext(context, position, target)
      end
  end

  -- no match
  return .false



-- wrapper around a group to perform a negative lookbehind.
-- Negative lookbehind means this is true if the group
-- does NOT match at prior position.
-- This only checks for a match, but does not consume
-- any text.  If this matches, the same position
-- information is passed along to the successor node
::class "NegativeLookBehindNode" subclass PositiveLookBehindNode
-- backward matching method designed to be overridden by a subclass.
-- this version performs the positive match
::method backMatch
  use strict arg context, position, target, subContext, backPosition

  -- we need to test the shortest matches first
  do i = backPosition by -1 while subContext~checkStart(i)
      -- if we have a back match, then this is a failure
      if self~group~match(subContext, i, target) then do
          return .false
      end
  end

  -- no match, then this passes
  return self~matchNext(context, position, target)



-- A matcher for beginning of text that works in
-- multiline mode.  To match, this either must be
-- the beginning of the line or the first character after
-- a \r\n sequence
::class "InternetMultilineCaretNode" subclass MatchNode
::method match
  use strict arg context, position, target

  -- the end is always game over, even if preceeded by a newline
  if context~checkEnd(position) then do
      return .false
  end

  -- if at the real beginning, this is good
  if context~atStart(position) then do
      return self~matchNext(context, position, target)
  end

  -- Not enough space to test for a linend?  This also fails ,
  -- we need to see if we're positioned at a lineend
  if \context~checkStart(position - 2) then do
      return .false
  end

  -- must have a CRLF sequence before this to match
  if \target~match(position - 2, .RegexCompiler~CRLF) then do
      return .false
  end
  -- and go finish up
  return self~matchNext(context, position, target)



-- A matcher for beginning of text that works in
-- multiline mode.  To match, this either must be
-- the beginning of the line or the first character after
-- a \n sequence
::class "UnixMultilineCaretNode" subclass MatchNode
::method match
  use strict arg context, position, target

  -- the end is always game over, even if preceeded by a newline
  if context~checkEnd(position) then do
      return .false
  end

  -- if at the real beginning, this is good
  if context~atStart(position) then do
      return self~matchNext(context, position, target)
  end

  -- must have a CRLF sequence before this to match
  if \target~match(position - 1, .String~nl) then do
      return .false
  end
  -- and go finish up
  return self~matchNext(context, position, target)



-- A terminal node for a full match chain.  This does any
-- end-of-match processing.
::class "TerminatorNode" subclass MatchNode
::method match
  use strict arg context, position, target

  -- set the end match position
  context~matchEnd = position
  -- if we require a match with the end, then only return true
  -- if our position is at the end
  if context~matchEndRequired then do
      return position == context~endPosition
  end

  -- this is always a successful match.  This really just
  -- records the last position of this branch segment.
  return .true


-- add a .TerminatorNode terminator to a node that will
-- be used as part of a composite.  This not process
-- the call, since a terminator node cannot have a
-- next element
::method terminate



-- Search nodes are not normally part of the pattern tree.  These
-- are only instantiated when a find() operation is being done with
-- a pattern
::class "SearchNode" subclass MatchNode
::method init
  expose minLength
  use strict arg pattern
  self~init:super

  self~next = pattern  -- this is our search pattern

  -- to optimize, we scan the tree to figure out what the minium match
  -- length will be.  This will prevent making unnecessary probes on the
  -- target once matches are no longer possible.
  metrics = self~calculateChildMatchMetrics(pattern)

  minLength = metrics~minLength


::method match
  expose minLength
  use strict arg context, position, target
  -- NB, + 1 is needed because the start position is not
  -- zero based.  We might have exactly enough for a single test
  probes = (context~endPosition - position) - minLength + 1

  if probes <= 0 then do
      return .false
  end

  do i = position for probes
      if self~matchNext(context, i, target) then do
          -- update the match information
          context~setMatchPosition(i, context~matchEnd);
          return .true
      end
  end

  -- no matches possible
  return .false


::method calculateMatchMetrics
  use strict arg metrics

  -- get the successor metrics, but we need to tweak them
  self~calculateMatchMetrics:super(metrics)
  -- because we slide, these are never valid
  metrics~deterministic = .false
  metrics~validMaximum = .false



-- A node that holds and executes an indirect match to another
-- pattern. usually for library pattern references.
::class "PatternNode" subclass MatchNode
::method init
  expose groupName patternName pattern
  use strict arg groupName, patternName, pattern


::method match
  expose groupName pattern
  use strict arg context, position, target

  -- create a new context to execute this
  subcontext = context~createNestedPatternContext(pattern)
  -- attempt a match from the current position
  matchResult = subcontext~match(pattern, position)
  -- record the result of this with the current group
  context~setGroupResult(groupName, matchResult, subcontext~groups)
  -- if this matched, then foward along
  if matchResult~matched then do
      return self~matchNext(context, matchResult~end, target)
  end
  -- if it did not match, this is a failure
  return .false


::method calculateMatchMetrics
  expose pattern
  use strict arg metrics
  -- add in the metrics from the pattern
  metrics~addMetrics(pattern~matchMetrics)
  -- get the successor metrics, but we need to tweak them
  self~calculateMatchMetrics:super(metrics)



-- A small class that contains the specifics of the information from the
-- match context.
::class "MatchText"
::method init
  expose regionText regionStart regionEnd
  use strict arg regionText, regionStart, regionEnd


-- accessor methods
::attribute regionText
::attribute regionStart
::attribute regionEnd


-- get the size of the matching text region
::attribute textLength GET
  expose regionStart regionEnd

  return regionEnd - regionStart -- end position is one past the end


 -- extract a text subpiece
::method text
  expose regionText regionStart regionEnd
  use strict arg start = (regionStart), length = (self~textLength)

  -- cap the length at the size of the region
  length = min(length, self~textLength)
  return regionText~substr(start, length)


 -- extract a prefix section
::method prefix
  expose regionText regionStart
  use strict arg start

  return regionText~substr(regionStart, start - regionStart)


 -- extract a suffix section
::method suffix
  expose regionText regionEnd
  use strict arg end

  return regionText~substr(end, regionEnd - end)



-- Information about an individual match.  The match
-- may have embedded groups.  The match information for
-- the embedded groups can also be retrieved
::class "MatchResult"
::method init
  expose matchText start end groups
  use strict arg matchText, start, end, groups

-- belt and braces...if the start is 0, make sure the end is zero also
  if start == 0 then do
      end = 0
  end


-- return a response if the operation matched.
::method matched
  expose start

  return start > 0


-- individual match results
::attribute start GET
::attribute end GET


-- retrieve the length of the matching region
::attribute textLength GET
  expose matchText

  return return matchText~regionLength  -- get the information from the descriptor


-- retrieve the length of the match string
::attribute length GET
  expose end start

  return end - start -- end position is one past the last match character


-- retrieves the match text
::attribute text GET
  expose matchText start

  -- No match position?  Nothing to return.
  if start == 0 then do
      return ''
  end

  -- extract the text subpiece
  return matchText~text(start, self~length)


-- retrieve the prefix string before the match, up to the start
-- of the match region
::attribute prefix GET
  expose matchText start

  -- no match is always a null string
  if start == 0 then do
      return ""
  end

  return matchText~prefix(start)


-- retrieve any text following the match position, up to the end
-- of the match region
::attribute suffix GET
  expose matchText start end

  -- no match is always the entire region string
  if start == 0 then do
      return matchText~text
  end

  -- just everything from the end to the end of text region
  return matchText~suffix(end)


-- return the position of character after the match position.  If
-- past the end of the match region, returns regionEnd + 1
::method nextMatch
  expose end

  -- NOTE:  This is the correct result even if the match string
  -- is zero-length
  return end + 1


-- resolve a group path name
::method resolveGroupInfo PRIVATE
  expose groups
  use strict arg path

  -- we might not have any groups hung off of us
  if groups == .nil then do
      return .nil
  end

  -- simple path, just return the match info
  if \path~contains('/') then do
      return groups[path~upper]
  end

  -- we have a complex path to handle...parse out the first piece,
  -- resolve our group, then pass this along to resolve the rest of
  -- the pieces.
  parse upper var path groupName '/' path

  -- get this level...if we can't handle this, this can't be resolved
  group = groups[groupName]
  if group == .nil then do
      return .nil
  end

  -- pass this along to the next level
  return group~resolveGroupInfo(path)


-- retrieve a group.  This is either a numeric id
-- or a symbolic name
::method group
  expose groups
  use strict arg path

  -- resolve the group path
  group = self~resolveGroupInfo(path)

  -- give a reasonable default rather than raising an error
  if .nil == group then do
      return .nil
  end

  -- this relies on the base match information for a lot of the string specifics,
  -- so wrapper this.
  return group~matchResult(self)


-- retrieve the text for a given group item in a match result
::method "[]"
  expose groups
  -- default is to retrieve the entire group match
  use strict arg id = "0"

  -- make sure this is uppercase
  id = id~upper

  group = groups[id]
  -- unknown group, return the .nil default
  if group == .nil then do
      return .nil
  end

  -- return the match text (which will be null if not matched)
  text = group~matchText
  if text == .nil then do
      return ""
  end
  return text



-- the class for a group result.  There's really not much difference
-- between this and a top-level match result...this is really just a tagging
-- difference at this point.
::class "GroupMatchResult" subclass MatchResult



-- the current active context for a match operation.  This
-- provides information to the matching filters and holds any
-- intermediate state required by the filters.  This also
-- implements the matching operations.
::class "MatchContext"
::method init
  expose text startPosition endPosition matchPosition matchEnd matchText
  use strict arg text, startPosition = 1, length = (text~length)

  -- make sure the length is bounded.  The end position is one
  -- past the end
  endPosition = min(text~length + 1, startPosition + length)

  -- create an object we can hand out for inclusion in match results
  matchText = .MatchText~new(text, startPosition, endPosition)
  matchPosition = 0
  matchEnd = 0


-- accessor for the matchText information
::attribute matchText GET
-- we also need access to the groups for constructing embedded references
::attribute groups GET


-- prepare for a match using a given pattern instance
::method prepareMatch private
  expose root groups locals groupStack rootGroup
  use strict arg pattern

  -- get the pattern root and a copy of the groups information
  root = pattern~root
  groups = pattern~groups~copy

  -- do a deep copy of the groups directory
  -- to create a working groups list
  do name over groups
      groups[name] = groups[name]~copy
  end

  locals = .table~new   -- this allows patterns to save/retrieve local state
  groupStack = .queue~new -- create a stack for pushing and popping group usage

  -- push the main group at the head of the stack (if it exists)
  rootGroup = groups[0]
  groupStack~push(rootGroup)


-- create a subcontext of the current context with a different start
-- and stop range.  Used by lookbehind matchers to restrict the
-- search range of the filters
::method createSubContext
  use strict arg start, end

  -- just clone the context and update the bounds
  copy = self~copy
  copy~setRange(start, end)

  return copy


-- create a new context for invoking a nested pattern
::method createNestedPatternContext
  expose text startPosition endPosition
  use strict arg pattern

  -- create a context as if this was toplevel
  return self~class~new(text, startPosition, endPosition - startPosition)


-- Retrieve the group reference item for a given group
::method getGroupReference
  expose groups
  use strict arg id

  -- return the information inherited from the pattern
  return groups[id]


-- record the entering of a group
::method enterGroup
  expose groupStack
  use strict arg group

  groupStack~push(group)


-- perform exit clean up for group completion
::method exitGroup
  expose groupStack

  return groupStack~pull   -- just pop this group instance off of the stack


-- Poke the group at the top of the execution stack with the result of
-- an embedded Pattern reference operation.
::method setGroupResult
  use strict arg name, result, groups

  group = self~getGroupReference(name)
  group~setPatternMatch(self, result, groups)


-- save a local value
::method setLocal
  expose locals
  use strict arg index, value
  locals[index] = value


-- retrieve a local value
::method getLocal
  expose locals
  use strict arg index
  return locals[index]


-- remove a given local value
::method removeLocal
  expose locals
  use strict arg index
  locals~remove(index)


-- test if the pattern is an exact match for the entire region, from
-- beginning to end
::method matches
  expose text startPosition matchPosition matchEndRequired root
  use strict arg pattern

  -- set up the context for matching
  self~prepareMatch(pattern)
  matchEndRequired = .true   -- this must match exactly
  -- this is the default result, and possibly checked by filters
  -- that need to examine the starting match position
  matchPosition = startPosition
  if root~match(self, startPosition, text) then do
      return .true  -- good match, give the indicator
  end
  else do
      matchPosition = 0   -- this needs to be cleared
      return .false       -- no match
  end


-- test if the pattern is an exact match from the start of the match
-- region up to the end of the pattern.  This does not need to match
-- to the end of the region
::method startsWith
  expose text startPosition matchPosition matchEndRequired root
  use strict arg pattern, start = (startPosition)

  -- set up the context for matching
  self~prepareMatch(pattern)

  matchEndRequired = .false  -- This does not need to match exactly
  -- adjust the starting position to the bounds
  start = max(startPosition, start)
  -- this is the default result, and possibly checked by filters
  -- that need to examine the starting match position
  matchPosition = start
  if root~match(self, startPosition, text) then do
      return .true  -- good match, give the indicator
  end
  else do
      matchPosition = 0   -- this needs to be cleared
      return .false       -- no match
  end


-- search the region for the first position with a pattern match.  Returns
-- a match result object with the match result
::method find
  expose text startPosition root
  -- the start is optional, but defalt to the beginning of the
  -- region (typical)
  use strict arg pattern, start = (startPosition), end = (test~length)

  -- set up the context for matching
  self~prepareMatch(pattern)
  -- forward to the common matching routine

  -- this node implements a sliding search search of the pattern.
  -- Not really part of the pattern tree, but it wrappers it
  return self~performMatch(start, .SearchNode~new(root))


-- match the given (or starting position) for a match.  Returns
-- a match result object with the match result
::method match
  expose startPosition root
  -- the start is optional, but defalt to the beginning of the
  -- region (typical)
  use strict arg pattern, start = (startPosition)

  -- set up the context for matching
  self~prepareMatch(pattern)
  -- forward to the common matching routine
  return self~performMatch(start, root)


-- match the given (or starting position) for a match.  Returns
-- a match result object with the match result
::method performMatch private
  expose text startPosition endPosition matchPosition matchEndRequired root
  -- the start is optional, but defalt to the beginning of the
  -- region (typical)
  use strict arg start, pattern

  -- adjust the starting position to the bounds
  start = max(startPosition, start)
  -- set the initial match position
  matchPosition = start

  if start > endPosition then do
      matchPosition = 0
      return self~result     -- give a match result object with the info
  end

  matchEndRequired = .false  -- This does not need to match exactly
  if \pattern~match(self, start, text) then do
      matchPosition = 0   -- this needs to be cleared
  end
  return self~result  -- give a match result object with the info


-- return a result object from the match result
::method result
  expose matchText matchPosition matchEnd groups rootGroup

  rootGroup~setMatch(self, matchPosition, matchEnd)
  return .MatchResult~new(matchText, matchPosition, matchEnd, groups)


-- check if we're at the start position
::method atStart
  expose startPosition
  use strict arg test

  return test == startPosition


-- test is a position is equal to or greater than the
-- start position.  Used for checks where the matcher needs
-- to make tests prior to the current position
::method checkStart
  expose startPosition
  use strict arg test

  return test >= startPosition


-- tests for whether a position is past the end of the range
::method checkEnd
  expose endPosition
  use strict arg test

  if test >= endPosition then do
      -- remember that we've hit the end of the pattern
      return .true
  end
  return .false


-- the range positions
::attribute startPosition
::attribute endPosition


-- any set match positions
::attribute matchPosition
::attribute matchEnd


-- change the bounds of the search to a restricted range.
::method setRange private
  expose matchText text startPosition endPosition
  use strict arg startPosition, endPosition

  -- we need an updated matchText with the new range positions
  -- recorded
  matchText = .MatchText~new(text, startPosition, endPosition)


-- indicates whether the pattern must exactly match the end
-- of the string (e.g., we're doing a match vs. a locate)
::attribute matchEndRequired

-- set the match position for a positive match.  The sets both the
-- beginning and end positions.
::method setMatchPosition
  expose matchPosition matchEnd
  use strict arg matchPosition, matchEnd

-- retrieve a backreference value
::method getBackReferenceGroup
  expose groups
  use strict arg id

  if groups == .nil then do
      raise syntax 93.900 array("Back reference to unknown group" id);
  end

  groupRef = groups[id]
  if groupRef == .nil then do
      raise syntax 93.900 array("Back reference to unknown group" id);
  end

  return groupRef

-- resolve a group backreference path that may be more than one
-- level deep.  These will be references to elements defined in
-- embedded patterns
::method resolveBackReferencePath
  expose groups
  use strict arg path

  -- get the base group (which will raise an error on our behalf if it does not
  -- exist
  group = self~getBackReferenceGroup(path[1])

  loop i = 2 to path~items
     -- get an item from our current group level.
     group = group~group(path[i])
     if group == .nil then do
         -- report the element we can't find
         raise syntax 93.900 array("Back reference to unknown group" path[i]);
     end
  end
  -- return the final located group
  return group


-- return the offset of the given position.  This will be
-- the number of characters from the start of the match
-- range to the given position.
::method matchOffset
  expose startPosition
  use strict arg position

  return position - startPosition



-- the class for managing group information during a pattern match
::class "GroupReference"
::method init
  expose id start end context participated namedGroups
  use strict arg id

  -- these will be filled in during the course of matching
  start = 0
  end = 0
  context = .nil
  namedGroups = .nil   -- This is only used when an embedded pattern is used
  participated = .true -- participation is different from "matching"


-- the start and end match positions
::attribute start
::attribute end


-- indicates whether a group participated in a match
::attribute participated
-- the context we're attached to.  This gets set when the context
-- first initiates a pattern search and copies the group information.
::attribute context


-- the list of results from named patterns used inside this
-- group.  Returns .nil if none were used (common)
::attribute namedGroups


-- add a named subgroup to this group
::method addGroup
  expose namedGroups
  use strict arg name, group

  -- we create this lazily, since it's more common
  -- that a group not have any of these
  if namedGroups == .nil then do
      namedGroups = .directory~new
  end

  -- add this to the directory table
  namedGroups[name~upper] = group


-- retrieve a named group result from the group information
::method group
  expose namedGroups
  use strict arg name

  if namedGroups == .nil then do
      return .nil
  end

  return namedGroups[name~upper]


-- indicates whether this match was a success or a failure
::method matched
  expose start

  return start > 0


-- set the match values for an embedded pattern.  When created, this
-- item is just a placeholder for the group that contains the pattern reference.
-- Once the match is made, we can fill in the group information that was
-- pulled from the pattern and used by the context for matching
::method setPatternMatch
  expose context start end namedGroups
  use strict arg context, result, namedGroups

  -- set the the match positions from the match result
  start = result~start
  end = result~end


-- set the match values for an embedded pattern.  When created, this
-- item is just a placeholder for the group that contains the pattern reference.
-- Once the match is made, we can fill in the group information that was
-- pulled from the pattern and used by the context for matching
::method setMatch
  expose context start end
  use strict arg context, start, end


-- clear the match values
::method clearMatch
  expose start end

  -- it is useful to keep the context, since that contains the text we use.
  start = 0
  end = 0


-- return the text that was matched.  Returns .nil if
-- this match was not successful
::method matchText
  expose context start end

  -- we only have text if we've got a matching position
  if start > 0 then do
      return context~matchText~text(start, end - start)
  end
  -- no matching text
  return .nil


-- get a copy of the match positions
::method saveMatch
  expose start end

  r = .directory~new
  r~start = start
  r~end = end
  return r


-- restore the match location from a saved copy
::method restoreMatch
  expose start end
  use strict arg r

  start = r['START']
  end = r~end


-- return a match result for this group.
::method matchResult
  expose context start end namedGroups

  -- if we don't have a set context, then create a dummy matchText
  if context == .nil then do
      matchText = .MatchText~new(" ", 1, 1)
  end
  else do
      matchText = context~matchText
  end

  -- this gets wrappered into a result item
  return .GroupMatchResult~new(matchText, start, end, namedGroups)



-- a general purpose parsing context that includes support for
-- regular expressions
::class "Parser" public
::method init
  expose string matchstart matchend sectionstart sectionend compiler matchResult
  use strict arg string, compiler = (.RegexCompiler~new)

  matchstart = 1;    -- There is an implicit absolute trigger of 1 at the start
  matchend = 1;      -- All numeric movement triggers have a zero-length size
  sectionstart = 1;  -- The section also starts here
  sectionend = string~length -- and is the length of the string
  matchResult = .nil -- no match result at the start


-- return the current match position for the parsing context
::attribute matchStart GET


-- The end position of the match position.  Note that this
-- will be located at matchStart + matchLength.  The sectionStart
-- and matchEnd will be the same normally.
::attribute matchEnd GET


-- The length of the pattern match
::attribute matchLength GET
  expose matchStart matchEnd
  return matchEnd - matchStart


-- the text for the last section.  All numeric triggers
-- have zero length.  You can think of them as matching
-- in the space between matchStart and the previous character.
::attribute matchText GET
  expose string matchStart matchEnd
  return string~substr(matchStart, matchEnd - matchStart)


-- this is the current section of the parsed text that is
-- between the current match position and the previous match
-- end.  For example, if you were working with the parse template
-- parse value '1234567890' with '3' x '9'
-- sectionstart would be 4, sectionend would be 8 after the
-- trigger '9' was applied
::attribute sectionStart GET


-- end of the equivalent section (see above).  Note that this will
-- also be sectionStart + sectionLength.  If the section is a null string,
-- then start and end will be the same
::attribute sectionEnd GET


-- length of the current section
::attribute sectionLength GET
  expose sectionStart sectionEnd
  return sectionEnd - sectionStart


-- the section text
::method section
expose string sectionstart sectionEnd
return string~substr(sectionstart, sectionEnd - sectionStart)


-- return the current section parsed into blank delimited words
::method sectionwords
  return self~section~wordList
  count = section~words
  wordlist = .array~new(count)

  do i = 1 to count
     wordlist[i] = section~word(i)
  end

  return wordlist


-- return the remainder of the string after the last
-- match position.  Note that if backward movement is
-- used, the remainder will overlap with the last parsed
-- section
::method remainder
  expose string matchend
  return string~substr(matchend)


-- locate a needle in the current string, returning an
-- indicator of whether the string was located.
-- details of the match can be retrieved from the context.
::method find
  expose string matchstart matchend sectionstart sectionEnd matchResult
  use strict arg needle

  temp = string~pos(needle, matchend)
  -- NOTE:  this differs from the PARSE instruction.  If the
  -- string is not found, the match positions will be unchanged.
  -- This allows one to attempt a match, detect if it failed, and
  -- maybe try other alternatives without needing to save and
  -- restore the match position
  if temp == 0 then do
      return .false
  end
  else do
      matchResult = .nil  -- clear the old match result
      matchStart = temp
      sectionstart = matchend
      matchend = matchstart + needle~length
      sectionEnd = matchstart
      return .true
  end


-- locate a needle in the current string, returning an
-- indicator of whether the string was located.
-- details of the match can be retrieved from the context.
::method caselessFind
  expose string matchstart matchend sectionstart sectionEnd matchEnd matchResult
  use strict arg needle

  temp = string~caselessPos(needle, matchend)
  -- NOTE:  this differs from the PARSE instruction.  If the
  -- string is not found, the match positions will be unchanged.
  -- This allows one to attempt a match, detect if it failed, and
  -- maybe try other alternatives without needing to save and
  -- restore the match position
  if temp == 0 then do
      return .false
  end
  else do
      matchResult = .nil  -- clear the old match result
      matchStart = temp
      sectionstart = matchend
      matchend = matchstart + needle~length
      sectionEnd = matchstart
      return .true
  end


-- locate a needle in the current string, returning an
-- indicator of whether the string was located.
-- details of the match can be retrieved from the context.
-- the needle may be a string, which will be converted into
-- a regex pattern using the current compiler or a directly
-- provided pattern
::method findRegex
  expose string matchstart matchend sectionstart sectionEnd compiler matchResult
  use strict arg needle

  -- the regular expression can be specified as a string or a
  -- pattern instance.  If this is a string, then compile it before
  -- using
  if needle~isa(.string) then do
      needle = compiler~compile(needle)
  end

  -- now do a regex search on the string
  result = needle~find(string, matchend, string~length)
  -- NOTE:  this differs from the PARSE instruction.  If the
  -- string is not found, the match positions will be unchanged.
  -- This allows one to attempt a match, detect if it failed, and
  -- maybe try other alternatives without needing to save and
  -- restore the match position
  if \result~matched then do
      return .false
  end
  else do
      matchResult = result  -- save the result for retrieval, if needed
      matchStart = result~start
      sectionstart = matchend
      matchend = matchstart + result~length
      sectionEnd = matchstart
      return .true
  end


  -- return a .MatchResult object that represents the last match
  -- operation.  This will either be a MatchResult returned from a
  -- regex find operation or one constructed from the current parser
  -- state that represents the prior operations
  ::attribute matchResult GET
  expose methodResult string matchStart matchEnd

  -- if the matchResult is not set, then construct one
  if methodResult == .nil then do
      methodResult = .MatchResult~new(string, 1, string~length, matchStart, matchEnd, .false, .nil)
  end

  return methodResult


-- relative postional movement of the pointer
::method move
  expose string matchstart matchend sectionstart sectionlength
  use strict arg delta

  -- the section begins at the start of the movement
  sectionstart = matchstart
  matchstart = matchstart + delta

  self~validateposition


-- absolute positional movement
::method moveTo
  expose string matchstart matchend sectionstart sectionlength

  sectionStart = matchStart
  use strict arg matchStart

  self~validateposition


-- Validate and adjust the positional values after a non-search
-- parse movement
::method validateposition private
  expose string matchstart matchend sectionstart sectionlength matchResult

  matchResult = .nil  -- clear the old match result

  -- keep the start within the bounds of the string
  if matchstart < 1 then do
      matchstart = 1
  end
  else if matchstart > string~length then do
      matchstart = string~length + 1
  end

  -- if this moved backwards from the previous, then the
  -- section goes to the end of the string
  if matchstart <= sectionstart then do
      sectionEnd = string~length + 1
  end
  else do
      -- for forward movement, the sectionEnd is the same
      -- as the matchStart position
      sectionEnd = matchstart
  end

  -- for postional movement, the match string is always
  -- zero-length, so these will be the same
  matchend = matchstart



-- simple class used to accumulate matching metrics
::class "MatchMetrics"
::method init
  expose minLength maxLength validMaximum deterministic

  minLength = 0
  maxLength = 0
  validMaximum = .true  -- indicates the max accumulator is good
  deterministic = .true  -- indicates whether calculations are even possible


-- accessors for the various metric values
::attribute minLength
::attribute maxLength
::attribute validMaximum
::attribute deterministic


-- bump both the min and max by the same value
::method addLength
  expose minLength maxLength
  use strict arg length

  minLength += length
  maxLength += length


-- increment the pattern minimum
::method addMin
  expose minLength
  use strict arg length

  minLength += length


-- increment the pattern maximum
::method addMax
  expose maxLength
  use strict arg length

  maxLength += length


-- add a node's metric set to our accumulated set
::method addMetrics
  expose deterministic validMaximum
  use strict arg other

  -- add in the minumum and maximum
  self~addMin(other~minLength)
  self~addMax(other~maxLength)

  -- or in the various flags
  deterministic = deterministic & other~deterministic
  validMaximum = validMaximum & other~validMaximum


-- like addMetrics, but this version will just calculate
-- the various max and min values between two sets of
-- metrics
::method mergeMetrics
  expose minLength maxLength deterministic validMaximum
  use strict arg other

  -- calculate the new minimum and maximum
  minLength = min(minLength, other~minLength)
  maxLength = max(maxLength, other~maxLength)

  -- or in the various flags
  deterministic = deterministic & other~deterministic
  validMaximum = validMaximum & other~validMaximum



-- a class containing a library of Regex Patterns.  this includes
-- class families and named patterns that can be referenced in
-- other patterns.  Patterns in this library can contain other
-- patterns in the library, but they must not be circular.  Generally, patterns
-- from the library will be compiled on first use.
::class "PatternLibrary" PUBLIC
::method init
  expose families patterns compiler libraries

  -- we can be given an optional source to load (as a property file) and an optional
  -- compiler instance to compile the patterns.
  use strict arg source = .nil, compiler = (.RegexCompiler~new)

  -- were we given a property file/stream to load from?  Go do this.
  if source \= .nil then do
      self~load(source)
  end

  -- our mappings for the library elements
  families = .directory~new
  patterns = .directory~new

  -- this is the set of pattern libraries required to compile patterns in
  -- this library.  Patterns in the attached libraries will be returned
  -- if they are not found in this.
  libraries = .array~new


-- load a set of patterns and/or class families from a properties file.  Also
-- allows for compile options to be specified, which will be used for compiling
-- any patterns loaded from the file.
::method load
  use strict arg source

  -- if not a property bundle already, try to load one from a file or stream
  if \source~isA(.directory) then do
     -- read the properties file
     source = .Properties~load(source)
  end
  self~loadBundle(source)


-- load a library from a directory or property bundle
::method loadBundle PRIVATE
  use strict arg

  -- we handle patterns last, once we've located all options and classes.
  loop name over bundle
     -- a named character family.  The part after the period is the
     -- family name, the value is the set of characters.
     if name~caselessMatch(1, 'FAMILY.') then do
        parse var name '.' familyName
        self~addCharacterClass(familyName, bundle[name])
     end
     -- like the character classes, the PATTERN. prefix identifies
     -- this as a pattern.  We just accumulate these as strings and
     -- process them after we've examined all of the properties.
     else if name~caselessMatch(1, 'PATTERN.') then do
        parse var name '.' patternName
        self~addPattern(patternName, filePatterns[name])
     end
     -- just ignore any unrecognized properties to allow other information
     -- to be stored in the same source.
  end


-- save the context of this library to a property bundle.  This does not
-- do anything with the attached libraries it depends upon
::method save
  expose patterns families
  use strict arg target

  -- first populate a propery bundle, we'll use its save method to
  -- persist the library
  bundle = .properties~new

  -- write the family information out as a property, prefixed with the
  -- string "FAMILY." on the name
  loop name over classes
     bundle["FAMILY."name] = families[name]
  end

  -- and also save each of the patterns
  loop pattern over patterns~allItems
     pattern~addToBundle(bundle)
  end

  -- and finally save this
  bundle~save(target)


-- add a library to the dependency list for this library.  The dependent
-- libraries will be used for compiling patterns in this library
::method addLibrary
  expose libraries compiler
  use strict arg library

  -- if not a pattern library, assume this is a file name or string and
  -- try to load the library.  We will make it's compiler our compiler.
  if \library~isA(.PatternLibrary) then do
      library = self~class~new(library, compiler)
  end

  -- we search these in the order added
  libraries~append(library)


-- add a named character family to this library
::method addFamily
  expose families
  use strict arg name, characters

  -- this is just a directory of character sets.  Note, we
  -- index this by the upper case name

  families[name~upper] = characters


-- add a named pattern to the library.  This is lazy-compiled
-- on first request.  It is difficult to compile at this point because
-- it could depend upon other named patterns that are not yet part of the
-- library .
::method addPattern
  expose patterns

  use strict arg name, pattern

  -- this gives access back to the compiler so it can compile
  -- when needed.  The library also has access back to this library
  patterns[name~upper] = .LibraryPattern~new(self, name, pattern)


-- compile a pattern in the current context, using the configures library
::method compile
  expose compiler
  use strict arg pattern

  -- compile this pattern, making sure we are added to the pattern search order
  return compiler~compile(pattern, ,self)


-- retrieve a character family from this library
::method getFamily
  expose families libraries
  use strict arg name

  -- we always use the uppercase name
  name = name~upper
  -- step 1:  Check this library directly
  family = families[name]

  if family \= .nil  then return family
  -- not found locally?  try each of our attached libraries
  loop library over libraries
     family = library~getFamily(name)
     if family \= .nil then do
         return family
     end
  end

  -- not found
  return .nil


-- retrieve a named pattern from this library
::method getPattern
  expose patterns libraries
  use strict arg name

  -- we always use the uppercase name
  name = name~upper
  -- step 1:  Check this library directly
  pattern = patterns[name]

  -- if we found something, retrieve the compiled pattern
  -- from it (which may force it to compile if this is the
  -- first request).
  if pattern \= .nil  then return pattern~pattern
  -- not found locally?  try each of our attached libraries
  loop library over libraries
     pattern = library~getPattern(name)
     if pattern \= .nil then do
         return pattern
     end
  end

  -- not found
  return .nil



-- internal class used to hold the library patterns
::class "LibraryPattern"
::method init
  expose library patternString pattern name
  use strict arg library, name, patternString

  pattern = .nil

  -- if handed an already compiled pattern, we can switch things around
  if patternString~isA(.Pattern) then do
      pattern = patternString
      patternString = pattern~string
  end


-- retrieve the compile pattern from the library node
::method pattern
  expose pattern library patternString

  -- already compiled?  We're done
  if pattern \= .nil then return pattern

  -- compile this pattern and return
  pattern = library~compile(patternString)
  return pattern


-- add a pattern instance to a property bundle on save
::method addToBundle
  expose name patternString
  use strict arg bundle

  bundle["CLASS."name] = patternString
