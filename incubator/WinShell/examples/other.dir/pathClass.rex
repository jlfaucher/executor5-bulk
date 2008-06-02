/** This is a sample program showing usage of the .Path class.  This class
 *  provides access to the Shell lightweight path utilities.  Each Shell path
 *  function is accessed by using the .Path class method of the same name.  For
 *  most of the functions there is a one to one map of the function name to the
 *  class method name, but not for all of them:
 *
 *    Shell utility function        .Path class method
 *    ====================================================
 *    PathMakePretty                .Path~makePretty
 *    PathCanonicalize              .Path~canonicalize
 *    PathCompactPathEx         *   .Path~compact
 *    PathQuoteSpaces               .Path~quoteSpaces
 *    PathUnquoteSpaces             .Path~unquoteSpaces
 *    PathAddBackslash              .Path~addBackslash
 *    PathRemoveBackslash           .Path~removeBackslash
 *    PathFileExists            *   .Path~exists
 *    PathIsDirectory               .Path~isDirectory
 *    PathIsDirectoryEmpty          .Path~isDirectoryEmpty
 *    PathGetShortPath              .Path~getShortPath
 *    <N/A> (GetLongPathName)   *   .Path~getLongPath
 *
 *  The sample might, one day, show examples of all the methods.  Unfortunately
 *  it is only partially done.
 */

  -- Unlike the normal usage of the WinShell classes, you do *not* need to
  -- instantiate an object before using the .Path methods.  All methods are
  -- implemented as class methods.


  path = "E:\UNWORK.OOREXX\OTHER\HELP\ACTIVE.WINDOWS\SAMPLES"
  say 'Path before makePretty:' path
  fPath = .Path~makePretty(path)
  say 'Path after makePretty: ' fPath
  say

  say 'The PathMakePretty API only changes the path if it is all caps ;-('
  say

  path = "E:\Unwork.ooRexx\Other\Help\Active.Windows\Samples"
  say 'Path before makePretty:' path
  fPath = .Path~makePretty(path)
  say 'Path after makePretty: ' path
  say


  -- Going to use .WinShell here to get some common paths.  Remember it is not
  -- necessary to instantiate a WinShell object to use .Path.
  shell = .WinShell~new

  desktopPath = shell~pathFromCSIDL("CSIDL_COMMON_DESKTOPDIRECTORY")
  desktopPathQ = .Path~quoteSpaces(desktopPath)
  deskTopPathUnQ = .Path~unquoteSpaces(desktopPathQ)
  say 'Path:          ' desktopPath
  say "Path quoted:   " desktopPathQ
  say "unquote quoted:" desktopPathUnQ
  say

  startUpPath = shell~pathFromCSIDL("CSIDL_COMMON_STARTUP")
  say 'Path:' startUpPath
  say '  exists?        ' booleanToText(.Path~exists(startUpPath))
  say '  is a directory?' booleanToText(.Path~isDirectory(startUpPath))
  say '  is empty?      ' booleanToText(.Path~isDirectoryEmpty(startUpPath))
  say

  bogusDir = "C:\non-existent-directory\madeUp"
  say 'A non existent directory is not a directory, so it can not be an'
  say 'empty directory'
  say
  say 'Path:              ' bogusDir
  say 'is empty directory?' booleanToText(.Path~isDirectoryEmpty(bogusDir))
  say

  fPath = directory()
  nPath = fPath'\test'
  say 'Path:' nPath
  if .Path~isDirectory(nPath) then do
    say " " nPath "IS a directory"
    if .Path~isDirectoryEmpty(nPath) then say " " nPath 'IS empty'
    else say nPath 'is NOT empty'
  end
  else do
    say " " nPath "is NOT a directory"
  end
  say "  Empty?" booleanToText(.Path~isDirectoryEmpty(nPath))
  say

  nPath = nPath'\'
  say 'Path:' nPath
  nPath = .Path~removeBackslash(nPath)
  say '  remove backslash:' nPath

  nPath = .Path~removeBackslash(nPath)
  say '  remove again:    ' nPath
  path = .Path~addBackslash(nPath)
  say '  add backslash:   ' path
  say

  say "Use path compact"
  say 'Long path:' path
  say '     ' .Path~compact(nPath, 60)
  say '     ' .Path~compact(nPath, 50)
  say '     ' .Path~compact(nPath, 40)
  say '     ' .Path~compact(nPath, 30)
  say '     ' .Path~compact(nPath, 20)
  say '     ' .Path~compact(nPath, 10)

::requires 'WinShell.cls'

::routine booleanToText
  use strict arg b
  if b then return 'true'
  else return 'false'
