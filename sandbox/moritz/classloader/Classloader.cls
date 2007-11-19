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
if .Environment~Package \= .nil then return


.local~CLtrace = 0
-- the path seperator
.local~PathSeperator = '/'
-- the file extension for package files
.local~FileExtension = ".rex"

.local~tar = "tar --no-wildcards-match-slash --wildcards --no-recursion -tf" 
.local~tarPrefix = "root"||.pathSeperator

.local~temp = "/tmp/"

.local~initExtension = ".init"
-- a lis of back ends
backEnds = .set~new
--backEnds~put(.TarBackEnd~new("/home/moritz/rexx/classloader/archive.tar.gz"))
backEnds~put(.FileBackEnd~new("./root/"))
--backEnds~put(.FileBackEnd~new("/home/moritz/rexx/classloader/root/"))
--backEnds~put(.FileBackEnd~new("/home/moritz/tmp/classloader/root/"))

-- publish the root element under .package
.Environment~Package=.PackageElement~new("Base",,,.MergeBackEnd~new(backEnds))


.Environment~PackageData = .PackageData

/*
This class is a super class to all back ends. A back end has the ability to
load categories and containers.
Back ends can either load resources at the mie they are loaded or they delay
it until the requested element is required.
*/
::CLASS BackEnd
::METHOD getElement ABSTRACT

/*
The Element class
This class defines all methods all sub classes have to implement.
*/
::CLASS Element
-- Return the rexx name
::METHOD getName ABSTRACT
-- return the object supplied at creation time by the back end
::METHOD getHandle ABSTRACT
-- a link to the parent element
::METHOD getParent ABSTRACT
-- resolve a sub element, forwarded to the back end
::METHOD getElement ABSTRACT
-- get the back end for this element
::METHOD getBackEnd ABSTRACT

/*
A FileElement refers to a package that contains rexx code.
*/
::CLASS FileElement subclass Element
::METHOD Init
  expose name handle parent backEnd package  
  use arg name, handle, parent, backEnd, package
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
  raise user badmethod description "File elements do not have sub elements"
::METHOD getBackEnd
  expose backEnd
  if backEnd = .nil then return self~getParent~getBackEnd
  return backEnd


-- all messages not implemented by this class are forwarded to the package object
::METHOD Unknown
  expose package
  use arg method, args
  forward to (package)  message (method) arguments (args)

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
  signal on user PackageNotFound name PackageNotFound
  use arg name
	if elements~hasindex(name) then
		return elements[name]
	else do
		e = self~getBackEnd~getElement(name,self)
		elements[name] = e
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
	name = name~strip~translate
  return self~getElement(name)

  -- the back end could not satisfy the request, try the package
  PackageNotFound:
  if .local~CLtrace then
    say "Forwarding" name "to" package
  forward to (package) message (name) arguments (arguments)

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
::METHOD getElement
  expose backEnds
  signal on user PackageNotFound name PackageNotFound
  use arg name element
  --raise packagenotfound
  -- iterate over all back ends
  do backEnd over backEnds
    -- call the active back end
    element = backEnd~getElement(name, element)
    -- found an element, then return it
    if element \= .nil then
      return element
  end
  raise user PackageNotFound additional (name)

  PackageNotFound:
  raise propagate

::CLASS FileBackEnd SUBCLASS BackEnd
::METHOD Init
  expose path
  use arg path

