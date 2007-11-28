#!/usr/bin/rexx
/* This application creates a tar archive from a path given. */

if arg() = 0 then call help
else do
  tokens = arg(1)~makeArray(" ")
  has = .directory~new
  expect = ""
  strip = .false
  output = "archive.tar.gz"
  include = .array~new
  -- little state machine to parse command line arguments
  do i = 1 to tokens~items
    token = tokens[i]
    if expect \= "" then do
      select
        when expect = "OUTPUT" then do
          output = token
        end
        when expect = "INCLUDE" then do
          include[include~items + 1] = token
        end
        otherwise Say "Unknown expect, internal error:" expect
      end
      expect = ""
      iterate
    end
    token = token~upper
    if has~strip = .nil & (token = "-S" | token = "--STRIP") then do
      strip = 1
      has~strip = 1
    end
    else if has~output = .nil & (token = "-O" | token = "--OUTPUT") then do
      has~output = 1
      expect = "OUTPUT"
    end
    else if token = "-I" | token = "--INCLUDE" then do
      has~include = 1
      expect = "INCLUDE"
    end
    else Say "Unknown token" tokens[i]
  end
  select
    when has~include \= 1 then say "Missing include directive"
    when expect \= "" then say "Missing expected element" expect
    otherwise call package output, include, strip
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
Say "producing file" name "with stripped content:" strip
do p over path
  say "include path:" p
end
"tar --exclude=.svn --group 0 --owner 0 -czf" name path
Say ".TarBackEnd~new("""name""","""path""")"
return
