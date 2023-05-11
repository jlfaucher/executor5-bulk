/*
 * Build a compressed dmg file containing a Mac app
 * based on create-dmg from Andrey Tarantsov (andreyvit)
 *
 * Requires
 * - a packagename-volume.icns file containing multiple images based on
 *   common/volume-blank.png with the package icon overlaid
 * - common/rexxback-dmg.png
 * Arguments
 * - appname - name of app without .app extension; eg RxSafe
 * - volname - name of dmg; no extension eg RxSafe-1.1
 * - packagedir - location of package specific files
 * - commondir - location of common files
 */
Trace off
Parse Arg appname volname packagedir commondir
Call RxFuncAdd 'sysloadfuncs', 'regutil', 'sysloadfuncs'
Call SysLoadFuncs

If commondir = '' Then Call Abort 'Too few arguments. Requires 4'
srcdir = './'appname'.app'
dmg_name = volname'.dmg'
dmg_temp_name = './raw.'dmg_name
volume_icon_file = packagedir'/'appname'-volume.icns'
background_image = commondir'/rexxback-dmg.png'

If Stream( srcdir, 'C', 'QUERY EXISTS' ) = '' Then Call Abort 'No app in current directory:' srcdir
If Stream( packagedir, 'C', 'QUERY EXISTS' ) = '' Then Call Abort 'Package Directory:' packagedir 'does not exist'
If Stream( volume_icon_file, 'C', 'QUERY EXISTS' ) = '' Then Call Abort 'Volume .icns file:' volume_icon_file 'does not exist'
If Stream( background_image, 'C', 'QUERY EXISTS' ) = '' Then Call Abort 'Background image file:' background_image 'does not exist'
-- remove any .DS_Store in source directory (appname.app)
tmp = srcdir'/.DS_Store'
Say 'Deleting' tmp
'rm -fr' tmp

-- Create the image
Say "Creating disk image..."
'rm -fr' dmg_temp_name dmg_name
Address System "du -sm" appname'.app' "| sed -e 's/	.*//g'" With Output FIFO ''
If rc \= 0 Then Abort 'Getting size of source'
Parse Pull actual_size
disk_image_size = actual_size + 20 -- 20mb larger than actual size
cmd = 'hdiutil create -srcfolder' srcdir '-volname' volname '-fs HFS+ -fsargs "-c c=64,a=16,e=16" -format UDRW -size' disk_image_size'm' dmg_temp_name
Address System cmd
If rc \= 0 Then Abort 'Creating disk image'

-- mount temporary image
mount_dir='/Volumes/'volname
Say 'Mount directory:' mount_dir
Address System 'hdiutil attach -readwrite -noverify -noautoopen' dmg_temp_name With Output FIFO ''
If rc \= 0 Then Abort 'Mounting' mount_dir
Do Queued()
   Parse Pull line
   If Countstr( mount_dir, line ) = 1 Then Parse Var line dev_name .
End
Say 'Device name:' dev_name

-- copy image files and make Applications link
Say "Copying background file..."
'mkdir -p' mount_dir'/.background'
'cp' background_image mount_dir'/.background/'

Say "Making link to Applications dir"
'ln -s /Applications' mount_dir'/Applications'

Say 'Copying volume icon file' volume_icon_file
'cp' volume_icon_file mount_dir'/.VolumeIcon.icns'
'SetFile -c icnC' mount_dir'/.VolumeIcon.icns'
say 'Line:' .line rc

tmpfile = SysTempFileName( 'buildDMG?????.applescript' )
Call GenerateApplescript tmpfile, appname

Say 'Running Applescript: osascript' tmpfile volname
'osascript' tmpfile volname
If rc \= 0 Then Abort 'Running AppleScript:' tmpfile
Say "Done running the applescript..."
Call Sleep 4

Call SysFileDelete tmpfile

-- make sure it's not world writeable
Say "Fixing permissions..."
'chmod -Rf go-w' mount_dir '> /dev/null 2>&1'
Say "Done fixing permissions."

-- tell the volume that it has a special file attribute
'SetFile -a C' mount_dir
If rc \= 0 Then Abort 'Setting volume icons'

Say "Unmounting disk image..."
'hdiutil detach' dev_name
If rc \= 0 Then Abort 'Unmounting disk image'

-- compress image
Say "Compressing disk image..."
'hdiutil convert' dmg_temp_name '-format UDZO -imagekey zlib-level=9 -o' dmg_name
If rc \= 0 Then Abort 'Compressing disk image'
'rm -f' dmg_temp_name

Say dmg_name 'created successfully!!'
Say 'Signing disk image...'
Address System 'codesign --verify --verbose --sign "Developer ID Application: Mark Hessling"' dmg_name
Return 0

GenerateAppleScript: Procedure
Parse Arg fn, appname
start = .LINE + 2
/*
on run (volumeName)
   tell application "Finder"
      tell disk (volumeName as string)
         open

         set theXOrigin to 100
         set theYOrigin to 100
         set theWidth to 512
         set theHeight to 256

         set theBottomRightX to (theXOrigin + theWidth)
         set theBottomRightY to (theYOrigin + theHeight)
         set dsStore to "\"" & "/Volumes/" & volumeName & "/" & ".DS_STORE\""

         tell container window
            set current view to icon view
            set toolbar visible to false
            set statusbar visible to false
            set the bounds to {theXOrigin, theYOrigin, theBottomRightX, theBottomRightY}
            set statusbar visible to false
         end tell

         set opts to the icon view options of container window
         tell opts
            set icon size to 100
            set arrangement to not arranged
         end tell
         set background picture of opts to file ".background:rexxback-dmg.png"

         -- Positioning; coords are based on centre of icon
         set position of item "%APPNAME%" to {154, 128}

         -- Application Link Clause; coords are based on centre of icon
         set position of item "Applications" to {358, 128}
            close
            open

         update without registering applications
         -- Force saving of the size
         delay 1

         tell container window
            set statusbar visible to false
            set the bounds to {theXOrigin, theYOrigin, theBottomRightX - 10, theBottomRightY - 10}
         end tell

         update without registering applications
      end tell

      delay 1

      tell disk (volumeName as string)
         tell container window
            set statusbar visible to false
            set the bounds to {theXOrigin, theYOrigin, theBottomRightX, theBottomRightY}
         end tell

         update without registering applications
      end tell

      --give the finder some time to write the .DS_Store file
      delay 3

      set waitTime to 0
      set ejectMe to false
      repeat while ejectMe is false
         delay 1
         set waitTime to waitTime + 1

         if (do shell script "[ -f " & dsStore & " ]; echo $?") = "0" then set ejectMe to true
      end repeat
      log "waited " & waitTime & " seconds for .DS_STORE to be created."
   end tell
end run
 */
end = .LINE - 2
Call Stream fn, 'C', 'OPEN WRITE REPLACE'
Do i = start To end
   line = Changestr( '%APPNAME%', SourceLine(i), appname )
   Call Lineout fn, line
End
Call Stream fn, 'C', 'CLOSE'
Return

Abort: Procedure
Parse Arg msg
Say 'Aborting:' msg
Exit 1
