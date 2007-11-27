#!/usr/bin/rexx
/* This application creates a tar archive from a path given. */

if arg() = 0 then call help
else do
  tokens = arg(1)~makeArray(" ")
  strip = .false
  state = 1
  name = "archive.tar.gz"
  -- little state machine to parse command line arguments
  do i = 1 to tokens~items
    token = tokens[i]~upper
    if state = 1 | state = 3 then do
      if token = "-S" & \ strip then strip = .true
      else if token = "-O" & state = 1 then state = 2
      else do
        path = tokens[i]
        state = 4
      end
    end
    else if state = 2 then do
      name = tokens[i]
      state = 3
    end
    else if state = 4 then
      Say "Unknown token" tokens[i]
  end
  select
    when state = 1 | state = 3 then say "Missing path"
    when state = 2 then say "Missing file name for -o"
    otherwise call package name, path, strip
  end
end

exit


help:
Say "rexxmkarchive.rex [-so] directory - Creates a rexx package archive"
Say "Optional parameters:"
Say " -o FILE  The file to save the archive in."
Say " -s       Strip the archived files."
return

package: procedure
use strict arg name, path, strip
"tar --exclude=.svn --group 0 --owner 0 -czf" name path
Say ".TarBackEnd~new("""name""","""path""")"
return
