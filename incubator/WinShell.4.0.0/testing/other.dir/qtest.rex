

  fileOp = .ShellFileOp~new
  say 'got fileOp:' fileOp

  n = fileOp~myTest("Mark")
  say n

  say 'Call the private method:' fileOp~name("Tom")

::requires 'WinShell.cls'
