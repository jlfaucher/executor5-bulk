#!/usr/bin/env rexx

-- #867 Determine whether a stream is associated with a terminal
-- https://sourceforge.net/p/oorexx/feature-requests/867/

parse arg filename option .

stream = .nil
if filename \== "" then do
    stream = .stream~new(filename)
    stream~open("append")
end

output = .stdout
if option == "dont_stdout" then output = .stderr

width = 10
indent = "    "

call output indent "stdin " "streamType:" .stdin~query( "streamtype")~left(width) "isTTY:" .stdin~query( "isTTY")~left(width)
call output indent "stdout" "streamType:" .stdout~query("streamtype")~left(width) "isTTY:" .stdout~query("isTTY")~left(width)
call output indent "stderr" "streamType:" .stderr~query("streamtype")~left(width) "isTTY:" .stderr~query("isTTY")~left(width)

output~say
if .nil \== stream then stream~say

exit 0

output:
    use strict arg text
    output~say(text)
    if .nil \== stream then stream~say(text)
    return
