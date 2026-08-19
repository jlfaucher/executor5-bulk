#######################################
# GNUmakefile for make (Macos, Linux) #
#######################################

# Keep this name!
# Under Macos and Linux, make will use in priority GNUmakefile.
# Under Windows, nmake will ignore GNUmakefile and use Makefile.

# ooRexxShell scripts in the current directory
ooRexxShellFiles = diary_examples \
                   rxunicode_examples

# ooRexx scripts in the current directory
scriptsFiles = controlBoundary_test \
               eastAsianWidthIsAmbiguous_test \
               enums_test \
               incorrect_character \
               RexxUnicode_test \
               RexxUnicodeServices_test \
               width_printable_test

help:
	@echo "make all"
	@echo "make html"
	@echo "---------- ooRexxShellFiles"
	@for f in $(ooRexxShellFiles); do echo "make" $$f; done
	@echo "---------- scriptsFiles"
	@for f in $(scriptsFiles); do echo "make" $$f; done
	@echo "---------- html (needs asciinema and aha)"
	@for f in $(ooRexxShellFiles); do echo "make" $$f-out.html; done
	@echo "----------"
	@echo "make clean"

all:
	@for f in $(ooRexxShellFiles); do make $$f; done
	@for f in $(scriptsFiles); do make $$f; done
	@echo
	@echo "To be launched separately: make html (needs asciinema and aha)"

# Not in the 'all' target because I don't know how to redirect the output of asciinema to /dev/null
# Without such a redirection, it's difficult to see if some errors were raised during make all.
html:
	@for f in $(ooRexxShellFiles); do make $$f-out.html; done

$(ooRexxShellFiles):
	rm -f $@-out.txt
	bash -i -c '( cat $@.oorexxshell ; echo RC = 0 ) | oorexxshell test regression > $@-out.txt 2>&1'
	diff -q $@-out-ref.txt $@-out.txt

$(scriptsFiles):
	rm -f $@-out.txt
	bash -i -c 'rexx $@.rex  --testRegression > $@-out.txt 2>&1'
	diff -q $@-out-ref.txt $@-out.txt

%.cast: %.oorexxshell
	-asciinema rec -c "bash -i -c 'echo prompt off address | cat $< | oorexxshell --showInitialization test regression'" --overwrite $@

.PHONY: always
%-out.html: %.cast always
	asciinema convert -f raw $< - | aha --title "$@" > $@

clean:
	@for f in $(ooRexxShellFiles); do rm -f $$f-out.txt; done
	@#for f in $(ooRexxShellFiles); do rm -f $$f-out.html; done
	@for f in $(scriptsFiles); do rm -f $$f-out.txt; done


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
