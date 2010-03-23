#----------------------------------------------------------------------------
#
# Description: Makefile for the Object Rexx Reference Documents.
#
# Copyright (c) 2005-2010, Rexx Language Association. All rights reserved.
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


STYLESHEET = /usr/share/sgml/docbook/dsssl-stylesheets/print/docbook.dsl \
	          ../shared/oorexx.print.dsl

HTML_STYLESHEET = /usr/share/sgml/docbook/dsssl-stylesheets/html/docbook.dsl \
	                ../shared/oorexx.html.dsl

xml_FILES = winprogmanager.xml \
		   clipboard.xml \
		   winregistry.xml \
		   wineventlog.xml \
		   windowmanager.xml \
		   windowobj.xml \
		   menuobj.xml \
		   winole.xml \
		   wsheng.xml \
	           ../shared/legalstuff.xml \
	           ../shared/notices.xml \
	           ../shared/gethelp.xml \
	           ../shared/CPLv1.0.xml

IMAGE_FILES =	docbook-1.png \
	            oorexx.jpg

JADETEXFMT  = ./pdfjadetex.fmt
HYPERREFCFG = ./hyperref.cfg
TEXMF       = /usr/share/texmf

export TEXFORMATS=.:

all: pdf html

pdf: winextensions.pdf

html: winextensions-html.zip

winextensions.pdf: $(IMAGE_FILES) $(STYLESHEET) $(JADETEXFMT) genindex.xml svnrev.tmp
	docbook2pdf -d ../shared/oorexx.print.dsl winextensions.xml

book1.htm: $(IMAGE_FILES) $(HTML_STYLESHEET) genindex.xml svnrev.tmp
	docbook2html -d ../shared/oorexx.html.dsl winextensions.xml

winextensions-html.zip: book1.htm
	zip winextensions-html *.htm *.jpg *.png

$(JADETEXFMT): $(HYPERREFCFG)
	pdftex -ini \&pdflatex pdfjadetex.ini

genindex.xml: $(HTML_STYLESHEET) $(xml_FILES) svnrev.tmp
	collateindex.pl -N -o genindex.xml
	jade -t xml -d ../shared/oorexx.html.dsl -V html-index winextensions.xml
	rm *.htm
	collateindex.pl -p -g -o genindex.xml HTML.index

svnrev.tmp:
	svnversion > svnrev.tmp


clean:
	rm -f *.log *.aux *.out *.fmt *.pdf genindex.xml *.htm *.zip HTML.index *.tmp

