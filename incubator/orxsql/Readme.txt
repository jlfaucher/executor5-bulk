                            OrxSQL Library Reference


This is a preliminary library to support SQL CLI access from ooRexx. This
design is fairly complete but can still change as it is still not tested
to any extent.

The C++ code in this file conforms to the ISO/IEC 9075-3: "SQL Call-Level
Interface" and the ODBC V3.5 updates. Any database or driver that supplies a CLI
that conforms to this specification should be able to utilize this source code.
However, the design does NOT statically link to a vendor's CLI library. All
calls to CLI APIs are linked dynamically at run time. Thus this library is
independent of any vendor's specific CLI implementation and compiling the code
does not even require the vendor's library or header files to be present
(but it does require a set of compliant header files).

To compile the code on Windows does require a vendor's header files. We utilize
Microsoft's ODBC header files for this purpose as they are readily available
and mostly standards compliant.

To compile the code on Linux we utilize the unixODBC header files. These are
compliant with the standard and contain almost all of the Microsoft extensions.
Note that these header files are not usually installed by default on a Linux
distribution so you will need to add them. Your specific distribution's Software
Update/Install mechanism should be used to install the required unixODBC
development header files.

Example code
============

Here is some example code to get you stated on using the dbconnx class.


   dbx = .dbconnx~new('libmyodbc3.so')  -- mysql odbc library
   retc = dbx~connect('dbname', 'username', 'password')

   ...

   retc = dbx~disconnect()


This is an example on how to use the sqlstatement class. It assumes a dbconnx is
already connected (see example above).

   stmt = .sqlstatement~new(dbx)
   stmt~stmt = 'select * from mytable where name = ?'
   arr = .array~new()
   arr[1] = 'Brandon Cherry'    -- specify the substitution parameter array
   retc = stmt~bindParams(arr)  -- bind the parameters
   retc = stmt~prepare()	-- prepare the statement
   retc = stmt~execute()	-- execute the statement
   retc = stmt~fetch()	        -- fetch the first row
   do while retc <. 100
      cols = stmt~getRow()	-- get all the result columns as an array
      do col over cols	        -- display the array
         say col
         end
      retc = stmt~fetch()	-- fetch the next row
      end


David Ashley
ooRexx Team

