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
/* Rexx source code documentation system */
/**
* This is a set of classes to parse ooRexx source code and to extract
* information from it to create source code documentation. It parses
* all directives and collects comments belonging to them.
* @todo Add a RXDOC class, composite pattern
**/
if .environment~FileParser = .nil then .environment~FileParser = .FileParser

/** This is the main class for parsing rexx source files. It produces a parse
* tree that can be used to create a visualisation of the source.
*/
::CLASS FileParser
/** This method returns the default options. A parser can be created with
* custom options, but they should be a modified copy of the default options.
*/
::METHOD defaultOptions CLASS
  expose defaultOptions
  if var("defaultOptions") then return defaultOptions~copy

  -- intialize the singleton variable with its default values
  defaultOptions = .directory~new
  defaultOptions["FOLLOW.REQUIRES"] = .true
  defaultOptions["EVALUATE.CONTINUATION"] = .true
  defaultOptions["WARN.AS.ERROR"] = .false
  defaultOptions["WARN.PRINTLINE"] = .true
  defaultOptions["ERROR.PRINTLINE"] = .true
  return defaultOptions~copy

/** Initializer for the FileParser class. The first argument is the file name
* followed by optional options and a parent tree. The parent tree is only used
* internally.
* @param file The file name to work with.
* @param options Optional options directory. If not supplied the default option
* directory is used instead.
* @param parent The parent file, defaults to .nil.
* @todo Allow working on in buffer source.
*/
::METHOD Init
  expose file data tree has options parseState node parent
  use strict arg file, options = (self~class~defaultOptions), parent = .nil
  parseState = .ParseState~new(file,options)
  has = .directory~new
  node = .nil

/** Call this method to start parsing a file. 
*/
::METHOD Parse
  expose file state options tree source parent
  source = .SourceContainer~new
  tree = .SourceFile~new(file, source, "", parent, self~ParseState)
  expectDefinition = .false
  fileStream = .stream~new(file)
  i = 0
  append = .false
  prefix = ""
  do while fileStream~lines > 0
    i+=1
    line = prefix||fileStream~lineIn
    -- handle continuation chars
    lastChar = line~right(1)
    -- check options if - and , at the end of line are to be evaluated
    if options~evaluate.continuation then
      -- only evaluate line continuation outside rxdoc sections
      if state != "RXDOC" & lastChar = "," | lastChar = "-" then do 
        append = .true
        -- cut the last character
        prefix = line~left(line~length-1)||" "
      end
      else if append then do 
        append = .false
        prefix = ""
      end
    if \ append then do
      self~ParseState~lineNumber = i
      self~ParseState~line = line
      self~parseLine(line~strip,line)
    end
  end
  -- start resolving missing references only when this is the root parser
  -- this method is propagated through the whole tree
  if tree~parent = .nil then tree~resolve

::ATTRIBUTE Tree Get
::ATTRIBUTE ParseState Get
::ATTRIBUTE Options Get

/** Get the current RXDOC and reset it
*/
::METHOD getResetRxDoc PRIVATE
  expose rxdoc
  r = rxdoc
  -- In future rxdoc will be a special object...
  rxdoc = .rxdoc~new
  return r

/** The main parsing method. It maintains a state for the current parsing
* state. It creates parsers for source elements.
*/
::METHOD parseLine PRIVATE
  expose tree node state nextstate has accu rxdoc options source
  use arg line, rawline
  if \ var("state") then state = "UNDETERMINED"
