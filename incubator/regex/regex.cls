/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Copyright (c) 2007 Rexx Language Association. All rights reserved.         */
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
-- ::options trace i
::class Pattern public

::method compile class
  -- the compiler is configurable, but there is a default
  use strict arg pattern, compiler = (.RegexCompiler~new)
  return compiler~compile(pattern)

::method init
  expose pattern root groups
  use strict arg pattern, root, groups

::attribute pattern GET
::attribute groups GET

::method string
  expose pattern
  return pattern

-- create a parsing context for performing matches and searches
-- using this pattern.
::method context
  expose root groups
  use strict arg text, start = 1, end = (text~length)
  return .MatchContext~new(text, start, end, root, groups)

-- simple one time match operation on a regular expression and
-- a region within a string.  Returns .true if this is an exact
-- match for the entire region
::method matches
  use strict arg text, start = 1, end = (text~length)
  -- create a matching context for this
  context = self~context(text, start, end)
  -- and just perform a match on this
  return context~matches

-- simple one time match operation on a regular expression and
-- a region within a string.  Returns .true if the region starts
-- with the given pattern
::method startsWith
  use strict arg text, start = 1, end = (text~length)
  -- create a matching context for this
  context = self~context(text, start, end)
  -- and just perform a match on this
  return context~startsWith

-- search a region of a string for a regex value.  Only the
-- section between the start position and the given length
-- are searched.  The return value is a MatchResult object with the
-- full details of the match.
::method find
  use strict arg text, start = 1, end = (text~length)
  -- create a matching context for this
  context = self~context(text, start, end)
  -- and just perform a search and return the match position
  -- value
  return context~find

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

  -- append and tail piece, if there is one
  if indexPosition <= text~length then do
      list~append(text~substr(indexPosition))
  end
  return list


::class RegexCompiler public

::constant METACHARACTERS "([{\^-$|]})?*+."
::constant CLASSMETACHARACTERS "[^-]&"
::constant CLOSURECHARACTERS "*+?{"
::method init class
  expose classFamilies registeredClassFamilies

  classFamilies = .directory~new
  registeredClassFamilies = .directory~new

  classFamilies["Lower"] = "abcdefghijklmnopqrstuvwxyz"
  classFamilies["Upper"] = "ABCDEVGHIJKLMNOPQRSTUVWXYZ"
  classFamilies["ASCII"] = xrange('00'x, '7F'x)
  classFamilies["Alpha"] = classFamilies["Lower"] || classFamilies["Upper"]
  classFamilies["Digit"] = "0123456789"
  classFamilies["Alnum"] = classFamilies["Alpha"] || classFamilies["Digit"]
  classFamilies["Punct"] = "!""#$%&'()*+,-./:;<=>?@[\]^_`{|}~"
  classFamilies["Graph"] = classFamilies["Alnum"] || classFamilies["Punct"]
  classFamilies["Print"] = classFamilies["Graph"]
  classFamilies["Blank"] = " " || '09'x
  classFamilies["Cntrl"] = xrange("00"x, "1F"x) || "7F"x
  classFamilies["XDigit"] = "0123456789abcdefABCDEF"
  classFamilies["Space"] = ' ' || '09'x || '0a'x || '0d'x || '0c'x || '0b'x

  classFamilies["RexxSymbol"] = classFamilies["Alnum"] || ".!?_"
  classFamilies["RexxVariableStart"] = classFamilies["Alpha"] || "!?_"
  classFamilies["RexxOperator"] = "+-\/%*|&=<>" || 'aa'x
  classFamilies["RexxSpecial"] = ",;~()[]"

-- retrieve a name class family from the list of registered class families
-- subclasses are free to override this method to provide additional
-- class families
::method classFamily class
  expose classFamilies registeredClassFamilies
  use strict arg name

  family = classFamilies[name]

  if .nil == family then do
      return registeredClassFamilies[name]
  end

  return family

-- register additional class families with a compiler instance
::method registerClassFamily class
  expose classFamilies registeredClassFamilies
  use strict arg name, chars

  if classFamilies~hasIndex(name) then do
      raise syntax 98.900 array("Class family" name "already exists")
  end

  registeredClassFamilies[name] = chars

-- initialize a regex compiler instance
::method init
  use strict arg options = .nil
  -- set default options
  self~multiline = .false
  self~unixlines = .false
  self~caseless = .false
  self~dotall = .false
  self~validateOptions(arg(1))

-- the following CONSTANTs define the valid options; split into two parts for
-- readability and source line length
--  NB DO NOT change the order without modifying the code in validateOptions()
::constant validOptions1 "MULTILINE SINGLELINE INTERNETLINES UNIXLINES"
::constant validOptions2 "CASELESS RESPECTCASE DOTALL DOTRESTRICTED"

-- validate any options passed to the compiler when created
::method validateOptions private
  use arg options
  if options == .nil then do
      return
  end

  options = options~upper
  -- combine the two lists of options
  validOptions = self~validOptions1 self~validOptions2

  do while options \== ""
      parse var options option options

      select
          -- option = MULTILINE
          when validOptions~word(1)~abbrev(option, 1) then do
              self~multiline = .true
          end
          -- option = SINGLELINE
          when validOptions~word(2)~abbrev(option, 1) then do
              self~multiline = .false
          end
          -- option = INTERNETLINES
          when validOptions~word(3)~abbrev(option, 1) then do
              self~unixlines = .false
          end
          -- option = UNIXLINES
          when validOptions~word(4)~abbrev(option, 1) then do
              self~unixlines = .true
          end
          -- option = CASELESS
          when validOptions~word(5)~abbrev(option, 1) then do
              self~caseless = .true
          end
          -- option = RESPECTCASE
          when validOptions~word(6)~abbrev(option, 1) then do
              self~caseless = .false
          end
          -- option = DOTALL
          when validOptions~word(7)~abbrev(option, 4) then do
              self~dotall = .true
          end
          -- option = DOTRESTRICTED
          when validOptions~word(8)~abbrev(option, 4) then do
              self~dotall = .false
          end
          otherwise  do
              raise syntax 93.915 array(validOptions, option)
          end
      end
  end

-- compile a regex expression using the posix-like
-- regular expression syntax
::method compile
  expose pattern current length groups
  use strict arg pattern
  current = 1     -- always start at the beginning
  length = pattern~length
  groups = .nil   -- no group information until we hit the first capturing group

  -- this is the last node that will get control only if all other
  -- elements match cleanly.  The expression parser hooks this up to the
  -- end of the expression graph.
  lastNode = .TerminatorNode~new
  rootNode = self~parseExpression(lastNode)
  -- and return a usable pattern for this
  return .Pattern~new(pattern, rootNode, groups)

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
  expose current length

  current = max(current - 1, 1)

-- test if a character is a metacharacter that needs to be skipped
::method isMetaCharacter
  use arg ch
  return self~METACHARACTERS~pos(ch) > 0

-- test if a character is a metacharacter in the context of parsing
-- a class range
::method isClassMetaCharacter
  use arg ch
  return self~CLASSMETACHARACTERS~pos(ch) > 0

-- test if a character is a closure character
-- a class range
::method isClosureCharacter
  use arg ch
  return self~CLOSURECHARACTERS~pos(ch) > 0

-- extract a string from a pattern that is delimited
-- by a pair of markers
::method extractDelimited
  expose pattern current length
  use arg start, end
  -- In theory, we should be at the start delimiter now.  If
  -- not there, this is an error
  if \pattern~match(current, start) then do
      raise syntax 13.900 array("Delimiter character" start "expected for delimited string")
  end

  -- step over the delimiter
  startPos = current + start~length
  -- and look for the closing delimiter
  endPos = pattern~pos(end, startPos)
  if endPos == 0 then do
      raise syntax 6.900 array("Missing closing delimeter" end)
  end

  current = endPos + end~length  -- position past the read position
  -- and return the string between the markers
  return pattern~substr(startPos, endPos - startPos + 1)

