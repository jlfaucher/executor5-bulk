#!/usr/bin/env rexx
/*
# -----------------------------------------------------------------------------
#
# Copyright (c) 2024-2025 Rexx Language Association. All rights reserved.
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
# Name:  Modifybuildjobs.rex
#
# Type: ooRexx script to modify xml files defining Jenkins build&test jobs
#
# Intended to be used when releasing a new version of ooRexx; this script will
# modify the config.xml files defining the Jenkins build and test jobs
# changing the svn path to the release folder on Sourceforge and changing
# the version according to the needle -> newneedle variables below
#
# Since we run tests directly off our build tree there is no need to change
# the test jobs, but change them here anyhow
#
# ------------------------------------------------------------------------------
*/

trace o

-- If rexx is not installed call it from the build dir
-- /Users/jenkins/workspace/ooRexx-macOS-build/oorexxBuild/bin/rexx

--  needle    = enter here the beta version that is currently used
--  newneedle = enter here the ooRexx version to release, rc = release candidate

-- needle1 for all build jobs, also for source package build
  needle1     ='<remote>https://svn.code.sf.net/p/oorexx/code-0/main/trunk</remote>'
--  newneedle1  ='<remote>https://svn.code.sf.net/p/oorexx/code-0/main/branches/5.1/trunk</remote>'
  newneedle1  ='<remote>https://svn.code.sf.net/p/oorexx/code-0/main/releases/5.1.0/trunk</remote>'

-- needle 2 and needle3 are for the documentation build
-- remember we must build the complete release documentation once before we build the installers
  needle2     ='<remote>https://svn.code.sf.net/p/oorexx/code-0/docs/trunk</remote>'
--  newneedle2  ='<remote>https://svn.code.sf.net/p/oorexx/code-0/docs/branches/5.1/trunk</remote>'
  newneedle2  ='<remote>https://svn.code.sf.net/p/oorexx/code-0/docs/releases/5.1.0/trunk</remote>'

  needle3     ='rexx BuildandUploadDocs.rex svn.code.sf.net/p/oorexx/code-0/docs/trunk /home/frs/project/oorexx/oorexx-docs/5.2.0beta'
--  newneedle3  ='rexx BuildandUploadDocs.rex svn.code.sf.net/p/oorexx/code-0/docs/branches/5.1/trunk /home/frs/project/oorexx/oorexx-docs/5.1.1'
  newneedle3  ='rexx BuildandUploadDocs.rex svn.code.sf.net/p/oorexx/code-0/docs/releases/5.1.0/trunk /home/frs/project/oorexx/oorexx-docs/5.1.1'

-- ooRexx-sourceforge-upload and ooRexx-sourceforge-zip-installer-upload can take 3 arguments:
-- the upload folder tail part
-- the version number indicated in the filename
-- the min revision, set to release number
  needle4     ='rexx jenkinsArtifactUpload.rex 5.2.0beta 5.2.0 0'
--  newneedle4  ='rexx jenkinsArtifactUpload.rex 5.1.1 5.1.1 12973'
  newneedle4  ='rexx jenkinsArtifactUpload.rex 5.1.1 5.1.0 12973'
  needle5     ='rexx jenkinsZIPArtifactUpload.rex 5.2.0beta 5.2.0 0'
--  newneedle5  ='rexx jenkinsZIPArtifactUpload.rex 5.1.1 5.1.1 12973'
  newneedle5  ='rexx jenkinsZIPArtifactUpload.rex 5.1.1 5.1.0 12973'

-- ooRexx tests jobs new 2025-04-30
  needle6     ='<remote>https://svn.code.sf.net/p/oorexx/code-0/test/trunk</remote>'
--  newneedle6  ='<remote>https://svn.code.sf.net/p/oorexx/code-0/test/branches/5.1/trunk</remote>'
  newneedle6  ='<remote>https://svn.code.sf.net/p/oorexx/code-0/test/releases/5.1.0/trunk</remote>'