--  say state
  select
    -- the initial state, everything can follow
    -- to avoid parsing wrong files there is a "has" directory that contains
    -- all points that have been found so far.
    when state = "UNDETERMINED" then do
      if line~left(2) = "::" then repeat("DIRECTIVE")
      else if line~left(3) = "/**" then repeat("RXDOC")
      else if line~caselessPos(": procedure") > 0 then do
        parse upper var line name": PROCEDURE"
        if 0 = name~verify("ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890") then
          repeat("PROCEDURE")
      end
      if source \= .nil then source~append(rawline)
    end
    when state = "DIRECTIVE" then do
      source = .SourceContainer~new
      line = line~right(line~length - 2)
      directive = line~word(1)~upper
      parameters = line~delword(1)
      select
        -- parse routines
        when directive = "ROUTINE" then do
          rd = .RoutineDirective~new(self)~~parse(line,source,,
            self~getResetRxDoc, tree)
          tree~routines~put(rd~getObject)
          node = .nil
        end
        when directive = "REQUIRES"  & has~ClassMethod = .nil then do
          rd = .RequiresDirective~new(self)~~parse(line,source,,
            self~getResetRxDoc, tree)
          tree~requires~put(rd~getObject)
        end
        -- parse ::CLASS
        when directive = "CLASS" then do
          has~ClassMethod = .true
          cp = .ClassDirective~new(self)~~parse(line,source,,
            self~getResetRxDoc,tree)
          node = cp~getObject
          tree~classes~put(node)
        end
        -- parse ::METHOD
        when directive = "METHOD" then do
          has~ClassMethod = .true
          -- Is this a floating method or does it belong to some class?
          if node \= .nil then
            target = node
          else
            target = tree
          mp = .MethodDirective~new(self)~~parse(line,source,,
            self~getResetRxDoc,target)
          method = mp~getObject
          target~methods~put(method)
        end
        -- parse ::ATTRIBUTE
        when directive = "ATTRIBUTE" then do
          has~ClassMethod = .true
          if node \= .nil then
            target = node
          else
            target = tree
          ad = .AttributeDirective~new(self)~~parse(line,source,,
            self~getResetRxDoc,target)
          attribute = ad~getObject
          target~attributes~put(attribute)
        end
        -- parse ::CONSTANT
        when directive = "CONSTANT" then do
          if node = .nil then self~ParseState~error("Constant without class")
          else do
            cd = .ConstantDirective~new(self)~~parse(line,source,,
              self~getResetRxDoc,node)
            constant= cd~getObject
            node~constants~put(constant)
          end
        end
        otherwise do
          self~ParseState~warn("Unknown directive:" directive)
        end
      end
      state = "UNDETERMINED"
    end
    when state = "RXDOC" then do
      if line~right(2) = "*/" then do
        state = "UNDETERMINED"
        rxdoc = accu
        drop accu
      end
      else do
        if \ var("accu") then accu = .rxdoc~new
        if tree~doc = "" then
          tree~doc = accu
        -- "* " is needed to match only the last * char
        parse var line "* " remain
        accu~append(remain)
      end
    end
-- just ignore procedures now, this needs to be implemented
    when state = "PROCEDURE" then do
      state = "UNDETERMINED"
      self~ParseState~warn("Procedure not evaluated")
    end
    otherwise do
      self~ParseState~warn("Incorrect state reached:" state)
    end
  end
  has~something = .true
  return

  /** This procedure repeats the current parsing process for the current line
   * with a different state.
   * @arg state The new state for the current line.
   **/
  repeat: procedure expose state line i self
  use arg state
  self~parseLine(line,i)

/** A simple class to hold the parse state and to report warnings and errors.
*/
::CLASS ParseState
::METHOD Init
  expose fileName line lineNumber options
  use strict arg fileName, options = (.FileParser~defaultOptions)
  lineNumber = -1
  line = ""

::ATTRIBUTE Line
::ATTRIBUTE FileName
::ATTRIBUTE LineNumber

/** This method prints a warning with the text passed as parameter. Additionally
* it adds the file name and line number to the warning shown. If the option
* warn.as.error is set the warning will be shown as error. .stderr is used
* for printing the warning.
* @param message The warning message.
*/
::METHOD Warn
  expose fileName line lineNumber options
  if options~warn.as.error then forward message "error"
  use arg message
  .stderr~say("W" fileName":"lineNUmber message)
  if options~warn.printline then
    .stderr~say(">>>"line)

/** This method prints an error message. Additionally
* it adds the file name and line number to the error. .stderr is used
* for printing the error. Errors indicate that the source code contains some
* hard error that will prevent the file from being executed by the interpreter.
* @param message The error message.
*/
::METHOD Error
  expose fileName line lineNumber
  use arg message
  .stderr~say("E" fileName":"lineNUmber message)
  if options~error.printline then
    .stderr~say(">>>"line)

