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



XML_FILES = classes.xml \
            api.xml \
            command.xml \
            concept.xml \
            meet.xml \
            preface.xml \
            provide.xml \
            rexio.xml \
            rexxpg.xml \
            rxruntime.xml \
            samples.xml \
            tour.xml \
            ../shared/notices.xml \
            ../shared/legalstuff.xml \
            ../shared/gethelp.xml \
            ../shared/CPLv1.0.xml

IMAGE_FILES = rxo90004.jpg \
	      rxo90005.jpg \
	      rxo90006.jpg \
	      rxo90007.jpg \
	      rxo90008.jpg \
	      rxo90011.jpg \
	      rxo90014.jpg \
	      rxo90017.jpg \
	      rxo90018.jpg \
	      rxo90019.jpg \
	      rxo90020.jpg \
	      rxoq0s01.jpg \
	      rxoq0s02.jpg \
	      rxoq0s03.jpg \
	      rxoq0s04.jpg \
	      rxoq0s05.jpg \
	      rxoq0s06.jpg \
	      rxoq0s07.jpg \
	      rxoq0s09.jpg \
	      ooRexxClasses1.png \
	      ooRexxClasses2.png \
	      ooRexxClasses3.png \
	      ooRexxClasses4.png \
	      ooRexxClasses5.png \
	      ooRexxClasses6.png \
	      ooRexxClasses7.png \
 	      oorexx.jpg \
 	      docbook-1.png \
	      new_rexxla.jpg

# The following is so that fop can find the jai class files. Otherwise fop will
# not be able to process the png graphic files for the pdf document.
CLASSPATH=/usr/share/java/jai-core.jar:/usr/share/java/jai-codec.jar:$CLASSPATH


all: pdf html

pdf: rexxpg.pdf

html: rexxpg-html.zip

rexxpg.pdf: rexxpg.fo
	fop -param fop1.extensions 1 \
	  -fo rexxpg.fo -pdf rexxpg.pdf

rexxpg.fo: $(IMAGE_FILES) $(XML_FILES) $(STYLESHEET) svnrev.tmp
	xsltproc --output rexxpg.fo \
	  --stringparam use.extensions 0 \
	  --stringparam fop1.extensions 1 \
	  ../shared/oorexx.fo.xsl rexxpg.xml

rexxpg.html: $(IMAGE_FILES) $(XML_FILES) $(HTML_STYLESHEET) svnrev.tmp
	xsltproc --output rexxpg.html \
	  ../shared/oorexx.html.xsl rexxpg.xml

rexxpg-html.zip: rexxpg.html
	zip rexxpg-html *.html *.jpg

../shared/oorexx.fo.titlepage.xsl: ../shared/titlepage.template.xml
	xsltproc --output ../shared/oorexx.fo.titlepage.xsl \
	  /usr/share/sgml/docbook/xsl-stylesheets/template/titlepage.xsl \
	  ../shared/titlepage.template.xml

svnrev.tmp:
	svnversion > svnrev.tmp




clean:
	rm *.fo *.pdf *.html *.zip *.tmp

