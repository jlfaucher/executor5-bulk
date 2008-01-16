#!//bin/rexx
-- Combine script
-- Written by Moritz Hoffmann 2006, part of the orx-irc project
-- Released under the BSD license
-- This application takes all the classes defined in the files array
-- and combines their content to the final irc.cls file.
-- While doing this it removes indenting spaces, tab stops, comments (as far as
-- possible) at both the beginning of a line and in the middle of one.

outfile = "./out/irc.cls";
outstream = .stream~new(outfile)~~open("WRITE REPLACE")
files = .array~of("irc_main.cls","user.cls","channel.cls","messages.cls",,
		"routines.cls","events.cls",,
		"config.cls","socket.cls","defaultevents.cls",,
		"servercommands.cls","serverinfo.cls")
-- "talkback.cls",
total_lines_in	= 0
total_lines_out	= 0
total_char_in	= 0
total_char_out	= 0
found_comment = .false
if arg(1) = "--no-debug" then debug = .false
else debug = .true
do file over files
    filestream = .stream~new(file)~~open("READ")
    inlines = filestream~arrayin
    -- +1: the linebreak
    total_lines_in = total_lines_in + inlines~items + 1
    do inline over inlines
	total_char_in = total_char_in + inline~length
	-- remove tabs and additional spaces
	inline = inline~translate(" ",9~d2c)~space
	-- remove empty lines
	if inline = "" then do
	    total_lines_in = total_lines_in - 1
	    iterate
	end
	-- remove comment blocks, note that this doesn't work on inline comments'
	if (inline = "/**") | (inline = "/*") then found_comment = .true
	if inline = "*/" then do
		found_comment = .false
		iterate
	end
	if found_comment then iterate
	-- remove simple comments
	if inline~left(2)~translate = "--" then iterate
	-- remove comments somewhere on the line
	commentstart = inline~pos(" -- ")
	if commentstart > 0 then do
	    inline = inline~left(commentstart)~space
	end
	if inline = "" then iterate
	if \ debug then
    	    if inline~left(10)~translate = "CALL DEBUG" then iterate
        outstream~lineout(inline)
	total_lines_out = total_lines_out + 1
	total_char_out = total_char_out + inline~length + 1
    end
end
say "Read" files~items "files with" total_lines_in "lines and" total_char_in "bytes."
Say "Wrote one file with" total_lines_out "lines and" total_char_out "bytes."
Say "Ratio:" trunc(total_char_out/total_char_in*100)"%," trunc(total_char_in/total_lines_in,1) "chars/line (excluding emtpy lines)"
