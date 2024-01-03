#!/bin/sh
# -----------------------------------------------------------------------------
#
# Copyright (c) 2020-2024, Rexx Language Association. All rights reserved.
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
# ------------------------------------------------------------------------------
#
# Name: doc2fo.sh
# Type: macOS bash shell script
#
# Transform an ooRexx Docbook XML file to XSL-FO using a modified
# Docbook stylesheet
#
# To trace the steps
# set -x

if [ -z "$1" ]; then
  echo
  echo Syntax: $0 book_to_build
  echo enter the name of a book and try again!
  echo
  exit 2
fi

# Check that local SVN copy exists
if [ -z "$docpath" ] || [! -d ${docpath} ]; then
  docpath=$PWD/oorexxdocSVN
  if [ ! -d "$docpath" ]; then
    echo
    echo \$docpath=\"$docpath\" does not exist, aborting
    echo
    exit 1
  fi
fi

# set up book to be built
whichdoc=$1
echo Starting to build ${whichdoc}.fo $(date +%H:%M:%S)

# make XML_CATALOG_FILES known to Xsltproc
export XML_CATALOG_FILES=/usr/local/etc/xml/catalog

# set up paths and filenames to be used
# Most of this is done in docprep on Windows
inpath=${docpath}/${whichdoc}/en-US
indoc=${inpath}/${whichdoc}.xml
log_files=$PWD/log_files
log_file=${log_files}/${whichdoc}_doc2fo.log
common_source=${docpath}/oorexx/en-US
common_target=${inpath}/Common_Content
output_dir=$PWD/fo_files
output_file=${output_dir}/${whichdoc}.fo

# Create output folder if not existing; delete any existing fo file
if [ ! -d "$output_dir" ]; then
  mkdir $output_dir
else
  rm -dfr $output_file
fi

# copy the common content to the book
if [ ! -d "$common_target" ]; then
  mkdir $common_target
fi

cp -R ${common_source}/ ${common_target}/
cp $PWD/fop.cfg  ${common_target}/fop.cfg

# Create logfile folder if not existing; delete old logfile
if [ ! -d "$log_files" ]; then
  mkdir $log_files
else
  rm -dfr $log_file
fi

echo Starting xsltproc $(date +%H:%M:%S)

# set the temp variables necessary for certain files
if hash rexx /q 2>/dev/null; then
  rexx -e "parse value date() with d . y;say date(W)', 'date(M) d', 'y" > $docpath/../datepub.tmp
else
  date "+%d/%m/%Y" >$docpath/../datepub.tmp
fi
/usr/local/bin/svnversion $docpath >$docpath/../svnrev.tmp

# Finally run conversion to .fo
# xslt_opts=--debug
# xslt_opts=--dumpextensions
# xslt_opts=--profile
# xslt_opts=--verbose
# xslt_opts="--debug --dumpextensions --profile --verbose --load-trace"
# xslt_opts= --timing
# xslt_opts=--load-trace
# xslt_opts=--nonet

/usr/bin/xsltproc $xslt_opts --xinclude --output $output_file pdf.xsl $indoc 1>$log_file 2>&1

echo xsltproc finished $(date +%H:%M:%S)
