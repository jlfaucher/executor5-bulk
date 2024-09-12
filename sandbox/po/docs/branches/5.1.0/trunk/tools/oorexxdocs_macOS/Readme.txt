Readme.txt

Toolset for building ooRexx documentation on macOS
--------------------------------------------------

Unlike Windows 10, where all the tools are included in the toolset,
this toolset assumes some installations being present beforehand.
Examples are given using Homebrew but also Macports has been shown to work.

INITIAL SETUP
-------------

1. Copy the folder oorexxdocs_macOS to a workspace and rename it to oorexxdocs

Note: the package can be installed anywhere, "workspace" is just an example,
Copying can be done by drop-and-drag

2. Get hold of the latest ooRexx doc source files using SVN
Note: As of macOS Catalina (10.15) SVN is not part of macOS installation
and must be installed from e.g. Homebrew
cd ~/workspace/oorexxdocs
svn co svn://svn.code.sf.net/p/oorexx/code-0/docs/trunk oorexxdocSVN

3. macOS already have libxslt/xsltproc installed; check that it is working
xsltproc --version should give something like this:
Using libxml 20904, libxslt 10129 and libexslt 817
xsltproc was compiled against libxml 20904, libxslt 10129 and libexslt 817
libxslt 10129 was compiled against libxml 20904
libexslt 817 was compiled against libxml 20904

4. brew install docbook
-> at the end you will see something like this:
To use the DocBook package in your XML toolchain,
you need to add the following to your ~/.bashrc:

export XML_CATALOG_FILES="/usr/local/etc/xml/catalog"
==> Summary
/usr/local/Cellar/docbook/5.1_1: 199 files, 8.9MB

Rather than doing this we have added this line to the shell files (in UPPERCASE)
so please ignore this remark from Homebrew

Note: the information indicates version 5.1 but it actually contains all versions back to version 4.1.2, we will be using version 4.5.

5. brew install docbook-xsl
-> installs into
/usr/local/Cellar/docbook-xsl/1.79.2_1: 4,910 files, 94.0MB

6. brew install fop (fop is a java pgm used to create the pdf files from .fo files)
-> installs fop 2.8 into /usr/local/Cellar/fop/2.8

7. install the Liberation Fonts (included in a separate package)
-> Copy them somewhere and then use the Font Collection app
  ("Schriftsammlung" in German)
- double click each font in the included folder Liberation
-> this will open the Font Collection App, select "install" for all fonts.
Read more here: http://pilhuhn.blogspot.com/2008/03/use-liberation-fonts-on-mac.html

8. brew install sevenzip 
-> installs into /usr/local/Cellar/sevenzip/22.01
This is needed to make the zip files compatible with Windows afterwards
https://7-zip.org

General background information

The following files/folders have been copied over from the Windows build tools
pdf.xsl
html.xsl
defaults.xsl
xhtml-common.xsl
fop.cfg
Liberation Fonts

These are the script files you will need:

doc2fo.sh creates a .fo file from the book description source

fo2pdf.sh creates a .pdf file from the .fo description

doc2pdf.sh is calling doc2fo.sh and fo2pdf.sh  to make a pdf 
from a book description

buildallpdfs.sh is calling doc2pdf repetively to make pdf versions
of all ooRexx books

doc2html.sh creates a html folder from a book description source
that can be browsed with a browser. a zipped copy is also generated

buildallhtml.sh is calling doc2html.sh to render all the ooRexx books
as html folders and zipfiles

buildalldocs.sh generate both PDF and HTML output for all books

How to build:

To build a specific book
bash doc2pdf.sh rexxpg
bash doc2html.sh rexxpg

To build all PDF (takes around 10 minutes)
bash buildallpdfs.sh

To build all HTML books (takes around 20 minutes)
bash buildallhtml.sh

To build everything (takes around 30 minutes)
bash buildalldocs.sh

To get a deeper understanding on building the documentation for ooRexx please
read the documentation for Windows.
