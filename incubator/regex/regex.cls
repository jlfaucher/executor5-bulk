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
::class Pattern public
::constant multiline "MULTILINE"
::constant singleline "SINGLELINE"
::constant internetlines "INTERNETLINES"
::constant unixlines "UNIXLINES"
::constant caseless  "CASELESS"
::constant respectcase "RESPECTCASE"
::constant dotall "DOTALL"
::constant dotrestricted "DOTRESTRICTED"

::method compile class
  -- the compiler is configurable, but there is a default
  use strict arg pattern, compiler = (.RegexCompiler~new)
  return compiler~compile(pattern)

::method init
  expose pattern root
  use arg pattern, root

::attribute pattern get

::method string
  expose pattern
  return pattern

::method matches
  -- TODO implement matching logic

::method split
  -- TODO implement splitting logic


::class RegexCompiler public
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

::method classFamily class
  expose classFamilies registeredClassFamilies
  use arg name

  family = classFamilies[name]

  if .nil == family then do
      return registeredClassFamilies[name]
  end

  return family

::method registerClassFamily class
  expose classFamilies registeredClassFamilies
  use arg name, chars

  if classFamilies~hasIndex(name) then do
      raise syntax 98.900 array("Class family" name "already exists")
  end

  registeredClassFamilies[name] = chars

::method init
  use strict arg options = .nil
  validateOptions(arg(1))

-- validate any options passed to the compiler when created
::method validateOptions private
  use arg options
  if options == .nil then do
      return
  end

  options = options~upper

  do while options \== ""
      parse var options option options

      select
          when option == .Pattern~MULTILINE then do
              self~multiline = .true
          end
          when option == .Pattern~SINGLELINE then do
              self~multiline = .false
          end
          when option == .Pattern~INTERNETLINES then do
              self~unixlines = .false
          end
          when option == .Pattern~UNIXLINES then do
              self~unixlines = .true
          end
          when option == .Pattern~CASELESS then do
              self~caseless = .true
          end
          when option == .Pattern~RESPECTCASE then do
              self~caseless = .false
          end
          when option == .Pattern~DOTALL then do
              self~dotall = .true
          end
          when option == .Pattern~DOTRESTRICTED then do
              self~dotall = .false
          end
          otherwise  do
              raise syntax 93.915 array("MULTILINE SINGLELINE INTERNETLINES UNIXLINES CASELESS RESPECTCASE DOTALL DOTRESTRICTED", option)
          end
      end
  end

::method compile
  expose pattern current length
  use strict arg pattern
  current = 1     -- always start at the beginning
  length = pattern~length

  -- this is the last node that will get control only if all other
  -- elements match cleanly.  The expression parser hooks this up to the
  -- end of the expression graph.
  lastNode = .TerminatorNode~new
  rootNode = self~parseExpression(lastNode)
  -- and return a usable pattern for this
  return .Pattern~new(pattern, rootNode)

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

-- move the cursor to a previous position, ensuring that we
-- don't move past the beginning
::method previous
  expose current length

  current = max(current - 1, 1)

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
      node = parseSequence(terminator)
      if alternative \== .nil then do
          alternative~addAlternative(node)
      end

      -- look ahead to see if this is again part of an alternation
      ch = self~peek

      if ch \== "|" then do
          -- not an alternative....see what we need to return
          if alternative \== .nil then do
              return node
          end
          else do
              return alternative
          end
      end

      -- if we've only processed the first node, create an alternative and
      -- add the node.
      if alternative == .nil then do
          alternative = .AlternativeNode~new
          alternative~addAlternative(node)
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
              node = self~parseAtom
          end
      end

      node = self~processClosure(node)
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
  else if self~unixLineends then do
      return .UnixMultilineCaretNode~new
  end
  else do
      return .MultilineCaretNode~new
  end


