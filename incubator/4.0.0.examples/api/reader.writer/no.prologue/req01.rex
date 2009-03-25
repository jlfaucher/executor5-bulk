
parse source . . s
say "  req01.rex: in prolog code of" pp(getFileName(s))
call sysSleep .1
.local~req01=.DateTime~new~string

::requires req02.rex


::routine pp public
  parse arg a
  return "["a"]"
