#######################################
# GNUmakefile for make (Macos, Linux) #
#######################################

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
               ../../main/trunk/interpreter/classes/support/Unicode/unicode-width/tests/tests

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

help:
	@echo
	@echo "make all"
	@echo "make html"
	@echo "make clean"
	@echo
	@echo "---------- ooRexxShellFiles"
	@for f in $(ooRexxShellFiles); do echo "make" $$f; done
	@echo
	@echo "---------- scriptsFiles1"
	@for f in $(scriptsFiles1); do echo "make" $$f; done
	@echo
	@echo "---------- scriptsFiles2, StringIndexer"
	@for f in $(scriptsFiles2); do echo "make" $$f; done
	@echo
	@echo "---------- scriptsFiles3, results impacted by ICU4C version"
	@for f in $(scriptsFiles3); do echo "make" $$f; done
	@echo
	@echo "---------- html (needs asciinema and aha)"
	@for f in $(ooRexxShellFiles); do echo "make" $$f-out.html; done
	@echo

# Keep set -e and the backslash to stop immediately at the first error
all:
	@set -e; \
	for f in $(ooRexxShellFiles); do make $$f; done; \
	for f in $(scriptsFiles1); do make $$f; done; \
	for f in $(scriptsFiles2); do make $$f; done; \
	for f in $(scriptsFiles3); do make $$f; done
	@echo
	@echo "To be launched separately: make html (needs asciinema and aha)"

# Not in the 'all' target because I don't know how to redirect the output of asciinema to /dev/null
# Without such a redirection, it's difficult to see if some errors were raised during make all.
html:
	@for f in $(ooRexxShellFiles); do make $$f-out.html; done

ooRexxShellFiles: $(ooRexxShellFiles)

$(ooRexxShellFiles):
	rm -f $@-out.txt
	bash -i -c '( cat $@.oorexxshell ; echo RC = 0 ) | oorexxshell test regression > $@-out.txt 2>&1'
	# diff -q $@-out.txt $@-out-ref.txt
	diff -q $@-out-ref.txt $@-out.txt

scriptsFiles1: $(scriptsFiles1)

scriptsFiles2 StringIndexer: $(scriptsFiles2)

scriptsFiles3: $(scriptsFiles3)

$(scriptsFiles1) $(scriptsFiles2) $(scriptsFiles3):
	rm -f $@-out.txt
	bash -i -c 'rexx $@.rex  --testRegression > $@-out.txt 2>&1'
	# diff -q $@-out.txt $@-out-ref.txt
	diff -q $@-out-ref.txt $@-out.txt

%.cast: %.oorexxshell
	-asciinema rec -c "bash -i -c 'echo prompt off address | cat $< | oorexxshell --showInitialization test regression'" --overwrite $@

.PHONY: always
%-out.html: %.cast always
	asciinema convert -f raw $< - | aha --title "$@" > $@

clean:
	@for f in $(ooRexxShellFiles); do rm -f $$f-out.txt; done
	@#for f in $(ooRexxShellFiles); do rm -f $$f-out.html; done
	@for f in $(scriptsFiles1); do rm -f $$f-out.txt; done
	@for f in $(scriptsFiles2); do rm -f $$f-out.txt; done
	@for f in $(scriptsFiles3); do rm -f $$f-out.txt; done


# bash -i -c 'command" is a trick to bypass the SIP hell:
# The variable DYLD_LIBRARY_PATH is unset because of System Integrity Protection.
# Without this variable, BSF4ooRexx is not loaded by ooRexxShell, because I don't
# put BSF4ooRExx in a trusted directory and because I don't want to hack the
# binaries with install_name_tool.
# By using an interactive bash, I have this sequence:
# - execution of ~/.bashrc which calls ~/.bash_env
# - execution of ~/.bash_env which calls : . "$REXX_ENVIRONMENT"
# where the file $REXX_ENVIRONMENT is created like that when initializing the environment:
#     echo "export LD_LIBRARY_PATH=`echo $LD_LIBRARY_PATH`" >> "$REXX_ENVIRONMENT"
#     echo "export DYLD_LIBRARY_PATH=`echo $DYLD_LIBRARY_PATH`" >> "$REXX_ENVIRONMENT"
# These 2 variables are made available to the rexx process, and BSF4ooRexx is loaded.
