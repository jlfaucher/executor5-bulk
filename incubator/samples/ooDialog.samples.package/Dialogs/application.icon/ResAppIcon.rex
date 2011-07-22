/* Simple Dialog showing how to use an application icon.  In this example
 * the icon is located in the resource only DLL file: ApplicationIcon.dll.  The
 * symbolic ID for the icon is: IDI_APP_ICON which is defined in the appIcon.h
 * file.
 */

  dlg = .SimpleDialog~new("ApplicationIcon.dll", IDD_DIALOG1, , "appIcon.h")
  dlg~execute("SHOWTOP", IDI_APP_ICON)

::requires "ooDialog.cls"

::class 'SimpleDialog' subclass ResDialog


