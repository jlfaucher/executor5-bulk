parse source . . s
say "  req02.rex: in prolog code of" pp(getFileName(s))
call sysSleep .1
.local~req02=.DateTime~new~string

::routine getFileName public
  parse arg fn
  return filespec("Name", fn)

::routine pp
  parse arg a
  return "["a"]"
