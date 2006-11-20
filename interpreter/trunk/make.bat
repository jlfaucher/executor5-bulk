@echo off
rem
rem There are 2 ways of running this command:
rem
rem      make
rem      make clean
rem
rem Anything else will cause problems.
rem
rem Note that debugging is set in the Nmake.inc file, not here.
rem
@nmake /F Makefile.win %1