::method parseEndAnchor
  -- an end anchor, which has 4 possibilities
  if self~unixLinends then do
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
  if self~dotMatchAll then do
      return .AllDotNode~new
  end
  else if self~unixLineends then do
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

  node = parseClosure(groupInfo~firstNode)
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
::method unixlines attribute
::method caseless attribute
::method multiline attribute
::method dotall attribute


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
          ch = self~next
          -- ?? operates in lazy mode
          if ch == "?" then do
              return .LazyQuestionNode~new(target)
          end
          -- possesive question mode
          else if ch == "+" then do
              return .PossessiveQuestionNode~new(target)
          end
          else do
              -- no modifier is greedy
              self~previous
              return .GreedyQuestionNode~new(target)
          end
      end
      -- zero or more occurrences.  This also operates with mode
      -- modifiers
      when ch == "*" then do
          ch = self~next

          if ch == "?" then do
              return .LazyRepetitionNode~new(target, 0, 999999999)
          end
          else if ch == "+" then do
              return .PossessiveRepetitionNode~new(target, 0, 999999999)
          end
          else do
              self~previous
              return .GreedyRepetitionNode~new(target, 0, 999999999)
          end
      end
      -- one or more occurrences.  Also with different modes of
      -- operation
      when ch == "+" then do
          ch = self~next

          if ch == "?" then do
              return .LazyRepetitionNode~new(target, 1, 999999999)
          end
          else if ch == "+" then do
              return .PossessiveRepetitionNode~new(target, 1, 999999999)
          end
          else do
              self~previous
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
          if .nil == min || .nil == max then do
              raise syntax 93.900 array("Invalid repetition count")
          end

          if max < min then do
              raise syntax 93.900 array("Repetition maximum" max "is less than minimum" min)
          end
          ch = self~next

          if ch == "?" then do
              return .LazyRepetitionNode~new(target, min, max)
          end
          else if ch == "+" then do
              return .PossessiveRepetitionNode~new(target, min, max)
          end
          else do
              self~previous
              return .GreedyRepetitionNode~new(target, min, max)
          end
      end
      otherwise do
          -- no additional closure
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
      char = self~next

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
          otherwise do
              firstchar = .false
              self~previous
          end
      end

      -- processed all of the special chars, we should be looking at class characters.
      -- process this into a node.
      node = self~parseClassRange

      if negated then do
          if previousNode \= .nil then do
              previousNode = .ClassAndNotNode~new(previousNode, node)
          end
          else do
              previousNode = node
          end
      end
      else do
          if previousNode \= .nil then do
              previousNode = .ClassOrNode~new(previousNode, node)
          end
          else do
              previousNode = node
          end
      end
      negated = .false
  end


::method parseClassRange
  firstchar = .true
  characters = ""
  inRange = .false
  startRange = ""

  ch = self~peek
  if ch == "^" then do
      negated = .true
      self~next
  end

  do forever
      ch = self~next

      if ch == .nil then do
          leave
      end
      else if ch == "-" then do
          if characters == "" then do
              characters = "_"
          end
          else do
              if inRange then do
                  raise syntax 93.900 array("extra '-' in character range")
              end
              inRange = .true
              startRange = characters~right(1)
              characters = characters~substr(1, characters~length - 1)
          end
      end
      else if ch == "\\" then do
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
      if \inRange then do
          characters = characters || ch
      end
      else do
          if ch < startRange then do
              raise syntax 93.900 array("End range character '"ch"' is less than starting character '"startRange"'")
          end
          characters = characters || xrange(startRange, ch)
          inRange = .false
      end
  end

  return .ClassNode~new(characters)

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
          -- if this is a non-escaped special char,
          -- backup and finish up this section
          when "$,^([|)"~pos(ch) > 0 then do
              self~previous
              leave
          end
          when ch == '\' then do
              ch = self~parseEscapedCharacters
              -- this is either an invalid escape char, or an operation
              -- we push the backshad back on, and stop processeing.
              if ch == .nil then do
                  self~previous
                  leave
              end
          end

          otherwise do
              -- We have a good character, but it might be followed
              -- by a modifier.  If it is, push it back and terminate
              -- the parsing here.
              if "*+?{"~pos(self~peek) > 0  then do
                  self~previous
                  leave
              end

              characters = characters + ch
          end
      end
  end

  -- this is either a single char node, or some sort of string sequence.
  if characters~length == 1 then do
      return self~singleNode(characters)
  end
  else do
      return self~stringNode(characters)
  end