-- extract a numeric value from the stream.  Terminates
-- on the first non-numeric character or the EOS.
::method parseNumber
  number = ""

  do forever
     ch = self~next
     if ch == .nil then do
         leave
     end
     if \ch~datatype("W") then do
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

  use arg terminator   -- this is the end node that gets plugged into the end of the expression.

  alternative = .nil   -- if we have alternatives in this expression, we group them all

  do forever
      -- parse a sequence and chain up with the terminator
      node = self~parseSequence(terminator)
      if alternative \== .nil then do
          alternative~addAlternative(node)
      end

      -- look ahead to see if this is again part of an alternation
      ch = self~peek

      if ch \== "|" then do
          -- not an alternative....see what we need to return
          if alternative == .nil then do
              return node
          end
          else do
              return alternative
          end
      end
      else do
          -- if we've only processed the first node, create an alternative and
          -- add the node.
          if alternative == .nil then do
              alternative = .AlternativeNode~new
              alternative~addAlternative(node)
          end
      end
  end

-- parse out a subexpression.  This also handles any of the modifiers
-- that might be associated with a single subexpression
::method parseSequence
  use arg terminator

  firstNode = .nil
  lastNode = .nil

  do forever
      ch = self~next

      select
          -- this is an end of string, done parsing
          when .nil == ch then do
              leave
          end
          -- either of these is a terminator for the sequence.
          -- if this is "|", the caller will accumulate the alternatives.
          -- if this is ")", then this is the close of a group
          when ch == "|" | ch == ")" then do
              self~previous     -- make sure to back up and allow the caller to handle
              leave;
          end
          -- a class of characters
          when ch == "[" then do
              node = self~parseClass
          end
          -- start of a group.  We'll process and then recurse
          when ch == '(' then do
              -- parse out the group expression, then chain the
              -- entire section into our graph.  All subsequent
              -- bits follow the group sequence.
              groupInto = self~parseGroup
              if groupnode \= .nil then do
                  if firstNode == .nil then do
                      firstNode = groupInto~firstNode
                  end
                  else do
                      tail~next = groupInfo~firstNode
                  end
                  tail = groupInto~lastNode
              end
              iterate     -- closure has already been handled
          end
          -- a start anchor
          when ch == "^" then do
              node = self~parseStartAnchor
          end
          -- end anchor variants
          when ch == "$" then do
              node = self~parseEndAnchor
          end
          -- match any character
          when ch == "." then do
              node = self~parseDot
          end
          -- various escape characters
          when ch == "\" then do
              node = self~parseEscapes
          end
          -- these are not expected here
          when ch == "?" | ch == "*" | ch == "+" then do
              raise syntax 13.900 array ("Unexpected modifier character '"||ch||"'" )
          end
          otherwise do
              -- an atom string that is taken as-is
              self~previous   -- backup so parseAtom will grab the first character
              node = self~parseAtom
          end
      end

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
  -- return the terminator node we were given
  if firstNode == .nil then do
      return terminator
  end
  else do
      -- put the terminator as the next element of the last node of
      -- the chain.
      lastNode~next = terminator
  end
  return firstNode


::method parseStartAnchor
  -- if operating in single line mode, this is just
  -- a text beginning
  if \self~multiline then do
      return .BeginTextNode~new
  end
  else if self~unixLines then do
      return .UnixMultilineCaretNode~new
  end
  else do
      return .MultilineCaretNode~new
  end


::method parseEndAnchor
  -- an end anchor, which has 4 possibilities
  if self~unixLines then do
      if self~multiline then do
          return .UnixMultiLineEndNode~new
      end
      else do
          return .UnixLineEndNode~new
      end
  end
  else do
      if self~multiline then do
          return .MultiLineEndNode~new
      end
      else do
          return .LineEndNode~new
      end
  end


  -- a match anything character.  The meaning of
  -- "match anything" differs depending on options
::method parseDot
  if self~dotAll then do
      return .AllDotNode~new
  end
  else if self~unixLines then do
      return .UnixDotNode~new
  end
  else do
      return .DotNode~new
  end

::method parseEscapes
  -- first check for escaped characters that are really class nodes
  node = self~parseEscapedCharacters
  if node == .nil then do
      -- there are more complex operations that are escaped
      node = self~parseEscapedOperations
      if node == .nil then do
          raise syntax 13.900 array("Missing character after \ escape character")
      end
  end

  return node


::method parseGroup
  groupInfo = .directory~new

  ch = self~next
  -- do we have a group qualifier.  These are all non-capturing groups
  if ch == "?" then do
      ch = self~next
      select
          when ch == "?" then do
              -- non capture group
              group = .Group~new
              -- parse the encapsulated group expression.  This gets
              -- slotted between the group element and the terminator
              group~next = self~parseExpression(group~terminator)

              -- to wire in to the caller, we need to provide a node that
              -- is plugged into the previous node, and a node that will be
              -- the new tail end of the chain.
              groupInfo~firstNode = group
              groupInfo~lastNode = group~terminator
          end
          when ch == ">" then do
              -- an atomic group.  Matching is managed differently
              group = .Group~new
              -- parse the encapsulated group expression.  This gets
              -- slotted between the group element and the terminator
              group~next = self~parseExpression(group~terminator)

              -- wrap this with an encapsulating atomic node.
              wrapper = .AtomicGroupNode~new(group)
              -- wrappered nodes fill both roles
              groupInfo~firstNode = wrapper
              groupInfo~lastNode = wrapper
          end
          when ch == "=" then do
              -- a positive look ahead
              group = .Group~new
              -- parse the encapsulated group expression.  This gets
              -- slotted between the group element and the terminator
              group~next = self~parseExpression(group~terminator)

              wrapper = .PositiveLookAhead~new(group)
              -- wrappered nodes fill both roles
              groupInfo~firstNode = wrapper
              groupInfo~lastNode = wrapper
          end
          when ch == "!" then do
              -- a negative look ahead
              group = .Group~new
              -- parse the encapsulated group expression.  This gets
              -- slotted between the group element and the terminator
              group~next = self~parseExpression(group~terminator)

              wrapper = .NegativeLookAhead~new(group)
              -- wrappered nodes fill both roles
              groupInfo~firstNode = wrapper
              groupInfo~lastNode = wrapper
          end
          when ch == "<" then do
              -- a look behind.  These are a pain.  We can only
              -- support this if the pattern has a deterministic
              -- max and min.  Before we can decide, we need to
              -- extract the group expression and then calculate
              -- the metrics for the entire expression tree.

              -- this is the "=" or "!" qualifier
              ch = self~next

              group = .Group~new
              -- parse the encapsulated group expression.  This gets
              -- slotted between the group element and the terminator
              group~next = self~parseExpression(group~terminator)

              -- now have the pattern see if it can calculate the length
              metrics = self~getPatternMetrics(group~next)

              if \info.maxValue then do
                  raise syntax 98.900 array("A look behind group does not have a deterministic maximum length")
              end
              -- look behind match
              if ch == "=" then do
                  wrapper = .PositiveLookBehind~new(group, metrics)
              end
              else if ch == "!" then do
                  wrapper = .NegativeLookBehind~new(group, metrics)
              end
              else do
                  raise syntax 93.915 array("=!", ch)
              end
              -- wrappered nodes fill both roles
              groupInfo~firstNode = wrapper
              groupInfo~lastNode = wrapper
          end
          when ch == "<" then do
              -- named capture group
              self~previous
              referenceName = self~extractDelimited("{", "}")

              -- a named capturing group
              group = .CapturingGroup~new(referenceName)
              -- parse the encapsulated group expression.  This gets
              -- slotted between the group element and the terminator
              group~next = self~parseExpression(group~terminator)

              -- to wire in to the caller, we need to provide a node that
              -- is plugged into the previous node, and a node that will be
              -- the new tail end of the chain.
              groupInfo~firstNode = group
              groupInfo~lastNode = group~terminator
          end
          otherwise do
              -- if .true, this is a closed modifier
              if self~parseFlag then do
                  return .nil
              end
              -- non capture group after the flag value
              group = .Group~new
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
      -- a negative look ahead
      group = .CapturingGroup~new(self~nextCapture)
      -- parse the encapsulated group expression.  This gets
      -- slotted between the group element and the terminator
      group~next = self~parseExpression(group~terminator)

      -- to wire in to the caller, we need to provide a node that
      -- is plugged into the previous node, and a node that will be
      -- the new tail end of the chain.
      groupInfo~firstNode = group
      groupInfo~lastNode = group~terminator
  end

  node = self~parseClosure(groupInfo~firstNode)
  if node \== groupInfo~firstNode then do
      -- this is wrappered in a closure, so terminate the group chain with
      -- a bit that updates the match location, and replace the first and
      -- last bits with the closure node
      groupInfo~lastNode~next = .TerminatorNode~new
      groupInfo~firstNode = node
      groupInfo~lastNode = node
  end
  -- return the new chain information.
  return groupInfo