/*
This getElement implementation searches for packages on the file system.
It first looks for directories, after that for files. Note that it is not
possible to have directories and files with the same name. The directory will
be prefered over the file.
*/
::METHOD getElement
  expose path
  use arg name, element
  -- don't allow names with path seperator and dots
  if name~verify("."||.PathSeperator,"M") \= 0 then
    raise syntax 93
  uname = name~translate
  if .local~CLtrace then
    say "Looking for" name "in" element~getName
  -- the relative path to the name that's being searched for
  currentPath = ""
  e = element
  -- iterate over the packages a until the back end's root package has
  -- been found. Construct currentPath
  do while e \= .nil
    if e~getBackEnd \= self then leave
    currentPath = e~getHandle||.PathSeperator||currentPath
    e = e~getParent
  end
  -- convert to absolute path
  currentPath = path||currentPath
  -- search for files
  call SysFileTree currentPath||"*"||.FileExtension,"files","FO"
  do i = 1 to files.0
    file = files.i
    fileShort = file~substr(1,file~length-.FileExtension~length)
    handle = fileShort~right(fileShort~length-fileShort~lastpos(.PathSeperator))
    if handle~translate = uname then do
      -- parse the path
      if element~getBackEnd \= self then e = self
      else e = .nil
      packageData = self~loadFile(file)
      newElement =  .PackageElement~new(name, handle, element, e, packageData)
      return newElement
    end
  end
  -- search for directories
  call SysFileTree currentPath,"files","DO"
  do i = 1 to files.0
    file = files.i
    handle = file~right(file~length - file~lastpos(.PathSeperator))
    if handle~translate = uname then do
      if element~getBackEnd \= self then e = self
      else e = .nil
      newElement =  .PackageElement~new(name, handle, element, e)
      return newElement
    end
  end
  raise user PackageNotFound additional (name) description ("Unable to locate element" name "in element" element~getName "'"element~getHandle"'") return (.nil)


::METHOD loadFile PRIVATE
  use arg fileName
  call (filename)
  package = result
  if package = "RESULT" then raise syntax 93
--  say filename package
--  say package~new~author
  return result
  


::CLASS TarBackEnd SUBCLASS MergeBackEnd
::METHOD Init
  expose path
  use arg path
  elements = .table~new
::METHOD getElement
  expose path
  use arg name, element
  if name~verify("."||.PathSeperator,"M") \= 0 then
    raise syntax 93
  uname = name~translate
  if .local~CLtrace then
    say name element
  currentPath = ""
  e = element
  do while e \= .nil
    if e~getBackEnd \= self then leave
    currentPath = e~getHandle||.PathSeperator||currentPath
    e = e~getParent
  end
	currentPath = .local~tarPrefix||currentPath
  -- search for directories
	.local~tar path '"'currentPath'*"|rxqueue'
	do while .stdque~queued > 0
		file = .stdque~linein
		-- tar lists directories with trailing slash
		if file~right(1) \= .PathSeperator then iterate
		p = file~lastpos(.PathSeperator, file~length - 1)
		-- the area between the last / and the one before it
    handle = file~substr(p+1,file~length - p - 1)
    if handle~translate = uname then do
      if element~getBackEnd \= self then
				e = self
      else
				e = .nil
      newElement =  .PackageElement~new(name, handle, element, e)
      return newElement
    end
	end
  -- search for files
	.local~tar path '"'currentPath'*'||.FileExtension'"|rxqueue'
	do while .stdque~queued > 0
		file = .stdque~linein
    fileShort = file~substr(1,file~length-.FileExtension~length)
    handle = fileShort~right(fileShort~length-fileShort~lastpos(.PathSeperator))
    if handle~translate = uname then do
      -- parse the path
      if element~getBackEnd \= self then
				e = self
      else
				e = .nil
      packageData = self~loadFile(file)
      newElement =  .PackageElement~new(name, handle, element, e, packageData)
      return newElement
    end
  end
  raise user PackageNotFound additional (name) description ("Unable to locate element" name "in element" element~getName "'"element~getHandle"'")return (.nil)


::METHOD loadFile PRIVATE
	expose path
  use arg fileName
  command = "tar -C ".temp" --ignore-case --anchored -xf" path filename-
  "2>&1 >/dev/null"
  --say command
  command
  file = .temp||filename
  call (file)
	rc = SysFileDelete(file)

  package = result
  if package = "RESULT" then raise syntax 93
  return result


/*
The PackageData cass
This class is an abstract class for all packages to store their information in.
It has several methods that shall return information about the package.
*/
::CLASS PackageData
::METHOD Classes ABSTRACT
::METHOD Name ABSTRACT
::METHOD Version ABSTRACT
::METHOD Author ABSTRACT
::METHOD License ABSTRACT
