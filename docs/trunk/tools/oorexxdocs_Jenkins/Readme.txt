This folder contains the rexx script BuildandUploadDocs.rex
used by the Jenkins framwork to 
(i) build the ooRexx documentation (if amended) using the existing documentation
build tools, and
(ii) upload the ooRexx documentation to sourceforge.

The script will check and build/upload only documents that have been
amended. Further instructions and requirements are embedded in the script.

Once launched the script will set up its own working environment. 

As of 27.12.2023 the script can use the universal rexx build scripts from
bldoc_orx folder rather than the shell scripts originally used.

As of 2024-09-15 the script accepts two parameters on the command line:
Parameter 1 = sfRoot the root folder for the documentation in svn repository
Parameter 2 = sfdocfolder where to upload the built documentation on source forge

To use the rexx scripts rather than the shell scripts simply copy the
content of the entire folder bldoc_orx to the DocBuild folder.

Please note that on macOS a number of actions is necessary to install the
necessary tools. See Readme.txt in the oorexxdocs_macOS folder for details.

This script is intended for use by the developers only. Do not try this at home ;-)

--------------------------------------------------

Detailed Information

Toolset for Automated building of ooRexx documentation
Using Jenkins with a client running on a macOS Platform

With "Jenkins" is meant the Web service for building & testing used, to be found under

https://build.oorexx.org/

Currently the documentation is built on macOS Sonoma (14.6.1), the utilities in this folder are used on top of what is installed from oorexxdocs_macOS folder

Should there be a need to move to another platform this can serve as a template.

SETUP
-----

1. Copy the folder oorexxdocs_macOS to /Users/jenkins/workspace/
   and rename to ooRexxdocs-build, follow the instructions in the readme.txt file.

2. Copy the items in folder oorexxdocs_Jenkins to ooRexxdocs-build

2.5 *NEW* Copy the items in folder bldoc_orx to ooRexxdocs-build

3. Do not forget to INSTALL the Liberation Fonts (step 7) before building

4. In Jenkins, create an "Agent" that connects to the machine where the build is to
   take place

5. In Jenkins, create a "Project" called ooRexxdocs-build

6. In Jenkins use this url for the svn plugin
   https://svn.code.sf.net/p/oorexx/code-0/docs/trunk oorexxdocTEMP
   NOTE: the svn plugin is used merely to start the build, the build script
   then uses svn from the command line

7. The Jenkins Build Steps use the rexx script BuildandUploadDocs.rex
   In the Build Steps then use
   svn.code.sf.net/p/oorexx/code-0/docs/trunk for a build from trunk.
   For a release version use this (or similar)
   svn.code.sf.net/p/oorexx/code-0/docs/branches/5.0/trunk
   svn.code.sf.net/p/oorexx/code-0/docs/branches/5.0.0/trunk
   BuildandUploadDocs.rex will build the ooRexx documentation
   - Only books that have been changed will be built
   - PDF and HTML versions of each book are built

8. BuildandUploadDocs.rex uses scp to upload the documentation to Sourceforge
   - The user must have R/W (admin) rights on Sourceforge.
   - Read BuildandUploadDocs.rex itself for more details.