/** A base class for rexx doc strings. This is not yet finished.
*/
::CLASS RXDOC
::METHOD Init
  expose content
  use strict arg
  content = .array~new
::METHOD append
  expose content
  use strict arg what
  content~append(what)
::METHOD MakeArray
  expose content
  if var("content") then return content~copy
  else return .array~of("(No documentation available)")

/** A simple class that collects source entries belonging to a directive.
*/
::CLASS SourceContainer
::METHOD Init
  expose source
  source = .array~new
::METHOD append
  expose source
  use strict arg line
  source~append(line)
::ATTRIBUTE source Get

::CLASS Directive
::METHOD Init
  expose parser state
  use strict arg parser
  state = parser~parseState~copy
::ATTRIBUTE parser Get
::ATTRIBUTE State Get
::METHOD name Abstract
::METHOD parse Abstract
::METHOD getObject ABSTRACT

::CLASS AttributeDirective SUBCLASS Directive
::ATTRIBUTE name Get
::METHOD parse
  expose name attribute
  use strict arg line, source, rxdoc, target
  parse var line . name tokens
  attribute = .SourceAttribute~new(name, source, rxdoc, target, self~state)
  has_getOrSet = .false
  has_guarded = .false
  has_protected = .false
  has_private = .false
  has_class = .false
  do j = 3 to line~words
    modifier = line~word(j)~upper
    select
      when modifier = "GET" & has_getOrSet = .false then do
        has_getOrSet = .true
        attribute~setAccess("GET")
      end
      when modifier = "SET" & has_getOrSet = .false then do
        has_getOrSet = .true
        attribute~setAccess("SET")
      end
      when modifier = "GUARDED" & has_guarded = .false then do
        has_guarded = .true
        attribute~setGuarded
      end
      when modifier = "UNGUARDED" & has_guarded = .false then do
        has_guarded = .true
        attribute~setUnguarded
      end
      when modifier = "PROTECTED" & has_protected = .false then do
        has_protected = .true
        attribute~setProtected
      end
      when modifier = "UNPROTECTED" & has_protected = .false then do
        has_protected = .true
        attribute~setUnprotected
      end
      when modifier = "PRIVATE" & has_private = .false then do
        has_private = .true
        attribute~setPrivate
      end
      when modifier = "PUBLIC" & has_private = .false then do
        has_private = .true
        attribute~setPublic
      end
      when modifier = "CLASS" & has_class = .false then do
        has_class = .true
        attribute~setClassMethod
      end
      otherwise do
        flags = "("
        if has_guarded then
          if method~isGuarded then flags||="Guarded "
          else flags||="Unguarded "
        if has_protected then
          if method~isProtected then flags||="Protected "
          else flags||="Unptotected "
        if has_private then
          if method~isPrivate then flags||="Private "
          else flags||="Public"
        if has_class then flags||="Class"
        flags ||= ")"
        if flags = "()" then flags = ""
        self~parser~ParseState~warn("Keyword error" modifier "attribute" name flags)
      end
    end
  end
::METHOD getObject
  expose attribute
  return attribute

::CLASS ClassDirective SUBCLASS Directive
::METHOD name
  expose name
  return name
::METHOD getObject
  expose class
  return class