::method getPatternMetrics
  use arg head

  metrics = .directory~new
  metrics~maxLength = 0
  metrics~minLength = 0
  metrics~fixedMax = .true

  head~getMatchMetrics(metrics)
  return metrics

-- parse options flags in the expressions
::method parseFlag

  ch = self~next
  setting = .true

  do 2
      select
          when ch == "d" then do
              self~unixlines = setting
              leave
          end
          when ch == "i" then do
              self~caseless = setting
              leave
          end
          when ch =="m" then do
              self~multiline = setting
              leave
          end
          when ch == "s" then do
              self~dotall = setting
          end
          when ch == "-" then do
              -- this is a mode negation
              setting = .false
              ch = self~next
          end
          otherwise do
              raise syntax 93.915 array("dims-", ch)
          end
      end
  end

  ch = self~next
  if ch \== ")" then do
      raise syntax 36.900 array('Missing closing parentheses for mode flag')
  end

-- various option flags
::attribute unixlines
::attribute caseless
::attribute multiline
::attribute dotall


::method parseGroupClosure
  use arg groupInfo

  -- we're parsing any closure items that may follow the group itself.
  -- If there are closure items to process, this will end up adjusting the
  -- firstNode/lastNode information in groupInfo

  -- this will be the piece that's consumed by a potential closure item.
  target = groupInfo~firstNode

  ch = self~next

  select
      when ch == "?" then do
          -- a question repetitor
          ch = self~next

          if ch == "?" then do
              node = .LazyQuestionNode~new(target)
          end
          else if ch == "+" then do
              node = .PossessiveQuestionNode~new(target)
          end
          else do
              self~previous
              node = .GreedyQuestionNode~new(target)
          end

          -- stick a terminator after the end of this.  The
          -- question node will handle the repetitions.
          groupInfo~lastNode~next = .TerminatorNode~new
          -- this is atomic, and handles both roles
          groupInfo~firstNode = node
          groupInfo~lastNode = node
          return
      end
      when ch == "*" then do
          -- zero or more occurrences
          ch = self~next


          if ch == "?" then do
              node = .LazyRepetitionNode~new(target, 0, 999999999)
          end
          else if ch == "+" then do
              node = .PossessiveRepetitionNode~new(target, 0, 999999999)
          end
          else do
              self~previous
              node = .GreedyRepetitionNode~new(target, 0, 999999999)
          end

          -- stick a terminator after the end of this.  The
          -- question node will handle the repetitions.
          groupInfo~lastNode~next = .TerminatorNode~new
          -- this is atomic, and handles both roles
          groupInfo~firstNode = node
          groupInfo~lastNode = node
          return
      end
      when ch == "+" then do
          -- one or more occurrences
          ch = self~next


          if ch == "?" then do
              node = .LazyRepetitionNode~new(target, 1, 999999999)
          end
          else if ch == "+" then do
              node = .PossessiveRepetitionNode~new(target, 1, 999999999)
          end
          else do
              self~previous
              node = .GreedyRepetitionNode~new(target, 1, 999999999)
          end

          -- stick a terminator after the end of this.  The
          -- question node will handle the repetitions.
          groupInfo~lastNode~next = .TerminatorNode~new
          -- this is atomic, and handles both roles
          groupInfo~firstNode = node
          groupInfo~lastNode = node
          return
      end
      when ch == "{" then do
          min = self~parseNumber
          max = min
          ch = self~next
          if ch == "," then do
              max = self~parseNumber
              ch = self~next
          end
          if ch \= "}" then do
              raise syntax 36.900 array("Missing closing '}' for repetition count")
          end

          if .nil == min || .nil == max then do
              raise syntax 93.900 array("Invalid repetition count")
          end

          if max < min then do
              raise syntax 93.900 array("Repetition maximum" max "is less than minimum" min)
          end
          ch = self~next

          if ch == "?" then do
              node = .LazyRepetitionNode~new(target, min, max)
          end
          else if ch == "+" then do
              node = .PossessiveRepetitionNode~new(target, min, max)
          end
          else do
              self~previous
              return .GreedyRepetitionNode~new(target, min, max)
          end
      end
      otherwise do
          -- no additional closure
          return
      end

      -- stick a terminator after the end of this.  The
      -- question node will handle the repetitions.
      groupInfo~lastNode~next = .TerminatorNode~new
      -- this is atomic, and handles both roles
      groupInfo~firstNode = node
      groupInfo~lastNode = node
      return
  end

-- process the closure for a node.  Because a sequence might have
-- modifiers, we might need to wrap the node sequence in
-- an enclosing node that handles the modifiers
::method parseClosure
  use arg target
  ch = self~next

  select
      -- A question modifier, which itself might have a
      -- modifier to control the operating mode
      when ch == "?" then do
          -- a question repetitor
          ch = self~peek
          -- ?? operates in lazy mode
          if ch == "?" then do
              self~next
              return .LazyQuestionNode~new(target)
          end
          -- possesive question mode
          else if ch == "+" then do
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
      when ch == "*" then do
          ch = self~peek

          if ch == "?" then do
              self~next
              return .LazyRepetitionNode~new(target, 0, 999999999)
          end
          else if ch == "+" then do
              self~next
              return .PossessiveRepetitionNode~new(target, 0, 999999999)
          end
          else do
              return .GreedyRepetitionNode~new(target, 0, 999999999)
          end
      end
      -- one or more occurrences.  Also with different modes of
      -- operation
      when ch == "+" then do
          ch = self~peek

          if ch == "?" then do
              self~next
              return .LazyRepetitionNode~new(target, 1, 999999999)
          end
          else if ch == "+" then do
              self~next
              return .PossessiveRepetitionNode~new(target, 1, 999999999)
          end
          else do
              return .GreedyRepetitionNode~new(target, 1, 999999999)
          end
      end
      -- {min[,max]} range
      when ch == "{" then do
          min = self~parseNumber
          max = min
          ch = self~next
          if ch == "," then do
              max = self~parseNumber
              ch = self~next
          end
          -- must be a closing '}' on this
          if ch \= "}" then do
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

          if ch == "?" then do
              self~next
              return .LazyRepetitionNode~new(target, min, max)
          end
          else if ch == "+" then do
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
  node = self~parseClassSection

  ch = self~next
  if ch \= "]" then do
      raise syntax 36.900 array("Missing closing ']' for character class")
  end

  return node

-- parse out the characters between a class section.
::method parseClassSection
  expose current length pattern

  previousNode = .nil
  negated = .false
  firstchar = .true

  do forever
      ch = self~next

      select
          when ch == .nil then do
              raise syntax 36.900 array("Missing closing ']' for character class")
          end
          when ch == '^' then do
              if firstChar then do
                  negated = .true
                  iterate    -- go around and check the next character
              end
              else do
                  -- back up for the characcter scan
                  self~previous
              end
          end
          when ch == ']' then do
              -- we've hit the end of this class spec, time to return.
              self~previous
              return previousNode
          end
          when ch == "[" then do
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
          when ch == "&" then do
              ch = self~next
              if ch == "&" then do     -- this is a logical "&&"
                  if prev == .nil then do
                      raise syntax 35.900 array("Missing left term for '&&' operator")
                  end

                  rightHand = .nil

                  do forever
                      ch = self~next
                      select
                          when ch == "[" then do
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
                          when ch == "]" | ch == "&" then do
                              -- either the complete end, or a section end.
                              self~previous
                              leave
                          end
                          otherwise do
                              self~previous
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

                  if rightHand == .nil then do
                      raise syntax 35.900 array("Missing right term for '&&' operator")
                  end
                  if previousNode == .nil then do
                      previousNode = rightHand
                  end
                  else do
                      previousNode = .ClassAndNode~new(previousNode, rightHand)
                  end
              end
              else do
                  -- unread both characters...the "&" is just a literal char
                  self~previous
                  self~previous
              end

          end
