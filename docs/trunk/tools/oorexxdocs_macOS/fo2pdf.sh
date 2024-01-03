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
# Name: fo2pdf.sh
# Type: macOS bash shell script
#
# Create a PDF from an XSL-FO file using Apache FOP
#
# To trace the steps
# set -x

if [ -z "$1" ];
then
  echo
  echo Syntax: $0 book_to_build
  echo enter the name of a book and try again!
  echo
  exit 2
fi

if [ -z "$docpath" ] || [! -d ${docpath} ]; then
  docpath=$PWD/oorexxdocSVN
  if [ ! -d "$docpath" ]; then
    echo
    echo \$docpath=\"$docpath\" does not exist, aborting
    echo
    exit 1 
  fi
fi	

# set up book to be built and paths to be used
whichdoc=$1

common_target=${docpath}/${whichdoc}/en-US/Common_Content
input_dir=$PWD/fo_files
output_dir=$PWD/pdf_files
log_files=$PWD/log_files

# set up file names
in_file=${input_dir}/${whichdoc}.fo
out_file=${output_dir}/${whichdoc}.pdf
log_file=${log_files}/${whichdoc}_fo2pdf.log
fopcfg=${common_target}/fop.cfg

# Check that the input file exists
if [ ! -f "$in_file" ]; then
  echo
  echo Unable to find ${whichdoc}.fo
  echo Make sure the doc2fo command has run successfully!
  echo aborting $0
  echo
  exit 3
fi

echo Starting to convert ${whichdoc}.fo $(date +%H:%M:%S)

# Create output folder if not existing; delete old pdf file
if [ ! -d "$output_dir" ]; then
  mkdir $output_dir
else
  rm -dfr $out_file
fi

# Create logfile folder if not existing; delete old logfile
if [ ! -d "log_files" ]; then
  mkdir $log_files
else
  rm -dfr $log_file
fi

# Finally run fop (this setting may not be necessary)
export fop_opts="-Xmx2048M"

echo Starting Apache FOP $(date +%H:%M:%S)
/usr/local/bin/fop -c $fopcfg $in_file $out_file 2>>$log_file

echo Apache FOP finished $(date +%H:%M:%S)
