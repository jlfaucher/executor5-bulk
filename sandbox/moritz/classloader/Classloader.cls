/*
ooRexx Class Loader

This set of classes aid programmers by offering functions to load classes
without caring about their physical location.

Written by Moritz Hoffmann. Contributed to the Open Object Rexx project.
How it works
============

The class loader rexx script adds an entry to the local directory under the
name "Package". This object is a package directory element which has to be used
to load all other packages. Each elemenet is associated directly or via its
parent element with a back end. A back end has the task to load elements from
any kind of resource, like file systems or archive files.
At some point there will be a Fileentry that represents a rexx class resource.
This resource should implement the logic necessary to make its classes
available to the calling application.
*/
-- Don't load this package several times
if .Local~Package \= .nil then return

-- enable or disable tracing
.local~CLtrace = 0

-- the path seperator
.local~PathSeperator = '/'

-- the file extension for package files
.local~FileExtension = ".rex"

.local~tar = "tar --no-wildcards-match-slash --wildcards --no-recursion -tf"
--.local~tar = "tar -tf"

.local~temp = "/tmp/"
rootBackEnd = .MergeBackEnd~new
backEnds = rootBackEnd~getBackEnds
-- a list of back ends
backEnds~append(.TarBackEnd~new("./archive.tar.gz","root"||.pathSeperator))
--backEnds~append(.FileBackEnd~new("./root"))

-- publish the root element under .package
.Local~Package=.PackageElement~new("Base",,,rootBackEnd)

-- publish the package data class
.Local~PackageData = .PackageData

/*
The PackageData class
This class is an abstract class for all packages to store their information in.
It has several methods that shall return information about the package.
*/
::CLASS PackageData

/* Returns an array of public classes this package offers.
 * @returns .array of public classes
 */
::METHOD Classes ABSTRACT

/* Returns the name
 * @returns .string name of the package
 */
::METHOD Name ABSTRACT

/* Returns the package's version. This should be a number to allow comparison
 * using arithmetic operations.
 * @returns .string version number
 */
::METHOD Version ABSTRACT

/* Returns the package's author.
 * @returns .string package author.
 */
::METHOD Author ABSTRACT

/* Returns the package's license. This should be an abbreviation for common
 * licenses like CPL, Apache, GPL or a name and a link for any other license.
 * Depending on the license a license version is needed as well.
 * @returns .string package license.
 */
::METHOD License ABSTRACT

/*
This class is a super class to all back ends. A back end has the ability to
load categories and containers.
Back ends can either load resources at the time they are loaded or they delay
it until the requested element is required.
*/
::CLASS BackEnd
/* Get an element with the supplied name. Search is done case insensitive.
 * @param name The element name to resolve.
 * @returns .Element An element for the name. In case it is not found .nil is
 * returned and user condition PackageNotFound is raised.
 */
::METHOD getElement ABSTRACT

/*
The Element class
This class defines all methods all sub classes have to implement.
*/
::CLASS Element

/* Return the public name.
 * @returns .String the name.
 */
::METHOD getName ABSTRACT

/* Return the handle of this element. This is an object to allow the backend
 * to store custom information in this element. This should not be touched
 * from outside.
 * @returns .Object Back end's custom data.
 */
::METHOD getHandle ABSTRACT

/* Return the parent element.
 * @returns .Element The parent element or .nil if there is no public element.
 */
::METHOD getParent ABSTRACT

/* Return a child element for the current element. Should raise user
 * PackageNotFound if resolution fails. This request is forwarded to the
 * back end.
 * @param .String The child element's name.
 * @returns .Element The child element.
 */
::METHOD getElement ABSTRACT

/* Return the element's back end. To avoid storing the back end several times
 * an element has to return the parent's back end unless it has its very own
 * back end.
 * @returns .BackEnd The element's back end.
 */
::METHOD getBackEnd ABSTRACT

/* The unknown method. This is used to simplify package lookup. Instead of
 * writing <code>.Package~getElement("name")~getElement("package")</code>
 * it is possible to write <code>.Package~name~package</code>.
 */
::METHOD UNKNOWN ABSTRACT