/* TODO:  This needs to be worked in.  It was moved from the parse class range method
          when if ch == "\\" then do
          ch = self~next
          if escaped == "p" || escaped == "P" then do
              -- if this is the beginning of the range, we'll return that node instead.
              -- otherwise, we'll process what we have and pick this up later
              if inRange | characters \== "" then do
                  -- push back both characters
                  self~previous
                  self~previous
                  leave
              end

              version = escaped == "P"
              -- strangely, this could be something like ^\P{ } which is a double negative
              if negated then do
                  version = \version
              end
              return self~parseNamedProperty(version)
          end
          else do
              ch = self~parseEscapedCharacters
              -- this is some sort of escaped op, probably
              if ch == .nil then do
                  if inRange | characters \= "" then do
                      -- push back both characters
                      self~previous
                      self~previous
                      leave
                  end
                  else do
                      -- we can process this as a node
                      return self~parseEscapedOperations
                  end
              end
          end
      end
 */
          otherwise do
              firstchar = .false
              self~previous
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
-- be a set of specifically specified characters (including escaped
-- characters, or a range in the form "x-z".  The range will be terminated
-- by any of the meta characters.  We may also encounter a named family
-- class, in which case we'll return just that piece.  Range negations have
-- already been processed by our caller
::method parseClassRange
  use arg negated    -- determines the type of node we return
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
      else do
          -- This is a potential range character, pull it from
          -- the queue now.  Need to handle escaped characters
          -- here too
          ch = self~singleChar
      end

      -- this could be followed by a '-' character
      if self~peek == '-' then do
          -- skip over that and get the next character
          self~next
          endRange = self~singleChar
          -- special unescaped character?  This is also an error
          if endRange == .nil | endRange == '[' | endRange == ']' | endRange == '-' then do
              raise syntax 93.900 array("missing end character in class range")
          end
          -- replace the character with the end range
          ch = xrange(ch, endRange)
      end
      -- add this to the accumulator
      characters = characters || ch
  end
  -- return the node based on the class modifier
  if negated then do
      return .NotClassNode~new(characters)
  end
  else do
      return .ClassNode~new(characters)
  end

-- parse a named property and turn it into an equivalent family class
::method parseNamedProperty
  use arg negated

  familyName = self~extractDelimited("{", "}")

  if familyName~match(1, "Is") then do
      familyName = familyName~substr(3)
  end

  familyChars = .Pattern~classFamily(familyName)
  if familyChars == .nil then do
      raise syntax 93.900 array("Unknown named class family '"familyName"'")
  end

  if negated then do
      return .NotClassNode~new(familyChars)
  end
  else do
      return .ClassNode~new(familyChars)
  end

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
  use arg string

  if self~caseless then do
      return .CaselessStringNode~new(string)
  end
  else do
      return .StringNode~new(string)
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
      when ch == 'a' then do
          return '07'x
      end
      when ch == 'c' then do
          control = self~read
          if .nil == control then do
              raise syntax 93.900 array("Character expected after \c escape")
          end
          return bitxor(control, '40'x)
      end
      when ch == 'e' then do
          return '1b'x  -- escape char
      end
      when ch == 'f' then do
          return '0c'x  -- form feed
      end
      when ch == 'n' then do
          return '0a'x  -- form feed
      end
      when ch == 'r' then do
          return '0d'x  -- form feed
      end
      when ch == 'x' then do
          -- hex value
          return x2c(self~readHex || self~readHex)
      end
      otherwise do
          -- if not one of the reserved characters at this point, we return the escaped char
          if '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ'~pos(ch) == 0 then do
              return ch
          end

          -- this is either an operation or invalid.  We leave that for another phase
          self~previous
          return .nil
      end
  end

-- special escaped operations
::method parseEscapedOperations private
  ch = self~next

  select
      when .nil == ch then do
          raise syntax 93.900 array("Missing character after escape")
      end
      when '123456789'~pos(ch) > 0 then do
          -- a back reference
          if \self~haveGroup(ch) then do
              raise syntax 93.900 array("Unrecognized group back reference" ch)
          end
          if self~caseless then do
              return .CaselessBackReferenceNode~new(ch)
          end
          else do
              return .BackReferenceNode~new(ch)
          end
      end
      when ch == 'k' then do
          -- named back reference
          referenceName = self~extractDelimited("{", "}")

          if \self~haveGroup(referenceName) then do
              raise syntax 93.900 array("Unrecognized group back reference" referenceName)
          end
          if self~caseless then do
              return .CaselessBackReferenceNode~new(referenceName)
          end
          else do
              return .BackReferenceNode~new(referenceName)
          end
      end
      when ch == 'A' then do
          return .BeginTextNode~new
      end
      when ch == 'B' then do
          return .NotWordBoundary~new
      end
      when ch == 'b' then do
          return .WordBoundary~new
      end
      when ch == '<' then do
          return .BeginWordBoundary~new
      end
      when ch == '>' then do
          return .EndWordBoundary~new
      end
      when ch == 'D' then do
          return .NotClassNode~new('0123456789')
      end
      when ch == 'd' then do
          return .ClassNode~new('0123456789')
      end
      when ch == 'G' then do
          return .LastMatch~new
      end
      when ch == 'Q' then do
          return self~parseLiteral
      end
      when ch == 'S' then do
          return .NotClassNode~new('20090a0b0c0d'x)
      end
      when ch == 's' then do
          return .ClassNode~new('20090a0b0c0d'x)
      end
      when ch == 'W' then do
          return .NotClassNode~new('abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_')
      end
      when ch == 'w' then do
          return .ClassNode~new('abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_')
      end
      when ch == 'Z' then do
          -- this is like $, but it always is an end of input match, rather than multiline
          if self~unixLines then do
              return .UnixLineEndNode~new
          end
          else do
              return .LineEndNode~new
          end
      end
      when ch == 'z' then do
          return .TextEndNode~new
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
          endPosition = self~current
              leave
          end
          if modifier == 'E' then do
              endPosition = self~current
              self~next
              leave
          end
      end
  end

  return self~stringNode(self~extract(current, endPosition - current))


-- read and validate a octal value in an escaped value, returning the
-- eqivalent character value
::method readOctal private
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
::method readHex private
  ch = self~next

  if .nil \= ch then do
      if '1234567890abcdefABCDEF'~pos(ch) > 0 then do
          raise syntax 93.900 array("Invalid hex digit '"ch"'")
      end
      return ch
  end
  -- we can directly raise a syntax error for reading hex
  raise syntax 93.900 array("Hex digit expected")

-- Start of nodes that implement the various matching algorithms

-- base class for all matching nodes
::class MatchNode
::constant wordChars 'abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_'
::method init
  expose next anchored

  next = .nil
  anchored = .false    -- most nodes are not anchored

::attribute next
::attribute anchored

-- calculate the length of this match, if possible.  The
-- default implementation assumes the length of this node is zero and
-- just passes the request down the chain
::method getMatchMetrics
  expose next
  use arg metrics
  if next \== .nil then do
      next~getMatchMetrics(metrics)
  end

::method getChildMatchMetrics
  use arg child
  -- get our repetition node values as if it was the only thing of interest
  submetrics = .directory~new
  submetrics~maxLength = 0
  submetrics~minLength = 0
  submetrics~fixedMax = .true
  submetrics~deterministic = .true

  child~getMatchMetrics(submetrics)
  return submetrics

-- add a .TerminatorNode terminator to a node that will
-- be used as part of a composite
::method terminate
  expose next
  if next == .nil then do
      -- this is a branch terminator, which merely records
      -- the match position and return true if it is reached.
      self~next = .BranchTerminatorNode~new
  end
  else do
      -- we have a child node, make sure that child has a
      -- terminator
      next~terminate
  end

-- A node that enforces anchoring at the beginning of the text
::class BeginTextNode subclass matchNode
::method init
  self~init:super
  -- these are anchored
  self~anchored = .true

::method match
  use arg context, position, target

  if context~atStart(position), self~next~match(context, position, target) then do
      context~matchPosition = position
      return .true
  end

  return .false

-- A node that enforces anchoring at the end of the text
::class EndTextNode subclass matchNode
::method match
  use arg context, position, target

  if context~checkEnd(position), self~next~match(context, position, target) then do
      return .true
  end
  return .false


-- A node that enforces a match at the beginning of a word boundary
::class WordBoundaryNode subclass MatchNode
::method match
  use arg context, position, target

  -- if at the start of the range, the character must be a word character
  if \context~atStart(position) then do
      if \target~matchChar(position - 1, self~wordChars) then do
          return self~next~match(context, position, target)
      end
  end

  -- At the very end (e.g., past the end really), the last character
  -- must be a word character
  if \context~checkEnd(position) then do
      if \target~matchChar(position - 1, self~wordChars) then do
          return self~next~match(context, position, target)
      end
  end

  return .false

-- the inverse of a wordboundary match.  Matches only if the current
-- position is NOT on a word boundary
::class NotWordBoundaryNode subclass MatchNode
::method match
  use arg context, position, target

  leftBoundary = .false
  rightBoundary = .false

  if \context~atStart(position) then do
      if \target~matchChar(position - 1, self~wordChars) then do
          leftBouundary = .true
      end
  end

  if \context~checkEnd(position) then do
      if \target~matchChar(position - 1, self~wordChars) then do
          rightBoundary = .true
      end
  end

  if (\leftBoundary & \rightBoundary), self~next~match(context, position, target) then do
      return .true
  end
  return .false


::class BeginWordBoundaryNode subclass MatchNode
::method match
  use arg context, position, target

  if \context~atStart(position) then do
      if \target~matchChar(position - 1, self~wordChars) then do
          return self~next~match(context, position, target)
      end
  end

  return .false


::class EndWordBoundaryNode subclass MatchNode
::method match
  use arg context, position, target

  if \context~checkEnd(position) then do
      if \target~matchChar(position - 1, self~wordChars) then do
          return self~next~match(context, position, target)
      end
  end

  return .false


-- retrieves the last match information.  Since a match context
-- only implements a single match, this is the same as the starting
-- position of the context.
::class LastMatchNode subclass MatchNode
::method match
  use arg context, position, target

  if position \= context~regionStart then do
      return .false
  end

  return self~next~match(context, position, target)

-- Generic "dot".  This matches everything but the end of
-- of data
::class AllDotNode subclass matchNode
::method match
  use arg context, position, target

  if context~checkEnd(position) then do
      return .false
  end

  -- always true at this node, so just return the successor result.
  return self~next~match(context, position + 1, target)

::method getMatchMetrics
  use arg metrics
  -- a dot is always a single character, so easy to manage
  metrics~maxLength = metrics~maxLength + 1
  metrics~minLength = metrics~minLength + 1

  self~getMatchMetrics:super(metrics)


-- like all dot node, but doesn't match lineends (CRLF sequences,
-- in this case)
::class DotNode subclass AllDotNode
::method match
  use arg context, position, target

  if context~checkEnd(position) then do
      return .false
  end

  -- if positioned on ANY linend char, this is a failure
  if target~matchChar(position, '0d0a'x) then do
      return .false
  end

  -- always true at this point, so just return the successor result.
  return self~next~match(context, position + 1, target)


