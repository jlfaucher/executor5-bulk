@echo off
rem
rem There are 3 ways of running this command:
rem
rem      make
rem      make debug
rem      make clean
rem
rem Anything else will cause problems.
rem
@nmake /F Makefile.win %1