/*
A DirectoryElement represents a directory containing only sub elements.
If it has been created with an package data value this object will receive
messages that could not be satisfied by the back end. This way it is
possible to have packages that have the same file name as a directory...
*/
::CLASS PackageElement subclass Element
::METHOD Init
  expose name handle parent backEnd elements package
  use arg name, handle, parent, backEnd, package
	elements = .directory~new
::METHOD getName
  expose name
  return name
::METHOD getHandle
  expose handle
  return handle
::METHOD getParent
  expose parent
  return parent
::METHOD getElement
	expose elements
  signal on user PackageNotFound
  use arg name
  uname = name~upper~strip
	if elements[uname] \= .nil then
		return elements[uname]
	else do
		e = self~getBackEnd~getElement(name,self)
		elements[uname] = e
		return e
	end
  PackageNotFound:
  raise propagate
::METHOD getBackEnd
  expose backEnd
  if backEnd = .nil then return self~getParent~getBackEnd
  return backEnd

-- calls the back end to resolve the name
::METHOD Unknown
	expose package
  signal on user PackageNotFound
  use arg name, arguments
	name = name~strip
  return self~getElement(name)
  -- the back end could not satisfy the request, try the package
  PackageNotFound:
  if var("package") then do
    if .local~CLtrace then
      say "Forwarding" name "to" package
    forward to (package) message (name) arguments (arguments)
  end
  else
    raise propagate

/*
The .MergeBackEnd. This .BackEnd forwards requests to get an element to a
list of BackEnds. It merges several .BackEnd objects into a single object.
The order of back end calling is undefined.
In case there are duplicates it is not known which one is used.
*/
::CLASS MergeBackEnd SUBCLASS BackEnd
::METHOD Init
  expose backEnds
  use arg backEnds
  if arg() = 0 then backEnds = .list~new
::METHOD getElement
  expose backEnds
  if .local~CLtrace then
    call on user PackageNotFound
  use strict arg name, element
  -- iterate over all back ends
  do backEnd over backEnds
    -- call the active back end
    element = backEnd~getElement(name, element)
    -- found an element, then return it
    if element \= .nil then
      return element
  end
  call off user PackageNotFound
  raise user PackageNotFound additional (name) return (.nil)

  PackageNotFound:
    .stderr~lineout("BackEnd" backEnd "could not resolve" name)
    return

::METHOD getBackEnds
  expose backEnds
  return backEnds

::CLASS FileSystemBackEnd SUBCLASS BackEnd
-- This method converts a given element to a relative path.
::METHOD getRelativePath
  use strict arg element, seperator
  path = ""
  e = element
  -- iterate over the packages a until the back end's root package has
  -- been found. Construct path from back to front.
  do while e \= .nil
    if e~getBackEnd \= self then leave
    path = e~getHandle||seperator||path
    e = e~getParent
  end
  return path

::METHOD getElementBackEnd
  use strict arg element
  if element~getBackEnd \= self then do
	  return self
  end
  else do
		return .nil
  end

::CLASS FileBackEnd SUBCLASS FileSystemBackEnd
::METHOD Init
  expose path
  use arg path
  call SysFileTree path, "file", "DO"
  if file.0 > 1 then raise syntax 93
  else path = file.1
  -- add a trailing path seperator if missing
  if path~right(1) \= .PathSeperator then path||=.PathSeperator

/*
This getElement implementation searches for packages on the file system.
It first looks for directories, after that for files. Note that it is not
possible to have directories and files with the same name. The directory will
be prefered over the file.
*/
::METHOD getElement
  expose path
  use strict arg name, element
  -- don't allow names with path seperator and dots
  if name~verify("."||.PathSeperator,"M") \= 0 then
    raise syntax 93
  if .local~CLtrace then
    say "FILE Looking for" name "in" element~getName
  currentPath = path||self~getRelativePath(element,.PathSeperator)
  -- search for files
  pkg = self~searchFile(name, element, currentPath)
  if pkg \= .nil then
    return pkg
  -- search for directories
  pkg = self~searchDirectory(name, element, currentPath)
  if pkg \= .nil then
    return pkg
  raise user PackageNotFound additional (name) description ("Unable to locate element" name "in element" element~getName "'"element~getHandle"'") return (.nil)