-- Add here distinctive parts of build that are NOT part of the ooRexx release
  IgnoreItems = '-configuration_best_practises- Android.Build crexx_aarch64_linux Jenkins-Environment-Variables-List NetRexx-aarch64-jvm11-build regina-rexx-bsd-build regina-rexx-bsd-build-delete regina-rexx-deb-build regina-rexx-macos-build regina-rexx-rpm-build throttle_test ooRexx-docs-buildutils-check'

  buildArray  = .array~new	-- Build jobs
  testArray   = .array~new	-- Test jobs
  docArray    = .array~new	-- Documentation build job (should only be one)
  sourceArray = .array~new	-- Source file build (should only be one)
  UploadArray = .array~new	-- Uploader script (should only be one)
  ZipUpArray  = .array~new	-- Uploader script for zipped items (should only be one)

/*
Search Jenkins local subdirectories for build/test/doc jobs
This is the actual path on Jenkins buildmachine (the Controller)
BuildjobsPath = "/var/lib/jenkins/jobs"
*/

BuildjobsPath = "/var/lib/jenkins/jobs/"

/*
To run in another place, run first the script jenkinsLogs.sh on the Controller, 
copy the file jenkinsLogs.zip to a workdir, unpack it, enable the following two
lines and run Modifybuildjobs.rex from within the workdir.
*/
  res = SysFileTree('*.*', 'file', 'FO')
  cwd = filespec('path',file.1)     -- trailing slash for cwd

  say 'cwd' cwd
  BuildjobsPath = cwd || BuildjobsPath~subStr(2)
  say 'BuildjobsPath' BuildjobsPath

  res =SysFileTree(BuildjobsPath, dir., "DO")
  say
  say dir.0 'Builddirs found:'

  DO i=1 TO dir.0
    DO j=1 TO IgnoreItems~words
      IF dir.i~caselessPos(IgnoreItems~word(j)) > 0 THEN
      DO
        say 'Skipping' filespec('name',dir.i)
        Iterate i
      END
    END j
    -- say 'Saving' filespec('name',dir.i)

    SELECT
-- docs-build MUST come before build, you have been warned
-- source-package-build MUST come before build, you have been warned
      WHEN dir.i~caselessPos('docs-build') > 0 				THEN docArray~append(dir.i || '/config.xml')
      WHEN dir.i~caselessPos('source-package') > 0 			THEN sourceArray~append(dir.i || '/config.xml')
      WHEN dir.i~caselessPos('sourceforge-upload') > 0 			THEN UploadArray~append(dir.i || '/config.xml')
      WHEN dir.i~caselessPos('sourceforge-zip-installer-upload') > 0	THEN ZipUpArray~append(dir.i || '/config.xml')
      WHEN dir.i~caselessPos('build') > 0 				THEN buildArray~append(dir.i || '/config.xml')
      WHEN dir.i~caselessPos('test') > 0 				THEN testArray~append(dir.i || '/config.xml')
      OTHERWISE
      DO
        say
        say 'WHOOA! Unknown type, add to list of IgnoreItems!'
        say dir.i
        say
      END
    END
  END i

say
say docArray~items  'docArray items:'
DO item over docArray
--  say item
END

say
say sourceArray~items 'sourceArray items:'
DO item over sourceArray
--  say item
END

say
say UploadArray~items 'UploadArray items:'
DO item over UploadArray
--  say item
END

say
say ZipUpArray~items 'ZipUpArray items:'
DO item over ZipUpArray
--  say item
END

say
say buildArray~items 'buildArray items:'
DO item over buildArray
--  say item
END

say
say testArray~items 'testArray items:'
DO item over testArray
--  say item
END

/**** This is for Documentation build job	****/

-- Remember to build the complete release documentation once before we build the installers!
  IF docArray~items > 1 THEN
  DO
    say
    say 'FATAL, more than one Documentation job, please investigate'
    say 'Aborting'
    Exit
  END
  ELSE
  DO
    say
    say docArray~items 'Documentation job to process:'
  END

-- there is only one item in the docArray
  xmlfile = docArray[1]
  say
  -- say xmlfile
-- store away original
  res = SysFileCopy(xmlfile, xmlfile~changeStr('.xml','.bup'))

-- read the entire file
  qfile = .stream~new(xmlfile)
  qfile~open("read shared")
  myArray = qfile~arrayIn()
  qfile~close