-- create a node for matching a single character
::method singleNode
  use arg char

  if self~caseless then do
      return .CaselessSingleCharNode~new(char)
  end
  else do
      return .SingleCharNode~new(char)
  end

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
          if self~unixLinends then do
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

::method readHex private
  ch = self~next

  if .nil \= ch then do
      if '1234567890abcdefABCDEF'~pos(ch) > 0 then do
          raise syntax 93.900 array("Invalid hex digit '"ch"'")
      end
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
  return metrics


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


::class EndTextNode subclass matchNode
::method match
  use arg context, position, target

  if context~checkEnd(position), self~next~match(context, position, target) then do
      return .true
  end
  return .false


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


::class LastMatchNode subclass MatchNode
::method match
  use arg context, position, target

  if position \= context~matchPosition then do
      return .false
  end

  return self~next~match(context, position, target)

-- match for a single character
::class SingleCharNode subclass matchNode
::method init
  expose matchChar
  use arg matchChar
  self~init:super

::attribute matchChar

::method match
  expose matchChar

  use arg context, position, target

  if context~checkEnd(position) then do
      return .false
  end

  if \target~match(position, matchChar) then do
      return .false
  end

  return self~next~match(context, position + 1, target)

::method getMatchMetrics
  use arg metrics

  metrics~maxLen = metrics~maxLen + 1
  metrics~minLen = metrics~minLen + 1

  self~getMatchMetrics:super(metrics)

-- caseless match for a single character
::class CaselessSingleCharNode subclass SingleCharNode
::method match
  use arg context, position, target

  if context~checkEnd(position) then do
      return .false
  end

  if \target~caselessMatch(position, self~matchChar) then do
      return .false
  end

  return self~next~match(context, position + 1, target)

-- inverse match for a single character
::class NotSingleCharNode subclass SingleCharNode
::method match
  use arg context, position, target

  if context~checkEnd(position) then do
      return .false
  end

  -- if we have a match at this position, then this is false
  if target~match(position, self~matchChar) then do
      return .false
  end

  -- forward down the chain, we're good so far
  return self~next~match(context, position + 1, target)

-- inverse caseless match for a single character
::class NotCaselessSingleCharNode subclass SingleCharNode
::method match
  use arg context, position, target

  if context~checkEnd(position) then do
      return .false
  end

  -- if we have a match at this position, then this is false
  if target~caselessMatch(position, self~matchChar) then do
      return .false
  end

  -- forward down the chain, we're good so far
  return self~next~match(context, position + 1, target)

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
  metrics~maxLen = metrics~maxLen + 1
  metrics~minLen = metrics~minLen + 1

  self~getMatchMetrics:super(metrics)


-- like all dot node, but doesn't match lineends (CRLF sequences,
-- in this case)
::class DotNode subclass AllDotNode
::method match
  use arg context, position, target

  if context~checkEnd(position) then do
      return .false
  end

  ch = target~subChar(position)
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
  if context~checkEnd(position + matchString~length) then do
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
  metrics~maxLen = metrics~maxLen + matchString~length
  metrics~minLen = metrics~minLen + matchString~length

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
  metrics~maxLen = metrics~maxLen + 1
  metrics~minLen = metrics~minLen + 1

  self~getMatchMetrics:super(metrics)

