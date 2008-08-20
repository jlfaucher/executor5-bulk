#----------------------------------------------------------------------------
#
# Description: Makefile for the ooTest Framework Documents.
#
# Copyright (c) 2008, Rexx Language Association. All rights reserved.
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
#----------------------------------------------------------------------------


STYLESHEET = /usr/share/sgml/docbook/dsssl-stylesheets/print/docbook.dsl \
             ../shared/oorexx.print.dsl
HTML_STYLESHEET = /usr/share/sgml/docbook/dsssl-stylesheets/html/docbook.dsl \
             ../shared/oorexx.html.dsl


OOTEST_SGML_FILES = ootest.sgml \
     ../shared/notices.sgml \
     ../shared/legalstuff.sgml \
     ../shared/gethelp.sgml \
     ../shared/CPLv1.0.sgml

OOTESTQS_SGML_FILES = ootestQuick.sgml \
     legalstuff_oot.sgml \
     ../shared/notices.sgml \
     ../shared/gethelp.sgml \
     ../shared/CPLv1.0.sgml

IMAGE_FILES = oorexx.jpg \
 	      docbook-1.png \
	      new_rexxla.jpg

JADETEXFMT  = ./pdfjadetex.fmt
HYPERREFCFG = ./hyperref.cfg
TEXMF       = /usr/share/texmf

export TEXFORMATS=.:


all: pdf html

pdf: ootest.pdf ootestQuickStart.pdf

html: ootest-html.zip

ootest.pdf: $(IMAGE_FILES) $(STYLESHEET) $(JADETEXFMT) ootest_index.sgml svnrev.tmp
	docbook2pdf -d ../shared/oorexx.print.dsl ootest.sgml

ootestQuickStart.pdf: $(IMAGE_FILES) $(STYLESHEET) $(JADETEXFMT) ootestQuick_index.sgml svnrev.tmp
	docbook2pdf -d ../shared/oorexx.print.dsl ootestQuick.sgml

book1.htm: $(IMAGE_FILES) $(HTML_STYLESHEET) ootest_index.sgml svnrev.tmp
	docbook2html -d ../shared/oorexx.html.dsl ootest.sgml

ootest-html.zip: book1.htm
	zip ootest-html *.htm *.jpg

$(JADETEXFMT): $(HYPERREFCFG)
	pdftex -ini \&pdflatex pdfjadetex.ini

ootest_index.sgml: $(HTML_STYLESHEET) $(OOTEST_SGML_FILES) svnrev.tmp
	collateindex.pl -N -o ootest_index.sgml
	jade -t sgml -d ../shared/oorexx.html.dsl -V html-index ootest.sgml
	rm *.htm
	collateindex.pl -p -g -o ootest_index.sgml HTML.index

ootestQuick_index.sgml: $(HTML_STYLESHEET) $(OOTESTQS_SGML_FILES) svnrev.tmp
	collateindex.pl -N -o ootestQuick_index.sgml
	jade -t sgml -d ../shared/oorexx.html.dsl -V html-index ootestQuick.sgml
	rm *.htm
	collateindex.pl -p -g -o ootestQuick_index.sgml HTML.index

svnrev.tmp:
	svnversion > svnrev.tmp


clean:
	@rm -f *.log *.aux *.out *.fmt *.pdf ootest_index.sgml *.htm *.zip \
           HTML.index *.tmp ootestQuick_index.sgml

