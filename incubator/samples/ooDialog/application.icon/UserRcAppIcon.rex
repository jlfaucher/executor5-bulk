/* Simple Dialog showing how to use an application icon.  In this example
 * the icon is specified by the addIcon() method in the defineDialog()
 * method of the RcDialog subclass.
 */

  -- The dialog template is loaded from a resource script file, ApplicationIcon.rc
  -- In the appIcon.h file are defines for the symbolic resource IDs used in this
  -- program.

  dlg = .SimpleDialog~new("ApplicationIcon.rc", IDD_DIALOG1, , "appIcon.h")
  dlg~Execute("SHOWTOP", IDI_CUSTOM_ICON)

::requires "OODWin32.cls"

::class 'SimpleDialog' subclass RcDialog inherit AdvancedControls MessageExtensions

::method defineDialog
  -- Have ooDialog load the application icon used for this program.  This must
  -- done before the underlying dialog is created.  So, we do it here in
  -- defineDialog().

  self~addIcon(IDI_CUSTOM_ICON, "Question32.ico")