::METHOD parse
  expose name class
  use strict arg line, source, rxdoc, target = .nil
  name = line~word(2)
  class = .SourceClass~new(name,source,rxdoc,target, self~state)
  has_metaclass = .false
  has_mixinsubclass = .false
  has_visibility = .false
  has_inherit = .false
  has_inherit_token = .false
  tokens = line~space~makeArray(" ")
  i = 3
  do while i <= tokens~size
    token = tokens[i]~upper
    select
      when token = "METACLASS" & has_metaclass = .false & has_inherit = .false then do
        has_metaclass = .true
        i+=1
        t = tokens[i]
        if t = .nil then self~parser~parseState~error("Missing symbol after METACLASS")
        class~metaclass = t
      end
      when token = "SUBCLASS" & has_mixinsubclass = .false & has_inherit = .false then do
        has_mixinsubclass = .true
        i+=1
        t = tokens[i]
        if t = .nil then self~parser~parseState~error("Missing symbol after SUBCLASS")
        class~subclass = t
      end
      when token = "MIXINCLASS" & has_mixinsubclass = .false & has_inherit = .false then do
        has_mixinsubclass = .true
        i+=1
        t = tokens[i]
        if t = .nil then self~parser~parseState~error("Missing symbol after MIXINCLASS")
        class~mixinclass = t
      end
      when token = "PRIVATE" & has_visibility = .false & has_inherit = .false then do
        has_visibility = .true
        class~setPrivate
      end
      when token = "PUBLIC" & has_visibility = .false & has_inherit = .false then do
        has_visibility = .true
        class~setPublic
      end
      when token = "INHERIT" & has_metaclass = .false & has_inherit = .false then do
        has_inherit = .true
      end
      when has_inherit then do
        has_inherit_token = .true
        class~inherit~append(token)
      end
      otherwise self~parser~ParseState~error("Keyword error" token", class" name)
    end
    i+=1
  end
  if has_inherit & \ has_inherit_token then
    self~parser~ParseState~error("Missing symbol after INHERIT")

::CLASS ConstantDirective SUBCLASS Directive
::ATTRIBUTE name Get
::METHOD parse
  expose name sourceConstant
  use strict arg line, source, doc, target
  parse var line . name value
  if name = "" then
    self~parser~parseState("Missing name for constant")
  sourceConstant = .SourceConstant~new(name, source, doc, target, self~state)
  sourceConstant~setValue(value)
  
::METHOD getObject
  expose sourceConstant
  return sourceConstant

::CLASS MethodDirective SUBCLASS Directive
::METHOD name
  expose name
  return name
::METHOD getObject
  expose method
  return method
::METHOD parse
  expose name method
  use strict arg line, source, rxdoc, target
  name = line~word(2)
  method = .SourceMethod~new(name, source, rxdoc, target, self~state)
  has_guarded = .false
  has_protected = .false
  has_private = .false
  has_class = .false
  has_abstract = .false
  has_attribute = .false
  do j = 3 to line~words
    modifier = line~word(j)~upper
    select
      when modifier = "GUARDED" & has_guarded = .false then do
        has_guarded = .true
        method~setGuarded
      end
      when modifier = "UNGUARDED" & has_guarded = .false then do
        has_guarded = .true
        method~setUnguarded
      end
      when modifier = "PROTECTED" & has_protected = .false then do
        has_protected = .true
        method~setProtected
      end
      when modifier = "UNPROTECTED" & has_protected = .false then do
        has_protected = .true
        method~setUnprotected
      end
      when modifier = "PRIVATE" & has_private = .false then do
        has_private = .true
        method~setPrivate
      end
      when modifier = "PUBLIC" & has_private = .false then do
        has_private = .true
        method~setPublic
      end
      when modifier = "CLASS" & has_class = .false then do
        has_class = .true
        method~setClassMethod
      end
      when modifier = "ATTRIBUTE" & has_attribute = .false & has_abstract = .false then do
        has_attribute = .true
        method~setAttribute
      end
      when modifier = "ABSTRACT" & has_abstract = .false & has_attribute = .false then do
        has_abstract = .true
        method~setAbstract
      end
      otherwise do
        flags = "("
        if has_guarded then
          if method~isGuarded then flags="Guarded "
          else flags="Unguarded "
        if has_protected then
          if method~isProtected then flags||="Protected "
          else flags||="Unptotected "
        if has_private then
          if method~isPrivate then flags||="Private "
          else flags||="Public "
        if has_class then flags||="Class "
        if has_attribute then flags||="Attribute "
        if has_abstract then flags||="Abstract "
        flags ||= ")"
        if flags = "()" then flags = ""
        self~parser~ParseState~warn("Keyword error" modifier "method" name flags)
      end
    end
  end