-- like all dot node, but doesn't match lineends (Unix mode, so
-- this is just linefeeds)
::class UnixDotNode subclass AllDotNode
::method match
  use arg context, position, target

  if context~checkEnd(position) then do
      return .false
  end

  -- if positioned on a linefeed char, this is a failure
  if target~match(position, '0a'x) then do
      return .false
  end

  -- always true at this point, so just return the successor result.
  return self~next~match(context, position + 1, target)

-- match a string of characters
::class StringNode subclass matchNode
::method init
  expose matchString
  use arg matchString
  self~init:super

::method match
  expose matchString
  use arg context, position, target

  -- must have at least the same length as the string available
  if context~checkEnd(position + matchString~length - 1) then do
      return .false
  end
  -- if no match, fail
  if \target~match(position, matchString) then do
      return .false
  end
  -- continue checking after this position
  return self~next~match(context, position + matchString~length, target)

::method getMatchMetrics
  expose matchString
  use arg metrics
  -- always the length of this string
  metrics~maxLength = metrics~maxLength + matchString~length
  metrics~minLength = metrics~minLength + matchString~length

  self~getMatchMetrics:super(metrics)

-- a caseless match for a string
::class CaselessStringNode subclass StringNode
::method match
  expose matchString

  use arg context, position, target

  if context~checkEnd(position + matchString~length) then do
      return .false
  end

  if \target~caselessMatch(position, matchString) then do
      return .false
  end

  return self~next~match(context, position + 1, target)


-- match for a defined set of characters
::class ClassNode subclass MatchNode
::method init
  expose characters
  use arg characters
  self~init:super
-- the characters we check against
::attribute characters GET

::method match
  expose characters
  use arg context, position, target

  if context~checkEnd(position) then do
      return .false
  end
  -- must match at least one of these
  if \target~matchChar(position, characters) then do
      return .false
  end

  return self~next~match(context, position + 1, target)

::method getMatchMetrics
  use arg metrics
  -- also just a single character match
  metrics~maxLength = metrics~maxLength + 1
  metrics~minLength = metrics~minLength + 1

  self~getMatchMetrics:super(metrics)

-- an inversion of the class node
::class NotClassNode subclass ClassNode
::method match
  use arg context, position, target

  if context~checkEnd(position) then do
      return .false
  end

  if target~matchChar(position, self~characters) then do
      return .false
  end

  return self~next~match(context, position + 1, target)

-- the intersection of multiple class node definitions
::class ClassIntersectionNode subclass MatchNode

::method init
  expose subExpressions
  use arg subExpressions
  self~init:super

::method match
  expose subExpressions

  use arg context, position, target

  do test over subExpressions
      if \test~match(context, position, target) then do
          return .false
      end
  end

  return self~next~match(context, position + 1, target)

::method getMatchMetrics
  use arg metrics
  -- also just a single character match
  metrics~maxLength = metrics~maxLength + 1
  metrics~minLength = metrics~minLength + 1

  self~getMatchMetrics:super(metrics)


-- match on a series of OR expressions
::class AlternativeNode subclass MatchNode
::method init
  expose alternatives
  -- these are accumulated during parsing
  alternatives = .array~new

  self~init:super

::method match
  expose alternatives

  use arg context, position, target

  -- return on the first match
  do alternative over alternatives
      if alternative~match(context, position, target)  then do
          -- TODO:  Should this return now, or should it forward to
          -- the next stage?
          return .true
      end
  end

  return .false

::method getMatchMetrics
  expose alternatives

  maxLength = 0
  minLength = 999999999
  deterministic = .true
  maxValid = .true

  -- we need to analyze all of the alternative paths.

  do alternative over alternatives
      -- get our repetition node values as if it was the only thing of interest
      submetrics = self~getChildMatchMetrics(repNode)
      maxLength = max(maxLength, subMetrics~maxLength)
      minLength = min(minLength, subMetrics~minLength)
      deterministic = deterministic & submetrics~deterministic
      maxValid = maxValid & submetrics~deterministic
  end


  metrics~minLength = metrics~minLength + minLen
  metrics~maxLength = metrics~maxLength + maxlen

  metrics~deterministic = metrics~deterministic & deterministic
  metrics~maxValid = metrics~maxValid * maxValid
  -- send this along
  self~getMatchMetrics:super(metrics)

-- add an expression to our list of alternatives
::method addAlternative
  expose alternatives

  use arg newChoice

  alternatives~append(newChoice)


-- check for a lineend in non-unix, multiline mode.  This matches on a '\r\n' sequence
-- OR the end of the input
::class MultiLineEndNode subclass MatchNode
::method match
  use arg context, position, target

  -- end match, this is true
  if context~checkEnd(position) then do
      return self~next~match(context, position, target)
  end

  endPosition = context~endPosition - 2

  -- not enough room for a linend?
  if position > endPosition then do
      return .false
  end

  if target~match(position, '0d0a'x), self~next~match(context, position, target) then do
      return .true
  end
  return .false


-- check for a lineend in unix mode.  This matches on a '\n' sequence
-- but NOT the end of the input
::class UnixLineEndNode subclass MatchNode
::method match
  use arg context, position, target

  if context~checkEnd(position) then do
      return .false
  end

  endPosition = context~endPosition - 1

  -- not possibly at the end of the text?
  if position < endPosition then do
      return .false
  end

  if target~match(position, '0a'x), self~next~match(context, position, target) then do
      return .true
  end
  return .false

-- check for a lineend in unix mode.  This matches on a '\n' sequence
-- but NOT the end of the input
::class UnixMultiLineEndNode subclass MatchNode
::method match
  use arg context, position, target

  if context~checkEnd(position) then do
      return self~next~match(context, position, target)
  end

  if target~match(position, '0a'x), self~next~match(context, position, target) then do
      return .true
  end
  return .false

-- check for being at the end of the text string.  Does not recognize linend sequences.
::class TextEndNode subclass MatchNode
::method match
  use arg context, position, target

  if context~checkEnd(position) then do
      return self~next~match(context, position, target)
  end

  return .false

