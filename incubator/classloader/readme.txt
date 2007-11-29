The Open Object Rexx class loader.

Requirements
* Currently only tested on Linux, Windows version will be available later
* For archives tar and unzip are required. Get zip/unzip from
  ftp://ftp.dante.de/tex-archive/tools/zip/info-zip/

Creating archives:

tar --exclude .svn --group 0 --owner 0 -czf archive.tar.gz root/
zip -r archive root/  -x "*/.svn/*"

Bugs:
Note that all files need to have a line break at the end of file. Otherwise
the last line will be lost.

Running on Windows:

* zip



