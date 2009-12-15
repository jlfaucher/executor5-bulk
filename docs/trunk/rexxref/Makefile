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


STYLESHEET = /usr/share/sgml/docbook/dsssl-stylesheets/print/docbook.dsl \
             ../shared/oorexx.print.dsl
HTML_STYLESHEET = /usr/share/sgml/docbook/dsssl-stylesheets/html/docbook.dsl \
             ../shared/oorexx.html.dsl


SGML_FILES = collclasses.sgml \
	     condtra.sgml \
	     classes.sgml \
	     streamclasses.sgml \
	     utilityclasses.sgml \
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
	     xconcur.sgml \
	     xintdeb.sgml \
	     collectionclassmethods.sgml \
	     mapcollectionclassmethods.sgml \
	     orderedcollectionclassmethods.sgml \
	     ../shared/legalstuff.sgml \
	     ../shared/notices.sgml \
	     ../shared/gethelp.sgml \
	     ../shared/CPLv1.0.sgml

HTML_IMAGE_FILES = ooRexxObjectClass.png \
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
              ooRexxCaselessDescendingComparatorClass.png \
              ooRexxDescendingComparatorClass.png \
              ooRexxInvertingComparatorClass.png \
              ooRexxIteratorClass.png \
              ooRexxCircularIteratorClass.png \
              ooRexxStreamSupplierClass.png \
              ooRexxRexxQueueClass.png \
              ooRexxDateTimeClass.png \
              ooRexxTimeSpanClass.png \
              ooRexxClasses1.png \
              ooRexxClasses2.png \
              ooRexxClasses3.png \
              ooRexxClasses4.png \
              ooRexxClasses5.png \
              ooRexxClasses6.png \
              ooRexxClasses7.png \
              doloopconcept.png \
              doloopcollection.png \
              parseconcepts1.png \
              parseconcepts2.png \
              parseconcepts3.png \
              filerecord.png \
              templates1.png \
              templates2.png \
              templates3.png \
              ReportObject.png \
	      BallObject.png \
	      BallObjectData.png \
	      EncapsulatedObject.png \
	      SimpleClass.png \
	      IconClass.png \
	      IconClassInstances.png \
	      Subclasses.png \
	      ScreenObject.png \
	      MultipleInheritance.png \
	      Modules.png \
	      MultipleRequires.png \
	      EarlyReply.png \
	      BeforeReply.png \
	      AfterReply.png \
	      IndirectRecursion.png \
              externalFunction.png \
              externalFileSearch.png \
 	      oorexx.jpg \
 	      docbook-1.png \
	      new_rexxla.jpg

PDF_IMAGE_FILES = ooRexxObjectClass.pdf \
              ooRexxClassClass.pdf \
              ooRexxMethodClass.pdf \
              ooRexxMessageClass.pdf \
              ooRexxStringClass.pdf \
              ooRexxArrayClass.pdf \
              ooRexxCollectionsClass.pdf \
              ooRexxOrderedCollectionClass.pdf \
              ooRexxCircularQueueClass.pdf \
              ooRexxListClass.pdf \
              ooRexxQueueClass.pdf \
              ooRexxDirectoryClass.pdf \
              ooRexxPropertiesClass.pdf \
              ooRexxStemClass.pdf \
              ooRexxMapCollectionClass.pdf \
              ooRexxRelationClass.pdf \
              ooRexxTableClass.pdf \
              ooRexxSetCollectionClass.pdf \
              ooRexxSetClass.pdf \
              ooRexxBagClass.pdf \
              ooRexxStreamClass.pdf \
              ooRexxInputStreamClass.pdf \
              ooRexxInputOutputStreamClass.pdf \
              ooRexxOutputStreamClass.pdf \
              ooRexxComparableClass.pdf \
              ooRexxAlarmClass.pdf \
              ooRexxMonitorClass.pdf \
              ooRexxMutableBufferClass.pdf \
              ooRexxRegularExpressionClass.pdf \
              ooRexxSupplierClass.pdf \
              ooRexxComparatorClass.pdf \
              ooRexxCaselessColumnComparatorClass.pdf \
              ooRexxColumnComparatorClass.pdf \
              ooRexxCaselessComparatorClass.pdf \
              ooRexxCaselessDescendingComparatorClass.pdf \
              ooRexxDescendingComparatorClass.pdf \
              ooRexxInvertingComparatorClass.pdf \
              ooRexxIteratorClass.pdf \
              ooRexxCircularIteratorClass.pdf \
              ooRexxStreamSupplierClass.pdf \
              ooRexxRexxQueueClass.pdf \
              ooRexxDateTimeClass.pdf \
              ooRexxTimeSpanClass.pdf \
              ooRexxClasses1.pdf \
              ooRexxClasses2.pdf \
              ooRexxClasses3.pdf \
              ooRexxClasses4.pdf \
              ooRexxClasses5.pdf \
              ooRexxClasses6.pdf \
              ooRexxClasses7.pdf \
              doloopconcept.pdf \
              doloopcollection.pdf \
              parseconcepts1.pdf \
              parseconcepts2.pdf \
              parseconcepts3.pdf \
              filerecord.pdf \
              templates1.pdf \
              templates2.pdf \
              templates3.pdf \
              ReportObject.pdf \
	      BallObject.pdf \
	      BallObjectData.pdf \
	      EncapsulatedObject.pdf \
	      SimpleClass.pdf \
	      IconClass.pdf \
	      IconClassInstances.pdf \
	      Subclasses.pdf \
	      ScreenObject.pdf \
	      MultipleInheritance.pdf \
	      Modules.pdf \
	      MultipleRequires.pdf \
	      EarlyReply.pdf \
	      BeforeReply.pdf \
	      AfterReply.pdf \
	      IndirectRecursion.pdf \
              externalFunction.pdf \
              externalFileSearch.pdf \
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

rexxref.pdf: $(PDF_IMAGE_FILES) $(STYLESHEET) $(JADETEXFMT) genindex.sgml svnrev.tmp
	docbook2pdf -d ../shared/oorexx.print.dsl rexxref.sgml

book1.htm: $(HTML_IMAGE_FILES) $(HTML_STYLESHEET) genindex.sgml svnrev.tmp
	docbook2html -d ../shared/oorexx.html.dsl rexxref.sgml

rexxref-html.zip: book1.htm
	zip rexxref-html *.htm *.jpg *.png

$(JADETEXFMT): $(HYPERREFCFG)
	pdftex -ini \&pdflatex pdfjadetex.ini

genindex.sgml: $(HTML_STYLESHEET) $(SGML_FILES) svnrev.tmp
	collateindex.pl -N -o genindex.sgml
	jade -t sgml -d ../shared/oorexx.html.dsl -V html-index rexxref.sgml
	rm *.htm
	collateindex.pl -p -g -o genindex.sgml HTML.index

svnrev.tmp:
	svnversion > svnrev.tmp




clean:
	rm -f *.log *.aux *.out *.fmt rexxref.pdf genindex.sgml *.htm *.zip \
           HTML.index *.tmp

