/* Demonstrates that the fromList and toList in the ShellFileOp class can not
 * be set by the programmer.  The attributes are defined in WinShell.cls as:
 *
 *  ::attribute fromList get
 *  ::attribute toList   get
 *
 * There is no way to do:  obj~fromList = <someValue>, even by subclassing.
 */

  flags = "FOF_SILENT FOF_NOCONFIRMATION FOF_ALLOWUNDO FOF_NOCONFIRMMKDIR FOF_NOERRORUI"
  fo =  .MarkFileOp~new("my.test", "other.dir", flags)

  say 'from list:' fo~fromList
  if fo~fromList \== .nil then do
    say 'length:   ' fo~fromList~length
    say 'last char:' fo~fromList~right(1)~c2x
    say '2nd last: ' fo~fromList~right(2)~left(1)~c2x
  end
  say 'to list:' fo~toList

  -- Does not work, the desired behavior:
  --  fo~fromList = 'dog'

  say
  say
  say
  say 'TRY IT'
  say
  fo~trySomeThing
  say
  say
  say
  say 'NOW WHAT IS FROM LIST ????'
  say 'from list:' fo~fromList

  ret = fo~move
  if ret then success = "true"
  else success = "false"
  say 'success:' success

::requires 'WinShell.cls'

::class 'MarkFileOp' public subclass ShellFileOp

::method trySomeThing
  say 'in try some thing'
  say 'from list again:' self~fromList
  say 'make from list a dog:'

  -- Does not work, which is the desired behavior.
  self~fromList = "dog"
