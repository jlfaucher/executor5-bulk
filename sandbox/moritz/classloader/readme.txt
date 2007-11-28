The Open Object Rexx class loader.

Requirements
* Currently only tested on Linux, Windows version will be available later
* For archives tar and unzip/zipinfo is required. Zipinfo is included
in unzip package.

Creating archives:

tar --exclude .svn --group 0 --owner 0 -czf archive.tar.gz root/
zip -r archive root/  -x "*/.svn/*"