-- Base class for the different Question nodes.  The init and
-- getMatchMetrics are the same, but the matching logic is different
::class QuestionNode subclass MatchNode
::method init
  expose optional
  use arg optional
  self~init:super
  optional~terminate  -- these needs termination

::attribute optional GET

::method getMatchMetrics
  expose optional
  use arg metrics

  -- our following match pattern is optional, so we ignore any
  -- additions it makes to the metrics before sending it down the chain.
  minLength = metrics~minLength
  optional~getMatchMetrics(metrics)
  metrics~minLength = minLength
  metrics~deterministic = .false

self~getMatchMetrics:super(metrics)


::class GreedyQuestionNode subclass QuestionNode
::method match
  use arg context, position, target

  -- if we match the optional part, and if we match the following part,
  -- this is gold.
  if self~optional~match(context, position, target) then do
      if self~next~match(context, context~matchEnd, target) then do
          return .true
      end
  end

  -- try again, but without the optional section.
  return self~next~match(context, position, target)


::class LazyQuestionNode subclass QuestionNode
::method match
  use arg context, position, target

  -- first try for a match on the trailing part.  If that matches, we ignore
  -- the optional section
  if self~next~match(context, position, target) then do
      return .true
  end

  -- try again, but without the optional section.
  if \self~optional~match(context, position, target) then do
      return .false
  end

  return self~next~match(context, context~matchPosition, target)


::class PossessiveQuestionNode subclass QuestionNode
::method match
  use arg context, position, target

  -- try for the optional match...no backtracking if it is a match.
  if self~optional~match(context, position, target) then do
      position = context~matchEnd
  end

  return self~next~match(context, position, target)


-- match a repetition of nodes
::class RepetitionNode subclass MatchNode
::method init
  expose repNode min max
  use arg repNode, min, max
  self~init:super
  repNode~terminate   -- we need to ensure the chain ends in a terminator

::attribute repNode GET
::attribute min GET
::attribute max GET


::method getMatchMetrics
  expose repNode min max
  use arg metrics
  -- get our repetition node values as if it was the only thing of interest
  submetrics = self~getChildMatchMetrics(repNode)

  metrics~minLength = metrics~minLength + (min * submetrics~minLength)
  metrics~maxLength = metrics~maxLength + (max * submetrics~maxLength)

  -- if the max and min are the same, then this is potentially deterministic
  if max == min then do
      metrics~deterministic = metrics~deterministic & submetrics~deterministic
  end
  else do
      -- no go on the length predictions
      metrics~determinsitic = .false
  end

  -- send this along
  self~getMatchMetrics:super(metrics)


::class GreedyRepetitionNode subclass RepetitionNode
::method match
  use arg context, position, target

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

::method recursiveMatch
  use arg context, position, target, repNode, matches, max

  -- we've already satisfied the minimum, if we're past the end, then
  -- pass on the terminator
  if context~checkEnd(position) then do
  -- try matching the following bit
      return self~next~match(context, position, target)
  end

  -- we hit the max count, now try to match the trailing bit
  if matches >= max then do
      return self~next~match(context, position, target)
  end

  if repNode~match(context, position, target) then do
      -- a pattern that is a zero-length match will cause an infinite loop,
      -- so fail this
      if position == context~matchEnd then do
          return .false
      end
      -- now recurse on this
      if self~recursiveMatch(context, context~matchEnd, target, repNode, matches + 1, max) then do
          return .true
      end
      position = context~matchEnd   -- update the position
  end

  -- try matching the following bit
  return self~next~match(context, position, target)


::class LazyRepetitionNode subclass RepetitionNode
::method match
  use arg context, position, target

  current = position
  repNode = self~repNode

  -- if we have a minimum specified, we must get at least that, else we fail
  do i = 1 to self~min
      if \repNode~match(context, current, target) then do
          return .false
      end
      current = context~matchPosition
  end
  -- we have the minimum number of matches, now proceed trying to avoid checking again
  next = self~next

  matches = self~min
  max = self~max

  do forever
      -- now start checking the following part
      if next~match(context, current, target) then do
          return .true
      end
      -- had the maximum number of variable matches
      if matches >= max then do
          return .false
      end

      if \repNode~match(context, current, target) then do
          return .false
      end

      -- if not moving, this is a failure here
      if current == context~matchPosition then do
          return .false
      end
      -- step the position and count and check again
      current = context~matchPosition
      matches = matches + 1
  end


::class PossessiveRepetitionNode subclass RepetitionNode
::method match
  use arg context, position, target

  current = position
  repNode = self~repNode

  -- if we have a minimum specified, we must get at least that, else we fail
  do i = 1 to self~min
      if \repNode~match(context, current, target) then do
          return .false
      end
      current = context~matchPosition
  end

  do i = self~min + 1 to self~max
      if \repNode~match(context, current, target) then do
          leave
      end

      if current == context~matchPosition then do
          leave
      end

      current = context~matchPosition
  end

  return self~next~match(context, current, target)


-- matches the current position for the same string value as
-- a back reference match
::class BackReferenceNode subclass MatchNode
::method init
  expose ref
  use arg ref
  self~init:super

::method match
  expose ref
  use arg context, position, target

  refGroup = context~getBackReferenceResult(ref)

  -- if the back reference did not match, then this is a
  -- failure here too
  if \refGroup~matched then do
      return .false
  end

  -- this is essentially a string reference now
  matchValue = refGroup~matchString

  if \context~checkEnd(position + matchValue~length - 1) then do
      return .false
  end

  if \target~match(position, matchValue) then do
      return .false
  end

  -- exhausted the max, so now search for the next
  return self~next~match(context, position + matchValue~length, target)

::method getMatchMetrics
  use arg metrics

  metrics~maxValid = .false
  self~getMatchMetrics:super(metrics)

-- same as a back reference, but matching is done caselessly
::class CaselessBackReferenceNode subclass MatchNode
::method init
  expose ref
  use arg ref
  self~init:super

::method match
  expose ref
  use arg context, position, target

  refGroup = context~getBackRefResult(ref)

  if \refGroup~matched then do
      return .false
  end

  matchValue = refGroup~matchString

  if \context~checkEnd(position + matchValue~length - 1) then do
      return .false
  end

  if \target~caselessMatch(position, matchValue) then do
      return .false
  end

  -- exhausted the max, so now search for the next
  return self~next~match(context, position + matchValue~length, target)

::method getMatchMetrics
  use arg metrics

  metrics~maxValid = .false
  self~getMatchMetrics:super(metrics)


-- a normal group node with implicit numbering
::class GroupNode subclass MatchNode
::method init
  expose id groupMatch
  use arg id, groupMatch
  self~init:super

::method match
  expose id groupMatch
  use arg context, position, target

  info = context~getGroupInfo(id)

  -- if the pattern inside the group matches, then record
  -- the match position inside group matches, record this
  -- in the corresponding group information and continue
  if groupMatch~matches(context, position, target) then do
      info~setMatch(target, position, context~matchEnd)
      return self~next~match(context, context~matchEnd, target)
  end
  else do
      -- mark the group as non-matching
      info~clearMatch
      return .false
  end

-- a group node with an explicit name
::class NamedGroupNode subclass MatchNode
::method init
  expose id groupMatch
  use arg id, groupMatch
  self~init:super

::method match
  expose id groupMatch
  use arg context, position, target

  info = context~namedGroupInfo(id)

  if groupMatch~matches(context, position, target) then do
      info~setMatch(target, position, context~matchEnd)
      return self~next~match(context, context~matchEnd, target)
  end
  else do
      info~clearMatch
      return .false
  end

-- base class for logical operations of class patterns
::class ClassLogicalNode subclass MatchNode
::method init
  expose leftSide rightSide
  use arg leftSide, rightSide

  self~init:super
  -- ensure these side branches have terminator elements
  leftSide~terminate
  rightSide~terminate

::method getMatchMetrics
  expose leftSide rightSide
  use arg metrics

  leftMetrics = self~getChildMatchMetrics(leftSide)
  rightMetrics = self~getChildMatchMetrics(rightSide)

  metrics~minLength = metrics~minLength + min(leftMetrics~minLength, rightMetrics~minLength)
  metrics~maxLength = metrics~minLength + max(leftMetrics~maxLength, rightMetrics~maxLength)
  metrics~deterministic = metrics~deterministic & leftMetrics~deterministic & rightMetrics~deterministic

  self~getMatchMetrics:super(metrics)

