                            OrxSQL Library Reference


This is a VERY preliminary library to support SQL CLI access from ooRexx. Nothing
you see here should be taken as the final design as most of it will probably
change as the design become more complete.

The C++ code in this file conforms to the ISO/IEC 9075-3: "SQL Call-Level
Interface" and the ODBC V3.5 updates. Any database or driver that supplies a CLI
that conforms to this specification should be able to utilize this source code.
However, the design does NOT statically link to a vendor's CLI library. All
calls to CLI APIs are linked dynamically at run time. Thus this library is
independent of any vendor's specific CLI implementation and compiling the code
does not even require the vendor's library or header files to be present.

To compile the code on Windows does require a vendor's header files. We utilize
Microsoft's ODBC header files for this purpose as they are readily available
and mostly standards compliant.

To compile the code on Linux we utilize the unixODBC header files. These are
compliant with the standard and contain almost all of the Microsoft extensions.
Note that these header files are not usually installed by default on a Linux
distribution so you will need to add them. Your specific distribution's Software
Update/Install mechanism should be used to install the required unixODBC
development header files.

David Ashley
ooRexx Team

