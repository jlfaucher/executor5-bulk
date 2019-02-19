@echo off
:-- download sqlite-amalgamation-nnnnnnn.zip from https://sqlite.org/download.html
:-- unpack four files shell., sqlite3.c sqlite3.h, and sqlite3ext.h to this directory
:-- then run this batch file
:-- then in the base directory run nmake -fmakefile.win package
copy sqlite3.c+sqlite3.c-append sqlite3.c
copy sqlite3.h+sqlite3.h-append sqlite3.h
