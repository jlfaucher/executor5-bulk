#!/bin/sh
# -----------------------------------------------------------------------------
#
# Copyright (c) 2020-2022 Rexx Language Association. All rights reserved.
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
# Name: doc2html.sh
# Type: macOS bash shell script
#
# Transform an ooRexx Docbook XML file to chunked HTML files using a modified
# Docbook stylesheet
#
# To trace the steps
# set -x
# Change made 2022-12-19 .zip changed to -html.zip -> for Release candidate
# Change needed also in BuildandUploadDocs.rex

if [ -z "$1" ]; then
  echo
  echo Syntax: $0 book_to_build
  echo enter the name of a book and try again!
  echo
  exit 2
fi

# Check that local SVN copy exists
if [ -z "$docpath" ] || [ ! -d ${docpath} ]; then
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
echo
echo Transforming $whichdoc to HTML $(date +%H:%M:%S)

# make XML_CATALOG_FILES known to Xsltproc
export XML_CATALOG_FILES=/usr/local/etc/xml/catalog

# set up paths and filenames to be used
# Most of this is done in docprep on Windows
inpath=${docpath}/${whichdoc}/en-US
indoc=${inpath}/${whichdoc}.xml
log_files=$PWD/log_files
log_file=${log_files}/${whichdoc}_doc2html.log
common_source=${docpath}/oorexx/en-US
common_target=${inpath}/Common_Content
html_folders=$PWD/html_folders
base_dir=${html_folders}/${whichdoc}
output_dir=$PWD/html_zipfiles

if [ ! -d "$html_folders" ]; then
  mkdir $html_folders
fi

# create the basefolder for html if not existing; remove old files if existing
# avoid doc2html.sh: line 97: /bin/rm: Argument list too long
# by simply deleting and recreating the directory every time
if [ ! -d "$base_dir" ]; then
  mkdir $base_dir
else
  rm -dfr ${base_dir}
  mkdir $base_dir
fi

# Create output folder if not existing;remove old file, if existing
if [ ! -d "$output_dir" ]; then
  mkdir $output_dir
else
  rm -dfr ${output_dir}/${whichdoc}.*
fi

# copy the common content to the book
if [ ! -d "$common_target" ]; then
  mkdir $common_target
fi
cp -R ${common_source}/ ${common_target}/

# Create logfile folder if not existing; delete old logfile
if [ ! -d "$log_files" ]; then
  mkdir $log_files
else
  rm -dfr $log_file
fi

# set the temp variables necessary for certain files
if hash rexx /q 2>/dev/null; then
  rexx -e "parse value date() with d . y;say date(W)', 'date(M) d', 'y" > $docpath/../datepub.tmp
else
  date "+%d/%m/%Y" >$docpath/../datepub.tmp
fi
/usr/local/bin/svnversion $docpath >$docpath/../svnrev.tmp

# x_opt0=--debug
# x_opt0=--dumpextensions
# x_opt0=--profile
# x_opt0=--verbose
# x_opt0="--debug --dumpextensions --profile --verbose"
# x_opt0=--timing
# x_opt0=--load-trace
# x_opt0=--nonet

x_opt1="--stringparam base.dir $base_dir"
x_opt2="--stringparam package ooRexx_Documentation-Open_Object_Rexx-5.0-en-US-0-0"
x_opt3="--stringparam prod.url https://www.oorexx.org/"
x_opt4="--stringparam xsltproc.version 1.1.29"

x_opts="--xinclude $x_opt0 $x_opt1 $x_opt2 $x_opt3 $x_opt4 $xslt_opts"

/usr/bin/xsltproc $x_opts html.xsl $indoc 1>$log_file 2>&1

cpf1=${common_source}/css
cpf2=${common_source}/images

cpt=${base_dir}/Common_Content

if [ ! -d "$cpt" ]; then
  mkdir $cpt
else
  rm -dfr ${cpt}/*
fi

cp -R $cpf1 $cpt
cp -R $cpf2 $cpt

if [ -d "${inpath}/images" ]; then
  cp -R ${inpath}/images ${base_dir}/images
fi

# Zip
echo zip and move to $output_dir
currDir=`pwd`
cd $html_folders
# replaced zip with 7zip (sevenzip) 2022-12-21 to enable unzip on Windows
# zip -r -q ${output_dir}/${whichdoc}-html.zip ./${whichdoc}/.
/usr/local/bin/7zz a ${output_dir}/${whichdoc}-html.zip ./${whichdoc}/.
cd $currDir
echo Transformation to HTML finished $(date +%H:%M:%S)