-- an inversion of the class node
::class NotClassNode subclass ClassNode
::method match
  expose characters

  use arg context, position, target

  if context~checkEnd(position) then do
      return .false
  end

  if target~matchChar(position, characters) then do
      return .false
  end

  return self~next~match(context, position + 1, target)


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

  maxLen = 0
  minLen = 999999999
  deterministic = .true
  maxValid = .true

  -- we need to analyze all of the alternative paths.

  do alternative over alternatives
      -- get our repetition node values as if it was the only thing of interest
      submetrics = self~getChildMetrics(repNode)
      maxLen = max(maxLen, subMetrics~maxLen)
      minLen = min(minLen, subMetrics~minLen)
      deterministic = deterministic & submetrics~deterministic
      maxValid = maxValid & submetrics~deterministic
  end


  metrics~minLen = metrics~minLen + minLen
  metrics~maxLen = metrics~maxLen + maxlen

  metrics~deterministic = metrics~deterministic & deterministic
  metrics~maxValid = metrics~maxValid * maxValid
  -- send this along
  self~getMatchMetrics:super(metrics)

-- add an expression to our list of alternatives
::method addAlternative
  expose alternatives

  use arg newChoice

  alternatives~append(newChoice)


::class WordStartNode subclass MatchNode
::method match
  use arg context, position, target

  if context~checkEnd(position) then do
      return .false
  end

  -- not a alphanumeric at the current position, so cannot be a word start
  if \target~matchChar(position, self~wordChars) then do
      return .false
  end

  -- if this is at the start, then it is a word start
  if \context~atStart(position - 1) then do
      return .true
  end

  -- the previous character must not be a word character
  if target~matchChar(position - 1, self~wordChars) then do
      return .false
  end

  -- this one does not advance the position, it just validates the position
  return self~next~match(context, position, target)


::class WordEndNode subclass MatchNode

::method match
  expose wordChars
  use arg context, position, target

  -- if not at the end, make sure this position is not a word char
  if \context~checkEnd(position) then do
      if target~matchChar(position, self~wordChars) then do
          return .false
      end
  end

  -- this one does not advance the position, it just validates the position
  return self~next~match(context, position, target)


-- check for a lineend in non-unix mode.  This matches on a '\r\n' sequence
-- but NOT the end of the input
::class LineEndNode subclass MatchNode
::method match
  use arg context, position, target

  -- always false if past the end
  if context~checkEnd(position) then do
      .false
  end

  endPosition = context~endPosition - 2

  -- not possibly at the end of the text?
  if position < endPosition then do
      return .false
  end

  if position == endPosition then do
      if target~match(position, '0d0a'x), self~next~match(context, position, target) then do
          return .true
      end
  end

  return .false


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

::attribute optional GET

::method getMatchMetrics
  expose optional
  use arg metrics

  -- our following match pattern is optional, so we ignore any
  -- additions it makes to the metrics before sending it down the chain.
  minLen = metrics~minLen
  optional~getMatchMetrics(metrics)
  metrics~minLen = minLen
  metrics~deterministic = .false

self~getMatchMetrics:super(metrics)


::class GreedyQuestionNode subclass QuestionNode
::method match
  use arg context, position, target

  -- if we match the optional part, and if we match the following part,
  -- this is gold.
  if self~optional~match(context, position, target) then do
      if self~next~match(context, context~matchPosition, target) then do
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
      position = context~matchPosition
  end

  return self~next~match(context, position, target)


-- match a repetition of nodes
::class RepetitionNode subclass MatchNode
::method init
  expose repNode min max
  use arg repNode, min, max
  self~init:super

::attribute repNode GET
::attribute min GET
::attribute max GET


::method getMatchMetrics
  expose repNode min max
  use arg metrics
  -- get our repetition node values as if it was the only thing of interest
  submetrics = self~getChildMetrics(repNode)

  metrics~minLen = metrics~minLen + (min * submetrics~minLen)
  metrics~maxLength = metrics~maxLength + (max * submetrics~maxLen)

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
      current = context~matchPosition
  end

  return self~recursiveMatch(context, current, target, repNode, self~min, self~max)

