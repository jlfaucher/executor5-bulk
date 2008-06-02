
  title = "ooRexx Windows Shell for ooDialog Programmers"
  app = "Vail Miesfeld's Icon Display"
  other = "Icons are the intellectual propertey of Vail Miesfeld."

  shell = .WinShell~new

  iconDll = "C:\WINDOWS\system32\shell32.dll"
  --iconDll = .Path~searchAndQualify("vIcons.dll")

  iconDetails = shell~selectIcon(iconDll)
  if iconDetails == .nil then do
    say 'got back .nil'
    ret = shell~about(title, app, other)
    say 'got (no icon) back:' ret
  end
  else do
    say 'Got back icon:'
    say '  Index:' iconDetails[1]
    say '  Path: ' iconDetails[2]

    /*
    --hIcon = shell~loadIcon(iconDetails[2], iconDetails[1])
    icons = shell~extractDefaultIcons(iconDetails[2], iconDetails[1])
    --say 'load icon:' hIcon
    say 'icons:' icons
    if icons~isA(.array) then do
      say 'big:  ' icons[1]
      say 'small:' icons[2]
      ret = shell~about(title, app, other, , icons[1])
      say 'got (with icon) back:' ret
      ret = shell~about(title, app, other, , icons[2])
      say 'got (with icon) back:' ret

      ret = shell~releaseIcon(icons[1])
      say 'released icon:' ret
      ret = shell~releaseIcon(icons[2])
      say 'released icon:' ret
    end
    */
    icons = shell~extractDefaultIcons(iconDetails[2])
    say 'got icons:' icons
    say 'count:    ' icons~items

    largeIcons = icons[1]
    smallIcons = icons[2]
    himl = shell~createImageList(32, 32, largeIcons)
    say 'got image list:' himl
    pull
    do icon over largeIcons
      say 'hLarge:' icon
      shell~releaseIcon(icon)
    end
    do icon over smallIcons
      say 'hSmall:' icon
      shell~releaseIcon(icon)
    end
  end

::requires 'WinShell.cls'
