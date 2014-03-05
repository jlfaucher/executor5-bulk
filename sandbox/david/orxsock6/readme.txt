               orxsock6 - IPv6 Sockets Implementation

This is an initial attempt to implement an IPv6 Sockets interface to ooRexx.
It is a full replacement for the rxsock library, but it is object oriented,
thus the interface is via classes and methods and not function calls.

The version presented here is fully implemented and tested on Fedora20. It
has not been tested on Windows but should work provided I did not mess up
the needed Windows API calls.

Requirements (needed and not needed):

1. You do not need to have an IPv6 configured adapter to run the test
   programs. They all use the local (127.0.0.1 or ::1) interface.
2. You can use either IPv4 addresses or IPv6 addresses in the method calls.
   But do NOT mix them in the same program as it could cause problems.
3. There are both IPv4 and IPv6 test programs provided.

There are 4 ooRexx classes supplied in the class file orxsock6.cls.

Socket -      This class encapsulates a typical socket (any type or family).
INetAddress - This class encapsulates an Internet address.
Linger -      This class encapsulates the Linger options in a call to the 
              Socket methods SetSockOpt and GetSockOpt.
AdddrInfo -   This class encapsulates information about an Internet 
              addresses.

Three of the four classes are demonstrated in the provided test programs.
For the most part, the classes and methods are as simple as I could make
them and keep the required functionality. They mimic the undelying C API
functions as much as possible.

One of the things you will notice right away is that the Socket instance
methods require a lot more arguments that the old rxsock library functions.
This because the old rxsock library functions assumed the address mode for
all the functions was IPv4. This implementation is more flexible (you can 
use IPv4 or IPv6 addressing), but it results in more information about
addressing being required from the programmer. orxsock6 classes make no
assumptions about the addressing mode, you must provide that information.

As more functionality and tests are added to this implementation I will try
to keep this Readme up-to-date.


David Ashley

