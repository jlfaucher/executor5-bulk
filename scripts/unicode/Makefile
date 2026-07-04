################################
# Makefile for nmake (Windows) #
################################

# Keep this name!
# Under Macos and Linux, make will use in priority GNUmakefile.
# Under Windows, nmake will ignore GNUmakefile and use Makefile.

# ooRexxShell scripts in the current directory
ooRexxShellFiles = diary_examples

# ooRexx scripts in the current directory
scriptsFiles = enums_test \
               incorrect_character \
               RexxUnicode_test \
               RexxUnicodeServices_test

NMAKE = nmake /c /f Makefile

# https://gnuwin32.sourceforge.net/packages/diffutils.htm
# When using echo in a pipe (e.g., (...echo RC = 0) | oorexxshell), CMD
# reconstructs the command line internally, inserting spaces during parsing.
# This introduces unwanted spaces.
# diff  -b  --ignore-space-change  Ignore changes in the amount of white space.
DIFF = diff --ignore-space-change

# fc is native Windows but returns sometimes the code '0x1', whereas both files are identical (line ends differ)
# DIFF = fc /W

help:
	@echo nmake all
	@echo ---------- ooRexxShellFiles
	@-for %%f in ($(ooRexxShellFiles)) do @echo nmake %%f
	@echo ---------- scriptsFiles
	@-for %%f in ($(scriptsFiles)) do @echo nmake %%f
	@echo ----------
	@echo nmake clean

all:
	@-for %%f in ($(ooRexxShellFiles)) do @$(NMAKE) %%f
	@-for %%f in ($(scriptsFiles)) do @$(NMAKE) %%f

$(ooRexxShellFiles):
	( type $@.oorexxshell & echo RC = 0 ) | oorexxshell test regression > $@-out.txt 2>&1
	$(DIFF) $@-out-ref.txt $@-out.txt > nul

$(scriptsFiles):
	rexx $@.rex --testRegression > $@-out.txt 2>&1
	$(DIFF) $@-out-ref.txt $@-out.txt > nul

clean:
	@-for %%f in ($(ooRexxShellFiles)) do @del -f %%f-out.txt
	@-for %%f in ($(scriptsFiles)) do @del -f %%f-out.txt
