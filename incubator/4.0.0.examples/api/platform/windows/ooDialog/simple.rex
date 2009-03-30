parse arg a
parse source . . s
say "  simple.rex: in" pp(getFileName(s))", received arg:" pp(a)
call sysSleep .1
return "returning from ooRexx: <"a">"


::requires req01.rex