::method recursiveMatch
  use arg context, position, target, repNode, matches, max

  -- we hit the max count, now try to match the trailing bit
  if matches >= max then do
      return self~next~match(context, position, target)
  end

  if repNode~match(context, position, target) then do
      -- a pattern that is a zero-length match will cause an infinite loop,
      -- so fail this
      if position == context~matchPosition then do
          return .false
      end
      -- now recurse on this
      if self~recursiveMatch(context, context~matchPosition, target, repNode, matches + 1, max) then do
          return .true
      end
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


::class BackReferenceNode subclass MatchNode
::method init
  expose ref
  use arg ref
  self~init:super

::method match
  expose ref
  use arg context, position, target

  refGroup = context~getBackReferenceResult(ref)

  if \refGroup~matched then do
      return .false
  end

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


::class GroupNode subclass MatchNode
::method init
  expose id groupMatch
  use arg id, groupMatch
  self~init:super

::method match
  expose id groupMatch
  use arg context, position, target

  info = context~getGroupInfo(id)

  if groupMatch~matches(context, position, target) then do
      info~setMatch(target, position, context~matchEnd)
      return self~next~match(context, context~matchEnd, target)
  end
  else do
      info~clearMatch
      return .false
  end


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


::class ClassLogicalNode subclass MatchNode
::method init
  expose leftSide rightSide
  use arg leftSide, rightSide

  self~init:super

::method getMatchMetrics
  expose leftSide rightSide
  use arg metrics

  leftMetrics = self~getChildMetrics(leftSide)
  rightMetrics = self~getChildMetrics(rightSide)

  metrics~minLen = metrics~minLen + min(leftMetrics~minLen, rightMetrics~minLen)
  metrics~maxLen = metrics~minLen + max(leftMetrics~maxLen, rightMetrics~maxLen)
  metrics~deterministic = metrics~deterministic & leftMetrics~deterministic & rightMetrics~deterministic

  self~getMatchMetrics:super(metrics)

::attribute leftSide
::attribute rightSide


::class ClassOrNode subclass ClassLogicalNode
::method match
  use arg context, position, target

  if \context~checkEnd(position) then do
      if self~leftSide~match(context, position, target) then do
         return self~next~match(context, context~matchEnd, target)
      end
      else if self~rightSide(context, position, target) then do
         return self~next~match(context, context~matchEnd, target)
      end
  end

  return .false

::class ClassAndNode subclass ClassLogicalNode
::method match
  use arg context, position, target

  if \context~checkEnd(position) then do
      if self~leftSide~match(context, position, target) then
         if self~rightSide(context, position, target) then do
             return self~next~match(context, context~matchEnd, target)
         end
  end

  return .false


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


::class GroupTerminatorNode subclass Node
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
  do i = position - minLen by -1 for possibleMatches
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
  do i = position - minLen by -1 for possibleMatches
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


::class TerminatorNode subclass MatchNode
::method match
  use arg context, position, target

  -- set the end match position
  context~matchEnd = position
  -- this is always a successful match.  This really just
  -- records the last position of this branch segment.
  return .true


::class SearchNode subclass MatchNode
::method init
  expose minLen
  use arg pattern

  self~next = pattern  -- this is our search pattern

  -- to optimize, we scan the tree to figure out what the minium match
  -- length will be.  This will prevent making unnecessary probes on the
  -- target once matches are no longer possible.
  metrics self~getChildMetrics(pattern)

  minLen = metrics~minlen
  self~init:super

::method match
  expose minLen
  use arg context, position, target

  probes = (context~endPosition - position) - minLen

  if probes <= 0 then do
      context~hitEnd = .true
      return .false
  end


  do i = position for probes
      if self~next~match(context, i, target) then do
          -- update the match information
          context~setMatchPosition(i, context~matchEnd);
          return .true
      end
  end
  -- no matches possible
  context~hitEnd = .true

  return .false


