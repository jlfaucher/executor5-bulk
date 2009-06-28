                         Host Emulator (HostEmu)

HostEmu is a subcommand environment that partially emulates a TSO/CMS environment. It
provides a small subset of commands avaliable in those environments which make the
transition from a real host Rexx programming environment to a Linux Rexx environment much
easier. The following subcommands are available:

HI - halt the current Rexx program
TS - activate the Rexx trace mechanism
TE - deactivate the Rexx trace mechanism
EXECIO - an I/O mechanism

Other commands may be added later.

Currently the HostEmu subcommand environment is only available for Linux but Windows and
other *nix systems will probably be added in the future.

To include and use the HostEmu subcommand environment you must place a ooRexx directive
in your script. The following shows how to accomplish this.

::requires "hostemu" LIBRARY

This will activate the environment. The subcommand name is 'HostEmu' (the case of this
string is not important). You can send commands to this environment via the Rexx
address statement. Here is an example.

address hostemu 'execio * diskr "./inputfile.txt" (finis stem in.'

Note that the file name MUST be placed within a set of quotation marks.

The example above should look very familiar to a mainframe Rexx programmer. The big 
difference is that a real file name is used instead of a DDNAME and the HostEmu
environment is not the default address environment, thus the requirement that you either
include the 'HostEmu' environment name in the address statement or you cause the 'HostEmu'
environment to be made the default environment.

The HostEmu HI, TS, TE commands have no arguments that are acceptable in the HostEmu
environment. Thus their syntax is very simple. The EXECIO subcommand is more complicated
and requires some explanation. It is a simplified version of the mainframe command but
will provide most of the functionality you will need on a daily basis. Here is the
syntax diagram.

>>--EXECIO--+--num--+--+--DISKR--+--"filename"--(--+--FINIS STEM stemname.--+--><
	    +--*----+  +--DISKW--+		   +--STEM stemname. FINIS--+
                                 		   +--STEM stemname.--------+
						   +--FINIS--+--LIFO--+-----+
						    	     +--FIFO--+
						   	     +--SKIP--+

num
    Specifies the number of records (text lines) to read or write.
*
    Specifies that all remaining records are to be read or written.
DISKR
    The operation is a disk read operation.
DISKW
    The operation os a disk write operation.
filename
    The name of the file for the disk operation.
FINIS
    The file will be closed at the end of the operation.
STEM stemname.
    The operation is work against the specified stem. A trailing period is required or
    the name will be used as the root of a standard Rexx variable name.
LIFO
    Specifies that instead of a Rexx stem The Rexx SESSION queue should be used. The queue
    will be read or written to using LIFO.
FIFO
    Specifies that instead of a Rexx stem The Rexx SESSION queue should be used. The queue
    will be read or written to using FIFO.
SKIP
    Specifies the number of records (text lines) to be skipped. No stem or queue
    operations will be performed in this case.

Note that the LIFO, FIFO and SKIP options are not valid for DISKW operations.

These options should provide almost all of the EXECIO operations you will need in the
Linux environment.

I hope you will find this a useful tool.

W. David Ashley
Open Object Rexx Team

