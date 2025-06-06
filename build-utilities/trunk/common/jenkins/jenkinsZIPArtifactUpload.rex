-- Upload built ooRexx zipped USB installers to Sourceforge

-- ooRexx is built on https://build.oorexx.org, running on P.O.'s Ubuntu Controller (master)
-- ooRexx build jobs use the Jenkins post-build Action "archive the artifacts"
-- to move their built .deb/.rpm/.exe files to the master
-- here the zipped USB items are picked up
-- the master receives these artifacts in /var/lib/jenkins/jobs/...
-- e.g. /var/lib/jenkins/jobs/ooRexx-Z-build/builds/lastSuccessfulBuild/archive/oorexxBuild/ooRexx-5.0.0-11798.sles12.s390x.rpm

/* Issues/ToDo
if there are multiple builds (eg. started manually) with the same SVN, just one should be used
rm .. may be wrong; more than five may be left

Done
In order to enable dynamic changes of upload path make sourceforgeFiles an input variable
to be given in in Project on Jenkins; ->
1st parameter = folder on sourceforge
2nd parameter = what version to upload (will prevent uploading 5.1.0 files for 5.2.0 etc
3rd parameter = minimum revision -> will prevent upload of older files during release
*/

/*
   usage:   jenkinsZIPArtifactUpload.rex [targetDirectory [version]]
            jenkinsZIPArtifactUpload.rex 5.2.0beta 5.2.0
*/

trace o 

-- default (fallback) values
tgtDirName  ="portable"    
version     ="5.2.0"       -- ooRexx version name
tgtDirectory=version"beta/"tgtDirName
minimumRevision. = 0

if .sysCargs~items>0 then  -- command line arguments supplied?
do
    tgtDirectory=.sysCargs[1]"/"tgtDirName
    if .sysCargs~items>1 then
    do
        version=.sysCargs[2]
        if .sysCargs~items>2 then
        if verify(.sysCargs[3],"0123456789") = 0 then
            minimumRevision. = .sysCargs[3]
        else   -- version looks wrong: report and abend
        do
            .error~say(.context~name .line": 3rd parameter" .sysCargs[3] "not numeric,aborting")
            exit -1
        end
    end
    else -- version not given, extract version from tgtDirectory
    do
        pos=verify(tgtDirectory,"0123456789.")
        if pos>1 then
           version=tgtDirectory~left(pos-1)
        else if pos=0 then
           version=tgtDirectory   -- tgtDirectory is same as version
        else   -- version looks wrong: report and abend
        do
            .error~say(.context~name .line": no version supplied and tgtDirectory" tgtDirectory "does not begin with a valid version number, aborting")
            exit -1
        end
    end
end

pkgLocal=.context~package~local
pkgLocal~ooRexx.id="ooRexx-"version"-" -- define the needle the filename must start with
pkgLocal~bDebug   =.true               -- set to .false to stop debug output

if .bDebug then .error~say("*** debug #".line~right(3) "- version:" pp(version) "tgtDirectory:" pp(tgtDirectory))

-- we use Erichs credential on the Jenkins controller
sourceforgeUrl = "frs.sourceforge.net"
sourceforgeFiles = "/home/frs/project/oorexx/oorexx/"tgtDirectory
if .bDebug then .error~say("*** debug #".line~right(3) "- sourceforgeFiles:" pp(sourceforgeFiles))

sourceforgeUser = "erich_st"
sourceforge = sourceforgeUser"@"sourceforgeUrl":"sourceforgeFiles

-- step 1.  check which installers are already available on sourceforge
uploadedFiles = .Array~new
address "" "sftp" sourceforge with input using ("ls -1", "quit") output using (uploadedFiles)
if rc \= 0 then do
  say "return code" rc "from sftp"
  say uploadedFiles
end
say uploadedFiles~items "lines output from sftp" sourceforgeFiles
existingBuilds = .Relation~new
do file over uploadedFiles
  build = .Artifact~new(file)
  if build~validExtension, build~ooRexx, build~revision~dataType("Whole") then
    existingBuilds[build~platform] = build
end
say existingBuilds~items "uploaded installers available"
if existingBuilds~items > 0 then do
  say " build platform               uploaded revisions"
  say " ----------------------------------------------------------------"
end
do platform over existingBuilds~uniqueIndexes~sort
  allBuilds = existingBuilds~allAt(platform)~sort
  minimumRevision.platform = allBuilds[allBuilds~last]
  say "" platform~left(28~max(platform~length)) allBuilds~toString(, " ")
end

-- step 2.  search Jenkins local subdirectories for artifacts
artifactPath = "/var/lib/jenkins/jobs/ooRexx*"
call SysFileTree artifactPath, files., "fsoi" -- ignore case, file name only
say
say files.0 "files/links found in" artifactPath