::CLASS RequiresDirective SUBCLASS Directive
::ATTRIBUTE name Get
::METHOD getObject
  expose sourceRequires
  use strict arg
  return sourceRequires
::METHOD parse
  expose name sourceRequires
  use strict arg line, source, rxdoc, parent
  parse var line '"'name'"'
  if name = "" then parse var line . name
  sourceRequires = .SourceRequires~new(name, source, rxdoc, parent, self~parser~options, self~state)

::CLASS RoutineDirective SUBCLASS Directive
::ATTRIBUTE name Get
::METHOD getObject
  expose sourceRoutine
  use strict arg
  return sourceRoutine
::METHOD parse
  expose name sourceRoutine
  use strict arg line, source rxdoc, parent
  parse var line . name visibility
  sourceRoutine = .SourceRoutine~new(name,source,rxdoc,parent, self~state)
  visibility = visibility~strip
  if visibility \= "" then
    if visibility~caselessEquals("PRIVATE") then
      sourceRoutine~setPrivate
    else if visibility~caselessEquals("PUBLIC") then
      sourceRoutine~setPublic
    else self~parser~ParseState~error("Invalid option" visibility "on routine" name)

::CLASS Tag
::METHOD name ABSTRACT
::METHOD parse ABSTRACT
::CLASS paramTag SUBCLASS Tag
::CLASS returnTag SUBCLASS Tag
::CLASS sinceTag SUBCLASS Tag
::CLASS seeTag SUBCLASS TAg


::CLASS SourceElement INHERIT Comparable
::METHOD INIT
  expose name source doc parent state
  doc = .nil
  parent = .nil
  source = .nil
  use strict arg name source doc parent state

::ATTRIBUTE Name Get
::ATTRIBUTE Source Get
::ATTRIBUTE Doc
::ATTRIBUTE Parent Get
::ATTRIBUTE State Get

::METHOD CompareTo
  expose name
  use strict arg other
  return name~compareTo(other~name)

::METHOD resolve ABSTRACT

::CLASS SourceFile SUBCLASS SourceElement
::METHOD INIT
  expose classes methods routines packageDoc name requires
  classes = .set~new
  methods = .set~new
  routines = .set~new
  requires = .set~new
  
  if arg() = 1 then use strict arg name
  else forward class (super)

::ATTRIBUTE Classes Get
::ATTRIBUTE Methods Get
::ATTRIBUTE Routines Get
::ATTRIBUTE Requires Get

::METHOD resolve
  expose classes methods requires routines
  do require over requires
    require~resolve
  end
  do class over classes
    class~resolve
  end
  do method over methods
    method~resolve
  end
  do routine over routines
    routine~resolve
  end

/** Recursive method to find a class with a matching name
 * This method searches the whole parse tree for a matching class.
 * It tries to reassemble the class resolution code from the ooRexx
 * interpreter.
 **/
::METHOD getClass
  expose classes requires
  use strict arg name, include_private = .true, recurse = .true
  found = .false
  if name = .nil then return "NIL"
  -- search local classes
  do class over classes
    if class~name~caselessEquals(name) then do
      if class~isPublic | include_private then do
        found = .true
        leave
      end
    end
  end
  if \ found then do
    if recurse then
      do req over requires
        file = req~getFile
        if file \= .nil then
          class = file~getClass(name,.false, .false)
      end
    if self~parent \= .nil then
      class = self~parent~getClass(name,false, .false)
    else class = name
  end
--  if \ class~isinstanceOf(.string) then say class~name
--  else say "not found:"name
  return class

::CLASS SourceClass SUBCLASS SourceElement
::METHOD INIT
  expose methods attributes private inherit metaclass subclass mixinclass constants
  methods = .set~new
  attributes = .set~new
  constants = .set~new
  inherit = .list~new
  private = .true
  metaclass = .nil
  subclass = "OBJECT"
  mixinclass = .nil
  forward class (super)