-- and rewrite with newNeedle where needle is found
  qfile = .stream~new(xmlfile)
  res = qfile~open('write replace')

-- iterate over file and replace those part referring to needle with newneedle
  DO item OVER myArray
    SELECT
      WHEN item~caselessPos(needle2) > 0 THEN
      DO
        say 'found' needle2
        say 'replacing with' newneedle2
        res = qfile~lineOut(item~changeStr(needle2,newneedle2))
      END
      WHEN item~caselessPos(needle3) > 0 THEN
      DO
        say 'found' needle3
        say 'replacing with' newneedle3
        res = qfile~lineOut(item~changeStr(needle3,newneedle3))
      END
      OTHERWISE res = qfile~lineOut(item)	-- untouched line
    END /* Select */
  END
  qfile~close

/**** This is for the Source-Package build job	****/

  IF sourceArray~items > 1 THEN
  DO
    say
    say 'FATAL, more than one Source-Package job, please investigate'
    say 'Aborting'
    Exit
  END
  ELSE
  DO
    say
    say sourceArray~items 'Source-Package job to process:'
  END

-- there is only one item in the sourceArray
  xmlfile = sourceArray[1]
  say
    -- say xmlfile
-- store away original
    res = SysFileCopy(xmlfile, xmlfile~changeStr('.xml','.bup'))

-- empty the build directory, tedious but necessary
    res = SysFileDelete(xmlfile~changeStr('config.xml','nextBuildNumber'))

    tmpDir = fileSpec('Path',xmlfile) || .File~separator || 'builds' || .File~separator || '*'
    res =SysFileTree(tmpDir,tmpDir2, "DOS")	-- all dirs
    res =SysFileTree(tmpDir,tmpFile, "FOS")	-- all files
-- say 'deleting' tmpFile.0 'files'
    DO i=1 TO tmpFile.0
      res = SysFileDelete(tmpFile.i)
--say 'res=' res tmpFile.i
    END i

-- say 'deleting' tmpDir2.0 'dirs'
    DO i=tmpDir2.0 TO 1 BY -1
      res = SysRmDir(tmpDir2.i)
--say 'i=' i 'res=' res tmpDir2.i
    END i

-- read the entire file
  qfile = .stream~new(xmlfile)
  qfile~open("read shared")
  myArray = qfile~arrayIn()
  qfile~close

-- and rewrite with newNeedle where needle is found
  qfile = .stream~new(xmlfile)
  res = qfile~open('write replace')

-- iterate over file and replace those part referring to needle with newneedle
  DO item OVER myArray
    IF item~caselessPos(needle1) > 0 THEN
    DO
      say 'found' needle1
      say 'replacing with' newneedle1
      res = qfile~lineOut(item~changeStr(needle1,newneedle1))
    END
    ELSE res = qfile~lineOut(item)
  END
  qfile~close

/**** This is for the sourceforge-upload job	****/

  IF UploadArray~items > 1 THEN
  DO
    say
    say 'FATAL, more than one {sourceforge-upload} job, please investigate'
    say 'Aborting'
    Exit
  END
  ELSE
  DO
    say
    say UploadArray~items 'sourceforge-upload job to process:'
  END

-- there is only one item in the UploadArray
  xmlfile = UploadArray[1]
  say
  -- say xmlfile
-- store away original
  res = SysFileCopy(xmlfile, xmlfile~changeStr('.xml','.bup'))

-- read the entire file
  qfile = .stream~new(xmlfile)
  qfile~open("read shared")
  myArray = qfile~arrayIn()
  qfile~close

-- and rewrite with newNeedle where needle is found
  qfile = .stream~new(xmlfile)
  res = qfile~open('write replace')

-- iterate over file and replace those part referring to needle with newneedle
  DO item OVER myArray
    IF item~caselessPos(needle4) > 0 THEN
    DO
      say 'found' needle4
      say 'replacing with' newneedle4
      res = qfile~lineOut(item~changeStr(needle4,newneedle4))
    END
    ELSE res = qfile~lineOut(item)
  END
  qfile~close

