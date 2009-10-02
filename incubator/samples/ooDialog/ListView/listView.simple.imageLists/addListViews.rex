
  dlg = .ListViews~new("iconList.rc", IDD_DIALOG, , "iconList.h")
  if dlg~initCode == 0 then do
    dlg~Execute("SHOWTOP", IDI_DLG_OOREXX)
    dlg~Deinstall
  end

-- End of entry point.

::requires "OODWIN32.CLS"

::class 'ListViews' subclass RcDialog inherit AdvancedControls MessageExtensions

::method initDialog
  expose list

  self~getRadioControl(IDC_RB_ICON)~check
  self~connectButtonNotify(IDC_RB_REPORT, "CLICKED", onReport)
  self~connectButtonNotify(IDC_RB_LIST, "CLICKED", onList)
  self~connectButtonNotify(IDC_RB_ICON, "CLICKED", onIcon)
  self~connectButtonNotify(IDC_RB_SMALL_ICON, "CLICKED", onSmallIcon)

  list  = self~getListControl(IDC_LV_VIEWS)

  flags = .Image~toID(ILC_COLOR24)
  imageList = .ImageList~create(.Size~new(16), flags, 7, 0)

  imageList~add(.Image~getImage("iconList_16.bmp"))
  list~setImageList(imageList, .Image~toID(LVSIL_SMALL))

  imageList = .ImageList~create(.Size~new(32), flags, 7, 0)

  imageList~add(.Image~getImage("iconList_32.bmp"))
  list~setImageList(imageList, .Image~toID(LVSIL_NORMAL))

  self~populateList(list)

::method onReport
  expose list
  list~replaceStyle("LIST ICON SMALLICON", "REPORT")

::method onList
  expose list
  list~replaceStyle("REPORT ICON SMALLICON", "LIST")

::method onIcon
  expose list
  list~replaceStyle("LIST REPORT SMALLICON", "ICON")

::method onSmallIcon
  expose list
  list~replaceStyle("LIST ICON REPORT", "SMALLICON")

::method populateList private
  use strict arg list

  list~insertColumn(0, "Title", 70)
  list~insertColumn(1, "Name", 35)
  list~insertColumn(2, "Last", 50)
  list~insertColumn(3, "e-mail", 70)

  list~addRow(0, 6, "Business Manager",   "Tom",    "Saywer",   "ts@google.com" )
  list~addRow(1, 1, "Software Developer", "Sam",    "Frank",    "boo@gmail.com")
  list~addRow(2, 7, "Accountant",         "Cienna", "Smith",    "cs@yahoo.com")
  list~addRow(3, 5, "Lawyer",             "Mary",   "Tyler",    "fkan@qualcom.com")
  list~addRow(4, 2, "Doctor",             "Sue",    "Acer",     "sa@sharp.org")
  list~addRow(5, 3, "Clerk",              "Harry",  "Houdini",  "HH@magic.net")
  list~addRow(6, 4, "Nurse",              "Mike",   "Thompson", "mike@microsoft.com")

  list~addExtendedStyle("FULLROWSELECT UNDERLINEHOT ONECLICKACTIVATE")