::attribute leftSide
::attribute rightSide

-- class logical OR node
::class ClassOrNode subclass ClassLogicalNode
::method match
  use arg context, position, target

  if \context~checkEnd(position) then do
      if self~leftSide~match(context, position, target) then do
         return self~next~match(context, context~matchEnd, target)
      end
      else if self~rightSide~match(context, position, target) then do
         return self~next~match(context, context~matchEnd, target)
      end
  end

  return .false

-- class logical AND
::class ClassAndNode subclass ClassLogicalNode
::method match
  use arg context, position, target

  if \context~checkEnd(position) then do
      if self~leftSide~match(context, position, target) then
         if self~rightSide~match(context, position, target) then do
             return self~next~match(context, context~matchEnd, target)
         end
  end

  return .false


-- class Logical & ^ node
::class ClassAndNotNode subclass ClassLogicalNode
::method match
  use arg context, position, target

  if \context~checkEnd(position) then do
      if self~leftSide~match(context, position, target) then
         if \self~rightSide(context, position, target) then do
             return self~next~match(context, context~matchEnd, target)
         end
  end

  return .false


-- base wrapper class for group operations
::class Group subclass MatchNode
::method init
  expose terminator
  terminator = .GroupTerminator~new(self)
  self~init:super


::method match
  use arg context, position, target

  self~setGroupState(context, position)
  result = self~next~match(context, position, target)
  self~restoreGroupState(context)

::method setGroupStart
  -- this is a noop in the base class

::method restoreGroupStart
  -- this is a noop in the base class

::method setGroupEnd
  -- this is a noop in the base class

::method revertGroupEnd
  -- this is a noop in the base class

::method terminator
  expose terminator
  return terminator


-- base class for capturing group interactions
::class CapturingGroup subclass Group
::method init
  expose id
  use arg id
  self~init:super

::method setGroupStart
  expose id
  use arg context, position

  context~saveGroupStart(id, position)

::method restoreGroupStart
  expose id
  use arg context

  context~restoreGroupStart(id)

::method setGroupEnd
  expose id
  use arg context, position

  context~saveGroupEnd(id, position)

::method revertGroupEnd
  expose id
  use arg context

  context~restoreGroupEnd(id)

-- special terminator for the end of a grouping
::class GroupTerminatorNode subclass MatchNode
::method init
  expose group
  use arg group
  self~init:super

::method match
  expose group
  use arg context, position, target

  group~setGroupEnd(context, position)

  -- if we're in a good match, then our info is correct.
  if self~next~match(context, position, target) then do
      return .true
  end

  -- not a match, need to undo the state for back references
  group~revertGroupEnd(context)

  return .false


-- wrapper around a group to give atomic nature.
::class AtomicGroupNode subclass MatchNode
::method init
  expose group
  use arg group
  self~init:super


::method match
  expose group
  use arg context, position, target

  -- do the group match and accounting, then forward along.
  if group~match(context, position, target) then do
      return self~next~match(context, context~matchPosition, target)
  end

  return .false


-- wrapper around a group to give atomic nature.
::class PositiveLookaheadNode subclass MatchNode
::method init
  expose group
  use arg group
  self~init:super


::method match
  expose group
  use arg context, position, target

  -- do the group match and accounting, then forward along.
  -- NOTE:  both of these match the same position
  if group~match(context, position, target) then do
      return self~next~match(context, position, target)
  end

  return .false


-- wrapper around a group to give atomic nature.
::class NegativeLookaheadNode subclass MatchNode
::method init
  expose group
  use arg group
  self~init:super


::method match
  expose group
  use arg context, position, target

  -- do the group match and accounting, then forward along.
  -- NOTE:  both of these match the same position
  if \group~match(context, position, target) then do
      return self~next~match(context, position, target)
  end

  return .false


::class PositiveLookBehindNode subclass MatchNode
::method init
  expose group max min
  use arg group, metrics

  max = metrics~maxLen
  min = metrics~minLen

::method match
  expose group max min
  use arg context, position, target

  -- This is the offset from the beginning of the match
  -- range, which might not be the same as the string beginning
  -- This will tell us the size of the string before this position
  offset = context~matchOffset(position)

  -- we need to have a least the min preceeding us to match
  if offset < min then do
      return .false
  end

  maxStart = min(offset, max)

  backPosition = position - maxStart

  possibleMatchs = backPosition - min + 1

  -- we need to test the shortest matches first
  do i = position - minLength by -1 for possibleMatches
      -- if we have a back match, then time to forward to the next bit
      if group~match(context, i, target) then do
          return self~next~match(context, position, target)
      end
  end

  -- no match
  return .false


::class NegativeLookBehindNode subclass MatchNode
::method init
  expose group max min
  use arg group, metrics

  max = metrics~maxLen
  min = metrics~minLen

  self~init:super

::method match
  expose group max min
  use arg context, position, target

  offset = context~matchOffset(position)

  -- we need to have a least the min preceeding us to match
  if offset < min then do
      return .false
  end

  maxStart = min(offset, max)

  backPosition = position - maxStart

  possibleMatchs = backPosition - min + 1

  -- we need to test the shortest matches first
  do i = position - minLength by -1 for possibleMatches
      -- if we have a back match, then this is a failure
      if group~match(context, i, target) then do
          return .false
      end
  end

  -- no match
  return self~next~match(context, position, target)


::class MultilineCaretNode subclass MatchNode
::method match
  use arg context, position, target

  -- the end is always game over, even if preceeded by a newline
  if context~checkEnd(position) then do
      return .false
  end

  start = context~start

  -- if we're at the start position, this matches.  Otherwise,
  -- we need to see if we're positioned at a lineend
  if start < position  then do
      ch = target~substr(position - 1, 1)

      if \target~matchChar(position - 1, "0A0D"x) then do
          -- not positioned after a lineend, so this is a failure
          return .false
          -- TODO:  Obviously, there is a problem here.

          -- if the previous character is a CR character, we need to
          -- check if we're positioned between the two.  That's also a match
          -- failure
          if ch == '0D'x then do
              if target~match(position, '0A'x) then do
                  return .false
              end
          end
      end
  end

  return self~next~match(context, position, target)


::class UnixMultilineCaretNode subclass MatchNode
::method match
  use arg context, position, target

  -- the end is always game over, even if preceeded by a newline
  if context~checkEnd(position) then do
      return .false
  end

  start = context~start

  -- if we're at the start position, this matches.  Otherwise,
  -- we need to see if we're positioned at a lineend
  if start < position  then do
      if \target~match(position - 1, "0A"x) then do
          -- not positioned after a lineend, so this is a failure
          return .false
      end
  end

  return self~next~match(context, position, target)

-- A terminal node for a full match chain.  This does any
-- end-of-match processing.
::class TerminatorNode subclass MatchNode
::method match
  use arg context, position, target

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

-- A terminal node for a side branch chain.  This does match
-- termination for the branch, but does not enforce any data end
-- match rules
::class BranchTerminatorNode subclass TerminatorNode
::method match
  use arg context, position, target

  -- set the end match position
  context~matchEnd = position
  -- this is always a successful match.  This really just
  -- records the last position of this branch segment.
  return .true


-- Search nodes are not normally part of the pattern tree.  These
-- are only instantiated when a find() operation is being done with
-- a pattern
::class SearchNode subclass MatchNode
::method init
  expose minLength
  use arg pattern
  self~init:super

  self~next = pattern  -- this is our search pattern

  -- to optimize, we scan the tree to figure out what the minium match
  -- length will be.  This will prevent making unnecessary probes on the
  -- target once matches are no longer possible.
  metrics = self~getChildMatchMetrics(pattern)

  minLength = metrics~minLength

::method match
  expose minLength
  use arg context, position, target
  -- NB, + 1 is needed because the start position is not
  -- zero based.  We might have exactly enough for a single test
  probes = (context~endPosition - position) - minLength + 1

  if probes <= 0 then do
      context~hitEnd = .true
      return .false
  end

  do i = position for probes
      -- the test for hitting the end generally turns this on, so ensure it is off
      context~hitEnd = .false
      if self~next~match(context, i, target) then do
          -- update the match information
          context~setMatchPosition(i, context~matchEnd);
          return .true
      end
  end
  -- no matches possible

  return .false