/**** This is for the sourceforge-zip-installer-upload job	****/

  IF ZipUpArray~items > 1 THEN
  DO
    say
    say 'FATAL, more than one sourceforge-zip-installer-upload job, please investigate'
    say 'Aborting'
    Exit
  END
  ELSE
  DO
    say
    say ZipUpArray~items 'sourceforge-zip-installer-upload job to process:'
  END

-- there is only one item in the ZipUpArray
  xmlfile = ZipUpArray[1]
  say
  -- say xmlfile
-- store away original
  res = SysFileCopy(xmlfile, xmlfile~changeStr('.xml','.bup'))

-- read the entire file
  qfile = .stream~new(xmlfile)
  qfile~open("read shared")
  myArray = qfile~arrayIn()
  qfile~close

-- and rewrite with newNeedle where needle is found
  qfile = .stream~new(xmlfile)
  res = qfile~open('write replace')

-- iterate over file and replace those part referring to needle with newneedle
  DO item OVER myArray
    IF item~caselessPos(needle5) > 0 THEN
    DO
      say 'found' needle5
      say 'replacing with' newneedle5
      res = qfile~lineOut(item~caselessChangeStr(needle5,newneedle5))
    END
    ELSE res = qfile~lineOut(item)
  END
  qfile~close

-- split the remaining tasks according to Platform

  buildArray~stableSort
  testArray~stableSort

  macbuildArray = .array~new
  winbuildArray = .array~new
  nixbuildArray = .array~new

  mactestArray = .array~new
  wintestArray = .array~new
  nixtestArray = .array~new

/**** This is for build jobs	****/

  say
  say buildArray~items 'Build jobs to process:'
  DO item over buildArray
--    say item
    SELECT
      WHEN item~caselessPos('macOS') > 0 THEN macbuildArray~append(item)
      WHEN item~caselessPos('windows') > 0 THEN winbuildArray~append(item)
      OTHERWISE nixbuildArray~append(item)
    END /* Select */
  END


/**** Windows build jobs	****/

  say
  say winbuildArray~items 'Windows build jobs to process:'
  DO xmlfile OVER winbuildArray
--    -- say xmlfile
-- store away original
    res = SysFileCopy(xmlfile, xmlfile~changeStr('.xml','.bup'))

-- read the entire file
    qfile = .stream~new(xmlfile)
    qfile~open("read shared")
    myArray = qfile~arrayIn()
    qfile~close

-- and rewrite with newNeedle where needle is found
    qfile = .stream~new(xmlfile)
    res = qfile~open('write replace')

-- iterate over file and replace those part referring to needle with newneedle
    DO item OVER myArray
      SELECT
        WHEN item~caselessPos(needle1) > 0 THEN
        DO
          say 'found' needle1
          say 'replacing with' newneedle1
          res = qfile~lineOut(item~changeStr(needle1,newneedle1))
        END
        OTHERWISE  res = qfile~lineOut(item)
      END /* Select */
    END
    qfile~close
  END

/**** MacOS build jobs	****/

  say
  say macbuildArray~items 'macOS build jobs to process:'
  DO xmlfile OVER macbuildArray
    -- say xmlfile
-- store away original
    res = SysFileCopy(xmlfile, xmlfile~changeStr('.xml','.bup'))

-- read the entire file
    qfile = .stream~new(xmlfile)
    qfile~open("read shared")
    myArray = qfile~arrayIn()
    qfile~close

-- and rewrite with newNeedle where needle is found
    qfile = .stream~new(xmlfile)
    res = qfile~open('write replace')

-- iterate over file and replace those part referring to needle with newneedle
    DO item OVER myArray
      SELECT
        WHEN item~caselessPos(needle1) > 0 THEN
        DO
          say 'found' needle1
          say 'replacing with' newneedle1
          res = qfile~lineOut(item~changeStr(needle1,newneedle1))
        END
        OTHERWISE  res = qfile~lineOut(item)
      END /* Select */
    END
    qfile~close
  END

/**** *Nix build jobs	****/

  say
  say nixbuildArray~items '*nix build jobs to process:'
  DO xmlfile OVER nixbuildArray
    -- say xmlfile
-- store away original
    res = SysFileCopy(xmlfile, xmlfile~changeStr('.xml','.bup'))

