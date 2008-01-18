/* Rexx source code documentation system */
/**
* This is a set of classes to parse ooRexx source code and to extract
* information from it to create source code documentation. It parses
* all directives and collects comments belonging to them.
* @todo Add a RXDOC class, composite pattern
**/
use strict arg file
parser = .FileParser~new(file)
parser~parse
say parser~tree
classes = parser~tree~classes
do o over classes
  say o
  say classes[o]~doc~makestring
  methods = classes[o]~methods
  do p over methods
    say "  "p
    say methods[p]~doc
  end
end

::CLASS FileParser
::METHOD defaultOptions CLASS
  expose defaultOptions
  if var("defaultOptions") then return defaultOptions

  -- intialize the singleton variable with its default values
  defaultOptions = .directory~new
  defaultOptions["FOLLOW.REQUIRES"] = .true
  defaultOptions["EVALUATE.CONTINUATION"] = .true
  defaultOptions["WARN.AS.ERROR"] = .false

  return defaultOptions

::METHOD Init
  expose file data tree has options parseState
  use strict arg file, options = (self~class~defaultOptions), parent = .nil
  tree = .SourceFile~new(file, "", "", parent)
  parseState = .ParseState~new(file,options)
  has = .directory~new

::METHOD Parse
  expose file data state options
  buffer = .array~new
  inDoc = .false
  expectDefinition = .false
  fileStream = .stream~new(file)
  i = 0
  append = .false
  prefix = ""
  do while fileStream~lines > 0
    i+=1
    line = prefix||fileStream~lineIn~strip
    -- handle continuation chars
    lastChar = line~right(1)
    -- check options if - and , at the end of line are to be evaluated
    if options~evaluate.continuation then
    -- only evaluate line continuation outside rxdoc sections
    if state != "RXDOC" & lastChar = "," | lastChar = "-" then do 
      append = .true
      -- cut the last character
      prefix = line~left(line~length-1)
    end
    else if append then do 
      append = .false
      prefix = ""
    end

    if \ append then do
--      say line
      self~ParseState~lineNumber = i
      self~ParseState~line = line
      self~parseLine(line,i)
    end
  end
::ATTRIBUTE Tree Get
::ATTRIBUTE ParseState Get

::METHOD resetRxDoc
  expose rxdoc
  -- In future rxdoc will be a special object...
  rxdoc = "(No documentation)"

::METHOD parseLine PRIVATE
  expose tree node state nextstate has accu rxdoc options
  use arg line
  if \ var("state") then state = "UNDETERMINED"
-- say state
  select
    -- the initial state, everything can follow
    -- to avoid parsing wrong files there is a "has" directory that contains
    -- all points that have been found so far.
    when state = "UNDETERMINED" then do
      if line~left(2) = "::" then repeat("DIRECTIVE")
      else if line~left(3) = "/**" then repeat("RXDOC")
      else if line~caselessPos("procedure") > 0 then repeat("PROCEDURE")
    end
    when state = "DIRECTIVE" then do
      has~directive = .true
      line = line~right(line~length - 2)
      directive = line~word(1)~upper
      parameters = line~delword(1)
      select
        when directive = "REQUIRES"  & has~ClassMethod = .nil then do
          rd = .RequiresDirective~new(self,options)~~parse(line,rxdoc)
          tree~requires~put(rd~getObject)
        end
        -- parse ::CLASS
        when directive = "CLASS" then do
          has~ClassMethod = .true
          cp = .ClassDirective~new(self)~~parse(line,rxdoc)
          self~resetRxDoc
          node = cp~getObject
          tree~classes~put(node)
        end
        -- parse ::METHOD
        when directive = "METHOD" then do
          has~ClassMethod = .true
          -- Is this a floating method or does it belong to some class?
          if var("node") then
            target = node
          else
            target = tree
          mp = .MethodDirective~new(self)~~parse(line,rxdoc,target)
          method = mp~getObject
          target~methods~put(method)
          self~resetRxDoc
        end
        -- parse ::ATTRIBUTE
        when directive = "ATTRIBUTE" then do
          name = line~word(2)
          node~attributes~put(.SourceAttribute~new(name, "", rxdoc, node))
          self~resetRxDoc
        end
        -- parse ::CONSTANT
        when directive = "CONSTANT" then do
          name = line~word(2)
          node~attributes~put(.SourceConstant~new(name, "", rxdoc, node))
          self~resetRxDoc
        end
        otherwise do
          self~ParseState~warn("Incorrect directive:" directive)
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
        if tree~doc = .nil then
          tree~doc = rxdoc
        if \ var("accu") then accu = .array~new
        -- "* " is needed to match only the last * char
        parse var line "* " remain
        accu~append(remain)
      end
    end
-- just ignore procedures now, this needs to be implemented
    when state = "PROCEDURE" then state = "UNDETERMINED"
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

::METHOD parseClass PRIVATE
  use arg line buffer
  name = line~word(2)
  return .SourceClass~new(name, "", buffer, "")

::CLASS ParseState
::METHOD Init
  expose fileName line lineNumber options
  use strict arg fileName, options = (.FileParser~defaultOptions)
  lineNumber = -1
  line = ""

::ATTRIBUTE Line
::ATTRIBUTE FileName
::ATTRIBUTE LineNumber