::method getMatchMetrics
  use arg metrics

  self~getMatchMetrics:super(metrics)

  -- because we slide, these are never valid
  metrics~deterministic = .false
  metrics~maxValid = .false

::class Matcher
::method init
  expose parentPattern text
  use strict arg parentPattern, text
  self~reset    -- reset all field values

-- reset the matcher to a clean state
::method reset
  expose first last from to

  first = -1
  last = 0
  -- set the scan limits based on the string
  from = 1
  to = self~testLength
  return self

-- get the length of the text
::method textLength private
  expose text
  return text~length

-- Information about an individual match.  The match
-- may have embedded groups.  The match information for
-- the embedded groups can also be retrieved
::class MatchResult subclass GroupMatchResult
::method init
expose start end text groups
use strict arg start, end, text, groups = .nil
-- return a response if the operation matched.
::method matched
  expose start
  return start > 0

-- individual match results
::attribute start GET
::attribute end GET
::attribute text GET

-- retrieve a group.  This is either a numeric id
-- or a symbolic name
::method group
  expose groups
  use strict arg id
  group = groups[id]
  if .nil == group then do
      raise syntax ...  -- this is an error
  end

  return group

::class MatchContext
::method init
  expose text startPosition endPosition matchPosition groups hitEnd
  use arg text, startPosition = 1, length = (text~length)
  endPosition = min(text~length, startPosition + length - 1)
  matchPosition = 0
  groups = .nil   -- default is not to have groups
  hitEnd = false  -- indicator of whether a search has reached the end

::method atStart
  expose startPosition
  use arg test
  return test == startPosition

-- tests for whether a position is past the end of the range
::method checkEnd
  expose endPosition
  use arg test
  return test > endPosition

-- the range positions
::attribute startPosition
::attribute endPosition

-- any set match positions
::attribute matchPosition
::attribute matchEnd

::attribute hitEnd

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

::attribute start
::attribute end

::method matched
  expose start
  return start > 0

::method setMatch
  expose text start end
  use arg text, start, end

::method clearMatch
  expose text start end
  text = .nil
  start = 0
  end = 0

::method matchValue
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


::class parser public
::method init
expose string matchstart matchend sectionstart sectionlength
use arg string

matchstart = 1;
matchend = 1;
sectionstart = 1;
sectionstart = 1
sectionlength = string~length

::method matchstart
expose matchstart
return matchstart

::method sectionstart
expose sectionstart
return sectionstart

::method sectionlength
expose sectionlength
return sectionlength

::method section
expose string sectionstart sectionlength
return string~substr(sectionstart, sectionlength)

::method sectionwords
section = self~section
count = section~words
wordlist = .array~new(count)

do i = 1 to count
   wordlist[i] = section~word(i)
end

return wordlist

::method remainder
expose string matchend
return string~substr(matchend)

::method match
expose string matchstart matchend sectionstart sectionlength
use arg needle

sectionstart = matchend

matchstart = string~pos(needle, matchend)

if matchstart == 0 then do
    matchstart = string~length + 1
    matchend = matchstart
    sectionlength = matchstart - sectionstart
    return .false
end
else do
    matchend = matchstart + needle~length
    sectionlength = matchstart - sectionstart
    return .true
end

::method move
expose string matchstart matchend sectionstart sectionlength
use arg delta

sectionstart = matchstart
matchstart = matchstart + delta

self~validateposition

::method moveTo
expose string matchstart matchend sectionstart sectionlength

sectionstart = matchstart
use arg matchstart

self~validateposition

::method validateposition private
expose string matchstart matchend sectionstart sectionlength

if matchstart < 1 then do
    matchstart = 1
end
else if matchstart > string~length then do
    matchstart = string~length + 1
end


if matchstart <= sectionstart then do
    sectionlength = string~length - sectionstart + 1
end
else do
    sectionlength = matchstart - sectionstart
end

matchend = matchstart