::METHOD searchFile PRIVATE
  use strict arg name, element, path
  call SysFileTree path||"*"||.FileExtension,"files","FO"
  do i = 1 to files.0
    file = files.i
    fileShort = file~substr(1,file~length-.FileExtension~length)
    handle = fileShort~right(fileShort~length-fileShort~lastpos(.PathSeperator))
    if handle~caselessEquals(name) then do
      packageData = self~loadFile(file)
      newElement =  .PackageElement~new(name, handle, element, self~getElementBackEnd(element), packageData)
      return newElement
    end
  end
  return .nil

::METHOD searchDirectory PRIVATE
  use strict arg name, element, path
  call SysFileTree path,"files","DO"
  do i = 1 to files.0
    file = files.i
    handle = file~right(file~length - file~lastpos(.PathSeperator))
    if handle~caselessEquals(name) then do
      newElement =  .PackageElement~new(name, handle, element, self~getElementBackEnd(element))
      return newElement
    end
  end
  return .nil


::METHOD loadFile PRIVATE
  use arg fileName
  call (filename)
  package = result
  if package = "RESULT" then
    raise syntax 93.900 array -
      ("File" filename "could not be loaded (no result).")
--  say filename package
--  say package~new~author
  return result
  


::CLASS TarBackEnd SUBCLASS FileSystemBackEnd
::METHOD Init
  expose path prefix
  use strict arg path, prefix=""
  elements = .table~new
::METHOD getElement
  expose path prefix
  use arg name, element
  if name~verify("."||.PathSeperator,"M") \= 0 then
    raise syntax 93
  uname = name~translate
  if .local~CLtrace then
    say "TAR Looking for" name "in" element~getName
	currentPath = prefix||self~getRelativePath(element,.PathSeperator)
  -- search for files
  pkg = self~searchFile(name, element, currentPath)
  if pkg \= .nil then return pkg
  -- search for directories
  pkg = self~searchDirectory(name, element, currentPath)
  if pkg \= .nil then return pkg
  raise user PackageNotFound additional (name) description ("Unable to locate element" name "in element" element~getName "'"element~getHandle"'")return (.nil)

::METHOD searchFile PRIVATE
  expose path
  use strict arg name, element, currentPath
	.local~tar path '"'currentPath'*'||.FileExtension'" 2>/dev/null|rxqueue '
	do while .stdque~queued > 0
		file = .stdque~linein
    pos = file~lastPos(.PathSeperator)
    handle = file~substr(pos + 1, file~length - .FileExtension~length - pos)
    if handle~caselessEquals(name) then do
      packageData = self~loadFile(file)
      newElement =  .PackageElement~new(name, handle, element, self~getElementBackEnd(element), packageData)
      return newElement
    end
  end
  return .nil

::METHOD searchDirectory PRIVATE
  expose path
  use strict arg name, element, currentPath
	.local~tar path '"'currentPath'*" 2>/dev/null|rxqueue'
	do while .stdque~queued > 0
		file = .stdque~linein
		-- tar lists directories with trailing slash
    -- if this fails it listed a file which needs to be skipped.
		if file~right(1) \= .PathSeperator then
      iterate
		pos = file~lastpos(.PathSeperator, file~length - 1)
		-- the area between the last / and the one before it
    handle = file~substr(pos + 1, file~length - pos - 1)
    if handle~caselessEquals(name) then do
      newElement =  .PackageElement~new(name, handle, element, self~getElementBackEnd(element))
      return newElement
    end
	end
  return .nil
::METHOD loadFile PRIVATE
	expose path
  use arg fileName
/*
  command = "tar -O --ignore-case --anchored -xf" path filename "|rxqueue"
  say command
  command
  lines = .array~new
  do while .stdque~queued > 0
--    say .stdque~linein
    a = .stdque~linein
    lines[lines~items + 1] = a
  end
  say lines~makeString
  package = self~run(.method~new(filename,lines))
*/
  command = "tar -C ".temp" --ignore-case --anchored -xf" path filename "|rxqueue"
  command
  file = .temp||filename
  call (file)
	rc = SysFileDelete(file)

  package = result
  if package = "RESULT" then raise syntax 93
  return package


