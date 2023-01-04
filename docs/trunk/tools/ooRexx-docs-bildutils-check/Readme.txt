This folder contains the rexx script CheckDoctoolsandUpload.rex

used by the Jenkins framework to 
(i)  see if any changes have been made to the readme.txt in the documentation
     build tools folder, and,
(ii) if it has been amended, upload readme.txt to sourceforge
     oorexx-docs-bildutils in the Files section

The script should be called as follows:

<full-path-to-rexx/>rexx CheckDoctoolsandUpload.rex sfRoot sftoolsfolder

Default settings are

sfRoot = 'svn.code.sf.net/p/oorexx/code-0/docs/trunk/tools/'
sftoolsfolder = '/home/frs/project/oorexx/oorexx-docs-bildutils'

The script will upload readme.txt only if it has been amended.
Further instructions and requirements are embedded in the script.

Once launched the script will set up its own working environment. 

This script is intended for use by the developers only. Do not try this at home ;-)
