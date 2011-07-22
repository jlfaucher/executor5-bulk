/* Simple Dialog showing how to use an application icon.  In this example
 * the icon is specified in the resource script file: ApplicationIcon.rc.  The
 * symbolic ID for the icon is: IDI_APP_ICON which is defined in the appIcon.h
 * file.
 */

  dlg = .SimpleDialog~new("ApplicationIcon.rc", IDD_DIALOG1, , "appIcon.h")
  dlg~Execute("SHOWTOP", IDI_APP_ICON)

::requires "OODWin32.cls"

::class 'SimpleDialog' subclass RcDialog inherit AdvancedControls MessageExtensions


