/** <h1>ooRexx Class Loader</h1>
*
* This set of classes aid programmers by offering functions to load classes
* without caring about their physical location.
*
* Written by Moritz Hoffmann. Contributed to the Open Object Rexx project.
* 
* <h2>How it works</h2>
*
* The class loader rexx script adds an entry to the local directory under the
* name "Package". This object is a package directory element which has to be used
* to load all other packages. Each elemenet is associated directly or via its
* parent element with a back end. A back end has the task to load elements from
* any kind of resource, like file systems or archive files.
* At some point there will be a Fileentry that represents a rexx class resource.
* This resource should implement the logic necessary to make its classes
* available to the calling application.
*/
-- Don't load this package several times
if .Local~Package \= .nil then return

-- enable or disable tracing
.local~CLtrace = 0

-- the path seperator - select the correct one for your OS
.local~PathSeperator = '\'
--.local~PathSeperator = '/'

-- the file extension for package files
.local~FileExtension = ".rex"
-- not used atm
.local~errRedirect = "2>/dev/null"
-- This is only required to extract temprary files - will be removed
.local~temp = "/tmp/"

rootBackEnd = .MergeBackEnd~new
backEnds = rootBackEnd~getBackEnds
-- a list of back ends
--backEnds~append(.TarBackEnd~new("archive.tar.gz","root"||.pathSeperator))
backEnds~append(.ZipBackEnd~new("archive.zip","root"||.pathSeperator))
--backEnds~append(.FileBackEnd~new("root"))

-- publish the root element under .package
.Local~Package=.PackageElement~new("Base",,,rootBackEnd)

-- publish the package data class
.Local~PackageData = .PackageData

/** This class is an abstract class for all packages to store their information in.
* It has several methods that shall return information about the package.
*/
::CLASS PackageData

/** Returns an array of public classes this package offers.
 * @returns .array of public classes
 */
::METHOD Classes ABSTRACT

/** Returns the name
 * @returns .string name of the package
 */
::METHOD Name ABSTRACT

/** Returns the package's version. This should be a number to allow comparison
 * using arithmetic operations.
 * @returns .string version number
 */
::METHOD Version ABSTRACT

/** Returns the package's author.
 * @returns .string package author.
 */
::METHOD Author ABSTRACT

/** Returns the package's license. This should be an abbreviation for common
 * licenses like CPL, Apache, GPL or a name and a link for any other license.
 * Depending on the license a license version is needed as well.
 * @returns .string package license.
 */
::METHOD License ABSTRACT

/** This class is a super class to all back ends. A back end has the ability to
* load categories and containers.
* Back ends can either load resources at the time they are loaded or they delay
* it until the requested element is required.
*/
::CLASS BackEnd

/** Get an element with the supplied name. Search is done case insensitive.
 * @param name The element name to resolve.
 * @returns .Element An element for the name. In case it is not found .nil is
 * returned and user condition PackageNotFound is raised.
 */
::METHOD getElement ABSTRACT

/** This class defines all methods all sub classes have to implement.
*/
::CLASS Element

/** Return the public name.
 * @returns .String the name.
 */
::METHOD getName ABSTRACT

/** Return the handle of this element. This is an object to allow the backend
 * to store custom information in this element. This should not be touched
 * from outside.
 * @returns .Object Back end's custom data.
 */
::METHOD getHandle ABSTRACT

/** Return the parent element.
 * @returns .Element The parent element or .nil if there is no public element.
 */
::METHOD getParent ABSTRACT

/** Return a child element for the current element. Should raise user
 * PackageNotFound if resolution fails. This request is forwarded to the
 * back end.
 * @param .String The child element's name.
 * @returns .Element The child element.
 */
::METHOD getElement ABSTRACT

/** Return the element's back end. To avoid storing the back end several times
 * an element has to return the parent's back end unless it has its very own
 * back end.
 * @returns .BackEnd The element's back end.
 */
::METHOD getBackEnd ABSTRACT

/** The unknown method. This is used to simplify package lookup. Instead of
 * writing <code>.Package~getElement("name")~getElement("package")</code>
 * it is possible to write <code>.Package~name~package</code>.
 */
::METHOD UNKNOWN ABSTRACT

