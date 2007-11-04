#! /bin/bash
#/*----------------------------------------------------------------------------*/
#/*                                                                            */
#/* Copyright (c) 2007 Rexx Language Association. All rights reserved.         */
#/*                                                                            */
#/* This program and the accompanying materials are made available under       */
#/* the terms of the Common Public License v1.0 which accompanies this         */
#/* distribution. A copy is also available at the following address:           */
#/* http://www.oorexx.org/license.html                                         */
#/*                                                                            */
#/* Redistribution and use in source and binary forms, with or                 */
#/* without modification, are permitted provided that the following            */
#/* conditions are met:                                                        */
#/*                                                                            */
#/* Redistributions of source code must retain the above copyright             */
#/* notice, this list of conditions and the following disclaimer.              */
#/* Redistributions in binary form must reproduce the above copyright          */
#/* notice, this list of conditions and the following disclaimer in            */
#/* the documentation and/or other materials provided with the distribution.   */
#/*                                                                            */
#/* Neither the name of Rexx Language Association nor the names                */
#/* of its contributors may be used to endorse or promote products             */
#/* derived from this software without specific prior written permission.      */
#/*                                                                            */
#/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS        */
#/* "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT          */
#/* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS          */
#/* FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT   */
#/* OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,      */
#/* SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED   */
#/* TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,        */
#/* OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY     */
#/* OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING    */
#/* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS         */
#/* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.               */
#/*                                                                            */
#/*----------------------------------------------------------------------------*/
#
# Script to set things up to correctly build a .deb package of ooRexx, and to
# actually build it.  This is very much a 'Work In Progress' file.  Eventually,
# much of the function in this script may be integrated into the regular
# Makefile used in the ooRexx build.  In the mean time it provides a relatively
# easy method to build .deb packages on Debian Sarge and K/Ubuntu.
#
# ./deb_build MODE [buildDir]  or ./deb_build for syntax help

# Will only work for a 3.1.2 or 3.2.0 build without editing.

# Source in the functions for this script
. ./deb_build.funcs

dst_dir=/opt/ooRexx

# checkArgs() sets src_dir and determines mode (from the arguments.)
# For mode: 0 == bootstrap 1 == all 2 == config 3 == build 4 == package
# Anything else is an error.
src_dir=""
checkArgs $@
mode=$?
if [ $mode -gt 4 ]; then
  # In reality we do not get here.
  exit 255
fi

# checkEnv() sets platform, ensures this is a Debian system, and that src_dir
# exists.
platform=""
checkEnv
if [ $? -ne 0 ]; then
  exit 255
fi

# Configure and build need to be done as not root.  Package and clean
# need to be root.  fakeroot is used to do the root stuff.  We are now
# reasonable sure we have the correct environment.
#
# The following are the essential build and package steps:

# 1.  Set up the debian subdirectory with its files
if [ $mode -lt 2 ]; then
  destDir=${src_dir}/debian/
  mkdir $destDir

  # Apparently the changelog format is very rigid
  echo "oorexx (${version}-1) stable; urgency=medium" > ${destDir}changelog
  echo "" >> ${destDir}changelog
  echo "            Fixes in Open Object Rexx ${version}" >> ${destDir}changelog
  echo " -- Mark Miesfeld <miesfeld@users.sourceforge.net>  Mon, 16 Jul 2007 11:42:16 -0700" >> ${destDir}changelog

  # Instead of determing if this is a Debian Sarge or a K/Ubuntu system, use
  # the lowest common denominator.
  echo "4" > ${destDir}compat
  cp debian/control.sarge ${destDir}control

  cp debian/copyright ${destDir}
  cp debian/dirs ${destDir}
  cp debian/docs ${destDir}
  cp debian/postinst ${destDir}
  cp debian/postrm ${destDir}
  cp debian/prerm ${destDir}
  cp debian/rules ${destDir}
fi

# Move to the ooRexx build directory.
cd $src_dir

# dh_testdir ensures things are initially correct to do a debian build.
dh_testdir
if [ $? -ne 0 ]; then
  exit 255
fi

# Capture the full path of the current directory.
cur_dir=`pwd`

# 2. Run bootstrap if requested.
if [ $mode -lt 1 ]; then
  ./bootstrap
fi

# 3. Configure
if [ $mode -lt 3 ]; then
  # The xalan version installed on Debian Sarge chokes, the xalan on Kubuntu
  # works.  So, use the coward's way out.
  ./touchheaders.sh

  # Probably don't need to export these, a left over from previous incantations
  export DEB_HOST_GNU_TYPE=`dpkg-architecture -qDEB_HOST_GNU_TYPE`
  export DEB_BUILD_GNU_TYPE=`dpkg-architecture -qDEB_HOST_GNU_TYPE`

  # Run the configure script with the proper args for a Debian build
  ./configure --disable-static --host=$DEB_HOST_GNU_TYPE --build=$DEB_BUILD_GNU_TYPE --prefix=$dst_dir --mandir=${dst_dir}/share/man --infodir=${dst_dir}/share/info

fi

# 4.  Build
if [ $mode -lt 4 ]; then
  make
fi

# 5. Package.  Debian packages have to be created as root.  fakeroot works.
if [ $mode -lt 5 ]; then
  # Install into the Debian package directory.
  fakeroot dh_clean -k
  fakeroot dh_installdirs
  fakeroot make install DESTDIR=${cur_dir}/debian/oorexx

  # Do the packaging steps
  fakeroot dh_installchangelogs CHANGES
  fakeroot dh_installdocs
  fakeroot dh_installexamples
  fakeroot dh_installman
  fakeroot dh_link
  fakeroot dh_strip
  fakeroot dh_compress
  fakeroot dh_fixperms
  fakeroot dh_installdeb

  # This step just produces a lot of warnings.  Not sure that this is needed.
  fakeroot dh_shlibdeps

  fakeroot dh_gencontrol
  fakeroot dh_md5sums
  fakeroot dh_builddeb
fi

