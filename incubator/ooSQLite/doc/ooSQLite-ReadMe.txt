                                  ooSQLite
                                  ========

ooSQLite is an ooRexx extension that provides an interface to the SQLite
database engine.  For complete details on SQLite, go to:

https://www.sqlite.org

The SQLite database engine is embedded in the ooSQLite extension.  SQLite
is Public Domain:

"All of the code and documentation in SQLite has been dedicated to the
public domain by the authors. All code authors, and representatives of the
companies they work for, have signed affidavits dedicating their
contributions to the public domain and originals of those signed affidavits
are stored in a firesafe at the main offices of Hwaci. All contributors are
citizens of countries that allow creative works to be dedicated into the
public domain. Anyone is free to copy, modify, publish, use, compile, sell,
or distribute the original SQLite code, either in source code form or as a
compiled binary, for any purpose, commercial or non-commercial, and by any
means."

Using ooSQLite:
---------------

ooSQLite is currently distributed as 32-bit and 64-bit binaries along with
some example programs and some informal example programs.  The informal
examples were used during development and may not be rigorously commented.
Eventually the informal examples may be either dropped from the
distribution, or enhanced to make them better examples.

To use ooSQLite in your ooRexx programs simply add this requires statement
to the program:

::requires 'ooSQLite.cls'

and ensure that the files ooSQLite.cls and ooSqlite.dll (Windows) or
oosqlite.so (unix-like systems) are somewhere in the path on your system.


                             foodsPristine.rdbx
                             ------------------

The binary database file, foodsPristine.rdbx, located in the samples/misc
directory, was created from a database definition file, foods.sql, included
in the example code that accompanies the book:

"Definitive Guide to SQLite", Second Edition, by Grant Allen and Mike Owens,
published by Apress.

Apress and the authors have okayed the use and inlcusion of the binary file
in the ooRexx project.

foodsPristine.rdbx is then copied into the examples directories, as
ooFoods.rdbx.  ooFoods.rdbx is used in some of the testing and example
programs.