/** A DirectoryElement represents a directory containing only sub elements.
* If it has been created with an package data value this object will receive
* messages that could not be satisfied by the back end. This way it is
* possible to have packages that have the same file name as a directory...
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
  use arg name
  -- Check if this name was supplied previously.
  uname = name~upper~strip
  if elements[uname] \= .nil then
    if elements~uname = 1 then
      raise user PackageNotFound additional (name) return (.nil)
    else
      return elements[uname]
  else do
    signal on user PackageNotFound
    e = self~getBackEnd~getElement(name,self)
    elements[uname] = e
    return e
  end
  PackageNotFound:
  elements~uname = 1
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

/** This back end forwards requests to get an element to a
* list of BackEnds. It merges several .BackEnd objects into a single object.
* The order of back end calling is undefined.
* In case there are duplicates it is not known which one is used.
*/
::CLASS MergeBackEnd SUBCLASS BackEnd
::METHOD Init
  expose backEnds
  use strict arg backEnds=(.list~new)

::METHOD getElement
  expose backEnds
  use strict arg name, element
  -- only activate trapping of PackageNotFound if debug messages are wanted
  if .local~CLtrace then
    call on user PackageNotFound
  -- iterate over all back ends
  do backEnd over backEnds
    -- call the active back end
    element = backEnd~getElement(name, element)
    -- found an element, then return it
    if element \= .nil then
      return element
  end
  -- disable trapping of PackageNotFound
  call off user PackageNotFound
  -- raise a condition
  raise user PackageNotFound additional (name) return (.nil)

  PackageNotFound:
    .stderr~lineout("BackEnd" backEnd "could not resolve" name)
    return

/** This method exposes the list of back ends this object merges.
* The data type is .List per default, it can be any object supporting the
* MakeArray method to return an array of back ends. A List is used because
* it allows ordering of elements.
*/
::METHOD getBackEnds
  expose backEnds
  return backEnds

/** This back end is a super class to all back ends that operate on files.
 * It offers some common methods to keep implementations small.
 */
::CLASS FileSystemBackEnd SUBCLASS BackEnd

/** This is the main entry point for back ends to query an element. It
 * builds up a path for the current resource and calls the methods
 * searchFile and searchDirectory to resolve the supplied name. The prefix
 * is put in front of the generated path.
 * This method should only be called by sub classes.
 * @param name The name of the element that is searched.
 * @param element The parent element.
 * @param prefix The prefix to prepend to the generated path.
 * @returns .Elememt The created element or .nil.
 * @raises user PackageNotFound when the package could not be found. Returns
 * .nil as well.
 */
::METHOD getElement PRIVATE
  use strict arg name, element, prefix
  -- Print some debugging information if wanted
  if .local~CLtrace then
    say self~class~id "Looking for" name "in" element~getName
  -- generate the path for the current situation
  currentPath = prefix||self~getRelativePath(element,.PathSeperator)
  -- search for files
  pkg = self~searchFile(name, element, currentPath)
  if pkg \= .nil then
    return pkg
  -- search for directories
  pkg = self~searchDirectory(name, element, currentPath)
  if pkg \= .nil then
    return pkg
  -- only get here if nothing was found
  raise user PackageNotFound additional (name) description ("Unable to locate element" name "in element" element~getName "'"element~getHandle"'") return (.nil)

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

/** This method returns the backend that new elements have to use. This is
 * either self if the parent element has a different back end or .nil if
 * the current element inherits the back end from its super class.
 * @param element Parent element.
 * @returns self or .nil.
 */
::METHOD getElementBackEnd
  use strict arg element
  if element~getBackEnd \= self then do
	  return self
  end
  else do
		return .nil
  end

::METHOD searchFile ABSTRACT PRIVATE

::METHOD searchDirectory ABSTRACT PRIVATE


/** A back end that maps to a directory on the file system. It directly loads
* the required files.
*/
::CLASS FileBackEnd SUBCLASS FileSystemBackEnd

::METHOD Init
  expose path
  use arg path
  -- Check if directory exists
  if \ SysIsFileDirectory(path) then
    raise syntax 93.900 array ("Unable to open directory" path)
  else do
    -- check if directory is readable
    old_dir = directory()
    completePath =  directory(path)
    if completePath = "" then 
      raise syntax 93.900 array ("Unable to read from" path)
    -- reset current path to previous value
    call directory old_dir
    path = completePath
  end
  -- add a trailing path seperator if missing
  if path~right(1) \= .PathSeperator then
    path ||= .PathSeperator