successful = .Array~new
do file over files.~allItems
  artifact = .Artifact~new(file)
  if artifact~lastSuccessful, artifact~validExtension then
    successful~append(artifact)
end
say successful~items "of those are" .Artifact~successfulMarker || "###"

-- step 3.  figure out which artifacts should be uploaded
toBeUploaded = .Array~new
if successful~items > 0 then do
  say " action                       file"
  say "" "-"~copies(78)
end

do artifact over successful

-- say 'artifact' artifact 
-- say 'artifact~ooRexx' artifact~ooRexx
-- say 'artifact~name' artifact~name
-- say 'artifact~revision~dataType("Whole")' artifact~revision~dataType("Whole")
-- say 'artifact~revision' artifact~revision
-- say 'minimumRevision.[artifact~platform]' minimumRevision.[artifact~platform]

  select
    when \artifact~ooRexx then
      say "" "cannot identify name"~left(28) artifact~name
    when \artifact~revision~dataType("Whole") then
      say "" "cannot identify revision"~left(28) artifact~name
    when artifact~revision >= minimumRevision.[artifact~platform] then do
      say "" "to be uploaded"~left(28) artifact~name
      toBeUploaded~append(artifact)
    end
    otherwise
      say "" "do not upload"~left(28) artifact~name
  end
end

-- step 4.  upload new installers and remove the oldest ones
-- we're keeping a maximum of 5 installers per platform on sourceforge
say
code = 0
uploadAndRemove = .Array~new
if toBeUploaded~items > 0 then
  say "uploading to" sourceforgeFiles
do artifact over toBeUploaded
  uploadAndRemove~empty
  allPlatform = existingBuilds~allAt(artifact~platform)~sort
  -- we run sftp in batch mode: first add a "put" command to upload
  uploadAndRemove~append("put" artifact~filename)
  -- we remove as many outdated builds as necessary to keep the total
  -- number of installers per platform to five
  toBeRemoved = allPlatform~items - 4
  select
    when toBeRemoved < 1 then
      remove = ""
    when toBeRemoved = 1 then
      remove = "and remove old build" allPlatform[1]
    when toBeRemoved > 1 then
      remove = "and rm" toBeRemoved "old builds" allPlatform~section(1, toBeRemoved)~toString(, ", ")
  end
  say
  say "sftp put" artifact~name remove
  do build over allPlatform for 0~max(toBeRemoved)
    -- optionally add "rm" commands to remove outdated builds
    uploadAndRemove~append("rm" build~filename)
  end
  -- "quit" will end sftp, but any "put" or "rm" failures will end it prematurely
  uploadAndRemove~append("quit")
  address "" "sftp" sourceforge with input using (uploadAndRemove)
  code = code~max(rc) -- we'll exit with the maximum rc received
end

exit code


::class Artifact
-- Jenkins artifacts may be available through various aliases/soft links (see above)
-- although this currently isn't true, we want to make sure we upload only one file
-- we consider it only if its path contains below marker, followed by a numeric build number
::constant successfulMarker "/builds/"

-- also we consider an artifact only if it has one of these extensions
::constant validExtensions ("zip")

-- in addition, for an artifact to qualify as an ooRexx installer,
-- its filenames must start with this identifier
-- use .ooRexx.id instead of:
-- ::constant ooRexxId "ooRexx-5.1.0-"

::attribute filename get
::attribute name get
::attribute lastSuccessful get
::attribute ooRexx
::attribute validExtension get
::attribute revision get
::attribute platform get

::method init
  expose filename name lastSuccessful ooRexx validExtension revision platform
  use strict arg filename
  name = filespec("name", filename)
  -- e. g. /var/lib/jenkins/jobs/ooRexx-macOS10-build/builds/48/archive/oorexxBuild/ooRexx-5.0.1-darwin64-r12617-universal-portable-release.zip
  parse var filename (self~successfulMarker) buildNr "/"
  lastSuccessful = buildNr~dataType("W")
  ooRexx = name~caselessStartsWith(.ooRexx.Id)
  validExtension = .false
  do extension over self~validExtensions
    validExtension |= filename~caselessEndsWith(extension)
  end

  -- installer names must have a distinct format
  -- e. g. ooRexx-5.1.0-darwin64-r12617-universal-portable-release.zip
  -- since 2024-01-05:
  -- e. g. ooRexx-5.1.0-12616.windows.x86_64-portable-release
  parse var name +(.ooRexx.Id~length) revision "." platform

  -- allow Array toString() to give readable output
  self~objectName = revision

-- allow Artifact instances to be sorted by revision
::method compareTo
  expose revision
  use strict arg other
  return revision~compareTo(other~revision)

::routine pp
  return "["arg(1)"]"

::options novalue error