-- read the entire file
    qfile = .stream~new(xmlfile)
    qfile~open("read shared")
    myArray = qfile~arrayIn()
    qfile~close

-- and rewrite with newNeedle where needle is found
    qfile = .stream~new(xmlfile)
    res = qfile~open('write replace')

-- iterate over file and replace those part referring to needle with newneedle
    DO item OVER myArray
      SELECT
        WHEN item~caselessPos(needle1) > 0 THEN
        DO
          say 'found' needle1
          say 'replacing with' newneedle1
          res = qfile~lineOut(item~changeStr(needle1,newneedle1))
        END
        OTHERWISE res = qfile~lineOut(item)
      END /* Select */
    END
    qfile~close
  END

/**** This is for test jobs	****/
  say
  say
  say testArray~items 'test jobs to process:'
  DO item over testArray
--    say item
    SELECT
      WHEN item~caselessPos('macOS') > 0 THEN mactestArray~append(item)
      WHEN item~caselessPos('windows') > 0 THEN wintestArray~append(item)
      OTHERWISE nixtestArray~append(item)
    END /* Select */
  END

/**** Windows test jobs	****/

  say
  say wintestArray~items 'Windows test jobs to process:'
  DO xmlfile OVER wintestArray
    -- say xmlfile
-- store away original
    res = SysFileCopy(xmlfile, xmlfile~changeStr('.xml','.bup'))

-- read the entire file
    qfile = .stream~new(xmlfile)
    qfile~open("read shared")
    myArray = qfile~arrayIn()
    qfile~close

-- and rewrite with newNeedle where needle is found
    qfile = .stream~new(xmlfile)
    res = qfile~open('write replace')

-- iterate over file and replace those part referring to needle with newneedle
    DO item OVER myArray
      SELECT
        WHEN item~caselessPos(needle6) > 0 THEN
        DO
          say 'found' needle6
          say 'replacing with' newneedle6
          res = qfile~lineOut(item~changeStr(needle6,newneedle6))
        END

        OTHERWISE  res = qfile~lineOut(item)
      END /* Select */
    END
    qfile~close
  END

/**** MacOS test jobs	****/

  say
  say mactestArray~items 'MacOS test jobs to process:'
  DO xmlfile OVER mactestArray
    -- say xmlfile
-- store away original
    res = SysFileCopy(xmlfile, xmlfile~changeStr('.xml','.bup'))

-- read the entire file
    qfile = .stream~new(xmlfile)
    qfile~open("read shared")
    myArray = qfile~arrayIn()
    qfile~close

-- and rewrite with newNeedle where needle is found
    qfile = .stream~new(xmlfile)
    res = qfile~open('write replace')

-- iterate over file and replace those part referring to needle with newneedle
    DO item OVER myArray
      SELECT
        WHEN item~caselessPos(needle6) > 0 THEN
        DO
          say 'found' needle6
          say 'replacing with' newneedle6
          res = qfile~lineOut(item~changeStr(needle6,newneedle6))
        END

        OTHERWISE  res = qfile~lineOut(item)
      END /* Select */
    END
    qfile~close
  END

/**** *Nix test jobs	****/

  say
  say nixtestArray~items '*Nix test jobs to process:'
  DO xmlfile OVER nixtestArray
    -- say xmlfile
-- store away original
    res = SysFileCopy(xmlfile, xmlfile~changeStr('.xml','.bup'))

-- read the entire file
    qfile = .stream~new(xmlfile)
    qfile~open("read shared")
    myArray = qfile~arrayIn()
    qfile~close

-- and rewrite with newNeedle where needle is found
    qfile = .stream~new(xmlfile)
    res = qfile~open('write replace')

-- iterate over file and replace those part referring to needle with newneedle
    DO item OVER myArray
      SELECT
        WHEN item~caselessPos(needle6) > 0 THEN
        DO
          say 'found' needle6
          say 'replacing with' newneedle6
          res = qfile~lineOut(item~changeStr(needle6,newneedle6))
        END

        OTHERWISE  res = qfile~lineOut(item)
      END /* Select */
    END
    qfile~close
  END

exit
