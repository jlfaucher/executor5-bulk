################################
# Makefile for nmake (Windows) #
################################

# Keep this name!
# Under Macos and Linux, make will use in priority GNUmakefile.
# Under Windows, nmake will ignore GNUmakefile and use Makefile.

# ooRexxShell scripts
ooRexxShellFiles = diary_examples \
                   rxunicode_examples

# ooRexx scripts
scriptsFiles1= eastAsianWidthIsAmbiguous_test \
               enums_test \
               incorrect_character \
               RexxUnicode_test \
               RexxUnicodeServices_test \
               ..\..\main\trunk\interpreter\classes\support\Unicode\unicode-width\tests\tests

# ooRexx scripts, StringIndexer
scriptsFiles2= StringIndexer_test1 \
               StringIndexer_test2 \
               StringIndexer_test3 \
               StringIndexer_test4 \
               StringIndexer_test5 \
               StringIndexer_test6 \
               StringIndexer_test7 \
               StringIndexer_test8 \
               StringIndexer_test9

# ooRexx scripts, results impacted by ICU4C version
scriptsFiles3= charWidth_stringWidth_test \
               controlBoundary_test \
               width_printable_test

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
	@echo nmake clean
	@echo.
	@echo ---------- ooRexxShellFiles
	@-for %%f in ($(ooRexxShellFiles)) do @echo nmake %%f
	@echo.
	@echo ---------- scriptsFiles1
	@-for %%f in ($(scriptsFiles1)) do @echo nmake %%f
	@echo.
	@echo ---------- scriptsFiles2, StringIndexer
	@-for %%f in ($(scriptsFiles2)) do @echo nmake %%f
	@echo.
	@echo ---------- scriptsFiles3, results impacted by ICU4C version
	@-for %%f in ($(scriptsFiles3)) do @echo nmake %%f

all:
	@for %%f in ($(ooRexxShellFiles)) do @$(NMAKE) %%f || exit /b
	@for %%f in ($(scriptsFiles1)) do @$(NMAKE) %%f || exit /b
	@for %%f in ($(scriptsFiles2)) do @$(NMAKE) %%f || exit /b
	@for %%f in ($(scriptsFiles3)) do @$(NMAKE) %%f || exit /b

ooRexxShellFiles: $(ooRexxShellFiles)

$(ooRexxShellFiles):
	del /F $@-out.txt > nul 2>&1
	( type $@.oorexxshell & echo RC = 0 ) | oorexxshell test regression > $@-out.txt 2>&1
	:: $(DIFF) $@-out.txt $@-out-ref.txt > nul
	$(DIFF) $@-out-ref.txt $@-out.txt > nul

scriptsFiles1: $(scriptsFiles1)

scriptsFiles2 StringIndexer: $(scriptsFiles2)

scriptsFiles3: $(scriptsFiles3)

$(scriptsFiles1) $(scriptsFiles2) $(scriptsFiles3):
	del /F $@-out.txt > nul 2>&1
	rexx $@.rex --testRegression > $@-out.txt 2>&1
	:: $(DIFF) $@-out.txt $@-out-ref.txt > nul
	$(DIFF) $@-out-ref.txt $@-out.txt > nul

clean:
	@-for %%f in ($(ooRexxShellFiles)) do @del /f %%f-out.txt > nul 2>&1
	@-for %%f in ($(scriptsFiles1)) do @del /f %%f-out.txt > nul 2>&1
	@-for %%f in ($(scriptsFiles2)) do @del /f %%f-out.txt > nul 2>&1
	@-for %%f in ($(scriptsFiles3)) do @del /f %%f-out.txt > nul 2>&1
