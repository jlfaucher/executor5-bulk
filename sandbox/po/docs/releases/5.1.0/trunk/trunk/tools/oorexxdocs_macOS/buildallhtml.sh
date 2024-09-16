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
# Name: buildallhtml.sh
# Type: macOS bash shell script
#
# Build all HTML manuals for ooRexx 5.0.0
#
# To trace the steps
# set -x

echo
echo Starting to launch HTML documentation builds: $(date +%H:%M:%S)

if [ -z "$docpath" ] || [ ! -d ${docpath} ]; then
  docpath=$PWD/oorexxdocSVN
  if [ ! -d "$docpath" ]; then
    echo
    echo \$docpath=\"$docpath\" does not exist, aborting
    echo
    exit 1
  fi
fi

# Get latest svn versions
echo Uppdating documentation using SVN
svn update $docpath

# note: oorexx is common content

for dir in $docpath/*/    # list SVN main directories
do
    dir=${dir%*/}      # remove the trailing "/"

    if [ "${dir##*/}" = oorexx -o "${dir##*/}" = tools ]; then
      echo NOT building "${dir##*/}"
      continue
    fi
    echo building "${dir##*/}"
    bash doc2html.sh "${dir##*/}"
done

echo Finished building HTML documentation: $(date +%H:%M:%S)
exit 0
