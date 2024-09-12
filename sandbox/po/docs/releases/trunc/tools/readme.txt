docs\trunk\tools (as of: 2024-04-22)
------------------------------------

To use the tools below start by checking out the complete documentation source:

svn co svn://svn.code.sf.net/p/oorexx/code-0/docs/trunk/ docSVN


The tools directory is supposed to collect all utilities that can be used to
create/change documentation parts. IF you decide to add any tools to this folder
make sure to update this file.

- rexxpg/createClassHierarchy.rex:

        - creates the class hierarchy for the "rexxpg" book, chapter "provide.xml"

        - usage:

                createClassHierarchy > classHierarchy.xml

- listAuthorsAsOfLate.rex

        - lists the authors of the different books in case new authors need to
          be reflected in the books

        - usage from "tools" directory:

          listAuthorsasOfLate.rex ..

- postProcessSVG.rex

        - if SVG images get created by exporting an ODG image using OpenOffice, then
          remove the string " embedded" in the font-family name that AOO adds

- svnListRevisions.rex

        - This tool uses "svn list --xml --incremental" to get the revision numbers
          of all files and directories under revision control from the supplied path.

- updateCopyright.rex

        - Updates the copyright year in all files

        - usage from "tools" directory:

          updateCopyright [dir] [year]
                 dir  ... optional, defaults to ".."
                 year ... optional, defaults to current year

- updateEntityValues.rex

        - usage from "tools" directory:

          updateEntityValues [arguments]
                no argument    ... show this text
                dir            ... always the last argument, denotes the root of the docs
                                   directory to work on (if dir contains spaces it needs
                                   to be enquoted in double-quotes).
                                   If no switches are supplied than the current values
                                   of the YEAR, VERSION and EDITION entity values for
                                   each books get displayed.
                                   If running off the
                -y latestYear  ... set the latest year in YEAR to latestYear (a number)
                -v "version"   ... set VERSION to the supplied string in the form (quoted!)
                                   of "oldValue -> newValue"
                -e "edition"   ... set EDITION to the supplied string (can be anything);
                                   if -r switch is not set, then no revision information
                                   string gets appended
                -r revision    ... append revision information to EDITION; if the file
                                   ${book}/revision_info.txt exists, its first line will
                                   be used as the revision information string; otherwise
                                   the revision information is built from commits with
                                   revisions that are smaller than the supplied revision,
                                   the result gets also written to ${book}/revision_info.txt
          examples:
                updateEntityValues.rex -v "5.0.0 -> 5.1.0" -e "2022.12.24" ".."
                updateEntityValues.rex -e "2022.12.24" -r 12537  ..
                updateEntityValues.rex -y 2022 -v "5.0 -> 5.1" -e "2022.12.25" -r 99999  ..
                updateEntityValues.rex ..

- bldoc_orx:

        - contains the tools to build the ooRexx documentation; copy
          the contents of this folder to a folder to be used for building the
          documents and run setup.rex to complete the installation.

- bldoc_win:

        - contains the tools to build the ooRexx documentation on Windows; copy
          the contents of this folder to a folder to be used for building the
          documents and run setup.rex to complete the installation.

- oorexxdocs_macOS

        - contains the tools to build the ooRexx documentation on macOS; Copy
          the contents of this folder to a folder to be used for building the
          documents and follow the instructions in WhatIsHere.txt. For a better
          understanding of the build process reference is made to the read1st.txt
          in the Windows build tools

- oorexxdocs_Jenkins

        - contains an additional script to build the ooRexx documentation on
          a macOS Jenkins agent; Start by unzipping oorexxdocs_macOS and make
          sure everything works, thereafter unzip and copy the contents of this
          folder to a folder to be used for building the documents and follow
          the instructions in Readme.txt.
          This folder contains the rex script used by Jenkins to
          (i) build the ooRexx documentation (if amended)
          (ii) upload the ooRexx documentation
          The script will check and build/upload only documents that have been
          amended. Instructions for use are embedded in the script.
          This script is intended for use by the developers only.

- oorexxdocs_bildutils-check

        - contains an additional script used by the Jenkins framework to 
          (i) see if any changes have been made to the readme.txt file and
          (ii) upload it to sourceforge when there are amendments.
          This script is intended for use by the developers only.

- LiberationFonts

        - contains the fonts used to build the documentation

- RailRoadDiagrams

        - The ooRexx project uses a free tool "RailDiagramGenerator" for creating
          its syntax diagrams from ebnf. For more information read the
          RailDiagramGenerator-readme.txt
