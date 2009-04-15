---------------------------------------------------------
Version 3.1.2
---------------------------------------------------------
This release provides documentation for Open Object Rexx.

The following documents are available in the release:
        Open Object Rexx: Reference
        Open Object Rexx: Programming Guide
        Open Object Rexx: RxMath Math Functions Reference
        Open Object Rexx: RxSock TCP/IP Socket Functions Reference
        Open Object Rexx: RxFtp Class Library Reference
        Open Object Rexx: Windows OODialog Reference
        Open Object Rexx: Release Notes

These documents and the accompanying materials are made available under
the terms of the Common Public License v1.0 which accompanies this
distribution. A copy is also available at the following address:

http://www.oorexx.org/license.html

These documents cover all platforms and operating systems known to
work with Open Object Rexx including Windows, Linux, Solaris, and AIX.

The example code in all the documents has been modified from the last
revision of these documents. It is now rendered in a typewriter font
and most of the single quotes have been changed to double quotes. This
will make most of the examples compatible for cut and paste operations
on most platforms.

Although the text source files all have an SGML extension, they are all
treated as if they were XML documents.

Feedback, as always, is welcome and encouraged.


---------------------------------------------------------
PREREQUISITES
---------------------------------------------------------
The following software is required to build the documentation:

Any version of Linux with the following components installed

        DocBook  >= 4.2
        OpenJade >= 1.3.2
        TeX      >= 7.4.5 with passivetex installed

The above configuration is available in Fedora Core 3 and later.

WARNING!!!!!!!!

The standard /usr/share/texmf/web2c/texmf.cnf file contains settings
that are too small to process the rexxref documentation. Please reference the
texmf.cnf file in the trunk of the docs repository for the entries that need
to be modified.


---------------------------------------------------------
BUILD / INSTALLATION
---------------------------------------------------------

Whether you downloaded a tarball or a CVS extract, the build procedure
is the same. Use the standard Linux make mechanism:

        make

The current build mechanism makes both PDF and HTML versions of the
documents. The HTML documents are primarily built for the ooRexx
website but can be deployed on a user's machine as well. In any case,
the primary delivery format for all documents is PDF. You will need to
modify the makefiles to create other kinds of output.

WARNING!!!!!!!!

The standard /usr/share/texmf/web2c/texmf.cnf file contains settings
that are too small to process the rexxref documentation. Please reference the
texmf.cnf file in the trunk of the docs repository for the entries that need
to be modified.


---------------------------------------------------------
CLEANUP
---------------------------------------------------------

To remove all the build files, type:

        make clean


---------------------------------------------------------
CREDITS
---------------------------------------------------------

The Rexx Language Association would like to thank IBM for contributing
Object Rexx to the open source community. Without their help and
contributions this project would not have been possible.

RexxLA would also like to thank the open source community for their
review and contributions to this documentation.

These documents are maintained primarily by W. David Ashley
<dashley@us.ibm.com> with the help and collaboration of the entire
ooRexx project team. Problems with these documents should be reported
via the bug reporting tool at
http://sourceforge.net/tracker/?group_id=119701&atid=684730.


---------------------------------------------------------
FOR MORE INFO
---------------------------------------------------------

For more information please see the Open Object Rexx project website at
http://oorexx.sourceforge.net/.

Also visit the Open Object Rexx website at http://www.oorexx.org/ or the
Rexx Language Association website at http://www.rexxla.org/