/** This getElement implementation searches for packages on the file system.
* It first looks for directories, after that for files. Note that it is not
* possible to have directories and files with the same name. The directory will
* be prefered over the file.
*/
::METHOD getElement
  expose path
  use strict arg name, element
  signal on user PackageNotFound
  return self~getElement:super(name, element, path)
  PackageNotFound: raise propagate

::METHOD searchFile PRIVATE
  use strict arg name, element, path
  call SysFileTree path||"*"||.FileExtension,"files","FO"
  do i = 1 to files.0
    file = files.i
    fileShort = file~substr(1,file~length-.FileExtension~length)
    handle = fileShort~right(fileShort~length-fileShort~lastpos(.PathSeperator))
    if handle~caselessEquals(name) then do
      packageData = self~loadFile(file)
      return .PackageElement~new(name, handle, element, self~getElementBackEnd(element), packageData)
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
      return .PackageElement~new(name, handle, element, self~getElementBackEnd(element))
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
  return result
 
/** This class provides common mechanisms for back ends based on archive
 * files. As most archivers return their output in a similar format only
 * a single parsing method is needed. At the moment the zip and tar back
 * end share most of their code. Only the interface to their application
 * is different.
 */
::CLASS ArchiveBackEnd SUBCLASS FileSystemBackEnd

::METHOD Init
  expose archive prefix
  use strict arg archive, prefix=""
  -- test if file exists
  if \ SysIsFile(archive) then
    raise syntax 93.900 array ("Unable to load file" archive)

::METHOD getElement
  expose prefix
  use arg name, element
  signal on user PackageNotFound
  return self~getElement:super(name, element, prefix)
  PackageNotFound: raise propagate

::METHOD getArchive
  expose archive
  return archive

::METHOD getPrefix
  expose prefix
  return prefix

::METHOD callArchiver ABSTRACT PRIVATE
::METHOD loadFile ABSTRACT PRIVATE

::METHOD searchFile PRIVATE
  use strict arg name, element, currentPath
  self~callArchiver(currentPath||"*"||.FileExtension)
  currentPath = currentPath~translate("/",.PathSeperator)
  do while .stdque~queued > 0
    file = .stdque~linein
    if file~caselessPos(currentPath) = 1 then do
      len = currentPath~length
      handle = file~substr(len + 1, file~length - len - .FileExtension~length)
      if handle~caselessEquals(name) then do
        packageData = self~loadFile(file)
        backEnd = self~getElementBackEnd(element)
        return .PackageElement~new(name, handle, element, backEnd, packageData)
      end
    end
    else Say "!!! unexpected return value from zip:" file
  end
  return .nil

::METHOD searchDirectory PRIVATE
  use strict arg name, element, currentPath
  self~callArchiver(currentPath||"*")
  currentPath = currentPath~translate("/",.PathSeperator)
  do while .stdque~queued > 0
    file = .stdque~linein
    if file~caselessPos(currentPath) = 1 then do
      handle = file~right(file~length - currentPath~length)
      pos = handle~caselessPos(.PathSeperator)
      if pos > 1 then do
        handle = handle~left(pos-1)
        if handle~caselessEquals(name) then 
          return .PackageElement~new(name, handle, element, self~getElementBackEnd(element))
      end
    end
  end
  return .nil

/** A back end implementing the archive back end to access tar archives. These
* may be compressed.
*/
::CLASS TarBackEnd SUBCLASS ArchiveBackEnd

::METHOD callArchiver PRIVATE
  use strict arg path
  c = "tar --wildcards -tf" self~getArchive '"'path'"|rxqueue'
  c

::METHOD loadFile PRIVATE
  use strict arg fileName
  command = "tar -C ".temp" --ignore-case --anchored -xf" self~getArchive filename "|rxqueue"
  command "|rxqueue"
  lines = .array~new
  do while .stdque~queued > 0
    lines[lines~items + 1] = .stdque~linein
  end
  package = self~run(.method~new(filename,lines))
  return package

/** A back end to access files stored in zip files.
*/

::CLASS ZipBackEnd SUBCLASS ArchiveBackEnd

::METHOD callArchiver PRIVATE
  use strict arg path
  "unzip -Z -1" self~getArchive '"'path'"|rxqueue'

::METHOD loadFile PRIVATE
  use strict arg fileName
  command = "unzip -qq -c" self~getArchive filename
  command "|rxqueue"
  lines = .array~new
  do while .stdque~queued > 0
    lines[lines~items + 1] = .stdque~linein
  end
  package = self~run(.method~new(filename,lines))
  return package