::METHOD Warn
  expose fileName line lineNumber options
  if options~warn.as.error then forward message "error"
  use arg message
  .stderr~say("W" fileName":"lineNUmber message)
::METHOD Error
  expose fileName line lineNumber
  use arg message
  .stderr~say("E" fileName":"lineNUmber message)


::CLASS Directive
::METHOD name Abstract
::METHOD parse Abstract
::METHOD getObject

::CLASS AttributeDirective SUBCLASS Directive
::CLASS ClassDirective SUBCLASS Directive
::METHOD Init
  expose parser
  use strict arg parser
::METHOD name
  expose name
  return name
::METHOD getObject
  expose class
  return class
::METHOD parse
  expose name class parser
  use strict arg line, rxdoc, target = .nil
  name = line~word(2)
  class = .sourceclass~new(name,"",rxdoc,target)
  has_metaclass = .false
  has_mixinsubclass = .false
  has_visibility = .false
  has_inherit = .false
  tokens = line~makeArray(" ")
  i = 3
  do while i <= tokens~size
    token = tokens[i]~upper
    select
      when token = "METACLASS" & has_metaclass = .false & has_inherit = .false then do
        has_metaclass = .true
        i+=1
        class~metaclass = parser~tree~getClass(tokens[i])
      end
      when token = "SUBCLASS" & has_mixinsubclass = .false & has_inherit = .false then do
        has_mixinsubclass = .true
        i+=1
        class~subclass = parser~tree~getClass(tokens[i])
      end
      when token = "MIXINCLASS" & has_mixinsubclass = .false & has_inherit = .false then do
        has_mixinsubclass = .true
        i+=1
        class~mixinclass = parser~tree~getClass(tokens[i])
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
        class~inherit~append(parser~tree~getClass(token))
      end
      otherwise parser~ParseState~error("Keyword error" token", class" name)
    end
    i+=1
  end
parser~ParseState~error(line)

::CLASS MethodDirective SUBCLASS Directive
::METHOD Init
  expose parser
  use strict arg parser
::METHOD name
  expose name
  return name
::METHOD getObject
  expose method
  return method
::METHOD parse
  expose name method parser
  use strict arg line, rxdoc, target
  name = line~word(2)
  method = .sourcemethod~new(name, "", rxdoc, target)
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
        parser~ParseState~warn("Keyword error" modifier "("j-2") method" name "("flags~strip")")
      end
    end
  end

::CLASS RequiresDirective SUBCLASS Directive
::METHOD Init
  expose parser options
  use strict arg parser, options
::ATTRIBUTE name Get
::METHOD getObject
  expose sourceFile
  return sourceFile
::METHOD parse
  expose parser name sourceFile options
  use arg line, rxdoc, parent
  parse var line '"'name'"'
  if name = "" then parse var line . name
  if options~follow.requires then do
    sourceFile = .FileParser~new(name,options,parser~tree)~~parse~tree
  end
  else sourceFile = .nil
::CLASS RoutineDirective SUBCLASS Directive

::CLASS Tag
::METHOD name ABSTRACT
::METHOD parse ABSTRACT
::CLASS paramTag SUBCLASS Tag
::CLASS returnTag SUBCLASS Tag
::CLASS sinceTag SUBCLASS Tag
::CLASS seeTag SUBCLASS TAg


::CLASS SourceElement
::METHOD INIT
  expose name source doc parent
  doc = .nil
  parent = .nil
  source = .nil
  use strict arg name source doc parent

::ATTRIBUTE Name Get
::ATTRIBUTE Source Get
::ATTRIBUTE Doc
::ATTRIBUTE Parent Get

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

/** Recursive method to find a class with a matching name
 * This method searches the whole parse tree for a matching class.
 * It tries to reassemble the class resolution code from the ooRexx
 * interpreter.
 **/
::METHOD getClass
  expose classes requires
  use strict arg name, include_private = .true, recurse = .true
  found = .false
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
    class = .nil
    if recurse then
      do req over requires
        class = req~getClass(name,.false, .false)
      end
    if self~parent \= .nil then
      class = self~parent~getClass(name,false, .false)
    else class = .nil
  end
  if class \= .nil then say class~name
  else say "not found:"name
  return class

::CLASS SourceClass SUBCLASS SourceElement
::METHOD INIT
  expose methods attributes private inherit metaclass subclass mixinclass
  methods = .set~new
  attributes = .set~new
  inherit = .list~new
  private = .true
  metaclass = .nil
  subclass = .object
  mixinclass = .nil
  forward class (super)

::ATTRIBUTE Methods Get
::ATTRIBUTE attributes Get

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

::CLASS SourceMethod SUBCLASS SourceElement
::METHOD Init
  expose guarded protected private
  guarded = .true
  protected = .false
  private = .false
  class = .false
  attribute = .false
  abstract = .false
  forward class (super)

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

::METHOD isAttribute
  expose attribute; return attribute
::METHOD setAttribute
  expose attribute; attribute = .true

::METHOD isAbstract
  expose abstract; return abstract
::METHOD setAbstract
  expose abstract; abstract = .true

::CLASS SourceAttribute SUBCLASS SourceElement
::CLASS SourceConstant SUBCLASS SourceElement
::CLASS SourceRoutine SUBCLASS SourceElement
::CLASS SourceProcedure SUBCLASS SourceElement
