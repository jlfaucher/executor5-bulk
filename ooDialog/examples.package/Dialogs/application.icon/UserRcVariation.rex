/* Simple Dialog showing how to use an application icon.  In this example
 * the icon is specified by the addIcon() method, called right after the dialog
 * object is instantiated.
 */

  -- The dialog template is loaded from a resource script file, ApplicationIcon.rc
  -- In the appIcon.h file are defines for the symbolic resource IDs used in this
  -- program.
  --
  -- The custom dialog is loaded after creating the dialog object.

  dlg = .SimpleDialog~new("ApplicationIcon.rc", IDD_DIALOG1, , "appIcon.h")
  dlg~addIcon(IDI_CUSTOM_ICON, "Question32.ico")

  dlg~execute("SHOWTOP", IDI_CUSTOM_ICON)

::requires "OODWin32.cls"

::class 'SimpleDialog' subclass RcDialog inherit AdvancedControls MessageExtensions


