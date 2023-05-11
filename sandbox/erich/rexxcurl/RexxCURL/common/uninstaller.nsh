  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${REGISTRYNAME}" "DisplayName" "${LONGNAME}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${REGISTRYNAME}" "DisplayIcon" "${DISPLAYICON}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${REGISTRYNAME}" "HelpLink" "${SFHOME}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${REGISTRYNAME}" "URLUpdateInfo" "http://sourceforge.net/project/showfiles.php?group_id=${SFGROUP}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${REGISTRYNAME}" "URLInfoAbout" "http://www.rexx.org/"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${REGISTRYNAME}" "DisplayVersion" "${VERSION}"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${REGISTRYNAME}" "Publisher" "Mark Hessling"
  WriteRegExpandStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${REGISTRYNAME}" "UninstallString" '"$INSTDIR\${UNINSTALLER}"'
  WriteRegExpandStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${REGISTRYNAME}" "InstallLocation" '"$INSTDIR"'
  WriteRegExpandStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${REGISTRYNAME}" "UnInstallLocation" "$INSTDIR" ; dont quote it
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${REGISTRYNAME}" "NoModify" 0x00000001
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\${REGISTRYNAME}" "NoRepair" 0x00000001
  WriteUninstaller "$INSTDIR\${UNINSTALLER}"