::method getMatchMetrics
  use arg metrics

  self~getMatchMetrics:super(metrics)

  -- because we slide, these are never valid
  metrics~deterministic = .false
  metrics~maxValid = .false


-- base class for a match result from a group match.  This is
-- similar to the MatchResult class, but lacks the ability to request
-- embedded group values
::class GroupMatchResult
::method init
expose regionStart regionEnd start end text hitEnd
use strict arg text, regionStart, regionEnd, start, end, hitEnd
-- return a response if the operation matched.
::method matched
  expose start
  return start > 0

-- individual match results
::attribute start GET
::attribute end GET

-- information about the string region we're matching on
::attribute regionStart GET
::attribute regionEnd GET
::attribute regionLength GET
  expose regionStart regionEnd
  return regionEnd - regionStart + 1  -- end position is Rexx-like last character

-- retrieve the length of the match string
::attribute length GET
  expose end start
  return end - start -- end position is one past the last match character
-- retrieves the match text
::attribute text GET
  expose text start
  -- return the match text
  if start == 0 then do
      return ''
  end
  -- extract the text subpiece
  return text~substr(start, self~length)

-- retrieve the prefix string before the match, up to the start
-- of the match region
::attribute prefix GET
  expose text start regionStart
  -- no match is always a null string
  if start == 0 then do
      return ""
  end

  return text~substr(regionStart, start - regionStart)

-- retrieve any text following the match position, up to the end
-- of the match region
::attribute suffix GET
  expose text start end regionStart regionEnd
  -- no match is always the entire region string
  if start == 0 then do
  return text~substr(regionStart, regionEnd - regionStart)
      return ""
  end
  return text~substr(end, regionEnd - end)

-- return the position of character after the match position.  If
-- past the end of the match region, returns regionEnd + 1
::method nextMatch
  expose end
  -- NOTE:  This is the correct result even if the match string
  -- is zero-length
  return end + 1

-- Information about an individual match.  The match
-- may have embedded groups.  The match information for
-- the embedded groups can also be retrieved
::class MatchResult subclass GroupMatchResult
::method init
expose groups
use strict arg text, regionStart, regionEnd, start, end, hitEnd, groups
self~init:super(text, regionStart, regionEnd, start, end, hitEnd)

-- retrieve a group.  This is either a numeric id
-- or a symbolic name
::method group
  expose groups
  use strict arg id
  group = groups[id]
  if .nil == group then do
      raise syntax 93.900 array("Group" id "not found in regular expression pattern") -- this is an error
  end

  return group

-- the current active context for a match operation.  This
-- provides information to the matching filters and holds any
-- intermediate state required by the filters.  This also
-- implements the matching operations.
::class MatchContext
::method init
  expose text startPosition endPosition matchPosition matchEnd root groups hitEnd
  use strict arg text, startPosition, length, root, groups
  -- make sure the length is bounded.  The end position is one
  -- past the end
  endPosition = min(text~length + 1, startPosition + length)
  matchPosition = 0
  matchEnd = 0
  hitEnd = .false  -- indicator of whether a search has reached the end
  -- if we have groups, then do a deep copy of the directory
  -- to create a working groups list
  if groups \= .nil then do
      groups = groups~copy
      do name over groups
          groups[name] = groups[name]~copy
      end
  end

-- test if the pattern is an exact match for the entire region, from
-- beginning to end
::method matches
  expose text startPosition matchPosition hitEnd matchEndRequired root
  hitEnd = .false
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
  expose text startPosition matchPosition hitEnd matchEndRequired root
  hitEnd = .false
  matchEndRequired = .false  -- This does not need to match exactly
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

-- search the region for the first position with a pattern match.  Returns
-- a match result object with the match result
::method find
  expose text startPosition endPosition matchPosition hitEnd matchEndRequired root
  -- the start is optional, but defalt to the beginning of the
  -- region (typical)
  use strict arg start = (startPosition)

  if startPosition > endPosition then do
      matchPosition = 0
      hitEnd = .true  -- sort of true by definition
      return self~matchResult  -- give a match result object with the info
  end

  hitEnd = .false
  matchEndRequired = .false  -- This does not need to match exactly
  -- this node implements a sliding search search of the pattern.
  -- Not really part of the pattern tree, but it wrappers it
  searcher = .SearchNode~new(root)
  if \searcher~match(self, startPosition, text) then do
      matchPosition = 0   -- this needs to be cleared
  end
  return self~result  -- give a match result object with the info

-- return a result object from the match result
::method result
  expose text startPosition endPosition matchPosition matchEnd groups hitEnd
  return .MatchResult~new(text, startPosition, endPosition, matchPosition, matchEnd, hitEnd, groups)

::method atStart
  expose startPosition
  use arg test
  return test == startPosition

-- tests for whether a position is past the end of the range
::method checkEnd
  expose endPosition hitEnd
  use arg test
  if test >= endPosition then do
      -- remember that we've hit the end of the pattern
      hitEnd = .true
      return .true
  end
  return .false

-- the range positions
::attribute startPosition
::attribute endPosition

-- any set match positions
::attribute matchPosition
::attribute matchEnd

-- indicates we've run out of string before the match
-- completes
::attribute hitEnd

-- indicates whether the pattern must exactly match the end
-- of the string (e.g., we're doing a match vs. a locate)
::attribute matchEndRequired

::method setMatchPosition
  expose matchPosition matchEnd
  use arg matchPosition, matchEnd

::method getBackReferenceResult
  expose groups
  use arg id

  if groups == .nil then do
      raise syntax 93.900 array("Back reference to unknown group" id);
  end

  groupRef = groups[id]
  if groupRef == .nil then do
      raise syntax 93.900 array("Back reference to unknown group" id);
  end

  return groupRef

-- Create the group reference item for a numbered or named group.
-- If the group already exists, this is an error
::method getGroupReference
  expose groups
  use arg id

  if groups == .nil then do
      groups = .directory~new
  end

  if groups~hasIndex(id) then do
      raise syntax 93.900 array("Duplicate group identifier" id);
  end

  groupInfo = .GroupReference~new(id)
  groups[id] = groupInfo
  return groupInfo

::method saveGroupStart
  use arg id, position
  groupInfo = self~getGroupReference(id)
  groupInfo~saveStart(position)

::method saveGroupEnd
  use arg id, position
  groupInfo = self~getGroupReference(id)
  groupInfo~saveEnd(position)

::method restoreGroupStart
  use arg id, position
  groupInfo = self~getGroupReference(id)
  groupInfo~restoreStart(position)

::method restoreGroupEnd
  use arg id, position
  groupInfo = self~getGroupReference(id)
  groupInfo~restoreEnd(position)

-- return the offset of the given position.  This will be
-- the number of characters from the start of the match
-- range to the given position.
::method matchOffset
  expose start
  use arg position

  return position - start


::class GroupReference
::method init
  expose id start end text startStack endStack
  use arg id
  start = 0
  end = 0
  text = .nil
  startStack = .nil
  endStack = .nil

-- the start and end match positions
::attribute start
::attribute end

-- indicates whether this match was a success or a failure
::method matched
  expose start
  return start > 0

-- set the match values
::method setMatch
  expose text start end
  use arg text, start, end

-- clear the match values
::method clearMatch
  expose text start end
  text = .nil
  start = 0
  end = 0

-- return the text that was matched.  Returns .nil if
-- this match was not successful
::method matchText
  expose text start end

  if start > 0 then do
      return text~substr(start, end - start + 1)
  end

  return .nil

-- set a new starting position for the group, but save
-- the previous position in a stack in case it needs restoring
::method saveStart
  expose start startStack
  use arg newStart

  if startStack = .nil then do
      startStack = .queue~new
  end

  startStack~push(start)
  start = newStart

-- set a new end position for the group, but save
-- the previous position in a stack in case it needs restoring
::method saveEnd
  expose end endStack
  use arg newEnd

  if endStack = .nil then do
      endStack = .queue~new
  end

  endStack~push(end)
  end = newEnd

-- methods for restoring the previous start and end positions
::method restoreStart
  expose start startStack

  start = startStack~pop

::method restoreEnd
  expose end endStack

  end = endStack~pop

-- a general purpose parsing context that includes support for
-- regular expressions
::class parser public
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
-- section startstart would be 4, sectionend would be 8 after the
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
section = self~section
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
use arg matchStart

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
