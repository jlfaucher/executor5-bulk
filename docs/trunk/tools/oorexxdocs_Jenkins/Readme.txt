This folder contains the rex script BuildandUploadDocs.rex
used by the Jenkins framwork to 
(i) build the ooRexx documentation (if amended) using the existing documentation
build tools, and
(ii) upload the ooRexx documentation to sourceforge.

The script will check and build/upload only documents that have been
amended. Further instructions and requirements are embedded in the script.

Once launched the script will set up its own working environment. 

This script is intended for use by the developers only. Do not try this at home ;-)

--------------------------------------------------

Detailed Information

Toolset for Automated building of ooRexx documentation
Using Jenkins with a client running on a macOS Platform

With "Jenkins" is meant the Web service for building & testing used, to be found under

https://build.oorexx.org/

Currently the documentation is built on macOS Catalina (10.15), the utilities in this folder are used on top of what is installed from oorexxdocs_macOS folder

Should there be a need to move to another platform this can serve as a template.

SETUP
-----

1. Copy the folder oorexxdocs_macOS to /Users/jenkins/workspace/
   and rename to ooRexxdocs-build, follow the instructions in the readme.txt file.

2. Copy the items in folder oorexxdocs_Jenkins to ooRexxdocs-build

3. Do not forget to INSTALL the Liberation Fonts (step 7) before building

4. In Jenkins, create an "Agent" that connects to the machine where the build is to
   take place

5. In Jenkins, create a "Project" called ooRexxdocs-build

6. In Jenkins use this url for svn
   https://svn.code.sf.net/p/oorexx/code-0/docs/trunk oorexxdocSVN
   For a Release version use this (or similar)
   https://svn.code.sf.net/p/oorexx/code-0/docs/branches/5.0.0

7. The Jenkins build steps use the rexx script BuildandUploadDocs.rex
   # This script will build the ooRexx documentation
   # Only books that have been changed will be built
   # PDF and HTML versions of each book is built
   export PATH=~/Applications/ooRexx5/bin:$PATH
   rexx -v
   which rexx
   rexx BuildandUploadDocs.rex

8. BuildandUploadDocs.rex uses scp to upload the documentation to Sourceforge
   - The user must have R/W (admin) rights on Sourceforge.
   - Read BuildandUploadDocs.rex itself for more details.
