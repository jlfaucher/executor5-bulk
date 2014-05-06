DBF.CLS - Readme

This framework allows for the reading, writing, searching and traversing of DBF files
of type 3: FoxBASE+/Dbase III plus, no memo

It does NOT use or REINDEX any dBASE index files, but rather uses it's own 'in memory'
indexes.

It understands the following dbf data types
   C - Character
   D - Date
   L - Logical
   N - Numeric

The code is intended as a demonstration of what can be done with ooRexx and comes
entirely as/is without warrantee or support.

It may or may not work with other DBF file types and it may or may not work to
some extent with other data types.  You are welcome to extend it or support it
yourself.

As the original Author I can take no responsibility for your data and strongly
encourage you to back up any critical data in your DBF files before playing with
or relying on this.

It is not my intention to encourage anyone in a greenfield environment to choose dBASE
or one of it's descendants as a database for ooRexx.

There are many other suitable and relational databases available.
A good place to start would be ooRexx's native support for SQLLite

If you are forced through legacy to interact with DBF files, then other free options that
you might try which might be more suitable and/or have more support than this might include
1) RexxSQL for odbc with Microsoft dBASE Driver
2) BSF4ooRexx with JDBC

Here is some code to help you establish a connection with option 1
|  if rxfuncquery('SQLLoadFuncs')
|  then call rxfuncadd 'SQLLoadFuncs', 'REXXSQL', 'SQLLoadFuncs'
|  call SQLloadfuncs
|  connection = 'DBF1'
|  connectString = .array~of('Driver={Microsoft dBASE Driver (*.dbf)}',
|                           ,'DriverID=277',
|                           ,'Dbq=c:\dbfFolder')~makestring('l',';')
|  rv = SQLconnect(connection,,,,connectString)
|  if rv < 0 then say rv': Could not connect using' connectString

Enjoy this demonstration code

Jon Wolfers 2014


