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
do r over parser~tree~requires
  say r~string r~getFile~string r~getFile~name
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
  expose file data tree has options parseState node
  use strict arg file, options = (self~class~defaultOptions), parent = .nil
  tree = .SourceFile~new(file, "", "", parent)
  parseState = .ParseState~new(file,options)
  has = .directory~new
  node = .nil

::METHOD Parse
  expose file data state options tree
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
      prefix = line~left(line~length-1)||" "
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
  -- start resolving missing references only when this is the root parser
  -- this method is propagated through the whole tree
  if tree~parent = .nil then tree~resolve

::ATTRIBUTE Tree Get
::ATTRIBUTE ParseState Get

::METHOD getResetRxDoc
  expose rxdoc
  r = rxdoc
  -- In future rxdoc will be a special object...
  rxdoc = "(No documentation)"
  return r

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
      else if line~caselessPos(": procedure") > 0 then do
        parse upper var line name": PROCEDURE"
        if 0 = name~verify("ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890") then
          repeat("PROCEDURE")
      end
    end
    when state = "DIRECTIVE" then do
      line = line~right(line~length - 2)
      directive = line~word(1)~upper
      parameters = line~delword(1)
      select
        -- parse routines
        when directive = "ROUTINE" then do
          rd = .RoutineDirective~new(self,options)~~parse(line,self~getResetRxDoc, tree)
          tree~routines~put(rd~getObject)
          node = .nil
        end
        when directive = "REQUIRES"  & has~ClassMethod = .nil then do
          rd = .RequiresDirective~new(self,options)~~parse(line,self~getResetRxDoc, tree)
          tree~requires~put(rd~getObject)
        end
        -- parse ::CLASS
        when directive = "CLASS" then do
          has~ClassMethod = .true
          cp = .ClassDirective~new(self)~~parse(line,self~getResetRxDoc,tree)
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
          mp = .MethodDirective~new(self)~~parse(line,self~getResetRxDoc,target)
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
          ad = .AttributeDirective~new(self, tree)~~parse(line,self~getResetRxDoc,target)
          attribute = ad~getObject
          target~methods~put(attribute)
        end
        -- parse ::CONSTANT
        when directive = "CONSTANT" then do
          if node = .nil then self~ParseState~error("Constant without class")
          else do
            cd = .ConstantDirective~new(self)~~parse(line,self~getResetRxDoc,node)
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
        if tree~doc = .nil then
          tree~doc = rxdoc
        if \ var("accu") then accu = .array~new
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
::METHOD getObject ABSTRACT

::CLASS AttributeDirective SUBCLASS Directive
::METHOD Init
  expose parser tree
  use strict arg parser, tree
::ATTRIBUTE name Get
::METHOD parse
  expose parser tree name attribute
  use strict arg line, rxdoc, target
  parse var line . name tokens
  attribute = .SourceAttribute~new(name, "", rxdoc, target)
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
        parser~ParseState~warn("Keyword error" modifier "attribute" name flags)
      end
    end
  end
::METHOD getObject
  expose attribute
  return attribute

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
        if t = .nil then parser~parseState~error("Missing symbol after METACLASS")
        class~metaclass = t
      end
      when token = "SUBCLASS" & has_mixinsubclass = .false & has_inherit = .false then do
        has_mixinsubclass = .true
        i+=1
        t = tokens[i]
        if t = .nil then parser~parseState~error("Missing symbol after SUBCLASS")
        class~subclass = t
      end
      when token = "MIXINCLASS" & has_mixinsubclass = .false & has_inherit = .false then do
        has_mixinsubclass = .true
        i+=1
        t = tokens[i]
        if t = .nil then parser~parseState~error("Missing symbol after MIXINCLASS")
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
      otherwise parser~ParseState~error("Keyword error" token", class" name)
    end
    i+=1
  end
  if has_inherit & \ has_inherit_token then
    parser~ParseState~error("Missing symbol after INHERIT")

::CLASS ConstantDirective SUBCLASS Directive
::METHOD Init
  expose parser
  use strict arg parser
::ATTRIBUTE name Get
::METHOD parse
  expose parser name sourceConstant
  use strict arg line, doc, target
  parse var line . name value
  if name = "" then
    parser~parseState("Missing name for constant")
  sourceConstant = .SourceConstant~new(name, "", doc, target)
  sourceConstant~setValue(value)
  
::METHOD getObject
  expose sourceConstant
  return sourceConstant

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
  method = .SourceMethod~new(name, "", rxdoc, target)
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
        parser~ParseState~warn("Keyword error" modifier "method" name flags)
      end
    end
  end

::CLASS RequiresDirective SUBCLASS Directive
::METHOD Init
  expose parser options
  use strict arg parser, options
::ATTRIBUTE name Get
::METHOD getObject
  expose sourceRequires
  use strict arg
  return sourceRequires
::METHOD parse
  expose parser name sourceRequires options
  use strict arg line, rxdoc, parent
  parse var line '"'name'"'
  if name = "" then parse var line . name
  sourceRequires = .SourceRequires~new(name, rxdoc, parent, options)

::CLASS RoutineDirective SUBCLASS Directive
::METHOD Init
  expose parser options
  use strict arg parser, options
::ATTRIBUTE name Get
::METHOD getObject
  expose sourceRoutine
  use strict arg
  return sourceRoutine
::METHOD parse
  expose parser name sourceRoutine options
  use strict arg line, rxdoc, parent
  parse var line . name visibility
  sourceRoutine = .SourceRoutine~new(name,"",rxdoc,parent)
  if visibility \= "" then
    if visibility~caselessEquals("PRIVATE") then
      sourceRoutine~setPrivate
    else if visibility~caselessEquals("PUBLIC") then
      sourceRoutine~setPublic
    else parser~ParseState~error("Invalid option" visibility "on routine" name)

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
::ATTRIBUTE attributes Get
::ATTRIBUTE constants Get

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
  expose access; use arg access
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
  use strict arg name,rxdoc, target, options
  sourceFile = .nil
  forward class (super) array (name, "", rxdoc, target)
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