To build ooRexx docs on Windows we need to install Publican plus fop
Publican on Windows has been unsupported for a long time and some time
before end of 2018 its download site, fedorahosted.org, has disappeared.

The files in this directory are an attempt to keep our ability to build
on Windows.

PublicanWindows.zip
unzip as \Program Files (x86)\Publican
the original xsl\pdf.xsl was modified for use with ooRexx (see xsl\pdf.xsl-changes.txt)

fop-1.1.mod-bin.zip
a modified version of the original fop-1.1-bin.zip
unzip to any suitable location; the optional tools (see below) expect it at %USERPROFILE%\Downloads\FOP\fop-1.1
the original fop.bat was modfied to increase heap space (see fop.bat-changes.txt)

PublicanWindows.zip
Erich's optional tools to enable
* a message file to suppress thousands of fop warnings during build time (fop-preprocess.rex)
* a fop preprocesser to remove unwanted whitespace (findPub.rex)
* a simple script to start a build (PublicanBuildHere.bat)

