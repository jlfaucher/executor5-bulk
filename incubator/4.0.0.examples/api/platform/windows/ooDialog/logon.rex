use arg iteration

  dlg = .LogonDialog~new

  dlg~createCenter(100, 45, "Logon", "VISIBLE NOMENU SYSTEMMODAL")
  ret = dlg~execute

  if ret == 1 then say "The user is granted access to the system."
  else if ret == 2 then say "The user canceled, does not want to log on."
  else say "An unexpected error happened."

  if iteration~datatype('W') then ret = iteration

return ret
-- End of entry point.

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*\
  Directives, Classes, or Routines.
\* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
::requires "oodPlain.cls"

::class 'LogonDialog' public subclass PlainUserDialog

::method defineDialog

  self~addText(5, 5, 85, 10, "Do you want to log on?")
  self~AddOkCancelRightBottom

/* - - - End Of File: logon.rex - - - - - - - - - - - - - - - - - - - - - - - */
