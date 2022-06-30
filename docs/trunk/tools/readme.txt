docs\trunk\tools (as of: 2020-05-26)
------------------------------------

This directory is supposed to collect all utilities that can be used to
create/change documentation parts.

- rexxpg/createClassHierarchy.rex:

        - creates the class hierarchy for the "rexxpg" book, chapter "provide.xml"

        - usage:

                createClassHierarchy > classHierarchy.xml

- postProcessSVG.rex

        - if SVG images get created by exporting an ODG image using OpenOffice, then
          remove the string " embedded" in the font-family name that AOO adds

- bldoc_win:

        - contains the tools to build the ooRexx documentation on Windows; copy
          the contents of this folder to a folder to be used for building the
          documents and run setup.rex to complete the installation.

- oorexxdocs-macOS.zip 

        - contains the tools to build the ooRexx documentation on macOS; unzip and
          copy the contents of this folder to a folder to be used for building the
          documents and follow the instructions in WhatIsHere.txt. For a better
          understanding of the build process reference is made to the read1st.txt
          in the Windows build tools
- oorexxdocs-Ubuntu.zip

        - contains the tools to build the ooRexx documentation on macOS; unzip and
          copy the contents of this folder to a folder to be used for building the
          documents and follow the instructions in WhatIsHere.txt. For a better
          understanding of the build process reference is made to the read1st.txt
          in the Windows build tools

          Note: the tools for Linux are relying on an internet connection and are
          considerably slower than on macOS or Windows. Proposals for amendments
          are welcome.

- JenkinsDocBuildTools.zip

        - contains the additional script to build the ooRexx documentation on
          a macOS Jenkins agent; Start by unzipping oorexxdocs_macOS and make
          sure everything works, thereafter unzip and copy the contents of this
          folder to a folder to be used for building the documents and follow
          the instructions in Readme.txt.
          This folder contains the rex script used by Jenkins to 
          (i) build the ooRexx documentation (if amended)
          (ii) upload the ooRexx documentation
          The script will check and build/upload only documents that have been
          amended. Instructions for use are embedded in the script.
