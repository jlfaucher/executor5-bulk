#----------------------------------------------------------------------------
#
# Description: Makefile for the Object Rexx Sockets Library Documents.
#
# Copyright (c) 2005-2008, Rexx Language Association. All rights reserved.
#
# This program and the accompanying materials are made available under
# the terms of the Common Public License v1.0 which accompanies this
# distribution. A copy is also available at the following address:
# http://www.oorexx.org/license.html
#
# Redistribution and use in source and binary forms, with or
# without modification, are permitted provided that the following
# conditions are met:
#
# Redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer.
# Redistributions in binary form must reproduce the above copyright
# notice, this list of conditions and the following disclaimer in
# the documentation and/or other materials provided with the distribution.
#
# Neither the name of Rexx Language Association nor the names
# of its contributors may be used to endorse or promote products
# derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
# FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
# TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
# OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
# OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# Author(s):
#      W. David Ashley <dashley@us.ibm.com>
#
#----------------------------------------------------------------------------


STYLESHEET = /usr/share/sgml/docbook/xsl-stylesheets/fo/docbook.xsl \
             ../shared/oorexx.fo.xsl \
             ../shared/oorexx.fo.titlepage.xsl
HTML_STYLESHEET = /usr/share/sgml/docbook/xsl-stylesheets/html/docbook.xsl \
             ../shared/oorexx.html.xsl



XML_FILES = advcntlc.xml \
            basedialog.xml \
            buttoncontrols.xml \
            categorydialog.xml \
            comboboxc.xml \
            dialogcontrolc.xml \
            editcontrolc.xml \
            listboxc.xml \
            listcontrolc.xml \
            mesextcl.xml \
            dlgareac.xml \
            oodialog.xml \
            plainuserdialogc.xml \
            preface.xml \
            progressbarc.xml \
            propertysheetc.xml \
            reference.xml \
            resdialog.xml \
            scrollbarc.xml \
            slidercontrolc.xml \
            staticcontrolc.xml \
            tabcontrolc.xml \
            termdef.xml \
            treecontrolc.xml \
            userdialog.xml \
            virtualkeycodesc.xml \
            ../shared/notices.xml \
            ../shared/legalstuff.xml \
            ../shared/gethelp.xml \
            ../shared/CPLv1.0.xml

IMAGE_FILES = rxou0s12.jpg \
              rxou0s17.jpg \
	      rxou0s18.jpg \
	      rxou0s19.jpg \
	      rxou0s20.jpg \
	      rxou0s24.jpg \
	      rxou0s25.jpg \
 	      docbook-1.png \
	      oorexx.jpg

# The following is so that fop can find the jai class files. Otherwise fop will
# not be able to process the png graphic files for the pdf document.
CLASSPATH=/usr/share/java/jai-core.jar:/usr/share/java/jai-codec.jar:$CLASSPATH


all: pdf html

pdf: oodialog.pdf

html: oodialog-html.zip

oodialog.pdf: oodialog.fo
	fop -param fop1.extensions 1 \
	  -fo oodialog.fo -pdf oodialog.pdf

oodialog.fo: $(IMAGE_FILES) $(XML_FILES) $(STYLESHEET) svnrev.tmp
	xsltproc --output oodialog.fo \
	  --stringparam use.extensions 0 \
	  --stringparam fop1.extensions 1 \
	  ../shared/oorexx.fo.xsl oodialog.xml

oodialog.html: $(IMAGE_FILES) $(XML_FILES) $(HTML_STYLESHEET) svnrev.tmp
	xsltproc --output oodialog.html \
	  ../shared/oorexx.html.xsl oodialog.xml

oodialog-html.zip: oodialog.html
	zip oodialog-html *.html *.jpg

../shared/oorexx.fo.titlepage.xsl: ../shared/titlepage.template.xml
	xsltproc --output ../shared/oorexx.fo.titlepage.xsl \
	  /usr/share/sgml/docbook/xsl-stylesheets/template/titlepage.xsl \
	  ../shared/titlepage.template.xml

svnrev.tmp:
	svnversion > svnrev.tmp




clean:
	rm *.fo *.pdf *.html *.zip *.tmp

