

               Template Files for an External Library Package
               ==============================================


The files in this directory are intended to be used as a quick start to
building a new external library package.


1.) Copy the files to a work directory for your new project.


2.) [optional]  Without changing anything in the files, you can build a
working external library package with the following command:

  nmake /NOLOGO /F MakeFile.generic


3.) If you tried step 2 above, first clean up:

  nmake /NOLOGO /F MakeFile.generic clean

Then rename the set of files to a name appropriate to your project.
MakeFile.generic can be renamed to just MakeFile.

4.) Go through the files and change the references to myFile* to the
appropriate names.  In the .cpp file, change the myRoutine and myMethod
names to something appropriate.


                               Start coding.
