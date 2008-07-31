/** Example of getShortPath / getLongPath.  Note that the underlying APIs that
 *  these methods use only work if the path already exists.  If you just make up
 *  a path name, you just get that path name back.
 */

  -- Examples using relative paths
  path  = "..\samples\short.path.data\activatedWindows.frm"
  short = .Path~getShortPath(path)
  long  = .Path~getLongPath(short)
  say 'Path name:      ' path
  say 'Short path name:' short
  say 'Long from short:' long
  say

  path  = "..\samples\short.path.data\activatedHook.dll"
  short = .Path~getShortPath(path)
  long  = .Path~getLongPath(short)
  say 'Path name:      ' path
  say 'Short path name:' short
  say 'Long from short:' long
  say

  -- Example using just a file name
  file  = "ShortPathDataFile.bin"
  short = .Path~getShortPath(file)
  long  = .Path~getLongPath(short)
  say 'File name:      ' file
  say 'Short file name:' short
  say 'Long from short:' long
  say

  -- Examples using a full path name.  Will get some known directories using a
  -- WinShell method.
  shell = .WinShell~new
  path  = shell~pathFromCSIDL("CSIDL_HISTORY")
  short = .Path~getShortPath(path)
  long  = .Path~getLongPath(short)
  say 'Path name:      ' path
  say 'Short path name:' short
  say 'Long from short:' long
  say

  path  = shell~pathFromCSIDL("CSIDL_PROGRAMS")
  short = .Path~getShortPath(path)
  long  = .Path~getLongPath(short)
  say 'Path name:      ' path
  say 'Short path name:' short
  say 'Long from short:' long
  say

  -- Examples using a made up path name.
  path  = "C:\My Bogus Diretory\myBogus.file"
  short = .Path~getShortPath(path)
  long  = .Path~getLongPath(short)
  say 'Path name:      ' path
  say 'Short path name:' short
  say 'Long from short:' long
  say

  file  = "someTotallyNonexistentFile.ImadeUp"
  short = .Path~getShortPath(file)
  long  = .Path~getLongPath(short)
  say 'File name:      ' file
  say 'Short file name:' short
  say 'Long from short:' long
  say

  return 0

::requires 'WinShell.cls'
