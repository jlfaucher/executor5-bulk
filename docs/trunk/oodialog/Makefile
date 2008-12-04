#----------------------------------------------------------------------------
#
# Description: Makefile for the Object Rexx OODialog Reference PDF
#
# Copyright (c) 2005-2007, Rexx Language Association. All rights reserved.
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


SGML_FILES = advcntlc.sgml \
	     basedialog.sgml \
	     buttoncontrols.sgml \
	     categorydialog.sgml \
	     comboboxc.sgml \
	     dialogcontrolc.sgml \
	     editcontrolc.sgml \
	     listboxc.sgml \
	     listcontrolc.sgml \
	     mesextcl.sgml \
	     utilityclasses.sgml \
	     oodialog.sgml \
	     plainuserdialogc.sgml \
	     preface.sgml \
	     progressbarc.sgml \
	     propertysheetc.sgml \
	     reference.sgml \
	     resdialog.sgml \
	     scrollbarc.sgml \
	     slidercontrolc.sgml \
	     staticcontrolc.sgml \
	     tabcontrolc.sgml \
	     termdef.sgml \
	     treecontrolc.sgml \
	     userdialog.sgml \
	     ../shared/notices.sgml \
	     ../shared/legalstuff.sgml \
	     ../shared/gethelp.sgml \
	     ../shared/CPLv1.0.sgml
# the following files are not used & are here only for historical reasons
#	     conceptoverv.sgml \
#	     graphui.sgml \
#	     tokenizescripts.sgml \
#	     tutresworkshop.sgml \
#	     usingdialog.sgml \



IMAGE_FILES = rxou0s12.jpg \
              rxou0s17.jpg \
	      rxou0s18.jpg \
	      rxou0s19.jpg \
	      rxou0s20.jpg \
	      rxou0s24.jpg \
	      rxou0s25.jpg \
 	      docbook-1.png \
	      oorexx.jpg

JADETEXFMT  = ./pdfjadetex.fmt
HYPERREFCFG = ./hyperref.cfg
TEXMF       = /usr/share/texmf

export TEXFORMATS=.:


all: pdf html

pdf: oodialog.pdf

html: oodialog-html.zip

oodialog.pdf: $(IMAGE_FILES) $(STYLESHEET) $(JADETEXFMT) genindex.sgml svnrev.tmp
	docbook2pdf -d ../shared/oorexx.print.dsl oodialog.sgml

book1.htm: $(IMAGE_FILES) $(HTML_STYLESHEET) genindex.sgml svnrev.tmp
	docbook2html -d ../shared/oorexx.html.dsl oodialog.sgml

oodialog-html.zip: book1.htm
	zip oodialog-html *.htm *.jpg *.png

$(JADETEXFMT): $(HYPERREFCFG)
	pdftex -ini \&pdflatex pdfjadetex.ini

genindex.sgml: $(HTML_STYLESHEET) $(SGML_FILES) svnrev.tmp
	collateindex.pl -N -o genindex.sgml
	jade -t sgml -d ../shared/oorexx.html.dsl -V html-index oodialog.sgml
	rm *.htm
	collateindex.pl -p -g -o genindex.sgml HTML.index

svnrev.tmp:
	svnversion > svnrev.tmp


clean:
	rm -f *.log *.aux *.out *.fmt *.pdf genindex.sgml *.htm *.zip HTML.index *.tmp