::METHOD resolve
  expose methods attributes metaclass subclass mixinclass inherit
  do method over methods
    method~resolve
  end
  do attribute over attributes
    attribute~resolve
  end
  if metaclass \= .nil then
    metaclass = self~parent~getClass(metaclass)
  subclass = self~parent~getClass(subclass)
  mixinclass = self~parent~getClass(mixinclass)
  new_inherit = .list~new
  do i over inherit
    new_inherit~append(self~parent~getClass(i))
  end
  inherit = new_inherit

::ATTRIBUTE Methods Get
::ATTRIBUTE Attributes Get
::ATTRIBUTE Constants Get

::ATTRIBUTE Metaclass
::ATTRIBUTE Subclass
::ATTRIBUTE Mixinclass
::METHOD isPrivate
  expose private; return private
::METHOD setPrivate
  expose private; private = .true
::METHOD isPublic
  expose private; return \private
::METHOD setPublic
  expose private; private = .false
::ATTRIBUTE Inherit GET

-- this method must be enhanced to seach super classes
::METHOD getMethod
  expose methods
  use strict arg name
  do sm over methods
    if sm~name~caselessEquals(name) then if \ sm~isClassMethod then return sm
  end
  return .nil
-- are class methods inheritable???
::METHOD getClassMethod
  expose methods
  use strict arg name
  do sm over methods
    if sm~name~caselessEquals(name) then if \ sm~isClassMethod then return sm
  end
  return .nil

::CLASS SourceAbstractMethod SUBCLASS SourceElement
::METHOD Init
  expose guarded protected private
  guarded = .true
  protected = .false
  private = .false
  class = .false
  abstract = .false
  forward class (super)

::METHOD resolve
::METHOD isGuarded
  expose guarded; return guarded
::METHOD isUnguarded
  expose guarded; return \guarded

::METHOD setGuarded
  expose guarded; guarded = .true
::METHOD setUnguarded
  expose guarded; guarded = .false

::METHOD isProtected
  expose protected; return protected
::METHOD isUnprotect
  expose protected; return \protected
::METHOD setProtected
  expose protected; protected = .true
::METHOD setUnprotected
  expose protected; protected = .false

::METHOD isPrivate
  expose private; return private
::METHOD isPublic
  expose private; return \private
::METHOD setPrivate
  expose private; private = .true
::METHOD setPublic
  expose private; private = .false

::METHOD isClassMethod
  expoes class; return class
::METHOD setClassMethod
  expose class; class = .true

::CLASS SourceMethod SUBCLASS SourceAbstractMethod
::METHOD Init
  expose attribute
  attribute = .false
  forward class (super)

::METHOD isAttribute
  expose attribute; return attribute
::METHOD setAttribute
  expose attribute; attribute = .true

::METHOD isAbstract
  expose abstract; return abstract
::METHOD setAbstract
  expose abstract; abstract = .true

::CLASS SourceAttribute SUBCLASS SourceElement
::METHOD Init
  expose access
  access = "SET/GET"
  forward class (super)

::METHOD setAccess
  expose access; use arg access  
::METHOD getAccess
  expose access; return access
::METHOD resolve

::CLASS SourceConstant SUBCLASS SourceElement
::METHOD resolve
::METHOD setValue
  expose value; use strict arg value
::METHOD getValue
  expose value; return value

::CLASS SourceRoutine SUBCLASS SourceElement
::METHOD Init
  expose private
  private = .false
  forward class (super)
::METHOD resolve
::METHOD isPrivate
  expose private; return private
::METHOD isPublic
  expose private; return \ private
::METHOD setPublic
  expose private; private = .false
::METHOD setPrivate
  expose private; private = .true
::CLASS SourceProcedure SUBCLASS SourceElement
::METHOD resolve

::CLASS SourceRequires SUBCLASS SourceElement
::METHOD Init
  expose options sourceFile
  use strict arg name, source, rxdoc, target, options, state
  sourceFile = .nil
  forward class (super) array (name, source, rxdoc, target, state)
::METHOD resolve
  expose options sourceFile
  if options~follow.requires then do
    sourceFile = .FileParser~new(self~name,options,self~parent)~~parse~tree
    -- the fileParse does not do this automatically here
    sourceFile~resolve
  end
::METHOD getFile
  expose sourceFile
  return sourceFile
