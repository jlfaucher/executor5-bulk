This folder contains the rexx script CheckDoctoolsandUpload.rex

used by the Jenkins framework to 
(i) see if any changes have been made to the documentation build tools folder, and
(ii) upload those tools that have been amended to sourceforge Files section

The script should be called as follows:

<full-path-to-rexx/>rexx CheckDoctoolsandUpload.rex sfRoot sftoolsfolder

Default settings are

sfRoot = 'svn.code.sf.net/p/oorexx/code-0/docs/trunk/tools/'
sftoolsfolder = '/home/frs/project/oorexx/oorexx-docs-bildutils'

The script will check and build/upload only items that have been
amended. Further instructions and requirements are embedded in the script.

Currently there are two shortcomings in the script
(i) it is only considering files, not folders
(ii) it is not removing files from sftoolsfolder that have been deleted from sfRoot

Once launched the script will set up its own working environment. 

This script is intended for use by the developers only. Do not try this at home ;-)
