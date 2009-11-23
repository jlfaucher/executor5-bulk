
  dlg = .ImageDisplay~new
  if dlg~initCode = 0 then do
    dlg~create(30, 30, 370, 230, "The System Image Cache", , , "Tahoma")
    dlg~Execute("SHOWTOP")
    dlg~Deinstall
  end

-- End of entry point.

::requires "OODWIN32.CLS"
::requires 'WinShell.cls'

::class ImageDisplay subclass UserDialog inherit AdvancedControls

::method defineDialog

  -- Use SHAREIMAGES so that the list-view control does not destroy the image
  -- list.
  self~addListControl(100, , 10, 10, 350, 185, "ICON SINGLESEL SHAREIMAGES");
  self~addButton(101, 330, 205, 30, 15, "Close", close, "DEFAULT")

  self~addText(15, 202, 80, 10, "", , 102)
  self~addText(15, 217, 80, 10, "", , 103)
  self~addText(105, 202, 180, 10, "", , 104)

::method initDialog
  expose list

  list  = self~getListControl(100)

  -- Create an image list from the system icon cache.
  imageList = .ImageList~new("SystemLarge")

  count = imageList~getImageCount
  size = imageList~getImageSize
  backGroundColor = imageList~getBkColor(.true)

  txt = 'Image count:' count
  self~getStaticControl(102)~title = txt

  txt = 'Image size: ' size~word(1) 'x' size~word(2)
  self~getStaticControl(103)~title = txt

  txt = 'Image background color R:' backGroundColor~word(1) 'G:' backGroundColor~word(2) -
       'B:' backGroundColor~word(3) '(' || backGroundColor~word(4) || ')'
  self~getStaticControl(104)~title = txt

  imageList~setListViewImages(list~hwnd, LVSIL_NORMAL)

  do i = 0 to (count - 1)
    list~add(i, i)
  end


::method close
  return self~ok:super


