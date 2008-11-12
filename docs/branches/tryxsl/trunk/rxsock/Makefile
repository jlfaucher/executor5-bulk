#----------------------------------------------------------------------------
#
# Description: Makefile for the Object Rexx Sockets Library Documents.
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


STYLESHEET = /usr/share/sgml/docbook/xsl-stylesheets/fo/docbook.xsl \
             ../shared/oorexx.xsl
HTML_STYLESHEET = /usr/share/sgml/docbook/xsl-stylesheets/html/docbook.xsl \
             ../shared/oorexx.xsl



SGML_FILES = rxsock.sgml \
     ../shared/notices.sgml \
     ../shared/legalstuff.sgml \
     ../shared/gethelp.sgml \
     ../shared/CPLv1.0.sgml \
     socketclass.sgml

IMAGE_FILES = oorexx.jpg \
 	      docbook-1.png \
	      new_rexxla.jpg \
	      HostInfo.png \
	      InetAddress.png \
	      SocketClass.png

JADETEXFMT  = ./pdfjadetex.fmt
HYPERREFCFG = ./hyperref.cfg
TEXMF       = /usr/share/texmf

export TEXFORMATS=.:


all: pdf html

pdf: rxsock.pdf

html: rxsock-html.zip

rxsock.pdf: rxsock.fo
	pdftex --interaction nonstopmode "&pdfxmltex" rxsock.sgml
	pdftex --interaction nonstopmode "&pdfxmltex" rxsock.sgml

rxsock.fo: $(IMAGE_FILES) $(STYLESHEET) $(JADETEXFMT) svnrev.tmp
	xsltproc --output rxsock.fo --stringparam passivetex.extensions 1 \
	         --stringparam use.extensions 0 \
	         ../shared/oorexx.xsl rxsock.sgml

book1.htm: $(IMAGE_FILES) $(HTML_STYLESHEET) svnrev.tmp
	docbook2html -d ../shared/oorexx.html.dsl rxsock.sgml

rxsock-html.zip: book1.htm
	zip rxsock-html *.htm *.jpg

$(JADETEXFMT): $(HYPERREFCFG)
	pdftex -ini \&pdflatex pdfjadetex.ini

svnrev.tmp:
	svnversion > svnrev.tmp




clean:
	rm *.log *.aux *.out *.fmt *.pdf *.fo *.htm *.zip \
           HTML.index *.tmp

