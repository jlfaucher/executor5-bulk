#----------------------------------------------------------------------------
#
# Description: Makefile for the Object Rexx Reference Documents.
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


STYLESHEET = /usr/share/sgml/docbook/dsssl-stylesheets/print/docbook.dsl
HTML_STYLESHEET = /usr/share/sgml/docbook/dsssl-stylesheets/html/docbook.dsl


SGML_FILES = collclasses.sgml \
	     condtra.sgml \
	     classes.sgml \
	     streamclasses.sgml \
	     utilityclasses.sgml \
	     winclasses.sgml \
	     dire.sgml \
	     errnum.sgml \
	     errnums.sgml \
	     funct.sgml \
	     fundclasses.sgml \
	     instrc.sgml \
	     intro.sgml \
	     iostrms.sgml \
	     mig.sgml \
	     numarit.sgml \
	     oneof.sgml \
	     parsingch.sgml \
	     preface.sgml \
	     provide.sgml \
	     resvkey.sgml \
	     rexutil.sgml \
	     rexxref.sgml \
	     secman.sgml \
	     spvard.sgml \
	     usepcom.sgml \
	     wsheng.sgml \
	     xconcur.sgml \
	     xintdeb.sgml \
	     ../shared/oorexx.xsl \
	     ../shared/legalstuff.sgml \
	     ../shared/notices.sgml \
	     ../shared/gethelp.sgml \
	     ../shared/CPLv1.0.sgml

IMAGE_FILES = ooRexxObjectClass.png \
              ooRexxClassClass.png \
              ooRexxMethodClass.png \
              ooRexxMessageClass.png \
              ooRexxStringClass.png \
              ooRexxArrayClass.png \
              ooRexxCollectionsClass.png \
              ooRexxOrderedCollectionClass.png \
              ooRexxCircularQueueClass.png \
              ooRexxListClass.png \
              ooRexxQueueClass.png \
              ooRexxDirectoryClass.png \
              ooRexxPropertiesClass.png \
              ooRexxStemClass.png \
              ooRexxMapCollectionClass.png \
              ooRexxRelationClass.png \
              ooRexxTableClass.png \
              ooRexxSetCollectionClass.png \
              ooRexxSetClass.png \
              ooRexxBagClass.png \
              ooRexxStreamClass.png \
              ooRexxInputStreamClass.png \
              ooRexxInputOutputStreamClass.png \
              ooRexxOutputStreamClass.png \
              ooRexxComparableClass.png \
              ooRexxAlarmClass.png \
              ooRexxMonitorClass.png \
              ooRexxMutableBufferClass.png \
              ooRexxRegularExpressionClass.png \
              ooRexxSupplierClass.png \
              ooRexxComparatorClass.png \
              ooRexxCaselessColumnComparatorClass.png \
              ooRexxColumnComparatorClass.png \
              ooRexxCaselessComparatorClass.png \
              ooRexxDescendingCaselessComparatorClass.png \
              ooRexxDescendingComparatorClass.png \
              ooRexxInvertingComparatorClass.png \
              rxo90004.jpg \
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
	      rxor0f01.jpg \
	      rxor0f02.jpg \
	      rxor0f03.jpg \
	      rxor0f04.jpg \
	      rxor0f05.jpg \
	      rxor0f06.jpg \
	      rxor0f07.jpg \
	      rxor0f08.jpg \
	      rxor0f09.jpg \
	      rxor0f10.jpg \
	      rxor0f11.jpg \
	      rxor0f12.jpg \
	      rxor0f13.jpg \
	      rxor0f20.jpg \
	      rxor0f21.jpg \
 	      oorexx.jpg \
 	      docbook-1.png \
	      new_rexxla.jpg

JADETEXFMT  = ./pdfjadetex.fmt
HYPERREFCFG = ./hyperref.cfg
TEXMF       = /usr/share/texmf

export TEXFORMATS=.:


all: pdf html

pdf: rexxref.pdf

html: rexxref-html.zip

rexxref.pdf: $(IMAGE_FILES) $(STYLESHEET) $(JADETEXFMT) genindex.sgml
	docbook2pdf -d ../shared/oorexx.dsl rexxref.sgml

book1.htm: $(IMAGE_FILES) $(HTML_STYLESHEET) genindex.sgml
	docbook2html -d $(HTML_STYLESHEET) rexxref.sgml

rexxref-html.zip: book1.htm
	zip rexxref-html *.htm *.jpg

$(JADETEXFMT): $(HYPERREFCFG)
	pdftex -ini \&pdflatex pdfjadetex.ini

genindex.sgml: $(HTML_STYLESHEET) $(SGML_FILES)
	collateindex.pl -N -o genindex.sgml
	jade -t sgml -d $(HTML_STYLESHEET) -V html-index rexxref.sgml
	rm *.htm
	collateindex.pl -p -g -o genindex.sgml HTML.index




clean:
	rm *.log *.aux *.out *.fmt *.pdf genindex.sgml *.htm *.zip HTML.index

